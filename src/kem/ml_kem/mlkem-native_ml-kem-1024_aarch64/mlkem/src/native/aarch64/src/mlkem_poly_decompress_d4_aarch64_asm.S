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
  Name: poly_decompress_d4_aarch64_asm
  Description: Decompression of a polynomial from 4 bits per coefficient
  Signature: void mlk_poly_decompress_d4_aarch64_asm(int16_t r[256], const uint8_t a[128], const uint8_t table[32])
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
      size_bytes: 128
      permissions: read-only
      c_parameter: const uint8_t a[128]
      description: Input compressed polynomial
    x2:
      type: buffer
      size_bytes: 32
      permissions: read-only
      c_parameter: const uint8_t table[32]
      description: Lookup table for Decompress_4
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
 *   dev/aarch64_opt/src/mlkem_poly_decompress_d4_aarch64_asm.S using scripts/simpasm. Do not modify it directly.
 */

.text
.balign 4
.global MLK_ASM_NAMESPACE(poly_decompress_d4_aarch64_asm)
MLK_ASM_FN_SYMBOL(poly_decompress_d4_aarch64_asm)

        .cfi_startproc
        ld1 { v17.16b, v18.16b }, [x2]
        movi v19.16b, #0xf
        mov x3, #0x8                // =8

Lmlk_poly_decompress_d4_loop_start:
        ld1 { v0.16b }, [x1], #16
        and v1.16b, v0.16b, v19.16b
        ushr v2.16b, v0.16b, #0x4
        zip1 v3.16b, v1.16b, v2.16b
        zip2 v16.16b, v1.16b, v2.16b
        tbl v4.16b, { v17.16b }, v3.16b
        tbl v5.16b, { v18.16b }, v3.16b
        tbl v6.16b, { v17.16b }, v16.16b
        tbl v7.16b, { v18.16b }, v16.16b
        st2 { v4.16b, v5.16b }, [x0], #32
        st2 { v6.16b, v7.16b }, [x0], #32
        subs x3, x3, #0x1
        cbnz x3, Lmlk_poly_decompress_d4_loop_start
        ret
        .cfi_endproc

MLK_ASM_FN_SIZE(poly_decompress_d4_aarch64_asm)

#endif /* MLK_SYS_AARCH64_NEON && MLK_ARITH_BACKEND_AARCH64 && \
          !MLK_CONFIG_MULTILEVEL_NO_SHARED && !MLK_CONFIG_NO_DECAPS_API && \
          (MLK_CONFIG_MULTILEVEL_WITH_SHARED || MLKEM_K == 2 || MLKEM_K == 3) \
        */

#if defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
