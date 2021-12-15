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

/**
 * Masks for 10 S-boxes.
 */
#define MASK_X0I UINT64_C(0x2492492400000000)
#define MASK_X1I UINT64_C(0x4924924800000000)
#define MASK_X2I UINT64_C(0x9249249000000000)
#define MASK_MASK UINT64_C(0x00000003ffffffff)

/**
 * LowMC parameters
 */
typedef struct {
  uint16_t n; // == k
  uint8_t m;
  uint8_t r;
} lowmc_parameters_t;

/**
 * LowMC round with full Sblox layer
 */
typedef struct {
  const mzd_local_t* k_matrix;
  const mzd_local_t* l_matrix;
  const mzd_local_t* li_matrix;
  const mzd_local_t* constant;
} lowmc_round_t;

/**
 * LowMC definition with full Sbox layer
 */
typedef struct {
  const mzd_local_t* k0_matrix;  // K_0
  const mzd_local_t* ki0_matrix; // inverse of K_0
  const lowmc_round_t* rounds;
} lowmc_t;

/**
 * LowMC round with partial Sblox layer
 */
typedef struct {
  const mzd_local_t* z_matrix;
  const mzd_local_t* r_matrix;
  const word r_mask;
} lowmc_partial_round_t;

/**
 * LowMC definition with partial Sbox layer
 */
typedef struct {
  const mzd_local_t* k0_matrix; // K_0 + precomputed
  const mzd_local_t* zr_matrix; // combined linear layers
  const lowmc_partial_round_t* rounds;

  const mzd_local_t* precomputed_non_linear_part_matrix;
  const mzd_local_t* precomputed_constant_linear;
  const mzd_local_t* precomputed_constant_non_linear;
} lowmc_partial_t;

#define LOWMC_ID(n, m) ((((uint32_t)n) << 16) | (uint32_t)m)
#define LOWMC_GET_ID(lowmc) LOWMC_ID(lowmc->n, lowmc->m)

#endif
