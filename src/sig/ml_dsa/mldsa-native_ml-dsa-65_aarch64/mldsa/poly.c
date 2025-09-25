/*
 * Copyright (c) The mldsa-native project authors
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#include <stdint.h>
#include <string.h>

#include "ct.h"
#include "debug.h"
#include "ntt.h"
#include "poly.h"
#include "reduce.h"
#include "rounding.h"
#include "symmetric.h"

MLD_INTERNAL_API
void mld_poly_reduce(mld_poly *a)
{
  unsigned int i;
  mld_assert_bound(a->coeffs, MLDSA_N, INT32_MIN, REDUCE32_DOMAIN_MAX);

  for (i = 0; i < MLDSA_N; ++i)
  __loop__(
    invariant(i <= MLDSA_N)
    invariant(forall(k0, i, MLDSA_N, a->coeffs[k0] == loop_entry(*a).coeffs[k0]))
    invariant(array_bound(a->coeffs, 0, i, -REDUCE32_RANGE_MAX, REDUCE32_RANGE_MAX)))
  {
    a->coeffs[i] = mld_reduce32(a->coeffs[i]);
  }

  mld_assert_bound(a->coeffs, MLDSA_N, -REDUCE32_RANGE_MAX, REDUCE32_RANGE_MAX);
}

MLD_INTERNAL_API
void mld_poly_caddq(mld_poly *a)
{
  unsigned int i;
  mld_assert_abs_bound(a->coeffs, MLDSA_N, MLDSA_Q);

  for (i = 0; i < MLDSA_N; ++i)
  __loop__(
    invariant(i <= MLDSA_N)
    invariant(forall(k0, i, MLDSA_N, a->coeffs[k0] == loop_entry(*a).coeffs[k0]))
    invariant(array_bound(a->coeffs, 0, i, 0, MLDSA_Q))
    )
  {
    a->coeffs[i] = mld_caddq(a->coeffs[i]);
  }

  mld_assert_bound(a->coeffs, MLDSA_N, 0, MLDSA_Q);
}

/* Reference: We use destructive version (output=first input) to avoid
 *            reasoning about aliasing in the CBMC specification */
MLD_INTERNAL_API
void mld_poly_add(mld_poly *r, const mld_poly *b)
{
  unsigned int i;
  for (i = 0; i < MLDSA_N; ++i)
  __loop__(
    assigns(i, memory_slice(r, sizeof(mld_poly)))
    invariant(i <= MLDSA_N)
    invariant(forall(k0, i, MLDSA_N, r->coeffs[k0] == loop_entry(*r).coeffs[k0]))
    invariant(forall(k1, 0, i, r->coeffs[k1] == loop_entry(*r).coeffs[k1] + b->coeffs[k1]))
    invariant(forall(k2, 0, i, r->coeffs[k2] < REDUCE32_DOMAIN_MAX))
    invariant(forall(k2, 0, i, r->coeffs[k2] >= INT32_MIN))
  )
  {
    r->coeffs[i] = r->coeffs[i] + b->coeffs[i];
  }
}

/* Reference: We use destructive version (output=first input) to avoid
 *            reasoning about aliasing in the CBMC specification */
MLD_INTERNAL_API
void mld_poly_sub(mld_poly *r, const mld_poly *b)
{
  unsigned int i;
  mld_assert_abs_bound(b->coeffs, MLDSA_N, MLDSA_Q);
  mld_assert_abs_bound(r->coeffs, MLDSA_N, MLDSA_Q);

  for (i = 0; i < MLDSA_N; ++i)
  __loop__(
    invariant(i <= MLDSA_N)
    invariant(array_bound(r->coeffs, 0, i, INT32_MIN, REDUCE32_DOMAIN_MAX))
    invariant(forall(k0, i, MLDSA_N, r->coeffs[k0] == loop_entry(*r).coeffs[k0]))
  )
  {
    r->coeffs[i] = r->coeffs[i] - b->coeffs[i];
  }

  mld_assert_bound(r->coeffs, MLDSA_N, INT32_MIN, REDUCE32_DOMAIN_MAX);
}

MLD_INTERNAL_API
void mld_poly_shiftl(mld_poly *a)
{
  unsigned int i;
  mld_assert_bound(a->coeffs, MLDSA_N, 0, 1 << 10);

  for (i = 0; i < MLDSA_N; i++)
  __loop__(
    invariant(i <= MLDSA_N)
    invariant(array_bound(a->coeffs, 0, i, 0, MLDSA_Q))
    invariant(forall(k0, i, MLDSA_N, a->coeffs[k0] == loop_entry(*a).coeffs[k0])))
  {
    /* Reference: uses a left shift by MLDSA_D which is undefined behaviour in
     * C90/C99
     */
    a->coeffs[i] *= (1 << MLDSA_D);
  }
  mld_assert_bound(a->coeffs, MLDSA_N, 0, MLDSA_Q);
}

#if !defined(MLD_USE_NATIVE_NTT)
MLD_INTERNAL_API
void mld_poly_ntt(mld_poly *a)
{
  mld_assert_abs_bound(a->coeffs, MLDSA_N, MLDSA_Q);
  mld_ntt(a->coeffs);
  mld_assert_abs_bound(a->coeffs, MLDSA_N, MLD_NTT_BOUND);
}
#else  /* !MLD_USE_NATIVE_NTT */
MLD_INTERNAL_API
void mld_poly_ntt(mld_poly *p)
{
  mld_assert_abs_bound(p->coeffs, MLDSA_N, MLDSA_Q);
  mld_ntt_native(p->coeffs);
  mld_assert_abs_bound(p->coeffs, MLDSA_N, MLD_NTT_BOUND);
}
#endif /* MLD_USE_NATIVE_NTT */

#if !defined(MLD_USE_NATIVE_INTT)
MLD_INTERNAL_API
void mld_poly_invntt_tomont(mld_poly *a)
{
  mld_assert_abs_bound(a->coeffs, MLDSA_N, MLDSA_Q);
  mld_invntt_tomont(a->coeffs);
  mld_assert_abs_bound(a->coeffs, MLDSA_N, MLD_INTT_BOUND);
}
#else  /* !MLD_USE_NATIVE_INTT */
MLD_INTERNAL_API
void mld_poly_invntt_tomont(mld_poly *a)
{
  mld_assert_abs_bound(a->coeffs, MLDSA_N, MLDSA_Q);
  mld_intt_native(a->coeffs);
  mld_assert_abs_bound(a->coeffs, MLDSA_N, MLD_INTT_BOUND);
}
#endif /* MLD_USE_NATIVE_INTT */

MLD_INTERNAL_API
void mld_poly_pointwise_montgomery(mld_poly *c, const mld_poly *a,
                                   const mld_poly *b)
{
  unsigned int i;

  mld_assert_abs_bound(a->coeffs, MLDSA_N, MLD_NTT_BOUND);
  mld_assert_abs_bound(b->coeffs, MLDSA_N, MLD_NTT_BOUND);

  for (i = 0; i < MLDSA_N; ++i)
  __loop__(
    invariant(i <= MLDSA_N)
    invariant(array_abs_bound(c->coeffs, 0, i, MLDSA_Q))
  )
  {
    c->coeffs[i] = mld_montgomery_reduce((int64_t)a->coeffs[i] * b->coeffs[i]);
  }

  mld_assert_abs_bound(c->coeffs, MLDSA_N, MLDSA_Q);
}

MLD_INTERNAL_API
void mld_poly_power2round(mld_poly *a1, mld_poly *a0, const mld_poly *a)
{
  unsigned int i;
  mld_assert_bound(a->coeffs, MLDSA_N, 0, MLDSA_Q);

  for (i = 0; i < MLDSA_N; ++i)
  __loop__(
    assigns(i, memory_slice(a0, sizeof(mld_poly)), memory_slice(a1, sizeof(mld_poly)))
    invariant(i <= MLDSA_N)
    invariant(array_bound(a0->coeffs, 0, i, -(MLD_2_POW_D/2)+1, (MLD_2_POW_D/2)+1))
    invariant(array_bound(a1->coeffs, 0, i, 0, ((MLDSA_Q - 1) / MLD_2_POW_D) + 1))
  )
  {
    mld_power2round(&a0->coeffs[i], &a1->coeffs[i], a->coeffs[i]);
  }

  mld_assert_bound(a0->coeffs, MLDSA_N, -(MLD_2_POW_D / 2) + 1,
                   (MLD_2_POW_D / 2) + 1);
  mld_assert_bound(a1->coeffs, MLDSA_N, 0, ((MLDSA_Q - 1) / MLD_2_POW_D) + 1);
}

