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
#define transpose_64x128_sp_asm CRYPTO_NAMESPACE(transpose_64x128_sp_asm)
#define _transpose_64x128_sp_asm _CRYPTO_NAMESPACE(transpose_64x128_sp_asm)

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

# qhasm: reg128 x0

# qhasm: reg128 x1

# qhasm: reg128 x2

# qhasm: reg128 x3

# qhasm: reg128 x4

# qhasm: reg128 x5

# qhasm: reg128 x6

# qhasm: reg128 x7

# qhasm: reg128 t0

# qhasm: reg128 t1

# qhasm: reg128 v00

# qhasm: reg128 v01

# qhasm: reg128 v10

# qhasm: reg128 v11

# qhasm: reg128 mask0

# qhasm: reg128 mask1

# qhasm: reg128 mask2

# qhasm: reg128 mask3

# qhasm: reg128 mask4

# qhasm: reg128 mask5

# qhasm: enter transpose_64x128_sp_asm
.p2align 5
.global _transpose_64x128_sp_asm
.global transpose_64x128_sp_asm
_transpose_64x128_sp_asm:
transpose_64x128_sp_asm:
mov % rsp, % r11
and $31, % r11
add $0, % r11
sub % r11, % rsp

# qhasm: mask0 aligned= mem128[ MASK5_0 ]
# asm 1: movdqa   MASK5_0(%rip),>mask0=reg128#1
# asm 2: movdqa   MASK5_0(%rip),>mask0=%xmm0
movdqa   MASK5_0( % rip), % xmm0

# qhasm: mask1 aligned= mem128[ MASK5_1 ]
# asm 1: movdqa   MASK5_1(%rip),>mask1=reg128#2
# asm 2: movdqa   MASK5_1(%rip),>mask1=%xmm1
movdqa   MASK5_1( % rip), % xmm1

# qhasm: mask2 aligned= mem128[ MASK4_0 ]
# asm 1: movdqa   MASK4_0(%rip),>mask2=reg128#3
# asm 2: movdqa   MASK4_0(%rip),>mask2=%xmm2
movdqa   MASK4_0( % rip), % xmm2

# qhasm: mask3 aligned= mem128[ MASK4_1 ]
# asm 1: movdqa   MASK4_1(%rip),>mask3=reg128#4
# asm 2: movdqa   MASK4_1(%rip),>mask3=%xmm3
movdqa   MASK4_1( % rip), % xmm3

# qhasm: mask4 aligned= mem128[ MASK3_0 ]
# asm 1: movdqa   MASK3_0(%rip),>mask4=reg128#5
# asm 2: movdqa   MASK3_0(%rip),>mask4=%xmm4
movdqa   MASK3_0( % rip), % xmm4

# qhasm: mask5 aligned= mem128[ MASK3_1 ]
# asm 1: movdqa   MASK3_1(%rip),>mask5=reg128#6
# asm 2: movdqa   MASK3_1(%rip),>mask5=%xmm5
movdqa   MASK3_1( % rip), % xmm5

# qhasm: x0 = mem128[ input_0 + 0 ]
# asm 1: movdqu   0(<input_0=int64#1),>x0=reg128#7
# asm 2: movdqu   0(<input_0=%rdi),>x0=%xmm6
movdqu   0( % rdi), % xmm6

# qhasm: x1 = mem128[ input_0 + 128 ]
# asm 1: movdqu   128(<input_0=int64#1),>x1=reg128#8
# asm 2: movdqu   128(<input_0=%rdi),>x1=%xmm7
movdqu   128( % rdi), % xmm7

# qhasm: x2 = mem128[ input_0 + 256 ]
# asm 1: movdqu   256(<input_0=int64#1),>x2=reg128#9
# asm 2: movdqu   256(<input_0=%rdi),>x2=%xmm8
movdqu   256( % rdi), % xmm8

# qhasm: x3 = mem128[ input_0 + 384 ]
# asm 1: movdqu   384(<input_0=int64#1),>x3=reg128#10
# asm 2: movdqu   384(<input_0=%rdi),>x3=%xmm9
movdqu   384( % rdi), % xmm9

# qhasm: x4 = mem128[ input_0 + 512 ]
# asm 1: movdqu   512(<input_0=int64#1),>x4=reg128#11
# asm 2: movdqu   512(<input_0=%rdi),>x4=%xmm10
movdqu   512( % rdi), % xmm10

# qhasm: x5 = mem128[ input_0 + 640 ]
# asm 1: movdqu   640(<input_0=int64#1),>x5=reg128#12
# asm 2: movdqu   640(<input_0=%rdi),>x5=%xmm11
movdqu   640( % rdi), % xmm11

# qhasm: x6 = mem128[ input_0 + 768 ]
# asm 1: movdqu   768(<input_0=int64#1),>x6=reg128#13
# asm 2: movdqu   768(<input_0=%rdi),>x6=%xmm12
movdqu   768( % rdi), % xmm12

# qhasm: x7 = mem128[ input_0 + 896 ]
# asm 1: movdqu   896(<input_0=int64#1),>x7=reg128#14
# asm 2: movdqu   896(<input_0=%rdi),>x7=%xmm13
movdqu   896( % rdi), % xmm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <mask0=reg128#1,<x0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<x0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: 2x v10 = x4 << 32
# asm 1: vpsllq $32,<x4=reg128#11,>v10=reg128#16
# asm 2: vpsllq $32,<x4=%xmm10,>v10=%xmm15
vpsllq $32, % xmm10, % xmm15

# qhasm: 2x v01 = x0 unsigned>> 32
# asm 1: vpsrlq $32,<x0=reg128#7,>v01=reg128#7
# asm 2: vpsrlq $32,<x0=%xmm6,>v01=%xmm6
vpsrlq $32, % xmm6, % xmm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <mask1=reg128#2,<x4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<x4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>x4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>x4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <mask0=reg128#1,<x1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<x1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: 2x v10 = x5 << 32
# asm 1: vpsllq $32,<x5=reg128#12,>v10=reg128#16
# asm 2: vpsllq $32,<x5=%xmm11,>v10=%xmm15
vpsllq $32, % xmm11, % xmm15

# qhasm: 2x v01 = x1 unsigned>> 32
# asm 1: vpsrlq $32,<x1=reg128#8,>v01=reg128#8
# asm 2: vpsrlq $32,<x1=%xmm7,>v01=%xmm7
vpsrlq $32, % xmm7, % xmm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <mask1=reg128#2,<x5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<x5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>x1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>x1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>x5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>x5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <mask0=reg128#1,<x2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<x2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: 2x v10 = x6 << 32
# asm 1: vpsllq $32,<x6=reg128#13,>v10=reg128#16
# asm 2: vpsllq $32,<x6=%xmm12,>v10=%xmm15
vpsllq $32, % xmm12, % xmm15

# qhasm: 2x v01 = x2 unsigned>> 32
# asm 1: vpsrlq $32,<x2=reg128#9,>v01=reg128#9
# asm 2: vpsrlq $32,<x2=%xmm8,>v01=%xmm8
vpsrlq $32, % xmm8, % xmm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <mask1=reg128#2,<x6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<x6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>x6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>x6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <mask0=reg128#1,<x3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<x3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: 2x v10 = x7 << 32
# asm 1: vpsllq $32,<x7=reg128#14,>v10=reg128#16
# asm 2: vpsllq $32,<x7=%xmm13,>v10=%xmm15
vpsllq $32, % xmm13, % xmm15

# qhasm: 2x v01 = x3 unsigned>> 32
# asm 1: vpsrlq $32,<x3=reg128#10,>v01=reg128#10
# asm 2: vpsrlq $32,<x3=%xmm9,>v01=%xmm9
vpsrlq $32, % xmm9, % xmm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <mask1=reg128#2,<x7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<x7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: x3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>x7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>x7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <mask2=reg128#3,<x0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<x0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: 4x v10 = x2 << 16
# asm 1: vpslld $16,<x2=reg128#12,>v10=reg128#16
# asm 2: vpslld $16,<x2=%xmm11,>v10=%xmm15
vpslld $16, % xmm11, % xmm15

# qhasm: 4x v01 = x0 unsigned>> 16
# asm 1: vpsrld $16,<x0=reg128#15,>v01=reg128#15
# asm 2: vpsrld $16,<x0=%xmm14,>v01=%xmm14
vpsrld $16, % xmm14, % xmm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <mask3=reg128#4,<x2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<x2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>x0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>x0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: x2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>x2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>x2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <mask2=reg128#3,<x1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<x1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: 4x v10 = x3 << 16
# asm 1: vpslld $16,<x3=reg128#13,>v10=reg128#16
# asm 2: vpslld $16,<x3=%xmm12,>v10=%xmm15
vpslld $16, % xmm12, % xmm15

# qhasm: 4x v01 = x1 unsigned>> 16
# asm 1: vpsrld $16,<x1=reg128#11,>v01=reg128#11
# asm 2: vpsrld $16,<x1=%xmm10,>v01=%xmm10
vpsrld $16, % xmm10, % xmm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <mask3=reg128#4,<x3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<x3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>x3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>x3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <mask2=reg128#3,<x4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<x4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: 4x v10 = x6 << 16
# asm 1: vpslld $16,<x6=reg128#9,>v10=reg128#16
# asm 2: vpslld $16,<x6=%xmm8,>v10=%xmm15
vpslld $16, % xmm8, % xmm15

# qhasm: 4x v01 = x4 unsigned>> 16
# asm 1: vpsrld $16,<x4=reg128#7,>v01=reg128#7
# asm 2: vpsrld $16,<x4=%xmm6,>v01=%xmm6
vpsrld $16, % xmm6, % xmm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <mask3=reg128#4,<x6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<x6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>x6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>x6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <mask2=reg128#3,<x5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<x5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: 4x v10 = x7 << 16
# asm 1: vpslld $16,<x7=reg128#10,>v10=reg128#16
# asm 2: vpslld $16,<x7=%xmm9,>v10=%xmm15
vpslld $16, % xmm9, % xmm15

# qhasm: 4x v01 = x5 unsigned>> 16
# asm 1: vpsrld $16,<x5=reg128#8,>v01=reg128#8
# asm 2: vpsrld $16,<x5=%xmm7,>v01=%xmm7
vpsrld $16, % xmm7, % xmm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <mask3=reg128#4,<x7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<x7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: x5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>x5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>x5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>x7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>x7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <mask4=reg128#5,<x0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<x0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: 8x v10 = x1 << 8
# asm 1: vpsllw $8,<x1=reg128#15,>v10=reg128#16
# asm 2: vpsllw $8,<x1=%xmm14,>v10=%xmm15
vpsllw $8, % xmm14, % xmm15

# qhasm: 8x v01 = x0 unsigned>> 8
# asm 1: vpsrlw $8,<x0=reg128#14,>v01=reg128#14
# asm 2: vpsrlw $8,<x0=%xmm13,>v01=%xmm13
vpsrlw $8, % xmm13, % xmm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <mask5=reg128#6,<x1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<x1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>x0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>x0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: x1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>x1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>x1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <mask4=reg128#5,<x2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<x2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: 8x v10 = x3 << 8
# asm 1: vpsllw $8,<x3=reg128#11,>v10=reg128#16
# asm 2: vpsllw $8,<x3=%xmm10,>v10=%xmm15
vpsllw $8, % xmm10, % xmm15

# qhasm: 8x v01 = x2 unsigned>> 8
# asm 1: vpsrlw $8,<x2=reg128#12,>v01=reg128#12
# asm 2: vpsrlw $8,<x2=%xmm11,>v01=%xmm11
vpsrlw $8, % xmm11, % xmm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <mask5=reg128#6,<x3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<x3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>x3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>x3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <mask4=reg128#5,<x4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<x4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: 8x v10 = x5 << 8
# asm 1: vpsllw $8,<x5=reg128#9,>v10=reg128#16
# asm 2: vpsllw $8,<x5=%xmm8,>v10=%xmm15
vpsllw $8, % xmm8, % xmm15

# qhasm: 8x v01 = x4 unsigned>> 8
# asm 1: vpsrlw $8,<x4=reg128#13,>v01=reg128#13
# asm 2: vpsrlw $8,<x4=%xmm12,>v01=%xmm12
vpsrlw $8, % xmm12, % xmm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <mask5=reg128#6,<x5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<x5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>x5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>x5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <mask4=reg128#5,<x6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<x6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: 8x v10 = x7 << 8
# asm 1: vpsllw $8,<x7=reg128#8,>v10=reg128#16
# asm 2: vpsllw $8,<x7=%xmm7,>v10=%xmm15
vpsllw $8, % xmm7, % xmm15

# qhasm: 8x v01 = x6 unsigned>> 8
# asm 1: vpsrlw $8,<x6=reg128#7,>v01=reg128#7
# asm 2: vpsrlw $8,<x6=%xmm6,>v01=%xmm6
vpsrlw $8, % xmm6, % xmm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <mask5=reg128#6,<x7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<x7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: x6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>x7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>x7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: mem128[ input_0 + 0 ] = x0
# asm 1: movdqu   <x0=reg128#10,0(<input_0=int64#1)
# asm 2: movdqu   <x0=%xmm9,0(<input_0=%rdi)
movdqu   % xmm9, 0( % rdi)

# qhasm: mem128[ input_0 + 128 ] = x1
# asm 1: movdqu   <x1=reg128#14,128(<input_0=int64#1)
# asm 2: movdqu   <x1=%xmm13,128(<input_0=%rdi)
movdqu   % xmm13, 128( % rdi)

# qhasm: mem128[ input_0 + 256 ] = x2
# asm 1: movdqu   <x2=reg128#15,256(<input_0=int64#1)
# asm 2: movdqu   <x2=%xmm14,256(<input_0=%rdi)
movdqu   % xmm14, 256( % rdi)

# qhasm: mem128[ input_0 + 384 ] = x3
# asm 1: movdqu   <x3=reg128#11,384(<input_0=int64#1)
# asm 2: movdqu   <x3=%xmm10,384(<input_0=%rdi)
movdqu   % xmm10, 384( % rdi)

# qhasm: mem128[ input_0 + 512 ] = x4
# asm 1: movdqu   <x4=reg128#12,512(<input_0=int64#1)
# asm 2: movdqu   <x4=%xmm11,512(<input_0=%rdi)
movdqu   % xmm11, 512( % rdi)

# qhasm: mem128[ input_0 + 640 ] = x5
# asm 1: movdqu   <x5=reg128#9,640(<input_0=int64#1)
# asm 2: movdqu   <x5=%xmm8,640(<input_0=%rdi)
movdqu   % xmm8, 640( % rdi)

# qhasm: mem128[ input_0 + 768 ] = x6
# asm 1: movdqu   <x6=reg128#13,768(<input_0=int64#1)
# asm 2: movdqu   <x6=%xmm12,768(<input_0=%rdi)
movdqu   % xmm12, 768( % rdi)

# qhasm: mem128[ input_0 + 896 ] = x7
# asm 1: movdqu   <x7=reg128#7,896(<input_0=int64#1)
# asm 2: movdqu   <x7=%xmm6,896(<input_0=%rdi)
movdqu   % xmm6, 896( % rdi)

# qhasm: x0 = mem128[ input_0 + 16 ]
# asm 1: movdqu   16(<input_0=int64#1),>x0=reg128#7
# asm 2: movdqu   16(<input_0=%rdi),>x0=%xmm6
movdqu   16( % rdi), % xmm6

# qhasm: x1 = mem128[ input_0 + 144 ]
# asm 1: movdqu   144(<input_0=int64#1),>x1=reg128#8
# asm 2: movdqu   144(<input_0=%rdi),>x1=%xmm7
movdqu   144( % rdi), % xmm7

# qhasm: x2 = mem128[ input_0 + 272 ]
# asm 1: movdqu   272(<input_0=int64#1),>x2=reg128#9
# asm 2: movdqu   272(<input_0=%rdi),>x2=%xmm8
movdqu   272( % rdi), % xmm8

# qhasm: x3 = mem128[ input_0 + 400 ]
# asm 1: movdqu   400(<input_0=int64#1),>x3=reg128#10
# asm 2: movdqu   400(<input_0=%rdi),>x3=%xmm9
movdqu   400( % rdi), % xmm9

# qhasm: x4 = mem128[ input_0 + 528 ]
# asm 1: movdqu   528(<input_0=int64#1),>x4=reg128#11
# asm 2: movdqu   528(<input_0=%rdi),>x4=%xmm10
movdqu   528( % rdi), % xmm10

# qhasm: x5 = mem128[ input_0 + 656 ]
# asm 1: movdqu   656(<input_0=int64#1),>x5=reg128#12
# asm 2: movdqu   656(<input_0=%rdi),>x5=%xmm11
movdqu   656( % rdi), % xmm11

# qhasm: x6 = mem128[ input_0 + 784 ]
# asm 1: movdqu   784(<input_0=int64#1),>x6=reg128#13
# asm 2: movdqu   784(<input_0=%rdi),>x6=%xmm12
movdqu   784( % rdi), % xmm12

# qhasm: x7 = mem128[ input_0 + 912 ]
# asm 1: movdqu   912(<input_0=int64#1),>x7=reg128#14
# asm 2: movdqu   912(<input_0=%rdi),>x7=%xmm13
movdqu   912( % rdi), % xmm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <mask0=reg128#1,<x0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<x0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: 2x v10 = x4 << 32
# asm 1: vpsllq $32,<x4=reg128#11,>v10=reg128#16
# asm 2: vpsllq $32,<x4=%xmm10,>v10=%xmm15
vpsllq $32, % xmm10, % xmm15

# qhasm: 2x v01 = x0 unsigned>> 32
# asm 1: vpsrlq $32,<x0=reg128#7,>v01=reg128#7
# asm 2: vpsrlq $32,<x0=%xmm6,>v01=%xmm6
vpsrlq $32, % xmm6, % xmm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <mask1=reg128#2,<x4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<x4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>x4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>x4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <mask0=reg128#1,<x1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<x1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: 2x v10 = x5 << 32
# asm 1: vpsllq $32,<x5=reg128#12,>v10=reg128#16
# asm 2: vpsllq $32,<x5=%xmm11,>v10=%xmm15
vpsllq $32, % xmm11, % xmm15

# qhasm: 2x v01 = x1 unsigned>> 32
# asm 1: vpsrlq $32,<x1=reg128#8,>v01=reg128#8
# asm 2: vpsrlq $32,<x1=%xmm7,>v01=%xmm7
vpsrlq $32, % xmm7, % xmm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <mask1=reg128#2,<x5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<x5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>x1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>x1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>x5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>x5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <mask0=reg128#1,<x2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<x2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: 2x v10 = x6 << 32
# asm 1: vpsllq $32,<x6=reg128#13,>v10=reg128#16
# asm 2: vpsllq $32,<x6=%xmm12,>v10=%xmm15
vpsllq $32, % xmm12, % xmm15

# qhasm: 2x v01 = x2 unsigned>> 32
# asm 1: vpsrlq $32,<x2=reg128#9,>v01=reg128#9
# asm 2: vpsrlq $32,<x2=%xmm8,>v01=%xmm8
vpsrlq $32, % xmm8, % xmm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <mask1=reg128#2,<x6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<x6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>x6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>x6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <mask0=reg128#1,<x3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<x3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: 2x v10 = x7 << 32
# asm 1: vpsllq $32,<x7=reg128#14,>v10=reg128#16
# asm 2: vpsllq $32,<x7=%xmm13,>v10=%xmm15
vpsllq $32, % xmm13, % xmm15

# qhasm: 2x v01 = x3 unsigned>> 32
# asm 1: vpsrlq $32,<x3=reg128#10,>v01=reg128#10
# asm 2: vpsrlq $32,<x3=%xmm9,>v01=%xmm9
vpsrlq $32, % xmm9, % xmm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <mask1=reg128#2,<x7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<x7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: x3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>x7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>x7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <mask2=reg128#3,<x0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<x0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: 4x v10 = x2 << 16
# asm 1: vpslld $16,<x2=reg128#12,>v10=reg128#16
# asm 2: vpslld $16,<x2=%xmm11,>v10=%xmm15
vpslld $16, % xmm11, % xmm15

# qhasm: 4x v01 = x0 unsigned>> 16
# asm 1: vpsrld $16,<x0=reg128#15,>v01=reg128#15
# asm 2: vpsrld $16,<x0=%xmm14,>v01=%xmm14
vpsrld $16, % xmm14, % xmm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <mask3=reg128#4,<x2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<x2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>x0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>x0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: x2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>x2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>x2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <mask2=reg128#3,<x1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<x1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: 4x v10 = x3 << 16
# asm 1: vpslld $16,<x3=reg128#13,>v10=reg128#16
# asm 2: vpslld $16,<x3=%xmm12,>v10=%xmm15
vpslld $16, % xmm12, % xmm15

# qhasm: 4x v01 = x1 unsigned>> 16
# asm 1: vpsrld $16,<x1=reg128#11,>v01=reg128#11
# asm 2: vpsrld $16,<x1=%xmm10,>v01=%xmm10
vpsrld $16, % xmm10, % xmm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <mask3=reg128#4,<x3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<x3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>x3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>x3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <mask2=reg128#3,<x4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<x4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: 4x v10 = x6 << 16
# asm 1: vpslld $16,<x6=reg128#9,>v10=reg128#16
# asm 2: vpslld $16,<x6=%xmm8,>v10=%xmm15
vpslld $16, % xmm8, % xmm15

# qhasm: 4x v01 = x4 unsigned>> 16
# asm 1: vpsrld $16,<x4=reg128#7,>v01=reg128#7
# asm 2: vpsrld $16,<x4=%xmm6,>v01=%xmm6
vpsrld $16, % xmm6, % xmm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <mask3=reg128#4,<x6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<x6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>x6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>x6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <mask2=reg128#3,<x5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<x5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: 4x v10 = x7 << 16
# asm 1: vpslld $16,<x7=reg128#10,>v10=reg128#16
# asm 2: vpslld $16,<x7=%xmm9,>v10=%xmm15
vpslld $16, % xmm9, % xmm15

# qhasm: 4x v01 = x5 unsigned>> 16
# asm 1: vpsrld $16,<x5=reg128#8,>v01=reg128#8
# asm 2: vpsrld $16,<x5=%xmm7,>v01=%xmm7
vpsrld $16, % xmm7, % xmm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <mask3=reg128#4,<x7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<x7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: x5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>x5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>x5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>x7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>x7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <mask4=reg128#5,<x0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<x0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: 8x v10 = x1 << 8
# asm 1: vpsllw $8,<x1=reg128#15,>v10=reg128#16
# asm 2: vpsllw $8,<x1=%xmm14,>v10=%xmm15
vpsllw $8, % xmm14, % xmm15

# qhasm: 8x v01 = x0 unsigned>> 8
# asm 1: vpsrlw $8,<x0=reg128#14,>v01=reg128#14
# asm 2: vpsrlw $8,<x0=%xmm13,>v01=%xmm13
vpsrlw $8, % xmm13, % xmm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <mask5=reg128#6,<x1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<x1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>x0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>x0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: x1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>x1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>x1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <mask4=reg128#5,<x2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<x2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: 8x v10 = x3 << 8
# asm 1: vpsllw $8,<x3=reg128#11,>v10=reg128#16
# asm 2: vpsllw $8,<x3=%xmm10,>v10=%xmm15
vpsllw $8, % xmm10, % xmm15

# qhasm: 8x v01 = x2 unsigned>> 8
# asm 1: vpsrlw $8,<x2=reg128#12,>v01=reg128#12
# asm 2: vpsrlw $8,<x2=%xmm11,>v01=%xmm11
vpsrlw $8, % xmm11, % xmm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <mask5=reg128#6,<x3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<x3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>x3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>x3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <mask4=reg128#5,<x4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<x4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: 8x v10 = x5 << 8
# asm 1: vpsllw $8,<x5=reg128#9,>v10=reg128#16
# asm 2: vpsllw $8,<x5=%xmm8,>v10=%xmm15
vpsllw $8, % xmm8, % xmm15

# qhasm: 8x v01 = x4 unsigned>> 8
# asm 1: vpsrlw $8,<x4=reg128#13,>v01=reg128#13
# asm 2: vpsrlw $8,<x4=%xmm12,>v01=%xmm12
vpsrlw $8, % xmm12, % xmm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <mask5=reg128#6,<x5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<x5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>x5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>x5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <mask4=reg128#5,<x6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<x6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: 8x v10 = x7 << 8
# asm 1: vpsllw $8,<x7=reg128#8,>v10=reg128#16
# asm 2: vpsllw $8,<x7=%xmm7,>v10=%xmm15
vpsllw $8, % xmm7, % xmm15

# qhasm: 8x v01 = x6 unsigned>> 8
# asm 1: vpsrlw $8,<x6=reg128#7,>v01=reg128#7
# asm 2: vpsrlw $8,<x6=%xmm6,>v01=%xmm6
vpsrlw $8, % xmm6, % xmm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <mask5=reg128#6,<x7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<x7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: x6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>x7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>x7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: mem128[ input_0 + 16 ] = x0
# asm 1: movdqu   <x0=reg128#10,16(<input_0=int64#1)
# asm 2: movdqu   <x0=%xmm9,16(<input_0=%rdi)
movdqu   % xmm9, 16( % rdi)

# qhasm: mem128[ input_0 + 144 ] = x1
# asm 1: movdqu   <x1=reg128#14,144(<input_0=int64#1)
# asm 2: movdqu   <x1=%xmm13,144(<input_0=%rdi)
movdqu   % xmm13, 144( % rdi)

# qhasm: mem128[ input_0 + 272 ] = x2
# asm 1: movdqu   <x2=reg128#15,272(<input_0=int64#1)
# asm 2: movdqu   <x2=%xmm14,272(<input_0=%rdi)
movdqu   % xmm14, 272( % rdi)

# qhasm: mem128[ input_0 + 400 ] = x3
# asm 1: movdqu   <x3=reg128#11,400(<input_0=int64#1)
# asm 2: movdqu   <x3=%xmm10,400(<input_0=%rdi)
movdqu   % xmm10, 400( % rdi)

# qhasm: mem128[ input_0 + 528 ] = x4
# asm 1: movdqu   <x4=reg128#12,528(<input_0=int64#1)
# asm 2: movdqu   <x4=%xmm11,528(<input_0=%rdi)
movdqu   % xmm11, 528( % rdi)

# qhasm: mem128[ input_0 + 656 ] = x5
# asm 1: movdqu   <x5=reg128#9,656(<input_0=int64#1)
# asm 2: movdqu   <x5=%xmm8,656(<input_0=%rdi)
movdqu   % xmm8, 656( % rdi)

# qhasm: mem128[ input_0 + 784 ] = x6
# asm 1: movdqu   <x6=reg128#13,784(<input_0=int64#1)
# asm 2: movdqu   <x6=%xmm12,784(<input_0=%rdi)
movdqu   % xmm12, 784( % rdi)

# qhasm: mem128[ input_0 + 912 ] = x7
# asm 1: movdqu   <x7=reg128#7,912(<input_0=int64#1)
# asm 2: movdqu   <x7=%xmm6,912(<input_0=%rdi)
movdqu   % xmm6, 912( % rdi)

