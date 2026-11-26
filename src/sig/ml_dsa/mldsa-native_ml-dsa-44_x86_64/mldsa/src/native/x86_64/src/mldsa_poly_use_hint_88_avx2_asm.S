/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* References
 * ==========
 *
 * - [REF_AVX2]
 *   CRYSTALS-Dilithium optimized AVX2 implementation
 *   Bai, Ducas, Kiltz, Lepoint, Lyubashevsky, Schwabe, Seiler, Stehlé
 *   https://github.com/pq-crystals/dilithium/tree/master/avx2
 */

/*
 * This file is derived from the public domain
 * AVX2 Dilithium implementation @[REF_AVX2].
 */


/*yaml
  Name: poly_use_hint_88_avx2_asm
  Description: x86_64 AVX2 hint application (alpha = (Q-1)/88).
    Use the hint polynomial h to correct the high bits of the polynomial a,
    in place. Variant for parameter set ML-DSA-44 (GAMMA2 = (Q-1)/88).
  Signature: void mld_poly_use_hint_88_avx2_asm(int32_t *a, const int32_t *h)
  ABI:
    Architecture: x86_64
    CallingConvention: SysV
    Features: [AVX2]
    rdi:
      type: buffer
      size_bytes: 1024
      permissions: read/write
      c_parameter: int32_t *a
      description: Input/output polynomial (256 x int32_t)
    rsi:
      type: buffer
      size_bytes: 1024
      permissions: read-only
      c_parameter: const int32_t *h
      description: Hint polynomial (256 x int32_t)
*/

#include "../../../common.h"

#if defined(MLD_SYS_X86_64_AVX2) && \
    defined(MLD_ARITH_BACKEND_X86_64_DEFAULT) &&   \
    !defined(MLD_CONFIG_NO_VERIFY_API) &&          \
    !defined(MLD_CONFIG_MULTILEVEL_NO_SHARED) &&   \
    (defined(MLD_CONFIG_MULTILEVEL_WITH_SHARED) || \
    MLD_CONFIG_PARAMETER_SET == 44)

/*
 * WARNING: This file is auto-derived from the mldsa-native source file
 *   dev/x86_64/src/mldsa_poly_use_hint_88_avx2_asm.S using scripts/simpasm. Do not modify it directly.
 */

.text
.balign 4
.global MLD_ASM_NAMESPACE(poly_use_hint_88_avx2_asm)
MLD_ASM_FN_SYMBOL(poly_use_hint_88_avx2_asm)

        .cfi_startproc
        movl $0x7f, %ecx
        xorl %eax, %eax
        vpxor %xmm5, %xmm5, %xmm5
        movl $0x2c0b, %r8d           # imm = 0x2C0B
        vmovd %r8d, %xmm8
        vpbroadcastd %xmm8, %ymm8
        vmovd %ecx, %xmm4
        movl $0x80, %r9d
        vmovd %r9d, %xmm7
        vpbroadcastd %xmm7, %ymm7
        movl $0x2b, %r10d
        vmovd %r10d, %xmm6
        vpbroadcastd %xmm6, %ymm6
        vpbroadcastd %xmm4, %ymm4

Lmld_poly_use_hint_88_avx2_asm_loop:
        vmovdqa (%rdi), %ymm0
        vmovdqa (%rsi), %ymm1
        vpaddd %ymm0, %ymm4, %ymm9
        vpsrld $0x7, %ymm9, %ymm9
        vpmulhuw %ymm8, %ymm9, %ymm9
        vpmulhrsw %ymm7, %ymm9, %ymm9
        vpslld $0x1, %ymm9, %ymm11
        vpaddd %ymm9, %ymm11, %ymm11
        vpslld $0x5, %ymm11, %ymm10
        vpsubd %ymm11, %ymm10, %ymm10
        vpslld $0xb, %ymm10, %ymm10
        vpsubd %ymm10, %ymm0, %ymm0
        vpcmpgtd %ymm5, %ymm0, %ymm0
        vpandn %ymm1, %ymm0, %ymm0
        vpslld $0x1, %ymm0, %ymm0
        vpsubd %ymm0, %ymm1, %ymm0
        vpaddd %ymm0, %ymm9, %ymm0
        vpblendvb %ymm0, %ymm6, %ymm0, %ymm0
        vpcmpgtd %ymm6, %ymm0, %ymm1
        vpandn %ymm0, %ymm1, %ymm0
        vmovdqa %ymm0, (%rdi)
        addq $0x20, %rdi
        addq $0x20, %rsi
        addq $0x20, %rax
        cmpq $0x400, %rax            # imm = 0x400
        jne Lmld_poly_use_hint_88_avx2_asm_loop
        vzeroupper
        retq
        .cfi_endproc

MLD_ASM_FN_SIZE(poly_use_hint_88_avx2_asm)


#endif /* MLD_SYS_X86_64_AVX2 && MLD_ARITH_BACKEND_X86_64_DEFAULT && \
          !MLD_CONFIG_NO_VERIFY_API && !MLD_CONFIG_MULTILEVEL_NO_SHARED && \
          (MLD_CONFIG_MULTILEVEL_WITH_SHARED || MLD_CONFIG_PARAMETER_SET == 44) \
        */

#if defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
