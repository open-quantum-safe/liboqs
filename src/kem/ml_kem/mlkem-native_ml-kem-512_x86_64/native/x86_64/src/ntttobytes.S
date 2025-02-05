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
.global MLKEM_ASM_NAMESPACE(ntttobytes_avx2)
.balign 4
MLKEM_ASM_NAMESPACE(ntttobytes_avx2):
#consts
vmovdqa		AVX2_BACKEND_DATA_OFFSET_16XQ*2(%rdx),%ymm0
call		ntttobytes128_avx
add		$256,%rsi
add		$192,%rdi
call		ntttobytes128_avx
ret

ntttobytes128_avx:
#load
vmovdqa		(%rsi),%ymm5
vmovdqa		32(%rsi),%ymm6
vmovdqa		64(%rsi),%ymm7
vmovdqa		96(%rsi),%ymm8
vmovdqa		128(%rsi),%ymm9
vmovdqa		160(%rsi),%ymm10
vmovdqa		192(%rsi),%ymm11
vmovdqa		224(%rsi),%ymm12

#bitpack
vpsllw		$12,%ymm6,%ymm4
vpor		%ymm4,%ymm5,%ymm4

vpsrlw		$4,%ymm6,%ymm5
vpsllw		$8,%ymm7,%ymm6
vpor		%ymm5,%ymm6,%ymm5

vpsrlw		$8,%ymm7,%ymm6
vpsllw		$4,%ymm8,%ymm7
vpor		%ymm6,%ymm7,%ymm6

vpsllw		$12,%ymm10,%ymm7
vpor		%ymm7,%ymm9,%ymm7

vpsrlw		$4,%ymm10,%ymm8
vpsllw		$8,%ymm11,%ymm9
vpor		%ymm8,%ymm9,%ymm8

vpsrlw		$8,%ymm11,%ymm9
vpsllw		$4,%ymm12,%ymm10
vpor		%ymm9,%ymm10,%ymm9

shuffle1	4,5,3,5
shuffle1	6,7,4,7
shuffle1	8,9,6,9

shuffle2	3,4,8,4
shuffle2	6,5,3,5
shuffle2	7,9,6,9

shuffle4	8,3,7,3
shuffle4	6,4,8,4
shuffle4	5,9,6,9

shuffle8	7,8,5,8
shuffle8	6,3,7,3
shuffle8	4,9,6,9

#store
vmovdqu		%ymm5,(%rdi)
vmovdqu		%ymm7,32(%rdi)
vmovdqu		%ymm6,64(%rdi)
vmovdqu		%ymm8,96(%rdi)
vmovdqu		%ymm3,128(%rdi)
vmovdqu		%ymm9,160(%rdi)

ret

/* simpasm: footer-start */
#endif /* MLKEM_NATIVE_ARITH_BACKEND_X86_64_DEFAULT */
