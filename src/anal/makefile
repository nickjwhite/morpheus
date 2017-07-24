
LEXLIB = -lfl

INCLUDE=../includes
CFLAGS= -O2 -I${INCLUDE}
RM=rm
CC=cc
CC=gcc
LEX=flex

ENDLIB=../gkends/gkends.a
GREEKLIB=../greeklib/greeklib.a
MORPHLIB=../morphlib/morphlib.a
GENERLIB=../gener/gener.a
DICTLIB=../gkdict/gkdict.a
ANALLIB=anal.a
LIBS=${GENERLIB} ${ENDLIB} ${DICTLIB} ${MORPHLIB} ${GREEKLIB} 

LSJDIR=/local/text/lsj
ANALOBJ= checkcrasis.o \
checkdict.o \
checkgenwds.o \
checkhalf1.o \
checkindecl.o \
checkirreg.o \
checknom.o \
checkpreverb.o \
checkstem.o \
checkstring.o \
checkverb.o \
checkword.o \
dictstems.o \
prntanal.o \
prvb.o 

${ANALLIB}: ${ANALOBJ}
	ar rv ${ANALLIB} ${ANALOBJ}
	ranlib ${ANALLIB}

ALL=cruncher pname findbase deverbal jsonanalysis ${ANALLIB}

all: ${ALL}

install: all
	cp ${ALL} ../../bin


lcnt: lcnt.o ${ANALLIB} ${LIBS}
	${CC} -o lcnt lcnt.o ../gener/genwd.o ${ANALLIB} ${LIBS}

cruncher:stdiomorph.o ${ANALLIB} ${LIBS}
	${CC} -o cruncher stdiomorph.o ../gener/genwd.o ${ANALLIB} ${LIBS}

jsonanalysis: jsonanalysis.o ${ANALLIB} ${LIBS}
	${CC} -o $@ $@.o ../gener/genwd.o ${ANALLIB} ${LIBS}

morphcheck:morphcheck.o ${LSJDIR}/MDBlib.o ${LSJDIR}/flen.o ${ANALOBJ} ${LIBS}
	${CC} -o morphcheck morphcheck.o ${LSJDIR}/MDBlib.o ${LSJDIR}/flen.o ../gener/genwd.o ${ANALOBJ} ${LIBS} ${LEXLIB}

pname:propname.o ${ANALOBJ} ${LIBS}
	${CC} -o pname propname.o ${ANALOBJ} ${LIBS}

findbase:	findbase.o ${ANALOBJ} ${LIBS}
	${CC} -o findbase findbase.o ${ANALOBJ} ${LIBS}

deverbal: deverb.o   ${ANALOBJ} ${LIBS}
	${CC} -o deverbal deverb.o ${ANALOBJ} ${LIBS}

morpheus.js: ${LIBS} ${ANALLIB} jsonanalysis.o
	${CC} -O2 -s EXPORTED_FUNCTIONS="['_analysis_as_json']" --preload-file ../../stemlib-outonly -o $@ jsonanalysis.o ../gener/genwd.o ${ANALLIB} ${LIBS}
	printf 'var MorpheusAnalysis = Module.cwrap("analysis_as_json", "string", ["string"]);' >> $@

clean:
	rm -f cruncher *.o

proclems: proclems.o
	${CC} -o proclems proclems.o
