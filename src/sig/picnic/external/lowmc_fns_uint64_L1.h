/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#include "lowmc_fns_undef.h"

#define ADDMUL mzd_addmul_v_uint64_128
#define MUL mzd_mul_v_uint64_128
#define XOR mzd_xor_uint64_128
#define SHUFFLE mzd_shuffle_128
#define COPY mzd_copy_uint64_128

#define MUL_MC_1 mzd_mul_v_uint64_128_576
#define MUL_MC_10 mzd_mul_v_uint64_128_640
#define ADDMUL_R_1 mzd_addmul_v_uint64_3_128
#define ADDMUL_R_10 mzd_addmul_v_uint64_30_128
#define MUL_Z_1 mzd_mul_v_parity_uint64_128_3
#define MUL_Z_10 mzd_mul_v_parity_uint64_128_30
#define XOR_MC_1 mzd_xor_uint64_576
#define XOR_MC_10 mzd_xor_uint64_640

#define LOWMC_N LOWMC_L1_N
#define LOWMC_R_10 LOWMC_L1_R
#define LOWMC_R_1 LOWMC_L1_1_R
#if defined(WITH_LOWMC_128_128_20)
#define LOWMC_INSTANCE_10 lowmc_128_128_20
#endif
#if defined(WITH_LOWMC_128_128_182)
#define LOWMC_INSTANCE_1 lowmc_128_128_182
#endif
