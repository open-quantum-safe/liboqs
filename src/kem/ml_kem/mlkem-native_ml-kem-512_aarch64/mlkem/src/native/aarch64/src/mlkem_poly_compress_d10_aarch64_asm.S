/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* References
 * ==========
 *
 * - [FIPS203]
 *   FIPS 203 Module-Lattice-Based Key-Encapsulation Mechanism Standard
 *   National Institute of Standards and Technology
 *   https://csrc.nist.gov/pubs/fips/203/final
 */

/*yaml
  Name: poly_compress_d10_aarch64_asm
  Description: AArch64 polynomial compression (d=10)
  Signature: void mlk_poly_compress_d10_aarch64_asm(uint8_t r[320], const int16_t a[256])
  ABI:
    Architecture: aarch64
    CallingConvention: AAPCS64
    Features: [NEON]
    x0:
      type: buffer
      size_bytes: 320
      permissions: write-only
      c_parameter: uint8_t r[320]
      description: Output compressed polynomial
    x1:
      type: buffer
      size_bytes: 512
      permissions: read-only
      c_parameter: const int16_t a[256]
      description: Input polynomial
  Stack:
    bytes: 0
*/

#include "../../../common.h"
#if defined(MLK_SYS_AARCH64_NEON) && \
    defined(MLK_ARITH_BACKEND_AARCH64) && \
    !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED) && \
    (!defined(MLK_CONFIG_NO_ENCAPS_API) || \
     !defined(MLK_CONFIG_NO_DECAPS_API)) && \
    (defined(MLK_CONFIG_MULTILEVEL_WITH_SHARED) || MLKEM_K == 2 || MLKEM_K == 3)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/aarch64_opt/src/mlkem_poly_compress_d10_aarch64_asm.S using scripts/simpasm. Do not modify it directly.
 */

