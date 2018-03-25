#!/bin/bash

set -e

make clean
make
make docs
./test_kem
./example_kem

for f in $(ls .travis/*-check.sh); do
    bash $f;
done
