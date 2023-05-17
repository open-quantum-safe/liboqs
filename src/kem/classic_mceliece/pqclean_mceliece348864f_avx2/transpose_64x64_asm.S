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
#define transpose_64x64_asm CRYPTO_NAMESPACE(transpose_64x64_asm)
#define _transpose_64x64_asm _CRYPTO_NAMESPACE(transpose_64x64_asm)

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

# qhasm: reg128 r0

# qhasm: reg128 r1

# qhasm: reg128 r2

# qhasm: reg128 r3

# qhasm: reg128 r4

# qhasm: reg128 r5

# qhasm: reg128 r6

# qhasm: reg128 r7

# qhasm: reg128 t0

# qhasm: reg128 t1

# qhasm: reg128 v00

# qhasm: reg128 v01

# qhasm: reg128 v10

# qhasm: reg128 v11

# qhasm: int64 buf

# qhasm: reg128 mask0

# qhasm: reg128 mask1

# qhasm: reg128 mask2

# qhasm: reg128 mask3

# qhasm: reg128 mask4

# qhasm: reg128 mask5

# qhasm: enter transpose_64x64_asm
.p2align 5
.global _transpose_64x64_asm
.global transpose_64x64_asm
_transpose_64x64_asm:
transpose_64x64_asm:
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

# qhasm: r0 = mem64[ input_0 + 0 ] x2
# asm 1: movddup 0(<input_0=int64#1),>r0=reg128#7
# asm 2: movddup 0(<input_0=%rdi),>r0=%xmm6
movddup 0( % rdi), % xmm6

# qhasm: r1 = mem64[ input_0 + 64 ] x2
# asm 1: movddup 64(<input_0=int64#1),>r1=reg128#8
# asm 2: movddup 64(<input_0=%rdi),>r1=%xmm7
movddup 64( % rdi), % xmm7

# qhasm: r2 = mem64[ input_0 + 128 ] x2
# asm 1: movddup 128(<input_0=int64#1),>r2=reg128#9
# asm 2: movddup 128(<input_0=%rdi),>r2=%xmm8
movddup 128( % rdi), % xmm8

# qhasm: r3 = mem64[ input_0 + 192 ] x2
# asm 1: movddup 192(<input_0=int64#1),>r3=reg128#10
# asm 2: movddup 192(<input_0=%rdi),>r3=%xmm9
movddup 192( % rdi), % xmm9

# qhasm: r4 = mem64[ input_0 + 256 ] x2
# asm 1: movddup 256(<input_0=int64#1),>r4=reg128#11
# asm 2: movddup 256(<input_0=%rdi),>r4=%xmm10
movddup 256( % rdi), % xmm10

# qhasm: r5 = mem64[ input_0 + 320 ] x2
# asm 1: movddup 320(<input_0=int64#1),>r5=reg128#12
# asm 2: movddup 320(<input_0=%rdi),>r5=%xmm11
movddup 320( % rdi), % xmm11

# qhasm: r6 = mem64[ input_0 + 384 ] x2
# asm 1: movddup 384(<input_0=int64#1),>r6=reg128#13
# asm 2: movddup 384(<input_0=%rdi),>r6=%xmm12
movddup 384( % rdi), % xmm12

# qhasm: r7 = mem64[ input_0 + 448 ] x2
# asm 1: movddup 448(<input_0=int64#1),>r7=reg128#14
# asm 2: movddup 448(<input_0=%rdi),>r7=%xmm13
movddup 448( % rdi), % xmm13

# qhasm: v00 = r0 & mask0
# asm 1: vpand <mask0=reg128#1,<r0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<r0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: 2x v10 = r4 << 32
# asm 1: vpsllq $32,<r4=reg128#11,>v10=reg128#16
# asm 2: vpsllq $32,<r4=%xmm10,>v10=%xmm15
vpsllq $32, % xmm10, % xmm15

# qhasm: 2x v01 = r0 unsigned>> 32
# asm 1: vpsrlq $32,<r0=reg128#7,>v01=reg128#7
# asm 2: vpsrlq $32,<r0=%xmm6,>v01=%xmm6
vpsrlq $32, % xmm6, % xmm6

# qhasm: v11 = r4 & mask1
# asm 1: vpand <mask1=reg128#2,<r4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<r4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>r4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>r4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = r1 & mask0
# asm 1: vpand <mask0=reg128#1,<r1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<r1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: 2x v10 = r5 << 32
# asm 1: vpsllq $32,<r5=reg128#12,>v10=reg128#16
# asm 2: vpsllq $32,<r5=%xmm11,>v10=%xmm15
vpsllq $32, % xmm11, % xmm15

# qhasm: 2x v01 = r1 unsigned>> 32
# asm 1: vpsrlq $32,<r1=reg128#8,>v01=reg128#8
# asm 2: vpsrlq $32,<r1=%xmm7,>v01=%xmm7
vpsrlq $32, % xmm7, % xmm7

# qhasm: v11 = r5 & mask1
# asm 1: vpand <mask1=reg128#2,<r5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<r5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>r1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>r1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>r5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>r5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = r2 & mask0
# asm 1: vpand <mask0=reg128#1,<r2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<r2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: 2x v10 = r6 << 32
# asm 1: vpsllq $32,<r6=reg128#13,>v10=reg128#16
# asm 2: vpsllq $32,<r6=%xmm12,>v10=%xmm15
vpsllq $32, % xmm12, % xmm15

# qhasm: 2x v01 = r2 unsigned>> 32
# asm 1: vpsrlq $32,<r2=reg128#9,>v01=reg128#9
# asm 2: vpsrlq $32,<r2=%xmm8,>v01=%xmm8
vpsrlq $32, % xmm8, % xmm8

# qhasm: v11 = r6 & mask1
# asm 1: vpand <mask1=reg128#2,<r6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<r6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>r6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>r6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = r3 & mask0
# asm 1: vpand <mask0=reg128#1,<r3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<r3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: 2x v10 = r7 << 32
# asm 1: vpsllq $32,<r7=reg128#14,>v10=reg128#16
# asm 2: vpsllq $32,<r7=%xmm13,>v10=%xmm15
vpsllq $32, % xmm13, % xmm15

# qhasm: 2x v01 = r3 unsigned>> 32
# asm 1: vpsrlq $32,<r3=reg128#10,>v01=reg128#10
# asm 2: vpsrlq $32,<r3=%xmm9,>v01=%xmm9
vpsrlq $32, % xmm9, % xmm9

# qhasm: v11 = r7 & mask1
# asm 1: vpand <mask1=reg128#2,<r7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<r7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: r3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>r7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>r7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = r0 & mask2
# asm 1: vpand <mask2=reg128#3,<r0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<r0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: 4x v10 = r2 << 16
# asm 1: vpslld $16,<r2=reg128#12,>v10=reg128#16
# asm 2: vpslld $16,<r2=%xmm11,>v10=%xmm15
vpslld $16, % xmm11, % xmm15

# qhasm: 4x v01 = r0 unsigned>> 16
# asm 1: vpsrld $16,<r0=reg128#15,>v01=reg128#15
# asm 2: vpsrld $16,<r0=%xmm14,>v01=%xmm14
vpsrld $16, % xmm14, % xmm14

# qhasm: v11 = r2 & mask3
# asm 1: vpand <mask3=reg128#4,<r2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<r2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>r0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>r0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: r2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>r2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>r2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = r1 & mask2
# asm 1: vpand <mask2=reg128#3,<r1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<r1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: 4x v10 = r3 << 16
# asm 1: vpslld $16,<r3=reg128#13,>v10=reg128#16
# asm 2: vpslld $16,<r3=%xmm12,>v10=%xmm15
vpslld $16, % xmm12, % xmm15

# qhasm: 4x v01 = r1 unsigned>> 16
# asm 1: vpsrld $16,<r1=reg128#11,>v01=reg128#11
# asm 2: vpsrld $16,<r1=%xmm10,>v01=%xmm10
vpsrld $16, % xmm10, % xmm10

# qhasm: v11 = r3 & mask3
# asm 1: vpand <mask3=reg128#4,<r3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<r3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>r3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>r3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = r4 & mask2
# asm 1: vpand <mask2=reg128#3,<r4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<r4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: 4x v10 = r6 << 16
# asm 1: vpslld $16,<r6=reg128#9,>v10=reg128#16
# asm 2: vpslld $16,<r6=%xmm8,>v10=%xmm15
vpslld $16, % xmm8, % xmm15

# qhasm: 4x v01 = r4 unsigned>> 16
# asm 1: vpsrld $16,<r4=reg128#7,>v01=reg128#7
# asm 2: vpsrld $16,<r4=%xmm6,>v01=%xmm6
vpsrld $16, % xmm6, % xmm6

# qhasm: v11 = r6 & mask3
# asm 1: vpand <mask3=reg128#4,<r6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<r6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>r6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>r6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = r5 & mask2
# asm 1: vpand <mask2=reg128#3,<r5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<r5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: 4x v10 = r7 << 16
# asm 1: vpslld $16,<r7=reg128#10,>v10=reg128#16
# asm 2: vpslld $16,<r7=%xmm9,>v10=%xmm15
vpslld $16, % xmm9, % xmm15

# qhasm: 4x v01 = r5 unsigned>> 16
# asm 1: vpsrld $16,<r5=reg128#8,>v01=reg128#8
# asm 2: vpsrld $16,<r5=%xmm7,>v01=%xmm7
vpsrld $16, % xmm7, % xmm7

# qhasm: v11 = r7 & mask3
# asm 1: vpand <mask3=reg128#4,<r7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<r7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: r5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>r5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>r5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>r7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>r7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = r0 & mask4
# asm 1: vpand <mask4=reg128#5,<r0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<r0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: 8x v10 = r1 << 8
# asm 1: vpsllw $8,<r1=reg128#15,>v10=reg128#16
# asm 2: vpsllw $8,<r1=%xmm14,>v10=%xmm15
vpsllw $8, % xmm14, % xmm15

# qhasm: 8x v01 = r0 unsigned>> 8
# asm 1: vpsrlw $8,<r0=reg128#14,>v01=reg128#14
# asm 2: vpsrlw $8,<r0=%xmm13,>v01=%xmm13
vpsrlw $8, % xmm13, % xmm13

# qhasm: v11 = r1 & mask5
# asm 1: vpand <mask5=reg128#6,<r1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<r1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>r0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>r0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: r1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>r1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>r1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = r2 & mask4
# asm 1: vpand <mask4=reg128#5,<r2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<r2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: 8x v10 = r3 << 8
# asm 1: vpsllw $8,<r3=reg128#11,>v10=reg128#16
# asm 2: vpsllw $8,<r3=%xmm10,>v10=%xmm15
vpsllw $8, % xmm10, % xmm15

# qhasm: 8x v01 = r2 unsigned>> 8
# asm 1: vpsrlw $8,<r2=reg128#12,>v01=reg128#12
# asm 2: vpsrlw $8,<r2=%xmm11,>v01=%xmm11
vpsrlw $8, % xmm11, % xmm11

# qhasm: v11 = r3 & mask5
# asm 1: vpand <mask5=reg128#6,<r3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<r3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>r3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>r3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = r4 & mask4
# asm 1: vpand <mask4=reg128#5,<r4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<r4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: 8x v10 = r5 << 8
# asm 1: vpsllw $8,<r5=reg128#9,>v10=reg128#16
# asm 2: vpsllw $8,<r5=%xmm8,>v10=%xmm15
vpsllw $8, % xmm8, % xmm15

# qhasm: 8x v01 = r4 unsigned>> 8
# asm 1: vpsrlw $8,<r4=reg128#13,>v01=reg128#13
# asm 2: vpsrlw $8,<r4=%xmm12,>v01=%xmm12
vpsrlw $8, % xmm12, % xmm12

# qhasm: v11 = r5 & mask5
# asm 1: vpand <mask5=reg128#6,<r5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<r5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>r5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>r5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = r6 & mask4
# asm 1: vpand <mask4=reg128#5,<r6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<r6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: 8x v10 = r7 << 8
# asm 1: vpsllw $8,<r7=reg128#8,>v10=reg128#16
# asm 2: vpsllw $8,<r7=%xmm7,>v10=%xmm15
vpsllw $8, % xmm7, % xmm15

# qhasm: 8x v01 = r6 unsigned>> 8
# asm 1: vpsrlw $8,<r6=reg128#7,>v01=reg128#7
# asm 2: vpsrlw $8,<r6=%xmm6,>v01=%xmm6
vpsrlw $8, % xmm6, % xmm6

# qhasm: v11 = r7 & mask5
# asm 1: vpand <mask5=reg128#6,<r7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<r7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: r6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>r7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>r7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: buf = r0[0]
# asm 1: pextrq $0x0,<r0=reg128#10,>buf=int64#2
# asm 2: pextrq $0x0,<r0=%xmm9,>buf=%rsi
pextrq $0x0, % xmm9, % rsi

# qhasm: mem64[ input_0 + 0 ] = buf
# asm 1: movq   <buf=int64#2,0(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,0(<input_0=%rdi)
movq   % rsi, 0( % rdi)

# qhasm: buf = r1[0]
# asm 1: pextrq $0x0,<r1=reg128#14,>buf=int64#2
# asm 2: pextrq $0x0,<r1=%xmm13,>buf=%rsi
pextrq $0x0, % xmm13, % rsi

# qhasm: mem64[ input_0 + 64 ] = buf
# asm 1: movq   <buf=int64#2,64(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,64(<input_0=%rdi)
movq   % rsi, 64( % rdi)

# qhasm: buf = r2[0]
# asm 1: pextrq $0x0,<r2=reg128#15,>buf=int64#2
# asm 2: pextrq $0x0,<r2=%xmm14,>buf=%rsi
pextrq $0x0, % xmm14, % rsi

# qhasm: mem64[ input_0 + 128 ] = buf
# asm 1: movq   <buf=int64#2,128(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,128(<input_0=%rdi)
movq   % rsi, 128( % rdi)

# qhasm: buf = r3[0]
# asm 1: pextrq $0x0,<r3=reg128#11,>buf=int64#2
# asm 2: pextrq $0x0,<r3=%xmm10,>buf=%rsi
pextrq $0x0, % xmm10, % rsi

# qhasm: mem64[ input_0 + 192 ] = buf
# asm 1: movq   <buf=int64#2,192(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,192(<input_0=%rdi)
movq   % rsi, 192( % rdi)

# qhasm: buf = r4[0]
# asm 1: pextrq $0x0,<r4=reg128#12,>buf=int64#2
# asm 2: pextrq $0x0,<r4=%xmm11,>buf=%rsi
pextrq $0x0, % xmm11, % rsi

# qhasm: mem64[ input_0 + 256 ] = buf
# asm 1: movq   <buf=int64#2,256(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,256(<input_0=%rdi)
movq   % rsi, 256( % rdi)

# qhasm: buf = r5[0]
# asm 1: pextrq $0x0,<r5=reg128#9,>buf=int64#2
# asm 2: pextrq $0x0,<r5=%xmm8,>buf=%rsi
pextrq $0x0, % xmm8, % rsi

# qhasm: mem64[ input_0 + 320 ] = buf
# asm 1: movq   <buf=int64#2,320(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,320(<input_0=%rdi)
movq   % rsi, 320( % rdi)

# qhasm: buf = r6[0]
# asm 1: pextrq $0x0,<r6=reg128#13,>buf=int64#2
# asm 2: pextrq $0x0,<r6=%xmm12,>buf=%rsi
pextrq $0x0, % xmm12, % rsi

# qhasm: mem64[ input_0 + 384 ] = buf
# asm 1: movq   <buf=int64#2,384(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,384(<input_0=%rdi)
movq   % rsi, 384( % rdi)

# qhasm: buf = r7[0]
# asm 1: pextrq $0x0,<r7=reg128#7,>buf=int64#2
# asm 2: pextrq $0x0,<r7=%xmm6,>buf=%rsi
pextrq $0x0, % xmm6, % rsi

# qhasm: mem64[ input_0 + 448 ] = buf
# asm 1: movq   <buf=int64#2,448(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,448(<input_0=%rdi)
movq   % rsi, 448( % rdi)

# qhasm: r0 = mem64[ input_0 + 8 ] x2
# asm 1: movddup 8(<input_0=int64#1),>r0=reg128#7
# asm 2: movddup 8(<input_0=%rdi),>r0=%xmm6
movddup 8( % rdi), % xmm6

# qhasm: r1 = mem64[ input_0 + 72 ] x2
# asm 1: movddup 72(<input_0=int64#1),>r1=reg128#8
# asm 2: movddup 72(<input_0=%rdi),>r1=%xmm7
movddup 72( % rdi), % xmm7

# qhasm: r2 = mem64[ input_0 + 136 ] x2
# asm 1: movddup 136(<input_0=int64#1),>r2=reg128#9
# asm 2: movddup 136(<input_0=%rdi),>r2=%xmm8
movddup 136( % rdi), % xmm8

# qhasm: r3 = mem64[ input_0 + 200 ] x2
# asm 1: movddup 200(<input_0=int64#1),>r3=reg128#10
# asm 2: movddup 200(<input_0=%rdi),>r3=%xmm9
movddup 200( % rdi), % xmm9

# qhasm: r4 = mem64[ input_0 + 264 ] x2
# asm 1: movddup 264(<input_0=int64#1),>r4=reg128#11
# asm 2: movddup 264(<input_0=%rdi),>r4=%xmm10
movddup 264( % rdi), % xmm10

# qhasm: r5 = mem64[ input_0 + 328 ] x2
# asm 1: movddup 328(<input_0=int64#1),>r5=reg128#12
# asm 2: movddup 328(<input_0=%rdi),>r5=%xmm11
movddup 328( % rdi), % xmm11

# qhasm: r6 = mem64[ input_0 + 392 ] x2
# asm 1: movddup 392(<input_0=int64#1),>r6=reg128#13
# asm 2: movddup 392(<input_0=%rdi),>r6=%xmm12
movddup 392( % rdi), % xmm12

# qhasm: r7 = mem64[ input_0 + 456 ] x2
# asm 1: movddup 456(<input_0=int64#1),>r7=reg128#14
# asm 2: movddup 456(<input_0=%rdi),>r7=%xmm13
movddup 456( % rdi), % xmm13

# qhasm: v00 = r0 & mask0
# asm 1: vpand <mask0=reg128#1,<r0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<r0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: 2x v10 = r4 << 32
# asm 1: vpsllq $32,<r4=reg128#11,>v10=reg128#16
# asm 2: vpsllq $32,<r4=%xmm10,>v10=%xmm15
vpsllq $32, % xmm10, % xmm15

# qhasm: 2x v01 = r0 unsigned>> 32
# asm 1: vpsrlq $32,<r0=reg128#7,>v01=reg128#7
# asm 2: vpsrlq $32,<r0=%xmm6,>v01=%xmm6
vpsrlq $32, % xmm6, % xmm6

# qhasm: v11 = r4 & mask1
# asm 1: vpand <mask1=reg128#2,<r4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<r4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>r4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>r4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = r1 & mask0
# asm 1: vpand <mask0=reg128#1,<r1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<r1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: 2x v10 = r5 << 32
# asm 1: vpsllq $32,<r5=reg128#12,>v10=reg128#16
# asm 2: vpsllq $32,<r5=%xmm11,>v10=%xmm15
vpsllq $32, % xmm11, % xmm15

# qhasm: 2x v01 = r1 unsigned>> 32
# asm 1: vpsrlq $32,<r1=reg128#8,>v01=reg128#8
# asm 2: vpsrlq $32,<r1=%xmm7,>v01=%xmm7
vpsrlq $32, % xmm7, % xmm7

# qhasm: v11 = r5 & mask1
# asm 1: vpand <mask1=reg128#2,<r5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<r5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>r1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>r1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>r5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>r5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = r2 & mask0
# asm 1: vpand <mask0=reg128#1,<r2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<r2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: 2x v10 = r6 << 32
# asm 1: vpsllq $32,<r6=reg128#13,>v10=reg128#16
# asm 2: vpsllq $32,<r6=%xmm12,>v10=%xmm15
vpsllq $32, % xmm12, % xmm15

# qhasm: 2x v01 = r2 unsigned>> 32
# asm 1: vpsrlq $32,<r2=reg128#9,>v01=reg128#9
# asm 2: vpsrlq $32,<r2=%xmm8,>v01=%xmm8
vpsrlq $32, % xmm8, % xmm8

# qhasm: v11 = r6 & mask1
# asm 1: vpand <mask1=reg128#2,<r6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<r6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>r6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>r6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = r3 & mask0
# asm 1: vpand <mask0=reg128#1,<r3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<r3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: 2x v10 = r7 << 32
# asm 1: vpsllq $32,<r7=reg128#14,>v10=reg128#16
# asm 2: vpsllq $32,<r7=%xmm13,>v10=%xmm15
vpsllq $32, % xmm13, % xmm15

# qhasm: 2x v01 = r3 unsigned>> 32
# asm 1: vpsrlq $32,<r3=reg128#10,>v01=reg128#10
# asm 2: vpsrlq $32,<r3=%xmm9,>v01=%xmm9
vpsrlq $32, % xmm9, % xmm9

# qhasm: v11 = r7 & mask1
# asm 1: vpand <mask1=reg128#2,<r7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<r7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: r3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>r7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>r7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = r0 & mask2
# asm 1: vpand <mask2=reg128#3,<r0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<r0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: 4x v10 = r2 << 16
# asm 1: vpslld $16,<r2=reg128#12,>v10=reg128#16
# asm 2: vpslld $16,<r2=%xmm11,>v10=%xmm15
vpslld $16, % xmm11, % xmm15

# qhasm: 4x v01 = r0 unsigned>> 16
# asm 1: vpsrld $16,<r0=reg128#15,>v01=reg128#15
# asm 2: vpsrld $16,<r0=%xmm14,>v01=%xmm14
vpsrld $16, % xmm14, % xmm14

# qhasm: v11 = r2 & mask3
# asm 1: vpand <mask3=reg128#4,<r2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<r2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>r0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>r0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: r2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>r2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>r2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = r1 & mask2
# asm 1: vpand <mask2=reg128#3,<r1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<r1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: 4x v10 = r3 << 16
# asm 1: vpslld $16,<r3=reg128#13,>v10=reg128#16
# asm 2: vpslld $16,<r3=%xmm12,>v10=%xmm15
vpslld $16, % xmm12, % xmm15

# qhasm: 4x v01 = r1 unsigned>> 16
# asm 1: vpsrld $16,<r1=reg128#11,>v01=reg128#11
# asm 2: vpsrld $16,<r1=%xmm10,>v01=%xmm10
vpsrld $16, % xmm10, % xmm10

# qhasm: v11 = r3 & mask3
# asm 1: vpand <mask3=reg128#4,<r3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<r3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>r3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>r3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = r4 & mask2
# asm 1: vpand <mask2=reg128#3,<r4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<r4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: 4x v10 = r6 << 16
# asm 1: vpslld $16,<r6=reg128#9,>v10=reg128#16
# asm 2: vpslld $16,<r6=%xmm8,>v10=%xmm15
vpslld $16, % xmm8, % xmm15

# qhasm: 4x v01 = r4 unsigned>> 16
# asm 1: vpsrld $16,<r4=reg128#7,>v01=reg128#7
# asm 2: vpsrld $16,<r4=%xmm6,>v01=%xmm6
vpsrld $16, % xmm6, % xmm6

# qhasm: v11 = r6 & mask3
# asm 1: vpand <mask3=reg128#4,<r6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<r6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>r6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>r6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = r5 & mask2
# asm 1: vpand <mask2=reg128#3,<r5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<r5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: 4x v10 = r7 << 16
# asm 1: vpslld $16,<r7=reg128#10,>v10=reg128#16
# asm 2: vpslld $16,<r7=%xmm9,>v10=%xmm15
vpslld $16, % xmm9, % xmm15

# qhasm: 4x v01 = r5 unsigned>> 16
# asm 1: vpsrld $16,<r5=reg128#8,>v01=reg128#8
# asm 2: vpsrld $16,<r5=%xmm7,>v01=%xmm7
vpsrld $16, % xmm7, % xmm7

# qhasm: v11 = r7 & mask3
# asm 1: vpand <mask3=reg128#4,<r7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<r7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: r5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>r5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>r5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>r7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>r7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = r0 & mask4
# asm 1: vpand <mask4=reg128#5,<r0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<r0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: 8x v10 = r1 << 8
# asm 1: vpsllw $8,<r1=reg128#15,>v10=reg128#16
# asm 2: vpsllw $8,<r1=%xmm14,>v10=%xmm15
vpsllw $8, % xmm14, % xmm15

# qhasm: 8x v01 = r0 unsigned>> 8
# asm 1: vpsrlw $8,<r0=reg128#14,>v01=reg128#14
# asm 2: vpsrlw $8,<r0=%xmm13,>v01=%xmm13
vpsrlw $8, % xmm13, % xmm13

# qhasm: v11 = r1 & mask5
# asm 1: vpand <mask5=reg128#6,<r1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<r1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>r0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>r0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: r1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>r1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>r1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = r2 & mask4
# asm 1: vpand <mask4=reg128#5,<r2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<r2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: 8x v10 = r3 << 8
# asm 1: vpsllw $8,<r3=reg128#11,>v10=reg128#16
# asm 2: vpsllw $8,<r3=%xmm10,>v10=%xmm15
vpsllw $8, % xmm10, % xmm15

# qhasm: 8x v01 = r2 unsigned>> 8
# asm 1: vpsrlw $8,<r2=reg128#12,>v01=reg128#12
# asm 2: vpsrlw $8,<r2=%xmm11,>v01=%xmm11
vpsrlw $8, % xmm11, % xmm11

# qhasm: v11 = r3 & mask5
# asm 1: vpand <mask5=reg128#6,<r3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<r3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>r3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>r3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = r4 & mask4
# asm 1: vpand <mask4=reg128#5,<r4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<r4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: 8x v10 = r5 << 8
# asm 1: vpsllw $8,<r5=reg128#9,>v10=reg128#16
# asm 2: vpsllw $8,<r5=%xmm8,>v10=%xmm15
vpsllw $8, % xmm8, % xmm15

# qhasm: 8x v01 = r4 unsigned>> 8
# asm 1: vpsrlw $8,<r4=reg128#13,>v01=reg128#13
# asm 2: vpsrlw $8,<r4=%xmm12,>v01=%xmm12
vpsrlw $8, % xmm12, % xmm12

# qhasm: v11 = r5 & mask5
# asm 1: vpand <mask5=reg128#6,<r5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<r5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>r5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>r5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = r6 & mask4
# asm 1: vpand <mask4=reg128#5,<r6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<r6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: 8x v10 = r7 << 8
# asm 1: vpsllw $8,<r7=reg128#8,>v10=reg128#16
# asm 2: vpsllw $8,<r7=%xmm7,>v10=%xmm15
vpsllw $8, % xmm7, % xmm15

# qhasm: 8x v01 = r6 unsigned>> 8
# asm 1: vpsrlw $8,<r6=reg128#7,>v01=reg128#7
# asm 2: vpsrlw $8,<r6=%xmm6,>v01=%xmm6
vpsrlw $8, % xmm6, % xmm6

# qhasm: v11 = r7 & mask5
# asm 1: vpand <mask5=reg128#6,<r7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<r7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: r6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>r7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>r7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: buf = r0[0]
# asm 1: pextrq $0x0,<r0=reg128#10,>buf=int64#2
# asm 2: pextrq $0x0,<r0=%xmm9,>buf=%rsi
pextrq $0x0, % xmm9, % rsi

# qhasm: mem64[ input_0 + 8 ] = buf
# asm 1: movq   <buf=int64#2,8(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,8(<input_0=%rdi)
movq   % rsi, 8( % rdi)

# qhasm: buf = r1[0]
# asm 1: pextrq $0x0,<r1=reg128#14,>buf=int64#2
# asm 2: pextrq $0x0,<r1=%xmm13,>buf=%rsi
pextrq $0x0, % xmm13, % rsi

# qhasm: mem64[ input_0 + 72 ] = buf
# asm 1: movq   <buf=int64#2,72(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,72(<input_0=%rdi)
movq   % rsi, 72( % rdi)

# qhasm: buf = r2[0]
# asm 1: pextrq $0x0,<r2=reg128#15,>buf=int64#2
# asm 2: pextrq $0x0,<r2=%xmm14,>buf=%rsi
pextrq $0x0, % xmm14, % rsi

# qhasm: mem64[ input_0 + 136 ] = buf
# asm 1: movq   <buf=int64#2,136(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,136(<input_0=%rdi)
movq   % rsi, 136( % rdi)

# qhasm: buf = r3[0]
# asm 1: pextrq $0x0,<r3=reg128#11,>buf=int64#2
# asm 2: pextrq $0x0,<r3=%xmm10,>buf=%rsi
pextrq $0x0, % xmm10, % rsi

# qhasm: mem64[ input_0 + 200 ] = buf
# asm 1: movq   <buf=int64#2,200(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,200(<input_0=%rdi)
movq   % rsi, 200( % rdi)

# qhasm: buf = r4[0]
# asm 1: pextrq $0x0,<r4=reg128#12,>buf=int64#2
# asm 2: pextrq $0x0,<r4=%xmm11,>buf=%rsi
pextrq $0x0, % xmm11, % rsi

# qhasm: mem64[ input_0 + 264 ] = buf
# asm 1: movq   <buf=int64#2,264(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,264(<input_0=%rdi)
movq   % rsi, 264( % rdi)

# qhasm: buf = r5[0]
# asm 1: pextrq $0x0,<r5=reg128#9,>buf=int64#2
# asm 2: pextrq $0x0,<r5=%xmm8,>buf=%rsi
pextrq $0x0, % xmm8, % rsi

# qhasm: mem64[ input_0 + 328 ] = buf
# asm 1: movq   <buf=int64#2,328(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,328(<input_0=%rdi)
movq   % rsi, 328( % rdi)

# qhasm: buf = r6[0]
# asm 1: pextrq $0x0,<r6=reg128#13,>buf=int64#2
# asm 2: pextrq $0x0,<r6=%xmm12,>buf=%rsi
pextrq $0x0, % xmm12, % rsi

# qhasm: mem64[ input_0 + 392 ] = buf
# asm 1: movq   <buf=int64#2,392(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,392(<input_0=%rdi)
movq   % rsi, 392( % rdi)

# qhasm: buf = r7[0]
# asm 1: pextrq $0x0,<r7=reg128#7,>buf=int64#2
# asm 2: pextrq $0x0,<r7=%xmm6,>buf=%rsi
pextrq $0x0, % xmm6, % rsi

# qhasm: mem64[ input_0 + 456 ] = buf
# asm 1: movq   <buf=int64#2,456(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,456(<input_0=%rdi)
movq   % rsi, 456( % rdi)

# qhasm: r0 = mem64[ input_0 + 16 ] x2
# asm 1: movddup 16(<input_0=int64#1),>r0=reg128#7
# asm 2: movddup 16(<input_0=%rdi),>r0=%xmm6
movddup 16( % rdi), % xmm6

# qhasm: r1 = mem64[ input_0 + 80 ] x2
# asm 1: movddup 80(<input_0=int64#1),>r1=reg128#8
# asm 2: movddup 80(<input_0=%rdi),>r1=%xmm7
movddup 80( % rdi), % xmm7

# qhasm: r2 = mem64[ input_0 + 144 ] x2
# asm 1: movddup 144(<input_0=int64#1),>r2=reg128#9
# asm 2: movddup 144(<input_0=%rdi),>r2=%xmm8
movddup 144( % rdi), % xmm8

