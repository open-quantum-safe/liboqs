/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#include "../../../common.h"
#if defined(MLK_ARITH_BACKEND_AARCH64) && \
    !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/aarch64_opt/src/poly_mulcache_compute_asm.S using scripts/simpasm. Do not modify it directly.
 */


.text
.balign 4
.global MLK_ASM_NAMESPACE(poly_mulcache_compute_asm)
MLK_ASM_FN_SYMBOL(poly_mulcache_compute_asm)

        mov	w5, #0xd01              // =3329
        dup	v6.8h, w5
        mov	w5, #0x4ebf             // =20159
        dup	v7.8h, w5
        mov	x4, #0x10               // =16
        ldr	q1, [x1, #0x10]
        ldr	q27, [x1], #0x20
        ldr	q23, [x2], #0x10
        uzp2	v27.8h, v27.8h, v1.8h
        ldr	q1, [x3], #0x10
        mul	v2.8h, v27.8h, v23.8h
        sqrdmulh	v27.8h, v27.8h, v1.8h
        sub	x4, x4, #0x1

poly_mulcache_compute_loop:
        ldr	q29, [x1, #0x10]
        ldr	q21, [x2], #0x10
        mls	v2.8h, v27.8h, v6.h[0]
        ldr	q27, [x1], #0x20
        ldr	q7, [x3], #0x10
        uzp2	v28.8h, v27.8h, v29.8h
        str	q2, [x0], #0x10
        mul	v2.8h, v28.8h, v21.8h
        sqrdmulh	v27.8h, v28.8h, v7.8h
        sub	x4, x4, #0x1
        cbnz	x4, poly_mulcache_compute_loop
        mls	v2.8h, v27.8h, v6.h[0]
        str	q2, [x0], #0x10
        ret

#endif /* MLK_ARITH_BACKEND_AARCH64 && !MLK_CONFIG_MULTILEVEL_NO_SHARED */
