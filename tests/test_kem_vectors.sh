#!/bin/bash
# SPDX-License-Identifier: MIT

file_keygen=$2
file_encaps=$3
file_decaps=$4
build_dir=$5

# input part
keygen_z=$(grep "z: " "$file_keygen" | sed 's/z: //g')
keygen_d=$(grep "d: " "$file_keygen" | sed 's/d: //g')

encaps_m=$(grep "m: " "$file_encaps" | sed 's/m: //g')
encaps_ek=$(grep "ek: " "$file_encaps" | sed 's/ek: //g')
encaps_k=$(grep "K: " "$file_encaps" | sed 's/K: //g')

decaps_dk=$(grep "dk: " "$file_decaps" | sed 's/dk: //g')
decaps_c=$(grep "c: " "$file_decaps" | sed 's/c: //g')
decaps_kprime=$(grep "KPrime: " "$file_decaps" | sed 's/KPrime: //g')

# KAT part
keygen_pk=$(grep "ek: " "$file_keygen")
keygen_sk=$(grep "dk: " "$file_keygen")

encaps_c=$(grep "c: " "$file_encaps")
encaps_K=$(grep "K: " "$file_encaps")

output=$($build_dir/tests/vectors_kem $1 "$keygen_z$keygen_d$encaps_m" "$encaps_ek" "$encaps_k" "$decaps_dk" "$decaps_c" "$decaps_kprime")
if [ $? != 0 ]; then
    exit 1
fi

# Parse output: pk, sk, signature
output_pk=$(echo "$output" | grep "ek: ")
output_sk=$(echo "$output" | grep "dk: ")
output_c=$(echo "$output" | grep "c: ")
output_K=$(echo "$output" | grep "K: ")

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
elif [ "$encaps_K" != "$output_K" ]; then
    echo "encaps_K mismatch for $1"
    exit 1
else
    echo "Vector tests succeeded for $1"
fi
