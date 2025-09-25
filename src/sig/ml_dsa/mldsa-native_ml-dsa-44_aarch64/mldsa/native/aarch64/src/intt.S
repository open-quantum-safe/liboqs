/* Copyright (c) 2022 Arm Limited
 * Copyright (c) 2022 Hanno Becker
 * Copyright (c) 2023 Amin Abdulrahman, Matthias Kannwischer
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "../../../common.h"
#if defined(MLD_ARITH_BACKEND_AARCH64)

.macro mulmodq dst, src, const, idx0, idx1
        sqrdmulh t2.4s,      \src\().4s, \const\().s[\idx1\()]
        mul      \dst\().4s, \src\().4s, \const\().s[\idx0\()]
        mls      \dst\().4s, t2.4s,      modulus.s[0]
.endm

.macro mulmod dst, src, const, const_twisted
        sqrdmulh t2.4s,   \src\().4s, \const_twisted\().4s
        mul      \dst\().4s, \src\().4s, \const\().4s
        mls      \dst\().4s, t2.4s,   modulus.s[0]
.endm


.macro gs_butterfly a, b, root, idx0, idx1
        sub tmp.4s,   \a\().4s, \b\().4s
        add \a\().4s, \a\().4s, \b\().4s
        mulmodq  \b, tmp, \root, \idx0, \idx1
.endm

.macro gs_butterfly_v a, b, root, root_twisted
        sub tmp.4s,   \a\().4s, \b\().4s
        add \a\().4s, \a\().4s, \b\().4s
        mulmod  \b, tmp, \root, \root_twisted
.endm

.macro mul_ninv dst0, dst1, dst2, dst3, dst4, dst5, dst6, dst7, src0, src1, src2, src3, src4, src5, src6, src7
        mulmod \dst0, \src0, ninv, ninv_tw
        mulmod \dst1, \src1, ninv, ninv_tw
        mulmod \dst2, \src2, ninv, ninv_tw
        mulmod \dst3, \src3, ninv, ninv_tw
        mulmod \dst4, \src4, ninv, ninv_tw
        mulmod \dst5, \src5, ninv, ninv_tw
        mulmod \dst6, \src6, ninv, ninv_tw
        mulmod \dst7, \src7, ninv, ninv_tw
.endm

.macro load_roots_1234 r_ptr
        ldr q_root0, [\r_ptr], #(8*16)
        ldr q_root1, [\r_ptr, #(-8*16 + 1*16)]
        ldr q_root2, [\r_ptr, #(-8*16 + 2*16)]
        ldr q_root3, [\r_ptr, #(-8*16 + 3*16)]
        ldr q_root4, [\r_ptr, #(-8*16 + 4*16)]
        ldr q_root5, [\r_ptr, #(-8*16 + 5*16)]
        ldr q_root6, [\r_ptr, #(-8*16 + 6*16)]
        ldr q_root7, [\r_ptr, #(-8*16 + 7*16)]
.endm

.macro load_next_roots_56 root0, r_ptr0
        ldr q_\root0, [\r_ptr0], #16
.endm

.macro load_next_roots_6 root0, r_ptr0
        ldr q_\root0, [\r_ptr0], #8
.endm

.macro load_next_roots_78 root0, root0_tw, root1, root1_tw, root2, root2_tw, r_ptr1
        ldr q_\root0,    [\r_ptr1], #(6*16)
        ldr q_\root0_tw, [\r_ptr1, #(-6*16 + 1*16)]
        ldr q_\root1,    [\r_ptr1, #(-6*16 + 2*16)]
        ldr q_\root1_tw, [\r_ptr1, #(-6*16 + 3*16)]
        ldr q_\root2,    [\r_ptr1, #(-6*16 + 4*16)]
        ldr q_\root2_tw, [\r_ptr1, #(-6*16 + 5*16)]
.endm

.macro transpose4 data0, data1, data2, data3
        trn1 t0.4s, \data0\().4s, \data1\().4s
        trn2 t1.4s, \data0\().4s, \data1\().4s
        trn1 t2.4s, \data2\().4s, \data3\().4s
        trn2 t3.4s, \data2\().4s, \data3\().4s

        trn2 \data2\().2d, t0.2d, t2.2d
        trn2 \data3\().2d, t1.2d, t3.2d
        trn1 \data0\().2d, t0.2d, t2.2d
        trn1 \data1\().2d, t1.2d, t3.2d
.endm

.macro save_vregs
        sub sp, sp, #(16*4)
        stp  d8,  d9, [sp, #16*0]
        stp d10, d11, [sp, #16*1]
        stp d12, d13, [sp, #16*2]
        stp d14, d15, [sp, #16*3]
.endm

.macro restore_vregs
        ldp  d8,  d9, [sp, #16*0]
        ldp d10, d11, [sp, #16*1]
        ldp d12, d13, [sp, #16*2]
        ldp d14, d15, [sp, #16*3]
        add sp, sp, #(16*4)
.endm

.macro push_stack
        save_vregs
.endm

.macro pop_stack
        restore_vregs
.endm

.text
.global MLD_ASM_NAMESPACE(intt_asm)
.balign 4
MLD_ASM_FN_SYMBOL(intt_asm)
        push_stack

        in        .req x0
        r5678_ptr .req x1
        r1234_ptr .req x2
        inp       .req x3
        count     .req x4
        xtmp      .req x5

        wtmp .req w5

        data0  .req v8
        data1  .req v9
        data2  .req v10
        data3  .req v11
        data4  .req v12
        data5  .req v13
        data6  .req v14
        data7  .req v15
        data8  .req v16
        data9  .req v17
        data10 .req v18
        data11 .req v19
        data12 .req v20
        data13 .req v21
        data14 .req v22
        data15 .req v23

        q_data0  .req q8
        q_data1  .req q9
        q_data2  .req q10
        q_data3  .req q11
        q_data4  .req q12
        q_data5  .req q13
        q_data6  .req q14
        q_data7  .req q15
        q_data8  .req q16
        q_data9  .req q17
        q_data10 .req q18
        q_data11 .req q19
        q_data12 .req q20
        q_data13 .req q21
        q_data14 .req q22
        q_data15 .req q23

        root0    .req v0
        root1    .req v1
        root2    .req v2
        root3    .req v3
        root0_tw .req v4
        root1_tw .req v5
        root2_tw .req v6
        root3_tw .req v7


        q_root0    .req q0
        q_root1    .req q1
        q_root2    .req q2
        q_root3    .req q3
        q_root0_tw .req q4
        q_root1_tw .req q5
        q_root2_tw .req q6
        q_root3_tw .req q7


        tmp .req v24
        q_tmp .req q24
        t0  .req v25
        t1  .req v26
        t2  .req v27
        t3  .req v28

        modulus .req v29

        // load q = 8380417
        movz wtmp, #57345
        movk wtmp, #127, lsl #16
        dup modulus.4s, wtmp

        mov inp, in

        mov count, #16

        .p2align 2
layer5678_start:

        ld4 {data0.4S, data1.4S, data2.4S, data3.4S}, [in]

        load_next_roots_78 root0, root0_tw, root1, root1_tw, root2, root2_tw, r5678_ptr

        gs_butterfly_v data0, data1, root1, root1_tw
        gs_butterfly_v data2, data3, root2, root2_tw
        gs_butterfly_v data0, data2, root0, root0_tw
        gs_butterfly_v data1, data3, root0, root0_tw

        transpose4 data0, data1, data2, data3

        load_next_roots_6  root1, r1234_ptr
        load_next_roots_56 root0, r1234_ptr

        gs_butterfly data0, data1, root0, 0, 1
        gs_butterfly data2, data3, root0, 2, 3
        gs_butterfly data0, data2, root1, 0, 1
        gs_butterfly data1, data3, root1, 0, 1

        str  q_data0, [in], #(16*4)
        str  q_data1, [in, #(-16*4 +  1*16)]
        str  q_data2, [in, #(-16*4 +  2*16)]
        str  q_data3, [in, #(-16*4 +  3*16)]

        subs count, count, #1
        cbnz count, layer5678_start

        .unreq root0_tw
        .unreq root1_tw
        .unreq root2_tw
        .unreq root3_tw
        .unreq q_root0_tw
        .unreq q_root1_tw
        .unreq q_root2_tw
        .unreq q_root3_tw
        .unreq t0
        .unreq t1

        root4            .req v4
        root5            .req v5
        root6            .req v6
        root7            .req v7
        q_root4      .req q4
        q_root5      .req q5
        q_root6      .req q6
        q_root7      .req q7
        ninv             .req v25
        ninv_tw          .req v26

        mov in, inp
        mov count, #4

        // load ninv
        mov wtmp, #16382 // 2^(32 - 8) mod Q
        dup ninv.4s, wtmp

        // load ninv_tw = 4197891
        movz wtmp, #3587
        movk wtmp, #64, lsl #16
        dup ninv_tw.4s, wtmp

        load_roots_1234 r1234_ptr

        .p2align 2
layer1234_start:
        ldr q_data0,  [in, #(0*(512/8))]
        ldr q_data1,  [in, #(1*(512/8))]
        ldr q_data2,  [in, #(2*(512/8))]
        ldr q_data3,  [in, #(3*(512/8))]
        ldr q_data4,  [in, #(4*(512/8))]
        ldr q_data5,  [in, #(5*(512/8))]
        ldr q_data6,  [in, #(6*(512/8))]
        ldr q_data7,  [in, #(7*(512/8))]
        ldr q_data8,  [in, #(8*(512/8))]
        ldr q_data9,  [in, #(9*(512/8))]
        ldr q_data10, [in, #(10*(512/8))]
        ldr q_data11, [in, #(11*(512/8))]
        ldr q_data12, [in, #(12*(512/8))]
        ldr q_data13, [in, #(13*(512/8))]
        ldr q_data14, [in, #(14*(512/8))]
        ldr q_data15, [in, #(15*(512/8))]

        // layer4
        gs_butterfly data0, data1, root3, 2, 3
        gs_butterfly data2, data3, root4, 0, 1
        gs_butterfly data4, data5, root4, 2, 3
        gs_butterfly data6, data7, root5, 0, 1
        gs_butterfly data8, data9, root5, 2, 3
        gs_butterfly data10, data11, root6, 0, 1
        gs_butterfly data12, data13, root6, 2, 3
        gs_butterfly data14, data15, root7, 0, 1

        // layer3
        gs_butterfly data0, data2, root1, 2, 3
        gs_butterfly data1, data3, root1, 2, 3
        gs_butterfly data4, data6, root2, 0, 1
        gs_butterfly data5, data7, root2, 0, 1
        gs_butterfly data8, data10, root2, 2, 3
        gs_butterfly data9, data11, root2, 2, 3
        gs_butterfly data12, data14, root3, 0, 1
        gs_butterfly data13, data15, root3, 0, 1

        // layer2
        gs_butterfly data0, data4, root0, 2, 3
        gs_butterfly data1, data5, root0, 2, 3
        gs_butterfly data2, data6, root0, 2, 3
        gs_butterfly data3, data7, root0, 2, 3
        gs_butterfly data8, data12, root1, 0, 1
        gs_butterfly data9, data13, root1, 0, 1
        gs_butterfly data10, data14, root1, 0, 1
        gs_butterfly data11, data15, root1, 0, 1

        // layer 1
        gs_butterfly data0, data8, root0, 0, 1
        gs_butterfly data1, data9, root0, 0, 1
        gs_butterfly data2, data10, root0, 0, 1
        gs_butterfly data3, data11, root0, 0, 1
        gs_butterfly data4, data12, root0, 0, 1
        gs_butterfly data5, data13, root0, 0, 1
        gs_butterfly data6, data14, root0, 0, 1
        gs_butterfly data7, data15, root0, 0, 1

        str q_data8,  [in,  #(8*(512/8))]
        str q_data9,  [in,  #(9*(512/8))]
        str q_data10, [in,  #(10*(512/8))]
        str q_data11, [in,  #(11*(512/8))]
        str q_data12, [in,  #(12*(512/8))]
        str q_data13, [in,  #(13*(512/8))]
        str q_data14, [in,  #(14*(512/8))]
        str q_data15, [in,  #(15*(512/8))]

        // Scale half the coeffs 2^-8 and the Montgomery factor 2^32.
        // For the other half, the scaling has been merged into the
        // multiplication with the twiddle factor on the last layer.
        mul_ninv data0, data1, data2, data3, data4, data5, data6, data7, data0, data1, data2, data3, data4, data5, data6, data7

        str q_data0, [in], #(16)
        str q_data1, [in, #(-16 + 1*(512/8))]
        str q_data2, [in, #(-16 + 2*(512/8))]
        str q_data3, [in, #(-16 + 3*(512/8))]
        str q_data4, [in, #(-16 + 4*(512/8))]
        str q_data5, [in, #(-16 + 5*(512/8))]
        str q_data6, [in, #(-16 + 6*(512/8))]
        str q_data7, [in, #(-16 + 7*(512/8))]

        subs count, count, #1
        cbnz count, layer1234_start

        pop_stack
        ret

#endif /* MLD_ARITH_BACKEND_AARCH64 */
