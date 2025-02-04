/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Implementation from Kyber reference repository
 * https://github.com/pq-crystals/kyber/blob/main/avx2/align.h
 */

#ifndef ALIGN_H
#define ALIGN_H

#include <immintrin.h>
#include <stdint.h>

#define ALIGNED_UINT8(N)        \
  union                         \
  {                             \
    uint8_t coeffs[N];          \
    __m256i vec[(N + 31) / 32]; \
  }

#define ALIGNED_INT16(N)        \
  union                         \
  {                             \
    int16_t coeffs[N];          \
    __m256i vec[(N + 15) / 16]; \
  }

#endif
