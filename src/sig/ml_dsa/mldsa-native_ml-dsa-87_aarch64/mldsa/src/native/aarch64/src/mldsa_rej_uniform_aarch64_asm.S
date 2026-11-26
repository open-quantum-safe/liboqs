/*
 * Copyright (c) The mldsa-native project authors
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/*yaml
  Name: rej_uniform_aarch64_asm
  Description: AArch64 rejection sampling of uniform coefficients mod q
  Signature: uint64_t mld_rej_uniform_aarch64_asm(int32_t r[256], const uint8_t *buf, uint64_t buflen, const uint8_t table[256])
  ABI:
    Architecture: aarch64
    CallingConvention: AAPCS64
    Features: [NEON]
    x0:
      type: buffer
      size_bytes: 1024
      permissions: write-only
      c_parameter: int32_t r[256]
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
      description: Length of input buffer (must be a multiple of 24)
      test_with: 840  # MLD_POLY_UNIFORM_NBLOCKS * SHAKE128_RATE = 5 * 168
    x3:
      type: buffer
      size_bytes: 256
      permissions: read-only
      c_parameter: const uint8_t table[256]
      description: Lookup table (256 x uint8_t)
*/

#include "../../../common.h"
#if defined(MLD_SYS_AARCH64_NEON) && \
    defined(MLD_ARITH_BACKEND_AARCH64) && \
    !defined(MLD_CONFIG_MULTILEVEL_NO_SHARED)

/*
 * WARNING: This file is auto-derived from the mldsa-native source file
 *   dev/aarch64_opt/src/mldsa_rej_uniform_aarch64_asm.S using scripts/simpasm. Do not modify it directly.
 */

.text
.balign 4
.global MLD_ASM_NAMESPACE(rej_uniform_aarch64_asm)
MLD_ASM_FN_SYMBOL(rej_uniform_aarch64_asm)

        .cfi_startproc
        mov x7, #0x1                // =1
        movk x7, #0x2, lsl #32
        mov x11, #0x4               // =4
        movk x11, #0x8, lsl #32
        fmov d31, x7
        mov v31.d[1], x11
        movi v29.16b, #0x1
        mul v29.4s, v31.4s, v29.4s
        mov w4, #0xe001             // =57345
        movk w4, #0x7f, lsl #16
        dup v30.4s, w4
        mov x7, #0x100              // =256
        movk x7, #0x8002, lsl #16
        movk x7, #0x403, lsl #32
        movk x7, #0x8005, lsl #48
        mov x11, #0x706             // =1798
        movk x11, #0x8008, lsl #16
        movk x11, #0xa09, lsl #32
        movk x11, #0x800b, lsl #48
        fmov d20, x7
        mov v20.d[1], x11
        movi v28.4s, #0x80, lsl #16
        movi v4.16b, #0xc
        movi v5.16b, #0x8
        movi v6.16b, #0x4
        add v21.16b, v20.16b, v4.16b
        add v22.16b, v20.16b, v5.16b
        add v23.16b, v20.16b, v6.16b
        mov x9, #0x0                // =0
        cmp x2, #0x30
        b.lo Lmld_rej_uniform_block8

