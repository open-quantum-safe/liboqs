/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

/* Implementation based on Kyber repository
 * https://github.com/pq-crystals/kyber/blob/main/avx2
 *
 * Changes to placement of modular reductions have
 * been made to simplify reasoning of non-overflow */

#include "../../../common.h"

#if defined(MLKEM_NATIVE_ARITH_BACKEND_X86_64_DEFAULT)

#include "consts.h"
#include "shuffle.inc"
#include "fq.inc"

/* Compute four GS butterflies between rh{0,1,2,3} and rl{0,1,2,3}.
 * Butterflies 0,1 use root zh0 and twisted root zl0, and butterflies
 * 2,3 use root zh1 and twisted root zl1
 * Results are again in rl{0-3} and rh{0-3} */
.macro butterfly rl0,rl1,rl2,rl3,rh0,rh1,rh2,rh3,zl0=2,zl1=2,zh0=3,zh1=3
vpsubw		%ymm\rl0,%ymm\rh0,%ymm12    /* ymm12 = rh0 - rl0 */
vpaddw		%ymm\rh0,%ymm\rl0,%ymm\rl0  /* rl0   = rh0 + rl0 */
vpsubw		%ymm\rl1,%ymm\rh1,%ymm13    /* ymm13 = rh1 - rl1 */

vpmullw		%ymm\zl0,%ymm12,%ymm\rh0    /* rh0   = (rh0 - rl0) * root0_twisted */
vpaddw		%ymm\rh1,%ymm\rl1,%ymm\rl1  /* rl1   = rh1 + rh1 */
vpsubw		%ymm\rl2,%ymm\rh2,%ymm14    /* ymm14 = rh2 - rl2 */

vpmullw		%ymm\zl0,%ymm13,%ymm\rh1    /* rh1   = (rh1 - rl1) * root0_twisted */
vpaddw		%ymm\rh2,%ymm\rl2,%ymm\rl2  /* rl2   = rh2 + rl2 */
vpsubw		%ymm\rl3,%ymm\rh3,%ymm15    /* ymm15 = rh3 - rl3 */

vpmullw		%ymm\zl1,%ymm14,%ymm\rh2    /* rh2   = (rh2 - rl2) * root1_twisted */
vpaddw		%ymm\rh3,%ymm\rl3,%ymm\rl3  /* rl3   = rh3 + rl3 */
vpmullw		%ymm\zl1,%ymm15,%ymm\rh3    /* rh3   = (rh3 - rl3) * root1_twisted */

vpmulhw		%ymm\zh0,%ymm12,%ymm12      /* ymm12 = (rh0 - rl0) * root0 */
vpmulhw		%ymm\zh0,%ymm13,%ymm13      /* ymm13 = (rh1 - rl1) * root0 */

vpmulhw		%ymm\zh1,%ymm14,%ymm14      /* ymm14 = (rh2 - rl2) * root1 */
vpmulhw		%ymm\zh1,%ymm15,%ymm15      /* ymm15 = (rh3 - rl3) * root1 */

vpmulhw		%ymm0,%ymm\rh0,%ymm\rh0     /* rh0 = Q * [(rh0 - rl0) * root0_twisted] */
vpmulhw		%ymm0,%ymm\rh1,%ymm\rh1     /* rh1 = Q * [(rh1 - rl1) * root0_twisted] */
vpmulhw		%ymm0,%ymm\rh2,%ymm\rh2     /* rh2 = Q * [(rh2 - rl2) * root0_twisted] */
vpmulhw		%ymm0,%ymm\rh3,%ymm\rh3     /* rh3 = Q * [(rh3 - rl3) * root0_twisted] */

vpsubw		%ymm\rh0,%ymm12,%ymm\rh0    /* rh0 = montmul(rh0-rl0, root0) */
vpsubw		%ymm\rh1,%ymm13,%ymm\rh1    /* rh1 = montmul(rh1-rl1, root0) */
vpsubw		%ymm\rh2,%ymm14,%ymm\rh2    /* rh2 = montmul(rh2-rl2, root0) */
vpsubw		%ymm\rh3,%ymm15,%ymm\rh3    /* rh3 = montmul(rh3-rl3, root0) */
.endm

.macro intt_levels0t5 off
/* level 0 */
/* no bounds assumptions */
vmovdqa		AVX2_BACKEND_DATA_OFFSET_16XFLO*2(%rsi),%ymm2
vmovdqa		AVX2_BACKEND_DATA_OFFSET_16XFHI*2(%rsi),%ymm3

vmovdqa         (128*\off+  0)*2(%rdi),%ymm4
vmovdqa         (128*\off+ 32)*2(%rdi),%ymm6
vmovdqa         (128*\off+ 16)*2(%rdi),%ymm5
vmovdqa         (128*\off+ 48)*2(%rdi),%ymm7

