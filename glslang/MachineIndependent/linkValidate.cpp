//
//Copyright (C) 2013 LunarG, Inc.
//
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//
//    Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the following
//    disclaimer in the documentation and/or other materials provided
//    with the distribution.
//
//    Neither the name of 3Dlabs Inc. Ltd. nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
//COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
//BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//POSSIBILITY OF SUCH DAMAGE.
//

//
// Do link-time merging and validation of intermediate representations.
//
// Basic model is that during compilation, each compilation unit (shader) is
// compiled into one TIntermediate instance.  Then, at link time, multiple
// units for the same stage can be merged together, which can generate errors.
// Then, after all merging, a single instance of TIntermediate represents
// the whole stage.  A final error check can be done on the resulting stage,
// even if no merging was done (i.e., the stage was only one compilation unit).
//

#include "localintermediate.h"
#include "../Include/InfoSink.h"

namespace glslang {
    
//
// Link-time error emitter.
//
void TIntermediate::error(TInfoSink& infoSink, const char* message)
{
    infoSink.info.prefix(EPrefixError);
    infoSink.info << "Linking " << StageName(language) << " stage: " << message << "\n";

    ++numErrors;
}

//
// Merge the information from 'unit' into 'this'
//
void TIntermediate::merge(TInfoSink& infoSink, TIntermediate& unit)
{
    numMains += unit.numMains;
    numErrors += unit.numErrors;
    callGraph.insert(callGraph.end(), unit.callGraph.begin(), unit.callGraph.end());

    if ((profile != EEsProfile && unit.profile == EEsProfile) ||
        (profile == EEsProfile && unit.profile != EEsProfile))
        error(infoSink, "Cannot mix ES profile with non-ES profile shaders\n");

    if (originUpperLeft != unit.originUpperLeft || pixelCenterInteger != unit.pixelCenterInteger)
        error(infoSink, "gl_FragCoord redeclarations must match across shaders\n");

    if (inputPrimitive == ElgNone)
        inputPrimitive = unit.inputPrimitive;
    else if (inputPrimitive != unit.inputPrimitive)
        error(infoSink, "Contradictory input layout primitives");
    
    if (outputPrimitive == ElgNone)
        outputPrimitive = unit.outputPrimitive;
    else if (outputPrimitive != unit.outputPrimitive)
        error(infoSink, "Contradictory output layout primitives");
    
    if (vertices == 0)
        vertices = unit.vertices;
    else if (vertices != unit.vertices) {
        if (language == EShLangGeometry)
            error(infoSink, "Contradictory layout max_vertices values");
        else if (language == EShLangTessControl)
            error(infoSink, "Contradictory layout vertices values");
        else
            assert(0);
    }

    if (vertexSpacing == EvsNone)
        vertexSpacing = unit.vertexSpacing;
    else if (vertexSpacing != unit.vertexSpacing)
        error(infoSink, "Contradictory input vertex spacing");

    if (vertexOrder == EvoNone)
        vertexOrder = unit.vertexOrder;
    else if (vertexOrder != unit.vertexOrder)
        error(infoSink, "Contradictory triangle ordering");

    if (unit.pointMode)
        pointMode = true;

    if (unit.treeRoot == 0)
        return;

    if (treeRoot == 0) {
        version = unit.version;
        treeRoot = unit.treeRoot;
        return;
    } else
        version = std::max(version, unit.version);

    // Get the top-level globals of each unit
    TIntermSequence& globals = treeRoot->getAsAggregate()->getSequence();
    TIntermSequence& unitGlobals = unit.treeRoot->getAsAggregate()->getSequence();

    // Get the linker-object lists
    TIntermSequence& linkerObjects = findLinkerObjects();
    TIntermSequence& unitLinkerObjects = unit.findLinkerObjects();

    mergeBodies(infoSink, globals, unitGlobals);
    mergeLinkerObjects(infoSink, linkerObjects, unitLinkerObjects);

    ioAccessed.insert(unit.ioAccessed.begin(), unit.ioAccessed.end());
}

//
// Merge the function bodies and global-level initalizers from unitGlobals into globals.
// Will error check duplication of function bodies for the same signature.
//
void TIntermediate::mergeBodies(TInfoSink& infoSink, TIntermSequence& globals, const TIntermSequence& unitGlobals)
{
    // TODO: link-time performance: Processing in alphabetical order will be faster

    // Error check the global objects, not including the linker objects
    for (unsigned int child = 0; child < globals.size() - 1; ++child) {
        for (unsigned int unitChild = 0; unitChild < unitGlobals.size() - 1; ++unitChild) {
            TIntermAggregate* body = globals[child]->getAsAggregate();
            TIntermAggregate* unitBody = unitGlobals[unitChild]->getAsAggregate();
            if (body && unitBody && body->getOp() == EOpFunction && unitBody->getOp() == EOpFunction && body->getName() == unitBody->getName()) {
                error(infoSink, "Multiple function bodies in multiple compilation units for the same signature in the same stage:");
                infoSink.info << "    " << globals[child]->getAsAggregate()->getName() << "\n";
            }
        }
    }

    // Merge the global objects, just in front of the linker objects
    globals.insert(globals.end() - 1, unitGlobals.begin(), unitGlobals.end() - 1);
}

//
// Merge the linker objects from unitLinkerObjects into linkerObjects.
// Duplication is expected and filtered out, but contradictions are an error.
//
void TIntermediate::mergeLinkerObjects(TInfoSink& infoSink, TIntermSequence& linkerObjects, const TIntermSequence& unitLinkerObjects)
{
    // Error check and merge the linker objects (duplicates should not be merged)
    std::size_t initialNumLinkerObjects = linkerObjects.size();
    for (unsigned int unitLinkObj = 0; unitLinkObj < unitLinkerObjects.size(); ++unitLinkObj) {
        bool merge = true;
        for (std::size_t linkObj = 0; linkObj < initialNumLinkerObjects; ++linkObj) {
            TIntermSymbol* symbol = linkerObjects[linkObj]->getAsSymbolNode();
            TIntermSymbol* unitSymbol = unitLinkerObjects[unitLinkObj]->getAsSymbolNode();
            assert(symbol && unitSymbol);
            if (symbol->getName() == unitSymbol->getName()) {
                // filter out copy
                merge = false;

                // but if one has an initializer and the other does not, update
                // the initializer
                if (symbol->getConstArray().empty() && ! unitSymbol->getConstArray().empty())
                    symbol->setConstArray(unitSymbol->getConstArray());

                // Similarly for binding
                if (! symbol->getQualifier().hasBinding() && unitSymbol->getQualifier().hasBinding())
                    symbol->getQualifier().layoutBinding = unitSymbol->getQualifier().layoutBinding;
                
                // Check for consistent types/qualification/initializers etc.
                mergeErrorCheck(infoSink, *symbol, *unitSymbol, false);
            }
        }
        if (merge)
            linkerObjects.push_back(unitLinkerObjects[unitLinkObj]);
    }
}

//
// Compare two global objects from two compilation units and see if they match
// well enough.  Rules can be different for intra- vs. cross-stage matching.
//
// This function only does one of intra- or cross-stage matching per call.
//
void TIntermediate::mergeErrorCheck(TInfoSink& infoSink, const TIntermSymbol& symbol, const TIntermSymbol& unitSymbol, bool crossStage)
{
    bool writeTypeComparison = false;

    // Types have to match
    if (symbol.getType() != unitSymbol.getType()) {
        error(infoSink, "Types must match:");
        writeTypeComparison = true;
    }

    // Qualifiers have to (almost) match

    // Storage...
    if (symbol.getQualifier().storage != unitSymbol.getQualifier().storage) {
        error(infoSink, "Storage qualifiers must match:");
        writeTypeComparison = true;
    }

    // Precision...
    if (symbol.getQualifier().precision != unitSymbol.getQualifier().precision) {
        error(infoSink, "Precision qualifiers must match:");
        writeTypeComparison = true;
    }

    // Invariance...
    if (! crossStage && symbol.getQualifier().invariant != unitSymbol.getQualifier().invariant) {
        error(infoSink, "Presence of invariant qualifier must match:");
        writeTypeComparison = true;
    }

    // Auxiliary and interpolation...
    if (symbol.getQualifier().centroid  != unitSymbol.getQualifier().centroid ||
        symbol.getQualifier().smooth    != unitSymbol.getQualifier().smooth ||
        symbol.getQualifier().flat      != unitSymbol.getQualifier().flat ||
        symbol.getQualifier().sample    != unitSymbol.getQualifier().sample ||
        symbol.getQualifier().patch     != unitSymbol.getQualifier().patch ||
        symbol.getQualifier().nopersp   != unitSymbol.getQualifier().nopersp) {
        error(infoSink, "Interpolation and auxiliary storage qualifiers must match:");
        writeTypeComparison = true;
    }

    // Memory...
    if (symbol.getQualifier().shared    != unitSymbol.getQualifier().shared ||
        symbol.getQualifier().coherent  != unitSymbol.getQualifier().coherent ||
        symbol.getQualifier().volatil   != unitSymbol.getQualifier().volatil ||
        symbol.getQualifier().restrict  != unitSymbol.getQualifier().restrict ||
        symbol.getQualifier().readonly  != unitSymbol.getQualifier().readonly ||
        symbol.getQualifier().writeonly != unitSymbol.getQualifier().writeonly) {
        error(infoSink, "Memory qualifiers must match:");
        writeTypeComparison = true;
    }

    // Layouts... 
    // TODO: 4.4 enhanced layouts: generalize to include offset/align
    if (symbol.getQualifier().layoutMatrix   != unitSymbol.getQualifier().layoutMatrix ||
        symbol.getQualifier().layoutPacking  != unitSymbol.getQualifier().layoutPacking ||
        symbol.getQualifier().layoutLocation != unitSymbol.getQualifier().layoutLocation ||
        symbol.getQualifier().layoutBinding  != unitSymbol.getQualifier().layoutBinding) {
        error(infoSink, "Layout qualification must match:");
        writeTypeComparison = true;
    }

    // Initializers have to match, if both are present, and if we don't already know the types don't match
    if (! writeTypeComparison) {
        if (! symbol.getConstArray().empty() && ! unitSymbol.getConstArray().empty()) {
            if (symbol.getConstArray() != unitSymbol.getConstArray()) {
                error(infoSink, "Initializers must match:");
                infoSink.info << "    " << symbol.getName() << "\n";
            }
        }
    }

    if (writeTypeComparison)
        infoSink.info << "    " << symbol.getName() << ": \"" << symbol.getType().getCompleteString() << "\" versus \"" <<
                                                             unitSymbol.getType().getCompleteString() << "\"\n";
}

//
// Do final link-time error checking of a complete (merged) intermediate representation.
// (Much error checking was done during merging).
//
// Also, lock in defaults of things not set.
//
void TIntermediate::finalCheck(TInfoSink& infoSink)
{   
    if (numMains < 1)
        error(infoSink, "Missing entry point: Each stage requires one \"void main()\" entry point");

    // recursion checking
    checkCallGraphCycles(infoSink);

    // overlap/alias/missing I/O, etc.
    inOutLocationCheck(infoSink);

    if (inIoAccessed("gl_ClipDistance") && inIoAccessed("gl_ClipVertex"))
        error(infoSink, "Can only use one of gl_ClipDistance or gl_ClipVertex (gl_ClipDistance is preferred)");

    if (userOutputUsed() && (inIoAccessed("gl_FragColor") || inIoAccessed("gl_FragData")))
        error(infoSink, "Cannot use gl_FragColor or gl_FragData when using user-defined outputs");
    if (inIoAccessed("gl_FragColor") && inIoAccessed("gl_FragData"))
        error(infoSink, "Cannot use both gl_FragColor and gl_FragData");

    switch (language) {
    case EShLangVertex:
        break;
    case EShLangTessControl:
        if (vertices == 0)
            error(infoSink, "At least one shader must specify an output layout(vertices=...)");
        break;
    case EShLangTessEvaluation:
        if (inputPrimitive == ElgNone)
            error(infoSink, "At least one shader must specify an input layout primitive");
        if (vertexSpacing == EvsNone)
            vertexSpacing = EvsEqual;
        if (vertexOrder == EvoNone)
            vertexOrder = EvoCcw;
        break;
    case EShLangGeometry:
        if (inputPrimitive == ElgNone)
            error(infoSink, "At least one shader must specify an input layout primitive");
        if (outputPrimitive == ElgNone)
            error(infoSink, "At least one shader must specify an output layout primitive");
        if (vertices == 0)
            error(infoSink, "At least one shader must specify a layout(max_vertices = value)");
        break;
    case EShLangFragment:
        break;
    case EShLangCompute:
        break;
    }
}

//
// See if the call graph contains any static recursion, which is disallowed
// by the specification.
//
void TIntermediate::checkCallGraphCycles(TInfoSink& infoSink)
{
    // Reset everything, once.
    for (TGraph::iterator call = callGraph.begin(); call != callGraph.end(); ++call) {
        call->visited = false;
        call->currentPath = false;
        call->errorGiven = false;
    }

    //
    // Loop, looking for a new connected subgraph.  One subgraph is handled per loop iteration.
    //

    TCall* newRoot;
    do {
        // See if we have unvisited parts of the graph.
        newRoot = 0;
        for (TGraph::iterator call = callGraph.begin(); call != callGraph.end(); ++call) {
            if (! call->visited) {
                newRoot = &(*call);
                break;
            }
        }

        // If not, we are done.
        if (! newRoot)
            break;

        // Otherwise, we found a new subgraph, process it:
        // See what all can be reached by this new root, and if any of 
        // that is recursive.  This is done by depth-first traversals, seeing
        // if a new call is found that was already in the currentPath (a back edge),
        // thereby detecting recursion.
        std::list<TCall*> stack;
        newRoot->currentPath = true; // currentPath will be true iff it is on the stack
        stack.push_back(newRoot);
        while (! stack.empty()) {
            // get a caller
            TCall* call = stack.back();

            // Add to the stack just one callee.
            // This algorithm always terminates, because only !visited and !currentPath causes a push
            // and all pushes change currentPath to true, and all pops change visited to true.
            TGraph::iterator child = callGraph.begin();
            for (; child != callGraph.end(); ++child) {

                // If we already visited this node, its whole subgraph has already been processed, so skip it.
                if (child->visited)
                    continue;

                if (call->callee == child->caller) {
                    if (child->currentPath) {
                        // Then, we found a back edge
                        if (! child->errorGiven) {
                            error(infoSink, "Recursion detected:");
                            infoSink.info << "    " << call->callee << " calling " << child->callee << "\n";
                            child->errorGiven = true;
                            recursive = true;
                        }
                    } else {
                        child->currentPath = true;
                        stack.push_back(&(*child));
                        break;
                    }
                }
            }
            if (child == callGraph.end()) {
                // no more callees, we bottomed out, never look at this node again
                stack.back()->currentPath = false;
                stack.back()->visited = true;
                stack.pop_back();
            }
        }  // end while, meaning nothing left to process in this subtree

    } while (newRoot);  // redundant loop check; should always exit via the 'break' above
}

//
// Satisfy rules for location qualifiers on inputs and outputs
//
void TIntermediate::inOutLocationCheck(TInfoSink& infoSink)
{
    // ES 3.0 requires all outputs to have location qualifiers if there is more than one output
    bool fragOutHasLocation = false;
    bool fragOutWithNoLocation = false;
    int numFragOut = 0;

    // TODO: linker functionality: location collision checking

    TIntermSequence& linkObjects = findLinkerObjects();
    for (size_t i = 0; i < linkObjects.size(); ++i) {
        const TType& type = linkObjects[i]->getAsTyped()->getType();
        const TQualifier& qualifier = type.getQualifier();
        if (language == EShLangFragment) {
            if (qualifier.storage == EvqVaryingOut) {
                ++numFragOut;
                if (qualifier.hasLocation())
                    fragOutHasLocation = true;
                else
                    fragOutWithNoLocation = true;
            }
        }
    }

    if (profile == EEsProfile) {
        if (numFragOut > 1 && fragOutWithNoLocation)
            error(infoSink, "when more than one fragment shader output, all must have location qualifiers");
    }        
}

TIntermSequence& TIntermediate::findLinkerObjects() const
{
    // Get the top-level globals
    TIntermSequence& globals = treeRoot->getAsAggregate()->getSequence();

    // Get the last member of the sequences, expected to be the linker-object lists
    assert(globals.back()->getAsAggregate()->getOp() == EOpLinkerObjects);

    return globals.back()->getAsAggregate()->getSequence();
}

// See if a variable was both a user-declared output and used.
// Note: the spec discusses writing to one, but this looks at read or write, which 
// is more useful, and perhaps the spec should be changed to reflect that.
bool TIntermediate::userOutputUsed() const
{
    const TIntermSequence& linkerObjects = findLinkerObjects();

    bool found = false;
    for (size_t i = 0; i < linkerObjects.size(); ++i) {
        const TIntermSymbol& symbolNode = *linkerObjects[i]->getAsSymbolNode();
        if (symbolNode.getQualifier().storage == EvqVaryingOut &&
            symbolNode.getName().compare(0, 3, "gl_") != 0 &&
            inIoAccessed(symbolNode.getName())) {            
            found = true;
            break;
        }
    }

    return found;
}

// Accumulate locations used for inputs, outputs, and uniforms, and check for collisions
// as the accumulation is done.
//
// Returns < 0 if no collision, >= 0 if collision and the value returned is a colliding value.
//
// typeCollision is set to true if there is no direct collision, but the types in the same location
// are different.
//
int TIntermediate::addUsedLocation(const TQualifier& qualifier, const TType& type, bool& typeCollision)
{
    typeCollision = false;

    int set;
    if (qualifier.isPipeInput())
        set = 0;
    else if (qualifier.isPipeOutput())
        set = 1;
    else if (qualifier.isUniform())
        set = 2;
    else
        return -1;

    int size;
    if (qualifier.isUniform()) {
        if (type.isArray())
            size = type.getArraySize();
        else
            size = 1;
    } else {
        if (type.isArray() && ! qualifier.patch &&
            (language == EShLangGeometry && qualifier.isPipeInput()) ||
            language == EShLangTessControl                           ||
            (language == EShLangTessEvaluation && qualifier.isPipeInput())) {
            TType elementType(type, 0);
            size = computeTypeLocationSize(elementType);
        } else
            size = computeTypeLocationSize(type);
    }

    TRange locationRange = { qualifier.layoutLocation, qualifier.layoutLocation + size - 1 };
    TRange componentRange = { 0, 3 };
    if (qualifier.layoutComponent != TQualifier::layoutComponentEnd) {
        componentRange.start = qualifier.layoutComponent;
        componentRange.last = componentRange.start + type.getVectorSize() - 1;
    }

    // check for collisions, except for vertex inputs on desktop
    if (! (profile != EEsProfile && language == EShLangVertex && qualifier.isPipeInput())) {
        for (size_t r = 0; r < usedIo[set].size(); ++r) {
            if (locationRange.last  >= usedIo[set][r].location.start &&
                locationRange.start <= usedIo[set][r].location.last &&
                componentRange.last  >= usedIo[set][r].component.start &&
                componentRange.start <= usedIo[set][r].component.last) {
                // there is a collision; pick one
                return std::max(locationRange.start, usedIo[set][r].location.start);
            } else if (locationRange.last  >= usedIo[set][r].location.start &&
                       locationRange.start <= usedIo[set][r].location.last &&
                       type.getBasicType() != usedIo[set][r].basicType) {
                typeCollision = true;
                return std::max(locationRange.start, usedIo[set][r].location.start);
            }
        }
    }

    TIoRange range = { locationRange, componentRange, type.getBasicType() };

    usedIo[set].push_back(range);

    return -1;
}

// Recursively figure out how many locations are used up by an input or output type.
// Return the size of type, as measured by "locations".
int TIntermediate::computeTypeLocationSize(const TType& type)
{
    // "If the declared input is an array of size n and each element takes m locations, it will be assigned m * n 
    // consecutive locations..."
    if (type.isArray()) {
        TType elementType(type, 0);        
        if (type.getArraySize() == 0) {
            // TODO: are there valid cases of having an unsized array with a location?  If so, running this code too early.
            return computeTypeLocationSize(elementType);
        } else
            return type.getArraySize() * computeTypeLocationSize(elementType);
    }

    // "The locations consumed by block and structure members are determined by applying the rules above 
    // recursively..."    
    if (type.isStruct()) {
        int size = 0;
        for (size_t member = 0; member < type.getStruct()->size(); ++member) {
            TType memberType(type, member);
            size += computeTypeLocationSize(memberType);
        }
        return size;
    }

    // "If a vertex shader input is any scalar or vector type, it will consume a single location. If a non-vertex 
    // shader input is a scalar or vector type other than dvec3 or dvec4, it will consume a single location, while 
    // types dvec3 or dvec4 will consume two consecutive locations. Inputs of type double and dvec2 will 
    // consume only a single location, in all stages."
    if (type.isScalar())
        return 1;
    if (type.isVector()) {
        if (language == EShLangVertex && type.getQualifier().isPipeInput())
            return 1;
        if (type.getBasicType() == EbtDouble && type.getVectorSize() > 2)
            return 2;
        else
            return 1;
    }

    // "If the declared input is an n x m single- or double-precision matrix, ...
    // The number of locations assigned for each matrix will be the same as 
    // for an n-element array of m-component vectors..."
    if (type.isMatrix()) {
        TType columnType(type, 0);
        return type.getMatrixCols() * computeTypeLocationSize(columnType);
    }

    assert(0);
    return 1;
}

} // end namespace glslang
