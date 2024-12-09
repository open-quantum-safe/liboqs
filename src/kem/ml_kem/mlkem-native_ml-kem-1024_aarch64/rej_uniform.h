/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef REJ_UNIFORM_H
#define REJ_UNIFORM_H

#include <stdint.h>
#include <stdlib.h>
#include "cbmc.h"
#include "common.h"

#define rej_uniform MLKEM_NAMESPACE(rej_uniform)
/*************************************************
 * Name:        rej_uniform
 *
 * Description: Run rejection sampling on uniform random bytes to generate
 *              uniform random integers mod q
 *
 * Arguments:   - int16_t *r:          pointer to output buffer
 *              - unsigned int target: requested number of 16-bit integers
 *                                     (uniform mod q).
 *                                     Must be <= 4096.
 *              - unsigned int offset: number of 16-bit integers that have
 *                                     already been sampled.
 *                                     Must be <= target.
 *              - const uint8_t *buf:  pointer to input buffer
 *                                     (assumed to be uniform random bytes)
 *              - unsigned int buflen: length of input buffer in bytes
 *                                     Must be <= 4096.
 *                                     Must be a multiple of 3.
 *
 * Note: Strictly speaking, only a few values of buflen near UINT_MAX need
 * excluding. The limit of 4096 is somewhat arbitary but sufficient for all
 * uses of this function. Similarly, the actual limit for target is UINT_MAX/2.
 *
 * Returns the new offset of sampled 16-bit integers, at most target,
 * and at least the initial offset.
 * If the new offset is strictly less than len, all of the input buffers
 * is guaranteed to have been consumed. If it is equal to len, no information
 * is provided on how many bytes of the input buffer have been consumed.
 **************************************************/

/*
 * NOTE: The signature differs from the Kyber reference implementation
 * in that it adds the offset and always expects the base of the target
 * buffer. This avoids shifting the buffer base in the caller, which appears
 * tricky to reason about.
 */
MLKEM_NATIVE_INTERNAL_API
unsigned int rej_uniform(int16_t *r, unsigned int target, unsigned int offset,
                         const uint8_t *buf, unsigned int buflen)
__contract__(
  requires(offset <= target && target <= 4096 && buflen <= 4096 && buflen % 3 == 0)
  requires(memory_no_alias(r, sizeof(int16_t) * target))
  requires(memory_no_alias(buf, buflen))
  requires(offset > 0 ==> array_bound(r, 0, offset, 0, MLKEM_Q))
  assigns(memory_slice(r, sizeof(int16_t) * target))
  ensures(offset <= return_value && return_value <= target)
  ensures(return_value > 0 ==> array_bound(r, 0, return_value, 0, MLKEM_Q))
);
#endif
