/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/*yaml
  Name: rej_uniform_aarch64_asm
  Description: Run rejection sampling on uniform random bytes to generate uniform random integers mod q
  Signature: uint64_t mlk_rej_uniform_aarch64_asm(int16_t r[256], const uint8_t *buf, uint64_t buflen, const uint8_t table[4096])
  ABI:
    Architecture: aarch64
    CallingConvention: AAPCS64
    Features: [NEON]
    x0:
      type: buffer
      size_bytes: 512
      permissions: write-only
      c_parameter: int16_t r[256]
      description: Output buffer, disjoint from buf and table. The return value is its valid prefix length.
    x1:
      type: buffer
      size_bytes: x2
      permissions: read-only
      c_parameter: const uint8_t *buf
      description: Input buffer
    x2:
      type: scalar
      c_parameter: uint64_t buflen
      description: Length of input buffer (must be multiple of 24)
      test_with: 504  # MLKEM_GEN_MATRIX_NBLOCKS * MLK_XOF_RATE
    x3:
      type: buffer
      size_bytes: 4096
      permissions: read-only
      c_parameter: const uint8_t table[4096]
      description: Lookup table
*/

#include "../../../common.h"
#if defined(MLK_SYS_AARCH64_NEON) && \
    defined(MLK_ARITH_BACKEND_AARCH64) && \
    !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/aarch64_opt/src/mlkem_rej_uniform_aarch64_asm.S using scripts/simpasm. Do not modify it directly.
 */

.text
.balign 4
.global MLK_ASM_NAMESPACE(rej_uniform_aarch64_asm)
MLK_ASM_FN_SYMBOL(rej_uniform_aarch64_asm)

        .cfi_startproc
        mov x7, #0x1                // =1
        movk x7, #0x2, lsl #16
        movk x7, #0x4, lsl #32
        movk x7, #0x8, lsl #48
        mov v31.d[0], x7
        mov x7, #0x10               // =16
        movk x7, #0x20, lsl #16
        movk x7, #0x40, lsl #32
        movk x7, #0x80, lsl #48
        mov v31.d[1], x7
        movi v4.16b, #0x1
        shl v28.4s, v31.4s, #0x10
        ushr v28.4s, v28.4s, #0x10
        ushr v29.4s, v31.4s, #0x10
        mul v28.4s, v28.4s, v4.4s
        mul v29.4s, v29.4s, v4.4s
        mov w4, #0xd01              // =3329
        dup v30.8h, w4
        mov x7, #0x100              // =256
        movk x7, #0x403, lsl #16
        movk x7, #0x706, lsl #32
        movk x7, #0xa09, lsl #48
        mov x11, #0xd0c             // =3340
        movk x11, #0x100f, lsl #16
        movk x11, #0x1312, lsl #32
        movk x11, #0x1615, lsl #48
        fmov d20, x7
        mov v20.d[1], x11
        movi v4.16b, #0x1
        movi v5.16b, #0x8
        add v21.16b, v20.16b, v4.16b
        add v22.16b, v20.16b, v5.16b
        add v23.16b, v22.16b, v4.16b
        mov x9, #0x0                // =0
        cmp x2, #0x30
        b.lo Lmlk_rej_uniform_block16

