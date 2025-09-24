/*
 * Copyright (c) The mlkem-native project authors
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

#ifndef MLD_NATIVE_X86_64_SRC_ALIGN_H
#define MLD_NATIVE_X86_64_SRC_ALIGN_H

/*
 * This file is derived from the public domain
 * AVX2 Dilithium implementation @[REF_AVX2].
 */

#include <immintrin.h>
#include <stdint.h>

#define MLD_ALIGNED_INT32(N)  \
  union                       \
  {                           \
    int32_t coeffs[N];        \
    __m256i vec[(N + 7) / 8]; \
  }

#endif /* !MLD_NATIVE_X86_64_SRC_ALIGN_H */