MLD_INTERNAL_API
void mld_poly_decompose(mld_poly *a1, mld_poly *a0, const mld_poly *a)
{
#if defined(MLD_USE_NATIVE_POLY_DECOMPOSE_88) && MLDSA_MODE == 2
  /* TODO: proof */
  mld_assert_bound(a->coeffs, MLDSA_N, 0, MLDSA_Q);
  mld_poly_decompose_88_native(a1->coeffs, a0->coeffs, a->coeffs);
#elif defined(MLD_USE_NATIVE_POLY_DECOMPOSE_32) && \
    (MLDSA_MODE == 3 || MLDSA_MODE == 5)
  /* TODO: proof */
  mld_assert_bound(a->coeffs, MLDSA_N, 0, MLDSA_Q);
  mld_poly_decompose_32_native(a1->coeffs, a0->coeffs, a->coeffs);
#else  /* !(MLD_USE_NATIVE_POLY_DECOMPOSE_88 && MLDSA_MODE == 2) &&             \
          MLD_USE_NATIVE_POLY_DECOMPOSE_32 && (MLDSA_MODE == 3 || MLDSA_MODE == \
          5) */
  unsigned int i;
  mld_assert_bound(a->coeffs, MLDSA_N, 0, MLDSA_Q);
  for (i = 0; i < MLDSA_N; ++i)
  __loop__(
    assigns(i, memory_slice(a0, sizeof(mld_poly)), memory_slice(a1, sizeof(mld_poly)))
    invariant(i <= MLDSA_N)
    invariant(array_bound(a1->coeffs, 0, i, 0, (MLDSA_Q-1)/(2*MLDSA_GAMMA2)))
    invariant(array_abs_bound(a0->coeffs, 0, i, MLDSA_GAMMA2+1))
  )
  {
    mld_decompose(&a0->coeffs[i], &a1->coeffs[i], a->coeffs[i]);
  }
#endif /* !(MLD_USE_NATIVE_POLY_DECOMPOSE_88 && MLDSA_MODE == 2) &&            \
          !(MLD_USE_NATIVE_POLY_DECOMPOSE_32 && (MLDSA_MODE == 3 || MLDSA_MODE \
          == 5)) */

  mld_assert_abs_bound(a0->coeffs, MLDSA_N, MLDSA_GAMMA2 + 1);
  mld_assert_bound(a1->coeffs, MLDSA_N, 0, (MLDSA_Q - 1) / (2 * MLDSA_GAMMA2));
}

MLD_INTERNAL_API
unsigned int mld_poly_make_hint(mld_poly *h, const mld_poly *a0,
                                const mld_poly *a1)
{
  unsigned int i, s = 0;

  for (i = 0; i < MLDSA_N; ++i)
  __loop__(
    invariant(i <= MLDSA_N)
    invariant(s <= i)
    invariant(array_bound(h->coeffs, 0, i, 0, 2))
  )
  {
    const unsigned int hint_bit = mld_make_hint(a0->coeffs[i], a1->coeffs[i]);
    h->coeffs[i] = hint_bit;
    s += hint_bit;
  }

  mld_assert(s <= MLDSA_N);
  mld_assert_bound(h->coeffs, MLDSA_N, 0, 2);
  return s;
}

MLD_INTERNAL_API
void mld_poly_use_hint(mld_poly *b, const mld_poly *a, const mld_poly *h)
{
  unsigned int i;
  mld_assert_bound(a->coeffs, MLDSA_N, 0, MLDSA_Q);
  mld_assert_bound(h->coeffs, MLDSA_N, 0, 2);

  for (i = 0; i < MLDSA_N; ++i)
  __loop__(
    invariant(i <= MLDSA_N)
    invariant(array_bound(b->coeffs, 0, i, 0, (MLDSA_Q-1)/(2*MLDSA_GAMMA2)))
  )
  {
    b->coeffs[i] = mld_use_hint(a->coeffs[i], h->coeffs[i]);
  }

  mld_assert_bound(b->coeffs, MLDSA_N, 0, (MLDSA_Q - 1) / (2 * MLDSA_GAMMA2));
}

/* Reference: explicitly checks the bound B to be <= (MLDSA_Q - 1) / 8).
 * This is unnecessary as it's always a compile-time constant.
 * We instead model it as a precondition.
 * Checking the bound is performed using a conditional arguing
 * that it is okay to leak which coefficient violates the bound (while the
 * coefficient itself must remain secret).
 * We instead perform everything in constant-time.
 */
MLD_INTERNAL_API
uint32_t mld_poly_chknorm(const mld_poly *a, int32_t B)
{
  unsigned int i;
  uint32_t t = 0;
  mld_assert_bound(a->coeffs, MLDSA_N, -REDUCE32_RANGE_MAX, REDUCE32_RANGE_MAX);


  for (i = 0; i < MLDSA_N; ++i)
  __loop__(
    invariant(i <= MLDSA_N)
    invariant(t == 0 || t == 0xFFFFFFFF)
    invariant((t == 0) == array_abs_bound(a->coeffs, 0, i, B))
  )
  {
    /* Reference: Leaks which coefficient violates the bound via a conditional.
     * We are more conservative to reduce the number of declassifications in
     * constant-time testing.
     */

    /* if (abs(a[i]) >= B) */
    t |= mld_ct_cmask_neg_i32(B - 1 - mld_ct_abs_i32(a->coeffs[i]));
  }

  return t;
}

/*************************************************
 * Name:        mld_rej_uniform
 *
 * Description: Sample uniformly random coefficients in [0, MLDSA_Q-1] by
 *              performing rejection sampling on array of random bytes.
 *
 * Arguments:   - int32_t *a: pointer to output array (allocated)
 *              - unsigned int target:  requested number of coefficients to
 *sample
 *              - unsigned int offset:  number of coefficients already sampled
 *              - const uint8_t *buf: array of random bytes to sample from
 *              - unsigned int buflen: length of array of random bytes (must be
 *                multiple of 3)
 *
 * Returns number of sampled coefficients. Can be smaller than len if not enough
 * random bytes were given.
 **************************************************/

/* Reference: `mld_rej_uniform()` in the reference implementation [@REF].
 *            - Our signature differs from the reference implementation
 *              in that it adds the offset and always expects the base of the
 *              target buffer. This avoids shifting the buffer base in the
 *              caller, which appears tricky to reason about. */
#define POLY_UNIFORM_NBLOCKS \
  ((768 + STREAM128_BLOCKBYTES - 1) / STREAM128_BLOCKBYTES)
static unsigned int mld_rej_uniform(int32_t *a, unsigned int target,
                                    unsigned int offset, const uint8_t *buf,
                                    unsigned int buflen)
