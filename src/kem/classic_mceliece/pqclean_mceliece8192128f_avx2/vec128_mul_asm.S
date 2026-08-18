#include "namespace.h"

#define vec128_mul_asm CRYPTO_NAMESPACE(vec128_mul_asm)
#define _vec128_mul_asm _CRYPTO_NAMESPACE(vec128_mul_asm)

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

# qhasm: reg256 b0

# qhasm: reg256 b1

# qhasm: reg256 b2

# qhasm: reg256 b3

# qhasm: reg256 b4

# qhasm: reg256 b5

# qhasm: reg256 b6

# qhasm: reg256 b7

# qhasm: reg256 b8

# qhasm: reg256 b9

# qhasm: reg256 b10

# qhasm: reg256 b11

# qhasm: reg256 b12

# qhasm: reg256 a0

# qhasm: reg256 a1

# qhasm: reg256 a2

# qhasm: reg256 a3

# qhasm: reg256 a4

# qhasm: reg256 a5

# qhasm: reg256 a6

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

# qhasm: reg128 h0

# qhasm: reg128 h1

# qhasm: reg128 h2

# qhasm: reg128 h3

# qhasm: reg128 h4

# qhasm: reg128 h5

# qhasm: reg128 h6

# qhasm: reg128 h7

# qhasm: reg128 h8

# qhasm: reg128 h9

# qhasm: reg128 h10

# qhasm: reg128 h11

# qhasm: reg128 h12

# qhasm: reg128 h13

# qhasm: reg128 h14

# qhasm: reg128 h15

# qhasm: reg128 h16

# qhasm: reg128 h17

# qhasm: reg128 h18

# qhasm: reg128 h19

# qhasm: reg128 h20

# qhasm: reg128 h21

# qhasm: reg128 h22

# qhasm: reg128 h23

# qhasm: reg128 h24

# qhasm: stack4864 buf

# qhasm: int64 ptr

# qhasm: int64 tmp

# qhasm: enter vec128_mul_asm
.p2align 5
.global _vec128_mul_asm
.global vec128_mul_asm
_vec128_mul_asm:
vec128_mul_asm:
mov % rsp, % r11
and $31, % r11
add $608, % r11
sub % r11, % rsp

# qhasm: ptr = &buf
# asm 1: leaq <buf=stack4864#1,>ptr=int64#5
# asm 2: leaq <buf=0(%rsp),>ptr=%r8
leaq 0( % rsp), % r8

# qhasm: tmp = input_3
# asm 1: mov  <input_3=int64#4,>tmp=int64#6
# asm 2: mov  <input_3=%rcx,>tmp=%r9
mov  % rcx, % r9

# qhasm: tmp *= 12
# asm 1: imulq  $12,<tmp=int64#6,>tmp=int64#6
# asm 2: imulq  $12,<tmp=%r9,>tmp=%r9
imulq  $12, % r9, % r9

# qhasm: input_2 += tmp
# asm 1: add  <tmp=int64#6,<input_2=int64#3
# asm 2: add  <tmp=%r9,<input_2=%rdx
add  % r9, % rdx

# qhasm: b12 = mem128[ input_2 + 0 ] x2
# asm 1: vbroadcasti128 0(<input_2=int64#3), >b12=reg256#1
# asm 2: vbroadcasti128 0(<input_2=%rdx), >b12=%ymm0
vbroadcasti128 0( % rdx), % ymm0

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  % rcx, % rdx

# qhasm: a6 = a6 ^ a6
# asm 1: vpxor <a6=reg256#2,<a6=reg256#2,>a6=reg256#2
# asm 2: vpxor <a6=%ymm1,<a6=%ymm1,>a6=%ymm1
vpxor % ymm1, % ymm1, % ymm1

# qhasm: a6[0] = mem128[ input_1 + 96 ]
# asm 1: vinsertf128 $0x0,96(<input_1=int64#2),<a6=reg256#2,<a6=reg256#2
# asm 2: vinsertf128 $0x0,96(<input_1=%rsi),<a6=%ymm1,<a6=%ymm1
vinsertf128 $0x0, 96( % rsi), % ymm1, % ymm1

# qhasm: r18 = b12 & a6
# asm 1: vpand <b12=reg256#1,<a6=reg256#2,>r18=reg256#3
# asm 2: vpand <b12=%ymm0,<a6=%ymm1,>r18=%ymm2
vpand % ymm0, % ymm1, % ymm2

# qhasm: mem256[ ptr + 576 ] = r18
# asm 1: vmovupd   <r18=reg256#3,576(<ptr=int64#5)
# asm 2: vmovupd   <r18=%ymm2,576(<ptr=%r8)
vmovupd   % ymm2, 576( % r8)

# qhasm: a5[0] = mem128[ input_1 + 80 ]
# asm 1: vinsertf128 $0x0,80(<input_1=int64#2),<a5=reg256#3,<a5=reg256#3
# asm 2: vinsertf128 $0x0,80(<input_1=%rsi),<a5=%ymm2,<a5=%ymm2
vinsertf128 $0x0, 80( % rsi), % ymm2, % ymm2

# qhasm: a5[1] = mem128[ input_1 + 192 ]
# asm 1: vinsertf128 $0x1,192(<input_1=int64#2),<a5=reg256#3,<a5=reg256#3
# asm 2: vinsertf128 $0x1,192(<input_1=%rsi),<a5=%ymm2,<a5=%ymm2
vinsertf128 $0x1, 192( % rsi), % ymm2, % ymm2

# qhasm: r17 = b12 & a5
# asm 1: vpand <b12=reg256#1,<a5=reg256#3,>r17=reg256#4
# asm 2: vpand <b12=%ymm0,<a5=%ymm2,>r17=%ymm3
vpand % ymm0, % ymm2, % ymm3

# qhasm: a4[0] = mem128[ input_1 + 64 ]
# asm 1: vinsertf128 $0x0,64(<input_1=int64#2),<a4=reg256#5,<a4=reg256#5
# asm 2: vinsertf128 $0x0,64(<input_1=%rsi),<a4=%ymm4,<a4=%ymm4
vinsertf128 $0x0, 64( % rsi), % ymm4, % ymm4

# qhasm: a4[1] = mem128[ input_1 + 176 ]
# asm 1: vinsertf128 $0x1,176(<input_1=int64#2),<a4=reg256#5,<a4=reg256#5
# asm 2: vinsertf128 $0x1,176(<input_1=%rsi),<a4=%ymm4,<a4=%ymm4
vinsertf128 $0x1, 176( % rsi), % ymm4, % ymm4

# qhasm: r16 = b12 & a4
# asm 1: vpand <b12=reg256#1,<a4=reg256#5,>r16=reg256#6
# asm 2: vpand <b12=%ymm0,<a4=%ymm4,>r16=%ymm5
vpand % ymm0, % ymm4, % ymm5

# qhasm: a3[0] = mem128[ input_1 + 48 ]
# asm 1: vinsertf128 $0x0,48(<input_1=int64#2),<a3=reg256#7,<a3=reg256#7
# asm 2: vinsertf128 $0x0,48(<input_1=%rsi),<a3=%ymm6,<a3=%ymm6
vinsertf128 $0x0, 48( % rsi), % ymm6, % ymm6

# qhasm: a3[1] = mem128[ input_1 + 160 ]
# asm 1: vinsertf128 $0x1,160(<input_1=int64#2),<a3=reg256#7,<a3=reg256#7
# asm 2: vinsertf128 $0x1,160(<input_1=%rsi),<a3=%ymm6,<a3=%ymm6
vinsertf128 $0x1, 160( % rsi), % ymm6, % ymm6

# qhasm: r15 = b12 & a3
# asm 1: vpand <b12=reg256#1,<a3=reg256#7,>r15=reg256#8
# asm 2: vpand <b12=%ymm0,<a3=%ymm6,>r15=%ymm7
vpand % ymm0, % ymm6, % ymm7

# qhasm: a2[0] = mem128[ input_1 + 32 ]
# asm 1: vinsertf128 $0x0,32(<input_1=int64#2),<a2=reg256#9,<a2=reg256#9
# asm 2: vinsertf128 $0x0,32(<input_1=%rsi),<a2=%ymm8,<a2=%ymm8
vinsertf128 $0x0, 32( % rsi), % ymm8, % ymm8

# qhasm: a2[1] = mem128[ input_1 + 144 ]
# asm 1: vinsertf128 $0x1,144(<input_1=int64#2),<a2=reg256#9,<a2=reg256#9
# asm 2: vinsertf128 $0x1,144(<input_1=%rsi),<a2=%ymm8,<a2=%ymm8
vinsertf128 $0x1, 144( % rsi), % ymm8, % ymm8

# qhasm: r14 = b12 & a2
# asm 1: vpand <b12=reg256#1,<a2=reg256#9,>r14=reg256#10
# asm 2: vpand <b12=%ymm0,<a2=%ymm8,>r14=%ymm9
vpand % ymm0, % ymm8, % ymm9

# qhasm: a1[0] = mem128[ input_1 + 16 ]
# asm 1: vinsertf128 $0x0,16(<input_1=int64#2),<a1=reg256#11,<a1=reg256#11
# asm 2: vinsertf128 $0x0,16(<input_1=%rsi),<a1=%ymm10,<a1=%ymm10
vinsertf128 $0x0, 16( % rsi), % ymm10, % ymm10

# qhasm: a1[1] = mem128[ input_1 + 128 ]
# asm 1: vinsertf128 $0x1,128(<input_1=int64#2),<a1=reg256#11,<a1=reg256#11
# asm 2: vinsertf128 $0x1,128(<input_1=%rsi),<a1=%ymm10,<a1=%ymm10
vinsertf128 $0x1, 128( % rsi), % ymm10, % ymm10

# qhasm: r13 = b12 & a1
# asm 1: vpand <b12=reg256#1,<a1=reg256#11,>r13=reg256#12
# asm 2: vpand <b12=%ymm0,<a1=%ymm10,>r13=%ymm11
vpand % ymm0, % ymm10, % ymm11

# qhasm: a0[0] = mem128[ input_1 + 0 ]
# asm 1: vinsertf128 $0x0,0(<input_1=int64#2),<a0=reg256#13,<a0=reg256#13
# asm 2: vinsertf128 $0x0,0(<input_1=%rsi),<a0=%ymm12,<a0=%ymm12
vinsertf128 $0x0, 0( % rsi), % ymm12, % ymm12

# qhasm: a0[1] = mem128[ input_1 + 112 ]
# asm 1: vinsertf128 $0x1,112(<input_1=int64#2),<a0=reg256#13,<a0=reg256#13
# asm 2: vinsertf128 $0x1,112(<input_1=%rsi),<a0=%ymm12,<a0=%ymm12
vinsertf128 $0x1, 112( % rsi), % ymm12, % ymm12

# qhasm: r12 = b12 & a0
# asm 1: vpand <b12=reg256#1,<a0=reg256#13,>r12=reg256#1
# asm 2: vpand <b12=%ymm0,<a0=%ymm12,>r12=%ymm0
vpand % ymm0, % ymm12, % ymm0

