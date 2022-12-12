#ifndef PQCLEAN_SNTRUP653_AVX2_API_H
#define PQCLEAN_SNTRUP653_AVX2_API_H

#include <stdint.h>


#define PQCLEAN_SNTRUP653_AVX2_CRYPTO_ALGNAME "sntrup653"

#define PQCLEAN_SNTRUP653_AVX2_CRYPTO_SECRETKEYBYTES 1518
#define PQCLEAN_SNTRUP653_AVX2_CRYPTO_PUBLICKEYBYTES 994
#define PQCLEAN_SNTRUP653_AVX2_CRYPTO_CIPHERTEXTBYTES 897
#define PQCLEAN_SNTRUP653_AVX2_CRYPTO_BYTES 32

int PQCLEAN_SNTRUP653_AVX2_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int PQCLEAN_SNTRUP653_AVX2_crypto_kem_enc(uint8_t *c, uint8_t *k, const uint8_t *pk);
int PQCLEAN_SNTRUP653_AVX2_crypto_kem_dec(uint8_t *k, const uint8_t *c, const uint8_t *sk);
#endif
