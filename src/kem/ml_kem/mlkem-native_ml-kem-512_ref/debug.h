/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef MLKEM_DEBUG_H
#define MLKEM_DEBUG_H
#include "common.h"

#if defined(MLKEM_DEBUG)
#include <stdint.h>

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
 *              - val: Value asserted to be non-zero
 **************************************************/
#define mlkem_debug_assert MLKEM_NAMESPACE(mlkem_debug_assert)
void mlkem_debug_assert(const char *file, int line, const int val);

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
 *              - ptr: Base of array to be checked
 *              - len: Number of int16_t in ptr
 *              - lower_bound_exclusive: Exclusive lower bound
 *              - upper_bound_exclusive: Exclusive upper bound
 **************************************************/
#define mlkem_debug_check_bounds MLKEM_NAMESPACE(mlkem_debug_check_bounds)
void mlkem_debug_check_bounds(const char *file, int line, const int16_t *ptr,
                              unsigned len, int lower_bound_exclusive,
                              int upper_bound_exclusive);

/* Check assertion, calling exit() upon failure
 *
 * val: Value that's asserted to be non-zero
 */
#define debug_assert(val) mlkem_debug_assert(__FILE__, __LINE__, (val))

/* Check bounds in array of int16_t's
 * ptr: Base of int16_t array; will be explicitly cast to int16_t*,
 *      so you may pass a byte-compatible type such as poly or polyvec.
 * len: Number of int16_t in array
 * value_lb: Inclusive lower value bound
 * value_ub: Exclusive upper value bound */
#define debug_assert_bound(ptr, len, value_lb, value_ub)                      \
  mlkem_debug_check_bounds(__FILE__, __LINE__, (const int16_t *)(ptr), (len), \
                           (value_lb)-1, (value_ub))

/* Check absolute bounds in array of int16_t's
 * ptr: Base of array, expression of type int16_t*
 * len: Number of int16_t in array
 * value_abs_bd: Exclusive absolute upper bound */
#define debug_assert_abs_bound(ptr, len, value_abs_bd) \
  debug_assert_bound((ptr), (len), (-(value_abs_bd) + 1), (value_abs_bd))

/* Version of bounds assertions for 2-dimensional arrays */
#define debug_assert_bound_2d(ptr, len0, len1, value_lb, value_ub) \
  debug_assert_bound((ptr), ((len0) * (len1)), (value_lb), (value_ub))

#define debug_assert_abs_bound_2d(ptr, len0, len1, value_abs_bd) \
  debug_assert_abs_bound((ptr), ((len0) * (len1)), (value_abs_bd))

/* When running CBMC, convert debug assertions into proof obligations */
#elif defined(CBMC)

#include "../cbmc.h"

#define debug_assert(val) cassert(val)

#define debug_assert_bound(ptr, len, value_lb, value_ub) \
  cassert(array_bound(((int16_t *)(ptr)), 0, (len), (value_lb), (value_ub)))

#define debug_assert_abs_bound(ptr, len, value_abs_bd) \
  cassert(array_abs_bound(((int16_t *)(ptr)), 0, (len), (value_abs_bd)))

/* Because of https://github.com/diffblue/cbmc/issues/8570, we can't
 * just use a single flattened array_bound(...) here. */
#define debug_assert_bound_2d(ptr, M, N, value_lb, value_ub)           \
  cassert(forall(kN, 0, (M),                                           \
                 array_bound(&((int16_t(*)[(N)])(ptr))[kN][0], 0, (N), \
                             (value_lb), (value_ub))))

#define debug_assert_abs_bound_2d(ptr, M, N, value_abs_bd)                 \
  cassert(forall(kN, 0, (M),                                               \
                 array_abs_bound(&((int16_t(*)[(N)])(ptr))[kN][0], 0, (N), \
                                 (value_abs_bd))))

#else /* MLKEM_DEBUG */

#define debug_assert(val) \
  do                      \
  {                       \
  } while (0)
#define debug_assert_bound(ptr, len, value_lb, value_ub) \
  do                                                     \
  {                                                      \
  } while (0)
#define debug_assert_abs_bound(ptr, len, value_abs_bd) \
  do                                                   \
  {                                                    \
  } while (0)

#define debug_assert_bound_2d(ptr, len0, len1, value_lb, value_ub) \
  do                                                               \
  {                                                                \
  } while (0)

#define debug_assert_abs_bound_2d(ptr, len0, len1, value_abs_bd) \
  do                                                             \
  {                                                              \
  } while (0)


#endif /* MLKEM_DEBUG */
#endif /* MLKEM_DEBUG_H */
