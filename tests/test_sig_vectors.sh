#!/bin/bash
# SPDX-License-Identifier: MIT

file=$2
build_dir=$3

# input part
prng_output_stream=$(grep "seed: " "$file" | sed 's/seed: //g')

sig_msg=$(grep "sig_message: " "$file" | sed 's/sig_message: //g')
sig_sk=$(grep "sig_sk: " "$file" | sed 's/sig_sk: //g')
sig_rnd=$(grep "rnd: " "$file" | sed "s/rnd: //g")

verif_sig=$(grep "verif_signature: " "$file" | sed "s/verif_signature: //g")
verif_pk=$(grep "verif_pk: " "$file" | sed "s/verif_pk: //g")
verif_msg=$(grep "verif_message: " "$file" | sed "s/verif_message: //g")

# KAT part
keygen_pk=$(grep "keygen_pk: " "$file")
keygen_sk=$(grep "keygen_sk: " "$file")
sig_signature=$(grep "sig_signature: " "$file")

output=$($build_dir/tests/vectors_sig $1 "$prng_output_stream$sig_rnd" "$sig_msg" "$sig_sk" "$verif_sig" "$verif_pk" "$verif_msg")
if [ $? != 0 ]; then
    exit 1
fi

# Parse output: pk, sk, signature
output_pk=$(echo "$output" | grep "pk: " | sed "s/pk: /keygen_pk: /g")
output_sk=$(echo "$output" | grep "sk: " | sed "s/sk: /keygen_sk: /g")
output_signature=$(echo "$output" | grep "signature: " | sed "s/signature: /sig_signature: /g")

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