# qhasm: x0 = mem128[ input_0 + 32 ]
# asm 1: movdqu   32(<input_0=int64#1),>x0=reg128#7
# asm 2: movdqu   32(<input_0=%rdi),>x0=%xmm6
movdqu   32( % rdi), % xmm6

# qhasm: x1 = mem128[ input_0 + 160 ]
# asm 1: movdqu   160(<input_0=int64#1),>x1=reg128#8
# asm 2: movdqu   160(<input_0=%rdi),>x1=%xmm7
movdqu   160( % rdi), % xmm7

# qhasm: x2 = mem128[ input_0 + 288 ]
# asm 1: movdqu   288(<input_0=int64#1),>x2=reg128#9
# asm 2: movdqu   288(<input_0=%rdi),>x2=%xmm8
movdqu   288( % rdi), % xmm8

# qhasm: x3 = mem128[ input_0 + 416 ]
# asm 1: movdqu   416(<input_0=int64#1),>x3=reg128#10
# asm 2: movdqu   416(<input_0=%rdi),>x3=%xmm9
movdqu   416( % rdi), % xmm9

# qhasm: x4 = mem128[ input_0 + 544 ]
# asm 1: movdqu   544(<input_0=int64#1),>x4=reg128#11
# asm 2: movdqu   544(<input_0=%rdi),>x4=%xmm10
movdqu   544( % rdi), % xmm10

# qhasm: x5 = mem128[ input_0 + 672 ]
# asm 1: movdqu   672(<input_0=int64#1),>x5=reg128#12
# asm 2: movdqu   672(<input_0=%rdi),>x5=%xmm11
movdqu   672( % rdi), % xmm11

# qhasm: x6 = mem128[ input_0 + 800 ]
# asm 1: movdqu   800(<input_0=int64#1),>x6=reg128#13
# asm 2: movdqu   800(<input_0=%rdi),>x6=%xmm12
movdqu   800( % rdi), % xmm12

# qhasm: x7 = mem128[ input_0 + 928 ]
# asm 1: movdqu   928(<input_0=int64#1),>x7=reg128#14
# asm 2: movdqu   928(<input_0=%rdi),>x7=%xmm13
movdqu   928( % rdi), % xmm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <mask0=reg128#1,<x0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<x0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: 2x v10 = x4 << 32
# asm 1: vpsllq $32,<x4=reg128#11,>v10=reg128#16
# asm 2: vpsllq $32,<x4=%xmm10,>v10=%xmm15
vpsllq $32, % xmm10, % xmm15

# qhasm: 2x v01 = x0 unsigned>> 32
# asm 1: vpsrlq $32,<x0=reg128#7,>v01=reg128#7
# asm 2: vpsrlq $32,<x0=%xmm6,>v01=%xmm6
vpsrlq $32, % xmm6, % xmm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <mask1=reg128#2,<x4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<x4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>x4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>x4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <mask0=reg128#1,<x1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<x1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: 2x v10 = x5 << 32
# asm 1: vpsllq $32,<x5=reg128#12,>v10=reg128#16
# asm 2: vpsllq $32,<x5=%xmm11,>v10=%xmm15
vpsllq $32, % xmm11, % xmm15

# qhasm: 2x v01 = x1 unsigned>> 32
# asm 1: vpsrlq $32,<x1=reg128#8,>v01=reg128#8
# asm 2: vpsrlq $32,<x1=%xmm7,>v01=%xmm7
vpsrlq $32, % xmm7, % xmm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <mask1=reg128#2,<x5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<x5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>x1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>x1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>x5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>x5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <mask0=reg128#1,<x2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<x2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: 2x v10 = x6 << 32
# asm 1: vpsllq $32,<x6=reg128#13,>v10=reg128#16
# asm 2: vpsllq $32,<x6=%xmm12,>v10=%xmm15
vpsllq $32, % xmm12, % xmm15

# qhasm: 2x v01 = x2 unsigned>> 32
# asm 1: vpsrlq $32,<x2=reg128#9,>v01=reg128#9
# asm 2: vpsrlq $32,<x2=%xmm8,>v01=%xmm8
vpsrlq $32, % xmm8, % xmm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <mask1=reg128#2,<x6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<x6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>x6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>x6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <mask0=reg128#1,<x3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<x3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: 2x v10 = x7 << 32
# asm 1: vpsllq $32,<x7=reg128#14,>v10=reg128#16
# asm 2: vpsllq $32,<x7=%xmm13,>v10=%xmm15
vpsllq $32, % xmm13, % xmm15

# qhasm: 2x v01 = x3 unsigned>> 32
# asm 1: vpsrlq $32,<x3=reg128#10,>v01=reg128#10
# asm 2: vpsrlq $32,<x3=%xmm9,>v01=%xmm9
vpsrlq $32, % xmm9, % xmm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <mask1=reg128#2,<x7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<x7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: x3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>x7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>x7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <mask2=reg128#3,<x0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<x0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: 4x v10 = x2 << 16
# asm 1: vpslld $16,<x2=reg128#12,>v10=reg128#16
# asm 2: vpslld $16,<x2=%xmm11,>v10=%xmm15
vpslld $16, % xmm11, % xmm15

# qhasm: 4x v01 = x0 unsigned>> 16
# asm 1: vpsrld $16,<x0=reg128#15,>v01=reg128#15
# asm 2: vpsrld $16,<x0=%xmm14,>v01=%xmm14
vpsrld $16, % xmm14, % xmm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <mask3=reg128#4,<x2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<x2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>x0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>x0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: x2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>x2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>x2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <mask2=reg128#3,<x1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<x1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: 4x v10 = x3 << 16
# asm 1: vpslld $16,<x3=reg128#13,>v10=reg128#16
# asm 2: vpslld $16,<x3=%xmm12,>v10=%xmm15
vpslld $16, % xmm12, % xmm15

# qhasm: 4x v01 = x1 unsigned>> 16
# asm 1: vpsrld $16,<x1=reg128#11,>v01=reg128#11
# asm 2: vpsrld $16,<x1=%xmm10,>v01=%xmm10
vpsrld $16, % xmm10, % xmm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <mask3=reg128#4,<x3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<x3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>x3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>x3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <mask2=reg128#3,<x4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<x4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: 4x v10 = x6 << 16
# asm 1: vpslld $16,<x6=reg128#9,>v10=reg128#16
# asm 2: vpslld $16,<x6=%xmm8,>v10=%xmm15
vpslld $16, % xmm8, % xmm15

# qhasm: 4x v01 = x4 unsigned>> 16
# asm 1: vpsrld $16,<x4=reg128#7,>v01=reg128#7
# asm 2: vpsrld $16,<x4=%xmm6,>v01=%xmm6
vpsrld $16, % xmm6, % xmm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <mask3=reg128#4,<x6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<x6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>x6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>x6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <mask2=reg128#3,<x5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<x5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: 4x v10 = x7 << 16
# asm 1: vpslld $16,<x7=reg128#10,>v10=reg128#16
# asm 2: vpslld $16,<x7=%xmm9,>v10=%xmm15
vpslld $16, % xmm9, % xmm15

# qhasm: 4x v01 = x5 unsigned>> 16
# asm 1: vpsrld $16,<x5=reg128#8,>v01=reg128#8
# asm 2: vpsrld $16,<x5=%xmm7,>v01=%xmm7
vpsrld $16, % xmm7, % xmm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <mask3=reg128#4,<x7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<x7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: x5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>x5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>x5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>x7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>x7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <mask4=reg128#5,<x0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<x0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: 8x v10 = x1 << 8
# asm 1: vpsllw $8,<x1=reg128#15,>v10=reg128#16
# asm 2: vpsllw $8,<x1=%xmm14,>v10=%xmm15
vpsllw $8, % xmm14, % xmm15

# qhasm: 8x v01 = x0 unsigned>> 8
# asm 1: vpsrlw $8,<x0=reg128#14,>v01=reg128#14
# asm 2: vpsrlw $8,<x0=%xmm13,>v01=%xmm13
vpsrlw $8, % xmm13, % xmm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <mask5=reg128#6,<x1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<x1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>x0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>x0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: x1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>x1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>x1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <mask4=reg128#5,<x2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<x2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: 8x v10 = x3 << 8
# asm 1: vpsllw $8,<x3=reg128#11,>v10=reg128#16
# asm 2: vpsllw $8,<x3=%xmm10,>v10=%xmm15
vpsllw $8, % xmm10, % xmm15

# qhasm: 8x v01 = x2 unsigned>> 8
# asm 1: vpsrlw $8,<x2=reg128#12,>v01=reg128#12
# asm 2: vpsrlw $8,<x2=%xmm11,>v01=%xmm11
vpsrlw $8, % xmm11, % xmm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <mask5=reg128#6,<x3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<x3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>x3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>x3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <mask4=reg128#5,<x4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<x4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: 8x v10 = x5 << 8
# asm 1: vpsllw $8,<x5=reg128#9,>v10=reg128#16
# asm 2: vpsllw $8,<x5=%xmm8,>v10=%xmm15
vpsllw $8, % xmm8, % xmm15

# qhasm: 8x v01 = x4 unsigned>> 8
# asm 1: vpsrlw $8,<x4=reg128#13,>v01=reg128#13
# asm 2: vpsrlw $8,<x4=%xmm12,>v01=%xmm12
vpsrlw $8, % xmm12, % xmm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <mask5=reg128#6,<x5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<x5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>x5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>x5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <mask4=reg128#5,<x6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<x6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: 8x v10 = x7 << 8
# asm 1: vpsllw $8,<x7=reg128#8,>v10=reg128#16
# asm 2: vpsllw $8,<x7=%xmm7,>v10=%xmm15
vpsllw $8, % xmm7, % xmm15

# qhasm: 8x v01 = x6 unsigned>> 8
# asm 1: vpsrlw $8,<x6=reg128#7,>v01=reg128#7
# asm 2: vpsrlw $8,<x6=%xmm6,>v01=%xmm6
vpsrlw $8, % xmm6, % xmm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <mask5=reg128#6,<x7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<x7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: x6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>x7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>x7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: mem128[ input_0 + 32 ] = x0
# asm 1: movdqu   <x0=reg128#10,32(<input_0=int64#1)
# asm 2: movdqu   <x0=%xmm9,32(<input_0=%rdi)
movdqu   % xmm9, 32( % rdi)

# qhasm: mem128[ input_0 + 160 ] = x1
# asm 1: movdqu   <x1=reg128#14,160(<input_0=int64#1)
# asm 2: movdqu   <x1=%xmm13,160(<input_0=%rdi)
movdqu   % xmm13, 160( % rdi)

# qhasm: mem128[ input_0 + 288 ] = x2
# asm 1: movdqu   <x2=reg128#15,288(<input_0=int64#1)
# asm 2: movdqu   <x2=%xmm14,288(<input_0=%rdi)
movdqu   % xmm14, 288( % rdi)

# qhasm: mem128[ input_0 + 416 ] = x3
# asm 1: movdqu   <x3=reg128#11,416(<input_0=int64#1)
# asm 2: movdqu   <x3=%xmm10,416(<input_0=%rdi)
movdqu   % xmm10, 416( % rdi)

# qhasm: mem128[ input_0 + 544 ] = x4
# asm 1: movdqu   <x4=reg128#12,544(<input_0=int64#1)
# asm 2: movdqu   <x4=%xmm11,544(<input_0=%rdi)
movdqu   % xmm11, 544( % rdi)

# qhasm: mem128[ input_0 + 672 ] = x5
# asm 1: movdqu   <x5=reg128#9,672(<input_0=int64#1)
# asm 2: movdqu   <x5=%xmm8,672(<input_0=%rdi)
movdqu   % xmm8, 672( % rdi)

# qhasm: mem128[ input_0 + 800 ] = x6
# asm 1: movdqu   <x6=reg128#13,800(<input_0=int64#1)
# asm 2: movdqu   <x6=%xmm12,800(<input_0=%rdi)
movdqu   % xmm12, 800( % rdi)

# qhasm: mem128[ input_0 + 928 ] = x7
# asm 1: movdqu   <x7=reg128#7,928(<input_0=int64#1)
# asm 2: movdqu   <x7=%xmm6,928(<input_0=%rdi)
movdqu   % xmm6, 928( % rdi)

# qhasm: x0 = mem128[ input_0 + 48 ]
# asm 1: movdqu   48(<input_0=int64#1),>x0=reg128#7
# asm 2: movdqu   48(<input_0=%rdi),>x0=%xmm6
movdqu   48( % rdi), % xmm6

# qhasm: x1 = mem128[ input_0 + 176 ]
# asm 1: movdqu   176(<input_0=int64#1),>x1=reg128#8
# asm 2: movdqu   176(<input_0=%rdi),>x1=%xmm7
movdqu   176( % rdi), % xmm7

# qhasm: x2 = mem128[ input_0 + 304 ]
# asm 1: movdqu   304(<input_0=int64#1),>x2=reg128#9
# asm 2: movdqu   304(<input_0=%rdi),>x2=%xmm8
movdqu   304( % rdi), % xmm8

# qhasm: x3 = mem128[ input_0 + 432 ]
# asm 1: movdqu   432(<input_0=int64#1),>x3=reg128#10
# asm 2: movdqu   432(<input_0=%rdi),>x3=%xmm9
movdqu   432( % rdi), % xmm9

# qhasm: x4 = mem128[ input_0 + 560 ]
# asm 1: movdqu   560(<input_0=int64#1),>x4=reg128#11
# asm 2: movdqu   560(<input_0=%rdi),>x4=%xmm10
movdqu   560( % rdi), % xmm10

# qhasm: x5 = mem128[ input_0 + 688 ]
# asm 1: movdqu   688(<input_0=int64#1),>x5=reg128#12
# asm 2: movdqu   688(<input_0=%rdi),>x5=%xmm11
movdqu   688( % rdi), % xmm11

# qhasm: x6 = mem128[ input_0 + 816 ]
# asm 1: movdqu   816(<input_0=int64#1),>x6=reg128#13
# asm 2: movdqu   816(<input_0=%rdi),>x6=%xmm12
movdqu   816( % rdi), % xmm12

# qhasm: x7 = mem128[ input_0 + 944 ]
# asm 1: movdqu   944(<input_0=int64#1),>x7=reg128#14
# asm 2: movdqu   944(<input_0=%rdi),>x7=%xmm13
movdqu   944( % rdi), % xmm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <mask0=reg128#1,<x0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<x0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: 2x v10 = x4 << 32
# asm 1: vpsllq $32,<x4=reg128#11,>v10=reg128#16
# asm 2: vpsllq $32,<x4=%xmm10,>v10=%xmm15
vpsllq $32, % xmm10, % xmm15

# qhasm: 2x v01 = x0 unsigned>> 32
# asm 1: vpsrlq $32,<x0=reg128#7,>v01=reg128#7
# asm 2: vpsrlq $32,<x0=%xmm6,>v01=%xmm6
vpsrlq $32, % xmm6, % xmm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <mask1=reg128#2,<x4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<x4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>x4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>x4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <mask0=reg128#1,<x1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<x1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: 2x v10 = x5 << 32
# asm 1: vpsllq $32,<x5=reg128#12,>v10=reg128#16
# asm 2: vpsllq $32,<x5=%xmm11,>v10=%xmm15
vpsllq $32, % xmm11, % xmm15

# qhasm: 2x v01 = x1 unsigned>> 32
# asm 1: vpsrlq $32,<x1=reg128#8,>v01=reg128#8
# asm 2: vpsrlq $32,<x1=%xmm7,>v01=%xmm7
vpsrlq $32, % xmm7, % xmm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <mask1=reg128#2,<x5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<x5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>x1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>x1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>x5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>x5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <mask0=reg128#1,<x2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<x2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: 2x v10 = x6 << 32
# asm 1: vpsllq $32,<x6=reg128#13,>v10=reg128#16
# asm 2: vpsllq $32,<x6=%xmm12,>v10=%xmm15
vpsllq $32, % xmm12, % xmm15

# qhasm: 2x v01 = x2 unsigned>> 32
# asm 1: vpsrlq $32,<x2=reg128#9,>v01=reg128#9
# asm 2: vpsrlq $32,<x2=%xmm8,>v01=%xmm8
vpsrlq $32, % xmm8, % xmm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <mask1=reg128#2,<x6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<x6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>x6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>x6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <mask0=reg128#1,<x3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<x3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: 2x v10 = x7 << 32
# asm 1: vpsllq $32,<x7=reg128#14,>v10=reg128#16
# asm 2: vpsllq $32,<x7=%xmm13,>v10=%xmm15
vpsllq $32, % xmm13, % xmm15

# qhasm: 2x v01 = x3 unsigned>> 32
# asm 1: vpsrlq $32,<x3=reg128#10,>v01=reg128#10
# asm 2: vpsrlq $32,<x3=%xmm9,>v01=%xmm9
vpsrlq $32, % xmm9, % xmm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <mask1=reg128#2,<x7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<x7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: x3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>x7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>x7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <mask2=reg128#3,<x0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<x0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: 4x v10 = x2 << 16
# asm 1: vpslld $16,<x2=reg128#12,>v10=reg128#16
# asm 2: vpslld $16,<x2=%xmm11,>v10=%xmm15
vpslld $16, % xmm11, % xmm15

# qhasm: 4x v01 = x0 unsigned>> 16
# asm 1: vpsrld $16,<x0=reg128#15,>v01=reg128#15
# asm 2: vpsrld $16,<x0=%xmm14,>v01=%xmm14
vpsrld $16, % xmm14, % xmm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <mask3=reg128#4,<x2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<x2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>x0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>x0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: x2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>x2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>x2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <mask2=reg128#3,<x1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<x1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: 4x v10 = x3 << 16
# asm 1: vpslld $16,<x3=reg128#13,>v10=reg128#16
# asm 2: vpslld $16,<x3=%xmm12,>v10=%xmm15
vpslld $16, % xmm12, % xmm15

# qhasm: 4x v01 = x1 unsigned>> 16
# asm 1: vpsrld $16,<x1=reg128#11,>v01=reg128#11
# asm 2: vpsrld $16,<x1=%xmm10,>v01=%xmm10
vpsrld $16, % xmm10, % xmm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <mask3=reg128#4,<x3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<x3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>x3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>x3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <mask2=reg128#3,<x4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<x4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: 4x v10 = x6 << 16
# asm 1: vpslld $16,<x6=reg128#9,>v10=reg128#16
# asm 2: vpslld $16,<x6=%xmm8,>v10=%xmm15
vpslld $16, % xmm8, % xmm15

# qhasm: 4x v01 = x4 unsigned>> 16
# asm 1: vpsrld $16,<x4=reg128#7,>v01=reg128#7
# asm 2: vpsrld $16,<x4=%xmm6,>v01=%xmm6
vpsrld $16, % xmm6, % xmm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <mask3=reg128#4,<x6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<x6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>x6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>x6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <mask2=reg128#3,<x5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<x5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: 4x v10 = x7 << 16
# asm 1: vpslld $16,<x7=reg128#10,>v10=reg128#16
# asm 2: vpslld $16,<x7=%xmm9,>v10=%xmm15
vpslld $16, % xmm9, % xmm15

# qhasm: 4x v01 = x5 unsigned>> 16
# asm 1: vpsrld $16,<x5=reg128#8,>v01=reg128#8
# asm 2: vpsrld $16,<x5=%xmm7,>v01=%xmm7
vpsrld $16, % xmm7, % xmm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <mask3=reg128#4,<x7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<x7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: x5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>x5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>x5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>x7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>x7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <mask4=reg128#5,<x0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<x0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: 8x v10 = x1 << 8
# asm 1: vpsllw $8,<x1=reg128#15,>v10=reg128#16
# asm 2: vpsllw $8,<x1=%xmm14,>v10=%xmm15
vpsllw $8, % xmm14, % xmm15

# qhasm: 8x v01 = x0 unsigned>> 8
# asm 1: vpsrlw $8,<x0=reg128#14,>v01=reg128#14
# asm 2: vpsrlw $8,<x0=%xmm13,>v01=%xmm13
vpsrlw $8, % xmm13, % xmm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <mask5=reg128#6,<x1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<x1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>x0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>x0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: x1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>x1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>x1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <mask4=reg128#5,<x2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<x2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: 8x v10 = x3 << 8
# asm 1: vpsllw $8,<x3=reg128#11,>v10=reg128#16
# asm 2: vpsllw $8,<x3=%xmm10,>v10=%xmm15
vpsllw $8, % xmm10, % xmm15

# qhasm: 8x v01 = x2 unsigned>> 8
# asm 1: vpsrlw $8,<x2=reg128#12,>v01=reg128#12
# asm 2: vpsrlw $8,<x2=%xmm11,>v01=%xmm11
vpsrlw $8, % xmm11, % xmm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <mask5=reg128#6,<x3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<x3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>x3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>x3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <mask4=reg128#5,<x4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<x4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: 8x v10 = x5 << 8
# asm 1: vpsllw $8,<x5=reg128#9,>v10=reg128#16
# asm 2: vpsllw $8,<x5=%xmm8,>v10=%xmm15
vpsllw $8, % xmm8, % xmm15

# qhasm: 8x v01 = x4 unsigned>> 8
# asm 1: vpsrlw $8,<x4=reg128#13,>v01=reg128#13
# asm 2: vpsrlw $8,<x4=%xmm12,>v01=%xmm12
vpsrlw $8, % xmm12, % xmm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <mask5=reg128#6,<x5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<x5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>x5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>x5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <mask4=reg128#5,<x6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<x6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: 8x v10 = x7 << 8
# asm 1: vpsllw $8,<x7=reg128#8,>v10=reg128#16
# asm 2: vpsllw $8,<x7=%xmm7,>v10=%xmm15
vpsllw $8, % xmm7, % xmm15

# qhasm: 8x v01 = x6 unsigned>> 8
# asm 1: vpsrlw $8,<x6=reg128#7,>v01=reg128#7
# asm 2: vpsrlw $8,<x6=%xmm6,>v01=%xmm6
vpsrlw $8, % xmm6, % xmm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <mask5=reg128#6,<x7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<x7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: x6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>x7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>x7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: mem128[ input_0 + 48 ] = x0
# asm 1: movdqu   <x0=reg128#10,48(<input_0=int64#1)
# asm 2: movdqu   <x0=%xmm9,48(<input_0=%rdi)
movdqu   % xmm9, 48( % rdi)

# qhasm: mem128[ input_0 + 176 ] = x1
# asm 1: movdqu   <x1=reg128#14,176(<input_0=int64#1)
# asm 2: movdqu   <x1=%xmm13,176(<input_0=%rdi)
movdqu   % xmm13, 176( % rdi)

# qhasm: mem128[ input_0 + 304 ] = x2
# asm 1: movdqu   <x2=reg128#15,304(<input_0=int64#1)
# asm 2: movdqu   <x2=%xmm14,304(<input_0=%rdi)
movdqu   % xmm14, 304( % rdi)

# qhasm: mem128[ input_0 + 432 ] = x3
# asm 1: movdqu   <x3=reg128#11,432(<input_0=int64#1)
# asm 2: movdqu   <x3=%xmm10,432(<input_0=%rdi)
movdqu   % xmm10, 432( % rdi)

# qhasm: mem128[ input_0 + 560 ] = x4
# asm 1: movdqu   <x4=reg128#12,560(<input_0=int64#1)
# asm 2: movdqu   <x4=%xmm11,560(<input_0=%rdi)
movdqu   % xmm11, 560( % rdi)

# qhasm: mem128[ input_0 + 688 ] = x5
# asm 1: movdqu   <x5=reg128#9,688(<input_0=int64#1)
# asm 2: movdqu   <x5=%xmm8,688(<input_0=%rdi)
movdqu   % xmm8, 688( % rdi)

# qhasm: mem128[ input_0 + 816 ] = x6
# asm 1: movdqu   <x6=reg128#13,816(<input_0=int64#1)
# asm 2: movdqu   <x6=%xmm12,816(<input_0=%rdi)
movdqu   % xmm12, 816( % rdi)

# qhasm: mem128[ input_0 + 944 ] = x7
# asm 1: movdqu   <x7=reg128#7,944(<input_0=int64#1)
# asm 2: movdqu   <x7=%xmm6,944(<input_0=%rdi)
movdqu   % xmm6, 944( % rdi)

# qhasm: x0 = mem128[ input_0 + 64 ]
# asm 1: movdqu   64(<input_0=int64#1),>x0=reg128#7
# asm 2: movdqu   64(<input_0=%rdi),>x0=%xmm6
movdqu   64( % rdi), % xmm6

# qhasm: x1 = mem128[ input_0 + 192 ]
# asm 1: movdqu   192(<input_0=int64#1),>x1=reg128#8
# asm 2: movdqu   192(<input_0=%rdi),>x1=%xmm7
movdqu   192( % rdi), % xmm7

# qhasm: x2 = mem128[ input_0 + 320 ]
# asm 1: movdqu   320(<input_0=int64#1),>x2=reg128#9
# asm 2: movdqu   320(<input_0=%rdi),>x2=%xmm8
movdqu   320( % rdi), % xmm8

# qhasm: x3 = mem128[ input_0 + 448 ]
# asm 1: movdqu   448(<input_0=int64#1),>x3=reg128#10
# asm 2: movdqu   448(<input_0=%rdi),>x3=%xmm9
movdqu   448( % rdi), % xmm9

# qhasm: x4 = mem128[ input_0 + 576 ]
# asm 1: movdqu   576(<input_0=int64#1),>x4=reg128#11
# asm 2: movdqu   576(<input_0=%rdi),>x4=%xmm10
movdqu   576( % rdi), % xmm10

# qhasm: x5 = mem128[ input_0 + 704 ]
# asm 1: movdqu   704(<input_0=int64#1),>x5=reg128#12
# asm 2: movdqu   704(<input_0=%rdi),>x5=%xmm11
movdqu   704( % rdi), % xmm11

# qhasm: x6 = mem128[ input_0 + 832 ]
# asm 1: movdqu   832(<input_0=int64#1),>x6=reg128#13
# asm 2: movdqu   832(<input_0=%rdi),>x6=%xmm12
movdqu   832( % rdi), % xmm12

# qhasm: x7 = mem128[ input_0 + 960 ]
# asm 1: movdqu   960(<input_0=int64#1),>x7=reg128#14
# asm 2: movdqu   960(<input_0=%rdi),>x7=%xmm13
movdqu   960( % rdi), % xmm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <mask0=reg128#1,<x0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<x0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: 2x v10 = x4 << 32
# asm 1: vpsllq $32,<x4=reg128#11,>v10=reg128#16
# asm 2: vpsllq $32,<x4=%xmm10,>v10=%xmm15
vpsllq $32, % xmm10, % xmm15

# qhasm: 2x v01 = x0 unsigned>> 32
# asm 1: vpsrlq $32,<x0=reg128#7,>v01=reg128#7
# asm 2: vpsrlq $32,<x0=%xmm6,>v01=%xmm6
vpsrlq $32, % xmm6, % xmm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <mask1=reg128#2,<x4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<x4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>x4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>x4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <mask0=reg128#1,<x1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<x1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: 2x v10 = x5 << 32
# asm 1: vpsllq $32,<x5=reg128#12,>v10=reg128#16
# asm 2: vpsllq $32,<x5=%xmm11,>v10=%xmm15
vpsllq $32, % xmm11, % xmm15

