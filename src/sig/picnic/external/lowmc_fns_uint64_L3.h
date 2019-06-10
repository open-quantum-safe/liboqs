/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#include "lowmc_fns_undef.h"

#define ADDMUL SELECT_V_VL(mzd_addmul_v_uint64_192, mzd_addmul_vl_uint64_192)
#define MUL SELECT_V_VL(mzd_mul_v_uint64_192, mzd_mul_vl_uint64_192)
#define SHUFFLE mzd_shuffle_192
#define XOR mzd_xor_uint64_192
#define COPY mzd_copy_uint64_192

#define MUL_MC_1 SELECT_V_VL(mzd_mul_v_uint64_192_896, mzd_mul_vl_uint64_192_896)
#define MUL_MC_10 SELECT_V_VL(mzd_mul_v_uint64_192_960, mzd_mul_vl_uint64_192_960)
#define ADDMUL_R_1 mzd_addmul_v_uint64_3_192
#define ADDMUL_R_10 mzd_addmul_v_uint64_30_192
#define MUL_Z_1 mzd_mul_v_parity_uint64_192_3
#define MUL_Z_10 mzd_mul_v_parity_uint64_192_30
#define XOR_MC_1 mzd_xor_uint64_896
#define XOR_MC_10 mzd_xor_uint64_960

#define LOWMC_N LOWMC_L3_N
#define LOWMC_R_10 LOWMC_L3_R
#define LOWMC_R_1 LOWMC_L3_1_R
#if defined(WITH_LOWMC_192_192_30)
#define LOWMC_INSTANCE_10 lowmc_192_192_30
#endif
#if defined(WITH_LOWMC_192_192_284)
#define LOWMC_INSTANCE_1 lowmc_192_192_284
#endif