Lmld_rej_uniform_loop48:
        cmp x9, #0xf0
        b.hi Lmld_rej_uniform_block8
        sub x2, x2, #0x30
        ld1 { v0.16b, v1.16b, v2.16b }, [x1], #48
        tbl v16.16b, { v0.16b }, v20.16b
        tbl v17.16b, { v0.16b, v1.16b }, v21.16b
        tbl v18.16b, { v1.16b, v2.16b }, v22.16b
        tbl v19.16b, { v2.16b }, v23.16b
        bic v16.16b, v16.16b, v28.16b
        bic v17.16b, v17.16b, v28.16b
        bic v18.16b, v18.16b, v28.16b
        bic v19.16b, v19.16b, v28.16b
        cmhi v4.4s, v30.4s, v16.4s
        cmhi v5.4s, v30.4s, v17.4s
        cmhi v6.4s, v30.4s, v18.4s
        cmhi v7.4s, v30.4s, v19.4s
        trn1 v4.16b, v4.16b, v5.16b
        trn1 v5.16b, v6.16b, v7.16b
        trn1 v4.8h, v4.8h, v5.8h
        and v4.16b, v4.16b, v29.16b
        uaddlv d0, v4.4s
        fmov x15, d0
        ubfx x12, x15, #0, #4
        ubfx x13, x15, #8, #4
        ubfx x14, x15, #16, #4
        ubfx x15, x15, #24, #4
        ldr q24, [x3, x12, lsl #4]
        ldr q25, [x3, x13, lsl #4]
        ldr q26, [x3, x14, lsl #4]
        ldr q27, [x3, x15, lsl #4]
        cnt v4.16b, v4.16b
        uaddlv d0, v4.4s
        fmov x15, d0
        ubfx x12, x15, #0, #3
        ubfx x13, x15, #8, #3
        ubfx x14, x15, #16, #3
        ubfx x15, x15, #24, #3
        tbl v16.16b, { v16.16b }, v24.16b
        tbl v17.16b, { v17.16b }, v25.16b
        tbl v18.16b, { v18.16b }, v26.16b
        tbl v19.16b, { v19.16b }, v27.16b
        st1 { v16.4s }, [x0]
        add x0, x0, x12, lsl #2
        st1 { v17.4s }, [x0]
        add x0, x0, x13, lsl #2
        st1 { v18.4s }, [x0]
        add x0, x0, x14, lsl #2
        st1 { v19.4s }, [x0]
        add x0, x0, x15, lsl #2
        add x12, x12, x13
        add x14, x14, x15
        add x9, x9, x12
        add x9, x9, x14
        cmp x2, #0x30
        b.hs Lmld_rej_uniform_loop48

Lmld_rej_uniform_block8:
        cmp x2, #0x18
        b.lo Lmld_rej_uniform_return
        cmp x9, #0xf8
        b.hi Lmld_rej_uniform_scalar_loop
        sub x2, x2, #0x18
        ld1 { v0.16b }, [x1], #16
        ld1 { v1.8b }, [x1], #8
        tbl v16.16b, { v0.16b }, v20.16b
        tbl v17.16b, { v0.16b, v1.16b }, v21.16b
        bic v16.16b, v16.16b, v28.16b
        bic v17.16b, v17.16b, v28.16b
        cmhi v4.4s, v30.4s, v16.4s
        cmhi v5.4s, v30.4s, v17.4s
        and v4.16b, v4.16b, v31.16b
        and v5.16b, v5.16b, v31.16b
        uaddlv d0, v4.4s
        uaddlv d1, v5.4s
        fmov x12, d0
        fmov x13, d1
        ldr q24, [x3, x12, lsl #4]
        ldr q25, [x3, x13, lsl #4]
        cnt v4.16b, v4.16b
        cnt v5.16b, v5.16b
        uaddlv d0, v4.4s
        uaddlv d1, v5.4s
        fmov x12, d0
        fmov x13, d1
        tbl v16.16b, { v16.16b }, v24.16b
        tbl v17.16b, { v17.16b }, v25.16b
        st1 { v16.4s }, [x0]
        add x0, x0, x12, lsl #2
        st1 { v17.4s }, [x0]
        add x0, x0, x13, lsl #2
        add x9, x9, x12
        add x9, x9, x13

Lmld_rej_uniform_scalar_loop:
        cmp x9, #0x100
        b.hs Lmld_rej_uniform_return
        cbz x2, Lmld_rej_uniform_return
        ldrb w5, [x1]
        ldrb w7, [x1, #0x1]
        ldrb w6, [x1, #0x2]
        add x1, x1, #0x3
        sub x2, x2, #0x3
        and w6, w6, #0x7f
        orr w5, w5, w7, lsl #8
        orr w5, w5, w6, lsl #16
        cmp w5, w4
        b.hs Lmld_rej_uniform_scalar_loop
        str w5, [x0], #0x4
        add x9, x9, #0x1
        b Lmld_rej_uniform_scalar_loop

Lmld_rej_uniform_return:
        mov x0, x9
        ret
        .cfi_endproc

MLD_ASM_FN_SIZE(rej_uniform_aarch64_asm)

#endif /* MLD_SYS_AARCH64_NEON && MLD_ARITH_BACKEND_AARCH64 && \
          !MLD_CONFIG_MULTILEVEL_NO_SHARED */

#if defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