# qhasm: b11 = mem128[ input_2 + 0 ] x2
# asm 1: vbroadcasti128 0(<input_2=int64#3), >b11=reg256#14
# asm 2: vbroadcasti128 0(<input_2=%rdx), >b11=%ymm13
vbroadcasti128 0( % rdx), % ymm13

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  % rcx, % rdx

# qhasm: r = b11 & a6
# asm 1: vpand <b11=reg256#14,<a6=reg256#2,>r=reg256#15
# asm 2: vpand <b11=%ymm13,<a6=%ymm1,>r=%ymm14
vpand % ymm13, % ymm1, % ymm14

# qhasm: r17 ^= r
# asm 1: vpxor <r=reg256#15,<r17=reg256#4,<r17=reg256#4
# asm 2: vpxor <r=%ymm14,<r17=%ymm3,<r17=%ymm3
vpxor % ymm14, % ymm3, % ymm3

# qhasm: mem256[ ptr + 544 ] = r17
# asm 1: vmovupd   <r17=reg256#4,544(<ptr=int64#5)
# asm 2: vmovupd   <r17=%ymm3,544(<ptr=%r8)
vmovupd   % ymm3, 544( % r8)

# qhasm: r = b11 & a5
# asm 1: vpand <b11=reg256#14,<a5=reg256#3,>r=reg256#4
# asm 2: vpand <b11=%ymm13,<a5=%ymm2,>r=%ymm3
vpand % ymm13, % ymm2, % ymm3

# qhasm: r16 ^= r
# asm 1: vpxor <r=reg256#4,<r16=reg256#6,<r16=reg256#6
# asm 2: vpxor <r=%ymm3,<r16=%ymm5,<r16=%ymm5
vpxor % ymm3, % ymm5, % ymm5

# qhasm: r = b11 & a4
# asm 1: vpand <b11=reg256#14,<a4=reg256#5,>r=reg256#4
# asm 2: vpand <b11=%ymm13,<a4=%ymm4,>r=%ymm3
vpand % ymm13, % ymm4, % ymm3

# qhasm: r15 ^= r
# asm 1: vpxor <r=reg256#4,<r15=reg256#8,<r15=reg256#8
# asm 2: vpxor <r=%ymm3,<r15=%ymm7,<r15=%ymm7
vpxor % ymm3, % ymm7, % ymm7

# qhasm: r = b11 & a3
# asm 1: vpand <b11=reg256#14,<a3=reg256#7,>r=reg256#4
# asm 2: vpand <b11=%ymm13,<a3=%ymm6,>r=%ymm3
vpand % ymm13, % ymm6, % ymm3

# qhasm: r14 ^= r
# asm 1: vpxor <r=reg256#4,<r14=reg256#10,<r14=reg256#10
# asm 2: vpxor <r=%ymm3,<r14=%ymm9,<r14=%ymm9
vpxor % ymm3, % ymm9, % ymm9

# qhasm: r = b11 & a2
# asm 1: vpand <b11=reg256#14,<a2=reg256#9,>r=reg256#4
# asm 2: vpand <b11=%ymm13,<a2=%ymm8,>r=%ymm3
vpand % ymm13, % ymm8, % ymm3

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#4,<r13=reg256#12,<r13=reg256#12
# asm 2: vpxor <r=%ymm3,<r13=%ymm11,<r13=%ymm11
vpxor % ymm3, % ymm11, % ymm11

# qhasm: r = b11 & a1
# asm 1: vpand <b11=reg256#14,<a1=reg256#11,>r=reg256#4
# asm 2: vpand <b11=%ymm13,<a1=%ymm10,>r=%ymm3
vpand % ymm13, % ymm10, % ymm3

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#4,<r12=reg256#1,<r12=reg256#1
# asm 2: vpxor <r=%ymm3,<r12=%ymm0,<r12=%ymm0
vpxor % ymm3, % ymm0, % ymm0

# qhasm: r11 = b11 & a0
# asm 1: vpand <b11=reg256#14,<a0=reg256#13,>r11=reg256#4
# asm 2: vpand <b11=%ymm13,<a0=%ymm12,>r11=%ymm3
vpand % ymm13, % ymm12, % ymm3

# qhasm: b10 = mem128[ input_2 + 0 ] x2
# asm 1: vbroadcasti128 0(<input_2=int64#3), >b10=reg256#14
# asm 2: vbroadcasti128 0(<input_2=%rdx), >b10=%ymm13
vbroadcasti128 0( % rdx), % ymm13

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  % rcx, % rdx

# qhasm: r = b10 & a6
# asm 1: vpand <b10=reg256#14,<a6=reg256#2,>r=reg256#15
# asm 2: vpand <b10=%ymm13,<a6=%ymm1,>r=%ymm14
vpand % ymm13, % ymm1, % ymm14

# qhasm: r16 ^= r
# asm 1: vpxor <r=reg256#15,<r16=reg256#6,<r16=reg256#6
# asm 2: vpxor <r=%ymm14,<r16=%ymm5,<r16=%ymm5
vpxor % ymm14, % ymm5, % ymm5

# qhasm: mem256[ ptr + 512 ] = r16
# asm 1: vmovupd   <r16=reg256#6,512(<ptr=int64#5)
# asm 2: vmovupd   <r16=%ymm5,512(<ptr=%r8)
vmovupd   % ymm5, 512( % r8)

# qhasm: r = b10 & a5
# asm 1: vpand <b10=reg256#14,<a5=reg256#3,>r=reg256#6
# asm 2: vpand <b10=%ymm13,<a5=%ymm2,>r=%ymm5
vpand % ymm13, % ymm2, % ymm5

# qhasm: r15 ^= r
# asm 1: vpxor <r=reg256#6,<r15=reg256#8,<r15=reg256#8
# asm 2: vpxor <r=%ymm5,<r15=%ymm7,<r15=%ymm7
vpxor % ymm5, % ymm7, % ymm7

# qhasm: r = b10 & a4
# asm 1: vpand <b10=reg256#14,<a4=reg256#5,>r=reg256#6
# asm 2: vpand <b10=%ymm13,<a4=%ymm4,>r=%ymm5
vpand % ymm13, % ymm4, % ymm5

# qhasm: r14 ^= r
# asm 1: vpxor <r=reg256#6,<r14=reg256#10,<r14=reg256#10
# asm 2: vpxor <r=%ymm5,<r14=%ymm9,<r14=%ymm9
vpxor % ymm5, % ymm9, % ymm9

# qhasm: r = b10 & a3
# asm 1: vpand <b10=reg256#14,<a3=reg256#7,>r=reg256#6
# asm 2: vpand <b10=%ymm13,<a3=%ymm6,>r=%ymm5
vpand % ymm13, % ymm6, % ymm5

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#6,<r13=reg256#12,<r13=reg256#12
# asm 2: vpxor <r=%ymm5,<r13=%ymm11,<r13=%ymm11
vpxor % ymm5, % ymm11, % ymm11

# qhasm: r = b10 & a2
# asm 1: vpand <b10=reg256#14,<a2=reg256#9,>r=reg256#6
# asm 2: vpand <b10=%ymm13,<a2=%ymm8,>r=%ymm5
vpand % ymm13, % ymm8, % ymm5

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#6,<r12=reg256#1,<r12=reg256#1
# asm 2: vpxor <r=%ymm5,<r12=%ymm0,<r12=%ymm0
vpxor % ymm5, % ymm0, % ymm0

# qhasm: r = b10 & a1
# asm 1: vpand <b10=reg256#14,<a1=reg256#11,>r=reg256#6
# asm 2: vpand <b10=%ymm13,<a1=%ymm10,>r=%ymm5
vpand % ymm13, % ymm10, % ymm5

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#6,<r11=reg256#4,<r11=reg256#4
# asm 2: vpxor <r=%ymm5,<r11=%ymm3,<r11=%ymm3
vpxor % ymm5, % ymm3, % ymm3

# qhasm: r10 = b10 & a0
# asm 1: vpand <b10=reg256#14,<a0=reg256#13,>r10=reg256#6
# asm 2: vpand <b10=%ymm13,<a0=%ymm12,>r10=%ymm5
vpand % ymm13, % ymm12, % ymm5

# qhasm: b9 = mem128[ input_2 + 0 ] x2
# asm 1: vbroadcasti128 0(<input_2=int64#3), >b9=reg256#14
# asm 2: vbroadcasti128 0(<input_2=%rdx), >b9=%ymm13
vbroadcasti128 0( % rdx), % ymm13

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  % rcx, % rdx

# qhasm: r = b9 & a6
# asm 1: vpand <b9=reg256#14,<a6=reg256#2,>r=reg256#15
# asm 2: vpand <b9=%ymm13,<a6=%ymm1,>r=%ymm14
vpand % ymm13, % ymm1, % ymm14

# qhasm: r15 ^= r
# asm 1: vpxor <r=reg256#15,<r15=reg256#8,<r15=reg256#8
# asm 2: vpxor <r=%ymm14,<r15=%ymm7,<r15=%ymm7
vpxor % ymm14, % ymm7, % ymm7

# qhasm: mem256[ ptr + 480 ] = r15
# asm 1: vmovupd   <r15=reg256#8,480(<ptr=int64#5)
# asm 2: vmovupd   <r15=%ymm7,480(<ptr=%r8)
vmovupd   % ymm7, 480( % r8)

# qhasm: r = b9 & a5
# asm 1: vpand <b9=reg256#14,<a5=reg256#3,>r=reg256#8
# asm 2: vpand <b9=%ymm13,<a5=%ymm2,>r=%ymm7
vpand % ymm13, % ymm2, % ymm7

# qhasm: r14 ^= r
# asm 1: vpxor <r=reg256#8,<r14=reg256#10,<r14=reg256#10
# asm 2: vpxor <r=%ymm7,<r14=%ymm9,<r14=%ymm9
vpxor % ymm7, % ymm9, % ymm9

# qhasm: r = b9 & a4
# asm 1: vpand <b9=reg256#14,<a4=reg256#5,>r=reg256#8
# asm 2: vpand <b9=%ymm13,<a4=%ymm4,>r=%ymm7
vpand % ymm13, % ymm4, % ymm7

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#8,<r13=reg256#12,<r13=reg256#12
# asm 2: vpxor <r=%ymm7,<r13=%ymm11,<r13=%ymm11
vpxor % ymm7, % ymm11, % ymm11

# qhasm: r = b9 & a3
# asm 1: vpand <b9=reg256#14,<a3=reg256#7,>r=reg256#8
# asm 2: vpand <b9=%ymm13,<a3=%ymm6,>r=%ymm7
vpand % ymm13, % ymm6, % ymm7

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#8,<r12=reg256#1,<r12=reg256#1
# asm 2: vpxor <r=%ymm7,<r12=%ymm0,<r12=%ymm0
vpxor % ymm7, % ymm0, % ymm0

# qhasm: r = b9 & a2
# asm 1: vpand <b9=reg256#14,<a2=reg256#9,>r=reg256#8
# asm 2: vpand <b9=%ymm13,<a2=%ymm8,>r=%ymm7
vpand % ymm13, % ymm8, % ymm7

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#8,<r11=reg256#4,<r11=reg256#4
# asm 2: vpxor <r=%ymm7,<r11=%ymm3,<r11=%ymm3
vpxor % ymm7, % ymm3, % ymm3

