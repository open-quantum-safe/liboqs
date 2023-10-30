
/*
 * This file was originally licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html)
 * at https://github.com/GMUCERG/PQC_NEON/blob/main/neon/kyber or
 * public domain at https://github.com/cothan/kyber/blob/master/neon
 *
 * We choose
 * CC0 1.0 Universal or the following MIT License for this file.
 *
 * MIT License
 *
 * Copyright (c) 2023: Hanno Becker, Vincent Hwang, Matthias J. Kannwischer, Bo-Yin Yang, and Shang-Yi Yang
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <arm_neon.h>
#include "params.h"
#include "poly.h"
#include "ntt.h"
#include "cbd.h"
#include "symmetric.h"


/*************************************************
* Name:        neon_poly_getnoise_eta1_2x
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER_ETA1
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *seed: pointer to input seed
*                                     (of length KYBER_SYMBYTES bytes)
*              - uint8_t nonce: one-byte input nonce
**************************************************/
void neon_poly_getnoise_eta1_2x(int16_t vec1[KYBER_N], int16_t vec2[KYBER_N],
                                const uint8_t seed[KYBER_SYMBYTES],
                                uint8_t nonce1, uint8_t nonce2) {
    uint8_t buf1[KYBER_ETA1 * KYBER_N / 4],
            buf2[KYBER_ETA1 * KYBER_N / 4];
    neon_prf(buf1, buf2, sizeof(buf1), seed, nonce1, nonce2);
    poly_cbd_eta1(vec1, buf1);
    poly_cbd_eta1(vec2, buf2);
}

/*************************************************
* Name:        neon_poly_getnoise_eta2_2x
*              neon_poly_getnoise_eta2
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER_ETA2
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *seed: pointer to input seed
*                                     (of length KYBER_SYMBYTES bytes)
*              - uint8_t nonce: one-byte input nonce
**************************************************/
void neon_poly_getnoise_eta2_2x(int16_t vec1[KYBER_N], int16_t vec2[KYBER_N],
                                const uint8_t seed[KYBER_SYMBYTES],
                                uint8_t nonce1, uint8_t nonce2) {
    uint8_t buf1[KYBER_ETA2 * KYBER_N / 4],
            buf2[KYBER_ETA2 * KYBER_N / 4];
    neon_prf(buf1, buf2, sizeof(buf1), seed, nonce1, nonce2);
    poly_cbd_eta2(vec1, buf1);
    poly_cbd_eta2(vec2, buf2);
}

void neon_poly_getnoise_eta2(int16_t r[KYBER_N],
                             const uint8_t seed[KYBER_SYMBYTES],
                             uint8_t nonce) {
    uint8_t buf[KYBER_ETA2 * KYBER_N / 4];
    prf(buf, sizeof(buf), seed, nonce);
    poly_cbd_eta2(r, buf);
}

/*************************************************
* Name:        neon_ntt
*
* Description: Computes negacyclic number-theoretic transform (NTT) of
*              a polynomial in place;
*              inputs assumed to be in normal order, output in bitreversed order
*
* Arguments:   - uint16_t *r: pointer to in/output polynomial
**************************************************/
void neon_poly_ntt(int16_t r[KYBER_N]) {
    ntt(r);
}

/*************************************************
* Name:        neon_invntt
*
* Description: Computes inverse of negacyclic number-theoretic transform (NTT)
*              of a polynomial in place;
*              inputs assumed to be in bitreversed order, output in normal order
*
* Arguments:   - uint16_t *a: pointer to in/output polynomial
**************************************************/
void neon_poly_invntt_tomont(int16_t r[KYBER_N]) {
    invntt(r);
}


/*************************************************
* Name:        neon_poly_add_reduce
*
* Description: Add two polynomials; no modular reduction is performed
*              Applies Barrett reduction to all coefficients of a polynomial
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments: - poly *r: pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
**************************************************/
extern void PQCLEAN_KYBER512_AARCH64__asm_add_reduce(int16_t *, const int16_t *);
void neon_poly_add_reduce(int16_t c[KYBER_N], const int16_t a[KYBER_N]) {
    PQCLEAN_KYBER512_AARCH64__asm_add_reduce(c, a);
}

extern void PQCLEAN_KYBER512_AARCH64__asm_add_add_reduce(int16_t *, const int16_t *, const int16_t *);
void neon_poly_add_add_reduce(int16_t c[KYBER_N], const int16_t a[KYBER_N], const int16_t b[KYBER_N]) {
    PQCLEAN_KYBER512_AARCH64__asm_add_add_reduce(c, a, b);
}

/*************************************************
* Name:        neon_poly_sub_reduce
*
* Description: Subtract two polynomials; no modular reduction is performed
*              Applies Barrett reduction to all coefficients of a polynomial
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments: - poly *r:       pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
**************************************************/
extern void PQCLEAN_KYBER512_AARCH64__asm_sub_reduce(int16_t *, const int16_t *);
void neon_poly_sub_reduce(int16_t c[KYBER_N], const int16_t a[KYBER_N]) {
    PQCLEAN_KYBER512_AARCH64__asm_sub_reduce(c, a);
}
