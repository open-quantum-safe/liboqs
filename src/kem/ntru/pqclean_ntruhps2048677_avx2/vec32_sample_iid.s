.data
.p2align 5
cast8_to_16:
.byte 255
.byte 0
.byte 255
.byte 1
.byte 255
.byte 2
.byte 255
.byte 3
.byte 255
.byte 4
.byte 255
.byte 5
.byte 255
.byte 6
.byte 255
.byte 7
.byte 255
.byte 0
.byte 255
.byte 1
.byte 255
.byte 2
.byte 255
.byte 3
.byte 255
.byte 4
.byte 255
.byte 5
.byte 255
.byte 6
.byte 255
.byte 7
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
.global PQCLEAN_NTRUHPS2048677_AVX2_vec32_sample_iid
.global _PQCLEAN_NTRUHPS2048677_AVX2_vec32_sample_iid
PQCLEAN_NTRUHPS2048677_AVX2_vec32_sample_iid:
_PQCLEAN_NTRUHPS2048677_AVX2_vec32_sample_iid:
vmovdqa 0(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa 32(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa 64(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa 96(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa 128(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa 160(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa 192(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa 224(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa 256(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa 288(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa 320(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa 352(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa 384(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa 416(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa 448(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa 480(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa 512(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa %ymm2, 1024(%rdi)
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa %ymm2, 1056(%rdi)
vmovdqa 544(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa %ymm2, 1088(%rdi)
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa %ymm2, 1120(%rdi)
vmovdqa 576(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa %ymm2, 1152(%rdi)
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa %ymm2, 1184(%rdi)
vmovdqa 608(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa %ymm2, 1216(%rdi)
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa %ymm2, 1248(%rdi)
vmovdqa 640(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa %ymm2, 1280(%rdi)
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa %ymm2, 1312(%rdi)
vmovdqa 672(%rsi), %ymm3
vextracti128 $0, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa %ymm2, 1344(%rdi)
vextracti128 $1, %ymm3, %xmm1
vpermq $216, %ymm1, %ymm1
vpshufb cast8_to_16(%rip), %ymm1, %ymm1
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
vmovdqa %ymm2, 1376(%rdi)
movw $0, 1352(%rdi)
movw $0, 1354(%rdi)
movw $0, 1356(%rdi)
movw $0, 1358(%rdi)
movw $0, 1360(%rdi)
movw $0, 1362(%rdi)
movw $0, 1364(%rdi)
movw $0, 1366(%rdi)
movw $0, 1368(%rdi)
movw $0, 1370(%rdi)
movw $0, 1372(%rdi)
movw $0, 1374(%rdi)
movw $0, 1376(%rdi)
movw $0, 1378(%rdi)
movw $0, 1380(%rdi)
movw $0, 1382(%rdi)
movw $0, 1384(%rdi)
movw $0, 1386(%rdi)
movw $0, 1388(%rdi)
movw $0, 1390(%rdi)
movw $0, 1392(%rdi)
movw $0, 1394(%rdi)
movw $0, 1396(%rdi)
movw $0, 1398(%rdi)
movw $0, 1400(%rdi)
movw $0, 1402(%rdi)
movw $0, 1404(%rdi)
movw $0, 1406(%rdi)
ret
