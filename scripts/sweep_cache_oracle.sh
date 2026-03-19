#!/bin/bash

set -e

detector=${LIBOQS_DIR}/build/tests/kem_fo_cache_oracle

if [ ! -f "$detector" ] || [ ! -x "$detector" ]; then
    echo "$detector is either not a file or not executable."
    exit 1
fi

mkdir -p ${LIBOQS_DIR}/build/tmpresults
tmpresults=${LIBOQS_DIR}/build/tmpresults
echo "sweep results written to $tmpresults"

echo "Classic-McEliece-348864 0" \
    && sudo $detector Classic-McEliece-348864 0 > $tmpresults/Classic-McEliece-348864_probeloc_0.log
echo "Classic-McEliece-348864 16" \
    && sudo $detector Classic-McEliece-348864 16 > $tmpresults/Classic-McEliece-348864_probeloc_16.log
echo "Classic-McEliece-348864 32" \
    && sudo $detector Classic-McEliece-348864 32 > $tmpresults/Classic-McEliece-348864_probeloc_32.log
echo "Classic-McEliece-348864 6460" \
    && sudo $detector Classic-McEliece-348864 6460 > $tmpresults/Classic-McEliece-348864_probeloc_6460.log
echo "Classic-McEliece-348864 6476" \
    && sudo $detector Classic-McEliece-348864 6476 > $tmpresults/Classic-McEliece-348864_probeloc_6476.log
echo "Classic-McEliece-348864 6491" \
    && sudo $detector Classic-McEliece-348864 6491 > $tmpresults/Classic-McEliece-348864_probeloc_6491.log
echo "Classic-McEliece-348864f 0" \
    && sudo $detector Classic-McEliece-348864f 0 > $tmpresults/Classic-McEliece-348864f_probeloc_0.log
echo "Classic-McEliece-348864f 16" \
    && sudo $detector Classic-McEliece-348864f 16 > $tmpresults/Classic-McEliece-348864f_probeloc_16.log
echo "Classic-McEliece-348864f 32" \
    && sudo $detector Classic-McEliece-348864f 32 > $tmpresults/Classic-McEliece-348864f_probeloc_32.log
echo "Classic-McEliece-348864f 6460" \
    && sudo $detector Classic-McEliece-348864f 6460 > $tmpresults/Classic-McEliece-348864f_probeloc_6460.log
echo "Classic-McEliece-348864f 6476" \
    && sudo $detector Classic-McEliece-348864f 6476 > $tmpresults/Classic-McEliece-348864f_probeloc_6476.log
echo "Classic-McEliece-348864f 6491" \
    && sudo $detector Classic-McEliece-348864f 6491 > $tmpresults/Classic-McEliece-348864f_probeloc_6491.log
echo "Classic-McEliece-460896 0" \
    && sudo $detector Classic-McEliece-460896 0 > $tmpresults/Classic-McEliece-460896_probeloc_0.log
echo "Classic-McEliece-460896 16" \
    && sudo $detector Classic-McEliece-460896 16 > $tmpresults/Classic-McEliece-460896_probeloc_16.log
echo "Classic-McEliece-460896 32" \
    && sudo $detector Classic-McEliece-460896 32 > $tmpresults/Classic-McEliece-460896_probeloc_32.log
echo "Classic-McEliece-460896 13576" \
    && sudo $detector Classic-McEliece-460896 13576 > $tmpresults/Classic-McEliece-460896_probeloc_13576.log
echo "Classic-McEliece-460896 13592" \
    && sudo $detector Classic-McEliece-460896 13592 > $tmpresults/Classic-McEliece-460896_probeloc_13592.log
echo "Classic-McEliece-460896 13607" \
    && sudo $detector Classic-McEliece-460896 13607 > $tmpresults/Classic-McEliece-460896_probeloc_13607.log
echo "Classic-McEliece-460896f 0" \
    && sudo $detector Classic-McEliece-460896f 0 > $tmpresults/Classic-McEliece-460896f_probeloc_0.log
echo "Classic-McEliece-460896f 16" \
    && sudo $detector Classic-McEliece-460896f 16 > $tmpresults/Classic-McEliece-460896f_probeloc_16.log
echo "Classic-McEliece-460896f 32" \
    && sudo $detector Classic-McEliece-460896f 32 > $tmpresults/Classic-McEliece-460896f_probeloc_32.log
echo "Classic-McEliece-460896f 13576" \
    && sudo $detector Classic-McEliece-460896f 13576 > $tmpresults/Classic-McEliece-460896f_probeloc_13576.log
echo "Classic-McEliece-460896f 13592" \
    && sudo $detector Classic-McEliece-460896f 13592 > $tmpresults/Classic-McEliece-460896f_probeloc_13592.log
echo "Classic-McEliece-460896f 13607" \
    && sudo $detector Classic-McEliece-460896f 13607 > $tmpresults/Classic-McEliece-460896f_probeloc_13607.log
