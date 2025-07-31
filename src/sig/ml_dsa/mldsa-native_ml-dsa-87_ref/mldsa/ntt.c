/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#include <stdint.h>

#include "ntt.h"
#include "reduce.h"

static int32_t mld_fqmul(int32_t a, int32_t b)
__contract__(
  requires(b > -MLDSA_Q_HALF && b < MLDSA_Q_HALF)
  ensures(return_value > -MLDSA_Q && return_value < MLDSA_Q)
)
{
  return mld_montgomery_reduce((int64_t)a * (int64_t)b);
  /* TODO: reason about bounds */
}

/*************************************************
 * Name:        mld_fqsacle
 *
 * Description: Scales a field element by mont/256 , i.e., performs Montgomery
 *              multiplication by mont^2/256.
 *              Input is expected to have absolute value smaller than
 *              256 * MLDSA_Q.
 *              Output has absolute value smaller than MLD_INTT_BOUND (4211139).
 *
 * Arguments:   - int32_t a: Field element to be scaled.
 **************************************************/
static int32_t mld_fqscale(int32_t a)
__contract__(
  requires(a > -256*MLDSA_Q && a < 256*MLDSA_Q)
  ensures(return_value > -MLD_INTT_BOUND && return_value < MLD_INTT_BOUND)
)
{
  const int32_t f = 41978; /* mont^2/256 */
  return mld_montgomery_reduce((int64_t)a * f);
  /* TODO: reason about bounds */
}

#include "zetas.inc"


/* mld_ntt_butterfly_block()
 *
 * Computes a block CT butterflies with a fixed twiddle factor,
 * using Montgomery multiplication.
 *
 * Parameters:
 * - r: Pointer to base of polynomial (_not_ the base of butterfly block)
 * - zeta: Twiddle factor to use for the butterfly. This must be in
 *         Montgomery form and signed canonical.
 * - start: Offset to the beginning of the butterfly block
 * - len: Index difference between coefficients subject to a butterfly
 * - bound: Ghost variable describing coefficient bound: Prior to `start`,
 *          coefficients must be bound by `bound + MLDSA_Q`. Post `start`,
 *          they must be bound by `bound`.
 * When this function returns, output coefficients in the index range
 * [start, start+2*len) have bound bumped to `bound + MLDSA_Q`.
 * Example:
 * - start=8, len=4
 *   This would compute the following four butterflies
 *          8     --    12
 *             9    --     13
 *                10   --     14
 *                   11   --     15
 * - start=4, len=2
 *   This would compute the following two butterflies
 *          4 -- 6
 *             5 -- 7
 */

/* Reference: Embedded in `ntt()` in the reference implementation. */
static void mld_ntt_butterfly_block(int32_t r[MLDSA_N], const int32_t zeta,
                                    const unsigned start, const unsigned len,
                                    const int32_t bound)
__contract__(
  requires(start < MLDSA_N)
  requires(1 <= len && len <= MLDSA_N / 2 && start + 2 * len <= MLDSA_N)
  requires(0 <= bound && bound < INT32_MAX - MLDSA_Q)
  requires(-MLDSA_Q_HALF < zeta && zeta < MLDSA_Q_HALF)
  requires(memory_no_alias(r, sizeof(int32_t) * MLDSA_N))
  requires(array_abs_bound(r, 0, start, bound + MLDSA_Q))
  requires(array_abs_bound(r, start, MLDSA_N, bound))
  assigns(memory_slice(r, sizeof(int32_t) * MLDSA_N))
  ensures(array_abs_bound(r, 0, start + 2*len, bound + MLDSA_Q))
  ensures(array_abs_bound(r, start + 2 * len, MLDSA_N, bound)))
{
  /* `bound` is a ghost variable only needed in the CBMC specification */
  unsigned j;
  ((void)bound);
  for (j = start; j < start + len; j++)
  __loop__(
    invariant(start <= j && j <= start + len)
    /*
     * Coefficients are updated in strided pairs, so the bounds for the
     * intermediate states alternate twice between the old and new bound
     */
    invariant(array_abs_bound(r, 0,           j,           bound + MLDSA_Q))
    invariant(array_abs_bound(r, j,           start + len, bound))
    invariant(array_abs_bound(r, start + len, j + len,     bound + MLDSA_Q))
    invariant(array_abs_bound(r, j + len,     MLDSA_N,     bound)))
  {
    int32_t t;
    t = mld_fqmul(r[j + len], zeta);
    r[j + len] = r[j] - t;
    r[j] = r[j] + t;
  }
}

/* mld_ntt_layer()
 *
 * Compute one layer of forward NTT
 *
 * Parameters:
 * - r:     Pointer to base of polynomial
 * - layer: Indicates which layer is being applied.
 */

