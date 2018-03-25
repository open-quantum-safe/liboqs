#!/bin/bash

###
# Explode archive to objects files and rename
# - used if the upstream program builds a .a rather than bunch of .o files
# - takes as input a module name and an archive file
# - unarchives the files to a temporary directory, then runs the collect_objects script
###

if [ $# -lt 2 ]
then
	echo "Usage: explode.sh module archive"
	echo ""
	echo "Example: module could be kem_frodokem_640_aes"
	exit 1
fi

MODULE=$1
ARCHIVE_FILE=$2

UNAME=`uname`
if [ ${UNAME} = 'Darwin' ]
then
	mkdir -p .objs/${MODULE}
	cp ${ARCHIVE_FILE} .objs/${MODULE}/${MODULE}_upstream.a
elif [ ${UNAME} = 'Linux' ]
then
	rm -rf .tmp/${MODULE}
	mkdir -p .tmp/${MODULE}
	cd .tmp/${MODULE}
	ar x ../../${ARCHIVE_FILE}
	cd ../..
	OBJFILES=`find .tmp/${MODULE} -name '*.o'`
	echo ${OBJFILES}
	bash scripts/collect_objects.sh ${MODULE} ${OBJFILES}
	rm -rf .tmp/${MODULE}
else
	PRINT_GREEN="tput setaf 2"
	PRINT_RED="tput setaf 1"
	PRINT_RESET="tput sgr 0"

	${PRINT_RED}
	echo "Don't know how to explode and collect a library on this platform."
	${PRINT_RESET}
	exit 1
fi
