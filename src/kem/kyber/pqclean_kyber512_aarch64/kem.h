#ifndef KEM_H
#define KEM_H

#include "params.h"
#include <stdint.h>

#define CRYPTO_SECRETKEYBYTES  KYBER_SECRETKEYBYTES
#define CRYPTO_PUBLICKEYBYTES  KYBER_PUBLICKEYBYTES
#define CRYPTO_CIPHERTEXTBYTES KYBER_CIPHERTEXTBYTES
#define CRYPTO_BYTES           KYBER_SSBYTES

#define CRYPTO_ALGNAME "Kyber512"

int PQCLEAN_KYBER512_AARCH64_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);

#ifdef OQS_HAZARDOUS_ENABLE_DERIVE_KEYPAIR
int PQCLEAN_KYBER768_AARCH64_crypto_kem_derive_keypair(const uint8_t *randomness, uint8_t *pk, uint8_t *sk);
#endif

int PQCLEAN_KYBER512_AARCH64_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);

int PQCLEAN_KYBER512_AARCH64_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
