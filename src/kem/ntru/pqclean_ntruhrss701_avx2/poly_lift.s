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
coeff_0:
.word 0xFFFF
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
.word 0
.word 0
.word 0
coeff_1:
.word 0
.word 0xFFFF
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
.word 0
.word 0
coeff_2:
.word 0
.word 0
.word 0xFFFF
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
.word 0
mask100:
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
mask010:
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
mask001:
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
mask100_701:
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0
mask010_701:
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0
.word 0
.word 0
mask001_701:
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0xFFFF
.word 0
.word 0
.word 0
.word 0
shuf_128_to_64:
.byte 8
.byte 9
.byte 10
.byte 11
.byte 12
.byte 13
.byte 14
.byte 15
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
const_modq:
.word 8191
.word 8191
.word 8191
.word 8191
.word 8191
.word 8191
.word 8191
.word 8191
.word 8191
.word 8191
.word 8191
.word 8191
.word 8191
.word 8191
.word 8191
.word 8191
mask_n:
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0
.word 0
.word 0
mask_omit_lowest:
.word 0
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
.word 0xFFFF
shuf_5_to_0_zerorest:
.byte 10
.byte 11
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.byte 255
.text
.global PQCLEAN_NTRUHRSS701_AVX2_poly_lift
.global _PQCLEAN_NTRUHRSS701_AVX2_poly_lift
PQCLEAN_NTRUHRSS701_AVX2_poly_lift:
_PQCLEAN_NTRUHRSS701_AVX2_poly_lift:
mov %rsp, %r8
andq $-32, %rsp
subq $1408, %rsp
vmovdqa 0(%rsi), %ymm1
vpxor %ymm0, %ymm0, %ymm0
vpand coeff_0(%rip), %ymm1, %ymm5
vpsubw %ymm5, %ymm0, %ymm2
vpsubw %ymm5, %ymm0, %ymm3
vpsubw %ymm5, %ymm0, %ymm4
vpand coeff_1(%rip), %ymm1, %ymm5
vpsubw %ymm5, %ymm3, %ymm3
vpsubw %ymm5, %ymm4, %ymm4
vpand coeff_2(%rip), %ymm1, %ymm5
vpsubw %ymm5, %ymm4, %ymm4
vmovdqa 0(%rsi), %ymm1
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 32(%rsi), %ymm1
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 64(%rsi), %ymm1
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 96(%rsi), %ymm1
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 128(%rsi), %ymm1
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 160(%rsi), %ymm1
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 192(%rsi), %ymm1
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 224(%rsi), %ymm1
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 256(%rsi), %ymm1
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 288(%rsi), %ymm1
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 320(%rsi), %ymm1
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 352(%rsi), %ymm1
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 384(%rsi), %ymm1
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 416(%rsi), %ymm1
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 448(%rsi), %ymm1
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 480(%rsi), %ymm1
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 512(%rsi), %ymm1
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 544(%rsi), %ymm1
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 576(%rsi), %ymm1
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 608(%rsi), %ymm1
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 640(%rsi), %ymm1
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 672(%rsi), %ymm1
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 704(%rsi), %ymm1
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 736(%rsi), %ymm1
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 768(%rsi), %ymm1
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 800(%rsi), %ymm1
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 832(%rsi), %ymm1
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 864(%rsi), %ymm1
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 896(%rsi), %ymm1
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 928(%rsi), %ymm1
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 960(%rsi), %ymm1
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 992(%rsi), %ymm1
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 1024(%rsi), %ymm1
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 1056(%rsi), %ymm1
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 1088(%rsi), %ymm1
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 1120(%rsi), %ymm1
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 1152(%rsi), %ymm1
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 1184(%rsi), %ymm1
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 1216(%rsi), %ymm1
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 1248(%rsi), %ymm1
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 1280(%rsi), %ymm1
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 1312(%rsi), %ymm1
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 1344(%rsi), %ymm1
vpand mask001(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask100(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask010(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask001(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa 1376(%rsi), %ymm1
vpand mask010_701(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001_701(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand mask001_701(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100_701(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand mask100_701(%rip), %ymm1, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand mask010_701(%rip), %ymm1, %ymm5
vpsllw $1, %ymm5, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vextracti128 $1, %ymm2, %xmm5
vpaddw %ymm5, %ymm2, %ymm2
vpshufb shuf_128_to_64(%rip), %ymm2, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpsrlq $32, %ymm2, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpsrlq $16, %ymm2, %ymm5
vpaddw %ymm5, %ymm2, %ymm2
vpand coeff_0(%rip), %ymm2, %ymm2
vextracti128 $1, %ymm3, %xmm5
vpaddw %ymm5, %ymm3, %ymm3
vpshufb shuf_128_to_64(%rip), %ymm3, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpsrlq $32, %ymm3, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpsllq $16, %ymm3, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vpand coeff_1(%rip), %ymm3, %ymm3
vextracti128 $1, %ymm4, %xmm5
vpaddw %ymm5, %ymm4, %ymm4
vpshufb shuf_128_to_64(%rip), %ymm4, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpsllq $32, %ymm4, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpsrlq $16, %ymm4, %ymm5
vpaddw %ymm5, %ymm4, %ymm4
vpand coeff_2(%rip), %ymm4, %ymm4
vpor %ymm2, %ymm3, %ymm5
vpor %ymm5, %ymm4, %ymm5
vmovdqa %ymm5, 0(%rsp)
vmovdqu 6(%rsi), %ymm1
vmovdqu 4(%rsi), %ymm4
vmovdqu 2(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 6(%rsp)
vmovdqu 38(%rsi), %ymm1
vmovdqu 36(%rsi), %ymm4
vmovdqu 34(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 38(%rsp)
vmovdqu 70(%rsi), %ymm1
vmovdqu 68(%rsi), %ymm4
vmovdqu 66(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 70(%rsp)
vmovdqu 102(%rsi), %ymm1
vmovdqu 100(%rsi), %ymm4
vmovdqu 98(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 102(%rsp)
vmovdqu 134(%rsi), %ymm1
vmovdqu 132(%rsi), %ymm4
vmovdqu 130(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 134(%rsp)
vmovdqu 166(%rsi), %ymm1
vmovdqu 164(%rsi), %ymm4
vmovdqu 162(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 166(%rsp)
vmovdqu 198(%rsi), %ymm1
vmovdqu 196(%rsi), %ymm4
vmovdqu 194(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 198(%rsp)
vmovdqu 230(%rsi), %ymm1
vmovdqu 228(%rsi), %ymm4
vmovdqu 226(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 230(%rsp)
vmovdqu 262(%rsi), %ymm1
vmovdqu 260(%rsi), %ymm4
vmovdqu 258(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 262(%rsp)
vmovdqu 294(%rsi), %ymm1
vmovdqu 292(%rsi), %ymm4
vmovdqu 290(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 294(%rsp)
vmovdqu 326(%rsi), %ymm1
vmovdqu 324(%rsi), %ymm4
vmovdqu 322(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 326(%rsp)
vmovdqu 358(%rsi), %ymm1
vmovdqu 356(%rsi), %ymm4
vmovdqu 354(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 358(%rsp)
vmovdqu 390(%rsi), %ymm1
vmovdqu 388(%rsi), %ymm4
vmovdqu 386(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 390(%rsp)
vmovdqu 422(%rsi), %ymm1
vmovdqu 420(%rsi), %ymm4
vmovdqu 418(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 422(%rsp)
vmovdqu 454(%rsi), %ymm1
vmovdqu 452(%rsi), %ymm4
vmovdqu 450(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 454(%rsp)
vmovdqu 486(%rsi), %ymm1
vmovdqu 484(%rsi), %ymm4
vmovdqu 482(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 486(%rsp)
vmovdqu 518(%rsi), %ymm1
vmovdqu 516(%rsi), %ymm4
vmovdqu 514(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 518(%rsp)
vmovdqu 550(%rsi), %ymm1
vmovdqu 548(%rsi), %ymm4
vmovdqu 546(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 550(%rsp)
vmovdqu 582(%rsi), %ymm1
vmovdqu 580(%rsi), %ymm4
vmovdqu 578(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 582(%rsp)
vmovdqu 614(%rsi), %ymm1
vmovdqu 612(%rsi), %ymm4
vmovdqu 610(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 614(%rsp)
vmovdqu 646(%rsi), %ymm1
vmovdqu 644(%rsi), %ymm4
vmovdqu 642(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 646(%rsp)
vmovdqu 678(%rsi), %ymm1
vmovdqu 676(%rsi), %ymm4
vmovdqu 674(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 678(%rsp)
vmovdqu 710(%rsi), %ymm1
vmovdqu 708(%rsi), %ymm4
vmovdqu 706(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 710(%rsp)
vmovdqu 742(%rsi), %ymm1
vmovdqu 740(%rsi), %ymm4
vmovdqu 738(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 742(%rsp)
vmovdqu 774(%rsi), %ymm1
vmovdqu 772(%rsi), %ymm4
vmovdqu 770(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 774(%rsp)
vmovdqu 806(%rsi), %ymm1
vmovdqu 804(%rsi), %ymm4
vmovdqu 802(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 806(%rsp)
vmovdqu 838(%rsi), %ymm1
vmovdqu 836(%rsi), %ymm4
vmovdqu 834(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 838(%rsp)
vmovdqu 870(%rsi), %ymm1
vmovdqu 868(%rsi), %ymm4
vmovdqu 866(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 870(%rsp)
vmovdqu 902(%rsi), %ymm1
vmovdqu 900(%rsi), %ymm4
vmovdqu 898(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 902(%rsp)
vmovdqu 934(%rsi), %ymm1
vmovdqu 932(%rsi), %ymm4
vmovdqu 930(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 934(%rsp)
vmovdqu 966(%rsi), %ymm1
vmovdqu 964(%rsi), %ymm4
vmovdqu 962(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 966(%rsp)
vmovdqu 998(%rsi), %ymm1
vmovdqu 996(%rsi), %ymm4
vmovdqu 994(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 998(%rsp)
vmovdqu 1030(%rsi), %ymm1
vmovdqu 1028(%rsi), %ymm4
vmovdqu 1026(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 1030(%rsp)
vmovdqu 1062(%rsi), %ymm1
vmovdqu 1060(%rsi), %ymm4
vmovdqu 1058(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 1062(%rsp)
vmovdqu 1094(%rsi), %ymm1
vmovdqu 1092(%rsi), %ymm4
vmovdqu 1090(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 1094(%rsp)
vmovdqu 1126(%rsi), %ymm1
vmovdqu 1124(%rsi), %ymm4
vmovdqu 1122(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 1126(%rsp)
vmovdqu 1158(%rsi), %ymm1
vmovdqu 1156(%rsi), %ymm4
vmovdqu 1154(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 1158(%rsp)
vmovdqu 1190(%rsi), %ymm1
vmovdqu 1188(%rsi), %ymm4
vmovdqu 1186(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 1190(%rsp)
vmovdqu 1222(%rsi), %ymm1
vmovdqu 1220(%rsi), %ymm4
vmovdqu 1218(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 1222(%rsp)
vmovdqu 1254(%rsi), %ymm1
vmovdqu 1252(%rsi), %ymm4
vmovdqu 1250(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 1254(%rsp)
vmovdqu 1286(%rsi), %ymm1
vmovdqu 1284(%rsi), %ymm4
vmovdqu 1282(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 1286(%rsp)
vmovdqu 1318(%rsi), %ymm1
vmovdqu 1316(%rsi), %ymm4
vmovdqu 1314(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 1318(%rsp)
vmovdqu 1350(%rsi), %ymm1
vmovdqu 1348(%rsi), %ymm4
vmovdqu 1346(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqu %ymm2, 1350(%rsp)
vmovdqa 1376(%rsi), %ymm1
vpand mask_n(%rip), %ymm1, %ymm1
vmovdqu 1374(%rsi), %ymm4
vmovdqu 1372(%rsi), %ymm5
vpaddw %ymm4, %ymm1, %ymm1
vpaddw %ymm5, %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm2
vmovdqa %ymm2, 1376(%rsp)
movq 0(%rsp), %r9
movq 6(%rsp), %r10
add %r9, %r10
movq 12(%rsp), %r11
movq %r10, 6(%rsp)
add %r10, %r11
movq 18(%rsp), %r9
movq %r11, 12(%rsp)
add %r11, %r9
movq 24(%rsp), %r10
movq %r9, 18(%rsp)
add %r9, %r10
movq 30(%rsp), %r11
movq %r10, 24(%rsp)
add %r10, %r11
movq 36(%rsp), %r9
movq %r11, 30(%rsp)
add %r11, %r9
movq 42(%rsp), %r10
movq %r9, 36(%rsp)
add %r9, %r10
movq 48(%rsp), %r11
movq %r10, 42(%rsp)
add %r10, %r11
movq 54(%rsp), %r9
movq %r11, 48(%rsp)
add %r11, %r9
movq 60(%rsp), %r10
movq %r9, 54(%rsp)
add %r9, %r10
movq 66(%rsp), %r11
movq %r10, 60(%rsp)
add %r10, %r11
movq 72(%rsp), %r9
movq %r11, 66(%rsp)
add %r11, %r9
movq 78(%rsp), %r10
movq %r9, 72(%rsp)
add %r9, %r10
movq 84(%rsp), %r11
movq %r10, 78(%rsp)
add %r10, %r11
movq 90(%rsp), %r9
movq %r11, 84(%rsp)
add %r11, %r9
movq 96(%rsp), %r10
movq %r9, 90(%rsp)
add %r9, %r10
movq 102(%rsp), %r11
movq %r10, 96(%rsp)
add %r10, %r11
movq 108(%rsp), %r9
movq %r11, 102(%rsp)
add %r11, %r9
movq 114(%rsp), %r10
movq %r9, 108(%rsp)
add %r9, %r10
movq 120(%rsp), %r11
movq %r10, 114(%rsp)
add %r10, %r11
movq 126(%rsp), %r9
movq %r11, 120(%rsp)
add %r11, %r9
movq 132(%rsp), %r10
movq %r9, 126(%rsp)
add %r9, %r10
movq 138(%rsp), %r11
movq %r10, 132(%rsp)
add %r10, %r11
movq 144(%rsp), %r9
movq %r11, 138(%rsp)
add %r11, %r9
movq 150(%rsp), %r10
movq %r9, 144(%rsp)
add %r9, %r10
movq 156(%rsp), %r11
movq %r10, 150(%rsp)
add %r10, %r11
movq 162(%rsp), %r9
movq %r11, 156(%rsp)
add %r11, %r9
movq 168(%rsp), %r10
movq %r9, 162(%rsp)
add %r9, %r10
movq 174(%rsp), %r11
movq %r10, 168(%rsp)
add %r10, %r11
movq 180(%rsp), %r9
movq %r11, 174(%rsp)
add %r11, %r9
movq 186(%rsp), %r10
movq %r9, 180(%rsp)
add %r9, %r10
movq 192(%rsp), %r11
movq %r10, 186(%rsp)
add %r10, %r11
movq 198(%rsp), %r9
movq %r11, 192(%rsp)
add %r11, %r9
movq 204(%rsp), %r10
movq %r9, 198(%rsp)
add %r9, %r10
movq 210(%rsp), %r11
movq %r10, 204(%rsp)
add %r10, %r11
movq 216(%rsp), %r9
movq %r11, 210(%rsp)
add %r11, %r9
movq 222(%rsp), %r10
movq %r9, 216(%rsp)
add %r9, %r10
movq 228(%rsp), %r11
movq %r10, 222(%rsp)
add %r10, %r11
movq 234(%rsp), %r9
movq %r11, 228(%rsp)
add %r11, %r9
movq 240(%rsp), %r10
movq %r9, 234(%rsp)
add %r9, %r10
movq 246(%rsp), %r11
movq %r10, 240(%rsp)
add %r10, %r11
movq 252(%rsp), %r9
movq %r11, 246(%rsp)
add %r11, %r9
movq 258(%rsp), %r10
movq %r9, 252(%rsp)
add %r9, %r10
movq 264(%rsp), %r11
movq %r10, 258(%rsp)
add %r10, %r11
movq 270(%rsp), %r9
movq %r11, 264(%rsp)
add %r11, %r9
movq 276(%rsp), %r10
movq %r9, 270(%rsp)
add %r9, %r10
movq 282(%rsp), %r11
movq %r10, 276(%rsp)
add %r10, %r11
movq 288(%rsp), %r9
movq %r11, 282(%rsp)
add %r11, %r9
movq 294(%rsp), %r10
movq %r9, 288(%rsp)
add %r9, %r10
movq 300(%rsp), %r11
movq %r10, 294(%rsp)
add %r10, %r11
movq 306(%rsp), %r9
movq %r11, 300(%rsp)
add %r11, %r9
movq 312(%rsp), %r10
movq %r9, 306(%rsp)
add %r9, %r10
movq 318(%rsp), %r11
movq %r10, 312(%rsp)
add %r10, %r11
movq 324(%rsp), %r9
movq %r11, 318(%rsp)
add %r11, %r9
movq 330(%rsp), %r10
movq %r9, 324(%rsp)
add %r9, %r10
movq 336(%rsp), %r11
movq %r10, 330(%rsp)
add %r10, %r11
movq 342(%rsp), %r9
movq %r11, 336(%rsp)
add %r11, %r9
movq 348(%rsp), %r10
movq %r9, 342(%rsp)
add %r9, %r10
movq 354(%rsp), %r11
movq %r10, 348(%rsp)
add %r10, %r11
movq 360(%rsp), %r9
movq %r11, 354(%rsp)
add %r11, %r9
movq 366(%rsp), %r10
movq %r9, 360(%rsp)
add %r9, %r10
movq 372(%rsp), %r11
movq %r10, 366(%rsp)
add %r10, %r11
movq 378(%rsp), %r9
movq %r11, 372(%rsp)
add %r11, %r9
movq 384(%rsp), %r10
movq %r9, 378(%rsp)
add %r9, %r10
movq 390(%rsp), %r11
movq %r10, 384(%rsp)
add %r10, %r11
movq 396(%rsp), %r9
movq %r11, 390(%rsp)
add %r11, %r9
movq 402(%rsp), %r10
movq %r9, 396(%rsp)
add %r9, %r10
movq 408(%rsp), %r11
movq %r10, 402(%rsp)
add %r10, %r11
movq 414(%rsp), %r9
movq %r11, 408(%rsp)
add %r11, %r9
movq 420(%rsp), %r10
movq %r9, 414(%rsp)
add %r9, %r10
movq 426(%rsp), %r11
movq %r10, 420(%rsp)
add %r10, %r11
movq 432(%rsp), %r9
movq %r11, 426(%rsp)
add %r11, %r9
movq 438(%rsp), %r10
movq %r9, 432(%rsp)
add %r9, %r10
movq 444(%rsp), %r11
movq %r10, 438(%rsp)
add %r10, %r11
movq 450(%rsp), %r9
movq %r11, 444(%rsp)
add %r11, %r9
movq 456(%rsp), %r10
movq %r9, 450(%rsp)
add %r9, %r10
movq 462(%rsp), %r11
movq %r10, 456(%rsp)
add %r10, %r11
movq 468(%rsp), %r9
movq %r11, 462(%rsp)
add %r11, %r9
movq 474(%rsp), %r10
movq %r9, 468(%rsp)
add %r9, %r10
movq 480(%rsp), %r11
movq %r10, 474(%rsp)
add %r10, %r11
movq 486(%rsp), %r9
movq %r11, 480(%rsp)
add %r11, %r9
movq 492(%rsp), %r10
movq %r9, 486(%rsp)
add %r9, %r10
movq 498(%rsp), %r11
movq %r10, 492(%rsp)
add %r10, %r11
movq 504(%rsp), %r9
movq %r11, 498(%rsp)
add %r11, %r9
movq 510(%rsp), %r10
movq %r9, 504(%rsp)
add %r9, %r10
movq 516(%rsp), %r11
movq %r10, 510(%rsp)
add %r10, %r11
movq 522(%rsp), %r9
movq %r11, 516(%rsp)
add %r11, %r9
movq 528(%rsp), %r10
movq %r9, 522(%rsp)
add %r9, %r10
movq 534(%rsp), %r11
movq %r10, 528(%rsp)
add %r10, %r11
movq 540(%rsp), %r9
movq %r11, 534(%rsp)
add %r11, %r9
movq 546(%rsp), %r10
movq %r9, 540(%rsp)
add %r9, %r10
movq 552(%rsp), %r11
movq %r10, 546(%rsp)
add %r10, %r11
movq 558(%rsp), %r9
movq %r11, 552(%rsp)
add %r11, %r9
movq 564(%rsp), %r10
movq %r9, 558(%rsp)
add %r9, %r10
movq 570(%rsp), %r11
movq %r10, 564(%rsp)
add %r10, %r11
movq 576(%rsp), %r9
movq %r11, 570(%rsp)
add %r11, %r9
movq 582(%rsp), %r10
movq %r9, 576(%rsp)
add %r9, %r10
movq 588(%rsp), %r11
movq %r10, 582(%rsp)
add %r10, %r11
movq 594(%rsp), %r9
movq %r11, 588(%rsp)
add %r11, %r9
movq 600(%rsp), %r10
movq %r9, 594(%rsp)
add %r9, %r10
movq 606(%rsp), %r11
movq %r10, 600(%rsp)
add %r10, %r11
movq 612(%rsp), %r9
movq %r11, 606(%rsp)
add %r11, %r9
movq 618(%rsp), %r10
movq %r9, 612(%rsp)
add %r9, %r10
movq 624(%rsp), %r11
movq %r10, 618(%rsp)
add %r10, %r11
movq 630(%rsp), %r9
movq %r11, 624(%rsp)
add %r11, %r9
movq 636(%rsp), %r10
movq %r9, 630(%rsp)
add %r9, %r10
movq 642(%rsp), %r11
movq %r10, 636(%rsp)
add %r10, %r11
movq 648(%rsp), %r9
movq %r11, 642(%rsp)
add %r11, %r9
movq 654(%rsp), %r10
movq %r9, 648(%rsp)
add %r9, %r10
movq 660(%rsp), %r11
movq %r10, 654(%rsp)
add %r10, %r11
movq 666(%rsp), %r9
movq %r11, 660(%rsp)
add %r11, %r9
movq 672(%rsp), %r10
movq %r9, 666(%rsp)
add %r9, %r10
movq 678(%rsp), %r11
movq %r10, 672(%rsp)
add %r10, %r11
movq 684(%rsp), %r9
movq %r11, 678(%rsp)
add %r11, %r9
movq 690(%rsp), %r10
movq %r9, 684(%rsp)
add %r9, %r10
movq 696(%rsp), %r11
movq %r10, 690(%rsp)
add %r10, %r11
movq 702(%rsp), %r9
movq %r11, 696(%rsp)
add %r11, %r9
movq 708(%rsp), %r10
movq %r9, 702(%rsp)
add %r9, %r10
movq 714(%rsp), %r11
movq %r10, 708(%rsp)
add %r10, %r11
movq 720(%rsp), %r9
movq %r11, 714(%rsp)
add %r11, %r9
movq 726(%rsp), %r10
movq %r9, 720(%rsp)
add %r9, %r10
movq 732(%rsp), %r11
movq %r10, 726(%rsp)
add %r10, %r11
movq 738(%rsp), %r9
movq %r11, 732(%rsp)
add %r11, %r9
movq 744(%rsp), %r10
movq %r9, 738(%rsp)
add %r9, %r10
movq 750(%rsp), %r11
movq %r10, 744(%rsp)
add %r10, %r11
movq 756(%rsp), %r9
movq %r11, 750(%rsp)
add %r11, %r9
movq 762(%rsp), %r10
movq %r9, 756(%rsp)
add %r9, %r10
movq 768(%rsp), %r11
movq %r10, 762(%rsp)
add %r10, %r11
movq 774(%rsp), %r9
movq %r11, 768(%rsp)
add %r11, %r9
movq 780(%rsp), %r10
movq %r9, 774(%rsp)
add %r9, %r10
movq 786(%rsp), %r11
movq %r10, 780(%rsp)
add %r10, %r11
movq 792(%rsp), %r9
movq %r11, 786(%rsp)
add %r11, %r9
movq 798(%rsp), %r10
movq %r9, 792(%rsp)
add %r9, %r10
movq 804(%rsp), %r11
movq %r10, 798(%rsp)
add %r10, %r11
movq 810(%rsp), %r9
movq %r11, 804(%rsp)
add %r11, %r9
movq 816(%rsp), %r10
movq %r9, 810(%rsp)
add %r9, %r10
movq 822(%rsp), %r11
movq %r10, 816(%rsp)
add %r10, %r11
movq 828(%rsp), %r9
movq %r11, 822(%rsp)
add %r11, %r9
movq 834(%rsp), %r10
movq %r9, 828(%rsp)
add %r9, %r10
movq 840(%rsp), %r11
movq %r10, 834(%rsp)
add %r10, %r11
movq 846(%rsp), %r9
movq %r11, 840(%rsp)
add %r11, %r9
movq 852(%rsp), %r10
movq %r9, 846(%rsp)
add %r9, %r10
movq 858(%rsp), %r11
movq %r10, 852(%rsp)
add %r10, %r11
movq 864(%rsp), %r9
movq %r11, 858(%rsp)
add %r11, %r9
movq 870(%rsp), %r10
movq %r9, 864(%rsp)
add %r9, %r10
movq 876(%rsp), %r11
movq %r10, 870(%rsp)
add %r10, %r11
movq 882(%rsp), %r9
movq %r11, 876(%rsp)
add %r11, %r9
movq 888(%rsp), %r10
movq %r9, 882(%rsp)
add %r9, %r10
movq 894(%rsp), %r11
movq %r10, 888(%rsp)
add %r10, %r11
movq 900(%rsp), %r9
movq %r11, 894(%rsp)
add %r11, %r9
movq 906(%rsp), %r10
movq %r9, 900(%rsp)
add %r9, %r10
movq 912(%rsp), %r11
movq %r10, 906(%rsp)
add %r10, %r11
movq 918(%rsp), %r9
movq %r11, 912(%rsp)
add %r11, %r9
movq 924(%rsp), %r10
movq %r9, 918(%rsp)
add %r9, %r10
movq 930(%rsp), %r11
movq %r10, 924(%rsp)
add %r10, %r11
movq 936(%rsp), %r9
movq %r11, 930(%rsp)
add %r11, %r9
movq 942(%rsp), %r10
movq %r9, 936(%rsp)
add %r9, %r10
movq 948(%rsp), %r11
movq %r10, 942(%rsp)
add %r10, %r11
movq 954(%rsp), %r9
movq %r11, 948(%rsp)
add %r11, %r9
movq 960(%rsp), %r10
movq %r9, 954(%rsp)
add %r9, %r10
movq 966(%rsp), %r11
movq %r10, 960(%rsp)
add %r10, %r11
movq 972(%rsp), %r9
movq %r11, 966(%rsp)
add %r11, %r9
movq 978(%rsp), %r10
movq %r9, 972(%rsp)
add %r9, %r10
movq 984(%rsp), %r11
movq %r10, 978(%rsp)
add %r10, %r11
movq 990(%rsp), %r9
movq %r11, 984(%rsp)
add %r11, %r9
movq 996(%rsp), %r10
movq %r9, 990(%rsp)
add %r9, %r10
movq 1002(%rsp), %r11
movq %r10, 996(%rsp)
add %r10, %r11
movq 1008(%rsp), %r9
movq %r11, 1002(%rsp)
add %r11, %r9
movq 1014(%rsp), %r10
movq %r9, 1008(%rsp)
add %r9, %r10
movq 1020(%rsp), %r11
movq %r10, 1014(%rsp)
add %r10, %r11
movq 1026(%rsp), %r9
movq %r11, 1020(%rsp)
add %r11, %r9
movq 1032(%rsp), %r10
movq %r9, 1026(%rsp)
add %r9, %r10
movq 1038(%rsp), %r11
movq %r10, 1032(%rsp)
add %r10, %r11
movq 1044(%rsp), %r9
movq %r11, 1038(%rsp)
add %r11, %r9
movq 1050(%rsp), %r10
movq %r9, 1044(%rsp)
add %r9, %r10
movq 1056(%rsp), %r11
movq %r10, 1050(%rsp)
add %r10, %r11
movq 1062(%rsp), %r9
movq %r11, 1056(%rsp)
add %r11, %r9
movq 1068(%rsp), %r10
movq %r9, 1062(%rsp)
add %r9, %r10
movq 1074(%rsp), %r11
movq %r10, 1068(%rsp)
add %r10, %r11
movq 1080(%rsp), %r9
movq %r11, 1074(%rsp)
add %r11, %r9
movq 1086(%rsp), %r10
movq %r9, 1080(%rsp)
add %r9, %r10
movq 1092(%rsp), %r11
movq %r10, 1086(%rsp)
add %r10, %r11
movq 1098(%rsp), %r9
movq %r11, 1092(%rsp)
add %r11, %r9
movq 1104(%rsp), %r10
movq %r9, 1098(%rsp)
add %r9, %r10
movq 1110(%rsp), %r11
movq %r10, 1104(%rsp)
add %r10, %r11
movq 1116(%rsp), %r9
movq %r11, 1110(%rsp)
add %r11, %r9
movq 1122(%rsp), %r10
movq %r9, 1116(%rsp)
add %r9, %r10
movq 1128(%rsp), %r11
movq %r10, 1122(%rsp)
add %r10, %r11
movq 1134(%rsp), %r9
movq %r11, 1128(%rsp)
add %r11, %r9
movq 1140(%rsp), %r10
movq %r9, 1134(%rsp)
add %r9, %r10
movq 1146(%rsp), %r11
movq %r10, 1140(%rsp)
add %r10, %r11
movq 1152(%rsp), %r9
movq %r11, 1146(%rsp)
add %r11, %r9
movq 1158(%rsp), %r10
movq %r9, 1152(%rsp)
add %r9, %r10
movq 1164(%rsp), %r11
movq %r10, 1158(%rsp)
add %r10, %r11
movq 1170(%rsp), %r9
movq %r11, 1164(%rsp)
add %r11, %r9
movq 1176(%rsp), %r10
movq %r9, 1170(%rsp)
add %r9, %r10
movq 1182(%rsp), %r11
movq %r10, 1176(%rsp)
add %r10, %r11
movq 1188(%rsp), %r9
movq %r11, 1182(%rsp)
add %r11, %r9
movq 1194(%rsp), %r10
movq %r9, 1188(%rsp)
add %r9, %r10
movq 1200(%rsp), %r11
movq %r10, 1194(%rsp)
add %r10, %r11
movq 1206(%rsp), %r9
movq %r11, 1200(%rsp)
add %r11, %r9
movq 1212(%rsp), %r10
movq %r9, 1206(%rsp)
add %r9, %r10
movq 1218(%rsp), %r11
movq %r10, 1212(%rsp)
add %r10, %r11
movq 1224(%rsp), %r9
movq %r11, 1218(%rsp)
add %r11, %r9
movq 1230(%rsp), %r10
movq %r9, 1224(%rsp)
add %r9, %r10
movq 1236(%rsp), %r11
movq %r10, 1230(%rsp)
add %r10, %r11
movq 1242(%rsp), %r9
movq %r11, 1236(%rsp)
add %r11, %r9
movq 1248(%rsp), %r10
movq %r9, 1242(%rsp)
add %r9, %r10
movq 1254(%rsp), %r11
movq %r10, 1248(%rsp)
add %r10, %r11
movq 1260(%rsp), %r9
movq %r11, 1254(%rsp)
add %r11, %r9
movq 1266(%rsp), %r10
movq %r9, 1260(%rsp)
add %r9, %r10
movq 1272(%rsp), %r11
movq %r10, 1266(%rsp)
add %r10, %r11
movq 1278(%rsp), %r9
movq %r11, 1272(%rsp)
add %r11, %r9
movq 1284(%rsp), %r10
movq %r9, 1278(%rsp)
add %r9, %r10
movq 1290(%rsp), %r11
movq %r10, 1284(%rsp)
add %r10, %r11
movq 1296(%rsp), %r9
movq %r11, 1290(%rsp)
add %r11, %r9
movq 1302(%rsp), %r10
movq %r9, 1296(%rsp)
add %r9, %r10
movq 1308(%rsp), %r11
movq %r10, 1302(%rsp)
add %r10, %r11
movq 1314(%rsp), %r9
movq %r11, 1308(%rsp)
add %r11, %r9
movq 1320(%rsp), %r10
movq %r9, 1314(%rsp)
add %r9, %r10
movq 1326(%rsp), %r11
movq %r10, 1320(%rsp)
add %r10, %r11
movq 1332(%rsp), %r9
movq %r11, 1326(%rsp)
add %r11, %r9
movq 1338(%rsp), %r10
movq %r9, 1332(%rsp)
add %r9, %r10
movq 1344(%rsp), %r11
movq %r10, 1338(%rsp)
add %r10, %r11
movq 1350(%rsp), %r9
movq %r11, 1344(%rsp)
add %r11, %r9
movq 1356(%rsp), %r10
movq %r9, 1350(%rsp)
add %r9, %r10
movq 1362(%rsp), %r11
movq %r10, 1356(%rsp)
add %r10, %r11
movq 1368(%rsp), %r9
movq %r11, 1362(%rsp)
add %r11, %r9
movq 1374(%rsp), %r10
movq %r9, 1368(%rsp)
add %r9, %r10
movq 1380(%rsp), %r11
movq %r10, 1374(%rsp)
add %r10, %r11
movq 1386(%rsp), %r9
movq %r11, 1380(%rsp)
add %r11, %r9
movq 1392(%rsp), %r10
movq %r9, 1386(%rsp)
add %r9, %r10
movq 1398(%rsp), %r11
movq %r10, 1392(%rsp)
add %r10, %r11
movq %r11, 1398(%rsp)
vmovdqa 1376(%rsp), %ymm1
vpermq $3, %ymm1, %ymm1
vpslld $17, %ymm1, %ymm1
vpsrld $16, %ymm1, %ymm2
vpor %ymm1, %ymm2, %ymm1
vbroadcastss %xmm1, %ymm1
vpaddw 0(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 0(%rsp)
vpaddw 32(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 32(%rsp)
vpaddw 64(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 64(%rsp)
vpaddw 96(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 96(%rsp)
vpaddw 128(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 128(%rsp)
vpaddw 160(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 160(%rsp)
vpaddw 192(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 192(%rsp)
vpaddw 224(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 224(%rsp)
vpaddw 256(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 256(%rsp)
vpaddw 288(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 288(%rsp)
vpaddw 320(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 320(%rsp)
vpaddw 352(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 352(%rsp)
vpaddw 384(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 384(%rsp)
vpaddw 416(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 416(%rsp)
vpaddw 448(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 448(%rsp)
vpaddw 480(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 480(%rsp)
vpaddw 512(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 512(%rsp)
vpaddw 544(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 544(%rsp)
vpaddw 576(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 576(%rsp)
vpaddw 608(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 608(%rsp)
vpaddw 640(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 640(%rsp)
vpaddw 672(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 672(%rsp)
vpaddw 704(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 704(%rsp)
vpaddw 736(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 736(%rsp)
vpaddw 768(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 768(%rsp)
vpaddw 800(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 800(%rsp)
vpaddw 832(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 832(%rsp)
vpaddw 864(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 864(%rsp)
vpaddw 896(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 896(%rsp)
vpaddw 928(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 928(%rsp)
vpaddw 960(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 960(%rsp)
vpaddw 992(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 992(%rsp)
vpaddw 1024(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 1024(%rsp)
vpaddw 1056(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 1056(%rsp)
vpaddw 1088(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 1088(%rsp)
vpaddw 1120(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 1120(%rsp)
vpaddw 1152(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 1152(%rsp)
vpaddw 1184(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 1184(%rsp)
vpaddw 1216(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 1216(%rsp)
vpaddw 1248(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 1248(%rsp)
vpaddw 1280(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 1280(%rsp)
vpaddw 1312(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 1312(%rsp)
vpaddw 1344(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 1344(%rsp)
vpaddw 1376(%rsp), %ymm1, %ymm2
vpsrlw $8, %ymm2, %ymm3
vpand mask_ff(%rip), %ymm2, %ymm2
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_f(%rip), %ymm3, %ymm2
vpsrlw $4, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpand mask_3(%rip), %ymm3, %ymm2
vpsrlw $2, %ymm3, %ymm3
vpaddw %ymm3, %ymm2, %ymm3
vpsubw mask_3(%rip), %ymm3, %ymm14
vpsraw $15, %ymm14, %ymm15
vpandn %ymm14, %ymm15, %ymm2
vpand %ymm15, %ymm3, %ymm14
vpxor %ymm14, %ymm2, %ymm3
vpsrlq $1, %ymm3, %ymm2
vpsubw %ymm2, %ymm0, %ymm2
vpand const_modq(%rip), %ymm2, %ymm2
vpor %ymm3, %ymm2, %ymm3
vmovdqa %ymm3, 1376(%rsp)
vmovdqu 1374(%rsp), %ymm0
vpsubw 1376(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 1376(%rdi)
vextracti128 $1, %ymm0, %xmm4
vpshufb shuf_5_to_0_zerorest(%rip), %ymm4, %ymm4
vpsubw 0(%rsp), %ymm4, %ymm4
vpand coeff_0(%rip), %ymm4, %ymm4
vmovdqu 1342(%rsp), %ymm0
vpsubw 1344(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 1344(%rdi)
vmovdqu 1310(%rsp), %ymm0
vpsubw 1312(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 1312(%rdi)
vmovdqu 1278(%rsp), %ymm0
vpsubw 1280(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 1280(%rdi)
vmovdqu 1246(%rsp), %ymm0
vpsubw 1248(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 1248(%rdi)
vmovdqu 1214(%rsp), %ymm0
vpsubw 1216(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 1216(%rdi)
vmovdqu 1182(%rsp), %ymm0
vpsubw 1184(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 1184(%rdi)
vmovdqu 1150(%rsp), %ymm0
vpsubw 1152(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 1152(%rdi)
vmovdqu 1118(%rsp), %ymm0
vpsubw 1120(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 1120(%rdi)
vmovdqu 1086(%rsp), %ymm0
vpsubw 1088(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 1088(%rdi)
vmovdqu 1054(%rsp), %ymm0
vpsubw 1056(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 1056(%rdi)
vmovdqu 1022(%rsp), %ymm0
vpsubw 1024(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 1024(%rdi)
vmovdqu 990(%rsp), %ymm0
vpsubw 992(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 992(%rdi)
vmovdqu 958(%rsp), %ymm0
vpsubw 960(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 960(%rdi)
vmovdqu 926(%rsp), %ymm0
vpsubw 928(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 928(%rdi)
vmovdqu 894(%rsp), %ymm0
vpsubw 896(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 896(%rdi)
vmovdqu 862(%rsp), %ymm0
vpsubw 864(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 864(%rdi)
vmovdqu 830(%rsp), %ymm0
vpsubw 832(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 832(%rdi)
vmovdqu 798(%rsp), %ymm0
vpsubw 800(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 800(%rdi)
vmovdqu 766(%rsp), %ymm0
vpsubw 768(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 768(%rdi)
vmovdqu 734(%rsp), %ymm0
vpsubw 736(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 736(%rdi)
vmovdqu 702(%rsp), %ymm0
vpsubw 704(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 704(%rdi)
vmovdqu 670(%rsp), %ymm0
vpsubw 672(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 672(%rdi)
vmovdqu 638(%rsp), %ymm0
vpsubw 640(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 640(%rdi)
vmovdqu 606(%rsp), %ymm0
vpsubw 608(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 608(%rdi)
vmovdqu 574(%rsp), %ymm0
vpsubw 576(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 576(%rdi)
vmovdqu 542(%rsp), %ymm0
vpsubw 544(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 544(%rdi)
vmovdqu 510(%rsp), %ymm0
vpsubw 512(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 512(%rdi)
vmovdqu 478(%rsp), %ymm0
vpsubw 480(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 480(%rdi)
vmovdqu 446(%rsp), %ymm0
vpsubw 448(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 448(%rdi)
vmovdqu 414(%rsp), %ymm0
vpsubw 416(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 416(%rdi)
vmovdqu 382(%rsp), %ymm0
vpsubw 384(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 384(%rdi)
vmovdqu 350(%rsp), %ymm0
vpsubw 352(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 352(%rdi)
vmovdqu 318(%rsp), %ymm0
vpsubw 320(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 320(%rdi)
vmovdqu 286(%rsp), %ymm0
vpsubw 288(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 288(%rdi)
vmovdqu 254(%rsp), %ymm0
vpsubw 256(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 256(%rdi)
vmovdqu 222(%rsp), %ymm0
vpsubw 224(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 224(%rdi)
vmovdqu 190(%rsp), %ymm0
vpsubw 192(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 192(%rdi)
vmovdqu 158(%rsp), %ymm0
vpsubw 160(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 160(%rdi)
vmovdqu 126(%rsp), %ymm0
vpsubw 128(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 128(%rdi)
vmovdqu 94(%rsp), %ymm0
vpsubw 96(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 96(%rdi)
vmovdqu 62(%rsp), %ymm0
vpsubw 64(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 64(%rdi)
vmovdqu 30(%rsp), %ymm0
vpsubw 32(%rsp), %ymm0, %ymm1
vmovdqa %ymm1, 32(%rdi)
vmovdqa 0(%rsp), %ymm3
vpsrlq $48, %ymm3, %ymm0
vpermq $147, %ymm0, %ymm0
vpsllq $16, %ymm3, %ymm2
vpxor %ymm0, %ymm2, %ymm2
vpsubw %ymm3, %ymm2, %ymm3
vpand mask_omit_lowest(%rip), %ymm3, %ymm3
vpxor %ymm3, %ymm4, %ymm3
vmovdqa %ymm3, 0(%rdi)
mov %r8, %rsp
ret
