/*
 * Copyright (c) 2024-2025 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

// Implementation from Kyber reference repository
// https://github.com/pq-crystals/kyber/blob/main/avx2

#include "../../../common.h"
#if defined(MLK_ARITH_BACKEND_X86_64_DEFAULT) && \
    !defined(MLK_MULTILEVEL_BUILD_NO_SHARED)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/x86_64/src/basemul.S using scripts/simpasm. Do not modify it directly.
 */


.text
.balign 4
.global MLK_ASM_NAMESPACE(basemul_avx2)
MLK_ASM_FN_SYMBOL(basemul_avx2)

        movq	%rsp, %r8
        andq	$-0x20, %rsp
        subq	$0x20, %rsp
        leaq	0x2a0(%rcx), %r9
        vmovdqa	0x20(%rcx), %ymm0
        vmovdqa	(%rsi), %ymm1
        vmovdqa	0x20(%rsi), %ymm2
        vmovdqa	0x40(%rsi), %ymm3
        vmovdqa	0x60(%rsi), %ymm4
        vpmullw	%ymm0, %ymm1, %ymm9
        vpmullw	%ymm0, %ymm2, %ymm10
        vpmullw	%ymm0, %ymm3, %ymm11
        vpmullw	%ymm0, %ymm4, %ymm12
        vmovdqa	(%rdx), %ymm5
        vmovdqa	0x20(%rdx), %ymm6
        vpmulhw	%ymm5, %ymm1, %ymm13
        vpmulhw	%ymm6, %ymm1, %ymm1
        vpmulhw	%ymm5, %ymm2, %ymm14
        vpmulhw	%ymm6, %ymm2, %ymm2
        vmovdqa	0x40(%rdx), %ymm7
        vmovdqa	0x60(%rdx), %ymm8
        vpmulhw	%ymm7, %ymm3, %ymm15
        vpmulhw	%ymm8, %ymm3, %ymm3
        vpmulhw	%ymm7, %ymm4, %ymm0
        vpmulhw	%ymm8, %ymm4, %ymm4
        vmovdqa	%ymm13, (%rsp)
        vpmullw	%ymm5, %ymm9, %ymm13
        vpmullw	%ymm6, %ymm9, %ymm9
        vpmullw	%ymm5, %ymm10, %ymm5
        vpmullw	%ymm6, %ymm10, %ymm10
        vpmullw	%ymm7, %ymm11, %ymm6
        vpmullw	%ymm8, %ymm11, %ymm11
        vpmullw	%ymm7, %ymm12, %ymm7
        vpmullw	%ymm8, %ymm12, %ymm12
        vmovdqa	(%rcx), %ymm8
        vpmulhw	%ymm8, %ymm13, %ymm13
        vpmulhw	%ymm8, %ymm9, %ymm9
        vpmulhw	%ymm8, %ymm5, %ymm5
        vpmulhw	%ymm8, %ymm10, %ymm10
        vpmulhw	%ymm8, %ymm6, %ymm6
        vpmulhw	%ymm8, %ymm11, %ymm11
        vpmulhw	%ymm8, %ymm7, %ymm7
        vpmulhw	%ymm8, %ymm12, %ymm12
        vpsubw	(%rsp), %ymm13, %ymm13
        vpsubw	%ymm9, %ymm1, %ymm9
        vpsubw	%ymm5, %ymm14, %ymm5
        vpsubw	%ymm10, %ymm2, %ymm10
        vpsubw	%ymm6, %ymm15, %ymm6
        vpsubw	%ymm11, %ymm3, %ymm11
        vpsubw	%ymm7, %ymm0, %ymm7
        vpsubw	%ymm12, %ymm4, %ymm12
        vmovdqa	(%r9), %ymm0
        vmovdqa	0x20(%r9), %ymm1
        vpmullw	%ymm0, %ymm10, %ymm2
        vpmullw	%ymm0, %ymm12, %ymm3
        vpmulhw	%ymm1, %ymm10, %ymm10
        vpmulhw	%ymm1, %ymm12, %ymm12
        vpmulhw	%ymm8, %ymm2, %ymm2
        vpmulhw	%ymm8, %ymm3, %ymm3
        vpsubw	%ymm2, %ymm10, %ymm10
        vpsubw	%ymm3, %ymm12, %ymm12
        vpaddw	%ymm5, %ymm9, %ymm9
        vpaddw	%ymm7, %ymm11, %ymm11
        vpsubw	%ymm13, %ymm10, %ymm13
        vpsubw	%ymm12, %ymm6, %ymm6
        vmovdqa	%ymm13, (%rdi)
        vmovdqa	%ymm9, 0x20(%rdi)
        vmovdqa	%ymm6, 0x40(%rdi)
        vmovdqa	%ymm11, 0x60(%rdi)
        addq	$0x40, %r9
        vmovdqa	0x20(%rcx), %ymm0
        vmovdqa	0x80(%rsi), %ymm1
        vmovdqa	0xa0(%rsi), %ymm2
        vmovdqa	0xc0(%rsi), %ymm3
        vmovdqa	0xe0(%rsi), %ymm4
        vpmullw	%ymm0, %ymm1, %ymm9
        vpmullw	%ymm0, %ymm2, %ymm10
        vpmullw	%ymm0, %ymm3, %ymm11
        vpmullw	%ymm0, %ymm4, %ymm12
        vmovdqa	0x80(%rdx), %ymm5
        vmovdqa	0xa0(%rdx), %ymm6
        vpmulhw	%ymm5, %ymm1, %ymm13
        vpmulhw	%ymm6, %ymm1, %ymm1
        vpmulhw	%ymm5, %ymm2, %ymm14
        vpmulhw	%ymm6, %ymm2, %ymm2
        vmovdqa	0xc0(%rdx), %ymm7
        vmovdqa	0xe0(%rdx), %ymm8
        vpmulhw	%ymm7, %ymm3, %ymm15
        vpmulhw	%ymm8, %ymm3, %ymm3
        vpmulhw	%ymm7, %ymm4, %ymm0
        vpmulhw	%ymm8, %ymm4, %ymm4
        vmovdqa	%ymm13, (%rsp)
        vpmullw	%ymm5, %ymm9, %ymm13
        vpmullw	%ymm6, %ymm9, %ymm9
        vpmullw	%ymm5, %ymm10, %ymm5
        vpmullw	%ymm6, %ymm10, %ymm10
        vpmullw	%ymm7, %ymm11, %ymm6
        vpmullw	%ymm8, %ymm11, %ymm11
        vpmullw	%ymm7, %ymm12, %ymm7
        vpmullw	%ymm8, %ymm12, %ymm12
        vmovdqa	(%rcx), %ymm8
        vpmulhw	%ymm8, %ymm13, %ymm13
        vpmulhw	%ymm8, %ymm9, %ymm9
        vpmulhw	%ymm8, %ymm5, %ymm5
        vpmulhw	%ymm8, %ymm10, %ymm10
        vpmulhw	%ymm8, %ymm6, %ymm6
        vpmulhw	%ymm8, %ymm11, %ymm11
        vpmulhw	%ymm8, %ymm7, %ymm7
        vpmulhw	%ymm8, %ymm12, %ymm12
        vpsubw	(%rsp), %ymm13, %ymm13
        vpsubw	%ymm9, %ymm1, %ymm9
        vpsubw	%ymm5, %ymm14, %ymm5
        vpsubw	%ymm10, %ymm2, %ymm10
        vpsubw	%ymm6, %ymm15, %ymm6
        vpsubw	%ymm11, %ymm3, %ymm11
        vpsubw	%ymm7, %ymm0, %ymm7
        vpsubw	%ymm12, %ymm4, %ymm12
        vmovdqa	(%r9), %ymm0
        vmovdqa	0x20(%r9), %ymm1
        vpmullw	%ymm0, %ymm10, %ymm2
        vpmullw	%ymm0, %ymm12, %ymm3
        vpmulhw	%ymm1, %ymm10, %ymm10
        vpmulhw	%ymm1, %ymm12, %ymm12
        vpmulhw	%ymm8, %ymm2, %ymm2
        vpmulhw	%ymm8, %ymm3, %ymm3
        vpsubw	%ymm2, %ymm10, %ymm10
        vpsubw	%ymm3, %ymm12, %ymm12
        vpaddw	%ymm5, %ymm9, %ymm9
        vpaddw	%ymm7, %ymm11, %ymm11
        vpsubw	%ymm13, %ymm10, %ymm13
        vpsubw	%ymm12, %ymm6, %ymm6
        vmovdqa	%ymm13, 0x80(%rdi)
        vmovdqa	%ymm9, 0xa0(%rdi)
        vmovdqa	%ymm6, 0xc0(%rdi)
        vmovdqa	%ymm11, 0xe0(%rdi)
        addq	$0x180, %r9             # imm = 0x180
        vmovdqa	0x20(%rcx), %ymm0
        vmovdqa	0x100(%rsi), %ymm1
        vmovdqa	0x120(%rsi), %ymm2
        vmovdqa	0x140(%rsi), %ymm3
        vmovdqa	0x160(%rsi), %ymm4
        vpmullw	%ymm0, %ymm1, %ymm9
        vpmullw	%ymm0, %ymm2, %ymm10
        vpmullw	%ymm0, %ymm3, %ymm11
        vpmullw	%ymm0, %ymm4, %ymm12
        vmovdqa	0x100(%rdx), %ymm5
        vmovdqa	0x120(%rdx), %ymm6
        vpmulhw	%ymm5, %ymm1, %ymm13
        vpmulhw	%ymm6, %ymm1, %ymm1
        vpmulhw	%ymm5, %ymm2, %ymm14
        vpmulhw	%ymm6, %ymm2, %ymm2
        vmovdqa	0x140(%rdx), %ymm7
        vmovdqa	0x160(%rdx), %ymm8
        vpmulhw	%ymm7, %ymm3, %ymm15
        vpmulhw	%ymm8, %ymm3, %ymm3
        vpmulhw	%ymm7, %ymm4, %ymm0
        vpmulhw	%ymm8, %ymm4, %ymm4
        vmovdqa	%ymm13, (%rsp)
        vpmullw	%ymm5, %ymm9, %ymm13
        vpmullw	%ymm6, %ymm9, %ymm9
        vpmullw	%ymm5, %ymm10, %ymm5
        vpmullw	%ymm6, %ymm10, %ymm10
        vpmullw	%ymm7, %ymm11, %ymm6
        vpmullw	%ymm8, %ymm11, %ymm11
        vpmullw	%ymm7, %ymm12, %ymm7
        vpmullw	%ymm8, %ymm12, %ymm12
        vmovdqa	(%rcx), %ymm8
        vpmulhw	%ymm8, %ymm13, %ymm13
        vpmulhw	%ymm8, %ymm9, %ymm9
        vpmulhw	%ymm8, %ymm5, %ymm5
        vpmulhw	%ymm8, %ymm10, %ymm10
        vpmulhw	%ymm8, %ymm6, %ymm6
        vpmulhw	%ymm8, %ymm11, %ymm11
        vpmulhw	%ymm8, %ymm7, %ymm7
        vpmulhw	%ymm8, %ymm12, %ymm12
        vpsubw	(%rsp), %ymm13, %ymm13
        vpsubw	%ymm9, %ymm1, %ymm9
        vpsubw	%ymm5, %ymm14, %ymm5
        vpsubw	%ymm10, %ymm2, %ymm10
        vpsubw	%ymm6, %ymm15, %ymm6
        vpsubw	%ymm11, %ymm3, %ymm11
        vpsubw	%ymm7, %ymm0, %ymm7
        vpsubw	%ymm12, %ymm4, %ymm12
        vmovdqa	(%r9), %ymm0
        vmovdqa	0x20(%r9), %ymm1
        vpmullw	%ymm0, %ymm10, %ymm2
        vpmullw	%ymm0, %ymm12, %ymm3
        vpmulhw	%ymm1, %ymm10, %ymm10
        vpmulhw	%ymm1, %ymm12, %ymm12
        vpmulhw	%ymm8, %ymm2, %ymm2
        vpmulhw	%ymm8, %ymm3, %ymm3
        vpsubw	%ymm2, %ymm10, %ymm10
        vpsubw	%ymm3, %ymm12, %ymm12
        vpaddw	%ymm5, %ymm9, %ymm9
        vpaddw	%ymm7, %ymm11, %ymm11
        vpsubw	%ymm13, %ymm10, %ymm13
        vpsubw	%ymm12, %ymm6, %ymm6
        vmovdqa	%ymm13, 0x100(%rdi)
        vmovdqa	%ymm9, 0x120(%rdi)
        vmovdqa	%ymm6, 0x140(%rdi)
        vmovdqa	%ymm11, 0x160(%rdi)
        addq	$0x40, %r9
        vmovdqa	0x20(%rcx), %ymm0
        vmovdqa	0x180(%rsi), %ymm1
        vmovdqa	0x1a0(%rsi), %ymm2
        vmovdqa	0x1c0(%rsi), %ymm3
        vmovdqa	0x1e0(%rsi), %ymm4
        vpmullw	%ymm0, %ymm1, %ymm9
        vpmullw	%ymm0, %ymm2, %ymm10
        vpmullw	%ymm0, %ymm3, %ymm11
        vpmullw	%ymm0, %ymm4, %ymm12
        vmovdqa	0x180(%rdx), %ymm5
        vmovdqa	0x1a0(%rdx), %ymm6
        vpmulhw	%ymm5, %ymm1, %ymm13
        vpmulhw	%ymm6, %ymm1, %ymm1
        vpmulhw	%ymm5, %ymm2, %ymm14
        vpmulhw	%ymm6, %ymm2, %ymm2
        vmovdqa	0x1c0(%rdx), %ymm7
        vmovdqa	0x1e0(%rdx), %ymm8
        vpmulhw	%ymm7, %ymm3, %ymm15
        vpmulhw	%ymm8, %ymm3, %ymm3
        vpmulhw	%ymm7, %ymm4, %ymm0
        vpmulhw	%ymm8, %ymm4, %ymm4
        vmovdqa	%ymm13, (%rsp)
        vpmullw	%ymm5, %ymm9, %ymm13
        vpmullw	%ymm6, %ymm9, %ymm9
        vpmullw	%ymm5, %ymm10, %ymm5
        vpmullw	%ymm6, %ymm10, %ymm10
        vpmullw	%ymm7, %ymm11, %ymm6
        vpmullw	%ymm8, %ymm11, %ymm11
        vpmullw	%ymm7, %ymm12, %ymm7
        vpmullw	%ymm8, %ymm12, %ymm12
        vmovdqa	(%rcx), %ymm8
        vpmulhw	%ymm8, %ymm13, %ymm13
        vpmulhw	%ymm8, %ymm9, %ymm9
        vpmulhw	%ymm8, %ymm5, %ymm5
        vpmulhw	%ymm8, %ymm10, %ymm10
        vpmulhw	%ymm8, %ymm6, %ymm6
        vpmulhw	%ymm8, %ymm11, %ymm11
        vpmulhw	%ymm8, %ymm7, %ymm7
        vpmulhw	%ymm8, %ymm12, %ymm12
        vpsubw	(%rsp), %ymm13, %ymm13
        vpsubw	%ymm9, %ymm1, %ymm9
        vpsubw	%ymm5, %ymm14, %ymm5
        vpsubw	%ymm10, %ymm2, %ymm10
        vpsubw	%ymm6, %ymm15, %ymm6
        vpsubw	%ymm11, %ymm3, %ymm11
        vpsubw	%ymm7, %ymm0, %ymm7
        vpsubw	%ymm12, %ymm4, %ymm12
        vmovdqa	(%r9), %ymm0
        vmovdqa	0x20(%r9), %ymm1
        vpmullw	%ymm0, %ymm10, %ymm2
        vpmullw	%ymm0, %ymm12, %ymm3
        vpmulhw	%ymm1, %ymm10, %ymm10
        vpmulhw	%ymm1, %ymm12, %ymm12
        vpmulhw	%ymm8, %ymm2, %ymm2
        vpmulhw	%ymm8, %ymm3, %ymm3
        vpsubw	%ymm2, %ymm10, %ymm10
        vpsubw	%ymm3, %ymm12, %ymm12
        vpaddw	%ymm5, %ymm9, %ymm9
        vpaddw	%ymm7, %ymm11, %ymm11
        vpsubw	%ymm13, %ymm10, %ymm13
        vpsubw	%ymm12, %ymm6, %ymm6
        vmovdqa	%ymm13, 0x180(%rdi)
        vmovdqa	%ymm9, 0x1a0(%rdi)
        vmovdqa	%ymm6, 0x1c0(%rdi)
        vmovdqa	%ymm11, 0x1e0(%rdi)
        movq	%r8, %rsp
        retq

#endif /* defined(MLK_ARITH_BACKEND_X86_64_DEFAULT) && \
          !defined(MLK_MULTILEVEL_BUILD_NO_SHARED) */
