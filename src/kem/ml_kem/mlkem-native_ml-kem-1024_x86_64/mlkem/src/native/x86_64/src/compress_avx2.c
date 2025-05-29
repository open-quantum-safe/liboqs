/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* References
 * ==========
 *
 * - [REF_AVX2]
 *   CRYSTALS-Kyber optimized AVX2 implementation
 *   Bos, Ducas, Kiltz, Lepoint, Lyubashevsky, Schanck, Schwabe, Seiler, Stehlé
 *   https://github.com/pq-crystals/kyber/tree/main/avx2
 */

/*
 * This file is derived from the public domain
 * AVX2 Kyber implementation @[REF_AVX2].
 */

#include "../../../common.h"

#if defined(MLK_ARITH_BACKEND_X86_64_DEFAULT) && \
    !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED)

#include <immintrin.h>
#include <stdint.h>
#include <string.h>
#include "arith_native_x86_64.h"
#include "consts.h"

#if defined(MLK_CONFIG_MULTILEVEL_WITH_SHARED) || (MLKEM_K == 2 || MLKEM_K == 3)
void mlk_poly_compress_d4_avx2(uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D4],
                               const __m256i *MLK_RESTRICT a)
{
  unsigned int i;
  __m256i f0, f1, f2, f3;
  const __m256i v =
      _mm256_load_si256(&mlk_qdata.vec[MLK_AVX2_BACKEND_DATA_OFFSET_16XV / 16]);
  const __m256i shift1 = _mm256_set1_epi16(1 << 9);
  const __m256i mask = _mm256_set1_epi16(15);
  const __m256i shift2 = _mm256_set1_epi16((16 << 8) + 1);
  const __m256i permdidx = _mm256_set_epi32(7, 3, 6, 2, 5, 1, 4, 0);

  for (i = 0; i < MLKEM_N / 64; i++)
  {
    f0 = _mm256_load_si256(&a[4 * i + 0]);
    f1 = _mm256_load_si256(&a[4 * i + 1]);
    f2 = _mm256_load_si256(&a[4 * i + 2]);
    f3 = _mm256_load_si256(&a[4 * i + 3]);
    f0 = _mm256_mulhi_epi16(f0, v);
    f1 = _mm256_mulhi_epi16(f1, v);
    f2 = _mm256_mulhi_epi16(f2, v);
    f3 = _mm256_mulhi_epi16(f3, v);
    f0 = _mm256_mulhrs_epi16(f0, shift1);
    f1 = _mm256_mulhrs_epi16(f1, shift1);
    f2 = _mm256_mulhrs_epi16(f2, shift1);
    f3 = _mm256_mulhrs_epi16(f3, shift1);
    f0 = _mm256_and_si256(f0, mask);
    f1 = _mm256_and_si256(f1, mask);
    f2 = _mm256_and_si256(f2, mask);
    f3 = _mm256_and_si256(f3, mask);
    f0 = _mm256_packus_epi16(f0, f1);
    f2 = _mm256_packus_epi16(f2, f3);
    f0 = _mm256_maddubs_epi16(f0, shift2);
    f2 = _mm256_maddubs_epi16(f2, shift2);
    f0 = _mm256_packus_epi16(f0, f2);
    f0 = _mm256_permutevar8x32_epi32(f0, permdidx);
    _mm256_storeu_si256((__m256i *)&r[32 * i], f0);
  }
}

void mlk_poly_decompress_d4_avx2(__m256i *MLK_RESTRICT r,
                                 const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D4])
{
  unsigned int i;
  __m128i t;
  __m256i f;
  const __m256i q =
      _mm256_load_si256(&mlk_qdata.vec[MLK_AVX2_BACKEND_DATA_OFFSET_16XQ / 16]);
  const __m256i shufbidx =
      _mm256_set_epi8(7, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3,
                      3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0);
  const __m256i mask = _mm256_set1_epi32(0x00F0000F);
  const __m256i shift = _mm256_set1_epi32((128 << 16) + 2048);

  for (i = 0; i < MLKEM_N / 16; i++)
  {
    t = _mm_loadl_epi64((__m128i *)&a[8 * i]);
    f = _mm256_broadcastsi128_si256(t);
    f = _mm256_shuffle_epi8(f, shufbidx);
    f = _mm256_and_si256(f, mask);
    f = _mm256_mullo_epi16(f, shift);
    f = _mm256_mulhrs_epi16(f, q);
    _mm256_store_si256(&r[i], f);
  }
}

