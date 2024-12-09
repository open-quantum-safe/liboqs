/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef MLKEM_DEBUG_H
#define MLKEM_DEBUG_H

#include "../common.h"

#if defined(MLKEM_DEBUG)
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*************************************************
 * Name:        mlkem_debug_assert
 *
 * Description: Check debug assertion
 *
 *              Prints an error message to stderr and calls
 *              exit(1) if not.
 *
 * Arguments:   - file: filename
 *              - line: line number
 *              - description: Textual description of assertion
 *              - val: Value asserted to be non-zero
 **************************************************/
#define mlkem_debug_assert MLKEM_NAMESPACE(mlkem_debug_assert)
void mlkem_debug_assert(const char *file, int line, const char *description,
                        const int val);

/*************************************************
 * Name:        mlkem_debug_check_bounds
 *
 * Description: Check whether values in an array of int16_t
 *              are within specified bounds.
 *
 *              Prints an error message to stderr and calls
 *              exit(1) if not.
 *
 * Arguments:   - file: filename
 *              - line: line number
 *              - description: Textual description of check
 *              - ptr: Base of array to be checked
 *              - len: Number of int16_t in ptr
 *              - lower_bound_exclusive: Exclusive lower bound
 *              - upper_bound_exclusive: Exclusive upper bound
 **************************************************/
#define mlkem_debug_check_bounds MLKEM_NAMESPACE(mlkem_debug_check_bounds)
void mlkem_debug_check_bounds(const char *file, int line,
                              const char *description, const int16_t *ptr,
                              unsigned len, int lower_bound_exclusive,
                              int upper_bound_exclusive);

/* Check assertion, calling exit() upon failure
 *
 * val: Value that's asserted to be non-zero
 * msg: Message to print on failure
 *
 * Currently called CASSERT to avoid clash with CBMC assert.
 */
#define CASSERT(val, msg)                                 \
  do                                                      \
  {                                                       \
    mlkem_debug_assert(__FILE__, __LINE__, (msg), (val)); \
  } while (0)

/* Check absolute bounds of scalar
 * val: Scalar to be checked
 * abs_bound: Exclusive upper bound on absolute value to check
 * msg: Message to print on failure */
#define SCALAR_BOUND(val, abs_bound, msg) \
  CASSERT((val) > -(abs_bound) && (val) < (abs_bound), msg)

/* Check that all coefficients in array of int16_t's are non-negative
 * and below an exclusive upper bound.
 *
 * ptr: Base of array, expression of type int16_t*
 * len: Number of int16_t in array
 * high_bound: Exclusive upper bound on absolute value to check
 * msg: Message to print on failure */
#define UBOUND(ptr, len, high_bound, msg)                                 \
  do                                                                      \
  {                                                                       \
    mlkem_debug_check_bounds(__FILE__, __LINE__, (msg), (int16_t *)(ptr), \
                             (len), -1, ((high_bound)));                  \
  } while (0)

/* Check absolute bounds in array of int16_t's
 * ptr: Base of array, expression of type int16_t*
 * len: Number of int16_t in array
 * abs_bound: Exclusive upper bound on absolute value to check
 * msg: Message to print on failure */
#define BOUND(ptr, len, abs_bound, msg)                                   \
  do                                                                      \
  {                                                                       \
    mlkem_debug_check_bounds(__FILE__, __LINE__, (msg), (int16_t *)(ptr), \
                             (len), -(abs_bound), (abs_bound));           \
  } while (0)

/* Check absolute bounds on coefficients in polynomial or mulcache
 * ptr: poly* or poly_mulcache* pointer to polynomial (cache) to check
 * abs_bound: Exclusive upper bound on absolute value to check
 * msg: Message to print on failure */
#define POLY_BOUND_MSG(ptr, abs_bound, msg)                                    \
  BOUND((ptr)->coeffs, (sizeof((ptr)->coeffs) / sizeof(int16_t)), (abs_bound), \
        msg)

/* Check unsigned bounds on coefficients in polynomial or mulcache
 * ptr: poly* or poly_mulcache* pointer to polynomial (cache) to check
 * ubound: Exclusive upper bound on value to check. Inclusive lower bound is 0.
 * msg: Message to print on failure */
#define POLY_UBOUND_MSG(ptr, ubound, msg)                                    \
  UBOUND((ptr)->coeffs, (sizeof((ptr)->coeffs) / sizeof(int16_t)), (ubound), \
         msg)

/* Check absolute bounds on coefficients in polynomial
 * ptr: poly* of poly_mulcache* pointer to polynomial (cache) to check
 * abs_bound: Exclusive upper bound on absolute value to check */