__contract__(
  requires(offset <= target && target <= MLDSA_N)
  requires(buflen <= (POLY_UNIFORM_NBLOCKS * STREAM128_BLOCKBYTES) && buflen % 3 == 0)
  requires(memory_no_alias(a, sizeof(int32_t) * target))
  requires(memory_no_alias(buf, buflen))
  requires(array_bound(a, 0, offset, 0, MLDSA_Q))
  assigns(memory_slice(a, sizeof(int32_t) * target))
  ensures(offset <= return_value && return_value <= target)
  ensures(array_bound(a, 0, return_value, 0, MLDSA_Q))
)
{
  unsigned int ctr, pos;
  uint32_t t;
  mld_assert_bound(a, offset, 0, MLDSA_Q);

/* TODO: CBMC proof based on mld_rej_uniform_native */
#if defined(MLD_USE_NATIVE_REJ_UNIFORM)
  if (offset == 0)
  {
    int ret = mld_rej_uniform_native(a, target, buf, buflen);
    if (ret != -1)
    {
      unsigned res = (unsigned)ret;
      mld_assert_bound(a, res, 0, MLDSA_Q);
      return res;
    }
  }
#endif /* MLD_USE_NATIVE_REJ_UNIFORM */

  ctr = offset;
  pos = 0;
  /* pos + 3 cannot overflow due to the assumption
  buflen <= (POLY_UNIFORM_NBLOCKS * STREAM128_BLOCKBYTES) */
  while (ctr < target && pos + 3 <= buflen)
  __loop__(
    invariant(offset <= ctr && ctr <= target && pos <= buflen)
    invariant(array_bound(a, 0, ctr, 0, MLDSA_Q)))
  {
    t = buf[pos++];
    t |= (uint32_t)buf[pos++] << 8;
    t |= (uint32_t)buf[pos++] << 16;
    t &= 0x7FFFFF;

    if (t < MLDSA_Q)
    {
      a[ctr++] = t;
    }
  }

  mld_assert_bound(a, ctr, 0, MLDSA_Q);

  return ctr;
}

/* Reference: poly_uniform() in the reference implementation [@REF].
 *           - Simplified from reference by removing buffer tail handling
 *             since buflen % 3 = 0 always holds true (STREAM128_BLOCKBYTES =
 *             168).
 *           - Modified rej_uniform interface to track offset directly.
 *           - Pass nonce packed in the extended seed array instead of a third
 *             argument.
 * */
MLD_INTERNAL_API
void mld_poly_uniform(mld_poly *a, const uint8_t seed[MLDSA_SEEDBYTES + 2])
{
  unsigned int ctr;
  unsigned int buflen = POLY_UNIFORM_NBLOCKS * STREAM128_BLOCKBYTES;
  MLD_ALIGN uint8_t buf[POLY_UNIFORM_NBLOCKS * STREAM128_BLOCKBYTES];
  mld_xof128_ctx state;

  mld_xof128_init(&state);
  mld_xof128_absorb_once(&state, seed, MLDSA_SEEDBYTES + 2);
  mld_xof128_squeezeblocks(buf, POLY_UNIFORM_NBLOCKS, &state);

  ctr = mld_rej_uniform(a->coeffs, MLDSA_N, 0, buf, buflen);
  buflen = STREAM128_BLOCKBYTES;
  while (ctr < MLDSA_N)
  __loop__(
    assigns(ctr, state, memory_slice(a, sizeof(mld_poly)), object_whole(buf))
    invariant(ctr <= MLDSA_N)
    invariant(array_bound(a->coeffs, 0, ctr, 0, MLDSA_Q))
    invariant(state.pos <= SHAKE128_RATE)
  )
  {
    mld_xof128_squeezeblocks(buf, 1, &state);
    ctr = mld_rej_uniform(a->coeffs, MLDSA_N, ctr, buf, buflen);
  }
  mld_xof128_release(&state);
  mld_assert_bound(a->coeffs, MLDSA_N, 0, MLDSA_Q);

  /* FIPS 204. Section 3.6.3 Destruction of intermediate values. */
  mld_zeroize(buf, sizeof(buf));
}

MLD_INTERNAL_API
void mld_poly_uniform_4x(mld_poly *vec0, mld_poly *vec1, mld_poly *vec2,
                         mld_poly *vec3,
                         uint8_t seed[4][MLD_ALIGN_UP(MLDSA_SEEDBYTES + 2)])
{
  /* Temporary buffers for XOF output before rejection sampling */
  MLD_ALIGN uint8_t
      buf[4][MLD_ALIGN_UP(POLY_UNIFORM_NBLOCKS * STREAM128_BLOCKBYTES)];

  /* Tracks the number of coefficients we have already sampled */
  unsigned ctr[4];
  mld_xof128_x4_ctx state;
  unsigned buflen;

  mld_xof128_x4_init(&state);
  mld_xof128_x4_absorb(&state, seed, MLDSA_SEEDBYTES + 2);

  /*
   * Initially, squeeze heuristic number of POLY_UNIFORM_NBLOCKS.
   * This should generate the matrix entries with high probability.
   */

  mld_xof128_x4_squeezeblocks(buf, POLY_UNIFORM_NBLOCKS, &state);
  buflen = POLY_UNIFORM_NBLOCKS * STREAM128_BLOCKBYTES;
  ctr[0] = mld_rej_uniform(vec0->coeffs, MLDSA_N, 0, buf[0], buflen);
  ctr[1] = mld_rej_uniform(vec1->coeffs, MLDSA_N, 0, buf[1], buflen);
  ctr[2] = mld_rej_uniform(vec2->coeffs, MLDSA_N, 0, buf[2], buflen);
  ctr[3] = mld_rej_uniform(vec3->coeffs, MLDSA_N, 0, buf[3], buflen);

  /*
   * So long as not all matrix entries have been generated, squeeze
   * one more block a time until we're done.
   */
  buflen = STREAM128_BLOCKBYTES;
  while (ctr[0] < MLDSA_N || ctr[1] < MLDSA_N || ctr[2] < MLDSA_N ||
         ctr[3] < MLDSA_N)
  __loop__(
    assigns(ctr, state, object_whole(buf),
            memory_slice(vec0, sizeof(mld_poly)), memory_slice(vec1, sizeof(mld_poly)),
            memory_slice(vec2, sizeof(mld_poly)), memory_slice(vec3, sizeof(mld_poly)))
    invariant(ctr[0] <= MLDSA_N && ctr[1] <= MLDSA_N)
    invariant(ctr[2] <= MLDSA_N && ctr[3] <= MLDSA_N)
    invariant(array_bound(vec0->coeffs, 0, ctr[0], 0, MLDSA_Q))
    invariant(array_bound(vec1->coeffs, 0, ctr[1], 0, MLDSA_Q))
    invariant(array_bound(vec2->coeffs, 0, ctr[2], 0, MLDSA_Q))
    invariant(array_bound(vec3->coeffs, 0, ctr[3], 0, MLDSA_Q)))
  {
    mld_xof128_x4_squeezeblocks(buf, 1, &state);
    ctr[0] = mld_rej_uniform(vec0->coeffs, MLDSA_N, ctr[0], buf[0], buflen);
    ctr[1] = mld_rej_uniform(vec1->coeffs, MLDSA_N, ctr[1], buf[1], buflen);
    ctr[2] = mld_rej_uniform(vec2->coeffs, MLDSA_N, ctr[2], buf[2], buflen);
    ctr[3] = mld_rej_uniform(vec3->coeffs, MLDSA_N, ctr[3], buf[3], buflen);
  }
  mld_xof128_x4_release(&state);

  mld_assert_bound(vec0->coeffs, MLDSA_N, 0, MLDSA_Q);
  mld_assert_bound(vec1->coeffs, MLDSA_N, 0, MLDSA_Q);
  mld_assert_bound(vec2->coeffs, MLDSA_N, 0, MLDSA_Q);
  mld_assert_bound(vec3->coeffs, MLDSA_N, 0, MLDSA_Q);

  /* FIPS 204. Section 3.6.3 Destruction of intermediate values. */
  mld_zeroize(buf, sizeof(buf));
}

/*************************************************
 * Name:        mld_rej_eta
 *
 * Description: Sample uniformly random coefficients in [-MLDSA_ETA, MLDSA_ETA]
 *by performing rejection sampling on array of random bytes.
 *
 * Arguments:   - int32_t *a:          pointer to output array (allocated)
 *              - unsigned int target: requested number of coefficients to
 *sample
 *              - unsigned int offset: number of coefficients already sampled
 *              - const uint8_t *buf:  array of random bytes to sample from
 *              - unsigned int buflen: length of array of random bytes
 *
 * Returns number of sampled coefficients. Can be smaller than target if not
 *enough random bytes were given.
 **************************************************/

/* Reference: `mld_rej_eta()` in the reference implementation [@REF].
 *            - Our signature differs from the reference implementation
 *              in that it adds the offset and always expects the base of the
 *              target buffer. This avoids shifting the buffer base in the
 *              caller, which appears tricky to reason about. */
