#ifndef PQCLEAN_RAINBOWIACYCLIC_CLEAN_API_H
#define PQCLEAN_RAINBOWIACYCLIC_CLEAN_API_H

#include <stddef.h>
#include <stdint.h>

#define PQCLEAN_RAINBOWIACYCLIC_CLEAN_CRYPTO_SECRETKEYBYTES 92960
#define PQCLEAN_RAINBOWIACYCLIC_CLEAN_CRYPTO_PUBLICKEYBYTES 58144
#define PQCLEAN_RAINBOWIACYCLIC_CLEAN_CRYPTO_BYTES 64
#define PQCLEAN_RAINBOWIACYCLIC_CLEAN_CRYPTO_ALGNAME "RAINBOW(16,32,32,32) - cyclic"

int PQCLEAN_RAINBOWIACYCLIC_CLEAN_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);


int PQCLEAN_RAINBOWIACYCLIC_CLEAN_crypto_sign_signature(
    uint8_t *sig, size_t *siglen,
    const uint8_t *m, size_t mlen, const uint8_t *sk);

int PQCLEAN_RAINBOWIACYCLIC_CLEAN_crypto_sign_verify(
    const uint8_t *sig, size_t siglen,
    const uint8_t *m, size_t mlen, const uint8_t *pk);

int PQCLEAN_RAINBOWIACYCLIC_CLEAN_crypto_sign(uint8_t *sm, size_t *smlen,
        const uint8_t *m, size_t mlen,
        const uint8_t *sk);

int PQCLEAN_RAINBOWIACYCLIC_CLEAN_crypto_sign_open(uint8_t *m, size_t *mlen,
        const uint8_t *sm, size_t smlen,
        const uint8_t *pk);


#endif
