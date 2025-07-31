/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#ifndef MLD_ROUNDING_H
#define MLD_ROUNDING_H

#include <stdint.h>
#include "cbmc.h"
#include "common.h"

#define MLD_2_POW_D (1 << MLDSA_D)

#define mld_power2round MLD_NAMESPACE(power2round)
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
void mld_power2round(int32_t *a0, int32_t *a1, int32_t a)
__contract__(
  requires(memory_no_alias(a0, sizeof(int32_t)))
  requires(memory_no_alias(a1, sizeof(int32_t)))
  requires(a >= 0 && a < MLDSA_Q)
  assigns(memory_slice(a0, sizeof(int32_t)))
  assigns(memory_slice(a1, sizeof(int32_t)))
  ensures(*a0 > -(MLD_2_POW_D/2) && *a0 <= (MLD_2_POW_D/2))
  ensures(*a1 >= 0 && *a1 <= (MLDSA_Q - 1) / MLD_2_POW_D)
  ensures((*a1 * MLD_2_POW_D + *a0 - a) % MLDSA_Q == 0)
);


#define mld_decompose MLD_NAMESPACE(decompose)
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
void mld_decompose(int32_t *a0, int32_t *a1, int32_t a)
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
);

#define mld_make_hint MLD_NAMESPACE(make_hint)
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
unsigned int mld_make_hint(int32_t a0, int32_t a1)
__contract__(
  ensures(return_value >= 0 && return_value <= 1)
);

#define mld_use_hint MLD_NAMESPACE(use_hint)
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
int32_t mld_use_hint(int32_t a, unsigned int hint)
__contract__(
  requires(hint >= 0 && hint <= 1)
  requires(a >= 0 && a < MLDSA_Q)
  ensures(return_value >= 0 && return_value < (MLDSA_Q-1)/(2*MLDSA_GAMMA2))
);

#endif /* !MLD_ROUNDING_H */
