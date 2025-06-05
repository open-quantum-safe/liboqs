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

#ifndef MLK_NATIVE_X86_64_SRC_ALIGN_H
#define MLK_NATIVE_X86_64_SRC_ALIGN_H

/*
 * This file is derived from the public domain
 * AVX2 Kyber implementation @[REF_AVX2].
 */

#include <immintrin.h>
#include <stdint.h>

#define MLK_ALIGNED_INT16(N)    \
  union                         \
  {                             \
    int16_t coeffs[N];          \
    __m256i vec[(N + 15) / 16]; \
  }

#endif /* !MLK_NATIVE_X86_64_SRC_ALIGN_H */