echo "Classic-McEliece-6688128 0" \
    && sudo $detector Classic-McEliece-6688128 0 > $tmpresults/Classic-McEliece-6688128_probeloc_0.log
echo "Classic-McEliece-6688128 16" \
    && sudo $detector Classic-McEliece-6688128 16 > $tmpresults/Classic-McEliece-6688128_probeloc_16.log
echo "Classic-McEliece-6688128 32" \
    && sudo $detector Classic-McEliece-6688128 32 > $tmpresults/Classic-McEliece-6688128_probeloc_32.log
echo "Classic-McEliece-6688128 13900" \
    && sudo $detector Classic-McEliece-6688128 13900 > $tmpresults/Classic-McEliece-6688128_probeloc_13900.log
echo "Classic-McEliece-6688128 13916" \
    && sudo $detector Classic-McEliece-6688128 13916 > $tmpresults/Classic-McEliece-6688128_probeloc_13916.log
echo "Classic-McEliece-6688128 13931" \
    && sudo $detector Classic-McEliece-6688128 13931 > $tmpresults/Classic-McEliece-6688128_probeloc_13931.log
echo "Classic-McEliece-6688128f 0" \
    && sudo $detector Classic-McEliece-6688128f 0 > $tmpresults/Classic-McEliece-6688128f_probeloc_0.log
echo "Classic-McEliece-6688128f 16" \
    && sudo $detector Classic-McEliece-6688128f 16 > $tmpresults/Classic-McEliece-6688128f_probeloc_16.log
echo "Classic-McEliece-6688128f 32" \
    && sudo $detector Classic-McEliece-6688128f 32 > $tmpresults/Classic-McEliece-6688128f_probeloc_32.log
echo "Classic-McEliece-6688128f 13900" \
    && sudo $detector Classic-McEliece-6688128f 13900 > $tmpresults/Classic-McEliece-6688128f_probeloc_13900.log
echo "Classic-McEliece-6688128f 13916" \
    && sudo $detector Classic-McEliece-6688128f 13916 > $tmpresults/Classic-McEliece-6688128f_probeloc_13916.log
echo "Classic-McEliece-6688128f 13931" \
    && sudo $detector Classic-McEliece-6688128f 13931 > $tmpresults/Classic-McEliece-6688128f_probeloc_13931.log
echo "Classic-McEliece-6960119 0" \
    && sudo $detector Classic-McEliece-6960119 0 > $tmpresults/Classic-McEliece-6960119_probeloc_0.log
echo "Classic-McEliece-6960119 16" \
    && sudo $detector Classic-McEliece-6960119 16 > $tmpresults/Classic-McEliece-6960119_probeloc_16.log
echo "Classic-McEliece-6960119 32" \
    && sudo $detector Classic-McEliece-6960119 32 > $tmpresults/Classic-McEliece-6960119_probeloc_32.log
echo "Classic-McEliece-6960119 13916" \
    && sudo $detector Classic-McEliece-6960119 13916 > $tmpresults/Classic-McEliece-6960119_probeloc_13916.log
echo "Classic-McEliece-6960119 13932" \
    && sudo $detector Classic-McEliece-6960119 13932 > $tmpresults/Classic-McEliece-6960119_probeloc_13932.log
echo "Classic-McEliece-6960119 13947" \
    && sudo $detector Classic-McEliece-6960119 13947 > $tmpresults/Classic-McEliece-6960119_probeloc_13947.log
echo "Classic-McEliece-6960119f 0" \
    && sudo $detector Classic-McEliece-6960119f 0 > $tmpresults/Classic-McEliece-6960119f_probeloc_0.log
echo "Classic-McEliece-6960119f 16" \
    && sudo $detector Classic-McEliece-6960119f 16 > $tmpresults/Classic-McEliece-6960119f_probeloc_16.log
echo "Classic-McEliece-6960119f 32" \
    && sudo $detector Classic-McEliece-6960119f 32 > $tmpresults/Classic-McEliece-6960119f_probeloc_32.log
echo "Classic-McEliece-6960119f 13916" \
    && sudo $detector Classic-McEliece-6960119f 13916 > $tmpresults/Classic-McEliece-6960119f_probeloc_13916.log
echo "Classic-McEliece-6960119f 13932" \
    && sudo $detector Classic-McEliece-6960119f 13932 > $tmpresults/Classic-McEliece-6960119f_probeloc_13932.log
echo "Classic-McEliece-6960119f 13947" \
    && sudo $detector Classic-McEliece-6960119f 13947 > $tmpresults/Classic-McEliece-6960119f_probeloc_13947.log
echo "Classic-McEliece-8192128 0" \
    && sudo $detector Classic-McEliece-8192128 0 > $tmpresults/Classic-McEliece-8192128_probeloc_0.log
echo "Classic-McEliece-8192128 16" \
    && sudo $detector Classic-McEliece-8192128 16 > $tmpresults/Classic-McEliece-8192128_probeloc_16.log
