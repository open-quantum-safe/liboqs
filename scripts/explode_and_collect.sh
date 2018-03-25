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
	echo "Example: module could be kem_dummy1"
	exit 1
fi

MODULE=$1

ARCHIVE_FILE=$2

rm -rf .tmp/${MODULE}
mkdir -p .tmp/${MODULE}
cd .tmp/${MODULE}
ar x ../../${ARCHIVE_FILE}
cd ../..
OBJFILES=`find .tmp/${MODULE}`
bash scripts/collect_objects.sh ${MODULE} ${OBJFILES}
rm -rf .tmp/${MODULE}
