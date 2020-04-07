#include <immintrin.h>
#include <stdint.h>

#include "fips202x4.h"
#include "ntt.h"
#include "nttconsts.h"
#include "params.h"
#include "poly.h"
#include "reduce.h"
#include "rejsample.h"
#include "rounding.h"
#include "symmetric.h"

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_poly_reduce
*
* Description: Reduce all coefficients of input polynomial to representative
*              in [0,2*Q[.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_poly_reduce(poly *a) {
    PQCLEAN_DILITHIUM2_AVX2_reduce_avx(a->coeffs);
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_poly_csubq
*
* Description: For all coefficients of input polynomial subtract Q if
*              coefficient is bigger than Q.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_poly_csubq(poly *a) {
    PQCLEAN_DILITHIUM2_AVX2_csubq_avx(a->coeffs);
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_poly_freeze
*
* Description: Reduce all coefficients of the polynomial to standard
*              representatives.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_poly_freeze(poly *a) {
    PQCLEAN_DILITHIUM2_AVX2_reduce_avx(a->coeffs);
    PQCLEAN_DILITHIUM2_AVX2_csubq_avx(a->coeffs);

}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_poly_add
*
* Description: Add polynomials. No modular reduction is performed.
*
* Arguments:   - poly *c: pointer to output polynomial
*              - const poly *a: pointer to first summand
*              - const poly *b: pointer to second summand
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_poly_add(poly *c, const poly *a, const poly *b)  {
    __m256i vec0, vec1;
    for (size_t i = 0; i < N / 8; i++) {
        vec0 = _mm256_load_si256(&a->coeffs_x8[i]);
        vec1 = _mm256_load_si256(&b->coeffs_x8[i]);
        vec0 = _mm256_add_epi32(vec0, vec1);
        _mm256_store_si256(&c->coeffs_x8[i], vec0);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_poly_sub
*
* Description: Subtract polynomials. Assumes coefficients of second input
*              polynomial to be less than 2*Q. No modular reduction is
*              performed.
*
* Arguments:   - poly *c: pointer to output polynomial
*              - const poly *a: pointer to first input polynomial
*              - const poly *b: pointer to second input polynomial to be
*                               subtraced from first input polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_poly_sub(poly *c, const poly *a, const poly *b) {
    __m256i vec0, vec1;
    const __m256i twoq = _mm256_load_si256(_PQCLEAN_DILITHIUM2_AVX2_8x2q.as_vec);

    for (size_t i = 0; i < N / 8; i++) {
        vec0 = _mm256_load_si256(&a->coeffs_x8[i]);
        vec1 = _mm256_load_si256(&b->coeffs_x8[i]);
        vec0 = _mm256_add_epi32(vec0, twoq);
        vec0 = _mm256_sub_epi32(vec0, vec1);
        _mm256_store_si256(&c->coeffs_x8[i], vec0);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_poly_shiftl
*
* Description: Multiply polynomial by 2^D without modular reduction. Assumes
*              input coefficients to be less than 2^{32-D}.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_poly_shiftl(poly *a) {
    __m256i vec;

    for (size_t i = 0; i < N / 8; i++) {
        vec = _mm256_load_si256(&a->coeffs_x8[i]);
        vec = _mm256_slli_epi32(vec, D);
        _mm256_store_si256(&a->coeffs_x8[i], vec);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_poly_ntt
*
* Description: Forward NTT. Output coefficients can be up to 16*Q larger than
*              input coefficients.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_poly_ntt(poly *a) {
    ALIGNED_UINT64(N) tmp;

    for (size_t i = 0; i < N / 32; ++i) {
        PQCLEAN_DILITHIUM2_AVX2_ntt_levels0t2_avx(tmp.as_arr + 4 * i, a->coeffs + 4 * i, PQCLEAN_DILITHIUM2_AVX2_zetas.as_arr + 1);
    }
    for (size_t i = 0; i < N / 32; ++i) {
        PQCLEAN_DILITHIUM2_AVX2_ntt_levels3t8_avx(a->coeffs + 32 * i, tmp.as_arr + 32 * i, PQCLEAN_DILITHIUM2_AVX2_zetas.as_arr + 8 + 31 * i);
    }
}

/*************************************************
* Name:        poly_invntt_montgomery
*
* Description: Inverse NTT and multiplication with 2^{32}. Input coefficients
*              need to be less than 2*Q. Output coefficients are less than 2*Q.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_poly_invntt_montgomery(poly *a) {
    ALIGNED_UINT64(N) tmp;

    for (size_t i = 0; i < N / 32; i++) {
        PQCLEAN_DILITHIUM2_AVX2_invntt_levels0t4_avx(tmp.as_arr + 32 * i, a->coeffs + 32 * i, PQCLEAN_DILITHIUM2_AVX2_zetas_inv.as_arr + 31 * i);
    }
    for (size_t i = 0; i < N / 32; i++) {
        PQCLEAN_DILITHIUM2_AVX2_invntt_levels5t7_avx(a->coeffs + 4 * i, tmp.as_arr + 4 * i, PQCLEAN_DILITHIUM2_AVX2_zetas_inv.as_arr + 248);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_poly_pointwise_invmontgomery
*
* Description: Pointwise multiplication of polynomials in NTT domain
*              representation and multiplication of resulting polynomial
*              with 2^{-32}. Output coefficients are less than 2*Q if input
*              coefficient are less than 22*Q.
*
* Arguments:   - poly *c: pointer to output polynomial
*              - const poly *a: pointer to first input polynomial
*              - const poly *b: pointer to second input polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_poly_pointwise_invmontgomery(poly *c, const poly *a, const poly *b) {
    PQCLEAN_DILITHIUM2_AVX2_pointwise_avx(c->coeffs, a->coeffs, b->coeffs);
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_poly_power2round
*
* Description: For all coefficients c of the input polynomial,
*              compute c0, c1 such that c mod Q = c1*2^D + c0
*              with -2^{D-1} < c0 <= 2^{D-1}. Assumes coefficients to be
*              standard representatives.
*
* Arguments:   - poly *a1: pointer to output polynomial with coefficients c1
*              - poly *a0: pointer to output polynomial with coefficients Q + a0
*              - const poly *v: pointer to input polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_poly_power2round(poly *restrict a1,
        poly *restrict a0,
        const poly *restrict a) {
    for (size_t i = 0; i < N; ++i) {
        a1->coeffs[i] = PQCLEAN_DILITHIUM2_AVX2_power2round(a->coeffs[i], &a0->coeffs[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_poly_decompose
*
* Description: For all coefficients c of the input polynomial,
*              compute high and low bits c0, c1 such c mod Q = c1*ALPHA + c0
*              with -ALPHA/2 < c0 <= ALPHA/2 except c1 = (Q-1)/ALPHA where we
*              set c1 = 0 and -ALPHA/2 <= c0 = c mod Q - Q < 0.
*              Assumes coefficients to be standard representatives.
*
* Arguments:   - poly *a1: pointer to output polynomial with coefficients c1
*              - poly *a0: pointer to output polynomial with coefficients Q + a0
*              - const poly *c: pointer to input polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_poly_decompose(
    poly *restrict a1,
    poly *restrict a0,
    const poly *restrict a) {
    for (size_t i = 0; i < N; ++i) {
        a1->coeffs[i] = PQCLEAN_DILITHIUM2_AVX2_decompose(a->coeffs[i], &a0->coeffs[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_poly_make_hint
*
* Description: Compute hint polynomial. The coefficients of which indicate
*              whether the low bits of the corresponding coefficient of
*              the input polynomial overflow into the high bits.
*
* Arguments:   - poly *h: pointer to output hint polynomial
*              - const poly *a0: pointer to low part of input polynomial
*              - const poly *a1: pointer to high part of input polynomial
*
* Returns number of 1 bits.
**************************************************/
uint32_t PQCLEAN_DILITHIUM2_AVX2_poly_make_hint(
    poly *restrict h,
    const poly *restrict a0,
    const poly *restrict a1) {
    uint32_t s = 0;
    for (size_t i = 0; i < N; ++i) {
        h->coeffs[i] = PQCLEAN_DILITHIUM2_AVX2_make_hint(a0->coeffs[i], a1->coeffs[i]);
        s += h->coeffs[i];
    }
    return s;
}

/*************************************************
 * Name:       PQCLEAN_DILITHIUM2_AVX2_poly_use_hint
 *
 * Description: Use hint polynomial to correct the high bits of a polynomial.
*
* Arguments:   - poly *a: pointer to output polynomial with corrected high bits
*              - const poly *b: pointer to input polynomial
*              - const poly *h: pointer to input hint polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_poly_use_hint(
    poly *restrict a,
    const poly *restrict b,
    const poly *restrict h) {
    for (size_t i = 0; i < N; ++i) {
        a->coeffs[i] = PQCLEAN_DILITHIUM2_AVX2_use_hint(b->coeffs[i], h->coeffs[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_poly_chknorm
*
* Description: Check infinity norm of polynomial against given bound.
*              Assumes input coefficients to be standard representatives.
*
* Arguments:   - const poly *a: pointer to polynomial
*              - uint32_t B: norm bound
*
* Returns 0 if norm is strictly smaller than B and 1 otherwise.
**************************************************/
int PQCLEAN_DILITHIUM2_AVX2_poly_chknorm(const poly *a, uint32_t B) {
    int32_t t;

    /* It is ok to leak which coefficient violates the bound since
       the probability for each coefficient is independent of secret
       data but we must not leak the sign of the centralized representative. */
    for (size_t i = 0; i < N; ++i) {
        /* Absolute value of centralized representative */
        t = (Q - 1) / 2 - a->coeffs[i];
        t ^= (t >> 31);
        t = (Q - 1) / 2 - t;

        if ((uint32_t)t >= B) {
            return 1;
        }
    }

    return 0;
}

/*************************************************
* Name:        rej_uniform_ref
*
* Description: Sample uniformly random coefficients in [0, Q-1] by
*              performing rejection sampling using array of random bytes.
*
* Arguments:   - uint32_t *a: pointer to output array (allocated)
*              - size_t len: number of coefficients to be sampled
*              - const uint8_t *buf: array of random bytes
*              - size_t buflen: length of array of random bytes
*
* Returns number of sampled coefficients. Can be smaller than len if not enough
* random bytes were given.
**************************************************/
static size_t rej_uniform_ref(
    uint32_t *a,
    size_t len,
    const uint8_t *buf,
    size_t buflen) {
    size_t ctr, pos;
    uint32_t t;

    ctr = pos = 0;
    while (ctr < len && pos + 3 <= buflen) {
        t  = buf[pos++];
        t |= (uint32_t)buf[pos++] << 8;
        t |= (uint32_t)buf[pos++] << 16;
        t &= 0x7FFFFF;

        if (t < Q) {
            a[ctr++] = t;
        }
    }

    return ctr;
}

/*************************************************
* Name:        poly_uniform
*
* Description: Sample polynomial with uniformly random coefficients
*              in [0,Q-1] by performing rejection sampling using the
*              output stream from SHAKE256(seed|nonce).
*
* Arguments:   - poly *a: pointer to output polynomial
*              - const uint8_t seed[]: byte array with seed of length
*                                            SEEDBYTES
*              - uint16_t nonce: 2-byte nonce
**************************************************/
#define POLY_UNIFORM_NBLOCKS ((769 + STREAM128_BLOCKBYTES) / STREAM128_BLOCKBYTES)
#define POLY_UNIFORM_BUFLEN (POLY_UNIFORM_NBLOCKS * STREAM128_BLOCKBYTES)
void PQCLEAN_DILITHIUM2_AVX2_poly_uniform(poly *a,
        const uint8_t seed[SEEDBYTES],
        uint16_t nonce) {
    size_t ctr, off;
    size_t nblocks = POLY_UNIFORM_NBLOCKS;
    size_t buflen = POLY_UNIFORM_BUFLEN;
    uint8_t buf[POLY_UNIFORM_BUFLEN + 2];
    stream128_state state;

    stream128_init(&state, seed, nonce);
    stream128_squeezeblocks(buf, nblocks, &state);

    ctr = PQCLEAN_DILITHIUM2_AVX2_rej_uniform(a->coeffs, N, buf, buflen);

    while (ctr < N) {
        off = buflen % 3;
        for (size_t i = 0; i < off; ++i) {
            buf[i] = buf[buflen - off + i];
        }

        buflen = STREAM128_BLOCKBYTES + off;
        stream128_squeezeblocks(buf + off, 1, &state);
        ctr += rej_uniform_ref(a->coeffs + ctr, N - ctr, buf, buflen);
    }
    stream128_ctx_release(&state);
}

void PQCLEAN_DILITHIUM2_AVX2_poly_uniform_4x(poly *a0,
        poly *a1,
        poly *a2,
        poly *a3,
        const uint8_t seed[SEEDBYTES],
        uint16_t nonce0,
        uint16_t nonce1,
        uint16_t nonce2,
        uint16_t nonce3) {
    size_t ctr0, ctr1, ctr2, ctr3;
    uint8_t inbuf[4][SEEDBYTES + 2];
    uint8_t outbuf[4][5 * SHAKE128_RATE];
    __m256i state[25];

    for (size_t i = 0; i < SEEDBYTES; ++i) {
        inbuf[0][i] = seed[i];
        inbuf[1][i] = seed[i];
        inbuf[2][i] = seed[i];
        inbuf[3][i] = seed[i];
    }
    inbuf[0][SEEDBYTES + 0] = nonce0;
    inbuf[0][SEEDBYTES + 1] = nonce0 >> 8;
    inbuf[1][SEEDBYTES + 0] = nonce1;
    inbuf[1][SEEDBYTES + 1] = nonce1 >> 8;
    inbuf[2][SEEDBYTES + 0] = nonce2;
    inbuf[2][SEEDBYTES + 1] = nonce2 >> 8;
    inbuf[3][SEEDBYTES + 0] = nonce3;
    inbuf[3][SEEDBYTES + 1] = nonce3 >> 8;

    PQCLEAN_DILITHIUM2_AVX2_shake128_absorb4x(state, inbuf[0], inbuf[1], inbuf[2], inbuf[3],
            SEEDBYTES + 2);
    PQCLEAN_DILITHIUM2_AVX2_shake128_squeezeblocks4x(outbuf[0], outbuf[1], outbuf[2], outbuf[3], 5,
            state);

    ctr0 = PQCLEAN_DILITHIUM2_AVX2_rej_uniform(a0->coeffs, N, outbuf[0], 5 * SHAKE128_RATE);
    ctr1 = PQCLEAN_DILITHIUM2_AVX2_rej_uniform(a1->coeffs, N, outbuf[1], 5 * SHAKE128_RATE);
    ctr2 = PQCLEAN_DILITHIUM2_AVX2_rej_uniform(a2->coeffs, N, outbuf[2], 5 * SHAKE128_RATE);
    ctr3 = PQCLEAN_DILITHIUM2_AVX2_rej_uniform(a3->coeffs, N, outbuf[3], 5 * SHAKE128_RATE);

    while (ctr0 < N || ctr1 < N || ctr2 < N || ctr3 < N) {
        PQCLEAN_DILITHIUM2_AVX2_shake128_squeezeblocks4x(outbuf[0], outbuf[1], outbuf[2], outbuf[3], 1,
                state);

        ctr0 += rej_uniform_ref(a0->coeffs + ctr0, N - ctr0, outbuf[0],
                                SHAKE128_RATE);
        ctr1 += rej_uniform_ref(a1->coeffs + ctr1, N - ctr1, outbuf[1],
                                SHAKE128_RATE);
        ctr2 += rej_uniform_ref(a2->coeffs + ctr2, N - ctr2, outbuf[2],
                                SHAKE128_RATE);
        ctr3 += rej_uniform_ref(a3->coeffs + ctr3, N - ctr3, outbuf[3],
                                SHAKE128_RATE);
    }
}

/*************************************************
* Name:        rej_eta
*
* Description: Sample uniformly random coefficients in [-ETA, ETA] by
*              performing rejection sampling using array of random bytes.
*
* Arguments:   - uint32_t *a: pointer to output array (allocated)
*              - size_t len: number of coefficients to be sampled
*              - const uint8_t *buf: array of random bytes
*              - size_t buflen: length of array of random bytes
*
* Returns number of sampled coefficients. Can be smaller than len if not enough
* random bytes were given.
**************************************************/
static size_t rej_eta_ref(
    uint32_t *a,
    size_t len,
    const uint8_t *buf,
    size_t buflen) {
    size_t ctr, pos;
    uint32_t t0, t1;

    ctr = pos = 0;
    while (ctr < len && pos < buflen) {
        t0 = buf[pos] & 0x0F;
        t1 = buf[pos++] >> 4;

        if (t0 <= 2 * ETA) {
            a[ctr++] = Q + ETA - t0;
        }
        if (t1 <= 2 * ETA && ctr < len) {
            a[ctr++] = Q + ETA - t1;
        }
    }

    return ctr;
}

/*************************************************
* Name:        poly_uniform_eta
*
* Description: Sample polynomial with uniformly random coefficients
*              in [-ETA,ETA] by performing rejection sampling using the
*              output stream from SHAKE256(seed|nonce).
*
* Arguments:   - poly *a: pointer to output polynomial
*              - const uint8_t seed[]: byte array with seed of length
*                                            SEEDBYTES
*              - uint16_t nonce: 2-byte nonce
**************************************************/
#define POLY_UNIFORM_ETA_NBLOCKS (((N / 2 * (1u << SETABITS)) / (2 * ETA + 1) + STREAM128_BLOCKBYTES) / STREAM128_BLOCKBYTES)
#define POLY_UNIFORM_ETA_BUFLEN (POLY_UNIFORM_ETA_NBLOCKS*STREAM128_BLOCKBYTES)
void PQCLEAN_DILITHIUM2_AVX2_poly_uniform_eta(
    poly *a,
    const uint8_t seed[SEEDBYTES],
    uint16_t nonce) {
    size_t ctr;
    uint8_t buf[POLY_UNIFORM_ETA_BUFLEN];
    stream128_state state;

    stream128_init(&state, seed, nonce);
    stream128_squeezeblocks(buf, POLY_UNIFORM_ETA_NBLOCKS, &state);

    ctr = PQCLEAN_DILITHIUM2_AVX2_rej_eta(a->coeffs, N, buf, POLY_UNIFORM_ETA_BUFLEN);

    while (ctr < N) {
        stream128_squeezeblocks(buf, 1, &state);
        ctr += rej_eta_ref(a->coeffs + ctr, N - ctr, buf, STREAM128_BLOCKBYTES);
    }
    stream128_ctx_release(&state);
}

void PQCLEAN_DILITHIUM2_AVX2_poly_uniform_eta_4x(
    poly *a0,
    poly *a1,
    poly *a2,
    poly *a3,
    const uint8_t seed[SEEDBYTES],
    uint16_t nonce0,
    uint16_t nonce1,
    uint16_t nonce2,
    uint16_t nonce3) {
    size_t ctr0, ctr1, ctr2, ctr3;
    uint8_t inbuf[4][SEEDBYTES + 2];
    uint8_t outbuf[4][2 * SHAKE128_RATE];
    __m256i state[25];

    for (size_t i = 0; i < SEEDBYTES; ++i) {
        inbuf[0][i] = seed[i];
        inbuf[1][i] = seed[i];
        inbuf[2][i] = seed[i];
        inbuf[3][i] = seed[i];
    }
    inbuf[0][SEEDBYTES + 0] = nonce0;
    inbuf[0][SEEDBYTES + 1] = nonce0 >> 8;
    inbuf[1][SEEDBYTES + 0] = nonce1;
    inbuf[1][SEEDBYTES + 1] = nonce1 >> 8;
    inbuf[2][SEEDBYTES + 0] = nonce2;
    inbuf[2][SEEDBYTES + 1] = nonce2 >> 8;
    inbuf[3][SEEDBYTES + 0] = nonce3;
    inbuf[3][SEEDBYTES + 1] = nonce3 >> 8;

    PQCLEAN_DILITHIUM2_AVX2_shake128_absorb4x(state, inbuf[0], inbuf[1], inbuf[2], inbuf[3],
            SEEDBYTES + 2);
    PQCLEAN_DILITHIUM2_AVX2_shake128_squeezeblocks4x(outbuf[0], outbuf[1], outbuf[2], outbuf[3], 2,
            state);

    ctr0 = PQCLEAN_DILITHIUM2_AVX2_rej_eta(a0->coeffs, N, outbuf[0], 2 * SHAKE128_RATE);
    ctr1 = PQCLEAN_DILITHIUM2_AVX2_rej_eta(a1->coeffs, N, outbuf[1], 2 * SHAKE128_RATE);
    ctr2 = PQCLEAN_DILITHIUM2_AVX2_rej_eta(a2->coeffs, N, outbuf[2], 2 * SHAKE128_RATE);
    ctr3 = PQCLEAN_DILITHIUM2_AVX2_rej_eta(a3->coeffs, N, outbuf[3], 2 * SHAKE128_RATE);

    while (ctr0 < N || ctr1 < N || ctr2 < N || ctr3 < N) {
        PQCLEAN_DILITHIUM2_AVX2_shake128_squeezeblocks4x(outbuf[0], outbuf[1], outbuf[2], outbuf[3], 1,
                state);

        ctr0 += rej_eta_ref(a0->coeffs + ctr0, N - ctr0, outbuf[0], SHAKE128_RATE);
        ctr1 += rej_eta_ref(a1->coeffs + ctr1, N - ctr1, outbuf[1], SHAKE128_RATE);
        ctr2 += rej_eta_ref(a2->coeffs + ctr2, N - ctr2, outbuf[2], SHAKE128_RATE);
        ctr3 += rej_eta_ref(a3->coeffs + ctr3, N - ctr3, outbuf[3], SHAKE128_RATE);
    }
}

/*************************************************
* Name:        rej_gamma1m1_ref
*
* Description: Sample uniformly random coefficients
*              in [-(GAMMA1 - 1), GAMMA1 - 1] by performing rejection sampling
*              using array of random bytes.
*
* Arguments:   - uint32_t *a: pointer to output array (allocated)
*              - size_t len: number of coefficients to be sampled
*              - const uint8_t *buf: array of random bytes
*              - size_t buflen: length of array of random bytes
*
* Returns number of sampled coefficients. Can be smaller than len if not enough
* random bytes were given.
**************************************************/
static size_t rej_gamma1m1_ref(
    uint32_t *a,
    size_t len,
    const uint8_t *buf,
    size_t buflen) {
    size_t ctr, pos;
    uint32_t t0, t1;

    ctr = pos = 0;
    while (ctr < len && pos + 5 <= buflen) {
        t0  = buf[pos];
        t0 |= (uint32_t)buf[pos + 1] << 8;
        t0 |= (uint32_t)buf[pos + 2] << 16;
        t0 &= 0xFFFFF;

        t1  = buf[pos + 2] >> 4;
        t1 |= (uint32_t)buf[pos + 3] << 4;
        t1 |= (uint32_t)buf[pos + 4] << 12;

        pos += 5;

        if (t0 <= 2 * GAMMA1 - 2) {
            a[ctr++] = Q + GAMMA1 - 1 - t0;
        }
        if (t1 <= 2 * GAMMA1 - 2 && ctr < len) {
            a[ctr++] = Q + GAMMA1 - 1 - t1;
        }
    }
    return ctr;
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_poly_uniform_gamma1m1
*
* Description: Sample polynomial with uniformly random coefficients
*              in [-(GAMMA1 - 1), GAMMA1 - 1] by performing rejection
*              sampling on output stream of SHAKE256(seed|nonce).
*
* Arguments:   - poly *a: pointer to output polynomial
*              - const uint8_t seed[]: byte array with seed of length
*                                            CRHBYTES
*              - uint16_t nonce: 16-bit nonce
**************************************************/
#define POLY_UNIFORM_GAMMA1M1_NBLOCKS ((641 + STREAM256_BLOCKBYTES) / STREAM256_BLOCKBYTES)
#define POLY_UNIFORM_GAMMA1M1_BUFLEN (POLY_UNIFORM_GAMMA1M1_NBLOCKS * STREAM256_BLOCKBYTES)
void PQCLEAN_DILITHIUM2_AVX2_poly_uniform_gamma1m1(
    poly *a,
    const uint8_t seed[CRHBYTES],
    uint16_t nonce) {
    size_t ctr, off;
    size_t buflen = POLY_UNIFORM_GAMMA1M1_BUFLEN;
    uint8_t buf[POLY_UNIFORM_GAMMA1M1_BUFLEN + 4];
    stream256_state state;

    stream256_init(&state, seed, nonce);
    stream256_squeezeblocks(buf, POLY_UNIFORM_GAMMA1M1_NBLOCKS, &state);

    ctr = PQCLEAN_DILITHIUM2_AVX2_rej_gamma1m1(a->coeffs, N, buf, POLY_UNIFORM_GAMMA1M1_BUFLEN);

    while (ctr < N) {
        off = buflen % 5;
        for (size_t i = 0; i < off; ++i) {
            buf[i] = buf[buflen - off + i];
        }

        buflen = STREAM256_BLOCKBYTES + off;
        stream256_squeezeblocks(buf + off, 1, &state);
        ctr += rej_gamma1m1_ref(a->coeffs + ctr, N - ctr, buf, buflen);
    }
    stream256_ctx_release(&state);
}

void PQCLEAN_DILITHIUM2_AVX2_poly_uniform_gamma1m1_4x(poly *a0,
        poly *a1,
        poly *a2,
        poly *a3,
        const uint8_t seed[CRHBYTES],
        uint16_t nonce0,
        uint16_t nonce1,
        uint16_t nonce2,
        uint16_t nonce3) {
    size_t ctr0, ctr1, ctr2, ctr3;
    uint8_t inbuf[4][CRHBYTES + 2];
    uint8_t outbuf[4][5 * SHAKE256_RATE];
    __m256i state[25];

    for (size_t i = 0; i < CRHBYTES; ++i) {
        inbuf[0][i] = seed[i];
        inbuf[1][i] = seed[i];
        inbuf[2][i] = seed[i];
        inbuf[3][i] = seed[i];
    }
    inbuf[0][CRHBYTES + 0] = nonce0 & 0xFF;
    inbuf[0][CRHBYTES + 1] = nonce0 >> 8;
    inbuf[1][CRHBYTES + 0] = nonce1 & 0xFF;
    inbuf[1][CRHBYTES + 1] = nonce1 >> 8;
    inbuf[2][CRHBYTES + 0] = nonce2 & 0xFF;
    inbuf[2][CRHBYTES + 1] = nonce2 >> 8;
    inbuf[3][CRHBYTES + 0] = nonce3 & 0xFF;
    inbuf[3][CRHBYTES + 1] = nonce3 >> 8;

    PQCLEAN_DILITHIUM2_AVX2_shake256_absorb4x(state, inbuf[0], inbuf[1], inbuf[2], inbuf[3],
            CRHBYTES + 2);
    PQCLEAN_DILITHIUM2_AVX2_shake256_squeezeblocks4x(outbuf[0], outbuf[1], outbuf[2], outbuf[3], 5,
            state);

    ctr0 = rej_gamma1m1_ref(a0->coeffs, N, outbuf[0], 5 * SHAKE256_RATE);
    ctr1 = rej_gamma1m1_ref(a1->coeffs, N, outbuf[1], 5 * SHAKE256_RATE);
    ctr2 = rej_gamma1m1_ref(a2->coeffs, N, outbuf[2], 5 * SHAKE256_RATE);
    ctr3 = rej_gamma1m1_ref(a3->coeffs, N, outbuf[3], 5 * SHAKE256_RATE);

    while (ctr0 < N || ctr1 < N || ctr2 < N || ctr3 < N) {
        PQCLEAN_DILITHIUM2_AVX2_shake256_squeezeblocks4x(outbuf[0], outbuf[1], outbuf[2], outbuf[3], 1,
                state);

        ctr0 += rej_gamma1m1_ref(a0->coeffs + ctr0, N - ctr0, outbuf[0],
                                 SHAKE256_RATE);
        ctr1 += rej_gamma1m1_ref(a1->coeffs + ctr1, N - ctr1, outbuf[1],
                                 SHAKE256_RATE);
        ctr2 += rej_gamma1m1_ref(a2->coeffs + ctr2, N - ctr2, outbuf[2],
                                 SHAKE256_RATE);
        ctr3 += rej_gamma1m1_ref(a3->coeffs + ctr3, N - ctr3, outbuf[3],
                                 SHAKE256_RATE);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_polyeta_pack
*
* Description: Bit-pack polynomial with coefficients in [-ETA,ETA].
*              Input coefficients are assumed to lie in [Q-ETA,Q+ETA].
*
* Arguments:   - uint8_t *r: pointer to output byte array with at least
*                                  POLETA_SIZE_PACKED bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_polyeta_pack(uint8_t *restrict r, const poly *restrict a) {
    uint8_t t[N / 2];
    for (size_t i = 0; i < N / 2; ++i) {
        t[0] = Q + ETA - a->coeffs[2 * i + 0];
        t[1] = Q + ETA - a->coeffs[2 * i + 1];
        r[i] = t[0] | (t[1] << 4);
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_polyeta_unpack
*
* Description: Unpack polynomial with coefficients in [-ETA,ETA].
*              Output coefficients lie in [Q-ETA,Q+ETA].
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: byte array with bit-packed polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_polyeta_unpack(poly *restrict r, const uint8_t *restrict a) {
    for (size_t i = 0; i < N / 2; ++i) {
        r->coeffs[2 * i + 0] = a[i] & 0x0F;
        r->coeffs[2 * i + 1] = a[i] >> 4;
        r->coeffs[2 * i + 0] = Q + ETA - r->coeffs[2 * i + 0];
        r->coeffs[2 * i + 1] = Q + ETA - r->coeffs[2 * i + 1];
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_polyt1_pack
*
* Description: Bit-pack polynomial t1 with coefficients fitting in 9 bits.
*              Input coefficients are assumed to be standard representatives.
*
* Arguments:   - uint8_t *r: pointer to output byte array with at least
*                                  POLT1_SIZE_PACKED bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_polyt1_pack(uint8_t *restrict r, const poly *restrict a) {
    for (size_t i = 0; i < N / 8; ++i) {
        r[9 * i + 0]  = (uint8_t)((a->coeffs[8 * i + 0] >> 0));
        r[9 * i + 1]  = (uint8_t)((a->coeffs[8 * i + 0] >> 8) | (a->coeffs[8 * i + 1] << 1));
        r[9 * i + 2]  = (uint8_t)((a->coeffs[8 * i + 1] >> 7) | (a->coeffs[8 * i + 2] << 2));
        r[9 * i + 3]  = (uint8_t)((a->coeffs[8 * i + 2] >> 6) | (a->coeffs[8 * i + 3] << 3));
        r[9 * i + 4]  = (uint8_t)((a->coeffs[8 * i + 3] >> 5) | (a->coeffs[8 * i + 4] << 4));
        r[9 * i + 5]  = (uint8_t)((a->coeffs[8 * i + 4] >> 4) | (a->coeffs[8 * i + 5] << 5));
        r[9 * i + 6]  = (uint8_t)((a->coeffs[8 * i + 5] >> 3) | (a->coeffs[8 * i + 6] << 6));
        r[9 * i + 7]  = (uint8_t)((a->coeffs[8 * i + 6] >> 2) | (a->coeffs[8 * i + 7] << 7));
        r[9 * i + 8]  = (uint8_t)((a->coeffs[8 * i + 7] >> 1));
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_polyt1_unpack
*
* Description: Unpack polynomial t1 with 9-bit coefficients.
*              Output coefficients are standard representatives.
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: byte array with bit-packed polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_polyt1_unpack(poly *restrict r, const uint8_t *restrict a) {
    for (size_t i = 0; i < N / 8; ++i) {
        r->coeffs[8 * i + 0] = ((a[9 * i + 0] >> 0) | ((uint32_t)a[9 * i + 1] << 8)) & 0x1FF;
        r->coeffs[8 * i + 1] = ((a[9 * i + 1] >> 1) | ((uint32_t)a[9 * i + 2] << 7)) & 0x1FF;
        r->coeffs[8 * i + 2] = ((a[9 * i + 2] >> 2) | ((uint32_t)a[9 * i + 3] << 6)) & 0x1FF;
        r->coeffs[8 * i + 3] = ((a[9 * i + 3] >> 3) | ((uint32_t)a[9 * i + 4] << 5)) & 0x1FF;
        r->coeffs[8 * i + 4] = ((a[9 * i + 4] >> 4) | ((uint32_t)a[9 * i + 5] << 4)) & 0x1FF;
        r->coeffs[8 * i + 5] = ((a[9 * i + 5] >> 5) | ((uint32_t)a[9 * i + 6] << 3)) & 0x1FF;
        r->coeffs[8 * i + 6] = ((a[9 * i + 6] >> 6) | ((uint32_t)a[9 * i + 7] << 2)) & 0x1FF;
        r->coeffs[8 * i + 7] = ((a[9 * i + 7] >> 7) | ((uint32_t)a[9 * i + 8] << 1)) & 0x1FF;
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_polyt0_pack
*
* Description: Bit-pack polynomial t0 with coefficients in ]-2^{D-1}, 2^{D-1}].
*              Input coefficients are assumed to lie in ]Q-2^{D-1}, Q+2^{D-1}].
*
* Arguments:   - uint8_t *r: pointer to output byte array with at least
*                                  POLT0_SIZE_PACKED bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_polyt0_pack(uint8_t *restrict r, const poly *restrict a) {
    uint32_t t[4];

    for (size_t i = 0; i < N / 4; ++i) {
        t[0] = Q + (1U << (D - 1)) - a->coeffs[4 * i + 0];
        t[1] = Q + (1U << (D - 1)) - a->coeffs[4 * i + 1];
        t[2] = Q + (1U << (D - 1)) - a->coeffs[4 * i + 2];
        t[3] = Q + (1U << (D - 1)) - a->coeffs[4 * i + 3];

        r[7 * i + 0]  =  t[0];
        r[7 * i + 1]  =  t[0] >> 8;
        r[7 * i + 1] |=  t[1] << 6;
        r[7 * i + 2]  =  t[1] >> 2;
        r[7 * i + 3]  =  t[1] >> 10;
        r[7 * i + 3] |=  t[2] << 4;
        r[7 * i + 4]  =  t[2] >> 4;
        r[7 * i + 5]  =  t[2] >> 12;
        r[7 * i + 5] |=  t[3] << 2;
        r[7 * i + 6]  =  t[3] >> 6;
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_polyt0_unpack
*
* Description: Unpack polynomial t0 with coefficients in ]-2^{D-1}, 2^{D-1}].
*              Output coefficients lie in ]Q-2^{D-1},Q+2^{D-1}].
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: byte array with bit-packed polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_polyt0_unpack(poly *restrict r, const uint8_t *restrict a) {
    for (size_t i = 0; i < N / 4; ++i) {
        r->coeffs[4 * i + 0]  = a[7 * i + 0];
        r->coeffs[4 * i + 0] |= (uint32_t)(a[7 * i + 1] & 0x3F) << 8;

        r->coeffs[4 * i + 1]  = a[7 * i + 1] >> 6;
        r->coeffs[4 * i + 1] |= (uint32_t)a[7 * i + 2] << 2;
        r->coeffs[4 * i + 1] |= (uint32_t)(a[7 * i + 3] & 0x0F) << 10;

        r->coeffs[4 * i + 2]  = a[7 * i + 3] >> 4;
        r->coeffs[4 * i + 2] |= (uint32_t)a[7 * i + 4] << 4;
        r->coeffs[4 * i + 2] |= (uint32_t)(a[7 * i + 5] & 0x03) << 12;

        r->coeffs[4 * i + 3]  = a[7 * i + 5] >> 2;
        r->coeffs[4 * i + 3] |= (uint32_t)a[7 * i + 6] << 6;

        r->coeffs[4 * i + 0] = Q + (1U << (D - 1)) - r->coeffs[4 * i + 0];
        r->coeffs[4 * i + 1] = Q + (1U << (D - 1)) - r->coeffs[4 * i + 1];
        r->coeffs[4 * i + 2] = Q + (1U << (D - 1)) - r->coeffs[4 * i + 2];
        r->coeffs[4 * i + 3] = Q + (1U << (D - 1)) - r->coeffs[4 * i + 3];
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_polyz_pack
*
* Description: Bit-pack polynomial z with coefficients
*              in [-(GAMMA1 - 1), GAMMA1 - 1].
*              Input coefficients are assumed to be standard representatives.
*
* Arguments:   - uint8_t *r: pointer to output byte array with at least
*                                  POLZ_SIZE_PACKED bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_polyz_pack(uint8_t *restrict r, const poly *restrict a) {
    uint32_t t[2];

    for (size_t i = 0; i < N / 2; ++i) {
        /* Map to {0,...,2*GAMMA1 - 2} */
        t[0] = GAMMA1 - 1 - a->coeffs[2 * i + 0];
        t[0] += ((int32_t)t[0] >> 31) & Q;
        t[1] = GAMMA1 - 1 - a->coeffs[2 * i + 1];
        t[1] += ((int32_t)t[1] >> 31) & Q;

        r[5 * i + 0]  = t[0];
        r[5 * i + 1]  = t[0] >> 8;
        r[5 * i + 2]  = t[0] >> 16;
        r[5 * i + 2] |= t[1] << 4;
        r[5 * i + 3]  = t[1] >> 4;
        r[5 * i + 4]  = t[1] >> 12;
    }
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_polyz_unpack
*
* Description: Unpack polynomial z with coefficients
*              in [-(GAMMA1 - 1), GAMMA1 - 1].
*              Output coefficients are standard representatives.
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: byte array with bit-packed polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_polyz_unpack(poly *restrict r, const uint8_t *restrict a) {
    for (size_t i = 0; i < N / 2; ++i) {
        r->coeffs[2 * i + 0]  = a[5 * i + 0];
        r->coeffs[2 * i + 0] |= (uint32_t)a[5 * i + 1] << 8;
        r->coeffs[2 * i + 0] |= (uint32_t)(a[5 * i + 2] & 0x0F) << 16;

        r->coeffs[2 * i + 1]  = a[5 * i + 2] >> 4;
        r->coeffs[2 * i + 1] |= (uint32_t)a[5 * i + 3] << 4;
        r->coeffs[2 * i + 1] |= (uint32_t)a[5 * i + 4] << 12;

        r->coeffs[2 * i + 0] = GAMMA1 - 1 - r->coeffs[2 * i + 0];
        r->coeffs[2 * i + 0] += ((int32_t)r->coeffs[2 * i + 0] >> 31) & Q;
        r->coeffs[2 * i + 1] = GAMMA1 - 1 - r->coeffs[2 * i + 1];
        r->coeffs[2 * i + 1] += ((int32_t)r->coeffs[2 * i + 1] >> 31) & Q;
    }

}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_AVX2_polyw1_pack
*
* Description: Bit-pack polynomial w1 with coefficients in [0, 15].
*              Input coefficients are assumed to be standard representatives.
*
* Arguments:   - uint8_t *r: pointer to output byte array with at least
*                                  POLW1_SIZE_PACKED bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void PQCLEAN_DILITHIUM2_AVX2_polyw1_pack(
    uint8_t *restrict r,
    const poly *restrict a) {
    for (size_t i = 0; i < N / 2; ++i) {
        r[i] = a->coeffs[2 * i + 0] | (a->coeffs[2 * i + 1] << 4);
    }
}
