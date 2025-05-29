/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#include "../../../common.h"
#if defined(MLK_ARITH_BACKEND_AARCH64) && \
    !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/aarch64_opt/src/poly_tomont_asm.S using scripts/simpasm. Do not modify it directly.
 */


.text
.balign 4
.global MLK_ASM_NAMESPACE(poly_tomont_asm)
MLK_ASM_FN_SYMBOL(poly_tomont_asm)

        mov	w2, #0xd01              // =3329
        dup	v4.8h, w2
        mov	w2, #0x4ebf             // =20159
        dup	v5.8h, w2
        mov	w2, #-0x414             // =-1044
        dup	v2.8h, w2
        mov	w2, #-0x2824            // =-10276
        dup	v3.8h, w2
        mov	x1, #0x8                // =8
        ldr	q26, [x0, #0x30]
        ldr	q23, [x0, #0x10]
        mul	v17.8h, v26.8h, v2.8h
        sqrdmulh	v7.8h, v26.8h, v3.8h
        ldr	q27, [x0, #0x20]
        sub	x1, x1, #0x1

poly_tomont_loop:
        mls	v17.8h, v7.8h, v4.h[0]
        sqrdmulh	v5.8h, v23.8h, v3.8h
        ldr	q7, [x0], #0x40
        stur	q17, [x0, #-0x10]
        sqrdmulh	v29.8h, v27.8h, v3.8h
        sqrdmulh	v19.8h, v7.8h, v3.8h
        mul	v25.8h, v23.8h, v2.8h
        mul	v0.8h, v7.8h, v2.8h
        mul	v26.8h, v27.8h, v2.8h
        ldr	q7, [x0, #0x30]
        mls	v25.8h, v5.8h, v4.h[0]
        ldr	q23, [x0, #0x10]
        mls	v26.8h, v29.8h, v4.h[0]
        mls	v0.8h, v19.8h, v4.h[0]
        stur	q25, [x0, #-0x30]
        mul	v17.8h, v7.8h, v2.8h
        sqrdmulh	v7.8h, v7.8h, v3.8h
        stur	q0, [x0, #-0x40]
        ldr	q27, [x0, #0x20]
        stur	q26, [x0, #-0x20]
        sub	x1, x1, #0x1
        cbnz	x1, poly_tomont_loop
        mls	v17.8h, v7.8h, v4.h[0]
        sqrdmulh	v7.8h, v23.8h, v3.8h
        mul	v26.8h, v23.8h, v2.8h
        sqrdmulh	v25.8h, v27.8h, v3.8h
        ldr	q23, [x0], #0x40
        mul	v27.8h, v27.8h, v2.8h
        mls	v26.8h, v7.8h, v4.h[0]
        sqrdmulh	v7.8h, v23.8h, v3.8h
        mul	v23.8h, v23.8h, v2.8h
        stur	q17, [x0, #-0x10]
        mls	v27.8h, v25.8h, v4.h[0]
        stur	q26, [x0, #-0x30]
        mls	v23.8h, v7.8h, v4.h[0]
        stur	q27, [x0, #-0x20]
        stur	q23, [x0, #-0x40]
        ret

#endif /* MLK_ARITH_BACKEND_AARCH64 && !MLK_CONFIG_MULTILEVEL_NO_SHARED */
