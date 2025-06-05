/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* References
 * ==========
 *
 * - [REF_AVX2]
 *   CRYSTALS-Kyber optimized AVX2 implementation
 *   Bos, Ducas, Kiltz, Lepoint, Lyubashevsky, Schanck, Schwabe, Seiler, Stehlé
 *   https://github.com/pq-crystals/kyber/tree/main/avx2
 */

/*
 * This file is derived from the public domain
 * AVX2 Kyber implementation @[REF_AVX2].
 *
 * The main difference is the use of a mulcache.
 */

#include "../../../common.h"
#if defined(MLK_ARITH_BACKEND_X86_64_DEFAULT) && \
    !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/x86_64/src/basemul.S using scripts/simpasm. Do not modify it directly.
 */


.text
.balign 4
.global MLK_ASM_NAMESPACE(basemul_avx2)
MLK_ASM_FN_SYMBOL(basemul_avx2)

        movq	%rsp, %r11
        andq	$-0x20, %rsp
        subq	$0x20, %rsp
        vmovdqa	0x20(%r8), %ymm0
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
        vmovdqa	0x40(%rdx), %ymm7
        vmovdqa	0x60(%rdx), %ymm8
        vpmulhw	%ymm7, %ymm3, %ymm15
        vpmulhw	%ymm8, %ymm3, %ymm3
        vpmulhw	%ymm7, %ymm4, %ymm0
        vmovdqa	%ymm13, (%rsp)
        vpmullw	%ymm5, %ymm9, %ymm13
        vpmullw	%ymm6, %ymm9, %ymm9
        vpmullw	%ymm5, %ymm10, %ymm5
        vpmullw	%ymm7, %ymm11, %ymm6
        vpmullw	%ymm8, %ymm11, %ymm11
        vpmullw	%ymm7, %ymm12, %ymm7
        vmovdqa	(%rcx), %ymm8
        vpmulhw	%ymm8, %ymm2, %ymm2
        vpmullw	%ymm8, %ymm10, %ymm10
        vmovdqa	0x20(%rcx), %ymm8
        vpmulhw	%ymm8, %ymm4, %ymm4
        vpmullw	%ymm8, %ymm12, %ymm12
        vmovdqa	(%r8), %ymm8
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
        vpaddw	%ymm5, %ymm9, %ymm9
        vpaddw	%ymm7, %ymm11, %ymm11
        vpsubw	%ymm13, %ymm10, %ymm13
        vpsubw	%ymm12, %ymm6, %ymm6
        vmovdqa	%ymm13, (%rdi)
        vmovdqa	%ymm9, 0x20(%rdi)
        vmovdqa	%ymm6, 0x40(%rdi)
        vmovdqa	%ymm11, 0x60(%rdi)
        vmovdqa	0x20(%r8), %ymm0
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
        vmovdqa	0xc0(%rdx), %ymm7
        vmovdqa	0xe0(%rdx), %ymm8
        vpmulhw	%ymm7, %ymm3, %ymm15
        vpmulhw	%ymm8, %ymm3, %ymm3
        vpmulhw	%ymm7, %ymm4, %ymm0
        vmovdqa	%ymm13, (%rsp)
        vpmullw	%ymm5, %ymm9, %ymm13
        vpmullw	%ymm6, %ymm9, %ymm9
        vpmullw	%ymm5, %ymm10, %ymm5
        vpmullw	%ymm7, %ymm11, %ymm6
        vpmullw	%ymm8, %ymm11, %ymm11
        vpmullw	%ymm7, %ymm12, %ymm7
        vmovdqa	0x40(%rcx), %ymm8
        vpmulhw	%ymm8, %ymm2, %ymm2
        vpmullw	%ymm8, %ymm10, %ymm10
        vmovdqa	0x60(%rcx), %ymm8
        vpmulhw	%ymm8, %ymm4, %ymm4
        vpmullw	%ymm8, %ymm12, %ymm12
        vmovdqa	(%r8), %ymm8
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
        vpaddw	%ymm5, %ymm9, %ymm9
        vpaddw	%ymm7, %ymm11, %ymm11
        vpsubw	%ymm13, %ymm10, %ymm13
        vpsubw	%ymm12, %ymm6, %ymm6
        vmovdqa	%ymm13, 0x80(%rdi)
        vmovdqa	%ymm9, 0xa0(%rdi)
        vmovdqa	%ymm6, 0xc0(%rdi)
        vmovdqa	%ymm11, 0xe0(%rdi)
        vmovdqa	0x20(%r8), %ymm0
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
        vmovdqa	0x140(%rdx), %ymm7
        vmovdqa	0x160(%rdx), %ymm8
        vpmulhw	%ymm7, %ymm3, %ymm15
        vpmulhw	%ymm8, %ymm3, %ymm3
        vpmulhw	%ymm7, %ymm4, %ymm0
        vmovdqa	%ymm13, (%rsp)
        vpmullw	%ymm5, %ymm9, %ymm13
        vpmullw	%ymm6, %ymm9, %ymm9
        vpmullw	%ymm5, %ymm10, %ymm5
        vpmullw	%ymm7, %ymm11, %ymm6
        vpmullw	%ymm8, %ymm11, %ymm11
        vpmullw	%ymm7, %ymm12, %ymm7
        vmovdqa	0x80(%rcx), %ymm8
        vpmulhw	%ymm8, %ymm2, %ymm2
        vpmullw	%ymm8, %ymm10, %ymm10
        vmovdqa	0xa0(%rcx), %ymm8
        vpmulhw	%ymm8, %ymm4, %ymm4
        vpmullw	%ymm8, %ymm12, %ymm12
        vmovdqa	(%r8), %ymm8
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
        vpaddw	%ymm5, %ymm9, %ymm9
        vpaddw	%ymm7, %ymm11, %ymm11
        vpsubw	%ymm13, %ymm10, %ymm13
        vpsubw	%ymm12, %ymm6, %ymm6
        vmovdqa	%ymm13, 0x100(%rdi)
        vmovdqa	%ymm9, 0x120(%rdi)
        vmovdqa	%ymm6, 0x140(%rdi)
        vmovdqa	%ymm11, 0x160(%rdi)
        vmovdqa	0x20(%r8), %ymm0
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
        vmovdqa	0x1c0(%rdx), %ymm7
        vmovdqa	0x1e0(%rdx), %ymm8
        vpmulhw	%ymm7, %ymm3, %ymm15
        vpmulhw	%ymm8, %ymm3, %ymm3
        vpmulhw	%ymm7, %ymm4, %ymm0
        vmovdqa	%ymm13, (%rsp)
        vpmullw	%ymm5, %ymm9, %ymm13
        vpmullw	%ymm6, %ymm9, %ymm9
        vpmullw	%ymm5, %ymm10, %ymm5
        vpmullw	%ymm7, %ymm11, %ymm6
        vpmullw	%ymm8, %ymm11, %ymm11
        vpmullw	%ymm7, %ymm12, %ymm7
        vmovdqa	0xc0(%rcx), %ymm8
        vpmulhw	%ymm8, %ymm2, %ymm2
        vpmullw	%ymm8, %ymm10, %ymm10
        vmovdqa	0xe0(%rcx), %ymm8
        vpmulhw	%ymm8, %ymm4, %ymm4
        vpmullw	%ymm8, %ymm12, %ymm12
        vmovdqa	(%r8), %ymm8
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
        vpaddw	%ymm5, %ymm9, %ymm9
        vpaddw	%ymm7, %ymm11, %ymm11
        vpsubw	%ymm13, %ymm10, %ymm13
        vpsubw	%ymm12, %ymm6, %ymm6
        vmovdqa	%ymm13, 0x180(%rdi)
        vmovdqa	%ymm9, 0x1a0(%rdi)
        vmovdqa	%ymm6, 0x1c0(%rdi)
        vmovdqa	%ymm11, 0x1e0(%rdi)
        movq	%r11, %rsp
        retq

#endif /* MLK_ARITH_BACKEND_X86_64_DEFAULT && !MLK_CONFIG_MULTILEVEL_NO_SHARED \
        */
