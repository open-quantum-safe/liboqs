/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#include "../../../common.h"

#if defined(MLK_ARITH_BACKEND_PPC64LE_DEFAULT) && \
    !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED) && defined(__POWER8_VECTOR__)

#include "consts.h"

/* 7 groups of 8 base constants + 4 twiddle tables * 63 rows * 8 values */
/* check-magic: 2072 == 7 * 8 + 4 * 63 * 8 */
MLK_ALIGN MLK_INTERNAL_DATA_DEFINITION const int16_t mlk_ppc_qdata[2072] = {
    /* -Q */
    /* check-magic: -3329 == -1 * MLKEM_Q */
    -3329,
    -3329,
    -3329,
    -3329,
    -3329,
    -3329,
    -3329,
    -3329,
    /* QINV */
    /* check-magic: -3327 == pow(MLKEM_Q,-1,2^16) */
    -3327,
    -3327,
    -3327,
    -3327,
    -3327,
    -3327,
    -3327,
    -3327,
    /* Q */
    3329,
    3329,
    3329,
    3329,
    3329,
    3329,
    3329,
    3329,
    /* check-magic: 20159 == round(2^26 / MLKEM_Q) */
    20159,
    20159,
    20159,
    20159,
    20159,
    20159,
    20159,
    20159,
    /* N^-1 in Montgomery form: pow(128,-1,MLKEM_Q) * 2^16 mod MLKEM_Q = 512.
     * Multiplying by this via Barrett-fqmul scales INTT output by N^-1 and
     * leaves it in Montgomery form (mlk_poly_invntt_tomont contract). */
    512,
    512,
    512,
    512,
    512,
    512,
    512,
    512,
    /* check-magic: 5040 == round((512 * 2**16 + MLKEM_Q) / MLKEM_Q) // 2 */
    /* Barrett twist of N^-1*R = round_to_even(N_INV_MONT * 2^16 / MLKEM_Q) / 2
     */
    5040,
    5040,
    5040,
    5040,
    5040,
    5040,
    5040,
    5040,
    /* check-magic: 1353 == pow(2, 32, MLKEM_Q) */
    1353,
    1353,
    1353,
    1353,
    1353,
    1353,
    1353,
    1353,
/* zetas for NTT */
#include "consts_ntt.inc"
/* zetas for invNTT */
#include "consts_intt.inc"
/* twisted zetas for NTT (Barrett high-mul) */
#include "consts_ntt_tw.inc"
/* twisted zetas for invNTT (Barrett high-mul) */
#include "consts_intt_tw.inc"
};
#endif /* MLK_ARITH_BACKEND_PPC64LE_DEFAULT && \
          !MLK_CONFIG_MULTILEVEL_NO_SHARED && __POWER8_VECTOR__ */
