#!/bin/bash

# sudo apt-get install gcc-arm-linux-gnueabi

set -e

CBUILD="$(gcc -dumpmachine)"
CHOST=arm-linux-gnueabi

mkdir build-arm
cd build-arm
PREFIX=`pwd`
cd ..

wget https://www.openssl.org/source/openssl-1.1.1a.tar.gz
tar xf openssl-1.1.1a.tar.gz
cd openssl-1.1.1a
CC="${CHOST}-gcc" ./Configure linux-armv4 no-shared no-dso no-hw no-engine no-tests -DOPENSSL_NO_SECURE_MEMORY --prefix="${PREFIX}"
make -j
make install
cd ..

cd liboqs
autoreconf -i
hacks=(
	gcc_cv_compiler=true	# Detecting at this phase isn't good for cross compilation
	--disable-aes-ni	# This should be conditionalized on x86 host
    --disable-sig-picnic
	CFLAGS=-D_ARM_		# Several files aren't using the right define
)

./configure --disable-shared --enable-static --host="${CHOST}" --build="$CBUILD" CC="${CHOST}-gcc" --with-openssl-dir="${PREFIX}" "${hacks[@]}"
make -j
make -j check LDFLAGS="-L${PREFIX}/lib -static"