#if MLDSA_ETA == 2
/*
 * Sampling 256 coefficients mod 15 using rejection sampling from 4 bits.
 * Expected number of required bytes: (256 * (16/15))/2 = 136.5 bytes.
 * We sample 1 block (=136 bytes) of SHAKE256_RATE output initially.
 * Sampling 2 blocks initially results in slightly worse performance.
 */
#define POLY_UNIFORM_ETA_NBLOCKS 1
#elif MLDSA_ETA == 4
/*
 * Sampling 256 coefficients mod 9 using rejection sampling from 4 bits.
 * Expected number of required bytes: (256 * (16/9))/2 = 227.5 bytes.
 * We sample 2 blocks (=272 bytes) of SHAKE256_RATE output initially.
 */
#define POLY_UNIFORM_ETA_NBLOCKS 2
#else /* MLDSA_ETA == 4 */
#error "Invalid value of MLDSA_ETA"
#endif /* MLDSA_ETA != 2 && MLDSA_ETA != 4 */
static unsigned int mld_rej_eta(int32_t *a, unsigned int target,
                                unsigned int offset, const uint8_t *buf,
                                unsigned int buflen)
__contract__(
  requires(offset <= target && target <= MLDSA_N)
  requires(buflen <= (POLY_UNIFORM_ETA_NBLOCKS * STREAM256_BLOCKBYTES))
  requires(memory_no_alias(a, sizeof(int32_t) * target))
  requires(memory_no_alias(buf, buflen))
  requires(array_abs_bound(a, 0, offset, MLDSA_ETA + 1))
  assigns(memory_slice(a, sizeof(int32_t) * target))
  ensures(offset <= return_value && return_value <= target)
  ensures(array_abs_bound(a, 0, return_value, MLDSA_ETA + 1))
)
{
  unsigned int ctr, pos;
  int t_valid;
  uint32_t t0, t1;
  mld_assert_abs_bound(a, offset, MLDSA_ETA + 1);

/* TODO: CBMC proof based on mld_rej_uniform_eta2_native */
#if MLDSA_ETA == 2 && defined(MLD_USE_NATIVE_REJ_UNIFORM_ETA2)
  if (offset == 0)
  {
    int ret = mld_rej_uniform_eta2_native(a, target, buf, buflen);
    if (ret != -1)
    {
      unsigned res = (unsigned)ret;
      mld_assert_abs_bound(a, res, MLDSA_ETA + 1);
      return res;
    }
  }
/* TODO: CBMC proof based on mld_rej_uniform_eta4_native */
#elif MLDSA_ETA == 4 && defined(MLD_USE_NATIVE_REJ_UNIFORM_ETA4)
  if (offset == 0)
  {
    int ret = mld_rej_uniform_eta4_native(a, target, buf, buflen);
    if (ret != -1)
    {
      unsigned res = (unsigned)ret;
      mld_assert_abs_bound(a, res, MLDSA_ETA + 1);
      return res;
    }
  }
#endif /* !(MLDSA_ETA == 2 && MLD_USE_NATIVE_REJ_UNIFORM_ETA2) && MLDSA_ETA == \
          4 && MLD_USE_NATIVE_REJ_UNIFORM_ETA4 */

  ctr = offset;
  pos = 0;
  while (ctr < target && pos < buflen)
  __loop__(
    invariant(offset <= ctr && ctr <= target && pos <= buflen)
    invariant(array_abs_bound(a, 0, ctr, MLDSA_ETA + 1))
  )
  {
    t0 = buf[pos] & 0x0F;
    t1 = buf[pos++] >> 4;

    /* Constant time: The inputs and outputs to the rejection sampling are
     * secret. However, it is fine to leak which coefficients have been
     * rejected. For constant-time testing, we declassify the result of
     * the comparison.
     */
#if MLDSA_ETA == 2
    t_valid = t0 < 15;
    MLD_CT_TESTING_DECLASSIFY(&t_valid, sizeof(int));
    if (t_valid) /* t0 < 15 */
    {
      t0 = t0 - (205 * t0 >> 10) * 5;
      a[ctr++] = 2 - (int32_t)t0;
    }
    t_valid = t1 < 15;
    MLD_CT_TESTING_DECLASSIFY(&t_valid, sizeof(int));
    if (t_valid && ctr < target) /* t1 < 15 */
    {
      t1 = t1 - (205 * t1 >> 10) * 5;
      a[ctr++] = 2 - (int32_t)t1;
    }
#elif MLDSA_ETA == 4
    t_valid = t0 < 9;
    MLD_CT_TESTING_DECLASSIFY(&t_valid, sizeof(int));
    if (t_valid) /* t0 < 9 */
    {
      a[ctr++] = 4 - (int32_t)t0;
    }
    t_valid = t1 < 9; /* t1 < 9 */
    MLD_CT_TESTING_DECLASSIFY(&t_valid, sizeof(int));
    if (t_valid && ctr < target)
    {
      a[ctr++] = 4 - (int32_t)t1;
    }
#else /* MLDSA_ETA == 4 */
#error "Invalid value of MLDSA_ETA"
#endif /* MLDSA_ETA != 2 && MLDSA_ETA != 4 */
  }

  mld_assert_abs_bound(a, ctr, MLDSA_ETA + 1);

  return ctr;
}

MLD_INTERNAL_API
void mld_poly_uniform_eta_4x(mld_poly *r0, mld_poly *r1, mld_poly *r2,
                             mld_poly *r3, const uint8_t seed[MLDSA_CRHBYTES],
                             uint8_t nonce0, uint8_t nonce1, uint8_t nonce2,
                             uint8_t nonce3)
{
  /* Temporary buffers for XOF output before rejection sampling */
  MLD_ALIGN uint8_t
      buf[4][MLD_ALIGN_UP(POLY_UNIFORM_ETA_NBLOCKS * STREAM256_BLOCKBYTES)];

  MLD_ALIGN uint8_t extseed[4][MLD_ALIGN_UP(MLDSA_CRHBYTES + 2)];

  /* Tracks the number of coefficients we have already sampled */
  unsigned ctr[4];
  mld_xof256_x4_ctx state;
  unsigned buflen;

  mld_memcpy(extseed[0], seed, MLDSA_CRHBYTES);
  mld_memcpy(extseed[1], seed, MLDSA_CRHBYTES);
  mld_memcpy(extseed[2], seed, MLDSA_CRHBYTES);
  mld_memcpy(extseed[3], seed, MLDSA_CRHBYTES);
  extseed[0][MLDSA_CRHBYTES] = nonce0;
  extseed[1][MLDSA_CRHBYTES] = nonce1;
  extseed[2][MLDSA_CRHBYTES] = nonce2;
  extseed[3][MLDSA_CRHBYTES] = nonce3;
  extseed[0][MLDSA_CRHBYTES + 1] = 0;
  extseed[1][MLDSA_CRHBYTES + 1] = 0;
  extseed[2][MLDSA_CRHBYTES + 1] = 0;
  extseed[3][MLDSA_CRHBYTES + 1] = 0;

  mld_xof256_x4_init(&state);
  mld_xof256_x4_absorb(&state, extseed, MLDSA_CRHBYTES + 2);

  /*
   * Initially, squeeze heuristic number of POLY_UNIFORM_ETA_NBLOCKS.
   * This should generate the coefficients with high probability.
   */
  mld_xof256_x4_squeezeblocks(buf, POLY_UNIFORM_ETA_NBLOCKS, &state);
  buflen = POLY_UNIFORM_ETA_NBLOCKS * STREAM256_BLOCKBYTES;

  ctr[0] = mld_rej_eta(r0->coeffs, MLDSA_N, 0, buf[0], buflen);
  ctr[1] = mld_rej_eta(r1->coeffs, MLDSA_N, 0, buf[1], buflen);
  ctr[2] = mld_rej_eta(r2->coeffs, MLDSA_N, 0, buf[2], buflen);
  ctr[3] = mld_rej_eta(r3->coeffs, MLDSA_N, 0, buf[3], buflen);

  /*
   * So long as not all entries have been generated, squeeze
   * one more block a time until we're done.
   */
  buflen = STREAM256_BLOCKBYTES;
  while (ctr[0] < MLDSA_N || ctr[1] < MLDSA_N || ctr[2] < MLDSA_N ||
         ctr[3] < MLDSA_N)
  __loop__(
    assigns(ctr, state, memory_slice(r0, sizeof(mld_poly)),
            memory_slice(r1, sizeof(mld_poly)), memory_slice(r2, sizeof(mld_poly)),
            memory_slice(r3, sizeof(mld_poly)), object_whole(buf[0]),
            object_whole(buf[1]), object_whole(buf[2]),
            object_whole(buf[3]))
    invariant(ctr[0] <= MLDSA_N && ctr[1] <= MLDSA_N)
    invariant(ctr[2] <= MLDSA_N && ctr[3] <= MLDSA_N)
    invariant(array_abs_bound(r0->coeffs, 0, ctr[0], MLDSA_ETA + 1))
    invariant(array_abs_bound(r1->coeffs, 0, ctr[1], MLDSA_ETA + 1))
    invariant(array_abs_bound(r2->coeffs, 0, ctr[2], MLDSA_ETA + 1))
    invariant(array_abs_bound(r3->coeffs, 0, ctr[3], MLDSA_ETA + 1)))
  {
    mld_xof256_x4_squeezeblocks(buf, 1, &state);
    ctr[0] = mld_rej_eta(r0->coeffs, MLDSA_N, ctr[0], buf[0], buflen);
    ctr[1] = mld_rej_eta(r1->coeffs, MLDSA_N, ctr[1], buf[1], buflen);
    ctr[2] = mld_rej_eta(r2->coeffs, MLDSA_N, ctr[2], buf[2], buflen);
    ctr[3] = mld_rej_eta(r3->coeffs, MLDSA_N, ctr[3], buf[3], buflen);
  }

  mld_xof256_x4_release(&state);

  mld_assert_abs_bound(r0->coeffs, MLDSA_N, MLDSA_ETA + 1);
  mld_assert_abs_bound(r1->coeffs, MLDSA_N, MLDSA_ETA + 1);
  mld_assert_abs_bound(r2->coeffs, MLDSA_N, MLDSA_ETA + 1);
  mld_assert_abs_bound(r3->coeffs, MLDSA_N, MLDSA_ETA + 1);

  /* FIPS 204. Section 3.6.3 Destruction of intermediate values. */
  mld_zeroize(buf, sizeof(buf));
  mld_zeroize(extseed, sizeof(extseed));
}


