/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

/* ML-KEM arithmetic native profile for clean assembly */

#ifdef MLKEM_NATIVE_ARITH_PROFILE_IMPL_H
#error Only one MLKEM_ARITH assembly profile can be defined -- did you include multiple profiles?
#else
#define MLKEM_NATIVE_ARITH_PROFILE_IMPL_H

#include <string.h>

#include "../../../params.h"
#include "arith_native_x86_64.h"

#define MLKEM_USE_NATIVE_NTT_CUSTOM_ORDER

#define MLKEM_USE_NATIVE_REJ_UNIFORM
#define MLKEM_USE_NATIVE_NTT
#define MLKEM_USE_NATIVE_INTT
#define MLKEM_USE_NATIVE_POLY_REDUCE
#define MLKEM_USE_NATIVE_POLY_TOMONT
#define MLKEM_USE_NATIVE_POLYVEC_BASEMUL_ACC_MONTGOMERY_CACHED
#define MLKEM_USE_NATIVE_POLY_MULCACHE_COMPUTE
#define MLKEM_USE_NATIVE_POLY_TOBYTES
#define MLKEM_USE_NATIVE_POLY_FROMBYTES
#define MLKEM_USE_NATIVE_POLY_COMPRESS_D4
#define MLKEM_USE_NATIVE_POLY_COMPRESS_D5
#define MLKEM_USE_NATIVE_POLY_COMPRESS_D10
#define MLKEM_USE_NATIVE_POLY_COMPRESS_D11
#define MLKEM_USE_NATIVE_POLY_DECOMPRESS_D4
#define MLKEM_USE_NATIVE_POLY_DECOMPRESS_D5
#define MLKEM_USE_NATIVE_POLY_DECOMPRESS_D10
#define MLKEM_USE_NATIVE_POLY_DECOMPRESS_D11

static INLINE void poly_permute_bitrev_to_custom(int16_t data[MLKEM_N])
{
  nttunpack_avx2((__m256i *)(data), qdata.vec);
}

static INLINE int rej_uniform_native(int16_t *r, unsigned len,
                                     const uint8_t *buf, unsigned buflen)
{
  /* AVX2 implementation assumes specific buffer lengths */
  if (len != MLKEM_N || buflen != REJ_UNIFORM_AVX_BUFLEN)
  {
    return -1;
  }

  return (int)rej_uniform_avx2(r, buf);
}

static INLINE void ntt_native(int16_t data[MLKEM_N])
{
  ntt_avx2((__m256i *)data, qdata.vec);
}

static INLINE void intt_native(int16_t data[MLKEM_N])
{
  invntt_avx2((__m256i *)data, qdata.vec);
}

static INLINE void poly_reduce_native(int16_t data[MLKEM_N])
{
  reduce_avx2((__m256i *)data, qdata.vec);
}

static INLINE void poly_tomont_native(int16_t data[MLKEM_N])
{
  tomont_avx2((__m256i *)data, qdata.vec);
}

static INLINE void poly_mulcache_compute_native(int16_t x[MLKEM_N / 2],
                                                const int16_t y[MLKEM_N])
{
  /* AVX2 backend does not use mulcache */
  ((void)y);
  ((void)x);
}

static INLINE void polyvec_basemul_acc_montgomery_cached_native(
    int16_t r[MLKEM_N], const int16_t a[MLKEM_K * MLKEM_N],
    const int16_t b[MLKEM_K * MLKEM_N],
    const int16_t b_cache[MLKEM_K * (MLKEM_N / 2)])
{
  polyvec_basemul_acc_montgomery_cached_avx2(r, a, b, b_cache);
}

static INLINE void poly_tobytes_native(uint8_t r[MLKEM_POLYBYTES],
                                       const int16_t a[MLKEM_N])
{
  ntttobytes_avx2(r, (const __m256i *)a, qdata.vec);
}

static INLINE void poly_frombytes_native(int16_t r[MLKEM_N],
                                         const uint8_t a[MLKEM_POLYBYTES])
{
  nttfrombytes_avx2((__m256i *)r, a, qdata.vec);
}

#if defined(MLKEM_NATIVE_MULTILEVEL_BUILD_WITH_SHARED) || \
    (MLKEM_K == 2 || MLKEM_K == 3)
static INLINE void poly_compress_d4_native(
    uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D4], const int16_t a[MLKEM_N])
{
  poly_compress_d4_avx2(r, (const __m256i *)a);
}

static INLINE void poly_compress_d10_native(
    uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D10], const int16_t a[MLKEM_N])
{
  poly_compress_d10_avx2(r, (const __m256i *)a);
}

static INLINE void poly_decompress_d4_native(
    int16_t r[MLKEM_N], const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D4])
{
  poly_decompress_d4_avx2((__m256i *)r, a);
}

static INLINE void poly_decompress_d10_native(
    int16_t r[MLKEM_N], const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D10])
{
  poly_decompress_d10_avx2((__m256i *)r, a);
}
#endif /* defined(MLKEM_NATIVE_MULTILEVEL_BUILD_WITH_SHARED) || (MLKEM_K == 2 \
          || MLKEM_K == 3) */

#if defined(MLKEM_NATIVE_MULTILEVEL_BUILD_WITH_SHARED) || MLKEM_K == 4
static INLINE void poly_compress_d5_native(
    uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D5], const int16_t a[MLKEM_N])
{
  poly_compress_d5_avx2(r, (const __m256i *)a);
}

static INLINE void poly_compress_d11_native(
    uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D11], const int16_t a[MLKEM_N])
{
  poly_compress_d11_avx2(r, (const __m256i *)a);
}

static INLINE void poly_decompress_d5_native(
    int16_t r[MLKEM_N], const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D5])
{
  poly_decompress_d5_avx2((__m256i *)r, a);
}

static INLINE void poly_decompress_d11_native(
    int16_t r[MLKEM_N], const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D11])
{
  poly_decompress_d11_avx2((__m256i *)r, a);
}
#endif /* MLKEM_NATIVE_MULTILEVEL_BUILD_WITH_SHARED || MLKEM_K == 4 */

#endif /* MLKEM_NATIVE_ARITH_PROFILE_IMPL_H */
