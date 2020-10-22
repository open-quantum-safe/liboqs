/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#include "randomness.h"
#include <oqs/rand.h>

/* OQS note: random functions using OQS's */
int rand_bits(uint8_t* dst, size_t num_bits) {
  const size_t num_bytes = (num_bits + 7) / 8;
  const size_t num_extra_bits = num_bits % 8;

  OQS_randombytes(dst, num_bytes);

  if (num_extra_bits) {
    dst[num_bytes - 1] &= UINT8_C(0xff) << (8 - num_extra_bits);
  }

  return 0;
}