# qhasm: 2x v01 = x1 unsigned>> 32
# asm 1: vpsrlq $32,<x1=reg128#8,>v01=reg128#8
# asm 2: vpsrlq $32,<x1=%xmm7,>v01=%xmm7
vpsrlq $32, % xmm7, % xmm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <mask1=reg128#2,<x5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<x5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>x1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>x1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>x5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>x5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <mask0=reg128#1,<x2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<x2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: 2x v10 = x6 << 32
# asm 1: vpsllq $32,<x6=reg128#13,>v10=reg128#16
# asm 2: vpsllq $32,<x6=%xmm12,>v10=%xmm15
vpsllq $32, % xmm12, % xmm15

# qhasm: 2x v01 = x2 unsigned>> 32
# asm 1: vpsrlq $32,<x2=reg128#9,>v01=reg128#9
# asm 2: vpsrlq $32,<x2=%xmm8,>v01=%xmm8
vpsrlq $32, % xmm8, % xmm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <mask1=reg128#2,<x6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<x6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>x6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>x6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <mask0=reg128#1,<x3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<x3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: 2x v10 = x7 << 32
# asm 1: vpsllq $32,<x7=reg128#14,>v10=reg128#16
# asm 2: vpsllq $32,<x7=%xmm13,>v10=%xmm15
vpsllq $32, % xmm13, % xmm15

# qhasm: 2x v01 = x3 unsigned>> 32
# asm 1: vpsrlq $32,<x3=reg128#10,>v01=reg128#10
# asm 2: vpsrlq $32,<x3=%xmm9,>v01=%xmm9
vpsrlq $32, % xmm9, % xmm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <mask1=reg128#2,<x7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<x7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: x3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>x7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>x7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <mask2=reg128#3,<x0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<x0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: 4x v10 = x2 << 16
# asm 1: vpslld $16,<x2=reg128#12,>v10=reg128#16
# asm 2: vpslld $16,<x2=%xmm11,>v10=%xmm15
vpslld $16, % xmm11, % xmm15

# qhasm: 4x v01 = x0 unsigned>> 16
# asm 1: vpsrld $16,<x0=reg128#15,>v01=reg128#15
# asm 2: vpsrld $16,<x0=%xmm14,>v01=%xmm14
vpsrld $16, % xmm14, % xmm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <mask3=reg128#4,<x2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<x2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>x0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>x0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: x2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>x2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>x2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <mask2=reg128#3,<x1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<x1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: 4x v10 = x3 << 16
# asm 1: vpslld $16,<x3=reg128#13,>v10=reg128#16
# asm 2: vpslld $16,<x3=%xmm12,>v10=%xmm15
vpslld $16, % xmm12, % xmm15

# qhasm: 4x v01 = x1 unsigned>> 16
# asm 1: vpsrld $16,<x1=reg128#11,>v01=reg128#11
# asm 2: vpsrld $16,<x1=%xmm10,>v01=%xmm10
vpsrld $16, % xmm10, % xmm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <mask3=reg128#4,<x3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<x3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>x3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>x3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <mask2=reg128#3,<x4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<x4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: 4x v10 = x6 << 16
# asm 1: vpslld $16,<x6=reg128#9,>v10=reg128#16
# asm 2: vpslld $16,<x6=%xmm8,>v10=%xmm15
vpslld $16, % xmm8, % xmm15

# qhasm: 4x v01 = x4 unsigned>> 16
# asm 1: vpsrld $16,<x4=reg128#7,>v01=reg128#7
# asm 2: vpsrld $16,<x4=%xmm6,>v01=%xmm6
vpsrld $16, % xmm6, % xmm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <mask3=reg128#4,<x6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<x6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>x6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>x6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <mask2=reg128#3,<x5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<x5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: 4x v10 = x7 << 16
# asm 1: vpslld $16,<x7=reg128#10,>v10=reg128#16
# asm 2: vpslld $16,<x7=%xmm9,>v10=%xmm15
vpslld $16, % xmm9, % xmm15

# qhasm: 4x v01 = x5 unsigned>> 16
# asm 1: vpsrld $16,<x5=reg128#8,>v01=reg128#8
# asm 2: vpsrld $16,<x5=%xmm7,>v01=%xmm7
vpsrld $16, % xmm7, % xmm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <mask3=reg128#4,<x7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<x7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: x5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>x5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>x5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>x7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>x7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <mask4=reg128#5,<x0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<x0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: 8x v10 = x1 << 8
# asm 1: vpsllw $8,<x1=reg128#15,>v10=reg128#16
# asm 2: vpsllw $8,<x1=%xmm14,>v10=%xmm15
vpsllw $8, % xmm14, % xmm15

# qhasm: 8x v01 = x0 unsigned>> 8
# asm 1: vpsrlw $8,<x0=reg128#14,>v01=reg128#14
# asm 2: vpsrlw $8,<x0=%xmm13,>v01=%xmm13
vpsrlw $8, % xmm13, % xmm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <mask5=reg128#6,<x1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<x1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>x0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>x0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: x1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>x1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>x1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <mask4=reg128#5,<x2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<x2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: 8x v10 = x3 << 8
# asm 1: vpsllw $8,<x3=reg128#11,>v10=reg128#16
# asm 2: vpsllw $8,<x3=%xmm10,>v10=%xmm15
vpsllw $8, % xmm10, % xmm15

# qhasm: 8x v01 = x2 unsigned>> 8
# asm 1: vpsrlw $8,<x2=reg128#12,>v01=reg128#12
# asm 2: vpsrlw $8,<x2=%xmm11,>v01=%xmm11
vpsrlw $8, % xmm11, % xmm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <mask5=reg128#6,<x3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<x3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>x3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>x3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <mask4=reg128#5,<x4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<x4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: 8x v10 = x5 << 8
# asm 1: vpsllw $8,<x5=reg128#9,>v10=reg128#16
# asm 2: vpsllw $8,<x5=%xmm8,>v10=%xmm15
vpsllw $8, % xmm8, % xmm15

# qhasm: 8x v01 = x4 unsigned>> 8
# asm 1: vpsrlw $8,<x4=reg128#13,>v01=reg128#13
# asm 2: vpsrlw $8,<x4=%xmm12,>v01=%xmm12
vpsrlw $8, % xmm12, % xmm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <mask5=reg128#6,<x5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<x5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>x5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>x5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <mask4=reg128#5,<x6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<x6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: 8x v10 = x7 << 8
# asm 1: vpsllw $8,<x7=reg128#8,>v10=reg128#16
# asm 2: vpsllw $8,<x7=%xmm7,>v10=%xmm15
vpsllw $8, % xmm7, % xmm15

# qhasm: 8x v01 = x6 unsigned>> 8
# asm 1: vpsrlw $8,<x6=reg128#7,>v01=reg128#7
# asm 2: vpsrlw $8,<x6=%xmm6,>v01=%xmm6
vpsrlw $8, % xmm6, % xmm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <mask5=reg128#6,<x7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<x7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: x6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>x7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>x7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: mem128[ input_0 + 64 ] = x0
# asm 1: movdqu   <x0=reg128#10,64(<input_0=int64#1)
# asm 2: movdqu   <x0=%xmm9,64(<input_0=%rdi)
movdqu   % xmm9, 64( % rdi)

# qhasm: mem128[ input_0 + 192 ] = x1
# asm 1: movdqu   <x1=reg128#14,192(<input_0=int64#1)
# asm 2: movdqu   <x1=%xmm13,192(<input_0=%rdi)
movdqu   % xmm13, 192( % rdi)

# qhasm: mem128[ input_0 + 320 ] = x2
# asm 1: movdqu   <x2=reg128#15,320(<input_0=int64#1)
# asm 2: movdqu   <x2=%xmm14,320(<input_0=%rdi)
movdqu   % xmm14, 320( % rdi)

# qhasm: mem128[ input_0 + 448 ] = x3
# asm 1: movdqu   <x3=reg128#11,448(<input_0=int64#1)
# asm 2: movdqu   <x3=%xmm10,448(<input_0=%rdi)
movdqu   % xmm10, 448( % rdi)

# qhasm: mem128[ input_0 + 576 ] = x4
# asm 1: movdqu   <x4=reg128#12,576(<input_0=int64#1)
# asm 2: movdqu   <x4=%xmm11,576(<input_0=%rdi)
movdqu   % xmm11, 576( % rdi)

# qhasm: mem128[ input_0 + 704 ] = x5
# asm 1: movdqu   <x5=reg128#9,704(<input_0=int64#1)
# asm 2: movdqu   <x5=%xmm8,704(<input_0=%rdi)
movdqu   % xmm8, 704( % rdi)

# qhasm: mem128[ input_0 + 832 ] = x6
# asm 1: movdqu   <x6=reg128#13,832(<input_0=int64#1)
# asm 2: movdqu   <x6=%xmm12,832(<input_0=%rdi)
movdqu   % xmm12, 832( % rdi)

# qhasm: mem128[ input_0 + 960 ] = x7
# asm 1: movdqu   <x7=reg128#7,960(<input_0=int64#1)
# asm 2: movdqu   <x7=%xmm6,960(<input_0=%rdi)
movdqu   % xmm6, 960( % rdi)

# qhasm: x0 = mem128[ input_0 + 80 ]
# asm 1: movdqu   80(<input_0=int64#1),>x0=reg128#7
# asm 2: movdqu   80(<input_0=%rdi),>x0=%xmm6
movdqu   80( % rdi), % xmm6

# qhasm: x1 = mem128[ input_0 + 208 ]
# asm 1: movdqu   208(<input_0=int64#1),>x1=reg128#8
# asm 2: movdqu   208(<input_0=%rdi),>x1=%xmm7
movdqu   208( % rdi), % xmm7

# qhasm: x2 = mem128[ input_0 + 336 ]
# asm 1: movdqu   336(<input_0=int64#1),>x2=reg128#9
# asm 2: movdqu   336(<input_0=%rdi),>x2=%xmm8
movdqu   336( % rdi), % xmm8

# qhasm: x3 = mem128[ input_0 + 464 ]
# asm 1: movdqu   464(<input_0=int64#1),>x3=reg128#10
# asm 2: movdqu   464(<input_0=%rdi),>x3=%xmm9
movdqu   464( % rdi), % xmm9

# qhasm: x4 = mem128[ input_0 + 592 ]
# asm 1: movdqu   592(<input_0=int64#1),>x4=reg128#11
# asm 2: movdqu   592(<input_0=%rdi),>x4=%xmm10
movdqu   592( % rdi), % xmm10

# qhasm: x5 = mem128[ input_0 + 720 ]
# asm 1: movdqu   720(<input_0=int64#1),>x5=reg128#12
# asm 2: movdqu   720(<input_0=%rdi),>x5=%xmm11
movdqu   720( % rdi), % xmm11

# qhasm: x6 = mem128[ input_0 + 848 ]
# asm 1: movdqu   848(<input_0=int64#1),>x6=reg128#13
# asm 2: movdqu   848(<input_0=%rdi),>x6=%xmm12
movdqu   848( % rdi), % xmm12

# qhasm: x7 = mem128[ input_0 + 976 ]
# asm 1: movdqu   976(<input_0=int64#1),>x7=reg128#14
# asm 2: movdqu   976(<input_0=%rdi),>x7=%xmm13
movdqu   976( % rdi), % xmm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <mask0=reg128#1,<x0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<x0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: 2x v10 = x4 << 32
# asm 1: vpsllq $32,<x4=reg128#11,>v10=reg128#16
# asm 2: vpsllq $32,<x4=%xmm10,>v10=%xmm15
vpsllq $32, % xmm10, % xmm15

# qhasm: 2x v01 = x0 unsigned>> 32
# asm 1: vpsrlq $32,<x0=reg128#7,>v01=reg128#7
# asm 2: vpsrlq $32,<x0=%xmm6,>v01=%xmm6
vpsrlq $32, % xmm6, % xmm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <mask1=reg128#2,<x4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<x4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>x4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>x4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <mask0=reg128#1,<x1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<x1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: 2x v10 = x5 << 32
# asm 1: vpsllq $32,<x5=reg128#12,>v10=reg128#16
# asm 2: vpsllq $32,<x5=%xmm11,>v10=%xmm15
vpsllq $32, % xmm11, % xmm15

# qhasm: 2x v01 = x1 unsigned>> 32
# asm 1: vpsrlq $32,<x1=reg128#8,>v01=reg128#8
# asm 2: vpsrlq $32,<x1=%xmm7,>v01=%xmm7
vpsrlq $32, % xmm7, % xmm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <mask1=reg128#2,<x5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<x5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>x1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>x1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>x5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>x5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <mask0=reg128#1,<x2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<x2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: 2x v10 = x6 << 32
# asm 1: vpsllq $32,<x6=reg128#13,>v10=reg128#16
# asm 2: vpsllq $32,<x6=%xmm12,>v10=%xmm15
vpsllq $32, % xmm12, % xmm15

# qhasm: 2x v01 = x2 unsigned>> 32
# asm 1: vpsrlq $32,<x2=reg128#9,>v01=reg128#9
# asm 2: vpsrlq $32,<x2=%xmm8,>v01=%xmm8
vpsrlq $32, % xmm8, % xmm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <mask1=reg128#2,<x6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<x6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>x6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>x6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <mask0=reg128#1,<x3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<x3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: 2x v10 = x7 << 32
# asm 1: vpsllq $32,<x7=reg128#14,>v10=reg128#16
# asm 2: vpsllq $32,<x7=%xmm13,>v10=%xmm15
vpsllq $32, % xmm13, % xmm15

# qhasm: 2x v01 = x3 unsigned>> 32
# asm 1: vpsrlq $32,<x3=reg128#10,>v01=reg128#10
# asm 2: vpsrlq $32,<x3=%xmm9,>v01=%xmm9
vpsrlq $32, % xmm9, % xmm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <mask1=reg128#2,<x7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<x7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: x3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>x7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>x7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <mask2=reg128#3,<x0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<x0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: 4x v10 = x2 << 16
# asm 1: vpslld $16,<x2=reg128#12,>v10=reg128#16
# asm 2: vpslld $16,<x2=%xmm11,>v10=%xmm15
vpslld $16, % xmm11, % xmm15

# qhasm: 4x v01 = x0 unsigned>> 16
# asm 1: vpsrld $16,<x0=reg128#15,>v01=reg128#15
# asm 2: vpsrld $16,<x0=%xmm14,>v01=%xmm14
vpsrld $16, % xmm14, % xmm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <mask3=reg128#4,<x2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<x2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>x0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>x0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: x2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>x2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>x2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <mask2=reg128#3,<x1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<x1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: 4x v10 = x3 << 16
# asm 1: vpslld $16,<x3=reg128#13,>v10=reg128#16
# asm 2: vpslld $16,<x3=%xmm12,>v10=%xmm15
vpslld $16, % xmm12, % xmm15

# qhasm: 4x v01 = x1 unsigned>> 16
# asm 1: vpsrld $16,<x1=reg128#11,>v01=reg128#11
# asm 2: vpsrld $16,<x1=%xmm10,>v01=%xmm10
vpsrld $16, % xmm10, % xmm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <mask3=reg128#4,<x3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<x3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>x3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>x3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <mask2=reg128#3,<x4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<x4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: 4x v10 = x6 << 16
# asm 1: vpslld $16,<x6=reg128#9,>v10=reg128#16
# asm 2: vpslld $16,<x6=%xmm8,>v10=%xmm15
vpslld $16, % xmm8, % xmm15

# qhasm: 4x v01 = x4 unsigned>> 16
# asm 1: vpsrld $16,<x4=reg128#7,>v01=reg128#7
# asm 2: vpsrld $16,<x4=%xmm6,>v01=%xmm6
vpsrld $16, % xmm6, % xmm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <mask3=reg128#4,<x6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<x6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>x6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>x6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <mask2=reg128#3,<x5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<x5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: 4x v10 = x7 << 16
# asm 1: vpslld $16,<x7=reg128#10,>v10=reg128#16
# asm 2: vpslld $16,<x7=%xmm9,>v10=%xmm15
vpslld $16, % xmm9, % xmm15

# qhasm: 4x v01 = x5 unsigned>> 16
# asm 1: vpsrld $16,<x5=reg128#8,>v01=reg128#8
# asm 2: vpsrld $16,<x5=%xmm7,>v01=%xmm7
vpsrld $16, % xmm7, % xmm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <mask3=reg128#4,<x7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<x7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: x5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>x5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>x5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>x7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>x7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <mask4=reg128#5,<x0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<x0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: 8x v10 = x1 << 8
# asm 1: vpsllw $8,<x1=reg128#15,>v10=reg128#16
# asm 2: vpsllw $8,<x1=%xmm14,>v10=%xmm15
vpsllw $8, % xmm14, % xmm15

# qhasm: 8x v01 = x0 unsigned>> 8
# asm 1: vpsrlw $8,<x0=reg128#14,>v01=reg128#14
# asm 2: vpsrlw $8,<x0=%xmm13,>v01=%xmm13
vpsrlw $8, % xmm13, % xmm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <mask5=reg128#6,<x1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<x1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>x0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>x0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: x1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>x1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>x1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <mask4=reg128#5,<x2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<x2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: 8x v10 = x3 << 8
# asm 1: vpsllw $8,<x3=reg128#11,>v10=reg128#16
# asm 2: vpsllw $8,<x3=%xmm10,>v10=%xmm15
vpsllw $8, % xmm10, % xmm15

# qhasm: 8x v01 = x2 unsigned>> 8
# asm 1: vpsrlw $8,<x2=reg128#12,>v01=reg128#12
# asm 2: vpsrlw $8,<x2=%xmm11,>v01=%xmm11
vpsrlw $8, % xmm11, % xmm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <mask5=reg128#6,<x3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<x3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>x3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>x3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <mask4=reg128#5,<x4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<x4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: 8x v10 = x5 << 8
# asm 1: vpsllw $8,<x5=reg128#9,>v10=reg128#16
# asm 2: vpsllw $8,<x5=%xmm8,>v10=%xmm15
vpsllw $8, % xmm8, % xmm15

# qhasm: 8x v01 = x4 unsigned>> 8
# asm 1: vpsrlw $8,<x4=reg128#13,>v01=reg128#13
# asm 2: vpsrlw $8,<x4=%xmm12,>v01=%xmm12
vpsrlw $8, % xmm12, % xmm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <mask5=reg128#6,<x5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<x5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>x5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>x5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <mask4=reg128#5,<x6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<x6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: 8x v10 = x7 << 8
# asm 1: vpsllw $8,<x7=reg128#8,>v10=reg128#16
# asm 2: vpsllw $8,<x7=%xmm7,>v10=%xmm15
vpsllw $8, % xmm7, % xmm15

# qhasm: 8x v01 = x6 unsigned>> 8
# asm 1: vpsrlw $8,<x6=reg128#7,>v01=reg128#7
# asm 2: vpsrlw $8,<x6=%xmm6,>v01=%xmm6
vpsrlw $8, % xmm6, % xmm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <mask5=reg128#6,<x7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<x7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: x6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>x7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>x7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: mem128[ input_0 + 80 ] = x0
# asm 1: movdqu   <x0=reg128#10,80(<input_0=int64#1)
# asm 2: movdqu   <x0=%xmm9,80(<input_0=%rdi)
movdqu   % xmm9, 80( % rdi)

# qhasm: mem128[ input_0 + 208 ] = x1
# asm 1: movdqu   <x1=reg128#14,208(<input_0=int64#1)
# asm 2: movdqu   <x1=%xmm13,208(<input_0=%rdi)
movdqu   % xmm13, 208( % rdi)

# qhasm: mem128[ input_0 + 336 ] = x2
# asm 1: movdqu   <x2=reg128#15,336(<input_0=int64#1)
# asm 2: movdqu   <x2=%xmm14,336(<input_0=%rdi)
movdqu   % xmm14, 336( % rdi)

# qhasm: mem128[ input_0 + 464 ] = x3
# asm 1: movdqu   <x3=reg128#11,464(<input_0=int64#1)
# asm 2: movdqu   <x3=%xmm10,464(<input_0=%rdi)
movdqu   % xmm10, 464( % rdi)

# qhasm: mem128[ input_0 + 592 ] = x4
# asm 1: movdqu   <x4=reg128#12,592(<input_0=int64#1)
# asm 2: movdqu   <x4=%xmm11,592(<input_0=%rdi)
movdqu   % xmm11, 592( % rdi)

# qhasm: mem128[ input_0 + 720 ] = x5
# asm 1: movdqu   <x5=reg128#9,720(<input_0=int64#1)
# asm 2: movdqu   <x5=%xmm8,720(<input_0=%rdi)
movdqu   % xmm8, 720( % rdi)

# qhasm: mem128[ input_0 + 848 ] = x6
# asm 1: movdqu   <x6=reg128#13,848(<input_0=int64#1)
# asm 2: movdqu   <x6=%xmm12,848(<input_0=%rdi)
movdqu   % xmm12, 848( % rdi)

# qhasm: mem128[ input_0 + 976 ] = x7
# asm 1: movdqu   <x7=reg128#7,976(<input_0=int64#1)
# asm 2: movdqu   <x7=%xmm6,976(<input_0=%rdi)
movdqu   % xmm6, 976( % rdi)

# qhasm: x0 = mem128[ input_0 + 96 ]
# asm 1: movdqu   96(<input_0=int64#1),>x0=reg128#7
# asm 2: movdqu   96(<input_0=%rdi),>x0=%xmm6
movdqu   96( % rdi), % xmm6

# qhasm: x1 = mem128[ input_0 + 224 ]
# asm 1: movdqu   224(<input_0=int64#1),>x1=reg128#8
# asm 2: movdqu   224(<input_0=%rdi),>x1=%xmm7
movdqu   224( % rdi), % xmm7

# qhasm: x2 = mem128[ input_0 + 352 ]
# asm 1: movdqu   352(<input_0=int64#1),>x2=reg128#9
# asm 2: movdqu   352(<input_0=%rdi),>x2=%xmm8
movdqu   352( % rdi), % xmm8

# qhasm: x3 = mem128[ input_0 + 480 ]
# asm 1: movdqu   480(<input_0=int64#1),>x3=reg128#10
# asm 2: movdqu   480(<input_0=%rdi),>x3=%xmm9
movdqu   480( % rdi), % xmm9

# qhasm: x4 = mem128[ input_0 + 608 ]
# asm 1: movdqu   608(<input_0=int64#1),>x4=reg128#11
# asm 2: movdqu   608(<input_0=%rdi),>x4=%xmm10
movdqu   608( % rdi), % xmm10

# qhasm: x5 = mem128[ input_0 + 736 ]
# asm 1: movdqu   736(<input_0=int64#1),>x5=reg128#12
# asm 2: movdqu   736(<input_0=%rdi),>x5=%xmm11
movdqu   736( % rdi), % xmm11

# qhasm: x6 = mem128[ input_0 + 864 ]
# asm 1: movdqu   864(<input_0=int64#1),>x6=reg128#13
# asm 2: movdqu   864(<input_0=%rdi),>x6=%xmm12
movdqu   864( % rdi), % xmm12

# qhasm: x7 = mem128[ input_0 + 992 ]
# asm 1: movdqu   992(<input_0=int64#1),>x7=reg128#14
# asm 2: movdqu   992(<input_0=%rdi),>x7=%xmm13
movdqu   992( % rdi), % xmm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <mask0=reg128#1,<x0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<x0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: 2x v10 = x4 << 32
# asm 1: vpsllq $32,<x4=reg128#11,>v10=reg128#16
# asm 2: vpsllq $32,<x4=%xmm10,>v10=%xmm15
vpsllq $32, % xmm10, % xmm15

# qhasm: 2x v01 = x0 unsigned>> 32
# asm 1: vpsrlq $32,<x0=reg128#7,>v01=reg128#7
# asm 2: vpsrlq $32,<x0=%xmm6,>v01=%xmm6
vpsrlq $32, % xmm6, % xmm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <mask1=reg128#2,<x4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<x4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>x4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>x4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <mask0=reg128#1,<x1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<x1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: 2x v10 = x5 << 32
# asm 1: vpsllq $32,<x5=reg128#12,>v10=reg128#16
# asm 2: vpsllq $32,<x5=%xmm11,>v10=%xmm15
vpsllq $32, % xmm11, % xmm15

# qhasm: 2x v01 = x1 unsigned>> 32
# asm 1: vpsrlq $32,<x1=reg128#8,>v01=reg128#8
# asm 2: vpsrlq $32,<x1=%xmm7,>v01=%xmm7
vpsrlq $32, % xmm7, % xmm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <mask1=reg128#2,<x5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<x5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>x1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>x1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>x5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>x5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <mask0=reg128#1,<x2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<x2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: 2x v10 = x6 << 32
# asm 1: vpsllq $32,<x6=reg128#13,>v10=reg128#16
# asm 2: vpsllq $32,<x6=%xmm12,>v10=%xmm15
vpsllq $32, % xmm12, % xmm15

# qhasm: 2x v01 = x2 unsigned>> 32
# asm 1: vpsrlq $32,<x2=reg128#9,>v01=reg128#9
# asm 2: vpsrlq $32,<x2=%xmm8,>v01=%xmm8
vpsrlq $32, % xmm8, % xmm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <mask1=reg128#2,<x6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<x6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>x6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>x6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <mask0=reg128#1,<x3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<x3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: 2x v10 = x7 << 32
# asm 1: vpsllq $32,<x7=reg128#14,>v10=reg128#16
# asm 2: vpsllq $32,<x7=%xmm13,>v10=%xmm15
vpsllq $32, % xmm13, % xmm15

# qhasm: 2x v01 = x3 unsigned>> 32
# asm 1: vpsrlq $32,<x3=reg128#10,>v01=reg128#10
# asm 2: vpsrlq $32,<x3=%xmm9,>v01=%xmm9
vpsrlq $32, % xmm9, % xmm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <mask1=reg128#2,<x7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<x7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: x3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>x7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>x7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <mask2=reg128#3,<x0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<x0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: 4x v10 = x2 << 16
# asm 1: vpslld $16,<x2=reg128#12,>v10=reg128#16
# asm 2: vpslld $16,<x2=%xmm11,>v10=%xmm15
vpslld $16, % xmm11, % xmm15

# qhasm: 4x v01 = x0 unsigned>> 16
# asm 1: vpsrld $16,<x0=reg128#15,>v01=reg128#15
# asm 2: vpsrld $16,<x0=%xmm14,>v01=%xmm14
vpsrld $16, % xmm14, % xmm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <mask3=reg128#4,<x2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<x2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>x0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>x0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: x2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>x2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>x2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <mask2=reg128#3,<x1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<x1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: 4x v10 = x3 << 16
# asm 1: vpslld $16,<x3=reg128#13,>v10=reg128#16
# asm 2: vpslld $16,<x3=%xmm12,>v10=%xmm15
vpslld $16, % xmm12, % xmm15

# qhasm: 4x v01 = x1 unsigned>> 16
# asm 1: vpsrld $16,<x1=reg128#11,>v01=reg128#11
# asm 2: vpsrld $16,<x1=%xmm10,>v01=%xmm10
vpsrld $16, % xmm10, % xmm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <mask3=reg128#4,<x3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<x3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>x3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>x3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <mask2=reg128#3,<x4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<x4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: 4x v10 = x6 << 16
# asm 1: vpslld $16,<x6=reg128#9,>v10=reg128#16
# asm 2: vpslld $16,<x6=%xmm8,>v10=%xmm15
vpslld $16, % xmm8, % xmm15

