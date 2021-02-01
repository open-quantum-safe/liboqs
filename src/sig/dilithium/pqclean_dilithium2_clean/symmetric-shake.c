#include "fips202.h"
#include "params.h"
#include "symmetric.h"
#include <stdint.h>

void PQCLEAN_DILITHIUM2_CLEAN_dilithium_shake128_stream_init(shake128incctx *state, const uint8_t seed[SEEDBYTES], uint16_t nonce) {
    uint8_t t[2];
    t[0] = (uint8_t) nonce;
    t[1] = (uint8_t) (nonce >> 8);

    shake128_inc_init(state);
    shake128_inc_absorb(state, seed, SEEDBYTES);
    shake128_inc_absorb(state, t, 2);
    shake128_inc_finalize(state);
}

void PQCLEAN_DILITHIUM2_CLEAN_dilithium_shake256_stream_init(shake256incctx *state, const uint8_t seed[CRHBYTES], uint16_t nonce) {
    uint8_t t[2];
    t[0] = (uint8_t) nonce;
    t[1] = (uint8_t) (nonce >> 8);

    shake256_inc_init(state);
    shake256_inc_absorb(state, seed, CRHBYTES);
    shake256_inc_absorb(state, t, 2);
    shake256_inc_finalize(state);
}
