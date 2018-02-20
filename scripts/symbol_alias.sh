#!/bin/bash

###
# Renames symbols in library files
###

if [ $# -ne 3 ]
then
	echo "Usage: symbol_alias.sh file.a old_sym new_sym"
	exit 1
fi

UNAME=`uname`

PRINT_GREEN="tput setaf 2"
PRINT_RED="tput setaf 1"
PRINT_RESET="tput sgr 0"

if [ ${UNAME} = 'Darwin' ]
then
	ld -r $1 -o $1 -alias _$2 _$3
elif [ ${UNAME} = 'Linux' ]
then
	objcopy --redefine-sym $2=$3 $1
else
	${PRINT_RED}
	echo "Don't know how to alias symbols on this platform."
	${PRINT_RESET}
	exit 1
fi
