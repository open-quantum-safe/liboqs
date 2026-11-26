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
  Name: poly_compress_d4_aarch64_asm
  Description: AArch64 polynomial compression (d=4)
  Signature: void mlk_poly_compress_d4_aarch64_asm(uint8_t r[128], const int16_t a[256])
  ABI:
    Architecture: aarch64
    CallingConvention: AAPCS64
    Features: [NEON]
    x0:
      type: buffer
      size_bytes: 128
      permissions: write-only
      c_parameter: uint8_t r[128]
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
 *   dev/aarch64_opt/src/mlkem_poly_compress_d4_aarch64_asm.S using scripts/simpasm. Do not modify it directly.
 */

.text
.balign 4
.global MLK_ASM_NAMESPACE(poly_compress_d4_aarch64_asm)
MLK_ASM_FN_SYMBOL(poly_compress_d4_aarch64_asm)

        .cfi_startproc
        mov w3, #0x9d8              // =2520
        dup v24.8h, w3
        movi v25.16b, #0x8
        mov x2, #0x4                // =4

Lmlk_poly_compress_d4_loop_start:
        ldp q0, q1, [x1], #0x80
        ldp q2, q3, [x1, #-0x60]
        ldp q4, q5, [x1, #-0x40]
        ldp q6, q7, [x1, #-0x20]
        sqdmulh v0.8h, v0.8h, v24.8h
        sqdmulh v1.8h, v1.8h, v24.8h
        sqdmulh v2.8h, v2.8h, v24.8h
        sqdmulh v3.8h, v3.8h, v24.8h
        sqdmulh v4.8h, v4.8h, v24.8h
        sqdmulh v5.8h, v5.8h, v24.8h
        sqdmulh v6.8h, v6.8h, v24.8h
        sqdmulh v7.8h, v7.8h, v24.8h
        uzp1 v16.16b, v0.16b, v1.16b
        uzp1 v17.16b, v2.16b, v3.16b
        uzp1 v20.16b, v16.16b, v17.16b
        uzp2 v21.16b, v16.16b, v17.16b
        uzp1 v18.16b, v4.16b, v5.16b
        uzp1 v19.16b, v6.16b, v7.16b
        uzp1 v22.16b, v18.16b, v19.16b
        uzp2 v23.16b, v18.16b, v19.16b
        add v20.16b, v20.16b, v25.16b
        add v21.16b, v21.16b, v25.16b
        sri v21.16b, v20.16b, #0x4
        add v22.16b, v22.16b, v25.16b
        add v23.16b, v23.16b, v25.16b
        sri v23.16b, v22.16b, #0x4
        st1 { v21.16b }, [x0], #16
        st1 { v23.16b }, [x0], #16
        subs x2, x2, #0x1
        cbnz x2, Lmlk_poly_compress_d4_loop_start
        ret
        .cfi_endproc

MLK_ASM_FN_SIZE(poly_compress_d4_aarch64_asm)

#endif /* MLK_SYS_AARCH64_NEON && MLK_ARITH_BACKEND_AARCH64 && \
          !MLK_CONFIG_MULTILEVEL_NO_SHARED && (!MLK_CONFIG_NO_ENCAPS_API || \
          !MLK_CONFIG_NO_DECAPS_API) && (MLK_CONFIG_MULTILEVEL_WITH_SHARED || \
          MLKEM_K == 2 || MLKEM_K == 3) */

#if defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
