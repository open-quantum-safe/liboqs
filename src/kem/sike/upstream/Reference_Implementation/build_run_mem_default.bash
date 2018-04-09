#!/bin/bash
# Memory analysis using massif-valgrind and massif-cherrypick (compiled with clang)
mkdir -p build
cd build
cmake ..
make
valgrind --massif-out-file=503.all.out --tool=massif --stacks=yes --detailed-freq=1 ./test/sike_test SIKEp503 sike
echo "crypto_kem_keypair_gen" >> 503.txt
massif-cherrypick 503.all.out crypto_kem_keypair_gen >> 503.txt
echo "crypto_kem_enc_gen" >> 503.txt
massif-cherrypick 503.all.out crypto_kem_enc_gen >> 503.txt
echo "crypto_kem_dec_gen" >> 503.txt
massif-cherrypick 503.all.out crypto_kem_dec_gen >> 503.txt
valgrind --massif-out-file=751.all.out --tool=massif --stacks=yes --detailed-freq=1 ./test/sike_test SIKEp751 sike
echo "crypto_kem_keypair_gen" >> 751.txt
massif-cherrypick 751.all.out crypto_kem_keypair_gen >> 751.txt
echo "crypto_kem_enc_gen" >> 751.txt
massif-cherrypick 751.all.out crypto_kem_enc_gen >> 751.txt
echo "crypto_kem_dec_gen" >> 751.txt
massif-cherrypick 751.all.out crypto_kem_dec_gen >> 751.txt
valgrind --massif-out-file=964.all.out --tool=massif --stacks=yes --detailed-freq=1 ./test/sike_test SIKEp964 sike
echo "crypto_kem_keypair_gen" >> 964.txt
massif-cherrypick 964.all.out crypto_kem_keypair_gen >> 964.txt
echo "crypto_kem_enc_gen" >> 964.txt
massif-cherrypick 964.all.out crypto_kem_enc_gen >> 964.txt
echo "crypto_kem_dec_gen" >> 964.txt
massif-cherrypick 964.all.out crypto_kem_dec_gen >> 964.txt
