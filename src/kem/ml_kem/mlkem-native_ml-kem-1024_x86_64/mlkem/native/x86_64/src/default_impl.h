/*
 * Copyright (c) 2024-2025 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MLK_NATIVE_X86_64_SRC_DEFAULT_IMPL_H
#define MLK_NATIVE_X86_64_SRC_DEFAULT_IMPL_H
/* ML-KEM arithmetic native profile for clean assembly */

#ifdef MLK_ARITH_PROFILE_IMPL_H
#error Only one MLKEM_ARITH assembly profile can be defined -- did you include multiple profiles?
#else
#define MLK_ARITH_PROFILE_IMPL_H

#include <string.h>

#include "../../../params.h"
#include "arith_native_x86_64.h"

#define MLK_USE_NATIVE_NTT_CUSTOM_ORDER

#define MLK_USE_NATIVE_REJ_UNIFORM
#define MLK_USE_NATIVE_NTT
#define MLK_USE_NATIVE_INTT
#define MLK_USE_NATIVE_POLY_REDUCE
#define MLK_USE_NATIVE_POLY_TOMONT
#define MLK_USE_NATIVE_POLYVEC_BASEMUL_ACC_MONTGOMERY_CACHED
#define MLK_USE_NATIVE_POLY_MULCACHE_COMPUTE
#define MLK_USE_NATIVE_POLY_TOBYTES
#define MLK_USE_NATIVE_POLY_FROMBYTES
#define MLK_USE_NATIVE_POLY_COMPRESS_D4
#define MLK_USE_NATIVE_POLY_COMPRESS_D5
#define MLK_USE_NATIVE_POLY_COMPRESS_D10
#define MLK_USE_NATIVE_POLY_COMPRESS_D11
#define MLK_USE_NATIVE_POLY_DECOMPRESS_D4
#define MLK_USE_NATIVE_POLY_DECOMPRESS_D5
#define MLK_USE_NATIVE_POLY_DECOMPRESS_D10
#define MLK_USE_NATIVE_POLY_DECOMPRESS_D11

static MLK_INLINE void mlk_poly_permute_bitrev_to_custom(int16_t data[MLKEM_N])
{
  mlk_nttunpack_avx2((__m256i *)(data), mlk_qdata.vec);
}

static MLK_INLINE int mlk_rej_uniform_native(int16_t *r, unsigned len,
                                             const uint8_t *buf,
                                             unsigned buflen)
{
  /* AVX2 implementation assumes specific buffer lengths */
  if (len != MLKEM_N || buflen != MLK_AVX2_REJ_UNIFORM_BUFLEN)
  {
    return -1;
  }

  return (int)mlk_rej_uniform_avx2(r, buf);
}

static MLK_INLINE void mlk_ntt_native(int16_t data[MLKEM_N])
{
  mlk_ntt_avx2((__m256i *)data, mlk_qdata.vec);
}

static MLK_INLINE void mlk_intt_native(int16_t data[MLKEM_N])
{
  mlk_invntt_avx2((__m256i *)data, mlk_qdata.vec);
}

static MLK_INLINE void mlk_poly_reduce_native(int16_t data[MLKEM_N])
{
  mlk_reduce_avx2((__m256i *)data, mlk_qdata.vec);
}

static MLK_INLINE void mlk_poly_tomont_native(int16_t data[MLKEM_N])
{
  mlk_tomont_avx2((__m256i *)data, mlk_qdata.vec);
}

static MLK_INLINE void mlk_poly_mulcache_compute_native(
    int16_t x[MLKEM_N / 2], const int16_t y[MLKEM_N])
{
  /* AVX2 backend does not use mulcache */
  ((void)y);
  ((void)x);
}

#if defined(MLK_MULTILEVEL_BUILD_WITH_SHARED) || MLKEM_K == 2
static MLK_INLINE void mlk_polyvec_basemul_acc_montgomery_cached_k2_native(
    int16_t r[MLKEM_N], const int16_t a[2 * MLKEM_N],
    const int16_t b[2 * MLKEM_N], const int16_t b_cache[2 * (MLKEM_N / 2)])
{
  mlk_polyvec_basemul_acc_montgomery_cached_avx2(2, r, a, b, b_cache);
}
#endif /* MLK_MULTILEVEL_BUILD_WITH_SHARED || MLKEM_K == 2 */

