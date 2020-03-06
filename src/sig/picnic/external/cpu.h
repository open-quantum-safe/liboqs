/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifndef CPU_H
#define CPU_H

#if defined(__GNUC__) && !(defined(__APPLE__) && (__clang_major__ <= 8)) &&                        \
  !defined(__MINGW32__) && !defined(__MINGW64__)
#define BUILTIN_CPU_SUPPORTED
#endif

#if !defined(BUILTIN_CPU_SUPPORTED)
#include <stdbool.h>
#include "oqs_picnic_macros.h"

/* CPU supports SSE2 */
#define CPU_CAP_SSE2 0x00000001
/* CPU supports popcnt */
#define CPU_CAP_POPCNT 0x00000002
/* CPU supports AVX2 + BMI2 */
#define CPU_CAP_AVX2 0x00000004
/* CPU supports NEON */
#define CPU_CAP_NEON 0x00000008

/**
 * Helper function in case __builtin_cpu_supports is not available.
 */
bool cpu_supports(unsigned int caps);
#endif

#endif
