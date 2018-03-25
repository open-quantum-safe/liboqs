#!/bin/bash

###
# Collects upstream object files to the .objs folder
# - takes as input a module name and a list of objects with paths
# - on Linux: copies those objects to the module's upstream .objs directory and renames them to be prefixed with the module name
# - on macOS: makes a .a file
###

if [ $# -lt 2 ]
then
	echo "Usage: collect_object.sh module objfiles ..."
	echo ""
	echo "Example: module could be kem_frodokem_640_aes"
	exit 1
fi

MODULE=$1
OBJFILES=${@:2}

UNAME=`uname`
if [ ${UNAME} = 'Darwin' ]
then
	rm -f .objs/${MODULE}/${MODULE}_upstream.a
	mkdir -p .objs/${MODULE}
	ar rcs .objs/${MODULE}/${MODULE}_upstream.a ${OBJFILES}
elif [ ${UNAME} = 'Linux' ]
then
	rm -rf .objs/${MODULE}/upstream
	mkdir -p .objs/${MODULE}/upstream
	for OBJFILE in ${OBJFILES}; do
		BASENAME=$(basename ${OBJFILE})
		cp ${OBJFILE} .objs/${MODULE}/upstream/${MODULE}_${BASENAME}
	done
else
	PRINT_GREEN="tput setaf 2"
	PRINT_RED="tput setaf 1"
	PRINT_RESET="tput sgr 0"

	${PRINT_RED}
	echo "Don't know how to collect objects on this platform."
	${PRINT_RESET}
	exit 1
fi
