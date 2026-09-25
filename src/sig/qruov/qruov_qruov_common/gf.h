#ifndef GF_H
#define GF_H

#include <stdint.h>
#include "qruov.h"

static inline uint8_t gf_reduce(uint32_t x)
{
    return x % QRUOV_q;
}

static inline uint8_t gf_add(uint8_t a, uint8_t b)
{
    return gf_reduce(a + b);
}

static inline uint8_t gf_sub(uint8_t a, uint8_t b)
{
    return gf_reduce(a + QRUOV_q - b);
}

static inline uint8_t gf_mul(uint8_t a, uint8_t b)
{
    return gf_reduce((uint32_t)a * b);
}

#if QRUOV_q == 7
static const uint8_t gf_inv_table[256] = {
    0, 1, 4, 5, 2, 3, 6,
};
#elif QRUOV_q == 31
static const uint8_t gf_inv_table[256] = {
    0, 1, 16, 21, 8, 25, 26, 9, 4, 7, 28, 17, 13, 12, 20, 29,
    2, 11, 19, 18, 14, 3, 24, 27, 22, 5, 6, 23, 10, 15, 30,
};
#elif QRUOV_q == 127
static const uint8_t gf_inv_table[256] = {
    0, 1, 64, 85, 32, 51, 106, 109, 16, 113, 89, 104, 53, 88, 118, 17,
    8, 15, 120, 107, 108, 121, 52, 116, 90, 61, 44, 80, 59, 92, 72, 41,
    4, 77, 71, 98, 60, 103, 117, 114, 54, 31, 124, 65, 26, 48, 58, 100,
    45, 70, 94, 5, 22, 12, 40, 97, 93, 78, 46, 28, 36, 25, 84, 125,
    2, 43, 102, 91, 99, 81, 49, 34, 30, 87, 115, 105, 122, 33, 57, 82,
    27, 69, 79, 101, 62, 3, 96, 73, 13, 10, 24, 67, 29, 56, 50, 123,
    86, 55, 35, 68, 47, 83, 66, 37, 11, 75, 6, 19, 20, 7, 112, 119,
    110, 9, 39, 74, 23, 38, 14, 111, 18, 21, 76, 95, 42, 63, 126,
};
#else
#error "Unsupported QRUOV_q"
#endif

static inline uint8_t gf_inv(uint8_t a)
{
    return gf_inv_table[a];
}

#endif // GF_H
