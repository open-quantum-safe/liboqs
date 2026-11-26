/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/*
 * WARNING: This file is auto-generated from scripts/autogen
 *          in the mlkem-native repository.
 *          Do not modify it directly.
 */

#include "../../../common.h"

#if defined(MLK_ARITH_BACKEND_AARCH64) &&                          \
    !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED) &&                   \
    !defined(MLK_CONFIG_NO_DECAPS_API) &&                          \
    (defined(MLK_CONFIG_MULTILEVEL_WITH_SHARED) || MLKEM_K == 2 || \
     MLKEM_K == 3)

#include "arith_native_aarch64.h"

/*
 * Constant vectors used by poly_decompress_d10_aarch64_asm:
 * TBL indices for groups starting at bytes 0 and 6 (rows 0, 1),
 * USHL shift amounts (row 2), AND mask (row 3) and SQRDMULH
 * multiplier (row 4). See autogen for details.
 */
MLK_ALIGN MLK_INTERNAL_DATA_DEFINITION const uint8_t
    mlk_aarch64_decompress_d10_table[80] = {
        0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03, 0x04, 0x05,
        0x06, 0x06, 0x07, 0x07, 0x08, 0x08, 0x09, /* idx0 */
        0x06, 0x07, 0x07, 0x08, 0x08, 0x09, 0x09, 0x0a, 0x0b,
        0x0c, 0x0c, 0x0d, 0x0d, 0x0e, 0x0e, 0x0f, /* idx6 */
        0x02, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xfc, 0xff, 0x02,
        0x00, 0x00, 0x00, 0xfe, 0xff, 0xfc, 0xff, /* shifts (16-bit lanes) */
        0xfc, 0x0f, 0xfc, 0x0f, 0xfc, 0x0f, 0xfc, 0x0f, 0xfc,
        0x0f, 0xfc, 0x0f, 0xfc, 0x0f, 0xfc, 0x0f, /* mask (16-bit lanes) */
        0x08, 0x68, 0x08, 0x68, 0x08, 0x68, 0x08, 0x68, 0x08,
        0x68, 0x08, 0x68, 0x08, 0x68, 0x08, 0x68, /* mult (16-bit lanes) */
};

#else /* MLK_ARITH_BACKEND_AARCH64 && !MLK_CONFIG_MULTILEVEL_NO_SHARED &&   \
         !MLK_CONFIG_NO_DECAPS_API && (MLK_CONFIG_MULTILEVEL_WITH_SHARED || \
         MLKEM_K == 2 || MLKEM_K == 3) */

MLK_EMPTY_CU(aarch64_decompress_d10_table)

#endif /* !(MLK_ARITH_BACKEND_AARCH64 && !MLK_CONFIG_MULTILEVEL_NO_SHARED && \
          !MLK_CONFIG_NO_DECAPS_API && (MLK_CONFIG_MULTILEVEL_WITH_SHARED || \
          MLKEM_K == 2 || MLKEM_K == 3)) */