fqmulprecomp	2,3,4
fqmulprecomp	2,3,6
fqmulprecomp	2,3,5
fqmulprecomp	2,3,7

vmovdqa         (128*\off+ 64)*2(%rdi),%ymm8
vmovdqa         (128*\off+ 96)*2(%rdi),%ymm10
vmovdqa         (128*\off+ 80)*2(%rdi),%ymm9
vmovdqa         (128*\off+112)*2(%rdi),%ymm11

fqmulprecomp	2,3,8
fqmulprecomp	2,3,10
fqmulprecomp	2,3,9
fqmulprecomp	2,3,11

/* bounds: coefficients < q */

vpermq		$0x4E,(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+(1-\off)*224+208)*2(%rsi),%ymm15
vpermq		$0x4E,(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+(1-\off)*224+176)*2(%rsi),%ymm1
vpermq		$0x4E,(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+(1-\off)*224+224)*2(%rsi),%ymm2
vpermq		$0x4E,(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+(1-\off)*224+192)*2(%rsi),%ymm3
vmovdqa		AVX2_BACKEND_DATA_OFFSET_REVIDXB*2(%rsi),%ymm12
vpshufb		%ymm12,%ymm15,%ymm15
vpshufb		%ymm12,%ymm1,%ymm1
vpshufb		%ymm12,%ymm2,%ymm2
vpshufb		%ymm12,%ymm3,%ymm3

butterfly	4,5,8,9,6,7,10,11,15,1,2,3

/* Montgmoery multiplication with a signed canonical twiddle
 * always has absolute value < q. This is used henceforth to
 * normalize the absolute bounds on the second half inputs
 * to the current butterfly
 *
 * 4,5,8,9 abs bound < 2q; 6,7,10,11 abs bound < q */

/* level 1 */
vpermq		$0x4E,(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+(1-\off)*224+144)*2(%rsi),%ymm2
vpermq		$0x4E,(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+(1-\off)*224+160)*2(%rsi),%ymm3
vmovdqa		AVX2_BACKEND_DATA_OFFSET_REVIDXB*2(%rsi),%ymm1
vpshufb		%ymm1,%ymm2,%ymm2
vpshufb		%ymm1,%ymm3,%ymm3

butterfly	4,5,6,7,8,9,10,11,2,2,3,3

/* For 8,9,10,11, it is sufficient to use the bound <q (much weaker
 * than what we used above) for the absolute value of the Montgomery
 * multiplication with a twiddle.
 * 4,5 abs bound < 4q; 6,7 abs bound < 2q; 8,9,10,11 abs bound <q. */

shuffle1	4,5,3,5      // 3,5  abs bound < 4q
shuffle1	6,7,4,7      // 4,7  abs bound < INT16_MAX/8
shuffle1	8,9,6,9      // 6,9  abs bound < q
shuffle1	10,11,8,11   // 8,11 abs bound < q

/* level 2 */
vmovdqa		AVX2_BACKEND_DATA_OFFSET_REVIDXD*2(%rsi),%ymm12
vpermd		(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+(1-\off)*224+112)*2(%rsi),%ymm12,%ymm2
vpermd		(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+(1-\off)*224+128)*2(%rsi),%ymm12,%ymm10

butterfly	3,4,6,8,5,7,9,11,2,2,10,10
/* 3 abs bound < 8q, 4 abs bound < 4q, 6,8 abs bound < 2q, 5,7,9,11 abs bound < q */

vmovdqa		AVX2_BACKEND_DATA_OFFSET_16XV*2(%rsi),%ymm1
red16		3
/*  4 abs bound < 4q, 6,8 abs bound < 2q, 3,5,7,9,11 abs bound < q */

shuffle2	3,4,10,4   // 4,10 abs bound < 4q
shuffle2	6,8,3,8    // 3,8 abs bound < 2q
shuffle2	5,7,6,7    // 6,7 abs bound < q
shuffle2	9,11,5,11  // 5,11 abs bound < q

/* level 3 */
vpermq		$0x1B,(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+(1-\off)*224+80)*2(%rsi),%ymm2
vpermq		$0x1B,(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+(1-\off)*224+96)*2(%rsi),%ymm9

butterfly	10,3,6,5,4,8,7,11,2,2,9,9
/* 10 abs bound < 8q
 * 3 abs bound < 4q
 * 5,6 abs bound < 2q
 * 4,8,7,11 abs bound < q */

/* REF-CHANGE: The official AVX2 implementation from
 * https://github.com/pq-crystals/kyber/blob/main/avx2
 * does not have this reduction. We add it here to simplify reasoning of
 * non-overflow. Without it, one has to work with more precise bounds for
 * the output of a Montgomery multiplication; with this reduction,
 * in turn, the generic bound by q is sufficient. */
