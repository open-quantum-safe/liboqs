#!/bin/bash

set -euo pipefail

if [[ -z $LIBOQS_DIR ]]; then
    echo "ERROR: LIBOQS_DIR is not set"
    exit 1
fi

builddir=$LIBOQS_DIR/build

if [[ -d $builddir ]]; then
    echo "$builddir will be removed"
    read -p "Enter yes to continue: " remove_build_dir
    if [[ $remove_build_dir == "yes" || $remove_build_dir == "YES" ]]; then
        rm -rf $builddir
    else
        echo "Build cancelled"
        exit 0
    fi
fi
mkdir -p $builddir

cd $LIBOQS_DIR
./scripts/copy_from_upstream/copy_from_upstream.py -d copy

cd $builddir

cmake -GNinja \
    -DOQS_MINIMAL_BUILD="KEM_hqc_1;KEM_hqc_3;KEM_hqc_5" \
    $LIBOQS_DIR \
    && ninja

cd $LIBOQS_DIR
python -m pytest \
    -k "(not test_style) and (not test_spdx)" \
    --ignore tests/test_hash.py | tee $builddir/pytest.log
echo "pytest results written to $builddir/pytest.log"
