/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ATTRIBUTE = 258,
     VARYING = 259,
     CONST = 260,
     BOOL = 261,
     FLOAT = 262,
     DOUBLE = 263,
     INT = 264,
     UINT = 265,
     BREAK = 266,
     CONTINUE = 267,
     DO = 268,
     ELSE = 269,
     FOR = 270,
     IF = 271,
     DISCARD = 272,
     RETURN = 273,
     SWITCH = 274,
     CASE = 275,
     DEFAULT = 276,
     SUBROUTINE = 277,
     BVEC2 = 278,
     BVEC3 = 279,
     BVEC4 = 280,
     IVEC2 = 281,
     IVEC3 = 282,
     IVEC4 = 283,
     UVEC2 = 284,
     UVEC3 = 285,
     UVEC4 = 286,
     VEC2 = 287,
     VEC3 = 288,
     VEC4 = 289,
     MAT2 = 290,
     MAT3 = 291,
     MAT4 = 292,
     CENTROID = 293,
     IN = 294,
     OUT = 295,
     INOUT = 296,
     UNIFORM = 297,
     PATCH = 298,
     SAMPLE = 299,
     BUFFER = 300,
     SHARED = 301,
     COHERENT = 302,
     VOLATILE = 303,
     RESTRICT = 304,
     READONLY = 305,
     WRITEONLY = 306,
     DVEC2 = 307,
     DVEC3 = 308,
     DVEC4 = 309,
     DMAT2 = 310,
     DMAT3 = 311,
     DMAT4 = 312,
     NOPERSPECTIVE = 313,
     FLAT = 314,
     SMOOTH = 315,
     LAYOUT = 316,
     MAT2X2 = 317,
     MAT2X3 = 318,
     MAT2X4 = 319,
     MAT3X2 = 320,
     MAT3X3 = 321,
     MAT3X4 = 322,
     MAT4X2 = 323,
     MAT4X3 = 324,
     MAT4X4 = 325,
     DMAT2X2 = 326,
     DMAT2X3 = 327,
     DMAT2X4 = 328,
     DMAT3X2 = 329,
     DMAT3X3 = 330,
     DMAT3X4 = 331,
     DMAT4X2 = 332,
     DMAT4X3 = 333,
     DMAT4X4 = 334,
     ATOMIC_UINT = 335,
     SAMPLER1D = 336,
     SAMPLER2D = 337,
     SAMPLER3D = 338,
     SAMPLERCUBE = 339,
     SAMPLER1DSHADOW = 340,
     SAMPLER2DSHADOW = 341,
     SAMPLERCUBESHADOW = 342,
     SAMPLER1DARRAY = 343,
     SAMPLER2DARRAY = 344,
     SAMPLER1DARRAYSHADOW = 345,
     SAMPLER2DARRAYSHADOW = 346,
     ISAMPLER1D = 347,
     ISAMPLER2D = 348,
     ISAMPLER3D = 349,
     ISAMPLERCUBE = 350,
     ISAMPLER1DARRAY = 351,
     ISAMPLER2DARRAY = 352,
     USAMPLER1D = 353,
     USAMPLER2D = 354,
     USAMPLER3D = 355,
     USAMPLERCUBE = 356,
     USAMPLER1DARRAY = 357,
     USAMPLER2DARRAY = 358,
     SAMPLER2DRECT = 359,
     SAMPLER2DRECTSHADOW = 360,
     ISAMPLER2DRECT = 361,
     USAMPLER2DRECT = 362,
     SAMPLERBUFFER = 363,
     ISAMPLERBUFFER = 364,
     USAMPLERBUFFER = 365,
     SAMPLERCUBEARRAY = 366,
     SAMPLERCUBEARRAYSHADOW = 367,
     ISAMPLERCUBEARRAY = 368,
     USAMPLERCUBEARRAY = 369,
     SAMPLER2DMS = 370,
     ISAMPLER2DMS = 371,
     USAMPLER2DMS = 372,
     SAMPLER2DMSARRAY = 373,
     ISAMPLER2DMSARRAY = 374,
     USAMPLER2DMSARRAY = 375,
     SAMPLEREXTERNALOES = 376,
     IMAGE1D = 377,
     IIMAGE1D = 378,
     UIMAGE1D = 379,
     IMAGE2D = 380,
     IIMAGE2D = 381,
     UIMAGE2D = 382,
     IMAGE3D = 383,
     IIMAGE3D = 384,
     UIMAGE3D = 385,
     IMAGE2DRECT = 386,
     IIMAGE2DRECT = 387,
     UIMAGE2DRECT = 388,
     IMAGECUBE = 389,
     IIMAGECUBE = 390,
     UIMAGECUBE = 391,
     IMAGEBUFFER = 392,
     IIMAGEBUFFER = 393,
     UIMAGEBUFFER = 394,
     IMAGE1DARRAY = 395,
     IIMAGE1DARRAY = 396,
     UIMAGE1DARRAY = 397,
     IMAGE2DARRAY = 398,
     IIMAGE2DARRAY = 399,
     UIMAGE2DARRAY = 400,
     IMAGECUBEARRAY = 401,
     IIMAGECUBEARRAY = 402,
     UIMAGECUBEARRAY = 403,
     IMAGE2DMS = 404,
     IIMAGE2DMS = 405,
     UIMAGE2DMS = 406,
     IMAGE2DMSARRAY = 407,
     IIMAGE2DMSARRAY = 408,
     UIMAGE2DMSARRAY = 409,
     STRUCT = 410,
     VOID = 411,
     WHILE = 412,
     IDENTIFIER = 413,
     TYPE_NAME = 414,
     FLOATCONSTANT = 415,
     DOUBLECONSTANT = 416,
     INTCONSTANT = 417,
     UINTCONSTANT = 418,
     BOOLCONSTANT = 419,
     FIELD_SELECTION = 420,
     LEFT_OP = 421,
     RIGHT_OP = 422,
     INC_OP = 423,
     DEC_OP = 424,
     LE_OP = 425,
     GE_OP = 426,
     EQ_OP = 427,
     NE_OP = 428,
     AND_OP = 429,
     OR_OP = 430,
     XOR_OP = 431,
     MUL_ASSIGN = 432,
     DIV_ASSIGN = 433,
     ADD_ASSIGN = 434,
     MOD_ASSIGN = 435,
     LEFT_ASSIGN = 436,
     RIGHT_ASSIGN = 437,
     AND_ASSIGN = 438,
     XOR_ASSIGN = 439,
     OR_ASSIGN = 440,
     SUB_ASSIGN = 441,
     LEFT_PAREN = 442,
     RIGHT_PAREN = 443,
     LEFT_BRACKET = 444,
     RIGHT_BRACKET = 445,
     LEFT_BRACE = 446,
     RIGHT_BRACE = 447,
     DOT = 448,
     COMMA = 449,
     COLON = 450,
     EQUAL = 451,
     SEMICOLON = 452,
     BANG = 453,
     DASH = 454,
     TILDE = 455,
     PLUS = 456,
     STAR = 457,
     SLASH = 458,
     PERCENT = 459,
     LEFT_ANGLE = 460,
     RIGHT_ANGLE = 461,
     VERTICAL_BAR = 462,
     CARET = 463,
     AMPERSAND = 464,
     QUESTION = 465,
     INVARIANT = 466,
     PRECISE = 467,
     HIGH_PRECISION = 468,
     MEDIUM_PRECISION = 469,
     LOW_PRECISION = 470,
     PRECISION = 471,
     PACKED = 472,
     RESOURCE = 473,
     SUPERP = 474
   };
