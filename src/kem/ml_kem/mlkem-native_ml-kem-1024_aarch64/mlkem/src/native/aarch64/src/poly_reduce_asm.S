/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#include "../../../common.h"
#if defined(MLK_ARITH_BACKEND_AARCH64) && \
    !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/aarch64_opt/src/poly_reduce_asm.S using scripts/simpasm. Do not modify it directly.
 */


.text
.balign 4
.global MLK_ASM_NAMESPACE(poly_reduce_asm)
MLK_ASM_FN_SYMBOL(poly_reduce_asm)

        mov	w2, #0xd01              // =3329
        dup	v3.8h, w2
        mov	w2, #0x4ebf             // =20159
        dup	v4.8h, w2
        mov	x1, #0x8                // =8
        ldr	q21, [x0, #0x20]
        ldr	q23, [x0, #0x30]
        sqdmulh	v7.8h, v21.8h, v4.h[0]
        sqdmulh	v30.8h, v23.8h, v4.h[0]
        srshr	v7.8h, v7.8h, #0xb
        srshr	v30.8h, v30.8h, #0xb
        mls	v21.8h, v7.8h, v3.h[0]
        mls	v23.8h, v30.8h, v3.h[0]
        ldr	q5, [x0, #0x10]
        sshr	v7.8h, v21.8h, #0xf
        sshr	v30.8h, v23.8h, #0xf
        and	v7.16b, v3.16b, v7.16b
        add	v21.8h, v21.8h, v7.8h
        and	v7.16b, v3.16b, v30.16b
        add	v16.8h, v23.8h, v7.8h
        sub	x1, x1, #0x1

poly_reduce_loop:
        ldr	q6, [x0], #0x40
        ldr	q30, [x0, #0x20]
        sqdmulh	v31.8h, v6.8h, v4.h[0]
        sqdmulh	v29.8h, v5.8h, v4.h[0]
        sqdmulh	v22.8h, v30.8h, v4.h[0]
        stur	q16, [x0, #-0x10]
        srshr	v20.8h, v31.8h, #0xb
        srshr	v28.8h, v29.8h, #0xb
        stur	q21, [x0, #-0x20]
        mls	v6.8h, v20.8h, v3.h[0]
        mls	v5.8h, v28.8h, v3.h[0]
        ldr	q2, [x0, #0x30]
        sshr	v31.8h, v6.8h, #0xf
        srshr	v19.8h, v22.8h, #0xb
        and	v22.16b, v3.16b, v31.16b
        add	v0.8h, v6.8h, v22.8h
        mls	v30.8h, v19.8h, v3.h[0]
        sshr	v26.8h, v5.8h, #0xf
        sqdmulh	v25.8h, v2.8h, v4.h[0]
        and	v17.16b, v3.16b, v26.16b
        add	v1.8h, v5.8h, v17.8h
        sshr	v31.8h, v30.8h, #0xf
        srshr	v25.8h, v25.8h, #0xb
        stur	q1, [x0, #-0x30]
        and	v18.16b, v3.16b, v31.16b
        mls	v2.8h, v25.8h, v3.h[0]
        add	v21.8h, v30.8h, v18.8h
        ldr	q5, [x0, #0x10]
        sshr	v18.8h, v2.8h, #0xf
        stur	q0, [x0, #-0x40]
        and	v27.16b, v3.16b, v18.16b
        add	v16.8h, v2.8h, v27.8h
        sub	x1, x1, #0x1
        cbnz	x1, poly_reduce_loop
        sqdmulh	v20.8h, v5.8h, v4.h[0]
        ldr	q24, [x0], #0x40
        stur	q21, [x0, #-0x20]
        srshr	v20.8h, v20.8h, #0xb
        sqdmulh	v25.8h, v24.8h, v4.h[0]
        stur	q16, [x0, #-0x10]
        mls	v5.8h, v20.8h, v3.h[0]
        srshr	v20.8h, v25.8h, #0xb
        sshr	v2.8h, v5.8h, #0xf
        mls	v24.8h, v20.8h, v3.h[0]
        and	v20.16b, v3.16b, v2.16b
        add	v31.8h, v5.8h, v20.8h
        sshr	v20.8h, v24.8h, #0xf
        stur	q31, [x0, #-0x30]
        and	v31.16b, v3.16b, v20.16b
        add	v24.8h, v24.8h, v31.8h
        stur	q24, [x0, #-0x40]
        ret

#endif /* MLK_ARITH_BACKEND_AARCH64 && !MLK_CONFIG_MULTILEVEL_NO_SHARED */
