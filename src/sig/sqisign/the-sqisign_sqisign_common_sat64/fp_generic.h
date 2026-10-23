// SPDX-License-Identifier: Apache-2.0
// Generic GF(p) support layer for p = FPG_C * 2^FPG_T - 1 over saturated
// 64-bit limbs, backed by the asm kernels fp_add/fp_sub/fp_mul/fp_sqr.
// Values follow the asm convention: Montgomery form (R = 2^(64*FPG_N)),
// partially reduced to [0, 2^FPG_BITS).

#ifndef FP_GENERIC_H
#define FP_GENERIC_H

#include <sqisign_namespace.h>
#include <stdint.h>
#include <stddef.h>
#include <fp_generic_params.h>

typedef struct
{
    uint64_t v[FPG_N];
} fpg_t;

extern const fpg_t fpg_ZERO;
extern const fpg_t fpg_ONE;

// implemented in fp_asm.S
void fp_add(fpg_t *out, const fpg_t *a, const fpg_t *b);
void fp_sub(fpg_t *out, const fpg_t *a, const fpg_t *b);
void fp_mul(fpg_t *out, const fpg_t *a, const fpg_t *b);
void fp_sqr(fpg_t *out, const fpg_t *a);

// implemented in fp_generic.c
void fp_neg(fpg_t *d, const fpg_t *a);
void fp_half(fpg_t *d, const fpg_t *a);
void fp_div3(fpg_t *d, const fpg_t *a);
void fp_mul_small(fpg_t *d, const fpg_t *a, uint32_t x);
void fp_set_small(fpg_t *d, uint32_t x);
void fp_select(fpg_t *d, const fpg_t *a0, const fpg_t *a1, uint32_t ctl);
void fp_cswap(fpg_t *a, fpg_t *b, uint32_t ctl);
void fp_copy(fpg_t *d, const fpg_t *a);
void fp_set_zero(fpg_t *a);
void fp_set_one(fpg_t *a);
uint32_t fp_is_zero(const fpg_t *a);
uint32_t fp_is_equal(const fpg_t *a, const fpg_t *b);
void fp_encode(void *dst, const fpg_t *a);
uint32_t fp_decode(fpg_t *d, const void *src);
void fp_decode_reduce(fpg_t *d, const void *src, size_t len);
void fp_exp3div4(fpg_t *a);
void fp_inv(fpg_t *a);
void fp_sqrt(fpg_t *a);
uint32_t fp_is_square(const fpg_t *a);

#endif
