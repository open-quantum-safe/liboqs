.data
.p2align 5
.text
.global PQCLEAN_NTRUHPS4096821_AVX2_poly_mod_q_Phi_n
.global _PQCLEAN_NTRUHPS4096821_AVX2_poly_mod_q_Phi_n
PQCLEAN_NTRUHPS4096821_AVX2_poly_mod_q_Phi_n:
_PQCLEAN_NTRUHPS4096821_AVX2_poly_mod_q_Phi_n:
vmovdqa 1632(%rdi), %ymm0
vpermq $1, %ymm0, %ymm0
vpslld $16, %ymm0, %ymm0
vpsrld $16, %ymm0, %ymm1
vpor %ymm0, %ymm1, %ymm0
vbroadcastss %xmm0, %ymm0
vxorpd %ymm1, %ymm1, %ymm1
vpsubw %ymm0, %ymm1, %ymm0
vpaddw 0(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 0(%rdi)
vpaddw 32(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 32(%rdi)
vpaddw 64(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 64(%rdi)
vpaddw 96(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 96(%rdi)
vpaddw 128(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 128(%rdi)
vpaddw 160(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 160(%rdi)
vpaddw 192(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 192(%rdi)
vpaddw 224(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 224(%rdi)
vpaddw 256(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 256(%rdi)
vpaddw 288(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 288(%rdi)
vpaddw 320(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 320(%rdi)
vpaddw 352(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 352(%rdi)
vpaddw 384(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 384(%rdi)
vpaddw 416(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 416(%rdi)
vpaddw 448(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 448(%rdi)
vpaddw 480(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 480(%rdi)
vpaddw 512(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 512(%rdi)
vpaddw 544(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 544(%rdi)
vpaddw 576(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 576(%rdi)
vpaddw 608(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 608(%rdi)
vpaddw 640(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 640(%rdi)
vpaddw 672(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 672(%rdi)
vpaddw 704(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 704(%rdi)
vpaddw 736(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 736(%rdi)
vpaddw 768(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 768(%rdi)
vpaddw 800(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 800(%rdi)
vpaddw 832(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 832(%rdi)
vpaddw 864(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 864(%rdi)
vpaddw 896(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 896(%rdi)
vpaddw 928(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 928(%rdi)
vpaddw 960(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 960(%rdi)
vpaddw 992(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 992(%rdi)
vpaddw 1024(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1024(%rdi)
vpaddw 1056(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1056(%rdi)
vpaddw 1088(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1088(%rdi)
vpaddw 1120(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1120(%rdi)
vpaddw 1152(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1152(%rdi)
vpaddw 1184(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1184(%rdi)
vpaddw 1216(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1216(%rdi)
vpaddw 1248(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1248(%rdi)
vpaddw 1280(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1280(%rdi)
vpaddw 1312(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1312(%rdi)
vpaddw 1344(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1344(%rdi)
vpaddw 1376(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1376(%rdi)
vpaddw 1408(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1408(%rdi)
vpaddw 1440(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1440(%rdi)
vpaddw 1472(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1472(%rdi)
vpaddw 1504(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1504(%rdi)
vpaddw 1536(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1536(%rdi)
vpaddw 1568(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1568(%rdi)
vpaddw 1600(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1600(%rdi)
vpaddw 1632(%rdi), %ymm0, %ymm1
vmovdqa %ymm1, 1632(%rdi)
ret
