## - - - - - - - - - - - - - - - - - - - -

CXXFLAGS_STANDARD = -g -Wall -pedantic
CXXFLAGS_LOCAL = -I.
CXXFLAGS_XERCES =
CXXFLAGS = ${CXXFLAGS_STANDARD} ${CXXFLAGS_LOCAL} ${CXXFLAGS_XERCES}

LDFLAGS_STANDARD = -g
LDFLAGS_XERCES = -lxerces-c
LDFLAGS = ${LDFLAGS_STANDARD} ${LDFLAGS_XERCES}

BINARIES = step1 step2 step3 step4
OBJ_FILES = helper-classes.o sample-classes.o

## - - - - - - - - - - - - - - - - - - - -

all: ${BINARIES}

## SAX2: simple talking handler
step1: step1.cc helper-classes.o sample-classes.o
	${CXX} ${CXXFLAGS} step1.cc -o ${@} helper-classes.o sample-classes.o ${LDFLAGS}

## extract fields from the repodata file
step2: step2.cc helper-classes.o sample-classes.o
	${CXX} ${CXXFLAGS} step2.cc -o ${@} helper-classes.o sample-classes.o ${LDFLAGS}

## add SAX error handling
step3: step3.cc helper-classes.o sample-classes.o
	${CXX} ${CXXFLAGS} step3.cc -o ${@} helper-classes.o sample-classes.o ${LDFLAGS}

## add validation and entity resolving
step4: step4.cc helper-classes.o sample-classes.o
	${CXX} ${CXXFLAGS} step4.cc -o ${@} helper-classes.o sample-classes.o ${LDFLAGS}

helper-classes.o: helper-classes.cc helper-classes.h
	${CXX} ${CXXFLAGS} -c helper-classes.cc -o ${@}

sample-classes.o: sample-classes.cc sample-classes.h
	${CXX} ${CXXFLAGS} -c sample-classes.cc -o ${@}


clean:
	-rm -f ${BINARIES} ${OBJ_FILES}
