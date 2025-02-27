
// SPDX-License-Identifier: Apache-2.0

#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include <stdint.h>
#include <mayo.h>
#include <stdint.h>
#include <stddef.h>

#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#ifndef TARGET_BIG_ENDIAN
#define TARGET_BIG_ENDIAN
#endif
#endif

#define uint32_t_blocker MAYO_NAMESPACE(uint32_t_blocker)
extern volatile uint32_t uint32_t_blocker;
#define uint64_t_blocker MAYO_NAMESPACE(uint64_t_blocker)
extern volatile uint64_t uint64_t_blocker;
#define unsigned_char_blocker MAYO_NAMESPACE(unsigned_char_blocker)
extern volatile unsigned char unsigned_char_blocker;

#if !(((!defined(__clang__) && defined(__GNUC__) && __GNUC__ <= 12)) && (defined(__x86_64__) || defined(_M_X64)))
// a > b -> b - a is negative
// returns 0xFFFFFFFF if true, 0x00000000 if false
static inline uint32_t ct_is_greater_than(int a, int b) {
    int32_t diff = b - a;
    return ((uint32_t) (diff >> (8*sizeof(uint32_t)-1)) ^ uint32_t_blocker);
}

// a > b -> b - a is negative
// returns 0xFFFFFFFF if true, 0x00000000 if false
static inline uint64_t ct_64_is_greater_than(int a, int b) {
    int64_t diff = ((int64_t) b) - ((int64_t) a);
    return ((uint64_t) (diff >> (8*sizeof(uint64_t)-1)) ^ uint64_t_blocker);
}

// if a == b -> 0x00000000, else 0xFFFFFFFF
static inline uint32_t ct_compare_32(int a, int b) {
    return ((uint32_t)((-(int32_t)(a ^ b)) >> (8*sizeof(uint32_t)-1)) ^ uint32_t_blocker);
}

// if a == b -> 0x0000000000000000, else 0xFFFFFFFFFFFFFFFF
static inline uint64_t ct_compare_64(int a, int b) {
    return ((uint64_t)((-(int64_t)(a ^ b)) >> (8*sizeof(uint64_t)-1)) ^ uint64_t_blocker);
}

// if a == b -> 0x00, else 0xFF
static inline unsigned char ct_compare_8(unsigned char a, unsigned char b) {
    return ((int8_t)((-(int32_t)(a ^ b)) >> (8*sizeof(uint32_t)-1)) ^ unsigned_char_blocker);
}
#else
// a > b -> b - a is negative
// returns 0xFFFFFFFF if true, 0x00000000 if false
static inline uint32_t ct_is_greater_than(int a, int b) {
    int32_t diff = b - a;
    return ((uint32_t) (diff >> (8*sizeof(uint32_t)-1)));
}

// a > b -> b - a is negative
// returns 0xFFFFFFFF if true, 0x00000000 if false
static inline uint64_t ct_64_is_greater_than(int a, int b) {
    int64_t diff = ((int64_t) b) - ((int64_t) a);
    return ((uint64_t) (diff >> (8*sizeof(uint64_t)-1)));
}

// if a == b -> 0x00000000, else 0xFFFFFFFF
static inline uint32_t ct_compare_32(int a, int b) {
    return ((uint32_t)((-(int32_t)(a ^ b)) >> (8*sizeof(uint32_t)-1)));
}

// if a == b -> 0x0000000000000000, else 0xFFFFFFFFFFFFFFFF
static inline uint64_t ct_compare_64(int a, int b) {
    return ((uint64_t)((-(int64_t)(a ^ b)) >> (8*sizeof(uint64_t)-1)));
}

// if a == b -> 0x00, else 0xFF
static inline unsigned char ct_compare_8(unsigned char a, unsigned char b) {
    return ((int8_t)((-(int32_t)(a ^ b)) >> (8*sizeof(uint32_t)-1)));
}
#endif

#if defined(MAYO_AVX) || defined(MAYO_NEON)
    #include <shuffle_arithmetic.h>
#elif defined(MAYO_M4)
    #include <m4_arithmetic.h>
#else
    #include <generic_arithmetic.h>
#endif

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

// Calculate Upper in KeyGen
#define m_upper MAYO_NAMESPACE(m_upper)
void m_upper(const mayo_params_t* p, const uint64_t *in, uint64_t *out, int size);

// Sample solution in Sign
#define sample_solution MAYO_NAMESPACE(sample_solution)
int sample_solution(const mayo_params_t *p, unsigned char *A, const unsigned char *y, const unsigned char *r, unsigned char *x, int k, int o, int m, int A_cols);

#if defined(__GNUC__) || defined(__clang__)
#define BSWAP32(i) __builtin_bswap32((i))
#define BSWAP64(i) __builtin_bswap64((i))
#else
#define BSWAP32(i) ((((i) >> 24) & 0xff) | (((i) >> 8) & 0xff00) | (((i) & 0xff00) << 8) | ((i) << 24))
#define BSWAP64(i) ((BSWAP32((i) >> 32) & 0xffffffff) | (BSWAP32(i) << 32))
#endif

#endif