#define POLY_UNIFORM_GAMMA1_NBLOCKS \
  ((MLDSA_POLYZ_PACKEDBYTES + STREAM256_BLOCKBYTES - 1) / STREAM256_BLOCKBYTES)
MLD_INTERNAL_API
void mld_poly_uniform_gamma1(mld_poly *a, const uint8_t seed[MLDSA_CRHBYTES],
                             uint16_t nonce)
{
  MLD_ALIGN uint8_t buf[POLY_UNIFORM_GAMMA1_NBLOCKS * STREAM256_BLOCKBYTES];
  MLD_ALIGN uint8_t extseed[MLDSA_CRHBYTES + 2];
  mld_xof256_ctx state;

  mld_memcpy(extseed, seed, MLDSA_CRHBYTES);
  extseed[MLDSA_CRHBYTES] = nonce & 0xFF;
  extseed[MLDSA_CRHBYTES + 1] = nonce >> 8;

  mld_xof256_init(&state);
  mld_xof256_absorb_once(&state, extseed, MLDSA_CRHBYTES + 2);

  mld_xof256_squeezeblocks(buf, POLY_UNIFORM_GAMMA1_NBLOCKS, &state);
  mld_polyz_unpack(a, buf);

  mld_xof256_release(&state);

  mld_assert_bound(a->coeffs, MLDSA_N, -(MLDSA_GAMMA1 - 1), MLDSA_GAMMA1 + 1);

  /* FIPS 204. Section 3.6.3 Destruction of intermediate values. */
  mld_zeroize(buf, sizeof(buf));
  mld_zeroize(extseed, sizeof(extseed));
}

MLD_INTERNAL_API
void mld_poly_uniform_gamma1_4x(mld_poly *r0, mld_poly *r1, mld_poly *r2,
                                mld_poly *r3,
                                const uint8_t seed[MLDSA_CRHBYTES],
                                uint16_t nonce0, uint16_t nonce1,
                                uint16_t nonce2, uint16_t nonce3)
{
  /* Temporary buffers for XOF output before rejection sampling */
  MLD_ALIGN uint8_t
      buf[4][MLD_ALIGN_UP(POLY_UNIFORM_GAMMA1_NBLOCKS * STREAM256_BLOCKBYTES)];

  MLD_ALIGN uint8_t extseed[4][MLD_ALIGN_UP(MLDSA_CRHBYTES + 2)];

  /* Tracks the number of coefficients we have already sampled */
  mld_xof256_x4_ctx state;

  mld_memcpy(extseed[0], seed, MLDSA_CRHBYTES);
  mld_memcpy(extseed[1], seed, MLDSA_CRHBYTES);
  mld_memcpy(extseed[2], seed, MLDSA_CRHBYTES);
  mld_memcpy(extseed[3], seed, MLDSA_CRHBYTES);
  extseed[0][MLDSA_CRHBYTES] = nonce0 & 0xFF;
  extseed[1][MLDSA_CRHBYTES] = nonce1 & 0xFF;
  extseed[2][MLDSA_CRHBYTES] = nonce2 & 0xFF;
  extseed[3][MLDSA_CRHBYTES] = nonce3 & 0xFF;
  extseed[0][MLDSA_CRHBYTES + 1] = nonce0 >> 8;
  extseed[1][MLDSA_CRHBYTES + 1] = nonce1 >> 8;
  extseed[2][MLDSA_CRHBYTES + 1] = nonce2 >> 8;
  extseed[3][MLDSA_CRHBYTES + 1] = nonce3 >> 8;

  mld_xof256_x4_init(&state);
  mld_xof256_x4_absorb(&state, extseed, MLDSA_CRHBYTES + 2);
  mld_xof256_x4_squeezeblocks(buf, POLY_UNIFORM_GAMMA1_NBLOCKS, &state);

  mld_polyz_unpack(r0, buf[0]);
  mld_polyz_unpack(r1, buf[1]);
  mld_polyz_unpack(r2, buf[2]);
  mld_polyz_unpack(r3, buf[3]);
  mld_xof256_x4_release(&state);

  mld_assert_bound(r0->coeffs, MLDSA_N, -(MLDSA_GAMMA1 - 1), MLDSA_GAMMA1 + 1);
  mld_assert_bound(r1->coeffs, MLDSA_N, -(MLDSA_GAMMA1 - 1), MLDSA_GAMMA1 + 1);
  mld_assert_bound(r2->coeffs, MLDSA_N, -(MLDSA_GAMMA1 - 1), MLDSA_GAMMA1 + 1);
  mld_assert_bound(r3->coeffs, MLDSA_N, -(MLDSA_GAMMA1 - 1), MLDSA_GAMMA1 + 1);

  /* FIPS 204. Section 3.6.3 Destruction of intermediate values. */
  mld_zeroize(buf, sizeof(buf));
  mld_zeroize(extseed, sizeof(extseed));
}

