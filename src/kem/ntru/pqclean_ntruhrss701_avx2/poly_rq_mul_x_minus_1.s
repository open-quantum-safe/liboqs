.data
.p2align 5
mask_mod8192:
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
mask_mod8192_omit_lowest:
.word 0
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
mask_mod8192_only_lowest:
.word 8191
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
.global PQCLEAN_NTRUHRSS701_AVX2_poly_Rq_mul_x_minus_1
.global _PQCLEAN_NTRUHRSS701_AVX2_poly_Rq_mul_x_minus_1
PQCLEAN_NTRUHRSS701_AVX2_poly_Rq_mul_x_minus_1:
_PQCLEAN_NTRUHRSS701_AVX2_poly_Rq_mul_x_minus_1:
vmovdqu 1374(%rsi), %ymm0
vpsubw 1376(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 1376(%rdi)
vextracti128 $1, %ymm0, %xmm4
vpshufb shuf_5_to_0_zerorest(%rip), %ymm4, %ymm4
vpsubw 0(%rsi), %ymm4, %ymm4
vpand mask_mod8192_only_lowest(%rip), %ymm4, %ymm4
vmovdqu 1342(%rsi), %ymm0
vpsubw 1344(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 1344(%rdi)
vmovdqu 1310(%rsi), %ymm0
vpsubw 1312(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 1312(%rdi)
vmovdqu 1278(%rsi), %ymm0
vpsubw 1280(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 1280(%rdi)
vmovdqu 1246(%rsi), %ymm0
vpsubw 1248(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 1248(%rdi)
vmovdqu 1214(%rsi), %ymm0
vpsubw 1216(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 1216(%rdi)
vmovdqu 1182(%rsi), %ymm0
vpsubw 1184(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 1184(%rdi)
vmovdqu 1150(%rsi), %ymm0
vpsubw 1152(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 1152(%rdi)
vmovdqu 1118(%rsi), %ymm0
vpsubw 1120(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 1120(%rdi)
vmovdqu 1086(%rsi), %ymm0
vpsubw 1088(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 1088(%rdi)
vmovdqu 1054(%rsi), %ymm0
vpsubw 1056(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 1056(%rdi)
vmovdqu 1022(%rsi), %ymm0
vpsubw 1024(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 1024(%rdi)
vmovdqu 990(%rsi), %ymm0
vpsubw 992(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 992(%rdi)
vmovdqu 958(%rsi), %ymm0
vpsubw 960(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 960(%rdi)
vmovdqu 926(%rsi), %ymm0
vpsubw 928(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 928(%rdi)
vmovdqu 894(%rsi), %ymm0
vpsubw 896(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 896(%rdi)
vmovdqu 862(%rsi), %ymm0
vpsubw 864(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 864(%rdi)
vmovdqu 830(%rsi), %ymm0
vpsubw 832(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 832(%rdi)
vmovdqu 798(%rsi), %ymm0
vpsubw 800(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 800(%rdi)
vmovdqu 766(%rsi), %ymm0
vpsubw 768(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 768(%rdi)
vmovdqu 734(%rsi), %ymm0
vpsubw 736(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 736(%rdi)
vmovdqu 702(%rsi), %ymm0
vpsubw 704(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 704(%rdi)
vmovdqu 670(%rsi), %ymm0
vpsubw 672(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 672(%rdi)
vmovdqu 638(%rsi), %ymm0
vpsubw 640(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 640(%rdi)
vmovdqu 606(%rsi), %ymm0
vpsubw 608(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 608(%rdi)
vmovdqu 574(%rsi), %ymm0
vpsubw 576(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 576(%rdi)
vmovdqu 542(%rsi), %ymm0
vpsubw 544(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 544(%rdi)
vmovdqu 510(%rsi), %ymm0
vpsubw 512(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 512(%rdi)
vmovdqu 478(%rsi), %ymm0
vpsubw 480(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 480(%rdi)
vmovdqu 446(%rsi), %ymm0
vpsubw 448(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 448(%rdi)
vmovdqu 414(%rsi), %ymm0
vpsubw 416(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 416(%rdi)
vmovdqu 382(%rsi), %ymm0
vpsubw 384(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 384(%rdi)
vmovdqu 350(%rsi), %ymm0
vpsubw 352(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 352(%rdi)
vmovdqu 318(%rsi), %ymm0
vpsubw 320(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 320(%rdi)
vmovdqu 286(%rsi), %ymm0
vpsubw 288(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 288(%rdi)
vmovdqu 254(%rsi), %ymm0
vpsubw 256(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 256(%rdi)
vmovdqu 222(%rsi), %ymm0
vpsubw 224(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 224(%rdi)
vmovdqu 190(%rsi), %ymm0
vpsubw 192(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 192(%rdi)
vmovdqu 158(%rsi), %ymm0
vpsubw 160(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 160(%rdi)
vmovdqu 126(%rsi), %ymm0
vpsubw 128(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 128(%rdi)
vmovdqu 94(%rsi), %ymm0
vpsubw 96(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 96(%rdi)
vmovdqu 62(%rsi), %ymm0
vpsubw 64(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 64(%rdi)
vmovdqu 30(%rsi), %ymm0
vpsubw 32(%rsi), %ymm0, %ymm1
vpand mask_mod8192(%rip), %ymm1, %ymm1
vmovdqa %ymm1, 32(%rdi)
vmovdqa 0(%rsi), %ymm3
vpsrlq $48, %ymm3, %ymm0
vpermq $147, %ymm0, %ymm0
vpsllq $16, %ymm3, %ymm2
vpxor %ymm0, %ymm2, %ymm2
vpsubw %ymm3, %ymm2, %ymm3
vpand mask_mod8192_omit_lowest(%rip), %ymm3, %ymm3
vpxor %ymm3, %ymm4, %ymm3
vmovdqa %ymm3, 0(%rdi)
ret
