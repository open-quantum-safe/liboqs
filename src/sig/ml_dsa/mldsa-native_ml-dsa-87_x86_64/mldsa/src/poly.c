/*
 * Copyright (c) The mldsa-native project authors
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* References
 * ==========
 *
 * - [FIPS204]
 *   FIPS 204 Module-Lattice-Based Digital Signature Standard
 *   National Institute of Standards and Technology
 *   https://csrc.nist.gov/pubs/fips/204/final
 *
 * - [REF]
 *   CRYSTALS-Dilithium reference implementation
 *   Bai, Ducas, Kiltz, Lepoint, Lyubashevsky, Schwabe, Seiler, Stehlé
 *   https://github.com/pq-crystals/dilithium/tree/master/ref
 */

#include <stdint.h>
#include <string.h>

#include "common.h"
#include "ct.h"
#include "debug.h"
#include "ntt.h"
#include "poly.h"
#include "reduce.h"
#include "rounding.h"
#include "symmetric.h"

#if !defined(MLD_CONFIG_MULTILEVEL_NO_SHARED)

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


#if !defined(MLD_USE_NATIVE_POLY_CADDQ)
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
#else  /* !MLD_USE_NATIVE_POLY_CADDQ */
MLD_INTERNAL_API
void mld_poly_caddq(mld_poly *a)
{
  mld_assert_abs_bound(a->coeffs, MLDSA_N, MLDSA_Q);
  mld_poly_caddq_native(a->coeffs);
  mld_assert_bound(a->coeffs, MLDSA_N, 0, MLDSA_Q);
}
#endif /* MLD_USE_NATIVE_POLY_CADDQ */

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
#if defined(MLD_USE_NATIVE_POINTWISE_MONTGOMERY)
  /* TODO: proof */
  mld_assert_abs_bound(a->coeffs, MLDSA_N, MLD_NTT_BOUND);
  mld_assert_abs_bound(b->coeffs, MLDSA_N, MLD_NTT_BOUND);
  mld_poly_pointwise_montgomery_native(c->coeffs, a->coeffs, b->coeffs);
  mld_assert_abs_bound(c->coeffs, MLDSA_N, MLDSA_Q);
#else  /* MLD_USE_NATIVE_POINTWISE_MONTGOMERY */
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
#endif /* !MLD_USE_NATIVE_POINTWISE_MONTGOMERY */
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

/* Reference: `mld_rej_uniform()` in the reference implementation @[REF].
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
      a[ctr++] = (int32_t)t;
    }
  }

  mld_assert_bound(a, ctr, 0, MLDSA_Q);

  return ctr;
}

/* Reference: poly_uniform() in the reference implementation @[REF].
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

  /* @[FIPS204, Section 3.6.3] Destruction of intermediate values. */
  mld_zeroize(buf, sizeof(buf));
}

#if !defined(MLD_CONFIG_SERIAL_FIPS202_ONLY)
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

  /* @[FIPS204, Section 3.6.3] Destruction of intermediate values. */
  mld_zeroize(buf, sizeof(buf));
}

#endif /* !MLD_CONFIG_SERIAL_FIPS202_ONLY */