# qhasm: r = b9 & a1
# asm 1: vpand <b9=reg256#14,<a1=reg256#11,>r=reg256#8
# asm 2: vpand <b9=%ymm13,<a1=%ymm10,>r=%ymm7
vpand % ymm13, % ymm10, % ymm7

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#8,<r10=reg256#6,<r10=reg256#6
# asm 2: vpxor <r=%ymm7,<r10=%ymm5,<r10=%ymm5
vpxor % ymm7, % ymm5, % ymm5

# qhasm: r9 = b9 & a0
# asm 1: vpand <b9=reg256#14,<a0=reg256#13,>r9=reg256#8
# asm 2: vpand <b9=%ymm13,<a0=%ymm12,>r9=%ymm7
vpand % ymm13, % ymm12, % ymm7

# qhasm: b8 = mem128[ input_2 + 0 ] x2
# asm 1: vbroadcasti128 0(<input_2=int64#3), >b8=reg256#14
# asm 2: vbroadcasti128 0(<input_2=%rdx), >b8=%ymm13
vbroadcasti128 0( % rdx), % ymm13

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  % rcx, % rdx

# qhasm: r = b8 & a6
# asm 1: vpand <b8=reg256#14,<a6=reg256#2,>r=reg256#15
# asm 2: vpand <b8=%ymm13,<a6=%ymm1,>r=%ymm14
vpand % ymm13, % ymm1, % ymm14

# qhasm: r14 ^= r
# asm 1: vpxor <r=reg256#15,<r14=reg256#10,<r14=reg256#10
# asm 2: vpxor <r=%ymm14,<r14=%ymm9,<r14=%ymm9
vpxor % ymm14, % ymm9, % ymm9

# qhasm: mem256[ ptr + 448 ] = r14
# asm 1: vmovupd   <r14=reg256#10,448(<ptr=int64#5)
# asm 2: vmovupd   <r14=%ymm9,448(<ptr=%r8)
vmovupd   % ymm9, 448( % r8)

# qhasm: r = b8 & a5
# asm 1: vpand <b8=reg256#14,<a5=reg256#3,>r=reg256#10
# asm 2: vpand <b8=%ymm13,<a5=%ymm2,>r=%ymm9
vpand % ymm13, % ymm2, % ymm9

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#10,<r13=reg256#12,<r13=reg256#12
# asm 2: vpxor <r=%ymm9,<r13=%ymm11,<r13=%ymm11
vpxor % ymm9, % ymm11, % ymm11

# qhasm: r = b8 & a4
# asm 1: vpand <b8=reg256#14,<a4=reg256#5,>r=reg256#10
# asm 2: vpand <b8=%ymm13,<a4=%ymm4,>r=%ymm9
vpand % ymm13, % ymm4, % ymm9

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#10,<r12=reg256#1,<r12=reg256#1
# asm 2: vpxor <r=%ymm9,<r12=%ymm0,<r12=%ymm0
vpxor % ymm9, % ymm0, % ymm0

# qhasm: r = b8 & a3
# asm 1: vpand <b8=reg256#14,<a3=reg256#7,>r=reg256#10
# asm 2: vpand <b8=%ymm13,<a3=%ymm6,>r=%ymm9
vpand % ymm13, % ymm6, % ymm9

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#10,<r11=reg256#4,<r11=reg256#4
# asm 2: vpxor <r=%ymm9,<r11=%ymm3,<r11=%ymm3
vpxor % ymm9, % ymm3, % ymm3

# qhasm: r = b8 & a2
# asm 1: vpand <b8=reg256#14,<a2=reg256#9,>r=reg256#10
# asm 2: vpand <b8=%ymm13,<a2=%ymm8,>r=%ymm9
vpand % ymm13, % ymm8, % ymm9

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#10,<r10=reg256#6,<r10=reg256#6
# asm 2: vpxor <r=%ymm9,<r10=%ymm5,<r10=%ymm5
vpxor % ymm9, % ymm5, % ymm5

# qhasm: r = b8 & a1
# asm 1: vpand <b8=reg256#14,<a1=reg256#11,>r=reg256#10
# asm 2: vpand <b8=%ymm13,<a1=%ymm10,>r=%ymm9
vpand % ymm13, % ymm10, % ymm9

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#10,<r9=reg256#8,<r9=reg256#8
# asm 2: vpxor <r=%ymm9,<r9=%ymm7,<r9=%ymm7
vpxor % ymm9, % ymm7, % ymm7

# qhasm: r8 = b8 & a0
# asm 1: vpand <b8=reg256#14,<a0=reg256#13,>r8=reg256#10
# asm 2: vpand <b8=%ymm13,<a0=%ymm12,>r8=%ymm9
vpand % ymm13, % ymm12, % ymm9

# qhasm: b7 = mem128[ input_2 + 0 ] x2
# asm 1: vbroadcasti128 0(<input_2=int64#3), >b7=reg256#14
# asm 2: vbroadcasti128 0(<input_2=%rdx), >b7=%ymm13
vbroadcasti128 0( % rdx), % ymm13

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  % rcx, % rdx

# qhasm: r = b7 & a6
# asm 1: vpand <b7=reg256#14,<a6=reg256#2,>r=reg256#15
# asm 2: vpand <b7=%ymm13,<a6=%ymm1,>r=%ymm14
vpand % ymm13, % ymm1, % ymm14

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#15,<r13=reg256#12,<r13=reg256#12
# asm 2: vpxor <r=%ymm14,<r13=%ymm11,<r13=%ymm11
vpxor % ymm14, % ymm11, % ymm11

# qhasm: mem256[ ptr + 416 ] = r13
# asm 1: vmovupd   <r13=reg256#12,416(<ptr=int64#5)
# asm 2: vmovupd   <r13=%ymm11,416(<ptr=%r8)
vmovupd   % ymm11, 416( % r8)

# qhasm: r = b7 & a5
# asm 1: vpand <b7=reg256#14,<a5=reg256#3,>r=reg256#12
# asm 2: vpand <b7=%ymm13,<a5=%ymm2,>r=%ymm11
vpand % ymm13, % ymm2, % ymm11

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#12,<r12=reg256#1,<r12=reg256#1
# asm 2: vpxor <r=%ymm11,<r12=%ymm0,<r12=%ymm0
vpxor % ymm11, % ymm0, % ymm0

# qhasm: r = b7 & a4
# asm 1: vpand <b7=reg256#14,<a4=reg256#5,>r=reg256#12
# asm 2: vpand <b7=%ymm13,<a4=%ymm4,>r=%ymm11
vpand % ymm13, % ymm4, % ymm11

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#12,<r11=reg256#4,<r11=reg256#4
# asm 2: vpxor <r=%ymm11,<r11=%ymm3,<r11=%ymm3
vpxor % ymm11, % ymm3, % ymm3

# qhasm: r = b7 & a3
# asm 1: vpand <b7=reg256#14,<a3=reg256#7,>r=reg256#12
# asm 2: vpand <b7=%ymm13,<a3=%ymm6,>r=%ymm11
vpand % ymm13, % ymm6, % ymm11

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#12,<r10=reg256#6,<r10=reg256#6
# asm 2: vpxor <r=%ymm11,<r10=%ymm5,<r10=%ymm5
vpxor % ymm11, % ymm5, % ymm5

# qhasm: r = b7 & a2
# asm 1: vpand <b7=reg256#14,<a2=reg256#9,>r=reg256#12
# asm 2: vpand <b7=%ymm13,<a2=%ymm8,>r=%ymm11
vpand % ymm13, % ymm8, % ymm11

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#12,<r9=reg256#8,<r9=reg256#8
# asm 2: vpxor <r=%ymm11,<r9=%ymm7,<r9=%ymm7
vpxor % ymm11, % ymm7, % ymm7

# qhasm: r = b7 & a1
# asm 1: vpand <b7=reg256#14,<a1=reg256#11,>r=reg256#12
# asm 2: vpand <b7=%ymm13,<a1=%ymm10,>r=%ymm11
vpand % ymm13, % ymm10, % ymm11

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#12,<r8=reg256#10,<r8=reg256#10
# asm 2: vpxor <r=%ymm11,<r8=%ymm9,<r8=%ymm9
vpxor % ymm11, % ymm9, % ymm9

# qhasm: r7 = b7 & a0
# asm 1: vpand <b7=reg256#14,<a0=reg256#13,>r7=reg256#12
# asm 2: vpand <b7=%ymm13,<a0=%ymm12,>r7=%ymm11
vpand % ymm13, % ymm12, % ymm11

# qhasm: b6 = mem128[ input_2 + 0 ] x2
# asm 1: vbroadcasti128 0(<input_2=int64#3), >b6=reg256#14
# asm 2: vbroadcasti128 0(<input_2=%rdx), >b6=%ymm13
vbroadcasti128 0( % rdx), % ymm13

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  % rcx, % rdx

# qhasm: r = b6 & a6
# asm 1: vpand <b6=reg256#14,<a6=reg256#2,>r=reg256#15
# asm 2: vpand <b6=%ymm13,<a6=%ymm1,>r=%ymm14
vpand % ymm13, % ymm1, % ymm14

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#15,<r12=reg256#1,<r12=reg256#1
# asm 2: vpxor <r=%ymm14,<r12=%ymm0,<r12=%ymm0
vpxor % ymm14, % ymm0, % ymm0

# qhasm: mem256[ ptr + 384 ] = r12
# asm 1: vmovupd   <r12=reg256#1,384(<ptr=int64#5)
# asm 2: vmovupd   <r12=%ymm0,384(<ptr=%r8)
vmovupd   % ymm0, 384( % r8)

# qhasm: r = b6 & a5
# asm 1: vpand <b6=reg256#14,<a5=reg256#3,>r=reg256#1
# asm 2: vpand <b6=%ymm13,<a5=%ymm2,>r=%ymm0
vpand % ymm13, % ymm2, % ymm0

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#1,<r11=reg256#4,<r11=reg256#4
# asm 2: vpxor <r=%ymm0,<r11=%ymm3,<r11=%ymm3
vpxor % ymm0, % ymm3, % ymm3

# qhasm: r = b6 & a4
# asm 1: vpand <b6=reg256#14,<a4=reg256#5,>r=reg256#1
# asm 2: vpand <b6=%ymm13,<a4=%ymm4,>r=%ymm0
vpand % ymm13, % ymm4, % ymm0

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#1,<r10=reg256#6,<r10=reg256#6
# asm 2: vpxor <r=%ymm0,<r10=%ymm5,<r10=%ymm5
vpxor % ymm0, % ymm5, % ymm5

# qhasm: r = b6 & a3
# asm 1: vpand <b6=reg256#14,<a3=reg256#7,>r=reg256#1
# asm 2: vpand <b6=%ymm13,<a3=%ymm6,>r=%ymm0
vpand % ymm13, % ymm6, % ymm0

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#1,<r9=reg256#8,<r9=reg256#8
# asm 2: vpxor <r=%ymm0,<r9=%ymm7,<r9=%ymm7
vpxor % ymm0, % ymm7, % ymm7

