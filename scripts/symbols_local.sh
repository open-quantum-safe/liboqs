#!/bin/bash

###
# Makes a collection of symbols into local symbols
# - takes as input a module name and file containing list of symbols (one symbol per line)
# - in every object file in that module's directory in .objs:
#   - rename every specified symbol to be prfixed by _oqs
#   - unexport every specified symbol
###

if [ $# -ne 2 ]
then
	echo "Usage: symbols_local.sh module symbols.txt"
	echo ""
	echo "  symbols.txt: file containing list of symbols to unexport, one per line"
	exit 1
fi

MODULE=$1

SYMBOLS_FILE=$2

PRINT_GREEN="tput setaf 2"
PRINT_RED="tput setaf 1"
PRINT_RESET="tput sgr 0"

SYMBOLS=`cat ${SYMBOLS_FILE}`
PREFIX=_oqs_${MODULE}

UNAME=`uname`
if [ ${UNAME} = 'Darwin' ]
then
	UNEXPORT_ARG=
	for SYMBOL in ${SYMBOLS}; do
		OLD_SYMBOL="_${SYMBOL}"
		UNEXPORT_ARG+=" -unexported_symbol ${OLD_SYMBOL}"
	done
	ARCHIVE_FILE=.objs/${MODULE}/${MODULE}_upstream.a
	ld -r ${ARCHIVE_FILE} -o ${ARCHIVE_FILE} ${UNEXPORT_ARG}
elif [ ${UNAME} = 'Linux' ]
then
	REDEFINE_ARG=
	WEAKEN_ARG=
	for SYMBOL in ${SYMBOLS}; do
		NEW_SYMBOL="${PREFIX}_${SYMBOL}"
		REDEFINE_ARG+=" --redefine-sym ${SYMBOL}=${NEW_SYMBOL}"
		WEAKEN_ARG+=" --weaken-symbol ${NEW_SYMBOL}"
	done
	OBJFILES=`find .objs/${MODULE}/upstream -name '*.o'`
	for OBJFILE in ${OBJFILES}; do
		objcopy ${REDEFINE_ARG} ${OBJFILE}
		objcopy ${WEAKEN_ARG} ${OBJFILE}
	done
else
	${PRINT_RED}
	echo "Don't know how to unexport symbols on this platform."
	${PRINT_RESET}
	exit 1
fi
