// SPDX-License-Identifier: Apache-2.0

#ifndef ARITHMETIC_96_H
#define ARITHMETIC_96_H

#include <stdint.h>
#include <mayo.h>
#include <arithmetic_64.h>

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
    uint64_t mask_msb = 0x8888888888888888ULL;
    for(int i=0; i < 6;i++){
        uint64_t t = in[i] & mask_msb;
        acc[i] ^= ((in[i] ^ t) << 1) ^ ((t >> 3) * 3);
    }
}

inline
static void m_vec_mul_add_x_inv_96(const uint64_t *in, uint64_t *acc) {
    uint64_t mask_lsb = 0x1111111111111111ULL;
    for(int i=0; i < 6;i++){
        uint64_t t = in[i] & mask_lsb;
        acc[i] ^= ((in[i] ^ t) >> 1) ^ (t * 9);
    }
}

static 
inline void vec_mul_add_96(const uint64_t *in, unsigned char a, uint64_t *acc) {
    uint32_t tab = mul_table(a);

    uint64_t lsb_ask = 0x1111111111111111ULL;

    for(int i=0; i < 6;i++){
        acc[i] ^= ( in[i]       & lsb_ask) * (tab & 0xff)
                ^ ((in[i] >> 1) & lsb_ask) * ((tab >> 8)  & 0xf)
                ^ ((in[i] >> 2) & lsb_ask) * ((tab >> 16) & 0xf)
                ^ ((in[i] >> 3) & lsb_ask) * ((tab >> 24) & 0xf);
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