#endif
/* Tokens.  */
#define ATTRIBUTE 258
#define VARYING 259
#define CONST 260
#define BOOL 261
#define FLOAT 262
#define DOUBLE 263
#define INT 264
#define UINT 265
#define BREAK 266
#define CONTINUE 267
#define DO 268
#define ELSE 269
#define FOR 270
#define IF 271
#define DISCARD 272
#define RETURN 273
#define SWITCH 274
#define CASE 275
#define DEFAULT 276
#define SUBROUTINE 277
#define BVEC2 278
#define BVEC3 279
#define BVEC4 280
#define IVEC2 281
#define IVEC3 282
#define IVEC4 283
#define UVEC2 284
#define UVEC3 285
#define UVEC4 286
#define VEC2 287
#define VEC3 288
#define VEC4 289
#define MAT2 290
#define MAT3 291
#define MAT4 292
#define CENTROID 293
#define IN 294
#define OUT 295
#define INOUT 296
#define UNIFORM 297
#define PATCH 298
#define SAMPLE 299
#define BUFFER 300
#define SHARED 301
#define COHERENT 302
#define VOLATILE 303
#define RESTRICT 304
#define READONLY 305
#define WRITEONLY 306
#define DVEC2 307
#define DVEC3 308
#define DVEC4 309
#define DMAT2 310
#define DMAT3 311
#define DMAT4 312
#define NOPERSPECTIVE 313
#define FLAT 314
#define SMOOTH 315
#define LAYOUT 316
#define MAT2X2 317
#define MAT2X3 318
#define MAT2X4 319
#define MAT3X2 320
#define MAT3X3 321
#define MAT3X4 322
#define MAT4X2 323
#define MAT4X3 324
#define MAT4X4 325
#define DMAT2X2 326
#define DMAT2X3 327
#define DMAT2X4 328
#define DMAT3X2 329
#define DMAT3X3 330
#define DMAT3X4 331
#define DMAT4X2 332
#define DMAT4X3 333
#define DMAT4X4 334
#define ATOMIC_UINT 335
#define SAMPLER1D 336
#define SAMPLER2D 337
#define SAMPLER3D 338
#define SAMPLERCUBE 339
#define SAMPLER1DSHADOW 340
#define SAMPLER2DSHADOW 341
#define SAMPLERCUBESHADOW 342
#define SAMPLER1DARRAY 343
#define SAMPLER2DARRAY 344
#define SAMPLER1DARRAYSHADOW 345
#define SAMPLER2DARRAYSHADOW 346
#define ISAMPLER1D 347
#define ISAMPLER2D 348
#define ISAMPLER3D 349
#define ISAMPLERCUBE 350
#define ISAMPLER1DARRAY 351
#define ISAMPLER2DARRAY 352
#define USAMPLER1D 353
#define USAMPLER2D 354
#define USAMPLER3D 355
#define USAMPLERCUBE 356
#define USAMPLER1DARRAY 357
#define USAMPLER2DARRAY 358
#define SAMPLER2DRECT 359
#define SAMPLER2DRECTSHADOW 360
#define ISAMPLER2DRECT 361
#define USAMPLER2DRECT 362
#define SAMPLERBUFFER 363
#define ISAMPLERBUFFER 364
#define USAMPLERBUFFER 365
#define SAMPLERCUBEARRAY 366
#define SAMPLERCUBEARRAYSHADOW 367
#define ISAMPLERCUBEARRAY 368
#define USAMPLERCUBEARRAY 369
#define SAMPLER2DMS 370
#define ISAMPLER2DMS 371
#define USAMPLER2DMS 372
#define SAMPLER2DMSARRAY 373
#define ISAMPLER2DMSARRAY 374
#define USAMPLER2DMSARRAY 375
#define SAMPLEREXTERNALOES 376
#define IMAGE1D 377
#define IIMAGE1D 378
#define UIMAGE1D 379
#define IMAGE2D 380
#define IIMAGE2D 381
#define UIMAGE2D 382
#define IMAGE3D 383
#define IIMAGE3D 384
#define UIMAGE3D 385
#define IMAGE2DRECT 386
#define IIMAGE2DRECT 387
#define UIMAGE2DRECT 388
#define IMAGECUBE 389
#define IIMAGECUBE 390
#define UIMAGECUBE 391
#define IMAGEBUFFER 392
#define IIMAGEBUFFER 393
#define UIMAGEBUFFER 394
#define IMAGE1DARRAY 395
#define IIMAGE1DARRAY 396
#define UIMAGE1DARRAY 397
#define IMAGE2DARRAY 398
#define IIMAGE2DARRAY 399
#define UIMAGE2DARRAY 400
#define IMAGECUBEARRAY 401
#define IIMAGECUBEARRAY 402
#define UIMAGECUBEARRAY 403
#define IMAGE2DMS 404
#define IIMAGE2DMS 405
#define UIMAGE2DMS 406
#define IMAGE2DMSARRAY 407
#define IIMAGE2DMSARRAY 408
#define UIMAGE2DMSARRAY 409
#define STRUCT 410
#define VOID 411
#define WHILE 412
#define IDENTIFIER 413
#define TYPE_NAME 414
#define FLOATCONSTANT 415
#define DOUBLECONSTANT 416
#define INTCONSTANT 417
#define UINTCONSTANT 418
#define BOOLCONSTANT 419
#define FIELD_SELECTION 420
#define LEFT_OP 421
#define RIGHT_OP 422
#define INC_OP 423
#define DEC_OP 424
#define LE_OP 425
#define GE_OP 426
#define EQ_OP 427
#define NE_OP 428
#define AND_OP 429
#define OR_OP 430
#define XOR_OP 431
#define MUL_ASSIGN 432
#define DIV_ASSIGN 433
#define ADD_ASSIGN 434
#define MOD_ASSIGN 435
#define LEFT_ASSIGN 436
#define RIGHT_ASSIGN 437
#define AND_ASSIGN 438
#define XOR_ASSIGN 439
#define OR_ASSIGN 440
#define SUB_ASSIGN 441
#define LEFT_PAREN 442
#define RIGHT_PAREN 443
#define LEFT_BRACKET 444
#define RIGHT_BRACKET 445
#define LEFT_BRACE 446
#define RIGHT_BRACE 447
#define DOT 448
#define COMMA 449
#define COLON 450
#define EQUAL 451
#define SEMICOLON 452
#define BANG 453
#define DASH 454
#define TILDE 455
#define PLUS 456
#define STAR 457
#define SLASH 458
#define PERCENT 459
#define LEFT_ANGLE 460
#define RIGHT_ANGLE 461
#define VERTICAL_BAR 462
#define CARET 463
#define AMPERSAND 464
#define QUESTION 465
#define INVARIANT 466
#define PRECISE 467
#define HIGH_PRECISION 468
#define MEDIUM_PRECISION 469
#define LOW_PRECISION 470
#define PRECISION 471
#define PACKED 472
#define RESOURCE 473
#define SUPERP 474




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 66 "glslang.y"
{
    struct {
        glslang::TSourceLoc loc;
        union {
            glslang::TString *string;
            int i;
            unsigned int u;
            bool b;
            double d;
        };
        glslang::TSymbol* symbol;
    } lex;
    struct {
        glslang::TSourceLoc loc;
        glslang::TOperator op;
        union {
            TIntermNode* intermNode;
            glslang::TIntermNodePair nodePair;
            glslang::TIntermTyped* intermTypedNode;
        };
        union {
            glslang::TPublicType type;
            glslang::TFunction* function;
            glslang::TParameter param;
            glslang::TTypeLoc typeLine;
            glslang::TTypeList* typeList;
            glslang::TArraySizes* arraySizes;
            glslang::TIdentifierList* identifierList;
        };
    } interm;
}
/* Line 1529 of yacc.c.  */
#line 519 "glslang.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



