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

#include "picnic_instances.h"

// Prefix values for domain separation
const uint8_t HASH_PREFIX_0 = 0;
const uint8_t HASH_PREFIX_1 = 1;
const uint8_t HASH_PREFIX_2 = 2;
const uint8_t HASH_PREFIX_3 = 3;
const uint8_t HASH_PREFIX_4 = 4;
const uint8_t HASH_PREFIX_5 = 5;

// instance handling

// L1, L3, and L5 LowMC instances
#if defined(WITH_LOWMC_128_128_20)
#include "lowmc_128_128_20.h"
#define LOWMC_L1_OR_NULL &lowmc_128_128_20
#else
#define LOWMC_L1_OR_NULL NULL
#endif
#if defined(WITH_LOWMC_192_192_30)
#include "lowmc_192_192_30.h"
#define LOWMC_L3_OR_NULL &lowmc_192_192_30
#else
#define LOWMC_L3_OR_NULL NULL
#endif
#if defined(WITH_LOWMC_256_256_38)
#include "lowmc_256_256_38.h"
#define LOWMC_L5_OR_NULL &lowmc_256_256_38
#else
#define LOWMC_L5_OR_NULL NULL
#endif

// L1, L3, and L5 lowmc instances with 1 SBOX
#if defined(WITH_LOWMC_128_128_182)
#include "lowmc_128_128_182.h"
#define LOWMC_L1_1_OR_NULL &lowmc_128_128_182
#else
#define LOWMC_L1_1_OR_NULL NULL
#endif
#if defined(WITH_LOWMC_192_192_284)
#include "lowmc_192_192_284.h"
#define LOWMC_L3_1_OR_NULL &lowmc_192_192_284
#else
#define LOWMC_L3_1_OR_NULL NULL
#endif
#if defined(WITH_LOWMC_256_256_363)
#include "lowmc_256_256_363.h"
#define LOWMC_L5_1_OR_NULL &lowmc_256_256_363
#else
#define LOWMC_L5_1_OR_NULL NULL
#endif

#if defined(WITH_ZKBPP)
#define ENABLE_ZKBPP(x) x
#else
#define ENABLE_ZKBPP(x) NULL
#endif

#if defined(WITH_KKW)
#define ENABLE_KKW(x) x
#else
#define ENABLE_KKW(x) NULL
#endif

#if defined(WITH_ZKBPP) && defined(WITH_KKW)
#define NULL_FNS                                                                                   \
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL }
#elif defined(WITH_ZKBPP)
#define NULL_FNS                                                                                   \
  { NULL, NULL, NULL, NULL, NULL }
#else
#define NULL_FNS                                                                                   \
  { NULL }
#endif