void mlk_poly_compress_d10_avx2(uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D10],
                                const __m256i *MLK_RESTRICT a)
{
  unsigned int i;
  __m256i f0, f1, f2;
  __m128i t0, t1;
  const __m256i v =
      _mm256_load_si256(&mlk_qdata.vec[MLK_AVX2_BACKEND_DATA_OFFSET_16XV / 16]);
  const __m256i v8 = _mm256_slli_epi16(v, 3);
  const __m256i off = _mm256_set1_epi16(15);
  const __m256i shift1 = _mm256_set1_epi16(1 << 12);
  const __m256i mask = _mm256_set1_epi16(1023);
  const __m256i shift2 =
      _mm256_set1_epi64x((1024LL << 48) + (1LL << 32) + (1024 << 16) + 1);
  const __m256i sllvdidx = _mm256_set1_epi64x(12);
  const __m256i shufbidx =
      _mm256_set_epi8(8, 4, 3, 2, 1, 0, -1, -1, -1, -1, -1, -1, 12, 11, 10, 9,
                      -1, -1, -1, -1, -1, -1, 12, 11, 10, 9, 8, 4, 3, 2, 1, 0);

  for (i = 0; i < MLKEM_N / 16; i++)
  {
    f0 = _mm256_load_si256(&a[i]);
    f1 = _mm256_mullo_epi16(f0, v8);
    f2 = _mm256_add_epi16(f0, off);
    f0 = _mm256_slli_epi16(f0, 3);
    f0 = _mm256_mulhi_epi16(f0, v);
    f2 = _mm256_sub_epi16(f1, f2);
    f1 = _mm256_andnot_si256(f1, f2);
    f1 = _mm256_srli_epi16(f1, 15);
    f0 = _mm256_sub_epi16(f0, f1);
    f0 = _mm256_mulhrs_epi16(f0, shift1);
    f0 = _mm256_and_si256(f0, mask);
    f0 = _mm256_madd_epi16(f0, shift2);
    f0 = _mm256_sllv_epi32(f0, sllvdidx);
    f0 = _mm256_srli_epi64(f0, 12);
    f0 = _mm256_shuffle_epi8(f0, shufbidx);
    t0 = _mm256_castsi256_si128(f0);
    t1 = _mm256_extracti128_si256(f0, 1);
    t0 = _mm_blend_epi16(t0, t1, 0xE0);
    _mm_storeu_si128((__m128i *)&r[20 * i + 0], t0);
    memcpy(&r[20 * i + 16], &t1, 4);
  }
}

void mlk_poly_decompress_d10_avx2(
    __m256i *MLK_RESTRICT r, const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D10])
{
  unsigned int i;
  __m256i f;
  const __m256i q = _mm256_set1_epi32((MLKEM_Q << 16) + 4 * MLKEM_Q);
  const __m256i shufbidx =
      _mm256_set_epi8(11, 10, 10, 9, 9, 8, 8, 7, 6, 5, 5, 4, 4, 3, 3, 2, 9, 8,
                      8, 7, 7, 6, 6, 5, 4, 3, 3, 2, 2, 1, 1, 0);
  const __m256i sllvdidx = _mm256_set1_epi64x(4);
  /* TODO: Explain magic values */
  /* check-magic: off */
  const __m256i mask = _mm256_set1_epi32((32736 << 16) + 8184);
  /* check-magic: on */

  for (i = 0; i < (MLKEM_N / 16) - 1; i++)
  {
    f = _mm256_loadu_si256((__m256i *)&a[20 * i]);
    f = _mm256_permute4x64_epi64(f, 0x94);
    f = _mm256_shuffle_epi8(f, shufbidx);
    f = _mm256_sllv_epi32(f, sllvdidx);
    f = _mm256_srli_epi16(f, 1);
    f = _mm256_and_si256(f, mask);
    f = _mm256_mulhrs_epi16(f, q);
    _mm256_store_si256(&r[i], f);
  }

  /* Handle load in last iteration especially to avoid buffer overflow */
  memcpy(&f, &a[20 * i], 20);
  /* The rest is the same */
  f = _mm256_permute4x64_epi64(f, 0x94);
  f = _mm256_shuffle_epi8(f, shufbidx);
  f = _mm256_sllv_epi32(f, sllvdidx);
  f = _mm256_srli_epi16(f, 1);
  f = _mm256_and_si256(f, mask);
  f = _mm256_mulhrs_epi16(f, q);
  _mm256_store_si256(&r[i], f);
}

