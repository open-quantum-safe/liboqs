/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* References
 * ==========
 *
 * - [REF_AVX2]
 *   CRYSTALS-Kyber optimized AVX2 implementation
 *   Bos, Ducas, Kiltz, Lepoint, Lyubashevsky, Schanck, Schwabe, Seiler, Stehlé
 *   https://github.com/pq-crystals/kyber/tree/main/avx2
 */

/*
 * This file is derived from the public domain
 * AVX2 Kyber implementation @[REF_AVX2].
 */

#include "../../../common.h"

#if defined(MLK_ARITH_BACKEND_X86_64_DEFAULT) && \
    !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED)

#include "align.h"
#include "consts.h"

#define MLK_AVX2_Q MLKEM_Q
/* check-magic: -1044 == pow(2,16,MLKEM_Q) */
#define MLK_AVX2_MONT -1044
/* check-magic: -3327 == pow(MLKEM_Q,-1,2^16) */
#define MLK_AVX2_QINV -3327
/* check-magic: 20159 == round(2^26/MLKEM_Q) */
#define MLK_AVX2_V 20159
/* check-magic: 1441 == pow(2,32-7,MLKEM_Q) */
#define MLK_AVX2_FHI 1441
/* check-magic: -10079 == signed_mod(MLK_AVX2_QINV*MLK_AVX2_FHI,2^16) */
#define MLK_AVX2_FLO -10079
/* check-magic: 1353 == pow(2, 32, MLKEM_Q) */
#define MLK_AVX2_MONTSQHI 1353
/* check-magic: 20553 == signed_mod(MLK_AVX2_QINV*MLK_AVX2_MONTSQHI,2^16) */
#define MLK_AVX2_MONTSQLO 20553
#define MLK_AVX2_MASK 4095
#define MLK_AVX2_SHIFT 32

