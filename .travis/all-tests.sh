#!/bin/bash

###
# Run all tests for Travis
#
# Need to set the following environment variables:
# - ARCH: x64 OR x86
# - CC_OVERRIDE: whatever compiler you want to use
###

set -e

PRINT_GREEN="tput setaf 2"
PRINT_RED="tput setaf 1"
PRINT_RESET="tput sgr 0"

# see what has been modified (ignoring submodules because they are likely patched)
MODIFIED=$(git status -s)

if [[ ! -z "${MODIFIED}" ]];
then
	${PRINT_RED}
	echo "There are modified files present in the directory prior to .travis/all-tests.sh. This may indicate that some files should be added to .gitignore or need to be committed.  Travis tests will not yield correct results if modified files are present.  Please fix and try again.";
	${PRINT_RESET}
	git status -s
	exit 1;
fi;

if [ -z ${ARCH+x} ]; then
	echo "ARCH environment variable not set."
	exit 1
fi

if [ -z ${CC_OVERRIDE+x} ]; then
	echo "CC_OVERRIDE environment variable not set."
	exit 1
fi

make clean
make -j8 "ARCH=${ARCH}" "CC=${CC_OVERRIDE}"
make docs
./test_kem
./example_kem

for f in $(ls .travis/*-check.sh); do
	bash $f;
done
