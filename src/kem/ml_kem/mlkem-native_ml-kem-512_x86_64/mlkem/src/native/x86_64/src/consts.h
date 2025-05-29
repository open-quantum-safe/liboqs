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

#ifndef MLK_NATIVE_X86_64_SRC_CONSTS_H
#define MLK_NATIVE_X86_64_SRC_CONSTS_H
#include "../../../common.h"

#define MLK_AVX2_BACKEND_DATA_OFFSET_16XQ 0
#define MLK_AVX2_BACKEND_DATA_OFFSET_16XQINV 16
#define MLK_AVX2_BACKEND_DATA_OFFSET_16XV 32
#define MLK_AVX2_BACKEND_DATA_OFFSET_16XFLO 48
#define MLK_AVX2_BACKEND_DATA_OFFSET_16XFHI 64
#define MLK_AVX2_BACKEND_DATA_OFFSET_16XMONTSQLO 80
#define MLK_AVX2_BACKEND_DATA_OFFSET_16XMONTSQHI 96
#define MLK_AVX2_BACKEND_DATA_OFFSET_16XMASK 112
#define MLK_AVX2_BACKEND_DATA_OFFSET_REVIDXB 128
#define MLK_AVX2_BACKEND_DATA_OFFSET_REVIDXD 144
#define MLK_AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP 160
#define MLK_AVX2_BACKEND_DATA_OFFSET_16XSHIFT 624
#define MLK_AVX2_BACKEND_DATA_OFFSET_MULCACHE_TWIDDLES 640

#ifndef __ASSEMBLER__
#include "align.h"
typedef MLK_ALIGNED_INT16(768) qdata_t;
#define mlk_qdata MLK_NAMESPACE(qdata)
extern const qdata_t mlk_qdata;
#endif /* !__ASSEMBLER__ */

#endif /* !MLK_NATIVE_X86_64_SRC_CONSTS_H */
