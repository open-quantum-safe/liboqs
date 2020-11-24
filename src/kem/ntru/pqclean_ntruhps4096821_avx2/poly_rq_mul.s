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
mask_9_7:
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
.word 0
.word 0
.word 0
mask_7_9:
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
.word 0
.text
.global PQCLEAN_NTRUHPS4096821_AVX2_poly_Rq_mul
.global _PQCLEAN_NTRUHPS4096821_AVX2_poly_Rq_mul
PQCLEAN_NTRUHPS4096821_AVX2_poly_Rq_mul:
_PQCLEAN_NTRUHPS4096821_AVX2_poly_Rq_mul:
push %r12
mov %rsp, %r8
andq $-32, %rsp
subq $8192, %rsp
mov %rsp, %rax
subq $8192, %rsp
mov %rsp, %r11
subq $16384, %rsp
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
vmovdqa %ymm3, 1408(%rdi)
vmovdqa %ymm3, 1440(%rdi)
vmovdqa %ymm3, 1472(%rdi)
vmovdqa %ymm3, 1504(%rdi)
vmovdqa %ymm3, 1536(%rdi)
vmovdqa %ymm3, 1568(%rdi)
vmovdqa %ymm3, 1600(%rdi)
vmovdqa %ymm3, 1632(%rdi)
vmovdqa const3(%rip), %ymm3
vmovdqu 0(%rsi), %ymm0
vmovdqu 104(%rsi), %ymm1
vmovdqu 208(%rsi), %ymm2
vmovdqu 312(%rsi), %ymm12
vmovdqu 1248(%rsi), %ymm4
vmovdqu 1352(%rsi), %ymm5
vmovdqu 1456(%rsi), %ymm6
vmovdqu 1560(%rsi), %ymm7
vmovdqu 416(%rsi), %ymm8
vmovdqu 520(%rsi), %ymm9
vmovdqu 624(%rsi), %ymm10
vmovdqu 728(%rsi), %ymm11
vmovdqa %ymm0, 0(%rax)
vmovdqa %ymm1, 128(%rax)
vpaddw %ymm0, %ymm1, %ymm14
vmovdqa %ymm14, 256(%rax)
vmovdqa %ymm2, 384(%rax)
vmovdqa %ymm12, 512(%rax)
vpaddw %ymm2, %ymm12, %ymm14
vmovdqa %ymm14, 640(%rax)
vpaddw %ymm0, %ymm2, %ymm14
vmovdqa %ymm14, 768(%rax)
vpaddw %ymm1, %ymm12, %ymm15
vmovdqa %ymm15, 896(%rax)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 1024(%rax)
vmovdqa %ymm4, 6912(%rax)
vmovdqa %ymm5, 7040(%rax)
vpaddw %ymm4, %ymm5, %ymm14
vmovdqa %ymm14, 7168(%rax)
vmovdqa %ymm6, 7296(%rax)
vmovdqa %ymm7, 7424(%rax)
vpaddw %ymm6, %ymm7, %ymm14
vmovdqa %ymm14, 7552(%rax)
vpaddw %ymm4, %ymm6, %ymm14
vmovdqa %ymm14, 7680(%rax)
vpaddw %ymm5, %ymm7, %ymm15
vmovdqa %ymm15, 7808(%rax)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7936(%rax)
vmovdqa %ymm0, 0(%rsp)
vmovdqa %ymm1, 32(%rsp)
vmovdqa %ymm2, 64(%rsp)
vmovdqa %ymm12, 96(%rsp)
vmovdqa %ymm8, 128(%rsp)
vmovdqa %ymm9, 160(%rsp)
vmovdqa %ymm10, 192(%rsp)
vmovdqa %ymm11, 224(%rsp)
vmovdqu 832(%rsi), %ymm0
vpaddw 0(%rsp), %ymm0, %ymm1
vpaddw 128(%rsp), %ymm4, %ymm2
vpaddw %ymm2, %ymm1, %ymm8
vpsubw %ymm2, %ymm1, %ymm12
vmovdqa %ymm0, 256(%rsp)
vmovdqu 936(%rsi), %ymm0
vpaddw 32(%rsp), %ymm0, %ymm1
vpaddw 160(%rsp), %ymm5, %ymm2
vpaddw %ymm2, %ymm1, %ymm9
vpsubw %ymm2, %ymm1, %ymm13
vmovdqa %ymm0, 288(%rsp)
vmovdqu 1040(%rsi), %ymm0
vpaddw 64(%rsp), %ymm0, %ymm1
vpaddw 192(%rsp), %ymm6, %ymm2
vpaddw %ymm2, %ymm1, %ymm10
vpsubw %ymm2, %ymm1, %ymm14
vmovdqa %ymm0, 320(%rsp)
vmovdqu 1144(%rsi), %ymm0
vpaddw 96(%rsp), %ymm0, %ymm1
vpaddw 224(%rsp), %ymm7, %ymm2
vpaddw %ymm2, %ymm1, %ymm11
vpsubw %ymm2, %ymm1, %ymm15
vmovdqa %ymm0, 352(%rsp)
vmovdqa %ymm8, 1152(%rax)
vmovdqa %ymm9, 1280(%rax)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 1408(%rax)
vmovdqa %ymm10, 1536(%rax)
vmovdqa %ymm11, 1664(%rax)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 1792(%rax)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 1920(%rax)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 2048(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2176(%rax)
vmovdqa %ymm12, 2304(%rax)
vmovdqa %ymm13, 2432(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 2560(%rax)
vmovdqa %ymm14, 2688(%rax)
vmovdqa %ymm15, 2816(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 2944(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 3072(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 3200(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 3328(%rax)
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
vmovdqa %ymm8, 3456(%rax)
vmovdqa %ymm9, 3584(%rax)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 3712(%rax)
vmovdqa %ymm10, 3840(%rax)
vmovdqa %ymm11, 3968(%rax)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 4096(%rax)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 4224(%rax)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 4352(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 4480(%rax)
vmovdqa %ymm12, 4608(%rax)
vmovdqa %ymm13, 4736(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 4864(%rax)
vmovdqa %ymm14, 4992(%rax)
vmovdqa %ymm15, 5120(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 5248(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 5376(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 5504(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 5632(%rax)
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
vmovdqa %ymm12, 5760(%rax)
vmovdqa %ymm13, 5888(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 6016(%rax)
vmovdqa %ymm14, 6144(%rax)
vmovdqa %ymm15, 6272(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 6400(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 6528(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 6656(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 6784(%rax)
vmovdqu 32(%rsi), %ymm0
vmovdqu 136(%rsi), %ymm1
vmovdqu 240(%rsi), %ymm2
vmovdqu 344(%rsi), %ymm12
vmovdqu 1280(%rsi), %ymm4
vmovdqu 1384(%rsi), %ymm5
vmovdqu 1488(%rsi), %ymm6
vmovdqu 1592(%rsi), %ymm7
vmovdqu 448(%rsi), %ymm8
vmovdqu 552(%rsi), %ymm9
vmovdqu 656(%rsi), %ymm10
vmovdqu 760(%rsi), %ymm11
vmovdqa %ymm0, 32(%rax)
vmovdqa %ymm1, 160(%rax)
vpaddw %ymm0, %ymm1, %ymm14
vmovdqa %ymm14, 288(%rax)
vmovdqa %ymm2, 416(%rax)
vmovdqa %ymm12, 544(%rax)
vpaddw %ymm2, %ymm12, %ymm14
vmovdqa %ymm14, 672(%rax)
vpaddw %ymm0, %ymm2, %ymm14
vmovdqa %ymm14, 800(%rax)
vpaddw %ymm1, %ymm12, %ymm15
vmovdqa %ymm15, 928(%rax)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 1056(%rax)
vmovdqa %ymm4, 6944(%rax)
vmovdqa %ymm5, 7072(%rax)
vpaddw %ymm4, %ymm5, %ymm14
vmovdqa %ymm14, 7200(%rax)
vmovdqa %ymm6, 7328(%rax)
vmovdqa %ymm7, 7456(%rax)
vpaddw %ymm6, %ymm7, %ymm14
vmovdqa %ymm14, 7584(%rax)
vpaddw %ymm4, %ymm6, %ymm14
vmovdqa %ymm14, 7712(%rax)
vpaddw %ymm5, %ymm7, %ymm15
vmovdqa %ymm15, 7840(%rax)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7968(%rax)
vmovdqa %ymm0, 0(%rsp)
vmovdqa %ymm1, 32(%rsp)
vmovdqa %ymm2, 64(%rsp)
vmovdqa %ymm12, 96(%rsp)
vmovdqa %ymm8, 128(%rsp)
vmovdqa %ymm9, 160(%rsp)
vmovdqa %ymm10, 192(%rsp)
vmovdqa %ymm11, 224(%rsp)
vmovdqu 864(%rsi), %ymm0
vpaddw 0(%rsp), %ymm0, %ymm1
vpaddw 128(%rsp), %ymm4, %ymm2
vpaddw %ymm2, %ymm1, %ymm8
vpsubw %ymm2, %ymm1, %ymm12
vmovdqa %ymm0, 256(%rsp)
vmovdqu 968(%rsi), %ymm0
vpaddw 32(%rsp), %ymm0, %ymm1
vpaddw 160(%rsp), %ymm5, %ymm2
vpaddw %ymm2, %ymm1, %ymm9
vpsubw %ymm2, %ymm1, %ymm13
vmovdqa %ymm0, 288(%rsp)
vmovdqu 1072(%rsi), %ymm0
vpaddw 64(%rsp), %ymm0, %ymm1
vpaddw 192(%rsp), %ymm6, %ymm2
vpaddw %ymm2, %ymm1, %ymm10
vpsubw %ymm2, %ymm1, %ymm14
vmovdqa %ymm0, 320(%rsp)
vmovdqu 1176(%rsi), %ymm0
vpaddw 96(%rsp), %ymm0, %ymm1
vpaddw 224(%rsp), %ymm7, %ymm2
vpaddw %ymm2, %ymm1, %ymm11
vpsubw %ymm2, %ymm1, %ymm15
vmovdqa %ymm0, 352(%rsp)
vmovdqa %ymm8, 1184(%rax)
vmovdqa %ymm9, 1312(%rax)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 1440(%rax)
vmovdqa %ymm10, 1568(%rax)
vmovdqa %ymm11, 1696(%rax)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 1824(%rax)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 1952(%rax)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 2080(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2208(%rax)
vmovdqa %ymm12, 2336(%rax)
vmovdqa %ymm13, 2464(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 2592(%rax)
vmovdqa %ymm14, 2720(%rax)
vmovdqa %ymm15, 2848(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 2976(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 3104(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 3232(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 3360(%rax)
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
vmovdqa %ymm8, 3488(%rax)
vmovdqa %ymm9, 3616(%rax)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 3744(%rax)
vmovdqa %ymm10, 3872(%rax)
vmovdqa %ymm11, 4000(%rax)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 4128(%rax)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 4256(%rax)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 4384(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 4512(%rax)
vmovdqa %ymm12, 4640(%rax)
vmovdqa %ymm13, 4768(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 4896(%rax)
vmovdqa %ymm14, 5024(%rax)
vmovdqa %ymm15, 5152(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 5280(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 5408(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 5536(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 5664(%rax)
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
vmovdqa %ymm12, 5792(%rax)
vmovdqa %ymm13, 5920(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 6048(%rax)
vmovdqa %ymm14, 6176(%rax)
vmovdqa %ymm15, 6304(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 6432(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 6560(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 6688(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 6816(%rax)
vmovdqu 64(%rsi), %ymm0
vmovdqu 168(%rsi), %ymm1
vmovdqu 272(%rsi), %ymm2
vmovdqu 376(%rsi), %ymm12
vmovdqu 1312(%rsi), %ymm4
vmovdqu 1416(%rsi), %ymm5
vmovdqu 1520(%rsi), %ymm6
vmovdqu 1624(%rsi), %ymm7
vpand mask_9_7(%rip), %ymm7, %ymm7
vmovdqu 480(%rsi), %ymm8
vmovdqu 584(%rsi), %ymm9
vmovdqu 688(%rsi), %ymm10
vmovdqu 792(%rsi), %ymm11
vmovdqa %ymm0, 64(%rax)
vmovdqa %ymm1, 192(%rax)
vpaddw %ymm0, %ymm1, %ymm14
vmovdqa %ymm14, 320(%rax)
vmovdqa %ymm2, 448(%rax)
vmovdqa %ymm12, 576(%rax)
vpaddw %ymm2, %ymm12, %ymm14
vmovdqa %ymm14, 704(%rax)
vpaddw %ymm0, %ymm2, %ymm14
vmovdqa %ymm14, 832(%rax)
vpaddw %ymm1, %ymm12, %ymm15
vmovdqa %ymm15, 960(%rax)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 1088(%rax)
vmovdqa %ymm4, 6976(%rax)
vmovdqa %ymm5, 7104(%rax)
vpaddw %ymm4, %ymm5, %ymm14
vmovdqa %ymm14, 7232(%rax)
vmovdqa %ymm6, 7360(%rax)
vmovdqa %ymm7, 7488(%rax)
vpaddw %ymm6, %ymm7, %ymm14
vmovdqa %ymm14, 7616(%rax)
vpaddw %ymm4, %ymm6, %ymm14
vmovdqa %ymm14, 7744(%rax)
vpaddw %ymm5, %ymm7, %ymm15
vmovdqa %ymm15, 7872(%rax)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8000(%rax)
vmovdqa %ymm0, 0(%rsp)
vmovdqa %ymm1, 32(%rsp)
vmovdqa %ymm2, 64(%rsp)
vmovdqa %ymm12, 96(%rsp)
vmovdqa %ymm8, 128(%rsp)
vmovdqa %ymm9, 160(%rsp)
vmovdqa %ymm10, 192(%rsp)
vmovdqa %ymm11, 224(%rsp)
vmovdqu 896(%rsi), %ymm0
vpaddw 0(%rsp), %ymm0, %ymm1
vpaddw 128(%rsp), %ymm4, %ymm2
vpaddw %ymm2, %ymm1, %ymm8
vpsubw %ymm2, %ymm1, %ymm12
vmovdqa %ymm0, 256(%rsp)
vmovdqu 1000(%rsi), %ymm0
vpaddw 32(%rsp), %ymm0, %ymm1
vpaddw 160(%rsp), %ymm5, %ymm2
vpaddw %ymm2, %ymm1, %ymm9
vpsubw %ymm2, %ymm1, %ymm13
vmovdqa %ymm0, 288(%rsp)
vmovdqu 1104(%rsi), %ymm0
vpaddw 64(%rsp), %ymm0, %ymm1
vpaddw 192(%rsp), %ymm6, %ymm2
vpaddw %ymm2, %ymm1, %ymm10
vpsubw %ymm2, %ymm1, %ymm14
vmovdqa %ymm0, 320(%rsp)
vmovdqu 1208(%rsi), %ymm0
vpaddw 96(%rsp), %ymm0, %ymm1
vpaddw 224(%rsp), %ymm7, %ymm2
vpaddw %ymm2, %ymm1, %ymm11
vpsubw %ymm2, %ymm1, %ymm15
vmovdqa %ymm0, 352(%rsp)
vmovdqa %ymm8, 1216(%rax)
vmovdqa %ymm9, 1344(%rax)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 1472(%rax)
vmovdqa %ymm10, 1600(%rax)
vmovdqa %ymm11, 1728(%rax)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 1856(%rax)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 1984(%rax)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 2112(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2240(%rax)
vmovdqa %ymm12, 2368(%rax)
vmovdqa %ymm13, 2496(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 2624(%rax)
vmovdqa %ymm14, 2752(%rax)
vmovdqa %ymm15, 2880(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 3008(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 3136(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 3264(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 3392(%rax)
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
vmovdqa %ymm8, 3520(%rax)
vmovdqa %ymm9, 3648(%rax)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 3776(%rax)
vmovdqa %ymm10, 3904(%rax)
vmovdqa %ymm11, 4032(%rax)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 4160(%rax)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 4288(%rax)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 4416(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 4544(%rax)
vmovdqa %ymm12, 4672(%rax)
vmovdqa %ymm13, 4800(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 4928(%rax)
vmovdqa %ymm14, 5056(%rax)
vmovdqa %ymm15, 5184(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 5312(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 5440(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 5568(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 5696(%rax)
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
vmovdqa %ymm12, 5824(%rax)
vmovdqa %ymm13, 5952(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 6080(%rax)
vmovdqa %ymm14, 6208(%rax)
vmovdqa %ymm15, 6336(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 6464(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 6592(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 6720(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 6848(%rax)
vmovdqu 96(%rsi), %ymm0
vmovdqu 200(%rsi), %ymm1
vmovdqu 304(%rsi), %ymm2
vmovdqu 408(%rsi), %ymm12
vmovdqu 1344(%rsi), %ymm4
vmovdqu 1448(%rsi), %ymm5
vmovdqu 1552(%rsi), %ymm6
vmovdqu 1656(%rsi), %ymm7
vpxor %ymm7, %ymm7, %ymm7
vmovdqu 512(%rsi), %ymm8
vmovdqu 616(%rsi), %ymm9
vmovdqu 720(%rsi), %ymm10
vmovdqu 824(%rsi), %ymm11
vmovdqa %ymm0, 96(%rax)
vmovdqa %ymm1, 224(%rax)
vpaddw %ymm0, %ymm1, %ymm14
vmovdqa %ymm14, 352(%rax)
vmovdqa %ymm2, 480(%rax)
vmovdqa %ymm12, 608(%rax)
vpaddw %ymm2, %ymm12, %ymm14
vmovdqa %ymm14, 736(%rax)
vpaddw %ymm0, %ymm2, %ymm14
vmovdqa %ymm14, 864(%rax)
vpaddw %ymm1, %ymm12, %ymm15
vmovdqa %ymm15, 992(%rax)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 1120(%rax)
vmovdqa %ymm4, 7008(%rax)
vmovdqa %ymm5, 7136(%rax)
vpaddw %ymm4, %ymm5, %ymm14
vmovdqa %ymm14, 7264(%rax)
vmovdqa %ymm6, 7392(%rax)
vmovdqa %ymm7, 7520(%rax)
vpaddw %ymm6, %ymm7, %ymm14
vmovdqa %ymm14, 7648(%rax)
vpaddw %ymm4, %ymm6, %ymm14
vmovdqa %ymm14, 7776(%rax)
vpaddw %ymm5, %ymm7, %ymm15
vmovdqa %ymm15, 7904(%rax)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8032(%rax)
vmovdqa %ymm0, 0(%rsp)
vmovdqa %ymm1, 32(%rsp)
vmovdqa %ymm2, 64(%rsp)
vmovdqa %ymm12, 96(%rsp)
vmovdqa %ymm8, 128(%rsp)
vmovdqa %ymm9, 160(%rsp)
vmovdqa %ymm10, 192(%rsp)
vmovdqa %ymm11, 224(%rsp)
vmovdqu 928(%rsi), %ymm0
vpaddw 0(%rsp), %ymm0, %ymm1
vpaddw 128(%rsp), %ymm4, %ymm2
vpaddw %ymm2, %ymm1, %ymm8
vpsubw %ymm2, %ymm1, %ymm12
vmovdqa %ymm0, 256(%rsp)
vmovdqu 1032(%rsi), %ymm0
vpaddw 32(%rsp), %ymm0, %ymm1
vpaddw 160(%rsp), %ymm5, %ymm2
vpaddw %ymm2, %ymm1, %ymm9
vpsubw %ymm2, %ymm1, %ymm13
vmovdqa %ymm0, 288(%rsp)
vmovdqu 1136(%rsi), %ymm0
vpaddw 64(%rsp), %ymm0, %ymm1
vpaddw 192(%rsp), %ymm6, %ymm2
vpaddw %ymm2, %ymm1, %ymm10
vpsubw %ymm2, %ymm1, %ymm14
vmovdqa %ymm0, 320(%rsp)
vmovdqu 1240(%rsi), %ymm0
vpaddw 96(%rsp), %ymm0, %ymm1
vpaddw 224(%rsp), %ymm7, %ymm2
vpaddw %ymm2, %ymm1, %ymm11
vpsubw %ymm2, %ymm1, %ymm15
vmovdqa %ymm0, 352(%rsp)
vmovdqa %ymm8, 1248(%rax)
vmovdqa %ymm9, 1376(%rax)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 1504(%rax)
vmovdqa %ymm10, 1632(%rax)
vmovdqa %ymm11, 1760(%rax)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 1888(%rax)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 2016(%rax)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 2144(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2272(%rax)
vmovdqa %ymm12, 2400(%rax)
vmovdqa %ymm13, 2528(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 2656(%rax)
vmovdqa %ymm14, 2784(%rax)
vmovdqa %ymm15, 2912(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 3040(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 3168(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 3296(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 3424(%rax)
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
vmovdqa %ymm8, 3552(%rax)
vmovdqa %ymm9, 3680(%rax)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 3808(%rax)
vmovdqa %ymm10, 3936(%rax)
vmovdqa %ymm11, 4064(%rax)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 4192(%rax)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 4320(%rax)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 4448(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 4576(%rax)
vmovdqa %ymm12, 4704(%rax)
vmovdqa %ymm13, 4832(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 4960(%rax)
vmovdqa %ymm14, 5088(%rax)
vmovdqa %ymm15, 5216(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 5344(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 5472(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 5600(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 5728(%rax)
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
vmovdqa %ymm12, 5856(%rax)
vmovdqa %ymm13, 5984(%rax)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 6112(%rax)
vmovdqa %ymm14, 6240(%rax)
vmovdqa %ymm15, 6368(%rax)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 6496(%rax)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 6624(%rax)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 6752(%rax)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 6880(%rax)
vmovdqu 0(%rdx), %ymm0
vmovdqu 104(%rdx), %ymm1
vmovdqu 208(%rdx), %ymm2
vmovdqu 312(%rdx), %ymm12
vmovdqu 1248(%rdx), %ymm4
vmovdqu 1352(%rdx), %ymm5
vmovdqu 1456(%rdx), %ymm6
vmovdqu 1560(%rdx), %ymm7
vmovdqu 416(%rdx), %ymm8
vmovdqu 520(%rdx), %ymm9
vmovdqu 624(%rdx), %ymm10
vmovdqu 728(%rdx), %ymm11
vmovdqa %ymm0, 0(%r11)
vmovdqa %ymm1, 128(%r11)
vpaddw %ymm0, %ymm1, %ymm14
vmovdqa %ymm14, 256(%r11)
vmovdqa %ymm2, 384(%r11)
vmovdqa %ymm12, 512(%r11)
vpaddw %ymm2, %ymm12, %ymm14
vmovdqa %ymm14, 640(%r11)
vpaddw %ymm0, %ymm2, %ymm14
vmovdqa %ymm14, 768(%r11)
vpaddw %ymm1, %ymm12, %ymm15
vmovdqa %ymm15, 896(%r11)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 1024(%r11)
vmovdqa %ymm4, 6912(%r11)
vmovdqa %ymm5, 7040(%r11)
vpaddw %ymm4, %ymm5, %ymm14
vmovdqa %ymm14, 7168(%r11)
vmovdqa %ymm6, 7296(%r11)
vmovdqa %ymm7, 7424(%r11)
vpaddw %ymm6, %ymm7, %ymm14
vmovdqa %ymm14, 7552(%r11)
vpaddw %ymm4, %ymm6, %ymm14
vmovdqa %ymm14, 7680(%r11)
vpaddw %ymm5, %ymm7, %ymm15
vmovdqa %ymm15, 7808(%r11)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7936(%r11)
vmovdqa %ymm0, 0(%rsp)
vmovdqa %ymm1, 32(%rsp)
vmovdqa %ymm2, 64(%rsp)
vmovdqa %ymm12, 96(%rsp)
vmovdqa %ymm8, 128(%rsp)
vmovdqa %ymm9, 160(%rsp)
vmovdqa %ymm10, 192(%rsp)
vmovdqa %ymm11, 224(%rsp)
vmovdqu 832(%rdx), %ymm0
vpaddw 0(%rsp), %ymm0, %ymm1
vpaddw 128(%rsp), %ymm4, %ymm2
vpaddw %ymm2, %ymm1, %ymm8
vpsubw %ymm2, %ymm1, %ymm12
vmovdqa %ymm0, 256(%rsp)
vmovdqu 936(%rdx), %ymm0
vpaddw 32(%rsp), %ymm0, %ymm1
vpaddw 160(%rsp), %ymm5, %ymm2
vpaddw %ymm2, %ymm1, %ymm9
vpsubw %ymm2, %ymm1, %ymm13
vmovdqa %ymm0, 288(%rsp)
vmovdqu 1040(%rdx), %ymm0
vpaddw 64(%rsp), %ymm0, %ymm1
vpaddw 192(%rsp), %ymm6, %ymm2
vpaddw %ymm2, %ymm1, %ymm10
vpsubw %ymm2, %ymm1, %ymm14
vmovdqa %ymm0, 320(%rsp)
vmovdqu 1144(%rdx), %ymm0
vpaddw 96(%rsp), %ymm0, %ymm1
vpaddw 224(%rsp), %ymm7, %ymm2
vpaddw %ymm2, %ymm1, %ymm11
vpsubw %ymm2, %ymm1, %ymm15
vmovdqa %ymm0, 352(%rsp)
vmovdqa %ymm8, 1152(%r11)
vmovdqa %ymm9, 1280(%r11)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 1408(%r11)
vmovdqa %ymm10, 1536(%r11)
vmovdqa %ymm11, 1664(%r11)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 1792(%r11)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 1920(%r11)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 2048(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2176(%r11)
vmovdqa %ymm12, 2304(%r11)
vmovdqa %ymm13, 2432(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 2560(%r11)
vmovdqa %ymm14, 2688(%r11)
vmovdqa %ymm15, 2816(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 2944(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 3072(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 3200(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 3328(%r11)
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
vmovdqa %ymm8, 3456(%r11)
vmovdqa %ymm9, 3584(%r11)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 3712(%r11)
vmovdqa %ymm10, 3840(%r11)
vmovdqa %ymm11, 3968(%r11)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 4096(%r11)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 4224(%r11)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 4352(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 4480(%r11)
vmovdqa %ymm12, 4608(%r11)
vmovdqa %ymm13, 4736(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 4864(%r11)
vmovdqa %ymm14, 4992(%r11)
vmovdqa %ymm15, 5120(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 5248(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 5376(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 5504(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 5632(%r11)
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
vmovdqa %ymm12, 5760(%r11)
vmovdqa %ymm13, 5888(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 6016(%r11)
vmovdqa %ymm14, 6144(%r11)
vmovdqa %ymm15, 6272(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 6400(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 6528(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 6656(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 6784(%r11)
vmovdqu 32(%rdx), %ymm0
vmovdqu 136(%rdx), %ymm1
vmovdqu 240(%rdx), %ymm2
vmovdqu 344(%rdx), %ymm12
vmovdqu 1280(%rdx), %ymm4
vmovdqu 1384(%rdx), %ymm5
vmovdqu 1488(%rdx), %ymm6
vmovdqu 1592(%rdx), %ymm7
vmovdqu 448(%rdx), %ymm8
vmovdqu 552(%rdx), %ymm9
vmovdqu 656(%rdx), %ymm10
vmovdqu 760(%rdx), %ymm11
vmovdqa %ymm0, 32(%r11)
vmovdqa %ymm1, 160(%r11)
vpaddw %ymm0, %ymm1, %ymm14
vmovdqa %ymm14, 288(%r11)
vmovdqa %ymm2, 416(%r11)
vmovdqa %ymm12, 544(%r11)
vpaddw %ymm2, %ymm12, %ymm14
vmovdqa %ymm14, 672(%r11)
vpaddw %ymm0, %ymm2, %ymm14
vmovdqa %ymm14, 800(%r11)
vpaddw %ymm1, %ymm12, %ymm15
vmovdqa %ymm15, 928(%r11)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 1056(%r11)
vmovdqa %ymm4, 6944(%r11)
vmovdqa %ymm5, 7072(%r11)
vpaddw %ymm4, %ymm5, %ymm14
vmovdqa %ymm14, 7200(%r11)
vmovdqa %ymm6, 7328(%r11)
vmovdqa %ymm7, 7456(%r11)
vpaddw %ymm6, %ymm7, %ymm14
vmovdqa %ymm14, 7584(%r11)
vpaddw %ymm4, %ymm6, %ymm14
vmovdqa %ymm14, 7712(%r11)
vpaddw %ymm5, %ymm7, %ymm15
vmovdqa %ymm15, 7840(%r11)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7968(%r11)
vmovdqa %ymm0, 0(%rsp)
vmovdqa %ymm1, 32(%rsp)
vmovdqa %ymm2, 64(%rsp)
vmovdqa %ymm12, 96(%rsp)
vmovdqa %ymm8, 128(%rsp)
vmovdqa %ymm9, 160(%rsp)
vmovdqa %ymm10, 192(%rsp)
vmovdqa %ymm11, 224(%rsp)
vmovdqu 864(%rdx), %ymm0
vpaddw 0(%rsp), %ymm0, %ymm1
vpaddw 128(%rsp), %ymm4, %ymm2
vpaddw %ymm2, %ymm1, %ymm8
vpsubw %ymm2, %ymm1, %ymm12
vmovdqa %ymm0, 256(%rsp)
vmovdqu 968(%rdx), %ymm0
vpaddw 32(%rsp), %ymm0, %ymm1
vpaddw 160(%rsp), %ymm5, %ymm2
vpaddw %ymm2, %ymm1, %ymm9
vpsubw %ymm2, %ymm1, %ymm13
vmovdqa %ymm0, 288(%rsp)
vmovdqu 1072(%rdx), %ymm0
vpaddw 64(%rsp), %ymm0, %ymm1
vpaddw 192(%rsp), %ymm6, %ymm2
vpaddw %ymm2, %ymm1, %ymm10
vpsubw %ymm2, %ymm1, %ymm14
vmovdqa %ymm0, 320(%rsp)
vmovdqu 1176(%rdx), %ymm0
vpaddw 96(%rsp), %ymm0, %ymm1
vpaddw 224(%rsp), %ymm7, %ymm2
vpaddw %ymm2, %ymm1, %ymm11
vpsubw %ymm2, %ymm1, %ymm15
vmovdqa %ymm0, 352(%rsp)
vmovdqa %ymm8, 1184(%r11)
vmovdqa %ymm9, 1312(%r11)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 1440(%r11)
vmovdqa %ymm10, 1568(%r11)
vmovdqa %ymm11, 1696(%r11)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 1824(%r11)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 1952(%r11)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 2080(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2208(%r11)
vmovdqa %ymm12, 2336(%r11)
vmovdqa %ymm13, 2464(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 2592(%r11)
vmovdqa %ymm14, 2720(%r11)
vmovdqa %ymm15, 2848(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 2976(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 3104(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 3232(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 3360(%r11)
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
vmovdqa %ymm8, 3488(%r11)
vmovdqa %ymm9, 3616(%r11)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 3744(%r11)
vmovdqa %ymm10, 3872(%r11)
vmovdqa %ymm11, 4000(%r11)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 4128(%r11)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 4256(%r11)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 4384(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 4512(%r11)
vmovdqa %ymm12, 4640(%r11)
vmovdqa %ymm13, 4768(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 4896(%r11)
vmovdqa %ymm14, 5024(%r11)
vmovdqa %ymm15, 5152(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 5280(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 5408(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 5536(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 5664(%r11)
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
vmovdqa %ymm12, 5792(%r11)
vmovdqa %ymm13, 5920(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 6048(%r11)
vmovdqa %ymm14, 6176(%r11)
vmovdqa %ymm15, 6304(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 6432(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 6560(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 6688(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 6816(%r11)
vmovdqu 64(%rdx), %ymm0
vmovdqu 168(%rdx), %ymm1
vmovdqu 272(%rdx), %ymm2
vmovdqu 376(%rdx), %ymm12
vmovdqu 1312(%rdx), %ymm4
vmovdqu 1416(%rdx), %ymm5
vmovdqu 1520(%rdx), %ymm6
vmovdqu 1624(%rdx), %ymm7
vpand mask_9_7(%rip), %ymm7, %ymm7
vmovdqu 480(%rdx), %ymm8
vmovdqu 584(%rdx), %ymm9
vmovdqu 688(%rdx), %ymm10
vmovdqu 792(%rdx), %ymm11
vmovdqa %ymm0, 64(%r11)
vmovdqa %ymm1, 192(%r11)
vpaddw %ymm0, %ymm1, %ymm14
vmovdqa %ymm14, 320(%r11)
vmovdqa %ymm2, 448(%r11)
vmovdqa %ymm12, 576(%r11)
vpaddw %ymm2, %ymm12, %ymm14
vmovdqa %ymm14, 704(%r11)
vpaddw %ymm0, %ymm2, %ymm14
vmovdqa %ymm14, 832(%r11)
vpaddw %ymm1, %ymm12, %ymm15
vmovdqa %ymm15, 960(%r11)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 1088(%r11)
vmovdqa %ymm4, 6976(%r11)
vmovdqa %ymm5, 7104(%r11)
vpaddw %ymm4, %ymm5, %ymm14
vmovdqa %ymm14, 7232(%r11)
vmovdqa %ymm6, 7360(%r11)
vmovdqa %ymm7, 7488(%r11)
vpaddw %ymm6, %ymm7, %ymm14
vmovdqa %ymm14, 7616(%r11)
vpaddw %ymm4, %ymm6, %ymm14
vmovdqa %ymm14, 7744(%r11)
vpaddw %ymm5, %ymm7, %ymm15
vmovdqa %ymm15, 7872(%r11)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8000(%r11)
vmovdqa %ymm0, 0(%rsp)
vmovdqa %ymm1, 32(%rsp)
vmovdqa %ymm2, 64(%rsp)
vmovdqa %ymm12, 96(%rsp)
vmovdqa %ymm8, 128(%rsp)
vmovdqa %ymm9, 160(%rsp)
vmovdqa %ymm10, 192(%rsp)
vmovdqa %ymm11, 224(%rsp)
vmovdqu 896(%rdx), %ymm0
vpaddw 0(%rsp), %ymm0, %ymm1
vpaddw 128(%rsp), %ymm4, %ymm2
vpaddw %ymm2, %ymm1, %ymm8
vpsubw %ymm2, %ymm1, %ymm12
vmovdqa %ymm0, 256(%rsp)
vmovdqu 1000(%rdx), %ymm0
vpaddw 32(%rsp), %ymm0, %ymm1
vpaddw 160(%rsp), %ymm5, %ymm2
vpaddw %ymm2, %ymm1, %ymm9
vpsubw %ymm2, %ymm1, %ymm13
vmovdqa %ymm0, 288(%rsp)
vmovdqu 1104(%rdx), %ymm0
vpaddw 64(%rsp), %ymm0, %ymm1
vpaddw 192(%rsp), %ymm6, %ymm2
vpaddw %ymm2, %ymm1, %ymm10
vpsubw %ymm2, %ymm1, %ymm14
vmovdqa %ymm0, 320(%rsp)
vmovdqu 1208(%rdx), %ymm0
vpaddw 96(%rsp), %ymm0, %ymm1
vpaddw 224(%rsp), %ymm7, %ymm2
vpaddw %ymm2, %ymm1, %ymm11
vpsubw %ymm2, %ymm1, %ymm15
vmovdqa %ymm0, 352(%rsp)
vmovdqa %ymm8, 1216(%r11)
vmovdqa %ymm9, 1344(%r11)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 1472(%r11)
vmovdqa %ymm10, 1600(%r11)
vmovdqa %ymm11, 1728(%r11)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 1856(%r11)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 1984(%r11)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 2112(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2240(%r11)
vmovdqa %ymm12, 2368(%r11)
vmovdqa %ymm13, 2496(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 2624(%r11)
vmovdqa %ymm14, 2752(%r11)
vmovdqa %ymm15, 2880(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 3008(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 3136(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 3264(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 3392(%r11)
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
vmovdqa %ymm8, 3520(%r11)
vmovdqa %ymm9, 3648(%r11)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 3776(%r11)
vmovdqa %ymm10, 3904(%r11)
vmovdqa %ymm11, 4032(%r11)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 4160(%r11)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 4288(%r11)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 4416(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 4544(%r11)
vmovdqa %ymm12, 4672(%r11)
vmovdqa %ymm13, 4800(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 4928(%r11)
vmovdqa %ymm14, 5056(%r11)
vmovdqa %ymm15, 5184(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 5312(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 5440(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 5568(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 5696(%r11)
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
vmovdqa %ymm12, 5824(%r11)
vmovdqa %ymm13, 5952(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 6080(%r11)
vmovdqa %ymm14, 6208(%r11)
vmovdqa %ymm15, 6336(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 6464(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 6592(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 6720(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 6848(%r11)
vmovdqu 96(%rdx), %ymm0
vmovdqu 200(%rdx), %ymm1
vmovdqu 304(%rdx), %ymm2
vmovdqu 408(%rdx), %ymm12
vmovdqu 1344(%rdx), %ymm4
vmovdqu 1448(%rdx), %ymm5
vmovdqu 1552(%rdx), %ymm6
vmovdqu 1656(%rdx), %ymm7
vpxor %ymm7, %ymm7, %ymm7
vmovdqu 512(%rdx), %ymm8
vmovdqu 616(%rdx), %ymm9
vmovdqu 720(%rdx), %ymm10
vmovdqu 824(%rdx), %ymm11
vmovdqa %ymm0, 96(%r11)
vmovdqa %ymm1, 224(%r11)
vpaddw %ymm0, %ymm1, %ymm14
vmovdqa %ymm14, 352(%r11)
vmovdqa %ymm2, 480(%r11)
vmovdqa %ymm12, 608(%r11)
vpaddw %ymm2, %ymm12, %ymm14
vmovdqa %ymm14, 736(%r11)
vpaddw %ymm0, %ymm2, %ymm14
vmovdqa %ymm14, 864(%r11)
vpaddw %ymm1, %ymm12, %ymm15
vmovdqa %ymm15, 992(%r11)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 1120(%r11)
vmovdqa %ymm4, 7008(%r11)
vmovdqa %ymm5, 7136(%r11)
vpaddw %ymm4, %ymm5, %ymm14
vmovdqa %ymm14, 7264(%r11)
vmovdqa %ymm6, 7392(%r11)
vmovdqa %ymm7, 7520(%r11)
vpaddw %ymm6, %ymm7, %ymm14
vmovdqa %ymm14, 7648(%r11)
vpaddw %ymm4, %ymm6, %ymm14
vmovdqa %ymm14, 7776(%r11)
vpaddw %ymm5, %ymm7, %ymm15
vmovdqa %ymm15, 7904(%r11)
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8032(%r11)
vmovdqa %ymm0, 0(%rsp)
vmovdqa %ymm1, 32(%rsp)
vmovdqa %ymm2, 64(%rsp)
vmovdqa %ymm12, 96(%rsp)
vmovdqa %ymm8, 128(%rsp)
vmovdqa %ymm9, 160(%rsp)
vmovdqa %ymm10, 192(%rsp)
vmovdqa %ymm11, 224(%rsp)
vmovdqu 928(%rdx), %ymm0
vpaddw 0(%rsp), %ymm0, %ymm1
vpaddw 128(%rsp), %ymm4, %ymm2
vpaddw %ymm2, %ymm1, %ymm8
vpsubw %ymm2, %ymm1, %ymm12
vmovdqa %ymm0, 256(%rsp)
vmovdqu 1032(%rdx), %ymm0
vpaddw 32(%rsp), %ymm0, %ymm1
vpaddw 160(%rsp), %ymm5, %ymm2
vpaddw %ymm2, %ymm1, %ymm9
vpsubw %ymm2, %ymm1, %ymm13
vmovdqa %ymm0, 288(%rsp)
vmovdqu 1136(%rdx), %ymm0
vpaddw 64(%rsp), %ymm0, %ymm1
vpaddw 192(%rsp), %ymm6, %ymm2
vpaddw %ymm2, %ymm1, %ymm10
vpsubw %ymm2, %ymm1, %ymm14
vmovdqa %ymm0, 320(%rsp)
vmovdqu 1240(%rdx), %ymm0
vpaddw 96(%rsp), %ymm0, %ymm1
vpaddw 224(%rsp), %ymm7, %ymm2
vpaddw %ymm2, %ymm1, %ymm11
vpsubw %ymm2, %ymm1, %ymm15
vmovdqa %ymm0, 352(%rsp)
vmovdqa %ymm8, 1248(%r11)
vmovdqa %ymm9, 1376(%r11)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 1504(%r11)
vmovdqa %ymm10, 1632(%r11)
vmovdqa %ymm11, 1760(%r11)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 1888(%r11)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 2016(%r11)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 2144(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 2272(%r11)
vmovdqa %ymm12, 2400(%r11)
vmovdqa %ymm13, 2528(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 2656(%r11)
vmovdqa %ymm14, 2784(%r11)
vmovdqa %ymm15, 2912(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 3040(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 3168(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 3296(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 3424(%r11)
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
vmovdqa %ymm8, 3552(%r11)
vmovdqa %ymm9, 3680(%r11)
vpaddw %ymm8, %ymm9, %ymm0
vmovdqa %ymm0, 3808(%r11)
vmovdqa %ymm10, 3936(%r11)
vmovdqa %ymm11, 4064(%r11)
vpaddw %ymm10, %ymm11, %ymm0
vmovdqa %ymm0, 4192(%r11)
vpaddw %ymm8, %ymm10, %ymm0
vmovdqa %ymm0, 4320(%r11)
vpaddw %ymm9, %ymm11, %ymm1
vmovdqa %ymm1, 4448(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 4576(%r11)
vmovdqa %ymm12, 4704(%r11)
vmovdqa %ymm13, 4832(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 4960(%r11)
vmovdqa %ymm14, 5088(%r11)
vmovdqa %ymm15, 5216(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 5344(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 5472(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 5600(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 5728(%r11)
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
vmovdqa %ymm12, 5856(%r11)
vmovdqa %ymm13, 5984(%r11)
vpaddw %ymm12, %ymm13, %ymm0
vmovdqa %ymm0, 6112(%r11)
vmovdqa %ymm14, 6240(%r11)
vmovdqa %ymm15, 6368(%r11)
vpaddw %ymm14, %ymm15, %ymm0
vmovdqa %ymm0, 6496(%r11)
vpaddw %ymm12, %ymm14, %ymm0
vmovdqa %ymm0, 6624(%r11)
vpaddw %ymm13, %ymm15, %ymm1
vmovdqa %ymm1, 6752(%r11)
vpaddw %ymm0, %ymm1, %ymm0
vmovdqa %ymm0, 6880(%r11)
subq $11584, %rsp
mov $4, %ecx
karatsuba_loop_1:
mov %rsp, %r9
mov %rsp, %r10
subq $32, %rsp
vmovdqa 0(%rax), %ymm0
vmovdqa 256(%rax), %ymm1
vmovdqa 512(%rax), %ymm2
vmovdqa 768(%rax), %ymm3
vpunpcklwd 128(%rax), %ymm0, %ymm4
vpunpckhwd 128(%rax), %ymm0, %ymm5
vpunpcklwd 384(%rax), %ymm1, %ymm6
vpunpckhwd 384(%rax), %ymm1, %ymm7
vpunpcklwd 640(%rax), %ymm2, %ymm8
vpunpckhwd 640(%rax), %ymm2, %ymm9
vpunpcklwd 896(%rax), %ymm3, %ymm10
vpunpckhwd 896(%rax), %ymm3, %ymm11
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
vmovdqa 1024(%rax), %ymm0
vmovdqa 1280(%rax), %ymm1
vmovdqa 1536(%rax), %ymm2
vmovdqa 1792(%rax), %ymm3
vpunpcklwd 1152(%rax), %ymm0, %ymm12
vpunpckhwd 1152(%rax), %ymm0, %ymm13
vpunpcklwd 1408(%rax), %ymm1, %ymm14
vpunpckhwd 1408(%rax), %ymm1, %ymm15
vpunpcklwd 1664(%rax), %ymm2, %ymm0
vpunpckhwd 1664(%rax), %ymm2, %ymm1
vpunpcklwd 1920(%rax), %ymm3, %ymm2
vpunpckhwd 1920(%rax), %ymm3, %ymm3
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
vmovdqa 288(%rax), %ymm1
vmovdqa 544(%rax), %ymm2
vmovdqa 800(%rax), %ymm3
vpunpcklwd 160(%rax), %ymm0, %ymm4
vpunpckhwd 160(%rax), %ymm0, %ymm5
vpunpcklwd 416(%rax), %ymm1, %ymm6
vpunpckhwd 416(%rax), %ymm1, %ymm7
vpunpcklwd 672(%rax), %ymm2, %ymm8
vpunpckhwd 672(%rax), %ymm2, %ymm9
vpunpcklwd 928(%rax), %ymm3, %ymm10
vpunpckhwd 928(%rax), %ymm3, %ymm11
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
vmovdqa 1056(%rax), %ymm0
vmovdqa 1312(%rax), %ymm1
vmovdqa 1568(%rax), %ymm2
vmovdqa 1824(%rax), %ymm3
vpunpcklwd 1184(%rax), %ymm0, %ymm12
vpunpckhwd 1184(%rax), %ymm0, %ymm13
vpunpcklwd 1440(%rax), %ymm1, %ymm14
vpunpckhwd 1440(%rax), %ymm1, %ymm15
vpunpcklwd 1696(%rax), %ymm2, %ymm0
vpunpckhwd 1696(%rax), %ymm2, %ymm1
vpunpcklwd 1952(%rax), %ymm3, %ymm2
vpunpckhwd 1952(%rax), %ymm3, %ymm3
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
vmovdqa 320(%rax), %ymm1
vmovdqa 576(%rax), %ymm2
vmovdqa 832(%rax), %ymm3
vpunpcklwd 192(%rax), %ymm0, %ymm4
vpunpckhwd 192(%rax), %ymm0, %ymm5
vpunpcklwd 448(%rax), %ymm1, %ymm6
vpunpckhwd 448(%rax), %ymm1, %ymm7
vpunpcklwd 704(%rax), %ymm2, %ymm8
vpunpckhwd 704(%rax), %ymm2, %ymm9
vpunpcklwd 960(%rax), %ymm3, %ymm10
vpunpckhwd 960(%rax), %ymm3, %ymm11
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
vmovdqa 1088(%rax), %ymm0
vmovdqa 1344(%rax), %ymm1
vmovdqa 1600(%rax), %ymm2
vmovdqa 1856(%rax), %ymm3
vpunpcklwd 1216(%rax), %ymm0, %ymm12
vpunpckhwd 1216(%rax), %ymm0, %ymm13
vpunpcklwd 1472(%rax), %ymm1, %ymm14
vpunpckhwd 1472(%rax), %ymm1, %ymm15
vpunpcklwd 1728(%rax), %ymm2, %ymm0
vpunpckhwd 1728(%rax), %ymm2, %ymm1
vpunpcklwd 1984(%rax), %ymm3, %ymm2
vpunpckhwd 1984(%rax), %ymm3, %ymm3
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
vmovdqa 96(%rax), %ymm0
vmovdqa 352(%rax), %ymm1
vmovdqa 608(%rax), %ymm2
vmovdqa 864(%rax), %ymm3
vpunpcklwd 224(%rax), %ymm0, %ymm4
vpunpckhwd 224(%rax), %ymm0, %ymm5
vpunpcklwd 480(%rax), %ymm1, %ymm6
vpunpckhwd 480(%rax), %ymm1, %ymm7
vpunpcklwd 736(%rax), %ymm2, %ymm8
vpunpckhwd 736(%rax), %ymm2, %ymm9
vpunpcklwd 992(%rax), %ymm3, %ymm10
vpunpckhwd 992(%rax), %ymm3, %ymm11
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
vmovdqa 1120(%rax), %ymm0
vmovdqa 1376(%rax), %ymm1
vmovdqa 1632(%rax), %ymm2
vmovdqa 1888(%rax), %ymm3
vpunpcklwd 1248(%rax), %ymm0, %ymm12
vpunpckhwd 1248(%rax), %ymm0, %ymm13
vpunpcklwd 1504(%rax), %ymm1, %ymm14
vpunpckhwd 1504(%rax), %ymm1, %ymm15
vpunpcklwd 1760(%rax), %ymm2, %ymm0
vpunpckhwd 1760(%rax), %ymm2, %ymm1
vpunpcklwd 2016(%rax), %ymm3, %ymm2
vpunpckhwd 2016(%rax), %ymm3, %ymm3
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
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1760(%r9)
addq $32, %rsp
subq $32, %rsp
vmovdqa 0(%r11), %ymm0
vmovdqa 256(%r11), %ymm1
vmovdqa 512(%r11), %ymm2
vmovdqa 768(%r11), %ymm3
vpunpcklwd 128(%r11), %ymm0, %ymm4
vpunpckhwd 128(%r11), %ymm0, %ymm5
vpunpcklwd 384(%r11), %ymm1, %ymm6
vpunpckhwd 384(%r11), %ymm1, %ymm7
vpunpcklwd 640(%r11), %ymm2, %ymm8
vpunpckhwd 640(%r11), %ymm2, %ymm9
vpunpcklwd 896(%r11), %ymm3, %ymm10
vpunpckhwd 896(%r11), %ymm3, %ymm11
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
vmovdqa 1024(%r11), %ymm0
vmovdqa 1280(%r11), %ymm1
vmovdqa 1536(%r11), %ymm2
vmovdqa 1792(%r11), %ymm3
vpunpcklwd 1152(%r11), %ymm0, %ymm12
vpunpckhwd 1152(%r11), %ymm0, %ymm13
vpunpcklwd 1408(%r11), %ymm1, %ymm14
vpunpckhwd 1408(%r11), %ymm1, %ymm15
vpunpcklwd 1664(%r11), %ymm2, %ymm0
vpunpckhwd 1664(%r11), %ymm2, %ymm1
vpunpcklwd 1920(%r11), %ymm3, %ymm2
vpunpckhwd 1920(%r11), %ymm3, %ymm3
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
vmovdqa %ymm15, 1664(%r9)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 1696(%r9)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 1728(%r9)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 1760(%r9)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 1792(%r9)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 1824(%r9)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 1856(%r9)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 1920(%r9)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 1952(%r9)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 1984(%r9)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 2016(%r9)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 2048(%r9)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 2080(%r9)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 2112(%r9)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1888(%r9)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 2144(%r9)
vmovdqa 32(%r11), %ymm0
vmovdqa 288(%r11), %ymm1
vmovdqa 544(%r11), %ymm2
vmovdqa 800(%r11), %ymm3
vpunpcklwd 160(%r11), %ymm0, %ymm4
vpunpckhwd 160(%r11), %ymm0, %ymm5
vpunpcklwd 416(%r11), %ymm1, %ymm6
vpunpckhwd 416(%r11), %ymm1, %ymm7
vpunpcklwd 672(%r11), %ymm2, %ymm8
vpunpckhwd 672(%r11), %ymm2, %ymm9
vpunpcklwd 928(%r11), %ymm3, %ymm10
vpunpckhwd 928(%r11), %ymm3, %ymm11
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
vmovdqa 1056(%r11), %ymm0
vmovdqa 1312(%r11), %ymm1
vmovdqa 1568(%r11), %ymm2
vmovdqa 1824(%r11), %ymm3
vpunpcklwd 1184(%r11), %ymm0, %ymm12
vpunpckhwd 1184(%r11), %ymm0, %ymm13
vpunpcklwd 1440(%r11), %ymm1, %ymm14
vpunpckhwd 1440(%r11), %ymm1, %ymm15
vpunpcklwd 1696(%r11), %ymm2, %ymm0
vpunpckhwd 1696(%r11), %ymm2, %ymm1
vpunpcklwd 1952(%r11), %ymm3, %ymm2
vpunpckhwd 1952(%r11), %ymm3, %ymm3
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
vmovdqa %ymm15, 2176(%r9)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 2208(%r9)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 2240(%r9)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 2272(%r9)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 2304(%r9)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 2336(%r9)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 2368(%r9)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 2432(%r9)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 2464(%r9)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 2496(%r9)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 2528(%r9)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 2560(%r9)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 2592(%r9)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 2624(%r9)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 2400(%r9)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 2656(%r9)
vmovdqa 64(%r11), %ymm0
vmovdqa 320(%r11), %ymm1
vmovdqa 576(%r11), %ymm2
vmovdqa 832(%r11), %ymm3
vpunpcklwd 192(%r11), %ymm0, %ymm4
vpunpckhwd 192(%r11), %ymm0, %ymm5
vpunpcklwd 448(%r11), %ymm1, %ymm6
vpunpckhwd 448(%r11), %ymm1, %ymm7
vpunpcklwd 704(%r11), %ymm2, %ymm8
vpunpckhwd 704(%r11), %ymm2, %ymm9
vpunpcklwd 960(%r11), %ymm3, %ymm10
vpunpckhwd 960(%r11), %ymm3, %ymm11
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
vmovdqa 1088(%r11), %ymm0
vmovdqa 1344(%r11), %ymm1
vmovdqa 1600(%r11), %ymm2
vmovdqa 1856(%r11), %ymm3
vpunpcklwd 1216(%r11), %ymm0, %ymm12
vpunpckhwd 1216(%r11), %ymm0, %ymm13
vpunpcklwd 1472(%r11), %ymm1, %ymm14
vpunpckhwd 1472(%r11), %ymm1, %ymm15
vpunpcklwd 1728(%r11), %ymm2, %ymm0
vpunpckhwd 1728(%r11), %ymm2, %ymm1
vpunpcklwd 1984(%r11), %ymm3, %ymm2
vpunpckhwd 1984(%r11), %ymm3, %ymm3
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
vmovdqa %ymm15, 2688(%r9)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 2720(%r9)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 2752(%r9)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 2784(%r9)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 2816(%r9)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 2848(%r9)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 2880(%r9)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 2944(%r9)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 2976(%r9)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 3008(%r9)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 3040(%r9)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 3072(%r9)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 3104(%r9)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 3136(%r9)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 2912(%r9)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 3168(%r9)
vmovdqa 96(%r11), %ymm0
vmovdqa 352(%r11), %ymm1
vmovdqa 608(%r11), %ymm2
vmovdqa 864(%r11), %ymm3
vpunpcklwd 224(%r11), %ymm0, %ymm4
vpunpckhwd 224(%r11), %ymm0, %ymm5
vpunpcklwd 480(%r11), %ymm1, %ymm6
vpunpckhwd 480(%r11), %ymm1, %ymm7
vpunpcklwd 736(%r11), %ymm2, %ymm8
vpunpckhwd 736(%r11), %ymm2, %ymm9
vpunpcklwd 992(%r11), %ymm3, %ymm10
vpunpckhwd 992(%r11), %ymm3, %ymm11
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
vmovdqa 1120(%r11), %ymm0
vmovdqa 1376(%r11), %ymm1
vmovdqa 1632(%r11), %ymm2
vmovdqa 1888(%r11), %ymm3
vpunpcklwd 1248(%r11), %ymm0, %ymm12
vpunpckhwd 1248(%r11), %ymm0, %ymm13
vpunpcklwd 1504(%r11), %ymm1, %ymm14
vpunpckhwd 1504(%r11), %ymm1, %ymm15
vpunpcklwd 1760(%r11), %ymm2, %ymm0
vpunpckhwd 1760(%r11), %ymm2, %ymm1
vpunpcklwd 2016(%r11), %ymm3, %ymm2
vpunpckhwd 2016(%r11), %ymm3, %ymm3
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
vmovdqa %ymm15, 3200(%r9)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 3232(%r9)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 3264(%r9)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 3296(%r9)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 3328(%r9)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 3360(%r9)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 3392(%r9)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 3424(%r9)
addq $32, %rsp
innerloop_1:
vmovdqa 0(%r9), %ymm0
vmovdqa 1664(%r9), %ymm7
vmovdqa 32(%r9), %ymm1
vmovdqa 1696(%r9), %ymm8
vmovdqa 64(%r9), %ymm2
vmovdqa 1728(%r9), %ymm9
vmovdqa 96(%r9), %ymm3
vmovdqa 1760(%r9), %ymm10
vmovdqa 128(%r9), %ymm4
vmovdqa 1792(%r9), %ymm11
vmovdqa 160(%r9), %ymm5
vmovdqa 1824(%r9), %ymm12
vmovdqa 192(%r9), %ymm6
vmovdqa 1856(%r9), %ymm13
vpmullw %ymm0, %ymm7, %ymm14
vmovdqa %ymm14, 3328(%r10)
vpmullw %ymm0, %ymm8, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3360(%r10)
vpmullw %ymm0, %ymm9, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3392(%r10)
vpmullw %ymm0, %ymm10, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3424(%r10)
vpmullw %ymm0, %ymm11, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3456(%r10)
vpmullw %ymm0, %ymm12, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3488(%r10)
vpmullw %ymm0, %ymm13, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3520(%r10)
vpmullw %ymm1, %ymm13, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3552(%r10)
vpmullw %ymm2, %ymm13, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3584(%r10)
vpmullw %ymm3, %ymm13, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3616(%r10)
vpmullw %ymm4, %ymm13, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3648(%r10)
vpmullw %ymm5, %ymm13, %ymm14
vpmullw %ymm6, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3680(%r10)
vpmullw %ymm6, %ymm13, %ymm14
vmovdqa %ymm14, 3712(%r10)
vmovdqa 1888(%r9), %ymm7
vmovdqa 1920(%r9), %ymm8
vmovdqa 1952(%r9), %ymm9
vmovdqa 1984(%r9), %ymm10
vmovdqa 2016(%r9), %ymm11
vmovdqa 2048(%r9), %ymm12
vmovdqa 3552(%r10), %ymm14
vpmullw %ymm0, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3552(%r10)
vmovdqa 3584(%r10), %ymm14
vpmullw %ymm0, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3584(%r10)
vmovdqa 3616(%r10), %ymm14
vpmullw %ymm0, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3616(%r10)
vmovdqa 3648(%r10), %ymm14
vpmullw %ymm0, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3648(%r10)
vmovdqa 3680(%r10), %ymm14
vpmullw %ymm0, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3680(%r10)
vmovdqa 3712(%r10), %ymm14
vpmullw %ymm0, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3712(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3744(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3776(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3808(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3840(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3872(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm6, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3904(%r10)
vmovdqa 224(%r9), %ymm0
vmovdqa 256(%r9), %ymm1
vmovdqa 288(%r9), %ymm2
vmovdqa 320(%r9), %ymm3
vmovdqa 352(%r9), %ymm4
vmovdqa 384(%r9), %ymm5
vmovdqa 3776(%r10), %ymm14
vpmullw %ymm0, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3776(%r10)
vmovdqa 3808(%r10), %ymm14
vpmullw %ymm0, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3808(%r10)
vmovdqa 3840(%r10), %ymm14
vpmullw %ymm0, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3840(%r10)
vmovdqa 3872(%r10), %ymm14
vpmullw %ymm0, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3872(%r10)
vmovdqa 3904(%r10), %ymm14
vpmullw %ymm0, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3904(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm0, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3936(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3968(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4000(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4032(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4064(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4096(%r10)
vmovdqa 1664(%r9), %ymm7
vmovdqa 1696(%r9), %ymm8
vmovdqa 1728(%r9), %ymm9
vmovdqa 1760(%r9), %ymm10
vmovdqa 1792(%r9), %ymm11
vmovdqa 1824(%r9), %ymm12
vmovdqa 3552(%r10), %ymm14
vpmullw %ymm0, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3552(%r10)
vmovdqa 3584(%r10), %ymm14
vpmullw %ymm0, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3584(%r10)
vmovdqa 3616(%r10), %ymm14
vpmullw %ymm0, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3616(%r10)
vmovdqa 3648(%r10), %ymm14
vpmullw %ymm0, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3648(%r10)
vmovdqa 3680(%r10), %ymm14
vpmullw %ymm0, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3680(%r10)
vmovdqa 3712(%r10), %ymm14
vpmullw %ymm0, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3712(%r10)
vmovdqa 3744(%r10), %ymm14
vpmullw %ymm0, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3744(%r10)
vmovdqa 3776(%r10), %ymm14
vpmullw %ymm1, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3776(%r10)
vmovdqa 3808(%r10), %ymm14
vpmullw %ymm2, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3808(%r10)
vmovdqa 3840(%r10), %ymm14
vpmullw %ymm3, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3840(%r10)
vmovdqa 3872(%r10), %ymm14
vpmullw %ymm4, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3872(%r10)
vmovdqa 3904(%r10), %ymm14
vpmullw %ymm5, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 3904(%r10)
vmovdqa 416(%r9), %ymm0
vmovdqa 2080(%r9), %ymm7
vmovdqa 448(%r9), %ymm1
vmovdqa 2112(%r9), %ymm8
vmovdqa 480(%r9), %ymm2
vmovdqa 2144(%r9), %ymm9
vmovdqa 512(%r9), %ymm3
vmovdqa 2176(%r9), %ymm10
vmovdqa 544(%r9), %ymm4
vmovdqa 2208(%r9), %ymm11
vmovdqa 576(%r9), %ymm5
vmovdqa 2240(%r9), %ymm12
vmovdqa 608(%r9), %ymm6
vmovdqa 2272(%r9), %ymm13
vpmullw %ymm0, %ymm7, %ymm14
vmovdqa %ymm14, 4160(%r10)
vpmullw %ymm0, %ymm8, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4192(%r10)
vpmullw %ymm0, %ymm9, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4224(%r10)
vpmullw %ymm0, %ymm10, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4256(%r10)
vpmullw %ymm0, %ymm11, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4288(%r10)
vpmullw %ymm0, %ymm12, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4320(%r10)
vpmullw %ymm0, %ymm13, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4352(%r10)
vpmullw %ymm1, %ymm13, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4384(%r10)
vpmullw %ymm2, %ymm13, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4416(%r10)
vpmullw %ymm3, %ymm13, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4448(%r10)
vpmullw %ymm4, %ymm13, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4480(%r10)
vpmullw %ymm5, %ymm13, %ymm14
vpmullw %ymm6, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4512(%r10)
vpmullw %ymm6, %ymm13, %ymm14
vmovdqa %ymm14, 4544(%r10)
vmovdqa 2304(%r9), %ymm7
vmovdqa 2336(%r9), %ymm8
vmovdqa 2368(%r9), %ymm9
vmovdqa 2400(%r9), %ymm10
vmovdqa 2432(%r9), %ymm11
vmovdqa 2464(%r9), %ymm12
vmovdqa 4384(%r10), %ymm14
vpmullw %ymm0, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4384(%r10)
vmovdqa 4416(%r10), %ymm14
vpmullw %ymm0, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4416(%r10)
vmovdqa 4448(%r10), %ymm14
vpmullw %ymm0, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4448(%r10)
vmovdqa 4480(%r10), %ymm14
vpmullw %ymm0, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4480(%r10)
vmovdqa 4512(%r10), %ymm14
vpmullw %ymm0, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4512(%r10)
vmovdqa 4544(%r10), %ymm14
vpmullw %ymm0, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4544(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4576(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4608(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4640(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4672(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4704(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm6, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4736(%r10)
vmovdqa 640(%r9), %ymm0
vmovdqa 672(%r9), %ymm1
vmovdqa 704(%r9), %ymm2
vmovdqa 736(%r9), %ymm3
vmovdqa 768(%r9), %ymm4
vmovdqa 800(%r9), %ymm5
vmovdqa 4608(%r10), %ymm14
vpmullw %ymm0, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4608(%r10)
vmovdqa 4640(%r10), %ymm14
vpmullw %ymm0, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4640(%r10)
vmovdqa 4672(%r10), %ymm14
vpmullw %ymm0, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4672(%r10)
vmovdqa 4704(%r10), %ymm14
vpmullw %ymm0, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4704(%r10)
vmovdqa 4736(%r10), %ymm14
vpmullw %ymm0, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4736(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm0, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4768(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4800(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4832(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4864(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4896(%r10)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4928(%r10)
vmovdqa 2080(%r9), %ymm7
vmovdqa 2112(%r9), %ymm8
vmovdqa 2144(%r9), %ymm9
vmovdqa 2176(%r9), %ymm10
vmovdqa 2208(%r9), %ymm11
vmovdqa 2240(%r9), %ymm12
vmovdqa 4384(%r10), %ymm14
vpmullw %ymm0, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4384(%r10)
vmovdqa 4416(%r10), %ymm14
vpmullw %ymm0, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4416(%r10)
vmovdqa 4448(%r10), %ymm14
vpmullw %ymm0, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4448(%r10)
vmovdqa 4480(%r10), %ymm14
vpmullw %ymm0, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4480(%r10)
vmovdqa 4512(%r10), %ymm14
vpmullw %ymm0, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4512(%r10)
vmovdqa 4544(%r10), %ymm14
vpmullw %ymm0, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4544(%r10)
vmovdqa 4576(%r10), %ymm14
vpmullw %ymm0, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4576(%r10)
vmovdqa 4608(%r10), %ymm14
vpmullw %ymm1, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4608(%r10)
vmovdqa 4640(%r10), %ymm14
vpmullw %ymm2, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4640(%r10)
vmovdqa 4672(%r10), %ymm14
vpmullw %ymm3, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4672(%r10)
vmovdqa 4704(%r10), %ymm14
vpmullw %ymm4, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4704(%r10)
vmovdqa 4736(%r10), %ymm14
vpmullw %ymm5, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 4736(%r10)
vmovdqa 0(%r9), %ymm0
vmovdqa 1664(%r9), %ymm7
vpaddw 416(%r9), %ymm0, %ymm0
vpaddw 2080(%r9), %ymm7, %ymm7
vmovdqa 32(%r9), %ymm1
vmovdqa 1696(%r9), %ymm8
vpaddw 448(%r9), %ymm1, %ymm1
vpaddw 2112(%r9), %ymm8, %ymm8
vmovdqa 64(%r9), %ymm2
vmovdqa 1728(%r9), %ymm9
vpaddw 480(%r9), %ymm2, %ymm2
vpaddw 2144(%r9), %ymm9, %ymm9
vmovdqa 96(%r9), %ymm3
vmovdqa 1760(%r9), %ymm10
vpaddw 512(%r9), %ymm3, %ymm3
vpaddw 2176(%r9), %ymm10, %ymm10
vmovdqa 128(%r9), %ymm4
vmovdqa 1792(%r9), %ymm11
vpaddw 544(%r9), %ymm4, %ymm4
vpaddw 2208(%r9), %ymm11, %ymm11
vmovdqa 160(%r9), %ymm5
vmovdqa 1824(%r9), %ymm12
vpaddw 576(%r9), %ymm5, %ymm5
vpaddw 2240(%r9), %ymm12, %ymm12
vmovdqa 192(%r9), %ymm6
vmovdqa 1856(%r9), %ymm13
vpaddw 608(%r9), %ymm6, %ymm6
vpaddw 2272(%r9), %ymm13, %ymm13
vpmullw %ymm0, %ymm7, %ymm14
vmovdqa %ymm14, 7424(%rsp)
vpmullw %ymm0, %ymm8, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7456(%rsp)
vpmullw %ymm0, %ymm9, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7488(%rsp)
vpmullw %ymm0, %ymm10, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7520(%rsp)
vpmullw %ymm0, %ymm11, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7552(%rsp)
vpmullw %ymm0, %ymm12, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7584(%rsp)
vpmullw %ymm0, %ymm13, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7616(%rsp)
vpmullw %ymm1, %ymm13, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7648(%rsp)
vpmullw %ymm2, %ymm13, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7680(%rsp)
vpmullw %ymm3, %ymm13, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7712(%rsp)
vpmullw %ymm4, %ymm13, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7744(%rsp)
vpmullw %ymm5, %ymm13, %ymm14
vpmullw %ymm6, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7776(%rsp)
vpmullw %ymm6, %ymm13, %ymm14
vmovdqa %ymm14, 7808(%rsp)
vmovdqa 1888(%r9), %ymm7
vpaddw 2304(%r9), %ymm7, %ymm7
vmovdqa 1920(%r9), %ymm8
vpaddw 2336(%r9), %ymm8, %ymm8
vmovdqa 1952(%r9), %ymm9
vpaddw 2368(%r9), %ymm9, %ymm9
vmovdqa 1984(%r9), %ymm10
vpaddw 2400(%r9), %ymm10, %ymm10
vmovdqa 2016(%r9), %ymm11
vpaddw 2432(%r9), %ymm11, %ymm11
vmovdqa 2048(%r9), %ymm12
vpaddw 2464(%r9), %ymm12, %ymm12
vmovdqa 7648(%rsp), %ymm14
vpmullw %ymm0, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7648(%rsp)
vmovdqa 7680(%rsp), %ymm14
vpmullw %ymm0, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7680(%rsp)
vmovdqa 7712(%rsp), %ymm14
vpmullw %ymm0, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7712(%rsp)
vmovdqa 7744(%rsp), %ymm14
vpmullw %ymm0, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7744(%rsp)
vmovdqa 7776(%rsp), %ymm14
vpmullw %ymm0, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7776(%rsp)
vmovdqa 7808(%rsp), %ymm14
vpmullw %ymm0, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7808(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7840(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7872(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7904(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7936(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7968(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm6, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8000(%rsp)
vmovdqa 224(%r9), %ymm0
vpaddw 640(%r9), %ymm0, %ymm0
vmovdqa 256(%r9), %ymm1
vpaddw 672(%r9), %ymm1, %ymm1
vmovdqa 288(%r9), %ymm2
vpaddw 704(%r9), %ymm2, %ymm2
vmovdqa 320(%r9), %ymm3
vpaddw 736(%r9), %ymm3, %ymm3
vmovdqa 352(%r9), %ymm4
vpaddw 768(%r9), %ymm4, %ymm4
vmovdqa 384(%r9), %ymm5
vpaddw 800(%r9), %ymm5, %ymm5
vmovdqa 7872(%rsp), %ymm14
vpmullw %ymm0, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7872(%rsp)
vmovdqa 7904(%rsp), %ymm14
vpmullw %ymm0, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7904(%rsp)
vmovdqa 7936(%rsp), %ymm14
vpmullw %ymm0, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7936(%rsp)
vmovdqa 7968(%rsp), %ymm14
vpmullw %ymm0, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7968(%rsp)
vmovdqa 8000(%rsp), %ymm14
vpmullw %ymm0, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8000(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm0, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8032(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8064(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8096(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8128(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8160(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8192(%rsp)
vmovdqa 1664(%r9), %ymm7
vpaddw 2080(%r9), %ymm7, %ymm7
vmovdqa 1696(%r9), %ymm8
vpaddw 2112(%r9), %ymm8, %ymm8
vmovdqa 1728(%r9), %ymm9
vpaddw 2144(%r9), %ymm9, %ymm9
vmovdqa 1760(%r9), %ymm10
vpaddw 2176(%r9), %ymm10, %ymm10
vmovdqa 1792(%r9), %ymm11
vpaddw 2208(%r9), %ymm11, %ymm11
vmovdqa 1824(%r9), %ymm12
vpaddw 2240(%r9), %ymm12, %ymm12
vmovdqa 7648(%rsp), %ymm14
vpmullw %ymm0, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7648(%rsp)
vmovdqa 7680(%rsp), %ymm14
vpmullw %ymm0, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7680(%rsp)
vmovdqa 7712(%rsp), %ymm14
vpmullw %ymm0, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7712(%rsp)
vmovdqa 7744(%rsp), %ymm14
vpmullw %ymm0, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7744(%rsp)
vmovdqa 7776(%rsp), %ymm14
vpmullw %ymm0, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7776(%rsp)
vmovdqa 7808(%rsp), %ymm14
vpmullw %ymm0, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7808(%rsp)
vmovdqa 7840(%rsp), %ymm14
vpmullw %ymm0, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7840(%rsp)
vmovdqa 7872(%rsp), %ymm14
vpmullw %ymm1, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7872(%rsp)
vmovdqa 7904(%rsp), %ymm14
vpmullw %ymm2, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7904(%rsp)
vmovdqa 7936(%rsp), %ymm14
vpmullw %ymm3, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7936(%rsp)
vmovdqa 7968(%rsp), %ymm14
vpmullw %ymm4, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7968(%rsp)
vmovdqa 8000(%rsp), %ymm14
vpmullw %ymm5, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8000(%rsp)
vmovdqa 7808(%rsp), %ymm0
vpsubw 3712(%r10), %ymm0, %ymm0
vpsubw 4544(%r10), %ymm0, %ymm0
vmovdqa %ymm0, 4128(%r10)
vmovdqa 3744(%r10), %ymm0
vpsubw 4160(%r10), %ymm0, %ymm0
vmovdqa 7840(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4576(%r10), %ymm1, %ymm1
vpsubw 3328(%r10), %ymm0, %ymm0
vpaddw 7424(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3744(%r10)
vmovdqa %ymm1, 4160(%r10)
vmovdqa 3776(%r10), %ymm0
vpsubw 4192(%r10), %ymm0, %ymm0
vmovdqa 7872(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4608(%r10), %ymm1, %ymm1
vpsubw 3360(%r10), %ymm0, %ymm0
vpaddw 7456(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3776(%r10)
vmovdqa %ymm1, 4192(%r10)
vmovdqa 3808(%r10), %ymm0
vpsubw 4224(%r10), %ymm0, %ymm0
vmovdqa 7904(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4640(%r10), %ymm1, %ymm1
vpsubw 3392(%r10), %ymm0, %ymm0
vpaddw 7488(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3808(%r10)
vmovdqa %ymm1, 4224(%r10)
vmovdqa 3840(%r10), %ymm0
vpsubw 4256(%r10), %ymm0, %ymm0
vmovdqa 7936(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4672(%r10), %ymm1, %ymm1
vpsubw 3424(%r10), %ymm0, %ymm0
vpaddw 7520(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3840(%r10)
vmovdqa %ymm1, 4256(%r10)
vmovdqa 3872(%r10), %ymm0
vpsubw 4288(%r10), %ymm0, %ymm0
vmovdqa 7968(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4704(%r10), %ymm1, %ymm1
vpsubw 3456(%r10), %ymm0, %ymm0
vpaddw 7552(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3872(%r10)
vmovdqa %ymm1, 4288(%r10)
vmovdqa 3904(%r10), %ymm0
vpsubw 4320(%r10), %ymm0, %ymm0
vmovdqa 8000(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4736(%r10), %ymm1, %ymm1
vpsubw 3488(%r10), %ymm0, %ymm0
vpaddw 7584(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3904(%r10)
vmovdqa %ymm1, 4320(%r10)
vmovdqa 3936(%r10), %ymm0
vpsubw 4352(%r10), %ymm0, %ymm0
vmovdqa 8032(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4768(%r10), %ymm1, %ymm1
vpsubw 3520(%r10), %ymm0, %ymm0
vpaddw 7616(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3936(%r10)
vmovdqa %ymm1, 4352(%r10)
vmovdqa 3968(%r10), %ymm0
vpsubw 4384(%r10), %ymm0, %ymm0
vmovdqa 8064(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4800(%r10), %ymm1, %ymm1
vpsubw 3552(%r10), %ymm0, %ymm0
vpaddw 7648(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 3968(%r10)
vmovdqa %ymm1, 4384(%r10)
vmovdqa 4000(%r10), %ymm0
vpsubw 4416(%r10), %ymm0, %ymm0
vmovdqa 8096(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4832(%r10), %ymm1, %ymm1
vpsubw 3584(%r10), %ymm0, %ymm0
vpaddw 7680(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4000(%r10)
vmovdqa %ymm1, 4416(%r10)
vmovdqa 4032(%r10), %ymm0
vpsubw 4448(%r10), %ymm0, %ymm0
vmovdqa 8128(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4864(%r10), %ymm1, %ymm1
vpsubw 3616(%r10), %ymm0, %ymm0
vpaddw 7712(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4032(%r10)
vmovdqa %ymm1, 4448(%r10)
vmovdqa 4064(%r10), %ymm0
vpsubw 4480(%r10), %ymm0, %ymm0
vmovdqa 8160(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4896(%r10), %ymm1, %ymm1
vpsubw 3648(%r10), %ymm0, %ymm0
vpaddw 7744(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4064(%r10)
vmovdqa %ymm1, 4480(%r10)
vmovdqa 4096(%r10), %ymm0
vpsubw 4512(%r10), %ymm0, %ymm0
vmovdqa 8192(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4928(%r10), %ymm1, %ymm1
vpsubw 3680(%r10), %ymm0, %ymm0
vpaddw 7776(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4096(%r10)
vmovdqa %ymm1, 4512(%r10)
neg %ecx
jns done_1
add $832, %r9
add $1664, %r10
jmp innerloop_1
done_1:
sub $832, %r9
sub $1664, %r10
vmovdqa 0(%r9), %ymm0
vpaddw 832(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8256(%rsp)
vmovdqa 1664(%r9), %ymm0
vpaddw 2496(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9088(%rsp)
vmovdqa 32(%r9), %ymm0
vpaddw 864(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8288(%rsp)
vmovdqa 1696(%r9), %ymm0
vpaddw 2528(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9120(%rsp)
vmovdqa 64(%r9), %ymm0
vpaddw 896(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8320(%rsp)
vmovdqa 1728(%r9), %ymm0
vpaddw 2560(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9152(%rsp)
vmovdqa 96(%r9), %ymm0
vpaddw 928(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8352(%rsp)
vmovdqa 1760(%r9), %ymm0
vpaddw 2592(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9184(%rsp)
vmovdqa 128(%r9), %ymm0
vpaddw 960(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8384(%rsp)
vmovdqa 1792(%r9), %ymm0
vpaddw 2624(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9216(%rsp)
vmovdqa 160(%r9), %ymm0
vpaddw 992(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8416(%rsp)
vmovdqa 1824(%r9), %ymm0
vpaddw 2656(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9248(%rsp)
vmovdqa 192(%r9), %ymm0
vpaddw 1024(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8448(%rsp)
vmovdqa 1856(%r9), %ymm0
vpaddw 2688(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9280(%rsp)
vmovdqa 224(%r9), %ymm0
vpaddw 1056(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8480(%rsp)
vmovdqa 1888(%r9), %ymm0
vpaddw 2720(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9312(%rsp)
vmovdqa 256(%r9), %ymm0
vpaddw 1088(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8512(%rsp)
vmovdqa 1920(%r9), %ymm0
vpaddw 2752(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9344(%rsp)
vmovdqa 288(%r9), %ymm0
vpaddw 1120(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8544(%rsp)
vmovdqa 1952(%r9), %ymm0
vpaddw 2784(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9376(%rsp)
vmovdqa 320(%r9), %ymm0
vpaddw 1152(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8576(%rsp)
vmovdqa 1984(%r9), %ymm0
vpaddw 2816(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9408(%rsp)
vmovdqa 352(%r9), %ymm0
vpaddw 1184(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8608(%rsp)
vmovdqa 2016(%r9), %ymm0
vpaddw 2848(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9440(%rsp)
vmovdqa 384(%r9), %ymm0
vpaddw 1216(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8640(%rsp)
vmovdqa 2048(%r9), %ymm0
vpaddw 2880(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9472(%rsp)
vmovdqa 416(%r9), %ymm0
vpaddw 1248(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8672(%rsp)
vmovdqa 2080(%r9), %ymm0
vpaddw 2912(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9504(%rsp)
vmovdqa 448(%r9), %ymm0
vpaddw 1280(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8704(%rsp)
vmovdqa 2112(%r9), %ymm0
vpaddw 2944(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9536(%rsp)
vmovdqa 480(%r9), %ymm0
vpaddw 1312(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8736(%rsp)
vmovdqa 2144(%r9), %ymm0
vpaddw 2976(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9568(%rsp)
vmovdqa 512(%r9), %ymm0
vpaddw 1344(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8768(%rsp)
vmovdqa 2176(%r9), %ymm0
vpaddw 3008(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9600(%rsp)
vmovdqa 544(%r9), %ymm0
vpaddw 1376(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8800(%rsp)
vmovdqa 2208(%r9), %ymm0
vpaddw 3040(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9632(%rsp)
vmovdqa 576(%r9), %ymm0
vpaddw 1408(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8832(%rsp)
vmovdqa 2240(%r9), %ymm0
vpaddw 3072(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9664(%rsp)
vmovdqa 608(%r9), %ymm0
vpaddw 1440(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8864(%rsp)
vmovdqa 2272(%r9), %ymm0
vpaddw 3104(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9696(%rsp)
vmovdqa 640(%r9), %ymm0
vpaddw 1472(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8896(%rsp)
vmovdqa 2304(%r9), %ymm0
vpaddw 3136(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9728(%rsp)
vmovdqa 672(%r9), %ymm0
vpaddw 1504(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8928(%rsp)
vmovdqa 2336(%r9), %ymm0
vpaddw 3168(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9760(%rsp)
vmovdqa 704(%r9), %ymm0
vpaddw 1536(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8960(%rsp)
vmovdqa 2368(%r9), %ymm0
vpaddw 3200(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9792(%rsp)
vmovdqa 736(%r9), %ymm0
vpaddw 1568(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 8992(%rsp)
vmovdqa 2400(%r9), %ymm0
vpaddw 3232(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9824(%rsp)
vmovdqa 768(%r9), %ymm0
vpaddw 1600(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9024(%rsp)
vmovdqa 2432(%r9), %ymm0
vpaddw 3264(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9856(%rsp)
vmovdqa 800(%r9), %ymm0
vpaddw 1632(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9056(%rsp)
vmovdqa 2464(%r9), %ymm0
vpaddw 3296(%r9), %ymm0, %ymm0
vmovdqa %ymm0, 9888(%rsp)
vmovdqa 8256(%rsp), %ymm0
vmovdqa 9088(%rsp), %ymm7
vmovdqa 8288(%rsp), %ymm1
vmovdqa 9120(%rsp), %ymm8
vmovdqa 8320(%rsp), %ymm2
vmovdqa 9152(%rsp), %ymm9
vmovdqa 8352(%rsp), %ymm3
vmovdqa 9184(%rsp), %ymm10
vmovdqa 8384(%rsp), %ymm4
vmovdqa 9216(%rsp), %ymm11
vmovdqa 8416(%rsp), %ymm5
vmovdqa 9248(%rsp), %ymm12
vmovdqa 8448(%rsp), %ymm6
vmovdqa 9280(%rsp), %ymm13
vpmullw %ymm0, %ymm7, %ymm14
vmovdqa %ymm14, 9920(%rsp)
vpmullw %ymm0, %ymm8, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 9952(%rsp)
vpmullw %ymm0, %ymm9, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 9984(%rsp)
vpmullw %ymm0, %ymm10, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10016(%rsp)
vpmullw %ymm0, %ymm11, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10048(%rsp)
vpmullw %ymm0, %ymm12, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10080(%rsp)
vpmullw %ymm0, %ymm13, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10112(%rsp)
vpmullw %ymm1, %ymm13, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10144(%rsp)
vpmullw %ymm2, %ymm13, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10176(%rsp)
vpmullw %ymm3, %ymm13, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10208(%rsp)
vpmullw %ymm4, %ymm13, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10240(%rsp)
vpmullw %ymm5, %ymm13, %ymm14
vpmullw %ymm6, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10272(%rsp)
vpmullw %ymm6, %ymm13, %ymm14
vmovdqa %ymm14, 10304(%rsp)
vmovdqa 9312(%rsp), %ymm7
vmovdqa 9344(%rsp), %ymm8
vmovdqa 9376(%rsp), %ymm9
vmovdqa 9408(%rsp), %ymm10
vmovdqa 9440(%rsp), %ymm11
vmovdqa 9472(%rsp), %ymm12
vmovdqa 10144(%rsp), %ymm14
vpmullw %ymm0, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10144(%rsp)
vmovdqa 10176(%rsp), %ymm14
vpmullw %ymm0, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10176(%rsp)
vmovdqa 10208(%rsp), %ymm14
vpmullw %ymm0, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10208(%rsp)
vmovdqa 10240(%rsp), %ymm14
vpmullw %ymm0, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10240(%rsp)
vmovdqa 10272(%rsp), %ymm14
vpmullw %ymm0, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10272(%rsp)
vmovdqa 10304(%rsp), %ymm14
vpmullw %ymm0, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10304(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10336(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10368(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10400(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10432(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10464(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm6, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10496(%rsp)
vmovdqa 8480(%rsp), %ymm0
vmovdqa 8512(%rsp), %ymm1
vmovdqa 8544(%rsp), %ymm2
vmovdqa 8576(%rsp), %ymm3
vmovdqa 8608(%rsp), %ymm4
vmovdqa 8640(%rsp), %ymm5
vmovdqa 10368(%rsp), %ymm14
vpmullw %ymm0, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10368(%rsp)
vmovdqa 10400(%rsp), %ymm14
vpmullw %ymm0, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10400(%rsp)
vmovdqa 10432(%rsp), %ymm14
vpmullw %ymm0, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10432(%rsp)
vmovdqa 10464(%rsp), %ymm14
vpmullw %ymm0, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10464(%rsp)
vmovdqa 10496(%rsp), %ymm14
vpmullw %ymm0, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10496(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm0, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10528(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10560(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10592(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10624(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10656(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10688(%rsp)
vmovdqa 9088(%rsp), %ymm7
vmovdqa 9120(%rsp), %ymm8
vmovdqa 9152(%rsp), %ymm9
vmovdqa 9184(%rsp), %ymm10
vmovdqa 9216(%rsp), %ymm11
vmovdqa 9248(%rsp), %ymm12
vmovdqa 10144(%rsp), %ymm14
vpmullw %ymm0, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10144(%rsp)
vmovdqa 10176(%rsp), %ymm14
vpmullw %ymm0, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10176(%rsp)
vmovdqa 10208(%rsp), %ymm14
vpmullw %ymm0, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10208(%rsp)
vmovdqa 10240(%rsp), %ymm14
vpmullw %ymm0, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10240(%rsp)
vmovdqa 10272(%rsp), %ymm14
vpmullw %ymm0, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10272(%rsp)
vmovdqa 10304(%rsp), %ymm14
vpmullw %ymm0, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10304(%rsp)
vmovdqa 10336(%rsp), %ymm14
vpmullw %ymm0, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10336(%rsp)
vmovdqa 10368(%rsp), %ymm14
vpmullw %ymm1, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10368(%rsp)
vmovdqa 10400(%rsp), %ymm14
vpmullw %ymm2, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10400(%rsp)
vmovdqa 10432(%rsp), %ymm14
vpmullw %ymm3, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10432(%rsp)
vmovdqa 10464(%rsp), %ymm14
vpmullw %ymm4, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10464(%rsp)
vmovdqa 10496(%rsp), %ymm14
vpmullw %ymm5, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10496(%rsp)
vmovdqa 8672(%rsp), %ymm0
vmovdqa 9504(%rsp), %ymm7
vmovdqa 8704(%rsp), %ymm1
vmovdqa 9536(%rsp), %ymm8
vmovdqa 8736(%rsp), %ymm2
vmovdqa 9568(%rsp), %ymm9
vmovdqa 8768(%rsp), %ymm3
vmovdqa 9600(%rsp), %ymm10
vmovdqa 8800(%rsp), %ymm4
vmovdqa 9632(%rsp), %ymm11
vmovdqa 8832(%rsp), %ymm5
vmovdqa 9664(%rsp), %ymm12
vmovdqa 8864(%rsp), %ymm6
vmovdqa 9696(%rsp), %ymm13
vpmullw %ymm0, %ymm7, %ymm14
vmovdqa %ymm14, 10752(%rsp)
vpmullw %ymm0, %ymm8, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10784(%rsp)
vpmullw %ymm0, %ymm9, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10816(%rsp)
vpmullw %ymm0, %ymm10, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10848(%rsp)
vpmullw %ymm0, %ymm11, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10880(%rsp)
vpmullw %ymm0, %ymm12, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10912(%rsp)
vpmullw %ymm0, %ymm13, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10944(%rsp)
vpmullw %ymm1, %ymm13, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10976(%rsp)
vpmullw %ymm2, %ymm13, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11008(%rsp)
vpmullw %ymm3, %ymm13, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11040(%rsp)
vpmullw %ymm4, %ymm13, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11072(%rsp)
vpmullw %ymm5, %ymm13, %ymm14
vpmullw %ymm6, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11104(%rsp)
vpmullw %ymm6, %ymm13, %ymm14
vmovdqa %ymm14, 11136(%rsp)
vmovdqa 9728(%rsp), %ymm7
vmovdqa 9760(%rsp), %ymm8
vmovdqa 9792(%rsp), %ymm9
vmovdqa 9824(%rsp), %ymm10
vmovdqa 9856(%rsp), %ymm11
vmovdqa 9888(%rsp), %ymm12
vmovdqa 10976(%rsp), %ymm14
vpmullw %ymm0, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10976(%rsp)
vmovdqa 11008(%rsp), %ymm14
vpmullw %ymm0, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11008(%rsp)
vmovdqa 11040(%rsp), %ymm14
vpmullw %ymm0, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11040(%rsp)
vmovdqa 11072(%rsp), %ymm14
vpmullw %ymm0, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11072(%rsp)
vmovdqa 11104(%rsp), %ymm14
vpmullw %ymm0, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11104(%rsp)
vmovdqa 11136(%rsp), %ymm14
vpmullw %ymm0, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11136(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11168(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11200(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11232(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11264(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11296(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm6, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11328(%rsp)
vmovdqa 8896(%rsp), %ymm0
vmovdqa 8928(%rsp), %ymm1
vmovdqa 8960(%rsp), %ymm2
vmovdqa 8992(%rsp), %ymm3
vmovdqa 9024(%rsp), %ymm4
vmovdqa 9056(%rsp), %ymm5
vmovdqa 11200(%rsp), %ymm14
vpmullw %ymm0, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11200(%rsp)
vmovdqa 11232(%rsp), %ymm14
vpmullw %ymm0, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11232(%rsp)
vmovdqa 11264(%rsp), %ymm14
vpmullw %ymm0, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11264(%rsp)
vmovdqa 11296(%rsp), %ymm14
vpmullw %ymm0, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11296(%rsp)
vmovdqa 11328(%rsp), %ymm14
vpmullw %ymm0, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11328(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm0, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11360(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11392(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11424(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11456(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11488(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11520(%rsp)
vmovdqa 9504(%rsp), %ymm7
vmovdqa 9536(%rsp), %ymm8
vmovdqa 9568(%rsp), %ymm9
vmovdqa 9600(%rsp), %ymm10
vmovdqa 9632(%rsp), %ymm11
vmovdqa 9664(%rsp), %ymm12
vmovdqa 10976(%rsp), %ymm14
vpmullw %ymm0, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 10976(%rsp)
vmovdqa 11008(%rsp), %ymm14
vpmullw %ymm0, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11008(%rsp)
vmovdqa 11040(%rsp), %ymm14
vpmullw %ymm0, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11040(%rsp)
vmovdqa 11072(%rsp), %ymm14
vpmullw %ymm0, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11072(%rsp)
vmovdqa 11104(%rsp), %ymm14
vpmullw %ymm0, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11104(%rsp)
vmovdqa 11136(%rsp), %ymm14
vpmullw %ymm0, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11136(%rsp)
vmovdqa 11168(%rsp), %ymm14
vpmullw %ymm0, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11168(%rsp)
vmovdqa 11200(%rsp), %ymm14
vpmullw %ymm1, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11200(%rsp)
vmovdqa 11232(%rsp), %ymm14
vpmullw %ymm2, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11232(%rsp)
vmovdqa 11264(%rsp), %ymm14
vpmullw %ymm3, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11264(%rsp)
vmovdqa 11296(%rsp), %ymm14
vpmullw %ymm4, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11296(%rsp)
vmovdqa 11328(%rsp), %ymm14
vpmullw %ymm5, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 11328(%rsp)
vmovdqa 8256(%rsp), %ymm0
vmovdqa 9088(%rsp), %ymm7
vpaddw 8672(%rsp), %ymm0, %ymm0
vpaddw 9504(%rsp), %ymm7, %ymm7
vmovdqa 8288(%rsp), %ymm1
vmovdqa 9120(%rsp), %ymm8
vpaddw 8704(%rsp), %ymm1, %ymm1
vpaddw 9536(%rsp), %ymm8, %ymm8
vmovdqa 8320(%rsp), %ymm2
vmovdqa 9152(%rsp), %ymm9
vpaddw 8736(%rsp), %ymm2, %ymm2
vpaddw 9568(%rsp), %ymm9, %ymm9
vmovdqa 8352(%rsp), %ymm3
vmovdqa 9184(%rsp), %ymm10
vpaddw 8768(%rsp), %ymm3, %ymm3
vpaddw 9600(%rsp), %ymm10, %ymm10
vmovdqa 8384(%rsp), %ymm4
vmovdqa 9216(%rsp), %ymm11
vpaddw 8800(%rsp), %ymm4, %ymm4
vpaddw 9632(%rsp), %ymm11, %ymm11
vmovdqa 8416(%rsp), %ymm5
vmovdqa 9248(%rsp), %ymm12
vpaddw 8832(%rsp), %ymm5, %ymm5
vpaddw 9664(%rsp), %ymm12, %ymm12
vmovdqa 8448(%rsp), %ymm6
vmovdqa 9280(%rsp), %ymm13
vpaddw 8864(%rsp), %ymm6, %ymm6
vpaddw 9696(%rsp), %ymm13, %ymm13
vpmullw %ymm0, %ymm7, %ymm14
vmovdqa %ymm14, 7424(%rsp)
vpmullw %ymm0, %ymm8, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7456(%rsp)
vpmullw %ymm0, %ymm9, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7488(%rsp)
vpmullw %ymm0, %ymm10, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7520(%rsp)
vpmullw %ymm0, %ymm11, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7552(%rsp)
vpmullw %ymm0, %ymm12, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7584(%rsp)
vpmullw %ymm0, %ymm13, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7616(%rsp)
vpmullw %ymm1, %ymm13, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7648(%rsp)
vpmullw %ymm2, %ymm13, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7680(%rsp)
vpmullw %ymm3, %ymm13, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7712(%rsp)
vpmullw %ymm4, %ymm13, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7744(%rsp)
vpmullw %ymm5, %ymm13, %ymm14
vpmullw %ymm6, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7776(%rsp)
vpmullw %ymm6, %ymm13, %ymm14
vmovdqa %ymm14, 7808(%rsp)
vmovdqa 9312(%rsp), %ymm7
vpaddw 9728(%rsp), %ymm7, %ymm7
vmovdqa 9344(%rsp), %ymm8
vpaddw 9760(%rsp), %ymm8, %ymm8
vmovdqa 9376(%rsp), %ymm9
vpaddw 9792(%rsp), %ymm9, %ymm9
vmovdqa 9408(%rsp), %ymm10
vpaddw 9824(%rsp), %ymm10, %ymm10
vmovdqa 9440(%rsp), %ymm11
vpaddw 9856(%rsp), %ymm11, %ymm11
vmovdqa 9472(%rsp), %ymm12
vpaddw 9888(%rsp), %ymm12, %ymm12
vmovdqa 7648(%rsp), %ymm14
vpmullw %ymm0, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7648(%rsp)
vmovdqa 7680(%rsp), %ymm14
vpmullw %ymm0, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7680(%rsp)
vmovdqa 7712(%rsp), %ymm14
vpmullw %ymm0, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7712(%rsp)
vmovdqa 7744(%rsp), %ymm14
vpmullw %ymm0, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7744(%rsp)
vmovdqa 7776(%rsp), %ymm14
vpmullw %ymm0, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7776(%rsp)
vmovdqa 7808(%rsp), %ymm14
vpmullw %ymm0, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7808(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7840(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7872(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7904(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7936(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm6, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7968(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm6, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8000(%rsp)
vmovdqa 8480(%rsp), %ymm0
vpaddw 8896(%rsp), %ymm0, %ymm0
vmovdqa 8512(%rsp), %ymm1
vpaddw 8928(%rsp), %ymm1, %ymm1
vmovdqa 8544(%rsp), %ymm2
vpaddw 8960(%rsp), %ymm2, %ymm2
vmovdqa 8576(%rsp), %ymm3
vpaddw 8992(%rsp), %ymm3, %ymm3
vmovdqa 8608(%rsp), %ymm4
vpaddw 9024(%rsp), %ymm4, %ymm4
vmovdqa 8640(%rsp), %ymm5
vpaddw 9056(%rsp), %ymm5, %ymm5
vmovdqa 7872(%rsp), %ymm14
vpmullw %ymm0, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7872(%rsp)
vmovdqa 7904(%rsp), %ymm14
vpmullw %ymm0, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7904(%rsp)
vmovdqa 7936(%rsp), %ymm14
vpmullw %ymm0, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7936(%rsp)
vmovdqa 7968(%rsp), %ymm14
vpmullw %ymm0, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7968(%rsp)
vmovdqa 8000(%rsp), %ymm14
vpmullw %ymm0, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8000(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm0, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8032(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8064(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8096(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8128(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8160(%rsp)
vpxor %ymm14, %ymm14, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8192(%rsp)
vmovdqa 9088(%rsp), %ymm7
vpaddw 9504(%rsp), %ymm7, %ymm7
vmovdqa 9120(%rsp), %ymm8
vpaddw 9536(%rsp), %ymm8, %ymm8
vmovdqa 9152(%rsp), %ymm9
vpaddw 9568(%rsp), %ymm9, %ymm9
vmovdqa 9184(%rsp), %ymm10
vpaddw 9600(%rsp), %ymm10, %ymm10
vmovdqa 9216(%rsp), %ymm11
vpaddw 9632(%rsp), %ymm11, %ymm11
vmovdqa 9248(%rsp), %ymm12
vpaddw 9664(%rsp), %ymm12, %ymm12
vmovdqa 7648(%rsp), %ymm14
vpmullw %ymm0, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7648(%rsp)
vmovdqa 7680(%rsp), %ymm14
vpmullw %ymm0, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7680(%rsp)
vmovdqa 7712(%rsp), %ymm14
vpmullw %ymm0, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7712(%rsp)
vmovdqa 7744(%rsp), %ymm14
vpmullw %ymm0, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7744(%rsp)
vmovdqa 7776(%rsp), %ymm14
vpmullw %ymm0, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7776(%rsp)
vmovdqa 7808(%rsp), %ymm14
vpmullw %ymm0, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm7, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7808(%rsp)
vmovdqa 7840(%rsp), %ymm14
vpmullw %ymm0, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm1, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm8, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7840(%rsp)
vmovdqa 7872(%rsp), %ymm14
vpmullw %ymm1, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm2, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm9, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7872(%rsp)
vmovdqa 7904(%rsp), %ymm14
vpmullw %ymm2, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm3, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm10, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7904(%rsp)
vmovdqa 7936(%rsp), %ymm14
vpmullw %ymm3, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm4, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm11, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7936(%rsp)
vmovdqa 7968(%rsp), %ymm14
vpmullw %ymm4, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vpmullw %ymm5, %ymm12, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 7968(%rsp)
vmovdqa 8000(%rsp), %ymm14
vpmullw %ymm5, %ymm13, %ymm15
vpaddw %ymm14, %ymm15, %ymm14
vmovdqa %ymm14, 8000(%rsp)
vmovdqa 10336(%rsp), %ymm0
vpsubw 10752(%rsp), %ymm0, %ymm0
vmovdqa 7840(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 11168(%rsp), %ymm1, %ymm4
vpsubw 9920(%rsp), %ymm0, %ymm0
vpaddw 7424(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 10336(%rsp)
vmovdqa 10368(%rsp), %ymm0
vpsubw 10784(%rsp), %ymm0, %ymm0
vmovdqa 7872(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 11200(%rsp), %ymm1, %ymm5
vpsubw 9952(%rsp), %ymm0, %ymm0
vpaddw 7456(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 10368(%rsp)
vmovdqa 10400(%rsp), %ymm0
vpsubw 10816(%rsp), %ymm0, %ymm0
vmovdqa 7904(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 11232(%rsp), %ymm1, %ymm6
vpsubw 9984(%rsp), %ymm0, %ymm0
vpaddw 7488(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 10400(%rsp)
vmovdqa 10432(%rsp), %ymm0
vpsubw 10848(%rsp), %ymm0, %ymm0
vmovdqa 7936(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 11264(%rsp), %ymm1, %ymm7
vpsubw 10016(%rsp), %ymm0, %ymm0
vpaddw 7520(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 10432(%rsp)
vmovdqa 10464(%rsp), %ymm0
vpsubw 10880(%rsp), %ymm0, %ymm0
vmovdqa 7968(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 11296(%rsp), %ymm1, %ymm8
vpsubw 10048(%rsp), %ymm0, %ymm0
vpaddw 7552(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 10464(%rsp)
vmovdqa 10496(%rsp), %ymm0
vpsubw 10912(%rsp), %ymm0, %ymm0
vmovdqa 8000(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 11328(%rsp), %ymm1, %ymm9
vpsubw 10080(%rsp), %ymm0, %ymm0
vpaddw 7584(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 10496(%rsp)
vmovdqa 10528(%rsp), %ymm0
vpsubw 10944(%rsp), %ymm0, %ymm0
vmovdqa 8032(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 11360(%rsp), %ymm1, %ymm10
vpsubw 10112(%rsp), %ymm0, %ymm0
vpaddw 7616(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 10528(%rsp)
vmovdqa 10560(%rsp), %ymm0
vpsubw 10976(%rsp), %ymm0, %ymm0
vmovdqa 8064(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 11392(%rsp), %ymm1, %ymm11
vpsubw 10144(%rsp), %ymm0, %ymm0
vpaddw 7648(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 10560(%rsp)
vmovdqa 10592(%rsp), %ymm0
vpsubw 11008(%rsp), %ymm0, %ymm0
vmovdqa 8096(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 11424(%rsp), %ymm1, %ymm12
vpsubw 10176(%rsp), %ymm0, %ymm0
vpaddw 7680(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 10592(%rsp)
vmovdqa 10624(%rsp), %ymm0
vpsubw 11040(%rsp), %ymm0, %ymm0
vmovdqa 8128(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 11456(%rsp), %ymm1, %ymm13
vpsubw 10208(%rsp), %ymm0, %ymm0
vpaddw 7712(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 10624(%rsp)
vmovdqa 10656(%rsp), %ymm0
vpsubw 11072(%rsp), %ymm0, %ymm0
vmovdqa 8160(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 11488(%rsp), %ymm1, %ymm14
vpsubw 10240(%rsp), %ymm0, %ymm0
vpaddw 7744(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 10656(%rsp)
vmovdqa 10688(%rsp), %ymm0
vpsubw 11104(%rsp), %ymm0, %ymm0
vmovdqa 8192(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 11520(%rsp), %ymm1, %ymm15
vpsubw 10272(%rsp), %ymm0, %ymm0
vpaddw 7776(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 10688(%rsp)
vmovdqa 7808(%rsp), %ymm0
vpsubw 10304(%rsp), %ymm0, %ymm0
vpsubw 11136(%rsp), %ymm0, %ymm0
vpsubw 4128(%r10), %ymm0, %ymm0
vpsubw 5792(%r10), %ymm0, %ymm0
vmovdqa %ymm0, 4960(%r10)
vmovdqa 4160(%r10), %ymm0
vpsubw 4992(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm4, %ymm4
vpsubw 5824(%r10), %ymm4, %ymm4
vpsubw 3328(%r10), %ymm0, %ymm0
vpaddw 9920(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4160(%r10)
vmovdqa %ymm4, 4992(%r10)
vmovdqa 4192(%r10), %ymm0
vpsubw 5024(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm5, %ymm5
vpsubw 5856(%r10), %ymm5, %ymm5
vpsubw 3360(%r10), %ymm0, %ymm0
vpaddw 9952(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4192(%r10)
vmovdqa %ymm5, 5024(%r10)
vmovdqa 4224(%r10), %ymm0
vpsubw 5056(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm6, %ymm6
vpsubw 5888(%r10), %ymm6, %ymm6
vpsubw 3392(%r10), %ymm0, %ymm0
vpaddw 9984(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4224(%r10)
vmovdqa %ymm6, 5056(%r10)
vmovdqa 4256(%r10), %ymm0
vpsubw 5088(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm7, %ymm7
vpsubw 5920(%r10), %ymm7, %ymm7
vpsubw 3424(%r10), %ymm0, %ymm0
vpaddw 10016(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4256(%r10)
vmovdqa %ymm7, 5088(%r10)
vmovdqa 4288(%r10), %ymm0
vpsubw 5120(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm8, %ymm8
vpsubw 5952(%r10), %ymm8, %ymm8
vpsubw 3456(%r10), %ymm0, %ymm0
vpaddw 10048(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4288(%r10)
vmovdqa %ymm8, 5120(%r10)
vmovdqa 4320(%r10), %ymm0
vpsubw 5152(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm9, %ymm9
vpsubw 5984(%r10), %ymm9, %ymm9
vpsubw 3488(%r10), %ymm0, %ymm0
vpaddw 10080(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4320(%r10)
vmovdqa %ymm9, 5152(%r10)
vmovdqa 4352(%r10), %ymm0
vpsubw 5184(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm10, %ymm10
vpsubw 6016(%r10), %ymm10, %ymm10
vpsubw 3520(%r10), %ymm0, %ymm0
vpaddw 10112(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4352(%r10)
vmovdqa %ymm10, 5184(%r10)
vmovdqa 4384(%r10), %ymm0
vpsubw 5216(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm11, %ymm11
vpsubw 6048(%r10), %ymm11, %ymm11
vpsubw 3552(%r10), %ymm0, %ymm0
vpaddw 10144(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4384(%r10)
vmovdqa %ymm11, 5216(%r10)
vmovdqa 4416(%r10), %ymm0
vpsubw 5248(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm12, %ymm12
vpsubw 6080(%r10), %ymm12, %ymm12
vpsubw 3584(%r10), %ymm0, %ymm0
vpaddw 10176(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4416(%r10)
vmovdqa %ymm12, 5248(%r10)
vmovdqa 4448(%r10), %ymm0
vpsubw 5280(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm13, %ymm13
vpsubw 6112(%r10), %ymm13, %ymm13
vpsubw 3616(%r10), %ymm0, %ymm0
vpaddw 10208(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4448(%r10)
vmovdqa %ymm13, 5280(%r10)
vmovdqa 4480(%r10), %ymm0
vpsubw 5312(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm14, %ymm14
vpsubw 6144(%r10), %ymm14, %ymm14
vpsubw 3648(%r10), %ymm0, %ymm0
vpaddw 10240(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4480(%r10)
vmovdqa %ymm14, 5312(%r10)
vmovdqa 4512(%r10), %ymm0
vpsubw 5344(%r10), %ymm0, %ymm0
vpsubw %ymm0, %ymm15, %ymm15
vpsubw 6176(%r10), %ymm15, %ymm15
vpsubw 3680(%r10), %ymm0, %ymm0
vpaddw 10272(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4512(%r10)
vmovdqa %ymm15, 5344(%r10)
vmovdqa 4544(%r10), %ymm0
vpsubw 5376(%r10), %ymm0, %ymm0
vmovdqa 11136(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6208(%r10), %ymm1, %ymm1
vpsubw 3712(%r10), %ymm0, %ymm0
vpaddw 10304(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4544(%r10)
vmovdqa %ymm1, 5376(%r10)
vmovdqa 4576(%r10), %ymm0
vpsubw 5408(%r10), %ymm0, %ymm0
vmovdqa 11168(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6240(%r10), %ymm1, %ymm1
vpsubw 3744(%r10), %ymm0, %ymm0
vpaddw 10336(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4576(%r10)
vmovdqa %ymm1, 5408(%r10)
vmovdqa 4608(%r10), %ymm0
vpsubw 5440(%r10), %ymm0, %ymm0
vmovdqa 11200(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6272(%r10), %ymm1, %ymm1
vpsubw 3776(%r10), %ymm0, %ymm0
vpaddw 10368(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4608(%r10)
vmovdqa %ymm1, 5440(%r10)
vmovdqa 4640(%r10), %ymm0
vpsubw 5472(%r10), %ymm0, %ymm0
vmovdqa 11232(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6304(%r10), %ymm1, %ymm1
vpsubw 3808(%r10), %ymm0, %ymm0
vpaddw 10400(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4640(%r10)
vmovdqa %ymm1, 5472(%r10)
vmovdqa 4672(%r10), %ymm0
vpsubw 5504(%r10), %ymm0, %ymm0
vmovdqa 11264(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6336(%r10), %ymm1, %ymm1
vpsubw 3840(%r10), %ymm0, %ymm0
vpaddw 10432(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4672(%r10)
vmovdqa %ymm1, 5504(%r10)
vmovdqa 4704(%r10), %ymm0
vpsubw 5536(%r10), %ymm0, %ymm0
vmovdqa 11296(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6368(%r10), %ymm1, %ymm1
vpsubw 3872(%r10), %ymm0, %ymm0
vpaddw 10464(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4704(%r10)
vmovdqa %ymm1, 5536(%r10)
vmovdqa 4736(%r10), %ymm0
vpsubw 5568(%r10), %ymm0, %ymm0
vmovdqa 11328(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6400(%r10), %ymm1, %ymm1
vpsubw 3904(%r10), %ymm0, %ymm0
vpaddw 10496(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4736(%r10)
vmovdqa %ymm1, 5568(%r10)
vmovdqa 4768(%r10), %ymm0
vpsubw 5600(%r10), %ymm0, %ymm0
vmovdqa 11360(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6432(%r10), %ymm1, %ymm1
vpsubw 3936(%r10), %ymm0, %ymm0
vpaddw 10528(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4768(%r10)
vmovdqa %ymm1, 5600(%r10)
vmovdqa 4800(%r10), %ymm0
vpsubw 5632(%r10), %ymm0, %ymm0
vmovdqa 11392(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6464(%r10), %ymm1, %ymm1
vpsubw 3968(%r10), %ymm0, %ymm0
vpaddw 10560(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4800(%r10)
vmovdqa %ymm1, 5632(%r10)
vmovdqa 4832(%r10), %ymm0
vpsubw 5664(%r10), %ymm0, %ymm0
vmovdqa 11424(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6496(%r10), %ymm1, %ymm1
vpsubw 4000(%r10), %ymm0, %ymm0
vpaddw 10592(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4832(%r10)
vmovdqa %ymm1, 5664(%r10)
vmovdqa 4864(%r10), %ymm0
vpsubw 5696(%r10), %ymm0, %ymm0
vmovdqa 11456(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6528(%r10), %ymm1, %ymm1
vpsubw 4032(%r10), %ymm0, %ymm0
vpaddw 10624(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4864(%r10)
vmovdqa %ymm1, 5696(%r10)
vmovdqa 4896(%r10), %ymm0
vpsubw 5728(%r10), %ymm0, %ymm0
vmovdqa 11488(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6560(%r10), %ymm1, %ymm1
vpsubw 4064(%r10), %ymm0, %ymm0
vpaddw 10656(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4896(%r10)
vmovdqa %ymm1, 5728(%r10)
vmovdqa 4928(%r10), %ymm0
vpsubw 5760(%r10), %ymm0, %ymm0
vmovdqa 11520(%rsp), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 6592(%r10), %ymm1, %ymm1
vpsubw 4096(%r10), %ymm0, %ymm0
vpaddw 10688(%rsp), %ymm0, %ymm0
vmovdqa %ymm0, 4928(%r10)
vmovdqa %ymm1, 5760(%r10)
vpxor %ymm1, %ymm1, %ymm1
vmovdqa %ymm1, 6624(%r10)
subq $32, %rsp
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
vmovdqa %ymm15, 0(%r12)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 256(%r12)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 512(%r12)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 768(%r12)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 1024(%r12)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 1280(%r12)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 1536(%r12)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 2048(%r12)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 2304(%r12)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 2560(%r12)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 2816(%r12)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 3072(%r12)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 3328(%r12)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 3584(%r12)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1792(%r12)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 3840(%r12)
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
vmovdqa %ymm15, 32(%r12)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 288(%r12)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 544(%r12)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 800(%r12)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 1056(%r12)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 1312(%r12)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 1568(%r12)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 2080(%r12)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 2336(%r12)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 2592(%r12)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 2848(%r12)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 3104(%r12)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 3360(%r12)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 3616(%r12)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1824(%r12)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 3872(%r12)
vmovdqa 4352(%r10), %ymm0
vmovdqa 4416(%r10), %ymm1
vmovdqa 4480(%r10), %ymm2
vmovdqa 4544(%r10), %ymm3
vpunpcklwd 4384(%r10), %ymm0, %ymm4
vpunpckhwd 4384(%r10), %ymm0, %ymm5
vpunpcklwd 4448(%r10), %ymm1, %ymm6
vpunpckhwd 4448(%r10), %ymm1, %ymm7
vpunpcklwd 4512(%r10), %ymm2, %ymm8
vpunpckhwd 4512(%r10), %ymm2, %ymm9
vpunpcklwd 4576(%r10), %ymm3, %ymm10
vpunpckhwd 4576(%r10), %ymm3, %ymm11
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
vmovdqa 4608(%r10), %ymm0
vmovdqa 4672(%r10), %ymm1
vmovdqa 4736(%r10), %ymm2
vmovdqa 4800(%r10), %ymm3
vpunpcklwd 4640(%r10), %ymm0, %ymm12
vpunpckhwd 4640(%r10), %ymm0, %ymm13
vpunpcklwd 4704(%r10), %ymm1, %ymm14
vpunpckhwd 4704(%r10), %ymm1, %ymm15
vpunpcklwd 4768(%r10), %ymm2, %ymm0
vpunpckhwd 4768(%r10), %ymm2, %ymm1
vpunpcklwd 4832(%r10), %ymm3, %ymm2
vpunpckhwd 4832(%r10), %ymm3, %ymm3
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
vmovdqa %ymm15, 320(%r12)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 576(%r12)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 832(%r12)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 1088(%r12)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 1344(%r12)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 1600(%r12)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 2112(%r12)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 2368(%r12)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 2624(%r12)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 2880(%r12)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 3136(%r12)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 3392(%r12)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 3648(%r12)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1856(%r12)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 3904(%r12)
vmovdqa 4864(%r10), %ymm0
vmovdqa 4928(%r10), %ymm1
vmovdqa 4992(%r10), %ymm2
vmovdqa 5056(%r10), %ymm3
vpunpcklwd 4896(%r10), %ymm0, %ymm4
vpunpckhwd 4896(%r10), %ymm0, %ymm5
vpunpcklwd 4960(%r10), %ymm1, %ymm6
vpunpckhwd 4960(%r10), %ymm1, %ymm7
vpunpcklwd 5024(%r10), %ymm2, %ymm8
vpunpckhwd 5024(%r10), %ymm2, %ymm9
vpunpcklwd 5088(%r10), %ymm3, %ymm10
vpunpckhwd 5088(%r10), %ymm3, %ymm11
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
vmovdqa 5120(%r10), %ymm0
vmovdqa 5184(%r10), %ymm1
vmovdqa 5248(%r10), %ymm2
vmovdqa 5312(%r10), %ymm3
vpunpcklwd 5152(%r10), %ymm0, %ymm12
vpunpckhwd 5152(%r10), %ymm0, %ymm13
vpunpcklwd 5216(%r10), %ymm1, %ymm14
vpunpckhwd 5216(%r10), %ymm1, %ymm15
vpunpcklwd 5280(%r10), %ymm2, %ymm0
vpunpckhwd 5280(%r10), %ymm2, %ymm1
vpunpcklwd 5344(%r10), %ymm3, %ymm2
vpunpckhwd 5344(%r10), %ymm3, %ymm3
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
vmovdqa %ymm15, 352(%r12)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 608(%r12)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 864(%r12)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 1120(%r12)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 1376(%r12)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 1632(%r12)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 2144(%r12)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 2400(%r12)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 2656(%r12)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 2912(%r12)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 3168(%r12)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 3424(%r12)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 3680(%r12)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1888(%r12)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 3936(%r12)
vmovdqa 4992(%r10), %ymm0
vmovdqa 5056(%r10), %ymm1
vmovdqa 5120(%r10), %ymm2
vmovdqa 5184(%r10), %ymm3
vpunpcklwd 5024(%r10), %ymm0, %ymm4
vpunpckhwd 5024(%r10), %ymm0, %ymm5
vpunpcklwd 5088(%r10), %ymm1, %ymm6
vpunpckhwd 5088(%r10), %ymm1, %ymm7
vpunpcklwd 5152(%r10), %ymm2, %ymm8
vpunpckhwd 5152(%r10), %ymm2, %ymm9
vpunpcklwd 5216(%r10), %ymm3, %ymm10
vpunpckhwd 5216(%r10), %ymm3, %ymm11
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
vmovdqa 5248(%r10), %ymm0
vmovdqa 5312(%r10), %ymm1
vmovdqa 5376(%r10), %ymm2
vmovdqa 5440(%r10), %ymm3
vpunpcklwd 5280(%r10), %ymm0, %ymm12
vpunpckhwd 5280(%r10), %ymm0, %ymm13
vpunpcklwd 5344(%r10), %ymm1, %ymm14
vpunpckhwd 5344(%r10), %ymm1, %ymm15
vpunpcklwd 5408(%r10), %ymm2, %ymm0
vpunpckhwd 5408(%r10), %ymm2, %ymm1
vpunpcklwd 5472(%r10), %ymm3, %ymm2
vpunpckhwd 5472(%r10), %ymm3, %ymm3
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
vmovdqa %ymm15, 384(%r12)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 640(%r12)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 896(%r12)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 1152(%r12)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 1408(%r12)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 1664(%r12)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 2176(%r12)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 2432(%r12)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 2688(%r12)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 2944(%r12)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 3200(%r12)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 3456(%r12)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 3712(%r12)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1920(%r12)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 3968(%r12)
vmovdqa 5504(%r10), %ymm0
vmovdqa 5568(%r10), %ymm1
vmovdqa 5632(%r10), %ymm2
vmovdqa 5696(%r10), %ymm3
vpunpcklwd 5536(%r10), %ymm0, %ymm4
vpunpckhwd 5536(%r10), %ymm0, %ymm5
vpunpcklwd 5600(%r10), %ymm1, %ymm6
vpunpckhwd 5600(%r10), %ymm1, %ymm7
vpunpcklwd 5664(%r10), %ymm2, %ymm8
vpunpckhwd 5664(%r10), %ymm2, %ymm9
vpunpcklwd 5728(%r10), %ymm3, %ymm10
vpunpckhwd 5728(%r10), %ymm3, %ymm11
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
vmovdqa 5760(%r10), %ymm0
vmovdqa 5824(%r10), %ymm1
vmovdqa 5888(%r10), %ymm2
vmovdqa 5952(%r10), %ymm3
vpunpcklwd 5792(%r10), %ymm0, %ymm12
vpunpckhwd 5792(%r10), %ymm0, %ymm13
vpunpcklwd 5856(%r10), %ymm1, %ymm14
vpunpckhwd 5856(%r10), %ymm1, %ymm15
vpunpcklwd 5920(%r10), %ymm2, %ymm0
vpunpckhwd 5920(%r10), %ymm2, %ymm1
vpunpcklwd 5984(%r10), %ymm3, %ymm2
vpunpckhwd 5984(%r10), %ymm3, %ymm3
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
vmovdqa %ymm15, 416(%r12)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 672(%r12)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 928(%r12)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 1184(%r12)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 1440(%r12)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 1696(%r12)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 2208(%r12)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 2464(%r12)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 2720(%r12)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 2976(%r12)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 3232(%r12)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 3488(%r12)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 3744(%r12)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1952(%r12)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 4000(%r12)
vmovdqa 6016(%r10), %ymm0
vmovdqa 6080(%r10), %ymm1
vmovdqa 6144(%r10), %ymm2
vmovdqa 6208(%r10), %ymm3
vpunpcklwd 6048(%r10), %ymm0, %ymm4
vpunpckhwd 6048(%r10), %ymm0, %ymm5
vpunpcklwd 6112(%r10), %ymm1, %ymm6
vpunpckhwd 6112(%r10), %ymm1, %ymm7
vpunpcklwd 6176(%r10), %ymm2, %ymm8
vpunpckhwd 6176(%r10), %ymm2, %ymm9
vpunpcklwd 6240(%r10), %ymm3, %ymm10
vpunpckhwd 6240(%r10), %ymm3, %ymm11
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
vmovdqa 6272(%r10), %ymm0
vmovdqa 6336(%r10), %ymm1
vmovdqa 6400(%r10), %ymm2
vmovdqa 6464(%r10), %ymm3
vpunpcklwd 6304(%r10), %ymm0, %ymm12
vpunpckhwd 6304(%r10), %ymm0, %ymm13
vpunpcklwd 6368(%r10), %ymm1, %ymm14
vpunpckhwd 6368(%r10), %ymm1, %ymm15
vpunpcklwd 6432(%r10), %ymm2, %ymm0
vpunpckhwd 6432(%r10), %ymm2, %ymm1
vpunpcklwd 6496(%r10), %ymm3, %ymm2
vpunpckhwd 6496(%r10), %ymm3, %ymm3
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
vmovdqa %ymm15, 192(%r12)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 448(%r12)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 704(%r12)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 960(%r12)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 1216(%r12)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 1472(%r12)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 1728(%r12)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 2240(%r12)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 2496(%r12)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 2752(%r12)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 3008(%r12)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 3264(%r12)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 3520(%r12)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 3776(%r12)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 1984(%r12)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 4032(%r12)
vmovdqa 6528(%r10), %ymm0
vmovdqa 6592(%r10), %ymm1
vmovdqa 6656(%r10), %ymm2
vmovdqa 6720(%r10), %ymm3
vpunpcklwd 6560(%r10), %ymm0, %ymm4
vpunpckhwd 6560(%r10), %ymm0, %ymm5
vpunpcklwd 6624(%r10), %ymm1, %ymm6
vpunpckhwd 6624(%r10), %ymm1, %ymm7
vpunpcklwd 6688(%r10), %ymm2, %ymm8
vpunpckhwd 6688(%r10), %ymm2, %ymm9
vpunpcklwd 6752(%r10), %ymm3, %ymm10
vpunpckhwd 6752(%r10), %ymm3, %ymm11
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
vmovdqa 6784(%r10), %ymm0
vmovdqa 6848(%r10), %ymm1
vmovdqa 6912(%r10), %ymm2
vmovdqa 6976(%r10), %ymm3
vpunpcklwd 6816(%r10), %ymm0, %ymm12
vpunpckhwd 6816(%r10), %ymm0, %ymm13
vpunpcklwd 6880(%r10), %ymm1, %ymm14
vpunpckhwd 6880(%r10), %ymm1, %ymm15
vpunpcklwd 6944(%r10), %ymm2, %ymm0
vpunpckhwd 6944(%r10), %ymm2, %ymm1
vpunpcklwd 7008(%r10), %ymm3, %ymm2
vpunpckhwd 7008(%r10), %ymm3, %ymm3
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
vmovdqa %ymm15, 224(%r12)
vinserti128 $1, %xmm13, %ymm5, %ymm15
vmovdqa %ymm15, 480(%r12)
vinserti128 $1, %xmm11, %ymm6, %ymm15
vmovdqa %ymm15, 736(%r12)
vinserti128 $1, %xmm0, %ymm7, %ymm15
vmovdqa %ymm15, 992(%r12)
vinserti128 $1, %xmm12, %ymm8, %ymm15
vmovdqa %ymm15, 1248(%r12)
vinserti128 $1, %xmm2, %ymm9, %ymm15
vmovdqa %ymm15, 1504(%r12)
vinserti128 $1, %xmm14, %ymm10, %ymm15
vmovdqa %ymm15, 1760(%r12)
vpermq $78, %ymm4, %ymm4
vpermq $78, %ymm5, %ymm5
vpermq $78, %ymm6, %ymm6
vpermq $78, %ymm7, %ymm7
vpermq $78, %ymm8, %ymm8
vpermq $78, %ymm9, %ymm9
vpermq $78, %ymm10, %ymm10
vinserti128 $0, %xmm4, %ymm3, %ymm15
vmovdqa %ymm15, 2272(%r12)
vinserti128 $0, %xmm5, %ymm13, %ymm15
vmovdqa %ymm15, 2528(%r12)
vinserti128 $0, %xmm6, %ymm11, %ymm15
vmovdqa %ymm15, 2784(%r12)
vinserti128 $0, %xmm7, %ymm0, %ymm15
vmovdqa %ymm15, 3040(%r12)
vinserti128 $0, %xmm8, %ymm12, %ymm15
vmovdqa %ymm15, 3296(%r12)
vinserti128 $0, %xmm9, %ymm2, %ymm15
vmovdqa %ymm15, 3552(%r12)
vinserti128 $0, %xmm10, %ymm14, %ymm15
vmovdqa %ymm15, 3808(%r12)
vmovdqa 0(%rsp), %ymm11
vinserti128 $1, %xmm1, %ymm11, %ymm14
vmovdqa %ymm14, 2016(%r12)
vpermq $78, %ymm11, %ymm11
vinserti128 $0, %xmm11, %ymm1, %ymm1
vmovdqa %ymm1, 4064(%r12)
addq $32, %rsp
add $2048, %rax
add $2048, %r11
add $4096, %r12
dec %ecx
jnz karatsuba_loop_1
sub $16384, %r12
add $11584, %rsp
subq $1280, %rsp
vmovdqa const729(%rip), %ymm15
vmovdqa const3_inv(%rip), %ymm14
vmovdqa const5_inv(%rip), %ymm13
vmovdqa const9(%rip), %ymm12
vmovdqa 128(%r12), %ymm0
vpsubw 256(%r12), %ymm0, %ymm0
vmovdqa 640(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 384(%r12), %ymm1, %ymm1
vpsubw 0(%r12), %ymm0, %ymm0
vpaddw 512(%r12), %ymm0, %ymm0
vmovdqa 896(%r12), %ymm2
vpsubw 1024(%r12), %ymm2, %ymm2
vmovdqa 1408(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 1152(%r12), %ymm3, %ymm3
vpsubw 768(%r12), %ymm2, %ymm2
vpaddw 1280(%r12), %ymm2, %ymm2
vmovdqa 1664(%r12), %ymm4
vpsubw 1792(%r12), %ymm4, %ymm4
vmovdqa 2176(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 1920(%r12), %ymm5, %ymm5
vpsubw 1536(%r12), %ymm4, %ymm4
vpaddw 2048(%r12), %ymm4, %ymm4
vpsubw 768(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 0(%r12), %ymm1, %ymm1
vpaddw 1536(%r12), %ymm1, %ymm1
vmovdqa 384(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 1920(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 1152(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 0(%r12), %ymm8
vmovdqa 1152(%r12), %ymm9
vmovdqa %ymm8, 0(%rsp)
vmovdqa %ymm0, 32(%rsp)
vmovdqa %ymm1, 64(%rsp)
vmovdqa %ymm7, 96(%rsp)
vmovdqa %ymm5, 128(%rsp)
vmovdqa %ymm2, 160(%rsp)
vmovdqa %ymm3, 192(%rsp)
vmovdqa %ymm9, 224(%rsp)
vmovdqa 2432(%r12), %ymm0
vpsubw 2560(%r12), %ymm0, %ymm0
vmovdqa 2944(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 2688(%r12), %ymm1, %ymm1
vpsubw 2304(%r12), %ymm0, %ymm0
vpaddw 2816(%r12), %ymm0, %ymm0
vmovdqa 3200(%r12), %ymm2
vpsubw 3328(%r12), %ymm2, %ymm2
vmovdqa 3712(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 3456(%r12), %ymm3, %ymm3
vpsubw 3072(%r12), %ymm2, %ymm2
vpaddw 3584(%r12), %ymm2, %ymm2
vmovdqa 3968(%r12), %ymm4
vpsubw 4096(%r12), %ymm4, %ymm4
vmovdqa 4480(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 4224(%r12), %ymm5, %ymm5
vpsubw 3840(%r12), %ymm4, %ymm4
vpaddw 4352(%r12), %ymm4, %ymm4
vpsubw 3072(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 2304(%r12), %ymm1, %ymm1
vpaddw 3840(%r12), %ymm1, %ymm1
vmovdqa 2688(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 4224(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 3456(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 2304(%r12), %ymm8
vmovdqa 3456(%r12), %ymm9
vmovdqa %ymm8, 256(%rsp)
vmovdqa %ymm0, 288(%rsp)
vmovdqa %ymm1, 320(%rsp)
vmovdqa %ymm7, 352(%rsp)
vmovdqa %ymm5, 384(%rsp)
vmovdqa %ymm2, 416(%rsp)
vmovdqa %ymm3, 448(%rsp)
vmovdqa %ymm9, 480(%rsp)
vmovdqa 4736(%r12), %ymm0
vpsubw 4864(%r12), %ymm0, %ymm0
vmovdqa 5248(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 4992(%r12), %ymm1, %ymm1
vpsubw 4608(%r12), %ymm0, %ymm0
vpaddw 5120(%r12), %ymm0, %ymm0
vmovdqa 5504(%r12), %ymm2
vpsubw 5632(%r12), %ymm2, %ymm2
vmovdqa 6016(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 5760(%r12), %ymm3, %ymm3
vpsubw 5376(%r12), %ymm2, %ymm2
vpaddw 5888(%r12), %ymm2, %ymm2
vmovdqa 6272(%r12), %ymm4
vpsubw 6400(%r12), %ymm4, %ymm4
vmovdqa 6784(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 6528(%r12), %ymm5, %ymm5
vpsubw 6144(%r12), %ymm4, %ymm4
vpaddw 6656(%r12), %ymm4, %ymm4
vpsubw 5376(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 4608(%r12), %ymm1, %ymm1
vpaddw 6144(%r12), %ymm1, %ymm1
vmovdqa 4992(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 6528(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 5760(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 4608(%r12), %ymm8
vmovdqa 5760(%r12), %ymm9
vmovdqa %ymm8, 512(%rsp)
vmovdqa %ymm0, 544(%rsp)
vmovdqa %ymm1, 576(%rsp)
vmovdqa %ymm7, 608(%rsp)
vmovdqa %ymm5, 640(%rsp)
vmovdqa %ymm2, 672(%rsp)
vmovdqa %ymm3, 704(%rsp)
vmovdqa %ymm9, 736(%rsp)
vmovdqa 7040(%r12), %ymm0
vpsubw 7168(%r12), %ymm0, %ymm0
vmovdqa 7552(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 7296(%r12), %ymm1, %ymm1
vpsubw 6912(%r12), %ymm0, %ymm0
vpaddw 7424(%r12), %ymm0, %ymm0
vmovdqa 7808(%r12), %ymm2
vpsubw 7936(%r12), %ymm2, %ymm2
vmovdqa 8320(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 8064(%r12), %ymm3, %ymm3
vpsubw 7680(%r12), %ymm2, %ymm2
vpaddw 8192(%r12), %ymm2, %ymm2
vmovdqa 8576(%r12), %ymm4
vpsubw 8704(%r12), %ymm4, %ymm4
vmovdqa 9088(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 8832(%r12), %ymm5, %ymm5
vpsubw 8448(%r12), %ymm4, %ymm4
vpaddw 8960(%r12), %ymm4, %ymm4
vpsubw 7680(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 6912(%r12), %ymm1, %ymm1
vpaddw 8448(%r12), %ymm1, %ymm1
vmovdqa 7296(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 8832(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 8064(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 6912(%r12), %ymm8
vmovdqa 8064(%r12), %ymm9
vmovdqa %ymm8, 768(%rsp)
vmovdqa %ymm0, 800(%rsp)
vmovdqa %ymm1, 832(%rsp)
vmovdqa %ymm7, 864(%rsp)
vmovdqa %ymm5, 896(%rsp)
vmovdqa %ymm2, 928(%rsp)
vmovdqa %ymm3, 960(%rsp)
vmovdqa %ymm9, 992(%rsp)
vmovdqa 9344(%r12), %ymm0
vpsubw 9472(%r12), %ymm0, %ymm0
vmovdqa 9856(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 9600(%r12), %ymm1, %ymm1
vpsubw 9216(%r12), %ymm0, %ymm0
vpaddw 9728(%r12), %ymm0, %ymm0
vmovdqa 10112(%r12), %ymm2
vpsubw 10240(%r12), %ymm2, %ymm2
vmovdqa 10624(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 10368(%r12), %ymm3, %ymm3
vpsubw 9984(%r12), %ymm2, %ymm2
vpaddw 10496(%r12), %ymm2, %ymm2
vmovdqa 10880(%r12), %ymm4
vpsubw 11008(%r12), %ymm4, %ymm4
vmovdqa 11392(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 11136(%r12), %ymm5, %ymm5
vpsubw 10752(%r12), %ymm4, %ymm4
vpaddw 11264(%r12), %ymm4, %ymm4
vpsubw 9984(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 9216(%r12), %ymm1, %ymm1
vpaddw 10752(%r12), %ymm1, %ymm1
vmovdqa 9600(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 11136(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 10368(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 9216(%r12), %ymm8
vmovdqa 10368(%r12), %ymm9
vmovdqa %ymm8, 1024(%rsp)
vmovdqa %ymm0, 1056(%rsp)
vmovdqa %ymm1, 1088(%rsp)
vmovdqa %ymm7, 1120(%rsp)
vmovdqa %ymm5, 1152(%rsp)
vmovdqa %ymm2, 1184(%rsp)
vmovdqa %ymm3, 1216(%rsp)
vmovdqa %ymm9, 1248(%rsp)
vmovdqa 11648(%r12), %ymm0
vpsubw 11776(%r12), %ymm0, %ymm0
vmovdqa 12160(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 11904(%r12), %ymm1, %ymm1
vpsubw 11520(%r12), %ymm0, %ymm0
vpaddw 12032(%r12), %ymm0, %ymm0
vmovdqa 12416(%r12), %ymm2
vpsubw 12544(%r12), %ymm2, %ymm2
vmovdqa 12928(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 12672(%r12), %ymm3, %ymm3
vpsubw 12288(%r12), %ymm2, %ymm2
vpaddw 12800(%r12), %ymm2, %ymm2
vmovdqa 13184(%r12), %ymm4
vpsubw 13312(%r12), %ymm4, %ymm4
vmovdqa 13696(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 13440(%r12), %ymm5, %ymm5
vpsubw 13056(%r12), %ymm4, %ymm4
vpaddw 13568(%r12), %ymm4, %ymm4
vpsubw 12288(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 11520(%r12), %ymm1, %ymm1
vpaddw 13056(%r12), %ymm1, %ymm1
vmovdqa 11904(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 13440(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 12672(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 11520(%r12), %ymm8
vmovdqa 12672(%r12), %ymm9
vmovdqa %ymm8, 1280(%rsp)
vmovdqa %ymm0, 1312(%rsp)
vmovdqa %ymm1, 1344(%rsp)
vmovdqa %ymm7, 1376(%rsp)
vmovdqa %ymm5, 1408(%rsp)
vmovdqa %ymm2, 1440(%rsp)
vmovdqa %ymm3, 1472(%rsp)
vmovdqa %ymm9, 1504(%rsp)
vmovdqa 13952(%r12), %ymm0
vpsubw 14080(%r12), %ymm0, %ymm0
vmovdqa 14464(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 14208(%r12), %ymm1, %ymm1
vpsubw 13824(%r12), %ymm0, %ymm0
vpaddw 14336(%r12), %ymm0, %ymm0
vmovdqa 14720(%r12), %ymm2
vpsubw 14848(%r12), %ymm2, %ymm2
vmovdqa 15232(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 14976(%r12), %ymm3, %ymm3
vpsubw 14592(%r12), %ymm2, %ymm2
vpaddw 15104(%r12), %ymm2, %ymm2
vmovdqa 15488(%r12), %ymm4
vpsubw 15616(%r12), %ymm4, %ymm4
vmovdqa 16000(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 15744(%r12), %ymm5, %ymm5
vpsubw 15360(%r12), %ymm4, %ymm4
vpaddw 15872(%r12), %ymm4, %ymm4
vpsubw 14592(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 13824(%r12), %ymm1, %ymm1
vpaddw 15360(%r12), %ymm1, %ymm1
vmovdqa 14208(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 15744(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 14976(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 13824(%r12), %ymm8
vmovdqa 14976(%r12), %ymm9
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
vmovdqu 416(%rdi), %ymm9
vpaddw %ymm9, %ymm6, %ymm9
vmovdqu %ymm9, 416(%rdi)
vmovdqu 832(%rdi), %ymm9
vpaddw %ymm9, %ymm3, %ymm9
vmovdqu %ymm9, 832(%rdi)
vmovdqu 1248(%rdi), %ymm9
vpaddw %ymm9, %ymm4, %ymm9
vmovdqu %ymm9, 1248(%rdi)
vmovdqu 22(%rdi), %ymm9
vpaddw %ymm9, %ymm7, %ymm9
vmovdqu %ymm9, 22(%rdi)
vmovdqu 438(%rdi), %ymm9
vpaddw %ymm9, %ymm8, %ymm9
vmovdqu %ymm9, 438(%rdi)
vmovdqu 854(%rdi), %ymm9
vpaddw %ymm9, %ymm5, %ymm9
vmovdqu %ymm9, 854(%rdi)
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
vmovdqu 104(%rdi), %ymm7
vpaddw %ymm7, %ymm5, %ymm7
vmovdqu %ymm7, 104(%rdi)
vmovdqu 520(%rdi), %ymm7
vpaddw %ymm7, %ymm6, %ymm7
vmovdqu %ymm7, 520(%rdi)
vmovdqu 936(%rdi), %ymm7
vpaddw %ymm7, %ymm10, %ymm7
vmovdqu %ymm7, 936(%rdi)
vmovdqu 1352(%rdi), %ymm7
vpaddw %ymm7, %ymm9, %ymm7
vmovdqu %ymm7, 1352(%rdi)
vmovdqu 126(%rdi), %ymm7
vpaddw %ymm7, %ymm3, %ymm7
vmovdqu %ymm7, 126(%rdi)
vmovdqu 542(%rdi), %ymm7
vpaddw %ymm7, %ymm4, %ymm7
vmovdqu %ymm7, 542(%rdi)
vmovdqu 958(%rdi), %ymm7
vpaddw %ymm7, %ymm11, %ymm7
vmovdqu %ymm7, 958(%rdi)
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
vmovdqu 208(%rdi), %ymm3
vpaddw %ymm3, %ymm11, %ymm3
vmovdqu %ymm3, 208(%rdi)
vmovdqu 624(%rdi), %ymm3
vpaddw %ymm3, %ymm6, %ymm3
vmovdqu %ymm3, 624(%rdi)
vmovdqu 1040(%rdi), %ymm3
vpaddw %ymm3, %ymm8, %ymm3
vmovdqu %ymm3, 1040(%rdi)
vmovdqu 1456(%rdi), %ymm3
vpaddw %ymm3, %ymm7, %ymm3
vmovdqu %ymm3, 1456(%rdi)
vmovdqu 230(%rdi), %ymm3
vpaddw %ymm3, %ymm10, %ymm3
vmovdqu %ymm3, 230(%rdi)
vmovdqu 646(%rdi), %ymm3
vpaddw %ymm3, %ymm9, %ymm3
vmovdqu %ymm3, 646(%rdi)
vmovdqu 1062(%rdi), %ymm3
vpaddw %ymm3, %ymm5, %ymm3
vmovdqu %ymm3, 1062(%rdi)
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
vmovdqu 312(%rdi), %ymm10
vpaddw %ymm10, %ymm5, %ymm10
vmovdqu %ymm10, 312(%rdi)
vmovdqu 728(%rdi), %ymm10
vpaddw %ymm10, %ymm6, %ymm10
vmovdqu %ymm10, 728(%rdi)
vmovdqu 1144(%rdi), %ymm10
vpaddw %ymm10, %ymm4, %ymm10
vmovdqu %ymm10, 1144(%rdi)
vmovdqu 1560(%rdi), %ymm10
vpaddw %ymm10, %ymm3, %ymm10
vmovdqu %ymm10, 1560(%rdi)
vmovdqu 334(%rdi), %ymm10
vpaddw %ymm10, %ymm8, %ymm10
vmovdqu %ymm10, 334(%rdi)
vmovdqu 750(%rdi), %ymm10
vpaddw %ymm10, %ymm7, %ymm10
vmovdqu %ymm10, 750(%rdi)
vmovdqu 1166(%rdi), %ymm10
vpaddw %ymm10, %ymm11, %ymm10
vmovdqu %ymm10, 1166(%rdi)
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
vmovdqu 416(%rdi), %ymm8
vpaddw %ymm8, %ymm11, %ymm8
vmovdqu %ymm8, 416(%rdi)
vmovdqu 832(%rdi), %ymm8
vpaddw %ymm8, %ymm6, %ymm8
vmovdqu %ymm8, 832(%rdi)
vmovdqu 1248(%rdi), %ymm8
vpaddw %ymm8, %ymm9, %ymm8
vmovdqu %ymm8, 1248(%rdi)
vmovdqu 22(%rdi), %ymm8
vpaddw %ymm8, %ymm10, %ymm8
vmovdqu %ymm8, 22(%rdi)
vmovdqu 438(%rdi), %ymm8
vpaddw %ymm8, %ymm4, %ymm8
vmovdqu %ymm8, 438(%rdi)
vmovdqu 854(%rdi), %ymm8
vpaddw %ymm8, %ymm3, %ymm8
vmovdqu %ymm8, 854(%rdi)
vmovdqu 1270(%rdi), %ymm8
vpaddw %ymm8, %ymm5, %ymm8
vmovdqu %ymm8, 1270(%rdi)
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
vmovdqu 520(%rdi), %ymm4
vpaddw %ymm4, %ymm5, %ymm4
vmovdqu %ymm4, 520(%rdi)
vmovdqu 936(%rdi), %ymm4
vpaddw %ymm4, %ymm6, %ymm4
vmovdqu %ymm4, 936(%rdi)
vmovdqu 1352(%rdi), %ymm4
vpaddw %ymm4, %ymm7, %ymm4
vmovdqu %ymm4, 1352(%rdi)
vmovdqu 126(%rdi), %ymm4
vpaddw %ymm4, %ymm8, %ymm4
vmovdqu %ymm4, 126(%rdi)
vmovdqu 542(%rdi), %ymm4
vpaddw %ymm4, %ymm9, %ymm4
vmovdqu %ymm4, 542(%rdi)
vmovdqu 958(%rdi), %ymm4
vpaddw %ymm4, %ymm10, %ymm4
vmovdqu %ymm4, 958(%rdi)
vmovdqu 1374(%rdi), %ymm4
vpaddw %ymm4, %ymm11, %ymm4
vmovdqu %ymm4, 1374(%rdi)
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
vmovdqu 624(%rdi), %ymm9
vpaddw %ymm9, %ymm11, %ymm9
vmovdqu %ymm9, 624(%rdi)
vmovdqu 1040(%rdi), %ymm9
vpaddw %ymm9, %ymm6, %ymm9
vmovdqu %ymm9, 1040(%rdi)
vmovdqu 1456(%rdi), %ymm9
vpaddw %ymm9, %ymm3, %ymm9
vmovdqu %ymm9, 1456(%rdi)
vmovdqu 230(%rdi), %ymm9
vpaddw %ymm9, %ymm4, %ymm9
vmovdqu %ymm9, 230(%rdi)
vmovdqu 646(%rdi), %ymm9
vpaddw %ymm9, %ymm7, %ymm9
vmovdqu %ymm9, 646(%rdi)
vmovdqu 1062(%rdi), %ymm9
vpaddw %ymm9, %ymm8, %ymm9
vmovdqu %ymm9, 1062(%rdi)
vmovdqu 1478(%rdi), %ymm9
vpaddw %ymm9, %ymm5, %ymm9
vmovdqu %ymm9, 1478(%rdi)
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
vmovdqu 728(%rdi), %ymm7
vpaddw %ymm7, %ymm5, %ymm7
vmovdqu %ymm7, 728(%rdi)
vmovdqu 1144(%rdi), %ymm7
vpaddw %ymm7, %ymm6, %ymm7
vmovdqu %ymm7, 1144(%rdi)
vmovdqu 1560(%rdi), %ymm7
vpaddw %ymm7, %ymm10, %ymm7
vmovdqu %ymm7, 1560(%rdi)
vmovdqu 334(%rdi), %ymm7
vpaddw %ymm7, %ymm9, %ymm7
vmovdqu %ymm7, 334(%rdi)
vmovdqu 750(%rdi), %ymm7
vpaddw %ymm7, %ymm3, %ymm7
vmovdqu %ymm7, 750(%rdi)
vmovdqu 1166(%rdi), %ymm7
vpaddw %ymm7, %ymm4, %ymm7
vmovdqu %ymm7, 1166(%rdi)
vmovdqu 1582(%rdi), %ymm7
vpaddw %ymm7, %ymm11, %ymm7
vmovdqu %ymm7, 1582(%rdi)
vmovdqa 160(%r12), %ymm0
vpsubw 288(%r12), %ymm0, %ymm0
vmovdqa 672(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 416(%r12), %ymm1, %ymm1
vpsubw 32(%r12), %ymm0, %ymm0
vpaddw 544(%r12), %ymm0, %ymm0
vmovdqa 928(%r12), %ymm2
vpsubw 1056(%r12), %ymm2, %ymm2
vmovdqa 1440(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 1184(%r12), %ymm3, %ymm3
vpsubw 800(%r12), %ymm2, %ymm2
vpaddw 1312(%r12), %ymm2, %ymm2
vmovdqa 1696(%r12), %ymm4
vpsubw 1824(%r12), %ymm4, %ymm4
vmovdqa 2208(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 1952(%r12), %ymm5, %ymm5
vpsubw 1568(%r12), %ymm4, %ymm4
vpaddw 2080(%r12), %ymm4, %ymm4
vpsubw 800(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 32(%r12), %ymm1, %ymm1
vpaddw 1568(%r12), %ymm1, %ymm1
vmovdqa 416(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 1952(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 1184(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 32(%r12), %ymm8
vmovdqa 1184(%r12), %ymm9
vmovdqa %ymm8, 0(%rsp)
vmovdqa %ymm0, 32(%rsp)
vmovdqa %ymm1, 64(%rsp)
vmovdqa %ymm7, 96(%rsp)
vmovdqa %ymm5, 128(%rsp)
vmovdqa %ymm2, 160(%rsp)
vmovdqa %ymm3, 192(%rsp)
vmovdqa %ymm9, 224(%rsp)
vmovdqa 2464(%r12), %ymm0
vpsubw 2592(%r12), %ymm0, %ymm0
vmovdqa 2976(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 2720(%r12), %ymm1, %ymm1
vpsubw 2336(%r12), %ymm0, %ymm0
vpaddw 2848(%r12), %ymm0, %ymm0
vmovdqa 3232(%r12), %ymm2
vpsubw 3360(%r12), %ymm2, %ymm2
vmovdqa 3744(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 3488(%r12), %ymm3, %ymm3
vpsubw 3104(%r12), %ymm2, %ymm2
vpaddw 3616(%r12), %ymm2, %ymm2
vmovdqa 4000(%r12), %ymm4
vpsubw 4128(%r12), %ymm4, %ymm4
vmovdqa 4512(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 4256(%r12), %ymm5, %ymm5
vpsubw 3872(%r12), %ymm4, %ymm4
vpaddw 4384(%r12), %ymm4, %ymm4
vpsubw 3104(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 2336(%r12), %ymm1, %ymm1
vpaddw 3872(%r12), %ymm1, %ymm1
vmovdqa 2720(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 4256(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 3488(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 2336(%r12), %ymm8
vmovdqa 3488(%r12), %ymm9
vmovdqa %ymm8, 256(%rsp)
vmovdqa %ymm0, 288(%rsp)
vmovdqa %ymm1, 320(%rsp)
vmovdqa %ymm7, 352(%rsp)
vmovdqa %ymm5, 384(%rsp)
vmovdqa %ymm2, 416(%rsp)
vmovdqa %ymm3, 448(%rsp)
vmovdqa %ymm9, 480(%rsp)
vmovdqa 4768(%r12), %ymm0
vpsubw 4896(%r12), %ymm0, %ymm0
vmovdqa 5280(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5024(%r12), %ymm1, %ymm1
vpsubw 4640(%r12), %ymm0, %ymm0
vpaddw 5152(%r12), %ymm0, %ymm0
vmovdqa 5536(%r12), %ymm2
vpsubw 5664(%r12), %ymm2, %ymm2
vmovdqa 6048(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 5792(%r12), %ymm3, %ymm3
vpsubw 5408(%r12), %ymm2, %ymm2
vpaddw 5920(%r12), %ymm2, %ymm2
vmovdqa 6304(%r12), %ymm4
vpsubw 6432(%r12), %ymm4, %ymm4
vmovdqa 6816(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 6560(%r12), %ymm5, %ymm5
vpsubw 6176(%r12), %ymm4, %ymm4
vpaddw 6688(%r12), %ymm4, %ymm4
vpsubw 5408(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 4640(%r12), %ymm1, %ymm1
vpaddw 6176(%r12), %ymm1, %ymm1
vmovdqa 5024(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 6560(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 5792(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 4640(%r12), %ymm8
vmovdqa 5792(%r12), %ymm9
vmovdqa %ymm8, 512(%rsp)
vmovdqa %ymm0, 544(%rsp)
vmovdqa %ymm1, 576(%rsp)
vmovdqa %ymm7, 608(%rsp)
vmovdqa %ymm5, 640(%rsp)
vmovdqa %ymm2, 672(%rsp)
vmovdqa %ymm3, 704(%rsp)
vmovdqa %ymm9, 736(%rsp)
vmovdqa 7072(%r12), %ymm0
vpsubw 7200(%r12), %ymm0, %ymm0
vmovdqa 7584(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 7328(%r12), %ymm1, %ymm1
vpsubw 6944(%r12), %ymm0, %ymm0
vpaddw 7456(%r12), %ymm0, %ymm0
vmovdqa 7840(%r12), %ymm2
vpsubw 7968(%r12), %ymm2, %ymm2
vmovdqa 8352(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 8096(%r12), %ymm3, %ymm3
vpsubw 7712(%r12), %ymm2, %ymm2
vpaddw 8224(%r12), %ymm2, %ymm2
vmovdqa 8608(%r12), %ymm4
vpsubw 8736(%r12), %ymm4, %ymm4
vmovdqa 9120(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 8864(%r12), %ymm5, %ymm5
vpsubw 8480(%r12), %ymm4, %ymm4
vpaddw 8992(%r12), %ymm4, %ymm4
vpsubw 7712(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 6944(%r12), %ymm1, %ymm1
vpaddw 8480(%r12), %ymm1, %ymm1
vmovdqa 7328(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 8864(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 8096(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 6944(%r12), %ymm8
vmovdqa 8096(%r12), %ymm9
vmovdqa %ymm8, 768(%rsp)
vmovdqa %ymm0, 800(%rsp)
vmovdqa %ymm1, 832(%rsp)
vmovdqa %ymm7, 864(%rsp)
vmovdqa %ymm5, 896(%rsp)
vmovdqa %ymm2, 928(%rsp)
vmovdqa %ymm3, 960(%rsp)
vmovdqa %ymm9, 992(%rsp)
vmovdqa 9376(%r12), %ymm0
vpsubw 9504(%r12), %ymm0, %ymm0
vmovdqa 9888(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 9632(%r12), %ymm1, %ymm1
vpsubw 9248(%r12), %ymm0, %ymm0
vpaddw 9760(%r12), %ymm0, %ymm0
vmovdqa 10144(%r12), %ymm2
vpsubw 10272(%r12), %ymm2, %ymm2
vmovdqa 10656(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 10400(%r12), %ymm3, %ymm3
vpsubw 10016(%r12), %ymm2, %ymm2
vpaddw 10528(%r12), %ymm2, %ymm2
vmovdqa 10912(%r12), %ymm4
vpsubw 11040(%r12), %ymm4, %ymm4
vmovdqa 11424(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 11168(%r12), %ymm5, %ymm5
vpsubw 10784(%r12), %ymm4, %ymm4
vpaddw 11296(%r12), %ymm4, %ymm4
vpsubw 10016(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 9248(%r12), %ymm1, %ymm1
vpaddw 10784(%r12), %ymm1, %ymm1
vmovdqa 9632(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 11168(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 10400(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 9248(%r12), %ymm8
vmovdqa 10400(%r12), %ymm9
vmovdqa %ymm8, 1024(%rsp)
vmovdqa %ymm0, 1056(%rsp)
vmovdqa %ymm1, 1088(%rsp)
vmovdqa %ymm7, 1120(%rsp)
vmovdqa %ymm5, 1152(%rsp)
vmovdqa %ymm2, 1184(%rsp)
vmovdqa %ymm3, 1216(%rsp)
vmovdqa %ymm9, 1248(%rsp)
vmovdqa 11680(%r12), %ymm0
vpsubw 11808(%r12), %ymm0, %ymm0
vmovdqa 12192(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 11936(%r12), %ymm1, %ymm1
vpsubw 11552(%r12), %ymm0, %ymm0
vpaddw 12064(%r12), %ymm0, %ymm0
vmovdqa 12448(%r12), %ymm2
vpsubw 12576(%r12), %ymm2, %ymm2
vmovdqa 12960(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 12704(%r12), %ymm3, %ymm3
vpsubw 12320(%r12), %ymm2, %ymm2
vpaddw 12832(%r12), %ymm2, %ymm2
vmovdqa 13216(%r12), %ymm4
vpsubw 13344(%r12), %ymm4, %ymm4
vmovdqa 13728(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 13472(%r12), %ymm5, %ymm5
vpsubw 13088(%r12), %ymm4, %ymm4
vpaddw 13600(%r12), %ymm4, %ymm4
vpsubw 12320(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 11552(%r12), %ymm1, %ymm1
vpaddw 13088(%r12), %ymm1, %ymm1
vmovdqa 11936(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 13472(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 12704(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 11552(%r12), %ymm8
vmovdqa 12704(%r12), %ymm9
vmovdqa %ymm8, 1280(%rsp)
vmovdqa %ymm0, 1312(%rsp)
vmovdqa %ymm1, 1344(%rsp)
vmovdqa %ymm7, 1376(%rsp)
vmovdqa %ymm5, 1408(%rsp)
vmovdqa %ymm2, 1440(%rsp)
vmovdqa %ymm3, 1472(%rsp)
vmovdqa %ymm9, 1504(%rsp)
vmovdqa 13984(%r12), %ymm0
vpsubw 14112(%r12), %ymm0, %ymm0
vmovdqa 14496(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 14240(%r12), %ymm1, %ymm1
vpsubw 13856(%r12), %ymm0, %ymm0
vpaddw 14368(%r12), %ymm0, %ymm0
vmovdqa 14752(%r12), %ymm2
vpsubw 14880(%r12), %ymm2, %ymm2
vmovdqa 15264(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 15008(%r12), %ymm3, %ymm3
vpsubw 14624(%r12), %ymm2, %ymm2
vpaddw 15136(%r12), %ymm2, %ymm2
vmovdqa 15520(%r12), %ymm4
vpsubw 15648(%r12), %ymm4, %ymm4
vmovdqa 16032(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 15776(%r12), %ymm5, %ymm5
vpsubw 15392(%r12), %ymm4, %ymm4
vpaddw 15904(%r12), %ymm4, %ymm4
vpsubw 14624(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 13856(%r12), %ymm1, %ymm1
vpaddw 15392(%r12), %ymm1, %ymm1
vmovdqa 14240(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 15776(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 15008(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 13856(%r12), %ymm8
vmovdqa 15008(%r12), %ymm9
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
vmovdqu 448(%rdi), %ymm3
vpaddw %ymm3, %ymm6, %ymm3
vmovdqu %ymm3, 448(%rdi)
vmovdqu 864(%rdi), %ymm3
vpaddw %ymm3, %ymm8, %ymm3
vmovdqu %ymm3, 864(%rdi)
vmovdqu 1280(%rdi), %ymm3
vpaddw %ymm3, %ymm7, %ymm3
vmovdqu %ymm3, 1280(%rdi)
vmovdqu 54(%rdi), %ymm3
vpaddw %ymm3, %ymm10, %ymm3
vmovdqu %ymm3, 54(%rdi)
vmovdqu 470(%rdi), %ymm3
vpaddw %ymm3, %ymm9, %ymm3
vmovdqu %ymm3, 470(%rdi)
vmovdqu 886(%rdi), %ymm3
vpaddw %ymm3, %ymm5, %ymm3
vmovdqu %ymm3, 886(%rdi)
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
vmovdqu 136(%rdi), %ymm10
vpaddw %ymm10, %ymm5, %ymm10
vmovdqu %ymm10, 136(%rdi)
vmovdqu 552(%rdi), %ymm10
vpaddw %ymm10, %ymm6, %ymm10
vmovdqu %ymm10, 552(%rdi)
vmovdqu 968(%rdi), %ymm10
vpaddw %ymm10, %ymm4, %ymm10
vmovdqu %ymm10, 968(%rdi)
vmovdqu 1384(%rdi), %ymm10
vpaddw %ymm10, %ymm3, %ymm10
vmovdqu %ymm10, 1384(%rdi)
vmovdqu 158(%rdi), %ymm10
vpaddw %ymm10, %ymm8, %ymm10
vmovdqu %ymm10, 158(%rdi)
vmovdqu 574(%rdi), %ymm10
vpaddw %ymm10, %ymm7, %ymm10
vmovdqu %ymm10, 574(%rdi)
vmovdqu 990(%rdi), %ymm10
vpaddw %ymm10, %ymm11, %ymm10
vmovdqu %ymm10, 990(%rdi)
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
vmovdqu 240(%rdi), %ymm8
vpaddw %ymm8, %ymm11, %ymm8
vmovdqu %ymm8, 240(%rdi)
vmovdqu 656(%rdi), %ymm8
vpaddw %ymm8, %ymm6, %ymm8
vmovdqu %ymm8, 656(%rdi)
vmovdqu 1072(%rdi), %ymm8
vpaddw %ymm8, %ymm9, %ymm8
vmovdqu %ymm8, 1072(%rdi)
vmovdqu 1488(%rdi), %ymm8
vpaddw %ymm8, %ymm10, %ymm8
vmovdqu %ymm8, 1488(%rdi)
vmovdqu 262(%rdi), %ymm8
vpaddw %ymm8, %ymm4, %ymm8
vmovdqu %ymm8, 262(%rdi)
vmovdqu 678(%rdi), %ymm8
vpaddw %ymm8, %ymm3, %ymm8
vmovdqu %ymm8, 678(%rdi)
vmovdqu 1094(%rdi), %ymm8
vpaddw %ymm8, %ymm5, %ymm8
vmovdqu %ymm8, 1094(%rdi)
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
vmovdqu 344(%rdi), %ymm4
vpaddw %ymm4, %ymm5, %ymm4
vmovdqu %ymm4, 344(%rdi)
vmovdqu 760(%rdi), %ymm4
vpaddw %ymm4, %ymm6, %ymm4
vmovdqu %ymm4, 760(%rdi)
vmovdqu 1176(%rdi), %ymm4
vpaddw %ymm4, %ymm7, %ymm4
vmovdqu %ymm4, 1176(%rdi)
vmovdqu 1592(%rdi), %ymm4
vpaddw %ymm4, %ymm8, %ymm4
vmovdqu %ymm4, 1592(%rdi)
vmovdqu 366(%rdi), %ymm4
vpaddw %ymm4, %ymm9, %ymm4
vmovdqu %ymm4, 366(%rdi)
vmovdqu 782(%rdi), %ymm4
vpaddw %ymm4, %ymm10, %ymm4
vmovdqu %ymm4, 782(%rdi)
vmovdqu 1198(%rdi), %ymm4
vpaddw %ymm4, %ymm11, %ymm4
vmovdqu %ymm4, 1198(%rdi)
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
vmovdqu 448(%rdi), %ymm9
vpaddw %ymm9, %ymm11, %ymm9
vmovdqu %ymm9, 448(%rdi)
vmovdqu 864(%rdi), %ymm9
vpaddw %ymm9, %ymm6, %ymm9
vmovdqu %ymm9, 864(%rdi)
vmovdqu 1280(%rdi), %ymm9
vpaddw %ymm9, %ymm3, %ymm9
vmovdqu %ymm9, 1280(%rdi)
vmovdqu 54(%rdi), %ymm9
vpaddw %ymm9, %ymm4, %ymm9
vmovdqu %ymm9, 54(%rdi)
vmovdqu 470(%rdi), %ymm9
vpaddw %ymm9, %ymm7, %ymm9
vmovdqu %ymm9, 470(%rdi)
vmovdqu 886(%rdi), %ymm9
vpaddw %ymm9, %ymm8, %ymm9
vmovdqu %ymm9, 886(%rdi)
vmovdqu 1302(%rdi), %ymm9
vpaddw %ymm9, %ymm5, %ymm9
vmovdqu %ymm9, 1302(%rdi)
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
vmovdqu 552(%rdi), %ymm7
vpaddw %ymm7, %ymm5, %ymm7
vmovdqu %ymm7, 552(%rdi)
vmovdqu 968(%rdi), %ymm7
vpaddw %ymm7, %ymm6, %ymm7
vmovdqu %ymm7, 968(%rdi)
vmovdqu 1384(%rdi), %ymm7
vpaddw %ymm7, %ymm10, %ymm7
vmovdqu %ymm7, 1384(%rdi)
vmovdqu 158(%rdi), %ymm7
vpaddw %ymm7, %ymm9, %ymm7
vmovdqu %ymm7, 158(%rdi)
vmovdqu 574(%rdi), %ymm7
vpaddw %ymm7, %ymm3, %ymm7
vmovdqu %ymm7, 574(%rdi)
vmovdqu 990(%rdi), %ymm7
vpaddw %ymm7, %ymm4, %ymm7
vmovdqu %ymm7, 990(%rdi)
vmovdqu 1406(%rdi), %ymm7
vpaddw %ymm7, %ymm11, %ymm7
vmovdqu %ymm7, 1406(%rdi)
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
vmovdqu 656(%rdi), %ymm3
vpaddw %ymm3, %ymm11, %ymm3
vmovdqu %ymm3, 656(%rdi)
vmovdqu 1072(%rdi), %ymm3
vpaddw %ymm3, %ymm6, %ymm3
vmovdqu %ymm3, 1072(%rdi)
vmovdqu 1488(%rdi), %ymm3
vpaddw %ymm3, %ymm8, %ymm3
vmovdqu %ymm3, 1488(%rdi)
vmovdqu 262(%rdi), %ymm3
vpaddw %ymm3, %ymm7, %ymm3
vmovdqu %ymm3, 262(%rdi)
vmovdqu 678(%rdi), %ymm3
vpaddw %ymm3, %ymm10, %ymm3
vmovdqu %ymm3, 678(%rdi)
vmovdqu 1094(%rdi), %ymm3
vpaddw %ymm3, %ymm9, %ymm3
vmovdqu %ymm3, 1094(%rdi)
vmovdqu 1510(%rdi), %ymm3
vpaddw %ymm3, %ymm5, %ymm3
vmovdqu %ymm3, 1510(%rdi)
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
vmovdqu 760(%rdi), %ymm10
vpaddw %ymm10, %ymm5, %ymm10
vmovdqu %ymm10, 760(%rdi)
vmovdqu 1176(%rdi), %ymm10
vpaddw %ymm10, %ymm6, %ymm10
vmovdqu %ymm10, 1176(%rdi)
vmovdqu 1592(%rdi), %ymm10
vpaddw %ymm10, %ymm4, %ymm10
vmovdqu %ymm10, 1592(%rdi)
vmovdqu 366(%rdi), %ymm10
vpaddw %ymm10, %ymm3, %ymm10
vmovdqu %ymm10, 366(%rdi)
vmovdqu 782(%rdi), %ymm10
vpaddw %ymm10, %ymm8, %ymm10
vmovdqu %ymm10, 782(%rdi)
vmovdqu 1198(%rdi), %ymm10
vpaddw %ymm10, %ymm7, %ymm10
vmovdqu %ymm10, 1198(%rdi)
vmovdqu 1614(%rdi), %ymm10
vpaddw %ymm10, %ymm11, %ymm10
vmovdqu %ymm10, 1614(%rdi)
vmovdqa 192(%r12), %ymm0
vpsubw 320(%r12), %ymm0, %ymm0
vmovdqa 704(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 448(%r12), %ymm1, %ymm1
vpsubw 64(%r12), %ymm0, %ymm0
vpaddw 576(%r12), %ymm0, %ymm0
vmovdqa 960(%r12), %ymm2
vpsubw 1088(%r12), %ymm2, %ymm2
vmovdqa 1472(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 1216(%r12), %ymm3, %ymm3
vpsubw 832(%r12), %ymm2, %ymm2
vpaddw 1344(%r12), %ymm2, %ymm2
vmovdqa 1728(%r12), %ymm4
vpsubw 1856(%r12), %ymm4, %ymm4
vmovdqa 2240(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 1984(%r12), %ymm5, %ymm5
vpsubw 1600(%r12), %ymm4, %ymm4
vpaddw 2112(%r12), %ymm4, %ymm4
vpsubw 832(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 64(%r12), %ymm1, %ymm1
vpaddw 1600(%r12), %ymm1, %ymm1
vmovdqa 448(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 1984(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 1216(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 64(%r12), %ymm8
vmovdqa 1216(%r12), %ymm9
vmovdqa %ymm8, 0(%rsp)
vmovdqa %ymm0, 32(%rsp)
vmovdqa %ymm1, 64(%rsp)
vmovdqa %ymm7, 96(%rsp)
vmovdqa %ymm5, 128(%rsp)
vmovdqa %ymm2, 160(%rsp)
vmovdqa %ymm3, 192(%rsp)
vmovdqa %ymm9, 224(%rsp)
vmovdqa 2496(%r12), %ymm0
vpsubw 2624(%r12), %ymm0, %ymm0
vmovdqa 3008(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 2752(%r12), %ymm1, %ymm1
vpsubw 2368(%r12), %ymm0, %ymm0
vpaddw 2880(%r12), %ymm0, %ymm0
vmovdqa 3264(%r12), %ymm2
vpsubw 3392(%r12), %ymm2, %ymm2
vmovdqa 3776(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 3520(%r12), %ymm3, %ymm3
vpsubw 3136(%r12), %ymm2, %ymm2
vpaddw 3648(%r12), %ymm2, %ymm2
vmovdqa 4032(%r12), %ymm4
vpsubw 4160(%r12), %ymm4, %ymm4
vmovdqa 4544(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 4288(%r12), %ymm5, %ymm5
vpsubw 3904(%r12), %ymm4, %ymm4
vpaddw 4416(%r12), %ymm4, %ymm4
vpsubw 3136(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 2368(%r12), %ymm1, %ymm1
vpaddw 3904(%r12), %ymm1, %ymm1
vmovdqa 2752(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 4288(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 3520(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 2368(%r12), %ymm8
vmovdqa 3520(%r12), %ymm9
vmovdqa %ymm8, 256(%rsp)
vmovdqa %ymm0, 288(%rsp)
vmovdqa %ymm1, 320(%rsp)
vmovdqa %ymm7, 352(%rsp)
vmovdqa %ymm5, 384(%rsp)
vmovdqa %ymm2, 416(%rsp)
vmovdqa %ymm3, 448(%rsp)
vmovdqa %ymm9, 480(%rsp)
vmovdqa 4800(%r12), %ymm0
vpsubw 4928(%r12), %ymm0, %ymm0
vmovdqa 5312(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5056(%r12), %ymm1, %ymm1
vpsubw 4672(%r12), %ymm0, %ymm0
vpaddw 5184(%r12), %ymm0, %ymm0
vmovdqa 5568(%r12), %ymm2
vpsubw 5696(%r12), %ymm2, %ymm2
vmovdqa 6080(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 5824(%r12), %ymm3, %ymm3
vpsubw 5440(%r12), %ymm2, %ymm2
vpaddw 5952(%r12), %ymm2, %ymm2
vmovdqa 6336(%r12), %ymm4
vpsubw 6464(%r12), %ymm4, %ymm4
vmovdqa 6848(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 6592(%r12), %ymm5, %ymm5
vpsubw 6208(%r12), %ymm4, %ymm4
vpaddw 6720(%r12), %ymm4, %ymm4
vpsubw 5440(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 4672(%r12), %ymm1, %ymm1
vpaddw 6208(%r12), %ymm1, %ymm1
vmovdqa 5056(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 6592(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 5824(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 4672(%r12), %ymm8
vmovdqa 5824(%r12), %ymm9
vmovdqa %ymm8, 512(%rsp)
vmovdqa %ymm0, 544(%rsp)
vmovdqa %ymm1, 576(%rsp)
vmovdqa %ymm7, 608(%rsp)
vmovdqa %ymm5, 640(%rsp)
vmovdqa %ymm2, 672(%rsp)
vmovdqa %ymm3, 704(%rsp)
vmovdqa %ymm9, 736(%rsp)
vmovdqa 7104(%r12), %ymm0
vpsubw 7232(%r12), %ymm0, %ymm0
vmovdqa 7616(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 7360(%r12), %ymm1, %ymm1
vpsubw 6976(%r12), %ymm0, %ymm0
vpaddw 7488(%r12), %ymm0, %ymm0
vmovdqa 7872(%r12), %ymm2
vpsubw 8000(%r12), %ymm2, %ymm2
vmovdqa 8384(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 8128(%r12), %ymm3, %ymm3
vpsubw 7744(%r12), %ymm2, %ymm2
vpaddw 8256(%r12), %ymm2, %ymm2
vmovdqa 8640(%r12), %ymm4
vpsubw 8768(%r12), %ymm4, %ymm4
vmovdqa 9152(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 8896(%r12), %ymm5, %ymm5
vpsubw 8512(%r12), %ymm4, %ymm4
vpaddw 9024(%r12), %ymm4, %ymm4
vpsubw 7744(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 6976(%r12), %ymm1, %ymm1
vpaddw 8512(%r12), %ymm1, %ymm1
vmovdqa 7360(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 8896(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 8128(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 6976(%r12), %ymm8
vmovdqa 8128(%r12), %ymm9
vmovdqa %ymm8, 768(%rsp)
vmovdqa %ymm0, 800(%rsp)
vmovdqa %ymm1, 832(%rsp)
vmovdqa %ymm7, 864(%rsp)
vmovdqa %ymm5, 896(%rsp)
vmovdqa %ymm2, 928(%rsp)
vmovdqa %ymm3, 960(%rsp)
vmovdqa %ymm9, 992(%rsp)
vmovdqa 9408(%r12), %ymm0
vpsubw 9536(%r12), %ymm0, %ymm0
vmovdqa 9920(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 9664(%r12), %ymm1, %ymm1
vpsubw 9280(%r12), %ymm0, %ymm0
vpaddw 9792(%r12), %ymm0, %ymm0
vmovdqa 10176(%r12), %ymm2
vpsubw 10304(%r12), %ymm2, %ymm2
vmovdqa 10688(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 10432(%r12), %ymm3, %ymm3
vpsubw 10048(%r12), %ymm2, %ymm2
vpaddw 10560(%r12), %ymm2, %ymm2
vmovdqa 10944(%r12), %ymm4
vpsubw 11072(%r12), %ymm4, %ymm4
vmovdqa 11456(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 11200(%r12), %ymm5, %ymm5
vpsubw 10816(%r12), %ymm4, %ymm4
vpaddw 11328(%r12), %ymm4, %ymm4
vpsubw 10048(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 9280(%r12), %ymm1, %ymm1
vpaddw 10816(%r12), %ymm1, %ymm1
vmovdqa 9664(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 11200(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 10432(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 9280(%r12), %ymm8
vmovdqa 10432(%r12), %ymm9
vmovdqa %ymm8, 1024(%rsp)
vmovdqa %ymm0, 1056(%rsp)
vmovdqa %ymm1, 1088(%rsp)
vmovdqa %ymm7, 1120(%rsp)
vmovdqa %ymm5, 1152(%rsp)
vmovdqa %ymm2, 1184(%rsp)
vmovdqa %ymm3, 1216(%rsp)
vmovdqa %ymm9, 1248(%rsp)
vmovdqa 11712(%r12), %ymm0
vpsubw 11840(%r12), %ymm0, %ymm0
vmovdqa 12224(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 11968(%r12), %ymm1, %ymm1
vpsubw 11584(%r12), %ymm0, %ymm0
vpaddw 12096(%r12), %ymm0, %ymm0
vmovdqa 12480(%r12), %ymm2
vpsubw 12608(%r12), %ymm2, %ymm2
vmovdqa 12992(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 12736(%r12), %ymm3, %ymm3
vpsubw 12352(%r12), %ymm2, %ymm2
vpaddw 12864(%r12), %ymm2, %ymm2
vmovdqa 13248(%r12), %ymm4
vpsubw 13376(%r12), %ymm4, %ymm4
vmovdqa 13760(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 13504(%r12), %ymm5, %ymm5
vpsubw 13120(%r12), %ymm4, %ymm4
vpaddw 13632(%r12), %ymm4, %ymm4
vpsubw 12352(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 11584(%r12), %ymm1, %ymm1
vpaddw 13120(%r12), %ymm1, %ymm1
vmovdqa 11968(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 13504(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 12736(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 11584(%r12), %ymm8
vmovdqa 12736(%r12), %ymm9
vmovdqa %ymm8, 1280(%rsp)
vmovdqa %ymm0, 1312(%rsp)
vmovdqa %ymm1, 1344(%rsp)
vmovdqa %ymm7, 1376(%rsp)
vmovdqa %ymm5, 1408(%rsp)
vmovdqa %ymm2, 1440(%rsp)
vmovdqa %ymm3, 1472(%rsp)
vmovdqa %ymm9, 1504(%rsp)
vmovdqa 14016(%r12), %ymm0
vpsubw 14144(%r12), %ymm0, %ymm0
vmovdqa 14528(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 14272(%r12), %ymm1, %ymm1
vpsubw 13888(%r12), %ymm0, %ymm0
vpaddw 14400(%r12), %ymm0, %ymm0
vmovdqa 14784(%r12), %ymm2
vpsubw 14912(%r12), %ymm2, %ymm2
vmovdqa 15296(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 15040(%r12), %ymm3, %ymm3
vpsubw 14656(%r12), %ymm2, %ymm2
vpaddw 15168(%r12), %ymm2, %ymm2
vmovdqa 15552(%r12), %ymm4
vpsubw 15680(%r12), %ymm4, %ymm4
vmovdqa 16064(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 15808(%r12), %ymm5, %ymm5
vpsubw 15424(%r12), %ymm4, %ymm4
vpaddw 15936(%r12), %ymm4, %ymm4
vpsubw 14656(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 13888(%r12), %ymm1, %ymm1
vpaddw 15424(%r12), %ymm1, %ymm1
vmovdqa 14272(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 15808(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 15040(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 13888(%r12), %ymm8
vmovdqa 15040(%r12), %ymm9
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
vmovdqu %ymm8, 64(%rdi)
vmovdqu 480(%rdi), %ymm8
vpaddw %ymm8, %ymm6, %ymm8
vmovdqu %ymm8, 480(%rdi)
vmovdqu 896(%rdi), %ymm8
vpaddw %ymm8, %ymm9, %ymm8
vmovdqu %ymm8, 896(%rdi)
vmovdqu 1312(%rdi), %ymm8
vpaddw %ymm8, %ymm10, %ymm8
vmovdqu %ymm8, 1312(%rdi)
vmovdqu 86(%rdi), %ymm8
vpaddw %ymm8, %ymm4, %ymm8
vmovdqu %ymm8, 86(%rdi)
vmovdqu 502(%rdi), %ymm8
vpaddw %ymm8, %ymm3, %ymm8
vmovdqu %ymm8, 502(%rdi)
vmovdqu 918(%rdi), %ymm8
vpaddw %ymm8, %ymm5, %ymm8
vmovdqu %ymm8, 918(%rdi)
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
vmovdqu 168(%rdi), %ymm4
vpaddw %ymm4, %ymm5, %ymm4
vmovdqu %ymm4, 168(%rdi)
vmovdqu 584(%rdi), %ymm4
vpaddw %ymm4, %ymm6, %ymm4
vmovdqu %ymm4, 584(%rdi)
vmovdqu 1000(%rdi), %ymm4
vpaddw %ymm4, %ymm7, %ymm4
vmovdqu %ymm4, 1000(%rdi)
vmovdqu 1416(%rdi), %ymm4
vpaddw %ymm4, %ymm8, %ymm4
vmovdqu %ymm4, 1416(%rdi)
vmovdqu 190(%rdi), %ymm4
vpaddw %ymm4, %ymm9, %ymm4
vmovdqu %ymm4, 190(%rdi)
vmovdqu 606(%rdi), %ymm4
vpaddw %ymm4, %ymm10, %ymm4
vmovdqu %ymm4, 606(%rdi)
vmovdqu 1022(%rdi), %ymm4
vpaddw %ymm4, %ymm11, %ymm4
vmovdqu %ymm4, 1022(%rdi)
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
vmovdqu 272(%rdi), %ymm9
vpaddw %ymm9, %ymm11, %ymm9
vmovdqu %ymm9, 272(%rdi)
vmovdqu 688(%rdi), %ymm9
vpaddw %ymm9, %ymm6, %ymm9
vmovdqu %ymm9, 688(%rdi)
vmovdqu 1104(%rdi), %ymm9
vpaddw %ymm9, %ymm3, %ymm9
vmovdqu %ymm9, 1104(%rdi)
vmovdqu 1520(%rdi), %ymm9
vpaddw %ymm9, %ymm4, %ymm9
vmovdqu %ymm9, 1520(%rdi)
vmovdqu 294(%rdi), %ymm9
vpaddw %ymm9, %ymm7, %ymm9
vmovdqu %ymm9, 294(%rdi)
vmovdqu 710(%rdi), %ymm9
vpaddw %ymm9, %ymm8, %ymm9
vmovdqu %ymm9, 710(%rdi)
vmovdqu 1126(%rdi), %ymm9
vpaddw %ymm9, %ymm5, %ymm9
vmovdqu %ymm9, 1126(%rdi)
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
vmovdqu 376(%rdi), %ymm7
vpaddw %ymm7, %ymm5, %ymm7
vmovdqu %ymm7, 376(%rdi)
vmovdqu 792(%rdi), %ymm7
vpaddw %ymm7, %ymm6, %ymm7
vmovdqu %ymm7, 792(%rdi)
vmovdqu 1208(%rdi), %ymm7
vpaddw %ymm7, %ymm10, %ymm7
vmovdqu %ymm7, 1208(%rdi)
vmovdqu 1624(%rdi), %ymm7
vpand mask_9_7(%rip), %ymm9, %ymm8
vpaddw %ymm7, %ymm8, %ymm7
vmovdqu %ymm7, 1624(%rdi)
vpshufb rol_rol_16(%rip), %ymm9, %ymm9
vextracti128 $1, %ymm9, %xmm9
vpand mask_7_9(%rip), %ymm9, %ymm9
vmovdqu 0(%rdi), %ymm7
vpaddw %ymm7, %ymm9, %ymm7
vmovdqu %ymm7, 0(%rdi)
vmovdqu 398(%rdi), %ymm7
vpaddw %ymm7, %ymm3, %ymm7
vmovdqu %ymm7, 398(%rdi)
vmovdqu 814(%rdi), %ymm7
vpaddw %ymm7, %ymm4, %ymm7
vmovdqu %ymm7, 814(%rdi)
vmovdqu 1230(%rdi), %ymm7
vpaddw %ymm7, %ymm11, %ymm7
vmovdqu %ymm7, 1230(%rdi)
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
vmovdqu 480(%rdi), %ymm3
vpaddw %ymm3, %ymm11, %ymm3
vmovdqu %ymm3, 480(%rdi)
vmovdqu 896(%rdi), %ymm3
vpaddw %ymm3, %ymm6, %ymm3
vmovdqu %ymm3, 896(%rdi)
vmovdqu 1312(%rdi), %ymm3
vpaddw %ymm3, %ymm8, %ymm3
vmovdqu %ymm3, 1312(%rdi)
vmovdqu 86(%rdi), %ymm3
vpaddw %ymm3, %ymm7, %ymm3
vmovdqu %ymm3, 86(%rdi)
vmovdqu 502(%rdi), %ymm3
vpaddw %ymm3, %ymm10, %ymm3
vmovdqu %ymm3, 502(%rdi)
vmovdqu 918(%rdi), %ymm3
vpaddw %ymm3, %ymm9, %ymm3
vmovdqu %ymm3, 918(%rdi)
vmovdqu 1334(%rdi), %ymm3
vpaddw %ymm3, %ymm5, %ymm3
vmovdqu %ymm3, 1334(%rdi)
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
vmovdqu 584(%rdi), %ymm10
vpaddw %ymm10, %ymm5, %ymm10
vmovdqu %ymm10, 584(%rdi)
vmovdqu 1000(%rdi), %ymm10
vpaddw %ymm10, %ymm6, %ymm10
vmovdqu %ymm10, 1000(%rdi)
vmovdqu 1416(%rdi), %ymm10
vpaddw %ymm10, %ymm4, %ymm10
vmovdqu %ymm10, 1416(%rdi)
vmovdqu 190(%rdi), %ymm10
vpaddw %ymm10, %ymm3, %ymm10
vmovdqu %ymm10, 190(%rdi)
vmovdqu 606(%rdi), %ymm10
vpaddw %ymm10, %ymm8, %ymm10
vmovdqu %ymm10, 606(%rdi)
vmovdqu 1022(%rdi), %ymm10
vpaddw %ymm10, %ymm7, %ymm10
vmovdqu %ymm10, 1022(%rdi)
vmovdqu 1438(%rdi), %ymm10
vpaddw %ymm10, %ymm11, %ymm10
vmovdqu %ymm10, 1438(%rdi)
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
vmovdqu 688(%rdi), %ymm8
vpaddw %ymm8, %ymm11, %ymm8
vmovdqu %ymm8, 688(%rdi)
vmovdqu 1104(%rdi), %ymm8
vpaddw %ymm8, %ymm6, %ymm8
vmovdqu %ymm8, 1104(%rdi)
vmovdqu 1520(%rdi), %ymm8
vpaddw %ymm8, %ymm9, %ymm8
vmovdqu %ymm8, 1520(%rdi)
vmovdqu 294(%rdi), %ymm8
vpaddw %ymm8, %ymm10, %ymm8
vmovdqu %ymm8, 294(%rdi)
vmovdqu 710(%rdi), %ymm8
vpaddw %ymm8, %ymm4, %ymm8
vmovdqu %ymm8, 710(%rdi)
vmovdqu 1126(%rdi), %ymm8
vpaddw %ymm8, %ymm3, %ymm8
vmovdqu %ymm8, 1126(%rdi)
vmovdqu 1542(%rdi), %ymm8
vpaddw %ymm8, %ymm5, %ymm8
vmovdqu %ymm8, 1542(%rdi)
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
vmovdqu 792(%rdi), %ymm4
vpaddw %ymm4, %ymm5, %ymm4
vmovdqu %ymm4, 792(%rdi)
vmovdqu 1208(%rdi), %ymm4
vpaddw %ymm4, %ymm6, %ymm4
vmovdqu %ymm4, 1208(%rdi)
vmovdqu 1624(%rdi), %ymm4
vpand mask_9_7(%rip), %ymm7, %ymm3
vpaddw %ymm4, %ymm3, %ymm4
vmovdqu %ymm4, 1624(%rdi)
vpshufb rol_rol_16(%rip), %ymm7, %ymm7
vextracti128 $1, %ymm7, %xmm7
vpand mask_7_9(%rip), %ymm7, %ymm7
vmovdqu 0(%rdi), %ymm4
vpaddw %ymm4, %ymm7, %ymm4
vmovdqu %ymm4, 0(%rdi)
vmovdqu 398(%rdi), %ymm4
vpaddw %ymm4, %ymm8, %ymm4
vmovdqu %ymm4, 398(%rdi)
vmovdqu 814(%rdi), %ymm4
vpaddw %ymm4, %ymm9, %ymm4
vmovdqu %ymm4, 814(%rdi)
vmovdqu 1230(%rdi), %ymm4
vpaddw %ymm4, %ymm10, %ymm4
vmovdqu %ymm4, 1230(%rdi)
vmovdqa 224(%r12), %ymm0
vpsubw 352(%r12), %ymm0, %ymm0
vmovdqa 736(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 480(%r12), %ymm1, %ymm1
vpsubw 96(%r12), %ymm0, %ymm0
vpaddw 608(%r12), %ymm0, %ymm0
vmovdqa 992(%r12), %ymm2
vpsubw 1120(%r12), %ymm2, %ymm2
vmovdqa 1504(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 1248(%r12), %ymm3, %ymm3
vpsubw 864(%r12), %ymm2, %ymm2
vpaddw 1376(%r12), %ymm2, %ymm2
vmovdqa 1760(%r12), %ymm4
vpsubw 1888(%r12), %ymm4, %ymm4
vmovdqa 2272(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 2016(%r12), %ymm5, %ymm5
vpsubw 1632(%r12), %ymm4, %ymm4
vpaddw 2144(%r12), %ymm4, %ymm4
vpsubw 864(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 96(%r12), %ymm1, %ymm1
vpaddw 1632(%r12), %ymm1, %ymm1
vmovdqa 480(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 2016(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 1248(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 96(%r12), %ymm8
vmovdqa 1248(%r12), %ymm9
vmovdqa %ymm8, 0(%rsp)
vmovdqa %ymm0, 32(%rsp)
vmovdqa %ymm1, 64(%rsp)
vmovdqa %ymm7, 96(%rsp)
vmovdqa %ymm5, 128(%rsp)
vmovdqa %ymm2, 160(%rsp)
vmovdqa %ymm3, 192(%rsp)
vmovdqa %ymm9, 224(%rsp)
vmovdqa 2528(%r12), %ymm0
vpsubw 2656(%r12), %ymm0, %ymm0
vmovdqa 3040(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 2784(%r12), %ymm1, %ymm1
vpsubw 2400(%r12), %ymm0, %ymm0
vpaddw 2912(%r12), %ymm0, %ymm0
vmovdqa 3296(%r12), %ymm2
vpsubw 3424(%r12), %ymm2, %ymm2
vmovdqa 3808(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 3552(%r12), %ymm3, %ymm3
vpsubw 3168(%r12), %ymm2, %ymm2
vpaddw 3680(%r12), %ymm2, %ymm2
vmovdqa 4064(%r12), %ymm4
vpsubw 4192(%r12), %ymm4, %ymm4
vmovdqa 4576(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 4320(%r12), %ymm5, %ymm5
vpsubw 3936(%r12), %ymm4, %ymm4
vpaddw 4448(%r12), %ymm4, %ymm4
vpsubw 3168(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 2400(%r12), %ymm1, %ymm1
vpaddw 3936(%r12), %ymm1, %ymm1
vmovdqa 2784(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 4320(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 3552(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 2400(%r12), %ymm8
vmovdqa 3552(%r12), %ymm9
vmovdqa %ymm8, 256(%rsp)
vmovdqa %ymm0, 288(%rsp)
vmovdqa %ymm1, 320(%rsp)
vmovdqa %ymm7, 352(%rsp)
vmovdqa %ymm5, 384(%rsp)
vmovdqa %ymm2, 416(%rsp)
vmovdqa %ymm3, 448(%rsp)
vmovdqa %ymm9, 480(%rsp)
vmovdqa 4832(%r12), %ymm0
vpsubw 4960(%r12), %ymm0, %ymm0
vmovdqa 5344(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 5088(%r12), %ymm1, %ymm1
vpsubw 4704(%r12), %ymm0, %ymm0
vpaddw 5216(%r12), %ymm0, %ymm0
vmovdqa 5600(%r12), %ymm2
vpsubw 5728(%r12), %ymm2, %ymm2
vmovdqa 6112(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 5856(%r12), %ymm3, %ymm3
vpsubw 5472(%r12), %ymm2, %ymm2
vpaddw 5984(%r12), %ymm2, %ymm2
vmovdqa 6368(%r12), %ymm4
vpsubw 6496(%r12), %ymm4, %ymm4
vmovdqa 6880(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 6624(%r12), %ymm5, %ymm5
vpsubw 6240(%r12), %ymm4, %ymm4
vpaddw 6752(%r12), %ymm4, %ymm4
vpsubw 5472(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 4704(%r12), %ymm1, %ymm1
vpaddw 6240(%r12), %ymm1, %ymm1
vmovdqa 5088(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 6624(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 5856(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 4704(%r12), %ymm8
vmovdqa 5856(%r12), %ymm9
vmovdqa %ymm8, 512(%rsp)
vmovdqa %ymm0, 544(%rsp)
vmovdqa %ymm1, 576(%rsp)
vmovdqa %ymm7, 608(%rsp)
vmovdqa %ymm5, 640(%rsp)
vmovdqa %ymm2, 672(%rsp)
vmovdqa %ymm3, 704(%rsp)
vmovdqa %ymm9, 736(%rsp)
vmovdqa 7136(%r12), %ymm0
vpsubw 7264(%r12), %ymm0, %ymm0
vmovdqa 7648(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 7392(%r12), %ymm1, %ymm1
vpsubw 7008(%r12), %ymm0, %ymm0
vpaddw 7520(%r12), %ymm0, %ymm0
vmovdqa 7904(%r12), %ymm2
vpsubw 8032(%r12), %ymm2, %ymm2
vmovdqa 8416(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 8160(%r12), %ymm3, %ymm3
vpsubw 7776(%r12), %ymm2, %ymm2
vpaddw 8288(%r12), %ymm2, %ymm2
vmovdqa 8672(%r12), %ymm4
vpsubw 8800(%r12), %ymm4, %ymm4
vmovdqa 9184(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 8928(%r12), %ymm5, %ymm5
vpsubw 8544(%r12), %ymm4, %ymm4
vpaddw 9056(%r12), %ymm4, %ymm4
vpsubw 7776(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 7008(%r12), %ymm1, %ymm1
vpaddw 8544(%r12), %ymm1, %ymm1
vmovdqa 7392(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 8928(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 8160(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 7008(%r12), %ymm8
vmovdqa 8160(%r12), %ymm9
vmovdqa %ymm8, 768(%rsp)
vmovdqa %ymm0, 800(%rsp)
vmovdqa %ymm1, 832(%rsp)
vmovdqa %ymm7, 864(%rsp)
vmovdqa %ymm5, 896(%rsp)
vmovdqa %ymm2, 928(%rsp)
vmovdqa %ymm3, 960(%rsp)
vmovdqa %ymm9, 992(%rsp)
vmovdqa 9440(%r12), %ymm0
vpsubw 9568(%r12), %ymm0, %ymm0
vmovdqa 9952(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 9696(%r12), %ymm1, %ymm1
vpsubw 9312(%r12), %ymm0, %ymm0
vpaddw 9824(%r12), %ymm0, %ymm0
vmovdqa 10208(%r12), %ymm2
vpsubw 10336(%r12), %ymm2, %ymm2
vmovdqa 10720(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 10464(%r12), %ymm3, %ymm3
vpsubw 10080(%r12), %ymm2, %ymm2
vpaddw 10592(%r12), %ymm2, %ymm2
vmovdqa 10976(%r12), %ymm4
vpsubw 11104(%r12), %ymm4, %ymm4
vmovdqa 11488(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 11232(%r12), %ymm5, %ymm5
vpsubw 10848(%r12), %ymm4, %ymm4
vpaddw 11360(%r12), %ymm4, %ymm4
vpsubw 10080(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 9312(%r12), %ymm1, %ymm1
vpaddw 10848(%r12), %ymm1, %ymm1
vmovdqa 9696(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 11232(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 10464(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 9312(%r12), %ymm8
vmovdqa 10464(%r12), %ymm9
vmovdqa %ymm8, 1024(%rsp)
vmovdqa %ymm0, 1056(%rsp)
vmovdqa %ymm1, 1088(%rsp)
vmovdqa %ymm7, 1120(%rsp)
vmovdqa %ymm5, 1152(%rsp)
vmovdqa %ymm2, 1184(%rsp)
vmovdqa %ymm3, 1216(%rsp)
vmovdqa %ymm9, 1248(%rsp)
vmovdqa 11744(%r12), %ymm0
vpsubw 11872(%r12), %ymm0, %ymm0
vmovdqa 12256(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 12000(%r12), %ymm1, %ymm1
vpsubw 11616(%r12), %ymm0, %ymm0
vpaddw 12128(%r12), %ymm0, %ymm0
vmovdqa 12512(%r12), %ymm2
vpsubw 12640(%r12), %ymm2, %ymm2
vmovdqa 13024(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 12768(%r12), %ymm3, %ymm3
vpsubw 12384(%r12), %ymm2, %ymm2
vpaddw 12896(%r12), %ymm2, %ymm2
vmovdqa 13280(%r12), %ymm4
vpsubw 13408(%r12), %ymm4, %ymm4
vmovdqa 13792(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 13536(%r12), %ymm5, %ymm5
vpsubw 13152(%r12), %ymm4, %ymm4
vpaddw 13664(%r12), %ymm4, %ymm4
vpsubw 12384(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 11616(%r12), %ymm1, %ymm1
vpaddw 13152(%r12), %ymm1, %ymm1
vmovdqa 12000(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 13536(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 12768(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 11616(%r12), %ymm8
vmovdqa 12768(%r12), %ymm9
vmovdqa %ymm8, 1280(%rsp)
vmovdqa %ymm0, 1312(%rsp)
vmovdqa %ymm1, 1344(%rsp)
vmovdqa %ymm7, 1376(%rsp)
vmovdqa %ymm5, 1408(%rsp)
vmovdqa %ymm2, 1440(%rsp)
vmovdqa %ymm3, 1472(%rsp)
vmovdqa %ymm9, 1504(%rsp)
vmovdqa 14048(%r12), %ymm0
vpsubw 14176(%r12), %ymm0, %ymm0
vmovdqa 14560(%r12), %ymm1
vpsubw %ymm0, %ymm1, %ymm1
vpsubw 14304(%r12), %ymm1, %ymm1
vpsubw 13920(%r12), %ymm0, %ymm0
vpaddw 14432(%r12), %ymm0, %ymm0
vmovdqa 14816(%r12), %ymm2
vpsubw 14944(%r12), %ymm2, %ymm2
vmovdqa 15328(%r12), %ymm3
vpsubw %ymm2, %ymm3, %ymm3
vpsubw 15072(%r12), %ymm3, %ymm3
vpsubw 14688(%r12), %ymm2, %ymm2
vpaddw 15200(%r12), %ymm2, %ymm2
vmovdqa 15584(%r12), %ymm4
vpsubw 15712(%r12), %ymm4, %ymm4
vmovdqa 16096(%r12), %ymm5
vpsubw %ymm4, %ymm5, %ymm5
vpsubw 15840(%r12), %ymm5, %ymm5
vpsubw 15456(%r12), %ymm4, %ymm4
vpaddw 15968(%r12), %ymm4, %ymm4
vpsubw 14688(%r12), %ymm1, %ymm1
vpsubw %ymm1, %ymm5, %ymm5
vpsubw %ymm3, %ymm5, %ymm5
vpsubw 13920(%r12), %ymm1, %ymm1
vpaddw 15456(%r12), %ymm1, %ymm1
vmovdqa 14304(%r12), %ymm6
vpsubw %ymm2, %ymm6, %ymm7
vmovdqa 15840(%r12), %ymm2
vpsubw %ymm7, %ymm2, %ymm2
vpsubw 15072(%r12), %ymm2, %ymm2
vpsubw %ymm0, %ymm7, %ymm7
vpaddw %ymm4, %ymm7, %ymm7
vmovdqa 13920(%r12), %ymm8
vmovdqa 15072(%r12), %ymm9
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
vmovdqu 96(%rdi), %ymm9
vpaddw %ymm9, %ymm11, %ymm9
vmovq %xmm9, 96(%rdi)
vmovdqu 512(%rdi), %ymm9
vpaddw %ymm9, %ymm6, %ymm9
vmovq %xmm9, 512(%rdi)
vmovdqu 928(%rdi), %ymm9
vpaddw %ymm9, %ymm3, %ymm9
vmovq %xmm9, 928(%rdi)
vmovdqu 1344(%rdi), %ymm9
vpaddw %ymm9, %ymm4, %ymm9
vmovq %xmm9, 1344(%rdi)
vmovdqu 118(%rdi), %ymm9
vpaddw %ymm9, %ymm7, %ymm9
vmovq %xmm9, 118(%rdi)
vmovdqu 534(%rdi), %ymm9
vpaddw %ymm9, %ymm8, %ymm9
vmovq %xmm9, 534(%rdi)
vmovdqu 950(%rdi), %ymm9
vpaddw %ymm9, %ymm5, %ymm9
vmovq %xmm9, 950(%rdi)
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
vmovdqu 200(%rdi), %ymm7
vpaddw %ymm7, %ymm5, %ymm7
vmovq %xmm7, 200(%rdi)
vmovdqu 616(%rdi), %ymm7
vpaddw %ymm7, %ymm6, %ymm7
vmovq %xmm7, 616(%rdi)
vmovdqu 1032(%rdi), %ymm7
vpaddw %ymm7, %ymm10, %ymm7
vmovq %xmm7, 1032(%rdi)
vmovdqu 1448(%rdi), %ymm7
vpaddw %ymm7, %ymm9, %ymm7
vmovq %xmm7, 1448(%rdi)
vmovdqu 222(%rdi), %ymm7
vpaddw %ymm7, %ymm3, %ymm7
vmovq %xmm7, 222(%rdi)
vmovdqu 638(%rdi), %ymm7
vpaddw %ymm7, %ymm4, %ymm7
vmovq %xmm7, 638(%rdi)
vmovdqu 1054(%rdi), %ymm7
vpaddw %ymm7, %ymm11, %ymm7
vmovq %xmm7, 1054(%rdi)
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
vmovdqu 304(%rdi), %ymm3
vpaddw %ymm3, %ymm11, %ymm3
vmovq %xmm3, 304(%rdi)
vmovdqu 720(%rdi), %ymm3
vpaddw %ymm3, %ymm6, %ymm3
vmovq %xmm3, 720(%rdi)
vmovdqu 1136(%rdi), %ymm3
vpaddw %ymm3, %ymm8, %ymm3
vmovq %xmm3, 1136(%rdi)
vmovdqu 1552(%rdi), %ymm3
vpaddw %ymm3, %ymm7, %ymm3
vmovq %xmm3, 1552(%rdi)
vmovdqu 326(%rdi), %ymm3
vpaddw %ymm3, %ymm10, %ymm3
vmovq %xmm3, 326(%rdi)
vmovdqu 742(%rdi), %ymm3
vpaddw %ymm3, %ymm9, %ymm3
vmovq %xmm3, 742(%rdi)
vmovdqu 1158(%rdi), %ymm3
vpaddw %ymm3, %ymm5, %ymm3
vmovq %xmm3, 1158(%rdi)
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
vmovdqu 408(%rdi), %ymm10
vpaddw %ymm10, %ymm5, %ymm10
vmovq %xmm10, 408(%rdi)
vmovdqu 824(%rdi), %ymm10
vpaddw %ymm10, %ymm6, %ymm10
vmovq %xmm10, 824(%rdi)
vmovdqu 1240(%rdi), %ymm10
vpaddw %ymm10, %ymm4, %ymm10
vmovq %xmm10, 1240(%rdi)
vmovdqu 14(%rdi), %ymm10
vpaddw %ymm10, %ymm3, %ymm10
vmovq %xmm10, 14(%rdi)
vmovdqu 430(%rdi), %ymm10
vpaddw %ymm10, %ymm8, %ymm10
vmovq %xmm10, 430(%rdi)
vmovdqu 846(%rdi), %ymm10
vpaddw %ymm10, %ymm7, %ymm10
vmovq %xmm10, 846(%rdi)
vmovdqu 1262(%rdi), %ymm10
vpaddw %ymm10, %ymm11, %ymm10
vmovq %xmm10, 1262(%rdi)
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
vmovdqu 512(%rdi), %ymm8
vpaddw %ymm8, %ymm11, %ymm8
vmovq %xmm8, 512(%rdi)
vmovdqu 928(%rdi), %ymm8
vpaddw %ymm8, %ymm6, %ymm8
vmovq %xmm8, 928(%rdi)
vmovdqu 1344(%rdi), %ymm8
vpaddw %ymm8, %ymm9, %ymm8
vmovq %xmm8, 1344(%rdi)
vmovdqu 118(%rdi), %ymm8
vpaddw %ymm8, %ymm10, %ymm8
vmovq %xmm8, 118(%rdi)
vmovdqu 534(%rdi), %ymm8
vpaddw %ymm8, %ymm4, %ymm8
vmovq %xmm8, 534(%rdi)
vmovdqu 950(%rdi), %ymm8
vpaddw %ymm8, %ymm3, %ymm8
vmovq %xmm8, 950(%rdi)
vmovdqu 1366(%rdi), %ymm8
vpaddw %ymm8, %ymm5, %ymm8
vmovq %xmm8, 1366(%rdi)
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
vmovdqu 616(%rdi), %ymm4
vpaddw %ymm4, %ymm5, %ymm4
vmovq %xmm4, 616(%rdi)
vmovdqu 1032(%rdi), %ymm4
vpaddw %ymm4, %ymm6, %ymm4
vmovq %xmm4, 1032(%rdi)
vmovdqu 1448(%rdi), %ymm4
vpaddw %ymm4, %ymm7, %ymm4
vmovq %xmm4, 1448(%rdi)
vmovdqu 222(%rdi), %ymm4
vpaddw %ymm4, %ymm8, %ymm4
vmovq %xmm4, 222(%rdi)
vmovdqu 638(%rdi), %ymm4
vpaddw %ymm4, %ymm9, %ymm4
vmovq %xmm4, 638(%rdi)
vmovdqu 1054(%rdi), %ymm4
vpaddw %ymm4, %ymm10, %ymm4
vmovq %xmm4, 1054(%rdi)
vmovdqu 1470(%rdi), %ymm4
vpaddw %ymm4, %ymm11, %ymm4
vmovq %xmm4, 1470(%rdi)
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
vmovdqu 720(%rdi), %ymm9
vpaddw %ymm9, %ymm11, %ymm9
vmovq %xmm9, 720(%rdi)
vmovdqu 1136(%rdi), %ymm9
vpaddw %ymm9, %ymm6, %ymm9
vmovq %xmm9, 1136(%rdi)
vmovdqu 1552(%rdi), %ymm9
vpaddw %ymm9, %ymm3, %ymm9
vmovq %xmm9, 1552(%rdi)
vmovdqu 326(%rdi), %ymm9
vpaddw %ymm9, %ymm4, %ymm9
vmovq %xmm9, 326(%rdi)
vmovdqu 742(%rdi), %ymm9
vpaddw %ymm9, %ymm7, %ymm9
vmovq %xmm9, 742(%rdi)
vmovdqu 1158(%rdi), %ymm9
vpaddw %ymm9, %ymm8, %ymm9
vmovq %xmm9, 1158(%rdi)
vmovdqu 1574(%rdi), %ymm9
vpaddw %ymm9, %ymm5, %ymm9
vmovq %xmm9, 1574(%rdi)
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
vmovdqu 824(%rdi), %ymm7
vpaddw %ymm7, %ymm5, %ymm7
vmovq %xmm7, 824(%rdi)
vmovdqu 1240(%rdi), %ymm7
vpaddw %ymm7, %ymm6, %ymm7
vmovq %xmm7, 1240(%rdi)
vmovdqu 14(%rdi), %ymm7
vpaddw %ymm7, %ymm10, %ymm7
vmovq %xmm7, 14(%rdi)
vmovdqu 430(%rdi), %ymm7
vpaddw %ymm7, %ymm9, %ymm7
vmovq %xmm7, 430(%rdi)
vmovdqu 846(%rdi), %ymm7
vpaddw %ymm7, %ymm3, %ymm7
vmovq %xmm7, 846(%rdi)
vmovdqu 1262(%rdi), %ymm7
vpaddw %ymm7, %ymm4, %ymm7
vmovq %xmm7, 1262(%rdi)
mov %r8, %rsp
pop %r12
ret