# qhasm: r = b6 & a2
# asm 1: vpand <b6=reg256#14,<a2=reg256#9,>r=reg256#1
# asm 2: vpand <b6=%ymm13,<a2=%ymm8,>r=%ymm0
vpand % ymm13, % ymm8, % ymm0

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#1,<r8=reg256#10,<r8=reg256#10
# asm 2: vpxor <r=%ymm0,<r8=%ymm9,<r8=%ymm9
vpxor % ymm0, % ymm9, % ymm9

# qhasm: r = b6 & a1
# asm 1: vpand <b6=reg256#14,<a1=reg256#11,>r=reg256#1
# asm 2: vpand <b6=%ymm13,<a1=%ymm10,>r=%ymm0
vpand % ymm13, % ymm10, % ymm0

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#1,<r7=reg256#12,<r7=reg256#12
# asm 2: vpxor <r=%ymm0,<r7=%ymm11,<r7=%ymm11
vpxor % ymm0, % ymm11, % ymm11

# qhasm: r6 = b6 & a0
# asm 1: vpand <b6=reg256#14,<a0=reg256#13,>r6=reg256#1
# asm 2: vpand <b6=%ymm13,<a0=%ymm12,>r6=%ymm0
vpand % ymm13, % ymm12, % ymm0

# qhasm: b5 = mem128[ input_2 + 0 ] x2
# asm 1: vbroadcasti128 0(<input_2=int64#3), >b5=reg256#14
# asm 2: vbroadcasti128 0(<input_2=%rdx), >b5=%ymm13
vbroadcasti128 0( % rdx), % ymm13

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  % rcx, % rdx

# qhasm: r = b5 & a6
# asm 1: vpand <b5=reg256#14,<a6=reg256#2,>r=reg256#15
# asm 2: vpand <b5=%ymm13,<a6=%ymm1,>r=%ymm14
vpand % ymm13, % ymm1, % ymm14

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#15,<r11=reg256#4,<r11=reg256#4
# asm 2: vpxor <r=%ymm14,<r11=%ymm3,<r11=%ymm3
vpxor % ymm14, % ymm3, % ymm3

# qhasm: mem256[ ptr + 352 ] = r11
# asm 1: vmovupd   <r11=reg256#4,352(<ptr=int64#5)
# asm 2: vmovupd   <r11=%ymm3,352(<ptr=%r8)
vmovupd   % ymm3, 352( % r8)

# qhasm: r = b5 & a5
# asm 1: vpand <b5=reg256#14,<a5=reg256#3,>r=reg256#4
# asm 2: vpand <b5=%ymm13,<a5=%ymm2,>r=%ymm3
vpand % ymm13, % ymm2, % ymm3

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#4,<r10=reg256#6,<r10=reg256#6
# asm 2: vpxor <r=%ymm3,<r10=%ymm5,<r10=%ymm5
vpxor % ymm3, % ymm5, % ymm5

# qhasm: r = b5 & a4
# asm 1: vpand <b5=reg256#14,<a4=reg256#5,>r=reg256#4
# asm 2: vpand <b5=%ymm13,<a4=%ymm4,>r=%ymm3
vpand % ymm13, % ymm4, % ymm3

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#4,<r9=reg256#8,<r9=reg256#8
# asm 2: vpxor <r=%ymm3,<r9=%ymm7,<r9=%ymm7
vpxor % ymm3, % ymm7, % ymm7

# qhasm: r = b5 & a3
# asm 1: vpand <b5=reg256#14,<a3=reg256#7,>r=reg256#4
# asm 2: vpand <b5=%ymm13,<a3=%ymm6,>r=%ymm3
vpand % ymm13, % ymm6, % ymm3

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#4,<r8=reg256#10,<r8=reg256#10
# asm 2: vpxor <r=%ymm3,<r8=%ymm9,<r8=%ymm9
vpxor % ymm3, % ymm9, % ymm9

# qhasm: r = b5 & a2
# asm 1: vpand <b5=reg256#14,<a2=reg256#9,>r=reg256#4
# asm 2: vpand <b5=%ymm13,<a2=%ymm8,>r=%ymm3
vpand % ymm13, % ymm8, % ymm3

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#4,<r7=reg256#12,<r7=reg256#12
# asm 2: vpxor <r=%ymm3,<r7=%ymm11,<r7=%ymm11
vpxor % ymm3, % ymm11, % ymm11

# qhasm: r = b5 & a1
# asm 1: vpand <b5=reg256#14,<a1=reg256#11,>r=reg256#4
# asm 2: vpand <b5=%ymm13,<a1=%ymm10,>r=%ymm3
vpand % ymm13, % ymm10, % ymm3

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#4,<r6=reg256#1,<r6=reg256#1
# asm 2: vpxor <r=%ymm3,<r6=%ymm0,<r6=%ymm0
vpxor % ymm3, % ymm0, % ymm0

# qhasm: r5 = b5 & a0
# asm 1: vpand <b5=reg256#14,<a0=reg256#13,>r5=reg256#4
# asm 2: vpand <b5=%ymm13,<a0=%ymm12,>r5=%ymm3
vpand % ymm13, % ymm12, % ymm3

# qhasm: b4 = mem128[ input_2 + 0 ] x2
# asm 1: vbroadcasti128 0(<input_2=int64#3), >b4=reg256#14
# asm 2: vbroadcasti128 0(<input_2=%rdx), >b4=%ymm13
vbroadcasti128 0( % rdx), % ymm13

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  % rcx, % rdx

# qhasm: r = b4 & a6
# asm 1: vpand <b4=reg256#14,<a6=reg256#2,>r=reg256#15
# asm 2: vpand <b4=%ymm13,<a6=%ymm1,>r=%ymm14
vpand % ymm13, % ymm1, % ymm14

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#15,<r10=reg256#6,<r10=reg256#6
# asm 2: vpxor <r=%ymm14,<r10=%ymm5,<r10=%ymm5
vpxor % ymm14, % ymm5, % ymm5

# qhasm: mem256[ ptr + 320 ] = r10
# asm 1: vmovupd   <r10=reg256#6,320(<ptr=int64#5)
# asm 2: vmovupd   <r10=%ymm5,320(<ptr=%r8)
vmovupd   % ymm5, 320( % r8)

# qhasm: r = b4 & a5
# asm 1: vpand <b4=reg256#14,<a5=reg256#3,>r=reg256#6
# asm 2: vpand <b4=%ymm13,<a5=%ymm2,>r=%ymm5
vpand % ymm13, % ymm2, % ymm5

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#6,<r9=reg256#8,<r9=reg256#8
# asm 2: vpxor <r=%ymm5,<r9=%ymm7,<r9=%ymm7
vpxor % ymm5, % ymm7, % ymm7

# qhasm: r = b4 & a4
# asm 1: vpand <b4=reg256#14,<a4=reg256#5,>r=reg256#6
# asm 2: vpand <b4=%ymm13,<a4=%ymm4,>r=%ymm5
vpand % ymm13, % ymm4, % ymm5

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#6,<r8=reg256#10,<r8=reg256#10
# asm 2: vpxor <r=%ymm5,<r8=%ymm9,<r8=%ymm9
vpxor % ymm5, % ymm9, % ymm9

# qhasm: r = b4 & a3
# asm 1: vpand <b4=reg256#14,<a3=reg256#7,>r=reg256#6
# asm 2: vpand <b4=%ymm13,<a3=%ymm6,>r=%ymm5
vpand % ymm13, % ymm6, % ymm5

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#6,<r7=reg256#12,<r7=reg256#12
# asm 2: vpxor <r=%ymm5,<r7=%ymm11,<r7=%ymm11
vpxor % ymm5, % ymm11, % ymm11

# qhasm: r = b4 & a2
# asm 1: vpand <b4=reg256#14,<a2=reg256#9,>r=reg256#6
# asm 2: vpand <b4=%ymm13,<a2=%ymm8,>r=%ymm5
vpand % ymm13, % ymm8, % ymm5

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#6,<r6=reg256#1,<r6=reg256#1
# asm 2: vpxor <r=%ymm5,<r6=%ymm0,<r6=%ymm0
vpxor % ymm5, % ymm0, % ymm0

# qhasm: r = b4 & a1
# asm 1: vpand <b4=reg256#14,<a1=reg256#11,>r=reg256#6
# asm 2: vpand <b4=%ymm13,<a1=%ymm10,>r=%ymm5
vpand % ymm13, % ymm10, % ymm5

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#6,<r5=reg256#4,<r5=reg256#4
# asm 2: vpxor <r=%ymm5,<r5=%ymm3,<r5=%ymm3
vpxor % ymm5, % ymm3, % ymm3

# qhasm: r4 = b4 & a0
# asm 1: vpand <b4=reg256#14,<a0=reg256#13,>r4=reg256#6
# asm 2: vpand <b4=%ymm13,<a0=%ymm12,>r4=%ymm5
vpand % ymm13, % ymm12, % ymm5

# qhasm: b3 = mem128[ input_2 + 0 ] x2
# asm 1: vbroadcasti128 0(<input_2=int64#3), >b3=reg256#14
# asm 2: vbroadcasti128 0(<input_2=%rdx), >b3=%ymm13
vbroadcasti128 0( % rdx), % ymm13

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  % rcx, % rdx

# qhasm: r = b3 & a6
# asm 1: vpand <b3=reg256#14,<a6=reg256#2,>r=reg256#15
# asm 2: vpand <b3=%ymm13,<a6=%ymm1,>r=%ymm14
vpand % ymm13, % ymm1, % ymm14

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#15,<r9=reg256#8,<r9=reg256#8
# asm 2: vpxor <r=%ymm14,<r9=%ymm7,<r9=%ymm7
vpxor % ymm14, % ymm7, % ymm7

# qhasm: mem256[ ptr + 288 ] = r9
# asm 1: vmovupd   <r9=reg256#8,288(<ptr=int64#5)
# asm 2: vmovupd   <r9=%ymm7,288(<ptr=%r8)
vmovupd   % ymm7, 288( % r8)

# qhasm: r = b3 & a5
# asm 1: vpand <b3=reg256#14,<a5=reg256#3,>r=reg256#8
# asm 2: vpand <b3=%ymm13,<a5=%ymm2,>r=%ymm7
vpand % ymm13, % ymm2, % ymm7

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#8,<r8=reg256#10,<r8=reg256#10
# asm 2: vpxor <r=%ymm7,<r8=%ymm9,<r8=%ymm9
vpxor % ymm7, % ymm9, % ymm9

# qhasm: r = b3 & a4
# asm 1: vpand <b3=reg256#14,<a4=reg256#5,>r=reg256#8
# asm 2: vpand <b3=%ymm13,<a4=%ymm4,>r=%ymm7
vpand % ymm13, % ymm4, % ymm7

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#8,<r7=reg256#12,<r7=reg256#12
# asm 2: vpxor <r=%ymm7,<r7=%ymm11,<r7=%ymm11
vpxor % ymm7, % ymm11, % ymm11