#endif /* MLK_CONFIG_MULTILEVEL_WITH_SHARED || MLKEM_K == 2 || MLKEM_K == 3 */

#if defined(MLK_CONFIG_MULTILEVEL_WITH_SHARED) || MLKEM_K == 4
void mlk_poly_compress_d5_avx2(uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D5],
                               const __m256i *MLK_RESTRICT a)
{
  unsigned int i;
  __m256i f0, f1;
  __m128i t0, t1;
  const __m256i v =
      _mm256_load_si256(&mlk_qdata.vec[MLK_AVX2_BACKEND_DATA_OFFSET_16XV / 16]);
  const __m256i shift1 = _mm256_set1_epi16(1 << 10);
  const __m256i mask = _mm256_set1_epi16(31);
  const __m256i shift2 = _mm256_set1_epi16((32 << 8) + 1);
  const __m256i shift3 = _mm256_set1_epi32((1024 << 16) + 1);
  const __m256i sllvdidx = _mm256_set1_epi64x(12);
  const __m256i shufbidx =
      _mm256_set_epi8(8, -1, -1, -1, -1, -1, 4, 3, 2, 1, 0, -1, 12, 11, 10, 9,
                      -1, 12, 11, 10, 9, 8, -1, -1, -1, -1, -1, 4, 3, 2, 1, 0);

  for (i = 0; i < MLKEM_N / 32; i++)
  {
    f0 = _mm256_load_si256(&a[2 * i + 0]);
    f1 = _mm256_load_si256(&a[2 * i + 1]);
    f0 = _mm256_mulhi_epi16(f0, v);
    f1 = _mm256_mulhi_epi16(f1, v);
    f0 = _mm256_mulhrs_epi16(f0, shift1);
    f1 = _mm256_mulhrs_epi16(f1, shift1);
    f0 = _mm256_and_si256(f0, mask);
    f1 = _mm256_and_si256(f1, mask);
    f0 = _mm256_packus_epi16(f0, f1);
    f0 = _mm256_maddubs_epi16(
        f0, shift2); /* a0 a1 a2 a3 b0 b1 b2 b3 a4 a5 a6 a7 b4 b5 b6 b7 */
    f0 = _mm256_madd_epi16(f0, shift3); /* a0 a1 b0 b1 a2 a3 b2 b3 */
    f0 = _mm256_sllv_epi32(f0, sllvdidx);
    f0 = _mm256_srlv_epi64(f0, sllvdidx);
    f0 = _mm256_shuffle_epi8(f0, shufbidx);
    t0 = _mm256_castsi256_si128(f0);
    t1 = _mm256_extracti128_si256(f0, 1);
    t0 = _mm_blendv_epi8(t0, t1, _mm256_castsi256_si128(shufbidx));
    _mm_storeu_si128((__m128i *)&r[20 * i + 0], t0);
    memcpy(&r[20 * i + 16], &t1, 4);
  }
}