const qdata_t mlk_qdata = {{
#define MLK_AVX2_BACKEND_DATA_OFFSET_16XQ 0
    MLK_AVX2_Q,
    MLK_AVX2_Q,
    MLK_AVX2_Q,
    MLK_AVX2_Q,
    MLK_AVX2_Q,
    MLK_AVX2_Q,
    MLK_AVX2_Q,
    MLK_AVX2_Q,
    MLK_AVX2_Q,
    MLK_AVX2_Q,
    MLK_AVX2_Q,
    MLK_AVX2_Q,
    MLK_AVX2_Q,
    MLK_AVX2_Q,
    MLK_AVX2_Q,
    MLK_AVX2_Q,

#define MLK_AVX2_BACKEND_DATA_OFFSET_16XQINV 16
    MLK_AVX2_QINV,
    MLK_AVX2_QINV,
    MLK_AVX2_QINV,
    MLK_AVX2_QINV,
    MLK_AVX2_QINV,
    MLK_AVX2_QINV,
    MLK_AVX2_QINV,
    MLK_AVX2_QINV,
    MLK_AVX2_QINV,
    MLK_AVX2_QINV,
    MLK_AVX2_QINV,
    MLK_AVX2_QINV,
    MLK_AVX2_QINV,
    MLK_AVX2_QINV,
    MLK_AVX2_QINV,
    MLK_AVX2_QINV,

#define MLK_AVX2_BACKEND_DATA_OFFSET_16XV 32
    MLK_AVX2_V,
    MLK_AVX2_V,
    MLK_AVX2_V,
    MLK_AVX2_V,
    MLK_AVX2_V,
    MLK_AVX2_V,
    MLK_AVX2_V,
    MLK_AVX2_V,
    MLK_AVX2_V,
    MLK_AVX2_V,
    MLK_AVX2_V,
    MLK_AVX2_V,
    MLK_AVX2_V,
    MLK_AVX2_V,
    MLK_AVX2_V,
    MLK_AVX2_V,

#define MLK_AVX2_BACKEND_DATA_OFFSET_16XFLO 48
    MLK_AVX2_FLO,
    MLK_AVX2_FLO,
    MLK_AVX2_FLO,
    MLK_AVX2_FLO,
    MLK_AVX2_FLO,
    MLK_AVX2_FLO,
    MLK_AVX2_FLO,
    MLK_AVX2_FLO,
    MLK_AVX2_FLO,
    MLK_AVX2_FLO,
    MLK_AVX2_FLO,
    MLK_AVX2_FLO,
    MLK_AVX2_FLO,
    MLK_AVX2_FLO,
    MLK_AVX2_FLO,
    MLK_AVX2_FLO,

#define MLK_AVX2_BACKEND_DATA_OFFSET_16XFHI 64
    MLK_AVX2_FHI,
    MLK_AVX2_FHI,
    MLK_AVX2_FHI,
    MLK_AVX2_FHI,
    MLK_AVX2_FHI,
    MLK_AVX2_FHI,
    MLK_AVX2_FHI,
    MLK_AVX2_FHI,
    MLK_AVX2_FHI,
    MLK_AVX2_FHI,
    MLK_AVX2_FHI,
    MLK_AVX2_FHI,
    MLK_AVX2_FHI,
    MLK_AVX2_FHI,
    MLK_AVX2_FHI,
    MLK_AVX2_FHI,

#define MLK_AVX2_BACKEND_DATA_OFFSET_16XMONTSQLO 80
    MLK_AVX2_MONTSQLO,
    MLK_AVX2_MONTSQLO,
    MLK_AVX2_MONTSQLO,
    MLK_AVX2_MONTSQLO,
    MLK_AVX2_MONTSQLO,
    MLK_AVX2_MONTSQLO,
    MLK_AVX2_MONTSQLO,
    MLK_AVX2_MONTSQLO,
    MLK_AVX2_MONTSQLO,
    MLK_AVX2_MONTSQLO,
    MLK_AVX2_MONTSQLO,
    MLK_AVX2_MONTSQLO,
    MLK_AVX2_MONTSQLO,
    MLK_AVX2_MONTSQLO,
    MLK_AVX2_MONTSQLO,
    MLK_AVX2_MONTSQLO,

#define MLK_AVX2_BACKEND_DATA_OFFSET_16XMONTSQHI 96
    MLK_AVX2_MONTSQHI,
    MLK_AVX2_MONTSQHI,
    MLK_AVX2_MONTSQHI,
    MLK_AVX2_MONTSQHI,
    MLK_AVX2_MONTSQHI,
    MLK_AVX2_MONTSQHI,
    MLK_AVX2_MONTSQHI,
    MLK_AVX2_MONTSQHI,
    MLK_AVX2_MONTSQHI,
    MLK_AVX2_MONTSQHI,
    MLK_AVX2_MONTSQHI,
    MLK_AVX2_MONTSQHI,
    MLK_AVX2_MONTSQHI,
    MLK_AVX2_MONTSQHI,
    MLK_AVX2_MONTSQHI,
    MLK_AVX2_MONTSQHI,

#define MLK_AVX2_BACKEND_DATA_OFFSET_16XMASK 112
    MLK_AVX2_MASK,
    MLK_AVX2_MASK,
    MLK_AVX2_MASK,
    MLK_AVX2_MASK,
    MLK_AVX2_MASK,
    MLK_AVX2_MASK,
    MLK_AVX2_MASK,
    MLK_AVX2_MASK,
    MLK_AVX2_MASK,
    MLK_AVX2_MASK,
    MLK_AVX2_MASK,
    MLK_AVX2_MASK,
    MLK_AVX2_MASK,
    MLK_AVX2_MASK,
    MLK_AVX2_MASK,
    MLK_AVX2_MASK,

#define MLK_AVX2_BACKEND_DATA_OFFSET_REVIDXB 128
    /* TODO: Explain these numbers */
    /* check-magic: off */
    3854,
    3340,
    2826,
    2312,
    1798,
    1284,
    770,
    256,
    3854,
    3340,
    2826,
    2312,
    1798,
    1284,
    770,
    256,
/* check-magic: on */

#define MLK_AVX2_BACKEND_DATA_OFFSET_REVIDXD 144
    7,
    0,
    6,
    0,
    5,
    0,
    4,
    0,
    3,
    0,
    2,
    0,
    1,
    0,
    0,
    0,

#define MLK_AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP 160
#include "x86_64_zetas.i"

#define MLK_AVX2_BACKEND_DATA_OFFSET_16XSHIFT 624
    MLK_AVX2_SHIFT,
    MLK_AVX2_SHIFT,
    MLK_AVX2_SHIFT,
    MLK_AVX2_SHIFT,
    MLK_AVX2_SHIFT,
    MLK_AVX2_SHIFT,
    MLK_AVX2_SHIFT,
    MLK_AVX2_SHIFT,
    MLK_AVX2_SHIFT,
    MLK_AVX2_SHIFT,
    MLK_AVX2_SHIFT,
    MLK_AVX2_SHIFT,
    MLK_AVX2_SHIFT,
    MLK_AVX2_SHIFT,
    MLK_AVX2_SHIFT,
    MLK_AVX2_SHIFT,
#define MLK_AVX2_BACKEND_DATA_OFFSET_MULCACHE_TWIDDLES 640
#include "x86_64_mulcache_twiddles.i"
}};

