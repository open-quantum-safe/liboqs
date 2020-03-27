#include "stream.h"

#include <string.h>

void PQCLEAN_DILITHIUM2_AVX2_shake128_stream_init(
    shake128ctx *state, const uint8_t seed[SEEDBYTES], uint16_t nonce) {

    uint8_t buf[SEEDBYTES + 2];
    memcpy(buf, seed, SEEDBYTES);
    buf[SEEDBYTES] = (uint8_t)nonce;
    buf[SEEDBYTES + 1] = (uint8_t)(nonce >> 8);

    shake128_absorb(state, buf, SEEDBYTES + 2);
}


void PQCLEAN_DILITHIUM2_AVX2_shake256_stream_init(
    shake256ctx *state, const uint8_t seed[CRHBYTES], uint16_t nonce) {

    uint8_t buf[CRHBYTES + 2];
    memcpy(buf, seed, CRHBYTES);
    buf[CRHBYTES] = (uint8_t)nonce;
    buf[CRHBYTES + 1] = (uint8_t)(nonce >> 8);

    shake256_absorb(state, buf, CRHBYTES + 2);
}
