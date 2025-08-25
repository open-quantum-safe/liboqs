#!/bin/bash
# SPDX-License-Identifier: MIT

# This script fetches the ACVP vectors for ML-DSA and ML-KEM

rel_name="1.1.0.40"
rel_url="https://github.com/usnistgov/ACVP-Server/archive/refs/tags/v$rel_name.tar.gz"
fld_vecs="ACVP-Server-$rel_name/gen-val/json-files"

wget "$rel_url"
tar xvfz "v$rel_name.tar.gz"

flds=(
  "ML-DSA-keyGen-FIPS204" "ML-DSA-sigGen-FIPS204" "ML-DSA-sigVer-FIPS204"
  "ML-KEM-encapDecap-FIPS203" "ML-KEM-keyGen-FIPS203"
  "SLH-DSA-keyGen-FIPS205" "SLH-DSA-sigGen-FIPS205" "SLH-DSA-sigVer-FIPS205"
)

for FLD in "${flds[@]}"
do
    mkdir -p "$FLD" && mv "$fld_vecs/$FLD/internalProjection.json" "$FLD"
done

rm "v$rel_name.tar.gz"
rm -rf "ACVP-Server-$rel_name"