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
    (defined(MLK_CONFIG_MULTILEVEL_WITH_SHARED) || MLKEM_K == 4)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/aarch64_opt/src/polyvec_basemul_acc_montgomery_cached_asm_k4.S using scripts/simpasm. Do not modify it directly.
 */


.text
.balign 4
.global MLK_ASM_NAMESPACE(polyvec_basemul_acc_montgomery_cached_asm_k4)
MLK_ASM_FN_SYMBOL(polyvec_basemul_acc_montgomery_cached_asm_k4)

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
        add	x10, x1, #0x600
        add	x11, x2, #0x600
        add	x12, x3, #0x300
        mov	x13, #0x10              // =16
        ldr	q23, [x2, #0x10]
        ldr	q19, [x2], #0x20
        ldr	q17, [x5], #0x20
        uzp2	v13.8h, v19.8h, v23.8h
        uzp1	v19.8h, v19.8h, v23.8h
        ldur	q23, [x5, #-0x10]
        ldr	q30, [x1, #0x10]
        uzp2	v9.8h, v17.8h, v23.8h
        uzp1	v23.8h, v17.8h, v23.8h
        ldr	q17, [x1], #0x20
        ldr	q10, [x7, #0x10]
        uzp1	v12.8h, v17.8h, v30.8h
        uzp2	v17.8h, v17.8h, v30.8h
        smull2	v30.4s, v12.8h, v13.8h
        smull	v13.4s, v12.4h, v13.4h
        smull2	v22.4s, v12.8h, v19.8h
        smull	v12.4s, v12.4h, v19.4h
        smlal2	v30.4s, v17.8h, v19.8h
        smlal	v13.4s, v17.4h, v19.4h
        ldr	q19, [x4], #0x20
        ldur	q16, [x4, #-0x10]
        ld1	{ v8.8h }, [x3], #16
        uzp1	v26.8h, v19.8h, v16.8h
        uzp2	v19.8h, v19.8h, v16.8h
        smlal2	v30.4s, v26.8h, v9.8h
        smlal	v13.4s, v26.4h, v9.4h
        smlal2	v22.4s, v17.8h, v8.8h
        smlal	v12.4s, v17.4h, v8.4h
        smlal2	v30.4s, v19.8h, v23.8h
        smlal	v13.4s, v19.4h, v23.4h
        smlal2	v22.4s, v26.8h, v23.8h
        smlal	v12.4s, v26.4h, v23.4h
        ldr	q23, [x7], #0x20
        ldr	q17, [x8, #0x10]
        uzp1	v9.8h, v23.8h, v10.8h
        uzp2	v23.8h, v23.8h, v10.8h
        ldr	q10, [x10], #0x20
        ldur	q16, [x10, #-0x10]
        ld1	{ v8.8h }, [x12], #16
        uzp1	v26.8h, v10.8h, v16.8h
        uzp2	v10.8h, v10.8h, v16.8h
        ld1	{ v16.8h }, [x6], #16
        ldr	q3, [x11, #0x10]
        smlal2	v22.4s, v19.8h, v16.8h
        smlal	v12.4s, v19.4h, v16.4h
        ldr	q19, [x11], #0x20
        ld1	{ v16.8h }, [x9], #16
        uzp1	v4.8h, v19.8h, v3.8h
        uzp2	v19.8h, v19.8h, v3.8h
        ldr	q3, [x8], #0x20
        ldr	q31, [x2], #0x20
        uzp1	v6.8h, v3.8h, v17.8h
        uzp2	v17.8h, v3.8h, v17.8h
        smlal2	v22.4s, v9.8h, v6.8h
        smlal2	v30.4s, v9.8h, v17.8h
        smlal	v13.4s, v9.4h, v17.4h
        smlal	v12.4s, v9.4h, v6.4h
        smlal2	v22.4s, v23.8h, v16.8h
        smlal2	v30.4s, v23.8h, v6.8h
        smlal	v13.4s, v23.4h, v6.4h
        smlal	v12.4s, v23.4h, v16.4h
        smlal2	v22.4s, v26.8h, v4.8h
        smlal2	v30.4s, v26.8h, v19.8h
        smlal	v13.4s, v26.4h, v19.4h
        smlal	v12.4s, v26.4h, v4.4h
        smlal2	v22.4s, v10.8h, v8.8h
        smlal2	v30.4s, v10.8h, v4.8h
        smlal	v13.4s, v10.4h, v4.4h
        smlal	v12.4s, v10.4h, v8.4h
        ldur	q19, [x2, #-0x10]
        uzp1	v23.8h, v13.8h, v30.8h
        uzp1	v17.8h, v12.8h, v22.8h
        mul	v23.8h, v23.8h, v2.8h
        uzp2	v21.8h, v31.8h, v19.8h
        uzp1	v19.8h, v31.8h, v19.8h
        mul	v17.8h, v17.8h, v2.8h
        smlal	v13.4s, v23.4h, v0.4h
        smlal2	v30.4s, v23.8h, v0.8h
        ldr	q23, [x5], #0x20
        smlal2	v22.4s, v17.8h, v0.8h
        uzp2	v15.8h, v13.8h, v30.8h
        smlal	v12.4s, v17.4h, v0.4h
        ldur	q17, [x5, #-0x10]
        ldr	q13, [x1, #0x10]
        uzp2	v27.8h, v23.8h, v17.8h
        uzp1	v28.8h, v23.8h, v17.8h
        uzp2	v7.8h, v12.8h, v22.8h
        ldr	q23, [x1], #0x20
        zip1	v5.8h, v7.8h, v15.8h
        ldr	q3, [x7, #0x10]
        uzp1	v31.8h, v23.8h, v13.8h
        uzp2	v16.8h, v23.8h, v13.8h
        smull2	v24.4s, v31.8h, v21.8h
        ldr	q6, [x8, #0x10]
        ldr	q23, [x10], #0x20
        smlal2	v24.4s, v16.8h, v19.8h
        ldur	q17, [x10, #-0x10]
        ld1	{ v22.8h }, [x12], #16
        uzp1	v30.8h, v23.8h, v17.8h
        uzp2	v11.8h, v23.8h, v17.8h
        ldr	q23, [x4], #0x20
        ldur	q17, [x4, #-0x10]
        ldr	q4, [x7], #0x20
        uzp1	v20.8h, v23.8h, v17.8h
        uzp2	v26.8h, v23.8h, v17.8h
        uzp1	v9.8h, v4.8h, v3.8h
        smlal2	v24.4s, v20.8h, v27.8h
        ld1	{ v8.8h }, [x6], #16
        ldr	q25, [x11, #0x10]
        ldr	q29, [x11], #0x20
        ld1	{ v12.8h }, [x9], #16
        uzp1	v10.8h, v29.8h, v25.8h
        ldr	q14, [x8], #0x20
        ld1	{ v23.8h }, [x3], #16
        sub	x13, x13, #0x2

polyvec_basemul_acc_montgomery_cached_k4_loop:
        smlal2	v24.4s, v26.8h, v28.8h
        uzp2	v4.8h, v4.8h, v3.8h
        smull2	v13.4s, v31.8h, v19.8h
        ldr	q3, [x2], #0x20
        uzp2	v1.8h, v29.8h, v25.8h
        smlal2	v13.4s, v16.8h, v23.8h
        ldur	q17, [x2, #-0x10]
        smull	v18.4s, v31.4h, v19.4h
        smlal2	v13.4s, v20.8h, v28.8h
        smull	v29.4s, v31.4h, v21.4h
        ldr	q21, [x5], #0x20
        smlal2	v13.4s, v26.8h, v8.8h
        smlal	v29.4s, v16.4h, v19.4h
        ldur	q19, [x5, #-0x10]
        smlal	v18.4s, v16.4h, v23.4h
        smlal	v29.4s, v20.4h, v27.4h
        uzp1	v31.8h, v14.8h, v6.8h
        uzp2	v27.8h, v21.8h, v19.8h
        smlal	v18.4s, v20.4h, v28.4h
        ldr	q25, [x1, #0x10]
        smlal	v29.4s, v26.4h, v28.4h
        smlal	v18.4s, v26.4h, v8.4h
        uzp2	v26.8h, v14.8h, v6.8h
        smlal2	v13.4s, v9.8h, v31.8h
        smlal2	v24.4s, v9.8h, v26.8h
        smlal	v29.4s, v9.4h, v26.4h
        smlal	v18.4s, v9.4h, v31.4h
        smlal2	v13.4s, v4.8h, v12.8h
        smlal2	v24.4s, v4.8h, v31.8h
        smlal	v29.4s, v4.4h, v31.4h
        smlal	v18.4s, v4.4h, v12.4h
        smlal2	v13.4s, v30.8h, v10.8h
        smlal2	v24.4s, v30.8h, v1.8h
        smlal	v29.4s, v30.4h, v1.4h
        smlal	v18.4s, v30.4h, v10.4h
        smlal2	v13.4s, v11.8h, v22.8h
        smlal2	v24.4s, v11.8h, v10.8h
        smlal	v29.4s, v11.4h, v10.4h
        smlal	v18.4s, v11.4h, v22.4h
        ldr	q22, [x1], #0x20
        uzp1	v31.8h, v29.8h, v24.8h
        uzp1	v28.8h, v21.8h, v19.8h
        mul	v19.8h, v31.8h, v2.8h
        uzp1	v31.8h, v22.8h, v25.8h
        uzp2	v16.8h, v22.8h, v25.8h
        uzp2	v21.8h, v3.8h, v17.8h
        smlal	v29.4s, v19.4h, v0.4h
        smlal2	v24.4s, v19.8h, v0.8h
        uzp1	v19.8h, v3.8h, v17.8h
        uzp1	v26.8h, v18.8h, v13.8h
        zip2	v14.8h, v7.8h, v15.8h
        mul	v23.8h, v26.8h, v2.8h
        uzp2	v15.8h, v29.8h, v24.8h
        smull2	v24.4s, v31.8h, v21.8h
        str	q14, [x0, #0x10]
        ldr	q3, [x7, #0x10]
        ldr	q6, [x8, #0x10]
        ldr	q8, [x10], #0x20
        ldur	q26, [x10, #-0x10]
        ld1	{ v22.8h }, [x12], #16
        uzp1	v30.8h, v8.8h, v26.8h
        uzp2	v11.8h, v8.8h, v26.8h
        ldr	q8, [x4], #0x20
        ldur	q26, [x4, #-0x10]
        ldr	q4, [x7], #0x20
        uzp1	v20.8h, v8.8h, v26.8h
        uzp2	v26.8h, v8.8h, v26.8h
        ld1	{ v8.8h }, [x6], #16
        uzp1	v9.8h, v4.8h, v3.8h
        ldr	q25, [x11, #0x10]
        ldr	q29, [x11], #0x20
        ld1	{ v12.8h }, [x9], #16
        ldr	q14, [x8], #0x20
        smlal2	v24.4s, v16.8h, v19.8h
        smlal2	v13.4s, v23.8h, v0.8h
        smlal	v18.4s, v23.4h, v0.4h
        ld1	{ v23.8h }, [x3], #16
        smlal2	v24.4s, v20.8h, v27.8h
        uzp2	v7.8h, v18.8h, v13.8h
        uzp1	v10.8h, v29.8h, v25.8h
        str	q5, [x0], #0x20
        zip1	v5.8h, v7.8h, v15.8h
        sub	x13, x13, #0x1
        cbnz	x13, polyvec_basemul_acc_montgomery_cached_k4_loop
        smull2	v17.4s, v31.8h, v19.8h
        uzp2	v1.8h, v14.8h, v6.8h
        smull	v18.4s, v31.4h, v21.4h
        smlal2	v24.4s, v26.8h, v28.8h
        smlal2	v17.4s, v16.8h, v23.8h
        smull	v21.4s, v31.4h, v19.4h
        smlal	v18.4s, v16.4h, v19.4h
        uzp2	v31.8h, v4.8h, v3.8h
        uzp1	v3.8h, v14.8h, v6.8h
        smlal	v21.4s, v16.4h, v23.4h
        smlal	v18.4s, v20.4h, v27.4h
        uzp2	v14.8h, v29.8h, v25.8h
        smlal2	v17.4s, v20.8h, v28.8h
        smlal	v21.4s, v20.4h, v28.4h
        smlal	v18.4s, v26.4h, v28.4h
        smlal2	v24.4s, v9.8h, v1.8h
        smlal2	v17.4s, v26.8h, v8.8h
        smlal	v21.4s, v26.4h, v8.4h
        smlal	v18.4s, v9.4h, v1.4h
        smlal2	v24.4s, v31.8h, v3.8h
        smlal2	v17.4s, v9.8h, v3.8h
        smlal	v21.4s, v9.4h, v3.4h
        smlal	v18.4s, v31.4h, v3.4h
        smlal2	v24.4s, v30.8h, v14.8h
        smlal2	v17.4s, v31.8h, v12.8h
        smlal	v21.4s, v31.4h, v12.4h
        smlal	v18.4s, v30.4h, v14.4h
        smlal2	v24.4s, v11.8h, v10.8h
        smlal2	v17.4s, v30.8h, v10.8h
        smlal	v21.4s, v30.4h, v10.4h
        smlal	v18.4s, v11.4h, v10.4h
        zip2	v19.8h, v7.8h, v15.8h
        smlal2	v17.4s, v11.8h, v22.8h
        smlal	v21.4s, v11.4h, v22.4h
        uzp1	v23.8h, v18.8h, v24.8h
        str	q19, [x0, #0x10]
        mul	v19.8h, v23.8h, v2.8h
        uzp1	v23.8h, v21.8h, v17.8h
        str	q5, [x0], #0x20
        mul	v26.8h, v23.8h, v2.8h
        smlal	v18.4s, v19.4h, v0.4h
        smlal2	v24.4s, v19.8h, v0.8h
        smlal	v21.4s, v26.4h, v0.4h
        smlal2	v17.4s, v26.8h, v0.8h
        uzp2	v13.8h, v18.8h, v24.8h
        uzp2	v19.8h, v21.8h, v17.8h
        zip1	v23.8h, v19.8h, v13.8h
        zip2	v19.8h, v19.8h, v13.8h
        str	q23, [x0], #0x20
        stur	q19, [x0, #-0x10]
        ldp	d8, d9, [sp]
        ldp	d10, d11, [sp, #0x10]
        ldp	d12, d13, [sp, #0x20]
        ldp	d14, d15, [sp, #0x30]
        add	sp, sp, #0x40
        ret

#endif /* MLK_ARITH_BACKEND_AARCH64 && !MLK_CONFIG_MULTILEVEL_NO_SHARED && \
          (MLK_CONFIG_MULTILEVEL_WITH_SHARED || MLKEM_K == 4) */
