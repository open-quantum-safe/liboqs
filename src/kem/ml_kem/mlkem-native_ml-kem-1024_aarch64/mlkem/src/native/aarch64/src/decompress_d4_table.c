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
 * Lookup tables for Decompress_4, used by
 * poly_decompress_d4_aarch64_asm: row 0 holds the low bytes and
 * row 1 the high bytes of Decompress_4(x) for x = 0, ..., 15.
 * See autogen for details.
 */
MLK_ALIGN MLK_INTERNAL_DATA_DEFINITION const uint8_t
    mlk_aarch64_decompress_d4_table[32] = {
        0x00, 0xd0, 0xa0, 0x70, 0x40, 0x10, 0xe0, 0xb0,
        0x81, 0x51, 0x21, 0xf1, 0xc1, 0x91, 0x61, 0x31, /* low bytes */
        0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x04, 0x05,
        0x06, 0x07, 0x08, 0x08, 0x09, 0x0a, 0x0b, 0x0c, /* high bytes */
};

#else /* MLK_ARITH_BACKEND_AARCH64 && !MLK_CONFIG_MULTILEVEL_NO_SHARED &&   \
         !MLK_CONFIG_NO_DECAPS_API && (MLK_CONFIG_MULTILEVEL_WITH_SHARED || \
         MLKEM_K == 2 || MLKEM_K == 3) */

MLK_EMPTY_CU(aarch64_decompress_d4_table)

#endif /* !(MLK_ARITH_BACKEND_AARCH64 && !MLK_CONFIG_MULTILEVEL_NO_SHARED && \
          !MLK_CONFIG_NO_DECAPS_API && (MLK_CONFIG_MULTILEVEL_WITH_SHARED || \
          MLKEM_K == 2 || MLKEM_K == 3)) */
