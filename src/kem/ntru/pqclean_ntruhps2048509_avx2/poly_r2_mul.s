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
low253:
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
.word 65535
.word 65535
.word 65535
.word 8191
.text
.global PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul
.global _PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul
PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul:
_PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul:
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
vextracti128 $1, %ymm1, %xmm11
vextracti128 $1, %ymm4, %xmm12
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
vpxor %xmm1, %xmm11, %xmm11
vpxor %xmm4, %xmm12, %xmm12
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
vpclmulqdq $1, %xmm1, %xmm4, %xmm8
vpclmulqdq $16, %xmm1, %xmm4, %xmm14
vpclmulqdq $17, %xmm1, %xmm4, %xmm15
vpxor %xmm8, %xmm14, %xmm14
vpclmulqdq $0, %xmm1, %xmm4, %xmm8
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
vextracti128 $1, %ymm6, %xmm11
vextracti128 $1, %ymm7, %xmm12
vpclmulqdq $1, %xmm11, %xmm12, %xmm1
vpclmulqdq $16, %xmm11, %xmm12, %xmm14
vpclmulqdq $17, %xmm11, %xmm12, %xmm15
vpxor %xmm1, %xmm14, %xmm14
vpclmulqdq $0, %xmm11, %xmm12, %xmm1
vpermq $16, %ymm14, %ymm14
vinserti128 $1, %xmm15, %ymm15, %ymm15
vpand mask0011(%rip), %ymm1, %ymm1
vpand mask0110(%rip), %ymm14, %ymm14
vpand mask1100(%rip), %ymm15, %ymm15
vpxor %ymm1, %ymm14, %ymm14
vpxor %ymm14, %ymm15, %ymm1
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
vpclmulqdq $1, %xmm6, %xmm7, %xmm0
vpclmulqdq $16, %xmm6, %xmm7, %xmm14
vpclmulqdq $17, %xmm6, %xmm7, %xmm15
vpxor %xmm0, %xmm14, %xmm14
vpclmulqdq $0, %xmm6, %xmm7, %xmm0
vpermq $16, %ymm14, %ymm14
vinserti128 $1, %xmm15, %ymm15, %ymm15
vpand mask0011(%rip), %ymm0, %ymm0
vpand mask0110(%rip), %ymm14, %ymm14
vpand mask1100(%rip), %ymm15, %ymm15
vpxor %ymm0, %ymm14, %ymm14
vpxor %ymm14, %ymm15, %ymm0
vpxor %ymm13, %ymm1, %ymm13
vpxor %ymm13, %ymm0, %ymm13
vpxor %ymm11, %ymm11, %ymm11
vextracti128 $1, %ymm13, %xmm11
vpxor %ymm1, %ymm11, %ymm1
vpxor %ymm11, %ymm11, %ymm11
vinserti128 $1, %xmm13, %ymm11, %ymm11
vpxor %ymm11, %ymm0, %ymm0
vpxor %ymm0, %ymm2, %ymm0
vpxor %ymm0, %ymm8, %ymm0
vpxor %ymm1, %ymm5, %ymm1
vpxor %ymm1, %ymm9, %ymm1
vpxor %ymm0, %ymm5, %ymm5
vpxor %ymm1, %ymm8, %ymm8
vpand mask1000(%rip), %ymm5, %ymm13
vpand mask0111(%rip), %ymm8, %ymm12
vpxor %ymm12, %ymm13, %ymm12
vpsrlq $61, %ymm12, %ymm12
vpermq $147, %ymm12, %ymm12
vpxor %ymm12, %ymm2, %ymm2
vpsllq $3, %ymm8, %ymm12
vpxor %ymm12, %ymm2, %ymm2
vpand mask1000(%rip), %ymm8, %ymm13
vpand mask0111(%rip), %ymm9, %ymm12
vpxor %ymm12, %ymm13, %ymm12
vpsrlq $61, %ymm12, %ymm12
vpermq $147, %ymm12, %ymm12
vpxor %ymm12, %ymm5, %ymm5
vpsllq $3, %ymm9, %ymm12
vpxor %ymm12, %ymm5, %ymm5
vpand low253(%rip), %ymm5, %ymm5
vmovdqa %ymm2, 0(%rdi)
vmovdqa %ymm5, 32(%rdi)
ret
