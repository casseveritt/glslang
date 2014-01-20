CC = g++

OBJECTS = CodeGen.o Link.o
AR=ar

SRCS=CodeGen.cpp Link.cpp

CPPFLAGS = -fPIC

default : all
all : libCodeGen.a

libCodeGen.a : $(OBJECTS)
	$(AR) rvu $@ $(OBJECTS)
	ranlib $@

%.o : %.cpp
	$(CC) -c $(CPPFLAGS) $<

#
# Cleanup
#
.PHONY : clean
clean :
	$(RM) *.o *.a

depend:
	makedepend -- $(CFLAGS) -- $(SRCS)
# DO NOT DELETE
