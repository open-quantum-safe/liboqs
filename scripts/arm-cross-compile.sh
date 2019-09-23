#!/bin/bash

# sudo apt-get install gcc-arm-linux-gnueabi libc6-dev-armel-cross

set -e

CBUILD="$(gcc -dumpmachine)"
CHOST=arm-linux-gnueabi

rm -rf build-arm
mkdir build-arm
PREFIX="$(pwd)/build-arm"

if [ ! -f "openssl-1.1.1a.tar.gz" ]; then
    wget https://www.openssl.org/source/openssl-1.1.1a.tar.gz
fi
rm -rf openssl-1.1.1a
tar zxf openssl-1.1.1a.tar.gz
cd openssl-1.1.1a
CC="${CHOST}-gcc" ./Configure linux-armv4 no-shared no-dso no-hw no-engine no-tests -DOPENSSL_NO_SECURE_MEMORY --prefix="${PREFIX}"
make -j
make install_dev
cd ..

autoreconf -i
hacks=(
	gcc_cv_compiler=true	# Detecting at this phase isn't good for cross compilation
	CFLAGS=-D_ARM_		# Several files aren't using the right define
	--disable-sig-qtesla	# qTesla fails on armhf and armel
)

./configure --disable-shared --enable-static --host="${CHOST}" --build="$CBUILD" CC="${CHOST}-gcc" --with-openssl="${PREFIX}" "${hacks[@]}"
make -j
make -j check LDFLAGS="-L${PREFIX}/lib -static -pthread"
