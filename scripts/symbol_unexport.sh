#!/bin/bash

###
# Unexports a given list of symbols in a library file
###

if [ $# -ne 2 ]
then
	echo "Usage: symbol_unexport.sh file.a symbols.txt"
	echo ""
	echo "  symbols.txt: file containing list of symbols to unexport, one per line"
	exit 1
fi

UNAME=`uname`

PRINT_GREEN="tput setaf 2"
PRINT_RED="tput setaf 1"
PRINT_RESET="tput sgr 0"

if [ ${UNAME} = 'Darwin' ]
then
	ld -r $1 -o $1 -unexported_symbols_list $2
elif [ ${UNAME} = 'Linux' ]
then
	objcopy --weaken-symbols=$2 $1
else
	${PRINT_RED}
	echo "Don't know how to unexport symbols on this platform."
	${PRINT_RESET}
	exit 1
fi
