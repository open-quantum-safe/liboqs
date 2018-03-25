#!/bin/bash

###
# Renames globally exported symbols
# - takes as input a module name and file containing list of symbols (file format: two symbols per line, the old name and then the new name)
# - in every object file in that module's directory in .objs:
#   - rename every specified symbol to be prfixed by _oqs
#   - unexport every specified symbol
###

if [ $# -ne 2 ]
then
	echo "Usage: symbols_global_rename.sh module symbols.txt"
	exit 1
fi

MODULE=$1

SYMBOLS_FILE=$2

PRINT_GREEN="tput setaf 2"
PRINT_RED="tput setaf 1"
PRINT_RESET="tput sgr 0"

SYMBOLS=(`cat ${SYMBOLS_FILE}`)

UNAME=`uname`
if [ ${UNAME} = 'Darwin' ]
then
	ALIAS_ARG=
	for (( i = 0 ; i < ${#SYMBOLS[@]} ; i += 2)) ; do
		OLD_SYMBOL=${SYMBOLS[i]}
		NEW_SYMBOL=${SYMBOLS[i+1]}
		ALIAS_ARG+=" -alias _${OLD_SYMBOL} _${NEW_SYMBOL}"
	done
	ARCHIVE_FILE=.objs/${MODULE}/${MODULE}_upstream.a
	ld -r ${ARCHIVE_FILE} -o ${ARCHIVE_FILE} ${ALIAS_ARG}
elif [ ${UNAME} = 'Linux' ]
then
	REDEFINE_ARG=
	for (( i = 0 ; i < ${#SYMBOLS[@]} ; i += 2)) ; do
		OLD_SYMBOL=${SYMBOLS[i]}
		NEW_SYMBOL=${SYMBOLS[i+1]}
		REDEFINE_ARG+=" --redefine-sym ${OLD_SYMBOL}=${NEW_SYMBOL}"
	done
	OBJFILES=`find .objs/${MODULE}/upstream -name '*.o'`
	for OBJFILE in ${OBJFILES}; do
		objcopy ${REDEFINE_ARG} ${OBJFILE}
	done
else
	${PRINT_RED}
	echo "Don't know how to alias symbols on this platform."
	${PRINT_RESET}
	exit 1
fi