static picnic_instance_t instances[PARAMETER_SET_MAX_INDEX] = {
    {NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, PARAMETER_SET_INVALID, TRANSFORM_FS, NULL_FNS},
    {ENABLE_ZKBPP(LOWMC_L1_OR_NULL), 32, 16, 219, 219, 3, 16, 16, 75, 30, 55, 0, 0,
     PICNIC_SIGNATURE_SIZE_Picnic_L1_FS, Picnic_L1_FS, TRANSFORM_FS, NULL_FNS},
    {ENABLE_ZKBPP(LOWMC_L1_OR_NULL), 32, 16, 219, 219, 3, 16, 16, 75, 30, 55, 91, 107,
     PICNIC_SIGNATURE_SIZE_Picnic_L1_UR, Picnic_L1_UR, TRANSFORM_UR, NULL_FNS},
    {ENABLE_ZKBPP(LOWMC_L3_OR_NULL), 48, 24, 329, 329, 3, 24, 24, 113, 30, 83, 0, 0,
     PICNIC_SIGNATURE_SIZE_Picnic_L3_FS, Picnic_L3_FS, TRANSFORM_FS, NULL_FNS},
    {ENABLE_ZKBPP(LOWMC_L3_OR_NULL), 48, 24, 329, 329, 3, 24, 24, 113, 30, 83, 137, 161,
     PICNIC_SIGNATURE_SIZE_Picnic_L3_UR, Picnic_L3_UR, TRANSFORM_UR, NULL_FNS},
    {ENABLE_ZKBPP(LOWMC_L5_OR_NULL), 64, 32, 438, 438, 3, 32, 32, 143, 30, 110, 0, 0,
     PICNIC_SIGNATURE_SIZE_Picnic_L5_FS, Picnic_L5_FS, TRANSFORM_FS, NULL_FNS},
    {ENABLE_ZKBPP(LOWMC_L5_OR_NULL), 64, 32, 438, 438, 3, 32, 32, 143, 30, 110, 175, 207,
     PICNIC_SIGNATURE_SIZE_Picnic_L5_UR, Picnic_L5_UR, TRANSFORM_UR, NULL_FNS},
    // Picnic2 params
    {ENABLE_KKW(LOWMC_L1_OR_NULL), 32, 16, 343, 27, 64, 16, 16, 75, 30, 55, 0, 0,
     PICNIC_SIGNATURE_SIZE_Picnic2_L1_FS, Picnic2_L1_FS, TRANSFORM_FS, NULL_FNS},
    {ENABLE_KKW(LOWMC_L3_OR_NULL), 48, 24, 570, 39, 64, 24, 24, 113, 30, 83, 0, 0,
     PICNIC_SIGNATURE_SIZE_Picnic2_L3_FS, Picnic2_L3_FS, TRANSFORM_FS, NULL_FNS},
    {ENABLE_KKW(LOWMC_L5_OR_NULL), 64, 32, 803, 50, 64, 32, 32, 143, 30, 110, 0, 0,
     PICNIC_SIGNATURE_SIZE_Picnic2_L5_FS, Picnic2_L5_FS, TRANSFORM_FS, NULL_FNS},
    // Picnic with LowMC with m=1
    {ENABLE_ZKBPP(LOWMC_L1_1_OR_NULL), 32, 16, 219, 219, 3, 16, 16, 69, 3, 55, 0, 0,
     PICNIC_SIGNATURE_SIZE_Picnic_L1_1_FS, Picnic_L1_1_FS, TRANSFORM_FS, NULL_FNS},
    {ENABLE_ZKBPP(LOWMC_L1_1_OR_NULL), 32, 16, 219, 219, 3, 16, 16, 69, 3, 55, 87, 103,
     PICNIC_SIGNATURE_SIZE_Picnic_L1_1_UR, Picnic_L1_1_UR, TRANSFORM_UR, NULL_FNS},
    {ENABLE_ZKBPP(LOWMC_L3_1_OR_NULL), 48, 24, 329, 329, 3, 24, 24, 107, 3, 83, 0, 0,
     PICNIC_SIGNATURE_SIZE_Picnic_L3_1_FS, Picnic_L3_1_FS, TRANSFORM_FS, NULL_FNS},
    {ENABLE_ZKBPP(LOWMC_L3_1_OR_NULL), 48, 24, 329, 329, 3, 24, 24, 107, 3, 83, 131, 155,
     PICNIC_SIGNATURE_SIZE_Picnic_L3_1_UR, Picnic_L3_1_UR, TRANSFORM_UR, NULL_FNS},
    {ENABLE_ZKBPP(LOWMC_L5_1_OR_NULL), 64, 32, 438, 438, 3, 32, 32, 137, 3, 110, 0, 0,
     PICNIC_SIGNATURE_SIZE_Picnic_L5_1_FS, Picnic_L5_1_FS, TRANSFORM_FS, NULL_FNS},
    {ENABLE_ZKBPP(LOWMC_L5_1_OR_NULL), 64, 32, 438, 438, 3, 32, 32, 137, 3, 110, 169, 201,
     PICNIC_SIGNATURE_SIZE_Picnic_L5_1_UR, Picnic_L5_1_UR, TRANSFORM_UR, NULL_FNS}};
static bool instance_initialized[PARAMETER_SET_MAX_INDEX];

static bool create_instance(picnic_instance_t* pp) {
  if (!pp->lowmc) {
    return false;
  }

  pp->impls.lowmc                 = lowmc_get_implementation(pp->lowmc);
#if defined(WITH_ZKBPP)
  pp->impls.lowmc_store           = lowmc_store_get_implementation(pp->lowmc);
  pp->impls.zkbpp_lowmc           = get_zkbpp_lowmc_implementation(pp->lowmc);
  pp->impls.zkbpp_lowmc_verify    = get_zkbpp_lowmc_verify_implementation(pp->lowmc);
  pp->impls.mzd_share             = get_zkbpp_share_implentation(pp->lowmc);
#endif
#if defined(WITH_KKW)
  pp->impls.lowmc_aux             = lowmc_compute_aux_get_implementation(pp->lowmc);
  pp->impls.lowmc_simulate_online = lowmc_simulate_online_get_implementation(pp->lowmc);
#endif

  return true;
}

const picnic_instance_t* picnic_instance_get(picnic_params_t param) {
  if (param <= PARAMETER_SET_INVALID || param >= PARAMETER_SET_MAX_INDEX) {
    return NULL;
  }

  if (!instance_initialized[param]) {
    if (!create_instance(&instances[param])) {
      return NULL;
    }
    instance_initialized[param] = true;
  }

  return &instances[param];
}
