#!/bin/bash

export CC=$CC_OQS

autoreconf -i
enable_disable_str=
if [[ ${USE_OPENSSL} == 1 ]];then
  enable_disable_str=" --enable-openssl"
  if [[ ! -z "${OPENSSL_DIR// }" ]];then
     enable_disable_str+=" --with-openssl-dir=${OPENSSL_DIR}"
  fi
fi

if [[ ${AES_NI} == 0 ]];then
  enable_disable_str+=" --disable-aes-ni"
fi

if [[ ${ENABLE_CODE_MCBITS} == 1 ]];then
  enable_disable_str+=" --enable-kex-code-mcbits"
  if [[ ! -z "${SODIUM_DIR// }" ]];then
     enable_disable_str+=" --with-sodium-dir=${SODIUM_DIR}"
  fi
fi

if [[ ${ENABLE_SIDH_IQC_REF} == 1 ]];then
  enable_disable_str+=" --enable-kex-sidh-iqc-ref"
    if [[ ! -z "${GMP_DIR// }" ]];then
     enable_disable_str+=" --with-gmp-dir=${GMP_DIR}"
  fi
fi

if [[ ${USE_PICNIC} == 1 ]];then
  enable_disable_str+=" --enable-picnic"
fi



./configure --enable-silent-rules ${enable_disable_str}
make clean
make
make test
for f in $(ls .travis/*-check.sh); do bash $f; done


