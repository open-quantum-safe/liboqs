#include "namespace.h"

#define vec256_mul_asm CRYPTO_NAMESPACE(vec256_mul_asm)
#define _vec256_mul_asm _CRYPTO_NAMESPACE(vec256_mul_asm)

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

# qhasm: reg256 a0

# qhasm: reg256 a1

# qhasm: reg256 a2

# qhasm: reg256 a3

# qhasm: reg256 a4

# qhasm: reg256 a5

# qhasm: reg256 a6

# qhasm: reg256 a7

# qhasm: reg256 a8

# qhasm: reg256 a9

# qhasm: reg256 a10

# qhasm: reg256 a11

# qhasm: reg256 a12

# qhasm: reg256 b0

# qhasm: reg256 b1

# qhasm: reg256 r0

# qhasm: reg256 r1

# qhasm: reg256 r2

# qhasm: reg256 r3

# qhasm: reg256 r4

# qhasm: reg256 r5

# qhasm: reg256 r6

# qhasm: reg256 r7

# qhasm: reg256 r8

# qhasm: reg256 r9

# qhasm: reg256 r10

# qhasm: reg256 r11

# qhasm: reg256 r12

# qhasm: reg256 r13

# qhasm: reg256 r14

# qhasm: reg256 r15

# qhasm: reg256 r16

# qhasm: reg256 r17

# qhasm: reg256 r18

# qhasm: reg256 r19

# qhasm: reg256 r20

# qhasm: reg256 r21

# qhasm: reg256 r22

# qhasm: reg256 r23

# qhasm: reg256 r24

# qhasm: reg256 r

# qhasm: enter vec256_mul_asm
.p2align 5
.global _vec256_mul_asm
.global vec256_mul_asm
_vec256_mul_asm:
vec256_mul_asm:
mov % rsp, % r11
and $31, % r11
add $0, % r11
sub % r11, % rsp

# qhasm: b0 = mem256[ input_2 + 0 ]
# asm 1: vmovupd   0(<input_2=int64#3),>b0=reg256#1
# asm 2: vmovupd   0(<input_2=%rdx),>b0=%ymm0
vmovupd   0( % rdx), % ymm0

# qhasm: a12 = mem256[ input_1 + 384 ]
# asm 1: vmovupd   384(<input_1=int64#2),>a12=reg256#2
# asm 2: vmovupd   384(<input_1=%rsi),>a12=%ymm1
vmovupd   384( % rsi), % ymm1

# qhasm: r12 = a12 & b0
# asm 1: vpand <a12=reg256#2,<b0=reg256#1,>r12=reg256#3
# asm 2: vpand <a12=%ymm1,<b0=%ymm0,>r12=%ymm2
vpand % ymm1, % ymm0, % ymm2

# qhasm: r13 = a12 & mem256[input_2 + 32]
# asm 1: vpand 32(<input_2=int64#3),<a12=reg256#2,>r13=reg256#4
# asm 2: vpand 32(<input_2=%rdx),<a12=%ymm1,>r13=%ymm3
vpand 32( % rdx), % ymm1, % ymm3

# qhasm: r14 = a12 & mem256[input_2 + 64]
# asm 1: vpand 64(<input_2=int64#3),<a12=reg256#2,>r14=reg256#5
# asm 2: vpand 64(<input_2=%rdx),<a12=%ymm1,>r14=%ymm4
vpand 64( % rdx), % ymm1, % ymm4

# qhasm: r15 = a12 & mem256[input_2 + 96]
# asm 1: vpand 96(<input_2=int64#3),<a12=reg256#2,>r15=reg256#6
# asm 2: vpand 96(<input_2=%rdx),<a12=%ymm1,>r15=%ymm5
vpand 96( % rdx), % ymm1, % ymm5

# qhasm: r16 = a12 & mem256[input_2 + 128]
# asm 1: vpand 128(<input_2=int64#3),<a12=reg256#2,>r16=reg256#7
# asm 2: vpand 128(<input_2=%rdx),<a12=%ymm1,>r16=%ymm6
vpand 128( % rdx), % ymm1, % ymm6

# qhasm: r17 = a12 & mem256[input_2 + 160]
# asm 1: vpand 160(<input_2=int64#3),<a12=reg256#2,>r17=reg256#8
# asm 2: vpand 160(<input_2=%rdx),<a12=%ymm1,>r17=%ymm7
vpand 160( % rdx), % ymm1, % ymm7

# qhasm: r18 = a12 & mem256[input_2 + 192]
# asm 1: vpand 192(<input_2=int64#3),<a12=reg256#2,>r18=reg256#9
# asm 2: vpand 192(<input_2=%rdx),<a12=%ymm1,>r18=%ymm8
vpand 192( % rdx), % ymm1, % ymm8

# qhasm: r19 = a12 & mem256[input_2 + 224]
# asm 1: vpand 224(<input_2=int64#3),<a12=reg256#2,>r19=reg256#10
# asm 2: vpand 224(<input_2=%rdx),<a12=%ymm1,>r19=%ymm9
vpand 224( % rdx), % ymm1, % ymm9

# qhasm: r20 = a12 & mem256[input_2 + 256]
# asm 1: vpand 256(<input_2=int64#3),<a12=reg256#2,>r20=reg256#11
# asm 2: vpand 256(<input_2=%rdx),<a12=%ymm1,>r20=%ymm10
vpand 256( % rdx), % ymm1, % ymm10

# qhasm: r21 = a12 & mem256[input_2 + 288]
# asm 1: vpand 288(<input_2=int64#3),<a12=reg256#2,>r21=reg256#12
# asm 2: vpand 288(<input_2=%rdx),<a12=%ymm1,>r21=%ymm11
vpand 288( % rdx), % ymm1, % ymm11

# qhasm: r22 = a12 & mem256[input_2 + 320]
# asm 1: vpand 320(<input_2=int64#3),<a12=reg256#2,>r22=reg256#13
# asm 2: vpand 320(<input_2=%rdx),<a12=%ymm1,>r22=%ymm12
vpand 320( % rdx), % ymm1, % ymm12

# qhasm: r23 = a12 & mem256[input_2 + 352]
# asm 1: vpand 352(<input_2=int64#3),<a12=reg256#2,>r23=reg256#14
# asm 2: vpand 352(<input_2=%rdx),<a12=%ymm1,>r23=%ymm13
vpand 352( % rdx), % ymm1, % ymm13

# qhasm: r24 = a12 & mem256[input_2 + 384]
# asm 1: vpand 384(<input_2=int64#3),<a12=reg256#2,>r24=reg256#2
# asm 2: vpand 384(<input_2=%rdx),<a12=%ymm1,>r24=%ymm1
vpand 384( % rdx), % ymm1, % ymm1

# qhasm: r15 ^= r24
# asm 1: vpxor <r24=reg256#2,<r15=reg256#6,<r15=reg256#6
# asm 2: vpxor <r24=%ymm1,<r15=%ymm5,<r15=%ymm5
vpxor % ymm1, % ymm5, % ymm5

# qhasm: r14 ^= r24
# asm 1: vpxor <r24=reg256#2,<r14=reg256#5,<r14=reg256#5
# asm 2: vpxor <r24=%ymm1,<r14=%ymm4,<r14=%ymm4
vpxor % ymm1, % ymm4, % ymm4

# qhasm: r12 ^= r24
# asm 1: vpxor <r24=reg256#2,<r12=reg256#3,<r12=reg256#3
# asm 2: vpxor <r24=%ymm1,<r12=%ymm2,<r12=%ymm2
vpxor % ymm1, % ymm2, % ymm2

# qhasm: r11 = r24
# asm 1: vmovapd <r24=reg256#2,>r11=reg256#2
# asm 2: vmovapd <r24=%ymm1,>r11=%ymm1
vmovapd % ymm1, % ymm1

# qhasm: a11 = mem256[ input_1 + 352 ]
# asm 1: vmovupd   352(<input_1=int64#2),>a11=reg256#15
# asm 2: vmovupd   352(<input_1=%rsi),>a11=%ymm14
vmovupd   352( % rsi), % ymm14

# qhasm: r = a11 & b0
# asm 1: vpand <a11=reg256#15,<b0=reg256#1,>r=reg256#16
# asm 2: vpand <a11=%ymm14,<b0=%ymm0,>r=%ymm15
vpand % ymm14, % ymm0, % ymm15

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#16,<r11=reg256#2,<r11=reg256#2
# asm 2: vpxor <r=%ymm15,<r11=%ymm1,<r11=%ymm1
vpxor % ymm15, % ymm1, % ymm1

# qhasm: r = a11 & mem256[input_2 + 32]
# asm 1: vpand 32(<input_2=int64#3),<a11=reg256#15,>r=reg256#16
# asm 2: vpand 32(<input_2=%rdx),<a11=%ymm14,>r=%ymm15
vpand 32( % rdx), % ymm14, % ymm15

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#16,<r12=reg256#3,<r12=reg256#3
# asm 2: vpxor <r=%ymm15,<r12=%ymm2,<r12=%ymm2
vpxor % ymm15, % ymm2, % ymm2

# qhasm: r = a11 & mem256[input_2 + 64]
# asm 1: vpand 64(<input_2=int64#3),<a11=reg256#15,>r=reg256#16
# asm 2: vpand 64(<input_2=%rdx),<a11=%ymm14,>r=%ymm15
vpand 64( % rdx), % ymm14, % ymm15

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#16,<r13=reg256#4,<r13=reg256#4
# asm 2: vpxor <r=%ymm15,<r13=%ymm3,<r13=%ymm3
vpxor % ymm15, % ymm3, % ymm3

# qhasm: r = a11 & mem256[input_2 + 96]
# asm 1: vpand 96(<input_2=int64#3),<a11=reg256#15,>r=reg256#16
# asm 2: vpand 96(<input_2=%rdx),<a11=%ymm14,>r=%ymm15
vpand 96( % rdx), % ymm14, % ymm15

# qhasm: r14 ^= r
# asm 1: vpxor <r=reg256#16,<r14=reg256#5,<r14=reg256#5
# asm 2: vpxor <r=%ymm15,<r14=%ymm4,<r14=%ymm4
vpxor % ymm15, % ymm4, % ymm4

# qhasm: r = a11 & mem256[input_2 + 128]
# asm 1: vpand 128(<input_2=int64#3),<a11=reg256#15,>r=reg256#16
# asm 2: vpand 128(<input_2=%rdx),<a11=%ymm14,>r=%ymm15
vpand 128( % rdx), % ymm14, % ymm15

# qhasm: r15 ^= r
# asm 1: vpxor <r=reg256#16,<r15=reg256#6,<r15=reg256#6
# asm 2: vpxor <r=%ymm15,<r15=%ymm5,<r15=%ymm5
vpxor % ymm15, % ymm5, % ymm5

# qhasm: r = a11 & mem256[input_2 + 160]
# asm 1: vpand 160(<input_2=int64#3),<a11=reg256#15,>r=reg256#16
# asm 2: vpand 160(<input_2=%rdx),<a11=%ymm14,>r=%ymm15
vpand 160( % rdx), % ymm14, % ymm15

# qhasm: r16 ^= r
# asm 1: vpxor <r=reg256#16,<r16=reg256#7,<r16=reg256#7
# asm 2: vpxor <r=%ymm15,<r16=%ymm6,<r16=%ymm6
vpxor % ymm15, % ymm6, % ymm6

# qhasm: r = a11 & mem256[input_2 + 192]
# asm 1: vpand 192(<input_2=int64#3),<a11=reg256#15,>r=reg256#16
# asm 2: vpand 192(<input_2=%rdx),<a11=%ymm14,>r=%ymm15
vpand 192( % rdx), % ymm14, % ymm15

# qhasm: r17 ^= r
# asm 1: vpxor <r=reg256#16,<r17=reg256#8,<r17=reg256#8
# asm 2: vpxor <r=%ymm15,<r17=%ymm7,<r17=%ymm7
vpxor % ymm15, % ymm7, % ymm7

# qhasm: r = a11 & mem256[input_2 + 224]
# asm 1: vpand 224(<input_2=int64#3),<a11=reg256#15,>r=reg256#16
# asm 2: vpand 224(<input_2=%rdx),<a11=%ymm14,>r=%ymm15
vpand 224( % rdx), % ymm14, % ymm15

# qhasm: r18 ^= r
# asm 1: vpxor <r=reg256#16,<r18=reg256#9,<r18=reg256#9
# asm 2: vpxor <r=%ymm15,<r18=%ymm8,<r18=%ymm8
vpxor % ymm15, % ymm8, % ymm8

# qhasm: r = a11 & mem256[input_2 + 256]
# asm 1: vpand 256(<input_2=int64#3),<a11=reg256#15,>r=reg256#16
# asm 2: vpand 256(<input_2=%rdx),<a11=%ymm14,>r=%ymm15
vpand 256( % rdx), % ymm14, % ymm15

# qhasm: r19 ^= r
# asm 1: vpxor <r=reg256#16,<r19=reg256#10,<r19=reg256#10
# asm 2: vpxor <r=%ymm15,<r19=%ymm9,<r19=%ymm9
vpxor % ymm15, % ymm9, % ymm9

# qhasm: r = a11 & mem256[input_2 + 288]
# asm 1: vpand 288(<input_2=int64#3),<a11=reg256#15,>r=reg256#16
# asm 2: vpand 288(<input_2=%rdx),<a11=%ymm14,>r=%ymm15
vpand 288( % rdx), % ymm14, % ymm15

# qhasm: r20 ^= r
# asm 1: vpxor <r=reg256#16,<r20=reg256#11,<r20=reg256#11
# asm 2: vpxor <r=%ymm15,<r20=%ymm10,<r20=%ymm10
vpxor % ymm15, % ymm10, % ymm10

# qhasm: r = a11 & mem256[input_2 + 320]
# asm 1: vpand 320(<input_2=int64#3),<a11=reg256#15,>r=reg256#16
# asm 2: vpand 320(<input_2=%rdx),<a11=%ymm14,>r=%ymm15
vpand 320( % rdx), % ymm14, % ymm15

# qhasm: r21 ^= r
# asm 1: vpxor <r=reg256#16,<r21=reg256#12,<r21=reg256#12
# asm 2: vpxor <r=%ymm15,<r21=%ymm11,<r21=%ymm11
vpxor % ymm15, % ymm11, % ymm11

