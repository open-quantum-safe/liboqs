#include <immintrin.h>
#include <stdint.h>

#include "fips202.h"
#include "fips202x4.h"
#include "params.h"

#define NROUNDS 24
#define ROL(a, offset) (((a) << (offset)) ^ ((a) >> (64 - (offset))))

static uint64_t load64(const uint8_t *x) {
    uint64_t r = 0;

    for (size_t i = 0; i < 8; ++i) {
        r |= (uint64_t)x[i] << 8 * i;
    }

    return r;
}

static void store64(uint8_t *x, uint64_t u) {
    for (size_t i = 0; i < 8; ++i) {
        x[i] = (uint8_t)(u >> 8 * i);
    }
}

/* Use implementation from the Keccak Code Package */
extern void KeccakP1600times4_PermuteAll_24rounds(__m256i *s);
#define KeccakF1600_StatePermute4x KeccakP1600times4_PermuteAll_24rounds

static void keccak_absorb4x(__m256i *s,
                            uint8_t r,
                            const uint8_t *m0,
                            const uint8_t *m1,
                            const uint8_t *m2,
                            const uint8_t *m3,
                            size_t mlen,
                            uint8_t p) {
    size_t i;
    uint8_t t0[200];
    uint8_t t1[200];
    uint8_t t2[200];
    uint8_t t3[200];
    uint64_t *ss = (uint64_t *)s;

    for (i = 0; i < 25; ++i) {
        s[i] = _mm256_xor_si256(s[i], s[i]);
    }

    while (mlen >= r) {
        for (i = 0; i < r / 8; ++i) {
            ss[4 * i + 0] ^= load64(m0 + 8 * i);
            ss[4 * i + 1] ^= load64(m1 + 8 * i);
            ss[4 * i + 2] ^= load64(m2 + 8 * i);
            ss[4 * i + 3] ^= load64(m3 + 8 * i);
        }

        KeccakF1600_StatePermute4x(s);
        mlen -= r;
        m0 += r;
        m1 += r;
        m2 += r;
        m3 += r;
    }

    for (i = 0; i < r; ++i) {
        t0[i] = 0;
        t1[i] = 0;
        t2[i] = 0;
        t3[i] = 0;
    }
    for (i = 0; i < mlen; ++i) {
        t0[i] = m0[i];
        t1[i] = m1[i];
        t2[i] = m2[i];
        t3[i] = m3[i];
    }

    t0[i] = p;
    t1[i] = p;
    t2[i] = p;
    t3[i] = p;

    t0[r - 1] |= 128;
    t1[r - 1] |= 128;
    t2[r - 1] |= 128;
    t3[r - 1] |= 128;

    for (i = 0; i < r / 8; ++i) {
        ss[4 * i + 0] ^= load64(t0 + 8 * i);
        ss[4 * i + 1] ^= load64(t1 + 8 * i);
        ss[4 * i + 2] ^= load64(t2 + 8 * i);
        ss[4 * i + 3] ^= load64(t3 + 8 * i);
    }
}


static void keccak_squeezeblocks4x(uint8_t *h0,
                                   uint8_t *h1,
                                   uint8_t *h2,
                                   uint8_t *h3,
                                   size_t nblocks,
                                   uint8_t r,
                                   __m256i *s) {
    uint64_t *ss = (uint64_t *)s;

    while (nblocks > 0) {
        KeccakF1600_StatePermute4x(s);
        for (size_t i = 0; i < r / 8; ++i) {
            store64(h0 + 8 * i, ss[4 * i + 0]);
            store64(h1 + 8 * i, ss[4 * i + 1]);
            store64(h2 + 8 * i, ss[4 * i + 2]);
            store64(h3 + 8 * i, ss[4 * i + 3]);
        }

        h0 += r;
        h1 += r;
        h2 += r;
        h3 += r;
        --nblocks;
    }

}

void PQCLEAN_DILITHIUM2_AVX2_shake128_absorb4x(
    __m256i *s,
    const uint8_t *m0,
    const uint8_t *m1,
    const uint8_t *m2,
    const uint8_t *m3,
    size_t mlen) {
    keccak_absorb4x(s, SHAKE128_RATE, m0, m1, m2, m3, mlen, 0x1F);
}

void PQCLEAN_DILITHIUM2_AVX2_shake128_squeezeblocks4x(
    uint8_t *h0,
    uint8_t *h1,
    uint8_t *h2,
    uint8_t *h3,
    size_t nblocks,
    __m256i *s) {
    keccak_squeezeblocks4x(h0, h1, h2, h3, nblocks, SHAKE128_RATE, s);
}

void PQCLEAN_DILITHIUM2_AVX2_shake256_absorb4x(
    __m256i *s,
    const uint8_t *m0,
    const uint8_t *m1,
    const uint8_t *m2,
    const uint8_t *m3,
    size_t mlen) {
    keccak_absorb4x(s, SHAKE256_RATE, m0, m1, m2, m3, mlen, 0x1F);
}

void PQCLEAN_DILITHIUM2_AVX2_shake256_squeezeblocks4x(
    uint8_t *h0,
    uint8_t *h1,
    uint8_t *h2,
    uint8_t *h3,
    size_t nblocks,
    __m256i *s) {
    keccak_squeezeblocks4x(h0, h1, h2, h3, nblocks, SHAKE256_RATE, s);
}

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
    size_t mlen) {
    size_t nblocks = hlen / SHAKE128_RATE;
    uint8_t t[4][SHAKE128_RATE];
    __m256i s[25];

    PQCLEAN_DILITHIUM2_AVX2_shake128_absorb4x(s, m0, m1, m2, m3, mlen);
    PQCLEAN_DILITHIUM2_AVX2_shake128_squeezeblocks4x(h0, h1, h2, h3, nblocks, s);

    h0 += nblocks * SHAKE128_RATE;
    h1 += nblocks * SHAKE128_RATE;
    h2 += nblocks * SHAKE128_RATE;
    h3 += nblocks * SHAKE128_RATE;
    hlen -= nblocks * SHAKE128_RATE;

    if (hlen) {
        PQCLEAN_DILITHIUM2_AVX2_shake128_squeezeblocks4x(t[0], t[1], t[2], t[3], 1, s);
        for (size_t i = 0; i < hlen; ++i) {
            h0[i] = t[0][i];
            h1[i] = t[1][i];
            h2[i] = t[2][i];
            h3[i] = t[3][i];
        }
    }
}

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
    size_t mlen) {
    size_t nblocks = hlen / SHAKE256_RATE;
    uint8_t t[4][SHAKE256_RATE];
    __m256i s[25];

    PQCLEAN_DILITHIUM2_AVX2_shake256_absorb4x(s, m0, m1, m2, m3, mlen);
    PQCLEAN_DILITHIUM2_AVX2_shake256_squeezeblocks4x(h0, h1, h2, h3, nblocks, s);

    h0 += nblocks * SHAKE256_RATE;
    h1 += nblocks * SHAKE256_RATE;
    h2 += nblocks * SHAKE256_RATE;
    h3 += nblocks * SHAKE256_RATE;
    hlen -= nblocks * SHAKE256_RATE;

    if (hlen) {
        PQCLEAN_DILITHIUM2_AVX2_shake256_squeezeblocks4x(t[0], t[1], t[2], t[3], 1, s);
        for (size_t i = 0; i < hlen; ++i) {
            h0[i] = t[0][i];
            h1[i] = t[1][i];
            h2[i] = t[2][i];
            h3[i] = t[3][i];
        }
    }
}
