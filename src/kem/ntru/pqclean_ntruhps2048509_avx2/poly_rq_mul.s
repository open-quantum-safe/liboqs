.data
.p2align 5
mask_low13words:
.word 0xffff
.word 0xffff
.word 0xffff
.word 0xffff
.word 0xffff
.word 0xffff
.word 0xffff
.word 0xffff
.word 0xffff
.word 0xffff
.word 0xffff
.word 0xffff
.word 0xffff
.word 0x0
.word 0x0
.word 0x0
const3:
.word 3
.word 3
.word 3
.word 3
.word 3
.word 3
.word 3
.word 3
.word 3
.word 3
.word 3
.word 3
.word 3
.word 3
.word 3
.word 3
const9:
.word 9
.word 9
.word 9
.word 9
.word 9
.word 9
.word 9
.word 9
.word 9
.word 9
.word 9
.word 9
.word 9
.word 9
.word 9
.word 9
const0:
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
.word 0
const729:
.word 729
.word 729
.word 729
.word 729
.word 729
.word 729
.word 729
.word 729
.word 729
.word 729
.word 729
.word 729
.word 729
.word 729
.word 729
.word 729
const3_inv:
.word 43691
.word 43691
.word 43691
.word 43691
.word 43691
.word 43691
.word 43691
.word 43691
.word 43691
.word 43691
.word 43691
.word 43691
.word 43691
.word 43691
.word 43691
.word 43691
const5_inv:
.word 52429
.word 52429
.word 52429
.word 52429
.word 52429
.word 52429
.word 52429
.word 52429
.word 52429
.word 52429
.word 52429
.word 52429
.word 52429
.word 52429
.word 52429
.word 52429
shuf48_16:
.byte 10
.byte 11
.byte 12
.byte 13
.byte 14
.byte 15
.byte 0
.byte 1
.byte 2
.byte 3
.byte 4
.byte 5
.byte 6
.byte 7
.byte 8
.byte 9
.byte 10
.byte 11
.byte 12
.byte 13
.byte 14
.byte 15
.byte 0
.byte 1
.byte 2
.byte 3
.byte 4
.byte 5
.byte 6
.byte 7
.byte 8
.byte 9
shufmin5_mask3:
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
.byte 255
.byte 255
mask32_to_16:
.word 0xffff
.word 0x0
.word 0xffff
.word 0x0
.word 0xffff
.word 0x0
.word 0xffff
.word 0x0
.word 0xffff
.word 0x0
.word 0xffff
.word 0x0
.word 0xffff
.word 0x0
.word 0xffff
.word 0x0
mask5_3_5_3:
.word 0
.word 0
.word 0
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
.word 0
.word 0
.word 0
.word 65535
.word 65535
.word 65535
.word 65535
.word 65535
mask3_5_3_5:
.word 65535
.word 65535
.word 65535
.word 0
.word 0
.word 0
.word 0
.word 0
.word 65535
.word 65535
.word 65535
.word 0
.word 0
.word 0
.word 0
.word 0
mask_keephigh:
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
.text
.global PQCLEAN_NTRUHPS2048509_AVX2_poly_Rq_mul
.global _PQCLEAN_NTRUHPS2048509_AVX2_poly_Rq_mul
PQCLEAN_NTRUHPS2048509_AVX2_poly_Rq_mul:
_PQCLEAN_NTRUHPS2048509_AVX2_poly_Rq_mul:
push %r12
mov %rsp, %r8
andq $-32, %rsp
subq $4096, %rsp
mov %rsp, %rax
subq $4096, %rsp
mov %rsp, %r11
subq $8192, %rsp
mov %rsp, %r12
subq $512, %rsp
vmovdqa const3(%rip), %ymm3
vmovdqa 0(%rsi), %ymm0
vmovdqa 64(%rsi), %ymm1
vmovdqa 128(%rsi), %ymm2
vmovdqa 192(%rsi), %ymm12
vmovdqa 768(%rsi), %ymm4
vmovdqa 832(%rsi), %ymm5
vmovdqa 896(%rsi), %ymm6
vmovdqa 960(%rsi), %ymm7
vmovdqa 256(%rsi), %ymm8
vmovdqa 320(%rsi), %ymm9
vmovdqa 384(%rsi), %ymm10
vmovdqa 448(%rsi), %ymm11
vmovdqa %ymm0, 0(%rax)
vmovdqa %ymm1, 64(%rax)
vpaddw %ymm0, %ymm1, %ymm14
vmovdqa %ymm14, 128(%rax)
vmovdqa %ymm2, 192(%rax)
vmovdqa %ymm12, 256(%rax)
vpaddw %ymm2, %ymm12, %ymm14
vmovdqa %ymm14, 320(%rax)
vpaddw %ymm0, %ymm2, %ymm14
vmovdqa %ymm14, 384(%rax)
vpaddw %ymm1, %ymm12, %ymm15
vmovdqa %ymm15, 448(%rax)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 512(%rax)
vmovdqa %ymm4, 3456(%rax)
vmovdqa %ymm5, 3520(%rax)
vpaddw %ymm4, %ymm5, %ymm14
vmovdqa %ymm14, 3584(%rax)
vmovdqa %ymm6, 3648(%rax)
vmovdqa %ymm7, 3712(%rax)
vpaddw %ymm6, %ymm7, %ymm14
vmovdqa %ymm14, 3776(%rax)
vpaddw %ymm4, %ymm6, %ymm14
vmovdqa %ymm14, 3840(%rax)
vpaddw %ymm5, %ymm7, %ymm15
vmovdqa %ymm15, 3904(%rax)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3968(%rax)
vmovdqa %ymm0, 0(%rsp)
vmovdqa %ymm1, 32(%rsp)
vmovdqa %ymm2, 64(%rsp)
vmovdqa %ymm12, 96(%rsp)
vmovdqa %ymm8, 128(%rsp)
vmovdqa %ymm9, 160(%rsp)
vmovdqa %ymm10, 192(%rsp)
vmovdqa %ymm11, 224(%rsp)
vmovdqa 512(%rsi), %ymm0
vpaddw 0(%rsp), %ymm0, %ymm1
vpaddw 128(%rsp), %ymm4, %ymm2
vpaddw %ymm2, %ymm1, %ymm8
vpsubw %ymm2, %ymm1, %ymm12
vmovdqa %ymm0, 256(%rsp)
vmovdqa 576(%rsi), %ymm0
vpaddw 32(%rsp), %ymm0, %ymm1
vpaddw 160(%rsp), %ymm5, %ymm2
vpaddw %ymm2, %ymm1, %ymm9
vpsubw %ymm2, %ymm1, %ymm13
vmovdqa %ymm0, 288(%rsp)
vmovdqa 640(%rsi), %ymm0
vpaddw 64(%rsp), %ymm0, %ymm1
vpaddw 192(%rsp), %ymm6, %ymm2
vpaddw %ymm2, %ymm1, %ymm10
vpsubw %ymm2, %ymm1, %ymm14
vmovdqa %ymm0, 320(%rsp)
vmovdqa 704(%rsi), %ymm0
vpaddw 96(%rsp), %ymm0, %ymm1
vpaddw 224(%rsp), %ymm7, %ymm2
vpaddw %ymm2, %ymm1, %ymm11
vpsubw %ymm2, %ymm1, %ymm15
vmovdqa %ymm0, 352(%rsp)
vmovdqa %ymm8, 576(%rax)
vmovdqa %ymm9, 640(%rax)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 704(%rax)
vmovdqa %ymm10, 768(%rax)
vmovdqa %ymm11, 832(%rax)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 896(%rax)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 960(%rax)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 1024(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 1088(%rax)
vmovdqa %ymm12, 1152(%rax)
vmovdqa %ymm13, 1216(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 1280(%rax)
vmovdqa %ymm14, 1344(%rax)
vmovdqa %ymm15, 1408(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 1472(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 1536(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 1600(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 1664(%rax)
vmovdqa 256(%rsp), %ymm0
vpsllw $2, %ymm0, %ymm0
vpaddw 0(%rsp), %ymm0, %ymm0
vpsllw $2, %ymm4, %ymm1
vpaddw 128(%rsp), %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm8
vpsubw %ymm1, %ymm0, %ymm12
vmovdqa 288(%rsp), %ymm0
vpsllw $2, %ymm0, %ymm0
vpaddw 32(%rsp), %ymm0, %ymm0
vpsllw $2, %ymm5, %ymm1
vpaddw 160(%rsp), %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm9
vpsubw %ymm1, %ymm0, %ymm13
vmovdqa 320(%rsp), %ymm0
vpsllw $2, %ymm0, %ymm0
vpaddw 64(%rsp), %ymm0, %ymm0
vpsllw $2, %ymm6, %ymm1
vpaddw 192(%rsp), %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm10
vpsubw %ymm1, %ymm0, %ymm14
vmovdqa 352(%rsp), %ymm0
vpsllw $2, %ymm0, %ymm0
vpaddw 96(%rsp), %ymm0, %ymm0
vpsllw $2, %ymm7, %ymm1
vpaddw 224(%rsp), %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm11
vpsubw %ymm1, %ymm0, %ymm15
vmovdqa %ymm8, 1728(%rax)
vmovdqa %ymm9, 1792(%rax)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 1856(%rax)
vmovdqa %ymm10, 1920(%rax)
vmovdqa %ymm11, 1984(%rax)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 2048(%rax)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 2112(%rax)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 2176(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2240(%rax)
vmovdqa %ymm12, 2304(%rax)
vmovdqa %ymm13, 2368(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 2432(%rax)
vmovdqa %ymm14, 2496(%rax)
vmovdqa %ymm15, 2560(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 2624(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 2688(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 2752(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2816(%rax)
vpmullw %ymm3, %ymm4, %ymm0
vpaddw 256(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 128(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 0(%rsp), %ymm0, %ymm12
vpmullw %ymm3, %ymm5, %ymm0
vpaddw 288(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 160(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 32(%rsp), %ymm0, %ymm13
vpmullw %ymm3, %ymm6, %ymm0
vpaddw 320(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 192(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 64(%rsp), %ymm0, %ymm14
vpmullw %ymm3, %ymm7, %ymm0
vpaddw 352(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 224(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 96(%rsp), %ymm0, %ymm15
vmovdqa %ymm12, 2880(%rax)
vmovdqa %ymm13, 2944(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 3008(%rax)
vmovdqa %ymm14, 3072(%rax)
vmovdqa %ymm15, 3136(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 3200(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 3264(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 3328(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 3392(%rax)
vmovdqa 32(%rsi), %ymm0
vmovdqa 96(%rsi), %ymm1
vmovdqa 160(%rsi), %ymm2
vmovdqa 224(%rsi), %ymm12
vmovdqa 800(%rsi), %ymm4
vmovdqa 864(%rsi), %ymm5
vmovdqa 928(%rsi), %ymm6
vmovdqa 992(%rsi), %ymm7
vpand mask_low13words(%rip), %ymm7, %ymm7
vmovdqa 288(%rsi), %ymm8
vmovdqa 352(%rsi), %ymm9
vmovdqa 416(%rsi), %ymm10
vmovdqa 480(%rsi), %ymm11
vmovdqa %ymm0, 32(%rax)
vmovdqa %ymm1, 96(%rax)
vpaddw %ymm0, %ymm1, %ymm14
vmovdqa %ymm14, 160(%rax)
vmovdqa %ymm2, 224(%rax)
vmovdqa %ymm12, 288(%rax)
vpaddw %ymm2, %ymm12, %ymm14
vmovdqa %ymm14, 352(%rax)
vpaddw %ymm0, %ymm2, %ymm14
vmovdqa %ymm14, 416(%rax)
vpaddw %ymm1, %ymm12, %ymm15
vmovdqa %ymm15, 480(%rax)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 544(%rax)
vmovdqa %ymm4, 3488(%rax)
vmovdqa %ymm5, 3552(%rax)
vpaddw %ymm4, %ymm5, %ymm14
vmovdqa %ymm14, 3616(%rax)
vmovdqa %ymm6, 3680(%rax)
vmovdqa %ymm7, 3744(%rax)
vpaddw %ymm6, %ymm7, %ymm14
vmovdqa %ymm14, 3808(%rax)
vpaddw %ymm4, %ymm6, %ymm14
vmovdqa %ymm14, 3872(%rax)
vpaddw %ymm5, %ymm7, %ymm15
vmovdqa %ymm15, 3936(%rax)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4000(%rax)
vmovdqa %ymm0, 0(%rsp)
vmovdqa %ymm1, 32(%rsp)
vmovdqa %ymm2, 64(%rsp)
vmovdqa %ymm12, 96(%rsp)
vmovdqa %ymm8, 128(%rsp)
vmovdqa %ymm9, 160(%rsp)
vmovdqa %ymm10, 192(%rsp)
vmovdqa %ymm11, 224(%rsp)
vmovdqa 544(%rsi), %ymm0
vpaddw 0(%rsp), %ymm0, %ymm1
vpaddw 128(%rsp), %ymm4, %ymm2
vpaddw %ymm2, %ymm1, %ymm8
vpsubw %ymm2, %ymm1, %ymm12
vmovdqa %ymm0, 256(%rsp)
vmovdqa 608(%rsi), %ymm0
vpaddw 32(%rsp), %ymm0, %ymm1
vpaddw 160(%rsp), %ymm5, %ymm2
vpaddw %ymm2, %ymm1, %ymm9
vpsubw %ymm2, %ymm1, %ymm13
vmovdqa %ymm0, 288(%rsp)
vmovdqa 672(%rsi), %ymm0
vpaddw 64(%rsp), %ymm0, %ymm1
vpaddw 192(%rsp), %ymm6, %ymm2
vpaddw %ymm2, %ymm1, %ymm10
vpsubw %ymm2, %ymm1, %ymm14
vmovdqa %ymm0, 320(%rsp)
vmovdqa 736(%rsi), %ymm0
vpaddw 96(%rsp), %ymm0, %ymm1
vpaddw 224(%rsp), %ymm7, %ymm2
vpaddw %ymm2, %ymm1, %ymm11
vpsubw %ymm2, %ymm1, %ymm15
vmovdqa %ymm0, 352(%rsp)
vmovdqa %ymm8, 608(%rax)
vmovdqa %ymm9, 672(%rax)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 736(%rax)
vmovdqa %ymm10, 800(%rax)
vmovdqa %ymm11, 864(%rax)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 928(%rax)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 992(%rax)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 1056(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 1120(%rax)
vmovdqa %ymm12, 1184(%rax)
vmovdqa %ymm13, 1248(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 1312(%rax)
vmovdqa %ymm14, 1376(%rax)
vmovdqa %ymm15, 1440(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 1504(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 1568(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 1632(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 1696(%rax)
vmovdqa 256(%rsp), %ymm0
vpsllw $2, %ymm0, %ymm0
vpaddw 0(%rsp), %ymm0, %ymm0
vpsllw $2, %ymm4, %ymm1
vpaddw 128(%rsp), %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm8
vpsubw %ymm1, %ymm0, %ymm12
vmovdqa 288(%rsp), %ymm0
vpsllw $2, %ymm0, %ymm0
vpaddw 32(%rsp), %ymm0, %ymm0
vpsllw $2, %ymm5, %ymm1
vpaddw 160(%rsp), %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm9
vpsubw %ymm1, %ymm0, %ymm13
vmovdqa 320(%rsp), %ymm0
vpsllw $2, %ymm0, %ymm0
vpaddw 64(%rsp), %ymm0, %ymm0
vpsllw $2, %ymm6, %ymm1
vpaddw 192(%rsp), %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm10
vpsubw %ymm1, %ymm0, %ymm14
vmovdqa 352(%rsp), %ymm0
vpsllw $2, %ymm0, %ymm0
vpaddw 96(%rsp), %ymm0, %ymm0
vpsllw $2, %ymm7, %ymm1
vpaddw 224(%rsp), %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm11
vpsubw %ymm1, %ymm0, %ymm15
vmovdqa %ymm8, 1760(%rax)
vmovdqa %ymm9, 1824(%rax)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 1888(%rax)
vmovdqa %ymm10, 1952(%rax)
vmovdqa %ymm11, 2016(%rax)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 2080(%rax)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 2144(%rax)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 2208(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2272(%rax)
vmovdqa %ymm12, 2336(%rax)
vmovdqa %ymm13, 2400(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 2464(%rax)
vmovdqa %ymm14, 2528(%rax)
vmovdqa %ymm15, 2592(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 2656(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 2720(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 2784(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2848(%rax)
vpmullw %ymm3, %ymm4, %ymm0
vpaddw 256(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 128(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 0(%rsp), %ymm0, %ymm12
vpmullw %ymm3, %ymm5, %ymm0
vpaddw 288(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 160(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 32(%rsp), %ymm0, %ymm13
vpmullw %ymm3, %ymm6, %ymm0
vpaddw 320(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 192(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 64(%rsp), %ymm0, %ymm14
vpmullw %ymm3, %ymm7, %ymm0
vpaddw 352(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 224(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 96(%rsp), %ymm0, %ymm15
vmovdqa %ymm12, 2912(%rax)
vmovdqa %ymm13, 2976(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 3040(%rax)
vmovdqa %ymm14, 3104(%rax)
vmovdqa %ymm15, 3168(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 3232(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 3296(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 3360(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 3424(%rax)
vmovdqa 0(%rdx), %ymm0
vmovdqa 64(%rdx), %ymm1
vmovdqa 128(%rdx), %ymm2
vmovdqa 192(%rdx), %ymm12
vmovdqa 768(%rdx), %ymm4
vmovdqa 832(%rdx), %ymm5
vmovdqa 896(%rdx), %ymm6
vmovdqa 960(%rdx), %ymm7
vmovdqa 256(%rdx), %ymm8
vmovdqa 320(%rdx), %ymm9
vmovdqa 384(%rdx), %ymm10
vmovdqa 448(%rdx), %ymm11
vmovdqa %ymm0, 0(%r11)
vmovdqa %ymm1, 64(%r11)
vpaddw %ymm0, %ymm1, %ymm14
vmovdqa %ymm14, 128(%r11)
vmovdqa %ymm2, 192(%r11)
vmovdqa %ymm12, 256(%r11)
vpaddw %ymm2, %ymm12, %ymm14
vmovdqa %ymm14, 320(%r11)
vpaddw %ymm0, %ymm2, %ymm14
vmovdqa %ymm14, 384(%r11)
vpaddw %ymm1, %ymm12, %ymm15
vmovdqa %ymm15, 448(%r11)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 512(%r11)
vmovdqa %ymm4, 3456(%r11)
vmovdqa %ymm5, 3520(%r11)
vpaddw %ymm4, %ymm5, %ymm14
vmovdqa %ymm14, 3584(%r11)
vmovdqa %ymm6, 3648(%r11)
vmovdqa %ymm7, 3712(%r11)
vpaddw %ymm6, %ymm7, %ymm14
vmovdqa %ymm14, 3776(%r11)
vpaddw %ymm4, %ymm6, %ymm14
vmovdqa %ymm14, 3840(%r11)
vpaddw %ymm5, %ymm7, %ymm15
vmovdqa %ymm15, 3904(%r11)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3968(%r11)
vmovdqa %ymm0, 0(%rsp)
vmovdqa %ymm1, 32(%rsp)
vmovdqa %ymm2, 64(%rsp)
vmovdqa %ymm12, 96(%rsp)
vmovdqa %ymm8, 128(%rsp)
vmovdqa %ymm9, 160(%rsp)
vmovdqa %ymm10, 192(%rsp)
vmovdqa %ymm11, 224(%rsp)
vmovdqa 512(%rdx), %ymm0
vpaddw 0(%rsp), %ymm0, %ymm1
vpaddw 128(%rsp), %ymm4, %ymm2
vpaddw %ymm2, %ymm1, %ymm8
vpsubw %ymm2, %ymm1, %ymm12
vmovdqa %ymm0, 256(%rsp)
vmovdqa 576(%rdx), %ymm0
vpaddw 32(%rsp), %ymm0, %ymm1
vpaddw 160(%rsp), %ymm5, %ymm2
vpaddw %ymm2, %ymm1, %ymm9
vpsubw %ymm2, %ymm1, %ymm13
vmovdqa %ymm0, 288(%rsp)
vmovdqa 640(%rdx), %ymm0
vpaddw 64(%rsp), %ymm0, %ymm1
vpaddw 192(%rsp), %ymm6, %ymm2
vpaddw %ymm2, %ymm1, %ymm10
vpsubw %ymm2, %ymm1, %ymm14
vmovdqa %ymm0, 320(%rsp)
vmovdqa 704(%rdx), %ymm0
vpaddw 96(%rsp), %ymm0, %ymm1
vpaddw 224(%rsp), %ymm7, %ymm2
vpaddw %ymm2, %ymm1, %ymm11
vpsubw %ymm2, %ymm1, %ymm15
vmovdqa %ymm0, 352(%rsp)
vmovdqa %ymm8, 576(%r11)
vmovdqa %ymm9, 640(%r11)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 704(%r11)
vmovdqa %ymm10, 768(%r11)
vmovdqa %ymm11, 832(%r11)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 896(%r11)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 960(%r11)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 1024(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 1088(%r11)
vmovdqa %ymm12, 1152(%r11)
vmovdqa %ymm13, 1216(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 1280(%r11)
vmovdqa %ymm14, 1344(%r11)
vmovdqa %ymm15, 1408(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 1472(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 1536(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 1600(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 1664(%r11)
vmovdqa 256(%rsp), %ymm0
vpsllw $2, %ymm0, %ymm0
vpaddw 0(%rsp), %ymm0, %ymm0
vpsllw $2, %ymm4, %ymm1
vpaddw 128(%rsp), %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm8
vpsubw %ymm1, %ymm0, %ymm12
vmovdqa 288(%rsp), %ymm0
vpsllw $2, %ymm0, %ymm0
vpaddw 32(%rsp), %ymm0, %ymm0
vpsllw $2, %ymm5, %ymm1
vpaddw 160(%rsp), %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm9
vpsubw %ymm1, %ymm0, %ymm13
vmovdqa 320(%rsp), %ymm0
vpsllw $2, %ymm0, %ymm0
vpaddw 64(%rsp), %ymm0, %ymm0
vpsllw $2, %ymm6, %ymm1
vpaddw 192(%rsp), %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm10
vpsubw %ymm1, %ymm0, %ymm14
vmovdqa 352(%rsp), %ymm0
vpsllw $2, %ymm0, %ymm0
vpaddw 96(%rsp), %ymm0, %ymm0
vpsllw $2, %ymm7, %ymm1
vpaddw 224(%rsp), %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm11
vpsubw %ymm1, %ymm0, %ymm15
vmovdqa %ymm8, 1728(%r11)
vmovdqa %ymm9, 1792(%r11)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 1856(%r11)
vmovdqa %ymm10, 1920(%r11)
vmovdqa %ymm11, 1984(%r11)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 2048(%r11)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 2112(%r11)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 2176(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2240(%r11)
vmovdqa %ymm12, 2304(%r11)
vmovdqa %ymm13, 2368(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 2432(%r11)
vmovdqa %ymm14, 2496(%r11)
vmovdqa %ymm15, 2560(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 2624(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 2688(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 2752(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2816(%r11)
vpmullw %ymm3, %ymm4, %ymm0
vpaddw 256(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 128(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 0(%rsp), %ymm0, %ymm12
vpmullw %ymm3, %ymm5, %ymm0
vpaddw 288(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 160(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 32(%rsp), %ymm0, %ymm13
vpmullw %ymm3, %ymm6, %ymm0
vpaddw 320(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 192(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 64(%rsp), %ymm0, %ymm14
vpmullw %ymm3, %ymm7, %ymm0
vpaddw 352(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 224(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 96(%rsp), %ymm0, %ymm15
vmovdqa %ymm12, 2880(%r11)
vmovdqa %ymm13, 2944(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 3008(%r11)
vmovdqa %ymm14, 3072(%r11)
vmovdqa %ymm15, 3136(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 3200(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 3264(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 3328(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 3392(%r11)
vmovdqa 32(%rdx), %ymm0
vmovdqa 96(%rdx), %ymm1
vmovdqa 160(%rdx), %ymm2
vmovdqa 224(%rdx), %ymm12
vmovdqa 800(%rdx), %ymm4
vmovdqa 864(%rdx), %ymm5
vmovdqa 928(%rdx), %ymm6
vmovdqa 992(%rdx), %ymm7
vpand mask_low13words(%rip), %ymm7, %ymm7
vmovdqa 288(%rdx), %ymm8
vmovdqa 352(%rdx), %ymm9
vmovdqa 416(%rdx), %ymm10
vmovdqa 480(%rdx), %ymm11
vmovdqa %ymm0, 32(%r11)
vmovdqa %ymm1, 96(%r11)
vpaddw %ymm0, %ymm1, %ymm14
vmovdqa %ymm14, 160(%r11)
vmovdqa %ymm2, 224(%r11)
vmovdqa %ymm12, 288(%r11)
vpaddw %ymm2, %ymm12, %ymm14
vmovdqa %ymm14, 352(%r11)
vpaddw %ymm0, %ymm2, %ymm14
vmovdqa %ymm14, 416(%r11)
vpaddw %ymm1, %ymm12, %ymm15
vmovdqa %ymm15, 480(%r11)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 544(%r11)
vmovdqa %ymm4, 3488(%r11)
vmovdqa %ymm5, 3552(%r11)
vpaddw %ymm4, %ymm5, %ymm14
vmovdqa %ymm14, 3616(%r11)
vmovdqa %ymm6, 3680(%r11)
vmovdqa %ymm7, 3744(%r11)
vpaddw %ymm6, %ymm7, %ymm14
vmovdqa %ymm14, 3808(%r11)
vpaddw %ymm4, %ymm6, %ymm14
vmovdqa %ymm14, 3872(%r11)
vpaddw %ymm5, %ymm7, %ymm15
vmovdqa %ymm15, 3936(%r11)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4000(%r11)
vmovdqa %ymm0, 0(%rsp)
vmovdqa %ymm1, 32(%rsp)
vmovdqa %ymm2, 64(%rsp)
vmovdqa %ymm12, 96(%rsp)
vmovdqa %ymm8, 128(%rsp)
vmovdqa %ymm9, 160(%rsp)
vmovdqa %ymm10, 192(%rsp)
vmovdqa %ymm11, 224(%rsp)
vmovdqa 544(%rdx), %ymm0
vpaddw 0(%rsp), %ymm0, %ymm1
vpaddw 128(%rsp), %ymm4, %ymm2
vpaddw %ymm2, %ymm1, %ymm8
vpsubw %ymm2, %ymm1, %ymm12
vmovdqa %ymm0, 256(%rsp)
vmovdqa 608(%rdx), %ymm0
vpaddw 32(%rsp), %ymm0, %ymm1
vpaddw 160(%rsp), %ymm5, %ymm2
vpaddw %ymm2, %ymm1, %ymm9
vpsubw %ymm2, %ymm1, %ymm13
vmovdqa %ymm0, 288(%rsp)
vmovdqa 672(%rdx), %ymm0
vpaddw 64(%rsp), %ymm0, %ymm1
vpaddw 192(%rsp), %ymm6, %ymm2
vpaddw %ymm2, %ymm1, %ymm10
vpsubw %ymm2, %ymm1, %ymm14
vmovdqa %ymm0, 320(%rsp)
vmovdqa 736(%rdx), %ymm0
vpaddw 96(%rsp), %ymm0, %ymm1
vpaddw 224(%rsp), %ymm7, %ymm2
vpaddw %ymm2, %ymm1, %ymm11
vpsubw %ymm2, %ymm1, %ymm15
vmovdqa %ymm0, 352(%rsp)
vmovdqa %ymm8, 608(%r11)
vmovdqa %ymm9, 672(%r11)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 736(%r11)
vmovdqa %ymm10, 800(%r11)
vmovdqa %ymm11, 864(%r11)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 928(%r11)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 992(%r11)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 1056(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 1120(%r11)
vmovdqa %ymm12, 1184(%r11)
vmovdqa %ymm13, 1248(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 1312(%r11)
vmovdqa %ymm14, 1376(%r11)
vmovdqa %ymm15, 1440(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 1504(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 1568(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 1632(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 1696(%r11)
vmovdqa 256(%rsp), %ymm0
vpsllw $2, %ymm0, %ymm0
vpaddw 0(%rsp), %ymm0, %ymm0
vpsllw $2, %ymm4, %ymm1
vpaddw 128(%rsp), %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm8
vpsubw %ymm1, %ymm0, %ymm12
vmovdqa 288(%rsp), %ymm0
vpsllw $2, %ymm0, %ymm0
vpaddw 32(%rsp), %ymm0, %ymm0
vpsllw $2, %ymm5, %ymm1
vpaddw 160(%rsp), %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm9
vpsubw %ymm1, %ymm0, %ymm13
vmovdqa 320(%rsp), %ymm0
vpsllw $2, %ymm0, %ymm0
vpaddw 64(%rsp), %ymm0, %ymm0
vpsllw $2, %ymm6, %ymm1
vpaddw 192(%rsp), %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm10
vpsubw %ymm1, %ymm0, %ymm14
vmovdqa 352(%rsp), %ymm0
vpsllw $2, %ymm0, %ymm0
vpaddw 96(%rsp), %ymm0, %ymm0
vpsllw $2, %ymm7, %ymm1
vpaddw 224(%rsp), %ymm1, %ymm1
vpsllw $1, %ymm1, %ymm1
vpaddw %ymm1, %ymm0, %ymm11
vpsubw %ymm1, %ymm0, %ymm15
vmovdqa %ymm8, 1760(%r11)
vmovdqa %ymm9, 1824(%r11)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 1888(%r11)
vmovdqa %ymm10, 1952(%r11)
vmovdqa %ymm11, 2016(%r11)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 2080(%r11)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 2144(%r11)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 2208(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2272(%r11)
vmovdqa %ymm12, 2336(%r11)
vmovdqa %ymm13, 2400(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 2464(%r11)
vmovdqa %ymm14, 2528(%r11)
vmovdqa %ymm15, 2592(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 2656(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 2720(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 2784(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2848(%r11)
vpmullw %ymm3, %ymm4, %ymm0
vpaddw 256(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 128(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 0(%rsp), %ymm0, %ymm12
vpmullw %ymm3, %ymm5, %ymm0
vpaddw 288(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 160(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 32(%rsp), %ymm0, %ymm13
vpmullw %ymm3, %ymm6, %ymm0
vpaddw 320(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 192(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 64(%rsp), %ymm0, %ymm14
vpmullw %ymm3, %ymm7, %ymm0
vpaddw 352(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 224(%rsp), %ymm0, %ymm0
vpmullw %ymm3, %ymm0, %ymm0
vpaddw 96(%rsp), %ymm0, %ymm15
vmovdqa %ymm12, 2912(%r11)
vmovdqa %ymm13, 2976(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 3040(%r11)
vmovdqa %ymm14, 3104(%r11)
vmovdqa %ymm15, 3168(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 3232(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 3296(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 3360(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 3424(%r11)
subq $6656, %rsp
mov $4, %ecx
karatsuba_loop_1:
mov %rsp, %r9
mov %rsp, %r10
subq $32, %rsp
vmovdqa 0(%rax), %ymm0
vmovdqa 128(%rax), %ymm1
vmovdqa 256(%rax), %ymm2
vmovdqa 384(%rax), %ymm3
vpunpcklwd 64(%rax), %ymm0, %ymm4
vpunpckhwd 64(%rax), %ymm0, %ymm5
vpunpcklwd 192(%rax), %ymm1, %ymm6
vpunpckhwd 192(%rax), %ymm1, %ymm7
vpunpcklwd 320(%rax), %ymm2, %ymm8
vpunpckhwd 320(%rax), %ymm2, %ymm9
vpunpcklwd 448(%rax), %ymm3, %ymm10
vpunpckhwd 448(%rax), %ymm3, %ymm11
vpunpckldq %ymm6, %ymm4, %ymm0
vpunpckhdq %ymm6, %ymm4, %ymm1
vpunpckldq %ymm7, %ymm5, %ymm2
vpunpckhdq %ymm7, %ymm5, %ymm3
vpunpckldq %ymm10, %ymm8, %ymm12
vpunpckhdq %ymm10, %ymm8, %ymm13
vpunpckldq %ymm11, %ymm9, %ymm14
vpunpckhdq %ymm11, %ymm9, %ymm15
vpunpcklqdq %ymm12, %ymm0, %ymm4
vpunpckhqdq %ymm12, %ymm0, %ymm5
vpunpcklqdq %ymm13, %ymm1, %ymm6
vpunpckhqdq %ymm13, %ymm1, %ymm7
vpunpcklqdq %ymm14, %ymm2, %ymm8
vpunpckhqdq %ymm14, %ymm2, %ymm9
vpunpcklqdq %ymm15, %ymm3, %ymm10
vpunpckhqdq %ymm15, %ymm3, %ymm11
vmovdqa 512(%rax), %ymm0
vmovdqa 640(%rax), %ymm1
vmovdqa 768(%rax), %ymm2
vmovdqa 896(%rax), %ymm3
vpunpcklwd 576(%rax), %ymm0, %ymm12
vpunpckhwd 576(%rax), %ymm0, %ymm13
vpunpcklwd 704(%rax), %ymm1, %ymm14
vpunpckhwd 704(%rax), %ymm1, %ymm15
vpunpcklwd 832(%rax), %ymm2, %ymm0
vpunpckhwd 832(%rax), %ymm2, %ymm1
vpunpcklwd 960(%rax), %ymm3, %ymm2
vpunpckhwd 960(%rax), %ymm3, %ymm3
vmovdqa %ymm11, 0(%rsp)
vpunpckldq %ymm14, %ymm12, %ymm11
vpunpckhdq %ymm14, %ymm12, %ymm12
vpunpckldq %ymm15, %ymm13, %ymm14
vpunpckhdq %ymm15, %ymm13, %ymm15
vpunpckldq %ymm2, %ymm0, %ymm13
vpunpckhdq %ymm2, %ymm0, %ymm0
vpunpckldq %ymm3, %ymm1, %ymm2
vpunpckhdq %ymm3, %ymm1, %ymm1
vpunpcklqdq %ymm13, %ymm11, %ymm3
vpunpckhqdq %ymm13, %ymm11, %ymm13
vpunpcklqdq %ymm0, %ymm12, %ymm11
vpunpckhqdq %ymm0, %ymm12, %ymm0
vpunpcklqdq %ymm2, %ymm14, %ymm12
vpunpckhqdq %ymm2, %ymm14, %ymm2
vpunpcklqdq %ymm1, %ymm15, %ymm14
vpunpckhqdq %ymm1, %ymm15, %ymm1
vinserti128 $1, %xmm3, %ymm4, %ymm15
vmovdqa %ymm15, 0(%r9)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 32(%r9)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 64(%r9)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 96(%r9)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 128(%r9)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 160(%r9)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 192(%r9)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 256(%r9)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 288(%r9)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 320(%r9)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 352(%r9)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 384(%r9)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 416(%r9)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 448(%r9)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 224(%r9)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 480(%r9)
vmovdqa 32(%rax), %ymm0
vmovdqa 160(%rax), %ymm1
vmovdqa 288(%rax), %ymm2
vmovdqa 416(%rax), %ymm3
vpunpcklwd 96(%rax), %ymm0, %ymm4
vpunpckhwd 96(%rax), %ymm0, %ymm5
vpunpcklwd 224(%rax), %ymm1, %ymm6
vpunpckhwd 224(%rax), %ymm1, %ymm7
vpunpcklwd 352(%rax), %ymm2, %ymm8
vpunpckhwd 352(%rax), %ymm2, %ymm9
vpunpcklwd 480(%rax), %ymm3, %ymm10
vpunpckhwd 480(%rax), %ymm3, %ymm11
vpunpckldq %ymm6, %ymm4, %ymm0
vpunpckhdq %ymm6, %ymm4, %ymm1
vpunpckldq %ymm7, %ymm5, %ymm2
vpunpckhdq %ymm7, %ymm5, %ymm3
vpunpckldq %ymm10, %ymm8, %ymm12
vpunpckhdq %ymm10, %ymm8, %ymm13
vpunpckldq %ymm11, %ymm9, %ymm14
vpunpckhdq %ymm11, %ymm9, %ymm15
vpunpcklqdq %ymm12, %ymm0, %ymm4
vpunpckhqdq %ymm12, %ymm0, %ymm5
vpunpcklqdq %ymm13, %ymm1, %ymm6
vpunpckhqdq %ymm13, %ymm1, %ymm7
vpunpcklqdq %ymm14, %ymm2, %ymm8
vpunpckhqdq %ymm14, %ymm2, %ymm9
vpunpcklqdq %ymm15, %ymm3, %ymm10
vpunpckhqdq %ymm15, %ymm3, %ymm11
vmovdqa 544(%rax), %ymm0
vmovdqa 672(%rax), %ymm1
vmovdqa 800(%rax), %ymm2
vmovdqa 928(%rax), %ymm3
vpunpcklwd 608(%rax), %ymm0, %ymm12
vpunpckhwd 608(%rax), %ymm0, %ymm13
vpunpcklwd 736(%rax), %ymm1, %ymm14
vpunpckhwd 736(%rax), %ymm1, %ymm15
vpunpcklwd 864(%rax), %ymm2, %ymm0
vpunpckhwd 864(%rax), %ymm2, %ymm1
vpunpcklwd 992(%rax), %ymm3, %ymm2
vpunpckhwd 992(%rax), %ymm3, %ymm3
vmovdqa %ymm11, 0(%rsp)
vpunpckldq %ymm14, %ymm12, %ymm11
vpunpckhdq %ymm14, %ymm12, %ymm12
vpunpckldq %ymm15, %ymm13, %ymm14
vpunpckhdq %ymm15, %ymm13, %ymm15
vpunpckldq %ymm2, %ymm0, %ymm13
vpunpckhdq %ymm2, %ymm0, %ymm0
vpunpckldq %ymm3, %ymm1, %ymm2
vpunpckhdq %ymm3, %ymm1, %ymm1
vpunpcklqdq %ymm13, %ymm11, %ymm3
vpunpckhqdq %ymm13, %ymm11, %ymm13
vpunpcklqdq %ymm0, %ymm12, %ymm11
vpunpckhqdq %ymm0, %ymm12, %ymm0
vpunpcklqdq %ymm2, %ymm14, %ymm12
vpunpckhqdq %ymm2, %ymm14, %ymm2
vpunpcklqdq %ymm1, %ymm15, %ymm14
vpunpckhqdq %ymm1, %ymm15, %ymm1
vinserti128 $1, %xmm3, %ymm4, %ymm15
vmovdqa %ymm15, 512(%r9)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 544(%r9)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 576(%r9)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 608(%r9)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 640(%r9)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 672(%r9)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 704(%r9)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 768(%r9)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 800(%r9)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 832(%r9)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 864(%r9)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 896(%r9)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 928(%r9)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 960(%r9)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 736(%r9)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 992(%r9)
addq $32, %rsp
subq $32, %rsp
vmovdqa 0(%r11), %ymm0
vmovdqa 128(%r11), %ymm1
vmovdqa 256(%r11), %ymm2
vmovdqa 384(%r11), %ymm3
vpunpcklwd 64(%r11), %ymm0, %ymm4
vpunpckhwd 64(%r11), %ymm0, %ymm5
vpunpcklwd 192(%r11), %ymm1, %ymm6
vpunpckhwd 192(%r11), %ymm1, %ymm7
vpunpcklwd 320(%r11), %ymm2, %ymm8
vpunpckhwd 320(%r11), %ymm2, %ymm9
vpunpcklwd 448(%r11), %ymm3, %ymm10
vpunpckhwd 448(%r11), %ymm3, %ymm11
vpunpckldq %ymm6, %ymm4, %ymm0
vpunpckhdq %ymm6, %ymm4, %ymm1
vpunpckldq %ymm7, %ymm5, %ymm2
vpunpckhdq %ymm7, %ymm5, %ymm3
vpunpckldq %ymm10, %ymm8, %ymm12
vpunpckhdq %ymm10, %ymm8, %ymm13
vpunpckldq %ymm11, %ymm9, %ymm14
vpunpckhdq %ymm11, %ymm9, %ymm15
vpunpcklqdq %ymm12, %ymm0, %ymm4
vpunpckhqdq %ymm12, %ymm0, %ymm5
vpunpcklqdq %ymm13, %ymm1, %ymm6
vpunpckhqdq %ymm13, %ymm1, %ymm7
vpunpcklqdq %ymm14, %ymm2, %ymm8
vpunpckhqdq %ymm14, %ymm2, %ymm9
vpunpcklqdq %ymm15, %ymm3, %ymm10
vpunpckhqdq %ymm15, %ymm3, %ymm11
vmovdqa 512(%r11), %ymm0
vmovdqa 640(%r11), %ymm1
vmovdqa 768(%r11), %ymm2
vmovdqa 896(%r11), %ymm3
vpunpcklwd 576(%r11), %ymm0, %ymm12
vpunpckhwd 576(%r11), %ymm0, %ymm13
vpunpcklwd 704(%r11), %ymm1, %ymm14
vpunpckhwd 704(%r11), %ymm1, %ymm15
vpunpcklwd 832(%r11), %ymm2, %ymm0
vpunpckhwd 832(%r11), %ymm2, %ymm1
vpunpcklwd 960(%r11), %ymm3, %ymm2
vpunpckhwd 960(%r11), %ymm3, %ymm3
vmovdqa %ymm11, 0(%rsp)
vpunpckldq %ymm14, %ymm12, %ymm11
vpunpckhdq %ymm14, %ymm12, %ymm12
vpunpckldq %ymm15, %ymm13, %ymm14
vpunpckhdq %ymm15, %ymm13, %ymm15
vpunpckldq %ymm2, %ymm0, %ymm13
vpunpckhdq %ymm2, %ymm0, %ymm0
vpunpckldq %ymm3, %ymm1, %ymm2
vpunpckhdq %ymm3, %ymm1, %ymm1
vpunpcklqdq %ymm13, %ymm11, %ymm3
vpunpckhqdq %ymm13, %ymm11, %ymm13
vpunpcklqdq %ymm0, %ymm12, %ymm11
vpunpckhqdq %ymm0, %ymm12, %ymm0
vpunpcklqdq %ymm2, %ymm14, %ymm12
vpunpckhqdq %ymm2, %ymm14, %ymm2
vpunpcklqdq %ymm1, %ymm15, %ymm14
vpunpckhqdq %ymm1, %ymm15, %ymm1
vinserti128 $1, %xmm3, %ymm4, %ymm15
vmovdqa %ymm15, 1024(%r9)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 1056(%r9)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 1088(%r9)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 1120(%r9)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 1152(%r9)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 1184(%r9)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 1216(%r9)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 1280(%r9)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 1312(%r9)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 1344(%r9)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 1376(%r9)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 1408(%r9)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 1440(%r9)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 1472(%r9)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1248(%r9)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 1504(%r9)
vmovdqa 32(%r11), %ymm0
vmovdqa 160(%r11), %ymm1
vmovdqa 288(%r11), %ymm2
vmovdqa 416(%r11), %ymm3
vpunpcklwd 96(%r11), %ymm0, %ymm4
vpunpckhwd 96(%r11), %ymm0, %ymm5
vpunpcklwd 224(%r11), %ymm1, %ymm6
vpunpckhwd 224(%r11), %ymm1, %ymm7
vpunpcklwd 352(%r11), %ymm2, %ymm8
vpunpckhwd 352(%r11), %ymm2, %ymm9
vpunpcklwd 480(%r11), %ymm3, %ymm10
vpunpckhwd 480(%r11), %ymm3, %ymm11
vpunpckldq %ymm6, %ymm4, %ymm0
vpunpckhdq %ymm6, %ymm4, %ymm1
vpunpckldq %ymm7, %ymm5, %ymm2
vpunpckhdq %ymm7, %ymm5, %ymm3
vpunpckldq %ymm10, %ymm8, %ymm12
vpunpckhdq %ymm10, %ymm8, %ymm13
vpunpckldq %ymm11, %ymm9, %ymm14
vpunpckhdq %ymm11, %ymm9, %ymm15
vpunpcklqdq %ymm12, %ymm0, %ymm4
vpunpckhqdq %ymm12, %ymm0, %ymm5
vpunpcklqdq %ymm13, %ymm1, %ymm6
vpunpckhqdq %ymm13, %ymm1, %ymm7
vpunpcklqdq %ymm14, %ymm2, %ymm8
vpunpckhqdq %ymm14, %ymm2, %ymm9
vpunpcklqdq %ymm15, %ymm3, %ymm10
vpunpckhqdq %ymm15, %ymm3, %ymm11
vmovdqa 544(%r11), %ymm0
vmovdqa 672(%r11), %ymm1
vmovdqa 800(%r11), %ymm2
vmovdqa 928(%r11), %ymm3
vpunpcklwd 608(%r11), %ymm0, %ymm12
vpunpckhwd 608(%r11), %ymm0, %ymm13
vpunpcklwd 736(%r11), %ymm1, %ymm14
vpunpckhwd 736(%r11), %ymm1, %ymm15
vpunpcklwd 864(%r11), %ymm2, %ymm0
vpunpckhwd 864(%r11), %ymm2, %ymm1
vpunpcklwd 992(%r11), %ymm3, %ymm2
vpunpckhwd 992(%r11), %ymm3, %ymm3
vmovdqa %ymm11, 0(%rsp)
vpunpckldq %ymm14, %ymm12, %ymm11
vpunpckhdq %ymm14, %ymm12, %ymm12
vpunpckldq %ymm15, %ymm13, %ymm14
vpunpckhdq %ymm15, %ymm13, %ymm15
vpunpckldq %ymm2, %ymm0, %ymm13
vpunpckhdq %ymm2, %ymm0, %ymm0
vpunpckldq %ymm3, %ymm1, %ymm2
vpunpckhdq %ymm3, %ymm1, %ymm1
vpunpcklqdq %ymm13, %ymm11, %ymm3
vpunpckhqdq %ymm13, %ymm11, %ymm13
vpunpcklqdq %ymm0, %ymm12, %ymm11
vpunpckhqdq %ymm0, %ymm12, %ymm0
vpunpcklqdq %ymm2, %ymm14, %ymm12
vpunpckhqdq %ymm2, %ymm14, %ymm2
vpunpcklqdq %ymm1, %ymm15, %ymm14
vpunpckhqdq %ymm1, %ymm15, %ymm1
vinserti128 $1, %xmm3, %ymm4, %ymm15
vmovdqa %ymm15, 1536(%r9)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 1568(%r9)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 1600(%r9)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 1632(%r9)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 1664(%r9)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 1696(%r9)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 1728(%r9)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 1792(%r9)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 1824(%r9)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 1856(%r9)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 1888(%r9)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 1920(%r9)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 1952(%r9)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 1984(%r9)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1760(%r9)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 2016(%r9)
addq $32, %rsp
innerloop_1:
vmovdqa 0(%r9), %ymm0
vmovdqa 1024(%r9), %ymm4
vmovdqa 32(%r9), %ymm1
vmovdqa 1056(%r9), %ymm5
vmovdqa 64(%r9), %ymm2
vmovdqa 1088(%r9), %ymm6
vmovdqa 96(%r9), %ymm3
vmovdqa 1120(%r9), %ymm7
vpmullw %ymm0, %ymm4, %ymm12
vmovdqa %ymm12, 2048(%r10)
vpmullw %ymm0, %ymm5, %ymm13
vpmullw %ymm1, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 2080(%r10)
vpmullw %ymm0, %ymm6, %ymm12
vpmullw %ymm1, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm4, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 2112(%r10)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm5, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 2144(%r10)
vpmullw %ymm1, %ymm7, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 2176(%r10)
vpmullw %ymm2, %ymm7, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 2208(%r10)
vpmullw %ymm3, %ymm7, %ymm12
vmovdqa %ymm12, 2240(%r10)
vmovdqa 128(%r9), %ymm0
vmovdqa 1152(%r9), %ymm4
vmovdqa 160(%r9), %ymm1
vmovdqa 1184(%r9), %ymm5
vmovdqa 192(%r9), %ymm2
vmovdqa 1216(%r9), %ymm6
vmovdqa 224(%r9), %ymm3
vmovdqa 1248(%r9), %ymm7
vpmullw %ymm0, %ymm4, %ymm12
vmovdqa %ymm12, 2304(%r10)
vpmullw %ymm0, %ymm5, %ymm13
vpmullw %ymm1, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 2336(%r10)
vpmullw %ymm0, %ymm6, %ymm12
vpmullw %ymm1, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm4, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 2368(%r10)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm5, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 2400(%r10)
vpmullw %ymm1, %ymm7, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 2432(%r10)
vpmullw %ymm2, %ymm7, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 2464(%r10)
vpmullw %ymm3, %ymm7, %ymm12
vmovdqa %ymm12, 2496(%r10)
vpaddw 0(%r9), %ymm0, %ymm0
vpaddw 1024(%r9), %ymm4, %ymm4
vpaddw 32(%r9), %ymm1, %ymm1
vpaddw 1056(%r9), %ymm5, %ymm5
vpaddw 64(%r9), %ymm2, %ymm2
vpaddw 1088(%r9), %ymm6, %ymm6
vpaddw 96(%r9), %ymm3, %ymm3
vpaddw 1120(%r9), %ymm7, %ymm7
vpmullw %ymm0, %ymm4, %ymm9
vpmullw %ymm0, %ymm5, %ymm10
vpmullw %ymm1, %ymm4, %ymm8
vpaddw %ymm8, %ymm10, %ymm10
vpmullw %ymm0, %ymm6, %ymm11
vpmullw %ymm1, %ymm5, %ymm8
vpaddw %ymm8, %ymm11, %ymm11
vpmullw %ymm2, %ymm4, %ymm8
vpaddw %ymm8, %ymm11, %ymm11
vpmullw %ymm0, %ymm7, %ymm12
vpmullw %ymm1, %ymm6, %ymm8
vpaddw %ymm8, %ymm12, %ymm12
vpmullw %ymm2, %ymm5, %ymm8
vpaddw %ymm8, %ymm12, %ymm12
vpmullw %ymm3, %ymm4, %ymm8
vpaddw %ymm8, %ymm12, %ymm12
vpmullw %ymm1, %ymm7, %ymm13
vpmullw %ymm2, %ymm6, %ymm8
vpaddw %ymm8, %ymm13, %ymm13
vpmullw %ymm3, %ymm5, %ymm8
vpaddw %ymm8, %ymm13, %ymm13
vpmullw %ymm2, %ymm7, %ymm14
vpmullw %ymm3, %ymm6, %ymm8
vpaddw %ymm8, %ymm14, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpsubw 2048(%r10), %ymm9, %ymm9
vmovdqa 2304(%r10), %ymm3
vpsubw %ymm3, %ymm9, %ymm9
vpsubw 2080(%r10), %ymm10, %ymm10
vmovdqa 2336(%r10), %ymm4
vpsubw %ymm4, %ymm10, %ymm10
vpsubw 2112(%r10), %ymm11, %ymm11
vmovdqa 2368(%r10), %ymm5
vpsubw %ymm5, %ymm11, %ymm11
vpsubw 2144(%r10), %ymm12, %ymm12
vpsubw 2400(%r10), %ymm12, %ymm12
vmovdqa 2176(%r10), %ymm0
vpsubw %ymm0, %ymm13, %ymm13
vpsubw 2432(%r10), %ymm13, %ymm13
vmovdqa 2208(%r10), %ymm1
vpsubw %ymm1, %ymm14, %ymm14
vpsubw 2464(%r10), %ymm14, %ymm14
vmovdqa 2240(%r10), %ymm2
vpsubw %ymm2, %ymm15, %ymm15
vpsubw 2496(%r10), %ymm15, %ymm15
vpaddw %ymm0, %ymm9, %ymm9
vmovdqa %ymm9, 2176(%r10)
vpaddw %ymm1, %ymm10, %ymm10
vmovdqa %ymm10, 2208(%r10)
vpaddw %ymm2, %ymm11, %ymm11
vmovdqa %ymm11, 2240(%r10)
vmovdqa %ymm12, 2272(%r10)
vpaddw %ymm3, %ymm13, %ymm13
vmovdqa %ymm13, 2304(%r10)
vpaddw %ymm4, %ymm14, %ymm14
vmovdqa %ymm14, 2336(%r10)
vpaddw %ymm5, %ymm15, %ymm15
vmovdqa %ymm15, 2368(%r10)
vmovdqa 256(%r9), %ymm0
vmovdqa 1280(%r9), %ymm4
vmovdqa 288(%r9), %ymm1
vmovdqa 1312(%r9), %ymm5
vmovdqa 320(%r9), %ymm2
vmovdqa 1344(%r9), %ymm6
vmovdqa 352(%r9), %ymm3
vmovdqa 1376(%r9), %ymm7
vpmullw %ymm0, %ymm4, %ymm12
vmovdqa %ymm12, 2560(%r10)
vpmullw %ymm0, %ymm5, %ymm13
vpmullw %ymm1, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 2592(%r10)
vpmullw %ymm0, %ymm6, %ymm12
vpmullw %ymm1, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm4, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 2624(%r10)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm5, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 2656(%r10)
vpmullw %ymm1, %ymm7, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 2688(%r10)
vpmullw %ymm2, %ymm7, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 2720(%r10)
vpmullw %ymm3, %ymm7, %ymm12
vmovdqa %ymm12, 2752(%r10)
vmovdqa 384(%r9), %ymm0
vmovdqa 1408(%r9), %ymm4
vmovdqa 416(%r9), %ymm1
vmovdqa 1440(%r9), %ymm5
vmovdqa 448(%r9), %ymm2
vmovdqa 1472(%r9), %ymm6
vmovdqa 480(%r9), %ymm3
vmovdqa 1504(%r9), %ymm7
vpmullw %ymm0, %ymm4, %ymm12
vmovdqa %ymm12, 2816(%r10)
vpmullw %ymm0, %ymm5, %ymm13
vpmullw %ymm1, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 2848(%r10)
vpmullw %ymm0, %ymm6, %ymm12
vpmullw %ymm1, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm4, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 2880(%r10)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm5, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 2912(%r10)
vpmullw %ymm1, %ymm7, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 2944(%r10)
vpmullw %ymm2, %ymm7, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 2976(%r10)
vpmullw %ymm3, %ymm7, %ymm12
vmovdqa %ymm12, 3008(%r10)
vpaddw 256(%r9), %ymm0, %ymm0
vpaddw 1280(%r9), %ymm4, %ymm4
vpaddw 288(%r9), %ymm1, %ymm1
vpaddw 1312(%r9), %ymm5, %ymm5
vpaddw 320(%r9), %ymm2, %ymm2
vpaddw 1344(%r9), %ymm6, %ymm6
vpaddw 352(%r9), %ymm3, %ymm3
vpaddw 1376(%r9), %ymm7, %ymm7
vpmullw %ymm0, %ymm4, %ymm9
vpmullw %ymm0, %ymm5, %ymm10
vpmullw %ymm1, %ymm4, %ymm8
vpaddw %ymm8, %ymm10, %ymm10
vpmullw %ymm0, %ymm6, %ymm11
vpmullw %ymm1, %ymm5, %ymm8
vpaddw %ymm8, %ymm11, %ymm11
vpmullw %ymm2, %ymm4, %ymm8
vpaddw %ymm8, %ymm11, %ymm11
vpmullw %ymm0, %ymm7, %ymm12
vpmullw %ymm1, %ymm6, %ymm8
vpaddw %ymm8, %ymm12, %ymm12
vpmullw %ymm2, %ymm5, %ymm8
vpaddw %ymm8, %ymm12, %ymm12
vpmullw %ymm3, %ymm4, %ymm8
vpaddw %ymm8, %ymm12, %ymm12
vpmullw %ymm1, %ymm7, %ymm13
vpmullw %ymm2, %ymm6, %ymm8
vpaddw %ymm8, %ymm13, %ymm13
vpmullw %ymm3, %ymm5, %ymm8
vpaddw %ymm8, %ymm13, %ymm13
vpmullw %ymm2, %ymm7, %ymm14
vpmullw %ymm3, %ymm6, %ymm8
vpaddw %ymm8, %ymm14, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpsubw 2560(%r10), %ymm9, %ymm9
vmovdqa 2816(%r10), %ymm3
vpsubw %ymm3, %ymm9, %ymm9
vpsubw 2592(%r10), %ymm10, %ymm10
vmovdqa 2848(%r10), %ymm4
vpsubw %ymm4, %ymm10, %ymm10
vpsubw 2624(%r10), %ymm11, %ymm11
vmovdqa 2880(%r10), %ymm5
vpsubw %ymm5, %ymm11, %ymm11
vpsubw 2656(%r10), %ymm12, %ymm12
vpsubw 2912(%r10), %ymm12, %ymm12
vmovdqa 2688(%r10), %ymm0
vpsubw %ymm0, %ymm13, %ymm13
vpsubw 2944(%r10), %ymm13, %ymm13
vmovdqa 2720(%r10), %ymm1
vpsubw %ymm1, %ymm14, %ymm14
vpsubw 2976(%r10), %ymm14, %ymm14
vmovdqa 2752(%r10), %ymm2
vpsubw %ymm2, %ymm15, %ymm15
vpsubw 3008(%r10), %ymm15, %ymm15
vpaddw %ymm0, %ymm9, %ymm9
vmovdqa %ymm9, 2688(%r10)
vpaddw %ymm1, %ymm10, %ymm10
vmovdqa %ymm10, 2720(%r10)
vpaddw %ymm2, %ymm11, %ymm11
vmovdqa %ymm11, 2752(%r10)
vmovdqa %ymm12, 2784(%r10)
vpaddw %ymm3, %ymm13, %ymm13
vmovdqa %ymm13, 2816(%r10)
vpaddw %ymm4, %ymm14, %ymm14
vmovdqa %ymm14, 2848(%r10)
vpaddw %ymm5, %ymm15, %ymm15
vmovdqa %ymm15, 2880(%r10)
vmovdqa 0(%r9), %ymm0
vmovdqa 1024(%r9), %ymm4
vpaddw 256(%r9), %ymm0, %ymm0
vpaddw 1280(%r9), %ymm4, %ymm4
vmovdqa 32(%r9), %ymm1
vmovdqa 1056(%r9), %ymm5
vpaddw 288(%r9), %ymm1, %ymm1
vpaddw 1312(%r9), %ymm5, %ymm5
vmovdqa 64(%r9), %ymm2
vmovdqa 1088(%r9), %ymm6
vpaddw 320(%r9), %ymm2, %ymm2
vpaddw 1344(%r9), %ymm6, %ymm6
vmovdqa 96(%r9), %ymm3
vmovdqa 1120(%r9), %ymm7
vpaddw 352(%r9), %ymm3, %ymm3
vpaddw 1376(%r9), %ymm7, %ymm7
vpmullw %ymm0, %ymm4, %ymm12
vmovdqa %ymm12, 4096(%rsp)
vpmullw %ymm0, %ymm5, %ymm13
vpmullw %ymm1, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 4128(%rsp)
vpmullw %ymm0, %ymm6, %ymm12
vpmullw %ymm1, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm4, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 4160(%rsp)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm5, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 4192(%rsp)
vpmullw %ymm1, %ymm7, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 4224(%rsp)
vpmullw %ymm2, %ymm7, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 4256(%rsp)
vpmullw %ymm3, %ymm7, %ymm12
vmovdqa %ymm12, 4288(%rsp)
vmovdqa 128(%r9), %ymm0
vmovdqa 1152(%r9), %ymm4
vpaddw 384(%r9), %ymm0, %ymm0
vpaddw 1408(%r9), %ymm4, %ymm4
vmovdqa 160(%r9), %ymm1
vmovdqa 1184(%r9), %ymm5
vpaddw 416(%r9), %ymm1, %ymm1
vpaddw 1440(%r9), %ymm5, %ymm5
vmovdqa 192(%r9), %ymm2
vmovdqa 1216(%r9), %ymm6
vpaddw 448(%r9), %ymm2, %ymm2
vpaddw 1472(%r9), %ymm6, %ymm6
vmovdqa 224(%r9), %ymm3
vmovdqa 1248(%r9), %ymm7
vpaddw 480(%r9), %ymm3, %ymm3
vpaddw 1504(%r9), %ymm7, %ymm7
vpmullw %ymm0, %ymm4, %ymm12
vmovdqa %ymm12, 4352(%rsp)
vpmullw %ymm0, %ymm5, %ymm13
vpmullw %ymm1, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 4384(%rsp)
vpmullw %ymm0, %ymm6, %ymm12
vpmullw %ymm1, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm4, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 4416(%rsp)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm5, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 4448(%rsp)
vpmullw %ymm1, %ymm7, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 4480(%rsp)
vpmullw %ymm2, %ymm7, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 4512(%rsp)
vpmullw %ymm3, %ymm7, %ymm12
vmovdqa %ymm12, 4544(%rsp)
vpaddw 0(%r9), %ymm0, %ymm0
vpaddw 1024(%r9), %ymm4, %ymm4
vpaddw 256(%r9), %ymm0, %ymm0
vpaddw 1280(%r9), %ymm4, %ymm4
vpaddw 32(%r9), %ymm1, %ymm1
vpaddw 1056(%r9), %ymm5, %ymm5
vpaddw 288(%r9), %ymm1, %ymm1
vpaddw 1312(%r9), %ymm5, %ymm5
vpaddw 64(%r9), %ymm2, %ymm2
vpaddw 1088(%r9), %ymm6, %ymm6
vpaddw 320(%r9), %ymm2, %ymm2
vpaddw 1344(%r9), %ymm6, %ymm6
vpaddw 96(%r9), %ymm3, %ymm3
vpaddw 1120(%r9), %ymm7, %ymm7
vpaddw 352(%r9), %ymm3, %ymm3
vpaddw 1376(%r9), %ymm7, %ymm7
vpmullw %ymm0, %ymm4, %ymm9
vpmullw %ymm0, %ymm5, %ymm10
vpmullw %ymm1, %ymm4, %ymm8
vpaddw %ymm8, %ymm10, %ymm10
vpmullw %ymm0, %ymm6, %ymm11
vpmullw %ymm1, %ymm5, %ymm8
vpaddw %ymm8, %ymm11, %ymm11
vpmullw %ymm2, %ymm4, %ymm8
vpaddw %ymm8, %ymm11, %ymm11
vpmullw %ymm0, %ymm7, %ymm12
vpmullw %ymm1, %ymm6, %ymm8
vpaddw %ymm8, %ymm12, %ymm12
vpmullw %ymm2, %ymm5, %ymm8
vpaddw %ymm8, %ymm12, %ymm12
vpmullw %ymm3, %ymm4, %ymm8
vpaddw %ymm8, %ymm12, %ymm12
vpmullw %ymm1, %ymm7, %ymm13
vpmullw %ymm2, %ymm6, %ymm8
vpaddw %ymm8, %ymm13, %ymm13
vpmullw %ymm3, %ymm5, %ymm8
vpaddw %ymm8, %ymm13, %ymm13
vpmullw %ymm2, %ymm7, %ymm14
vpmullw %ymm3, %ymm6, %ymm8
vpaddw %ymm8, %ymm14, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpsubw 4096(%rsp), %ymm9, %ymm9
vmovdqa 4352(%rsp), %ymm3
vpsubw %ymm3, %ymm9, %ymm9
vpsubw 4128(%rsp), %ymm10, %ymm10
vmovdqa 4384(%rsp), %ymm4
vpsubw %ymm4, %ymm10, %ymm10
vpsubw 4160(%rsp), %ymm11, %ymm11
vmovdqa 4416(%rsp), %ymm5
vpsubw %ymm5, %ymm11, %ymm11
vpsubw 4192(%rsp), %ymm12, %ymm12
vpsubw 4448(%rsp), %ymm12, %ymm12
vmovdqa 4224(%rsp), %ymm0
vpsubw %ymm0, %ymm13, %ymm13
vpsubw 4480(%rsp), %ymm13, %ymm13
vmovdqa 4256(%rsp), %ymm1
vpsubw %ymm1, %ymm14, %ymm14
vpsubw 4512(%rsp), %ymm14, %ymm14
vmovdqa 4288(%rsp), %ymm2
vpsubw %ymm2, %ymm15, %ymm15
vpsubw 4544(%rsp), %ymm15, %ymm15
vpaddw %ymm0, %ymm9, %ymm9
vmovdqa %ymm9, 4224(%rsp)
vpaddw %ymm1, %ymm10, %ymm10
vmovdqa %ymm10, 4256(%rsp)
vpaddw %ymm2, %ymm11, %ymm11
vmovdqa %ymm11, 4288(%rsp)
vmovdqa %ymm12, 4320(%rsp)
vpaddw %ymm3, %ymm13, %ymm13
vmovdqa %ymm13, 4352(%rsp)
vpaddw %ymm4, %ymm14, %ymm14
vmovdqa %ymm14, 4384(%rsp)
vpaddw %ymm5, %ymm15, %ymm15
vmovdqa %ymm15, 4416(%rsp)
vmovdqa 4320(%rsp), %ymm0
vpsubw 2272(%r10), %ymm0, %ymm0
vpsubw 2784(%r10), %ymm0, %ymm0
vmovdqa %ymm0, 2528(%r10)
vmovdqa 2304(%r10), %ymm0
vpsubw 2560(%r10), %ymm0, %ymm0
vmovdqa 4352(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 2816(%r10), %ymm1, %ymm1
vpsubw 2048(%r10), %ymm0, %ymm0
vpaddw 4096(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2304(%r10)
vmovdqa %ymm1, 2560(%r10)
vmovdqa 2336(%r10), %ymm0
vpsubw 2592(%r10), %ymm0, %ymm0
vmovdqa 4384(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 2848(%r10), %ymm1, %ymm1
vpsubw 2080(%r10), %ymm0, %ymm0
vpaddw 4128(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2336(%r10)
vmovdqa %ymm1, 2592(%r10)
vmovdqa 2368(%r10), %ymm0
vpsubw 2624(%r10), %ymm0, %ymm0
vmovdqa 4416(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 2880(%r10), %ymm1, %ymm1
vpsubw 2112(%r10), %ymm0, %ymm0
vpaddw 4160(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2368(%r10)
vmovdqa %ymm1, 2624(%r10)
vmovdqa 2400(%r10), %ymm0
vpsubw 2656(%r10), %ymm0, %ymm0
vmovdqa 4448(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 2912(%r10), %ymm1, %ymm1
vpsubw 2144(%r10), %ymm0, %ymm0
vpaddw 4192(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2400(%r10)
vmovdqa %ymm1, 2656(%r10)
vmovdqa 2432(%r10), %ymm0
vpsubw 2688(%r10), %ymm0, %ymm0
vmovdqa 4480(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 2944(%r10), %ymm1, %ymm1
vpsubw 2176(%r10), %ymm0, %ymm0
vpaddw 4224(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2432(%r10)
vmovdqa %ymm1, 2688(%r10)
vmovdqa 2464(%r10), %ymm0
vpsubw 2720(%r10), %ymm0, %ymm0
vmovdqa 4512(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 2976(%r10), %ymm1, %ymm1
vpsubw 2208(%r10), %ymm0, %ymm0
vpaddw 4256(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2464(%r10)
vmovdqa %ymm1, 2720(%r10)
vmovdqa 2496(%r10), %ymm0
vpsubw 2752(%r10), %ymm0, %ymm0
vmovdqa 4544(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 3008(%r10), %ymm1, %ymm1
vpsubw 2240(%r10), %ymm0, %ymm0
vpaddw 4288(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2496(%r10)
vmovdqa %ymm1, 2752(%r10)
neg %ecx
jns done_1
add $512, %r9
add $1024, %r10
jmp innerloop_1
done_1:
sub $512, %r9
sub $1024, %r10
vmovdqa 0(%r9), %ymm0
vpaddw 512(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 4608(%rsp)
vmovdqa 1024(%r9), %ymm0
vpaddw 1536(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5120(%rsp)
vmovdqa 32(%r9), %ymm0
vpaddw 544(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 4640(%rsp)
vmovdqa 1056(%r9), %ymm0
vpaddw 1568(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5152(%rsp)
vmovdqa 64(%r9), %ymm0
vpaddw 576(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 4672(%rsp)
vmovdqa 1088(%r9), %ymm0
vpaddw 1600(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5184(%rsp)
vmovdqa 96(%r9), %ymm0
vpaddw 608(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 4704(%rsp)
vmovdqa 1120(%r9), %ymm0
vpaddw 1632(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5216(%rsp)
vmovdqa 128(%r9), %ymm0
vpaddw 640(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 4736(%rsp)
vmovdqa 1152(%r9), %ymm0
vpaddw 1664(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5248(%rsp)
vmovdqa 160(%r9), %ymm0
vpaddw 672(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 4768(%rsp)
vmovdqa 1184(%r9), %ymm0
vpaddw 1696(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5280(%rsp)
vmovdqa 192(%r9), %ymm0
vpaddw 704(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 4800(%rsp)
vmovdqa 1216(%r9), %ymm0
vpaddw 1728(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5312(%rsp)
vmovdqa 224(%r9), %ymm0
vpaddw 736(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 4832(%rsp)
vmovdqa 1248(%r9), %ymm0
vpaddw 1760(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5344(%rsp)
vmovdqa 256(%r9), %ymm0
vpaddw 768(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 4864(%rsp)
vmovdqa 1280(%r9), %ymm0
vpaddw 1792(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5376(%rsp)
vmovdqa 288(%r9), %ymm0
vpaddw 800(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 4896(%rsp)
vmovdqa 1312(%r9), %ymm0
vpaddw 1824(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5408(%rsp)
vmovdqa 320(%r9), %ymm0
vpaddw 832(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 4928(%rsp)
vmovdqa 1344(%r9), %ymm0
vpaddw 1856(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5440(%rsp)
vmovdqa 352(%r9), %ymm0
vpaddw 864(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 4960(%rsp)
vmovdqa 1376(%r9), %ymm0
vpaddw 1888(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5472(%rsp)
vmovdqa 384(%r9), %ymm0
vpaddw 896(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 4992(%rsp)
vmovdqa 1408(%r9), %ymm0
vpaddw 1920(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5504(%rsp)
vmovdqa 416(%r9), %ymm0
vpaddw 928(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5024(%rsp)
vmovdqa 1440(%r9), %ymm0
vpaddw 1952(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5536(%rsp)
vmovdqa 448(%r9), %ymm0
vpaddw 960(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5056(%rsp)
vmovdqa 1472(%r9), %ymm0
vpaddw 1984(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5568(%rsp)
vmovdqa 480(%r9), %ymm0
vpaddw 992(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5088(%rsp)
vmovdqa 1504(%r9), %ymm0
vpaddw 2016(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 5600(%rsp)
vmovdqa 4608(%rsp), %ymm0
vmovdqa 5120(%rsp), %ymm4
vmovdqa 4640(%rsp), %ymm1
vmovdqa 5152(%rsp), %ymm5
vmovdqa 4672(%rsp), %ymm2
vmovdqa 5184(%rsp), %ymm6
vmovdqa 4704(%rsp), %ymm3
vmovdqa 5216(%rsp), %ymm7
vpmullw %ymm0, %ymm4, %ymm12
vmovdqa %ymm12, 5632(%rsp)
vpmullw %ymm0, %ymm5, %ymm13
vpmullw %ymm1, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 5664(%rsp)
vpmullw %ymm0, %ymm6, %ymm12
vpmullw %ymm1, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm4, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 5696(%rsp)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm5, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 5728(%rsp)
vpmullw %ymm1, %ymm7, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 5760(%rsp)
vpmullw %ymm2, %ymm7, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 5792(%rsp)
vpmullw %ymm3, %ymm7, %ymm12
vmovdqa %ymm12, 5824(%rsp)
vmovdqa 4736(%rsp), %ymm0
vmovdqa 5248(%rsp), %ymm4
vmovdqa 4768(%rsp), %ymm1
vmovdqa 5280(%rsp), %ymm5
vmovdqa 4800(%rsp), %ymm2
vmovdqa 5312(%rsp), %ymm6
vmovdqa 4832(%rsp), %ymm3
vmovdqa 5344(%rsp), %ymm7
vpmullw %ymm0, %ymm4, %ymm12
vmovdqa %ymm12, 5888(%rsp)
vpmullw %ymm0, %ymm5, %ymm13
vpmullw %ymm1, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 5920(%rsp)
vpmullw %ymm0, %ymm6, %ymm12
vpmullw %ymm1, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm4, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 5952(%rsp)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm5, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 5984(%rsp)
vpmullw %ymm1, %ymm7, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 6016(%rsp)
vpmullw %ymm2, %ymm7, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6048(%rsp)
vpmullw %ymm3, %ymm7, %ymm12
vmovdqa %ymm12, 6080(%rsp)
vpaddw 4608(%rsp), %ymm0, %ymm0
vpaddw 5120(%rsp), %ymm4, %ymm4
vpaddw 4640(%rsp), %ymm1, %ymm1
vpaddw 5152(%rsp), %ymm5, %ymm5
vpaddw 4672(%rsp), %ymm2, %ymm2
vpaddw 5184(%rsp), %ymm6, %ymm6
vpaddw 4704(%rsp), %ymm3, %ymm3
vpaddw 5216(%rsp), %ymm7, %ymm7
vpmullw %ymm0, %ymm4, %ymm9
vpmullw %ymm0, %ymm5, %ymm10
vpmullw %ymm1, %ymm4, %ymm8
vpaddw %ymm8, %ymm10, %ymm10
vpmullw %ymm0, %ymm6, %ymm11
vpmullw %ymm1, %ymm5, %ymm8
vpaddw %ymm8, %ymm11, %ymm11
vpmullw %ymm2, %ymm4, %ymm8
vpaddw %ymm8, %ymm11, %ymm11
vpmullw %ymm0, %ymm7, %ymm12
vpmullw %ymm1, %ymm6, %ymm8
vpaddw %ymm8, %ymm12, %ymm12
vpmullw %ymm2, %ymm5, %ymm8
vpaddw %ymm8, %ymm12, %ymm12
vpmullw %ymm3, %ymm4, %ymm8
vpaddw %ymm8, %ymm12, %ymm12
vpmullw %ymm1, %ymm7, %ymm13
vpmullw %ymm2, %ymm6, %ymm8
vpaddw %ymm8, %ymm13, %ymm13
vpmullw %ymm3, %ymm5, %ymm8
vpaddw %ymm8, %ymm13, %ymm13
vpmullw %ymm2, %ymm7, %ymm14
vpmullw %ymm3, %ymm6, %ymm8
vpaddw %ymm8, %ymm14, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpsubw 5632(%rsp), %ymm9, %ymm9
vmovdqa 5888(%rsp), %ymm3
vpsubw %ymm3, %ymm9, %ymm9
vpsubw 5664(%rsp), %ymm10, %ymm10
vmovdqa 5920(%rsp), %ymm4
vpsubw %ymm4, %ymm10, %ymm10
vpsubw 5696(%rsp), %ymm11, %ymm11
vmovdqa 5952(%rsp), %ymm5
vpsubw %ymm5, %ymm11, %ymm11
vpsubw 5728(%rsp), %ymm12, %ymm12
vpsubw 5984(%rsp), %ymm12, %ymm12
vmovdqa 5760(%rsp), %ymm0
vpsubw %ymm0, %ymm13, %ymm13
vpsubw 6016(%rsp), %ymm13, %ymm13
vmovdqa 5792(%rsp), %ymm1
vpsubw %ymm1, %ymm14, %ymm14
vpsubw 6048(%rsp), %ymm14, %ymm14
vmovdqa 5824(%rsp), %ymm2
vpsubw %ymm2, %ymm15, %ymm15
vpsubw 6080(%rsp), %ymm15, %ymm15
vpaddw %ymm0, %ymm9, %ymm9
vmovdqa %ymm9, 5760(%rsp)
vpaddw %ymm1, %ymm10, %ymm10
vmovdqa %ymm10, 5792(%rsp)
vpaddw %ymm2, %ymm11, %ymm11
vmovdqa %ymm11, 5824(%rsp)
vmovdqa %ymm12, 5856(%rsp)
vpaddw %ymm3, %ymm13, %ymm13
vmovdqa %ymm13, 5888(%rsp)
vpaddw %ymm4, %ymm14, %ymm14
vmovdqa %ymm14, 5920(%rsp)
vpaddw %ymm5, %ymm15, %ymm15
vmovdqa %ymm15, 5952(%rsp)
vmovdqa 4864(%rsp), %ymm0
vmovdqa 5376(%rsp), %ymm4
vmovdqa 4896(%rsp), %ymm1
vmovdqa 5408(%rsp), %ymm5
vmovdqa 4928(%rsp), %ymm2
vmovdqa 5440(%rsp), %ymm6
vmovdqa 4960(%rsp), %ymm3
vmovdqa 5472(%rsp), %ymm7
vpmullw %ymm0, %ymm4, %ymm12
vmovdqa %ymm12, 6144(%rsp)
vpmullw %ymm0, %ymm5, %ymm13
vpmullw %ymm1, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6176(%rsp)
vpmullw %ymm0, %ymm6, %ymm12
vpmullw %ymm1, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm4, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 6208(%rsp)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm5, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6240(%rsp)
vpmullw %ymm1, %ymm7, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 6272(%rsp)
vpmullw %ymm2, %ymm7, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6304(%rsp)
vpmullw %ymm3, %ymm7, %ymm12
vmovdqa %ymm12, 6336(%rsp)
vmovdqa 4992(%rsp), %ymm0
vmovdqa 5504(%rsp), %ymm4
vmovdqa 5024(%rsp), %ymm1
vmovdqa 5536(%rsp), %ymm5
vmovdqa 5056(%rsp), %ymm2
vmovdqa 5568(%rsp), %ymm6
vmovdqa 5088(%rsp), %ymm3
vmovdqa 5600(%rsp), %ymm7
vpmullw %ymm0, %ymm4, %ymm12
vmovdqa %ymm12, 6400(%rsp)
vpmullw %ymm0, %ymm5, %ymm13
vpmullw %ymm1, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6432(%rsp)
vpmullw %ymm0, %ymm6, %ymm12
vpmullw %ymm1, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm4, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 6464(%rsp)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm5, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6496(%rsp)
vpmullw %ymm1, %ymm7, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 6528(%rsp)
vpmullw %ymm2, %ymm7, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6560(%rsp)
vpmullw %ymm3, %ymm7, %ymm12
vmovdqa %ymm12, 6592(%rsp)
vpaddw 4864(%rsp), %ymm0, %ymm0
vpaddw 5376(%rsp), %ymm4, %ymm4
vpaddw 4896(%rsp), %ymm1, %ymm1
vpaddw 5408(%rsp), %ymm5, %ymm5
vpaddw 4928(%rsp), %ymm2, %ymm2
vpaddw 5440(%rsp), %ymm6, %ymm6
vpaddw 4960(%rsp), %ymm3, %ymm3
vpaddw 5472(%rsp), %ymm7, %ymm7
vpmullw %ymm0, %ymm4, %ymm9
vpmullw %ymm0, %ymm5, %ymm10
vpmullw %ymm1, %ymm4, %ymm8
vpaddw %ymm8, %ymm10, %ymm10
vpmullw %ymm0, %ymm6, %ymm11
vpmullw %ymm1, %ymm5, %ymm8
vpaddw %ymm8, %ymm11, %ymm11
vpmullw %ymm2, %ymm4, %ymm8
vpaddw %ymm8, %ymm11, %ymm11
vpmullw %ymm0, %ymm7, %ymm12
vpmullw %ymm1, %ymm6, %ymm8
vpaddw %ymm8, %ymm12, %ymm12
vpmullw %ymm2, %ymm5, %ymm8
vpaddw %ymm8, %ymm12, %ymm12
vpmullw %ymm3, %ymm4, %ymm8
vpaddw %ymm8, %ymm12, %ymm12
vpmullw %ymm1, %ymm7, %ymm13
vpmullw %ymm2, %ymm6, %ymm8
vpaddw %ymm8, %ymm13, %ymm13
vpmullw %ymm3, %ymm5, %ymm8
vpaddw %ymm8, %ymm13, %ymm13
vpmullw %ymm2, %ymm7, %ymm14
vpmullw %ymm3, %ymm6, %ymm8
vpaddw %ymm8, %ymm14, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpsubw 6144(%rsp), %ymm9, %ymm9
vmovdqa 6400(%rsp), %ymm3
vpsubw %ymm3, %ymm9, %ymm9
vpsubw 6176(%rsp), %ymm10, %ymm10
vmovdqa 6432(%rsp), %ymm4
vpsubw %ymm4, %ymm10, %ymm10
vpsubw 6208(%rsp), %ymm11, %ymm11
vmovdqa 6464(%rsp), %ymm5
vpsubw %ymm5, %ymm11, %ymm11
vpsubw 6240(%rsp), %ymm12, %ymm12
vpsubw 6496(%rsp), %ymm12, %ymm12
vmovdqa 6272(%rsp), %ymm0
vpsubw %ymm0, %ymm13, %ymm13
vpsubw 6528(%rsp), %ymm13, %ymm13
vmovdqa 6304(%rsp), %ymm1
vpsubw %ymm1, %ymm14, %ymm14
vpsubw 6560(%rsp), %ymm14, %ymm14
vmovdqa 6336(%rsp), %ymm2
vpsubw %ymm2, %ymm15, %ymm15
vpsubw 6592(%rsp), %ymm15, %ymm15
vpaddw %ymm0, %ymm9, %ymm9
vmovdqa %ymm9, 6272(%rsp)
vpaddw %ymm1, %ymm10, %ymm10
vmovdqa %ymm10, 6304(%rsp)
vpaddw %ymm2, %ymm11, %ymm11
vmovdqa %ymm11, 6336(%rsp)
vmovdqa %ymm12, 6368(%rsp)
vpaddw %ymm3, %ymm13, %ymm13
vmovdqa %ymm13, 6400(%rsp)
vpaddw %ymm4, %ymm14, %ymm14
vmovdqa %ymm14, 6432(%rsp)
vpaddw %ymm5, %ymm15, %ymm15
vmovdqa %ymm15, 6464(%rsp)
vmovdqa 4608(%rsp), %ymm0
vmovdqa 5120(%rsp), %ymm4
vpaddw 4864(%rsp), %ymm0, %ymm0
vpaddw 5376(%rsp), %ymm4, %ymm4
vmovdqa 4640(%rsp), %ymm1
vmovdqa 5152(%rsp), %ymm5
vpaddw 4896(%rsp), %ymm1, %ymm1
vpaddw 5408(%rsp), %ymm5, %ymm5
vmovdqa 4672(%rsp), %ymm2
vmovdqa 5184(%rsp), %ymm6
vpaddw 4928(%rsp), %ymm2, %ymm2
vpaddw 5440(%rsp), %ymm6, %ymm6
vmovdqa 4704(%rsp), %ymm3
vmovdqa 5216(%rsp), %ymm7
vpaddw 4960(%rsp), %ymm3, %ymm3
vpaddw 5472(%rsp), %ymm7, %ymm7
vpmullw %ymm0, %ymm4, %ymm12
vmovdqa %ymm12, 4096(%rsp)
vpmullw %ymm0, %ymm5, %ymm13
vpmullw %ymm1, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 4128(%rsp)
vpmullw %ymm0, %ymm6, %ymm12
vpmullw %ymm1, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm4, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 4160(%rsp)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm5, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 4192(%rsp)
vpmullw %ymm1, %ymm7, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 4224(%rsp)
vpmullw %ymm2, %ymm7, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 4256(%rsp)
vpmullw %ymm3, %ymm7, %ymm12
vmovdqa %ymm12, 4288(%rsp)
vmovdqa 4736(%rsp), %ymm0
vmovdqa 5248(%rsp), %ymm4
vpaddw 4992(%rsp), %ymm0, %ymm0
vpaddw 5504(%rsp), %ymm4, %ymm4
vmovdqa 4768(%rsp), %ymm1
vmovdqa 5280(%rsp), %ymm5
vpaddw 5024(%rsp), %ymm1, %ymm1
vpaddw 5536(%rsp), %ymm5, %ymm5
vmovdqa 4800(%rsp), %ymm2
vmovdqa 5312(%rsp), %ymm6
vpaddw 5056(%rsp), %ymm2, %ymm2
vpaddw 5568(%rsp), %ymm6, %ymm6
vmovdqa 4832(%rsp), %ymm3
vmovdqa 5344(%rsp), %ymm7
vpaddw 5088(%rsp), %ymm3, %ymm3
vpaddw 5600(%rsp), %ymm7, %ymm7
vpmullw %ymm0, %ymm4, %ymm12
vmovdqa %ymm12, 4352(%rsp)
vpmullw %ymm0, %ymm5, %ymm13
vpmullw %ymm1, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 4384(%rsp)
vpmullw %ymm0, %ymm6, %ymm12
vpmullw %ymm1, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm4, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 4416(%rsp)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm5, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm4, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 4448(%rsp)
vpmullw %ymm1, %ymm7, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm5, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 4480(%rsp)
vpmullw %ymm2, %ymm7, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 4512(%rsp)
vpmullw %ymm3, %ymm7, %ymm12
vmovdqa %ymm12, 4544(%rsp)
vpaddw 4608(%rsp), %ymm0, %ymm0
vpaddw 5120(%rsp), %ymm4, %ymm4
vpaddw 4864(%rsp), %ymm0, %ymm0
vpaddw 5376(%rsp), %ymm4, %ymm4
vpaddw 4640(%rsp), %ymm1, %ymm1
vpaddw 5152(%rsp), %ymm5, %ymm5
vpaddw 4896(%rsp), %ymm1, %ymm1
vpaddw 5408(%rsp), %ymm5, %ymm5
vpaddw 4672(%rsp), %ymm2, %ymm2
vpaddw 5184(%rsp), %ymm6, %ymm6
vpaddw 4928(%rsp), %ymm2, %ymm2
vpaddw 5440(%rsp), %ymm6, %ymm6
vpaddw 4704(%rsp), %ymm3, %ymm3
vpaddw 5216(%rsp), %ymm7, %ymm7
vpaddw 4960(%rsp), %ymm3, %ymm3
vpaddw 5472(%rsp), %ymm7, %ymm7
vpmullw %ymm0, %ymm4, %ymm9
vpmullw %ymm0, %ymm5, %ymm10
vpmullw %ymm1, %ymm4, %ymm8
vpaddw %ymm8, %ymm10, %ymm10
vpmullw %ymm0, %ymm6, %ymm11
vpmullw %ymm1, %ymm5, %ymm8
vpaddw %ymm8, %ymm11, %ymm11
vpmullw %ymm2, %ymm4, %ymm8
vpaddw %ymm8, %ymm11, %ymm11
vpmullw %ymm0, %ymm7, %ymm12
vpmullw %ymm1, %ymm6, %ymm8
vpaddw %ymm8, %ymm12, %ymm12
vpmullw %ymm2, %ymm5, %ymm8
vpaddw %ymm8, %ymm12, %ymm12
vpmullw %ymm3, %ymm4, %ymm8
vpaddw %ymm8, %ymm12, %ymm12
vpmullw %ymm1, %ymm7, %ymm13
vpmullw %ymm2, %ymm6, %ymm8
vpaddw %ymm8, %ymm13, %ymm13
vpmullw %ymm3, %ymm5, %ymm8
vpaddw %ymm8, %ymm13, %ymm13
vpmullw %ymm2, %ymm7, %ymm14
vpmullw %ymm3, %ymm6, %ymm8
vpaddw %ymm8, %ymm14, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpsubw 4096(%rsp), %ymm9, %ymm9
vmovdqa 4352(%rsp), %ymm3
vpsubw %ymm3, %ymm9, %ymm9
vpsubw 4128(%rsp), %ymm10, %ymm10
vmovdqa 4384(%rsp), %ymm4
vpsubw %ymm4, %ymm10, %ymm10
vpsubw 4160(%rsp), %ymm11, %ymm11
vmovdqa 4416(%rsp), %ymm5
vpsubw %ymm5, %ymm11, %ymm11
vpsubw 4192(%rsp), %ymm12, %ymm12
vpsubw 4448(%rsp), %ymm12, %ymm12
vmovdqa 4224(%rsp), %ymm0
vpsubw %ymm0, %ymm13, %ymm13
vpsubw 4480(%rsp), %ymm13, %ymm13
vmovdqa 4256(%rsp), %ymm1
vpsubw %ymm1, %ymm14, %ymm14
vpsubw 4512(%rsp), %ymm14, %ymm14
vmovdqa 4288(%rsp), %ymm2
vpsubw %ymm2, %ymm15, %ymm15
vpsubw 4544(%rsp), %ymm15, %ymm15
vpaddw %ymm0, %ymm9, %ymm9
vmovdqa %ymm9, 4224(%rsp)
vpaddw %ymm1, %ymm10, %ymm10
vmovdqa %ymm10, 4256(%rsp)
vpaddw %ymm2, %ymm11, %ymm11
vmovdqa %ymm11, 4288(%rsp)
vmovdqa %ymm12, 4320(%rsp)
vpaddw %ymm3, %ymm13, %ymm13
vmovdqa %ymm13, 4352(%rsp)
vpaddw %ymm4, %ymm14, %ymm14
vmovdqa %ymm14, 4384(%rsp)
vpaddw %ymm5, %ymm15, %ymm15
vmovdqa %ymm15, 4416(%rsp)
vmovdqa 5888(%rsp), %ymm0
vpsubw 6144(%rsp), %ymm0, %ymm0
vmovdqa 4352(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6400(%rsp), %ymm1, %ymm6
vpsubw 5632(%rsp), %ymm0, %ymm0
vpaddw 4096(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 5888(%rsp)
vmovdqa 5920(%rsp), %ymm0
vpsubw 6176(%rsp), %ymm0, %ymm0
vmovdqa 4384(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6432(%rsp), %ymm1, %ymm7
vpsubw 5664(%rsp), %ymm0, %ymm0
vpaddw 4128(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 5920(%rsp)
vmovdqa 5952(%rsp), %ymm0
vpsubw 6208(%rsp), %ymm0, %ymm0
vmovdqa 4416(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6464(%rsp), %ymm1, %ymm8
vpsubw 5696(%rsp), %ymm0, %ymm0
vpaddw 4160(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 5952(%rsp)
vmovdqa 5984(%rsp), %ymm0
vpsubw 6240(%rsp), %ymm0, %ymm0
vmovdqa 4448(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6496(%rsp), %ymm1, %ymm9
vpsubw 5728(%rsp), %ymm0, %ymm0
vpaddw 4192(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 5984(%rsp)
vmovdqa 6016(%rsp), %ymm0
vpsubw 6272(%rsp), %ymm0, %ymm0
vmovdqa 4480(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6528(%rsp), %ymm1, %ymm10
vpsubw 5760(%rsp), %ymm0, %ymm0
vpaddw 4224(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 6016(%rsp)
vmovdqa 6048(%rsp), %ymm0
vpsubw 6304(%rsp), %ymm0, %ymm0
vmovdqa 4512(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6560(%rsp), %ymm1, %ymm11
vpsubw 5792(%rsp), %ymm0, %ymm0
vpaddw 4256(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 6048(%rsp)
vmovdqa 6080(%rsp), %ymm0
vpsubw 6336(%rsp), %ymm0, %ymm0
vmovdqa 4544(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6592(%rsp), %ymm1, %ymm12
vpsubw 5824(%rsp), %ymm0, %ymm0
vpaddw 4288(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 6080(%rsp)
vmovdqa 4320(%rsp), %ymm0
vpsubw 5856(%rsp), %ymm0, %ymm0
vpsubw 6368(%rsp), %ymm0, %ymm0
vpsubw 2528(%r10), %ymm0, %ymm0
vpsubw 3552(%r10), %ymm0, %ymm0
vmovdqa %ymm0, 3040(%r10)
vmovdqa 2560(%r10), %ymm0
vpsubw 3072(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm6, %ymm6
vpsubw 3584(%r10), %ymm6, %ymm6
vpsubw 2048(%r10), %ymm0, %ymm0
vpaddw 5632(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2560(%r10)
vmovdqa %ymm6, 3072(%r10)
vmovdqa 2592(%r10), %ymm0
vpsubw 3104(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm7, %ymm7
vpsubw 3616(%r10), %ymm7, %ymm7
vpsubw 2080(%r10), %ymm0, %ymm0
vpaddw 5664(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2592(%r10)
vmovdqa %ymm7, 3104(%r10)
vmovdqa 2624(%r10), %ymm0
vpsubw 3136(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm8, %ymm8
vpsubw 3648(%r10), %ymm8, %ymm8
vpsubw 2112(%r10), %ymm0, %ymm0
vpaddw 5696(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2624(%r10)
vmovdqa %ymm8, 3136(%r10)
vmovdqa 2656(%r10), %ymm0
vpsubw 3168(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm9, %ymm9
vpsubw 3680(%r10), %ymm9, %ymm9
vpsubw 2144(%r10), %ymm0, %ymm0
vpaddw 5728(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2656(%r10)
vmovdqa %ymm9, 3168(%r10)
vmovdqa 2688(%r10), %ymm0
vpsubw 3200(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm10, %ymm10
vpsubw 3712(%r10), %ymm10, %ymm10
vpsubw 2176(%r10), %ymm0, %ymm0
vpaddw 5760(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2688(%r10)
vmovdqa %ymm10, 3200(%r10)
vmovdqa 2720(%r10), %ymm0
vpsubw 3232(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm11, %ymm11
vpsubw 3744(%r10), %ymm11, %ymm11
vpsubw 2208(%r10), %ymm0, %ymm0
vpaddw 5792(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2720(%r10)
vmovdqa %ymm11, 3232(%r10)
vmovdqa 2752(%r10), %ymm0
vpsubw 3264(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm12, %ymm12
vpsubw 3776(%r10), %ymm12, %ymm12
vpsubw 2240(%r10), %ymm0, %ymm0
vpaddw 5824(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2752(%r10)
vmovdqa %ymm12, 3264(%r10)
vmovdqa 2784(%r10), %ymm0
vpsubw 3296(%r10), %ymm0, %ymm0
vmovdqa 6368(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 3808(%r10), %ymm1, %ymm1
vpsubw 2272(%r10), %ymm0, %ymm0
vpaddw 5856(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2784(%r10)
vmovdqa %ymm1, 3296(%r10)
vmovdqa 2816(%r10), %ymm0
vpsubw 3328(%r10), %ymm0, %ymm0
vmovdqa 6400(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 3840(%r10), %ymm1, %ymm1
vpsubw 2304(%r10), %ymm0, %ymm0
vpaddw 5888(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2816(%r10)
vmovdqa %ymm1, 3328(%r10)
vmovdqa 2848(%r10), %ymm0
vpsubw 3360(%r10), %ymm0, %ymm0
vmovdqa 6432(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 3872(%r10), %ymm1, %ymm1
vpsubw 2336(%r10), %ymm0, %ymm0
vpaddw 5920(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2848(%r10)
vmovdqa %ymm1, 3360(%r10)
vmovdqa 2880(%r10), %ymm0
vpsubw 3392(%r10), %ymm0, %ymm0
vmovdqa 6464(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 3904(%r10), %ymm1, %ymm1
vpsubw 2368(%r10), %ymm0, %ymm0
vpaddw 5952(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2880(%r10)
vmovdqa %ymm1, 3392(%r10)
vmovdqa 2912(%r10), %ymm0
vpsubw 3424(%r10), %ymm0, %ymm0
vmovdqa 6496(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 3936(%r10), %ymm1, %ymm1
vpsubw 2400(%r10), %ymm0, %ymm0
vpaddw 5984(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2912(%r10)
vmovdqa %ymm1, 3424(%r10)
vmovdqa 2944(%r10), %ymm0
vpsubw 3456(%r10), %ymm0, %ymm0
vmovdqa 6528(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 3968(%r10), %ymm1, %ymm1
vpsubw 2432(%r10), %ymm0, %ymm0
vpaddw 6016(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2944(%r10)
vmovdqa %ymm1, 3456(%r10)
vmovdqa 2976(%r10), %ymm0
vpsubw 3488(%r10), %ymm0, %ymm0
vmovdqa 6560(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4000(%r10), %ymm1, %ymm1
vpsubw 2464(%r10), %ymm0, %ymm0
vpaddw 6048(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 2976(%r10)
vmovdqa %ymm1, 3488(%r10)
vmovdqa 3008(%r10), %ymm0
vpsubw 3520(%r10), %ymm0, %ymm0
vmovdqa 6592(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4032(%r10), %ymm1, %ymm1
vpsubw 2496(%r10), %ymm0, %ymm0
vpaddw 6080(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3008(%r10)
vmovdqa %ymm1, 3520(%r10)
vpxor %ymm1, %ymm1, %ymm1
vmovdqa %ymm1, 4064(%r10)
subq $32, %rsp
vmovdqa 2048(%r10), %ymm0
vmovdqa 2112(%r10), %ymm1
vmovdqa 2176(%r10), %ymm2
vmovdqa 2240(%r10), %ymm3
vpunpcklwd 2080(%r10), %ymm0, %ymm4
vpunpckhwd 2080(%r10), %ymm0, %ymm5
vpunpcklwd 2144(%r10), %ymm1, %ymm6
vpunpckhwd 2144(%r10), %ymm1, %ymm7
vpunpcklwd 2208(%r10), %ymm2, %ymm8
vpunpckhwd 2208(%r10), %ymm2, %ymm9
vpunpcklwd 2272(%r10), %ymm3, %ymm10
vpunpckhwd 2272(%r10), %ymm3, %ymm11
vpunpckldq %ymm6, %ymm4, %ymm0
vpunpckhdq %ymm6, %ymm4, %ymm1
vpunpckldq %ymm7, %ymm5, %ymm2
vpunpckhdq %ymm7, %ymm5, %ymm3
vpunpckldq %ymm10, %ymm8, %ymm12
vpunpckhdq %ymm10, %ymm8, %ymm13
vpunpckldq %ymm11, %ymm9, %ymm14
vpunpckhdq %ymm11, %ymm9, %ymm15
vpunpcklqdq %ymm12, %ymm0, %ymm4
vpunpckhqdq %ymm12, %ymm0, %ymm5
vpunpcklqdq %ymm13, %ymm1, %ymm6
vpunpckhqdq %ymm13, %ymm1, %ymm7
vpunpcklqdq %ymm14, %ymm2, %ymm8
vpunpckhqdq %ymm14, %ymm2, %ymm9
vpunpcklqdq %ymm15, %ymm3, %ymm10
vpunpckhqdq %ymm15, %ymm3, %ymm11
vmovdqa 2304(%r10), %ymm0
vmovdqa 2368(%r10), %ymm1
vmovdqa 2432(%r10), %ymm2
vmovdqa 2496(%r10), %ymm3
vpunpcklwd 2336(%r10), %ymm0, %ymm12
vpunpckhwd 2336(%r10), %ymm0, %ymm13
vpunpcklwd 2400(%r10), %ymm1, %ymm14
vpunpckhwd 2400(%r10), %ymm1, %ymm15
vpunpcklwd 2464(%r10), %ymm2, %ymm0
vpunpckhwd 2464(%r10), %ymm2, %ymm1
vpunpcklwd 2528(%r10), %ymm3, %ymm2
vpunpckhwd 2528(%r10), %ymm3, %ymm3
vmovdqa %ymm11, 0(%rsp)
vpunpckldq %ymm14, %ymm12, %ymm11
vpunpckhdq %ymm14, %ymm12, %ymm12
vpunpckldq %ymm15, %ymm13, %ymm14
vpunpckhdq %ymm15, %ymm13, %ymm15
vpunpckldq %ymm2, %ymm0, %ymm13
vpunpckhdq %ymm2, %ymm0, %ymm0
vpunpckldq %ymm3, %ymm1, %ymm2
vpunpckhdq %ymm3, %ymm1, %ymm1
vpunpcklqdq %ymm13, %ymm11, %ymm3
vpunpckhqdq %ymm13, %ymm11, %ymm13
vpunpcklqdq %ymm0, %ymm12, %ymm11
vpunpckhqdq %ymm0, %ymm12, %ymm0
vpunpcklqdq %ymm2, %ymm14, %ymm12
vpunpckhqdq %ymm2, %ymm14, %ymm2
vpunpcklqdq %ymm1, %ymm15, %ymm14
vpunpckhqdq %ymm1, %ymm15, %ymm1
vinserti128 $1, %xmm3, %ymm4, %ymm15
vmovdqa %ymm15, 0(%r12)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 128(%r12)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 256(%r12)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 384(%r12)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 512(%r12)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 640(%r12)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 768(%r12)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 1024(%r12)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 1152(%r12)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 1280(%r12)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 1408(%r12)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 1536(%r12)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 1664(%r12)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 1792(%r12)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 896(%r12)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 1920(%r12)
vmovdqa 2560(%r10), %ymm0
vmovdqa 2624(%r10), %ymm1
vmovdqa 2688(%r10), %ymm2
vmovdqa 2752(%r10), %ymm3
vpunpcklwd 2592(%r10), %ymm0, %ymm4
vpunpckhwd 2592(%r10), %ymm0, %ymm5
vpunpcklwd 2656(%r10), %ymm1, %ymm6
vpunpckhwd 2656(%r10), %ymm1, %ymm7
vpunpcklwd 2720(%r10), %ymm2, %ymm8
vpunpckhwd 2720(%r10), %ymm2, %ymm9
vpunpcklwd 2784(%r10), %ymm3, %ymm10
vpunpckhwd 2784(%r10), %ymm3, %ymm11
vpunpckldq %ymm6, %ymm4, %ymm0
vpunpckhdq %ymm6, %ymm4, %ymm1
vpunpckldq %ymm7, %ymm5, %ymm2
vpunpckhdq %ymm7, %ymm5, %ymm3
vpunpckldq %ymm10, %ymm8, %ymm12
vpunpckhdq %ymm10, %ymm8, %ymm13
vpunpckldq %ymm11, %ymm9, %ymm14
vpunpckhdq %ymm11, %ymm9, %ymm15
vpunpcklqdq %ymm12, %ymm0, %ymm4
vpunpckhqdq %ymm12, %ymm0, %ymm5
vpunpcklqdq %ymm13, %ymm1, %ymm6
vpunpckhqdq %ymm13, %ymm1, %ymm7
vpunpcklqdq %ymm14, %ymm2, %ymm8
vpunpckhqdq %ymm14, %ymm2, %ymm9
vpunpcklqdq %ymm15, %ymm3, %ymm10
vpunpckhqdq %ymm15, %ymm3, %ymm11
vmovdqa 2816(%r10), %ymm0
vmovdqa 2880(%r10), %ymm1
vmovdqa 2944(%r10), %ymm2
vmovdqa 3008(%r10), %ymm3
vpunpcklwd 2848(%r10), %ymm0, %ymm12
vpunpckhwd 2848(%r10), %ymm0, %ymm13
vpunpcklwd 2912(%r10), %ymm1, %ymm14
vpunpckhwd 2912(%r10), %ymm1, %ymm15
vpunpcklwd 2976(%r10), %ymm2, %ymm0
vpunpckhwd 2976(%r10), %ymm2, %ymm1
vpunpcklwd 3040(%r10), %ymm3, %ymm2
vpunpckhwd 3040(%r10), %ymm3, %ymm3
vmovdqa %ymm11, 0(%rsp)
vpunpckldq %ymm14, %ymm12, %ymm11
vpunpckhdq %ymm14, %ymm12, %ymm12
vpunpckldq %ymm15, %ymm13, %ymm14
vpunpckhdq %ymm15, %ymm13, %ymm15
vpunpckldq %ymm2, %ymm0, %ymm13
vpunpckhdq %ymm2, %ymm0, %ymm0
vpunpckldq %ymm3, %ymm1, %ymm2
vpunpckhdq %ymm3, %ymm1, %ymm1
vpunpcklqdq %ymm13, %ymm11, %ymm3
vpunpckhqdq %ymm13, %ymm11, %ymm13
vpunpcklqdq %ymm0, %ymm12, %ymm11
vpunpckhqdq %ymm0, %ymm12, %ymm0
vpunpcklqdq %ymm2, %ymm14, %ymm12
vpunpckhqdq %ymm2, %ymm14, %ymm2
vpunpcklqdq %ymm1, %ymm15, %ymm14
vpunpckhqdq %ymm1, %ymm15, %ymm1
vinserti128 $1, %xmm3, %ymm4, %ymm15
vmovdqa %ymm15, 32(%r12)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 160(%r12)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 288(%r12)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 416(%r12)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 544(%r12)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 672(%r12)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 800(%r12)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 1056(%r12)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 1184(%r12)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 1312(%r12)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 1440(%r12)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 1568(%r12)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 1696(%r12)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 1824(%r12)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 928(%r12)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 1952(%r12)
vmovdqa 3072(%r10), %ymm0
vmovdqa 3136(%r10), %ymm1
vmovdqa 3200(%r10), %ymm2
vmovdqa 3264(%r10), %ymm3
vpunpcklwd 3104(%r10), %ymm0, %ymm4
vpunpckhwd 3104(%r10), %ymm0, %ymm5
vpunpcklwd 3168(%r10), %ymm1, %ymm6
vpunpckhwd 3168(%r10), %ymm1, %ymm7
vpunpcklwd 3232(%r10), %ymm2, %ymm8
vpunpckhwd 3232(%r10), %ymm2, %ymm9
vpunpcklwd 3296(%r10), %ymm3, %ymm10
vpunpckhwd 3296(%r10), %ymm3, %ymm11
vpunpckldq %ymm6, %ymm4, %ymm0
vpunpckhdq %ymm6, %ymm4, %ymm1
vpunpckldq %ymm7, %ymm5, %ymm2
vpunpckhdq %ymm7, %ymm5, %ymm3
vpunpckldq %ymm10, %ymm8, %ymm12
vpunpckhdq %ymm10, %ymm8, %ymm13
vpunpckldq %ymm11, %ymm9, %ymm14
vpunpckhdq %ymm11, %ymm9, %ymm15
vpunpcklqdq %ymm12, %ymm0, %ymm4
vpunpckhqdq %ymm12, %ymm0, %ymm5
vpunpcklqdq %ymm13, %ymm1, %ymm6
vpunpckhqdq %ymm13, %ymm1, %ymm7
vpunpcklqdq %ymm14, %ymm2, %ymm8
vpunpckhqdq %ymm14, %ymm2, %ymm9
vpunpcklqdq %ymm15, %ymm3, %ymm10
vpunpckhqdq %ymm15, %ymm3, %ymm11
vmovdqa 3328(%r10), %ymm0
vmovdqa 3392(%r10), %ymm1
vmovdqa 3456(%r10), %ymm2
vmovdqa 3520(%r10), %ymm3
vpunpcklwd 3360(%r10), %ymm0, %ymm12
vpunpckhwd 3360(%r10), %ymm0, %ymm13
vpunpcklwd 3424(%r10), %ymm1, %ymm14
vpunpckhwd 3424(%r10), %ymm1, %ymm15
vpunpcklwd 3488(%r10), %ymm2, %ymm0
vpunpckhwd 3488(%r10), %ymm2, %ymm1
vpunpcklwd 3552(%r10), %ymm3, %ymm2
vpunpckhwd 3552(%r10), %ymm3, %ymm3
vmovdqa %ymm11, 0(%rsp)
vpunpckldq %ymm14, %ymm12, %ymm11
vpunpckhdq %ymm14, %ymm12, %ymm12
vpunpckldq %ymm15, %ymm13, %ymm14
vpunpckhdq %ymm15, %ymm13, %ymm15
vpunpckldq %ymm2, %ymm0, %ymm13
vpunpckhdq %ymm2, %ymm0, %ymm0
vpunpckldq %ymm3, %ymm1, %ymm2
vpunpckhdq %ymm3, %ymm1, %ymm1
vpunpcklqdq %ymm13, %ymm11, %ymm3
vpunpckhqdq %ymm13, %ymm11, %ymm13
vpunpcklqdq %ymm0, %ymm12, %ymm11
vpunpckhqdq %ymm0, %ymm12, %ymm0
vpunpcklqdq %ymm2, %ymm14, %ymm12
vpunpckhqdq %ymm2, %ymm14, %ymm2
vpunpcklqdq %ymm1, %ymm15, %ymm14
vpunpckhqdq %ymm1, %ymm15, %ymm1
vinserti128 $1, %xmm3, %ymm4, %ymm15
vmovdqa %ymm15, 64(%r12)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 192(%r12)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 320(%r12)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 448(%r12)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 576(%r12)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 704(%r12)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 832(%r12)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 1088(%r12)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 1216(%r12)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 1344(%r12)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 1472(%r12)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 1600(%r12)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 1728(%r12)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 1856(%r12)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 960(%r12)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 1984(%r12)
vmovdqa 3584(%r10), %ymm0
vmovdqa 3648(%r10), %ymm1
vmovdqa 3712(%r10), %ymm2
vmovdqa 3776(%r10), %ymm3
vpunpcklwd 3616(%r10), %ymm0, %ymm4
vpunpckhwd 3616(%r10), %ymm0, %ymm5
vpunpcklwd 3680(%r10), %ymm1, %ymm6
vpunpckhwd 3680(%r10), %ymm1, %ymm7
vpunpcklwd 3744(%r10), %ymm2, %ymm8
vpunpckhwd 3744(%r10), %ymm2, %ymm9
vpunpcklwd 3808(%r10), %ymm3, %ymm10
vpunpckhwd 3808(%r10), %ymm3, %ymm11
vpunpckldq %ymm6, %ymm4, %ymm0
vpunpckhdq %ymm6, %ymm4, %ymm1
vpunpckldq %ymm7, %ymm5, %ymm2
vpunpckhdq %ymm7, %ymm5, %ymm3
vpunpckldq %ymm10, %ymm8, %ymm12
vpunpckhdq %ymm10, %ymm8, %ymm13
vpunpckldq %ymm11, %ymm9, %ymm14
vpunpckhdq %ymm11, %ymm9, %ymm15
vpunpcklqdq %ymm12, %ymm0, %ymm4
vpunpckhqdq %ymm12, %ymm0, %ymm5
vpunpcklqdq %ymm13, %ymm1, %ymm6
vpunpckhqdq %ymm13, %ymm1, %ymm7
vpunpcklqdq %ymm14, %ymm2, %ymm8
vpunpckhqdq %ymm14, %ymm2, %ymm9
vpunpcklqdq %ymm15, %ymm3, %ymm10
vpunpckhqdq %ymm15, %ymm3, %ymm11
vmovdqa 3840(%r10), %ymm0
vmovdqa 3904(%r10), %ymm1
vmovdqa 3968(%r10), %ymm2
vmovdqa 4032(%r10), %ymm3
vpunpcklwd 3872(%r10), %ymm0, %ymm12
vpunpckhwd 3872(%r10), %ymm0, %ymm13
vpunpcklwd 3936(%r10), %ymm1, %ymm14
vpunpckhwd 3936(%r10), %ymm1, %ymm15
vpunpcklwd 4000(%r10), %ymm2, %ymm0
vpunpckhwd 4000(%r10), %ymm2, %ymm1
vpunpcklwd 4064(%r10), %ymm3, %ymm2
vpunpckhwd 4064(%r10), %ymm3, %ymm3
vmovdqa %ymm11, 0(%rsp)
vpunpckldq %ymm14, %ymm12, %ymm11
vpunpckhdq %ymm14, %ymm12, %ymm12
vpunpckldq %ymm15, %ymm13, %ymm14
vpunpckhdq %ymm15, %ymm13, %ymm15
vpunpckldq %ymm2, %ymm0, %ymm13
vpunpckhdq %ymm2, %ymm0, %ymm0
vpunpckldq %ymm3, %ymm1, %ymm2
vpunpckhdq %ymm3, %ymm1, %ymm1
vpunpcklqdq %ymm13, %ymm11, %ymm3
vpunpckhqdq %ymm13, %ymm11, %ymm13
vpunpcklqdq %ymm0, %ymm12, %ymm11
vpunpckhqdq %ymm0, %ymm12, %ymm0
vpunpcklqdq %ymm2, %ymm14, %ymm12
vpunpckhqdq %ymm2, %ymm14, %ymm2
vpunpcklqdq %ymm1, %ymm15, %ymm14
vpunpckhqdq %ymm1, %ymm15, %ymm1
vinserti128 $1, %xmm3, %ymm4, %ymm15
vmovdqa %ymm15, 96(%r12)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 224(%r12)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 352(%r12)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 480(%r12)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 608(%r12)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 736(%r12)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 864(%r12)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 1120(%r12)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 1248(%r12)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 1376(%r12)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 1504(%r12)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 1632(%r12)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 1760(%r12)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 1888(%r12)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 992(%r12)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 2016(%r12)
addq $32, %rsp
add $1024, %rax
add $1024, %r11
add $2048, %r12
dec %ecx
jnz karatsuba_loop_1
sub $8192, %r12
add $6656, %rsp
subq $2400, %rsp
vpxor %ymm0, %ymm0, %ymm0
vmovdqa %ymm0, 1792(%rsp)
vmovdqa %ymm0, 1824(%rsp)
vmovdqa %ymm0, 1856(%rsp)
vmovdqa %ymm0, 1888(%rsp)
vmovdqa %ymm0, 1920(%rsp)
vmovdqa %ymm0, 1952(%rsp)
vmovdqa %ymm0, 1984(%rsp)
vmovdqa %ymm0, 2016(%rsp)
vmovdqa %ymm0, 2048(%rsp)
vmovdqa %ymm0, 2080(%rsp)
vmovdqa %ymm0, 2112(%rsp)
vmovdqa %ymm0, 2144(%rsp)
vmovdqa %ymm0, 2176(%rsp)
vmovdqa %ymm0, 2208(%rsp)
vmovdqa %ymm0, 2240(%rsp)
vmovdqa %ymm0, 2272(%rsp)
vmovdqa %ymm0, 2304(%rsp)
vmovdqa %ymm0, 2336(%rsp)
vmovdqa %ymm0, 2368(%rsp)
vmovdqa %ymm0, 2400(%rsp)
vmovdqa %ymm0, 2432(%rsp)
vmovdqa %ymm0, 2464(%rsp)
vmovdqa %ymm0, 2496(%rsp)
vmovdqa %ymm0, 2528(%rsp)
vmovdqa %ymm0, 2560(%rsp)
vmovdqa %ymm0, 2592(%rsp)
vmovdqa %ymm0, 2624(%rsp)
vmovdqa %ymm0, 2656(%rsp)
vmovdqa %ymm0, 2688(%rsp)
vmovdqa %ymm0, 2720(%rsp)
vmovdqa %ymm0, 2752(%rsp)
vmovdqa %ymm0, 2784(%rsp)
vmovdqa const729(%rip), %ymm15
vmovdqa const3_inv(%rip), %ymm14
vmovdqa const5_inv(%rip), %ymm13
vmovdqa const9(%rip), %ymm12
vmovdqa 64(%r12), %ymm0
vpsubw 128(%r12), %ymm0, %ymm0
vmovdqa 320(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 192(%r12), %ymm1, %ymm1
vpsubw 0(%r12), %ymm0, %ymm0
vpaddw 256(%r12), %ymm0, %ymm0
vmovdqa 448(%r12), %ymm2
vpsubw 512(%r12), %ymm2, %ymm2
vmovdqa 704(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 576(%r12), %ymm3, %ymm3
vpsubw 384(%r12), %ymm2, %ymm2
vpaddw 640(%r12), %ymm2, %ymm2
vmovdqa 832(%r12), %ymm4
vpsubw 896(%r12), %ymm4, %ymm4
vmovdqa 1088(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 960(%r12), %ymm5, %ymm5
vpsubw 768(%r12), %ymm4, %ymm4
vpaddw 1024(%r12), %ymm4, %ymm4
vpsubw 384(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 0(%r12), %ymm1, %ymm1
vpaddw 768(%r12), %ymm1, %ymm1
vmovdqa 192(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 960(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 576(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 0(%r12), %ymm8
vmovdqa 576(%r12), %ymm9
vmovdqa %ymm8, 0(%rsp)
vmovdqa %ymm0, 32(%rsp)
vmovdqa %ymm1, 64(%rsp)
vmovdqa %ymm7, 96(%rsp)
vmovdqa %ymm5, 128(%rsp)
vmovdqa %ymm2, 160(%rsp)
vmovdqa %ymm3, 192(%rsp)
vmovdqa %ymm9, 224(%rsp)
vmovdqa 1216(%r12), %ymm0
vpsubw 1280(%r12), %ymm0, %ymm0
vmovdqa 1472(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 1344(%r12), %ymm1, %ymm1
vpsubw 1152(%r12), %ymm0, %ymm0
vpaddw 1408(%r12), %ymm0, %ymm0
vmovdqa 1600(%r12), %ymm2
vpsubw 1664(%r12), %ymm2, %ymm2
vmovdqa 1856(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 1728(%r12), %ymm3, %ymm3
vpsubw 1536(%r12), %ymm2, %ymm2
vpaddw 1792(%r12), %ymm2, %ymm2
vmovdqa 1984(%r12), %ymm4
vpsubw 2048(%r12), %ymm4, %ymm4
vmovdqa 2240(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 2112(%r12), %ymm5, %ymm5
vpsubw 1920(%r12), %ymm4, %ymm4
vpaddw 2176(%r12), %ymm4, %ymm4
vpsubw 1536(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 1152(%r12), %ymm1, %ymm1
vpaddw 1920(%r12), %ymm1, %ymm1
vmovdqa 1344(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 2112(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 1728(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 1152(%r12), %ymm8
vmovdqa 1728(%r12), %ymm9
vmovdqa %ymm8, 256(%rsp)
vmovdqa %ymm0, 288(%rsp)
vmovdqa %ymm1, 320(%rsp)
vmovdqa %ymm7, 352(%rsp)
vmovdqa %ymm5, 384(%rsp)
vmovdqa %ymm2, 416(%rsp)
vmovdqa %ymm3, 448(%rsp)
vmovdqa %ymm9, 480(%rsp)
vmovdqa 2368(%r12), %ymm0
vpsubw 2432(%r12), %ymm0, %ymm0
vmovdqa 2624(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 2496(%r12), %ymm1, %ymm1
vpsubw 2304(%r12), %ymm0, %ymm0
vpaddw 2560(%r12), %ymm0, %ymm0
vmovdqa 2752(%r12), %ymm2
vpsubw 2816(%r12), %ymm2, %ymm2
vmovdqa 3008(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 2880(%r12), %ymm3, %ymm3
vpsubw 2688(%r12), %ymm2, %ymm2
vpaddw 2944(%r12), %ymm2, %ymm2
vmovdqa 3136(%r12), %ymm4
vpsubw 3200(%r12), %ymm4, %ymm4
vmovdqa 3392(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 3264(%r12), %ymm5, %ymm5
vpsubw 3072(%r12), %ymm4, %ymm4
vpaddw 3328(%r12), %ymm4, %ymm4
vpsubw 2688(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 2304(%r12), %ymm1, %ymm1
vpaddw 3072(%r12), %ymm1, %ymm1
vmovdqa 2496(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 3264(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 2880(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 2304(%r12), %ymm8
vmovdqa 2880(%r12), %ymm9
vmovdqa %ymm8, 512(%rsp)
vmovdqa %ymm0, 544(%rsp)
vmovdqa %ymm1, 576(%rsp)
vmovdqa %ymm7, 608(%rsp)
vmovdqa %ymm5, 640(%rsp)
vmovdqa %ymm2, 672(%rsp)
vmovdqa %ymm3, 704(%rsp)
vmovdqa %ymm9, 736(%rsp)
vmovdqa 3520(%r12), %ymm0
vpsubw 3584(%r12), %ymm0, %ymm0
vmovdqa 3776(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 3648(%r12), %ymm1, %ymm1
vpsubw 3456(%r12), %ymm0, %ymm0
vpaddw 3712(%r12), %ymm0, %ymm0
vmovdqa 3904(%r12), %ymm2
vpsubw 3968(%r12), %ymm2, %ymm2
vmovdqa 4160(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 4032(%r12), %ymm3, %ymm3
vpsubw 3840(%r12), %ymm2, %ymm2
vpaddw 4096(%r12), %ymm2, %ymm2
vmovdqa 4288(%r12), %ymm4
vpsubw 4352(%r12), %ymm4, %ymm4
vmovdqa 4544(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 4416(%r12), %ymm5, %ymm5
vpsubw 4224(%r12), %ymm4, %ymm4
vpaddw 4480(%r12), %ymm4, %ymm4
vpsubw 3840(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 3456(%r12), %ymm1, %ymm1
vpaddw 4224(%r12), %ymm1, %ymm1
vmovdqa 3648(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 4416(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 4032(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 3456(%r12), %ymm8
vmovdqa 4032(%r12), %ymm9
vmovdqa %ymm8, 768(%rsp)
vmovdqa %ymm0, 800(%rsp)
vmovdqa %ymm1, 832(%rsp)
vmovdqa %ymm7, 864(%rsp)
vmovdqa %ymm5, 896(%rsp)
vmovdqa %ymm2, 928(%rsp)
vmovdqa %ymm3, 960(%rsp)
vmovdqa %ymm9, 992(%rsp)
vmovdqa 4672(%r12), %ymm0
vpsubw 4736(%r12), %ymm0, %ymm0
vmovdqa 4928(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4800(%r12), %ymm1, %ymm1
vpsubw 4608(%r12), %ymm0, %ymm0
vpaddw 4864(%r12), %ymm0, %ymm0
vmovdqa 5056(%r12), %ymm2
vpsubw 5120(%r12), %ymm2, %ymm2
vmovdqa 5312(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 5184(%r12), %ymm3, %ymm3
vpsubw 4992(%r12), %ymm2, %ymm2
vpaddw 5248(%r12), %ymm2, %ymm2
vmovdqa 5440(%r12), %ymm4
vpsubw 5504(%r12), %ymm4, %ymm4
vmovdqa 5696(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 5568(%r12), %ymm5, %ymm5
vpsubw 5376(%r12), %ymm4, %ymm4
vpaddw 5632(%r12), %ymm4, %ymm4
vpsubw 4992(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 4608(%r12), %ymm1, %ymm1
vpaddw 5376(%r12), %ymm1, %ymm1
vmovdqa 4800(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 5568(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 5184(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 4608(%r12), %ymm8
vmovdqa 5184(%r12), %ymm9
vmovdqa %ymm8, 1024(%rsp)
vmovdqa %ymm0, 1056(%rsp)
vmovdqa %ymm1, 1088(%rsp)
vmovdqa %ymm7, 1120(%rsp)
vmovdqa %ymm5, 1152(%rsp)
vmovdqa %ymm2, 1184(%rsp)
vmovdqa %ymm3, 1216(%rsp)
vmovdqa %ymm9, 1248(%rsp)
vmovdqa 5824(%r12), %ymm0
vpsubw 5888(%r12), %ymm0, %ymm0
vmovdqa 6080(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5952(%r12), %ymm1, %ymm1
vpsubw 5760(%r12), %ymm0, %ymm0
vpaddw 6016(%r12), %ymm0, %ymm0
vmovdqa 6208(%r12), %ymm2
vpsubw 6272(%r12), %ymm2, %ymm2
vmovdqa 6464(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 6336(%r12), %ymm3, %ymm3
vpsubw 6144(%r12), %ymm2, %ymm2
vpaddw 6400(%r12), %ymm2, %ymm2
vmovdqa 6592(%r12), %ymm4
vpsubw 6656(%r12), %ymm4, %ymm4
vmovdqa 6848(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 6720(%r12), %ymm5, %ymm5
vpsubw 6528(%r12), %ymm4, %ymm4
vpaddw 6784(%r12), %ymm4, %ymm4
vpsubw 6144(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 5760(%r12), %ymm1, %ymm1
vpaddw 6528(%r12), %ymm1, %ymm1
vmovdqa 5952(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 6720(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 6336(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 5760(%r12), %ymm8
vmovdqa 6336(%r12), %ymm9
vmovdqa %ymm8, 1280(%rsp)
vmovdqa %ymm0, 1312(%rsp)
vmovdqa %ymm1, 1344(%rsp)
vmovdqa %ymm7, 1376(%rsp)
vmovdqa %ymm5, 1408(%rsp)
vmovdqa %ymm2, 1440(%rsp)
vmovdqa %ymm3, 1472(%rsp)
vmovdqa %ymm9, 1504(%rsp)
vmovdqa 6976(%r12), %ymm0
vpsubw 7040(%r12), %ymm0, %ymm0
vmovdqa 7232(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 7104(%r12), %ymm1, %ymm1
vpsubw 6912(%r12), %ymm0, %ymm0
vpaddw 7168(%r12), %ymm0, %ymm0
vmovdqa 7360(%r12), %ymm2
vpsubw 7424(%r12), %ymm2, %ymm2
vmovdqa 7616(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 7488(%r12), %ymm3, %ymm3
vpsubw 7296(%r12), %ymm2, %ymm2
vpaddw 7552(%r12), %ymm2, %ymm2
vmovdqa 7744(%r12), %ymm4
vpsubw 7808(%r12), %ymm4, %ymm4
vmovdqa 8000(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 7872(%r12), %ymm5, %ymm5
vpsubw 7680(%r12), %ymm4, %ymm4
vpaddw 7936(%r12), %ymm4, %ymm4
vpsubw 7296(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 6912(%r12), %ymm1, %ymm1
vpaddw 7680(%r12), %ymm1, %ymm1
vmovdqa 7104(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 7872(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 7488(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 6912(%r12), %ymm8
vmovdqa 7488(%r12), %ymm9
vmovdqa %ymm8, 1536(%rsp)
vmovdqa %ymm0, 1568(%rsp)
vmovdqa %ymm1, 1600(%rsp)
vmovdqa %ymm7, 1632(%rsp)
vmovdqa %ymm5, 1664(%rsp)
vmovdqa %ymm2, 1696(%rsp)
vmovdqa %ymm3, 1728(%rsp)
vmovdqa %ymm9, 1760(%rsp)
vmovdqa 0(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm10
vpunpckhwd const0(%rip), %ymm11, %ymm9
vpslld $1, %ymm10, %ymm10
vpslld $1, %ymm9, %ymm9
vmovdqa 256(%rsp), %ymm8
vpunpcklwd const0(%rip), %ymm8, %ymm7
vpunpckhwd const0(%rip), %ymm8, %ymm8
vmovdqa 512(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm5
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm5, %ymm7, %ymm4
vpaddd %ymm6, %ymm8, %ymm3
vpsubd %ymm10, %ymm4, %ymm4
vpsubd %ymm9, %ymm3, %ymm3
vpsubd %ymm5, %ymm7, %ymm5
vpsubd %ymm6, %ymm8, %ymm6
vpsrld $1, %ymm5, %ymm5
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm5, %ymm5
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm5, %ymm6
vmovdqa 1536(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm8
vpunpckhwd const0(%rip), %ymm5, %ymm7
vpslld $1, %ymm8, %ymm8
vpslld $1, %ymm7, %ymm7
vpsubd %ymm8, %ymm4, %ymm4
vpsubd %ymm7, %ymm3, %ymm3
vpsrld $1, %ymm4, %ymm4
vpsrld $1, %ymm3, %ymm3
vpand mask32_to_16(%rip), %ymm4, %ymm4
vpand mask32_to_16(%rip), %ymm3, %ymm3
vpackusdw %ymm3, %ymm4, %ymm3
vmovdqa 768(%rsp), %ymm4
vpaddw 1024(%rsp), %ymm4, %ymm7
vpsubw 1024(%rsp), %ymm4, %ymm4
vpsrlw $2, %ymm4, %ymm4
vpsubw %ymm6, %ymm4, %ymm4
vpmullw %ymm14, %ymm4, %ymm4
vpsllw $1, %ymm11, %ymm8
vpsubw %ymm8, %ymm7, %ymm8
vpsllw $7, %ymm5, %ymm7
vpsubw %ymm7, %ymm8, %ymm7
vpsrlw $3, %ymm7, %ymm7
vpsubw %ymm3, %ymm7, %ymm7
vmovdqa 1280(%rsp), %ymm8
vpsubw %ymm11, %ymm8, %ymm8
vpmullw %ymm15, %ymm5, %ymm9
vpsubw %ymm9, %ymm8, %ymm9
vpmullw %ymm14, %ymm7, %ymm7
vpsubw %ymm7, %ymm3, %ymm3
vpmullw %ymm12, %ymm7, %ymm8
vpaddw %ymm8, %ymm3, %ymm8
vpmullw %ymm12, %ymm8, %ymm8
vpsubw %ymm8, %ymm9, %ymm8
vpmullw %ymm14, %ymm8, %ymm8
vpsubw %ymm6, %ymm8, %ymm8
vpsrlw $3, %ymm8, %ymm8
vpsubw %ymm4, %ymm8, %ymm8
vpsubw %ymm8, %ymm4, %ymm4
vpsubw %ymm4, %ymm6, %ymm6
vpmullw %ymm13, %ymm8, %ymm8
vpsubw %ymm8, %ymm6, %ymm6
vpshufb shuf48_16(%rip), %ymm7, %ymm7
vpand mask3_5_3_5(%rip), %ymm7, %ymm9
vpand mask5_3_5_3(%rip), %ymm7, %ymm7
vpermq $206, %ymm9, %ymm9
vpand mask_keephigh(%rip), %ymm9, %ymm10
vpor %ymm10, %ymm7, %ymm7
vpaddw %ymm7, %ymm11, %ymm11
vmovdqa %xmm9, 2048(%rsp)
vpshufb shuf48_16(%rip), %ymm8, %ymm8
vpand mask3_5_3_5(%rip), %ymm8, %ymm9
vpand mask5_3_5_3(%rip), %ymm8, %ymm8
vpermq $206, %ymm9, %ymm9
vpand mask_keephigh(%rip), %ymm9, %ymm10
vpor %ymm10, %ymm8, %ymm8
vpaddw %ymm8, %ymm6, %ymm6
vmovdqa %xmm9, 2304(%rsp)
vpshufb shuf48_16(%rip), %ymm5, %ymm5
vpand mask3_5_3_5(%rip), %ymm5, %ymm9
vpand mask5_3_5_3(%rip), %ymm5, %ymm5
vpermq $206, %ymm9, %ymm9
vpand mask_keephigh(%rip), %ymm9, %ymm10
vpor %ymm10, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vmovdqa %xmm9, 2560(%rsp)
vmovdqa %ymm11, 0(%rdi)
vmovdqa %ymm6, 256(%rdi)
vmovdqa %ymm3, 512(%rdi)
vmovdqa %ymm4, 768(%rdi)
vmovdqa 32(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm8
vpunpckhwd const0(%rip), %ymm5, %ymm7
vpslld $1, %ymm8, %ymm8
vpslld $1, %ymm7, %ymm7
vmovdqa 288(%rsp), %ymm4
vpunpcklwd const0(%rip), %ymm4, %ymm3
vpunpckhwd const0(%rip), %ymm4, %ymm4
vmovdqa 544(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm11
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm11, %ymm3, %ymm9
vpaddd %ymm6, %ymm4, %ymm10
vpsubd %ymm8, %ymm9, %ymm9
vpsubd %ymm7, %ymm10, %ymm10
vpsubd %ymm11, %ymm3, %ymm11
vpsubd %ymm6, %ymm4, %ymm6
vpsrld $1, %ymm11, %ymm11
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm11, %ymm11
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm11, %ymm6
vmovdqa 1568(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm4
vpunpckhwd const0(%rip), %ymm11, %ymm3
vpslld $1, %ymm4, %ymm4
vpslld $1, %ymm3, %ymm3
vpsubd %ymm4, %ymm9, %ymm9
vpsubd %ymm3, %ymm10, %ymm10
vpsrld $1, %ymm9, %ymm9
vpsrld $1, %ymm10, %ymm10
vpand mask32_to_16(%rip), %ymm9, %ymm9
vpand mask32_to_16(%rip), %ymm10, %ymm10
vpackusdw %ymm10, %ymm9, %ymm10
vmovdqa 800(%rsp), %ymm9
vpaddw 1056(%rsp), %ymm9, %ymm3
vpsubw 1056(%rsp), %ymm9, %ymm9
vpsrlw $2, %ymm9, %ymm9
vpsubw %ymm6, %ymm9, %ymm9
vpmullw %ymm14, %ymm9, %ymm9
vpsllw $1, %ymm5, %ymm4
vpsubw %ymm4, %ymm3, %ymm4
vpsllw $7, %ymm11, %ymm3
vpsubw %ymm3, %ymm4, %ymm3
vpsrlw $3, %ymm3, %ymm3
vpsubw %ymm10, %ymm3, %ymm3
vmovdqa 1312(%rsp), %ymm4
vpsubw %ymm5, %ymm4, %ymm4
vpmullw %ymm15, %ymm11, %ymm7
vpsubw %ymm7, %ymm4, %ymm7
vpmullw %ymm14, %ymm3, %ymm3
vpsubw %ymm3, %ymm10, %ymm10
vpmullw %ymm12, %ymm3, %ymm4
vpaddw %ymm4, %ymm10, %ymm4
vpmullw %ymm12, %ymm4, %ymm4
vpsubw %ymm4, %ymm7, %ymm4
vpmullw %ymm14, %ymm4, %ymm4
vpsubw %ymm6, %ymm4, %ymm4
vpsrlw $3, %ymm4, %ymm4
vpsubw %ymm9, %ymm4, %ymm4
vpsubw %ymm4, %ymm9, %ymm9
vpsubw %ymm9, %ymm6, %ymm6
vpmullw %ymm13, %ymm4, %ymm4
vpsubw %ymm4, %ymm6, %ymm6
vpshufb shuf48_16(%rip), %ymm3, %ymm3
vpand mask3_5_3_5(%rip), %ymm3, %ymm7
vpand mask5_3_5_3(%rip), %ymm3, %ymm3
vpermq $206, %ymm7, %ymm7
vpand mask_keephigh(%rip), %ymm7, %ymm8
vpor %ymm8, %ymm3, %ymm3
vpaddw %ymm3, %ymm5, %ymm5
vmovdqa %xmm7, 2080(%rsp)
vpshufb shuf48_16(%rip), %ymm4, %ymm4
vpand mask3_5_3_5(%rip), %ymm4, %ymm7
vpand mask5_3_5_3(%rip), %ymm4, %ymm4
vpermq $206, %ymm7, %ymm7
vpand mask_keephigh(%rip), %ymm7, %ymm8
vpor %ymm8, %ymm4, %ymm4
vpaddw %ymm4, %ymm6, %ymm6
vmovdqa %xmm7, 2336(%rsp)
vpshufb shuf48_16(%rip), %ymm11, %ymm11
vpand mask3_5_3_5(%rip), %ymm11, %ymm7
vpand mask5_3_5_3(%rip), %ymm11, %ymm11
vpermq $206, %ymm7, %ymm7
vpand mask_keephigh(%rip), %ymm7, %ymm8
vpor %ymm8, %ymm11, %ymm11
vpaddw %ymm11, %ymm10, %ymm10
vmovdqa %xmm7, 2592(%rsp)
vmovdqa %ymm5, 64(%rdi)
vmovdqa %ymm6, 320(%rdi)
vmovdqa %ymm10, 576(%rdi)
vmovdqa %ymm9, 832(%rdi)
vmovdqa 64(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm4
vpunpckhwd const0(%rip), %ymm11, %ymm3
vpslld $1, %ymm4, %ymm4
vpslld $1, %ymm3, %ymm3
vmovdqa 320(%rsp), %ymm9
vpunpcklwd const0(%rip), %ymm9, %ymm10
vpunpckhwd const0(%rip), %ymm9, %ymm9
vmovdqa 576(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm5
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm5, %ymm10, %ymm7
vpaddd %ymm6, %ymm9, %ymm8
vpsubd %ymm4, %ymm7, %ymm7
vpsubd %ymm3, %ymm8, %ymm8
vpsubd %ymm5, %ymm10, %ymm5
vpsubd %ymm6, %ymm9, %ymm6
vpsrld $1, %ymm5, %ymm5
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm5, %ymm5
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm5, %ymm6
vmovdqa 1600(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm9
vpunpckhwd const0(%rip), %ymm5, %ymm10
vpslld $1, %ymm9, %ymm9
vpslld $1, %ymm10, %ymm10
vpsubd %ymm9, %ymm7, %ymm7
vpsubd %ymm10, %ymm8, %ymm8
vpsrld $1, %ymm7, %ymm7
vpsrld $1, %ymm8, %ymm8
vpand mask32_to_16(%rip), %ymm7, %ymm7
vpand mask32_to_16(%rip), %ymm8, %ymm8
vpackusdw %ymm8, %ymm7, %ymm8
vmovdqa 832(%rsp), %ymm7
vpaddw 1088(%rsp), %ymm7, %ymm10
vpsubw 1088(%rsp), %ymm7, %ymm7
vpsrlw $2, %ymm7, %ymm7
vpsubw %ymm6, %ymm7, %ymm7
vpmullw %ymm14, %ymm7, %ymm7
vpsllw $1, %ymm11, %ymm9
vpsubw %ymm9, %ymm10, %ymm9
vpsllw $7, %ymm5, %ymm10
vpsubw %ymm10, %ymm9, %ymm10
vpsrlw $3, %ymm10, %ymm10
vpsubw %ymm8, %ymm10, %ymm10
vmovdqa 1344(%rsp), %ymm9
vpsubw %ymm11, %ymm9, %ymm9
vpmullw %ymm15, %ymm5, %ymm3
vpsubw %ymm3, %ymm9, %ymm3
vpmullw %ymm14, %ymm10, %ymm10
vpsubw %ymm10, %ymm8, %ymm8
vpmullw %ymm12, %ymm10, %ymm9
vpaddw %ymm9, %ymm8, %ymm9
vpmullw %ymm12, %ymm9, %ymm9
vpsubw %ymm9, %ymm3, %ymm9
vpmullw %ymm14, %ymm9, %ymm9
vpsubw %ymm6, %ymm9, %ymm9
vpsrlw $3, %ymm9, %ymm9
vpsubw %ymm7, %ymm9, %ymm9
vpsubw %ymm9, %ymm7, %ymm7
vpsubw %ymm7, %ymm6, %ymm6
vpmullw %ymm13, %ymm9, %ymm9
vpsubw %ymm9, %ymm6, %ymm6
vpshufb shuf48_16(%rip), %ymm10, %ymm10
vpand mask3_5_3_5(%rip), %ymm10, %ymm3
vpand mask5_3_5_3(%rip), %ymm10, %ymm10
vpermq $206, %ymm3, %ymm3
vpand mask_keephigh(%rip), %ymm3, %ymm4
vpor %ymm4, %ymm10, %ymm10
vpaddw %ymm10, %ymm11, %ymm11
vmovdqa %xmm3, 2112(%rsp)
vpshufb shuf48_16(%rip), %ymm9, %ymm9
vpand mask3_5_3_5(%rip), %ymm9, %ymm3
vpand mask5_3_5_3(%rip), %ymm9, %ymm9
vpermq $206, %ymm3, %ymm3
vpand mask_keephigh(%rip), %ymm3, %ymm4
vpor %ymm4, %ymm9, %ymm9
vpaddw %ymm9, %ymm6, %ymm6
vmovdqa %xmm3, 2368(%rsp)
vpshufb shuf48_16(%rip), %ymm5, %ymm5
vpand mask3_5_3_5(%rip), %ymm5, %ymm3
vpand mask5_3_5_3(%rip), %ymm5, %ymm5
vpermq $206, %ymm3, %ymm3
vpand mask_keephigh(%rip), %ymm3, %ymm4
vpor %ymm4, %ymm5, %ymm5
vpaddw %ymm5, %ymm8, %ymm8
vmovdqa %xmm3, 2624(%rsp)
vmovdqa %ymm11, 128(%rdi)
vmovdqa %ymm6, 384(%rdi)
vmovdqa %ymm8, 640(%rdi)
vmovdqa %ymm7, 896(%rdi)
vmovdqa 96(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm9
vpunpckhwd const0(%rip), %ymm5, %ymm10
vpslld $1, %ymm9, %ymm9
vpslld $1, %ymm10, %ymm10
vmovdqa 352(%rsp), %ymm7
vpunpcklwd const0(%rip), %ymm7, %ymm8
vpunpckhwd const0(%rip), %ymm7, %ymm7
vmovdqa 608(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm11
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm11, %ymm8, %ymm3
vpaddd %ymm6, %ymm7, %ymm4
vpsubd %ymm9, %ymm3, %ymm3
vpsubd %ymm10, %ymm4, %ymm4
vpsubd %ymm11, %ymm8, %ymm11
vpsubd %ymm6, %ymm7, %ymm6
vpsrld $1, %ymm11, %ymm11
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm11, %ymm11
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm11, %ymm6
vmovdqa 1632(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm7
vpunpckhwd const0(%rip), %ymm11, %ymm8
vpslld $1, %ymm7, %ymm7
vpslld $1, %ymm8, %ymm8
vpsubd %ymm7, %ymm3, %ymm3
vpsubd %ymm8, %ymm4, %ymm4
vpsrld $1, %ymm3, %ymm3
vpsrld $1, %ymm4, %ymm4
vpand mask32_to_16(%rip), %ymm3, %ymm3
vpand mask32_to_16(%rip), %ymm4, %ymm4
vpackusdw %ymm4, %ymm3, %ymm4
vmovdqa 864(%rsp), %ymm3
vpaddw 1120(%rsp), %ymm3, %ymm8
vpsubw 1120(%rsp), %ymm3, %ymm3
vpsrlw $2, %ymm3, %ymm3
vpsubw %ymm6, %ymm3, %ymm3
vpmullw %ymm14, %ymm3, %ymm3
vpsllw $1, %ymm5, %ymm7
vpsubw %ymm7, %ymm8, %ymm7
vpsllw $7, %ymm11, %ymm8
vpsubw %ymm8, %ymm7, %ymm8
vpsrlw $3, %ymm8, %ymm8
vpsubw %ymm4, %ymm8, %ymm8
vmovdqa 1376(%rsp), %ymm7
vpsubw %ymm5, %ymm7, %ymm7
vpmullw %ymm15, %ymm11, %ymm10
vpsubw %ymm10, %ymm7, %ymm10
vpmullw %ymm14, %ymm8, %ymm8
vpsubw %ymm8, %ymm4, %ymm4
vpmullw %ymm12, %ymm8, %ymm7
vpaddw %ymm7, %ymm4, %ymm7
vpmullw %ymm12, %ymm7, %ymm7
vpsubw %ymm7, %ymm10, %ymm7
vpmullw %ymm14, %ymm7, %ymm7
vpsubw %ymm6, %ymm7, %ymm7
vpsrlw $3, %ymm7, %ymm7
vpsubw %ymm3, %ymm7, %ymm7
vpsubw %ymm7, %ymm3, %ymm3
vpsubw %ymm3, %ymm6, %ymm6
vpmullw %ymm13, %ymm7, %ymm7
vpsubw %ymm7, %ymm6, %ymm6
vpshufb shuf48_16(%rip), %ymm8, %ymm8
vpand mask3_5_3_5(%rip), %ymm8, %ymm10
vpand mask5_3_5_3(%rip), %ymm8, %ymm8
vpermq $206, %ymm10, %ymm10
vpand mask_keephigh(%rip), %ymm10, %ymm9
vpor %ymm9, %ymm8, %ymm8
vpaddw %ymm8, %ymm5, %ymm5
vmovdqa %xmm10, 2144(%rsp)
vpshufb shuf48_16(%rip), %ymm7, %ymm7
vpand mask3_5_3_5(%rip), %ymm7, %ymm10
vpand mask5_3_5_3(%rip), %ymm7, %ymm7
vpermq $206, %ymm10, %ymm10
vpand mask_keephigh(%rip), %ymm10, %ymm9
vpor %ymm9, %ymm7, %ymm7
vpaddw %ymm7, %ymm6, %ymm6
vmovdqa %xmm10, 2400(%rsp)
vpshufb shuf48_16(%rip), %ymm11, %ymm11
vpand mask3_5_3_5(%rip), %ymm11, %ymm10
vpand mask5_3_5_3(%rip), %ymm11, %ymm11
vpermq $206, %ymm10, %ymm10
vpand mask_keephigh(%rip), %ymm10, %ymm9
vpor %ymm9, %ymm11, %ymm11
vpaddw %ymm11, %ymm4, %ymm4
vmovdqa %xmm10, 2656(%rsp)
vmovdqa %ymm5, 192(%rdi)
vmovdqa %ymm6, 448(%rdi)
vmovdqa %ymm4, 704(%rdi)
vmovdqa %ymm3, 960(%rdi)
vmovdqa 128(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm7
vpunpckhwd const0(%rip), %ymm11, %ymm8
vpslld $1, %ymm7, %ymm7
vpslld $1, %ymm8, %ymm8
vmovdqa 384(%rsp), %ymm3
vpunpcklwd const0(%rip), %ymm3, %ymm4
vpunpckhwd const0(%rip), %ymm3, %ymm3
vmovdqa 640(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm5
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm5, %ymm4, %ymm10
vpaddd %ymm6, %ymm3, %ymm9
vpsubd %ymm7, %ymm10, %ymm10
vpsubd %ymm8, %ymm9, %ymm9
vpsubd %ymm5, %ymm4, %ymm5
vpsubd %ymm6, %ymm3, %ymm6
vpsrld $1, %ymm5, %ymm5
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm5, %ymm5
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm5, %ymm6
vmovdqa 1664(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm3
vpunpckhwd const0(%rip), %ymm5, %ymm4
vpslld $1, %ymm3, %ymm3
vpslld $1, %ymm4, %ymm4
vpsubd %ymm3, %ymm10, %ymm10
vpsubd %ymm4, %ymm9, %ymm9
vpsrld $1, %ymm10, %ymm10
vpsrld $1, %ymm9, %ymm9
vpand mask32_to_16(%rip), %ymm10, %ymm10
vpand mask32_to_16(%rip), %ymm9, %ymm9
vpackusdw %ymm9, %ymm10, %ymm9
vmovdqa 896(%rsp), %ymm10
vpaddw 1152(%rsp), %ymm10, %ymm4
vpsubw 1152(%rsp), %ymm10, %ymm10
vpsrlw $2, %ymm10, %ymm10
vpsubw %ymm6, %ymm10, %ymm10
vpmullw %ymm14, %ymm10, %ymm10
vpsllw $1, %ymm11, %ymm3
vpsubw %ymm3, %ymm4, %ymm3
vpsllw $7, %ymm5, %ymm4
vpsubw %ymm4, %ymm3, %ymm4
vpsrlw $3, %ymm4, %ymm4
vpsubw %ymm9, %ymm4, %ymm4
vmovdqa 1408(%rsp), %ymm3
vpsubw %ymm11, %ymm3, %ymm3
vpmullw %ymm15, %ymm5, %ymm8
vpsubw %ymm8, %ymm3, %ymm8
vpmullw %ymm14, %ymm4, %ymm4
vpsubw %ymm4, %ymm9, %ymm9
vpmullw %ymm12, %ymm4, %ymm3
vpaddw %ymm3, %ymm9, %ymm3
vpmullw %ymm12, %ymm3, %ymm3
vpsubw %ymm3, %ymm8, %ymm3
vpmullw %ymm14, %ymm3, %ymm3
vpsubw %ymm6, %ymm3, %ymm3
vpsrlw $3, %ymm3, %ymm3
vpsubw %ymm10, %ymm3, %ymm3
vpsubw %ymm3, %ymm10, %ymm10
vpsubw %ymm10, %ymm6, %ymm6
vpmullw %ymm13, %ymm3, %ymm3
vpsubw %ymm3, %ymm6, %ymm6
vmovdqa 256(%rdi), %ymm8
vmovdqa 512(%rdi), %ymm7
vmovdqa 768(%rdi), %ymm2
vpaddw %ymm11, %ymm8, %ymm11
vpaddw %ymm6, %ymm7, %ymm6
vpaddw %ymm9, %ymm2, %ymm9
vpshufb shuf48_16(%rip), %ymm10, %ymm10
vpand mask3_5_3_5(%rip), %ymm10, %ymm2
vpand mask5_3_5_3(%rip), %ymm10, %ymm10
vpermq $206, %ymm2, %ymm2
vpand mask_keephigh(%rip), %ymm2, %ymm7
vpor %ymm7, %ymm10, %ymm10
vmovdqa 0(%rdi), %ymm7
vpaddw %ymm10, %ymm7, %ymm7
vmovdqa %ymm7, 0(%rdi)
vmovdqa %xmm2, 1920(%rsp)
vpshufb shuf48_16(%rip), %ymm4, %ymm4
vpand mask3_5_3_5(%rip), %ymm4, %ymm2
vpand mask5_3_5_3(%rip), %ymm4, %ymm4
vpermq $206, %ymm2, %ymm2
vpand mask_keephigh(%rip), %ymm2, %ymm7
vpor %ymm7, %ymm4, %ymm4
vpaddw %ymm4, %ymm11, %ymm11
vmovdqa %xmm2, 2176(%rsp)
vpshufb shuf48_16(%rip), %ymm3, %ymm3
vpand mask3_5_3_5(%rip), %ymm3, %ymm2
vpand mask5_3_5_3(%rip), %ymm3, %ymm3
vpermq $206, %ymm2, %ymm2
vpand mask_keephigh(%rip), %ymm2, %ymm7
vpor %ymm7, %ymm3, %ymm3
vpaddw %ymm3, %ymm6, %ymm6
vmovdqa %xmm2, 2432(%rsp)
vpshufb shuf48_16(%rip), %ymm5, %ymm5
vpand mask3_5_3_5(%rip), %ymm5, %ymm2
vpand mask5_3_5_3(%rip), %ymm5, %ymm5
vpermq $206, %ymm2, %ymm2
vpand mask_keephigh(%rip), %ymm2, %ymm7
vpor %ymm7, %ymm5, %ymm5
vpaddw %ymm5, %ymm9, %ymm9
vmovdqa %xmm2, 2688(%rsp)
vmovdqa %ymm11, 256(%rdi)
vmovdqa %ymm6, 512(%rdi)
vmovdqa %ymm9, 768(%rdi)
vmovdqa 160(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm3
vpunpckhwd const0(%rip), %ymm5, %ymm4
vpslld $1, %ymm3, %ymm3
vpslld $1, %ymm4, %ymm4
vmovdqa 416(%rsp), %ymm10
vpunpcklwd const0(%rip), %ymm10, %ymm9
vpunpckhwd const0(%rip), %ymm10, %ymm10
vmovdqa 672(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm11
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm11, %ymm9, %ymm2
vpaddd %ymm6, %ymm10, %ymm7
vpsubd %ymm3, %ymm2, %ymm2
vpsubd %ymm4, %ymm7, %ymm7
vpsubd %ymm11, %ymm9, %ymm11
vpsubd %ymm6, %ymm10, %ymm6
vpsrld $1, %ymm11, %ymm11
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm11, %ymm11
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm11, %ymm6
vmovdqa 1696(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm10
vpunpckhwd const0(%rip), %ymm11, %ymm9
vpslld $1, %ymm10, %ymm10
vpslld $1, %ymm9, %ymm9
vpsubd %ymm10, %ymm2, %ymm2
vpsubd %ymm9, %ymm7, %ymm7
vpsrld $1, %ymm2, %ymm2
vpsrld $1, %ymm7, %ymm7
vpand mask32_to_16(%rip), %ymm2, %ymm2
vpand mask32_to_16(%rip), %ymm7, %ymm7
vpackusdw %ymm7, %ymm2, %ymm7
vmovdqa 928(%rsp), %ymm2
vpaddw 1184(%rsp), %ymm2, %ymm9
vpsubw 1184(%rsp), %ymm2, %ymm2
vpsrlw $2, %ymm2, %ymm2
vpsubw %ymm6, %ymm2, %ymm2
vpmullw %ymm14, %ymm2, %ymm2
vpsllw $1, %ymm5, %ymm10
vpsubw %ymm10, %ymm9, %ymm10
vpsllw $7, %ymm11, %ymm9
vpsubw %ymm9, %ymm10, %ymm9
vpsrlw $3, %ymm9, %ymm9
vpsubw %ymm7, %ymm9, %ymm9
vmovdqa 1440(%rsp), %ymm10
vpsubw %ymm5, %ymm10, %ymm10
vpmullw %ymm15, %ymm11, %ymm4
vpsubw %ymm4, %ymm10, %ymm4
vpmullw %ymm14, %ymm9, %ymm9
vpsubw %ymm9, %ymm7, %ymm7
vpmullw %ymm12, %ymm9, %ymm10
vpaddw %ymm10, %ymm7, %ymm10
vpmullw %ymm12, %ymm10, %ymm10
vpsubw %ymm10, %ymm4, %ymm10
vpmullw %ymm14, %ymm10, %ymm10
vpsubw %ymm6, %ymm10, %ymm10
vpsrlw $3, %ymm10, %ymm10
vpsubw %ymm2, %ymm10, %ymm10
vpsubw %ymm10, %ymm2, %ymm2
vpsubw %ymm2, %ymm6, %ymm6
vpmullw %ymm13, %ymm10, %ymm10
vpsubw %ymm10, %ymm6, %ymm6
vmovdqa 320(%rdi), %ymm4
vmovdqa 576(%rdi), %ymm3
vmovdqa 832(%rdi), %ymm8
vpaddw %ymm5, %ymm4, %ymm5
vpaddw %ymm6, %ymm3, %ymm6
vpaddw %ymm7, %ymm8, %ymm7
vpshufb shuf48_16(%rip), %ymm2, %ymm2
vpand mask3_5_3_5(%rip), %ymm2, %ymm8
vpand mask5_3_5_3(%rip), %ymm2, %ymm2
vpermq $206, %ymm8, %ymm8
vpand mask_keephigh(%rip), %ymm8, %ymm3
vpor %ymm3, %ymm2, %ymm2
vmovdqa 64(%rdi), %ymm3
vpaddw %ymm2, %ymm3, %ymm3
vmovdqa %ymm3, 64(%rdi)
vmovdqa %xmm8, 1952(%rsp)
vpshufb shuf48_16(%rip), %ymm9, %ymm9
vpand mask3_5_3_5(%rip), %ymm9, %ymm8
vpand mask5_3_5_3(%rip), %ymm9, %ymm9
vpermq $206, %ymm8, %ymm8
vpand mask_keephigh(%rip), %ymm8, %ymm3
vpor %ymm3, %ymm9, %ymm9
vpaddw %ymm9, %ymm5, %ymm5
vmovdqa %xmm8, 2208(%rsp)
vpshufb shuf48_16(%rip), %ymm10, %ymm10
vpand mask3_5_3_5(%rip), %ymm10, %ymm8
vpand mask5_3_5_3(%rip), %ymm10, %ymm10
vpermq $206, %ymm8, %ymm8
vpand mask_keephigh(%rip), %ymm8, %ymm3
vpor %ymm3, %ymm10, %ymm10
vpaddw %ymm10, %ymm6, %ymm6
vmovdqa %xmm8, 2464(%rsp)
vpshufb shuf48_16(%rip), %ymm11, %ymm11
vpand mask3_5_3_5(%rip), %ymm11, %ymm8
vpand mask5_3_5_3(%rip), %ymm11, %ymm11
vpermq $206, %ymm8, %ymm8
vpand mask_keephigh(%rip), %ymm8, %ymm3
vpor %ymm3, %ymm11, %ymm11
vpaddw %ymm11, %ymm7, %ymm7
vmovdqa %xmm8, 2720(%rsp)
vmovdqa %ymm5, 320(%rdi)
vmovdqa %ymm6, 576(%rdi)
vmovdqa %ymm7, 832(%rdi)
vmovdqa 192(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm10
vpunpckhwd const0(%rip), %ymm11, %ymm9
vpslld $1, %ymm10, %ymm10
vpslld $1, %ymm9, %ymm9
vmovdqa 448(%rsp), %ymm2
vpunpcklwd const0(%rip), %ymm2, %ymm7
vpunpckhwd const0(%rip), %ymm2, %ymm2
vmovdqa 704(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm5
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm5, %ymm7, %ymm8
vpaddd %ymm6, %ymm2, %ymm3
vpsubd %ymm10, %ymm8, %ymm8
vpsubd %ymm9, %ymm3, %ymm3
vpsubd %ymm5, %ymm7, %ymm5
vpsubd %ymm6, %ymm2, %ymm6
vpsrld $1, %ymm5, %ymm5
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm5, %ymm5
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm5, %ymm6
vmovdqa 1728(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm2
vpunpckhwd const0(%rip), %ymm5, %ymm7
vpslld $1, %ymm2, %ymm2
vpslld $1, %ymm7, %ymm7
vpsubd %ymm2, %ymm8, %ymm8
vpsubd %ymm7, %ymm3, %ymm3
vpsrld $1, %ymm8, %ymm8
vpsrld $1, %ymm3, %ymm3
vpand mask32_to_16(%rip), %ymm8, %ymm8
vpand mask32_to_16(%rip), %ymm3, %ymm3
vpackusdw %ymm3, %ymm8, %ymm3
vmovdqa 960(%rsp), %ymm8
vpaddw 1216(%rsp), %ymm8, %ymm7
vpsubw 1216(%rsp), %ymm8, %ymm8
vpsrlw $2, %ymm8, %ymm8
vpsubw %ymm6, %ymm8, %ymm8
vpmullw %ymm14, %ymm8, %ymm8
vpsllw $1, %ymm11, %ymm2
vpsubw %ymm2, %ymm7, %ymm2
vpsllw $7, %ymm5, %ymm7
vpsubw %ymm7, %ymm2, %ymm7
vpsrlw $3, %ymm7, %ymm7
vpsubw %ymm3, %ymm7, %ymm7
vmovdqa 1472(%rsp), %ymm2
vpsubw %ymm11, %ymm2, %ymm2
vpmullw %ymm15, %ymm5, %ymm9
vpsubw %ymm9, %ymm2, %ymm9
vpmullw %ymm14, %ymm7, %ymm7
vpsubw %ymm7, %ymm3, %ymm3
vpmullw %ymm12, %ymm7, %ymm2
vpaddw %ymm2, %ymm3, %ymm2
vpmullw %ymm12, %ymm2, %ymm2
vpsubw %ymm2, %ymm9, %ymm2
vpmullw %ymm14, %ymm2, %ymm2
vpsubw %ymm6, %ymm2, %ymm2
vpsrlw $3, %ymm2, %ymm2
vpsubw %ymm8, %ymm2, %ymm2
vpsubw %ymm2, %ymm8, %ymm8
vpsubw %ymm8, %ymm6, %ymm6
vpmullw %ymm13, %ymm2, %ymm2
vpsubw %ymm2, %ymm6, %ymm6
vmovdqa 384(%rdi), %ymm9
vmovdqa 640(%rdi), %ymm10
vmovdqa 896(%rdi), %ymm4
vpaddw %ymm11, %ymm9, %ymm11
vpaddw %ymm6, %ymm10, %ymm6
vpaddw %ymm3, %ymm4, %ymm3
vpshufb shuf48_16(%rip), %ymm8, %ymm8
vpand mask3_5_3_5(%rip), %ymm8, %ymm4
vpand mask5_3_5_3(%rip), %ymm8, %ymm8
vpermq $206, %ymm4, %ymm4
vpand mask_keephigh(%rip), %ymm4, %ymm10
vpor %ymm10, %ymm8, %ymm8
vmovdqa 128(%rdi), %ymm10
vpaddw %ymm8, %ymm10, %ymm10
vmovdqa %ymm10, 128(%rdi)
vmovdqa %xmm4, 1984(%rsp)
vpshufb shuf48_16(%rip), %ymm7, %ymm7
vpand mask3_5_3_5(%rip), %ymm7, %ymm4
vpand mask5_3_5_3(%rip), %ymm7, %ymm7
vpermq $206, %ymm4, %ymm4
vpand mask_keephigh(%rip), %ymm4, %ymm10
vpor %ymm10, %ymm7, %ymm7
vpaddw %ymm7, %ymm11, %ymm11
vmovdqa %xmm4, 2240(%rsp)
vpshufb shuf48_16(%rip), %ymm2, %ymm2
vpand mask3_5_3_5(%rip), %ymm2, %ymm4
vpand mask5_3_5_3(%rip), %ymm2, %ymm2
vpermq $206, %ymm4, %ymm4
vpand mask_keephigh(%rip), %ymm4, %ymm10
vpor %ymm10, %ymm2, %ymm2
vpaddw %ymm2, %ymm6, %ymm6
vmovdqa %xmm4, 2496(%rsp)
vpshufb shuf48_16(%rip), %ymm5, %ymm5
vpand mask3_5_3_5(%rip), %ymm5, %ymm4
vpand mask5_3_5_3(%rip), %ymm5, %ymm5
vpermq $206, %ymm4, %ymm4
vpand mask_keephigh(%rip), %ymm4, %ymm10
vpor %ymm10, %ymm5, %ymm5
vpaddw %ymm5, %ymm3, %ymm3
vmovdqa %xmm4, 2752(%rsp)
vmovdqa %ymm11, 384(%rdi)
vmovdqa %ymm6, 640(%rdi)
vmovdqa %ymm3, 896(%rdi)
vmovdqa 224(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm2
vpunpckhwd const0(%rip), %ymm5, %ymm7
vpslld $1, %ymm2, %ymm2
vpslld $1, %ymm7, %ymm7
vmovdqa 480(%rsp), %ymm8
vpunpcklwd const0(%rip), %ymm8, %ymm3
vpunpckhwd const0(%rip), %ymm8, %ymm8
vmovdqa 736(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm11
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm11, %ymm3, %ymm4
vpaddd %ymm6, %ymm8, %ymm10
vpsubd %ymm2, %ymm4, %ymm4
vpsubd %ymm7, %ymm10, %ymm10
vpsubd %ymm11, %ymm3, %ymm11
vpsubd %ymm6, %ymm8, %ymm6
vpsrld $1, %ymm11, %ymm11
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm11, %ymm11
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm11, %ymm6
vmovdqa 1760(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm8
vpunpckhwd const0(%rip), %ymm11, %ymm3
vpslld $1, %ymm8, %ymm8
vpslld $1, %ymm3, %ymm3
vpsubd %ymm8, %ymm4, %ymm4
vpsubd %ymm3, %ymm10, %ymm10
vpsrld $1, %ymm4, %ymm4
vpsrld $1, %ymm10, %ymm10
vpand mask32_to_16(%rip), %ymm4, %ymm4
vpand mask32_to_16(%rip), %ymm10, %ymm10
vpackusdw %ymm10, %ymm4, %ymm10
vmovdqa 992(%rsp), %ymm4
vpaddw 1248(%rsp), %ymm4, %ymm3
vpsubw 1248(%rsp), %ymm4, %ymm4
vpsrlw $2, %ymm4, %ymm4
vpsubw %ymm6, %ymm4, %ymm4
vpmullw %ymm14, %ymm4, %ymm4
vpsllw $1, %ymm5, %ymm8
vpsubw %ymm8, %ymm3, %ymm8
vpsllw $7, %ymm11, %ymm3
vpsubw %ymm3, %ymm8, %ymm3
vpsrlw $3, %ymm3, %ymm3
vpsubw %ymm10, %ymm3, %ymm3
vmovdqa 1504(%rsp), %ymm8
vpsubw %ymm5, %ymm8, %ymm8
vpmullw %ymm15, %ymm11, %ymm7
vpsubw %ymm7, %ymm8, %ymm7
vpmullw %ymm14, %ymm3, %ymm3
vpsubw %ymm3, %ymm10, %ymm10
vpmullw %ymm12, %ymm3, %ymm8
vpaddw %ymm8, %ymm10, %ymm8
vpmullw %ymm12, %ymm8, %ymm8
vpsubw %ymm8, %ymm7, %ymm8
vpmullw %ymm14, %ymm8, %ymm8
vpsubw %ymm6, %ymm8, %ymm8
vpsrlw $3, %ymm8, %ymm8
vpsubw %ymm4, %ymm8, %ymm8
vpsubw %ymm8, %ymm4, %ymm4
vpsubw %ymm4, %ymm6, %ymm6
vpmullw %ymm13, %ymm8, %ymm8
vpsubw %ymm8, %ymm6, %ymm6
vmovdqa 448(%rdi), %ymm7
vmovdqa 704(%rdi), %ymm2
vmovdqa 960(%rdi), %ymm9
vpaddw %ymm5, %ymm7, %ymm5
vpaddw %ymm6, %ymm2, %ymm6
vpaddw %ymm10, %ymm9, %ymm10
vpshufb shuf48_16(%rip), %ymm4, %ymm4
vpand mask3_5_3_5(%rip), %ymm4, %ymm9
vpand mask5_3_5_3(%rip), %ymm4, %ymm4
vpermq $206, %ymm9, %ymm9
vpand mask_keephigh(%rip), %ymm9, %ymm2
vpor %ymm2, %ymm4, %ymm4
vmovdqa 192(%rdi), %ymm2
vpaddw %ymm4, %ymm2, %ymm2
vmovdqa %ymm2, 192(%rdi)
vmovdqa %xmm9, 2016(%rsp)
vpshufb shuf48_16(%rip), %ymm3, %ymm3
vpand mask3_5_3_5(%rip), %ymm3, %ymm9
vpand mask5_3_5_3(%rip), %ymm3, %ymm3
vpermq $206, %ymm9, %ymm9
vpand mask_keephigh(%rip), %ymm9, %ymm2
vpor %ymm2, %ymm3, %ymm3
vpaddw %ymm3, %ymm5, %ymm5
vmovdqa %xmm9, 2272(%rsp)
vpshufb shuf48_16(%rip), %ymm8, %ymm8
vpand mask3_5_3_5(%rip), %ymm8, %ymm9
vpand mask5_3_5_3(%rip), %ymm8, %ymm8
vpermq $206, %ymm9, %ymm9
vpand mask_keephigh(%rip), %ymm9, %ymm2
vpor %ymm2, %ymm8, %ymm8
vpaddw %ymm8, %ymm6, %ymm6
vmovdqa %xmm9, 2528(%rsp)
vpshufb shuf48_16(%rip), %ymm11, %ymm11
vpand mask3_5_3_5(%rip), %ymm11, %ymm9
vpand mask5_3_5_3(%rip), %ymm11, %ymm11
vpermq $206, %ymm9, %ymm9
vpand mask_keephigh(%rip), %ymm9, %ymm2
vpor %ymm2, %ymm11, %ymm11
vpaddw %ymm11, %ymm10, %ymm10
vmovdqa %xmm9, 2784(%rsp)
vmovdqa %ymm5, 448(%rdi)
vmovdqa %ymm6, 704(%rdi)
vmovdqa %ymm10, 960(%rdi)
vmovdqa 96(%r12), %ymm0
vpsubw 160(%r12), %ymm0, %ymm0
vmovdqa 352(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 224(%r12), %ymm1, %ymm1
vpsubw 32(%r12), %ymm0, %ymm0
vpaddw 288(%r12), %ymm0, %ymm0
vmovdqa 480(%r12), %ymm2
vpsubw 544(%r12), %ymm2, %ymm2
vmovdqa 736(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 608(%r12), %ymm3, %ymm3
vpsubw 416(%r12), %ymm2, %ymm2
vpaddw 672(%r12), %ymm2, %ymm2
vmovdqa 864(%r12), %ymm4
vpsubw 928(%r12), %ymm4, %ymm4
vmovdqa 1120(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 992(%r12), %ymm5, %ymm5
vpsubw 800(%r12), %ymm4, %ymm4
vpaddw 1056(%r12), %ymm4, %ymm4
vpsubw 416(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 32(%r12), %ymm1, %ymm1
vpaddw 800(%r12), %ymm1, %ymm1
vmovdqa 224(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 992(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 608(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 32(%r12), %ymm8
vmovdqa 608(%r12), %ymm9
vmovdqa %ymm8, 0(%rsp)
vmovdqa %ymm0, 32(%rsp)
vmovdqa %ymm1, 64(%rsp)
vmovdqa %ymm7, 96(%rsp)
vmovdqa %ymm5, 128(%rsp)
vmovdqa %ymm2, 160(%rsp)
vmovdqa %ymm3, 192(%rsp)
vmovdqa %ymm9, 224(%rsp)
vmovdqa 1248(%r12), %ymm0
vpsubw 1312(%r12), %ymm0, %ymm0
vmovdqa 1504(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 1376(%r12), %ymm1, %ymm1
vpsubw 1184(%r12), %ymm0, %ymm0
vpaddw 1440(%r12), %ymm0, %ymm0
vmovdqa 1632(%r12), %ymm2
vpsubw 1696(%r12), %ymm2, %ymm2
vmovdqa 1888(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 1760(%r12), %ymm3, %ymm3
vpsubw 1568(%r12), %ymm2, %ymm2
vpaddw 1824(%r12), %ymm2, %ymm2
vmovdqa 2016(%r12), %ymm4
vpsubw 2080(%r12), %ymm4, %ymm4
vmovdqa 2272(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 2144(%r12), %ymm5, %ymm5
vpsubw 1952(%r12), %ymm4, %ymm4
vpaddw 2208(%r12), %ymm4, %ymm4
vpsubw 1568(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 1184(%r12), %ymm1, %ymm1
vpaddw 1952(%r12), %ymm1, %ymm1
vmovdqa 1376(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 2144(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 1760(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 1184(%r12), %ymm8
vmovdqa 1760(%r12), %ymm9
vmovdqa %ymm8, 256(%rsp)
vmovdqa %ymm0, 288(%rsp)
vmovdqa %ymm1, 320(%rsp)
vmovdqa %ymm7, 352(%rsp)
vmovdqa %ymm5, 384(%rsp)
vmovdqa %ymm2, 416(%rsp)
vmovdqa %ymm3, 448(%rsp)
vmovdqa %ymm9, 480(%rsp)
vmovdqa 2400(%r12), %ymm0
vpsubw 2464(%r12), %ymm0, %ymm0
vmovdqa 2656(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 2528(%r12), %ymm1, %ymm1
vpsubw 2336(%r12), %ymm0, %ymm0
vpaddw 2592(%r12), %ymm0, %ymm0
vmovdqa 2784(%r12), %ymm2
vpsubw 2848(%r12), %ymm2, %ymm2
vmovdqa 3040(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 2912(%r12), %ymm3, %ymm3
vpsubw 2720(%r12), %ymm2, %ymm2
vpaddw 2976(%r12), %ymm2, %ymm2
vmovdqa 3168(%r12), %ymm4
vpsubw 3232(%r12), %ymm4, %ymm4
vmovdqa 3424(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 3296(%r12), %ymm5, %ymm5
vpsubw 3104(%r12), %ymm4, %ymm4
vpaddw 3360(%r12), %ymm4, %ymm4
vpsubw 2720(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 2336(%r12), %ymm1, %ymm1
vpaddw 3104(%r12), %ymm1, %ymm1
vmovdqa 2528(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 3296(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 2912(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 2336(%r12), %ymm8
vmovdqa 2912(%r12), %ymm9
vmovdqa %ymm8, 512(%rsp)
vmovdqa %ymm0, 544(%rsp)
vmovdqa %ymm1, 576(%rsp)
vmovdqa %ymm7, 608(%rsp)
vmovdqa %ymm5, 640(%rsp)
vmovdqa %ymm2, 672(%rsp)
vmovdqa %ymm3, 704(%rsp)
vmovdqa %ymm9, 736(%rsp)
vmovdqa 3552(%r12), %ymm0
vpsubw 3616(%r12), %ymm0, %ymm0
vmovdqa 3808(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 3680(%r12), %ymm1, %ymm1
vpsubw 3488(%r12), %ymm0, %ymm0
vpaddw 3744(%r12), %ymm0, %ymm0
vmovdqa 3936(%r12), %ymm2
vpsubw 4000(%r12), %ymm2, %ymm2
vmovdqa 4192(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 4064(%r12), %ymm3, %ymm3
vpsubw 3872(%r12), %ymm2, %ymm2
vpaddw 4128(%r12), %ymm2, %ymm2
vmovdqa 4320(%r12), %ymm4
vpsubw 4384(%r12), %ymm4, %ymm4
vmovdqa 4576(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 4448(%r12), %ymm5, %ymm5
vpsubw 4256(%r12), %ymm4, %ymm4
vpaddw 4512(%r12), %ymm4, %ymm4
vpsubw 3872(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 3488(%r12), %ymm1, %ymm1
vpaddw 4256(%r12), %ymm1, %ymm1
vmovdqa 3680(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 4448(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 4064(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 3488(%r12), %ymm8
vmovdqa 4064(%r12), %ymm9
vmovdqa %ymm8, 768(%rsp)
vmovdqa %ymm0, 800(%rsp)
vmovdqa %ymm1, 832(%rsp)
vmovdqa %ymm7, 864(%rsp)
vmovdqa %ymm5, 896(%rsp)
vmovdqa %ymm2, 928(%rsp)
vmovdqa %ymm3, 960(%rsp)
vmovdqa %ymm9, 992(%rsp)
vmovdqa 4704(%r12), %ymm0
vpsubw 4768(%r12), %ymm0, %ymm0
vmovdqa 4960(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4832(%r12), %ymm1, %ymm1
vpsubw 4640(%r12), %ymm0, %ymm0
vpaddw 4896(%r12), %ymm0, %ymm0
vmovdqa 5088(%r12), %ymm2
vpsubw 5152(%r12), %ymm2, %ymm2
vmovdqa 5344(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 5216(%r12), %ymm3, %ymm3
vpsubw 5024(%r12), %ymm2, %ymm2
vpaddw 5280(%r12), %ymm2, %ymm2
vmovdqa 5472(%r12), %ymm4
vpsubw 5536(%r12), %ymm4, %ymm4
vmovdqa 5728(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 5600(%r12), %ymm5, %ymm5
vpsubw 5408(%r12), %ymm4, %ymm4
vpaddw 5664(%r12), %ymm4, %ymm4
vpsubw 5024(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 4640(%r12), %ymm1, %ymm1
vpaddw 5408(%r12), %ymm1, %ymm1
vmovdqa 4832(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 5600(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 5216(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 4640(%r12), %ymm8
vmovdqa 5216(%r12), %ymm9
vmovdqa %ymm8, 1024(%rsp)
vmovdqa %ymm0, 1056(%rsp)
vmovdqa %ymm1, 1088(%rsp)
vmovdqa %ymm7, 1120(%rsp)
vmovdqa %ymm5, 1152(%rsp)
vmovdqa %ymm2, 1184(%rsp)
vmovdqa %ymm3, 1216(%rsp)
vmovdqa %ymm9, 1248(%rsp)
vmovdqa 5856(%r12), %ymm0
vpsubw 5920(%r12), %ymm0, %ymm0
vmovdqa 6112(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5984(%r12), %ymm1, %ymm1
vpsubw 5792(%r12), %ymm0, %ymm0
vpaddw 6048(%r12), %ymm0, %ymm0
vmovdqa 6240(%r12), %ymm2
vpsubw 6304(%r12), %ymm2, %ymm2
vmovdqa 6496(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 6368(%r12), %ymm3, %ymm3
vpsubw 6176(%r12), %ymm2, %ymm2
vpaddw 6432(%r12), %ymm2, %ymm2
vmovdqa 6624(%r12), %ymm4
vpsubw 6688(%r12), %ymm4, %ymm4
vmovdqa 6880(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 6752(%r12), %ymm5, %ymm5
vpsubw 6560(%r12), %ymm4, %ymm4
vpaddw 6816(%r12), %ymm4, %ymm4
vpsubw 6176(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 5792(%r12), %ymm1, %ymm1
vpaddw 6560(%r12), %ymm1, %ymm1
vmovdqa 5984(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 6752(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 6368(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 5792(%r12), %ymm8
vmovdqa 6368(%r12), %ymm9
vmovdqa %ymm8, 1280(%rsp)
vmovdqa %ymm0, 1312(%rsp)
vmovdqa %ymm1, 1344(%rsp)
vmovdqa %ymm7, 1376(%rsp)
vmovdqa %ymm5, 1408(%rsp)
vmovdqa %ymm2, 1440(%rsp)
vmovdqa %ymm3, 1472(%rsp)
vmovdqa %ymm9, 1504(%rsp)
vmovdqa 7008(%r12), %ymm0
vpsubw 7072(%r12), %ymm0, %ymm0
vmovdqa 7264(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 7136(%r12), %ymm1, %ymm1
vpsubw 6944(%r12), %ymm0, %ymm0
vpaddw 7200(%r12), %ymm0, %ymm0
vmovdqa 7392(%r12), %ymm2
vpsubw 7456(%r12), %ymm2, %ymm2
vmovdqa 7648(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 7520(%r12), %ymm3, %ymm3
vpsubw 7328(%r12), %ymm2, %ymm2
vpaddw 7584(%r12), %ymm2, %ymm2
vmovdqa 7776(%r12), %ymm4
vpsubw 7840(%r12), %ymm4, %ymm4
vmovdqa 8032(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 7904(%r12), %ymm5, %ymm5
vpsubw 7712(%r12), %ymm4, %ymm4
vpaddw 7968(%r12), %ymm4, %ymm4
vpsubw 7328(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 6944(%r12), %ymm1, %ymm1
vpaddw 7712(%r12), %ymm1, %ymm1
vmovdqa 7136(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 7904(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 7520(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 6944(%r12), %ymm8
vmovdqa 7520(%r12), %ymm9
vmovdqa %ymm8, 1536(%rsp)
vmovdqa %ymm0, 1568(%rsp)
vmovdqa %ymm1, 1600(%rsp)
vmovdqa %ymm7, 1632(%rsp)
vmovdqa %ymm5, 1664(%rsp)
vmovdqa %ymm2, 1696(%rsp)
vmovdqa %ymm3, 1728(%rsp)
vmovdqa %ymm9, 1760(%rsp)
vmovdqa 0(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm8
vpunpckhwd const0(%rip), %ymm11, %ymm3
vpslld $1, %ymm8, %ymm8
vpslld $1, %ymm3, %ymm3
vmovdqa 256(%rsp), %ymm4
vpunpcklwd const0(%rip), %ymm4, %ymm10
vpunpckhwd const0(%rip), %ymm4, %ymm4
vmovdqa 512(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm5
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm5, %ymm10, %ymm9
vpaddd %ymm6, %ymm4, %ymm2
vpsubd %ymm8, %ymm9, %ymm9
vpsubd %ymm3, %ymm2, %ymm2
vpsubd %ymm5, %ymm10, %ymm5
vpsubd %ymm6, %ymm4, %ymm6
vpsrld $1, %ymm5, %ymm5
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm5, %ymm5
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm5, %ymm6
vmovdqa 1536(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm4
vpunpckhwd const0(%rip), %ymm5, %ymm10
vpslld $1, %ymm4, %ymm4
vpslld $1, %ymm10, %ymm10
vpsubd %ymm4, %ymm9, %ymm9
vpsubd %ymm10, %ymm2, %ymm2
vpsrld $1, %ymm9, %ymm9
vpsrld $1, %ymm2, %ymm2
vpand mask32_to_16(%rip), %ymm9, %ymm9
vpand mask32_to_16(%rip), %ymm2, %ymm2
vpackusdw %ymm2, %ymm9, %ymm2
vmovdqa 768(%rsp), %ymm9
vpaddw 1024(%rsp), %ymm9, %ymm10
vpsubw 1024(%rsp), %ymm9, %ymm9
vpsrlw $2, %ymm9, %ymm9
vpsubw %ymm6, %ymm9, %ymm9
vpmullw %ymm14, %ymm9, %ymm9
vpsllw $1, %ymm11, %ymm4
vpsubw %ymm4, %ymm10, %ymm4
vpsllw $7, %ymm5, %ymm10
vpsubw %ymm10, %ymm4, %ymm10
vpsrlw $3, %ymm10, %ymm10
vpsubw %ymm2, %ymm10, %ymm10
vmovdqa 1280(%rsp), %ymm4
vpsubw %ymm11, %ymm4, %ymm4
vpmullw %ymm15, %ymm5, %ymm3
vpsubw %ymm3, %ymm4, %ymm3
vpmullw %ymm14, %ymm10, %ymm10
vpsubw %ymm10, %ymm2, %ymm2
vpmullw %ymm12, %ymm10, %ymm4
vpaddw %ymm4, %ymm2, %ymm4
vpmullw %ymm12, %ymm4, %ymm4
vpsubw %ymm4, %ymm3, %ymm4
vpmullw %ymm14, %ymm4, %ymm4
vpsubw %ymm6, %ymm4, %ymm4
vpsrlw $3, %ymm4, %ymm4
vpsubw %ymm9, %ymm4, %ymm4
vpsubw %ymm4, %ymm9, %ymm9
vpsubw %ymm9, %ymm6, %ymm6
vpmullw %ymm13, %ymm4, %ymm4
vpsubw %ymm4, %ymm6, %ymm6
vpshufb shuf48_16(%rip), %ymm10, %ymm10
vpand mask3_5_3_5(%rip), %ymm10, %ymm3
vpand mask5_3_5_3(%rip), %ymm10, %ymm10
vpermq $206, %ymm3, %ymm3
vpand mask_keephigh(%rip), %ymm3, %ymm8
vpor %ymm8, %ymm10, %ymm10
vpaddw 2048(%rsp), %ymm11, %ymm11
vpaddw %ymm10, %ymm11, %ymm11
vmovdqa %xmm3, 2048(%rsp)
vpshufb shuf48_16(%rip), %ymm4, %ymm4
vpand mask3_5_3_5(%rip), %ymm4, %ymm3
vpand mask5_3_5_3(%rip), %ymm4, %ymm4
vpermq $206, %ymm3, %ymm3
vpand mask_keephigh(%rip), %ymm3, %ymm8
vpor %ymm8, %ymm4, %ymm4
vpaddw 2304(%rsp), %ymm6, %ymm6
vpaddw %ymm4, %ymm6, %ymm6
vmovdqa %xmm3, 2304(%rsp)
vpshufb shuf48_16(%rip), %ymm5, %ymm5
vpand mask3_5_3_5(%rip), %ymm5, %ymm3
vpand mask5_3_5_3(%rip), %ymm5, %ymm5
vpermq $206, %ymm3, %ymm3
vpand mask_keephigh(%rip), %ymm3, %ymm8
vpor %ymm8, %ymm5, %ymm5
vpaddw 2560(%rsp), %ymm2, %ymm2
vpaddw %ymm5, %ymm2, %ymm2
vmovdqa %xmm3, 2560(%rsp)
vmovdqa %ymm11, 32(%rdi)
vmovdqa %ymm6, 288(%rdi)
vmovdqa %ymm2, 544(%rdi)
vmovdqa %ymm9, 800(%rdi)
vmovdqa 32(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm4
vpunpckhwd const0(%rip), %ymm5, %ymm10
vpslld $1, %ymm4, %ymm4
vpslld $1, %ymm10, %ymm10
vmovdqa 288(%rsp), %ymm9
vpunpcklwd const0(%rip), %ymm9, %ymm2
vpunpckhwd const0(%rip), %ymm9, %ymm9
vmovdqa 544(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm11
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm11, %ymm2, %ymm3
vpaddd %ymm6, %ymm9, %ymm8
vpsubd %ymm4, %ymm3, %ymm3
vpsubd %ymm10, %ymm8, %ymm8
vpsubd %ymm11, %ymm2, %ymm11
vpsubd %ymm6, %ymm9, %ymm6
vpsrld $1, %ymm11, %ymm11
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm11, %ymm11
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm11, %ymm6
vmovdqa 1568(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm9
vpunpckhwd const0(%rip), %ymm11, %ymm2
vpslld $1, %ymm9, %ymm9
vpslld $1, %ymm2, %ymm2
vpsubd %ymm9, %ymm3, %ymm3
vpsubd %ymm2, %ymm8, %ymm8
vpsrld $1, %ymm3, %ymm3
vpsrld $1, %ymm8, %ymm8
vpand mask32_to_16(%rip), %ymm3, %ymm3
vpand mask32_to_16(%rip), %ymm8, %ymm8
vpackusdw %ymm8, %ymm3, %ymm8
vmovdqa 800(%rsp), %ymm3
vpaddw 1056(%rsp), %ymm3, %ymm2
vpsubw 1056(%rsp), %ymm3, %ymm3
vpsrlw $2, %ymm3, %ymm3
vpsubw %ymm6, %ymm3, %ymm3
vpmullw %ymm14, %ymm3, %ymm3
vpsllw $1, %ymm5, %ymm9
vpsubw %ymm9, %ymm2, %ymm9
vpsllw $7, %ymm11, %ymm2
vpsubw %ymm2, %ymm9, %ymm2
vpsrlw $3, %ymm2, %ymm2
vpsubw %ymm8, %ymm2, %ymm2
vmovdqa 1312(%rsp), %ymm9
vpsubw %ymm5, %ymm9, %ymm9
vpmullw %ymm15, %ymm11, %ymm10
vpsubw %ymm10, %ymm9, %ymm10
vpmullw %ymm14, %ymm2, %ymm2
vpsubw %ymm2, %ymm8, %ymm8
vpmullw %ymm12, %ymm2, %ymm9
vpaddw %ymm9, %ymm8, %ymm9
vpmullw %ymm12, %ymm9, %ymm9
vpsubw %ymm9, %ymm10, %ymm9
vpmullw %ymm14, %ymm9, %ymm9
vpsubw %ymm6, %ymm9, %ymm9
vpsrlw $3, %ymm9, %ymm9
vpsubw %ymm3, %ymm9, %ymm9
vpsubw %ymm9, %ymm3, %ymm3
vpsubw %ymm3, %ymm6, %ymm6
vpmullw %ymm13, %ymm9, %ymm9
vpsubw %ymm9, %ymm6, %ymm6
vpshufb shuf48_16(%rip), %ymm2, %ymm2
vpand mask3_5_3_5(%rip), %ymm2, %ymm10
vpand mask5_3_5_3(%rip), %ymm2, %ymm2
vpermq $206, %ymm10, %ymm10
vpand mask_keephigh(%rip), %ymm10, %ymm4
vpor %ymm4, %ymm2, %ymm2
vpaddw 2080(%rsp), %ymm5, %ymm5
vpaddw %ymm2, %ymm5, %ymm5
vmovdqa %xmm10, 2080(%rsp)
vpshufb shuf48_16(%rip), %ymm9, %ymm9
vpand mask3_5_3_5(%rip), %ymm9, %ymm10
vpand mask5_3_5_3(%rip), %ymm9, %ymm9
vpermq $206, %ymm10, %ymm10
vpand mask_keephigh(%rip), %ymm10, %ymm4
vpor %ymm4, %ymm9, %ymm9
vpaddw 2336(%rsp), %ymm6, %ymm6
vpaddw %ymm9, %ymm6, %ymm6
vmovdqa %xmm10, 2336(%rsp)
vpshufb shuf48_16(%rip), %ymm11, %ymm11
vpand mask3_5_3_5(%rip), %ymm11, %ymm10
vpand mask5_3_5_3(%rip), %ymm11, %ymm11
vpermq $206, %ymm10, %ymm10
vpand mask_keephigh(%rip), %ymm10, %ymm4
vpor %ymm4, %ymm11, %ymm11
vpaddw 2592(%rsp), %ymm8, %ymm8
vpaddw %ymm11, %ymm8, %ymm8
vmovdqa %xmm10, 2592(%rsp)
vmovdqa %ymm5, 96(%rdi)
vmovdqa %ymm6, 352(%rdi)
vmovdqa %ymm8, 608(%rdi)
vmovdqa %ymm3, 864(%rdi)
vmovdqa 64(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm9
vpunpckhwd const0(%rip), %ymm11, %ymm2
vpslld $1, %ymm9, %ymm9
vpslld $1, %ymm2, %ymm2
vmovdqa 320(%rsp), %ymm3
vpunpcklwd const0(%rip), %ymm3, %ymm8
vpunpckhwd const0(%rip), %ymm3, %ymm3
vmovdqa 576(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm5
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm5, %ymm8, %ymm10
vpaddd %ymm6, %ymm3, %ymm4
vpsubd %ymm9, %ymm10, %ymm10
vpsubd %ymm2, %ymm4, %ymm4
vpsubd %ymm5, %ymm8, %ymm5
vpsubd %ymm6, %ymm3, %ymm6
vpsrld $1, %ymm5, %ymm5
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm5, %ymm5
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm5, %ymm6
vmovdqa 1600(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm3
vpunpckhwd const0(%rip), %ymm5, %ymm8
vpslld $1, %ymm3, %ymm3
vpslld $1, %ymm8, %ymm8
vpsubd %ymm3, %ymm10, %ymm10
vpsubd %ymm8, %ymm4, %ymm4
vpsrld $1, %ymm10, %ymm10
vpsrld $1, %ymm4, %ymm4
vpand mask32_to_16(%rip), %ymm10, %ymm10
vpand mask32_to_16(%rip), %ymm4, %ymm4
vpackusdw %ymm4, %ymm10, %ymm4
vmovdqa 832(%rsp), %ymm10
vpaddw 1088(%rsp), %ymm10, %ymm8
vpsubw 1088(%rsp), %ymm10, %ymm10
vpsrlw $2, %ymm10, %ymm10
vpsubw %ymm6, %ymm10, %ymm10
vpmullw %ymm14, %ymm10, %ymm10
vpsllw $1, %ymm11, %ymm3
vpsubw %ymm3, %ymm8, %ymm3
vpsllw $7, %ymm5, %ymm8
vpsubw %ymm8, %ymm3, %ymm8
vpsrlw $3, %ymm8, %ymm8
vpsubw %ymm4, %ymm8, %ymm8
vmovdqa 1344(%rsp), %ymm3
vpsubw %ymm11, %ymm3, %ymm3
vpmullw %ymm15, %ymm5, %ymm2
vpsubw %ymm2, %ymm3, %ymm2
vpmullw %ymm14, %ymm8, %ymm8
vpsubw %ymm8, %ymm4, %ymm4
vpmullw %ymm12, %ymm8, %ymm3
vpaddw %ymm3, %ymm4, %ymm3
vpmullw %ymm12, %ymm3, %ymm3
vpsubw %ymm3, %ymm2, %ymm3
vpmullw %ymm14, %ymm3, %ymm3
vpsubw %ymm6, %ymm3, %ymm3
vpsrlw $3, %ymm3, %ymm3
vpsubw %ymm10, %ymm3, %ymm3
vpsubw %ymm3, %ymm10, %ymm10
vpsubw %ymm10, %ymm6, %ymm6
vpmullw %ymm13, %ymm3, %ymm3
vpsubw %ymm3, %ymm6, %ymm6
vpshufb shuf48_16(%rip), %ymm8, %ymm8
vpand mask3_5_3_5(%rip), %ymm8, %ymm2
vpand mask5_3_5_3(%rip), %ymm8, %ymm8
vpermq $206, %ymm2, %ymm2
vpand mask_keephigh(%rip), %ymm2, %ymm9
vpor %ymm9, %ymm8, %ymm8
vpaddw 2112(%rsp), %ymm11, %ymm11
vpaddw %ymm8, %ymm11, %ymm11
vmovdqa %xmm2, 2112(%rsp)
vpshufb shuf48_16(%rip), %ymm3, %ymm3
vpand mask3_5_3_5(%rip), %ymm3, %ymm2
vpand mask5_3_5_3(%rip), %ymm3, %ymm3
vpermq $206, %ymm2, %ymm2
vpand mask_keephigh(%rip), %ymm2, %ymm9
vpor %ymm9, %ymm3, %ymm3
vpaddw 2368(%rsp), %ymm6, %ymm6
vpaddw %ymm3, %ymm6, %ymm6
vmovdqa %xmm2, 2368(%rsp)
vpshufb shuf48_16(%rip), %ymm5, %ymm5
vpand mask3_5_3_5(%rip), %ymm5, %ymm2
vpand mask5_3_5_3(%rip), %ymm5, %ymm5
vpermq $206, %ymm2, %ymm2
vpand mask_keephigh(%rip), %ymm2, %ymm9
vpor %ymm9, %ymm5, %ymm5
vpaddw 2624(%rsp), %ymm4, %ymm4
vpaddw %ymm5, %ymm4, %ymm4
vmovdqa %xmm2, 2624(%rsp)
vmovdqa %ymm11, 160(%rdi)
vmovdqa %ymm6, 416(%rdi)
vmovdqa %ymm4, 672(%rdi)
vmovdqa %ymm10, 928(%rdi)
vmovdqa 96(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm3
vpunpckhwd const0(%rip), %ymm5, %ymm8
vpslld $1, %ymm3, %ymm3
vpslld $1, %ymm8, %ymm8
vmovdqa 352(%rsp), %ymm10
vpunpcklwd const0(%rip), %ymm10, %ymm4
vpunpckhwd const0(%rip), %ymm10, %ymm10
vmovdqa 608(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm11
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm11, %ymm4, %ymm2
vpaddd %ymm6, %ymm10, %ymm9
vpsubd %ymm3, %ymm2, %ymm2
vpsubd %ymm8, %ymm9, %ymm9
vpsubd %ymm11, %ymm4, %ymm11
vpsubd %ymm6, %ymm10, %ymm6
vpsrld $1, %ymm11, %ymm11
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm11, %ymm11
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm11, %ymm6
vmovdqa 1632(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm10
vpunpckhwd const0(%rip), %ymm11, %ymm4
vpslld $1, %ymm10, %ymm10
vpslld $1, %ymm4, %ymm4
vpsubd %ymm10, %ymm2, %ymm2
vpsubd %ymm4, %ymm9, %ymm9
vpsrld $1, %ymm2, %ymm2
vpsrld $1, %ymm9, %ymm9
vpand mask32_to_16(%rip), %ymm2, %ymm2
vpand mask32_to_16(%rip), %ymm9, %ymm9
vpackusdw %ymm9, %ymm2, %ymm9
vmovdqa 864(%rsp), %ymm2
vpaddw 1120(%rsp), %ymm2, %ymm4
vpsubw 1120(%rsp), %ymm2, %ymm2
vpsrlw $2, %ymm2, %ymm2
vpsubw %ymm6, %ymm2, %ymm2
vpmullw %ymm14, %ymm2, %ymm2
vpsllw $1, %ymm5, %ymm10
vpsubw %ymm10, %ymm4, %ymm10
vpsllw $7, %ymm11, %ymm4
vpsubw %ymm4, %ymm10, %ymm4
vpsrlw $3, %ymm4, %ymm4
vpsubw %ymm9, %ymm4, %ymm4
vmovdqa 1376(%rsp), %ymm10
vpsubw %ymm5, %ymm10, %ymm10
vpmullw %ymm15, %ymm11, %ymm8
vpsubw %ymm8, %ymm10, %ymm8
vpmullw %ymm14, %ymm4, %ymm4
vpsubw %ymm4, %ymm9, %ymm9
vpmullw %ymm12, %ymm4, %ymm10
vpaddw %ymm10, %ymm9, %ymm10
vpmullw %ymm12, %ymm10, %ymm10
vpsubw %ymm10, %ymm8, %ymm10
vpmullw %ymm14, %ymm10, %ymm10
vpsubw %ymm6, %ymm10, %ymm10
vpsrlw $3, %ymm10, %ymm10
vpsubw %ymm2, %ymm10, %ymm10
vpsubw %ymm10, %ymm2, %ymm2
vpsubw %ymm2, %ymm6, %ymm6
vpmullw %ymm13, %ymm10, %ymm10
vpsubw %ymm10, %ymm6, %ymm6
vpshufb shuf48_16(%rip), %ymm4, %ymm4
vpand mask3_5_3_5(%rip), %ymm4, %ymm8
vpand mask5_3_5_3(%rip), %ymm4, %ymm4
vpermq $206, %ymm8, %ymm8
vpand mask_keephigh(%rip), %ymm8, %ymm3
vpor %ymm3, %ymm4, %ymm4
vpaddw 2144(%rsp), %ymm5, %ymm5
vpaddw %ymm4, %ymm5, %ymm5
vmovdqa %xmm8, 2144(%rsp)
vpshufb shuf48_16(%rip), %ymm10, %ymm10
vpand mask3_5_3_5(%rip), %ymm10, %ymm8
vpand mask5_3_5_3(%rip), %ymm10, %ymm10
vpermq $206, %ymm8, %ymm8
vpand mask_keephigh(%rip), %ymm8, %ymm3
vpor %ymm3, %ymm10, %ymm10
vpaddw 2400(%rsp), %ymm6, %ymm6
vpaddw %ymm10, %ymm6, %ymm6
vmovdqa %xmm8, 2400(%rsp)
vpshufb shuf48_16(%rip), %ymm11, %ymm11
vpand mask3_5_3_5(%rip), %ymm11, %ymm8
vpand mask5_3_5_3(%rip), %ymm11, %ymm11
vpermq $206, %ymm8, %ymm8
vpand mask_keephigh(%rip), %ymm8, %ymm3
vpor %ymm3, %ymm11, %ymm11
vpaddw 2656(%rsp), %ymm9, %ymm9
vpaddw %ymm11, %ymm9, %ymm9
vmovdqa %xmm8, 2656(%rsp)
vmovdqa %ymm5, 224(%rdi)
vextracti128 $1, %ymm5, %xmm5
vpshufb shufmin5_mask3(%rip), %ymm5, %ymm5
vmovdqa %xmm5, 1792(%rsp)
vmovdqa %ymm6, 480(%rdi)
vextracti128 $1, %ymm6, %xmm6
vpshufb shufmin5_mask3(%rip), %ymm6, %ymm6
vmovdqa %xmm6, 1824(%rsp)
vmovdqa %ymm9, 736(%rdi)
vextracti128 $1, %ymm9, %xmm9
vpshufb shufmin5_mask3(%rip), %ymm9, %ymm9
vmovdqa %xmm9, 1856(%rsp)
vmovdqa %ymm2, 992(%rdi)
vextracti128 $1, %ymm2, %xmm2
vpshufb shufmin5_mask3(%rip), %ymm2, %ymm2
vmovdqa %xmm2, 1888(%rsp)
vmovdqa 128(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm10
vpunpckhwd const0(%rip), %ymm11, %ymm4
vpslld $1, %ymm10, %ymm10
vpslld $1, %ymm4, %ymm4
vmovdqa 384(%rsp), %ymm2
vpunpcklwd const0(%rip), %ymm2, %ymm9
vpunpckhwd const0(%rip), %ymm2, %ymm2
vmovdqa 640(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm5
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm5, %ymm9, %ymm8
vpaddd %ymm6, %ymm2, %ymm3
vpsubd %ymm10, %ymm8, %ymm8
vpsubd %ymm4, %ymm3, %ymm3
vpsubd %ymm5, %ymm9, %ymm5
vpsubd %ymm6, %ymm2, %ymm6
vpsrld $1, %ymm5, %ymm5
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm5, %ymm5
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm5, %ymm6
vmovdqa 1664(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm2
vpunpckhwd const0(%rip), %ymm5, %ymm9
vpslld $1, %ymm2, %ymm2
vpslld $1, %ymm9, %ymm9
vpsubd %ymm2, %ymm8, %ymm8
vpsubd %ymm9, %ymm3, %ymm3
vpsrld $1, %ymm8, %ymm8
vpsrld $1, %ymm3, %ymm3
vpand mask32_to_16(%rip), %ymm8, %ymm8
vpand mask32_to_16(%rip), %ymm3, %ymm3
vpackusdw %ymm3, %ymm8, %ymm3
vmovdqa 896(%rsp), %ymm8
vpaddw 1152(%rsp), %ymm8, %ymm9
vpsubw 1152(%rsp), %ymm8, %ymm8
vpsrlw $2, %ymm8, %ymm8
vpsubw %ymm6, %ymm8, %ymm8
vpmullw %ymm14, %ymm8, %ymm8
vpsllw $1, %ymm11, %ymm2
vpsubw %ymm2, %ymm9, %ymm2
vpsllw $7, %ymm5, %ymm9
vpsubw %ymm9, %ymm2, %ymm9
vpsrlw $3, %ymm9, %ymm9
vpsubw %ymm3, %ymm9, %ymm9
vmovdqa 1408(%rsp), %ymm2
vpsubw %ymm11, %ymm2, %ymm2
vpmullw %ymm15, %ymm5, %ymm4
vpsubw %ymm4, %ymm2, %ymm4
vpmullw %ymm14, %ymm9, %ymm9
vpsubw %ymm9, %ymm3, %ymm3
vpmullw %ymm12, %ymm9, %ymm2
vpaddw %ymm2, %ymm3, %ymm2
vpmullw %ymm12, %ymm2, %ymm2
vpsubw %ymm2, %ymm4, %ymm2
vpmullw %ymm14, %ymm2, %ymm2
vpsubw %ymm6, %ymm2, %ymm2
vpsrlw $3, %ymm2, %ymm2
vpsubw %ymm8, %ymm2, %ymm2
vpsubw %ymm2, %ymm8, %ymm8
vpsubw %ymm8, %ymm6, %ymm6
vpmullw %ymm13, %ymm2, %ymm2
vpsubw %ymm2, %ymm6, %ymm6
vmovdqa 288(%rdi), %ymm4
vmovdqa 544(%rdi), %ymm10
vmovdqa 800(%rdi), %ymm7
vpaddw %ymm11, %ymm4, %ymm11
vpaddw %ymm6, %ymm10, %ymm6
vpaddw %ymm3, %ymm7, %ymm3
vpshufb shuf48_16(%rip), %ymm8, %ymm8
vpand mask3_5_3_5(%rip), %ymm8, %ymm7
vpand mask5_3_5_3(%rip), %ymm8, %ymm8
vpermq $206, %ymm7, %ymm7
vpand mask_keephigh(%rip), %ymm7, %ymm10
vpor %ymm10, %ymm8, %ymm8
vmovdqa 32(%rdi), %ymm10
vpaddw 1920(%rsp), %ymm10, %ymm10
vpaddw %ymm8, %ymm10, %ymm10
vmovdqa %ymm10, 32(%rdi)
vmovdqa %xmm7, 1920(%rsp)
vpshufb shuf48_16(%rip), %ymm9, %ymm9
vpand mask3_5_3_5(%rip), %ymm9, %ymm7
vpand mask5_3_5_3(%rip), %ymm9, %ymm9
vpermq $206, %ymm7, %ymm7
vpand mask_keephigh(%rip), %ymm7, %ymm10
vpor %ymm10, %ymm9, %ymm9
vpaddw 2176(%rsp), %ymm11, %ymm11
vpaddw %ymm9, %ymm11, %ymm11
vmovdqa %xmm7, 2176(%rsp)
vpshufb shuf48_16(%rip), %ymm2, %ymm2
vpand mask3_5_3_5(%rip), %ymm2, %ymm7
vpand mask5_3_5_3(%rip), %ymm2, %ymm2
vpermq $206, %ymm7, %ymm7
vpand mask_keephigh(%rip), %ymm7, %ymm10
vpor %ymm10, %ymm2, %ymm2
vpaddw 2432(%rsp), %ymm6, %ymm6
vpaddw %ymm2, %ymm6, %ymm6
vmovdqa %xmm7, 2432(%rsp)
vpshufb shuf48_16(%rip), %ymm5, %ymm5
vpand mask3_5_3_5(%rip), %ymm5, %ymm7
vpand mask5_3_5_3(%rip), %ymm5, %ymm5
vpermq $206, %ymm7, %ymm7
vpand mask_keephigh(%rip), %ymm7, %ymm10
vpor %ymm10, %ymm5, %ymm5
vpaddw 2688(%rsp), %ymm3, %ymm3
vpaddw %ymm5, %ymm3, %ymm3
vmovdqa %xmm7, 2688(%rsp)
vmovdqa %ymm11, 288(%rdi)
vmovdqa %ymm6, 544(%rdi)
vmovdqa %ymm3, 800(%rdi)
vmovdqa 160(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm2
vpunpckhwd const0(%rip), %ymm5, %ymm9
vpslld $1, %ymm2, %ymm2
vpslld $1, %ymm9, %ymm9
vmovdqa 416(%rsp), %ymm8
vpunpcklwd const0(%rip), %ymm8, %ymm3
vpunpckhwd const0(%rip), %ymm8, %ymm8
vmovdqa 672(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm11
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm11, %ymm3, %ymm7
vpaddd %ymm6, %ymm8, %ymm10
vpsubd %ymm2, %ymm7, %ymm7
vpsubd %ymm9, %ymm10, %ymm10
vpsubd %ymm11, %ymm3, %ymm11
vpsubd %ymm6, %ymm8, %ymm6
vpsrld $1, %ymm11, %ymm11
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm11, %ymm11
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm11, %ymm6
vmovdqa 1696(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm8
vpunpckhwd const0(%rip), %ymm11, %ymm3
vpslld $1, %ymm8, %ymm8
vpslld $1, %ymm3, %ymm3
vpsubd %ymm8, %ymm7, %ymm7
vpsubd %ymm3, %ymm10, %ymm10
vpsrld $1, %ymm7, %ymm7
vpsrld $1, %ymm10, %ymm10
vpand mask32_to_16(%rip), %ymm7, %ymm7
vpand mask32_to_16(%rip), %ymm10, %ymm10
vpackusdw %ymm10, %ymm7, %ymm10
vmovdqa 928(%rsp), %ymm7
vpaddw 1184(%rsp), %ymm7, %ymm3
vpsubw 1184(%rsp), %ymm7, %ymm7
vpsrlw $2, %ymm7, %ymm7
vpsubw %ymm6, %ymm7, %ymm7
vpmullw %ymm14, %ymm7, %ymm7
vpsllw $1, %ymm5, %ymm8
vpsubw %ymm8, %ymm3, %ymm8
vpsllw $7, %ymm11, %ymm3
vpsubw %ymm3, %ymm8, %ymm3
vpsrlw $3, %ymm3, %ymm3
vpsubw %ymm10, %ymm3, %ymm3
vmovdqa 1440(%rsp), %ymm8
vpsubw %ymm5, %ymm8, %ymm8
vpmullw %ymm15, %ymm11, %ymm9
vpsubw %ymm9, %ymm8, %ymm9
vpmullw %ymm14, %ymm3, %ymm3
vpsubw %ymm3, %ymm10, %ymm10
vpmullw %ymm12, %ymm3, %ymm8
vpaddw %ymm8, %ymm10, %ymm8
vpmullw %ymm12, %ymm8, %ymm8
vpsubw %ymm8, %ymm9, %ymm8
vpmullw %ymm14, %ymm8, %ymm8
vpsubw %ymm6, %ymm8, %ymm8
vpsrlw $3, %ymm8, %ymm8
vpsubw %ymm7, %ymm8, %ymm8
vpsubw %ymm8, %ymm7, %ymm7
vpsubw %ymm7, %ymm6, %ymm6
vpmullw %ymm13, %ymm8, %ymm8
vpsubw %ymm8, %ymm6, %ymm6
vmovdqa 352(%rdi), %ymm9
vmovdqa 608(%rdi), %ymm2
vmovdqa 864(%rdi), %ymm4
vpaddw %ymm5, %ymm9, %ymm5
vpaddw %ymm6, %ymm2, %ymm6
vpaddw %ymm10, %ymm4, %ymm10
vpshufb shuf48_16(%rip), %ymm7, %ymm7
vpand mask3_5_3_5(%rip), %ymm7, %ymm4
vpand mask5_3_5_3(%rip), %ymm7, %ymm7
vpermq $206, %ymm4, %ymm4
vpand mask_keephigh(%rip), %ymm4, %ymm2
vpor %ymm2, %ymm7, %ymm7
vmovdqa 96(%rdi), %ymm2
vpaddw 1952(%rsp), %ymm2, %ymm2
vpaddw %ymm7, %ymm2, %ymm2
vmovdqa %ymm2, 96(%rdi)
vmovdqa %xmm4, 1952(%rsp)
vpshufb shuf48_16(%rip), %ymm3, %ymm3
vpand mask3_5_3_5(%rip), %ymm3, %ymm4
vpand mask5_3_5_3(%rip), %ymm3, %ymm3
vpermq $206, %ymm4, %ymm4
vpand mask_keephigh(%rip), %ymm4, %ymm2
vpor %ymm2, %ymm3, %ymm3
vpaddw 2208(%rsp), %ymm5, %ymm5
vpaddw %ymm3, %ymm5, %ymm5
vmovdqa %xmm4, 2208(%rsp)
vpshufb shuf48_16(%rip), %ymm8, %ymm8
vpand mask3_5_3_5(%rip), %ymm8, %ymm4
vpand mask5_3_5_3(%rip), %ymm8, %ymm8
vpermq $206, %ymm4, %ymm4
vpand mask_keephigh(%rip), %ymm4, %ymm2
vpor %ymm2, %ymm8, %ymm8
vpaddw 2464(%rsp), %ymm6, %ymm6
vpaddw %ymm8, %ymm6, %ymm6
vmovdqa %xmm4, 2464(%rsp)
vpshufb shuf48_16(%rip), %ymm11, %ymm11
vpand mask3_5_3_5(%rip), %ymm11, %ymm4
vpand mask5_3_5_3(%rip), %ymm11, %ymm11
vpermq $206, %ymm4, %ymm4
vpand mask_keephigh(%rip), %ymm4, %ymm2
vpor %ymm2, %ymm11, %ymm11
vpaddw 2720(%rsp), %ymm10, %ymm10
vpaddw %ymm11, %ymm10, %ymm10
vmovdqa %xmm4, 2720(%rsp)
vmovdqa %ymm5, 352(%rdi)
vmovdqa %ymm6, 608(%rdi)
vmovdqa %ymm10, 864(%rdi)
vmovdqa 192(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm8
vpunpckhwd const0(%rip), %ymm11, %ymm3
vpslld $1, %ymm8, %ymm8
vpslld $1, %ymm3, %ymm3
vmovdqa 448(%rsp), %ymm7
vpunpcklwd const0(%rip), %ymm7, %ymm10
vpunpckhwd const0(%rip), %ymm7, %ymm7
vmovdqa 704(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm5
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm5, %ymm10, %ymm4
vpaddd %ymm6, %ymm7, %ymm2
vpsubd %ymm8, %ymm4, %ymm4
vpsubd %ymm3, %ymm2, %ymm2
vpsubd %ymm5, %ymm10, %ymm5
vpsubd %ymm6, %ymm7, %ymm6
vpsrld $1, %ymm5, %ymm5
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm5, %ymm5
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm5, %ymm6
vmovdqa 1728(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm7
vpunpckhwd const0(%rip), %ymm5, %ymm10
vpslld $1, %ymm7, %ymm7
vpslld $1, %ymm10, %ymm10
vpsubd %ymm7, %ymm4, %ymm4
vpsubd %ymm10, %ymm2, %ymm2
vpsrld $1, %ymm4, %ymm4
vpsrld $1, %ymm2, %ymm2
vpand mask32_to_16(%rip), %ymm4, %ymm4
vpand mask32_to_16(%rip), %ymm2, %ymm2
vpackusdw %ymm2, %ymm4, %ymm2
vmovdqa 960(%rsp), %ymm4
vpaddw 1216(%rsp), %ymm4, %ymm10
vpsubw 1216(%rsp), %ymm4, %ymm4
vpsrlw $2, %ymm4, %ymm4
vpsubw %ymm6, %ymm4, %ymm4
vpmullw %ymm14, %ymm4, %ymm4
vpsllw $1, %ymm11, %ymm7
vpsubw %ymm7, %ymm10, %ymm7
vpsllw $7, %ymm5, %ymm10
vpsubw %ymm10, %ymm7, %ymm10
vpsrlw $3, %ymm10, %ymm10
vpsubw %ymm2, %ymm10, %ymm10
vmovdqa 1472(%rsp), %ymm7
vpsubw %ymm11, %ymm7, %ymm7
vpmullw %ymm15, %ymm5, %ymm3
vpsubw %ymm3, %ymm7, %ymm3
vpmullw %ymm14, %ymm10, %ymm10
vpsubw %ymm10, %ymm2, %ymm2
vpmullw %ymm12, %ymm10, %ymm7
vpaddw %ymm7, %ymm2, %ymm7
vpmullw %ymm12, %ymm7, %ymm7
vpsubw %ymm7, %ymm3, %ymm7
vpmullw %ymm14, %ymm7, %ymm7
vpsubw %ymm6, %ymm7, %ymm7
vpsrlw $3, %ymm7, %ymm7
vpsubw %ymm4, %ymm7, %ymm7
vpsubw %ymm7, %ymm4, %ymm4
vpsubw %ymm4, %ymm6, %ymm6
vpmullw %ymm13, %ymm7, %ymm7
vpsubw %ymm7, %ymm6, %ymm6
vmovdqa 416(%rdi), %ymm3
vmovdqa 672(%rdi), %ymm8
vmovdqa 928(%rdi), %ymm9
vpaddw %ymm11, %ymm3, %ymm11
vpaddw %ymm6, %ymm8, %ymm6
vpaddw %ymm2, %ymm9, %ymm2
vpshufb shuf48_16(%rip), %ymm4, %ymm4
vpand mask3_5_3_5(%rip), %ymm4, %ymm9
vpand mask5_3_5_3(%rip), %ymm4, %ymm4
vpermq $206, %ymm9, %ymm9
vpand mask_keephigh(%rip), %ymm9, %ymm8
vpor %ymm8, %ymm4, %ymm4
vmovdqa 160(%rdi), %ymm8
vpaddw 1984(%rsp), %ymm8, %ymm8
vpaddw %ymm4, %ymm8, %ymm8
vmovdqa %ymm8, 160(%rdi)
vmovdqa %xmm9, 1984(%rsp)
vpshufb shuf48_16(%rip), %ymm10, %ymm10
vpand mask3_5_3_5(%rip), %ymm10, %ymm9
vpand mask5_3_5_3(%rip), %ymm10, %ymm10
vpermq $206, %ymm9, %ymm9
vpand mask_keephigh(%rip), %ymm9, %ymm8
vpor %ymm8, %ymm10, %ymm10
vpaddw 2240(%rsp), %ymm11, %ymm11
vpaddw %ymm10, %ymm11, %ymm11
vmovdqa %xmm9, 2240(%rsp)
vpshufb shuf48_16(%rip), %ymm7, %ymm7
vpand mask3_5_3_5(%rip), %ymm7, %ymm9
vpand mask5_3_5_3(%rip), %ymm7, %ymm7
vpermq $206, %ymm9, %ymm9
vpand mask_keephigh(%rip), %ymm9, %ymm8
vpor %ymm8, %ymm7, %ymm7
vpaddw 2496(%rsp), %ymm6, %ymm6
vpaddw %ymm7, %ymm6, %ymm6
vmovdqa %xmm9, 2496(%rsp)
vpshufb shuf48_16(%rip), %ymm5, %ymm5
vpand mask3_5_3_5(%rip), %ymm5, %ymm9
vpand mask5_3_5_3(%rip), %ymm5, %ymm5
vpermq $206, %ymm9, %ymm9
vpand mask_keephigh(%rip), %ymm9, %ymm8
vpor %ymm8, %ymm5, %ymm5
vpaddw 2752(%rsp), %ymm2, %ymm2
vpaddw %ymm5, %ymm2, %ymm2
vmovdqa %xmm9, 2752(%rsp)
vmovdqa %ymm11, 416(%rdi)
vmovdqa %ymm6, 672(%rdi)
vmovdqa %ymm2, 928(%rdi)
vmovdqa 224(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm7
vpunpckhwd const0(%rip), %ymm5, %ymm10
vpslld $1, %ymm7, %ymm7
vpslld $1, %ymm10, %ymm10
vmovdqa 480(%rsp), %ymm4
vpunpcklwd const0(%rip), %ymm4, %ymm2
vpunpckhwd const0(%rip), %ymm4, %ymm4
vmovdqa 736(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm11
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm11, %ymm2, %ymm9
vpaddd %ymm6, %ymm4, %ymm8
vpsubd %ymm7, %ymm9, %ymm9
vpsubd %ymm10, %ymm8, %ymm8
vpsubd %ymm11, %ymm2, %ymm11
vpsubd %ymm6, %ymm4, %ymm6
vpsrld $1, %ymm11, %ymm11
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm11, %ymm11
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm11, %ymm6
vmovdqa 1760(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm4
vpunpckhwd const0(%rip), %ymm11, %ymm2
vpslld $1, %ymm4, %ymm4
vpslld $1, %ymm2, %ymm2
vpsubd %ymm4, %ymm9, %ymm9
vpsubd %ymm2, %ymm8, %ymm8
vpsrld $1, %ymm9, %ymm9
vpsrld $1, %ymm8, %ymm8
vpand mask32_to_16(%rip), %ymm9, %ymm9
vpand mask32_to_16(%rip), %ymm8, %ymm8
vpackusdw %ymm8, %ymm9, %ymm8
vmovdqa 992(%rsp), %ymm9
vpaddw 1248(%rsp), %ymm9, %ymm2
vpsubw 1248(%rsp), %ymm9, %ymm9
vpsrlw $2, %ymm9, %ymm9
vpsubw %ymm6, %ymm9, %ymm9
vpmullw %ymm14, %ymm9, %ymm9
vpsllw $1, %ymm5, %ymm4
vpsubw %ymm4, %ymm2, %ymm4
vpsllw $7, %ymm11, %ymm2
vpsubw %ymm2, %ymm4, %ymm2
vpsrlw $3, %ymm2, %ymm2
vpsubw %ymm8, %ymm2, %ymm2
vmovdqa 1504(%rsp), %ymm4
vpsubw %ymm5, %ymm4, %ymm4
vpmullw %ymm15, %ymm11, %ymm10
vpsubw %ymm10, %ymm4, %ymm10
vpmullw %ymm14, %ymm2, %ymm2
vpsubw %ymm2, %ymm8, %ymm8
vpmullw %ymm12, %ymm2, %ymm4
vpaddw %ymm4, %ymm8, %ymm4
vpmullw %ymm12, %ymm4, %ymm4
vpsubw %ymm4, %ymm10, %ymm4
vpmullw %ymm14, %ymm4, %ymm4
vpsubw %ymm6, %ymm4, %ymm4
vpsrlw $3, %ymm4, %ymm4
vpsubw %ymm9, %ymm4, %ymm4
vpsubw %ymm4, %ymm9, %ymm9
vpsubw %ymm9, %ymm6, %ymm6
vpmullw %ymm13, %ymm4, %ymm4
vpsubw %ymm4, %ymm6, %ymm6
vextracti128 $1, %ymm8, %xmm10
vpshufb shufmin5_mask3(%rip), %ymm10, %ymm10
vmovdqa %ymm10, 2816(%rsp)
vextracti128 $1, %ymm9, %xmm10
vpshufb shufmin5_mask3(%rip), %ymm10, %ymm10
vmovdqa %ymm10, 2848(%rsp)
vextracti128 $1, %ymm2, %xmm10
vpshufb shufmin5_mask3(%rip), %ymm10, %ymm10
vmovdqa %ymm10, 2880(%rsp)
vmovdqa 480(%rdi), %ymm10
vmovdqa 736(%rdi), %ymm7
vmovdqa 992(%rdi), %ymm3
vpaddw %ymm5, %ymm10, %ymm5
vpaddw %ymm6, %ymm7, %ymm6
vpaddw %ymm8, %ymm3, %ymm8
vpshufb shuf48_16(%rip), %ymm9, %ymm9
vpand mask3_5_3_5(%rip), %ymm9, %ymm3
vpand mask5_3_5_3(%rip), %ymm9, %ymm9
vpermq $206, %ymm3, %ymm3
vpand mask_keephigh(%rip), %ymm3, %ymm7
vpor %ymm7, %ymm9, %ymm9
vmovdqa 224(%rdi), %ymm7
vpaddw 2016(%rsp), %ymm7, %ymm7
vpaddw %ymm9, %ymm7, %ymm7
vmovdqa %ymm7, 224(%rdi)
vextracti128 $1, %ymm7, %xmm7
vpshufb shufmin5_mask3(%rip), %ymm7, %ymm7
vmovdqa %xmm7, 1792(%rsp)
vmovdqa %xmm3, 2016(%rsp)
vpshufb shuf48_16(%rip), %ymm2, %ymm2
vpand mask3_5_3_5(%rip), %ymm2, %ymm3
vpand mask5_3_5_3(%rip), %ymm2, %ymm2
vpermq $206, %ymm3, %ymm3
vpand mask_keephigh(%rip), %ymm3, %ymm7
vpor %ymm7, %ymm2, %ymm2
vpaddw 2272(%rsp), %ymm5, %ymm5
vpaddw %ymm2, %ymm5, %ymm5
vmovdqa %xmm3, 2272(%rsp)
vpshufb shuf48_16(%rip), %ymm4, %ymm4
vpand mask3_5_3_5(%rip), %ymm4, %ymm3
vpand mask5_3_5_3(%rip), %ymm4, %ymm4
vpermq $206, %ymm3, %ymm3
vpand mask_keephigh(%rip), %ymm3, %ymm7
vpor %ymm7, %ymm4, %ymm4
vpaddw 2528(%rsp), %ymm6, %ymm6
vpaddw %ymm4, %ymm6, %ymm6
vmovdqa %xmm3, 2528(%rsp)
vpshufb shuf48_16(%rip), %ymm11, %ymm11
vpand mask3_5_3_5(%rip), %ymm11, %ymm3
vpand mask5_3_5_3(%rip), %ymm11, %ymm11
vpermq $206, %ymm3, %ymm3
vpand mask_keephigh(%rip), %ymm3, %ymm7
vpor %ymm7, %ymm11, %ymm11
vpaddw 2784(%rsp), %ymm8, %ymm8
vpaddw %ymm11, %ymm8, %ymm8
vmovdqa %xmm3, 2784(%rsp)
vmovdqa %ymm5, 480(%rdi)
vmovdqa %ymm6, 736(%rdi)
vmovdqa %ymm8, 992(%rdi)
vmovdqa 0(%rdi), %ymm11
vpaddw 1888(%rsp), %ymm11, %ymm11
vpaddw 2816(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 0(%rdi)
vmovdqa 256(%rdi), %ymm11
vpaddw 2528(%rsp), %ymm11, %ymm11
vpaddw 2848(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 256(%rdi)
vmovdqa 512(%rdi), %ymm11
vpaddw 2784(%rsp), %ymm11, %ymm11
vpaddw 2880(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 512(%rdi)
vmovdqa 64(%rdi), %ymm11
vpaddw 2048(%rsp), %ymm11, %ymm11
vpaddw 1920(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 64(%rdi)
vmovdqa 320(%rdi), %ymm11
vpaddw 2304(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 320(%rdi)
vmovdqa 576(%rdi), %ymm11
vpaddw 2560(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 576(%rdi)
vmovdqa 128(%rdi), %ymm11
vpaddw 2080(%rsp), %ymm11, %ymm11
vpaddw 1952(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 128(%rdi)
vmovdqa 384(%rdi), %ymm11
vpaddw 2336(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 384(%rdi)
vmovdqa 640(%rdi), %ymm11
vpaddw 2592(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 640(%rdi)
vmovdqa 192(%rdi), %ymm11
vpaddw 2112(%rsp), %ymm11, %ymm11
vpaddw 1984(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 192(%rdi)
vmovdqa 448(%rdi), %ymm11
vpaddw 2368(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 448(%rdi)
vmovdqa 704(%rdi), %ymm11
vpaddw 2624(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 704(%rdi)
vmovdqa 256(%rdi), %ymm11
vpaddw 2144(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 256(%rdi)
vmovdqa 512(%rdi), %ymm11
vpaddw 2400(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 512(%rdi)
vmovdqa 768(%rdi), %ymm11
vpaddw 2656(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 768(%rdi)
vmovdqa 320(%rdi), %ymm11
vpaddw 2176(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 320(%rdi)
vmovdqa 576(%rdi), %ymm11
vpaddw 2432(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 576(%rdi)
vmovdqa 832(%rdi), %ymm11
vpaddw 2688(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 832(%rdi)
vmovdqa 384(%rdi), %ymm11
vpaddw 2208(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 384(%rdi)
vmovdqa 640(%rdi), %ymm11
vpaddw 2464(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 640(%rdi)
vmovdqa 896(%rdi), %ymm11
vpaddw 2720(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 896(%rdi)
vmovdqa 448(%rdi), %ymm11
vpaddw 2240(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 448(%rdi)
vmovdqa 704(%rdi), %ymm11
vpaddw 2496(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 704(%rdi)
vmovdqa 960(%rdi), %ymm11
vpaddw 2752(%rsp), %ymm11, %ymm11
vmovdqa %ymm11, 960(%rdi)
mov %r8, %rsp
pop %r12
ret