# qhasm: r3 = mem64[ input_0 + 208 ] x2
# asm 1: movddup 208(<input_0=int64#1),>r3=reg128#10
# asm 2: movddup 208(<input_0=%rdi),>r3=%xmm9
movddup 208( % rdi), % xmm9

# qhasm: r4 = mem64[ input_0 + 272 ] x2
# asm 1: movddup 272(<input_0=int64#1),>r4=reg128#11
# asm 2: movddup 272(<input_0=%rdi),>r4=%xmm10
movddup 272( % rdi), % xmm10

# qhasm: r5 = mem64[ input_0 + 336 ] x2
# asm 1: movddup 336(<input_0=int64#1),>r5=reg128#12
# asm 2: movddup 336(<input_0=%rdi),>r5=%xmm11
movddup 336( % rdi), % xmm11

# qhasm: r6 = mem64[ input_0 + 400 ] x2
# asm 1: movddup 400(<input_0=int64#1),>r6=reg128#13
# asm 2: movddup 400(<input_0=%rdi),>r6=%xmm12
movddup 400( % rdi), % xmm12

# qhasm: r7 = mem64[ input_0 + 464 ] x2
# asm 1: movddup 464(<input_0=int64#1),>r7=reg128#14
# asm 2: movddup 464(<input_0=%rdi),>r7=%xmm13
movddup 464( % rdi), % xmm13

# qhasm: v00 = r0 & mask0
# asm 1: vpand <mask0=reg128#1,<r0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<r0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: 2x v10 = r4 << 32
# asm 1: vpsllq $32,<r4=reg128#11,>v10=reg128#16
# asm 2: vpsllq $32,<r4=%xmm10,>v10=%xmm15
vpsllq $32, % xmm10, % xmm15

# qhasm: 2x v01 = r0 unsigned>> 32
# asm 1: vpsrlq $32,<r0=reg128#7,>v01=reg128#7
# asm 2: vpsrlq $32,<r0=%xmm6,>v01=%xmm6
vpsrlq $32, % xmm6, % xmm6

# qhasm: v11 = r4 & mask1
# asm 1: vpand <mask1=reg128#2,<r4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<r4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>r4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>r4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = r1 & mask0
# asm 1: vpand <mask0=reg128#1,<r1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<r1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: 2x v10 = r5 << 32
# asm 1: vpsllq $32,<r5=reg128#12,>v10=reg128#16
# asm 2: vpsllq $32,<r5=%xmm11,>v10=%xmm15
vpsllq $32, % xmm11, % xmm15

# qhasm: 2x v01 = r1 unsigned>> 32
# asm 1: vpsrlq $32,<r1=reg128#8,>v01=reg128#8
# asm 2: vpsrlq $32,<r1=%xmm7,>v01=%xmm7
vpsrlq $32, % xmm7, % xmm7

# qhasm: v11 = r5 & mask1
# asm 1: vpand <mask1=reg128#2,<r5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<r5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>r1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>r1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>r5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>r5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = r2 & mask0
# asm 1: vpand <mask0=reg128#1,<r2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<r2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: 2x v10 = r6 << 32
# asm 1: vpsllq $32,<r6=reg128#13,>v10=reg128#16
# asm 2: vpsllq $32,<r6=%xmm12,>v10=%xmm15
vpsllq $32, % xmm12, % xmm15

# qhasm: 2x v01 = r2 unsigned>> 32
# asm 1: vpsrlq $32,<r2=reg128#9,>v01=reg128#9
# asm 2: vpsrlq $32,<r2=%xmm8,>v01=%xmm8
vpsrlq $32, % xmm8, % xmm8

# qhasm: v11 = r6 & mask1
# asm 1: vpand <mask1=reg128#2,<r6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<r6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>r6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>r6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = r3 & mask0
# asm 1: vpand <mask0=reg128#1,<r3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<r3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: 2x v10 = r7 << 32
# asm 1: vpsllq $32,<r7=reg128#14,>v10=reg128#16
# asm 2: vpsllq $32,<r7=%xmm13,>v10=%xmm15
vpsllq $32, % xmm13, % xmm15

# qhasm: 2x v01 = r3 unsigned>> 32
# asm 1: vpsrlq $32,<r3=reg128#10,>v01=reg128#10
# asm 2: vpsrlq $32,<r3=%xmm9,>v01=%xmm9
vpsrlq $32, % xmm9, % xmm9

# qhasm: v11 = r7 & mask1
# asm 1: vpand <mask1=reg128#2,<r7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<r7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: r3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>r7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>r7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = r0 & mask2
# asm 1: vpand <mask2=reg128#3,<r0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<r0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: 4x v10 = r2 << 16
# asm 1: vpslld $16,<r2=reg128#12,>v10=reg128#16
# asm 2: vpslld $16,<r2=%xmm11,>v10=%xmm15
vpslld $16, % xmm11, % xmm15

# qhasm: 4x v01 = r0 unsigned>> 16
# asm 1: vpsrld $16,<r0=reg128#15,>v01=reg128#15
# asm 2: vpsrld $16,<r0=%xmm14,>v01=%xmm14
vpsrld $16, % xmm14, % xmm14

# qhasm: v11 = r2 & mask3
# asm 1: vpand <mask3=reg128#4,<r2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<r2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>r0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>r0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: r2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>r2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>r2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = r1 & mask2
# asm 1: vpand <mask2=reg128#3,<r1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<r1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: 4x v10 = r3 << 16
# asm 1: vpslld $16,<r3=reg128#13,>v10=reg128#16
# asm 2: vpslld $16,<r3=%xmm12,>v10=%xmm15
vpslld $16, % xmm12, % xmm15

# qhasm: 4x v01 = r1 unsigned>> 16
# asm 1: vpsrld $16,<r1=reg128#11,>v01=reg128#11
# asm 2: vpsrld $16,<r1=%xmm10,>v01=%xmm10
vpsrld $16, % xmm10, % xmm10

# qhasm: v11 = r3 & mask3
# asm 1: vpand <mask3=reg128#4,<r3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<r3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>r3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>r3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = r4 & mask2
# asm 1: vpand <mask2=reg128#3,<r4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<r4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: 4x v10 = r6 << 16
# asm 1: vpslld $16,<r6=reg128#9,>v10=reg128#16
# asm 2: vpslld $16,<r6=%xmm8,>v10=%xmm15
vpslld $16, % xmm8, % xmm15

# qhasm: 4x v01 = r4 unsigned>> 16
# asm 1: vpsrld $16,<r4=reg128#7,>v01=reg128#7
# asm 2: vpsrld $16,<r4=%xmm6,>v01=%xmm6
vpsrld $16, % xmm6, % xmm6

# qhasm: v11 = r6 & mask3
# asm 1: vpand <mask3=reg128#4,<r6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<r6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>r6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>r6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = r5 & mask2
# asm 1: vpand <mask2=reg128#3,<r5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<r5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: 4x v10 = r7 << 16
# asm 1: vpslld $16,<r7=reg128#10,>v10=reg128#16
# asm 2: vpslld $16,<r7=%xmm9,>v10=%xmm15
vpslld $16, % xmm9, % xmm15

# qhasm: 4x v01 = r5 unsigned>> 16
# asm 1: vpsrld $16,<r5=reg128#8,>v01=reg128#8
# asm 2: vpsrld $16,<r5=%xmm7,>v01=%xmm7
vpsrld $16, % xmm7, % xmm7

# qhasm: v11 = r7 & mask3
# asm 1: vpand <mask3=reg128#4,<r7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<r7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: r5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>r5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>r5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>r7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>r7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = r0 & mask4
# asm 1: vpand <mask4=reg128#5,<r0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<r0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: 8x v10 = r1 << 8
# asm 1: vpsllw $8,<r1=reg128#15,>v10=reg128#16
# asm 2: vpsllw $8,<r1=%xmm14,>v10=%xmm15
vpsllw $8, % xmm14, % xmm15

# qhasm: 8x v01 = r0 unsigned>> 8
# asm 1: vpsrlw $8,<r0=reg128#14,>v01=reg128#14
# asm 2: vpsrlw $8,<r0=%xmm13,>v01=%xmm13
vpsrlw $8, % xmm13, % xmm13

# qhasm: v11 = r1 & mask5
# asm 1: vpand <mask5=reg128#6,<r1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<r1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>r0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>r0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: r1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>r1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>r1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = r2 & mask4
# asm 1: vpand <mask4=reg128#5,<r2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<r2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: 8x v10 = r3 << 8
# asm 1: vpsllw $8,<r3=reg128#11,>v10=reg128#16
# asm 2: vpsllw $8,<r3=%xmm10,>v10=%xmm15
vpsllw $8, % xmm10, % xmm15

# qhasm: 8x v01 = r2 unsigned>> 8
# asm 1: vpsrlw $8,<r2=reg128#12,>v01=reg128#12
# asm 2: vpsrlw $8,<r2=%xmm11,>v01=%xmm11
vpsrlw $8, % xmm11, % xmm11

# qhasm: v11 = r3 & mask5
# asm 1: vpand <mask5=reg128#6,<r3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<r3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>r3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>r3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = r4 & mask4
# asm 1: vpand <mask4=reg128#5,<r4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<r4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: 8x v10 = r5 << 8
# asm 1: vpsllw $8,<r5=reg128#9,>v10=reg128#16
# asm 2: vpsllw $8,<r5=%xmm8,>v10=%xmm15
vpsllw $8, % xmm8, % xmm15

# qhasm: 8x v01 = r4 unsigned>> 8
# asm 1: vpsrlw $8,<r4=reg128#13,>v01=reg128#13
# asm 2: vpsrlw $8,<r4=%xmm12,>v01=%xmm12
vpsrlw $8, % xmm12, % xmm12

# qhasm: v11 = r5 & mask5
# asm 1: vpand <mask5=reg128#6,<r5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<r5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>r5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>r5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = r6 & mask4
# asm 1: vpand <mask4=reg128#5,<r6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<r6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: 8x v10 = r7 << 8
# asm 1: vpsllw $8,<r7=reg128#8,>v10=reg128#16
# asm 2: vpsllw $8,<r7=%xmm7,>v10=%xmm15
vpsllw $8, % xmm7, % xmm15

# qhasm: 8x v01 = r6 unsigned>> 8
# asm 1: vpsrlw $8,<r6=reg128#7,>v01=reg128#7
# asm 2: vpsrlw $8,<r6=%xmm6,>v01=%xmm6
vpsrlw $8, % xmm6, % xmm6

# qhasm: v11 = r7 & mask5
# asm 1: vpand <mask5=reg128#6,<r7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<r7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: r6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>r7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>r7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: buf = r0[0]
# asm 1: pextrq $0x0,<r0=reg128#10,>buf=int64#2
# asm 2: pextrq $0x0,<r0=%xmm9,>buf=%rsi
pextrq $0x0, % xmm9, % rsi

# qhasm: mem64[ input_0 + 16 ] = buf
# asm 1: movq   <buf=int64#2,16(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,16(<input_0=%rdi)
movq   % rsi, 16( % rdi)

# qhasm: buf = r1[0]
# asm 1: pextrq $0x0,<r1=reg128#14,>buf=int64#2
# asm 2: pextrq $0x0,<r1=%xmm13,>buf=%rsi
pextrq $0x0, % xmm13, % rsi

# qhasm: mem64[ input_0 + 80 ] = buf
# asm 1: movq   <buf=int64#2,80(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,80(<input_0=%rdi)
movq   % rsi, 80( % rdi)

# qhasm: buf = r2[0]
# asm 1: pextrq $0x0,<r2=reg128#15,>buf=int64#2
# asm 2: pextrq $0x0,<r2=%xmm14,>buf=%rsi
pextrq $0x0, % xmm14, % rsi

# qhasm: mem64[ input_0 + 144 ] = buf
# asm 1: movq   <buf=int64#2,144(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,144(<input_0=%rdi)
movq   % rsi, 144( % rdi)

# qhasm: buf = r3[0]
# asm 1: pextrq $0x0,<r3=reg128#11,>buf=int64#2
# asm 2: pextrq $0x0,<r3=%xmm10,>buf=%rsi
pextrq $0x0, % xmm10, % rsi

# qhasm: mem64[ input_0 + 208 ] = buf
# asm 1: movq   <buf=int64#2,208(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,208(<input_0=%rdi)
movq   % rsi, 208( % rdi)

# qhasm: buf = r4[0]
# asm 1: pextrq $0x0,<r4=reg128#12,>buf=int64#2
# asm 2: pextrq $0x0,<r4=%xmm11,>buf=%rsi
pextrq $0x0, % xmm11, % rsi

# qhasm: mem64[ input_0 + 272 ] = buf
# asm 1: movq   <buf=int64#2,272(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,272(<input_0=%rdi)
movq   % rsi, 272( % rdi)

# qhasm: buf = r5[0]
# asm 1: pextrq $0x0,<r5=reg128#9,>buf=int64#2
# asm 2: pextrq $0x0,<r5=%xmm8,>buf=%rsi
pextrq $0x0, % xmm8, % rsi

# qhasm: mem64[ input_0 + 336 ] = buf
# asm 1: movq   <buf=int64#2,336(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,336(<input_0=%rdi)
movq   % rsi, 336( % rdi)

# qhasm: buf = r6[0]
# asm 1: pextrq $0x0,<r6=reg128#13,>buf=int64#2
# asm 2: pextrq $0x0,<r6=%xmm12,>buf=%rsi
pextrq $0x0, % xmm12, % rsi

# qhasm: mem64[ input_0 + 400 ] = buf
# asm 1: movq   <buf=int64#2,400(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,400(<input_0=%rdi)
movq   % rsi, 400( % rdi)

# qhasm: buf = r7[0]
# asm 1: pextrq $0x0,<r7=reg128#7,>buf=int64#2
# asm 2: pextrq $0x0,<r7=%xmm6,>buf=%rsi
pextrq $0x0, % xmm6, % rsi

# qhasm: mem64[ input_0 + 464 ] = buf
# asm 1: movq   <buf=int64#2,464(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,464(<input_0=%rdi)
movq   % rsi, 464( % rdi)

# qhasm: r0 = mem64[ input_0 + 24 ] x2
# asm 1: movddup 24(<input_0=int64#1),>r0=reg128#7
# asm 2: movddup 24(<input_0=%rdi),>r0=%xmm6
movddup 24( % rdi), % xmm6

# qhasm: r1 = mem64[ input_0 + 88 ] x2
# asm 1: movddup 88(<input_0=int64#1),>r1=reg128#8
# asm 2: movddup 88(<input_0=%rdi),>r1=%xmm7
movddup 88( % rdi), % xmm7

# qhasm: r2 = mem64[ input_0 + 152 ] x2
# asm 1: movddup 152(<input_0=int64#1),>r2=reg128#9
# asm 2: movddup 152(<input_0=%rdi),>r2=%xmm8
movddup 152( % rdi), % xmm8

# qhasm: r3 = mem64[ input_0 + 216 ] x2
# asm 1: movddup 216(<input_0=int64#1),>r3=reg128#10
# asm 2: movddup 216(<input_0=%rdi),>r3=%xmm9
movddup 216( % rdi), % xmm9

# qhasm: r4 = mem64[ input_0 + 280 ] x2
# asm 1: movddup 280(<input_0=int64#1),>r4=reg128#11
# asm 2: movddup 280(<input_0=%rdi),>r4=%xmm10
movddup 280( % rdi), % xmm10

# qhasm: r5 = mem64[ input_0 + 344 ] x2
# asm 1: movddup 344(<input_0=int64#1),>r5=reg128#12
# asm 2: movddup 344(<input_0=%rdi),>r5=%xmm11
movddup 344( % rdi), % xmm11

# qhasm: r6 = mem64[ input_0 + 408 ] x2
# asm 1: movddup 408(<input_0=int64#1),>r6=reg128#13
# asm 2: movddup 408(<input_0=%rdi),>r6=%xmm12
movddup 408( % rdi), % xmm12

# qhasm: r7 = mem64[ input_0 + 472 ] x2
# asm 1: movddup 472(<input_0=int64#1),>r7=reg128#14
# asm 2: movddup 472(<input_0=%rdi),>r7=%xmm13
movddup 472( % rdi), % xmm13

# qhasm: v00 = r0 & mask0
# asm 1: vpand <mask0=reg128#1,<r0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<r0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: 2x v10 = r4 << 32
# asm 1: vpsllq $32,<r4=reg128#11,>v10=reg128#16
# asm 2: vpsllq $32,<r4=%xmm10,>v10=%xmm15
vpsllq $32, % xmm10, % xmm15

# qhasm: 2x v01 = r0 unsigned>> 32
# asm 1: vpsrlq $32,<r0=reg128#7,>v01=reg128#7
# asm 2: vpsrlq $32,<r0=%xmm6,>v01=%xmm6
vpsrlq $32, % xmm6, % xmm6

# qhasm: v11 = r4 & mask1
# asm 1: vpand <mask1=reg128#2,<r4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<r4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>r4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>r4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = r1 & mask0
# asm 1: vpand <mask0=reg128#1,<r1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<r1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: 2x v10 = r5 << 32
# asm 1: vpsllq $32,<r5=reg128#12,>v10=reg128#16
# asm 2: vpsllq $32,<r5=%xmm11,>v10=%xmm15
vpsllq $32, % xmm11, % xmm15

# qhasm: 2x v01 = r1 unsigned>> 32
# asm 1: vpsrlq $32,<r1=reg128#8,>v01=reg128#8
# asm 2: vpsrlq $32,<r1=%xmm7,>v01=%xmm7
vpsrlq $32, % xmm7, % xmm7

# qhasm: v11 = r5 & mask1
# asm 1: vpand <mask1=reg128#2,<r5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<r5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>r1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>r1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>r5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>r5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = r2 & mask0
# asm 1: vpand <mask0=reg128#1,<r2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<r2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: 2x v10 = r6 << 32
# asm 1: vpsllq $32,<r6=reg128#13,>v10=reg128#16
# asm 2: vpsllq $32,<r6=%xmm12,>v10=%xmm15
vpsllq $32, % xmm12, % xmm15

# qhasm: 2x v01 = r2 unsigned>> 32
# asm 1: vpsrlq $32,<r2=reg128#9,>v01=reg128#9
# asm 2: vpsrlq $32,<r2=%xmm8,>v01=%xmm8
vpsrlq $32, % xmm8, % xmm8

# qhasm: v11 = r6 & mask1
# asm 1: vpand <mask1=reg128#2,<r6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<r6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>r6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>r6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = r3 & mask0
# asm 1: vpand <mask0=reg128#1,<r3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<r3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: 2x v10 = r7 << 32
# asm 1: vpsllq $32,<r7=reg128#14,>v10=reg128#16
# asm 2: vpsllq $32,<r7=%xmm13,>v10=%xmm15
vpsllq $32, % xmm13, % xmm15

# qhasm: 2x v01 = r3 unsigned>> 32
# asm 1: vpsrlq $32,<r3=reg128#10,>v01=reg128#10
# asm 2: vpsrlq $32,<r3=%xmm9,>v01=%xmm9
vpsrlq $32, % xmm9, % xmm9

# qhasm: v11 = r7 & mask1
# asm 1: vpand <mask1=reg128#2,<r7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<r7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: r3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>r7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>r7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = r0 & mask2
# asm 1: vpand <mask2=reg128#3,<r0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<r0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: 4x v10 = r2 << 16
# asm 1: vpslld $16,<r2=reg128#12,>v10=reg128#16
# asm 2: vpslld $16,<r2=%xmm11,>v10=%xmm15
vpslld $16, % xmm11, % xmm15

# qhasm: 4x v01 = r0 unsigned>> 16
# asm 1: vpsrld $16,<r0=reg128#15,>v01=reg128#15
# asm 2: vpsrld $16,<r0=%xmm14,>v01=%xmm14
vpsrld $16, % xmm14, % xmm14

# qhasm: v11 = r2 & mask3
# asm 1: vpand <mask3=reg128#4,<r2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<r2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>r0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>r0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: r2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>r2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>r2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = r1 & mask2
# asm 1: vpand <mask2=reg128#3,<r1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<r1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: 4x v10 = r3 << 16
# asm 1: vpslld $16,<r3=reg128#13,>v10=reg128#16
# asm 2: vpslld $16,<r3=%xmm12,>v10=%xmm15
vpslld $16, % xmm12, % xmm15

# qhasm: 4x v01 = r1 unsigned>> 16
# asm 1: vpsrld $16,<r1=reg128#11,>v01=reg128#11
# asm 2: vpsrld $16,<r1=%xmm10,>v01=%xmm10
vpsrld $16, % xmm10, % xmm10

# qhasm: v11 = r3 & mask3
# asm 1: vpand <mask3=reg128#4,<r3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<r3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>r3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>r3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = r4 & mask2
# asm 1: vpand <mask2=reg128#3,<r4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<r4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: 4x v10 = r6 << 16
# asm 1: vpslld $16,<r6=reg128#9,>v10=reg128#16
# asm 2: vpslld $16,<r6=%xmm8,>v10=%xmm15
vpslld $16, % xmm8, % xmm15

# qhasm: 4x v01 = r4 unsigned>> 16
# asm 1: vpsrld $16,<r4=reg128#7,>v01=reg128#7
# asm 2: vpsrld $16,<r4=%xmm6,>v01=%xmm6
vpsrld $16, % xmm6, % xmm6

# qhasm: v11 = r6 & mask3
# asm 1: vpand <mask3=reg128#4,<r6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<r6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>r6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>r6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = r5 & mask2
# asm 1: vpand <mask2=reg128#3,<r5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<r5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: 4x v10 = r7 << 16
# asm 1: vpslld $16,<r7=reg128#10,>v10=reg128#16
# asm 2: vpslld $16,<r7=%xmm9,>v10=%xmm15
vpslld $16, % xmm9, % xmm15

# qhasm: 4x v01 = r5 unsigned>> 16
# asm 1: vpsrld $16,<r5=reg128#8,>v01=reg128#8
# asm 2: vpsrld $16,<r5=%xmm7,>v01=%xmm7
vpsrld $16, % xmm7, % xmm7

# qhasm: v11 = r7 & mask3
# asm 1: vpand <mask3=reg128#4,<r7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<r7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: r5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>r5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>r5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>r7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>r7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = r0 & mask4
# asm 1: vpand <mask4=reg128#5,<r0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<r0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: 8x v10 = r1 << 8
# asm 1: vpsllw $8,<r1=reg128#15,>v10=reg128#16
# asm 2: vpsllw $8,<r1=%xmm14,>v10=%xmm15
vpsllw $8, % xmm14, % xmm15

# qhasm: 8x v01 = r0 unsigned>> 8
# asm 1: vpsrlw $8,<r0=reg128#14,>v01=reg128#14
# asm 2: vpsrlw $8,<r0=%xmm13,>v01=%xmm13
vpsrlw $8, % xmm13, % xmm13

# qhasm: v11 = r1 & mask5
# asm 1: vpand <mask5=reg128#6,<r1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<r1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>r0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>r0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: r1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>r1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>r1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = r2 & mask4
# asm 1: vpand <mask4=reg128#5,<r2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<r2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: 8x v10 = r3 << 8
# asm 1: vpsllw $8,<r3=reg128#11,>v10=reg128#16
# asm 2: vpsllw $8,<r3=%xmm10,>v10=%xmm15
vpsllw $8, % xmm10, % xmm15

# qhasm: 8x v01 = r2 unsigned>> 8
# asm 1: vpsrlw $8,<r2=reg128#12,>v01=reg128#12
# asm 2: vpsrlw $8,<r2=%xmm11,>v01=%xmm11
vpsrlw $8, % xmm11, % xmm11

# qhasm: v11 = r3 & mask5
# asm 1: vpand <mask5=reg128#6,<r3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<r3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>r3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>r3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = r4 & mask4
# asm 1: vpand <mask4=reg128#5,<r4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<r4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: 8x v10 = r5 << 8
# asm 1: vpsllw $8,<r5=reg128#9,>v10=reg128#16
# asm 2: vpsllw $8,<r5=%xmm8,>v10=%xmm15
vpsllw $8, % xmm8, % xmm15

# qhasm: 8x v01 = r4 unsigned>> 8
# asm 1: vpsrlw $8,<r4=reg128#13,>v01=reg128#13
# asm 2: vpsrlw $8,<r4=%xmm12,>v01=%xmm12
vpsrlw $8, % xmm12, % xmm12

# qhasm: v11 = r5 & mask5
# asm 1: vpand <mask5=reg128#6,<r5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<r5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>r5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>r5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = r6 & mask4
# asm 1: vpand <mask4=reg128#5,<r6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<r6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: 8x v10 = r7 << 8
# asm 1: vpsllw $8,<r7=reg128#8,>v10=reg128#16
# asm 2: vpsllw $8,<r7=%xmm7,>v10=%xmm15
vpsllw $8, % xmm7, % xmm15

# qhasm: 8x v01 = r6 unsigned>> 8
# asm 1: vpsrlw $8,<r6=reg128#7,>v01=reg128#7
# asm 2: vpsrlw $8,<r6=%xmm6,>v01=%xmm6
vpsrlw $8, % xmm6, % xmm6

# qhasm: v11 = r7 & mask5
# asm 1: vpand <mask5=reg128#6,<r7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<r7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: r6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>r7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>r7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: buf = r0[0]
# asm 1: pextrq $0x0,<r0=reg128#10,>buf=int64#2
# asm 2: pextrq $0x0,<r0=%xmm9,>buf=%rsi
pextrq $0x0, % xmm9, % rsi

# qhasm: mem64[ input_0 + 24 ] = buf
# asm 1: movq   <buf=int64#2,24(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,24(<input_0=%rdi)
movq   % rsi, 24( % rdi)

# qhasm: buf = r1[0]
# asm 1: pextrq $0x0,<r1=reg128#14,>buf=int64#2
# asm 2: pextrq $0x0,<r1=%xmm13,>buf=%rsi
pextrq $0x0, % xmm13, % rsi

# qhasm: mem64[ input_0 + 88 ] = buf
# asm 1: movq   <buf=int64#2,88(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,88(<input_0=%rdi)
movq   % rsi, 88( % rdi)

# qhasm: buf = r2[0]
# asm 1: pextrq $0x0,<r2=reg128#15,>buf=int64#2
# asm 2: pextrq $0x0,<r2=%xmm14,>buf=%rsi
pextrq $0x0, % xmm14, % rsi

# qhasm: mem64[ input_0 + 152 ] = buf
# asm 1: movq   <buf=int64#2,152(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,152(<input_0=%rdi)
movq   % rsi, 152( % rdi)

# qhasm: buf = r3[0]
# asm 1: pextrq $0x0,<r3=reg128#11,>buf=int64#2
# asm 2: pextrq $0x0,<r3=%xmm10,>buf=%rsi
pextrq $0x0, % xmm10, % rsi

# qhasm: mem64[ input_0 + 216 ] = buf
# asm 1: movq   <buf=int64#2,216(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,216(<input_0=%rdi)
movq   % rsi, 216( % rdi)

# qhasm: buf = r4[0]
# asm 1: pextrq $0x0,<r4=reg128#12,>buf=int64#2
# asm 2: pextrq $0x0,<r4=%xmm11,>buf=%rsi
pextrq $0x0, % xmm11, % rsi

# qhasm: mem64[ input_0 + 280 ] = buf
# asm 1: movq   <buf=int64#2,280(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,280(<input_0=%rdi)
movq   % rsi, 280( % rdi)

# qhasm: buf = r5[0]
# asm 1: pextrq $0x0,<r5=reg128#9,>buf=int64#2
# asm 2: pextrq $0x0,<r5=%xmm8,>buf=%rsi
pextrq $0x0, % xmm8, % rsi

# qhasm: mem64[ input_0 + 344 ] = buf
# asm 1: movq   <buf=int64#2,344(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,344(<input_0=%rdi)
movq   % rsi, 344( % rdi)

# qhasm: buf = r6[0]
# asm 1: pextrq $0x0,<r6=reg128#13,>buf=int64#2
# asm 2: pextrq $0x0,<r6=%xmm12,>buf=%rsi
pextrq $0x0, % xmm12, % rsi

# qhasm: mem64[ input_0 + 408 ] = buf
# asm 1: movq   <buf=int64#2,408(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,408(<input_0=%rdi)
movq   % rsi, 408( % rdi)

# qhasm: buf = r7[0]
# asm 1: pextrq $0x0,<r7=reg128#7,>buf=int64#2
# asm 2: pextrq $0x0,<r7=%xmm6,>buf=%rsi
pextrq $0x0, % xmm6, % rsi

# qhasm: mem64[ input_0 + 472 ] = buf
# asm 1: movq   <buf=int64#2,472(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,472(<input_0=%rdi)
movq   % rsi, 472( % rdi)

# qhasm: r0 = mem64[ input_0 + 32 ] x2
# asm 1: movddup 32(<input_0=int64#1),>r0=reg128#7
# asm 2: movddup 32(<input_0=%rdi),>r0=%xmm6
movddup 32( % rdi), % xmm6

# qhasm: r1 = mem64[ input_0 + 96 ] x2
# asm 1: movddup 96(<input_0=int64#1),>r1=reg128#8
# asm 2: movddup 96(<input_0=%rdi),>r1=%xmm7
movddup 96( % rdi), % xmm7

# qhasm: r2 = mem64[ input_0 + 160 ] x2
# asm 1: movddup 160(<input_0=int64#1),>r2=reg128#9
# asm 2: movddup 160(<input_0=%rdi),>r2=%xmm8
movddup 160( % rdi), % xmm8

# qhasm: r3 = mem64[ input_0 + 224 ] x2
# asm 1: movddup 224(<input_0=int64#1),>r3=reg128#10
# asm 2: movddup 224(<input_0=%rdi),>r3=%xmm9
movddup 224( % rdi), % xmm9

# qhasm: r4 = mem64[ input_0 + 288 ] x2
# asm 1: movddup 288(<input_0=int64#1),>r4=reg128#11
# asm 2: movddup 288(<input_0=%rdi),>r4=%xmm10
movddup 288( % rdi), % xmm10

# qhasm: r5 = mem64[ input_0 + 352 ] x2
# asm 1: movddup 352(<input_0=int64#1),>r5=reg128#12
# asm 2: movddup 352(<input_0=%rdi),>r5=%xmm11
movddup 352( % rdi), % xmm11

# qhasm: r6 = mem64[ input_0 + 416 ] x2
# asm 1: movddup 416(<input_0=int64#1),>r6=reg128#13
# asm 2: movddup 416(<input_0=%rdi),>r6=%xmm12
movddup 416( % rdi), % xmm12

# qhasm: r7 = mem64[ input_0 + 480 ] x2
# asm 1: movddup 480(<input_0=int64#1),>r7=reg128#14
# asm 2: movddup 480(<input_0=%rdi),>r7=%xmm13
movddup 480( % rdi), % xmm13

# qhasm: v00 = r0 & mask0
# asm 1: vpand <mask0=reg128#1,<r0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<r0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: 2x v10 = r4 << 32
# asm 1: vpsllq $32,<r4=reg128#11,>v10=reg128#16
# asm 2: vpsllq $32,<r4=%xmm10,>v10=%xmm15
vpsllq $32, % xmm10, % xmm15

# qhasm: 2x v01 = r0 unsigned>> 32
# asm 1: vpsrlq $32,<r0=reg128#7,>v01=reg128#7
# asm 2: vpsrlq $32,<r0=%xmm6,>v01=%xmm6
vpsrlq $32, % xmm6, % xmm6

# qhasm: v11 = r4 & mask1
# asm 1: vpand <mask1=reg128#2,<r4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<r4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>r4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>r4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = r1 & mask0
# asm 1: vpand <mask0=reg128#1,<r1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<r1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: 2x v10 = r5 << 32
# asm 1: vpsllq $32,<r5=reg128#12,>v10=reg128#16
# asm 2: vpsllq $32,<r5=%xmm11,>v10=%xmm15
vpsllq $32, % xmm11, % xmm15

