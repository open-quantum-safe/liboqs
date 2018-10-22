#!/bin/bash

###
# Run all tests for Travis
#
# Need to set the following environment variables:
# - ARCH: x64 OR x86
# - CC_OVERRIDE: whatever compiler you want to use
###

source $(dirname $0)/defs.sh

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

if [[ "${TRAVIS_OS_NAME}" == "linux" ]]; then
	cat /proc/cpuinfo
	dpkg -l | grep binutils
fi

make clean
make -j8 "ARCH=${ARCH}" "CC=${CC_OVERRIDE}"
make docs
./test_kem
./test_sig
LD_LIBRARY_PATH=.
export LD_LIBRARY_PATH
./test_kem_shared
./test_sig_shared
./example_kem
./example_sig

for f in $(ls .travis/*-check.sh); do
	bash $f;
done