MLD_INTERNAL_API
void mld_poly_challenge(mld_poly *c, const uint8_t seed[MLDSA_CTILDEBYTES])
{
  unsigned int i, j, pos;
  uint64_t signs;
  uint64_t offset;
  MLD_ALIGN uint8_t buf[SHAKE256_RATE];
  mld_shake256ctx state;

  mld_shake256_init(&state);
  mld_shake256_absorb(&state, seed, MLDSA_CTILDEBYTES);
  mld_shake256_finalize(&state);
  mld_shake256_squeeze(buf, SHAKE256_RATE, &state);

  /* Convert the first 8 bytes of buf[] into an unsigned 64-bit value.   */
  /* Each bit of that dictates the sign of the resulting challenge value */
  signs = 0;
  for (i = 0; i < 8; ++i)
  __loop__(
    assigns(i, signs)
    invariant(i <= 8)
  )
  {
    signs |= (uint64_t)buf[i] << 8 * i;
  }
  pos = 8;

  mld_memset(c, 0, sizeof(mld_poly));

  for (i = MLDSA_N - MLDSA_TAU; i < MLDSA_N; ++i)
  __loop__(
    assigns(i, j, object_whole(buf), state, pos, memory_slice(c, sizeof(mld_poly)), signs)
    invariant(i >= MLDSA_N - MLDSA_TAU)
    invariant(i <= MLDSA_N)
    invariant(pos >= 1)
    invariant(pos <= SHAKE256_RATE)
    invariant(array_bound(c->coeffs, 0, MLDSA_N, -1, 2))
    invariant(state.pos <= SHAKE256_RATE)
  )
  {
    do
    __loop__(
      assigns(j, object_whole(buf), state, pos)
      invariant(state.pos <= SHAKE256_RATE)
    )
    {
      if (pos >= SHAKE256_RATE)
      {
        mld_shake256_squeeze(buf, SHAKE256_RATE, &state);
        pos = 0;
      }
      j = buf[pos++];
    } while (j > i);

    c->coeffs[i] = c->coeffs[j];

    /* Reference: Compute coefficent value here in two steps to */
    /* mixinf unsigned and signed arithmetic with implicit      */
    /* conversions, and so that CBMC can keep track of ranges   */
    /* to complete type-safety proof here.                      */

    /* The least-significant bit of signs tells us if we want -1 or +1 */
    offset = 2 * (signs & 1);

    /* offset has value 0 or 2 here, so (1 - (int32_t) offset) has
     * value -1 or +1 */
    c->coeffs[j] = 1 - (int32_t)offset;

    /* Move to the next bit of signs for next time */
    signs >>= 1;
  }

  mld_assert_bound(c->coeffs, MLDSA_N, -1, 2);
  mld_shake256_release(&state);

  /* FIPS 204. Section 3.6.3 Destruction of intermediate values. */
  mld_zeroize(buf, sizeof(buf));
  mld_zeroize(&signs, sizeof(signs));
}

MLD_INTERNAL_API
void mld_polyeta_pack(uint8_t *r, const mld_poly *a)
{
  unsigned int i;
  uint8_t t[8];

  mld_assert_abs_bound(a->coeffs, MLDSA_N, MLDSA_ETA + 1);

#if MLDSA_ETA == 2
  for (i = 0; i < MLDSA_N / 8; ++i)
  __loop__(
    invariant(i <= MLDSA_N/8))
  {
    t[0] = MLDSA_ETA - a->coeffs[8 * i + 0];
    t[1] = MLDSA_ETA - a->coeffs[8 * i + 1];
    t[2] = MLDSA_ETA - a->coeffs[8 * i + 2];
    t[3] = MLDSA_ETA - a->coeffs[8 * i + 3];
    t[4] = MLDSA_ETA - a->coeffs[8 * i + 4];
    t[5] = MLDSA_ETA - a->coeffs[8 * i + 5];
    t[6] = MLDSA_ETA - a->coeffs[8 * i + 6];
    t[7] = MLDSA_ETA - a->coeffs[8 * i + 7];

    r[3 * i + 0] = ((t[0] >> 0) | (t[1] << 3) | (t[2] << 6)) & 0xFF;
    r[3 * i + 1] =
        ((t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7)) & 0xFF;
    r[3 * i + 2] = ((t[5] >> 1) | (t[6] << 2) | (t[7] << 5)) & 0xFF;
  }
#elif MLDSA_ETA == 4
  for (i = 0; i < MLDSA_N / 2; ++i)
  __loop__(
    invariant(i <= MLDSA_N/2))
  {
    t[0] = MLDSA_ETA - a->coeffs[2 * i + 0];
    t[1] = MLDSA_ETA - a->coeffs[2 * i + 1];
    r[i] = t[0] | (t[1] << 4);
  }
#else /* MLDSA_ETA == 4 */
#error "Invalid value of MLDSA_ETA"
#endif /* MLDSA_ETA != 2 && MLDSA_ETA != 4 */
}

void mld_polyeta_unpack(mld_poly *r, const uint8_t *a)
{
  unsigned int i;

#if MLDSA_ETA == 2
  for (i = 0; i < MLDSA_N / 8; ++i)
  __loop__(
    invariant(i <= MLDSA_N/8)
    invariant(array_bound(r->coeffs, 0, i*8, -5, MLDSA_ETA + 1)))
  {
    r->coeffs[8 * i + 0] = (a[3 * i + 0] >> 0) & 7;
    r->coeffs[8 * i + 1] = (a[3 * i + 0] >> 3) & 7;
    r->coeffs[8 * i + 2] = ((a[3 * i + 0] >> 6) | (a[3 * i + 1] << 2)) & 7;
    r->coeffs[8 * i + 3] = (a[3 * i + 1] >> 1) & 7;
    r->coeffs[8 * i + 4] = (a[3 * i + 1] >> 4) & 7;
    r->coeffs[8 * i + 5] = ((a[3 * i + 1] >> 7) | (a[3 * i + 2] << 1)) & 7;
    r->coeffs[8 * i + 6] = (a[3 * i + 2] >> 2) & 7;
    r->coeffs[8 * i + 7] = (a[3 * i + 2] >> 5) & 7;

    r->coeffs[8 * i + 0] = MLDSA_ETA - r->coeffs[8 * i + 0];
    r->coeffs[8 * i + 1] = MLDSA_ETA - r->coeffs[8 * i + 1];
    r->coeffs[8 * i + 2] = MLDSA_ETA - r->coeffs[8 * i + 2];
    r->coeffs[8 * i + 3] = MLDSA_ETA - r->coeffs[8 * i + 3];
    r->coeffs[8 * i + 4] = MLDSA_ETA - r->coeffs[8 * i + 4];
    r->coeffs[8 * i + 5] = MLDSA_ETA - r->coeffs[8 * i + 5];
    r->coeffs[8 * i + 6] = MLDSA_ETA - r->coeffs[8 * i + 6];
    r->coeffs[8 * i + 7] = MLDSA_ETA - r->coeffs[8 * i + 7];
  }
#elif MLDSA_ETA == 4
  for (i = 0; i < MLDSA_N / 2; ++i)
  __loop__(
    invariant(i <= MLDSA_N/2)
    invariant(array_bound(r->coeffs, 0, i*2, -11, MLDSA_ETA + 1)))
  {
    r->coeffs[2 * i + 0] = a[i] & 0x0F;
    r->coeffs[2 * i + 1] = a[i] >> 4;
    r->coeffs[2 * i + 0] = MLDSA_ETA - r->coeffs[2 * i + 0];
    r->coeffs[2 * i + 1] = MLDSA_ETA - r->coeffs[2 * i + 1];
  }
#else /* MLDSA_ETA == 4 */
#error "Invalid value of MLDSA_ETA"
#endif /* MLDSA_ETA != 2 && MLDSA_ETA != 4 */

  mld_assert_bound(r->coeffs, MLDSA_N, MLD_POLYETA_UNPACK_LOWER_BOUND,
                   MLDSA_ETA + 1);
}

MLD_INTERNAL_API
void mld_polyt1_pack(uint8_t *r, const mld_poly *a)
{
  unsigned int i;
  mld_assert_bound(a->coeffs, MLDSA_N, 0, 1 << 10);

  for (i = 0; i < MLDSA_N / 4; ++i)
  __loop__(
    invariant(i <= MLDSA_N/4))
  {
    r[5 * i + 0] = (a->coeffs[4 * i + 0] >> 0) & 0xFF;
    r[5 * i + 1] =
        ((a->coeffs[4 * i + 0] >> 8) | (a->coeffs[4 * i + 1] << 2)) & 0xFF;
    r[5 * i + 2] =
        ((a->coeffs[4 * i + 1] >> 6) | (a->coeffs[4 * i + 2] << 4)) & 0xFF;
    r[5 * i + 3] =
        ((a->coeffs[4 * i + 2] >> 4) | (a->coeffs[4 * i + 3] << 6)) & 0xFF;
    r[5 * i + 4] = (a->coeffs[4 * i + 3] >> 2) & 0xFF;
  }
}