# qhasm: 2x v01 = r1 unsigned>> 32
# asm 1: vpsrlq $32,<r1=reg128#8,>v01=reg128#8
# asm 2: vpsrlq $32,<r1=%xmm7,>v01=%xmm7
vpsrlq $32, % xmm7, % xmm7

# qhasm: v11 = r5 & mask1
# asm 1: vpand <mask1=reg128#2,<r5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<r5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>r1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>r1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>r5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>r5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = r2 & mask0
# asm 1: vpand <mask0=reg128#1,<r2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<r2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: 2x v10 = r6 << 32
# asm 1: vpsllq $32,<r6=reg128#13,>v10=reg128#16
# asm 2: vpsllq $32,<r6=%xmm12,>v10=%xmm15
vpsllq $32, % xmm12, % xmm15

# qhasm: 2x v01 = r2 unsigned>> 32
# asm 1: vpsrlq $32,<r2=reg128#9,>v01=reg128#9
# asm 2: vpsrlq $32,<r2=%xmm8,>v01=%xmm8
vpsrlq $32, % xmm8, % xmm8

# qhasm: v11 = r6 & mask1
# asm 1: vpand <mask1=reg128#2,<r6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<r6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>r6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>r6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = r3 & mask0
# asm 1: vpand <mask0=reg128#1,<r3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<r3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: 2x v10 = r7 << 32
# asm 1: vpsllq $32,<r7=reg128#14,>v10=reg128#16
# asm 2: vpsllq $32,<r7=%xmm13,>v10=%xmm15
vpsllq $32, % xmm13, % xmm15

# qhasm: 2x v01 = r3 unsigned>> 32
# asm 1: vpsrlq $32,<r3=reg128#10,>v01=reg128#10
# asm 2: vpsrlq $32,<r3=%xmm9,>v01=%xmm9
vpsrlq $32, % xmm9, % xmm9

# qhasm: v11 = r7 & mask1
# asm 1: vpand <mask1=reg128#2,<r7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<r7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: r3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>r7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>r7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = r0 & mask2
# asm 1: vpand <mask2=reg128#3,<r0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<r0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: 4x v10 = r2 << 16
# asm 1: vpslld $16,<r2=reg128#12,>v10=reg128#16
# asm 2: vpslld $16,<r2=%xmm11,>v10=%xmm15
vpslld $16, % xmm11, % xmm15

# qhasm: 4x v01 = r0 unsigned>> 16
# asm 1: vpsrld $16,<r0=reg128#15,>v01=reg128#15
# asm 2: vpsrld $16,<r0=%xmm14,>v01=%xmm14
vpsrld $16, % xmm14, % xmm14

# qhasm: v11 = r2 & mask3
# asm 1: vpand <mask3=reg128#4,<r2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<r2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>r0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>r0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: r2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>r2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>r2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = r1 & mask2
# asm 1: vpand <mask2=reg128#3,<r1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<r1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: 4x v10 = r3 << 16
# asm 1: vpslld $16,<r3=reg128#13,>v10=reg128#16
# asm 2: vpslld $16,<r3=%xmm12,>v10=%xmm15
vpslld $16, % xmm12, % xmm15

# qhasm: 4x v01 = r1 unsigned>> 16
# asm 1: vpsrld $16,<r1=reg128#11,>v01=reg128#11
# asm 2: vpsrld $16,<r1=%xmm10,>v01=%xmm10
vpsrld $16, % xmm10, % xmm10

# qhasm: v11 = r3 & mask3
# asm 1: vpand <mask3=reg128#4,<r3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<r3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>r3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>r3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = r4 & mask2
# asm 1: vpand <mask2=reg128#3,<r4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<r4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: 4x v10 = r6 << 16
# asm 1: vpslld $16,<r6=reg128#9,>v10=reg128#16
# asm 2: vpslld $16,<r6=%xmm8,>v10=%xmm15
vpslld $16, % xmm8, % xmm15

# qhasm: 4x v01 = r4 unsigned>> 16
# asm 1: vpsrld $16,<r4=reg128#7,>v01=reg128#7
# asm 2: vpsrld $16,<r4=%xmm6,>v01=%xmm6
vpsrld $16, % xmm6, % xmm6

# qhasm: v11 = r6 & mask3
# asm 1: vpand <mask3=reg128#4,<r6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<r6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>r6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>r6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = r5 & mask2
# asm 1: vpand <mask2=reg128#3,<r5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<r5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: 4x v10 = r7 << 16
# asm 1: vpslld $16,<r7=reg128#10,>v10=reg128#16
# asm 2: vpslld $16,<r7=%xmm9,>v10=%xmm15
vpslld $16, % xmm9, % xmm15

# qhasm: 4x v01 = r5 unsigned>> 16
# asm 1: vpsrld $16,<r5=reg128#8,>v01=reg128#8
# asm 2: vpsrld $16,<r5=%xmm7,>v01=%xmm7
vpsrld $16, % xmm7, % xmm7

# qhasm: v11 = r7 & mask3
# asm 1: vpand <mask3=reg128#4,<r7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<r7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: r5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>r5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>r5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>r7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>r7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = r0 & mask4
# asm 1: vpand <mask4=reg128#5,<r0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<r0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: 8x v10 = r1 << 8
# asm 1: vpsllw $8,<r1=reg128#15,>v10=reg128#16
# asm 2: vpsllw $8,<r1=%xmm14,>v10=%xmm15
vpsllw $8, % xmm14, % xmm15

# qhasm: 8x v01 = r0 unsigned>> 8
# asm 1: vpsrlw $8,<r0=reg128#14,>v01=reg128#14
# asm 2: vpsrlw $8,<r0=%xmm13,>v01=%xmm13
vpsrlw $8, % xmm13, % xmm13

# qhasm: v11 = r1 & mask5
# asm 1: vpand <mask5=reg128#6,<r1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<r1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>r0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>r0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: r1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>r1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>r1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = r2 & mask4
# asm 1: vpand <mask4=reg128#5,<r2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<r2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: 8x v10 = r3 << 8
# asm 1: vpsllw $8,<r3=reg128#11,>v10=reg128#16
# asm 2: vpsllw $8,<r3=%xmm10,>v10=%xmm15
vpsllw $8, % xmm10, % xmm15

# qhasm: 8x v01 = r2 unsigned>> 8
# asm 1: vpsrlw $8,<r2=reg128#12,>v01=reg128#12
# asm 2: vpsrlw $8,<r2=%xmm11,>v01=%xmm11
vpsrlw $8, % xmm11, % xmm11

# qhasm: v11 = r3 & mask5
# asm 1: vpand <mask5=reg128#6,<r3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<r3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>r3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>r3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = r4 & mask4
# asm 1: vpand <mask4=reg128#5,<r4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<r4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: 8x v10 = r5 << 8
# asm 1: vpsllw $8,<r5=reg128#9,>v10=reg128#16
# asm 2: vpsllw $8,<r5=%xmm8,>v10=%xmm15
vpsllw $8, % xmm8, % xmm15

# qhasm: 8x v01 = r4 unsigned>> 8
# asm 1: vpsrlw $8,<r4=reg128#13,>v01=reg128#13
# asm 2: vpsrlw $8,<r4=%xmm12,>v01=%xmm12
vpsrlw $8, % xmm12, % xmm12

# qhasm: v11 = r5 & mask5
# asm 1: vpand <mask5=reg128#6,<r5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<r5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>r5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>r5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = r6 & mask4
# asm 1: vpand <mask4=reg128#5,<r6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<r6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: 8x v10 = r7 << 8
# asm 1: vpsllw $8,<r7=reg128#8,>v10=reg128#16
# asm 2: vpsllw $8,<r7=%xmm7,>v10=%xmm15
vpsllw $8, % xmm7, % xmm15

# qhasm: 8x v01 = r6 unsigned>> 8
# asm 1: vpsrlw $8,<r6=reg128#7,>v01=reg128#7
# asm 2: vpsrlw $8,<r6=%xmm6,>v01=%xmm6
vpsrlw $8, % xmm6, % xmm6

# qhasm: v11 = r7 & mask5
# asm 1: vpand <mask5=reg128#6,<r7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<r7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: r6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>r7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>r7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: buf = r0[0]
# asm 1: pextrq $0x0,<r0=reg128#10,>buf=int64#2
# asm 2: pextrq $0x0,<r0=%xmm9,>buf=%rsi
pextrq $0x0, % xmm9, % rsi

# qhasm: mem64[ input_0 + 32 ] = buf
# asm 1: movq   <buf=int64#2,32(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,32(<input_0=%rdi)
movq   % rsi, 32( % rdi)

# qhasm: buf = r1[0]
# asm 1: pextrq $0x0,<r1=reg128#14,>buf=int64#2
# asm 2: pextrq $0x0,<r1=%xmm13,>buf=%rsi
pextrq $0x0, % xmm13, % rsi

# qhasm: mem64[ input_0 + 96 ] = buf
# asm 1: movq   <buf=int64#2,96(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,96(<input_0=%rdi)
movq   % rsi, 96( % rdi)

# qhasm: buf = r2[0]
# asm 1: pextrq $0x0,<r2=reg128#15,>buf=int64#2
# asm 2: pextrq $0x0,<r2=%xmm14,>buf=%rsi
pextrq $0x0, % xmm14, % rsi

# qhasm: mem64[ input_0 + 160 ] = buf
# asm 1: movq   <buf=int64#2,160(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,160(<input_0=%rdi)
movq   % rsi, 160( % rdi)

# qhasm: buf = r3[0]
# asm 1: pextrq $0x0,<r3=reg128#11,>buf=int64#2
# asm 2: pextrq $0x0,<r3=%xmm10,>buf=%rsi
pextrq $0x0, % xmm10, % rsi

# qhasm: mem64[ input_0 + 224 ] = buf
# asm 1: movq   <buf=int64#2,224(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,224(<input_0=%rdi)
movq   % rsi, 224( % rdi)

# qhasm: buf = r4[0]
# asm 1: pextrq $0x0,<r4=reg128#12,>buf=int64#2
# asm 2: pextrq $0x0,<r4=%xmm11,>buf=%rsi
pextrq $0x0, % xmm11, % rsi

# qhasm: mem64[ input_0 + 288 ] = buf
# asm 1: movq   <buf=int64#2,288(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,288(<input_0=%rdi)
movq   % rsi, 288( % rdi)

# qhasm: buf = r5[0]
# asm 1: pextrq $0x0,<r5=reg128#9,>buf=int64#2
# asm 2: pextrq $0x0,<r5=%xmm8,>buf=%rsi
pextrq $0x0, % xmm8, % rsi

# qhasm: mem64[ input_0 + 352 ] = buf
# asm 1: movq   <buf=int64#2,352(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,352(<input_0=%rdi)
movq   % rsi, 352( % rdi)

# qhasm: buf = r6[0]
# asm 1: pextrq $0x0,<r6=reg128#13,>buf=int64#2
# asm 2: pextrq $0x0,<r6=%xmm12,>buf=%rsi
pextrq $0x0, % xmm12, % rsi

# qhasm: mem64[ input_0 + 416 ] = buf
# asm 1: movq   <buf=int64#2,416(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,416(<input_0=%rdi)
movq   % rsi, 416( % rdi)

# qhasm: buf = r7[0]
# asm 1: pextrq $0x0,<r7=reg128#7,>buf=int64#2
# asm 2: pextrq $0x0,<r7=%xmm6,>buf=%rsi
pextrq $0x0, % xmm6, % rsi

# qhasm: mem64[ input_0 + 480 ] = buf
# asm 1: movq   <buf=int64#2,480(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,480(<input_0=%rdi)
movq   % rsi, 480( % rdi)

# qhasm: r0 = mem64[ input_0 + 40 ] x2
# asm 1: movddup 40(<input_0=int64#1),>r0=reg128#7
# asm 2: movddup 40(<input_0=%rdi),>r0=%xmm6
movddup 40( % rdi), % xmm6

# qhasm: r1 = mem64[ input_0 + 104 ] x2
# asm 1: movddup 104(<input_0=int64#1),>r1=reg128#8
# asm 2: movddup 104(<input_0=%rdi),>r1=%xmm7
movddup 104( % rdi), % xmm7

# qhasm: r2 = mem64[ input_0 + 168 ] x2
# asm 1: movddup 168(<input_0=int64#1),>r2=reg128#9
# asm 2: movddup 168(<input_0=%rdi),>r2=%xmm8
movddup 168( % rdi), % xmm8

# qhasm: r3 = mem64[ input_0 + 232 ] x2
# asm 1: movddup 232(<input_0=int64#1),>r3=reg128#10
# asm 2: movddup 232(<input_0=%rdi),>r3=%xmm9
movddup 232( % rdi), % xmm9

# qhasm: r4 = mem64[ input_0 + 296 ] x2
# asm 1: movddup 296(<input_0=int64#1),>r4=reg128#11
# asm 2: movddup 296(<input_0=%rdi),>r4=%xmm10
movddup 296( % rdi), % xmm10

# qhasm: r5 = mem64[ input_0 + 360 ] x2
# asm 1: movddup 360(<input_0=int64#1),>r5=reg128#12
# asm 2: movddup 360(<input_0=%rdi),>r5=%xmm11
movddup 360( % rdi), % xmm11

# qhasm: r6 = mem64[ input_0 + 424 ] x2
# asm 1: movddup 424(<input_0=int64#1),>r6=reg128#13
# asm 2: movddup 424(<input_0=%rdi),>r6=%xmm12
movddup 424( % rdi), % xmm12

# qhasm: r7 = mem64[ input_0 + 488 ] x2
# asm 1: movddup 488(<input_0=int64#1),>r7=reg128#14
# asm 2: movddup 488(<input_0=%rdi),>r7=%xmm13
movddup 488( % rdi), % xmm13

# qhasm: v00 = r0 & mask0
# asm 1: vpand <mask0=reg128#1,<r0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<r0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: 2x v10 = r4 << 32
# asm 1: vpsllq $32,<r4=reg128#11,>v10=reg128#16
# asm 2: vpsllq $32,<r4=%xmm10,>v10=%xmm15
vpsllq $32, % xmm10, % xmm15

# qhasm: 2x v01 = r0 unsigned>> 32
# asm 1: vpsrlq $32,<r0=reg128#7,>v01=reg128#7
# asm 2: vpsrlq $32,<r0=%xmm6,>v01=%xmm6
vpsrlq $32, % xmm6, % xmm6

# qhasm: v11 = r4 & mask1
# asm 1: vpand <mask1=reg128#2,<r4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<r4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>r4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>r4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = r1 & mask0
# asm 1: vpand <mask0=reg128#1,<r1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<r1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: 2x v10 = r5 << 32
# asm 1: vpsllq $32,<r5=reg128#12,>v10=reg128#16
# asm 2: vpsllq $32,<r5=%xmm11,>v10=%xmm15
vpsllq $32, % xmm11, % xmm15

# qhasm: 2x v01 = r1 unsigned>> 32
# asm 1: vpsrlq $32,<r1=reg128#8,>v01=reg128#8
# asm 2: vpsrlq $32,<r1=%xmm7,>v01=%xmm7
vpsrlq $32, % xmm7, % xmm7

# qhasm: v11 = r5 & mask1
# asm 1: vpand <mask1=reg128#2,<r5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<r5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>r1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>r1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>r5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>r5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = r2 & mask0
# asm 1: vpand <mask0=reg128#1,<r2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<r2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: 2x v10 = r6 << 32
# asm 1: vpsllq $32,<r6=reg128#13,>v10=reg128#16
# asm 2: vpsllq $32,<r6=%xmm12,>v10=%xmm15
vpsllq $32, % xmm12, % xmm15

# qhasm: 2x v01 = r2 unsigned>> 32
# asm 1: vpsrlq $32,<r2=reg128#9,>v01=reg128#9
# asm 2: vpsrlq $32,<r2=%xmm8,>v01=%xmm8
vpsrlq $32, % xmm8, % xmm8

# qhasm: v11 = r6 & mask1
# asm 1: vpand <mask1=reg128#2,<r6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<r6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>r6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>r6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = r3 & mask0
# asm 1: vpand <mask0=reg128#1,<r3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<r3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: 2x v10 = r7 << 32
# asm 1: vpsllq $32,<r7=reg128#14,>v10=reg128#16
# asm 2: vpsllq $32,<r7=%xmm13,>v10=%xmm15
vpsllq $32, % xmm13, % xmm15

# qhasm: 2x v01 = r3 unsigned>> 32
# asm 1: vpsrlq $32,<r3=reg128#10,>v01=reg128#10
# asm 2: vpsrlq $32,<r3=%xmm9,>v01=%xmm9
vpsrlq $32, % xmm9, % xmm9

# qhasm: v11 = r7 & mask1
# asm 1: vpand <mask1=reg128#2,<r7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<r7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: r3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>r7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>r7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = r0 & mask2
# asm 1: vpand <mask2=reg128#3,<r0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<r0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: 4x v10 = r2 << 16
# asm 1: vpslld $16,<r2=reg128#12,>v10=reg128#16
# asm 2: vpslld $16,<r2=%xmm11,>v10=%xmm15
vpslld $16, % xmm11, % xmm15

# qhasm: 4x v01 = r0 unsigned>> 16
# asm 1: vpsrld $16,<r0=reg128#15,>v01=reg128#15
# asm 2: vpsrld $16,<r0=%xmm14,>v01=%xmm14
vpsrld $16, % xmm14, % xmm14

# qhasm: v11 = r2 & mask3
# asm 1: vpand <mask3=reg128#4,<r2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<r2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>r0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>r0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: r2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>r2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>r2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = r1 & mask2
# asm 1: vpand <mask2=reg128#3,<r1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<r1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: 4x v10 = r3 << 16
# asm 1: vpslld $16,<r3=reg128#13,>v10=reg128#16
# asm 2: vpslld $16,<r3=%xmm12,>v10=%xmm15
vpslld $16, % xmm12, % xmm15

# qhasm: 4x v01 = r1 unsigned>> 16
# asm 1: vpsrld $16,<r1=reg128#11,>v01=reg128#11
# asm 2: vpsrld $16,<r1=%xmm10,>v01=%xmm10
vpsrld $16, % xmm10, % xmm10

# qhasm: v11 = r3 & mask3
# asm 1: vpand <mask3=reg128#4,<r3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<r3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>r3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>r3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = r4 & mask2
# asm 1: vpand <mask2=reg128#3,<r4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<r4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: 4x v10 = r6 << 16
# asm 1: vpslld $16,<r6=reg128#9,>v10=reg128#16
# asm 2: vpslld $16,<r6=%xmm8,>v10=%xmm15
vpslld $16, % xmm8, % xmm15

# qhasm: 4x v01 = r4 unsigned>> 16
# asm 1: vpsrld $16,<r4=reg128#7,>v01=reg128#7
# asm 2: vpsrld $16,<r4=%xmm6,>v01=%xmm6
vpsrld $16, % xmm6, % xmm6

# qhasm: v11 = r6 & mask3
# asm 1: vpand <mask3=reg128#4,<r6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<r6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>r6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>r6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = r5 & mask2
# asm 1: vpand <mask2=reg128#3,<r5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<r5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: 4x v10 = r7 << 16
# asm 1: vpslld $16,<r7=reg128#10,>v10=reg128#16
# asm 2: vpslld $16,<r7=%xmm9,>v10=%xmm15
vpslld $16, % xmm9, % xmm15

# qhasm: 4x v01 = r5 unsigned>> 16
# asm 1: vpsrld $16,<r5=reg128#8,>v01=reg128#8
# asm 2: vpsrld $16,<r5=%xmm7,>v01=%xmm7
vpsrld $16, % xmm7, % xmm7

# qhasm: v11 = r7 & mask3
# asm 1: vpand <mask3=reg128#4,<r7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<r7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: r5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>r5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>r5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>r7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>r7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = r0 & mask4
# asm 1: vpand <mask4=reg128#5,<r0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<r0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: 8x v10 = r1 << 8
# asm 1: vpsllw $8,<r1=reg128#15,>v10=reg128#16
# asm 2: vpsllw $8,<r1=%xmm14,>v10=%xmm15
vpsllw $8, % xmm14, % xmm15

# qhasm: 8x v01 = r0 unsigned>> 8
# asm 1: vpsrlw $8,<r0=reg128#14,>v01=reg128#14
# asm 2: vpsrlw $8,<r0=%xmm13,>v01=%xmm13
vpsrlw $8, % xmm13, % xmm13

# qhasm: v11 = r1 & mask5
# asm 1: vpand <mask5=reg128#6,<r1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<r1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>r0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>r0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: r1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>r1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>r1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = r2 & mask4
# asm 1: vpand <mask4=reg128#5,<r2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<r2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: 8x v10 = r3 << 8
# asm 1: vpsllw $8,<r3=reg128#11,>v10=reg128#16
# asm 2: vpsllw $8,<r3=%xmm10,>v10=%xmm15
vpsllw $8, % xmm10, % xmm15

# qhasm: 8x v01 = r2 unsigned>> 8
# asm 1: vpsrlw $8,<r2=reg128#12,>v01=reg128#12
# asm 2: vpsrlw $8,<r2=%xmm11,>v01=%xmm11
vpsrlw $8, % xmm11, % xmm11

# qhasm: v11 = r3 & mask5
# asm 1: vpand <mask5=reg128#6,<r3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<r3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>r3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>r3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = r4 & mask4
# asm 1: vpand <mask4=reg128#5,<r4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<r4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: 8x v10 = r5 << 8
# asm 1: vpsllw $8,<r5=reg128#9,>v10=reg128#16
# asm 2: vpsllw $8,<r5=%xmm8,>v10=%xmm15
vpsllw $8, % xmm8, % xmm15

# qhasm: 8x v01 = r4 unsigned>> 8
# asm 1: vpsrlw $8,<r4=reg128#13,>v01=reg128#13
# asm 2: vpsrlw $8,<r4=%xmm12,>v01=%xmm12
vpsrlw $8, % xmm12, % xmm12

# qhasm: v11 = r5 & mask5
# asm 1: vpand <mask5=reg128#6,<r5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<r5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>r5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>r5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = r6 & mask4
# asm 1: vpand <mask4=reg128#5,<r6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<r6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: 8x v10 = r7 << 8
# asm 1: vpsllw $8,<r7=reg128#8,>v10=reg128#16
# asm 2: vpsllw $8,<r7=%xmm7,>v10=%xmm15
vpsllw $8, % xmm7, % xmm15

# qhasm: 8x v01 = r6 unsigned>> 8
# asm 1: vpsrlw $8,<r6=reg128#7,>v01=reg128#7
# asm 2: vpsrlw $8,<r6=%xmm6,>v01=%xmm6
vpsrlw $8, % xmm6, % xmm6

# qhasm: v11 = r7 & mask5
# asm 1: vpand <mask5=reg128#6,<r7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<r7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: r6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>r7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>r7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: buf = r0[0]
# asm 1: pextrq $0x0,<r0=reg128#10,>buf=int64#2
# asm 2: pextrq $0x0,<r0=%xmm9,>buf=%rsi
pextrq $0x0, % xmm9, % rsi

# qhasm: mem64[ input_0 + 40 ] = buf
# asm 1: movq   <buf=int64#2,40(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,40(<input_0=%rdi)
movq   % rsi, 40( % rdi)

# qhasm: buf = r1[0]
# asm 1: pextrq $0x0,<r1=reg128#14,>buf=int64#2
# asm 2: pextrq $0x0,<r1=%xmm13,>buf=%rsi
pextrq $0x0, % xmm13, % rsi

# qhasm: mem64[ input_0 + 104 ] = buf
# asm 1: movq   <buf=int64#2,104(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,104(<input_0=%rdi)
movq   % rsi, 104( % rdi)

# qhasm: buf = r2[0]
# asm 1: pextrq $0x0,<r2=reg128#15,>buf=int64#2
# asm 2: pextrq $0x0,<r2=%xmm14,>buf=%rsi
pextrq $0x0, % xmm14, % rsi

# qhasm: mem64[ input_0 + 168 ] = buf
# asm 1: movq   <buf=int64#2,168(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,168(<input_0=%rdi)
movq   % rsi, 168( % rdi)

# qhasm: buf = r3[0]
# asm 1: pextrq $0x0,<r3=reg128#11,>buf=int64#2
# asm 2: pextrq $0x0,<r3=%xmm10,>buf=%rsi
pextrq $0x0, % xmm10, % rsi

# qhasm: mem64[ input_0 + 232 ] = buf
# asm 1: movq   <buf=int64#2,232(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,232(<input_0=%rdi)
movq   % rsi, 232( % rdi)

# qhasm: buf = r4[0]
# asm 1: pextrq $0x0,<r4=reg128#12,>buf=int64#2
# asm 2: pextrq $0x0,<r4=%xmm11,>buf=%rsi
pextrq $0x0, % xmm11, % rsi

# qhasm: mem64[ input_0 + 296 ] = buf
# asm 1: movq   <buf=int64#2,296(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,296(<input_0=%rdi)
movq   % rsi, 296( % rdi)

# qhasm: buf = r5[0]
# asm 1: pextrq $0x0,<r5=reg128#9,>buf=int64#2
# asm 2: pextrq $0x0,<r5=%xmm8,>buf=%rsi
pextrq $0x0, % xmm8, % rsi

# qhasm: mem64[ input_0 + 360 ] = buf
# asm 1: movq   <buf=int64#2,360(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,360(<input_0=%rdi)
movq   % rsi, 360( % rdi)

# qhasm: buf = r6[0]
# asm 1: pextrq $0x0,<r6=reg128#13,>buf=int64#2
# asm 2: pextrq $0x0,<r6=%xmm12,>buf=%rsi
pextrq $0x0, % xmm12, % rsi

# qhasm: mem64[ input_0 + 424 ] = buf
# asm 1: movq   <buf=int64#2,424(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,424(<input_0=%rdi)
movq   % rsi, 424( % rdi)

# qhasm: buf = r7[0]
# asm 1: pextrq $0x0,<r7=reg128#7,>buf=int64#2
# asm 2: pextrq $0x0,<r7=%xmm6,>buf=%rsi
pextrq $0x0, % xmm6, % rsi

# qhasm: mem64[ input_0 + 488 ] = buf
# asm 1: movq   <buf=int64#2,488(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,488(<input_0=%rdi)
movq   % rsi, 488( % rdi)

# qhasm: r0 = mem64[ input_0 + 48 ] x2
# asm 1: movddup 48(<input_0=int64#1),>r0=reg128#7
# asm 2: movddup 48(<input_0=%rdi),>r0=%xmm6
movddup 48( % rdi), % xmm6

# qhasm: r1 = mem64[ input_0 + 112 ] x2
# asm 1: movddup 112(<input_0=int64#1),>r1=reg128#8
# asm 2: movddup 112(<input_0=%rdi),>r1=%xmm7
movddup 112( % rdi), % xmm7

# qhasm: r2 = mem64[ input_0 + 176 ] x2
# asm 1: movddup 176(<input_0=int64#1),>r2=reg128#9
# asm 2: movddup 176(<input_0=%rdi),>r2=%xmm8
movddup 176( % rdi), % xmm8

# qhasm: r3 = mem64[ input_0 + 240 ] x2
# asm 1: movddup 240(<input_0=int64#1),>r3=reg128#10
# asm 2: movddup 240(<input_0=%rdi),>r3=%xmm9
movddup 240( % rdi), % xmm9

# qhasm: r4 = mem64[ input_0 + 304 ] x2
# asm 1: movddup 304(<input_0=int64#1),>r4=reg128#11
# asm 2: movddup 304(<input_0=%rdi),>r4=%xmm10
movddup 304( % rdi), % xmm10

# qhasm: r5 = mem64[ input_0 + 368 ] x2
# asm 1: movddup 368(<input_0=int64#1),>r5=reg128#12
# asm 2: movddup 368(<input_0=%rdi),>r5=%xmm11
movddup 368( % rdi), % xmm11

# qhasm: r6 = mem64[ input_0 + 432 ] x2
# asm 1: movddup 432(<input_0=int64#1),>r6=reg128#13
# asm 2: movddup 432(<input_0=%rdi),>r6=%xmm12
movddup 432( % rdi), % xmm12

# qhasm: r7 = mem64[ input_0 + 496 ] x2
# asm 1: movddup 496(<input_0=int64#1),>r7=reg128#14
# asm 2: movddup 496(<input_0=%rdi),>r7=%xmm13
movddup 496( % rdi), % xmm13

# qhasm: v00 = r0 & mask0
# asm 1: vpand <mask0=reg128#1,<r0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<r0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: 2x v10 = r4 << 32
# asm 1: vpsllq $32,<r4=reg128#11,>v10=reg128#16
# asm 2: vpsllq $32,<r4=%xmm10,>v10=%xmm15
vpsllq $32, % xmm10, % xmm15

# qhasm: 2x v01 = r0 unsigned>> 32
# asm 1: vpsrlq $32,<r0=reg128#7,>v01=reg128#7
# asm 2: vpsrlq $32,<r0=%xmm6,>v01=%xmm6
vpsrlq $32, % xmm6, % xmm6

# qhasm: v11 = r4 & mask1
# asm 1: vpand <mask1=reg128#2,<r4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<r4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>r4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>r4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = r1 & mask0
# asm 1: vpand <mask0=reg128#1,<r1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<r1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: 2x v10 = r5 << 32
# asm 1: vpsllq $32,<r5=reg128#12,>v10=reg128#16
# asm 2: vpsllq $32,<r5=%xmm11,>v10=%xmm15
vpsllq $32, % xmm11, % xmm15

# qhasm: 2x v01 = r1 unsigned>> 32
# asm 1: vpsrlq $32,<r1=reg128#8,>v01=reg128#8
# asm 2: vpsrlq $32,<r1=%xmm7,>v01=%xmm7
vpsrlq $32, % xmm7, % xmm7

# qhasm: v11 = r5 & mask1
# asm 1: vpand <mask1=reg128#2,<r5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<r5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>r1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>r1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>r5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>r5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = r2 & mask0
# asm 1: vpand <mask0=reg128#1,<r2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<r2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: 2x v10 = r6 << 32
# asm 1: vpsllq $32,<r6=reg128#13,>v10=reg128#16
# asm 2: vpsllq $32,<r6=%xmm12,>v10=%xmm15
vpsllq $32, % xmm12, % xmm15

# qhasm: 2x v01 = r2 unsigned>> 32
# asm 1: vpsrlq $32,<r2=reg128#9,>v01=reg128#9
# asm 2: vpsrlq $32,<r2=%xmm8,>v01=%xmm8
vpsrlq $32, % xmm8, % xmm8

# qhasm: v11 = r6 & mask1
# asm 1: vpand <mask1=reg128#2,<r6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<r6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>r6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>r6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = r3 & mask0
# asm 1: vpand <mask0=reg128#1,<r3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<r3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: 2x v10 = r7 << 32
# asm 1: vpsllq $32,<r7=reg128#14,>v10=reg128#16
# asm 2: vpsllq $32,<r7=%xmm13,>v10=%xmm15
vpsllq $32, % xmm13, % xmm15

# qhasm: 2x v01 = r3 unsigned>> 32
# asm 1: vpsrlq $32,<r3=reg128#10,>v01=reg128#10
# asm 2: vpsrlq $32,<r3=%xmm9,>v01=%xmm9
vpsrlq $32, % xmm9, % xmm9