# qhasm: r = b3 & a3
# asm 1: vpand <b3=reg256#14,<a3=reg256#7,>r=reg256#8
# asm 2: vpand <b3=%ymm13,<a3=%ymm6,>r=%ymm7
vpand % ymm13, % ymm6, % ymm7

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#8,<r6=reg256#1,<r6=reg256#1
# asm 2: vpxor <r=%ymm7,<r6=%ymm0,<r6=%ymm0
vpxor % ymm7, % ymm0, % ymm0

# qhasm: r = b3 & a2
# asm 1: vpand <b3=reg256#14,<a2=reg256#9,>r=reg256#8
# asm 2: vpand <b3=%ymm13,<a2=%ymm8,>r=%ymm7
vpand % ymm13, % ymm8, % ymm7

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#8,<r5=reg256#4,<r5=reg256#4
# asm 2: vpxor <r=%ymm7,<r5=%ymm3,<r5=%ymm3
vpxor % ymm7, % ymm3, % ymm3

# qhasm: r = b3 & a1
# asm 1: vpand <b3=reg256#14,<a1=reg256#11,>r=reg256#8
# asm 2: vpand <b3=%ymm13,<a1=%ymm10,>r=%ymm7
vpand % ymm13, % ymm10, % ymm7

# qhasm: r4 ^= r
# asm 1: vpxor <r=reg256#8,<r4=reg256#6,<r4=reg256#6
# asm 2: vpxor <r=%ymm7,<r4=%ymm5,<r4=%ymm5
vpxor % ymm7, % ymm5, % ymm5

# qhasm: r3 = b3 & a0
# asm 1: vpand <b3=reg256#14,<a0=reg256#13,>r3=reg256#8
# asm 2: vpand <b3=%ymm13,<a0=%ymm12,>r3=%ymm7
vpand % ymm13, % ymm12, % ymm7

# qhasm: b2 = mem128[ input_2 + 0 ] x2
# asm 1: vbroadcasti128 0(<input_2=int64#3), >b2=reg256#14
# asm 2: vbroadcasti128 0(<input_2=%rdx), >b2=%ymm13
vbroadcasti128 0( % rdx), % ymm13

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  % rcx, % rdx

# qhasm: r = b2 & a6
# asm 1: vpand <b2=reg256#14,<a6=reg256#2,>r=reg256#15
# asm 2: vpand <b2=%ymm13,<a6=%ymm1,>r=%ymm14
vpand % ymm13, % ymm1, % ymm14

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#15,<r8=reg256#10,<r8=reg256#10
# asm 2: vpxor <r=%ymm14,<r8=%ymm9,<r8=%ymm9
vpxor % ymm14, % ymm9, % ymm9

# qhasm: mem256[ ptr + 256 ] = r8
# asm 1: vmovupd   <r8=reg256#10,256(<ptr=int64#5)
# asm 2: vmovupd   <r8=%ymm9,256(<ptr=%r8)
vmovupd   % ymm9, 256( % r8)

# qhasm: r = b2 & a5
# asm 1: vpand <b2=reg256#14,<a5=reg256#3,>r=reg256#10
# asm 2: vpand <b2=%ymm13,<a5=%ymm2,>r=%ymm9
vpand % ymm13, % ymm2, % ymm9

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#10,<r7=reg256#12,<r7=reg256#12
# asm 2: vpxor <r=%ymm9,<r7=%ymm11,<r7=%ymm11
vpxor % ymm9, % ymm11, % ymm11

# qhasm: r = b2 & a4
# asm 1: vpand <b2=reg256#14,<a4=reg256#5,>r=reg256#10
# asm 2: vpand <b2=%ymm13,<a4=%ymm4,>r=%ymm9
vpand % ymm13, % ymm4, % ymm9

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#10,<r6=reg256#1,<r6=reg256#1
# asm 2: vpxor <r=%ymm9,<r6=%ymm0,<r6=%ymm0
vpxor % ymm9, % ymm0, % ymm0

# qhasm: r = b2 & a3
# asm 1: vpand <b2=reg256#14,<a3=reg256#7,>r=reg256#10
# asm 2: vpand <b2=%ymm13,<a3=%ymm6,>r=%ymm9
vpand % ymm13, % ymm6, % ymm9

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#10,<r5=reg256#4,<r5=reg256#4
# asm 2: vpxor <r=%ymm9,<r5=%ymm3,<r5=%ymm3
vpxor % ymm9, % ymm3, % ymm3

# qhasm: r = b2 & a2
# asm 1: vpand <b2=reg256#14,<a2=reg256#9,>r=reg256#10
# asm 2: vpand <b2=%ymm13,<a2=%ymm8,>r=%ymm9
vpand % ymm13, % ymm8, % ymm9

# qhasm: r4 ^= r
# asm 1: vpxor <r=reg256#10,<r4=reg256#6,<r4=reg256#6
# asm 2: vpxor <r=%ymm9,<r4=%ymm5,<r4=%ymm5
vpxor % ymm9, % ymm5, % ymm5

# qhasm: r = b2 & a1
# asm 1: vpand <b2=reg256#14,<a1=reg256#11,>r=reg256#10
# asm 2: vpand <b2=%ymm13,<a1=%ymm10,>r=%ymm9
vpand % ymm13, % ymm10, % ymm9

# qhasm: r3 ^= r
# asm 1: vpxor <r=reg256#10,<r3=reg256#8,<r3=reg256#8
# asm 2: vpxor <r=%ymm9,<r3=%ymm7,<r3=%ymm7
vpxor % ymm9, % ymm7, % ymm7

# qhasm: r2 = b2 & a0
# asm 1: vpand <b2=reg256#14,<a0=reg256#13,>r2=reg256#10
# asm 2: vpand <b2=%ymm13,<a0=%ymm12,>r2=%ymm9
vpand % ymm13, % ymm12, % ymm9

# qhasm: b1 = mem128[ input_2 + 0 ] x2
# asm 1: vbroadcasti128 0(<input_2=int64#3), >b1=reg256#14
# asm 2: vbroadcasti128 0(<input_2=%rdx), >b1=%ymm13
vbroadcasti128 0( % rdx), % ymm13

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  % rcx, % rdx

# qhasm: r = b1 & a6
# asm 1: vpand <b1=reg256#14,<a6=reg256#2,>r=reg256#15
# asm 2: vpand <b1=%ymm13,<a6=%ymm1,>r=%ymm14
vpand % ymm13, % ymm1, % ymm14

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#15,<r7=reg256#12,<r7=reg256#12
# asm 2: vpxor <r=%ymm14,<r7=%ymm11,<r7=%ymm11
vpxor % ymm14, % ymm11, % ymm11

# qhasm: mem256[ ptr + 224 ] = r7
# asm 1: vmovupd   <r7=reg256#12,224(<ptr=int64#5)
# asm 2: vmovupd   <r7=%ymm11,224(<ptr=%r8)
vmovupd   % ymm11, 224( % r8)

# qhasm: r = b1 & a5
# asm 1: vpand <b1=reg256#14,<a5=reg256#3,>r=reg256#12
# asm 2: vpand <b1=%ymm13,<a5=%ymm2,>r=%ymm11
vpand % ymm13, % ymm2, % ymm11

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#12,<r6=reg256#1,<r6=reg256#1
# asm 2: vpxor <r=%ymm11,<r6=%ymm0,<r6=%ymm0
vpxor % ymm11, % ymm0, % ymm0

# qhasm: r = b1 & a4
# asm 1: vpand <b1=reg256#14,<a4=reg256#5,>r=reg256#12
# asm 2: vpand <b1=%ymm13,<a4=%ymm4,>r=%ymm11
vpand % ymm13, % ymm4, % ymm11

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#12,<r5=reg256#4,<r5=reg256#4
# asm 2: vpxor <r=%ymm11,<r5=%ymm3,<r5=%ymm3
vpxor % ymm11, % ymm3, % ymm3

# qhasm: r = b1 & a3
# asm 1: vpand <b1=reg256#14,<a3=reg256#7,>r=reg256#12
# asm 2: vpand <b1=%ymm13,<a3=%ymm6,>r=%ymm11
vpand % ymm13, % ymm6, % ymm11

# qhasm: r4 ^= r
# asm 1: vpxor <r=reg256#12,<r4=reg256#6,<r4=reg256#6
# asm 2: vpxor <r=%ymm11,<r4=%ymm5,<r4=%ymm5
vpxor % ymm11, % ymm5, % ymm5

# qhasm: r = b1 & a2
# asm 1: vpand <b1=reg256#14,<a2=reg256#9,>r=reg256#12
# asm 2: vpand <b1=%ymm13,<a2=%ymm8,>r=%ymm11
vpand % ymm13, % ymm8, % ymm11

# qhasm: r3 ^= r
# asm 1: vpxor <r=reg256#12,<r3=reg256#8,<r3=reg256#8
# asm 2: vpxor <r=%ymm11,<r3=%ymm7,<r3=%ymm7
vpxor % ymm11, % ymm7, % ymm7

# qhasm: r = b1 & a1
# asm 1: vpand <b1=reg256#14,<a1=reg256#11,>r=reg256#12
# asm 2: vpand <b1=%ymm13,<a1=%ymm10,>r=%ymm11
vpand % ymm13, % ymm10, % ymm11

# qhasm: r2 ^= r
# asm 1: vpxor <r=reg256#12,<r2=reg256#10,<r2=reg256#10
# asm 2: vpxor <r=%ymm11,<r2=%ymm9,<r2=%ymm9
vpxor % ymm11, % ymm9, % ymm9

# qhasm: r1 = b1 & a0
# asm 1: vpand <b1=reg256#14,<a0=reg256#13,>r1=reg256#12
# asm 2: vpand <b1=%ymm13,<a0=%ymm12,>r1=%ymm11
vpand % ymm13, % ymm12, % ymm11

# qhasm: b0 = mem128[ input_2 + 0 ] x2
# asm 1: vbroadcasti128 0(<input_2=int64#3), >b0=reg256#14
# asm 2: vbroadcasti128 0(<input_2=%rdx), >b0=%ymm13
vbroadcasti128 0( % rdx), % ymm13

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  % rcx, % rdx

# qhasm: r = b0 & a6
# asm 1: vpand <b0=reg256#14,<a6=reg256#2,>r=reg256#2
# asm 2: vpand <b0=%ymm13,<a6=%ymm1,>r=%ymm1
vpand % ymm13, % ymm1, % ymm1

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#2,<r6=reg256#1,<r6=reg256#1
# asm 2: vpxor <r=%ymm1,<r6=%ymm0,<r6=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: mem256[ ptr + 192 ] = r6
# asm 1: vmovupd   <r6=reg256#1,192(<ptr=int64#5)
# asm 2: vmovupd   <r6=%ymm0,192(<ptr=%r8)
vmovupd   % ymm0, 192( % r8)