#if defined(MLK_MULTILEVEL_BUILD_WITH_SHARED) || MLKEM_K == 3
static MLK_INLINE void mlk_polyvec_basemul_acc_montgomery_cached_k3_native(
    int16_t r[MLKEM_N], const int16_t a[3 * MLKEM_N],
    const int16_t b[3 * MLKEM_N], const int16_t b_cache[3 * (MLKEM_N / 2)])
{
  mlk_polyvec_basemul_acc_montgomery_cached_avx2(3, r, a, b, b_cache);
}
#endif /* MLK_MULTILEVEL_BUILD_WITH_SHARED || MLKEM_K == 3 */

#if defined(MLK_MULTILEVEL_BUILD_WITH_SHARED) || MLKEM_K == 4
static MLK_INLINE void mlk_polyvec_basemul_acc_montgomery_cached_k4_native(
    int16_t r[MLKEM_N], const int16_t a[4 * MLKEM_N],
    const int16_t b[4 * MLKEM_N], const int16_t b_cache[4 * (MLKEM_N / 2)])
{
  mlk_polyvec_basemul_acc_montgomery_cached_avx2(4, r, a, b, b_cache);
}
#endif /* MLK_MULTILEVEL_BUILD_WITH_SHARED || MLKEM_K == 4 */

static MLK_INLINE void mlk_poly_tobytes_native(uint8_t r[MLKEM_POLYBYTES],
                                               const int16_t a[MLKEM_N])
{
  mlk_ntttobytes_avx2(r, (const __m256i *)a, mlk_qdata.vec);
}

static MLK_INLINE void mlk_poly_frombytes_native(
    int16_t r[MLKEM_N], const uint8_t a[MLKEM_POLYBYTES])
{
  mlk_nttfrombytes_avx2((__m256i *)r, a, mlk_qdata.vec);
}

#if defined(MLK_MULTILEVEL_BUILD_WITH_SHARED) || (MLKEM_K == 2 || MLKEM_K == 3)
static MLK_INLINE void mlk_poly_compress_d4_native(
    uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D4], const int16_t a[MLKEM_N])
{
  mlk_poly_compress_d4_avx2(r, (const __m256i *)a);
}

static MLK_INLINE void mlk_poly_compress_d10_native(
    uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D10], const int16_t a[MLKEM_N])
{
  mlk_poly_compress_d10_avx2(r, (const __m256i *)a);
}

static MLK_INLINE void mlk_poly_decompress_d4_native(
    int16_t r[MLKEM_N], const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D4])
{
  mlk_poly_decompress_d4_avx2((__m256i *)r, a);
}

static MLK_INLINE void mlk_poly_decompress_d10_native(
    int16_t r[MLKEM_N], const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D10])
{
  mlk_poly_decompress_d10_avx2((__m256i *)r, a);
}
#endif /* defined(MLK_MULTILEVEL_BUILD_WITH_SHARED) || (MLKEM_K == 2 \
          || MLKEM_K == 3) */

#if defined(MLK_MULTILEVEL_BUILD_WITH_SHARED) || MLKEM_K == 4
static MLK_INLINE void mlk_poly_compress_d5_native(
    uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D5], const int16_t a[MLKEM_N])
{
  mlk_poly_compress_d5_avx2(r, (const __m256i *)a);
}

static MLK_INLINE void mlk_poly_compress_d11_native(
    uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D11], const int16_t a[MLKEM_N])
{
  mlk_poly_compress_d11_avx2(r, (const __m256i *)a);
}

static MLK_INLINE void mlk_poly_decompress_d5_native(
    int16_t r[MLKEM_N], const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D5])
{
  mlk_poly_decompress_d5_avx2((__m256i *)r, a);
}

static MLK_INLINE void mlk_poly_decompress_d11_native(
    int16_t r[MLKEM_N], const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D11])
{
  mlk_poly_decompress_d11_avx2((__m256i *)r, a);
}
#endif /* MLK_MULTILEVEL_BUILD_WITH_SHARED || MLKEM_K == 4 */

#endif /* MLK_ARITH_PROFILE_IMPL_H */

#endif /* MLK_NATIVE_X86_64_SRC_DEFAULT_IMPL_H */
