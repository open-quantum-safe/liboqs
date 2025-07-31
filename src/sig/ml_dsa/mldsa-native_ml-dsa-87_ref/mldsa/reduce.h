/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#ifndef MLD_REDUCE_H
#define MLD_REDUCE_H

#include <stdint.h>
#include "cbmc.h"
#include "common.h"

#define MONT -4186625 /* 2^32 % MLDSA_Q */

/* Upper bound for domain of mld_reduce32() */
#define REDUCE32_DOMAIN_MAX (INT32_MAX - (1 << 22))

/* Absolute bound for range of mld_reduce32() */
#define REDUCE32_RANGE_MAX 6283009

/* Absolute bound for domain of mld_montgomery_reduce() */
#define MONTGOMERY_REDUCE_DOMAIN_MAX ((int64_t)INT32_MIN * INT32_MIN)

/* Absolute bound for tight domain of mld_montgomery_reduce() */
#define MONTGOMERY_REDUCE_STRONG_DOMAIN_MAX ((int64_t)INT32_MIN * -MLDSA_Q)


#define mld_montgomery_reduce MLD_NAMESPACE(montgomery_reduce)
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
int32_t mld_montgomery_reduce(int64_t a)
__contract__(
  requires(a >= -MONTGOMERY_REDUCE_DOMAIN_MAX && a <= MONTGOMERY_REDUCE_DOMAIN_MAX)
  ensures(return_value >= INT32_MIN && return_value < REDUCE32_DOMAIN_MAX)

  /* Special case - for stronger input bounds, we can ensure stronger bounds on the output */
  ensures((a >= -MONTGOMERY_REDUCE_STRONG_DOMAIN_MAX && a < MONTGOMERY_REDUCE_STRONG_DOMAIN_MAX) ==>
          (return_value > -MLDSA_Q && return_value < MLDSA_Q))
);

#define mld_reduce32 MLD_NAMESPACE(reduce32)
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
int32_t mld_reduce32(int32_t a)
__contract__(
  requires(a <= REDUCE32_DOMAIN_MAX)
  ensures(return_value >= -REDUCE32_RANGE_MAX)
  ensures(return_value <   REDUCE32_RANGE_MAX)
);

#define mld_caddq MLD_NAMESPACE(caddq)
/*************************************************
 * Name:        mld_caddq
 *
 * Description: Add MLDSA_Q if input coefficient is negative.
 *
 * Arguments:   - int32_t: finite field element a
 *
 * Returns r.
 **************************************************/
int32_t mld_caddq(int32_t a)
__contract__(
  requires(a > -MLDSA_Q)
  requires(a < MLDSA_Q)
  ensures(return_value >= 0)
  ensures(return_value < MLDSA_Q)
  ensures(return_value == (a >= 0) ? a : (a + MLDSA_Q))
);


#endif /* !MLD_REDUCE_H */