echo "Classic-McEliece-8192128 32" \
    && sudo $detector Classic-McEliece-8192128 32 > $tmpresults/Classic-McEliece-8192128_probeloc_32.log
echo "Classic-McEliece-8192128 14088" \
    && sudo $detector Classic-McEliece-8192128 14088 > $tmpresults/Classic-McEliece-8192128_probeloc_14088.log
echo "Classic-McEliece-8192128 14104" \
    && sudo $detector Classic-McEliece-8192128 14104 > $tmpresults/Classic-McEliece-8192128_probeloc_14104.log
echo "Classic-McEliece-8192128 14119" \
    && sudo $detector Classic-McEliece-8192128 14119 > $tmpresults/Classic-McEliece-8192128_probeloc_14119.log
echo "Classic-McEliece-8192128f 0" \
    && sudo $detector Classic-McEliece-8192128f 0 > $tmpresults/Classic-McEliece-8192128f_probeloc_0.log
echo "Classic-McEliece-8192128f 16" \
    && sudo $detector Classic-McEliece-8192128f 16 > $tmpresults/Classic-McEliece-8192128f_probeloc_16.log
echo "Classic-McEliece-8192128f 32" \
    && sudo $detector Classic-McEliece-8192128f 32 > $tmpresults/Classic-McEliece-8192128f_probeloc_32.log
echo "Classic-McEliece-8192128f 14088" \
    && sudo $detector Classic-McEliece-8192128f 14088 > $tmpresults/Classic-McEliece-8192128f_probeloc_14088.log
echo "Classic-McEliece-8192128f 14104" \
    && sudo $detector Classic-McEliece-8192128f 14104 > $tmpresults/Classic-McEliece-8192128f_probeloc_14104.log
echo "Classic-McEliece-8192128f 14119" \
    && sudo $detector Classic-McEliece-8192128f 14119 > $tmpresults/Classic-McEliece-8192128f_probeloc_14119.log
echo "eFrodoKEM-1344-AES 0" \
    && sudo $detector eFrodoKEM-1344-AES 0 > $tmpresults/eFrodoKEM-1344-AES_probeloc_0.log
echo "eFrodoKEM-1344-AES 16" \
    && sudo $detector eFrodoKEM-1344-AES 16 > $tmpresults/eFrodoKEM-1344-AES_probeloc_16.log
echo "eFrodoKEM-1344-AES 32" \
    && sudo $detector eFrodoKEM-1344-AES 32 > $tmpresults/eFrodoKEM-1344-AES_probeloc_32.log
echo "eFrodoKEM-1344-AES 43056" \
    && sudo $detector eFrodoKEM-1344-AES 43056 > $tmpresults/eFrodoKEM-1344-AES_probeloc_43056.log
echo "eFrodoKEM-1344-AES 43072" \
    && sudo $detector eFrodoKEM-1344-AES 43072 > $tmpresults/eFrodoKEM-1344-AES_probeloc_43072.log
echo "eFrodoKEM-1344-AES 43087" \
    && sudo $detector eFrodoKEM-1344-AES 43087 > $tmpresults/eFrodoKEM-1344-AES_probeloc_43087.log
echo "eFrodoKEM-1344-SHAKE 0" \
    && sudo $detector eFrodoKEM-1344-SHAKE 0 > $tmpresults/eFrodoKEM-1344-SHAKE_probeloc_0.log
echo "eFrodoKEM-1344-SHAKE 16" \
    && sudo $detector eFrodoKEM-1344-SHAKE 16 > $tmpresults/eFrodoKEM-1344-SHAKE_probeloc_16.log
echo "eFrodoKEM-1344-SHAKE 32" \
    && sudo $detector eFrodoKEM-1344-SHAKE 32 > $tmpresults/eFrodoKEM-1344-SHAKE_probeloc_32.log
echo "eFrodoKEM-1344-SHAKE 43056" \
    && sudo $detector eFrodoKEM-1344-SHAKE 43056 > $tmpresults/eFrodoKEM-1344-SHAKE_probeloc_43056.log
echo "eFrodoKEM-1344-SHAKE 43072" \
    && sudo $detector eFrodoKEM-1344-SHAKE 43072 > $tmpresults/eFrodoKEM-1344-SHAKE_probeloc_43072.log
echo "eFrodoKEM-1344-SHAKE 43087" \
    && sudo $detector eFrodoKEM-1344-SHAKE 43087 > $tmpresults/eFrodoKEM-1344-SHAKE_probeloc_43087.log
echo "eFrodoKEM-640-AES 0" \
    && sudo $detector eFrodoKEM-640-AES 0 > $tmpresults/eFrodoKEM-640-AES_probeloc_0.log
echo "eFrodoKEM-640-AES 16" \
    && sudo $detector eFrodoKEM-640-AES 16 > $tmpresults/eFrodoKEM-640-AES_probeloc_16.log
