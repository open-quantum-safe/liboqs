/*
 * Copyright (c) The mlkem-native project authors
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* References
 * ==========
 *
 * - [REF_AVX2]
 *   CRYSTALS-Dilithium optimized AVX2 implementation
 *   Bai, Ducas, Kiltz, Lepoint, Lyubashevsky, Schwabe, Seiler, Stehlé
 *   https://github.com/pq-crystals/dilithium/tree/master/avx2
 */

#ifndef MLD_NATIVE_X86_64_SRC_CONSTS_H
#define MLD_NATIVE_X86_64_SRC_CONSTS_H
/*
 * This file is derived from the public domain
 * AVX2 Dilithium implementation @[REF_AVX2].
 */

#include "../../../common.h"
#define MLD_AVX2_BACKEND_DATA_OFFSET_8XQ 0
#define MLD_AVX2_BACKEND_DATA_OFFSET_8XQINV 8
#define MLD_AVX2_BACKEND_DATA_OFFSET_8XDIV_QINV 16
#define MLD_AVX2_BACKEND_DATA_OFFSET_8XDIV 24
#define MLD_AVX2_BACKEND_DATA_OFFSET_ZETAS_QINV 32
#define MLD_AVX2_BACKEND_DATA_OFFSET_ZETAS 328


#ifndef __ASSEMBLER__
#include "align.h"
typedef MLD_ALIGNED_INT32(624) qdata_t;
#define mld_qdata MLD_NAMESPACE(qdata)
extern const qdata_t mld_qdata;
#endif /* !__ASSEMBLER__ */

#endif /* !MLD_NATIVE_X86_64_SRC_CONSTS_H */
