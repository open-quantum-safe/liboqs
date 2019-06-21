/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#include "lowmc_fns_undef.h"

#define ADDMUL SELECT_V_VL(mzd_addmul_v_s128_128, mzd_addmul_vl_s128_128)
#define MUL SELECT_V_VL(mzd_mul_v_s128_128, mzd_mul_vl_s128_128)
#define SHUFFLE mzd_shuffle_128
#define XOR mzd_xor_s128_128
#define COPY mzd_copy_s128_128

#define MUL_MC_1 SELECT_V_VL(mzd_mul_v_s128_128_640, mzd_mul_vl_s128_128_640)
#define MUL_MC_10 SELECT_V_VL(mzd_mul_v_s128_128_640, mzd_mul_vl_s128_128_640)
#define ADDMUL_R_1 mzd_addmul_v_s128_3_128
#define ADDMUL_R_10 mzd_addmul_v_s128_30_128
#define MUL_Z_1 mzd_mul_v_parity_uint64_128_3
#define MUL_Z_10 mzd_mul_v_parity_uint64_128_30
#define XOR_MC_1 mzd_xor_s128_640
#define XOR_MC_10 mzd_xor_s128_640

#if defined(WITH_LOWMC_128_128_20)
#define LOWMC_INSTANCE_10 lowmc_128_128_20
#endif
#if defined(WITH_LOWMC_128_128_182)
#define LOWMC_INSTANCE_1 lowmc_128_128_182
#endif
#define LOWMC_N LOWMC_L1_N
#define LOWMC_R_10 LOWMC_L1_R
#define LOWMC_R_1 LOWMC_L1_1_R
