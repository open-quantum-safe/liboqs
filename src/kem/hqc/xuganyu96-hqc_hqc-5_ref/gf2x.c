/**
 * @file gf2x.c
 * @brief Implementation of carry-less multiplication of two polynomials over GF(2) mod X^n - 1.
 *
 * @details
 * The temporary buffer requirement for recursive Karatsuba is computed as follows:
 *  - At each recursion level on words of length n, karatsuba_mul needs 8*n words.
 *    (z0, z2, zmid each 2*n, plus ta and tb of n each: total 8*n.)
 *  - Child calls operate on half the length (n/2), needing 8*(n/2) words, placed immediately after.
 *  - Summing across levels n + n/2 + n/4 + ... < 2*n, so total < 8*n * 2 = 16*n words.
 *  - We set TMP_BUFFER_WORDS = 16 * VEC_N_SIZE_64 to guarantee enough space for all recursion levels.
 */

#include "gf2x.h"
#include <stdint.h>
#include <string.h>
#include "parameters.h"

/** @def KARATSUBA_THRESHOLD
 *  @brief Input size (in words) below which schoolbook_mul is used.
 */
#define KARATSUBA_THRESHOLD 16

/** @def TMP_BUFFER_WORDS
 *  @brief Total size in 64-bit words for the temporary buffer used by recursive Karatsuba.
 */
#define TMP_BUFFER_WORDS (16 * VEC_N_SIZE_64)

/**
 * @brief Schoolbook multiplication over GF(2).
 *
 * Computes r = a * b where a, b are length-n words, result is 2*n words.
 *
 * @param[out] r  Result buffer, size 2*n words.
 * @param[in]  a  Operand a, size n words.
 * @param[in]  b  Operand b, size n words.
 * @param[in]  n  Number of 64-bit words in a and b.
 */
static void schoolbook_mul(uint64_t *r, const uint64_t *a, const uint64_t *b, size_t n) {
    memset(r, 0, 2 * n * sizeof(uint64_t));
    for (size_t i = 0; i < n; i++) {
        uint64_t ai = a[i];
        for (int bit = 0; bit < 64; bit++) {
            uint64_t mask = -((ai >> bit) & 1ULL);
            size_t base = i;
            int sh = bit;
            int inv = 64 - sh;
            if (sh == 0) {
                for (size_t j = 0; j < n; j++) {
                    r[base + j] ^= b[j] & mask;
                }
            } else {
                for (size_t j = 0; j < n; j++) {
                    r[base + j] ^= (b[j] << sh) & mask;
                    r[base + j + 1] ^= (b[j] >> inv) & mask;
                }
            }
        }
    }
}

/**
 * @brief Karatsuba multiplication using a caller-supplied temporary buffer.
 *
 * If n <= KARATSUBA_THRESHOLD, falls back to schoolbook_mul.
 * Otherwise splits operands in half and applies recursion.
 *
 * @param[out] r            Result buffer, size 2*n words.
 * @param[in]  a            Operand a, size n words.
 * @param[in]  b            Operand b, size n words.
 * @param[in]  n            Number of 64-bit words in a and b.
 * @param[in]  tmp_buffer  Temporary buffer, size >= 8*n words (child calls use remainder).
 */
static void karatsuba_mul(uint64_t *r, const uint64_t *a, const uint64_t *b, size_t n, uint64_t *tmp_buffer) {
    if (n <= KARATSUBA_THRESHOLD) {
        schoolbook_mul(r, a, b, n);
        return;
    }

    size_t m = n >> 1;
    size_t n0 = m;
    size_t n1 = n - m;

    /* take successive chunks of tmp_buffer for each intermediate result */
    uint64_t *z0 = tmp_buffer;   /* low-half product, size 2*n words */
    uint64_t *z2 = z0 + 2 * n;   /* high-half product, size 2*n words */
    uint64_t *zmid = z2 + 2 * n; /* middle product, size 2*n words */

    /* ta and tb hold the sums of low and high halves: */
    /* ta[i] = a0[i] XOR a1[i], tb[i] = b0[i] XOR b1[i] for i < n1 */
    uint64_t *ta = zmid + 2 * n;
    uint64_t *tb = ta + n;

    /* buffer for child recursions */
    uint64_t *child_buffer = tmp_buffer + 8 * n;

    /* 1) low * low */
    karatsuba_mul(z0, a, b, n0, child_buffer);

    /* 2) high * high */
    karatsuba_mul(z2, a + m, b + m, n1, child_buffer);

    /* 3) (a0+a1)*(b0+b1) */
    for (size_t i = 0; i < n1; i++) {
        uint64_t loa = (i < n0 ? a[i] : 0);
        uint64_t lob = (i < n0 ? b[i] : 0);
        ta[i] = loa ^ a[m + i];
        tb[i] = lob ^ b[m + i];
    }
    karatsuba_mul(zmid, ta, tb, n1, child_buffer);

    /* 4) assemble into r */
    memset(r, 0, 2 * n * sizeof(uint64_t));
    for (size_t i = 0; i < 2 * n0; i++) r[i] ^= z0[i];
    for (size_t i = 0; i < 2 * n1; i++) r[2 * m + i] ^= z2[i];
    for (size_t i = 0; i < 2 * n1; i++) {
        uint64_t z0i = (i < 2 * n0 ? z0[i] : 0);
        uint64_t z2i = (i < 2 * n1 ? z2[i] : 0);
        uint64_t mid = zmid[i] ^ z0i ^ z2i;
        r[m + i] ^= mid;
    }
}

/**
 * @brief Modular reduction of a degree < 2*n polynomial mod (X^n - 1).
 *
 * Folds the high half of the full product back into the low half
 * and masks any excess bits in the last word.
 *
 * @param[out] o  Result buffer, size VEC_N_SIZE_64 words.
 * @param[in]  a  Input buffer, size 2*VEC_N_SIZE_64 words.
 */
static void reduce(uint64_t *o, const uint64_t *a) {
    for (size_t i = 0; i < VEC_N_SIZE_64; i++) {
        uint64_t r = a[i + VEC_N_SIZE_64 - 1] >> (PARAM_N & 0x3F);
        uint64_t carry = a[i + VEC_N_SIZE_64] << (64 - (PARAM_N & 0x3F));
        o[i] = a[i] ^ r ^ carry;
    }
    o[VEC_N_SIZE_64 - 1] &= BITMASK(PARAM_N, 64);
}

/**
 * @brief Carry-less multiplication mod (X^PARAM_N - 1).
 *
 * Computes o = a1 * a2, each operand of VEC_N_SIZE_64 words, then reduces.
 *
 * @param[out] o   Result buffer, size VEC_N_SIZE_64 words.
 * @param[in]  a1  Operand polynomial a(x).
 * @param[in]  a2  Operand polynomial b(x).
 */
void vect_mul(uint64_t *o, const uint64_t *a1, const uint64_t *a2) {
    uint64_t unreduced[2 * VEC_N_SIZE_64];
    uint64_t tmp_buffer[TMP_BUFFER_WORDS];

    /* multiply via Karatsuba into unreduced */
    karatsuba_mul(unreduced, a1, a2, VEC_N_SIZE_64, tmp_buffer);

    /* reduce modulo X^n - 1 */
    reduce(o, unreduced);
}