echo "eFrodoKEM-640-AES 32" \
    && sudo $detector eFrodoKEM-640-AES 32 > $tmpresults/eFrodoKEM-640-AES_probeloc_32.log
echo "eFrodoKEM-640-AES 19856" \
    && sudo $detector eFrodoKEM-640-AES 19856 > $tmpresults/eFrodoKEM-640-AES_probeloc_19856.log
echo "eFrodoKEM-640-AES 19872" \
    && sudo $detector eFrodoKEM-640-AES 19872 > $tmpresults/eFrodoKEM-640-AES_probeloc_19872.log
echo "eFrodoKEM-640-AES 19887" \
    && sudo $detector eFrodoKEM-640-AES 19887 > $tmpresults/eFrodoKEM-640-AES_probeloc_19887.log
echo "eFrodoKEM-640-SHAKE 0" \
    && sudo $detector eFrodoKEM-640-SHAKE 0 > $tmpresults/eFrodoKEM-640-SHAKE_probeloc_0.log
echo "eFrodoKEM-640-SHAKE 16" \
    && sudo $detector eFrodoKEM-640-SHAKE 16 > $tmpresults/eFrodoKEM-640-SHAKE_probeloc_16.log
echo "eFrodoKEM-640-SHAKE 32" \
    && sudo $detector eFrodoKEM-640-SHAKE 32 > $tmpresults/eFrodoKEM-640-SHAKE_probeloc_32.log
echo "eFrodoKEM-640-SHAKE 19856" \
    && sudo $detector eFrodoKEM-640-SHAKE 19856 > $tmpresults/eFrodoKEM-640-SHAKE_probeloc_19856.log
echo "eFrodoKEM-640-SHAKE 19872" \
    && sudo $detector eFrodoKEM-640-SHAKE 19872 > $tmpresults/eFrodoKEM-640-SHAKE_probeloc_19872.log
echo "eFrodoKEM-640-SHAKE 19887" \
    && sudo $detector eFrodoKEM-640-SHAKE 19887 > $tmpresults/eFrodoKEM-640-SHAKE_probeloc_19887.log
echo "eFrodoKEM-976-AES 0" \
    && sudo $detector eFrodoKEM-976-AES 0 > $tmpresults/eFrodoKEM-976-AES_probeloc_0.log
echo "eFrodoKEM-976-AES 16" \
    && sudo $detector eFrodoKEM-976-AES 16 > $tmpresults/eFrodoKEM-976-AES_probeloc_16.log
echo "eFrodoKEM-976-AES 32" \
    && sudo $detector eFrodoKEM-976-AES 32 > $tmpresults/eFrodoKEM-976-AES_probeloc_32.log
echo "eFrodoKEM-976-AES 31264" \
    && sudo $detector eFrodoKEM-976-AES 31264 > $tmpresults/eFrodoKEM-976-AES_probeloc_31264.log
echo "eFrodoKEM-976-AES 31280" \
    && sudo $detector eFrodoKEM-976-AES 31280 > $tmpresults/eFrodoKEM-976-AES_probeloc_31280.log
echo "eFrodoKEM-976-AES 31295" \
    && sudo $detector eFrodoKEM-976-AES 31295 > $tmpresults/eFrodoKEM-976-AES_probeloc_31295.log
echo "eFrodoKEM-976-SHAKE 0" \
    && sudo $detector eFrodoKEM-976-SHAKE 0 > $tmpresults/eFrodoKEM-976-SHAKE_probeloc_0.log
echo "eFrodoKEM-976-SHAKE 16" \
    && sudo $detector eFrodoKEM-976-SHAKE 16 > $tmpresults/eFrodoKEM-976-SHAKE_probeloc_16.log
echo "eFrodoKEM-976-SHAKE 32" \
    && sudo $detector eFrodoKEM-976-SHAKE 32 > $tmpresults/eFrodoKEM-976-SHAKE_probeloc_32.log
echo "eFrodoKEM-976-SHAKE 31264" \
    && sudo $detector eFrodoKEM-976-SHAKE 31264 > $tmpresults/eFrodoKEM-976-SHAKE_probeloc_31264.log
echo "eFrodoKEM-976-SHAKE 31280" \
    && sudo $detector eFrodoKEM-976-SHAKE 31280 > $tmpresults/eFrodoKEM-976-SHAKE_probeloc_31280.log
echo "eFrodoKEM-976-SHAKE 31295" \
    && sudo $detector eFrodoKEM-976-SHAKE 31295 > $tmpresults/eFrodoKEM-976-SHAKE_probeloc_31295.log
echo "FrodoKEM-1344-AES 0" \
    && sudo $detector FrodoKEM-1344-AES 0 > $tmpresults/FrodoKEM-1344-AES_probeloc_0.log