# qhasm: r = a11 & mem256[input_2 + 352]
# asm 1: vpand 352(<input_2=int64#3),<a11=reg256#15,>r=reg256#16
# asm 2: vpand 352(<input_2=%rdx),<a11=%ymm14,>r=%ymm15
vpand 352( % rdx), % ymm14, % ymm15

# qhasm: r22 ^= r
# asm 1: vpxor <r=reg256#16,<r22=reg256#13,<r22=reg256#13
# asm 2: vpxor <r=%ymm15,<r22=%ymm12,<r22=%ymm12
vpxor % ymm15, % ymm12, % ymm12

# qhasm: r = a11 & mem256[input_2 + 384]
# asm 1: vpand 384(<input_2=int64#3),<a11=reg256#15,>r=reg256#15
# asm 2: vpand 384(<input_2=%rdx),<a11=%ymm14,>r=%ymm14
vpand 384( % rdx), % ymm14, % ymm14

# qhasm: r23 ^= r
# asm 1: vpxor <r=reg256#15,<r23=reg256#14,<r23=reg256#14
# asm 2: vpxor <r=%ymm14,<r23=%ymm13,<r23=%ymm13
vpxor % ymm14, % ymm13, % ymm13

# qhasm: r14 ^= r23
# asm 1: vpxor <r23=reg256#14,<r14=reg256#5,<r14=reg256#5
# asm 2: vpxor <r23=%ymm13,<r14=%ymm4,<r14=%ymm4
vpxor % ymm13, % ymm4, % ymm4

# qhasm: r13 ^= r23
# asm 1: vpxor <r23=reg256#14,<r13=reg256#4,<r13=reg256#4
# asm 2: vpxor <r23=%ymm13,<r13=%ymm3,<r13=%ymm3
vpxor % ymm13, % ymm3, % ymm3

# qhasm: r11 ^= r23
# asm 1: vpxor <r23=reg256#14,<r11=reg256#2,<r11=reg256#2
# asm 2: vpxor <r23=%ymm13,<r11=%ymm1,<r11=%ymm1
vpxor % ymm13, % ymm1, % ymm1

# qhasm: r10 = r23
# asm 1: vmovapd <r23=reg256#14,>r10=reg256#14
# asm 2: vmovapd <r23=%ymm13,>r10=%ymm13
vmovapd % ymm13, % ymm13

# qhasm: a10 = mem256[ input_1 + 320 ]
# asm 1: vmovupd   320(<input_1=int64#2),>a10=reg256#15
# asm 2: vmovupd   320(<input_1=%rsi),>a10=%ymm14
vmovupd   320( % rsi), % ymm14

# qhasm: r = a10 & b0
# asm 1: vpand <a10=reg256#15,<b0=reg256#1,>r=reg256#16
# asm 2: vpand <a10=%ymm14,<b0=%ymm0,>r=%ymm15
vpand % ymm14, % ymm0, % ymm15

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#16,<r10=reg256#14,<r10=reg256#14
# asm 2: vpxor <r=%ymm15,<r10=%ymm13,<r10=%ymm13
vpxor % ymm15, % ymm13, % ymm13

# qhasm: r = a10 & mem256[input_2 + 32]
# asm 1: vpand 32(<input_2=int64#3),<a10=reg256#15,>r=reg256#16
# asm 2: vpand 32(<input_2=%rdx),<a10=%ymm14,>r=%ymm15
vpand 32( % rdx), % ymm14, % ymm15

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#16,<r11=reg256#2,<r11=reg256#2
# asm 2: vpxor <r=%ymm15,<r11=%ymm1,<r11=%ymm1
vpxor % ymm15, % ymm1, % ymm1

# qhasm: r = a10 & mem256[input_2 + 64]
# asm 1: vpand 64(<input_2=int64#3),<a10=reg256#15,>r=reg256#16
# asm 2: vpand 64(<input_2=%rdx),<a10=%ymm14,>r=%ymm15
vpand 64( % rdx), % ymm14, % ymm15

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#16,<r12=reg256#3,<r12=reg256#3
# asm 2: vpxor <r=%ymm15,<r12=%ymm2,<r12=%ymm2
vpxor % ymm15, % ymm2, % ymm2

# qhasm: r = a10 & mem256[input_2 + 96]
# asm 1: vpand 96(<input_2=int64#3),<a10=reg256#15,>r=reg256#16
# asm 2: vpand 96(<input_2=%rdx),<a10=%ymm14,>r=%ymm15
vpand 96( % rdx), % ymm14, % ymm15

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#16,<r13=reg256#4,<r13=reg256#4
# asm 2: vpxor <r=%ymm15,<r13=%ymm3,<r13=%ymm3
vpxor % ymm15, % ymm3, % ymm3

# qhasm: r = a10 & mem256[input_2 + 128]
# asm 1: vpand 128(<input_2=int64#3),<a10=reg256#15,>r=reg256#16
# asm 2: vpand 128(<input_2=%rdx),<a10=%ymm14,>r=%ymm15
vpand 128( % rdx), % ymm14, % ymm15

# qhasm: r14 ^= r
# asm 1: vpxor <r=reg256#16,<r14=reg256#5,<r14=reg256#5
# asm 2: vpxor <r=%ymm15,<r14=%ymm4,<r14=%ymm4
vpxor % ymm15, % ymm4, % ymm4

# qhasm: r = a10 & mem256[input_2 + 160]
# asm 1: vpand 160(<input_2=int64#3),<a10=reg256#15,>r=reg256#16
# asm 2: vpand 160(<input_2=%rdx),<a10=%ymm14,>r=%ymm15
vpand 160( % rdx), % ymm14, % ymm15

# qhasm: r15 ^= r
# asm 1: vpxor <r=reg256#16,<r15=reg256#6,<r15=reg256#6
# asm 2: vpxor <r=%ymm15,<r15=%ymm5,<r15=%ymm5
vpxor % ymm15, % ymm5, % ymm5

# qhasm: r = a10 & mem256[input_2 + 192]
# asm 1: vpand 192(<input_2=int64#3),<a10=reg256#15,>r=reg256#16
# asm 2: vpand 192(<input_2=%rdx),<a10=%ymm14,>r=%ymm15
vpand 192( % rdx), % ymm14, % ymm15

# qhasm: r16 ^= r
# asm 1: vpxor <r=reg256#16,<r16=reg256#7,<r16=reg256#7
# asm 2: vpxor <r=%ymm15,<r16=%ymm6,<r16=%ymm6
vpxor % ymm15, % ymm6, % ymm6

# qhasm: r = a10 & mem256[input_2 + 224]
# asm 1: vpand 224(<input_2=int64#3),<a10=reg256#15,>r=reg256#16
# asm 2: vpand 224(<input_2=%rdx),<a10=%ymm14,>r=%ymm15
vpand 224( % rdx), % ymm14, % ymm15

# qhasm: r17 ^= r
# asm 1: vpxor <r=reg256#16,<r17=reg256#8,<r17=reg256#8
# asm 2: vpxor <r=%ymm15,<r17=%ymm7,<r17=%ymm7
vpxor % ymm15, % ymm7, % ymm7

# qhasm: r = a10 & mem256[input_2 + 256]
# asm 1: vpand 256(<input_2=int64#3),<a10=reg256#15,>r=reg256#16
# asm 2: vpand 256(<input_2=%rdx),<a10=%ymm14,>r=%ymm15
vpand 256( % rdx), % ymm14, % ymm15

# qhasm: r18 ^= r
# asm 1: vpxor <r=reg256#16,<r18=reg256#9,<r18=reg256#9
# asm 2: vpxor <r=%ymm15,<r18=%ymm8,<r18=%ymm8
vpxor % ymm15, % ymm8, % ymm8

# qhasm: r = a10 & mem256[input_2 + 288]
# asm 1: vpand 288(<input_2=int64#3),<a10=reg256#15,>r=reg256#16
# asm 2: vpand 288(<input_2=%rdx),<a10=%ymm14,>r=%ymm15
vpand 288( % rdx), % ymm14, % ymm15

# qhasm: r19 ^= r
# asm 1: vpxor <r=reg256#16,<r19=reg256#10,<r19=reg256#10
# asm 2: vpxor <r=%ymm15,<r19=%ymm9,<r19=%ymm9
vpxor % ymm15, % ymm9, % ymm9

# qhasm: r = a10 & mem256[input_2 + 320]
# asm 1: vpand 320(<input_2=int64#3),<a10=reg256#15,>r=reg256#16
# asm 2: vpand 320(<input_2=%rdx),<a10=%ymm14,>r=%ymm15
vpand 320( % rdx), % ymm14, % ymm15

# qhasm: r20 ^= r
# asm 1: vpxor <r=reg256#16,<r20=reg256#11,<r20=reg256#11
# asm 2: vpxor <r=%ymm15,<r20=%ymm10,<r20=%ymm10
vpxor % ymm15, % ymm10, % ymm10

# qhasm: r = a10 & mem256[input_2 + 352]
# asm 1: vpand 352(<input_2=int64#3),<a10=reg256#15,>r=reg256#16
# asm 2: vpand 352(<input_2=%rdx),<a10=%ymm14,>r=%ymm15
vpand 352( % rdx), % ymm14, % ymm15

# qhasm: r21 ^= r
# asm 1: vpxor <r=reg256#16,<r21=reg256#12,<r21=reg256#12
# asm 2: vpxor <r=%ymm15,<r21=%ymm11,<r21=%ymm11
vpxor % ymm15, % ymm11, % ymm11

# qhasm: r = a10 & mem256[input_2 + 384]
# asm 1: vpand 384(<input_2=int64#3),<a10=reg256#15,>r=reg256#15
# asm 2: vpand 384(<input_2=%rdx),<a10=%ymm14,>r=%ymm14
vpand 384( % rdx), % ymm14, % ymm14

# qhasm: r22 ^= r
# asm 1: vpxor <r=reg256#15,<r22=reg256#13,<r22=reg256#13
# asm 2: vpxor <r=%ymm14,<r22=%ymm12,<r22=%ymm12
vpxor % ymm14, % ymm12, % ymm12

# qhasm: r13 ^= r22
# asm 1: vpxor <r22=reg256#13,<r13=reg256#4,<r13=reg256#4
# asm 2: vpxor <r22=%ymm12,<r13=%ymm3,<r13=%ymm3
vpxor % ymm12, % ymm3, % ymm3

# qhasm: r12 ^= r22
# asm 1: vpxor <r22=reg256#13,<r12=reg256#3,<r12=reg256#3
# asm 2: vpxor <r22=%ymm12,<r12=%ymm2,<r12=%ymm2
vpxor % ymm12, % ymm2, % ymm2

# qhasm: r10 ^= r22
# asm 1: vpxor <r22=reg256#13,<r10=reg256#14,<r10=reg256#14
# asm 2: vpxor <r22=%ymm12,<r10=%ymm13,<r10=%ymm13
vpxor % ymm12, % ymm13, % ymm13

# qhasm: r9 = r22
# asm 1: vmovapd <r22=reg256#13,>r9=reg256#13
# asm 2: vmovapd <r22=%ymm12,>r9=%ymm12
vmovapd % ymm12, % ymm12

# qhasm: a9 = mem256[ input_1 + 288 ]
# asm 1: vmovupd   288(<input_1=int64#2),>a9=reg256#15
# asm 2: vmovupd   288(<input_1=%rsi),>a9=%ymm14
vmovupd   288( % rsi), % ymm14

# qhasm: r = a9 & b0
# asm 1: vpand <a9=reg256#15,<b0=reg256#1,>r=reg256#16
# asm 2: vpand <a9=%ymm14,<b0=%ymm0,>r=%ymm15
vpand % ymm14, % ymm0, % ymm15

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#16,<r9=reg256#13,<r9=reg256#13
# asm 2: vpxor <r=%ymm15,<r9=%ymm12,<r9=%ymm12
vpxor % ymm15, % ymm12, % ymm12

# qhasm: r = a9 & mem256[input_2 + 32]
# asm 1: vpand 32(<input_2=int64#3),<a9=reg256#15,>r=reg256#16
# asm 2: vpand 32(<input_2=%rdx),<a9=%ymm14,>r=%ymm15
vpand 32( % rdx), % ymm14, % ymm15

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#16,<r10=reg256#14,<r10=reg256#14
# asm 2: vpxor <r=%ymm15,<r10=%ymm13,<r10=%ymm13
vpxor % ymm15, % ymm13, % ymm13

# qhasm: r = a9 & mem256[input_2 + 64]
# asm 1: vpand 64(<input_2=int64#3),<a9=reg256#15,>r=reg256#16
# asm 2: vpand 64(<input_2=%rdx),<a9=%ymm14,>r=%ymm15
vpand 64( % rdx), % ymm14, % ymm15

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#16,<r11=reg256#2,<r11=reg256#2
# asm 2: vpxor <r=%ymm15,<r11=%ymm1,<r11=%ymm1
vpxor % ymm15, % ymm1, % ymm1

# qhasm: r = a9 & mem256[input_2 + 96]
# asm 1: vpand 96(<input_2=int64#3),<a9=reg256#15,>r=reg256#16
# asm 2: vpand 96(<input_2=%rdx),<a9=%ymm14,>r=%ymm15
vpand 96( % rdx), % ymm14, % ymm15

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#16,<r12=reg256#3,<r12=reg256#3
# asm 2: vpxor <r=%ymm15,<r12=%ymm2,<r12=%ymm2
vpxor % ymm15, % ymm2, % ymm2

# qhasm: r = a9 & mem256[input_2 + 128]
# asm 1: vpand 128(<input_2=int64#3),<a9=reg256#15,>r=reg256#16
# asm 2: vpand 128(<input_2=%rdx),<a9=%ymm14,>r=%ymm15
vpand 128( % rdx), % ymm14, % ymm15

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#16,<r13=reg256#4,<r13=reg256#4
# asm 2: vpxor <r=%ymm15,<r13=%ymm3,<r13=%ymm3
vpxor % ymm15, % ymm3, % ymm3

