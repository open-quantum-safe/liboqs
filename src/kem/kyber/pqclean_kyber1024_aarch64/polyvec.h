
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

#ifndef POLYVEC_H
#define POLYVEC_H

#include "params.h"
#include "poly.h"
#include <stdint.h>

typedef struct {
    poly vec[KYBER_K];
} polyvec;

#define polyvec_compress KYBER_NAMESPACE(polyvec_compress)
void polyvec_compress(uint8_t r[KYBER_POLYVECCOMPRESSEDBYTES], int16_t a[KYBER_K][KYBER_N]);
#define polyvec_decompress KYBER_NAMESPACE(polyvec_decompress)
void polyvec_decompress(int16_t r[KYBER_K][KYBER_N], const uint8_t a[KYBER_POLYVECCOMPRESSEDBYTES]);

#define polyvec_tobytes KYBER_NAMESPACE(polyvec_tobytes)
void polyvec_tobytes(uint8_t r[KYBER_POLYVECBYTES], int16_t a[KYBER_K][KYBER_N]);
#define polyvec_frombytes KYBER_NAMESPACE(polyvec_frombytes)
void polyvec_frombytes(int16_t r[KYBER_K][KYBER_N], const uint8_t a[KYBER_POLYVECBYTES]);

// Add NEON

#define neon_polyvec_ntt KYBER_NAMESPACE(polyvec_ntt)
void neon_polyvec_ntt(int16_t r[KYBER_K][KYBER_N]);

#define neon_polyvec_invntt_to_mont KYBER_NAMESPACE(polyvec_invntt_to_mont)
void neon_polyvec_invntt_to_mont(int16_t r[KYBER_K][KYBER_N]);

#define neon_polyvec_add_reduce KYBER_NAMESPACE(polyvec_add_reduce)
void neon_polyvec_add_reduce(int16_t c[KYBER_K][KYBER_N], int16_t a[KYBER_K][KYBER_N]);


#endif
