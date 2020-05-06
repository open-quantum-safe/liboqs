/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#if defined(LOWMC_INSTANCE)
#define LOWMC_M 10
#define N_LOWMC CONCAT(LOWMC, 10)
#define SBOX(x) sbox_layer_10_uint64(&BLOCK(x, 0)->w64[(LOWMC_N / (sizeof(word) * 8)) - 1])
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

#undef LOWMC_M
#undef N_LOWMC
#undef RECORD_STATE
#undef PICNIC2_AUX_COMPUTATION
#undef SBOX
#endif

// vim: ft=c
