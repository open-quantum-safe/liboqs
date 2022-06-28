/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifndef PICNIC_INSTANCES_H
#define PICNIC_INSTANCES_H

#include "oqs_picnic_macros.h"

#include "lowmc.h"
#if defined(WITH_ZKBPP)
#include "mpc_lowmc.h"
#endif
#if defined(WITH_KKW)
#include "picnic3_simulate.h"
#endif
#include "picnic.h"

#define SALT_SIZE 32

/* max digest and seed size */
#if defined(WITH_LOWMC_255_255_4) || defined(WITH_LOWMC_256_256_38)
#define MAX_DIGEST_SIZE 64
#define MAX_SEED_SIZE 32
#elif defined(WITH_LOWMC_192_192_4) || defined(WITH_LOWMC_192_192_30)
#define MAX_DIGEST_SIZE 48
#define MAX_SEED_SIZE 24
#elif defined(WITH_LOWMC_129_129_4) || defined(WITH_LOWMC_128_128_20)
#define MAX_DIGEST_SIZE 32
#define MAX_SEED_SIZE 16
#endif
/* max number of MPC parties for KWW; always 16 */
#define MAX_KKW_MPC_PARTIES 16

PICNIC_BEGIN_C_DECL

typedef struct picnic_instance_t {
  const lowmc_parameters_t lowmc;
  const uint16_t num_rounds;       // T
  const uint8_t digest_size;       // bytes
  const uint8_t seed_size;         // bytes
  const uint8_t input_output_size; // bytes
  const uint8_t view_size;         // bytes
  const uint8_t num_opened_rounds; // u (KKW only)
  const uint8_t num_MPC_parties;   // N (KKW only)
} picnic_instance_t;

ATTR_PURE const picnic_instance_t* picnic_instance_get(picnic_params_t param);

ATTR_CONST static inline bool picnic_instance_is_unruh(picnic_params_t param) {
  return param == Picnic_L1_UR || param == Picnic_L3_UR || param == Picnic_L5_UR;
}

PICNIC_EXPORT size_t PICNIC_CALLING_CONVENTION picnic_get_lowmc_block_size(picnic_params_t param);

/* Prefix values for domain separation */
static const uint8_t HASH_PREFIX_0 = 0;
static const uint8_t HASH_PREFIX_1 = 1;
static const uint8_t HASH_PREFIX_2 = 2;
static const uint8_t HASH_PREFIX_3 = 3;
static const uint8_t HASH_PREFIX_4 = 4;
static const uint8_t HASH_PREFIX_5 = 5;

PICNIC_END_C_DECL

#endif
