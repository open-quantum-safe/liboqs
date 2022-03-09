#include "fips202.h"
#include "params.h"
#include "symmetric.h"
#include <stdint.h>

void dilithium_shake128_stream_init(shake128incctx *state, const uint8_t seed[SEEDBYTES], uint16_t nonce) {
    uint8_t t[2];
    t[0] = (uint8_t) nonce;
    t[1] = (uint8_t) (nonce >> 8);

    shake128_inc_init(state);
    shake128_inc_absorb(state, seed, SEEDBYTES);
    shake128_inc_absorb(state, t, 2);
    shake128_inc_finalize(state);
}

void dilithium_shake256_stream_init(shake256incctx *state, const uint8_t seed[CRHBYTES], uint16_t nonce) {
    uint8_t t[2];
    t[0] = (uint8_t) nonce;
    t[1] = (uint8_t) (nonce >> 8);

    shake256_inc_init(state);
    shake256_inc_absorb(state, seed, CRHBYTES);
    shake256_inc_absorb(state, t, 2);
    shake256_inc_finalize(state);
}

void dilithium_shake128x2_stream_init(keccakx2_state *state,
                                      const uint8_t seed[SEEDBYTES],
                                      uint16_t nonce1, uint16_t nonce2) {
    unsigned int i;
    uint8_t extseed1[SEEDBYTES + 2 + 14];
    uint8_t extseed2[SEEDBYTES + 2 + 14];

    for (i = 0; i < SEEDBYTES; i++) {
        extseed1[i] = seed[i];
        extseed2[i] = seed[i];
    }
    extseed1[SEEDBYTES] = (uint8_t) nonce1;
    extseed1[SEEDBYTES + 1] = (uint8_t) (nonce1 >> 8);

    extseed2[SEEDBYTES  ] = (uint8_t) nonce2;
    extseed2[SEEDBYTES + 1] = (uint8_t) (nonce2 >> 8);

    shake128x2_absorb(state, extseed1, extseed2, SEEDBYTES + 2);
}

void dilithium_shake256x2_stream_init(keccakx2_state *state,
                                      const uint8_t seed[CRHBYTES],
                                      uint16_t nonce1, uint16_t nonce2) {
    unsigned int i;
    uint8_t extseed1[CRHBYTES + 2 + 14];
    uint8_t extseed2[CRHBYTES + 2 + 14];

    for (i = 0; i < CRHBYTES; i++) {
        extseed1[i] = seed[i];
        extseed2[i] = seed[i];
    }
    extseed1[CRHBYTES] = (uint8_t) nonce1;
    extseed1[CRHBYTES + 1] = (uint8_t) (nonce1 >> 8);

    extseed2[CRHBYTES  ] = (uint8_t) nonce2;
    extseed2[CRHBYTES + 1] = (uint8_t) (nonce2 >> 8);

    shake256x2_absorb(state, extseed1, extseed2, CRHBYTES + 2);
}
