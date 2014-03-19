## - - - - - - - - - - - - - - - - - - - -
CXX = g++-4.8
CXXFLAGS_STANDARD = -pedantic -std=c++11 -Wno-write-strings
CXXFLAGS_LOCAL = -I.
CXXFLAGS_XERCES =
CXXFLAGS = ${CXXFLAGS_STANDARD} ${CXXFLAGS_LOCAL} ${CXXFLAGS_XERCES}

LDFLAGS_STANDARD =
LDFLAGS_XERCES = -lxerces-c
LDFLAGS = ${LDFLAGS_STANDARD} ${LDFLAGS_XERCES}

BINARIES = step1 query
OBJ_FILES = helper-classes.o sample-classes.o

## - - - - - - - - - - - - - - - - - - - -

all: ${BINARIES}

## SAX2: simple talking handler
step1: step1.cc helper-classes.o sample-classes.o
	${CXX} ${CXXFLAGS} step1.cc -o ${@} helper-classes.o sample-classes.o ${LDFLAGS}

helper-classes.o: helper-classes.cc helper-classes.h
	${CXX} ${CXXFLAGS} -c helper-classes.cc -o ${@}

sample-classes.o: sample-classes.cc sample-classes.h
	${CXX} ${CXXFLAGS} -c sample-classes.cc -o ${@}

query: query.cpp 
	${CXX} ${CXXFLAGS} query.cpp -o ${@}

clean:
	-rm -f ${BINARIES} ${OBJ_FILES} query
