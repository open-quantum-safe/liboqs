/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#ifndef MLD_REDUCE_H
#define MLD_REDUCE_H

#include <stdint.h>
#include "cbmc.h"
#include "common.h"
#include "ct.h"
#include "debug.h"

/* check-magic: -4186625 == pow(2,32,MLDSA_Q) */
#define MONT -4186625

/* Upper bound for domain of mld_reduce32() */
#define REDUCE32_DOMAIN_MAX (INT32_MAX - (1 << 22))

/* Absolute bound for range of mld_reduce32() */
/* check-magic: 6283009 == (REDUCE32_DOMAIN_MAX - 255 * MLDSA_Q + 1) */
#define REDUCE32_RANGE_MAX 6283009

/* Absolute bound for domain of mld_montgomery_reduce() */
#define MONTGOMERY_REDUCE_DOMAIN_MAX ((int64_t)INT32_MIN * INT32_MIN)

/* Absolute bound for tight domain of mld_montgomery_reduce() */
#define MONTGOMERY_REDUCE_STRONG_DOMAIN_MAX ((int64_t)INT32_MIN * -MLDSA_Q)



/*************************************************
 * Name:        mld_cast_uint32_to_int32
 *
 * Description: Cast uint32 value to int32
 *
 * Returns:
 *   input x in     0 .. 2^31-1: returns value unchanged
 *   input x in  2^31 .. 2^32-1: returns (x - 2^32)
 **************************************************/
#ifdef CBMC
#pragma CPROVER check push
#pragma CPROVER check disable "conversion"
#endif
static MLD_INLINE int32_t mld_cast_uint32_to_int32(uint32_t x)
{
  /*
   * PORTABILITY: This relies on uint32_t -> int32_t
   * being implemented as the inverse of int32_t -> uint32_t,
   * which is implementation-defined (C99 6.3.1.3 (3))
   * CBMC (correctly) fails to prove this conversion is OK,
   * so we have to suppress that check here
   */
  return (int32_t)x;
}
#ifdef CBMC
#pragma CPROVER check pop
#endif

/*************************************************
 * Name:        mld_montgomery_reduce
 *
 * Description:
 *      For finite field element a with
 *        -MONTGOMERY_REDUCE_DOMAIN_MAX <= a <= MONTGOMERY_REDUCE_DOMAIN_MAX,
 *      compute r == a*2^{-32} (mod MLDSA_Q) such that
 *        INT32_MIN <= r < REDUCE32_DOMAIN_MAX
 *
 *      The upper-bound on the result ensures that a result from this
 *      function can be used as an input to mld_reduce32() declared below.
 *
 *      Additionally, as a special case, if the input a is in range
 *        -MONTGOMERY_REDUCE_STRONG_DOMAIN_MAX < a <
 *          MONTGOMERY_REDUCE_STRONG_DOMAIN_MAX
 *      then the result satisfies -MLDSA_Q < r < MLDSA_Q.
 *
 * Arguments:   - int64_t: finite field element a
 *
 * Returns r.
 **************************************************/
static MLD_INLINE int32_t mld_montgomery_reduce(int64_t a)
__contract__(
  requires(a >= -MONTGOMERY_REDUCE_DOMAIN_MAX && a <= MONTGOMERY_REDUCE_DOMAIN_MAX)
  ensures(return_value >= INT32_MIN && return_value < REDUCE32_DOMAIN_MAX)

  /* Special case - for stronger input bounds, we can ensure stronger bounds on the output */
  ensures((a >= -MONTGOMERY_REDUCE_STRONG_DOMAIN_MAX && a < MONTGOMERY_REDUCE_STRONG_DOMAIN_MAX) ==>
          (return_value > -MLDSA_Q && return_value < MLDSA_Q))
)
{
  /* check-magic: 58728449 == unsigned_mod(pow(MLDSA_Q, -1, 2^32), 2^32) */
  const uint64_t QINV = 58728449;

  /*  Compute a*q^{-1} mod 2^32 in unsigned representatives */
  const uint32_t a_reduced = a & UINT32_MAX;
  const uint32_t a_inverted = (a_reduced * QINV) & UINT32_MAX;

  /* Lift to signed canonical representative mod 2^32. */
  const int32_t t = mld_cast_uint32_to_int32(a_inverted);

  int64_t r;

  r = a - ((int64_t)t * MLDSA_Q);

  /*
   * PORTABILITY: Right-shift on a signed integer is, strictly-speaking,
   * implementation-defined for negative left argument. Here,
   * we assume it's sign-preserving "arithmetic" shift right. (C99 6.5.7 (5))
   */
  r = r >> 32;
  return (int32_t)r;
}

/*************************************************
 * Name:        mld_reduce32
 *
 * Description: For finite field element a with a <= 2^{31} - 2^{22} - 1,
 *              compute r \equiv a (mod MLDSA_Q) such that
 *              -REDUCE32_RANGE_MAX <= r < REDUCE32_RANGE_MAX.
 *
 * Arguments:   - int32_t: finite field element a
 *
 * Returns r.
 **************************************************/
static MLD_INLINE int32_t mld_reduce32(int32_t a)
__contract__(
  requires(a <= REDUCE32_DOMAIN_MAX)
  ensures(return_value >= -REDUCE32_RANGE_MAX)
  ensures(return_value <   REDUCE32_RANGE_MAX)
)
{
  int32_t t;

  t = (a + (1 << 22)) >> 23;
  t = a - t * MLDSA_Q;
  mld_assert((t - a) % MLDSA_Q == 0);
  return t;
}

/*************************************************
 * Name:        mld_caddq
 *
 * Description: Add MLDSA_Q if input coefficient is negative.
 *
 * Arguments:   - int32_t: finite field element a
 *
 * Returns r.
 **************************************************/
static MLD_INLINE int32_t mld_caddq(int32_t a)
__contract__(
  requires(a > -MLDSA_Q)
  requires(a < MLDSA_Q)
  ensures(return_value >= 0)
  ensures(return_value < MLDSA_Q)
  ensures(return_value == (a >= 0) ? a : (a + MLDSA_Q))
)
{
  return mld_ct_sel_int32(a + MLDSA_Q, a, mld_ct_cmask_neg_i32(a));
}


#endif /* !MLD_REDUCE_H */
