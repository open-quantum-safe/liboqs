/*
 *  SPDX-License-Identifier: MIT
 */

#ifndef FAEST_LIBOQS_API_192S_H
#define FAEST_LIBOQS_API_192S_H

#include <stddef.h>
#include <stdint.h>

#define CRYPTO_PUBLICKEYBYTES 48
#define CRYPTO_SECRETKEYBYTES 40
#define CRYPTO_BYTES 9410
#define CRYPTO_ALGNAME "faest_192s"

#define crypto_sign_keypair PQCLEAN_FAEST_192S_REF_crypto_sign_keypair
#define crypto_sign_signature PQCLEAN_FAEST_192S_REF_crypto_sign_signature
#define crypto_sign_verify PQCLEAN_FAEST_192S_REF_crypto_sign_verify

int crypto_sign_keypair(uint8_t* pk, uint8_t* sk);
int crypto_sign_signature(uint8_t* sig, size_t* siglen, const uint8_t* m, size_t mlen,
                          const uint8_t* sk);
int crypto_sign_verify(const uint8_t* sig, size_t siglen, const uint8_t* m, size_t mlen,
                       const uint8_t* pk);

#endif
