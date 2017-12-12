#!/bin/bash

mkdir -p external/build
# check if patch has already been applied
patch -p1 -N -f --dry-run --silent -d external < oqs_sig_picnic.patch 2>/dev/null
if [ $? -eq 0 ];
then
    #apply the patch
    echo Patching Picnic external
    patch -p1 -N -f -d external < oqs_sig_picnic.patch
fi
cd external/build
cmake -UWITH_LTO -DWITH_LTO:BOOL=OFF ..
make

