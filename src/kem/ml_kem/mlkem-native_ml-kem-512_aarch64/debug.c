/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

/* NOTE: You can remove this file unless you compile with MLKEM_DEBUG. */

#include "common.h"

#if !defined(MLKEM_NATIVE_MULTILEVEL_BUILD_NO_SHARED) && defined(MLKEM_DEBUG)


#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

#define MLKEM_NATIVE_DEBUG_ERROR_HEADER "[ERROR:%s:%04d] "

void mlkem_debug_assert(const char *file, int line, const int val)
{
  if (val == 0)
  {
    fprintf(stderr,
            MLKEM_NATIVE_DEBUG_ERROR_HEADER "Assertion failed (value %d)\n",
            file, line, val);
    exit(1);
  }
}

void mlkem_debug_check_bounds(const char *file, int line, const int16_t *ptr,
                              unsigned len, int lower_bound_exclusive,
                              int upper_bound_exclusive)
{
  int err = 0;
  unsigned i;
  for (i = 0; i < len; i++)
  {
    int16_t val = ptr[i];
    if (!(val > lower_bound_exclusive && val < upper_bound_exclusive))
    {
      fprintf(
          stderr,
          MLKEM_NATIVE_DEBUG_ERROR_HEADER
          "Bounds assertion failed: Index %u, value %d out of bounds (%d,%d)\n",
          file, line, i, (int)val, lower_bound_exclusive,
          upper_bound_exclusive);
      err = 1;
    }
  }

  if (err == 1)
    exit(1);
}

#else /* !MLKEM_NATIVE_MULTILEVEL_BUILD_NO_SHARED && MLKEM_DEBUG */

MLKEM_NATIVE_EMPTY_CU(debug)

#endif /* !MLKEM_NATIVE_MULTILEVEL_BUILD_NO_SHARED && MLKEM_DEBUG */