# qhasm: v11 = r7 & mask1
# asm 1: vpand <mask1=reg128#2,<r7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<r7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: r3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>r7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>r7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = r0 & mask2
# asm 1: vpand <mask2=reg128#3,<r0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<r0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: 4x v10 = r2 << 16
# asm 1: vpslld $16,<r2=reg128#12,>v10=reg128#16
# asm 2: vpslld $16,<r2=%xmm11,>v10=%xmm15
vpslld $16, % xmm11, % xmm15

# qhasm: 4x v01 = r0 unsigned>> 16
# asm 1: vpsrld $16,<r0=reg128#15,>v01=reg128#15
# asm 2: vpsrld $16,<r0=%xmm14,>v01=%xmm14
vpsrld $16, % xmm14, % xmm14

# qhasm: v11 = r2 & mask3
# asm 1: vpand <mask3=reg128#4,<r2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<r2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>r0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>r0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: r2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>r2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>r2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = r1 & mask2
# asm 1: vpand <mask2=reg128#3,<r1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<r1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: 4x v10 = r3 << 16
# asm 1: vpslld $16,<r3=reg128#13,>v10=reg128#16
# asm 2: vpslld $16,<r3=%xmm12,>v10=%xmm15
vpslld $16, % xmm12, % xmm15

# qhasm: 4x v01 = r1 unsigned>> 16
# asm 1: vpsrld $16,<r1=reg128#11,>v01=reg128#11
# asm 2: vpsrld $16,<r1=%xmm10,>v01=%xmm10
vpsrld $16, % xmm10, % xmm10

# qhasm: v11 = r3 & mask3
# asm 1: vpand <mask3=reg128#4,<r3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<r3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>r3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>r3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = r4 & mask2
# asm 1: vpand <mask2=reg128#3,<r4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<r4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: 4x v10 = r6 << 16
# asm 1: vpslld $16,<r6=reg128#9,>v10=reg128#16
# asm 2: vpslld $16,<r6=%xmm8,>v10=%xmm15
vpslld $16, % xmm8, % xmm15

# qhasm: 4x v01 = r4 unsigned>> 16
# asm 1: vpsrld $16,<r4=reg128#7,>v01=reg128#7
# asm 2: vpsrld $16,<r4=%xmm6,>v01=%xmm6
vpsrld $16, % xmm6, % xmm6

# qhasm: v11 = r6 & mask3
# asm 1: vpand <mask3=reg128#4,<r6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<r6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>r6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>r6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = r5 & mask2
# asm 1: vpand <mask2=reg128#3,<r5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<r5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: 4x v10 = r7 << 16
# asm 1: vpslld $16,<r7=reg128#10,>v10=reg128#16
# asm 2: vpslld $16,<r7=%xmm9,>v10=%xmm15
vpslld $16, % xmm9, % xmm15

# qhasm: 4x v01 = r5 unsigned>> 16
# asm 1: vpsrld $16,<r5=reg128#8,>v01=reg128#8
# asm 2: vpsrld $16,<r5=%xmm7,>v01=%xmm7
vpsrld $16, % xmm7, % xmm7

# qhasm: v11 = r7 & mask3
# asm 1: vpand <mask3=reg128#4,<r7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<r7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: r5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>r5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>r5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>r7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>r7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = r0 & mask4
# asm 1: vpand <mask4=reg128#5,<r0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<r0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: 8x v10 = r1 << 8
# asm 1: vpsllw $8,<r1=reg128#15,>v10=reg128#16
# asm 2: vpsllw $8,<r1=%xmm14,>v10=%xmm15
vpsllw $8, % xmm14, % xmm15

# qhasm: 8x v01 = r0 unsigned>> 8
# asm 1: vpsrlw $8,<r0=reg128#14,>v01=reg128#14
# asm 2: vpsrlw $8,<r0=%xmm13,>v01=%xmm13
vpsrlw $8, % xmm13, % xmm13

# qhasm: v11 = r1 & mask5
# asm 1: vpand <mask5=reg128#6,<r1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<r1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>r0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>r0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: r1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>r1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>r1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = r2 & mask4
# asm 1: vpand <mask4=reg128#5,<r2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<r2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: 8x v10 = r3 << 8
# asm 1: vpsllw $8,<r3=reg128#11,>v10=reg128#16
# asm 2: vpsllw $8,<r3=%xmm10,>v10=%xmm15
vpsllw $8, % xmm10, % xmm15

# qhasm: 8x v01 = r2 unsigned>> 8
# asm 1: vpsrlw $8,<r2=reg128#12,>v01=reg128#12
# asm 2: vpsrlw $8,<r2=%xmm11,>v01=%xmm11
vpsrlw $8, % xmm11, % xmm11

# qhasm: v11 = r3 & mask5
# asm 1: vpand <mask5=reg128#6,<r3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<r3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>r3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>r3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = r4 & mask4
# asm 1: vpand <mask4=reg128#5,<r4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<r4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: 8x v10 = r5 << 8
# asm 1: vpsllw $8,<r5=reg128#9,>v10=reg128#16
# asm 2: vpsllw $8,<r5=%xmm8,>v10=%xmm15
vpsllw $8, % xmm8, % xmm15

# qhasm: 8x v01 = r4 unsigned>> 8
# asm 1: vpsrlw $8,<r4=reg128#13,>v01=reg128#13
# asm 2: vpsrlw $8,<r4=%xmm12,>v01=%xmm12
vpsrlw $8, % xmm12, % xmm12

# qhasm: v11 = r5 & mask5
# asm 1: vpand <mask5=reg128#6,<r5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<r5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>r5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>r5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = r6 & mask4
# asm 1: vpand <mask4=reg128#5,<r6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<r6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: 8x v10 = r7 << 8
# asm 1: vpsllw $8,<r7=reg128#8,>v10=reg128#16
# asm 2: vpsllw $8,<r7=%xmm7,>v10=%xmm15
vpsllw $8, % xmm7, % xmm15

# qhasm: 8x v01 = r6 unsigned>> 8
# asm 1: vpsrlw $8,<r6=reg128#7,>v01=reg128#7
# asm 2: vpsrlw $8,<r6=%xmm6,>v01=%xmm6
vpsrlw $8, % xmm6, % xmm6

# qhasm: v11 = r7 & mask5
# asm 1: vpand <mask5=reg128#6,<r7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<r7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: r6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>r7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>r7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: buf = r0[0]
# asm 1: pextrq $0x0,<r0=reg128#10,>buf=int64#2
# asm 2: pextrq $0x0,<r0=%xmm9,>buf=%rsi
pextrq $0x0, % xmm9, % rsi

# qhasm: mem64[ input_0 + 48 ] = buf
# asm 1: movq   <buf=int64#2,48(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,48(<input_0=%rdi)
movq   % rsi, 48( % rdi)

# qhasm: buf = r1[0]
# asm 1: pextrq $0x0,<r1=reg128#14,>buf=int64#2
# asm 2: pextrq $0x0,<r1=%xmm13,>buf=%rsi
pextrq $0x0, % xmm13, % rsi

# qhasm: mem64[ input_0 + 112 ] = buf
# asm 1: movq   <buf=int64#2,112(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,112(<input_0=%rdi)
movq   % rsi, 112( % rdi)

# qhasm: buf = r2[0]
# asm 1: pextrq $0x0,<r2=reg128#15,>buf=int64#2
# asm 2: pextrq $0x0,<r2=%xmm14,>buf=%rsi
pextrq $0x0, % xmm14, % rsi

# qhasm: mem64[ input_0 + 176 ] = buf
# asm 1: movq   <buf=int64#2,176(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,176(<input_0=%rdi)
movq   % rsi, 176( % rdi)

# qhasm: buf = r3[0]
# asm 1: pextrq $0x0,<r3=reg128#11,>buf=int64#2
# asm 2: pextrq $0x0,<r3=%xmm10,>buf=%rsi
pextrq $0x0, % xmm10, % rsi

# qhasm: mem64[ input_0 + 240 ] = buf
# asm 1: movq   <buf=int64#2,240(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,240(<input_0=%rdi)
movq   % rsi, 240( % rdi)

# qhasm: buf = r4[0]
# asm 1: pextrq $0x0,<r4=reg128#12,>buf=int64#2
# asm 2: pextrq $0x0,<r4=%xmm11,>buf=%rsi
pextrq $0x0, % xmm11, % rsi

# qhasm: mem64[ input_0 + 304 ] = buf
# asm 1: movq   <buf=int64#2,304(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,304(<input_0=%rdi)
movq   % rsi, 304( % rdi)

# qhasm: buf = r5[0]
# asm 1: pextrq $0x0,<r5=reg128#9,>buf=int64#2
# asm 2: pextrq $0x0,<r5=%xmm8,>buf=%rsi
pextrq $0x0, % xmm8, % rsi

# qhasm: mem64[ input_0 + 368 ] = buf
# asm 1: movq   <buf=int64#2,368(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,368(<input_0=%rdi)
movq   % rsi, 368( % rdi)

# qhasm: buf = r6[0]
# asm 1: pextrq $0x0,<r6=reg128#13,>buf=int64#2
# asm 2: pextrq $0x0,<r6=%xmm12,>buf=%rsi
pextrq $0x0, % xmm12, % rsi

# qhasm: mem64[ input_0 + 432 ] = buf
# asm 1: movq   <buf=int64#2,432(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,432(<input_0=%rdi)
movq   % rsi, 432( % rdi)

# qhasm: buf = r7[0]
# asm 1: pextrq $0x0,<r7=reg128#7,>buf=int64#2
# asm 2: pextrq $0x0,<r7=%xmm6,>buf=%rsi
pextrq $0x0, % xmm6, % rsi

# qhasm: mem64[ input_0 + 496 ] = buf
# asm 1: movq   <buf=int64#2,496(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,496(<input_0=%rdi)
movq   % rsi, 496( % rdi)

# qhasm: r0 = mem64[ input_0 + 56 ] x2
# asm 1: movddup 56(<input_0=int64#1),>r0=reg128#7
# asm 2: movddup 56(<input_0=%rdi),>r0=%xmm6
movddup 56( % rdi), % xmm6

# qhasm: r1 = mem64[ input_0 + 120 ] x2
# asm 1: movddup 120(<input_0=int64#1),>r1=reg128#8
# asm 2: movddup 120(<input_0=%rdi),>r1=%xmm7
movddup 120( % rdi), % xmm7

# qhasm: r2 = mem64[ input_0 + 184 ] x2
# asm 1: movddup 184(<input_0=int64#1),>r2=reg128#9
# asm 2: movddup 184(<input_0=%rdi),>r2=%xmm8
movddup 184( % rdi), % xmm8

# qhasm: r3 = mem64[ input_0 + 248 ] x2
# asm 1: movddup 248(<input_0=int64#1),>r3=reg128#10
# asm 2: movddup 248(<input_0=%rdi),>r3=%xmm9
movddup 248( % rdi), % xmm9

# qhasm: r4 = mem64[ input_0 + 312 ] x2
# asm 1: movddup 312(<input_0=int64#1),>r4=reg128#11
# asm 2: movddup 312(<input_0=%rdi),>r4=%xmm10
movddup 312( % rdi), % xmm10

# qhasm: r5 = mem64[ input_0 + 376 ] x2
# asm 1: movddup 376(<input_0=int64#1),>r5=reg128#12
# asm 2: movddup 376(<input_0=%rdi),>r5=%xmm11
movddup 376( % rdi), % xmm11

# qhasm: r6 = mem64[ input_0 + 440 ] x2
# asm 1: movddup 440(<input_0=int64#1),>r6=reg128#13
# asm 2: movddup 440(<input_0=%rdi),>r6=%xmm12
movddup 440( % rdi), % xmm12

# qhasm: r7 = mem64[ input_0 + 504 ] x2
# asm 1: movddup 504(<input_0=int64#1),>r7=reg128#14
# asm 2: movddup 504(<input_0=%rdi),>r7=%xmm13
movddup 504( % rdi), % xmm13

# qhasm: v00 = r0 & mask0
# asm 1: vpand <mask0=reg128#1,<r0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<r0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: 2x v10 = r4 << 32
# asm 1: vpsllq $32,<r4=reg128#11,>v10=reg128#16
# asm 2: vpsllq $32,<r4=%xmm10,>v10=%xmm15
vpsllq $32, % xmm10, % xmm15

# qhasm: 2x v01 = r0 unsigned>> 32
# asm 1: vpsrlq $32,<r0=reg128#7,>v01=reg128#7
# asm 2: vpsrlq $32,<r0=%xmm6,>v01=%xmm6
vpsrlq $32, % xmm6, % xmm6

# qhasm: v11 = r4 & mask1
# asm 1: vpand <mask1=reg128#2,<r4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<r4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>r4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>r4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = r1 & mask0
# asm 1: vpand <mask0=reg128#1,<r1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<r1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: 2x v10 = r5 << 32
# asm 1: vpsllq $32,<r5=reg128#12,>v10=reg128#16
# asm 2: vpsllq $32,<r5=%xmm11,>v10=%xmm15
vpsllq $32, % xmm11, % xmm15

# qhasm: 2x v01 = r1 unsigned>> 32
# asm 1: vpsrlq $32,<r1=reg128#8,>v01=reg128#8
# asm 2: vpsrlq $32,<r1=%xmm7,>v01=%xmm7
vpsrlq $32, % xmm7, % xmm7

# qhasm: v11 = r5 & mask1
# asm 1: vpand <mask1=reg128#2,<r5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<r5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>r1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>r1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>r5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>r5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = r2 & mask0
# asm 1: vpand <mask0=reg128#1,<r2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<r2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: 2x v10 = r6 << 32
# asm 1: vpsllq $32,<r6=reg128#13,>v10=reg128#16
# asm 2: vpsllq $32,<r6=%xmm12,>v10=%xmm15
vpsllq $32, % xmm12, % xmm15

# qhasm: 2x v01 = r2 unsigned>> 32
# asm 1: vpsrlq $32,<r2=reg128#9,>v01=reg128#9
# asm 2: vpsrlq $32,<r2=%xmm8,>v01=%xmm8
vpsrlq $32, % xmm8, % xmm8

# qhasm: v11 = r6 & mask1
# asm 1: vpand <mask1=reg128#2,<r6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<r6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>r6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>r6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = r3 & mask0
# asm 1: vpand <mask0=reg128#1,<r3=reg128#10,>v00=reg128#1
# asm 2: vpand <mask0=%xmm0,<r3=%xmm9,>v00=%xmm0
vpand % xmm0, % xmm9, % xmm0

# qhasm: 2x v10 = r7 << 32
# asm 1: vpsllq $32,<r7=reg128#14,>v10=reg128#13
# asm 2: vpsllq $32,<r7=%xmm13,>v10=%xmm12
vpsllq $32, % xmm13, % xmm12

# qhasm: 2x v01 = r3 unsigned>> 32
# asm 1: vpsrlq $32,<r3=reg128#10,>v01=reg128#10
# asm 2: vpsrlq $32,<r3=%xmm9,>v01=%xmm9
vpsrlq $32, % xmm9, % xmm9

# qhasm: v11 = r7 & mask1
# asm 1: vpand <mask1=reg128#2,<r7=reg128#14,>v11=reg128#2
# asm 2: vpand <mask1=%xmm1,<r7=%xmm13,>v11=%xmm1
vpand % xmm1, % xmm13, % xmm1

# qhasm: r3 = v00 | v10
# asm 1: vpor <v10=reg128#13,<v00=reg128#1,>r3=reg128#1
# asm 2: vpor <v10=%xmm12,<v00=%xmm0,>r3=%xmm0
vpor % xmm12, % xmm0, % xmm0

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#2,<v01=reg128#10,>r7=reg128#2
# asm 2: vpor <v11=%xmm1,<v01=%xmm9,>r7=%xmm1
vpor % xmm1, % xmm9, % xmm1

# qhasm: v00 = r0 & mask2
# asm 1: vpand <mask2=reg128#3,<r0=reg128#15,>v00=reg128#10
# asm 2: vpand <mask2=%xmm2,<r0=%xmm14,>v00=%xmm9
vpand % xmm2, % xmm14, % xmm9

# qhasm: 4x v10 = r2 << 16
# asm 1: vpslld $16,<r2=reg128#12,>v10=reg128#13
# asm 2: vpslld $16,<r2=%xmm11,>v10=%xmm12
vpslld $16, % xmm11, % xmm12

# qhasm: 4x v01 = r0 unsigned>> 16
# asm 1: vpsrld $16,<r0=reg128#15,>v01=reg128#14
# asm 2: vpsrld $16,<r0=%xmm14,>v01=%xmm13
vpsrld $16, % xmm14, % xmm13

# qhasm: v11 = r2 & mask3
# asm 1: vpand <mask3=reg128#4,<r2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<r2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#13,<v00=reg128#10,>r0=reg128#10
# asm 2: vpor <v10=%xmm12,<v00=%xmm9,>r0=%xmm9
vpor % xmm12, % xmm9, % xmm9

# qhasm: r2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#14,>r2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm13,>r2=%xmm11
vpor % xmm11, % xmm13, % xmm11

# qhasm: v00 = r1 & mask2
# asm 1: vpand <mask2=reg128#3,<r1=reg128#11,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<r1=%xmm10,>v00=%xmm12
vpand % xmm2, % xmm10, % xmm12

# qhasm: 4x v10 = r3 << 16
# asm 1: vpslld $16,<r3=reg128#1,>v10=reg128#14
# asm 2: vpslld $16,<r3=%xmm0,>v10=%xmm13
vpslld $16, % xmm0, % xmm13

# qhasm: 4x v01 = r1 unsigned>> 16
# asm 1: vpsrld $16,<r1=reg128#11,>v01=reg128#11
# asm 2: vpsrld $16,<r1=%xmm10,>v01=%xmm10
vpsrld $16, % xmm10, % xmm10

# qhasm: v11 = r3 & mask3
# asm 1: vpand <mask3=reg128#4,<r3=reg128#1,>v11=reg128#1
# asm 2: vpand <mask3=%xmm3,<r3=%xmm0,>v11=%xmm0
vpand % xmm3, % xmm0, % xmm0

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#14,<v00=reg128#13,>r1=reg128#13
# asm 2: vpor <v10=%xmm13,<v00=%xmm12,>r1=%xmm12
vpor % xmm13, % xmm12, % xmm12

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#1,<v01=reg128#11,>r3=reg128#1
# asm 2: vpor <v11=%xmm0,<v01=%xmm10,>r3=%xmm0
vpor % xmm0, % xmm10, % xmm0

# qhasm: v00 = r4 & mask2
# asm 1: vpand <mask2=reg128#3,<r4=reg128#7,>v00=reg128#11
# asm 2: vpand <mask2=%xmm2,<r4=%xmm6,>v00=%xmm10
vpand % xmm2, % xmm6, % xmm10

# qhasm: 4x v10 = r6 << 16
# asm 1: vpslld $16,<r6=reg128#9,>v10=reg128#14
# asm 2: vpslld $16,<r6=%xmm8,>v10=%xmm13
vpslld $16, % xmm8, % xmm13

# qhasm: 4x v01 = r4 unsigned>> 16
# asm 1: vpsrld $16,<r4=reg128#7,>v01=reg128#7
# asm 2: vpsrld $16,<r4=%xmm6,>v01=%xmm6
vpsrld $16, % xmm6, % xmm6

# qhasm: v11 = r6 & mask3
# asm 1: vpand <mask3=reg128#4,<r6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<r6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#14,<v00=reg128#11,>r4=reg128#11
# asm 2: vpor <v10=%xmm13,<v00=%xmm10,>r4=%xmm10
vpor % xmm13, % xmm10, % xmm10

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>r6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>r6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = r5 & mask2
# asm 1: vpand <mask2=reg128#3,<r5=reg128#8,>v00=reg128#3
# asm 2: vpand <mask2=%xmm2,<r5=%xmm7,>v00=%xmm2
vpand % xmm2, % xmm7, % xmm2

# qhasm: 4x v10 = r7 << 16
# asm 1: vpslld $16,<r7=reg128#2,>v10=reg128#9
# asm 2: vpslld $16,<r7=%xmm1,>v10=%xmm8
vpslld $16, % xmm1, % xmm8

# qhasm: 4x v01 = r5 unsigned>> 16
# asm 1: vpsrld $16,<r5=reg128#8,>v01=reg128#8
# asm 2: vpsrld $16,<r5=%xmm7,>v01=%xmm7
vpsrld $16, % xmm7, % xmm7

# qhasm: v11 = r7 & mask3
# asm 1: vpand <mask3=reg128#4,<r7=reg128#2,>v11=reg128#2
# asm 2: vpand <mask3=%xmm3,<r7=%xmm1,>v11=%xmm1
vpand % xmm3, % xmm1, % xmm1

# qhasm: r5 = v00 | v10
# asm 1: vpor <v10=reg128#9,<v00=reg128#3,>r5=reg128#3
# asm 2: vpor <v10=%xmm8,<v00=%xmm2,>r5=%xmm2
vpor % xmm8, % xmm2, % xmm2

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#2,<v01=reg128#8,>r7=reg128#2
# asm 2: vpor <v11=%xmm1,<v01=%xmm7,>r7=%xmm1
vpor % xmm1, % xmm7, % xmm1

# qhasm: v00 = r0 & mask4
# asm 1: vpand <mask4=reg128#5,<r0=reg128#10,>v00=reg128#4
# asm 2: vpand <mask4=%xmm4,<r0=%xmm9,>v00=%xmm3
vpand % xmm4, % xmm9, % xmm3

# qhasm: 8x v10 = r1 << 8
# asm 1: vpsllw $8,<r1=reg128#13,>v10=reg128#8
# asm 2: vpsllw $8,<r1=%xmm12,>v10=%xmm7
vpsllw $8, % xmm12, % xmm7

# qhasm: 8x v01 = r0 unsigned>> 8
# asm 1: vpsrlw $8,<r0=reg128#10,>v01=reg128#9
# asm 2: vpsrlw $8,<r0=%xmm9,>v01=%xmm8
vpsrlw $8, % xmm9, % xmm8

# qhasm: v11 = r1 & mask5
# asm 1: vpand <mask5=reg128#6,<r1=reg128#13,>v11=reg128#10
# asm 2: vpand <mask5=%xmm5,<r1=%xmm12,>v11=%xmm9
vpand % xmm5, % xmm12, % xmm9

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#8,<v00=reg128#4,>r0=reg128#4
# asm 2: vpor <v10=%xmm7,<v00=%xmm3,>r0=%xmm3
vpor % xmm7, % xmm3, % xmm3

# qhasm: r1 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#9,>r1=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm8,>r1=%xmm7
vpor % xmm9, % xmm8, % xmm7

# qhasm: v00 = r2 & mask4
# asm 1: vpand <mask4=reg128#5,<r2=reg128#12,>v00=reg128#9
# asm 2: vpand <mask4=%xmm4,<r2=%xmm11,>v00=%xmm8
vpand % xmm4, % xmm11, % xmm8

# qhasm: 8x v10 = r3 << 8
# asm 1: vpsllw $8,<r3=reg128#1,>v10=reg128#10
# asm 2: vpsllw $8,<r3=%xmm0,>v10=%xmm9
vpsllw $8, % xmm0, % xmm9

# qhasm: 8x v01 = r2 unsigned>> 8
# asm 1: vpsrlw $8,<r2=reg128#12,>v01=reg128#12
# asm 2: vpsrlw $8,<r2=%xmm11,>v01=%xmm11
vpsrlw $8, % xmm11, % xmm11

# qhasm: v11 = r3 & mask5
# asm 1: vpand <mask5=reg128#6,<r3=reg128#1,>v11=reg128#1
# asm 2: vpand <mask5=%xmm5,<r3=%xmm0,>v11=%xmm0
vpand % xmm5, % xmm0, % xmm0

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#10,<v00=reg128#9,>r2=reg128#9
# asm 2: vpor <v10=%xmm9,<v00=%xmm8,>r2=%xmm8
vpor % xmm9, % xmm8, % xmm8

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#1,<v01=reg128#12,>r3=reg128#1
# asm 2: vpor <v11=%xmm0,<v01=%xmm11,>r3=%xmm0
vpor % xmm0, % xmm11, % xmm0

# qhasm: v00 = r4 & mask4
# asm 1: vpand <mask4=reg128#5,<r4=reg128#11,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<r4=%xmm10,>v00=%xmm9
vpand % xmm4, % xmm10, % xmm9

# qhasm: 8x v10 = r5 << 8
# asm 1: vpsllw $8,<r5=reg128#3,>v10=reg128#12
# asm 2: vpsllw $8,<r5=%xmm2,>v10=%xmm11
vpsllw $8, % xmm2, % xmm11

# qhasm: 8x v01 = r4 unsigned>> 8
# asm 1: vpsrlw $8,<r4=reg128#11,>v01=reg128#11
# asm 2: vpsrlw $8,<r4=%xmm10,>v01=%xmm10
vpsrlw $8, % xmm10, % xmm10

# qhasm: v11 = r5 & mask5
# asm 1: vpand <mask5=reg128#6,<r5=reg128#3,>v11=reg128#3
# asm 2: vpand <mask5=%xmm5,<r5=%xmm2,>v11=%xmm2
vpand % xmm5, % xmm2, % xmm2

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#12,<v00=reg128#10,>r4=reg128#10
# asm 2: vpor <v10=%xmm11,<v00=%xmm9,>r4=%xmm9
vpor % xmm11, % xmm9, % xmm9

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#3,<v01=reg128#11,>r5=reg128#3
# asm 2: vpor <v11=%xmm2,<v01=%xmm10,>r5=%xmm2
vpor % xmm2, % xmm10, % xmm2

# qhasm: v00 = r6 & mask4
# asm 1: vpand <mask4=reg128#5,<r6=reg128#7,>v00=reg128#5
# asm 2: vpand <mask4=%xmm4,<r6=%xmm6,>v00=%xmm4
vpand % xmm4, % xmm6, % xmm4

# qhasm: 8x v10 = r7 << 8
# asm 1: vpsllw $8,<r7=reg128#2,>v10=reg128#11
# asm 2: vpsllw $8,<r7=%xmm1,>v10=%xmm10
vpsllw $8, % xmm1, % xmm10

# qhasm: 8x v01 = r6 unsigned>> 8
# asm 1: vpsrlw $8,<r6=reg128#7,>v01=reg128#7
# asm 2: vpsrlw $8,<r6=%xmm6,>v01=%xmm6
vpsrlw $8, % xmm6, % xmm6

# qhasm: v11 = r7 & mask5
# asm 1: vpand <mask5=reg128#6,<r7=reg128#2,>v11=reg128#2
# asm 2: vpand <mask5=%xmm5,<r7=%xmm1,>v11=%xmm1
vpand % xmm5, % xmm1, % xmm1

# qhasm: r6 = v00 | v10
# asm 1: vpor <v10=reg128#11,<v00=reg128#5,>r6=reg128#5
# asm 2: vpor <v10=%xmm10,<v00=%xmm4,>r6=%xmm4
vpor % xmm10, % xmm4, % xmm4

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#2,<v01=reg128#7,>r7=reg128#2
# asm 2: vpor <v11=%xmm1,<v01=%xmm6,>r7=%xmm1
vpor % xmm1, % xmm6, % xmm1

# qhasm: buf = r0[0]
# asm 1: pextrq $0x0,<r0=reg128#4,>buf=int64#2
# asm 2: pextrq $0x0,<r0=%xmm3,>buf=%rsi
pextrq $0x0, % xmm3, % rsi

# qhasm: mem64[ input_0 + 56 ] = buf
# asm 1: movq   <buf=int64#2,56(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,56(<input_0=%rdi)
movq   % rsi, 56( % rdi)

# qhasm: buf = r1[0]
# asm 1: pextrq $0x0,<r1=reg128#8,>buf=int64#2
# asm 2: pextrq $0x0,<r1=%xmm7,>buf=%rsi
pextrq $0x0, % xmm7, % rsi

# qhasm: mem64[ input_0 + 120 ] = buf
# asm 1: movq   <buf=int64#2,120(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,120(<input_0=%rdi)
movq   % rsi, 120( % rdi)

# qhasm: buf = r2[0]
# asm 1: pextrq $0x0,<r2=reg128#9,>buf=int64#2
# asm 2: pextrq $0x0,<r2=%xmm8,>buf=%rsi
pextrq $0x0, % xmm8, % rsi

# qhasm: mem64[ input_0 + 184 ] = buf
# asm 1: movq   <buf=int64#2,184(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,184(<input_0=%rdi)
movq   % rsi, 184( % rdi)

# qhasm: buf = r3[0]
# asm 1: pextrq $0x0,<r3=reg128#1,>buf=int64#2
# asm 2: pextrq $0x0,<r3=%xmm0,>buf=%rsi
pextrq $0x0, % xmm0, % rsi

# qhasm: mem64[ input_0 + 248 ] = buf
# asm 1: movq   <buf=int64#2,248(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,248(<input_0=%rdi)
movq   % rsi, 248( % rdi)

# qhasm: buf = r4[0]
# asm 1: pextrq $0x0,<r4=reg128#10,>buf=int64#2
# asm 2: pextrq $0x0,<r4=%xmm9,>buf=%rsi
pextrq $0x0, % xmm9, % rsi

# qhasm: mem64[ input_0 + 312 ] = buf
# asm 1: movq   <buf=int64#2,312(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,312(<input_0=%rdi)
movq   % rsi, 312( % rdi)

# qhasm: buf = r5[0]
# asm 1: pextrq $0x0,<r5=reg128#3,>buf=int64#2
# asm 2: pextrq $0x0,<r5=%xmm2,>buf=%rsi
pextrq $0x0, % xmm2, % rsi

# qhasm: mem64[ input_0 + 376 ] = buf
# asm 1: movq   <buf=int64#2,376(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,376(<input_0=%rdi)
movq   % rsi, 376( % rdi)

# qhasm: buf = r6[0]
# asm 1: pextrq $0x0,<r6=reg128#5,>buf=int64#2
# asm 2: pextrq $0x0,<r6=%xmm4,>buf=%rsi
pextrq $0x0, % xmm4, % rsi

# qhasm: mem64[ input_0 + 440 ] = buf
# asm 1: movq   <buf=int64#2,440(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,440(<input_0=%rdi)
movq   % rsi, 440( % rdi)

# qhasm: buf = r7[0]
# asm 1: pextrq $0x0,<r7=reg128#2,>buf=int64#2
# asm 2: pextrq $0x0,<r7=%xmm1,>buf=%rsi
pextrq $0x0, % xmm1, % rsi

# qhasm: mem64[ input_0 + 504 ] = buf
# asm 1: movq   <buf=int64#2,504(<input_0=int64#1)
# asm 2: movq   <buf=%rsi,504(<input_0=%rdi)
movq   % rsi, 504( % rdi)

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

# qhasm: r0 = mem64[ input_0 + 0 ] x2
# asm 1: movddup 0(<input_0=int64#1),>r0=reg128#7
# asm 2: movddup 0(<input_0=%rdi),>r0=%xmm6
movddup 0( % rdi), % xmm6

# qhasm: r1 = mem64[ input_0 + 8 ] x2
# asm 1: movddup 8(<input_0=int64#1),>r1=reg128#8
# asm 2: movddup 8(<input_0=%rdi),>r1=%xmm7
movddup 8( % rdi), % xmm7

# qhasm: r2 = mem64[ input_0 + 16 ] x2
# asm 1: movddup 16(<input_0=int64#1),>r2=reg128#9
# asm 2: movddup 16(<input_0=%rdi),>r2=%xmm8
movddup 16( % rdi), % xmm8

# qhasm: r3 = mem64[ input_0 + 24 ] x2
# asm 1: movddup 24(<input_0=int64#1),>r3=reg128#10
# asm 2: movddup 24(<input_0=%rdi),>r3=%xmm9
movddup 24( % rdi), % xmm9