/* Reference: Embedded in `ntt()` in the reference implementation. */
static void mld_ntt_layer(int32_t r[MLDSA_N], const unsigned layer)
__contract__(
  requires(memory_no_alias(r, sizeof(int32_t) * MLDSA_N))
  requires(1 <= layer && layer <= 8)
  requires(array_abs_bound(r, 0, MLDSA_N, layer * MLDSA_Q))
  assigns(memory_slice(r, sizeof(int32_t) * MLDSA_N))
  ensures(array_abs_bound(r, 0, MLDSA_N, (layer + 1) * MLDSA_Q)))
{
  unsigned start, k, len;
  /* Twiddle factors for layer n are at indices 2^(n-1)..2^n-1. */
  k = 1u << (layer - 1);
  len = MLDSA_N >> layer;
  for (start = 0; start < MLDSA_N; start += 2 * len)
  __loop__(
    invariant(start < MLDSA_N + 2 * len)
    invariant(k <= MLDSA_N)
    invariant(2 * len * k == start + MLDSA_N)
    invariant(array_abs_bound(r, 0, start, layer * MLDSA_Q + MLDSA_Q))
    invariant(array_abs_bound(r, start, MLDSA_N, layer * MLDSA_Q)))
  {
    int32_t zeta = mld_zetas[k++];
    mld_ntt_butterfly_block(r, zeta, start, len, layer * MLDSA_Q);
  }
}


void mld_ntt(int32_t a[MLDSA_N])
{
  unsigned int layer;

  for (layer = 1; layer < 9; layer++)
  __loop__(
    invariant(1 <= layer && layer <= 9)
    invariant(array_abs_bound(a, 0, MLDSA_N, layer * MLDSA_Q))
  )
  {
    mld_ntt_layer(a, layer);
  }

  /* When the loop exits, layer == 9, so the loop invariant  */
  /* directly implies the postcondition in that coefficients */
  /* are bounded in magnitude by 9 * MLDSA_Q                 */
}

/* Reference: Embedded into `invntt_tomont()` in the reference implementation
 * [@REF] */
static void mld_invntt_layer(int32_t r[MLDSA_N], unsigned layer)
__contract__(
  requires(memory_no_alias(r, sizeof(int32_t) * MLDSA_N))
  requires(1 <= layer && layer <= 8)
  requires(array_abs_bound(r, 0, MLDSA_N, (MLDSA_N >> layer) * MLDSA_Q))
  assigns(memory_slice(r, sizeof(int32_t) * MLDSA_N))
  ensures(array_abs_bound(r, 0, MLDSA_N, (MLDSA_N >> (layer - 1)) * MLDSA_Q)))
{
  unsigned start, k, len;
  len = (MLDSA_N >> layer);
  k = (1u << layer) - 1;
  for (start = 0; start < MLDSA_N; start += 2 * len)
  __loop__(
    invariant(start <= MLDSA_N && k <= 255)
    invariant(2 * len * k + start == 2 * MLDSA_N - 2 * len)
    invariant(array_abs_bound(r, 0, start, (MLDSA_N >> (layer - 1)) * MLDSA_Q))
    invariant(array_abs_bound(r, start, MLDSA_N, (MLDSA_N >> layer) * MLDSA_Q)))
  {
    unsigned j;
    int32_t zeta = -mld_zetas[k--];

    for (j = start; j < start + len; j++)
    __loop__(
      invariant(start <= j && j <= start + len)
      invariant(array_abs_bound(r, 0, start, (MLDSA_N >> (layer - 1)) * MLDSA_Q))
      invariant(array_abs_bound(r, start, j, (MLDSA_N >> (layer - 1)) * MLDSA_Q))
      invariant(array_abs_bound(r, j, start + len, (MLDSA_N >> layer) * MLDSA_Q))
      invariant(array_abs_bound(r, start + len, MLDSA_N, (MLDSA_N >> layer) * MLDSA_Q)))
    {
      int32_t t = r[j];
      r[j] = t + r[j + len];
      r[j + len] = t - r[j + len];
      r[j + len] = mld_fqmul(r[j + len], zeta);
    }
  }
}

void mld_invntt_tomont(int32_t a[MLDSA_N])
{
  unsigned int layer, j;

  for (layer = 8; layer >= 1; layer--)
  __loop__(
    invariant(layer <= 8)
    /* Absolute bounds increase from 1Q before layer 8 */
    /* up to 256Q after layer 1                        */
    invariant(array_abs_bound(a, 0, MLDSA_N, (MLDSA_N >> layer) * MLDSA_Q)))
  {
    mld_invntt_layer(a, layer);
  }

  /* Coefficient bounds are now at 256Q. We now scale by mont / 256,
   * i.e., compute the Montgomery multiplication by mont^2 / 256.
   * mont corrects the mont^-1  factor introduced in the basemul.
   * 1/256 performs that scaling of the inverse NTT.
   * The reduced value is bounded by  MLD_INTT_BOUND (4211139) in absolute
   * value.*/
  for (j = 0; j < MLDSA_N; ++j)
  __loop__(
    invariant(j <= MLDSA_N)
    invariant(array_abs_bound(a, 0, j, MLD_INTT_BOUND))
    invariant(array_abs_bound(a, j, MLDSA_N, MLDSA_N * MLDSA_Q))
  )
  {
    a[j] = mld_fqscale(a[j]);
  }
}