# qhasm: r = a9 & mem256[input_2 + 160]
# asm 1: vpand 160(<input_2=int64#3),<a9=reg256#15,>r=reg256#16
# asm 2: vpand 160(<input_2=%rdx),<a9=%ymm14,>r=%ymm15
vpand 160( % rdx), % ymm14, % ymm15

# qhasm: r14 ^= r
# asm 1: vpxor <r=reg256#16,<r14=reg256#5,<r14=reg256#5
# asm 2: vpxor <r=%ymm15,<r14=%ymm4,<r14=%ymm4
vpxor % ymm15, % ymm4, % ymm4

# qhasm: r = a9 & mem256[input_2 + 192]
# asm 1: vpand 192(<input_2=int64#3),<a9=reg256#15,>r=reg256#16
# asm 2: vpand 192(<input_2=%rdx),<a9=%ymm14,>r=%ymm15
vpand 192( % rdx), % ymm14, % ymm15

# qhasm: r15 ^= r
# asm 1: vpxor <r=reg256#16,<r15=reg256#6,<r15=reg256#6
# asm 2: vpxor <r=%ymm15,<r15=%ymm5,<r15=%ymm5
vpxor % ymm15, % ymm5, % ymm5

# qhasm: r = a9 & mem256[input_2 + 224]
# asm 1: vpand 224(<input_2=int64#3),<a9=reg256#15,>r=reg256#16
# asm 2: vpand 224(<input_2=%rdx),<a9=%ymm14,>r=%ymm15
vpand 224( % rdx), % ymm14, % ymm15

# qhasm: r16 ^= r
# asm 1: vpxor <r=reg256#16,<r16=reg256#7,<r16=reg256#7
# asm 2: vpxor <r=%ymm15,<r16=%ymm6,<r16=%ymm6
vpxor % ymm15, % ymm6, % ymm6

# qhasm: r = a9 & mem256[input_2 + 256]
# asm 1: vpand 256(<input_2=int64#3),<a9=reg256#15,>r=reg256#16
# asm 2: vpand 256(<input_2=%rdx),<a9=%ymm14,>r=%ymm15
vpand 256( % rdx), % ymm14, % ymm15

# qhasm: r17 ^= r
# asm 1: vpxor <r=reg256#16,<r17=reg256#8,<r17=reg256#8
# asm 2: vpxor <r=%ymm15,<r17=%ymm7,<r17=%ymm7
vpxor % ymm15, % ymm7, % ymm7

# qhasm: r = a9 & mem256[input_2 + 288]
# asm 1: vpand 288(<input_2=int64#3),<a9=reg256#15,>r=reg256#16
# asm 2: vpand 288(<input_2=%rdx),<a9=%ymm14,>r=%ymm15
vpand 288( % rdx), % ymm14, % ymm15

# qhasm: r18 ^= r
# asm 1: vpxor <r=reg256#16,<r18=reg256#9,<r18=reg256#9
# asm 2: vpxor <r=%ymm15,<r18=%ymm8,<r18=%ymm8
vpxor % ymm15, % ymm8, % ymm8

# qhasm: r = a9 & mem256[input_2 + 320]
# asm 1: vpand 320(<input_2=int64#3),<a9=reg256#15,>r=reg256#16
# asm 2: vpand 320(<input_2=%rdx),<a9=%ymm14,>r=%ymm15
vpand 320( % rdx), % ymm14, % ymm15

# qhasm: r19 ^= r
# asm 1: vpxor <r=reg256#16,<r19=reg256#10,<r19=reg256#10
# asm 2: vpxor <r=%ymm15,<r19=%ymm9,<r19=%ymm9
vpxor % ymm15, % ymm9, % ymm9

# qhasm: r = a9 & mem256[input_2 + 352]
# asm 1: vpand 352(<input_2=int64#3),<a9=reg256#15,>r=reg256#16
# asm 2: vpand 352(<input_2=%rdx),<a9=%ymm14,>r=%ymm15
vpand 352( % rdx), % ymm14, % ymm15

# qhasm: r20 ^= r
# asm 1: vpxor <r=reg256#16,<r20=reg256#11,<r20=reg256#11
# asm 2: vpxor <r=%ymm15,<r20=%ymm10,<r20=%ymm10
vpxor % ymm15, % ymm10, % ymm10

# qhasm: r = a9 & mem256[input_2 + 384]
# asm 1: vpand 384(<input_2=int64#3),<a9=reg256#15,>r=reg256#15
# asm 2: vpand 384(<input_2=%rdx),<a9=%ymm14,>r=%ymm14
vpand 384( % rdx), % ymm14, % ymm14

# qhasm: r21 ^= r
# asm 1: vpxor <r=reg256#15,<r21=reg256#12,<r21=reg256#12
# asm 2: vpxor <r=%ymm14,<r21=%ymm11,<r21=%ymm11
vpxor % ymm14, % ymm11, % ymm11

# qhasm: r12 ^= r21
# asm 1: vpxor <r21=reg256#12,<r12=reg256#3,<r12=reg256#3
# asm 2: vpxor <r21=%ymm11,<r12=%ymm2,<r12=%ymm2
vpxor % ymm11, % ymm2, % ymm2

# qhasm: r11 ^= r21
# asm 1: vpxor <r21=reg256#12,<r11=reg256#2,<r11=reg256#2
# asm 2: vpxor <r21=%ymm11,<r11=%ymm1,<r11=%ymm1
vpxor % ymm11, % ymm1, % ymm1

# qhasm: r9 ^= r21
# asm 1: vpxor <r21=reg256#12,<r9=reg256#13,<r9=reg256#13
# asm 2: vpxor <r21=%ymm11,<r9=%ymm12,<r9=%ymm12
vpxor % ymm11, % ymm12, % ymm12

# qhasm: r8 = r21
# asm 1: vmovapd <r21=reg256#12,>r8=reg256#12
# asm 2: vmovapd <r21=%ymm11,>r8=%ymm11
vmovapd % ymm11, % ymm11

# qhasm: a8 = mem256[ input_1 + 256 ]
# asm 1: vmovupd   256(<input_1=int64#2),>a8=reg256#15
# asm 2: vmovupd   256(<input_1=%rsi),>a8=%ymm14
vmovupd   256( % rsi), % ymm14

# qhasm: r = a8 & b0
# asm 1: vpand <a8=reg256#15,<b0=reg256#1,>r=reg256#16
# asm 2: vpand <a8=%ymm14,<b0=%ymm0,>r=%ymm15
vpand % ymm14, % ymm0, % ymm15

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#16,<r8=reg256#12,<r8=reg256#12
# asm 2: vpxor <r=%ymm15,<r8=%ymm11,<r8=%ymm11
vpxor % ymm15, % ymm11, % ymm11

# qhasm: r = a8 & mem256[input_2 + 32]
# asm 1: vpand 32(<input_2=int64#3),<a8=reg256#15,>r=reg256#16
# asm 2: vpand 32(<input_2=%rdx),<a8=%ymm14,>r=%ymm15
vpand 32( % rdx), % ymm14, % ymm15

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#16,<r9=reg256#13,<r9=reg256#13
# asm 2: vpxor <r=%ymm15,<r9=%ymm12,<r9=%ymm12
vpxor % ymm15, % ymm12, % ymm12

# qhasm: r = a8 & mem256[input_2 + 64]
# asm 1: vpand 64(<input_2=int64#3),<a8=reg256#15,>r=reg256#16
# asm 2: vpand 64(<input_2=%rdx),<a8=%ymm14,>r=%ymm15
vpand 64( % rdx), % ymm14, % ymm15

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#16,<r10=reg256#14,<r10=reg256#14
# asm 2: vpxor <r=%ymm15,<r10=%ymm13,<r10=%ymm13
vpxor % ymm15, % ymm13, % ymm13

# qhasm: r = a8 & mem256[input_2 + 96]
# asm 1: vpand 96(<input_2=int64#3),<a8=reg256#15,>r=reg256#16
# asm 2: vpand 96(<input_2=%rdx),<a8=%ymm14,>r=%ymm15
vpand 96( % rdx), % ymm14, % ymm15

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#16,<r11=reg256#2,<r11=reg256#2
# asm 2: vpxor <r=%ymm15,<r11=%ymm1,<r11=%ymm1
vpxor % ymm15, % ymm1, % ymm1

# qhasm: r = a8 & mem256[input_2 + 128]
# asm 1: vpand 128(<input_2=int64#3),<a8=reg256#15,>r=reg256#16
# asm 2: vpand 128(<input_2=%rdx),<a8=%ymm14,>r=%ymm15
vpand 128( % rdx), % ymm14, % ymm15

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#16,<r12=reg256#3,<r12=reg256#3
# asm 2: vpxor <r=%ymm15,<r12=%ymm2,<r12=%ymm2
vpxor % ymm15, % ymm2, % ymm2

# qhasm: r = a8 & mem256[input_2 + 160]
# asm 1: vpand 160(<input_2=int64#3),<a8=reg256#15,>r=reg256#16
# asm 2: vpand 160(<input_2=%rdx),<a8=%ymm14,>r=%ymm15
vpand 160( % rdx), % ymm14, % ymm15

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#16,<r13=reg256#4,<r13=reg256#4
# asm 2: vpxor <r=%ymm15,<r13=%ymm3,<r13=%ymm3
vpxor % ymm15, % ymm3, % ymm3

# qhasm: r = a8 & mem256[input_2 + 192]
# asm 1: vpand 192(<input_2=int64#3),<a8=reg256#15,>r=reg256#16
# asm 2: vpand 192(<input_2=%rdx),<a8=%ymm14,>r=%ymm15
vpand 192( % rdx), % ymm14, % ymm15

# qhasm: r14 ^= r
# asm 1: vpxor <r=reg256#16,<r14=reg256#5,<r14=reg256#5
# asm 2: vpxor <r=%ymm15,<r14=%ymm4,<r14=%ymm4
vpxor % ymm15, % ymm4, % ymm4

# qhasm: r = a8 & mem256[input_2 + 224]
# asm 1: vpand 224(<input_2=int64#3),<a8=reg256#15,>r=reg256#16
# asm 2: vpand 224(<input_2=%rdx),<a8=%ymm14,>r=%ymm15
vpand 224( % rdx), % ymm14, % ymm15

# qhasm: r15 ^= r
# asm 1: vpxor <r=reg256#16,<r15=reg256#6,<r15=reg256#6
# asm 2: vpxor <r=%ymm15,<r15=%ymm5,<r15=%ymm5
vpxor % ymm15, % ymm5, % ymm5

# qhasm: r = a8 & mem256[input_2 + 256]
# asm 1: vpand 256(<input_2=int64#3),<a8=reg256#15,>r=reg256#16
# asm 2: vpand 256(<input_2=%rdx),<a8=%ymm14,>r=%ymm15
vpand 256( % rdx), % ymm14, % ymm15

# qhasm: r16 ^= r
# asm 1: vpxor <r=reg256#16,<r16=reg256#7,<r16=reg256#7
# asm 2: vpxor <r=%ymm15,<r16=%ymm6,<r16=%ymm6
vpxor % ymm15, % ymm6, % ymm6

# qhasm: r = a8 & mem256[input_2 + 288]
# asm 1: vpand 288(<input_2=int64#3),<a8=reg256#15,>r=reg256#16
# asm 2: vpand 288(<input_2=%rdx),<a8=%ymm14,>r=%ymm15
vpand 288( % rdx), % ymm14, % ymm15

# qhasm: r17 ^= r
# asm 1: vpxor <r=reg256#16,<r17=reg256#8,<r17=reg256#8
# asm 2: vpxor <r=%ymm15,<r17=%ymm7,<r17=%ymm7
vpxor % ymm15, % ymm7, % ymm7

# qhasm: r = a8 & mem256[input_2 + 320]
# asm 1: vpand 320(<input_2=int64#3),<a8=reg256#15,>r=reg256#16
# asm 2: vpand 320(<input_2=%rdx),<a8=%ymm14,>r=%ymm15
vpand 320( % rdx), % ymm14, % ymm15

# qhasm: r18 ^= r
# asm 1: vpxor <r=reg256#16,<r18=reg256#9,<r18=reg256#9
# asm 2: vpxor <r=%ymm15,<r18=%ymm8,<r18=%ymm8
vpxor % ymm15, % ymm8, % ymm8

# qhasm: r = a8 & mem256[input_2 + 352]
# asm 1: vpand 352(<input_2=int64#3),<a8=reg256#15,>r=reg256#16
# asm 2: vpand 352(<input_2=%rdx),<a8=%ymm14,>r=%ymm15
vpand 352( % rdx), % ymm14, % ymm15

# qhasm: r19 ^= r
# asm 1: vpxor <r=reg256#16,<r19=reg256#10,<r19=reg256#10
# asm 2: vpxor <r=%ymm15,<r19=%ymm9,<r19=%ymm9
vpxor % ymm15, % ymm9, % ymm9

# qhasm: r = a8 & mem256[input_2 + 384]
# asm 1: vpand 384(<input_2=int64#3),<a8=reg256#15,>r=reg256#15
# asm 2: vpand 384(<input_2=%rdx),<a8=%ymm14,>r=%ymm14
vpand 384( % rdx), % ymm14, % ymm14

# qhasm: r20 ^= r
# asm 1: vpxor <r=reg256#15,<r20=reg256#11,<r20=reg256#11
# asm 2: vpxor <r=%ymm14,<r20=%ymm10,<r20=%ymm10
vpxor % ymm14, % ymm10, % ymm10

# qhasm: r11 ^= r20
# asm 1: vpxor <r20=reg256#11,<r11=reg256#2,<r11=reg256#2
# asm 2: vpxor <r20=%ymm10,<r11=%ymm1,<r11=%ymm1
vpxor % ymm10, % ymm1, % ymm1

# qhasm: r10 ^= r20
# asm 1: vpxor <r20=reg256#11,<r10=reg256#14,<r10=reg256#14
# asm 2: vpxor <r20=%ymm10,<r10=%ymm13,<r10=%ymm13
vpxor % ymm10, % ymm13, % ymm13

# qhasm: r8 ^= r20
# asm 1: vpxor <r20=reg256#11,<r8=reg256#12,<r8=reg256#12
# asm 2: vpxor <r20=%ymm10,<r8=%ymm11,<r8=%ymm11
vpxor % ymm10, % ymm11, % ymm11

