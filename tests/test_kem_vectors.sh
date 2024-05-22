#!/bin/bash
# SPDX-License-Identifier: MIT

file=$2
build_dir=$3

# input part
keygen_z=$(grep "keygen_z: " "$file" | sed 's/keygen_z: //g')
keygen_d=$(grep "keygen_d: " "$file" | sed 's/keygen_d: //g')

encaps_m=$(grep "encaps_m: " "$file" | sed 's/encaps_m: //g')
encaps_ek=$(grep "encaps_ek: " "$file" | sed 's/encaps_ek: //g')
encaps_k=$(grep "encaps_K: " "$file" | sed 's/encaps_K: //g')

decaps_dk=$(grep "decaps_dk: " "$file" | sed 's/decaps_dk: //g')
decaps_c=$(grep "decaps_c: " "$file" | sed 's/decaps_c: //g')
decaps_kprime=$(grep "decaps_KPrime: " "$file" | sed 's/decaps_KPrime: //g')
decaps_kbar=$(grep "decaps_KBar: " "$file" | sed 's/decaps_KBar: //g')

# KAT part
keygen_pk=$(grep "keygen_ek: " "$file")
keygen_sk=$(grep "keygen_dk: " "$file")

encaps_c=$(grep "encaps_c: " "$file")
encaps_K=$(grep "encaps_K: " "$file")

output=$($build_dir/tests/vectors_kem $1 "$keygen_z$keygen_d$encaps_m" "$encaps_ek" "$encaps_k" "$decaps_dk" "$decaps_c" "$decaps_kprime" "$decaps_kbar")
if [ $? != 0 ]; then
    echo "$output"
    exit 1
fi

# Parse output: pk, sk, ciphertext
output_pk=$(echo "$output" | grep "ek: " | sed 's/ek: /keygen_ek: /g')
output_sk=$(echo "$output" | grep "dk: " | sed 's/dk: /keygen_dk: /g')
output_c=$(echo "$output" | grep "c: " | sed 's/c: /encaps_c: /g')

if [ "$keygen_pk" != "$output_pk" ]; then
    echo "keygen_pk mismatch for $1"
    echo "$keygen_pk\n$output_pk"
    exit 1
elif [ "$keygen_sk" != "$output_sk" ]; then
    echo "keygen_sk mismatch for $1"
    exit 1
elif [ "$encaps_c" != "$output_c" ]; then
    echo "$encaps_c\n$output_c"
    echo "encaps_c mismatch for $1"
    exit 1
else
    echo "Vector tests succeeded for $1"
fi
