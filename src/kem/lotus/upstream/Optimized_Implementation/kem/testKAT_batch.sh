#! /bin/bash
sec_array=(128 192 256)
kat_dir=../../KAT/kem

for sec in ${sec_array[@]}; do
    echo lotus${sec}-kem-opt
    make SEC=$sec > /dev/null 2>&1
    ./PQCgenKAT_kem
    make SEC=$sec clean > /dev/null 2>&1
    diff -qs PQCkemKAT_*.rsp ${kat_dir}/lotus${sec}/PQCkemKAT_*.rsp
    rm PQCkemKAT_*
done
