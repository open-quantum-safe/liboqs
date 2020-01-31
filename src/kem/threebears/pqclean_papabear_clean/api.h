#ifndef PQCLEAN_PAPABEAR_CLEAN_API_H
#define PQCLEAN_PAPABEAR_CLEAN_API_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define PQCLEAN_PAPABEAR_CLEAN_CRYPTO_SECRETKEYBYTES 40
#define PQCLEAN_PAPABEAR_CLEAN_CRYPTO_PUBLICKEYBYTES 1584
#define PQCLEAN_PAPABEAR_CLEAN_CRYPTO_BYTES 32
#define PQCLEAN_PAPABEAR_CLEAN_CRYPTO_CIPHERTEXTBYTES 1697
#define PQCLEAN_PAPABEAR_CLEAN_CRYPTO_ALGNAME "PapaBear"

int PQCLEAN_PAPABEAR_CLEAN_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int PQCLEAN_PAPABEAR_CLEAN_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int PQCLEAN_PAPABEAR_CLEAN_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
