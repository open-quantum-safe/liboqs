///  @file  sign.c
///  @brief the implementations for functions in api.h
///
///

#include "api.h"
#include "rainbow.h"
#include "rainbow_config.h"
#include "rainbow_keypair.h"
#include "randombytes.h"
#include "utils_hash.h"
#include <stdlib.h>
#include <string.h>

int PQCLEAN_RAINBOWVCCYCLICCOMPRESSED_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk) {
    unsigned char sk_seed[LEN_SKSEED] = {0};
    randombytes(sk_seed, LEN_SKSEED);

    unsigned char pk_seed[LEN_PKSEED] = {0};
    randombytes(pk_seed, LEN_PKSEED);
    PQCLEAN_RAINBOWVCCYCLICCOMPRESSED_CLEAN_generate_compact_keypair_cyclic((cpk_t *)pk, (csk_t *)sk, pk_seed, sk_seed);
    return 0;
}

int PQCLEAN_RAINBOWVCCYCLICCOMPRESSED_CLEAN_crypto_sign(unsigned char *sm, size_t *smlen, const unsigned char *m, size_t mlen, const unsigned char *sk) {
    unsigned char digest[_HASH_LEN];

    PQCLEAN_RAINBOWVCCYCLICCOMPRESSED_CLEAN_hash_msg(digest, _HASH_LEN, m, mlen);

    memcpy(sm, m, mlen);
    smlen[0] = mlen + _SIGNATURE_BYTE;

    return PQCLEAN_RAINBOWVCCYCLICCOMPRESSED_CLEAN_rainbow_sign_cyclic(sm + mlen, (const csk_t *)sk, digest);
}

int PQCLEAN_RAINBOWVCCYCLICCOMPRESSED_CLEAN_crypto_sign_open(unsigned char *m, size_t *mlen, const unsigned char *sm, size_t smlen, const unsigned char *pk) {
    int rc;
    if (_SIGNATURE_BYTE > smlen) {
        rc = -1;
    } else {
        *mlen = smlen - _SIGNATURE_BYTE;

        unsigned char digest[_HASH_LEN];
        PQCLEAN_RAINBOWVCCYCLICCOMPRESSED_CLEAN_hash_msg(digest, _HASH_LEN, sm, *mlen);

        rc = PQCLEAN_RAINBOWVCCYCLICCOMPRESSED_CLEAN_rainbow_verify_cyclic(digest, sm + mlen[0], (const cpk_t *)pk);
    }
    if (!rc) {
        memcpy(m, sm, smlen - _SIGNATURE_BYTE);
    } else { // bad signature
        *mlen = (size_t) -1;
        memset(m, 0, smlen);
    }
    return rc;
}

int PQCLEAN_RAINBOWVCCYCLICCOMPRESSED_CLEAN_crypto_sign_signature(
    uint8_t *sig, size_t *siglen,
    const uint8_t *m, size_t mlen, const uint8_t *sk) {
    unsigned char digest[_HASH_LEN];

    PQCLEAN_RAINBOWVCCYCLICCOMPRESSED_CLEAN_hash_msg(digest, _HASH_LEN, m, mlen);
    *siglen = _SIGNATURE_BYTE;
    return PQCLEAN_RAINBOWVCCYCLICCOMPRESSED_CLEAN_rainbow_sign_cyclic(sig, (const csk_t *)sk, digest);
}

int PQCLEAN_RAINBOWVCCYCLICCOMPRESSED_CLEAN_crypto_sign_verify(
    const uint8_t *sig, size_t siglen,
    const uint8_t *m, size_t mlen, const uint8_t *pk) {
    if (siglen != _SIGNATURE_BYTE) {
        return -1;
    }
    unsigned char digest[_HASH_LEN];
    PQCLEAN_RAINBOWVCCYCLICCOMPRESSED_CLEAN_hash_msg(digest, _HASH_LEN, m, mlen);
    return PQCLEAN_RAINBOWVCCYCLICCOMPRESSED_CLEAN_rainbow_verify_cyclic(digest, sig, (const cpk_t *)pk);
}
