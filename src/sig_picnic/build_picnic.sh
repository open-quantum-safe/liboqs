#!/bin/bash

mkdir -p external/build
patch -p1 -d external < oqs_sig_picnic.patch
cd external/build
cmake -UWITH_LTO -DWITH_LTO:BOOL=OFF ..
make

