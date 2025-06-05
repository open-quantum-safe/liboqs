/*
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
 */

/* Re-implementation of asymmetric base multiplication following @[NeonNTT] */

#include "../../../common.h"
#if defined(MLK_ARITH_BACKEND_AARCH64) && \
    !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED) && \
    (defined(MLK_CONFIG_MULTILEVEL_WITH_SHARED) || MLKEM_K == 3)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/aarch64_opt/src/polyvec_basemul_acc_montgomery_cached_asm_k3.S using scripts/simpasm. Do not modify it directly.
 */


.text
.balign 4
.global MLK_ASM_NAMESPACE(polyvec_basemul_acc_montgomery_cached_asm_k3)
MLK_ASM_FN_SYMBOL(polyvec_basemul_acc_montgomery_cached_asm_k3)

        sub	sp, sp, #0x40
        stp	d8, d9, [sp]
        stp	d10, d11, [sp, #0x10]
        stp	d12, d13, [sp, #0x20]
        stp	d14, d15, [sp, #0x30]
        mov	w14, #0xd01             // =3329
        dup	v0.8h, w14
        mov	w14, #0xcff             // =3327
        dup	v2.8h, w14
        add	x4, x1, #0x200
        add	x5, x2, #0x200
        add	x6, x3, #0x100
        add	x7, x1, #0x400
        add	x8, x2, #0x400
        add	x9, x3, #0x200
        mov	x13, #0x10              // =16
        ldr	q7, [x2, #0x10]
        ldr	q20, [x2], #0x20
        ldr	q15, [x1, #0x10]
        uzp1	v8.8h, v20.8h, v7.8h
        uzp2	v7.8h, v20.8h, v7.8h
        ld1	{ v20.8h }, [x3], #16
        ldr	q30, [x1], #0x20
        ldr	q11, [x4], #0x20
        uzp1	v16.8h, v30.8h, v15.8h
        uzp2	v15.8h, v30.8h, v15.8h
        smull	v30.4s, v16.4h, v7.4h
        smull2	v7.4s, v16.8h, v7.8h
        smull	v9.4s, v16.4h, v8.4h
        smull2	v16.4s, v16.8h, v8.8h
        smlal	v30.4s, v15.4h, v8.4h
        smlal2	v7.4s, v15.8h, v8.8h
        smlal	v9.4s, v15.4h, v20.4h
        smlal2	v16.4s, v15.8h, v20.8h
        ldur	q20, [x4, #-0x10]
        ldr	q15, [x5], #0x20
        uzp1	v8.8h, v11.8h, v20.8h
        uzp2	v20.8h, v11.8h, v20.8h
        ldur	q11, [x5, #-0x10]
        ld1	{ v27.8h }, [x6], #16
        uzp1	v10.8h, v15.8h, v11.8h
        uzp2	v15.8h, v15.8h, v11.8h
        smlal	v9.4s, v8.4h, v10.4h
        smlal2	v16.4s, v8.8h, v10.8h
        smlal	v30.4s, v8.4h, v15.4h
        smlal2	v7.4s, v8.8h, v15.8h
        smlal	v9.4s, v20.4h, v27.4h
        smlal2	v16.4s, v20.8h, v27.8h
        smlal	v30.4s, v20.4h, v10.4h
        smlal2	v7.4s, v20.8h, v10.8h
        ldr	q20, [x7], #0x20
        ldur	q15, [x7, #-0x10]
        ldr	q8, [x8], #0x20
        uzp1	v11.8h, v20.8h, v15.8h
        uzp2	v20.8h, v20.8h, v15.8h
        ldur	q15, [x8, #-0x10]
        ld1	{ v27.8h }, [x9], #16
        uzp1	v10.8h, v8.8h, v15.8h
        uzp2	v15.8h, v8.8h, v15.8h
        smlal	v9.4s, v11.4h, v10.4h
        smlal2	v16.4s, v11.8h, v10.8h
        smlal	v30.4s, v11.4h, v15.4h
        smlal2	v7.4s, v11.8h, v15.8h
        smlal	v9.4s, v20.4h, v27.4h
        smlal2	v16.4s, v20.8h, v27.8h
        smlal	v30.4s, v20.4h, v10.4h
        smlal2	v7.4s, v20.8h, v10.8h
        ldr	q15, [x2], #0x20
        uzp1	v20.8h, v9.8h, v16.8h
        uzp1	v8.8h, v30.8h, v7.8h
        mul	v20.8h, v20.8h, v2.8h
        mul	v8.8h, v8.8h, v2.8h
        ldr	q21, [x4], #0x20
        smlal	v9.4s, v20.4h, v0.4h
        smlal2	v16.4s, v20.8h, v0.8h
        smlal	v30.4s, v8.4h, v0.4h
        smlal2	v7.4s, v8.8h, v0.8h
        ldur	q6, [x4, #-0x10]
        uzp2	v27.8h, v9.8h, v16.8h
        uzp2	v10.8h, v30.8h, v7.8h
        ldur	q16, [x2, #-0x10]
        ldr	q30, [x1, #0x10]
        ld1	{ v9.8h }, [x3], #16
        ldr	q1, [x5], #0x20
        ldur	q12, [x5, #-0x10]
        ld1	{ v24.8h }, [x6], #16
        ldr	q19, [x7], #0x20
        ldur	q31, [x7, #-0x10]
        ldr	q17, [x8], #0x20
        ldur	q18, [x8, #-0x10]
        ld1	{ v25.8h }, [x9], #16
        sub	x13, x13, #0x2

polyvec_basemul_acc_montgomery_cached_k3_loop:
        ldr	q20, [x1], #0x20
        uzp1	v7.8h, v15.8h, v16.8h
        uzp2	v15.8h, v15.8h, v16.8h
        uzp1	v8.8h, v20.8h, v30.8h
        uzp2	v20.8h, v20.8h, v30.8h
        smull	v30.4s, v8.4h, v15.4h
        smull2	v15.4s, v8.8h, v15.8h
        smull	v11.4s, v8.4h, v7.4h
        smull2	v8.4s, v8.8h, v7.8h
        smlal	v30.4s, v20.4h, v7.4h
        smlal2	v15.4s, v20.8h, v7.8h
        smlal	v11.4s, v20.4h, v9.4h
        smlal2	v8.4s, v20.8h, v9.8h
        uzp1	v7.8h, v21.8h, v6.8h
        uzp2	v20.8h, v21.8h, v6.8h
        uzp1	v16.8h, v1.8h, v12.8h
        uzp2	v9.8h, v1.8h, v12.8h
        smlal	v11.4s, v7.4h, v16.4h
        smlal2	v8.4s, v7.8h, v16.8h
        smlal	v30.4s, v7.4h, v9.4h
        smlal2	v15.4s, v7.8h, v9.8h
        smlal	v11.4s, v20.4h, v24.4h
        smlal2	v8.4s, v20.8h, v24.8h
        smlal	v30.4s, v20.4h, v16.4h
        smlal2	v15.4s, v20.8h, v16.8h
        uzp1	v7.8h, v19.8h, v31.8h
        uzp2	v20.8h, v19.8h, v31.8h
        uzp1	v16.8h, v17.8h, v18.8h
        uzp2	v9.8h, v17.8h, v18.8h
        smlal	v11.4s, v7.4h, v16.4h
        smlal2	v8.4s, v7.8h, v16.8h
        smlal	v30.4s, v7.4h, v9.4h
        smlal2	v15.4s, v7.8h, v9.8h
        smlal	v11.4s, v20.4h, v25.4h
        smlal2	v8.4s, v20.8h, v25.8h
        smlal	v30.4s, v20.4h, v16.4h
        smlal2	v15.4s, v20.8h, v16.8h
        ldr	q16, [x2, #0x10]
        uzp1	v7.8h, v11.8h, v8.8h
        uzp1	v20.8h, v30.8h, v15.8h
        mul	v7.8h, v7.8h, v2.8h
        mul	v20.8h, v20.8h, v2.8h
        zip2	v9.8h, v27.8h, v10.8h
        zip1	v27.8h, v27.8h, v10.8h
        smlal	v11.4s, v7.4h, v0.4h
        smlal2	v8.4s, v7.8h, v0.8h
        smlal	v30.4s, v20.4h, v0.4h
        smlal2	v15.4s, v20.8h, v0.8h
        str	q27, [x0], #0x20
        uzp2	v27.8h, v11.8h, v8.8h
        stur	q9, [x0, #-0x10]
        uzp2	v10.8h, v30.8h, v15.8h
        ldr	q30, [x1, #0x10]
        ldr	q15, [x2], #0x20
        ld1	{ v9.8h }, [x3], #16
        ldr	q21, [x4], #0x20
        ldur	q6, [x4, #-0x10]
        ldr	q1, [x5], #0x20
        ldur	q12, [x5, #-0x10]
        ld1	{ v24.8h }, [x6], #16
        ldr	q19, [x7], #0x20
        ldur	q31, [x7, #-0x10]
        ldr	q17, [x8], #0x20
        ldur	q18, [x8, #-0x10]
        ld1	{ v25.8h }, [x9], #16
        sub	x13, x13, #0x1
        cbnz	x13, polyvec_basemul_acc_montgomery_cached_k3_loop
        ldr	q7, [x1], #0x20
        uzp1	v20.8h, v15.8h, v16.8h
        uzp2	v15.8h, v15.8h, v16.8h
        uzp1	v23.8h, v7.8h, v30.8h
        uzp2	v11.8h, v7.8h, v30.8h
        smull2	v8.4s, v23.8h, v20.8h
        smull	v5.4s, v23.4h, v20.4h
        smull2	v30.4s, v23.8h, v15.8h
        uzp1	v28.8h, v1.8h, v12.8h
        smlal2	v8.4s, v11.8h, v9.8h
        smlal	v5.4s, v11.4h, v9.4h
        uzp1	v3.8h, v21.8h, v6.8h
        smull	v16.4s, v23.4h, v15.4h
        smlal2	v8.4s, v3.8h, v28.8h
        smlal	v5.4s, v3.4h, v28.4h
        uzp2	v29.8h, v21.8h, v6.8h
        uzp1	v7.8h, v17.8h, v18.8h
        smlal2	v8.4s, v29.8h, v24.8h
        uzp1	v14.8h, v19.8h, v31.8h
        smlal	v16.4s, v11.4h, v20.4h
        smlal2	v30.4s, v11.8h, v20.8h
        smlal2	v8.4s, v14.8h, v7.8h
        uzp2	v20.8h, v1.8h, v12.8h
        uzp2	v21.8h, v19.8h, v31.8h
        smlal2	v30.4s, v3.8h, v20.8h
        smlal	v16.4s, v3.4h, v20.4h
        smlal	v5.4s, v29.4h, v24.4h
        uzp2	v9.8h, v17.8h, v18.8h
        smlal2	v30.4s, v29.8h, v28.8h
        smlal	v16.4s, v29.4h, v28.4h
        smlal	v5.4s, v14.4h, v7.4h
        smlal2	v8.4s, v21.8h, v25.8h
        smlal2	v30.4s, v14.8h, v9.8h
        smlal	v16.4s, v14.4h, v9.4h
        smlal	v5.4s, v21.4h, v25.4h
        zip1	v20.8h, v27.8h, v10.8h
        smlal2	v30.4s, v21.8h, v7.8h
        smlal	v16.4s, v21.4h, v7.4h
        uzp1	v7.8h, v5.8h, v8.8h
        str	q20, [x0], #0x20
        mul	v15.8h, v7.8h, v2.8h
        uzp1	v7.8h, v16.8h, v30.8h
        zip2	v31.8h, v27.8h, v10.8h
        mul	v20.8h, v7.8h, v2.8h
        smlal	v5.4s, v15.4h, v0.4h
        smlal2	v8.4s, v15.8h, v0.8h
        stur	q31, [x0, #-0x10]
        smlal2	v30.4s, v20.8h, v0.8h
        smlal	v16.4s, v20.4h, v0.4h
        uzp2	v15.8h, v5.8h, v8.8h
        uzp2	v20.8h, v16.8h, v30.8h
        zip1	v7.8h, v15.8h, v20.8h
        zip2	v20.8h, v15.8h, v20.8h
        str	q7, [x0], #0x20
        stur	q20, [x0, #-0x10]
        ldp	d8, d9, [sp]
        ldp	d10, d11, [sp, #0x10]
        ldp	d12, d13, [sp, #0x20]
        ldp	d14, d15, [sp, #0x30]
        add	sp, sp, #0x40
        ret

#endif /* MLK_ARITH_BACKEND_AARCH64 && !MLK_CONFIG_MULTILEVEL_NO_SHARED && \
          (MLK_CONFIG_MULTILEVEL_WITH_SHARED || MLKEM_K == 3) */
