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
  Name: poly_decompress_d5_aarch64_asm
  Description: Decompression of a polynomial from 5 bits per coefficient
  Signature: void mlk_poly_decompress_d5_aarch64_asm(int16_t r[256], const uint8_t a[160], const uint8_t table[112])
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
      size_bytes: 160
      permissions: read-only
      c_parameter: const uint8_t a[160]
      description: Input compressed polynomial
    x2:
      type: buffer
      size_bytes: 112
      permissions: read-only
      c_parameter: const uint8_t table[112]
      description: Constants (index, shift, mask and multiplier vectors)
  Stack:
    bytes: 0
*/

#include "../../../common.h"
#if defined(MLK_SYS_AARCH64_NEON) && \
    defined(MLK_ARITH_BACKEND_AARCH64) && \
    !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED) && \
    !defined(MLK_CONFIG_NO_DECAPS_API) && \
    (defined(MLK_CONFIG_MULTILEVEL_WITH_SHARED) || MLKEM_K == 4)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/aarch64_opt/src/mlkem_poly_decompress_d5_aarch64_asm.S using scripts/simpasm. Do not modify it directly.
 */

.text
.balign 4
.global MLK_ASM_NAMESPACE(poly_decompress_d5_aarch64_asm)
MLK_ASM_FN_SYMBOL(poly_decompress_d5_aarch64_asm)

        .cfi_startproc
        ld1 { v16.16b, v17.16b, v18.16b, v19.16b }, [x2], #64
        ld1 { v20.16b, v21.16b }, [x2], #32
        ld1 { v22.16b }, [x2]
        mov x3, #0x8                // =8

Lmlk_poly_decompress_d5_loop_start:
        add x4, x1, #0x4
        ld1 { v0.16b }, [x1]
        ld1 { v1.16b }, [x4]
        add x1, x1, #0x14
        tbl v2.16b, { v0.16b }, v16.16b
        ushl v2.8h, v2.8h, v20.8h
        and v2.16b, v2.16b, v21.16b
        tbl v3.16b, { v0.16b }, v17.16b
        ushl v3.8h, v3.8h, v20.8h
        and v3.16b, v3.16b, v21.16b
        tbl v4.16b, { v0.16b }, v18.16b
        ushl v4.8h, v4.8h, v20.8h
        and v4.16b, v4.16b, v21.16b
        tbl v5.16b, { v1.16b }, v19.16b
        ushl v5.8h, v5.8h, v20.8h
        and v5.16b, v5.16b, v21.16b
        sqrdmulh v2.8h, v2.8h, v22.8h
        sqrdmulh v3.8h, v3.8h, v22.8h
        sqrdmulh v4.8h, v4.8h, v22.8h
        sqrdmulh v5.8h, v5.8h, v22.8h
        stp q2, q3, [x0], #0x40
        stp q4, q5, [x0, #-0x20]
        subs x3, x3, #0x1
        cbnz x3, Lmlk_poly_decompress_d5_loop_start
        ret
        .cfi_endproc

MLK_ASM_FN_SIZE(poly_decompress_d5_aarch64_asm)

#endif /* MLK_SYS_AARCH64_NEON && MLK_ARITH_BACKEND_AARCH64 && \
          !MLK_CONFIG_MULTILEVEL_NO_SHARED && !MLK_CONFIG_NO_DECAPS_API && \
          (MLK_CONFIG_MULTILEVEL_WITH_SHARED || MLKEM_K == 4) */

#if defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
