/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#ifndef MLD_ROUNDING_H
#define MLD_ROUNDING_H

#include <stdint.h>
#include "cbmc.h"
#include "common.h"
#include "ct.h"
#include "debug.h"

#define MLD_2_POW_D (1 << MLDSA_D)

/*************************************************
 * Name:        mld_power2round
 *
 * Description: For finite field element a, compute a0, a1 such that
 *              a mod^+ MLDSA_Q = a1*2^MLDSA_D + a0 with -2^{MLDSA_D-1} < a0 <=
 *              2^{MLDSA_D-1}. Assumes a to be standard representative.
 *
 * Arguments:   - int32_t a: input element
 *              - int32_t *a0: pointer to output element a0
 *              - int32_t *a1: pointer to output element a1
 *
 * Reference: In the reference implementation, a1 is passed as a
 * return value instead.
 **************************************************/
static MLD_INLINE void mld_power2round(int32_t *a0, int32_t *a1, int32_t a)
__contract__(
  requires(memory_no_alias(a0, sizeof(int32_t)))
  requires(memory_no_alias(a1, sizeof(int32_t)))
  requires(a >= 0 && a < MLDSA_Q)
  assigns(memory_slice(a0, sizeof(int32_t)))
  assigns(memory_slice(a1, sizeof(int32_t)))
  ensures(*a0 > -(MLD_2_POW_D/2) && *a0 <= (MLD_2_POW_D/2))
  ensures(*a1 >= 0 && *a1 <= (MLDSA_Q - 1) / MLD_2_POW_D)
  ensures((*a1 * MLD_2_POW_D + *a0 - a) % MLDSA_Q == 0)
)
{
  *a1 = (a + (1 << (MLDSA_D - 1)) - 1) >> MLDSA_D;
  *a0 = a - (*a1 << MLDSA_D);
}

/*************************************************
 * Name:        mld_decompose
 *
 * Description: For finite field element a, compute high and low bits a0, a1
 * such that a mod^+ MLDSA_Q = a1* 2 * MLDSA_GAMMA2 + a0 with
 * -MLDSA_GAMMA2 < a0 <= MLDSA_GAMMA2 except
 * if a1 = (MLDSA_Q-1)/(MLDSA_GAMMA2*2) where we set a1 = 0 and
 * -MLDSA_GAMMA2 <= a0 = a mod^+ MLDSA_Q - MLDSA_Q < 0.
 * Assumes a to be standard representative.
 *
 * Arguments:   - int32_t a: input element
 *              - int32_t *a0: pointer to output element a0
 *              - int32_t *a1: pointer to output element a1
 *
 * Reference: a1 is passed as a return value instead
 **************************************************/
static MLD_INLINE void mld_decompose(int32_t *a0, int32_t *a1, int32_t a)
__contract__(
  requires(memory_no_alias(a0, sizeof(int32_t)))
  requires(memory_no_alias(a1, sizeof(int32_t)))
  requires(a >= 0 && a < MLDSA_Q)
  assigns(memory_slice(a0, sizeof(int32_t)))
  assigns(memory_slice(a1, sizeof(int32_t)))
  /* a0 = -MLDSA_GAMMA2 can only occur when (q-1) = a - (a mod MLDSA_GAMMA2),
   * then a1=1; and a0 = a - (a mod MLDSA_GAMMA2) - 1 (See Alg. 36, FIPS204) */
  ensures(*a0 >= -MLDSA_GAMMA2  && *a0 <= MLDSA_GAMMA2)
  ensures(*a1 >= 0 && *a1 < (MLDSA_Q-1)/(2*MLDSA_GAMMA2))
  ensures((*a1 * 2 * MLDSA_GAMMA2 + *a0 - a) % MLDSA_Q == 0)
)
{
  *a1 = (a + 127) >> 7;
  /* We know a >= 0 and a < MLDSA_Q, so... */
  /* check-magic: 65472 == round((MLDSA_Q-1)/128) */
  mld_assert(*a1 >= 0 && *a1 <= 65472);

#if MLDSA_MODE == 2
  /* check-magic: 11275 == round((2**24*128) / ((MLDSA_Q - 1) / 44)) */
  *a1 = (*a1 * 11275 + (1 << 23)) >> 24;
  mld_assert(*a1 >= 0 && *a1 <= 44);

  *a1 = mld_ct_sel_int32(0, *a1, mld_ct_cmask_neg_i32(43 - *a1));
  mld_assert(*a1 >= 0 && *a1 <= 43);
#else /* MLDSA_MODE == 2 */
  /* check-magic: 1025 == round((2**22*128) / ((MLDSA_Q - 1) / 16)) */
  *a1 = (*a1 * 1025 + (1 << 21)) >> 22;
  mld_assert(*a1 >= 0 && *a1 <= 16);

  *a1 &= 15;
  mld_assert(*a1 >= 0 && *a1 <= 15);

#endif /* MLDSA_MODE != 2 */

  *a0 = a - *a1 * 2 * MLDSA_GAMMA2;
  *a0 = mld_ct_sel_int32(*a0 - MLDSA_Q, *a0,
                         mld_ct_cmask_neg_i32((MLDSA_Q - 1) / 2 - *a0));
}

/*************************************************
 * Name:        mld_make_hint
 *
 * Description: Compute hint bit indicating whether the low bits of the
 *              input element overflow into the high bits.
 *
 * Arguments:   - int32_t a0: low bits of input element
 *              - int32_t a1: high bits of input element
 *
 * Returns 1 if overflow, 0 otherwise
 **************************************************/
static MLD_INLINE unsigned int mld_make_hint(int32_t a0, int32_t a1)
__contract__(
  ensures(return_value >= 0 && return_value <= 1)
)
{
  if (a0 > MLDSA_GAMMA2 || a0 < -MLDSA_GAMMA2 ||
      (a0 == -MLDSA_GAMMA2 && a1 != 0))
  {
    return 1;
  }

  return 0;
}

/*************************************************
 * Name:        mld_use_hint
 *
 * Description: Correct high bits according to hint.
 *
 * Arguments:   - int32_t a: input element
 *              - unsigned int hint: hint bit
 *
 * Returns corrected high bits.
 **************************************************/
static MLD_INLINE int32_t mld_use_hint(int32_t a, unsigned int hint)
__contract__(
  requires(hint >= 0 && hint <= 1)
  requires(a >= 0 && a < MLDSA_Q)
  ensures(return_value >= 0 && return_value < (MLDSA_Q-1)/(2*MLDSA_GAMMA2))
)
{
  int32_t a0, a1;

  mld_decompose(&a0, &a1, a);
  if (hint == 0)
  {
    return a1;
  }

#if MLDSA_MODE == 2
  if (a0 > 0)
  {
    return (a1 == 43) ? 0 : a1 + 1;
  }
  else
  {
    return (a1 == 0) ? 43 : a1 - 1;
  }
#else  /* MLDSA_MODE == 2 */
  if (a0 > 0)
  {
    return (a1 + 1) & 15;
  }
  else
  {
    return (a1 - 1) & 15;
  }
#endif /* MLDSA_MODE != 2 */
}


#endif /* !MLD_ROUNDING_H */
