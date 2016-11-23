#!/bin/bash

autoreconf -i
if [[ ${USE_OPENSSL} == 1 &&  ${AES_NI} == 0 ]];then 
  ./configure --disable-aes-ni; make clean; make; make test
  ./configure --enable-openssl; make clean; make; make test  
else
  ./configure; make; make test
  ./configure --disable-aes-ni; make clean; make; make test
  ./configure --enable-openssl; make clean; make; make test  
fi

bash .travis-style-check.sh
bash .travis-global-namespace-check.sh
bash .travis-banned-functions-check.sh
bash .travis-character-encoding-check.sh
