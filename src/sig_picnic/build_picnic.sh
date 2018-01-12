#!/bin/bash

mkdir -p external/build
cd external/build
# make picnic
cmake -UWITH_LTO -DWITH_LTO:BOOL=OFF ..
make

