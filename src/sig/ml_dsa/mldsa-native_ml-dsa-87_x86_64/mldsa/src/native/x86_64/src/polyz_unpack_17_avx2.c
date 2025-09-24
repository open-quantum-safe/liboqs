/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* References
 * ==========
 *
 * - [REF_AVX2]
 *   CRYSTALS-Dilithium optimized AVX2 implementation
 *   Bai, Ducas, Kiltz, Lepoint, Lyubashevsky, Schwabe, Seiler, Stehlé
 *   https://github.com/pq-crystals/dilithium/tree/master/avx2
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
#include "arith_native_x86_64.h"

void mld_polyz_unpack_17_avx2(__m256i *r, const uint8_t *a)
{
  unsigned int i;
  __m256i f;
  const __m256i shufbidx =
      _mm256_set_epi8(-1, 9, 8, 7, -1, 7, 6, 5, -1, 5, 4, 3, -1, 3, 2, 1, -1, 8,
                      7, 6, -1, 6, 5, 4, -1, 4, 3, 2, -1, 2, 1, 0);
  const __m256i srlvdidx = _mm256_set_epi32(6, 4, 2, 0, 6, 4, 2, 0);
  const __m256i mask = _mm256_set1_epi32(0x3FFFF);
  const __m256i gamma1 = _mm256_set1_epi32(MLDSA_GAMMA1);

  for (i = 0; i < MLDSA_N / 8; i++)
  {
    f = _mm256_loadu_si256((__m256i *)&a[18 * i]);

    /* Permute 64-bit lanes
     * 0x94 = 10010100b rearranges 64-bit lanes as: [3,2,1,0] -> [2,1,1,0]
     *
     * ╔═══════════════════════════════════════════════════════════════════════╗
     * ║                         Original Layout                               ║
     * ╚═══════════════════════════════════════════════════════════════════════╝
     * ┌─────────────────┬─────────────────┬─────────────────┬─────────────────┐
     * │     Lane 0      │     Lane 1      │     Lane 2      │     Lane 3      │
     * │   bytes 0..7    │   bytes 8..15   │   bytes 16..23  │   bytes 24..31  │
     * └─────────────────┴─────────────────┴─────────────────┴─────────────────┘
     *
     * ╔═══════════════════════════════════════════════════════════════════════╗
     * ║                        Layout after permute                           ║
     * ║        Byte indices in high half shifted down by 8 positions          ║
     * ╚═══════════════════════════════════════════════════════════════════════╝
     * ┌───────────────┬─────────────────┐ ┌─────────────────┬─────────────────┐
     * │   Lane 0      │     Lane 1      │ │     Lane 2      │     Lane 3      │
     * │ bytes 0..7    │   bytes 8..15   │ │   bytes 8..15   │   bytes 16..23  │
     * └───────────────┴─────────────────┘ └─────────────────┴─────────────────┘
     *   Lower 128-bit lane (bytes 0-15)      Upper 128-bit lane (bytes 16-31)
     */
    f = _mm256_permute4x64_epi64(f, 0x94);

    /* Shuffling 8-bit lanes
     *
     * ┌─ Indices 0-8 into low 128-bit half of permuted vector ────────────────┐
     * │ Shuffle: [-1, 8, 7, 6, -1, 6, 5, 4, -1, 4, 3, 2, -1, 2, 1, 0]         │
     * │ Result:  [0, byte8, byte7, byte6, ..., 0, byte2, byte1, byte0]        │
     * └───────────────────────────────────────────────────────────────────────┘
     *
     * ┌─ Indices 1-9 into high 128-bit half of permuted vector ───────────────┐
     * │ Shuffle: [-1, 9, 8, 7, -1, 7, 6, 5, -1, 5, 4, 3, -1, 3, 2, 1]         │
     * │ Result:  [0, byte17, byte16, byte15, ..., 0, byte11, byte10, byte9]   │
     * └───────────────────────────────────────────────────────────────────────┘
     */
    f = _mm256_shuffle_epi8(f, shufbidx);

    /* Keep only 18 out of 24 bits in each 32-bit lane */
    /* Bits   0..23     16..39    32..55    48..71
     *        72..95    88..111   104..127  120..143 */
    f = _mm256_srlv_epi32(f, srlvdidx);
    /* Bits   0..23     18..39    36..55    54..71
     *        72..95    90..111   108..127  126..143 */
    f = _mm256_and_si256(f, mask);
    /* Bits   0..17     18..35    36..53    54..71
     *        72..89    90..107   108..125  126..143 */

    /* Map [0, 1, ..., 2^18-1] to [2^17, 2^17-1, ..., -2^17+1] */
    f = _mm256_sub_epi32(gamma1, f);

    _mm256_store_si256(&r[i], f);
  }
}

#else /* MLD_ARITH_BACKEND_X86_64_DEFAULT && !MLD_CONFIG_MULTILEVEL_NO_SHARED \
       */

MLD_EMPTY_CU(avx2_polyz_unpack)

#endif /* !(MLD_ARITH_BACKEND_X86_64_DEFAULT && \
          !MLD_CONFIG_MULTILEVEL_NO_SHARED) */
