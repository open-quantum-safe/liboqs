#!/bin/bash

set -e

if [ "x${PQCLEAN_DIR}" == "x" ]; then
    echo "Must specify PQCLEAN_DIR environment variable"
    exit 1
fi

for SCHEME in frodokem640aes frodokem640shake frodokem976aes frodokem976shake; do
    rm -rf src/kem/frodokem/pqclean_${SCHEME}_clean
    cp -pr ${PQCLEAN_DIR}/crypto_kem/${SCHEME}/clean src/kem/frodokem/pqclean_${SCHEME}_clean
    rm src/kem/frodokem/pqclean_${SCHEME}_clean/Makefile*
done