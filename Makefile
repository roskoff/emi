.SUFFIXES: .cc
#
SHELL = /bin/sh

CC = cc
CXX = c++ 
#CFLAGS = -g -O2
CFLAGS = -g
#CXXFLAGS = -g -O2
CXXFLAGS = -g 

LDFLAGS = 

LIBS = 
LIBDIR =
RANLIB =

INCDIRS = -I .
OUTPUT_DIR = .

.cc.o:
	$(CXX) $(DEFINES) -c $(CXXFLAGS) $(INCDIRS) $< -o $@


LISTA_OBJS =	EMIParser.o \
		main.o \
		Matriz.o \
		SymTable.o \
		TokenReader.o \
		Variable.o 

LISTA_LIBS = 

all: prog

prog: $(LISTA_OBJS)
	$(CXX) $(CXXFLAGS) -o emi $(LISTA_OBJS) $(LIBS)

clean:
	-@rm -f *.o
	-@rm -f emi

## Si hay mkdep o makedep, puede descomentarse lo siguiente:
## include .depend
