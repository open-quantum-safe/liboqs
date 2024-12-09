/// Copyright (c) 2024 The mlkem-native project authors
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
#if defined(MLKEM_NATIVE_ARITH_BACKEND_AARCH64_CLEAN)

// Bounds:
// If C is chosen so that |src| < q * C, then |dst| < q * (0.0508 * C + 1/2)
//
// See mlken/reduce.c and test/test_bounds.py for more details.
.macro mulmodq dst, src, const, idx0, idx1
        // Signed barrett multiplication using
        // round-to-nearest-even-integer approximation.
        // Following https://eprint.iacr.org/2021/986.pdf, this
        // is functionally the same as a signed Montgomery multiplication
        // with a suitable constant of absolute value < q.
        sqrdmulh t2.8h,      \src\().8h, \const\().h[\idx1\()]
        mul      \dst\().8h, \src\().8h, \const\().h[\idx0\()]
        mls      \dst\().8h, t2.8h,      consts.h[0]
.endm

.macro mulmod dst, src, const, const_twisted
        sqrdmulh t2.8h,      \src\().8h, \const_twisted\().8h
        mul      \dst\().8h, \src\().8h, \const\().8h
        mls      \dst\().8h, t2.8h,      consts.h[0]
.endm

.macro gs_butterfly a, b, root, idx0, idx1
        sub tmp.8h,   \a\().8h, \b\().8h
        add \a\().8h, \a\().8h, \b\().8h
        mulmodq  \b, tmp, \root, \idx0, \idx1
.endm

.macro gs_butterfly_v a, b, root, root_twisted
        sub tmp.8h,   \a\().8h, \b\().8h
        add \a\().8h, \a\().8h, \b\().8h
        mulmod  \b, tmp, \root, \root_twisted
.endm

.macro mul_ninv dst0, dst1, dst2, dst3, src0, src1, src2, src3
        mulmod \dst0, \src0, ninv, ninv_tw
        mulmod \dst1, \src1, ninv, ninv_tw
        mulmod \dst2, \src2, ninv, ninv_tw
        mulmod \dst3, \src3, ninv, ninv_tw
.endm

.macro barrett_reduce a
        sqdmulh t0.8h,    \a\().8h, consts.h[1]
        srshr   t0.8h,    t0.8h,    #11
        mls     \a\().8h, t0.8h,    consts.h[0]
.endm

