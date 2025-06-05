/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#include "../../../common.h"
#if defined(MLK_ARITH_BACKEND_AARCH64) &&  !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/aarch64_opt/src/poly_tobytes_asm.S using scripts/simpasm. Do not modify it directly.
 */


.text
.balign 4
.global MLK_ASM_NAMESPACE(poly_tobytes_asm)
MLK_ASM_FN_SYMBOL(poly_tobytes_asm)

        mov	x2, #0x10               // =16
        ldr	q6, [x1], #0x20
        ldur	q24, [x1, #-0x10]
        ldr	q30, [x1], #0x20
        ldur	q22, [x1, #-0x10]
        ldr	q5, [x1], #0x20
        ldur	q17, [x1, #-0x10]
        ldr	q19, [x1], #0x20
        ldur	q4, [x1, #-0x10]
        lsr	x2, x2, #2
        sub	x2, x2, #0x1

poly_tobytes_loop_start:
        uzp1	v25.8h, v6.8h, v24.8h
        uzp2	v6.8h, v6.8h, v24.8h
        xtn	v24.8b, v25.8h
        shrn	v25.8b, v25.8h, #0x8
        xtn	v18.8b, v6.8h
        shrn	v26.8b, v6.8h, #0x4
        sli	v25.8b, v18.8b, #0x4
        st3	{ v24.8b, v25.8b, v26.8b }, [x0], #24
        uzp1	v25.8h, v30.8h, v22.8h
        uzp2	v6.8h, v30.8h, v22.8h
        xtn	v24.8b, v25.8h
        xtn	v18.8b, v6.8h
        uzp1	v30.8h, v5.8h, v17.8h
        uzp2	v22.8h, v5.8h, v17.8h
        xtn	v5.8b, v30.8h
        xtn	v17.8b, v22.8h
        uzp1	v28.8h, v19.8h, v4.8h
        uzp2	v19.8h, v19.8h, v4.8h
        xtn	v4.8b, v28.8h
        xtn	v20.8b, v19.8h
        shrn	v25.8b, v25.8h, #0x8
        sli	v25.8b, v18.8b, #0x4
        shrn	v26.8b, v6.8h, #0x4
        st3	{ v24.8b, v25.8b, v26.8b }, [x0], #24
        shrn	v6.8b, v30.8h, #0x8
        sli	v6.8b, v17.8b, #0x4
        shrn	v7.8b, v22.8h, #0x4
        st3	{ v5.8b, v6.8b, v7.8b }, [x0], #24
        shrn	v5.8b, v28.8h, #0x8
        shrn	v6.8b, v19.8h, #0x4
        sli	v5.8b, v20.8b, #0x4
        st3	{ v4.8b, v5.8b, v6.8b }, [x0], #24
        ldr	q6, [x1], #0x20
        ldur	q24, [x1, #-0x10]
        ldr	q30, [x1], #0x20
        ldur	q22, [x1, #-0x10]
        ldr	q5, [x1], #0x20
        ldur	q17, [x1, #-0x10]
        ldr	q19, [x1], #0x20
        ldur	q4, [x1, #-0x10]
        sub	x2, x2, #0x1
        cbnz	x2, poly_tobytes_loop_start
        uzp1	v25.8h, v30.8h, v22.8h
        uzp2	v18.8h, v30.8h, v22.8h
        uzp1	v30.8h, v6.8h, v24.8h
        uzp2	v6.8h, v6.8h, v24.8h
        uzp1	v24.8h, v5.8h, v17.8h
        uzp2	v22.8h, v5.8h, v17.8h
        uzp1	v5.8h, v19.8h, v4.8h
        uzp2	v17.8h, v19.8h, v4.8h
        xtn	v19.8b, v25.8h
        shrn	v20.8b, v25.8h, #0x8
        xtn	v25.8b, v18.8h
        shrn	v21.8b, v18.8h, #0x4
        xtn	v28.8b, v30.8h
        shrn	v29.8b, v30.8h, #0x8
        xtn	v18.8b, v6.8h
        shrn	v30.8b, v6.8h, #0x4
        xtn	v1.8b, v24.8h
        shrn	v2.8b, v24.8h, #0x8
        xtn	v6.8b, v22.8h
        shrn	v3.8b, v22.8h, #0x4
        xtn	v22.8b, v5.8h
        shrn	v23.8b, v5.8h, #0x8
        xtn	v5.8b, v17.8h
        shrn	v24.8b, v17.8h, #0x4
        sli	v20.8b, v25.8b, #0x4
        sli	v29.8b, v18.8b, #0x4
        st3	{ v28.8b, v29.8b, v30.8b }, [x0], #24
        st3	{ v19.8b, v20.8b, v21.8b }, [x0], #24
        sli	v2.8b, v6.8b, #0x4
        st3	{ v1.8b, v2.8b, v3.8b }, [x0], #24
        sli	v23.8b, v5.8b, #0x4
        st3	{ v22.8b, v23.8b, v24.8b }, [x0], #24
        ret

#endif /* MLK_ARITH_BACKEND_AARCH64 && !MLK_CONFIG_MULTILEVEL_NO_SHARED */
