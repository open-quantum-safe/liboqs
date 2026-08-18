// SPDX-License-Identifier: Apache-2.0

#include <api.h>
#include <mayo.h>

#ifdef ENABLE_PARAMS_DYNAMIC
#define MAYO_PARAMS &MAYO_3
#else
#define MAYO_PARAMS 0
#endif

int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk) {
    return mayo_keypair(MAYO_PARAMS, pk, sk);
}

int
crypto_sign(unsigned char *sm, size_t *smlen,
            const unsigned char *m, size_t mlen,
            const unsigned char *sk) {
    return mayo_sign(MAYO_PARAMS, sm, smlen, m, mlen, sk);
}

int
crypto_sign_signature(unsigned char *sig,
              size_t *siglen, const unsigned char *m,
              size_t mlen, const unsigned char *sk) {
    return mayo_sign_signature(MAYO_PARAMS, sig, siglen, m, mlen, sk);
}

int
crypto_sign_open(unsigned char *m, size_t *mlen,
                 const unsigned char *sm, size_t smlen,
                 const unsigned char *pk) {
    return mayo_open(MAYO_PARAMS, m, mlen, sm, smlen, pk);
}

int
crypto_sign_verify(const unsigned char *sig, size_t siglen,
                   const unsigned char *m, size_t mlen,
                   const unsigned char *pk) {
    if (siglen != CRYPTO_BYTES)
        return -1;
    return mayo_verify(MAYO_PARAMS, m, mlen, sig, pk);
}