# qhasm: r4 = mem64[ input_0 + 32 ] x2
# asm 1: movddup 32(<input_0=int64#1),>r4=reg128#11
# asm 2: movddup 32(<input_0=%rdi),>r4=%xmm10
movddup 32( % rdi), % xmm10

# qhasm: r5 = mem64[ input_0 + 40 ] x2
# asm 1: movddup 40(<input_0=int64#1),>r5=reg128#12
# asm 2: movddup 40(<input_0=%rdi),>r5=%xmm11
movddup 40( % rdi), % xmm11

# qhasm: r6 = mem64[ input_0 + 48 ] x2
# asm 1: movddup 48(<input_0=int64#1),>r6=reg128#13
# asm 2: movddup 48(<input_0=%rdi),>r6=%xmm12
movddup 48( % rdi), % xmm12

# qhasm: r7 = mem64[ input_0 + 56 ] x2
# asm 1: movddup 56(<input_0=int64#1),>r7=reg128#14
# asm 2: movddup 56(<input_0=%rdi),>r7=%xmm13
movddup 56( % rdi), % xmm13

# qhasm: v00 = r0 & mask0
# asm 1: vpand <mask0=reg128#1,<r0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<r0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: v10 = r4 & mask0
# asm 1: vpand <mask0=reg128#1,<r4=reg128#11,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r4=%xmm10,>v10=%xmm15
vpand % xmm0, % xmm10, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r0 & mask1
# asm 1: vpand <mask1=reg128#2,<r0=reg128#7,>v01=reg128#7
# asm 2: vpand <mask1=%xmm1,<r0=%xmm6,>v01=%xmm6
vpand % xmm1, % xmm6, % xmm6

# qhasm: v11 = r4 & mask1
# asm 1: vpand <mask1=reg128#2,<r4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<r4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#7
# asm 2: psrlq $4,<v01=%xmm6
psrlq $4, % xmm6

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>r4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>r4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = r1 & mask0
# asm 1: vpand <mask0=reg128#1,<r1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<r1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: v10 = r5 & mask0
# asm 1: vpand <mask0=reg128#1,<r5=reg128#12,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r5=%xmm11,>v10=%xmm15
vpand % xmm0, % xmm11, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r1 & mask1
# asm 1: vpand <mask1=reg128#2,<r1=reg128#8,>v01=reg128#8
# asm 2: vpand <mask1=%xmm1,<r1=%xmm7,>v01=%xmm7
vpand % xmm1, % xmm7, % xmm7

# qhasm: v11 = r5 & mask1
# asm 1: vpand <mask1=reg128#2,<r5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<r5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#8
# asm 2: psrlq $4,<v01=%xmm7
psrlq $4, % xmm7

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>r1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>r1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>r5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>r5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = r2 & mask0
# asm 1: vpand <mask0=reg128#1,<r2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<r2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: v10 = r6 & mask0
# asm 1: vpand <mask0=reg128#1,<r6=reg128#13,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r6=%xmm12,>v10=%xmm15
vpand % xmm0, % xmm12, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r2 & mask1
# asm 1: vpand <mask1=reg128#2,<r2=reg128#9,>v01=reg128#9
# asm 2: vpand <mask1=%xmm1,<r2=%xmm8,>v01=%xmm8
vpand % xmm1, % xmm8, % xmm8

# qhasm: v11 = r6 & mask1
# asm 1: vpand <mask1=reg128#2,<r6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<r6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#9
# asm 2: psrlq $4,<v01=%xmm8
psrlq $4, % xmm8

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>r6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>r6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = r3 & mask0
# asm 1: vpand <mask0=reg128#1,<r3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<r3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: v10 = r7 & mask0
# asm 1: vpand <mask0=reg128#1,<r7=reg128#14,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r7=%xmm13,>v10=%xmm15
vpand % xmm0, % xmm13, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r3 & mask1
# asm 1: vpand <mask1=reg128#2,<r3=reg128#10,>v01=reg128#10
# asm 2: vpand <mask1=%xmm1,<r3=%xmm9,>v01=%xmm9
vpand % xmm1, % xmm9, % xmm9

# qhasm: v11 = r7 & mask1
# asm 1: vpand <mask1=reg128#2,<r7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<r7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#10
# asm 2: psrlq $4,<v01=%xmm9
psrlq $4, % xmm9

# qhasm: r3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>r7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>r7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = r0 & mask2
# asm 1: vpand <mask2=reg128#3,<r0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<r0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: v10 = r2 & mask2
# asm 1: vpand <mask2=reg128#3,<r2=reg128#12,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r2=%xmm11,>v10=%xmm15
vpand % xmm2, % xmm11, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r0 & mask3
# asm 1: vpand <mask3=reg128#4,<r0=reg128#15,>v01=reg128#15
# asm 2: vpand <mask3=%xmm3,<r0=%xmm14,>v01=%xmm14
vpand % xmm3, % xmm14, % xmm14

# qhasm: v11 = r2 & mask3
# asm 1: vpand <mask3=reg128#4,<r2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<r2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#15
# asm 2: psrlq $2,<v01=%xmm14
psrlq $2, % xmm14

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>r0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>r0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: r2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>r2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>r2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = r1 & mask2
# asm 1: vpand <mask2=reg128#3,<r1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<r1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: v10 = r3 & mask2
# asm 1: vpand <mask2=reg128#3,<r3=reg128#13,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r3=%xmm12,>v10=%xmm15
vpand % xmm2, % xmm12, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r1 & mask3
# asm 1: vpand <mask3=reg128#4,<r1=reg128#11,>v01=reg128#11
# asm 2: vpand <mask3=%xmm3,<r1=%xmm10,>v01=%xmm10
vpand % xmm3, % xmm10, % xmm10

# qhasm: v11 = r3 & mask3
# asm 1: vpand <mask3=reg128#4,<r3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<r3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#11
# asm 2: psrlq $2,<v01=%xmm10
psrlq $2, % xmm10

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>r3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>r3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = r4 & mask2
# asm 1: vpand <mask2=reg128#3,<r4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<r4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: v10 = r6 & mask2
# asm 1: vpand <mask2=reg128#3,<r6=reg128#9,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r6=%xmm8,>v10=%xmm15
vpand % xmm2, % xmm8, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r4 & mask3
# asm 1: vpand <mask3=reg128#4,<r4=reg128#7,>v01=reg128#7
# asm 2: vpand <mask3=%xmm3,<r4=%xmm6,>v01=%xmm6
vpand % xmm3, % xmm6, % xmm6

# qhasm: v11 = r6 & mask3
# asm 1: vpand <mask3=reg128#4,<r6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<r6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#7
# asm 2: psrlq $2,<v01=%xmm6
psrlq $2, % xmm6

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>r6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>r6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = r5 & mask2
# asm 1: vpand <mask2=reg128#3,<r5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<r5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: v10 = r7 & mask2
# asm 1: vpand <mask2=reg128#3,<r7=reg128#10,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r7=%xmm9,>v10=%xmm15
vpand % xmm2, % xmm9, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r5 & mask3
# asm 1: vpand <mask3=reg128#4,<r5=reg128#8,>v01=reg128#8
# asm 2: vpand <mask3=%xmm3,<r5=%xmm7,>v01=%xmm7
vpand % xmm3, % xmm7, % xmm7

# qhasm: v11 = r7 & mask3
# asm 1: vpand <mask3=reg128#4,<r7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<r7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#8
# asm 2: psrlq $2,<v01=%xmm7
psrlq $2, % xmm7

# qhasm: r5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>r5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>r5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>r7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>r7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = r0 & mask4
# asm 1: vpand <mask4=reg128#5,<r0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<r0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: v10 = r1 & mask4
# asm 1: vpand <mask4=reg128#5,<r1=reg128#15,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r1=%xmm14,>v10=%xmm15
vpand % xmm4, % xmm14, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r0 & mask5
# asm 1: vpand <mask5=reg128#6,<r0=reg128#14,>v01=reg128#14
# asm 2: vpand <mask5=%xmm5,<r0=%xmm13,>v01=%xmm13
vpand % xmm5, % xmm13, % xmm13

# qhasm: v11 = r1 & mask5
# asm 1: vpand <mask5=reg128#6,<r1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<r1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#14
# asm 2: psrlq $1,<v01=%xmm13
psrlq $1, % xmm13

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>r0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>r0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: r1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>r1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>r1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = r2 & mask4
# asm 1: vpand <mask4=reg128#5,<r2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<r2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: v10 = r3 & mask4
# asm 1: vpand <mask4=reg128#5,<r3=reg128#11,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r3=%xmm10,>v10=%xmm15
vpand % xmm4, % xmm10, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r2 & mask5
# asm 1: vpand <mask5=reg128#6,<r2=reg128#12,>v01=reg128#12
# asm 2: vpand <mask5=%xmm5,<r2=%xmm11,>v01=%xmm11
vpand % xmm5, % xmm11, % xmm11

# qhasm: v11 = r3 & mask5
# asm 1: vpand <mask5=reg128#6,<r3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<r3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#12
# asm 2: psrlq $1,<v01=%xmm11
psrlq $1, % xmm11

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>r3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>r3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = r4 & mask4
# asm 1: vpand <mask4=reg128#5,<r4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<r4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: v10 = r5 & mask4
# asm 1: vpand <mask4=reg128#5,<r5=reg128#9,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r5=%xmm8,>v10=%xmm15
vpand % xmm4, % xmm8, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r4 & mask5
# asm 1: vpand <mask5=reg128#6,<r4=reg128#13,>v01=reg128#13
# asm 2: vpand <mask5=%xmm5,<r4=%xmm12,>v01=%xmm12
vpand % xmm5, % xmm12, % xmm12

# qhasm: v11 = r5 & mask5
# asm 1: vpand <mask5=reg128#6,<r5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<r5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#13
# asm 2: psrlq $1,<v01=%xmm12
psrlq $1, % xmm12

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>r5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>r5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = r6 & mask4
# asm 1: vpand <mask4=reg128#5,<r6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<r6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: v10 = r7 & mask4
# asm 1: vpand <mask4=reg128#5,<r7=reg128#8,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r7=%xmm7,>v10=%xmm15
vpand % xmm4, % xmm7, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r6 & mask5
# asm 1: vpand <mask5=reg128#6,<r6=reg128#7,>v01=reg128#7
# asm 2: vpand <mask5=%xmm5,<r6=%xmm6,>v01=%xmm6
vpand % xmm5, % xmm6, % xmm6

# qhasm: v11 = r7 & mask5
# asm 1: vpand <mask5=reg128#6,<r7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<r7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#7
# asm 2: psrlq $1,<v01=%xmm6
psrlq $1, % xmm6

# qhasm: r6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>r7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>r7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: t0 = r0[0]r1[0]
# asm 1: vpunpcklqdq <r1=reg128#14,<r0=reg128#10,>t0=reg128#8
# asm 2: vpunpcklqdq <r1=%xmm13,<r0=%xmm9,>t0=%xmm7
vpunpcklqdq % xmm13, % xmm9, % xmm7

# qhasm: mem128[ input_0 + 0 ] = t0
# asm 1: movdqu   <t0=reg128#8,0(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,0(<input_0=%rdi)
movdqu   % xmm7, 0( % rdi)

# qhasm: t0 = r2[0]r3[0]
# asm 1: vpunpcklqdq <r3=reg128#11,<r2=reg128#15,>t0=reg128#8
# asm 2: vpunpcklqdq <r3=%xmm10,<r2=%xmm14,>t0=%xmm7
vpunpcklqdq % xmm10, % xmm14, % xmm7

# qhasm: mem128[ input_0 + 16 ] = t0
# asm 1: movdqu   <t0=reg128#8,16(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,16(<input_0=%rdi)
movdqu   % xmm7, 16( % rdi)

# qhasm: t0 = r4[0]r5[0]
# asm 1: vpunpcklqdq <r5=reg128#9,<r4=reg128#12,>t0=reg128#8
# asm 2: vpunpcklqdq <r5=%xmm8,<r4=%xmm11,>t0=%xmm7
vpunpcklqdq % xmm8, % xmm11, % xmm7

# qhasm: mem128[ input_0 + 32 ] = t0
# asm 1: movdqu   <t0=reg128#8,32(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,32(<input_0=%rdi)
movdqu   % xmm7, 32( % rdi)

# qhasm: t0 = r6[0]r7[0]
# asm 1: vpunpcklqdq <r7=reg128#7,<r6=reg128#13,>t0=reg128#7
# asm 2: vpunpcklqdq <r7=%xmm6,<r6=%xmm12,>t0=%xmm6
vpunpcklqdq % xmm6, % xmm12, % xmm6

# qhasm: mem128[ input_0 + 48 ] = t0
# asm 1: movdqu   <t0=reg128#7,48(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm6,48(<input_0=%rdi)
movdqu   % xmm6, 48( % rdi)

# qhasm: r0 = mem64[ input_0 + 64 ] x2
# asm 1: movddup 64(<input_0=int64#1),>r0=reg128#7
# asm 2: movddup 64(<input_0=%rdi),>r0=%xmm6
movddup 64( % rdi), % xmm6

# qhasm: r1 = mem64[ input_0 + 72 ] x2
# asm 1: movddup 72(<input_0=int64#1),>r1=reg128#8
# asm 2: movddup 72(<input_0=%rdi),>r1=%xmm7
movddup 72( % rdi), % xmm7

# qhasm: r2 = mem64[ input_0 + 80 ] x2
# asm 1: movddup 80(<input_0=int64#1),>r2=reg128#9
# asm 2: movddup 80(<input_0=%rdi),>r2=%xmm8
movddup 80( % rdi), % xmm8

# qhasm: r3 = mem64[ input_0 + 88 ] x2
# asm 1: movddup 88(<input_0=int64#1),>r3=reg128#10
# asm 2: movddup 88(<input_0=%rdi),>r3=%xmm9
movddup 88( % rdi), % xmm9

# qhasm: r4 = mem64[ input_0 + 96 ] x2
# asm 1: movddup 96(<input_0=int64#1),>r4=reg128#11
# asm 2: movddup 96(<input_0=%rdi),>r4=%xmm10
movddup 96( % rdi), % xmm10

# qhasm: r5 = mem64[ input_0 + 104 ] x2
# asm 1: movddup 104(<input_0=int64#1),>r5=reg128#12
# asm 2: movddup 104(<input_0=%rdi),>r5=%xmm11
movddup 104( % rdi), % xmm11

# qhasm: r6 = mem64[ input_0 + 112 ] x2
# asm 1: movddup 112(<input_0=int64#1),>r6=reg128#13
# asm 2: movddup 112(<input_0=%rdi),>r6=%xmm12
movddup 112( % rdi), % xmm12

# qhasm: r7 = mem64[ input_0 + 120 ] x2
# asm 1: movddup 120(<input_0=int64#1),>r7=reg128#14
# asm 2: movddup 120(<input_0=%rdi),>r7=%xmm13
movddup 120( % rdi), % xmm13

# qhasm: v00 = r0 & mask0
# asm 1: vpand <mask0=reg128#1,<r0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<r0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: v10 = r4 & mask0
# asm 1: vpand <mask0=reg128#1,<r4=reg128#11,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r4=%xmm10,>v10=%xmm15
vpand % xmm0, % xmm10, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r0 & mask1
# asm 1: vpand <mask1=reg128#2,<r0=reg128#7,>v01=reg128#7
# asm 2: vpand <mask1=%xmm1,<r0=%xmm6,>v01=%xmm6
vpand % xmm1, % xmm6, % xmm6

# qhasm: v11 = r4 & mask1
# asm 1: vpand <mask1=reg128#2,<r4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<r4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#7
# asm 2: psrlq $4,<v01=%xmm6
psrlq $4, % xmm6

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>r4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>r4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = r1 & mask0
# asm 1: vpand <mask0=reg128#1,<r1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<r1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: v10 = r5 & mask0
# asm 1: vpand <mask0=reg128#1,<r5=reg128#12,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r5=%xmm11,>v10=%xmm15
vpand % xmm0, % xmm11, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r1 & mask1
# asm 1: vpand <mask1=reg128#2,<r1=reg128#8,>v01=reg128#8
# asm 2: vpand <mask1=%xmm1,<r1=%xmm7,>v01=%xmm7
vpand % xmm1, % xmm7, % xmm7

# qhasm: v11 = r5 & mask1
# asm 1: vpand <mask1=reg128#2,<r5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<r5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#8
# asm 2: psrlq $4,<v01=%xmm7
psrlq $4, % xmm7

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>r1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>r1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>r5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>r5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = r2 & mask0
# asm 1: vpand <mask0=reg128#1,<r2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<r2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: v10 = r6 & mask0
# asm 1: vpand <mask0=reg128#1,<r6=reg128#13,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r6=%xmm12,>v10=%xmm15
vpand % xmm0, % xmm12, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r2 & mask1
# asm 1: vpand <mask1=reg128#2,<r2=reg128#9,>v01=reg128#9
# asm 2: vpand <mask1=%xmm1,<r2=%xmm8,>v01=%xmm8
vpand % xmm1, % xmm8, % xmm8

# qhasm: v11 = r6 & mask1
# asm 1: vpand <mask1=reg128#2,<r6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<r6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#9
# asm 2: psrlq $4,<v01=%xmm8
psrlq $4, % xmm8

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>r6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>r6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = r3 & mask0
# asm 1: vpand <mask0=reg128#1,<r3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<r3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: v10 = r7 & mask0
# asm 1: vpand <mask0=reg128#1,<r7=reg128#14,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r7=%xmm13,>v10=%xmm15
vpand % xmm0, % xmm13, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r3 & mask1
# asm 1: vpand <mask1=reg128#2,<r3=reg128#10,>v01=reg128#10
# asm 2: vpand <mask1=%xmm1,<r3=%xmm9,>v01=%xmm9
vpand % xmm1, % xmm9, % xmm9

# qhasm: v11 = r7 & mask1
# asm 1: vpand <mask1=reg128#2,<r7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<r7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#10
# asm 2: psrlq $4,<v01=%xmm9
psrlq $4, % xmm9

# qhasm: r3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>r7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>r7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = r0 & mask2
# asm 1: vpand <mask2=reg128#3,<r0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<r0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: v10 = r2 & mask2
# asm 1: vpand <mask2=reg128#3,<r2=reg128#12,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r2=%xmm11,>v10=%xmm15
vpand % xmm2, % xmm11, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r0 & mask3
# asm 1: vpand <mask3=reg128#4,<r0=reg128#15,>v01=reg128#15
# asm 2: vpand <mask3=%xmm3,<r0=%xmm14,>v01=%xmm14
vpand % xmm3, % xmm14, % xmm14

# qhasm: v11 = r2 & mask3
# asm 1: vpand <mask3=reg128#4,<r2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<r2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#15
# asm 2: psrlq $2,<v01=%xmm14
psrlq $2, % xmm14

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>r0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>r0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: r2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>r2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>r2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = r1 & mask2
# asm 1: vpand <mask2=reg128#3,<r1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<r1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: v10 = r3 & mask2
# asm 1: vpand <mask2=reg128#3,<r3=reg128#13,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r3=%xmm12,>v10=%xmm15
vpand % xmm2, % xmm12, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r1 & mask3
# asm 1: vpand <mask3=reg128#4,<r1=reg128#11,>v01=reg128#11
# asm 2: vpand <mask3=%xmm3,<r1=%xmm10,>v01=%xmm10
vpand % xmm3, % xmm10, % xmm10

# qhasm: v11 = r3 & mask3
# asm 1: vpand <mask3=reg128#4,<r3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<r3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#11
# asm 2: psrlq $2,<v01=%xmm10
psrlq $2, % xmm10

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>r3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>r3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = r4 & mask2
# asm 1: vpand <mask2=reg128#3,<r4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<r4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: v10 = r6 & mask2
# asm 1: vpand <mask2=reg128#3,<r6=reg128#9,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r6=%xmm8,>v10=%xmm15
vpand % xmm2, % xmm8, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r4 & mask3
# asm 1: vpand <mask3=reg128#4,<r4=reg128#7,>v01=reg128#7
# asm 2: vpand <mask3=%xmm3,<r4=%xmm6,>v01=%xmm6
vpand % xmm3, % xmm6, % xmm6

# qhasm: v11 = r6 & mask3
# asm 1: vpand <mask3=reg128#4,<r6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<r6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#7
# asm 2: psrlq $2,<v01=%xmm6
psrlq $2, % xmm6

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>r6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>r6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = r5 & mask2
# asm 1: vpand <mask2=reg128#3,<r5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<r5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: v10 = r7 & mask2
# asm 1: vpand <mask2=reg128#3,<r7=reg128#10,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r7=%xmm9,>v10=%xmm15
vpand % xmm2, % xmm9, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r5 & mask3
# asm 1: vpand <mask3=reg128#4,<r5=reg128#8,>v01=reg128#8
# asm 2: vpand <mask3=%xmm3,<r5=%xmm7,>v01=%xmm7
vpand % xmm3, % xmm7, % xmm7

# qhasm: v11 = r7 & mask3
# asm 1: vpand <mask3=reg128#4,<r7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<r7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#8
# asm 2: psrlq $2,<v01=%xmm7
psrlq $2, % xmm7

# qhasm: r5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>r5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>r5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>r7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>r7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = r0 & mask4
# asm 1: vpand <mask4=reg128#5,<r0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<r0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: v10 = r1 & mask4
# asm 1: vpand <mask4=reg128#5,<r1=reg128#15,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r1=%xmm14,>v10=%xmm15
vpand % xmm4, % xmm14, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r0 & mask5
# asm 1: vpand <mask5=reg128#6,<r0=reg128#14,>v01=reg128#14
# asm 2: vpand <mask5=%xmm5,<r0=%xmm13,>v01=%xmm13
vpand % xmm5, % xmm13, % xmm13

# qhasm: v11 = r1 & mask5
# asm 1: vpand <mask5=reg128#6,<r1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<r1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#14
# asm 2: psrlq $1,<v01=%xmm13
psrlq $1, % xmm13

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>r0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>r0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: r1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>r1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>r1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = r2 & mask4
# asm 1: vpand <mask4=reg128#5,<r2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<r2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: v10 = r3 & mask4
# asm 1: vpand <mask4=reg128#5,<r3=reg128#11,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r3=%xmm10,>v10=%xmm15
vpand % xmm4, % xmm10, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r2 & mask5
# asm 1: vpand <mask5=reg128#6,<r2=reg128#12,>v01=reg128#12
# asm 2: vpand <mask5=%xmm5,<r2=%xmm11,>v01=%xmm11
vpand % xmm5, % xmm11, % xmm11

# qhasm: v11 = r3 & mask5
# asm 1: vpand <mask5=reg128#6,<r3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<r3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#12
# asm 2: psrlq $1,<v01=%xmm11
psrlq $1, % xmm11

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>r3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>r3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = r4 & mask4
# asm 1: vpand <mask4=reg128#5,<r4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<r4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: v10 = r5 & mask4
# asm 1: vpand <mask4=reg128#5,<r5=reg128#9,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r5=%xmm8,>v10=%xmm15
vpand % xmm4, % xmm8, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r4 & mask5
# asm 1: vpand <mask5=reg128#6,<r4=reg128#13,>v01=reg128#13
# asm 2: vpand <mask5=%xmm5,<r4=%xmm12,>v01=%xmm12
vpand % xmm5, % xmm12, % xmm12

# qhasm: v11 = r5 & mask5
# asm 1: vpand <mask5=reg128#6,<r5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<r5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#13
# asm 2: psrlq $1,<v01=%xmm12
psrlq $1, % xmm12

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>r5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>r5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = r6 & mask4
# asm 1: vpand <mask4=reg128#5,<r6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<r6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: v10 = r7 & mask4
# asm 1: vpand <mask4=reg128#5,<r7=reg128#8,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r7=%xmm7,>v10=%xmm15
vpand % xmm4, % xmm7, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r6 & mask5
# asm 1: vpand <mask5=reg128#6,<r6=reg128#7,>v01=reg128#7
# asm 2: vpand <mask5=%xmm5,<r6=%xmm6,>v01=%xmm6
vpand % xmm5, % xmm6, % xmm6

# qhasm: v11 = r7 & mask5
# asm 1: vpand <mask5=reg128#6,<r7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<r7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#7
# asm 2: psrlq $1,<v01=%xmm6
psrlq $1, % xmm6

# qhasm: r6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>r7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>r7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: t0 = r0[0]r1[0]
# asm 1: vpunpcklqdq <r1=reg128#14,<r0=reg128#10,>t0=reg128#8
# asm 2: vpunpcklqdq <r1=%xmm13,<r0=%xmm9,>t0=%xmm7
vpunpcklqdq % xmm13, % xmm9, % xmm7

# qhasm: mem128[ input_0 + 64 ] = t0
# asm 1: movdqu   <t0=reg128#8,64(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,64(<input_0=%rdi)
movdqu   % xmm7, 64( % rdi)

# qhasm: t0 = r2[0]r3[0]
# asm 1: vpunpcklqdq <r3=reg128#11,<r2=reg128#15,>t0=reg128#8
# asm 2: vpunpcklqdq <r3=%xmm10,<r2=%xmm14,>t0=%xmm7
vpunpcklqdq % xmm10, % xmm14, % xmm7

# qhasm: mem128[ input_0 + 80 ] = t0
# asm 1: movdqu   <t0=reg128#8,80(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,80(<input_0=%rdi)
movdqu   % xmm7, 80( % rdi)

# qhasm: t0 = r4[0]r5[0]
# asm 1: vpunpcklqdq <r5=reg128#9,<r4=reg128#12,>t0=reg128#8
# asm 2: vpunpcklqdq <r5=%xmm8,<r4=%xmm11,>t0=%xmm7
vpunpcklqdq % xmm8, % xmm11, % xmm7

# qhasm: mem128[ input_0 + 96 ] = t0
# asm 1: movdqu   <t0=reg128#8,96(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,96(<input_0=%rdi)
movdqu   % xmm7, 96( % rdi)

# qhasm: t0 = r6[0]r7[0]
# asm 1: vpunpcklqdq <r7=reg128#7,<r6=reg128#13,>t0=reg128#7
# asm 2: vpunpcklqdq <r7=%xmm6,<r6=%xmm12,>t0=%xmm6
vpunpcklqdq % xmm6, % xmm12, % xmm6

# qhasm: mem128[ input_0 + 112 ] = t0
# asm 1: movdqu   <t0=reg128#7,112(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm6,112(<input_0=%rdi)
movdqu   % xmm6, 112( % rdi)

# qhasm: r0 = mem64[ input_0 + 128 ] x2
# asm 1: movddup 128(<input_0=int64#1),>r0=reg128#7
# asm 2: movddup 128(<input_0=%rdi),>r0=%xmm6
movddup 128( % rdi), % xmm6

# qhasm: r1 = mem64[ input_0 + 136 ] x2
# asm 1: movddup 136(<input_0=int64#1),>r1=reg128#8
# asm 2: movddup 136(<input_0=%rdi),>r1=%xmm7
movddup 136( % rdi), % xmm7

# qhasm: r2 = mem64[ input_0 + 144 ] x2
# asm 1: movddup 144(<input_0=int64#1),>r2=reg128#9
# asm 2: movddup 144(<input_0=%rdi),>r2=%xmm8
movddup 144( % rdi), % xmm8

# qhasm: r3 = mem64[ input_0 + 152 ] x2
# asm 1: movddup 152(<input_0=int64#1),>r3=reg128#10
# asm 2: movddup 152(<input_0=%rdi),>r3=%xmm9
movddup 152( % rdi), % xmm9

# qhasm: r4 = mem64[ input_0 + 160 ] x2
# asm 1: movddup 160(<input_0=int64#1),>r4=reg128#11
# asm 2: movddup 160(<input_0=%rdi),>r4=%xmm10
movddup 160( % rdi), % xmm10

# qhasm: r5 = mem64[ input_0 + 168 ] x2
# asm 1: movddup 168(<input_0=int64#1),>r5=reg128#12
# asm 2: movddup 168(<input_0=%rdi),>r5=%xmm11
movddup 168( % rdi), % xmm11

# qhasm: r6 = mem64[ input_0 + 176 ] x2
# asm 1: movddup 176(<input_0=int64#1),>r6=reg128#13
# asm 2: movddup 176(<input_0=%rdi),>r6=%xmm12
movddup 176( % rdi), % xmm12

# qhasm: r7 = mem64[ input_0 + 184 ] x2
# asm 1: movddup 184(<input_0=int64#1),>r7=reg128#14
# asm 2: movddup 184(<input_0=%rdi),>r7=%xmm13
movddup 184( % rdi), % xmm13

# qhasm: v00 = r0 & mask0
# asm 1: vpand <mask0=reg128#1,<r0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<r0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: v10 = r4 & mask0
# asm 1: vpand <mask0=reg128#1,<r4=reg128#11,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r4=%xmm10,>v10=%xmm15
vpand % xmm0, % xmm10, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r0 & mask1
# asm 1: vpand <mask1=reg128#2,<r0=reg128#7,>v01=reg128#7
# asm 2: vpand <mask1=%xmm1,<r0=%xmm6,>v01=%xmm6
vpand % xmm1, % xmm6, % xmm6

# qhasm: v11 = r4 & mask1
# asm 1: vpand <mask1=reg128#2,<r4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<r4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#7
# asm 2: psrlq $4,<v01=%xmm6
psrlq $4, % xmm6

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>r4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>r4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = r1 & mask0
# asm 1: vpand <mask0=reg128#1,<r1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<r1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: v10 = r5 & mask0
# asm 1: vpand <mask0=reg128#1,<r5=reg128#12,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r5=%xmm11,>v10=%xmm15
vpand % xmm0, % xmm11, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r1 & mask1
# asm 1: vpand <mask1=reg128#2,<r1=reg128#8,>v01=reg128#8
# asm 2: vpand <mask1=%xmm1,<r1=%xmm7,>v01=%xmm7
vpand % xmm1, % xmm7, % xmm7

# qhasm: v11 = r5 & mask1
# asm 1: vpand <mask1=reg128#2,<r5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<r5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#8
# asm 2: psrlq $4,<v01=%xmm7
psrlq $4, % xmm7

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>r1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>r1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>r5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>r5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = r2 & mask0
# asm 1: vpand <mask0=reg128#1,<r2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<r2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: v10 = r6 & mask0
# asm 1: vpand <mask0=reg128#1,<r6=reg128#13,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r6=%xmm12,>v10=%xmm15
vpand % xmm0, % xmm12, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r2 & mask1
# asm 1: vpand <mask1=reg128#2,<r2=reg128#9,>v01=reg128#9
# asm 2: vpand <mask1=%xmm1,<r2=%xmm8,>v01=%xmm8
vpand % xmm1, % xmm8, % xmm8

# qhasm: v11 = r6 & mask1
# asm 1: vpand <mask1=reg128#2,<r6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<r6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#9
# asm 2: psrlq $4,<v01=%xmm8
psrlq $4, % xmm8

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>r6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>r6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = r3 & mask0
# asm 1: vpand <mask0=reg128#1,<r3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<r3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: v10 = r7 & mask0
# asm 1: vpand <mask0=reg128#1,<r7=reg128#14,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r7=%xmm13,>v10=%xmm15
vpand % xmm0, % xmm13, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r3 & mask1
# asm 1: vpand <mask1=reg128#2,<r3=reg128#10,>v01=reg128#10
# asm 2: vpand <mask1=%xmm1,<r3=%xmm9,>v01=%xmm9
vpand % xmm1, % xmm9, % xmm9

# qhasm: v11 = r7 & mask1
# asm 1: vpand <mask1=reg128#2,<r7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<r7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#10
# asm 2: psrlq $4,<v01=%xmm9
psrlq $4, % xmm9

