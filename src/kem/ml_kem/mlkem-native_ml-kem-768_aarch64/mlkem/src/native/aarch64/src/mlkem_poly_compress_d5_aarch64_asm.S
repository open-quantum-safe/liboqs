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
  Name: poly_compress_d5_aarch64_asm
  Description: AArch64 polynomial compression (d=5)
  Signature: void mlk_poly_compress_d5_aarch64_asm(uint8_t r[160], const int16_t a[256], const uint8_t table[32])
  ABI:
    Architecture: aarch64
    CallingConvention: AAPCS64
    Features: [NEON]
    x0:
      type: buffer
      size_bytes: 160
      permissions: write-only
      c_parameter: uint8_t r[160]
      description: Output compressed polynomial
    x1:
      type: buffer
      size_bytes: 512
      permissions: read-only
      c_parameter: const int16_t a[256]
      description: Input polynomial
    x2:
      type: buffer
      size_bytes: 32
      permissions: read-only
      c_parameter: const uint8_t table[32]
      description: Constants (multiplier and index vectors)
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
 *   dev/aarch64_opt/src/mlkem_poly_compress_d5_aarch64_asm.S using scripts/simpasm. Do not modify it directly.
 */

.text
.balign 4
.global MLK_ASM_NAMESPACE(poly_compress_d5_aarch64_asm)
MLK_ASM_FN_SYMBOL(poly_compress_d5_aarch64_asm)

        .cfi_startproc
        ld1 { v29.16b, v30.16b }, [x2]
        mov x3, #0x4                // =4

Lmlk_poly_compress_d5_loop_start:
        ld2 { v0.8h, v1.8h }, [x1], #32
        ld2 { v2.8h, v3.8h }, [x1], #32
        ld2 { v4.8h, v5.8h }, [x1], #32
        ld2 { v6.8h, v7.8h }, [x1], #32
        sqrdmulh v0.8h, v0.8h, v29.8h
        sqrdmulh v1.8h, v1.8h, v29.8h
        sqrdmulh v2.8h, v2.8h, v29.8h
        sqrdmulh v3.8h, v3.8h, v29.8h
        sqrdmulh v4.8h, v4.8h, v29.8h
        sqrdmulh v5.8h, v5.8h, v29.8h
        sqrdmulh v6.8h, v6.8h, v29.8h
        sqrdmulh v7.8h, v7.8h, v29.8h
        sli v0.8h, v1.8h, #0x5
        trn2 v16.8h, v0.8h, v0.8h
        sli v0.4s, v16.4s, #0xa
        trn2 v16.4s, v0.4s, v0.4s
        sli v0.2d, v16.2d, #0x14
        sli v2.8h, v3.8h, #0x5
        trn2 v17.8h, v2.8h, v2.8h
        sli v2.4s, v17.4s, #0xa
        trn2 v17.4s, v2.4s, v2.4s
        sli v2.2d, v17.2d, #0x14
        sli v4.8h, v5.8h, #0x5
        trn2 v18.8h, v4.8h, v4.8h
        sli v4.4s, v18.4s, #0xa
        trn2 v18.4s, v4.4s, v4.4s
        sli v4.2d, v18.2d, #0x14
        sli v6.8h, v7.8h, #0x5
        trn2 v19.8h, v6.8h, v6.8h
        sli v6.4s, v19.4s, #0xa
        trn2 v19.4s, v6.4s, v6.4s
        sli v6.2d, v19.2d, #0x14
        tbl v0.16b, { v0.16b }, v30.16b
        tbl v2.16b, { v2.16b }, v30.16b
        tbl v4.16b, { v4.16b }, v30.16b
        tbl v6.16b, { v6.16b }, v30.16b
        ext v20.16b, v0.16b, v2.16b, #0xa
        ext v20.16b, v20.16b, v4.16b, #0x4
        ext v21.16b, v4.16b, v6.16b, #0xa
        st1 { v0.8b }, [x0], #8
        st1 { v20.16b }, [x0], #16
        st1 { v21.16b }, [x0], #16
        subs x3, x3, #0x1
        cbnz x3, Lmlk_poly_compress_d5_loop_start
        ret
        .cfi_endproc

MLK_ASM_FN_SIZE(poly_compress_d5_aarch64_asm)

#endif /* MLK_SYS_AARCH64_NEON && MLK_ARITH_BACKEND_AARCH64 && \
          !MLK_CONFIG_MULTILEVEL_NO_SHARED && (!MLK_CONFIG_NO_ENCAPS_API || \
          !MLK_CONFIG_NO_DECAPS_API) && (MLK_CONFIG_MULTILEVEL_WITH_SHARED || \
          MLKEM_K == 4) */

#if defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