.macro load_roots_012
        ldr q_root0, [r01234_ptr], #32
        ldr q_root1, [r01234_ptr, #-16]
.endm

.macro load_next_roots_34
        ldr q_root0, [r01234_ptr], #16
.endm

.macro load_next_roots_56
        ldr q_root0,    [r56_ptr], #(6*16)
        ldr q_root0_tw, [r56_ptr, #(-6*16 + 1*16)]
        ldr q_root1,    [r56_ptr, #(-6*16 + 2*16)]
        ldr q_root1_tw, [r56_ptr, #(-6*16 + 3*16)]
        ldr q_root2,    [r56_ptr, #(-6*16 + 4*16)]
        ldr q_root2_tw, [r56_ptr, #(-6*16 + 5*16)]
.endm

.macro transpose4 data
        trn1 t0.4s, \data\()0.4s, \data\()1.4s
        trn2 t1.4s, \data\()0.4s, \data\()1.4s
        trn1 t2.4s, \data\()2.4s, \data\()3.4s
        trn2 t3.4s, \data\()2.4s, \data\()3.4s

        trn2 \data\()2.2d, t0.2d, t2.2d
        trn2 \data\()3.2d, t1.2d, t3.2d
        trn1 \data\()0.2d, t0.2d, t2.2d
        trn1 \data\()1.2d, t1.2d, t3.2d
.endm

.macro transpose_single data_out, data_in
        trn1 \data_out\()0.4s, \data_in\()0.4s, \data_in\()1.4s
        trn2 \data_out\()1.4s, \data_in\()0.4s, \data_in\()1.4s
        trn1 \data_out\()2.4s, \data_in\()2.4s, \data_in\()3.4s
        trn2 \data_out\()3.4s, \data_in\()2.4s, \data_in\()3.4s
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

// For comparability reasons, the output range for the coefficients of this
// invNTT code is supposed to match the implementation from PQClean on commit
// ee71d2c823982bfcf54686f3cf1d666f396dc9aa. After the invNTT, the coefficients
// are NOT canonically reduced. The ordering of the coefficients is canonical,
// also matching PQClean.

.text
        .global MLKEM_ASM_NAMESPACE(intt_asm_clean)

        in         .req x0
        r01234_ptr .req x1
        r56_ptr    .req x2

        inp     .req x3
        count   .req x4
        wtmp    .req w5

        data0  .req v8
        data1  .req v9
        data2  .req v10
        data3  .req v11
        data4  .req v12
        data5  .req v13
        data6  .req v14
        data7  .req v15

        q_data0  .req q8
        q_data1  .req q9
        q_data2  .req q10
        q_data3  .req q11
        q_data4  .req q12
        q_data5  .req q13
        q_data6  .req q14
        q_data7  .req q15

        root0    .req v0
        root1    .req v1
        root2    .req v2
        root0_tw .req v4
        root1_tw .req v5
        root2_tw .req v6

        consts     .req v7
        q_consts   .req q7

        q_root0    .req q0
        q_root1    .req q1
        q_root2    .req q2
        q_root0_tw .req q4
        q_root1_tw .req q5
        q_root2_tw .req q6

        tmp .req v24
        t0  .req v25
        t1  .req v26
        t2  .req v27
        t3  .req v28

        ninv             .req v29
        ninv_tw          .req v30

.balign 4
MLKEM_ASM_NAMESPACE(intt_asm_clean):
        push_stack

        // Setup constants
        mov wtmp, #3329
        mov consts.h[0], wtmp
        mov wtmp, #20159
        mov consts.h[1], wtmp
        mov wtmp, #512
        dup ninv.8h, wtmp
        mov wtmp, #5040
        dup ninv_tw.8h, wtmp

        mov inp, in
        mov count, #8

scale_start:

        ldr q_data0, [inp, #(16*0)]
        ldr q_data1, [inp, #(16*1)]
        ldr q_data2, [inp, #(16*2)]
        ldr q_data3, [inp, #(16*3)]

        mul_ninv data0, data1, data2, data3, data0, data1, data2, data3
        // Bounds: Absolute value < q

        str q_data0, [inp], #64
        str q_data1, [inp, #(-64 + 16*1)]
        str q_data2, [inp, #(-64 + 16*2)]
        str q_data3, [inp, #(-64 + 16*3)]

        subs count, count, #1
        cbnz count, scale_start

        mov inp, in
        mov count, #8

        .p2align 2
layer3456_start:

        ldr q_data0, [inp, #(16*0)]
        ldr q_data1, [inp, #(16*1)]
        ldr q_data2, [inp, #(16*2)]
        ldr q_data3, [inp, #(16*3)]

        transpose4 data // manual ld4

        load_next_roots_56

        // Layer 7
        gs_butterfly_v data0, data1, root1, root1_tw
        gs_butterfly_v data2, data3, root2, root2_tw
        // Bounds:
        // data0, data2: < 2q
        // data1, data3: < q

        // Layer 6
        gs_butterfly_v data0, data2, root0, root0_tw
        gs_butterfly_v data1, data3, root0, root0_tw
        // Bounds:
        // data0: < 4q
        // data1: < 2q
        // data2, data3: < q

        transpose4 data

        load_next_roots_34

        // Layer 5
        gs_butterfly data0, data1, root0, 2, 3
        gs_butterfly data2, data3, root0, 4, 5
        // Max bound: 8q

        // Not all of those reductions are needed, but the bounds tracking
        // is easier if we uniformly reduce at this point.
        barrett_reduce data0
        barrett_reduce data2
        barrett_reduce data1
        barrett_reduce data3

        // Bounds: q/2

        // Layer 4
        gs_butterfly data0, data2, root0, 0, 1
        gs_butterfly data1, data3, root0, 0, 1
        // Bounds: < q

        str q_data0, [inp], #(64)
        str q_data1, [inp, #(-64 + 16*1)]
        str q_data2, [inp, #(-64 + 16*2)]
        str q_data3, [inp, #(-64 + 16*3)]

        subs count, count, #1
        cbnz count, layer3456_start

        // ---------------------------------------------------------------------

        mov count, #4
        load_roots_012

        .p2align 2

layer012_start:

        ldr q_data0, [in, #0]
        ldr q_data1, [in, #(1*(512/8))]
        ldr q_data2, [in, #(2*(512/8))]
        ldr q_data3, [in, #(3*(512/8))]
        ldr q_data4, [in, #(4*(512/8))]
        ldr q_data5, [in, #(5*(512/8))]
        ldr q_data6, [in, #(6*(512/8))]
        ldr q_data7, [in, #(7*(512/8))]

        gs_butterfly data0, data1, root0, 6, 7
        gs_butterfly data2, data3, root1, 0, 1
        gs_butterfly data4, data5, root1, 2, 3
        gs_butterfly data6, data7, root1, 4, 5

        gs_butterfly data0, data2, root0, 2, 3
        gs_butterfly data1, data3, root0, 2, 3
        gs_butterfly data4, data6, root0, 4, 5
        gs_butterfly data5, data7, root0, 4, 5

        gs_butterfly data0, data4, root0, 0, 1
        gs_butterfly data1, data5, root0, 0, 1
        gs_butterfly data2, data6, root0, 0, 1
        gs_butterfly data3, data7, root0, 0, 1

        // Bounds: < 8q

        str q_data4, [in, #(4*(512/8))]
        str q_data5, [in, #(5*(512/8))]
        str q_data6, [in, #(6*(512/8))]
        str q_data7, [in, #(7*(512/8))]

        str q_data0, [in], #(16)
        str q_data1, [in, #(-16 + 1*(512/8))]
        str q_data2, [in, #(-16 + 2*(512/8))]
        str q_data3, [in, #(-16 + 3*(512/8))]

        subs count, count, #1
        cbnz count, layer012_start

        pop_stack
        ret

/****************** REGISTER DEALLOCATIONS *******************/
    .unreq in
    .unreq r01234_ptr
    .unreq r56_ptr
    .unreq inp
    .unreq count
    .unreq wtmp
    .unreq data0
    .unreq data1
    .unreq data2
    .unreq data3
    .unreq data4
    .unreq data5
    .unreq data6
    .unreq data7
    .unreq q_data0
    .unreq q_data1
    .unreq q_data2
    .unreq q_data3
    .unreq q_data4
    .unreq q_data5
    .unreq q_data6
    .unreq q_data7
    .unreq root0
    .unreq root1
    .unreq root2
    .unreq root0_tw
    .unreq root1_tw
    .unreq root2_tw
    .unreq consts
    .unreq q_consts
    .unreq q_root0
    .unreq q_root1
    .unreq q_root2
    .unreq q_root0_tw
    .unreq q_root1_tw
    .unreq q_root2_tw
    .unreq tmp
    .unreq t0
    .unreq t1
    .unreq t2
    .unreq t3
    .unreq ninv
    .unreq ninv_tw

#endif /* MLKEM_NATIVE_ARITH_BACKEND_AARCH64_CLEAN */
