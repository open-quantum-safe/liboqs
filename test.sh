#!/bin/bash

set -e

cd build; 
rm -rf * ; 
cmake -GNinja -DOQS_USE_OPENSSL=ON -DBUILD_SHARED_LIBS=ON -DOQS_USE_AES_OPENSSL=ON -DOQS_USE_SHA2_OPENSSL=ON -DOQS_USE_SHA3_OPENSSL=ON  -DOQS_ENABLE_SIG_STFL_LMS=ON -DOQS_ENABLE_SIG_STFL_XMSS=ON -DOQS_ENABLE_SIG_STFL_KEY_SIG_GEN=ON ../
# cmake -GNinja -DCMAKE_C_COMPILER=clang-15 -DCMAKE_BUILD_TYPE=Debug -DUSE_SANITIZER=Address ../
scan-build --status-bugs ninja
cd ../

# echo "==========================="

# echo "test code conventions"
# python3 -m pytest --verbose tests/test_code_conventions.py

# # echo "==========================="

# echo "test KAT"
# python3 tests/test_kat.py -v -k XMSS-SHA2_10_256 --show-capture=stderr
python3 tests/test_kat.py -v -k xmss --show-capture=stderr
# python3 tests/test_kat.py -v -k lms --show-capture=stderr
# ./build/tests/kat_sig_stfl XMSSMT-SHA2_20/2_256 tests/KATs/sig_stfl/xmss/XMSSMT-SHA2_20-2_256.rsp

# echo "==========================="


# echo "test cmdline"
# python3 tests/test_cmdline.py -v -k XMSSMT-SHA2_40/8_256
# python3 tests/test_cmdline.py -v -k xmss
# python3 tests/test_cmdline.py -v -k lms

# ./build/tests/test_sig_stfl XMSSMT-SHA2_40/8_256 tests/KATs/sig_stfl/xmss/XMSSMT-SHA2_40/8_256.rsp

echo "=========================== TEST.SH END ==========================="
