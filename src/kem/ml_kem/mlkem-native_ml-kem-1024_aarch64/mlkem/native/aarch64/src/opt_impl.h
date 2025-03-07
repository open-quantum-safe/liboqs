/*
 * Copyright (c) 2024-2025 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MLK_NATIVE_AARCH64_SRC_OPT_IMPL_H
#define MLK_NATIVE_AARCH64_SRC_OPT_IMPL_H
#ifdef MLK_ARITH_PROFILE_IMPL_H
#error Only one MLKEM_ARITH assembly profile can be defined -- did you include multiple profiles?
#else
#define MLK_ARITH_PROFILE_IMPL_H

#include "../../../params.h"
#include "arith_native_aarch64.h"

/* Set of primitives that this backend replaces */
#define MLK_USE_NATIVE_NTT
#define MLK_USE_NATIVE_INTT
#define MLK_USE_NATIVE_POLY_REDUCE
#define MLK_USE_NATIVE_POLY_TOMONT
#define MLK_USE_NATIVE_POLY_MULCACHE_COMPUTE
#define MLK_USE_NATIVE_POLYVEC_BASEMUL_ACC_MONTGOMERY_CACHED
#define MLK_USE_NATIVE_POLY_TOBYTES
#define MLK_USE_NATIVE_REJ_UNIFORM

static MLK_INLINE void mlk_ntt_native(int16_t data[MLKEM_N])
{
  mlk_ntt_asm(data, mlk_aarch64_ntt_zetas_layer12345,
              mlk_aarch64_ntt_zetas_layer67);
}

static MLK_INLINE void mlk_intt_native(int16_t data[MLKEM_N])
{
  mlk_intt_asm(data, mlk_aarch64_invntt_zetas_layer12345,
               mlk_aarch64_invntt_zetas_layer67);
}

static MLK_INLINE void mlk_poly_reduce_native(int16_t data[MLKEM_N])
{
  mlk_poly_reduce_asm(data);
}

static MLK_INLINE void mlk_poly_tomont_native(int16_t data[MLKEM_N])
{
  mlk_poly_tomont_asm(data);
}

static MLK_INLINE void mlk_poly_mulcache_compute_native(
    int16_t x[MLKEM_N / 2], const int16_t y[MLKEM_N])
{
  mlk_poly_mulcache_compute_asm(x, y, mlk_aarch64_zetas_mulcache_native,
                                mlk_aarch64_zetas_mulcache_twisted_native);
}

static MLK_INLINE void mlk_polyvec_basemul_acc_montgomery_cached_k2_native(
    int16_t r[MLKEM_N], const int16_t a[2 * MLKEM_N],
    const int16_t b[2 * MLKEM_N], const int16_t b_cache[2 * (MLKEM_N / 2)])
{
  mlk_polyvec_basemul_acc_montgomery_cached_asm_k2(r, a, b, b_cache);
}

static MLK_INLINE void mlk_polyvec_basemul_acc_montgomery_cached_k3_native(
    int16_t r[MLKEM_N], const int16_t a[3 * MLKEM_N],
    const int16_t b[3 * MLKEM_N], const int16_t b_cache[3 * (MLKEM_N / 2)])
{
  mlk_polyvec_basemul_acc_montgomery_cached_asm_k3(r, a, b, b_cache);
}

static MLK_INLINE void mlk_polyvec_basemul_acc_montgomery_cached_k4_native(
    int16_t r[MLKEM_N], const int16_t a[4 * MLKEM_N],
    const int16_t b[4 * MLKEM_N], const int16_t b_cache[4 * (MLKEM_N / 2)])
{
  mlk_polyvec_basemul_acc_montgomery_cached_asm_k4(r, a, b, b_cache);
}

static MLK_INLINE void mlk_poly_tobytes_native(uint8_t r[MLKEM_POLYBYTES],
                                               const int16_t a[MLKEM_N])
{
  mlk_poly_tobytes_asm(r, a);
}

static MLK_INLINE int mlk_rej_uniform_native(int16_t *r, unsigned len,
                                             const uint8_t *buf,
                                             unsigned buflen)
{
  if (len != MLKEM_N || buflen % 24 != 0)
  {
    return -1;
  }
  return (int)mlk_rej_uniform_asm(r, buf, buflen, mlk_rej_uniform_table);
}

#endif /* MLK_ARITH_PROFILE_IMPL_H */

#endif /* MLK_NATIVE_AARCH64_SRC_OPT_IMPL_H */
