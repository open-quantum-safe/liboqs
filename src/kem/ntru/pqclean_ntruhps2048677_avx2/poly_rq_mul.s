.data
.p2align 5
mask_low9words:
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
.word 0x0
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
rol_rol_16:
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
.byte 10
.byte 11
.byte 12
.byte 13
.byte 14
.byte 15
.byte 0
.byte 1
id_braid_16:
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
.byte 14
.byte 15
.byte 8
.byte 9
.byte 10
.byte 11
.byte 12
.byte 13
.byte 14
.byte 6
.byte 7
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
mask_1_15:
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
.word 0
.word 0
.word 0
mask_15_1:
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
.word 0
.text
.global PQCLEAN_NTRUHPS2048677_AVX2_poly_Rq_mul
.global _PQCLEAN_NTRUHPS2048677_AVX2_poly_Rq_mul
PQCLEAN_NTRUHPS2048677_AVX2_poly_Rq_mul:
_PQCLEAN_NTRUHPS2048677_AVX2_poly_Rq_mul:
push %r12
mov %rsp, %r8
andq $-32, %rsp
subq $6144, %rsp
mov %rsp, %rax
subq $6144, %rsp
mov %rsp, %r11
subq $12288, %rsp
mov %rsp, %r12
subq $512, %rsp
vpxor %ymm3, %ymm3, %ymm3
vmovdqa %ymm3, 0(%rdi)
vmovdqa %ymm3, 32(%rdi)
vmovdqa %ymm3, 64(%rdi)
vmovdqa %ymm3, 96(%rdi)
vmovdqa %ymm3, 128(%rdi)
vmovdqa %ymm3, 160(%rdi)
vmovdqa %ymm3, 192(%rdi)
vmovdqa %ymm3, 224(%rdi)
vmovdqa %ymm3, 256(%rdi)
vmovdqa %ymm3, 288(%rdi)
vmovdqa %ymm3, 320(%rdi)
vmovdqa %ymm3, 352(%rdi)
vmovdqa %ymm3, 384(%rdi)
vmovdqa %ymm3, 416(%rdi)
vmovdqa %ymm3, 448(%rdi)
vmovdqa %ymm3, 480(%rdi)
vmovdqa %ymm3, 512(%rdi)
vmovdqa %ymm3, 544(%rdi)
vmovdqa %ymm3, 576(%rdi)
vmovdqa %ymm3, 608(%rdi)
vmovdqa %ymm3, 640(%rdi)
vmovdqa %ymm3, 672(%rdi)
vmovdqa %ymm3, 704(%rdi)
vmovdqa %ymm3, 736(%rdi)
vmovdqa %ymm3, 768(%rdi)
vmovdqa %ymm3, 800(%rdi)
vmovdqa %ymm3, 832(%rdi)
vmovdqa %ymm3, 864(%rdi)
vmovdqa %ymm3, 896(%rdi)
vmovdqa %ymm3, 928(%rdi)
vmovdqa %ymm3, 960(%rdi)
vmovdqa %ymm3, 992(%rdi)
vmovdqa %ymm3, 1024(%rdi)
vmovdqa %ymm3, 1056(%rdi)
vmovdqa %ymm3, 1088(%rdi)
vmovdqa %ymm3, 1120(%rdi)
vmovdqa %ymm3, 1152(%rdi)
vmovdqa %ymm3, 1184(%rdi)
vmovdqa %ymm3, 1216(%rdi)
vmovdqa %ymm3, 1248(%rdi)
vmovdqa %ymm3, 1280(%rdi)
vmovdqa %ymm3, 1312(%rdi)
vmovdqa %ymm3, 1344(%rdi)
vmovdqa %ymm3, 1376(%rdi)
vmovdqa const3(%rip), %ymm3
vmovdqu 0(%rsi), %ymm0
vmovdqu 88(%rsi), %ymm1
vmovdqu 176(%rsi), %ymm2
vmovdqu 264(%rsi), %ymm12
vmovdqu 1056(%rsi), %ymm4
vmovdqu 1144(%rsi), %ymm5
vmovdqu 1232(%rsi), %ymm6
vmovdqu 1320(%rsi), %ymm7
vmovdqu 352(%rsi), %ymm8
vmovdqu 440(%rsi), %ymm9
vmovdqu 528(%rsi), %ymm10
vmovdqu 616(%rsi), %ymm11
vmovdqa %ymm0, 0(%rax)
vmovdqa %ymm1, 96(%rax)
vpaddw %ymm0, %ymm1, %ymm14
vmovdqa %ymm14, 192(%rax)
vmovdqa %ymm2, 288(%rax)
vmovdqa %ymm12, 384(%rax)
vpaddw %ymm2, %ymm12, %ymm14
vmovdqa %ymm14, 480(%rax)
vpaddw %ymm0, %ymm2, %ymm14
vmovdqa %ymm14, 576(%rax)
vpaddw %ymm1, %ymm12, %ymm15
vmovdqa %ymm15, 672(%rax)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 768(%rax)
vmovdqa %ymm4, 5184(%rax)
vmovdqa %ymm5, 5280(%rax)
vpaddw %ymm4, %ymm5, %ymm14
vmovdqa %ymm14, 5376(%rax)
vmovdqa %ymm6, 5472(%rax)
vmovdqa %ymm7, 5568(%rax)
vpaddw %ymm6, %ymm7, %ymm14
vmovdqa %ymm14, 5664(%rax)
vpaddw %ymm4, %ymm6, %ymm14
vmovdqa %ymm14, 5760(%rax)
vpaddw %ymm5, %ymm7, %ymm15
vmovdqa %ymm15, 5856(%rax)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 5952(%rax)
vmovdqa %ymm0, 0(%rsp)
vmovdqa %ymm1, 32(%rsp)
vmovdqa %ymm2, 64(%rsp)
vmovdqa %ymm12, 96(%rsp)
vmovdqa %ymm8, 128(%rsp)
vmovdqa %ymm9, 160(%rsp)
vmovdqa %ymm10, 192(%rsp)
vmovdqa %ymm11, 224(%rsp)
vmovdqu 704(%rsi), %ymm0
vpaddw 0(%rsp), %ymm0, %ymm1
vpaddw 128(%rsp), %ymm4, %ymm2
vpaddw %ymm2, %ymm1, %ymm8
vpsubw %ymm2, %ymm1, %ymm12
vmovdqa %ymm0, 256(%rsp)
vmovdqu 792(%rsi), %ymm0
vpaddw 32(%rsp), %ymm0, %ymm1
vpaddw 160(%rsp), %ymm5, %ymm2
vpaddw %ymm2, %ymm1, %ymm9
vpsubw %ymm2, %ymm1, %ymm13
vmovdqa %ymm0, 288(%rsp)
vmovdqu 880(%rsi), %ymm0
vpaddw 64(%rsp), %ymm0, %ymm1
vpaddw 192(%rsp), %ymm6, %ymm2
vpaddw %ymm2, %ymm1, %ymm10
vpsubw %ymm2, %ymm1, %ymm14
vmovdqa %ymm0, 320(%rsp)
vmovdqu 968(%rsi), %ymm0
vpaddw 96(%rsp), %ymm0, %ymm1
vpaddw 224(%rsp), %ymm7, %ymm2
vpaddw %ymm2, %ymm1, %ymm11
vpsubw %ymm2, %ymm1, %ymm15
vmovdqa %ymm0, 352(%rsp)
vmovdqa %ymm8, 864(%rax)
vmovdqa %ymm9, 960(%rax)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 1056(%rax)
vmovdqa %ymm10, 1152(%rax)
vmovdqa %ymm11, 1248(%rax)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 1344(%rax)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 1440(%rax)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 1536(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 1632(%rax)
vmovdqa %ymm12, 1728(%rax)
vmovdqa %ymm13, 1824(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 1920(%rax)
vmovdqa %ymm14, 2016(%rax)
vmovdqa %ymm15, 2112(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 2208(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 2304(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 2400(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2496(%rax)
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
vmovdqa %ymm8, 2592(%rax)
vmovdqa %ymm9, 2688(%rax)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 2784(%rax)
vmovdqa %ymm10, 2880(%rax)
vmovdqa %ymm11, 2976(%rax)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 3072(%rax)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 3168(%rax)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 3264(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 3360(%rax)
vmovdqa %ymm12, 3456(%rax)
vmovdqa %ymm13, 3552(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 3648(%rax)
vmovdqa %ymm14, 3744(%rax)
vmovdqa %ymm15, 3840(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 3936(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 4032(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 4128(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 4224(%rax)
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
vmovdqa %ymm12, 4320(%rax)
vmovdqa %ymm13, 4416(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 4512(%rax)
vmovdqa %ymm14, 4608(%rax)
vmovdqa %ymm15, 4704(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 4800(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 4896(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 4992(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 5088(%rax)
vmovdqu 32(%rsi), %ymm0
vmovdqu 120(%rsi), %ymm1
vmovdqu 208(%rsi), %ymm2
vmovdqu 296(%rsi), %ymm12
vmovdqu 1088(%rsi), %ymm4
vmovdqu 1176(%rsi), %ymm5
vmovdqu 1264(%rsi), %ymm6
vmovdqu 1352(%rsi), %ymm7
vpand mask_1_15(%rip), %ymm7, %ymm7
vmovdqu 384(%rsi), %ymm8
vmovdqu 472(%rsi), %ymm9
vmovdqu 560(%rsi), %ymm10
vmovdqu 648(%rsi), %ymm11
vmovdqa %ymm0, 32(%rax)
vmovdqa %ymm1, 128(%rax)
vpaddw %ymm0, %ymm1, %ymm14
vmovdqa %ymm14, 224(%rax)
vmovdqa %ymm2, 320(%rax)
vmovdqa %ymm12, 416(%rax)
vpaddw %ymm2, %ymm12, %ymm14
vmovdqa %ymm14, 512(%rax)
vpaddw %ymm0, %ymm2, %ymm14
vmovdqa %ymm14, 608(%rax)
vpaddw %ymm1, %ymm12, %ymm15
vmovdqa %ymm15, 704(%rax)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 800(%rax)
vmovdqa %ymm4, 5216(%rax)
vmovdqa %ymm5, 5312(%rax)
vpaddw %ymm4, %ymm5, %ymm14
vmovdqa %ymm14, 5408(%rax)
vmovdqa %ymm6, 5504(%rax)
vmovdqa %ymm7, 5600(%rax)
vpaddw %ymm6, %ymm7, %ymm14
vmovdqa %ymm14, 5696(%rax)
vpaddw %ymm4, %ymm6, %ymm14
vmovdqa %ymm14, 5792(%rax)
vpaddw %ymm5, %ymm7, %ymm15
vmovdqa %ymm15, 5888(%rax)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 5984(%rax)
vmovdqa %ymm0, 0(%rsp)
vmovdqa %ymm1, 32(%rsp)
vmovdqa %ymm2, 64(%rsp)
vmovdqa %ymm12, 96(%rsp)
vmovdqa %ymm8, 128(%rsp)
vmovdqa %ymm9, 160(%rsp)
vmovdqa %ymm10, 192(%rsp)
vmovdqa %ymm11, 224(%rsp)
vmovdqu 736(%rsi), %ymm0
vpaddw 0(%rsp), %ymm0, %ymm1
vpaddw 128(%rsp), %ymm4, %ymm2
vpaddw %ymm2, %ymm1, %ymm8
vpsubw %ymm2, %ymm1, %ymm12
vmovdqa %ymm0, 256(%rsp)
vmovdqu 824(%rsi), %ymm0
vpaddw 32(%rsp), %ymm0, %ymm1
vpaddw 160(%rsp), %ymm5, %ymm2
vpaddw %ymm2, %ymm1, %ymm9
vpsubw %ymm2, %ymm1, %ymm13
vmovdqa %ymm0, 288(%rsp)
vmovdqu 912(%rsi), %ymm0
vpaddw 64(%rsp), %ymm0, %ymm1
vpaddw 192(%rsp), %ymm6, %ymm2
vpaddw %ymm2, %ymm1, %ymm10
vpsubw %ymm2, %ymm1, %ymm14
vmovdqa %ymm0, 320(%rsp)
vmovdqu 1000(%rsi), %ymm0
vpaddw 96(%rsp), %ymm0, %ymm1
vpaddw 224(%rsp), %ymm7, %ymm2
vpaddw %ymm2, %ymm1, %ymm11
vpsubw %ymm2, %ymm1, %ymm15
vmovdqa %ymm0, 352(%rsp)
vmovdqa %ymm8, 896(%rax)
vmovdqa %ymm9, 992(%rax)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 1088(%rax)
vmovdqa %ymm10, 1184(%rax)
vmovdqa %ymm11, 1280(%rax)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 1376(%rax)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 1472(%rax)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 1568(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 1664(%rax)
vmovdqa %ymm12, 1760(%rax)
vmovdqa %ymm13, 1856(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 1952(%rax)
vmovdqa %ymm14, 2048(%rax)
vmovdqa %ymm15, 2144(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 2240(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 2336(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 2432(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2528(%rax)
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
vmovdqa %ymm8, 2624(%rax)
vmovdqa %ymm9, 2720(%rax)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 2816(%rax)
vmovdqa %ymm10, 2912(%rax)
vmovdqa %ymm11, 3008(%rax)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 3104(%rax)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 3200(%rax)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 3296(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 3392(%rax)
vmovdqa %ymm12, 3488(%rax)
vmovdqa %ymm13, 3584(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 3680(%rax)
vmovdqa %ymm14, 3776(%rax)
vmovdqa %ymm15, 3872(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 3968(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 4064(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 4160(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 4256(%rax)
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
vmovdqa %ymm12, 4352(%rax)
vmovdqa %ymm13, 4448(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 4544(%rax)
vmovdqa %ymm14, 4640(%rax)
vmovdqa %ymm15, 4736(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 4832(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 4928(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 5024(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 5120(%rax)
vmovdqu 64(%rsi), %ymm0
vmovdqu 152(%rsi), %ymm1
vmovdqu 240(%rsi), %ymm2
vmovdqu 328(%rsi), %ymm12
vmovdqu 1120(%rsi), %ymm4
vmovdqu 1208(%rsi), %ymm5
vmovdqu 1296(%rsi), %ymm6
vmovdqu 1384(%rsi), %ymm7
vpxor %ymm7, %ymm7, %ymm7
vmovdqu 416(%rsi), %ymm8
vmovdqu 504(%rsi), %ymm9
vmovdqu 592(%rsi), %ymm10
vmovdqu 680(%rsi), %ymm11
vmovdqa %ymm0, 64(%rax)
vmovdqa %ymm1, 160(%rax)
vpaddw %ymm0, %ymm1, %ymm14
vmovdqa %ymm14, 256(%rax)
vmovdqa %ymm2, 352(%rax)
vmovdqa %ymm12, 448(%rax)
vpaddw %ymm2, %ymm12, %ymm14
vmovdqa %ymm14, 544(%rax)
vpaddw %ymm0, %ymm2, %ymm14
vmovdqa %ymm14, 640(%rax)
vpaddw %ymm1, %ymm12, %ymm15
vmovdqa %ymm15, 736(%rax)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 832(%rax)
vmovdqa %ymm4, 5248(%rax)
vmovdqa %ymm5, 5344(%rax)
vpaddw %ymm4, %ymm5, %ymm14
vmovdqa %ymm14, 5440(%rax)
vmovdqa %ymm6, 5536(%rax)
vmovdqa %ymm7, 5632(%rax)
vpaddw %ymm6, %ymm7, %ymm14
vmovdqa %ymm14, 5728(%rax)
vpaddw %ymm4, %ymm6, %ymm14
vmovdqa %ymm14, 5824(%rax)
vpaddw %ymm5, %ymm7, %ymm15
vmovdqa %ymm15, 5920(%rax)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 6016(%rax)
vmovdqa %ymm0, 0(%rsp)
vmovdqa %ymm1, 32(%rsp)
vmovdqa %ymm2, 64(%rsp)
vmovdqa %ymm12, 96(%rsp)
vmovdqa %ymm8, 128(%rsp)
vmovdqa %ymm9, 160(%rsp)
vmovdqa %ymm10, 192(%rsp)
vmovdqa %ymm11, 224(%rsp)
vmovdqu 768(%rsi), %ymm0
vpaddw 0(%rsp), %ymm0, %ymm1
vpaddw 128(%rsp), %ymm4, %ymm2
vpaddw %ymm2, %ymm1, %ymm8
vpsubw %ymm2, %ymm1, %ymm12
vmovdqa %ymm0, 256(%rsp)
vmovdqu 856(%rsi), %ymm0
vpaddw 32(%rsp), %ymm0, %ymm1
vpaddw 160(%rsp), %ymm5, %ymm2
vpaddw %ymm2, %ymm1, %ymm9
vpsubw %ymm2, %ymm1, %ymm13
vmovdqa %ymm0, 288(%rsp)
vmovdqu 944(%rsi), %ymm0
vpaddw 64(%rsp), %ymm0, %ymm1
vpaddw 192(%rsp), %ymm6, %ymm2
vpaddw %ymm2, %ymm1, %ymm10
vpsubw %ymm2, %ymm1, %ymm14
vmovdqa %ymm0, 320(%rsp)
vmovdqu 1032(%rsi), %ymm0
vpaddw 96(%rsp), %ymm0, %ymm1
vpaddw 224(%rsp), %ymm7, %ymm2
vpaddw %ymm2, %ymm1, %ymm11
vpsubw %ymm2, %ymm1, %ymm15
vmovdqa %ymm0, 352(%rsp)
vmovdqa %ymm8, 928(%rax)
vmovdqa %ymm9, 1024(%rax)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 1120(%rax)
vmovdqa %ymm10, 1216(%rax)
vmovdqa %ymm11, 1312(%rax)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 1408(%rax)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 1504(%rax)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 1600(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 1696(%rax)
vmovdqa %ymm12, 1792(%rax)
vmovdqa %ymm13, 1888(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 1984(%rax)
vmovdqa %ymm14, 2080(%rax)
vmovdqa %ymm15, 2176(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 2272(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 2368(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 2464(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2560(%rax)
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
vmovdqa %ymm8, 2656(%rax)
vmovdqa %ymm9, 2752(%rax)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 2848(%rax)
vmovdqa %ymm10, 2944(%rax)
vmovdqa %ymm11, 3040(%rax)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 3136(%rax)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 3232(%rax)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 3328(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 3424(%rax)
vmovdqa %ymm12, 3520(%rax)
vmovdqa %ymm13, 3616(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 3712(%rax)
vmovdqa %ymm14, 3808(%rax)
vmovdqa %ymm15, 3904(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 4000(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 4096(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 4192(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 4288(%rax)
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
vmovdqa %ymm12, 4384(%rax)
vmovdqa %ymm13, 4480(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 4576(%rax)
vmovdqa %ymm14, 4672(%rax)
vmovdqa %ymm15, 4768(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 4864(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 4960(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 5056(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 5152(%rax)
vmovdqu 0(%rdx), %ymm0
vmovdqu 88(%rdx), %ymm1
vmovdqu 176(%rdx), %ymm2
vmovdqu 264(%rdx), %ymm12
vmovdqu 1056(%rdx), %ymm4
vmovdqu 1144(%rdx), %ymm5
vmovdqu 1232(%rdx), %ymm6
vmovdqu 1320(%rdx), %ymm7
vmovdqu 352(%rdx), %ymm8
vmovdqu 440(%rdx), %ymm9
vmovdqu 528(%rdx), %ymm10
vmovdqu 616(%rdx), %ymm11
vmovdqa %ymm0, 0(%r11)
vmovdqa %ymm1, 96(%r11)
vpaddw %ymm0, %ymm1, %ymm14
vmovdqa %ymm14, 192(%r11)
vmovdqa %ymm2, 288(%r11)
vmovdqa %ymm12, 384(%r11)
vpaddw %ymm2, %ymm12, %ymm14
vmovdqa %ymm14, 480(%r11)
vpaddw %ymm0, %ymm2, %ymm14
vmovdqa %ymm14, 576(%r11)
vpaddw %ymm1, %ymm12, %ymm15
vmovdqa %ymm15, 672(%r11)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 768(%r11)
vmovdqa %ymm4, 5184(%r11)
vmovdqa %ymm5, 5280(%r11)
vpaddw %ymm4, %ymm5, %ymm14
vmovdqa %ymm14, 5376(%r11)
vmovdqa %ymm6, 5472(%r11)
vmovdqa %ymm7, 5568(%r11)
vpaddw %ymm6, %ymm7, %ymm14
vmovdqa %ymm14, 5664(%r11)
vpaddw %ymm4, %ymm6, %ymm14
vmovdqa %ymm14, 5760(%r11)
vpaddw %ymm5, %ymm7, %ymm15
vmovdqa %ymm15, 5856(%r11)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 5952(%r11)
vmovdqa %ymm0, 0(%rsp)
vmovdqa %ymm1, 32(%rsp)
vmovdqa %ymm2, 64(%rsp)
vmovdqa %ymm12, 96(%rsp)
vmovdqa %ymm8, 128(%rsp)
vmovdqa %ymm9, 160(%rsp)
vmovdqa %ymm10, 192(%rsp)
vmovdqa %ymm11, 224(%rsp)
vmovdqu 704(%rdx), %ymm0
vpaddw 0(%rsp), %ymm0, %ymm1
vpaddw 128(%rsp), %ymm4, %ymm2
vpaddw %ymm2, %ymm1, %ymm8
vpsubw %ymm2, %ymm1, %ymm12
vmovdqa %ymm0, 256(%rsp)
vmovdqu 792(%rdx), %ymm0
vpaddw 32(%rsp), %ymm0, %ymm1
vpaddw 160(%rsp), %ymm5, %ymm2
vpaddw %ymm2, %ymm1, %ymm9
vpsubw %ymm2, %ymm1, %ymm13
vmovdqa %ymm0, 288(%rsp)
vmovdqu 880(%rdx), %ymm0
vpaddw 64(%rsp), %ymm0, %ymm1
vpaddw 192(%rsp), %ymm6, %ymm2
vpaddw %ymm2, %ymm1, %ymm10
vpsubw %ymm2, %ymm1, %ymm14
vmovdqa %ymm0, 320(%rsp)
vmovdqu 968(%rdx), %ymm0
vpaddw 96(%rsp), %ymm0, %ymm1
vpaddw 224(%rsp), %ymm7, %ymm2
vpaddw %ymm2, %ymm1, %ymm11
vpsubw %ymm2, %ymm1, %ymm15
vmovdqa %ymm0, 352(%rsp)
vmovdqa %ymm8, 864(%r11)
vmovdqa %ymm9, 960(%r11)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 1056(%r11)
vmovdqa %ymm10, 1152(%r11)
vmovdqa %ymm11, 1248(%r11)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 1344(%r11)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 1440(%r11)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 1536(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 1632(%r11)
vmovdqa %ymm12, 1728(%r11)
vmovdqa %ymm13, 1824(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 1920(%r11)
vmovdqa %ymm14, 2016(%r11)
vmovdqa %ymm15, 2112(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 2208(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 2304(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 2400(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2496(%r11)
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
vmovdqa %ymm8, 2592(%r11)
vmovdqa %ymm9, 2688(%r11)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 2784(%r11)
vmovdqa %ymm10, 2880(%r11)
vmovdqa %ymm11, 2976(%r11)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 3072(%r11)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 3168(%r11)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 3264(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 3360(%r11)
vmovdqa %ymm12, 3456(%r11)
vmovdqa %ymm13, 3552(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 3648(%r11)
vmovdqa %ymm14, 3744(%r11)
vmovdqa %ymm15, 3840(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 3936(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 4032(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 4128(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 4224(%r11)
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
vmovdqa %ymm12, 4320(%r11)
vmovdqa %ymm13, 4416(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 4512(%r11)
vmovdqa %ymm14, 4608(%r11)
vmovdqa %ymm15, 4704(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 4800(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 4896(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 4992(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 5088(%r11)
vmovdqu 32(%rdx), %ymm0
vmovdqu 120(%rdx), %ymm1
vmovdqu 208(%rdx), %ymm2
vmovdqu 296(%rdx), %ymm12
vmovdqu 1088(%rdx), %ymm4
vmovdqu 1176(%rdx), %ymm5
vmovdqu 1264(%rdx), %ymm6
vmovdqu 1352(%rdx), %ymm7
vpand mask_1_15(%rip), %ymm7, %ymm7
vmovdqu 384(%rdx), %ymm8
vmovdqu 472(%rdx), %ymm9
vmovdqu 560(%rdx), %ymm10
vmovdqu 648(%rdx), %ymm11
vmovdqa %ymm0, 32(%r11)
vmovdqa %ymm1, 128(%r11)
vpaddw %ymm0, %ymm1, %ymm14
vmovdqa %ymm14, 224(%r11)
vmovdqa %ymm2, 320(%r11)
vmovdqa %ymm12, 416(%r11)
vpaddw %ymm2, %ymm12, %ymm14
vmovdqa %ymm14, 512(%r11)
vpaddw %ymm0, %ymm2, %ymm14
vmovdqa %ymm14, 608(%r11)
vpaddw %ymm1, %ymm12, %ymm15
vmovdqa %ymm15, 704(%r11)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 800(%r11)
vmovdqa %ymm4, 5216(%r11)
vmovdqa %ymm5, 5312(%r11)
vpaddw %ymm4, %ymm5, %ymm14
vmovdqa %ymm14, 5408(%r11)
vmovdqa %ymm6, 5504(%r11)
vmovdqa %ymm7, 5600(%r11)
vpaddw %ymm6, %ymm7, %ymm14
vmovdqa %ymm14, 5696(%r11)
vpaddw %ymm4, %ymm6, %ymm14
vmovdqa %ymm14, 5792(%r11)
vpaddw %ymm5, %ymm7, %ymm15
vmovdqa %ymm15, 5888(%r11)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 5984(%r11)
vmovdqa %ymm0, 0(%rsp)
vmovdqa %ymm1, 32(%rsp)
vmovdqa %ymm2, 64(%rsp)
vmovdqa %ymm12, 96(%rsp)
vmovdqa %ymm8, 128(%rsp)
vmovdqa %ymm9, 160(%rsp)
vmovdqa %ymm10, 192(%rsp)
vmovdqa %ymm11, 224(%rsp)
vmovdqu 736(%rdx), %ymm0
vpaddw 0(%rsp), %ymm0, %ymm1
vpaddw 128(%rsp), %ymm4, %ymm2
vpaddw %ymm2, %ymm1, %ymm8
vpsubw %ymm2, %ymm1, %ymm12
vmovdqa %ymm0, 256(%rsp)
vmovdqu 824(%rdx), %ymm0
vpaddw 32(%rsp), %ymm0, %ymm1
vpaddw 160(%rsp), %ymm5, %ymm2
vpaddw %ymm2, %ymm1, %ymm9
vpsubw %ymm2, %ymm1, %ymm13
vmovdqa %ymm0, 288(%rsp)
vmovdqu 912(%rdx), %ymm0
vpaddw 64(%rsp), %ymm0, %ymm1
vpaddw 192(%rsp), %ymm6, %ymm2
vpaddw %ymm2, %ymm1, %ymm10
vpsubw %ymm2, %ymm1, %ymm14
vmovdqa %ymm0, 320(%rsp)
vmovdqu 1000(%rdx), %ymm0
vpaddw 96(%rsp), %ymm0, %ymm1
vpaddw 224(%rsp), %ymm7, %ymm2
vpaddw %ymm2, %ymm1, %ymm11
vpsubw %ymm2, %ymm1, %ymm15
vmovdqa %ymm0, 352(%rsp)
vmovdqa %ymm8, 896(%r11)
vmovdqa %ymm9, 992(%r11)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 1088(%r11)
vmovdqa %ymm10, 1184(%r11)
vmovdqa %ymm11, 1280(%r11)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 1376(%r11)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 1472(%r11)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 1568(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 1664(%r11)
vmovdqa %ymm12, 1760(%r11)
vmovdqa %ymm13, 1856(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 1952(%r11)
vmovdqa %ymm14, 2048(%r11)
vmovdqa %ymm15, 2144(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 2240(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 2336(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 2432(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2528(%r11)
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
vmovdqa %ymm8, 2624(%r11)
vmovdqa %ymm9, 2720(%r11)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 2816(%r11)
vmovdqa %ymm10, 2912(%r11)
vmovdqa %ymm11, 3008(%r11)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 3104(%r11)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 3200(%r11)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 3296(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 3392(%r11)
vmovdqa %ymm12, 3488(%r11)
vmovdqa %ymm13, 3584(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 3680(%r11)
vmovdqa %ymm14, 3776(%r11)
vmovdqa %ymm15, 3872(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 3968(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 4064(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 4160(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 4256(%r11)
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
vmovdqa %ymm12, 4352(%r11)
vmovdqa %ymm13, 4448(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 4544(%r11)
vmovdqa %ymm14, 4640(%r11)
vmovdqa %ymm15, 4736(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 4832(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 4928(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 5024(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 5120(%r11)
vmovdqu 64(%rdx), %ymm0
vmovdqu 152(%rdx), %ymm1
vmovdqu 240(%rdx), %ymm2
vmovdqu 328(%rdx), %ymm12
vmovdqu 1120(%rdx), %ymm4
vmovdqu 1208(%rdx), %ymm5
vmovdqu 1296(%rdx), %ymm6
vmovdqu 1384(%rdx), %ymm7
vpxor %ymm7, %ymm7, %ymm7
vmovdqu 416(%rdx), %ymm8
vmovdqu 504(%rdx), %ymm9
vmovdqu 592(%rdx), %ymm10
vmovdqu 680(%rdx), %ymm11
vmovdqa %ymm0, 64(%r11)
vmovdqa %ymm1, 160(%r11)
vpaddw %ymm0, %ymm1, %ymm14
vmovdqa %ymm14, 256(%r11)
vmovdqa %ymm2, 352(%r11)
vmovdqa %ymm12, 448(%r11)
vpaddw %ymm2, %ymm12, %ymm14
vmovdqa %ymm14, 544(%r11)
vpaddw %ymm0, %ymm2, %ymm14
vmovdqa %ymm14, 640(%r11)
vpaddw %ymm1, %ymm12, %ymm15
vmovdqa %ymm15, 736(%r11)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 832(%r11)
vmovdqa %ymm4, 5248(%r11)
vmovdqa %ymm5, 5344(%r11)
vpaddw %ymm4, %ymm5, %ymm14
vmovdqa %ymm14, 5440(%r11)
vmovdqa %ymm6, 5536(%r11)
vmovdqa %ymm7, 5632(%r11)
vpaddw %ymm6, %ymm7, %ymm14
vmovdqa %ymm14, 5728(%r11)
vpaddw %ymm4, %ymm6, %ymm14
vmovdqa %ymm14, 5824(%r11)
vpaddw %ymm5, %ymm7, %ymm15
vmovdqa %ymm15, 5920(%r11)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 6016(%r11)
vmovdqa %ymm0, 0(%rsp)
vmovdqa %ymm1, 32(%rsp)
vmovdqa %ymm2, 64(%rsp)
vmovdqa %ymm12, 96(%rsp)
vmovdqa %ymm8, 128(%rsp)
vmovdqa %ymm9, 160(%rsp)
vmovdqa %ymm10, 192(%rsp)
vmovdqa %ymm11, 224(%rsp)
vmovdqu 768(%rdx), %ymm0
vpaddw 0(%rsp), %ymm0, %ymm1
vpaddw 128(%rsp), %ymm4, %ymm2
vpaddw %ymm2, %ymm1, %ymm8
vpsubw %ymm2, %ymm1, %ymm12
vmovdqa %ymm0, 256(%rsp)
vmovdqu 856(%rdx), %ymm0
vpaddw 32(%rsp), %ymm0, %ymm1
vpaddw 160(%rsp), %ymm5, %ymm2
vpaddw %ymm2, %ymm1, %ymm9
vpsubw %ymm2, %ymm1, %ymm13
vmovdqa %ymm0, 288(%rsp)
vmovdqu 944(%rdx), %ymm0
vpaddw 64(%rsp), %ymm0, %ymm1
vpaddw 192(%rsp), %ymm6, %ymm2
vpaddw %ymm2, %ymm1, %ymm10
vpsubw %ymm2, %ymm1, %ymm14
vmovdqa %ymm0, 320(%rsp)
vmovdqu 1032(%rdx), %ymm0
vpaddw 96(%rsp), %ymm0, %ymm1
vpaddw 224(%rsp), %ymm7, %ymm2
vpaddw %ymm2, %ymm1, %ymm11
vpsubw %ymm2, %ymm1, %ymm15
vmovdqa %ymm0, 352(%rsp)
vmovdqa %ymm8, 928(%r11)
vmovdqa %ymm9, 1024(%r11)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 1120(%r11)
vmovdqa %ymm10, 1216(%r11)
vmovdqa %ymm11, 1312(%r11)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 1408(%r11)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 1504(%r11)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 1600(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 1696(%r11)
vmovdqa %ymm12, 1792(%r11)
vmovdqa %ymm13, 1888(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 1984(%r11)
vmovdqa %ymm14, 2080(%r11)
vmovdqa %ymm15, 2176(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 2272(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 2368(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 2464(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2560(%r11)
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
vmovdqa %ymm8, 2656(%r11)
vmovdqa %ymm9, 2752(%r11)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 2848(%r11)
vmovdqa %ymm10, 2944(%r11)
vmovdqa %ymm11, 3040(%r11)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 3136(%r11)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 3232(%r11)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 3328(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 3424(%r11)
vmovdqa %ymm12, 3520(%r11)
vmovdqa %ymm13, 3616(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 3712(%r11)
vmovdqa %ymm14, 3808(%r11)
vmovdqa %ymm15, 3904(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 4000(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 4096(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 4192(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 4288(%r11)
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
vmovdqa %ymm12, 4384(%r11)
vmovdqa %ymm13, 4480(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 4576(%r11)
vmovdqa %ymm14, 4672(%r11)
vmovdqa %ymm15, 4768(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 4864(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 4960(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 5056(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 5152(%r11)
subq $9408, %rsp
mov $4, %ecx
karatsuba_loop_1:
mov %rsp, %r9
mov %rsp, %r10
subq $32, %rsp
vmovdqa 0(%rax), %ymm0
vmovdqa 192(%rax), %ymm1
vmovdqa 384(%rax), %ymm2
vmovdqa 576(%rax), %ymm3
vpunpcklwd 96(%rax), %ymm0, %ymm4
vpunpckhwd 96(%rax), %ymm0, %ymm5
vpunpcklwd 288(%rax), %ymm1, %ymm6
vpunpckhwd 288(%rax), %ymm1, %ymm7
vpunpcklwd 480(%rax), %ymm2, %ymm8
vpunpckhwd 480(%rax), %ymm2, %ymm9
vpunpcklwd 672(%rax), %ymm3, %ymm10
vpunpckhwd 672(%rax), %ymm3, %ymm11
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
vmovdqa 768(%rax), %ymm0
vmovdqa 960(%rax), %ymm1
vmovdqa 1152(%rax), %ymm2
vmovdqa 1344(%rax), %ymm3
vpunpcklwd 864(%rax), %ymm0, %ymm12
vpunpckhwd 864(%rax), %ymm0, %ymm13
vpunpcklwd 1056(%rax), %ymm1, %ymm14
vpunpckhwd 1056(%rax), %ymm1, %ymm15
vpunpcklwd 1248(%rax), %ymm2, %ymm0
vpunpckhwd 1248(%rax), %ymm2, %ymm1
vpunpcklwd 1440(%rax), %ymm3, %ymm2
vpunpckhwd 1440(%rax), %ymm3, %ymm3
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
vmovdqa 224(%rax), %ymm1
vmovdqa 416(%rax), %ymm2
vmovdqa 608(%rax), %ymm3
vpunpcklwd 128(%rax), %ymm0, %ymm4
vpunpckhwd 128(%rax), %ymm0, %ymm5
vpunpcklwd 320(%rax), %ymm1, %ymm6
vpunpckhwd 320(%rax), %ymm1, %ymm7
vpunpcklwd 512(%rax), %ymm2, %ymm8
vpunpckhwd 512(%rax), %ymm2, %ymm9
vpunpcklwd 704(%rax), %ymm3, %ymm10
vpunpckhwd 704(%rax), %ymm3, %ymm11
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
vmovdqa 800(%rax), %ymm0
vmovdqa 992(%rax), %ymm1
vmovdqa 1184(%rax), %ymm2
vmovdqa 1376(%rax), %ymm3
vpunpcklwd 896(%rax), %ymm0, %ymm12
vpunpckhwd 896(%rax), %ymm0, %ymm13
vpunpcklwd 1088(%rax), %ymm1, %ymm14
vpunpckhwd 1088(%rax), %ymm1, %ymm15
vpunpcklwd 1280(%rax), %ymm2, %ymm0
vpunpckhwd 1280(%rax), %ymm2, %ymm1
vpunpcklwd 1472(%rax), %ymm3, %ymm2
vpunpckhwd 1472(%rax), %ymm3, %ymm3
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
vmovdqa 64(%rax), %ymm0
vmovdqa 256(%rax), %ymm1
vmovdqa 448(%rax), %ymm2
vmovdqa 640(%rax), %ymm3
vpunpcklwd 160(%rax), %ymm0, %ymm4
vpunpckhwd 160(%rax), %ymm0, %ymm5
vpunpcklwd 352(%rax), %ymm1, %ymm6
vpunpckhwd 352(%rax), %ymm1, %ymm7
vpunpcklwd 544(%rax), %ymm2, %ymm8
vpunpckhwd 544(%rax), %ymm2, %ymm9
vpunpcklwd 736(%rax), %ymm3, %ymm10
vpunpckhwd 736(%rax), %ymm3, %ymm11
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
vmovdqa 832(%rax), %ymm0
vmovdqa 1024(%rax), %ymm1
vmovdqa 1216(%rax), %ymm2
vmovdqa 1408(%rax), %ymm3
vpunpcklwd 928(%rax), %ymm0, %ymm12
vpunpckhwd 928(%rax), %ymm0, %ymm13
vpunpcklwd 1120(%rax), %ymm1, %ymm14
vpunpckhwd 1120(%rax), %ymm1, %ymm15
vpunpcklwd 1312(%rax), %ymm2, %ymm0
vpunpckhwd 1312(%rax), %ymm2, %ymm1
vpunpcklwd 1504(%rax), %ymm3, %ymm2
vpunpckhwd 1504(%rax), %ymm3, %ymm3
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
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1248(%r9)
addq $32, %rsp
subq $32, %rsp
vmovdqa 0(%r11), %ymm0
vmovdqa 192(%r11), %ymm1
vmovdqa 384(%r11), %ymm2
vmovdqa 576(%r11), %ymm3
vpunpcklwd 96(%r11), %ymm0, %ymm4
vpunpckhwd 96(%r11), %ymm0, %ymm5
vpunpcklwd 288(%r11), %ymm1, %ymm6
vpunpckhwd 288(%r11), %ymm1, %ymm7
vpunpcklwd 480(%r11), %ymm2, %ymm8
vpunpckhwd 480(%r11), %ymm2, %ymm9
vpunpcklwd 672(%r11), %ymm3, %ymm10
vpunpckhwd 672(%r11), %ymm3, %ymm11
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
vmovdqa 768(%r11), %ymm0
vmovdqa 960(%r11), %ymm1
vmovdqa 1152(%r11), %ymm2
vmovdqa 1344(%r11), %ymm3
vpunpcklwd 864(%r11), %ymm0, %ymm12
vpunpckhwd 864(%r11), %ymm0, %ymm13
vpunpcklwd 1056(%r11), %ymm1, %ymm14
vpunpckhwd 1056(%r11), %ymm1, %ymm15
vpunpcklwd 1248(%r11), %ymm2, %ymm0
vpunpckhwd 1248(%r11), %ymm2, %ymm1
vpunpcklwd 1440(%r11), %ymm3, %ymm2
vpunpckhwd 1440(%r11), %ymm3, %ymm3
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
vmovdqa %ymm15, 1408(%r9)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 1440(%r9)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 1472(%r9)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 1504(%r9)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 1536(%r9)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 1568(%r9)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 1600(%r9)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 1664(%r9)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 1696(%r9)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 1728(%r9)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 1760(%r9)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 1792(%r9)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 1824(%r9)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 1856(%r9)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1632(%r9)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 1888(%r9)
vmovdqa 32(%r11), %ymm0
vmovdqa 224(%r11), %ymm1
vmovdqa 416(%r11), %ymm2
vmovdqa 608(%r11), %ymm3
vpunpcklwd 128(%r11), %ymm0, %ymm4
vpunpckhwd 128(%r11), %ymm0, %ymm5
vpunpcklwd 320(%r11), %ymm1, %ymm6
vpunpckhwd 320(%r11), %ymm1, %ymm7
vpunpcklwd 512(%r11), %ymm2, %ymm8
vpunpckhwd 512(%r11), %ymm2, %ymm9
vpunpcklwd 704(%r11), %ymm3, %ymm10
vpunpckhwd 704(%r11), %ymm3, %ymm11
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
vmovdqa 800(%r11), %ymm0
vmovdqa 992(%r11), %ymm1
vmovdqa 1184(%r11), %ymm2
vmovdqa 1376(%r11), %ymm3
vpunpcklwd 896(%r11), %ymm0, %ymm12
vpunpckhwd 896(%r11), %ymm0, %ymm13
vpunpcklwd 1088(%r11), %ymm1, %ymm14
vpunpckhwd 1088(%r11), %ymm1, %ymm15
vpunpcklwd 1280(%r11), %ymm2, %ymm0
vpunpckhwd 1280(%r11), %ymm2, %ymm1
vpunpcklwd 1472(%r11), %ymm3, %ymm2
vpunpckhwd 1472(%r11), %ymm3, %ymm3
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
vmovdqa %ymm15, 1920(%r9)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 1952(%r9)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 1984(%r9)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 2016(%r9)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 2048(%r9)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 2080(%r9)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 2112(%r9)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 2176(%r9)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 2208(%r9)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 2240(%r9)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 2272(%r9)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 2304(%r9)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 2336(%r9)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 2368(%r9)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 2144(%r9)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 2400(%r9)
vmovdqa 64(%r11), %ymm0
vmovdqa 256(%r11), %ymm1
vmovdqa 448(%r11), %ymm2
vmovdqa 640(%r11), %ymm3
vpunpcklwd 160(%r11), %ymm0, %ymm4
vpunpckhwd 160(%r11), %ymm0, %ymm5
vpunpcklwd 352(%r11), %ymm1, %ymm6
vpunpckhwd 352(%r11), %ymm1, %ymm7
vpunpcklwd 544(%r11), %ymm2, %ymm8
vpunpckhwd 544(%r11), %ymm2, %ymm9
vpunpcklwd 736(%r11), %ymm3, %ymm10
vpunpckhwd 736(%r11), %ymm3, %ymm11
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
vmovdqa 832(%r11), %ymm0
vmovdqa 1024(%r11), %ymm1
vmovdqa 1216(%r11), %ymm2
vmovdqa 1408(%r11), %ymm3
vpunpcklwd 928(%r11), %ymm0, %ymm12
vpunpckhwd 928(%r11), %ymm0, %ymm13
vpunpcklwd 1120(%r11), %ymm1, %ymm14
vpunpckhwd 1120(%r11), %ymm1, %ymm15
vpunpcklwd 1312(%r11), %ymm2, %ymm0
vpunpckhwd 1312(%r11), %ymm2, %ymm1
vpunpcklwd 1504(%r11), %ymm3, %ymm2
vpunpckhwd 1504(%r11), %ymm3, %ymm3
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
vmovdqa %ymm15, 2432(%r9)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 2464(%r9)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 2496(%r9)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 2528(%r9)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 2560(%r9)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 2592(%r9)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 2624(%r9)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 2688(%r9)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 2720(%r9)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 2752(%r9)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 2784(%r9)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 2656(%r9)
addq $32, %rsp
innerloop_1:
vmovdqa 0(%r9), %ymm0
vmovdqa 1408(%r9), %ymm6
vmovdqa 32(%r9), %ymm1
vmovdqa 1440(%r9), %ymm7
vmovdqa 64(%r9), %ymm2
vmovdqa 1472(%r9), %ymm8
vmovdqa 96(%r9), %ymm3
vmovdqa 1504(%r9), %ymm9
vmovdqa 128(%r9), %ymm4
vmovdqa 1536(%r9), %ymm10
vmovdqa 160(%r9), %ymm5
vmovdqa 1568(%r9), %ymm11
vpmullw %ymm0, %ymm6, %ymm12
vmovdqa %ymm12, 2816(%r10)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 2848(%r10)
vpmullw %ymm0, %ymm8, %ymm12
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 2880(%r10)
vpmullw %ymm0, %ymm9, %ymm13
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 2912(%r10)
vpmullw %ymm0, %ymm10, %ymm12
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 2944(%r10)
vpmullw %ymm0, %ymm11, %ymm13
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm5, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 2976(%r10)
vpmullw %ymm1, %ymm11, %ymm12
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 3008(%r10)
vpmullw %ymm2, %ymm11, %ymm13
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 3040(%r10)
vpmullw %ymm3, %ymm11, %ymm12
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 3072(%r10)
vpmullw %ymm4, %ymm11, %ymm13
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 3104(%r10)
vpmullw %ymm5, %ymm11, %ymm12
vmovdqa %ymm12, 3136(%r10)
vmovdqa 192(%r9), %ymm0
vmovdqa 1600(%r9), %ymm6
vmovdqa 224(%r9), %ymm1
vmovdqa 1632(%r9), %ymm7
vmovdqa 256(%r9), %ymm2
vmovdqa 1664(%r9), %ymm8
vmovdqa 288(%r9), %ymm3
vmovdqa 1696(%r9), %ymm9
vmovdqa 320(%r9), %ymm4
vmovdqa 1728(%r9), %ymm10
vpmullw %ymm0, %ymm6, %ymm12
vmovdqa %ymm12, 3200(%r10)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 3232(%r10)
vpmullw %ymm0, %ymm8, %ymm12
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 3264(%r10)
vpmullw %ymm0, %ymm9, %ymm13
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 3296(%r10)
vpmullw %ymm0, %ymm10, %ymm12
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 3328(%r10)
vpmullw %ymm1, %ymm10, %ymm13
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 3360(%r10)
vpmullw %ymm2, %ymm10, %ymm12
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 3392(%r10)
vpmullw %ymm3, %ymm10, %ymm13
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 3424(%r10)
vpmullw %ymm4, %ymm10, %ymm12
vmovdqa %ymm12, 3456(%r10)
vpaddw 0(%r9), %ymm0, %ymm0
vpaddw 1408(%r9), %ymm6, %ymm6
vpaddw 32(%r9), %ymm1, %ymm1
vpaddw 1440(%r9), %ymm7, %ymm7
vpaddw 64(%r9), %ymm2, %ymm2
vpaddw 1472(%r9), %ymm8, %ymm8
vpaddw 96(%r9), %ymm3, %ymm3
vpaddw 1504(%r9), %ymm9, %ymm9
vpaddw 128(%r9), %ymm4, %ymm4
vpaddw 1536(%r9), %ymm10, %ymm10
vpmullw %ymm0, %ymm11, %ymm12
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm5, %ymm6, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpsubw 2976(%r10), %ymm12, %ymm12
vpsubw 3360(%r10), %ymm12, %ymm12
vmovdqa %ymm12, 3168(%r10)
vpmullw %ymm5, %ymm7, %ymm12
vpmullw %ymm5, %ymm8, %ymm13
vpmullw %ymm5, %ymm9, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpmullw %ymm1, %ymm11, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm2, %ymm10, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm3, %ymm9, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm4, %ymm8, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm2, %ymm11, %ymm5
vpaddw %ymm5, %ymm13, %ymm13
vpmullw %ymm3, %ymm10, %ymm5
vpaddw %ymm5, %ymm13, %ymm13
vpmullw %ymm4, %ymm9, %ymm5
vpaddw %ymm5, %ymm13, %ymm13
vpmullw %ymm3, %ymm11, %ymm5
vpaddw %ymm5, %ymm14, %ymm14
vpmullw %ymm4, %ymm10, %ymm5
vpaddw %ymm5, %ymm14, %ymm14
vpmullw %ymm4, %ymm11, %ymm5
vpaddw %ymm5, %ymm15, %ymm15
vpmullw %ymm0, %ymm10, %ymm11
vpmullw %ymm1, %ymm9, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm2, %ymm8, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm3, %ymm7, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm4, %ymm6, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm0, %ymm9, %ymm10
vpmullw %ymm1, %ymm8, %ymm5
vpaddw %ymm5, %ymm10, %ymm10
vpmullw %ymm2, %ymm7, %ymm5
vpaddw %ymm5, %ymm10, %ymm10
vpmullw %ymm3, %ymm6, %ymm5
vpaddw %ymm5, %ymm10, %ymm10
vpmullw %ymm0, %ymm8, %ymm9
vpmullw %ymm1, %ymm7, %ymm5
vpaddw %ymm5, %ymm9, %ymm9
vpmullw %ymm2, %ymm6, %ymm5
vpaddw %ymm5, %ymm9, %ymm9
vpmullw %ymm0, %ymm7, %ymm8
vpmullw %ymm1, %ymm6, %ymm5
vpaddw %ymm5, %ymm8, %ymm8
vpmullw %ymm0, %ymm6, %ymm7
vmovdqa 3008(%r10), %ymm0
vpsubw 3200(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm12, %ymm6
vpsubw 3392(%r10), %ymm6, %ymm6
vmovdqa %ymm6, 3200(%r10)
vpaddw %ymm7, %ymm0, %ymm0
vpsubw 2816(%r10), %ymm0, %ymm0
vmovdqa %ymm0, 3008(%r10)
vmovdqa 3040(%r10), %ymm1
vpsubw 3232(%r10), %ymm1, %ymm1
vpsubw %ymm1, %ymm13, %ymm7
vpsubw 3424(%r10), %ymm7, %ymm7
vmovdqa %ymm7, 3232(%r10)
vpaddw %ymm8, %ymm1, %ymm1
vpsubw 2848(%r10), %ymm1, %ymm1
vmovdqa %ymm1, 3040(%r10)
vmovdqa 3072(%r10), %ymm2
vpsubw 3264(%r10), %ymm2, %ymm2
vpsubw %ymm2, %ymm14, %ymm8
vpsubw 3456(%r10), %ymm8, %ymm8
vmovdqa %ymm8, 3264(%r10)
vpaddw %ymm9, %ymm2, %ymm2
vpsubw 2880(%r10), %ymm2, %ymm2
vmovdqa %ymm2, 3072(%r10)
vmovdqa 3104(%r10), %ymm3
vpsubw 3296(%r10), %ymm3, %ymm3
vpsubw %ymm3, %ymm15, %ymm9
vmovdqa %ymm9, 3296(%r10)
vpaddw %ymm10, %ymm3, %ymm3
vpsubw 2912(%r10), %ymm3, %ymm3
vmovdqa %ymm3, 3104(%r10)
vmovdqa 3136(%r10), %ymm4
vpsubw 3328(%r10), %ymm4, %ymm4
vpaddw %ymm11, %ymm4, %ymm4
vpsubw 2944(%r10), %ymm4, %ymm4
vmovdqa %ymm4, 3136(%r10)
vmovdqa 352(%r9), %ymm0
vmovdqa 1760(%r9), %ymm6
vmovdqa 384(%r9), %ymm1
vmovdqa 1792(%r9), %ymm7
vmovdqa 416(%r9), %ymm2
vmovdqa 1824(%r9), %ymm8
vmovdqa 448(%r9), %ymm3
vmovdqa 1856(%r9), %ymm9
vmovdqa 480(%r9), %ymm4
vmovdqa 1888(%r9), %ymm10
vmovdqa 512(%r9), %ymm5
vmovdqa 1920(%r9), %ymm11
vpmullw %ymm0, %ymm6, %ymm12
vmovdqa %ymm12, 3520(%r10)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 3552(%r10)
vpmullw %ymm0, %ymm8, %ymm12
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 3584(%r10)
vpmullw %ymm0, %ymm9, %ymm13
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 3616(%r10)
vpmullw %ymm0, %ymm10, %ymm12
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 3648(%r10)
vpmullw %ymm0, %ymm11, %ymm13
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm5, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 3680(%r10)
vpmullw %ymm1, %ymm11, %ymm12
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 3712(%r10)
vpmullw %ymm2, %ymm11, %ymm13
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 3744(%r10)
vpmullw %ymm3, %ymm11, %ymm12
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 3776(%r10)
vpmullw %ymm4, %ymm11, %ymm13
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 3808(%r10)
vpmullw %ymm5, %ymm11, %ymm12
vmovdqa %ymm12, 3840(%r10)
vmovdqa 544(%r9), %ymm0
vmovdqa 1952(%r9), %ymm6
vmovdqa 576(%r9), %ymm1
vmovdqa 1984(%r9), %ymm7
vmovdqa 608(%r9), %ymm2
vmovdqa 2016(%r9), %ymm8
vmovdqa 640(%r9), %ymm3
vmovdqa 2048(%r9), %ymm9
vmovdqa 672(%r9), %ymm4
vmovdqa 2080(%r9), %ymm10
vpmullw %ymm0, %ymm6, %ymm12
vmovdqa %ymm12, 3904(%r10)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 3936(%r10)
vpmullw %ymm0, %ymm8, %ymm12
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 3968(%r10)
vpmullw %ymm0, %ymm9, %ymm13
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 4000(%r10)
vpmullw %ymm0, %ymm10, %ymm12
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 4032(%r10)
vpmullw %ymm1, %ymm10, %ymm13
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 4064(%r10)
vpmullw %ymm2, %ymm10, %ymm12
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 4096(%r10)
vpmullw %ymm3, %ymm10, %ymm13
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 4128(%r10)
vpmullw %ymm4, %ymm10, %ymm12
vmovdqa %ymm12, 4160(%r10)
vpaddw 352(%r9), %ymm0, %ymm0
vpaddw 1760(%r9), %ymm6, %ymm6
vpaddw 384(%r9), %ymm1, %ymm1
vpaddw 1792(%r9), %ymm7, %ymm7
vpaddw 416(%r9), %ymm2, %ymm2
vpaddw 1824(%r9), %ymm8, %ymm8
vpaddw 448(%r9), %ymm3, %ymm3
vpaddw 1856(%r9), %ymm9, %ymm9
vpaddw 480(%r9), %ymm4, %ymm4
vpaddw 1888(%r9), %ymm10, %ymm10
vpmullw %ymm0, %ymm11, %ymm12
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm5, %ymm6, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpsubw 3680(%r10), %ymm12, %ymm12
vpsubw 4064(%r10), %ymm12, %ymm12
vmovdqa %ymm12, 3872(%r10)
vpmullw %ymm5, %ymm7, %ymm12
vpmullw %ymm5, %ymm8, %ymm13
vpmullw %ymm5, %ymm9, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpmullw %ymm1, %ymm11, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm2, %ymm10, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm3, %ymm9, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm4, %ymm8, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm2, %ymm11, %ymm5
vpaddw %ymm5, %ymm13, %ymm13
vpmullw %ymm3, %ymm10, %ymm5
vpaddw %ymm5, %ymm13, %ymm13
vpmullw %ymm4, %ymm9, %ymm5
vpaddw %ymm5, %ymm13, %ymm13
vpmullw %ymm3, %ymm11, %ymm5
vpaddw %ymm5, %ymm14, %ymm14
vpmullw %ymm4, %ymm10, %ymm5
vpaddw %ymm5, %ymm14, %ymm14
vpmullw %ymm4, %ymm11, %ymm5
vpaddw %ymm5, %ymm15, %ymm15
vpmullw %ymm0, %ymm10, %ymm11
vpmullw %ymm1, %ymm9, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm2, %ymm8, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm3, %ymm7, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm4, %ymm6, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm0, %ymm9, %ymm10
vpmullw %ymm1, %ymm8, %ymm5
vpaddw %ymm5, %ymm10, %ymm10
vpmullw %ymm2, %ymm7, %ymm5
vpaddw %ymm5, %ymm10, %ymm10
vpmullw %ymm3, %ymm6, %ymm5
vpaddw %ymm5, %ymm10, %ymm10
vpmullw %ymm0, %ymm8, %ymm9
vpmullw %ymm1, %ymm7, %ymm5
vpaddw %ymm5, %ymm9, %ymm9
vpmullw %ymm2, %ymm6, %ymm5
vpaddw %ymm5, %ymm9, %ymm9
vpmullw %ymm0, %ymm7, %ymm8
vpmullw %ymm1, %ymm6, %ymm5
vpaddw %ymm5, %ymm8, %ymm8
vpmullw %ymm0, %ymm6, %ymm7
vmovdqa 3712(%r10), %ymm0
vpsubw 3904(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm12, %ymm6
vpsubw 4096(%r10), %ymm6, %ymm6
vmovdqa %ymm6, 3904(%r10)
vpaddw %ymm7, %ymm0, %ymm0
vpsubw 3520(%r10), %ymm0, %ymm0
vmovdqa %ymm0, 3712(%r10)
vmovdqa 3744(%r10), %ymm1
vpsubw 3936(%r10), %ymm1, %ymm1
vpsubw %ymm1, %ymm13, %ymm7
vpsubw 4128(%r10), %ymm7, %ymm7
vmovdqa %ymm7, 3936(%r10)
vpaddw %ymm8, %ymm1, %ymm1
vpsubw 3552(%r10), %ymm1, %ymm1
vmovdqa %ymm1, 3744(%r10)
vmovdqa 3776(%r10), %ymm2
vpsubw 3968(%r10), %ymm2, %ymm2
vpsubw %ymm2, %ymm14, %ymm8
vpsubw 4160(%r10), %ymm8, %ymm8
vmovdqa %ymm8, 3968(%r10)
vpaddw %ymm9, %ymm2, %ymm2
vpsubw 3584(%r10), %ymm2, %ymm2
vmovdqa %ymm2, 3776(%r10)
vmovdqa 3808(%r10), %ymm3
vpsubw 4000(%r10), %ymm3, %ymm3
vpsubw %ymm3, %ymm15, %ymm9
vmovdqa %ymm9, 4000(%r10)
vpaddw %ymm10, %ymm3, %ymm3
vpsubw 3616(%r10), %ymm3, %ymm3
vmovdqa %ymm3, 3808(%r10)
vmovdqa 3840(%r10), %ymm4
vpsubw 4032(%r10), %ymm4, %ymm4
vpaddw %ymm11, %ymm4, %ymm4
vpsubw 3648(%r10), %ymm4, %ymm4
vmovdqa %ymm4, 3840(%r10)
vmovdqa 0(%r9), %ymm0
vmovdqa 1408(%r9), %ymm6
vpaddw 352(%r9), %ymm0, %ymm0
vpaddw 1760(%r9), %ymm6, %ymm6
vmovdqa 32(%r9), %ymm1
vmovdqa 1440(%r9), %ymm7
vpaddw 384(%r9), %ymm1, %ymm1
vpaddw 1792(%r9), %ymm7, %ymm7
vmovdqa 64(%r9), %ymm2
vmovdqa 1472(%r9), %ymm8
vpaddw 416(%r9), %ymm2, %ymm2
vpaddw 1824(%r9), %ymm8, %ymm8
vmovdqa 96(%r9), %ymm3
vmovdqa 1504(%r9), %ymm9
vpaddw 448(%r9), %ymm3, %ymm3
vpaddw 1856(%r9), %ymm9, %ymm9
vmovdqa 128(%r9), %ymm4
vmovdqa 1536(%r9), %ymm10
vpaddw 480(%r9), %ymm4, %ymm4
vpaddw 1888(%r9), %ymm10, %ymm10
vmovdqa 160(%r9), %ymm5
vmovdqa 1568(%r9), %ymm11
vpaddw 512(%r9), %ymm5, %ymm5
vpaddw 1920(%r9), %ymm11, %ymm11
vpmullw %ymm0, %ymm6, %ymm12
vmovdqa %ymm12, 5888(%rsp)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 5920(%rsp)
vpmullw %ymm0, %ymm8, %ymm12
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 5952(%rsp)
vpmullw %ymm0, %ymm9, %ymm13
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 5984(%rsp)
vpmullw %ymm0, %ymm10, %ymm12
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 6016(%rsp)
vpmullw %ymm0, %ymm11, %ymm13
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm5, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6048(%rsp)
vpmullw %ymm1, %ymm11, %ymm12
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 6080(%rsp)
vpmullw %ymm2, %ymm11, %ymm13
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6112(%rsp)
vpmullw %ymm3, %ymm11, %ymm12
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 6144(%rsp)
vpmullw %ymm4, %ymm11, %ymm13
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6176(%rsp)
vpmullw %ymm5, %ymm11, %ymm12
vmovdqa %ymm12, 6208(%rsp)
vmovdqa 192(%r9), %ymm0
vmovdqa 1600(%r9), %ymm6
vpaddw 544(%r9), %ymm0, %ymm0
vpaddw 1952(%r9), %ymm6, %ymm6
vmovdqa 224(%r9), %ymm1
vmovdqa 1632(%r9), %ymm7
vpaddw 576(%r9), %ymm1, %ymm1
vpaddw 1984(%r9), %ymm7, %ymm7
vmovdqa 256(%r9), %ymm2
vmovdqa 1664(%r9), %ymm8
vpaddw 608(%r9), %ymm2, %ymm2
vpaddw 2016(%r9), %ymm8, %ymm8
vmovdqa 288(%r9), %ymm3
vmovdqa 1696(%r9), %ymm9
vpaddw 640(%r9), %ymm3, %ymm3
vpaddw 2048(%r9), %ymm9, %ymm9
vmovdqa 320(%r9), %ymm4
vmovdqa 1728(%r9), %ymm10
vpaddw 672(%r9), %ymm4, %ymm4
vpaddw 2080(%r9), %ymm10, %ymm10
vpmullw %ymm0, %ymm6, %ymm12
vmovdqa %ymm12, 6272(%rsp)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6304(%rsp)
vpmullw %ymm0, %ymm8, %ymm12
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 6336(%rsp)
vpmullw %ymm0, %ymm9, %ymm13
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6368(%rsp)
vpmullw %ymm0, %ymm10, %ymm12
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 6400(%rsp)
vpmullw %ymm1, %ymm10, %ymm13
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6432(%rsp)
vpmullw %ymm2, %ymm10, %ymm12
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 6464(%rsp)
vpmullw %ymm3, %ymm10, %ymm13
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6496(%rsp)
vpmullw %ymm4, %ymm10, %ymm12
vmovdqa %ymm12, 6528(%rsp)
vpaddw 0(%r9), %ymm0, %ymm0
vpaddw 1408(%r9), %ymm6, %ymm6
vpaddw 352(%r9), %ymm0, %ymm0
vpaddw 1760(%r9), %ymm6, %ymm6
vpaddw 32(%r9), %ymm1, %ymm1
vpaddw 1440(%r9), %ymm7, %ymm7
vpaddw 384(%r9), %ymm1, %ymm1
vpaddw 1792(%r9), %ymm7, %ymm7
vpaddw 64(%r9), %ymm2, %ymm2
vpaddw 1472(%r9), %ymm8, %ymm8
vpaddw 416(%r9), %ymm2, %ymm2
vpaddw 1824(%r9), %ymm8, %ymm8
vpaddw 96(%r9), %ymm3, %ymm3
vpaddw 1504(%r9), %ymm9, %ymm9
vpaddw 448(%r9), %ymm3, %ymm3
vpaddw 1856(%r9), %ymm9, %ymm9
vpaddw 128(%r9), %ymm4, %ymm4
vpaddw 1536(%r9), %ymm10, %ymm10
vpaddw 480(%r9), %ymm4, %ymm4
vpaddw 1888(%r9), %ymm10, %ymm10
vpmullw %ymm0, %ymm11, %ymm12
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm5, %ymm6, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpsubw 6048(%rsp), %ymm12, %ymm12
vpsubw 6432(%rsp), %ymm12, %ymm12
vmovdqa %ymm12, 6240(%rsp)
vpmullw %ymm5, %ymm7, %ymm12
vpmullw %ymm5, %ymm8, %ymm13
vpmullw %ymm5, %ymm9, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpmullw %ymm1, %ymm11, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm2, %ymm10, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm3, %ymm9, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm4, %ymm8, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm2, %ymm11, %ymm5
vpaddw %ymm5, %ymm13, %ymm13
vpmullw %ymm3, %ymm10, %ymm5
vpaddw %ymm5, %ymm13, %ymm13
vpmullw %ymm4, %ymm9, %ymm5
vpaddw %ymm5, %ymm13, %ymm13
vpmullw %ymm3, %ymm11, %ymm5
vpaddw %ymm5, %ymm14, %ymm14
vpmullw %ymm4, %ymm10, %ymm5
vpaddw %ymm5, %ymm14, %ymm14
vpmullw %ymm4, %ymm11, %ymm5
vpaddw %ymm5, %ymm15, %ymm15
vpmullw %ymm0, %ymm10, %ymm11
vpmullw %ymm1, %ymm9, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm2, %ymm8, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm3, %ymm7, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm4, %ymm6, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm0, %ymm9, %ymm10
vpmullw %ymm1, %ymm8, %ymm5
vpaddw %ymm5, %ymm10, %ymm10
vpmullw %ymm2, %ymm7, %ymm5
vpaddw %ymm5, %ymm10, %ymm10
vpmullw %ymm3, %ymm6, %ymm5
vpaddw %ymm5, %ymm10, %ymm10
vpmullw %ymm0, %ymm8, %ymm9
vpmullw %ymm1, %ymm7, %ymm5
vpaddw %ymm5, %ymm9, %ymm9
vpmullw %ymm2, %ymm6, %ymm5
vpaddw %ymm5, %ymm9, %ymm9
vpmullw %ymm0, %ymm7, %ymm8
vpmullw %ymm1, %ymm6, %ymm5
vpaddw %ymm5, %ymm8, %ymm8
vpmullw %ymm0, %ymm6, %ymm7
vmovdqa 6080(%rsp), %ymm0
vpsubw 6272(%rsp), %ymm0, %ymm0
vpsubw %ymm0, %ymm12, %ymm6
vpsubw 6464(%rsp), %ymm6, %ymm6
vmovdqa %ymm6, 6272(%rsp)
vpaddw %ymm7, %ymm0, %ymm0
vpsubw 5888(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 6080(%rsp)
vmovdqa 6112(%rsp), %ymm1
vpsubw 6304(%rsp), %ymm1, %ymm1
vpsubw %ymm1, %ymm13, %ymm7
vpsubw 6496(%rsp), %ymm7, %ymm7
vmovdqa %ymm7, 6304(%rsp)
vpaddw %ymm8, %ymm1, %ymm1
vpsubw 5920(%rsp), %ymm1, %ymm1
vmovdqa %ymm1, 6112(%rsp)
vmovdqa 6144(%rsp), %ymm2
vpsubw 6336(%rsp), %ymm2, %ymm2
vpsubw %ymm2, %ymm14, %ymm8
vpsubw 6528(%rsp), %ymm8, %ymm8
vmovdqa %ymm8, 6336(%rsp)
vpaddw %ymm9, %ymm2, %ymm2
vpsubw 5952(%rsp), %ymm2, %ymm2
vmovdqa %ymm2, 6144(%rsp)
vmovdqa 6176(%rsp), %ymm3
vpsubw 6368(%rsp), %ymm3, %ymm3
vpsubw %ymm3, %ymm15, %ymm9
vmovdqa %ymm9, 6368(%rsp)
vpaddw %ymm10, %ymm3, %ymm3
vpsubw 5984(%rsp), %ymm3, %ymm3
vmovdqa %ymm3, 6176(%rsp)
vmovdqa 6208(%rsp), %ymm4
vpsubw 6400(%rsp), %ymm4, %ymm4
vpaddw %ymm11, %ymm4, %ymm4
vpsubw 6016(%rsp), %ymm4, %ymm4
vmovdqa %ymm4, 6208(%rsp)
vmovdqa 6208(%rsp), %ymm0
vpsubw 3136(%r10), %ymm0, %ymm0
vpsubw 3840(%r10), %ymm0, %ymm0
vmovdqa %ymm0, 3488(%r10)
vmovdqa 3168(%r10), %ymm0
vpsubw 3520(%r10), %ymm0, %ymm0
vmovdqa 6240(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 3872(%r10), %ymm1, %ymm1
vpsubw 2816(%r10), %ymm0, %ymm0
vpaddw 5888(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3168(%r10)
vmovdqa %ymm1, 3520(%r10)
vmovdqa 3200(%r10), %ymm0
vpsubw 3552(%r10), %ymm0, %ymm0
vmovdqa 6272(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 3904(%r10), %ymm1, %ymm1
vpsubw 2848(%r10), %ymm0, %ymm0
vpaddw 5920(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3200(%r10)
vmovdqa %ymm1, 3552(%r10)
vmovdqa 3232(%r10), %ymm0
vpsubw 3584(%r10), %ymm0, %ymm0
vmovdqa 6304(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 3936(%r10), %ymm1, %ymm1
vpsubw 2880(%r10), %ymm0, %ymm0
vpaddw 5952(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3232(%r10)
vmovdqa %ymm1, 3584(%r10)
vmovdqa 3264(%r10), %ymm0
vpsubw 3616(%r10), %ymm0, %ymm0
vmovdqa 6336(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 3968(%r10), %ymm1, %ymm1
vpsubw 2912(%r10), %ymm0, %ymm0
vpaddw 5984(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3264(%r10)
vmovdqa %ymm1, 3616(%r10)
vmovdqa 3296(%r10), %ymm0
vpsubw 3648(%r10), %ymm0, %ymm0
vmovdqa 6368(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4000(%r10), %ymm1, %ymm1
vpsubw 2944(%r10), %ymm0, %ymm0
vpaddw 6016(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3296(%r10)
vmovdqa %ymm1, 3648(%r10)
vmovdqa 3328(%r10), %ymm0
vpsubw 3680(%r10), %ymm0, %ymm0
vmovdqa 6400(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4032(%r10), %ymm1, %ymm1
vpsubw 2976(%r10), %ymm0, %ymm0
vpaddw 6048(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3328(%r10)
vmovdqa %ymm1, 3680(%r10)
vmovdqa 3360(%r10), %ymm0
vpsubw 3712(%r10), %ymm0, %ymm0
vmovdqa 6432(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4064(%r10), %ymm1, %ymm1
vpsubw 3008(%r10), %ymm0, %ymm0
vpaddw 6080(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3360(%r10)
vmovdqa %ymm1, 3712(%r10)
vmovdqa 3392(%r10), %ymm0
vpsubw 3744(%r10), %ymm0, %ymm0
vmovdqa 6464(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4096(%r10), %ymm1, %ymm1
vpsubw 3040(%r10), %ymm0, %ymm0
vpaddw 6112(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3392(%r10)
vmovdqa %ymm1, 3744(%r10)
vmovdqa 3424(%r10), %ymm0
vpsubw 3776(%r10), %ymm0, %ymm0
vmovdqa 6496(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4128(%r10), %ymm1, %ymm1
vpsubw 3072(%r10), %ymm0, %ymm0
vpaddw 6144(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3424(%r10)
vmovdqa %ymm1, 3776(%r10)
vmovdqa 3456(%r10), %ymm0
vpsubw 3808(%r10), %ymm0, %ymm0
vmovdqa 6528(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4160(%r10), %ymm1, %ymm1
vpsubw 3104(%r10), %ymm0, %ymm0
vpaddw 6176(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3456(%r10)
vmovdqa %ymm1, 3808(%r10)
neg %ecx
jns done_1
add $704, %r9
add $1408, %r10
jmp innerloop_1
done_1:
sub $704, %r9
sub $1408, %r10
vmovdqa 0(%r9), %ymm0
vpaddw 704(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 6592(%rsp)
vmovdqa 1408(%r9), %ymm0
vpaddw 2112(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7296(%rsp)
vmovdqa 32(%r9), %ymm0
vpaddw 736(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 6624(%rsp)
vmovdqa 1440(%r9), %ymm0
vpaddw 2144(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7328(%rsp)
vmovdqa 64(%r9), %ymm0
vpaddw 768(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 6656(%rsp)
vmovdqa 1472(%r9), %ymm0
vpaddw 2176(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7360(%rsp)
vmovdqa 96(%r9), %ymm0
vpaddw 800(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 6688(%rsp)
vmovdqa 1504(%r9), %ymm0
vpaddw 2208(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7392(%rsp)
vmovdqa 128(%r9), %ymm0
vpaddw 832(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 6720(%rsp)
vmovdqa 1536(%r9), %ymm0
vpaddw 2240(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7424(%rsp)
vmovdqa 160(%r9), %ymm0
vpaddw 864(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 6752(%rsp)
vmovdqa 1568(%r9), %ymm0
vpaddw 2272(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7456(%rsp)
vmovdqa 192(%r9), %ymm0
vpaddw 896(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 6784(%rsp)
vmovdqa 1600(%r9), %ymm0
vpaddw 2304(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7488(%rsp)
vmovdqa 224(%r9), %ymm0
vpaddw 928(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 6816(%rsp)
vmovdqa 1632(%r9), %ymm0
vpaddw 2336(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7520(%rsp)
vmovdqa 256(%r9), %ymm0
vpaddw 960(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 6848(%rsp)
vmovdqa 1664(%r9), %ymm0
vpaddw 2368(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7552(%rsp)
vmovdqa 288(%r9), %ymm0
vpaddw 992(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 6880(%rsp)
vmovdqa 1696(%r9), %ymm0
vpaddw 2400(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7584(%rsp)
vmovdqa 320(%r9), %ymm0
vpaddw 1024(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 6912(%rsp)
vmovdqa 1728(%r9), %ymm0
vpaddw 2432(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7616(%rsp)
vmovdqa 352(%r9), %ymm0
vpaddw 1056(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 6944(%rsp)
vmovdqa 1760(%r9), %ymm0
vpaddw 2464(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7648(%rsp)
vmovdqa 384(%r9), %ymm0
vpaddw 1088(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 6976(%rsp)
vmovdqa 1792(%r9), %ymm0
vpaddw 2496(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7680(%rsp)
vmovdqa 416(%r9), %ymm0
vpaddw 1120(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7008(%rsp)
vmovdqa 1824(%r9), %ymm0
vpaddw 2528(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7712(%rsp)
vmovdqa 448(%r9), %ymm0
vpaddw 1152(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7040(%rsp)
vmovdqa 1856(%r9), %ymm0
vpaddw 2560(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7744(%rsp)
vmovdqa 480(%r9), %ymm0
vpaddw 1184(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7072(%rsp)
vmovdqa 1888(%r9), %ymm0
vpaddw 2592(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7776(%rsp)
vmovdqa 512(%r9), %ymm0
vpaddw 1216(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7104(%rsp)
vmovdqa 1920(%r9), %ymm0
vpaddw 2624(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7808(%rsp)
vmovdqa 544(%r9), %ymm0
vpaddw 1248(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7136(%rsp)
vmovdqa 1952(%r9), %ymm0
vpaddw 2656(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7840(%rsp)
vmovdqa 576(%r9), %ymm0
vpaddw 1280(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7168(%rsp)
vmovdqa 1984(%r9), %ymm0
vpaddw 2688(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7872(%rsp)
vmovdqa 608(%r9), %ymm0
vpaddw 1312(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7200(%rsp)
vmovdqa 2016(%r9), %ymm0
vpaddw 2720(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7904(%rsp)
vmovdqa 640(%r9), %ymm0
vpaddw 1344(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7232(%rsp)
vmovdqa 2048(%r9), %ymm0
vpaddw 2752(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7936(%rsp)
vmovdqa 672(%r9), %ymm0
vpaddw 1376(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7264(%rsp)
vmovdqa 2080(%r9), %ymm0
vpaddw 2784(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 7968(%rsp)
vmovdqa 6592(%rsp), %ymm0
vmovdqa 7296(%rsp), %ymm6
vmovdqa 6624(%rsp), %ymm1
vmovdqa 7328(%rsp), %ymm7
vmovdqa 6656(%rsp), %ymm2
vmovdqa 7360(%rsp), %ymm8
vmovdqa 6688(%rsp), %ymm3
vmovdqa 7392(%rsp), %ymm9
vmovdqa 6720(%rsp), %ymm4
vmovdqa 7424(%rsp), %ymm10
vmovdqa 6752(%rsp), %ymm5
vmovdqa 7456(%rsp), %ymm11
vpmullw %ymm0, %ymm6, %ymm12
vmovdqa %ymm12, 8000(%rsp)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 8032(%rsp)
vpmullw %ymm0, %ymm8, %ymm12
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 8064(%rsp)
vpmullw %ymm0, %ymm9, %ymm13
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 8096(%rsp)
vpmullw %ymm0, %ymm10, %ymm12
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 8128(%rsp)
vpmullw %ymm0, %ymm11, %ymm13
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm5, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 8160(%rsp)
vpmullw %ymm1, %ymm11, %ymm12
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 8192(%rsp)
vpmullw %ymm2, %ymm11, %ymm13
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 8224(%rsp)
vpmullw %ymm3, %ymm11, %ymm12
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 8256(%rsp)
vpmullw %ymm4, %ymm11, %ymm13
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 8288(%rsp)
vpmullw %ymm5, %ymm11, %ymm12
vmovdqa %ymm12, 8320(%rsp)
vmovdqa 6784(%rsp), %ymm0
vmovdqa 7488(%rsp), %ymm6
vmovdqa 6816(%rsp), %ymm1
vmovdqa 7520(%rsp), %ymm7
vmovdqa 6848(%rsp), %ymm2
vmovdqa 7552(%rsp), %ymm8
vmovdqa 6880(%rsp), %ymm3
vmovdqa 7584(%rsp), %ymm9
vmovdqa 6912(%rsp), %ymm4
vmovdqa 7616(%rsp), %ymm10
vpmullw %ymm0, %ymm6, %ymm12
vmovdqa %ymm12, 8384(%rsp)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 8416(%rsp)
vpmullw %ymm0, %ymm8, %ymm12
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 8448(%rsp)
vpmullw %ymm0, %ymm9, %ymm13
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 8480(%rsp)
vpmullw %ymm0, %ymm10, %ymm12
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 8512(%rsp)
vpmullw %ymm1, %ymm10, %ymm13
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 8544(%rsp)
vpmullw %ymm2, %ymm10, %ymm12
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 8576(%rsp)
vpmullw %ymm3, %ymm10, %ymm13
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 8608(%rsp)
vpmullw %ymm4, %ymm10, %ymm12
vmovdqa %ymm12, 8640(%rsp)
vpaddw 6592(%rsp), %ymm0, %ymm0
vpaddw 7296(%rsp), %ymm6, %ymm6
vpaddw 6624(%rsp), %ymm1, %ymm1
vpaddw 7328(%rsp), %ymm7, %ymm7
vpaddw 6656(%rsp), %ymm2, %ymm2
vpaddw 7360(%rsp), %ymm8, %ymm8
vpaddw 6688(%rsp), %ymm3, %ymm3
vpaddw 7392(%rsp), %ymm9, %ymm9
vpaddw 6720(%rsp), %ymm4, %ymm4
vpaddw 7424(%rsp), %ymm10, %ymm10
vpmullw %ymm0, %ymm11, %ymm12
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm5, %ymm6, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpsubw 8160(%rsp), %ymm12, %ymm12
vpsubw 8544(%rsp), %ymm12, %ymm12
vmovdqa %ymm12, 8352(%rsp)
vpmullw %ymm5, %ymm7, %ymm12
vpmullw %ymm5, %ymm8, %ymm13
vpmullw %ymm5, %ymm9, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpmullw %ymm1, %ymm11, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm2, %ymm10, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm3, %ymm9, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm4, %ymm8, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm2, %ymm11, %ymm5
vpaddw %ymm5, %ymm13, %ymm13
vpmullw %ymm3, %ymm10, %ymm5
vpaddw %ymm5, %ymm13, %ymm13
vpmullw %ymm4, %ymm9, %ymm5
vpaddw %ymm5, %ymm13, %ymm13
vpmullw %ymm3, %ymm11, %ymm5
vpaddw %ymm5, %ymm14, %ymm14
vpmullw %ymm4, %ymm10, %ymm5
vpaddw %ymm5, %ymm14, %ymm14
vpmullw %ymm4, %ymm11, %ymm5
vpaddw %ymm5, %ymm15, %ymm15
vpmullw %ymm0, %ymm10, %ymm11
vpmullw %ymm1, %ymm9, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm2, %ymm8, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm3, %ymm7, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm4, %ymm6, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm0, %ymm9, %ymm10
vpmullw %ymm1, %ymm8, %ymm5
vpaddw %ymm5, %ymm10, %ymm10
vpmullw %ymm2, %ymm7, %ymm5
vpaddw %ymm5, %ymm10, %ymm10
vpmullw %ymm3, %ymm6, %ymm5
vpaddw %ymm5, %ymm10, %ymm10
vpmullw %ymm0, %ymm8, %ymm9
vpmullw %ymm1, %ymm7, %ymm5
vpaddw %ymm5, %ymm9, %ymm9
vpmullw %ymm2, %ymm6, %ymm5
vpaddw %ymm5, %ymm9, %ymm9
vpmullw %ymm0, %ymm7, %ymm8
vpmullw %ymm1, %ymm6, %ymm5
vpaddw %ymm5, %ymm8, %ymm8
vpmullw %ymm0, %ymm6, %ymm7
vmovdqa 8192(%rsp), %ymm0
vpsubw 8384(%rsp), %ymm0, %ymm0
vpsubw %ymm0, %ymm12, %ymm6
vpsubw 8576(%rsp), %ymm6, %ymm6
vmovdqa %ymm6, 8384(%rsp)
vpaddw %ymm7, %ymm0, %ymm0
vpsubw 8000(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 8192(%rsp)
vmovdqa 8224(%rsp), %ymm1
vpsubw 8416(%rsp), %ymm1, %ymm1
vpsubw %ymm1, %ymm13, %ymm7
vpsubw 8608(%rsp), %ymm7, %ymm7
vmovdqa %ymm7, 8416(%rsp)
vpaddw %ymm8, %ymm1, %ymm1
vpsubw 8032(%rsp), %ymm1, %ymm1
vmovdqa %ymm1, 8224(%rsp)
vmovdqa 8256(%rsp), %ymm2
vpsubw 8448(%rsp), %ymm2, %ymm2
vpsubw %ymm2, %ymm14, %ymm8
vpsubw 8640(%rsp), %ymm8, %ymm8
vmovdqa %ymm8, 8448(%rsp)
vpaddw %ymm9, %ymm2, %ymm2
vpsubw 8064(%rsp), %ymm2, %ymm2
vmovdqa %ymm2, 8256(%rsp)
vmovdqa 8288(%rsp), %ymm3
vpsubw 8480(%rsp), %ymm3, %ymm3
vpsubw %ymm3, %ymm15, %ymm9
vmovdqa %ymm9, 8480(%rsp)
vpaddw %ymm10, %ymm3, %ymm3
vpsubw 8096(%rsp), %ymm3, %ymm3
vmovdqa %ymm3, 8288(%rsp)
vmovdqa 8320(%rsp), %ymm4
vpsubw 8512(%rsp), %ymm4, %ymm4
vpaddw %ymm11, %ymm4, %ymm4
vpsubw 8128(%rsp), %ymm4, %ymm4
vmovdqa %ymm4, 8320(%rsp)
vmovdqa 6944(%rsp), %ymm0
vmovdqa 7648(%rsp), %ymm6
vmovdqa 6976(%rsp), %ymm1
vmovdqa 7680(%rsp), %ymm7
vmovdqa 7008(%rsp), %ymm2
vmovdqa 7712(%rsp), %ymm8
vmovdqa 7040(%rsp), %ymm3
vmovdqa 7744(%rsp), %ymm9
vmovdqa 7072(%rsp), %ymm4
vmovdqa 7776(%rsp), %ymm10
vmovdqa 7104(%rsp), %ymm5
vmovdqa 7808(%rsp), %ymm11
vpmullw %ymm0, %ymm6, %ymm12
vmovdqa %ymm12, 8704(%rsp)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 8736(%rsp)
vpmullw %ymm0, %ymm8, %ymm12
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 8768(%rsp)
vpmullw %ymm0, %ymm9, %ymm13
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 8800(%rsp)
vpmullw %ymm0, %ymm10, %ymm12
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 8832(%rsp)
vpmullw %ymm0, %ymm11, %ymm13
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm5, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 8864(%rsp)
vpmullw %ymm1, %ymm11, %ymm12
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 8896(%rsp)
vpmullw %ymm2, %ymm11, %ymm13
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 8928(%rsp)
vpmullw %ymm3, %ymm11, %ymm12
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 8960(%rsp)
vpmullw %ymm4, %ymm11, %ymm13
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 8992(%rsp)
vpmullw %ymm5, %ymm11, %ymm12
vmovdqa %ymm12, 9024(%rsp)
vmovdqa 7136(%rsp), %ymm0
vmovdqa 7840(%rsp), %ymm6
vmovdqa 7168(%rsp), %ymm1
vmovdqa 7872(%rsp), %ymm7
vmovdqa 7200(%rsp), %ymm2
vmovdqa 7904(%rsp), %ymm8
vmovdqa 7232(%rsp), %ymm3
vmovdqa 7936(%rsp), %ymm9
vmovdqa 7264(%rsp), %ymm4
vmovdqa 7968(%rsp), %ymm10
vpmullw %ymm0, %ymm6, %ymm12
vmovdqa %ymm12, 9088(%rsp)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 9120(%rsp)
vpmullw %ymm0, %ymm8, %ymm12
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 9152(%rsp)
vpmullw %ymm0, %ymm9, %ymm13
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 9184(%rsp)
vpmullw %ymm0, %ymm10, %ymm12
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 9216(%rsp)
vpmullw %ymm1, %ymm10, %ymm13
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 9248(%rsp)
vpmullw %ymm2, %ymm10, %ymm12
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 9280(%rsp)
vpmullw %ymm3, %ymm10, %ymm13
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 9312(%rsp)
vpmullw %ymm4, %ymm10, %ymm12
vmovdqa %ymm12, 9344(%rsp)
vpaddw 6944(%rsp), %ymm0, %ymm0
vpaddw 7648(%rsp), %ymm6, %ymm6
vpaddw 6976(%rsp), %ymm1, %ymm1
vpaddw 7680(%rsp), %ymm7, %ymm7
vpaddw 7008(%rsp), %ymm2, %ymm2
vpaddw 7712(%rsp), %ymm8, %ymm8
vpaddw 7040(%rsp), %ymm3, %ymm3
vpaddw 7744(%rsp), %ymm9, %ymm9
vpaddw 7072(%rsp), %ymm4, %ymm4
vpaddw 7776(%rsp), %ymm10, %ymm10
vpmullw %ymm0, %ymm11, %ymm12
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm5, %ymm6, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpsubw 8864(%rsp), %ymm12, %ymm12
vpsubw 9248(%rsp), %ymm12, %ymm12
vmovdqa %ymm12, 9056(%rsp)
vpmullw %ymm5, %ymm7, %ymm12
vpmullw %ymm5, %ymm8, %ymm13
vpmullw %ymm5, %ymm9, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpmullw %ymm1, %ymm11, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm2, %ymm10, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm3, %ymm9, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm4, %ymm8, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm2, %ymm11, %ymm5
vpaddw %ymm5, %ymm13, %ymm13
vpmullw %ymm3, %ymm10, %ymm5
vpaddw %ymm5, %ymm13, %ymm13
vpmullw %ymm4, %ymm9, %ymm5
vpaddw %ymm5, %ymm13, %ymm13
vpmullw %ymm3, %ymm11, %ymm5
vpaddw %ymm5, %ymm14, %ymm14
vpmullw %ymm4, %ymm10, %ymm5
vpaddw %ymm5, %ymm14, %ymm14
vpmullw %ymm4, %ymm11, %ymm5
vpaddw %ymm5, %ymm15, %ymm15
vpmullw %ymm0, %ymm10, %ymm11
vpmullw %ymm1, %ymm9, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm2, %ymm8, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm3, %ymm7, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm4, %ymm6, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm0, %ymm9, %ymm10
vpmullw %ymm1, %ymm8, %ymm5
vpaddw %ymm5, %ymm10, %ymm10
vpmullw %ymm2, %ymm7, %ymm5
vpaddw %ymm5, %ymm10, %ymm10
vpmullw %ymm3, %ymm6, %ymm5
vpaddw %ymm5, %ymm10, %ymm10
vpmullw %ymm0, %ymm8, %ymm9
vpmullw %ymm1, %ymm7, %ymm5
vpaddw %ymm5, %ymm9, %ymm9
vpmullw %ymm2, %ymm6, %ymm5
vpaddw %ymm5, %ymm9, %ymm9
vpmullw %ymm0, %ymm7, %ymm8
vpmullw %ymm1, %ymm6, %ymm5
vpaddw %ymm5, %ymm8, %ymm8
vpmullw %ymm0, %ymm6, %ymm7
vmovdqa 8896(%rsp), %ymm0
vpsubw 9088(%rsp), %ymm0, %ymm0
vpsubw %ymm0, %ymm12, %ymm6
vpsubw 9280(%rsp), %ymm6, %ymm6
vmovdqa %ymm6, 9088(%rsp)
vpaddw %ymm7, %ymm0, %ymm0
vpsubw 8704(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 8896(%rsp)
vmovdqa 8928(%rsp), %ymm1
vpsubw 9120(%rsp), %ymm1, %ymm1
vpsubw %ymm1, %ymm13, %ymm7
vpsubw 9312(%rsp), %ymm7, %ymm7
vmovdqa %ymm7, 9120(%rsp)
vpaddw %ymm8, %ymm1, %ymm1
vpsubw 8736(%rsp), %ymm1, %ymm1
vmovdqa %ymm1, 8928(%rsp)
vmovdqa 8960(%rsp), %ymm2
vpsubw 9152(%rsp), %ymm2, %ymm2
vpsubw %ymm2, %ymm14, %ymm8
vpsubw 9344(%rsp), %ymm8, %ymm8
vmovdqa %ymm8, 9152(%rsp)
vpaddw %ymm9, %ymm2, %ymm2
vpsubw 8768(%rsp), %ymm2, %ymm2
vmovdqa %ymm2, 8960(%rsp)
vmovdqa 8992(%rsp), %ymm3
vpsubw 9184(%rsp), %ymm3, %ymm3
vpsubw %ymm3, %ymm15, %ymm9
vmovdqa %ymm9, 9184(%rsp)
vpaddw %ymm10, %ymm3, %ymm3
vpsubw 8800(%rsp), %ymm3, %ymm3
vmovdqa %ymm3, 8992(%rsp)
vmovdqa 9024(%rsp), %ymm4
vpsubw 9216(%rsp), %ymm4, %ymm4
vpaddw %ymm11, %ymm4, %ymm4
vpsubw 8832(%rsp), %ymm4, %ymm4
vmovdqa %ymm4, 9024(%rsp)
vmovdqa 6592(%rsp), %ymm0
vmovdqa 7296(%rsp), %ymm6
vpaddw 6944(%rsp), %ymm0, %ymm0
vpaddw 7648(%rsp), %ymm6, %ymm6
vmovdqa 6624(%rsp), %ymm1
vmovdqa 7328(%rsp), %ymm7
vpaddw 6976(%rsp), %ymm1, %ymm1
vpaddw 7680(%rsp), %ymm7, %ymm7
vmovdqa 6656(%rsp), %ymm2
vmovdqa 7360(%rsp), %ymm8
vpaddw 7008(%rsp), %ymm2, %ymm2
vpaddw 7712(%rsp), %ymm8, %ymm8
vmovdqa 6688(%rsp), %ymm3
vmovdqa 7392(%rsp), %ymm9
vpaddw 7040(%rsp), %ymm3, %ymm3
vpaddw 7744(%rsp), %ymm9, %ymm9
vmovdqa 6720(%rsp), %ymm4
vmovdqa 7424(%rsp), %ymm10
vpaddw 7072(%rsp), %ymm4, %ymm4
vpaddw 7776(%rsp), %ymm10, %ymm10
vmovdqa 6752(%rsp), %ymm5
vmovdqa 7456(%rsp), %ymm11
vpaddw 7104(%rsp), %ymm5, %ymm5
vpaddw 7808(%rsp), %ymm11, %ymm11
vpmullw %ymm0, %ymm6, %ymm12
vmovdqa %ymm12, 5888(%rsp)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 5920(%rsp)
vpmullw %ymm0, %ymm8, %ymm12
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 5952(%rsp)
vpmullw %ymm0, %ymm9, %ymm13
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 5984(%rsp)
vpmullw %ymm0, %ymm10, %ymm12
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 6016(%rsp)
vpmullw %ymm0, %ymm11, %ymm13
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm5, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6048(%rsp)
vpmullw %ymm1, %ymm11, %ymm12
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 6080(%rsp)
vpmullw %ymm2, %ymm11, %ymm13
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6112(%rsp)
vpmullw %ymm3, %ymm11, %ymm12
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 6144(%rsp)
vpmullw %ymm4, %ymm11, %ymm13
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6176(%rsp)
vpmullw %ymm5, %ymm11, %ymm12
vmovdqa %ymm12, 6208(%rsp)
vmovdqa 6784(%rsp), %ymm0
vmovdqa 7488(%rsp), %ymm6
vpaddw 7136(%rsp), %ymm0, %ymm0
vpaddw 7840(%rsp), %ymm6, %ymm6
vmovdqa 6816(%rsp), %ymm1
vmovdqa 7520(%rsp), %ymm7
vpaddw 7168(%rsp), %ymm1, %ymm1
vpaddw 7872(%rsp), %ymm7, %ymm7
vmovdqa 6848(%rsp), %ymm2
vmovdqa 7552(%rsp), %ymm8
vpaddw 7200(%rsp), %ymm2, %ymm2
vpaddw 7904(%rsp), %ymm8, %ymm8
vmovdqa 6880(%rsp), %ymm3
vmovdqa 7584(%rsp), %ymm9
vpaddw 7232(%rsp), %ymm3, %ymm3
vpaddw 7936(%rsp), %ymm9, %ymm9
vmovdqa 6912(%rsp), %ymm4
vmovdqa 7616(%rsp), %ymm10
vpaddw 7264(%rsp), %ymm4, %ymm4
vpaddw 7968(%rsp), %ymm10, %ymm10
vpmullw %ymm0, %ymm6, %ymm12
vmovdqa %ymm12, 6272(%rsp)
vpmullw %ymm0, %ymm7, %ymm13
vpmullw %ymm1, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6304(%rsp)
vpmullw %ymm0, %ymm8, %ymm12
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 6336(%rsp)
vpmullw %ymm0, %ymm9, %ymm13
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm6, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6368(%rsp)
vpmullw %ymm0, %ymm10, %ymm12
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm6, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 6400(%rsp)
vpmullw %ymm1, %ymm10, %ymm13
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6432(%rsp)
vpmullw %ymm2, %ymm10, %ymm12
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm12, %ymm15, %ymm12
vmovdqa %ymm12, 6464(%rsp)
vpmullw %ymm3, %ymm10, %ymm13
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm13, %ymm15, %ymm13
vmovdqa %ymm13, 6496(%rsp)
vpmullw %ymm4, %ymm10, %ymm12
vmovdqa %ymm12, 6528(%rsp)
vpaddw 6592(%rsp), %ymm0, %ymm0
vpaddw 7296(%rsp), %ymm6, %ymm6
vpaddw 6944(%rsp), %ymm0, %ymm0
vpaddw 7648(%rsp), %ymm6, %ymm6
vpaddw 6624(%rsp), %ymm1, %ymm1
vpaddw 7328(%rsp), %ymm7, %ymm7
vpaddw 6976(%rsp), %ymm1, %ymm1
vpaddw 7680(%rsp), %ymm7, %ymm7
vpaddw 6656(%rsp), %ymm2, %ymm2
vpaddw 7360(%rsp), %ymm8, %ymm8
vpaddw 7008(%rsp), %ymm2, %ymm2
vpaddw 7712(%rsp), %ymm8, %ymm8
vpaddw 6688(%rsp), %ymm3, %ymm3
vpaddw 7392(%rsp), %ymm9, %ymm9
vpaddw 7040(%rsp), %ymm3, %ymm3
vpaddw 7744(%rsp), %ymm9, %ymm9
vpaddw 6720(%rsp), %ymm4, %ymm4
vpaddw 7424(%rsp), %ymm10, %ymm10
vpaddw 7072(%rsp), %ymm4, %ymm4
vpaddw 7776(%rsp), %ymm10, %ymm10
vpmullw %ymm0, %ymm11, %ymm12
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpmullw %ymm5, %ymm6, %ymm15
vpaddw %ymm15, %ymm12, %ymm12
vpsubw 6048(%rsp), %ymm12, %ymm12
vpsubw 6432(%rsp), %ymm12, %ymm12
vmovdqa %ymm12, 6240(%rsp)
vpmullw %ymm5, %ymm7, %ymm12
vpmullw %ymm5, %ymm8, %ymm13
vpmullw %ymm5, %ymm9, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpmullw %ymm1, %ymm11, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm2, %ymm10, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm3, %ymm9, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm4, %ymm8, %ymm5
vpaddw %ymm5, %ymm12, %ymm12
vpmullw %ymm2, %ymm11, %ymm5
vpaddw %ymm5, %ymm13, %ymm13
vpmullw %ymm3, %ymm10, %ymm5
vpaddw %ymm5, %ymm13, %ymm13
vpmullw %ymm4, %ymm9, %ymm5
vpaddw %ymm5, %ymm13, %ymm13
vpmullw %ymm3, %ymm11, %ymm5
vpaddw %ymm5, %ymm14, %ymm14
vpmullw %ymm4, %ymm10, %ymm5
vpaddw %ymm5, %ymm14, %ymm14
vpmullw %ymm4, %ymm11, %ymm5
vpaddw %ymm5, %ymm15, %ymm15
vpmullw %ymm0, %ymm10, %ymm11
vpmullw %ymm1, %ymm9, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm2, %ymm8, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm3, %ymm7, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm4, %ymm6, %ymm5
vpaddw %ymm5, %ymm11, %ymm11
vpmullw %ymm0, %ymm9, %ymm10
vpmullw %ymm1, %ymm8, %ymm5
vpaddw %ymm5, %ymm10, %ymm10
vpmullw %ymm2, %ymm7, %ymm5
vpaddw %ymm5, %ymm10, %ymm10
vpmullw %ymm3, %ymm6, %ymm5
vpaddw %ymm5, %ymm10, %ymm10
vpmullw %ymm0, %ymm8, %ymm9
vpmullw %ymm1, %ymm7, %ymm5
vpaddw %ymm5, %ymm9, %ymm9
vpmullw %ymm2, %ymm6, %ymm5
vpaddw %ymm5, %ymm9, %ymm9
vpmullw %ymm0, %ymm7, %ymm8
vpmullw %ymm1, %ymm6, %ymm5
vpaddw %ymm5, %ymm8, %ymm8
vpmullw %ymm0, %ymm6, %ymm7
vmovdqa 6080(%rsp), %ymm0
vpsubw 6272(%rsp), %ymm0, %ymm0
vpsubw %ymm0, %ymm12, %ymm6
vpsubw 6464(%rsp), %ymm6, %ymm6
vmovdqa %ymm6, 6272(%rsp)
vpaddw %ymm7, %ymm0, %ymm0
vpsubw 5888(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 6080(%rsp)
vmovdqa 6112(%rsp), %ymm1
vpsubw 6304(%rsp), %ymm1, %ymm1
vpsubw %ymm1, %ymm13, %ymm7
vpsubw 6496(%rsp), %ymm7, %ymm7
vmovdqa %ymm7, 6304(%rsp)
vpaddw %ymm8, %ymm1, %ymm1
vpsubw 5920(%rsp), %ymm1, %ymm1
vmovdqa %ymm1, 6112(%rsp)
vmovdqa 6144(%rsp), %ymm2
vpsubw 6336(%rsp), %ymm2, %ymm2
vpsubw %ymm2, %ymm14, %ymm8
vpsubw 6528(%rsp), %ymm8, %ymm8
vmovdqa %ymm8, 6336(%rsp)
vpaddw %ymm9, %ymm2, %ymm2
vpsubw 5952(%rsp), %ymm2, %ymm2
vmovdqa %ymm2, 6144(%rsp)
vmovdqa 6176(%rsp), %ymm3
vpsubw 6368(%rsp), %ymm3, %ymm3
vpsubw %ymm3, %ymm15, %ymm9
vmovdqa %ymm9, 6368(%rsp)
vpaddw %ymm10, %ymm3, %ymm3
vpsubw 5984(%rsp), %ymm3, %ymm3
vmovdqa %ymm3, 6176(%rsp)
vmovdqa 6208(%rsp), %ymm4
vpsubw 6400(%rsp), %ymm4, %ymm4
vpaddw %ymm11, %ymm4, %ymm4
vpsubw 6016(%rsp), %ymm4, %ymm4
vmovdqa %ymm4, 6208(%rsp)
vmovdqa 8352(%rsp), %ymm0
vpsubw 8704(%rsp), %ymm0, %ymm0
vmovdqa 6240(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 9056(%rsp), %ymm1, %ymm6
vpsubw 8000(%rsp), %ymm0, %ymm0
vpaddw 5888(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 8352(%rsp)
vmovdqa 8384(%rsp), %ymm0
vpsubw 8736(%rsp), %ymm0, %ymm0
vmovdqa 6272(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 9088(%rsp), %ymm1, %ymm7
vpsubw 8032(%rsp), %ymm0, %ymm0
vpaddw 5920(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 8384(%rsp)
vmovdqa 8416(%rsp), %ymm0
vpsubw 8768(%rsp), %ymm0, %ymm0
vmovdqa 6304(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 9120(%rsp), %ymm1, %ymm8
vpsubw 8064(%rsp), %ymm0, %ymm0
vpaddw 5952(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 8416(%rsp)
vmovdqa 8448(%rsp), %ymm0
vpsubw 8800(%rsp), %ymm0, %ymm0
vmovdqa 6336(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 9152(%rsp), %ymm1, %ymm9
vpsubw 8096(%rsp), %ymm0, %ymm0
vpaddw 5984(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 8448(%rsp)
vmovdqa 8480(%rsp), %ymm0
vpsubw 8832(%rsp), %ymm0, %ymm0
vmovdqa 6368(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 9184(%rsp), %ymm1, %ymm10
vpsubw 8128(%rsp), %ymm0, %ymm0
vpaddw 6016(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 8480(%rsp)
vmovdqa 8512(%rsp), %ymm0
vpsubw 8864(%rsp), %ymm0, %ymm0
vmovdqa 6400(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 9216(%rsp), %ymm1, %ymm11
vpsubw 8160(%rsp), %ymm0, %ymm0
vpaddw 6048(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 8512(%rsp)
vmovdqa 8544(%rsp), %ymm0
vpsubw 8896(%rsp), %ymm0, %ymm0
vmovdqa 6432(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 9248(%rsp), %ymm1, %ymm12
vpsubw 8192(%rsp), %ymm0, %ymm0
vpaddw 6080(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 8544(%rsp)
vmovdqa 8576(%rsp), %ymm0
vpsubw 8928(%rsp), %ymm0, %ymm0
vmovdqa 6464(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 9280(%rsp), %ymm1, %ymm13
vpsubw 8224(%rsp), %ymm0, %ymm0
vpaddw 6112(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 8576(%rsp)
vmovdqa 8608(%rsp), %ymm0
vpsubw 8960(%rsp), %ymm0, %ymm0
vmovdqa 6496(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 9312(%rsp), %ymm1, %ymm14
vpsubw 8256(%rsp), %ymm0, %ymm0
vpaddw 6144(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 8608(%rsp)
vmovdqa 8640(%rsp), %ymm0
vpsubw 8992(%rsp), %ymm0, %ymm0
vmovdqa 6528(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 9344(%rsp), %ymm1, %ymm15
vpsubw 8288(%rsp), %ymm0, %ymm0
vpaddw 6176(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 8640(%rsp)
vmovdqa 6208(%rsp), %ymm0
vpsubw 8320(%rsp), %ymm0, %ymm0
vpsubw 9024(%rsp), %ymm0, %ymm0
vpsubw 3488(%r10), %ymm0, %ymm0
vpsubw 4896(%r10), %ymm0, %ymm0
vmovdqa %ymm0, 4192(%r10)
vmovdqa 3520(%r10), %ymm0
vpsubw 4224(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm6, %ymm6
vpsubw 4928(%r10), %ymm6, %ymm6
vpsubw 2816(%r10), %ymm0, %ymm0
vpaddw 8000(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3520(%r10)
vmovdqa %ymm6, 4224(%r10)
vmovdqa 3552(%r10), %ymm0
vpsubw 4256(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm7, %ymm7
vpsubw 4960(%r10), %ymm7, %ymm7
vpsubw 2848(%r10), %ymm0, %ymm0
vpaddw 8032(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3552(%r10)
vmovdqa %ymm7, 4256(%r10)
vmovdqa 3584(%r10), %ymm0
vpsubw 4288(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm8, %ymm8
vpsubw 4992(%r10), %ymm8, %ymm8
vpsubw 2880(%r10), %ymm0, %ymm0
vpaddw 8064(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3584(%r10)
vmovdqa %ymm8, 4288(%r10)
vmovdqa 3616(%r10), %ymm0
vpsubw 4320(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm9, %ymm9
vpsubw 5024(%r10), %ymm9, %ymm9
vpsubw 2912(%r10), %ymm0, %ymm0
vpaddw 8096(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3616(%r10)
vmovdqa %ymm9, 4320(%r10)
vmovdqa 3648(%r10), %ymm0
vpsubw 4352(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm10, %ymm10
vpsubw 5056(%r10), %ymm10, %ymm10
vpsubw 2944(%r10), %ymm0, %ymm0
vpaddw 8128(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3648(%r10)
vmovdqa %ymm10, 4352(%r10)
vmovdqa 3680(%r10), %ymm0
vpsubw 4384(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm11, %ymm11
vpsubw 5088(%r10), %ymm11, %ymm11
vpsubw 2976(%r10), %ymm0, %ymm0
vpaddw 8160(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3680(%r10)
vmovdqa %ymm11, 4384(%r10)
vmovdqa 3712(%r10), %ymm0
vpsubw 4416(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm12, %ymm12
vpsubw 5120(%r10), %ymm12, %ymm12
vpsubw 3008(%r10), %ymm0, %ymm0
vpaddw 8192(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3712(%r10)
vmovdqa %ymm12, 4416(%r10)
vmovdqa 3744(%r10), %ymm0
vpsubw 4448(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm13, %ymm13
vpsubw 5152(%r10), %ymm13, %ymm13
vpsubw 3040(%r10), %ymm0, %ymm0
vpaddw 8224(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3744(%r10)
vmovdqa %ymm13, 4448(%r10)
vmovdqa 3776(%r10), %ymm0
vpsubw 4480(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm14, %ymm14
vpsubw 5184(%r10), %ymm14, %ymm14
vpsubw 3072(%r10), %ymm0, %ymm0
vpaddw 8256(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3776(%r10)
vmovdqa %ymm14, 4480(%r10)
vmovdqa 3808(%r10), %ymm0
vpsubw 4512(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm15, %ymm15
vpsubw 5216(%r10), %ymm15, %ymm15
vpsubw 3104(%r10), %ymm0, %ymm0
vpaddw 8288(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3808(%r10)
vmovdqa %ymm15, 4512(%r10)
vmovdqa 3840(%r10), %ymm0
vpsubw 4544(%r10), %ymm0, %ymm0
vmovdqa 9024(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5248(%r10), %ymm1, %ymm1
vpsubw 3136(%r10), %ymm0, %ymm0
vpaddw 8320(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3840(%r10)
vmovdqa %ymm1, 4544(%r10)
vmovdqa 3872(%r10), %ymm0
vpsubw 4576(%r10), %ymm0, %ymm0
vmovdqa 9056(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5280(%r10), %ymm1, %ymm1
vpsubw 3168(%r10), %ymm0, %ymm0
vpaddw 8352(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3872(%r10)
vmovdqa %ymm1, 4576(%r10)
vmovdqa 3904(%r10), %ymm0
vpsubw 4608(%r10), %ymm0, %ymm0
vmovdqa 9088(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5312(%r10), %ymm1, %ymm1
vpsubw 3200(%r10), %ymm0, %ymm0
vpaddw 8384(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3904(%r10)
vmovdqa %ymm1, 4608(%r10)
vmovdqa 3936(%r10), %ymm0
vpsubw 4640(%r10), %ymm0, %ymm0
vmovdqa 9120(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5344(%r10), %ymm1, %ymm1
vpsubw 3232(%r10), %ymm0, %ymm0
vpaddw 8416(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3936(%r10)
vmovdqa %ymm1, 4640(%r10)
vmovdqa 3968(%r10), %ymm0
vpsubw 4672(%r10), %ymm0, %ymm0
vmovdqa 9152(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5376(%r10), %ymm1, %ymm1
vpsubw 3264(%r10), %ymm0, %ymm0
vpaddw 8448(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3968(%r10)
vmovdqa %ymm1, 4672(%r10)
vmovdqa 4000(%r10), %ymm0
vpsubw 4704(%r10), %ymm0, %ymm0
vmovdqa 9184(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5408(%r10), %ymm1, %ymm1
vpsubw 3296(%r10), %ymm0, %ymm0
vpaddw 8480(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4000(%r10)
vmovdqa %ymm1, 4704(%r10)
vmovdqa 4032(%r10), %ymm0
vpsubw 4736(%r10), %ymm0, %ymm0
vmovdqa 9216(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5440(%r10), %ymm1, %ymm1
vpsubw 3328(%r10), %ymm0, %ymm0
vpaddw 8512(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4032(%r10)
vmovdqa %ymm1, 4736(%r10)
vmovdqa 4064(%r10), %ymm0
vpsubw 4768(%r10), %ymm0, %ymm0
vmovdqa 9248(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5472(%r10), %ymm1, %ymm1
vpsubw 3360(%r10), %ymm0, %ymm0
vpaddw 8544(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4064(%r10)
vmovdqa %ymm1, 4768(%r10)
vmovdqa 4096(%r10), %ymm0
vpsubw 4800(%r10), %ymm0, %ymm0
vmovdqa 9280(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5504(%r10), %ymm1, %ymm1
vpsubw 3392(%r10), %ymm0, %ymm0
vpaddw 8576(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4096(%r10)
vmovdqa %ymm1, 4800(%r10)
vmovdqa 4128(%r10), %ymm0
vpsubw 4832(%r10), %ymm0, %ymm0
vmovdqa 9312(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5536(%r10), %ymm1, %ymm1
vpsubw 3424(%r10), %ymm0, %ymm0
vpaddw 8608(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4128(%r10)
vmovdqa %ymm1, 4832(%r10)
vmovdqa 4160(%r10), %ymm0
vpsubw 4864(%r10), %ymm0, %ymm0
vmovdqa 9344(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5568(%r10), %ymm1, %ymm1
vpsubw 3456(%r10), %ymm0, %ymm0
vpaddw 8640(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4160(%r10)
vmovdqa %ymm1, 4864(%r10)
vpxor %ymm1, %ymm1, %ymm1
vmovdqa %ymm1, 5600(%r10)
subq $32, %rsp
vmovdqa 2816(%r10), %ymm0
vmovdqa 2880(%r10), %ymm1
vmovdqa 2944(%r10), %ymm2
vmovdqa 3008(%r10), %ymm3
vpunpcklwd 2848(%r10), %ymm0, %ymm4
vpunpckhwd 2848(%r10), %ymm0, %ymm5
vpunpcklwd 2912(%r10), %ymm1, %ymm6
vpunpckhwd 2912(%r10), %ymm1, %ymm7
vpunpcklwd 2976(%r10), %ymm2, %ymm8
vpunpckhwd 2976(%r10), %ymm2, %ymm9
vpunpcklwd 3040(%r10), %ymm3, %ymm10
vpunpckhwd 3040(%r10), %ymm3, %ymm11
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
vmovdqa 3072(%r10), %ymm0
vmovdqa 3136(%r10), %ymm1
vmovdqa 3200(%r10), %ymm2
vmovdqa 3264(%r10), %ymm3
vpunpcklwd 3104(%r10), %ymm0, %ymm12
vpunpckhwd 3104(%r10), %ymm0, %ymm13
vpunpcklwd 3168(%r10), %ymm1, %ymm14
vpunpckhwd 3168(%r10), %ymm1, %ymm15
vpunpcklwd 3232(%r10), %ymm2, %ymm0
vpunpckhwd 3232(%r10), %ymm2, %ymm1
vpunpcklwd 3296(%r10), %ymm3, %ymm2
vpunpckhwd 3296(%r10), %ymm3, %ymm3
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
vmovdqa %ymm15, 192(%r12)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 384(%r12)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 576(%r12)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 768(%r12)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 960(%r12)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 1152(%r12)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 1536(%r12)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 1728(%r12)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 1920(%r12)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 2112(%r12)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 2304(%r12)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 2496(%r12)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 2688(%r12)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1344(%r12)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 2880(%r12)
vmovdqa 3328(%r10), %ymm0
vmovdqa 3392(%r10), %ymm1
vmovdqa 3456(%r10), %ymm2
vmovdqa 3520(%r10), %ymm3
vpunpcklwd 3360(%r10), %ymm0, %ymm4
vpunpckhwd 3360(%r10), %ymm0, %ymm5
vpunpcklwd 3424(%r10), %ymm1, %ymm6
vpunpckhwd 3424(%r10), %ymm1, %ymm7
vpunpcklwd 3488(%r10), %ymm2, %ymm8
vpunpckhwd 3488(%r10), %ymm2, %ymm9
vpunpcklwd 3552(%r10), %ymm3, %ymm10
vpunpckhwd 3552(%r10), %ymm3, %ymm11
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
vmovdqa 3584(%r10), %ymm0
vmovdqa 3648(%r10), %ymm1
vmovdqa 3712(%r10), %ymm2
vmovdqa 3776(%r10), %ymm3
vpunpcklwd 3616(%r10), %ymm0, %ymm12
vpunpckhwd 3616(%r10), %ymm0, %ymm13
vpunpcklwd 3680(%r10), %ymm1, %ymm14
vpunpckhwd 3680(%r10), %ymm1, %ymm15
vpunpcklwd 3744(%r10), %ymm2, %ymm0
vpunpckhwd 3744(%r10), %ymm2, %ymm1
vpunpcklwd 3808(%r10), %ymm3, %ymm2
vpunpckhwd 3808(%r10), %ymm3, %ymm3
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
vmovdqa %ymm15, 224(%r12)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 416(%r12)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 608(%r12)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 800(%r12)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 992(%r12)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 1184(%r12)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 1568(%r12)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 1760(%r12)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 1952(%r12)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 2144(%r12)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 2336(%r12)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 2528(%r12)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 2720(%r12)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1376(%r12)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 2912(%r12)
vmovdqa 3840(%r10), %ymm0
vmovdqa 3904(%r10), %ymm1
vmovdqa 3968(%r10), %ymm2
vmovdqa 4032(%r10), %ymm3
vpunpcklwd 3872(%r10), %ymm0, %ymm4
vpunpckhwd 3872(%r10), %ymm0, %ymm5
vpunpcklwd 3936(%r10), %ymm1, %ymm6
vpunpckhwd 3936(%r10), %ymm1, %ymm7
vpunpcklwd 4000(%r10), %ymm2, %ymm8
vpunpckhwd 4000(%r10), %ymm2, %ymm9
vpunpcklwd 4064(%r10), %ymm3, %ymm10
vpunpckhwd 4064(%r10), %ymm3, %ymm11
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
vmovdqa 4096(%r10), %ymm0
vmovdqa 4160(%r10), %ymm1
vmovdqa 4224(%r10), %ymm2
vmovdqa 4288(%r10), %ymm3
vpunpcklwd 4128(%r10), %ymm0, %ymm12
vpunpckhwd 4128(%r10), %ymm0, %ymm13
vpunpcklwd 4192(%r10), %ymm1, %ymm14
vpunpckhwd 4192(%r10), %ymm1, %ymm15
vpunpcklwd 4256(%r10), %ymm2, %ymm0
vpunpckhwd 4256(%r10), %ymm2, %ymm1
vpunpcklwd 4320(%r10), %ymm3, %ymm2
vpunpckhwd 4320(%r10), %ymm3, %ymm3
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
vmovdqa %ymm15, 256(%r12)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 448(%r12)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 640(%r12)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 832(%r12)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 1024(%r12)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 1216(%r12)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 1600(%r12)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 1792(%r12)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 1984(%r12)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 2176(%r12)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 2368(%r12)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 2560(%r12)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 2752(%r12)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1408(%r12)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 2944(%r12)
vmovdqa 4224(%r10), %ymm0
vmovdqa 4288(%r10), %ymm1
vmovdqa 4352(%r10), %ymm2
vmovdqa 4416(%r10), %ymm3
vpunpcklwd 4256(%r10), %ymm0, %ymm4
vpunpckhwd 4256(%r10), %ymm0, %ymm5
vpunpcklwd 4320(%r10), %ymm1, %ymm6
vpunpckhwd 4320(%r10), %ymm1, %ymm7
vpunpcklwd 4384(%r10), %ymm2, %ymm8
vpunpckhwd 4384(%r10), %ymm2, %ymm9
vpunpcklwd 4448(%r10), %ymm3, %ymm10
vpunpckhwd 4448(%r10), %ymm3, %ymm11
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
vmovdqa 4480(%r10), %ymm0
vmovdqa 4544(%r10), %ymm1
vmovdqa 4608(%r10), %ymm2
vmovdqa 4672(%r10), %ymm3
vpunpcklwd 4512(%r10), %ymm0, %ymm12
vpunpckhwd 4512(%r10), %ymm0, %ymm13
vpunpcklwd 4576(%r10), %ymm1, %ymm14
vpunpckhwd 4576(%r10), %ymm1, %ymm15
vpunpcklwd 4640(%r10), %ymm2, %ymm0
vpunpckhwd 4640(%r10), %ymm2, %ymm1
vpunpcklwd 4704(%r10), %ymm3, %ymm2
vpunpckhwd 4704(%r10), %ymm3, %ymm3
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
vmovdqa %ymm15, 288(%r12)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 480(%r12)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 672(%r12)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 864(%r12)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 1056(%r12)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 1248(%r12)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 1632(%r12)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 1824(%r12)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 2016(%r12)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 2208(%r12)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 2400(%r12)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 2592(%r12)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 2784(%r12)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1440(%r12)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 2976(%r12)
vmovdqa 4736(%r10), %ymm0
vmovdqa 4800(%r10), %ymm1
vmovdqa 4864(%r10), %ymm2
vmovdqa 4928(%r10), %ymm3
vpunpcklwd 4768(%r10), %ymm0, %ymm4
vpunpckhwd 4768(%r10), %ymm0, %ymm5
vpunpcklwd 4832(%r10), %ymm1, %ymm6
vpunpckhwd 4832(%r10), %ymm1, %ymm7
vpunpcklwd 4896(%r10), %ymm2, %ymm8
vpunpckhwd 4896(%r10), %ymm2, %ymm9
vpunpcklwd 4960(%r10), %ymm3, %ymm10
vpunpckhwd 4960(%r10), %ymm3, %ymm11
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
vmovdqa 4992(%r10), %ymm0
vmovdqa 5056(%r10), %ymm1
vmovdqa 5120(%r10), %ymm2
vmovdqa 5184(%r10), %ymm3
vpunpcklwd 5024(%r10), %ymm0, %ymm12
vpunpckhwd 5024(%r10), %ymm0, %ymm13
vpunpcklwd 5088(%r10), %ymm1, %ymm14
vpunpckhwd 5088(%r10), %ymm1, %ymm15
vpunpcklwd 5152(%r10), %ymm2, %ymm0
vpunpckhwd 5152(%r10), %ymm2, %ymm1
vpunpcklwd 5216(%r10), %ymm3, %ymm2
vpunpckhwd 5216(%r10), %ymm3, %ymm3
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
vmovdqa %ymm15, 128(%r12)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 320(%r12)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 512(%r12)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 704(%r12)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 896(%r12)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 1088(%r12)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 1280(%r12)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 1664(%r12)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 1856(%r12)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 2048(%r12)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 2240(%r12)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 2432(%r12)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 2624(%r12)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 2816(%r12)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1472(%r12)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 3008(%r12)
vmovdqa 5248(%r10), %ymm0
vmovdqa 5312(%r10), %ymm1
vmovdqa 5376(%r10), %ymm2
vmovdqa 5440(%r10), %ymm3
vpunpcklwd 5280(%r10), %ymm0, %ymm4
vpunpckhwd 5280(%r10), %ymm0, %ymm5
vpunpcklwd 5344(%r10), %ymm1, %ymm6
vpunpckhwd 5344(%r10), %ymm1, %ymm7
vpunpcklwd 5408(%r10), %ymm2, %ymm8
vpunpckhwd 5408(%r10), %ymm2, %ymm9
vpunpcklwd 5472(%r10), %ymm3, %ymm10
vpunpckhwd 5472(%r10), %ymm3, %ymm11
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
vmovdqa 5504(%r10), %ymm0
vmovdqa 5568(%r10), %ymm1
vmovdqa 5632(%r10), %ymm2
vmovdqa 5696(%r10), %ymm3
vpunpcklwd 5536(%r10), %ymm0, %ymm12
vpunpckhwd 5536(%r10), %ymm0, %ymm13
vpunpcklwd 5600(%r10), %ymm1, %ymm14
vpunpckhwd 5600(%r10), %ymm1, %ymm15
vpunpcklwd 5664(%r10), %ymm2, %ymm0
vpunpckhwd 5664(%r10), %ymm2, %ymm1
vpunpcklwd 5728(%r10), %ymm3, %ymm2
vpunpckhwd 5728(%r10), %ymm3, %ymm3
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
vmovdqa %ymm15, 160(%r12)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 352(%r12)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 544(%r12)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 736(%r12)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 928(%r12)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 1120(%r12)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 1312(%r12)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 1696(%r12)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 1888(%r12)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 2080(%r12)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 2272(%r12)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 2464(%r12)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 2656(%r12)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 2848(%r12)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1504(%r12)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 3040(%r12)
addq $32, %rsp
add $1536, %rax
add $1536, %r11
add $3072, %r12
dec %ecx
jnz karatsuba_loop_1
sub $12288, %r12
add $9408, %rsp
subq $1280, %rsp
vmovdqa const729(%rip), %ymm15
vmovdqa const3_inv(%rip), %ymm14
vmovdqa const5_inv(%rip), %ymm13
vmovdqa const9(%rip), %ymm12
vmovdqa 96(%r12), %ymm0
vpsubw 192(%r12), %ymm0, %ymm0
vmovdqa 480(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 288(%r12), %ymm1, %ymm1
vpsubw 0(%r12), %ymm0, %ymm0
vpaddw 384(%r12), %ymm0, %ymm0
vmovdqa 672(%r12), %ymm2
vpsubw 768(%r12), %ymm2, %ymm2
vmovdqa 1056(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 864(%r12), %ymm3, %ymm3
vpsubw 576(%r12), %ymm2, %ymm2
vpaddw 960(%r12), %ymm2, %ymm2
vmovdqa 1248(%r12), %ymm4
vpsubw 1344(%r12), %ymm4, %ymm4
vmovdqa 1632(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 1440(%r12), %ymm5, %ymm5
vpsubw 1152(%r12), %ymm4, %ymm4
vpaddw 1536(%r12), %ymm4, %ymm4
vpsubw 576(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 0(%r12), %ymm1, %ymm1
vpaddw 1152(%r12), %ymm1, %ymm1
vmovdqa 288(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 1440(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 864(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 0(%r12), %ymm8
vmovdqa 864(%r12), %ymm9
vmovdqa %ymm8, 0(%rsp)
vmovdqa %ymm0, 32(%rsp)
vmovdqa %ymm1, 64(%rsp)
vmovdqa %ymm7, 96(%rsp)
vmovdqa %ymm5, 128(%rsp)
vmovdqa %ymm2, 160(%rsp)
vmovdqa %ymm3, 192(%rsp)
vmovdqa %ymm9, 224(%rsp)
vmovdqa 1824(%r12), %ymm0
vpsubw 1920(%r12), %ymm0, %ymm0
vmovdqa 2208(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 2016(%r12), %ymm1, %ymm1
vpsubw 1728(%r12), %ymm0, %ymm0
vpaddw 2112(%r12), %ymm0, %ymm0
vmovdqa 2400(%r12), %ymm2
vpsubw 2496(%r12), %ymm2, %ymm2
vmovdqa 2784(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 2592(%r12), %ymm3, %ymm3
vpsubw 2304(%r12), %ymm2, %ymm2
vpaddw 2688(%r12), %ymm2, %ymm2
vmovdqa 2976(%r12), %ymm4
vpsubw 3072(%r12), %ymm4, %ymm4
vmovdqa 3360(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 3168(%r12), %ymm5, %ymm5
vpsubw 2880(%r12), %ymm4, %ymm4
vpaddw 3264(%r12), %ymm4, %ymm4
vpsubw 2304(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 1728(%r12), %ymm1, %ymm1
vpaddw 2880(%r12), %ymm1, %ymm1
vmovdqa 2016(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 3168(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 2592(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 1728(%r12), %ymm8
vmovdqa 2592(%r12), %ymm9
vmovdqa %ymm8, 256(%rsp)
vmovdqa %ymm0, 288(%rsp)
vmovdqa %ymm1, 320(%rsp)
vmovdqa %ymm7, 352(%rsp)
vmovdqa %ymm5, 384(%rsp)
vmovdqa %ymm2, 416(%rsp)
vmovdqa %ymm3, 448(%rsp)
vmovdqa %ymm9, 480(%rsp)
vmovdqa 3552(%r12), %ymm0
vpsubw 3648(%r12), %ymm0, %ymm0
vmovdqa 3936(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 3744(%r12), %ymm1, %ymm1
vpsubw 3456(%r12), %ymm0, %ymm0
vpaddw 3840(%r12), %ymm0, %ymm0
vmovdqa 4128(%r12), %ymm2
vpsubw 4224(%r12), %ymm2, %ymm2
vmovdqa 4512(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 4320(%r12), %ymm3, %ymm3
vpsubw 4032(%r12), %ymm2, %ymm2
vpaddw 4416(%r12), %ymm2, %ymm2
vmovdqa 4704(%r12), %ymm4
vpsubw 4800(%r12), %ymm4, %ymm4
vmovdqa 5088(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 4896(%r12), %ymm5, %ymm5
vpsubw 4608(%r12), %ymm4, %ymm4
vpaddw 4992(%r12), %ymm4, %ymm4
vpsubw 4032(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 3456(%r12), %ymm1, %ymm1
vpaddw 4608(%r12), %ymm1, %ymm1
vmovdqa 3744(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 4896(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 4320(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 3456(%r12), %ymm8
vmovdqa 4320(%r12), %ymm9
vmovdqa %ymm8, 512(%rsp)
vmovdqa %ymm0, 544(%rsp)
vmovdqa %ymm1, 576(%rsp)
vmovdqa %ymm7, 608(%rsp)
vmovdqa %ymm5, 640(%rsp)
vmovdqa %ymm2, 672(%rsp)
vmovdqa %ymm3, 704(%rsp)
vmovdqa %ymm9, 736(%rsp)
vmovdqa 5280(%r12), %ymm0
vpsubw 5376(%r12), %ymm0, %ymm0
vmovdqa 5664(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5472(%r12), %ymm1, %ymm1
vpsubw 5184(%r12), %ymm0, %ymm0
vpaddw 5568(%r12), %ymm0, %ymm0
vmovdqa 5856(%r12), %ymm2
vpsubw 5952(%r12), %ymm2, %ymm2
vmovdqa 6240(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 6048(%r12), %ymm3, %ymm3
vpsubw 5760(%r12), %ymm2, %ymm2
vpaddw 6144(%r12), %ymm2, %ymm2
vmovdqa 6432(%r12), %ymm4
vpsubw 6528(%r12), %ymm4, %ymm4
vmovdqa 6816(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 6624(%r12), %ymm5, %ymm5
vpsubw 6336(%r12), %ymm4, %ymm4
vpaddw 6720(%r12), %ymm4, %ymm4
vpsubw 5760(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 5184(%r12), %ymm1, %ymm1
vpaddw 6336(%r12), %ymm1, %ymm1
vmovdqa 5472(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 6624(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 6048(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 5184(%r12), %ymm8
vmovdqa 6048(%r12), %ymm9
vmovdqa %ymm8, 768(%rsp)
vmovdqa %ymm0, 800(%rsp)
vmovdqa %ymm1, 832(%rsp)
vmovdqa %ymm7, 864(%rsp)
vmovdqa %ymm5, 896(%rsp)
vmovdqa %ymm2, 928(%rsp)
vmovdqa %ymm3, 960(%rsp)
vmovdqa %ymm9, 992(%rsp)
vmovdqa 7008(%r12), %ymm0
vpsubw 7104(%r12), %ymm0, %ymm0
vmovdqa 7392(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 7200(%r12), %ymm1, %ymm1
vpsubw 6912(%r12), %ymm0, %ymm0
vpaddw 7296(%r12), %ymm0, %ymm0
vmovdqa 7584(%r12), %ymm2
vpsubw 7680(%r12), %ymm2, %ymm2
vmovdqa 7968(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 7776(%r12), %ymm3, %ymm3
vpsubw 7488(%r12), %ymm2, %ymm2
vpaddw 7872(%r12), %ymm2, %ymm2
vmovdqa 8160(%r12), %ymm4
vpsubw 8256(%r12), %ymm4, %ymm4
vmovdqa 8544(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 8352(%r12), %ymm5, %ymm5
vpsubw 8064(%r12), %ymm4, %ymm4
vpaddw 8448(%r12), %ymm4, %ymm4
vpsubw 7488(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 6912(%r12), %ymm1, %ymm1
vpaddw 8064(%r12), %ymm1, %ymm1
vmovdqa 7200(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 8352(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 7776(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 6912(%r12), %ymm8
vmovdqa 7776(%r12), %ymm9
vmovdqa %ymm8, 1024(%rsp)
vmovdqa %ymm0, 1056(%rsp)
vmovdqa %ymm1, 1088(%rsp)
vmovdqa %ymm7, 1120(%rsp)
vmovdqa %ymm5, 1152(%rsp)
vmovdqa %ymm2, 1184(%rsp)
vmovdqa %ymm3, 1216(%rsp)
vmovdqa %ymm9, 1248(%rsp)
vmovdqa 8736(%r12), %ymm0
vpsubw 8832(%r12), %ymm0, %ymm0
vmovdqa 9120(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 8928(%r12), %ymm1, %ymm1
vpsubw 8640(%r12), %ymm0, %ymm0
vpaddw 9024(%r12), %ymm0, %ymm0
vmovdqa 9312(%r12), %ymm2
vpsubw 9408(%r12), %ymm2, %ymm2
vmovdqa 9696(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 9504(%r12), %ymm3, %ymm3
vpsubw 9216(%r12), %ymm2, %ymm2
vpaddw 9600(%r12), %ymm2, %ymm2
vmovdqa 9888(%r12), %ymm4
vpsubw 9984(%r12), %ymm4, %ymm4
vmovdqa 10272(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 10080(%r12), %ymm5, %ymm5
vpsubw 9792(%r12), %ymm4, %ymm4
vpaddw 10176(%r12), %ymm4, %ymm4
vpsubw 9216(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 8640(%r12), %ymm1, %ymm1
vpaddw 9792(%r12), %ymm1, %ymm1
vmovdqa 8928(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 10080(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 9504(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 8640(%r12), %ymm8
vmovdqa 9504(%r12), %ymm9
vmovdqa %ymm8, 1280(%rsp)
vmovdqa %ymm0, 1312(%rsp)
vmovdqa %ymm1, 1344(%rsp)
vmovdqa %ymm7, 1376(%rsp)
vmovdqa %ymm5, 1408(%rsp)
vmovdqa %ymm2, 1440(%rsp)
vmovdqa %ymm3, 1472(%rsp)
vmovdqa %ymm9, 1504(%rsp)
vmovdqa 10464(%r12), %ymm0
vpsubw 10560(%r12), %ymm0, %ymm0
vmovdqa 10848(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 10656(%r12), %ymm1, %ymm1
vpsubw 10368(%r12), %ymm0, %ymm0
vpaddw 10752(%r12), %ymm0, %ymm0
vmovdqa 11040(%r12), %ymm2
vpsubw 11136(%r12), %ymm2, %ymm2
vmovdqa 11424(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 11232(%r12), %ymm3, %ymm3
vpsubw 10944(%r12), %ymm2, %ymm2
vpaddw 11328(%r12), %ymm2, %ymm2
vmovdqa 11616(%r12), %ymm4
vpsubw 11712(%r12), %ymm4, %ymm4
vmovdqa 12000(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 11808(%r12), %ymm5, %ymm5
vpsubw 11520(%r12), %ymm4, %ymm4
vpaddw 11904(%r12), %ymm4, %ymm4
vpsubw 10944(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 10368(%r12), %ymm1, %ymm1
vpaddw 11520(%r12), %ymm1, %ymm1
vmovdqa 10656(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 11808(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 11232(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 10368(%r12), %ymm8
vmovdqa 11232(%r12), %ymm9
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
vmovdqu 0(%rdi), %ymm9
vpaddw %ymm9, %ymm11, %ymm9
vmovdqu %ymm9, 0(%rdi)
vmovdqu 352(%rdi), %ymm9
vpaddw %ymm9, %ymm6, %ymm9
vmovdqu %ymm9, 352(%rdi)
vmovdqu 704(%rdi), %ymm9
vpaddw %ymm9, %ymm3, %ymm9
vmovdqu %ymm9, 704(%rdi)
vmovdqu 1056(%rdi), %ymm9
vpaddw %ymm9, %ymm4, %ymm9
vmovdqu %ymm9, 1056(%rdi)
vmovdqu 54(%rdi), %ymm9
vpaddw %ymm9, %ymm7, %ymm9
vmovdqu %ymm9, 54(%rdi)
vmovdqu 406(%rdi), %ymm9
vpaddw %ymm9, %ymm8, %ymm9
vmovdqu %ymm9, 406(%rdi)
vmovdqu 758(%rdi), %ymm9
vpaddw %ymm9, %ymm5, %ymm9
vmovdqu %ymm9, 758(%rdi)
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
vmovdqu 88(%rdi), %ymm7
vpaddw %ymm7, %ymm5, %ymm7
vmovdqu %ymm7, 88(%rdi)
vmovdqu 440(%rdi), %ymm7
vpaddw %ymm7, %ymm6, %ymm7
vmovdqu %ymm7, 440(%rdi)
vmovdqu 792(%rdi), %ymm7
vpaddw %ymm7, %ymm10, %ymm7
vmovdqu %ymm7, 792(%rdi)
vmovdqu 1144(%rdi), %ymm7
vpaddw %ymm7, %ymm9, %ymm7
vmovdqu %ymm7, 1144(%rdi)
vmovdqu 142(%rdi), %ymm7
vpaddw %ymm7, %ymm3, %ymm7
vmovdqu %ymm7, 142(%rdi)
vmovdqu 494(%rdi), %ymm7
vpaddw %ymm7, %ymm4, %ymm7
vmovdqu %ymm7, 494(%rdi)
vmovdqu 846(%rdi), %ymm7
vpaddw %ymm7, %ymm11, %ymm7
vmovdqu %ymm7, 846(%rdi)
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
vmovdqu 176(%rdi), %ymm3
vpaddw %ymm3, %ymm11, %ymm3
vmovdqu %ymm3, 176(%rdi)
vmovdqu 528(%rdi), %ymm3
vpaddw %ymm3, %ymm6, %ymm3
vmovdqu %ymm3, 528(%rdi)
vmovdqu 880(%rdi), %ymm3
vpaddw %ymm3, %ymm8, %ymm3
vmovdqu %ymm3, 880(%rdi)
vmovdqu 1232(%rdi), %ymm3
vpaddw %ymm3, %ymm7, %ymm3
vmovdqu %ymm3, 1232(%rdi)
vmovdqu 230(%rdi), %ymm3
vpaddw %ymm3, %ymm10, %ymm3
vmovdqu %ymm3, 230(%rdi)
vmovdqu 582(%rdi), %ymm3
vpaddw %ymm3, %ymm9, %ymm3
vmovdqu %ymm3, 582(%rdi)
vmovdqu 934(%rdi), %ymm3
vpaddw %ymm3, %ymm5, %ymm3
vmovdqu %ymm3, 934(%rdi)
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
vmovdqu 264(%rdi), %ymm10
vpaddw %ymm10, %ymm5, %ymm10
vmovdqu %ymm10, 264(%rdi)
vmovdqu 616(%rdi), %ymm10
vpaddw %ymm10, %ymm6, %ymm10
vmovdqu %ymm10, 616(%rdi)
vmovdqu 968(%rdi), %ymm10
vpaddw %ymm10, %ymm4, %ymm10
vmovdqu %ymm10, 968(%rdi)
vmovdqu 1320(%rdi), %ymm10
vpaddw %ymm10, %ymm3, %ymm10
vmovdqu %ymm10, 1320(%rdi)
vmovdqu 318(%rdi), %ymm10
vpaddw %ymm10, %ymm8, %ymm10
vmovdqu %ymm10, 318(%rdi)
vmovdqu 670(%rdi), %ymm10
vpaddw %ymm10, %ymm7, %ymm10
vmovdqu %ymm10, 670(%rdi)
vmovdqu 1022(%rdi), %ymm10
vpaddw %ymm10, %ymm11, %ymm10
vmovdqu %ymm10, 1022(%rdi)
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
vmovdqu 352(%rdi), %ymm8
vpaddw %ymm8, %ymm11, %ymm8
vmovdqu %ymm8, 352(%rdi)
vmovdqu 704(%rdi), %ymm8
vpaddw %ymm8, %ymm6, %ymm8
vmovdqu %ymm8, 704(%rdi)
vmovdqu 1056(%rdi), %ymm8
vpaddw %ymm8, %ymm9, %ymm8
vmovdqu %ymm8, 1056(%rdi)
vmovdqu 54(%rdi), %ymm8
vpaddw %ymm8, %ymm10, %ymm8
vmovdqu %ymm8, 54(%rdi)
vmovdqu 406(%rdi), %ymm8
vpaddw %ymm8, %ymm4, %ymm8
vmovdqu %ymm8, 406(%rdi)
vmovdqu 758(%rdi), %ymm8
vpaddw %ymm8, %ymm3, %ymm8
vmovdqu %ymm8, 758(%rdi)
vmovdqu 1110(%rdi), %ymm8
vpaddw %ymm8, %ymm5, %ymm8
vmovdqu %ymm8, 1110(%rdi)
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
vpaddd %ymm11, %ymm9, %ymm8
vpaddd %ymm6, %ymm10, %ymm7
vpsubd %ymm3, %ymm8, %ymm8
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
vpsubd %ymm10, %ymm8, %ymm8
vpsubd %ymm9, %ymm7, %ymm7
vpsrld $1, %ymm8, %ymm8
vpsrld $1, %ymm7, %ymm7
vpand mask32_to_16(%rip), %ymm8, %ymm8
vpand mask32_to_16(%rip), %ymm7, %ymm7
vpackusdw %ymm7, %ymm8, %ymm7
vmovdqa 928(%rsp), %ymm8
vpaddw 1184(%rsp), %ymm8, %ymm9
vpsubw 1184(%rsp), %ymm8, %ymm8
vpsrlw $2, %ymm8, %ymm8
vpsubw %ymm6, %ymm8, %ymm8
vpmullw %ymm14, %ymm8, %ymm8
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
vpsubw %ymm8, %ymm10, %ymm10
vpsubw %ymm10, %ymm8, %ymm8
vpsubw %ymm8, %ymm6, %ymm6
vpmullw %ymm13, %ymm10, %ymm10
vpsubw %ymm10, %ymm6, %ymm6
vmovdqu 440(%rdi), %ymm4
vpaddw %ymm4, %ymm5, %ymm4
vmovdqu %ymm4, 440(%rdi)
vmovdqu 792(%rdi), %ymm4
vpaddw %ymm4, %ymm6, %ymm4
vmovdqu %ymm4, 792(%rdi)
vmovdqu 1144(%rdi), %ymm4
vpaddw %ymm4, %ymm7, %ymm4
vmovdqu %ymm4, 1144(%rdi)
vmovdqu 142(%rdi), %ymm4
vpaddw %ymm4, %ymm8, %ymm4
vmovdqu %ymm4, 142(%rdi)
vmovdqu 494(%rdi), %ymm4
vpaddw %ymm4, %ymm9, %ymm4
vmovdqu %ymm4, 494(%rdi)
vmovdqu 846(%rdi), %ymm4
vpaddw %ymm4, %ymm10, %ymm4
vmovdqu %ymm4, 846(%rdi)
vmovdqu 1198(%rdi), %ymm4
vpaddw %ymm4, %ymm11, %ymm4
vmovdqu %ymm4, 1198(%rdi)
vmovdqa 192(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm10
vpunpckhwd const0(%rip), %ymm11, %ymm9
vpslld $1, %ymm10, %ymm10
vpslld $1, %ymm9, %ymm9
vmovdqa 448(%rsp), %ymm8
vpunpcklwd const0(%rip), %ymm8, %ymm7
vpunpckhwd const0(%rip), %ymm8, %ymm8
vmovdqa 704(%rsp), %ymm6
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
vmovdqa 1728(%rsp), %ymm5
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
vmovdqa 960(%rsp), %ymm4
vpaddw 1216(%rsp), %ymm4, %ymm7
vpsubw 1216(%rsp), %ymm4, %ymm4
vpsrlw $2, %ymm4, %ymm4
vpsubw %ymm6, %ymm4, %ymm4
vpmullw %ymm14, %ymm4, %ymm4
vpsllw $1, %ymm11, %ymm8
vpsubw %ymm8, %ymm7, %ymm8
vpsllw $7, %ymm5, %ymm7
vpsubw %ymm7, %ymm8, %ymm7
vpsrlw $3, %ymm7, %ymm7
vpsubw %ymm3, %ymm7, %ymm7
vmovdqa 1472(%rsp), %ymm8
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
vmovdqu 528(%rdi), %ymm9
vpaddw %ymm9, %ymm11, %ymm9
vmovdqu %ymm9, 528(%rdi)
vmovdqu 880(%rdi), %ymm9
vpaddw %ymm9, %ymm6, %ymm9
vmovdqu %ymm9, 880(%rdi)
vmovdqu 1232(%rdi), %ymm9
vpaddw %ymm9, %ymm3, %ymm9
vmovdqu %ymm9, 1232(%rdi)
vmovdqu 230(%rdi), %ymm9
vpaddw %ymm9, %ymm4, %ymm9
vmovdqu %ymm9, 230(%rdi)
vmovdqu 582(%rdi), %ymm9
vpaddw %ymm9, %ymm7, %ymm9
vmovdqu %ymm9, 582(%rdi)
vmovdqu 934(%rdi), %ymm9
vpaddw %ymm9, %ymm8, %ymm9
vmovdqu %ymm9, 934(%rdi)
vmovdqu 1286(%rdi), %ymm9
vpaddw %ymm9, %ymm5, %ymm9
vmovdqu %ymm9, 1286(%rdi)
vmovdqa 224(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm8
vpunpckhwd const0(%rip), %ymm5, %ymm7
vpslld $1, %ymm8, %ymm8
vpslld $1, %ymm7, %ymm7
vmovdqa 480(%rsp), %ymm4
vpunpcklwd const0(%rip), %ymm4, %ymm3
vpunpckhwd const0(%rip), %ymm4, %ymm4
vmovdqa 736(%rsp), %ymm6
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
vmovdqa 1760(%rsp), %ymm11
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
vmovdqa 992(%rsp), %ymm9
vpaddw 1248(%rsp), %ymm9, %ymm3
vpsubw 1248(%rsp), %ymm9, %ymm9
vpsrlw $2, %ymm9, %ymm9
vpsubw %ymm6, %ymm9, %ymm9
vpmullw %ymm14, %ymm9, %ymm9
vpsllw $1, %ymm5, %ymm4
vpsubw %ymm4, %ymm3, %ymm4
vpsllw $7, %ymm11, %ymm3
vpsubw %ymm3, %ymm4, %ymm3
vpsrlw $3, %ymm3, %ymm3
vpsubw %ymm10, %ymm3, %ymm3
vmovdqa 1504(%rsp), %ymm4
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
vmovdqu 616(%rdi), %ymm7
vpaddw %ymm7, %ymm5, %ymm7
vmovdqu %ymm7, 616(%rdi)
vmovdqu 968(%rdi), %ymm7
vpaddw %ymm7, %ymm6, %ymm7
vmovdqu %ymm7, 968(%rdi)
vmovdqu 1320(%rdi), %ymm7
vpaddw %ymm7, %ymm10, %ymm7
vmovdqu %ymm7, 1320(%rdi)
vmovdqu 318(%rdi), %ymm7
vpaddw %ymm7, %ymm9, %ymm7
vmovdqu %ymm7, 318(%rdi)
vmovdqu 670(%rdi), %ymm7
vpaddw %ymm7, %ymm3, %ymm7
vmovdqu %ymm7, 670(%rdi)
vmovdqu 1022(%rdi), %ymm7
vpaddw %ymm7, %ymm4, %ymm7
vmovdqu %ymm7, 1022(%rdi)
vmovdqu 1374(%rdi), %ymm7
vpaddw %ymm7, %ymm11, %ymm7
vmovdqu %ymm7, 1374(%rdi)
vmovdqa 128(%r12), %ymm0
vpsubw 224(%r12), %ymm0, %ymm0
vmovdqa 512(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 320(%r12), %ymm1, %ymm1
vpsubw 32(%r12), %ymm0, %ymm0
vpaddw 416(%r12), %ymm0, %ymm0
vmovdqa 704(%r12), %ymm2
vpsubw 800(%r12), %ymm2, %ymm2
vmovdqa 1088(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 896(%r12), %ymm3, %ymm3
vpsubw 608(%r12), %ymm2, %ymm2
vpaddw 992(%r12), %ymm2, %ymm2
vmovdqa 1280(%r12), %ymm4
vpsubw 1376(%r12), %ymm4, %ymm4
vmovdqa 1664(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 1472(%r12), %ymm5, %ymm5
vpsubw 1184(%r12), %ymm4, %ymm4
vpaddw 1568(%r12), %ymm4, %ymm4
vpsubw 608(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 32(%r12), %ymm1, %ymm1
vpaddw 1184(%r12), %ymm1, %ymm1
vmovdqa 320(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 1472(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 896(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 32(%r12), %ymm8
vmovdqa 896(%r12), %ymm9
vmovdqa %ymm8, 0(%rsp)
vmovdqa %ymm0, 32(%rsp)
vmovdqa %ymm1, 64(%rsp)
vmovdqa %ymm7, 96(%rsp)
vmovdqa %ymm5, 128(%rsp)
vmovdqa %ymm2, 160(%rsp)
vmovdqa %ymm3, 192(%rsp)
vmovdqa %ymm9, 224(%rsp)
vmovdqa 1856(%r12), %ymm0
vpsubw 1952(%r12), %ymm0, %ymm0
vmovdqa 2240(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 2048(%r12), %ymm1, %ymm1
vpsubw 1760(%r12), %ymm0, %ymm0
vpaddw 2144(%r12), %ymm0, %ymm0
vmovdqa 2432(%r12), %ymm2
vpsubw 2528(%r12), %ymm2, %ymm2
vmovdqa 2816(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 2624(%r12), %ymm3, %ymm3
vpsubw 2336(%r12), %ymm2, %ymm2
vpaddw 2720(%r12), %ymm2, %ymm2
vmovdqa 3008(%r12), %ymm4
vpsubw 3104(%r12), %ymm4, %ymm4
vmovdqa 3392(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 3200(%r12), %ymm5, %ymm5
vpsubw 2912(%r12), %ymm4, %ymm4
vpaddw 3296(%r12), %ymm4, %ymm4
vpsubw 2336(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 1760(%r12), %ymm1, %ymm1
vpaddw 2912(%r12), %ymm1, %ymm1
vmovdqa 2048(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 3200(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 2624(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 1760(%r12), %ymm8
vmovdqa 2624(%r12), %ymm9
vmovdqa %ymm8, 256(%rsp)
vmovdqa %ymm0, 288(%rsp)
vmovdqa %ymm1, 320(%rsp)
vmovdqa %ymm7, 352(%rsp)
vmovdqa %ymm5, 384(%rsp)
vmovdqa %ymm2, 416(%rsp)
vmovdqa %ymm3, 448(%rsp)
vmovdqa %ymm9, 480(%rsp)
vmovdqa 3584(%r12), %ymm0
vpsubw 3680(%r12), %ymm0, %ymm0
vmovdqa 3968(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 3776(%r12), %ymm1, %ymm1
vpsubw 3488(%r12), %ymm0, %ymm0
vpaddw 3872(%r12), %ymm0, %ymm0
vmovdqa 4160(%r12), %ymm2
vpsubw 4256(%r12), %ymm2, %ymm2
vmovdqa 4544(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 4352(%r12), %ymm3, %ymm3
vpsubw 4064(%r12), %ymm2, %ymm2
vpaddw 4448(%r12), %ymm2, %ymm2
vmovdqa 4736(%r12), %ymm4
vpsubw 4832(%r12), %ymm4, %ymm4
vmovdqa 5120(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 4928(%r12), %ymm5, %ymm5
vpsubw 4640(%r12), %ymm4, %ymm4
vpaddw 5024(%r12), %ymm4, %ymm4
vpsubw 4064(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 3488(%r12), %ymm1, %ymm1
vpaddw 4640(%r12), %ymm1, %ymm1
vmovdqa 3776(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 4928(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 4352(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 3488(%r12), %ymm8
vmovdqa 4352(%r12), %ymm9
vmovdqa %ymm8, 512(%rsp)
vmovdqa %ymm0, 544(%rsp)
vmovdqa %ymm1, 576(%rsp)
vmovdqa %ymm7, 608(%rsp)
vmovdqa %ymm5, 640(%rsp)
vmovdqa %ymm2, 672(%rsp)
vmovdqa %ymm3, 704(%rsp)
vmovdqa %ymm9, 736(%rsp)
vmovdqa 5312(%r12), %ymm0
vpsubw 5408(%r12), %ymm0, %ymm0
vmovdqa 5696(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5504(%r12), %ymm1, %ymm1
vpsubw 5216(%r12), %ymm0, %ymm0
vpaddw 5600(%r12), %ymm0, %ymm0
vmovdqa 5888(%r12), %ymm2
vpsubw 5984(%r12), %ymm2, %ymm2
vmovdqa 6272(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 6080(%r12), %ymm3, %ymm3
vpsubw 5792(%r12), %ymm2, %ymm2
vpaddw 6176(%r12), %ymm2, %ymm2
vmovdqa 6464(%r12), %ymm4
vpsubw 6560(%r12), %ymm4, %ymm4
vmovdqa 6848(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 6656(%r12), %ymm5, %ymm5
vpsubw 6368(%r12), %ymm4, %ymm4
vpaddw 6752(%r12), %ymm4, %ymm4
vpsubw 5792(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 5216(%r12), %ymm1, %ymm1
vpaddw 6368(%r12), %ymm1, %ymm1
vmovdqa 5504(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 6656(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 6080(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 5216(%r12), %ymm8
vmovdqa 6080(%r12), %ymm9
vmovdqa %ymm8, 768(%rsp)
vmovdqa %ymm0, 800(%rsp)
vmovdqa %ymm1, 832(%rsp)
vmovdqa %ymm7, 864(%rsp)
vmovdqa %ymm5, 896(%rsp)
vmovdqa %ymm2, 928(%rsp)
vmovdqa %ymm3, 960(%rsp)
vmovdqa %ymm9, 992(%rsp)
vmovdqa 7040(%r12), %ymm0
vpsubw 7136(%r12), %ymm0, %ymm0
vmovdqa 7424(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 7232(%r12), %ymm1, %ymm1
vpsubw 6944(%r12), %ymm0, %ymm0
vpaddw 7328(%r12), %ymm0, %ymm0
vmovdqa 7616(%r12), %ymm2
vpsubw 7712(%r12), %ymm2, %ymm2
vmovdqa 8000(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 7808(%r12), %ymm3, %ymm3
vpsubw 7520(%r12), %ymm2, %ymm2
vpaddw 7904(%r12), %ymm2, %ymm2
vmovdqa 8192(%r12), %ymm4
vpsubw 8288(%r12), %ymm4, %ymm4
vmovdqa 8576(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 8384(%r12), %ymm5, %ymm5
vpsubw 8096(%r12), %ymm4, %ymm4
vpaddw 8480(%r12), %ymm4, %ymm4
vpsubw 7520(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 6944(%r12), %ymm1, %ymm1
vpaddw 8096(%r12), %ymm1, %ymm1
vmovdqa 7232(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 8384(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 7808(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 6944(%r12), %ymm8
vmovdqa 7808(%r12), %ymm9
vmovdqa %ymm8, 1024(%rsp)
vmovdqa %ymm0, 1056(%rsp)
vmovdqa %ymm1, 1088(%rsp)
vmovdqa %ymm7, 1120(%rsp)
vmovdqa %ymm5, 1152(%rsp)
vmovdqa %ymm2, 1184(%rsp)
vmovdqa %ymm3, 1216(%rsp)
vmovdqa %ymm9, 1248(%rsp)
vmovdqa 8768(%r12), %ymm0
vpsubw 8864(%r12), %ymm0, %ymm0
vmovdqa 9152(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 8960(%r12), %ymm1, %ymm1
vpsubw 8672(%r12), %ymm0, %ymm0
vpaddw 9056(%r12), %ymm0, %ymm0
vmovdqa 9344(%r12), %ymm2
vpsubw 9440(%r12), %ymm2, %ymm2
vmovdqa 9728(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 9536(%r12), %ymm3, %ymm3
vpsubw 9248(%r12), %ymm2, %ymm2
vpaddw 9632(%r12), %ymm2, %ymm2
vmovdqa 9920(%r12), %ymm4
vpsubw 10016(%r12), %ymm4, %ymm4
vmovdqa 10304(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 10112(%r12), %ymm5, %ymm5
vpsubw 9824(%r12), %ymm4, %ymm4
vpaddw 10208(%r12), %ymm4, %ymm4
vpsubw 9248(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 8672(%r12), %ymm1, %ymm1
vpaddw 9824(%r12), %ymm1, %ymm1
vmovdqa 8960(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 10112(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 9536(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 8672(%r12), %ymm8
vmovdqa 9536(%r12), %ymm9
vmovdqa %ymm8, 1280(%rsp)
vmovdqa %ymm0, 1312(%rsp)
vmovdqa %ymm1, 1344(%rsp)
vmovdqa %ymm7, 1376(%rsp)
vmovdqa %ymm5, 1408(%rsp)
vmovdqa %ymm2, 1440(%rsp)
vmovdqa %ymm3, 1472(%rsp)
vmovdqa %ymm9, 1504(%rsp)
vmovdqa 10496(%r12), %ymm0
vpsubw 10592(%r12), %ymm0, %ymm0
vmovdqa 10880(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 10688(%r12), %ymm1, %ymm1
vpsubw 10400(%r12), %ymm0, %ymm0
vpaddw 10784(%r12), %ymm0, %ymm0
vmovdqa 11072(%r12), %ymm2
vpsubw 11168(%r12), %ymm2, %ymm2
vmovdqa 11456(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 11264(%r12), %ymm3, %ymm3
vpsubw 10976(%r12), %ymm2, %ymm2
vpaddw 11360(%r12), %ymm2, %ymm2
vmovdqa 11648(%r12), %ymm4
vpsubw 11744(%r12), %ymm4, %ymm4
vmovdqa 12032(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 11840(%r12), %ymm5, %ymm5
vpsubw 11552(%r12), %ymm4, %ymm4
vpaddw 11936(%r12), %ymm4, %ymm4
vpsubw 10976(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 10400(%r12), %ymm1, %ymm1
vpaddw 11552(%r12), %ymm1, %ymm1
vmovdqa 10688(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 11840(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 11264(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 10400(%r12), %ymm8
vmovdqa 11264(%r12), %ymm9
vmovdqa %ymm8, 1536(%rsp)
vmovdqa %ymm0, 1568(%rsp)
vmovdqa %ymm1, 1600(%rsp)
vmovdqa %ymm7, 1632(%rsp)
vmovdqa %ymm5, 1664(%rsp)
vmovdqa %ymm2, 1696(%rsp)
vmovdqa %ymm3, 1728(%rsp)
vmovdqa %ymm9, 1760(%rsp)
vmovdqa 0(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm4
vpunpckhwd const0(%rip), %ymm11, %ymm3
vpslld $1, %ymm4, %ymm4
vpslld $1, %ymm3, %ymm3
vmovdqa 256(%rsp), %ymm9
vpunpcklwd const0(%rip), %ymm9, %ymm10
vpunpckhwd const0(%rip), %ymm9, %ymm9
vmovdqa 512(%rsp), %ymm6
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
vmovdqa 1536(%rsp), %ymm5
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
vmovdqa 768(%rsp), %ymm7
vpaddw 1024(%rsp), %ymm7, %ymm10
vpsubw 1024(%rsp), %ymm7, %ymm7
vpsrlw $2, %ymm7, %ymm7
vpsubw %ymm6, %ymm7, %ymm7
vpmullw %ymm14, %ymm7, %ymm7
vpsllw $1, %ymm11, %ymm9
vpsubw %ymm9, %ymm10, %ymm9
vpsllw $7, %ymm5, %ymm10
vpsubw %ymm10, %ymm9, %ymm10
vpsrlw $3, %ymm10, %ymm10
vpsubw %ymm8, %ymm10, %ymm10
vmovdqa 1280(%rsp), %ymm9
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
vmovdqu 32(%rdi), %ymm3
vpaddw %ymm3, %ymm11, %ymm3
vmovdqu %ymm3, 32(%rdi)
vmovdqu 384(%rdi), %ymm3
vpaddw %ymm3, %ymm6, %ymm3
vmovdqu %ymm3, 384(%rdi)
vmovdqu 736(%rdi), %ymm3
vpaddw %ymm3, %ymm8, %ymm3
vmovdqu %ymm3, 736(%rdi)
vmovdqu 1088(%rdi), %ymm3
vpaddw %ymm3, %ymm7, %ymm3
vmovdqu %ymm3, 1088(%rdi)
vmovdqu 86(%rdi), %ymm3
vpaddw %ymm3, %ymm10, %ymm3
vmovdqu %ymm3, 86(%rdi)
vmovdqu 438(%rdi), %ymm3
vpaddw %ymm3, %ymm9, %ymm3
vmovdqu %ymm3, 438(%rdi)
vmovdqu 790(%rdi), %ymm3
vpaddw %ymm3, %ymm5, %ymm3
vmovdqu %ymm3, 790(%rdi)
vmovdqa 32(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm9
vpunpckhwd const0(%rip), %ymm5, %ymm10
vpslld $1, %ymm9, %ymm9
vpslld $1, %ymm10, %ymm10
vmovdqa 288(%rsp), %ymm7
vpunpcklwd const0(%rip), %ymm7, %ymm8
vpunpckhwd const0(%rip), %ymm7, %ymm7
vmovdqa 544(%rsp), %ymm6
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
vmovdqa 1568(%rsp), %ymm11
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
vmovdqa 800(%rsp), %ymm3
vpaddw 1056(%rsp), %ymm3, %ymm8
vpsubw 1056(%rsp), %ymm3, %ymm3
vpsrlw $2, %ymm3, %ymm3
vpsubw %ymm6, %ymm3, %ymm3
vpmullw %ymm14, %ymm3, %ymm3
vpsllw $1, %ymm5, %ymm7
vpsubw %ymm7, %ymm8, %ymm7
vpsllw $7, %ymm11, %ymm8
vpsubw %ymm8, %ymm7, %ymm8
vpsrlw $3, %ymm8, %ymm8
vpsubw %ymm4, %ymm8, %ymm8
vmovdqa 1312(%rsp), %ymm7
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
vmovdqu 120(%rdi), %ymm10
vpaddw %ymm10, %ymm5, %ymm10
vmovdqu %ymm10, 120(%rdi)
vmovdqu 472(%rdi), %ymm10
vpaddw %ymm10, %ymm6, %ymm10
vmovdqu %ymm10, 472(%rdi)
vmovdqu 824(%rdi), %ymm10
vpaddw %ymm10, %ymm4, %ymm10
vmovdqu %ymm10, 824(%rdi)
vmovdqu 1176(%rdi), %ymm10
vpaddw %ymm10, %ymm3, %ymm10
vmovdqu %ymm10, 1176(%rdi)
vmovdqu 174(%rdi), %ymm10
vpaddw %ymm10, %ymm8, %ymm10
vmovdqu %ymm10, 174(%rdi)
vmovdqu 526(%rdi), %ymm10
vpaddw %ymm10, %ymm7, %ymm10
vmovdqu %ymm10, 526(%rdi)
vmovdqu 878(%rdi), %ymm10
vpaddw %ymm10, %ymm11, %ymm10
vmovdqu %ymm10, 878(%rdi)
vmovdqa 64(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm7
vpunpckhwd const0(%rip), %ymm11, %ymm8
vpslld $1, %ymm7, %ymm7
vpslld $1, %ymm8, %ymm8
vmovdqa 320(%rsp), %ymm3
vpunpcklwd const0(%rip), %ymm3, %ymm4
vpunpckhwd const0(%rip), %ymm3, %ymm3
vmovdqa 576(%rsp), %ymm6
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
vmovdqa 1600(%rsp), %ymm5
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
vmovdqa 832(%rsp), %ymm10
vpaddw 1088(%rsp), %ymm10, %ymm4
vpsubw 1088(%rsp), %ymm10, %ymm10
vpsrlw $2, %ymm10, %ymm10
vpsubw %ymm6, %ymm10, %ymm10
vpmullw %ymm14, %ymm10, %ymm10
vpsllw $1, %ymm11, %ymm3
vpsubw %ymm3, %ymm4, %ymm3
vpsllw $7, %ymm5, %ymm4
vpsubw %ymm4, %ymm3, %ymm4
vpsrlw $3, %ymm4, %ymm4
vpsubw %ymm9, %ymm4, %ymm4
vmovdqa 1344(%rsp), %ymm3
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
vmovdqu 208(%rdi), %ymm8
vpaddw %ymm8, %ymm11, %ymm8
vmovdqu %ymm8, 208(%rdi)
vmovdqu 560(%rdi), %ymm8
vpaddw %ymm8, %ymm6, %ymm8
vmovdqu %ymm8, 560(%rdi)
vmovdqu 912(%rdi), %ymm8
vpaddw %ymm8, %ymm9, %ymm8
vmovdqu %ymm8, 912(%rdi)
vmovdqu 1264(%rdi), %ymm8
vpaddw %ymm8, %ymm10, %ymm8
vmovdqu %ymm8, 1264(%rdi)
vmovdqu 262(%rdi), %ymm8
vpaddw %ymm8, %ymm4, %ymm8
vmovdqu %ymm8, 262(%rdi)
vmovdqu 614(%rdi), %ymm8
vpaddw %ymm8, %ymm3, %ymm8
vmovdqu %ymm8, 614(%rdi)
vmovdqu 966(%rdi), %ymm8
vpaddw %ymm8, %ymm5, %ymm8
vmovdqu %ymm8, 966(%rdi)
vmovdqa 96(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm3
vpunpckhwd const0(%rip), %ymm5, %ymm4
vpslld $1, %ymm3, %ymm3
vpslld $1, %ymm4, %ymm4
vmovdqa 352(%rsp), %ymm10
vpunpcklwd const0(%rip), %ymm10, %ymm9
vpunpckhwd const0(%rip), %ymm10, %ymm10
vmovdqa 608(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm11
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm11, %ymm9, %ymm8
vpaddd %ymm6, %ymm10, %ymm7
vpsubd %ymm3, %ymm8, %ymm8
vpsubd %ymm4, %ymm7, %ymm7
vpsubd %ymm11, %ymm9, %ymm11
vpsubd %ymm6, %ymm10, %ymm6
vpsrld $1, %ymm11, %ymm11
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm11, %ymm11
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm11, %ymm6
vmovdqa 1632(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm10
vpunpckhwd const0(%rip), %ymm11, %ymm9
vpslld $1, %ymm10, %ymm10
vpslld $1, %ymm9, %ymm9
vpsubd %ymm10, %ymm8, %ymm8
vpsubd %ymm9, %ymm7, %ymm7
vpsrld $1, %ymm8, %ymm8
vpsrld $1, %ymm7, %ymm7
vpand mask32_to_16(%rip), %ymm8, %ymm8
vpand mask32_to_16(%rip), %ymm7, %ymm7
vpackusdw %ymm7, %ymm8, %ymm7
vmovdqa 864(%rsp), %ymm8
vpaddw 1120(%rsp), %ymm8, %ymm9
vpsubw 1120(%rsp), %ymm8, %ymm8
vpsrlw $2, %ymm8, %ymm8
vpsubw %ymm6, %ymm8, %ymm8
vpmullw %ymm14, %ymm8, %ymm8
vpsllw $1, %ymm5, %ymm10
vpsubw %ymm10, %ymm9, %ymm10
vpsllw $7, %ymm11, %ymm9
vpsubw %ymm9, %ymm10, %ymm9
vpsrlw $3, %ymm9, %ymm9
vpsubw %ymm7, %ymm9, %ymm9
vmovdqa 1376(%rsp), %ymm10
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
vpsubw %ymm8, %ymm10, %ymm10
vpsubw %ymm10, %ymm8, %ymm8
vpsubw %ymm8, %ymm6, %ymm6
vpmullw %ymm13, %ymm10, %ymm10
vpsubw %ymm10, %ymm6, %ymm6
vmovdqu 296(%rdi), %ymm4
vpaddw %ymm4, %ymm5, %ymm4
vmovdqu %ymm4, 296(%rdi)
vmovdqu 648(%rdi), %ymm4
vpaddw %ymm4, %ymm6, %ymm4
vmovdqu %ymm4, 648(%rdi)
vmovdqu 1000(%rdi), %ymm4
vpaddw %ymm4, %ymm7, %ymm4
vmovdqu %ymm4, 1000(%rdi)
vmovdqu 1352(%rdi), %ymm4
vpand mask_1_15(%rip), %ymm8, %ymm3
vpaddw %ymm4, %ymm3, %ymm4
vmovdqu %ymm4, 1352(%rdi)
vpshufb rol_rol_16(%rip), %ymm8, %ymm8
vpermq $216, %ymm8, %ymm8
vpshufb id_braid_16(%rip), %ymm8, %ymm8
vpermq $216, %ymm8, %ymm8
vpand mask_15_1(%rip), %ymm8, %ymm8
vmovdqu 0(%rdi), %ymm4
vpaddw %ymm4, %ymm8, %ymm4
vmovdqu %ymm4, 0(%rdi)
vmovdqu 350(%rdi), %ymm4
vpaddw %ymm4, %ymm9, %ymm4
vmovdqu %ymm4, 350(%rdi)
vmovdqu 702(%rdi), %ymm4
vpaddw %ymm4, %ymm10, %ymm4
vmovdqu %ymm4, 702(%rdi)
vmovdqu 1054(%rdi), %ymm4
vpaddw %ymm4, %ymm11, %ymm4
vmovdqu %ymm4, 1054(%rdi)
vmovdqa 128(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm10
vpunpckhwd const0(%rip), %ymm11, %ymm9
vpslld $1, %ymm10, %ymm10
vpslld $1, %ymm9, %ymm9
vmovdqa 384(%rsp), %ymm8
vpunpcklwd const0(%rip), %ymm8, %ymm7
vpunpckhwd const0(%rip), %ymm8, %ymm8
vmovdqa 640(%rsp), %ymm6
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
vmovdqa 1664(%rsp), %ymm5
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
vmovdqa 896(%rsp), %ymm4
vpaddw 1152(%rsp), %ymm4, %ymm7
vpsubw 1152(%rsp), %ymm4, %ymm4
vpsrlw $2, %ymm4, %ymm4
vpsubw %ymm6, %ymm4, %ymm4
vpmullw %ymm14, %ymm4, %ymm4
vpsllw $1, %ymm11, %ymm8
vpsubw %ymm8, %ymm7, %ymm8
vpsllw $7, %ymm5, %ymm7
vpsubw %ymm7, %ymm8, %ymm7
vpsrlw $3, %ymm7, %ymm7
vpsubw %ymm3, %ymm7, %ymm7
vmovdqa 1408(%rsp), %ymm8
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
vmovdqu 384(%rdi), %ymm9
vpaddw %ymm9, %ymm11, %ymm9
vmovdqu %ymm9, 384(%rdi)
vmovdqu 736(%rdi), %ymm9
vpaddw %ymm9, %ymm6, %ymm9
vmovdqu %ymm9, 736(%rdi)
vmovdqu 1088(%rdi), %ymm9
vpaddw %ymm9, %ymm3, %ymm9
vmovdqu %ymm9, 1088(%rdi)
vmovdqu 86(%rdi), %ymm9
vpaddw %ymm9, %ymm4, %ymm9
vmovdqu %ymm9, 86(%rdi)
vmovdqu 438(%rdi), %ymm9
vpaddw %ymm9, %ymm7, %ymm9
vmovdqu %ymm9, 438(%rdi)
vmovdqu 790(%rdi), %ymm9
vpaddw %ymm9, %ymm8, %ymm9
vmovdqu %ymm9, 790(%rdi)
vmovdqu 1142(%rdi), %ymm9
vpaddw %ymm9, %ymm5, %ymm9
vmovdqu %ymm9, 1142(%rdi)
vmovdqa 160(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm8
vpunpckhwd const0(%rip), %ymm5, %ymm7
vpslld $1, %ymm8, %ymm8
vpslld $1, %ymm7, %ymm7
vmovdqa 416(%rsp), %ymm4
vpunpcklwd const0(%rip), %ymm4, %ymm3
vpunpckhwd const0(%rip), %ymm4, %ymm4
vmovdqa 672(%rsp), %ymm6
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
vmovdqa 1696(%rsp), %ymm11
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
vmovdqa 928(%rsp), %ymm9
vpaddw 1184(%rsp), %ymm9, %ymm3
vpsubw 1184(%rsp), %ymm9, %ymm9
vpsrlw $2, %ymm9, %ymm9
vpsubw %ymm6, %ymm9, %ymm9
vpmullw %ymm14, %ymm9, %ymm9
vpsllw $1, %ymm5, %ymm4
vpsubw %ymm4, %ymm3, %ymm4
vpsllw $7, %ymm11, %ymm3
vpsubw %ymm3, %ymm4, %ymm3
vpsrlw $3, %ymm3, %ymm3
vpsubw %ymm10, %ymm3, %ymm3
vmovdqa 1440(%rsp), %ymm4
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
vmovdqu 472(%rdi), %ymm7
vpaddw %ymm7, %ymm5, %ymm7
vmovdqu %ymm7, 472(%rdi)
vmovdqu 824(%rdi), %ymm7
vpaddw %ymm7, %ymm6, %ymm7
vmovdqu %ymm7, 824(%rdi)
vmovdqu 1176(%rdi), %ymm7
vpaddw %ymm7, %ymm10, %ymm7
vmovdqu %ymm7, 1176(%rdi)
vmovdqu 174(%rdi), %ymm7
vpaddw %ymm7, %ymm9, %ymm7
vmovdqu %ymm7, 174(%rdi)
vmovdqu 526(%rdi), %ymm7
vpaddw %ymm7, %ymm3, %ymm7
vmovdqu %ymm7, 526(%rdi)
vmovdqu 878(%rdi), %ymm7
vpaddw %ymm7, %ymm4, %ymm7
vmovdqu %ymm7, 878(%rdi)
vmovdqu 1230(%rdi), %ymm7
vpaddw %ymm7, %ymm11, %ymm7
vmovdqu %ymm7, 1230(%rdi)
vmovdqa 192(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm4
vpunpckhwd const0(%rip), %ymm11, %ymm3
vpslld $1, %ymm4, %ymm4
vpslld $1, %ymm3, %ymm3
vmovdqa 448(%rsp), %ymm9
vpunpcklwd const0(%rip), %ymm9, %ymm10
vpunpckhwd const0(%rip), %ymm9, %ymm9
vmovdqa 704(%rsp), %ymm6
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
vmovdqa 1728(%rsp), %ymm5
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
vmovdqa 960(%rsp), %ymm7
vpaddw 1216(%rsp), %ymm7, %ymm10
vpsubw 1216(%rsp), %ymm7, %ymm7
vpsrlw $2, %ymm7, %ymm7
vpsubw %ymm6, %ymm7, %ymm7
vpmullw %ymm14, %ymm7, %ymm7
vpsllw $1, %ymm11, %ymm9
vpsubw %ymm9, %ymm10, %ymm9
vpsllw $7, %ymm5, %ymm10
vpsubw %ymm10, %ymm9, %ymm10
vpsrlw $3, %ymm10, %ymm10
vpsubw %ymm8, %ymm10, %ymm10
vmovdqa 1472(%rsp), %ymm9
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
vmovdqu 560(%rdi), %ymm3
vpaddw %ymm3, %ymm11, %ymm3
vmovdqu %ymm3, 560(%rdi)
vmovdqu 912(%rdi), %ymm3
vpaddw %ymm3, %ymm6, %ymm3
vmovdqu %ymm3, 912(%rdi)
vmovdqu 1264(%rdi), %ymm3
vpaddw %ymm3, %ymm8, %ymm3
vmovdqu %ymm3, 1264(%rdi)
vmovdqu 262(%rdi), %ymm3
vpaddw %ymm3, %ymm7, %ymm3
vmovdqu %ymm3, 262(%rdi)
vmovdqu 614(%rdi), %ymm3
vpaddw %ymm3, %ymm10, %ymm3
vmovdqu %ymm3, 614(%rdi)
vmovdqu 966(%rdi), %ymm3
vpaddw %ymm3, %ymm9, %ymm3
vmovdqu %ymm3, 966(%rdi)
vmovdqu 1318(%rdi), %ymm3
vpaddw %ymm3, %ymm5, %ymm3
vmovdqu %ymm3, 1318(%rdi)
vmovdqa 224(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm9
vpunpckhwd const0(%rip), %ymm5, %ymm10
vpslld $1, %ymm9, %ymm9
vpslld $1, %ymm10, %ymm10
vmovdqa 480(%rsp), %ymm7
vpunpcklwd const0(%rip), %ymm7, %ymm8
vpunpckhwd const0(%rip), %ymm7, %ymm7
vmovdqa 736(%rsp), %ymm6
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
vmovdqa 1760(%rsp), %ymm11
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
vmovdqa 992(%rsp), %ymm3
vpaddw 1248(%rsp), %ymm3, %ymm8
vpsubw 1248(%rsp), %ymm3, %ymm3
vpsrlw $2, %ymm3, %ymm3
vpsubw %ymm6, %ymm3, %ymm3
vpmullw %ymm14, %ymm3, %ymm3
vpsllw $1, %ymm5, %ymm7
vpsubw %ymm7, %ymm8, %ymm7
vpsllw $7, %ymm11, %ymm8
vpsubw %ymm8, %ymm7, %ymm8
vpsrlw $3, %ymm8, %ymm8
vpsubw %ymm4, %ymm8, %ymm8
vmovdqa 1504(%rsp), %ymm7
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
vmovdqu 648(%rdi), %ymm10
vpaddw %ymm10, %ymm5, %ymm10
vmovdqu %ymm10, 648(%rdi)
vmovdqu 1000(%rdi), %ymm10
vpaddw %ymm10, %ymm6, %ymm10
vmovdqu %ymm10, 1000(%rdi)
vmovdqu 1352(%rdi), %ymm10
vpand mask_1_15(%rip), %ymm4, %ymm9
vpaddw %ymm10, %ymm9, %ymm10
vmovdqu %ymm10, 1352(%rdi)
vpshufb rol_rol_16(%rip), %ymm4, %ymm4
vpermq $216, %ymm4, %ymm4
vpshufb id_braid_16(%rip), %ymm4, %ymm4
vpermq $216, %ymm4, %ymm4
vpand mask_15_1(%rip), %ymm4, %ymm4
vmovdqu 0(%rdi), %ymm10
vpaddw %ymm10, %ymm4, %ymm10
vmovdqu %ymm10, 0(%rdi)
vmovdqu 350(%rdi), %ymm10
vpaddw %ymm10, %ymm3, %ymm10
vmovdqu %ymm10, 350(%rdi)
vmovdqu 702(%rdi), %ymm10
vpaddw %ymm10, %ymm8, %ymm10
vmovdqu %ymm10, 702(%rdi)
vmovdqu 1054(%rdi), %ymm10
vpaddw %ymm10, %ymm7, %ymm10
vmovdqu %ymm10, 1054(%rdi)
vmovdqa 160(%r12), %ymm0
vpsubw 256(%r12), %ymm0, %ymm0
vmovdqa 544(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 352(%r12), %ymm1, %ymm1
vpsubw 64(%r12), %ymm0, %ymm0
vpaddw 448(%r12), %ymm0, %ymm0
vmovdqa 736(%r12), %ymm2
vpsubw 832(%r12), %ymm2, %ymm2
vmovdqa 1120(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 928(%r12), %ymm3, %ymm3
vpsubw 640(%r12), %ymm2, %ymm2
vpaddw 1024(%r12), %ymm2, %ymm2
vmovdqa 1312(%r12), %ymm4
vpsubw 1408(%r12), %ymm4, %ymm4
vmovdqa 1696(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 1504(%r12), %ymm5, %ymm5
vpsubw 1216(%r12), %ymm4, %ymm4
vpaddw 1600(%r12), %ymm4, %ymm4
vpsubw 640(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 64(%r12), %ymm1, %ymm1
vpaddw 1216(%r12), %ymm1, %ymm1
vmovdqa 352(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 1504(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 928(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 64(%r12), %ymm8
vmovdqa 928(%r12), %ymm9
vmovdqa %ymm8, 0(%rsp)
vmovdqa %ymm0, 32(%rsp)
vmovdqa %ymm1, 64(%rsp)
vmovdqa %ymm7, 96(%rsp)
vmovdqa %ymm5, 128(%rsp)
vmovdqa %ymm2, 160(%rsp)
vmovdqa %ymm3, 192(%rsp)
vmovdqa %ymm9, 224(%rsp)
vmovdqa 1888(%r12), %ymm0
vpsubw 1984(%r12), %ymm0, %ymm0
vmovdqa 2272(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 2080(%r12), %ymm1, %ymm1
vpsubw 1792(%r12), %ymm0, %ymm0
vpaddw 2176(%r12), %ymm0, %ymm0
vmovdqa 2464(%r12), %ymm2
vpsubw 2560(%r12), %ymm2, %ymm2
vmovdqa 2848(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 2656(%r12), %ymm3, %ymm3
vpsubw 2368(%r12), %ymm2, %ymm2
vpaddw 2752(%r12), %ymm2, %ymm2
vmovdqa 3040(%r12), %ymm4
vpsubw 3136(%r12), %ymm4, %ymm4
vmovdqa 3424(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 3232(%r12), %ymm5, %ymm5
vpsubw 2944(%r12), %ymm4, %ymm4
vpaddw 3328(%r12), %ymm4, %ymm4
vpsubw 2368(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 1792(%r12), %ymm1, %ymm1
vpaddw 2944(%r12), %ymm1, %ymm1
vmovdqa 2080(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 3232(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 2656(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 1792(%r12), %ymm8
vmovdqa 2656(%r12), %ymm9
vmovdqa %ymm8, 256(%rsp)
vmovdqa %ymm0, 288(%rsp)
vmovdqa %ymm1, 320(%rsp)
vmovdqa %ymm7, 352(%rsp)
vmovdqa %ymm5, 384(%rsp)
vmovdqa %ymm2, 416(%rsp)
vmovdqa %ymm3, 448(%rsp)
vmovdqa %ymm9, 480(%rsp)
vmovdqa 3616(%r12), %ymm0
vpsubw 3712(%r12), %ymm0, %ymm0
vmovdqa 4000(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 3808(%r12), %ymm1, %ymm1
vpsubw 3520(%r12), %ymm0, %ymm0
vpaddw 3904(%r12), %ymm0, %ymm0
vmovdqa 4192(%r12), %ymm2
vpsubw 4288(%r12), %ymm2, %ymm2
vmovdqa 4576(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 4384(%r12), %ymm3, %ymm3
vpsubw 4096(%r12), %ymm2, %ymm2
vpaddw 4480(%r12), %ymm2, %ymm2
vmovdqa 4768(%r12), %ymm4
vpsubw 4864(%r12), %ymm4, %ymm4
vmovdqa 5152(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 4960(%r12), %ymm5, %ymm5
vpsubw 4672(%r12), %ymm4, %ymm4
vpaddw 5056(%r12), %ymm4, %ymm4
vpsubw 4096(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 3520(%r12), %ymm1, %ymm1
vpaddw 4672(%r12), %ymm1, %ymm1
vmovdqa 3808(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 4960(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 4384(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 3520(%r12), %ymm8
vmovdqa 4384(%r12), %ymm9
vmovdqa %ymm8, 512(%rsp)
vmovdqa %ymm0, 544(%rsp)
vmovdqa %ymm1, 576(%rsp)
vmovdqa %ymm7, 608(%rsp)
vmovdqa %ymm5, 640(%rsp)
vmovdqa %ymm2, 672(%rsp)
vmovdqa %ymm3, 704(%rsp)
vmovdqa %ymm9, 736(%rsp)
vmovdqa 5344(%r12), %ymm0
vpsubw 5440(%r12), %ymm0, %ymm0
vmovdqa 5728(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5536(%r12), %ymm1, %ymm1
vpsubw 5248(%r12), %ymm0, %ymm0
vpaddw 5632(%r12), %ymm0, %ymm0
vmovdqa 5920(%r12), %ymm2
vpsubw 6016(%r12), %ymm2, %ymm2
vmovdqa 6304(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 6112(%r12), %ymm3, %ymm3
vpsubw 5824(%r12), %ymm2, %ymm2
vpaddw 6208(%r12), %ymm2, %ymm2
vmovdqa 6496(%r12), %ymm4
vpsubw 6592(%r12), %ymm4, %ymm4
vmovdqa 6880(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 6688(%r12), %ymm5, %ymm5
vpsubw 6400(%r12), %ymm4, %ymm4
vpaddw 6784(%r12), %ymm4, %ymm4
vpsubw 5824(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 5248(%r12), %ymm1, %ymm1
vpaddw 6400(%r12), %ymm1, %ymm1
vmovdqa 5536(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 6688(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 6112(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 5248(%r12), %ymm8
vmovdqa 6112(%r12), %ymm9
vmovdqa %ymm8, 768(%rsp)
vmovdqa %ymm0, 800(%rsp)
vmovdqa %ymm1, 832(%rsp)
vmovdqa %ymm7, 864(%rsp)
vmovdqa %ymm5, 896(%rsp)
vmovdqa %ymm2, 928(%rsp)
vmovdqa %ymm3, 960(%rsp)
vmovdqa %ymm9, 992(%rsp)
vmovdqa 7072(%r12), %ymm0
vpsubw 7168(%r12), %ymm0, %ymm0
vmovdqa 7456(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 7264(%r12), %ymm1, %ymm1
vpsubw 6976(%r12), %ymm0, %ymm0
vpaddw 7360(%r12), %ymm0, %ymm0
vmovdqa 7648(%r12), %ymm2
vpsubw 7744(%r12), %ymm2, %ymm2
vmovdqa 8032(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 7840(%r12), %ymm3, %ymm3
vpsubw 7552(%r12), %ymm2, %ymm2
vpaddw 7936(%r12), %ymm2, %ymm2
vmovdqa 8224(%r12), %ymm4
vpsubw 8320(%r12), %ymm4, %ymm4
vmovdqa 8608(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 8416(%r12), %ymm5, %ymm5
vpsubw 8128(%r12), %ymm4, %ymm4
vpaddw 8512(%r12), %ymm4, %ymm4
vpsubw 7552(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 6976(%r12), %ymm1, %ymm1
vpaddw 8128(%r12), %ymm1, %ymm1
vmovdqa 7264(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 8416(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 7840(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 6976(%r12), %ymm8
vmovdqa 7840(%r12), %ymm9
vmovdqa %ymm8, 1024(%rsp)
vmovdqa %ymm0, 1056(%rsp)
vmovdqa %ymm1, 1088(%rsp)
vmovdqa %ymm7, 1120(%rsp)
vmovdqa %ymm5, 1152(%rsp)
vmovdqa %ymm2, 1184(%rsp)
vmovdqa %ymm3, 1216(%rsp)
vmovdqa %ymm9, 1248(%rsp)
vmovdqa 8800(%r12), %ymm0
vpsubw 8896(%r12), %ymm0, %ymm0
vmovdqa 9184(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 8992(%r12), %ymm1, %ymm1
vpsubw 8704(%r12), %ymm0, %ymm0
vpaddw 9088(%r12), %ymm0, %ymm0
vmovdqa 9376(%r12), %ymm2
vpsubw 9472(%r12), %ymm2, %ymm2
vmovdqa 9760(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 9568(%r12), %ymm3, %ymm3
vpsubw 9280(%r12), %ymm2, %ymm2
vpaddw 9664(%r12), %ymm2, %ymm2
vmovdqa 9952(%r12), %ymm4
vpsubw 10048(%r12), %ymm4, %ymm4
vmovdqa 10336(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 10144(%r12), %ymm5, %ymm5
vpsubw 9856(%r12), %ymm4, %ymm4
vpaddw 10240(%r12), %ymm4, %ymm4
vpsubw 9280(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 8704(%r12), %ymm1, %ymm1
vpaddw 9856(%r12), %ymm1, %ymm1
vmovdqa 8992(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 10144(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 9568(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 8704(%r12), %ymm8
vmovdqa 9568(%r12), %ymm9
vmovdqa %ymm8, 1280(%rsp)
vmovdqa %ymm0, 1312(%rsp)
vmovdqa %ymm1, 1344(%rsp)
vmovdqa %ymm7, 1376(%rsp)
vmovdqa %ymm5, 1408(%rsp)
vmovdqa %ymm2, 1440(%rsp)
vmovdqa %ymm3, 1472(%rsp)
vmovdqa %ymm9, 1504(%rsp)
vmovdqa 10528(%r12), %ymm0
vpsubw 10624(%r12), %ymm0, %ymm0
vmovdqa 10912(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 10720(%r12), %ymm1, %ymm1
vpsubw 10432(%r12), %ymm0, %ymm0
vpaddw 10816(%r12), %ymm0, %ymm0
vmovdqa 11104(%r12), %ymm2
vpsubw 11200(%r12), %ymm2, %ymm2
vmovdqa 11488(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 11296(%r12), %ymm3, %ymm3
vpsubw 11008(%r12), %ymm2, %ymm2
vpaddw 11392(%r12), %ymm2, %ymm2
vmovdqa 11680(%r12), %ymm4
vpsubw 11776(%r12), %ymm4, %ymm4
vmovdqa 12064(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 11872(%r12), %ymm5, %ymm5
vpsubw 11584(%r12), %ymm4, %ymm4
vpaddw 11968(%r12), %ymm4, %ymm4
vpsubw 11008(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 10432(%r12), %ymm1, %ymm1
vpaddw 11584(%r12), %ymm1, %ymm1
vmovdqa 10720(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 11872(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 11296(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 10432(%r12), %ymm8
vmovdqa 11296(%r12), %ymm9
vmovdqa %ymm8, 1536(%rsp)
vmovdqa %ymm0, 1568(%rsp)
vmovdqa %ymm1, 1600(%rsp)
vmovdqa %ymm7, 1632(%rsp)
vmovdqa %ymm5, 1664(%rsp)
vmovdqa %ymm2, 1696(%rsp)
vmovdqa %ymm3, 1728(%rsp)
vmovdqa %ymm9, 1760(%rsp)
vmovdqa 0(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm7
vpunpckhwd const0(%rip), %ymm11, %ymm8
vpslld $1, %ymm7, %ymm7
vpslld $1, %ymm8, %ymm8
vmovdqa 256(%rsp), %ymm3
vpunpcklwd const0(%rip), %ymm3, %ymm4
vpunpckhwd const0(%rip), %ymm3, %ymm3
vmovdqa 512(%rsp), %ymm6
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
vmovdqa 1536(%rsp), %ymm5
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
vmovdqa 768(%rsp), %ymm10
vpaddw 1024(%rsp), %ymm10, %ymm4
vpsubw 1024(%rsp), %ymm10, %ymm10
vpsrlw $2, %ymm10, %ymm10
vpsubw %ymm6, %ymm10, %ymm10
vpmullw %ymm14, %ymm10, %ymm10
vpsllw $1, %ymm11, %ymm3
vpsubw %ymm3, %ymm4, %ymm3
vpsllw $7, %ymm5, %ymm4
vpsubw %ymm4, %ymm3, %ymm4
vpsrlw $3, %ymm4, %ymm4
vpsubw %ymm9, %ymm4, %ymm4
vmovdqa 1280(%rsp), %ymm3
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
vmovdqu 64(%rdi), %ymm8
vpaddw %ymm8, %ymm11, %ymm8
vmovdqu %xmm8, 64(%rdi)
vextracti128 $1, %ymm8, %xmm8
vmovq %xmm8, 80(%rdi)
vmovdqu 416(%rdi), %ymm8
vpaddw %ymm8, %ymm6, %ymm8
vmovdqu %xmm8, 416(%rdi)
vextracti128 $1, %ymm8, %xmm8
vmovq %xmm8, 432(%rdi)
vmovdqu 768(%rdi), %ymm8
vpaddw %ymm8, %ymm9, %ymm8
vmovdqu %xmm8, 768(%rdi)
vextracti128 $1, %ymm8, %xmm8
vmovq %xmm8, 784(%rdi)
vmovdqu 1120(%rdi), %ymm8
vpaddw %ymm8, %ymm10, %ymm8
vmovdqu %xmm8, 1120(%rdi)
vextracti128 $1, %ymm8, %xmm8
vmovq %xmm8, 1136(%rdi)
vmovdqu 118(%rdi), %ymm8
vpaddw %ymm8, %ymm4, %ymm8
vmovdqu %xmm8, 118(%rdi)
vextracti128 $1, %ymm8, %xmm8
vmovq %xmm8, 134(%rdi)
vmovdqu 470(%rdi), %ymm8
vpaddw %ymm8, %ymm3, %ymm8
vmovdqu %xmm8, 470(%rdi)
vextracti128 $1, %ymm8, %xmm8
vmovq %xmm8, 486(%rdi)
vmovdqu 822(%rdi), %ymm8
vpaddw %ymm8, %ymm5, %ymm8
vmovdqu %xmm8, 822(%rdi)
vextracti128 $1, %ymm8, %xmm8
vmovq %xmm8, 838(%rdi)
vmovdqa 32(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm3
vpunpckhwd const0(%rip), %ymm5, %ymm4
vpslld $1, %ymm3, %ymm3
vpslld $1, %ymm4, %ymm4
vmovdqa 288(%rsp), %ymm10
vpunpcklwd const0(%rip), %ymm10, %ymm9
vpunpckhwd const0(%rip), %ymm10, %ymm10
vmovdqa 544(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm11
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm11, %ymm9, %ymm8
vpaddd %ymm6, %ymm10, %ymm7
vpsubd %ymm3, %ymm8, %ymm8
vpsubd %ymm4, %ymm7, %ymm7
vpsubd %ymm11, %ymm9, %ymm11
vpsubd %ymm6, %ymm10, %ymm6
vpsrld $1, %ymm11, %ymm11
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm11, %ymm11
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm11, %ymm6
vmovdqa 1568(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm10
vpunpckhwd const0(%rip), %ymm11, %ymm9
vpslld $1, %ymm10, %ymm10
vpslld $1, %ymm9, %ymm9
vpsubd %ymm10, %ymm8, %ymm8
vpsubd %ymm9, %ymm7, %ymm7
vpsrld $1, %ymm8, %ymm8
vpsrld $1, %ymm7, %ymm7
vpand mask32_to_16(%rip), %ymm8, %ymm8
vpand mask32_to_16(%rip), %ymm7, %ymm7
vpackusdw %ymm7, %ymm8, %ymm7
vmovdqa 800(%rsp), %ymm8
vpaddw 1056(%rsp), %ymm8, %ymm9
vpsubw 1056(%rsp), %ymm8, %ymm8
vpsrlw $2, %ymm8, %ymm8
vpsubw %ymm6, %ymm8, %ymm8
vpmullw %ymm14, %ymm8, %ymm8
vpsllw $1, %ymm5, %ymm10
vpsubw %ymm10, %ymm9, %ymm10
vpsllw $7, %ymm11, %ymm9
vpsubw %ymm9, %ymm10, %ymm9
vpsrlw $3, %ymm9, %ymm9
vpsubw %ymm7, %ymm9, %ymm9
vmovdqa 1312(%rsp), %ymm10
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
vpsubw %ymm8, %ymm10, %ymm10
vpsubw %ymm10, %ymm8, %ymm8
vpsubw %ymm8, %ymm6, %ymm6
vpmullw %ymm13, %ymm10, %ymm10
vpsubw %ymm10, %ymm6, %ymm6
vmovdqu 152(%rdi), %ymm4
vpaddw %ymm4, %ymm5, %ymm4
vmovdqu %xmm4, 152(%rdi)
vextracti128 $1, %ymm4, %xmm4
vmovq %xmm4, 168(%rdi)
vmovdqu 504(%rdi), %ymm4
vpaddw %ymm4, %ymm6, %ymm4
vmovdqu %xmm4, 504(%rdi)
vextracti128 $1, %ymm4, %xmm4
vmovq %xmm4, 520(%rdi)
vmovdqu 856(%rdi), %ymm4
vpaddw %ymm4, %ymm7, %ymm4
vmovdqu %xmm4, 856(%rdi)
vextracti128 $1, %ymm4, %xmm4
vmovq %xmm4, 872(%rdi)
vmovdqu 1208(%rdi), %ymm4
vpaddw %ymm4, %ymm8, %ymm4
vmovdqu %xmm4, 1208(%rdi)
vextracti128 $1, %ymm4, %xmm4
vmovq %xmm4, 1224(%rdi)
vmovdqu 206(%rdi), %ymm4
vpaddw %ymm4, %ymm9, %ymm4
vmovdqu %xmm4, 206(%rdi)
vextracti128 $1, %ymm4, %xmm4
vmovq %xmm4, 222(%rdi)
vmovdqu 558(%rdi), %ymm4
vpaddw %ymm4, %ymm10, %ymm4
vmovdqu %xmm4, 558(%rdi)
vextracti128 $1, %ymm4, %xmm4
vmovq %xmm4, 574(%rdi)
vmovdqu 910(%rdi), %ymm4
vpaddw %ymm4, %ymm11, %ymm4
vmovdqu %xmm4, 910(%rdi)
vextracti128 $1, %ymm4, %xmm4
vmovq %xmm4, 926(%rdi)
vmovdqa 64(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm10
vpunpckhwd const0(%rip), %ymm11, %ymm9
vpslld $1, %ymm10, %ymm10
vpslld $1, %ymm9, %ymm9
vmovdqa 320(%rsp), %ymm8
vpunpcklwd const0(%rip), %ymm8, %ymm7
vpunpckhwd const0(%rip), %ymm8, %ymm8
vmovdqa 576(%rsp), %ymm6
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
vmovdqa 1600(%rsp), %ymm5
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
vmovdqa 832(%rsp), %ymm4
vpaddw 1088(%rsp), %ymm4, %ymm7
vpsubw 1088(%rsp), %ymm4, %ymm4
vpsrlw $2, %ymm4, %ymm4
vpsubw %ymm6, %ymm4, %ymm4
vpmullw %ymm14, %ymm4, %ymm4
vpsllw $1, %ymm11, %ymm8
vpsubw %ymm8, %ymm7, %ymm8
vpsllw $7, %ymm5, %ymm7
vpsubw %ymm7, %ymm8, %ymm7
vpsrlw $3, %ymm7, %ymm7
vpsubw %ymm3, %ymm7, %ymm7
vmovdqa 1344(%rsp), %ymm8
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
vmovdqu 240(%rdi), %ymm9
vpaddw %ymm9, %ymm11, %ymm9
vmovdqu %xmm9, 240(%rdi)
vextracti128 $1, %ymm9, %xmm9
vmovq %xmm9, 256(%rdi)
vmovdqu 592(%rdi), %ymm9
vpaddw %ymm9, %ymm6, %ymm9
vmovdqu %xmm9, 592(%rdi)
vextracti128 $1, %ymm9, %xmm9
vmovq %xmm9, 608(%rdi)
vmovdqu 944(%rdi), %ymm9
vpaddw %ymm9, %ymm3, %ymm9
vmovdqu %xmm9, 944(%rdi)
vextracti128 $1, %ymm9, %xmm9
vmovq %xmm9, 960(%rdi)
vmovdqu 1296(%rdi), %ymm9
vpaddw %ymm9, %ymm4, %ymm9
vmovdqu %xmm9, 1296(%rdi)
vextracti128 $1, %ymm9, %xmm9
vmovq %xmm9, 1312(%rdi)
vmovdqu 294(%rdi), %ymm9
vpaddw %ymm9, %ymm7, %ymm9
vmovdqu %xmm9, 294(%rdi)
vextracti128 $1, %ymm9, %xmm9
vmovq %xmm9, 310(%rdi)
vmovdqu 646(%rdi), %ymm9
vpaddw %ymm9, %ymm8, %ymm9
vmovdqu %xmm9, 646(%rdi)
vextracti128 $1, %ymm9, %xmm9
vmovq %xmm9, 662(%rdi)
vmovdqu 998(%rdi), %ymm9
vpaddw %ymm9, %ymm5, %ymm9
vmovdqu %xmm9, 998(%rdi)
vextracti128 $1, %ymm9, %xmm9
vmovq %xmm9, 1014(%rdi)
vmovdqa 96(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm8
vpunpckhwd const0(%rip), %ymm5, %ymm7
vpslld $1, %ymm8, %ymm8
vpslld $1, %ymm7, %ymm7
vmovdqa 352(%rsp), %ymm4
vpunpcklwd const0(%rip), %ymm4, %ymm3
vpunpckhwd const0(%rip), %ymm4, %ymm4
vmovdqa 608(%rsp), %ymm6
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
vmovdqa 1632(%rsp), %ymm11
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
vmovdqa 864(%rsp), %ymm9
vpaddw 1120(%rsp), %ymm9, %ymm3
vpsubw 1120(%rsp), %ymm9, %ymm9
vpsrlw $2, %ymm9, %ymm9
vpsubw %ymm6, %ymm9, %ymm9
vpmullw %ymm14, %ymm9, %ymm9
vpsllw $1, %ymm5, %ymm4
vpsubw %ymm4, %ymm3, %ymm4
vpsllw $7, %ymm11, %ymm3
vpsubw %ymm3, %ymm4, %ymm3
vpsrlw $3, %ymm3, %ymm3
vpsubw %ymm10, %ymm3, %ymm3
vmovdqa 1376(%rsp), %ymm4
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
vmovdqu 328(%rdi), %ymm7
vpaddw %ymm7, %ymm5, %ymm7
vmovdqu %xmm7, 328(%rdi)
vextracti128 $1, %ymm7, %xmm7
vmovq %xmm7, 344(%rdi)
vmovdqu 680(%rdi), %ymm7
vpaddw %ymm7, %ymm6, %ymm7
vmovdqu %xmm7, 680(%rdi)
vextracti128 $1, %ymm7, %xmm7
vmovq %xmm7, 696(%rdi)
vmovdqu 1032(%rdi), %ymm7
vpaddw %ymm7, %ymm10, %ymm7
vmovdqu %xmm7, 1032(%rdi)
vextracti128 $1, %ymm7, %xmm7
vmovq %xmm7, 1048(%rdi)
vmovdqu 30(%rdi), %ymm7
vpaddw %ymm7, %ymm9, %ymm7
vmovdqu %xmm7, 30(%rdi)
vextracti128 $1, %ymm7, %xmm7
vmovq %xmm7, 46(%rdi)
vmovdqu 382(%rdi), %ymm7
vpaddw %ymm7, %ymm3, %ymm7
vmovdqu %xmm7, 382(%rdi)
vextracti128 $1, %ymm7, %xmm7
vmovq %xmm7, 398(%rdi)
vmovdqu 734(%rdi), %ymm7
vpaddw %ymm7, %ymm4, %ymm7
vmovdqu %xmm7, 734(%rdi)
vextracti128 $1, %ymm7, %xmm7
vmovq %xmm7, 750(%rdi)
vmovdqu 1086(%rdi), %ymm7
vpaddw %ymm7, %ymm11, %ymm7
vmovdqu %xmm7, 1086(%rdi)
vextracti128 $1, %ymm7, %xmm7
vmovq %xmm7, 1102(%rdi)
vmovdqa 128(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm4
vpunpckhwd const0(%rip), %ymm11, %ymm3
vpslld $1, %ymm4, %ymm4
vpslld $1, %ymm3, %ymm3
vmovdqa 384(%rsp), %ymm9
vpunpcklwd const0(%rip), %ymm9, %ymm10
vpunpckhwd const0(%rip), %ymm9, %ymm9
vmovdqa 640(%rsp), %ymm6
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
vmovdqa 1664(%rsp), %ymm5
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
vmovdqa 896(%rsp), %ymm7
vpaddw 1152(%rsp), %ymm7, %ymm10
vpsubw 1152(%rsp), %ymm7, %ymm7
vpsrlw $2, %ymm7, %ymm7
vpsubw %ymm6, %ymm7, %ymm7
vpmullw %ymm14, %ymm7, %ymm7
vpsllw $1, %ymm11, %ymm9
vpsubw %ymm9, %ymm10, %ymm9
vpsllw $7, %ymm5, %ymm10
vpsubw %ymm10, %ymm9, %ymm10
vpsrlw $3, %ymm10, %ymm10
vpsubw %ymm8, %ymm10, %ymm10
vmovdqa 1408(%rsp), %ymm9
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
vmovdqu 416(%rdi), %ymm3
vpaddw %ymm3, %ymm11, %ymm3
vmovdqu %xmm3, 416(%rdi)
vextracti128 $1, %ymm3, %xmm3
vmovq %xmm3, 432(%rdi)
vmovdqu 768(%rdi), %ymm3
vpaddw %ymm3, %ymm6, %ymm3
vmovdqu %xmm3, 768(%rdi)
vextracti128 $1, %ymm3, %xmm3
vmovq %xmm3, 784(%rdi)
vmovdqu 1120(%rdi), %ymm3
vpaddw %ymm3, %ymm8, %ymm3
vmovdqu %xmm3, 1120(%rdi)
vextracti128 $1, %ymm3, %xmm3
vmovq %xmm3, 1136(%rdi)
vmovdqu 118(%rdi), %ymm3
vpaddw %ymm3, %ymm7, %ymm3
vmovdqu %xmm3, 118(%rdi)
vextracti128 $1, %ymm3, %xmm3
vmovq %xmm3, 134(%rdi)
vmovdqu 470(%rdi), %ymm3
vpaddw %ymm3, %ymm10, %ymm3
vmovdqu %xmm3, 470(%rdi)
vextracti128 $1, %ymm3, %xmm3
vmovq %xmm3, 486(%rdi)
vmovdqu 822(%rdi), %ymm3
vpaddw %ymm3, %ymm9, %ymm3
vmovdqu %xmm3, 822(%rdi)
vextracti128 $1, %ymm3, %xmm3
vmovq %xmm3, 838(%rdi)
vmovdqu 1174(%rdi), %ymm3
vpaddw %ymm3, %ymm5, %ymm3
vmovdqu %xmm3, 1174(%rdi)
vextracti128 $1, %ymm3, %xmm3
vmovq %xmm3, 1190(%rdi)
vmovdqa 160(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm9
vpunpckhwd const0(%rip), %ymm5, %ymm10
vpslld $1, %ymm9, %ymm9
vpslld $1, %ymm10, %ymm10
vmovdqa 416(%rsp), %ymm7
vpunpcklwd const0(%rip), %ymm7, %ymm8
vpunpckhwd const0(%rip), %ymm7, %ymm7
vmovdqa 672(%rsp), %ymm6
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
vmovdqa 1696(%rsp), %ymm11
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
vmovdqa 928(%rsp), %ymm3
vpaddw 1184(%rsp), %ymm3, %ymm8
vpsubw 1184(%rsp), %ymm3, %ymm3
vpsrlw $2, %ymm3, %ymm3
vpsubw %ymm6, %ymm3, %ymm3
vpmullw %ymm14, %ymm3, %ymm3
vpsllw $1, %ymm5, %ymm7
vpsubw %ymm7, %ymm8, %ymm7
vpsllw $7, %ymm11, %ymm8
vpsubw %ymm8, %ymm7, %ymm8
vpsrlw $3, %ymm8, %ymm8
vpsubw %ymm4, %ymm8, %ymm8
vmovdqa 1440(%rsp), %ymm7
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
vmovdqu 504(%rdi), %ymm10
vpaddw %ymm10, %ymm5, %ymm10
vmovdqu %xmm10, 504(%rdi)
vextracti128 $1, %ymm10, %xmm10
vmovq %xmm10, 520(%rdi)
vmovdqu 856(%rdi), %ymm10
vpaddw %ymm10, %ymm6, %ymm10
vmovdqu %xmm10, 856(%rdi)
vextracti128 $1, %ymm10, %xmm10
vmovq %xmm10, 872(%rdi)
vmovdqu 1208(%rdi), %ymm10
vpaddw %ymm10, %ymm4, %ymm10
vmovdqu %xmm10, 1208(%rdi)
vextracti128 $1, %ymm10, %xmm10
vmovq %xmm10, 1224(%rdi)
vmovdqu 206(%rdi), %ymm10
vpaddw %ymm10, %ymm3, %ymm10
vmovdqu %xmm10, 206(%rdi)
vextracti128 $1, %ymm10, %xmm10
vmovq %xmm10, 222(%rdi)
vmovdqu 558(%rdi), %ymm10
vpaddw %ymm10, %ymm8, %ymm10
vmovdqu %xmm10, 558(%rdi)
vextracti128 $1, %ymm10, %xmm10
vmovq %xmm10, 574(%rdi)
vmovdqu 910(%rdi), %ymm10
vpaddw %ymm10, %ymm7, %ymm10
vmovdqu %xmm10, 910(%rdi)
vextracti128 $1, %ymm10, %xmm10
vmovq %xmm10, 926(%rdi)
vmovdqu 1262(%rdi), %ymm10
vpaddw %ymm10, %ymm11, %ymm10
vmovdqu %xmm10, 1262(%rdi)
vextracti128 $1, %ymm10, %xmm10
vmovq %xmm10, 1278(%rdi)
vmovdqa 192(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm7
vpunpckhwd const0(%rip), %ymm11, %ymm8
vpslld $1, %ymm7, %ymm7
vpslld $1, %ymm8, %ymm8
vmovdqa 448(%rsp), %ymm3
vpunpcklwd const0(%rip), %ymm3, %ymm4
vpunpckhwd const0(%rip), %ymm3, %ymm3
vmovdqa 704(%rsp), %ymm6
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
vmovdqa 1728(%rsp), %ymm5
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
vmovdqa 960(%rsp), %ymm10
vpaddw 1216(%rsp), %ymm10, %ymm4
vpsubw 1216(%rsp), %ymm10, %ymm10
vpsrlw $2, %ymm10, %ymm10
vpsubw %ymm6, %ymm10, %ymm10
vpmullw %ymm14, %ymm10, %ymm10
vpsllw $1, %ymm11, %ymm3
vpsubw %ymm3, %ymm4, %ymm3
vpsllw $7, %ymm5, %ymm4
vpsubw %ymm4, %ymm3, %ymm4
vpsrlw $3, %ymm4, %ymm4
vpsubw %ymm9, %ymm4, %ymm4
vmovdqa 1472(%rsp), %ymm3
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
vmovdqu 592(%rdi), %ymm8
vpaddw %ymm8, %ymm11, %ymm8
vmovdqu %xmm8, 592(%rdi)
vextracti128 $1, %ymm8, %xmm8
vmovq %xmm8, 608(%rdi)
vmovdqu 944(%rdi), %ymm8
vpaddw %ymm8, %ymm6, %ymm8
vmovdqu %xmm8, 944(%rdi)
vextracti128 $1, %ymm8, %xmm8
vmovq %xmm8, 960(%rdi)
vmovdqu 1296(%rdi), %ymm8
vpaddw %ymm8, %ymm9, %ymm8
vmovdqu %xmm8, 1296(%rdi)
vextracti128 $1, %ymm8, %xmm8
vmovq %xmm8, 1312(%rdi)
vmovdqu 294(%rdi), %ymm8
vpaddw %ymm8, %ymm10, %ymm8
vmovdqu %xmm8, 294(%rdi)
vextracti128 $1, %ymm8, %xmm8
vmovq %xmm8, 310(%rdi)
vmovdqu 646(%rdi), %ymm8
vpaddw %ymm8, %ymm4, %ymm8
vmovdqu %xmm8, 646(%rdi)
vextracti128 $1, %ymm8, %xmm8
vmovq %xmm8, 662(%rdi)
vmovdqu 998(%rdi), %ymm8
vpaddw %ymm8, %ymm3, %ymm8
vmovdqu %xmm8, 998(%rdi)
vextracti128 $1, %ymm8, %xmm8
vmovq %xmm8, 1014(%rdi)
vmovdqu 1350(%rdi), %ymm8
vpaddw %ymm8, %ymm5, %ymm8
vmovdqu %xmm8, 1350(%rdi)
vextracti128 $1, %ymm8, %xmm8
vmovq %xmm8, 1366(%rdi)
vmovdqa 224(%rsp), %ymm5
vpunpcklwd const0(%rip), %ymm5, %ymm3
vpunpckhwd const0(%rip), %ymm5, %ymm4
vpslld $1, %ymm3, %ymm3
vpslld $1, %ymm4, %ymm4
vmovdqa 480(%rsp), %ymm10
vpunpcklwd const0(%rip), %ymm10, %ymm9
vpunpckhwd const0(%rip), %ymm10, %ymm10
vmovdqa 736(%rsp), %ymm6
vpunpcklwd const0(%rip), %ymm6, %ymm11
vpunpckhwd const0(%rip), %ymm6, %ymm6
vpaddd %ymm11, %ymm9, %ymm8
vpaddd %ymm6, %ymm10, %ymm7
vpsubd %ymm3, %ymm8, %ymm8
vpsubd %ymm4, %ymm7, %ymm7
vpsubd %ymm11, %ymm9, %ymm11
vpsubd %ymm6, %ymm10, %ymm6
vpsrld $1, %ymm11, %ymm11
vpsrld $1, %ymm6, %ymm6
vpand mask32_to_16(%rip), %ymm11, %ymm11
vpand mask32_to_16(%rip), %ymm6, %ymm6
vpackusdw %ymm6, %ymm11, %ymm6
vmovdqa 1760(%rsp), %ymm11
vpunpcklwd const0(%rip), %ymm11, %ymm10
vpunpckhwd const0(%rip), %ymm11, %ymm9
vpslld $1, %ymm10, %ymm10
vpslld $1, %ymm9, %ymm9
vpsubd %ymm10, %ymm8, %ymm8
vpsubd %ymm9, %ymm7, %ymm7
vpsrld $1, %ymm8, %ymm8
vpsrld $1, %ymm7, %ymm7
vpand mask32_to_16(%rip), %ymm8, %ymm8
vpand mask32_to_16(%rip), %ymm7, %ymm7
vpackusdw %ymm7, %ymm8, %ymm7
vmovdqa 992(%rsp), %ymm8
vpaddw 1248(%rsp), %ymm8, %ymm9
vpsubw 1248(%rsp), %ymm8, %ymm8
vpsrlw $2, %ymm8, %ymm8
vpsubw %ymm6, %ymm8, %ymm8
vpmullw %ymm14, %ymm8, %ymm8
vpsllw $1, %ymm5, %ymm10
vpsubw %ymm10, %ymm9, %ymm10
vpsllw $7, %ymm11, %ymm9
vpsubw %ymm9, %ymm10, %ymm9
vpsrlw $3, %ymm9, %ymm9
vpsubw %ymm7, %ymm9, %ymm9
vmovdqa 1504(%rsp), %ymm10
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
vpsubw %ymm8, %ymm10, %ymm10
vpsubw %ymm10, %ymm8, %ymm8
vpsubw %ymm8, %ymm6, %ymm6
vpmullw %ymm13, %ymm10, %ymm10
vpsubw %ymm10, %ymm6, %ymm6
vmovdqu 680(%rdi), %ymm4
vpaddw %ymm4, %ymm5, %ymm4
vmovdqu %xmm4, 680(%rdi)
vextracti128 $1, %ymm4, %xmm4
vmovq %xmm4, 696(%rdi)
vmovdqu 1032(%rdi), %ymm4
vpaddw %ymm4, %ymm6, %ymm4
vmovdqu %xmm4, 1032(%rdi)
vextracti128 $1, %ymm4, %xmm4
vmovq %xmm4, 1048(%rdi)
vmovdqu 30(%rdi), %ymm4
vpaddw %ymm4, %ymm7, %ymm4
vmovdqu %xmm4, 30(%rdi)
vextracti128 $1, %ymm4, %xmm4
vmovq %xmm4, 46(%rdi)
vmovdqu 382(%rdi), %ymm4
vpaddw %ymm4, %ymm8, %ymm4
vmovdqu %xmm4, 382(%rdi)
vextracti128 $1, %ymm4, %xmm4
vmovq %xmm4, 398(%rdi)
vmovdqu 734(%rdi), %ymm4
vpaddw %ymm4, %ymm9, %ymm4
vmovdqu %xmm4, 734(%rdi)
vextracti128 $1, %ymm4, %xmm4
vmovq %xmm4, 750(%rdi)
vmovdqu 1086(%rdi), %ymm4
vpaddw %ymm4, %ymm10, %ymm4
vmovdqu %xmm4, 1086(%rdi)
vextracti128 $1, %ymm4, %xmm4
vmovq %xmm4, 1102(%rdi)
mov %r8, %rsp
pop %r12
ret
