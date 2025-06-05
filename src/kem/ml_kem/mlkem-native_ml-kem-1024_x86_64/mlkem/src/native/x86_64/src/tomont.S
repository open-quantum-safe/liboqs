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
 * Implementation from Kyber reference repository @[REF_AVX2]
 *
 * Changes:
 * - Add call to csub in reduce128_avx to produce outputs
 *   in [0,1,...,q-1] rather than [0,1,...,q], matching the
 *   semantics of mlk_poly_reduce().
 */

#include "../../../common.h"
#if defined(MLK_ARITH_BACKEND_X86_64_DEFAULT) && \
    !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/x86_64/src/tomont.S using scripts/simpasm. Do not modify it directly.
 */


.text
.balign 4
.global MLK_ASM_NAMESPACE(tomont_avx2)
MLK_ASM_FN_SYMBOL(tomont_avx2)

        vmovdqa	(%rsi), %ymm0
        vmovdqa	0xa0(%rsi), %ymm1
        vmovdqa	0xc0(%rsi), %ymm2
        callq	tomont_avx2_core
        addq	$0x100, %rdi            # imm = 0x100
        callq	tomont_avx2_core
        retq

tomont_avx2_core:
        vmovdqa	(%rdi), %ymm3
        vmovdqa	0x20(%rdi), %ymm4
        vmovdqa	0x40(%rdi), %ymm5
        vmovdqa	0x60(%rdi), %ymm6
        vmovdqa	0x80(%rdi), %ymm7
        vmovdqa	0xa0(%rdi), %ymm8
        vmovdqa	0xc0(%rdi), %ymm9
        vmovdqa	0xe0(%rdi), %ymm10
        vpmullw	%ymm1, %ymm3, %ymm11
        vpmulhw	%ymm2, %ymm3, %ymm3
        vpmulhw	%ymm0, %ymm11, %ymm11
        vpsubw	%ymm11, %ymm3, %ymm3
        vpmullw	%ymm1, %ymm4, %ymm12
        vpmulhw	%ymm2, %ymm4, %ymm4
        vpmulhw	%ymm0, %ymm12, %ymm12
        vpsubw	%ymm12, %ymm4, %ymm4
        vpmullw	%ymm1, %ymm5, %ymm13
        vpmulhw	%ymm2, %ymm5, %ymm5
        vpmulhw	%ymm0, %ymm13, %ymm13
        vpsubw	%ymm13, %ymm5, %ymm5
        vpmullw	%ymm1, %ymm6, %ymm14
        vpmulhw	%ymm2, %ymm6, %ymm6
        vpmulhw	%ymm0, %ymm14, %ymm14
        vpsubw	%ymm14, %ymm6, %ymm6
        vpmullw	%ymm1, %ymm7, %ymm15
        vpmulhw	%ymm2, %ymm7, %ymm7
        vpmulhw	%ymm0, %ymm15, %ymm15
        vpsubw	%ymm15, %ymm7, %ymm7
        vpmullw	%ymm1, %ymm8, %ymm11
        vpmulhw	%ymm2, %ymm8, %ymm8
        vpmulhw	%ymm0, %ymm11, %ymm11
        vpsubw	%ymm11, %ymm8, %ymm8
        vpmullw	%ymm1, %ymm9, %ymm12
        vpmulhw	%ymm2, %ymm9, %ymm9
        vpmulhw	%ymm0, %ymm12, %ymm12
        vpsubw	%ymm12, %ymm9, %ymm9
        vpmullw	%ymm1, %ymm10, %ymm13
        vpmulhw	%ymm2, %ymm10, %ymm10
        vpmulhw	%ymm0, %ymm13, %ymm13
        vpsubw	%ymm13, %ymm10, %ymm10
        vmovdqa	%ymm3, (%rdi)
        vmovdqa	%ymm4, 0x20(%rdi)
        vmovdqa	%ymm5, 0x40(%rdi)
        vmovdqa	%ymm6, 0x60(%rdi)
        vmovdqa	%ymm7, 0x80(%rdi)
        vmovdqa	%ymm8, 0xa0(%rdi)
        vmovdqa	%ymm9, 0xc0(%rdi)
        vmovdqa	%ymm10, 0xe0(%rdi)
        retq

#endif /* MLK_ARITH_BACKEND_X86_64_DEFAULT && !MLK_CONFIG_MULTILEVEL_NO_SHARED \
        */
