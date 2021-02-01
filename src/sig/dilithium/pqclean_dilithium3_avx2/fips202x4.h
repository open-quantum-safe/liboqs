#ifndef PQCLEAN_DILITHIUM3_AVX2_FIPS202X4_H
#define PQCLEAN_DILITHIUM3_AVX2_FIPS202X4_H

#include <immintrin.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    __m256i s[25];
} keccakx4_state;

void PQCLEAN_DILITHIUM3_AVX2_f1600x4(__m256i *s, const uint64_t *rc);

void PQCLEAN_DILITHIUM3_AVX2_shake128x4_absorb_once(keccakx4_state *state,
        const uint8_t *in0,
        const uint8_t *in1,
        const uint8_t *in2,
        const uint8_t *in3,
        size_t inlen);

void PQCLEAN_DILITHIUM3_AVX2_shake128x4_squeezeblocks(uint8_t *out0,
        uint8_t *out1,
        uint8_t *out2,
        uint8_t *out3,
        size_t nblocks,
        keccakx4_state *state);

void PQCLEAN_DILITHIUM3_AVX2_shake256x4_absorb_once(keccakx4_state *state,
        const uint8_t *in0,
        const uint8_t *in1,
        const uint8_t *in2,
        const uint8_t *in3,
        size_t inlen);

void PQCLEAN_DILITHIUM3_AVX2_shake256x4_squeezeblocks(uint8_t *out0,
        uint8_t *out1,
        uint8_t *out2,
        uint8_t *out3,
        size_t nblocks,
        keccakx4_state *state);

void PQCLEAN_DILITHIUM3_AVX2_shake128x4(uint8_t *out0,
                                        uint8_t *out1,
                                        uint8_t *out2,
                                        uint8_t *out3,
                                        size_t outlen,
                                        const uint8_t *in0,
                                        const uint8_t *in1,
                                        const uint8_t *in2,
                                        const uint8_t *in3,
                                        size_t inlen);

void PQCLEAN_DILITHIUM3_AVX2_shake256x4(uint8_t *out0,
                                        uint8_t *out1,
                                        uint8_t *out2,
                                        uint8_t *out3,
                                        size_t outlen,
                                        const uint8_t *in0,
                                        const uint8_t *in1,
                                        const uint8_t *in2,
                                        const uint8_t *in3,
                                        size_t inlen);

#endif
