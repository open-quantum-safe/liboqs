#!/bin/bash
#
# Downloads and builds Keccak code pacakage
#

set -e

if [ ! -d "vendor/XKCP-master" ];
then
	mkdir -p vendor
	if [ ! -f "vendor/XKCP-master.zip" ];
	then
		wget -O vendor/XKCP-master.zip https://github.com/XKCP/XKCP/archive/master.zip
	fi

	cd vendor
	unzip XKCP-master.zip
	cd ..
fi

cd vendor/XKCP-master
CFLAGS=-fPIC
export CFLAGS
make generic64/libkeccak.a
