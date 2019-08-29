/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#include "lowmc_fns_undef.h"

#define ADDMUL mzd_addmul_v_uint64_256
#define MUL mzd_mul_v_uint64_256
#define SHUFFLE mzd_shuffle_256
#define XOR mzd_xor_uint64_256
#define COPY mzd_copy_uint64_256

#define MUL_MC_1 mzd_mul_v_uint64_256_1152
#define MUL_MC_10 mzd_mul_v_uint64_256_1216
#define ADDMUL_R_1 mzd_addmul_v_uint64_3_256
#define ADDMUL_R_10 mzd_addmul_v_uint64_30_256
#define MUL_Z_1 mzd_mul_v_parity_uint64_256_3
#define MUL_Z_10 mzd_mul_v_parity_uint64_256_30
#define XOR_MC_1 mzd_xor_uint64_1152
#define XOR_MC_10 mzd_xor_uint64_1216

#define LOWMC_N LOWMC_L5_N
#define LOWMC_R_10 LOWMC_L5_R
#define LOWMC_R_1 LOWMC_L5_1_R
#if defined(WITH_LOWMC_256_256_38)
#define LOWMC_INSTANCE_10 lowmc_256_256_38
#endif
#if defined(WITH_LOWMC_256_256_363)
#define LOWMC_INSTANCE_1 lowmc_256_256_363
#endif
