/*
 * Copyright (c) 2024-2025 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../../../common.h"

#if defined(MLK_ARITH_BACKEND_X86_64_DEFAULT) && \
    !defined(MLK_MULTILEVEL_BUILD_NO_SHARED)

#include "../../../verify.h"
#include "arith_native_x86_64.h"
#include "consts.h"

static void poly_basemul_montgomery_avx2(int16_t r[MLKEM_N],
                                         const int16_t a[MLKEM_N],
                                         const int16_t b[MLKEM_N])
{
  mlk_basemul_avx2((__m256i *)r, (const __m256i *)a, (const __m256i *)b,
                   mlk_qdata.vec);
}

/*
 * Implementation from Kyber reference repository
 * https://github.com/pq-crystals/kyber/blob/main/avx2
 */
static void poly_add_avx2(int16_t r[MLKEM_N], const int16_t a[MLKEM_N],
                          const int16_t b[MLKEM_N])
{
  unsigned i;
  __m256i f0, f1;

  for (i = 0; i < MLKEM_N; i += 16)
  {
    f0 = _mm256_load_si256((const __m256i *)&a[i]);
    f1 = _mm256_load_si256((const __m256i *)&b[i]);
    f0 = _mm256_add_epi16(f0, f1);
    _mm256_store_si256((__m256i *)&r[i], f0);
  }
}

void mlk_polyvec_basemul_acc_montgomery_cached_avx2(unsigned k,
                                                    int16_t r[MLKEM_N],
                                                    const int16_t *a,
                                                    const int16_t *b,
                                                    const int16_t *b_cache)
{
  unsigned i;
  int16_t t[MLKEM_N] MLK_ALIGN;

  /* TODO: Use mulcache for AVX2. So far, it is unused. */
  ((void)b_cache);

  /* Coefficient-wise bound of each basemul is 2q.
   * Since we are accumulating at most 4 times, the
   * overall bound is 8q < INT16_MAX. */
  poly_basemul_montgomery_avx2(r, &a[0], &b[0]);
  for (i = 1; i < k; i++)
  {
    poly_basemul_montgomery_avx2(t, &a[i * MLKEM_N], &b[i * MLKEM_N]);
    poly_add_avx2(r, r, t);
  }

  /* Specification: Partially implements
   * [FIPS 203, Section 3.3, Destruction of intermediate values] */
  mlk_zeroize(t, sizeof(t));
}

#else /* defined(MLK_ARITH_BACKEND_X86_64_DEFAULT) && \
          !defined(MLK_MULTILEVEL_BUILD_NO_SHARED) */

MLK_EMPTY_CU(avx2_basemul)

#endif /* defined(MLK_ARITH_BACKEND_X86_64_DEFAULT) && \
          !defined(MLK_MULTILEVEL_BUILD_NO_SHARED) */
