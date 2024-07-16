// SPDX-License-Identifier: Apache-2.0

#ifndef ARITHMETIC_96_H
#define ARITHMETIC_96_H

#include <stdint.h>
#include <mayo.h>
#include <arithmetic_common.h>

// This implements arithmetic for vectors of 96 field elements in Z_2[x]/(x^4+x+1)

static
inline void vec_copy_96(const uint64_t *in, uint64_t *out) {
    out[0] = in[0];
    out[1] = in[1];
    out[2] = in[2];
    out[3] = in[3];
    out[4] = in[4];
    out[5] = in[5];
}

static
inline void vec_add_96(const uint64_t *in, uint64_t *acc) {
    acc[0] ^= in[0];
    acc[1] ^= in[1];
    acc[2] ^= in[2];
    acc[3] ^= in[3];
    acc[4] ^= in[4];
    acc[5] ^= in[5];
}

inline
static void m_vec_mul_add_x_96(const uint64_t *in, uint64_t *acc) {
    for(int i=0;i<6;i++){
        acc[i] ^= gf16v_mul_u64(in[i], 0x2);
    }
}

inline
static void m_vec_mul_add_x_inv_96(const uint64_t *in, uint64_t *acc) {
    for(int i=0;i<6;i++){
        acc[i] ^= gf16v_mul_u64(in[i], 0x9);
    }
}

static 
inline void vec_mul_add_96(const uint64_t *in, unsigned char a, uint64_t *acc) {
    for(int i=0; i < 6;i++){
        acc[i] ^= gf16v_mul_u64(in[i], a);        
    }
}

static 
inline void multiply_bins_96(uint64_t *bins, uint64_t *out) {

    m_vec_mul_add_x_inv_96(bins +  5 * 6, bins +  10 * 6);
    m_vec_mul_add_x_96(bins + 11 * 6, bins + 12 * 6);
    m_vec_mul_add_x_inv_96(bins +  10 * 6, bins +  7 * 6);
    m_vec_mul_add_x_96(bins + 12 * 6, bins +  6 * 6);
    m_vec_mul_add_x_inv_96(bins +  7 * 6, bins +  14 * 6);
    m_vec_mul_add_x_96(bins +  6 * 6, bins +  3 * 6);
    m_vec_mul_add_x_inv_96(bins +  14 * 6, bins +  15 * 6);
    m_vec_mul_add_x_96(bins +  3 * 6, bins +  8 * 6);
    m_vec_mul_add_x_inv_96(bins +  15 * 6, bins +  13 * 6);
    m_vec_mul_add_x_96(bins +  8 * 6, bins +  4 * 6);
    m_vec_mul_add_x_inv_96(bins +  13 * 6, bins +  9 * 6);
    m_vec_mul_add_x_96(bins +  4 * 6, bins +  2 * 6);
    m_vec_mul_add_x_inv_96(bins +   9 * 6, bins +  1 * 6);
    m_vec_mul_add_x_96(bins +  2 * 6, bins +  1 * 6);
    vec_copy_96(bins + 6, out);
}

#endif

