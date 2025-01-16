/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#include "../common.h"

#if defined(MLKEM_DEBUG)

#include <stdio.h>
#include "debug.h"

#define MLKEM_NATIVE_DEBUG_ERROR_HEADER "[ERROR:%s:%04d] "

void mlkem_debug_assert(const char *file, int line, const char *description,
                        const int val)
{
  if (val == 0)
  {
    fprintf(stderr,
            MLKEM_NATIVE_DEBUG_ERROR_HEADER "Assertion failed: %s (value %d)\n",
            file, line, description, val);
    exit(1);
  }
}

void mlkem_debug_check_bounds(const char *file, int line,
                              const char *description, const int16_t *ptr,
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
      fprintf(stderr,
              MLKEM_NATIVE_DEBUG_ERROR_HEADER
              "%s, index %u, value %d out of bounds (%d,%d)\n",
              file, line, description, i, (int)val, lower_bound_exclusive,
              upper_bound_exclusive);
      err = 1;
    }
  }

  if (err == 1)
    exit(1);
}

#else /* MLKEM_DEBUG */

#define empty_cu_debug MLKEM_NAMESPACE(empty_cu_debug)
int empty_cu_debug;

#endif /* MLKEM_DEBUG */
