#!/bin/bash

mkdir -p build
cd build
cmake ..
make

if [ "$1" == "p503" ]
then
    cd SIKEp503
    ./kat_SIKEp503
elif [ "$1" == "p751" ]
then
    cd SIKEp751
    ./kat_SIKEp751
elif [ "$1" == "p964" ]
then
    cd SIKEp964
    ./kat_SIKEp964
fi
