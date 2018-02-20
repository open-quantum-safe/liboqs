#!/bin/bash

set -e

make clean
make
./test_kem
./example_kem

for f in $(ls .travis/*-check.sh); do
    bash $f;
done
