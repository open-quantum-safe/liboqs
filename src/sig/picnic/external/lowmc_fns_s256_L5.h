/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#include "lowmc_fns_undef.h"

#define ADDMUL SELECT_V_VL(mzd_addmul_v_s256_256, mzd_addmul_vl_s256_256)
#define MUL SELECT_V_VL(mzd_mul_v_s256_256, mzd_mul_vl_s256_256)
#define SHUFFLE mzd_shuffle_pext_256
#define XOR mzd_xor_s256_256
#define COPY mzd_copy_s256_256

#define MUL_MC_1 SELECT_V_VL(mzd_mul_v_s256_256_1280, mzd_mul_vl_s256_256_1280)
#define MUL_MC_10 SELECT_V_VL(mzd_mul_v_s256_256_1280, mzd_mul_vl_s256_256_1280)
#define ADDMUL_R_1 mzd_addmul_v_s256_3_256
#define ADDMUL_R_10 mzd_addmul_v_s256_30_256
#define MUL_Z_1 mzd_mul_v_parity_uint64_256_3
#define MUL_Z_10 mzd_mul_v_parity_uint64_256_30
#define XOR_MC_1 mzd_xor_s256_1280
#define XOR_MC_10 mzd_xor_s256_1280

#if defined(WITH_LOWMC_256_256_38)
#define LOWMC_INSTANCE_10 lowmc_256_256_38
#endif
#if defined(WITH_LOWMC_256_256_363)
#define LOWMC_INSTANCE_1 lowmc_256_256_363
#endif
#define LOWMC_N LOWMC_L5_N
#define LOWMC_R_10 LOWMC_L5_R
#define LOWMC_R_1 LOWMC_L5_1_R
