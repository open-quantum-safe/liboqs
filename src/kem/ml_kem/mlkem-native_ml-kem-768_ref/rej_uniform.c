/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

#include "rej_uniform.h"
#include "arith_backend.h"

/* Static namespacing
 * This is to facilitate building multiple instances
 * of mlkem-native (e.g. with varying security levels)
 * within a single compilation unit. */
#define rej_uniform_scalar MLKEM_NAMESPACE(rej_uniform_scalar)
/* End of static namespacing */

/*************************************************
 * Name:        rej_uniform_scalar
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
static unsigned int rej_uniform_scalar(int16_t *r, unsigned int target,
                                       unsigned int offset, const uint8_t *buf,
                                       unsigned int buflen)
__contract__(
  requires(offset <= target && target <= 4096 && buflen <= 4096 && buflen % 3 == 0)
  requires(memory_no_alias(r, sizeof(int16_t) * target))
  requires(memory_no_alias(buf, buflen))
  requires(offset > 0 ==> array_bound(r, 0, offset, 0, MLKEM_Q))
  assigns(memory_slice(r, sizeof(int16_t) * target))
  ensures(offset <= return_value && return_value <= target)
  ensures(return_value > 0 ==> array_bound(r, 0, return_value, 0, MLKEM_Q))
)
{
  unsigned int ctr, pos;
  uint16_t val0, val1;

  ctr = offset;
  pos = 0;
  /* pos + 3 cannot overflow due to the assumption buflen <= 4096 */
  while (ctr < target && pos + 3 <= buflen)
  __loop__(
    invariant(offset <= ctr && ctr <= target && pos <= buflen)
    invariant(ctr > 0 ==> array_bound(r, 0, ctr, 0, MLKEM_Q)))
  {
    val0 = ((buf[pos + 0] >> 0) | ((uint16_t)buf[pos + 1] << 8)) & 0xFFF;
    val1 = ((buf[pos + 1] >> 4) | ((uint16_t)buf[pos + 2] << 4)) & 0xFFF;
    pos += 3;

    if (val0 < MLKEM_Q)
    {
      r[ctr++] = val0;
    }
    if (ctr < target && val1 < MLKEM_Q)
    {
      r[ctr++] = val1;
    }
  }
  return ctr;
}

#if !defined(MLKEM_USE_NATIVE_REJ_UNIFORM)
unsigned int rej_uniform(int16_t *r, unsigned int target, unsigned int offset,
                         const uint8_t *buf, unsigned int buflen)
{
  return rej_uniform_scalar(r, target, offset, buf, buflen);
}
#else  /* MLKEM_USE_NATIVE_REJ_UNIFORM */

MLKEM_NATIVE_INTERNAL_API
unsigned int rej_uniform(int16_t *r, unsigned int target, unsigned int offset,
                         const uint8_t *buf, unsigned int buflen)
{
  int ret;

  /* Sample from large buffer with full lane as much as possible. */
  ret = rej_uniform_native(r + offset, target - offset, buf, buflen);
  if (ret != -1)
    return offset + (unsigned)ret;

  return rej_uniform_scalar(r, target, offset, buf, buflen);
}
#endif /* MLKEM_USE_NATIVE_REJ_UNIFORM */
