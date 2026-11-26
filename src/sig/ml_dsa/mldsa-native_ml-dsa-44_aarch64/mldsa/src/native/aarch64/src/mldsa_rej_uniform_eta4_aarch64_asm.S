/*
 * Copyright (c) The mldsa-native project authors
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/*yaml
  Name: rej_uniform_eta4_aarch64_asm
  Description: AArch64 rejection sampling of eta=4 secret coefficients
  Signature: uint64_t mld_rej_uniform_eta4_aarch64_asm(int32_t r[256], const uint8_t *buf, uint64_t buflen, const uint8_t table[4096])
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
      description: Length of input buffer (must be a positive multiple of 8)
      test_with: 272  # MLD_AARCH64_REJ_UNIFORM_ETA4_BUFLEN
    x3:
      type: buffer
      size_bytes: 4096
      permissions: read-only
      c_parameter: const uint8_t table[4096]
      description: Lookup table (4096 x uint8_t)
*/

#include "../../../common.h"
#if defined(MLD_SYS_AARCH64_NEON) && \
    defined(MLD_ARITH_BACKEND_AARCH64) && \
    !defined(MLD_CONFIG_NO_KEYPAIR_API) && \
    !defined(MLD_CONFIG_MULTILEVEL_NO_SHARED) && \
    (defined(MLD_CONFIG_MULTILEVEL_WITH_SHARED) || MLDSA_ETA == 4)

/*
 * WARNING: This file is auto-derived from the mldsa-native source file
 *   dev/aarch64_opt/src/mldsa_rej_uniform_eta4_aarch64_asm.S using scripts/simpasm. Do not modify it directly.
 */

.text
.balign 4
.global MLD_ASM_NAMESPACE(rej_uniform_eta4_aarch64_asm)
MLD_ASM_FN_SYMBOL(rej_uniform_eta4_aarch64_asm)

        .cfi_startproc
        mov x5, #0x101              // =257
        movk x5, #0x102, lsl #16
        movk x5, #0x104, lsl #32
        movk x5, #0x108, lsl #48
        mov v31.d[0], x5
        mov x5, #0x110              // =272
        movk x5, #0x120, lsl #16
        movk x5, #0x140, lsl #32
        movk x5, #0x180, lsl #48
        mov v31.d[1], x5
        movi v30.16b, #0x9
        movi v7.8h, #0x4
        movi v29.8b, #0xf
        mov x4, #0x100              // =256

Lmld_rej_uniform_eta4_loop:
        ld1 { v0.8b }, [x1], #8
        and v2.8b, v0.8b, v29.8b
        ushr v3.8b, v0.8b, #0x4
        zip1 v6.16b, v2.16b, v3.16b
        cmhi v4.16b, v30.16b, v6.16b
        sshll2 v5.8h, v4.16b, #0x0
        sshll v4.8h, v4.8b, #0x0
        and v4.16b, v4.16b, v31.16b
        and v5.16b, v5.16b, v31.16b
        uaddlv s20, v4.8h
        uaddlv s21, v5.8h
        fmov x12, d20
        fmov x13, d21
        and x14, x12, #0xff
        and x15, x13, #0xff
        lsr x12, x12, #8
        lsr x13, x13, #8
        ldr q24, [x3, x14, lsl #4]
        ldr q25, [x3, x15, lsl #4]
        ushll v16.8h, v6.8b, #0x0
        ushll2 v18.8h, v6.16b, #0x0
        tbl v16.16b, { v16.16b }, v24.16b
        tbl v18.16b, { v18.16b }, v25.16b
        usubl2 v17.4s, v7.8h, v16.8h
        usubl v16.4s, v7.4h, v16.4h
        usubl2 v19.4s, v7.8h, v18.8h
        usubl v18.4s, v7.4h, v18.4h
        cmp x4, #0x10
        b.ls Lmld_rej_uniform_eta4_tail
        st1 { v16.4s, v17.4s }, [x0]
        add x0, x0, x12, lsl #2
        st1 { v18.4s, v19.4s }, [x0]
        add x0, x0, x13, lsl #2
        sub x4, x4, x12
        sub x4, x4, x13

Lmld_rej_uniform_eta4_next:
        subs x2, x2, #0x8
        b.ne Lmld_rej_uniform_eta4_loop
        b Lmld_rej_uniform_eta4_return

Lmld_rej_uniform_eta4_tail:
        mov x6, #0x2                // =2

Lmld_rej_uniform_eta4_store_group:
        cmp x4, #0x8
        b.lo Lmld_rej_uniform_eta4_store_partial
        st1 { v16.4s, v17.4s }, [x0]
        b Lmld_rej_uniform_eta4_stored

Lmld_rej_uniform_eta4_store_partial:
        mov x7, x0
        tbz w4, #0x2, Lmld_rej_uniform_eta4_store_two
        st1 { v16.4s }, [x7], #16
        mov v16.16b, v17.16b

Lmld_rej_uniform_eta4_store_two:
        tbz w4, #0x1, Lmld_rej_uniform_eta4_store_one
        st1 { v16.2s }, [x7], #8
        ext v16.16b, v16.16b, v16.16b, #0x8

Lmld_rej_uniform_eta4_store_one:
        tbz w4, #0x0, Lmld_rej_uniform_eta4_stored
        str s16, [x7]

Lmld_rej_uniform_eta4_stored:
        subs x4, x4, x12
        b.ls Lmld_rej_uniform_eta4_full
        add x0, x0, x12, lsl #2
        subs x6, x6, #0x1
        b.eq Lmld_rej_uniform_eta4_next
        mov v16.16b, v18.16b
        mov v17.16b, v19.16b
        mov x12, x13
        b Lmld_rej_uniform_eta4_store_group

Lmld_rej_uniform_eta4_full:
        mov x4, #0x0                // =0

Lmld_rej_uniform_eta4_return:
        mov x0, #0x100              // =256
        sub x0, x0, x4
        ret
        .cfi_endproc

MLD_ASM_FN_SIZE(rej_uniform_eta4_aarch64_asm)

#endif /* MLD_SYS_AARCH64_NEON && MLD_ARITH_BACKEND_AARCH64 && \
          !MLD_CONFIG_NO_KEYPAIR_API && !MLD_CONFIG_MULTILEVEL_NO_SHARED && \
          (MLD_CONFIG_MULTILEVEL_WITH_SHARED || MLDSA_ETA == 4) */

#if defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
