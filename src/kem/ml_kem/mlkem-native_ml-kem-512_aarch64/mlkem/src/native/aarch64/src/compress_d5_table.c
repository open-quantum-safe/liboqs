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

#if defined(MLK_ARITH_BACKEND_AARCH64) &&        \
    !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED) && \
    (!defined(MLK_CONFIG_NO_ENCAPS_API) ||       \
     !defined(MLK_CONFIG_NO_DECAPS_API)) &&      \
    (defined(MLK_CONFIG_MULTILEVEL_WITH_SHARED) || MLKEM_K == 4)

#include "arith_native_aarch64.h"

/*
 * Constant vectors used by poly_compress_d5_aarch64_asm:
 * SQRDMULH multiplier (row 0) and TBL indices (row 1).
 * See autogen for details.
 */
MLK_ALIGN MLK_INTERNAL_DATA_DEFINITION const uint8_t
    mlk_aarch64_compress_d5_table[32] = {
        0x3b, 0x01, 0x3b, 0x01, 0x3b, 0x01, 0x3b, 0x01, 0x3b,
        0x01, 0x3b, 0x01, 0x3b, 0x01, 0x3b, 0x01, /* mult (16-bit lanes) */
        0x00, 0x01, 0x02, 0x03, 0x04, 0x08, 0x09, 0x0a, 0x0b,
        0x0c, 0x04, 0x08, 0x09, 0x0a, 0x0b, 0x0c, /* idx */
};

#else /* MLK_ARITH_BACKEND_AARCH64 && !MLK_CONFIG_MULTILEVEL_NO_SHARED && \
         (!MLK_CONFIG_NO_ENCAPS_API || !MLK_CONFIG_NO_DECAPS_API) &&      \
         (MLK_CONFIG_MULTILEVEL_WITH_SHARED || MLKEM_K == 4) */

MLK_EMPTY_CU(aarch64_compress_d5_table)

#endif /* !(MLK_ARITH_BACKEND_AARCH64 && !MLK_CONFIG_MULTILEVEL_NO_SHARED && \
          (!MLK_CONFIG_NO_ENCAPS_API || !MLK_CONFIG_NO_DECAPS_API) &&        \
          (MLK_CONFIG_MULTILEVEL_WITH_SHARED || MLKEM_K == 4)) */
