#!/bin/bash

mkdir -p external/build
cd external/build
cmake -UWITH_LTO -DWITH_LTO:BOOL=OFF ..
make