# qhasm: r3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>r7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>r7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = r0 & mask2
# asm 1: vpand <mask2=reg128#3,<r0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<r0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: v10 = r2 & mask2
# asm 1: vpand <mask2=reg128#3,<r2=reg128#12,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r2=%xmm11,>v10=%xmm15
vpand % xmm2, % xmm11, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r0 & mask3
# asm 1: vpand <mask3=reg128#4,<r0=reg128#15,>v01=reg128#15
# asm 2: vpand <mask3=%xmm3,<r0=%xmm14,>v01=%xmm14
vpand % xmm3, % xmm14, % xmm14

# qhasm: v11 = r2 & mask3
# asm 1: vpand <mask3=reg128#4,<r2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<r2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#15
# asm 2: psrlq $2,<v01=%xmm14
psrlq $2, % xmm14

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>r0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>r0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: r2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>r2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>r2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = r1 & mask2
# asm 1: vpand <mask2=reg128#3,<r1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<r1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: v10 = r3 & mask2
# asm 1: vpand <mask2=reg128#3,<r3=reg128#13,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r3=%xmm12,>v10=%xmm15
vpand % xmm2, % xmm12, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r1 & mask3
# asm 1: vpand <mask3=reg128#4,<r1=reg128#11,>v01=reg128#11
# asm 2: vpand <mask3=%xmm3,<r1=%xmm10,>v01=%xmm10
vpand % xmm3, % xmm10, % xmm10

# qhasm: v11 = r3 & mask3
# asm 1: vpand <mask3=reg128#4,<r3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<r3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#11
# asm 2: psrlq $2,<v01=%xmm10
psrlq $2, % xmm10

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>r3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>r3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = r4 & mask2
# asm 1: vpand <mask2=reg128#3,<r4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<r4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: v10 = r6 & mask2
# asm 1: vpand <mask2=reg128#3,<r6=reg128#9,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r6=%xmm8,>v10=%xmm15
vpand % xmm2, % xmm8, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r4 & mask3
# asm 1: vpand <mask3=reg128#4,<r4=reg128#7,>v01=reg128#7
# asm 2: vpand <mask3=%xmm3,<r4=%xmm6,>v01=%xmm6
vpand % xmm3, % xmm6, % xmm6

# qhasm: v11 = r6 & mask3
# asm 1: vpand <mask3=reg128#4,<r6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<r6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#7
# asm 2: psrlq $2,<v01=%xmm6
psrlq $2, % xmm6

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>r6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>r6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = r5 & mask2
# asm 1: vpand <mask2=reg128#3,<r5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<r5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: v10 = r7 & mask2
# asm 1: vpand <mask2=reg128#3,<r7=reg128#10,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r7=%xmm9,>v10=%xmm15
vpand % xmm2, % xmm9, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r5 & mask3
# asm 1: vpand <mask3=reg128#4,<r5=reg128#8,>v01=reg128#8
# asm 2: vpand <mask3=%xmm3,<r5=%xmm7,>v01=%xmm7
vpand % xmm3, % xmm7, % xmm7

# qhasm: v11 = r7 & mask3
# asm 1: vpand <mask3=reg128#4,<r7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<r7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#8
# asm 2: psrlq $2,<v01=%xmm7
psrlq $2, % xmm7

# qhasm: r5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>r5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>r5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>r7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>r7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = r0 & mask4
# asm 1: vpand <mask4=reg128#5,<r0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<r0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: v10 = r1 & mask4
# asm 1: vpand <mask4=reg128#5,<r1=reg128#15,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r1=%xmm14,>v10=%xmm15
vpand % xmm4, % xmm14, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r0 & mask5
# asm 1: vpand <mask5=reg128#6,<r0=reg128#14,>v01=reg128#14
# asm 2: vpand <mask5=%xmm5,<r0=%xmm13,>v01=%xmm13
vpand % xmm5, % xmm13, % xmm13

# qhasm: v11 = r1 & mask5
# asm 1: vpand <mask5=reg128#6,<r1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<r1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#14
# asm 2: psrlq $1,<v01=%xmm13
psrlq $1, % xmm13

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>r0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>r0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: r1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>r1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>r1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = r2 & mask4
# asm 1: vpand <mask4=reg128#5,<r2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<r2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: v10 = r3 & mask4
# asm 1: vpand <mask4=reg128#5,<r3=reg128#11,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r3=%xmm10,>v10=%xmm15
vpand % xmm4, % xmm10, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r2 & mask5
# asm 1: vpand <mask5=reg128#6,<r2=reg128#12,>v01=reg128#12
# asm 2: vpand <mask5=%xmm5,<r2=%xmm11,>v01=%xmm11
vpand % xmm5, % xmm11, % xmm11

# qhasm: v11 = r3 & mask5
# asm 1: vpand <mask5=reg128#6,<r3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<r3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#12
# asm 2: psrlq $1,<v01=%xmm11
psrlq $1, % xmm11

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>r3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>r3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = r4 & mask4
# asm 1: vpand <mask4=reg128#5,<r4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<r4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: v10 = r5 & mask4
# asm 1: vpand <mask4=reg128#5,<r5=reg128#9,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r5=%xmm8,>v10=%xmm15
vpand % xmm4, % xmm8, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r4 & mask5
# asm 1: vpand <mask5=reg128#6,<r4=reg128#13,>v01=reg128#13
# asm 2: vpand <mask5=%xmm5,<r4=%xmm12,>v01=%xmm12
vpand % xmm5, % xmm12, % xmm12

# qhasm: v11 = r5 & mask5
# asm 1: vpand <mask5=reg128#6,<r5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<r5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#13
# asm 2: psrlq $1,<v01=%xmm12
psrlq $1, % xmm12

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>r5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>r5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = r6 & mask4
# asm 1: vpand <mask4=reg128#5,<r6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<r6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: v10 = r7 & mask4
# asm 1: vpand <mask4=reg128#5,<r7=reg128#8,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r7=%xmm7,>v10=%xmm15
vpand % xmm4, % xmm7, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r6 & mask5
# asm 1: vpand <mask5=reg128#6,<r6=reg128#7,>v01=reg128#7
# asm 2: vpand <mask5=%xmm5,<r6=%xmm6,>v01=%xmm6
vpand % xmm5, % xmm6, % xmm6

# qhasm: v11 = r7 & mask5
# asm 1: vpand <mask5=reg128#6,<r7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<r7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#7
# asm 2: psrlq $1,<v01=%xmm6
psrlq $1, % xmm6

# qhasm: r6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>r7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>r7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: t0 = r0[0]r1[0]
# asm 1: vpunpcklqdq <r1=reg128#14,<r0=reg128#10,>t0=reg128#8
# asm 2: vpunpcklqdq <r1=%xmm13,<r0=%xmm9,>t0=%xmm7
vpunpcklqdq % xmm13, % xmm9, % xmm7

# qhasm: mem128[ input_0 + 128 ] = t0
# asm 1: movdqu   <t0=reg128#8,128(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,128(<input_0=%rdi)
movdqu   % xmm7, 128( % rdi)

# qhasm: t0 = r2[0]r3[0]
# asm 1: vpunpcklqdq <r3=reg128#11,<r2=reg128#15,>t0=reg128#8
# asm 2: vpunpcklqdq <r3=%xmm10,<r2=%xmm14,>t0=%xmm7
vpunpcklqdq % xmm10, % xmm14, % xmm7

# qhasm: mem128[ input_0 + 144 ] = t0
# asm 1: movdqu   <t0=reg128#8,144(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,144(<input_0=%rdi)
movdqu   % xmm7, 144( % rdi)

# qhasm: t0 = r4[0]r5[0]
# asm 1: vpunpcklqdq <r5=reg128#9,<r4=reg128#12,>t0=reg128#8
# asm 2: vpunpcklqdq <r5=%xmm8,<r4=%xmm11,>t0=%xmm7
vpunpcklqdq % xmm8, % xmm11, % xmm7

# qhasm: mem128[ input_0 + 160 ] = t0
# asm 1: movdqu   <t0=reg128#8,160(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,160(<input_0=%rdi)
movdqu   % xmm7, 160( % rdi)

# qhasm: t0 = r6[0]r7[0]
# asm 1: vpunpcklqdq <r7=reg128#7,<r6=reg128#13,>t0=reg128#7
# asm 2: vpunpcklqdq <r7=%xmm6,<r6=%xmm12,>t0=%xmm6
vpunpcklqdq % xmm6, % xmm12, % xmm6

# qhasm: mem128[ input_0 + 176 ] = t0
# asm 1: movdqu   <t0=reg128#7,176(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm6,176(<input_0=%rdi)
movdqu   % xmm6, 176( % rdi)

# qhasm: r0 = mem64[ input_0 + 192 ] x2
# asm 1: movddup 192(<input_0=int64#1),>r0=reg128#7
# asm 2: movddup 192(<input_0=%rdi),>r0=%xmm6
movddup 192( % rdi), % xmm6

# qhasm: r1 = mem64[ input_0 + 200 ] x2
# asm 1: movddup 200(<input_0=int64#1),>r1=reg128#8
# asm 2: movddup 200(<input_0=%rdi),>r1=%xmm7
movddup 200( % rdi), % xmm7

# qhasm: r2 = mem64[ input_0 + 208 ] x2
# asm 1: movddup 208(<input_0=int64#1),>r2=reg128#9
# asm 2: movddup 208(<input_0=%rdi),>r2=%xmm8
movddup 208( % rdi), % xmm8

# qhasm: r3 = mem64[ input_0 + 216 ] x2
# asm 1: movddup 216(<input_0=int64#1),>r3=reg128#10
# asm 2: movddup 216(<input_0=%rdi),>r3=%xmm9
movddup 216( % rdi), % xmm9

# qhasm: r4 = mem64[ input_0 + 224 ] x2
# asm 1: movddup 224(<input_0=int64#1),>r4=reg128#11
# asm 2: movddup 224(<input_0=%rdi),>r4=%xmm10
movddup 224( % rdi), % xmm10

# qhasm: r5 = mem64[ input_0 + 232 ] x2
# asm 1: movddup 232(<input_0=int64#1),>r5=reg128#12
# asm 2: movddup 232(<input_0=%rdi),>r5=%xmm11
movddup 232( % rdi), % xmm11

# qhasm: r6 = mem64[ input_0 + 240 ] x2
# asm 1: movddup 240(<input_0=int64#1),>r6=reg128#13
# asm 2: movddup 240(<input_0=%rdi),>r6=%xmm12
movddup 240( % rdi), % xmm12

# qhasm: r7 = mem64[ input_0 + 248 ] x2
# asm 1: movddup 248(<input_0=int64#1),>r7=reg128#14
# asm 2: movddup 248(<input_0=%rdi),>r7=%xmm13
movddup 248( % rdi), % xmm13

# qhasm: v00 = r0 & mask0
# asm 1: vpand <mask0=reg128#1,<r0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<r0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: v10 = r4 & mask0
# asm 1: vpand <mask0=reg128#1,<r4=reg128#11,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r4=%xmm10,>v10=%xmm15
vpand % xmm0, % xmm10, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r0 & mask1
# asm 1: vpand <mask1=reg128#2,<r0=reg128#7,>v01=reg128#7
# asm 2: vpand <mask1=%xmm1,<r0=%xmm6,>v01=%xmm6
vpand % xmm1, % xmm6, % xmm6

# qhasm: v11 = r4 & mask1
# asm 1: vpand <mask1=reg128#2,<r4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<r4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#7
# asm 2: psrlq $4,<v01=%xmm6
psrlq $4, % xmm6

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>r4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>r4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = r1 & mask0
# asm 1: vpand <mask0=reg128#1,<r1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<r1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: v10 = r5 & mask0
# asm 1: vpand <mask0=reg128#1,<r5=reg128#12,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r5=%xmm11,>v10=%xmm15
vpand % xmm0, % xmm11, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r1 & mask1
# asm 1: vpand <mask1=reg128#2,<r1=reg128#8,>v01=reg128#8
# asm 2: vpand <mask1=%xmm1,<r1=%xmm7,>v01=%xmm7
vpand % xmm1, % xmm7, % xmm7

# qhasm: v11 = r5 & mask1
# asm 1: vpand <mask1=reg128#2,<r5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<r5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#8
# asm 2: psrlq $4,<v01=%xmm7
psrlq $4, % xmm7

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>r1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>r1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>r5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>r5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = r2 & mask0
# asm 1: vpand <mask0=reg128#1,<r2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<r2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: v10 = r6 & mask0
# asm 1: vpand <mask0=reg128#1,<r6=reg128#13,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r6=%xmm12,>v10=%xmm15
vpand % xmm0, % xmm12, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r2 & mask1
# asm 1: vpand <mask1=reg128#2,<r2=reg128#9,>v01=reg128#9
# asm 2: vpand <mask1=%xmm1,<r2=%xmm8,>v01=%xmm8
vpand % xmm1, % xmm8, % xmm8

# qhasm: v11 = r6 & mask1
# asm 1: vpand <mask1=reg128#2,<r6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<r6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#9
# asm 2: psrlq $4,<v01=%xmm8
psrlq $4, % xmm8

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>r6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>r6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = r3 & mask0
# asm 1: vpand <mask0=reg128#1,<r3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<r3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: v10 = r7 & mask0
# asm 1: vpand <mask0=reg128#1,<r7=reg128#14,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r7=%xmm13,>v10=%xmm15
vpand % xmm0, % xmm13, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r3 & mask1
# asm 1: vpand <mask1=reg128#2,<r3=reg128#10,>v01=reg128#10
# asm 2: vpand <mask1=%xmm1,<r3=%xmm9,>v01=%xmm9
vpand % xmm1, % xmm9, % xmm9

# qhasm: v11 = r7 & mask1
# asm 1: vpand <mask1=reg128#2,<r7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<r7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#10
# asm 2: psrlq $4,<v01=%xmm9
psrlq $4, % xmm9

# qhasm: r3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>r7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>r7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = r0 & mask2
# asm 1: vpand <mask2=reg128#3,<r0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<r0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: v10 = r2 & mask2
# asm 1: vpand <mask2=reg128#3,<r2=reg128#12,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r2=%xmm11,>v10=%xmm15
vpand % xmm2, % xmm11, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r0 & mask3
# asm 1: vpand <mask3=reg128#4,<r0=reg128#15,>v01=reg128#15
# asm 2: vpand <mask3=%xmm3,<r0=%xmm14,>v01=%xmm14
vpand % xmm3, % xmm14, % xmm14

# qhasm: v11 = r2 & mask3
# asm 1: vpand <mask3=reg128#4,<r2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<r2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#15
# asm 2: psrlq $2,<v01=%xmm14
psrlq $2, % xmm14

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>r0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>r0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: r2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>r2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>r2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = r1 & mask2
# asm 1: vpand <mask2=reg128#3,<r1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<r1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: v10 = r3 & mask2
# asm 1: vpand <mask2=reg128#3,<r3=reg128#13,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r3=%xmm12,>v10=%xmm15
vpand % xmm2, % xmm12, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r1 & mask3
# asm 1: vpand <mask3=reg128#4,<r1=reg128#11,>v01=reg128#11
# asm 2: vpand <mask3=%xmm3,<r1=%xmm10,>v01=%xmm10
vpand % xmm3, % xmm10, % xmm10

# qhasm: v11 = r3 & mask3
# asm 1: vpand <mask3=reg128#4,<r3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<r3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#11
# asm 2: psrlq $2,<v01=%xmm10
psrlq $2, % xmm10

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>r3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>r3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = r4 & mask2
# asm 1: vpand <mask2=reg128#3,<r4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<r4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: v10 = r6 & mask2
# asm 1: vpand <mask2=reg128#3,<r6=reg128#9,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r6=%xmm8,>v10=%xmm15
vpand % xmm2, % xmm8, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r4 & mask3
# asm 1: vpand <mask3=reg128#4,<r4=reg128#7,>v01=reg128#7
# asm 2: vpand <mask3=%xmm3,<r4=%xmm6,>v01=%xmm6
vpand % xmm3, % xmm6, % xmm6

# qhasm: v11 = r6 & mask3
# asm 1: vpand <mask3=reg128#4,<r6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<r6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#7
# asm 2: psrlq $2,<v01=%xmm6
psrlq $2, % xmm6

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>r6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>r6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = r5 & mask2
# asm 1: vpand <mask2=reg128#3,<r5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<r5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: v10 = r7 & mask2
# asm 1: vpand <mask2=reg128#3,<r7=reg128#10,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r7=%xmm9,>v10=%xmm15
vpand % xmm2, % xmm9, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r5 & mask3
# asm 1: vpand <mask3=reg128#4,<r5=reg128#8,>v01=reg128#8
# asm 2: vpand <mask3=%xmm3,<r5=%xmm7,>v01=%xmm7
vpand % xmm3, % xmm7, % xmm7

# qhasm: v11 = r7 & mask3
# asm 1: vpand <mask3=reg128#4,<r7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<r7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#8
# asm 2: psrlq $2,<v01=%xmm7
psrlq $2, % xmm7

# qhasm: r5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>r5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>r5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>r7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>r7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = r0 & mask4
# asm 1: vpand <mask4=reg128#5,<r0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<r0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: v10 = r1 & mask4
# asm 1: vpand <mask4=reg128#5,<r1=reg128#15,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r1=%xmm14,>v10=%xmm15
vpand % xmm4, % xmm14, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r0 & mask5
# asm 1: vpand <mask5=reg128#6,<r0=reg128#14,>v01=reg128#14
# asm 2: vpand <mask5=%xmm5,<r0=%xmm13,>v01=%xmm13
vpand % xmm5, % xmm13, % xmm13

# qhasm: v11 = r1 & mask5
# asm 1: vpand <mask5=reg128#6,<r1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<r1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#14
# asm 2: psrlq $1,<v01=%xmm13
psrlq $1, % xmm13

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>r0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>r0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: r1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>r1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>r1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = r2 & mask4
# asm 1: vpand <mask4=reg128#5,<r2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<r2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: v10 = r3 & mask4
# asm 1: vpand <mask4=reg128#5,<r3=reg128#11,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r3=%xmm10,>v10=%xmm15
vpand % xmm4, % xmm10, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r2 & mask5
# asm 1: vpand <mask5=reg128#6,<r2=reg128#12,>v01=reg128#12
# asm 2: vpand <mask5=%xmm5,<r2=%xmm11,>v01=%xmm11
vpand % xmm5, % xmm11, % xmm11

# qhasm: v11 = r3 & mask5
# asm 1: vpand <mask5=reg128#6,<r3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<r3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#12
# asm 2: psrlq $1,<v01=%xmm11
psrlq $1, % xmm11

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>r3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>r3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = r4 & mask4
# asm 1: vpand <mask4=reg128#5,<r4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<r4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: v10 = r5 & mask4
# asm 1: vpand <mask4=reg128#5,<r5=reg128#9,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r5=%xmm8,>v10=%xmm15
vpand % xmm4, % xmm8, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r4 & mask5
# asm 1: vpand <mask5=reg128#6,<r4=reg128#13,>v01=reg128#13
# asm 2: vpand <mask5=%xmm5,<r4=%xmm12,>v01=%xmm12
vpand % xmm5, % xmm12, % xmm12

# qhasm: v11 = r5 & mask5
# asm 1: vpand <mask5=reg128#6,<r5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<r5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#13
# asm 2: psrlq $1,<v01=%xmm12
psrlq $1, % xmm12

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>r5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>r5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = r6 & mask4
# asm 1: vpand <mask4=reg128#5,<r6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<r6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: v10 = r7 & mask4
# asm 1: vpand <mask4=reg128#5,<r7=reg128#8,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r7=%xmm7,>v10=%xmm15
vpand % xmm4, % xmm7, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r6 & mask5
# asm 1: vpand <mask5=reg128#6,<r6=reg128#7,>v01=reg128#7
# asm 2: vpand <mask5=%xmm5,<r6=%xmm6,>v01=%xmm6
vpand % xmm5, % xmm6, % xmm6

# qhasm: v11 = r7 & mask5
# asm 1: vpand <mask5=reg128#6,<r7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<r7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#7
# asm 2: psrlq $1,<v01=%xmm6
psrlq $1, % xmm6

# qhasm: r6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>r7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>r7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: t0 = r0[0]r1[0]
# asm 1: vpunpcklqdq <r1=reg128#14,<r0=reg128#10,>t0=reg128#8
# asm 2: vpunpcklqdq <r1=%xmm13,<r0=%xmm9,>t0=%xmm7
vpunpcklqdq % xmm13, % xmm9, % xmm7

# qhasm: mem128[ input_0 + 192 ] = t0
# asm 1: movdqu   <t0=reg128#8,192(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,192(<input_0=%rdi)
movdqu   % xmm7, 192( % rdi)

# qhasm: t0 = r2[0]r3[0]
# asm 1: vpunpcklqdq <r3=reg128#11,<r2=reg128#15,>t0=reg128#8
# asm 2: vpunpcklqdq <r3=%xmm10,<r2=%xmm14,>t0=%xmm7
vpunpcklqdq % xmm10, % xmm14, % xmm7

# qhasm: mem128[ input_0 + 208 ] = t0
# asm 1: movdqu   <t0=reg128#8,208(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,208(<input_0=%rdi)
movdqu   % xmm7, 208( % rdi)

# qhasm: t0 = r4[0]r5[0]
# asm 1: vpunpcklqdq <r5=reg128#9,<r4=reg128#12,>t0=reg128#8
# asm 2: vpunpcklqdq <r5=%xmm8,<r4=%xmm11,>t0=%xmm7
vpunpcklqdq % xmm8, % xmm11, % xmm7

# qhasm: mem128[ input_0 + 224 ] = t0
# asm 1: movdqu   <t0=reg128#8,224(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,224(<input_0=%rdi)
movdqu   % xmm7, 224( % rdi)

# qhasm: t0 = r6[0]r7[0]
# asm 1: vpunpcklqdq <r7=reg128#7,<r6=reg128#13,>t0=reg128#7
# asm 2: vpunpcklqdq <r7=%xmm6,<r6=%xmm12,>t0=%xmm6
vpunpcklqdq % xmm6, % xmm12, % xmm6

# qhasm: mem128[ input_0 + 240 ] = t0
# asm 1: movdqu   <t0=reg128#7,240(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm6,240(<input_0=%rdi)
movdqu   % xmm6, 240( % rdi)

# qhasm: r0 = mem64[ input_0 + 256 ] x2
# asm 1: movddup 256(<input_0=int64#1),>r0=reg128#7
# asm 2: movddup 256(<input_0=%rdi),>r0=%xmm6
movddup 256( % rdi), % xmm6

# qhasm: r1 = mem64[ input_0 + 264 ] x2
# asm 1: movddup 264(<input_0=int64#1),>r1=reg128#8
# asm 2: movddup 264(<input_0=%rdi),>r1=%xmm7
movddup 264( % rdi), % xmm7

# qhasm: r2 = mem64[ input_0 + 272 ] x2
# asm 1: movddup 272(<input_0=int64#1),>r2=reg128#9
# asm 2: movddup 272(<input_0=%rdi),>r2=%xmm8
movddup 272( % rdi), % xmm8

# qhasm: r3 = mem64[ input_0 + 280 ] x2
# asm 1: movddup 280(<input_0=int64#1),>r3=reg128#10
# asm 2: movddup 280(<input_0=%rdi),>r3=%xmm9
movddup 280( % rdi), % xmm9

# qhasm: r4 = mem64[ input_0 + 288 ] x2
# asm 1: movddup 288(<input_0=int64#1),>r4=reg128#11
# asm 2: movddup 288(<input_0=%rdi),>r4=%xmm10
movddup 288( % rdi), % xmm10

# qhasm: r5 = mem64[ input_0 + 296 ] x2
# asm 1: movddup 296(<input_0=int64#1),>r5=reg128#12
# asm 2: movddup 296(<input_0=%rdi),>r5=%xmm11
movddup 296( % rdi), % xmm11

# qhasm: r6 = mem64[ input_0 + 304 ] x2
# asm 1: movddup 304(<input_0=int64#1),>r6=reg128#13
# asm 2: movddup 304(<input_0=%rdi),>r6=%xmm12
movddup 304( % rdi), % xmm12

# qhasm: r7 = mem64[ input_0 + 312 ] x2
# asm 1: movddup 312(<input_0=int64#1),>r7=reg128#14
# asm 2: movddup 312(<input_0=%rdi),>r7=%xmm13
movddup 312( % rdi), % xmm13

# qhasm: v00 = r0 & mask0
# asm 1: vpand <mask0=reg128#1,<r0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<r0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: v10 = r4 & mask0
# asm 1: vpand <mask0=reg128#1,<r4=reg128#11,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r4=%xmm10,>v10=%xmm15
vpand % xmm0, % xmm10, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r0 & mask1
# asm 1: vpand <mask1=reg128#2,<r0=reg128#7,>v01=reg128#7
# asm 2: vpand <mask1=%xmm1,<r0=%xmm6,>v01=%xmm6
vpand % xmm1, % xmm6, % xmm6

# qhasm: v11 = r4 & mask1
# asm 1: vpand <mask1=reg128#2,<r4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<r4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#7
# asm 2: psrlq $4,<v01=%xmm6
psrlq $4, % xmm6

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>r4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>r4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = r1 & mask0
# asm 1: vpand <mask0=reg128#1,<r1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<r1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: v10 = r5 & mask0
# asm 1: vpand <mask0=reg128#1,<r5=reg128#12,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r5=%xmm11,>v10=%xmm15
vpand % xmm0, % xmm11, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r1 & mask1
# asm 1: vpand <mask1=reg128#2,<r1=reg128#8,>v01=reg128#8
# asm 2: vpand <mask1=%xmm1,<r1=%xmm7,>v01=%xmm7
vpand % xmm1, % xmm7, % xmm7

# qhasm: v11 = r5 & mask1
# asm 1: vpand <mask1=reg128#2,<r5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<r5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#8
# asm 2: psrlq $4,<v01=%xmm7
psrlq $4, % xmm7

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>r1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>r1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>r5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>r5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = r2 & mask0
# asm 1: vpand <mask0=reg128#1,<r2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<r2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: v10 = r6 & mask0
# asm 1: vpand <mask0=reg128#1,<r6=reg128#13,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r6=%xmm12,>v10=%xmm15
vpand % xmm0, % xmm12, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r2 & mask1
# asm 1: vpand <mask1=reg128#2,<r2=reg128#9,>v01=reg128#9
# asm 2: vpand <mask1=%xmm1,<r2=%xmm8,>v01=%xmm8
vpand % xmm1, % xmm8, % xmm8

# qhasm: v11 = r6 & mask1
# asm 1: vpand <mask1=reg128#2,<r6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<r6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#9
# asm 2: psrlq $4,<v01=%xmm8
psrlq $4, % xmm8

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>r6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>r6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = r3 & mask0
# asm 1: vpand <mask0=reg128#1,<r3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<r3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: v10 = r7 & mask0
# asm 1: vpand <mask0=reg128#1,<r7=reg128#14,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r7=%xmm13,>v10=%xmm15
vpand % xmm0, % xmm13, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r3 & mask1
# asm 1: vpand <mask1=reg128#2,<r3=reg128#10,>v01=reg128#10
# asm 2: vpand <mask1=%xmm1,<r3=%xmm9,>v01=%xmm9
vpand % xmm1, % xmm9, % xmm9

# qhasm: v11 = r7 & mask1
# asm 1: vpand <mask1=reg128#2,<r7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<r7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#10
# asm 2: psrlq $4,<v01=%xmm9
psrlq $4, % xmm9

# qhasm: r3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>r7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>r7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = r0 & mask2
# asm 1: vpand <mask2=reg128#3,<r0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<r0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: v10 = r2 & mask2
# asm 1: vpand <mask2=reg128#3,<r2=reg128#12,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r2=%xmm11,>v10=%xmm15
vpand % xmm2, % xmm11, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r0 & mask3
# asm 1: vpand <mask3=reg128#4,<r0=reg128#15,>v01=reg128#15
# asm 2: vpand <mask3=%xmm3,<r0=%xmm14,>v01=%xmm14
vpand % xmm3, % xmm14, % xmm14

# qhasm: v11 = r2 & mask3
# asm 1: vpand <mask3=reg128#4,<r2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<r2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#15
# asm 2: psrlq $2,<v01=%xmm14
psrlq $2, % xmm14

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>r0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>r0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: r2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>r2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>r2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = r1 & mask2
# asm 1: vpand <mask2=reg128#3,<r1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<r1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: v10 = r3 & mask2
# asm 1: vpand <mask2=reg128#3,<r3=reg128#13,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r3=%xmm12,>v10=%xmm15
vpand % xmm2, % xmm12, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r1 & mask3
# asm 1: vpand <mask3=reg128#4,<r1=reg128#11,>v01=reg128#11
# asm 2: vpand <mask3=%xmm3,<r1=%xmm10,>v01=%xmm10
vpand % xmm3, % xmm10, % xmm10

# qhasm: v11 = r3 & mask3
# asm 1: vpand <mask3=reg128#4,<r3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<r3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#11
# asm 2: psrlq $2,<v01=%xmm10
psrlq $2, % xmm10

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>r3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>r3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = r4 & mask2
# asm 1: vpand <mask2=reg128#3,<r4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<r4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: v10 = r6 & mask2
# asm 1: vpand <mask2=reg128#3,<r6=reg128#9,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r6=%xmm8,>v10=%xmm15
vpand % xmm2, % xmm8, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r4 & mask3
# asm 1: vpand <mask3=reg128#4,<r4=reg128#7,>v01=reg128#7
# asm 2: vpand <mask3=%xmm3,<r4=%xmm6,>v01=%xmm6
vpand % xmm3, % xmm6, % xmm6

# qhasm: v11 = r6 & mask3
# asm 1: vpand <mask3=reg128#4,<r6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<r6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#7
# asm 2: psrlq $2,<v01=%xmm6
psrlq $2, % xmm6

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>r6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>r6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = r5 & mask2
# asm 1: vpand <mask2=reg128#3,<r5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<r5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: v10 = r7 & mask2
# asm 1: vpand <mask2=reg128#3,<r7=reg128#10,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r7=%xmm9,>v10=%xmm15
vpand % xmm2, % xmm9, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r5 & mask3
# asm 1: vpand <mask3=reg128#4,<r5=reg128#8,>v01=reg128#8
# asm 2: vpand <mask3=%xmm3,<r5=%xmm7,>v01=%xmm7
vpand % xmm3, % xmm7, % xmm7

# qhasm: v11 = r7 & mask3
# asm 1: vpand <mask3=reg128#4,<r7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<r7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#8
# asm 2: psrlq $2,<v01=%xmm7
psrlq $2, % xmm7

# qhasm: r5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>r5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>r5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>r7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>r7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = r0 & mask4
# asm 1: vpand <mask4=reg128#5,<r0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<r0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: v10 = r1 & mask4
# asm 1: vpand <mask4=reg128#5,<r1=reg128#15,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r1=%xmm14,>v10=%xmm15
vpand % xmm4, % xmm14, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r0 & mask5
# asm 1: vpand <mask5=reg128#6,<r0=reg128#14,>v01=reg128#14
# asm 2: vpand <mask5=%xmm5,<r0=%xmm13,>v01=%xmm13
vpand % xmm5, % xmm13, % xmm13

# qhasm: v11 = r1 & mask5
# asm 1: vpand <mask5=reg128#6,<r1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<r1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#14
# asm 2: psrlq $1,<v01=%xmm13
psrlq $1, % xmm13

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>r0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>r0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: r1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>r1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>r1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = r2 & mask4
# asm 1: vpand <mask4=reg128#5,<r2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<r2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: v10 = r3 & mask4
# asm 1: vpand <mask4=reg128#5,<r3=reg128#11,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r3=%xmm10,>v10=%xmm15
vpand % xmm4, % xmm10, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r2 & mask5
# asm 1: vpand <mask5=reg128#6,<r2=reg128#12,>v01=reg128#12
# asm 2: vpand <mask5=%xmm5,<r2=%xmm11,>v01=%xmm11
vpand % xmm5, % xmm11, % xmm11

