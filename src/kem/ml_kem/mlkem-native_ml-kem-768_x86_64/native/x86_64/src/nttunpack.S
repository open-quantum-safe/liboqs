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
.global MLKEM_ASM_NAMESPACE(nttunpack_avx2)
.balign 4
MLKEM_ASM_NAMESPACE(nttunpack_avx2):
call		nttunpack128_avx2
add		$256,%rdi
call		nttunpack128_avx2
ret

nttunpack128_avx2:
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

shuffle1	9,5,10,5
shuffle1	8,4,9,4
shuffle1	7,3,8,3
shuffle1	6,11,7,11

#store
vmovdqa		%ymm10,(%rdi)
vmovdqa		%ymm5,32(%rdi)
vmovdqa		%ymm9,64(%rdi)
vmovdqa		%ymm4,96(%rdi)
vmovdqa		%ymm8,128(%rdi)
vmovdqa		%ymm3,160(%rdi)
vmovdqa		%ymm7,192(%rdi)
vmovdqa		%ymm11,224(%rdi)

ret

/* simpasm: footer-start */
#endif /* MLKEM_NATIVE_ARITH_BACKEND_X86_64_DEFAULT */
