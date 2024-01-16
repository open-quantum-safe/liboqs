// SPDX-License-Identifier: Apache-2.0

#ifndef ARITHMETIC_64_H
#define ARITHMETIC_64_H

#include <stdint.h>
#include <mayo.h>
#include <arithmetic_common.h>

// This implements arithmetic for vectors of 64 field elements in Z_2[x]/(x^4+x+1)

static
inline void vec_copy_64(const uint64_t *in, uint64_t *out) {
    out[0] = in[0];
    out[1] = in[1];
    out[2] = in[2];
    out[3] = in[3];
}

static
inline void vec_add_64(const uint64_t *in, uint64_t *acc) {
    acc[0] ^= in[0];
    acc[1] ^= in[1];
    acc[2] ^= in[2];
    acc[3] ^= in[3];
}

static 
inline void vec_mul_add_64(const uint64_t *in, unsigned char a, uint64_t *acc) {
    for(int i=0; i < 4;i++){
        acc[i] ^= gf16v_mul_u64(in[i], a);        
    }
}

inline
static void m_vec_mul_add_x_64(const uint64_t *in, uint64_t *acc) {
    for(int i=0;i<4;i++){
        acc[i] ^= gf16v_mul_u64(in[i], 0x2);
    }
}
inline
static void m_vec_mul_add_x_inv_64(const uint64_t *in, uint64_t *acc) {
    for(int i=0;i<4;i++){
        acc[i] ^= gf16v_mul_u64(in[i], 0x9);
    }
}

static 
inline void multiply_bins_64(uint64_t *bins, uint64_t *out) {

    m_vec_mul_add_x_inv_64(bins +  5 * 4, bins +  10 * 4);
    m_vec_mul_add_x_64(bins + 11 * 4, bins + 12 * 4);
    m_vec_mul_add_x_inv_64(bins +  10 * 4, bins +  7 * 4);
    m_vec_mul_add_x_64(bins + 12 * 4, bins +  6 * 4);
    m_vec_mul_add_x_inv_64(bins +  7 * 4, bins +  14 * 4);
    m_vec_mul_add_x_64(bins +  6 * 4, bins +  3 * 4);
    m_vec_mul_add_x_inv_64(bins +  14 * 4, bins +  15 * 4);
    m_vec_mul_add_x_64(bins +  3 * 4, bins +  8 * 4);
    m_vec_mul_add_x_inv_64(bins +  15 * 4, bins +  13 * 4);
    m_vec_mul_add_x_64(bins +  8 * 4, bins +  4 * 4);
    m_vec_mul_add_x_inv_64(bins +  13 * 4, bins +  9 * 4);
    m_vec_mul_add_x_64(bins +  4 * 4, bins +  2 * 4);
    m_vec_mul_add_x_inv_64(bins +   9 * 4, bins +  1 * 4);
    m_vec_mul_add_x_64(bins +  2 * 4, bins +  1 * 4);
    vec_copy_64(bins + 4, out);
}

#endif

