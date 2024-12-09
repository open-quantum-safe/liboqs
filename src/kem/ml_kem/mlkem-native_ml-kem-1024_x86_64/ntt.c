/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdint.h>

#include "arith_backend.h"
#include "debug/debug.h"
#include "ntt.h"
#include "reduce.h"

/* Static namespacing
 * This is to facilitate building multiple instances
 * of mlkem-native (e.g. with varying security levels)
 * within a single compilation unit. */
#define ntt_butterfly_block MLKEM_NAMESPACE(ntt_butterfly_block)
#define ntt_layer MLKEM_NAMESPACE(ntt_layer)
#define invntt_layer MLKEM_NAMESPACE(invntt_layer)
/* End of static namespacing */

#if !defined(MLKEM_USE_NATIVE_NTT)
/*
 * Computes a block CT butterflies with a fixed twiddle factor,
 * using Montgomery multiplication.
 * Parameters:
 * - r: Pointer to base of polynomial (_not_ the base of butterfly block)
 * - root: Twiddle factor to use for the butterfly. This must be in
 *         Montgomery form and signed canonical.
 * - start: Offset to the beginning of the butterfly block
 * - len: Index difference between coefficients subject to a butterfly
 * - bound: Ghost variable describing coefficient bound: Prior to `start`,
 *          coefficients must be bound by `bound + MLKEM_Q`. Post `start`,
 *          they must be bound by `bound`.
 * When this function returns, output coefficients in the index range
 * [start, start+2*len) have bound bumped to `bound + MLKEM_Q`.
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
static void ntt_butterfly_block(int16_t r[MLKEM_N], int16_t zeta, int start,
                                int len, int bound)
__contract__(
  requires(0 <= start && start < MLKEM_N)
  requires(1 <= len && len <= MLKEM_N / 2 && start + 2 * len <= MLKEM_N)
  requires(0 <= bound && bound < INT16_MAX - MLKEM_Q)
  requires(-HALF_Q < zeta && zeta < HALF_Q)
  requires(memory_no_alias(r, sizeof(int16_t) * MLKEM_N))
  requires(array_abs_bound(r, 0, start, bound + MLKEM_Q))
  requires(array_abs_bound(r, start, MLKEM_N, bound))
  assigns(memory_slice(r, sizeof(int16_t) * MLKEM_N))
  ensures(array_abs_bound(r, 0, start + 2*len, bound + MLKEM_Q))
  ensures(array_abs_bound(r, start + 2 * len, MLKEM_N, bound)))
{
  /* `bound` is a ghost variable only needed in the CBMC specification */
  int j;
  ((void)bound);
  for (j = start; j < start + len; j++)
  __loop__(
    invariant(start <= j && j <= start + len)
    /*
     * Coefficients are updated in strided pairs, so the bounds for the
     * intermediate states alternate twice between the old and new bound
     */
    invariant(array_abs_bound(r, 0,           j,           bound + MLKEM_Q))
    invariant(array_abs_bound(r, j,           start + len, bound))
    invariant(array_abs_bound(r, start + len, j + len,     bound + MLKEM_Q))
    invariant(array_abs_bound(r, j + len,     MLKEM_N,     bound)))
  {
    int16_t t;
    t = fqmul(r[j + len], zeta);
    r[j + len] = r[j] - t;
    r[j] = r[j] + t;
  }
}

/*
 *Compute one layer of forward NTT
 * Parameters:
 * - r: Pointer to base of polynomial
 * - len: Stride of butterflies in this layer.
 * - layer: Ghost variable indicating which layer is being applied.
 *          Must match `len` via `len == MLKEM_N >> layer`.
 * Note: `len` could be dropped and computed in the function, but
 *   we are following the structure of the reference NTT from the
 *   official Kyber implementation here, merely adding `layer` as
 *   a ghost variable for the specifications.
 */
static void ntt_layer(int16_t r[MLKEM_N], int len, int layer)
__contract__(
  requires(memory_no_alias(r, sizeof(int16_t) * MLKEM_N))
  requires(1 <= layer && layer <= 7 && len == (MLKEM_N >> layer))
  requires(array_abs_bound(r, 0, MLKEM_N, layer * MLKEM_Q))
  assigns(memory_slice(r, sizeof(int16_t) * MLKEM_N))
  ensures(array_abs_bound(r, 0, MLKEM_N, (layer + 1) * MLKEM_Q)))
{
  int start, k;
  /* `layer` is a ghost variable only needed in the CBMC specification */
  ((void)layer);
  /* Twiddle factors for layer n start at index 2^(layer-1) */
  k = MLKEM_N / (2 * len);
  for (start = 0; start < MLKEM_N; start += 2 * len)
  __loop__(
    invariant(0 <= start && start < MLKEM_N + 2 * len)
    invariant(0 <= k && k <= MLKEM_N / 2 && 2 * len * k == start + MLKEM_N)
    invariant(array_abs_bound(r, 0, start, layer * MLKEM_Q + MLKEM_Q))
    invariant(array_abs_bound(r, start, MLKEM_N, layer * MLKEM_Q)))
  {
    int16_t zeta = zetas[k++];
    ntt_butterfly_block(r, zeta, start, len, layer * MLKEM_Q);
  }
}

/*
 * Compute full forward NTT
 * NOTE: This particular implementation satisfies a much tighter
 * bound on the output coefficients (5*q) than the contractual one (8*q),
 * but this is not needed in the calling code. Should we change the
 * base multiplication strategy to require smaller NTT output bounds,
 * the proof may need strengthening.
 */

