// SPDX-License-Identifier: Apache-2.0

#ifndef ARITHMETIC_64_H
#define ARITHMETIC_64_H

#include <stdint.h>
#include <mayo.h>

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

static inline uint64_t gf16v_mul_u64( uint64_t a, uint8_t b ) {
    uint64_t mask_msb = 0x8888888888888888ULL;
    uint64_t a_msb;
    uint64_t a64 = a;
    uint64_t b32 = b;
    uint64_t r64 = a64 * (b32 & 1);

    a_msb = a64 & mask_msb; // MSB, 3rd bits
    a64 ^= a_msb;   // clear MSB
    a64 = (a64 << 1) ^ ((a_msb >> 3) * 3);
    r64 ^= (a64) * ((b32 >> 1) & 1);

    a_msb = a64 & mask_msb; // MSB, 3rd bits
    a64 ^= a_msb;   // clear MSB
    a64 = (a64 << 1) ^ ((a_msb >> 3) * 3);
    r64 ^= (a64) * ((b32 >> 2) & 1);

    a_msb = a64 & mask_msb; // MSB, 3rd bits
    a64 ^= a_msb;   // clear MSB
    a64 = (a64 << 1) ^ ((a_msb >> 3) * 3);
    r64 ^= (a64) * ((b32 >> 3) & 1);

    return r64;
}

static inline uint32_t mul_table(uint8_t b){
    uint32_t x = ((uint32_t) b) * 0x08040201;

    uint32_t high_nibble_mask = 0xf0f0f0f0;

    uint32_t high_half = x & high_nibble_mask;
    return (x ^ (high_half >> 4) ^ (high_half >> 3));
}

static 
inline void vec_mul_add_64(const uint64_t *in, unsigned char a, uint64_t *acc) {
    uint32_t tab = mul_table(a);

    uint64_t lsb_ask = 0x1111111111111111ULL;

    for(int i=0; i < 4;i++){
        acc[i] ^= ( in[i]       & lsb_ask) * (tab & 0xff)
                ^ ((in[i] >> 1) & lsb_ask) * ((tab >> 8)  & 0xf)
                ^ ((in[i] >> 2) & lsb_ask) * ((tab >> 16) & 0xf)
                ^ ((in[i] >> 3) & lsb_ask) * ((tab >> 24) & 0xf);
    }
}

static 
inline void vec_mul_add_u64(const int legs, const uint64_t *in, unsigned char a, uint64_t *acc) {
    uint32_t tab = mul_table(a);

    uint64_t lsb_ask = 0x1111111111111111ULL;

    for(int i=0; i < legs; i++){
        acc[i] ^= ( in[i]       & lsb_ask) * (tab & 0xff)
                ^ ((in[i] >> 1) & lsb_ask) * ((tab >> 8)  & 0xf)
                ^ ((in[i] >> 2) & lsb_ask) * ((tab >> 16) & 0xf)
                ^ ((in[i] >> 3) & lsb_ask) * ((tab >> 24) & 0xf);
    }
}

inline
static void m_vec_mul_add_x_64(const uint64_t *in, uint64_t *acc) {
    uint64_t mask_msb = 0x8888888888888888ULL;
    for(int i=0; i < 4;i++){
        uint64_t t = in[i] & mask_msb;
        acc[i] ^= ((in[i] ^ t) << 1) ^ ((t >> 3) * 3);
    }
}

inline
static void m_vec_mul_add_x_inv_64(const uint64_t *in, uint64_t *acc) {
    uint64_t mask_lsb = 0x1111111111111111ULL;
    for(int i=0; i < 4;i++){
        uint64_t t = in[i] & mask_lsb;
        acc[i] ^= ((in[i] ^ t) >> 1) ^ (t * 9);
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