#else /* MLK_ARITH_BACKEND_X86_64_DEFAULT && !MLK_CONFIG_MULTILEVEL_NO_SHARED \
       */

MLK_EMPTY_CU(avx2_consts)

#endif /* !(MLK_ARITH_BACKEND_X86_64_DEFAULT && \
          !MLK_CONFIG_MULTILEVEL_NO_SHARED) */

/* To facilitate single-compilation-unit (SCU) builds, undefine all macros.
 * Don't modify by hand -- this is auto-generated by scripts/autogen. */
#undef MLK_AVX2_Q
#undef MLK_AVX2_MONT
#undef MLK_AVX2_QINV
#undef MLK_AVX2_V
#undef MLK_AVX2_FHI
#undef MLK_AVX2_FLO
#undef MLK_AVX2_MONTSQHI
#undef MLK_AVX2_MONTSQLO
#undef MLK_AVX2_MASK
#undef MLK_AVX2_SHIFT
/* Some macros are kept because they are also defined in a header. */
/* Keep: MLK_AVX2_BACKEND_DATA_OFFSET_16XQ (consts.h) */
/* Keep: MLK_AVX2_BACKEND_DATA_OFFSET_16XQINV (consts.h) */
/* Keep: MLK_AVX2_BACKEND_DATA_OFFSET_16XV (consts.h) */
/* Keep: MLK_AVX2_BACKEND_DATA_OFFSET_16XFLO (consts.h) */
/* Keep: MLK_AVX2_BACKEND_DATA_OFFSET_16XFHI (consts.h) */
/* Keep: MLK_AVX2_BACKEND_DATA_OFFSET_16XMONTSQLO (consts.h) */
/* Keep: MLK_AVX2_BACKEND_DATA_OFFSET_16XMONTSQHI (consts.h) */
/* Keep: MLK_AVX2_BACKEND_DATA_OFFSET_16XMASK (consts.h) */
/* Keep: MLK_AVX2_BACKEND_DATA_OFFSET_REVIDXB (consts.h) */
/* Keep: MLK_AVX2_BACKEND_DATA_OFFSET_REVIDXD (consts.h) */
/* Keep: MLK_AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP (consts.h) */
/* Keep: MLK_AVX2_BACKEND_DATA_OFFSET_16XSHIFT (consts.h) */
/* Keep: MLK_AVX2_BACKEND_DATA_OFFSET_MULCACHE_TWIDDLES (consts.h) */