MLKEM_NATIVE_INTERNAL_API
void poly_ntt(poly *p)
{
  int len, layer;
  int16_t *r;
  POLY_BOUND_MSG(p, MLKEM_Q, "ref ntt input");
  r = p->coeffs;

  for (len = 128, layer = 1; len >= 2; len >>= 1, layer++)
  __loop__(
    invariant(1 <= layer && layer <= 8 && len == (MLKEM_N >> layer))
    invariant(array_abs_bound(r, 0, MLKEM_N, layer * MLKEM_Q)))
  {
    ntt_layer(r, len, layer);
  }

  /* Check the stronger bound */
  POLY_BOUND_MSG(p, NTT_BOUND, "ref ntt output");
}
#else  /* MLKEM_USE_NATIVE_NTT */

/* Check that bound for native NTT implies contractual bound */
STATIC_ASSERT(NTT_BOUND_NATIVE <= NTT_BOUND, invntt_bound)

MLKEM_NATIVE_INTERNAL_API
void poly_ntt(poly *p)
{
  POLY_BOUND_MSG(p, MLKEM_Q, "native ntt input");
  ntt_native(p);
  POLY_BOUND_MSG(p, NTT_BOUND_NATIVE, "native ntt output");
}
#endif /* MLKEM_USE_NATIVE_NTT */

#if !defined(MLKEM_USE_NATIVE_INTT)

/* Check that bound for reference invNTT implies contractual bound */
#define INVNTT_BOUND_REF (3 * MLKEM_Q / 4)
STATIC_ASSERT(INVNTT_BOUND_REF <= INVNTT_BOUND, invntt_bound)

/* Compute one layer of inverse NTT */
static void invntt_layer(int16_t *r, int len, int layer)
__contract__(
  requires(memory_no_alias(r, sizeof(int16_t) * MLKEM_N))
  requires(2 <= len && len <= 128 && 1 <= layer && layer <= 7)
  requires(len == (1 << (8 - layer)))
  requires(array_abs_bound(r, 0, MLKEM_N, MLKEM_Q))
  assigns(memory_slice(r, sizeof(int16_t) * MLKEM_N))
  ensures(array_abs_bound(r, 0, MLKEM_N, MLKEM_Q)))
{
  int start, k;
  /* `layer` is a ghost variable used only in the specification */
  ((void)layer);
  k = MLKEM_N / len - 1;
  for (start = 0; start < MLKEM_N; start += 2 * len)
  __loop__(
    invariant(array_abs_bound(r, 0, MLKEM_N, MLKEM_Q))
    invariant(0 <= start && start <= MLKEM_N && 0 <= k && k <= 127)
    /* Normalised form of k == MLKEM_N / len - 1 - start / (2 * len) */
    invariant(2 * len * k + start == 2 * MLKEM_N - 2 * len))
  {
    int j;
    int16_t zeta = zetas[k--];
    for (j = start; j < start + len; j++)
    __loop__(
      invariant(start <= j && j <= start + len)
      invariant(0 <= start && start <= MLKEM_N && 0 <= k && k <= 127)
      invariant(array_abs_bound(r, 0, MLKEM_N, MLKEM_Q)))
    {
      int16_t t = r[j];
      r[j] = barrett_reduce(t + r[j + len]);
      r[j + len] = r[j + len] - t;
      r[j + len] = fqmul(r[j + len], zeta);
    }
  }
}

MLKEM_NATIVE_INTERNAL_API
void poly_invntt_tomont(poly *p)
{
  /*
   * Scale input polynomial to account for Montgomery factor
   * and NTT twist. This also brings coefficients down to
   * absolute value < MLKEM_Q.
   */
  int j, len, layer;
  const int16_t f = 1441;
  int16_t *r = p->coeffs;

  for (j = 0; j < MLKEM_N; j++)
  __loop__(
    invariant(0 <= j && j <= MLKEM_N)
    invariant(array_abs_bound(r, 0, j, MLKEM_Q)))
  {
    r[j] = fqmul(r[j], f);
  }

  /* Run the invNTT layers */
  for (len = 2, layer = 7; len <= 128; len <<= 1, layer--)
  __loop__(
    invariant(2 <= len && len <= 256 && 0 <= layer && layer <= 7 && len == (1 << (8 - layer)))
    invariant(array_abs_bound(r, 0, MLKEM_N, MLKEM_Q)))
  {
    invntt_layer(p->coeffs, len, layer);
  }

  POLY_BOUND_MSG(p, INVNTT_BOUND_REF, "ref intt output");
}
#else  /* MLKEM_USE_NATIVE_INTT */

/* Check that bound for native invNTT implies contractual bound */
STATIC_ASSERT(INVNTT_BOUND_NATIVE <= INVNTT_BOUND, invntt_bound)

MLKEM_NATIVE_INTERNAL_API
void poly_invntt_tomont(poly *p)
{
  intt_native(p);
  POLY_BOUND_MSG(p, INVNTT_BOUND_NATIVE, "native intt output");
}
#endif /* MLKEM_USE_NATIVE_INTT */

MLKEM_NATIVE_INTERNAL_API
void basemul_cached(int16_t r[2], const int16_t a[2], const int16_t b[2],
                    int16_t b_cached)
{
  int32_t t0, t1;

  BOUND(a, 2, 4096, "basemul input bound");

  t0 = (int32_t)a[1] * b_cached;
  t0 += (int32_t)a[0] * b[0];
  t1 = (int32_t)a[0] * b[1];
  t1 += (int32_t)a[1] * b[0];

  /* |ti| < 2 * q * 2^15 */
  r[0] = montgomery_reduce(t0);
  r[1] = montgomery_reduce(t1);

  BOUND(r, 2, 2 * MLKEM_Q, "basemul output bound");
}
