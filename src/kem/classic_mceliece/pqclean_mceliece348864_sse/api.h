#ifndef PQCLEAN_MCELIECE348864_SSE_API_H
#define PQCLEAN_MCELIECE348864_SSE_API_H

#include <stdint.h>

#define PQCLEAN_MCELIECE348864_SSE_CRYPTO_ALGNAME "Classic McEliece 348864"
#define PQCLEAN_MCELIECE348864_SSE_CRYPTO_PUBLICKEYBYTES 261120
#define PQCLEAN_MCELIECE348864_SSE_CRYPTO_SECRETKEYBYTES 6452
#define PQCLEAN_MCELIECE348864_SSE_CRYPTO_CIPHERTEXTBYTES 128
#define PQCLEAN_MCELIECE348864_SSE_CRYPTO_BYTES 32


int PQCLEAN_MCELIECE348864_SSE_crypto_kem_enc(
    uint8_t *c,
    uint8_t *key,
    const uint8_t *pk
);

int PQCLEAN_MCELIECE348864_SSE_crypto_kem_dec(
    uint8_t *key,
    const uint8_t *c,
    const uint8_t *sk
);

int PQCLEAN_MCELIECE348864_SSE_crypto_kem_keypair
(
    uint8_t *pk,
    uint8_t *sk
);

#endif