#define POLY_BOUND(ptr, abs_bound) \
  POLY_BOUND_MSG((ptr), (abs_bound), "poly absolute bound for " #ptr)

/* Check unsigned bounds on coefficients in polynomial
 * ptr: poly* of poly_mulcache* pointer to polynomial (cache) to check
 * ubound: Exclusive upper bound on value to check. Inclusive lower bound is 0.
 */
#define POLY_UBOUND(ptr, ubound) \
  POLY_UBOUND_MSG((ptr), (ubound), "poly unsigned bound for " #ptr)

/* Check absolute bounds on coefficients in vector of polynomials
 * ptr: polyvec* or polyvec_mulcache* pointer to vector of polynomials to check
 * abs_bound: Exclusive upper bound on absolute value to check */
#define POLYVEC_BOUND(ptr, abs_bound)                                      \
  do                                                                       \
  {                                                                        \
    unsigned _debug_polyvec_bound_idx;                                     \
    for (_debug_polyvec_bound_idx = 0; _debug_polyvec_bound_idx < MLKEM_K; \
         _debug_polyvec_bound_idx++)                                       \
      POLY_BOUND_MSG(&(ptr)->vec[_debug_polyvec_bound_idx], (abs_bound),   \
                     "polyvec absolute bound for " #ptr ".vec[i]");        \
  } while (0)

/* Check unsigned bounds on coefficients in vector of polynomials
 * ptr: polyvec* or polyvec_mulcache* pointer to vector of polynomials to check
 * ubound: Exclusive upper bound on value to check. Inclusive lower bound is 0.
 */
#define POLYVEC_UBOUND(ptr, ubound)                                        \
  do                                                                       \
  {                                                                        \
    unsigned _debug_polyvec_bound_idx;                                     \
    for (_debug_polyvec_bound_idx = 0; _debug_polyvec_bound_idx < MLKEM_K; \
         _debug_polyvec_bound_idx++)                                       \
      POLY_UBOUND_MSG(&(ptr)->vec[_debug_polyvec_bound_idx], (ubound),     \
                      "polyvec unsigned bound for " #ptr ".vec[i]");       \
  } while (0)

#define MLKEM_CONCAT_(left, right) left##right
#define MLKEM_CONCAT(left, right) MLKEM_CONCAT_(left, right)

/* Following AWS-LC to define a C99-compliant static assert */
#define MLKEM_STATIC_ASSERT_DEFINE(cond, msg)                            \
  typedef struct                                                         \
  {                                                                      \
    unsigned int MLKEM_CONCAT(static_assertion_, msg) : (cond) ? 1 : -1; \
  } MLKEM_CONCAT(MLKEM_NAMESPACE(static_assertion_), msg)                \
      __attribute__((unused));

#define MLKEM_STATIC_ASSERT_ADD_LINE0(cond, suffix) \
  MLKEM_STATIC_ASSERT_DEFINE(cond, MLKEM_CONCAT(at_line_, suffix))
#define MLKEM_STATIC_ASSERT_ADD_LINE1(cond, line, suffix) \
  MLKEM_STATIC_ASSERT_ADD_LINE0(cond, MLKEM_CONCAT(line, suffix))
#define MLKEM_STATIC_ASSERT_ADD_LINE2(cond, suffix) \
  MLKEM_STATIC_ASSERT_ADD_LINE1(cond, __LINE__, suffix)
#define MLKEM_STATIC_ASSERT_ADD_ERROR(cond, suffix) \
  MLKEM_STATIC_ASSERT_ADD_LINE2(cond, MLKEM_CONCAT(_error_is_, suffix))
#define STATIC_ASSERT(cond, error) MLKEM_STATIC_ASSERT_ADD_ERROR(cond, error)

#else /* MLKEM_DEBUG */

#define CASSERT(val, msg) \
  do                      \
  {                       \
  } while (0)
#define SCALAR_BOUND(val, abs_bound, msg) \
  do                                      \
  {                                       \
  } while (0)
#define BOUND(ptr, len, abs_bound, msg) \
  do                                    \
  {                                     \
  } while (0)
#define POLY_BOUND(ptr, abs_bound) \
  do                               \
  {                                \
  } while (0)
#define POLYVEC_BOUND(ptr, abs_bound) \
  do                                  \
  {                                   \
  } while (0)
#define POLY_BOUND_MSG(ptr, ubound, abs_bound) \
  do                                           \
  {                                            \
  } while (0)
#define UBOUND(ptr, len, high_bound, msg) \
  do                                      \
  {                                       \
  } while (0)
#define POLY_UBOUND(ptr, ubound) \
  do                             \
  {                              \
  } while (0)
#define POLYVEC_UBOUND(ptr, ubound) \
  do                                \
  {                                 \
  } while (0)
#define POLY_UBOUND_MSG(ptr, ubound, msg) \
  do                                      \
  {                                       \
  } while (0)
#define STATIC_ASSERT(cond, error)

#endif /* MLKEM_DEBUG */

#endif /* MLKEM_DEBUG_H */
