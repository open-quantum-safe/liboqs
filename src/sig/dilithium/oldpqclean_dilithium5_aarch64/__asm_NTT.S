
/*
 * CC0 1.0 Universal or the following MIT License
 *
 * MIT License
 *
 * Copyright (c) 2023: Hanno Becker, Vincent Hwang, Matthias J. Kannwischer, Bo-Yin Yang, and Shang-Yi Yang
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "macros.inc"

.align 2
.global PQCLEAN_DILITHIUM5_AARCH64__asm_ntt_SIMD_top
.global _PQCLEAN_DILITHIUM5_AARCH64__asm_ntt_SIMD_top
PQCLEAN_DILITHIUM5_AARCH64__asm_ntt_SIMD_top:
_PQCLEAN_DILITHIUM5_AARCH64__asm_ntt_SIMD_top:

    push_all
    Q         .req w20
    src0      .req x0
    src1      .req x1
    src2      .req x2
    src3      .req x3
    src4      .req x4
    src5      .req x5
    src6      .req x6
    src7      .req x7
    src8      .req x8
    src9      .req x9
    src10     .req x10
    src11     .req x11
    src12     .req x12
    src13     .req x13
    src14     .req x14
    src15     .req x15
    table     .req x28
    counter   .req x19

    ldr Q, [x2]

    mov table, x1

    add  src1, src0, #64
    add  src2, src0, #128

    add  src3, src0, #192
    add  src4, src0, #256

    add  src5, src0, #320
    add  src6, src0, #384

    add  src7, src0, #448
    add  src8, src0, #512

    add  src9, src0, #576
    add src10, src0, #640

    add src11, src0, #704
    add src12, src0, #768

    add src13, src0, #832
    add src14, src0, #896

    add src15, src0, #960

    ld1 {v20.4S, v21.4S, v22.4S, v23.4S}, [table], #64
    ld1 {v24.4S, v25.4S, v26.4S, v27.4S}, [table], #64

    mov v20.S[0], Q

    ld1 { v1.4S}, [ src1]
    ld1 { v3.4S}, [ src3]
    ld1 { v5.4S}, [ src5]
    ld1 { v7.4S}, [ src7]
    ld1 { v9.4S}, [ src9]
    ld1 {v11.4S}, [src11]
    ld1 {v13.4S}, [src13]
    ld1 {v15.4S}, [src15]

    ld1 { v0.4S}, [ src0]
    ld1 { v2.4S}, [ src2]
    ld1 { v4.4S}, [ src4]
    ld1 { v6.4S}, [ src6]
    ld1 { v8.4S}, [ src8]
    ld1 {v10.4S}, [src10]
    ld1 {v12.4S}, [src12]
    ld1 {v14.4S}, [src14]

    qq_butterfly_top  v1,  v3,  v5,  v7,  v9, v11, v13, v15, v16, v17, v18, v19, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3
    qq_butterfly_mixed  v1,  v3,  v5,  v7,  v9, v11, v13, v15, v16, v17, v18, v19,  v0,  v2,  v4,  v6,  v8, v10, v12, v14, v28, v29, v30, v31, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3
    qq_butterfly_mixed  v0,  v2,  v4,  v6,  v8, v10, v12, v14, v28, v29, v30, v31,  v1,  v3,  v9, v11,  v5,  v7, v13, v15, v16, v17, v18, v19, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3
    qq_butterfly_mixed  v1,  v3,  v9, v11,  v5,  v7, v13, v15, v16, v17, v18, v19,  v0,  v2,  v8, v10,  v4,  v6, v12, v14, v28, v29, v30, v31, v20, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3
    qq_butterfly_mixed  v0,  v2,  v8, v10,  v4,  v6, v12, v14, v28, v29, v30, v31,  v1,  v5,  v9, v13,  v3,  v7, v11, v15, v16, v17, v18, v19, v20, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3
    qq_butterfly_mixed  v1,  v5,  v9, v13,  v3,  v7, v11, v15, v16, v17, v18, v19,  v0,  v4,  v8, v12,  v2,  v6, v10, v14, v28, v29, v30, v31, v20, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3
    qq_butterfly_mixed  v0,  v4,  v8, v12,  v2,  v6, v10, v14, v28, v29, v30, v31,  v0,  v2,  v4,  v6,  v1,  v3,  v5,  v7, v16, v17, v18, v19, v20, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3, v24, 0, 1, v24, 2, 3, v25, 0, 1, v25, 2, 3
    qq_butterfly_mixed  v0,  v2,  v4,  v6,  v1,  v3,  v5,  v7, v16, v17, v18, v19,  v8, v10, v12, v14,  v9, v11, v13, v15, v28, v29, v30, v31, v20,  v24, 0, 1, v24, 2, 3, v25, 0, 1, v25, 2, 3, v26, 0, 1, v26, 2, 3, v27, 0, 1, v27, 2, 3
    qq_butterfly_bot  v8, v10, v12, v14,  v9, v11, v13, v15, v28, v29, v30, v31, v20, v26, 0, 1, v26, 2, 3, v27, 0, 1, v27, 2, 3

    mov counter, #3
    _ntt_top_loop:

    st1 { v1.4S}, [ src1], #16
    ld1 { v1.4S}, [ src1]
    st1 { v3.4S}, [ src3], #16
    ld1 { v3.4S}, [ src3]
    st1 { v5.4S}, [ src5], #16
    ld1 { v5.4S}, [ src5]
    st1 { v7.4S}, [ src7], #16
    ld1 { v7.4S}, [ src7]
    st1 { v9.4S}, [ src9], #16
    ld1 { v9.4S}, [ src9]
    st1 {v11.4S}, [src11], #16
    ld1 {v11.4S}, [src11]
    st1 {v13.4S}, [src13], #16
    ld1 {v13.4S}, [src13]
    st1 {v15.4S}, [src15], #16
    ld1 {v15.4S}, [src15]

    st1 { v0.4S}, [ src0], #16
    ld1 { v0.4S}, [ src0]
    st1 { v2.4S}, [ src2], #16
    ld1 { v2.4S}, [ src2]
    st1 { v4.4S}, [ src4], #16
    ld1 { v4.4S}, [ src4]
    st1 { v6.4S}, [ src6], #16
    ld1 { v6.4S}, [ src6]
    st1 { v8.4S}, [ src8], #16
    ld1 { v8.4S}, [ src8]
    st1 {v10.4S}, [src10], #16
    ld1 {v10.4S}, [src10]
    st1 {v12.4S}, [src12], #16
    ld1 {v12.4S}, [src12]
    st1 {v14.4S}, [src14], #16
    ld1 {v14.4S}, [src14]

    qq_butterfly_top  v1,  v3,  v5,  v7,  v9, v11, v13, v15, v16, v17, v18, v19, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3
    qq_butterfly_mixed  v1,  v3,  v5,  v7,  v9, v11, v13, v15, v16, v17, v18, v19,  v0,  v2,  v4,  v6,  v8, v10, v12, v14, v28, v29, v30, v31, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3
    qq_butterfly_mixed  v0,  v2,  v4,  v6,  v8, v10, v12, v14, v28, v29, v30, v31,  v1,  v3,  v9, v11,  v5,  v7, v13, v15, v16, v17, v18, v19, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3
    qq_butterfly_mixed  v1,  v3,  v9, v11,  v5,  v7, v13, v15, v16, v17, v18, v19,  v0,  v2,  v8, v10,  v4,  v6, v12, v14, v28, v29, v30, v31, v20, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3
    qq_butterfly_mixed  v0,  v2,  v8, v10,  v4,  v6, v12, v14, v28, v29, v30, v31,  v1,  v5,  v9, v13,  v3,  v7, v11, v15, v16, v17, v18, v19, v20, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3
    qq_butterfly_mixed  v1,  v5,  v9, v13,  v3,  v7, v11, v15, v16, v17, v18, v19,  v0,  v4,  v8, v12,  v2,  v6, v10, v14, v28, v29, v30, v31, v20, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3
    qq_butterfly_mixed  v0,  v4,  v8, v12,  v2,  v6, v10, v14, v28, v29, v30, v31,  v0,  v2,  v4,  v6,  v1,  v3,  v5,  v7, v16, v17, v18, v19, v20, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3, v24, 0, 1, v24, 2, 3, v25, 0, 1, v25, 2, 3
    qq_butterfly_mixed  v0,  v2,  v4,  v6,  v1,  v3,  v5,  v7, v16, v17, v18, v19,  v8, v10, v12, v14,  v9, v11, v13, v15, v28, v29, v30, v31, v20,  v24, 0, 1, v24, 2, 3, v25, 0, 1, v25, 2, 3, v26, 0, 1, v26, 2, 3, v27, 0, 1, v27, 2, 3
    qq_butterfly_bot  v8, v10, v12, v14,  v9, v11, v13, v15, v28, v29, v30, v31, v20, v26, 0, 1, v26, 2, 3, v27, 0, 1, v27, 2, 3

    sub counter, counter, #1
    cbnz counter, _ntt_top_loop

    st1 { v1.4S}, [ src1], #16
    st1 { v3.4S}, [ src3], #16
    st1 { v5.4S}, [ src5], #16
    st1 { v7.4S}, [ src7], #16
    st1 { v9.4S}, [ src9], #16
    st1 {v11.4S}, [src11], #16
    st1 {v13.4S}, [src13], #16
    st1 {v15.4S}, [src15], #16

    st1 { v0.4S}, [ src0], #16
    st1 { v2.4S}, [ src2], #16
    st1 { v4.4S}, [ src4], #16
    st1 { v6.4S}, [ src6], #16
    st1 { v8.4S}, [ src8], #16
    st1 {v10.4S}, [src10], #16
    st1 {v12.4S}, [src12], #16
    st1 {v14.4S}, [src14], #16

    .unreq    Q
    .unreq    src0
    .unreq    src1
    .unreq    src2
    .unreq    src3
    .unreq    src4
    .unreq    src5
    .unreq    src6
    .unreq    src7
    .unreq    src8
    .unreq    src9
    .unreq    src10
    .unreq    src11
    .unreq    src12
    .unreq    src13
    .unreq    src14
    .unreq    src15
    .unreq    table
    .unreq    counter
    pop_all

    br lr

.align 2
.global PQCLEAN_DILITHIUM5_AARCH64__asm_ntt_SIMD_bot
.global _PQCLEAN_DILITHIUM5_AARCH64__asm_ntt_SIMD_bot
PQCLEAN_DILITHIUM5_AARCH64__asm_ntt_SIMD_bot:
_PQCLEAN_DILITHIUM5_AARCH64__asm_ntt_SIMD_bot:

    push_all
    Q         .req w20
    src0      .req x0
    des0      .req x1
    src1      .req x2
    des1      .req x3
    table0    .req x28
    table1    .req x27
    counter   .req x19

    ldr Q, [x2]

    add table0, x1, #128
    add table1, table0, #1024

    add src1, src0, #512

    add des0, src0, #0
    add des1, src0, #512

    mov counter, #8
    _ntt_bot_loop:

    ld1 {  v0.4S,  v1.4S,  v2.4S,  v3.4S}, [src0], #64
    ld1 { v16.4S, v17.4S, v18.4S, v19.4S}, [src1], #64

    ld1 {  v4.4S,  v5.4S}, [table0], #32
    ld2 {  v6.4S,  v7.4S}, [table0], #32
    ld4 {  v8.4S,  v9.4S, v10.4S, v11.4S}, [table0], #64
    ld1 { v20.4S, v21.4S}, [table1], #32
    ld2 { v22.4S, v23.4S}, [table1], #32
    ld4 { v24.4S, v25.4S, v26.4S, v27.4S}, [table1], #64

    mov v4.S[0], Q

    dq_butterfly_top  v0,  v1,  v2,  v3, v12, v13, v4,  v4, 2, 3,  v4, 2, 3
    dq_butterfly_mixed  v0,  v1,  v2,  v3, v12, v13, v16, v17, v18, v19, v28, v29, v4,  v4, 2, 3,  v4, 2, 3, v20, 2, 3, v20, 2, 3
    dq_butterfly_mixed v16, v17, v18, v19, v28, v29,  v0,  v2,  v1,  v3, v12, v13, v4, v20, 2, 3, v20, 2, 3,  v5, 0, 1,  v5, 2, 3
    dq_butterfly_mixed  v0,  v2,  v1,  v3, v12, v13, v16, v18, v17, v19, v28, v29, v4,  v5, 0, 1,  v5, 2, 3, v21, 0, 1, v21, 2, 3
    dq_butterfly_bot v16, v18, v17, v19, v28, v29, v4, v21, 0, 1, v21, 2, 3

    trn_4x4  v0,  v1,  v2,  v3, v12, v13, v14, v15
    trn_4x4 v16, v17, v18, v19, v28, v29, v30, v31

    dq_butterfly_vec_top  v0,  v1,  v2,  v3, v12, v13, v4,  v6,  v7,  v6,  v7
    dq_butterfly_vec_mixed  v0,  v1,  v2,  v3, v12, v13, v16, v17, v18, v19, v28, v29, v4,  v6,  v7,  v6,  v7, v22, v23, v22, v23
    dq_butterfly_vec_mixed v16, v17, v18, v19, v28, v29,  v0,  v2,  v1,  v3, v12, v13, v4, v22, v23, v22, v23,  v8,  v9, v10, v11
    dq_butterfly_vec_mixed  v0,  v2,  v1,  v3, v12, v13, v16, v18, v17, v19, v28, v29, v4,  v8,  v9, v10, v11, v24, v25, v26, v27
    dq_butterfly_vec_bot v16, v18, v17, v19, v28, v29, v4, v24, v25, v26, v27

    st4 {  v0.4S,  v1.4S,  v2.4S,  v3.4S}, [des0], #64
    st4 { v16.4S, v17.4S, v18.4S, v19.4S}, [des1], #64

    sub counter, counter, #1
    cbnz counter, _ntt_bot_loop

    .unreq    Q
    .unreq    src0
    .unreq    des0
    .unreq    src1
    .unreq    des1
    .unreq    table0
    .unreq    table1
    .unreq    counter
    pop_all

    br lr














