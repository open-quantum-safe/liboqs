/* Taken from Bas Westerbaan's new 4-way SHAKE implementation
 * for Sphincs+ (https://github.com/sphincs/sphincsplus/pull/14/),
 * but uses vpshufb for byte-granular rotations as in the Keccak Code Package. */

#include "cdecl.h"

.data
.p2align 5
rho8:
.byte 7,0,1,2,3,4,5,6,15,8,9,10,11,12,13,14,7,0,1,2,3,4,5,6,15,8,9,10,11,12,13,14
rho56:
.byte 1,2,3,4,5,6,7,0,9,10,11,12,13,14,15,8,1,2,3,4,5,6,7,0,9,10,11,12,13,14,15,8

.text
.global cdecl(PQCLEAN_DILITHIUM3_AVX2_f1600x4)
.global _cdecl(PQCLEAN_DILITHIUM3_AVX2_f1600x4)
cdecl(PQCLEAN_DILITHIUM3_AVX2_f1600x4):
_cdecl(PQCLEAN_DILITHIUM3_AVX2_f1600x4):
vmovdqa		rho8(%rip), %ymm0
movq		$6, %rax
looptop:
vmovdqa		0(%rdi), %ymm8
vmovdqa		32(%rdi), %ymm9
vmovdqa		64(%rdi), %ymm10
vmovdqa		96(%rdi), %ymm11
vmovdqa		128(%rdi), %ymm12
vpxor		160(%rdi), %ymm8, %ymm8
vpxor		192(%rdi), %ymm9, %ymm9
vpxor		224(%rdi), %ymm10, %ymm10
vpxor		256(%rdi), %ymm11, %ymm11
vpxor		288(%rdi), %ymm12, %ymm12
vpxor		320(%rdi), %ymm8, %ymm8
vpxor		352(%rdi), %ymm9, %ymm9
vpxor		384(%rdi), %ymm10, %ymm10
vpxor		416(%rdi), %ymm11, %ymm11
vpxor		448(%rdi), %ymm12, %ymm12
vpxor		480(%rdi), %ymm8, %ymm8
vpxor		512(%rdi), %ymm9, %ymm9
vpxor		544(%rdi), %ymm10, %ymm10
vpxor		576(%rdi), %ymm11, %ymm11
vpxor		608(%rdi), %ymm12, %ymm12
vpxor		640(%rdi), %ymm8, %ymm8
vpxor		672(%rdi), %ymm9, %ymm9
vpxor		704(%rdi), %ymm10, %ymm10
vpxor		736(%rdi), %ymm11, %ymm11
vpxor		768(%rdi), %ymm12, %ymm12
vpsllq		$1, %ymm9, %ymm13
vpsllq		$1, %ymm10, %ymm14
vpsllq		$1, %ymm11, %ymm15
vpsllq		$1, %ymm12, %ymm7
vpsllq		$1, %ymm8, %ymm6
vpsrlq		$63, %ymm9, %ymm5
vpsrlq		$63, %ymm10, %ymm4
vpsrlq		$63, %ymm11, %ymm3
vpsrlq		$63, %ymm12, %ymm2
vpsrlq		$63, %ymm8, %ymm1
vpor		%ymm13, %ymm5, %ymm5
vpor		%ymm14, %ymm4, %ymm4
vpor		%ymm15, %ymm3, %ymm3
vpor		%ymm7, %ymm2, %ymm2
vpor		%ymm6, %ymm1, %ymm1
vpxor		%ymm5, %ymm12, %ymm5
vpxor		%ymm4, %ymm8, %ymm4
vpxor		%ymm3, %ymm9, %ymm3
vpxor		%ymm2, %ymm10, %ymm2
vpxor		%ymm1, %ymm11, %ymm1
vpxor		0(%rdi), %ymm5, %ymm8
vpxor		192(%rdi), %ymm4, %ymm9
vpxor		384(%rdi), %ymm3, %ymm10
vpxor		576(%rdi), %ymm2, %ymm11
vpxor		768(%rdi), %ymm1, %ymm12
vpsllq		$44, %ymm9, %ymm14
vpsllq		$43, %ymm10, %ymm15
vpsllq		$21, %ymm11, %ymm7
vpsllq		$14, %ymm12, %ymm6
vpsrlq		$20, %ymm9, %ymm9
vpsrlq		$21, %ymm10, %ymm10
vpsrlq		$43, %ymm11, %ymm11
vpsrlq		$50, %ymm12, %ymm12
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
vpor		%ymm7, %ymm11, %ymm11
vpor		%ymm6, %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vpbroadcastq	0(%rsi), %ymm8
vpxor		%ymm8, %ymm13, %ymm13
vmovdqa		%ymm13, 0(%rdi)
vmovdqa		%ymm14, 192(%rdi)
vmovdqa		%ymm15, 384(%rdi)
vmovdqa		%ymm7, 576(%rdi)
vmovdqa		%ymm6, 768(%rdi)
vpxor		96(%rdi), %ymm2, %ymm8
vpxor		288(%rdi), %ymm1, %ymm9
vpxor		320(%rdi), %ymm5, %ymm10
vpxor		512(%rdi), %ymm4, %ymm11
vpxor		704(%rdi), %ymm3, %ymm12
vpsllq		$28, %ymm8, %ymm13
vpsllq		$20, %ymm9, %ymm14
vpsllq		$3, %ymm10, %ymm15
vpsllq		$45, %ymm11, %ymm7
vpsllq		$61, %ymm12, %ymm6
vpsrlq		$36, %ymm8, %ymm8
vpsrlq		$44, %ymm9, %ymm9
vpsrlq		$61, %ymm10, %ymm10
vpsrlq		$19, %ymm11, %ymm11
vpsrlq		$3, %ymm12, %ymm12
vpor		%ymm13, %ymm8, %ymm8
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
vpor		%ymm7, %ymm11, %ymm11
vpor		%ymm6, %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vmovdqa		%ymm13, 320(%rdi)
vmovdqa		%ymm14, 512(%rdi)
vmovdqa		%ymm15, 704(%rdi)
vmovdqa		%ymm7, 96(%rdi)
vmovdqa		%ymm6, 288(%rdi)
vpxor		32(%rdi), %ymm4, %ymm8
vpxor		224(%rdi), %ymm3, %ymm9
vpxor		416(%rdi), %ymm2, %ymm10
vpxor		608(%rdi), %ymm1, %ymm11
vpxor		640(%rdi), %ymm5, %ymm12
vpsllq		$1, %ymm8, %ymm13
vpsllq		$6, %ymm9, %ymm14
vpsllq		$25, %ymm10, %ymm15
#vpsllq		$8, %ymm11, %ymm7
vpsllq		$18, %ymm12, %ymm6
vpsrlq		$63, %ymm8, %ymm8
vpsrlq		$58, %ymm9, %ymm9
vpsrlq		$39, %ymm10, %ymm10
#vpsrlq		$56, %ymm11, %ymm11
vpsrlq		$46, %ymm12, %ymm12
vpor		%ymm13, %ymm8, %ymm8
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
#vpor		%ymm7, %ymm11, %ymm11
vpshufb		%ymm0, %ymm11, %ymm11
vpor		%ymm6, %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vmovdqa		%ymm13, 640(%rdi)
vmovdqa		%ymm14, 32(%rdi)
vmovdqa		%ymm15, 224(%rdi)
vmovdqa		%ymm7, 416(%rdi)
vmovdqa		%ymm6, 608(%rdi)
vpxor		128(%rdi), %ymm1, %ymm8
vpxor		160(%rdi), %ymm5, %ymm9
vpxor		352(%rdi), %ymm4, %ymm10
vpxor		544(%rdi), %ymm3, %ymm11
vpxor		736(%rdi), %ymm2, %ymm12
vpsllq		$27, %ymm8, %ymm13
vpsllq		$36, %ymm9, %ymm14
vpsllq		$10, %ymm10, %ymm15
vpsllq		$15, %ymm11, %ymm7
#vpsllq		$56, %ymm12, %ymm6
vpsrlq		$37, %ymm8, %ymm8
vpsrlq		$28, %ymm9, %ymm9
vpsrlq		$54, %ymm10, %ymm10
vpsrlq		$49, %ymm11, %ymm11
#vpsrlq		$8, %ymm12, %ymm12
vpor		%ymm13, %ymm8, %ymm8
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
vpor		%ymm7, %ymm11, %ymm11
#vpor		%ymm6, %ymm12, %ymm12
vpshufb		rho56(%rip), %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vmovdqa		%ymm13, 160(%rdi)
vmovdqa		%ymm14, 352(%rdi)
vmovdqa		%ymm15, 544(%rdi)
vmovdqa		%ymm7, 736(%rdi)
vmovdqa		%ymm6, 128(%rdi)
vpxor		64(%rdi), %ymm3, %ymm8
vpxor		256(%rdi), %ymm2, %ymm9
vpxor		448(%rdi), %ymm1, %ymm10
vpxor		480(%rdi), %ymm5, %ymm11
vpxor		672(%rdi), %ymm4, %ymm12
vpsllq		$62, %ymm8, %ymm13
vpsllq		$55, %ymm9, %ymm14
vpsllq		$39, %ymm10, %ymm15
vpsllq		$41, %ymm11, %ymm7
vpsllq		$2, %ymm12, %ymm6
vpsrlq		$2, %ymm8, %ymm8
vpsrlq		$9, %ymm9, %ymm9
vpsrlq		$25, %ymm10, %ymm10
vpsrlq		$23, %ymm11, %ymm11
vpsrlq		$62, %ymm12, %ymm12
vpor		%ymm13, %ymm8, %ymm8
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
vpor		%ymm7, %ymm11, %ymm11
vpor		%ymm6, %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vmovdqa		%ymm13, 480(%rdi)
vmovdqa		%ymm14, 672(%rdi)
vmovdqa		%ymm15, 64(%rdi)
vmovdqa		%ymm7, 256(%rdi)
vmovdqa		%ymm6, 448(%rdi)
vmovdqa		0(%rdi), %ymm8
vmovdqa		32(%rdi), %ymm9
vmovdqa		64(%rdi), %ymm10
vmovdqa		96(%rdi), %ymm11
vmovdqa		128(%rdi), %ymm12
vpxor		160(%rdi), %ymm8, %ymm8
vpxor		192(%rdi), %ymm9, %ymm9
vpxor		224(%rdi), %ymm10, %ymm10
vpxor		256(%rdi), %ymm11, %ymm11
vpxor		288(%rdi), %ymm12, %ymm12
vpxor		320(%rdi), %ymm8, %ymm8
vpxor		352(%rdi), %ymm9, %ymm9
vpxor		384(%rdi), %ymm10, %ymm10
vpxor		416(%rdi), %ymm11, %ymm11
vpxor		448(%rdi), %ymm12, %ymm12
vpxor		480(%rdi), %ymm8, %ymm8
vpxor		512(%rdi), %ymm9, %ymm9
vpxor		544(%rdi), %ymm10, %ymm10
vpxor		576(%rdi), %ymm11, %ymm11
vpxor		608(%rdi), %ymm12, %ymm12
vpxor		640(%rdi), %ymm8, %ymm8
vpxor		672(%rdi), %ymm9, %ymm9
vpxor		704(%rdi), %ymm10, %ymm10
vpxor		736(%rdi), %ymm11, %ymm11
vpxor		768(%rdi), %ymm12, %ymm12
vpsllq		$1, %ymm9, %ymm13
vpsllq		$1, %ymm10, %ymm14
vpsllq		$1, %ymm11, %ymm15
vpsllq		$1, %ymm12, %ymm7
vpsllq		$1, %ymm8, %ymm6
vpsrlq		$63, %ymm9, %ymm5
vpsrlq		$63, %ymm10, %ymm4
vpsrlq		$63, %ymm11, %ymm3
vpsrlq		$63, %ymm12, %ymm2
vpsrlq		$63, %ymm8, %ymm1
vpor		%ymm13, %ymm5, %ymm5
vpor		%ymm14, %ymm4, %ymm4
vpor		%ymm15, %ymm3, %ymm3
vpor		%ymm7, %ymm2, %ymm2
vpor		%ymm6, %ymm1, %ymm1
vpxor		%ymm5, %ymm12, %ymm5
vpxor		%ymm4, %ymm8, %ymm4
vpxor		%ymm3, %ymm9, %ymm3
vpxor		%ymm2, %ymm10, %ymm2
vpxor		%ymm1, %ymm11, %ymm1
vpxor		0(%rdi), %ymm5, %ymm8
vpxor		512(%rdi), %ymm4, %ymm9
vpxor		224(%rdi), %ymm3, %ymm10
vpxor		736(%rdi), %ymm2, %ymm11
vpxor		448(%rdi), %ymm1, %ymm12
vpsllq		$44, %ymm9, %ymm14
vpsllq		$43, %ymm10, %ymm15
vpsllq		$21, %ymm11, %ymm7
vpsllq		$14, %ymm12, %ymm6
vpsrlq		$20, %ymm9, %ymm9
vpsrlq		$21, %ymm10, %ymm10
vpsrlq		$43, %ymm11, %ymm11
vpsrlq		$50, %ymm12, %ymm12
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
vpor		%ymm7, %ymm11, %ymm11
vpor		%ymm6, %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vpbroadcastq	8(%rsi), %ymm8
vpxor		%ymm8, %ymm13, %ymm13
vmovdqa		%ymm13, 0(%rdi)
vmovdqa		%ymm14, 512(%rdi)
vmovdqa		%ymm15, 224(%rdi)
vmovdqa		%ymm7, 736(%rdi)
vmovdqa		%ymm6, 448(%rdi)
vpxor		576(%rdi), %ymm2, %ymm8
vpxor		288(%rdi), %ymm1, %ymm9
vpxor		640(%rdi), %ymm5, %ymm10
vpxor		352(%rdi), %ymm4, %ymm11
vpxor		64(%rdi), %ymm3, %ymm12
vpsllq		$28, %ymm8, %ymm13
vpsllq		$20, %ymm9, %ymm14
vpsllq		$3, %ymm10, %ymm15
vpsllq		$45, %ymm11, %ymm7
vpsllq		$61, %ymm12, %ymm6
vpsrlq		$36, %ymm8, %ymm8
vpsrlq		$44, %ymm9, %ymm9
vpsrlq		$61, %ymm10, %ymm10
vpsrlq		$19, %ymm11, %ymm11
vpsrlq		$3, %ymm12, %ymm12
vpor		%ymm13, %ymm8, %ymm8
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
vpor		%ymm7, %ymm11, %ymm11
vpor		%ymm6, %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vmovdqa		%ymm13, 640(%rdi)
vmovdqa		%ymm14, 352(%rdi)
vmovdqa		%ymm15, 64(%rdi)
vmovdqa		%ymm7, 576(%rdi)
vmovdqa		%ymm6, 288(%rdi)
vpxor		192(%rdi), %ymm4, %ymm8
vpxor		704(%rdi), %ymm3, %ymm9
vpxor		416(%rdi), %ymm2, %ymm10
vpxor		128(%rdi), %ymm1, %ymm11
vpxor		480(%rdi), %ymm5, %ymm12
vpsllq		$1, %ymm8, %ymm13
vpsllq		$6, %ymm9, %ymm14
vpsllq		$25, %ymm10, %ymm15
#vpsllq		$8, %ymm11, %ymm7
vpsllq		$18, %ymm12, %ymm6
vpsrlq		$63, %ymm8, %ymm8
vpsrlq		$58, %ymm9, %ymm9
vpsrlq		$39, %ymm10, %ymm10
#vpsrlq		$56, %ymm11, %ymm11
vpsrlq		$46, %ymm12, %ymm12
vpor		%ymm13, %ymm8, %ymm8
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
#vpor		%ymm7, %ymm11, %ymm11
vpshufb		%ymm0, %ymm11, %ymm11
vpor		%ymm6, %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vmovdqa		%ymm13, 480(%rdi)
vmovdqa		%ymm14, 192(%rdi)
vmovdqa		%ymm15, 704(%rdi)
vmovdqa		%ymm7, 416(%rdi)
vmovdqa		%ymm6, 128(%rdi)
vpxor		768(%rdi), %ymm1, %ymm8
vpxor		320(%rdi), %ymm5, %ymm9
vpxor		32(%rdi), %ymm4, %ymm10
vpxor		544(%rdi), %ymm3, %ymm11
vpxor		256(%rdi), %ymm2, %ymm12
vpsllq		$27, %ymm8, %ymm13
vpsllq		$36, %ymm9, %ymm14
vpsllq		$10, %ymm10, %ymm15
vpsllq		$15, %ymm11, %ymm7
#vpsllq		$56, %ymm12, %ymm6
vpsrlq		$37, %ymm8, %ymm8
vpsrlq		$28, %ymm9, %ymm9
vpsrlq		$54, %ymm10, %ymm10
vpsrlq		$49, %ymm11, %ymm11
#vpsrlq		$8, %ymm12, %ymm12
vpor		%ymm13, %ymm8, %ymm8
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
vpor		%ymm7, %ymm11, %ymm11
#vpor		%ymm6, %ymm12, %ymm12
vpshufb		rho56(%rip), %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vmovdqa		%ymm13, 320(%rdi)
vmovdqa		%ymm14, 32(%rdi)
vmovdqa		%ymm15, 544(%rdi)
vmovdqa		%ymm7, 256(%rdi)
vmovdqa		%ymm6, 768(%rdi)
vpxor		384(%rdi), %ymm3, %ymm8
vpxor		96(%rdi), %ymm2, %ymm9
vpxor		608(%rdi), %ymm1, %ymm10
vpxor		160(%rdi), %ymm5, %ymm11
vpxor		672(%rdi), %ymm4, %ymm12
vpsllq		$62, %ymm8, %ymm13
vpsllq		$55, %ymm9, %ymm14
vpsllq		$39, %ymm10, %ymm15
vpsllq		$41, %ymm11, %ymm7
vpsllq		$2, %ymm12, %ymm6
vpsrlq		$2, %ymm8, %ymm8
vpsrlq		$9, %ymm9, %ymm9
vpsrlq		$25, %ymm10, %ymm10
vpsrlq		$23, %ymm11, %ymm11
vpsrlq		$62, %ymm12, %ymm12
vpor		%ymm13, %ymm8, %ymm8
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
vpor		%ymm7, %ymm11, %ymm11
vpor		%ymm6, %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vmovdqa		%ymm13, 160(%rdi)
vmovdqa		%ymm14, 672(%rdi)
vmovdqa		%ymm15, 384(%rdi)
vmovdqa		%ymm7, 96(%rdi)
vmovdqa		%ymm6, 608(%rdi)
vmovdqa		0(%rdi), %ymm8
vmovdqa		32(%rdi), %ymm9
vmovdqa		64(%rdi), %ymm10
vmovdqa		96(%rdi), %ymm11
vmovdqa		128(%rdi), %ymm12
vpxor		160(%rdi), %ymm8, %ymm8
vpxor		192(%rdi), %ymm9, %ymm9
vpxor		224(%rdi), %ymm10, %ymm10
vpxor		256(%rdi), %ymm11, %ymm11
vpxor		288(%rdi), %ymm12, %ymm12
vpxor		320(%rdi), %ymm8, %ymm8
vpxor		352(%rdi), %ymm9, %ymm9
vpxor		384(%rdi), %ymm10, %ymm10
vpxor		416(%rdi), %ymm11, %ymm11
vpxor		448(%rdi), %ymm12, %ymm12
vpxor		480(%rdi), %ymm8, %ymm8
vpxor		512(%rdi), %ymm9, %ymm9
vpxor		544(%rdi), %ymm10, %ymm10
vpxor		576(%rdi), %ymm11, %ymm11
vpxor		608(%rdi), %ymm12, %ymm12
vpxor		640(%rdi), %ymm8, %ymm8
vpxor		672(%rdi), %ymm9, %ymm9
vpxor		704(%rdi), %ymm10, %ymm10
vpxor		736(%rdi), %ymm11, %ymm11
vpxor		768(%rdi), %ymm12, %ymm12
vpsllq		$1, %ymm9, %ymm13
vpsllq		$1, %ymm10, %ymm14
vpsllq		$1, %ymm11, %ymm15
vpsllq		$1, %ymm12, %ymm7
vpsllq		$1, %ymm8, %ymm6
vpsrlq		$63, %ymm9, %ymm5
vpsrlq		$63, %ymm10, %ymm4
vpsrlq		$63, %ymm11, %ymm3
vpsrlq		$63, %ymm12, %ymm2
vpsrlq		$63, %ymm8, %ymm1
vpor		%ymm13, %ymm5, %ymm5
vpor		%ymm14, %ymm4, %ymm4
vpor		%ymm15, %ymm3, %ymm3
vpor		%ymm7, %ymm2, %ymm2
vpor		%ymm6, %ymm1, %ymm1
vpxor		%ymm5, %ymm12, %ymm5
vpxor		%ymm4, %ymm8, %ymm4
vpxor		%ymm3, %ymm9, %ymm3
vpxor		%ymm2, %ymm10, %ymm2
vpxor		%ymm1, %ymm11, %ymm1
vpxor		0(%rdi), %ymm5, %ymm8
vpxor		352(%rdi), %ymm4, %ymm9
vpxor		704(%rdi), %ymm3, %ymm10
vpxor		256(%rdi), %ymm2, %ymm11
vpxor		608(%rdi), %ymm1, %ymm12
vpsllq		$44, %ymm9, %ymm14
vpsllq		$43, %ymm10, %ymm15
vpsllq		$21, %ymm11, %ymm7
vpsllq		$14, %ymm12, %ymm6
vpsrlq		$20, %ymm9, %ymm9
vpsrlq		$21, %ymm10, %ymm10
vpsrlq		$43, %ymm11, %ymm11
vpsrlq		$50, %ymm12, %ymm12
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
vpor		%ymm7, %ymm11, %ymm11
vpor		%ymm6, %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vpbroadcastq	16(%rsi), %ymm8
vpxor		%ymm8, %ymm13, %ymm13
vmovdqa		%ymm13, 0(%rdi)
vmovdqa		%ymm14, 352(%rdi)
vmovdqa		%ymm15, 704(%rdi)
vmovdqa		%ymm7, 256(%rdi)
vmovdqa		%ymm6, 608(%rdi)
vpxor		736(%rdi), %ymm2, %ymm8
vpxor		288(%rdi), %ymm1, %ymm9
vpxor		480(%rdi), %ymm5, %ymm10
vpxor		32(%rdi), %ymm4, %ymm11
vpxor		384(%rdi), %ymm3, %ymm12
vpsllq		$28, %ymm8, %ymm13
vpsllq		$20, %ymm9, %ymm14
vpsllq		$3, %ymm10, %ymm15
vpsllq		$45, %ymm11, %ymm7
vpsllq		$61, %ymm12, %ymm6
vpsrlq		$36, %ymm8, %ymm8
vpsrlq		$44, %ymm9, %ymm9
vpsrlq		$61, %ymm10, %ymm10
vpsrlq		$19, %ymm11, %ymm11
vpsrlq		$3, %ymm12, %ymm12
vpor		%ymm13, %ymm8, %ymm8
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
vpor		%ymm7, %ymm11, %ymm11
vpor		%ymm6, %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vmovdqa		%ymm13, 480(%rdi)
vmovdqa		%ymm14, 32(%rdi)
vmovdqa		%ymm15, 384(%rdi)
vmovdqa		%ymm7, 736(%rdi)
vmovdqa		%ymm6, 288(%rdi)
vpxor		512(%rdi), %ymm4, %ymm8
vpxor		64(%rdi), %ymm3, %ymm9
vpxor		416(%rdi), %ymm2, %ymm10
vpxor		768(%rdi), %ymm1, %ymm11
vpxor		160(%rdi), %ymm5, %ymm12
vpsllq		$1, %ymm8, %ymm13
vpsllq		$6, %ymm9, %ymm14
vpsllq		$25, %ymm10, %ymm15
#vpsllq		$8, %ymm11, %ymm7
vpsllq		$18, %ymm12, %ymm6
vpsrlq		$63, %ymm8, %ymm8
vpsrlq		$58, %ymm9, %ymm9
vpsrlq		$39, %ymm10, %ymm10
#vpsrlq		$56, %ymm11, %ymm11
vpsrlq		$46, %ymm12, %ymm12
vpor		%ymm13, %ymm8, %ymm8
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
#vpor		%ymm7, %ymm11, %ymm11
vpshufb		%ymm0, %ymm11, %ymm11
vpor		%ymm6, %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vmovdqa		%ymm13, 160(%rdi)
vmovdqa		%ymm14, 512(%rdi)
vmovdqa		%ymm15, 64(%rdi)
vmovdqa		%ymm7, 416(%rdi)
vmovdqa		%ymm6, 768(%rdi)
vpxor		448(%rdi), %ymm1, %ymm8
vpxor		640(%rdi), %ymm5, %ymm9
vpxor		192(%rdi), %ymm4, %ymm10
vpxor		544(%rdi), %ymm3, %ymm11
vpxor		96(%rdi), %ymm2, %ymm12
vpsllq		$27, %ymm8, %ymm13
vpsllq		$36, %ymm9, %ymm14
vpsllq		$10, %ymm10, %ymm15
vpsllq		$15, %ymm11, %ymm7
#vpsllq		$56, %ymm12, %ymm6
vpsrlq		$37, %ymm8, %ymm8
vpsrlq		$28, %ymm9, %ymm9
vpsrlq		$54, %ymm10, %ymm10
vpsrlq		$49, %ymm11, %ymm11
#vpsrlq		$8, %ymm12, %ymm12
vpor		%ymm13, %ymm8, %ymm8
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
vpor		%ymm7, %ymm11, %ymm11
#vpor		%ymm6, %ymm12, %ymm12
vpshufb		rho56(%rip), %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vmovdqa		%ymm13, 640(%rdi)
vmovdqa		%ymm14, 192(%rdi)
vmovdqa		%ymm15, 544(%rdi)
vmovdqa		%ymm7, 96(%rdi)
vmovdqa		%ymm6, 448(%rdi)
vpxor		224(%rdi), %ymm3, %ymm8
vpxor		576(%rdi), %ymm2, %ymm9
vpxor		128(%rdi), %ymm1, %ymm10
vpxor		320(%rdi), %ymm5, %ymm11
vpxor		672(%rdi), %ymm4, %ymm12
vpsllq		$62, %ymm8, %ymm13
vpsllq		$55, %ymm9, %ymm14
vpsllq		$39, %ymm10, %ymm15
vpsllq		$41, %ymm11, %ymm7
vpsllq		$2, %ymm12, %ymm6
vpsrlq		$2, %ymm8, %ymm8
vpsrlq		$9, %ymm9, %ymm9
vpsrlq		$25, %ymm10, %ymm10
vpsrlq		$23, %ymm11, %ymm11
vpsrlq		$62, %ymm12, %ymm12
vpor		%ymm13, %ymm8, %ymm8
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
vpor		%ymm7, %ymm11, %ymm11
vpor		%ymm6, %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vmovdqa		%ymm13, 320(%rdi)
vmovdqa		%ymm14, 672(%rdi)
vmovdqa		%ymm15, 224(%rdi)
vmovdqa		%ymm7, 576(%rdi)
vmovdqa		%ymm6, 128(%rdi)
vmovdqa		0(%rdi), %ymm8
vmovdqa		32(%rdi), %ymm9
vmovdqa		64(%rdi), %ymm10
vmovdqa		96(%rdi), %ymm11
vmovdqa		128(%rdi), %ymm12
vpxor		160(%rdi), %ymm8, %ymm8
vpxor		192(%rdi), %ymm9, %ymm9
vpxor		224(%rdi), %ymm10, %ymm10
vpxor		256(%rdi), %ymm11, %ymm11
vpxor		288(%rdi), %ymm12, %ymm12
vpxor		320(%rdi), %ymm8, %ymm8
vpxor		352(%rdi), %ymm9, %ymm9
vpxor		384(%rdi), %ymm10, %ymm10
vpxor		416(%rdi), %ymm11, %ymm11
vpxor		448(%rdi), %ymm12, %ymm12
vpxor		480(%rdi), %ymm8, %ymm8
vpxor		512(%rdi), %ymm9, %ymm9
vpxor		544(%rdi), %ymm10, %ymm10
vpxor		576(%rdi), %ymm11, %ymm11
vpxor		608(%rdi), %ymm12, %ymm12
vpxor		640(%rdi), %ymm8, %ymm8
vpxor		672(%rdi), %ymm9, %ymm9
vpxor		704(%rdi), %ymm10, %ymm10
vpxor		736(%rdi), %ymm11, %ymm11
vpxor		768(%rdi), %ymm12, %ymm12
vpsllq		$1, %ymm9, %ymm13
vpsllq		$1, %ymm10, %ymm14
vpsllq		$1, %ymm11, %ymm15
vpsllq		$1, %ymm12, %ymm7
vpsllq		$1, %ymm8, %ymm6
vpsrlq		$63, %ymm9, %ymm5
vpsrlq		$63, %ymm10, %ymm4
vpsrlq		$63, %ymm11, %ymm3
vpsrlq		$63, %ymm12, %ymm2
vpsrlq		$63, %ymm8, %ymm1
vpor		%ymm13, %ymm5, %ymm5
vpor		%ymm14, %ymm4, %ymm4
vpor		%ymm15, %ymm3, %ymm3
vpor		%ymm7, %ymm2, %ymm2
vpor		%ymm6, %ymm1, %ymm1
vpxor		%ymm5, %ymm12, %ymm5
vpxor		%ymm4, %ymm8, %ymm4
vpxor		%ymm3, %ymm9, %ymm3
vpxor		%ymm2, %ymm10, %ymm2
vpxor		%ymm1, %ymm11, %ymm1
vpxor		0(%rdi), %ymm5, %ymm8
vpxor		32(%rdi), %ymm4, %ymm9
vpxor		64(%rdi), %ymm3, %ymm10
vpxor		96(%rdi), %ymm2, %ymm11
vpxor		128(%rdi), %ymm1, %ymm12
vpsllq		$44, %ymm9, %ymm14
vpsllq		$43, %ymm10, %ymm15
vpsllq		$21, %ymm11, %ymm7
vpsllq		$14, %ymm12, %ymm6
vpsrlq		$20, %ymm9, %ymm9
vpsrlq		$21, %ymm10, %ymm10
vpsrlq		$43, %ymm11, %ymm11
vpsrlq		$50, %ymm12, %ymm12
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
vpor		%ymm7, %ymm11, %ymm11
vpor		%ymm6, %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vpbroadcastq	24(%rsi), %ymm8
vpxor		%ymm8, %ymm13, %ymm13
vmovdqa		%ymm13, 0(%rdi)
vmovdqa		%ymm14, 32(%rdi)
vmovdqa		%ymm15, 64(%rdi)
vmovdqa		%ymm7, 96(%rdi)
vmovdqa		%ymm6, 128(%rdi)
vpxor		256(%rdi), %ymm2, %ymm8
vpxor		288(%rdi), %ymm1, %ymm9
vpxor		160(%rdi), %ymm5, %ymm10
vpxor		192(%rdi), %ymm4, %ymm11
vpxor		224(%rdi), %ymm3, %ymm12
vpsllq		$28, %ymm8, %ymm13
vpsllq		$20, %ymm9, %ymm14
vpsllq		$3, %ymm10, %ymm15
vpsllq		$45, %ymm11, %ymm7
vpsllq		$61, %ymm12, %ymm6
vpsrlq		$36, %ymm8, %ymm8
vpsrlq		$44, %ymm9, %ymm9
vpsrlq		$61, %ymm10, %ymm10
vpsrlq		$19, %ymm11, %ymm11
vpsrlq		$3, %ymm12, %ymm12
vpor		%ymm13, %ymm8, %ymm8
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
vpor		%ymm7, %ymm11, %ymm11
vpor		%ymm6, %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vmovdqa		%ymm13, 160(%rdi)
vmovdqa		%ymm14, 192(%rdi)
vmovdqa		%ymm15, 224(%rdi)
vmovdqa		%ymm7, 256(%rdi)
vmovdqa		%ymm6, 288(%rdi)
vpxor		352(%rdi), %ymm4, %ymm8
vpxor		384(%rdi), %ymm3, %ymm9
vpxor		416(%rdi), %ymm2, %ymm10
vpxor		448(%rdi), %ymm1, %ymm11
vpxor		320(%rdi), %ymm5, %ymm12
vpsllq		$1, %ymm8, %ymm13
vpsllq		$6, %ymm9, %ymm14
vpsllq		$25, %ymm10, %ymm15
#vpsllq		$8, %ymm11, %ymm7
vpsllq		$18, %ymm12, %ymm6
vpsrlq		$63, %ymm8, %ymm8
vpsrlq		$58, %ymm9, %ymm9
vpsrlq		$39, %ymm10, %ymm10
#vpsrlq		$56, %ymm11, %ymm11
vpsrlq		$46, %ymm12, %ymm12
vpor		%ymm13, %ymm8, %ymm8
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
#vpor		%ymm7, %ymm11, %ymm11
vpshufb		%ymm0, %ymm11, %ymm11
vpor		%ymm6, %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vmovdqa		%ymm13, 320(%rdi)
vmovdqa		%ymm14, 352(%rdi)
vmovdqa		%ymm15, 384(%rdi)
vmovdqa		%ymm7, 416(%rdi)
vmovdqa		%ymm6, 448(%rdi)
vpxor		608(%rdi), %ymm1, %ymm8
vpxor		480(%rdi), %ymm5, %ymm9
vpxor		512(%rdi), %ymm4, %ymm10
vpxor		544(%rdi), %ymm3, %ymm11
vpxor		576(%rdi), %ymm2, %ymm12
vpsllq		$27, %ymm8, %ymm13
vpsllq		$36, %ymm9, %ymm14
vpsllq		$10, %ymm10, %ymm15
vpsllq		$15, %ymm11, %ymm7
#vpsllq		$56, %ymm12, %ymm6
vpsrlq		$37, %ymm8, %ymm8
vpsrlq		$28, %ymm9, %ymm9
vpsrlq		$54, %ymm10, %ymm10
vpsrlq		$49, %ymm11, %ymm11
#vpsrlq		$8, %ymm12, %ymm12
vpor		%ymm13, %ymm8, %ymm8
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
vpor		%ymm7, %ymm11, %ymm11
#vpor		%ymm6, %ymm12, %ymm12
vpshufb		rho56(%rip), %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vmovdqa		%ymm13, 480(%rdi)
vmovdqa		%ymm14, 512(%rdi)
vmovdqa		%ymm15, 544(%rdi)
vmovdqa		%ymm7, 576(%rdi)
vmovdqa		%ymm6, 608(%rdi)
vpxor		704(%rdi), %ymm3, %ymm8
vpxor		736(%rdi), %ymm2, %ymm9
vpxor		768(%rdi), %ymm1, %ymm10
vpxor		640(%rdi), %ymm5, %ymm11
vpxor		672(%rdi), %ymm4, %ymm12
vpsllq		$62, %ymm8, %ymm13
vpsllq		$55, %ymm9, %ymm14
vpsllq		$39, %ymm10, %ymm15
vpsllq		$41, %ymm11, %ymm7
vpsllq		$2, %ymm12, %ymm6
vpsrlq		$2, %ymm8, %ymm8
vpsrlq		$9, %ymm9, %ymm9
vpsrlq		$25, %ymm10, %ymm10
vpsrlq		$23, %ymm11, %ymm11
vpsrlq		$62, %ymm12, %ymm12
vpor		%ymm13, %ymm8, %ymm8
vpor		%ymm14, %ymm9, %ymm9
vpor		%ymm15, %ymm10, %ymm10
vpor		%ymm7, %ymm11, %ymm11
vpor		%ymm6, %ymm12, %ymm12
vpandn		%ymm10, %ymm9, %ymm13
vpandn		%ymm11, %ymm10, %ymm14
vpandn		%ymm12, %ymm11, %ymm15
vpandn		%ymm8, %ymm12, %ymm7
vpandn		%ymm9, %ymm8, %ymm6
vpxor		%ymm8, %ymm13, %ymm13
vpxor		%ymm9, %ymm14, %ymm14
vpxor		%ymm10, %ymm15, %ymm15
vpxor		%ymm11, %ymm7, %ymm7
vpxor		%ymm12, %ymm6, %ymm6
vmovdqa		%ymm13, 640(%rdi)
vmovdqa		%ymm14, 672(%rdi)
vmovdqa		%ymm15, 704(%rdi)
vmovdqa		%ymm7, 736(%rdi)
vmovdqa		%ymm6, 768(%rdi)
addq		$32, %rsi
subq		$1, %rax
jnz		looptop
ret
