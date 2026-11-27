// SPDX-License-Identifier: Apache-2.0

#ifndef api_h
#define api_h

#include <sqisign_namespace.h>

#define CRYPTO_SECRETKEYBYTES 270
#define CRYPTO_PUBLICKEYBYTES 83
#define CRYPTO_BYTES 200

#define CRYPTO_ALGNAME "SQIsign_p324_3"

#if defined(ENABLE_SIGN)
SQISIGN_API
int crypto_sign_keypair(unsigned char *pk, unsigned char *sk);

SQISIGN_API
int crypto_sign_signature(unsigned char *sig,
                          unsigned long long *siglen,
                          const unsigned char *m,
                          unsigned long long mlen,
                          const unsigned char *sk);

SQISIGN_API
int crypto_sign(unsigned char *sm,
                unsigned long long *smlen,
                const unsigned char *m,
                unsigned long long mlen,
                const unsigned char *sk);
#endif

SQISIGN_API
int crypto_sign_verify(const unsigned char *sig,
                       unsigned long long siglen,
                       const unsigned char *m,
                       unsigned long long mlen,
                       const unsigned char *pk);

SQISIGN_API
int crypto_sign_open(unsigned char *m,
                     unsigned long long *mlen,
                     const unsigned char *sm,
                     unsigned long long smlen,
                     const unsigned char *pk);

#endif /* api_h */
