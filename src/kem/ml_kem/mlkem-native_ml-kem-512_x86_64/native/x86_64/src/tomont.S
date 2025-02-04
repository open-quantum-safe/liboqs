/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

// Implementation based on Kyber reference repository
// https://github.com/pq-crystals/kyber/blob/main/avx2

// Changes:
// - Add call to csub in reduce128_avx to produce outputs
//   in [0,1,...,q-1] rather than [0,1,...,q], matching the
//   semantics of poly_reduce().

#include "../../../common.h"

#if defined(MLKEM_NATIVE_ARITH_BACKEND_X86_64_DEFAULT)
/* simpasm: header-end */

#include "consts.h"
#include "fq.inc"

.text
.global MLKEM_ASM_NAMESPACE(tomont_avx2)
.balign 4
MLKEM_ASM_NAMESPACE(tomont_avx2):
#consts
vmovdqa		AVX2_BACKEND_DATA_OFFSET_16XQ*2(%rsi),%ymm0
vmovdqa		AVX2_BACKEND_DATA_OFFSET_16XMONTSQLO*2(%rsi),%ymm1
vmovdqa		AVX2_BACKEND_DATA_OFFSET_16XMONTSQHI*2(%rsi),%ymm2
call		tomont128_avx2
add		$256,%rdi
call		tomont128_avx2
ret

tomont128_avx2:
#load
vmovdqa		(%rdi),%ymm3
vmovdqa		32(%rdi),%ymm4
vmovdqa		64(%rdi),%ymm5
vmovdqa		96(%rdi),%ymm6
vmovdqa		128(%rdi),%ymm7
vmovdqa		160(%rdi),%ymm8
vmovdqa		192(%rdi),%ymm9
vmovdqa		224(%rdi),%ymm10

fqmulprecomp	1,2,3,11
fqmulprecomp	1,2,4,12
fqmulprecomp	1,2,5,13
fqmulprecomp	1,2,6,14
fqmulprecomp	1,2,7,15
fqmulprecomp	1,2,8,11
fqmulprecomp	1,2,9,12
fqmulprecomp	1,2,10,13

#store
vmovdqa		%ymm3,(%rdi)
vmovdqa		%ymm4,32(%rdi)
vmovdqa		%ymm5,64(%rdi)
vmovdqa		%ymm6,96(%rdi)
vmovdqa		%ymm7,128(%rdi)
vmovdqa		%ymm8,160(%rdi)
vmovdqa		%ymm9,192(%rdi)
vmovdqa		%ymm10,224(%rdi)

ret

/* simpasm: footer-start */
#endif /* MLKEM_NATIVE_ARITH_BACKEND_X86_64_DEFAULT */
