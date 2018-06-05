#!/bin/bash
#
# Downloads and builds Keccak code pacakage
#

set -e

if [ ! -d "vendor/KeccakCodePackage-master" ];
then
	mkdir -p vendor
	if [ ! -f "vendor/KeccakCodePackage-master.zip" ];
	then
		wget -O vendor/KeccakCodePackage-master.zip https://github.com/gvanas/KeccakCodePackage/archive/master.zip
	fi

	cd vendor
	unzip KeccakCodePackage-master.zip
	cd ..
fi

cd vendor/KeccakCodePackage-master
CFLAGS=-fPIC
export CFLAGS
make generic64/libkeccak.a
