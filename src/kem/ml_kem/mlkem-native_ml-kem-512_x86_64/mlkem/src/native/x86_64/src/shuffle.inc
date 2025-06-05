/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/*
 * This file is derived from the public domain
 * AVX2 Kyber implementation @[REF_AVX2].
 */

.macro shuffle8 r0,r1,r2,r3
vperm2i128	$0x20,%ymm\r1,%ymm\r0,%ymm\r2
vperm2i128	$0x31,%ymm\r1,%ymm\r0,%ymm\r3
.endm

.macro shuffle4 r0,r1,r2,r3
vpunpcklqdq	%ymm\r1,%ymm\r0,%ymm\r2
vpunpckhqdq	%ymm\r1,%ymm\r0,%ymm\r3
.endm

/* Shuffle r0=(a0,b0,c0,d0,...), r1=(a1,b1,c1,d1,...) into */
/* r2 = (a0,b0,a1,b1,e0,f0,e1,f1,...) */
/* r3 = (c0,d0,c1,d1,g0,h0,g1,h1,...) */
.macro shuffle2 r0,r1,r2,r3
/* r2=(a1,b1,a1,b1,e1,f1,e1,f1,...) */
vmovsldup	%ymm\r1,%ymm\r2
/* Conditional move */
/* 0xAA = 0b10101010 */
/* r2=(a0,b0,a1,b1,e0,f0,e1,f1,...) */
vpblendd	$0xAA,%ymm\r2,%ymm\r0,%ymm\r2
/* r0=(c0,d0,0,0,g0,h0,0,0,...) */
vpsrlq		$32,%ymm\r0,%ymm\r0
/* r3=(c0,d0,c1,d1,g0,h0,g1,h1,...) */
vpblendd	$0xAA,%ymm\r1,%ymm\r0,%ymm\r3
.endm

.macro shuffle1 r0,r1,r2,r3
vpslld		$16,%ymm\r1,%ymm\r2
vpblendw	$0xAA,%ymm\r2,%ymm\r0,%ymm\r2
vpsrld		$16,%ymm\r0,%ymm\r0
vpblendw	$0xAA,%ymm\r1,%ymm\r0,%ymm\r3
.endm
