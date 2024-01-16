// SPDX-License-Identifier: Apache-2.0

#ifndef MEM_H
#define MEM_H
#include <stddef.h>
#include <stdint.h>

#if defined(__GNUC__) || defined(__clang__)
#define BSWAP32(i) __builtin_bswap32((i))
#define BSWAP64(i) __builtin_bswap64((i))
#else
#define BSWAP32(i) ((((i) >> 24) & 0xff) | (((i) >> 8) & 0xff00) | (((i) & 0xff00) << 8) | ((i) << 24))
#define BSWAP64(i) ((BSWAP32((i) >> 32) & 0xffffffff) | (BSWAP32(i) << 32))
#endif

// a > b -> b - a is negative
// returns 0xFFFFFFFF if true, 0x00000000 if false
static inline uint32_t ct_is_greater_than(int a, int b) {
    int32_t diff = b - a;
    return (uint32_t) (diff >> (8*sizeof(uint32_t)-1));
}

// a > b -> b - a is negative
// returns 0xFFFFFFFF if true, 0x00000000 if false
static inline uint64_t ct_64_is_greater_than(int a, int b) {
    int64_t diff = ((int64_t) b) - ((int64_t) a);
    return (uint64_t) (diff >> (8*sizeof(uint64_t)-1));
}

// if a == b -> 0x00000000, else 0xFFFFFFFF
static inline uint32_t ct_compare_32(int a, int b) {
    return (uint32_t)((-(int32_t)(a ^ b)) >> (8*sizeof(uint32_t)-1));
}

// if a == b -> 0x0000000000000000, else 0xFFFFFFFFFFFFFFFF
static inline uint64_t ct_compare_64(int a, int b) {
    return (uint64_t)((-(int64_t)(a ^ b)) >> (8*sizeof(uint64_t)-1));
}

// if a == b -> 0x00, else 0xFF
static inline unsigned char ct_compare_8(unsigned char a, unsigned char b) {
    return (int8_t)((-(int32_t)(a ^ b)) >> (8*sizeof(uint32_t)-1));
}

#include <oqs/common.h>
/**
 * Clears and frees allocated memory.
 * 
 * @param[out] mem Memory to be cleared and freed.
 * @param size Size of memory to be cleared and freed.
 */
static inline void mayo_secure_free(void *mem, size_t size) {
    OQS_MEM_secure_free(mem, size);
}

/**
 * Clears memory.
 * 
 * @param[out] mem Memory to be cleared.
 * @param size Size of memory to be cleared.
 */
static inline void mayo_secure_clear(void *mem, size_t size) {
    OQS_MEM_cleanse(mem, size);
}

#endif