.text
.balign 4
.global MLK_ASM_NAMESPACE(poly_compress_d10_aarch64_asm)
MLK_ASM_FN_SYMBOL(poly_compress_d10_aarch64_asm)

        .cfi_startproc
        movi v24.2d, #0000000000000000
        mov x3, #0x6ed0             // =28368
        movk x3, #0x275f, lsl #16
        dup v25.4s, w3
        mov x3, #0x100              // =256
        movk x3, #0x302, lsl #16
        movk x3, #0x804, lsl #32
        movk x3, #0xa09, lsl #48
        mov x4, #0xc0b              // =3083
        movk x4, #0x804, lsl #16
        movk x4, #0xa09, lsl #32
        movk x4, #0xc0b, lsl #48
        dup v26.2d, x3
        dup v16.2d, x4
        zip1 v26.2d, v26.2d, v16.2d
        ldr q0, [x1]
        ldr q1, [x1, #0x10]
        ldr q2, [x1, #0x20]
        ldr q3, [x1, #0x30]
        ldr q4, [x1, #0x40]
        ldr q5, [x1, #0x50]
        ldr q6, [x1, #0x60]
        ldr q7, [x1, #0x70]
        trn2 v16.8h, v0.8h, v24.8h
        trn1 v0.8h, v0.8h, v24.8h
        sqrdmulh v0.4s, v0.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v0.4s, v16.4s, #0xa
        trn2 v16.4s, v0.4s, v24.4s
        sli v0.2d, v16.2d, #0x14
        tbl v0.16b, { v0.16b }, v26.16b
        trn2 v16.8h, v1.8h, v24.8h
        trn1 v1.8h, v1.8h, v24.8h
        sqrdmulh v1.4s, v1.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v1.4s, v16.4s, #0xa
        trn2 v16.4s, v1.4s, v24.4s
        sli v1.2d, v16.2d, #0x14
        tbl v1.16b, { v1.16b }, v26.16b
        trn2 v16.8h, v2.8h, v24.8h
        trn1 v2.8h, v2.8h, v24.8h
        sqrdmulh v2.4s, v2.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v2.4s, v16.4s, #0xa
        trn2 v16.4s, v2.4s, v24.4s
        sli v2.2d, v16.2d, #0x14
        tbl v2.16b, { v2.16b }, v26.16b
        trn2 v16.8h, v3.8h, v24.8h
        trn1 v3.8h, v3.8h, v24.8h
        sqrdmulh v3.4s, v3.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v3.4s, v16.4s, #0xa
        trn2 v16.4s, v3.4s, v24.4s
        sli v3.2d, v16.2d, #0x14
        tbl v3.16b, { v3.16b }, v26.16b
        trn2 v16.8h, v4.8h, v24.8h
        trn1 v4.8h, v4.8h, v24.8h
        sqrdmulh v4.4s, v4.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v4.4s, v16.4s, #0xa
        trn2 v16.4s, v4.4s, v24.4s
        sli v4.2d, v16.2d, #0x14
        tbl v4.16b, { v4.16b }, v26.16b
        trn2 v16.8h, v5.8h, v24.8h
        trn1 v5.8h, v5.8h, v24.8h
        sqrdmulh v5.4s, v5.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v5.4s, v16.4s, #0xa
        trn2 v16.4s, v5.4s, v24.4s
        sli v5.2d, v16.2d, #0x14
        tbl v5.16b, { v5.16b }, v26.16b
        trn2 v16.8h, v6.8h, v24.8h
        trn1 v6.8h, v6.8h, v24.8h
        sqrdmulh v6.4s, v6.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v6.4s, v16.4s, #0xa
        trn2 v16.4s, v6.4s, v24.4s
        sli v6.2d, v16.2d, #0x14
        tbl v6.16b, { v6.16b }, v26.16b
        trn2 v16.8h, v7.8h, v24.8h
        trn1 v7.8h, v7.8h, v24.8h
        sqrdmulh v7.4s, v7.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v7.4s, v16.4s, #0xa
        trn2 v16.4s, v7.4s, v24.4s
        sli v7.2d, v16.2d, #0x14
        tbl v7.16b, { v7.16b }, v26.16b
        ext v17.16b, v6.16b, v7.16b, #0xa
        st1 { v0.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v1.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v2.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v3.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v4.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v5.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v6.16b }, [x0], #16
        sub x0, x0, #0xc
        st1 { v17.16b }, [x0], #16
        ldr q0, [x1, #0x80]
        ldr q1, [x1, #0x90]
        ldr q2, [x1, #0xa0]
        ldr q3, [x1, #0xb0]
        ldr q4, [x1, #0xc0]
        ldr q5, [x1, #0xd0]
        ldr q6, [x1, #0xe0]
        ldr q7, [x1, #0xf0]
        trn2 v16.8h, v0.8h, v24.8h
        trn1 v0.8h, v0.8h, v24.8h
        sqrdmulh v0.4s, v0.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v0.4s, v16.4s, #0xa
        trn2 v16.4s, v0.4s, v24.4s
        sli v0.2d, v16.2d, #0x14
        tbl v0.16b, { v0.16b }, v26.16b
        trn2 v16.8h, v1.8h, v24.8h
        trn1 v1.8h, v1.8h, v24.8h
        sqrdmulh v1.4s, v1.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v1.4s, v16.4s, #0xa
        trn2 v16.4s, v1.4s, v24.4s
        sli v1.2d, v16.2d, #0x14
        tbl v1.16b, { v1.16b }, v26.16b
        trn2 v16.8h, v2.8h, v24.8h
        trn1 v2.8h, v2.8h, v24.8h
        sqrdmulh v2.4s, v2.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v2.4s, v16.4s, #0xa
        trn2 v16.4s, v2.4s, v24.4s
        sli v2.2d, v16.2d, #0x14
        tbl v2.16b, { v2.16b }, v26.16b
        trn2 v16.8h, v3.8h, v24.8h
        trn1 v3.8h, v3.8h, v24.8h
        sqrdmulh v3.4s, v3.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v3.4s, v16.4s, #0xa
        trn2 v16.4s, v3.4s, v24.4s
        sli v3.2d, v16.2d, #0x14
        tbl v3.16b, { v3.16b }, v26.16b
        trn2 v16.8h, v4.8h, v24.8h
        trn1 v4.8h, v4.8h, v24.8h
        sqrdmulh v4.4s, v4.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v4.4s, v16.4s, #0xa
        trn2 v16.4s, v4.4s, v24.4s
        sli v4.2d, v16.2d, #0x14
        tbl v4.16b, { v4.16b }, v26.16b
        trn2 v16.8h, v5.8h, v24.8h
        trn1 v5.8h, v5.8h, v24.8h
        sqrdmulh v5.4s, v5.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v5.4s, v16.4s, #0xa
        trn2 v16.4s, v5.4s, v24.4s
        sli v5.2d, v16.2d, #0x14
        tbl v5.16b, { v5.16b }, v26.16b
        trn2 v16.8h, v6.8h, v24.8h
        trn1 v6.8h, v6.8h, v24.8h
        sqrdmulh v6.4s, v6.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v6.4s, v16.4s, #0xa
        trn2 v16.4s, v6.4s, v24.4s
        sli v6.2d, v16.2d, #0x14
        tbl v6.16b, { v6.16b }, v26.16b
        trn2 v16.8h, v7.8h, v24.8h
        trn1 v7.8h, v7.8h, v24.8h
        sqrdmulh v7.4s, v7.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v7.4s, v16.4s, #0xa
        trn2 v16.4s, v7.4s, v24.4s
        sli v7.2d, v16.2d, #0x14
        tbl v7.16b, { v7.16b }, v26.16b
        ext v17.16b, v6.16b, v7.16b, #0xa
        st1 { v0.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v1.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v2.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v3.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v4.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v5.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v6.16b }, [x0], #16
        sub x0, x0, #0xc
        st1 { v17.16b }, [x0], #16
        ldr q0, [x1, #0x100]
        ldr q1, [x1, #0x110]
        ldr q2, [x1, #0x120]
        ldr q3, [x1, #0x130]
        ldr q4, [x1, #0x140]
        ldr q5, [x1, #0x150]
        ldr q6, [x1, #0x160]
        ldr q7, [x1, #0x170]
        trn2 v16.8h, v0.8h, v24.8h
        trn1 v0.8h, v0.8h, v24.8h
        sqrdmulh v0.4s, v0.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v0.4s, v16.4s, #0xa
        trn2 v16.4s, v0.4s, v24.4s
        sli v0.2d, v16.2d, #0x14
        tbl v0.16b, { v0.16b }, v26.16b
        trn2 v16.8h, v1.8h, v24.8h
        trn1 v1.8h, v1.8h, v24.8h
        sqrdmulh v1.4s, v1.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v1.4s, v16.4s, #0xa
        trn2 v16.4s, v1.4s, v24.4s
        sli v1.2d, v16.2d, #0x14
        tbl v1.16b, { v1.16b }, v26.16b
        trn2 v16.8h, v2.8h, v24.8h
        trn1 v2.8h, v2.8h, v24.8h
        sqrdmulh v2.4s, v2.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v2.4s, v16.4s, #0xa
        trn2 v16.4s, v2.4s, v24.4s
        sli v2.2d, v16.2d, #0x14
        tbl v2.16b, { v2.16b }, v26.16b
        trn2 v16.8h, v3.8h, v24.8h
        trn1 v3.8h, v3.8h, v24.8h
        sqrdmulh v3.4s, v3.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v3.4s, v16.4s, #0xa
        trn2 v16.4s, v3.4s, v24.4s
        sli v3.2d, v16.2d, #0x14
        tbl v3.16b, { v3.16b }, v26.16b
        trn2 v16.8h, v4.8h, v24.8h
        trn1 v4.8h, v4.8h, v24.8h
        sqrdmulh v4.4s, v4.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v4.4s, v16.4s, #0xa
        trn2 v16.4s, v4.4s, v24.4s
        sli v4.2d, v16.2d, #0x14
        tbl v4.16b, { v4.16b }, v26.16b
        trn2 v16.8h, v5.8h, v24.8h
        trn1 v5.8h, v5.8h, v24.8h
        sqrdmulh v5.4s, v5.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v5.4s, v16.4s, #0xa
        trn2 v16.4s, v5.4s, v24.4s
        sli v5.2d, v16.2d, #0x14
        tbl v5.16b, { v5.16b }, v26.16b
        trn2 v16.8h, v6.8h, v24.8h
        trn1 v6.8h, v6.8h, v24.8h
        sqrdmulh v6.4s, v6.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v6.4s, v16.4s, #0xa
        trn2 v16.4s, v6.4s, v24.4s
        sli v6.2d, v16.2d, #0x14
        tbl v6.16b, { v6.16b }, v26.16b
        trn2 v16.8h, v7.8h, v24.8h
        trn1 v7.8h, v7.8h, v24.8h
        sqrdmulh v7.4s, v7.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v7.4s, v16.4s, #0xa
        trn2 v16.4s, v7.4s, v24.4s
        sli v7.2d, v16.2d, #0x14
        tbl v7.16b, { v7.16b }, v26.16b
        ext v17.16b, v6.16b, v7.16b, #0xa
        st1 { v0.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v1.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v2.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v3.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v4.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v5.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v6.16b }, [x0], #16
        sub x0, x0, #0xc
        st1 { v17.16b }, [x0], #16
        ldr q0, [x1, #0x180]
        ldr q1, [x1, #0x190]
        ldr q2, [x1, #0x1a0]
        ldr q3, [x1, #0x1b0]
        ldr q4, [x1, #0x1c0]
        ldr q5, [x1, #0x1d0]
        ldr q6, [x1, #0x1e0]
        ldr q7, [x1, #0x1f0]
        trn2 v16.8h, v0.8h, v24.8h
        trn1 v0.8h, v0.8h, v24.8h
        sqrdmulh v0.4s, v0.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v0.4s, v16.4s, #0xa
        trn2 v16.4s, v0.4s, v24.4s
        sli v0.2d, v16.2d, #0x14
        tbl v0.16b, { v0.16b }, v26.16b
        trn2 v16.8h, v1.8h, v24.8h
        trn1 v1.8h, v1.8h, v24.8h
        sqrdmulh v1.4s, v1.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v1.4s, v16.4s, #0xa
        trn2 v16.4s, v1.4s, v24.4s
        sli v1.2d, v16.2d, #0x14
        tbl v1.16b, { v1.16b }, v26.16b
        trn2 v16.8h, v2.8h, v24.8h
        trn1 v2.8h, v2.8h, v24.8h
        sqrdmulh v2.4s, v2.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v2.4s, v16.4s, #0xa
        trn2 v16.4s, v2.4s, v24.4s
        sli v2.2d, v16.2d, #0x14
        tbl v2.16b, { v2.16b }, v26.16b
        trn2 v16.8h, v3.8h, v24.8h
        trn1 v3.8h, v3.8h, v24.8h
        sqrdmulh v3.4s, v3.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v3.4s, v16.4s, #0xa
        trn2 v16.4s, v3.4s, v24.4s
        sli v3.2d, v16.2d, #0x14
        tbl v3.16b, { v3.16b }, v26.16b
        trn2 v16.8h, v4.8h, v24.8h
        trn1 v4.8h, v4.8h, v24.8h
        sqrdmulh v4.4s, v4.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v4.4s, v16.4s, #0xa
        trn2 v16.4s, v4.4s, v24.4s
        sli v4.2d, v16.2d, #0x14
        tbl v4.16b, { v4.16b }, v26.16b
        trn2 v16.8h, v5.8h, v24.8h
        trn1 v5.8h, v5.8h, v24.8h
        sqrdmulh v5.4s, v5.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v5.4s, v16.4s, #0xa
        trn2 v16.4s, v5.4s, v24.4s
        sli v5.2d, v16.2d, #0x14
        tbl v5.16b, { v5.16b }, v26.16b
        trn2 v16.8h, v6.8h, v24.8h
        trn1 v6.8h, v6.8h, v24.8h
        sqrdmulh v6.4s, v6.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v6.4s, v16.4s, #0xa
        trn2 v16.4s, v6.4s, v24.4s
        sli v6.2d, v16.2d, #0x14
        tbl v6.16b, { v6.16b }, v26.16b
        trn2 v16.8h, v7.8h, v24.8h
        trn1 v7.8h, v7.8h, v24.8h
        sqrdmulh v7.4s, v7.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v7.4s, v16.4s, #0xa
        trn2 v16.4s, v7.4s, v24.4s
        sli v7.2d, v16.2d, #0x14
        tbl v7.16b, { v7.16b }, v26.16b
        ext v17.16b, v6.16b, v7.16b, #0xa
        st1 { v0.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v1.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v2.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v3.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v4.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v5.16b }, [x0], #16
        sub x0, x0, #0x6
        st1 { v6.16b }, [x0], #16
        sub x0, x0, #0xc
        st1 { v17.16b }, [x0], #16

Lmlk_poly_compress_d10_end:
        ret
        .cfi_endproc

MLK_ASM_FN_SIZE(poly_compress_d10_aarch64_asm)

#endif /* MLK_SYS_AARCH64_NEON && MLK_ARITH_BACKEND_AARCH64 && \
          !MLK_CONFIG_MULTILEVEL_NO_SHARED && (!MLK_CONFIG_NO_ENCAPS_API || \
          !MLK_CONFIG_NO_DECAPS_API) && (MLK_CONFIG_MULTILEVEL_WITH_SHARED || \
          MLKEM_K == 2 || MLKEM_K == 3) */

#if defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
