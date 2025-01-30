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
.global MLKEM_ASM_NAMESPACE(reduce_avx2)
.balign 4
MLKEM_ASM_NAMESPACE(reduce_avx2):
#consts
vmovdqa		AVX2_BACKEND_DATA_OFFSET_16XQ*2(%rsi),%ymm0
vmovdqa		AVX2_BACKEND_DATA_OFFSET_16XV*2(%rsi),%ymm1
call		reduce128_avx2
add		$256,%rdi
call		reduce128_avx2
ret

reduce128_avx2:
#load
vmovdqa		(%rdi),%ymm2
vmovdqa		32(%rdi),%ymm3
vmovdqa		64(%rdi),%ymm4
vmovdqa		96(%rdi),%ymm5
vmovdqa		128(%rdi),%ymm6
vmovdqa		160(%rdi),%ymm7
vmovdqa		192(%rdi),%ymm8
vmovdqa		224(%rdi),%ymm9

red16		2
red16		3
red16		4
red16		5
red16		6
red16		7
red16		8
red16		9

csubq		2
csubq		3
csubq		4
csubq		5
csubq		6
csubq		7
csubq		8
csubq		9

#store
vmovdqa		%ymm2,(%rdi)
vmovdqa		%ymm3,32(%rdi)
vmovdqa		%ymm4,64(%rdi)
vmovdqa		%ymm5,96(%rdi)
vmovdqa		%ymm6,128(%rdi)
vmovdqa		%ymm7,160(%rdi)
vmovdqa		%ymm8,192(%rdi)
vmovdqa		%ymm9,224(%rdi)

ret

/* simpasm: footer-start */
#endif /* MLKEM_NATIVE_ARITH_BACKEND_X86_64_DEFAULT */
