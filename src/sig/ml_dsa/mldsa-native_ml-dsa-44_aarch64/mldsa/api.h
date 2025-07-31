/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#ifndef MLD_API_H
#define MLD_API_H

#include <stddef.h>
#include <stdint.h>

#define MLD_44_PUBLICKEYBYTES 1312
#define MLD_44_SECRETKEYBYTES 2560
#define MLD_44_BYTES 2420

#define MLD_44_ref_PUBLICKEYBYTES MLD_44_PUBLICKEYBYTES
#define MLD_44_ref_SECRETKEYBYTES MLD_44_SECRETKEYBYTES
#define MLD_44_ref_BYTES MLD_44_BYTES

int MLD_44_ref_keypair(uint8_t *pk, uint8_t *sk);

int MLD_44_ref_signature(uint8_t *sig, size_t *siglen, const uint8_t *m,
                         size_t mlen, const uint8_t *ctx, size_t ctxlen,
                         const uint8_t *sk);

int MLD_44_ref(uint8_t *sm, size_t *smlen, const uint8_t *m, size_t mlen,
               const uint8_t *ctx, size_t ctxlen, const uint8_t *sk);

int MLD_44_ref_verify(const uint8_t *sig, size_t siglen, const uint8_t *m,
                      size_t mlen, const uint8_t *ctx, size_t ctxlen,
                      const uint8_t *pk);

int MLD_44_ref_open(uint8_t *m, size_t *mlen, const uint8_t *sm, size_t smlen,
                    const uint8_t *ctx, size_t ctxlen, const uint8_t *pk);

#define MLD_65_PUBLICKEYBYTES 1952
#define MLD_65_SECRETKEYBYTES 4032
#define MLD_65_BYTES 3309

#define MLD_65_ref_PUBLICKEYBYTES MLD_65_PUBLICKEYBYTES
#define MLD_65_ref_SECRETKEYBYTES MLD_65_SECRETKEYBYTES
#define MLD_65_ref_BYTES MLD_65_BYTES

int MLD_65_ref_keypair(uint8_t *pk, uint8_t *sk);

int MLD_65_ref_signature(uint8_t *sig, size_t *siglen, const uint8_t *m,
                         size_t mlen, const uint8_t *ctx, size_t ctxlen,
                         const uint8_t *sk);

int MLD_65_ref(uint8_t *sm, size_t *smlen, const uint8_t *m, size_t mlen,
               const uint8_t *ctx, size_t ctxlen, const uint8_t *sk);

int MLD_65_ref_verify(const uint8_t *sig, size_t siglen, const uint8_t *m,
                      size_t mlen, const uint8_t *ctx, size_t ctxlen,
                      const uint8_t *pk);

int MLD_65_ref_open(uint8_t *m, size_t *mlen, const uint8_t *sm, size_t smlen,
                    const uint8_t *ctx, size_t ctxlen, const uint8_t *pk);

#define MLD_87_PUBLICKEYBYTES 2592
#define MLD_87_SECRETKEYBYTES 4896
#define MLD_87_BYTES 4627

#define MLD_87_ref_PUBLICKEYBYTES MLD_87_PUBLICKEYBYTES
#define MLD_87_ref_SECRETKEYBYTES MLD_87_SECRETKEYBYTES
#define MLD_87_ref_BYTES MLD_87_BYTES

int MLD_87_ref_keypair(uint8_t *pk, uint8_t *sk);

int MLD_87_ref_signature(uint8_t *sig, size_t *siglen, const uint8_t *m,
                         size_t mlen, const uint8_t *ctx, size_t ctxlen,
                         const uint8_t *sk);

int MLD_87_ref(uint8_t *sm, size_t *smlen, const uint8_t *m, size_t mlen,
               const uint8_t *ctx, size_t ctxlen, const uint8_t *sk);

int MLD_87_ref_verify(const uint8_t *sig, size_t siglen, const uint8_t *m,
                      size_t mlen, const uint8_t *ctx, size_t ctxlen,
                      const uint8_t *pk);

int MLD_87_ref_open(uint8_t *m, size_t *mlen, const uint8_t *sm, size_t smlen,
                    const uint8_t *ctx, size_t ctxlen, const uint8_t *pk);

#if MLDSA_MODE == 2
#define CRYPTO_PUBLICKEYBYTES MLD_44_PUBLICKEYBYTES
#define CRYPTO_SECRETKEYBYTES MLD_44_SECRETKEYBYTES
#define CRYPTO_BYTES MLD_44_BYTES
#define crypto_sign_keypair MLD_44_ref_keypair
#define crypto_sign_signature MLD_44_ref_signature
#define crypto_sign MLD_44_ref
#define crypto_sign_verify MLD_44_ref_verify
#define crypto_sign_open MLD_44_ref_open
#elif MLDSA_MODE == 3
#define CRYPTO_PUBLICKEYBYTES MLD_65_PUBLICKEYBYTES
#define CRYPTO_SECRETKEYBYTES MLD_65_SECRETKEYBYTES
#define CRYPTO_BYTES MLD_65_BYTES
#define crypto_sign_keypair MLD_65_ref_keypair
#define crypto_sign_signature MLD_65_ref_signature
#define crypto_sign MLD_65_ref
#define crypto_sign_verify MLD_65_ref_verify
#define crypto_sign_open MLD_65_ref_open
#elif MLDSA_MODE == 5
#define CRYPTO_PUBLICKEYBYTES MLD_87_PUBLICKEYBYTES
#define CRYPTO_SECRETKEYBYTES MLD_87_SECRETKEYBYTES
#define CRYPTO_BYTES MLD_87_BYTES
#define crypto_sign_keypair MLD_87_ref_keypair
#define crypto_sign_signature MLD_87_ref_signature
#define crypto_sign MLD_87_ref
#define crypto_sign_verify MLD_87_ref_verify
#define crypto_sign_open MLD_87_ref_open
#endif /* MLDSA_MODE == 5 */


#endif /* !MLD_API_H */
