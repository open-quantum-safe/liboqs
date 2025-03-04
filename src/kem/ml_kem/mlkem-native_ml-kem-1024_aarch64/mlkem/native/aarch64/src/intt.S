/// Copyright (c) 2024-2025 The mlkem-native project authors
/// Copyright (c) 2022 Arm Limited
/// Copyright (c) 2022 Hanno Becker
/// Copyright (c) 2023 Amin Abdulrahman, Matthias Kannwischer
/// SPDX-License-Identifier: MIT
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include "../../../common.h"
#if defined(MLK_ARITH_BACKEND_AARCH64_OPT) && \
    !defined(MLK_MULTILEVEL_BUILD_NO_SHARED)

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

scale_start:
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
        cbnz	x4, scale_start
        mov	x3, x0
        mov	x4, #0x8                // =8
        ldr	q26, [x3]
        ldr	q8, [x3, #0x10]
        ldr	q24, [x3, #0x20]
        ldr	q16, [x3, #0x30]
        ldr	q9, [x2], #0x60
        trn1	v0.4s, v24.4s, v16.4s
        ldur	q6, [x2, #-0x50]
        ldur	q3, [x2, #-0x40]
        ldur	q15, [x2, #-0x30]
        ldur	q4, [x2, #-0x20]
        ldur	q28, [x2, #-0x10]
        sub	x4, x4, #0x1

layer4567_start:
        trn1	v12.4s, v26.4s, v8.4s
        trn2	v26.4s, v26.4s, v8.4s
        trn2	v8.4s, v24.4s, v16.4s
        trn2	v11.2d, v12.2d, v0.2d
        trn1	v12.2d, v12.2d, v0.2d
        trn2	v16.2d, v26.2d, v8.2d
        trn1	v26.2d, v26.2d, v8.2d
        sub	v8.8h, v11.8h, v16.8h
        add	v11.8h, v11.8h, v16.8h
        sub	v16.8h, v12.8h, v26.8h
        add	v12.8h, v12.8h, v26.8h
        sqrdmulh	v26.8h, v8.8h, v28.8h
        sqrdmulh	v15.8h, v16.8h, v15.8h
        mul	v16.8h, v16.8h, v3.8h
        mul	v8.8h, v8.8h, v4.8h
        sub	v0.8h, v12.8h, v11.8h
        add	v12.8h, v12.8h, v11.8h
        mls	v16.8h, v15.8h, v7.h[0]
        mls	v8.8h, v26.8h, v7.h[0]
        sqrdmulh	v26.8h, v0.8h, v6.8h
        mul	v11.8h, v0.8h, v9.8h
        ldr	q15, [x1], #0x10
        sub	v0.8h, v16.8h, v8.8h
        mls	v11.8h, v26.8h, v7.h[0]
        add	v26.8h, v16.8h, v8.8h
        sqrdmulh	v8.8h, v0.8h, v6.8h
        mul	v16.8h, v0.8h, v9.8h
        trn1	v0.4s, v12.4s, v26.4s
        trn2	v12.4s, v12.4s, v26.4s
        ldr	q26, [x3, #0x40]
        mls	v16.8h, v8.8h, v7.h[0]
        ldr	q8, [x3, #0x50]
        ldr	q24, [x3, #0x60]
        trn1	v9.4s, v11.4s, v16.4s
        trn2	v11.4s, v11.4s, v16.4s
        ldr	q16, [x3, #0x70]
        trn2	v6.2d, v0.2d, v9.2d
        trn2	v3.2d, v12.2d, v11.2d
        trn1	v0.2d, v0.2d, v9.2d
        trn1	v12.2d, v12.2d, v11.2d
        sub	v11.8h, v6.8h, v3.8h
        sub	v9.8h, v0.8h, v12.8h
        add	v12.8h, v0.8h, v12.8h
        sqrdmulh	v0.8h, v11.8h, v15.h[5]
        sqrdmulh	v4.8h, v9.8h, v15.h[3]
        mul	v9.8h, v9.8h, v15.h[2]
        mul	v11.8h, v11.8h, v15.h[4]
        add	v6.8h, v6.8h, v3.8h
        sqdmulh	v3.8h, v12.8h, v7.h[1]
        mls	v9.8h, v4.8h, v7.h[0]
        mls	v11.8h, v0.8h, v7.h[0]
        sqdmulh	v0.8h, v6.8h, v7.h[1]
        srshr	v3.8h, v3.8h, #0xb
        sqdmulh	v4.8h, v9.8h, v7.h[1]
        sqdmulh	v28.8h, v11.8h, v7.h[1]
        mls	v12.8h, v3.8h, v7.h[0]
        srshr	v0.8h, v0.8h, #0xb
        srshr	v3.8h, v4.8h, #0xb
        srshr	v4.8h, v28.8h, #0xb
        mls	v6.8h, v0.8h, v7.h[0]
        mls	v9.8h, v3.8h, v7.h[0]
        mls	v11.8h, v4.8h, v7.h[0]
        trn1	v0.4s, v24.4s, v16.4s
        sub	v3.8h, v12.8h, v6.8h
        add	v12.8h, v12.8h, v6.8h
        sub	v6.8h, v9.8h, v11.8h
        sqrdmulh	v4.8h, v3.8h, v15.h[1]
        mul	v3.8h, v3.8h, v15.h[0]
        sqrdmulh	v28.8h, v6.8h, v15.h[1]
        mul	v15.8h, v6.8h, v15.h[0]
        add	v11.8h, v9.8h, v11.8h
        mls	v3.8h, v4.8h, v7.h[0]
        str	q12, [x3], #0x40
        mls	v15.8h, v28.8h, v7.h[0]
        stur	q11, [x3, #-0x30]
        ldr	q9, [x2], #0x60
        stur	q3, [x3, #-0x20]
        ldur	q6, [x2, #-0x50]
        stur	q15, [x3, #-0x10]
        ldur	q3, [x2, #-0x40]
        ldur	q15, [x2, #-0x30]
        ldur	q4, [x2, #-0x20]
        ldur	q28, [x2, #-0x10]
        sub	x4, x4, #0x1
        cbnz	x4, layer4567_start
        trn1	v11.4s, v26.4s, v8.4s
        trn2	v24.4s, v24.4s, v16.4s
        trn2	v26.4s, v26.4s, v8.4s
        trn1	v18.2d, v11.2d, v0.2d
        trn2	v11.2d, v11.2d, v0.2d
        trn2	v12.2d, v26.2d, v24.2d
        trn1	v8.2d, v26.2d, v24.2d
        sub	v26.8h, v11.8h, v12.8h
        sub	v13.8h, v18.8h, v8.8h
        add	v24.8h, v18.8h, v8.8h
        mul	v16.8h, v26.8h, v4.8h
        sqrdmulh	v17.8h, v13.8h, v15.8h
        mul	v3.8h, v13.8h, v3.8h
        sqrdmulh	v26.8h, v26.8h, v28.8h
        add	v10.8h, v11.8h, v12.8h
        mls	v3.8h, v17.8h, v7.h[0]
        mls	v16.8h, v26.8h, v7.h[0]
        sub	v26.8h, v24.8h, v10.8h
        ldr	q4, [x1], #0x10
        sub	v12.8h, v3.8h, v16.8h
        sqrdmulh	v15.8h, v26.8h, v6.8h
        mul	v11.8h, v26.8h, v9.8h
        mul	v8.8h, v12.8h, v9.8h
        sqrdmulh	v12.8h, v12.8h, v6.8h
        add	v0.8h, v24.8h, v10.8h
        mls	v11.8h, v15.8h, v7.h[0]
        add	v6.8h, v3.8h, v16.8h
        mls	v8.8h, v12.8h, v7.h[0]
        trn2	v26.4s, v0.4s, v6.4s
        trn2	v12.4s, v11.4s, v8.4s
        trn1	v3.4s, v11.4s, v8.4s
        trn1	v17.4s, v0.4s, v6.4s
        trn1	v8.2d, v26.2d, v12.2d
        trn2	v13.2d, v26.2d, v12.2d
        trn1	v11.2d, v17.2d, v3.2d
        trn2	v15.2d, v17.2d, v3.2d
        sub	v12.8h, v11.8h, v8.8h
        add	v16.8h, v15.8h, v13.8h
        sub	v26.8h, v15.8h, v13.8h
        mul	v0.8h, v12.8h, v4.h[2]
        sqrdmulh	v9.8h, v12.8h, v4.h[3]
        mul	v13.8h, v26.8h, v4.h[4]
        sqrdmulh	v26.8h, v26.8h, v4.h[5]
        add	v24.8h, v11.8h, v8.8h
        mls	v0.8h, v9.8h, v7.h[0]
        sqdmulh	v12.8h, v16.8h, v7.h[1]
        mls	v13.8h, v26.8h, v7.h[0]
        sqdmulh	v11.8h, v24.8h, v7.h[1]
        sqdmulh	v8.8h, v0.8h, v7.h[1]
        srshr	v12.8h, v12.8h, #0xb
        sqdmulh	v26.8h, v13.8h, v7.h[1]
        srshr	v11.8h, v11.8h, #0xb
        mls	v16.8h, v12.8h, v7.h[0]
        srshr	v8.8h, v8.8h, #0xb
        srshr	v26.8h, v26.8h, #0xb
        mls	v24.8h, v11.8h, v7.h[0]
        mls	v0.8h, v8.8h, v7.h[0]
        mls	v13.8h, v26.8h, v7.h[0]
        sub	v26.8h, v24.8h, v16.8h
        add	v15.8h, v24.8h, v16.8h
        sub	v12.8h, v0.8h, v13.8h
        mul	v11.8h, v26.8h, v4.h[0]
        sqrdmulh	v16.8h, v26.8h, v4.h[1]
        mul	v26.8h, v12.8h, v4.h[0]
        sqrdmulh	v8.8h, v12.8h, v4.h[1]
        add	v12.8h, v0.8h, v13.8h
        mls	v11.8h, v16.8h, v7.h[0]
        str	q15, [x3], #0x40
        mls	v26.8h, v8.8h, v7.h[0]
        stur	q12, [x3, #-0x30]
        stur	q11, [x3, #-0x20]
        stur	q26, [x3, #-0x10]
        mov	x4, #0x4                // =4
        ldr	q0, [x1], #0x20
        ldur	q1, [x1, #-0x10]
        ldr	q24, [x0, #0x80]
        ldr	q16, [x0, #0xc0]
        ldr	q9, [x0, #0x100]
        ldr	q6, [x0, #0x140]
        ldr	q3, [x0, #0x180]
        ldr	q4, [x0, #0x1c0]
        add	v28.8h, v9.8h, v6.8h
        add	v19.8h, v24.8h, v16.8h
        add	v13.8h, v3.8h, v4.8h
        ldr	q11, [x0]
        add	v23.8h, v28.8h, v13.8h
        ldr	q15, [x0, #0x40]
        sub	x4, x4, #0x1

layer123_start:
        sub	v12.8h, v11.8h, v15.8h
        add	v26.8h, v11.8h, v15.8h
        sub	v8.8h, v24.8h, v16.8h
        sqrdmulh	v11.8h, v12.8h, v0.h[7]
        mul	v12.8h, v12.8h, v0.h[6]
        sub	v16.8h, v26.8h, v19.8h
        add	v26.8h, v26.8h, v19.8h
        sqrdmulh	v15.8h, v8.8h, v1.h[1]
        mul	v8.8h, v8.8h, v1.h[0]
        mls	v12.8h, v11.8h, v7.h[0]
        sub	v11.8h, v9.8h, v6.8h
        sqrdmulh	v24.8h, v16.8h, v0.h[3]
        mul	v16.8h, v16.8h, v0.h[2]
        sub	v9.8h, v26.8h, v23.8h
        add	v26.8h, v26.8h, v23.8h
        mls	v8.8h, v15.8h, v7.h[0]
        sqrdmulh	v15.8h, v11.8h, v1.h[3]
        mul	v11.8h, v11.8h, v1.h[2]
        sub	v6.8h, v3.8h, v4.8h
        sub	v3.8h, v12.8h, v8.8h
        add	v12.8h, v12.8h, v8.8h
        mls	v11.8h, v15.8h, v7.h[0]
        sqrdmulh	v8.8h, v6.8h, v1.h[5]
        mls	v16.8h, v24.8h, v7.h[0]
        mul	v15.8h, v6.8h, v1.h[4]
        sqrdmulh	v24.8h, v3.8h, v0.h[3]
        mul	v6.8h, v3.8h, v0.h[2]
        sqrdmulh	v3.8h, v9.8h, v0.h[1]
        mul	v9.8h, v9.8h, v0.h[0]
        str	q26, [x0], #0x10
        mls	v15.8h, v8.8h, v7.h[0]
        mls	v6.8h, v24.8h, v7.h[0]
        sub	v26.8h, v28.8h, v13.8h
        mls	v9.8h, v3.8h, v7.h[0]
        sub	v8.8h, v11.8h, v15.8h
        sqrdmulh	v24.8h, v26.8h, v0.h[5]
        mul	v26.8h, v26.8h, v0.h[4]
        add	v11.8h, v11.8h, v15.8h
        sqrdmulh	v15.8h, v8.8h, v0.h[5]
        mul	v8.8h, v8.8h, v0.h[4]
        mls	v26.8h, v24.8h, v7.h[0]
        sub	v24.8h, v12.8h, v11.8h
        add	v12.8h, v12.8h, v11.8h
        mls	v8.8h, v15.8h, v7.h[0]
        sqrdmulh	v11.8h, v24.8h, v0.h[1]
        mul	v15.8h, v24.8h, v0.h[0]
        sub	v24.8h, v16.8h, v26.8h
        add	v26.8h, v16.8h, v26.8h
        sub	v16.8h, v6.8h, v8.8h
        mls	v15.8h, v11.8h, v7.h[0]
        sqrdmulh	v11.8h, v24.8h, v0.h[1]
        mul	v24.8h, v24.8h, v0.h[0]
        add	v8.8h, v6.8h, v8.8h
        sqrdmulh	v6.8h, v16.8h, v0.h[1]
        mul	v16.8h, v16.8h, v0.h[0]
        mls	v24.8h, v11.8h, v7.h[0]
        str	q9, [x0, #0xf0]
        ldr	q11, [x0]
        mls	v16.8h, v6.8h, v7.h[0]
        str	q15, [x0, #0x130]
        ldr	q15, [x0, #0x40]
        str	q24, [x0, #0x170]
        ldr	q24, [x0, #0x80]
        str	q16, [x0, #0x1b0]
        ldr	q16, [x0, #0xc0]
        str	q12, [x0, #0x30]
        ldr	q9, [x0, #0x100]
        ldr	q6, [x0, #0x140]
        ldr	q3, [x0, #0x180]
        ldr	q4, [x0, #0x1c0]
        str	q26, [x0, #0x70]
        add	v28.8h, v9.8h, v6.8h
        add	v13.8h, v3.8h, v4.8h
        str	q8, [x0, #0xb0]
        add	v19.8h, v24.8h, v16.8h
        add	v23.8h, v28.8h, v13.8h
        sub	x4, x4, #0x1
        cbnz	x4, layer123_start
        add	v10.8h, v11.8h, v15.8h
        sub	v12.8h, v28.8h, v13.8h
        sub	v11.8h, v11.8h, v15.8h
        sub	v22.8h, v10.8h, v19.8h
        mul	v18.8h, v12.8h, v0.h[4]
        sqrdmulh	v26.8h, v12.8h, v0.h[5]
        sqrdmulh	v12.8h, v22.8h, v0.h[3]
        mul	v13.8h, v22.8h, v0.h[2]
        sub	v31.8h, v24.8h, v16.8h
        sqrdmulh	v22.8h, v11.8h, v0.h[7]
        mls	v18.8h, v26.8h, v7.h[0]
        mls	v13.8h, v12.8h, v7.h[0]
        sqrdmulh	v2.8h, v31.8h, v1.h[1]
        mul	v5.8h, v31.8h, v1.h[0]
        mul	v15.8h, v11.8h, v0.h[6]
        sub	v12.8h, v13.8h, v18.8h
        sub	v4.8h, v3.8h, v4.8h
        mls	v5.8h, v2.8h, v7.h[0]
        sqrdmulh	v26.8h, v12.8h, v0.h[1]
        mul	v12.8h, v12.8h, v0.h[0]
        mls	v15.8h, v22.8h, v7.h[0]
        sqrdmulh	v8.8h, v4.8h, v1.h[5]
        mul	v4.8h, v4.8h, v1.h[4]
        mls	v12.8h, v26.8h, v7.h[0]
        sub	v21.8h, v15.8h, v5.8h
        sub	v28.8h, v9.8h, v6.8h
        mls	v4.8h, v8.8h, v7.h[0]
        mul	v24.8h, v21.8h, v0.h[2]
        sqrdmulh	v8.8h, v21.8h, v0.h[3]
        sqrdmulh	v6.8h, v28.8h, v1.h[3]
        add	v19.8h, v10.8h, v19.8h
        mul	v28.8h, v28.8h, v1.h[2]
        mls	v24.8h, v8.8h, v7.h[0]
        sub	v11.8h, v19.8h, v23.8h
        str	q12, [x0, #0x180]
        mls	v28.8h, v6.8h, v7.h[0]
        sqrdmulh	v16.8h, v11.8h, v0.h[1]
        mul	v9.8h, v11.8h, v0.h[0]
        add	v6.8h, v15.8h, v5.8h
        add	v26.8h, v28.8h, v4.8h
        sub	v15.8h, v28.8h, v4.8h
        mls	v9.8h, v16.8h, v7.h[0]
        add	v3.8h, v6.8h, v26.8h
        mul	v8.8h, v15.8h, v0.h[4]
        sqrdmulh	v15.8h, v15.8h, v0.h[5]
        str	q9, [x0, #0x100]
        sub	v2.8h, v6.8h, v26.8h
        str	q3, [x0, #0x40]
        mls	v8.8h, v15.8h, v7.h[0]
        sqrdmulh	v15.8h, v2.8h, v0.h[1]
        mul	v11.8h, v2.8h, v0.h[0]
        add	v16.8h, v13.8h, v18.8h
        sub	v12.8h, v24.8h, v8.8h
        add	v8.8h, v24.8h, v8.8h
        mls	v11.8h, v15.8h, v7.h[0]
        sqrdmulh	v26.8h, v12.8h, v0.h[1]
        mul	v12.8h, v12.8h, v0.h[0]
        str	q8, [x0, #0xc0]
        add	v15.8h, v19.8h, v23.8h
        str	q11, [x0, #0x140]
        mls	v12.8h, v26.8h, v7.h[0]
        str	q15, [x0], #0x10
        str	q16, [x0, #0x70]
        str	q12, [x0, #0x1b0]
        ldp	d8, d9, [sp]
        ldp	d10, d11, [sp, #0x10]
        ldp	d12, d13, [sp, #0x20]
        ldp	d14, d15, [sp, #0x30]
        add	sp, sp, #0x40
        ret

#endif /* MLK_ARITH_BACKEND_AARCH64_OPT &&
          !MLK_MULTILEVEL_BUILD_NO_SHARED */