echo "FrodoKEM-1344-AES 16" \
    && sudo $detector FrodoKEM-1344-AES 16 > $tmpresults/FrodoKEM-1344-AES_probeloc_16.log
echo "FrodoKEM-1344-AES 32" \
    && sudo $detector FrodoKEM-1344-AES 32 > $tmpresults/FrodoKEM-1344-AES_probeloc_32.log
echo "FrodoKEM-1344-AES 43056" \
    && sudo $detector FrodoKEM-1344-AES 43056 > $tmpresults/FrodoKEM-1344-AES_probeloc_43056.log
echo "FrodoKEM-1344-AES 43072" \
    && sudo $detector FrodoKEM-1344-AES 43072 > $tmpresults/FrodoKEM-1344-AES_probeloc_43072.log
echo "FrodoKEM-1344-AES 43087" \
    && sudo $detector FrodoKEM-1344-AES 43087 > $tmpresults/FrodoKEM-1344-AES_probeloc_43087.log
echo "FrodoKEM-1344-SHAKE 0" \
    && sudo $detector FrodoKEM-1344-SHAKE 0 > $tmpresults/FrodoKEM-1344-SHAKE_probeloc_0.log
echo "FrodoKEM-1344-SHAKE 16" \
    && sudo $detector FrodoKEM-1344-SHAKE 16 > $tmpresults/FrodoKEM-1344-SHAKE_probeloc_16.log
echo "FrodoKEM-1344-SHAKE 32" \
    && sudo $detector FrodoKEM-1344-SHAKE 32 > $tmpresults/FrodoKEM-1344-SHAKE_probeloc_32.log
echo "FrodoKEM-1344-SHAKE 43056" \
    && sudo $detector FrodoKEM-1344-SHAKE 43056 > $tmpresults/FrodoKEM-1344-SHAKE_probeloc_43056.log
echo "FrodoKEM-1344-SHAKE 43072" \
    && sudo $detector FrodoKEM-1344-SHAKE 43072 > $tmpresults/FrodoKEM-1344-SHAKE_probeloc_43072.log
echo "FrodoKEM-1344-SHAKE 43087" \
    && sudo $detector FrodoKEM-1344-SHAKE 43087 > $tmpresults/FrodoKEM-1344-SHAKE_probeloc_43087.log
echo "FrodoKEM-640-AES 0" \
    && sudo $detector FrodoKEM-640-AES 0 > $tmpresults/FrodoKEM-640-AES_probeloc_0.log
echo "FrodoKEM-640-AES 16" \
    && sudo $detector FrodoKEM-640-AES 16 > $tmpresults/FrodoKEM-640-AES_probeloc_16.log
echo "FrodoKEM-640-AES 32" \
    && sudo $detector FrodoKEM-640-AES 32 > $tmpresults/FrodoKEM-640-AES_probeloc_32.log
echo "FrodoKEM-640-AES 19856" \
    && sudo $detector FrodoKEM-640-AES 19856 > $tmpresults/FrodoKEM-640-AES_probeloc_19856.log
echo "FrodoKEM-640-AES 19872" \
    && sudo $detector FrodoKEM-640-AES 19872 > $tmpresults/FrodoKEM-640-AES_probeloc_19872.log
echo "FrodoKEM-640-AES 19887" \
    && sudo $detector FrodoKEM-640-AES 19887 > $tmpresults/FrodoKEM-640-AES_probeloc_19887.log
echo "FrodoKEM-640-SHAKE 0" \
    && sudo $detector FrodoKEM-640-SHAKE 0 > $tmpresults/FrodoKEM-640-SHAKE_probeloc_0.log
echo "FrodoKEM-640-SHAKE 16" \
    && sudo $detector FrodoKEM-640-SHAKE 16 > $tmpresults/FrodoKEM-640-SHAKE_probeloc_16.log
echo "FrodoKEM-640-SHAKE 32" \
    && sudo $detector FrodoKEM-640-SHAKE 32 > $tmpresults/FrodoKEM-640-SHAKE_probeloc_32.log
echo "FrodoKEM-640-SHAKE 19856" \
    && sudo $detector FrodoKEM-640-SHAKE 19856 > $tmpresults/FrodoKEM-640-SHAKE_probeloc_19856.log
echo "FrodoKEM-640-SHAKE 19872" \
    && sudo $detector FrodoKEM-640-SHAKE 19872 > $tmpresults/FrodoKEM-640-SHAKE_probeloc_19872.log
echo "FrodoKEM-640-SHAKE 19887" \
    && sudo $detector FrodoKEM-640-SHAKE 19887 > $tmpresults/FrodoKEM-640-SHAKE_probeloc_19887.log
echo "FrodoKEM-976-AES 0" \
    && sudo $detector FrodoKEM-976-AES 0 > $tmpresults/FrodoKEM-976-AES_probeloc_0.log
echo "FrodoKEM-976-AES 16" \
    && sudo $detector FrodoKEM-976-AES 16 > $tmpresults/FrodoKEM-976-AES_probeloc_16.log
