.data
.p2align 5
mask_modq:
.word 2047
.word 2047
.word 2047
.word 2047
.word 2047
.word 2047
.word 2047
.word 2047
.word 2047
.word 2047
.word 2047
.word 2047
.word 2047
.word 2047
.word 2047
.word 2047
mask_ff:
.word 0xff
.word 0xff
.word 0xff
.word 0xff
.word 0xff
.word 0xff
.word 0xff
.word 0xff
.word 0xff
.word 0xff
.word 0xff
.word 0xff
.word 0xff
.word 0xff
.word 0xff
.word 0xff
mask_f:
.word 0xf
.word 0xf
.word 0xf
.word 0xf
.word 0xf
.word 0xf
.word 0xf
.word 0xf
.word 0xf
.word 0xf
.word 0xf
.word 0xf
.word 0xf
.word 0xf
.word 0xf
.word 0xf
mask_3:
.word 0x03
.word 0x03
.word 0x03
.word 0x03
.word 0x03
.word 0x03
.word 0x03
.word 0x03
.word 0x03
.word 0x03
.word 0x03
.word 0x03
.word 0x03
.word 0x03
.word 0x03
.word 0x03
.text
.global PQCLEAN_NTRUHPS2048509_AVX2_poly_Rq_to_S3
.global _PQCLEAN_NTRUHPS2048509_AVX2_poly_Rq_to_S3
PQCLEAN_NTRUHPS2048509_AVX2_poly_Rq_to_S3:
_PQCLEAN_NTRUHPS2048509_AVX2_poly_Rq_to_S3:
vmovdqa mask_modq(%rip), %ymm6
vmovdqa 992(%rsi), %ymm5
vpand %ymm6, %ymm5, %ymm5
vpermq $3, %ymm5, %ymm5
vpslld $16, %ymm5, %ymm1
vpsrld $16, %ymm1, %ymm5
vpor %ymm5, %ymm1, %ymm5
vbroadcastss %xmm5, %ymm5
vpsrlw $10, %ymm5, %ymm1
vpaddw %ymm5, %ymm1, %ymm5
vpsrlw $8, %ymm5, %ymm1
vpand mask_ff(%rip), %ymm5, %ymm5
vpaddw %ymm1, %ymm5, %ymm1
vpand mask_f(%rip), %ymm1, %ymm5
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm5, %ymm1
vpand mask_3(%rip), %ymm1, %ymm5
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm5, %ymm1
vpand mask_3(%rip), %ymm1, %ymm5
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm5, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm5
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm5, %ymm1
vpsllw $1, %ymm1, %ymm5
vmovdqa 0(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 0(%rdi)
vmovdqa 32(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 32(%rdi)
vmovdqa 64(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 64(%rdi)
vmovdqa 96(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 96(%rdi)
vmovdqa 128(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 128(%rdi)
vmovdqa 160(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 160(%rdi)
vmovdqa 192(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 192(%rdi)
vmovdqa 224(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 224(%rdi)
vmovdqa 256(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 256(%rdi)
vmovdqa 288(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 288(%rdi)
vmovdqa 320(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 320(%rdi)
vmovdqa 352(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 352(%rdi)
vmovdqa 384(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 384(%rdi)
vmovdqa 416(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 416(%rdi)
vmovdqa 448(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 448(%rdi)
vmovdqa 480(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 480(%rdi)
vmovdqa 512(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 512(%rdi)
vmovdqa 544(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 544(%rdi)
vmovdqa 576(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 576(%rdi)
vmovdqa 608(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 608(%rdi)
vmovdqa 640(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 640(%rdi)
vmovdqa 672(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 672(%rdi)
vmovdqa 704(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 704(%rdi)
vmovdqa 736(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 736(%rdi)
vmovdqa 768(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 768(%rdi)
vmovdqa 800(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 800(%rdi)
vmovdqa 832(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 832(%rdi)
vmovdqa 864(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 864(%rdi)
vmovdqa 896(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 896(%rdi)
vmovdqa 928(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 928(%rdi)
vmovdqa 960(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 960(%rdi)
vmovdqa 992(%rsi), %ymm0
vpand %ymm6, %ymm0, %ymm0
vpsrlw $10, %ymm0, %ymm1
vpaddw %ymm0, %ymm1, %ymm0
vpaddw %ymm0, %ymm5, %ymm0
vpsrlw $8, %ymm0, %ymm1
vpand mask_ff(%rip), %ymm0, %ymm0
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_f(%rip), %ymm1, %ymm0
vpsrlw $4, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpand mask_3(%rip), %ymm1, %ymm0
vpsrlw $2, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm1
vpsubw mask_3(%rip), %ymm1, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm0
vpand %ymm15, %ymm1, %ymm14
vpxor %ymm14, %ymm0, %ymm1
vmovdqa %ymm1, 992(%rdi)
ret
