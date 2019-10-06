/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "io.h"
#include "lowmc.h"
#include "lowmc_pars.h"
#include "mzd_additional.h"
#include "picnic2_impl.h"

#if defined(WITH_OPT)
#include "simd.h"
#endif

#if !defined(_MSC_VER)
#include <stdalign.h>
#endif
#include <string.h>

static uint64_t sbox_layer_10_bitsliced_uint64(uint64_t in) {
  // a, b, c
  const uint64_t x0s = (in & MASK_X0I) << 2;
  const uint64_t x1s = (in & MASK_X1I) << 1;
  const uint64_t x2m = in & MASK_X2I;

  // (b & c) ^ a
  const uint64_t t0 = (x1s & x2m) ^ x0s;
  // (c & a) ^ a ^ b
  const uint64_t t1 = (x0s & x2m) ^ x0s ^ x1s;
  // (a & b) ^ a ^ b ^c
  const uint64_t t2 = (x0s & x1s) ^ x0s ^ x1s ^ x2m;

  return (in & MASK_MASK) ^ (t0 >> 2) ^ (t1 >> 1) ^ t2;
}

/**
 * S-box for m = 10
 */
static void sbox_layer_10_uint64(uint64_t* d) {
  *d = sbox_layer_10_bitsliced_uint64(*d);
}

#if defined(WITH_LOWMC_M1)
static uint64_t sbox_layer_1_bitsliced_uint64(uint64_t in) {
  // a, b, c
  const uint64_t x0s = (in & MASK_X0I_1) << 2;
  const uint64_t x1s = (in & MASK_X1I_1) << 1;
  const uint64_t x2m = in & MASK_X2I_1;

  // (b & c) ^ a
  const uint64_t t0 = (x1s & x2m) ^ x0s;
  // (c & a) ^ a ^ b
  const uint64_t t1 = (x0s & x2m) ^ x0s ^ x1s;
  // (a & b) ^ a ^ b ^c
  const uint64_t t2 = (x0s & x1s) ^ x0s ^ x1s ^ x2m;

  return (in & MASK_MASK_1) ^ (t0 >> 2) ^ (t1 >> 1) ^ t2;
}

/**
 * S-box for m = 1
 */
static void sbox_layer_1_uint64(uint64_t* d) {
  *d = sbox_layer_1_bitsliced_uint64(*d);
}
#endif

#if defined(WITH_LOWMC_128_128_20)
#include "lowmc_128_128_20.h"
#endif
#if defined(WITH_LOWMC_192_192_30)
#include "lowmc_192_192_30.h"
#endif
#if defined(WITH_LOWMC_256_256_38)
#include "lowmc_256_256_38.h"
#endif
#if defined(WITH_LOWMC_128_128_182)
#include "lowmc_128_128_182.h"
#endif
#if defined(WITH_LOWMC_192_192_284)
#include "lowmc_192_192_284.h"
#endif
#if defined(WITH_LOWMC_256_256_363)
#include "lowmc_256_256_363.h"
#endif

// uint64 based implementation
#include "lowmc_fns_uint64_L1.h"
#define LOWMC lowmc_uint64_128
#include "lowmc.c.i"

#include "lowmc_fns_uint64_L3.h"
#undef LOWMC
#define LOWMC lowmc_uint64_192
#include "lowmc.c.i"

#include "lowmc_fns_uint64_L5.h"
#undef LOWMC
#define LOWMC lowmc_uint64_256
#include "lowmc.c.i"

#if defined(WITH_OPT)
#if defined(WITH_SSE2) || defined(WITH_NEON)
#if defined(WITH_SSE2)
#define FN_ATTR ATTR_TARGET_SSE2
#endif

// L1 using SSE2/NEON
#include "lowmc_fns_s128_L1.h"
#undef LOWMC
#define LOWMC lowmc_s128_128
#include "lowmc.c.i"

// L3 using SSE2/NEON
#include "lowmc_fns_s128_L3.h"
#undef LOWMC
#define LOWMC lowmc_s128_192
#include "lowmc.c.i"

// L5 using SSE2/NEON
#include "lowmc_fns_s128_L5.h"
#undef LOWMC
#define LOWMC lowmc_s128_256
#include "lowmc.c.i"

#undef FN_ATTR
#endif

#if defined(WITH_AVX2)
#define FN_ATTR ATTR_TARGET_AVX2

// L1 using AVX2
#include "lowmc_fns_s256_L1.h"
#undef LOWMC
#define LOWMC lowmc_s256_128
#include "lowmc.c.i"

