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
mask0001:
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
.word 0
.word 0
.word 0
.word 0
mask1110:
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
.word 65535
.word 65535
.word 65535
.word 65535
low53:
.word 65535
.word 65535
.word 65535
.word 31
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
.text
.global PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_mul
.global _PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_mul
PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_mul:
_PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_mul:
vmovdqa 0(%rsi), %ymm4
vmovdqa 32(%rsi), %ymm5
vmovdqa 0(%rdx), %ymm6
vmovdqa 32(%rdx), %ymm7
vpxor %ymm4, %ymm5, %ymm13
vpxor %ymm6, %ymm7, %ymm14
vextracti128 $1, %ymm4, %xmm8
vextracti128 $1, %ymm6, %xmm9
vpclmulqdq $1, %xmm8, %xmm9, %xmm1
vpclmulqdq $16, %xmm8, %xmm9, %xmm11
vpclmulqdq $17, %xmm8, %xmm9, %xmm12
vpxor %xmm1, %xmm11, %xmm11
vpclmulqdq $0, %xmm8, %xmm9, %xmm1
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm1, %ymm1
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm1, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm1
vpxor %xmm4, %xmm8, %xmm8
vpxor %xmm6, %xmm9, %xmm9
vpclmulqdq $1, %xmm8, %xmm9, %xmm10
vpclmulqdq $16, %xmm8, %xmm9, %xmm11
vpclmulqdq $17, %xmm8, %xmm9, %xmm12
vpxor %xmm10, %xmm11, %xmm11
vpclmulqdq $0, %xmm8, %xmm9, %xmm10
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm10, %ymm10
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm10, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm10
vpclmulqdq $1, %xmm4, %xmm6, %xmm0
vpclmulqdq $16, %xmm4, %xmm6, %xmm11
vpclmulqdq $17, %xmm4, %xmm6, %xmm12
vpxor %xmm0, %xmm11, %xmm11
vpclmulqdq $0, %xmm4, %xmm6, %xmm0
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm0, %ymm0
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm0, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm0
vpxor %ymm10, %ymm1, %ymm10
vpxor %ymm10, %ymm0, %ymm10
vpxor %ymm8, %ymm8, %ymm8
vextracti128 $1, %ymm10, %xmm8
vpxor %ymm1, %ymm8, %ymm1
vpxor %ymm8, %ymm8, %ymm8
vinserti128 $1, %xmm10, %ymm8, %ymm8
vpxor %ymm8, %ymm0, %ymm0
vextracti128 $1, %ymm5, %xmm8
vextracti128 $1, %ymm7, %xmm9
vpclmulqdq $1, %xmm8, %xmm9, %xmm3
vpclmulqdq $16, %xmm8, %xmm9, %xmm11
vpclmulqdq $17, %xmm8, %xmm9, %xmm12
vpxor %xmm3, %xmm11, %xmm11
vpclmulqdq $0, %xmm8, %xmm9, %xmm3
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm3, %ymm3
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm3, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm3
vpxor %xmm5, %xmm8, %xmm8
vpxor %xmm7, %xmm9, %xmm9
vpclmulqdq $1, %xmm8, %xmm9, %xmm10
vpclmulqdq $16, %xmm8, %xmm9, %xmm11
vpclmulqdq $17, %xmm8, %xmm9, %xmm12
vpxor %xmm10, %xmm11, %xmm11
vpclmulqdq $0, %xmm8, %xmm9, %xmm10
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm10, %ymm10
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm10, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm10
vpclmulqdq $1, %xmm5, %xmm7, %xmm2
vpclmulqdq $16, %xmm5, %xmm7, %xmm11
vpclmulqdq $17, %xmm5, %xmm7, %xmm12
vpxor %xmm2, %xmm11, %xmm11
vpclmulqdq $0, %xmm5, %xmm7, %xmm2
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm2, %ymm2
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm2, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm2
vpxor %ymm10, %ymm3, %ymm10
vpxor %ymm10, %ymm2, %ymm10
vpxor %ymm8, %ymm8, %ymm8
vextracti128 $1, %ymm10, %xmm8
vpxor %ymm3, %ymm8, %ymm3
vpxor %ymm8, %ymm8, %ymm8
vinserti128 $1, %xmm10, %ymm8, %ymm8
vpxor %ymm8, %ymm2, %ymm2
vextracti128 $1, %ymm13, %xmm8
vextracti128 $1, %ymm14, %xmm9
vpclmulqdq $1, %xmm8, %xmm9, %xmm5
vpclmulqdq $16, %xmm8, %xmm9, %xmm11
vpclmulqdq $17, %xmm8, %xmm9, %xmm12
vpxor %xmm5, %xmm11, %xmm11
vpclmulqdq $0, %xmm8, %xmm9, %xmm5
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm5, %ymm5
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm5, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm5
vpxor %xmm13, %xmm8, %xmm8
vpxor %xmm14, %xmm9, %xmm9
vpclmulqdq $1, %xmm8, %xmm9, %xmm10
vpclmulqdq $16, %xmm8, %xmm9, %xmm11
vpclmulqdq $17, %xmm8, %xmm9, %xmm12
vpxor %xmm10, %xmm11, %xmm11
vpclmulqdq $0, %xmm8, %xmm9, %xmm10
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm10, %ymm10
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm10, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm10
vpclmulqdq $1, %xmm13, %xmm14, %xmm4
vpclmulqdq $16, %xmm13, %xmm14, %xmm11
vpclmulqdq $17, %xmm13, %xmm14, %xmm12
vpxor %xmm4, %xmm11, %xmm11
vpclmulqdq $0, %xmm13, %xmm14, %xmm4
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm4, %ymm4
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm4, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm4
vpxor %ymm10, %ymm5, %ymm10
vpxor %ymm10, %ymm4, %ymm10
vpxor %ymm8, %ymm8, %ymm8
vextracti128 $1, %ymm10, %xmm8
vpxor %ymm5, %ymm8, %ymm5
vpxor %ymm8, %ymm8, %ymm8
vinserti128 $1, %xmm10, %ymm8, %ymm8
vpxor %ymm8, %ymm4, %ymm4
vpxor %ymm0, %ymm4, %ymm4
vpxor %ymm1, %ymm5, %ymm5
vpxor %ymm2, %ymm4, %ymm4
vpxor %ymm3, %ymm5, %ymm5
vpxor %ymm4, %ymm1, %ymm1
vpxor %ymm5, %ymm2, %ymm2
vpand mask1110(%rip), %ymm3, %ymm8
vpsllq $11, %ymm8, %ymm8
vpermq $57, %ymm8, %ymm8
vpsrlq $53, %ymm3, %ymm14
vpxor %ymm14, %ymm8, %ymm8
vpxor %ymm0, %ymm8, %ymm8
vpand low53(%rip), %ymm3, %ymm9
vmovdqa %ymm8, 0(%rdi)
vmovdqa %ymm1, 32(%rdi)
vmovdqa %ymm2, 64(%rdi)
vmovdqa %ymm9, 96(%rdi)
vmovdqa 0(%rdi), %ymm4
vmovdqa 32(%rdi), %ymm5
vmovdqa 64(%rdi), %ymm6
vmovdqa 96(%rdi), %ymm7
vpxor %ymm0, %ymm6, %ymm6
vpand low53(%rip), %ymm1, %ymm11
vpxor %ymm11, %ymm7, %ymm7
vpand mask1110(%rip), %ymm1, %ymm8
vpsllq $11, %ymm8, %ymm8
vpermq $57, %ymm8, %ymm8
vpsrlq $53, %ymm1, %ymm11
vpxor %ymm11, %ymm8, %ymm8
vpand mask0001(%rip), %ymm2, %ymm9
vpermq $57, %ymm9, %ymm9
vpsllq $11, %ymm9, %ymm9
vpand mask1110(%rip), %ymm2, %ymm10
vpsllq $11, %ymm10, %ymm10
vpermq $57, %ymm10, %ymm10
vpsrlq $53, %ymm2, %ymm11
vpxor %ymm11, %ymm10, %ymm10
vpxor %ymm8, %ymm9, %ymm9
vpxor %ymm9, %ymm4, %ymm4
vpand mask0001(%rip), %ymm3, %ymm8
vpermq $57, %ymm8, %ymm8
vpsllq $11, %ymm8, %ymm8
vpand mask1110(%rip), %ymm3, %ymm9
vpsllq $11, %ymm9, %ymm9
vpermq $57, %ymm9, %ymm9
vpsrlq $53, %ymm3, %ymm11
vpxor %ymm11, %ymm9, %ymm9
vpxor %ymm8, %ymm10, %ymm10
vpxor %ymm10, %ymm5, %ymm5
vpxor %ymm9, %ymm6, %ymm6
vmovdqa %ymm4, 0(%rdi)
vmovdqa %ymm5, 32(%rdi)
vmovdqa %ymm6, 64(%rdi)
vmovdqa %ymm7, 96(%rdi)
vmovdqa 64(%rsi), %ymm4
vmovdqa 96(%rsi), %ymm5
vmovdqa 64(%rdx), %ymm6
vmovdqa 96(%rdx), %ymm7
vpxor %ymm4, %ymm5, %ymm13
vpxor %ymm6, %ymm7, %ymm14
vextracti128 $1, %ymm4, %xmm8
vextracti128 $1, %ymm6, %xmm9
vpclmulqdq $1, %xmm8, %xmm9, %xmm1
vpclmulqdq $16, %xmm8, %xmm9, %xmm11
vpclmulqdq $17, %xmm8, %xmm9, %xmm12
vpxor %xmm1, %xmm11, %xmm11
vpclmulqdq $0, %xmm8, %xmm9, %xmm1
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm1, %ymm1
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm1, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm1
vpxor %xmm4, %xmm8, %xmm8
vpxor %xmm6, %xmm9, %xmm9
vpclmulqdq $1, %xmm8, %xmm9, %xmm10
vpclmulqdq $16, %xmm8, %xmm9, %xmm11
vpclmulqdq $17, %xmm8, %xmm9, %xmm12
vpxor %xmm10, %xmm11, %xmm11
vpclmulqdq $0, %xmm8, %xmm9, %xmm10
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm10, %ymm10
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm10, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm10
vpclmulqdq $1, %xmm4, %xmm6, %xmm0
vpclmulqdq $16, %xmm4, %xmm6, %xmm11
vpclmulqdq $17, %xmm4, %xmm6, %xmm12
vpxor %xmm0, %xmm11, %xmm11
vpclmulqdq $0, %xmm4, %xmm6, %xmm0
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm0, %ymm0
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm0, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm0
vpxor %ymm10, %ymm1, %ymm10
vpxor %ymm10, %ymm0, %ymm10
vpxor %ymm8, %ymm8, %ymm8
vextracti128 $1, %ymm10, %xmm8
vpxor %ymm1, %ymm8, %ymm1
vpxor %ymm8, %ymm8, %ymm8
vinserti128 $1, %xmm10, %ymm8, %ymm8
vpxor %ymm8, %ymm0, %ymm0
vextracti128 $1, %ymm5, %xmm8
vextracti128 $1, %ymm7, %xmm9
vpclmulqdq $1, %xmm8, %xmm9, %xmm3
vpclmulqdq $16, %xmm8, %xmm9, %xmm11
vpclmulqdq $17, %xmm8, %xmm9, %xmm12
vpxor %xmm3, %xmm11, %xmm11
vpclmulqdq $0, %xmm8, %xmm9, %xmm3
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm3, %ymm3
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm3, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm3
vpxor %xmm5, %xmm8, %xmm8
vpxor %xmm7, %xmm9, %xmm9
vpclmulqdq $1, %xmm8, %xmm9, %xmm10
vpclmulqdq $16, %xmm8, %xmm9, %xmm11
vpclmulqdq $17, %xmm8, %xmm9, %xmm12
vpxor %xmm10, %xmm11, %xmm11
vpclmulqdq $0, %xmm8, %xmm9, %xmm10
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm10, %ymm10
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm10, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm10
vpclmulqdq $1, %xmm5, %xmm7, %xmm2
vpclmulqdq $16, %xmm5, %xmm7, %xmm11
vpclmulqdq $17, %xmm5, %xmm7, %xmm12
vpxor %xmm2, %xmm11, %xmm11
vpclmulqdq $0, %xmm5, %xmm7, %xmm2
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm2, %ymm2
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm2, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm2
vpxor %ymm10, %ymm3, %ymm10
vpxor %ymm10, %ymm2, %ymm10
vpxor %ymm8, %ymm8, %ymm8
vextracti128 $1, %ymm10, %xmm8
vpxor %ymm3, %ymm8, %ymm3
vpxor %ymm8, %ymm8, %ymm8
vinserti128 $1, %xmm10, %ymm8, %ymm8
vpxor %ymm8, %ymm2, %ymm2
vextracti128 $1, %ymm13, %xmm8
vextracti128 $1, %ymm14, %xmm9
vpclmulqdq $1, %xmm8, %xmm9, %xmm5
vpclmulqdq $16, %xmm8, %xmm9, %xmm11
vpclmulqdq $17, %xmm8, %xmm9, %xmm12
vpxor %xmm5, %xmm11, %xmm11
vpclmulqdq $0, %xmm8, %xmm9, %xmm5
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm5, %ymm5
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm5, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm5
vpxor %xmm13, %xmm8, %xmm8
vpxor %xmm14, %xmm9, %xmm9
vpclmulqdq $1, %xmm8, %xmm9, %xmm10
vpclmulqdq $16, %xmm8, %xmm9, %xmm11
vpclmulqdq $17, %xmm8, %xmm9, %xmm12
vpxor %xmm10, %xmm11, %xmm11
vpclmulqdq $0, %xmm8, %xmm9, %xmm10
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm10, %ymm10
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm10, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm10
vpclmulqdq $1, %xmm13, %xmm14, %xmm4
vpclmulqdq $16, %xmm13, %xmm14, %xmm11
vpclmulqdq $17, %xmm13, %xmm14, %xmm12
vpxor %xmm4, %xmm11, %xmm11
vpclmulqdq $0, %xmm13, %xmm14, %xmm4
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm4, %ymm4
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm4, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm4
vpxor %ymm10, %ymm5, %ymm10
vpxor %ymm10, %ymm4, %ymm10
vpxor %ymm8, %ymm8, %ymm8
vextracti128 $1, %ymm10, %xmm8
vpxor %ymm5, %ymm8, %ymm5
vpxor %ymm8, %ymm8, %ymm8
vinserti128 $1, %xmm10, %ymm8, %ymm8
vpxor %ymm8, %ymm4, %ymm4
vpxor %ymm0, %ymm4, %ymm4
vpxor %ymm1, %ymm5, %ymm5
vpxor %ymm2, %ymm4, %ymm4
vpxor %ymm3, %ymm5, %ymm5
vpxor %ymm4, %ymm1, %ymm1
vpxor %ymm5, %ymm2, %ymm2
vmovdqa 0(%rdi), %ymm4
vmovdqa 32(%rdi), %ymm5
vmovdqa 64(%rdi), %ymm6
vmovdqa 96(%rdi), %ymm7
vpxor %ymm0, %ymm6, %ymm6
vpand low53(%rip), %ymm1, %ymm11
vpxor %ymm11, %ymm7, %ymm7
vpand mask1110(%rip), %ymm1, %ymm8
vpsllq $11, %ymm8, %ymm8
vpermq $57, %ymm8, %ymm8
vpsrlq $53, %ymm1, %ymm11
vpxor %ymm11, %ymm8, %ymm8
vpand mask0001(%rip), %ymm2, %ymm9
vpermq $57, %ymm9, %ymm9
vpsllq $11, %ymm9, %ymm9
vpand mask1110(%rip), %ymm2, %ymm10
vpsllq $11, %ymm10, %ymm10
vpermq $57, %ymm10, %ymm10
vpsrlq $53, %ymm2, %ymm11
vpxor %ymm11, %ymm10, %ymm10
vpxor %ymm8, %ymm9, %ymm9
vpxor %ymm9, %ymm4, %ymm4
vpand mask0001(%rip), %ymm3, %ymm8
vpermq $57, %ymm8, %ymm8
vpsllq $11, %ymm8, %ymm8
vpand mask1110(%rip), %ymm3, %ymm9
vpsllq $11, %ymm9, %ymm9
vpermq $57, %ymm9, %ymm9
vpsrlq $53, %ymm3, %ymm11
vpxor %ymm11, %ymm9, %ymm9
vpxor %ymm8, %ymm10, %ymm10
vpxor %ymm10, %ymm5, %ymm5
vpxor %ymm9, %ymm6, %ymm6
vpand mask1110(%rip), %ymm3, %ymm8
vpsllq $11, %ymm8, %ymm8
vpermq $57, %ymm8, %ymm8
vpsrlq $53, %ymm3, %ymm10
vpxor %ymm10, %ymm8, %ymm8
vpand mask0001(%rip), %ymm0, %ymm9
vpermq $57, %ymm9, %ymm9
vpsllq $11, %ymm9, %ymm9
vpxor %ymm8, %ymm9, %ymm9
vpxor %ymm9, %ymm4, %ymm4
vpand mask1110(%rip), %ymm0, %ymm8
vpsllq $11, %ymm8, %ymm8
vpermq $57, %ymm8, %ymm8
vpsrlq $53, %ymm0, %ymm10
vpxor %ymm10, %ymm8, %ymm8
vpand mask0001(%rip), %ymm1, %ymm9
vpermq $57, %ymm9, %ymm9
vpsllq $11, %ymm9, %ymm9
vpxor %ymm8, %ymm9, %ymm9
vpxor %ymm9, %ymm5, %ymm5
vpand mask1110(%rip), %ymm1, %ymm8
vpsllq $11, %ymm8, %ymm8
vpermq $57, %ymm8, %ymm8
vpsrlq $53, %ymm1, %ymm10
vpxor %ymm10, %ymm8, %ymm8
vpand mask0001(%rip), %ymm2, %ymm9
vpermq $57, %ymm9, %ymm9
vpsllq $11, %ymm9, %ymm9
vpxor %ymm8, %ymm9, %ymm9
vpxor %ymm9, %ymm6, %ymm6
vpand mask1110(%rip), %ymm2, %ymm8
vpsllq $11, %ymm8, %ymm8
vpermq $57, %ymm8, %ymm8
vpsrlq $53, %ymm2, %ymm10
vpxor %ymm10, %ymm8, %ymm8
vpand mask0001(%rip), %ymm3, %ymm9
vpermq $57, %ymm9, %ymm9
vpsllq $11, %ymm9, %ymm9
vpxor %ymm8, %ymm9, %ymm9
vpxor %ymm9, %ymm7, %ymm7
vmovdqa %ymm4, 0(%rdi)
vmovdqa %ymm5, 32(%rdi)
vmovdqa %ymm6, 64(%rdi)
vmovdqa %ymm7, 96(%rdi)
vmovdqa 0(%rsi), %ymm4
vmovdqa 32(%rsi), %ymm5
vmovdqa 64(%rsi), %ymm8
vmovdqa 96(%rsi), %ymm9
vmovdqa 0(%rdx), %ymm6
vmovdqa 32(%rdx), %ymm7
vmovdqa 64(%rdx), %ymm10
vmovdqa 96(%rdx), %ymm11
vpxor %ymm4, %ymm8, %ymm4
vpxor %ymm5, %ymm9, %ymm5
vpxor %ymm6, %ymm10, %ymm6
vpxor %ymm7, %ymm11, %ymm7
vpxor %ymm4, %ymm5, %ymm13
vpxor %ymm6, %ymm7, %ymm14
vextracti128 $1, %ymm4, %xmm8
vextracti128 $1, %ymm6, %xmm9
vpclmulqdq $1, %xmm8, %xmm9, %xmm1
vpclmulqdq $16, %xmm8, %xmm9, %xmm11
vpclmulqdq $17, %xmm8, %xmm9, %xmm12
vpxor %xmm1, %xmm11, %xmm11
vpclmulqdq $0, %xmm8, %xmm9, %xmm1
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm1, %ymm1
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm1, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm1
vpxor %xmm4, %xmm8, %xmm8
vpxor %xmm6, %xmm9, %xmm9
vpclmulqdq $1, %xmm8, %xmm9, %xmm10
vpclmulqdq $16, %xmm8, %xmm9, %xmm11
vpclmulqdq $17, %xmm8, %xmm9, %xmm12
vpxor %xmm10, %xmm11, %xmm11
vpclmulqdq $0, %xmm8, %xmm9, %xmm10
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm10, %ymm10
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm10, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm10
vpclmulqdq $1, %xmm4, %xmm6, %xmm0
vpclmulqdq $16, %xmm4, %xmm6, %xmm11
vpclmulqdq $17, %xmm4, %xmm6, %xmm12
vpxor %xmm0, %xmm11, %xmm11
vpclmulqdq $0, %xmm4, %xmm6, %xmm0
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm0, %ymm0
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm0, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm0
vpxor %ymm10, %ymm1, %ymm10
vpxor %ymm10, %ymm0, %ymm10
vpxor %ymm8, %ymm8, %ymm8
vextracti128 $1, %ymm10, %xmm8
vpxor %ymm1, %ymm8, %ymm1
vpxor %ymm8, %ymm8, %ymm8
vinserti128 $1, %xmm10, %ymm8, %ymm8
vpxor %ymm8, %ymm0, %ymm0
vextracti128 $1, %ymm5, %xmm8
vextracti128 $1, %ymm7, %xmm9
vpclmulqdq $1, %xmm8, %xmm9, %xmm3
vpclmulqdq $16, %xmm8, %xmm9, %xmm11
vpclmulqdq $17, %xmm8, %xmm9, %xmm12
vpxor %xmm3, %xmm11, %xmm11
vpclmulqdq $0, %xmm8, %xmm9, %xmm3
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm3, %ymm3
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm3, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm3
vpxor %xmm5, %xmm8, %xmm8
vpxor %xmm7, %xmm9, %xmm9
vpclmulqdq $1, %xmm8, %xmm9, %xmm10
vpclmulqdq $16, %xmm8, %xmm9, %xmm11
vpclmulqdq $17, %xmm8, %xmm9, %xmm12
vpxor %xmm10, %xmm11, %xmm11
vpclmulqdq $0, %xmm8, %xmm9, %xmm10
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm10, %ymm10
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm10, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm10
vpclmulqdq $1, %xmm5, %xmm7, %xmm2
vpclmulqdq $16, %xmm5, %xmm7, %xmm11
vpclmulqdq $17, %xmm5, %xmm7, %xmm12
vpxor %xmm2, %xmm11, %xmm11
vpclmulqdq $0, %xmm5, %xmm7, %xmm2
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm2, %ymm2
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm2, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm2
vpxor %ymm10, %ymm3, %ymm10
vpxor %ymm10, %ymm2, %ymm10
vpxor %ymm8, %ymm8, %ymm8
vextracti128 $1, %ymm10, %xmm8
vpxor %ymm3, %ymm8, %ymm3
vpxor %ymm8, %ymm8, %ymm8
vinserti128 $1, %xmm10, %ymm8, %ymm8
vpxor %ymm8, %ymm2, %ymm2
vextracti128 $1, %ymm13, %xmm8
vextracti128 $1, %ymm14, %xmm9
vpclmulqdq $1, %xmm8, %xmm9, %xmm5
vpclmulqdq $16, %xmm8, %xmm9, %xmm11
vpclmulqdq $17, %xmm8, %xmm9, %xmm12
vpxor %xmm5, %xmm11, %xmm11
vpclmulqdq $0, %xmm8, %xmm9, %xmm5
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm5, %ymm5
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm5, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm5
vpxor %xmm13, %xmm8, %xmm8
vpxor %xmm14, %xmm9, %xmm9
vpclmulqdq $1, %xmm8, %xmm9, %xmm10
vpclmulqdq $16, %xmm8, %xmm9, %xmm11
vpclmulqdq $17, %xmm8, %xmm9, %xmm12
vpxor %xmm10, %xmm11, %xmm11
vpclmulqdq $0, %xmm8, %xmm9, %xmm10
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm10, %ymm10
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm10, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm10
vpclmulqdq $1, %xmm13, %xmm14, %xmm4
vpclmulqdq $16, %xmm13, %xmm14, %xmm11
vpclmulqdq $17, %xmm13, %xmm14, %xmm12
vpxor %xmm4, %xmm11, %xmm11
vpclmulqdq $0, %xmm13, %xmm14, %xmm4
vpermq $16, %ymm11, %ymm11
vinserti128 $1, %xmm12, %ymm12, %ymm12
vpand mask0011(%rip), %ymm4, %ymm4
vpand mask0110(%rip), %ymm11, %ymm11
vpand mask1100(%rip), %ymm12, %ymm12
vpxor %ymm4, %ymm11, %ymm11
vpxor %ymm11, %ymm12, %ymm4
vpxor %ymm10, %ymm5, %ymm10
vpxor %ymm10, %ymm4, %ymm10
vpxor %ymm8, %ymm8, %ymm8
vextracti128 $1, %ymm10, %xmm8
vpxor %ymm5, %ymm8, %ymm5
vpxor %ymm8, %ymm8, %ymm8
vinserti128 $1, %xmm10, %ymm8, %ymm8
vpxor %ymm8, %ymm4, %ymm4
vpxor %ymm0, %ymm4, %ymm4
vpxor %ymm1, %ymm5, %ymm5
vpxor %ymm2, %ymm4, %ymm4
vpxor %ymm3, %ymm5, %ymm5
vpxor %ymm4, %ymm1, %ymm1
vpxor %ymm5, %ymm2, %ymm2
vmovdqa 0(%rdi), %ymm8
vmovdqa 32(%rdi), %ymm9
vmovdqa 64(%rdi), %ymm10
vmovdqa 96(%rdi), %ymm11
vpxor %ymm0, %ymm10, %ymm10
vpand low53(%rip), %ymm1, %ymm7
vpxor %ymm7, %ymm11, %ymm11
vpand mask1110(%rip), %ymm1, %ymm4
vpsllq $11, %ymm4, %ymm4
vpermq $57, %ymm4, %ymm4
vpsrlq $53, %ymm1, %ymm7
vpxor %ymm7, %ymm4, %ymm4
vpand mask0001(%rip), %ymm2, %ymm5
vpermq $57, %ymm5, %ymm5
vpsllq $11, %ymm5, %ymm5
vpand mask1110(%rip), %ymm2, %ymm6
vpsllq $11, %ymm6, %ymm6
vpermq $57, %ymm6, %ymm6
vpsrlq $53, %ymm2, %ymm7
vpxor %ymm7, %ymm6, %ymm6
vpxor %ymm4, %ymm5, %ymm5
vpxor %ymm5, %ymm8, %ymm8
vpand mask0001(%rip), %ymm3, %ymm4
vpermq $57, %ymm4, %ymm4
vpsllq $11, %ymm4, %ymm4
vpand mask1110(%rip), %ymm3, %ymm5
vpsllq $11, %ymm5, %ymm5
vpermq $57, %ymm5, %ymm5
vpsrlq $53, %ymm3, %ymm7
vpxor %ymm7, %ymm5, %ymm5
vpxor %ymm4, %ymm6, %ymm6
vpxor %ymm6, %ymm9, %ymm9
vpxor %ymm5, %ymm10, %ymm10
vmovdqa %ymm8, 0(%rdi)
vmovdqa %ymm9, 32(%rdi)
vmovdqa %ymm10, 64(%rdi)
vmovdqa %ymm11, 96(%rdi)
ret
