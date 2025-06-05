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
    (defined(MLK_CONFIG_MULTILEVEL_WITH_SHARED) || MLKEM_K == 2)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/aarch64_opt/src/polyvec_basemul_acc_montgomery_cached_asm_k2.S using scripts/simpasm. Do not modify it directly.
 */


.text
.balign 4
.global MLK_ASM_NAMESPACE(polyvec_basemul_acc_montgomery_cached_asm_k2)
MLK_ASM_FN_SYMBOL(polyvec_basemul_acc_montgomery_cached_asm_k2)

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
        mov	x13, #0x10              // =16
        ldr	q9, [x4], #0x20
        ldur	q5, [x4, #-0x10]
        ldr	q11, [x5], #0x20
        uzp1	v23.8h, v9.8h, v5.8h
        uzp2	v9.8h, v9.8h, v5.8h
        ldr	q5, [x2], #0x20
        ldur	q7, [x5, #-0x10]
        ldur	q21, [x2, #-0x10]
        uzp2	v10.8h, v11.8h, v7.8h
        uzp1	v11.8h, v11.8h, v7.8h
        uzp1	v7.8h, v5.8h, v21.8h
        uzp2	v5.8h, v5.8h, v21.8h
        ldr	q21, [x1], #0x20
        ldur	q25, [x1, #-0x10]
        ld1	{ v6.8h }, [x3], #16
        uzp1	v26.8h, v21.8h, v25.8h
        uzp2	v21.8h, v21.8h, v25.8h
        smull	v25.4s, v26.4h, v5.4h
        smull2	v5.4s, v26.8h, v5.8h
        smull	v19.4s, v26.4h, v7.4h
        smull2	v26.4s, v26.8h, v7.8h
        smlal	v25.4s, v21.4h, v7.4h
        smlal2	v5.4s, v21.8h, v7.8h
        smlal	v19.4s, v21.4h, v6.4h
        smlal2	v26.4s, v21.8h, v6.8h
        smlal	v25.4s, v23.4h, v10.4h
        smlal2	v5.4s, v23.8h, v10.8h
        smlal	v19.4s, v23.4h, v11.4h
        smlal2	v26.4s, v23.8h, v11.8h
        ld1	{ v23.8h }, [x6], #16
        smlal	v25.4s, v9.4h, v11.4h
        smlal2	v5.4s, v9.8h, v11.8h
        smlal2	v26.4s, v9.8h, v23.8h
        smlal	v19.4s, v9.4h, v23.4h
        ldr	q9, [x4], #0x20
        uzp1	v11.8h, v25.8h, v5.8h
        uzp1	v23.8h, v19.8h, v26.8h
        mul	v11.8h, v11.8h, v2.8h
        mul	v23.8h, v23.8h, v2.8h
        ldr	q7, [x5], #0x20
        smlal2	v5.4s, v11.8h, v0.8h
        smlal	v25.4s, v11.4h, v0.4h
        ldr	q11, [x2], #0x20
        ldur	q21, [x2, #-0x10]
        ldur	q6, [x4, #-0x10]
        uzp1	v17.8h, v11.8h, v21.8h
        ldr	q10, [x1], #0x20
        ldur	q29, [x1, #-0x10]
        uzp2	v11.8h, v11.8h, v21.8h
        uzp1	v13.8h, v9.8h, v6.8h
        uzp1	v3.8h, v10.8h, v29.8h
        uzp2	v10.8h, v10.8h, v29.8h
        smull	v12.4s, v3.4h, v11.4h
        smull2	v11.4s, v3.8h, v11.8h
        ldur	q21, [x5, #-0x10]
        smlal	v12.4s, v10.4h, v17.4h
        smlal2	v11.4s, v10.8h, v17.8h
        uzp2	v29.8h, v7.8h, v21.8h
        uzp1	v15.8h, v7.8h, v21.8h
        smlal	v12.4s, v13.4h, v29.4h
        smlal2	v11.4s, v13.8h, v29.8h
        uzp2	v28.8h, v9.8h, v6.8h
        smlal2	v26.4s, v23.8h, v0.8h
        smlal	v12.4s, v28.4h, v15.4h
        smlal2	v11.4s, v28.8h, v15.8h
        smlal	v19.4s, v23.4h, v0.4h
        uzp2	v27.8h, v25.8h, v5.8h
        smull	v23.4s, v3.4h, v17.4h
        uzp1	v9.8h, v12.8h, v11.8h
        uzp2	v19.8h, v19.8h, v26.8h
        mul	v14.8h, v9.8h, v2.8h
        ld1	{ v22.8h }, [x6], #16
        zip2	v9.8h, v19.8h, v27.8h
        smlal2	v11.4s, v14.8h, v0.8h
        ld1	{ v4.8h }, [x3], #16
        sub	x13, x13, #0x2

polyvec_basemul_acc_montgomery_cached_k2_loop:
        smull2	v20.4s, v3.8h, v17.8h
        ldr	q18, [x4], #0x20
        ldr	q30, [x5], #0x20
        smlal2	v20.4s, v10.8h, v4.8h
        smlal	v12.4s, v14.4h, v0.4h
        smlal	v23.4s, v10.4h, v4.4h
        str	q9, [x0, #0x10]
        smlal2	v20.4s, v13.8h, v15.8h
        ldr	q8, [x2], #0x20
        smlal	v23.4s, v13.4h, v15.4h
        smlal2	v20.4s, v28.8h, v22.8h
        zip1	v26.8h, v19.8h, v27.8h
        ldur	q9, [x2, #-0x10]
        smlal	v23.4s, v28.4h, v22.4h
        uzp2	v27.8h, v12.8h, v11.8h
        uzp1	v17.8h, v8.8h, v9.8h
        uzp2	v4.8h, v8.8h, v9.8h
        uzp1	v5.8h, v23.8h, v20.8h
        str	q26, [x0], #0x20
        mul	v31.8h, v5.8h, v2.8h
        ldur	q19, [x4, #-0x10]
        ldr	q29, [x1], #0x20
        ldur	q12, [x1, #-0x10]
        smlal2	v20.4s, v31.8h, v0.8h
        uzp1	v13.8h, v18.8h, v19.8h
        uzp1	v3.8h, v29.8h, v12.8h
        uzp2	v10.8h, v29.8h, v12.8h
        smull	v12.4s, v3.4h, v4.4h
        smull2	v11.4s, v3.8h, v4.8h
        ldur	q5, [x5, #-0x10]
        smlal	v12.4s, v10.4h, v17.4h
        smlal2	v11.4s, v10.8h, v17.8h
        uzp2	v14.8h, v30.8h, v5.8h
        uzp1	v15.8h, v30.8h, v5.8h
        smlal	v12.4s, v13.4h, v14.4h
        smlal2	v11.4s, v13.8h, v14.8h
        uzp2	v28.8h, v18.8h, v19.8h
        smlal	v23.4s, v31.4h, v0.4h
        smlal	v12.4s, v28.4h, v15.4h
        smlal2	v11.4s, v28.8h, v15.8h
        ld1	{ v22.8h }, [x6], #16
        uzp2	v19.8h, v23.8h, v20.8h
        uzp1	v1.8h, v12.8h, v11.8h
        smull	v23.4s, v3.4h, v17.4h
        mul	v14.8h, v1.8h, v2.8h
        zip2	v9.8h, v19.8h, v27.8h
        ld1	{ v4.8h }, [x3], #16
        smlal2	v11.4s, v14.8h, v0.8h
        sub	x13, x13, #0x1
        cbnz	x13, polyvec_basemul_acc_montgomery_cached_k2_loop
        smull2	v5.4s, v3.8h, v17.8h
        smlal	v12.4s, v14.4h, v0.4h
        smlal	v23.4s, v10.4h, v4.4h
        str	q9, [x0, #0x10]
        smlal2	v5.4s, v10.8h, v4.8h
        uzp2	v11.8h, v12.8h, v11.8h
        zip1	v9.8h, v19.8h, v27.8h
        smlal	v23.4s, v13.4h, v15.4h
        smlal2	v5.4s, v13.8h, v15.8h
        str	q9, [x0], #0x20
        smlal	v23.4s, v28.4h, v22.4h
        smlal2	v5.4s, v28.8h, v22.8h
        uzp1	v9.8h, v23.8h, v5.8h
        mul	v9.8h, v9.8h, v2.8h
        smlal2	v5.4s, v9.8h, v0.8h
        smlal	v23.4s, v9.4h, v0.4h
        uzp2	v9.8h, v23.8h, v5.8h
        zip2	v5.8h, v9.8h, v11.8h
        zip1	v9.8h, v9.8h, v11.8h
        str	q5, [x0, #0x10]
        str	q9, [x0], #0x20
        ldp	d8, d9, [sp]
        ldp	d10, d11, [sp, #0x10]
        ldp	d12, d13, [sp, #0x20]
        ldp	d14, d15, [sp, #0x30]
        add	sp, sp, #0x40
        ret

#endif /* MLK_ARITH_BACKEND_AARCH64 && !MLK_CONFIG_MULTILEVEL_NO_SHARED && \
          (MLK_CONFIG_MULTILEVEL_WITH_SHARED || MLKEM_K == 2) */
