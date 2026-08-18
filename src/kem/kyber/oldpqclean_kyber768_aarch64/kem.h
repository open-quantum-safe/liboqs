
/*
 * This file is licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html) or
 * public domain at https://github.com/pq-crystals/kyber/tree/master/ref
 */

#ifndef KEM_H
#define KEM_H

#include "params.h"
#include <stdint.h>

#define CRYPTO_SECRETKEYBYTES  KYBER_SECRETKEYBYTES
#define CRYPTO_PUBLICKEYBYTES  KYBER_PUBLICKEYBYTES
#define CRYPTO_CIPHERTEXTBYTES KYBER_CIPHERTEXTBYTES
#define CRYPTO_BYTES           KYBER_SSBYTES

#define CRYPTO_ALGNAME "Kyber768"

int PQCLEAN_KYBER768_AARCH64_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);

int PQCLEAN_KYBER768_AARCH64_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);

int PQCLEAN_KYBER768_AARCH64_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
