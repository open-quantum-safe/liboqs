/*
 * Copyright (c) 2024-2025 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../../../common.h"
#if defined(MLK_ARITH_BACKEND_AARCH64_OPT) && \
    !defined(MLK_MULTILEVEL_BUILD_NO_SHARED)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/aarch64_opt/src/poly_tobytes_asm.S using scripts/simpasm. Do not modify it directly.
 */


.text
.balign 4
.global MLK_ASM_NAMESPACE(poly_tobytes_asm)
MLK_ASM_FN_SYMBOL(poly_tobytes_asm)

        mov	x2, #0x10               // =16

poly_tobytes_asm_asm_loop_start:
        ld2	{ v0.8h, v1.8h }, [x1], #32
        xtn	v2.8b, v0.8h
        shrn	v3.8b, v0.8h, #0x8
        xtn	v5.8b, v1.8h
        sli	v3.8b, v5.8b, #0x4
        shrn	v4.8b, v1.8h, #0x4
        st3	{ v2.8b, v3.8b, v4.8b }, [x0], #24
        subs	x2, x2, #0x1
        cbnz	x2, poly_tobytes_asm_asm_loop_start
        ret

#endif /* MLK_ARITH_BACKEND_AARCH64_OPT &&
          !MLK_MULTILEVEL_BUILD_NO_SHARED */
