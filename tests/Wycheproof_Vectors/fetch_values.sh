#!/bin/bash

# SPDX-License-Identifier: MIT

# This script fetches the Wycheproof vectors for ML-KEM

# Then fetches Wycheproof vectors for ML-KEM
 commit_hash="4a6c2bf5dc4c0b67c770233ad33961ee653996a0"
 commit_url="https://github.com/C2SP/wycheproof/archive/$commit_hash.tar.gz"
 folder="wycheproof-$commit_hash/testvectors_v1"

wget "$commit_url"
tar xvfz "$commit_hash.tar.gz"

fld_mlkem_test="mlkem_test"

for FLD in "$fld_mlkem_test"
do
    mkdir -p "$FLD" && mv "$folder/$FLD.json" "$FLD"
done

rm "$commit_hash.tar.gz"
rm -rf "wycheproof-$commit_hash"