# qhasm: r7 = r20
# asm 1: vmovapd <r20=reg256#11,>r7=reg256#11
# asm 2: vmovapd <r20=%ymm10,>r7=%ymm10
vmovapd % ymm10, % ymm10

# qhasm: a7 = mem256[ input_1 + 224 ]
# asm 1: vmovupd   224(<input_1=int64#2),>a7=reg256#15
# asm 2: vmovupd   224(<input_1=%rsi),>a7=%ymm14
vmovupd   224( % rsi), % ymm14

# qhasm: r = a7 & b0
# asm 1: vpand <a7=reg256#15,<b0=reg256#1,>r=reg256#16
# asm 2: vpand <a7=%ymm14,<b0=%ymm0,>r=%ymm15
vpand % ymm14, % ymm0, % ymm15

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#16,<r7=reg256#11,<r7=reg256#11
# asm 2: vpxor <r=%ymm15,<r7=%ymm10,<r7=%ymm10
vpxor % ymm15, % ymm10, % ymm10

# qhasm: r = a7 & mem256[input_2 + 32]
# asm 1: vpand 32(<input_2=int64#3),<a7=reg256#15,>r=reg256#16
# asm 2: vpand 32(<input_2=%rdx),<a7=%ymm14,>r=%ymm15
vpand 32( % rdx), % ymm14, % ymm15

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#16,<r8=reg256#12,<r8=reg256#12
# asm 2: vpxor <r=%ymm15,<r8=%ymm11,<r8=%ymm11
vpxor % ymm15, % ymm11, % ymm11

# qhasm: r = a7 & mem256[input_2 + 64]
# asm 1: vpand 64(<input_2=int64#3),<a7=reg256#15,>r=reg256#16
# asm 2: vpand 64(<input_2=%rdx),<a7=%ymm14,>r=%ymm15
vpand 64( % rdx), % ymm14, % ymm15

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#16,<r9=reg256#13,<r9=reg256#13
# asm 2: vpxor <r=%ymm15,<r9=%ymm12,<r9=%ymm12
vpxor % ymm15, % ymm12, % ymm12

# qhasm: r = a7 & mem256[input_2 + 96]
# asm 1: vpand 96(<input_2=int64#3),<a7=reg256#15,>r=reg256#16
# asm 2: vpand 96(<input_2=%rdx),<a7=%ymm14,>r=%ymm15
vpand 96( % rdx), % ymm14, % ymm15

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#16,<r10=reg256#14,<r10=reg256#14
# asm 2: vpxor <r=%ymm15,<r10=%ymm13,<r10=%ymm13
vpxor % ymm15, % ymm13, % ymm13

# qhasm: r = a7 & mem256[input_2 + 128]
# asm 1: vpand 128(<input_2=int64#3),<a7=reg256#15,>r=reg256#16
# asm 2: vpand 128(<input_2=%rdx),<a7=%ymm14,>r=%ymm15
vpand 128( % rdx), % ymm14, % ymm15

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#16,<r11=reg256#2,<r11=reg256#2
# asm 2: vpxor <r=%ymm15,<r11=%ymm1,<r11=%ymm1
vpxor % ymm15, % ymm1, % ymm1

# qhasm: r = a7 & mem256[input_2 + 160]
# asm 1: vpand 160(<input_2=int64#3),<a7=reg256#15,>r=reg256#16
# asm 2: vpand 160(<input_2=%rdx),<a7=%ymm14,>r=%ymm15
vpand 160( % rdx), % ymm14, % ymm15

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#16,<r12=reg256#3,<r12=reg256#3
# asm 2: vpxor <r=%ymm15,<r12=%ymm2,<r12=%ymm2
vpxor % ymm15, % ymm2, % ymm2

# qhasm: r = a7 & mem256[input_2 + 192]
# asm 1: vpand 192(<input_2=int64#3),<a7=reg256#15,>r=reg256#16
# asm 2: vpand 192(<input_2=%rdx),<a7=%ymm14,>r=%ymm15
vpand 192( % rdx), % ymm14, % ymm15

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#16,<r13=reg256#4,<r13=reg256#4
# asm 2: vpxor <r=%ymm15,<r13=%ymm3,<r13=%ymm3
vpxor % ymm15, % ymm3, % ymm3

# qhasm: r = a7 & mem256[input_2 + 224]
# asm 1: vpand 224(<input_2=int64#3),<a7=reg256#15,>r=reg256#16
# asm 2: vpand 224(<input_2=%rdx),<a7=%ymm14,>r=%ymm15
vpand 224( % rdx), % ymm14, % ymm15

# qhasm: r14 ^= r
# asm 1: vpxor <r=reg256#16,<r14=reg256#5,<r14=reg256#5
# asm 2: vpxor <r=%ymm15,<r14=%ymm4,<r14=%ymm4
vpxor % ymm15, % ymm4, % ymm4

# qhasm: r = a7 & mem256[input_2 + 256]
# asm 1: vpand 256(<input_2=int64#3),<a7=reg256#15,>r=reg256#16
# asm 2: vpand 256(<input_2=%rdx),<a7=%ymm14,>r=%ymm15
vpand 256( % rdx), % ymm14, % ymm15

# qhasm: r15 ^= r
# asm 1: vpxor <r=reg256#16,<r15=reg256#6,<r15=reg256#6
# asm 2: vpxor <r=%ymm15,<r15=%ymm5,<r15=%ymm5
vpxor % ymm15, % ymm5, % ymm5

# qhasm: r = a7 & mem256[input_2 + 288]
# asm 1: vpand 288(<input_2=int64#3),<a7=reg256#15,>r=reg256#16
# asm 2: vpand 288(<input_2=%rdx),<a7=%ymm14,>r=%ymm15
vpand 288( % rdx), % ymm14, % ymm15

# qhasm: r16 ^= r
# asm 1: vpxor <r=reg256#16,<r16=reg256#7,<r16=reg256#7
# asm 2: vpxor <r=%ymm15,<r16=%ymm6,<r16=%ymm6
vpxor % ymm15, % ymm6, % ymm6

# qhasm: r = a7 & mem256[input_2 + 320]
# asm 1: vpand 320(<input_2=int64#3),<a7=reg256#15,>r=reg256#16
# asm 2: vpand 320(<input_2=%rdx),<a7=%ymm14,>r=%ymm15
vpand 320( % rdx), % ymm14, % ymm15

# qhasm: r17 ^= r
# asm 1: vpxor <r=reg256#16,<r17=reg256#8,<r17=reg256#8
# asm 2: vpxor <r=%ymm15,<r17=%ymm7,<r17=%ymm7
vpxor % ymm15, % ymm7, % ymm7

# qhasm: r = a7 & mem256[input_2 + 352]
# asm 1: vpand 352(<input_2=int64#3),<a7=reg256#15,>r=reg256#16
# asm 2: vpand 352(<input_2=%rdx),<a7=%ymm14,>r=%ymm15
vpand 352( % rdx), % ymm14, % ymm15

# qhasm: r18 ^= r
# asm 1: vpxor <r=reg256#16,<r18=reg256#9,<r18=reg256#9
# asm 2: vpxor <r=%ymm15,<r18=%ymm8,<r18=%ymm8
vpxor % ymm15, % ymm8, % ymm8

# qhasm: r = a7 & mem256[input_2 + 384]
# asm 1: vpand 384(<input_2=int64#3),<a7=reg256#15,>r=reg256#15
# asm 2: vpand 384(<input_2=%rdx),<a7=%ymm14,>r=%ymm14
vpand 384( % rdx), % ymm14, % ymm14

# qhasm: r19 ^= r
# asm 1: vpxor <r=reg256#15,<r19=reg256#10,<r19=reg256#10
# asm 2: vpxor <r=%ymm14,<r19=%ymm9,<r19=%ymm9
vpxor % ymm14, % ymm9, % ymm9

# qhasm: r10 ^= r19
# asm 1: vpxor <r19=reg256#10,<r10=reg256#14,<r10=reg256#14
# asm 2: vpxor <r19=%ymm9,<r10=%ymm13,<r10=%ymm13
vpxor % ymm9, % ymm13, % ymm13

# qhasm: r9 ^= r19
# asm 1: vpxor <r19=reg256#10,<r9=reg256#13,<r9=reg256#13
# asm 2: vpxor <r19=%ymm9,<r9=%ymm12,<r9=%ymm12
vpxor % ymm9, % ymm12, % ymm12

# qhasm: r7 ^= r19
# asm 1: vpxor <r19=reg256#10,<r7=reg256#11,<r7=reg256#11
# asm 2: vpxor <r19=%ymm9,<r7=%ymm10,<r7=%ymm10
vpxor % ymm9, % ymm10, % ymm10

# qhasm: r6 = r19
# asm 1: vmovapd <r19=reg256#10,>r6=reg256#10
# asm 2: vmovapd <r19=%ymm9,>r6=%ymm9
vmovapd % ymm9, % ymm9

# qhasm: a6 = mem256[ input_1 + 192 ]
# asm 1: vmovupd   192(<input_1=int64#2),>a6=reg256#15
# asm 2: vmovupd   192(<input_1=%rsi),>a6=%ymm14
vmovupd   192( % rsi), % ymm14

# qhasm: r = a6 & b0
# asm 1: vpand <a6=reg256#15,<b0=reg256#1,>r=reg256#16
# asm 2: vpand <a6=%ymm14,<b0=%ymm0,>r=%ymm15
vpand % ymm14, % ymm0, % ymm15

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#16,<r6=reg256#10,<r6=reg256#10
# asm 2: vpxor <r=%ymm15,<r6=%ymm9,<r6=%ymm9
vpxor % ymm15, % ymm9, % ymm9

# qhasm: r = a6 & mem256[input_2 + 32]
# asm 1: vpand 32(<input_2=int64#3),<a6=reg256#15,>r=reg256#16
# asm 2: vpand 32(<input_2=%rdx),<a6=%ymm14,>r=%ymm15
vpand 32( % rdx), % ymm14, % ymm15

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#16,<r7=reg256#11,<r7=reg256#11
# asm 2: vpxor <r=%ymm15,<r7=%ymm10,<r7=%ymm10
vpxor % ymm15, % ymm10, % ymm10

# qhasm: r = a6 & mem256[input_2 + 64]
# asm 1: vpand 64(<input_2=int64#3),<a6=reg256#15,>r=reg256#16
# asm 2: vpand 64(<input_2=%rdx),<a6=%ymm14,>r=%ymm15
vpand 64( % rdx), % ymm14, % ymm15

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#16,<r8=reg256#12,<r8=reg256#12
# asm 2: vpxor <r=%ymm15,<r8=%ymm11,<r8=%ymm11
vpxor % ymm15, % ymm11, % ymm11

# qhasm: r = a6 & mem256[input_2 + 96]
# asm 1: vpand 96(<input_2=int64#3),<a6=reg256#15,>r=reg256#16
# asm 2: vpand 96(<input_2=%rdx),<a6=%ymm14,>r=%ymm15
vpand 96( % rdx), % ymm14, % ymm15

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#16,<r9=reg256#13,<r9=reg256#13
# asm 2: vpxor <r=%ymm15,<r9=%ymm12,<r9=%ymm12
vpxor % ymm15, % ymm12, % ymm12

# qhasm: r = a6 & mem256[input_2 + 128]
# asm 1: vpand 128(<input_2=int64#3),<a6=reg256#15,>r=reg256#16
# asm 2: vpand 128(<input_2=%rdx),<a6=%ymm14,>r=%ymm15
vpand 128( % rdx), % ymm14, % ymm15

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#16,<r10=reg256#14,<r10=reg256#14
# asm 2: vpxor <r=%ymm15,<r10=%ymm13,<r10=%ymm13
vpxor % ymm15, % ymm13, % ymm13

# qhasm: r = a6 & mem256[input_2 + 160]
# asm 1: vpand 160(<input_2=int64#3),<a6=reg256#15,>r=reg256#16
# asm 2: vpand 160(<input_2=%rdx),<a6=%ymm14,>r=%ymm15
vpand 160( % rdx), % ymm14, % ymm15

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#16,<r11=reg256#2,<r11=reg256#2
# asm 2: vpxor <r=%ymm15,<r11=%ymm1,<r11=%ymm1
vpxor % ymm15, % ymm1, % ymm1

# qhasm: r = a6 & mem256[input_2 + 192]
# asm 1: vpand 192(<input_2=int64#3),<a6=reg256#15,>r=reg256#16
# asm 2: vpand 192(<input_2=%rdx),<a6=%ymm14,>r=%ymm15
vpand 192( % rdx), % ymm14, % ymm15

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#16,<r12=reg256#3,<r12=reg256#3
# asm 2: vpxor <r=%ymm15,<r12=%ymm2,<r12=%ymm2
vpxor % ymm15, % ymm2, % ymm2

# qhasm: r = a6 & mem256[input_2 + 224]
# asm 1: vpand 224(<input_2=int64#3),<a6=reg256#15,>r=reg256#16
# asm 2: vpand 224(<input_2=%rdx),<a6=%ymm14,>r=%ymm15
vpand 224( % rdx), % ymm14, % ymm15

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#16,<r13=reg256#4,<r13=reg256#4
# asm 2: vpxor <r=%ymm15,<r13=%ymm3,<r13=%ymm3
vpxor % ymm15, % ymm3, % ymm3

# qhasm: r = a6 & mem256[input_2 + 256]
# asm 1: vpand 256(<input_2=int64#3),<a6=reg256#15,>r=reg256#16
# asm 2: vpand 256(<input_2=%rdx),<a6=%ymm14,>r=%ymm15
vpand 256( % rdx), % ymm14, % ymm15

# qhasm: r14 ^= r
# asm 1: vpxor <r=reg256#16,<r14=reg256#5,<r14=reg256#5
# asm 2: vpxor <r=%ymm15,<r14=%ymm4,<r14=%ymm4
vpxor % ymm15, % ymm4, % ymm4

# qhasm: r = a6 & mem256[input_2 + 288]
# asm 1: vpand 288(<input_2=int64#3),<a6=reg256#15,>r=reg256#16
# asm 2: vpand 288(<input_2=%rdx),<a6=%ymm14,>r=%ymm15
vpand 288( % rdx), % ymm14, % ymm15

