#!/bin/bash
# SPDX-License-Identifier: MIT

# This script fetches the ACVP vectors for ML-DSA and ML-KEM

rel_name="1.1.0.35"
rel_url="https://github.com/usnistgov/ACVP-Server/archive/refs/tags/v$rel_name.tar.gz"
fld_vecs="ACVP-Server-$rel_name/gen-val/json-files"

wget "$rel_url"
tar xvfz "v$rel_name.tar.gz"

fld_mldsa_kg="ML-DSA-keyGen-FIPS204"
fld_mldsa_sig="ML-DSA-sigGen-FIPS204"
fld_mldsa_ver="ML-DSA-sigVer-FIPS204"
fld_mlkem_endec="ML-KEM-encapDecap-FIPS203"
fld_mlkem_kg="ML-KEM-keyGen-FIPS203"

for FLD in "$fld_mldsa_kg" "$fld_mldsa_sig" "$fld_mldsa_ver" "$fld_mlkem_endec" "$fld_mlkem_kg"
do
    mkdir -p "$FLD" && mv "$fld_vecs/$FLD/internalProjection.json" "$FLD"
done

rm "v$rel_name.tar.gz"
rm -rf "ACVP-Server-$rel_name"