# qhasm: v11 = r3 & mask5
# asm 1: vpand <mask5=reg128#6,<r3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<r3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#12
# asm 2: psrlq $1,<v01=%xmm11
psrlq $1, % xmm11

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>r3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>r3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = r4 & mask4
# asm 1: vpand <mask4=reg128#5,<r4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<r4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: v10 = r5 & mask4
# asm 1: vpand <mask4=reg128#5,<r5=reg128#9,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r5=%xmm8,>v10=%xmm15
vpand % xmm4, % xmm8, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r4 & mask5
# asm 1: vpand <mask5=reg128#6,<r4=reg128#13,>v01=reg128#13
# asm 2: vpand <mask5=%xmm5,<r4=%xmm12,>v01=%xmm12
vpand % xmm5, % xmm12, % xmm12

# qhasm: v11 = r5 & mask5
# asm 1: vpand <mask5=reg128#6,<r5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<r5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#13
# asm 2: psrlq $1,<v01=%xmm12
psrlq $1, % xmm12

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>r5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>r5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = r6 & mask4
# asm 1: vpand <mask4=reg128#5,<r6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<r6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: v10 = r7 & mask4
# asm 1: vpand <mask4=reg128#5,<r7=reg128#8,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r7=%xmm7,>v10=%xmm15
vpand % xmm4, % xmm7, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r6 & mask5
# asm 1: vpand <mask5=reg128#6,<r6=reg128#7,>v01=reg128#7
# asm 2: vpand <mask5=%xmm5,<r6=%xmm6,>v01=%xmm6
vpand % xmm5, % xmm6, % xmm6

# qhasm: v11 = r7 & mask5
# asm 1: vpand <mask5=reg128#6,<r7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<r7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#7
# asm 2: psrlq $1,<v01=%xmm6
psrlq $1, % xmm6

# qhasm: r6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>r7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>r7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: t0 = r0[0]r1[0]
# asm 1: vpunpcklqdq <r1=reg128#14,<r0=reg128#10,>t0=reg128#8
# asm 2: vpunpcklqdq <r1=%xmm13,<r0=%xmm9,>t0=%xmm7
vpunpcklqdq % xmm13, % xmm9, % xmm7

# qhasm: mem128[ input_0 + 256 ] = t0
# asm 1: movdqu   <t0=reg128#8,256(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,256(<input_0=%rdi)
movdqu   % xmm7, 256( % rdi)

# qhasm: t0 = r2[0]r3[0]
# asm 1: vpunpcklqdq <r3=reg128#11,<r2=reg128#15,>t0=reg128#8
# asm 2: vpunpcklqdq <r3=%xmm10,<r2=%xmm14,>t0=%xmm7
vpunpcklqdq % xmm10, % xmm14, % xmm7

# qhasm: mem128[ input_0 + 272 ] = t0
# asm 1: movdqu   <t0=reg128#8,272(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,272(<input_0=%rdi)
movdqu   % xmm7, 272( % rdi)

# qhasm: t0 = r4[0]r5[0]
# asm 1: vpunpcklqdq <r5=reg128#9,<r4=reg128#12,>t0=reg128#8
# asm 2: vpunpcklqdq <r5=%xmm8,<r4=%xmm11,>t0=%xmm7
vpunpcklqdq % xmm8, % xmm11, % xmm7

# qhasm: mem128[ input_0 + 288 ] = t0
# asm 1: movdqu   <t0=reg128#8,288(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,288(<input_0=%rdi)
movdqu   % xmm7, 288( % rdi)

# qhasm: t0 = r6[0]r7[0]
# asm 1: vpunpcklqdq <r7=reg128#7,<r6=reg128#13,>t0=reg128#7
# asm 2: vpunpcklqdq <r7=%xmm6,<r6=%xmm12,>t0=%xmm6
vpunpcklqdq % xmm6, % xmm12, % xmm6

# qhasm: mem128[ input_0 + 304 ] = t0
# asm 1: movdqu   <t0=reg128#7,304(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm6,304(<input_0=%rdi)
movdqu   % xmm6, 304( % rdi)

# qhasm: r0 = mem64[ input_0 + 320 ] x2
# asm 1: movddup 320(<input_0=int64#1),>r0=reg128#7
# asm 2: movddup 320(<input_0=%rdi),>r0=%xmm6
movddup 320( % rdi), % xmm6

# qhasm: r1 = mem64[ input_0 + 328 ] x2
# asm 1: movddup 328(<input_0=int64#1),>r1=reg128#8
# asm 2: movddup 328(<input_0=%rdi),>r1=%xmm7
movddup 328( % rdi), % xmm7

# qhasm: r2 = mem64[ input_0 + 336 ] x2
# asm 1: movddup 336(<input_0=int64#1),>r2=reg128#9
# asm 2: movddup 336(<input_0=%rdi),>r2=%xmm8
movddup 336( % rdi), % xmm8

# qhasm: r3 = mem64[ input_0 + 344 ] x2
# asm 1: movddup 344(<input_0=int64#1),>r3=reg128#10
# asm 2: movddup 344(<input_0=%rdi),>r3=%xmm9
movddup 344( % rdi), % xmm9

# qhasm: r4 = mem64[ input_0 + 352 ] x2
# asm 1: movddup 352(<input_0=int64#1),>r4=reg128#11
# asm 2: movddup 352(<input_0=%rdi),>r4=%xmm10
movddup 352( % rdi), % xmm10

# qhasm: r5 = mem64[ input_0 + 360 ] x2
# asm 1: movddup 360(<input_0=int64#1),>r5=reg128#12
# asm 2: movddup 360(<input_0=%rdi),>r5=%xmm11
movddup 360( % rdi), % xmm11

# qhasm: r6 = mem64[ input_0 + 368 ] x2
# asm 1: movddup 368(<input_0=int64#1),>r6=reg128#13
# asm 2: movddup 368(<input_0=%rdi),>r6=%xmm12
movddup 368( % rdi), % xmm12

# qhasm: r7 = mem64[ input_0 + 376 ] x2
# asm 1: movddup 376(<input_0=int64#1),>r7=reg128#14
# asm 2: movddup 376(<input_0=%rdi),>r7=%xmm13
movddup 376( % rdi), % xmm13

# qhasm: v00 = r0 & mask0
# asm 1: vpand <mask0=reg128#1,<r0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<r0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: v10 = r4 & mask0
# asm 1: vpand <mask0=reg128#1,<r4=reg128#11,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r4=%xmm10,>v10=%xmm15
vpand % xmm0, % xmm10, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r0 & mask1
# asm 1: vpand <mask1=reg128#2,<r0=reg128#7,>v01=reg128#7
# asm 2: vpand <mask1=%xmm1,<r0=%xmm6,>v01=%xmm6
vpand % xmm1, % xmm6, % xmm6

# qhasm: v11 = r4 & mask1
# asm 1: vpand <mask1=reg128#2,<r4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<r4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#7
# asm 2: psrlq $4,<v01=%xmm6
psrlq $4, % xmm6

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>r4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>r4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = r1 & mask0
# asm 1: vpand <mask0=reg128#1,<r1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<r1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: v10 = r5 & mask0
# asm 1: vpand <mask0=reg128#1,<r5=reg128#12,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r5=%xmm11,>v10=%xmm15
vpand % xmm0, % xmm11, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r1 & mask1
# asm 1: vpand <mask1=reg128#2,<r1=reg128#8,>v01=reg128#8
# asm 2: vpand <mask1=%xmm1,<r1=%xmm7,>v01=%xmm7
vpand % xmm1, % xmm7, % xmm7

# qhasm: v11 = r5 & mask1
# asm 1: vpand <mask1=reg128#2,<r5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<r5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#8
# asm 2: psrlq $4,<v01=%xmm7
psrlq $4, % xmm7

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>r1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>r1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>r5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>r5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = r2 & mask0
# asm 1: vpand <mask0=reg128#1,<r2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<r2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: v10 = r6 & mask0
# asm 1: vpand <mask0=reg128#1,<r6=reg128#13,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r6=%xmm12,>v10=%xmm15
vpand % xmm0, % xmm12, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r2 & mask1
# asm 1: vpand <mask1=reg128#2,<r2=reg128#9,>v01=reg128#9
# asm 2: vpand <mask1=%xmm1,<r2=%xmm8,>v01=%xmm8
vpand % xmm1, % xmm8, % xmm8

# qhasm: v11 = r6 & mask1
# asm 1: vpand <mask1=reg128#2,<r6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<r6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#9
# asm 2: psrlq $4,<v01=%xmm8
psrlq $4, % xmm8

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>r6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>r6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = r3 & mask0
# asm 1: vpand <mask0=reg128#1,<r3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<r3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: v10 = r7 & mask0
# asm 1: vpand <mask0=reg128#1,<r7=reg128#14,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r7=%xmm13,>v10=%xmm15
vpand % xmm0, % xmm13, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r3 & mask1
# asm 1: vpand <mask1=reg128#2,<r3=reg128#10,>v01=reg128#10
# asm 2: vpand <mask1=%xmm1,<r3=%xmm9,>v01=%xmm9
vpand % xmm1, % xmm9, % xmm9

# qhasm: v11 = r7 & mask1
# asm 1: vpand <mask1=reg128#2,<r7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<r7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#10
# asm 2: psrlq $4,<v01=%xmm9
psrlq $4, % xmm9

# qhasm: r3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>r7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>r7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = r0 & mask2
# asm 1: vpand <mask2=reg128#3,<r0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<r0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: v10 = r2 & mask2
# asm 1: vpand <mask2=reg128#3,<r2=reg128#12,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r2=%xmm11,>v10=%xmm15
vpand % xmm2, % xmm11, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r0 & mask3
# asm 1: vpand <mask3=reg128#4,<r0=reg128#15,>v01=reg128#15
# asm 2: vpand <mask3=%xmm3,<r0=%xmm14,>v01=%xmm14
vpand % xmm3, % xmm14, % xmm14

# qhasm: v11 = r2 & mask3
# asm 1: vpand <mask3=reg128#4,<r2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<r2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#15
# asm 2: psrlq $2,<v01=%xmm14
psrlq $2, % xmm14

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>r0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>r0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: r2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>r2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>r2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = r1 & mask2
# asm 1: vpand <mask2=reg128#3,<r1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<r1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: v10 = r3 & mask2
# asm 1: vpand <mask2=reg128#3,<r3=reg128#13,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r3=%xmm12,>v10=%xmm15
vpand % xmm2, % xmm12, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r1 & mask3
# asm 1: vpand <mask3=reg128#4,<r1=reg128#11,>v01=reg128#11
# asm 2: vpand <mask3=%xmm3,<r1=%xmm10,>v01=%xmm10
vpand % xmm3, % xmm10, % xmm10

# qhasm: v11 = r3 & mask3
# asm 1: vpand <mask3=reg128#4,<r3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<r3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#11
# asm 2: psrlq $2,<v01=%xmm10
psrlq $2, % xmm10

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>r3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>r3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = r4 & mask2
# asm 1: vpand <mask2=reg128#3,<r4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<r4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: v10 = r6 & mask2
# asm 1: vpand <mask2=reg128#3,<r6=reg128#9,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r6=%xmm8,>v10=%xmm15
vpand % xmm2, % xmm8, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r4 & mask3
# asm 1: vpand <mask3=reg128#4,<r4=reg128#7,>v01=reg128#7
# asm 2: vpand <mask3=%xmm3,<r4=%xmm6,>v01=%xmm6
vpand % xmm3, % xmm6, % xmm6

# qhasm: v11 = r6 & mask3
# asm 1: vpand <mask3=reg128#4,<r6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<r6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#7
# asm 2: psrlq $2,<v01=%xmm6
psrlq $2, % xmm6

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>r6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>r6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = r5 & mask2
# asm 1: vpand <mask2=reg128#3,<r5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<r5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: v10 = r7 & mask2
# asm 1: vpand <mask2=reg128#3,<r7=reg128#10,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r7=%xmm9,>v10=%xmm15
vpand % xmm2, % xmm9, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r5 & mask3
# asm 1: vpand <mask3=reg128#4,<r5=reg128#8,>v01=reg128#8
# asm 2: vpand <mask3=%xmm3,<r5=%xmm7,>v01=%xmm7
vpand % xmm3, % xmm7, % xmm7

# qhasm: v11 = r7 & mask3
# asm 1: vpand <mask3=reg128#4,<r7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<r7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#8
# asm 2: psrlq $2,<v01=%xmm7
psrlq $2, % xmm7

# qhasm: r5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>r5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>r5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>r7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>r7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = r0 & mask4
# asm 1: vpand <mask4=reg128#5,<r0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<r0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: v10 = r1 & mask4
# asm 1: vpand <mask4=reg128#5,<r1=reg128#15,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r1=%xmm14,>v10=%xmm15
vpand % xmm4, % xmm14, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r0 & mask5
# asm 1: vpand <mask5=reg128#6,<r0=reg128#14,>v01=reg128#14
# asm 2: vpand <mask5=%xmm5,<r0=%xmm13,>v01=%xmm13
vpand % xmm5, % xmm13, % xmm13

# qhasm: v11 = r1 & mask5
# asm 1: vpand <mask5=reg128#6,<r1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<r1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#14
# asm 2: psrlq $1,<v01=%xmm13
psrlq $1, % xmm13

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>r0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>r0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: r1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>r1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>r1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = r2 & mask4
# asm 1: vpand <mask4=reg128#5,<r2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<r2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: v10 = r3 & mask4
# asm 1: vpand <mask4=reg128#5,<r3=reg128#11,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r3=%xmm10,>v10=%xmm15
vpand % xmm4, % xmm10, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r2 & mask5
# asm 1: vpand <mask5=reg128#6,<r2=reg128#12,>v01=reg128#12
# asm 2: vpand <mask5=%xmm5,<r2=%xmm11,>v01=%xmm11
vpand % xmm5, % xmm11, % xmm11

# qhasm: v11 = r3 & mask5
# asm 1: vpand <mask5=reg128#6,<r3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<r3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#12
# asm 2: psrlq $1,<v01=%xmm11
psrlq $1, % xmm11

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>r3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>r3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = r4 & mask4
# asm 1: vpand <mask4=reg128#5,<r4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<r4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: v10 = r5 & mask4
# asm 1: vpand <mask4=reg128#5,<r5=reg128#9,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r5=%xmm8,>v10=%xmm15
vpand % xmm4, % xmm8, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r4 & mask5
# asm 1: vpand <mask5=reg128#6,<r4=reg128#13,>v01=reg128#13
# asm 2: vpand <mask5=%xmm5,<r4=%xmm12,>v01=%xmm12
vpand % xmm5, % xmm12, % xmm12

# qhasm: v11 = r5 & mask5
# asm 1: vpand <mask5=reg128#6,<r5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<r5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#13
# asm 2: psrlq $1,<v01=%xmm12
psrlq $1, % xmm12

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>r5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>r5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = r6 & mask4
# asm 1: vpand <mask4=reg128#5,<r6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<r6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: v10 = r7 & mask4
# asm 1: vpand <mask4=reg128#5,<r7=reg128#8,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r7=%xmm7,>v10=%xmm15
vpand % xmm4, % xmm7, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r6 & mask5
# asm 1: vpand <mask5=reg128#6,<r6=reg128#7,>v01=reg128#7
# asm 2: vpand <mask5=%xmm5,<r6=%xmm6,>v01=%xmm6
vpand % xmm5, % xmm6, % xmm6

# qhasm: v11 = r7 & mask5
# asm 1: vpand <mask5=reg128#6,<r7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<r7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#7
# asm 2: psrlq $1,<v01=%xmm6
psrlq $1, % xmm6

# qhasm: r6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>r7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>r7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: t0 = r0[0]r1[0]
# asm 1: vpunpcklqdq <r1=reg128#14,<r0=reg128#10,>t0=reg128#8
# asm 2: vpunpcklqdq <r1=%xmm13,<r0=%xmm9,>t0=%xmm7
vpunpcklqdq % xmm13, % xmm9, % xmm7

# qhasm: mem128[ input_0 + 320 ] = t0
# asm 1: movdqu   <t0=reg128#8,320(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,320(<input_0=%rdi)
movdqu   % xmm7, 320( % rdi)

# qhasm: t0 = r2[0]r3[0]
# asm 1: vpunpcklqdq <r3=reg128#11,<r2=reg128#15,>t0=reg128#8
# asm 2: vpunpcklqdq <r3=%xmm10,<r2=%xmm14,>t0=%xmm7
vpunpcklqdq % xmm10, % xmm14, % xmm7

# qhasm: mem128[ input_0 + 336 ] = t0
# asm 1: movdqu   <t0=reg128#8,336(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,336(<input_0=%rdi)
movdqu   % xmm7, 336( % rdi)

# qhasm: t0 = r4[0]r5[0]
# asm 1: vpunpcklqdq <r5=reg128#9,<r4=reg128#12,>t0=reg128#8
# asm 2: vpunpcklqdq <r5=%xmm8,<r4=%xmm11,>t0=%xmm7
vpunpcklqdq % xmm8, % xmm11, % xmm7

# qhasm: mem128[ input_0 + 352 ] = t0
# asm 1: movdqu   <t0=reg128#8,352(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,352(<input_0=%rdi)
movdqu   % xmm7, 352( % rdi)

# qhasm: t0 = r6[0]r7[0]
# asm 1: vpunpcklqdq <r7=reg128#7,<r6=reg128#13,>t0=reg128#7
# asm 2: vpunpcklqdq <r7=%xmm6,<r6=%xmm12,>t0=%xmm6
vpunpcklqdq % xmm6, % xmm12, % xmm6

# qhasm: mem128[ input_0 + 368 ] = t0
# asm 1: movdqu   <t0=reg128#7,368(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm6,368(<input_0=%rdi)
movdqu   % xmm6, 368( % rdi)

# qhasm: r0 = mem64[ input_0 + 384 ] x2
# asm 1: movddup 384(<input_0=int64#1),>r0=reg128#7
# asm 2: movddup 384(<input_0=%rdi),>r0=%xmm6
movddup 384( % rdi), % xmm6

# qhasm: r1 = mem64[ input_0 + 392 ] x2
# asm 1: movddup 392(<input_0=int64#1),>r1=reg128#8
# asm 2: movddup 392(<input_0=%rdi),>r1=%xmm7
movddup 392( % rdi), % xmm7

# qhasm: r2 = mem64[ input_0 + 400 ] x2
# asm 1: movddup 400(<input_0=int64#1),>r2=reg128#9
# asm 2: movddup 400(<input_0=%rdi),>r2=%xmm8
movddup 400( % rdi), % xmm8

# qhasm: r3 = mem64[ input_0 + 408 ] x2
# asm 1: movddup 408(<input_0=int64#1),>r3=reg128#10
# asm 2: movddup 408(<input_0=%rdi),>r3=%xmm9
movddup 408( % rdi), % xmm9

# qhasm: r4 = mem64[ input_0 + 416 ] x2
# asm 1: movddup 416(<input_0=int64#1),>r4=reg128#11
# asm 2: movddup 416(<input_0=%rdi),>r4=%xmm10
movddup 416( % rdi), % xmm10

# qhasm: r5 = mem64[ input_0 + 424 ] x2
# asm 1: movddup 424(<input_0=int64#1),>r5=reg128#12
# asm 2: movddup 424(<input_0=%rdi),>r5=%xmm11
movddup 424( % rdi), % xmm11

# qhasm: r6 = mem64[ input_0 + 432 ] x2
# asm 1: movddup 432(<input_0=int64#1),>r6=reg128#13
# asm 2: movddup 432(<input_0=%rdi),>r6=%xmm12
movddup 432( % rdi), % xmm12

# qhasm: r7 = mem64[ input_0 + 440 ] x2
# asm 1: movddup 440(<input_0=int64#1),>r7=reg128#14
# asm 2: movddup 440(<input_0=%rdi),>r7=%xmm13
movddup 440( % rdi), % xmm13

# qhasm: v00 = r0 & mask0
# asm 1: vpand <mask0=reg128#1,<r0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<r0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: v10 = r4 & mask0
# asm 1: vpand <mask0=reg128#1,<r4=reg128#11,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r4=%xmm10,>v10=%xmm15
vpand % xmm0, % xmm10, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r0 & mask1
# asm 1: vpand <mask1=reg128#2,<r0=reg128#7,>v01=reg128#7
# asm 2: vpand <mask1=%xmm1,<r0=%xmm6,>v01=%xmm6
vpand % xmm1, % xmm6, % xmm6

# qhasm: v11 = r4 & mask1
# asm 1: vpand <mask1=reg128#2,<r4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<r4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#7
# asm 2: psrlq $4,<v01=%xmm6
psrlq $4, % xmm6

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>r4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>r4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = r1 & mask0
# asm 1: vpand <mask0=reg128#1,<r1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<r1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: v10 = r5 & mask0
# asm 1: vpand <mask0=reg128#1,<r5=reg128#12,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r5=%xmm11,>v10=%xmm15
vpand % xmm0, % xmm11, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r1 & mask1
# asm 1: vpand <mask1=reg128#2,<r1=reg128#8,>v01=reg128#8
# asm 2: vpand <mask1=%xmm1,<r1=%xmm7,>v01=%xmm7
vpand % xmm1, % xmm7, % xmm7

# qhasm: v11 = r5 & mask1
# asm 1: vpand <mask1=reg128#2,<r5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<r5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#8
# asm 2: psrlq $4,<v01=%xmm7
psrlq $4, % xmm7

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>r1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>r1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>r5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>r5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = r2 & mask0
# asm 1: vpand <mask0=reg128#1,<r2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<r2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: v10 = r6 & mask0
# asm 1: vpand <mask0=reg128#1,<r6=reg128#13,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r6=%xmm12,>v10=%xmm15
vpand % xmm0, % xmm12, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r2 & mask1
# asm 1: vpand <mask1=reg128#2,<r2=reg128#9,>v01=reg128#9
# asm 2: vpand <mask1=%xmm1,<r2=%xmm8,>v01=%xmm8
vpand % xmm1, % xmm8, % xmm8

# qhasm: v11 = r6 & mask1
# asm 1: vpand <mask1=reg128#2,<r6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<r6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#9
# asm 2: psrlq $4,<v01=%xmm8
psrlq $4, % xmm8

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>r6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>r6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = r3 & mask0
# asm 1: vpand <mask0=reg128#1,<r3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<r3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: v10 = r7 & mask0
# asm 1: vpand <mask0=reg128#1,<r7=reg128#14,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r7=%xmm13,>v10=%xmm15
vpand % xmm0, % xmm13, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r3 & mask1
# asm 1: vpand <mask1=reg128#2,<r3=reg128#10,>v01=reg128#10
# asm 2: vpand <mask1=%xmm1,<r3=%xmm9,>v01=%xmm9
vpand % xmm1, % xmm9, % xmm9

# qhasm: v11 = r7 & mask1
# asm 1: vpand <mask1=reg128#2,<r7=reg128#14,>v11=reg128#14
# asm 2: vpand <mask1=%xmm1,<r7=%xmm13,>v11=%xmm13
vpand % xmm1, % xmm13, % xmm13

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#10
# asm 2: psrlq $4,<v01=%xmm9
psrlq $4, % xmm9

# qhasm: r3 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r3=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r3=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#14,<v01=reg128#10,>r7=reg128#10
# asm 2: vpor <v11=%xmm13,<v01=%xmm9,>r7=%xmm9
vpor % xmm13, % xmm9, % xmm9

# qhasm: v00 = r0 & mask2
# asm 1: vpand <mask2=reg128#3,<r0=reg128#15,>v00=reg128#14
# asm 2: vpand <mask2=%xmm2,<r0=%xmm14,>v00=%xmm13
vpand % xmm2, % xmm14, % xmm13

# qhasm: v10 = r2 & mask2
# asm 1: vpand <mask2=reg128#3,<r2=reg128#12,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r2=%xmm11,>v10=%xmm15
vpand % xmm2, % xmm11, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r0 & mask3
# asm 1: vpand <mask3=reg128#4,<r0=reg128#15,>v01=reg128#15
# asm 2: vpand <mask3=%xmm3,<r0=%xmm14,>v01=%xmm14
vpand % xmm3, % xmm14, % xmm14

# qhasm: v11 = r2 & mask3
# asm 1: vpand <mask3=reg128#4,<r2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<r2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#15
# asm 2: psrlq $2,<v01=%xmm14
psrlq $2, % xmm14

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#14,>r0=reg128#14
# asm 2: vpor <v10=%xmm15,<v00=%xmm13,>r0=%xmm13
vpor % xmm15, % xmm13, % xmm13

# qhasm: r2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#15,>r2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm14,>r2=%xmm11
vpor % xmm11, % xmm14, % xmm11

# qhasm: v00 = r1 & mask2
# asm 1: vpand <mask2=reg128#3,<r1=reg128#11,>v00=reg128#15
# asm 2: vpand <mask2=%xmm2,<r1=%xmm10,>v00=%xmm14
vpand % xmm2, % xmm10, % xmm14

# qhasm: v10 = r3 & mask2
# asm 1: vpand <mask2=reg128#3,<r3=reg128#13,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r3=%xmm12,>v10=%xmm15
vpand % xmm2, % xmm12, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r1 & mask3
# asm 1: vpand <mask3=reg128#4,<r1=reg128#11,>v01=reg128#11
# asm 2: vpand <mask3=%xmm3,<r1=%xmm10,>v01=%xmm10
vpand % xmm3, % xmm10, % xmm10

# qhasm: v11 = r3 & mask3
# asm 1: vpand <mask3=reg128#4,<r3=reg128#13,>v11=reg128#13
# asm 2: vpand <mask3=%xmm3,<r3=%xmm12,>v11=%xmm12
vpand % xmm3, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#11
# asm 2: psrlq $2,<v01=%xmm10
psrlq $2, % xmm10

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r1=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r1=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#11,>r3=reg128#11
# asm 2: vpor <v11=%xmm12,<v01=%xmm10,>r3=%xmm10
vpor % xmm12, % xmm10, % xmm10

# qhasm: v00 = r4 & mask2
# asm 1: vpand <mask2=reg128#3,<r4=reg128#7,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<r4=%xmm6,>v00=%xmm12
vpand % xmm2, % xmm6, % xmm12

# qhasm: v10 = r6 & mask2
# asm 1: vpand <mask2=reg128#3,<r6=reg128#9,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r6=%xmm8,>v10=%xmm15
vpand % xmm2, % xmm8, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r4 & mask3
# asm 1: vpand <mask3=reg128#4,<r4=reg128#7,>v01=reg128#7
# asm 2: vpand <mask3=%xmm3,<r4=%xmm6,>v01=%xmm6
vpand % xmm3, % xmm6, % xmm6

# qhasm: v11 = r6 & mask3
# asm 1: vpand <mask3=reg128#4,<r6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<r6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#7
# asm 2: psrlq $2,<v01=%xmm6
psrlq $2, % xmm6

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r4=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r4=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>r6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>r6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = r5 & mask2
# asm 1: vpand <mask2=reg128#3,<r5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<r5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: v10 = r7 & mask2
# asm 1: vpand <mask2=reg128#3,<r7=reg128#10,>v10=reg128#16
# asm 2: vpand <mask2=%xmm2,<r7=%xmm9,>v10=%xmm15
vpand % xmm2, % xmm9, % xmm15

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#16
# asm 2: psllq $2,<v10=%xmm15
psllq $2, % xmm15

# qhasm: v01 = r5 & mask3
# asm 1: vpand <mask3=reg128#4,<r5=reg128#8,>v01=reg128#8
# asm 2: vpand <mask3=%xmm3,<r5=%xmm7,>v01=%xmm7
vpand % xmm3, % xmm7, % xmm7

# qhasm: v11 = r7 & mask3
# asm 1: vpand <mask3=reg128#4,<r7=reg128#10,>v11=reg128#10
# asm 2: vpand <mask3=%xmm3,<r7=%xmm9,>v11=%xmm9
vpand % xmm3, % xmm9, % xmm9

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#8
# asm 2: psrlq $2,<v01=%xmm7
psrlq $2, % xmm7

# qhasm: r5 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#9,>r5=reg128#9
# asm 2: vpor <v10=%xmm15,<v00=%xmm8,>r5=%xmm8
vpor % xmm15, % xmm8, % xmm8

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#8,>r7=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm7,>r7=%xmm7
vpor % xmm9, % xmm7, % xmm7

# qhasm: v00 = r0 & mask4
# asm 1: vpand <mask4=reg128#5,<r0=reg128#14,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<r0=%xmm13,>v00=%xmm9
vpand % xmm4, % xmm13, % xmm9

# qhasm: v10 = r1 & mask4
# asm 1: vpand <mask4=reg128#5,<r1=reg128#15,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r1=%xmm14,>v10=%xmm15
vpand % xmm4, % xmm14, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r0 & mask5
# asm 1: vpand <mask5=reg128#6,<r0=reg128#14,>v01=reg128#14
# asm 2: vpand <mask5=%xmm5,<r0=%xmm13,>v01=%xmm13
vpand % xmm5, % xmm13, % xmm13

# qhasm: v11 = r1 & mask5
# asm 1: vpand <mask5=reg128#6,<r1=reg128#15,>v11=reg128#15
# asm 2: vpand <mask5=%xmm5,<r1=%xmm14,>v11=%xmm14
vpand % xmm5, % xmm14, % xmm14

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#14
# asm 2: psrlq $1,<v01=%xmm13
psrlq $1, % xmm13

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#10,>r0=reg128#10
# asm 2: vpor <v10=%xmm15,<v00=%xmm9,>r0=%xmm9
vpor % xmm15, % xmm9, % xmm9

# qhasm: r1 = v01 | v11
# asm 1: vpor <v11=reg128#15,<v01=reg128#14,>r1=reg128#14
# asm 2: vpor <v11=%xmm14,<v01=%xmm13,>r1=%xmm13
vpor % xmm14, % xmm13, % xmm13

# qhasm: v00 = r2 & mask4
# asm 1: vpand <mask4=reg128#5,<r2=reg128#12,>v00=reg128#15
# asm 2: vpand <mask4=%xmm4,<r2=%xmm11,>v00=%xmm14
vpand % xmm4, % xmm11, % xmm14

# qhasm: v10 = r3 & mask4
# asm 1: vpand <mask4=reg128#5,<r3=reg128#11,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r3=%xmm10,>v10=%xmm15
vpand % xmm4, % xmm10, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r2 & mask5
# asm 1: vpand <mask5=reg128#6,<r2=reg128#12,>v01=reg128#12
# asm 2: vpand <mask5=%xmm5,<r2=%xmm11,>v01=%xmm11
vpand % xmm5, % xmm11, % xmm11

# qhasm: v11 = r3 & mask5
# asm 1: vpand <mask5=reg128#6,<r3=reg128#11,>v11=reg128#11
# asm 2: vpand <mask5=%xmm5,<r3=%xmm10,>v11=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#12
# asm 2: psrlq $1,<v01=%xmm11
psrlq $1, % xmm11

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r2=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r2=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#12,>r3=reg128#11
# asm 2: vpor <v11=%xmm10,<v01=%xmm11,>r3=%xmm10
vpor % xmm10, % xmm11, % xmm10

# qhasm: v00 = r4 & mask4
# asm 1: vpand <mask4=reg128#5,<r4=reg128#13,>v00=reg128#12
# asm 2: vpand <mask4=%xmm4,<r4=%xmm12,>v00=%xmm11
vpand % xmm4, % xmm12, % xmm11

