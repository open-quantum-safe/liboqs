#include "fips202.h"
#include "fips202x4.h"
#include "params.h"

#include <immintrin.h>
#include <stdint.h>


#define NROUNDS 24
#define ROL(a, offset) (((a) << (offset)) ^ ((a) >> (64-(offset))))

static uint64_t load64(const uint8_t *x) {
    unsigned long long r = 0, i;

    for (i = 0; i < 8; ++i) {
        r |= (unsigned long long)x[i] << 8 * i;
    }
    return r;
}

static void store64(uint8_t *x, uint64_t u) {
    size_t i;

    for (i = 0; i < 8; ++i) {
        x[i] = (uint8_t)u;
        u >>= 8;
    }
}

/* Use implementation from the Keccak Code Package */
extern void KeccakP1600times4_PermuteAll_24rounds(__m256i *s);
#define KeccakF1600_StatePermute4x KeccakP1600times4_PermuteAll_24rounds

static void keccak_absorb4x(__m256i *s,
                            unsigned int r,
                            const uint8_t *m0,
                            const uint8_t *m1,
                            const uint8_t *m2,
                            const uint8_t *m3,
                            size_t mlen,
                            uint8_t p) {
    size_t i;
    uint8_t t0[200] = {0};
    uint8_t t1[200] = {0};
    uint8_t t2[200] = {0};
    uint8_t t3[200] = {0};

    unsigned long long *ss = (unsigned long long *)s;

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
                                   __m256i *s,
                                   unsigned int r) {
    unsigned long long *ss = (unsigned long long *)s;

    while (nblocks > 0) {
        KeccakF1600_StatePermute4x(s);
        for (size_t i = 0; i < (r >> 3); i++) {
            store64(h0 + 8 * i, ss[4 * i + 0]);
            store64(h1 + 8 * i, ss[4 * i + 1]);
            store64(h2 + 8 * i, ss[4 * i + 2]);
            store64(h3 + 8 * i, ss[4 * i + 3]);
        }
        h0 += r;
        h1 += r;
        h2 += r;
        h3 += r;
        nblocks--;
    }
}

void PQCLEAN_KYBER1024_AVX2_kyber_shake128x4_absorb(keccak4x_state *state,
        const uint8_t *seed,
        uint16_t nonce0,
        uint16_t nonce1,
        uint16_t nonce2,
        uint16_t nonce3) {
    uint8_t extseed[4][KYBER_SYMBYTES + 2];

    for (size_t i = 0; i < KYBER_SYMBYTES; ++i) {
        extseed[0][i] = seed[i];
        extseed[1][i] = seed[i];
        extseed[2][i] = seed[i];
        extseed[3][i] = seed[i];
    }
    extseed[0][KYBER_SYMBYTES + 0] = (uint8_t)nonce0;
    extseed[0][KYBER_SYMBYTES + 1] = (uint8_t)(nonce0 >> 8);
    extseed[1][KYBER_SYMBYTES + 0] = (uint8_t)nonce1;
    extseed[1][KYBER_SYMBYTES + 1] = (uint8_t)(nonce1 >> 8);
    extseed[2][KYBER_SYMBYTES + 0] = (uint8_t)nonce2;
    extseed[2][KYBER_SYMBYTES + 1] = (uint8_t)(nonce2 >> 8);
    extseed[3][KYBER_SYMBYTES + 0] = (uint8_t)nonce3;
    extseed[3][KYBER_SYMBYTES + 1] = (uint8_t)(nonce3 >> 8);

    /* zero state */
    for (size_t i = 0; i < 25; i++) {
        state->s[i] = _mm256_xor_si256(state->s[i], state->s[i]);
    }

    /* absorb 4 message of identical length in parallel */
    keccak_absorb4x(state->s, SHAKE128_RATE, extseed[0], extseed[1], extseed[2], extseed[3], KYBER_SYMBYTES + 2, 0x1F);
}

void PQCLEAN_KYBER1024_AVX2_shake128x4_squeezeblocks(uint8_t *out0,
        uint8_t *out1,
        uint8_t *out2,
        uint8_t *out3,
        size_t nblocks,
        keccak4x_state *state) {
    keccak_squeezeblocks4x(out0, out1, out2, out3, nblocks, state->s, SHAKE128_RATE);
}

static void shake256x4(uint8_t *out0,
                       uint8_t *out1,
                       uint8_t *out2,
                       uint8_t *out3, size_t outlen,
                       const uint8_t *in0,
                       const uint8_t *in1,
                       const uint8_t *in2,
                       const uint8_t *in3, size_t inlen) {
    __m256i s[25];
    uint8_t t0[SHAKE256_RATE];
    uint8_t t1[SHAKE256_RATE];
    uint8_t t2[SHAKE256_RATE];
    uint8_t t3[SHAKE256_RATE];

    /* zero state */
    for (size_t i = 0; i < 25; i++) {
        s[i] = _mm256_xor_si256(s[i], s[i]);
    }

    /* absorb 4 message of identical length in parallel */
    keccak_absorb4x(s, SHAKE256_RATE, in0, in1, in2, in3, inlen, 0x1F);

    /* Squeeze output */
    keccak_squeezeblocks4x(out0, out1, out2, out3, outlen / SHAKE256_RATE, s, SHAKE256_RATE);

    out0 += (outlen / SHAKE256_RATE) * SHAKE256_RATE;
    out1 += (outlen / SHAKE256_RATE) * SHAKE256_RATE;
    out2 += (outlen / SHAKE256_RATE) * SHAKE256_RATE;
    out3 += (outlen / SHAKE256_RATE) * SHAKE256_RATE;

    if (outlen % SHAKE256_RATE) {
        keccak_squeezeblocks4x(t0, t1, t2, t3, 1, s, SHAKE256_RATE);
        for (size_t i = 0; i < outlen % SHAKE256_RATE; i++) {
            out0[i] = t0[i];
            out1[i] = t1[i];
            out2[i] = t2[i];
            out3[i] = t3[i];
        }
    }
}

void PQCLEAN_KYBER1024_AVX2_shake256x4_prf(uint8_t *out0,
        uint8_t *out1,
        uint8_t *out2,
        uint8_t *out3,
        size_t outlen,
        const uint8_t *key,
        uint8_t nonce0,
        uint8_t nonce1,
        uint8_t nonce2,
        uint8_t nonce3) {
    uint8_t extseed[4][KYBER_SYMBYTES + 1];

    for (size_t i = 0; i < KYBER_SYMBYTES; i++) {
        extseed[0][i] = key[i];
        extseed[1][i] = key[i];
        extseed[2][i] = key[i];
        extseed[3][i] = key[i];
    }
    extseed[0][KYBER_SYMBYTES] = nonce0;
    extseed[1][KYBER_SYMBYTES] = nonce1;
    extseed[2][KYBER_SYMBYTES] = nonce2;
    extseed[3][KYBER_SYMBYTES] = nonce3;

    shake256x4(out0,
               out1,
               out2,
               out3,
               outlen,
               extseed[0],
               extseed[1],
               extseed[2],
               extseed[3],
               KYBER_SYMBYTES + 1);
}
