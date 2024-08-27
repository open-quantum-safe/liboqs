#!/bin/bash
# SPDX-License-Identifier: MIT

if [[ -z "${OQS_BUILD_DIR}" ]]; then
  build_dir=build
else
  build_dir="${OQS_BUILD_DIR}"
fi

if [[ "$1" = "ML-DSA-44-ipd" || "$1" = "ML-DSA-44" ]]; then

  file=tests/PQC_Intermediate_Values/ML-DSA-44.txt
  scheme_name=$1
  
  sh tests/test_sig_vectors.sh "$scheme_name" "$file" "$build_dir"
  if [ $? != 0 ]; then
      exit 1
  fi

elif [[ "$1" = "ML-DSA-65-ipd" || "$1" = "ML-DSA-65" ]]; then

  file=tests/PQC_Intermediate_Values/ML-DSA-65.txt
  scheme_name=$1
  
  sh tests/test_sig_vectors.sh "$scheme_name" "$file" "$build_dir"
  if [ $? != 0 ]; then
      exit 1
  fi

elif [[ "$1" = "ML-DSA-87-ipd" || "$1" = "ML-DSA-87" ]]; then

  file=tests/PQC_Intermediate_Values/ML-DSA-87.txt
  scheme_name=$1
  
  sh tests/test_sig_vectors.sh "$scheme_name" "$file" "$build_dir"
  if [ $? != 0 ]; then
      exit 1
  fi

else
  echo "$1 not supported"
fi