MLD_INTERNAL_API
void mld_polyt1_unpack(mld_poly *r, const uint8_t *a)
{
  unsigned int i;

  for (i = 0; i < MLDSA_N / 4; ++i)
  __loop__(
    invariant(i <= MLDSA_N/4)
    invariant(array_bound(r->coeffs, 0, i*4, 0, 1 << 10)))
  {
    r->coeffs[4 * i + 0] =
        ((a[5 * i + 0] >> 0) | ((uint32_t)a[5 * i + 1] << 8)) & 0x3FF;
    r->coeffs[4 * i + 1] =
        ((a[5 * i + 1] >> 2) | ((uint32_t)a[5 * i + 2] << 6)) & 0x3FF;
    r->coeffs[4 * i + 2] =
        ((a[5 * i + 2] >> 4) | ((uint32_t)a[5 * i + 3] << 4)) & 0x3FF;
    r->coeffs[4 * i + 3] =
        ((a[5 * i + 3] >> 6) | ((uint32_t)a[5 * i + 4] << 2)) & 0x3FF;
  }

  mld_assert_bound(r->coeffs, MLDSA_N, 0, 1 << 10);
}

MLD_INTERNAL_API
void mld_polyt0_pack(uint8_t *r, const mld_poly *a)
{
  unsigned int i;
  uint32_t t[8];

  mld_assert_bound(a->coeffs, MLDSA_N, -(1 << (MLDSA_D - 1)) + 1,
                   (1 << (MLDSA_D - 1)) + 1);

  for (i = 0; i < MLDSA_N / 8; ++i)
  __loop__(
    invariant(i <= MLDSA_N/8))
  {
    t[0] = (1 << (MLDSA_D - 1)) - a->coeffs[8 * i + 0];
    t[1] = (1 << (MLDSA_D - 1)) - a->coeffs[8 * i + 1];
    t[2] = (1 << (MLDSA_D - 1)) - a->coeffs[8 * i + 2];
    t[3] = (1 << (MLDSA_D - 1)) - a->coeffs[8 * i + 3];
    t[4] = (1 << (MLDSA_D - 1)) - a->coeffs[8 * i + 4];
    t[5] = (1 << (MLDSA_D - 1)) - a->coeffs[8 * i + 5];
    t[6] = (1 << (MLDSA_D - 1)) - a->coeffs[8 * i + 6];
    t[7] = (1 << (MLDSA_D - 1)) - a->coeffs[8 * i + 7];

    r[13 * i + 0] = (t[0]) & 0xFF;
    r[13 * i + 1] = (t[0] >> 8) & 0xFF;
    r[13 * i + 1] |= (t[1] << 5) & 0xFF;
    r[13 * i + 2] = (t[1] >> 3) & 0xFF;
    r[13 * i + 3] = (t[1] >> 11) & 0xFF;
    r[13 * i + 3] |= (t[2] << 2) & 0xFF;
    r[13 * i + 4] = (t[2] >> 6) & 0xFF;
    r[13 * i + 4] |= (t[3] << 7) & 0xFF;
    r[13 * i + 5] = (t[3] >> 1) & 0xFF;
    r[13 * i + 6] = (t[3] >> 9) & 0xFF;
    r[13 * i + 6] |= (t[4] << 4) & 0xFF;
    r[13 * i + 7] = (t[4] >> 4) & 0xFF;
    r[13 * i + 8] = (t[4] >> 12) & 0xFF;
    r[13 * i + 8] |= (t[5] << 1) & 0xFF;
    r[13 * i + 9] = (t[5] >> 7) & 0xFF;
    r[13 * i + 9] |= (t[6] << 6) & 0xFF;
    r[13 * i + 10] = (t[6] >> 2) & 0xFF;
    r[13 * i + 11] = (t[6] >> 10) & 0xFF;
    r[13 * i + 11] |= (t[7] << 3) & 0xFF;
    r[13 * i + 12] = (t[7] >> 5) & 0xFF;
  }
}

MLD_INTERNAL_API
void mld_polyt0_unpack(mld_poly *r, const uint8_t *a)
{
  unsigned int i;

  for (i = 0; i < MLDSA_N / 8; ++i)
  __loop__(
    invariant(i <= MLDSA_N/8)
    invariant(array_bound(r->coeffs, 0, i*8, -(1<<(MLDSA_D-1)) + 1, (1<<(MLDSA_D-1)) + 1)))
  {
    r->coeffs[8 * i + 0] = a[13 * i + 0];
    r->coeffs[8 * i + 0] |= (uint32_t)a[13 * i + 1] << 8;
    r->coeffs[8 * i + 0] &= 0x1FFF;

    r->coeffs[8 * i + 1] = a[13 * i + 1] >> 5;
    r->coeffs[8 * i + 1] |= (uint32_t)a[13 * i + 2] << 3;
    r->coeffs[8 * i + 1] |= (uint32_t)a[13 * i + 3] << 11;
    r->coeffs[8 * i + 1] &= 0x1FFF;

    r->coeffs[8 * i + 2] = a[13 * i + 3] >> 2;
    r->coeffs[8 * i + 2] |= (uint32_t)a[13 * i + 4] << 6;
    r->coeffs[8 * i + 2] &= 0x1FFF;

    r->coeffs[8 * i + 3] = a[13 * i + 4] >> 7;
    r->coeffs[8 * i + 3] |= (uint32_t)a[13 * i + 5] << 1;
    r->coeffs[8 * i + 3] |= (uint32_t)a[13 * i + 6] << 9;
    r->coeffs[8 * i + 3] &= 0x1FFF;

    r->coeffs[8 * i + 4] = a[13 * i + 6] >> 4;
    r->coeffs[8 * i + 4] |= (uint32_t)a[13 * i + 7] << 4;
    r->coeffs[8 * i + 4] |= (uint32_t)a[13 * i + 8] << 12;
    r->coeffs[8 * i + 4] &= 0x1FFF;

    r->coeffs[8 * i + 5] = a[13 * i + 8] >> 1;
    r->coeffs[8 * i + 5] |= (uint32_t)a[13 * i + 9] << 7;
    r->coeffs[8 * i + 5] &= 0x1FFF;

    r->coeffs[8 * i + 6] = a[13 * i + 9] >> 6;
    r->coeffs[8 * i + 6] |= (uint32_t)a[13 * i + 10] << 2;
    r->coeffs[8 * i + 6] |= (uint32_t)a[13 * i + 11] << 10;
    r->coeffs[8 * i + 6] &= 0x1FFF;

    r->coeffs[8 * i + 7] = a[13 * i + 11] >> 3;
    r->coeffs[8 * i + 7] |= (uint32_t)a[13 * i + 12] << 5;
    r->coeffs[8 * i + 7] &= 0x1FFF;

    r->coeffs[8 * i + 0] = (1 << (MLDSA_D - 1)) - r->coeffs[8 * i + 0];
    r->coeffs[8 * i + 1] = (1 << (MLDSA_D - 1)) - r->coeffs[8 * i + 1];
    r->coeffs[8 * i + 2] = (1 << (MLDSA_D - 1)) - r->coeffs[8 * i + 2];
    r->coeffs[8 * i + 3] = (1 << (MLDSA_D - 1)) - r->coeffs[8 * i + 3];
    r->coeffs[8 * i + 4] = (1 << (MLDSA_D - 1)) - r->coeffs[8 * i + 4];
    r->coeffs[8 * i + 5] = (1 << (MLDSA_D - 1)) - r->coeffs[8 * i + 5];
    r->coeffs[8 * i + 6] = (1 << (MLDSA_D - 1)) - r->coeffs[8 * i + 6];
    r->coeffs[8 * i + 7] = (1 << (MLDSA_D - 1)) - r->coeffs[8 * i + 7];
  }

  mld_assert_bound(r->coeffs, MLDSA_N, -(1 << (MLDSA_D - 1)) + 1,
                   (1 << (MLDSA_D - 1)) + 1);
}

