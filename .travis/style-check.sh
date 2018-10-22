#!/bin/bash

###
# Checks that all non-upstream files satisfy prettyprint requirements.
###

set -e

source $(dirname $0)/defs.sh

if [[ "x${TRAVIS}" == "xtrue" ]];
then
	if [[ ! "x${CHECK_STYLE}" == "xtrue" ]];
	then
		echo "When running on Travis, style-check is only run on some builds."
		exit 0
	fi
fi

# Make sure that there are no modified files to start with
MODIFIED=$(git status -s)
if [[ ! -z "${MODIFIED}" ]];
then
	${PRINT_RED}
	echo "There are modified files present in the directory prior to prettyprint check. This may indicate that some files should be added to .gitignore or need to be committed.";
	${PRINT_RESET}
	git status -s
	exit 1;
fi;

# Find clang-format
TRY_CLANGFORMAT="/usr/local/Cellar/clang-format/2016-06-27/bin/clang-format"
if [[ ! -x $(which ${TRY_CLANGFORMAT}) ]];
then
	TRY_CLANGFORMAT="clang-format-3.9"
	if [[ ! -x $(which ${TRY_CLANGFORMAT}) ]];
	then
		TRY_CLANGFORMAT="clang-format"
		if [[ ! -x $(which ${TRY_CLANGFORMAT}) ]];
		then
			${PRINT_RED}
			echo "Cannot find clang-format."
			${PRINT_RESET}
			exit 1
		fi
	fi
fi

# Check clang-format version
set +e
CLANG_FORMAT_VERSION=$(${TRY_CLANGFORMAT} -version | grep 3.9)
ERROR_CODE=$?
set -e
if [ ${ERROR_CODE} -ne 0 ];
then
	${PRINT_RED}
	echo "clang-format is not version 3.9."
	${PRINT_RESET}
	${TRY_CLANGFORMAT} -version
	exit 1
fi;

# Pretty-print everything
make prettyprint CLANGFORMAT=${TRY_CLANGFORMAT}

# Check if there are any modified files
MODIFIED=$(git status -s)
if [[ ! -z "${MODIFIED}" ]]; then
	${PRINT_RED}
	echo "Code does not adhere to the project standards. Run \"make prettyprint\".";
	${PRINT_RESET}
	git status -s
	exit 1;
else
	${PRINT_GREEN}
	echo "Code adheres to the project standards (prettyprint).";
	${PRINT_RESET}
	exit 0;
fi;
