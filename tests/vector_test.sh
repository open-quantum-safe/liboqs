#!/bin/bash
# SPDX-License-Identifier: MIT

if [[ -z "${OQS_BUILD_DIR}" ]]; then
  build_dir=build
else
  build_dir="${OQS_BUILD_DIR}"
fi

file_keygen=tests/PQC\ Intermediate\ Values/Key\ Generation\ --\ ML-DSA-44.txt
file_signature=tests/PQC\ Intermediate\ Values/Signature\ Generation\ --\ ML-DSA-44.txt
file_verification=tests/PQC\ Intermediate\ Values/Signature\ Verification\ --\ ML-DSA-44.txt
scheme_name=ML-DSA-44-ipd

sh tests/vector_test_sig.sh "$scheme_name" "$file_keygen" "$file_signature" "$file_verification" "$build_dir"
if [ $? != 0 ]; then
    exit 1
fi

file_keygen=tests/PQC\ Intermediate\ Values/Key\ Generation\ --\ ML-DSA-65.txt
file_signature=tests/PQC\ Intermediate\ Values/Signature\ Generation\ --\ ML-DSA-65.txt
file_verification=tests/PQC\ Intermediate\ Values/Signature\ Verification\ --\ ML-DSA-65.txt
scheme_name=ML-DSA-65-ipd

sh tests/vector_test_sig.sh "$scheme_name" "$file_keygen" "$file_signature" "$file_verification" "$build_dir"
if [ $? != 0 ]; then
    exit 1
fi

file_keygen=tests/PQC\ Intermediate\ Values/Key\ Generation\ --\ ML-DSA-87.txt
file_signature=tests/PQC\ Intermediate\ Values/Signature\ Generation\ --\ ML-DSA-87.txt
file_verification=tests/PQC\ Intermediate\ Values/Signature\ Verification\ --\ ML-DSA-87.txt
scheme_name=ML-DSA-87-ipd

sh tests/vector_test_sig.sh "$scheme_name" "$file_keygen" "$file_signature" "$file_verification" "$build_dir"
if [ $? != 0 ]; then
    exit 1
fi

file_keygen=tests/PQC\ Intermediate\ Values/Key\ Generation\ --\ ML-KEM-512.txt
file_encaps=tests/PQC\ Intermediate\ Values/Encapsulation\ --\ ML-KEM-512.txt
file_decaps=tests/PQC\ Intermediate\ Values/Decapsulation\ --\ ML-KEM-512.txt
scheme_name=ML-KEM-512-ipd

sh tests/vector_test_kem.sh "$scheme_name" "$file_keygen" "$file_encaps" "$file_decaps" "$build_dir"
if [ $? != 0 ]; then
    exit 1
fi

file_keygen=tests/PQC\ Intermediate\ Values/Key\ Generation\ --\ ML-KEM-768.txt
file_encaps=tests/PQC\ Intermediate\ Values/Encapsulation\ --\ ML-KEM-768.txt
file_decaps=tests/PQC\ Intermediate\ Values/Decapsulation\ --\ ML-KEM-768.txt
scheme_name=ML-KEM-768-ipd

sh tests/vector_test_kem.sh "$scheme_name" "$file_keygen" "$file_encaps" "$file_decaps" "$build_dir"
if [ $? != 0 ]; then
    exit 1
fi

file_keygen=tests/PQC\ Intermediate\ Values/Key\ Generation\ --\ ML-KEM-1024.txt
file_encaps=tests/PQC\ Intermediate\ Values/Encapsulation\ --\ ML-KEM-1024.txt
file_decaps=tests/PQC\ Intermediate\ Values/Decapsulation\ --\ ML-KEM-1024.txt
scheme_name=ML-KEM-1024-ipd

sh tests/vector_test_kem.sh "$scheme_name" "$file_keygen" "$file_encaps" "$file_decaps" "$build_dir"
if [ $? != 0 ]; then
    exit 1
fi