# qhasm: r = b0 & a5
# asm 1: vpand <b0=reg256#14,<a5=reg256#3,>r=reg256#1
# asm 2: vpand <b0=%ymm13,<a5=%ymm2,>r=%ymm0
vpand % ymm13, % ymm2, % ymm0

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#1,<r5=reg256#4,<r5=reg256#4
# asm 2: vpxor <r=%ymm0,<r5=%ymm3,<r5=%ymm3
vpxor % ymm0, % ymm3, % ymm3

# qhasm: r = b0 & a4
# asm 1: vpand <b0=reg256#14,<a4=reg256#5,>r=reg256#1
# asm 2: vpand <b0=%ymm13,<a4=%ymm4,>r=%ymm0
vpand % ymm13, % ymm4, % ymm0

# qhasm: r4 ^= r
# asm 1: vpxor <r=reg256#1,<r4=reg256#6,<r4=reg256#6
# asm 2: vpxor <r=%ymm0,<r4=%ymm5,<r4=%ymm5
vpxor % ymm0, % ymm5, % ymm5

# qhasm: r = b0 & a3
# asm 1: vpand <b0=reg256#14,<a3=reg256#7,>r=reg256#1
# asm 2: vpand <b0=%ymm13,<a3=%ymm6,>r=%ymm0
vpand % ymm13, % ymm6, % ymm0

# qhasm: r3 ^= r
# asm 1: vpxor <r=reg256#1,<r3=reg256#8,<r3=reg256#8
# asm 2: vpxor <r=%ymm0,<r3=%ymm7,<r3=%ymm7
vpxor % ymm0, % ymm7, % ymm7

# qhasm: r = b0 & a2
# asm 1: vpand <b0=reg256#14,<a2=reg256#9,>r=reg256#1
# asm 2: vpand <b0=%ymm13,<a2=%ymm8,>r=%ymm0
vpand % ymm13, % ymm8, % ymm0

# qhasm: r2 ^= r
# asm 1: vpxor <r=reg256#1,<r2=reg256#10,<r2=reg256#10
# asm 2: vpxor <r=%ymm0,<r2=%ymm9,<r2=%ymm9
vpxor % ymm0, % ymm9, % ymm9

# qhasm: r = b0 & a1
# asm 1: vpand <b0=reg256#14,<a1=reg256#11,>r=reg256#1
# asm 2: vpand <b0=%ymm13,<a1=%ymm10,>r=%ymm0
vpand % ymm13, % ymm10, % ymm0

# qhasm: r1 ^= r
# asm 1: vpxor <r=reg256#1,<r1=reg256#12,<r1=reg256#12
# asm 2: vpxor <r=%ymm0,<r1=%ymm11,<r1=%ymm11
vpxor % ymm0, % ymm11, % ymm11

# qhasm: r0 = b0 & a0
# asm 1: vpand <b0=reg256#14,<a0=reg256#13,>r0=reg256#1
# asm 2: vpand <b0=%ymm13,<a0=%ymm12,>r0=%ymm0
vpand % ymm13, % ymm12, % ymm0

# qhasm: mem256[ ptr + 160 ] = r5
# asm 1: vmovupd   <r5=reg256#4,160(<ptr=int64#5)
# asm 2: vmovupd   <r5=%ymm3,160(<ptr=%r8)
vmovupd   % ymm3, 160( % r8)

# qhasm: mem256[ ptr + 128 ] = r4
# asm 1: vmovupd   <r4=reg256#6,128(<ptr=int64#5)
# asm 2: vmovupd   <r4=%ymm5,128(<ptr=%r8)
vmovupd   % ymm5, 128( % r8)

# qhasm: mem256[ ptr + 96 ] = r3
# asm 1: vmovupd   <r3=reg256#8,96(<ptr=int64#5)
# asm 2: vmovupd   <r3=%ymm7,96(<ptr=%r8)
vmovupd   % ymm7, 96( % r8)

# qhasm: mem256[ ptr + 64 ] = r2
# asm 1: vmovupd   <r2=reg256#10,64(<ptr=int64#5)
# asm 2: vmovupd   <r2=%ymm9,64(<ptr=%r8)
vmovupd   % ymm9, 64( % r8)

# qhasm: mem256[ ptr + 32 ] = r1
# asm 1: vmovupd   <r1=reg256#12,32(<ptr=int64#5)
# asm 2: vmovupd   <r1=%ymm11,32(<ptr=%r8)
vmovupd   % ymm11, 32( % r8)

# qhasm: mem256[ ptr + 0 ] = r0
# asm 1: vmovupd   <r0=reg256#1,0(<ptr=int64#5)
# asm 2: vmovupd   <r0=%ymm0,0(<ptr=%r8)
vmovupd   % ymm0, 0( % r8)

# qhasm: vzeroupper
vzeroupper

# qhasm: h24 = mem128[ ptr + 560 ]
# asm 1: movdqu   560(<ptr=int64#5),>h24=reg128#1
# asm 2: movdqu   560(<ptr=%r8),>h24=%xmm0
movdqu   560( % r8), % xmm0

# qhasm: h11 = h24
# asm 1: movdqa <h24=reg128#1,>h11=reg128#2
# asm 2: movdqa <h24=%xmm0,>h11=%xmm1
movdqa % xmm0, % xmm1

# qhasm: h12 = h24
# asm 1: movdqa <h24=reg128#1,>h12=reg128#3
# asm 2: movdqa <h24=%xmm0,>h12=%xmm2
movdqa % xmm0, % xmm2

# qhasm: h14 = h24
# asm 1: movdqa <h24=reg128#1,>h14=reg128#4
# asm 2: movdqa <h24=%xmm0,>h14=%xmm3
movdqa % xmm0, % xmm3

# qhasm: h15 = h24
# asm 1: movdqa <h24=reg128#1,>h15=reg128#1
# asm 2: movdqa <h24=%xmm0,>h15=%xmm0
movdqa % xmm0, % xmm0

# qhasm: h23 = mem128[ ptr + 528 ]
# asm 1: movdqu   528(<ptr=int64#5),>h23=reg128#5
# asm 2: movdqu   528(<ptr=%r8),>h23=%xmm4
movdqu   528( % r8), % xmm4

# qhasm: h10 = h23
# asm 1: movdqa <h23=reg128#5,>h10=reg128#6
# asm 2: movdqa <h23=%xmm4,>h10=%xmm5
movdqa % xmm4, % xmm5

# qhasm: h11 = h11 ^ h23
# asm 1: vpxor <h23=reg128#5,<h11=reg128#2,>h11=reg128#2
# asm 2: vpxor <h23=%xmm4,<h11=%xmm1,>h11=%xmm1
vpxor % xmm4, % xmm1, % xmm1

# qhasm: h13 = h23
# asm 1: movdqa <h23=reg128#5,>h13=reg128#7
# asm 2: movdqa <h23=%xmm4,>h13=%xmm6
movdqa % xmm4, % xmm6

# qhasm: h14 = h14 ^ h23
# asm 1: vpxor <h23=reg128#5,<h14=reg128#4,>h14=reg128#4
# asm 2: vpxor <h23=%xmm4,<h14=%xmm3,>h14=%xmm3
vpxor % xmm4, % xmm3, % xmm3

# qhasm: h22 = mem128[ ptr + 496 ]
# asm 1: movdqu   496(<ptr=int64#5),>h22=reg128#5
# asm 2: movdqu   496(<ptr=%r8),>h22=%xmm4
movdqu   496( % r8), % xmm4

# qhasm: h9 = h22
# asm 1: movdqa <h22=reg128#5,>h9=reg128#8
# asm 2: movdqa <h22=%xmm4,>h9=%xmm7
movdqa % xmm4, % xmm7

# qhasm: h10 = h10 ^ h22
# asm 1: vpxor <h22=reg128#5,<h10=reg128#6,>h10=reg128#6
# asm 2: vpxor <h22=%xmm4,<h10=%xmm5,>h10=%xmm5
vpxor % xmm4, % xmm5, % xmm5

# qhasm: h12 = h12 ^ h22
# asm 1: vpxor <h22=reg128#5,<h12=reg128#3,>h12=reg128#3
# asm 2: vpxor <h22=%xmm4,<h12=%xmm2,>h12=%xmm2
vpxor % xmm4, % xmm2, % xmm2

# qhasm: h13 = h13 ^ h22
# asm 1: vpxor <h22=reg128#5,<h13=reg128#7,>h13=reg128#5
# asm 2: vpxor <h22=%xmm4,<h13=%xmm6,>h13=%xmm4
vpxor % xmm4, % xmm6, % xmm4

# qhasm: h21 = mem128[ ptr + 464 ]
# asm 1: movdqu   464(<ptr=int64#5),>h21=reg128#7
# asm 2: movdqu   464(<ptr=%r8),>h21=%xmm6
movdqu   464( % r8), % xmm6

# qhasm: h8 = h21
# asm 1: movdqa <h21=reg128#7,>h8=reg128#9
# asm 2: movdqa <h21=%xmm6,>h8=%xmm8
movdqa % xmm6, % xmm8

# qhasm: h9 = h9 ^ h21
# asm 1: vpxor <h21=reg128#7,<h9=reg128#8,>h9=reg128#8
# asm 2: vpxor <h21=%xmm6,<h9=%xmm7,>h9=%xmm7
vpxor % xmm6, % xmm7, % xmm7

# qhasm: h11 = h11 ^ h21
# asm 1: vpxor <h21=reg128#7,<h11=reg128#2,>h11=reg128#2
# asm 2: vpxor <h21=%xmm6,<h11=%xmm1,>h11=%xmm1
vpxor % xmm6, % xmm1, % xmm1

# qhasm: h12 = h12 ^ h21
# asm 1: vpxor <h21=reg128#7,<h12=reg128#3,>h12=reg128#3
# asm 2: vpxor <h21=%xmm6,<h12=%xmm2,>h12=%xmm2
vpxor % xmm6, % xmm2, % xmm2

# qhasm: h20 = mem128[ ptr + 432 ]
# asm 1: movdqu   432(<ptr=int64#5),>h20=reg128#7
# asm 2: movdqu   432(<ptr=%r8),>h20=%xmm6
movdqu   432( % r8), % xmm6

# qhasm: h7 = h20
# asm 1: movdqa <h20=reg128#7,>h7=reg128#10
# asm 2: movdqa <h20=%xmm6,>h7=%xmm9
movdqa % xmm6, % xmm9

# qhasm: h8 = h8 ^ h20
# asm 1: vpxor <h20=reg128#7,<h8=reg128#9,>h8=reg128#9
# asm 2: vpxor <h20=%xmm6,<h8=%xmm8,>h8=%xmm8
vpxor % xmm6, % xmm8, % xmm8

# qhasm: h10 = h10 ^ h20
# asm 1: vpxor <h20=reg128#7,<h10=reg128#6,>h10=reg128#6
# asm 2: vpxor <h20=%xmm6,<h10=%xmm5,>h10=%xmm5
vpxor % xmm6, % xmm5, % xmm5

# qhasm: h11 = h11 ^ h20
# asm 1: vpxor <h20=reg128#7,<h11=reg128#2,>h11=reg128#2
# asm 2: vpxor <h20=%xmm6,<h11=%xmm1,>h11=%xmm1
vpxor % xmm6, % xmm1, % xmm1