void mlk_poly_decompress_d5_avx2(__m256i *MLK_RESTRICT r,
                                 const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D5])
{
  unsigned int i;
  __m128i t;
  __m256i f;
  int16_t ti;
  const __m256i q =
      _mm256_load_si256(&mlk_qdata.vec[MLK_AVX2_BACKEND_DATA_OFFSET_16XQ / 16]);
  const __m256i shufbidx =
      _mm256_set_epi8(9, 9, 9, 8, 8, 8, 8, 7, 7, 6, 6, 6, 6, 5, 5, 5, 4, 4, 4,
                      3, 3, 3, 3, 2, 2, 1, 1, 1, 1, 0, 0, 0);
  /* TODO: Document those magic values */
  /* check-magic: off */
  const __m256i mask = _mm256_set_epi16(248, 1984, 62, 496, 3968, 124, 992, 31,
                                        248, 1984, 62, 496, 3968, 124, 992, 31);
  const __m256i shift = _mm256_set_epi16(128, 16, 512, 64, 8, 256, 32, 1024,
                                         128, 16, 512, 64, 8, 256, 32, 1024);
  /* check-magic: on */
  for (i = 0; i < MLKEM_N / 16; i++)
  {
    t = _mm_loadl_epi64((__m128i *)&a[10 * i + 0]);
    memcpy(&ti, &a[10 * i + 8], 2);
    t = _mm_insert_epi16(t, ti, 4);
    f = _mm256_broadcastsi128_si256(t);
    f = _mm256_shuffle_epi8(f, shufbidx);
    f = _mm256_and_si256(f, mask);
    f = _mm256_mullo_epi16(f, shift);
    f = _mm256_mulhrs_epi16(f, q);
    _mm256_store_si256(&r[i], f);
  }
}

void mlk_poly_compress_d11_avx2(uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D11],
                                const __m256i *MLK_RESTRICT a)
{
  unsigned int i;
  __m256i f0, f1, f2;
  __m128i t0, t1;
  const __m256i v =
      _mm256_load_si256(&mlk_qdata.vec[MLK_AVX2_BACKEND_DATA_OFFSET_16XV / 16]);
  const __m256i v8 = _mm256_slli_epi16(v, 3);
  const __m256i off = _mm256_set1_epi16(36);
  const __m256i shift1 = _mm256_set1_epi16(1 << 13);
  const __m256i mask = _mm256_set1_epi16(2047);
  const __m256i shift2 =
      _mm256_set1_epi64x((2048LL << 48) + (1LL << 32) + (2048 << 16) + 1);
  const __m256i sllvdidx = _mm256_set1_epi64x(10);
  const __m256i srlvqidx = _mm256_set_epi64x(30, 10, 30, 10);
  const __m256i shufbidx =
      _mm256_set_epi8(4, 3, 2, 1, 0, 0, -1, -1, -1, -1, 10, 9, 8, 7, 6, 5, -1,
                      -1, -1, -1, -1, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);

  for (i = 0; i < (MLKEM_N / 16) - 1; i++)
  {
    f0 = _mm256_load_si256(&a[i]);
    f1 = _mm256_mullo_epi16(f0, v8);
    f2 = _mm256_add_epi16(f0, off);
    f0 = _mm256_slli_epi16(f0, 3);
    f0 = _mm256_mulhi_epi16(f0, v);
    f2 = _mm256_sub_epi16(f1, f2);
    f1 = _mm256_andnot_si256(f1, f2);
    f1 = _mm256_srli_epi16(f1, 15);
    f0 = _mm256_sub_epi16(f0, f1);
    f0 = _mm256_mulhrs_epi16(f0, shift1);
    f0 = _mm256_and_si256(f0, mask);
    f0 = _mm256_madd_epi16(f0, shift2);
    f0 = _mm256_sllv_epi32(f0, sllvdidx);
    f1 = _mm256_bsrli_epi128(f0, 8);
    f0 = _mm256_srlv_epi64(f0, srlvqidx);
    f1 = _mm256_slli_epi64(f1, 34);
    f0 = _mm256_add_epi64(f0, f1);
    f0 = _mm256_shuffle_epi8(f0, shufbidx);
    t0 = _mm256_castsi256_si128(f0);
    t1 = _mm256_extracti128_si256(f0, 1);
    t0 = _mm_blendv_epi8(t0, t1, _mm256_castsi256_si128(shufbidx));
    _mm_storeu_si128((__m128i *)&r[22 * i + 0], t0);
    _mm_storel_epi64((__m128i *)&r[22 * i + 16], t1);
  }

  f0 = _mm256_load_si256(&a[i]);
  f1 = _mm256_mullo_epi16(f0, v8);
  f2 = _mm256_add_epi16(f0, off);
  f0 = _mm256_slli_epi16(f0, 3);
  f0 = _mm256_mulhi_epi16(f0, v);
  f2 = _mm256_sub_epi16(f1, f2);
  f1 = _mm256_andnot_si256(f1, f2);
  f1 = _mm256_srli_epi16(f1, 15);
  f0 = _mm256_sub_epi16(f0, f1);
  f0 = _mm256_mulhrs_epi16(f0, shift1);
  f0 = _mm256_and_si256(f0, mask);
  f0 = _mm256_madd_epi16(f0, shift2);
  f0 = _mm256_sllv_epi32(f0, sllvdidx);
  f1 = _mm256_bsrli_epi128(f0, 8);
  f0 = _mm256_srlv_epi64(f0, srlvqidx);
  f1 = _mm256_slli_epi64(f1, 34);
  f0 = _mm256_add_epi64(f0, f1);
  f0 = _mm256_shuffle_epi8(f0, shufbidx);
  t0 = _mm256_castsi256_si128(f0);
  t1 = _mm256_extracti128_si256(f0, 1);
  t0 = _mm_blendv_epi8(t0, t1, _mm256_castsi256_si128(shufbidx));
  _mm_storeu_si128((__m128i *)&r[22 * i + 0], t0);
  /* Handle store in last iteration especially to avoid overflow */
  memcpy(&r[22 * i + 16], &t1, 6);
}