echo "FrodoKEM-976-AES 32" \
    && sudo $detector FrodoKEM-976-AES 32 > $tmpresults/FrodoKEM-976-AES_probeloc_32.log
echo "FrodoKEM-976-AES 31264" \
    && sudo $detector FrodoKEM-976-AES 31264 > $tmpresults/FrodoKEM-976-AES_probeloc_31264.log
echo "FrodoKEM-976-AES 31280" \
    && sudo $detector FrodoKEM-976-AES 31280 > $tmpresults/FrodoKEM-976-AES_probeloc_31280.log
echo "FrodoKEM-976-AES 31295" \
    && sudo $detector FrodoKEM-976-AES 31295 > $tmpresults/FrodoKEM-976-AES_probeloc_31295.log
echo "FrodoKEM-976-SHAKE 0" \
    && sudo $detector FrodoKEM-976-SHAKE 0 > $tmpresults/FrodoKEM-976-SHAKE_probeloc_0.log
echo "FrodoKEM-976-SHAKE 16" \
    && sudo $detector FrodoKEM-976-SHAKE 16 > $tmpresults/FrodoKEM-976-SHAKE_probeloc_16.log
echo "FrodoKEM-976-SHAKE 32" \
    && sudo $detector FrodoKEM-976-SHAKE 32 > $tmpresults/FrodoKEM-976-SHAKE_probeloc_32.log
echo "FrodoKEM-976-SHAKE 31264" \
    && sudo $detector FrodoKEM-976-SHAKE 31264 > $tmpresults/FrodoKEM-976-SHAKE_probeloc_31264.log
echo "FrodoKEM-976-SHAKE 31280" \
    && sudo $detector FrodoKEM-976-SHAKE 31280 > $tmpresults/FrodoKEM-976-SHAKE_probeloc_31280.log
echo "FrodoKEM-976-SHAKE 31295" \
    && sudo $detector FrodoKEM-976-SHAKE 31295 > $tmpresults/FrodoKEM-976-SHAKE_probeloc_31295.log
echo "HQC-128 0" \
    && sudo $detector HQC-128 0 > $tmpresults/HQC-128_probeloc_0.log
echo "HQC-128 16" \
    && sudo $detector HQC-128 16 > $tmpresults/HQC-128_probeloc_16.log
echo "HQC-128 32" \
    && sudo $detector HQC-128 32 > $tmpresults/HQC-128_probeloc_32.log
echo "HQC-128 2273" \
    && sudo $detector HQC-128 2273 > $tmpresults/HQC-128_probeloc_2273.log
echo "HQC-128 2289" \
    && sudo $detector HQC-128 2289 > $tmpresults/HQC-128_probeloc_2289.log
echo "HQC-128 2304" \
    && sudo $detector HQC-128 2304 > $tmpresults/HQC-128_probeloc_2304.log
echo "HQC-192 0" \
    && sudo $detector HQC-192 0 > $tmpresults/HQC-192_probeloc_0.log
echo "HQC-192 16" \
    && sudo $detector HQC-192 16 > $tmpresults/HQC-192_probeloc_16.log
echo "HQC-192 32" \
    && sudo $detector HQC-192 32 > $tmpresults/HQC-192_probeloc_32.log
echo "HQC-192 4554" \
    && sudo $detector HQC-192 4554 > $tmpresults/HQC-192_probeloc_4554.log
echo "HQC-192 4570" \
    && sudo $detector HQC-192 4570 > $tmpresults/HQC-192_probeloc_4570.log
echo "HQC-192 4585" \
    && sudo $detector HQC-192 4585 > $tmpresults/HQC-192_probeloc_4585.log
echo "HQC-256 0" \
    && sudo $detector HQC-256 0 > $tmpresults/HQC-256_probeloc_0.log
echo "HQC-256 16" \
    && sudo $detector HQC-256 16 > $tmpresults/HQC-256_probeloc_16.log
echo "HQC-256 32" \
    && sudo $detector HQC-256 32 > $tmpresults/HQC-256_probeloc_32.log
echo "HQC-256 7285" \
    && sudo $detector HQC-256 7285 > $tmpresults/HQC-256_probeloc_7285.log
echo "HQC-256 7301" \
    && sudo $detector HQC-256 7301 > $tmpresults/HQC-256_probeloc_7301.log
echo "HQC-256 7316" \
    && sudo $detector HQC-256 7316 > $tmpresults/HQC-256_probeloc_7316.log
echo "Kyber1024 0" \
    && sudo $detector Kyber1024 0 > $tmpresults/Kyber1024_probeloc_0.log
echo "Kyber1024 16" \
    && sudo $detector Kyber1024 16 > $tmpresults/Kyber1024_probeloc_16.log
echo "Kyber1024 32" \
    && sudo $detector Kyber1024 32 > $tmpresults/Kyber1024_probeloc_32.log
