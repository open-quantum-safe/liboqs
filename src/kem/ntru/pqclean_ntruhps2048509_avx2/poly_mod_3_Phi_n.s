.data
.p2align 5
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
.global PQCLEAN_NTRUHPS2048509_AVX2_poly_mod_3_Phi_n
.global _PQCLEAN_NTRUHPS2048509_AVX2_poly_mod_3_Phi_n
PQCLEAN_NTRUHPS2048509_AVX2_poly_mod_3_Phi_n:
_PQCLEAN_NTRUHPS2048509_AVX2_poly_mod_3_Phi_n:
vmovdqa 992(%rdi), %ymm0
vpermq $3, %ymm0, %ymm0
vpslld $17, %ymm0, %ymm0
vpsrld $16, %ymm0, %ymm1
vpor %ymm0, %ymm1, %ymm0
vbroadcastss %xmm0, %ymm0
vpaddw 0(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 0(%rdi)
vpaddw 32(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 32(%rdi)
vpaddw 64(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 64(%rdi)
vpaddw 96(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 96(%rdi)
vpaddw 128(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 128(%rdi)
vpaddw 160(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 160(%rdi)
vpaddw 192(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 192(%rdi)
vpaddw 224(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 224(%rdi)
vpaddw 256(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 256(%rdi)
vpaddw 288(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 288(%rdi)
vpaddw 320(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 320(%rdi)
vpaddw 352(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 352(%rdi)
vpaddw 384(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 384(%rdi)
vpaddw 416(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 416(%rdi)
vpaddw 448(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 448(%rdi)
vpaddw 480(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 480(%rdi)
vpaddw 512(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 512(%rdi)
vpaddw 544(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 544(%rdi)
vpaddw 576(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 576(%rdi)
vpaddw 608(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 608(%rdi)
vpaddw 640(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 640(%rdi)
vpaddw 672(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 672(%rdi)
vpaddw 704(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 704(%rdi)
vpaddw 736(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 736(%rdi)
vpaddw 768(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 768(%rdi)
vpaddw 800(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 800(%rdi)
vpaddw 832(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 832(%rdi)
vpaddw 864(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 864(%rdi)
vpaddw 896(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 896(%rdi)
vpaddw 928(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 928(%rdi)
vpaddw 960(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 960(%rdi)
vpaddw 992(%rdi), %ymm0, %ymm1
vpsrlw $8, %ymm1, %ymm2
vpand mask_ff(%rip), %ymm1, %ymm1
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_f(%rip), %ymm2, %ymm1
vpsrlw $4, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpand mask_3(%rip), %ymm2, %ymm1
vpsrlw $2, %ymm2, %ymm2
vpaddw %ymm2, %ymm1, %ymm2
vpsubw mask_3(%rip), %ymm2, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm1
vpand %ymm15, %ymm2, %ymm14
vpxor %ymm14, %ymm1, %ymm2
vmovdqa %ymm2, 992(%rdi)
movw $0, 1018(%rdi)
movw $0, 1020(%rdi)
movw $0, 1022(%rdi)
ret
