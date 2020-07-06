/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#if defined(LOWMC_INSTANCE)
#define N_SIGN CONCAT(mpc_lowmc_prove, CONCAT(IMPL, LOWMC_INSTANCE))
#define N_VERIFY CONCAT(mpc_lowmc_verify, CONCAT(IMPL, LOWMC_INSTANCE))
#include "mpc_lowmc_impl.c.i"
#endif

#undef N_SIGN
#undef N_VERIFY

// vim: ft=c