Lmlk_rej_uniform_loop48:
        cmp x9, #0xe0
        b.hi Lmlk_rej_uniform_block16
        sub x2, x2, #0x30
        ld1 { v0.16b, v1.16b }, [x1], #32
        ld1 { v2.16b }, [x1], #16
        tbl v4.16b, { v0.16b, v1.16b }, v20.16b
        tbl v5.16b, { v1.16b, v2.16b }, v22.16b
        tbl v6.16b, { v0.16b, v1.16b }, v21.16b
        tbl v7.16b, { v1.16b, v2.16b }, v23.16b
        bic v4.8h, #0xf0, lsl #8
        bic v5.8h, #0xf0, lsl #8
        ushr v6.8h, v6.8h, #0x4
        ushr v7.8h, v7.8h, #0x4
        zip1 v16.8h, v4.8h, v6.8h
        zip2 v17.8h, v4.8h, v6.8h
        zip1 v18.8h, v5.8h, v7.8h
        zip2 v19.8h, v5.8h, v7.8h
        cmhi v4.8h, v30.8h, v16.8h
        cmhi v5.8h, v30.8h, v17.8h
        cmhi v6.8h, v30.8h, v18.8h
        cmhi v7.8h, v30.8h, v19.8h
        trn1 v4.16b, v4.16b, v5.16b
        trn1 v5.16b, v6.16b, v7.16b
        trn2 v6.8h, v4.8h, v5.8h
        trn1 v4.8h, v4.8h, v5.8h
        and v4.16b, v4.16b, v28.16b
        and v6.16b, v6.16b, v29.16b
        add v4.4s, v4.4s, v6.4s
        uaddlv d0, v4.4s
        fmov x15, d0
        ubfx x12, x15, #0, #8
        ubfx x13, x15, #8, #8
        ubfx x14, x15, #16, #8
        ubfx x15, x15, #24, #8
        ldr q24, [x3, x12, lsl #4]
        ldr q25, [x3, x13, lsl #4]
        ldr q26, [x3, x14, lsl #4]
        ldr q27, [x3, x15, lsl #4]
        cnt v4.16b, v4.16b
        uaddlv d0, v4.4s
        fmov x15, d0
        ubfx x12, x15, #0, #4
        ubfx x13, x15, #8, #4
        ubfx x14, x15, #16, #4
        ubfx x15, x15, #24, #4
        tbl v16.16b, { v16.16b }, v24.16b
        tbl v17.16b, { v17.16b }, v25.16b
        tbl v18.16b, { v18.16b }, v26.16b
        tbl v19.16b, { v19.16b }, v27.16b
        st1 { v16.8h }, [x0]
        add x0, x0, x12, lsl #1
        st1 { v17.8h }, [x0]
        add x0, x0, x13, lsl #1
        st1 { v18.8h }, [x0]
        add x0, x0, x14, lsl #1
        st1 { v19.8h }, [x0]
        add x0, x0, x15, lsl #1
        add x12, x12, x13
        add x14, x14, x15
        add x9, x9, x12
        add x9, x9, x14
        cmp x2, #0x30
        b.hs Lmlk_rej_uniform_loop48

Lmlk_rej_uniform_block16:
        cmp x2, #0x18
        b.lo Lmlk_rej_uniform_return
        cmp x9, #0xf0
        b.hi Lmlk_rej_uniform_scalar_loop
        sub x2, x2, #0x18
        ld1 { v0.16b }, [x1], #16
        ld1 { v1.8b }, [x1], #8
        tbl v4.16b, { v0.16b, v1.16b }, v20.16b
        tbl v5.16b, { v0.16b, v1.16b }, v21.16b
        bic v4.8h, #0xf0, lsl #8
        ushr v5.8h, v5.8h, #0x4
        zip1 v16.8h, v4.8h, v5.8h
        zip2 v17.8h, v4.8h, v5.8h
        cmhi v4.8h, v30.8h, v16.8h
        cmhi v5.8h, v30.8h, v17.8h
        and v4.16b, v4.16b, v31.16b
        and v5.16b, v5.16b, v31.16b
        uaddlv s0, v4.8h
        uaddlv s1, v5.8h
        fmov w12, s0
        fmov w13, s1
        ldr q24, [x3, x12, lsl #4]
        ldr q25, [x3, x13, lsl #4]
        cnt v4.16b, v4.16b
        cnt v5.16b, v5.16b
        uaddlv s0, v4.8h
        uaddlv s1, v5.8h
        fmov w12, s0
        fmov w13, s1
        tbl v16.16b, { v16.16b }, v24.16b
        tbl v17.16b, { v17.16b }, v25.16b
        st1 { v16.8h }, [x0]
        add x0, x0, x12, lsl #1
        st1 { v17.8h }, [x0]
        add x0, x0, x13, lsl #1
        add x9, x9, x12
        add x9, x9, x13

Lmlk_rej_uniform_scalar_loop:
        cmp x9, #0x100
        b.hs Lmlk_rej_uniform_return
        cbz x2, Lmlk_rej_uniform_return
        ldrb w5, [x1]
        ldrb w7, [x1, #0x1]
        ldrb w6, [x1, #0x2]
        add x1, x1, #0x3
        sub x2, x2, #0x3
        bfi w5, w7, #8, #4
        lsr w7, w7, #4
        orr w6, w7, w6, lsl #4
        cmp w5, w4
        b.hs Lmlk_rej_uniform_second_sample
        strh w5, [x0], #0x2
        add x9, x9, #0x1
        cmp x9, #0x100
        b.hs Lmlk_rej_uniform_scalar_loop

Lmlk_rej_uniform_second_sample:
        cmp w6, w4
        b.hs Lmlk_rej_uniform_scalar_loop
        strh w6, [x0], #0x2
        add x9, x9, #0x1
        b Lmlk_rej_uniform_scalar_loop

Lmlk_rej_uniform_return:
        mov x0, x9
        ret
        .cfi_endproc

MLK_ASM_FN_SIZE(rej_uniform_aarch64_asm)

#endif /* MLK_SYS_AARCH64_NEON && MLK_ARITH_BACKEND_AARCH64 && \
          !MLK_CONFIG_MULTILEVEL_NO_SHARED */

#if defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