# qhasm: r15 ^= r
# asm 1: vpxor <r=reg256#16,<r15=reg256#6,<r15=reg256#6
# asm 2: vpxor <r=%ymm15,<r15=%ymm5,<r15=%ymm5
vpxor % ymm15, % ymm5, % ymm5

# qhasm: r = a6 & mem256[input_2 + 320]
# asm 1: vpand 320(<input_2=int64#3),<a6=reg256#15,>r=reg256#16
# asm 2: vpand 320(<input_2=%rdx),<a6=%ymm14,>r=%ymm15
vpand 320( % rdx), % ymm14, % ymm15

# qhasm: r16 ^= r
# asm 1: vpxor <r=reg256#16,<r16=reg256#7,<r16=reg256#7
# asm 2: vpxor <r=%ymm15,<r16=%ymm6,<r16=%ymm6
vpxor % ymm15, % ymm6, % ymm6

# qhasm: r = a6 & mem256[input_2 + 352]
# asm 1: vpand 352(<input_2=int64#3),<a6=reg256#15,>r=reg256#16
# asm 2: vpand 352(<input_2=%rdx),<a6=%ymm14,>r=%ymm15
vpand 352( % rdx), % ymm14, % ymm15

# qhasm: r17 ^= r
# asm 1: vpxor <r=reg256#16,<r17=reg256#8,<r17=reg256#8
# asm 2: vpxor <r=%ymm15,<r17=%ymm7,<r17=%ymm7
vpxor % ymm15, % ymm7, % ymm7

# qhasm: r = a6 & mem256[input_2 + 384]
# asm 1: vpand 384(<input_2=int64#3),<a6=reg256#15,>r=reg256#15
# asm 2: vpand 384(<input_2=%rdx),<a6=%ymm14,>r=%ymm14
vpand 384( % rdx), % ymm14, % ymm14

# qhasm: r18 ^= r
# asm 1: vpxor <r=reg256#15,<r18=reg256#9,<r18=reg256#9
# asm 2: vpxor <r=%ymm14,<r18=%ymm8,<r18=%ymm8
vpxor % ymm14, % ymm8, % ymm8

# qhasm: r9 ^= r18
# asm 1: vpxor <r18=reg256#9,<r9=reg256#13,<r9=reg256#13
# asm 2: vpxor <r18=%ymm8,<r9=%ymm12,<r9=%ymm12
vpxor % ymm8, % ymm12, % ymm12

# qhasm: r8 ^= r18
# asm 1: vpxor <r18=reg256#9,<r8=reg256#12,<r8=reg256#12
# asm 2: vpxor <r18=%ymm8,<r8=%ymm11,<r8=%ymm11
vpxor % ymm8, % ymm11, % ymm11

# qhasm: r6 ^= r18
# asm 1: vpxor <r18=reg256#9,<r6=reg256#10,<r6=reg256#10
# asm 2: vpxor <r18=%ymm8,<r6=%ymm9,<r6=%ymm9
vpxor % ymm8, % ymm9, % ymm9

# qhasm: r5 = r18
# asm 1: vmovapd <r18=reg256#9,>r5=reg256#9
# asm 2: vmovapd <r18=%ymm8,>r5=%ymm8
vmovapd % ymm8, % ymm8

# qhasm: a5 = mem256[ input_1 + 160 ]
# asm 1: vmovupd   160(<input_1=int64#2),>a5=reg256#15
# asm 2: vmovupd   160(<input_1=%rsi),>a5=%ymm14
vmovupd   160( % rsi), % ymm14

# qhasm: r = a5 & b0
# asm 1: vpand <a5=reg256#15,<b0=reg256#1,>r=reg256#16
# asm 2: vpand <a5=%ymm14,<b0=%ymm0,>r=%ymm15
vpand % ymm14, % ymm0, % ymm15

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#16,<r5=reg256#9,<r5=reg256#9
# asm 2: vpxor <r=%ymm15,<r5=%ymm8,<r5=%ymm8
vpxor % ymm15, % ymm8, % ymm8

# qhasm: r = a5 & mem256[input_2 + 32]
# asm 1: vpand 32(<input_2=int64#3),<a5=reg256#15,>r=reg256#16
# asm 2: vpand 32(<input_2=%rdx),<a5=%ymm14,>r=%ymm15
vpand 32( % rdx), % ymm14, % ymm15

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#16,<r6=reg256#10,<r6=reg256#10
# asm 2: vpxor <r=%ymm15,<r6=%ymm9,<r6=%ymm9
vpxor % ymm15, % ymm9, % ymm9

# qhasm: r = a5 & mem256[input_2 + 64]
# asm 1: vpand 64(<input_2=int64#3),<a5=reg256#15,>r=reg256#16
# asm 2: vpand 64(<input_2=%rdx),<a5=%ymm14,>r=%ymm15
vpand 64( % rdx), % ymm14, % ymm15

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#16,<r7=reg256#11,<r7=reg256#11
# asm 2: vpxor <r=%ymm15,<r7=%ymm10,<r7=%ymm10
vpxor % ymm15, % ymm10, % ymm10

# qhasm: r = a5 & mem256[input_2 + 96]
# asm 1: vpand 96(<input_2=int64#3),<a5=reg256#15,>r=reg256#16
# asm 2: vpand 96(<input_2=%rdx),<a5=%ymm14,>r=%ymm15
vpand 96( % rdx), % ymm14, % ymm15

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#16,<r8=reg256#12,<r8=reg256#12
# asm 2: vpxor <r=%ymm15,<r8=%ymm11,<r8=%ymm11
vpxor % ymm15, % ymm11, % ymm11

# qhasm: r = a5 & mem256[input_2 + 128]
# asm 1: vpand 128(<input_2=int64#3),<a5=reg256#15,>r=reg256#16
# asm 2: vpand 128(<input_2=%rdx),<a5=%ymm14,>r=%ymm15
vpand 128( % rdx), % ymm14, % ymm15

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#16,<r9=reg256#13,<r9=reg256#13
# asm 2: vpxor <r=%ymm15,<r9=%ymm12,<r9=%ymm12
vpxor % ymm15, % ymm12, % ymm12

# qhasm: r = a5 & mem256[input_2 + 160]
# asm 1: vpand 160(<input_2=int64#3),<a5=reg256#15,>r=reg256#16
# asm 2: vpand 160(<input_2=%rdx),<a5=%ymm14,>r=%ymm15
vpand 160( % rdx), % ymm14, % ymm15

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#16,<r10=reg256#14,<r10=reg256#14
# asm 2: vpxor <r=%ymm15,<r10=%ymm13,<r10=%ymm13
vpxor % ymm15, % ymm13, % ymm13

# qhasm: r = a5 & mem256[input_2 + 192]
# asm 1: vpand 192(<input_2=int64#3),<a5=reg256#15,>r=reg256#16
# asm 2: vpand 192(<input_2=%rdx),<a5=%ymm14,>r=%ymm15
vpand 192( % rdx), % ymm14, % ymm15

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#16,<r11=reg256#2,<r11=reg256#2
# asm 2: vpxor <r=%ymm15,<r11=%ymm1,<r11=%ymm1
vpxor % ymm15, % ymm1, % ymm1

# qhasm: r = a5 & mem256[input_2 + 224]
# asm 1: vpand 224(<input_2=int64#3),<a5=reg256#15,>r=reg256#16
# asm 2: vpand 224(<input_2=%rdx),<a5=%ymm14,>r=%ymm15
vpand 224( % rdx), % ymm14, % ymm15

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#16,<r12=reg256#3,<r12=reg256#3
# asm 2: vpxor <r=%ymm15,<r12=%ymm2,<r12=%ymm2
vpxor % ymm15, % ymm2, % ymm2

# qhasm: r = a5 & mem256[input_2 + 256]
# asm 1: vpand 256(<input_2=int64#3),<a5=reg256#15,>r=reg256#16
# asm 2: vpand 256(<input_2=%rdx),<a5=%ymm14,>r=%ymm15
vpand 256( % rdx), % ymm14, % ymm15

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#16,<r13=reg256#4,<r13=reg256#4
# asm 2: vpxor <r=%ymm15,<r13=%ymm3,<r13=%ymm3
vpxor % ymm15, % ymm3, % ymm3

# qhasm: r = a5 & mem256[input_2 + 288]
# asm 1: vpand 288(<input_2=int64#3),<a5=reg256#15,>r=reg256#16
# asm 2: vpand 288(<input_2=%rdx),<a5=%ymm14,>r=%ymm15
vpand 288( % rdx), % ymm14, % ymm15

# qhasm: r14 ^= r
# asm 1: vpxor <r=reg256#16,<r14=reg256#5,<r14=reg256#5
# asm 2: vpxor <r=%ymm15,<r14=%ymm4,<r14=%ymm4
vpxor % ymm15, % ymm4, % ymm4

# qhasm: r = a5 & mem256[input_2 + 320]
# asm 1: vpand 320(<input_2=int64#3),<a5=reg256#15,>r=reg256#16
# asm 2: vpand 320(<input_2=%rdx),<a5=%ymm14,>r=%ymm15
vpand 320( % rdx), % ymm14, % ymm15

# qhasm: r15 ^= r
# asm 1: vpxor <r=reg256#16,<r15=reg256#6,<r15=reg256#6
# asm 2: vpxor <r=%ymm15,<r15=%ymm5,<r15=%ymm5
vpxor % ymm15, % ymm5, % ymm5

# qhasm: r = a5 & mem256[input_2 + 352]
# asm 1: vpand 352(<input_2=int64#3),<a5=reg256#15,>r=reg256#16
# asm 2: vpand 352(<input_2=%rdx),<a5=%ymm14,>r=%ymm15
vpand 352( % rdx), % ymm14, % ymm15

# qhasm: r16 ^= r
# asm 1: vpxor <r=reg256#16,<r16=reg256#7,<r16=reg256#7
# asm 2: vpxor <r=%ymm15,<r16=%ymm6,<r16=%ymm6
vpxor % ymm15, % ymm6, % ymm6

# qhasm: r = a5 & mem256[input_2 + 384]
# asm 1: vpand 384(<input_2=int64#3),<a5=reg256#15,>r=reg256#15
# asm 2: vpand 384(<input_2=%rdx),<a5=%ymm14,>r=%ymm14
vpand 384( % rdx), % ymm14, % ymm14

# qhasm: r17 ^= r
# asm 1: vpxor <r=reg256#15,<r17=reg256#8,<r17=reg256#8
# asm 2: vpxor <r=%ymm14,<r17=%ymm7,<r17=%ymm7
vpxor % ymm14, % ymm7, % ymm7

# qhasm: r8 ^= r17
# asm 1: vpxor <r17=reg256#8,<r8=reg256#12,<r8=reg256#12
# asm 2: vpxor <r17=%ymm7,<r8=%ymm11,<r8=%ymm11
vpxor % ymm7, % ymm11, % ymm11

# qhasm: r7 ^= r17
# asm 1: vpxor <r17=reg256#8,<r7=reg256#11,<r7=reg256#11
# asm 2: vpxor <r17=%ymm7,<r7=%ymm10,<r7=%ymm10
vpxor % ymm7, % ymm10, % ymm10

# qhasm: r5 ^= r17
# asm 1: vpxor <r17=reg256#8,<r5=reg256#9,<r5=reg256#9
# asm 2: vpxor <r17=%ymm7,<r5=%ymm8,<r5=%ymm8
vpxor % ymm7, % ymm8, % ymm8

# qhasm: r4 = r17
# asm 1: vmovapd <r17=reg256#8,>r4=reg256#8
# asm 2: vmovapd <r17=%ymm7,>r4=%ymm7
vmovapd % ymm7, % ymm7

# qhasm: a4 = mem256[ input_1 + 128 ]
# asm 1: vmovupd   128(<input_1=int64#2),>a4=reg256#15
# asm 2: vmovupd   128(<input_1=%rsi),>a4=%ymm14
vmovupd   128( % rsi), % ymm14

# qhasm: r = a4 & b0
# asm 1: vpand <a4=reg256#15,<b0=reg256#1,>r=reg256#16
# asm 2: vpand <a4=%ymm14,<b0=%ymm0,>r=%ymm15
vpand % ymm14, % ymm0, % ymm15

# qhasm: r4 ^= r
# asm 1: vpxor <r=reg256#16,<r4=reg256#8,<r4=reg256#8
# asm 2: vpxor <r=%ymm15,<r4=%ymm7,<r4=%ymm7
vpxor % ymm15, % ymm7, % ymm7

# qhasm: r = a4 & mem256[input_2 + 32]
# asm 1: vpand 32(<input_2=int64#3),<a4=reg256#15,>r=reg256#16
# asm 2: vpand 32(<input_2=%rdx),<a4=%ymm14,>r=%ymm15
vpand 32( % rdx), % ymm14, % ymm15

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#16,<r5=reg256#9,<r5=reg256#9
# asm 2: vpxor <r=%ymm15,<r5=%ymm8,<r5=%ymm8
vpxor % ymm15, % ymm8, % ymm8

# qhasm: r = a4 & mem256[input_2 + 64]
# asm 1: vpand 64(<input_2=int64#3),<a4=reg256#15,>r=reg256#16
# asm 2: vpand 64(<input_2=%rdx),<a4=%ymm14,>r=%ymm15
vpand 64( % rdx), % ymm14, % ymm15

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#16,<r6=reg256#10,<r6=reg256#10
# asm 2: vpxor <r=%ymm15,<r6=%ymm9,<r6=%ymm9
vpxor % ymm15, % ymm9, % ymm9

# qhasm: r = a4 & mem256[input_2 + 96]
# asm 1: vpand 96(<input_2=int64#3),<a4=reg256#15,>r=reg256#16
# asm 2: vpand 96(<input_2=%rdx),<a4=%ymm14,>r=%ymm15
vpand 96( % rdx), % ymm14, % ymm15

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#16,<r7=reg256#11,<r7=reg256#11
# asm 2: vpxor <r=%ymm15,<r7=%ymm10,<r7=%ymm10
vpxor % ymm15, % ymm10, % ymm10

