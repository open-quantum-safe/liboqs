// SPDX-License-Identifier: Apache-2.0

#ifndef SIMPLE_ARITHMETIC_H
#define SIMPLE_ARITHMETIC_H
#include <mem.h>

// GF(16) multiplication mod x^4 + x + 1
static inline unsigned char mul_f(unsigned char a, unsigned char b) {
    // carryless multiply
    unsigned char p;

#if !(((defined(__clang__) && __clang_major__ < 15) || (!defined(__clang__) && defined(__GNUC__) && __GNUC__ <= 12)) && (defined(__x86_64__) || defined(_M_X64)))
    a ^= unsigned_char_blocker;
#endif

    p  = (a & 1)*b;
    p ^= (a & 2)*b;
    p ^= (a & 4)*b;
    p ^= (a & 8)*b;

    // reduce mod x^4 + x + 1
    unsigned char top_p = p & 0xf0;
    unsigned char out = (p ^ (top_p >> 4) ^ (top_p >> 3)) & 0x0f;
    return out;
}

static inline uint64_t mul_fx8(unsigned char a, uint64_t b) {
    // carryless multiply
    uint64_t p;
    p  = (a & 1)*b;
    p ^= (a & 2)*b;
    p ^= (a & 4)*b;
    p ^= (a & 8)*b;

    // reduce mod x^4 + x + 1
    uint64_t top_p = p & 0xf0f0f0f0f0f0f0f0;
    uint64_t out = (p ^ (top_p >> 4) ^ (top_p >> 3)) & 0x0f0f0f0f0f0f0f0f;
    return out;
}

// GF(16) addition
static inline unsigned char add_f(unsigned char a, unsigned char b) {
    return a ^ b;
}

// GF(16) subtraction
static inline unsigned char sub_f(unsigned char a, unsigned char b) {
    return a ^ b;
}

// GF(16) negation
static inline unsigned char neg_f(unsigned char a) {
    return a;
}

static inline unsigned char inverse_f(unsigned char a) {
    // static unsigned char table[16] = {0, 1, 9, 14, 13, 11, 7, 6, 15, 2, 12, 5,
    // 10, 4, 3, 8}; return table[a & 15];

    unsigned char a2 = mul_f(a, a);
    unsigned char a4 = mul_f(a2, a2);
    unsigned char a8 = mul_f(a4, a4);
    unsigned char a6 = mul_f(a2, a4);
    unsigned char a14 = mul_f(a8, a6);

    return a14;
}

static inline unsigned char lincomb(const unsigned char *a,
                                    const unsigned char *b, int n, int m) {
    unsigned char ret = 0;
    for (int i = 0; i < n; ++i, b += m) {
        ret = add_f(mul_f(a[i], *b), ret);
    }
    return ret;
}

static inline void mat_mul(const unsigned char *a, const unsigned char *b,
                    unsigned char *c, int colrow_ab, int row_a, int col_b) {
    for (int i = 0; i < row_a; ++i, a += colrow_ab) {
        for (int j = 0; j < col_b; ++j, ++c) {
            *c = lincomb(a, b + j, colrow_ab, col_b);
        }
    }
}

static inline void mat_add(const unsigned char *a, const unsigned char *b,
                    unsigned char *c, int m, int n) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            *(c + i * n + j) = add_f(*(a + i * n + j), *(b + i * n + j));
        }
    }
}

static inline uint64_t gf16v_mul_u64( uint64_t a, uint8_t b ) {
    uint64_t mask_msb = 0x8888888888888888ULL;
    uint64_t a_msb;
    uint64_t a64 = a;
#if !(((defined(__clang__) && __clang_major__ < 15) || (!defined(__clang__) && defined(__GNUC__) && __GNUC__ <= 12)) && (defined(__x86_64__) || defined(_M_X64)))
    uint64_t b32 = b ^ unsigned_char_blocker;
#else
    uint64_t b32 = b;
#endif
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

// This implements arithmetic for nibble-packed vectors of m field elements in Z_2[x]/(x^4+x+1)
// gf16 := gf2[x]/(x^4+x+1)
 
static inline uint32_t mul_table(uint8_t b){
    uint32_t x = ((uint32_t) b) * 0x08040201;

    uint32_t high_nibble_mask = 0xf0f0f0f0;

    uint32_t high_half = x & high_nibble_mask;
    return (x ^ (high_half >> 4) ^ (high_half >> 3));
}

#endif

