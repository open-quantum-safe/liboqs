#!/bin/bash

set -e

if [ -z ${ARCH+x} ]; then
    echo "ARCH environment variable not set."
    exit 1
fi

if [ -z ${CC_OVERRIDE+x} ]; then
    echo "CC_OVERRIDE environment variable not set."
    exit 1
fi

make clean
make "ARCH=${ARCH}" "CC=${CC_OVERRIDE}"
make docs
./test_kem
./example_kem

for f in $(ls .travis/*-check.sh); do
    bash $f;
done