# qhasm: 4x v01 = x4 unsigned>> 16
# asm 1: vpsrld $16,<x4=reg128#7,>v01=reg128#7
# asm 2: vpsrld $16,<x4=%xmm6,>v01=%xmm6
vpsrld $16, % xmm6, % xmm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <mask3=reg128#4,<x6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<x6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>x6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>x6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <mask2=reg128#3,<x5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<x5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: 4x v10 = x7 << 16
# asm 1: vpslld $16,<x7=reg128#10,>v10=reg128#16
# asm 2: vpslld $16,<x7=%xmm9,>v10=%xmm15
vpslld $16, % xmm9, % xmm15

# qhasm: 4x v01 = x5 unsigned>> 16
# asm 1: vpsrld $16,<x5=reg128#8,>v01=reg128#8
# asm 2: vpsrld $16,<x5=%xmm7,>v01=%xmm7
vpsrld $16, % xmm7, % xmm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <mask3=reg128#4,<x7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<x7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: x5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>x5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>x5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>x7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>x7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <mask4=reg128#5,<x0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<x0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: 8x v10 = x1 << 8
# asm 1: vpsllw $8,<x1=reg128#15,>v10=reg128#16
# asm 2: vpsllw $8,<x1=%xmm14,>v10=%xmm15
vpsllw $8, % xmm14, % xmm15

# qhasm: 8x v01 = x0 unsigned>> 8
# asm 1: vpsrlw $8,<x0=reg128#14,>v01=reg128#14
# asm 2: vpsrlw $8,<x0=%xmm13,>v01=%xmm13
vpsrlw $8, % xmm13, % xmm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <mask5=reg128#6,<x1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<x1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>x0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>x0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: x1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>x1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>x1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <mask4=reg128#5,<x2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<x2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: 8x v10 = x3 << 8
# asm 1: vpsllw $8,<x3=reg128#11,>v10=reg128#16
# asm 2: vpsllw $8,<x3=%xmm10,>v10=%xmm15
vpsllw $8, % xmm10, % xmm15

# qhasm: 8x v01 = x2 unsigned>> 8
# asm 1: vpsrlw $8,<x2=reg128#12,>v01=reg128#12
# asm 2: vpsrlw $8,<x2=%xmm11,>v01=%xmm11
vpsrlw $8, % xmm11, % xmm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <mask5=reg128#6,<x3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<x3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>x3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>x3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <mask4=reg128#5,<x4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<x4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: 8x v10 = x5 << 8
# asm 1: vpsllw $8,<x5=reg128#9,>v10=reg128#16
# asm 2: vpsllw $8,<x5=%xmm8,>v10=%xmm15
vpsllw $8, % xmm8, % xmm15

# qhasm: 8x v01 = x4 unsigned>> 8
# asm 1: vpsrlw $8,<x4=reg128#13,>v01=reg128#13
# asm 2: vpsrlw $8,<x4=%xmm12,>v01=%xmm12
vpsrlw $8, % xmm12, % xmm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <mask5=reg128#6,<x5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<x5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>x5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>x5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <mask4=reg128#5,<x6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<x6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: 8x v10 = x7 << 8
# asm 1: vpsllw $8,<x7=reg128#8,>v10=reg128#16
# asm 2: vpsllw $8,<x7=%xmm7,>v10=%xmm15
vpsllw $8, % xmm7, % xmm15

# qhasm: 8x v01 = x6 unsigned>> 8
# asm 1: vpsrlw $8,<x6=reg128#7,>v01=reg128#7
# asm 2: vpsrlw $8,<x6=%xmm6,>v01=%xmm6
vpsrlw $8, % xmm6, % xmm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <mask5=reg128#6,<x7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<x7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: x6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>x7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>x7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: mem128[ input_0 + 96 ] = x0
# asm 1: movdqu   <x0=reg128#10,96(<input_0=int64#1)
# asm 2: movdqu   <x0=%xmm9,96(<input_0=%rdi)
movdqu   % xmm9, 96( % rdi)

# qhasm: mem128[ input_0 + 224 ] = x1
# asm 1: movdqu   <x1=reg128#14,224(<input_0=int64#1)
# asm 2: movdqu   <x1=%xmm13,224(<input_0=%rdi)
movdqu   % xmm13, 224( % rdi)

# qhasm: mem128[ input_0 + 352 ] = x2
# asm 1: movdqu   <x2=reg128#15,352(<input_0=int64#1)
# asm 2: movdqu   <x2=%xmm14,352(<input_0=%rdi)
movdqu   % xmm14, 352( % rdi)

# qhasm: mem128[ input_0 + 480 ] = x3
# asm 1: movdqu   <x3=reg128#11,480(<input_0=int64#1)
# asm 2: movdqu   <x3=%xmm10,480(<input_0=%rdi)
movdqu   % xmm10, 480( % rdi)

# qhasm: mem128[ input_0 + 608 ] = x4
# asm 1: movdqu   <x4=reg128#12,608(<input_0=int64#1)
# asm 2: movdqu   <x4=%xmm11,608(<input_0=%rdi)
movdqu   % xmm11, 608( % rdi)

# qhasm: mem128[ input_0 + 736 ] = x5
# asm 1: movdqu   <x5=reg128#9,736(<input_0=int64#1)
# asm 2: movdqu   <x5=%xmm8,736(<input_0=%rdi)
movdqu   % xmm8, 736( % rdi)

# qhasm: mem128[ input_0 + 864 ] = x6
# asm 1: movdqu   <x6=reg128#13,864(<input_0=int64#1)
# asm 2: movdqu   <x6=%xmm12,864(<input_0=%rdi)
movdqu   % xmm12, 864( % rdi)

# qhasm: mem128[ input_0 + 992 ] = x7
# asm 1: movdqu   <x7=reg128#7,992(<input_0=int64#1)
# asm 2: movdqu   <x7=%xmm6,992(<input_0=%rdi)
movdqu   % xmm6, 992( % rdi)

# qhasm: x0 = mem128[ input_0 + 112 ]
# asm 1: movdqu   112(<input_0=int64#1),>x0=reg128#7
# asm 2: movdqu   112(<input_0=%rdi),>x0=%xmm6
movdqu   112( % rdi), % xmm6

# qhasm: x1 = mem128[ input_0 + 240 ]
# asm 1: movdqu   240(<input_0=int64#1),>x1=reg128#8
# asm 2: movdqu   240(<input_0=%rdi),>x1=%xmm7
movdqu   240( % rdi), % xmm7

# qhasm: x2 = mem128[ input_0 + 368 ]
# asm 1: movdqu   368(<input_0=int64#1),>x2=reg128#9
# asm 2: movdqu   368(<input_0=%rdi),>x2=%xmm8
movdqu   368( % rdi), % xmm8

# qhasm: x3 = mem128[ input_0 + 496 ]
# asm 1: movdqu   496(<input_0=int64#1),>x3=reg128#10
# asm 2: movdqu   496(<input_0=%rdi),>x3=%xmm9
movdqu   496( % rdi), % xmm9

# qhasm: x4 = mem128[ input_0 + 624 ]
# asm 1: movdqu   624(<input_0=int64#1),>x4=reg128#11
# asm 2: movdqu   624(<input_0=%rdi),>x4=%xmm10
movdqu   624( % rdi), % xmm10

# qhasm: x5 = mem128[ input_0 + 752 ]
# asm 1: movdqu   752(<input_0=int64#1),>x5=reg128#12
# asm 2: movdqu   752(<input_0=%rdi),>x5=%xmm11
movdqu   752( % rdi), % xmm11

# qhasm: x6 = mem128[ input_0 + 880 ]
# asm 1: movdqu   880(<input_0=int64#1),>x6=reg128#13
# asm 2: movdqu   880(<input_0=%rdi),>x6=%xmm12
movdqu   880( % rdi), % xmm12

# qhasm: x7 = mem128[ input_0 + 1008 ]
# asm 1: movdqu   1008(<input_0=int64#1),>x7=reg128#14
# asm 2: movdqu   1008(<input_0=%rdi),>x7=%xmm13
movdqu   1008( % rdi), % xmm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <mask0=reg128#1,<x0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<x0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: 2x v10 = x4 << 32
# asm 1: vpsllq $32,<x4=reg128#11,>v10=reg128#16
# asm 2: vpsllq $32,<x4=%xmm10,>v10=%xmm15
vpsllq $32, % xmm10, % xmm15

# qhasm: 2x v01 = x0 unsigned>> 32
# asm 1: vpsrlq $32,<x0=reg128#7,>v01=reg128#7
# asm 2: vpsrlq $32,<x0=%xmm6,>v01=%xmm6
vpsrlq $32, % xmm6, % xmm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <mask1=reg128#2,<x4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<x4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>x4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>x4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <mask0=reg128#1,<x1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<x1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: 2x v10 = x5 << 32
# asm 1: vpsllq $32,<x5=reg128#12,>v10=reg128#16
# asm 2: vpsllq $32,<x5=%xmm11,>v10=%xmm15
vpsllq $32, % xmm11, % xmm15

# qhasm: 2x v01 = x1 unsigned>> 32
# asm 1: vpsrlq $32,<x1=reg128#8,>v01=reg128#8
# asm 2: vpsrlq $32,<x1=%xmm7,>v01=%xmm7
vpsrlq $32, % xmm7, % xmm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <mask1=reg128#2,<x5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<x5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>x1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>x1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>x5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>x5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <mask0=reg128#1,<x2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<x2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: 2x v10 = x6 << 32
# asm 1: vpsllq $32,<x6=reg128#13,>v10=reg128#16
# asm 2: vpsllq $32,<x6=%xmm12,>v10=%xmm15
vpsllq $32, % xmm12, % xmm15

# qhasm: 2x v01 = x2 unsigned>> 32
# asm 1: vpsrlq $32,<x2=reg128#9,>v01=reg128#9
# asm 2: vpsrlq $32,<x2=%xmm8,>v01=%xmm8
vpsrlq $32, % xmm8, % xmm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <mask1=reg128#2,<x6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<x6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>x6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>x6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <mask0=reg128#1,<x3=reg128#10,>v00=reg128#1
# asm 2: vpand <mask0=%xmm0,<x3=%xmm9,>v00=%xmm0
vpand % xmm0, % xmm9, % xmm0

# qhasm: 2x v10 = x7 << 32
# asm 1: vpsllq $32,<x7=reg128#14,>v10=reg128#13
# asm 2: vpsllq $32,<x7=%xmm13,>v10=%xmm12
vpsllq $32, % xmm13, % xmm12

# qhasm: 2x v01 = x3 unsigned>> 32
# asm 1: vpsrlq $32,<x3=reg128#10,>v01=reg128#10
# asm 2: vpsrlq $32,<x3=%xmm9,>v01=%xmm9
vpsrlq $32, % xmm9, % xmm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <mask1=reg128#2,<x7=reg128#14,>v11=reg128#2
# asm 2: vpand <mask1=%xmm1,<x7=%xmm13,>v11=%xmm1
vpand % xmm1, % xmm13, % xmm1

# qhasm: x3 = v00 | v10
# asm 1: vpor <v10=reg128#13,<v00=reg128#1,>x3=reg128#1
# asm 2: vpor <v10=%xmm12,<v00=%xmm0,>x3=%xmm0
vpor % xmm12, % xmm0, % xmm0

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#2,<v01=reg128#10,>x7=reg128#2
# asm 2: vpor <v11=%xmm1,<v01=%xmm9,>x7=%xmm1
vpor % xmm1, % xmm9, % xmm1

# qhasm: v00 = x0 & mask2
# asm 1: vpand <mask2=reg128#3,<x0=reg128#15,>v00=reg128#10
# asm 2: vpand <mask2=%xmm2,<x0=%xmm14,>v00=%xmm9
vpand % xmm2, % xmm14, % xmm9

# qhasm: 4x v10 = x2 << 16
# asm 1: vpslld $16,<x2=reg128#12,>v10=reg128#13
# asm 2: vpslld $16,<x2=%xmm11,>v10=%xmm12
vpslld $16, % xmm11, % xmm12

# qhasm: 4x v01 = x0 unsigned>> 16
# asm 1: vpsrld $16,<x0=reg128#15,>v01=reg128#14
# asm 2: vpsrld $16,<x0=%xmm14,>v01=%xmm13
vpsrld $16, % xmm14, % xmm13

# qhasm: v11 = x2 & mask3
# asm 1: vpand <mask3=reg128#4,<x2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<x2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#13,<v00=reg128#10,>x0=reg128#10
# asm 2: vpor <v10=%xmm12,<v00=%xmm9,>x0=%xmm9
vpor % xmm12, % xmm9, % xmm9

# qhasm: x2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#14,>x2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm13,>x2=%xmm11
vpor % xmm11, % xmm13, % xmm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <mask2=reg128#3,<x1=reg128#11,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<x1=%xmm10,>v00=%xmm12
vpand % xmm2, % xmm10, % xmm12

# qhasm: 4x v10 = x3 << 16
# asm 1: vpslld $16,<x3=reg128#1,>v10=reg128#14
# asm 2: vpslld $16,<x3=%xmm0,>v10=%xmm13
vpslld $16, % xmm0, % xmm13

# qhasm: 4x v01 = x1 unsigned>> 16
# asm 1: vpsrld $16,<x1=reg128#11,>v01=reg128#11
# asm 2: vpsrld $16,<x1=%xmm10,>v01=%xmm10
vpsrld $16, % xmm10, % xmm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <mask3=reg128#4,<x3=reg128#1,>v11=reg128#1
# asm 2: vpand <mask3=%xmm3,<x3=%xmm0,>v11=%xmm0
vpand % xmm3, % xmm0, % xmm0

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#14,<v00=reg128#13,>x1=reg128#13
# asm 2: vpor <v10=%xmm13,<v00=%xmm12,>x1=%xmm12
vpor % xmm13, % xmm12, % xmm12

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#1,<v01=reg128#11,>x3=reg128#1
# asm 2: vpor <v11=%xmm0,<v01=%xmm10,>x3=%xmm0
vpor % xmm0, % xmm10, % xmm0

# qhasm: v00 = x4 & mask2
# asm 1: vpand <mask2=reg128#3,<x4=reg128#7,>v00=reg128#11
# asm 2: vpand <mask2=%xmm2,<x4=%xmm6,>v00=%xmm10
vpand % xmm2, % xmm6, % xmm10

# qhasm: 4x v10 = x6 << 16
# asm 1: vpslld $16,<x6=reg128#9,>v10=reg128#14
# asm 2: vpslld $16,<x6=%xmm8,>v10=%xmm13
vpslld $16, % xmm8, % xmm13

# qhasm: 4x v01 = x4 unsigned>> 16
# asm 1: vpsrld $16,<x4=reg128#7,>v01=reg128#7
# asm 2: vpsrld $16,<x4=%xmm6,>v01=%xmm6
vpsrld $16, % xmm6, % xmm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <mask3=reg128#4,<x6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<x6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#14,<v00=reg128#11,>x4=reg128#11
# asm 2: vpor <v10=%xmm13,<v00=%xmm10,>x4=%xmm10
vpor % xmm13, % xmm10, % xmm10

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>x6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>x6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <mask2=reg128#3,<x5=reg128#8,>v00=reg128#3
# asm 2: vpand <mask2=%xmm2,<x5=%xmm7,>v00=%xmm2
vpand % xmm2, % xmm7, % xmm2

# qhasm: 4x v10 = x7 << 16
# asm 1: vpslld $16,<x7=reg128#2,>v10=reg128#9
# asm 2: vpslld $16,<x7=%xmm1,>v10=%xmm8
vpslld $16, % xmm1, % xmm8

# qhasm: 4x v01 = x5 unsigned>> 16
# asm 1: vpsrld $16,<x5=reg128#8,>v01=reg128#8
# asm 2: vpsrld $16,<x5=%xmm7,>v01=%xmm7
vpsrld $16, % xmm7, % xmm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <mask3=reg128#4,<x7=reg128#2,>v11=reg128#2
# asm 2: vpand <mask3=%xmm3,<x7=%xmm1,>v11=%xmm1
vpand % xmm3, % xmm1, % xmm1

# qhasm: x5 = v00 | v10
# asm 1: vpor <v10=reg128#9,<v00=reg128#3,>x5=reg128#3
# asm 2: vpor <v10=%xmm8,<v00=%xmm2,>x5=%xmm2
vpor % xmm8, % xmm2, % xmm2

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#2,<v01=reg128#8,>x7=reg128#2
# asm 2: vpor <v11=%xmm1,<v01=%xmm7,>x7=%xmm1
vpor % xmm1, % xmm7, % xmm1

# qhasm: v00 = x0 & mask4
# asm 1: vpand <mask4=reg128#5,<x0=reg128#10,>v00=reg128#4
# asm 2: vpand <mask4=%xmm4,<x0=%xmm9,>v00=%xmm3
vpand % xmm4, % xmm9, % xmm3

# qhasm: 8x v10 = x1 << 8
# asm 1: vpsllw $8,<x1=reg128#13,>v10=reg128#8
# asm 2: vpsllw $8,<x1=%xmm12,>v10=%xmm7
vpsllw $8, % xmm12, % xmm7

# qhasm: 8x v01 = x0 unsigned>> 8
# asm 1: vpsrlw $8,<x0=reg128#10,>v01=reg128#9
# asm 2: vpsrlw $8,<x0=%xmm9,>v01=%xmm8
vpsrlw $8, % xmm9, % xmm8

# qhasm: v11 = x1 & mask5
# asm 1: vpand <mask5=reg128#6,<x1=reg128#13,>v11=reg128#10
# asm 2: vpand <mask5=%xmm5,<x1=%xmm12,>v11=%xmm9
vpand % xmm5, % xmm12, % xmm9

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#8,<v00=reg128#4,>x0=reg128#4
# asm 2: vpor <v10=%xmm7,<v00=%xmm3,>x0=%xmm3
vpor % xmm7, % xmm3, % xmm3

# qhasm: x1 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#9,>x1=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm8,>x1=%xmm7
vpor % xmm9, % xmm8, % xmm7

# qhasm: v00 = x2 & mask4
# asm 1: vpand <mask4=reg128#5,<x2=reg128#12,>v00=reg128#9
# asm 2: vpand <mask4=%xmm4,<x2=%xmm11,>v00=%xmm8
vpand % xmm4, % xmm11, % xmm8

# qhasm: 8x v10 = x3 << 8
# asm 1: vpsllw $8,<x3=reg128#1,>v10=reg128#10
# asm 2: vpsllw $8,<x3=%xmm0,>v10=%xmm9
vpsllw $8, % xmm0, % xmm9

# qhasm: 8x v01 = x2 unsigned>> 8
# asm 1: vpsrlw $8,<x2=reg128#12,>v01=reg128#12
# asm 2: vpsrlw $8,<x2=%xmm11,>v01=%xmm11
vpsrlw $8, % xmm11, % xmm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <mask5=reg128#6,<x3=reg128#1,>v11=reg128#1
# asm 2: vpand <mask5=%xmm5,<x3=%xmm0,>v11=%xmm0
vpand % xmm5, % xmm0, % xmm0

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#10,<v00=reg128#9,>x2=reg128#9
# asm 2: vpor <v10=%xmm9,<v00=%xmm8,>x2=%xmm8
vpor % xmm9, % xmm8, % xmm8

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#1,<v01=reg128#12,>x3=reg128#1
# asm 2: vpor <v11=%xmm0,<v01=%xmm11,>x3=%xmm0
vpor % xmm0, % xmm11, % xmm0

# qhasm: v00 = x4 & mask4
# asm 1: vpand <mask4=reg128#5,<x4=reg128#11,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<x4=%xmm10,>v00=%xmm9
vpand % xmm4, % xmm10, % xmm9

# qhasm: 8x v10 = x5 << 8
# asm 1: vpsllw $8,<x5=reg128#3,>v10=reg128#12
# asm 2: vpsllw $8,<x5=%xmm2,>v10=%xmm11
vpsllw $8, % xmm2, % xmm11

# qhasm: 8x v01 = x4 unsigned>> 8
# asm 1: vpsrlw $8,<x4=reg128#11,>v01=reg128#11
# asm 2: vpsrlw $8,<x4=%xmm10,>v01=%xmm10
vpsrlw $8, % xmm10, % xmm10

# qhasm: v11 = x5 & mask5
# asm 1: vpand <mask5=reg128#6,<x5=reg128#3,>v11=reg128#3
# asm 2: vpand <mask5=%xmm5,<x5=%xmm2,>v11=%xmm2
vpand % xmm5, % xmm2, % xmm2

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#12,<v00=reg128#10,>x4=reg128#10
# asm 2: vpor <v10=%xmm11,<v00=%xmm9,>x4=%xmm9
vpor % xmm11, % xmm9, % xmm9

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#3,<v01=reg128#11,>x5=reg128#3
# asm 2: vpor <v11=%xmm2,<v01=%xmm10,>x5=%xmm2
vpor % xmm2, % xmm10, % xmm2

# qhasm: v00 = x6 & mask4
# asm 1: vpand <mask4=reg128#5,<x6=reg128#7,>v00=reg128#5
# asm 2: vpand <mask4=%xmm4,<x6=%xmm6,>v00=%xmm4
vpand % xmm4, % xmm6, % xmm4

# qhasm: 8x v10 = x7 << 8
# asm 1: vpsllw $8,<x7=reg128#2,>v10=reg128#11
# asm 2: vpsllw $8,<x7=%xmm1,>v10=%xmm10
vpsllw $8, % xmm1, % xmm10

# qhasm: 8x v01 = x6 unsigned>> 8
# asm 1: vpsrlw $8,<x6=reg128#7,>v01=reg128#7
# asm 2: vpsrlw $8,<x6=%xmm6,>v01=%xmm6
vpsrlw $8, % xmm6, % xmm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <mask5=reg128#6,<x7=reg128#2,>v11=reg128#2
# asm 2: vpand <mask5=%xmm5,<x7=%xmm1,>v11=%xmm1
vpand % xmm5, % xmm1, % xmm1

# qhasm: x6 = v00 | v10
# asm 1: vpor <v10=reg128#11,<v00=reg128#5,>x6=reg128#5
# asm 2: vpor <v10=%xmm10,<v00=%xmm4,>x6=%xmm4
vpor % xmm10, % xmm4, % xmm4

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#2,<v01=reg128#7,>x7=reg128#2
# asm 2: vpor <v11=%xmm1,<v01=%xmm6,>x7=%xmm1
vpor % xmm1, % xmm6, % xmm1

# qhasm: mem128[ input_0 + 112 ] = x0
# asm 1: movdqu   <x0=reg128#4,112(<input_0=int64#1)
# asm 2: movdqu   <x0=%xmm3,112(<input_0=%rdi)
movdqu   % xmm3, 112( % rdi)

# qhasm: mem128[ input_0 + 240 ] = x1
# asm 1: movdqu   <x1=reg128#8,240(<input_0=int64#1)
# asm 2: movdqu   <x1=%xmm7,240(<input_0=%rdi)
movdqu   % xmm7, 240( % rdi)

# qhasm: mem128[ input_0 + 368 ] = x2
# asm 1: movdqu   <x2=reg128#9,368(<input_0=int64#1)
# asm 2: movdqu   <x2=%xmm8,368(<input_0=%rdi)
movdqu   % xmm8, 368( % rdi)

# qhasm: mem128[ input_0 + 496 ] = x3
# asm 1: movdqu   <x3=reg128#1,496(<input_0=int64#1)
# asm 2: movdqu   <x3=%xmm0,496(<input_0=%rdi)
movdqu   % xmm0, 496( % rdi)

# qhasm: mem128[ input_0 + 624 ] = x4
# asm 1: movdqu   <x4=reg128#10,624(<input_0=int64#1)
# asm 2: movdqu   <x4=%xmm9,624(<input_0=%rdi)
movdqu   % xmm9, 624( % rdi)

# qhasm: mem128[ input_0 + 752 ] = x5
# asm 1: movdqu   <x5=reg128#3,752(<input_0=int64#1)
# asm 2: movdqu   <x5=%xmm2,752(<input_0=%rdi)
movdqu   % xmm2, 752( % rdi)

# qhasm: mem128[ input_0 + 880 ] = x6
# asm 1: movdqu   <x6=reg128#5,880(<input_0=int64#1)
# asm 2: movdqu   <x6=%xmm4,880(<input_0=%rdi)
movdqu   % xmm4, 880( % rdi)

# qhasm: mem128[ input_0 + 1008 ] = x7
# asm 1: movdqu   <x7=reg128#2,1008(<input_0=int64#1)
# asm 2: movdqu   <x7=%xmm1,1008(<input_0=%rdi)
movdqu   % xmm1, 1008( % rdi)

# qhasm: mask0 aligned= mem128[ MASK2_0 ]
# asm 1: movdqa   MASK2_0(%rip),>mask0=reg128#1
# asm 2: movdqa   MASK2_0(%rip),>mask0=%xmm0
movdqa   MASK2_0( % rip), % xmm0

# qhasm: mask1 aligned= mem128[ MASK2_1 ]
# asm 1: movdqa   MASK2_1(%rip),>mask1=reg128#2
# asm 2: movdqa   MASK2_1(%rip),>mask1=%xmm1
movdqa   MASK2_1( % rip), % xmm1

# qhasm: mask2 aligned= mem128[ MASK1_0 ]
# asm 1: movdqa   MASK1_0(%rip),>mask2=reg128#3
# asm 2: movdqa   MASK1_0(%rip),>mask2=%xmm2
movdqa   MASK1_0( % rip), % xmm2

# qhasm: mask3 aligned= mem128[ MASK1_1 ]
# asm 1: movdqa   MASK1_1(%rip),>mask3=reg128#4
# asm 2: movdqa   MASK1_1(%rip),>mask3=%xmm3
movdqa   MASK1_1( % rip), % xmm3

# qhasm: mask4 aligned= mem128[ MASK0_0 ]
# asm 1: movdqa   MASK0_0(%rip),>mask4=reg128#5
# asm 2: movdqa   MASK0_0(%rip),>mask4=%xmm4
movdqa   MASK0_0( % rip), % xmm4

# qhasm: mask5 aligned= mem128[ MASK0_1 ]
# asm 1: movdqa   MASK0_1(%rip),>mask5=reg128#6
# asm 2: movdqa   MASK0_1(%rip),>mask5=%xmm5
movdqa   MASK0_1( % rip), % xmm5

# qhasm: x0 = mem128[ input_0 + 0 ]
# asm 1: movdqu   0(<input_0=int64#1),>x0=reg128#7
# asm 2: movdqu   0(<input_0=%rdi),>x0=%xmm6
movdqu   0( % rdi), % xmm6

# qhasm: x1 = mem128[ input_0 + 16 ]
# asm 1: movdqu   16(<input_0=int64#1),>x1=reg128#8
# asm 2: movdqu   16(<input_0=%rdi),>x1=%xmm7
movdqu   16( % rdi), % xmm7

# qhasm: x2 = mem128[ input_0 + 32 ]
# asm 1: movdqu   32(<input_0=int64#1),>x2=reg128#9
# asm 2: movdqu   32(<input_0=%rdi),>x2=%xmm8
movdqu   32( % rdi), % xmm8