# qhasm: v10 = r5 & mask4
# asm 1: vpand <mask4=reg128#5,<r5=reg128#9,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r5=%xmm8,>v10=%xmm15
vpand % xmm4, % xmm8, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r4 & mask5
# asm 1: vpand <mask5=reg128#6,<r4=reg128#13,>v01=reg128#13
# asm 2: vpand <mask5=%xmm5,<r4=%xmm12,>v01=%xmm12
vpand % xmm5, % xmm12, % xmm12

# qhasm: v11 = r5 & mask5
# asm 1: vpand <mask5=reg128#6,<r5=reg128#9,>v11=reg128#9
# asm 2: vpand <mask5=%xmm5,<r5=%xmm8,>v11=%xmm8
vpand % xmm5, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#13
# asm 2: psrlq $1,<v01=%xmm12
psrlq $1, % xmm12

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r4=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r4=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#13,>r5=reg128#9
# asm 2: vpor <v11=%xmm8,<v01=%xmm12,>r5=%xmm8
vpor % xmm8, % xmm12, % xmm8

# qhasm: v00 = r6 & mask4
# asm 1: vpand <mask4=reg128#5,<r6=reg128#7,>v00=reg128#13
# asm 2: vpand <mask4=%xmm4,<r6=%xmm6,>v00=%xmm12
vpand % xmm4, % xmm6, % xmm12

# qhasm: v10 = r7 & mask4
# asm 1: vpand <mask4=reg128#5,<r7=reg128#8,>v10=reg128#16
# asm 2: vpand <mask4=%xmm4,<r7=%xmm7,>v10=%xmm15
vpand % xmm4, % xmm7, % xmm15

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#16
# asm 2: psllq $1,<v10=%xmm15
psllq $1, % xmm15

# qhasm: v01 = r6 & mask5
# asm 1: vpand <mask5=reg128#6,<r6=reg128#7,>v01=reg128#7
# asm 2: vpand <mask5=%xmm5,<r6=%xmm6,>v01=%xmm6
vpand % xmm5, % xmm6, % xmm6

# qhasm: v11 = r7 & mask5
# asm 1: vpand <mask5=reg128#6,<r7=reg128#8,>v11=reg128#8
# asm 2: vpand <mask5=%xmm5,<r7=%xmm7,>v11=%xmm7
vpand % xmm5, % xmm7, % xmm7

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#7
# asm 2: psrlq $1,<v01=%xmm6
psrlq $1, % xmm6

# qhasm: r6 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#13,>r6=reg128#13
# asm 2: vpor <v10=%xmm15,<v00=%xmm12,>r6=%xmm12
vpor % xmm15, % xmm12, % xmm12

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#8,<v01=reg128#7,>r7=reg128#7
# asm 2: vpor <v11=%xmm7,<v01=%xmm6,>r7=%xmm6
vpor % xmm7, % xmm6, % xmm6

# qhasm: t0 = r0[0]r1[0]
# asm 1: vpunpcklqdq <r1=reg128#14,<r0=reg128#10,>t0=reg128#8
# asm 2: vpunpcklqdq <r1=%xmm13,<r0=%xmm9,>t0=%xmm7
vpunpcklqdq % xmm13, % xmm9, % xmm7

# qhasm: mem128[ input_0 + 384 ] = t0
# asm 1: movdqu   <t0=reg128#8,384(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,384(<input_0=%rdi)
movdqu   % xmm7, 384( % rdi)

# qhasm: t0 = r2[0]r3[0]
# asm 1: vpunpcklqdq <r3=reg128#11,<r2=reg128#15,>t0=reg128#8
# asm 2: vpunpcklqdq <r3=%xmm10,<r2=%xmm14,>t0=%xmm7
vpunpcklqdq % xmm10, % xmm14, % xmm7

# qhasm: mem128[ input_0 + 400 ] = t0
# asm 1: movdqu   <t0=reg128#8,400(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,400(<input_0=%rdi)
movdqu   % xmm7, 400( % rdi)

# qhasm: t0 = r4[0]r5[0]
# asm 1: vpunpcklqdq <r5=reg128#9,<r4=reg128#12,>t0=reg128#8
# asm 2: vpunpcklqdq <r5=%xmm8,<r4=%xmm11,>t0=%xmm7
vpunpcklqdq % xmm8, % xmm11, % xmm7

# qhasm: mem128[ input_0 + 416 ] = t0
# asm 1: movdqu   <t0=reg128#8,416(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm7,416(<input_0=%rdi)
movdqu   % xmm7, 416( % rdi)

# qhasm: t0 = r6[0]r7[0]
# asm 1: vpunpcklqdq <r7=reg128#7,<r6=reg128#13,>t0=reg128#7
# asm 2: vpunpcklqdq <r7=%xmm6,<r6=%xmm12,>t0=%xmm6
vpunpcklqdq % xmm6, % xmm12, % xmm6

# qhasm: mem128[ input_0 + 432 ] = t0
# asm 1: movdqu   <t0=reg128#7,432(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm6,432(<input_0=%rdi)
movdqu   % xmm6, 432( % rdi)

# qhasm: r0 = mem64[ input_0 + 448 ] x2
# asm 1: movddup 448(<input_0=int64#1),>r0=reg128#7
# asm 2: movddup 448(<input_0=%rdi),>r0=%xmm6
movddup 448( % rdi), % xmm6

# qhasm: r1 = mem64[ input_0 + 456 ] x2
# asm 1: movddup 456(<input_0=int64#1),>r1=reg128#8
# asm 2: movddup 456(<input_0=%rdi),>r1=%xmm7
movddup 456( % rdi), % xmm7

# qhasm: r2 = mem64[ input_0 + 464 ] x2
# asm 1: movddup 464(<input_0=int64#1),>r2=reg128#9
# asm 2: movddup 464(<input_0=%rdi),>r2=%xmm8
movddup 464( % rdi), % xmm8

# qhasm: r3 = mem64[ input_0 + 472 ] x2
# asm 1: movddup 472(<input_0=int64#1),>r3=reg128#10
# asm 2: movddup 472(<input_0=%rdi),>r3=%xmm9
movddup 472( % rdi), % xmm9

# qhasm: r4 = mem64[ input_0 + 480 ] x2
# asm 1: movddup 480(<input_0=int64#1),>r4=reg128#11
# asm 2: movddup 480(<input_0=%rdi),>r4=%xmm10
movddup 480( % rdi), % xmm10

# qhasm: r5 = mem64[ input_0 + 488 ] x2
# asm 1: movddup 488(<input_0=int64#1),>r5=reg128#12
# asm 2: movddup 488(<input_0=%rdi),>r5=%xmm11
movddup 488( % rdi), % xmm11

# qhasm: r6 = mem64[ input_0 + 496 ] x2
# asm 1: movddup 496(<input_0=int64#1),>r6=reg128#13
# asm 2: movddup 496(<input_0=%rdi),>r6=%xmm12
movddup 496( % rdi), % xmm12

# qhasm: r7 = mem64[ input_0 + 504 ] x2
# asm 1: movddup 504(<input_0=int64#1),>r7=reg128#14
# asm 2: movddup 504(<input_0=%rdi),>r7=%xmm13
movddup 504( % rdi), % xmm13

# qhasm: v00 = r0 & mask0
# asm 1: vpand <mask0=reg128#1,<r0=reg128#7,>v00=reg128#15
# asm 2: vpand <mask0=%xmm0,<r0=%xmm6,>v00=%xmm14
vpand % xmm0, % xmm6, % xmm14

# qhasm: v10 = r4 & mask0
# asm 1: vpand <mask0=reg128#1,<r4=reg128#11,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r4=%xmm10,>v10=%xmm15
vpand % xmm0, % xmm10, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r0 & mask1
# asm 1: vpand <mask1=reg128#2,<r0=reg128#7,>v01=reg128#7
# asm 2: vpand <mask1=%xmm1,<r0=%xmm6,>v01=%xmm6
vpand % xmm1, % xmm6, % xmm6

# qhasm: v11 = r4 & mask1
# asm 1: vpand <mask1=reg128#2,<r4=reg128#11,>v11=reg128#11
# asm 2: vpand <mask1=%xmm1,<r4=%xmm10,>v11=%xmm10
vpand % xmm1, % xmm10, % xmm10

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#7
# asm 2: psrlq $4,<v01=%xmm6
psrlq $4, % xmm6

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#15,>r0=reg128#15
# asm 2: vpor <v10=%xmm15,<v00=%xmm14,>r0=%xmm14
vpor % xmm15, % xmm14, % xmm14

# qhasm: r4 = v01 | v11
# asm 1: vpor <v11=reg128#11,<v01=reg128#7,>r4=reg128#7
# asm 2: vpor <v11=%xmm10,<v01=%xmm6,>r4=%xmm6
vpor % xmm10, % xmm6, % xmm6

# qhasm: v00 = r1 & mask0
# asm 1: vpand <mask0=reg128#1,<r1=reg128#8,>v00=reg128#11
# asm 2: vpand <mask0=%xmm0,<r1=%xmm7,>v00=%xmm10
vpand % xmm0, % xmm7, % xmm10

# qhasm: v10 = r5 & mask0
# asm 1: vpand <mask0=reg128#1,<r5=reg128#12,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r5=%xmm11,>v10=%xmm15
vpand % xmm0, % xmm11, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r1 & mask1
# asm 1: vpand <mask1=reg128#2,<r1=reg128#8,>v01=reg128#8
# asm 2: vpand <mask1=%xmm1,<r1=%xmm7,>v01=%xmm7
vpand % xmm1, % xmm7, % xmm7

# qhasm: v11 = r5 & mask1
# asm 1: vpand <mask1=reg128#2,<r5=reg128#12,>v11=reg128#12
# asm 2: vpand <mask1=%xmm1,<r5=%xmm11,>v11=%xmm11
vpand % xmm1, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#8
# asm 2: psrlq $4,<v01=%xmm7
psrlq $4, % xmm7

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#11,>r1=reg128#11
# asm 2: vpor <v10=%xmm15,<v00=%xmm10,>r1=%xmm10
vpor % xmm15, % xmm10, % xmm10

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#8,>r5=reg128#8
# asm 2: vpor <v11=%xmm11,<v01=%xmm7,>r5=%xmm7
vpor % xmm11, % xmm7, % xmm7

# qhasm: v00 = r2 & mask0
# asm 1: vpand <mask0=reg128#1,<r2=reg128#9,>v00=reg128#12
# asm 2: vpand <mask0=%xmm0,<r2=%xmm8,>v00=%xmm11
vpand % xmm0, % xmm8, % xmm11

# qhasm: v10 = r6 & mask0
# asm 1: vpand <mask0=reg128#1,<r6=reg128#13,>v10=reg128#16
# asm 2: vpand <mask0=%xmm0,<r6=%xmm12,>v10=%xmm15
vpand % xmm0, % xmm12, % xmm15

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#16
# asm 2: psllq $4,<v10=%xmm15
psllq $4, % xmm15

# qhasm: v01 = r2 & mask1
# asm 1: vpand <mask1=reg128#2,<r2=reg128#9,>v01=reg128#9
# asm 2: vpand <mask1=%xmm1,<r2=%xmm8,>v01=%xmm8
vpand % xmm1, % xmm8, % xmm8

# qhasm: v11 = r6 & mask1
# asm 1: vpand <mask1=reg128#2,<r6=reg128#13,>v11=reg128#13
# asm 2: vpand <mask1=%xmm1,<r6=%xmm12,>v11=%xmm12
vpand % xmm1, % xmm12, % xmm12

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#9
# asm 2: psrlq $4,<v01=%xmm8
psrlq $4, % xmm8

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#16,<v00=reg128#12,>r2=reg128#12
# asm 2: vpor <v10=%xmm15,<v00=%xmm11,>r2=%xmm11
vpor % xmm15, % xmm11, % xmm11

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#13,<v01=reg128#9,>r6=reg128#9
# asm 2: vpor <v11=%xmm12,<v01=%xmm8,>r6=%xmm8
vpor % xmm12, % xmm8, % xmm8

# qhasm: v00 = r3 & mask0
# asm 1: vpand <mask0=reg128#1,<r3=reg128#10,>v00=reg128#13
# asm 2: vpand <mask0=%xmm0,<r3=%xmm9,>v00=%xmm12
vpand % xmm0, % xmm9, % xmm12

# qhasm: v10 = r7 & mask0
# asm 1: vpand <mask0=reg128#1,<r7=reg128#14,>v10=reg128#1
# asm 2: vpand <mask0=%xmm0,<r7=%xmm13,>v10=%xmm0
vpand % xmm0, % xmm13, % xmm0

# qhasm: 2x v10 <<= 4
# asm 1: psllq $4,<v10=reg128#1
# asm 2: psllq $4,<v10=%xmm0
psllq $4, % xmm0

# qhasm: v01 = r3 & mask1
# asm 1: vpand <mask1=reg128#2,<r3=reg128#10,>v01=reg128#10
# asm 2: vpand <mask1=%xmm1,<r3=%xmm9,>v01=%xmm9
vpand % xmm1, % xmm9, % xmm9

# qhasm: v11 = r7 & mask1
# asm 1: vpand <mask1=reg128#2,<r7=reg128#14,>v11=reg128#2
# asm 2: vpand <mask1=%xmm1,<r7=%xmm13,>v11=%xmm1
vpand % xmm1, % xmm13, % xmm1

# qhasm: 2x v01 unsigned>>= 4
# asm 1: psrlq $4,<v01=reg128#10
# asm 2: psrlq $4,<v01=%xmm9
psrlq $4, % xmm9

# qhasm: r3 = v00 | v10
# asm 1: vpor <v10=reg128#1,<v00=reg128#13,>r3=reg128#1
# asm 2: vpor <v10=%xmm0,<v00=%xmm12,>r3=%xmm0
vpor % xmm0, % xmm12, % xmm0

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#2,<v01=reg128#10,>r7=reg128#2
# asm 2: vpor <v11=%xmm1,<v01=%xmm9,>r7=%xmm1
vpor % xmm1, % xmm9, % xmm1

# qhasm: v00 = r0 & mask2
# asm 1: vpand <mask2=reg128#3,<r0=reg128#15,>v00=reg128#10
# asm 2: vpand <mask2=%xmm2,<r0=%xmm14,>v00=%xmm9
vpand % xmm2, % xmm14, % xmm9

# qhasm: v10 = r2 & mask2
# asm 1: vpand <mask2=reg128#3,<r2=reg128#12,>v10=reg128#13
# asm 2: vpand <mask2=%xmm2,<r2=%xmm11,>v10=%xmm12
vpand % xmm2, % xmm11, % xmm12

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#13
# asm 2: psllq $2,<v10=%xmm12
psllq $2, % xmm12

# qhasm: v01 = r0 & mask3
# asm 1: vpand <mask3=reg128#4,<r0=reg128#15,>v01=reg128#14
# asm 2: vpand <mask3=%xmm3,<r0=%xmm14,>v01=%xmm13
vpand % xmm3, % xmm14, % xmm13

# qhasm: v11 = r2 & mask3
# asm 1: vpand <mask3=reg128#4,<r2=reg128#12,>v11=reg128#12
# asm 2: vpand <mask3=%xmm3,<r2=%xmm11,>v11=%xmm11
vpand % xmm3, % xmm11, % xmm11

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#14
# asm 2: psrlq $2,<v01=%xmm13
psrlq $2, % xmm13

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#13,<v00=reg128#10,>r0=reg128#10
# asm 2: vpor <v10=%xmm12,<v00=%xmm9,>r0=%xmm9
vpor % xmm12, % xmm9, % xmm9

# qhasm: r2 = v01 | v11
# asm 1: vpor <v11=reg128#12,<v01=reg128#14,>r2=reg128#12
# asm 2: vpor <v11=%xmm11,<v01=%xmm13,>r2=%xmm11
vpor % xmm11, % xmm13, % xmm11

# qhasm: v00 = r1 & mask2
# asm 1: vpand <mask2=reg128#3,<r1=reg128#11,>v00=reg128#13
# asm 2: vpand <mask2=%xmm2,<r1=%xmm10,>v00=%xmm12
vpand % xmm2, % xmm10, % xmm12

# qhasm: v10 = r3 & mask2
# asm 1: vpand <mask2=reg128#3,<r3=reg128#1,>v10=reg128#14
# asm 2: vpand <mask2=%xmm2,<r3=%xmm0,>v10=%xmm13
vpand % xmm2, % xmm0, % xmm13

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#14
# asm 2: psllq $2,<v10=%xmm13
psllq $2, % xmm13

# qhasm: v01 = r1 & mask3
# asm 1: vpand <mask3=reg128#4,<r1=reg128#11,>v01=reg128#11
# asm 2: vpand <mask3=%xmm3,<r1=%xmm10,>v01=%xmm10
vpand % xmm3, % xmm10, % xmm10

# qhasm: v11 = r3 & mask3
# asm 1: vpand <mask3=reg128#4,<r3=reg128#1,>v11=reg128#1
# asm 2: vpand <mask3=%xmm3,<r3=%xmm0,>v11=%xmm0
vpand % xmm3, % xmm0, % xmm0

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#11
# asm 2: psrlq $2,<v01=%xmm10
psrlq $2, % xmm10

# qhasm: r1 = v00 | v10
# asm 1: vpor <v10=reg128#14,<v00=reg128#13,>r1=reg128#13
# asm 2: vpor <v10=%xmm13,<v00=%xmm12,>r1=%xmm12
vpor % xmm13, % xmm12, % xmm12

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#1,<v01=reg128#11,>r3=reg128#1
# asm 2: vpor <v11=%xmm0,<v01=%xmm10,>r3=%xmm0
vpor % xmm0, % xmm10, % xmm0

# qhasm: v00 = r4 & mask2
# asm 1: vpand <mask2=reg128#3,<r4=reg128#7,>v00=reg128#11
# asm 2: vpand <mask2=%xmm2,<r4=%xmm6,>v00=%xmm10
vpand % xmm2, % xmm6, % xmm10

# qhasm: v10 = r6 & mask2
# asm 1: vpand <mask2=reg128#3,<r6=reg128#9,>v10=reg128#14
# asm 2: vpand <mask2=%xmm2,<r6=%xmm8,>v10=%xmm13
vpand % xmm2, % xmm8, % xmm13

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#14
# asm 2: psllq $2,<v10=%xmm13
psllq $2, % xmm13

# qhasm: v01 = r4 & mask3
# asm 1: vpand <mask3=reg128#4,<r4=reg128#7,>v01=reg128#7
# asm 2: vpand <mask3=%xmm3,<r4=%xmm6,>v01=%xmm6
vpand % xmm3, % xmm6, % xmm6

# qhasm: v11 = r6 & mask3
# asm 1: vpand <mask3=reg128#4,<r6=reg128#9,>v11=reg128#9
# asm 2: vpand <mask3=%xmm3,<r6=%xmm8,>v11=%xmm8
vpand % xmm3, % xmm8, % xmm8

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#7
# asm 2: psrlq $2,<v01=%xmm6
psrlq $2, % xmm6

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#14,<v00=reg128#11,>r4=reg128#11
# asm 2: vpor <v10=%xmm13,<v00=%xmm10,>r4=%xmm10
vpor % xmm13, % xmm10, % xmm10

# qhasm: r6 = v01 | v11
# asm 1: vpor <v11=reg128#9,<v01=reg128#7,>r6=reg128#7
# asm 2: vpor <v11=%xmm8,<v01=%xmm6,>r6=%xmm6
vpor % xmm8, % xmm6, % xmm6

# qhasm: v00 = r5 & mask2
# asm 1: vpand <mask2=reg128#3,<r5=reg128#8,>v00=reg128#9
# asm 2: vpand <mask2=%xmm2,<r5=%xmm7,>v00=%xmm8
vpand % xmm2, % xmm7, % xmm8

# qhasm: v10 = r7 & mask2
# asm 1: vpand <mask2=reg128#3,<r7=reg128#2,>v10=reg128#3
# asm 2: vpand <mask2=%xmm2,<r7=%xmm1,>v10=%xmm2
vpand % xmm2, % xmm1, % xmm2

# qhasm: 2x v10 <<= 2
# asm 1: psllq $2,<v10=reg128#3
# asm 2: psllq $2,<v10=%xmm2
psllq $2, % xmm2

# qhasm: v01 = r5 & mask3
# asm 1: vpand <mask3=reg128#4,<r5=reg128#8,>v01=reg128#8
# asm 2: vpand <mask3=%xmm3,<r5=%xmm7,>v01=%xmm7
vpand % xmm3, % xmm7, % xmm7

# qhasm: v11 = r7 & mask3
# asm 1: vpand <mask3=reg128#4,<r7=reg128#2,>v11=reg128#2
# asm 2: vpand <mask3=%xmm3,<r7=%xmm1,>v11=%xmm1
vpand % xmm3, % xmm1, % xmm1

# qhasm: 2x v01 unsigned>>= 2
# asm 1: psrlq $2,<v01=reg128#8
# asm 2: psrlq $2,<v01=%xmm7
psrlq $2, % xmm7

# qhasm: r5 = v00 | v10
# asm 1: vpor <v10=reg128#3,<v00=reg128#9,>r5=reg128#3
# asm 2: vpor <v10=%xmm2,<v00=%xmm8,>r5=%xmm2
vpor % xmm2, % xmm8, % xmm2

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#2,<v01=reg128#8,>r7=reg128#2
# asm 2: vpor <v11=%xmm1,<v01=%xmm7,>r7=%xmm1
vpor % xmm1, % xmm7, % xmm1

# qhasm: v00 = r0 & mask4
# asm 1: vpand <mask4=reg128#5,<r0=reg128#10,>v00=reg128#4
# asm 2: vpand <mask4=%xmm4,<r0=%xmm9,>v00=%xmm3
vpand % xmm4, % xmm9, % xmm3

# qhasm: v10 = r1 & mask4
# asm 1: vpand <mask4=reg128#5,<r1=reg128#13,>v10=reg128#8
# asm 2: vpand <mask4=%xmm4,<r1=%xmm12,>v10=%xmm7
vpand % xmm4, % xmm12, % xmm7

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#8
# asm 2: psllq $1,<v10=%xmm7
psllq $1, % xmm7

# qhasm: v01 = r0 & mask5
# asm 1: vpand <mask5=reg128#6,<r0=reg128#10,>v01=reg128#9
# asm 2: vpand <mask5=%xmm5,<r0=%xmm9,>v01=%xmm8
vpand % xmm5, % xmm9, % xmm8

# qhasm: v11 = r1 & mask5
# asm 1: vpand <mask5=reg128#6,<r1=reg128#13,>v11=reg128#10
# asm 2: vpand <mask5=%xmm5,<r1=%xmm12,>v11=%xmm9
vpand % xmm5, % xmm12, % xmm9

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#9
# asm 2: psrlq $1,<v01=%xmm8
psrlq $1, % xmm8

# qhasm: r0 = v00 | v10
# asm 1: vpor <v10=reg128#8,<v00=reg128#4,>r0=reg128#4
# asm 2: vpor <v10=%xmm7,<v00=%xmm3,>r0=%xmm3
vpor % xmm7, % xmm3, % xmm3

# qhasm: r1 = v01 | v11
# asm 1: vpor <v11=reg128#10,<v01=reg128#9,>r1=reg128#8
# asm 2: vpor <v11=%xmm9,<v01=%xmm8,>r1=%xmm7
vpor % xmm9, % xmm8, % xmm7

# qhasm: v00 = r2 & mask4
# asm 1: vpand <mask4=reg128#5,<r2=reg128#12,>v00=reg128#9
# asm 2: vpand <mask4=%xmm4,<r2=%xmm11,>v00=%xmm8
vpand % xmm4, % xmm11, % xmm8

# qhasm: v10 = r3 & mask4
# asm 1: vpand <mask4=reg128#5,<r3=reg128#1,>v10=reg128#10
# asm 2: vpand <mask4=%xmm4,<r3=%xmm0,>v10=%xmm9
vpand % xmm4, % xmm0, % xmm9

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#10
# asm 2: psllq $1,<v10=%xmm9
psllq $1, % xmm9

# qhasm: v01 = r2 & mask5
# asm 1: vpand <mask5=reg128#6,<r2=reg128#12,>v01=reg128#12
# asm 2: vpand <mask5=%xmm5,<r2=%xmm11,>v01=%xmm11
vpand % xmm5, % xmm11, % xmm11

# qhasm: v11 = r3 & mask5
# asm 1: vpand <mask5=reg128#6,<r3=reg128#1,>v11=reg128#1
# asm 2: vpand <mask5=%xmm5,<r3=%xmm0,>v11=%xmm0
vpand % xmm5, % xmm0, % xmm0

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#12
# asm 2: psrlq $1,<v01=%xmm11
psrlq $1, % xmm11

# qhasm: r2 = v00 | v10
# asm 1: vpor <v10=reg128#10,<v00=reg128#9,>r2=reg128#9
# asm 2: vpor <v10=%xmm9,<v00=%xmm8,>r2=%xmm8
vpor % xmm9, % xmm8, % xmm8

# qhasm: r3 = v01 | v11
# asm 1: vpor <v11=reg128#1,<v01=reg128#12,>r3=reg128#1
# asm 2: vpor <v11=%xmm0,<v01=%xmm11,>r3=%xmm0
vpor % xmm0, % xmm11, % xmm0

# qhasm: v00 = r4 & mask4
# asm 1: vpand <mask4=reg128#5,<r4=reg128#11,>v00=reg128#10
# asm 2: vpand <mask4=%xmm4,<r4=%xmm10,>v00=%xmm9
vpand % xmm4, % xmm10, % xmm9

# qhasm: v10 = r5 & mask4
# asm 1: vpand <mask4=reg128#5,<r5=reg128#3,>v10=reg128#12
# asm 2: vpand <mask4=%xmm4,<r5=%xmm2,>v10=%xmm11
vpand % xmm4, % xmm2, % xmm11

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#12
# asm 2: psllq $1,<v10=%xmm11
psllq $1, % xmm11

# qhasm: v01 = r4 & mask5
# asm 1: vpand <mask5=reg128#6,<r4=reg128#11,>v01=reg128#11
# asm 2: vpand <mask5=%xmm5,<r4=%xmm10,>v01=%xmm10
vpand % xmm5, % xmm10, % xmm10

# qhasm: v11 = r5 & mask5
# asm 1: vpand <mask5=reg128#6,<r5=reg128#3,>v11=reg128#3
# asm 2: vpand <mask5=%xmm5,<r5=%xmm2,>v11=%xmm2
vpand % xmm5, % xmm2, % xmm2

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#11
# asm 2: psrlq $1,<v01=%xmm10
psrlq $1, % xmm10

# qhasm: r4 = v00 | v10
# asm 1: vpor <v10=reg128#12,<v00=reg128#10,>r4=reg128#10
# asm 2: vpor <v10=%xmm11,<v00=%xmm9,>r4=%xmm9
vpor % xmm11, % xmm9, % xmm9

# qhasm: r5 = v01 | v11
# asm 1: vpor <v11=reg128#3,<v01=reg128#11,>r5=reg128#3
# asm 2: vpor <v11=%xmm2,<v01=%xmm10,>r5=%xmm2
vpor % xmm2, % xmm10, % xmm2

# qhasm: v00 = r6 & mask4
# asm 1: vpand <mask4=reg128#5,<r6=reg128#7,>v00=reg128#11
# asm 2: vpand <mask4=%xmm4,<r6=%xmm6,>v00=%xmm10
vpand % xmm4, % xmm6, % xmm10

# qhasm: v10 = r7 & mask4
# asm 1: vpand <mask4=reg128#5,<r7=reg128#2,>v10=reg128#5
# asm 2: vpand <mask4=%xmm4,<r7=%xmm1,>v10=%xmm4
vpand % xmm4, % xmm1, % xmm4

# qhasm: 2x v10 <<= 1
# asm 1: psllq $1,<v10=reg128#5
# asm 2: psllq $1,<v10=%xmm4
psllq $1, % xmm4

# qhasm: v01 = r6 & mask5
# asm 1: vpand <mask5=reg128#6,<r6=reg128#7,>v01=reg128#7
# asm 2: vpand <mask5=%xmm5,<r6=%xmm6,>v01=%xmm6
vpand % xmm5, % xmm6, % xmm6

# qhasm: v11 = r7 & mask5
# asm 1: vpand <mask5=reg128#6,<r7=reg128#2,>v11=reg128#2
# asm 2: vpand <mask5=%xmm5,<r7=%xmm1,>v11=%xmm1
vpand % xmm5, % xmm1, % xmm1

# qhasm: 2x v01 unsigned>>= 1
# asm 1: psrlq $1,<v01=reg128#7
# asm 2: psrlq $1,<v01=%xmm6
psrlq $1, % xmm6

# qhasm: r6 = v00 | v10
# asm 1: vpor <v10=reg128#5,<v00=reg128#11,>r6=reg128#5
# asm 2: vpor <v10=%xmm4,<v00=%xmm10,>r6=%xmm4
vpor % xmm4, % xmm10, % xmm4

# qhasm: r7 = v01 | v11
# asm 1: vpor <v11=reg128#2,<v01=reg128#7,>r7=reg128#2
# asm 2: vpor <v11=%xmm1,<v01=%xmm6,>r7=%xmm1
vpor % xmm1, % xmm6, % xmm1

# qhasm: t0 = r0[0]r1[0]
# asm 1: vpunpcklqdq <r1=reg128#8,<r0=reg128#4,>t0=reg128#4
# asm 2: vpunpcklqdq <r1=%xmm7,<r0=%xmm3,>t0=%xmm3
vpunpcklqdq % xmm7, % xmm3, % xmm3

# qhasm: mem128[ input_0 + 448 ] = t0
# asm 1: movdqu   <t0=reg128#4,448(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm3,448(<input_0=%rdi)
movdqu   % xmm3, 448( % rdi)

# qhasm: t0 = r2[0]r3[0]
# asm 1: vpunpcklqdq <r3=reg128#1,<r2=reg128#9,>t0=reg128#1
# asm 2: vpunpcklqdq <r3=%xmm0,<r2=%xmm8,>t0=%xmm0
vpunpcklqdq % xmm0, % xmm8, % xmm0

# qhasm: mem128[ input_0 + 464 ] = t0
# asm 1: movdqu   <t0=reg128#1,464(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm0,464(<input_0=%rdi)
movdqu   % xmm0, 464( % rdi)

# qhasm: t0 = r4[0]r5[0]
# asm 1: vpunpcklqdq <r5=reg128#3,<r4=reg128#10,>t0=reg128#1
# asm 2: vpunpcklqdq <r5=%xmm2,<r4=%xmm9,>t0=%xmm0
vpunpcklqdq % xmm2, % xmm9, % xmm0

# qhasm: mem128[ input_0 + 480 ] = t0
# asm 1: movdqu   <t0=reg128#1,480(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm0,480(<input_0=%rdi)
movdqu   % xmm0, 480( % rdi)

# qhasm: t0 = r6[0]r7[0]
# asm 1: vpunpcklqdq <r7=reg128#2,<r6=reg128#5,>t0=reg128#1
# asm 2: vpunpcklqdq <r7=%xmm1,<r6=%xmm4,>t0=%xmm0
vpunpcklqdq % xmm1, % xmm4, % xmm0

# qhasm: mem128[ input_0 + 496 ] = t0
# asm 1: movdqu   <t0=reg128#1,496(<input_0=int64#1)
# asm 2: movdqu   <t0=%xmm0,496(<input_0=%rdi)
movdqu   % xmm0, 496( % rdi)

# qhasm: return
add % r11, % rsp
ret