// L3 using AVX2
#include "lowmc_fns_s256_L3.h"
#undef LOWMC
#define LOWMC lowmc_s256_192
#include "lowmc.c.i"

// L5 using AVX2
#include "lowmc_fns_s256_L5.h"
#undef LOWMC
#define LOWMC lowmc_s256_256
#include "lowmc.c.i"

#undef FN_ATTR

#endif
#endif

lowmc_implementation_f lowmc_get_implementation(const lowmc_t* lowmc) {
#if defined(WITH_LOWMC_M1)
  ASSUME(lowmc->m == 10 || lowmc->m == 1);
#else
  ASSUME(lowmc->m == 10);
#endif
  ASSUME(lowmc->n == 128 || lowmc->n == 192 || lowmc->n == 256);

#if defined(WITH_OPT)
#if defined(WITH_AVX2)
  if (CPU_SUPPORTS_AVX2) {
    if (lowmc->m == 10) {
      switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_20)
      case 128:
        return lowmc_s256_128_10;
#endif
#if defined(WITH_LOWMC_192_192_30)
      case 192:
        return lowmc_s256_192_10;
#endif
#if defined(WITH_LOWMC_256_256_38)
      case 256:
        return lowmc_s256_256_10;
#endif
      }
    }
#if defined(WITH_LOWMC_M1)
    if (lowmc->m == 1) {
      switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_182)
      case 128:
        return lowmc_s256_128_1;
#endif
#if defined(WITH_LOWMC_192_192_284)
      case 192:
        return lowmc_s256_192_1;
#endif
#if defined(WITH_LOWMC_256_256_363)
      case 256:
        return lowmc_s256_256_1;
#endif
      }
    }
#endif
  }
#endif
#if defined(WITH_SSE2) || defined(WITH_NEON)
  if (CPU_SUPPORTS_SSE2 || CPU_SUPPORTS_NEON) {
    if (lowmc->m == 10) {
      switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_20)
      case 128:
        return lowmc_s128_128_10;
#endif
#if defined(WITH_LOWMC_192_192_30)
      case 192:
        return lowmc_s128_192_10;
#endif
#if defined(WITH_LOWMC_256_256_38)
      case 256:
        return lowmc_s128_256_10;
#endif
      }
    }
#if defined(WITH_LOWMC_M1)
    if (lowmc->m == 1) {
      switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_182)
      case 128:
        return lowmc_s128_128_1;
#endif
#if defined(WITH_LOWMC_192_192_284)
      case 192:
        return lowmc_s128_192_1;
#endif
#if defined(WITH_LOWMC_256_256_363)
      case 256:
        return lowmc_s128_256_1;
#endif
      }
    }
#endif
  }
#endif
#endif

  if (lowmc->m == 10) {
    switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_20)
    case 128:
      return lowmc_uint64_128_10;
#endif
#if defined(WITH_LOWMC_192_192_30)
    case 192:
      return lowmc_uint64_192_10;
#endif
#if defined(WITH_LOWMC_256_256_38)
    case 256:
      return lowmc_uint64_256_10;
#endif
    }
  }

#if defined(WITH_LOWMC_M1)
  if (lowmc->m == 1) {
    switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_182)
    case 128:
      return lowmc_uint64_128_1;
#endif
#if defined(WITH_LOWMC_192_192_284)
    case 192:
      return lowmc_uint64_192_1;
#endif
#if defined(WITH_LOWMC_256_256_363)
    case 256:
      return lowmc_uint64_256_1;
#endif
    }
  }
#endif

  return NULL;
}

lowmc_store_implementation_f lowmc_store_get_implementation(const lowmc_t* lowmc) {
#if defined(WITH_LOWMC_M1)
  ASSUME(lowmc->m == 10 || lowmc->m == 1);
#else
  ASSUME(lowmc->m == 10);
#endif
  ASSUME(lowmc->n == 128 || lowmc->n == 192 || lowmc->n == 256);

#if defined(WITH_OPT)
#if defined(WITH_AVX2)
  if (CPU_SUPPORTS_AVX2) {
    if (lowmc->m == 10) {
      switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_20)
      case 128:
        return lowmc_s256_128_store_10;
#endif
#if defined(WITH_LOWMC_192_192_30)
      case 192:
        return lowmc_s256_192_store_10;
#endif
#if defined(WITH_LOWMC_256_256_38)
      case 256:
        return lowmc_s256_256_store_10;
#endif
      }
    }
