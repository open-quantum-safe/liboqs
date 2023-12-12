#!/bin/sh
# SPDX-License-Identifier: MIT

file_keygen=$2
file_signature=$3
file_verification=$4
build_dir=$5

# input part
prng_output_stream=$(grep "seed: " "$file_keygen" | sed 's/seed: //g')

sig_msg=$(grep "message: " "$file_signature" | sed 's/message: //g')
sig_sk=$(grep "sk: " "$file_signature" | sed 's/sk: //g')
sig_rnd=$(grep "rnd: " "$file_signature" | sed "s/rnd: //g")

verif_sig=$(grep "signature: " "$file_verification" | sed "s/signature: //g")
verif_pk=$(grep "pk: " "$file_verification" | sed "s/pk: //g")
verif_msg=$(grep "message: " "$file_verification" | sed "s/message: //g")

# KAT part
keygen_pk=$(grep "pk: " "$file_keygen")
keygen_sk=$(grep "sk: " "$file_keygen")
sig_signature=$(grep "signature: " "$file_signature")

output=$($build_dir/tests/vectors_sig $1 "$prng_output_stream$sig_rnd" "$sig_msg" "$sig_sk" "$verif_sig" "$verif_pk" "$verif_msg")
if [ $? != 0 ]; then
    exit 1
fi

# Parse output: pk, sk, signature
output_pk=$(echo "$output" | grep "pk: ")
output_sk=$(echo "$output" | grep "sk: ")
output_signature=$(echo "$output" | grep "signature: ")

if [ "$keygen_pk" != "$output_pk" ]; then
    echo "keygen_pk mismatch for $1"
    exit 1
elif [ "$keygen_sk" != "$output_sk" ]; then
    echo "keygen_sk mismatch for $1"
    exit 1
elif [ "$sig_signature" != "$output_signature" ]; then
    echo "sig_signature mismatch for $1"
    exit 1
else
    echo "Vector tests succeeded for $1"
fi