MLD_INTERNAL_API
void mld_polyz_pack(uint8_t *r, const mld_poly *a)
{
  unsigned int i;
  uint32_t t[4];

  mld_assert_bound(a->coeffs, MLDSA_N, -(MLDSA_GAMMA1 - 1), MLDSA_GAMMA1 + 1);

#if MLDSA_MODE == 2
  for (i = 0; i < MLDSA_N / 4; ++i)
  __loop__(
    invariant(i <= MLDSA_N/4))
  {
    t[0] = MLDSA_GAMMA1 - a->coeffs[4 * i + 0];
    t[1] = MLDSA_GAMMA1 - a->coeffs[4 * i + 1];
    t[2] = MLDSA_GAMMA1 - a->coeffs[4 * i + 2];
    t[3] = MLDSA_GAMMA1 - a->coeffs[4 * i + 3];

    r[9 * i + 0] = (t[0]) & 0xFF;
    r[9 * i + 1] = (t[0] >> 8) & 0xFF;
    r[9 * i + 2] = (t[0] >> 16) & 0xFF;
    r[9 * i + 2] |= (t[1] << 2) & 0xFF;
    r[9 * i + 3] = (t[1] >> 6) & 0xFF;
    r[9 * i + 4] = (t[1] >> 14) & 0xFF;
    r[9 * i + 4] |= (t[2] << 4) & 0xFF;
    r[9 * i + 5] = (t[2] >> 4) & 0xFF;
    r[9 * i + 6] = (t[2] >> 12) & 0xFF;
    r[9 * i + 6] |= (t[3] << 6) & 0xFF;
    r[9 * i + 7] = (t[3] >> 2) & 0xFF;
    r[9 * i + 8] = (t[3] >> 10) & 0xFF;
  }
#else  /* MLDSA_MODE == 2 */
  for (i = 0; i < MLDSA_N / 2; ++i)
  __loop__(
    invariant(i <= MLDSA_N/2))
  {
    t[0] = MLDSA_GAMMA1 - a->coeffs[2 * i + 0];
    t[1] = MLDSA_GAMMA1 - a->coeffs[2 * i + 1];

    r[5 * i + 0] = (t[0]) & 0xFF;
    r[5 * i + 1] = (t[0] >> 8) & 0xFF;
    r[5 * i + 2] = (t[0] >> 16) & 0xFF;
    r[5 * i + 2] |= (t[1] << 4) & 0xFF;
    r[5 * i + 3] = (t[1] >> 4) & 0xFF;
    r[5 * i + 4] = (t[1] >> 12) & 0xFF;
  }
#endif /* MLDSA_MODE != 2 */
}

MLD_INTERNAL_API
void mld_polyz_unpack(mld_poly *r, const uint8_t *a)
{
  unsigned int i;

#if MLDSA_MODE == 2
  for (i = 0; i < MLDSA_N / 4; ++i)
  __loop__(
    invariant(i <= MLDSA_N/4)
    invariant(array_bound(r->coeffs, 0, i*4, -(MLDSA_GAMMA1 - 1), MLDSA_GAMMA1 + 1)))
  {
    r->coeffs[4 * i + 0] = a[9 * i + 0];
    r->coeffs[4 * i + 0] |= (uint32_t)a[9 * i + 1] << 8;
    r->coeffs[4 * i + 0] |= (uint32_t)a[9 * i + 2] << 16;
    r->coeffs[4 * i + 0] &= 0x3FFFF;

    r->coeffs[4 * i + 1] = a[9 * i + 2] >> 2;
    r->coeffs[4 * i + 1] |= (uint32_t)a[9 * i + 3] << 6;
    r->coeffs[4 * i + 1] |= (uint32_t)a[9 * i + 4] << 14;
    r->coeffs[4 * i + 1] &= 0x3FFFF;

    r->coeffs[4 * i + 2] = a[9 * i + 4] >> 4;
    r->coeffs[4 * i + 2] |= (uint32_t)a[9 * i + 5] << 4;
    r->coeffs[4 * i + 2] |= (uint32_t)a[9 * i + 6] << 12;
    r->coeffs[4 * i + 2] &= 0x3FFFF;

    r->coeffs[4 * i + 3] = a[9 * i + 6] >> 6;
    r->coeffs[4 * i + 3] |= (uint32_t)a[9 * i + 7] << 2;
    r->coeffs[4 * i + 3] |= (uint32_t)a[9 * i + 8] << 10;
    r->coeffs[4 * i + 3] &= 0x3FFFF;

    r->coeffs[4 * i + 0] = MLDSA_GAMMA1 - r->coeffs[4 * i + 0];
    r->coeffs[4 * i + 1] = MLDSA_GAMMA1 - r->coeffs[4 * i + 1];
    r->coeffs[4 * i + 2] = MLDSA_GAMMA1 - r->coeffs[4 * i + 2];
    r->coeffs[4 * i + 3] = MLDSA_GAMMA1 - r->coeffs[4 * i + 3];
  }
#else  /* MLDSA_MODE == 2 */
  for (i = 0; i < MLDSA_N / 2; ++i)
  __loop__(
    invariant(i <= MLDSA_N/2)
    invariant(array_bound(r->coeffs, 0, i*2, -(MLDSA_GAMMA1 - 1), MLDSA_GAMMA1 + 1)))
  {
    r->coeffs[2 * i + 0] = a[5 * i + 0];
    r->coeffs[2 * i + 0] |= (uint32_t)a[5 * i + 1] << 8;
    r->coeffs[2 * i + 0] |= (uint32_t)a[5 * i + 2] << 16;
    r->coeffs[2 * i + 0] &= 0xFFFFF;

    r->coeffs[2 * i + 1] = a[5 * i + 2] >> 4;
    r->coeffs[2 * i + 1] |= (uint32_t)a[5 * i + 3] << 4;
    r->coeffs[2 * i + 1] |= (uint32_t)a[5 * i + 4] << 12;
    /* r->coeffs[2*i+1] &= 0xFFFFF; */ /* No effect, since we're anyway at 20
                                          bits */

    r->coeffs[2 * i + 0] = MLDSA_GAMMA1 - r->coeffs[2 * i + 0];
    r->coeffs[2 * i + 1] = MLDSA_GAMMA1 - r->coeffs[2 * i + 1];
  }
#endif /* MLDSA_MODE != 2 */

  mld_assert_bound(r->coeffs, MLDSA_N, -(MLDSA_GAMMA1 - 1), MLDSA_GAMMA1 + 1);
}

MLD_INTERNAL_API
void mld_polyw1_pack(uint8_t r[MLDSA_POLYW1_PACKEDBYTES], const mld_poly *a)
{
  unsigned int i;

  mld_assert_bound(a->coeffs, MLDSA_N, 0, (MLDSA_Q - 1) / (2 * MLDSA_GAMMA2));

#if MLDSA_MODE == 2
  for (i = 0; i < MLDSA_N / 4; ++i)
  __loop__(
    invariant(i <= MLDSA_N/4))
  {
    r[3 * i + 0] = (a->coeffs[4 * i + 0]) & 0xFF;
    r[3 * i + 0] |= (a->coeffs[4 * i + 1] << 6) & 0xFF;
    r[3 * i + 1] = (a->coeffs[4 * i + 1] >> 2) & 0xFF;
    r[3 * i + 1] |= (a->coeffs[4 * i + 2] << 4) & 0xFF;
    r[3 * i + 2] = (a->coeffs[4 * i + 2] >> 4) & 0xFF;
    r[3 * i + 2] |= (a->coeffs[4 * i + 3] << 2) & 0xFF;
  }
#else  /* MLDSA_MODE == 2 */
  for (i = 0; i < MLDSA_N / 2; ++i)
  __loop__(
    invariant(i <= MLDSA_N/2))
  {
    r[i] = a->coeffs[2 * i + 0] | (a->coeffs[2 * i + 1] << 4);
  }
#endif /* MLDSA_MODE != 2 */
}
