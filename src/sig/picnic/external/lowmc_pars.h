/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifndef LOWMC_PARS_H
#define LOWMC_PARS_H

#include <stddef.h>

#include "mzd_additional.h"

typedef mzd_local_t lowmc_key_t;

#define MAX_LOWMC_BLOCK_SIZE 32
#define MAX_LOWMC_BLOCK_SIZE_BITS (MAX_LOWMC_BLOCK_SIZE * 8)
#define MAX_LOWMC_KEY_SIZE MAX_LOWMC_BLOCK_SIZE
#define MAX_LOWMC_KEY_SIZE_BITS (MAX_LOWMC_KEY_SIZE * 8)
#define MAX_LOWMC_ROUNDS 38
#define MAX_LOWMC_SBOXES 10

/**
 * Masks for 10 S-boxes.
 */
#define MASK_X0I UINT64_C(0x2492492400000000)
#define MASK_X1I UINT64_C(0x4924924800000000)
#define MASK_X2I UINT64_C(0x9249249000000000)
#define MASK_MASK UINT64_C(0x00000003ffffffff)

/**
 * LowMC instances
 */
#define LOWMC_L1_N 128
#define LOWMC_L1_M 10
#define LOWMC_L1_K LOWMC_L1_N
#define LOWMC_L1_R 20

#define LOWMC_L3_N 192
#define LOWMC_L3_M 10
#define LOWMC_L3_K LOWMC_L3_N
#define LOWMC_L3_R 30

#define LOWMC_L5_N 256
#define LOWMC_L5_M 10
#define LOWMC_L5_K LOWMC_L5_N
#define LOWMC_L5_R 38

typedef struct {
#if !defined(REDUCED_ROUND_KEY_COMPUTATION)
  const mzd_local_t* k_matrix;
#endif
#if !defined(OPTIMIZED_LINEAR_LAYER_EVALUATION)
  const mzd_local_t* l_matrix;
#else
  const mzd_local_t* z_matrix;
  const mzd_local_t* r_matrix;
  const word r_mask;
#endif
#if !defined(REDUCED_ROUND_KEY_COMPUTATION)
  const mzd_local_t* constant;
#endif
} lowmc_round_t;

/**
 * LowMC definition
 */
typedef struct {
  uint32_t m;
  uint32_t n;
  uint32_t r;
  uint32_t k;

  const mzd_local_t* k0_matrix; // K_0 or K_0 + precomputed if reduced_linear_layer is set
#if defined(OPTIMIZED_LINEAR_LAYER_EVALUATION)
  const mzd_local_t* zr_matrix; // combined linear layers
#endif
  const lowmc_round_t* rounds;

#if defined(REDUCED_ROUND_KEY_COMPUTATION)
  const mzd_local_t* precomputed_non_linear_part_matrix;
  const mzd_local_t* precomputed_constant_linear;
  const mzd_local_t* precomputed_constant_non_linear;
#endif
} lowmc_t;

#endif
