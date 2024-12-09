/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

// Implementation from Kyber reference repository
// https://github.com/pq-crystals/kyber/blob/main/avx2

#include "../../../common.h"
#if defined(MLKEM_NATIVE_ARITH_BACKEND_X86_64_DEFAULT)

#include "consts.h"
#include "shuffle.inc"

/* Compute steps 1,2 / 3 of Montgomery multiplication */
.macro mul rh0,rh1,rh2,rh3,zl0=15,zl1=15,zh0=2,zh1=2
vpmullw		%ymm\zl0,%ymm\rh0,%ymm12
vpmullw		%ymm\zl0,%ymm\rh1,%ymm13

vpmullw		%ymm\zl1,%ymm\rh2,%ymm14
vpmullw		%ymm\zl1,%ymm\rh3,%ymm15

vpmulhw		%ymm\zh0,%ymm\rh0,%ymm\rh0
vpmulhw		%ymm\zh0,%ymm\rh1,%ymm\rh1

vpmulhw		%ymm\zh1,%ymm\rh2,%ymm\rh2
vpmulhw		%ymm\zh1,%ymm\rh3,%ymm\rh3
.endm

/* Compute step 3 / 3 of Montgomery multiplication */
/* Multiply-high is signed; outputs are bound by 2^15 * q in abs value */
.macro reduce
vpmulhw		%ymm0,%ymm12,%ymm12
vpmulhw		%ymm0,%ymm13,%ymm13

vpmulhw		%ymm0,%ymm14,%ymm14
vpmulhw		%ymm0,%ymm15,%ymm15
.endm

/* Finish Montgomery multiplication and compute add/sub steps in NTT butterfly
 *
 * At this point, the two high-products of 4 ongoing Montgomery multiplications
 * are in %ymm{12,13,14,15} and %ymm{rh{0,1,2,3}}, respectively.
 * The NTT coefficients that the results of the Montgomery multiplications should
 * be add/sub-ed with, are in %ymm{rl{0,1,2,3}}.
 *
 * What's interesting, here, is that rather than completing the Montgomery
 * multiplications by computing `%ymm{12+i} + %ymm{rh{i}}`, and then add/sub'ing
 * the result into %ymm{rl{0,1,2,3}}, we add/sub both `%ymm{12+i}` and
 * %ymm{rh{i}} to %ymm{rl{0,1,2,3}}, and then add the results.
 *
 * Functionally, though, this is still a signed Montgomery multiplication
 * followed by an add/sub.
 *
 * Since the result of the Montgomery multiplication is bounded
 * by q in absolute value, the coefficients overall grow by not
 * more than q in absolute value per layer. */
.macro update rln,rl0,rl1,rl2,rl3,rh0,rh1,rh2,rh3
vpaddw		%ymm\rh0,%ymm\rl0,%ymm\rln /* rln = rl0 + rh0 */
vpsubw		%ymm\rh0,%ymm\rl0,%ymm\rh0 /* rh0 = rl0 - rh0 */
vpaddw		%ymm\rh1,%ymm\rl1,%ymm\rl0 /* rl0 = rl1 + rh1 */
vpsubw		%ymm\rh1,%ymm\rl1,%ymm\rh1 /* rh1 = rl1 - rh1 */
vpaddw		%ymm\rh2,%ymm\rl2,%ymm\rl1 /* rl1 = rl2 + rh2 */
vpsubw		%ymm\rh2,%ymm\rl2,%ymm\rh2 /* rh2 = rl2 - rh2 */
vpaddw		%ymm\rh3,%ymm\rl3,%ymm\rl2 /* rl2 = rl3 + rh3 */
vpsubw		%ymm\rh3,%ymm\rl3,%ymm\rh3 /* rh3 = rl3 - rh3 */

vpsubw		%ymm12,%ymm\rln,%ymm\rln /* rln = rh0 + rl0 - ymm12 = rl0 + (rh0 - ymm12) */
vpaddw		%ymm12,%ymm\rh0,%ymm\rh0 /* rh0 = rl0 - rh0 + ymm12 = rl0 - (rh0 - ymm12) */
vpsubw		%ymm13,%ymm\rl0,%ymm\rl0 /* rl0 = rl1 + rh1 - ymm13 = rl1 + (rh1 - ymm13) */
vpaddw		%ymm13,%ymm\rh1,%ymm\rh1 /* rh1 = rl1 - rh1 + ymm13 = rl1 - (rh1 - ymm13) */
vpsubw		%ymm14,%ymm\rl1,%ymm\rl1 /* rl1 = rh2 + rl2 - ymm14 = rl2 + (rh2 - ymm14) */
vpaddw		%ymm14,%ymm\rh2,%ymm\rh2 /* rh2 = rl2 - rh2 + ymm14 = rl2 - (rh2 - ymm14) */
vpsubw		%ymm15,%ymm\rl2,%ymm\rl2 /* rl2 = rh3 + rl3 - ymm15 = rl3 + (rh3 - ymm15) */
vpaddw		%ymm15,%ymm\rh3,%ymm\rh3 /* rh3 = rl3 - rh3 + ymm15 = rl3 - (rh3 - ymm15) */
.endm

