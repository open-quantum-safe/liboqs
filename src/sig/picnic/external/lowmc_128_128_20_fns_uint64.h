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
#define SHUFFLE mzd_shuffle_128_30
#define COPY mzd_copy_uint64_128

#define MUL_MC mzd_mul_v_uint64_128_640
#define ADDMUL_R mzd_addmul_v_uint64_30_128
#define MUL_Z mzd_mul_v_parity_uint64_128_30
#define XOR_MC mzd_xor_uint64_640

#if defined(WITH_LOWMC_128_128_20)
#define LOWMC_INSTANCE lowmc_128_128_20
#define LOWMC_PARTIAL
#define LOWMC_N LOWMC_128_128_20_N
#define LOWMC_R LOWMC_128_128_20_R
#define LOWMC_M LOWMC_128_128_20_M
#endif
