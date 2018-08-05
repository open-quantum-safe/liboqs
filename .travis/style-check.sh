#!/bin/bash

source $(dirname $0)/defs.sh

###
# Checks that all non-upstream files satisfy prettyprint requirements.
###

if [[ "x${TRAVIS}" == "xtrue" ]];
then
	if [[ ! "x${CHECK_STYLE}" == "xtrue" ]];
	then
		echo "When running on Travis, style-check is only run on some builds."
		exit 0
	fi
fi

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

CLANG_FORMAT_VERSION=`${TRY_CLANGFORMAT} -version | grep 3.9`
if [[ -z "${CLANG_FORMAT_VERSION}" ]];
then
	${PRINT_RED}
	echo "clang-format is not version 3.9."
	${PRINT_RESET}
	${TRY_CLANGFORMAT} -version
	exit 1
fi;

# See what has been modified (ignoring submodules because they are likely patched)
MODIFIED=$(git status -s)

if [[ ! -z "${MODIFIED}" ]];
then
	${PRINT_RED}
	echo "There are modified files present in the directory prior to prettyprint check. This may indicate that some files should be added to .gitignore or need to be committed.";
	${PRINT_RESET}
	git status -s
	exit 1;
fi;

make prettyprint CLANGFORMAT=${TRY_CLANGFORMAT}

if [[ ${ENABLE_KEX_RLWE_NEWHOPE_AVX2} == 1 ]];then
  MODIFIED=$(echo $MODIFIED | grep -v "kex_rlwe_newhope/avx2" | grep -v "Makefile.am" | grep -v "avx2/kex*")
else
  MODIFIED=$(echo $MODIFIED | grep -v "Makefile.am")
fi

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
