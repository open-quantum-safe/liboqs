/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Implementation from Kyber reference repository
 * https://github.com/pq-crystals/kyber/blob/main/avx2/consts.h
 */

#ifndef CONSTS_H
#define CONSTS_H

#include "../../../common.h"

#define AVX2_BACKEND_DATA_OFFSET_16XQ 0
#define AVX2_BACKEND_DATA_OFFSET_16XQINV 16
#define AVX2_BACKEND_DATA_OFFSET_16XV 32
#define AVX2_BACKEND_DATA_OFFSET_16XFLO 48
#define AVX2_BACKEND_DATA_OFFSET_16XFHI 64
#define AVX2_BACKEND_DATA_OFFSET_16XMONTSQLO 80
#define AVX2_BACKEND_DATA_OFFSET_16XMONTSQHI 96
#define AVX2_BACKEND_DATA_OFFSET_16XMASK 112
#define AVX2_BACKEND_DATA_OFFSET_REVIDXB 128
#define AVX2_BACKEND_DATA_OFFSET_REVIDXD 144
#define AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP 160
#define AVX2_BACKEND_DATA_OFFSET_16XSHIFT 624

/* The C ABI on MacOS exports all symbols with a leading
 * underscore. This means that any symbols we refer to from
 * C files (functions) can't be found, and all symbols we
 * refer to from ASM also can't be found.
 *
 * This define helps us get around this
 */

#ifndef __ASSEMBLER__
#include "align.h"
typedef ALIGNED_INT16(640) qdata_t;
#define qdata MLKEM_NAMESPACE(qdata)
extern const qdata_t qdata;
#endif

#endif
