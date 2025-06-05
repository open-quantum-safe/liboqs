/* Copyright (c) 2022 Arm Limited
 * Copyright (c) 2022 Hanno Becker
 * Copyright (c) 2023 Amin Abdulrahman, Matthias Kannwischer
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* References
 * ==========
 *
 * - [NeonNTT]
 *   Neon NTT: Faster Dilithium, Kyber, and Saber on Cortex-A72 and Apple M1
 *   Becker, Hwang, Kannwischer, Yang, Yang
 *   https://eprint.iacr.org/2021/986
 *
 * - [SLOTHY_Paper]
 *   Fast and Clean: Auditable high-performance assembly via constraint solving
 *   Abdulrahman, Becker, Kannwischer, Klein
 *   https://eprint.iacr.org/2022/1303
 */

/* AArch64 ML-KEM forward NTT following @[NeonNTT] and @[SLOTHY_Paper]. */

#include "../../../common.h"
#if defined(MLK_ARITH_BACKEND_AARCH64) &&  !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/aarch64_opt/src/ntt.S using scripts/simpasm. Do not modify it directly.
 */


.text
.balign 4
.global MLK_ASM_NAMESPACE(ntt_asm)
MLK_ASM_FN_SYMBOL(ntt_asm)

        sub	sp, sp, #0x40
        stp	d8, d9, [sp]
        stp	d10, d11, [sp, #0x10]
        stp	d12, d13, [sp, #0x20]
        stp	d14, d15, [sp, #0x30]
        mov	w5, #0xd01              // =3329
        mov	v7.h[0], w5
        mov	w5, #0x4ebf             // =20159
        mov	v7.h[1], w5
        mov	x3, x0
        mov	x4, #0x4                // =4
        ldr	q0, [x1], #0x20
        ldur	q1, [x1, #-0x10]
        ldr	q5, [x0]
        ldr	q13, [x0, #0x40]
        ldr	q3, [x0, #0x80]
        ldr	q22, [x0, #0xc0]
        ldr	q24, [x0, #0x100]
        ldr	q11, [x0, #0x1c0]
        mul	v23.8h, v24.8h, v0.h[0]
        ldr	q2, [x0, #0x140]
        mul	v17.8h, v11.8h, v0.h[0]
        ldr	q19, [x0, #0x180]
        sub	x4, x4, #0x1

ntt_layer123_start:
        sqrdmulh	v8.8h, v24.8h, v0.h[1]
        sqrdmulh	v24.8h, v2.8h, v0.h[1]
        mul	v2.8h, v2.8h, v0.h[0]
        sqrdmulh	v14.8h, v19.8h, v0.h[1]
        mls	v23.8h, v8.8h, v7.h[0]
        mul	v8.8h, v19.8h, v0.h[0]
        mls	v2.8h, v24.8h, v7.h[0]
        sqrdmulh	v24.8h, v11.8h, v0.h[1]
        sub	v11.8h, v5.8h, v23.8h
        mls	v8.8h, v14.8h, v7.h[0]
        sub	v14.8h, v13.8h, v2.8h
        add	v2.8h, v13.8h, v2.8h
        add	v23.8h, v5.8h, v23.8h
        sub	v19.8h, v3.8h, v8.8h
        add	v8.8h, v3.8h, v8.8h
        mls	v17.8h, v24.8h, v7.h[0]
        sqrdmulh	v24.8h, v19.8h, v0.h[5]
        mul	v19.8h, v19.8h, v0.h[4]
        sqrdmulh	v5.8h, v8.8h, v0.h[3]
        sub	v13.8h, v22.8h, v17.8h
        add	v17.8h, v22.8h, v17.8h
        mls	v19.8h, v24.8h, v7.h[0]
        sqrdmulh	v24.8h, v13.8h, v0.h[5]
        mul	v13.8h, v13.8h, v0.h[4]
        mul	v8.8h, v8.8h, v0.h[2]
        sub	v3.8h, v11.8h, v19.8h
        add	v11.8h, v11.8h, v19.8h
        mls	v13.8h, v24.8h, v7.h[0]
        sqrdmulh	v24.8h, v17.8h, v0.h[3]
        mul	v19.8h, v17.8h, v0.h[2]
        mls	v8.8h, v5.8h, v7.h[0]
        sub	v17.8h, v14.8h, v13.8h
        add	v14.8h, v14.8h, v13.8h
        mls	v19.8h, v24.8h, v7.h[0]
        sub	v24.8h, v23.8h, v8.8h
        add	v8.8h, v23.8h, v8.8h
        sqrdmulh	v23.8h, v14.8h, v1.h[3]
        sub	v5.8h, v2.8h, v19.8h
        add	v2.8h, v2.8h, v19.8h
        mul	v14.8h, v14.8h, v1.h[2]
        sqrdmulh	v19.8h, v5.8h, v1.h[1]
        sqrdmulh	v13.8h, v2.8h, v0.h[7]
        mul	v2.8h, v2.8h, v0.h[6]
        mul	v5.8h, v5.8h, v1.h[0]
        mls	v14.8h, v23.8h, v7.h[0]
        sqrdmulh	v23.8h, v17.8h, v1.h[5]
        mls	v2.8h, v13.8h, v7.h[0]
        mls	v5.8h, v19.8h, v7.h[0]
        sub	v19.8h, v11.8h, v14.8h
        add	v14.8h, v11.8h, v14.8h
        sub	v11.8h, v8.8h, v2.8h
        mul	v17.8h, v17.8h, v1.h[4]
        add	v8.8h, v8.8h, v2.8h
        sub	v2.8h, v24.8h, v5.8h
        add	v24.8h, v24.8h, v5.8h
        mls	v17.8h, v23.8h, v7.h[0]
        str	q8, [x0], #0x10
        ldr	q5, [x0]
        sub	v8.8h, v3.8h, v17.8h
        add	v23.8h, v3.8h, v17.8h
        str	q11, [x0, #0x30]
        ldr	q13, [x0, #0x40]
        str	q24, [x0, #0x70]
        ldr	q3, [x0, #0x80]
        str	q2, [x0, #0xb0]
        ldr	q22, [x0, #0xc0]
        str	q14, [x0, #0xf0]
        ldr	q24, [x0, #0x100]
        str	q19, [x0, #0x130]
        ldr	q2, [x0, #0x140]
        str	q23, [x0, #0x170]
        mul	v23.8h, v24.8h, v0.h[0]
        str	q8, [x0, #0x1b0]
        ldr	q11, [x0, #0x1c0]
        ldr	q19, [x0, #0x180]
        mul	v17.8h, v11.8h, v0.h[0]
        subs	x4, x4, #0x1
        cbnz	x4, ntt_layer123_start
        sqrdmulh	v6.8h, v11.8h, v0.h[1]
        mul	v25.8h, v19.8h, v0.h[0]
        sqrdmulh	v12.8h, v19.8h, v0.h[1]
        mul	v11.8h, v2.8h, v0.h[0]
        mls	v17.8h, v6.8h, v7.h[0]
        sqrdmulh	v14.8h, v2.8h, v0.h[1]
        mls	v25.8h, v12.8h, v7.h[0]
        sqrdmulh	v27.8h, v24.8h, v0.h[1]
        add	v9.8h, v22.8h, v17.8h
        mls	v11.8h, v14.8h, v7.h[0]
        sub	v26.8h, v3.8h, v25.8h
        sqrdmulh	v2.8h, v9.8h, v0.h[3]
        mul	v24.8h, v9.8h, v0.h[2]
        mul	v19.8h, v26.8h, v0.h[4]
        sqrdmulh	v14.8h, v26.8h, v0.h[5]
        mls	v23.8h, v27.8h, v7.h[0]
        mls	v24.8h, v2.8h, v7.h[0]
        add	v6.8h, v13.8h, v11.8h
        mls	v19.8h, v14.8h, v7.h[0]
        sub	v4.8h, v5.8h, v23.8h
        add	v10.8h, v3.8h, v25.8h
        sub	v8.8h, v6.8h, v24.8h
        add	v3.8h, v4.8h, v19.8h
        sub	v31.8h, v4.8h, v19.8h
        mul	v14.8h, v8.8h, v1.h[0]
        sqrdmulh	v4.8h, v10.8h, v0.h[3]
        mul	v12.8h, v10.8h, v0.h[2]
        sqrdmulh	v2.8h, v8.8h, v1.h[1]
        sub	v8.8h, v22.8h, v17.8h
        add	v30.8h, v5.8h, v23.8h
        mls	v12.8h, v4.8h, v7.h[0]
        sqrdmulh	v4.8h, v8.8h, v0.h[5]
        mul	v19.8h, v8.8h, v0.h[4]
        mls	v14.8h, v2.8h, v7.h[0]
        sub	v27.8h, v30.8h, v12.8h
        sub	v23.8h, v13.8h, v11.8h
        mls	v19.8h, v4.8h, v7.h[0]
        sub	v2.8h, v27.8h, v14.8h
        add	v8.8h, v27.8h, v14.8h
        add	v14.8h, v6.8h, v24.8h
        str	q2, [x0, #0xc0]
        add	v2.8h, v23.8h, v19.8h
        str	q8, [x0, #0x80]
        sub	v19.8h, v23.8h, v19.8h
        sqrdmulh	v13.8h, v2.8h, v1.h[3]
        mul	v17.8h, v2.8h, v1.h[2]
        add	v27.8h, v30.8h, v12.8h
        sqrdmulh	v24.8h, v19.8h, v1.h[5]
        mul	v19.8h, v19.8h, v1.h[4]
        mls	v17.8h, v13.8h, v7.h[0]
        sqrdmulh	v8.8h, v14.8h, v0.h[7]
        mul	v2.8h, v14.8h, v0.h[6]
        mls	v19.8h, v24.8h, v7.h[0]
        add	v26.8h, v3.8h, v17.8h
        sub	v14.8h, v3.8h, v17.8h
        mls	v2.8h, v8.8h, v7.h[0]
        str	q26, [x0, #0x100]
        add	v8.8h, v31.8h, v19.8h
        str	q14, [x0, #0x140]
        sub	v24.8h, v31.8h, v19.8h
        str	q8, [x0, #0x180]
        add	v18.8h, v27.8h, v2.8h
        str	q24, [x0, #0x1c0]
        sub	v14.8h, v27.8h, v2.8h
        str	q18, [x0], #0x10
        str	q14, [x0, #0x30]
        mov	x0, x3
        mov	x4, #0x8                // =8
        ldr	q11, [x1], #0x10
        ldr	q24, [x0, #0x30]
        ldr	q8, [x0, #0x20]
        sqrdmulh	v14.8h, v24.8h, v11.h[1]
        mul	v2.8h, v24.8h, v11.h[0]
        sqrdmulh	v9.8h, v8.8h, v11.h[1]
        ldr	q24, [x0, #0x10]
        mls	v2.8h, v14.8h, v7.h[0]
        mul	v14.8h, v8.8h, v11.h[0]
        ldr	q6, [x2, #0x40]
        sub	v8.8h, v24.8h, v2.8h
        mls	v14.8h, v9.8h, v7.h[0]
        add	v2.8h, v24.8h, v2.8h
        mul	v27.8h, v8.8h, v11.h[4]
        sqrdmulh	v8.8h, v8.8h, v11.h[5]
        mul	v24.8h, v2.8h, v11.h[2]
        sqrdmulh	v11.8h, v2.8h, v11.h[3]
        mls	v27.8h, v8.8h, v7.h[0]
        ldr	q5, [x2, #0x50]
        sub	x4, x4, #0x1

ntt_layer4567_start:
        ldr	q8, [x0]
        ldr	q17, [x2, #0x10]
        sub	v1.8h, v8.8h, v14.8h
        mls	v24.8h, v11.8h, v7.h[0]
        add	v8.8h, v8.8h, v14.8h
        sub	v0.8h, v1.8h, v27.8h
        add	v12.8h, v1.8h, v27.8h
        sub	v19.8h, v8.8h, v24.8h
        add	v8.8h, v8.8h, v24.8h
        trn1	v24.4s, v12.4s, v0.4s
        trn2	v13.4s, v12.4s, v0.4s
        trn1	v23.4s, v8.4s, v19.4s
        ldr	q2, [x2], #0x60
        trn2	v9.2d, v23.2d, v24.2d
        trn2	v8.4s, v8.4s, v19.4s
        sqrdmulh	v26.8h, v9.8h, v17.8h
        trn1	v24.2d, v23.2d, v24.2d
        trn2	v11.2d, v8.2d, v13.2d
        trn1	v29.2d, v8.2d, v13.2d
        sqrdmulh	v23.8h, v11.8h, v17.8h
        mul	v10.8h, v11.8h, v2.8h
        mul	v0.8h, v9.8h, v2.8h
        ldur	q11, [x2, #-0x40]
        mls	v10.8h, v23.8h, v7.h[0]
        mls	v0.8h, v26.8h, v7.h[0]
        ldur	q19, [x2, #-0x30]
        add	v17.8h, v29.8h, v10.8h
        sub	v23.8h, v24.8h, v0.8h
        sub	v30.8h, v29.8h, v10.8h
        mul	v2.8h, v17.8h, v11.8h
        sqrdmulh	v11.8h, v17.8h, v19.8h
        mul	v8.8h, v30.8h, v6.8h
        ldr	q22, [x0, #0x70]
        mls	v2.8h, v11.8h, v7.h[0]
        add	v24.8h, v24.8h, v0.8h
        ldr	q15, [x1], #0x10
        sub	v14.8h, v24.8h, v2.8h
        add	v24.8h, v24.8h, v2.8h
        sqrdmulh	v1.8h, v22.8h, v15.h[1]
        mul	v2.8h, v22.8h, v15.h[0]
        trn1	v0.4s, v24.4s, v14.4s
        trn2	v24.4s, v24.4s, v14.4s
        sqrdmulh	v19.8h, v30.8h, v5.8h
        mls	v2.8h, v1.8h, v7.h[0]
        ldr	q16, [x0, #0x60]
        mls	v8.8h, v19.8h, v7.h[0]
        ldr	q6, [x2, #0x40]
        mul	v14.8h, v16.8h, v15.h[0]
        sub	v3.8h, v23.8h, v8.8h
        add	v8.8h, v23.8h, v8.8h
        ldr	q5, [x2, #0x50]
        trn2	v23.4s, v8.4s, v3.4s
        trn1	v31.4s, v8.4s, v3.4s
        sqrdmulh	v8.8h, v16.8h, v15.h[1]
        trn2	v25.2d, v24.2d, v23.2d
        trn1	v29.2d, v24.2d, v23.2d
        ldr	q24, [x0, #0x50]
        trn1	v16.2d, v0.2d, v31.2d
        mls	v14.8h, v8.8h, v7.h[0]
        sub	v13.8h, v24.8h, v2.8h
        add	v24.8h, v24.8h, v2.8h
        trn2	v2.2d, v0.2d, v31.2d
        sqrdmulh	v19.8h, v13.8h, v15.h[5]
        str	q2, [x0, #0x20]
        sqrdmulh	v11.8h, v24.8h, v15.h[3]
        str	q16, [x0], #0x40
        mul	v27.8h, v13.8h, v15.h[4]
        stur	q29, [x0, #-0x30]
        mul	v24.8h, v24.8h, v15.h[2]
        stur	q25, [x0, #-0x10]
        mls	v27.8h, v19.8h, v7.h[0]
        subs	x4, x4, #0x1
        cbnz	x4, ntt_layer4567_start
        ldr	q23, [x0]
        ldr	q17, [x2], #0x60
        sub	v19.8h, v23.8h, v14.8h
        mls	v24.8h, v11.8h, v7.h[0]
        add	v14.8h, v23.8h, v14.8h
        add	v8.8h, v19.8h, v27.8h
        sub	v13.8h, v19.8h, v27.8h
        add	v12.8h, v14.8h, v24.8h
        sub	v24.8h, v14.8h, v24.8h
        trn1	v0.4s, v8.4s, v13.4s
        trn2	v23.4s, v8.4s, v13.4s
        trn2	v19.4s, v12.4s, v24.4s
        ldur	q27, [x2, #-0x50]
        trn2	v8.2d, v19.2d, v23.2d
        trn1	v22.4s, v12.4s, v24.4s
        mul	v14.8h, v8.8h, v17.8h
        sqrdmulh	v24.8h, v8.8h, v27.8h
        trn2	v2.2d, v22.2d, v0.2d
        trn1	v8.2d, v19.2d, v23.2d
        mul	v11.8h, v2.8h, v17.8h
        mls	v14.8h, v24.8h, v7.h[0]
        ldur	q26, [x2, #-0x30]
        sqrdmulh	v23.8h, v2.8h, v27.8h
        sub	v24.8h, v8.8h, v14.8h
        ldur	q2, [x2, #-0x40]
        sqrdmulh	v19.8h, v24.8h, v5.8h
        add	v14.8h, v8.8h, v14.8h
        mul	v24.8h, v24.8h, v6.8h
        mls	v11.8h, v23.8h, v7.h[0]
        sqrdmulh	v8.8h, v14.8h, v26.8h
        mul	v2.8h, v14.8h, v2.8h
        trn1	v14.2d, v22.2d, v0.2d
        mls	v24.8h, v19.8h, v7.h[0]
        sub	v23.8h, v14.8h, v11.8h
        mls	v2.8h, v8.8h, v7.h[0]
        add	v14.8h, v14.8h, v11.8h
        add	v8.8h, v23.8h, v24.8h
        sub	v24.8h, v23.8h, v24.8h
        sub	v19.8h, v14.8h, v2.8h
        add	v11.8h, v14.8h, v2.8h
        trn1	v2.4s, v8.4s, v24.4s
        trn2	v14.4s, v8.4s, v24.4s
        trn2	v23.4s, v11.4s, v19.4s
        trn1	v11.4s, v11.4s, v19.4s
        trn2	v8.2d, v23.2d, v14.2d
        trn1	v24.2d, v11.2d, v2.2d
        str	q8, [x0, #0x30]
        trn2	v8.2d, v11.2d, v2.2d
        str	q24, [x0], #0x40
        trn1	v24.2d, v23.2d, v14.2d
        stur	q8, [x0, #-0x20]
        stur	q24, [x0, #-0x30]
        ldp	d8, d9, [sp]
        ldp	d10, d11, [sp, #0x10]
        ldp	d12, d13, [sp, #0x20]
        ldp	d14, d15, [sp, #0x30]
        add	sp, sp, #0x40
        ret

#endif /* MLK_ARITH_BACKEND_AARCH64 && !MLK_CONFIG_MULTILEVEL_NO_SHARED */
