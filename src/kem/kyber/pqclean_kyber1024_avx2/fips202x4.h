#ifndef FIPS202X4_H
#define FIPS202X4_H

#include <immintrin.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    __m256i s[25];
} keccak4x_state;

void PQCLEAN_KYBER1024_AVX2_kyber_shake128x4_absorb(keccak4x_state *state,
        const uint8_t *seed,
        uint16_t nonce0,
        uint16_t nonce1,
        uint16_t nonce2,
        uint16_t nonce3);

void PQCLEAN_KYBER1024_AVX2_shake128x4_squeezeblocks(uint8_t *out0,
        uint8_t *out1,
        uint8_t *out2,
        uint8_t *out3,
        size_t nblocks,
        keccak4x_state *state);

void PQCLEAN_KYBER1024_AVX2_shake256x4_prf(uint8_t *out0,
        uint8_t *out1,
        uint8_t *out2,
        uint8_t *out3,
        size_t outlen,
        const uint8_t *key,
        uint8_t nonce0,
        uint8_t nonce1,
        uint8_t nonce2,
        uint8_t nonce3);

#endif
