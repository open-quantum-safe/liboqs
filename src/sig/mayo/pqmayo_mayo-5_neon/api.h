// SPDX-License-Identifier: Apache-2.0

#ifndef api_h
#define api_h

#include <mayo.h>

#define CRYPTO_SECRETKEYBYTES 40
#define CRYPTO_PUBLICKEYBYTES 5554
#define CRYPTO_BYTES 964

#define CRYPTO_ALGNAME "MAYO-5"

#define crypto_sign_keypair MAYO_NAMESPACE(crypto_sign_keypair)
int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk);

#define crypto_sign MAYO_NAMESPACE(crypto_sign)
int
crypto_sign(unsigned char *sm, size_t *smlen,
            const unsigned char *m, size_t mlen,
            const unsigned char *sk);

#define crypto_sign_signature MAYO_NAMESPACE(crypto_sign_signature)
int
crypto_sign_signature(unsigned char *sig,
              size_t *siglen, const unsigned char *m,
              size_t mlen, const unsigned char *sk);

#define crypto_sign_open MAYO_NAMESPACE(crypto_sign_open)
int
crypto_sign_open(unsigned char *m, size_t *mlen,
                 const unsigned char *sm, size_t smlen,
                 const unsigned char *pk);

#define crypto_sign_verify MAYO_NAMESPACE(crypto_sign_verify)
int
crypto_sign_verify(const unsigned char *sig, size_t siglen,
                   const unsigned char *m, size_t mlen,
                   const unsigned char *pk);

#endif /* api_h */

