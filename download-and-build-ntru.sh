#!/bin/bash

mkdir -p external 
cd external 
wget https://github.com/NTRUOpenSourceProject/NTRUEncrypt/archive/master.zip
unzip master
cd NTRUEncrypt-master 
./autogen.sh 
./configure
make -j
