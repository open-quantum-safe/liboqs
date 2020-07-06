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
#define SHUFFLE mzd_shuffle_256_30
#define XOR mzd_xor_uint64_256
#define COPY mzd_copy_uint64_256

#define MUL_MC mzd_mul_v_uint64_256_1216
#define ADDMUL_R mzd_addmul_v_uint64_30_256
#define MUL_Z mzd_mul_v_parity_uint64_256_30
#define XOR_MC mzd_xor_uint64_1216

#if defined(WITH_LOWMC_256_256_38)
#define LOWMC_INSTANCE lowmc_256_256_38
#define LOWMC_PARTIAL
#define LOWMC_N LOWMC_256_256_38_N
#define LOWMC_R LOWMC_256_256_38_R
#define LOWMC_M LOWMC_256_256_38_M
#endif
