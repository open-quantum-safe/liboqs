/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#if defined(LOWMC_INSTANCE_10)
#define LOWMC_INSTANCE LOWMC_INSTANCE_10
#define LOWMC_M 10
#define LOWMC_R LOWMC_R_10
#define MUL_MC MUL_MC_10
#define ADDMUL_R ADDMUL_R_10
#define MUL_Z MUL_Z_10
#define MZD_SHUFFLE CONCAT(SHUFFLE, 30)
#define M_FIXED_10
#define N_LOWMC CONCAT(LOWMC, 10)
#define SBOX(x) sbox_layer_10_uint64(&BLOCK(x, 0)->w64[(LOWMC_N / (sizeof(word) * 8)) - 1])
#define XOR_MC XOR_MC_10
#include "lowmc_impl.c.i"

#if defined(WITH_ZKBPP)
#undef N_LOWMC
#define N_LOWMC CONCAT(LOWMC, store_10)
#define RECORD_STATE
#include "lowmc_impl.c.i"
#endif

#if defined(WITH_KKW)
#undef N_LOWMC
#undef RECORD_STATE
#undef SBOX
#define SBOX(x, tapes)                                                                             \
  sbox_layer_10_uint64_aux(&BLOCK(x, 0)->w64[(LOWMC_N / (sizeof(word) * 8)) - 1], tapes)
#define N_LOWMC CONCAT(LOWMC, compute_aux_10)
#define PICNIC2_AUX_COMPUTATION
#include "lowmc_impl.c.i"
#endif

#undef LOWMC_INSTANCE
#undef LOWMC_M
#undef LOWMC_R
#undef MUL_MC
#undef ADDMUL_R
#undef MUL_Z
#undef MZD_SHUFFLE
#undef M_FIXED_10
#undef N_LOWMC
#undef RECORD_STATE
#undef PICNIC2_AUX_COMPUTATION
#undef SBOX
#undef XOR_MC
#endif

#if defined(WITH_LOWMC_M1) && defined(LOWMC_INSTANCE_1)
#define LOWMC_INSTANCE LOWMC_INSTANCE_1
#define LOWMC_M 1
#define LOWMC_R LOWMC_R_1
#define MUL_MC MUL_MC_1
#define ADDMUL_R ADDMUL_R_1
#define MUL_Z MUL_Z_1
#define MZD_SHUFFLE CONCAT(SHUFFLE, 3)
#define M_FIXED_1
#define N_LOWMC CONCAT(LOWMC, 1)
#define SBOX(x) sbox_layer_1_uint64(&BLOCK(x, 0)->w64[(LOWMC_N / (sizeof(word) * 8)) - 1])
#define XOR_MC XOR_MC_1
#include "lowmc_impl.c.i"

#if defined(WITH_ZKBPP)
#undef N_LOWMC
#define N_LOWMC CONCAT(LOWMC, store_1)
#define RECORD_STATE
#include "lowmc_impl.c.i"
#endif

#undef LOWMC_INSTANCE
#undef LOWMC_M
#undef LOWMC_R
#undef MUL_MC
#undef ADDMUL_R
#undef MUL_Z
#undef MZD_SHUFFLE
#undef M_FIXED_1
#undef N_LOWMC
#undef RECORD_STATE
#undef PICNIC2_AUX_COMPUTATION
#undef SBOX
#undef XOR_MC
#endif

// vim: ft=c
