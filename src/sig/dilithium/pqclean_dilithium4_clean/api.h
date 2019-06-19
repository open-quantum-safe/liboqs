#ifndef PQCLEAN_DILITHIUM4_CLEAN_API_H
#define PQCLEAN_DILITHIUM4_CLEAN_API_H

#include <stddef.h>
#include <stdint.h>

#define PQCLEAN_DILITHIUM4_CLEAN_CRYPTO_PUBLICKEYBYTES 1760U
#define PQCLEAN_DILITHIUM4_CLEAN_CRYPTO_SECRETKEYBYTES 3856U
#define PQCLEAN_DILITHIUM4_CLEAN_CRYPTO_BYTES 3366U
#define PQCLEAN_DILITHIUM4_CLEAN_CRYPTO_ALGNAME "Dilithium4"


int PQCLEAN_DILITHIUM4_CLEAN_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);

int PQCLEAN_DILITHIUM4_CLEAN_crypto_sign_signature(
    uint8_t *sig, size_t *siglen,
    const uint8_t *m, size_t mlen, const uint8_t *sk);

int PQCLEAN_DILITHIUM4_CLEAN_crypto_sign_verify(
    const uint8_t *sig, size_t siglen,
    const uint8_t *m, size_t mlen, const uint8_t *pk);

int PQCLEAN_DILITHIUM4_CLEAN_crypto_sign(uint8_t *sm, size_t *smlen,
        const uint8_t *msg, size_t len,
        const uint8_t *sk);

int PQCLEAN_DILITHIUM4_CLEAN_crypto_sign_open(uint8_t *m, size_t *mlen,
        const uint8_t *sm, size_t smlen,
        const uint8_t *pk);


#endif
