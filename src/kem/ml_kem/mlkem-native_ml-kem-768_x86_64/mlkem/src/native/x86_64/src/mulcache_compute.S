/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#include "../../../common.h"
#if defined(MLK_ARITH_BACKEND_X86_64_DEFAULT) && \
    !defined(MLK_CONFIG_MULTILEVEL_NO_SHARED)

/*
 * WARNING: This file is auto-derived from the mlkem-native source file
 *   dev/x86_64/src/mulcache_compute.S using scripts/simpasm. Do not modify it directly.
 */


.text
.balign 4
.global MLK_ASM_NAMESPACE(poly_mulcache_compute_avx2)
MLK_ASM_FN_SYMBOL(poly_mulcache_compute_avx2)

        vmovdqa	(%rdx), %ymm0
        vmovdqa	0x20(%rsi), %ymm2
        vmovdqa	0x60(%rsi), %ymm3
        vmovdqa	0x500(%rdx), %ymm4
        vmovdqa	0x580(%rdx), %ymm1
        vpmullw	%ymm2, %ymm1, %ymm5
        vpmullw	%ymm3, %ymm1, %ymm6
        vpmulhw	%ymm2, %ymm4, %ymm7
        vpmulhw	%ymm3, %ymm4, %ymm8
        vpmulhw	%ymm5, %ymm0, %ymm9
        vpmulhw	%ymm6, %ymm0, %ymm10
        vpsubw	%ymm9, %ymm7, %ymm7
        vpsubw	%ymm10, %ymm8, %ymm8
        vmovdqa	%ymm7, (%rdi)
        vmovdqa	%ymm8, 0x20(%rdi)
        vmovdqa	0xa0(%rsi), %ymm2
        vmovdqa	0xe0(%rsi), %ymm3
        vmovdqa	0x520(%rdx), %ymm4
        vmovdqa	0x5a0(%rdx), %ymm1
        vpmullw	%ymm2, %ymm1, %ymm5
        vpmullw	%ymm3, %ymm1, %ymm6
        vpmulhw	%ymm2, %ymm4, %ymm7
        vpmulhw	%ymm3, %ymm4, %ymm8
        vpmulhw	%ymm5, %ymm0, %ymm9
        vpmulhw	%ymm6, %ymm0, %ymm10
        vpsubw	%ymm9, %ymm7, %ymm7
        vpsubw	%ymm10, %ymm8, %ymm8
        vmovdqa	%ymm7, 0x40(%rdi)
        vmovdqa	%ymm8, 0x60(%rdi)
        vmovdqa	0x120(%rsi), %ymm2
        vmovdqa	0x160(%rsi), %ymm3
        vmovdqa	0x540(%rdx), %ymm4
        vmovdqa	0x5c0(%rdx), %ymm1
        vpmullw	%ymm2, %ymm1, %ymm5
        vpmullw	%ymm3, %ymm1, %ymm6
        vpmulhw	%ymm2, %ymm4, %ymm7
        vpmulhw	%ymm3, %ymm4, %ymm8
        vpmulhw	%ymm5, %ymm0, %ymm9
        vpmulhw	%ymm6, %ymm0, %ymm10
        vpsubw	%ymm9, %ymm7, %ymm7
        vpsubw	%ymm10, %ymm8, %ymm8
        vmovdqa	%ymm7, 0x80(%rdi)
        vmovdqa	%ymm8, 0xa0(%rdi)
        vmovdqa	0x1a0(%rsi), %ymm2
        vmovdqa	0x1e0(%rsi), %ymm3
        vmovdqa	0x560(%rdx), %ymm4
        vmovdqa	0x5e0(%rdx), %ymm1
        vpmullw	%ymm2, %ymm1, %ymm5
        vpmullw	%ymm3, %ymm1, %ymm6
        vpmulhw	%ymm2, %ymm4, %ymm7
        vpmulhw	%ymm3, %ymm4, %ymm8
        vpmulhw	%ymm5, %ymm0, %ymm9
        vpmulhw	%ymm6, %ymm0, %ymm10
        vpsubw	%ymm9, %ymm7, %ymm7
        vpsubw	%ymm10, %ymm8, %ymm8
        vmovdqa	%ymm7, 0xc0(%rdi)
        vmovdqa	%ymm8, 0xe0(%rdi)
        retq

#endif /* MLK_ARITH_BACKEND_X86_64_DEFAULT && !MLK_CONFIG_MULTILEVEL_NO_SHARED \
        */
