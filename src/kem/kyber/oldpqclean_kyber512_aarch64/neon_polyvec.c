
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
#include "reduce.h"
#include "ntt.h"
#include "poly.h"
#include "polyvec.h"

#include "NTT_params.h"

#define _V (((1U << 26) + KYBER_Q / 2) / KYBER_Q)

/*************************************************
* Name:        neon_polyvec_ntt
*
* Description: Apply forward NTT to all elements of a vector of polynomials
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
void neon_polyvec_ntt(int16_t r[KYBER_K][KYBER_N]) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        neon_poly_ntt(r[i]);
    }
}

/*************************************************
* Name:        neon_polyvec_invntt_to_mont
*
* Description: Apply inverse NTT to all elements of a vector of polynomials
*              and multiply by Montgomery factor 2^16
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
void neon_polyvec_invntt_to_mont(int16_t r[KYBER_K][KYBER_N]) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        neon_poly_invntt_tomont(r[i]);
    }
}

/*************************************************
* Name:        neon_polyvec_add_reduce
*
* Description: Applies Barrett reduction to each coefficient
*              of each element of a vector of polynomials
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments: - polyvec *r:       pointer to output vector of polynomials
*            - const polyvec *a: pointer to first input vector of polynomials
*            - const polyvec *b: pointer to second input vector of polynomials
**************************************************/
void neon_polyvec_add_reduce(int16_t c[KYBER_K][KYBER_N], int16_t a[KYBER_K][KYBER_N]) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        // c = c + a;
        // c = reduce(c);
        neon_poly_add_reduce(c[i], a[i]);
    }
}

