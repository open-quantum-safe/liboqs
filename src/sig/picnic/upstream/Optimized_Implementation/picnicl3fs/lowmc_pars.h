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

typedef struct {
  mzd_local_t* x0;
  mzd_local_t* x1;
  mzd_local_t* x2;
  mzd_local_t* mask;
} mask_t;

/**
 * Masks for 10 S-boxes.
 */
#define MASK_X0I  UINT64_C(0x2492492400000000)
#define MASK_X1I  UINT64_C(0x4924924800000000)
#define MASK_X2I  UINT64_C(0x9249249000000000)
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
  const mzd_local_t* l_matrix;
  const mzd_local_t* constant;

  mzd_local_t* l_lookup;
} lowmc_round_t;

/**
 * LowMC definition
 */
typedef struct {
  unsigned int m;
  unsigned int n;
  unsigned int r;
  unsigned int k;


  const mzd_local_t* k0_matrix; // K_0 or K_0 + precomputed if reduced_linear_layer is set
  mzd_local_t* k0_lookup;
  lowmc_round_t* rounds;

  const mzd_local_t* precomputed_non_linear_part_matrix;
  mzd_local_t* precomputed_non_linear_part_lookup;

  bool needs_free;
} lowmc_t;

/**
 * Generates a new LowMC instance
 *
 * \param m the number of sboxes
 * \param n the blocksize
 * \param r the number of rounds
 * \param k the keysize
 *
 * \return parameters defining a LowMC instance
 */
bool lowmc_init(lowmc_t* lowmc, unsigned int m, unsigned int n, unsigned int r, unsigned int k);

/**
 * Clears the allocated LowMC parameters
 *
 * \param lowmc the LowMC parameters to be cleared
 */
void lowmc_clear(lowmc_t* lowmc);

bool lowmc_read_file(lowmc_t* lowmc, unsigned int m, unsigned int n, unsigned int r,
                     unsigned int k);

#endif