# qhasm: r = a4 & mem256[input_2 + 128]
# asm 1: vpand 128(<input_2=int64#3),<a4=reg256#15,>r=reg256#16
# asm 2: vpand 128(<input_2=%rdx),<a4=%ymm14,>r=%ymm15
vpand 128( % rdx), % ymm14, % ymm15

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#16,<r8=reg256#12,<r8=reg256#12
# asm 2: vpxor <r=%ymm15,<r8=%ymm11,<r8=%ymm11
vpxor % ymm15, % ymm11, % ymm11

# qhasm: r = a4 & mem256[input_2 + 160]
# asm 1: vpand 160(<input_2=int64#3),<a4=reg256#15,>r=reg256#16
# asm 2: vpand 160(<input_2=%rdx),<a4=%ymm14,>r=%ymm15
vpand 160( % rdx), % ymm14, % ymm15

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#16,<r9=reg256#13,<r9=reg256#13
# asm 2: vpxor <r=%ymm15,<r9=%ymm12,<r9=%ymm12
vpxor % ymm15, % ymm12, % ymm12

# qhasm: r = a4 & mem256[input_2 + 192]
# asm 1: vpand 192(<input_2=int64#3),<a4=reg256#15,>r=reg256#16
# asm 2: vpand 192(<input_2=%rdx),<a4=%ymm14,>r=%ymm15
vpand 192( % rdx), % ymm14, % ymm15

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#16,<r10=reg256#14,<r10=reg256#14
# asm 2: vpxor <r=%ymm15,<r10=%ymm13,<r10=%ymm13
vpxor % ymm15, % ymm13, % ymm13

# qhasm: r = a4 & mem256[input_2 + 224]
# asm 1: vpand 224(<input_2=int64#3),<a4=reg256#15,>r=reg256#16
# asm 2: vpand 224(<input_2=%rdx),<a4=%ymm14,>r=%ymm15
vpand 224( % rdx), % ymm14, % ymm15

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#16,<r11=reg256#2,<r11=reg256#2
# asm 2: vpxor <r=%ymm15,<r11=%ymm1,<r11=%ymm1
vpxor % ymm15, % ymm1, % ymm1

# qhasm: r = a4 & mem256[input_2 + 256]
# asm 1: vpand 256(<input_2=int64#3),<a4=reg256#15,>r=reg256#16
# asm 2: vpand 256(<input_2=%rdx),<a4=%ymm14,>r=%ymm15
vpand 256( % rdx), % ymm14, % ymm15

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#16,<r12=reg256#3,<r12=reg256#3
# asm 2: vpxor <r=%ymm15,<r12=%ymm2,<r12=%ymm2
vpxor % ymm15, % ymm2, % ymm2

# qhasm: r = a4 & mem256[input_2 + 288]
# asm 1: vpand 288(<input_2=int64#3),<a4=reg256#15,>r=reg256#16
# asm 2: vpand 288(<input_2=%rdx),<a4=%ymm14,>r=%ymm15
vpand 288( % rdx), % ymm14, % ymm15

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#16,<r13=reg256#4,<r13=reg256#4
# asm 2: vpxor <r=%ymm15,<r13=%ymm3,<r13=%ymm3
vpxor % ymm15, % ymm3, % ymm3

# qhasm: r = a4 & mem256[input_2 + 320]
# asm 1: vpand 320(<input_2=int64#3),<a4=reg256#15,>r=reg256#16
# asm 2: vpand 320(<input_2=%rdx),<a4=%ymm14,>r=%ymm15
vpand 320( % rdx), % ymm14, % ymm15

# qhasm: r14 ^= r
# asm 1: vpxor <r=reg256#16,<r14=reg256#5,<r14=reg256#5
# asm 2: vpxor <r=%ymm15,<r14=%ymm4,<r14=%ymm4
vpxor % ymm15, % ymm4, % ymm4

# qhasm: r = a4 & mem256[input_2 + 352]
# asm 1: vpand 352(<input_2=int64#3),<a4=reg256#15,>r=reg256#16
# asm 2: vpand 352(<input_2=%rdx),<a4=%ymm14,>r=%ymm15
vpand 352( % rdx), % ymm14, % ymm15

# qhasm: r15 ^= r
# asm 1: vpxor <r=reg256#16,<r15=reg256#6,<r15=reg256#6
# asm 2: vpxor <r=%ymm15,<r15=%ymm5,<r15=%ymm5
vpxor % ymm15, % ymm5, % ymm5

# qhasm: r = a4 & mem256[input_2 + 384]
# asm 1: vpand 384(<input_2=int64#3),<a4=reg256#15,>r=reg256#15
# asm 2: vpand 384(<input_2=%rdx),<a4=%ymm14,>r=%ymm14
vpand 384( % rdx), % ymm14, % ymm14

# qhasm: r16 ^= r
# asm 1: vpxor <r=reg256#15,<r16=reg256#7,<r16=reg256#7
# asm 2: vpxor <r=%ymm14,<r16=%ymm6,<r16=%ymm6
vpxor % ymm14, % ymm6, % ymm6

# qhasm: r7 ^= r16
# asm 1: vpxor <r16=reg256#7,<r7=reg256#11,<r7=reg256#11
# asm 2: vpxor <r16=%ymm6,<r7=%ymm10,<r7=%ymm10
vpxor % ymm6, % ymm10, % ymm10

# qhasm: r6 ^= r16
# asm 1: vpxor <r16=reg256#7,<r6=reg256#10,<r6=reg256#10
# asm 2: vpxor <r16=%ymm6,<r6=%ymm9,<r6=%ymm9
vpxor % ymm6, % ymm9, % ymm9

# qhasm: r4 ^= r16
# asm 1: vpxor <r16=reg256#7,<r4=reg256#8,<r4=reg256#8
# asm 2: vpxor <r16=%ymm6,<r4=%ymm7,<r4=%ymm7
vpxor % ymm6, % ymm7, % ymm7

# qhasm: r3 = r16
# asm 1: vmovapd <r16=reg256#7,>r3=reg256#7
# asm 2: vmovapd <r16=%ymm6,>r3=%ymm6
vmovapd % ymm6, % ymm6

# qhasm: a3 = mem256[ input_1 + 96 ]
# asm 1: vmovupd   96(<input_1=int64#2),>a3=reg256#15
# asm 2: vmovupd   96(<input_1=%rsi),>a3=%ymm14
vmovupd   96( % rsi), % ymm14

# qhasm: r = a3 & b0
# asm 1: vpand <a3=reg256#15,<b0=reg256#1,>r=reg256#16
# asm 2: vpand <a3=%ymm14,<b0=%ymm0,>r=%ymm15
vpand % ymm14, % ymm0, % ymm15

# qhasm: r3 ^= r
# asm 1: vpxor <r=reg256#16,<r3=reg256#7,<r3=reg256#7
# asm 2: vpxor <r=%ymm15,<r3=%ymm6,<r3=%ymm6
vpxor % ymm15, % ymm6, % ymm6

# qhasm: r = a3 & mem256[input_2 + 32]
# asm 1: vpand 32(<input_2=int64#3),<a3=reg256#15,>r=reg256#16
# asm 2: vpand 32(<input_2=%rdx),<a3=%ymm14,>r=%ymm15
vpand 32( % rdx), % ymm14, % ymm15

# qhasm: r4 ^= r
# asm 1: vpxor <r=reg256#16,<r4=reg256#8,<r4=reg256#8
# asm 2: vpxor <r=%ymm15,<r4=%ymm7,<r4=%ymm7
vpxor % ymm15, % ymm7, % ymm7

# qhasm: r = a3 & mem256[input_2 + 64]
# asm 1: vpand 64(<input_2=int64#3),<a3=reg256#15,>r=reg256#16
# asm 2: vpand 64(<input_2=%rdx),<a3=%ymm14,>r=%ymm15
vpand 64( % rdx), % ymm14, % ymm15

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#16,<r5=reg256#9,<r5=reg256#9
# asm 2: vpxor <r=%ymm15,<r5=%ymm8,<r5=%ymm8
vpxor % ymm15, % ymm8, % ymm8

# qhasm: r = a3 & mem256[input_2 + 96]
# asm 1: vpand 96(<input_2=int64#3),<a3=reg256#15,>r=reg256#16
# asm 2: vpand 96(<input_2=%rdx),<a3=%ymm14,>r=%ymm15
vpand 96( % rdx), % ymm14, % ymm15

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#16,<r6=reg256#10,<r6=reg256#10
# asm 2: vpxor <r=%ymm15,<r6=%ymm9,<r6=%ymm9
vpxor % ymm15, % ymm9, % ymm9

# qhasm: r = a3 & mem256[input_2 + 128]
# asm 1: vpand 128(<input_2=int64#3),<a3=reg256#15,>r=reg256#16
# asm 2: vpand 128(<input_2=%rdx),<a3=%ymm14,>r=%ymm15
vpand 128( % rdx), % ymm14, % ymm15

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#16,<r7=reg256#11,<r7=reg256#11
# asm 2: vpxor <r=%ymm15,<r7=%ymm10,<r7=%ymm10
vpxor % ymm15, % ymm10, % ymm10

# qhasm: r = a3 & mem256[input_2 + 160]
# asm 1: vpand 160(<input_2=int64#3),<a3=reg256#15,>r=reg256#16
# asm 2: vpand 160(<input_2=%rdx),<a3=%ymm14,>r=%ymm15
vpand 160( % rdx), % ymm14, % ymm15

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#16,<r8=reg256#12,<r8=reg256#12
# asm 2: vpxor <r=%ymm15,<r8=%ymm11,<r8=%ymm11
vpxor % ymm15, % ymm11, % ymm11

# qhasm: r = a3 & mem256[input_2 + 192]
# asm 1: vpand 192(<input_2=int64#3),<a3=reg256#15,>r=reg256#16
# asm 2: vpand 192(<input_2=%rdx),<a3=%ymm14,>r=%ymm15
vpand 192( % rdx), % ymm14, % ymm15

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#16,<r9=reg256#13,<r9=reg256#13
# asm 2: vpxor <r=%ymm15,<r9=%ymm12,<r9=%ymm12
vpxor % ymm15, % ymm12, % ymm12

# qhasm: r = a3 & mem256[input_2 + 224]
# asm 1: vpand 224(<input_2=int64#3),<a3=reg256#15,>r=reg256#16
# asm 2: vpand 224(<input_2=%rdx),<a3=%ymm14,>r=%ymm15
vpand 224( % rdx), % ymm14, % ymm15

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#16,<r10=reg256#14,<r10=reg256#14
# asm 2: vpxor <r=%ymm15,<r10=%ymm13,<r10=%ymm13
vpxor % ymm15, % ymm13, % ymm13

# qhasm: r = a3 & mem256[input_2 + 256]
# asm 1: vpand 256(<input_2=int64#3),<a3=reg256#15,>r=reg256#16
# asm 2: vpand 256(<input_2=%rdx),<a3=%ymm14,>r=%ymm15
vpand 256( % rdx), % ymm14, % ymm15

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#16,<r11=reg256#2,<r11=reg256#2
# asm 2: vpxor <r=%ymm15,<r11=%ymm1,<r11=%ymm1
vpxor % ymm15, % ymm1, % ymm1

# qhasm: r = a3 & mem256[input_2 + 288]
# asm 1: vpand 288(<input_2=int64#3),<a3=reg256#15,>r=reg256#16
# asm 2: vpand 288(<input_2=%rdx),<a3=%ymm14,>r=%ymm15
vpand 288( % rdx), % ymm14, % ymm15

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#16,<r12=reg256#3,<r12=reg256#3
# asm 2: vpxor <r=%ymm15,<r12=%ymm2,<r12=%ymm2
vpxor % ymm15, % ymm2, % ymm2

# qhasm: r = a3 & mem256[input_2 + 320]
# asm 1: vpand 320(<input_2=int64#3),<a3=reg256#15,>r=reg256#16
# asm 2: vpand 320(<input_2=%rdx),<a3=%ymm14,>r=%ymm15
vpand 320( % rdx), % ymm14, % ymm15

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#16,<r13=reg256#4,<r13=reg256#4
# asm 2: vpxor <r=%ymm15,<r13=%ymm3,<r13=%ymm3
vpxor % ymm15, % ymm3, % ymm3

# qhasm: r = a3 & mem256[input_2 + 352]
# asm 1: vpand 352(<input_2=int64#3),<a3=reg256#15,>r=reg256#16
# asm 2: vpand 352(<input_2=%rdx),<a3=%ymm14,>r=%ymm15
vpand 352( % rdx), % ymm14, % ymm15

# qhasm: r14 ^= r
# asm 1: vpxor <r=reg256#16,<r14=reg256#5,<r14=reg256#5
# asm 2: vpxor <r=%ymm15,<r14=%ymm4,<r14=%ymm4
vpxor % ymm15, % ymm4, % ymm4

# qhasm: r = a3 & mem256[input_2 + 384]
# asm 1: vpand 384(<input_2=int64#3),<a3=reg256#15,>r=reg256#15
# asm 2: vpand 384(<input_2=%rdx),<a3=%ymm14,>r=%ymm14
vpand 384( % rdx), % ymm14, % ymm14

# qhasm: r15 ^= r
# asm 1: vpxor <r=reg256#15,<r15=reg256#6,<r15=reg256#6
# asm 2: vpxor <r=%ymm14,<r15=%ymm5,<r15=%ymm5
vpxor % ymm14, % ymm5, % ymm5

# qhasm: r6 ^= r15
# asm 1: vpxor <r15=reg256#6,<r6=reg256#10,<r6=reg256#10
# asm 2: vpxor <r15=%ymm5,<r6=%ymm9,<r6=%ymm9
vpxor % ymm5, % ymm9, % ymm9

# qhasm: r5 ^= r15
# asm 1: vpxor <r15=reg256#6,<r5=reg256#9,<r5=reg256#9
# asm 2: vpxor <r15=%ymm5,<r5=%ymm8,<r5=%ymm8
vpxor % ymm5, % ymm8, % ymm8

# qhasm: r3 ^= r15
# asm 1: vpxor <r15=reg256#6,<r3=reg256#7,<r3=reg256#7
# asm 2: vpxor <r15=%ymm5,<r3=%ymm6,<r3=%ymm6
vpxor % ymm5, % ymm6, % ymm6