# qhasm: h19 = mem128[ ptr + 400 ]
# asm 1: movdqu   400(<ptr=int64#5),>h19=reg128#7
# asm 2: movdqu   400(<ptr=%r8),>h19=%xmm6
movdqu   400( % r8), % xmm6

# qhasm: h6 = h19
# asm 1: movdqa <h19=reg128#7,>h6=reg128#11
# asm 2: movdqa <h19=%xmm6,>h6=%xmm10
movdqa % xmm6, % xmm10

# qhasm: h7 = h7 ^ h19
# asm 1: vpxor <h19=reg128#7,<h7=reg128#10,>h7=reg128#10
# asm 2: vpxor <h19=%xmm6,<h7=%xmm9,>h7=%xmm9
vpxor % xmm6, % xmm9, % xmm9

# qhasm: h9 = h9 ^ h19
# asm 1: vpxor <h19=reg128#7,<h9=reg128#8,>h9=reg128#8
# asm 2: vpxor <h19=%xmm6,<h9=%xmm7,>h9=%xmm7
vpxor % xmm6, % xmm7, % xmm7

# qhasm: h10 = h10 ^ h19
# asm 1: vpxor <h19=reg128#7,<h10=reg128#6,>h10=reg128#6
# asm 2: vpxor <h19=%xmm6,<h10=%xmm5,>h10=%xmm5
vpxor % xmm6, % xmm5, % xmm5

# qhasm: h18 = mem128[ ptr + 368 ]
# asm 1: movdqu   368(<ptr=int64#5),>h18=reg128#7
# asm 2: movdqu   368(<ptr=%r8),>h18=%xmm6
movdqu   368( % r8), % xmm6

# qhasm: h18 = h18 ^ mem128[ ptr + 576 ]
# asm 1: vpxor 576(<ptr=int64#5),<h18=reg128#7,>h18=reg128#7
# asm 2: vpxor 576(<ptr=%r8),<h18=%xmm6,>h18=%xmm6
vpxor 576( % r8), % xmm6, % xmm6

# qhasm: h5 = h18
# asm 1: movdqa <h18=reg128#7,>h5=reg128#12
# asm 2: movdqa <h18=%xmm6,>h5=%xmm11
movdqa % xmm6, % xmm11

# qhasm: h6 = h6 ^ h18
# asm 1: vpxor <h18=reg128#7,<h6=reg128#11,>h6=reg128#11
# asm 2: vpxor <h18=%xmm6,<h6=%xmm10,>h6=%xmm10
vpxor % xmm6, % xmm10, % xmm10

# qhasm: h8 = h8 ^ h18
# asm 1: vpxor <h18=reg128#7,<h8=reg128#9,>h8=reg128#9
# asm 2: vpxor <h18=%xmm6,<h8=%xmm8,>h8=%xmm8
vpxor % xmm6, % xmm8, % xmm8

# qhasm: h9 = h9 ^ h18
# asm 1: vpxor <h18=reg128#7,<h9=reg128#8,>h9=reg128#7
# asm 2: vpxor <h18=%xmm6,<h9=%xmm7,>h9=%xmm6
vpxor % xmm6, % xmm7, % xmm6

# qhasm: h17 = mem128[ ptr + 336 ]
# asm 1: movdqu   336(<ptr=int64#5),>h17=reg128#8
# asm 2: movdqu   336(<ptr=%r8),>h17=%xmm7
movdqu   336( % r8), % xmm7

# qhasm: h17 = h17 ^ mem128[ ptr + 544 ]
# asm 1: vpxor 544(<ptr=int64#5),<h17=reg128#8,>h17=reg128#8
# asm 2: vpxor 544(<ptr=%r8),<h17=%xmm7,>h17=%xmm7
vpxor 544( % r8), % xmm7, % xmm7

# qhasm: h4 = h17
# asm 1: movdqa <h17=reg128#8,>h4=reg128#13
# asm 2: movdqa <h17=%xmm7,>h4=%xmm12
movdqa % xmm7, % xmm12

# qhasm: h5 = h5 ^ h17
# asm 1: vpxor <h17=reg128#8,<h5=reg128#12,>h5=reg128#12
# asm 2: vpxor <h17=%xmm7,<h5=%xmm11,>h5=%xmm11
vpxor % xmm7, % xmm11, % xmm11

# qhasm: h7 = h7 ^ h17
# asm 1: vpxor <h17=reg128#8,<h7=reg128#10,>h7=reg128#10
# asm 2: vpxor <h17=%xmm7,<h7=%xmm9,>h7=%xmm9
vpxor % xmm7, % xmm9, % xmm9

# qhasm: h8 = h8 ^ h17
# asm 1: vpxor <h17=reg128#8,<h8=reg128#9,>h8=reg128#8
# asm 2: vpxor <h17=%xmm7,<h8=%xmm8,>h8=%xmm7
vpxor % xmm7, % xmm8, % xmm7

# qhasm: h16 = mem128[ ptr + 304 ]
# asm 1: movdqu   304(<ptr=int64#5),>h16=reg128#9
# asm 2: movdqu   304(<ptr=%r8),>h16=%xmm8
movdqu   304( % r8), % xmm8

# qhasm: h16 = h16 ^ mem128[ ptr + 512 ]
# asm 1: vpxor 512(<ptr=int64#5),<h16=reg128#9,>h16=reg128#9
# asm 2: vpxor 512(<ptr=%r8),<h16=%xmm8,>h16=%xmm8
vpxor 512( % r8), % xmm8, % xmm8

# qhasm: h3 = h16
# asm 1: movdqa <h16=reg128#9,>h3=reg128#14
# asm 2: movdqa <h16=%xmm8,>h3=%xmm13
movdqa % xmm8, % xmm13

# qhasm: h4 = h4 ^ h16
# asm 1: vpxor <h16=reg128#9,<h4=reg128#13,>h4=reg128#13
# asm 2: vpxor <h16=%xmm8,<h4=%xmm12,>h4=%xmm12
vpxor % xmm8, % xmm12, % xmm12

# qhasm: h6 = h6 ^ h16
# asm 1: vpxor <h16=reg128#9,<h6=reg128#11,>h6=reg128#11
# asm 2: vpxor <h16=%xmm8,<h6=%xmm10,>h6=%xmm10
vpxor % xmm8, % xmm10, % xmm10

# qhasm: h7 = h7 ^ h16
# asm 1: vpxor <h16=reg128#9,<h7=reg128#10,>h7=reg128#9
# asm 2: vpxor <h16=%xmm8,<h7=%xmm9,>h7=%xmm8
vpxor % xmm8, % xmm9, % xmm8

# qhasm: h15 = h15 ^ mem128[ ptr + 272 ]
# asm 1: vpxor 272(<ptr=int64#5),<h15=reg128#1,>h15=reg128#1
# asm 2: vpxor 272(<ptr=%r8),<h15=%xmm0,>h15=%xmm0
vpxor 272( % r8), % xmm0, % xmm0

# qhasm: h15 = h15 ^ mem128[ ptr + 480 ]
# asm 1: vpxor 480(<ptr=int64#5),<h15=reg128#1,>h15=reg128#1
# asm 2: vpxor 480(<ptr=%r8),<h15=%xmm0,>h15=%xmm0
vpxor 480( % r8), % xmm0, % xmm0

# qhasm: h2 = h15
# asm 1: movdqa <h15=reg128#1,>h2=reg128#10
# asm 2: movdqa <h15=%xmm0,>h2=%xmm9
movdqa % xmm0, % xmm9

# qhasm: h3 = h3 ^ h15
# asm 1: vpxor <h15=reg128#1,<h3=reg128#14,>h3=reg128#14
# asm 2: vpxor <h15=%xmm0,<h3=%xmm13,>h3=%xmm13
vpxor % xmm0, % xmm13, % xmm13

# qhasm: h5 = h5 ^ h15
# asm 1: vpxor <h15=reg128#1,<h5=reg128#12,>h5=reg128#12
# asm 2: vpxor <h15=%xmm0,<h5=%xmm11,>h5=%xmm11
vpxor % xmm0, % xmm11, % xmm11

# qhasm: h6 = h6 ^ h15
# asm 1: vpxor <h15=reg128#1,<h6=reg128#11,>h6=reg128#1
# asm 2: vpxor <h15=%xmm0,<h6=%xmm10,>h6=%xmm0
vpxor % xmm0, % xmm10, % xmm0

# qhasm: h14 = h14 ^ mem128[ ptr + 240 ]
# asm 1: vpxor 240(<ptr=int64#5),<h14=reg128#4,>h14=reg128#4
# asm 2: vpxor 240(<ptr=%r8),<h14=%xmm3,>h14=%xmm3
vpxor 240( % r8), % xmm3, % xmm3

# qhasm: h14 = h14 ^ mem128[ ptr + 448 ]
# asm 1: vpxor 448(<ptr=int64#5),<h14=reg128#4,>h14=reg128#4
# asm 2: vpxor 448(<ptr=%r8),<h14=%xmm3,>h14=%xmm3
vpxor 448( % r8), % xmm3, % xmm3

# qhasm: h1 = h14
# asm 1: movdqa <h14=reg128#4,>h1=reg128#11
# asm 2: movdqa <h14=%xmm3,>h1=%xmm10
movdqa % xmm3, % xmm10

# qhasm: h2 = h2 ^ h14
# asm 1: vpxor <h14=reg128#4,<h2=reg128#10,>h2=reg128#10
# asm 2: vpxor <h14=%xmm3,<h2=%xmm9,>h2=%xmm9
vpxor % xmm3, % xmm9, % xmm9

# qhasm: h4 = h4 ^ h14
# asm 1: vpxor <h14=reg128#4,<h4=reg128#13,>h4=reg128#13
# asm 2: vpxor <h14=%xmm3,<h4=%xmm12,>h4=%xmm12
vpxor % xmm3, % xmm12, % xmm12

# qhasm: h5 = h5 ^ h14
# asm 1: vpxor <h14=reg128#4,<h5=reg128#12,>h5=reg128#4
# asm 2: vpxor <h14=%xmm3,<h5=%xmm11,>h5=%xmm3
vpxor % xmm3, % xmm11, % xmm3

# qhasm: h13 = h13 ^ mem128[ ptr + 208 ]
# asm 1: vpxor 208(<ptr=int64#5),<h13=reg128#5,>h13=reg128#5
# asm 2: vpxor 208(<ptr=%r8),<h13=%xmm4,>h13=%xmm4
vpxor 208( % r8), % xmm4, % xmm4

# qhasm: h13 = h13 ^ mem128[ ptr + 416 ]
# asm 1: vpxor 416(<ptr=int64#5),<h13=reg128#5,>h13=reg128#5
# asm 2: vpxor 416(<ptr=%r8),<h13=%xmm4,>h13=%xmm4
vpxor 416( % r8), % xmm4, % xmm4

# qhasm: h0 = h13
# asm 1: movdqa <h13=reg128#5,>h0=reg128#12
# asm 2: movdqa <h13=%xmm4,>h0=%xmm11
movdqa % xmm4, % xmm11

