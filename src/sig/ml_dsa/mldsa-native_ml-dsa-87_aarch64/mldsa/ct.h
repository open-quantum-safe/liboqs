/*
 * Copyright (c) The mlkem-native project authors
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#ifndef MLD_CT_H
#define MLD_CT_H

/* References
 * ==========
 *
 * - [libmceliece]
 *   libmceliece implementation of Classic McEliece
 *   Bernstein, Chou
 *   https://lib.mceliece.org/
 *
 * - [optblocker]
 *   PQC forum post on opt-blockers using volatile globals
 *   Daniel J. Bernstein
 *   https://groups.google.com/a/list.nist.gov/g/pqc-forum/c/hqbtIGFKIpU/m/H14H0wOlBgAJ
 */

#include <stdint.h>
#include "cbmc.h"
#include "common.h"

/* Constant-time comparisons and conditional operations

   We reduce the risk for compilation into variable-time code
   through the use of 'value barriers'.

   Functionally, a value barrier is a no-op. To the compiler, however,
   it constitutes an arbitrary modification of its input, and therefore
   harden's value propagation and range analysis.

   We consider two approaches to implement a value barrier:
   - An empty inline asm block which marks the target value as clobbered.
   - XOR'ing with the value of a volatile global that's set to 0;
     see @[optblocker] for a discussion of this idea, and
     @[libmceliece, inttypes/crypto_intN.h] for an implementation.

   The first approach is cheap because it only prevents the compiler
   from reasoning about the value of the variable past the barrier,
   but does not directly generate additional instructions.

   The second approach generates redundant loads and XOR operations
   and therefore comes at a higher runtime cost. However, it appears
   more robust towards optimization, as compilers should never drop
   a volatile load.

   We use the empty-ASM value barrier for GCC and clang, and fall
   back to the global volatile barrier otherwise.

   The global value barrier can be forced by setting
   MLD_CONFIG_NO_ASM_VALUE_BARRIER.

*/

#if defined(MLD_HAVE_INLINE_ASM) && !defined(MLD_CONFIG_NO_ASM_VALUE_BARRIER)
#define MLD_USE_ASM_VALUE_BARRIER
#endif


#if !defined(MLD_USE_ASM_VALUE_BARRIER)
/*
 * Declaration of global volatile that the global value barrier
 * is loading from and masking with.
 */
#define mld_ct_opt_blocker_u64 MLD_NAMESPACE(ct_opt_blocker_u64)
extern volatile uint64_t mld_ct_opt_blocker_u64;


/* Helper functions for obtaining global masks of various sizes */

/* This contract is not proved but treated as an axiom.
 *
 * Its validity relies on the assumption that the global opt-blocker
 * constant mld_ct_opt_blocker_u64 is not modified.
 */
static MLD_INLINE uint64_t mld_ct_get_optblocker_u64(void)
__contract__(ensures(return_value == 0)) { return mld_ct_opt_blocker_u64; }

static MLD_INLINE int64_t mld_ct_get_optblocker_i64(void)
__contract__(ensures(return_value == 0)) { return (int64_t)mld_ct_get_optblocker_u64(); }

static MLD_INLINE uint32_t mld_ct_get_optblocker_u32(void)
__contract__(ensures(return_value == 0)) { return (uint32_t)mld_ct_get_optblocker_u64(); }

/* Opt-blocker based implementation of value barriers */
static MLD_INLINE int64_t mld_value_barrier_i64(int64_t b)
__contract__(ensures(return_value == b)) { return (b ^ mld_ct_get_optblocker_i64()); }

static MLD_INLINE uint32_t mld_value_barrier_u32(uint32_t b)
__contract__(ensures(return_value == b)) { return (b ^ mld_ct_get_optblocker_u32()); }


#else  /* !MLD_USE_ASM_VALUE_BARRIER */
static MLD_INLINE int64_t mld_value_barrier_i64(int64_t b)
__contract__(ensures(return_value == b))
{
  __asm__("" : "+r"(b));
  return b;
}

static MLD_INLINE uint32_t mld_value_barrier_u32(uint32_t b)
__contract__(ensures(return_value == b))
{
  __asm__("" : "+r"(b));
  return b;
}
#endif /* MLD_USE_ASM_VALUE_BARRIER */


/*
 * The mld_ct_sel_int32 function below makes deliberate use of unsigned
 * to signed integer conversion, which is implementation-defined
 * behaviour. Here, we assume that uint16_t -> int16_t is inverse
 * to int16_t -> uint16_t.
 */
#ifdef CBMC
#pragma CPROVER check push
#pragma CPROVER check disable "conversion"
#endif

/*************************************************
 * Name:        mld_ct_sel_int32
 *
 * Description: Functionally equivalent to cond ? a : b,
 *              but implemented with guards against
 *              compiler-introduced branches.
 *
 * Arguments:   int32_t a:       First alternative
 *              int32_t b:       Second alternative
 *              uint32_t cond:   Condition variable.
 *
 *
 **************************************************/
static MLD_INLINE int32_t mld_ct_sel_int32(int32_t a, int32_t b, uint32_t cond)
/* TODO: proof */
__contract__(
  requires(cond == 0x0 || cond == 0xFFFFFFFF)
  ensures(return_value == (cond ? a : b))
)
{
  uint32_t au = a, bu = b;
  uint32_t res = bu ^ (mld_value_barrier_u32(cond) & (au ^ bu));
  return (int32_t)res;
}

/* Put unsigned-to-signed warnings in CBMC back into scope */
#ifdef CBMC
#pragma CPROVER check pop
#endif



/*
 * The mld_ct_cmask_neg_i32 function below makes deliberate use of
 * signed to unsigned integer conversion, which is fully defined
 * behaviour in C. It is thus safe to disable this warning.
 */
#ifdef CBMC
#pragma CPROVER check push
#pragma CPROVER check disable "conversion"
#endif


/*************************************************
 * Name:        mld_ct_cmask_neg_i32
 *
 * Description: Return 0 if input is non-negative, and -1 otherwise.
 *
 * Arguments:   int32_t x: Value to be converted into a mask
 *
 **************************************************/
/* TODO: proof */
static MLD_INLINE uint32_t mld_ct_cmask_neg_i32(int32_t x)
__contract__(
  ensures(return_value == ((x < 0) ? 0xFFFFFFFF : 0))
)
{
  int64_t tmp = mld_value_barrier_i64((int64_t)x);
  tmp >>= 31;
  return (uint32_t)tmp;
}

/* Put signed-to-unsigned warnings in CBMC back into scope */
#ifdef CBMC
#pragma CPROVER check pop
#endif

/*************************************************
 * Name:        mld_ct_abs_i32
 *
 * Description: Return -x if x<0, x otherwise
 *
 * Arguments:   int32_t x: Input value
 *
 **************************************************/
/* TODO: proof */
static MLD_INLINE int32_t mld_ct_abs_i32(int32_t x)
__contract__(
  requires(x >= -INT32_MAX)
  ensures(return_value == ((x < 0) ? -x : x))
)
{
  return mld_ct_sel_int32(-x, x, mld_ct_cmask_neg_i32(x));
}


#endif /* !MLD_CT_H */