# qhasm: r2 = r15
# asm 1: vmovapd <r15=reg256#6,>r2=reg256#6
# asm 2: vmovapd <r15=%ymm5,>r2=%ymm5
vmovapd % ymm5, % ymm5

# qhasm: a2 = mem256[ input_1 + 64 ]
# asm 1: vmovupd   64(<input_1=int64#2),>a2=reg256#15
# asm 2: vmovupd   64(<input_1=%rsi),>a2=%ymm14
vmovupd   64( % rsi), % ymm14

# qhasm: r = a2 & b0
# asm 1: vpand <a2=reg256#15,<b0=reg256#1,>r=reg256#16
# asm 2: vpand <a2=%ymm14,<b0=%ymm0,>r=%ymm15
vpand % ymm14, % ymm0, % ymm15

# qhasm: r2 ^= r
# asm 1: vpxor <r=reg256#16,<r2=reg256#6,<r2=reg256#6
# asm 2: vpxor <r=%ymm15,<r2=%ymm5,<r2=%ymm5
vpxor % ymm15, % ymm5, % ymm5

# qhasm: r = a2 & mem256[input_2 + 32]
# asm 1: vpand 32(<input_2=int64#3),<a2=reg256#15,>r=reg256#16
# asm 2: vpand 32(<input_2=%rdx),<a2=%ymm14,>r=%ymm15
vpand 32( % rdx), % ymm14, % ymm15

# qhasm: r3 ^= r
# asm 1: vpxor <r=reg256#16,<r3=reg256#7,<r3=reg256#7
# asm 2: vpxor <r=%ymm15,<r3=%ymm6,<r3=%ymm6
vpxor % ymm15, % ymm6, % ymm6

# qhasm: r = a2 & mem256[input_2 + 64]
# asm 1: vpand 64(<input_2=int64#3),<a2=reg256#15,>r=reg256#16
# asm 2: vpand 64(<input_2=%rdx),<a2=%ymm14,>r=%ymm15
vpand 64( % rdx), % ymm14, % ymm15

# qhasm: r4 ^= r
# asm 1: vpxor <r=reg256#16,<r4=reg256#8,<r4=reg256#8
# asm 2: vpxor <r=%ymm15,<r4=%ymm7,<r4=%ymm7
vpxor % ymm15, % ymm7, % ymm7

# qhasm: r = a2 & mem256[input_2 + 96]
# asm 1: vpand 96(<input_2=int64#3),<a2=reg256#15,>r=reg256#16
# asm 2: vpand 96(<input_2=%rdx),<a2=%ymm14,>r=%ymm15
vpand 96( % rdx), % ymm14, % ymm15

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#16,<r5=reg256#9,<r5=reg256#9
# asm 2: vpxor <r=%ymm15,<r5=%ymm8,<r5=%ymm8
vpxor % ymm15, % ymm8, % ymm8

# qhasm: r = a2 & mem256[input_2 + 128]
# asm 1: vpand 128(<input_2=int64#3),<a2=reg256#15,>r=reg256#16
# asm 2: vpand 128(<input_2=%rdx),<a2=%ymm14,>r=%ymm15
vpand 128( % rdx), % ymm14, % ymm15

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#16,<r6=reg256#10,<r6=reg256#10
# asm 2: vpxor <r=%ymm15,<r6=%ymm9,<r6=%ymm9
vpxor % ymm15, % ymm9, % ymm9

# qhasm: r = a2 & mem256[input_2 + 160]
# asm 1: vpand 160(<input_2=int64#3),<a2=reg256#15,>r=reg256#16
# asm 2: vpand 160(<input_2=%rdx),<a2=%ymm14,>r=%ymm15
vpand 160( % rdx), % ymm14, % ymm15

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#16,<r7=reg256#11,<r7=reg256#11
# asm 2: vpxor <r=%ymm15,<r7=%ymm10,<r7=%ymm10
vpxor % ymm15, % ymm10, % ymm10

# qhasm: r = a2 & mem256[input_2 + 192]
# asm 1: vpand 192(<input_2=int64#3),<a2=reg256#15,>r=reg256#16
# asm 2: vpand 192(<input_2=%rdx),<a2=%ymm14,>r=%ymm15
vpand 192( % rdx), % ymm14, % ymm15

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#16,<r8=reg256#12,<r8=reg256#12
# asm 2: vpxor <r=%ymm15,<r8=%ymm11,<r8=%ymm11
vpxor % ymm15, % ymm11, % ymm11

# qhasm: r = a2 & mem256[input_2 + 224]
# asm 1: vpand 224(<input_2=int64#3),<a2=reg256#15,>r=reg256#16
# asm 2: vpand 224(<input_2=%rdx),<a2=%ymm14,>r=%ymm15
vpand 224( % rdx), % ymm14, % ymm15

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#16,<r9=reg256#13,<r9=reg256#13
# asm 2: vpxor <r=%ymm15,<r9=%ymm12,<r9=%ymm12
vpxor % ymm15, % ymm12, % ymm12

# qhasm: r = a2 & mem256[input_2 + 256]
# asm 1: vpand 256(<input_2=int64#3),<a2=reg256#15,>r=reg256#16
# asm 2: vpand 256(<input_2=%rdx),<a2=%ymm14,>r=%ymm15
vpand 256( % rdx), % ymm14, % ymm15

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#16,<r10=reg256#14,<r10=reg256#14
# asm 2: vpxor <r=%ymm15,<r10=%ymm13,<r10=%ymm13
vpxor % ymm15, % ymm13, % ymm13

# qhasm: r = a2 & mem256[input_2 + 288]
# asm 1: vpand 288(<input_2=int64#3),<a2=reg256#15,>r=reg256#16
# asm 2: vpand 288(<input_2=%rdx),<a2=%ymm14,>r=%ymm15
vpand 288( % rdx), % ymm14, % ymm15

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#16,<r11=reg256#2,<r11=reg256#2
# asm 2: vpxor <r=%ymm15,<r11=%ymm1,<r11=%ymm1
vpxor % ymm15, % ymm1, % ymm1

# qhasm: r = a2 & mem256[input_2 + 320]
# asm 1: vpand 320(<input_2=int64#3),<a2=reg256#15,>r=reg256#16
# asm 2: vpand 320(<input_2=%rdx),<a2=%ymm14,>r=%ymm15
vpand 320( % rdx), % ymm14, % ymm15

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#16,<r12=reg256#3,<r12=reg256#3
# asm 2: vpxor <r=%ymm15,<r12=%ymm2,<r12=%ymm2
vpxor % ymm15, % ymm2, % ymm2

# qhasm: r = a2 & mem256[input_2 + 352]
# asm 1: vpand 352(<input_2=int64#3),<a2=reg256#15,>r=reg256#16
# asm 2: vpand 352(<input_2=%rdx),<a2=%ymm14,>r=%ymm15
vpand 352( % rdx), % ymm14, % ymm15

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#16,<r13=reg256#4,<r13=reg256#4
# asm 2: vpxor <r=%ymm15,<r13=%ymm3,<r13=%ymm3
vpxor % ymm15, % ymm3, % ymm3

# qhasm: r = a2 & mem256[input_2 + 384]
# asm 1: vpand 384(<input_2=int64#3),<a2=reg256#15,>r=reg256#15
# asm 2: vpand 384(<input_2=%rdx),<a2=%ymm14,>r=%ymm14
vpand 384( % rdx), % ymm14, % ymm14

# qhasm: r14 ^= r
# asm 1: vpxor <r=reg256#15,<r14=reg256#5,<r14=reg256#5
# asm 2: vpxor <r=%ymm14,<r14=%ymm4,<r14=%ymm4
vpxor % ymm14, % ymm4, % ymm4

# qhasm: r5 ^= r14
# asm 1: vpxor <r14=reg256#5,<r5=reg256#9,<r5=reg256#9
# asm 2: vpxor <r14=%ymm4,<r5=%ymm8,<r5=%ymm8
vpxor % ymm4, % ymm8, % ymm8

# qhasm: r4 ^= r14
# asm 1: vpxor <r14=reg256#5,<r4=reg256#8,<r4=reg256#8
# asm 2: vpxor <r14=%ymm4,<r4=%ymm7,<r4=%ymm7
vpxor % ymm4, % ymm7, % ymm7

# qhasm: r2 ^= r14
# asm 1: vpxor <r14=reg256#5,<r2=reg256#6,<r2=reg256#6
# asm 2: vpxor <r14=%ymm4,<r2=%ymm5,<r2=%ymm5
vpxor % ymm4, % ymm5, % ymm5

# qhasm: r1 = r14
# asm 1: vmovapd <r14=reg256#5,>r1=reg256#5
# asm 2: vmovapd <r14=%ymm4,>r1=%ymm4
vmovapd % ymm4, % ymm4

# qhasm: a1 = mem256[ input_1 + 32 ]
# asm 1: vmovupd   32(<input_1=int64#2),>a1=reg256#15
# asm 2: vmovupd   32(<input_1=%rsi),>a1=%ymm14
vmovupd   32( % rsi), % ymm14

# qhasm: r = a1 & b0
# asm 1: vpand <a1=reg256#15,<b0=reg256#1,>r=reg256#16
# asm 2: vpand <a1=%ymm14,<b0=%ymm0,>r=%ymm15
vpand % ymm14, % ymm0, % ymm15

# qhasm: r1 ^= r
# asm 1: vpxor <r=reg256#16,<r1=reg256#5,<r1=reg256#5
# asm 2: vpxor <r=%ymm15,<r1=%ymm4,<r1=%ymm4
vpxor % ymm15, % ymm4, % ymm4

# qhasm: r = a1 & mem256[input_2 + 32]
# asm 1: vpand 32(<input_2=int64#3),<a1=reg256#15,>r=reg256#16
# asm 2: vpand 32(<input_2=%rdx),<a1=%ymm14,>r=%ymm15
vpand 32( % rdx), % ymm14, % ymm15

# qhasm: r2 ^= r
# asm 1: vpxor <r=reg256#16,<r2=reg256#6,<r2=reg256#6
# asm 2: vpxor <r=%ymm15,<r2=%ymm5,<r2=%ymm5
vpxor % ymm15, % ymm5, % ymm5

# qhasm: r = a1 & mem256[input_2 + 64]
# asm 1: vpand 64(<input_2=int64#3),<a1=reg256#15,>r=reg256#16
# asm 2: vpand 64(<input_2=%rdx),<a1=%ymm14,>r=%ymm15
vpand 64( % rdx), % ymm14, % ymm15

# qhasm: r3 ^= r
# asm 1: vpxor <r=reg256#16,<r3=reg256#7,<r3=reg256#7
# asm 2: vpxor <r=%ymm15,<r3=%ymm6,<r3=%ymm6
vpxor % ymm15, % ymm6, % ymm6

# qhasm: r = a1 & mem256[input_2 + 96]
# asm 1: vpand 96(<input_2=int64#3),<a1=reg256#15,>r=reg256#16
# asm 2: vpand 96(<input_2=%rdx),<a1=%ymm14,>r=%ymm15
vpand 96( % rdx), % ymm14, % ymm15

# qhasm: r4 ^= r
# asm 1: vpxor <r=reg256#16,<r4=reg256#8,<r4=reg256#8
# asm 2: vpxor <r=%ymm15,<r4=%ymm7,<r4=%ymm7
vpxor % ymm15, % ymm7, % ymm7

# qhasm: r = a1 & mem256[input_2 + 128]
# asm 1: vpand 128(<input_2=int64#3),<a1=reg256#15,>r=reg256#16
# asm 2: vpand 128(<input_2=%rdx),<a1=%ymm14,>r=%ymm15
vpand 128( % rdx), % ymm14, % ymm15

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#16,<r5=reg256#9,<r5=reg256#9
# asm 2: vpxor <r=%ymm15,<r5=%ymm8,<r5=%ymm8
vpxor % ymm15, % ymm8, % ymm8

# qhasm: r = a1 & mem256[input_2 + 160]
# asm 1: vpand 160(<input_2=int64#3),<a1=reg256#15,>r=reg256#16
# asm 2: vpand 160(<input_2=%rdx),<a1=%ymm14,>r=%ymm15
vpand 160( % rdx), % ymm14, % ymm15

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#16,<r6=reg256#10,<r6=reg256#10
# asm 2: vpxor <r=%ymm15,<r6=%ymm9,<r6=%ymm9
vpxor % ymm15, % ymm9, % ymm9

# qhasm: r = a1 & mem256[input_2 + 192]
# asm 1: vpand 192(<input_2=int64#3),<a1=reg256#15,>r=reg256#16
# asm 2: vpand 192(<input_2=%rdx),<a1=%ymm14,>r=%ymm15
vpand 192( % rdx), % ymm14, % ymm15

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#16,<r7=reg256#11,<r7=reg256#11
# asm 2: vpxor <r=%ymm15,<r7=%ymm10,<r7=%ymm10
vpxor % ymm15, % ymm10, % ymm10

# qhasm: r = a1 & mem256[input_2 + 224]
# asm 1: vpand 224(<input_2=int64#3),<a1=reg256#15,>r=reg256#16
# asm 2: vpand 224(<input_2=%rdx),<a1=%ymm14,>r=%ymm15
vpand 224( % rdx), % ymm14, % ymm15

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#16,<r8=reg256#12,<r8=reg256#12
# asm 2: vpxor <r=%ymm15,<r8=%ymm11,<r8=%ymm11
vpxor % ymm15, % ymm11, % ymm11

# qhasm: r = a1 & mem256[input_2 + 256]
# asm 1: vpand 256(<input_2=int64#3),<a1=reg256#15,>r=reg256#16
# asm 2: vpand 256(<input_2=%rdx),<a1=%ymm14,>r=%ymm15
vpand 256( % rdx), % ymm14, % ymm15

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#16,<r9=reg256#13,<r9=reg256#13
# asm 2: vpxor <r=%ymm15,<r9=%ymm12,<r9=%ymm12
vpxor % ymm15, % ymm12, % ymm12