echo "Kyber1024 3136" \
    && sudo $detector Kyber1024 3136 > $tmpresults/Kyber1024_probeloc_3136.log
echo "Kyber1024 3152" \
    && sudo $detector Kyber1024 3152 > $tmpresults/Kyber1024_probeloc_3152.log
echo "Kyber1024 3167" \
    && sudo $detector Kyber1024 3167 > $tmpresults/Kyber1024_probeloc_3167.log
echo "Kyber512 0" \
    && sudo $detector Kyber512 0 > $tmpresults/Kyber512_probeloc_0.log
echo "Kyber512 16" \
    && sudo $detector Kyber512 16 > $tmpresults/Kyber512_probeloc_16.log
echo "Kyber512 32" \
    && sudo $detector Kyber512 32 > $tmpresults/Kyber512_probeloc_32.log
echo "Kyber512 1600" \
    && sudo $detector Kyber512 1600 > $tmpresults/Kyber512_probeloc_1600.log
echo "Kyber512 1616" \
    && sudo $detector Kyber512 1616 > $tmpresults/Kyber512_probeloc_1616.log
echo "Kyber512 1631" \
    && sudo $detector Kyber512 1631 > $tmpresults/Kyber512_probeloc_1631.log
echo "Kyber768 0" \
    && sudo $detector Kyber768 0 > $tmpresults/Kyber768_probeloc_0.log
echo "Kyber768 16" \
    && sudo $detector Kyber768 16 > $tmpresults/Kyber768_probeloc_16.log
echo "Kyber768 32" \
    && sudo $detector Kyber768 32 > $tmpresults/Kyber768_probeloc_32.log
echo "Kyber768 2368" \
    && sudo $detector Kyber768 2368 > $tmpresults/Kyber768_probeloc_2368.log
echo "Kyber768 2384" \
    && sudo $detector Kyber768 2384 > $tmpresults/Kyber768_probeloc_2384.log
echo "Kyber768 2399" \
    && sudo $detector Kyber768 2399 > $tmpresults/Kyber768_probeloc_2399.log
echo "NTRU-HPS-2048-509 0" \
    && sudo $detector NTRU-HPS-2048-509 0 > $tmpresults/NTRU-HPS-2048-509_probeloc_0.log
echo "NTRU-HPS-2048-509 16" \
    && sudo $detector NTRU-HPS-2048-509 16 > $tmpresults/NTRU-HPS-2048-509_probeloc_16.log
echo "NTRU-HPS-2048-509 32" \
    && sudo $detector NTRU-HPS-2048-509 32 > $tmpresults/NTRU-HPS-2048-509_probeloc_32.log
echo "NTRU-HPS-2048-509 903" \
    && sudo $detector NTRU-HPS-2048-509 903 > $tmpresults/NTRU-HPS-2048-509_probeloc_903.log
echo "NTRU-HPS-2048-509 919" \
    && sudo $detector NTRU-HPS-2048-509 919 > $tmpresults/NTRU-HPS-2048-509_probeloc_919.log
echo "NTRU-HPS-2048-509 934" \
    && sudo $detector NTRU-HPS-2048-509 934 > $tmpresults/NTRU-HPS-2048-509_probeloc_934.log
echo "NTRU-HPS-2048-677 0" \
    && sudo $detector NTRU-HPS-2048-677 0 > $tmpresults/NTRU-HPS-2048-677_probeloc_0.log
echo "NTRU-HPS-2048-677 16" \
    && sudo $detector NTRU-HPS-2048-677 16 > $tmpresults/NTRU-HPS-2048-677_probeloc_16.log
echo "NTRU-HPS-2048-677 32" \
    && sudo $detector NTRU-HPS-2048-677 32 > $tmpresults/NTRU-HPS-2048-677_probeloc_32.log
echo "NTRU-HPS-2048-677 1202" \
    && sudo $detector NTRU-HPS-2048-677 1202 > $tmpresults/NTRU-HPS-2048-677_probeloc_1202.log
echo "NTRU-HPS-2048-677 1218" \
    && sudo $detector NTRU-HPS-2048-677 1218 > $tmpresults/NTRU-HPS-2048-677_probeloc_1218.log
echo "NTRU-HPS-2048-677 1233" \
    && sudo $detector NTRU-HPS-2048-677 1233 > $tmpresults/NTRU-HPS-2048-677_probeloc_1233.log
echo "NTRU-HPS-4096-1229 0" \
    && sudo $detector NTRU-HPS-4096-1229 0 > $tmpresults/NTRU-HPS-4096-1229_probeloc_0.log
echo "NTRU-HPS-4096-1229 16" \
    && sudo $detector NTRU-HPS-4096-1229 16 > $tmpresults/NTRU-HPS-4096-1229_probeloc_16.log
echo "NTRU-HPS-4096-1229 32" \
    && sudo $detector NTRU-HPS-4096-1229 32 > $tmpresults/NTRU-HPS-4096-1229_probeloc_32.log