.macro level0 off
vpbroadcastq	(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+0)*2(%rsi),%ymm15
vmovdqa		(64*\off+128)*2(%rdi),%ymm8
vmovdqa		(64*\off+144)*2(%rdi),%ymm9
vmovdqa		(64*\off+160)*2(%rdi),%ymm10
vmovdqa		(64*\off+176)*2(%rdi),%ymm11
vpbroadcastq	(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+4)*2(%rsi),%ymm2

mul		8,9,10,11

vmovdqa		(64*\off+  0)*2(%rdi),%ymm4
vmovdqa		(64*\off+ 16)*2(%rdi),%ymm5
vmovdqa		(64*\off+ 32)*2(%rdi),%ymm6
vmovdqa		(64*\off+ 48)*2(%rdi),%ymm7

reduce
update		3,4,5,6,7,8,9,10,11

vmovdqa		%ymm3,(64*\off+  0)*2(%rdi)
vmovdqa		%ymm4,(64*\off+ 16)*2(%rdi)
vmovdqa		%ymm5,(64*\off+ 32)*2(%rdi)
vmovdqa		%ymm6,(64*\off+ 48)*2(%rdi)
vmovdqa		%ymm8,(64*\off+128)*2(%rdi)
vmovdqa		%ymm9,(64*\off+144)*2(%rdi)
vmovdqa		%ymm10,(64*\off+160)*2(%rdi)
vmovdqa		%ymm11,(64*\off+176)*2(%rdi)
.endm

.macro levels1t6 off
/* level 1 */
vmovdqa		(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+224*\off+16)*2(%rsi),%ymm15
vmovdqa		(128*\off+ 64)*2(%rdi),%ymm8
vmovdqa		(128*\off+ 80)*2(%rdi),%ymm9
vmovdqa		(128*\off+ 96)*2(%rdi),%ymm10
vmovdqa		(128*\off+112)*2(%rdi),%ymm11
vmovdqa		(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+224*\off+32)*2(%rsi),%ymm2

mul		8,9,10,11

vmovdqa		(128*\off+  0)*2(%rdi),%ymm4
vmovdqa	 	(128*\off+ 16)*2(%rdi),%ymm5
vmovdqa		(128*\off+ 32)*2(%rdi),%ymm6
vmovdqa		(128*\off+ 48)*2(%rdi),%ymm7

reduce
update		3,4,5,6,7,8,9,10,11

/* level 2 */
shuffle8	5,10,7,10
shuffle8	6,11,5,11

vmovdqa		(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+224*\off+48)*2(%rsi),%ymm15
vmovdqa		(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+224*\off+64)*2(%rsi),%ymm2

mul		7,10,5,11

shuffle8	3,8,6,8
shuffle8	4,9,3,9

reduce
update		4,6,8,3,9,7,10,5,11

/* level 3 */
shuffle4	8,5,9,5
shuffle4	3,11,8,11

vmovdqa		(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+224*\off+80)*2(%rsi),%ymm15
vmovdqa		(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+224*\off+96)*2(%rsi),%ymm2

mul		9,5,8,11

shuffle4	4,7,3,7
shuffle4	6,10,4,10

reduce
update		6,3,7,4,10,9,5,8,11

/* level 4 */
shuffle2	7,8,10,8
shuffle2	4,11,7,11

vmovdqa		(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+224*\off+112)*2(%rsi),%ymm15
vmovdqa		(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+224*\off+128)*2(%rsi),%ymm2

mul		10,8,7,11

shuffle2	6,9,4,9
shuffle2	3,5,6,5

reduce
update		3,4,9,6,5,10,8,7,11

/* level 5 */
shuffle1	9,7,5,7
shuffle1	6,11,9,11

vmovdqa		(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+224*\off+144)*2(%rsi),%ymm15
vmovdqa		(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+224*\off+160)*2(%rsi),%ymm2

mul		5,7,9,11

shuffle1	3,10,6,10
shuffle1	4,8,3,8

reduce
update		4,6,10,3,8,5,7,9,11

/* level 6 */
vmovdqa		(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+224*\off+176)*2(%rsi),%ymm14
vmovdqa		(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+224*\off+208)*2(%rsi),%ymm15
vmovdqa		(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+224*\off+192)*2(%rsi),%ymm8
vmovdqa		(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+224*\off+224)*2(%rsi),%ymm2

mul		10,3,9,11,14,15,8,2

reduce
update		8,4,6,5,7,10,3,9,11

vmovdqa		%ymm8,(128*\off+  0)*2(%rdi)
vmovdqa		%ymm4,(128*\off+ 16)*2(%rdi)
vmovdqa		%ymm10,(128*\off+ 32)*2(%rdi)
vmovdqa		%ymm3,(128*\off+ 48)*2(%rdi)
vmovdqa		%ymm6,(128*\off+ 64)*2(%rdi)
vmovdqa		%ymm5,(128*\off+ 80)*2(%rdi)
vmovdqa		%ymm9,(128*\off+ 96)*2(%rdi)
vmovdqa		%ymm11,(128*\off+112)*2(%rdi)
.endm

.text
.global MLKEM_ASM_NAMESPACE(ntt_avx2)
MLKEM_ASM_NAMESPACE(ntt_avx2):
vmovdqa		AVX2_BACKEND_DATA_OFFSET_16XQ*2(%rsi),%ymm0

level0		0
level0		1

levels1t6	0
levels1t6	1

ret

#endif /* MLKEM_NATIVE_ARITH_BACKEND_X86_64_DEFAULT */
