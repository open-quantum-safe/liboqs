#!/bin/bash

###
# Run all tests for Travis
#
# Need to set the following environment variables:
# - CC_OVERRIDE: whatever compiler you want to use
###

set -e

PRINT_GREEN="tput setaf 2"
PRINT_RED="tput setaf 1"
PRINT_RESET="tput sgr 0"

# see what has been modified (ignoring submodules because they are likely patched)
MODIFIED=$(git status -s)
:<<'END'
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
END

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

if [[ ${ENABLE_CODE_MCBITS} == 1 ]];then
	enable_disable_str+=" --enable-kex-code-mcbits"
	if [[ ! -z "${SODIUM_DIR// }" ]];then
		enable_disable_str+=" --with-sodium-dir=${SODIUM_DIR}"
	fi
fi

if [[ ${ENABLE_KEX_NTRU} == 0 ]];then
	enable_disable_str+=" --disable-kex-ntru"
fi

if [[ ${ENABLE_KEX_RLWE_NEWHOPE} == 0 ]];then
	enable_disable_str+=" --disable-kex-rlwe-newhope"
fi

if [[ ${ENABLE_KEX_SIDH_MSR} == 0 ]];then
	enable_disable_str+=" --disable-kex-sidh-msr"
fi

if [[ ${ENABLE_SIG_PICNIC} == 0 ]];then
	enable_disable_str+=" --disable-sig-picnic"
fi

if [[ ${ENABLE_KEX_RLWE_NEWHOPE_AVX2} == 1 ]];then
	enable_disable_str+=" --enable-kex-rlwe-newhope-avx2"
fi

# build and run
autoreconf -i
./configure --enable-silent-rules ${enable_disable_str}
make clean
make
make test

for f in $(ls .travis/*-check.sh); do
	bash $f;
done
