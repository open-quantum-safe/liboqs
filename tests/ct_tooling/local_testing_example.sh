#!/bin/bash
# SPDX-License-Identifier: MIT

# Iterate through the compilation options provided by the framework to execute constant time tests
# Adjust valgrind-varlat/memsan accordingly (note that gcc uses -fno-tree-vectorize while clang uses -fno-vectorize)

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

for compiler_version in gcc gcc-14 clang clang-20; do
    if [[ "$compiler_version" == *"clang"* ]]; then
        vec_flag="-fno-vectorize"
    else
        vec_flag="-fno-tree-vectorize"
    fi

    for liboqs_build in generic auto; do
        for opt_flag in -O0 -O1 -O2 -O3 -Os -Ofast "-O2 $vec_flag" "-O3 $vec_flag"; do
            ./ct_test.sh valgrind-varlat "$compiler_version" "$liboqs_build" "$opt_flag" all
        done
    done
done