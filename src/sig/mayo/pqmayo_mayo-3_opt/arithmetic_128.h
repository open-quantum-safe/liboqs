// SPDX-License-Identifier: Apache-2.0

#ifndef ARITHMETIC_128_H
#define ARITHMETIC_128_H

#include <stdint.h>
#include <mayo.h>
#include <arithmetic_64.h>

// This implements arithmetic for vectors of 128 field elements in Z_2[x]/(x^4+x+1)

static
inline void vec_copy_128(const uint64_t *in, uint64_t *out) {
    out[0] = in[0];
    out[1] = in[1];
    out[2] = in[2];
    out[3] = in[3];
    out[4] = in[4];
    out[5] = in[5];
    out[6] = in[6];
    out[7] = in[7];
}


static
inline void vec_add_128(const uint64_t *in, uint64_t *acc) {
    acc[0] ^= in[0];
    acc[1] ^= in[1];
    acc[2] ^= in[2];
    acc[3] ^= in[3];
    acc[4] ^= in[4];
    acc[5] ^= in[5];
    acc[6] ^= in[6];
    acc[7] ^= in[7];
}

inline
static void m_vec_mul_add_x_128(const uint64_t *in, uint64_t *acc) {
    uint64_t mask_msb = 0x8888888888888888ULL;
    for(int i=0; i < 8;i++){
        uint64_t t = in[i] & mask_msb;
        acc[i] ^= ((in[i] ^ t) << 1) ^ ((t >> 3) * 3);
    }
}

inline
static void m_vec_mul_add_x_inv_128(const uint64_t *in, uint64_t *acc) {
    uint64_t mask_lsb = 0x1111111111111111ULL;
    for(int i=0; i < 8;i++){
        uint64_t t = in[i] & mask_lsb;
        acc[i] ^= ((in[i] ^ t) >> 1) ^ (t * 9);
    }
}

static 
inline void vec_mul_add_128(const uint64_t *in, unsigned char a, uint64_t *acc) {
    uint32_t tab = mul_table(a);

    uint64_t lsb_ask = 0x1111111111111111ULL;

    for(int i=0; i < 8;i++){
        acc[i] ^= ( in[i]       & lsb_ask) * (tab & 0xff)
                ^ ((in[i] >> 1) & lsb_ask) * ((tab >> 8)  & 0xf)
                ^ ((in[i] >> 2) & lsb_ask) * ((tab >> 16) & 0xf)
                ^ ((in[i] >> 3) & lsb_ask) * ((tab >> 24) & 0xf);
    }
}

static 
inline void multiply_bins_128(uint64_t *bins, uint64_t *out) {

    m_vec_mul_add_x_inv_128(bins +  5 * 8, bins +  10 * 8);
    m_vec_mul_add_x_128(bins + 11 * 8, bins + 12 * 8);
    m_vec_mul_add_x_inv_128(bins +  10 * 8, bins +  7 * 8);
    m_vec_mul_add_x_128(bins + 12 * 8, bins +  6 * 8);
    m_vec_mul_add_x_inv_128(bins +  7 * 8, bins +  14 * 8);
    m_vec_mul_add_x_128(bins +  6 * 8, bins +  3 * 8);
    m_vec_mul_add_x_inv_128(bins +  14 * 8, bins +  15 * 8);
    m_vec_mul_add_x_128(bins +  3 * 8, bins +  8 * 8);
    m_vec_mul_add_x_inv_128(bins +  15 * 8, bins +  13 * 8);
    m_vec_mul_add_x_128(bins +  8 * 8, bins +  4 * 8);
    m_vec_mul_add_x_inv_128(bins +  13 * 8, bins +  9 * 8);
    m_vec_mul_add_x_128(bins +  4 * 8, bins +  2 * 8);
    m_vec_mul_add_x_inv_128(bins +   9 * 8, bins +  1 * 8);
    m_vec_mul_add_x_128(bins +  2 * 8, bins +  1 * 8);
    vec_copy_128(bins + 8, out);
}

#endif

