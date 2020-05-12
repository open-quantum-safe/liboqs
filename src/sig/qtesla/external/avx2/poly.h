#ifndef POLY_H
#define POLY_H

#include "params.h"
#include "config.h"
#include <stdint.h>

typedef int32_t poly[PARAM_N] __attribute__((aligned(32)));
typedef int32_t poly2x[2 * PARAM_N] __attribute__((aligned(32)));
typedef int32_t poly_k[PARAM_N * PARAM_K] __attribute__((aligned(32)));

static int32_t reduce(int64_t a);
static int32_t barr_reduce(int32_t a);
static int64_t barr_reduce64(int64_t a);
static void poly_ntt(poly2x x_ntt, const poly x);
static void poly_mul(poly result, const poly x, const poly2x y);
static void poly_add(poly result, const poly x, const poly y);
static void poly_add_correct(poly result, const poly x, const poly y);
static void poly_sub(poly result, const poly x, const poly y);
static void poly_sub_reduce(poly result, const poly x, const poly y);
static void sparse_mul8(poly prod, const unsigned char *s, const uint32_t pos_list[PARAM_H], const int16_t sign_list[PARAM_H]);
static void sparse_mul32(poly prod, const int32_t *pk, const uint32_t pos_list[PARAM_H], const int16_t sign_list[PARAM_H]);
static void poly_uniform(poly_k a, const unsigned char *seed);

void poly_ntt_asm(poly2x c, const poly a, const poly w);
void poly_pmul_asm(poly2x c, const poly a, const poly2x b);
void poly_intt_asm(poly c, const poly2x a, const poly w);

#endif
