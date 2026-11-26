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
  Name: poly_compress_d11_aarch64_asm
  Description: AArch64 polynomial compression (d=11)
  Signature: void mlk_poly_compress_d11_aarch64_asm(uint8_t r[352], const int16_t a[256])
  ABI:
    Architecture: aarch64
    CallingConvention: AAPCS64
    Features: [NEON]
    x0:
      type: buffer
      size_bytes: 352
      permissions: write-only
      c_parameter: uint8_t r[352]
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
    (defined(MLK_CONFIG_MULTILEVEL_WITH_SHARED) || MLKEM_K == 4)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/aarch64_opt/src/mlkem_poly_compress_d11_aarch64_asm.S using scripts/simpasm. Do not modify it directly.
 */

.text
.balign 4
.global MLK_ASM_NAMESPACE(poly_compress_d11_aarch64_asm)
MLK_ASM_FN_SYMBOL(poly_compress_d11_aarch64_asm)

        .cfi_startproc
        movi v24.2d, #0000000000000000
        mov x3, #0xdda0             // =56736
        movk x3, #0x4ebe, lsl #16
        dup v25.4s, w3
        ld2 { v0.2d, v1.2d }, [x1], #32
        ld2 { v2.2d, v3.2d }, [x1], #32
        ld2 { v4.2d, v5.2d }, [x1], #32
        ld2 { v6.2d, v7.2d }, [x1], #32
        trn2 v16.8h, v0.8h, v24.8h
        trn1 v0.8h, v0.8h, v24.8h
        sqrdmulh v0.4s, v0.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v0.4s, v16.4s, #0xb
        trn2 v16.4s, v0.4s, v24.4s
        sli v0.2d, v16.2d, #0x16
        trn2 v16.8h, v1.8h, v24.8h
        trn1 v1.8h, v1.8h, v24.8h
        sqrdmulh v1.4s, v1.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v1.4s, v16.4s, #0xb
        trn2 v16.4s, v1.4s, v24.4s
        sli v1.2d, v16.2d, #0x16
        sli v0.2d, v1.2d, #0x2c
        ushr v1.2d, v1.2d, #0x14
        zip1 v17.2d, v0.2d, v1.2d
        zip2 v18.2d, v0.2d, v1.2d
        trn2 v16.8h, v2.8h, v24.8h
        trn1 v2.8h, v2.8h, v24.8h
        sqrdmulh v2.4s, v2.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v2.4s, v16.4s, #0xb
        trn2 v16.4s, v2.4s, v24.4s
        sli v2.2d, v16.2d, #0x16
        trn2 v16.8h, v3.8h, v24.8h
        trn1 v3.8h, v3.8h, v24.8h
        sqrdmulh v3.4s, v3.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v3.4s, v16.4s, #0xb
        trn2 v16.4s, v3.4s, v24.4s
        sli v3.2d, v16.2d, #0x16
        sli v2.2d, v3.2d, #0x2c
        ushr v3.2d, v3.2d, #0x14
        zip1 v19.2d, v2.2d, v3.2d
        zip2 v20.2d, v2.2d, v3.2d
        trn2 v16.8h, v4.8h, v24.8h
        trn1 v4.8h, v4.8h, v24.8h
        sqrdmulh v4.4s, v4.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v4.4s, v16.4s, #0xb
        trn2 v16.4s, v4.4s, v24.4s
        sli v4.2d, v16.2d, #0x16
        trn2 v16.8h, v5.8h, v24.8h
        trn1 v5.8h, v5.8h, v24.8h
        sqrdmulh v5.4s, v5.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v5.4s, v16.4s, #0xb
        trn2 v16.4s, v5.4s, v24.4s
        sli v5.2d, v16.2d, #0x16
        sli v4.2d, v5.2d, #0x2c
        ushr v5.2d, v5.2d, #0x14
        zip1 v21.2d, v4.2d, v5.2d
        zip2 v22.2d, v4.2d, v5.2d
        trn2 v16.8h, v6.8h, v24.8h
        trn1 v6.8h, v6.8h, v24.8h
        sqrdmulh v6.4s, v6.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v6.4s, v16.4s, #0xb
        trn2 v16.4s, v6.4s, v24.4s
        sli v6.2d, v16.2d, #0x16
        trn2 v16.8h, v7.8h, v24.8h
        trn1 v7.8h, v7.8h, v24.8h
        sqrdmulh v7.4s, v7.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v7.4s, v16.4s, #0xb
        trn2 v16.4s, v7.4s, v24.4s
        sli v7.2d, v16.2d, #0x16
        sli v6.2d, v7.2d, #0x2c
        ushr v7.2d, v7.2d, #0x14
        zip1 v23.2d, v6.2d, v7.2d
        zip2 v26.2d, v6.2d, v7.2d
        ext v27.16b, v26.16b, v26.16b, #0x3
        st1 { v17.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v18.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v19.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v20.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v21.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v22.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v23.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v26.8b }, [x0], #8
        sub x0, x0, #0x5
        st1 { v27.8b }, [x0], #8
        ld2 { v0.2d, v1.2d }, [x1], #32
        ld2 { v2.2d, v3.2d }, [x1], #32
        ld2 { v4.2d, v5.2d }, [x1], #32
        ld2 { v6.2d, v7.2d }, [x1], #32
        trn2 v16.8h, v0.8h, v24.8h
        trn1 v0.8h, v0.8h, v24.8h
        sqrdmulh v0.4s, v0.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v0.4s, v16.4s, #0xb
        trn2 v16.4s, v0.4s, v24.4s
        sli v0.2d, v16.2d, #0x16
        trn2 v16.8h, v1.8h, v24.8h
        trn1 v1.8h, v1.8h, v24.8h
        sqrdmulh v1.4s, v1.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v1.4s, v16.4s, #0xb
        trn2 v16.4s, v1.4s, v24.4s
        sli v1.2d, v16.2d, #0x16
        sli v0.2d, v1.2d, #0x2c
        ushr v1.2d, v1.2d, #0x14
        zip1 v17.2d, v0.2d, v1.2d
        zip2 v18.2d, v0.2d, v1.2d
        trn2 v16.8h, v2.8h, v24.8h
        trn1 v2.8h, v2.8h, v24.8h
        sqrdmulh v2.4s, v2.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v2.4s, v16.4s, #0xb
        trn2 v16.4s, v2.4s, v24.4s
        sli v2.2d, v16.2d, #0x16
        trn2 v16.8h, v3.8h, v24.8h
        trn1 v3.8h, v3.8h, v24.8h
        sqrdmulh v3.4s, v3.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v3.4s, v16.4s, #0xb
        trn2 v16.4s, v3.4s, v24.4s
        sli v3.2d, v16.2d, #0x16
        sli v2.2d, v3.2d, #0x2c
        ushr v3.2d, v3.2d, #0x14
        zip1 v19.2d, v2.2d, v3.2d
        zip2 v20.2d, v2.2d, v3.2d
        trn2 v16.8h, v4.8h, v24.8h
        trn1 v4.8h, v4.8h, v24.8h
        sqrdmulh v4.4s, v4.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v4.4s, v16.4s, #0xb
        trn2 v16.4s, v4.4s, v24.4s
        sli v4.2d, v16.2d, #0x16
        trn2 v16.8h, v5.8h, v24.8h
        trn1 v5.8h, v5.8h, v24.8h
        sqrdmulh v5.4s, v5.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v5.4s, v16.4s, #0xb
        trn2 v16.4s, v5.4s, v24.4s
        sli v5.2d, v16.2d, #0x16
        sli v4.2d, v5.2d, #0x2c
        ushr v5.2d, v5.2d, #0x14
        zip1 v21.2d, v4.2d, v5.2d
        zip2 v22.2d, v4.2d, v5.2d
        trn2 v16.8h, v6.8h, v24.8h
        trn1 v6.8h, v6.8h, v24.8h
        sqrdmulh v6.4s, v6.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v6.4s, v16.4s, #0xb
        trn2 v16.4s, v6.4s, v24.4s
        sli v6.2d, v16.2d, #0x16
        trn2 v16.8h, v7.8h, v24.8h
        trn1 v7.8h, v7.8h, v24.8h
        sqrdmulh v7.4s, v7.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v7.4s, v16.4s, #0xb
        trn2 v16.4s, v7.4s, v24.4s
        sli v7.2d, v16.2d, #0x16
        sli v6.2d, v7.2d, #0x2c
        ushr v7.2d, v7.2d, #0x14
        zip1 v23.2d, v6.2d, v7.2d
        zip2 v26.2d, v6.2d, v7.2d
        ext v27.16b, v26.16b, v26.16b, #0x3
        st1 { v17.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v18.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v19.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v20.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v21.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v22.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v23.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v26.8b }, [x0], #8
        sub x0, x0, #0x5
        st1 { v27.8b }, [x0], #8
        ld2 { v0.2d, v1.2d }, [x1], #32
        ld2 { v2.2d, v3.2d }, [x1], #32
        ld2 { v4.2d, v5.2d }, [x1], #32
        ld2 { v6.2d, v7.2d }, [x1], #32
        trn2 v16.8h, v0.8h, v24.8h
        trn1 v0.8h, v0.8h, v24.8h
        sqrdmulh v0.4s, v0.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v0.4s, v16.4s, #0xb
        trn2 v16.4s, v0.4s, v24.4s
        sli v0.2d, v16.2d, #0x16
        trn2 v16.8h, v1.8h, v24.8h
        trn1 v1.8h, v1.8h, v24.8h
        sqrdmulh v1.4s, v1.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v1.4s, v16.4s, #0xb
        trn2 v16.4s, v1.4s, v24.4s
        sli v1.2d, v16.2d, #0x16
        sli v0.2d, v1.2d, #0x2c
        ushr v1.2d, v1.2d, #0x14
        zip1 v17.2d, v0.2d, v1.2d
        zip2 v18.2d, v0.2d, v1.2d
        trn2 v16.8h, v2.8h, v24.8h
        trn1 v2.8h, v2.8h, v24.8h
        sqrdmulh v2.4s, v2.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v2.4s, v16.4s, #0xb
        trn2 v16.4s, v2.4s, v24.4s
        sli v2.2d, v16.2d, #0x16
        trn2 v16.8h, v3.8h, v24.8h
        trn1 v3.8h, v3.8h, v24.8h
        sqrdmulh v3.4s, v3.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v3.4s, v16.4s, #0xb
        trn2 v16.4s, v3.4s, v24.4s
        sli v3.2d, v16.2d, #0x16
        sli v2.2d, v3.2d, #0x2c
        ushr v3.2d, v3.2d, #0x14
        zip1 v19.2d, v2.2d, v3.2d
        zip2 v20.2d, v2.2d, v3.2d
        trn2 v16.8h, v4.8h, v24.8h
        trn1 v4.8h, v4.8h, v24.8h
        sqrdmulh v4.4s, v4.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v4.4s, v16.4s, #0xb
        trn2 v16.4s, v4.4s, v24.4s
        sli v4.2d, v16.2d, #0x16
        trn2 v16.8h, v5.8h, v24.8h
        trn1 v5.8h, v5.8h, v24.8h
        sqrdmulh v5.4s, v5.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v5.4s, v16.4s, #0xb
        trn2 v16.4s, v5.4s, v24.4s
        sli v5.2d, v16.2d, #0x16
        sli v4.2d, v5.2d, #0x2c
        ushr v5.2d, v5.2d, #0x14
        zip1 v21.2d, v4.2d, v5.2d
        zip2 v22.2d, v4.2d, v5.2d
        trn2 v16.8h, v6.8h, v24.8h
        trn1 v6.8h, v6.8h, v24.8h
        sqrdmulh v6.4s, v6.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v6.4s, v16.4s, #0xb
        trn2 v16.4s, v6.4s, v24.4s
        sli v6.2d, v16.2d, #0x16
        trn2 v16.8h, v7.8h, v24.8h
        trn1 v7.8h, v7.8h, v24.8h
        sqrdmulh v7.4s, v7.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v7.4s, v16.4s, #0xb
        trn2 v16.4s, v7.4s, v24.4s
        sli v7.2d, v16.2d, #0x16
        sli v6.2d, v7.2d, #0x2c
        ushr v7.2d, v7.2d, #0x14
        zip1 v23.2d, v6.2d, v7.2d
        zip2 v26.2d, v6.2d, v7.2d
        ext v27.16b, v26.16b, v26.16b, #0x3
        st1 { v17.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v18.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v19.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v20.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v21.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v22.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v23.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v26.8b }, [x0], #8
        sub x0, x0, #0x5
        st1 { v27.8b }, [x0], #8
        ld2 { v0.2d, v1.2d }, [x1], #32
        ld2 { v2.2d, v3.2d }, [x1], #32
        ld2 { v4.2d, v5.2d }, [x1], #32
        ld2 { v6.2d, v7.2d }, [x1], #32
        trn2 v16.8h, v0.8h, v24.8h
        trn1 v0.8h, v0.8h, v24.8h
        sqrdmulh v0.4s, v0.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v0.4s, v16.4s, #0xb
        trn2 v16.4s, v0.4s, v24.4s
        sli v0.2d, v16.2d, #0x16
        trn2 v16.8h, v1.8h, v24.8h
        trn1 v1.8h, v1.8h, v24.8h
        sqrdmulh v1.4s, v1.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v1.4s, v16.4s, #0xb
        trn2 v16.4s, v1.4s, v24.4s
        sli v1.2d, v16.2d, #0x16
        sli v0.2d, v1.2d, #0x2c
        ushr v1.2d, v1.2d, #0x14
        zip1 v17.2d, v0.2d, v1.2d
        zip2 v18.2d, v0.2d, v1.2d
        trn2 v16.8h, v2.8h, v24.8h
        trn1 v2.8h, v2.8h, v24.8h
        sqrdmulh v2.4s, v2.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v2.4s, v16.4s, #0xb
        trn2 v16.4s, v2.4s, v24.4s
        sli v2.2d, v16.2d, #0x16
        trn2 v16.8h, v3.8h, v24.8h
        trn1 v3.8h, v3.8h, v24.8h
        sqrdmulh v3.4s, v3.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v3.4s, v16.4s, #0xb
        trn2 v16.4s, v3.4s, v24.4s
        sli v3.2d, v16.2d, #0x16
        sli v2.2d, v3.2d, #0x2c
        ushr v3.2d, v3.2d, #0x14
        zip1 v19.2d, v2.2d, v3.2d
        zip2 v20.2d, v2.2d, v3.2d
        trn2 v16.8h, v4.8h, v24.8h
        trn1 v4.8h, v4.8h, v24.8h
        sqrdmulh v4.4s, v4.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v4.4s, v16.4s, #0xb
        trn2 v16.4s, v4.4s, v24.4s
        sli v4.2d, v16.2d, #0x16
        trn2 v16.8h, v5.8h, v24.8h
        trn1 v5.8h, v5.8h, v24.8h
        sqrdmulh v5.4s, v5.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v5.4s, v16.4s, #0xb
        trn2 v16.4s, v5.4s, v24.4s
        sli v5.2d, v16.2d, #0x16
        sli v4.2d, v5.2d, #0x2c
        ushr v5.2d, v5.2d, #0x14
        zip1 v21.2d, v4.2d, v5.2d
        zip2 v22.2d, v4.2d, v5.2d
        trn2 v16.8h, v6.8h, v24.8h
        trn1 v6.8h, v6.8h, v24.8h
        sqrdmulh v6.4s, v6.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v6.4s, v16.4s, #0xb
        trn2 v16.4s, v6.4s, v24.4s
        sli v6.2d, v16.2d, #0x16
        trn2 v16.8h, v7.8h, v24.8h
        trn1 v7.8h, v7.8h, v24.8h
        sqrdmulh v7.4s, v7.4s, v25.4s
        sqrdmulh v16.4s, v16.4s, v25.4s
        sli v7.4s, v16.4s, #0xb
        trn2 v16.4s, v7.4s, v24.4s
        sli v7.2d, v16.2d, #0x16
        sli v6.2d, v7.2d, #0x2c
        ushr v7.2d, v7.2d, #0x14
        zip1 v23.2d, v6.2d, v7.2d
        zip2 v26.2d, v6.2d, v7.2d
        ext v27.16b, v26.16b, v26.16b, #0x3
        st1 { v17.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v18.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v19.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v20.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v21.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v22.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v23.16b }, [x0], #16
        sub x0, x0, #0x5
        st1 { v26.8b }, [x0], #8
        sub x0, x0, #0x5
        st1 { v27.8b }, [x0], #8

Lmlk_poly_compress_d11_end:
        ret
        .cfi_endproc

MLK_ASM_FN_SIZE(poly_compress_d11_aarch64_asm)

#endif /* MLK_SYS_AARCH64_NEON && MLK_ARITH_BACKEND_AARCH64 && \
          !MLK_CONFIG_MULTILEVEL_NO_SHARED && (!MLK_CONFIG_NO_ENCAPS_API || \
          !MLK_CONFIG_NO_DECAPS_API) && (MLK_CONFIG_MULTILEVEL_WITH_SHARED || \
          MLKEM_K == 4) */

#if defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
