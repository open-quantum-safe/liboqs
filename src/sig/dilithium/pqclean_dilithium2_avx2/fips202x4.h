#ifndef PQCLEAN_DILITHIUM2_AVX2_FIPS202X4_H
#define PQCLEAN_DILITHIUM2_AVX2_FIPS202X4_H

#include <immintrin.h>
#include <stddef.h>
#include <stdint.h>

#include "params.h"

void PQCLEAN_DILITHIUM2_AVX2_shake128_absorb4x(
    __m256i *s,
    const uint8_t *m0,
    const uint8_t *m1,
    const uint8_t *m2,
    const uint8_t *m3,
    size_t mlen);

void PQCLEAN_DILITHIUM2_AVX2_shake128_squeezeblocks4x(
    uint8_t *h0,
    uint8_t *h1,
    uint8_t *h2,
    uint8_t *h3,
    size_t nblocks,
    __m256i *s);

void PQCLEAN_DILITHIUM2_AVX2_shake256_absorb4x(
    __m256i *s,
    const uint8_t *m0,
    const uint8_t *m1,
    const uint8_t *m2,
    const uint8_t *m3,
    size_t mlen);

void PQCLEAN_DILITHIUM2_AVX2_shake256_squeezeblocks4x(
    uint8_t *h0,
    uint8_t *h1,
    uint8_t *h2,
    uint8_t *h3,
    size_t nblocks,
    __m256i *s);

void PQCLEAN_DILITHIUM2_AVX2_shake128_4x(
    uint8_t *h0,
    uint8_t *h1,
    uint8_t *h2,
    uint8_t *h3,
    size_t hlen,
    const uint8_t *m0,
    const uint8_t *m1,
    const uint8_t *m2,
    const uint8_t *m3,
    size_t mlen);

void PQCLEAN_DILITHIUM2_AVX2_shake256_4x(
    uint8_t *h0,
    uint8_t *h1,
    uint8_t *h2,
    uint8_t *h3,
    size_t hlen,
    const uint8_t *m0,
    const uint8_t *m1,
    const uint8_t *m2,
    const uint8_t *m3,
    size_t mlen);

#endif
