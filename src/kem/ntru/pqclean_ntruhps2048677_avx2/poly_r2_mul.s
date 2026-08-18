.data
.p2align 5
mask1100:
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
mask0110:
.word 0
.word 0
.word 0
.word 0
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 0
.word 0
.word 0
.word 0
mask0011:
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
mask1000:
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 65535
.word 65535
.word 65535
.word 65535
mask0111:
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 0
.word 0
.word 0
.word 0
low165:
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 31
.word 0
.word 0
.word 0
.word 0
.word 0
.text
.global PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_mul
.global _PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_mul
PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_mul:
_PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_mul:
vmovdqa 0(%rsi), %ymm0
vmovdqa 32(%rsi), %ymm1
vmovdqa 0(%rdx), %ymm3
vmovdqa 32(%rdx), %ymm4
vpxor %ymm0, %ymm1, %ymm6
vpxor %ymm3, %ymm4, %ymm7
vextracti128 $1, %ymm0, %xmm11
vextracti128 $1, %ymm3, %xmm12
vpclmulqdq $1, %xmm11, %xmm12, %xmm5
vpclmulqdq $16, %xmm11, %xmm12, %xmm14
vpclmulqdq $17, %xmm11, %xmm12, %xmm15
vpxor %xmm5, %xmm14, %xmm14
vpclmulqdq $0, %xmm11, %xmm12, %xmm5
vpermq $16, %ymm14, %ymm14
vinserti128 $1, %xmm15, %ymm15, %ymm15
vpand mask0011(%rip), %ymm5, %ymm5
vpand mask0110(%rip), %ymm14, %ymm14
vpand mask1100(%rip), %ymm15, %ymm15
vpxor %ymm5, %ymm14, %ymm14
vpxor %ymm14, %ymm15, %ymm5
vpxor %xmm0, %xmm11, %xmm11
vpxor %xmm3, %xmm12, %xmm12
vpclmulqdq $1, %xmm11, %xmm12, %xmm13
vpclmulqdq $16, %xmm11, %xmm12, %xmm14
vpclmulqdq $17, %xmm11, %xmm12, %xmm15
vpxor %xmm13, %xmm14, %xmm14
vpclmulqdq $0, %xmm11, %xmm12, %xmm13
vpermq $16, %ymm14, %ymm14
vinserti128 $1, %xmm15, %ymm15, %ymm15
vpand mask0011(%rip), %ymm13, %ymm13
vpand mask0110(%rip), %ymm14, %ymm14
vpand mask1100(%rip), %ymm15, %ymm15
vpxor %ymm13, %ymm14, %ymm14
vpxor %ymm14, %ymm15, %ymm13
vpclmulqdq $1, %xmm0, %xmm3, %xmm2
vpclmulqdq $16, %xmm0, %xmm3, %xmm14
vpclmulqdq $17, %xmm0, %xmm3, %xmm15
vpxor %xmm2, %xmm14, %xmm14
vpclmulqdq $0, %xmm0, %xmm3, %xmm2
vpermq $16, %ymm14, %ymm14
vinserti128 $1, %xmm15, %ymm15, %ymm15
vpand mask0011(%rip), %ymm2, %ymm2
vpand mask0110(%rip), %ymm14, %ymm14
vpand mask1100(%rip), %ymm15, %ymm15
vpxor %ymm2, %ymm14, %ymm14
vpxor %ymm14, %ymm15, %ymm2
vpxor %ymm13, %ymm5, %ymm13
vpxor %ymm13, %ymm2, %ymm13
vpxor %ymm11, %ymm11, %ymm11
vextracti128 $1, %ymm13, %xmm11
vpxor %ymm5, %ymm11, %ymm5
vpxor %ymm11, %ymm11, %ymm11
vinserti128 $1, %xmm13, %ymm11, %ymm11
vpxor %ymm11, %ymm2, %ymm2
vextracti128 $1, %ymm6, %xmm11
vextracti128 $1, %ymm7, %xmm12
vpclmulqdq $1, %xmm11, %xmm12, %xmm9
vpclmulqdq $16, %xmm11, %xmm12, %xmm14
vpclmulqdq $17, %xmm11, %xmm12, %xmm15
vpxor %xmm9, %xmm14, %xmm14
vpclmulqdq $0, %xmm11, %xmm12, %xmm9
vpermq $16, %ymm14, %ymm14
vinserti128 $1, %xmm15, %ymm15, %ymm15
vpand mask0011(%rip), %ymm9, %ymm9
vpand mask0110(%rip), %ymm14, %ymm14
vpand mask1100(%rip), %ymm15, %ymm15
vpxor %ymm9, %ymm14, %ymm14
vpxor %ymm14, %ymm15, %ymm9
vpxor %xmm6, %xmm11, %xmm11
vpxor %xmm7, %xmm12, %xmm12
vpclmulqdq $1, %xmm11, %xmm12, %xmm13
vpclmulqdq $16, %xmm11, %xmm12, %xmm14
vpclmulqdq $17, %xmm11, %xmm12, %xmm15
vpxor %xmm13, %xmm14, %xmm14
vpclmulqdq $0, %xmm11, %xmm12, %xmm13
vpermq $16, %ymm14, %ymm14
vinserti128 $1, %xmm15, %ymm15, %ymm15
vpand mask0011(%rip), %ymm13, %ymm13
vpand mask0110(%rip), %ymm14, %ymm14
vpand mask1100(%rip), %ymm15, %ymm15
vpxor %ymm13, %ymm14, %ymm14
vpxor %ymm14, %ymm15, %ymm13
vpclmulqdq $1, %xmm6, %xmm7, %xmm8
vpclmulqdq $16, %xmm6, %xmm7, %xmm14
vpclmulqdq $17, %xmm6, %xmm7, %xmm15
vpxor %xmm8, %xmm14, %xmm14
vpclmulqdq $0, %xmm6, %xmm7, %xmm8
vpermq $16, %ymm14, %ymm14
vinserti128 $1, %xmm15, %ymm15, %ymm15
vpand mask0011(%rip), %ymm8, %ymm8
vpand mask0110(%rip), %ymm14, %ymm14
vpand mask1100(%rip), %ymm15, %ymm15
vpxor %ymm8, %ymm14, %ymm14
vpxor %ymm14, %ymm15, %ymm8
vpxor %ymm13, %ymm9, %ymm13
vpxor %ymm13, %ymm8, %ymm13
vpxor %ymm11, %ymm11, %ymm11
vextracti128 $1, %ymm13, %xmm11
vpxor %ymm9, %ymm11, %ymm9
vpxor %ymm11, %ymm11, %ymm11
vinserti128 $1, %xmm13, %ymm11, %ymm11
vpxor %ymm11, %ymm8, %ymm8
vpxor %ymm8, %ymm2, %ymm0
vpxor %ymm9, %ymm5, %ymm3
vpxor %ymm5, %ymm0, %ymm0
vpxor %ymm3, %ymm8, %ymm8
vmovdqa 64(%rsi), %ymm10
vmovdqa 64(%rdx), %ymm15
vpxor %ymm6, %ymm10, %ymm6
vpxor %ymm7, %ymm15, %ymm7
vextracti128 $1, %ymm6, %xmm11
vextracti128 $1, %ymm7, %xmm12
vpclmulqdq $1, %xmm11, %xmm12, %xmm5
vpclmulqdq $16, %xmm11, %xmm12, %xmm13
vpclmulqdq $17, %xmm11, %xmm12, %xmm14
vpxor %xmm5, %xmm13, %xmm13
vpclmulqdq $0, %xmm11, %xmm12, %xmm5
vpermq $16, %ymm13, %ymm13
vinserti128 $1, %xmm14, %ymm14, %ymm14
vpand mask0011(%rip), %ymm5, %ymm5
vpand mask0110(%rip), %ymm13, %ymm13
vpand mask1100(%rip), %ymm14, %ymm14
vpxor %ymm5, %ymm13, %ymm13
vpxor %ymm13, %ymm14, %ymm5
vpclmulqdq $1, %xmm6, %xmm7, %xmm3
vpclmulqdq $16, %xmm6, %xmm7, %xmm13
vpclmulqdq $17, %xmm6, %xmm7, %xmm14
vpxor %xmm3, %xmm13, %xmm13
vpclmulqdq $0, %xmm6, %xmm7, %xmm3
vpermq $16, %ymm13, %ymm13
vinserti128 $1, %xmm14, %ymm14, %ymm14
vpand mask0011(%rip), %ymm3, %ymm3
vpand mask0110(%rip), %ymm13, %ymm13
vpand mask1100(%rip), %ymm14, %ymm14
vpxor %ymm3, %ymm13, %ymm13
vpxor %ymm13, %ymm14, %ymm3
vpxor %xmm6, %xmm11, %xmm11
vpxor %xmm7, %xmm12, %xmm12
vpclmulqdq $1, %xmm11, %xmm12, %xmm13
vpclmulqdq $16, %xmm11, %xmm12, %xmm6
vpclmulqdq $17, %xmm11, %xmm12, %xmm7
vpxor %xmm13, %xmm6, %xmm6
vpclmulqdq $0, %xmm11, %xmm12, %xmm13
vpermq $16, %ymm6, %ymm6
vinserti128 $1, %xmm7, %ymm7, %ymm7
vpand mask0011(%rip), %ymm13, %ymm13
vpand mask0110(%rip), %ymm6, %ymm6
vpand mask1100(%rip), %ymm7, %ymm7
vpxor %ymm13, %ymm6, %ymm6
vpxor %ymm6, %ymm7, %ymm13
vpxor %ymm13, %ymm5, %ymm13
vpxor %ymm13, %ymm3, %ymm13
vpxor %ymm11, %ymm11, %ymm11
vextracti128 $1, %ymm13, %xmm11
vpxor %ymm5, %ymm11, %ymm5
vpxor %ymm11, %ymm11, %ymm11
vinserti128 $1, %xmm13, %ymm11, %ymm11
vpxor %ymm11, %ymm3, %ymm3
vpxor %ymm3, %ymm8, %ymm8
vpxor %ymm5, %ymm9, %ymm9
vpxor %ymm1, %ymm10, %ymm6
vpxor %ymm4, %ymm15, %ymm7
vextracti128 $1, %ymm6, %xmm11
vextracti128 $1, %ymm7, %xmm12
vpclmulqdq $1, %xmm11, %xmm12, %xmm5
vpclmulqdq $16, %xmm11, %xmm12, %xmm13
vpclmulqdq $17, %xmm11, %xmm12, %xmm14
vpxor %xmm5, %xmm13, %xmm13
vpclmulqdq $0, %xmm11, %xmm12, %xmm5
vpermq $16, %ymm13, %ymm13
vinserti128 $1, %xmm14, %ymm14, %ymm14
vpand mask0011(%rip), %ymm5, %ymm5
vpand mask0110(%rip), %ymm13, %ymm13
vpand mask1100(%rip), %ymm14, %ymm14
vpxor %ymm5, %ymm13, %ymm13
vpxor %ymm13, %ymm14, %ymm5
vpclmulqdq $1, %xmm6, %xmm7, %xmm3
vpclmulqdq $16, %xmm6, %xmm7, %xmm13
vpclmulqdq $17, %xmm6, %xmm7, %xmm14
vpxor %xmm3, %xmm13, %xmm13
vpclmulqdq $0, %xmm6, %xmm7, %xmm3
vpermq $16, %ymm13, %ymm13
vinserti128 $1, %xmm14, %ymm14, %ymm14
vpand mask0011(%rip), %ymm3, %ymm3
vpand mask0110(%rip), %ymm13, %ymm13
vpand mask1100(%rip), %ymm14, %ymm14
vpxor %ymm3, %ymm13, %ymm13
vpxor %ymm13, %ymm14, %ymm3
vpxor %xmm6, %xmm11, %xmm11
vpxor %xmm7, %xmm12, %xmm12
vpclmulqdq $1, %xmm11, %xmm12, %xmm13
vpclmulqdq $16, %xmm11, %xmm12, %xmm6
vpclmulqdq $17, %xmm11, %xmm12, %xmm7
vpxor %xmm13, %xmm6, %xmm6
vpclmulqdq $0, %xmm11, %xmm12, %xmm13
vpermq $16, %ymm6, %ymm6
vinserti128 $1, %xmm7, %ymm7, %ymm7
vpand mask0011(%rip), %ymm13, %ymm13
vpand mask0110(%rip), %ymm6, %ymm6
vpand mask1100(%rip), %ymm7, %ymm7
vpxor %ymm13, %ymm6, %ymm6
vpxor %ymm6, %ymm7, %ymm13
vpxor %ymm13, %ymm5, %ymm13
vpxor %ymm13, %ymm3, %ymm13
vpxor %ymm11, %ymm11, %ymm11
vextracti128 $1, %ymm13, %xmm11
vpxor %ymm5, %ymm11, %ymm5
vpxor %ymm11, %ymm11, %ymm11
vinserti128 $1, %xmm13, %ymm11, %ymm11
vpxor %ymm11, %ymm3, %ymm3
vpxor %ymm3, %ymm8, %ymm8
vpxor %ymm5, %ymm9, %ymm9
vextracti128 $1, %ymm1, %xmm11
vextracti128 $1, %ymm4, %xmm12
vpclmulqdq $1, %xmm11, %xmm12, %xmm7
vpclmulqdq $16, %xmm11, %xmm12, %xmm13
vpclmulqdq $17, %xmm11, %xmm12, %xmm14
vpxor %xmm7, %xmm13, %xmm13
vpclmulqdq $0, %xmm11, %xmm12, %xmm7
vpermq $16, %ymm13, %ymm13
vinserti128 $1, %xmm14, %ymm14, %ymm14
vpand mask0011(%rip), %ymm7, %ymm7
vpand mask0110(%rip), %ymm13, %ymm13
vpand mask1100(%rip), %ymm14, %ymm14
vpxor %ymm7, %ymm13, %ymm13
vpxor %ymm13, %ymm14, %ymm7
vpclmulqdq $1, %xmm1, %xmm4, %xmm6
vpclmulqdq $16, %xmm1, %xmm4, %xmm13
vpclmulqdq $17, %xmm1, %xmm4, %xmm14
vpxor %xmm6, %xmm13, %xmm13
vpclmulqdq $0, %xmm1, %xmm4, %xmm6
vpermq $16, %ymm13, %ymm13
vinserti128 $1, %xmm14, %ymm14, %ymm14
vpand mask0011(%rip), %ymm6, %ymm6
vpand mask0110(%rip), %ymm13, %ymm13
vpand mask1100(%rip), %ymm14, %ymm14
vpxor %ymm6, %ymm13, %ymm13
vpxor %ymm13, %ymm14, %ymm6
vpxor %xmm1, %xmm11, %xmm11
vpxor %xmm4, %xmm12, %xmm12
vpclmulqdq $1, %xmm11, %xmm12, %xmm13
vpclmulqdq $16, %xmm11, %xmm12, %xmm1
vpclmulqdq $17, %xmm11, %xmm12, %xmm4
vpxor %xmm13, %xmm1, %xmm1
vpclmulqdq $0, %xmm11, %xmm12, %xmm13
vpermq $16, %ymm1, %ymm1
vinserti128 $1, %xmm4, %ymm4, %ymm4
vpand mask0011(%rip), %ymm13, %ymm13
vpand mask0110(%rip), %ymm1, %ymm1
vpand mask1100(%rip), %ymm4, %ymm4
vpxor %ymm13, %ymm1, %ymm1
vpxor %ymm1, %ymm4, %ymm13
vpxor %ymm13, %ymm7, %ymm13
vpxor %ymm13, %ymm6, %ymm13
vpxor %ymm11, %ymm11, %ymm11
vextracti128 $1, %ymm13, %xmm11
vpxor %ymm7, %ymm11, %ymm7
vpxor %ymm11, %ymm11, %ymm11
vinserti128 $1, %xmm13, %ymm11, %ymm11
vpxor %ymm11, %ymm6, %ymm6
vpxor %ymm6, %ymm0, %ymm0
vpxor %ymm7, %ymm8, %ymm8
vpxor %ymm6, %ymm3, %ymm3
vpxor %ymm7, %ymm5, %ymm5
vextracti128 $1, %ymm10, %xmm11
vextracti128 $1, %ymm15, %xmm12
vpclmulqdq $1, %xmm11, %xmm12, %xmm4
vpclmulqdq $16, %xmm11, %xmm12, %xmm13
vpclmulqdq $17, %xmm11, %xmm12, %xmm14
vpxor %xmm4, %xmm13, %xmm13
vpclmulqdq $0, %xmm11, %xmm12, %xmm4
vpermq $16, %ymm13, %ymm13
vinserti128 $1, %xmm14, %ymm14, %ymm14
vpand mask0011(%rip), %ymm4, %ymm4
vpand mask0110(%rip), %ymm13, %ymm13
vpand mask1100(%rip), %ymm14, %ymm14
vpxor %ymm4, %ymm13, %ymm13
vpxor %ymm13, %ymm14, %ymm4
vpclmulqdq $1, %xmm10, %xmm15, %xmm1
vpclmulqdq $16, %xmm10, %xmm15, %xmm13
vpclmulqdq $17, %xmm10, %xmm15, %xmm14
vpxor %xmm1, %xmm13, %xmm13
vpclmulqdq $0, %xmm10, %xmm15, %xmm1
vpermq $16, %ymm13, %ymm13
vinserti128 $1, %xmm14, %ymm14, %ymm14
vpand mask0011(%rip), %ymm1, %ymm1
vpand mask0110(%rip), %ymm13, %ymm13
vpand mask1100(%rip), %ymm14, %ymm14
vpxor %ymm1, %ymm13, %ymm13
vpxor %ymm13, %ymm14, %ymm1
vpxor %xmm10, %xmm11, %xmm11
vpxor %xmm15, %xmm12, %xmm12
vpclmulqdq $1, %xmm11, %xmm12, %xmm13
vpclmulqdq $16, %xmm11, %xmm12, %xmm10
vpclmulqdq $17, %xmm11, %xmm12, %xmm15
vpxor %xmm13, %xmm10, %xmm10
vpclmulqdq $0, %xmm11, %xmm12, %xmm13
vpermq $16, %ymm10, %ymm10
vinserti128 $1, %xmm15, %ymm15, %ymm15
vpand mask0011(%rip), %ymm13, %ymm13
vpand mask0110(%rip), %ymm10, %ymm10
vpand mask1100(%rip), %ymm15, %ymm15
vpxor %ymm13, %ymm10, %ymm10
vpxor %ymm10, %ymm15, %ymm13
vpxor %ymm13, %ymm4, %ymm13
vpxor %ymm13, %ymm1, %ymm13
vpxor %ymm11, %ymm11, %ymm11
vextracti128 $1, %ymm13, %xmm11
vpxor %ymm4, %ymm11, %ymm4
vpxor %ymm11, %ymm11, %ymm11
vinserti128 $1, %xmm13, %ymm11, %ymm11
vpxor %ymm11, %ymm1, %ymm1
vpxor %ymm1, %ymm3, %ymm3
vpxor %ymm4, %ymm5, %ymm5
vpxor %ymm9, %ymm3, %ymm3
vpxor %ymm5, %ymm1, %ymm1
vpand mask1100(%rip), %ymm8, %ymm13
vpand mask0011(%rip), %ymm3, %ymm12
vpxor %ymm12, %ymm13, %ymm12
vpsrlq $37, %ymm12, %ymm12
vpermq $78, %ymm12, %ymm12
vpxor %ymm12, %ymm2, %ymm2
vpand mask1000(%rip), %ymm8, %ymm12
vpand mask0111(%rip), %ymm3, %ymm13
vpxor %ymm12, %ymm13, %ymm12
vpsllq $27, %ymm12, %ymm12
vpermq $147, %ymm12, %ymm12
vpxor %ymm12, %ymm2, %ymm2
vpand mask1100(%rip), %ymm3, %ymm13
vpand mask0011(%rip), %ymm1, %ymm12
vpxor %ymm12, %ymm13, %ymm12
vpsrlq $37, %ymm12, %ymm12
vpermq $78, %ymm12, %ymm12
vpxor %ymm12, %ymm0, %ymm0
vpand mask1000(%rip), %ymm3, %ymm12
vpand mask0111(%rip), %ymm1, %ymm13
vpxor %ymm12, %ymm13, %ymm12
vpsllq $27, %ymm12, %ymm12
vpermq $147, %ymm12, %ymm12
vpxor %ymm12, %ymm0, %ymm0
vpand mask1100(%rip), %ymm1, %ymm13
vpand mask0011(%rip), %ymm4, %ymm12
vpxor %ymm12, %ymm13, %ymm12
vpsrlq $37, %ymm12, %ymm12
vpermq $78, %ymm12, %ymm12
vpxor %ymm12, %ymm8, %ymm8
vpand mask1000(%rip), %ymm1, %ymm12
vpand mask0111(%rip), %ymm4, %ymm13
vpxor %ymm12, %ymm13, %ymm12
vpsllq $27, %ymm12, %ymm12
vpermq $147, %ymm12, %ymm12
vpxor %ymm12, %ymm8, %ymm8
vpand low165(%rip), %ymm8, %ymm8
vmovdqa %ymm2, 0(%rdi)
vmovdqa %ymm0, 32(%rdi)
vmovdqa %ymm8, 64(%rdi)
ret
