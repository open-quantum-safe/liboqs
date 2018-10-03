#!/bin/bash

###
# Run all tests for Travis
#
# Need to set the following environment variables:
# - CC_OVERRIDE: whatever compiler you want to use
###

source $(dirname $0)/defs.sh

( # this is like the start of a "try...catch" block, see https://stackoverflow.com/questions/22009364/is-there-a-try-catch-command-in-bash#22010339
	set -e

	# See what has been modified (ignoring submodules because they are likely patched)
	MODIFIED=$(git status -s)
	if [[ ! -z "${MODIFIED}" ]];
	then
		${PRINT_RED}
		echo "There are modified files present in the directory prior to .travis/all-tests.sh. This may indicate that some files should be added to .gitignore or need to be committed.  Travis tests will not yield correct results if modified files are present.  Please fix and try again.";
		${PRINT_RESET}
		git status -s
		exit 1;
	fi;

	if [ -z ${CC_OVERRIDE+x} ]; then
		echo "CC_OVERRIDE environment variable not set."
		exit 1
	fi

	export CC=$CC_OVERRIDE

	# construct configure arguments
	enable_disable_str=

	if [[ ${USE_OPENSSL} == 1 ]];then
		enable_disable_str=" --enable-openssl"
		if [[ ! -z "${OPENSSL_DIR// }" ]];then
			enable_disable_str+=" --with-openssl-dir=${OPENSSL_DIR}"
		fi
	fi

	if [[ ${AES_NI} == 0 ]];then
		enable_disable_str+=" --disable-aes-ni"
	fi

	if [[ ${ENABLE_SIG_PICNIC} == 0 ]];then
		enable_disable_str+=" --disable-sig-picnic"
	fi

	# build and run
	autoreconf -i
	./configure --enable-silent-rules ${enable_disable_str}
	make clean
	make
	make docs
	make test

	# Excercise static build of liboqs too
	./configure --enable-shared=no --enable-silent-rules ${enable_disable_str}
	make clean
	make
	make docs
	make test

	for f in $(ls .travis/*-check.sh); do
		bash $f;
	done
) # the end of the "try...catch" block
ERROR_CODE=$?
if [ ${ERROR_CODE} -ne 0 ]; then
	${PRINT_RED}
	echo "An error occurred while running all-tests.sh.  If the previous line is a green success message, that is likely what the *last* successful command, and the next command is what caused the error.";
	${PRINT_RESET}
	exit 1;
fi
