/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
//
// AArch64 re-implementation of the asymmetric base multiplication from:
//
// Neon NTT: Faster Dilithium, Kyber, and Saber on Cortex-A72 and Apple M1
// https://eprint.iacr.org/2021/986
// https://github.com/neon-ntt/neon-ntt

#include "../../../common.h"
#if defined(MLKEM_NATIVE_ARITH_BACKEND_AARCH64_CLEAN)

// Input:
// - Vectors al, ah of 32-bit entries
// Output:
// - Montgomery reductions of al || ah, stored in al
.macro montgomery_reduce_long x, a
        uzp1   t0.8h, \a\()l.8h, \a\()h.8h
        mul    t0.8h, t0.8h, modulus_twisted.8h
        smlal  \a\()l.4s, t0.4h, modulus.4h
        smlal2 \a\()h.4s, t0.8h, modulus.8h
        uzp2   \x\().8h, \a\()l.8h, \a\()h.8h
.endm

// Computes products (a0*b0 + a0*b0t, a0*b1 + a1*b0) in 32-bit.
//
// Bounds:
// - Assume |a| < 4096,
// - Result: < 2*4096*2^15 = 2^28
.macro pmull d, a, b
        smull  \d\()0l.4s, \a\()0.4h, \b\()0.4h
        smull2 \d\()0h.4s, \a\()0.8h, \b\()0.8h
        smlal  \d\()0l.4s, \a\()1.4h, \b\()1t.4h
        smlal2 \d\()0h.4s, \a\()1.8h, \b\()1t.8h

        smull  \d\()1l.4s, \a\()0.4h, \b\()1.4h
        smull2 \d\()1h.4s, \a\()0.8h, \b\()1.8h
        smlal  \d\()1l.4s, \a\()1.4h, \b\()0.4h
        smlal2 \d\()1h.4s, \a\()1.8h, \b\()0.8h
.endm

.macro pmlal d, a, b
        smlal  \d\()0l.4s, \a\()0.4h, \b\()0.4h
        smlal2 \d\()0h.4s, \a\()0.8h, \b\()0.8h
        smlal  \d\()0l.4s, \a\()1.4h, \b\()1t.4h
        smlal2 \d\()0h.4s, \a\()1.8h, \b\()1t.8h

        smlal  \d\()1l.4s, \a\()0.4h, \b\()1.4h
        smlal2 \d\()1h.4s, \a\()0.8h, \b\()1.8h
        smlal  \d\()1l.4s, \a\()1.4h, \b\()0.4h
        smlal2 \d\()1h.4s, \a\()1.8h, \b\()0.8h
.endm

