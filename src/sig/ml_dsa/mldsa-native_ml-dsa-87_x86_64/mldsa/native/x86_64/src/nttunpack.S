/*
 * Copyright (c) The mlkem-native project authors
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

 /*
 * This file is derived from the public domain
 * AVX2 Dilithium implementation @[REF_AVX2].
 */

 #include "../../../common.h"

#if defined(MLD_ARITH_BACKEND_X86_64_DEFAULT) && \
    !defined(MLD_CONFIG_MULTILEVEL_NO_SHARED)

.macro shuffle8 r0,r1,r2,r3
vperm2i128	$0x20,%ymm\r1,%ymm\r0,%ymm\r2
vperm2i128	$0x31,%ymm\r1,%ymm\r0,%ymm\r3
.endm

.macro shuffle4 r0,r1,r2,r3
vpunpcklqdq	%ymm\r1,%ymm\r0,%ymm\r2
vpunpckhqdq	%ymm\r1,%ymm\r0,%ymm\r3
.endm

.macro shuffle2 r0,r1,r2,r3
#vpsllq		$32,%ymm\r1,%ymm\r2
vmovsldup	%ymm\r1,%ymm\r2
vpblendd	$0xAA,%ymm\r2,%ymm\r0,%ymm\r2
vpsrlq		$32,%ymm\r0,%ymm\r0
#vmovshdup	%ymm\r0,%ymm\r0
vpblendd	$0xAA,%ymm\r1,%ymm\r0,%ymm\r3
.endm


.text
.balign 4
.global MLD_ASM_NAMESPACE(nttunpack_avx2)
MLD_ASM_FN_SYMBOL(nttunpack_avx2)

call		nttunpack128_avx
add		$256,%rdi
call		nttunpack128_avx
add		$256,%rdi
call		nttunpack128_avx
add		$256,%rdi
call		nttunpack128_avx
ret

nttunpack128_avx:
#load
vmovdqa		(%rdi),%ymm4
vmovdqa		32(%rdi),%ymm5
vmovdqa		64(%rdi),%ymm6
vmovdqa		96(%rdi),%ymm7
vmovdqa		128(%rdi),%ymm8
vmovdqa		160(%rdi),%ymm9
vmovdqa		192(%rdi),%ymm10
vmovdqa		224(%rdi),%ymm11

shuffle8	4,8,3,8
shuffle8	5,9,4,9
shuffle8	6,10,5,10
shuffle8	7,11,6,11

shuffle4	3,5,7,5
shuffle4	8,10,3,10
shuffle4	4,6,8,6
shuffle4	9,11,4,11

shuffle2	7,8,9,8
shuffle2	5,6,7,6
shuffle2	3,4,5,4
shuffle2	10,11,3,11

#store
vmovdqa		%ymm9,(%rdi)
vmovdqa		%ymm8,32(%rdi)
vmovdqa		%ymm7,64(%rdi)
vmovdqa		%ymm6,96(%rdi)
vmovdqa		%ymm5,128(%rdi)
vmovdqa		%ymm4,160(%rdi)
vmovdqa		%ymm3,192(%rdi)
vmovdqa		%ymm11,224(%rdi)

ret


#endif /* MLD_ARITH_BACKEND_X86_64_DEFAULT && !MLD_CONFIG_MULTILEVEL_NO_SHARED \
        */
