/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#include "lowmc_fns_undef.h"

#define ADDMUL mzd_addmul_v_s128_192
#define MUL mzd_mul_v_s128_192
#define SHUFFLE mzd_shuffle_192
#define XOR mzd_xor_s128_256
#define COPY mzd_copy_s128_256

#define MUL_MC_1 mzd_mul_v_s128_192_896
#define MUL_MC_10 mzd_mul_v_s128_192_1024
#define ADDMUL_R_1 mzd_addmul_v_s128_3_192
#define ADDMUL_R_10 mzd_addmul_v_s128_30_192
#define MUL_Z_1 mzd_mul_v_parity_uint64_192_3
#define MUL_Z_10 mzd_mul_v_parity_uint64_192_30
#define XOR_MC_1 mzd_xor_s128_896
#define XOR_MC_10 mzd_xor_s128_1024

#if defined(WITH_LOWMC_192_192_30)
#define LOWMC_INSTANCE_10 lowmc_192_192_30
#endif
#if defined(WITH_LOWMC_192_192_284)
#define LOWMC_INSTANCE_1 lowmc_192_192_284
#endif
#define LOWMC_N LOWMC_L3_N
#define LOWMC_R_10 LOWMC_L3_R
#define LOWMC_R_1 LOWMC_L3_1_R