.macro ld2_wrap a, ptr
        ldr q_tmp0, [\ptr\()], #32
        ldr q_tmp1, [\ptr\(), #-16]
        uzp1 \a\()0.8h, tmp0.8h, tmp1.8h
        uzp2 \a\()1.8h, tmp0.8h, tmp1.8h
.endm

.macro st2_wrap a, ptr
        zip1 tmp0.8h, \a\()0.8h, \a\()1.8h
        zip2 tmp1.8h, \a\()0.8h, \a\()1.8h
        str q_tmp0, [\ptr\()], #32
        str q_tmp1, [\ptr\(), #-16]
.endm

.macro load_polys a, b, a_ptr, b_ptr, b_cache_ptr
        ld2_wrap \a\(), \a_ptr
        ld2_wrap \b\(), \b_ptr
        ld1 {\b\()1t.8h}, [\b_cache_ptr], #16
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

        out          .req x0
        a0_ptr       .req x1
        b0_ptr       .req x2
        b0_cache_ptr .req x3
        a1_ptr       .req x4
        b1_ptr       .req x5
        b1_cache_ptr .req x6
        a2_ptr       .req x7
        b2_ptr       .req x8
        b2_cache_ptr .req x9
        a3_ptr       .req x10
        b3_ptr       .req x11
        b3_cache_ptr .req x12
        count        .req x13
        wtmp         .req w14

        modulus           .req v0
        modulus_twisted   .req v2

        aa0      .req v3
        aa1      .req v4
        bb0      .req v5
        bb1      .req v6
        bb1t     .req v7

        res0l   .req v8
        res1l   .req v9
        res0h   .req v10
        res1h   .req v11

        tmp0 .req v12
        tmp1 .req v13
        q_tmp0 .req q12
        q_tmp1 .req q13

        out0 .req v26
        out1 .req v27

        t0   .req v28

#if MLKEM_K == 2
.global MLKEM_ASM_NAMESPACE_K(polyvec_basemul_acc_montgomery_cached_asm_clean)

.balign 4
MLKEM_ASM_NAMESPACE_K(polyvec_basemul_acc_montgomery_cached_asm_clean):
        push_stack

        mov wtmp, #3329
        dup modulus.8h, wtmp

        mov wtmp, #3327
        dup modulus_twisted.8h, wtmp

        // Computed bases of vector entries

        add a1_ptr, a0_ptr, #(1 * 512)
        add b1_ptr, b0_ptr, #(1 * 512)
        add b1_cache_ptr, b0_cache_ptr, #(1 * 512/2)

        mov count, #(MLKEM_N / 16)
k2_loop_start:

        load_polys aa, bb, a0_ptr, b0_ptr, b0_cache_ptr
        pmull res, aa, bb
        load_polys aa, bb, a1_ptr, b1_ptr, b1_cache_ptr
        pmlal res, aa, bb

        montgomery_reduce_long out0, res0
        montgomery_reduce_long out1, res1

        st2_wrap out, out

        subs count, count, #1
        cbnz count, k2_loop_start

        pop_stack
        ret
#endif /* MLKEM_K == 2 */

#if MLKEM_K == 3
.global MLKEM_ASM_NAMESPACE_K(polyvec_basemul_acc_montgomery_cached_asm_clean)

.balign 4
MLKEM_ASM_NAMESPACE_K(polyvec_basemul_acc_montgomery_cached_asm_clean):
        push_stack
        mov wtmp, #3329
        dup modulus.8h, wtmp

        mov wtmp, #3327
        dup modulus_twisted.8h, wtmp

        // Computed bases of vector entries

        add a1_ptr, a0_ptr, #(1 * 512)
        add b1_ptr, b0_ptr, #(1 * 512)
        add b1_cache_ptr, b0_cache_ptr, #(1 * 512/2)
        add a2_ptr, a0_ptr, #(2 * 512)
        add b2_ptr, b0_ptr, #(2 * 512)
        add b2_cache_ptr, b0_cache_ptr, #(2 * 512/2)

        mov count, #(MLKEM_N / 16)
k3_loop_start:

        load_polys aa, bb, a0_ptr, b0_ptr, b0_cache_ptr
        pmull res, aa, bb
        load_polys aa, bb, a1_ptr, b1_ptr, b1_cache_ptr
        pmlal res, aa, bb
        load_polys aa, bb, a2_ptr, b2_ptr, b2_cache_ptr
        pmlal res, aa, bb

        montgomery_reduce_long out0, res0
        montgomery_reduce_long out1, res1

        st2_wrap out, out

        subs count, count, #1
        cbnz count, k3_loop_start

        pop_stack
        ret
#endif /* MLKEM_K == 3 */

#if MLKEM_K == 4
.global MLKEM_ASM_NAMESPACE_K(polyvec_basemul_acc_montgomery_cached_asm_clean)

.balign 4
MLKEM_ASM_NAMESPACE_K(polyvec_basemul_acc_montgomery_cached_asm_clean):
        push_stack
        mov wtmp, #3329
        dup modulus.8h, wtmp

        mov wtmp, #3327
        dup modulus_twisted.8h, wtmp

        // Computed bases of vector entries

        add a1_ptr, a0_ptr, #(1 * 512)
        add b1_ptr, b0_ptr, #(1 * 512)
        add b1_cache_ptr, b0_cache_ptr, #(1 * 512/2)
        add a2_ptr, a0_ptr, #(2 * 512)
        add b2_ptr, b0_ptr, #(2 * 512)
        add b2_cache_ptr, b0_cache_ptr, #(2 * 512/2)
        add a3_ptr, a0_ptr, #(3 * 512)
        add b3_ptr, b0_ptr, #(3 * 512)
        add b3_cache_ptr, b0_cache_ptr, #(3 * 512/2)

        // Bounds:
        //
        // Each pmull is bound by 2*4096*2^15=2^28, so the final value
        // before Montgomery reduction is bound by 2^30.

        mov count, #(MLKEM_N / 16)
k4_loop_start:

        load_polys aa, bb, a0_ptr, b0_ptr, b0_cache_ptr
        pmull res, aa, bb
        load_polys aa, bb, a1_ptr, b1_ptr, b1_cache_ptr
        pmlal res, aa, bb
        load_polys aa, bb, a2_ptr, b2_ptr, b2_cache_ptr
        pmlal res, aa, bb
        load_polys aa, bb, a3_ptr, b3_ptr, b3_cache_ptr
        pmlal res, aa, bb

        montgomery_reduce_long out0, res0
        montgomery_reduce_long out1, res1

        st2_wrap out, out

        subs count, count, #1
        cbnz count, k4_loop_start

        pop_stack
        ret
#endif /* MLKEM_K == 4 */

/****************** REGISTER DEALLOCATIONS *******************/
    .unreq out
    .unreq a0_ptr
    .unreq b0_ptr
    .unreq b0_cache_ptr
    .unreq a1_ptr
    .unreq b1_ptr
    .unreq b1_cache_ptr
    .unreq a2_ptr
    .unreq b2_ptr
    .unreq b2_cache_ptr
    .unreq a3_ptr
    .unreq b3_ptr
    .unreq b3_cache_ptr
    .unreq count
    .unreq modulus
    .unreq modulus_twisted
    .unreq aa0
    .unreq aa1
    .unreq bb0
    .unreq bb1
    .unreq bb1t
    .unreq res0l
    .unreq res1l
    .unreq res0h
    .unreq wtmp
    .unreq res1h
    .unreq tmp0
    .unreq tmp1
    .unreq q_tmp0
    .unreq q_tmp1
    .unreq out0
    .unreq out1
    .unreq t0

#endif /* MLKEM_NATIVE_ARITH_BACKEND_AARCH64_CLEAN */
