#ifndef FP_H
#define FP_H

// Include statements
#include <sqisign_namespace.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <tutil.h>
#include <fp_constants.h>

#include "gf65376.h"

// Type for elements of GF(p)
#define fp_t gf65376

// Operations in fp
static inline void
fp_neg(fp_t *d, const fp_t *a)
{
    gf65376_neg(d, a);
}

void fp_add(fp_t *out, const fp_t *a, const fp_t *b); // implemented in fp_asm.S
void fp_sub(fp_t *out, const fp_t *a, const fp_t *b); // implemented in fp_asm.S
void fp_sqr(fp_t *out, const fp_t *a);                // implemented in fp_asm.S
void fp_mul(fp_t *out, const fp_t *a, const fp_t *b); // implemented in fp_asm.S

static inline void
fp_mul_small(fp_t *d, const fp_t *a, uint32_t n)
{
    gf65376_mul_small(d, a, n);
}

static inline void
fp_half(fp_t *d, const fp_t *a)
{
    gf65376_half(d, a);
}
// #define fp_half gf65376_half

static inline void
fp_div3(fp_t *d, const fp_t *a)
{
    gf65376_div3(d, a);
}
// #define fp_div3 gf65376_div3

// Constant time selection and swapping
static inline void
fp_select(fp_t *d, const fp_t *a0, const fp_t *a1, uint32_t ctl)
{
    gf65376_select(d, a0, a1, ctl);
}
// #define fp_select gf65376_select
static inline void
fp_cswap(fp_t *a, fp_t *b, uint32_t ctl)
{
    gf65376_cswap(a, b, ctl);
}
// #define fp_cswap gf65376_cswap

// Comparisons for fp elements
static inline uint32_t
fp_is_zero(const fp_t *a)
{
    return gf65376_iszero(a);
}
// #define fp_is_zero gf65376_iszero

static inline uint32_t
fp_is_equal(const fp_t *a, const fp_t *b)
{
    return gf65376_equals(a, b);
}
// #define fp_is_equal gf65376_equals

// Set a uint32 to an Fp value
static inline void
fp_set_small(fp_t *d, uint32_t x)
{
    gf65376_set_small(d, x);
}
// #define fp_set_small gf65376_set_small

// Encoding and decoding of bytes
static inline void
fp_encode(void *dst, const fp_t *a)
{
    gf65376_encode(dst, a);
}
// #define fp_encode gf65376_encode
static inline uint32_t
fp_decode(fp_t *d, const void *src)
{
    return gf65376_decode(d, src);
}
// #define fp_decode gf65376_decode
static inline void
fp_decode_reduce(fp_t *d, const void *src, size_t len)
{
    gf65376_decode_reduce(d, src, len);
}
// #define fp_decode_reduce gf65376_decode_reduce

// These functions are essentially useless because we can just
// use = for the shallow copies we need, but they're here for
// now until we do a larger refactoring
static inline void
fp_copy(fp_t *out, const fp_t *a)
{
    memcpy(out, a, sizeof(fp_t));
}

static inline void
fp_set_zero(fp_t *a)
{
    memcpy(a, &ZERO, sizeof(fp_t));
}

static inline void
fp_set_one(fp_t *a)
{
    memcpy(a, &ONE, sizeof(fp_t));
}

// Functions defined in low level code but with different API
void fp_inv(fp_t *a);
void fp_sqrt(fp_t *a);
void fp_exp3div4(fp_t *a);
uint32_t fp_is_square(const fp_t *a);

#endif
