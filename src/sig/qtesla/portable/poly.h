#ifndef POLY_H
#define POLY_H

#include "params.h"
#include "config.h"
#include <stdint.h>

typedef int32_t poly[PARAM_N];
typedef int32_t poly_k[PARAM_N * PARAM_K];

static int32_t reduce(int64_t a);
static sdigit_t barr_reduce(sdigit_t a);
static int64_t barr_reduce64(int64_t a);
static void ntt(poly a, const poly w);
static void nttinv(poly a, const poly w);
static void poly_ntt(poly x_ntt, const poly x);
static void poly_mul(poly result, const poly x, const poly y);
static void poly_add(poly result, const poly x, const poly y);
static void poly_add_correct(poly result, const poly x, const poly y);
static void poly_sub(poly result, const poly x, const poly y);
static void poly_sub_reduce(poly result, const poly x, const poly y);
static void sparse_mul8(poly prod, const unsigned char *s, const uint32_t pos_list[PARAM_H], const int16_t sign_list[PARAM_H]);
static void sparse_mul32(poly prod, const int32_t *pk, const uint32_t pos_list[PARAM_H], const int16_t sign_list[PARAM_H]);
static void poly_uniform(poly_k a, const unsigned char *seed);

#endif
