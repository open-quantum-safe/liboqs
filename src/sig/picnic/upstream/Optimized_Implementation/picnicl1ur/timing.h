/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifndef TIMING_H
#define TIMING_H

#include <stdint.h>
#include <time.h>

typedef union {
#ifdef WITH_DETAILED_TIMING
  struct {
    struct {
      uint64_t lowmc_init, keygen, pubkey;
    } gen;
    struct {
      uint64_t rand, secret_sharing, lowmc_enc, views, challenge, hash;
    } sign;
    struct {
      uint64_t challenge, output_shares, output_views, verify, hash;
    } verify;
    uint64_t size;
  };
#else
  struct {
    uint64_t keygen, sign, verify, size, max_size;
  };
#endif
  uint64_t data[0];
} timing_and_size_t;

#define gettime gettime_clock

#define TIMING_SCALE (1000000 / CLOCKS_PER_SEC);

#ifdef WITH_OPENMP
#include <omp.h>

static inline uint64_t gettime_clock(void) {
  return omp_get_wtime() * 1000 * 1000;
}
#else
static inline uint64_t gettime_clock(void) {
  return clock() * TIMING_SCALE;
}
#endif

#ifdef WITH_DETAILED_TIMING
extern timing_and_size_t* timing_and_size;

#define TIME_FUNCTION uint64_t start_time
#define START_TIMING start_time = gettime()
#define END_TIMING(dst) dst     = gettime() - start_time

#else

#define TIME_FUNCTION                                                                              \
  do {                                                                                             \
  } while (0)
#define START_TIMING                                                                               \
  do {                                                                                             \
  } while (0)
#define END_TIMING(dst)                                                                            \
  do {                                                                                             \
  } while (0)

#endif

#endif
