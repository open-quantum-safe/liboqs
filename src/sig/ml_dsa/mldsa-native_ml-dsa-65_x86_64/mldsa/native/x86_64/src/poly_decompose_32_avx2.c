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

#define _mm256_blendv_epi32(a, b, mask)                        \
  _mm256_castps_si256(_mm256_blendv_ps(_mm256_castsi256_ps(a), \
                                       _mm256_castsi256_ps(b), \
                                       _mm256_castsi256_ps(mask)))

void mld_poly_decompose_32_avx2(__m256i *a1, __m256i *a0, const __m256i *a)
{
  unsigned int i;
  __m256i f, f0, f1;
  const __m256i q =
      _mm256_load_si256(&mld_qdata.vec[MLD_AVX2_BACKEND_DATA_OFFSET_8XQ / 8]);
  const __m256i hq = _mm256_srli_epi32(q, 1);
  /* check-magic: 1025 == round((2**22*128) / ((MLDSA_Q - 1) / 16)) */
  const __m256i v = _mm256_set1_epi32(1025);
  const __m256i alpha = _mm256_set1_epi32(2 * MLDSA_GAMMA2);
  const __m256i off = _mm256_set1_epi32(127);
  const __m256i shift = _mm256_set1_epi32(512);
  const __m256i mask = _mm256_set1_epi32(15);

  for (i = 0; i < MLDSA_N / 8; i++)
  {
    f = _mm256_load_si256(&a[i]);
    f1 = _mm256_add_epi32(f, off);
    f1 = _mm256_srli_epi32(f1, 7);
    f1 = _mm256_mulhi_epu16(f1, v);
    f1 = _mm256_mulhrs_epi16(f1, shift);
    f1 = _mm256_and_si256(f1, mask);
    f0 = _mm256_mullo_epi32(f1, alpha);
    f0 = _mm256_sub_epi32(f, f0);
    f = _mm256_cmpgt_epi32(f0, hq);
    f = _mm256_and_si256(f, q);
    f0 = _mm256_sub_epi32(f0, f);
    _mm256_store_si256(&a1[i], f1);
    _mm256_store_si256(&a0[i], f0);
  }
}

#undef _mm256_blendv_epi32

#else /* MLD_ARITH_BACKEND_X86_64_DEFAULT && !MLD_CONFIG_MULTILEVEL_NO_SHARED \
       */

MLD_EMPTY_CU(avx2_poly_decompose)

#endif /* !(MLD_ARITH_BACKEND_X86_64_DEFAULT && \
          !MLD_CONFIG_MULTILEVEL_NO_SHARED) */