# qhasm: h1 = h1 ^ h13
# asm 1: vpxor <h13=reg128#5,<h1=reg128#11,>h1=reg128#11
# asm 2: vpxor <h13=%xmm4,<h1=%xmm10,>h1=%xmm10
vpxor % xmm4, % xmm10, % xmm10

# qhasm: h3 = h3 ^ h13
# asm 1: vpxor <h13=reg128#5,<h3=reg128#14,>h3=reg128#14
# asm 2: vpxor <h13=%xmm4,<h3=%xmm13,>h3=%xmm13
vpxor % xmm4, % xmm13, % xmm13

# qhasm: h4 = h4 ^ h13
# asm 1: vpxor <h13=reg128#5,<h4=reg128#13,>h4=reg128#5
# asm 2: vpxor <h13=%xmm4,<h4=%xmm12,>h4=%xmm4
vpxor % xmm4, % xmm12, % xmm4

# qhasm: h12 = h12 ^ mem128[ ptr + 384 ]
# asm 1: vpxor 384(<ptr=int64#5),<h12=reg128#3,>h12=reg128#3
# asm 2: vpxor 384(<ptr=%r8),<h12=%xmm2,>h12=%xmm2
vpxor 384( % r8), % xmm2, % xmm2

# qhasm: h12 = h12 ^ mem128[ ptr + 176 ]
# asm 1: vpxor 176(<ptr=int64#5),<h12=reg128#3,>h12=reg128#3
# asm 2: vpxor 176(<ptr=%r8),<h12=%xmm2,>h12=%xmm2
vpxor 176( % r8), % xmm2, % xmm2

# qhasm: mem128[ input_0 + 192 ] = h12
# asm 1: movdqu   <h12=reg128#3,192(<input_0=int64#1)
# asm 2: movdqu   <h12=%xmm2,192(<input_0=%rdi)
movdqu   % xmm2, 192( % rdi)

# qhasm: h11 = h11 ^ mem128[ ptr + 352 ]
# asm 1: vpxor 352(<ptr=int64#5),<h11=reg128#2,>h11=reg128#2
# asm 2: vpxor 352(<ptr=%r8),<h11=%xmm1,>h11=%xmm1
vpxor 352( % r8), % xmm1, % xmm1

# qhasm: h11 = h11 ^ mem128[ ptr + 144 ]
# asm 1: vpxor 144(<ptr=int64#5),<h11=reg128#2,>h11=reg128#2
# asm 2: vpxor 144(<ptr=%r8),<h11=%xmm1,>h11=%xmm1
vpxor 144( % r8), % xmm1, % xmm1

# qhasm: mem128[ input_0 + 176 ] = h11
# asm 1: movdqu   <h11=reg128#2,176(<input_0=int64#1)
# asm 2: movdqu   <h11=%xmm1,176(<input_0=%rdi)
movdqu   % xmm1, 176( % rdi)

# qhasm: h10 = h10 ^ mem128[ ptr + 320 ]
# asm 1: vpxor 320(<ptr=int64#5),<h10=reg128#6,>h10=reg128#2
# asm 2: vpxor 320(<ptr=%r8),<h10=%xmm5,>h10=%xmm1
vpxor 320( % r8), % xmm5, % xmm1

# qhasm: h10 = h10 ^ mem128[ ptr + 112 ]
# asm 1: vpxor 112(<ptr=int64#5),<h10=reg128#2,>h10=reg128#2
# asm 2: vpxor 112(<ptr=%r8),<h10=%xmm1,>h10=%xmm1
vpxor 112( % r8), % xmm1, % xmm1

# qhasm: mem128[ input_0 + 160 ] = h10
# asm 1: movdqu   <h10=reg128#2,160(<input_0=int64#1)
# asm 2: movdqu   <h10=%xmm1,160(<input_0=%rdi)
movdqu   % xmm1, 160( % rdi)

# qhasm: h9 = h9 ^ mem128[ ptr + 288 ]
# asm 1: vpxor 288(<ptr=int64#5),<h9=reg128#7,>h9=reg128#2
# asm 2: vpxor 288(<ptr=%r8),<h9=%xmm6,>h9=%xmm1
vpxor 288( % r8), % xmm6, % xmm1

# qhasm: h9 = h9 ^ mem128[ ptr + 80 ]
# asm 1: vpxor 80(<ptr=int64#5),<h9=reg128#2,>h9=reg128#2
# asm 2: vpxor 80(<ptr=%r8),<h9=%xmm1,>h9=%xmm1
vpxor 80( % r8), % xmm1, % xmm1

# qhasm: mem128[ input_0 + 144 ] = h9
# asm 1: movdqu   <h9=reg128#2,144(<input_0=int64#1)
# asm 2: movdqu   <h9=%xmm1,144(<input_0=%rdi)
movdqu   % xmm1, 144( % rdi)

# qhasm: h8 = h8 ^ mem128[ ptr + 256 ]
# asm 1: vpxor 256(<ptr=int64#5),<h8=reg128#8,>h8=reg128#2
# asm 2: vpxor 256(<ptr=%r8),<h8=%xmm7,>h8=%xmm1
vpxor 256( % r8), % xmm7, % xmm1

# qhasm: h8 = h8 ^ mem128[ ptr + 48 ]
# asm 1: vpxor 48(<ptr=int64#5),<h8=reg128#2,>h8=reg128#2
# asm 2: vpxor 48(<ptr=%r8),<h8=%xmm1,>h8=%xmm1
vpxor 48( % r8), % xmm1, % xmm1

# qhasm: mem128[ input_0 + 128 ] = h8
# asm 1: movdqu   <h8=reg128#2,128(<input_0=int64#1)
# asm 2: movdqu   <h8=%xmm1,128(<input_0=%rdi)
movdqu   % xmm1, 128( % rdi)

# qhasm: h7 = h7 ^ mem128[ ptr + 224 ]
# asm 1: vpxor 224(<ptr=int64#5),<h7=reg128#9,>h7=reg128#2
# asm 2: vpxor 224(<ptr=%r8),<h7=%xmm8,>h7=%xmm1
vpxor 224( % r8), % xmm8, % xmm1

# qhasm: h7 = h7 ^ mem128[ ptr + 16 ]
# asm 1: vpxor 16(<ptr=int64#5),<h7=reg128#2,>h7=reg128#2
# asm 2: vpxor 16(<ptr=%r8),<h7=%xmm1,>h7=%xmm1
vpxor 16( % r8), % xmm1, % xmm1

# qhasm: mem128[ input_0 + 112 ] = h7
# asm 1: movdqu   <h7=reg128#2,112(<input_0=int64#1)
# asm 2: movdqu   <h7=%xmm1,112(<input_0=%rdi)
movdqu   % xmm1, 112( % rdi)

# qhasm: h6 = h6 ^ mem128[ ptr + 192 ]
# asm 1: vpxor 192(<ptr=int64#5),<h6=reg128#1,>h6=reg128#1
# asm 2: vpxor 192(<ptr=%r8),<h6=%xmm0,>h6=%xmm0
vpxor 192( % r8), % xmm0, % xmm0

# qhasm: mem128[ input_0 + 96 ] = h6
# asm 1: movdqu   <h6=reg128#1,96(<input_0=int64#1)
# asm 2: movdqu   <h6=%xmm0,96(<input_0=%rdi)
movdqu   % xmm0, 96( % rdi)

# qhasm: h5 = h5 ^ mem128[ ptr + 160 ]
# asm 1: vpxor 160(<ptr=int64#5),<h5=reg128#4,>h5=reg128#1
# asm 2: vpxor 160(<ptr=%r8),<h5=%xmm3,>h5=%xmm0
vpxor 160( % r8), % xmm3, % xmm0

# qhasm: mem128[ input_0 + 80 ] = h5
# asm 1: movdqu   <h5=reg128#1,80(<input_0=int64#1)
# asm 2: movdqu   <h5=%xmm0,80(<input_0=%rdi)
movdqu   % xmm0, 80( % rdi)

# qhasm: h4 = h4 ^ mem128[ ptr + 128 ]
# asm 1: vpxor 128(<ptr=int64#5),<h4=reg128#5,>h4=reg128#1
# asm 2: vpxor 128(<ptr=%r8),<h4=%xmm4,>h4=%xmm0
vpxor 128( % r8), % xmm4, % xmm0

# qhasm: mem128[ input_0 + 64 ] = h4
# asm 1: movdqu   <h4=reg128#1,64(<input_0=int64#1)
# asm 2: movdqu   <h4=%xmm0,64(<input_0=%rdi)
movdqu   % xmm0, 64( % rdi)

# qhasm: h3 = h3 ^ mem128[ ptr + 96 ]
# asm 1: vpxor 96(<ptr=int64#5),<h3=reg128#14,>h3=reg128#1
# asm 2: vpxor 96(<ptr=%r8),<h3=%xmm13,>h3=%xmm0
vpxor 96( % r8), % xmm13, % xmm0

# qhasm: mem128[ input_0 + 48 ] = h3
# asm 1: movdqu   <h3=reg128#1,48(<input_0=int64#1)
# asm 2: movdqu   <h3=%xmm0,48(<input_0=%rdi)
movdqu   % xmm0, 48( % rdi)

# qhasm: h2 = h2 ^ mem128[ ptr + 64 ]
# asm 1: vpxor 64(<ptr=int64#5),<h2=reg128#10,>h2=reg128#1
# asm 2: vpxor 64(<ptr=%r8),<h2=%xmm9,>h2=%xmm0
vpxor 64( % r8), % xmm9, % xmm0

# qhasm: mem128[ input_0 + 32 ] = h2
# asm 1: movdqu   <h2=reg128#1,32(<input_0=int64#1)
# asm 2: movdqu   <h2=%xmm0,32(<input_0=%rdi)
movdqu   % xmm0, 32( % rdi)

# qhasm: h1 = h1 ^ mem128[ ptr + 32 ]
# asm 1: vpxor 32(<ptr=int64#5),<h1=reg128#11,>h1=reg128#1
# asm 2: vpxor 32(<ptr=%r8),<h1=%xmm10,>h1=%xmm0
vpxor 32( % r8), % xmm10, % xmm0

# qhasm: mem128[ input_0 + 16 ] = h1
# asm 1: movdqu   <h1=reg128#1,16(<input_0=int64#1)
# asm 2: movdqu   <h1=%xmm0,16(<input_0=%rdi)
movdqu   % xmm0, 16( % rdi)

# qhasm: h0 = h0 ^ mem128[ ptr + 0 ]
# asm 1: vpxor 0(<ptr=int64#5),<h0=reg128#12,>h0=reg128#1
# asm 2: vpxor 0(<ptr=%r8),<h0=%xmm11,>h0=%xmm0
vpxor 0( % r8), % xmm11, % xmm0

# qhasm: mem128[ input_0 + 0 ] = h0
# asm 1: movdqu   <h0=reg128#1,0(<input_0=int64#1)
# asm 2: movdqu   <h0=%xmm0,0(<input_0=%rdi)
movdqu   % xmm0, 0( % rdi)

# qhasm: return
add % r11, % rsp
ret
