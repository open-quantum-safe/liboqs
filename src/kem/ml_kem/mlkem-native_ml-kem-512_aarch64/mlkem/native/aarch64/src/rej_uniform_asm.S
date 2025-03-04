/*
 * Copyright (c) 2024-2025 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

/*************************************************
 * Name:        mlk_rej_uniform_asm
 *
 * Description: Run rejection sampling on uniform random bytes to generate
 *              uniform random integers mod q
 *
 * Arguments:   - int16_t *r:          pointer to output buffer of MLKEM_N
 *                                     16-bit coefficients.
 *              - const uint8_t *buf:  pointer to input buffer
 *                                     (assumed to be uniform random bytes)
 *              - unsigned buflen:     length of input buffer in bytes.
 *                                     Must be a multiple of 24.
 *
 * Returns number of sampled 16-bit integers (at most MLKEM_N).
 **************************************************/
#include "../../../common.h"
#if (defined(MLK_ARITH_BACKEND_AARCH64_CLEAN) || \
        defined(MLK_ARITH_BACKEND_AARCH64_OPT))  \
    && !defined(MLK_MULTILEVEL_BUILD_NO_SHARED)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/aarch64_opt/src/rej_uniform_asm.S using scripts/simpasm. Do not modify it directly.
 */


.text
.balign 4
.global MLK_ASM_NAMESPACE(rej_uniform_asm)
MLK_ASM_FN_SYMBOL(rej_uniform_asm)

        sub	sp, sp, #0x240
        mov	x7, #0x1                // =1
        movk	x7, #0x2, lsl #16
        movk	x7, #0x4, lsl #32
        movk	x7, #0x8, lsl #48
        mov	v31.d[0], x7
        mov	x7, #0x10               // =16
        movk	x7, #0x20, lsl #16
        movk	x7, #0x40, lsl #32
        movk	x7, #0x80, lsl #48
        mov	v31.d[1], x7
        mov	w11, #0xd01             // =3329
        dup	v30.8h, w11
        mov	x8, sp
        mov	x7, x8
        mov	w9, #0x0                // =0
        mov	w4, #0x100              // =256
        cmp	w2, #0x30
        b.lo	loop48_end

