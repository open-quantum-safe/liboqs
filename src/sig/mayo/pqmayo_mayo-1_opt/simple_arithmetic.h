// SPDX-License-Identifier: Apache-2.0

#ifndef SIMPLE_ARITHMETIC_H
#define SIMPLE_ARITHMETIC_H

// GF(16) multiplication mod x^4 + x + 1
static inline unsigned char mul_f(unsigned char a, unsigned char b) {
    // carryless multiply
    unsigned char p;
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

static
inline void m_vec_copy(int m_legs, const uint64_t *in,
                                 uint64_t *out) {
    for (int i = 0; i < m_legs * 2; i++) {
        out[i] = in[i];
    }
}

static
inline void m_vec_add(int m_legs, const uint64_t *in,
                                uint64_t *acc) {
    for (int i = 0; i < m_legs * 2; i++) {
        acc[i] ^= in[i];
    }
}

// This implements arithmetic for nibble-packed vectors of m field elements in Z_2[x]/(x^4+x+1)
// gf16 := gf2[x]/(x^4+x+1)

static inline uint32_t gf16v_mul_u32(uint32_t a, uint8_t b) {
    uint32_t a_msb;
    uint32_t a32 = a;
    uint32_t b32 = b;
    uint32_t r32 = a32 * (b32 & 1);

    a_msb = a32 & 0x88888888; // MSB, 3rd bits
    a32 ^= a_msb;   // clear MSB
    a32 = (a32 << 1) ^ ((a_msb >> 3) * 3);
    r32 ^= (a32) * ((b32 >> 1) & 1);

    a_msb = a32 & 0x88888888; // MSB, 3rd bits
    a32 ^= a_msb;   // clear MSB
    a32 = (a32 << 1) ^ ((a_msb >> 3) * 3);
    r32 ^= (a32) * ((b32 >> 2) & 1);

    a_msb = a32 & 0x88888888; // MSB, 3rd bits
    a32 ^= a_msb;   // clear MSB
    a32 = (a32 << 1) ^ ((a_msb >> 3) * 3);
    r32 ^= (a32) * ((b32 >> 3) & 1);

    return r32;

}
 
static inline void m_vec_mul_add(int m_legs, const uint64_t *in, unsigned char a, uint64_t *acc) {
    for(int i=0; i < m_legs*2;i++){
        acc[i] ^= gf16v_mul_u64(in[i], a);        
    }
}

static inline void m_vec_mul_add_x(int m_legs, const uint64_t *in, uint64_t *acc) {
    for(int i=0;i<m_legs*2;i++){
        acc[i] ^= gf16v_mul_u64(in[i], 0x2);
    }
}

#endif