# qhasm: x3 = mem128[ input_0 + 48 ]
# asm 1: movdqu   48(<input_0=int64#1),>x3=reg128#10
# asm 2: movdqu   48(<input_0=%rdi),>x3=%xmm9
movdqu   48( % rdi), % xmm9

# qhasm: x4 = mem128[ input_0 + 64 ]
# asm 1: movdqu   64(<input_0=int64#1),>x4=reg128#11
# asm 2: movdqu   64(<input_0=%rdi),>x4=%xmm10
movdqu   64( % rdi), % xmm10

# qhasm: x5 = mem128[ input_0 + 80 ]
# asm 1: movdqu   80(<input_0=int64#1),>x5=reg128#12
# asm 2: movdqu   80(<input_0=%rdi),>x5=%xmm11
movdqu   80( % rdi), % xmm11

# qhasm: x6 = mem128[ input_0 + 96 ]
# asm 1: movdqu   96(<input_0=int64#1),>x6=reg128#13
# asm 2: movdqu   96(<input_0=%rdi),>x6=%xmm12
movdqu   96( % rdi), % xmm12

# qhasm: x7 = mem128[ input_0 + 112 ]
# asm 1: movdqu   112(<input_0=int64#1),>x7=reg128#14
# asm 2: movdqu   112(<input_0=%rdi),>x7=%xmm13
movdqu   112( % rdi), % xmm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <mask0=reg128#1,<x0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<x0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: v10 = x4 & mask0
# asm 1: vpand <mask0=reg128#1,<x4=reg128#11,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x4=%xmm10,>v10=%xmm15
vpand % xmm0, % xmm10, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x0 & mask1
# asm 1: vpand <mask1=reg128#2,<x0=reg128#7,>v01=reg128#7
# asm 2: vpand <mask1=%xmm1,<x0=%xmm6,>v01=%xmm6
vpand % xmm1, % xmm6, % xmm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <mask1=reg128#2,<x4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<x4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#7
# asm 2: psrlq $4,<v01=%xmm6
psrlq $4, % xmm6

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>x4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>x4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <mask0=reg128#1,<x1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<x1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: v10 = x5 & mask0
# asm 1: vpand <mask0=reg128#1,<x5=reg128#12,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x5=%xmm11,>v10=%xmm15
vpand % xmm0, % xmm11, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x1 & mask1
# asm 1: vpand <mask1=reg128#2,<x1=reg128#8,>v01=reg128#8
# asm 2: vpand <mask1=%xmm1,<x1=%xmm7,>v01=%xmm7
vpand % xmm1, % xmm7, % xmm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <mask1=reg128#2,<x5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<x5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#8
# asm 2: psrlq $4,<v01=%xmm7
psrlq $4, % xmm7

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>x1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>x1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>x5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>x5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <mask0=reg128#1,<x2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<x2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: v10 = x6 & mask0
# asm 1: vpand <mask0=reg128#1,<x6=reg128#13,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x6=%xmm12,>v10=%xmm15
vpand % xmm0, % xmm12, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x2 & mask1
# asm 1: vpand <mask1=reg128#2,<x2=reg128#9,>v01=reg128#9
# asm 2: vpand <mask1=%xmm1,<x2=%xmm8,>v01=%xmm8
vpand % xmm1, % xmm8, % xmm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <mask1=reg128#2,<x6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<x6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#9
# asm 2: psrlq $4,<v01=%xmm8
psrlq $4, % xmm8

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>x6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>x6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <mask0=reg128#1,<x3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<x3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: v10 = x7 & mask0
# asm 1: vpand <mask0=reg128#1,<x7=reg128#14,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x7=%xmm13,>v10=%xmm15
vpand % xmm0, % xmm13, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x3 & mask1
# asm 1: vpand <mask1=reg128#2,<x3=reg128#10,>v01=reg128#10
# asm 2: vpand <mask1=%xmm1,<x3=%xmm9,>v01=%xmm9
vpand % xmm1, % xmm9, % xmm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <mask1=reg128#2,<x7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<x7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#10
# asm 2: psrlq $4,<v01=%xmm9
psrlq $4, % xmm9

# qhasm: x3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>x7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>x7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <mask2=reg128#3,<x0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<x0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: v10 = x2 & mask2
# asm 1: vpand <mask2=reg128#3,<x2=reg128#12,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x2=%xmm11,>v10=%xmm15
vpand % xmm2, % xmm11, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x0 & mask3
# asm 1: vpand <mask3=reg128#4,<x0=reg128#15,>v01=reg128#15
# asm 2: vpand <mask3=%xmm3,<x0=%xmm14,>v01=%xmm14
vpand % xmm3, % xmm14, % xmm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <mask3=reg128#4,<x2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<x2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#15
# asm 2: psrlq $2,<v01=%xmm14
psrlq $2, % xmm14

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>x0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>x0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: x2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>x2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>x2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <mask2=reg128#3,<x1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<x1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: v10 = x3 & mask2
# asm 1: vpand <mask2=reg128#3,<x3=reg128#13,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x3=%xmm12,>v10=%xmm15
vpand % xmm2, % xmm12, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x1 & mask3
# asm 1: vpand <mask3=reg128#4,<x1=reg128#11,>v01=reg128#11
# asm 2: vpand <mask3=%xmm3,<x1=%xmm10,>v01=%xmm10
vpand % xmm3, % xmm10, % xmm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <mask3=reg128#4,<x3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<x3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#11
# asm 2: psrlq $2,<v01=%xmm10
psrlq $2, % xmm10

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>x3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>x3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <mask2=reg128#3,<x4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<x4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: v10 = x6 & mask2
# asm 1: vpand <mask2=reg128#3,<x6=reg128#9,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x6=%xmm8,>v10=%xmm15
vpand % xmm2, % xmm8, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x4 & mask3
# asm 1: vpand <mask3=reg128#4,<x4=reg128#7,>v01=reg128#7
# asm 2: vpand <mask3=%xmm3,<x4=%xmm6,>v01=%xmm6
vpand % xmm3, % xmm6, % xmm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <mask3=reg128#4,<x6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<x6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#7
# asm 2: psrlq $2,<v01=%xmm6
psrlq $2, % xmm6

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>x6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>x6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <mask2=reg128#3,<x5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<x5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: v10 = x7 & mask2
# asm 1: vpand <mask2=reg128#3,<x7=reg128#10,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x7=%xmm9,>v10=%xmm15
vpand % xmm2, % xmm9, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x5 & mask3
# asm 1: vpand <mask3=reg128#4,<x5=reg128#8,>v01=reg128#8
# asm 2: vpand <mask3=%xmm3,<x5=%xmm7,>v01=%xmm7
vpand % xmm3, % xmm7, % xmm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <mask3=reg128#4,<x7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<x7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#8
# asm 2: psrlq $2,<v01=%xmm7
psrlq $2, % xmm7

# qhasm: x5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>x5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>x5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>x7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>x7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <mask4=reg128#5,<x0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<x0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: v10 = x1 & mask4
# asm 1: vpand <mask4=reg128#5,<x1=reg128#15,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x1=%xmm14,>v10=%xmm15
vpand % xmm4, % xmm14, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x0 & mask5
# asm 1: vpand <mask5=reg128#6,<x0=reg128#14,>v01=reg128#14
# asm 2: vpand <mask5=%xmm5,<x0=%xmm13,>v01=%xmm13
vpand % xmm5, % xmm13, % xmm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <mask5=reg128#6,<x1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<x1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#14
# asm 2: psrlq $1,<v01=%xmm13
psrlq $1, % xmm13

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>x0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>x0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: x1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>x1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>x1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <mask4=reg128#5,<x2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<x2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: v10 = x3 & mask4
# asm 1: vpand <mask4=reg128#5,<x3=reg128#11,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x3=%xmm10,>v10=%xmm15
vpand % xmm4, % xmm10, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x2 & mask5
# asm 1: vpand <mask5=reg128#6,<x2=reg128#12,>v01=reg128#12
# asm 2: vpand <mask5=%xmm5,<x2=%xmm11,>v01=%xmm11
vpand % xmm5, % xmm11, % xmm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <mask5=reg128#6,<x3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<x3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#12
# asm 2: psrlq $1,<v01=%xmm11
psrlq $1, % xmm11

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>x3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>x3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <mask4=reg128#5,<x4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<x4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: v10 = x5 & mask4
# asm 1: vpand <mask4=reg128#5,<x5=reg128#9,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x5=%xmm8,>v10=%xmm15
vpand % xmm4, % xmm8, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x4 & mask5
# asm 1: vpand <mask5=reg128#6,<x4=reg128#13,>v01=reg128#13
# asm 2: vpand <mask5=%xmm5,<x4=%xmm12,>v01=%xmm12
vpand % xmm5, % xmm12, % xmm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <mask5=reg128#6,<x5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<x5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#13
# asm 2: psrlq $1,<v01=%xmm12
psrlq $1, % xmm12

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>x5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>x5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <mask4=reg128#5,<x6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<x6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: v10 = x7 & mask4
# asm 1: vpand <mask4=reg128#5,<x7=reg128#8,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x7=%xmm7,>v10=%xmm15
vpand % xmm4, % xmm7, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x6 & mask5
# asm 1: vpand <mask5=reg128#6,<x6=reg128#7,>v01=reg128#7
# asm 2: vpand <mask5=%xmm5,<x6=%xmm6,>v01=%xmm6
vpand % xmm5, % xmm6, % xmm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <mask5=reg128#6,<x7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<x7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#7
# asm 2: psrlq $1,<v01=%xmm6
psrlq $1, % xmm6

# qhasm: x6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>x7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>x7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: mem128[ input_0 + 0 ] = x0
# asm 1: movdqu   <x0=reg128#10,0(<input_0=int64#1)
# asm 2: movdqu   <x0=%xmm9,0(<input_0=%rdi)
movdqu   % xmm9, 0( % rdi)

# qhasm: mem128[ input_0 + 16 ] = x1
# asm 1: movdqu   <x1=reg128#14,16(<input_0=int64#1)
# asm 2: movdqu   <x1=%xmm13,16(<input_0=%rdi)
movdqu   % xmm13, 16( % rdi)

# qhasm: mem128[ input_0 + 32 ] = x2
# asm 1: movdqu   <x2=reg128#15,32(<input_0=int64#1)
# asm 2: movdqu   <x2=%xmm14,32(<input_0=%rdi)
movdqu   % xmm14, 32( % rdi)

# qhasm: mem128[ input_0 + 48 ] = x3
# asm 1: movdqu   <x3=reg128#11,48(<input_0=int64#1)
# asm 2: movdqu   <x3=%xmm10,48(<input_0=%rdi)
movdqu   % xmm10, 48( % rdi)

# qhasm: mem128[ input_0 + 64 ] = x4
# asm 1: movdqu   <x4=reg128#12,64(<input_0=int64#1)
# asm 2: movdqu   <x4=%xmm11,64(<input_0=%rdi)
movdqu   % xmm11, 64( % rdi)

# qhasm: mem128[ input_0 + 80 ] = x5
# asm 1: movdqu   <x5=reg128#9,80(<input_0=int64#1)
# asm 2: movdqu   <x5=%xmm8,80(<input_0=%rdi)
movdqu   % xmm8, 80( % rdi)

# qhasm: mem128[ input_0 + 96 ] = x6
# asm 1: movdqu   <x6=reg128#13,96(<input_0=int64#1)
# asm 2: movdqu   <x6=%xmm12,96(<input_0=%rdi)
movdqu   % xmm12, 96( % rdi)

# qhasm: mem128[ input_0 + 112 ] = x7
# asm 1: movdqu   <x7=reg128#7,112(<input_0=int64#1)
# asm 2: movdqu   <x7=%xmm6,112(<input_0=%rdi)
movdqu   % xmm6, 112( % rdi)

# qhasm: x0 = mem128[ input_0 + 128 ]
# asm 1: movdqu   128(<input_0=int64#1),>x0=reg128#7
# asm 2: movdqu   128(<input_0=%rdi),>x0=%xmm6
movdqu   128( % rdi), % xmm6

# qhasm: x1 = mem128[ input_0 + 144 ]
# asm 1: movdqu   144(<input_0=int64#1),>x1=reg128#8
# asm 2: movdqu   144(<input_0=%rdi),>x1=%xmm7
movdqu   144( % rdi), % xmm7

# qhasm: x2 = mem128[ input_0 + 160 ]
# asm 1: movdqu   160(<input_0=int64#1),>x2=reg128#9
# asm 2: movdqu   160(<input_0=%rdi),>x2=%xmm8
movdqu   160( % rdi), % xmm8

# qhasm: x3 = mem128[ input_0 + 176 ]
# asm 1: movdqu   176(<input_0=int64#1),>x3=reg128#10
# asm 2: movdqu   176(<input_0=%rdi),>x3=%xmm9
movdqu   176( % rdi), % xmm9

# qhasm: x4 = mem128[ input_0 + 192 ]
# asm 1: movdqu   192(<input_0=int64#1),>x4=reg128#11
# asm 2: movdqu   192(<input_0=%rdi),>x4=%xmm10
movdqu   192( % rdi), % xmm10

# qhasm: x5 = mem128[ input_0 + 208 ]
# asm 1: movdqu   208(<input_0=int64#1),>x5=reg128#12
# asm 2: movdqu   208(<input_0=%rdi),>x5=%xmm11
movdqu   208( % rdi), % xmm11

# qhasm: x6 = mem128[ input_0 + 224 ]
# asm 1: movdqu   224(<input_0=int64#1),>x6=reg128#13
# asm 2: movdqu   224(<input_0=%rdi),>x6=%xmm12
movdqu   224( % rdi), % xmm12

# qhasm: x7 = mem128[ input_0 + 240 ]
# asm 1: movdqu   240(<input_0=int64#1),>x7=reg128#14
# asm 2: movdqu   240(<input_0=%rdi),>x7=%xmm13
movdqu   240( % rdi), % xmm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <mask0=reg128#1,<x0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<x0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: v10 = x4 & mask0
# asm 1: vpand <mask0=reg128#1,<x4=reg128#11,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x4=%xmm10,>v10=%xmm15
vpand % xmm0, % xmm10, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x0 & mask1
# asm 1: vpand <mask1=reg128#2,<x0=reg128#7,>v01=reg128#7
# asm 2: vpand <mask1=%xmm1,<x0=%xmm6,>v01=%xmm6
vpand % xmm1, % xmm6, % xmm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <mask1=reg128#2,<x4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<x4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#7
# asm 2: psrlq $4,<v01=%xmm6
psrlq $4, % xmm6

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>x4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>x4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <mask0=reg128#1,<x1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<x1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: v10 = x5 & mask0
# asm 1: vpand <mask0=reg128#1,<x5=reg128#12,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x5=%xmm11,>v10=%xmm15
vpand % xmm0, % xmm11, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x1 & mask1
# asm 1: vpand <mask1=reg128#2,<x1=reg128#8,>v01=reg128#8
# asm 2: vpand <mask1=%xmm1,<x1=%xmm7,>v01=%xmm7
vpand % xmm1, % xmm7, % xmm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <mask1=reg128#2,<x5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<x5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#8
# asm 2: psrlq $4,<v01=%xmm7
psrlq $4, % xmm7

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>x1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>x1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>x5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>x5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <mask0=reg128#1,<x2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<x2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: v10 = x6 & mask0
# asm 1: vpand <mask0=reg128#1,<x6=reg128#13,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x6=%xmm12,>v10=%xmm15
vpand % xmm0, % xmm12, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x2 & mask1
# asm 1: vpand <mask1=reg128#2,<x2=reg128#9,>v01=reg128#9
# asm 2: vpand <mask1=%xmm1,<x2=%xmm8,>v01=%xmm8
vpand % xmm1, % xmm8, % xmm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <mask1=reg128#2,<x6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<x6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#9
# asm 2: psrlq $4,<v01=%xmm8
psrlq $4, % xmm8

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>x6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>x6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <mask0=reg128#1,<x3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<x3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: v10 = x7 & mask0
# asm 1: vpand <mask0=reg128#1,<x7=reg128#14,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x7=%xmm13,>v10=%xmm15
vpand % xmm0, % xmm13, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x3 & mask1
# asm 1: vpand <mask1=reg128#2,<x3=reg128#10,>v01=reg128#10
# asm 2: vpand <mask1=%xmm1,<x3=%xmm9,>v01=%xmm9
vpand % xmm1, % xmm9, % xmm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <mask1=reg128#2,<x7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<x7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#10
# asm 2: psrlq $4,<v01=%xmm9
psrlq $4, % xmm9

# qhasm: x3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>x7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>x7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <mask2=reg128#3,<x0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<x0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: v10 = x2 & mask2
# asm 1: vpand <mask2=reg128#3,<x2=reg128#12,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x2=%xmm11,>v10=%xmm15
vpand % xmm2, % xmm11, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x0 & mask3
# asm 1: vpand <mask3=reg128#4,<x0=reg128#15,>v01=reg128#15
# asm 2: vpand <mask3=%xmm3,<x0=%xmm14,>v01=%xmm14
vpand % xmm3, % xmm14, % xmm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <mask3=reg128#4,<x2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<x2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#15
# asm 2: psrlq $2,<v01=%xmm14
psrlq $2, % xmm14

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>x0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>x0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: x2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>x2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>x2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <mask2=reg128#3,<x1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<x1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: v10 = x3 & mask2
# asm 1: vpand <mask2=reg128#3,<x3=reg128#13,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x3=%xmm12,>v10=%xmm15
vpand % xmm2, % xmm12, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x1 & mask3
# asm 1: vpand <mask3=reg128#4,<x1=reg128#11,>v01=reg128#11
# asm 2: vpand <mask3=%xmm3,<x1=%xmm10,>v01=%xmm10
vpand % xmm3, % xmm10, % xmm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <mask3=reg128#4,<x3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<x3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#11
# asm 2: psrlq $2,<v01=%xmm10
psrlq $2, % xmm10

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>x3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>x3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <mask2=reg128#3,<x4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<x4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: v10 = x6 & mask2
# asm 1: vpand <mask2=reg128#3,<x6=reg128#9,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x6=%xmm8,>v10=%xmm15
vpand % xmm2, % xmm8, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x4 & mask3
# asm 1: vpand <mask3=reg128#4,<x4=reg128#7,>v01=reg128#7
# asm 2: vpand <mask3=%xmm3,<x4=%xmm6,>v01=%xmm6
vpand % xmm3, % xmm6, % xmm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <mask3=reg128#4,<x6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<x6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#7
# asm 2: psrlq $2,<v01=%xmm6
psrlq $2, % xmm6

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>x6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>x6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <mask2=reg128#3,<x5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<x5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: v10 = x7 & mask2
# asm 1: vpand <mask2=reg128#3,<x7=reg128#10,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x7=%xmm9,>v10=%xmm15
vpand % xmm2, % xmm9, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x5 & mask3
# asm 1: vpand <mask3=reg128#4,<x5=reg128#8,>v01=reg128#8
# asm 2: vpand <mask3=%xmm3,<x5=%xmm7,>v01=%xmm7
vpand % xmm3, % xmm7, % xmm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <mask3=reg128#4,<x7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<x7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#8
# asm 2: psrlq $2,<v01=%xmm7
psrlq $2, % xmm7

# qhasm: x5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>x5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>x5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>x7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>x7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <mask4=reg128#5,<x0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<x0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: v10 = x1 & mask4
# asm 1: vpand <mask4=reg128#5,<x1=reg128#15,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x1=%xmm14,>v10=%xmm15
vpand % xmm4, % xmm14, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x0 & mask5
# asm 1: vpand <mask5=reg128#6,<x0=reg128#14,>v01=reg128#14
# asm 2: vpand <mask5=%xmm5,<x0=%xmm13,>v01=%xmm13
vpand % xmm5, % xmm13, % xmm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <mask5=reg128#6,<x1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<x1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#14
# asm 2: psrlq $1,<v01=%xmm13
psrlq $1, % xmm13

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>x0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>x0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: x1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>x1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>x1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <mask4=reg128#5,<x2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<x2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: v10 = x3 & mask4
# asm 1: vpand <mask4=reg128#5,<x3=reg128#11,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x3=%xmm10,>v10=%xmm15
vpand % xmm4, % xmm10, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x2 & mask5
# asm 1: vpand <mask5=reg128#6,<x2=reg128#12,>v01=reg128#12
# asm 2: vpand <mask5=%xmm5,<x2=%xmm11,>v01=%xmm11
vpand % xmm5, % xmm11, % xmm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <mask5=reg128#6,<x3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<x3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#12
# asm 2: psrlq $1,<v01=%xmm11
psrlq $1, % xmm11

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>x3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>x3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <mask4=reg128#5,<x4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<x4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: v10 = x5 & mask4
# asm 1: vpand <mask4=reg128#5,<x5=reg128#9,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x5=%xmm8,>v10=%xmm15
vpand % xmm4, % xmm8, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x4 & mask5
# asm 1: vpand <mask5=reg128#6,<x4=reg128#13,>v01=reg128#13
# asm 2: vpand <mask5=%xmm5,<x4=%xmm12,>v01=%xmm12
vpand % xmm5, % xmm12, % xmm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <mask5=reg128#6,<x5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<x5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#13
# asm 2: psrlq $1,<v01=%xmm12
psrlq $1, % xmm12

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>x5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>x5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <mask4=reg128#5,<x6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<x6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: v10 = x7 & mask4
# asm 1: vpand <mask4=reg128#5,<x7=reg128#8,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x7=%xmm7,>v10=%xmm15
vpand % xmm4, % xmm7, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x6 & mask5
# asm 1: vpand <mask5=reg128#6,<x6=reg128#7,>v01=reg128#7
# asm 2: vpand <mask5=%xmm5,<x6=%xmm6,>v01=%xmm6
vpand % xmm5, % xmm6, % xmm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <mask5=reg128#6,<x7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<x7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#7
# asm 2: psrlq $1,<v01=%xmm6
psrlq $1, % xmm6

# qhasm: x6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>x7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>x7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: mem128[ input_0 + 128 ] = x0
# asm 1: movdqu   <x0=reg128#10,128(<input_0=int64#1)
# asm 2: movdqu   <x0=%xmm9,128(<input_0=%rdi)
movdqu   % xmm9, 128( % rdi)

# qhasm: mem128[ input_0 + 144 ] = x1
# asm 1: movdqu   <x1=reg128#14,144(<input_0=int64#1)
# asm 2: movdqu   <x1=%xmm13,144(<input_0=%rdi)
movdqu   % xmm13, 144( % rdi)

# qhasm: mem128[ input_0 + 160 ] = x2
# asm 1: movdqu   <x2=reg128#15,160(<input_0=int64#1)
# asm 2: movdqu   <x2=%xmm14,160(<input_0=%rdi)
movdqu   % xmm14, 160( % rdi)

# qhasm: mem128[ input_0 + 176 ] = x3
# asm 1: movdqu   <x3=reg128#11,176(<input_0=int64#1)
# asm 2: movdqu   <x3=%xmm10,176(<input_0=%rdi)
movdqu   % xmm10, 176( % rdi)

# qhasm: mem128[ input_0 + 192 ] = x4
# asm 1: movdqu   <x4=reg128#12,192(<input_0=int64#1)
# asm 2: movdqu   <x4=%xmm11,192(<input_0=%rdi)
movdqu   % xmm11, 192( % rdi)

# qhasm: mem128[ input_0 + 208 ] = x5
# asm 1: movdqu   <x5=reg128#9,208(<input_0=int64#1)
# asm 2: movdqu   <x5=%xmm8,208(<input_0=%rdi)
movdqu   % xmm8, 208( % rdi)

# qhasm: mem128[ input_0 + 224 ] = x6
# asm 1: movdqu   <x6=reg128#13,224(<input_0=int64#1)
# asm 2: movdqu   <x6=%xmm12,224(<input_0=%rdi)
movdqu   % xmm12, 224( % rdi)

# qhasm: mem128[ input_0 + 240 ] = x7
# asm 1: movdqu   <x7=reg128#7,240(<input_0=int64#1)
# asm 2: movdqu   <x7=%xmm6,240(<input_0=%rdi)
movdqu   % xmm6, 240( % rdi)

# qhasm: x0 = mem128[ input_0 + 256 ]
# asm 1: movdqu   256(<input_0=int64#1),>x0=reg128#7
# asm 2: movdqu   256(<input_0=%rdi),>x0=%xmm6
movdqu   256( % rdi), % xmm6

# qhasm: x1 = mem128[ input_0 + 272 ]
# asm 1: movdqu   272(<input_0=int64#1),>x1=reg128#8
# asm 2: movdqu   272(<input_0=%rdi),>x1=%xmm7
movdqu   272( % rdi), % xmm7

# qhasm: x2 = mem128[ input_0 + 288 ]
# asm 1: movdqu   288(<input_0=int64#1),>x2=reg128#9
# asm 2: movdqu   288(<input_0=%rdi),>x2=%xmm8
movdqu   288( % rdi), % xmm8

# qhasm: x3 = mem128[ input_0 + 304 ]
# asm 1: movdqu   304(<input_0=int64#1),>x3=reg128#10
# asm 2: movdqu   304(<input_0=%rdi),>x3=%xmm9
movdqu   304( % rdi), % xmm9

# qhasm: x4 = mem128[ input_0 + 320 ]
# asm 1: movdqu   320(<input_0=int64#1),>x4=reg128#11
# asm 2: movdqu   320(<input_0=%rdi),>x4=%xmm10
movdqu   320( % rdi), % xmm10

# qhasm: x5 = mem128[ input_0 + 336 ]
# asm 1: movdqu   336(<input_0=int64#1),>x5=reg128#12
# asm 2: movdqu   336(<input_0=%rdi),>x5=%xmm11
movdqu   336( % rdi), % xmm11

# qhasm: x6 = mem128[ input_0 + 352 ]
# asm 1: movdqu   352(<input_0=int64#1),>x6=reg128#13
# asm 2: movdqu   352(<input_0=%rdi),>x6=%xmm12
movdqu   352( % rdi), % xmm12

