/* Copyright (c) 2022 Arm Limited
 * Copyright (c) 2022 Hanno Becker
 * Copyright (c) 2023 Amin Abdulrahman, Matthias Kannwischer
 * Copyright (c) The mlkem-native project authors
 * Copyright (c) The mldsa-native project authors
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

/* AArch64 ML-DSA forward NTT following @[NeonNTT] and @[SLOTHY_Paper] */

#include "../../../common.h"
#if defined(MLD_ARITH_BACKEND_AARCH64) && \
    !defined(MLD_CONFIG_MULTILEVEL_NO_SHARED)

/*
 * WARNING: This file is auto-derived from the mldsa-native source file
 *   dev/aarch64_clean/src/ntt.S using scripts/simpasm. Do not modify it directly.
 */


.text
.balign 4
.global MLD_ASM_NAMESPACE(ntt_asm)
MLD_ASM_FN_SYMBOL(ntt_asm)

        .cfi_startproc
        sub	sp, sp, #0x40
        .cfi_adjust_cfa_offset 0x40
        stp	d8, d9, [sp]
        .cfi_rel_offset d8, 0x0
        .cfi_rel_offset d9, 0x8
        stp	d10, d11, [sp, #0x10]
        .cfi_rel_offset d10, 0x10
        .cfi_rel_offset d11, 0x18
        stp	d12, d13, [sp, #0x20]
        .cfi_rel_offset d12, 0x20
        .cfi_rel_offset d13, 0x28
        stp	d14, d15, [sp, #0x30]
        .cfi_rel_offset d14, 0x30
        .cfi_rel_offset d15, 0x38
        mov	w5, #0xe001             // =57345
        movk	w5, #0x7f, lsl #16
        dup	v7.4s, w5
        mov	x3, x0
        mov	x4, #0x8                // =8
        ldr	q0, [x1], #0x40
        ldur	q1, [x1, #-0x30]
        ldur	q2, [x1, #-0x20]
        ldur	q3, [x1, #-0x10]

Lntt_layer123_start:
        ldr	q8, [x0]
        ldr	q9, [x0, #0x80]
        ldr	q10, [x0, #0x100]
        ldr	q11, [x0, #0x180]
        ldr	q12, [x0, #0x200]
        ldr	q13, [x0, #0x280]
        ldr	q14, [x0, #0x300]
        ldr	q15, [x0, #0x380]
        sqrdmulh	v27.4s, v12.4s, v0.s[1]
        mul	v24.4s, v12.4s, v0.s[0]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v12.4s, v8.4s, v24.4s
        add	v8.4s, v8.4s, v24.4s
        sqrdmulh	v27.4s, v13.4s, v0.s[1]
        mul	v24.4s, v13.4s, v0.s[0]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v13.4s, v9.4s, v24.4s
        add	v9.4s, v9.4s, v24.4s
        sqrdmulh	v27.4s, v14.4s, v0.s[1]
        mul	v24.4s, v14.4s, v0.s[0]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v14.4s, v10.4s, v24.4s
        add	v10.4s, v10.4s, v24.4s
        sqrdmulh	v27.4s, v15.4s, v0.s[1]
        mul	v24.4s, v15.4s, v0.s[0]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v15.4s, v11.4s, v24.4s
        add	v11.4s, v11.4s, v24.4s
        sqrdmulh	v27.4s, v10.4s, v0.s[3]
        mul	v24.4s, v10.4s, v0.s[2]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v10.4s, v8.4s, v24.4s
        add	v8.4s, v8.4s, v24.4s
        sqrdmulh	v27.4s, v11.4s, v0.s[3]
        mul	v24.4s, v11.4s, v0.s[2]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v11.4s, v9.4s, v24.4s
        add	v9.4s, v9.4s, v24.4s
        sqrdmulh	v27.4s, v14.4s, v1.s[1]
        mul	v24.4s, v14.4s, v1.s[0]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v14.4s, v12.4s, v24.4s
        add	v12.4s, v12.4s, v24.4s
        sqrdmulh	v27.4s, v15.4s, v1.s[1]
        mul	v24.4s, v15.4s, v1.s[0]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v15.4s, v13.4s, v24.4s
        add	v13.4s, v13.4s, v24.4s
        sqrdmulh	v27.4s, v9.4s, v1.s[3]
        mul	v24.4s, v9.4s, v1.s[2]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v9.4s, v8.4s, v24.4s
        add	v8.4s, v8.4s, v24.4s
        sqrdmulh	v27.4s, v11.4s, v2.s[1]
        mul	v24.4s, v11.4s, v2.s[0]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v11.4s, v10.4s, v24.4s
        add	v10.4s, v10.4s, v24.4s
        sqrdmulh	v27.4s, v13.4s, v2.s[3]
        mul	v24.4s, v13.4s, v2.s[2]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v13.4s, v12.4s, v24.4s
        add	v12.4s, v12.4s, v24.4s
        sqrdmulh	v27.4s, v15.4s, v3.s[1]
        mul	v24.4s, v15.4s, v3.s[0]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v15.4s, v14.4s, v24.4s
        add	v14.4s, v14.4s, v24.4s
        str	q8, [x0], #0x10
        str	q9, [x0, #0x70]
        str	q10, [x0, #0xf0]
        str	q11, [x0, #0x170]
        str	q12, [x0, #0x1f0]
        str	q13, [x0, #0x270]
        str	q14, [x0, #0x2f0]
        str	q15, [x0, #0x370]
        subs	x4, x4, #0x1
        cbnz	x4, Lntt_layer123_start
        mov	x0, x3
        mov	x4, #0x8                // =8

Lntt_layer45678_start:
        ldr	q8, [x0]
        ldr	q9, [x0, #0x10]
        ldr	q10, [x0, #0x20]
        ldr	q11, [x0, #0x30]
        ldr	q12, [x0, #0x40]
        ldr	q13, [x0, #0x50]
        ldr	q14, [x0, #0x60]
        ldr	q15, [x0, #0x70]
        ldr	q0, [x1], #0x40
        ldur	q1, [x1, #-0x30]
        ldur	q2, [x1, #-0x20]
        ldur	q3, [x1, #-0x10]
        sqrdmulh	v27.4s, v12.4s, v0.s[1]
        mul	v24.4s, v12.4s, v0.s[0]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v12.4s, v8.4s, v24.4s
        add	v8.4s, v8.4s, v24.4s
        sqrdmulh	v27.4s, v13.4s, v0.s[1]
        mul	v24.4s, v13.4s, v0.s[0]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v13.4s, v9.4s, v24.4s
        add	v9.4s, v9.4s, v24.4s
        sqrdmulh	v27.4s, v14.4s, v0.s[1]
        mul	v24.4s, v14.4s, v0.s[0]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v14.4s, v10.4s, v24.4s
        add	v10.4s, v10.4s, v24.4s
        sqrdmulh	v27.4s, v15.4s, v0.s[1]
        mul	v24.4s, v15.4s, v0.s[0]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v15.4s, v11.4s, v24.4s
        add	v11.4s, v11.4s, v24.4s
        sqrdmulh	v27.4s, v10.4s, v0.s[3]
        mul	v24.4s, v10.4s, v0.s[2]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v10.4s, v8.4s, v24.4s
        add	v8.4s, v8.4s, v24.4s
        sqrdmulh	v27.4s, v11.4s, v0.s[3]
        mul	v24.4s, v11.4s, v0.s[2]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v11.4s, v9.4s, v24.4s
        add	v9.4s, v9.4s, v24.4s
        sqrdmulh	v27.4s, v14.4s, v1.s[1]
        mul	v24.4s, v14.4s, v1.s[0]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v14.4s, v12.4s, v24.4s
        add	v12.4s, v12.4s, v24.4s
        sqrdmulh	v27.4s, v15.4s, v1.s[1]
        mul	v24.4s, v15.4s, v1.s[0]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v15.4s, v13.4s, v24.4s
        add	v13.4s, v13.4s, v24.4s
        sqrdmulh	v27.4s, v9.4s, v1.s[3]
        mul	v24.4s, v9.4s, v1.s[2]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v9.4s, v8.4s, v24.4s
        add	v8.4s, v8.4s, v24.4s
        sqrdmulh	v27.4s, v11.4s, v2.s[1]
        mul	v24.4s, v11.4s, v2.s[0]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v11.4s, v10.4s, v24.4s
        add	v10.4s, v10.4s, v24.4s
        sqrdmulh	v27.4s, v13.4s, v2.s[3]
        mul	v24.4s, v13.4s, v2.s[2]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v13.4s, v12.4s, v24.4s
        add	v12.4s, v12.4s, v24.4s
        sqrdmulh	v27.4s, v15.4s, v3.s[1]
        mul	v24.4s, v15.4s, v3.s[0]
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v15.4s, v14.4s, v24.4s
        add	v14.4s, v14.4s, v24.4s
        trn1	v25.4s, v8.4s, v9.4s
        trn2	v26.4s, v8.4s, v9.4s
        trn1	v27.4s, v10.4s, v11.4s
        trn2	v28.4s, v10.4s, v11.4s
        trn2	v10.2d, v25.2d, v27.2d
        trn2	v11.2d, v26.2d, v28.2d
        trn1	v8.2d, v25.2d, v27.2d
        trn1	v9.2d, v26.2d, v28.2d
        trn1	v25.4s, v12.4s, v13.4s
        trn2	v26.4s, v12.4s, v13.4s
        trn1	v27.4s, v14.4s, v15.4s
        trn2	v28.4s, v14.4s, v15.4s
        trn2	v14.2d, v25.2d, v27.2d
        trn2	v15.2d, v26.2d, v28.2d
        trn1	v12.2d, v25.2d, v27.2d
        trn1	v13.2d, v26.2d, v28.2d
        ldr	q0, [x2], #0xc0
        ldur	q4, [x2, #-0xb0]
        ldur	q1, [x2, #-0xa0]
        ldur	q5, [x2, #-0x90]
        ldur	q2, [x2, #-0x80]
        ldur	q6, [x2, #-0x70]
        sqrdmulh	v27.4s, v10.4s, v4.4s
        mul	v24.4s, v10.4s, v0.4s
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v10.4s, v8.4s, v24.4s
        add	v8.4s, v8.4s, v24.4s
        sqrdmulh	v27.4s, v11.4s, v4.4s
        mul	v24.4s, v11.4s, v0.4s
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v11.4s, v9.4s, v24.4s
        add	v9.4s, v9.4s, v24.4s
        sqrdmulh	v27.4s, v9.4s, v5.4s
        mul	v24.4s, v9.4s, v1.4s
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v9.4s, v8.4s, v24.4s
        add	v8.4s, v8.4s, v24.4s
        sqrdmulh	v27.4s, v11.4s, v6.4s
        mul	v24.4s, v11.4s, v2.4s
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v11.4s, v10.4s, v24.4s
        add	v10.4s, v10.4s, v24.4s
        ldur	q0, [x2, #-0x60]
        ldur	q4, [x2, #-0x50]
        ldur	q1, [x2, #-0x40]
        ldur	q5, [x2, #-0x30]
        ldur	q2, [x2, #-0x20]
        ldur	q6, [x2, #-0x10]
        sqrdmulh	v27.4s, v14.4s, v4.4s
        mul	v24.4s, v14.4s, v0.4s
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v14.4s, v12.4s, v24.4s
        add	v12.4s, v12.4s, v24.4s
        sqrdmulh	v27.4s, v15.4s, v4.4s
        mul	v24.4s, v15.4s, v0.4s
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v15.4s, v13.4s, v24.4s
        add	v13.4s, v13.4s, v24.4s
        sqrdmulh	v27.4s, v13.4s, v5.4s
        mul	v24.4s, v13.4s, v1.4s
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v13.4s, v12.4s, v24.4s
        add	v12.4s, v12.4s, v24.4s
        sqrdmulh	v27.4s, v15.4s, v6.4s
        mul	v24.4s, v15.4s, v2.4s
        mls	v24.4s, v27.4s, v7.s[0]
        sub	v15.4s, v14.4s, v24.4s
        add	v14.4s, v14.4s, v24.4s
        trn1	v25.4s, v8.4s, v9.4s
        trn2	v26.4s, v8.4s, v9.4s
        trn1	v27.4s, v10.4s, v11.4s
        trn2	v28.4s, v10.4s, v11.4s
        trn2	v10.2d, v25.2d, v27.2d
        trn2	v11.2d, v26.2d, v28.2d
        trn1	v8.2d, v25.2d, v27.2d
        trn1	v9.2d, v26.2d, v28.2d
        trn1	v25.4s, v12.4s, v13.4s
        trn2	v26.4s, v12.4s, v13.4s
        trn1	v27.4s, v14.4s, v15.4s
        trn2	v28.4s, v14.4s, v15.4s
        trn2	v14.2d, v25.2d, v27.2d
        trn2	v15.2d, v26.2d, v28.2d
        trn1	v12.2d, v25.2d, v27.2d
        trn1	v13.2d, v26.2d, v28.2d
        str	q8, [x0], #0x80
        stur	q9, [x0, #-0x70]
        stur	q10, [x0, #-0x60]
        stur	q11, [x0, #-0x50]
        stur	q12, [x0, #-0x40]
        stur	q13, [x0, #-0x30]
        stur	q14, [x0, #-0x20]
        stur	q15, [x0, #-0x10]
        subs	x4, x4, #0x1
        cbnz	x4, Lntt_layer45678_start
        ldp	d8, d9, [sp]
        .cfi_restore d8
        .cfi_restore d9
        ldp	d10, d11, [sp, #0x10]
        .cfi_restore d10
        .cfi_restore d11
        ldp	d12, d13, [sp, #0x20]
        .cfi_restore d12
        .cfi_restore d13
        ldp	d14, d15, [sp, #0x30]
        .cfi_restore d14
        .cfi_restore d15
        add	sp, sp, #0x40
        .cfi_adjust_cfa_offset -0x40
        ret
        .cfi_endproc

#endif /* MLD_ARITH_BACKEND_AARCH64 && !MLD_CONFIG_MULTILEVEL_NO_SHARED */