red16 10
/* 3 abs bound < 4q
 * 5,6 abs bound < 2q
 * 4,8,7,10,11 abs bound < q */

shuffle4	10,3,9,3   /* 3,9  abs bound < 4q */
shuffle4	6,5,10,5   /* 5,10 abs bound < 2q */
shuffle4	4,8,6,8    /* 6,8  abs bound < q */
shuffle4	7,11,4,11  /* 4,11 abs bound < q */

/* level 4 */
vpermq		$0x4E,(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+(1-\off)*224+48)*2(%rsi),%ymm2
vpermq		$0x4E,(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+(1-\off)*224+64)*2(%rsi),%ymm7

butterfly	9,10,6,4,3,5,8,11,2,2,7,7
/* 9 abs bound < 8q
 * 10 abs bound < 4q
 * 4,6 abs bound <2q
 * 3,5,8,11 abs bound < q */
red16 9
/* 10 abs bound < 4q
 * 4,6 abs bound <2q
 * 3,5,8,9,11 abs bound < q */

shuffle8	9,10,7,10  /* 7,10 abs bound < 4q */
shuffle8	6,4,9,4    /* 4,9  abs bound < 2q */
shuffle8	3,5,6,5    /* 5,6  abs bound < q */
shuffle8	8,11,3,11  /* 3,11 abs bound < q */

/* level 5 */
vmovdqa		(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+(1-\off)*224+16)*2(%rsi),%ymm2
vmovdqa		(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+(1-\off)*224+32)*2(%rsi),%ymm8

butterfly	7,9,6,3,10,4,5,11,2,2,8,8
/* 7         abs bound <8q
 * 9         abs bound <4q
 * 6,3       abs bound < 2q
 * 4,5,10,11 abs bound < q */

/* REF-CHANGE: The official AVX2 implementation does not
 * have this reduction, but it is not readily clear how
 * to improve the 8q bound on ymm7 to guarantee that
 * layer 6 won't overflow (16q > INT16_MAX). */
red16 7
/* global abs bound < 4q */

vmovdqa         %ymm7,(128*\off+  0)*2(%rdi)
vmovdqa         %ymm9,(128*\off+ 16)*2(%rdi)
vmovdqa         %ymm6,(128*\off+ 32)*2(%rdi)
vmovdqa         %ymm3,(128*\off+ 48)*2(%rdi)
vmovdqa         %ymm10,(128*\off+ 64)*2(%rdi)
vmovdqa         %ymm4,(128*\off+ 80)*2(%rdi)
vmovdqa         %ymm5,(128*\off+ 96)*2(%rdi)
vmovdqa         %ymm11,(128*\off+112)*2(%rdi)
.endm

.macro intt_level6 off
/* level 6 */
vmovdqa         (64*\off+  0)*2(%rdi),%ymm4
vmovdqa         (64*\off+128)*2(%rdi),%ymm8
vmovdqa         (64*\off+ 16)*2(%rdi),%ymm5
vmovdqa         (64*\off+144)*2(%rdi),%ymm9
vpbroadcastq	(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+0)*2(%rsi),%ymm2

vmovdqa         (64*\off+ 32)*2(%rdi),%ymm6
vmovdqa         (64*\off+160)*2(%rdi),%ymm10
vmovdqa         (64*\off+ 48)*2(%rdi),%ymm7
vmovdqa         (64*\off+176)*2(%rdi),%ymm11
vpbroadcastq	(AVX2_BACKEND_DATA_OFFSET_ZETAS_EXP+4)*2(%rsi),%ymm3

butterfly	4,5,6,7,8,9,10,11
/* global abs bound < 8q */

/* REF-CHANGE: The official AVX2 implementation has a `red16 4` for `off=0`.
 * We don't need this because of the earlier red16 which ensures an 8q bound */

vmovdqa		%ymm4,(64*\off+  0)*2(%rdi)
vmovdqa		%ymm5,(64*\off+ 16)*2(%rdi)
vmovdqa		%ymm6,(64*\off+ 32)*2(%rdi)
vmovdqa		%ymm7,(64*\off+ 48)*2(%rdi)
vmovdqa		%ymm8,(64*\off+128)*2(%rdi)
vmovdqa		%ymm9,(64*\off+144)*2(%rdi)
vmovdqa		%ymm10,(64*\off+160)*2(%rdi)
vmovdqa		%ymm11,(64*\off+176)*2(%rdi)
.endm

.text
.global MLKEM_ASM_NAMESPACE(invntt_avx2)
MLKEM_ASM_NAMESPACE(invntt_avx2):
vmovdqa         AVX2_BACKEND_DATA_OFFSET_16XQ*2(%rsi),%ymm0

intt_levels0t5	0
intt_levels0t5	1

intt_level6	0
intt_level6	1
ret

#endif /* MLKEM_NATIVE_ARITH_BACKEND_X86_64_DEFAULT */
