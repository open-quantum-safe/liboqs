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

// instance handling

// L1, L3, and L5 instances with partial Sbox layer
#if defined(WITH_LOWMC_128_128_20)
#include "lowmc_128_128_20.h"
#else
#define lowmc_parameters_128_128_20                                                                \
  { 0, 0, 0 }
#endif
#if defined(WITH_LOWMC_192_192_30)
#include "lowmc_192_192_30.h"
#else
#define lowmc_parameters_192_192_30                                                                \
  { 0, 0, 0 }
#endif
#if defined(WITH_LOWMC_256_256_38)
#include "lowmc_256_256_38.h"
#else
#define lowmc_parameters_256_256_38                                                                \
  { 0, 0, 0 }
#endif

// L1, L3, and L5 instances with full Sbox layer
#if defined(WITH_LOWMC_129_129_4)
#include "lowmc_129_129_4.h"
#else
#define lowmc_parameters_129_129_4                                                                 \
  { 0, 0, 0 }
#endif
#if defined(WITH_LOWMC_192_192_4)
#include "lowmc_192_192_4.h"
#else
#define lowmc_parameters_192_192_4                                                                 \
  { 0, 0, 0 }
#endif
#if defined(WITH_LOWMC_255_255_4)
#include "lowmc_255_255_4.h"
#else
#define lowmc_parameters_255_255_4                                                                 \
  { 0, 0, 0 }
#endif

#if defined(WITH_ZKBPP)
#define PARAMETER_SET_ZKBPP(params, digest_size, seed_size, num_rounds, input_output_size,         \
                            view_size)                                                             \
  { params, num_rounds, digest_size, seed_size, input_output_size, view_size, 0, 0 }
#else
#define PARAMETER_SET_ZKBPP(params, digest_size, seed_size, num_rounds, input_output_size,         \
                            view_size)                                                             \
  { {0, 0, 0}, 0, 0, 0, 0, 0, 0, 0 }
#endif

#if defined(WITH_KKW)
#define PARAMETER_SET_KKW(params, digest_size, seed_size, num_rounds, num_opened_rounds,           \
                          num_MPC_parties, input_output_size, view_size)                           \
  {                                                                                                \
    params, num_rounds, digest_size, seed_size, input_output_size, view_size, num_opened_rounds,   \
        num_MPC_parties                                                                            \
  }
#else
#define PARAMETER_SET_KKW(params, digest_size, seed_size, num_rounds, num_opened_rounds,           \
                          num_MPC_parties, input_output_size, view_size)                           \
  { {0, 0, 0}, 0, 0, 0, 0, 0, 0, 0 }

#endif

static const picnic_instance_t instances[] = {
    /* ZKB++ with partial LowMC instances */
    PARAMETER_SET_ZKBPP(lowmc_parameters_128_128_20, 32, 16, 219, 16, 75),
    PARAMETER_SET_ZKBPP(lowmc_parameters_192_192_30, 48, 24, 329, 24, 113),
    PARAMETER_SET_ZKBPP(lowmc_parameters_256_256_38, 64, 32, 438, 32, 143),
    /* KKW with full LowMC instances */
    PARAMETER_SET_KKW(lowmc_parameters_129_129_4, 32, 16, 250, 36, 16, 17, 65),
    PARAMETER_SET_KKW(lowmc_parameters_192_192_4, 48, 24, 419, 52, 16, 24, 96),
    PARAMETER_SET_KKW(lowmc_parameters_255_255_4, 64, 32, 601, 68, 16, 32, 128),
    /* ZKB++ with full LowMC instances */
    PARAMETER_SET_ZKBPP(lowmc_parameters_129_129_4, 32, 16, 219, 17, 65),
    PARAMETER_SET_ZKBPP(lowmc_parameters_192_192_4, 48, 24, 329, 24, 96),
    PARAMETER_SET_ZKBPP(lowmc_parameters_255_255_4, 64, 32, 438, 32, 128),
};

const picnic_instance_t* picnic_instance_get(picnic_params_t param) {
  if (param <= PARAMETER_SET_INVALID || param >= PARAMETER_SET_MAX_INDEX) {
    return NULL;
  }
#if !defined(WITH_UNRUH)
  // because the FS and Unruh instances are interleaved, we need to explicitely check for them
  if (picnic_instance_is_unruh(param)) {
    return NULL;
  }
#endif

  const picnic_instance_t* pp =
      &instances[param <= Picnic_L5_UR ? (param - 1) / 2 : param - Picnic3_L1 + 3];
  // check if the LowMC instance and ZKBPP/KKW are enabled
  return pp->lowmc.n ? pp : NULL;
}
