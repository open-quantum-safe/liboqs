#!/bin/bash

autoreconf -i
enable_disable_str=
if [[ ${USE_OPENSSL} == 1 ]];then
  enable_disable_str="--enable-openssl"
fi

if [[ ${AES_NI} == 0 ]];then
  enable_disable_str=${enable_disable_str}" --disable-aes-ni"
fi

if [[ ${ENABLE_CODE_MCBITS} == 1 ]];then
  enable_disable_str="--enable-mcbits"
fi

if [[ ${ENABLE_NTRU} == 1 ]];then
  enable_disable_str="--enable-ntru"
fi

./configure ${enable_disable_str}
make clean
make
make test
for f in $(ls .travis/*-check.sh); do bash $f; done


