/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#include "lowmc_fns_undef.h"

#define ADDMUL mzd_addmul_v_uint64_192
#define MUL mzd_mul_v_uint64_192
#define XOR mzd_xor_uint64_192
#define COPY mzd_copy_uint64_192
#define MPC_MUL mpc_matrix_mul_uint64_192

#if defined(WITH_LOWMC_192_192_4)
#define LOWMC_INSTANCE lowmc_192_192_4
#define LOWMC_N LOWMC_192_192_4_N
#define LOWMC_R LOWMC_192_192_4_R
#define LOWMC_M LOWMC_192_192_4_M
#endif
