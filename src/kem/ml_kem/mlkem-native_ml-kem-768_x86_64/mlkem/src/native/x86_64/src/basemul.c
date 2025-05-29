/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* References
 * ==========
 *
 * - [FIPS203]
 *   FIPS 203 Module-Lattice-Based Key-Encapsulation Mechanism Standard
 *   National Institute of Standards and Technology
 *   https://csrc.nist.gov/pubs/fips/203/final
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

#include "../../../verify.h"
#include "arith_native_x86_64.h"
#include "consts.h"

static void poly_basemul_montgomery_avx2(int16_t r[MLKEM_N],
                                         const int16_t a[MLKEM_N],
                                         const int16_t b[MLKEM_N],
                                         const int16_t b_cache[MLKEM_N / 2])
{
  mlk_basemul_avx2((__m256i *)r, (const __m256i *)a, (const __m256i *)b,
                   (const __m256i *)b_cache, mlk_qdata.vec);
}

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

  /* Coefficient-wise bound of each basemul is 2q.
   * Since we are accumulating at most 4 times, the
   * overall bound is 8q < INT16_MAX. */
  poly_basemul_montgomery_avx2(r, &a[0], &b[0], &b_cache[0]);
  for (i = 1; i < k; i++)
  {
    poly_basemul_montgomery_avx2(t, &a[i * MLKEM_N], &b[i * MLKEM_N],
                                 &b_cache[i * (MLKEM_N / 2)]);
    poly_add_avx2(r, r, t);
  }

  /* Specification: Partially implements
   * @[FIPS203, Section 3.3, Destruction of intermediate values] */
  mlk_zeroize(t, sizeof(t));
}

#else /* MLK_ARITH_BACKEND_X86_64_DEFAULT && !MLK_CONFIG_MULTILEVEL_NO_SHARED \
       */

MLK_EMPTY_CU(avx2_basemul)

#endif /* !(MLK_ARITH_BACKEND_X86_64_DEFAULT && \
          !MLK_CONFIG_MULTILEVEL_NO_SHARED) */
