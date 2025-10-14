#ifndef NTRUPLUS_H
#define NTRUPLUS_H

#include <stdint.h>
#include <stddef.h>

#define NTRUPLUS_PUBLICKEYBYTES 1138
#define NTRUPLUS_SECRETKEYBYTES 1450
#define NTRUPLUS_CIPHERTEXTBYTES 1138
#define NTRUPLUS_BYTES 32

int crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif