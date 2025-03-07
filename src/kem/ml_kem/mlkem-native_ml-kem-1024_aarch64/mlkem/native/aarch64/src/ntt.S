///
/// Copyright (c) 2022 Arm Limited
/// Copyright (c) 2022 Hanno Becker
/// Copyright (c) 2023 Amin Abdulrahman, Matthias Kannwischer
/// Copyright (c) 2024-2025 The mlkem-native project authors
// SPDX-License-Identifier: MIT
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
        ldr	q21, [x0]
        ldr	q26, [x0, #0x40]
        ldr	q29, [x0, #0x80]
        ldr	q20, [x0, #0xc0]
        ldr	q23, [x0, #0x100]
        ldr	q11, [x0, #0x1c0]
        mul	v2.8h, v23.8h, v0.h[0]
        ldr	q17, [x0, #0x140]
        mul	v15.8h, v11.8h, v0.h[0]
        ldr	q13, [x0, #0x180]
        sub	x4, x4, #0x1

ntt_loop0:
        sqrdmulh	v14.8h, v23.8h, v0.h[1]
        sqrdmulh	v23.8h, v17.8h, v0.h[1]
        mul	v17.8h, v17.8h, v0.h[0]
        sqrdmulh	v28.8h, v13.8h, v0.h[1]
        mls	v2.8h, v14.8h, v7.h[0]
        mul	v14.8h, v13.8h, v0.h[0]
        mls	v17.8h, v23.8h, v7.h[0]
        sqrdmulh	v23.8h, v11.8h, v0.h[1]
        sub	v11.8h, v21.8h, v2.8h
        mls	v14.8h, v28.8h, v7.h[0]
        sub	v28.8h, v26.8h, v17.8h
        add	v17.8h, v26.8h, v17.8h
        add	v2.8h, v21.8h, v2.8h
        sub	v13.8h, v29.8h, v14.8h
        add	v14.8h, v29.8h, v14.8h
        mls	v15.8h, v23.8h, v7.h[0]
        sqrdmulh	v23.8h, v13.8h, v0.h[5]
        mul	v13.8h, v13.8h, v0.h[4]
        sqrdmulh	v21.8h, v14.8h, v0.h[3]
        sub	v26.8h, v20.8h, v15.8h
        add	v15.8h, v20.8h, v15.8h
        mls	v13.8h, v23.8h, v7.h[0]
        sqrdmulh	v23.8h, v26.8h, v0.h[5]
        mul	v26.8h, v26.8h, v0.h[4]
        mul	v14.8h, v14.8h, v0.h[2]
        sub	v29.8h, v11.8h, v13.8h
        add	v11.8h, v11.8h, v13.8h
        mls	v26.8h, v23.8h, v7.h[0]
        sqrdmulh	v23.8h, v15.8h, v0.h[3]
        mul	v13.8h, v15.8h, v0.h[2]
        mls	v14.8h, v21.8h, v7.h[0]
        sub	v15.8h, v28.8h, v26.8h
        add	v28.8h, v28.8h, v26.8h
        mls	v13.8h, v23.8h, v7.h[0]
        sub	v23.8h, v2.8h, v14.8h
        add	v14.8h, v2.8h, v14.8h
        sqrdmulh	v2.8h, v28.8h, v1.h[3]
        sub	v21.8h, v17.8h, v13.8h
        add	v17.8h, v17.8h, v13.8h
        mul	v28.8h, v28.8h, v1.h[2]
        sqrdmulh	v13.8h, v21.8h, v1.h[1]
        sqrdmulh	v26.8h, v17.8h, v0.h[7]
        mul	v17.8h, v17.8h, v0.h[6]
        mul	v21.8h, v21.8h, v1.h[0]
        mls	v28.8h, v2.8h, v7.h[0]
        sqrdmulh	v2.8h, v15.8h, v1.h[5]
        mls	v17.8h, v26.8h, v7.h[0]
        mls	v21.8h, v13.8h, v7.h[0]
        sub	v13.8h, v11.8h, v28.8h
        add	v28.8h, v11.8h, v28.8h
        sub	v11.8h, v14.8h, v17.8h
        mul	v15.8h, v15.8h, v1.h[4]
        add	v14.8h, v14.8h, v17.8h
        sub	v17.8h, v23.8h, v21.8h
        add	v23.8h, v23.8h, v21.8h
        mls	v15.8h, v2.8h, v7.h[0]
        str	q14, [x0], #0x10
        ldr	q21, [x0]
        sub	v14.8h, v29.8h, v15.8h
        add	v2.8h, v29.8h, v15.8h
        str	q11, [x0, #0x30]
        ldr	q26, [x0, #0x40]
        str	q23, [x0, #0x70]
        ldr	q29, [x0, #0x80]
        str	q17, [x0, #0xb0]
        ldr	q20, [x0, #0xc0]
        str	q28, [x0, #0xf0]
        ldr	q23, [x0, #0x100]
        str	q13, [x0, #0x130]
        ldr	q17, [x0, #0x140]
        str	q2, [x0, #0x170]
        mul	v2.8h, v23.8h, v0.h[0]
        str	q14, [x0, #0x1b0]
        ldr	q11, [x0, #0x1c0]
        ldr	q13, [x0, #0x180]
        mul	v15.8h, v11.8h, v0.h[0]
        sub	x4, x4, #0x1
        cbnz	x4, ntt_loop0
        sqrdmulh	v27.8h, v11.8h, v0.h[1]
        mul	v8.8h, v13.8h, v0.h[0]
        sqrdmulh	v22.8h, v13.8h, v0.h[1]
        mul	v11.8h, v17.8h, v0.h[0]
        mls	v15.8h, v27.8h, v7.h[0]
        sqrdmulh	v28.8h, v17.8h, v0.h[1]
        mls	v8.8h, v22.8h, v7.h[0]
        sqrdmulh	v5.8h, v23.8h, v0.h[1]
        add	v16.8h, v20.8h, v15.8h
        mls	v11.8h, v28.8h, v7.h[0]
        sub	v6.8h, v29.8h, v8.8h
        sqrdmulh	v17.8h, v16.8h, v0.h[3]
        mul	v23.8h, v16.8h, v0.h[2]
        mul	v13.8h, v6.8h, v0.h[4]
        sqrdmulh	v28.8h, v6.8h, v0.h[5]
        mls	v2.8h, v5.8h, v7.h[0]
        mls	v23.8h, v17.8h, v7.h[0]
        add	v27.8h, v26.8h, v11.8h
        mls	v13.8h, v28.8h, v7.h[0]
        sub	v9.8h, v21.8h, v2.8h
        add	v18.8h, v29.8h, v8.8h
        sub	v14.8h, v27.8h, v23.8h
        add	v29.8h, v9.8h, v13.8h
        sub	v30.8h, v9.8h, v13.8h
        mul	v28.8h, v14.8h, v1.h[0]
        sqrdmulh	v9.8h, v18.8h, v0.h[3]
        mul	v22.8h, v18.8h, v0.h[2]
        sqrdmulh	v17.8h, v14.8h, v1.h[1]
        sub	v14.8h, v20.8h, v15.8h
        add	v24.8h, v21.8h, v2.8h
        mls	v22.8h, v9.8h, v7.h[0]
        sqrdmulh	v9.8h, v14.8h, v0.h[5]
        mul	v13.8h, v14.8h, v0.h[4]
        mls	v28.8h, v17.8h, v7.h[0]
        sub	v5.8h, v24.8h, v22.8h
        sub	v2.8h, v26.8h, v11.8h
        mls	v13.8h, v9.8h, v7.h[0]
        sub	v17.8h, v5.8h, v28.8h
        add	v14.8h, v5.8h, v28.8h
        add	v28.8h, v27.8h, v23.8h
        str	q17, [x0, #0xc0]
        add	v17.8h, v2.8h, v13.8h
        str	q14, [x0, #0x80]
        sub	v13.8h, v2.8h, v13.8h
        sqrdmulh	v26.8h, v17.8h, v1.h[3]
        mul	v15.8h, v17.8h, v1.h[2]
        add	v5.8h, v24.8h, v22.8h
        sqrdmulh	v23.8h, v13.8h, v1.h[5]
        mul	v13.8h, v13.8h, v1.h[4]
        mls	v15.8h, v26.8h, v7.h[0]
        sqrdmulh	v14.8h, v28.8h, v0.h[7]
        mul	v17.8h, v28.8h, v0.h[6]
        mls	v13.8h, v23.8h, v7.h[0]
        add	v6.8h, v29.8h, v15.8h
        sub	v28.8h, v29.8h, v15.8h
        mls	v17.8h, v14.8h, v7.h[0]
        str	q6, [x0, #0x100]
        add	v14.8h, v30.8h, v13.8h
        str	q28, [x0, #0x140]
        sub	v23.8h, v30.8h, v13.8h
        str	q14, [x0, #0x180]
        add	v3.8h, v5.8h, v17.8h
        str	q23, [x0, #0x1c0]
        sub	v28.8h, v5.8h, v17.8h
        str	q3, [x0], #0x10
        str	q28, [x0, #0x30]
        mov	x0, x3
        mov	x4, #0x8                // =8
        ldr	q2, [x1], #0x10
        ldr	q14, [x0, #0x30]
        ldr	q1, [x0, #0x20]
        mul	v17.8h, v14.8h, v2.h[0]
        sqrdmulh	v14.8h, v14.8h, v2.h[1]
        mul	v8.8h, v1.8h, v2.h[0]
        ldr	q23, [x0, #0x10]
        mls	v17.8h, v14.8h, v7.h[0]
        sqrdmulh	v1.8h, v1.8h, v2.h[1]
        ldr	q30, [x2], #0x60
        sub	v14.8h, v23.8h, v17.8h
        add	v10.8h, v23.8h, v17.8h
        mls	v8.8h, v1.8h, v7.h[0]
        sqrdmulh	v1.8h, v14.8h, v2.h[5]
        mul	v14.8h, v14.8h, v2.h[4]
        ldr	q27, [x0]
        mul	v23.8h, v10.8h, v2.h[2]
        mls	v14.8h, v1.8h, v7.h[0]
        sub	v1.8h, v27.8h, v8.8h
        ldur	q28, [x2, #-0x40]
        add	v12.8h, v1.8h, v14.8h
        sqrdmulh	v21.8h, v10.8h, v2.h[3]
        sub	v5.8h, v1.8h, v14.8h
        ldur	q13, [x2, #-0x10]
        sub	x4, x4, #0x1

ntt_loop1:
        ldr	q19, [x0, #0x70]
        ldr	q1, [x1], #0x10
        mls	v23.8h, v21.8h, v7.h[0]
        add	v6.8h, v27.8h, v8.8h
        mul	v4.8h, v19.8h, v1.h[0]
        sqrdmulh	v19.8h, v19.8h, v1.h[1]
        ldr	q25, [x0, #0x50]
        trn1	v11.4s, v12.4s, v5.4s
        mls	v4.8h, v19.8h, v7.h[0]
        sub	v0.8h, v6.8h, v23.8h
        ldur	q16, [x2, #-0x50]
        sub	v24.8h, v25.8h, v4.8h
        add	v26.8h, v6.8h, v23.8h
        add	v4.8h, v25.8h, v4.8h
        sqrdmulh	v23.8h, v24.8h, v1.h[5]
        mul	v20.8h, v24.8h, v1.h[4]
        sqrdmulh	v21.8h, v4.8h, v1.h[3]
        trn1	v27.4s, v26.4s, v0.4s
        trn2	v25.4s, v12.4s, v5.4s
        mls	v20.8h, v23.8h, v7.h[0]
        mul	v23.8h, v4.8h, v1.h[2]
        ldr	q31, [x0, #0x60]
        trn2	v12.4s, v26.4s, v0.4s
        trn2	v19.2d, v27.2d, v11.2d
        mul	v8.8h, v31.8h, v1.h[0]
        sqrdmulh	v1.8h, v31.8h, v1.h[1]
        trn2	v10.2d, v12.2d, v25.2d
        sqrdmulh	v0.8h, v19.8h, v16.8h
        sqrdmulh	v18.8h, v10.8h, v16.8h
        trn1	v16.2d, v27.2d, v11.2d
        trn1	v2.2d, v12.2d, v25.2d
        mul	v12.8h, v10.8h, v30.8h
        mul	v10.8h, v19.8h, v30.8h
        mls	v8.8h, v1.8h, v7.h[0]
        ldur	q14, [x2, #-0x30]
        mls	v10.8h, v0.8h, v7.h[0]
        mls	v12.8h, v18.8h, v7.h[0]
        ldr	q27, [x0, #0x40]
        add	v9.8h, v16.8h, v10.8h
        sub	v16.8h, v16.8h, v10.8h
        sub	v25.8h, v2.8h, v12.8h
        add	v30.8h, v2.8h, v12.8h
        sub	v10.8h, v27.8h, v8.8h
        sqrdmulh	v22.8h, v25.8h, v13.8h
        sqrdmulh	v13.8h, v30.8h, v14.8h
        ldur	q14, [x2, #-0x20]
        add	v12.8h, v10.8h, v20.8h
        mul	v5.8h, v30.8h, v28.8h
        mul	v26.8h, v25.8h, v14.8h
        ldr	q30, [x2], #0x60
        mls	v5.8h, v13.8h, v7.h[0]
        mls	v26.8h, v22.8h, v7.h[0]
        ldur	q28, [x2, #-0x40]
        add	v13.8h, v9.8h, v5.8h
        sub	v9.8h, v9.8h, v5.8h
        sub	v5.8h, v16.8h, v26.8h
        add	v25.8h, v16.8h, v26.8h
        trn1	v15.4s, v13.4s, v9.4s
        trn2	v3.4s, v13.4s, v9.4s
        trn1	v13.4s, v25.4s, v5.4s
        trn2	v31.4s, v25.4s, v5.4s
        sub	v5.8h, v10.8h, v20.8h
        trn1	v2.2d, v15.2d, v13.2d
        trn2	v9.2d, v15.2d, v13.2d
        str	q2, [x0], #0x40
        trn1	v29.2d, v3.2d, v31.2d
        stur	q9, [x0, #-0x20]
        trn2	v9.2d, v3.2d, v31.2d
        stur	q29, [x0, #-0x30]
        ldur	q13, [x2, #-0x10]
        stur	q9, [x0, #-0x10]
        sub	x4, x4, #0x1
        cbnz	x4, ntt_loop1
        mls	v23.8h, v21.8h, v7.h[0]
        add	v14.8h, v27.8h, v8.8h
        ldur	q1, [x2, #-0x20]
        add	v17.8h, v14.8h, v23.8h
        sub	v23.8h, v14.8h, v23.8h
        trn2	v11.4s, v12.4s, v5.4s
        trn1	v27.4s, v12.4s, v5.4s
        trn2	v2.4s, v17.4s, v23.4s
        ldur	q26, [x2, #-0x50]
        trn2	v14.2d, v2.2d, v11.2d
        trn1	v15.4s, v17.4s, v23.4s
        mul	v5.8h, v14.8h, v30.8h
        sqrdmulh	v23.8h, v14.8h, v26.8h
        trn2	v17.2d, v15.2d, v27.2d
        trn1	v14.2d, v2.2d, v11.2d
        mul	v21.8h, v17.8h, v30.8h
        mls	v5.8h, v23.8h, v7.h[0]
        sqrdmulh	v17.8h, v17.8h, v26.8h
        ldur	q2, [x2, #-0x30]
        sub	v23.8h, v14.8h, v5.8h
        add	v14.8h, v14.8h, v5.8h
        mls	v21.8h, v17.8h, v7.h[0]
        mul	v1.8h, v23.8h, v1.8h
        sqrdmulh	v17.8h, v23.8h, v13.8h
        mul	v23.8h, v14.8h, v28.8h
        sqrdmulh	v14.8h, v14.8h, v2.8h
        trn1	v28.2d, v15.2d, v27.2d
        mls	v1.8h, v17.8h, v7.h[0]
        sub	v11.8h, v28.8h, v21.8h
        mls	v23.8h, v14.8h, v7.h[0]
        add	v17.8h, v28.8h, v21.8h
        sub	v14.8h, v11.8h, v1.8h
        add	v1.8h, v11.8h, v1.8h
        sub	v28.8h, v17.8h, v23.8h
        add	v2.8h, v17.8h, v23.8h
        trn1	v23.4s, v1.4s, v14.4s
        trn2	v14.4s, v1.4s, v14.4s
        trn2	v17.4s, v2.4s, v28.4s
        trn1	v28.4s, v2.4s, v28.4s
        trn2	v1.2d, v17.2d, v14.2d
        trn1	v14.2d, v17.2d, v14.2d
        str	q1, [x0, #0x30]
        trn2	v1.2d, v28.2d, v23.2d
        str	q14, [x0, #0x10]
        trn1	v14.2d, v28.2d, v23.2d
        str	q1, [x0, #0x20]
        str	q14, [x0], #0x40
        ldp	d8, d9, [sp]
        ldp	d10, d11, [sp, #0x10]
        ldp	d12, d13, [sp, #0x20]
        ldp	d14, d15, [sp, #0x30]
        add	sp, sp, #0x40
        ret

#endif /* MLK_ARITH_BACKEND_AARCH64_OPT &&
          !MLK_MULTILEVEL_BUILD_NO_SHARED */
