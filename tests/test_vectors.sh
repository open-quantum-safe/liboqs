#!/bin/bash
# SPDX-License-Identifier: MIT

if [[ -z "${OQS_BUILD_DIR}" ]]; then
  build_dir=build
else
  build_dir="${OQS_BUILD_DIR}"
fi

if [[ "$1" =~ ^ML-DSA-(44|65|87)(-ipd)?$ ]]; then

  scheme_name=$1
  file="tests/PQC_Intermediate_Values/${1%-ipd}.txt"

  sh tests/test_sig_vectors.sh "$scheme_name" "$file" "$build_dir"
  if [ $? != 0 ]; then
      exit 1
  fi

elif [[ "$1" =~ ^ML-KEM-(512|768|1024)(-ipd)?$ ]]; then

  scheme_name=$1
  file="tests/PQC_Intermediate_Values/${1%-ipd}.txt"
  
  sh tests/test_kem_vectors.sh "$scheme_name" "$file" "$build_dir"
  if [ $? != 0 ]; then
      exit 1
  fi

else
  echo "$1 not supported"
fi