#if defined(WITH_LOWMC_M1)
    if (lowmc->m == 1) {
      switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_182)
      case 128:
        return lowmc_s256_128_store_1;
#endif
#if defined(WITH_LOWMC_192_192_284)
      case 192:
        return lowmc_s256_192_store_1;
#endif
#if defined(WITH_LOWMC_256_256_363)
      case 256:
        return lowmc_s256_256_store_1;
#endif
      }
    }
#endif
  }
#endif
#if defined(WITH_SSE2) || defined(WITH_NEON)
  if (CPU_SUPPORTS_SSE2 || CPU_SUPPORTS_NEON) {
    if (lowmc->m == 10) {
      switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_20)
      case 128:
        return lowmc_s128_128_store_10;
#endif
#if defined(WITH_LOWMC_192_192_30)
      case 192:
        return lowmc_s128_192_store_10;
#endif
#if defined(WITH_LOWMC_256_256_38)
      case 256:
        return lowmc_s128_256_store_10;
#endif
      }
    }
#if defined(WITH_LOWMC_M1)
    if (lowmc->m == 1) {
      switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_182)
      case 128:
        return lowmc_s128_128_store_1;
#endif
#if defined(WITH_LOWMC_192_192_284)
      case 192:
        return lowmc_s128_192_store_1;
#endif
#if defined(WITH_LOWMC_256_256_363)
      case 256:
        return lowmc_s128_256_store_1;
#endif
      }
    }
#endif
  }
#endif
#endif

  if (lowmc->m == 10) {
    switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_20)
    case 128:
      return lowmc_uint64_128_store_10;
#endif
#if defined(WITH_LOWMC_192_192_30)
    case 192:
      return lowmc_uint64_192_store_10;
#endif
#if defined(WITH_LOWMC_256_256_38)
    case 256:
      return lowmc_uint64_256_store_10;
#endif
    }
  }

#if defined(WITH_LOWMC_M1)
  if (lowmc->m == 1) {
    switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_182)
    case 128:
      return lowmc_uint64_128_store_1;
#endif
#if defined(WITH_LOWMC_192_192_284)
    case 192:
      return lowmc_uint64_192_store_1;
#endif
#if defined(WITH_LOWMC_256_256_363)
    case 256:
      return lowmc_uint64_256_store_1;
#endif
    }
  }
#endif

  return NULL;
}

lowmc_compute_aux_implementation_f lowmc_compute_aux_get_implementation(const lowmc_t* lowmc) {
#if defined(WITH_LOWMC_M1)
  ASSUME(lowmc->m == 10 || lowmc->m == 1);
#else
  ASSUME(lowmc->m == 10);
#endif
  ASSUME(lowmc->n == 128 || lowmc->n == 192 || lowmc->n == 256);

#if defined(WITH_OPT)
#if defined(WITH_AVX2)
  if (CPU_SUPPORTS_AVX2) {
    if (lowmc->m == 10) {
      switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_20)
      case 128:
        return lowmc_s256_128_compute_aux_10;
#endif
#if defined(WITH_LOWMC_192_192_30)
      case 192:
        return lowmc_s256_192_compute_aux_10;
#endif
#if defined(WITH_LOWMC_256_256_38)
      case 256:
        return lowmc_s256_256_compute_aux_10;
#endif
      }
    }
  }
#endif
#if defined(WITH_SSE2) || defined(WITH_NEON)
  if (CPU_SUPPORTS_SSE2 || CPU_SUPPORTS_NEON) {
    if (lowmc->m == 10) {
      switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_20)
      case 128:
        return lowmc_s128_128_compute_aux_10;
#endif
#if defined(WITH_LOWMC_192_192_30)
      case 192:
        return lowmc_s128_192_compute_aux_10;
#endif
#if defined(WITH_LOWMC_256_256_38)
      case 256:
        return lowmc_s128_256_compute_aux_10;
#endif
      }
    }
  }
#endif
#endif

  if (lowmc->m == 10) {
    switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_20)
    case 128:
      return lowmc_uint64_128_compute_aux_10;
#endif
#if defined(WITH_LOWMC_192_192_30)
    case 192:
      return lowmc_uint64_192_compute_aux_10;
#endif
#if defined(WITH_LOWMC_256_256_38)
    case 256:
      return lowmc_uint64_256_compute_aux_10;
#endif
    }
  }

  return NULL;
}
