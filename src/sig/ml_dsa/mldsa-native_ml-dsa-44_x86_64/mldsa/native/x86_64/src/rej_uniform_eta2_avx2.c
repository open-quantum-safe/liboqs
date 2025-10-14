/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */


/*
 * This file is derived from the public domain
 * AVX2 Dilithium implementation @[REF_AVX2].
 */

#include "../../../common.h"

#if defined(MLD_ARITH_BACKEND_X86_64_DEFAULT) && \
    !defined(MLD_CONFIG_MULTILEVEL_NO_SHARED)

#include <immintrin.h>
#include <stdint.h>
#include <string.h>
#include "arith_native_x86_64.h"
#include "consts.h"

#define MLD_AVX2_ETA2 2

/*
 * Reference: In the pqcrystals implementation this function is called
 *            rej_eta_avx and supports multiple values for ETA via preprocessor
 *            conditionals. We move the conditionals to the frontend.
 */
unsigned int mld_rej_uniform_eta2_avx2(
    int32_t *MLD_RESTRICT r,
    const uint8_t buf[MLD_AVX2_REJ_UNIFORM_ETA2_BUFLEN])
{
  unsigned int ctr, pos;
  uint32_t good;
  __m256i f0, f1, f2;
  __m128i g0, g1;
  const __m256i mask = _mm256_set1_epi8(15);
  const __m256i eta = _mm256_set1_epi8(MLD_AVX2_ETA2);
  const __m256i bound = mask;
  /* check-magic: -6560 ==  32*round(-2**10 / 5) */
  const __m256i v = _mm256_set1_epi32(-6560);
  const __m256i p = _mm256_set1_epi32(5);

  ctr = pos = 0;
  while (ctr <= MLDSA_N - 8 && pos <= MLD_AVX2_REJ_UNIFORM_ETA2_BUFLEN - 16)
  {
    f0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *)&buf[pos]));
    f1 = _mm256_slli_epi16(f0, 4);
    f0 = _mm256_or_si256(f0, f1);
    f0 = _mm256_and_si256(f0, mask);

    f1 = _mm256_sub_epi8(f0, bound);
    f0 = _mm256_sub_epi8(eta, f0);
    good = _mm256_movemask_epi8(f1);

    g0 = _mm256_castsi256_si128(f0);
    g1 = _mm_loadl_epi64((__m128i *)&mld_rej_uniform_table[good & 0xFF]);
    g1 = _mm_shuffle_epi8(g0, g1);
    f1 = _mm256_cvtepi8_epi32(g1);
    f2 = _mm256_mulhrs_epi16(f1, v);
    f2 = _mm256_mullo_epi16(f2, p);
    f1 = _mm256_add_epi32(f1, f2);
    _mm256_storeu_si256((__m256i *)&r[ctr], f1);
    ctr += _mm_popcnt_u32(good & 0xFF);
    good >>= 8;
    pos += 4;

    if (ctr > MLDSA_N - 8)
    {
      break;
    }
    g0 = _mm_bsrli_si128(g0, 8);
    g1 = _mm_loadl_epi64((__m128i *)&mld_rej_uniform_table[good & 0xFF]);
    g1 = _mm_shuffle_epi8(g0, g1);
    f1 = _mm256_cvtepi8_epi32(g1);
    f2 = _mm256_mulhrs_epi16(f1, v);
    f2 = _mm256_mullo_epi16(f2, p);
    f1 = _mm256_add_epi32(f1, f2);
    _mm256_storeu_si256((__m256i *)&r[ctr], f1);
    ctr += _mm_popcnt_u32(good & 0xFF);
    good >>= 8;
    pos += 4;

    if (ctr > MLDSA_N - 8)
    {
      break;
    }
    g0 = _mm256_extracti128_si256(f0, 1);
    g1 = _mm_loadl_epi64((__m128i *)&mld_rej_uniform_table[good & 0xFF]);
    g1 = _mm_shuffle_epi8(g0, g1);
    f1 = _mm256_cvtepi8_epi32(g1);
    f2 = _mm256_mulhrs_epi16(f1, v);
    f2 = _mm256_mullo_epi16(f2, p);
    f1 = _mm256_add_epi32(f1, f2);
    _mm256_storeu_si256((__m256i *)&r[ctr], f1);
    ctr += _mm_popcnt_u32(good & 0xFF);
    good >>= 8;
    pos += 4;

    if (ctr > MLDSA_N - 8)
    {
      break;
    }
    g0 = _mm_bsrli_si128(g0, 8);
    g1 = _mm_loadl_epi64((__m128i *)&mld_rej_uniform_table[good]);
    g1 = _mm_shuffle_epi8(g0, g1);
    f1 = _mm256_cvtepi8_epi32(g1);
    f2 = _mm256_mulhrs_epi16(f1, v);
    f2 = _mm256_mullo_epi16(f2, p);
    f1 = _mm256_add_epi32(f1, f2);
    _mm256_storeu_si256((__m256i *)&r[ctr], f1);
    ctr += _mm_popcnt_u32(good);
    pos += 4;
  }

  while (ctr < MLDSA_N && pos < MLD_AVX2_REJ_UNIFORM_ETA2_BUFLEN)
  {
    uint32_t t0 = buf[pos] & 0x0F;
    uint32_t t1 = buf[pos++] >> 4;

    if (t0 < 15)
    {
      t0 = t0 - (205 * t0 >> 10) * 5;
      r[ctr++] = 2 - t0;
    }
    if (t1 < 15 && ctr < MLDSA_N)
    {
      t1 = t1 - (205 * t1 >> 10) * 5;
      r[ctr++] = 2 - t1;
    }
  }

  return ctr;
}

#else /* MLD_ARITH_BACKEND_X86_64_DEFAULT && !MLD_CONFIG_MULTILEVEL_NO_SHARED \
       */

MLD_EMPTY_CU(avx2_rej_uniform_eta2)

#endif /* !(MLD_ARITH_BACKEND_X86_64_DEFAULT && \
          !MLD_CONFIG_MULTILEVEL_NO_SHARED) */
