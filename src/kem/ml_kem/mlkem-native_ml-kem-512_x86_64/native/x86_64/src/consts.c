/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Implementation from Kyber reference repository
 * https://github.com/pq-crystals/kyber/blob/main/avx2/consts.c
 */

#include "../../../common.h"

#if defined(MLKEM_NATIVE_ARITH_BACKEND_X86_64_DEFAULT)

#include "align.h"
#include "consts.h"

#define Q MLKEM_Q
#define MONT -1044     /* 2^16 mod q */
#define QINV -3327     /* q^-1 mod 2^16 */
#define V 20159        /* floor(2^26/q + 0.5) */
#define FHI 1441       /* mont^2/128 */
#define FLO -10079     /* qinv*FHI */
#define MONTSQHI 1353  /* mont^2 */
#define MONTSQLO 20553 /* qinv*MONTSQHI */
#define MASK 4095
#define SHIFT 32

const qdata_t qdata = {{
#define AVX2_BACKEND_DATA_OFFSET_16XQ 0
    Q,        Q,        Q,        Q,        Q,        Q,
    Q,        Q,        Q,        Q,        Q,        Q,
    Q,        Q,        Q,        Q,

#define AVX2_BACKEND_DATA_OFFSET_16XQINV 16
    QINV,     QINV,     QINV,     QINV,     QINV,     QINV,
    QINV,     QINV,     QINV,     QINV,     QINV,     QINV,
    QINV,     QINV,     QINV,     QINV,

#define AVX2_BACKEND_DATA_OFFSET_16XV 32
    V,        V,        V,        V,        V,        V,
    V,        V,        V,        V,        V,        V,
    V,        V,        V,        V,

#define AVX2_BACKEND_DATA_OFFSET_16XFLO 48
    FLO,      FLO,      FLO,      FLO,      FLO,      FLO,
    FLO,      FLO,      FLO,      FLO,      FLO,      FLO,
    FLO,      FLO,      FLO,      FLO,

#define AVX2_BACKEND_DATA_OFFSET_16XFHI 64
    FHI,      FHI,      FHI,      FHI,      FHI,      FHI,
    FHI,      FHI,      FHI,      FHI,      FHI,      FHI,
    FHI,      FHI,      FHI,      FHI,

#define AVX2_BACKEND_DATA_OFFSET_16XMONTSQLO 80
    MONTSQLO, MONTSQLO, MONTSQLO, MONTSQLO, MONTSQLO, MONTSQLO,
    MONTSQLO, MONTSQLO, MONTSQLO, MONTSQLO, MONTSQLO, MONTSQLO,
    MONTSQLO, MONTSQLO, MONTSQLO, MONTSQLO,

#define AVX2_BACKEND_DATA_OFFSET_16XMONTSQHI 96
    MONTSQHI, MONTSQHI, MONTSQHI, MONTSQHI, MONTSQHI, MONTSQHI,
    MONTSQHI, MONTSQHI, MONTSQHI, MONTSQHI, MONTSQHI, MONTSQHI,
    MONTSQHI, MONTSQHI, MONTSQHI, MONTSQHI,

#define AVX2_BACKEND_DATA_OFFSET_16XMASK 112
    MASK,     MASK,     MASK,     MASK,     MASK,     MASK,
    MASK,     MASK,     MASK,     MASK,     MASK,     MASK,
    MASK,     MASK,     MASK,     MASK,

#define AVX2_BACKEND_DATA_OFFSET_REVIDXB 128
    3854,     3340,     2826,     2312,     1798,     1284,
    770,      256,      3854,     3340,     2826,     2312,
    1798,     1284,     770,      256,

#define AVX2_BACKEND_DATA_OFFSET_REVIDXD 144
    7,        0,        6,        0,        5,        0,
    4,        0,        3,        0,        2,        0,
    1,        0,        0,        0,

#define AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP 160
#include "x86_64_zetas.i"

#define AVX2_BACKEND_DATA_OFFSET_16XSHIFT 624
    SHIFT,    SHIFT,    SHIFT,    SHIFT,    SHIFT,    SHIFT,
    SHIFT,    SHIFT,    SHIFT,    SHIFT,    SHIFT,    SHIFT,
    SHIFT,    SHIFT,    SHIFT,    SHIFT}};

#else /* MLKEM_NATIVE_ARITH_BACKEND_X86_64_DEFAULT */

MLKEM_NATIVE_EMPTY_CU(avx2_consts)

#endif /* MLKEM_NATIVE_ARITH_BACKEND_X86_64_DEFAULT */
