#include "symmetric.h"
#include "fips202.h"

void PQCLEAN_DILITHIUM3_CLEAN_shake128_stream_init(shake128ctx *state,
        const unsigned char seed[SEEDBYTES],
        uint16_t nonce) {
    unsigned int i;
    unsigned char buf[SEEDBYTES + 2];

    for (i = 0; i < SEEDBYTES; ++i) {
        buf[i] = seed[i];
    }
    buf[SEEDBYTES] = (uint8_t) nonce;
    buf[SEEDBYTES + 1] = (uint8_t) (nonce >> 8);

    shake128_absorb(state, buf, sizeof(buf));
}

void PQCLEAN_DILITHIUM3_CLEAN_shake256_stream_init(shake256ctx *state,
        const unsigned char seed[CRHBYTES],
        uint16_t nonce) {
    unsigned int i;
    unsigned char buf[CRHBYTES + 2];

    for (i = 0; i < CRHBYTES; ++i) {
        buf[i] = seed[i];
    }
    buf[CRHBYTES] = (uint8_t) nonce;
    buf[CRHBYTES + 1] = (uint8_t) (nonce >> 8);

    shake256_absorb(state, buf, sizeof(buf));
}