# qhasm: r = a1 & mem256[input_2 + 288]
# asm 1: vpand 288(<input_2=int64#3),<a1=reg256#15,>r=reg256#16
# asm 2: vpand 288(<input_2=%rdx),<a1=%ymm14,>r=%ymm15
vpand 288( % rdx), % ymm14, % ymm15

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#16,<r10=reg256#14,<r10=reg256#14
# asm 2: vpxor <r=%ymm15,<r10=%ymm13,<r10=%ymm13
vpxor % ymm15, % ymm13, % ymm13

# qhasm: r = a1 & mem256[input_2 + 320]
# asm 1: vpand 320(<input_2=int64#3),<a1=reg256#15,>r=reg256#16
# asm 2: vpand 320(<input_2=%rdx),<a1=%ymm14,>r=%ymm15
vpand 320( % rdx), % ymm14, % ymm15

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#16,<r11=reg256#2,<r11=reg256#2
# asm 2: vpxor <r=%ymm15,<r11=%ymm1,<r11=%ymm1
vpxor % ymm15, % ymm1, % ymm1

# qhasm: r = a1 & mem256[input_2 + 352]
# asm 1: vpand 352(<input_2=int64#3),<a1=reg256#15,>r=reg256#16
# asm 2: vpand 352(<input_2=%rdx),<a1=%ymm14,>r=%ymm15
vpand 352( % rdx), % ymm14, % ymm15

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#16,<r12=reg256#3,<r12=reg256#3
# asm 2: vpxor <r=%ymm15,<r12=%ymm2,<r12=%ymm2
vpxor % ymm15, % ymm2, % ymm2

# qhasm: r = a1 & mem256[input_2 + 384]
# asm 1: vpand 384(<input_2=int64#3),<a1=reg256#15,>r=reg256#15
# asm 2: vpand 384(<input_2=%rdx),<a1=%ymm14,>r=%ymm14
vpand 384( % rdx), % ymm14, % ymm14

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#15,<r13=reg256#4,<r13=reg256#4
# asm 2: vpxor <r=%ymm14,<r13=%ymm3,<r13=%ymm3
vpxor % ymm14, % ymm3, % ymm3

# qhasm: r4 ^= r13
# asm 1: vpxor <r13=reg256#4,<r4=reg256#8,<r4=reg256#8
# asm 2: vpxor <r13=%ymm3,<r4=%ymm7,<r4=%ymm7
vpxor % ymm3, % ymm7, % ymm7

# qhasm: r3 ^= r13
# asm 1: vpxor <r13=reg256#4,<r3=reg256#7,<r3=reg256#7
# asm 2: vpxor <r13=%ymm3,<r3=%ymm6,<r3=%ymm6
vpxor % ymm3, % ymm6, % ymm6

# qhasm: r1 ^= r13
# asm 1: vpxor <r13=reg256#4,<r1=reg256#5,<r1=reg256#5
# asm 2: vpxor <r13=%ymm3,<r1=%ymm4,<r1=%ymm4
vpxor % ymm3, % ymm4, % ymm4

# qhasm: r0 = r13
# asm 1: vmovapd <r13=reg256#4,>r0=reg256#4
# asm 2: vmovapd <r13=%ymm3,>r0=%ymm3
vmovapd % ymm3, % ymm3

# qhasm: a0 = mem256[ input_1 + 0 ]
# asm 1: vmovupd   0(<input_1=int64#2),>a0=reg256#15
# asm 2: vmovupd   0(<input_1=%rsi),>a0=%ymm14
vmovupd   0( % rsi), % ymm14

# qhasm: r = a0 & b0
# asm 1: vpand <a0=reg256#15,<b0=reg256#1,>r=reg256#1
# asm 2: vpand <a0=%ymm14,<b0=%ymm0,>r=%ymm0
vpand % ymm14, % ymm0, % ymm0

# qhasm: r0 ^= r
# asm 1: vpxor <r=reg256#1,<r0=reg256#4,<r0=reg256#4
# asm 2: vpxor <r=%ymm0,<r0=%ymm3,<r0=%ymm3
vpxor % ymm0, % ymm3, % ymm3

# qhasm: r = a0 & mem256[input_2 + 32]
# asm 1: vpand 32(<input_2=int64#3),<a0=reg256#15,>r=reg256#1
# asm 2: vpand 32(<input_2=%rdx),<a0=%ymm14,>r=%ymm0
vpand 32( % rdx), % ymm14, % ymm0

# qhasm: r1 ^= r
# asm 1: vpxor <r=reg256#1,<r1=reg256#5,<r1=reg256#5
# asm 2: vpxor <r=%ymm0,<r1=%ymm4,<r1=%ymm4
vpxor % ymm0, % ymm4, % ymm4

# qhasm: r = a0 & mem256[input_2 + 64]
# asm 1: vpand 64(<input_2=int64#3),<a0=reg256#15,>r=reg256#1
# asm 2: vpand 64(<input_2=%rdx),<a0=%ymm14,>r=%ymm0
vpand 64( % rdx), % ymm14, % ymm0

# qhasm: r2 ^= r
# asm 1: vpxor <r=reg256#1,<r2=reg256#6,<r2=reg256#6
# asm 2: vpxor <r=%ymm0,<r2=%ymm5,<r2=%ymm5
vpxor % ymm0, % ymm5, % ymm5

# qhasm: r = a0 & mem256[input_2 + 96]
# asm 1: vpand 96(<input_2=int64#3),<a0=reg256#15,>r=reg256#1
# asm 2: vpand 96(<input_2=%rdx),<a0=%ymm14,>r=%ymm0
vpand 96( % rdx), % ymm14, % ymm0

# qhasm: r3 ^= r
# asm 1: vpxor <r=reg256#1,<r3=reg256#7,<r3=reg256#7
# asm 2: vpxor <r=%ymm0,<r3=%ymm6,<r3=%ymm6
vpxor % ymm0, % ymm6, % ymm6

# qhasm: r = a0 & mem256[input_2 + 128]
# asm 1: vpand 128(<input_2=int64#3),<a0=reg256#15,>r=reg256#1
# asm 2: vpand 128(<input_2=%rdx),<a0=%ymm14,>r=%ymm0
vpand 128( % rdx), % ymm14, % ymm0

# qhasm: r4 ^= r
# asm 1: vpxor <r=reg256#1,<r4=reg256#8,<r4=reg256#8
# asm 2: vpxor <r=%ymm0,<r4=%ymm7,<r4=%ymm7
vpxor % ymm0, % ymm7, % ymm7

# qhasm: r = a0 & mem256[input_2 + 160]
# asm 1: vpand 160(<input_2=int64#3),<a0=reg256#15,>r=reg256#1
# asm 2: vpand 160(<input_2=%rdx),<a0=%ymm14,>r=%ymm0
vpand 160( % rdx), % ymm14, % ymm0

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#1,<r5=reg256#9,<r5=reg256#9
# asm 2: vpxor <r=%ymm0,<r5=%ymm8,<r5=%ymm8
vpxor % ymm0, % ymm8, % ymm8

# qhasm: r = a0 & mem256[input_2 + 192]
# asm 1: vpand 192(<input_2=int64#3),<a0=reg256#15,>r=reg256#1
# asm 2: vpand 192(<input_2=%rdx),<a0=%ymm14,>r=%ymm0
vpand 192( % rdx), % ymm14, % ymm0

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#1,<r6=reg256#10,<r6=reg256#10
# asm 2: vpxor <r=%ymm0,<r6=%ymm9,<r6=%ymm9
vpxor % ymm0, % ymm9, % ymm9

# qhasm: r = a0 & mem256[input_2 + 224]
# asm 1: vpand 224(<input_2=int64#3),<a0=reg256#15,>r=reg256#1
# asm 2: vpand 224(<input_2=%rdx),<a0=%ymm14,>r=%ymm0
vpand 224( % rdx), % ymm14, % ymm0

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#1,<r7=reg256#11,<r7=reg256#11
# asm 2: vpxor <r=%ymm0,<r7=%ymm10,<r7=%ymm10
vpxor % ymm0, % ymm10, % ymm10

# qhasm: r = a0 & mem256[input_2 + 256]
# asm 1: vpand 256(<input_2=int64#3),<a0=reg256#15,>r=reg256#1
# asm 2: vpand 256(<input_2=%rdx),<a0=%ymm14,>r=%ymm0
vpand 256( % rdx), % ymm14, % ymm0

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#1,<r8=reg256#12,<r8=reg256#12
# asm 2: vpxor <r=%ymm0,<r8=%ymm11,<r8=%ymm11
vpxor % ymm0, % ymm11, % ymm11

# qhasm: r = a0 & mem256[input_2 + 288]
# asm 1: vpand 288(<input_2=int64#3),<a0=reg256#15,>r=reg256#1
# asm 2: vpand 288(<input_2=%rdx),<a0=%ymm14,>r=%ymm0
vpand 288( % rdx), % ymm14, % ymm0

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#1,<r9=reg256#13,<r9=reg256#13
# asm 2: vpxor <r=%ymm0,<r9=%ymm12,<r9=%ymm12
vpxor % ymm0, % ymm12, % ymm12

# qhasm: r = a0 & mem256[input_2 + 320]
# asm 1: vpand 320(<input_2=int64#3),<a0=reg256#15,>r=reg256#1
# asm 2: vpand 320(<input_2=%rdx),<a0=%ymm14,>r=%ymm0
vpand 320( % rdx), % ymm14, % ymm0

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#1,<r10=reg256#14,<r10=reg256#14
# asm 2: vpxor <r=%ymm0,<r10=%ymm13,<r10=%ymm13
vpxor % ymm0, % ymm13, % ymm13

# qhasm: r = a0 & mem256[input_2 + 352]
# asm 1: vpand 352(<input_2=int64#3),<a0=reg256#15,>r=reg256#1
# asm 2: vpand 352(<input_2=%rdx),<a0=%ymm14,>r=%ymm0
vpand 352( % rdx), % ymm14, % ymm0

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#1,<r11=reg256#2,<r11=reg256#2
# asm 2: vpxor <r=%ymm0,<r11=%ymm1,<r11=%ymm1
vpxor % ymm0, % ymm1, % ymm1

# qhasm: r = a0 & mem256[input_2 + 384]
# asm 1: vpand 384(<input_2=int64#3),<a0=reg256#15,>r=reg256#1
# asm 2: vpand 384(<input_2=%rdx),<a0=%ymm14,>r=%ymm0
vpand 384( % rdx), % ymm14, % ymm0

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#1,<r12=reg256#3,<r12=reg256#3
# asm 2: vpxor <r=%ymm0,<r12=%ymm2,<r12=%ymm2
vpxor % ymm0, % ymm2, % ymm2

# qhasm: mem256[ input_0 + 384 ] = r12
# asm 1: vmovupd   <r12=reg256#3,384(<input_0=int64#1)
# asm 2: vmovupd   <r12=%ymm2,384(<input_0=%rdi)
vmovupd   % ymm2, 384( % rdi)

# qhasm: mem256[ input_0 + 352 ] = r11
# asm 1: vmovupd   <r11=reg256#2,352(<input_0=int64#1)
# asm 2: vmovupd   <r11=%ymm1,352(<input_0=%rdi)
vmovupd   % ymm1, 352( % rdi)

# qhasm: mem256[ input_0 + 320 ] = r10
# asm 1: vmovupd   <r10=reg256#14,320(<input_0=int64#1)
# asm 2: vmovupd   <r10=%ymm13,320(<input_0=%rdi)
vmovupd   % ymm13, 320( % rdi)

# qhasm: mem256[ input_0 + 288 ] = r9
# asm 1: vmovupd   <r9=reg256#13,288(<input_0=int64#1)
# asm 2: vmovupd   <r9=%ymm12,288(<input_0=%rdi)
vmovupd   % ymm12, 288( % rdi)

# qhasm: mem256[ input_0 + 256 ] = r8
# asm 1: vmovupd   <r8=reg256#12,256(<input_0=int64#1)
# asm 2: vmovupd   <r8=%ymm11,256(<input_0=%rdi)
vmovupd   % ymm11, 256( % rdi)

# qhasm: mem256[ input_0 + 224 ] = r7
# asm 1: vmovupd   <r7=reg256#11,224(<input_0=int64#1)
# asm 2: vmovupd   <r7=%ymm10,224(<input_0=%rdi)
vmovupd   % ymm10, 224( % rdi)

# qhasm: mem256[ input_0 + 192 ] = r6
# asm 1: vmovupd   <r6=reg256#10,192(<input_0=int64#1)
# asm 2: vmovupd   <r6=%ymm9,192(<input_0=%rdi)
vmovupd   % ymm9, 192( % rdi)

# qhasm: mem256[ input_0 + 160 ] = r5
# asm 1: vmovupd   <r5=reg256#9,160(<input_0=int64#1)
# asm 2: vmovupd   <r5=%ymm8,160(<input_0=%rdi)
vmovupd   % ymm8, 160( % rdi)

# qhasm: mem256[ input_0 + 128 ] = r4
# asm 1: vmovupd   <r4=reg256#8,128(<input_0=int64#1)
# asm 2: vmovupd   <r4=%ymm7,128(<input_0=%rdi)
vmovupd   % ymm7, 128( % rdi)

# qhasm: mem256[ input_0 + 96 ] = r3
# asm 1: vmovupd   <r3=reg256#7,96(<input_0=int64#1)
# asm 2: vmovupd   <r3=%ymm6,96(<input_0=%rdi)
vmovupd   % ymm6, 96( % rdi)

# qhasm: mem256[ input_0 + 64 ] = r2
# asm 1: vmovupd   <r2=reg256#6,64(<input_0=int64#1)
# asm 2: vmovupd   <r2=%ymm5,64(<input_0=%rdi)
vmovupd   % ymm5, 64( % rdi)

# qhasm: mem256[ input_0 + 32 ] = r1
# asm 1: vmovupd   <r1=reg256#5,32(<input_0=int64#1)
# asm 2: vmovupd   <r1=%ymm4,32(<input_0=%rdi)
vmovupd   % ymm4, 32( % rdi)

# qhasm: mem256[ input_0 + 0 ] = r0
# asm 1: vmovupd   <r0=reg256#4,0(<input_0=int64#1)
# asm 2: vmovupd   <r0=%ymm3,0(<input_0=%rdi)
vmovupd   % ymm3, 0( % rdi)

# qhasm: return
add % r11, % rsp
ret