void mlk_poly_decompress_d11_avx2(
    __m256i *MLK_RESTRICT r, const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D11])
{
  unsigned int i;
  __m256i f;
  const __m256i q =
      _mm256_load_si256(&mlk_qdata.vec[MLK_AVX2_BACKEND_DATA_OFFSET_16XQ / 16]);
  const __m256i shufbidx =
      _mm256_set_epi8(13, 12, 12, 11, 10, 9, 9, 8, 8, 7, 6, 5, 5, 4, 4, 3, 10,
                      9, 9, 8, 7, 6, 6, 5, 5, 4, 3, 2, 2, 1, 1, 0);
  const __m256i srlvdidx = _mm256_set_epi32(0, 0, 1, 0, 0, 0, 1, 0);
  const __m256i srlvqidx = _mm256_set_epi64x(2, 0, 2, 0);
  const __m256i shift =
      _mm256_set_epi16(4, 32, 1, 8, 32, 1, 4, 32, 4, 32, 1, 8, 32, 1, 4, 32);
  /* TODO: Explain magic constant */
  /* check-magic: off */
  const __m256i mask = _mm256_set1_epi16(32752);
  /* check-magic: on */

  for (i = 0; i < (MLKEM_N / 16) - 1; i++)
  {
    f = _mm256_loadu_si256((__m256i *)&a[22 * i]);
    f = _mm256_permute4x64_epi64(f, 0x94);
    f = _mm256_shuffle_epi8(f, shufbidx);
    f = _mm256_srlv_epi32(f, srlvdidx);
    f = _mm256_srlv_epi64(f, srlvqidx);
    f = _mm256_mullo_epi16(f, shift);
    f = _mm256_srli_epi16(f, 1);
    f = _mm256_and_si256(f, mask);
    f = _mm256_mulhrs_epi16(f, q);
    _mm256_store_si256(&r[i], f);
  }

  /* Handle load of last iteration especially */
  memcpy(&f, &a[22 * i], 22);
  /* The rest of the iteration is the same */
  f = _mm256_permute4x64_epi64(f, 0x94);
  f = _mm256_shuffle_epi8(f, shufbidx);
  f = _mm256_srlv_epi32(f, srlvdidx);
  f = _mm256_srlv_epi64(f, srlvqidx);
  f = _mm256_mullo_epi16(f, shift);
  f = _mm256_srli_epi16(f, 1);
  f = _mm256_and_si256(f, mask);
  f = _mm256_mulhrs_epi16(f, q);
  _mm256_store_si256(&r[i], f);
}

#endif /* MLK_CONFIG_MULTILEVEL_WITH_SHARED || MLKEM_K == 4 */

#else /* MLK_ARITH_BACKEND_X86_64_DEFAULT && !MLK_CONFIG_MULTILEVEL_NO_SHARED \
       */

MLK_EMPTY_CU(avx2_poly_compress)

#endif /* !(MLK_ARITH_BACKEND_X86_64_DEFAULT && \
          !MLK_CONFIG_MULTILEVEL_NO_SHARED) */