# qhasm: x7 = mem128[ input_0 + 368 ]
# asm 1: movdqu   368(<input_0=int64#1),>x7=reg128#14
# asm 2: movdqu   368(<input_0=%rdi),>x7=%xmm13
movdqu   368( % rdi), % xmm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <mask0=reg128#1,<x0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<x0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: v10 = x4 & mask0
# asm 1: vpand <mask0=reg128#1,<x4=reg128#11,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x4=%xmm10,>v10=%xmm15
vpand % xmm0, % xmm10, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x0 & mask1
# asm 1: vpand <mask1=reg128#2,<x0=reg128#7,>v01=reg128#7
# asm 2: vpand <mask1=%xmm1,<x0=%xmm6,>v01=%xmm6
vpand % xmm1, % xmm6, % xmm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <mask1=reg128#2,<x4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<x4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#7
# asm 2: psrlq $4,<v01=%xmm6
psrlq $4, % xmm6

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>x4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>x4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <mask0=reg128#1,<x1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<x1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: v10 = x5 & mask0
# asm 1: vpand <mask0=reg128#1,<x5=reg128#12,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x5=%xmm11,>v10=%xmm15
vpand % xmm0, % xmm11, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x1 & mask1
# asm 1: vpand <mask1=reg128#2,<x1=reg128#8,>v01=reg128#8
# asm 2: vpand <mask1=%xmm1,<x1=%xmm7,>v01=%xmm7
vpand % xmm1, % xmm7, % xmm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <mask1=reg128#2,<x5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<x5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#8
# asm 2: psrlq $4,<v01=%xmm7
psrlq $4, % xmm7

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>x1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>x1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>x5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>x5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <mask0=reg128#1,<x2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<x2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: v10 = x6 & mask0
# asm 1: vpand <mask0=reg128#1,<x6=reg128#13,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x6=%xmm12,>v10=%xmm15
vpand % xmm0, % xmm12, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x2 & mask1
# asm 1: vpand <mask1=reg128#2,<x2=reg128#9,>v01=reg128#9
# asm 2: vpand <mask1=%xmm1,<x2=%xmm8,>v01=%xmm8
vpand % xmm1, % xmm8, % xmm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <mask1=reg128#2,<x6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<x6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#9
# asm 2: psrlq $4,<v01=%xmm8
psrlq $4, % xmm8

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>x6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>x6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <mask0=reg128#1,<x3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<x3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: v10 = x7 & mask0
# asm 1: vpand <mask0=reg128#1,<x7=reg128#14,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x7=%xmm13,>v10=%xmm15
vpand % xmm0, % xmm13, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x3 & mask1
# asm 1: vpand <mask1=reg128#2,<x3=reg128#10,>v01=reg128#10
# asm 2: vpand <mask1=%xmm1,<x3=%xmm9,>v01=%xmm9
vpand % xmm1, % xmm9, % xmm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <mask1=reg128#2,<x7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<x7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#10
# asm 2: psrlq $4,<v01=%xmm9
psrlq $4, % xmm9

# qhasm: x3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>x7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>x7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <mask2=reg128#3,<x0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<x0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: v10 = x2 & mask2
# asm 1: vpand <mask2=reg128#3,<x2=reg128#12,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x2=%xmm11,>v10=%xmm15
vpand % xmm2, % xmm11, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x0 & mask3
# asm 1: vpand <mask3=reg128#4,<x0=reg128#15,>v01=reg128#15
# asm 2: vpand <mask3=%xmm3,<x0=%xmm14,>v01=%xmm14
vpand % xmm3, % xmm14, % xmm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <mask3=reg128#4,<x2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<x2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#15
# asm 2: psrlq $2,<v01=%xmm14
psrlq $2, % xmm14

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>x0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>x0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: x2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>x2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>x2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <mask2=reg128#3,<x1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<x1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: v10 = x3 & mask2
# asm 1: vpand <mask2=reg128#3,<x3=reg128#13,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x3=%xmm12,>v10=%xmm15
vpand % xmm2, % xmm12, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x1 & mask3
# asm 1: vpand <mask3=reg128#4,<x1=reg128#11,>v01=reg128#11
# asm 2: vpand <mask3=%xmm3,<x1=%xmm10,>v01=%xmm10
vpand % xmm3, % xmm10, % xmm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <mask3=reg128#4,<x3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<x3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#11
# asm 2: psrlq $2,<v01=%xmm10
psrlq $2, % xmm10

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>x3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>x3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <mask2=reg128#3,<x4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<x4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: v10 = x6 & mask2
# asm 1: vpand <mask2=reg128#3,<x6=reg128#9,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x6=%xmm8,>v10=%xmm15
vpand % xmm2, % xmm8, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x4 & mask3
# asm 1: vpand <mask3=reg128#4,<x4=reg128#7,>v01=reg128#7
# asm 2: vpand <mask3=%xmm3,<x4=%xmm6,>v01=%xmm6
vpand % xmm3, % xmm6, % xmm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <mask3=reg128#4,<x6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<x6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#7
# asm 2: psrlq $2,<v01=%xmm6
psrlq $2, % xmm6

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>x6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>x6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <mask2=reg128#3,<x5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<x5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: v10 = x7 & mask2
# asm 1: vpand <mask2=reg128#3,<x7=reg128#10,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x7=%xmm9,>v10=%xmm15
vpand % xmm2, % xmm9, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x5 & mask3
# asm 1: vpand <mask3=reg128#4,<x5=reg128#8,>v01=reg128#8
# asm 2: vpand <mask3=%xmm3,<x5=%xmm7,>v01=%xmm7
vpand % xmm3, % xmm7, % xmm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <mask3=reg128#4,<x7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<x7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#8
# asm 2: psrlq $2,<v01=%xmm7
psrlq $2, % xmm7

# qhasm: x5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>x5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>x5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>x7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>x7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <mask4=reg128#5,<x0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<x0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: v10 = x1 & mask4
# asm 1: vpand <mask4=reg128#5,<x1=reg128#15,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x1=%xmm14,>v10=%xmm15
vpand % xmm4, % xmm14, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x0 & mask5
# asm 1: vpand <mask5=reg128#6,<x0=reg128#14,>v01=reg128#14
# asm 2: vpand <mask5=%xmm5,<x0=%xmm13,>v01=%xmm13
vpand % xmm5, % xmm13, % xmm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <mask5=reg128#6,<x1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<x1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#14
# asm 2: psrlq $1,<v01=%xmm13
psrlq $1, % xmm13

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>x0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>x0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: x1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>x1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>x1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <mask4=reg128#5,<x2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<x2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: v10 = x3 & mask4
# asm 1: vpand <mask4=reg128#5,<x3=reg128#11,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x3=%xmm10,>v10=%xmm15
vpand % xmm4, % xmm10, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x2 & mask5
# asm 1: vpand <mask5=reg128#6,<x2=reg128#12,>v01=reg128#12
# asm 2: vpand <mask5=%xmm5,<x2=%xmm11,>v01=%xmm11
vpand % xmm5, % xmm11, % xmm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <mask5=reg128#6,<x3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<x3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#12
# asm 2: psrlq $1,<v01=%xmm11
psrlq $1, % xmm11

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>x3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>x3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <mask4=reg128#5,<x4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<x4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: v10 = x5 & mask4
# asm 1: vpand <mask4=reg128#5,<x5=reg128#9,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x5=%xmm8,>v10=%xmm15
vpand % xmm4, % xmm8, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x4 & mask5
# asm 1: vpand <mask5=reg128#6,<x4=reg128#13,>v01=reg128#13
# asm 2: vpand <mask5=%xmm5,<x4=%xmm12,>v01=%xmm12
vpand % xmm5, % xmm12, % xmm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <mask5=reg128#6,<x5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<x5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#13
# asm 2: psrlq $1,<v01=%xmm12
psrlq $1, % xmm12

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>x5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>x5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <mask4=reg128#5,<x6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<x6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: v10 = x7 & mask4
# asm 1: vpand <mask4=reg128#5,<x7=reg128#8,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x7=%xmm7,>v10=%xmm15
vpand % xmm4, % xmm7, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x6 & mask5
# asm 1: vpand <mask5=reg128#6,<x6=reg128#7,>v01=reg128#7
# asm 2: vpand <mask5=%xmm5,<x6=%xmm6,>v01=%xmm6
vpand % xmm5, % xmm6, % xmm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <mask5=reg128#6,<x7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<x7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#7
# asm 2: psrlq $1,<v01=%xmm6
psrlq $1, % xmm6

# qhasm: x6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>x7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>x7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: mem128[ input_0 + 256 ] = x0
# asm 1: movdqu   <x0=reg128#10,256(<input_0=int64#1)
# asm 2: movdqu   <x0=%xmm9,256(<input_0=%rdi)
movdqu   % xmm9, 256( % rdi)

# qhasm: mem128[ input_0 + 272 ] = x1
# asm 1: movdqu   <x1=reg128#14,272(<input_0=int64#1)
# asm 2: movdqu   <x1=%xmm13,272(<input_0=%rdi)
movdqu   % xmm13, 272( % rdi)

# qhasm: mem128[ input_0 + 288 ] = x2
# asm 1: movdqu   <x2=reg128#15,288(<input_0=int64#1)
# asm 2: movdqu   <x2=%xmm14,288(<input_0=%rdi)
movdqu   % xmm14, 288( % rdi)

# qhasm: mem128[ input_0 + 304 ] = x3
# asm 1: movdqu   <x3=reg128#11,304(<input_0=int64#1)
# asm 2: movdqu   <x3=%xmm10,304(<input_0=%rdi)
movdqu   % xmm10, 304( % rdi)

# qhasm: mem128[ input_0 + 320 ] = x4
# asm 1: movdqu   <x4=reg128#12,320(<input_0=int64#1)
# asm 2: movdqu   <x4=%xmm11,320(<input_0=%rdi)
movdqu   % xmm11, 320( % rdi)

# qhasm: mem128[ input_0 + 336 ] = x5
# asm 1: movdqu   <x5=reg128#9,336(<input_0=int64#1)
# asm 2: movdqu   <x5=%xmm8,336(<input_0=%rdi)
movdqu   % xmm8, 336( % rdi)

# qhasm: mem128[ input_0 + 352 ] = x6
# asm 1: movdqu   <x6=reg128#13,352(<input_0=int64#1)
# asm 2: movdqu   <x6=%xmm12,352(<input_0=%rdi)
movdqu   % xmm12, 352( % rdi)

# qhasm: mem128[ input_0 + 368 ] = x7
# asm 1: movdqu   <x7=reg128#7,368(<input_0=int64#1)
# asm 2: movdqu   <x7=%xmm6,368(<input_0=%rdi)
movdqu   % xmm6, 368( % rdi)

# qhasm: x0 = mem128[ input_0 + 384 ]
# asm 1: movdqu   384(<input_0=int64#1),>x0=reg128#7
# asm 2: movdqu   384(<input_0=%rdi),>x0=%xmm6
movdqu   384( % rdi), % xmm6

# qhasm: x1 = mem128[ input_0 + 400 ]
# asm 1: movdqu   400(<input_0=int64#1),>x1=reg128#8
# asm 2: movdqu   400(<input_0=%rdi),>x1=%xmm7
movdqu   400( % rdi), % xmm7

# qhasm: x2 = mem128[ input_0 + 416 ]
# asm 1: movdqu   416(<input_0=int64#1),>x2=reg128#9
# asm 2: movdqu   416(<input_0=%rdi),>x2=%xmm8
movdqu   416( % rdi), % xmm8

# qhasm: x3 = mem128[ input_0 + 432 ]
# asm 1: movdqu   432(<input_0=int64#1),>x3=reg128#10
# asm 2: movdqu   432(<input_0=%rdi),>x3=%xmm9
movdqu   432( % rdi), % xmm9

# qhasm: x4 = mem128[ input_0 + 448 ]
# asm 1: movdqu   448(<input_0=int64#1),>x4=reg128#11
# asm 2: movdqu   448(<input_0=%rdi),>x4=%xmm10
movdqu   448( % rdi), % xmm10

# qhasm: x5 = mem128[ input_0 + 464 ]
# asm 1: movdqu   464(<input_0=int64#1),>x5=reg128#12
# asm 2: movdqu   464(<input_0=%rdi),>x5=%xmm11
movdqu   464( % rdi), % xmm11

# qhasm: x6 = mem128[ input_0 + 480 ]
# asm 1: movdqu   480(<input_0=int64#1),>x6=reg128#13
# asm 2: movdqu   480(<input_0=%rdi),>x6=%xmm12
movdqu   480( % rdi), % xmm12

# qhasm: x7 = mem128[ input_0 + 496 ]
# asm 1: movdqu   496(<input_0=int64#1),>x7=reg128#14
# asm 2: movdqu   496(<input_0=%rdi),>x7=%xmm13
movdqu   496( % rdi), % xmm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <mask0=reg128#1,<x0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<x0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: v10 = x4 & mask0
# asm 1: vpand <mask0=reg128#1,<x4=reg128#11,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x4=%xmm10,>v10=%xmm15
vpand % xmm0, % xmm10, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x0 & mask1
# asm 1: vpand <mask1=reg128#2,<x0=reg128#7,>v01=reg128#7
# asm 2: vpand <mask1=%xmm1,<x0=%xmm6,>v01=%xmm6
vpand % xmm1, % xmm6, % xmm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <mask1=reg128#2,<x4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<x4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#7
# asm 2: psrlq $4,<v01=%xmm6
psrlq $4, % xmm6

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>x4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>x4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <mask0=reg128#1,<x1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<x1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: v10 = x5 & mask0
# asm 1: vpand <mask0=reg128#1,<x5=reg128#12,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x5=%xmm11,>v10=%xmm15
vpand % xmm0, % xmm11, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x1 & mask1
# asm 1: vpand <mask1=reg128#2,<x1=reg128#8,>v01=reg128#8
# asm 2: vpand <mask1=%xmm1,<x1=%xmm7,>v01=%xmm7
vpand % xmm1, % xmm7, % xmm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <mask1=reg128#2,<x5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<x5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#8
# asm 2: psrlq $4,<v01=%xmm7
psrlq $4, % xmm7

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>x1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>x1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>x5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>x5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <mask0=reg128#1,<x2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<x2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: v10 = x6 & mask0
# asm 1: vpand <mask0=reg128#1,<x6=reg128#13,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x6=%xmm12,>v10=%xmm15
vpand % xmm0, % xmm12, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x2 & mask1
# asm 1: vpand <mask1=reg128#2,<x2=reg128#9,>v01=reg128#9
# asm 2: vpand <mask1=%xmm1,<x2=%xmm8,>v01=%xmm8
vpand % xmm1, % xmm8, % xmm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <mask1=reg128#2,<x6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<x6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#9
# asm 2: psrlq $4,<v01=%xmm8
psrlq $4, % xmm8

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>x6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>x6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <mask0=reg128#1,<x3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<x3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: v10 = x7 & mask0
# asm 1: vpand <mask0=reg128#1,<x7=reg128#14,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x7=%xmm13,>v10=%xmm15
vpand % xmm0, % xmm13, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x3 & mask1
# asm 1: vpand <mask1=reg128#2,<x3=reg128#10,>v01=reg128#10
# asm 2: vpand <mask1=%xmm1,<x3=%xmm9,>v01=%xmm9
vpand % xmm1, % xmm9, % xmm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <mask1=reg128#2,<x7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<x7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#10
# asm 2: psrlq $4,<v01=%xmm9
psrlq $4, % xmm9

# qhasm: x3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>x7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>x7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <mask2=reg128#3,<x0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<x0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: v10 = x2 & mask2
# asm 1: vpand <mask2=reg128#3,<x2=reg128#12,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x2=%xmm11,>v10=%xmm15
vpand % xmm2, % xmm11, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x0 & mask3
# asm 1: vpand <mask3=reg128#4,<x0=reg128#15,>v01=reg128#15
# asm 2: vpand <mask3=%xmm3,<x0=%xmm14,>v01=%xmm14
vpand % xmm3, % xmm14, % xmm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <mask3=reg128#4,<x2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<x2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#15
# asm 2: psrlq $2,<v01=%xmm14
psrlq $2, % xmm14

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>x0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>x0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: x2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>x2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>x2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <mask2=reg128#3,<x1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<x1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: v10 = x3 & mask2
# asm 1: vpand <mask2=reg128#3,<x3=reg128#13,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x3=%xmm12,>v10=%xmm15
vpand % xmm2, % xmm12, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x1 & mask3
# asm 1: vpand <mask3=reg128#4,<x1=reg128#11,>v01=reg128#11
# asm 2: vpand <mask3=%xmm3,<x1=%xmm10,>v01=%xmm10
vpand % xmm3, % xmm10, % xmm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <mask3=reg128#4,<x3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<x3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#11
# asm 2: psrlq $2,<v01=%xmm10
psrlq $2, % xmm10

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>x3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>x3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <mask2=reg128#3,<x4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<x4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: v10 = x6 & mask2
# asm 1: vpand <mask2=reg128#3,<x6=reg128#9,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x6=%xmm8,>v10=%xmm15
vpand % xmm2, % xmm8, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x4 & mask3
# asm 1: vpand <mask3=reg128#4,<x4=reg128#7,>v01=reg128#7
# asm 2: vpand <mask3=%xmm3,<x4=%xmm6,>v01=%xmm6
vpand % xmm3, % xmm6, % xmm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <mask3=reg128#4,<x6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<x6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#7
# asm 2: psrlq $2,<v01=%xmm6
psrlq $2, % xmm6

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>x6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>x6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <mask2=reg128#3,<x5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<x5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: v10 = x7 & mask2
# asm 1: vpand <mask2=reg128#3,<x7=reg128#10,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x7=%xmm9,>v10=%xmm15
vpand % xmm2, % xmm9, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x5 & mask3
# asm 1: vpand <mask3=reg128#4,<x5=reg128#8,>v01=reg128#8
# asm 2: vpand <mask3=%xmm3,<x5=%xmm7,>v01=%xmm7
vpand % xmm3, % xmm7, % xmm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <mask3=reg128#4,<x7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<x7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#8
# asm 2: psrlq $2,<v01=%xmm7
psrlq $2, % xmm7

# qhasm: x5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>x5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>x5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>x7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>x7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <mask4=reg128#5,<x0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<x0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: v10 = x1 & mask4
# asm 1: vpand <mask4=reg128#5,<x1=reg128#15,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x1=%xmm14,>v10=%xmm15
vpand % xmm4, % xmm14, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x0 & mask5
# asm 1: vpand <mask5=reg128#6,<x0=reg128#14,>v01=reg128#14
# asm 2: vpand <mask5=%xmm5,<x0=%xmm13,>v01=%xmm13
vpand % xmm5, % xmm13, % xmm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <mask5=reg128#6,<x1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<x1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#14
# asm 2: psrlq $1,<v01=%xmm13
psrlq $1, % xmm13

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>x0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>x0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: x1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>x1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>x1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <mask4=reg128#5,<x2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<x2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: v10 = x3 & mask4
# asm 1: vpand <mask4=reg128#5,<x3=reg128#11,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x3=%xmm10,>v10=%xmm15
vpand % xmm4, % xmm10, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x2 & mask5
# asm 1: vpand <mask5=reg128#6,<x2=reg128#12,>v01=reg128#12
# asm 2: vpand <mask5=%xmm5,<x2=%xmm11,>v01=%xmm11
vpand % xmm5, % xmm11, % xmm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <mask5=reg128#6,<x3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<x3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#12
# asm 2: psrlq $1,<v01=%xmm11
psrlq $1, % xmm11

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>x3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>x3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <mask4=reg128#5,<x4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<x4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: v10 = x5 & mask4
# asm 1: vpand <mask4=reg128#5,<x5=reg128#9,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x5=%xmm8,>v10=%xmm15
vpand % xmm4, % xmm8, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x4 & mask5
# asm 1: vpand <mask5=reg128#6,<x4=reg128#13,>v01=reg128#13
# asm 2: vpand <mask5=%xmm5,<x4=%xmm12,>v01=%xmm12
vpand % xmm5, % xmm12, % xmm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <mask5=reg128#6,<x5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<x5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#13
# asm 2: psrlq $1,<v01=%xmm12
psrlq $1, % xmm12

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>x5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>x5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <mask4=reg128#5,<x6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<x6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: v10 = x7 & mask4
# asm 1: vpand <mask4=reg128#5,<x7=reg128#8,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x7=%xmm7,>v10=%xmm15
vpand % xmm4, % xmm7, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x6 & mask5
# asm 1: vpand <mask5=reg128#6,<x6=reg128#7,>v01=reg128#7
# asm 2: vpand <mask5=%xmm5,<x6=%xmm6,>v01=%xmm6
vpand % xmm5, % xmm6, % xmm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <mask5=reg128#6,<x7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<x7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#7
# asm 2: psrlq $1,<v01=%xmm6
psrlq $1, % xmm6

# qhasm: x6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>x7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>x7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: mem128[ input_0 + 384 ] = x0
# asm 1: movdqu   <x0=reg128#10,384(<input_0=int64#1)
# asm 2: movdqu   <x0=%xmm9,384(<input_0=%rdi)
movdqu   % xmm9, 384( % rdi)

# qhasm: mem128[ input_0 + 400 ] = x1
# asm 1: movdqu   <x1=reg128#14,400(<input_0=int64#1)
# asm 2: movdqu   <x1=%xmm13,400(<input_0=%rdi)
movdqu   % xmm13, 400( % rdi)

# qhasm: mem128[ input_0 + 416 ] = x2
# asm 1: movdqu   <x2=reg128#15,416(<input_0=int64#1)
# asm 2: movdqu   <x2=%xmm14,416(<input_0=%rdi)
movdqu   % xmm14, 416( % rdi)

# qhasm: mem128[ input_0 + 432 ] = x3
# asm 1: movdqu   <x3=reg128#11,432(<input_0=int64#1)
# asm 2: movdqu   <x3=%xmm10,432(<input_0=%rdi)
movdqu   % xmm10, 432( % rdi)

# qhasm: mem128[ input_0 + 448 ] = x4
# asm 1: movdqu   <x4=reg128#12,448(<input_0=int64#1)
# asm 2: movdqu   <x4=%xmm11,448(<input_0=%rdi)
movdqu   % xmm11, 448( % rdi)

# qhasm: mem128[ input_0 + 464 ] = x5
# asm 1: movdqu   <x5=reg128#9,464(<input_0=int64#1)
# asm 2: movdqu   <x5=%xmm8,464(<input_0=%rdi)
movdqu   % xmm8, 464( % rdi)

# qhasm: mem128[ input_0 + 480 ] = x6
# asm 1: movdqu   <x6=reg128#13,480(<input_0=int64#1)
# asm 2: movdqu   <x6=%xmm12,480(<input_0=%rdi)
movdqu   % xmm12, 480( % rdi)

# qhasm: mem128[ input_0 + 496 ] = x7
# asm 1: movdqu   <x7=reg128#7,496(<input_0=int64#1)
# asm 2: movdqu   <x7=%xmm6,496(<input_0=%rdi)
movdqu   % xmm6, 496( % rdi)

# qhasm: x0 = mem128[ input_0 + 512 ]
# asm 1: movdqu   512(<input_0=int64#1),>x0=reg128#7
# asm 2: movdqu   512(<input_0=%rdi),>x0=%xmm6
movdqu   512( % rdi), % xmm6

# qhasm: x1 = mem128[ input_0 + 528 ]
# asm 1: movdqu   528(<input_0=int64#1),>x1=reg128#8
# asm 2: movdqu   528(<input_0=%rdi),>x1=%xmm7
movdqu   528( % rdi), % xmm7

# qhasm: x2 = mem128[ input_0 + 544 ]
# asm 1: movdqu   544(<input_0=int64#1),>x2=reg128#9
# asm 2: movdqu   544(<input_0=%rdi),>x2=%xmm8
movdqu   544( % rdi), % xmm8

# qhasm: x3 = mem128[ input_0 + 560 ]
# asm 1: movdqu   560(<input_0=int64#1),>x3=reg128#10
# asm 2: movdqu   560(<input_0=%rdi),>x3=%xmm9
movdqu   560( % rdi), % xmm9

# qhasm: x4 = mem128[ input_0 + 576 ]
# asm 1: movdqu   576(<input_0=int64#1),>x4=reg128#11
# asm 2: movdqu   576(<input_0=%rdi),>x4=%xmm10
movdqu   576( % rdi), % xmm10

# qhasm: x5 = mem128[ input_0 + 592 ]
# asm 1: movdqu   592(<input_0=int64#1),>x5=reg128#12
# asm 2: movdqu   592(<input_0=%rdi),>x5=%xmm11
movdqu   592( % rdi), % xmm11

# qhasm: x6 = mem128[ input_0 + 608 ]
# asm 1: movdqu   608(<input_0=int64#1),>x6=reg128#13
# asm 2: movdqu   608(<input_0=%rdi),>x6=%xmm12
movdqu   608( % rdi), % xmm12

# qhasm: x7 = mem128[ input_0 + 624 ]
# asm 1: movdqu   624(<input_0=int64#1),>x7=reg128#14
# asm 2: movdqu   624(<input_0=%rdi),>x7=%xmm13
movdqu   624( % rdi), % xmm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <mask0=reg128#1,<x0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<x0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: v10 = x4 & mask0
# asm 1: vpand <mask0=reg128#1,<x4=reg128#11,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x4=%xmm10,>v10=%xmm15
vpand % xmm0, % xmm10, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x0 & mask1
# asm 1: vpand <mask1=reg128#2,<x0=reg128#7,>v01=reg128#7
# asm 2: vpand <mask1=%xmm1,<x0=%xmm6,>v01=%xmm6
vpand % xmm1, % xmm6, % xmm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <mask1=reg128#2,<x4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<x4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#7
# asm 2: psrlq $4,<v01=%xmm6
psrlq $4, % xmm6

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>x4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>x4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <mask0=reg128#1,<x1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<x1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: v10 = x5 & mask0
# asm 1: vpand <mask0=reg128#1,<x5=reg128#12,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x5=%xmm11,>v10=%xmm15
vpand % xmm0, % xmm11, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x1 & mask1
# asm 1: vpand <mask1=reg128#2,<x1=reg128#8,>v01=reg128#8
# asm 2: vpand <mask1=%xmm1,<x1=%xmm7,>v01=%xmm7
vpand % xmm1, % xmm7, % xmm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <mask1=reg128#2,<x5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<x5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#8
# asm 2: psrlq $4,<v01=%xmm7
psrlq $4, % xmm7

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>x1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>x1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>x5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>x5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <mask0=reg128#1,<x2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<x2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: v10 = x6 & mask0
# asm 1: vpand <mask0=reg128#1,<x6=reg128#13,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x6=%xmm12,>v10=%xmm15
vpand % xmm0, % xmm12, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x2 & mask1
# asm 1: vpand <mask1=reg128#2,<x2=reg128#9,>v01=reg128#9
# asm 2: vpand <mask1=%xmm1,<x2=%xmm8,>v01=%xmm8
vpand % xmm1, % xmm8, % xmm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <mask1=reg128#2,<x6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<x6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#9
# asm 2: psrlq $4,<v01=%xmm8
psrlq $4, % xmm8

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>x6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>x6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <mask0=reg128#1,<x3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<x3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: v10 = x7 & mask0
# asm 1: vpand <mask0=reg128#1,<x7=reg128#14,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x7=%xmm13,>v10=%xmm15
vpand % xmm0, % xmm13, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x3 & mask1
# asm 1: vpand <mask1=reg128#2,<x3=reg128#10,>v01=reg128#10
# asm 2: vpand <mask1=%xmm1,<x3=%xmm9,>v01=%xmm9
vpand % xmm1, % xmm9, % xmm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <mask1=reg128#2,<x7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<x7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#10
# asm 2: psrlq $4,<v01=%xmm9
psrlq $4, % xmm9

