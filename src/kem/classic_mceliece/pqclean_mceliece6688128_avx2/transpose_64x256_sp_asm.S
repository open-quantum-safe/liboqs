#include "namespace.h"

#define MASK0_0 CRYPTO_NAMESPACE(MASK0_0)
#define _MASK0_0 _CRYPTO_NAMESPACE(MASK0_0)
#define MASK0_1 CRYPTO_NAMESPACE(MASK0_1)
#define _MASK0_1 _CRYPTO_NAMESPACE(MASK0_1)
#define MASK1_0 CRYPTO_NAMESPACE(MASK1_0)
#define _MASK1_0 _CRYPTO_NAMESPACE(MASK1_0)
#define MASK1_1 CRYPTO_NAMESPACE(MASK1_1)
#define _MASK1_1 _CRYPTO_NAMESPACE(MASK1_1)
#define MASK2_0 CRYPTO_NAMESPACE(MASK2_0)
#define _MASK2_0 _CRYPTO_NAMESPACE(MASK2_0)
#define MASK2_1 CRYPTO_NAMESPACE(MASK2_1)
#define _MASK2_1 _CRYPTO_NAMESPACE(MASK2_1)
#define MASK3_0 CRYPTO_NAMESPACE(MASK3_0)
#define _MASK3_0 _CRYPTO_NAMESPACE(MASK3_0)
#define MASK3_1 CRYPTO_NAMESPACE(MASK3_1)
#define _MASK3_1 _CRYPTO_NAMESPACE(MASK3_1)
#define MASK4_0 CRYPTO_NAMESPACE(MASK4_0)
#define _MASK4_0 _CRYPTO_NAMESPACE(MASK4_0)
#define MASK4_1 CRYPTO_NAMESPACE(MASK4_1)
#define _MASK4_1 _CRYPTO_NAMESPACE(MASK4_1)
#define MASK5_0 CRYPTO_NAMESPACE(MASK5_0)
#define _MASK5_0 _CRYPTO_NAMESPACE(MASK5_0)
#define MASK5_1 CRYPTO_NAMESPACE(MASK5_1)
#define _MASK5_1 _CRYPTO_NAMESPACE(MASK5_1)
#define transpose_64x256_sp_asm CRYPTO_NAMESPACE(transpose_64x256_sp_asm)
#define _transpose_64x256_sp_asm _CRYPTO_NAMESPACE(transpose_64x256_sp_asm)

# qhasm: int64 input_0

# qhasm: int64 input_1

# qhasm: int64 input_2

# qhasm: int64 input_3

# qhasm: int64 input_4

# qhasm: int64 input_5

# qhasm: stack64 input_6

# qhasm: stack64 input_7

# qhasm: int64 caller_r11

# qhasm: int64 caller_r12

# qhasm: int64 caller_r13

# qhasm: int64 caller_r14

# qhasm: int64 caller_r15

# qhasm: int64 caller_rbx

# qhasm: int64 caller_rbp

# qhasm: reg256 x0

# qhasm: reg256 x1

# qhasm: reg256 x2

# qhasm: reg256 x3

# qhasm: reg256 x4

# qhasm: reg256 x5

# qhasm: reg256 x6

# qhasm: reg256 x7

# qhasm: reg256 t0

# qhasm: reg256 t1

# qhasm: reg256 v00

# qhasm: reg256 v01

# qhasm: reg256 v10

# qhasm: reg256 v11

# qhasm: reg256 mask0

# qhasm: reg256 mask1

# qhasm: reg256 mask2

# qhasm: reg256 mask3

# qhasm: reg256 mask4

# qhasm: reg256 mask5

# qhasm: enter transpose_64x256_sp_asm
.p2align 5
.global _transpose_64x256_sp_asm
.global transpose_64x256_sp_asm
_transpose_64x256_sp_asm:
transpose_64x256_sp_asm:
mov % rsp, % r11
and $31, % r11
add $0, % r11
sub % r11, % rsp

# qhasm: mask0 aligned= mem256[ MASK5_0 ]
# asm 1: vmovapd MASK5_0(%rip),>mask0=reg256#1
# asm 2: vmovapd MASK5_0(%rip),>mask0=%ymm0
vmovapd MASK5_0( % rip), % ymm0

# qhasm: mask1 aligned= mem256[ MASK5_1 ]
# asm 1: vmovapd MASK5_1(%rip),>mask1=reg256#2
# asm 2: vmovapd MASK5_1(%rip),>mask1=%ymm1
vmovapd MASK5_1( % rip), % ymm1

# qhasm: mask2 aligned= mem256[ MASK4_0 ]
# asm 1: vmovapd MASK4_0(%rip),>mask2=reg256#3
# asm 2: vmovapd MASK4_0(%rip),>mask2=%ymm2
vmovapd MASK4_0( % rip), % ymm2

# qhasm: mask3 aligned= mem256[ MASK4_1 ]
# asm 1: vmovapd MASK4_1(%rip),>mask3=reg256#4
# asm 2: vmovapd MASK4_1(%rip),>mask3=%ymm3
vmovapd MASK4_1( % rip), % ymm3

# qhasm: mask4 aligned= mem256[ MASK3_0 ]
# asm 1: vmovapd MASK3_0(%rip),>mask4=reg256#5
# asm 2: vmovapd MASK3_0(%rip),>mask4=%ymm4
vmovapd MASK3_0( % rip), % ymm4

# qhasm: mask5 aligned= mem256[ MASK3_1 ]
# asm 1: vmovapd MASK3_1(%rip),>mask5=reg256#6
# asm 2: vmovapd MASK3_1(%rip),>mask5=%ymm5
vmovapd MASK3_1( % rip), % ymm5

# qhasm: x0 = mem256[ input_0 + 0 ]
# asm 1: vmovupd   0(<input_0=int64#1),>x0=reg256#7
# asm 2: vmovupd   0(<input_0=%rdi),>x0=%ymm6
vmovupd   0( % rdi), % ymm6

# qhasm: x1 = mem256[ input_0 + 256 ]
# asm 1: vmovupd   256(<input_0=int64#1),>x1=reg256#8
# asm 2: vmovupd   256(<input_0=%rdi),>x1=%ymm7
vmovupd   256( % rdi), % ymm7

# qhasm: x2 = mem256[ input_0 + 512 ]
# asm 1: vmovupd   512(<input_0=int64#1),>x2=reg256#9
# asm 2: vmovupd   512(<input_0=%rdi),>x2=%ymm8
vmovupd   512( % rdi), % ymm8

# qhasm: x3 = mem256[ input_0 + 768 ]
# asm 1: vmovupd   768(<input_0=int64#1),>x3=reg256#10
# asm 2: vmovupd   768(<input_0=%rdi),>x3=%ymm9
vmovupd   768( % rdi), % ymm9

# qhasm: x4 = mem256[ input_0 + 1024 ]
# asm 1: vmovupd   1024(<input_0=int64#1),>x4=reg256#11
# asm 2: vmovupd   1024(<input_0=%rdi),>x4=%ymm10
vmovupd   1024( % rdi), % ymm10

# qhasm: x5 = mem256[ input_0 + 1280 ]
# asm 1: vmovupd   1280(<input_0=int64#1),>x5=reg256#12
# asm 2: vmovupd   1280(<input_0=%rdi),>x5=%ymm11
vmovupd   1280( % rdi), % ymm11

# qhasm: x6 = mem256[ input_0 + 1536 ]
# asm 1: vmovupd   1536(<input_0=int64#1),>x6=reg256#13
# asm 2: vmovupd   1536(<input_0=%rdi),>x6=%ymm12
vmovupd   1536( % rdi), % ymm12

# qhasm: x7 = mem256[ input_0 + 1792 ]
# asm 1: vmovupd   1792(<input_0=int64#1),>x7=reg256#14
# asm 2: vmovupd   1792(<input_0=%rdi),>x7=%ymm13
vmovupd   1792( % rdi), % ymm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <x0=reg256#7,<mask0=reg256#1,>v00=reg256#15
# asm 2: vpand <x0=%ymm6,<mask0=%ymm0,>v00=%ymm14
vpand % ymm6, % ymm0, % ymm14

# qhasm: 4x v10 = x4 << 32
# asm 1: vpsllq $32,<x4=reg256#11,>v10=reg256#16
# asm 2: vpsllq $32,<x4=%ymm10,>v10=%ymm15
vpsllq $32, % ymm10, % ymm15

# qhasm: 4x v01 = x0 unsigned>> 32
# asm 1: vpsrlq $32,<x0=reg256#7,>v01=reg256#7
# asm 2: vpsrlq $32,<x0=%ymm6,>v01=%ymm6
vpsrlq $32, % ymm6, % ymm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <x4=reg256#11,<mask1=reg256#2,>v11=reg256#11
# asm 2: vpand <x4=%ymm10,<mask1=%ymm1,>v11=%ymm10
vpand % ymm10, % ymm1, % ymm10

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x0=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x0=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x4 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#11,>x4=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm10,>x4=%ymm6
vpor  % ymm6, % ymm10, % ymm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <x1=reg256#8,<mask0=reg256#1,>v00=reg256#11
# asm 2: vpand <x1=%ymm7,<mask0=%ymm0,>v00=%ymm10
vpand % ymm7, % ymm0, % ymm10

# qhasm: 4x v10 = x5 << 32
# asm 1: vpsllq $32,<x5=reg256#12,>v10=reg256#16
# asm 2: vpsllq $32,<x5=%ymm11,>v10=%ymm15
vpsllq $32, % ymm11, % ymm15

# qhasm: 4x v01 = x1 unsigned>> 32
# asm 1: vpsrlq $32,<x1=reg256#8,>v01=reg256#8
# asm 2: vpsrlq $32,<x1=%ymm7,>v01=%ymm7
vpsrlq $32, % ymm7, % ymm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <x5=reg256#12,<mask1=reg256#2,>v11=reg256#12
# asm 2: vpand <x5=%ymm11,<mask1=%ymm1,>v11=%ymm11
vpand % ymm11, % ymm1, % ymm11

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#16,>x1=reg256#11
# asm 2: vpor  <v00=%ymm10,<v10=%ymm15,>x1=%ymm10
vpor  % ymm10, % ymm15, % ymm10

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#12,>x5=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm11,>x5=%ymm7
vpor  % ymm7, % ymm11, % ymm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <x2=reg256#9,<mask0=reg256#1,>v00=reg256#12
# asm 2: vpand <x2=%ymm8,<mask0=%ymm0,>v00=%ymm11
vpand % ymm8, % ymm0, % ymm11

# qhasm: 4x v10 = x6 << 32
# asm 1: vpsllq $32,<x6=reg256#13,>v10=reg256#16
# asm 2: vpsllq $32,<x6=%ymm12,>v10=%ymm15
vpsllq $32, % ymm12, % ymm15

# qhasm: 4x v01 = x2 unsigned>> 32
# asm 1: vpsrlq $32,<x2=reg256#9,>v01=reg256#9
# asm 2: vpsrlq $32,<x2=%ymm8,>v01=%ymm8
vpsrlq $32, % ymm8, % ymm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <x6=reg256#13,<mask1=reg256#2,>v11=reg256#13
# asm 2: vpand <x6=%ymm12,<mask1=%ymm1,>v11=%ymm12
vpand % ymm12, % ymm1, % ymm12

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x2=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x2=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#9,<v11=reg256#13,>x6=reg256#9
# asm 2: vpor  <v01=%ymm8,<v11=%ymm12,>x6=%ymm8
vpor  % ymm8, % ymm12, % ymm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <x3=reg256#10,<mask0=reg256#1,>v00=reg256#13
# asm 2: vpand <x3=%ymm9,<mask0=%ymm0,>v00=%ymm12
vpand % ymm9, % ymm0, % ymm12

# qhasm: 4x v10 = x7 << 32
# asm 1: vpsllq $32,<x7=reg256#14,>v10=reg256#16
# asm 2: vpsllq $32,<x7=%ymm13,>v10=%ymm15
vpsllq $32, % ymm13, % ymm15

# qhasm: 4x v01 = x3 unsigned>> 32
# asm 1: vpsrlq $32,<x3=reg256#10,>v01=reg256#10
# asm 2: vpsrlq $32,<x3=%ymm9,>v01=%ymm9
vpsrlq $32, % ymm9, % ymm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <x7=reg256#14,<mask1=reg256#2,>v11=reg256#14
# asm 2: vpand <x7=%ymm13,<mask1=%ymm1,>v11=%ymm13
vpand % ymm13, % ymm1, % ymm13

# qhasm: x3 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x3=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x3=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#10,<v11=reg256#14,>x7=reg256#10
# asm 2: vpor  <v01=%ymm9,<v11=%ymm13,>x7=%ymm9
vpor  % ymm9, % ymm13, % ymm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <x0=reg256#15,<mask2=reg256#3,>v00=reg256#14
# asm 2: vpand <x0=%ymm14,<mask2=%ymm2,>v00=%ymm13
vpand % ymm14, % ymm2, % ymm13

# qhasm: 8x v10 = x2 << 16
# asm 1: vpslld $16,<x2=reg256#12,>v10=reg256#16
# asm 2: vpslld $16,<x2=%ymm11,>v10=%ymm15
vpslld $16, % ymm11, % ymm15

# qhasm: 8x v01 = x0 unsigned>> 16
# asm 1: vpsrld $16,<x0=reg256#15,>v01=reg256#15
# asm 2: vpsrld $16,<x0=%ymm14,>v01=%ymm14
vpsrld $16, % ymm14, % ymm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <x2=reg256#12,<mask3=reg256#4,>v11=reg256#12
# asm 2: vpand <x2=%ymm11,<mask3=%ymm3,>v11=%ymm11
vpand % ymm11, % ymm3, % ymm11

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#14,<v10=reg256#16,>x0=reg256#14
# asm 2: vpor  <v00=%ymm13,<v10=%ymm15,>x0=%ymm13
vpor  % ymm13, % ymm15, % ymm13

# qhasm: x2 = v01 | v11
# asm 1: vpor  <v01=reg256#15,<v11=reg256#12,>x2=reg256#12
# asm 2: vpor  <v01=%ymm14,<v11=%ymm11,>x2=%ymm11
vpor  % ymm14, % ymm11, % ymm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <x1=reg256#11,<mask2=reg256#3,>v00=reg256#15
# asm 2: vpand <x1=%ymm10,<mask2=%ymm2,>v00=%ymm14
vpand % ymm10, % ymm2, % ymm14

# qhasm: 8x v10 = x3 << 16
# asm 1: vpslld $16,<x3=reg256#13,>v10=reg256#16
# asm 2: vpslld $16,<x3=%ymm12,>v10=%ymm15
vpslld $16, % ymm12, % ymm15

# qhasm: 8x v01 = x1 unsigned>> 16
# asm 1: vpsrld $16,<x1=reg256#11,>v01=reg256#11
# asm 2: vpsrld $16,<x1=%ymm10,>v01=%ymm10
vpsrld $16, % ymm10, % ymm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <x3=reg256#13,<mask3=reg256#4,>v11=reg256#13
# asm 2: vpand <x3=%ymm12,<mask3=%ymm3,>v11=%ymm12
vpand % ymm12, % ymm3, % ymm12

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x1=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x1=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#11,<v11=reg256#13,>x3=reg256#11
# asm 2: vpor  <v01=%ymm10,<v11=%ymm12,>x3=%ymm10
vpor  % ymm10, % ymm12, % ymm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <x4=reg256#7,<mask2=reg256#3,>v00=reg256#13
# asm 2: vpand <x4=%ymm6,<mask2=%ymm2,>v00=%ymm12
vpand % ymm6, % ymm2, % ymm12

# qhasm: 8x v10 = x6 << 16
# asm 1: vpslld $16,<x6=reg256#9,>v10=reg256#16
# asm 2: vpslld $16,<x6=%ymm8,>v10=%ymm15
vpslld $16, % ymm8, % ymm15

# qhasm: 8x v01 = x4 unsigned>> 16
# asm 1: vpsrld $16,<x4=reg256#7,>v01=reg256#7
# asm 2: vpsrld $16,<x4=%ymm6,>v01=%ymm6
vpsrld $16, % ymm6, % ymm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <x6=reg256#9,<mask3=reg256#4,>v11=reg256#9
# asm 2: vpand <x6=%ymm8,<mask3=%ymm3,>v11=%ymm8
vpand % ymm8, % ymm3, % ymm8

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x4=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x4=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#9,>x6=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm8,>x6=%ymm6
vpor  % ymm6, % ymm8, % ymm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <x5=reg256#8,<mask2=reg256#3,>v00=reg256#9
# asm 2: vpand <x5=%ymm7,<mask2=%ymm2,>v00=%ymm8
vpand % ymm7, % ymm2, % ymm8

# qhasm: 8x v10 = x7 << 16
# asm 1: vpslld $16,<x7=reg256#10,>v10=reg256#16
# asm 2: vpslld $16,<x7=%ymm9,>v10=%ymm15
vpslld $16, % ymm9, % ymm15

# qhasm: 8x v01 = x5 unsigned>> 16
# asm 1: vpsrld $16,<x5=reg256#8,>v01=reg256#8
# asm 2: vpsrld $16,<x5=%ymm7,>v01=%ymm7
vpsrld $16, % ymm7, % ymm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <x7=reg256#10,<mask3=reg256#4,>v11=reg256#10
# asm 2: vpand <x7=%ymm9,<mask3=%ymm3,>v11=%ymm9
vpand % ymm9, % ymm3, % ymm9

# qhasm: x5 = v00 | v10
# asm 1: vpor  <v00=reg256#9,<v10=reg256#16,>x5=reg256#9
# asm 2: vpor  <v00=%ymm8,<v10=%ymm15,>x5=%ymm8
vpor  % ymm8, % ymm15, % ymm8

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#10,>x7=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm9,>x7=%ymm7
vpor  % ymm7, % ymm9, % ymm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <x0=reg256#14,<mask4=reg256#5,>v00=reg256#10
# asm 2: vpand <x0=%ymm13,<mask4=%ymm4,>v00=%ymm9
vpand % ymm13, % ymm4, % ymm9

# qhasm: 16x v10 = x1 << 8
# asm 1: vpsllw $8,<x1=reg256#15,>v10=reg256#16
# asm 2: vpsllw $8,<x1=%ymm14,>v10=%ymm15
vpsllw $8, % ymm14, % ymm15

# qhasm: 16x v01 = x0 unsigned>> 8
# asm 1: vpsrlw $8,<x0=reg256#14,>v01=reg256#14
# asm 2: vpsrlw $8,<x0=%ymm13,>v01=%ymm13
vpsrlw $8, % ymm13, % ymm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <x1=reg256#15,<mask5=reg256#6,>v11=reg256#15
# asm 2: vpand <x1=%ymm14,<mask5=%ymm5,>v11=%ymm14
vpand % ymm14, % ymm5, % ymm14

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#10,<v10=reg256#16,>x0=reg256#10
# asm 2: vpor  <v00=%ymm9,<v10=%ymm15,>x0=%ymm9
vpor  % ymm9, % ymm15, % ymm9

# qhasm: x1 = v01 | v11
# asm 1: vpor  <v01=reg256#14,<v11=reg256#15,>x1=reg256#14
# asm 2: vpor  <v01=%ymm13,<v11=%ymm14,>x1=%ymm13
vpor  % ymm13, % ymm14, % ymm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <x2=reg256#12,<mask4=reg256#5,>v00=reg256#15
# asm 2: vpand <x2=%ymm11,<mask4=%ymm4,>v00=%ymm14
vpand % ymm11, % ymm4, % ymm14

# qhasm: 16x v10 = x3 << 8
# asm 1: vpsllw $8,<x3=reg256#11,>v10=reg256#16
# asm 2: vpsllw $8,<x3=%ymm10,>v10=%ymm15
vpsllw $8, % ymm10, % ymm15

# qhasm: 16x v01 = x2 unsigned>> 8
# asm 1: vpsrlw $8,<x2=reg256#12,>v01=reg256#12
# asm 2: vpsrlw $8,<x2=%ymm11,>v01=%ymm11
vpsrlw $8, % ymm11, % ymm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <x3=reg256#11,<mask5=reg256#6,>v11=reg256#11
# asm 2: vpand <x3=%ymm10,<mask5=%ymm5,>v11=%ymm10
vpand % ymm10, % ymm5, % ymm10

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x2=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x2=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#12,<v11=reg256#11,>x3=reg256#11
# asm 2: vpor  <v01=%ymm11,<v11=%ymm10,>x3=%ymm10
vpor  % ymm11, % ymm10, % ymm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <x4=reg256#13,<mask4=reg256#5,>v00=reg256#12
# asm 2: vpand <x4=%ymm12,<mask4=%ymm4,>v00=%ymm11
vpand % ymm12, % ymm4, % ymm11

# qhasm: 16x v10 = x5 << 8
# asm 1: vpsllw $8,<x5=reg256#9,>v10=reg256#16
# asm 2: vpsllw $8,<x5=%ymm8,>v10=%ymm15
vpsllw $8, % ymm8, % ymm15

# qhasm: 16x v01 = x4 unsigned>> 8
# asm 1: vpsrlw $8,<x4=reg256#13,>v01=reg256#13
# asm 2: vpsrlw $8,<x4=%ymm12,>v01=%ymm12
vpsrlw $8, % ymm12, % ymm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <x5=reg256#9,<mask5=reg256#6,>v11=reg256#9
# asm 2: vpand <x5=%ymm8,<mask5=%ymm5,>v11=%ymm8
vpand % ymm8, % ymm5, % ymm8

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x4=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x4=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#13,<v11=reg256#9,>x5=reg256#9
# asm 2: vpor  <v01=%ymm12,<v11=%ymm8,>x5=%ymm8
vpor  % ymm12, % ymm8, % ymm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <x6=reg256#7,<mask4=reg256#5,>v00=reg256#13
# asm 2: vpand <x6=%ymm6,<mask4=%ymm4,>v00=%ymm12
vpand % ymm6, % ymm4, % ymm12

# qhasm: 16x v10 = x7 << 8
# asm 1: vpsllw $8,<x7=reg256#8,>v10=reg256#16
# asm 2: vpsllw $8,<x7=%ymm7,>v10=%ymm15
vpsllw $8, % ymm7, % ymm15

# qhasm: 16x v01 = x6 unsigned>> 8
# asm 1: vpsrlw $8,<x6=reg256#7,>v01=reg256#7
# asm 2: vpsrlw $8,<x6=%ymm6,>v01=%ymm6
vpsrlw $8, % ymm6, % ymm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <x7=reg256#8,<mask5=reg256#6,>v11=reg256#8
# asm 2: vpand <x7=%ymm7,<mask5=%ymm5,>v11=%ymm7
vpand % ymm7, % ymm5, % ymm7

# qhasm: x6 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x6=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x6=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#8,>x7=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm7,>x7=%ymm6
vpor  % ymm6, % ymm7, % ymm6

# qhasm: mem256[ input_0 + 0 ] = x0
# asm 1: vmovupd   <x0=reg256#10,0(<input_0=int64#1)
# asm 2: vmovupd   <x0=%ymm9,0(<input_0=%rdi)
vmovupd   % ymm9, 0( % rdi)

# qhasm: mem256[ input_0 + 256 ] = x1
# asm 1: vmovupd   <x1=reg256#14,256(<input_0=int64#1)
# asm 2: vmovupd   <x1=%ymm13,256(<input_0=%rdi)
vmovupd   % ymm13, 256( % rdi)

# qhasm: mem256[ input_0 + 512 ] = x2
# asm 1: vmovupd   <x2=reg256#15,512(<input_0=int64#1)
# asm 2: vmovupd   <x2=%ymm14,512(<input_0=%rdi)
vmovupd   % ymm14, 512( % rdi)

# qhasm: mem256[ input_0 + 768 ] = x3
# asm 1: vmovupd   <x3=reg256#11,768(<input_0=int64#1)
# asm 2: vmovupd   <x3=%ymm10,768(<input_0=%rdi)
vmovupd   % ymm10, 768( % rdi)

# qhasm: mem256[ input_0 + 1024 ] = x4
# asm 1: vmovupd   <x4=reg256#12,1024(<input_0=int64#1)
# asm 2: vmovupd   <x4=%ymm11,1024(<input_0=%rdi)
vmovupd   % ymm11, 1024( % rdi)

# qhasm: mem256[ input_0 + 1280 ] = x5
# asm 1: vmovupd   <x5=reg256#9,1280(<input_0=int64#1)
# asm 2: vmovupd   <x5=%ymm8,1280(<input_0=%rdi)
vmovupd   % ymm8, 1280( % rdi)

# qhasm: mem256[ input_0 + 1536 ] = x6
# asm 1: vmovupd   <x6=reg256#13,1536(<input_0=int64#1)
# asm 2: vmovupd   <x6=%ymm12,1536(<input_0=%rdi)
vmovupd   % ymm12, 1536( % rdi)

# qhasm: mem256[ input_0 + 1792 ] = x7
# asm 1: vmovupd   <x7=reg256#7,1792(<input_0=int64#1)
# asm 2: vmovupd   <x7=%ymm6,1792(<input_0=%rdi)
vmovupd   % ymm6, 1792( % rdi)

# qhasm: x0 = mem256[ input_0 + 32 ]
# asm 1: vmovupd   32(<input_0=int64#1),>x0=reg256#7
# asm 2: vmovupd   32(<input_0=%rdi),>x0=%ymm6
vmovupd   32( % rdi), % ymm6

# qhasm: x1 = mem256[ input_0 + 288 ]
# asm 1: vmovupd   288(<input_0=int64#1),>x1=reg256#8
# asm 2: vmovupd   288(<input_0=%rdi),>x1=%ymm7
vmovupd   288( % rdi), % ymm7

# qhasm: x2 = mem256[ input_0 + 544 ]
# asm 1: vmovupd   544(<input_0=int64#1),>x2=reg256#9
# asm 2: vmovupd   544(<input_0=%rdi),>x2=%ymm8
vmovupd   544( % rdi), % ymm8

# qhasm: x3 = mem256[ input_0 + 800 ]
# asm 1: vmovupd   800(<input_0=int64#1),>x3=reg256#10
# asm 2: vmovupd   800(<input_0=%rdi),>x3=%ymm9
vmovupd   800( % rdi), % ymm9

# qhasm: x4 = mem256[ input_0 + 1056 ]
# asm 1: vmovupd   1056(<input_0=int64#1),>x4=reg256#11
# asm 2: vmovupd   1056(<input_0=%rdi),>x4=%ymm10
vmovupd   1056( % rdi), % ymm10

# qhasm: x5 = mem256[ input_0 + 1312 ]
# asm 1: vmovupd   1312(<input_0=int64#1),>x5=reg256#12
# asm 2: vmovupd   1312(<input_0=%rdi),>x5=%ymm11
vmovupd   1312( % rdi), % ymm11

# qhasm: x6 = mem256[ input_0 + 1568 ]
# asm 1: vmovupd   1568(<input_0=int64#1),>x6=reg256#13
# asm 2: vmovupd   1568(<input_0=%rdi),>x6=%ymm12
vmovupd   1568( % rdi), % ymm12

# qhasm: x7 = mem256[ input_0 + 1824 ]
# asm 1: vmovupd   1824(<input_0=int64#1),>x7=reg256#14
# asm 2: vmovupd   1824(<input_0=%rdi),>x7=%ymm13
vmovupd   1824( % rdi), % ymm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <x0=reg256#7,<mask0=reg256#1,>v00=reg256#15
# asm 2: vpand <x0=%ymm6,<mask0=%ymm0,>v00=%ymm14
vpand % ymm6, % ymm0, % ymm14

# qhasm: 4x v10 = x4 << 32
# asm 1: vpsllq $32,<x4=reg256#11,>v10=reg256#16
# asm 2: vpsllq $32,<x4=%ymm10,>v10=%ymm15
vpsllq $32, % ymm10, % ymm15

# qhasm: 4x v01 = x0 unsigned>> 32
# asm 1: vpsrlq $32,<x0=reg256#7,>v01=reg256#7
# asm 2: vpsrlq $32,<x0=%ymm6,>v01=%ymm6
vpsrlq $32, % ymm6, % ymm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <x4=reg256#11,<mask1=reg256#2,>v11=reg256#11
# asm 2: vpand <x4=%ymm10,<mask1=%ymm1,>v11=%ymm10
vpand % ymm10, % ymm1, % ymm10

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x0=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x0=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x4 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#11,>x4=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm10,>x4=%ymm6
vpor  % ymm6, % ymm10, % ymm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <x1=reg256#8,<mask0=reg256#1,>v00=reg256#11
# asm 2: vpand <x1=%ymm7,<mask0=%ymm0,>v00=%ymm10
vpand % ymm7, % ymm0, % ymm10

# qhasm: 4x v10 = x5 << 32
# asm 1: vpsllq $32,<x5=reg256#12,>v10=reg256#16
# asm 2: vpsllq $32,<x5=%ymm11,>v10=%ymm15
vpsllq $32, % ymm11, % ymm15

# qhasm: 4x v01 = x1 unsigned>> 32
# asm 1: vpsrlq $32,<x1=reg256#8,>v01=reg256#8
# asm 2: vpsrlq $32,<x1=%ymm7,>v01=%ymm7
vpsrlq $32, % ymm7, % ymm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <x5=reg256#12,<mask1=reg256#2,>v11=reg256#12
# asm 2: vpand <x5=%ymm11,<mask1=%ymm1,>v11=%ymm11
vpand % ymm11, % ymm1, % ymm11

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#16,>x1=reg256#11
# asm 2: vpor  <v00=%ymm10,<v10=%ymm15,>x1=%ymm10
vpor  % ymm10, % ymm15, % ymm10

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#12,>x5=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm11,>x5=%ymm7
vpor  % ymm7, % ymm11, % ymm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <x2=reg256#9,<mask0=reg256#1,>v00=reg256#12
# asm 2: vpand <x2=%ymm8,<mask0=%ymm0,>v00=%ymm11
vpand % ymm8, % ymm0, % ymm11

# qhasm: 4x v10 = x6 << 32
# asm 1: vpsllq $32,<x6=reg256#13,>v10=reg256#16
# asm 2: vpsllq $32,<x6=%ymm12,>v10=%ymm15
vpsllq $32, % ymm12, % ymm15

# qhasm: 4x v01 = x2 unsigned>> 32
# asm 1: vpsrlq $32,<x2=reg256#9,>v01=reg256#9
# asm 2: vpsrlq $32,<x2=%ymm8,>v01=%ymm8
vpsrlq $32, % ymm8, % ymm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <x6=reg256#13,<mask1=reg256#2,>v11=reg256#13
# asm 2: vpand <x6=%ymm12,<mask1=%ymm1,>v11=%ymm12
vpand % ymm12, % ymm1, % ymm12

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x2=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x2=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#9,<v11=reg256#13,>x6=reg256#9
# asm 2: vpor  <v01=%ymm8,<v11=%ymm12,>x6=%ymm8
vpor  % ymm8, % ymm12, % ymm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <x3=reg256#10,<mask0=reg256#1,>v00=reg256#13
# asm 2: vpand <x3=%ymm9,<mask0=%ymm0,>v00=%ymm12
vpand % ymm9, % ymm0, % ymm12

# qhasm: 4x v10 = x7 << 32
# asm 1: vpsllq $32,<x7=reg256#14,>v10=reg256#16
# asm 2: vpsllq $32,<x7=%ymm13,>v10=%ymm15
vpsllq $32, % ymm13, % ymm15

# qhasm: 4x v01 = x3 unsigned>> 32
# asm 1: vpsrlq $32,<x3=reg256#10,>v01=reg256#10
# asm 2: vpsrlq $32,<x3=%ymm9,>v01=%ymm9
vpsrlq $32, % ymm9, % ymm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <x7=reg256#14,<mask1=reg256#2,>v11=reg256#14
# asm 2: vpand <x7=%ymm13,<mask1=%ymm1,>v11=%ymm13
vpand % ymm13, % ymm1, % ymm13

# qhasm: x3 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x3=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x3=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#10,<v11=reg256#14,>x7=reg256#10
# asm 2: vpor  <v01=%ymm9,<v11=%ymm13,>x7=%ymm9
vpor  % ymm9, % ymm13, % ymm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <x0=reg256#15,<mask2=reg256#3,>v00=reg256#14
# asm 2: vpand <x0=%ymm14,<mask2=%ymm2,>v00=%ymm13
vpand % ymm14, % ymm2, % ymm13

# qhasm: 8x v10 = x2 << 16
# asm 1: vpslld $16,<x2=reg256#12,>v10=reg256#16
# asm 2: vpslld $16,<x2=%ymm11,>v10=%ymm15
vpslld $16, % ymm11, % ymm15

# qhasm: 8x v01 = x0 unsigned>> 16
# asm 1: vpsrld $16,<x0=reg256#15,>v01=reg256#15
# asm 2: vpsrld $16,<x0=%ymm14,>v01=%ymm14
vpsrld $16, % ymm14, % ymm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <x2=reg256#12,<mask3=reg256#4,>v11=reg256#12
# asm 2: vpand <x2=%ymm11,<mask3=%ymm3,>v11=%ymm11
vpand % ymm11, % ymm3, % ymm11

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#14,<v10=reg256#16,>x0=reg256#14
# asm 2: vpor  <v00=%ymm13,<v10=%ymm15,>x0=%ymm13
vpor  % ymm13, % ymm15, % ymm13

# qhasm: x2 = v01 | v11
# asm 1: vpor  <v01=reg256#15,<v11=reg256#12,>x2=reg256#12
# asm 2: vpor  <v01=%ymm14,<v11=%ymm11,>x2=%ymm11
vpor  % ymm14, % ymm11, % ymm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <x1=reg256#11,<mask2=reg256#3,>v00=reg256#15
# asm 2: vpand <x1=%ymm10,<mask2=%ymm2,>v00=%ymm14
vpand % ymm10, % ymm2, % ymm14

# qhasm: 8x v10 = x3 << 16
# asm 1: vpslld $16,<x3=reg256#13,>v10=reg256#16
# asm 2: vpslld $16,<x3=%ymm12,>v10=%ymm15
vpslld $16, % ymm12, % ymm15

# qhasm: 8x v01 = x1 unsigned>> 16
# asm 1: vpsrld $16,<x1=reg256#11,>v01=reg256#11
# asm 2: vpsrld $16,<x1=%ymm10,>v01=%ymm10
vpsrld $16, % ymm10, % ymm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <x3=reg256#13,<mask3=reg256#4,>v11=reg256#13
# asm 2: vpand <x3=%ymm12,<mask3=%ymm3,>v11=%ymm12
vpand % ymm12, % ymm3, % ymm12

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x1=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x1=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#11,<v11=reg256#13,>x3=reg256#11
# asm 2: vpor  <v01=%ymm10,<v11=%ymm12,>x3=%ymm10
vpor  % ymm10, % ymm12, % ymm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <x4=reg256#7,<mask2=reg256#3,>v00=reg256#13
# asm 2: vpand <x4=%ymm6,<mask2=%ymm2,>v00=%ymm12
vpand % ymm6, % ymm2, % ymm12

# qhasm: 8x v10 = x6 << 16
# asm 1: vpslld $16,<x6=reg256#9,>v10=reg256#16
# asm 2: vpslld $16,<x6=%ymm8,>v10=%ymm15
vpslld $16, % ymm8, % ymm15

# qhasm: 8x v01 = x4 unsigned>> 16
# asm 1: vpsrld $16,<x4=reg256#7,>v01=reg256#7
# asm 2: vpsrld $16,<x4=%ymm6,>v01=%ymm6
vpsrld $16, % ymm6, % ymm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <x6=reg256#9,<mask3=reg256#4,>v11=reg256#9
# asm 2: vpand <x6=%ymm8,<mask3=%ymm3,>v11=%ymm8
vpand % ymm8, % ymm3, % ymm8

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x4=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x4=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#9,>x6=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm8,>x6=%ymm6
vpor  % ymm6, % ymm8, % ymm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <x5=reg256#8,<mask2=reg256#3,>v00=reg256#9
# asm 2: vpand <x5=%ymm7,<mask2=%ymm2,>v00=%ymm8
vpand % ymm7, % ymm2, % ymm8

# qhasm: 8x v10 = x7 << 16
# asm 1: vpslld $16,<x7=reg256#10,>v10=reg256#16
# asm 2: vpslld $16,<x7=%ymm9,>v10=%ymm15
vpslld $16, % ymm9, % ymm15

# qhasm: 8x v01 = x5 unsigned>> 16
# asm 1: vpsrld $16,<x5=reg256#8,>v01=reg256#8
# asm 2: vpsrld $16,<x5=%ymm7,>v01=%ymm7
vpsrld $16, % ymm7, % ymm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <x7=reg256#10,<mask3=reg256#4,>v11=reg256#10
# asm 2: vpand <x7=%ymm9,<mask3=%ymm3,>v11=%ymm9
vpand % ymm9, % ymm3, % ymm9

# qhasm: x5 = v00 | v10
# asm 1: vpor  <v00=reg256#9,<v10=reg256#16,>x5=reg256#9
# asm 2: vpor  <v00=%ymm8,<v10=%ymm15,>x5=%ymm8
vpor  % ymm8, % ymm15, % ymm8

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#10,>x7=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm9,>x7=%ymm7
vpor  % ymm7, % ymm9, % ymm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <x0=reg256#14,<mask4=reg256#5,>v00=reg256#10
# asm 2: vpand <x0=%ymm13,<mask4=%ymm4,>v00=%ymm9
vpand % ymm13, % ymm4, % ymm9

# qhasm: 16x v10 = x1 << 8
# asm 1: vpsllw $8,<x1=reg256#15,>v10=reg256#16
# asm 2: vpsllw $8,<x1=%ymm14,>v10=%ymm15
vpsllw $8, % ymm14, % ymm15

# qhasm: 16x v01 = x0 unsigned>> 8
# asm 1: vpsrlw $8,<x0=reg256#14,>v01=reg256#14
# asm 2: vpsrlw $8,<x0=%ymm13,>v01=%ymm13
vpsrlw $8, % ymm13, % ymm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <x1=reg256#15,<mask5=reg256#6,>v11=reg256#15
# asm 2: vpand <x1=%ymm14,<mask5=%ymm5,>v11=%ymm14
vpand % ymm14, % ymm5, % ymm14

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#10,<v10=reg256#16,>x0=reg256#10
# asm 2: vpor  <v00=%ymm9,<v10=%ymm15,>x0=%ymm9
vpor  % ymm9, % ymm15, % ymm9

# qhasm: x1 = v01 | v11
# asm 1: vpor  <v01=reg256#14,<v11=reg256#15,>x1=reg256#14
# asm 2: vpor  <v01=%ymm13,<v11=%ymm14,>x1=%ymm13
vpor  % ymm13, % ymm14, % ymm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <x2=reg256#12,<mask4=reg256#5,>v00=reg256#15
# asm 2: vpand <x2=%ymm11,<mask4=%ymm4,>v00=%ymm14
vpand % ymm11, % ymm4, % ymm14

# qhasm: 16x v10 = x3 << 8
# asm 1: vpsllw $8,<x3=reg256#11,>v10=reg256#16
# asm 2: vpsllw $8,<x3=%ymm10,>v10=%ymm15
vpsllw $8, % ymm10, % ymm15

# qhasm: 16x v01 = x2 unsigned>> 8
# asm 1: vpsrlw $8,<x2=reg256#12,>v01=reg256#12
# asm 2: vpsrlw $8,<x2=%ymm11,>v01=%ymm11
vpsrlw $8, % ymm11, % ymm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <x3=reg256#11,<mask5=reg256#6,>v11=reg256#11
# asm 2: vpand <x3=%ymm10,<mask5=%ymm5,>v11=%ymm10
vpand % ymm10, % ymm5, % ymm10

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x2=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x2=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#12,<v11=reg256#11,>x3=reg256#11
# asm 2: vpor  <v01=%ymm11,<v11=%ymm10,>x3=%ymm10
vpor  % ymm11, % ymm10, % ymm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <x4=reg256#13,<mask4=reg256#5,>v00=reg256#12
# asm 2: vpand <x4=%ymm12,<mask4=%ymm4,>v00=%ymm11
vpand % ymm12, % ymm4, % ymm11

# qhasm: 16x v10 = x5 << 8
# asm 1: vpsllw $8,<x5=reg256#9,>v10=reg256#16
# asm 2: vpsllw $8,<x5=%ymm8,>v10=%ymm15
vpsllw $8, % ymm8, % ymm15

# qhasm: 16x v01 = x4 unsigned>> 8
# asm 1: vpsrlw $8,<x4=reg256#13,>v01=reg256#13
# asm 2: vpsrlw $8,<x4=%ymm12,>v01=%ymm12
vpsrlw $8, % ymm12, % ymm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <x5=reg256#9,<mask5=reg256#6,>v11=reg256#9
# asm 2: vpand <x5=%ymm8,<mask5=%ymm5,>v11=%ymm8
vpand % ymm8, % ymm5, % ymm8

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x4=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x4=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#13,<v11=reg256#9,>x5=reg256#9
# asm 2: vpor  <v01=%ymm12,<v11=%ymm8,>x5=%ymm8
vpor  % ymm12, % ymm8, % ymm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <x6=reg256#7,<mask4=reg256#5,>v00=reg256#13
# asm 2: vpand <x6=%ymm6,<mask4=%ymm4,>v00=%ymm12
vpand % ymm6, % ymm4, % ymm12

# qhasm: 16x v10 = x7 << 8
# asm 1: vpsllw $8,<x7=reg256#8,>v10=reg256#16
# asm 2: vpsllw $8,<x7=%ymm7,>v10=%ymm15
vpsllw $8, % ymm7, % ymm15

# qhasm: 16x v01 = x6 unsigned>> 8
# asm 1: vpsrlw $8,<x6=reg256#7,>v01=reg256#7
# asm 2: vpsrlw $8,<x6=%ymm6,>v01=%ymm6
vpsrlw $8, % ymm6, % ymm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <x7=reg256#8,<mask5=reg256#6,>v11=reg256#8
# asm 2: vpand <x7=%ymm7,<mask5=%ymm5,>v11=%ymm7
vpand % ymm7, % ymm5, % ymm7

# qhasm: x6 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x6=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x6=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#8,>x7=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm7,>x7=%ymm6
vpor  % ymm6, % ymm7, % ymm6

# qhasm: mem256[ input_0 + 32 ] = x0
# asm 1: vmovupd   <x0=reg256#10,32(<input_0=int64#1)
# asm 2: vmovupd   <x0=%ymm9,32(<input_0=%rdi)
vmovupd   % ymm9, 32( % rdi)

# qhasm: mem256[ input_0 + 288 ] = x1
# asm 1: vmovupd   <x1=reg256#14,288(<input_0=int64#1)
# asm 2: vmovupd   <x1=%ymm13,288(<input_0=%rdi)
vmovupd   % ymm13, 288( % rdi)

# qhasm: mem256[ input_0 + 544 ] = x2
# asm 1: vmovupd   <x2=reg256#15,544(<input_0=int64#1)
# asm 2: vmovupd   <x2=%ymm14,544(<input_0=%rdi)
vmovupd   % ymm14, 544( % rdi)

# qhasm: mem256[ input_0 + 800 ] = x3
# asm 1: vmovupd   <x3=reg256#11,800(<input_0=int64#1)
# asm 2: vmovupd   <x3=%ymm10,800(<input_0=%rdi)
vmovupd   % ymm10, 800( % rdi)

# qhasm: mem256[ input_0 + 1056 ] = x4
# asm 1: vmovupd   <x4=reg256#12,1056(<input_0=int64#1)
# asm 2: vmovupd   <x4=%ymm11,1056(<input_0=%rdi)
vmovupd   % ymm11, 1056( % rdi)

# qhasm: mem256[ input_0 + 1312 ] = x5
# asm 1: vmovupd   <x5=reg256#9,1312(<input_0=int64#1)
# asm 2: vmovupd   <x5=%ymm8,1312(<input_0=%rdi)
vmovupd   % ymm8, 1312( % rdi)

# qhasm: mem256[ input_0 + 1568 ] = x6
# asm 1: vmovupd   <x6=reg256#13,1568(<input_0=int64#1)
# asm 2: vmovupd   <x6=%ymm12,1568(<input_0=%rdi)
vmovupd   % ymm12, 1568( % rdi)

# qhasm: mem256[ input_0 + 1824 ] = x7
# asm 1: vmovupd   <x7=reg256#7,1824(<input_0=int64#1)
# asm 2: vmovupd   <x7=%ymm6,1824(<input_0=%rdi)
vmovupd   % ymm6, 1824( % rdi)

# qhasm: x0 = mem256[ input_0 + 64 ]
# asm 1: vmovupd   64(<input_0=int64#1),>x0=reg256#7
# asm 2: vmovupd   64(<input_0=%rdi),>x0=%ymm6
vmovupd   64( % rdi), % ymm6

# qhasm: x1 = mem256[ input_0 + 320 ]
# asm 1: vmovupd   320(<input_0=int64#1),>x1=reg256#8
# asm 2: vmovupd   320(<input_0=%rdi),>x1=%ymm7
vmovupd   320( % rdi), % ymm7

# qhasm: x2 = mem256[ input_0 + 576 ]
# asm 1: vmovupd   576(<input_0=int64#1),>x2=reg256#9
# asm 2: vmovupd   576(<input_0=%rdi),>x2=%ymm8
vmovupd   576( % rdi), % ymm8

# qhasm: x3 = mem256[ input_0 + 832 ]
# asm 1: vmovupd   832(<input_0=int64#1),>x3=reg256#10
# asm 2: vmovupd   832(<input_0=%rdi),>x3=%ymm9
vmovupd   832( % rdi), % ymm9

# qhasm: x4 = mem256[ input_0 + 1088 ]
# asm 1: vmovupd   1088(<input_0=int64#1),>x4=reg256#11
# asm 2: vmovupd   1088(<input_0=%rdi),>x4=%ymm10
vmovupd   1088( % rdi), % ymm10

# qhasm: x5 = mem256[ input_0 + 1344 ]
# asm 1: vmovupd   1344(<input_0=int64#1),>x5=reg256#12
# asm 2: vmovupd   1344(<input_0=%rdi),>x5=%ymm11
vmovupd   1344( % rdi), % ymm11

# qhasm: x6 = mem256[ input_0 + 1600 ]
# asm 1: vmovupd   1600(<input_0=int64#1),>x6=reg256#13
# asm 2: vmovupd   1600(<input_0=%rdi),>x6=%ymm12
vmovupd   1600( % rdi), % ymm12

# qhasm: x7 = mem256[ input_0 + 1856 ]
# asm 1: vmovupd   1856(<input_0=int64#1),>x7=reg256#14
# asm 2: vmovupd   1856(<input_0=%rdi),>x7=%ymm13
vmovupd   1856( % rdi), % ymm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <x0=reg256#7,<mask0=reg256#1,>v00=reg256#15
# asm 2: vpand <x0=%ymm6,<mask0=%ymm0,>v00=%ymm14
vpand % ymm6, % ymm0, % ymm14

# qhasm: 4x v10 = x4 << 32
# asm 1: vpsllq $32,<x4=reg256#11,>v10=reg256#16
# asm 2: vpsllq $32,<x4=%ymm10,>v10=%ymm15
vpsllq $32, % ymm10, % ymm15

# qhasm: 4x v01 = x0 unsigned>> 32
# asm 1: vpsrlq $32,<x0=reg256#7,>v01=reg256#7
# asm 2: vpsrlq $32,<x0=%ymm6,>v01=%ymm6
vpsrlq $32, % ymm6, % ymm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <x4=reg256#11,<mask1=reg256#2,>v11=reg256#11
# asm 2: vpand <x4=%ymm10,<mask1=%ymm1,>v11=%ymm10
vpand % ymm10, % ymm1, % ymm10

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x0=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x0=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x4 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#11,>x4=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm10,>x4=%ymm6
vpor  % ymm6, % ymm10, % ymm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <x1=reg256#8,<mask0=reg256#1,>v00=reg256#11
# asm 2: vpand <x1=%ymm7,<mask0=%ymm0,>v00=%ymm10
vpand % ymm7, % ymm0, % ymm10

# qhasm: 4x v10 = x5 << 32
# asm 1: vpsllq $32,<x5=reg256#12,>v10=reg256#16
# asm 2: vpsllq $32,<x5=%ymm11,>v10=%ymm15
vpsllq $32, % ymm11, % ymm15

# qhasm: 4x v01 = x1 unsigned>> 32
# asm 1: vpsrlq $32,<x1=reg256#8,>v01=reg256#8
# asm 2: vpsrlq $32,<x1=%ymm7,>v01=%ymm7
vpsrlq $32, % ymm7, % ymm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <x5=reg256#12,<mask1=reg256#2,>v11=reg256#12
# asm 2: vpand <x5=%ymm11,<mask1=%ymm1,>v11=%ymm11
vpand % ymm11, % ymm1, % ymm11

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#16,>x1=reg256#11
# asm 2: vpor  <v00=%ymm10,<v10=%ymm15,>x1=%ymm10
vpor  % ymm10, % ymm15, % ymm10

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#12,>x5=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm11,>x5=%ymm7
vpor  % ymm7, % ymm11, % ymm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <x2=reg256#9,<mask0=reg256#1,>v00=reg256#12
# asm 2: vpand <x2=%ymm8,<mask0=%ymm0,>v00=%ymm11
vpand % ymm8, % ymm0, % ymm11

# qhasm: 4x v10 = x6 << 32
# asm 1: vpsllq $32,<x6=reg256#13,>v10=reg256#16
# asm 2: vpsllq $32,<x6=%ymm12,>v10=%ymm15
vpsllq $32, % ymm12, % ymm15

# qhasm: 4x v01 = x2 unsigned>> 32
# asm 1: vpsrlq $32,<x2=reg256#9,>v01=reg256#9
# asm 2: vpsrlq $32,<x2=%ymm8,>v01=%ymm8
vpsrlq $32, % ymm8, % ymm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <x6=reg256#13,<mask1=reg256#2,>v11=reg256#13
# asm 2: vpand <x6=%ymm12,<mask1=%ymm1,>v11=%ymm12
vpand % ymm12, % ymm1, % ymm12

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x2=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x2=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#9,<v11=reg256#13,>x6=reg256#9
# asm 2: vpor  <v01=%ymm8,<v11=%ymm12,>x6=%ymm8
vpor  % ymm8, % ymm12, % ymm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <x3=reg256#10,<mask0=reg256#1,>v00=reg256#13
# asm 2: vpand <x3=%ymm9,<mask0=%ymm0,>v00=%ymm12
vpand % ymm9, % ymm0, % ymm12

# qhasm: 4x v10 = x7 << 32
# asm 1: vpsllq $32,<x7=reg256#14,>v10=reg256#16
# asm 2: vpsllq $32,<x7=%ymm13,>v10=%ymm15
vpsllq $32, % ymm13, % ymm15

# qhasm: 4x v01 = x3 unsigned>> 32
# asm 1: vpsrlq $32,<x3=reg256#10,>v01=reg256#10
# asm 2: vpsrlq $32,<x3=%ymm9,>v01=%ymm9
vpsrlq $32, % ymm9, % ymm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <x7=reg256#14,<mask1=reg256#2,>v11=reg256#14
# asm 2: vpand <x7=%ymm13,<mask1=%ymm1,>v11=%ymm13
vpand % ymm13, % ymm1, % ymm13

# qhasm: x3 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x3=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x3=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#10,<v11=reg256#14,>x7=reg256#10
# asm 2: vpor  <v01=%ymm9,<v11=%ymm13,>x7=%ymm9
vpor  % ymm9, % ymm13, % ymm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <x0=reg256#15,<mask2=reg256#3,>v00=reg256#14
# asm 2: vpand <x0=%ymm14,<mask2=%ymm2,>v00=%ymm13
vpand % ymm14, % ymm2, % ymm13

# qhasm: 8x v10 = x2 << 16
# asm 1: vpslld $16,<x2=reg256#12,>v10=reg256#16
# asm 2: vpslld $16,<x2=%ymm11,>v10=%ymm15
vpslld $16, % ymm11, % ymm15

# qhasm: 8x v01 = x0 unsigned>> 16
# asm 1: vpsrld $16,<x0=reg256#15,>v01=reg256#15
# asm 2: vpsrld $16,<x0=%ymm14,>v01=%ymm14
vpsrld $16, % ymm14, % ymm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <x2=reg256#12,<mask3=reg256#4,>v11=reg256#12
# asm 2: vpand <x2=%ymm11,<mask3=%ymm3,>v11=%ymm11
vpand % ymm11, % ymm3, % ymm11

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#14,<v10=reg256#16,>x0=reg256#14
# asm 2: vpor  <v00=%ymm13,<v10=%ymm15,>x0=%ymm13
vpor  % ymm13, % ymm15, % ymm13

# qhasm: x2 = v01 | v11
# asm 1: vpor  <v01=reg256#15,<v11=reg256#12,>x2=reg256#12
# asm 2: vpor  <v01=%ymm14,<v11=%ymm11,>x2=%ymm11
vpor  % ymm14, % ymm11, % ymm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <x1=reg256#11,<mask2=reg256#3,>v00=reg256#15
# asm 2: vpand <x1=%ymm10,<mask2=%ymm2,>v00=%ymm14
vpand % ymm10, % ymm2, % ymm14

# qhasm: 8x v10 = x3 << 16
# asm 1: vpslld $16,<x3=reg256#13,>v10=reg256#16
# asm 2: vpslld $16,<x3=%ymm12,>v10=%ymm15
vpslld $16, % ymm12, % ymm15

# qhasm: 8x v01 = x1 unsigned>> 16
# asm 1: vpsrld $16,<x1=reg256#11,>v01=reg256#11
# asm 2: vpsrld $16,<x1=%ymm10,>v01=%ymm10
vpsrld $16, % ymm10, % ymm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <x3=reg256#13,<mask3=reg256#4,>v11=reg256#13
# asm 2: vpand <x3=%ymm12,<mask3=%ymm3,>v11=%ymm12
vpand % ymm12, % ymm3, % ymm12

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x1=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x1=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#11,<v11=reg256#13,>x3=reg256#11
# asm 2: vpor  <v01=%ymm10,<v11=%ymm12,>x3=%ymm10
vpor  % ymm10, % ymm12, % ymm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <x4=reg256#7,<mask2=reg256#3,>v00=reg256#13
# asm 2: vpand <x4=%ymm6,<mask2=%ymm2,>v00=%ymm12
vpand % ymm6, % ymm2, % ymm12

# qhasm: 8x v10 = x6 << 16
# asm 1: vpslld $16,<x6=reg256#9,>v10=reg256#16
# asm 2: vpslld $16,<x6=%ymm8,>v10=%ymm15
vpslld $16, % ymm8, % ymm15

# qhasm: 8x v01 = x4 unsigned>> 16
# asm 1: vpsrld $16,<x4=reg256#7,>v01=reg256#7
# asm 2: vpsrld $16,<x4=%ymm6,>v01=%ymm6
vpsrld $16, % ymm6, % ymm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <x6=reg256#9,<mask3=reg256#4,>v11=reg256#9
# asm 2: vpand <x6=%ymm8,<mask3=%ymm3,>v11=%ymm8
vpand % ymm8, % ymm3, % ymm8

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x4=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x4=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#9,>x6=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm8,>x6=%ymm6
vpor  % ymm6, % ymm8, % ymm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <x5=reg256#8,<mask2=reg256#3,>v00=reg256#9
# asm 2: vpand <x5=%ymm7,<mask2=%ymm2,>v00=%ymm8
vpand % ymm7, % ymm2, % ymm8

# qhasm: 8x v10 = x7 << 16
# asm 1: vpslld $16,<x7=reg256#10,>v10=reg256#16
# asm 2: vpslld $16,<x7=%ymm9,>v10=%ymm15
vpslld $16, % ymm9, % ymm15

# qhasm: 8x v01 = x5 unsigned>> 16
# asm 1: vpsrld $16,<x5=reg256#8,>v01=reg256#8
# asm 2: vpsrld $16,<x5=%ymm7,>v01=%ymm7
vpsrld $16, % ymm7, % ymm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <x7=reg256#10,<mask3=reg256#4,>v11=reg256#10
# asm 2: vpand <x7=%ymm9,<mask3=%ymm3,>v11=%ymm9
vpand % ymm9, % ymm3, % ymm9

# qhasm: x5 = v00 | v10
# asm 1: vpor  <v00=reg256#9,<v10=reg256#16,>x5=reg256#9
# asm 2: vpor  <v00=%ymm8,<v10=%ymm15,>x5=%ymm8
vpor  % ymm8, % ymm15, % ymm8

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#10,>x7=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm9,>x7=%ymm7
vpor  % ymm7, % ymm9, % ymm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <x0=reg256#14,<mask4=reg256#5,>v00=reg256#10
# asm 2: vpand <x0=%ymm13,<mask4=%ymm4,>v00=%ymm9
vpand % ymm13, % ymm4, % ymm9

# qhasm: 16x v10 = x1 << 8
# asm 1: vpsllw $8,<x1=reg256#15,>v10=reg256#16
# asm 2: vpsllw $8,<x1=%ymm14,>v10=%ymm15
vpsllw $8, % ymm14, % ymm15

# qhasm: 16x v01 = x0 unsigned>> 8
# asm 1: vpsrlw $8,<x0=reg256#14,>v01=reg256#14
# asm 2: vpsrlw $8,<x0=%ymm13,>v01=%ymm13
vpsrlw $8, % ymm13, % ymm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <x1=reg256#15,<mask5=reg256#6,>v11=reg256#15
# asm 2: vpand <x1=%ymm14,<mask5=%ymm5,>v11=%ymm14
vpand % ymm14, % ymm5, % ymm14

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#10,<v10=reg256#16,>x0=reg256#10
# asm 2: vpor  <v00=%ymm9,<v10=%ymm15,>x0=%ymm9
vpor  % ymm9, % ymm15, % ymm9

# qhasm: x1 = v01 | v11
# asm 1: vpor  <v01=reg256#14,<v11=reg256#15,>x1=reg256#14
# asm 2: vpor  <v01=%ymm13,<v11=%ymm14,>x1=%ymm13
vpor  % ymm13, % ymm14, % ymm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <x2=reg256#12,<mask4=reg256#5,>v00=reg256#15
# asm 2: vpand <x2=%ymm11,<mask4=%ymm4,>v00=%ymm14
vpand % ymm11, % ymm4, % ymm14

# qhasm: 16x v10 = x3 << 8
# asm 1: vpsllw $8,<x3=reg256#11,>v10=reg256#16
# asm 2: vpsllw $8,<x3=%ymm10,>v10=%ymm15
vpsllw $8, % ymm10, % ymm15

# qhasm: 16x v01 = x2 unsigned>> 8
# asm 1: vpsrlw $8,<x2=reg256#12,>v01=reg256#12
# asm 2: vpsrlw $8,<x2=%ymm11,>v01=%ymm11
vpsrlw $8, % ymm11, % ymm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <x3=reg256#11,<mask5=reg256#6,>v11=reg256#11
# asm 2: vpand <x3=%ymm10,<mask5=%ymm5,>v11=%ymm10
vpand % ymm10, % ymm5, % ymm10

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x2=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x2=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#12,<v11=reg256#11,>x3=reg256#11
# asm 2: vpor  <v01=%ymm11,<v11=%ymm10,>x3=%ymm10
vpor  % ymm11, % ymm10, % ymm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <x4=reg256#13,<mask4=reg256#5,>v00=reg256#12
# asm 2: vpand <x4=%ymm12,<mask4=%ymm4,>v00=%ymm11
vpand % ymm12, % ymm4, % ymm11

# qhasm: 16x v10 = x5 << 8
# asm 1: vpsllw $8,<x5=reg256#9,>v10=reg256#16
# asm 2: vpsllw $8,<x5=%ymm8,>v10=%ymm15
vpsllw $8, % ymm8, % ymm15

# qhasm: 16x v01 = x4 unsigned>> 8
# asm 1: vpsrlw $8,<x4=reg256#13,>v01=reg256#13
# asm 2: vpsrlw $8,<x4=%ymm12,>v01=%ymm12
vpsrlw $8, % ymm12, % ymm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <x5=reg256#9,<mask5=reg256#6,>v11=reg256#9
# asm 2: vpand <x5=%ymm8,<mask5=%ymm5,>v11=%ymm8
vpand % ymm8, % ymm5, % ymm8

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x4=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x4=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#13,<v11=reg256#9,>x5=reg256#9
# asm 2: vpor  <v01=%ymm12,<v11=%ymm8,>x5=%ymm8
vpor  % ymm12, % ymm8, % ymm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <x6=reg256#7,<mask4=reg256#5,>v00=reg256#13
# asm 2: vpand <x6=%ymm6,<mask4=%ymm4,>v00=%ymm12
vpand % ymm6, % ymm4, % ymm12

# qhasm: 16x v10 = x7 << 8
# asm 1: vpsllw $8,<x7=reg256#8,>v10=reg256#16
# asm 2: vpsllw $8,<x7=%ymm7,>v10=%ymm15
vpsllw $8, % ymm7, % ymm15

# qhasm: 16x v01 = x6 unsigned>> 8
# asm 1: vpsrlw $8,<x6=reg256#7,>v01=reg256#7
# asm 2: vpsrlw $8,<x6=%ymm6,>v01=%ymm6
vpsrlw $8, % ymm6, % ymm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <x7=reg256#8,<mask5=reg256#6,>v11=reg256#8
# asm 2: vpand <x7=%ymm7,<mask5=%ymm5,>v11=%ymm7
vpand % ymm7, % ymm5, % ymm7

# qhasm: x6 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x6=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x6=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#8,>x7=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm7,>x7=%ymm6
vpor  % ymm6, % ymm7, % ymm6

# qhasm: mem256[ input_0 + 64 ] = x0
# asm 1: vmovupd   <x0=reg256#10,64(<input_0=int64#1)
# asm 2: vmovupd   <x0=%ymm9,64(<input_0=%rdi)
vmovupd   % ymm9, 64( % rdi)

# qhasm: mem256[ input_0 + 320 ] = x1
# asm 1: vmovupd   <x1=reg256#14,320(<input_0=int64#1)
# asm 2: vmovupd   <x1=%ymm13,320(<input_0=%rdi)
vmovupd   % ymm13, 320( % rdi)

# qhasm: mem256[ input_0 + 576 ] = x2
# asm 1: vmovupd   <x2=reg256#15,576(<input_0=int64#1)
# asm 2: vmovupd   <x2=%ymm14,576(<input_0=%rdi)
vmovupd   % ymm14, 576( % rdi)

# qhasm: mem256[ input_0 + 832 ] = x3
# asm 1: vmovupd   <x3=reg256#11,832(<input_0=int64#1)
# asm 2: vmovupd   <x3=%ymm10,832(<input_0=%rdi)
vmovupd   % ymm10, 832( % rdi)

# qhasm: mem256[ input_0 + 1088 ] = x4
# asm 1: vmovupd   <x4=reg256#12,1088(<input_0=int64#1)
# asm 2: vmovupd   <x4=%ymm11,1088(<input_0=%rdi)
vmovupd   % ymm11, 1088( % rdi)

# qhasm: mem256[ input_0 + 1344 ] = x5
# asm 1: vmovupd   <x5=reg256#9,1344(<input_0=int64#1)
# asm 2: vmovupd   <x5=%ymm8,1344(<input_0=%rdi)
vmovupd   % ymm8, 1344( % rdi)

# qhasm: mem256[ input_0 + 1600 ] = x6
# asm 1: vmovupd   <x6=reg256#13,1600(<input_0=int64#1)
# asm 2: vmovupd   <x6=%ymm12,1600(<input_0=%rdi)
vmovupd   % ymm12, 1600( % rdi)

# qhasm: mem256[ input_0 + 1856 ] = x7
# asm 1: vmovupd   <x7=reg256#7,1856(<input_0=int64#1)
# asm 2: vmovupd   <x7=%ymm6,1856(<input_0=%rdi)
vmovupd   % ymm6, 1856( % rdi)

# qhasm: x0 = mem256[ input_0 + 96 ]
# asm 1: vmovupd   96(<input_0=int64#1),>x0=reg256#7
# asm 2: vmovupd   96(<input_0=%rdi),>x0=%ymm6
vmovupd   96( % rdi), % ymm6

# qhasm: x1 = mem256[ input_0 + 352 ]
# asm 1: vmovupd   352(<input_0=int64#1),>x1=reg256#8
# asm 2: vmovupd   352(<input_0=%rdi),>x1=%ymm7
vmovupd   352( % rdi), % ymm7

# qhasm: x2 = mem256[ input_0 + 608 ]
# asm 1: vmovupd   608(<input_0=int64#1),>x2=reg256#9
# asm 2: vmovupd   608(<input_0=%rdi),>x2=%ymm8
vmovupd   608( % rdi), % ymm8

# qhasm: x3 = mem256[ input_0 + 864 ]
# asm 1: vmovupd   864(<input_0=int64#1),>x3=reg256#10
# asm 2: vmovupd   864(<input_0=%rdi),>x3=%ymm9
vmovupd   864( % rdi), % ymm9

# qhasm: x4 = mem256[ input_0 + 1120 ]
# asm 1: vmovupd   1120(<input_0=int64#1),>x4=reg256#11
# asm 2: vmovupd   1120(<input_0=%rdi),>x4=%ymm10
vmovupd   1120( % rdi), % ymm10

# qhasm: x5 = mem256[ input_0 + 1376 ]
# asm 1: vmovupd   1376(<input_0=int64#1),>x5=reg256#12
# asm 2: vmovupd   1376(<input_0=%rdi),>x5=%ymm11
vmovupd   1376( % rdi), % ymm11

# qhasm: x6 = mem256[ input_0 + 1632 ]
# asm 1: vmovupd   1632(<input_0=int64#1),>x6=reg256#13
# asm 2: vmovupd   1632(<input_0=%rdi),>x6=%ymm12
vmovupd   1632( % rdi), % ymm12

# qhasm: x7 = mem256[ input_0 + 1888 ]
# asm 1: vmovupd   1888(<input_0=int64#1),>x7=reg256#14
# asm 2: vmovupd   1888(<input_0=%rdi),>x7=%ymm13
vmovupd   1888( % rdi), % ymm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <x0=reg256#7,<mask0=reg256#1,>v00=reg256#15
# asm 2: vpand <x0=%ymm6,<mask0=%ymm0,>v00=%ymm14
vpand % ymm6, % ymm0, % ymm14

# qhasm: 4x v10 = x4 << 32
# asm 1: vpsllq $32,<x4=reg256#11,>v10=reg256#16
# asm 2: vpsllq $32,<x4=%ymm10,>v10=%ymm15
vpsllq $32, % ymm10, % ymm15

# qhasm: 4x v01 = x0 unsigned>> 32
# asm 1: vpsrlq $32,<x0=reg256#7,>v01=reg256#7
# asm 2: vpsrlq $32,<x0=%ymm6,>v01=%ymm6
vpsrlq $32, % ymm6, % ymm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <x4=reg256#11,<mask1=reg256#2,>v11=reg256#11
# asm 2: vpand <x4=%ymm10,<mask1=%ymm1,>v11=%ymm10
vpand % ymm10, % ymm1, % ymm10

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x0=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x0=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x4 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#11,>x4=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm10,>x4=%ymm6
vpor  % ymm6, % ymm10, % ymm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <x1=reg256#8,<mask0=reg256#1,>v00=reg256#11
# asm 2: vpand <x1=%ymm7,<mask0=%ymm0,>v00=%ymm10
vpand % ymm7, % ymm0, % ymm10

# qhasm: 4x v10 = x5 << 32
# asm 1: vpsllq $32,<x5=reg256#12,>v10=reg256#16
# asm 2: vpsllq $32,<x5=%ymm11,>v10=%ymm15
vpsllq $32, % ymm11, % ymm15

# qhasm: 4x v01 = x1 unsigned>> 32
# asm 1: vpsrlq $32,<x1=reg256#8,>v01=reg256#8
# asm 2: vpsrlq $32,<x1=%ymm7,>v01=%ymm7
vpsrlq $32, % ymm7, % ymm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <x5=reg256#12,<mask1=reg256#2,>v11=reg256#12
# asm 2: vpand <x5=%ymm11,<mask1=%ymm1,>v11=%ymm11
vpand % ymm11, % ymm1, % ymm11

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#16,>x1=reg256#11
# asm 2: vpor  <v00=%ymm10,<v10=%ymm15,>x1=%ymm10
vpor  % ymm10, % ymm15, % ymm10

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#12,>x5=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm11,>x5=%ymm7
vpor  % ymm7, % ymm11, % ymm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <x2=reg256#9,<mask0=reg256#1,>v00=reg256#12
# asm 2: vpand <x2=%ymm8,<mask0=%ymm0,>v00=%ymm11
vpand % ymm8, % ymm0, % ymm11

# qhasm: 4x v10 = x6 << 32
# asm 1: vpsllq $32,<x6=reg256#13,>v10=reg256#16
# asm 2: vpsllq $32,<x6=%ymm12,>v10=%ymm15
vpsllq $32, % ymm12, % ymm15

# qhasm: 4x v01 = x2 unsigned>> 32
# asm 1: vpsrlq $32,<x2=reg256#9,>v01=reg256#9
# asm 2: vpsrlq $32,<x2=%ymm8,>v01=%ymm8
vpsrlq $32, % ymm8, % ymm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <x6=reg256#13,<mask1=reg256#2,>v11=reg256#13
# asm 2: vpand <x6=%ymm12,<mask1=%ymm1,>v11=%ymm12
vpand % ymm12, % ymm1, % ymm12

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x2=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x2=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#9,<v11=reg256#13,>x6=reg256#9
# asm 2: vpor  <v01=%ymm8,<v11=%ymm12,>x6=%ymm8
vpor  % ymm8, % ymm12, % ymm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <x3=reg256#10,<mask0=reg256#1,>v00=reg256#13
# asm 2: vpand <x3=%ymm9,<mask0=%ymm0,>v00=%ymm12
vpand % ymm9, % ymm0, % ymm12

# qhasm: 4x v10 = x7 << 32
# asm 1: vpsllq $32,<x7=reg256#14,>v10=reg256#16
# asm 2: vpsllq $32,<x7=%ymm13,>v10=%ymm15
vpsllq $32, % ymm13, % ymm15

# qhasm: 4x v01 = x3 unsigned>> 32
# asm 1: vpsrlq $32,<x3=reg256#10,>v01=reg256#10
# asm 2: vpsrlq $32,<x3=%ymm9,>v01=%ymm9
vpsrlq $32, % ymm9, % ymm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <x7=reg256#14,<mask1=reg256#2,>v11=reg256#14
# asm 2: vpand <x7=%ymm13,<mask1=%ymm1,>v11=%ymm13
vpand % ymm13, % ymm1, % ymm13

# qhasm: x3 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x3=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x3=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#10,<v11=reg256#14,>x7=reg256#10
# asm 2: vpor  <v01=%ymm9,<v11=%ymm13,>x7=%ymm9
vpor  % ymm9, % ymm13, % ymm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <x0=reg256#15,<mask2=reg256#3,>v00=reg256#14
# asm 2: vpand <x0=%ymm14,<mask2=%ymm2,>v00=%ymm13
vpand % ymm14, % ymm2, % ymm13

# qhasm: 8x v10 = x2 << 16
# asm 1: vpslld $16,<x2=reg256#12,>v10=reg256#16
# asm 2: vpslld $16,<x2=%ymm11,>v10=%ymm15
vpslld $16, % ymm11, % ymm15

# qhasm: 8x v01 = x0 unsigned>> 16
# asm 1: vpsrld $16,<x0=reg256#15,>v01=reg256#15
# asm 2: vpsrld $16,<x0=%ymm14,>v01=%ymm14
vpsrld $16, % ymm14, % ymm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <x2=reg256#12,<mask3=reg256#4,>v11=reg256#12
# asm 2: vpand <x2=%ymm11,<mask3=%ymm3,>v11=%ymm11
vpand % ymm11, % ymm3, % ymm11

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#14,<v10=reg256#16,>x0=reg256#14
# asm 2: vpor  <v00=%ymm13,<v10=%ymm15,>x0=%ymm13
vpor  % ymm13, % ymm15, % ymm13

# qhasm: x2 = v01 | v11
# asm 1: vpor  <v01=reg256#15,<v11=reg256#12,>x2=reg256#12
# asm 2: vpor  <v01=%ymm14,<v11=%ymm11,>x2=%ymm11
vpor  % ymm14, % ymm11, % ymm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <x1=reg256#11,<mask2=reg256#3,>v00=reg256#15
# asm 2: vpand <x1=%ymm10,<mask2=%ymm2,>v00=%ymm14
vpand % ymm10, % ymm2, % ymm14

# qhasm: 8x v10 = x3 << 16
# asm 1: vpslld $16,<x3=reg256#13,>v10=reg256#16
# asm 2: vpslld $16,<x3=%ymm12,>v10=%ymm15
vpslld $16, % ymm12, % ymm15

# qhasm: 8x v01 = x1 unsigned>> 16
# asm 1: vpsrld $16,<x1=reg256#11,>v01=reg256#11
# asm 2: vpsrld $16,<x1=%ymm10,>v01=%ymm10
vpsrld $16, % ymm10, % ymm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <x3=reg256#13,<mask3=reg256#4,>v11=reg256#13
# asm 2: vpand <x3=%ymm12,<mask3=%ymm3,>v11=%ymm12
vpand % ymm12, % ymm3, % ymm12

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x1=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x1=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#11,<v11=reg256#13,>x3=reg256#11
# asm 2: vpor  <v01=%ymm10,<v11=%ymm12,>x3=%ymm10
vpor  % ymm10, % ymm12, % ymm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <x4=reg256#7,<mask2=reg256#3,>v00=reg256#13
# asm 2: vpand <x4=%ymm6,<mask2=%ymm2,>v00=%ymm12
vpand % ymm6, % ymm2, % ymm12

# qhasm: 8x v10 = x6 << 16
# asm 1: vpslld $16,<x6=reg256#9,>v10=reg256#16
# asm 2: vpslld $16,<x6=%ymm8,>v10=%ymm15
vpslld $16, % ymm8, % ymm15

# qhasm: 8x v01 = x4 unsigned>> 16
# asm 1: vpsrld $16,<x4=reg256#7,>v01=reg256#7
# asm 2: vpsrld $16,<x4=%ymm6,>v01=%ymm6
vpsrld $16, % ymm6, % ymm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <x6=reg256#9,<mask3=reg256#4,>v11=reg256#9
# asm 2: vpand <x6=%ymm8,<mask3=%ymm3,>v11=%ymm8
vpand % ymm8, % ymm3, % ymm8

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x4=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x4=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#9,>x6=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm8,>x6=%ymm6
vpor  % ymm6, % ymm8, % ymm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <x5=reg256#8,<mask2=reg256#3,>v00=reg256#9
# asm 2: vpand <x5=%ymm7,<mask2=%ymm2,>v00=%ymm8
vpand % ymm7, % ymm2, % ymm8

# qhasm: 8x v10 = x7 << 16
# asm 1: vpslld $16,<x7=reg256#10,>v10=reg256#16
# asm 2: vpslld $16,<x7=%ymm9,>v10=%ymm15
vpslld $16, % ymm9, % ymm15

# qhasm: 8x v01 = x5 unsigned>> 16
# asm 1: vpsrld $16,<x5=reg256#8,>v01=reg256#8
# asm 2: vpsrld $16,<x5=%ymm7,>v01=%ymm7
vpsrld $16, % ymm7, % ymm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <x7=reg256#10,<mask3=reg256#4,>v11=reg256#10
# asm 2: vpand <x7=%ymm9,<mask3=%ymm3,>v11=%ymm9
vpand % ymm9, % ymm3, % ymm9

# qhasm: x5 = v00 | v10
# asm 1: vpor  <v00=reg256#9,<v10=reg256#16,>x5=reg256#9
# asm 2: vpor  <v00=%ymm8,<v10=%ymm15,>x5=%ymm8
vpor  % ymm8, % ymm15, % ymm8

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#10,>x7=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm9,>x7=%ymm7
vpor  % ymm7, % ymm9, % ymm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <x0=reg256#14,<mask4=reg256#5,>v00=reg256#10
# asm 2: vpand <x0=%ymm13,<mask4=%ymm4,>v00=%ymm9
vpand % ymm13, % ymm4, % ymm9

# qhasm: 16x v10 = x1 << 8
# asm 1: vpsllw $8,<x1=reg256#15,>v10=reg256#16
# asm 2: vpsllw $8,<x1=%ymm14,>v10=%ymm15
vpsllw $8, % ymm14, % ymm15

# qhasm: 16x v01 = x0 unsigned>> 8
# asm 1: vpsrlw $8,<x0=reg256#14,>v01=reg256#14
# asm 2: vpsrlw $8,<x0=%ymm13,>v01=%ymm13
vpsrlw $8, % ymm13, % ymm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <x1=reg256#15,<mask5=reg256#6,>v11=reg256#15
# asm 2: vpand <x1=%ymm14,<mask5=%ymm5,>v11=%ymm14
vpand % ymm14, % ymm5, % ymm14

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#10,<v10=reg256#16,>x0=reg256#10
# asm 2: vpor  <v00=%ymm9,<v10=%ymm15,>x0=%ymm9
vpor  % ymm9, % ymm15, % ymm9

# qhasm: x1 = v01 | v11
# asm 1: vpor  <v01=reg256#14,<v11=reg256#15,>x1=reg256#14
# asm 2: vpor  <v01=%ymm13,<v11=%ymm14,>x1=%ymm13
vpor  % ymm13, % ymm14, % ymm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <x2=reg256#12,<mask4=reg256#5,>v00=reg256#15
# asm 2: vpand <x2=%ymm11,<mask4=%ymm4,>v00=%ymm14
vpand % ymm11, % ymm4, % ymm14

# qhasm: 16x v10 = x3 << 8
# asm 1: vpsllw $8,<x3=reg256#11,>v10=reg256#16
# asm 2: vpsllw $8,<x3=%ymm10,>v10=%ymm15
vpsllw $8, % ymm10, % ymm15

# qhasm: 16x v01 = x2 unsigned>> 8
# asm 1: vpsrlw $8,<x2=reg256#12,>v01=reg256#12
# asm 2: vpsrlw $8,<x2=%ymm11,>v01=%ymm11
vpsrlw $8, % ymm11, % ymm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <x3=reg256#11,<mask5=reg256#6,>v11=reg256#11
# asm 2: vpand <x3=%ymm10,<mask5=%ymm5,>v11=%ymm10
vpand % ymm10, % ymm5, % ymm10

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x2=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x2=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#12,<v11=reg256#11,>x3=reg256#11
# asm 2: vpor  <v01=%ymm11,<v11=%ymm10,>x3=%ymm10
vpor  % ymm11, % ymm10, % ymm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <x4=reg256#13,<mask4=reg256#5,>v00=reg256#12
# asm 2: vpand <x4=%ymm12,<mask4=%ymm4,>v00=%ymm11
vpand % ymm12, % ymm4, % ymm11

# qhasm: 16x v10 = x5 << 8
# asm 1: vpsllw $8,<x5=reg256#9,>v10=reg256#16
# asm 2: vpsllw $8,<x5=%ymm8,>v10=%ymm15
vpsllw $8, % ymm8, % ymm15

# qhasm: 16x v01 = x4 unsigned>> 8
# asm 1: vpsrlw $8,<x4=reg256#13,>v01=reg256#13
# asm 2: vpsrlw $8,<x4=%ymm12,>v01=%ymm12
vpsrlw $8, % ymm12, % ymm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <x5=reg256#9,<mask5=reg256#6,>v11=reg256#9
# asm 2: vpand <x5=%ymm8,<mask5=%ymm5,>v11=%ymm8
vpand % ymm8, % ymm5, % ymm8

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x4=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x4=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#13,<v11=reg256#9,>x5=reg256#9
# asm 2: vpor  <v01=%ymm12,<v11=%ymm8,>x5=%ymm8
vpor  % ymm12, % ymm8, % ymm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <x6=reg256#7,<mask4=reg256#5,>v00=reg256#13
# asm 2: vpand <x6=%ymm6,<mask4=%ymm4,>v00=%ymm12
vpand % ymm6, % ymm4, % ymm12

# qhasm: 16x v10 = x7 << 8
# asm 1: vpsllw $8,<x7=reg256#8,>v10=reg256#16
# asm 2: vpsllw $8,<x7=%ymm7,>v10=%ymm15
vpsllw $8, % ymm7, % ymm15

# qhasm: 16x v01 = x6 unsigned>> 8
# asm 1: vpsrlw $8,<x6=reg256#7,>v01=reg256#7
# asm 2: vpsrlw $8,<x6=%ymm6,>v01=%ymm6
vpsrlw $8, % ymm6, % ymm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <x7=reg256#8,<mask5=reg256#6,>v11=reg256#8
# asm 2: vpand <x7=%ymm7,<mask5=%ymm5,>v11=%ymm7
vpand % ymm7, % ymm5, % ymm7

# qhasm: x6 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x6=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x6=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#8,>x7=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm7,>x7=%ymm6
vpor  % ymm6, % ymm7, % ymm6

# qhasm: mem256[ input_0 + 96 ] = x0
# asm 1: vmovupd   <x0=reg256#10,96(<input_0=int64#1)
# asm 2: vmovupd   <x0=%ymm9,96(<input_0=%rdi)
vmovupd   % ymm9, 96( % rdi)

# qhasm: mem256[ input_0 + 352 ] = x1
# asm 1: vmovupd   <x1=reg256#14,352(<input_0=int64#1)
# asm 2: vmovupd   <x1=%ymm13,352(<input_0=%rdi)
vmovupd   % ymm13, 352( % rdi)

# qhasm: mem256[ input_0 + 608 ] = x2
# asm 1: vmovupd   <x2=reg256#15,608(<input_0=int64#1)
# asm 2: vmovupd   <x2=%ymm14,608(<input_0=%rdi)
vmovupd   % ymm14, 608( % rdi)

# qhasm: mem256[ input_0 + 864 ] = x3
# asm 1: vmovupd   <x3=reg256#11,864(<input_0=int64#1)
# asm 2: vmovupd   <x3=%ymm10,864(<input_0=%rdi)
vmovupd   % ymm10, 864( % rdi)

# qhasm: mem256[ input_0 + 1120 ] = x4
# asm 1: vmovupd   <x4=reg256#12,1120(<input_0=int64#1)
# asm 2: vmovupd   <x4=%ymm11,1120(<input_0=%rdi)
vmovupd   % ymm11, 1120( % rdi)

# qhasm: mem256[ input_0 + 1376 ] = x5
# asm 1: vmovupd   <x5=reg256#9,1376(<input_0=int64#1)
# asm 2: vmovupd   <x5=%ymm8,1376(<input_0=%rdi)
vmovupd   % ymm8, 1376( % rdi)

# qhasm: mem256[ input_0 + 1632 ] = x6
# asm 1: vmovupd   <x6=reg256#13,1632(<input_0=int64#1)
# asm 2: vmovupd   <x6=%ymm12,1632(<input_0=%rdi)
vmovupd   % ymm12, 1632( % rdi)

# qhasm: mem256[ input_0 + 1888 ] = x7
# asm 1: vmovupd   <x7=reg256#7,1888(<input_0=int64#1)
# asm 2: vmovupd   <x7=%ymm6,1888(<input_0=%rdi)
vmovupd   % ymm6, 1888( % rdi)

# qhasm: x0 = mem256[ input_0 + 128 ]
# asm 1: vmovupd   128(<input_0=int64#1),>x0=reg256#7
# asm 2: vmovupd   128(<input_0=%rdi),>x0=%ymm6
vmovupd   128( % rdi), % ymm6

# qhasm: x1 = mem256[ input_0 + 384 ]
# asm 1: vmovupd   384(<input_0=int64#1),>x1=reg256#8
# asm 2: vmovupd   384(<input_0=%rdi),>x1=%ymm7
vmovupd   384( % rdi), % ymm7

# qhasm: x2 = mem256[ input_0 + 640 ]
# asm 1: vmovupd   640(<input_0=int64#1),>x2=reg256#9
# asm 2: vmovupd   640(<input_0=%rdi),>x2=%ymm8
vmovupd   640( % rdi), % ymm8

# qhasm: x3 = mem256[ input_0 + 896 ]
# asm 1: vmovupd   896(<input_0=int64#1),>x3=reg256#10
# asm 2: vmovupd   896(<input_0=%rdi),>x3=%ymm9
vmovupd   896( % rdi), % ymm9

# qhasm: x4 = mem256[ input_0 + 1152 ]
# asm 1: vmovupd   1152(<input_0=int64#1),>x4=reg256#11
# asm 2: vmovupd   1152(<input_0=%rdi),>x4=%ymm10
vmovupd   1152( % rdi), % ymm10

# qhasm: x5 = mem256[ input_0 + 1408 ]
# asm 1: vmovupd   1408(<input_0=int64#1),>x5=reg256#12
# asm 2: vmovupd   1408(<input_0=%rdi),>x5=%ymm11
vmovupd   1408( % rdi), % ymm11

# qhasm: x6 = mem256[ input_0 + 1664 ]
# asm 1: vmovupd   1664(<input_0=int64#1),>x6=reg256#13
# asm 2: vmovupd   1664(<input_0=%rdi),>x6=%ymm12
vmovupd   1664( % rdi), % ymm12

# qhasm: x7 = mem256[ input_0 + 1920 ]
# asm 1: vmovupd   1920(<input_0=int64#1),>x7=reg256#14
# asm 2: vmovupd   1920(<input_0=%rdi),>x7=%ymm13
vmovupd   1920( % rdi), % ymm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <x0=reg256#7,<mask0=reg256#1,>v00=reg256#15
# asm 2: vpand <x0=%ymm6,<mask0=%ymm0,>v00=%ymm14
vpand % ymm6, % ymm0, % ymm14

# qhasm: 4x v10 = x4 << 32
# asm 1: vpsllq $32,<x4=reg256#11,>v10=reg256#16
# asm 2: vpsllq $32,<x4=%ymm10,>v10=%ymm15
vpsllq $32, % ymm10, % ymm15

# qhasm: 4x v01 = x0 unsigned>> 32
# asm 1: vpsrlq $32,<x0=reg256#7,>v01=reg256#7
# asm 2: vpsrlq $32,<x0=%ymm6,>v01=%ymm6
vpsrlq $32, % ymm6, % ymm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <x4=reg256#11,<mask1=reg256#2,>v11=reg256#11
# asm 2: vpand <x4=%ymm10,<mask1=%ymm1,>v11=%ymm10
vpand % ymm10, % ymm1, % ymm10

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x0=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x0=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x4 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#11,>x4=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm10,>x4=%ymm6
vpor  % ymm6, % ymm10, % ymm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <x1=reg256#8,<mask0=reg256#1,>v00=reg256#11
# asm 2: vpand <x1=%ymm7,<mask0=%ymm0,>v00=%ymm10
vpand % ymm7, % ymm0, % ymm10

# qhasm: 4x v10 = x5 << 32
# asm 1: vpsllq $32,<x5=reg256#12,>v10=reg256#16
# asm 2: vpsllq $32,<x5=%ymm11,>v10=%ymm15
vpsllq $32, % ymm11, % ymm15

# qhasm: 4x v01 = x1 unsigned>> 32
# asm 1: vpsrlq $32,<x1=reg256#8,>v01=reg256#8
# asm 2: vpsrlq $32,<x1=%ymm7,>v01=%ymm7
vpsrlq $32, % ymm7, % ymm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <x5=reg256#12,<mask1=reg256#2,>v11=reg256#12
# asm 2: vpand <x5=%ymm11,<mask1=%ymm1,>v11=%ymm11
vpand % ymm11, % ymm1, % ymm11

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#16,>x1=reg256#11
# asm 2: vpor  <v00=%ymm10,<v10=%ymm15,>x1=%ymm10
vpor  % ymm10, % ymm15, % ymm10

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#12,>x5=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm11,>x5=%ymm7
vpor  % ymm7, % ymm11, % ymm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <x2=reg256#9,<mask0=reg256#1,>v00=reg256#12
# asm 2: vpand <x2=%ymm8,<mask0=%ymm0,>v00=%ymm11
vpand % ymm8, % ymm0, % ymm11

# qhasm: 4x v10 = x6 << 32
# asm 1: vpsllq $32,<x6=reg256#13,>v10=reg256#16
# asm 2: vpsllq $32,<x6=%ymm12,>v10=%ymm15
vpsllq $32, % ymm12, % ymm15

# qhasm: 4x v01 = x2 unsigned>> 32
# asm 1: vpsrlq $32,<x2=reg256#9,>v01=reg256#9
# asm 2: vpsrlq $32,<x2=%ymm8,>v01=%ymm8
vpsrlq $32, % ymm8, % ymm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <x6=reg256#13,<mask1=reg256#2,>v11=reg256#13
# asm 2: vpand <x6=%ymm12,<mask1=%ymm1,>v11=%ymm12
vpand % ymm12, % ymm1, % ymm12

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x2=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x2=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#9,<v11=reg256#13,>x6=reg256#9
# asm 2: vpor  <v01=%ymm8,<v11=%ymm12,>x6=%ymm8
vpor  % ymm8, % ymm12, % ymm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <x3=reg256#10,<mask0=reg256#1,>v00=reg256#13
# asm 2: vpand <x3=%ymm9,<mask0=%ymm0,>v00=%ymm12
vpand % ymm9, % ymm0, % ymm12

# qhasm: 4x v10 = x7 << 32
# asm 1: vpsllq $32,<x7=reg256#14,>v10=reg256#16
# asm 2: vpsllq $32,<x7=%ymm13,>v10=%ymm15
vpsllq $32, % ymm13, % ymm15

# qhasm: 4x v01 = x3 unsigned>> 32
# asm 1: vpsrlq $32,<x3=reg256#10,>v01=reg256#10
# asm 2: vpsrlq $32,<x3=%ymm9,>v01=%ymm9
vpsrlq $32, % ymm9, % ymm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <x7=reg256#14,<mask1=reg256#2,>v11=reg256#14
# asm 2: vpand <x7=%ymm13,<mask1=%ymm1,>v11=%ymm13
vpand % ymm13, % ymm1, % ymm13

# qhasm: x3 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x3=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x3=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#10,<v11=reg256#14,>x7=reg256#10
# asm 2: vpor  <v01=%ymm9,<v11=%ymm13,>x7=%ymm9
vpor  % ymm9, % ymm13, % ymm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <x0=reg256#15,<mask2=reg256#3,>v00=reg256#14
# asm 2: vpand <x0=%ymm14,<mask2=%ymm2,>v00=%ymm13
vpand % ymm14, % ymm2, % ymm13

# qhasm: 8x v10 = x2 << 16
# asm 1: vpslld $16,<x2=reg256#12,>v10=reg256#16
# asm 2: vpslld $16,<x2=%ymm11,>v10=%ymm15
vpslld $16, % ymm11, % ymm15

# qhasm: 8x v01 = x0 unsigned>> 16
# asm 1: vpsrld $16,<x0=reg256#15,>v01=reg256#15
# asm 2: vpsrld $16,<x0=%ymm14,>v01=%ymm14
vpsrld $16, % ymm14, % ymm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <x2=reg256#12,<mask3=reg256#4,>v11=reg256#12
# asm 2: vpand <x2=%ymm11,<mask3=%ymm3,>v11=%ymm11
vpand % ymm11, % ymm3, % ymm11

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#14,<v10=reg256#16,>x0=reg256#14
# asm 2: vpor  <v00=%ymm13,<v10=%ymm15,>x0=%ymm13
vpor  % ymm13, % ymm15, % ymm13

# qhasm: x2 = v01 | v11
# asm 1: vpor  <v01=reg256#15,<v11=reg256#12,>x2=reg256#12
# asm 2: vpor  <v01=%ymm14,<v11=%ymm11,>x2=%ymm11
vpor  % ymm14, % ymm11, % ymm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <x1=reg256#11,<mask2=reg256#3,>v00=reg256#15
# asm 2: vpand <x1=%ymm10,<mask2=%ymm2,>v00=%ymm14
vpand % ymm10, % ymm2, % ymm14

# qhasm: 8x v10 = x3 << 16
# asm 1: vpslld $16,<x3=reg256#13,>v10=reg256#16
# asm 2: vpslld $16,<x3=%ymm12,>v10=%ymm15
vpslld $16, % ymm12, % ymm15

# qhasm: 8x v01 = x1 unsigned>> 16
# asm 1: vpsrld $16,<x1=reg256#11,>v01=reg256#11
# asm 2: vpsrld $16,<x1=%ymm10,>v01=%ymm10
vpsrld $16, % ymm10, % ymm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <x3=reg256#13,<mask3=reg256#4,>v11=reg256#13
# asm 2: vpand <x3=%ymm12,<mask3=%ymm3,>v11=%ymm12
vpand % ymm12, % ymm3, % ymm12

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x1=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x1=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#11,<v11=reg256#13,>x3=reg256#11
# asm 2: vpor  <v01=%ymm10,<v11=%ymm12,>x3=%ymm10
vpor  % ymm10, % ymm12, % ymm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <x4=reg256#7,<mask2=reg256#3,>v00=reg256#13
# asm 2: vpand <x4=%ymm6,<mask2=%ymm2,>v00=%ymm12
vpand % ymm6, % ymm2, % ymm12

# qhasm: 8x v10 = x6 << 16
# asm 1: vpslld $16,<x6=reg256#9,>v10=reg256#16
# asm 2: vpslld $16,<x6=%ymm8,>v10=%ymm15
vpslld $16, % ymm8, % ymm15

# qhasm: 8x v01 = x4 unsigned>> 16
# asm 1: vpsrld $16,<x4=reg256#7,>v01=reg256#7
# asm 2: vpsrld $16,<x4=%ymm6,>v01=%ymm6
vpsrld $16, % ymm6, % ymm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <x6=reg256#9,<mask3=reg256#4,>v11=reg256#9
# asm 2: vpand <x6=%ymm8,<mask3=%ymm3,>v11=%ymm8
vpand % ymm8, % ymm3, % ymm8

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x4=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x4=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#9,>x6=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm8,>x6=%ymm6
vpor  % ymm6, % ymm8, % ymm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <x5=reg256#8,<mask2=reg256#3,>v00=reg256#9
# asm 2: vpand <x5=%ymm7,<mask2=%ymm2,>v00=%ymm8
vpand % ymm7, % ymm2, % ymm8

# qhasm: 8x v10 = x7 << 16
# asm 1: vpslld $16,<x7=reg256#10,>v10=reg256#16
# asm 2: vpslld $16,<x7=%ymm9,>v10=%ymm15
vpslld $16, % ymm9, % ymm15

# qhasm: 8x v01 = x5 unsigned>> 16
# asm 1: vpsrld $16,<x5=reg256#8,>v01=reg256#8
# asm 2: vpsrld $16,<x5=%ymm7,>v01=%ymm7
vpsrld $16, % ymm7, % ymm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <x7=reg256#10,<mask3=reg256#4,>v11=reg256#10
# asm 2: vpand <x7=%ymm9,<mask3=%ymm3,>v11=%ymm9
vpand % ymm9, % ymm3, % ymm9

# qhasm: x5 = v00 | v10
# asm 1: vpor  <v00=reg256#9,<v10=reg256#16,>x5=reg256#9
# asm 2: vpor  <v00=%ymm8,<v10=%ymm15,>x5=%ymm8
vpor  % ymm8, % ymm15, % ymm8

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#10,>x7=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm9,>x7=%ymm7
vpor  % ymm7, % ymm9, % ymm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <x0=reg256#14,<mask4=reg256#5,>v00=reg256#10
# asm 2: vpand <x0=%ymm13,<mask4=%ymm4,>v00=%ymm9
vpand % ymm13, % ymm4, % ymm9

# qhasm: 16x v10 = x1 << 8
# asm 1: vpsllw $8,<x1=reg256#15,>v10=reg256#16
# asm 2: vpsllw $8,<x1=%ymm14,>v10=%ymm15
vpsllw $8, % ymm14, % ymm15

# qhasm: 16x v01 = x0 unsigned>> 8
# asm 1: vpsrlw $8,<x0=reg256#14,>v01=reg256#14
# asm 2: vpsrlw $8,<x0=%ymm13,>v01=%ymm13
vpsrlw $8, % ymm13, % ymm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <x1=reg256#15,<mask5=reg256#6,>v11=reg256#15
# asm 2: vpand <x1=%ymm14,<mask5=%ymm5,>v11=%ymm14
vpand % ymm14, % ymm5, % ymm14

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#10,<v10=reg256#16,>x0=reg256#10
# asm 2: vpor  <v00=%ymm9,<v10=%ymm15,>x0=%ymm9
vpor  % ymm9, % ymm15, % ymm9

# qhasm: x1 = v01 | v11
# asm 1: vpor  <v01=reg256#14,<v11=reg256#15,>x1=reg256#14
# asm 2: vpor  <v01=%ymm13,<v11=%ymm14,>x1=%ymm13
vpor  % ymm13, % ymm14, % ymm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <x2=reg256#12,<mask4=reg256#5,>v00=reg256#15
# asm 2: vpand <x2=%ymm11,<mask4=%ymm4,>v00=%ymm14
vpand % ymm11, % ymm4, % ymm14

# qhasm: 16x v10 = x3 << 8
# asm 1: vpsllw $8,<x3=reg256#11,>v10=reg256#16
# asm 2: vpsllw $8,<x3=%ymm10,>v10=%ymm15
vpsllw $8, % ymm10, % ymm15

# qhasm: 16x v01 = x2 unsigned>> 8
# asm 1: vpsrlw $8,<x2=reg256#12,>v01=reg256#12
# asm 2: vpsrlw $8,<x2=%ymm11,>v01=%ymm11
vpsrlw $8, % ymm11, % ymm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <x3=reg256#11,<mask5=reg256#6,>v11=reg256#11
# asm 2: vpand <x3=%ymm10,<mask5=%ymm5,>v11=%ymm10
vpand % ymm10, % ymm5, % ymm10

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x2=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x2=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#12,<v11=reg256#11,>x3=reg256#11
# asm 2: vpor  <v01=%ymm11,<v11=%ymm10,>x3=%ymm10
vpor  % ymm11, % ymm10, % ymm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <x4=reg256#13,<mask4=reg256#5,>v00=reg256#12
# asm 2: vpand <x4=%ymm12,<mask4=%ymm4,>v00=%ymm11
vpand % ymm12, % ymm4, % ymm11

# qhasm: 16x v10 = x5 << 8
# asm 1: vpsllw $8,<x5=reg256#9,>v10=reg256#16
# asm 2: vpsllw $8,<x5=%ymm8,>v10=%ymm15
vpsllw $8, % ymm8, % ymm15

# qhasm: 16x v01 = x4 unsigned>> 8
# asm 1: vpsrlw $8,<x4=reg256#13,>v01=reg256#13
# asm 2: vpsrlw $8,<x4=%ymm12,>v01=%ymm12
vpsrlw $8, % ymm12, % ymm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <x5=reg256#9,<mask5=reg256#6,>v11=reg256#9
# asm 2: vpand <x5=%ymm8,<mask5=%ymm5,>v11=%ymm8
vpand % ymm8, % ymm5, % ymm8

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x4=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x4=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#13,<v11=reg256#9,>x5=reg256#9
# asm 2: vpor  <v01=%ymm12,<v11=%ymm8,>x5=%ymm8
vpor  % ymm12, % ymm8, % ymm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <x6=reg256#7,<mask4=reg256#5,>v00=reg256#13
# asm 2: vpand <x6=%ymm6,<mask4=%ymm4,>v00=%ymm12
vpand % ymm6, % ymm4, % ymm12

# qhasm: 16x v10 = x7 << 8
# asm 1: vpsllw $8,<x7=reg256#8,>v10=reg256#16
# asm 2: vpsllw $8,<x7=%ymm7,>v10=%ymm15
vpsllw $8, % ymm7, % ymm15

# qhasm: 16x v01 = x6 unsigned>> 8
# asm 1: vpsrlw $8,<x6=reg256#7,>v01=reg256#7
# asm 2: vpsrlw $8,<x6=%ymm6,>v01=%ymm6
vpsrlw $8, % ymm6, % ymm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <x7=reg256#8,<mask5=reg256#6,>v11=reg256#8
# asm 2: vpand <x7=%ymm7,<mask5=%ymm5,>v11=%ymm7
vpand % ymm7, % ymm5, % ymm7

# qhasm: x6 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x6=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x6=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#8,>x7=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm7,>x7=%ymm6
vpor  % ymm6, % ymm7, % ymm6

# qhasm: mem256[ input_0 + 128 ] = x0
# asm 1: vmovupd   <x0=reg256#10,128(<input_0=int64#1)
# asm 2: vmovupd   <x0=%ymm9,128(<input_0=%rdi)
vmovupd   % ymm9, 128( % rdi)

# qhasm: mem256[ input_0 + 384 ] = x1
# asm 1: vmovupd   <x1=reg256#14,384(<input_0=int64#1)
# asm 2: vmovupd   <x1=%ymm13,384(<input_0=%rdi)
vmovupd   % ymm13, 384( % rdi)

# qhasm: mem256[ input_0 + 640 ] = x2
# asm 1: vmovupd   <x2=reg256#15,640(<input_0=int64#1)
# asm 2: vmovupd   <x2=%ymm14,640(<input_0=%rdi)
vmovupd   % ymm14, 640( % rdi)

# qhasm: mem256[ input_0 + 896 ] = x3
# asm 1: vmovupd   <x3=reg256#11,896(<input_0=int64#1)
# asm 2: vmovupd   <x3=%ymm10,896(<input_0=%rdi)
vmovupd   % ymm10, 896( % rdi)

# qhasm: mem256[ input_0 + 1152 ] = x4
# asm 1: vmovupd   <x4=reg256#12,1152(<input_0=int64#1)
# asm 2: vmovupd   <x4=%ymm11,1152(<input_0=%rdi)
vmovupd   % ymm11, 1152( % rdi)

# qhasm: mem256[ input_0 + 1408 ] = x5
# asm 1: vmovupd   <x5=reg256#9,1408(<input_0=int64#1)
# asm 2: vmovupd   <x5=%ymm8,1408(<input_0=%rdi)
vmovupd   % ymm8, 1408( % rdi)

# qhasm: mem256[ input_0 + 1664 ] = x6
# asm 1: vmovupd   <x6=reg256#13,1664(<input_0=int64#1)
# asm 2: vmovupd   <x6=%ymm12,1664(<input_0=%rdi)
vmovupd   % ymm12, 1664( % rdi)

# qhasm: mem256[ input_0 + 1920 ] = x7
# asm 1: vmovupd   <x7=reg256#7,1920(<input_0=int64#1)
# asm 2: vmovupd   <x7=%ymm6,1920(<input_0=%rdi)
vmovupd   % ymm6, 1920( % rdi)

# qhasm: x0 = mem256[ input_0 + 160 ]
# asm 1: vmovupd   160(<input_0=int64#1),>x0=reg256#7
# asm 2: vmovupd   160(<input_0=%rdi),>x0=%ymm6
vmovupd   160( % rdi), % ymm6

# qhasm: x1 = mem256[ input_0 + 416 ]
# asm 1: vmovupd   416(<input_0=int64#1),>x1=reg256#8
# asm 2: vmovupd   416(<input_0=%rdi),>x1=%ymm7
vmovupd   416( % rdi), % ymm7

# qhasm: x2 = mem256[ input_0 + 672 ]
# asm 1: vmovupd   672(<input_0=int64#1),>x2=reg256#9
# asm 2: vmovupd   672(<input_0=%rdi),>x2=%ymm8
vmovupd   672( % rdi), % ymm8

# qhasm: x3 = mem256[ input_0 + 928 ]
# asm 1: vmovupd   928(<input_0=int64#1),>x3=reg256#10
# asm 2: vmovupd   928(<input_0=%rdi),>x3=%ymm9
vmovupd   928( % rdi), % ymm9

# qhasm: x4 = mem256[ input_0 + 1184 ]
# asm 1: vmovupd   1184(<input_0=int64#1),>x4=reg256#11
# asm 2: vmovupd   1184(<input_0=%rdi),>x4=%ymm10
vmovupd   1184( % rdi), % ymm10

# qhasm: x5 = mem256[ input_0 + 1440 ]
# asm 1: vmovupd   1440(<input_0=int64#1),>x5=reg256#12
# asm 2: vmovupd   1440(<input_0=%rdi),>x5=%ymm11
vmovupd   1440( % rdi), % ymm11

# qhasm: x6 = mem256[ input_0 + 1696 ]
# asm 1: vmovupd   1696(<input_0=int64#1),>x6=reg256#13
# asm 2: vmovupd   1696(<input_0=%rdi),>x6=%ymm12
vmovupd   1696( % rdi), % ymm12

# qhasm: x7 = mem256[ input_0 + 1952 ]
# asm 1: vmovupd   1952(<input_0=int64#1),>x7=reg256#14
# asm 2: vmovupd   1952(<input_0=%rdi),>x7=%ymm13
vmovupd   1952( % rdi), % ymm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <x0=reg256#7,<mask0=reg256#1,>v00=reg256#15
# asm 2: vpand <x0=%ymm6,<mask0=%ymm0,>v00=%ymm14
vpand % ymm6, % ymm0, % ymm14

# qhasm: 4x v10 = x4 << 32
# asm 1: vpsllq $32,<x4=reg256#11,>v10=reg256#16
# asm 2: vpsllq $32,<x4=%ymm10,>v10=%ymm15
vpsllq $32, % ymm10, % ymm15

# qhasm: 4x v01 = x0 unsigned>> 32
# asm 1: vpsrlq $32,<x0=reg256#7,>v01=reg256#7
# asm 2: vpsrlq $32,<x0=%ymm6,>v01=%ymm6
vpsrlq $32, % ymm6, % ymm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <x4=reg256#11,<mask1=reg256#2,>v11=reg256#11
# asm 2: vpand <x4=%ymm10,<mask1=%ymm1,>v11=%ymm10
vpand % ymm10, % ymm1, % ymm10

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x0=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x0=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x4 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#11,>x4=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm10,>x4=%ymm6
vpor  % ymm6, % ymm10, % ymm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <x1=reg256#8,<mask0=reg256#1,>v00=reg256#11
# asm 2: vpand <x1=%ymm7,<mask0=%ymm0,>v00=%ymm10
vpand % ymm7, % ymm0, % ymm10

# qhasm: 4x v10 = x5 << 32
# asm 1: vpsllq $32,<x5=reg256#12,>v10=reg256#16
# asm 2: vpsllq $32,<x5=%ymm11,>v10=%ymm15
vpsllq $32, % ymm11, % ymm15

# qhasm: 4x v01 = x1 unsigned>> 32
# asm 1: vpsrlq $32,<x1=reg256#8,>v01=reg256#8
# asm 2: vpsrlq $32,<x1=%ymm7,>v01=%ymm7
vpsrlq $32, % ymm7, % ymm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <x5=reg256#12,<mask1=reg256#2,>v11=reg256#12
# asm 2: vpand <x5=%ymm11,<mask1=%ymm1,>v11=%ymm11
vpand % ymm11, % ymm1, % ymm11

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#16,>x1=reg256#11
# asm 2: vpor  <v00=%ymm10,<v10=%ymm15,>x1=%ymm10
vpor  % ymm10, % ymm15, % ymm10

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#12,>x5=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm11,>x5=%ymm7
vpor  % ymm7, % ymm11, % ymm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <x2=reg256#9,<mask0=reg256#1,>v00=reg256#12
# asm 2: vpand <x2=%ymm8,<mask0=%ymm0,>v00=%ymm11
vpand % ymm8, % ymm0, % ymm11

# qhasm: 4x v10 = x6 << 32
# asm 1: vpsllq $32,<x6=reg256#13,>v10=reg256#16
# asm 2: vpsllq $32,<x6=%ymm12,>v10=%ymm15
vpsllq $32, % ymm12, % ymm15

# qhasm: 4x v01 = x2 unsigned>> 32
# asm 1: vpsrlq $32,<x2=reg256#9,>v01=reg256#9
# asm 2: vpsrlq $32,<x2=%ymm8,>v01=%ymm8
vpsrlq $32, % ymm8, % ymm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <x6=reg256#13,<mask1=reg256#2,>v11=reg256#13
# asm 2: vpand <x6=%ymm12,<mask1=%ymm1,>v11=%ymm12
vpand % ymm12, % ymm1, % ymm12

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x2=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x2=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#9,<v11=reg256#13,>x6=reg256#9
# asm 2: vpor  <v01=%ymm8,<v11=%ymm12,>x6=%ymm8
vpor  % ymm8, % ymm12, % ymm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <x3=reg256#10,<mask0=reg256#1,>v00=reg256#13
# asm 2: vpand <x3=%ymm9,<mask0=%ymm0,>v00=%ymm12
vpand % ymm9, % ymm0, % ymm12

# qhasm: 4x v10 = x7 << 32
# asm 1: vpsllq $32,<x7=reg256#14,>v10=reg256#16
# asm 2: vpsllq $32,<x7=%ymm13,>v10=%ymm15
vpsllq $32, % ymm13, % ymm15

# qhasm: 4x v01 = x3 unsigned>> 32
# asm 1: vpsrlq $32,<x3=reg256#10,>v01=reg256#10
# asm 2: vpsrlq $32,<x3=%ymm9,>v01=%ymm9
vpsrlq $32, % ymm9, % ymm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <x7=reg256#14,<mask1=reg256#2,>v11=reg256#14
# asm 2: vpand <x7=%ymm13,<mask1=%ymm1,>v11=%ymm13
vpand % ymm13, % ymm1, % ymm13

# qhasm: x3 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x3=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x3=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#10,<v11=reg256#14,>x7=reg256#10
# asm 2: vpor  <v01=%ymm9,<v11=%ymm13,>x7=%ymm9
vpor  % ymm9, % ymm13, % ymm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <x0=reg256#15,<mask2=reg256#3,>v00=reg256#14
# asm 2: vpand <x0=%ymm14,<mask2=%ymm2,>v00=%ymm13
vpand % ymm14, % ymm2, % ymm13

# qhasm: 8x v10 = x2 << 16
# asm 1: vpslld $16,<x2=reg256#12,>v10=reg256#16
# asm 2: vpslld $16,<x2=%ymm11,>v10=%ymm15
vpslld $16, % ymm11, % ymm15

# qhasm: 8x v01 = x0 unsigned>> 16
# asm 1: vpsrld $16,<x0=reg256#15,>v01=reg256#15
# asm 2: vpsrld $16,<x0=%ymm14,>v01=%ymm14
vpsrld $16, % ymm14, % ymm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <x2=reg256#12,<mask3=reg256#4,>v11=reg256#12
# asm 2: vpand <x2=%ymm11,<mask3=%ymm3,>v11=%ymm11
vpand % ymm11, % ymm3, % ymm11

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#14,<v10=reg256#16,>x0=reg256#14
# asm 2: vpor  <v00=%ymm13,<v10=%ymm15,>x0=%ymm13
vpor  % ymm13, % ymm15, % ymm13

# qhasm: x2 = v01 | v11
# asm 1: vpor  <v01=reg256#15,<v11=reg256#12,>x2=reg256#12
# asm 2: vpor  <v01=%ymm14,<v11=%ymm11,>x2=%ymm11
vpor  % ymm14, % ymm11, % ymm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <x1=reg256#11,<mask2=reg256#3,>v00=reg256#15
# asm 2: vpand <x1=%ymm10,<mask2=%ymm2,>v00=%ymm14
vpand % ymm10, % ymm2, % ymm14

# qhasm: 8x v10 = x3 << 16
# asm 1: vpslld $16,<x3=reg256#13,>v10=reg256#16
# asm 2: vpslld $16,<x3=%ymm12,>v10=%ymm15
vpslld $16, % ymm12, % ymm15

# qhasm: 8x v01 = x1 unsigned>> 16
# asm 1: vpsrld $16,<x1=reg256#11,>v01=reg256#11
# asm 2: vpsrld $16,<x1=%ymm10,>v01=%ymm10
vpsrld $16, % ymm10, % ymm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <x3=reg256#13,<mask3=reg256#4,>v11=reg256#13
# asm 2: vpand <x3=%ymm12,<mask3=%ymm3,>v11=%ymm12
vpand % ymm12, % ymm3, % ymm12

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x1=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x1=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#11,<v11=reg256#13,>x3=reg256#11
# asm 2: vpor  <v01=%ymm10,<v11=%ymm12,>x3=%ymm10
vpor  % ymm10, % ymm12, % ymm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <x4=reg256#7,<mask2=reg256#3,>v00=reg256#13
# asm 2: vpand <x4=%ymm6,<mask2=%ymm2,>v00=%ymm12
vpand % ymm6, % ymm2, % ymm12

# qhasm: 8x v10 = x6 << 16
# asm 1: vpslld $16,<x6=reg256#9,>v10=reg256#16
# asm 2: vpslld $16,<x6=%ymm8,>v10=%ymm15
vpslld $16, % ymm8, % ymm15

# qhasm: 8x v01 = x4 unsigned>> 16
# asm 1: vpsrld $16,<x4=reg256#7,>v01=reg256#7
# asm 2: vpsrld $16,<x4=%ymm6,>v01=%ymm6
vpsrld $16, % ymm6, % ymm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <x6=reg256#9,<mask3=reg256#4,>v11=reg256#9
# asm 2: vpand <x6=%ymm8,<mask3=%ymm3,>v11=%ymm8
vpand % ymm8, % ymm3, % ymm8

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x4=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x4=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#9,>x6=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm8,>x6=%ymm6
vpor  % ymm6, % ymm8, % ymm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <x5=reg256#8,<mask2=reg256#3,>v00=reg256#9
# asm 2: vpand <x5=%ymm7,<mask2=%ymm2,>v00=%ymm8
vpand % ymm7, % ymm2, % ymm8

# qhasm: 8x v10 = x7 << 16
# asm 1: vpslld $16,<x7=reg256#10,>v10=reg256#16
# asm 2: vpslld $16,<x7=%ymm9,>v10=%ymm15
vpslld $16, % ymm9, % ymm15

# qhasm: 8x v01 = x5 unsigned>> 16
# asm 1: vpsrld $16,<x5=reg256#8,>v01=reg256#8
# asm 2: vpsrld $16,<x5=%ymm7,>v01=%ymm7
vpsrld $16, % ymm7, % ymm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <x7=reg256#10,<mask3=reg256#4,>v11=reg256#10
# asm 2: vpand <x7=%ymm9,<mask3=%ymm3,>v11=%ymm9
vpand % ymm9, % ymm3, % ymm9

# qhasm: x5 = v00 | v10
# asm 1: vpor  <v00=reg256#9,<v10=reg256#16,>x5=reg256#9
# asm 2: vpor  <v00=%ymm8,<v10=%ymm15,>x5=%ymm8
vpor  % ymm8, % ymm15, % ymm8

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#10,>x7=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm9,>x7=%ymm7
vpor  % ymm7, % ymm9, % ymm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <x0=reg256#14,<mask4=reg256#5,>v00=reg256#10
# asm 2: vpand <x0=%ymm13,<mask4=%ymm4,>v00=%ymm9
vpand % ymm13, % ymm4, % ymm9

# qhasm: 16x v10 = x1 << 8
# asm 1: vpsllw $8,<x1=reg256#15,>v10=reg256#16
# asm 2: vpsllw $8,<x1=%ymm14,>v10=%ymm15
vpsllw $8, % ymm14, % ymm15

# qhasm: 16x v01 = x0 unsigned>> 8
# asm 1: vpsrlw $8,<x0=reg256#14,>v01=reg256#14
# asm 2: vpsrlw $8,<x0=%ymm13,>v01=%ymm13
vpsrlw $8, % ymm13, % ymm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <x1=reg256#15,<mask5=reg256#6,>v11=reg256#15
# asm 2: vpand <x1=%ymm14,<mask5=%ymm5,>v11=%ymm14
vpand % ymm14, % ymm5, % ymm14

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#10,<v10=reg256#16,>x0=reg256#10
# asm 2: vpor  <v00=%ymm9,<v10=%ymm15,>x0=%ymm9
vpor  % ymm9, % ymm15, % ymm9

# qhasm: x1 = v01 | v11
# asm 1: vpor  <v01=reg256#14,<v11=reg256#15,>x1=reg256#14
# asm 2: vpor  <v01=%ymm13,<v11=%ymm14,>x1=%ymm13
vpor  % ymm13, % ymm14, % ymm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <x2=reg256#12,<mask4=reg256#5,>v00=reg256#15
# asm 2: vpand <x2=%ymm11,<mask4=%ymm4,>v00=%ymm14
vpand % ymm11, % ymm4, % ymm14

# qhasm: 16x v10 = x3 << 8
# asm 1: vpsllw $8,<x3=reg256#11,>v10=reg256#16
# asm 2: vpsllw $8,<x3=%ymm10,>v10=%ymm15
vpsllw $8, % ymm10, % ymm15

# qhasm: 16x v01 = x2 unsigned>> 8
# asm 1: vpsrlw $8,<x2=reg256#12,>v01=reg256#12
# asm 2: vpsrlw $8,<x2=%ymm11,>v01=%ymm11
vpsrlw $8, % ymm11, % ymm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <x3=reg256#11,<mask5=reg256#6,>v11=reg256#11
# asm 2: vpand <x3=%ymm10,<mask5=%ymm5,>v11=%ymm10
vpand % ymm10, % ymm5, % ymm10

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x2=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x2=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#12,<v11=reg256#11,>x3=reg256#11
# asm 2: vpor  <v01=%ymm11,<v11=%ymm10,>x3=%ymm10
vpor  % ymm11, % ymm10, % ymm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <x4=reg256#13,<mask4=reg256#5,>v00=reg256#12
# asm 2: vpand <x4=%ymm12,<mask4=%ymm4,>v00=%ymm11
vpand % ymm12, % ymm4, % ymm11

# qhasm: 16x v10 = x5 << 8
# asm 1: vpsllw $8,<x5=reg256#9,>v10=reg256#16
# asm 2: vpsllw $8,<x5=%ymm8,>v10=%ymm15
vpsllw $8, % ymm8, % ymm15

# qhasm: 16x v01 = x4 unsigned>> 8
# asm 1: vpsrlw $8,<x4=reg256#13,>v01=reg256#13
# asm 2: vpsrlw $8,<x4=%ymm12,>v01=%ymm12
vpsrlw $8, % ymm12, % ymm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <x5=reg256#9,<mask5=reg256#6,>v11=reg256#9
# asm 2: vpand <x5=%ymm8,<mask5=%ymm5,>v11=%ymm8
vpand % ymm8, % ymm5, % ymm8

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x4=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x4=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#13,<v11=reg256#9,>x5=reg256#9
# asm 2: vpor  <v01=%ymm12,<v11=%ymm8,>x5=%ymm8
vpor  % ymm12, % ymm8, % ymm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <x6=reg256#7,<mask4=reg256#5,>v00=reg256#13
# asm 2: vpand <x6=%ymm6,<mask4=%ymm4,>v00=%ymm12
vpand % ymm6, % ymm4, % ymm12

# qhasm: 16x v10 = x7 << 8
# asm 1: vpsllw $8,<x7=reg256#8,>v10=reg256#16
# asm 2: vpsllw $8,<x7=%ymm7,>v10=%ymm15
vpsllw $8, % ymm7, % ymm15

# qhasm: 16x v01 = x6 unsigned>> 8
# asm 1: vpsrlw $8,<x6=reg256#7,>v01=reg256#7
# asm 2: vpsrlw $8,<x6=%ymm6,>v01=%ymm6
vpsrlw $8, % ymm6, % ymm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <x7=reg256#8,<mask5=reg256#6,>v11=reg256#8
# asm 2: vpand <x7=%ymm7,<mask5=%ymm5,>v11=%ymm7
vpand % ymm7, % ymm5, % ymm7

# qhasm: x6 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x6=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x6=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#8,>x7=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm7,>x7=%ymm6
vpor  % ymm6, % ymm7, % ymm6

# qhasm: mem256[ input_0 + 160 ] = x0
# asm 1: vmovupd   <x0=reg256#10,160(<input_0=int64#1)
# asm 2: vmovupd   <x0=%ymm9,160(<input_0=%rdi)
vmovupd   % ymm9, 160( % rdi)

# qhasm: mem256[ input_0 + 416 ] = x1
# asm 1: vmovupd   <x1=reg256#14,416(<input_0=int64#1)
# asm 2: vmovupd   <x1=%ymm13,416(<input_0=%rdi)
vmovupd   % ymm13, 416( % rdi)

# qhasm: mem256[ input_0 + 672 ] = x2
# asm 1: vmovupd   <x2=reg256#15,672(<input_0=int64#1)
# asm 2: vmovupd   <x2=%ymm14,672(<input_0=%rdi)
vmovupd   % ymm14, 672( % rdi)

# qhasm: mem256[ input_0 + 928 ] = x3
# asm 1: vmovupd   <x3=reg256#11,928(<input_0=int64#1)
# asm 2: vmovupd   <x3=%ymm10,928(<input_0=%rdi)
vmovupd   % ymm10, 928( % rdi)

# qhasm: mem256[ input_0 + 1184 ] = x4
# asm 1: vmovupd   <x4=reg256#12,1184(<input_0=int64#1)
# asm 2: vmovupd   <x4=%ymm11,1184(<input_0=%rdi)
vmovupd   % ymm11, 1184( % rdi)

# qhasm: mem256[ input_0 + 1440 ] = x5
# asm 1: vmovupd   <x5=reg256#9,1440(<input_0=int64#1)
# asm 2: vmovupd   <x5=%ymm8,1440(<input_0=%rdi)
vmovupd   % ymm8, 1440( % rdi)

# qhasm: mem256[ input_0 + 1696 ] = x6
# asm 1: vmovupd   <x6=reg256#13,1696(<input_0=int64#1)
# asm 2: vmovupd   <x6=%ymm12,1696(<input_0=%rdi)
vmovupd   % ymm12, 1696( % rdi)

# qhasm: mem256[ input_0 + 1952 ] = x7
# asm 1: vmovupd   <x7=reg256#7,1952(<input_0=int64#1)
# asm 2: vmovupd   <x7=%ymm6,1952(<input_0=%rdi)
vmovupd   % ymm6, 1952( % rdi)

# qhasm: x0 = mem256[ input_0 + 192 ]
# asm 1: vmovupd   192(<input_0=int64#1),>x0=reg256#7
# asm 2: vmovupd   192(<input_0=%rdi),>x0=%ymm6
vmovupd   192( % rdi), % ymm6

# qhasm: x1 = mem256[ input_0 + 448 ]
# asm 1: vmovupd   448(<input_0=int64#1),>x1=reg256#8
# asm 2: vmovupd   448(<input_0=%rdi),>x1=%ymm7
vmovupd   448( % rdi), % ymm7

# qhasm: x2 = mem256[ input_0 + 704 ]
# asm 1: vmovupd   704(<input_0=int64#1),>x2=reg256#9
# asm 2: vmovupd   704(<input_0=%rdi),>x2=%ymm8
vmovupd   704( % rdi), % ymm8

# qhasm: x3 = mem256[ input_0 + 960 ]
# asm 1: vmovupd   960(<input_0=int64#1),>x3=reg256#10
# asm 2: vmovupd   960(<input_0=%rdi),>x3=%ymm9
vmovupd   960( % rdi), % ymm9

# qhasm: x4 = mem256[ input_0 + 1216 ]
# asm 1: vmovupd   1216(<input_0=int64#1),>x4=reg256#11
# asm 2: vmovupd   1216(<input_0=%rdi),>x4=%ymm10
vmovupd   1216( % rdi), % ymm10

# qhasm: x5 = mem256[ input_0 + 1472 ]
# asm 1: vmovupd   1472(<input_0=int64#1),>x5=reg256#12
# asm 2: vmovupd   1472(<input_0=%rdi),>x5=%ymm11
vmovupd   1472( % rdi), % ymm11

# qhasm: x6 = mem256[ input_0 + 1728 ]
# asm 1: vmovupd   1728(<input_0=int64#1),>x6=reg256#13
# asm 2: vmovupd   1728(<input_0=%rdi),>x6=%ymm12
vmovupd   1728( % rdi), % ymm12

# qhasm: x7 = mem256[ input_0 + 1984 ]
# asm 1: vmovupd   1984(<input_0=int64#1),>x7=reg256#14
# asm 2: vmovupd   1984(<input_0=%rdi),>x7=%ymm13
vmovupd   1984( % rdi), % ymm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <x0=reg256#7,<mask0=reg256#1,>v00=reg256#15
# asm 2: vpand <x0=%ymm6,<mask0=%ymm0,>v00=%ymm14
vpand % ymm6, % ymm0, % ymm14

# qhasm: 4x v10 = x4 << 32
# asm 1: vpsllq $32,<x4=reg256#11,>v10=reg256#16
# asm 2: vpsllq $32,<x4=%ymm10,>v10=%ymm15
vpsllq $32, % ymm10, % ymm15

# qhasm: 4x v01 = x0 unsigned>> 32
# asm 1: vpsrlq $32,<x0=reg256#7,>v01=reg256#7
# asm 2: vpsrlq $32,<x0=%ymm6,>v01=%ymm6
vpsrlq $32, % ymm6, % ymm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <x4=reg256#11,<mask1=reg256#2,>v11=reg256#11
# asm 2: vpand <x4=%ymm10,<mask1=%ymm1,>v11=%ymm10
vpand % ymm10, % ymm1, % ymm10

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x0=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x0=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x4 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#11,>x4=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm10,>x4=%ymm6
vpor  % ymm6, % ymm10, % ymm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <x1=reg256#8,<mask0=reg256#1,>v00=reg256#11
# asm 2: vpand <x1=%ymm7,<mask0=%ymm0,>v00=%ymm10
vpand % ymm7, % ymm0, % ymm10

# qhasm: 4x v10 = x5 << 32
# asm 1: vpsllq $32,<x5=reg256#12,>v10=reg256#16
# asm 2: vpsllq $32,<x5=%ymm11,>v10=%ymm15
vpsllq $32, % ymm11, % ymm15

# qhasm: 4x v01 = x1 unsigned>> 32
# asm 1: vpsrlq $32,<x1=reg256#8,>v01=reg256#8
# asm 2: vpsrlq $32,<x1=%ymm7,>v01=%ymm7
vpsrlq $32, % ymm7, % ymm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <x5=reg256#12,<mask1=reg256#2,>v11=reg256#12
# asm 2: vpand <x5=%ymm11,<mask1=%ymm1,>v11=%ymm11
vpand % ymm11, % ymm1, % ymm11

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#16,>x1=reg256#11
# asm 2: vpor  <v00=%ymm10,<v10=%ymm15,>x1=%ymm10
vpor  % ymm10, % ymm15, % ymm10

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#12,>x5=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm11,>x5=%ymm7
vpor  % ymm7, % ymm11, % ymm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <x2=reg256#9,<mask0=reg256#1,>v00=reg256#12
# asm 2: vpand <x2=%ymm8,<mask0=%ymm0,>v00=%ymm11
vpand % ymm8, % ymm0, % ymm11

# qhasm: 4x v10 = x6 << 32
# asm 1: vpsllq $32,<x6=reg256#13,>v10=reg256#16
# asm 2: vpsllq $32,<x6=%ymm12,>v10=%ymm15
vpsllq $32, % ymm12, % ymm15

# qhasm: 4x v01 = x2 unsigned>> 32
# asm 1: vpsrlq $32,<x2=reg256#9,>v01=reg256#9
# asm 2: vpsrlq $32,<x2=%ymm8,>v01=%ymm8
vpsrlq $32, % ymm8, % ymm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <x6=reg256#13,<mask1=reg256#2,>v11=reg256#13
# asm 2: vpand <x6=%ymm12,<mask1=%ymm1,>v11=%ymm12
vpand % ymm12, % ymm1, % ymm12

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x2=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x2=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#9,<v11=reg256#13,>x6=reg256#9
# asm 2: vpor  <v01=%ymm8,<v11=%ymm12,>x6=%ymm8
vpor  % ymm8, % ymm12, % ymm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <x3=reg256#10,<mask0=reg256#1,>v00=reg256#13
# asm 2: vpand <x3=%ymm9,<mask0=%ymm0,>v00=%ymm12
vpand % ymm9, % ymm0, % ymm12

# qhasm: 4x v10 = x7 << 32
# asm 1: vpsllq $32,<x7=reg256#14,>v10=reg256#16
# asm 2: vpsllq $32,<x7=%ymm13,>v10=%ymm15
vpsllq $32, % ymm13, % ymm15

# qhasm: 4x v01 = x3 unsigned>> 32
# asm 1: vpsrlq $32,<x3=reg256#10,>v01=reg256#10
# asm 2: vpsrlq $32,<x3=%ymm9,>v01=%ymm9
vpsrlq $32, % ymm9, % ymm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <x7=reg256#14,<mask1=reg256#2,>v11=reg256#14
# asm 2: vpand <x7=%ymm13,<mask1=%ymm1,>v11=%ymm13
vpand % ymm13, % ymm1, % ymm13

# qhasm: x3 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x3=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x3=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#10,<v11=reg256#14,>x7=reg256#10
# asm 2: vpor  <v01=%ymm9,<v11=%ymm13,>x7=%ymm9
vpor  % ymm9, % ymm13, % ymm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <x0=reg256#15,<mask2=reg256#3,>v00=reg256#14
# asm 2: vpand <x0=%ymm14,<mask2=%ymm2,>v00=%ymm13
vpand % ymm14, % ymm2, % ymm13

# qhasm: 8x v10 = x2 << 16
# asm 1: vpslld $16,<x2=reg256#12,>v10=reg256#16
# asm 2: vpslld $16,<x2=%ymm11,>v10=%ymm15
vpslld $16, % ymm11, % ymm15

# qhasm: 8x v01 = x0 unsigned>> 16
# asm 1: vpsrld $16,<x0=reg256#15,>v01=reg256#15
# asm 2: vpsrld $16,<x0=%ymm14,>v01=%ymm14
vpsrld $16, % ymm14, % ymm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <x2=reg256#12,<mask3=reg256#4,>v11=reg256#12
# asm 2: vpand <x2=%ymm11,<mask3=%ymm3,>v11=%ymm11
vpand % ymm11, % ymm3, % ymm11

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#14,<v10=reg256#16,>x0=reg256#14
# asm 2: vpor  <v00=%ymm13,<v10=%ymm15,>x0=%ymm13
vpor  % ymm13, % ymm15, % ymm13

# qhasm: x2 = v01 | v11
# asm 1: vpor  <v01=reg256#15,<v11=reg256#12,>x2=reg256#12
# asm 2: vpor  <v01=%ymm14,<v11=%ymm11,>x2=%ymm11
vpor  % ymm14, % ymm11, % ymm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <x1=reg256#11,<mask2=reg256#3,>v00=reg256#15
# asm 2: vpand <x1=%ymm10,<mask2=%ymm2,>v00=%ymm14
vpand % ymm10, % ymm2, % ymm14

# qhasm: 8x v10 = x3 << 16
# asm 1: vpslld $16,<x3=reg256#13,>v10=reg256#16
# asm 2: vpslld $16,<x3=%ymm12,>v10=%ymm15
vpslld $16, % ymm12, % ymm15

# qhasm: 8x v01 = x1 unsigned>> 16
# asm 1: vpsrld $16,<x1=reg256#11,>v01=reg256#11
# asm 2: vpsrld $16,<x1=%ymm10,>v01=%ymm10
vpsrld $16, % ymm10, % ymm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <x3=reg256#13,<mask3=reg256#4,>v11=reg256#13
# asm 2: vpand <x3=%ymm12,<mask3=%ymm3,>v11=%ymm12
vpand % ymm12, % ymm3, % ymm12

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x1=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x1=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#11,<v11=reg256#13,>x3=reg256#11
# asm 2: vpor  <v01=%ymm10,<v11=%ymm12,>x3=%ymm10
vpor  % ymm10, % ymm12, % ymm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <x4=reg256#7,<mask2=reg256#3,>v00=reg256#13
# asm 2: vpand <x4=%ymm6,<mask2=%ymm2,>v00=%ymm12
vpand % ymm6, % ymm2, % ymm12

# qhasm: 8x v10 = x6 << 16
# asm 1: vpslld $16,<x6=reg256#9,>v10=reg256#16
# asm 2: vpslld $16,<x6=%ymm8,>v10=%ymm15
vpslld $16, % ymm8, % ymm15

# qhasm: 8x v01 = x4 unsigned>> 16
# asm 1: vpsrld $16,<x4=reg256#7,>v01=reg256#7
# asm 2: vpsrld $16,<x4=%ymm6,>v01=%ymm6
vpsrld $16, % ymm6, % ymm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <x6=reg256#9,<mask3=reg256#4,>v11=reg256#9
# asm 2: vpand <x6=%ymm8,<mask3=%ymm3,>v11=%ymm8
vpand % ymm8, % ymm3, % ymm8

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x4=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x4=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#9,>x6=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm8,>x6=%ymm6
vpor  % ymm6, % ymm8, % ymm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <x5=reg256#8,<mask2=reg256#3,>v00=reg256#9
# asm 2: vpand <x5=%ymm7,<mask2=%ymm2,>v00=%ymm8
vpand % ymm7, % ymm2, % ymm8

# qhasm: 8x v10 = x7 << 16
# asm 1: vpslld $16,<x7=reg256#10,>v10=reg256#16
# asm 2: vpslld $16,<x7=%ymm9,>v10=%ymm15
vpslld $16, % ymm9, % ymm15

# qhasm: 8x v01 = x5 unsigned>> 16
# asm 1: vpsrld $16,<x5=reg256#8,>v01=reg256#8
# asm 2: vpsrld $16,<x5=%ymm7,>v01=%ymm7
vpsrld $16, % ymm7, % ymm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <x7=reg256#10,<mask3=reg256#4,>v11=reg256#10
# asm 2: vpand <x7=%ymm9,<mask3=%ymm3,>v11=%ymm9
vpand % ymm9, % ymm3, % ymm9

# qhasm: x5 = v00 | v10
# asm 1: vpor  <v00=reg256#9,<v10=reg256#16,>x5=reg256#9
# asm 2: vpor  <v00=%ymm8,<v10=%ymm15,>x5=%ymm8
vpor  % ymm8, % ymm15, % ymm8

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#10,>x7=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm9,>x7=%ymm7
vpor  % ymm7, % ymm9, % ymm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <x0=reg256#14,<mask4=reg256#5,>v00=reg256#10
# asm 2: vpand <x0=%ymm13,<mask4=%ymm4,>v00=%ymm9
vpand % ymm13, % ymm4, % ymm9

# qhasm: 16x v10 = x1 << 8
# asm 1: vpsllw $8,<x1=reg256#15,>v10=reg256#16
# asm 2: vpsllw $8,<x1=%ymm14,>v10=%ymm15
vpsllw $8, % ymm14, % ymm15

# qhasm: 16x v01 = x0 unsigned>> 8
# asm 1: vpsrlw $8,<x0=reg256#14,>v01=reg256#14
# asm 2: vpsrlw $8,<x0=%ymm13,>v01=%ymm13
vpsrlw $8, % ymm13, % ymm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <x1=reg256#15,<mask5=reg256#6,>v11=reg256#15
# asm 2: vpand <x1=%ymm14,<mask5=%ymm5,>v11=%ymm14
vpand % ymm14, % ymm5, % ymm14

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#10,<v10=reg256#16,>x0=reg256#10
# asm 2: vpor  <v00=%ymm9,<v10=%ymm15,>x0=%ymm9
vpor  % ymm9, % ymm15, % ymm9

# qhasm: x1 = v01 | v11
# asm 1: vpor  <v01=reg256#14,<v11=reg256#15,>x1=reg256#14
# asm 2: vpor  <v01=%ymm13,<v11=%ymm14,>x1=%ymm13
vpor  % ymm13, % ymm14, % ymm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <x2=reg256#12,<mask4=reg256#5,>v00=reg256#15
# asm 2: vpand <x2=%ymm11,<mask4=%ymm4,>v00=%ymm14
vpand % ymm11, % ymm4, % ymm14

# qhasm: 16x v10 = x3 << 8
# asm 1: vpsllw $8,<x3=reg256#11,>v10=reg256#16
# asm 2: vpsllw $8,<x3=%ymm10,>v10=%ymm15
vpsllw $8, % ymm10, % ymm15

# qhasm: 16x v01 = x2 unsigned>> 8
# asm 1: vpsrlw $8,<x2=reg256#12,>v01=reg256#12
# asm 2: vpsrlw $8,<x2=%ymm11,>v01=%ymm11
vpsrlw $8, % ymm11, % ymm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <x3=reg256#11,<mask5=reg256#6,>v11=reg256#11
# asm 2: vpand <x3=%ymm10,<mask5=%ymm5,>v11=%ymm10
vpand % ymm10, % ymm5, % ymm10

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x2=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x2=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#12,<v11=reg256#11,>x3=reg256#11
# asm 2: vpor  <v01=%ymm11,<v11=%ymm10,>x3=%ymm10
vpor  % ymm11, % ymm10, % ymm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <x4=reg256#13,<mask4=reg256#5,>v00=reg256#12
# asm 2: vpand <x4=%ymm12,<mask4=%ymm4,>v00=%ymm11
vpand % ymm12, % ymm4, % ymm11

# qhasm: 16x v10 = x5 << 8
# asm 1: vpsllw $8,<x5=reg256#9,>v10=reg256#16
# asm 2: vpsllw $8,<x5=%ymm8,>v10=%ymm15
vpsllw $8, % ymm8, % ymm15

# qhasm: 16x v01 = x4 unsigned>> 8
# asm 1: vpsrlw $8,<x4=reg256#13,>v01=reg256#13
# asm 2: vpsrlw $8,<x4=%ymm12,>v01=%ymm12
vpsrlw $8, % ymm12, % ymm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <x5=reg256#9,<mask5=reg256#6,>v11=reg256#9
# asm 2: vpand <x5=%ymm8,<mask5=%ymm5,>v11=%ymm8
vpand % ymm8, % ymm5, % ymm8

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x4=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x4=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#13,<v11=reg256#9,>x5=reg256#9
# asm 2: vpor  <v01=%ymm12,<v11=%ymm8,>x5=%ymm8
vpor  % ymm12, % ymm8, % ymm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <x6=reg256#7,<mask4=reg256#5,>v00=reg256#13
# asm 2: vpand <x6=%ymm6,<mask4=%ymm4,>v00=%ymm12
vpand % ymm6, % ymm4, % ymm12

# qhasm: 16x v10 = x7 << 8
# asm 1: vpsllw $8,<x7=reg256#8,>v10=reg256#16
# asm 2: vpsllw $8,<x7=%ymm7,>v10=%ymm15
vpsllw $8, % ymm7, % ymm15

# qhasm: 16x v01 = x6 unsigned>> 8
# asm 1: vpsrlw $8,<x6=reg256#7,>v01=reg256#7
# asm 2: vpsrlw $8,<x6=%ymm6,>v01=%ymm6
vpsrlw $8, % ymm6, % ymm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <x7=reg256#8,<mask5=reg256#6,>v11=reg256#8
# asm 2: vpand <x7=%ymm7,<mask5=%ymm5,>v11=%ymm7
vpand % ymm7, % ymm5, % ymm7

# qhasm: x6 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x6=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x6=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#8,>x7=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm7,>x7=%ymm6
vpor  % ymm6, % ymm7, % ymm6

# qhasm: mem256[ input_0 + 192 ] = x0
# asm 1: vmovupd   <x0=reg256#10,192(<input_0=int64#1)
# asm 2: vmovupd   <x0=%ymm9,192(<input_0=%rdi)
vmovupd   % ymm9, 192( % rdi)

# qhasm: mem256[ input_0 + 448 ] = x1
# asm 1: vmovupd   <x1=reg256#14,448(<input_0=int64#1)
# asm 2: vmovupd   <x1=%ymm13,448(<input_0=%rdi)
vmovupd   % ymm13, 448( % rdi)

# qhasm: mem256[ input_0 + 704 ] = x2
# asm 1: vmovupd   <x2=reg256#15,704(<input_0=int64#1)
# asm 2: vmovupd   <x2=%ymm14,704(<input_0=%rdi)
vmovupd   % ymm14, 704( % rdi)

# qhasm: mem256[ input_0 + 960 ] = x3
# asm 1: vmovupd   <x3=reg256#11,960(<input_0=int64#1)
# asm 2: vmovupd   <x3=%ymm10,960(<input_0=%rdi)
vmovupd   % ymm10, 960( % rdi)

# qhasm: mem256[ input_0 + 1216 ] = x4
# asm 1: vmovupd   <x4=reg256#12,1216(<input_0=int64#1)
# asm 2: vmovupd   <x4=%ymm11,1216(<input_0=%rdi)
vmovupd   % ymm11, 1216( % rdi)

# qhasm: mem256[ input_0 + 1472 ] = x5
# asm 1: vmovupd   <x5=reg256#9,1472(<input_0=int64#1)
# asm 2: vmovupd   <x5=%ymm8,1472(<input_0=%rdi)
vmovupd   % ymm8, 1472( % rdi)

# qhasm: mem256[ input_0 + 1728 ] = x6
# asm 1: vmovupd   <x6=reg256#13,1728(<input_0=int64#1)
# asm 2: vmovupd   <x6=%ymm12,1728(<input_0=%rdi)
vmovupd   % ymm12, 1728( % rdi)

# qhasm: mem256[ input_0 + 1984 ] = x7
# asm 1: vmovupd   <x7=reg256#7,1984(<input_0=int64#1)
# asm 2: vmovupd   <x7=%ymm6,1984(<input_0=%rdi)
vmovupd   % ymm6, 1984( % rdi)

# qhasm: x0 = mem256[ input_0 + 224 ]
# asm 1: vmovupd   224(<input_0=int64#1),>x0=reg256#7
# asm 2: vmovupd   224(<input_0=%rdi),>x0=%ymm6
vmovupd   224( % rdi), % ymm6

# qhasm: x1 = mem256[ input_0 + 480 ]
# asm 1: vmovupd   480(<input_0=int64#1),>x1=reg256#8
# asm 2: vmovupd   480(<input_0=%rdi),>x1=%ymm7
vmovupd   480( % rdi), % ymm7

# qhasm: x2 = mem256[ input_0 + 736 ]
# asm 1: vmovupd   736(<input_0=int64#1),>x2=reg256#9
# asm 2: vmovupd   736(<input_0=%rdi),>x2=%ymm8
vmovupd   736( % rdi), % ymm8

# qhasm: x3 = mem256[ input_0 + 992 ]
# asm 1: vmovupd   992(<input_0=int64#1),>x3=reg256#10
# asm 2: vmovupd   992(<input_0=%rdi),>x3=%ymm9
vmovupd   992( % rdi), % ymm9

# qhasm: x4 = mem256[ input_0 + 1248 ]
# asm 1: vmovupd   1248(<input_0=int64#1),>x4=reg256#11
# asm 2: vmovupd   1248(<input_0=%rdi),>x4=%ymm10
vmovupd   1248( % rdi), % ymm10

# qhasm: x5 = mem256[ input_0 + 1504 ]
# asm 1: vmovupd   1504(<input_0=int64#1),>x5=reg256#12
# asm 2: vmovupd   1504(<input_0=%rdi),>x5=%ymm11
vmovupd   1504( % rdi), % ymm11

# qhasm: x6 = mem256[ input_0 + 1760 ]
# asm 1: vmovupd   1760(<input_0=int64#1),>x6=reg256#13
# asm 2: vmovupd   1760(<input_0=%rdi),>x6=%ymm12
vmovupd   1760( % rdi), % ymm12

# qhasm: x7 = mem256[ input_0 + 2016 ]
# asm 1: vmovupd   2016(<input_0=int64#1),>x7=reg256#14
# asm 2: vmovupd   2016(<input_0=%rdi),>x7=%ymm13
vmovupd   2016( % rdi), % ymm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <x0=reg256#7,<mask0=reg256#1,>v00=reg256#15
# asm 2: vpand <x0=%ymm6,<mask0=%ymm0,>v00=%ymm14
vpand % ymm6, % ymm0, % ymm14

# qhasm: 4x v10 = x4 << 32
# asm 1: vpsllq $32,<x4=reg256#11,>v10=reg256#16
# asm 2: vpsllq $32,<x4=%ymm10,>v10=%ymm15
vpsllq $32, % ymm10, % ymm15

# qhasm: 4x v01 = x0 unsigned>> 32
# asm 1: vpsrlq $32,<x0=reg256#7,>v01=reg256#7
# asm 2: vpsrlq $32,<x0=%ymm6,>v01=%ymm6
vpsrlq $32, % ymm6, % ymm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <x4=reg256#11,<mask1=reg256#2,>v11=reg256#11
# asm 2: vpand <x4=%ymm10,<mask1=%ymm1,>v11=%ymm10
vpand % ymm10, % ymm1, % ymm10

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x0=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x0=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x4 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#11,>x4=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm10,>x4=%ymm6
vpor  % ymm6, % ymm10, % ymm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <x1=reg256#8,<mask0=reg256#1,>v00=reg256#11
# asm 2: vpand <x1=%ymm7,<mask0=%ymm0,>v00=%ymm10
vpand % ymm7, % ymm0, % ymm10

# qhasm: 4x v10 = x5 << 32
# asm 1: vpsllq $32,<x5=reg256#12,>v10=reg256#16
# asm 2: vpsllq $32,<x5=%ymm11,>v10=%ymm15
vpsllq $32, % ymm11, % ymm15

# qhasm: 4x v01 = x1 unsigned>> 32
# asm 1: vpsrlq $32,<x1=reg256#8,>v01=reg256#8
# asm 2: vpsrlq $32,<x1=%ymm7,>v01=%ymm7
vpsrlq $32, % ymm7, % ymm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <x5=reg256#12,<mask1=reg256#2,>v11=reg256#12
# asm 2: vpand <x5=%ymm11,<mask1=%ymm1,>v11=%ymm11
vpand % ymm11, % ymm1, % ymm11

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#16,>x1=reg256#11
# asm 2: vpor  <v00=%ymm10,<v10=%ymm15,>x1=%ymm10
vpor  % ymm10, % ymm15, % ymm10

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#12,>x5=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm11,>x5=%ymm7
vpor  % ymm7, % ymm11, % ymm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <x2=reg256#9,<mask0=reg256#1,>v00=reg256#12
# asm 2: vpand <x2=%ymm8,<mask0=%ymm0,>v00=%ymm11
vpand % ymm8, % ymm0, % ymm11

# qhasm: 4x v10 = x6 << 32
# asm 1: vpsllq $32,<x6=reg256#13,>v10=reg256#16
# asm 2: vpsllq $32,<x6=%ymm12,>v10=%ymm15
vpsllq $32, % ymm12, % ymm15

# qhasm: 4x v01 = x2 unsigned>> 32
# asm 1: vpsrlq $32,<x2=reg256#9,>v01=reg256#9
# asm 2: vpsrlq $32,<x2=%ymm8,>v01=%ymm8
vpsrlq $32, % ymm8, % ymm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <x6=reg256#13,<mask1=reg256#2,>v11=reg256#13
# asm 2: vpand <x6=%ymm12,<mask1=%ymm1,>v11=%ymm12
vpand % ymm12, % ymm1, % ymm12

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x2=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x2=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#9,<v11=reg256#13,>x6=reg256#9
# asm 2: vpor  <v01=%ymm8,<v11=%ymm12,>x6=%ymm8
vpor  % ymm8, % ymm12, % ymm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <x3=reg256#10,<mask0=reg256#1,>v00=reg256#1
# asm 2: vpand <x3=%ymm9,<mask0=%ymm0,>v00=%ymm0
vpand % ymm9, % ymm0, % ymm0

# qhasm: 4x v10 = x7 << 32
# asm 1: vpsllq $32,<x7=reg256#14,>v10=reg256#13
# asm 2: vpsllq $32,<x7=%ymm13,>v10=%ymm12
vpsllq $32, % ymm13, % ymm12

# qhasm: 4x v01 = x3 unsigned>> 32
# asm 1: vpsrlq $32,<x3=reg256#10,>v01=reg256#10
# asm 2: vpsrlq $32,<x3=%ymm9,>v01=%ymm9
vpsrlq $32, % ymm9, % ymm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <x7=reg256#14,<mask1=reg256#2,>v11=reg256#2
# asm 2: vpand <x7=%ymm13,<mask1=%ymm1,>v11=%ymm1
vpand % ymm13, % ymm1, % ymm1

# qhasm: x3 = v00 | v10
# asm 1: vpor  <v00=reg256#1,<v10=reg256#13,>x3=reg256#1
# asm 2: vpor  <v00=%ymm0,<v10=%ymm12,>x3=%ymm0
vpor  % ymm0, % ymm12, % ymm0

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#10,<v11=reg256#2,>x7=reg256#2
# asm 2: vpor  <v01=%ymm9,<v11=%ymm1,>x7=%ymm1
vpor  % ymm9, % ymm1, % ymm1

# qhasm: v00 = x0 & mask2
# asm 1: vpand <x0=reg256#15,<mask2=reg256#3,>v00=reg256#10
# asm 2: vpand <x0=%ymm14,<mask2=%ymm2,>v00=%ymm9
vpand % ymm14, % ymm2, % ymm9

# qhasm: 8x v10 = x2 << 16
# asm 1: vpslld $16,<x2=reg256#12,>v10=reg256#13
# asm 2: vpslld $16,<x2=%ymm11,>v10=%ymm12
vpslld $16, % ymm11, % ymm12

# qhasm: 8x v01 = x0 unsigned>> 16
# asm 1: vpsrld $16,<x0=reg256#15,>v01=reg256#14
# asm 2: vpsrld $16,<x0=%ymm14,>v01=%ymm13
vpsrld $16, % ymm14, % ymm13

# qhasm: v11 = x2 & mask3
# asm 1: vpand <x2=reg256#12,<mask3=reg256#4,>v11=reg256#12
# asm 2: vpand <x2=%ymm11,<mask3=%ymm3,>v11=%ymm11
vpand % ymm11, % ymm3, % ymm11

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#10,<v10=reg256#13,>x0=reg256#10
# asm 2: vpor  <v00=%ymm9,<v10=%ymm12,>x0=%ymm9
vpor  % ymm9, % ymm12, % ymm9

# qhasm: x2 = v01 | v11
# asm 1: vpor  <v01=reg256#14,<v11=reg256#12,>x2=reg256#12
# asm 2: vpor  <v01=%ymm13,<v11=%ymm11,>x2=%ymm11
vpor  % ymm13, % ymm11, % ymm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <x1=reg256#11,<mask2=reg256#3,>v00=reg256#13
# asm 2: vpand <x1=%ymm10,<mask2=%ymm2,>v00=%ymm12
vpand % ymm10, % ymm2, % ymm12

# qhasm: 8x v10 = x3 << 16
# asm 1: vpslld $16,<x3=reg256#1,>v10=reg256#14
# asm 2: vpslld $16,<x3=%ymm0,>v10=%ymm13
vpslld $16, % ymm0, % ymm13

# qhasm: 8x v01 = x1 unsigned>> 16
# asm 1: vpsrld $16,<x1=reg256#11,>v01=reg256#11
# asm 2: vpsrld $16,<x1=%ymm10,>v01=%ymm10
vpsrld $16, % ymm10, % ymm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <x3=reg256#1,<mask3=reg256#4,>v11=reg256#1
# asm 2: vpand <x3=%ymm0,<mask3=%ymm3,>v11=%ymm0
vpand % ymm0, % ymm3, % ymm0

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#14,>x1=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm13,>x1=%ymm12
vpor  % ymm12, % ymm13, % ymm12

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#11,<v11=reg256#1,>x3=reg256#1
# asm 2: vpor  <v01=%ymm10,<v11=%ymm0,>x3=%ymm0
vpor  % ymm10, % ymm0, % ymm0

# qhasm: v00 = x4 & mask2
# asm 1: vpand <x4=reg256#7,<mask2=reg256#3,>v00=reg256#11
# asm 2: vpand <x4=%ymm6,<mask2=%ymm2,>v00=%ymm10
vpand % ymm6, % ymm2, % ymm10

# qhasm: 8x v10 = x6 << 16
# asm 1: vpslld $16,<x6=reg256#9,>v10=reg256#14
# asm 2: vpslld $16,<x6=%ymm8,>v10=%ymm13
vpslld $16, % ymm8, % ymm13

# qhasm: 8x v01 = x4 unsigned>> 16
# asm 1: vpsrld $16,<x4=reg256#7,>v01=reg256#7
# asm 2: vpsrld $16,<x4=%ymm6,>v01=%ymm6
vpsrld $16, % ymm6, % ymm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <x6=reg256#9,<mask3=reg256#4,>v11=reg256#9
# asm 2: vpand <x6=%ymm8,<mask3=%ymm3,>v11=%ymm8
vpand % ymm8, % ymm3, % ymm8

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#14,>x4=reg256#11
# asm 2: vpor  <v00=%ymm10,<v10=%ymm13,>x4=%ymm10
vpor  % ymm10, % ymm13, % ymm10

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#9,>x6=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm8,>x6=%ymm6
vpor  % ymm6, % ymm8, % ymm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <x5=reg256#8,<mask2=reg256#3,>v00=reg256#3
# asm 2: vpand <x5=%ymm7,<mask2=%ymm2,>v00=%ymm2
vpand % ymm7, % ymm2, % ymm2

# qhasm: 8x v10 = x7 << 16
# asm 1: vpslld $16,<x7=reg256#2,>v10=reg256#9
# asm 2: vpslld $16,<x7=%ymm1,>v10=%ymm8
vpslld $16, % ymm1, % ymm8

# qhasm: 8x v01 = x5 unsigned>> 16
# asm 1: vpsrld $16,<x5=reg256#8,>v01=reg256#8
# asm 2: vpsrld $16,<x5=%ymm7,>v01=%ymm7
vpsrld $16, % ymm7, % ymm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <x7=reg256#2,<mask3=reg256#4,>v11=reg256#2
# asm 2: vpand <x7=%ymm1,<mask3=%ymm3,>v11=%ymm1
vpand % ymm1, % ymm3, % ymm1

# qhasm: x5 = v00 | v10
# asm 1: vpor  <v00=reg256#3,<v10=reg256#9,>x5=reg256#3
# asm 2: vpor  <v00=%ymm2,<v10=%ymm8,>x5=%ymm2
vpor  % ymm2, % ymm8, % ymm2

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#2,>x7=reg256#2
# asm 2: vpor  <v01=%ymm7,<v11=%ymm1,>x7=%ymm1
vpor  % ymm7, % ymm1, % ymm1

# qhasm: v00 = x0 & mask4
# asm 1: vpand <x0=reg256#10,<mask4=reg256#5,>v00=reg256#4
# asm 2: vpand <x0=%ymm9,<mask4=%ymm4,>v00=%ymm3
vpand % ymm9, % ymm4, % ymm3

# qhasm: 16x v10 = x1 << 8
# asm 1: vpsllw $8,<x1=reg256#13,>v10=reg256#8
# asm 2: vpsllw $8,<x1=%ymm12,>v10=%ymm7
vpsllw $8, % ymm12, % ymm7

# qhasm: 16x v01 = x0 unsigned>> 8
# asm 1: vpsrlw $8,<x0=reg256#10,>v01=reg256#9
# asm 2: vpsrlw $8,<x0=%ymm9,>v01=%ymm8
vpsrlw $8, % ymm9, % ymm8

# qhasm: v11 = x1 & mask5
# asm 1: vpand <x1=reg256#13,<mask5=reg256#6,>v11=reg256#10
# asm 2: vpand <x1=%ymm12,<mask5=%ymm5,>v11=%ymm9
vpand % ymm12, % ymm5, % ymm9

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#4,<v10=reg256#8,>x0=reg256#4
# asm 2: vpor  <v00=%ymm3,<v10=%ymm7,>x0=%ymm3
vpor  % ymm3, % ymm7, % ymm3

# qhasm: x1 = v01 | v11
# asm 1: vpor  <v01=reg256#9,<v11=reg256#10,>x1=reg256#8
# asm 2: vpor  <v01=%ymm8,<v11=%ymm9,>x1=%ymm7
vpor  % ymm8, % ymm9, % ymm7

# qhasm: v00 = x2 & mask4
# asm 1: vpand <x2=reg256#12,<mask4=reg256#5,>v00=reg256#9
# asm 2: vpand <x2=%ymm11,<mask4=%ymm4,>v00=%ymm8
vpand % ymm11, % ymm4, % ymm8

# qhasm: 16x v10 = x3 << 8
# asm 1: vpsllw $8,<x3=reg256#1,>v10=reg256#10
# asm 2: vpsllw $8,<x3=%ymm0,>v10=%ymm9
vpsllw $8, % ymm0, % ymm9

# qhasm: 16x v01 = x2 unsigned>> 8
# asm 1: vpsrlw $8,<x2=reg256#12,>v01=reg256#12
# asm 2: vpsrlw $8,<x2=%ymm11,>v01=%ymm11
vpsrlw $8, % ymm11, % ymm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <x3=reg256#1,<mask5=reg256#6,>v11=reg256#1
# asm 2: vpand <x3=%ymm0,<mask5=%ymm5,>v11=%ymm0
vpand % ymm0, % ymm5, % ymm0

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#9,<v10=reg256#10,>x2=reg256#9
# asm 2: vpor  <v00=%ymm8,<v10=%ymm9,>x2=%ymm8
vpor  % ymm8, % ymm9, % ymm8

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#12,<v11=reg256#1,>x3=reg256#1
# asm 2: vpor  <v01=%ymm11,<v11=%ymm0,>x3=%ymm0
vpor  % ymm11, % ymm0, % ymm0

# qhasm: v00 = x4 & mask4
# asm 1: vpand <x4=reg256#11,<mask4=reg256#5,>v00=reg256#10
# asm 2: vpand <x4=%ymm10,<mask4=%ymm4,>v00=%ymm9
vpand % ymm10, % ymm4, % ymm9

# qhasm: 16x v10 = x5 << 8
# asm 1: vpsllw $8,<x5=reg256#3,>v10=reg256#12
# asm 2: vpsllw $8,<x5=%ymm2,>v10=%ymm11
vpsllw $8, % ymm2, % ymm11

# qhasm: 16x v01 = x4 unsigned>> 8
# asm 1: vpsrlw $8,<x4=reg256#11,>v01=reg256#11
# asm 2: vpsrlw $8,<x4=%ymm10,>v01=%ymm10
vpsrlw $8, % ymm10, % ymm10

# qhasm: v11 = x5 & mask5
# asm 1: vpand <x5=reg256#3,<mask5=reg256#6,>v11=reg256#3
# asm 2: vpand <x5=%ymm2,<mask5=%ymm5,>v11=%ymm2
vpand % ymm2, % ymm5, % ymm2

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#10,<v10=reg256#12,>x4=reg256#10
# asm 2: vpor  <v00=%ymm9,<v10=%ymm11,>x4=%ymm9
vpor  % ymm9, % ymm11, % ymm9

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#11,<v11=reg256#3,>x5=reg256#3
# asm 2: vpor  <v01=%ymm10,<v11=%ymm2,>x5=%ymm2
vpor  % ymm10, % ymm2, % ymm2

# qhasm: v00 = x6 & mask4
# asm 1: vpand <x6=reg256#7,<mask4=reg256#5,>v00=reg256#5
# asm 2: vpand <x6=%ymm6,<mask4=%ymm4,>v00=%ymm4
vpand % ymm6, % ymm4, % ymm4

# qhasm: 16x v10 = x7 << 8
# asm 1: vpsllw $8,<x7=reg256#2,>v10=reg256#11
# asm 2: vpsllw $8,<x7=%ymm1,>v10=%ymm10
vpsllw $8, % ymm1, % ymm10

# qhasm: 16x v01 = x6 unsigned>> 8
# asm 1: vpsrlw $8,<x6=reg256#7,>v01=reg256#7
# asm 2: vpsrlw $8,<x6=%ymm6,>v01=%ymm6
vpsrlw $8, % ymm6, % ymm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <x7=reg256#2,<mask5=reg256#6,>v11=reg256#2
# asm 2: vpand <x7=%ymm1,<mask5=%ymm5,>v11=%ymm1
vpand % ymm1, % ymm5, % ymm1

# qhasm: x6 = v00 | v10
# asm 1: vpor  <v00=reg256#5,<v10=reg256#11,>x6=reg256#5
# asm 2: vpor  <v00=%ymm4,<v10=%ymm10,>x6=%ymm4
vpor  % ymm4, % ymm10, % ymm4

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#2,>x7=reg256#2
# asm 2: vpor  <v01=%ymm6,<v11=%ymm1,>x7=%ymm1
vpor  % ymm6, % ymm1, % ymm1

# qhasm: mem256[ input_0 + 224 ] = x0
# asm 1: vmovupd   <x0=reg256#4,224(<input_0=int64#1)
# asm 2: vmovupd   <x0=%ymm3,224(<input_0=%rdi)
vmovupd   % ymm3, 224( % rdi)

# qhasm: mem256[ input_0 + 480 ] = x1
# asm 1: vmovupd   <x1=reg256#8,480(<input_0=int64#1)
# asm 2: vmovupd   <x1=%ymm7,480(<input_0=%rdi)
vmovupd   % ymm7, 480( % rdi)

# qhasm: mem256[ input_0 + 736 ] = x2
# asm 1: vmovupd   <x2=reg256#9,736(<input_0=int64#1)
# asm 2: vmovupd   <x2=%ymm8,736(<input_0=%rdi)
vmovupd   % ymm8, 736( % rdi)

# qhasm: mem256[ input_0 + 992 ] = x3
# asm 1: vmovupd   <x3=reg256#1,992(<input_0=int64#1)
# asm 2: vmovupd   <x3=%ymm0,992(<input_0=%rdi)
vmovupd   % ymm0, 992( % rdi)

# qhasm: mem256[ input_0 + 1248 ] = x4
# asm 1: vmovupd   <x4=reg256#10,1248(<input_0=int64#1)
# asm 2: vmovupd   <x4=%ymm9,1248(<input_0=%rdi)
vmovupd   % ymm9, 1248( % rdi)

# qhasm: mem256[ input_0 + 1504 ] = x5
# asm 1: vmovupd   <x5=reg256#3,1504(<input_0=int64#1)
# asm 2: vmovupd   <x5=%ymm2,1504(<input_0=%rdi)
vmovupd   % ymm2, 1504( % rdi)

# qhasm: mem256[ input_0 + 1760 ] = x6
# asm 1: vmovupd   <x6=reg256#5,1760(<input_0=int64#1)
# asm 2: vmovupd   <x6=%ymm4,1760(<input_0=%rdi)
vmovupd   % ymm4, 1760( % rdi)

# qhasm: mem256[ input_0 + 2016 ] = x7
# asm 1: vmovupd   <x7=reg256#2,2016(<input_0=int64#1)
# asm 2: vmovupd   <x7=%ymm1,2016(<input_0=%rdi)
vmovupd   % ymm1, 2016( % rdi)

# qhasm: mask0 aligned= mem256[ MASK2_0 ]
# asm 1: vmovapd MASK2_0(%rip),>mask0=reg256#1
# asm 2: vmovapd MASK2_0(%rip),>mask0=%ymm0
vmovapd MASK2_0( % rip), % ymm0

# qhasm: mask1 aligned= mem256[ MASK2_1 ]
# asm 1: vmovapd MASK2_1(%rip),>mask1=reg256#2
# asm 2: vmovapd MASK2_1(%rip),>mask1=%ymm1
vmovapd MASK2_1( % rip), % ymm1

# qhasm: mask2 aligned= mem256[ MASK1_0 ]
# asm 1: vmovapd MASK1_0(%rip),>mask2=reg256#3
# asm 2: vmovapd MASK1_0(%rip),>mask2=%ymm2
vmovapd MASK1_0( % rip), % ymm2

# qhasm: mask3 aligned= mem256[ MASK1_1 ]
# asm 1: vmovapd MASK1_1(%rip),>mask3=reg256#4
# asm 2: vmovapd MASK1_1(%rip),>mask3=%ymm3
vmovapd MASK1_1( % rip), % ymm3

# qhasm: mask4 aligned= mem256[ MASK0_0 ]
# asm 1: vmovapd MASK0_0(%rip),>mask4=reg256#5
# asm 2: vmovapd MASK0_0(%rip),>mask4=%ymm4
vmovapd MASK0_0( % rip), % ymm4

# qhasm: mask5 aligned= mem256[ MASK0_1 ]
# asm 1: vmovapd MASK0_1(%rip),>mask5=reg256#6
# asm 2: vmovapd MASK0_1(%rip),>mask5=%ymm5
vmovapd MASK0_1( % rip), % ymm5

# qhasm: x0 = mem256[ input_0 + 0 ]
# asm 1: vmovupd   0(<input_0=int64#1),>x0=reg256#7
# asm 2: vmovupd   0(<input_0=%rdi),>x0=%ymm6
vmovupd   0( % rdi), % ymm6

# qhasm: x1 = mem256[ input_0 + 32 ]
# asm 1: vmovupd   32(<input_0=int64#1),>x1=reg256#8
# asm 2: vmovupd   32(<input_0=%rdi),>x1=%ymm7
vmovupd   32( % rdi), % ymm7

# qhasm: x2 = mem256[ input_0 + 64 ]
# asm 1: vmovupd   64(<input_0=int64#1),>x2=reg256#9
# asm 2: vmovupd   64(<input_0=%rdi),>x2=%ymm8
vmovupd   64( % rdi), % ymm8

# qhasm: x3 = mem256[ input_0 + 96 ]
# asm 1: vmovupd   96(<input_0=int64#1),>x3=reg256#10
# asm 2: vmovupd   96(<input_0=%rdi),>x3=%ymm9
vmovupd   96( % rdi), % ymm9

# qhasm: x4 = mem256[ input_0 + 128 ]
# asm 1: vmovupd   128(<input_0=int64#1),>x4=reg256#11
# asm 2: vmovupd   128(<input_0=%rdi),>x4=%ymm10
vmovupd   128( % rdi), % ymm10

# qhasm: x5 = mem256[ input_0 + 160 ]
# asm 1: vmovupd   160(<input_0=int64#1),>x5=reg256#12
# asm 2: vmovupd   160(<input_0=%rdi),>x5=%ymm11
vmovupd   160( % rdi), % ymm11

# qhasm: x6 = mem256[ input_0 + 192 ]
# asm 1: vmovupd   192(<input_0=int64#1),>x6=reg256#13
# asm 2: vmovupd   192(<input_0=%rdi),>x6=%ymm12
vmovupd   192( % rdi), % ymm12

# qhasm: x7 = mem256[ input_0 + 224 ]
# asm 1: vmovupd   224(<input_0=int64#1),>x7=reg256#14
# asm 2: vmovupd   224(<input_0=%rdi),>x7=%ymm13
vmovupd   224( % rdi), % ymm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <x0=reg256#7,<mask0=reg256#1,>v00=reg256#15
# asm 2: vpand <x0=%ymm6,<mask0=%ymm0,>v00=%ymm14
vpand % ymm6, % ymm0, % ymm14

# qhasm: v10 = x4 & mask0
# asm 1: vpand <x4=reg256#11,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x4=%ymm10,<mask0=%ymm0,>v10=%ymm15
vpand % ymm10, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x0 & mask1
# asm 1: vpand <x0=reg256#7,<mask1=reg256#2,>v01=reg256#7
# asm 2: vpand <x0=%ymm6,<mask1=%ymm1,>v01=%ymm6
vpand % ymm6, % ymm1, % ymm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <x4=reg256#11,<mask1=reg256#2,>v11=reg256#11
# asm 2: vpand <x4=%ymm10,<mask1=%ymm1,>v11=%ymm10
vpand % ymm10, % ymm1, % ymm10

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $4,<v01=%ymm6,<v01=%ymm6
vpsrlq $4, % ymm6, % ymm6

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x0=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x0=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x4 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#11,>x4=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm10,>x4=%ymm6
vpor  % ymm6, % ymm10, % ymm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <x1=reg256#8,<mask0=reg256#1,>v00=reg256#11
# asm 2: vpand <x1=%ymm7,<mask0=%ymm0,>v00=%ymm10
vpand % ymm7, % ymm0, % ymm10

# qhasm: v10 = x5 & mask0
# asm 1: vpand <x5=reg256#12,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x5=%ymm11,<mask0=%ymm0,>v10=%ymm15
vpand % ymm11, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x1 & mask1
# asm 1: vpand <x1=reg256#8,<mask1=reg256#2,>v01=reg256#8
# asm 2: vpand <x1=%ymm7,<mask1=%ymm1,>v01=%ymm7
vpand % ymm7, % ymm1, % ymm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <x5=reg256#12,<mask1=reg256#2,>v11=reg256#12
# asm 2: vpand <x5=%ymm11,<mask1=%ymm1,>v11=%ymm11
vpand % ymm11, % ymm1, % ymm11

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#8,<v01=reg256#8
# asm 2: vpsrlq $4,<v01=%ymm7,<v01=%ymm7
vpsrlq $4, % ymm7, % ymm7

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#16,>x1=reg256#11
# asm 2: vpor  <v00=%ymm10,<v10=%ymm15,>x1=%ymm10
vpor  % ymm10, % ymm15, % ymm10

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#12,>x5=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm11,>x5=%ymm7
vpor  % ymm7, % ymm11, % ymm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <x2=reg256#9,<mask0=reg256#1,>v00=reg256#12
# asm 2: vpand <x2=%ymm8,<mask0=%ymm0,>v00=%ymm11
vpand % ymm8, % ymm0, % ymm11

# qhasm: v10 = x6 & mask0
# asm 1: vpand <x6=reg256#13,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x6=%ymm12,<mask0=%ymm0,>v10=%ymm15
vpand % ymm12, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x2 & mask1
# asm 1: vpand <x2=reg256#9,<mask1=reg256#2,>v01=reg256#9
# asm 2: vpand <x2=%ymm8,<mask1=%ymm1,>v01=%ymm8
vpand % ymm8, % ymm1, % ymm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <x6=reg256#13,<mask1=reg256#2,>v11=reg256#13
# asm 2: vpand <x6=%ymm12,<mask1=%ymm1,>v11=%ymm12
vpand % ymm12, % ymm1, % ymm12

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#9,<v01=reg256#9
# asm 2: vpsrlq $4,<v01=%ymm8,<v01=%ymm8
vpsrlq $4, % ymm8, % ymm8

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x2=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x2=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#9,<v11=reg256#13,>x6=reg256#9
# asm 2: vpor  <v01=%ymm8,<v11=%ymm12,>x6=%ymm8
vpor  % ymm8, % ymm12, % ymm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <x3=reg256#10,<mask0=reg256#1,>v00=reg256#13
# asm 2: vpand <x3=%ymm9,<mask0=%ymm0,>v00=%ymm12
vpand % ymm9, % ymm0, % ymm12

# qhasm: v10 = x7 & mask0
# asm 1: vpand <x7=reg256#14,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x7=%ymm13,<mask0=%ymm0,>v10=%ymm15
vpand % ymm13, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x3 & mask1
# asm 1: vpand <x3=reg256#10,<mask1=reg256#2,>v01=reg256#10
# asm 2: vpand <x3=%ymm9,<mask1=%ymm1,>v01=%ymm9
vpand % ymm9, % ymm1, % ymm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <x7=reg256#14,<mask1=reg256#2,>v11=reg256#14
# asm 2: vpand <x7=%ymm13,<mask1=%ymm1,>v11=%ymm13
vpand % ymm13, % ymm1, % ymm13

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#10,<v01=reg256#10
# asm 2: vpsrlq $4,<v01=%ymm9,<v01=%ymm9
vpsrlq $4, % ymm9, % ymm9

# qhasm: x3 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x3=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x3=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#10,<v11=reg256#14,>x7=reg256#10
# asm 2: vpor  <v01=%ymm9,<v11=%ymm13,>x7=%ymm9
vpor  % ymm9, % ymm13, % ymm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <x0=reg256#15,<mask2=reg256#3,>v00=reg256#14
# asm 2: vpand <x0=%ymm14,<mask2=%ymm2,>v00=%ymm13
vpand % ymm14, % ymm2, % ymm13

# qhasm: v10 = x2 & mask2
# asm 1: vpand <x2=reg256#12,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x2=%ymm11,<mask2=%ymm2,>v10=%ymm15
vpand % ymm11, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x0 & mask3
# asm 1: vpand <x0=reg256#15,<mask3=reg256#4,>v01=reg256#15
# asm 2: vpand <x0=%ymm14,<mask3=%ymm3,>v01=%ymm14
vpand % ymm14, % ymm3, % ymm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <x2=reg256#12,<mask3=reg256#4,>v11=reg256#12
# asm 2: vpand <x2=%ymm11,<mask3=%ymm3,>v11=%ymm11
vpand % ymm11, % ymm3, % ymm11

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#15,<v01=reg256#15
# asm 2: vpsrlq $2,<v01=%ymm14,<v01=%ymm14
vpsrlq $2, % ymm14, % ymm14

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#14,<v10=reg256#16,>x0=reg256#14
# asm 2: vpor  <v00=%ymm13,<v10=%ymm15,>x0=%ymm13
vpor  % ymm13, % ymm15, % ymm13

# qhasm: x2 = v01 | v11
# asm 1: vpor  <v01=reg256#15,<v11=reg256#12,>x2=reg256#12
# asm 2: vpor  <v01=%ymm14,<v11=%ymm11,>x2=%ymm11
vpor  % ymm14, % ymm11, % ymm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <x1=reg256#11,<mask2=reg256#3,>v00=reg256#15
# asm 2: vpand <x1=%ymm10,<mask2=%ymm2,>v00=%ymm14
vpand % ymm10, % ymm2, % ymm14

# qhasm: v10 = x3 & mask2
# asm 1: vpand <x3=reg256#13,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x3=%ymm12,<mask2=%ymm2,>v10=%ymm15
vpand % ymm12, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x1 & mask3
# asm 1: vpand <x1=reg256#11,<mask3=reg256#4,>v01=reg256#11
# asm 2: vpand <x1=%ymm10,<mask3=%ymm3,>v01=%ymm10
vpand % ymm10, % ymm3, % ymm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <x3=reg256#13,<mask3=reg256#4,>v11=reg256#13
# asm 2: vpand <x3=%ymm12,<mask3=%ymm3,>v11=%ymm12
vpand % ymm12, % ymm3, % ymm12

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#11,<v01=reg256#11
# asm 2: vpsrlq $2,<v01=%ymm10,<v01=%ymm10
vpsrlq $2, % ymm10, % ymm10

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x1=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x1=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#11,<v11=reg256#13,>x3=reg256#11
# asm 2: vpor  <v01=%ymm10,<v11=%ymm12,>x3=%ymm10
vpor  % ymm10, % ymm12, % ymm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <x4=reg256#7,<mask2=reg256#3,>v00=reg256#13
# asm 2: vpand <x4=%ymm6,<mask2=%ymm2,>v00=%ymm12
vpand % ymm6, % ymm2, % ymm12

# qhasm: v10 = x6 & mask2
# asm 1: vpand <x6=reg256#9,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x6=%ymm8,<mask2=%ymm2,>v10=%ymm15
vpand % ymm8, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x4 & mask3
# asm 1: vpand <x4=reg256#7,<mask3=reg256#4,>v01=reg256#7
# asm 2: vpand <x4=%ymm6,<mask3=%ymm3,>v01=%ymm6
vpand % ymm6, % ymm3, % ymm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <x6=reg256#9,<mask3=reg256#4,>v11=reg256#9
# asm 2: vpand <x6=%ymm8,<mask3=%ymm3,>v11=%ymm8
vpand % ymm8, % ymm3, % ymm8

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $2,<v01=%ymm6,<v01=%ymm6
vpsrlq $2, % ymm6, % ymm6

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x4=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x4=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#9,>x6=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm8,>x6=%ymm6
vpor  % ymm6, % ymm8, % ymm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <x5=reg256#8,<mask2=reg256#3,>v00=reg256#9
# asm 2: vpand <x5=%ymm7,<mask2=%ymm2,>v00=%ymm8
vpand % ymm7, % ymm2, % ymm8

# qhasm: v10 = x7 & mask2
# asm 1: vpand <x7=reg256#10,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x7=%ymm9,<mask2=%ymm2,>v10=%ymm15
vpand % ymm9, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x5 & mask3
# asm 1: vpand <x5=reg256#8,<mask3=reg256#4,>v01=reg256#8
# asm 2: vpand <x5=%ymm7,<mask3=%ymm3,>v01=%ymm7
vpand % ymm7, % ymm3, % ymm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <x7=reg256#10,<mask3=reg256#4,>v11=reg256#10
# asm 2: vpand <x7=%ymm9,<mask3=%ymm3,>v11=%ymm9
vpand % ymm9, % ymm3, % ymm9

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#8,<v01=reg256#8
# asm 2: vpsrlq $2,<v01=%ymm7,<v01=%ymm7
vpsrlq $2, % ymm7, % ymm7

# qhasm: x5 = v00 | v10
# asm 1: vpor  <v00=reg256#9,<v10=reg256#16,>x5=reg256#9
# asm 2: vpor  <v00=%ymm8,<v10=%ymm15,>x5=%ymm8
vpor  % ymm8, % ymm15, % ymm8

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#10,>x7=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm9,>x7=%ymm7
vpor  % ymm7, % ymm9, % ymm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <x0=reg256#14,<mask4=reg256#5,>v00=reg256#10
# asm 2: vpand <x0=%ymm13,<mask4=%ymm4,>v00=%ymm9
vpand % ymm13, % ymm4, % ymm9

# qhasm: v10 = x1 & mask4
# asm 1: vpand <x1=reg256#15,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x1=%ymm14,<mask4=%ymm4,>v10=%ymm15
vpand % ymm14, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x0 & mask5
# asm 1: vpand <x0=reg256#14,<mask5=reg256#6,>v01=reg256#14
# asm 2: vpand <x0=%ymm13,<mask5=%ymm5,>v01=%ymm13
vpand % ymm13, % ymm5, % ymm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <x1=reg256#15,<mask5=reg256#6,>v11=reg256#15
# asm 2: vpand <x1=%ymm14,<mask5=%ymm5,>v11=%ymm14
vpand % ymm14, % ymm5, % ymm14

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#14,<v01=reg256#14
# asm 2: vpsrlq $1,<v01=%ymm13,<v01=%ymm13
vpsrlq $1, % ymm13, % ymm13

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#10,<v10=reg256#16,>x0=reg256#10
# asm 2: vpor  <v00=%ymm9,<v10=%ymm15,>x0=%ymm9
vpor  % ymm9, % ymm15, % ymm9

# qhasm: x1 = v01 | v11
# asm 1: vpor  <v01=reg256#14,<v11=reg256#15,>x1=reg256#14
# asm 2: vpor  <v01=%ymm13,<v11=%ymm14,>x1=%ymm13
vpor  % ymm13, % ymm14, % ymm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <x2=reg256#12,<mask4=reg256#5,>v00=reg256#15
# asm 2: vpand <x2=%ymm11,<mask4=%ymm4,>v00=%ymm14
vpand % ymm11, % ymm4, % ymm14

# qhasm: v10 = x3 & mask4
# asm 1: vpand <x3=reg256#11,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x3=%ymm10,<mask4=%ymm4,>v10=%ymm15
vpand % ymm10, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x2 & mask5
# asm 1: vpand <x2=reg256#12,<mask5=reg256#6,>v01=reg256#12
# asm 2: vpand <x2=%ymm11,<mask5=%ymm5,>v01=%ymm11
vpand % ymm11, % ymm5, % ymm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <x3=reg256#11,<mask5=reg256#6,>v11=reg256#11
# asm 2: vpand <x3=%ymm10,<mask5=%ymm5,>v11=%ymm10
vpand % ymm10, % ymm5, % ymm10

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#12,<v01=reg256#12
# asm 2: vpsrlq $1,<v01=%ymm11,<v01=%ymm11
vpsrlq $1, % ymm11, % ymm11

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x2=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x2=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#12,<v11=reg256#11,>x3=reg256#11
# asm 2: vpor  <v01=%ymm11,<v11=%ymm10,>x3=%ymm10
vpor  % ymm11, % ymm10, % ymm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <x4=reg256#13,<mask4=reg256#5,>v00=reg256#12
# asm 2: vpand <x4=%ymm12,<mask4=%ymm4,>v00=%ymm11
vpand % ymm12, % ymm4, % ymm11

# qhasm: v10 = x5 & mask4
# asm 1: vpand <x5=reg256#9,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x5=%ymm8,<mask4=%ymm4,>v10=%ymm15
vpand % ymm8, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x4 & mask5
# asm 1: vpand <x4=reg256#13,<mask5=reg256#6,>v01=reg256#13
# asm 2: vpand <x4=%ymm12,<mask5=%ymm5,>v01=%ymm12
vpand % ymm12, % ymm5, % ymm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <x5=reg256#9,<mask5=reg256#6,>v11=reg256#9
# asm 2: vpand <x5=%ymm8,<mask5=%ymm5,>v11=%ymm8
vpand % ymm8, % ymm5, % ymm8

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#13,<v01=reg256#13
# asm 2: vpsrlq $1,<v01=%ymm12,<v01=%ymm12
vpsrlq $1, % ymm12, % ymm12

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x4=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x4=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#13,<v11=reg256#9,>x5=reg256#9
# asm 2: vpor  <v01=%ymm12,<v11=%ymm8,>x5=%ymm8
vpor  % ymm12, % ymm8, % ymm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <x6=reg256#7,<mask4=reg256#5,>v00=reg256#13
# asm 2: vpand <x6=%ymm6,<mask4=%ymm4,>v00=%ymm12
vpand % ymm6, % ymm4, % ymm12

# qhasm: v10 = x7 & mask4
# asm 1: vpand <x7=reg256#8,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x7=%ymm7,<mask4=%ymm4,>v10=%ymm15
vpand % ymm7, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x6 & mask5
# asm 1: vpand <x6=reg256#7,<mask5=reg256#6,>v01=reg256#7
# asm 2: vpand <x6=%ymm6,<mask5=%ymm5,>v01=%ymm6
vpand % ymm6, % ymm5, % ymm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <x7=reg256#8,<mask5=reg256#6,>v11=reg256#8
# asm 2: vpand <x7=%ymm7,<mask5=%ymm5,>v11=%ymm7
vpand % ymm7, % ymm5, % ymm7

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $1,<v01=%ymm6,<v01=%ymm6
vpsrlq $1, % ymm6, % ymm6

# qhasm: x6 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x6=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x6=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#8,>x7=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm7,>x7=%ymm6
vpor  % ymm6, % ymm7, % ymm6

# qhasm: mem256[ input_0 + 0 ] = x0
# asm 1: vmovupd   <x0=reg256#10,0(<input_0=int64#1)
# asm 2: vmovupd   <x0=%ymm9,0(<input_0=%rdi)
vmovupd   % ymm9, 0( % rdi)

# qhasm: mem256[ input_0 + 32 ] = x1
# asm 1: vmovupd   <x1=reg256#14,32(<input_0=int64#1)
# asm 2: vmovupd   <x1=%ymm13,32(<input_0=%rdi)
vmovupd   % ymm13, 32( % rdi)

# qhasm: mem256[ input_0 + 64 ] = x2
# asm 1: vmovupd   <x2=reg256#15,64(<input_0=int64#1)
# asm 2: vmovupd   <x2=%ymm14,64(<input_0=%rdi)
vmovupd   % ymm14, 64( % rdi)

# qhasm: mem256[ input_0 + 96 ] = x3
# asm 1: vmovupd   <x3=reg256#11,96(<input_0=int64#1)
# asm 2: vmovupd   <x3=%ymm10,96(<input_0=%rdi)
vmovupd   % ymm10, 96( % rdi)

# qhasm: mem256[ input_0 + 128 ] = x4
# asm 1: vmovupd   <x4=reg256#12,128(<input_0=int64#1)
# asm 2: vmovupd   <x4=%ymm11,128(<input_0=%rdi)
vmovupd   % ymm11, 128( % rdi)

# qhasm: mem256[ input_0 + 160 ] = x5
# asm 1: vmovupd   <x5=reg256#9,160(<input_0=int64#1)
# asm 2: vmovupd   <x5=%ymm8,160(<input_0=%rdi)
vmovupd   % ymm8, 160( % rdi)

# qhasm: mem256[ input_0 + 192 ] = x6
# asm 1: vmovupd   <x6=reg256#13,192(<input_0=int64#1)
# asm 2: vmovupd   <x6=%ymm12,192(<input_0=%rdi)
vmovupd   % ymm12, 192( % rdi)

# qhasm: mem256[ input_0 + 224 ] = x7
# asm 1: vmovupd   <x7=reg256#7,224(<input_0=int64#1)
# asm 2: vmovupd   <x7=%ymm6,224(<input_0=%rdi)
vmovupd   % ymm6, 224( % rdi)

# qhasm: x0 = mem256[ input_0 + 256 ]
# asm 1: vmovupd   256(<input_0=int64#1),>x0=reg256#7
# asm 2: vmovupd   256(<input_0=%rdi),>x0=%ymm6
vmovupd   256( % rdi), % ymm6

# qhasm: x1 = mem256[ input_0 + 288 ]
# asm 1: vmovupd   288(<input_0=int64#1),>x1=reg256#8
# asm 2: vmovupd   288(<input_0=%rdi),>x1=%ymm7
vmovupd   288( % rdi), % ymm7

# qhasm: x2 = mem256[ input_0 + 320 ]
# asm 1: vmovupd   320(<input_0=int64#1),>x2=reg256#9
# asm 2: vmovupd   320(<input_0=%rdi),>x2=%ymm8
vmovupd   320( % rdi), % ymm8

# qhasm: x3 = mem256[ input_0 + 352 ]
# asm 1: vmovupd   352(<input_0=int64#1),>x3=reg256#10
# asm 2: vmovupd   352(<input_0=%rdi),>x3=%ymm9
vmovupd   352( % rdi), % ymm9

# qhasm: x4 = mem256[ input_0 + 384 ]
# asm 1: vmovupd   384(<input_0=int64#1),>x4=reg256#11
# asm 2: vmovupd   384(<input_0=%rdi),>x4=%ymm10
vmovupd   384( % rdi), % ymm10

# qhasm: x5 = mem256[ input_0 + 416 ]
# asm 1: vmovupd   416(<input_0=int64#1),>x5=reg256#12
# asm 2: vmovupd   416(<input_0=%rdi),>x5=%ymm11
vmovupd   416( % rdi), % ymm11

# qhasm: x6 = mem256[ input_0 + 448 ]
# asm 1: vmovupd   448(<input_0=int64#1),>x6=reg256#13
# asm 2: vmovupd   448(<input_0=%rdi),>x6=%ymm12
vmovupd   448( % rdi), % ymm12

# qhasm: x7 = mem256[ input_0 + 480 ]
# asm 1: vmovupd   480(<input_0=int64#1),>x7=reg256#14
# asm 2: vmovupd   480(<input_0=%rdi),>x7=%ymm13
vmovupd   480( % rdi), % ymm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <x0=reg256#7,<mask0=reg256#1,>v00=reg256#15
# asm 2: vpand <x0=%ymm6,<mask0=%ymm0,>v00=%ymm14
vpand % ymm6, % ymm0, % ymm14

# qhasm: v10 = x4 & mask0
# asm 1: vpand <x4=reg256#11,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x4=%ymm10,<mask0=%ymm0,>v10=%ymm15
vpand % ymm10, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x0 & mask1
# asm 1: vpand <x0=reg256#7,<mask1=reg256#2,>v01=reg256#7
# asm 2: vpand <x0=%ymm6,<mask1=%ymm1,>v01=%ymm6
vpand % ymm6, % ymm1, % ymm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <x4=reg256#11,<mask1=reg256#2,>v11=reg256#11
# asm 2: vpand <x4=%ymm10,<mask1=%ymm1,>v11=%ymm10
vpand % ymm10, % ymm1, % ymm10

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $4,<v01=%ymm6,<v01=%ymm6
vpsrlq $4, % ymm6, % ymm6

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x0=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x0=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x4 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#11,>x4=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm10,>x4=%ymm6
vpor  % ymm6, % ymm10, % ymm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <x1=reg256#8,<mask0=reg256#1,>v00=reg256#11
# asm 2: vpand <x1=%ymm7,<mask0=%ymm0,>v00=%ymm10
vpand % ymm7, % ymm0, % ymm10

# qhasm: v10 = x5 & mask0
# asm 1: vpand <x5=reg256#12,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x5=%ymm11,<mask0=%ymm0,>v10=%ymm15
vpand % ymm11, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x1 & mask1
# asm 1: vpand <x1=reg256#8,<mask1=reg256#2,>v01=reg256#8
# asm 2: vpand <x1=%ymm7,<mask1=%ymm1,>v01=%ymm7
vpand % ymm7, % ymm1, % ymm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <x5=reg256#12,<mask1=reg256#2,>v11=reg256#12
# asm 2: vpand <x5=%ymm11,<mask1=%ymm1,>v11=%ymm11
vpand % ymm11, % ymm1, % ymm11

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#8,<v01=reg256#8
# asm 2: vpsrlq $4,<v01=%ymm7,<v01=%ymm7
vpsrlq $4, % ymm7, % ymm7

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#16,>x1=reg256#11
# asm 2: vpor  <v00=%ymm10,<v10=%ymm15,>x1=%ymm10
vpor  % ymm10, % ymm15, % ymm10

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#12,>x5=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm11,>x5=%ymm7
vpor  % ymm7, % ymm11, % ymm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <x2=reg256#9,<mask0=reg256#1,>v00=reg256#12
# asm 2: vpand <x2=%ymm8,<mask0=%ymm0,>v00=%ymm11
vpand % ymm8, % ymm0, % ymm11

# qhasm: v10 = x6 & mask0
# asm 1: vpand <x6=reg256#13,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x6=%ymm12,<mask0=%ymm0,>v10=%ymm15
vpand % ymm12, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x2 & mask1
# asm 1: vpand <x2=reg256#9,<mask1=reg256#2,>v01=reg256#9
# asm 2: vpand <x2=%ymm8,<mask1=%ymm1,>v01=%ymm8
vpand % ymm8, % ymm1, % ymm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <x6=reg256#13,<mask1=reg256#2,>v11=reg256#13
# asm 2: vpand <x6=%ymm12,<mask1=%ymm1,>v11=%ymm12
vpand % ymm12, % ymm1, % ymm12

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#9,<v01=reg256#9
# asm 2: vpsrlq $4,<v01=%ymm8,<v01=%ymm8
vpsrlq $4, % ymm8, % ymm8

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x2=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x2=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#9,<v11=reg256#13,>x6=reg256#9
# asm 2: vpor  <v01=%ymm8,<v11=%ymm12,>x6=%ymm8
vpor  % ymm8, % ymm12, % ymm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <x3=reg256#10,<mask0=reg256#1,>v00=reg256#13
# asm 2: vpand <x3=%ymm9,<mask0=%ymm0,>v00=%ymm12
vpand % ymm9, % ymm0, % ymm12

# qhasm: v10 = x7 & mask0
# asm 1: vpand <x7=reg256#14,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x7=%ymm13,<mask0=%ymm0,>v10=%ymm15
vpand % ymm13, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x3 & mask1
# asm 1: vpand <x3=reg256#10,<mask1=reg256#2,>v01=reg256#10
# asm 2: vpand <x3=%ymm9,<mask1=%ymm1,>v01=%ymm9
vpand % ymm9, % ymm1, % ymm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <x7=reg256#14,<mask1=reg256#2,>v11=reg256#14
# asm 2: vpand <x7=%ymm13,<mask1=%ymm1,>v11=%ymm13
vpand % ymm13, % ymm1, % ymm13

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#10,<v01=reg256#10
# asm 2: vpsrlq $4,<v01=%ymm9,<v01=%ymm9
vpsrlq $4, % ymm9, % ymm9

# qhasm: x3 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x3=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x3=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#10,<v11=reg256#14,>x7=reg256#10
# asm 2: vpor  <v01=%ymm9,<v11=%ymm13,>x7=%ymm9
vpor  % ymm9, % ymm13, % ymm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <x0=reg256#15,<mask2=reg256#3,>v00=reg256#14
# asm 2: vpand <x0=%ymm14,<mask2=%ymm2,>v00=%ymm13
vpand % ymm14, % ymm2, % ymm13

# qhasm: v10 = x2 & mask2
# asm 1: vpand <x2=reg256#12,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x2=%ymm11,<mask2=%ymm2,>v10=%ymm15
vpand % ymm11, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x0 & mask3
# asm 1: vpand <x0=reg256#15,<mask3=reg256#4,>v01=reg256#15
# asm 2: vpand <x0=%ymm14,<mask3=%ymm3,>v01=%ymm14
vpand % ymm14, % ymm3, % ymm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <x2=reg256#12,<mask3=reg256#4,>v11=reg256#12
# asm 2: vpand <x2=%ymm11,<mask3=%ymm3,>v11=%ymm11
vpand % ymm11, % ymm3, % ymm11

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#15,<v01=reg256#15
# asm 2: vpsrlq $2,<v01=%ymm14,<v01=%ymm14
vpsrlq $2, % ymm14, % ymm14

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#14,<v10=reg256#16,>x0=reg256#14
# asm 2: vpor  <v00=%ymm13,<v10=%ymm15,>x0=%ymm13
vpor  % ymm13, % ymm15, % ymm13

# qhasm: x2 = v01 | v11
# asm 1: vpor  <v01=reg256#15,<v11=reg256#12,>x2=reg256#12
# asm 2: vpor  <v01=%ymm14,<v11=%ymm11,>x2=%ymm11
vpor  % ymm14, % ymm11, % ymm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <x1=reg256#11,<mask2=reg256#3,>v00=reg256#15
# asm 2: vpand <x1=%ymm10,<mask2=%ymm2,>v00=%ymm14
vpand % ymm10, % ymm2, % ymm14

# qhasm: v10 = x3 & mask2
# asm 1: vpand <x3=reg256#13,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x3=%ymm12,<mask2=%ymm2,>v10=%ymm15
vpand % ymm12, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x1 & mask3
# asm 1: vpand <x1=reg256#11,<mask3=reg256#4,>v01=reg256#11
# asm 2: vpand <x1=%ymm10,<mask3=%ymm3,>v01=%ymm10
vpand % ymm10, % ymm3, % ymm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <x3=reg256#13,<mask3=reg256#4,>v11=reg256#13
# asm 2: vpand <x3=%ymm12,<mask3=%ymm3,>v11=%ymm12
vpand % ymm12, % ymm3, % ymm12

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#11,<v01=reg256#11
# asm 2: vpsrlq $2,<v01=%ymm10,<v01=%ymm10
vpsrlq $2, % ymm10, % ymm10

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x1=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x1=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#11,<v11=reg256#13,>x3=reg256#11
# asm 2: vpor  <v01=%ymm10,<v11=%ymm12,>x3=%ymm10
vpor  % ymm10, % ymm12, % ymm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <x4=reg256#7,<mask2=reg256#3,>v00=reg256#13
# asm 2: vpand <x4=%ymm6,<mask2=%ymm2,>v00=%ymm12
vpand % ymm6, % ymm2, % ymm12

# qhasm: v10 = x6 & mask2
# asm 1: vpand <x6=reg256#9,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x6=%ymm8,<mask2=%ymm2,>v10=%ymm15
vpand % ymm8, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x4 & mask3
# asm 1: vpand <x4=reg256#7,<mask3=reg256#4,>v01=reg256#7
# asm 2: vpand <x4=%ymm6,<mask3=%ymm3,>v01=%ymm6
vpand % ymm6, % ymm3, % ymm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <x6=reg256#9,<mask3=reg256#4,>v11=reg256#9
# asm 2: vpand <x6=%ymm8,<mask3=%ymm3,>v11=%ymm8
vpand % ymm8, % ymm3, % ymm8

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $2,<v01=%ymm6,<v01=%ymm6
vpsrlq $2, % ymm6, % ymm6

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x4=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x4=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#9,>x6=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm8,>x6=%ymm6
vpor  % ymm6, % ymm8, % ymm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <x5=reg256#8,<mask2=reg256#3,>v00=reg256#9
# asm 2: vpand <x5=%ymm7,<mask2=%ymm2,>v00=%ymm8
vpand % ymm7, % ymm2, % ymm8

# qhasm: v10 = x7 & mask2
# asm 1: vpand <x7=reg256#10,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x7=%ymm9,<mask2=%ymm2,>v10=%ymm15
vpand % ymm9, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x5 & mask3
# asm 1: vpand <x5=reg256#8,<mask3=reg256#4,>v01=reg256#8
# asm 2: vpand <x5=%ymm7,<mask3=%ymm3,>v01=%ymm7
vpand % ymm7, % ymm3, % ymm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <x7=reg256#10,<mask3=reg256#4,>v11=reg256#10
# asm 2: vpand <x7=%ymm9,<mask3=%ymm3,>v11=%ymm9
vpand % ymm9, % ymm3, % ymm9

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#8,<v01=reg256#8
# asm 2: vpsrlq $2,<v01=%ymm7,<v01=%ymm7
vpsrlq $2, % ymm7, % ymm7

# qhasm: x5 = v00 | v10
# asm 1: vpor  <v00=reg256#9,<v10=reg256#16,>x5=reg256#9
# asm 2: vpor  <v00=%ymm8,<v10=%ymm15,>x5=%ymm8
vpor  % ymm8, % ymm15, % ymm8

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#10,>x7=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm9,>x7=%ymm7
vpor  % ymm7, % ymm9, % ymm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <x0=reg256#14,<mask4=reg256#5,>v00=reg256#10
# asm 2: vpand <x0=%ymm13,<mask4=%ymm4,>v00=%ymm9
vpand % ymm13, % ymm4, % ymm9

# qhasm: v10 = x1 & mask4
# asm 1: vpand <x1=reg256#15,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x1=%ymm14,<mask4=%ymm4,>v10=%ymm15
vpand % ymm14, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x0 & mask5
# asm 1: vpand <x0=reg256#14,<mask5=reg256#6,>v01=reg256#14
# asm 2: vpand <x0=%ymm13,<mask5=%ymm5,>v01=%ymm13
vpand % ymm13, % ymm5, % ymm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <x1=reg256#15,<mask5=reg256#6,>v11=reg256#15
# asm 2: vpand <x1=%ymm14,<mask5=%ymm5,>v11=%ymm14
vpand % ymm14, % ymm5, % ymm14

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#14,<v01=reg256#14
# asm 2: vpsrlq $1,<v01=%ymm13,<v01=%ymm13
vpsrlq $1, % ymm13, % ymm13

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#10,<v10=reg256#16,>x0=reg256#10
# asm 2: vpor  <v00=%ymm9,<v10=%ymm15,>x0=%ymm9
vpor  % ymm9, % ymm15, % ymm9

# qhasm: x1 = v01 | v11
# asm 1: vpor  <v01=reg256#14,<v11=reg256#15,>x1=reg256#14
# asm 2: vpor  <v01=%ymm13,<v11=%ymm14,>x1=%ymm13
vpor  % ymm13, % ymm14, % ymm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <x2=reg256#12,<mask4=reg256#5,>v00=reg256#15
# asm 2: vpand <x2=%ymm11,<mask4=%ymm4,>v00=%ymm14
vpand % ymm11, % ymm4, % ymm14

# qhasm: v10 = x3 & mask4
# asm 1: vpand <x3=reg256#11,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x3=%ymm10,<mask4=%ymm4,>v10=%ymm15
vpand % ymm10, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x2 & mask5
# asm 1: vpand <x2=reg256#12,<mask5=reg256#6,>v01=reg256#12
# asm 2: vpand <x2=%ymm11,<mask5=%ymm5,>v01=%ymm11
vpand % ymm11, % ymm5, % ymm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <x3=reg256#11,<mask5=reg256#6,>v11=reg256#11
# asm 2: vpand <x3=%ymm10,<mask5=%ymm5,>v11=%ymm10
vpand % ymm10, % ymm5, % ymm10

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#12,<v01=reg256#12
# asm 2: vpsrlq $1,<v01=%ymm11,<v01=%ymm11
vpsrlq $1, % ymm11, % ymm11

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x2=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x2=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#12,<v11=reg256#11,>x3=reg256#11
# asm 2: vpor  <v01=%ymm11,<v11=%ymm10,>x3=%ymm10
vpor  % ymm11, % ymm10, % ymm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <x4=reg256#13,<mask4=reg256#5,>v00=reg256#12
# asm 2: vpand <x4=%ymm12,<mask4=%ymm4,>v00=%ymm11
vpand % ymm12, % ymm4, % ymm11

# qhasm: v10 = x5 & mask4
# asm 1: vpand <x5=reg256#9,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x5=%ymm8,<mask4=%ymm4,>v10=%ymm15
vpand % ymm8, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x4 & mask5
# asm 1: vpand <x4=reg256#13,<mask5=reg256#6,>v01=reg256#13
# asm 2: vpand <x4=%ymm12,<mask5=%ymm5,>v01=%ymm12
vpand % ymm12, % ymm5, % ymm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <x5=reg256#9,<mask5=reg256#6,>v11=reg256#9
# asm 2: vpand <x5=%ymm8,<mask5=%ymm5,>v11=%ymm8
vpand % ymm8, % ymm5, % ymm8

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#13,<v01=reg256#13
# asm 2: vpsrlq $1,<v01=%ymm12,<v01=%ymm12
vpsrlq $1, % ymm12, % ymm12

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x4=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x4=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#13,<v11=reg256#9,>x5=reg256#9
# asm 2: vpor  <v01=%ymm12,<v11=%ymm8,>x5=%ymm8
vpor  % ymm12, % ymm8, % ymm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <x6=reg256#7,<mask4=reg256#5,>v00=reg256#13
# asm 2: vpand <x6=%ymm6,<mask4=%ymm4,>v00=%ymm12
vpand % ymm6, % ymm4, % ymm12

# qhasm: v10 = x7 & mask4
# asm 1: vpand <x7=reg256#8,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x7=%ymm7,<mask4=%ymm4,>v10=%ymm15
vpand % ymm7, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x6 & mask5
# asm 1: vpand <x6=reg256#7,<mask5=reg256#6,>v01=reg256#7
# asm 2: vpand <x6=%ymm6,<mask5=%ymm5,>v01=%ymm6
vpand % ymm6, % ymm5, % ymm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <x7=reg256#8,<mask5=reg256#6,>v11=reg256#8
# asm 2: vpand <x7=%ymm7,<mask5=%ymm5,>v11=%ymm7
vpand % ymm7, % ymm5, % ymm7

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $1,<v01=%ymm6,<v01=%ymm6
vpsrlq $1, % ymm6, % ymm6

# qhasm: x6 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x6=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x6=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#8,>x7=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm7,>x7=%ymm6
vpor  % ymm6, % ymm7, % ymm6

# qhasm: mem256[ input_0 + 256 ] = x0
# asm 1: vmovupd   <x0=reg256#10,256(<input_0=int64#1)
# asm 2: vmovupd   <x0=%ymm9,256(<input_0=%rdi)
vmovupd   % ymm9, 256( % rdi)

# qhasm: mem256[ input_0 + 288 ] = x1
# asm 1: vmovupd   <x1=reg256#14,288(<input_0=int64#1)
# asm 2: vmovupd   <x1=%ymm13,288(<input_0=%rdi)
vmovupd   % ymm13, 288( % rdi)

# qhasm: mem256[ input_0 + 320 ] = x2
# asm 1: vmovupd   <x2=reg256#15,320(<input_0=int64#1)
# asm 2: vmovupd   <x2=%ymm14,320(<input_0=%rdi)
vmovupd   % ymm14, 320( % rdi)

# qhasm: mem256[ input_0 + 352 ] = x3
# asm 1: vmovupd   <x3=reg256#11,352(<input_0=int64#1)
# asm 2: vmovupd   <x3=%ymm10,352(<input_0=%rdi)
vmovupd   % ymm10, 352( % rdi)

# qhasm: mem256[ input_0 + 384 ] = x4
# asm 1: vmovupd   <x4=reg256#12,384(<input_0=int64#1)
# asm 2: vmovupd   <x4=%ymm11,384(<input_0=%rdi)
vmovupd   % ymm11, 384( % rdi)

# qhasm: mem256[ input_0 + 416 ] = x5
# asm 1: vmovupd   <x5=reg256#9,416(<input_0=int64#1)
# asm 2: vmovupd   <x5=%ymm8,416(<input_0=%rdi)
vmovupd   % ymm8, 416( % rdi)

# qhasm: mem256[ input_0 + 448 ] = x6
# asm 1: vmovupd   <x6=reg256#13,448(<input_0=int64#1)
# asm 2: vmovupd   <x6=%ymm12,448(<input_0=%rdi)
vmovupd   % ymm12, 448( % rdi)

# qhasm: mem256[ input_0 + 480 ] = x7
# asm 1: vmovupd   <x7=reg256#7,480(<input_0=int64#1)
# asm 2: vmovupd   <x7=%ymm6,480(<input_0=%rdi)
vmovupd   % ymm6, 480( % rdi)

# qhasm: x0 = mem256[ input_0 + 512 ]
# asm 1: vmovupd   512(<input_0=int64#1),>x0=reg256#7
# asm 2: vmovupd   512(<input_0=%rdi),>x0=%ymm6
vmovupd   512( % rdi), % ymm6

# qhasm: x1 = mem256[ input_0 + 544 ]
# asm 1: vmovupd   544(<input_0=int64#1),>x1=reg256#8
# asm 2: vmovupd   544(<input_0=%rdi),>x1=%ymm7
vmovupd   544( % rdi), % ymm7

# qhasm: x2 = mem256[ input_0 + 576 ]
# asm 1: vmovupd   576(<input_0=int64#1),>x2=reg256#9
# asm 2: vmovupd   576(<input_0=%rdi),>x2=%ymm8
vmovupd   576( % rdi), % ymm8

# qhasm: x3 = mem256[ input_0 + 608 ]
# asm 1: vmovupd   608(<input_0=int64#1),>x3=reg256#10
# asm 2: vmovupd   608(<input_0=%rdi),>x3=%ymm9
vmovupd   608( % rdi), % ymm9

# qhasm: x4 = mem256[ input_0 + 640 ]
# asm 1: vmovupd   640(<input_0=int64#1),>x4=reg256#11
# asm 2: vmovupd   640(<input_0=%rdi),>x4=%ymm10
vmovupd   640( % rdi), % ymm10

# qhasm: x5 = mem256[ input_0 + 672 ]
# asm 1: vmovupd   672(<input_0=int64#1),>x5=reg256#12
# asm 2: vmovupd   672(<input_0=%rdi),>x5=%ymm11
vmovupd   672( % rdi), % ymm11

# qhasm: x6 = mem256[ input_0 + 704 ]
# asm 1: vmovupd   704(<input_0=int64#1),>x6=reg256#13
# asm 2: vmovupd   704(<input_0=%rdi),>x6=%ymm12
vmovupd   704( % rdi), % ymm12

# qhasm: x7 = mem256[ input_0 + 736 ]
# asm 1: vmovupd   736(<input_0=int64#1),>x7=reg256#14
# asm 2: vmovupd   736(<input_0=%rdi),>x7=%ymm13
vmovupd   736( % rdi), % ymm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <x0=reg256#7,<mask0=reg256#1,>v00=reg256#15
# asm 2: vpand <x0=%ymm6,<mask0=%ymm0,>v00=%ymm14
vpand % ymm6, % ymm0, % ymm14

# qhasm: v10 = x4 & mask0
# asm 1: vpand <x4=reg256#11,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x4=%ymm10,<mask0=%ymm0,>v10=%ymm15
vpand % ymm10, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x0 & mask1
# asm 1: vpand <x0=reg256#7,<mask1=reg256#2,>v01=reg256#7
# asm 2: vpand <x0=%ymm6,<mask1=%ymm1,>v01=%ymm6
vpand % ymm6, % ymm1, % ymm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <x4=reg256#11,<mask1=reg256#2,>v11=reg256#11
# asm 2: vpand <x4=%ymm10,<mask1=%ymm1,>v11=%ymm10
vpand % ymm10, % ymm1, % ymm10

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $4,<v01=%ymm6,<v01=%ymm6
vpsrlq $4, % ymm6, % ymm6

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x0=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x0=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x4 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#11,>x4=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm10,>x4=%ymm6
vpor  % ymm6, % ymm10, % ymm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <x1=reg256#8,<mask0=reg256#1,>v00=reg256#11
# asm 2: vpand <x1=%ymm7,<mask0=%ymm0,>v00=%ymm10
vpand % ymm7, % ymm0, % ymm10

# qhasm: v10 = x5 & mask0
# asm 1: vpand <x5=reg256#12,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x5=%ymm11,<mask0=%ymm0,>v10=%ymm15
vpand % ymm11, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x1 & mask1
# asm 1: vpand <x1=reg256#8,<mask1=reg256#2,>v01=reg256#8
# asm 2: vpand <x1=%ymm7,<mask1=%ymm1,>v01=%ymm7
vpand % ymm7, % ymm1, % ymm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <x5=reg256#12,<mask1=reg256#2,>v11=reg256#12
# asm 2: vpand <x5=%ymm11,<mask1=%ymm1,>v11=%ymm11
vpand % ymm11, % ymm1, % ymm11

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#8,<v01=reg256#8
# asm 2: vpsrlq $4,<v01=%ymm7,<v01=%ymm7
vpsrlq $4, % ymm7, % ymm7

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#16,>x1=reg256#11
# asm 2: vpor  <v00=%ymm10,<v10=%ymm15,>x1=%ymm10
vpor  % ymm10, % ymm15, % ymm10

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#12,>x5=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm11,>x5=%ymm7
vpor  % ymm7, % ymm11, % ymm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <x2=reg256#9,<mask0=reg256#1,>v00=reg256#12
# asm 2: vpand <x2=%ymm8,<mask0=%ymm0,>v00=%ymm11
vpand % ymm8, % ymm0, % ymm11

# qhasm: v10 = x6 & mask0
# asm 1: vpand <x6=reg256#13,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x6=%ymm12,<mask0=%ymm0,>v10=%ymm15
vpand % ymm12, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x2 & mask1
# asm 1: vpand <x2=reg256#9,<mask1=reg256#2,>v01=reg256#9
# asm 2: vpand <x2=%ymm8,<mask1=%ymm1,>v01=%ymm8
vpand % ymm8, % ymm1, % ymm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <x6=reg256#13,<mask1=reg256#2,>v11=reg256#13
# asm 2: vpand <x6=%ymm12,<mask1=%ymm1,>v11=%ymm12
vpand % ymm12, % ymm1, % ymm12

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#9,<v01=reg256#9
# asm 2: vpsrlq $4,<v01=%ymm8,<v01=%ymm8
vpsrlq $4, % ymm8, % ymm8

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x2=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x2=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#9,<v11=reg256#13,>x6=reg256#9
# asm 2: vpor  <v01=%ymm8,<v11=%ymm12,>x6=%ymm8
vpor  % ymm8, % ymm12, % ymm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <x3=reg256#10,<mask0=reg256#1,>v00=reg256#13
# asm 2: vpand <x3=%ymm9,<mask0=%ymm0,>v00=%ymm12
vpand % ymm9, % ymm0, % ymm12

# qhasm: v10 = x7 & mask0
# asm 1: vpand <x7=reg256#14,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x7=%ymm13,<mask0=%ymm0,>v10=%ymm15
vpand % ymm13, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x3 & mask1
# asm 1: vpand <x3=reg256#10,<mask1=reg256#2,>v01=reg256#10
# asm 2: vpand <x3=%ymm9,<mask1=%ymm1,>v01=%ymm9
vpand % ymm9, % ymm1, % ymm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <x7=reg256#14,<mask1=reg256#2,>v11=reg256#14
# asm 2: vpand <x7=%ymm13,<mask1=%ymm1,>v11=%ymm13
vpand % ymm13, % ymm1, % ymm13

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#10,<v01=reg256#10
# asm 2: vpsrlq $4,<v01=%ymm9,<v01=%ymm9
vpsrlq $4, % ymm9, % ymm9

# qhasm: x3 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x3=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x3=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#10,<v11=reg256#14,>x7=reg256#10
# asm 2: vpor  <v01=%ymm9,<v11=%ymm13,>x7=%ymm9
vpor  % ymm9, % ymm13, % ymm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <x0=reg256#15,<mask2=reg256#3,>v00=reg256#14
# asm 2: vpand <x0=%ymm14,<mask2=%ymm2,>v00=%ymm13
vpand % ymm14, % ymm2, % ymm13

# qhasm: v10 = x2 & mask2
# asm 1: vpand <x2=reg256#12,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x2=%ymm11,<mask2=%ymm2,>v10=%ymm15
vpand % ymm11, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x0 & mask3
# asm 1: vpand <x0=reg256#15,<mask3=reg256#4,>v01=reg256#15
# asm 2: vpand <x0=%ymm14,<mask3=%ymm3,>v01=%ymm14
vpand % ymm14, % ymm3, % ymm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <x2=reg256#12,<mask3=reg256#4,>v11=reg256#12
# asm 2: vpand <x2=%ymm11,<mask3=%ymm3,>v11=%ymm11
vpand % ymm11, % ymm3, % ymm11

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#15,<v01=reg256#15
# asm 2: vpsrlq $2,<v01=%ymm14,<v01=%ymm14
vpsrlq $2, % ymm14, % ymm14

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#14,<v10=reg256#16,>x0=reg256#14
# asm 2: vpor  <v00=%ymm13,<v10=%ymm15,>x0=%ymm13
vpor  % ymm13, % ymm15, % ymm13

# qhasm: x2 = v01 | v11
# asm 1: vpor  <v01=reg256#15,<v11=reg256#12,>x2=reg256#12
# asm 2: vpor  <v01=%ymm14,<v11=%ymm11,>x2=%ymm11
vpor  % ymm14, % ymm11, % ymm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <x1=reg256#11,<mask2=reg256#3,>v00=reg256#15
# asm 2: vpand <x1=%ymm10,<mask2=%ymm2,>v00=%ymm14
vpand % ymm10, % ymm2, % ymm14

# qhasm: v10 = x3 & mask2
# asm 1: vpand <x3=reg256#13,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x3=%ymm12,<mask2=%ymm2,>v10=%ymm15
vpand % ymm12, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x1 & mask3
# asm 1: vpand <x1=reg256#11,<mask3=reg256#4,>v01=reg256#11
# asm 2: vpand <x1=%ymm10,<mask3=%ymm3,>v01=%ymm10
vpand % ymm10, % ymm3, % ymm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <x3=reg256#13,<mask3=reg256#4,>v11=reg256#13
# asm 2: vpand <x3=%ymm12,<mask3=%ymm3,>v11=%ymm12
vpand % ymm12, % ymm3, % ymm12

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#11,<v01=reg256#11
# asm 2: vpsrlq $2,<v01=%ymm10,<v01=%ymm10
vpsrlq $2, % ymm10, % ymm10

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x1=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x1=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#11,<v11=reg256#13,>x3=reg256#11
# asm 2: vpor  <v01=%ymm10,<v11=%ymm12,>x3=%ymm10
vpor  % ymm10, % ymm12, % ymm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <x4=reg256#7,<mask2=reg256#3,>v00=reg256#13
# asm 2: vpand <x4=%ymm6,<mask2=%ymm2,>v00=%ymm12
vpand % ymm6, % ymm2, % ymm12

# qhasm: v10 = x6 & mask2
# asm 1: vpand <x6=reg256#9,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x6=%ymm8,<mask2=%ymm2,>v10=%ymm15
vpand % ymm8, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x4 & mask3
# asm 1: vpand <x4=reg256#7,<mask3=reg256#4,>v01=reg256#7
# asm 2: vpand <x4=%ymm6,<mask3=%ymm3,>v01=%ymm6
vpand % ymm6, % ymm3, % ymm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <x6=reg256#9,<mask3=reg256#4,>v11=reg256#9
# asm 2: vpand <x6=%ymm8,<mask3=%ymm3,>v11=%ymm8
vpand % ymm8, % ymm3, % ymm8

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $2,<v01=%ymm6,<v01=%ymm6
vpsrlq $2, % ymm6, % ymm6

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x4=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x4=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#9,>x6=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm8,>x6=%ymm6
vpor  % ymm6, % ymm8, % ymm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <x5=reg256#8,<mask2=reg256#3,>v00=reg256#9
# asm 2: vpand <x5=%ymm7,<mask2=%ymm2,>v00=%ymm8
vpand % ymm7, % ymm2, % ymm8

# qhasm: v10 = x7 & mask2
# asm 1: vpand <x7=reg256#10,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x7=%ymm9,<mask2=%ymm2,>v10=%ymm15
vpand % ymm9, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x5 & mask3
# asm 1: vpand <x5=reg256#8,<mask3=reg256#4,>v01=reg256#8
# asm 2: vpand <x5=%ymm7,<mask3=%ymm3,>v01=%ymm7
vpand % ymm7, % ymm3, % ymm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <x7=reg256#10,<mask3=reg256#4,>v11=reg256#10
# asm 2: vpand <x7=%ymm9,<mask3=%ymm3,>v11=%ymm9
vpand % ymm9, % ymm3, % ymm9

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#8,<v01=reg256#8
# asm 2: vpsrlq $2,<v01=%ymm7,<v01=%ymm7
vpsrlq $2, % ymm7, % ymm7

# qhasm: x5 = v00 | v10
# asm 1: vpor  <v00=reg256#9,<v10=reg256#16,>x5=reg256#9
# asm 2: vpor  <v00=%ymm8,<v10=%ymm15,>x5=%ymm8
vpor  % ymm8, % ymm15, % ymm8

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#10,>x7=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm9,>x7=%ymm7
vpor  % ymm7, % ymm9, % ymm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <x0=reg256#14,<mask4=reg256#5,>v00=reg256#10
# asm 2: vpand <x0=%ymm13,<mask4=%ymm4,>v00=%ymm9
vpand % ymm13, % ymm4, % ymm9

# qhasm: v10 = x1 & mask4
# asm 1: vpand <x1=reg256#15,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x1=%ymm14,<mask4=%ymm4,>v10=%ymm15
vpand % ymm14, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x0 & mask5
# asm 1: vpand <x0=reg256#14,<mask5=reg256#6,>v01=reg256#14
# asm 2: vpand <x0=%ymm13,<mask5=%ymm5,>v01=%ymm13
vpand % ymm13, % ymm5, % ymm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <x1=reg256#15,<mask5=reg256#6,>v11=reg256#15
# asm 2: vpand <x1=%ymm14,<mask5=%ymm5,>v11=%ymm14
vpand % ymm14, % ymm5, % ymm14

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#14,<v01=reg256#14
# asm 2: vpsrlq $1,<v01=%ymm13,<v01=%ymm13
vpsrlq $1, % ymm13, % ymm13

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#10,<v10=reg256#16,>x0=reg256#10
# asm 2: vpor  <v00=%ymm9,<v10=%ymm15,>x0=%ymm9
vpor  % ymm9, % ymm15, % ymm9

# qhasm: x1 = v01 | v11
# asm 1: vpor  <v01=reg256#14,<v11=reg256#15,>x1=reg256#14
# asm 2: vpor  <v01=%ymm13,<v11=%ymm14,>x1=%ymm13
vpor  % ymm13, % ymm14, % ymm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <x2=reg256#12,<mask4=reg256#5,>v00=reg256#15
# asm 2: vpand <x2=%ymm11,<mask4=%ymm4,>v00=%ymm14
vpand % ymm11, % ymm4, % ymm14

# qhasm: v10 = x3 & mask4
# asm 1: vpand <x3=reg256#11,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x3=%ymm10,<mask4=%ymm4,>v10=%ymm15
vpand % ymm10, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x2 & mask5
# asm 1: vpand <x2=reg256#12,<mask5=reg256#6,>v01=reg256#12
# asm 2: vpand <x2=%ymm11,<mask5=%ymm5,>v01=%ymm11
vpand % ymm11, % ymm5, % ymm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <x3=reg256#11,<mask5=reg256#6,>v11=reg256#11
# asm 2: vpand <x3=%ymm10,<mask5=%ymm5,>v11=%ymm10
vpand % ymm10, % ymm5, % ymm10

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#12,<v01=reg256#12
# asm 2: vpsrlq $1,<v01=%ymm11,<v01=%ymm11
vpsrlq $1, % ymm11, % ymm11

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x2=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x2=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#12,<v11=reg256#11,>x3=reg256#11
# asm 2: vpor  <v01=%ymm11,<v11=%ymm10,>x3=%ymm10
vpor  % ymm11, % ymm10, % ymm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <x4=reg256#13,<mask4=reg256#5,>v00=reg256#12
# asm 2: vpand <x4=%ymm12,<mask4=%ymm4,>v00=%ymm11
vpand % ymm12, % ymm4, % ymm11

# qhasm: v10 = x5 & mask4
# asm 1: vpand <x5=reg256#9,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x5=%ymm8,<mask4=%ymm4,>v10=%ymm15
vpand % ymm8, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x4 & mask5
# asm 1: vpand <x4=reg256#13,<mask5=reg256#6,>v01=reg256#13
# asm 2: vpand <x4=%ymm12,<mask5=%ymm5,>v01=%ymm12
vpand % ymm12, % ymm5, % ymm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <x5=reg256#9,<mask5=reg256#6,>v11=reg256#9
# asm 2: vpand <x5=%ymm8,<mask5=%ymm5,>v11=%ymm8
vpand % ymm8, % ymm5, % ymm8

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#13,<v01=reg256#13
# asm 2: vpsrlq $1,<v01=%ymm12,<v01=%ymm12
vpsrlq $1, % ymm12, % ymm12

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x4=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x4=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#13,<v11=reg256#9,>x5=reg256#9
# asm 2: vpor  <v01=%ymm12,<v11=%ymm8,>x5=%ymm8
vpor  % ymm12, % ymm8, % ymm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <x6=reg256#7,<mask4=reg256#5,>v00=reg256#13
# asm 2: vpand <x6=%ymm6,<mask4=%ymm4,>v00=%ymm12
vpand % ymm6, % ymm4, % ymm12

# qhasm: v10 = x7 & mask4
# asm 1: vpand <x7=reg256#8,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x7=%ymm7,<mask4=%ymm4,>v10=%ymm15
vpand % ymm7, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x6 & mask5
# asm 1: vpand <x6=reg256#7,<mask5=reg256#6,>v01=reg256#7
# asm 2: vpand <x6=%ymm6,<mask5=%ymm5,>v01=%ymm6
vpand % ymm6, % ymm5, % ymm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <x7=reg256#8,<mask5=reg256#6,>v11=reg256#8
# asm 2: vpand <x7=%ymm7,<mask5=%ymm5,>v11=%ymm7
vpand % ymm7, % ymm5, % ymm7

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $1,<v01=%ymm6,<v01=%ymm6
vpsrlq $1, % ymm6, % ymm6

# qhasm: x6 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x6=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x6=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#8,>x7=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm7,>x7=%ymm6
vpor  % ymm6, % ymm7, % ymm6

# qhasm: mem256[ input_0 + 512 ] = x0
# asm 1: vmovupd   <x0=reg256#10,512(<input_0=int64#1)
# asm 2: vmovupd   <x0=%ymm9,512(<input_0=%rdi)
vmovupd   % ymm9, 512( % rdi)

# qhasm: mem256[ input_0 + 544 ] = x1
# asm 1: vmovupd   <x1=reg256#14,544(<input_0=int64#1)
# asm 2: vmovupd   <x1=%ymm13,544(<input_0=%rdi)
vmovupd   % ymm13, 544( % rdi)

# qhasm: mem256[ input_0 + 576 ] = x2
# asm 1: vmovupd   <x2=reg256#15,576(<input_0=int64#1)
# asm 2: vmovupd   <x2=%ymm14,576(<input_0=%rdi)
vmovupd   % ymm14, 576( % rdi)

# qhasm: mem256[ input_0 + 608 ] = x3
# asm 1: vmovupd   <x3=reg256#11,608(<input_0=int64#1)
# asm 2: vmovupd   <x3=%ymm10,608(<input_0=%rdi)
vmovupd   % ymm10, 608( % rdi)

# qhasm: mem256[ input_0 + 640 ] = x4
# asm 1: vmovupd   <x4=reg256#12,640(<input_0=int64#1)
# asm 2: vmovupd   <x4=%ymm11,640(<input_0=%rdi)
vmovupd   % ymm11, 640( % rdi)

# qhasm: mem256[ input_0 + 672 ] = x5
# asm 1: vmovupd   <x5=reg256#9,672(<input_0=int64#1)
# asm 2: vmovupd   <x5=%ymm8,672(<input_0=%rdi)
vmovupd   % ymm8, 672( % rdi)

# qhasm: mem256[ input_0 + 704 ] = x6
# asm 1: vmovupd   <x6=reg256#13,704(<input_0=int64#1)
# asm 2: vmovupd   <x6=%ymm12,704(<input_0=%rdi)
vmovupd   % ymm12, 704( % rdi)

# qhasm: mem256[ input_0 + 736 ] = x7
# asm 1: vmovupd   <x7=reg256#7,736(<input_0=int64#1)
# asm 2: vmovupd   <x7=%ymm6,736(<input_0=%rdi)
vmovupd   % ymm6, 736( % rdi)

# qhasm: x0 = mem256[ input_0 + 768 ]
# asm 1: vmovupd   768(<input_0=int64#1),>x0=reg256#7
# asm 2: vmovupd   768(<input_0=%rdi),>x0=%ymm6
vmovupd   768( % rdi), % ymm6

# qhasm: x1 = mem256[ input_0 + 800 ]
# asm 1: vmovupd   800(<input_0=int64#1),>x1=reg256#8
# asm 2: vmovupd   800(<input_0=%rdi),>x1=%ymm7
vmovupd   800( % rdi), % ymm7

# qhasm: x2 = mem256[ input_0 + 832 ]
# asm 1: vmovupd   832(<input_0=int64#1),>x2=reg256#9
# asm 2: vmovupd   832(<input_0=%rdi),>x2=%ymm8
vmovupd   832( % rdi), % ymm8

# qhasm: x3 = mem256[ input_0 + 864 ]
# asm 1: vmovupd   864(<input_0=int64#1),>x3=reg256#10
# asm 2: vmovupd   864(<input_0=%rdi),>x3=%ymm9
vmovupd   864( % rdi), % ymm9

# qhasm: x4 = mem256[ input_0 + 896 ]
# asm 1: vmovupd   896(<input_0=int64#1),>x4=reg256#11
# asm 2: vmovupd   896(<input_0=%rdi),>x4=%ymm10
vmovupd   896( % rdi), % ymm10

# qhasm: x5 = mem256[ input_0 + 928 ]
# asm 1: vmovupd   928(<input_0=int64#1),>x5=reg256#12
# asm 2: vmovupd   928(<input_0=%rdi),>x5=%ymm11
vmovupd   928( % rdi), % ymm11

# qhasm: x6 = mem256[ input_0 + 960 ]
# asm 1: vmovupd   960(<input_0=int64#1),>x6=reg256#13
# asm 2: vmovupd   960(<input_0=%rdi),>x6=%ymm12
vmovupd   960( % rdi), % ymm12

# qhasm: x7 = mem256[ input_0 + 992 ]
# asm 1: vmovupd   992(<input_0=int64#1),>x7=reg256#14
# asm 2: vmovupd   992(<input_0=%rdi),>x7=%ymm13
vmovupd   992( % rdi), % ymm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <x0=reg256#7,<mask0=reg256#1,>v00=reg256#15
# asm 2: vpand <x0=%ymm6,<mask0=%ymm0,>v00=%ymm14
vpand % ymm6, % ymm0, % ymm14

# qhasm: v10 = x4 & mask0
# asm 1: vpand <x4=reg256#11,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x4=%ymm10,<mask0=%ymm0,>v10=%ymm15
vpand % ymm10, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x0 & mask1
# asm 1: vpand <x0=reg256#7,<mask1=reg256#2,>v01=reg256#7
# asm 2: vpand <x0=%ymm6,<mask1=%ymm1,>v01=%ymm6
vpand % ymm6, % ymm1, % ymm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <x4=reg256#11,<mask1=reg256#2,>v11=reg256#11
# asm 2: vpand <x4=%ymm10,<mask1=%ymm1,>v11=%ymm10
vpand % ymm10, % ymm1, % ymm10

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $4,<v01=%ymm6,<v01=%ymm6
vpsrlq $4, % ymm6, % ymm6

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x0=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x0=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x4 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#11,>x4=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm10,>x4=%ymm6
vpor  % ymm6, % ymm10, % ymm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <x1=reg256#8,<mask0=reg256#1,>v00=reg256#11
# asm 2: vpand <x1=%ymm7,<mask0=%ymm0,>v00=%ymm10
vpand % ymm7, % ymm0, % ymm10

# qhasm: v10 = x5 & mask0
# asm 1: vpand <x5=reg256#12,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x5=%ymm11,<mask0=%ymm0,>v10=%ymm15
vpand % ymm11, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x1 & mask1
# asm 1: vpand <x1=reg256#8,<mask1=reg256#2,>v01=reg256#8
# asm 2: vpand <x1=%ymm7,<mask1=%ymm1,>v01=%ymm7
vpand % ymm7, % ymm1, % ymm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <x5=reg256#12,<mask1=reg256#2,>v11=reg256#12
# asm 2: vpand <x5=%ymm11,<mask1=%ymm1,>v11=%ymm11
vpand % ymm11, % ymm1, % ymm11

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#8,<v01=reg256#8
# asm 2: vpsrlq $4,<v01=%ymm7,<v01=%ymm7
vpsrlq $4, % ymm7, % ymm7

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#16,>x1=reg256#11
# asm 2: vpor  <v00=%ymm10,<v10=%ymm15,>x1=%ymm10
vpor  % ymm10, % ymm15, % ymm10

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#12,>x5=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm11,>x5=%ymm7
vpor  % ymm7, % ymm11, % ymm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <x2=reg256#9,<mask0=reg256#1,>v00=reg256#12
# asm 2: vpand <x2=%ymm8,<mask0=%ymm0,>v00=%ymm11
vpand % ymm8, % ymm0, % ymm11

# qhasm: v10 = x6 & mask0
# asm 1: vpand <x6=reg256#13,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x6=%ymm12,<mask0=%ymm0,>v10=%ymm15
vpand % ymm12, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x2 & mask1
# asm 1: vpand <x2=reg256#9,<mask1=reg256#2,>v01=reg256#9
# asm 2: vpand <x2=%ymm8,<mask1=%ymm1,>v01=%ymm8
vpand % ymm8, % ymm1, % ymm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <x6=reg256#13,<mask1=reg256#2,>v11=reg256#13
# asm 2: vpand <x6=%ymm12,<mask1=%ymm1,>v11=%ymm12
vpand % ymm12, % ymm1, % ymm12

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#9,<v01=reg256#9
# asm 2: vpsrlq $4,<v01=%ymm8,<v01=%ymm8
vpsrlq $4, % ymm8, % ymm8

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x2=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x2=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#9,<v11=reg256#13,>x6=reg256#9
# asm 2: vpor  <v01=%ymm8,<v11=%ymm12,>x6=%ymm8
vpor  % ymm8, % ymm12, % ymm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <x3=reg256#10,<mask0=reg256#1,>v00=reg256#13
# asm 2: vpand <x3=%ymm9,<mask0=%ymm0,>v00=%ymm12
vpand % ymm9, % ymm0, % ymm12

# qhasm: v10 = x7 & mask0
# asm 1: vpand <x7=reg256#14,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x7=%ymm13,<mask0=%ymm0,>v10=%ymm15
vpand % ymm13, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x3 & mask1
# asm 1: vpand <x3=reg256#10,<mask1=reg256#2,>v01=reg256#10
# asm 2: vpand <x3=%ymm9,<mask1=%ymm1,>v01=%ymm9
vpand % ymm9, % ymm1, % ymm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <x7=reg256#14,<mask1=reg256#2,>v11=reg256#14
# asm 2: vpand <x7=%ymm13,<mask1=%ymm1,>v11=%ymm13
vpand % ymm13, % ymm1, % ymm13

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#10,<v01=reg256#10
# asm 2: vpsrlq $4,<v01=%ymm9,<v01=%ymm9
vpsrlq $4, % ymm9, % ymm9

# qhasm: x3 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x3=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x3=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#10,<v11=reg256#14,>x7=reg256#10
# asm 2: vpor  <v01=%ymm9,<v11=%ymm13,>x7=%ymm9
vpor  % ymm9, % ymm13, % ymm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <x0=reg256#15,<mask2=reg256#3,>v00=reg256#14
# asm 2: vpand <x0=%ymm14,<mask2=%ymm2,>v00=%ymm13
vpand % ymm14, % ymm2, % ymm13

# qhasm: v10 = x2 & mask2
# asm 1: vpand <x2=reg256#12,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x2=%ymm11,<mask2=%ymm2,>v10=%ymm15
vpand % ymm11, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x0 & mask3
# asm 1: vpand <x0=reg256#15,<mask3=reg256#4,>v01=reg256#15
# asm 2: vpand <x0=%ymm14,<mask3=%ymm3,>v01=%ymm14
vpand % ymm14, % ymm3, % ymm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <x2=reg256#12,<mask3=reg256#4,>v11=reg256#12
# asm 2: vpand <x2=%ymm11,<mask3=%ymm3,>v11=%ymm11
vpand % ymm11, % ymm3, % ymm11

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#15,<v01=reg256#15
# asm 2: vpsrlq $2,<v01=%ymm14,<v01=%ymm14
vpsrlq $2, % ymm14, % ymm14

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#14,<v10=reg256#16,>x0=reg256#14
# asm 2: vpor  <v00=%ymm13,<v10=%ymm15,>x0=%ymm13
vpor  % ymm13, % ymm15, % ymm13

# qhasm: x2 = v01 | v11
# asm 1: vpor  <v01=reg256#15,<v11=reg256#12,>x2=reg256#12
# asm 2: vpor  <v01=%ymm14,<v11=%ymm11,>x2=%ymm11
vpor  % ymm14, % ymm11, % ymm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <x1=reg256#11,<mask2=reg256#3,>v00=reg256#15
# asm 2: vpand <x1=%ymm10,<mask2=%ymm2,>v00=%ymm14
vpand % ymm10, % ymm2, % ymm14

# qhasm: v10 = x3 & mask2
# asm 1: vpand <x3=reg256#13,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x3=%ymm12,<mask2=%ymm2,>v10=%ymm15
vpand % ymm12, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x1 & mask3
# asm 1: vpand <x1=reg256#11,<mask3=reg256#4,>v01=reg256#11
# asm 2: vpand <x1=%ymm10,<mask3=%ymm3,>v01=%ymm10
vpand % ymm10, % ymm3, % ymm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <x3=reg256#13,<mask3=reg256#4,>v11=reg256#13
# asm 2: vpand <x3=%ymm12,<mask3=%ymm3,>v11=%ymm12
vpand % ymm12, % ymm3, % ymm12

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#11,<v01=reg256#11
# asm 2: vpsrlq $2,<v01=%ymm10,<v01=%ymm10
vpsrlq $2, % ymm10, % ymm10

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x1=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x1=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#11,<v11=reg256#13,>x3=reg256#11
# asm 2: vpor  <v01=%ymm10,<v11=%ymm12,>x3=%ymm10
vpor  % ymm10, % ymm12, % ymm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <x4=reg256#7,<mask2=reg256#3,>v00=reg256#13
# asm 2: vpand <x4=%ymm6,<mask2=%ymm2,>v00=%ymm12
vpand % ymm6, % ymm2, % ymm12

# qhasm: v10 = x6 & mask2
# asm 1: vpand <x6=reg256#9,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x6=%ymm8,<mask2=%ymm2,>v10=%ymm15
vpand % ymm8, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x4 & mask3
# asm 1: vpand <x4=reg256#7,<mask3=reg256#4,>v01=reg256#7
# asm 2: vpand <x4=%ymm6,<mask3=%ymm3,>v01=%ymm6
vpand % ymm6, % ymm3, % ymm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <x6=reg256#9,<mask3=reg256#4,>v11=reg256#9
# asm 2: vpand <x6=%ymm8,<mask3=%ymm3,>v11=%ymm8
vpand % ymm8, % ymm3, % ymm8

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $2,<v01=%ymm6,<v01=%ymm6
vpsrlq $2, % ymm6, % ymm6

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x4=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x4=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#9,>x6=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm8,>x6=%ymm6
vpor  % ymm6, % ymm8, % ymm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <x5=reg256#8,<mask2=reg256#3,>v00=reg256#9
# asm 2: vpand <x5=%ymm7,<mask2=%ymm2,>v00=%ymm8
vpand % ymm7, % ymm2, % ymm8

# qhasm: v10 = x7 & mask2
# asm 1: vpand <x7=reg256#10,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x7=%ymm9,<mask2=%ymm2,>v10=%ymm15
vpand % ymm9, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x5 & mask3
# asm 1: vpand <x5=reg256#8,<mask3=reg256#4,>v01=reg256#8
# asm 2: vpand <x5=%ymm7,<mask3=%ymm3,>v01=%ymm7
vpand % ymm7, % ymm3, % ymm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <x7=reg256#10,<mask3=reg256#4,>v11=reg256#10
# asm 2: vpand <x7=%ymm9,<mask3=%ymm3,>v11=%ymm9
vpand % ymm9, % ymm3, % ymm9

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#8,<v01=reg256#8
# asm 2: vpsrlq $2,<v01=%ymm7,<v01=%ymm7
vpsrlq $2, % ymm7, % ymm7

# qhasm: x5 = v00 | v10
# asm 1: vpor  <v00=reg256#9,<v10=reg256#16,>x5=reg256#9
# asm 2: vpor  <v00=%ymm8,<v10=%ymm15,>x5=%ymm8
vpor  % ymm8, % ymm15, % ymm8

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#10,>x7=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm9,>x7=%ymm7
vpor  % ymm7, % ymm9, % ymm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <x0=reg256#14,<mask4=reg256#5,>v00=reg256#10
# asm 2: vpand <x0=%ymm13,<mask4=%ymm4,>v00=%ymm9
vpand % ymm13, % ymm4, % ymm9

# qhasm: v10 = x1 & mask4
# asm 1: vpand <x1=reg256#15,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x1=%ymm14,<mask4=%ymm4,>v10=%ymm15
vpand % ymm14, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x0 & mask5
# asm 1: vpand <x0=reg256#14,<mask5=reg256#6,>v01=reg256#14
# asm 2: vpand <x0=%ymm13,<mask5=%ymm5,>v01=%ymm13
vpand % ymm13, % ymm5, % ymm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <x1=reg256#15,<mask5=reg256#6,>v11=reg256#15
# asm 2: vpand <x1=%ymm14,<mask5=%ymm5,>v11=%ymm14
vpand % ymm14, % ymm5, % ymm14

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#14,<v01=reg256#14
# asm 2: vpsrlq $1,<v01=%ymm13,<v01=%ymm13
vpsrlq $1, % ymm13, % ymm13

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#10,<v10=reg256#16,>x0=reg256#10
# asm 2: vpor  <v00=%ymm9,<v10=%ymm15,>x0=%ymm9
vpor  % ymm9, % ymm15, % ymm9

# qhasm: x1 = v01 | v11
# asm 1: vpor  <v01=reg256#14,<v11=reg256#15,>x1=reg256#14
# asm 2: vpor  <v01=%ymm13,<v11=%ymm14,>x1=%ymm13
vpor  % ymm13, % ymm14, % ymm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <x2=reg256#12,<mask4=reg256#5,>v00=reg256#15
# asm 2: vpand <x2=%ymm11,<mask4=%ymm4,>v00=%ymm14
vpand % ymm11, % ymm4, % ymm14

# qhasm: v10 = x3 & mask4
# asm 1: vpand <x3=reg256#11,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x3=%ymm10,<mask4=%ymm4,>v10=%ymm15
vpand % ymm10, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x2 & mask5
# asm 1: vpand <x2=reg256#12,<mask5=reg256#6,>v01=reg256#12
# asm 2: vpand <x2=%ymm11,<mask5=%ymm5,>v01=%ymm11
vpand % ymm11, % ymm5, % ymm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <x3=reg256#11,<mask5=reg256#6,>v11=reg256#11
# asm 2: vpand <x3=%ymm10,<mask5=%ymm5,>v11=%ymm10
vpand % ymm10, % ymm5, % ymm10

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#12,<v01=reg256#12
# asm 2: vpsrlq $1,<v01=%ymm11,<v01=%ymm11
vpsrlq $1, % ymm11, % ymm11

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x2=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x2=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#12,<v11=reg256#11,>x3=reg256#11
# asm 2: vpor  <v01=%ymm11,<v11=%ymm10,>x3=%ymm10
vpor  % ymm11, % ymm10, % ymm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <x4=reg256#13,<mask4=reg256#5,>v00=reg256#12
# asm 2: vpand <x4=%ymm12,<mask4=%ymm4,>v00=%ymm11
vpand % ymm12, % ymm4, % ymm11

# qhasm: v10 = x5 & mask4
# asm 1: vpand <x5=reg256#9,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x5=%ymm8,<mask4=%ymm4,>v10=%ymm15
vpand % ymm8, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x4 & mask5
# asm 1: vpand <x4=reg256#13,<mask5=reg256#6,>v01=reg256#13
# asm 2: vpand <x4=%ymm12,<mask5=%ymm5,>v01=%ymm12
vpand % ymm12, % ymm5, % ymm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <x5=reg256#9,<mask5=reg256#6,>v11=reg256#9
# asm 2: vpand <x5=%ymm8,<mask5=%ymm5,>v11=%ymm8
vpand % ymm8, % ymm5, % ymm8

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#13,<v01=reg256#13
# asm 2: vpsrlq $1,<v01=%ymm12,<v01=%ymm12
vpsrlq $1, % ymm12, % ymm12

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x4=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x4=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#13,<v11=reg256#9,>x5=reg256#9
# asm 2: vpor  <v01=%ymm12,<v11=%ymm8,>x5=%ymm8
vpor  % ymm12, % ymm8, % ymm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <x6=reg256#7,<mask4=reg256#5,>v00=reg256#13
# asm 2: vpand <x6=%ymm6,<mask4=%ymm4,>v00=%ymm12
vpand % ymm6, % ymm4, % ymm12

# qhasm: v10 = x7 & mask4
# asm 1: vpand <x7=reg256#8,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x7=%ymm7,<mask4=%ymm4,>v10=%ymm15
vpand % ymm7, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x6 & mask5
# asm 1: vpand <x6=reg256#7,<mask5=reg256#6,>v01=reg256#7
# asm 2: vpand <x6=%ymm6,<mask5=%ymm5,>v01=%ymm6
vpand % ymm6, % ymm5, % ymm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <x7=reg256#8,<mask5=reg256#6,>v11=reg256#8
# asm 2: vpand <x7=%ymm7,<mask5=%ymm5,>v11=%ymm7
vpand % ymm7, % ymm5, % ymm7

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $1,<v01=%ymm6,<v01=%ymm6
vpsrlq $1, % ymm6, % ymm6

# qhasm: x6 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x6=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x6=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#8,>x7=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm7,>x7=%ymm6
vpor  % ymm6, % ymm7, % ymm6

# qhasm: mem256[ input_0 + 768 ] = x0
# asm 1: vmovupd   <x0=reg256#10,768(<input_0=int64#1)
# asm 2: vmovupd   <x0=%ymm9,768(<input_0=%rdi)
vmovupd   % ymm9, 768( % rdi)

# qhasm: mem256[ input_0 + 800 ] = x1
# asm 1: vmovupd   <x1=reg256#14,800(<input_0=int64#1)
# asm 2: vmovupd   <x1=%ymm13,800(<input_0=%rdi)
vmovupd   % ymm13, 800( % rdi)

# qhasm: mem256[ input_0 + 832 ] = x2
# asm 1: vmovupd   <x2=reg256#15,832(<input_0=int64#1)
# asm 2: vmovupd   <x2=%ymm14,832(<input_0=%rdi)
vmovupd   % ymm14, 832( % rdi)

# qhasm: mem256[ input_0 + 864 ] = x3
# asm 1: vmovupd   <x3=reg256#11,864(<input_0=int64#1)
# asm 2: vmovupd   <x3=%ymm10,864(<input_0=%rdi)
vmovupd   % ymm10, 864( % rdi)

# qhasm: mem256[ input_0 + 896 ] = x4
# asm 1: vmovupd   <x4=reg256#12,896(<input_0=int64#1)
# asm 2: vmovupd   <x4=%ymm11,896(<input_0=%rdi)
vmovupd   % ymm11, 896( % rdi)

# qhasm: mem256[ input_0 + 928 ] = x5
# asm 1: vmovupd   <x5=reg256#9,928(<input_0=int64#1)
# asm 2: vmovupd   <x5=%ymm8,928(<input_0=%rdi)
vmovupd   % ymm8, 928( % rdi)

# qhasm: mem256[ input_0 + 960 ] = x6
# asm 1: vmovupd   <x6=reg256#13,960(<input_0=int64#1)
# asm 2: vmovupd   <x6=%ymm12,960(<input_0=%rdi)
vmovupd   % ymm12, 960( % rdi)

# qhasm: mem256[ input_0 + 992 ] = x7
# asm 1: vmovupd   <x7=reg256#7,992(<input_0=int64#1)
# asm 2: vmovupd   <x7=%ymm6,992(<input_0=%rdi)
vmovupd   % ymm6, 992( % rdi)

# qhasm: x0 = mem256[ input_0 + 1024 ]
# asm 1: vmovupd   1024(<input_0=int64#1),>x0=reg256#7
# asm 2: vmovupd   1024(<input_0=%rdi),>x0=%ymm6
vmovupd   1024( % rdi), % ymm6

# qhasm: x1 = mem256[ input_0 + 1056 ]
# asm 1: vmovupd   1056(<input_0=int64#1),>x1=reg256#8
# asm 2: vmovupd   1056(<input_0=%rdi),>x1=%ymm7
vmovupd   1056( % rdi), % ymm7

# qhasm: x2 = mem256[ input_0 + 1088 ]
# asm 1: vmovupd   1088(<input_0=int64#1),>x2=reg256#9
# asm 2: vmovupd   1088(<input_0=%rdi),>x2=%ymm8
vmovupd   1088( % rdi), % ymm8

# qhasm: x3 = mem256[ input_0 + 1120 ]
# asm 1: vmovupd   1120(<input_0=int64#1),>x3=reg256#10
# asm 2: vmovupd   1120(<input_0=%rdi),>x3=%ymm9
vmovupd   1120( % rdi), % ymm9

# qhasm: x4 = mem256[ input_0 + 1152 ]
# asm 1: vmovupd   1152(<input_0=int64#1),>x4=reg256#11
# asm 2: vmovupd   1152(<input_0=%rdi),>x4=%ymm10
vmovupd   1152( % rdi), % ymm10

# qhasm: x5 = mem256[ input_0 + 1184 ]
# asm 1: vmovupd   1184(<input_0=int64#1),>x5=reg256#12
# asm 2: vmovupd   1184(<input_0=%rdi),>x5=%ymm11
vmovupd   1184( % rdi), % ymm11

# qhasm: x6 = mem256[ input_0 + 1216 ]
# asm 1: vmovupd   1216(<input_0=int64#1),>x6=reg256#13
# asm 2: vmovupd   1216(<input_0=%rdi),>x6=%ymm12
vmovupd   1216( % rdi), % ymm12

# qhasm: x7 = mem256[ input_0 + 1248 ]
# asm 1: vmovupd   1248(<input_0=int64#1),>x7=reg256#14
# asm 2: vmovupd   1248(<input_0=%rdi),>x7=%ymm13
vmovupd   1248( % rdi), % ymm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <x0=reg256#7,<mask0=reg256#1,>v00=reg256#15
# asm 2: vpand <x0=%ymm6,<mask0=%ymm0,>v00=%ymm14
vpand % ymm6, % ymm0, % ymm14

# qhasm: v10 = x4 & mask0
# asm 1: vpand <x4=reg256#11,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x4=%ymm10,<mask0=%ymm0,>v10=%ymm15
vpand % ymm10, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x0 & mask1
# asm 1: vpand <x0=reg256#7,<mask1=reg256#2,>v01=reg256#7
# asm 2: vpand <x0=%ymm6,<mask1=%ymm1,>v01=%ymm6
vpand % ymm6, % ymm1, % ymm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <x4=reg256#11,<mask1=reg256#2,>v11=reg256#11
# asm 2: vpand <x4=%ymm10,<mask1=%ymm1,>v11=%ymm10
vpand % ymm10, % ymm1, % ymm10

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $4,<v01=%ymm6,<v01=%ymm6
vpsrlq $4, % ymm6, % ymm6

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x0=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x0=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x4 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#11,>x4=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm10,>x4=%ymm6
vpor  % ymm6, % ymm10, % ymm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <x1=reg256#8,<mask0=reg256#1,>v00=reg256#11
# asm 2: vpand <x1=%ymm7,<mask0=%ymm0,>v00=%ymm10
vpand % ymm7, % ymm0, % ymm10

# qhasm: v10 = x5 & mask0
# asm 1: vpand <x5=reg256#12,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x5=%ymm11,<mask0=%ymm0,>v10=%ymm15
vpand % ymm11, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x1 & mask1
# asm 1: vpand <x1=reg256#8,<mask1=reg256#2,>v01=reg256#8
# asm 2: vpand <x1=%ymm7,<mask1=%ymm1,>v01=%ymm7
vpand % ymm7, % ymm1, % ymm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <x5=reg256#12,<mask1=reg256#2,>v11=reg256#12
# asm 2: vpand <x5=%ymm11,<mask1=%ymm1,>v11=%ymm11
vpand % ymm11, % ymm1, % ymm11

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#8,<v01=reg256#8
# asm 2: vpsrlq $4,<v01=%ymm7,<v01=%ymm7
vpsrlq $4, % ymm7, % ymm7

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#16,>x1=reg256#11
# asm 2: vpor  <v00=%ymm10,<v10=%ymm15,>x1=%ymm10
vpor  % ymm10, % ymm15, % ymm10

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#12,>x5=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm11,>x5=%ymm7
vpor  % ymm7, % ymm11, % ymm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <x2=reg256#9,<mask0=reg256#1,>v00=reg256#12
# asm 2: vpand <x2=%ymm8,<mask0=%ymm0,>v00=%ymm11
vpand % ymm8, % ymm0, % ymm11

# qhasm: v10 = x6 & mask0
# asm 1: vpand <x6=reg256#13,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x6=%ymm12,<mask0=%ymm0,>v10=%ymm15
vpand % ymm12, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x2 & mask1
# asm 1: vpand <x2=reg256#9,<mask1=reg256#2,>v01=reg256#9
# asm 2: vpand <x2=%ymm8,<mask1=%ymm1,>v01=%ymm8
vpand % ymm8, % ymm1, % ymm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <x6=reg256#13,<mask1=reg256#2,>v11=reg256#13
# asm 2: vpand <x6=%ymm12,<mask1=%ymm1,>v11=%ymm12
vpand % ymm12, % ymm1, % ymm12

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#9,<v01=reg256#9
# asm 2: vpsrlq $4,<v01=%ymm8,<v01=%ymm8
vpsrlq $4, % ymm8, % ymm8

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x2=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x2=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#9,<v11=reg256#13,>x6=reg256#9
# asm 2: vpor  <v01=%ymm8,<v11=%ymm12,>x6=%ymm8
vpor  % ymm8, % ymm12, % ymm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <x3=reg256#10,<mask0=reg256#1,>v00=reg256#13
# asm 2: vpand <x3=%ymm9,<mask0=%ymm0,>v00=%ymm12
vpand % ymm9, % ymm0, % ymm12

# qhasm: v10 = x7 & mask0
# asm 1: vpand <x7=reg256#14,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x7=%ymm13,<mask0=%ymm0,>v10=%ymm15
vpand % ymm13, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x3 & mask1
# asm 1: vpand <x3=reg256#10,<mask1=reg256#2,>v01=reg256#10
# asm 2: vpand <x3=%ymm9,<mask1=%ymm1,>v01=%ymm9
vpand % ymm9, % ymm1, % ymm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <x7=reg256#14,<mask1=reg256#2,>v11=reg256#14
# asm 2: vpand <x7=%ymm13,<mask1=%ymm1,>v11=%ymm13
vpand % ymm13, % ymm1, % ymm13

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#10,<v01=reg256#10
# asm 2: vpsrlq $4,<v01=%ymm9,<v01=%ymm9
vpsrlq $4, % ymm9, % ymm9

# qhasm: x3 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x3=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x3=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#10,<v11=reg256#14,>x7=reg256#10
# asm 2: vpor  <v01=%ymm9,<v11=%ymm13,>x7=%ymm9
vpor  % ymm9, % ymm13, % ymm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <x0=reg256#15,<mask2=reg256#3,>v00=reg256#14
# asm 2: vpand <x0=%ymm14,<mask2=%ymm2,>v00=%ymm13
vpand % ymm14, % ymm2, % ymm13

# qhasm: v10 = x2 & mask2
# asm 1: vpand <x2=reg256#12,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x2=%ymm11,<mask2=%ymm2,>v10=%ymm15
vpand % ymm11, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x0 & mask3
# asm 1: vpand <x0=reg256#15,<mask3=reg256#4,>v01=reg256#15
# asm 2: vpand <x0=%ymm14,<mask3=%ymm3,>v01=%ymm14
vpand % ymm14, % ymm3, % ymm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <x2=reg256#12,<mask3=reg256#4,>v11=reg256#12
# asm 2: vpand <x2=%ymm11,<mask3=%ymm3,>v11=%ymm11
vpand % ymm11, % ymm3, % ymm11

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#15,<v01=reg256#15
# asm 2: vpsrlq $2,<v01=%ymm14,<v01=%ymm14
vpsrlq $2, % ymm14, % ymm14

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#14,<v10=reg256#16,>x0=reg256#14
# asm 2: vpor  <v00=%ymm13,<v10=%ymm15,>x0=%ymm13
vpor  % ymm13, % ymm15, % ymm13

# qhasm: x2 = v01 | v11
# asm 1: vpor  <v01=reg256#15,<v11=reg256#12,>x2=reg256#12
# asm 2: vpor  <v01=%ymm14,<v11=%ymm11,>x2=%ymm11
vpor  % ymm14, % ymm11, % ymm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <x1=reg256#11,<mask2=reg256#3,>v00=reg256#15
# asm 2: vpand <x1=%ymm10,<mask2=%ymm2,>v00=%ymm14
vpand % ymm10, % ymm2, % ymm14

# qhasm: v10 = x3 & mask2
# asm 1: vpand <x3=reg256#13,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x3=%ymm12,<mask2=%ymm2,>v10=%ymm15
vpand % ymm12, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x1 & mask3
# asm 1: vpand <x1=reg256#11,<mask3=reg256#4,>v01=reg256#11
# asm 2: vpand <x1=%ymm10,<mask3=%ymm3,>v01=%ymm10
vpand % ymm10, % ymm3, % ymm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <x3=reg256#13,<mask3=reg256#4,>v11=reg256#13
# asm 2: vpand <x3=%ymm12,<mask3=%ymm3,>v11=%ymm12
vpand % ymm12, % ymm3, % ymm12

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#11,<v01=reg256#11
# asm 2: vpsrlq $2,<v01=%ymm10,<v01=%ymm10
vpsrlq $2, % ymm10, % ymm10

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x1=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x1=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#11,<v11=reg256#13,>x3=reg256#11
# asm 2: vpor  <v01=%ymm10,<v11=%ymm12,>x3=%ymm10
vpor  % ymm10, % ymm12, % ymm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <x4=reg256#7,<mask2=reg256#3,>v00=reg256#13
# asm 2: vpand <x4=%ymm6,<mask2=%ymm2,>v00=%ymm12
vpand % ymm6, % ymm2, % ymm12

# qhasm: v10 = x6 & mask2
# asm 1: vpand <x6=reg256#9,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x6=%ymm8,<mask2=%ymm2,>v10=%ymm15
vpand % ymm8, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x4 & mask3
# asm 1: vpand <x4=reg256#7,<mask3=reg256#4,>v01=reg256#7
# asm 2: vpand <x4=%ymm6,<mask3=%ymm3,>v01=%ymm6
vpand % ymm6, % ymm3, % ymm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <x6=reg256#9,<mask3=reg256#4,>v11=reg256#9
# asm 2: vpand <x6=%ymm8,<mask3=%ymm3,>v11=%ymm8
vpand % ymm8, % ymm3, % ymm8

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $2,<v01=%ymm6,<v01=%ymm6
vpsrlq $2, % ymm6, % ymm6

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x4=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x4=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#9,>x6=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm8,>x6=%ymm6
vpor  % ymm6, % ymm8, % ymm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <x5=reg256#8,<mask2=reg256#3,>v00=reg256#9
# asm 2: vpand <x5=%ymm7,<mask2=%ymm2,>v00=%ymm8
vpand % ymm7, % ymm2, % ymm8

# qhasm: v10 = x7 & mask2
# asm 1: vpand <x7=reg256#10,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x7=%ymm9,<mask2=%ymm2,>v10=%ymm15
vpand % ymm9, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x5 & mask3
# asm 1: vpand <x5=reg256#8,<mask3=reg256#4,>v01=reg256#8
# asm 2: vpand <x5=%ymm7,<mask3=%ymm3,>v01=%ymm7
vpand % ymm7, % ymm3, % ymm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <x7=reg256#10,<mask3=reg256#4,>v11=reg256#10
# asm 2: vpand <x7=%ymm9,<mask3=%ymm3,>v11=%ymm9
vpand % ymm9, % ymm3, % ymm9

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#8,<v01=reg256#8
# asm 2: vpsrlq $2,<v01=%ymm7,<v01=%ymm7
vpsrlq $2, % ymm7, % ymm7

# qhasm: x5 = v00 | v10
# asm 1: vpor  <v00=reg256#9,<v10=reg256#16,>x5=reg256#9
# asm 2: vpor  <v00=%ymm8,<v10=%ymm15,>x5=%ymm8
vpor  % ymm8, % ymm15, % ymm8

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#10,>x7=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm9,>x7=%ymm7
vpor  % ymm7, % ymm9, % ymm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <x0=reg256#14,<mask4=reg256#5,>v00=reg256#10
# asm 2: vpand <x0=%ymm13,<mask4=%ymm4,>v00=%ymm9
vpand % ymm13, % ymm4, % ymm9

# qhasm: v10 = x1 & mask4
# asm 1: vpand <x1=reg256#15,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x1=%ymm14,<mask4=%ymm4,>v10=%ymm15
vpand % ymm14, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x0 & mask5
# asm 1: vpand <x0=reg256#14,<mask5=reg256#6,>v01=reg256#14
# asm 2: vpand <x0=%ymm13,<mask5=%ymm5,>v01=%ymm13
vpand % ymm13, % ymm5, % ymm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <x1=reg256#15,<mask5=reg256#6,>v11=reg256#15
# asm 2: vpand <x1=%ymm14,<mask5=%ymm5,>v11=%ymm14
vpand % ymm14, % ymm5, % ymm14

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#14,<v01=reg256#14
# asm 2: vpsrlq $1,<v01=%ymm13,<v01=%ymm13
vpsrlq $1, % ymm13, % ymm13

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#10,<v10=reg256#16,>x0=reg256#10
# asm 2: vpor  <v00=%ymm9,<v10=%ymm15,>x0=%ymm9
vpor  % ymm9, % ymm15, % ymm9

# qhasm: x1 = v01 | v11
# asm 1: vpor  <v01=reg256#14,<v11=reg256#15,>x1=reg256#14
# asm 2: vpor  <v01=%ymm13,<v11=%ymm14,>x1=%ymm13
vpor  % ymm13, % ymm14, % ymm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <x2=reg256#12,<mask4=reg256#5,>v00=reg256#15
# asm 2: vpand <x2=%ymm11,<mask4=%ymm4,>v00=%ymm14
vpand % ymm11, % ymm4, % ymm14

# qhasm: v10 = x3 & mask4
# asm 1: vpand <x3=reg256#11,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x3=%ymm10,<mask4=%ymm4,>v10=%ymm15
vpand % ymm10, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x2 & mask5
# asm 1: vpand <x2=reg256#12,<mask5=reg256#6,>v01=reg256#12
# asm 2: vpand <x2=%ymm11,<mask5=%ymm5,>v01=%ymm11
vpand % ymm11, % ymm5, % ymm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <x3=reg256#11,<mask5=reg256#6,>v11=reg256#11
# asm 2: vpand <x3=%ymm10,<mask5=%ymm5,>v11=%ymm10
vpand % ymm10, % ymm5, % ymm10

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#12,<v01=reg256#12
# asm 2: vpsrlq $1,<v01=%ymm11,<v01=%ymm11
vpsrlq $1, % ymm11, % ymm11

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x2=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x2=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#12,<v11=reg256#11,>x3=reg256#11
# asm 2: vpor  <v01=%ymm11,<v11=%ymm10,>x3=%ymm10
vpor  % ymm11, % ymm10, % ymm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <x4=reg256#13,<mask4=reg256#5,>v00=reg256#12
# asm 2: vpand <x4=%ymm12,<mask4=%ymm4,>v00=%ymm11
vpand % ymm12, % ymm4, % ymm11

# qhasm: v10 = x5 & mask4
# asm 1: vpand <x5=reg256#9,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x5=%ymm8,<mask4=%ymm4,>v10=%ymm15
vpand % ymm8, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x4 & mask5
# asm 1: vpand <x4=reg256#13,<mask5=reg256#6,>v01=reg256#13
# asm 2: vpand <x4=%ymm12,<mask5=%ymm5,>v01=%ymm12
vpand % ymm12, % ymm5, % ymm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <x5=reg256#9,<mask5=reg256#6,>v11=reg256#9
# asm 2: vpand <x5=%ymm8,<mask5=%ymm5,>v11=%ymm8
vpand % ymm8, % ymm5, % ymm8

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#13,<v01=reg256#13
# asm 2: vpsrlq $1,<v01=%ymm12,<v01=%ymm12
vpsrlq $1, % ymm12, % ymm12

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x4=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x4=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#13,<v11=reg256#9,>x5=reg256#9
# asm 2: vpor  <v01=%ymm12,<v11=%ymm8,>x5=%ymm8
vpor  % ymm12, % ymm8, % ymm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <x6=reg256#7,<mask4=reg256#5,>v00=reg256#13
# asm 2: vpand <x6=%ymm6,<mask4=%ymm4,>v00=%ymm12
vpand % ymm6, % ymm4, % ymm12

# qhasm: v10 = x7 & mask4
# asm 1: vpand <x7=reg256#8,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x7=%ymm7,<mask4=%ymm4,>v10=%ymm15
vpand % ymm7, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x6 & mask5
# asm 1: vpand <x6=reg256#7,<mask5=reg256#6,>v01=reg256#7
# asm 2: vpand <x6=%ymm6,<mask5=%ymm5,>v01=%ymm6
vpand % ymm6, % ymm5, % ymm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <x7=reg256#8,<mask5=reg256#6,>v11=reg256#8
# asm 2: vpand <x7=%ymm7,<mask5=%ymm5,>v11=%ymm7
vpand % ymm7, % ymm5, % ymm7

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $1,<v01=%ymm6,<v01=%ymm6
vpsrlq $1, % ymm6, % ymm6

# qhasm: x6 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x6=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x6=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#8,>x7=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm7,>x7=%ymm6
vpor  % ymm6, % ymm7, % ymm6

# qhasm: mem256[ input_0 + 1024 ] = x0
# asm 1: vmovupd   <x0=reg256#10,1024(<input_0=int64#1)
# asm 2: vmovupd   <x0=%ymm9,1024(<input_0=%rdi)
vmovupd   % ymm9, 1024( % rdi)

# qhasm: mem256[ input_0 + 1056 ] = x1
# asm 1: vmovupd   <x1=reg256#14,1056(<input_0=int64#1)
# asm 2: vmovupd   <x1=%ymm13,1056(<input_0=%rdi)
vmovupd   % ymm13, 1056( % rdi)

# qhasm: mem256[ input_0 + 1088 ] = x2
# asm 1: vmovupd   <x2=reg256#15,1088(<input_0=int64#1)
# asm 2: vmovupd   <x2=%ymm14,1088(<input_0=%rdi)
vmovupd   % ymm14, 1088( % rdi)

# qhasm: mem256[ input_0 + 1120 ] = x3
# asm 1: vmovupd   <x3=reg256#11,1120(<input_0=int64#1)
# asm 2: vmovupd   <x3=%ymm10,1120(<input_0=%rdi)
vmovupd   % ymm10, 1120( % rdi)

# qhasm: mem256[ input_0 + 1152 ] = x4
# asm 1: vmovupd   <x4=reg256#12,1152(<input_0=int64#1)
# asm 2: vmovupd   <x4=%ymm11,1152(<input_0=%rdi)
vmovupd   % ymm11, 1152( % rdi)

# qhasm: mem256[ input_0 + 1184 ] = x5
# asm 1: vmovupd   <x5=reg256#9,1184(<input_0=int64#1)
# asm 2: vmovupd   <x5=%ymm8,1184(<input_0=%rdi)
vmovupd   % ymm8, 1184( % rdi)

# qhasm: mem256[ input_0 + 1216 ] = x6
# asm 1: vmovupd   <x6=reg256#13,1216(<input_0=int64#1)
# asm 2: vmovupd   <x6=%ymm12,1216(<input_0=%rdi)
vmovupd   % ymm12, 1216( % rdi)

# qhasm: mem256[ input_0 + 1248 ] = x7
# asm 1: vmovupd   <x7=reg256#7,1248(<input_0=int64#1)
# asm 2: vmovupd   <x7=%ymm6,1248(<input_0=%rdi)
vmovupd   % ymm6, 1248( % rdi)

# qhasm: x0 = mem256[ input_0 + 1280 ]
# asm 1: vmovupd   1280(<input_0=int64#1),>x0=reg256#7
# asm 2: vmovupd   1280(<input_0=%rdi),>x0=%ymm6
vmovupd   1280( % rdi), % ymm6

# qhasm: x1 = mem256[ input_0 + 1312 ]
# asm 1: vmovupd   1312(<input_0=int64#1),>x1=reg256#8
# asm 2: vmovupd   1312(<input_0=%rdi),>x1=%ymm7
vmovupd   1312( % rdi), % ymm7

# qhasm: x2 = mem256[ input_0 + 1344 ]
# asm 1: vmovupd   1344(<input_0=int64#1),>x2=reg256#9
# asm 2: vmovupd   1344(<input_0=%rdi),>x2=%ymm8
vmovupd   1344( % rdi), % ymm8

# qhasm: x3 = mem256[ input_0 + 1376 ]
# asm 1: vmovupd   1376(<input_0=int64#1),>x3=reg256#10
# asm 2: vmovupd   1376(<input_0=%rdi),>x3=%ymm9
vmovupd   1376( % rdi), % ymm9

# qhasm: x4 = mem256[ input_0 + 1408 ]
# asm 1: vmovupd   1408(<input_0=int64#1),>x4=reg256#11
# asm 2: vmovupd   1408(<input_0=%rdi),>x4=%ymm10
vmovupd   1408( % rdi), % ymm10

# qhasm: x5 = mem256[ input_0 + 1440 ]
# asm 1: vmovupd   1440(<input_0=int64#1),>x5=reg256#12
# asm 2: vmovupd   1440(<input_0=%rdi),>x5=%ymm11
vmovupd   1440( % rdi), % ymm11

# qhasm: x6 = mem256[ input_0 + 1472 ]
# asm 1: vmovupd   1472(<input_0=int64#1),>x6=reg256#13
# asm 2: vmovupd   1472(<input_0=%rdi),>x6=%ymm12
vmovupd   1472( % rdi), % ymm12

# qhasm: x7 = mem256[ input_0 + 1504 ]
# asm 1: vmovupd   1504(<input_0=int64#1),>x7=reg256#14
# asm 2: vmovupd   1504(<input_0=%rdi),>x7=%ymm13
vmovupd   1504( % rdi), % ymm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <x0=reg256#7,<mask0=reg256#1,>v00=reg256#15
# asm 2: vpand <x0=%ymm6,<mask0=%ymm0,>v00=%ymm14
vpand % ymm6, % ymm0, % ymm14

# qhasm: v10 = x4 & mask0
# asm 1: vpand <x4=reg256#11,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x4=%ymm10,<mask0=%ymm0,>v10=%ymm15
vpand % ymm10, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x0 & mask1
# asm 1: vpand <x0=reg256#7,<mask1=reg256#2,>v01=reg256#7
# asm 2: vpand <x0=%ymm6,<mask1=%ymm1,>v01=%ymm6
vpand % ymm6, % ymm1, % ymm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <x4=reg256#11,<mask1=reg256#2,>v11=reg256#11
# asm 2: vpand <x4=%ymm10,<mask1=%ymm1,>v11=%ymm10
vpand % ymm10, % ymm1, % ymm10

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $4,<v01=%ymm6,<v01=%ymm6
vpsrlq $4, % ymm6, % ymm6

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x0=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x0=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x4 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#11,>x4=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm10,>x4=%ymm6
vpor  % ymm6, % ymm10, % ymm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <x1=reg256#8,<mask0=reg256#1,>v00=reg256#11
# asm 2: vpand <x1=%ymm7,<mask0=%ymm0,>v00=%ymm10
vpand % ymm7, % ymm0, % ymm10

# qhasm: v10 = x5 & mask0
# asm 1: vpand <x5=reg256#12,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x5=%ymm11,<mask0=%ymm0,>v10=%ymm15
vpand % ymm11, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x1 & mask1
# asm 1: vpand <x1=reg256#8,<mask1=reg256#2,>v01=reg256#8
# asm 2: vpand <x1=%ymm7,<mask1=%ymm1,>v01=%ymm7
vpand % ymm7, % ymm1, % ymm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <x5=reg256#12,<mask1=reg256#2,>v11=reg256#12
# asm 2: vpand <x5=%ymm11,<mask1=%ymm1,>v11=%ymm11
vpand % ymm11, % ymm1, % ymm11

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#8,<v01=reg256#8
# asm 2: vpsrlq $4,<v01=%ymm7,<v01=%ymm7
vpsrlq $4, % ymm7, % ymm7

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#16,>x1=reg256#11
# asm 2: vpor  <v00=%ymm10,<v10=%ymm15,>x1=%ymm10
vpor  % ymm10, % ymm15, % ymm10

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#12,>x5=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm11,>x5=%ymm7
vpor  % ymm7, % ymm11, % ymm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <x2=reg256#9,<mask0=reg256#1,>v00=reg256#12
# asm 2: vpand <x2=%ymm8,<mask0=%ymm0,>v00=%ymm11
vpand % ymm8, % ymm0, % ymm11

# qhasm: v10 = x6 & mask0
# asm 1: vpand <x6=reg256#13,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x6=%ymm12,<mask0=%ymm0,>v10=%ymm15
vpand % ymm12, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x2 & mask1
# asm 1: vpand <x2=reg256#9,<mask1=reg256#2,>v01=reg256#9
# asm 2: vpand <x2=%ymm8,<mask1=%ymm1,>v01=%ymm8
vpand % ymm8, % ymm1, % ymm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <x6=reg256#13,<mask1=reg256#2,>v11=reg256#13
# asm 2: vpand <x6=%ymm12,<mask1=%ymm1,>v11=%ymm12
vpand % ymm12, % ymm1, % ymm12

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#9,<v01=reg256#9
# asm 2: vpsrlq $4,<v01=%ymm8,<v01=%ymm8
vpsrlq $4, % ymm8, % ymm8

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x2=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x2=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#9,<v11=reg256#13,>x6=reg256#9
# asm 2: vpor  <v01=%ymm8,<v11=%ymm12,>x6=%ymm8
vpor  % ymm8, % ymm12, % ymm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <x3=reg256#10,<mask0=reg256#1,>v00=reg256#13
# asm 2: vpand <x3=%ymm9,<mask0=%ymm0,>v00=%ymm12
vpand % ymm9, % ymm0, % ymm12

# qhasm: v10 = x7 & mask0
# asm 1: vpand <x7=reg256#14,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x7=%ymm13,<mask0=%ymm0,>v10=%ymm15
vpand % ymm13, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x3 & mask1
# asm 1: vpand <x3=reg256#10,<mask1=reg256#2,>v01=reg256#10
# asm 2: vpand <x3=%ymm9,<mask1=%ymm1,>v01=%ymm9
vpand % ymm9, % ymm1, % ymm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <x7=reg256#14,<mask1=reg256#2,>v11=reg256#14
# asm 2: vpand <x7=%ymm13,<mask1=%ymm1,>v11=%ymm13
vpand % ymm13, % ymm1, % ymm13

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#10,<v01=reg256#10
# asm 2: vpsrlq $4,<v01=%ymm9,<v01=%ymm9
vpsrlq $4, % ymm9, % ymm9

# qhasm: x3 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x3=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x3=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#10,<v11=reg256#14,>x7=reg256#10
# asm 2: vpor  <v01=%ymm9,<v11=%ymm13,>x7=%ymm9
vpor  % ymm9, % ymm13, % ymm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <x0=reg256#15,<mask2=reg256#3,>v00=reg256#14
# asm 2: vpand <x0=%ymm14,<mask2=%ymm2,>v00=%ymm13
vpand % ymm14, % ymm2, % ymm13

# qhasm: v10 = x2 & mask2
# asm 1: vpand <x2=reg256#12,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x2=%ymm11,<mask2=%ymm2,>v10=%ymm15
vpand % ymm11, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x0 & mask3
# asm 1: vpand <x0=reg256#15,<mask3=reg256#4,>v01=reg256#15
# asm 2: vpand <x0=%ymm14,<mask3=%ymm3,>v01=%ymm14
vpand % ymm14, % ymm3, % ymm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <x2=reg256#12,<mask3=reg256#4,>v11=reg256#12
# asm 2: vpand <x2=%ymm11,<mask3=%ymm3,>v11=%ymm11
vpand % ymm11, % ymm3, % ymm11

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#15,<v01=reg256#15
# asm 2: vpsrlq $2,<v01=%ymm14,<v01=%ymm14
vpsrlq $2, % ymm14, % ymm14

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#14,<v10=reg256#16,>x0=reg256#14
# asm 2: vpor  <v00=%ymm13,<v10=%ymm15,>x0=%ymm13
vpor  % ymm13, % ymm15, % ymm13

# qhasm: x2 = v01 | v11
# asm 1: vpor  <v01=reg256#15,<v11=reg256#12,>x2=reg256#12
# asm 2: vpor  <v01=%ymm14,<v11=%ymm11,>x2=%ymm11
vpor  % ymm14, % ymm11, % ymm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <x1=reg256#11,<mask2=reg256#3,>v00=reg256#15
# asm 2: vpand <x1=%ymm10,<mask2=%ymm2,>v00=%ymm14
vpand % ymm10, % ymm2, % ymm14

# qhasm: v10 = x3 & mask2
# asm 1: vpand <x3=reg256#13,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x3=%ymm12,<mask2=%ymm2,>v10=%ymm15
vpand % ymm12, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x1 & mask3
# asm 1: vpand <x1=reg256#11,<mask3=reg256#4,>v01=reg256#11
# asm 2: vpand <x1=%ymm10,<mask3=%ymm3,>v01=%ymm10
vpand % ymm10, % ymm3, % ymm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <x3=reg256#13,<mask3=reg256#4,>v11=reg256#13
# asm 2: vpand <x3=%ymm12,<mask3=%ymm3,>v11=%ymm12
vpand % ymm12, % ymm3, % ymm12

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#11,<v01=reg256#11
# asm 2: vpsrlq $2,<v01=%ymm10,<v01=%ymm10
vpsrlq $2, % ymm10, % ymm10

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x1=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x1=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#11,<v11=reg256#13,>x3=reg256#11
# asm 2: vpor  <v01=%ymm10,<v11=%ymm12,>x3=%ymm10
vpor  % ymm10, % ymm12, % ymm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <x4=reg256#7,<mask2=reg256#3,>v00=reg256#13
# asm 2: vpand <x4=%ymm6,<mask2=%ymm2,>v00=%ymm12
vpand % ymm6, % ymm2, % ymm12

# qhasm: v10 = x6 & mask2
# asm 1: vpand <x6=reg256#9,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x6=%ymm8,<mask2=%ymm2,>v10=%ymm15
vpand % ymm8, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x4 & mask3
# asm 1: vpand <x4=reg256#7,<mask3=reg256#4,>v01=reg256#7
# asm 2: vpand <x4=%ymm6,<mask3=%ymm3,>v01=%ymm6
vpand % ymm6, % ymm3, % ymm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <x6=reg256#9,<mask3=reg256#4,>v11=reg256#9
# asm 2: vpand <x6=%ymm8,<mask3=%ymm3,>v11=%ymm8
vpand % ymm8, % ymm3, % ymm8

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $2,<v01=%ymm6,<v01=%ymm6
vpsrlq $2, % ymm6, % ymm6

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x4=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x4=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#9,>x6=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm8,>x6=%ymm6
vpor  % ymm6, % ymm8, % ymm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <x5=reg256#8,<mask2=reg256#3,>v00=reg256#9
# asm 2: vpand <x5=%ymm7,<mask2=%ymm2,>v00=%ymm8
vpand % ymm7, % ymm2, % ymm8

# qhasm: v10 = x7 & mask2
# asm 1: vpand <x7=reg256#10,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x7=%ymm9,<mask2=%ymm2,>v10=%ymm15
vpand % ymm9, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x5 & mask3
# asm 1: vpand <x5=reg256#8,<mask3=reg256#4,>v01=reg256#8
# asm 2: vpand <x5=%ymm7,<mask3=%ymm3,>v01=%ymm7
vpand % ymm7, % ymm3, % ymm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <x7=reg256#10,<mask3=reg256#4,>v11=reg256#10
# asm 2: vpand <x7=%ymm9,<mask3=%ymm3,>v11=%ymm9
vpand % ymm9, % ymm3, % ymm9

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#8,<v01=reg256#8
# asm 2: vpsrlq $2,<v01=%ymm7,<v01=%ymm7
vpsrlq $2, % ymm7, % ymm7

# qhasm: x5 = v00 | v10
# asm 1: vpor  <v00=reg256#9,<v10=reg256#16,>x5=reg256#9
# asm 2: vpor  <v00=%ymm8,<v10=%ymm15,>x5=%ymm8
vpor  % ymm8, % ymm15, % ymm8

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#10,>x7=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm9,>x7=%ymm7
vpor  % ymm7, % ymm9, % ymm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <x0=reg256#14,<mask4=reg256#5,>v00=reg256#10
# asm 2: vpand <x0=%ymm13,<mask4=%ymm4,>v00=%ymm9
vpand % ymm13, % ymm4, % ymm9

# qhasm: v10 = x1 & mask4
# asm 1: vpand <x1=reg256#15,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x1=%ymm14,<mask4=%ymm4,>v10=%ymm15
vpand % ymm14, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x0 & mask5
# asm 1: vpand <x0=reg256#14,<mask5=reg256#6,>v01=reg256#14
# asm 2: vpand <x0=%ymm13,<mask5=%ymm5,>v01=%ymm13
vpand % ymm13, % ymm5, % ymm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <x1=reg256#15,<mask5=reg256#6,>v11=reg256#15
# asm 2: vpand <x1=%ymm14,<mask5=%ymm5,>v11=%ymm14
vpand % ymm14, % ymm5, % ymm14

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#14,<v01=reg256#14
# asm 2: vpsrlq $1,<v01=%ymm13,<v01=%ymm13
vpsrlq $1, % ymm13, % ymm13

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#10,<v10=reg256#16,>x0=reg256#10
# asm 2: vpor  <v00=%ymm9,<v10=%ymm15,>x0=%ymm9
vpor  % ymm9, % ymm15, % ymm9

# qhasm: x1 = v01 | v11
# asm 1: vpor  <v01=reg256#14,<v11=reg256#15,>x1=reg256#14
# asm 2: vpor  <v01=%ymm13,<v11=%ymm14,>x1=%ymm13
vpor  % ymm13, % ymm14, % ymm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <x2=reg256#12,<mask4=reg256#5,>v00=reg256#15
# asm 2: vpand <x2=%ymm11,<mask4=%ymm4,>v00=%ymm14
vpand % ymm11, % ymm4, % ymm14

# qhasm: v10 = x3 & mask4
# asm 1: vpand <x3=reg256#11,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x3=%ymm10,<mask4=%ymm4,>v10=%ymm15
vpand % ymm10, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x2 & mask5
# asm 1: vpand <x2=reg256#12,<mask5=reg256#6,>v01=reg256#12
# asm 2: vpand <x2=%ymm11,<mask5=%ymm5,>v01=%ymm11
vpand % ymm11, % ymm5, % ymm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <x3=reg256#11,<mask5=reg256#6,>v11=reg256#11
# asm 2: vpand <x3=%ymm10,<mask5=%ymm5,>v11=%ymm10
vpand % ymm10, % ymm5, % ymm10

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#12,<v01=reg256#12
# asm 2: vpsrlq $1,<v01=%ymm11,<v01=%ymm11
vpsrlq $1, % ymm11, % ymm11

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x2=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x2=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#12,<v11=reg256#11,>x3=reg256#11
# asm 2: vpor  <v01=%ymm11,<v11=%ymm10,>x3=%ymm10
vpor  % ymm11, % ymm10, % ymm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <x4=reg256#13,<mask4=reg256#5,>v00=reg256#12
# asm 2: vpand <x4=%ymm12,<mask4=%ymm4,>v00=%ymm11
vpand % ymm12, % ymm4, % ymm11

# qhasm: v10 = x5 & mask4
# asm 1: vpand <x5=reg256#9,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x5=%ymm8,<mask4=%ymm4,>v10=%ymm15
vpand % ymm8, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x4 & mask5
# asm 1: vpand <x4=reg256#13,<mask5=reg256#6,>v01=reg256#13
# asm 2: vpand <x4=%ymm12,<mask5=%ymm5,>v01=%ymm12
vpand % ymm12, % ymm5, % ymm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <x5=reg256#9,<mask5=reg256#6,>v11=reg256#9
# asm 2: vpand <x5=%ymm8,<mask5=%ymm5,>v11=%ymm8
vpand % ymm8, % ymm5, % ymm8

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#13,<v01=reg256#13
# asm 2: vpsrlq $1,<v01=%ymm12,<v01=%ymm12
vpsrlq $1, % ymm12, % ymm12

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x4=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x4=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#13,<v11=reg256#9,>x5=reg256#9
# asm 2: vpor  <v01=%ymm12,<v11=%ymm8,>x5=%ymm8
vpor  % ymm12, % ymm8, % ymm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <x6=reg256#7,<mask4=reg256#5,>v00=reg256#13
# asm 2: vpand <x6=%ymm6,<mask4=%ymm4,>v00=%ymm12
vpand % ymm6, % ymm4, % ymm12

# qhasm: v10 = x7 & mask4
# asm 1: vpand <x7=reg256#8,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x7=%ymm7,<mask4=%ymm4,>v10=%ymm15
vpand % ymm7, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x6 & mask5
# asm 1: vpand <x6=reg256#7,<mask5=reg256#6,>v01=reg256#7
# asm 2: vpand <x6=%ymm6,<mask5=%ymm5,>v01=%ymm6
vpand % ymm6, % ymm5, % ymm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <x7=reg256#8,<mask5=reg256#6,>v11=reg256#8
# asm 2: vpand <x7=%ymm7,<mask5=%ymm5,>v11=%ymm7
vpand % ymm7, % ymm5, % ymm7

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $1,<v01=%ymm6,<v01=%ymm6
vpsrlq $1, % ymm6, % ymm6

# qhasm: x6 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x6=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x6=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#8,>x7=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm7,>x7=%ymm6
vpor  % ymm6, % ymm7, % ymm6

# qhasm: mem256[ input_0 + 1280 ] = x0
# asm 1: vmovupd   <x0=reg256#10,1280(<input_0=int64#1)
# asm 2: vmovupd   <x0=%ymm9,1280(<input_0=%rdi)
vmovupd   % ymm9, 1280( % rdi)

# qhasm: mem256[ input_0 + 1312 ] = x1
# asm 1: vmovupd   <x1=reg256#14,1312(<input_0=int64#1)
# asm 2: vmovupd   <x1=%ymm13,1312(<input_0=%rdi)
vmovupd   % ymm13, 1312( % rdi)

# qhasm: mem256[ input_0 + 1344 ] = x2
# asm 1: vmovupd   <x2=reg256#15,1344(<input_0=int64#1)
# asm 2: vmovupd   <x2=%ymm14,1344(<input_0=%rdi)
vmovupd   % ymm14, 1344( % rdi)

# qhasm: mem256[ input_0 + 1376 ] = x3
# asm 1: vmovupd   <x3=reg256#11,1376(<input_0=int64#1)
# asm 2: vmovupd   <x3=%ymm10,1376(<input_0=%rdi)
vmovupd   % ymm10, 1376( % rdi)

# qhasm: mem256[ input_0 + 1408 ] = x4
# asm 1: vmovupd   <x4=reg256#12,1408(<input_0=int64#1)
# asm 2: vmovupd   <x4=%ymm11,1408(<input_0=%rdi)
vmovupd   % ymm11, 1408( % rdi)

# qhasm: mem256[ input_0 + 1440 ] = x5
# asm 1: vmovupd   <x5=reg256#9,1440(<input_0=int64#1)
# asm 2: vmovupd   <x5=%ymm8,1440(<input_0=%rdi)
vmovupd   % ymm8, 1440( % rdi)

# qhasm: mem256[ input_0 + 1472 ] = x6
# asm 1: vmovupd   <x6=reg256#13,1472(<input_0=int64#1)
# asm 2: vmovupd   <x6=%ymm12,1472(<input_0=%rdi)
vmovupd   % ymm12, 1472( % rdi)

# qhasm: mem256[ input_0 + 1504 ] = x7
# asm 1: vmovupd   <x7=reg256#7,1504(<input_0=int64#1)
# asm 2: vmovupd   <x7=%ymm6,1504(<input_0=%rdi)
vmovupd   % ymm6, 1504( % rdi)

# qhasm: x0 = mem256[ input_0 + 1536 ]
# asm 1: vmovupd   1536(<input_0=int64#1),>x0=reg256#7
# asm 2: vmovupd   1536(<input_0=%rdi),>x0=%ymm6
vmovupd   1536( % rdi), % ymm6

# qhasm: x1 = mem256[ input_0 + 1568 ]
# asm 1: vmovupd   1568(<input_0=int64#1),>x1=reg256#8
# asm 2: vmovupd   1568(<input_0=%rdi),>x1=%ymm7
vmovupd   1568( % rdi), % ymm7

# qhasm: x2 = mem256[ input_0 + 1600 ]
# asm 1: vmovupd   1600(<input_0=int64#1),>x2=reg256#9
# asm 2: vmovupd   1600(<input_0=%rdi),>x2=%ymm8
vmovupd   1600( % rdi), % ymm8

# qhasm: x3 = mem256[ input_0 + 1632 ]
# asm 1: vmovupd   1632(<input_0=int64#1),>x3=reg256#10
# asm 2: vmovupd   1632(<input_0=%rdi),>x3=%ymm9
vmovupd   1632( % rdi), % ymm9

# qhasm: x4 = mem256[ input_0 + 1664 ]
# asm 1: vmovupd   1664(<input_0=int64#1),>x4=reg256#11
# asm 2: vmovupd   1664(<input_0=%rdi),>x4=%ymm10
vmovupd   1664( % rdi), % ymm10

# qhasm: x5 = mem256[ input_0 + 1696 ]
# asm 1: vmovupd   1696(<input_0=int64#1),>x5=reg256#12
# asm 2: vmovupd   1696(<input_0=%rdi),>x5=%ymm11
vmovupd   1696( % rdi), % ymm11

# qhasm: x6 = mem256[ input_0 + 1728 ]
# asm 1: vmovupd   1728(<input_0=int64#1),>x6=reg256#13
# asm 2: vmovupd   1728(<input_0=%rdi),>x6=%ymm12
vmovupd   1728( % rdi), % ymm12

# qhasm: x7 = mem256[ input_0 + 1760 ]
# asm 1: vmovupd   1760(<input_0=int64#1),>x7=reg256#14
# asm 2: vmovupd   1760(<input_0=%rdi),>x7=%ymm13
vmovupd   1760( % rdi), % ymm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <x0=reg256#7,<mask0=reg256#1,>v00=reg256#15
# asm 2: vpand <x0=%ymm6,<mask0=%ymm0,>v00=%ymm14
vpand % ymm6, % ymm0, % ymm14

# qhasm: v10 = x4 & mask0
# asm 1: vpand <x4=reg256#11,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x4=%ymm10,<mask0=%ymm0,>v10=%ymm15
vpand % ymm10, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x0 & mask1
# asm 1: vpand <x0=reg256#7,<mask1=reg256#2,>v01=reg256#7
# asm 2: vpand <x0=%ymm6,<mask1=%ymm1,>v01=%ymm6
vpand % ymm6, % ymm1, % ymm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <x4=reg256#11,<mask1=reg256#2,>v11=reg256#11
# asm 2: vpand <x4=%ymm10,<mask1=%ymm1,>v11=%ymm10
vpand % ymm10, % ymm1, % ymm10

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $4,<v01=%ymm6,<v01=%ymm6
vpsrlq $4, % ymm6, % ymm6

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x0=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x0=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x4 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#11,>x4=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm10,>x4=%ymm6
vpor  % ymm6, % ymm10, % ymm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <x1=reg256#8,<mask0=reg256#1,>v00=reg256#11
# asm 2: vpand <x1=%ymm7,<mask0=%ymm0,>v00=%ymm10
vpand % ymm7, % ymm0, % ymm10

# qhasm: v10 = x5 & mask0
# asm 1: vpand <x5=reg256#12,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x5=%ymm11,<mask0=%ymm0,>v10=%ymm15
vpand % ymm11, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x1 & mask1
# asm 1: vpand <x1=reg256#8,<mask1=reg256#2,>v01=reg256#8
# asm 2: vpand <x1=%ymm7,<mask1=%ymm1,>v01=%ymm7
vpand % ymm7, % ymm1, % ymm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <x5=reg256#12,<mask1=reg256#2,>v11=reg256#12
# asm 2: vpand <x5=%ymm11,<mask1=%ymm1,>v11=%ymm11
vpand % ymm11, % ymm1, % ymm11

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#8,<v01=reg256#8
# asm 2: vpsrlq $4,<v01=%ymm7,<v01=%ymm7
vpsrlq $4, % ymm7, % ymm7

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#16,>x1=reg256#11
# asm 2: vpor  <v00=%ymm10,<v10=%ymm15,>x1=%ymm10
vpor  % ymm10, % ymm15, % ymm10

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#12,>x5=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm11,>x5=%ymm7
vpor  % ymm7, % ymm11, % ymm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <x2=reg256#9,<mask0=reg256#1,>v00=reg256#12
# asm 2: vpand <x2=%ymm8,<mask0=%ymm0,>v00=%ymm11
vpand % ymm8, % ymm0, % ymm11

# qhasm: v10 = x6 & mask0
# asm 1: vpand <x6=reg256#13,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x6=%ymm12,<mask0=%ymm0,>v10=%ymm15
vpand % ymm12, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x2 & mask1
# asm 1: vpand <x2=reg256#9,<mask1=reg256#2,>v01=reg256#9
# asm 2: vpand <x2=%ymm8,<mask1=%ymm1,>v01=%ymm8
vpand % ymm8, % ymm1, % ymm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <x6=reg256#13,<mask1=reg256#2,>v11=reg256#13
# asm 2: vpand <x6=%ymm12,<mask1=%ymm1,>v11=%ymm12
vpand % ymm12, % ymm1, % ymm12

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#9,<v01=reg256#9
# asm 2: vpsrlq $4,<v01=%ymm8,<v01=%ymm8
vpsrlq $4, % ymm8, % ymm8

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x2=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x2=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#9,<v11=reg256#13,>x6=reg256#9
# asm 2: vpor  <v01=%ymm8,<v11=%ymm12,>x6=%ymm8
vpor  % ymm8, % ymm12, % ymm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <x3=reg256#10,<mask0=reg256#1,>v00=reg256#13
# asm 2: vpand <x3=%ymm9,<mask0=%ymm0,>v00=%ymm12
vpand % ymm9, % ymm0, % ymm12

# qhasm: v10 = x7 & mask0
# asm 1: vpand <x7=reg256#14,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x7=%ymm13,<mask0=%ymm0,>v10=%ymm15
vpand % ymm13, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x3 & mask1
# asm 1: vpand <x3=reg256#10,<mask1=reg256#2,>v01=reg256#10
# asm 2: vpand <x3=%ymm9,<mask1=%ymm1,>v01=%ymm9
vpand % ymm9, % ymm1, % ymm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <x7=reg256#14,<mask1=reg256#2,>v11=reg256#14
# asm 2: vpand <x7=%ymm13,<mask1=%ymm1,>v11=%ymm13
vpand % ymm13, % ymm1, % ymm13

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#10,<v01=reg256#10
# asm 2: vpsrlq $4,<v01=%ymm9,<v01=%ymm9
vpsrlq $4, % ymm9, % ymm9

# qhasm: x3 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x3=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x3=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#10,<v11=reg256#14,>x7=reg256#10
# asm 2: vpor  <v01=%ymm9,<v11=%ymm13,>x7=%ymm9
vpor  % ymm9, % ymm13, % ymm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <x0=reg256#15,<mask2=reg256#3,>v00=reg256#14
# asm 2: vpand <x0=%ymm14,<mask2=%ymm2,>v00=%ymm13
vpand % ymm14, % ymm2, % ymm13

# qhasm: v10 = x2 & mask2
# asm 1: vpand <x2=reg256#12,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x2=%ymm11,<mask2=%ymm2,>v10=%ymm15
vpand % ymm11, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x0 & mask3
# asm 1: vpand <x0=reg256#15,<mask3=reg256#4,>v01=reg256#15
# asm 2: vpand <x0=%ymm14,<mask3=%ymm3,>v01=%ymm14
vpand % ymm14, % ymm3, % ymm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <x2=reg256#12,<mask3=reg256#4,>v11=reg256#12
# asm 2: vpand <x2=%ymm11,<mask3=%ymm3,>v11=%ymm11
vpand % ymm11, % ymm3, % ymm11

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#15,<v01=reg256#15
# asm 2: vpsrlq $2,<v01=%ymm14,<v01=%ymm14
vpsrlq $2, % ymm14, % ymm14

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#14,<v10=reg256#16,>x0=reg256#14
# asm 2: vpor  <v00=%ymm13,<v10=%ymm15,>x0=%ymm13
vpor  % ymm13, % ymm15, % ymm13

# qhasm: x2 = v01 | v11
# asm 1: vpor  <v01=reg256#15,<v11=reg256#12,>x2=reg256#12
# asm 2: vpor  <v01=%ymm14,<v11=%ymm11,>x2=%ymm11
vpor  % ymm14, % ymm11, % ymm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <x1=reg256#11,<mask2=reg256#3,>v00=reg256#15
# asm 2: vpand <x1=%ymm10,<mask2=%ymm2,>v00=%ymm14
vpand % ymm10, % ymm2, % ymm14

# qhasm: v10 = x3 & mask2
# asm 1: vpand <x3=reg256#13,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x3=%ymm12,<mask2=%ymm2,>v10=%ymm15
vpand % ymm12, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x1 & mask3
# asm 1: vpand <x1=reg256#11,<mask3=reg256#4,>v01=reg256#11
# asm 2: vpand <x1=%ymm10,<mask3=%ymm3,>v01=%ymm10
vpand % ymm10, % ymm3, % ymm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <x3=reg256#13,<mask3=reg256#4,>v11=reg256#13
# asm 2: vpand <x3=%ymm12,<mask3=%ymm3,>v11=%ymm12
vpand % ymm12, % ymm3, % ymm12

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#11,<v01=reg256#11
# asm 2: vpsrlq $2,<v01=%ymm10,<v01=%ymm10
vpsrlq $2, % ymm10, % ymm10

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x1=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x1=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#11,<v11=reg256#13,>x3=reg256#11
# asm 2: vpor  <v01=%ymm10,<v11=%ymm12,>x3=%ymm10
vpor  % ymm10, % ymm12, % ymm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <x4=reg256#7,<mask2=reg256#3,>v00=reg256#13
# asm 2: vpand <x4=%ymm6,<mask2=%ymm2,>v00=%ymm12
vpand % ymm6, % ymm2, % ymm12

# qhasm: v10 = x6 & mask2
# asm 1: vpand <x6=reg256#9,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x6=%ymm8,<mask2=%ymm2,>v10=%ymm15
vpand % ymm8, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x4 & mask3
# asm 1: vpand <x4=reg256#7,<mask3=reg256#4,>v01=reg256#7
# asm 2: vpand <x4=%ymm6,<mask3=%ymm3,>v01=%ymm6
vpand % ymm6, % ymm3, % ymm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <x6=reg256#9,<mask3=reg256#4,>v11=reg256#9
# asm 2: vpand <x6=%ymm8,<mask3=%ymm3,>v11=%ymm8
vpand % ymm8, % ymm3, % ymm8

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $2,<v01=%ymm6,<v01=%ymm6
vpsrlq $2, % ymm6, % ymm6

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x4=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x4=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#9,>x6=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm8,>x6=%ymm6
vpor  % ymm6, % ymm8, % ymm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <x5=reg256#8,<mask2=reg256#3,>v00=reg256#9
# asm 2: vpand <x5=%ymm7,<mask2=%ymm2,>v00=%ymm8
vpand % ymm7, % ymm2, % ymm8

# qhasm: v10 = x7 & mask2
# asm 1: vpand <x7=reg256#10,<mask2=reg256#3,>v10=reg256#16
# asm 2: vpand <x7=%ymm9,<mask2=%ymm2,>v10=%ymm15
vpand % ymm9, % ymm2, % ymm15

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $2,<v10=%ymm15,<v10=%ymm15
vpsllq $2, % ymm15, % ymm15

# qhasm: v01 = x5 & mask3
# asm 1: vpand <x5=reg256#8,<mask3=reg256#4,>v01=reg256#8
# asm 2: vpand <x5=%ymm7,<mask3=%ymm3,>v01=%ymm7
vpand % ymm7, % ymm3, % ymm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <x7=reg256#10,<mask3=reg256#4,>v11=reg256#10
# asm 2: vpand <x7=%ymm9,<mask3=%ymm3,>v11=%ymm9
vpand % ymm9, % ymm3, % ymm9

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#8,<v01=reg256#8
# asm 2: vpsrlq $2,<v01=%ymm7,<v01=%ymm7
vpsrlq $2, % ymm7, % ymm7

# qhasm: x5 = v00 | v10
# asm 1: vpor  <v00=reg256#9,<v10=reg256#16,>x5=reg256#9
# asm 2: vpor  <v00=%ymm8,<v10=%ymm15,>x5=%ymm8
vpor  % ymm8, % ymm15, % ymm8

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#10,>x7=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm9,>x7=%ymm7
vpor  % ymm7, % ymm9, % ymm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <x0=reg256#14,<mask4=reg256#5,>v00=reg256#10
# asm 2: vpand <x0=%ymm13,<mask4=%ymm4,>v00=%ymm9
vpand % ymm13, % ymm4, % ymm9

# qhasm: v10 = x1 & mask4
# asm 1: vpand <x1=reg256#15,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x1=%ymm14,<mask4=%ymm4,>v10=%ymm15
vpand % ymm14, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x0 & mask5
# asm 1: vpand <x0=reg256#14,<mask5=reg256#6,>v01=reg256#14
# asm 2: vpand <x0=%ymm13,<mask5=%ymm5,>v01=%ymm13
vpand % ymm13, % ymm5, % ymm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <x1=reg256#15,<mask5=reg256#6,>v11=reg256#15
# asm 2: vpand <x1=%ymm14,<mask5=%ymm5,>v11=%ymm14
vpand % ymm14, % ymm5, % ymm14

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#14,<v01=reg256#14
# asm 2: vpsrlq $1,<v01=%ymm13,<v01=%ymm13
vpsrlq $1, % ymm13, % ymm13

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#10,<v10=reg256#16,>x0=reg256#10
# asm 2: vpor  <v00=%ymm9,<v10=%ymm15,>x0=%ymm9
vpor  % ymm9, % ymm15, % ymm9

# qhasm: x1 = v01 | v11
# asm 1: vpor  <v01=reg256#14,<v11=reg256#15,>x1=reg256#14
# asm 2: vpor  <v01=%ymm13,<v11=%ymm14,>x1=%ymm13
vpor  % ymm13, % ymm14, % ymm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <x2=reg256#12,<mask4=reg256#5,>v00=reg256#15
# asm 2: vpand <x2=%ymm11,<mask4=%ymm4,>v00=%ymm14
vpand % ymm11, % ymm4, % ymm14

# qhasm: v10 = x3 & mask4
# asm 1: vpand <x3=reg256#11,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x3=%ymm10,<mask4=%ymm4,>v10=%ymm15
vpand % ymm10, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x2 & mask5
# asm 1: vpand <x2=reg256#12,<mask5=reg256#6,>v01=reg256#12
# asm 2: vpand <x2=%ymm11,<mask5=%ymm5,>v01=%ymm11
vpand % ymm11, % ymm5, % ymm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <x3=reg256#11,<mask5=reg256#6,>v11=reg256#11
# asm 2: vpand <x3=%ymm10,<mask5=%ymm5,>v11=%ymm10
vpand % ymm10, % ymm5, % ymm10

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#12,<v01=reg256#12
# asm 2: vpsrlq $1,<v01=%ymm11,<v01=%ymm11
vpsrlq $1, % ymm11, % ymm11

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x2=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x2=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#12,<v11=reg256#11,>x3=reg256#11
# asm 2: vpor  <v01=%ymm11,<v11=%ymm10,>x3=%ymm10
vpor  % ymm11, % ymm10, % ymm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <x4=reg256#13,<mask4=reg256#5,>v00=reg256#12
# asm 2: vpand <x4=%ymm12,<mask4=%ymm4,>v00=%ymm11
vpand % ymm12, % ymm4, % ymm11

# qhasm: v10 = x5 & mask4
# asm 1: vpand <x5=reg256#9,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x5=%ymm8,<mask4=%ymm4,>v10=%ymm15
vpand % ymm8, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x4 & mask5
# asm 1: vpand <x4=reg256#13,<mask5=reg256#6,>v01=reg256#13
# asm 2: vpand <x4=%ymm12,<mask5=%ymm5,>v01=%ymm12
vpand % ymm12, % ymm5, % ymm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <x5=reg256#9,<mask5=reg256#6,>v11=reg256#9
# asm 2: vpand <x5=%ymm8,<mask5=%ymm5,>v11=%ymm8
vpand % ymm8, % ymm5, % ymm8

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#13,<v01=reg256#13
# asm 2: vpsrlq $1,<v01=%ymm12,<v01=%ymm12
vpsrlq $1, % ymm12, % ymm12

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x4=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x4=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#13,<v11=reg256#9,>x5=reg256#9
# asm 2: vpor  <v01=%ymm12,<v11=%ymm8,>x5=%ymm8
vpor  % ymm12, % ymm8, % ymm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <x6=reg256#7,<mask4=reg256#5,>v00=reg256#13
# asm 2: vpand <x6=%ymm6,<mask4=%ymm4,>v00=%ymm12
vpand % ymm6, % ymm4, % ymm12

# qhasm: v10 = x7 & mask4
# asm 1: vpand <x7=reg256#8,<mask4=reg256#5,>v10=reg256#16
# asm 2: vpand <x7=%ymm7,<mask4=%ymm4,>v10=%ymm15
vpand % ymm7, % ymm4, % ymm15

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $1,<v10=%ymm15,<v10=%ymm15
vpsllq $1, % ymm15, % ymm15

# qhasm: v01 = x6 & mask5
# asm 1: vpand <x6=reg256#7,<mask5=reg256#6,>v01=reg256#7
# asm 2: vpand <x6=%ymm6,<mask5=%ymm5,>v01=%ymm6
vpand % ymm6, % ymm5, % ymm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <x7=reg256#8,<mask5=reg256#6,>v11=reg256#8
# asm 2: vpand <x7=%ymm7,<mask5=%ymm5,>v11=%ymm7
vpand % ymm7, % ymm5, % ymm7

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $1,<v01=%ymm6,<v01=%ymm6
vpsrlq $1, % ymm6, % ymm6

# qhasm: x6 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#16,>x6=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm15,>x6=%ymm12
vpor  % ymm12, % ymm15, % ymm12

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#8,>x7=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm7,>x7=%ymm6
vpor  % ymm6, % ymm7, % ymm6

# qhasm: mem256[ input_0 + 1536 ] = x0
# asm 1: vmovupd   <x0=reg256#10,1536(<input_0=int64#1)
# asm 2: vmovupd   <x0=%ymm9,1536(<input_0=%rdi)
vmovupd   % ymm9, 1536( % rdi)

# qhasm: mem256[ input_0 + 1568 ] = x1
# asm 1: vmovupd   <x1=reg256#14,1568(<input_0=int64#1)
# asm 2: vmovupd   <x1=%ymm13,1568(<input_0=%rdi)
vmovupd   % ymm13, 1568( % rdi)

# qhasm: mem256[ input_0 + 1600 ] = x2
# asm 1: vmovupd   <x2=reg256#15,1600(<input_0=int64#1)
# asm 2: vmovupd   <x2=%ymm14,1600(<input_0=%rdi)
vmovupd   % ymm14, 1600( % rdi)

# qhasm: mem256[ input_0 + 1632 ] = x3
# asm 1: vmovupd   <x3=reg256#11,1632(<input_0=int64#1)
# asm 2: vmovupd   <x3=%ymm10,1632(<input_0=%rdi)
vmovupd   % ymm10, 1632( % rdi)

# qhasm: mem256[ input_0 + 1664 ] = x4
# asm 1: vmovupd   <x4=reg256#12,1664(<input_0=int64#1)
# asm 2: vmovupd   <x4=%ymm11,1664(<input_0=%rdi)
vmovupd   % ymm11, 1664( % rdi)

# qhasm: mem256[ input_0 + 1696 ] = x5
# asm 1: vmovupd   <x5=reg256#9,1696(<input_0=int64#1)
# asm 2: vmovupd   <x5=%ymm8,1696(<input_0=%rdi)
vmovupd   % ymm8, 1696( % rdi)

# qhasm: mem256[ input_0 + 1728 ] = x6
# asm 1: vmovupd   <x6=reg256#13,1728(<input_0=int64#1)
# asm 2: vmovupd   <x6=%ymm12,1728(<input_0=%rdi)
vmovupd   % ymm12, 1728( % rdi)

# qhasm: mem256[ input_0 + 1760 ] = x7
# asm 1: vmovupd   <x7=reg256#7,1760(<input_0=int64#1)
# asm 2: vmovupd   <x7=%ymm6,1760(<input_0=%rdi)
vmovupd   % ymm6, 1760( % rdi)

# qhasm: x0 = mem256[ input_0 + 1792 ]
# asm 1: vmovupd   1792(<input_0=int64#1),>x0=reg256#7
# asm 2: vmovupd   1792(<input_0=%rdi),>x0=%ymm6
vmovupd   1792( % rdi), % ymm6

# qhasm: x1 = mem256[ input_0 + 1824 ]
# asm 1: vmovupd   1824(<input_0=int64#1),>x1=reg256#8
# asm 2: vmovupd   1824(<input_0=%rdi),>x1=%ymm7
vmovupd   1824( % rdi), % ymm7

# qhasm: x2 = mem256[ input_0 + 1856 ]
# asm 1: vmovupd   1856(<input_0=int64#1),>x2=reg256#9
# asm 2: vmovupd   1856(<input_0=%rdi),>x2=%ymm8
vmovupd   1856( % rdi), % ymm8

# qhasm: x3 = mem256[ input_0 + 1888 ]
# asm 1: vmovupd   1888(<input_0=int64#1),>x3=reg256#10
# asm 2: vmovupd   1888(<input_0=%rdi),>x3=%ymm9
vmovupd   1888( % rdi), % ymm9

# qhasm: x4 = mem256[ input_0 + 1920 ]
# asm 1: vmovupd   1920(<input_0=int64#1),>x4=reg256#11
# asm 2: vmovupd   1920(<input_0=%rdi),>x4=%ymm10
vmovupd   1920( % rdi), % ymm10

# qhasm: x5 = mem256[ input_0 + 1952 ]
# asm 1: vmovupd   1952(<input_0=int64#1),>x5=reg256#12
# asm 2: vmovupd   1952(<input_0=%rdi),>x5=%ymm11
vmovupd   1952( % rdi), % ymm11

# qhasm: x6 = mem256[ input_0 + 1984 ]
# asm 1: vmovupd   1984(<input_0=int64#1),>x6=reg256#13
# asm 2: vmovupd   1984(<input_0=%rdi),>x6=%ymm12
vmovupd   1984( % rdi), % ymm12

# qhasm: x7 = mem256[ input_0 + 2016 ]
# asm 1: vmovupd   2016(<input_0=int64#1),>x7=reg256#14
# asm 2: vmovupd   2016(<input_0=%rdi),>x7=%ymm13
vmovupd   2016( % rdi), % ymm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <x0=reg256#7,<mask0=reg256#1,>v00=reg256#15
# asm 2: vpand <x0=%ymm6,<mask0=%ymm0,>v00=%ymm14
vpand % ymm6, % ymm0, % ymm14

# qhasm: v10 = x4 & mask0
# asm 1: vpand <x4=reg256#11,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x4=%ymm10,<mask0=%ymm0,>v10=%ymm15
vpand % ymm10, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x0 & mask1
# asm 1: vpand <x0=reg256#7,<mask1=reg256#2,>v01=reg256#7
# asm 2: vpand <x0=%ymm6,<mask1=%ymm1,>v01=%ymm6
vpand % ymm6, % ymm1, % ymm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <x4=reg256#11,<mask1=reg256#2,>v11=reg256#11
# asm 2: vpand <x4=%ymm10,<mask1=%ymm1,>v11=%ymm10
vpand % ymm10, % ymm1, % ymm10

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $4,<v01=%ymm6,<v01=%ymm6
vpsrlq $4, % ymm6, % ymm6

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#15,<v10=reg256#16,>x0=reg256#15
# asm 2: vpor  <v00=%ymm14,<v10=%ymm15,>x0=%ymm14
vpor  % ymm14, % ymm15, % ymm14

# qhasm: x4 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#11,>x4=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm10,>x4=%ymm6
vpor  % ymm6, % ymm10, % ymm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <x1=reg256#8,<mask0=reg256#1,>v00=reg256#11
# asm 2: vpand <x1=%ymm7,<mask0=%ymm0,>v00=%ymm10
vpand % ymm7, % ymm0, % ymm10

# qhasm: v10 = x5 & mask0
# asm 1: vpand <x5=reg256#12,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x5=%ymm11,<mask0=%ymm0,>v10=%ymm15
vpand % ymm11, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x1 & mask1
# asm 1: vpand <x1=reg256#8,<mask1=reg256#2,>v01=reg256#8
# asm 2: vpand <x1=%ymm7,<mask1=%ymm1,>v01=%ymm7
vpand % ymm7, % ymm1, % ymm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <x5=reg256#12,<mask1=reg256#2,>v11=reg256#12
# asm 2: vpand <x5=%ymm11,<mask1=%ymm1,>v11=%ymm11
vpand % ymm11, % ymm1, % ymm11

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#8,<v01=reg256#8
# asm 2: vpsrlq $4,<v01=%ymm7,<v01=%ymm7
vpsrlq $4, % ymm7, % ymm7

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#16,>x1=reg256#11
# asm 2: vpor  <v00=%ymm10,<v10=%ymm15,>x1=%ymm10
vpor  % ymm10, % ymm15, % ymm10

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#12,>x5=reg256#8
# asm 2: vpor  <v01=%ymm7,<v11=%ymm11,>x5=%ymm7
vpor  % ymm7, % ymm11, % ymm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <x2=reg256#9,<mask0=reg256#1,>v00=reg256#12
# asm 2: vpand <x2=%ymm8,<mask0=%ymm0,>v00=%ymm11
vpand % ymm8, % ymm0, % ymm11

# qhasm: v10 = x6 & mask0
# asm 1: vpand <x6=reg256#13,<mask0=reg256#1,>v10=reg256#16
# asm 2: vpand <x6=%ymm12,<mask0=%ymm0,>v10=%ymm15
vpand % ymm12, % ymm0, % ymm15

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#16,<v10=reg256#16
# asm 2: vpsllq $4,<v10=%ymm15,<v10=%ymm15
vpsllq $4, % ymm15, % ymm15

# qhasm: v01 = x2 & mask1
# asm 1: vpand <x2=reg256#9,<mask1=reg256#2,>v01=reg256#9
# asm 2: vpand <x2=%ymm8,<mask1=%ymm1,>v01=%ymm8
vpand % ymm8, % ymm1, % ymm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <x6=reg256#13,<mask1=reg256#2,>v11=reg256#13
# asm 2: vpand <x6=%ymm12,<mask1=%ymm1,>v11=%ymm12
vpand % ymm12, % ymm1, % ymm12

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#9,<v01=reg256#9
# asm 2: vpsrlq $4,<v01=%ymm8,<v01=%ymm8
vpsrlq $4, % ymm8, % ymm8

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#12,<v10=reg256#16,>x2=reg256#12
# asm 2: vpor  <v00=%ymm11,<v10=%ymm15,>x2=%ymm11
vpor  % ymm11, % ymm15, % ymm11

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#9,<v11=reg256#13,>x6=reg256#9
# asm 2: vpor  <v01=%ymm8,<v11=%ymm12,>x6=%ymm8
vpor  % ymm8, % ymm12, % ymm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <x3=reg256#10,<mask0=reg256#1,>v00=reg256#13
# asm 2: vpand <x3=%ymm9,<mask0=%ymm0,>v00=%ymm12
vpand % ymm9, % ymm0, % ymm12

# qhasm: v10 = x7 & mask0
# asm 1: vpand <x7=reg256#14,<mask0=reg256#1,>v10=reg256#1
# asm 2: vpand <x7=%ymm13,<mask0=%ymm0,>v10=%ymm0
vpand % ymm13, % ymm0, % ymm0

# qhasm: 4x v10 <<= 4
# asm 1: vpsllq $4,<v10=reg256#1,<v10=reg256#1
# asm 2: vpsllq $4,<v10=%ymm0,<v10=%ymm0
vpsllq $4, % ymm0, % ymm0

# qhasm: v01 = x3 & mask1
# asm 1: vpand <x3=reg256#10,<mask1=reg256#2,>v01=reg256#10
# asm 2: vpand <x3=%ymm9,<mask1=%ymm1,>v01=%ymm9
vpand % ymm9, % ymm1, % ymm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <x7=reg256#14,<mask1=reg256#2,>v11=reg256#2
# asm 2: vpand <x7=%ymm13,<mask1=%ymm1,>v11=%ymm1
vpand % ymm13, % ymm1, % ymm1

# qhasm: 4x v01 unsigned>>= 4
# asm 1: vpsrlq $4,<v01=reg256#10,<v01=reg256#10
# asm 2: vpsrlq $4,<v01=%ymm9,<v01=%ymm9
vpsrlq $4, % ymm9, % ymm9

# qhasm: x3 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#1,>x3=reg256#1
# asm 2: vpor  <v00=%ymm12,<v10=%ymm0,>x3=%ymm0
vpor  % ymm12, % ymm0, % ymm0

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#10,<v11=reg256#2,>x7=reg256#2
# asm 2: vpor  <v01=%ymm9,<v11=%ymm1,>x7=%ymm1
vpor  % ymm9, % ymm1, % ymm1

# qhasm: v00 = x0 & mask2
# asm 1: vpand <x0=reg256#15,<mask2=reg256#3,>v00=reg256#10
# asm 2: vpand <x0=%ymm14,<mask2=%ymm2,>v00=%ymm9
vpand % ymm14, % ymm2, % ymm9

# qhasm: v10 = x2 & mask2
# asm 1: vpand <x2=reg256#12,<mask2=reg256#3,>v10=reg256#13
# asm 2: vpand <x2=%ymm11,<mask2=%ymm2,>v10=%ymm12
vpand % ymm11, % ymm2, % ymm12

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#13,<v10=reg256#13
# asm 2: vpsllq $2,<v10=%ymm12,<v10=%ymm12
vpsllq $2, % ymm12, % ymm12

# qhasm: v01 = x0 & mask3
# asm 1: vpand <x0=reg256#15,<mask3=reg256#4,>v01=reg256#14
# asm 2: vpand <x0=%ymm14,<mask3=%ymm3,>v01=%ymm13
vpand % ymm14, % ymm3, % ymm13

# qhasm: v11 = x2 & mask3
# asm 1: vpand <x2=reg256#12,<mask3=reg256#4,>v11=reg256#12
# asm 2: vpand <x2=%ymm11,<mask3=%ymm3,>v11=%ymm11
vpand % ymm11, % ymm3, % ymm11

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#14,<v01=reg256#14
# asm 2: vpsrlq $2,<v01=%ymm13,<v01=%ymm13
vpsrlq $2, % ymm13, % ymm13

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#10,<v10=reg256#13,>x0=reg256#10
# asm 2: vpor  <v00=%ymm9,<v10=%ymm12,>x0=%ymm9
vpor  % ymm9, % ymm12, % ymm9

# qhasm: x2 = v01 | v11
# asm 1: vpor  <v01=reg256#14,<v11=reg256#12,>x2=reg256#12
# asm 2: vpor  <v01=%ymm13,<v11=%ymm11,>x2=%ymm11
vpor  % ymm13, % ymm11, % ymm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <x1=reg256#11,<mask2=reg256#3,>v00=reg256#13
# asm 2: vpand <x1=%ymm10,<mask2=%ymm2,>v00=%ymm12
vpand % ymm10, % ymm2, % ymm12

# qhasm: v10 = x3 & mask2
# asm 1: vpand <x3=reg256#1,<mask2=reg256#3,>v10=reg256#14
# asm 2: vpand <x3=%ymm0,<mask2=%ymm2,>v10=%ymm13
vpand % ymm0, % ymm2, % ymm13

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#14,<v10=reg256#14
# asm 2: vpsllq $2,<v10=%ymm13,<v10=%ymm13
vpsllq $2, % ymm13, % ymm13

# qhasm: v01 = x1 & mask3
# asm 1: vpand <x1=reg256#11,<mask3=reg256#4,>v01=reg256#11
# asm 2: vpand <x1=%ymm10,<mask3=%ymm3,>v01=%ymm10
vpand % ymm10, % ymm3, % ymm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <x3=reg256#1,<mask3=reg256#4,>v11=reg256#1
# asm 2: vpand <x3=%ymm0,<mask3=%ymm3,>v11=%ymm0
vpand % ymm0, % ymm3, % ymm0

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#11,<v01=reg256#11
# asm 2: vpsrlq $2,<v01=%ymm10,<v01=%ymm10
vpsrlq $2, % ymm10, % ymm10

# qhasm: x1 = v00 | v10
# asm 1: vpor  <v00=reg256#13,<v10=reg256#14,>x1=reg256#13
# asm 2: vpor  <v00=%ymm12,<v10=%ymm13,>x1=%ymm12
vpor  % ymm12, % ymm13, % ymm12

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#11,<v11=reg256#1,>x3=reg256#1
# asm 2: vpor  <v01=%ymm10,<v11=%ymm0,>x3=%ymm0
vpor  % ymm10, % ymm0, % ymm0

# qhasm: v00 = x4 & mask2
# asm 1: vpand <x4=reg256#7,<mask2=reg256#3,>v00=reg256#11
# asm 2: vpand <x4=%ymm6,<mask2=%ymm2,>v00=%ymm10
vpand % ymm6, % ymm2, % ymm10

# qhasm: v10 = x6 & mask2
# asm 1: vpand <x6=reg256#9,<mask2=reg256#3,>v10=reg256#14
# asm 2: vpand <x6=%ymm8,<mask2=%ymm2,>v10=%ymm13
vpand % ymm8, % ymm2, % ymm13

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#14,<v10=reg256#14
# asm 2: vpsllq $2,<v10=%ymm13,<v10=%ymm13
vpsllq $2, % ymm13, % ymm13

# qhasm: v01 = x4 & mask3
# asm 1: vpand <x4=reg256#7,<mask3=reg256#4,>v01=reg256#7
# asm 2: vpand <x4=%ymm6,<mask3=%ymm3,>v01=%ymm6
vpand % ymm6, % ymm3, % ymm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <x6=reg256#9,<mask3=reg256#4,>v11=reg256#9
# asm 2: vpand <x6=%ymm8,<mask3=%ymm3,>v11=%ymm8
vpand % ymm8, % ymm3, % ymm8

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $2,<v01=%ymm6,<v01=%ymm6
vpsrlq $2, % ymm6, % ymm6

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#14,>x4=reg256#11
# asm 2: vpor  <v00=%ymm10,<v10=%ymm13,>x4=%ymm10
vpor  % ymm10, % ymm13, % ymm10

# qhasm: x6 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#9,>x6=reg256#7
# asm 2: vpor  <v01=%ymm6,<v11=%ymm8,>x6=%ymm6
vpor  % ymm6, % ymm8, % ymm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <x5=reg256#8,<mask2=reg256#3,>v00=reg256#9
# asm 2: vpand <x5=%ymm7,<mask2=%ymm2,>v00=%ymm8
vpand % ymm7, % ymm2, % ymm8

# qhasm: v10 = x7 & mask2
# asm 1: vpand <x7=reg256#2,<mask2=reg256#3,>v10=reg256#3
# asm 2: vpand <x7=%ymm1,<mask2=%ymm2,>v10=%ymm2
vpand % ymm1, % ymm2, % ymm2

# qhasm: 4x v10 <<= 2
# asm 1: vpsllq $2,<v10=reg256#3,<v10=reg256#3
# asm 2: vpsllq $2,<v10=%ymm2,<v10=%ymm2
vpsllq $2, % ymm2, % ymm2

# qhasm: v01 = x5 & mask3
# asm 1: vpand <x5=reg256#8,<mask3=reg256#4,>v01=reg256#8
# asm 2: vpand <x5=%ymm7,<mask3=%ymm3,>v01=%ymm7
vpand % ymm7, % ymm3, % ymm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <x7=reg256#2,<mask3=reg256#4,>v11=reg256#2
# asm 2: vpand <x7=%ymm1,<mask3=%ymm3,>v11=%ymm1
vpand % ymm1, % ymm3, % ymm1

# qhasm: 4x v01 unsigned>>= 2
# asm 1: vpsrlq $2,<v01=reg256#8,<v01=reg256#8
# asm 2: vpsrlq $2,<v01=%ymm7,<v01=%ymm7
vpsrlq $2, % ymm7, % ymm7

# qhasm: x5 = v00 | v10
# asm 1: vpor  <v00=reg256#9,<v10=reg256#3,>x5=reg256#3
# asm 2: vpor  <v00=%ymm8,<v10=%ymm2,>x5=%ymm2
vpor  % ymm8, % ymm2, % ymm2

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#8,<v11=reg256#2,>x7=reg256#2
# asm 2: vpor  <v01=%ymm7,<v11=%ymm1,>x7=%ymm1
vpor  % ymm7, % ymm1, % ymm1

# qhasm: v00 = x0 & mask4
# asm 1: vpand <x0=reg256#10,<mask4=reg256#5,>v00=reg256#4
# asm 2: vpand <x0=%ymm9,<mask4=%ymm4,>v00=%ymm3
vpand % ymm9, % ymm4, % ymm3

# qhasm: v10 = x1 & mask4
# asm 1: vpand <x1=reg256#13,<mask4=reg256#5,>v10=reg256#8
# asm 2: vpand <x1=%ymm12,<mask4=%ymm4,>v10=%ymm7
vpand % ymm12, % ymm4, % ymm7

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#8,<v10=reg256#8
# asm 2: vpsllq $1,<v10=%ymm7,<v10=%ymm7
vpsllq $1, % ymm7, % ymm7

# qhasm: v01 = x0 & mask5
# asm 1: vpand <x0=reg256#10,<mask5=reg256#6,>v01=reg256#9
# asm 2: vpand <x0=%ymm9,<mask5=%ymm5,>v01=%ymm8
vpand % ymm9, % ymm5, % ymm8

# qhasm: v11 = x1 & mask5
# asm 1: vpand <x1=reg256#13,<mask5=reg256#6,>v11=reg256#10
# asm 2: vpand <x1=%ymm12,<mask5=%ymm5,>v11=%ymm9
vpand % ymm12, % ymm5, % ymm9

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#9,<v01=reg256#9
# asm 2: vpsrlq $1,<v01=%ymm8,<v01=%ymm8
vpsrlq $1, % ymm8, % ymm8

# qhasm: x0 = v00 | v10
# asm 1: vpor  <v00=reg256#4,<v10=reg256#8,>x0=reg256#4
# asm 2: vpor  <v00=%ymm3,<v10=%ymm7,>x0=%ymm3
vpor  % ymm3, % ymm7, % ymm3

# qhasm: x1 = v01 | v11
# asm 1: vpor  <v01=reg256#9,<v11=reg256#10,>x1=reg256#8
# asm 2: vpor  <v01=%ymm8,<v11=%ymm9,>x1=%ymm7
vpor  % ymm8, % ymm9, % ymm7

# qhasm: v00 = x2 & mask4
# asm 1: vpand <x2=reg256#12,<mask4=reg256#5,>v00=reg256#9
# asm 2: vpand <x2=%ymm11,<mask4=%ymm4,>v00=%ymm8
vpand % ymm11, % ymm4, % ymm8

# qhasm: v10 = x3 & mask4
# asm 1: vpand <x3=reg256#1,<mask4=reg256#5,>v10=reg256#10
# asm 2: vpand <x3=%ymm0,<mask4=%ymm4,>v10=%ymm9
vpand % ymm0, % ymm4, % ymm9

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#10,<v10=reg256#10
# asm 2: vpsllq $1,<v10=%ymm9,<v10=%ymm9
vpsllq $1, % ymm9, % ymm9

# qhasm: v01 = x2 & mask5
# asm 1: vpand <x2=reg256#12,<mask5=reg256#6,>v01=reg256#12
# asm 2: vpand <x2=%ymm11,<mask5=%ymm5,>v01=%ymm11
vpand % ymm11, % ymm5, % ymm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <x3=reg256#1,<mask5=reg256#6,>v11=reg256#1
# asm 2: vpand <x3=%ymm0,<mask5=%ymm5,>v11=%ymm0
vpand % ymm0, % ymm5, % ymm0

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#12,<v01=reg256#12
# asm 2: vpsrlq $1,<v01=%ymm11,<v01=%ymm11
vpsrlq $1, % ymm11, % ymm11

# qhasm: x2 = v00 | v10
# asm 1: vpor  <v00=reg256#9,<v10=reg256#10,>x2=reg256#9
# asm 2: vpor  <v00=%ymm8,<v10=%ymm9,>x2=%ymm8
vpor  % ymm8, % ymm9, % ymm8

# qhasm: x3 = v01 | v11
# asm 1: vpor  <v01=reg256#12,<v11=reg256#1,>x3=reg256#1
# asm 2: vpor  <v01=%ymm11,<v11=%ymm0,>x3=%ymm0
vpor  % ymm11, % ymm0, % ymm0

# qhasm: v00 = x4 & mask4
# asm 1: vpand <x4=reg256#11,<mask4=reg256#5,>v00=reg256#10
# asm 2: vpand <x4=%ymm10,<mask4=%ymm4,>v00=%ymm9
vpand % ymm10, % ymm4, % ymm9

# qhasm: v10 = x5 & mask4
# asm 1: vpand <x5=reg256#3,<mask4=reg256#5,>v10=reg256#12
# asm 2: vpand <x5=%ymm2,<mask4=%ymm4,>v10=%ymm11
vpand % ymm2, % ymm4, % ymm11

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#12,<v10=reg256#12
# asm 2: vpsllq $1,<v10=%ymm11,<v10=%ymm11
vpsllq $1, % ymm11, % ymm11

# qhasm: v01 = x4 & mask5
# asm 1: vpand <x4=reg256#11,<mask5=reg256#6,>v01=reg256#11
# asm 2: vpand <x4=%ymm10,<mask5=%ymm5,>v01=%ymm10
vpand % ymm10, % ymm5, % ymm10

# qhasm: v11 = x5 & mask5
# asm 1: vpand <x5=reg256#3,<mask5=reg256#6,>v11=reg256#3
# asm 2: vpand <x5=%ymm2,<mask5=%ymm5,>v11=%ymm2
vpand % ymm2, % ymm5, % ymm2

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#11,<v01=reg256#11
# asm 2: vpsrlq $1,<v01=%ymm10,<v01=%ymm10
vpsrlq $1, % ymm10, % ymm10

# qhasm: x4 = v00 | v10
# asm 1: vpor  <v00=reg256#10,<v10=reg256#12,>x4=reg256#10
# asm 2: vpor  <v00=%ymm9,<v10=%ymm11,>x4=%ymm9
vpor  % ymm9, % ymm11, % ymm9

# qhasm: x5 = v01 | v11
# asm 1: vpor  <v01=reg256#11,<v11=reg256#3,>x5=reg256#3
# asm 2: vpor  <v01=%ymm10,<v11=%ymm2,>x5=%ymm2
vpor  % ymm10, % ymm2, % ymm2

# qhasm: v00 = x6 & mask4
# asm 1: vpand <x6=reg256#7,<mask4=reg256#5,>v00=reg256#11
# asm 2: vpand <x6=%ymm6,<mask4=%ymm4,>v00=%ymm10
vpand % ymm6, % ymm4, % ymm10

# qhasm: v10 = x7 & mask4
# asm 1: vpand <x7=reg256#2,<mask4=reg256#5,>v10=reg256#5
# asm 2: vpand <x7=%ymm1,<mask4=%ymm4,>v10=%ymm4
vpand % ymm1, % ymm4, % ymm4

# qhasm: 4x v10 <<= 1
# asm 1: vpsllq $1,<v10=reg256#5,<v10=reg256#5
# asm 2: vpsllq $1,<v10=%ymm4,<v10=%ymm4
vpsllq $1, % ymm4, % ymm4

# qhasm: v01 = x6 & mask5
# asm 1: vpand <x6=reg256#7,<mask5=reg256#6,>v01=reg256#7
# asm 2: vpand <x6=%ymm6,<mask5=%ymm5,>v01=%ymm6
vpand % ymm6, % ymm5, % ymm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <x7=reg256#2,<mask5=reg256#6,>v11=reg256#2
# asm 2: vpand <x7=%ymm1,<mask5=%ymm5,>v11=%ymm1
vpand % ymm1, % ymm5, % ymm1

# qhasm: 4x v01 unsigned>>= 1
# asm 1: vpsrlq $1,<v01=reg256#7,<v01=reg256#7
# asm 2: vpsrlq $1,<v01=%ymm6,<v01=%ymm6
vpsrlq $1, % ymm6, % ymm6

# qhasm: x6 = v00 | v10
# asm 1: vpor  <v00=reg256#11,<v10=reg256#5,>x6=reg256#5
# asm 2: vpor  <v00=%ymm10,<v10=%ymm4,>x6=%ymm4
vpor  % ymm10, % ymm4, % ymm4

# qhasm: x7 = v01 | v11
# asm 1: vpor  <v01=reg256#7,<v11=reg256#2,>x7=reg256#2
# asm 2: vpor  <v01=%ymm6,<v11=%ymm1,>x7=%ymm1
vpor  % ymm6, % ymm1, % ymm1

# qhasm: mem256[ input_0 + 1792 ] = x0
# asm 1: vmovupd   <x0=reg256#4,1792(<input_0=int64#1)
# asm 2: vmovupd   <x0=%ymm3,1792(<input_0=%rdi)
vmovupd   % ymm3, 1792( % rdi)

# qhasm: mem256[ input_0 + 1824 ] = x1
# asm 1: vmovupd   <x1=reg256#8,1824(<input_0=int64#1)
# asm 2: vmovupd   <x1=%ymm7,1824(<input_0=%rdi)
vmovupd   % ymm7, 1824( % rdi)

# qhasm: mem256[ input_0 + 1856 ] = x2
# asm 1: vmovupd   <x2=reg256#9,1856(<input_0=int64#1)
# asm 2: vmovupd   <x2=%ymm8,1856(<input_0=%rdi)
vmovupd   % ymm8, 1856( % rdi)

# qhasm: mem256[ input_0 + 1888 ] = x3
# asm 1: vmovupd   <x3=reg256#1,1888(<input_0=int64#1)
# asm 2: vmovupd   <x3=%ymm0,1888(<input_0=%rdi)
vmovupd   % ymm0, 1888( % rdi)

# qhasm: mem256[ input_0 + 1920 ] = x4
# asm 1: vmovupd   <x4=reg256#10,1920(<input_0=int64#1)
# asm 2: vmovupd   <x4=%ymm9,1920(<input_0=%rdi)
vmovupd   % ymm9, 1920( % rdi)

# qhasm: mem256[ input_0 + 1952 ] = x5
# asm 1: vmovupd   <x5=reg256#3,1952(<input_0=int64#1)
# asm 2: vmovupd   <x5=%ymm2,1952(<input_0=%rdi)
vmovupd   % ymm2, 1952( % rdi)

# qhasm: mem256[ input_0 + 1984 ] = x6
# asm 1: vmovupd   <x6=reg256#5,1984(<input_0=int64#1)
# asm 2: vmovupd   <x6=%ymm4,1984(<input_0=%rdi)
vmovupd   % ymm4, 1984( % rdi)

# qhasm: mem256[ input_0 + 2016 ] = x7
# asm 1: vmovupd   <x7=reg256#2,2016(<input_0=int64#1)
# asm 2: vmovupd   <x7=%ymm1,2016(<input_0=%rdi)
vmovupd   % ymm1, 2016( % rdi)

# qhasm: return
add % r11, % rsp
ret
