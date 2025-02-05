/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

// Implementation from Kyber reference repository
// https://github.com/pq-crystals/kyber/blob/main/avx2

#include "../../../common.h"

#if defined(MLKEM_NATIVE_ARITH_BACKEND_X86_64_DEFAULT)
/* simpasm: header-end */

#include "consts.h"
#include "fq.inc"
#include "shuffle.inc"

.text
.global MLKEM_ASM_NAMESPACE(nttfrombytes_avx2)
.balign 4
MLKEM_ASM_NAMESPACE(nttfrombytes_avx2):
#consts
vmovdqa		AVX2_BACKEND_DATA_OFFSET_16XMASK*2(%rdx),%ymm0
call		nttfrombytes128_avx
add		$256,%rdi
add		$192,%rsi
call		nttfrombytes128_avx
ret

nttfrombytes128_avx:
#load
vmovdqu		(%rsi),%ymm4
vmovdqu		32(%rsi),%ymm5
vmovdqu		64(%rsi),%ymm6
vmovdqu		96(%rsi),%ymm7
vmovdqu		128(%rsi),%ymm8
vmovdqu		160(%rsi),%ymm9

shuffle8	4,7,3,7
shuffle8	5,8,4,8
shuffle8	6,9,5,9

shuffle4	3,8,6,8
shuffle4	7,5,3,5
shuffle4	4,9,7,9

shuffle2	6,5,4,5
shuffle2	8,7,6,7
shuffle2	3,9,8,9

shuffle1	4,7,10,7
shuffle1	5,8,4,8
shuffle1	6,9,5,9

#bitunpack
vpsrlw		$12,%ymm10,%ymm11
vpsllw		$4,%ymm7,%ymm12
vpor		%ymm11,%ymm12,%ymm11
vpand		%ymm0,%ymm10,%ymm10
vpand		%ymm0,%ymm11,%ymm11

vpsrlw		$8,%ymm7,%ymm12
vpsllw		$8,%ymm4,%ymm13
vpor		%ymm12,%ymm13,%ymm12
vpand		%ymm0,%ymm12,%ymm12

vpsrlw		$4,%ymm4,%ymm13
vpand		%ymm0,%ymm13,%ymm13

vpsrlw		$12,%ymm8,%ymm14
vpsllw		$4,%ymm5,%ymm15
vpor		%ymm14,%ymm15,%ymm14
vpand		%ymm0,%ymm8,%ymm8
vpand		%ymm0,%ymm14,%ymm14

vpsrlw		$8,%ymm5,%ymm15
vpsllw		$8,%ymm9,%ymm1
vpor		%ymm15,%ymm1,%ymm15
vpand		%ymm0,%ymm15,%ymm15

vpsrlw		$4,%ymm9,%ymm1
vpand		%ymm0,%ymm1,%ymm1

#store
vmovdqa		%ymm10,(%rdi)
vmovdqa		%ymm11,32(%rdi)
vmovdqa		%ymm12,64(%rdi)
vmovdqa		%ymm13,96(%rdi)
vmovdqa		%ymm8,128(%rdi)
vmovdqa		%ymm14,160(%rdi)
vmovdqa		%ymm15,192(%rdi)
vmovdqa		%ymm1,224(%rdi)

ret

/* simpasm: footer-start */
#endif /* MLKEM_NATIVE_ARITH_BACKEND_X86_64_DEFAULT */