# qhasm: x3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>x7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>x7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <mask2=reg128#3,<x0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<x0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: v10 = x2 & mask2
# asm 1: vpand <mask2=reg128#3,<x2=reg128#12,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x2=%xmm11,>v10=%xmm15
vpand % xmm2, % xmm11, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x0 & mask3
# asm 1: vpand <mask3=reg128#4,<x0=reg128#15,>v01=reg128#15
# asm 2: vpand <mask3=%xmm3,<x0=%xmm14,>v01=%xmm14
vpand % xmm3, % xmm14, % xmm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <mask3=reg128#4,<x2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<x2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#15
# asm 2: psrlq $2,<v01=%xmm14
psrlq $2, % xmm14

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>x0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>x0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: x2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>x2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>x2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <mask2=reg128#3,<x1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<x1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: v10 = x3 & mask2
# asm 1: vpand <mask2=reg128#3,<x3=reg128#13,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x3=%xmm12,>v10=%xmm15
vpand % xmm2, % xmm12, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x1 & mask3
# asm 1: vpand <mask3=reg128#4,<x1=reg128#11,>v01=reg128#11
# asm 2: vpand <mask3=%xmm3,<x1=%xmm10,>v01=%xmm10
vpand % xmm3, % xmm10, % xmm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <mask3=reg128#4,<x3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<x3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#11
# asm 2: psrlq $2,<v01=%xmm10
psrlq $2, % xmm10

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>x3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>x3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <mask2=reg128#3,<x4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<x4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: v10 = x6 & mask2
# asm 1: vpand <mask2=reg128#3,<x6=reg128#9,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x6=%xmm8,>v10=%xmm15
vpand % xmm2, % xmm8, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x4 & mask3
# asm 1: vpand <mask3=reg128#4,<x4=reg128#7,>v01=reg128#7
# asm 2: vpand <mask3=%xmm3,<x4=%xmm6,>v01=%xmm6
vpand % xmm3, % xmm6, % xmm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <mask3=reg128#4,<x6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<x6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#7
# asm 2: psrlq $2,<v01=%xmm6
psrlq $2, % xmm6

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>x6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>x6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <mask2=reg128#3,<x5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<x5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: v10 = x7 & mask2
# asm 1: vpand <mask2=reg128#3,<x7=reg128#10,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x7=%xmm9,>v10=%xmm15
vpand % xmm2, % xmm9, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x5 & mask3
# asm 1: vpand <mask3=reg128#4,<x5=reg128#8,>v01=reg128#8
# asm 2: vpand <mask3=%xmm3,<x5=%xmm7,>v01=%xmm7
vpand % xmm3, % xmm7, % xmm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <mask3=reg128#4,<x7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<x7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#8
# asm 2: psrlq $2,<v01=%xmm7
psrlq $2, % xmm7

# qhasm: x5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>x5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>x5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>x7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>x7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <mask4=reg128#5,<x0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<x0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: v10 = x1 & mask4
# asm 1: vpand <mask4=reg128#5,<x1=reg128#15,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x1=%xmm14,>v10=%xmm15
vpand % xmm4, % xmm14, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x0 & mask5
# asm 1: vpand <mask5=reg128#6,<x0=reg128#14,>v01=reg128#14
# asm 2: vpand <mask5=%xmm5,<x0=%xmm13,>v01=%xmm13
vpand % xmm5, % xmm13, % xmm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <mask5=reg128#6,<x1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<x1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#14
# asm 2: psrlq $1,<v01=%xmm13
psrlq $1, % xmm13

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>x0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>x0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: x1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>x1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>x1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <mask4=reg128#5,<x2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<x2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: v10 = x3 & mask4
# asm 1: vpand <mask4=reg128#5,<x3=reg128#11,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x3=%xmm10,>v10=%xmm15
vpand % xmm4, % xmm10, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x2 & mask5
# asm 1: vpand <mask5=reg128#6,<x2=reg128#12,>v01=reg128#12
# asm 2: vpand <mask5=%xmm5,<x2=%xmm11,>v01=%xmm11
vpand % xmm5, % xmm11, % xmm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <mask5=reg128#6,<x3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<x3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#12
# asm 2: psrlq $1,<v01=%xmm11
psrlq $1, % xmm11

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>x3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>x3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <mask4=reg128#5,<x4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<x4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: v10 = x5 & mask4
# asm 1: vpand <mask4=reg128#5,<x5=reg128#9,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x5=%xmm8,>v10=%xmm15
vpand % xmm4, % xmm8, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x4 & mask5
# asm 1: vpand <mask5=reg128#6,<x4=reg128#13,>v01=reg128#13
# asm 2: vpand <mask5=%xmm5,<x4=%xmm12,>v01=%xmm12
vpand % xmm5, % xmm12, % xmm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <mask5=reg128#6,<x5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<x5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#13
# asm 2: psrlq $1,<v01=%xmm12
psrlq $1, % xmm12

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>x5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>x5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <mask4=reg128#5,<x6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<x6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: v10 = x7 & mask4
# asm 1: vpand <mask4=reg128#5,<x7=reg128#8,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x7=%xmm7,>v10=%xmm15
vpand % xmm4, % xmm7, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x6 & mask5
# asm 1: vpand <mask5=reg128#6,<x6=reg128#7,>v01=reg128#7
# asm 2: vpand <mask5=%xmm5,<x6=%xmm6,>v01=%xmm6
vpand % xmm5, % xmm6, % xmm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <mask5=reg128#6,<x7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<x7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#7
# asm 2: psrlq $1,<v01=%xmm6
psrlq $1, % xmm6

# qhasm: x6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>x7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>x7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: mem128[ input_0 + 512 ] = x0
# asm 1: movdqu   <x0=reg128#10,512(<input_0=int64#1)
# asm 2: movdqu   <x0=%xmm9,512(<input_0=%rdi)
movdqu   % xmm9, 512( % rdi)

# qhasm: mem128[ input_0 + 528 ] = x1
# asm 1: movdqu   <x1=reg128#14,528(<input_0=int64#1)
# asm 2: movdqu   <x1=%xmm13,528(<input_0=%rdi)
movdqu   % xmm13, 528( % rdi)

# qhasm: mem128[ input_0 + 544 ] = x2
# asm 1: movdqu   <x2=reg128#15,544(<input_0=int64#1)
# asm 2: movdqu   <x2=%xmm14,544(<input_0=%rdi)
movdqu   % xmm14, 544( % rdi)

# qhasm: mem128[ input_0 + 560 ] = x3
# asm 1: movdqu   <x3=reg128#11,560(<input_0=int64#1)
# asm 2: movdqu   <x3=%xmm10,560(<input_0=%rdi)
movdqu   % xmm10, 560( % rdi)

# qhasm: mem128[ input_0 + 576 ] = x4
# asm 1: movdqu   <x4=reg128#12,576(<input_0=int64#1)
# asm 2: movdqu   <x4=%xmm11,576(<input_0=%rdi)
movdqu   % xmm11, 576( % rdi)

# qhasm: mem128[ input_0 + 592 ] = x5
# asm 1: movdqu   <x5=reg128#9,592(<input_0=int64#1)
# asm 2: movdqu   <x5=%xmm8,592(<input_0=%rdi)
movdqu   % xmm8, 592( % rdi)

# qhasm: mem128[ input_0 + 608 ] = x6
# asm 1: movdqu   <x6=reg128#13,608(<input_0=int64#1)
# asm 2: movdqu   <x6=%xmm12,608(<input_0=%rdi)
movdqu   % xmm12, 608( % rdi)

# qhasm: mem128[ input_0 + 624 ] = x7
# asm 1: movdqu   <x7=reg128#7,624(<input_0=int64#1)
# asm 2: movdqu   <x7=%xmm6,624(<input_0=%rdi)
movdqu   % xmm6, 624( % rdi)

# qhasm: x0 = mem128[ input_0 + 640 ]
# asm 1: movdqu   640(<input_0=int64#1),>x0=reg128#7
# asm 2: movdqu   640(<input_0=%rdi),>x0=%xmm6
movdqu   640( % rdi), % xmm6

# qhasm: x1 = mem128[ input_0 + 656 ]
# asm 1: movdqu   656(<input_0=int64#1),>x1=reg128#8
# asm 2: movdqu   656(<input_0=%rdi),>x1=%xmm7
movdqu   656( % rdi), % xmm7

# qhasm: x2 = mem128[ input_0 + 672 ]
# asm 1: movdqu   672(<input_0=int64#1),>x2=reg128#9
# asm 2: movdqu   672(<input_0=%rdi),>x2=%xmm8
movdqu   672( % rdi), % xmm8

# qhasm: x3 = mem128[ input_0 + 688 ]
# asm 1: movdqu   688(<input_0=int64#1),>x3=reg128#10
# asm 2: movdqu   688(<input_0=%rdi),>x3=%xmm9
movdqu   688( % rdi), % xmm9

# qhasm: x4 = mem128[ input_0 + 704 ]
# asm 1: movdqu   704(<input_0=int64#1),>x4=reg128#11
# asm 2: movdqu   704(<input_0=%rdi),>x4=%xmm10
movdqu   704( % rdi), % xmm10

# qhasm: x5 = mem128[ input_0 + 720 ]
# asm 1: movdqu   720(<input_0=int64#1),>x5=reg128#12
# asm 2: movdqu   720(<input_0=%rdi),>x5=%xmm11
movdqu   720( % rdi), % xmm11

# qhasm: x6 = mem128[ input_0 + 736 ]
# asm 1: movdqu   736(<input_0=int64#1),>x6=reg128#13
# asm 2: movdqu   736(<input_0=%rdi),>x6=%xmm12
movdqu   736( % rdi), % xmm12

# qhasm: x7 = mem128[ input_0 + 752 ]
# asm 1: movdqu   752(<input_0=int64#1),>x7=reg128#14
# asm 2: movdqu   752(<input_0=%rdi),>x7=%xmm13
movdqu   752( % rdi), % xmm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <mask0=reg128#1,<x0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<x0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: v10 = x4 & mask0
# asm 1: vpand <mask0=reg128#1,<x4=reg128#11,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x4=%xmm10,>v10=%xmm15
vpand % xmm0, % xmm10, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x0 & mask1
# asm 1: vpand <mask1=reg128#2,<x0=reg128#7,>v01=reg128#7
# asm 2: vpand <mask1=%xmm1,<x0=%xmm6,>v01=%xmm6
vpand % xmm1, % xmm6, % xmm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <mask1=reg128#2,<x4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<x4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#7
# asm 2: psrlq $4,<v01=%xmm6
psrlq $4, % xmm6

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>x4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>x4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <mask0=reg128#1,<x1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<x1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: v10 = x5 & mask0
# asm 1: vpand <mask0=reg128#1,<x5=reg128#12,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x5=%xmm11,>v10=%xmm15
vpand % xmm0, % xmm11, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x1 & mask1
# asm 1: vpand <mask1=reg128#2,<x1=reg128#8,>v01=reg128#8
# asm 2: vpand <mask1=%xmm1,<x1=%xmm7,>v01=%xmm7
vpand % xmm1, % xmm7, % xmm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <mask1=reg128#2,<x5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<x5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#8
# asm 2: psrlq $4,<v01=%xmm7
psrlq $4, % xmm7

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>x1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>x1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>x5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>x5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <mask0=reg128#1,<x2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<x2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: v10 = x6 & mask0
# asm 1: vpand <mask0=reg128#1,<x6=reg128#13,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x6=%xmm12,>v10=%xmm15
vpand % xmm0, % xmm12, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x2 & mask1
# asm 1: vpand <mask1=reg128#2,<x2=reg128#9,>v01=reg128#9
# asm 2: vpand <mask1=%xmm1,<x2=%xmm8,>v01=%xmm8
vpand % xmm1, % xmm8, % xmm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <mask1=reg128#2,<x6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<x6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#9
# asm 2: psrlq $4,<v01=%xmm8
psrlq $4, % xmm8

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>x6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>x6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <mask0=reg128#1,<x3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<x3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: v10 = x7 & mask0
# asm 1: vpand <mask0=reg128#1,<x7=reg128#14,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x7=%xmm13,>v10=%xmm15
vpand % xmm0, % xmm13, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x3 & mask1
# asm 1: vpand <mask1=reg128#2,<x3=reg128#10,>v01=reg128#10
# asm 2: vpand <mask1=%xmm1,<x3=%xmm9,>v01=%xmm9
vpand % xmm1, % xmm9, % xmm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <mask1=reg128#2,<x7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<x7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#10
# asm 2: psrlq $4,<v01=%xmm9
psrlq $4, % xmm9

# qhasm: x3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>x7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>x7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <mask2=reg128#3,<x0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<x0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: v10 = x2 & mask2
# asm 1: vpand <mask2=reg128#3,<x2=reg128#12,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x2=%xmm11,>v10=%xmm15
vpand % xmm2, % xmm11, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x0 & mask3
# asm 1: vpand <mask3=reg128#4,<x0=reg128#15,>v01=reg128#15
# asm 2: vpand <mask3=%xmm3,<x0=%xmm14,>v01=%xmm14
vpand % xmm3, % xmm14, % xmm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <mask3=reg128#4,<x2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<x2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#15
# asm 2: psrlq $2,<v01=%xmm14
psrlq $2, % xmm14

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>x0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>x0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: x2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>x2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>x2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <mask2=reg128#3,<x1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<x1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: v10 = x3 & mask2
# asm 1: vpand <mask2=reg128#3,<x3=reg128#13,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x3=%xmm12,>v10=%xmm15
vpand % xmm2, % xmm12, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x1 & mask3
# asm 1: vpand <mask3=reg128#4,<x1=reg128#11,>v01=reg128#11
# asm 2: vpand <mask3=%xmm3,<x1=%xmm10,>v01=%xmm10
vpand % xmm3, % xmm10, % xmm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <mask3=reg128#4,<x3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<x3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#11
# asm 2: psrlq $2,<v01=%xmm10
psrlq $2, % xmm10

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>x3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>x3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <mask2=reg128#3,<x4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<x4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: v10 = x6 & mask2
# asm 1: vpand <mask2=reg128#3,<x6=reg128#9,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x6=%xmm8,>v10=%xmm15
vpand % xmm2, % xmm8, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x4 & mask3
# asm 1: vpand <mask3=reg128#4,<x4=reg128#7,>v01=reg128#7
# asm 2: vpand <mask3=%xmm3,<x4=%xmm6,>v01=%xmm6
vpand % xmm3, % xmm6, % xmm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <mask3=reg128#4,<x6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<x6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#7
# asm 2: psrlq $2,<v01=%xmm6
psrlq $2, % xmm6

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>x6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>x6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <mask2=reg128#3,<x5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<x5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: v10 = x7 & mask2
# asm 1: vpand <mask2=reg128#3,<x7=reg128#10,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x7=%xmm9,>v10=%xmm15
vpand % xmm2, % xmm9, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x5 & mask3
# asm 1: vpand <mask3=reg128#4,<x5=reg128#8,>v01=reg128#8
# asm 2: vpand <mask3=%xmm3,<x5=%xmm7,>v01=%xmm7
vpand % xmm3, % xmm7, % xmm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <mask3=reg128#4,<x7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<x7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#8
# asm 2: psrlq $2,<v01=%xmm7
psrlq $2, % xmm7

# qhasm: x5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>x5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>x5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>x7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>x7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <mask4=reg128#5,<x0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<x0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: v10 = x1 & mask4
# asm 1: vpand <mask4=reg128#5,<x1=reg128#15,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x1=%xmm14,>v10=%xmm15
vpand % xmm4, % xmm14, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x0 & mask5
# asm 1: vpand <mask5=reg128#6,<x0=reg128#14,>v01=reg128#14
# asm 2: vpand <mask5=%xmm5,<x0=%xmm13,>v01=%xmm13
vpand % xmm5, % xmm13, % xmm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <mask5=reg128#6,<x1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<x1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#14
# asm 2: psrlq $1,<v01=%xmm13
psrlq $1, % xmm13

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>x0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>x0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: x1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>x1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>x1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <mask4=reg128#5,<x2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<x2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: v10 = x3 & mask4
# asm 1: vpand <mask4=reg128#5,<x3=reg128#11,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x3=%xmm10,>v10=%xmm15
vpand % xmm4, % xmm10, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x2 & mask5
# asm 1: vpand <mask5=reg128#6,<x2=reg128#12,>v01=reg128#12
# asm 2: vpand <mask5=%xmm5,<x2=%xmm11,>v01=%xmm11
vpand % xmm5, % xmm11, % xmm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <mask5=reg128#6,<x3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<x3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#12
# asm 2: psrlq $1,<v01=%xmm11
psrlq $1, % xmm11

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>x3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>x3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <mask4=reg128#5,<x4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<x4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: v10 = x5 & mask4
# asm 1: vpand <mask4=reg128#5,<x5=reg128#9,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x5=%xmm8,>v10=%xmm15
vpand % xmm4, % xmm8, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x4 & mask5
# asm 1: vpand <mask5=reg128#6,<x4=reg128#13,>v01=reg128#13
# asm 2: vpand <mask5=%xmm5,<x4=%xmm12,>v01=%xmm12
vpand % xmm5, % xmm12, % xmm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <mask5=reg128#6,<x5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<x5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#13
# asm 2: psrlq $1,<v01=%xmm12
psrlq $1, % xmm12

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>x5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>x5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <mask4=reg128#5,<x6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<x6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: v10 = x7 & mask4
# asm 1: vpand <mask4=reg128#5,<x7=reg128#8,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x7=%xmm7,>v10=%xmm15
vpand % xmm4, % xmm7, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x6 & mask5
# asm 1: vpand <mask5=reg128#6,<x6=reg128#7,>v01=reg128#7
# asm 2: vpand <mask5=%xmm5,<x6=%xmm6,>v01=%xmm6
vpand % xmm5, % xmm6, % xmm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <mask5=reg128#6,<x7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<x7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#7
# asm 2: psrlq $1,<v01=%xmm6
psrlq $1, % xmm6

# qhasm: x6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>x7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>x7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: mem128[ input_0 + 640 ] = x0
# asm 1: movdqu   <x0=reg128#10,640(<input_0=int64#1)
# asm 2: movdqu   <x0=%xmm9,640(<input_0=%rdi)
movdqu   % xmm9, 640( % rdi)

# qhasm: mem128[ input_0 + 656 ] = x1
# asm 1: movdqu   <x1=reg128#14,656(<input_0=int64#1)
# asm 2: movdqu   <x1=%xmm13,656(<input_0=%rdi)
movdqu   % xmm13, 656( % rdi)

# qhasm: mem128[ input_0 + 672 ] = x2
# asm 1: movdqu   <x2=reg128#15,672(<input_0=int64#1)
# asm 2: movdqu   <x2=%xmm14,672(<input_0=%rdi)
movdqu   % xmm14, 672( % rdi)

# qhasm: mem128[ input_0 + 688 ] = x3
# asm 1: movdqu   <x3=reg128#11,688(<input_0=int64#1)
# asm 2: movdqu   <x3=%xmm10,688(<input_0=%rdi)
movdqu   % xmm10, 688( % rdi)

# qhasm: mem128[ input_0 + 704 ] = x4
# asm 1: movdqu   <x4=reg128#12,704(<input_0=int64#1)
# asm 2: movdqu   <x4=%xmm11,704(<input_0=%rdi)
movdqu   % xmm11, 704( % rdi)

# qhasm: mem128[ input_0 + 720 ] = x5
# asm 1: movdqu   <x5=reg128#9,720(<input_0=int64#1)
# asm 2: movdqu   <x5=%xmm8,720(<input_0=%rdi)
movdqu   % xmm8, 720( % rdi)

# qhasm: mem128[ input_0 + 736 ] = x6
# asm 1: movdqu   <x6=reg128#13,736(<input_0=int64#1)
# asm 2: movdqu   <x6=%xmm12,736(<input_0=%rdi)
movdqu   % xmm12, 736( % rdi)

# qhasm: mem128[ input_0 + 752 ] = x7
# asm 1: movdqu   <x7=reg128#7,752(<input_0=int64#1)
# asm 2: movdqu   <x7=%xmm6,752(<input_0=%rdi)
movdqu   % xmm6, 752( % rdi)

# qhasm: x0 = mem128[ input_0 + 768 ]
# asm 1: movdqu   768(<input_0=int64#1),>x0=reg128#7
# asm 2: movdqu   768(<input_0=%rdi),>x0=%xmm6
movdqu   768( % rdi), % xmm6

# qhasm: x1 = mem128[ input_0 + 784 ]
# asm 1: movdqu   784(<input_0=int64#1),>x1=reg128#8
# asm 2: movdqu   784(<input_0=%rdi),>x1=%xmm7
movdqu   784( % rdi), % xmm7

# qhasm: x2 = mem128[ input_0 + 800 ]
# asm 1: movdqu   800(<input_0=int64#1),>x2=reg128#9
# asm 2: movdqu   800(<input_0=%rdi),>x2=%xmm8
movdqu   800( % rdi), % xmm8

# qhasm: x3 = mem128[ input_0 + 816 ]
# asm 1: movdqu   816(<input_0=int64#1),>x3=reg128#10
# asm 2: movdqu   816(<input_0=%rdi),>x3=%xmm9
movdqu   816( % rdi), % xmm9

# qhasm: x4 = mem128[ input_0 + 832 ]
# asm 1: movdqu   832(<input_0=int64#1),>x4=reg128#11
# asm 2: movdqu   832(<input_0=%rdi),>x4=%xmm10
movdqu   832( % rdi), % xmm10

# qhasm: x5 = mem128[ input_0 + 848 ]
# asm 1: movdqu   848(<input_0=int64#1),>x5=reg128#12
# asm 2: movdqu   848(<input_0=%rdi),>x5=%xmm11
movdqu   848( % rdi), % xmm11

# qhasm: x6 = mem128[ input_0 + 864 ]
# asm 1: movdqu   864(<input_0=int64#1),>x6=reg128#13
# asm 2: movdqu   864(<input_0=%rdi),>x6=%xmm12
movdqu   864( % rdi), % xmm12

# qhasm: x7 = mem128[ input_0 + 880 ]
# asm 1: movdqu   880(<input_0=int64#1),>x7=reg128#14
# asm 2: movdqu   880(<input_0=%rdi),>x7=%xmm13
movdqu   880( % rdi), % xmm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <mask0=reg128#1,<x0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<x0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: v10 = x4 & mask0
# asm 1: vpand <mask0=reg128#1,<x4=reg128#11,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x4=%xmm10,>v10=%xmm15
vpand % xmm0, % xmm10, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x0 & mask1
# asm 1: vpand <mask1=reg128#2,<x0=reg128#7,>v01=reg128#7
# asm 2: vpand <mask1=%xmm1,<x0=%xmm6,>v01=%xmm6
vpand % xmm1, % xmm6, % xmm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <mask1=reg128#2,<x4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<x4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#7
# asm 2: psrlq $4,<v01=%xmm6
psrlq $4, % xmm6

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>x4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>x4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <mask0=reg128#1,<x1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<x1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: v10 = x5 & mask0
# asm 1: vpand <mask0=reg128#1,<x5=reg128#12,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x5=%xmm11,>v10=%xmm15
vpand % xmm0, % xmm11, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x1 & mask1
# asm 1: vpand <mask1=reg128#2,<x1=reg128#8,>v01=reg128#8
# asm 2: vpand <mask1=%xmm1,<x1=%xmm7,>v01=%xmm7
vpand % xmm1, % xmm7, % xmm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <mask1=reg128#2,<x5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<x5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#8
# asm 2: psrlq $4,<v01=%xmm7
psrlq $4, % xmm7

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>x1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>x1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>x5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>x5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <mask0=reg128#1,<x2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<x2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: v10 = x6 & mask0
# asm 1: vpand <mask0=reg128#1,<x6=reg128#13,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x6=%xmm12,>v10=%xmm15
vpand % xmm0, % xmm12, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x2 & mask1
# asm 1: vpand <mask1=reg128#2,<x2=reg128#9,>v01=reg128#9
# asm 2: vpand <mask1=%xmm1,<x2=%xmm8,>v01=%xmm8
vpand % xmm1, % xmm8, % xmm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <mask1=reg128#2,<x6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<x6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#9
# asm 2: psrlq $4,<v01=%xmm8
psrlq $4, % xmm8

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>x6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>x6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <mask0=reg128#1,<x3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<x3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: v10 = x7 & mask0
# asm 1: vpand <mask0=reg128#1,<x7=reg128#14,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x7=%xmm13,>v10=%xmm15
vpand % xmm0, % xmm13, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x3 & mask1
# asm 1: vpand <mask1=reg128#2,<x3=reg128#10,>v01=reg128#10
# asm 2: vpand <mask1=%xmm1,<x3=%xmm9,>v01=%xmm9
vpand % xmm1, % xmm9, % xmm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <mask1=reg128#2,<x7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<x7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#10
# asm 2: psrlq $4,<v01=%xmm9
psrlq $4, % xmm9

# qhasm: x3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>x7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>x7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = x0 & mask2
# asm 1: vpand <mask2=reg128#3,<x0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<x0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: v10 = x2 & mask2
# asm 1: vpand <mask2=reg128#3,<x2=reg128#12,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x2=%xmm11,>v10=%xmm15
vpand % xmm2, % xmm11, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x0 & mask3
# asm 1: vpand <mask3=reg128#4,<x0=reg128#15,>v01=reg128#15
# asm 2: vpand <mask3=%xmm3,<x0=%xmm14,>v01=%xmm14
vpand % xmm3, % xmm14, % xmm14

# qhasm: v11 = x2 & mask3
# asm 1: vpand <mask3=reg128#4,<x2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<x2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#15
# asm 2: psrlq $2,<v01=%xmm14
psrlq $2, % xmm14

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>x0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>x0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: x2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>x2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>x2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <mask2=reg128#3,<x1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<x1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: v10 = x3 & mask2
# asm 1: vpand <mask2=reg128#3,<x3=reg128#13,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x3=%xmm12,>v10=%xmm15
vpand % xmm2, % xmm12, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x1 & mask3
# asm 1: vpand <mask3=reg128#4,<x1=reg128#11,>v01=reg128#11
# asm 2: vpand <mask3=%xmm3,<x1=%xmm10,>v01=%xmm10
vpand % xmm3, % xmm10, % xmm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <mask3=reg128#4,<x3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<x3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#11
# asm 2: psrlq $2,<v01=%xmm10
psrlq $2, % xmm10

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>x3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>x3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = x4 & mask2
# asm 1: vpand <mask2=reg128#3,<x4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<x4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: v10 = x6 & mask2
# asm 1: vpand <mask2=reg128#3,<x6=reg128#9,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x6=%xmm8,>v10=%xmm15
vpand % xmm2, % xmm8, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x4 & mask3
# asm 1: vpand <mask3=reg128#4,<x4=reg128#7,>v01=reg128#7
# asm 2: vpand <mask3=%xmm3,<x4=%xmm6,>v01=%xmm6
vpand % xmm3, % xmm6, % xmm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <mask3=reg128#4,<x6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<x6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#7
# asm 2: psrlq $2,<v01=%xmm6
psrlq $2, % xmm6

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>x6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>x6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <mask2=reg128#3,<x5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<x5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: v10 = x7 & mask2
# asm 1: vpand <mask2=reg128#3,<x7=reg128#10,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<x7=%xmm9,>v10=%xmm15
vpand % xmm2, % xmm9, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = x5 & mask3
# asm 1: vpand <mask3=reg128#4,<x5=reg128#8,>v01=reg128#8
# asm 2: vpand <mask3=%xmm3,<x5=%xmm7,>v01=%xmm7
vpand % xmm3, % xmm7, % xmm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <mask3=reg128#4,<x7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<x7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#8
# asm 2: psrlq $2,<v01=%xmm7
psrlq $2, % xmm7

