#ifndef PQCLEAN_BABYBEAR_CLEAN_API_H
#define PQCLEAN_BABYBEAR_CLEAN_API_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define PQCLEAN_BABYBEAR_CLEAN_CRYPTO_SECRETKEYBYTES 40
#define PQCLEAN_BABYBEAR_CLEAN_CRYPTO_PUBLICKEYBYTES 804
#define PQCLEAN_BABYBEAR_CLEAN_CRYPTO_BYTES 32
#define PQCLEAN_BABYBEAR_CLEAN_CRYPTO_CIPHERTEXTBYTES 917
#define PQCLEAN_BABYBEAR_CLEAN_CRYPTO_ALGNAME "BabyBear"

int PQCLEAN_BABYBEAR_CLEAN_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int PQCLEAN_BABYBEAR_CLEAN_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int PQCLEAN_BABYBEAR_CLEAN_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
