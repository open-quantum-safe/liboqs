/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

/* ML-KEM arithmetic native profile for clean assembly */

#ifdef MLKEM_NATIVE_ARITH_PROFILE_IMPL_H
#error Only one MLKEM_ARITH assembly profile can be defined -- did you include multiple profiles?
#else
#define MLKEM_NATIVE_ARITH_PROFILE_IMPL_H

#include "arith_native_aarch64.h"

/* Set of primitives that this backend replaces */
#define MLKEM_USE_NATIVE_NTT
#define MLKEM_USE_NATIVE_INTT
#define MLKEM_USE_NATIVE_POLY_REDUCE
#define MLKEM_USE_NATIVE_POLY_TOMONT
#define MLKEM_USE_NATIVE_POLY_MULCACHE_COMPUTE
#define MLKEM_USE_NATIVE_POLYVEC_BASEMUL_ACC_MONTGOMERY_CACHED
#define MLKEM_USE_NATIVE_POLY_TOBYTES
#define MLKEM_USE_NATIVE_REJ_UNIFORM

static INLINE void ntt_native(int16_t data[MLKEM_N])
{
  ntt_asm_clean(data, aarch64_ntt_zetas_layer01234, aarch64_ntt_zetas_layer56);
}

static INLINE void intt_native(int16_t data[MLKEM_N])
{
  intt_asm_clean(data, aarch64_invntt_zetas_layer01234,
                 aarch64_invntt_zetas_layer56);
}

static INLINE void poly_reduce_native(int16_t data[MLKEM_N])
{
  poly_reduce_asm_clean(data);
}

static INLINE void poly_tomont_native(int16_t data[MLKEM_N])
{
  poly_tomont_asm_clean(data);
}

static INLINE void poly_mulcache_compute_native(int16_t x[MLKEM_N / 2],
                                                const int16_t y[MLKEM_N])
{
  poly_mulcache_compute_asm_clean(x, y, aarch64_zetas_mulcache_native,
                                  aarch64_zetas_mulcache_twisted_native);
}

static INLINE void polyvec_basemul_acc_montgomery_cached_native(
    int16_t r[MLKEM_N], const int16_t a[MLKEM_K * MLKEM_N],
    const int16_t b[MLKEM_K * MLKEM_N],
    const int16_t b_cache[MLKEM_K * (MLKEM_N / 2)])
{
  polyvec_basemul_acc_montgomery_cached_asm_clean(r, a, b, b_cache);
}

static INLINE void poly_tobytes_native(uint8_t r[MLKEM_POLYBYTES],
                                       const int16_t a[MLKEM_N])
{
  poly_tobytes_asm_clean(r, a);
}

static INLINE int rej_uniform_native(int16_t *r, unsigned len,
                                     const uint8_t *buf, unsigned buflen)
{
  if (len != MLKEM_N || buflen % 24 != 0)
  {
    return -1;
  }
  return (int)rej_uniform_asm_clean(r, buf, buflen, rej_uniform_table);
}

#endif /* MLKEM_NATIVE_ARITH_PROFILE_IMPL_H */
