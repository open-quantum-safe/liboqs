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

unsigned mlk_rej_uniform_avx2(int16_t *MLK_RESTRICT r, const uint8_t *buf)
{
  unsigned ctr, pos;
  uint16_t val0, val1;
  uint32_t good;
  const __m256i bound =
      _mm256_load_si256(&mlk_qdata.vec[MLK_AVX2_BACKEND_DATA_OFFSET_16XQ / 16]);
  const __m256i ones = _mm256_set1_epi8(1);
  const __m256i mask = _mm256_set1_epi16(0xFFF);
  const __m256i idx8 =
      _mm256_set_epi8(15, 14, 14, 13, 12, 11, 11, 10, 9, 8, 8, 7, 6, 5, 5, 4,
                      11, 10, 10, 9, 8, 7, 7, 6, 5, 4, 4, 3, 2, 1, 1, 0);
  __m256i f0, f1, g0, g1, g2, g3;
  __m128i f, t, pilo, pihi;

  ctr = pos = 0;
  while (ctr <= MLKEM_N - 32 && pos <= MLK_AVX2_REJ_UNIFORM_BUFLEN - 48)
  {
    f0 = _mm256_loadu_si256((__m256i *)&buf[pos]);
    /* Don't load from offset 24, as this would over-read the buffer */
    f1 = _mm256_loadu_si256((__m256i *)&buf[pos + 16]);
    f0 = _mm256_permute4x64_epi64(f0, 0x94 /* 0b10010100 ~= (2,1,1,0) */);
    f1 = _mm256_permute4x64_epi64(f1, 0xe9 /* 0x11101001 ~= (3,2,2,1) */);
    f0 = _mm256_shuffle_epi8(f0, idx8);
    f1 = _mm256_shuffle_epi8(f1, idx8);
    g0 = _mm256_srli_epi16(f0, 4);
    g1 = _mm256_srli_epi16(f1, 4);
    f0 = _mm256_blend_epi16(f0, g0, 0xAA);
    f1 = _mm256_blend_epi16(f1, g1, 0xAA);
    f0 = _mm256_and_si256(f0, mask);
    f1 = _mm256_and_si256(f1, mask);
    pos += 48;

    g0 = _mm256_cmpgt_epi16(bound, f0);
    g1 = _mm256_cmpgt_epi16(bound, f1);

    g0 = _mm256_packs_epi16(g0, g1);
    good = _mm256_movemask_epi8(g0);

    g0 = _mm256_castsi128_si256(
        _mm_loadl_epi64((__m128i *)&mlk_rej_uniform_table[(good >> 0) & 0xFF]));
    g1 = _mm256_castsi128_si256(
        _mm_loadl_epi64((__m128i *)&mlk_rej_uniform_table[(good >> 8) & 0xFF]));
    g0 = _mm256_inserti128_si256(
        g0,
        _mm_loadl_epi64((__m128i *)&mlk_rej_uniform_table[(good >> 16) & 0xFF]),
        1);
    g1 = _mm256_inserti128_si256(
        g1,
        _mm_loadl_epi64((__m128i *)&mlk_rej_uniform_table[(good >> 24) & 0xFF]),
        1);

    g2 = _mm256_add_epi8(g0, ones);
    g3 = _mm256_add_epi8(g1, ones);
    g0 = _mm256_unpacklo_epi8(g0, g2);
    g1 = _mm256_unpacklo_epi8(g1, g3);

    f0 = _mm256_shuffle_epi8(f0, g0);
    f1 = _mm256_shuffle_epi8(f1, g1);

    _mm_storeu_si128((__m128i *)&r[ctr], _mm256_castsi256_si128(f0));
    ctr += _mm_popcnt_u32((good >> 0) & 0xFF);
    _mm_storeu_si128((__m128i *)&r[ctr], _mm256_extracti128_si256(f0, 1));
    ctr += _mm_popcnt_u32((good >> 16) & 0xFF);
    _mm_storeu_si128((__m128i *)&r[ctr], _mm256_castsi256_si128(f1));
    ctr += _mm_popcnt_u32((good >> 8) & 0xFF);
    _mm_storeu_si128((__m128i *)&r[ctr], _mm256_extracti128_si256(f1, 1));
    ctr += _mm_popcnt_u32((good >> 24) & 0xFF);
  }

  while (ctr <= MLKEM_N - 8 && pos <= MLK_AVX2_REJ_UNIFORM_BUFLEN - 24)
  {
    f = _mm_loadu_si128((__m128i *)&buf[pos]);
    f = _mm_shuffle_epi8(f, _mm256_castsi256_si128(idx8));
    t = _mm_srli_epi16(f, 4);
    f = _mm_blend_epi16(f, t, 0xAA);
    f = _mm_and_si128(f, _mm256_castsi256_si128(mask));
    pos += 12;

    t = _mm_cmpgt_epi16(_mm256_castsi256_si128(bound), f);
    good = _mm_movemask_epi8(t);

    good = _pext_u32(good, 0x5555);
    pilo = _mm_loadl_epi64((__m128i *)&mlk_rej_uniform_table[good]);

    pihi = _mm_add_epi8(pilo, _mm256_castsi256_si128(ones));
    pilo = _mm_unpacklo_epi8(pilo, pihi);
    f = _mm_shuffle_epi8(f, pilo);
    _mm_storeu_si128((__m128i *)&r[ctr], f);
    ctr += _mm_popcnt_u32(good);
  }

  while (ctr < MLKEM_N && pos <= MLK_AVX2_REJ_UNIFORM_BUFLEN - 3)
  {
    val0 = ((buf[pos + 0] >> 0) | ((uint16_t)buf[pos + 1] << 8)) & 0xFFF;
    val1 = ((buf[pos + 1] >> 4) | ((uint16_t)buf[pos + 2] << 4));
    pos += 3;

    if (val0 < MLKEM_Q)
    {
      r[ctr++] = val0;
    }
    if (val1 < MLKEM_Q && ctr < MLKEM_N)
    {
      r[ctr++] = val1;
    }
  }

  return ctr;
}

#else /* MLK_ARITH_BACKEND_X86_64_DEFAULT && !MLK_CONFIG_MULTILEVEL_NO_SHARED \
       */

MLK_EMPTY_CU(avx2_rej_uniform)

#endif /* !(MLK_ARITH_BACKEND_X86_64_DEFAULT && \
          !MLK_CONFIG_MULTILEVEL_NO_SHARED) */