MLD_INTERNAL_API
void mld_polyt1_pack(uint8_t *r, const mld_poly *a)
{
  unsigned int i;
  mld_assert_bound(a->coeffs, MLDSA_N, 0, 1 << 10);

  for (i = 0; i < MLDSA_N / 4; ++i)
  __loop__(
    invariant(i <= MLDSA_N/4))
  {
    r[5 * i + 0] = (uint8_t)((a->coeffs[4 * i + 0] >> 0) & 0xFF);
    r[5 * i + 1] =
        (uint8_t)(((a->coeffs[4 * i + 0] >> 8) | (a->coeffs[4 * i + 1] << 2)) &
                  0xFF);
    r[5 * i + 2] =
        (uint8_t)(((a->coeffs[4 * i + 1] >> 6) | (a->coeffs[4 * i + 2] << 4)) &
                  0xFF);
    r[5 * i + 3] =
        (uint8_t)(((a->coeffs[4 * i + 2] >> 4) | (a->coeffs[4 * i + 3] << 6)) &
                  0xFF);
    r[5 * i + 4] = (uint8_t)((a->coeffs[4 * i + 3] >> 2) & 0xFF);
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
        ((a[5 * i + 0] >> 0) | ((int32_t)a[5 * i + 1] << 8)) & 0x3FF;
    r->coeffs[4 * i + 1] =
        ((a[5 * i + 1] >> 2) | ((int32_t)a[5 * i + 2] << 6)) & 0x3FF;
    r->coeffs[4 * i + 2] =
        ((a[5 * i + 2] >> 4) | ((int32_t)a[5 * i + 3] << 4)) & 0x3FF;
    r->coeffs[4 * i + 3] =
        ((a[5 * i + 3] >> 6) | ((int32_t)a[5 * i + 4] << 2)) & 0x3FF;
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
    /* Safety: a->coeffs[i] <= (1 << (MLDSA_D - 1) as they are output of
     * power2round, hence, these casts are safe. */
    t[0] = (uint32_t)((1 << (MLDSA_D - 1)) - a->coeffs[8 * i + 0]);
    t[1] = (uint32_t)((1 << (MLDSA_D - 1)) - a->coeffs[8 * i + 1]);
    t[2] = (uint32_t)((1 << (MLDSA_D - 1)) - a->coeffs[8 * i + 2]);
    t[3] = (uint32_t)((1 << (MLDSA_D - 1)) - a->coeffs[8 * i + 3]);
    t[4] = (uint32_t)((1 << (MLDSA_D - 1)) - a->coeffs[8 * i + 4]);
    t[5] = (uint32_t)((1 << (MLDSA_D - 1)) - a->coeffs[8 * i + 5]);
    t[6] = (uint32_t)((1 << (MLDSA_D - 1)) - a->coeffs[8 * i + 6]);
    t[7] = (uint32_t)((1 << (MLDSA_D - 1)) - a->coeffs[8 * i + 7]);

    r[13 * i + 0] = (uint8_t)((t[0]) & 0xFF);
    r[13 * i + 1] = (uint8_t)((t[0] >> 8) & 0xFF);
    r[13 * i + 1] |= (uint8_t)((t[1] << 5) & 0xFF);
    r[13 * i + 2] = (uint8_t)((t[1] >> 3) & 0xFF);
    r[13 * i + 3] = (uint8_t)((t[1] >> 11) & 0xFF);
    r[13 * i + 3] |= (uint8_t)((t[2] << 2) & 0xFF);
    r[13 * i + 4] = (uint8_t)((t[2] >> 6) & 0xFF);
    r[13 * i + 4] |= (uint8_t)((t[3] << 7) & 0xFF);
    r[13 * i + 5] = (uint8_t)((t[3] >> 1) & 0xFF);
    r[13 * i + 6] = (uint8_t)((t[3] >> 9) & 0xFF);
    r[13 * i + 6] |= (uint8_t)((t[4] << 4) & 0xFF);
    r[13 * i + 7] = (uint8_t)((t[4] >> 4) & 0xFF);
    r[13 * i + 8] = (uint8_t)((t[4] >> 12) & 0xFF);
    r[13 * i + 8] |= (uint8_t)((t[5] << 1) & 0xFF);
    r[13 * i + 9] = (uint8_t)((t[5] >> 7) & 0xFF);
    r[13 * i + 9] |= (uint8_t)((t[6] << 6) & 0xFF);
    r[13 * i + 10] = (uint8_t)((t[6] >> 2) & 0xFF);
    r[13 * i + 11] = (uint8_t)((t[6] >> 10) & 0xFF);
    r[13 * i + 11] |= (uint8_t)((t[7] << 3) & 0xFF);
    r[13 * i + 12] = (uint8_t)((t[7] >> 5) & 0xFF);
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
    r->coeffs[8 * i + 0] |= (int32_t)a[13 * i + 1] << 8;
    r->coeffs[8 * i + 0] &= 0x1FFF;

    r->coeffs[8 * i + 1] = a[13 * i + 1] >> 5;
    r->coeffs[8 * i + 1] |= (int32_t)a[13 * i + 2] << 3;
    r->coeffs[8 * i + 1] |= (int32_t)a[13 * i + 3] << 11;
    r->coeffs[8 * i + 1] &= 0x1FFF;

    r->coeffs[8 * i + 2] = a[13 * i + 3] >> 2;
    r->coeffs[8 * i + 2] |= (int32_t)a[13 * i + 4] << 6;
    r->coeffs[8 * i + 2] &= 0x1FFF;

    r->coeffs[8 * i + 3] = a[13 * i + 4] >> 7;
    r->coeffs[8 * i + 3] |= (int32_t)a[13 * i + 5] << 1;
    r->coeffs[8 * i + 3] |= (int32_t)a[13 * i + 6] << 9;
    r->coeffs[8 * i + 3] &= 0x1FFF;

    r->coeffs[8 * i + 4] = a[13 * i + 6] >> 4;
    r->coeffs[8 * i + 4] |= (int32_t)a[13 * i + 7] << 4;
    r->coeffs[8 * i + 4] |= (int32_t)a[13 * i + 8] << 12;
    r->coeffs[8 * i + 4] &= 0x1FFF;

    r->coeffs[8 * i + 5] = a[13 * i + 8] >> 1;
    r->coeffs[8 * i + 5] |= (int32_t)a[13 * i + 9] << 7;
    r->coeffs[8 * i + 5] &= 0x1FFF;

    r->coeffs[8 * i + 6] = a[13 * i + 9] >> 6;
    r->coeffs[8 * i + 6] |= (int32_t)a[13 * i + 10] << 2;
    r->coeffs[8 * i + 6] |= (int32_t)a[13 * i + 11] << 10;
    r->coeffs[8 * i + 6] &= 0x1FFF;

    r->coeffs[8 * i + 7] = a[13 * i + 11] >> 3;
    r->coeffs[8 * i + 7] |= (int32_t)a[13 * i + 12] << 5;
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

/* Reference: explicitly checks the bound B to be <= (MLDSA_Q - 1) / 8).
 * This is unnecessary as it's always a compile-time constant.
 * We instead model it as a precondition.
 * Checking the bound is performed using a conditional arguing
 * that it is okay to leak which coefficient violates the bound (while the
 * coefficient itself must remain secret).
 * We instead perform everything in constant-time.
 * Also it is sufficient to check that it is smaller than
 * MLDSA_Q - REDUCE32_RANGE_MAX > (MLDSA_Q - 1) / 8).
 */
MLD_INTERNAL_API
uint32_t mld_poly_chknorm(const mld_poly *a, int32_t B)
{
#if defined(MLD_USE_NATIVE_POLY_CHKNORM)
  /* TODO: proof */
  mld_assert_bound(a->coeffs, MLDSA_N, -REDUCE32_RANGE_MAX, REDUCE32_RANGE_MAX);

  /* The native backend returns 0 if all coeffs within the bound, 1 otherwise */
  /* Convert to 0 / 0xFFFFFFFF here */
  return 0U - (uint32_t)mld_poly_chknorm_native(a->coeffs, B);
#else  /* MLD_USE_NATIVE_POLY_CHKNORM */
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
    /*
     * Since we know that -REDUCE32_RANGE_MAX <= a < REDUCE32_RANGE_MAX,
     * and B <= MLDSA_Q - REDUCE32_RANGE_MAX, to check if
     * -B < (a mod± MLDSA_Q) < B, it suffices to check if -B < a < B.
     *
     * We prove this to be true using the following CBMC assertions.
     * a ==> b expressed as !a || b to also allow run-time assertion.
     */
    mld_assert(a->coeffs[i] < B || a->coeffs[i] - MLDSA_Q <= -B);
    mld_assert(a->coeffs[i] > -B || a->coeffs[i] + MLDSA_Q >= B);

    /* Reference: Leaks which coefficient violates the bound via a conditional.
     * We are more conservative to reduce the number of declassifications in
     * constant-time testing.
     */

    /* if (abs(a[i]) >= B) */
    t |= mld_ct_cmask_neg_i32(B - 1 - mld_ct_abs_i32(a->coeffs[i]));
  }

  return t;
#endif /* !MLD_USE_NATIVE_POLY_CHKNORM */
}

#else  /* !MLD_CONFIG_MULTILEVEL_NO_SHARED */
MLD_EMPTY_CU(mld_poly)
#endif /* MLD_CONFIG_MULTILEVEL_NO_SHARED */

/* To facilitate single-compilation-unit (SCU) builds, undefine all macros.
 * Don't modify by hand -- this is auto-generated by scripts/autogen. */
#undef POLY_UNIFORM_NBLOCKS
