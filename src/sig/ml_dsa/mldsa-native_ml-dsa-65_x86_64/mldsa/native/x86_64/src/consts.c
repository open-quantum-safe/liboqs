/*
 * Copyright (c) The mlkem-native project authors
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/*
 * This file is derived from the public domain
 * AVX2 Dilithium implementation @[REF_AVX2].
 */

#include "../../../common.h"
#if defined(MLD_ARITH_BACKEND_X86_64_DEFAULT) && \
    !defined(MLD_CONFIG_MULTILEVEL_NO_SHARED)

#include "align.h"
#include "consts.h"
#define MLD_AVX2_Q MLDSA_Q
/* check-magic: 58728449 == pow(MLDSA_Q,-1,2^32) */
#define MLD_AVX2_QINV 58728449
/* check-magic: -4186625 == pow(2,32,MLDSA_Q) */
#define MLD_AVX2_MONT -4186625
/* check-magic: 41978 == pow(2,64-8,MLDSA_Q) */
#define MLD_AVX2_DIV 41978
/* check-magic: -8395782 == signed_mod(MLD_AVX2_QINV*MLD_AVX2_DIV,2^32) */
#define MLD_AVX2_DIV_QINV -8395782

const qdata_t mld_qdata = {{
#define MLD_AVX2_BACKEND_DATA_OFFSET_8XQ 0
    MLD_AVX2_Q,        MLD_AVX2_Q,        MLD_AVX2_Q,        MLD_AVX2_Q,
    MLD_AVX2_Q,        MLD_AVX2_Q,        MLD_AVX2_Q,        MLD_AVX2_Q,

#define MLD_AVX2_BACKEND_DATA_OFFSET_8XQINV 8
    MLD_AVX2_QINV,     MLD_AVX2_QINV,     MLD_AVX2_QINV,     MLD_AVX2_QINV,
    MLD_AVX2_QINV,     MLD_AVX2_QINV,     MLD_AVX2_QINV,     MLD_AVX2_QINV,

#define MLD_AVX2_BACKEND_DATA_OFFSET_8XDIV_QINV 16
    MLD_AVX2_DIV_QINV, MLD_AVX2_DIV_QINV, MLD_AVX2_DIV_QINV, MLD_AVX2_DIV_QINV,
    MLD_AVX2_DIV_QINV, MLD_AVX2_DIV_QINV, MLD_AVX2_DIV_QINV, MLD_AVX2_DIV_QINV,

#define MLD_AVX2_BACKEND_DATA_OFFSET_8XDIV 24
    MLD_AVX2_DIV,      MLD_AVX2_DIV,      MLD_AVX2_DIV,      MLD_AVX2_DIV,
    MLD_AVX2_DIV,      MLD_AVX2_DIV,      MLD_AVX2_DIV,      MLD_AVX2_DIV,

#define MLD_AVX2_BACKEND_DATA_OFFSET_ZETAS_QINV 32
#define MLD_AVX2_BACKEND_DATA_OFFSET_ZETAS 328
#include "x86_64_zetas.i"

}};

#else /* MLD_ARITH_BACKEND_X86_64_DEFAULT && !MLD_CONFIG_MULTILEVEL_NO_SHARED \
       */

MLD_EMPTY_CU(avx2_consts)

#endif /* !(MLD_ARITH_BACKEND_X86_64_DEFAULT && \
          !MLD_CONFIG_MULTILEVEL_NO_SHARED) */