echo "NTRU-HPS-4096-1229 2334" \
    && sudo $detector NTRU-HPS-4096-1229 2334 > $tmpresults/NTRU-HPS-4096-1229_probeloc_2334.log
echo "NTRU-HPS-4096-1229 2350" \
    && sudo $detector NTRU-HPS-4096-1229 2350 > $tmpresults/NTRU-HPS-4096-1229_probeloc_2350.log
echo "NTRU-HPS-4096-1229 2365" \
    && sudo $detector NTRU-HPS-4096-1229 2365 > $tmpresults/NTRU-HPS-4096-1229_probeloc_2365.log
echo "NTRU-HPS-4096-821 0" \
    && sudo $detector NTRU-HPS-4096-821 0 > $tmpresults/NTRU-HPS-4096-821_probeloc_0.log
echo "NTRU-HPS-4096-821 16" \
    && sudo $detector NTRU-HPS-4096-821 16 > $tmpresults/NTRU-HPS-4096-821_probeloc_16.log
echo "NTRU-HPS-4096-821 32" \
    && sudo $detector NTRU-HPS-4096-821 32 > $tmpresults/NTRU-HPS-4096-821_probeloc_32.log
echo "NTRU-HPS-4096-821 1558" \
    && sudo $detector NTRU-HPS-4096-821 1558 > $tmpresults/NTRU-HPS-4096-821_probeloc_1558.log
echo "NTRU-HPS-4096-821 1574" \
    && sudo $detector NTRU-HPS-4096-821 1574 > $tmpresults/NTRU-HPS-4096-821_probeloc_1574.log
echo "NTRU-HPS-4096-821 1589" \
    && sudo $detector NTRU-HPS-4096-821 1589 > $tmpresults/NTRU-HPS-4096-821_probeloc_1589.log
echo "NTRU-HRSS-1373 0" \
    && sudo $detector NTRU-HRSS-1373 0 > $tmpresults/NTRU-HRSS-1373_probeloc_0.log
echo "NTRU-HRSS-1373 16" \
    && sudo $detector NTRU-HRSS-1373 16 > $tmpresults/NTRU-HRSS-1373_probeloc_16.log
echo "NTRU-HRSS-1373 32" \
    && sudo $detector NTRU-HRSS-1373 32 > $tmpresults/NTRU-HRSS-1373_probeloc_32.log
echo "NTRU-HRSS-1373 2951" \
    && sudo $detector NTRU-HRSS-1373 2951 > $tmpresults/NTRU-HRSS-1373_probeloc_2951.log
echo "NTRU-HRSS-1373 2967" \
    && sudo $detector NTRU-HRSS-1373 2967 > $tmpresults/NTRU-HRSS-1373_probeloc_2967.log
echo "NTRU-HRSS-1373 2982" \
    && sudo $detector NTRU-HRSS-1373 2982 > $tmpresults/NTRU-HRSS-1373_probeloc_2982.log
echo "NTRU-HRSS-701 0" \
    && sudo $detector NTRU-HRSS-701 0 > $tmpresults/NTRU-HRSS-701_probeloc_0.log
echo "NTRU-HRSS-701 16" \
    && sudo $detector NTRU-HRSS-701 16 > $tmpresults/NTRU-HRSS-701_probeloc_16.log
echo "NTRU-HRSS-701 32" \
    && sudo $detector NTRU-HRSS-701 32 > $tmpresults/NTRU-HRSS-701_probeloc_32.log
echo "NTRU-HRSS-701 1418" \
    && sudo $detector NTRU-HRSS-701 1418 > $tmpresults/NTRU-HRSS-701_probeloc_1418.log
echo "NTRU-HRSS-701 1434" \
    && sudo $detector NTRU-HRSS-701 1434 > $tmpresults/NTRU-HRSS-701_probeloc_1434.log
echo "NTRU-HRSS-701 1449" \
    && sudo $detector NTRU-HRSS-701 1449 > $tmpresults/NTRU-HRSS-701_probeloc_1449.log
echo "sntrup761 0" \
    && sudo $detector sntrup761 0 > $tmpresults/sntrup761_probeloc_0.log
echo "sntrup761 16" \
    && sudo $detector sntrup761 16 > $tmpresults/sntrup761_probeloc_16.log
echo "sntrup761 32" \
    && sudo $detector sntrup761 32 > $tmpresults/sntrup761_probeloc_32.log
echo "sntrup761 1731" \
    && sudo $detector sntrup761 1731 > $tmpresults/sntrup761_probeloc_1731.log
echo "sntrup761 1747" \
    && sudo $detector sntrup761 1747 > $tmpresults/sntrup761_probeloc_1747.log
echo "sntrup761 1762" \
    && sudo $detector sntrup761 1762 > $tmpresults/sntrup761_probeloc_1762.log
