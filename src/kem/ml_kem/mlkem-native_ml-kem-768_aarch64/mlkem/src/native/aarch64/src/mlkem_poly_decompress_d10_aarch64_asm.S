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
  Name: poly_decompress_d10_aarch64_asm
  Description: Decompression of a polynomial from 10 bits per coefficient
  Signature: void mlk_poly_decompress_d10_aarch64_asm(int16_t r[256], const uint8_t a[320], const uint8_t table[80])
  ABI:
    Architecture: aarch64
    CallingConvention: AAPCS64
    Features: [NEON]
    x0:
      type: buffer
      size_bytes: 512
      permissions: write-only
      c_parameter: int16_t r[256]
      description: Output polynomial
    x1:
      type: buffer
      size_bytes: 320
      permissions: read-only
      c_parameter: const uint8_t a[320]
      description: Input compressed polynomial
    x2:
      type: buffer
      size_bytes: 80
      permissions: read-only
      c_parameter: const uint8_t table[80]
      description: Constants (index, shift, mask and multiplier vectors)
  Stack:
    bytes: 0
*/

#include "../../../common.h"
#if defined(MLK_SYS_AARCH64_NEON) && \
    defined(MLK_ARITH_BACKEND_AARCH64) && \
    !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED) && \
    !defined(MLK_CONFIG_NO_DECAPS_API) && \
    (defined(MLK_CONFIG_MULTILEVEL_WITH_SHARED) || MLKEM_K == 2 || MLKEM_K == 3)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/aarch64_opt/src/mlkem_poly_decompress_d10_aarch64_asm.S using scripts/simpasm. Do not modify it directly.
 */

.text
.balign 4
.global MLK_ASM_NAMESPACE(poly_decompress_d10_aarch64_asm)
MLK_ASM_FN_SYMBOL(poly_decompress_d10_aarch64_asm)

        .cfi_startproc
        ld1 { v16.16b, v17.16b, v18.16b, v19.16b }, [x2], #64
        ld1 { v20.16b }, [x2]
        mov x3, #0x8                // =8

Lmlk_poly_decompress_d10_loop_start:
        add x4, x1, #0xa
        add x5, x1, #0x14
        add x6, x1, #0x18
        ld1 { v0.16b }, [x1]
        ld1 { v1.16b }, [x4]
        ld1 { v2.16b }, [x5]
        ld1 { v3.16b }, [x6]
        add x1, x1, #0x28
        tbl v4.16b, { v0.16b }, v16.16b
        ushl v4.8h, v4.8h, v18.8h
        and v4.16b, v4.16b, v19.16b
        tbl v5.16b, { v1.16b }, v16.16b
        ushl v5.8h, v5.8h, v18.8h
        and v5.16b, v5.16b, v19.16b
        tbl v6.16b, { v2.16b }, v16.16b
        ushl v6.8h, v6.8h, v18.8h
        and v6.16b, v6.16b, v19.16b
        tbl v7.16b, { v3.16b }, v17.16b
        ushl v7.8h, v7.8h, v18.8h
        and v7.16b, v7.16b, v19.16b
        sqrdmulh v4.8h, v4.8h, v20.8h
        sqrdmulh v5.8h, v5.8h, v20.8h
        sqrdmulh v6.8h, v6.8h, v20.8h
        sqrdmulh v7.8h, v7.8h, v20.8h
        stp q4, q5, [x0], #0x40
        stp q6, q7, [x0, #-0x20]
        subs x3, x3, #0x1
        cbnz x3, Lmlk_poly_decompress_d10_loop_start
        ret
        .cfi_endproc

MLK_ASM_FN_SIZE(poly_decompress_d10_aarch64_asm)

#endif /* MLK_SYS_AARCH64_NEON && MLK_ARITH_BACKEND_AARCH64 && \
          !MLK_CONFIG_MULTILEVEL_NO_SHARED && !MLK_CONFIG_NO_DECAPS_API && \
          (MLK_CONFIG_MULTILEVEL_WITH_SHARED || MLKEM_K == 2 || MLKEM_K == 3) \
        */

#if defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
