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

/* AArch64 ML-KEM inverse NTT following @[NeonNTT] and @[SLOTHY_Paper]. */

#include "../../../common.h"
#if defined(MLK_ARITH_BACKEND_AARCH64) &&  !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/aarch64_opt/src/intt.S using scripts/simpasm. Do not modify it directly.
 */


.text
.balign 4
.global MLK_ASM_NAMESPACE(intt_asm)
MLK_ASM_FN_SYMBOL(intt_asm)

        sub	sp, sp, #0x40
        stp	d8, d9, [sp]
        stp	d10, d11, [sp, #0x10]
        stp	d12, d13, [sp, #0x20]
        stp	d14, d15, [sp, #0x30]
        mov	w5, #0xd01              // =3329
        mov	v7.h[0], w5
        mov	w5, #0x4ebf             // =20159
        mov	v7.h[1], w5
        mov	w5, #0x200              // =512
        dup	v29.8h, w5
        mov	w5, #0x13b0             // =5040
        dup	v30.8h, w5
        mov	x3, x0
        mov	x4, #0x8                // =8

intt_scale_start:
        ldr	q8, [x3]
        ldr	q9, [x3, #0x10]
        ldr	q10, [x3, #0x20]
        ldr	q11, [x3, #0x30]
        sqrdmulh	v27.8h, v8.8h, v30.8h
        mul	v8.8h, v8.8h, v29.8h
        mls	v8.8h, v27.8h, v7.h[0]
        sqrdmulh	v27.8h, v9.8h, v30.8h
        mul	v9.8h, v9.8h, v29.8h
        mls	v9.8h, v27.8h, v7.h[0]
        sqrdmulh	v27.8h, v10.8h, v30.8h
        mul	v10.8h, v10.8h, v29.8h
        mls	v10.8h, v27.8h, v7.h[0]
        sqrdmulh	v27.8h, v11.8h, v30.8h
        mul	v11.8h, v11.8h, v29.8h
        mls	v11.8h, v27.8h, v7.h[0]
        str	q8, [x3], #0x40
        stur	q9, [x3, #-0x30]
        stur	q10, [x3, #-0x20]
        stur	q11, [x3, #-0x10]
        subs	x4, x4, #0x1
        cbnz	x4, intt_scale_start
        mov	x3, x0
        mov	x4, #0x8                // =8
        ldr	q3, [x3, #0x10]
        ldr	q20, [x3]
        ldr	q25, [x3, #0x20]
        ldr	q24, [x3, #0x30]
        ldr	q21, [x2, #0x50]
        trn1	v18.4s, v25.4s, v24.4s
        trn1	v6.4s, v20.4s, v3.4s
        trn2	v12.4s, v25.4s, v24.4s
        trn2	v31.4s, v20.4s, v3.4s
        trn2	v28.2d, v6.2d, v18.2d
        trn1	v25.2d, v6.2d, v18.2d
        trn2	v15.2d, v31.2d, v12.2d
        trn1	v20.2d, v31.2d, v12.2d
        add	v4.8h, v28.8h, v15.8h
        add	v1.8h, v25.8h, v20.8h
        sub	v30.8h, v28.8h, v15.8h
        sub	v3.8h, v25.8h, v20.8h
        add	v6.8h, v1.8h, v4.8h
        sqrdmulh	v9.8h, v30.8h, v21.8h
        ldr	q21, [x2, #0x40]
        ldr	q25, [x2, #0x30]
        mul	v21.8h, v30.8h, v21.8h
        ldr	q30, [x2, #0x20]
        sub	v28.8h, v1.8h, v4.8h
        ldr	q1, [x2, #0x10]
        mls	v21.8h, v9.8h, v7.h[0]
        sqrdmulh	v9.8h, v3.8h, v25.8h
        mul	v20.8h, v3.8h, v30.8h
        ldr	q29, [x2], #0x60
        ldr	q17, [x3, #0x60]
        mls	v20.8h, v9.8h, v7.h[0]
        ldr	q3, [x3, #0x70]
        mul	v4.8h, v28.8h, v29.8h
        sub	v25.8h, v20.8h, v21.8h
        trn1	v15.4s, v17.4s, v3.4s
        sqrdmulh	v28.8h, v28.8h, v1.8h
        trn2	v31.4s, v17.4s, v3.4s
        mul	v30.8h, v25.8h, v29.8h
        add	v20.8h, v20.8h, v21.8h
        mls	v4.8h, v28.8h, v7.h[0]
        sqrdmulh	v3.8h, v25.8h, v1.8h
        ldr	q28, [x3, #0x40]
        trn1	v25.4s, v6.4s, v20.4s
        mls	v30.8h, v3.8h, v7.h[0]
        ldr	q27, [x3, #0x50]
        trn2	v6.4s, v6.4s, v20.4s
        trn1	v3.4s, v4.4s, v30.4s
        trn2	v10.4s, v28.4s, v27.4s
        trn2	v20.4s, v4.4s, v30.4s
        trn2	v8.2d, v25.2d, v3.2d
        trn1	v9.2d, v25.2d, v3.2d
        trn1	v1.2d, v6.2d, v20.2d
        trn2	v30.2d, v6.2d, v20.2d
        add	v4.8h, v9.8h, v1.8h
        add	v11.8h, v8.8h, v30.8h
        trn2	v25.2d, v10.2d, v31.2d
        sqdmulh	v6.8h, v4.8h, v7.h[1]
        sqdmulh	v20.8h, v11.8h, v7.h[1]
        ldr	q21, [x2, #0x50]
        srshr	v0.8h, v6.8h, #0xb
        srshr	v3.8h, v20.8h, #0xb
        trn1	v2.4s, v28.4s, v27.4s
        mls	v4.8h, v0.8h, v7.h[0]
        mls	v11.8h, v3.8h, v7.h[0]
        ldr	q0, [x1], #0x10
        trn2	v20.2d, v2.2d, v15.2d
        sub	v6.8h, v4.8h, v11.8h
        sub	v5.8h, v20.8h, v25.8h
        sub	v22.8h, v9.8h, v1.8h
        sqrdmulh	v3.8h, v6.8h, v0.h[1]
        mul	v6.8h, v6.8h, v0.h[0]
        sqrdmulh	v12.8h, v5.8h, v21.8h
        ldr	q19, [x2, #0x40]
        mls	v6.8h, v3.8h, v7.h[0]
        ldr	q14, [x2], #0x60
        sub	x4, x4, #0x2

intt_layer4567_start:
        str	q6, [x3, #0x20]
        ldur	q18, [x2, #-0x50]
        mul	v26.8h, v5.8h, v19.8h
        trn1	v16.2d, v10.2d, v31.2d
        mul	v27.8h, v22.8h, v0.h[2]
        trn1	v10.2d, v2.2d, v15.2d
        add	v5.8h, v4.8h, v11.8h
        mls	v26.8h, v12.8h, v7.h[0]
        add	v11.8h, v10.8h, v16.8h
        add	v6.8h, v20.8h, v25.8h
        ldur	q25, [x2, #-0x40]
        ldur	q28, [x2, #-0x30]
        ldr	q2, [x3, #0xa0]
        ldr	q19, [x2, #0x40]
        sub	v17.8h, v8.8h, v30.8h
        ldr	q1, [x3, #0x90]
        sqrdmulh	v9.8h, v17.8h, v0.h[5]
        str	q5, [x3], #0x40
        ldr	q30, [x3, #0x70]
        sub	v10.8h, v10.8h, v16.8h
        ldr	q16, [x3, #0x40]
        sqrdmulh	v24.8h, v10.8h, v28.8h
        mul	v13.8h, v10.8h, v25.8h
        sub	v21.8h, v11.8h, v6.8h
        trn1	v15.4s, v2.4s, v30.4s
        trn2	v31.4s, v2.4s, v30.4s
        mls	v13.8h, v24.8h, v7.h[0]
        mul	v29.8h, v21.8h, v14.8h
        ldr	q12, [x2, #0x50]
        sub	v28.8h, v13.8h, v26.8h
        trn2	v10.4s, v16.4s, v1.4s
        add	v30.8h, v11.8h, v6.8h
        sqrdmulh	v2.8h, v28.8h, v18.8h
        mul	v8.8h, v28.8h, v14.8h
        sqrdmulh	v18.8h, v21.8h, v18.8h
        ldr	q14, [x2], #0x60
        mls	v8.8h, v2.8h, v7.h[0]
        add	v11.8h, v13.8h, v26.8h
        mls	v29.8h, v18.8h, v7.h[0]
        sqrdmulh	v20.8h, v22.8h, v0.h[3]
        trn1	v23.4s, v30.4s, v11.4s
        trn2	v28.4s, v30.4s, v11.4s
        trn2	v13.4s, v29.4s, v8.4s
        trn1	v11.4s, v29.4s, v8.4s
        mls	v27.8h, v20.8h, v7.h[0]
        trn1	v21.2d, v28.2d, v13.2d
        trn2	v8.2d, v23.2d, v11.2d
        trn1	v24.2d, v23.2d, v11.2d
        mul	v26.8h, v17.8h, v0.h[4]
        trn2	v30.2d, v28.2d, v13.2d
        add	v4.8h, v24.8h, v21.8h
        add	v11.8h, v8.8h, v30.8h
        mls	v26.8h, v9.8h, v7.h[0]
        sqdmulh	v17.8h, v4.8h, v7.h[1]
        sqdmulh	v29.8h, v11.8h, v7.h[1]
        trn2	v25.2d, v10.2d, v31.2d
        add	v2.8h, v27.8h, v26.8h
        srshr	v28.8h, v17.8h, #0xb
        srshr	v13.8h, v29.8h, #0xb
        sqdmulh	v20.8h, v2.8h, v7.h[1]
        sub	v5.8h, v27.8h, v26.8h
        mls	v4.8h, v28.8h, v7.h[0]
        mls	v11.8h, v13.8h, v7.h[0]
        srshr	v23.8h, v20.8h, #0xb
        sqrdmulh	v17.8h, v5.8h, v0.h[1]
        mul	v9.8h, v5.8h, v0.h[0]
        mls	v2.8h, v23.8h, v7.h[0]
        sub	v29.8h, v4.8h, v11.8h
        ldr	q0, [x1], #0x10
        stur	q2, [x3, #-0x30]
        trn1	v2.4s, v16.4s, v1.4s
        sqrdmulh	v3.8h, v29.8h, v0.h[1]
        mul	v6.8h, v29.8h, v0.h[0]
        trn2	v20.2d, v2.2d, v15.2d
        mls	v9.8h, v17.8h, v7.h[0]
        sub	v5.8h, v20.8h, v25.8h
        mls	v6.8h, v3.8h, v7.h[0]
        sub	v22.8h, v24.8h, v21.8h
        stur	q9, [x3, #-0x10]
        sqrdmulh	v12.8h, v5.8h, v12.8h
        subs	x4, x4, #0x1
        cbnz	x4, intt_layer4567_start
        mul	v21.8h, v22.8h, v0.h[2]
        mul	v28.8h, v5.8h, v19.8h
        trn1	v10.2d, v10.2d, v31.2d
        trn1	v2.2d, v2.2d, v15.2d
        add	v11.8h, v4.8h, v11.8h
        sub	v30.8h, v8.8h, v30.8h
        add	v23.8h, v20.8h, v25.8h
        add	v24.8h, v2.8h, v10.8h
        mul	v8.8h, v30.8h, v0.h[4]
        sqrdmulh	v5.8h, v30.8h, v0.h[5]
        sqrdmulh	v22.8h, v22.8h, v0.h[3]
        add	v30.8h, v24.8h, v23.8h
        ldur	q26, [x2, #-0x30]
        mls	v8.8h, v5.8h, v7.h[0]
        sub	v5.8h, v2.8h, v10.8h
        ldur	q13, [x2, #-0x40]
        mls	v21.8h, v22.8h, v7.h[0]
        str	q6, [x3, #0x20]
        mul	v3.8h, v5.8h, v13.8h
        sqrdmulh	v22.8h, v5.8h, v26.8h
        sub	v18.8h, v21.8h, v8.8h
        mls	v28.8h, v12.8h, v7.h[0]
        str	q11, [x3], #0x40
        mls	v3.8h, v22.8h, v7.h[0]
        sqrdmulh	v16.8h, v18.8h, v0.h[1]
        sub	v10.8h, v24.8h, v23.8h
        mul	v17.8h, v18.8h, v0.h[0]
        sub	v11.8h, v3.8h, v28.8h
        mul	v13.8h, v10.8h, v14.8h
        add	v22.8h, v3.8h, v28.8h
        mul	v14.8h, v11.8h, v14.8h
        ldur	q26, [x2, #-0x50]
        trn2	v2.4s, v30.4s, v22.4s
        mls	v17.8h, v16.8h, v7.h[0]
        sqrdmulh	v10.8h, v10.8h, v26.8h
        sqrdmulh	v11.8h, v11.8h, v26.8h
        ldr	q9, [x1], #0x10
        mls	v13.8h, v10.8h, v7.h[0]
        mls	v14.8h, v11.8h, v7.h[0]
        trn1	v6.4s, v30.4s, v22.4s
        add	v8.8h, v21.8h, v8.8h
        stur	q17, [x3, #-0x10]
        trn2	v0.4s, v13.4s, v14.4s
        trn1	v1.4s, v13.4s, v14.4s
        sqdmulh	v13.8h, v8.8h, v7.h[1]
        trn1	v24.2d, v2.2d, v0.2d
        trn2	v2.2d, v2.2d, v0.2d
        trn2	v26.2d, v6.2d, v1.2d
        trn1	v11.2d, v6.2d, v1.2d
        add	v22.8h, v26.8h, v2.8h
        sub	v28.8h, v11.8h, v24.8h
        sub	v27.8h, v26.8h, v2.8h
        add	v10.8h, v11.8h, v24.8h
        sqrdmulh	v11.8h, v28.8h, v9.h[3]
        mul	v24.8h, v28.8h, v9.h[2]
        sqdmulh	v1.8h, v22.8h, v7.h[1]
        sqrdmulh	v0.8h, v27.8h, v9.h[5]
        srshr	v12.8h, v13.8h, #0xb
        mls	v24.8h, v11.8h, v7.h[0]
        sqdmulh	v14.8h, v10.8h, v7.h[1]
        mul	v27.8h, v27.8h, v9.h[4]
        mls	v8.8h, v12.8h, v7.h[0]
        srshr	v5.8h, v1.8h, #0xb
        srshr	v14.8h, v14.8h, #0xb
        mls	v27.8h, v0.8h, v7.h[0]
        mls	v22.8h, v5.8h, v7.h[0]
        mls	v10.8h, v14.8h, v7.h[0]
        stur	q8, [x3, #-0x30]
        sub	v2.8h, v24.8h, v27.8h
        add	v14.8h, v24.8h, v27.8h
        sub	v11.8h, v10.8h, v22.8h
        add	v20.8h, v10.8h, v22.8h
        sqdmulh	v22.8h, v14.8h, v7.h[1]
        sqrdmulh	v8.8h, v11.8h, v9.h[1]
        mul	v27.8h, v11.8h, v9.h[0]
        sqrdmulh	v0.8h, v2.8h, v9.h[1]
        mul	v11.8h, v2.8h, v9.h[0]
        srshr	v10.8h, v22.8h, #0xb
        mls	v27.8h, v8.8h, v7.h[0]
        str	q20, [x3], #0x40
        mls	v11.8h, v0.8h, v7.h[0]
        mls	v14.8h, v10.8h, v7.h[0]
        stur	q27, [x3, #-0x20]
        stur	q11, [x3, #-0x10]
        stur	q14, [x3, #-0x30]
        mov	x4, #0x4                // =4
        ldr	q0, [x1], #0x20
        ldur	q1, [x1, #-0x10]
        ldr	q2, [x0]
        ldr	q10, [x0, #0x40]
        ldr	q11, [x0, #0x80]
        sub	v14.8h, v2.8h, v10.8h
        add	v2.8h, v2.8h, v10.8h
        ldr	q10, [x0, #0xc0]
        sqrdmulh	v8.8h, v14.8h, v0.h[7]
        mul	v14.8h, v14.8h, v0.h[6]
        sub	v22.8h, v11.8h, v10.8h
        add	v10.8h, v11.8h, v10.8h
        ldr	q11, [x0, #0x1c0]
        add	v13.8h, v2.8h, v10.8h
        sub	v2.8h, v2.8h, v10.8h
        sqrdmulh	v10.8h, v22.8h, v1.h[1]
        mul	v22.8h, v22.8h, v1.h[0]
        mls	v14.8h, v8.8h, v7.h[0]
        sqrdmulh	v8.8h, v2.8h, v0.h[3]
        mul	v2.8h, v2.8h, v0.h[2]
        mls	v22.8h, v10.8h, v7.h[0]
        ldr	q10, [x0, #0x100]
        mls	v2.8h, v8.8h, v7.h[0]
        sub	v8.8h, v14.8h, v22.8h
        add	v14.8h, v14.8h, v22.8h
        ldr	q22, [x0, #0x180]
        sqrdmulh	v24.8h, v8.8h, v0.h[3]
        mul	v8.8h, v8.8h, v0.h[2]
        sub	v26.8h, v22.8h, v11.8h
        add	v11.8h, v22.8h, v11.8h
        ldr	q22, [x0, #0x140]
        sqrdmulh	v16.8h, v26.8h, v1.h[5]
        mul	v26.8h, v26.8h, v1.h[4]
        add	v23.8h, v10.8h, v22.8h
        sub	v10.8h, v10.8h, v22.8h
        mls	v8.8h, v24.8h, v7.h[0]
        add	v22.8h, v23.8h, v11.8h
        mul	v24.8h, v10.8h, v1.h[2]
        sqrdmulh	v10.8h, v10.8h, v1.h[3]
        sub	v19.8h, v13.8h, v22.8h
        add	v18.8h, v13.8h, v22.8h
        sub	v11.8h, v23.8h, v11.8h
        mls	v24.8h, v10.8h, v7.h[0]
        mls	v26.8h, v16.8h, v7.h[0]
        sqrdmulh	v10.8h, v11.8h, v0.h[5]
        mul	v11.8h, v11.8h, v0.h[4]
        sqrdmulh	v22.8h, v19.8h, v0.h[1]
        sub	v13.8h, v24.8h, v26.8h
        mul	v16.8h, v19.8h, v0.h[0]
        mls	v11.8h, v10.8h, v7.h[0]
        sqrdmulh	v10.8h, v13.8h, v0.h[5]
        mul	v13.8h, v13.8h, v0.h[4]
        add	v24.8h, v24.8h, v26.8h
        sub	v26.8h, v2.8h, v11.8h
        add	v9.8h, v2.8h, v11.8h
        add	v11.8h, v14.8h, v24.8h
        sub	v14.8h, v14.8h, v24.8h
        sqrdmulh	v2.8h, v26.8h, v0.h[1]
        mul	v24.8h, v26.8h, v0.h[0]
        mls	v13.8h, v10.8h, v7.h[0]
        mls	v16.8h, v22.8h, v7.h[0]
        sqrdmulh	v10.8h, v14.8h, v0.h[1]
        mls	v24.8h, v2.8h, v7.h[0]
        add	v22.8h, v8.8h, v13.8h
        str	q16, [x0, #0x100]
        sub	v2.8h, v8.8h, v13.8h
        str	q24, [x0, #0x180]
        mul	v13.8h, v14.8h, v0.h[0]
        str	q22, [x0, #0xc0]
        sqrdmulh	v21.8h, v2.8h, v0.h[1]
        ldr	q6, [x0, #0x90]
        ldr	q14, [x0, #0xd0]
        mls	v13.8h, v10.8h, v7.h[0]
        str	q11, [x0, #0x40]
        sub	v10.8h, v6.8h, v14.8h
        ldr	q11, [x0, #0x10]
        sqrdmulh	v19.8h, v10.8h, v1.h[1]
        mul	v20.8h, v10.8h, v1.h[0]
        ldr	q28, [x0, #0x50]
        sub	x4, x4, #0x2

intt_layer123_start:
        mls	v20.8h, v19.8h, v7.h[0]
        ldr	q31, [x0, #0x1d0]
        sub	v22.8h, v11.8h, v28.8h
        ldr	q30, [x0, #0x110]
        sqrdmulh	v8.8h, v22.8h, v0.h[7]
        mul	v3.8h, v22.8h, v0.h[6]
        mul	v5.8h, v2.8h, v0.h[0]
        str	q13, [x0, #0x140]
        add	v10.8h, v11.8h, v28.8h
        ldr	q22, [x0, #0x150]
        ldr	q4, [x0, #0x190]
        sub	v23.8h, v30.8h, v22.8h
        add	v27.8h, v30.8h, v22.8h
        mls	v3.8h, v8.8h, v7.h[0]
        mls	v5.8h, v21.8h, v7.h[0]
        ldr	q11, [x0, #0x20]
        sub	v17.8h, v4.8h, v31.8h
        add	v2.8h, v6.8h, v14.8h
        mul	v19.8h, v23.8h, v1.h[2]
        sub	v22.8h, v3.8h, v20.8h
        add	v14.8h, v10.8h, v2.8h
        sub	v24.8h, v10.8h, v2.8h
        sqrdmulh	v2.8h, v23.8h, v1.h[3]
        sqrdmulh	v30.8h, v22.8h, v0.h[3]
        mul	v23.8h, v22.8h, v0.h[2]
        sqrdmulh	v15.8h, v17.8h, v1.h[5]
        mls	v19.8h, v2.8h, v7.h[0]
        add	v2.8h, v4.8h, v31.8h
        mul	v21.8h, v17.8h, v1.h[4]
        sqrdmulh	v22.8h, v24.8h, v0.h[3]
        sub	v26.8h, v27.8h, v2.8h
        add	v8.8h, v27.8h, v2.8h
        mul	v28.8h, v24.8h, v0.h[2]
        sqrdmulh	v10.8h, v26.8h, v0.h[5]
        mul	v31.8h, v26.8h, v0.h[4]
        mls	v21.8h, v15.8h, v7.h[0]
        mls	v28.8h, v22.8h, v7.h[0]
        sub	v17.8h, v14.8h, v8.8h
        mls	v31.8h, v10.8h, v7.h[0]
        sub	v27.8h, v19.8h, v21.8h
        sqrdmulh	v29.8h, v17.8h, v0.h[1]
        mul	v10.8h, v17.8h, v0.h[0]
        sub	v15.8h, v28.8h, v31.8h
        sqrdmulh	v17.8h, v27.8h, v0.h[5]
        mul	v25.8h, v27.8h, v0.h[4]
        sqrdmulh	v6.8h, v15.8h, v0.h[1]
        mul	v27.8h, v15.8h, v0.h[0]
        add	v16.8h, v19.8h, v21.8h
        mls	v25.8h, v17.8h, v7.h[0]
        mls	v23.8h, v30.8h, v7.h[0]
        mls	v27.8h, v6.8h, v7.h[0]
        ldr	q6, [x0, #0xa0]
        add	v22.8h, v23.8h, v25.8h
        str	q27, [x0, #0x190]
        add	v4.8h, v3.8h, v20.8h
        str	q22, [x0, #0xd0]
        mls	v10.8h, v29.8h, v7.h[0]
        str	q5, [x0, #0x1c0]
        add	v20.8h, v4.8h, v16.8h
        str	q18, [x0], #0x10
        sub	v18.8h, v4.8h, v16.8h
        str	q10, [x0, #0x100]
        sub	v2.8h, v23.8h, v25.8h
        sqrdmulh	v12.8h, v18.8h, v0.h[1]
        mul	v13.8h, v18.8h, v0.h[0]
        add	v18.8h, v14.8h, v8.8h
        ldr	q14, [x0, #0xd0]
        mls	v13.8h, v12.8h, v7.h[0]
        str	q9, [x0, #0x70]
        sub	v3.8h, v6.8h, v14.8h
        add	v9.8h, v28.8h, v31.8h
        str	q20, [x0, #0x40]
        sqrdmulh	v19.8h, v3.8h, v1.h[1]
        mul	v20.8h, v3.8h, v1.h[0]
        sqrdmulh	v21.8h, v2.8h, v0.h[1]
        ldr	q28, [x0, #0x50]
        subs	x4, x4, #0x1
        cbnz	x4, intt_layer123_start
        mls	v20.8h, v19.8h, v7.h[0]
        sub	v10.8h, v11.8h, v28.8h
        add	v11.8h, v11.8h, v28.8h
        mul	v2.8h, v2.8h, v0.h[0]
        str	q13, [x0, #0x140]
        add	v25.8h, v6.8h, v14.8h
        str	q18, [x0], #0x10
        sqrdmulh	v17.8h, v10.8h, v0.h[7]
        str	q9, [x0, #0x70]
        ldr	q8, [x0, #0x1c0]
        ldr	q13, [x0, #0x100]
        ldr	q26, [x0, #0x180]
        ldr	q24, [x0, #0x140]
        add	v15.8h, v26.8h, v8.8h
        sub	v8.8h, v26.8h, v8.8h
        sub	v12.8h, v13.8h, v24.8h
        add	v24.8h, v13.8h, v24.8h
        sqrdmulh	v18.8h, v8.8h, v1.h[5]
        mul	v26.8h, v12.8h, v1.h[2]
        mul	v8.8h, v8.8h, v1.h[4]
        sqrdmulh	v16.8h, v12.8h, v1.h[3]
        mul	v10.8h, v10.8h, v0.h[6]
        add	v22.8h, v11.8h, v25.8h
        mls	v8.8h, v18.8h, v7.h[0]
        mls	v26.8h, v16.8h, v7.h[0]
        mls	v10.8h, v17.8h, v7.h[0]
        add	v23.8h, v24.8h, v15.8h
        sub	v11.8h, v11.8h, v25.8h
        sub	v3.8h, v26.8h, v8.8h
        sub	v14.8h, v10.8h, v20.8h
        sub	v19.8h, v22.8h, v23.8h
        mul	v18.8h, v3.8h, v0.h[4]
        sqrdmulh	v17.8h, v14.8h, v0.h[3]
        mul	v14.8h, v14.8h, v0.h[2]
        sqrdmulh	v3.8h, v3.8h, v0.h[5]
        sub	v16.8h, v24.8h, v15.8h
        mls	v2.8h, v21.8h, v7.h[0]
        mls	v14.8h, v17.8h, v7.h[0]
        mls	v18.8h, v3.8h, v7.h[0]
        sqrdmulh	v31.8h, v16.8h, v0.h[5]
        str	q2, [x0, #0x1b0]
        mul	v13.8h, v16.8h, v0.h[4]
        add	v24.8h, v14.8h, v18.8h
        sqrdmulh	v2.8h, v11.8h, v0.h[3]
        mul	v21.8h, v11.8h, v0.h[2]
        mls	v13.8h, v31.8h, v7.h[0]
        add	v16.8h, v26.8h, v8.8h
        add	v28.8h, v10.8h, v20.8h
        mls	v21.8h, v2.8h, v7.h[0]
        sub	v14.8h, v14.8h, v18.8h
        add	v2.8h, v28.8h, v16.8h
        sub	v10.8h, v28.8h, v16.8h
        sub	v16.8h, v21.8h, v13.8h
        sqrdmulh	v27.8h, v19.8h, v0.h[1]
        mul	v26.8h, v19.8h, v0.h[0]
        sqrdmulh	v19.8h, v16.8h, v0.h[1]
        mul	v28.8h, v16.8h, v0.h[0]
        sqrdmulh	v8.8h, v14.8h, v0.h[1]
        mls	v26.8h, v27.8h, v7.h[0]
        mul	v14.8h, v14.8h, v0.h[0]
        mls	v28.8h, v19.8h, v7.h[0]
        sqrdmulh	v20.8h, v10.8h, v0.h[1]
        str	q26, [x0, #0x100]
        mul	v10.8h, v10.8h, v0.h[0]
        str	q28, [x0, #0x180]
        add	v22.8h, v22.8h, v23.8h
        str	q24, [x0, #0xc0]
        mls	v10.8h, v20.8h, v7.h[0]
        str	q2, [x0, #0x40]
        mls	v14.8h, v8.8h, v7.h[0]
        str	q22, [x0], #0x10
        add	v11.8h, v21.8h, v13.8h
        str	q10, [x0, #0x130]
        str	q11, [x0, #0x70]
        str	q14, [x0, #0x1b0]
        ldp	d8, d9, [sp]
        ldp	d10, d11, [sp, #0x10]
        ldp	d12, d13, [sp, #0x20]
        ldp	d14, d15, [sp, #0x30]
        add	sp, sp, #0x40
        ret

#endif /* MLK_ARITH_BACKEND_AARCH64 && !MLK_CONFIG_MULTILEVEL_NO_SHARED */