# qhasm: x5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>x5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>x5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>x7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>x7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = x0 & mask4
# asm 1: vpand <mask4=reg128#5,<x0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<x0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: v10 = x1 & mask4
# asm 1: vpand <mask4=reg128#5,<x1=reg128#15,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x1=%xmm14,>v10=%xmm15
vpand % xmm4, % xmm14, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x0 & mask5
# asm 1: vpand <mask5=reg128#6,<x0=reg128#14,>v01=reg128#14
# asm 2: vpand <mask5=%xmm5,<x0=%xmm13,>v01=%xmm13
vpand % xmm5, % xmm13, % xmm13

# qhasm: v11 = x1 & mask5
# asm 1: vpand <mask5=reg128#6,<x1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<x1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#14
# asm 2: psrlq $1,<v01=%xmm13
psrlq $1, % xmm13

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>x0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>x0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: x1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>x1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>x1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = x2 & mask4
# asm 1: vpand <mask4=reg128#5,<x2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<x2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: v10 = x3 & mask4
# asm 1: vpand <mask4=reg128#5,<x3=reg128#11,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x3=%xmm10,>v10=%xmm15
vpand % xmm4, % xmm10, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x2 & mask5
# asm 1: vpand <mask5=reg128#6,<x2=reg128#12,>v01=reg128#12
# asm 2: vpand <mask5=%xmm5,<x2=%xmm11,>v01=%xmm11
vpand % xmm5, % xmm11, % xmm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <mask5=reg128#6,<x3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<x3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#12
# asm 2: psrlq $1,<v01=%xmm11
psrlq $1, % xmm11

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>x3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>x3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = x4 & mask4
# asm 1: vpand <mask4=reg128#5,<x4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<x4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: v10 = x5 & mask4
# asm 1: vpand <mask4=reg128#5,<x5=reg128#9,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x5=%xmm8,>v10=%xmm15
vpand % xmm4, % xmm8, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x4 & mask5
# asm 1: vpand <mask5=reg128#6,<x4=reg128#13,>v01=reg128#13
# asm 2: vpand <mask5=%xmm5,<x4=%xmm12,>v01=%xmm12
vpand % xmm5, % xmm12, % xmm12

# qhasm: v11 = x5 & mask5
# asm 1: vpand <mask5=reg128#6,<x5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<x5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#13
# asm 2: psrlq $1,<v01=%xmm12
psrlq $1, % xmm12

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>x5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>x5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = x6 & mask4
# asm 1: vpand <mask4=reg128#5,<x6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<x6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: v10 = x7 & mask4
# asm 1: vpand <mask4=reg128#5,<x7=reg128#8,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<x7=%xmm7,>v10=%xmm15
vpand % xmm4, % xmm7, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = x6 & mask5
# asm 1: vpand <mask5=reg128#6,<x6=reg128#7,>v01=reg128#7
# asm 2: vpand <mask5=%xmm5,<x6=%xmm6,>v01=%xmm6
vpand % xmm5, % xmm6, % xmm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <mask5=reg128#6,<x7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<x7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#7
# asm 2: psrlq $1,<v01=%xmm6
psrlq $1, % xmm6

# qhasm: x6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>x6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>x6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>x7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>x7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: mem128[ input_0 + 768 ] = x0
# asm 1: movdqu   <x0=reg128#10,768(<input_0=int64#1)
# asm 2: movdqu   <x0=%xmm9,768(<input_0=%rdi)
movdqu   % xmm9, 768( % rdi)

# qhasm: mem128[ input_0 + 784 ] = x1
# asm 1: movdqu   <x1=reg128#14,784(<input_0=int64#1)
# asm 2: movdqu   <x1=%xmm13,784(<input_0=%rdi)
movdqu   % xmm13, 784( % rdi)

# qhasm: mem128[ input_0 + 800 ] = x2
# asm 1: movdqu   <x2=reg128#15,800(<input_0=int64#1)
# asm 2: movdqu   <x2=%xmm14,800(<input_0=%rdi)
movdqu   % xmm14, 800( % rdi)

# qhasm: mem128[ input_0 + 816 ] = x3
# asm 1: movdqu   <x3=reg128#11,816(<input_0=int64#1)
# asm 2: movdqu   <x3=%xmm10,816(<input_0=%rdi)
movdqu   % xmm10, 816( % rdi)

# qhasm: mem128[ input_0 + 832 ] = x4
# asm 1: movdqu   <x4=reg128#12,832(<input_0=int64#1)
# asm 2: movdqu   <x4=%xmm11,832(<input_0=%rdi)
movdqu   % xmm11, 832( % rdi)

# qhasm: mem128[ input_0 + 848 ] = x5
# asm 1: movdqu   <x5=reg128#9,848(<input_0=int64#1)
# asm 2: movdqu   <x5=%xmm8,848(<input_0=%rdi)
movdqu   % xmm8, 848( % rdi)

# qhasm: mem128[ input_0 + 864 ] = x6
# asm 1: movdqu   <x6=reg128#13,864(<input_0=int64#1)
# asm 2: movdqu   <x6=%xmm12,864(<input_0=%rdi)
movdqu   % xmm12, 864( % rdi)

# qhasm: mem128[ input_0 + 880 ] = x7
# asm 1: movdqu   <x7=reg128#7,880(<input_0=int64#1)
# asm 2: movdqu   <x7=%xmm6,880(<input_0=%rdi)
movdqu   % xmm6, 880( % rdi)

# qhasm: x0 = mem128[ input_0 + 896 ]
# asm 1: movdqu   896(<input_0=int64#1),>x0=reg128#7
# asm 2: movdqu   896(<input_0=%rdi),>x0=%xmm6
movdqu   896( % rdi), % xmm6

# qhasm: x1 = mem128[ input_0 + 912 ]
# asm 1: movdqu   912(<input_0=int64#1),>x1=reg128#8
# asm 2: movdqu   912(<input_0=%rdi),>x1=%xmm7
movdqu   912( % rdi), % xmm7

# qhasm: x2 = mem128[ input_0 + 928 ]
# asm 1: movdqu   928(<input_0=int64#1),>x2=reg128#9
# asm 2: movdqu   928(<input_0=%rdi),>x2=%xmm8
movdqu   928( % rdi), % xmm8

# qhasm: x3 = mem128[ input_0 + 944 ]
# asm 1: movdqu   944(<input_0=int64#1),>x3=reg128#10
# asm 2: movdqu   944(<input_0=%rdi),>x3=%xmm9
movdqu   944( % rdi), % xmm9

# qhasm: x4 = mem128[ input_0 + 960 ]
# asm 1: movdqu   960(<input_0=int64#1),>x4=reg128#11
# asm 2: movdqu   960(<input_0=%rdi),>x4=%xmm10
movdqu   960( % rdi), % xmm10

# qhasm: x5 = mem128[ input_0 + 976 ]
# asm 1: movdqu   976(<input_0=int64#1),>x5=reg128#12
# asm 2: movdqu   976(<input_0=%rdi),>x5=%xmm11
movdqu   976( % rdi), % xmm11

# qhasm: x6 = mem128[ input_0 + 992 ]
# asm 1: movdqu   992(<input_0=int64#1),>x6=reg128#13
# asm 2: movdqu   992(<input_0=%rdi),>x6=%xmm12
movdqu   992( % rdi), % xmm12

# qhasm: x7 = mem128[ input_0 + 1008 ]
# asm 1: movdqu   1008(<input_0=int64#1),>x7=reg128#14
# asm 2: movdqu   1008(<input_0=%rdi),>x7=%xmm13
movdqu   1008( % rdi), % xmm13

# qhasm: v00 = x0 & mask0
# asm 1: vpand <mask0=reg128#1,<x0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<x0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: v10 = x4 & mask0
# asm 1: vpand <mask0=reg128#1,<x4=reg128#11,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x4=%xmm10,>v10=%xmm15
vpand % xmm0, % xmm10, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x0 & mask1
# asm 1: vpand <mask1=reg128#2,<x0=reg128#7,>v01=reg128#7
# asm 2: vpand <mask1=%xmm1,<x0=%xmm6,>v01=%xmm6
vpand % xmm1, % xmm6, % xmm6

# qhasm: v11 = x4 & mask1
# asm 1: vpand <mask1=reg128#2,<x4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<x4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#7
# asm 2: psrlq $4,<v01=%xmm6
psrlq $4, % xmm6

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>x0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>x0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: x4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>x4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>x4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = x1 & mask0
# asm 1: vpand <mask0=reg128#1,<x1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<x1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: v10 = x5 & mask0
# asm 1: vpand <mask0=reg128#1,<x5=reg128#12,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x5=%xmm11,>v10=%xmm15
vpand % xmm0, % xmm11, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x1 & mask1
# asm 1: vpand <mask1=reg128#2,<x1=reg128#8,>v01=reg128#8
# asm 2: vpand <mask1=%xmm1,<x1=%xmm7,>v01=%xmm7
vpand % xmm1, % xmm7, % xmm7

# qhasm: v11 = x5 & mask1
# asm 1: vpand <mask1=reg128#2,<x5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<x5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#8
# asm 2: psrlq $4,<v01=%xmm7
psrlq $4, % xmm7

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>x1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>x1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>x5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>x5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = x2 & mask0
# asm 1: vpand <mask0=reg128#1,<x2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<x2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: v10 = x6 & mask0
# asm 1: vpand <mask0=reg128#1,<x6=reg128#13,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<x6=%xmm12,>v10=%xmm15
vpand % xmm0, % xmm12, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = x2 & mask1
# asm 1: vpand <mask1=reg128#2,<x2=reg128#9,>v01=reg128#9
# asm 2: vpand <mask1=%xmm1,<x2=%xmm8,>v01=%xmm8
vpand % xmm1, % xmm8, % xmm8

# qhasm: v11 = x6 & mask1
# asm 1: vpand <mask1=reg128#2,<x6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<x6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#9
# asm 2: psrlq $4,<v01=%xmm8
psrlq $4, % xmm8

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>x2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>x2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>x6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>x6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = x3 & mask0
# asm 1: vpand <mask0=reg128#1,<x3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<x3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: v10 = x7 & mask0
# asm 1: vpand <mask0=reg128#1,<x7=reg128#14,>v10=reg128#1
# asm 2: vpand <mask0=%xmm0,<x7=%xmm13,>v10=%xmm0
vpand % xmm0, % xmm13, % xmm0

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#1
# asm 2: psllq $4,<v10=%xmm0
psllq $4, % xmm0

# qhasm: v01 = x3 & mask1
# asm 1: vpand <mask1=reg128#2,<x3=reg128#10,>v01=reg128#10
# asm 2: vpand <mask1=%xmm1,<x3=%xmm9,>v01=%xmm9
vpand % xmm1, % xmm9, % xmm9

# qhasm: v11 = x7 & mask1
# asm 1: vpand <mask1=reg128#2,<x7=reg128#14,>v11=reg128#2
# asm 2: vpand <mask1=%xmm1,<x7=%xmm13,>v11=%xmm1
vpand % xmm1, % xmm13, % xmm1

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#10
# asm 2: psrlq $4,<v01=%xmm9
psrlq $4, % xmm9

# qhasm: x3 = v00 | v10
# asm 1: vpor <v10=reg128#1,<v00=reg128#13,>x3=reg128#1
# asm 2: vpor <v10=%xmm0,<v00=%xmm12,>x3=%xmm0
vpor % xmm0, % xmm12, % xmm0

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#2,<v01=reg128#10,>x7=reg128#2
# asm 2: vpor <v11=%xmm1,<v01=%xmm9,>x7=%xmm1
vpor % xmm1, % xmm9, % xmm1

# qhasm: v00 = x0 & mask2
# asm 1: vpand <mask2=reg128#3,<x0=reg128#15,>v00=reg128#10
# asm 2: vpand <mask2=%xmm2,<x0=%xmm14,>v00=%xmm9
vpand % xmm2, % xmm14, % xmm9

# qhasm: v10 = x2 & mask2
# asm 1: vpand <mask2=reg128#3,<x2=reg128#12,>v10=reg128#13
# asm 2: vpand <mask2=%xmm2,<x2=%xmm11,>v10=%xmm12
vpand % xmm2, % xmm11, % xmm12

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#13
# asm 2: psllq $2,<v10=%xmm12
psllq $2, % xmm12

# qhasm: v01 = x0 & mask3
# asm 1: vpand <mask3=reg128#4,<x0=reg128#15,>v01=reg128#14
# asm 2: vpand <mask3=%xmm3,<x0=%xmm14,>v01=%xmm13
vpand % xmm3, % xmm14, % xmm13

# qhasm: v11 = x2 & mask3
# asm 1: vpand <mask3=reg128#4,<x2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<x2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#14
# asm 2: psrlq $2,<v01=%xmm13
psrlq $2, % xmm13

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#13,<v00=reg128#10,>x0=reg128#10
# asm 2: vpor <v10=%xmm12,<v00=%xmm9,>x0=%xmm9
vpor % xmm12, % xmm9, % xmm9

# qhasm: x2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#14,>x2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm13,>x2=%xmm11
vpor % xmm11, % xmm13, % xmm11

# qhasm: v00 = x1 & mask2
# asm 1: vpand <mask2=reg128#3,<x1=reg128#11,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<x1=%xmm10,>v00=%xmm12
vpand % xmm2, % xmm10, % xmm12

# qhasm: v10 = x3 & mask2
# asm 1: vpand <mask2=reg128#3,<x3=reg128#1,>v10=reg128#14
# asm 2: vpand <mask2=%xmm2,<x3=%xmm0,>v10=%xmm13
vpand % xmm2, % xmm0, % xmm13

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#14
# asm 2: psllq $2,<v10=%xmm13
psllq $2, % xmm13

# qhasm: v01 = x1 & mask3
# asm 1: vpand <mask3=reg128#4,<x1=reg128#11,>v01=reg128#11
# asm 2: vpand <mask3=%xmm3,<x1=%xmm10,>v01=%xmm10
vpand % xmm3, % xmm10, % xmm10

# qhasm: v11 = x3 & mask3
# asm 1: vpand <mask3=reg128#4,<x3=reg128#1,>v11=reg128#1
# asm 2: vpand <mask3=%xmm3,<x3=%xmm0,>v11=%xmm0
vpand % xmm3, % xmm0, % xmm0

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#11
# asm 2: psrlq $2,<v01=%xmm10
psrlq $2, % xmm10

# qhasm: x1 = v00 | v10
# asm 1: vpor <v10=reg128#14,<v00=reg128#13,>x1=reg128#13
# asm 2: vpor <v10=%xmm13,<v00=%xmm12,>x1=%xmm12
vpor % xmm13, % xmm12, % xmm12

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#1,<v01=reg128#11,>x3=reg128#1
# asm 2: vpor <v11=%xmm0,<v01=%xmm10,>x3=%xmm0
vpor % xmm0, % xmm10, % xmm0

# qhasm: v00 = x4 & mask2
# asm 1: vpand <mask2=reg128#3,<x4=reg128#7,>v00=reg128#11
# asm 2: vpand <mask2=%xmm2,<x4=%xmm6,>v00=%xmm10
vpand % xmm2, % xmm6, % xmm10

# qhasm: v10 = x6 & mask2
# asm 1: vpand <mask2=reg128#3,<x6=reg128#9,>v10=reg128#14
# asm 2: vpand <mask2=%xmm2,<x6=%xmm8,>v10=%xmm13
vpand % xmm2, % xmm8, % xmm13

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#14
# asm 2: psllq $2,<v10=%xmm13
psllq $2, % xmm13

# qhasm: v01 = x4 & mask3
# asm 1: vpand <mask3=reg128#4,<x4=reg128#7,>v01=reg128#7
# asm 2: vpand <mask3=%xmm3,<x4=%xmm6,>v01=%xmm6
vpand % xmm3, % xmm6, % xmm6

# qhasm: v11 = x6 & mask3
# asm 1: vpand <mask3=reg128#4,<x6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<x6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#7
# asm 2: psrlq $2,<v01=%xmm6
psrlq $2, % xmm6

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#14,<v00=reg128#11,>x4=reg128#11
# asm 2: vpor <v10=%xmm13,<v00=%xmm10,>x4=%xmm10
vpor % xmm13, % xmm10, % xmm10

# qhasm: x6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>x6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>x6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = x5 & mask2
# asm 1: vpand <mask2=reg128#3,<x5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<x5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: v10 = x7 & mask2
# asm 1: vpand <mask2=reg128#3,<x7=reg128#2,>v10=reg128#3
# asm 2: vpand <mask2=%xmm2,<x7=%xmm1,>v10=%xmm2
vpand % xmm2, % xmm1, % xmm2

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#3
# asm 2: psllq $2,<v10=%xmm2
psllq $2, % xmm2

# qhasm: v01 = x5 & mask3
# asm 1: vpand <mask3=reg128#4,<x5=reg128#8,>v01=reg128#8
# asm 2: vpand <mask3=%xmm3,<x5=%xmm7,>v01=%xmm7
vpand % xmm3, % xmm7, % xmm7

# qhasm: v11 = x7 & mask3
# asm 1: vpand <mask3=reg128#4,<x7=reg128#2,>v11=reg128#2
# asm 2: vpand <mask3=%xmm3,<x7=%xmm1,>v11=%xmm1
vpand % xmm3, % xmm1, % xmm1

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#8
# asm 2: psrlq $2,<v01=%xmm7
psrlq $2, % xmm7

# qhasm: x5 = v00 | v10
# asm 1: vpor <v10=reg128#3,<v00=reg128#9,>x5=reg128#3
# asm 2: vpor <v10=%xmm2,<v00=%xmm8,>x5=%xmm2
vpor % xmm2, % xmm8, % xmm2

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#2,<v01=reg128#8,>x7=reg128#2
# asm 2: vpor <v11=%xmm1,<v01=%xmm7,>x7=%xmm1
vpor % xmm1, % xmm7, % xmm1

# qhasm: v00 = x0 & mask4
# asm 1: vpand <mask4=reg128#5,<x0=reg128#10,>v00=reg128#4
# asm 2: vpand <mask4=%xmm4,<x0=%xmm9,>v00=%xmm3
vpand % xmm4, % xmm9, % xmm3

# qhasm: v10 = x1 & mask4
# asm 1: vpand <mask4=reg128#5,<x1=reg128#13,>v10=reg128#8
# asm 2: vpand <mask4=%xmm4,<x1=%xmm12,>v10=%xmm7
vpand % xmm4, % xmm12, % xmm7

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#8
# asm 2: psllq $1,<v10=%xmm7
psllq $1, % xmm7

# qhasm: v01 = x0 & mask5
# asm 1: vpand <mask5=reg128#6,<x0=reg128#10,>v01=reg128#9
# asm 2: vpand <mask5=%xmm5,<x0=%xmm9,>v01=%xmm8
vpand % xmm5, % xmm9, % xmm8

# qhasm: v11 = x1 & mask5
# asm 1: vpand <mask5=reg128#6,<x1=reg128#13,>v11=reg128#10
# asm 2: vpand <mask5=%xmm5,<x1=%xmm12,>v11=%xmm9
vpand % xmm5, % xmm12, % xmm9

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#9
# asm 2: psrlq $1,<v01=%xmm8
psrlq $1, % xmm8

# qhasm: x0 = v00 | v10
# asm 1: vpor <v10=reg128#8,<v00=reg128#4,>x0=reg128#4
# asm 2: vpor <v10=%xmm7,<v00=%xmm3,>x0=%xmm3
vpor % xmm7, % xmm3, % xmm3

# qhasm: x1 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#9,>x1=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm8,>x1=%xmm7
vpor % xmm9, % xmm8, % xmm7

# qhasm: v00 = x2 & mask4
# asm 1: vpand <mask4=reg128#5,<x2=reg128#12,>v00=reg128#9
# asm 2: vpand <mask4=%xmm4,<x2=%xmm11,>v00=%xmm8
vpand % xmm4, % xmm11, % xmm8

# qhasm: v10 = x3 & mask4
# asm 1: vpand <mask4=reg128#5,<x3=reg128#1,>v10=reg128#10
# asm 2: vpand <mask4=%xmm4,<x3=%xmm0,>v10=%xmm9
vpand % xmm4, % xmm0, % xmm9

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#10
# asm 2: psllq $1,<v10=%xmm9
psllq $1, % xmm9

# qhasm: v01 = x2 & mask5
# asm 1: vpand <mask5=reg128#6,<x2=reg128#12,>v01=reg128#12
# asm 2: vpand <mask5=%xmm5,<x2=%xmm11,>v01=%xmm11
vpand % xmm5, % xmm11, % xmm11

# qhasm: v11 = x3 & mask5
# asm 1: vpand <mask5=reg128#6,<x3=reg128#1,>v11=reg128#1
# asm 2: vpand <mask5=%xmm5,<x3=%xmm0,>v11=%xmm0
vpand % xmm5, % xmm0, % xmm0

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#12
# asm 2: psrlq $1,<v01=%xmm11
psrlq $1, % xmm11

# qhasm: x2 = v00 | v10
# asm 1: vpor <v10=reg128#10,<v00=reg128#9,>x2=reg128#9
# asm 2: vpor <v10=%xmm9,<v00=%xmm8,>x2=%xmm8
vpor % xmm9, % xmm8, % xmm8

# qhasm: x3 = v01 | v11
# asm 1: vpor <v11=reg128#1,<v01=reg128#12,>x3=reg128#1
# asm 2: vpor <v11=%xmm0,<v01=%xmm11,>x3=%xmm0
vpor % xmm0, % xmm11, % xmm0

# qhasm: v00 = x4 & mask4
# asm 1: vpand <mask4=reg128#5,<x4=reg128#11,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<x4=%xmm10,>v00=%xmm9
vpand % xmm4, % xmm10, % xmm9

# qhasm: v10 = x5 & mask4
# asm 1: vpand <mask4=reg128#5,<x5=reg128#3,>v10=reg128#12
# asm 2: vpand <mask4=%xmm4,<x5=%xmm2,>v10=%xmm11
vpand % xmm4, % xmm2, % xmm11

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#12
# asm 2: psllq $1,<v10=%xmm11
psllq $1, % xmm11

# qhasm: v01 = x4 & mask5
# asm 1: vpand <mask5=reg128#6,<x4=reg128#11,>v01=reg128#11
# asm 2: vpand <mask5=%xmm5,<x4=%xmm10,>v01=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: v11 = x5 & mask5
# asm 1: vpand <mask5=reg128#6,<x5=reg128#3,>v11=reg128#3
# asm 2: vpand <mask5=%xmm5,<x5=%xmm2,>v11=%xmm2
vpand % xmm5, % xmm2, % xmm2

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#11
# asm 2: psrlq $1,<v01=%xmm10
psrlq $1, % xmm10

# qhasm: x4 = v00 | v10
# asm 1: vpor <v10=reg128#12,<v00=reg128#10,>x4=reg128#10
# asm 2: vpor <v10=%xmm11,<v00=%xmm9,>x4=%xmm9
vpor % xmm11, % xmm9, % xmm9

# qhasm: x5 = v01 | v11
# asm 1: vpor <v11=reg128#3,<v01=reg128#11,>x5=reg128#3
# asm 2: vpor <v11=%xmm2,<v01=%xmm10,>x5=%xmm2
vpor % xmm2, % xmm10, % xmm2

# qhasm: v00 = x6 & mask4
# asm 1: vpand <mask4=reg128#5,<x6=reg128#7,>v00=reg128#11
# asm 2: vpand <mask4=%xmm4,<x6=%xmm6,>v00=%xmm10
vpand % xmm4, % xmm6, % xmm10

# qhasm: v10 = x7 & mask4
# asm 1: vpand <mask4=reg128#5,<x7=reg128#2,>v10=reg128#5
# asm 2: vpand <mask4=%xmm4,<x7=%xmm1,>v10=%xmm4
vpand % xmm4, % xmm1, % xmm4

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#5
# asm 2: psllq $1,<v10=%xmm4
psllq $1, % xmm4

# qhasm: v01 = x6 & mask5
# asm 1: vpand <mask5=reg128#6,<x6=reg128#7,>v01=reg128#7
# asm 2: vpand <mask5=%xmm5,<x6=%xmm6,>v01=%xmm6
vpand % xmm5, % xmm6, % xmm6

# qhasm: v11 = x7 & mask5
# asm 1: vpand <mask5=reg128#6,<x7=reg128#2,>v11=reg128#2
# asm 2: vpand <mask5=%xmm5,<x7=%xmm1,>v11=%xmm1
vpand % xmm5, % xmm1, % xmm1

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#7
# asm 2: psrlq $1,<v01=%xmm6
psrlq $1, % xmm6

# qhasm: x6 = v00 | v10
# asm 1: vpor <v10=reg128#5,<v00=reg128#11,>x6=reg128#5
# asm 2: vpor <v10=%xmm4,<v00=%xmm10,>x6=%xmm4
vpor % xmm4, % xmm10, % xmm4

# qhasm: x7 = v01 | v11
# asm 1: vpor <v11=reg128#2,<v01=reg128#7,>x7=reg128#2
# asm 2: vpor <v11=%xmm1,<v01=%xmm6,>x7=%xmm1
vpor % xmm1, % xmm6, % xmm1

# qhasm: mem128[ input_0 + 896 ] = x0
# asm 1: movdqu   <x0=reg128#4,896(<input_0=int64#1)
# asm 2: movdqu   <x0=%xmm3,896(<input_0=%rdi)
movdqu   % xmm3, 896( % rdi)

# qhasm: mem128[ input_0 + 912 ] = x1
# asm 1: movdqu   <x1=reg128#8,912(<input_0=int64#1)
# asm 2: movdqu   <x1=%xmm7,912(<input_0=%rdi)
movdqu   % xmm7, 912( % rdi)

# qhasm: mem128[ input_0 + 928 ] = x2
# asm 1: movdqu   <x2=reg128#9,928(<input_0=int64#1)
# asm 2: movdqu   <x2=%xmm8,928(<input_0=%rdi)
movdqu   % xmm8, 928( % rdi)

# qhasm: mem128[ input_0 + 944 ] = x3
# asm 1: movdqu   <x3=reg128#1,944(<input_0=int64#1)
# asm 2: movdqu   <x3=%xmm0,944(<input_0=%rdi)
movdqu   % xmm0, 944( % rdi)

# qhasm: mem128[ input_0 + 960 ] = x4
# asm 1: movdqu   <x4=reg128#10,960(<input_0=int64#1)
# asm 2: movdqu   <x4=%xmm9,960(<input_0=%rdi)
movdqu   % xmm9, 960( % rdi)

# qhasm: mem128[ input_0 + 976 ] = x5
# asm 1: movdqu   <x5=reg128#3,976(<input_0=int64#1)
# asm 2: movdqu   <x5=%xmm2,976(<input_0=%rdi)
movdqu   % xmm2, 976( % rdi)

# qhasm: mem128[ input_0 + 992 ] = x6
# asm 1: movdqu   <x6=reg128#5,992(<input_0=int64#1)
# asm 2: movdqu   <x6=%xmm4,992(<input_0=%rdi)
movdqu   % xmm4, 992( % rdi)

# qhasm: mem128[ input_0 + 1008 ] = x7
# asm 1: movdqu   <x7=reg128#2,1008(<input_0=int64#1)
# asm 2: movdqu   <x7=%xmm1,1008(<input_0=%rdi)
movdqu   % xmm1, 1008( % rdi)

# qhasm: return
add % r11, % rsp
ret