loop48:
        cmp	w9, w4
        b.hs	memory_copy
        sub	w2, w2, #0x30
        ld3	{ v0.16b, v1.16b, v2.16b }, [x1], #48
        zip1	v4.16b, v0.16b, v1.16b
        zip2	v5.16b, v0.16b, v1.16b
        zip1	v6.16b, v1.16b, v2.16b
        zip2	v7.16b, v1.16b, v2.16b
        bic	v4.8h, #0xf0, lsl #8
        bic	v5.8h, #0xf0, lsl #8
        ushr	v6.8h, v6.8h, #0x4
        ushr	v7.8h, v7.8h, #0x4
        zip1	v16.8h, v4.8h, v6.8h
        zip2	v17.8h, v4.8h, v6.8h
        zip1	v18.8h, v5.8h, v7.8h
        zip2	v19.8h, v5.8h, v7.8h
        cmhi	v4.8h, v30.8h, v16.8h
        cmhi	v5.8h, v30.8h, v17.8h
        cmhi	v6.8h, v30.8h, v18.8h
        cmhi	v7.8h, v30.8h, v19.8h
        and	v4.16b, v4.16b, v31.16b
        and	v5.16b, v5.16b, v31.16b
        and	v6.16b, v6.16b, v31.16b
        and	v7.16b, v7.16b, v31.16b
        uaddlv	s20, v4.8h
        uaddlv	s21, v5.8h
        uaddlv	s22, v6.8h
        uaddlv	s23, v7.8h
        fmov	w12, s20
        fmov	w13, s21
        fmov	w14, s22
        fmov	w15, s23
        ldr	q24, [x3, w12, uxtw #4]
        ldr	q25, [x3, w13, uxtw #4]
        ldr	q26, [x3, w14, uxtw #4]
        ldr	q27, [x3, w15, uxtw #4]
        cnt	v4.16b, v4.16b
        cnt	v5.16b, v5.16b
        cnt	v6.16b, v6.16b
        cnt	v7.16b, v7.16b
        uaddlv	s20, v4.8h
        uaddlv	s21, v5.8h
        uaddlv	s22, v6.8h
        uaddlv	s23, v7.8h
        fmov	w12, s20
        fmov	w13, s21
        fmov	w14, s22
        fmov	w15, s23
        tbl	v16.16b, { v16.16b }, v24.16b
        tbl	v17.16b, { v17.16b }, v25.16b
        tbl	v18.16b, { v18.16b }, v26.16b
        tbl	v19.16b, { v19.16b }, v27.16b
        str	q16, [x7]
        add	x7, x7, w12, uxtw #1
        str	q17, [x7]
        add	x7, x7, w13, uxtw #1
        str	q18, [x7]
        add	x7, x7, w14, uxtw #1
        str	q19, [x7]
        add	x7, x7, w15, uxtw #1
        add	w12, w12, w13
        add	w14, w14, w15
        add	w9, w9, w12
        add	w9, w9, w14
        cmp	w2, #0x30
        b.hs	loop48

loop48_end:
        cmp	w9, w4
        b.hs	memory_copy
        cmp	w2, #0x18
        b.lo	memory_copy
        sub	w2, w2, #0x18
        ld3	{ v0.8b, v1.8b, v2.8b }, [x1], #24
        zip1	v4.16b, v0.16b, v1.16b
        zip1	v5.16b, v1.16b, v2.16b
        bic	v4.8h, #0xf0, lsl #8
        ushr	v5.8h, v5.8h, #0x4
        zip1	v16.8h, v4.8h, v5.8h
        zip2	v17.8h, v4.8h, v5.8h
        cmhi	v4.8h, v30.8h, v16.8h
        cmhi	v5.8h, v30.8h, v17.8h
        and	v4.16b, v4.16b, v31.16b
        and	v5.16b, v5.16b, v31.16b
        uaddlv	s20, v4.8h
        uaddlv	s21, v5.8h
        fmov	w12, s20
        fmov	w13, s21
        ldr	q24, [x3, w12, uxtw #4]
        ldr	q25, [x3, w13, uxtw #4]
        cnt	v4.16b, v4.16b
        cnt	v5.16b, v5.16b
        uaddlv	s20, v4.8h
        uaddlv	s21, v5.8h
        fmov	w12, s20
        fmov	w13, s21
        tbl	v16.16b, { v16.16b }, v24.16b
        tbl	v17.16b, { v17.16b }, v25.16b
        str	q16, [x7]
        add	x7, x7, w12, uxtw #1
        str	q17, [x7]
        add	x7, x7, w13, uxtw #1
        add	w9, w9, w12
        add	w9, w9, w13

memory_copy:
        cmp	w9, w4
        csel	w9, w9, w4, lo
        mov	w11, #0x0               // =0
        mov	x7, x8

final_copy:
        ldr	q16, [x7], #0x40
        ldur	q17, [x7, #-0x30]
        ldur	q18, [x7, #-0x20]
        ldur	q19, [x7, #-0x10]
        str	q16, [x0], #0x40
        stur	q17, [x0, #-0x30]
        stur	q18, [x0, #-0x20]
        stur	q19, [x0, #-0x10]
        add	w11, w11, #0x20
        cmp	w11, #0x100
        b.lt	final_copy
        mov	w0, w9
        b	return

return:
        add	sp, sp, #0x240
        ret

#endif /* (defined(MLK_ARITH_BACKEND_AARCH64_CLEAN) ||
           defined(MLK_ARITH_BACKEND_AARCH64_OPT))
          && !defined(MLK_MULTILEVEL_BUILD_NO_SHARED) */
