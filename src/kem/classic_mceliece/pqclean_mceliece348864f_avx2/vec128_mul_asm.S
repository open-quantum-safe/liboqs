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

# qhasm: reg256 a0

# qhasm: reg256 a1

# qhasm: reg256 a2

# qhasm: reg256 a3

# qhasm: reg256 a4

# qhasm: reg256 a5

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
# asm 1: leaq <buf=stack4864#1,>ptr=int64#4
# asm 2: leaq <buf=0(%rsp),>ptr=%rcx
leaq 0( % rsp), % rcx

# qhasm: b11 = mem128[ input_2 + 176 ] x2
# asm 1: vbroadcasti128 176(<input_2=int64#3), >b11=reg256#1
# asm 2: vbroadcasti128 176(<input_2=%rdx), >b11=%ymm0
vbroadcasti128 176( % rdx), % ymm0

# qhasm: a5[0] = mem128[ input_1 + 80 ]
# asm 1: vinsertf128 $0x0,80(<input_1=int64#2),<a5=reg256#2,<a5=reg256#2
# asm 2: vinsertf128 $0x0,80(<input_1=%rsi),<a5=%ymm1,<a5=%ymm1
vinsertf128 $0x0, 80( % rsi), % ymm1, % ymm1

# qhasm: a5[1] = mem128[ input_1 + 176 ]
# asm 1: vinsertf128 $0x1,176(<input_1=int64#2),<a5=reg256#2,<a5=reg256#2
# asm 2: vinsertf128 $0x1,176(<input_1=%rsi),<a5=%ymm1,<a5=%ymm1
vinsertf128 $0x1, 176( % rsi), % ymm1, % ymm1

# qhasm: r16 = b11 & a5
# asm 1: vpand <b11=reg256#1,<a5=reg256#2,>r16=reg256#3
# asm 2: vpand <b11=%ymm0,<a5=%ymm1,>r16=%ymm2
vpand % ymm0, % ymm1, % ymm2

# qhasm: mem256[ ptr + 512 ] = r16
# asm 1: vmovupd   <r16=reg256#3,512(<ptr=int64#4)
# asm 2: vmovupd   <r16=%ymm2,512(<ptr=%rcx)
vmovupd   % ymm2, 512( % rcx)

# qhasm: a4[0] = mem128[ input_1 + 64 ]
# asm 1: vinsertf128 $0x0,64(<input_1=int64#2),<a4=reg256#3,<a4=reg256#3
# asm 2: vinsertf128 $0x0,64(<input_1=%rsi),<a4=%ymm2,<a4=%ymm2
vinsertf128 $0x0, 64( % rsi), % ymm2, % ymm2

# qhasm: a4[1] = mem128[ input_1 + 160 ]
# asm 1: vinsertf128 $0x1,160(<input_1=int64#2),<a4=reg256#3,<a4=reg256#3
# asm 2: vinsertf128 $0x1,160(<input_1=%rsi),<a4=%ymm2,<a4=%ymm2
vinsertf128 $0x1, 160( % rsi), % ymm2, % ymm2

# qhasm: r15 = b11 & a4
# asm 1: vpand <b11=reg256#1,<a4=reg256#3,>r15=reg256#4
# asm 2: vpand <b11=%ymm0,<a4=%ymm2,>r15=%ymm3
vpand % ymm0, % ymm2, % ymm3

# qhasm: a3[0] = mem128[ input_1 + 48 ]
# asm 1: vinsertf128 $0x0,48(<input_1=int64#2),<a3=reg256#5,<a3=reg256#5
# asm 2: vinsertf128 $0x0,48(<input_1=%rsi),<a3=%ymm4,<a3=%ymm4
vinsertf128 $0x0, 48( % rsi), % ymm4, % ymm4

# qhasm: a3[1] = mem128[ input_1 + 144 ]
# asm 1: vinsertf128 $0x1,144(<input_1=int64#2),<a3=reg256#5,<a3=reg256#5
# asm 2: vinsertf128 $0x1,144(<input_1=%rsi),<a3=%ymm4,<a3=%ymm4
vinsertf128 $0x1, 144( % rsi), % ymm4, % ymm4

# qhasm: r14 = b11 & a3
# asm 1: vpand <b11=reg256#1,<a3=reg256#5,>r14=reg256#6
# asm 2: vpand <b11=%ymm0,<a3=%ymm4,>r14=%ymm5
vpand % ymm0, % ymm4, % ymm5

# qhasm: a2[0] = mem128[ input_1 + 32 ]
# asm 1: vinsertf128 $0x0,32(<input_1=int64#2),<a2=reg256#7,<a2=reg256#7
# asm 2: vinsertf128 $0x0,32(<input_1=%rsi),<a2=%ymm6,<a2=%ymm6
vinsertf128 $0x0, 32( % rsi), % ymm6, % ymm6

# qhasm: a2[1] = mem128[ input_1 + 128 ]
# asm 1: vinsertf128 $0x1,128(<input_1=int64#2),<a2=reg256#7,<a2=reg256#7
# asm 2: vinsertf128 $0x1,128(<input_1=%rsi),<a2=%ymm6,<a2=%ymm6
vinsertf128 $0x1, 128( % rsi), % ymm6, % ymm6

# qhasm: r13 = b11 & a2
# asm 1: vpand <b11=reg256#1,<a2=reg256#7,>r13=reg256#8
# asm 2: vpand <b11=%ymm0,<a2=%ymm6,>r13=%ymm7
vpand % ymm0, % ymm6, % ymm7

# qhasm: a1[0] = mem128[ input_1 + 16 ]
# asm 1: vinsertf128 $0x0,16(<input_1=int64#2),<a1=reg256#9,<a1=reg256#9
# asm 2: vinsertf128 $0x0,16(<input_1=%rsi),<a1=%ymm8,<a1=%ymm8
vinsertf128 $0x0, 16( % rsi), % ymm8, % ymm8

# qhasm: a1[1] = mem128[ input_1 + 112 ]
# asm 1: vinsertf128 $0x1,112(<input_1=int64#2),<a1=reg256#9,<a1=reg256#9
# asm 2: vinsertf128 $0x1,112(<input_1=%rsi),<a1=%ymm8,<a1=%ymm8
vinsertf128 $0x1, 112( % rsi), % ymm8, % ymm8

# qhasm: r12 = b11 & a1
# asm 1: vpand <b11=reg256#1,<a1=reg256#9,>r12=reg256#10
# asm 2: vpand <b11=%ymm0,<a1=%ymm8,>r12=%ymm9
vpand % ymm0, % ymm8, % ymm9

# qhasm: a0[0] = mem128[ input_1 + 0 ]
# asm 1: vinsertf128 $0x0,0(<input_1=int64#2),<a0=reg256#11,<a0=reg256#11
# asm 2: vinsertf128 $0x0,0(<input_1=%rsi),<a0=%ymm10,<a0=%ymm10
vinsertf128 $0x0, 0( % rsi), % ymm10, % ymm10

# qhasm: a0[1] = mem128[ input_1 + 96 ]
# asm 1: vinsertf128 $0x1,96(<input_1=int64#2),<a0=reg256#11,<a0=reg256#11
# asm 2: vinsertf128 $0x1,96(<input_1=%rsi),<a0=%ymm10,<a0=%ymm10
vinsertf128 $0x1, 96( % rsi), % ymm10, % ymm10

# qhasm: r11 = b11 & a0
# asm 1: vpand <b11=reg256#1,<a0=reg256#11,>r11=reg256#1
# asm 2: vpand <b11=%ymm0,<a0=%ymm10,>r11=%ymm0
vpand % ymm0, % ymm10, % ymm0

# qhasm: b10 = mem128[ input_2 + 160 ] x2
# asm 1: vbroadcasti128 160(<input_2=int64#3), >b10=reg256#12
# asm 2: vbroadcasti128 160(<input_2=%rdx), >b10=%ymm11
vbroadcasti128 160( % rdx), % ymm11

# qhasm: r = b10 & a5
# asm 1: vpand <b10=reg256#12,<a5=reg256#2,>r=reg256#13
# asm 2: vpand <b10=%ymm11,<a5=%ymm1,>r=%ymm12
vpand % ymm11, % ymm1, % ymm12

# qhasm: r15 ^= r
# asm 1: vpxor <r=reg256#13,<r15=reg256#4,<r15=reg256#4
# asm 2: vpxor <r=%ymm12,<r15=%ymm3,<r15=%ymm3
vpxor % ymm12, % ymm3, % ymm3

# qhasm: mem256[ ptr + 480 ] = r15
# asm 1: vmovupd   <r15=reg256#4,480(<ptr=int64#4)
# asm 2: vmovupd   <r15=%ymm3,480(<ptr=%rcx)
vmovupd   % ymm3, 480( % rcx)

# qhasm: r = b10 & a4
# asm 1: vpand <b10=reg256#12,<a4=reg256#3,>r=reg256#4
# asm 2: vpand <b10=%ymm11,<a4=%ymm2,>r=%ymm3
vpand % ymm11, % ymm2, % ymm3

# qhasm: r14 ^= r
# asm 1: vpxor <r=reg256#4,<r14=reg256#6,<r14=reg256#6
# asm 2: vpxor <r=%ymm3,<r14=%ymm5,<r14=%ymm5
vpxor % ymm3, % ymm5, % ymm5

# qhasm: r = b10 & a3
# asm 1: vpand <b10=reg256#12,<a3=reg256#5,>r=reg256#4
# asm 2: vpand <b10=%ymm11,<a3=%ymm4,>r=%ymm3
vpand % ymm11, % ymm4, % ymm3

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#4,<r13=reg256#8,<r13=reg256#8
# asm 2: vpxor <r=%ymm3,<r13=%ymm7,<r13=%ymm7
vpxor % ymm3, % ymm7, % ymm7

# qhasm: r = b10 & a2
# asm 1: vpand <b10=reg256#12,<a2=reg256#7,>r=reg256#4
# asm 2: vpand <b10=%ymm11,<a2=%ymm6,>r=%ymm3
vpand % ymm11, % ymm6, % ymm3

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#4,<r12=reg256#10,<r12=reg256#10
# asm 2: vpxor <r=%ymm3,<r12=%ymm9,<r12=%ymm9
vpxor % ymm3, % ymm9, % ymm9

# qhasm: r = b10 & a1
# asm 1: vpand <b10=reg256#12,<a1=reg256#9,>r=reg256#4
# asm 2: vpand <b10=%ymm11,<a1=%ymm8,>r=%ymm3
vpand % ymm11, % ymm8, % ymm3

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#4,<r11=reg256#1,<r11=reg256#1
# asm 2: vpxor <r=%ymm3,<r11=%ymm0,<r11=%ymm0
vpxor % ymm3, % ymm0, % ymm0

# qhasm: r10 = b10 & a0
# asm 1: vpand <b10=reg256#12,<a0=reg256#11,>r10=reg256#4
# asm 2: vpand <b10=%ymm11,<a0=%ymm10,>r10=%ymm3
vpand % ymm11, % ymm10, % ymm3

# qhasm: b9 = mem128[ input_2 + 144 ] x2
# asm 1: vbroadcasti128 144(<input_2=int64#3), >b9=reg256#12
# asm 2: vbroadcasti128 144(<input_2=%rdx), >b9=%ymm11
vbroadcasti128 144( % rdx), % ymm11

# qhasm: r = b9 & a5
# asm 1: vpand <b9=reg256#12,<a5=reg256#2,>r=reg256#13
# asm 2: vpand <b9=%ymm11,<a5=%ymm1,>r=%ymm12
vpand % ymm11, % ymm1, % ymm12

# qhasm: r14 ^= r
# asm 1: vpxor <r=reg256#13,<r14=reg256#6,<r14=reg256#6
# asm 2: vpxor <r=%ymm12,<r14=%ymm5,<r14=%ymm5
vpxor % ymm12, % ymm5, % ymm5

# qhasm: mem256[ ptr + 448 ] = r14
# asm 1: vmovupd   <r14=reg256#6,448(<ptr=int64#4)
# asm 2: vmovupd   <r14=%ymm5,448(<ptr=%rcx)
vmovupd   % ymm5, 448( % rcx)

# qhasm: r = b9 & a4
# asm 1: vpand <b9=reg256#12,<a4=reg256#3,>r=reg256#6
# asm 2: vpand <b9=%ymm11,<a4=%ymm2,>r=%ymm5
vpand % ymm11, % ymm2, % ymm5

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#6,<r13=reg256#8,<r13=reg256#8
# asm 2: vpxor <r=%ymm5,<r13=%ymm7,<r13=%ymm7
vpxor % ymm5, % ymm7, % ymm7

# qhasm: r = b9 & a3
# asm 1: vpand <b9=reg256#12,<a3=reg256#5,>r=reg256#6
# asm 2: vpand <b9=%ymm11,<a3=%ymm4,>r=%ymm5
vpand % ymm11, % ymm4, % ymm5

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#6,<r12=reg256#10,<r12=reg256#10
# asm 2: vpxor <r=%ymm5,<r12=%ymm9,<r12=%ymm9
vpxor % ymm5, % ymm9, % ymm9

# qhasm: r = b9 & a2
# asm 1: vpand <b9=reg256#12,<a2=reg256#7,>r=reg256#6
# asm 2: vpand <b9=%ymm11,<a2=%ymm6,>r=%ymm5
vpand % ymm11, % ymm6, % ymm5

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#6,<r11=reg256#1,<r11=reg256#1
# asm 2: vpxor <r=%ymm5,<r11=%ymm0,<r11=%ymm0
vpxor % ymm5, % ymm0, % ymm0

# qhasm: r = b9 & a1
# asm 1: vpand <b9=reg256#12,<a1=reg256#9,>r=reg256#6
# asm 2: vpand <b9=%ymm11,<a1=%ymm8,>r=%ymm5
vpand % ymm11, % ymm8, % ymm5

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#6,<r10=reg256#4,<r10=reg256#4
# asm 2: vpxor <r=%ymm5,<r10=%ymm3,<r10=%ymm3
vpxor % ymm5, % ymm3, % ymm3

# qhasm: r9 = b9 & a0
# asm 1: vpand <b9=reg256#12,<a0=reg256#11,>r9=reg256#6
# asm 2: vpand <b9=%ymm11,<a0=%ymm10,>r9=%ymm5
vpand % ymm11, % ymm10, % ymm5

# qhasm: b8 = mem128[ input_2 + 128 ] x2
# asm 1: vbroadcasti128 128(<input_2=int64#3), >b8=reg256#12
# asm 2: vbroadcasti128 128(<input_2=%rdx), >b8=%ymm11
vbroadcasti128 128( % rdx), % ymm11

# qhasm: r = b8 & a5
# asm 1: vpand <b8=reg256#12,<a5=reg256#2,>r=reg256#13
# asm 2: vpand <b8=%ymm11,<a5=%ymm1,>r=%ymm12
vpand % ymm11, % ymm1, % ymm12

# qhasm: r13 ^= r
# asm 1: vpxor <r=reg256#13,<r13=reg256#8,<r13=reg256#8
# asm 2: vpxor <r=%ymm12,<r13=%ymm7,<r13=%ymm7
vpxor % ymm12, % ymm7, % ymm7

# qhasm: mem256[ ptr + 416 ] = r13
# asm 1: vmovupd   <r13=reg256#8,416(<ptr=int64#4)
# asm 2: vmovupd   <r13=%ymm7,416(<ptr=%rcx)
vmovupd   % ymm7, 416( % rcx)

# qhasm: r = b8 & a4
# asm 1: vpand <b8=reg256#12,<a4=reg256#3,>r=reg256#8
# asm 2: vpand <b8=%ymm11,<a4=%ymm2,>r=%ymm7
vpand % ymm11, % ymm2, % ymm7

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#8,<r12=reg256#10,<r12=reg256#10
# asm 2: vpxor <r=%ymm7,<r12=%ymm9,<r12=%ymm9
vpxor % ymm7, % ymm9, % ymm9

# qhasm: r = b8 & a3
# asm 1: vpand <b8=reg256#12,<a3=reg256#5,>r=reg256#8
# asm 2: vpand <b8=%ymm11,<a3=%ymm4,>r=%ymm7
vpand % ymm11, % ymm4, % ymm7

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#8,<r11=reg256#1,<r11=reg256#1
# asm 2: vpxor <r=%ymm7,<r11=%ymm0,<r11=%ymm0
vpxor % ymm7, % ymm0, % ymm0

# qhasm: r = b8 & a2
# asm 1: vpand <b8=reg256#12,<a2=reg256#7,>r=reg256#8
# asm 2: vpand <b8=%ymm11,<a2=%ymm6,>r=%ymm7
vpand % ymm11, % ymm6, % ymm7

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#8,<r10=reg256#4,<r10=reg256#4
# asm 2: vpxor <r=%ymm7,<r10=%ymm3,<r10=%ymm3
vpxor % ymm7, % ymm3, % ymm3

# qhasm: r = b8 & a1
# asm 1: vpand <b8=reg256#12,<a1=reg256#9,>r=reg256#8
# asm 2: vpand <b8=%ymm11,<a1=%ymm8,>r=%ymm7
vpand % ymm11, % ymm8, % ymm7

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#8,<r9=reg256#6,<r9=reg256#6
# asm 2: vpxor <r=%ymm7,<r9=%ymm5,<r9=%ymm5
vpxor % ymm7, % ymm5, % ymm5

# qhasm: r8 = b8 & a0
# asm 1: vpand <b8=reg256#12,<a0=reg256#11,>r8=reg256#8
# asm 2: vpand <b8=%ymm11,<a0=%ymm10,>r8=%ymm7
vpand % ymm11, % ymm10, % ymm7

# qhasm: b7 = mem128[ input_2 + 112 ] x2
# asm 1: vbroadcasti128 112(<input_2=int64#3), >b7=reg256#12
# asm 2: vbroadcasti128 112(<input_2=%rdx), >b7=%ymm11
vbroadcasti128 112( % rdx), % ymm11

# qhasm: r = b7 & a5
# asm 1: vpand <b7=reg256#12,<a5=reg256#2,>r=reg256#13
# asm 2: vpand <b7=%ymm11,<a5=%ymm1,>r=%ymm12
vpand % ymm11, % ymm1, % ymm12

# qhasm: r12 ^= r
# asm 1: vpxor <r=reg256#13,<r12=reg256#10,<r12=reg256#10
# asm 2: vpxor <r=%ymm12,<r12=%ymm9,<r12=%ymm9
vpxor % ymm12, % ymm9, % ymm9

# qhasm: mem256[ ptr + 384 ] = r12
# asm 1: vmovupd   <r12=reg256#10,384(<ptr=int64#4)
# asm 2: vmovupd   <r12=%ymm9,384(<ptr=%rcx)
vmovupd   % ymm9, 384( % rcx)

# qhasm: r = b7 & a4
# asm 1: vpand <b7=reg256#12,<a4=reg256#3,>r=reg256#10
# asm 2: vpand <b7=%ymm11,<a4=%ymm2,>r=%ymm9
vpand % ymm11, % ymm2, % ymm9

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#10,<r11=reg256#1,<r11=reg256#1
# asm 2: vpxor <r=%ymm9,<r11=%ymm0,<r11=%ymm0
vpxor % ymm9, % ymm0, % ymm0

# qhasm: r = b7 & a3
# asm 1: vpand <b7=reg256#12,<a3=reg256#5,>r=reg256#10
# asm 2: vpand <b7=%ymm11,<a3=%ymm4,>r=%ymm9
vpand % ymm11, % ymm4, % ymm9

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#10,<r10=reg256#4,<r10=reg256#4
# asm 2: vpxor <r=%ymm9,<r10=%ymm3,<r10=%ymm3
vpxor % ymm9, % ymm3, % ymm3

# qhasm: r = b7 & a2
# asm 1: vpand <b7=reg256#12,<a2=reg256#7,>r=reg256#10
# asm 2: vpand <b7=%ymm11,<a2=%ymm6,>r=%ymm9
vpand % ymm11, % ymm6, % ymm9

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#10,<r9=reg256#6,<r9=reg256#6
# asm 2: vpxor <r=%ymm9,<r9=%ymm5,<r9=%ymm5
vpxor % ymm9, % ymm5, % ymm5

# qhasm: r = b7 & a1
# asm 1: vpand <b7=reg256#12,<a1=reg256#9,>r=reg256#10
# asm 2: vpand <b7=%ymm11,<a1=%ymm8,>r=%ymm9
vpand % ymm11, % ymm8, % ymm9

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#10,<r8=reg256#8,<r8=reg256#8
# asm 2: vpxor <r=%ymm9,<r8=%ymm7,<r8=%ymm7
vpxor % ymm9, % ymm7, % ymm7

# qhasm: r7 = b7 & a0
# asm 1: vpand <b7=reg256#12,<a0=reg256#11,>r7=reg256#10
# asm 2: vpand <b7=%ymm11,<a0=%ymm10,>r7=%ymm9
vpand % ymm11, % ymm10, % ymm9

# qhasm: b6 = mem128[ input_2 + 96 ] x2
# asm 1: vbroadcasti128 96(<input_2=int64#3), >b6=reg256#12
# asm 2: vbroadcasti128 96(<input_2=%rdx), >b6=%ymm11
vbroadcasti128 96( % rdx), % ymm11

# qhasm: r = b6 & a5
# asm 1: vpand <b6=reg256#12,<a5=reg256#2,>r=reg256#13
# asm 2: vpand <b6=%ymm11,<a5=%ymm1,>r=%ymm12
vpand % ymm11, % ymm1, % ymm12

# qhasm: r11 ^= r
# asm 1: vpxor <r=reg256#13,<r11=reg256#1,<r11=reg256#1
# asm 2: vpxor <r=%ymm12,<r11=%ymm0,<r11=%ymm0
vpxor % ymm12, % ymm0, % ymm0

# qhasm: mem256[ ptr + 352 ] = r11
# asm 1: vmovupd   <r11=reg256#1,352(<ptr=int64#4)
# asm 2: vmovupd   <r11=%ymm0,352(<ptr=%rcx)
vmovupd   % ymm0, 352( % rcx)

# qhasm: r = b6 & a4
# asm 1: vpand <b6=reg256#12,<a4=reg256#3,>r=reg256#1
# asm 2: vpand <b6=%ymm11,<a4=%ymm2,>r=%ymm0
vpand % ymm11, % ymm2, % ymm0

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#1,<r10=reg256#4,<r10=reg256#4
# asm 2: vpxor <r=%ymm0,<r10=%ymm3,<r10=%ymm3
vpxor % ymm0, % ymm3, % ymm3

# qhasm: r = b6 & a3
# asm 1: vpand <b6=reg256#12,<a3=reg256#5,>r=reg256#1
# asm 2: vpand <b6=%ymm11,<a3=%ymm4,>r=%ymm0
vpand % ymm11, % ymm4, % ymm0

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#1,<r9=reg256#6,<r9=reg256#6
# asm 2: vpxor <r=%ymm0,<r9=%ymm5,<r9=%ymm5
vpxor % ymm0, % ymm5, % ymm5

# qhasm: r = b6 & a2
# asm 1: vpand <b6=reg256#12,<a2=reg256#7,>r=reg256#1
# asm 2: vpand <b6=%ymm11,<a2=%ymm6,>r=%ymm0
vpand % ymm11, % ymm6, % ymm0

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#1,<r8=reg256#8,<r8=reg256#8
# asm 2: vpxor <r=%ymm0,<r8=%ymm7,<r8=%ymm7
vpxor % ymm0, % ymm7, % ymm7

# qhasm: r = b6 & a1
# asm 1: vpand <b6=reg256#12,<a1=reg256#9,>r=reg256#1
# asm 2: vpand <b6=%ymm11,<a1=%ymm8,>r=%ymm0
vpand % ymm11, % ymm8, % ymm0

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#1,<r7=reg256#10,<r7=reg256#10
# asm 2: vpxor <r=%ymm0,<r7=%ymm9,<r7=%ymm9
vpxor % ymm0, % ymm9, % ymm9

# qhasm: r6 = b6 & a0
# asm 1: vpand <b6=reg256#12,<a0=reg256#11,>r6=reg256#1
# asm 2: vpand <b6=%ymm11,<a0=%ymm10,>r6=%ymm0
vpand % ymm11, % ymm10, % ymm0

# qhasm: b5 = mem128[ input_2 + 80 ] x2
# asm 1: vbroadcasti128 80(<input_2=int64#3), >b5=reg256#12
# asm 2: vbroadcasti128 80(<input_2=%rdx), >b5=%ymm11
vbroadcasti128 80( % rdx), % ymm11

# qhasm: r = b5 & a5
# asm 1: vpand <b5=reg256#12,<a5=reg256#2,>r=reg256#13
# asm 2: vpand <b5=%ymm11,<a5=%ymm1,>r=%ymm12
vpand % ymm11, % ymm1, % ymm12

# qhasm: r10 ^= r
# asm 1: vpxor <r=reg256#13,<r10=reg256#4,<r10=reg256#4
# asm 2: vpxor <r=%ymm12,<r10=%ymm3,<r10=%ymm3
vpxor % ymm12, % ymm3, % ymm3

# qhasm: mem256[ ptr + 320 ] = r10
# asm 1: vmovupd   <r10=reg256#4,320(<ptr=int64#4)
# asm 2: vmovupd   <r10=%ymm3,320(<ptr=%rcx)
vmovupd   % ymm3, 320( % rcx)

# qhasm: r = b5 & a4
# asm 1: vpand <b5=reg256#12,<a4=reg256#3,>r=reg256#4
# asm 2: vpand <b5=%ymm11,<a4=%ymm2,>r=%ymm3
vpand % ymm11, % ymm2, % ymm3

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#4,<r9=reg256#6,<r9=reg256#6
# asm 2: vpxor <r=%ymm3,<r9=%ymm5,<r9=%ymm5
vpxor % ymm3, % ymm5, % ymm5

# qhasm: r = b5 & a3
# asm 1: vpand <b5=reg256#12,<a3=reg256#5,>r=reg256#4
# asm 2: vpand <b5=%ymm11,<a3=%ymm4,>r=%ymm3
vpand % ymm11, % ymm4, % ymm3

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#4,<r8=reg256#8,<r8=reg256#8
# asm 2: vpxor <r=%ymm3,<r8=%ymm7,<r8=%ymm7
vpxor % ymm3, % ymm7, % ymm7

# qhasm: r = b5 & a2
# asm 1: vpand <b5=reg256#12,<a2=reg256#7,>r=reg256#4
# asm 2: vpand <b5=%ymm11,<a2=%ymm6,>r=%ymm3
vpand % ymm11, % ymm6, % ymm3

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#4,<r7=reg256#10,<r7=reg256#10
# asm 2: vpxor <r=%ymm3,<r7=%ymm9,<r7=%ymm9
vpxor % ymm3, % ymm9, % ymm9

# qhasm: r = b5 & a1
# asm 1: vpand <b5=reg256#12,<a1=reg256#9,>r=reg256#4
# asm 2: vpand <b5=%ymm11,<a1=%ymm8,>r=%ymm3
vpand % ymm11, % ymm8, % ymm3

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#4,<r6=reg256#1,<r6=reg256#1
# asm 2: vpxor <r=%ymm3,<r6=%ymm0,<r6=%ymm0
vpxor % ymm3, % ymm0, % ymm0

# qhasm: r5 = b5 & a0
# asm 1: vpand <b5=reg256#12,<a0=reg256#11,>r5=reg256#4
# asm 2: vpand <b5=%ymm11,<a0=%ymm10,>r5=%ymm3
vpand % ymm11, % ymm10, % ymm3

# qhasm: b4 = mem128[ input_2 + 64 ] x2
# asm 1: vbroadcasti128 64(<input_2=int64#3), >b4=reg256#12
# asm 2: vbroadcasti128 64(<input_2=%rdx), >b4=%ymm11
vbroadcasti128 64( % rdx), % ymm11

# qhasm: r = b4 & a5
# asm 1: vpand <b4=reg256#12,<a5=reg256#2,>r=reg256#13
# asm 2: vpand <b4=%ymm11,<a5=%ymm1,>r=%ymm12
vpand % ymm11, % ymm1, % ymm12

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#13,<r9=reg256#6,<r9=reg256#6
# asm 2: vpxor <r=%ymm12,<r9=%ymm5,<r9=%ymm5
vpxor % ymm12, % ymm5, % ymm5

# qhasm: mem256[ ptr + 288 ] = r9
# asm 1: vmovupd   <r9=reg256#6,288(<ptr=int64#4)
# asm 2: vmovupd   <r9=%ymm5,288(<ptr=%rcx)
vmovupd   % ymm5, 288( % rcx)

# qhasm: r = b4 & a4
# asm 1: vpand <b4=reg256#12,<a4=reg256#3,>r=reg256#6
# asm 2: vpand <b4=%ymm11,<a4=%ymm2,>r=%ymm5
vpand % ymm11, % ymm2, % ymm5

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#6,<r8=reg256#8,<r8=reg256#8
# asm 2: vpxor <r=%ymm5,<r8=%ymm7,<r8=%ymm7
vpxor % ymm5, % ymm7, % ymm7

# qhasm: r = b4 & a3
# asm 1: vpand <b4=reg256#12,<a3=reg256#5,>r=reg256#6
# asm 2: vpand <b4=%ymm11,<a3=%ymm4,>r=%ymm5
vpand % ymm11, % ymm4, % ymm5

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#6,<r7=reg256#10,<r7=reg256#10
# asm 2: vpxor <r=%ymm5,<r7=%ymm9,<r7=%ymm9
vpxor % ymm5, % ymm9, % ymm9

# qhasm: r = b4 & a2
# asm 1: vpand <b4=reg256#12,<a2=reg256#7,>r=reg256#6
# asm 2: vpand <b4=%ymm11,<a2=%ymm6,>r=%ymm5
vpand % ymm11, % ymm6, % ymm5

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#6,<r6=reg256#1,<r6=reg256#1
# asm 2: vpxor <r=%ymm5,<r6=%ymm0,<r6=%ymm0
vpxor % ymm5, % ymm0, % ymm0

# qhasm: r = b4 & a1
# asm 1: vpand <b4=reg256#12,<a1=reg256#9,>r=reg256#6
# asm 2: vpand <b4=%ymm11,<a1=%ymm8,>r=%ymm5
vpand % ymm11, % ymm8, % ymm5

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#6,<r5=reg256#4,<r5=reg256#4
# asm 2: vpxor <r=%ymm5,<r5=%ymm3,<r5=%ymm3
vpxor % ymm5, % ymm3, % ymm3

# qhasm: r4 = b4 & a0
# asm 1: vpand <b4=reg256#12,<a0=reg256#11,>r4=reg256#6
# asm 2: vpand <b4=%ymm11,<a0=%ymm10,>r4=%ymm5
vpand % ymm11, % ymm10, % ymm5

# qhasm: b3 = mem128[ input_2 + 48 ] x2
# asm 1: vbroadcasti128 48(<input_2=int64#3), >b3=reg256#12
# asm 2: vbroadcasti128 48(<input_2=%rdx), >b3=%ymm11
vbroadcasti128 48( % rdx), % ymm11

# qhasm: r = b3 & a5
# asm 1: vpand <b3=reg256#12,<a5=reg256#2,>r=reg256#13
# asm 2: vpand <b3=%ymm11,<a5=%ymm1,>r=%ymm12
vpand % ymm11, % ymm1, % ymm12

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#13,<r8=reg256#8,<r8=reg256#8
# asm 2: vpxor <r=%ymm12,<r8=%ymm7,<r8=%ymm7
vpxor % ymm12, % ymm7, % ymm7

# qhasm: mem256[ ptr + 256 ] = r8
# asm 1: vmovupd   <r8=reg256#8,256(<ptr=int64#4)
# asm 2: vmovupd   <r8=%ymm7,256(<ptr=%rcx)
vmovupd   % ymm7, 256( % rcx)

# qhasm: r = b3 & a4
# asm 1: vpand <b3=reg256#12,<a4=reg256#3,>r=reg256#8
# asm 2: vpand <b3=%ymm11,<a4=%ymm2,>r=%ymm7
vpand % ymm11, % ymm2, % ymm7

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#8,<r7=reg256#10,<r7=reg256#10
# asm 2: vpxor <r=%ymm7,<r7=%ymm9,<r7=%ymm9
vpxor % ymm7, % ymm9, % ymm9

# qhasm: r = b3 & a3
# asm 1: vpand <b3=reg256#12,<a3=reg256#5,>r=reg256#8
# asm 2: vpand <b3=%ymm11,<a3=%ymm4,>r=%ymm7
vpand % ymm11, % ymm4, % ymm7

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#8,<r6=reg256#1,<r6=reg256#1
# asm 2: vpxor <r=%ymm7,<r6=%ymm0,<r6=%ymm0
vpxor % ymm7, % ymm0, % ymm0

# qhasm: r = b3 & a2
# asm 1: vpand <b3=reg256#12,<a2=reg256#7,>r=reg256#8
# asm 2: vpand <b3=%ymm11,<a2=%ymm6,>r=%ymm7
vpand % ymm11, % ymm6, % ymm7

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#8,<r5=reg256#4,<r5=reg256#4
# asm 2: vpxor <r=%ymm7,<r5=%ymm3,<r5=%ymm3
vpxor % ymm7, % ymm3, % ymm3

# qhasm: r = b3 & a1
# asm 1: vpand <b3=reg256#12,<a1=reg256#9,>r=reg256#8
# asm 2: vpand <b3=%ymm11,<a1=%ymm8,>r=%ymm7
vpand % ymm11, % ymm8, % ymm7

# qhasm: r4 ^= r
# asm 1: vpxor <r=reg256#8,<r4=reg256#6,<r4=reg256#6
# asm 2: vpxor <r=%ymm7,<r4=%ymm5,<r4=%ymm5
vpxor % ymm7, % ymm5, % ymm5

# qhasm: r3 = b3 & a0
# asm 1: vpand <b3=reg256#12,<a0=reg256#11,>r3=reg256#8
# asm 2: vpand <b3=%ymm11,<a0=%ymm10,>r3=%ymm7
vpand % ymm11, % ymm10, % ymm7

# qhasm: b2 = mem128[ input_2 + 32 ] x2
# asm 1: vbroadcasti128 32(<input_2=int64#3), >b2=reg256#12
# asm 2: vbroadcasti128 32(<input_2=%rdx), >b2=%ymm11
vbroadcasti128 32( % rdx), % ymm11

# qhasm: r = b2 & a5
# asm 1: vpand <b2=reg256#12,<a5=reg256#2,>r=reg256#13
# asm 2: vpand <b2=%ymm11,<a5=%ymm1,>r=%ymm12
vpand % ymm11, % ymm1, % ymm12

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#13,<r7=reg256#10,<r7=reg256#10
# asm 2: vpxor <r=%ymm12,<r7=%ymm9,<r7=%ymm9
vpxor % ymm12, % ymm9, % ymm9

# qhasm: mem256[ ptr + 224 ] = r7
# asm 1: vmovupd   <r7=reg256#10,224(<ptr=int64#4)
# asm 2: vmovupd   <r7=%ymm9,224(<ptr=%rcx)
vmovupd   % ymm9, 224( % rcx)

# qhasm: r = b2 & a4
# asm 1: vpand <b2=reg256#12,<a4=reg256#3,>r=reg256#10
# asm 2: vpand <b2=%ymm11,<a4=%ymm2,>r=%ymm9
vpand % ymm11, % ymm2, % ymm9

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#10,<r6=reg256#1,<r6=reg256#1
# asm 2: vpxor <r=%ymm9,<r6=%ymm0,<r6=%ymm0
vpxor % ymm9, % ymm0, % ymm0

# qhasm: r = b2 & a3
# asm 1: vpand <b2=reg256#12,<a3=reg256#5,>r=reg256#10
# asm 2: vpand <b2=%ymm11,<a3=%ymm4,>r=%ymm9
vpand % ymm11, % ymm4, % ymm9

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#10,<r5=reg256#4,<r5=reg256#4
# asm 2: vpxor <r=%ymm9,<r5=%ymm3,<r5=%ymm3
vpxor % ymm9, % ymm3, % ymm3

# qhasm: r = b2 & a2
# asm 1: vpand <b2=reg256#12,<a2=reg256#7,>r=reg256#10
# asm 2: vpand <b2=%ymm11,<a2=%ymm6,>r=%ymm9
vpand % ymm11, % ymm6, % ymm9

# qhasm: r4 ^= r
# asm 1: vpxor <r=reg256#10,<r4=reg256#6,<r4=reg256#6
# asm 2: vpxor <r=%ymm9,<r4=%ymm5,<r4=%ymm5
vpxor % ymm9, % ymm5, % ymm5

# qhasm: r = b2 & a1
# asm 1: vpand <b2=reg256#12,<a1=reg256#9,>r=reg256#10
# asm 2: vpand <b2=%ymm11,<a1=%ymm8,>r=%ymm9
vpand % ymm11, % ymm8, % ymm9

# qhasm: r3 ^= r
# asm 1: vpxor <r=reg256#10,<r3=reg256#8,<r3=reg256#8
# asm 2: vpxor <r=%ymm9,<r3=%ymm7,<r3=%ymm7
vpxor % ymm9, % ymm7, % ymm7

# qhasm: r2 = b2 & a0
# asm 1: vpand <b2=reg256#12,<a0=reg256#11,>r2=reg256#10
# asm 2: vpand <b2=%ymm11,<a0=%ymm10,>r2=%ymm9
vpand % ymm11, % ymm10, % ymm9

# qhasm: b1 = mem128[ input_2 + 16 ] x2
# asm 1: vbroadcasti128 16(<input_2=int64#3), >b1=reg256#12
# asm 2: vbroadcasti128 16(<input_2=%rdx), >b1=%ymm11
vbroadcasti128 16( % rdx), % ymm11

# qhasm: r = b1 & a5
# asm 1: vpand <b1=reg256#12,<a5=reg256#2,>r=reg256#13
# asm 2: vpand <b1=%ymm11,<a5=%ymm1,>r=%ymm12
vpand % ymm11, % ymm1, % ymm12

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#13,<r6=reg256#1,<r6=reg256#1
# asm 2: vpxor <r=%ymm12,<r6=%ymm0,<r6=%ymm0
vpxor % ymm12, % ymm0, % ymm0

# qhasm: mem256[ ptr + 192 ] = r6
# asm 1: vmovupd   <r6=reg256#1,192(<ptr=int64#4)
# asm 2: vmovupd   <r6=%ymm0,192(<ptr=%rcx)
vmovupd   % ymm0, 192( % rcx)

# qhasm: r = b1 & a4
# asm 1: vpand <b1=reg256#12,<a4=reg256#3,>r=reg256#1
# asm 2: vpand <b1=%ymm11,<a4=%ymm2,>r=%ymm0
vpand % ymm11, % ymm2, % ymm0

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#1,<r5=reg256#4,<r5=reg256#4
# asm 2: vpxor <r=%ymm0,<r5=%ymm3,<r5=%ymm3
vpxor % ymm0, % ymm3, % ymm3

# qhasm: r = b1 & a3
# asm 1: vpand <b1=reg256#12,<a3=reg256#5,>r=reg256#1
# asm 2: vpand <b1=%ymm11,<a3=%ymm4,>r=%ymm0
vpand % ymm11, % ymm4, % ymm0

# qhasm: r4 ^= r
# asm 1: vpxor <r=reg256#1,<r4=reg256#6,<r4=reg256#6
# asm 2: vpxor <r=%ymm0,<r4=%ymm5,<r4=%ymm5
vpxor % ymm0, % ymm5, % ymm5

# qhasm: r = b1 & a2
# asm 1: vpand <b1=reg256#12,<a2=reg256#7,>r=reg256#1
# asm 2: vpand <b1=%ymm11,<a2=%ymm6,>r=%ymm0
vpand % ymm11, % ymm6, % ymm0

# qhasm: r3 ^= r
# asm 1: vpxor <r=reg256#1,<r3=reg256#8,<r3=reg256#8
# asm 2: vpxor <r=%ymm0,<r3=%ymm7,<r3=%ymm7
vpxor % ymm0, % ymm7, % ymm7

# qhasm: r = b1 & a1
# asm 1: vpand <b1=reg256#12,<a1=reg256#9,>r=reg256#1
# asm 2: vpand <b1=%ymm11,<a1=%ymm8,>r=%ymm0
vpand % ymm11, % ymm8, % ymm0

# qhasm: r2 ^= r
# asm 1: vpxor <r=reg256#1,<r2=reg256#10,<r2=reg256#10
# asm 2: vpxor <r=%ymm0,<r2=%ymm9,<r2=%ymm9
vpxor % ymm0, % ymm9, % ymm9

# qhasm: r1 = b1 & a0
# asm 1: vpand <b1=reg256#12,<a0=reg256#11,>r1=reg256#1
# asm 2: vpand <b1=%ymm11,<a0=%ymm10,>r1=%ymm0
vpand % ymm11, % ymm10, % ymm0

# qhasm: b0 = mem128[ input_2 + 0 ] x2
# asm 1: vbroadcasti128 0(<input_2=int64#3), >b0=reg256#12
# asm 2: vbroadcasti128 0(<input_2=%rdx), >b0=%ymm11
vbroadcasti128 0( % rdx), % ymm11

# qhasm: r = b0 & a5
# asm 1: vpand <b0=reg256#12,<a5=reg256#2,>r=reg256#2
# asm 2: vpand <b0=%ymm11,<a5=%ymm1,>r=%ymm1
vpand % ymm11, % ymm1, % ymm1

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#2,<r5=reg256#4,<r5=reg256#4
# asm 2: vpxor <r=%ymm1,<r5=%ymm3,<r5=%ymm3
vpxor % ymm1, % ymm3, % ymm3

# qhasm: mem256[ ptr + 160 ] = r5
# asm 1: vmovupd   <r5=reg256#4,160(<ptr=int64#4)
# asm 2: vmovupd   <r5=%ymm3,160(<ptr=%rcx)
vmovupd   % ymm3, 160( % rcx)

# qhasm: r = b0 & a4
# asm 1: vpand <b0=reg256#12,<a4=reg256#3,>r=reg256#2
# asm 2: vpand <b0=%ymm11,<a4=%ymm2,>r=%ymm1
vpand % ymm11, % ymm2, % ymm1

# qhasm: r4 ^= r
# asm 1: vpxor <r=reg256#2,<r4=reg256#6,<r4=reg256#6
# asm 2: vpxor <r=%ymm1,<r4=%ymm5,<r4=%ymm5
vpxor % ymm1, % ymm5, % ymm5

# qhasm: r = b0 & a3
# asm 1: vpand <b0=reg256#12,<a3=reg256#5,>r=reg256#2
# asm 2: vpand <b0=%ymm11,<a3=%ymm4,>r=%ymm1
vpand % ymm11, % ymm4, % ymm1

# qhasm: r3 ^= r
# asm 1: vpxor <r=reg256#2,<r3=reg256#8,<r3=reg256#8
# asm 2: vpxor <r=%ymm1,<r3=%ymm7,<r3=%ymm7
vpxor % ymm1, % ymm7, % ymm7

# qhasm: r = b0 & a2
# asm 1: vpand <b0=reg256#12,<a2=reg256#7,>r=reg256#2
# asm 2: vpand <b0=%ymm11,<a2=%ymm6,>r=%ymm1
vpand % ymm11, % ymm6, % ymm1

# qhasm: r2 ^= r
# asm 1: vpxor <r=reg256#2,<r2=reg256#10,<r2=reg256#10
# asm 2: vpxor <r=%ymm1,<r2=%ymm9,<r2=%ymm9
vpxor % ymm1, % ymm9, % ymm9

# qhasm: r = b0 & a1
# asm 1: vpand <b0=reg256#12,<a1=reg256#9,>r=reg256#2
# asm 2: vpand <b0=%ymm11,<a1=%ymm8,>r=%ymm1
vpand % ymm11, % ymm8, % ymm1

# qhasm: r1 ^= r
# asm 1: vpxor <r=reg256#2,<r1=reg256#1,<r1=reg256#1
# asm 2: vpxor <r=%ymm1,<r1=%ymm0,<r1=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: r0 = b0 & a0
# asm 1: vpand <b0=reg256#12,<a0=reg256#11,>r0=reg256#2
# asm 2: vpand <b0=%ymm11,<a0=%ymm10,>r0=%ymm1
vpand % ymm11, % ymm10, % ymm1

# qhasm: mem256[ ptr + 128 ] = r4
# asm 1: vmovupd   <r4=reg256#6,128(<ptr=int64#4)
# asm 2: vmovupd   <r4=%ymm5,128(<ptr=%rcx)
vmovupd   % ymm5, 128( % rcx)

# qhasm: mem256[ ptr + 96 ] = r3
# asm 1: vmovupd   <r3=reg256#8,96(<ptr=int64#4)
# asm 2: vmovupd   <r3=%ymm7,96(<ptr=%rcx)
vmovupd   % ymm7, 96( % rcx)

# qhasm: mem256[ ptr + 64 ] = r2
# asm 1: vmovupd   <r2=reg256#10,64(<ptr=int64#4)
# asm 2: vmovupd   <r2=%ymm9,64(<ptr=%rcx)
vmovupd   % ymm9, 64( % rcx)

# qhasm: mem256[ ptr + 32 ] = r1
# asm 1: vmovupd   <r1=reg256#1,32(<ptr=int64#4)
# asm 2: vmovupd   <r1=%ymm0,32(<ptr=%rcx)
vmovupd   % ymm0, 32( % rcx)

# qhasm: mem256[ ptr + 0 ] = r0
# asm 1: vmovupd   <r0=reg256#2,0(<ptr=int64#4)
# asm 2: vmovupd   <r0=%ymm1,0(<ptr=%rcx)
vmovupd   % ymm1, 0( % rcx)

# qhasm: vzeroupper
vzeroupper

# qhasm: h22 = mem128[ ptr + 528 ]
# asm 1: movdqu   528(<ptr=int64#4),>h22=reg128#1
# asm 2: movdqu   528(<ptr=%rcx),>h22=%xmm0
movdqu   528( % rcx), % xmm0

# qhasm: h13 = h22
# asm 1: movdqa <h22=reg128#1,>h13=reg128#2
# asm 2: movdqa <h22=%xmm0,>h13=%xmm1
movdqa % xmm0, % xmm1

# qhasm: h10 = h22
# asm 1: movdqa <h22=reg128#1,>h10=reg128#1
# asm 2: movdqa <h22=%xmm0,>h10=%xmm0
movdqa % xmm0, % xmm0

# qhasm: h21 = mem128[ ptr + 496 ]
# asm 1: movdqu   496(<ptr=int64#4),>h21=reg128#3
# asm 2: movdqu   496(<ptr=%rcx),>h21=%xmm2
movdqu   496( % rcx), % xmm2

# qhasm: h12 = h21
# asm 1: movdqa <h21=reg128#3,>h12=reg128#4
# asm 2: movdqa <h21=%xmm2,>h12=%xmm3
movdqa % xmm2, % xmm3

# qhasm: h9 = h21
# asm 1: movdqa <h21=reg128#3,>h9=reg128#3
# asm 2: movdqa <h21=%xmm2,>h9=%xmm2
movdqa % xmm2, % xmm2

# qhasm: h20 = mem128[ ptr + 464 ]
# asm 1: movdqu   464(<ptr=int64#4),>h20=reg128#5
# asm 2: movdqu   464(<ptr=%rcx),>h20=%xmm4
movdqu   464( % rcx), % xmm4

# qhasm: h11 = h20
# asm 1: movdqa <h20=reg128#5,>h11=reg128#6
# asm 2: movdqa <h20=%xmm4,>h11=%xmm5
movdqa % xmm4, % xmm5

# qhasm: h8 = h20
# asm 1: movdqa <h20=reg128#5,>h8=reg128#5
# asm 2: movdqa <h20=%xmm4,>h8=%xmm4
movdqa % xmm4, % xmm4

# qhasm: h19 = mem128[ ptr + 432 ]
# asm 1: movdqu   432(<ptr=int64#4),>h19=reg128#7
# asm 2: movdqu   432(<ptr=%rcx),>h19=%xmm6
movdqu   432( % rcx), % xmm6

# qhasm: h10 = h10 ^ h19
# asm 1: vpxor <h19=reg128#7,<h10=reg128#1,>h10=reg128#1
# asm 2: vpxor <h19=%xmm6,<h10=%xmm0,>h10=%xmm0
vpxor % xmm6, % xmm0, % xmm0

# qhasm: h7 = h19
# asm 1: movdqa <h19=reg128#7,>h7=reg128#7
# asm 2: movdqa <h19=%xmm6,>h7=%xmm6
movdqa % xmm6, % xmm6

# qhasm: h18 = mem128[ ptr + 400 ]
# asm 1: movdqu   400(<ptr=int64#4),>h18=reg128#8
# asm 2: movdqu   400(<ptr=%rcx),>h18=%xmm7
movdqu   400( % rcx), % xmm7

# qhasm: h9 = h9 ^ h18
# asm 1: vpxor <h18=reg128#8,<h9=reg128#3,>h9=reg128#3
# asm 2: vpxor <h18=%xmm7,<h9=%xmm2,>h9=%xmm2
vpxor % xmm7, % xmm2, % xmm2

# qhasm: h6 = h18
# asm 1: movdqa <h18=reg128#8,>h6=reg128#8
# asm 2: movdqa <h18=%xmm7,>h6=%xmm7
movdqa % xmm7, % xmm7

# qhasm: h17 = mem128[ ptr + 368 ]
# asm 1: movdqu   368(<ptr=int64#4),>h17=reg128#9
# asm 2: movdqu   368(<ptr=%rcx),>h17=%xmm8
movdqu   368( % rcx), % xmm8

# qhasm: h8 = h8 ^ h17
# asm 1: vpxor <h17=reg128#9,<h8=reg128#5,>h8=reg128#5
# asm 2: vpxor <h17=%xmm8,<h8=%xmm4,>h8=%xmm4
vpxor % xmm8, % xmm4, % xmm4

# qhasm: h5 = h17
# asm 1: movdqa <h17=reg128#9,>h5=reg128#9
# asm 2: movdqa <h17=%xmm8,>h5=%xmm8
movdqa % xmm8, % xmm8

# qhasm: h16 = mem128[ ptr + 336 ]
# asm 1: movdqu   336(<ptr=int64#4),>h16=reg128#10
# asm 2: movdqu   336(<ptr=%rcx),>h16=%xmm9
movdqu   336( % rcx), % xmm9

# qhasm: h16 = h16 ^ mem128[ ptr + 512 ]
# asm 1: vpxor 512(<ptr=int64#4),<h16=reg128#10,>h16=reg128#10
# asm 2: vpxor 512(<ptr=%rcx),<h16=%xmm9,>h16=%xmm9
vpxor 512( % rcx), % xmm9, % xmm9

# qhasm: h7 = h7 ^ h16
# asm 1: vpxor <h16=reg128#10,<h7=reg128#7,>h7=reg128#7
# asm 2: vpxor <h16=%xmm9,<h7=%xmm6,>h7=%xmm6
vpxor % xmm9, % xmm6, % xmm6

# qhasm: h4 = h16
# asm 1: movdqa <h16=reg128#10,>h4=reg128#10
# asm 2: movdqa <h16=%xmm9,>h4=%xmm9
movdqa % xmm9, % xmm9

# qhasm: h15 = mem128[ ptr + 304 ]
# asm 1: movdqu   304(<ptr=int64#4),>h15=reg128#11
# asm 2: movdqu   304(<ptr=%rcx),>h15=%xmm10
movdqu   304( % rcx), % xmm10

# qhasm: h15 = h15 ^ mem128[ ptr + 480 ]
# asm 1: vpxor 480(<ptr=int64#4),<h15=reg128#11,>h15=reg128#11
# asm 2: vpxor 480(<ptr=%rcx),<h15=%xmm10,>h15=%xmm10
vpxor 480( % rcx), % xmm10, % xmm10

# qhasm: h6 = h6 ^ h15
# asm 1: vpxor <h15=reg128#11,<h6=reg128#8,>h6=reg128#8
# asm 2: vpxor <h15=%xmm10,<h6=%xmm7,>h6=%xmm7
vpxor % xmm10, % xmm7, % xmm7

# qhasm: h3 = h15
# asm 1: movdqa <h15=reg128#11,>h3=reg128#11
# asm 2: movdqa <h15=%xmm10,>h3=%xmm10
movdqa % xmm10, % xmm10

# qhasm: h14 = mem128[ ptr + 272 ]
# asm 1: movdqu   272(<ptr=int64#4),>h14=reg128#12
# asm 2: movdqu   272(<ptr=%rcx),>h14=%xmm11
movdqu   272( % rcx), % xmm11

# qhasm: h14 = h14 ^ mem128[ ptr + 448 ]
# asm 1: vpxor 448(<ptr=int64#4),<h14=reg128#12,>h14=reg128#12
# asm 2: vpxor 448(<ptr=%rcx),<h14=%xmm11,>h14=%xmm11
vpxor 448( % rcx), % xmm11, % xmm11

# qhasm: h5 = h5 ^ h14
# asm 1: vpxor <h14=reg128#12,<h5=reg128#9,>h5=reg128#9
# asm 2: vpxor <h14=%xmm11,<h5=%xmm8,>h5=%xmm8
vpxor % xmm11, % xmm8, % xmm8

# qhasm: h2 = h14
# asm 1: movdqa <h14=reg128#12,>h2=reg128#12
# asm 2: movdqa <h14=%xmm11,>h2=%xmm11
movdqa % xmm11, % xmm11

# qhasm: h13 = h13 ^ mem128[ ptr + 240 ]
# asm 1: vpxor 240(<ptr=int64#4),<h13=reg128#2,>h13=reg128#2
# asm 2: vpxor 240(<ptr=%rcx),<h13=%xmm1,>h13=%xmm1
vpxor 240( % rcx), % xmm1, % xmm1

# qhasm: h13 = h13 ^ mem128[ ptr + 416 ]
# asm 1: vpxor 416(<ptr=int64#4),<h13=reg128#2,>h13=reg128#2
# asm 2: vpxor 416(<ptr=%rcx),<h13=%xmm1,>h13=%xmm1
vpxor 416( % rcx), % xmm1, % xmm1

# qhasm: h4 = h4 ^ h13
# asm 1: vpxor <h13=reg128#2,<h4=reg128#10,>h4=reg128#10
# asm 2: vpxor <h13=%xmm1,<h4=%xmm9,>h4=%xmm9
vpxor % xmm1, % xmm9, % xmm9

# qhasm: h1 = h13
# asm 1: movdqa <h13=reg128#2,>h1=reg128#2
# asm 2: movdqa <h13=%xmm1,>h1=%xmm1
movdqa % xmm1, % xmm1

# qhasm: h12 = h12 ^ mem128[ ptr + 208 ]
# asm 1: vpxor 208(<ptr=int64#4),<h12=reg128#4,>h12=reg128#4
# asm 2: vpxor 208(<ptr=%rcx),<h12=%xmm3,>h12=%xmm3
vpxor 208( % rcx), % xmm3, % xmm3

# qhasm: h12 = h12 ^ mem128[ ptr + 384 ]
# asm 1: vpxor 384(<ptr=int64#4),<h12=reg128#4,>h12=reg128#4
# asm 2: vpxor 384(<ptr=%rcx),<h12=%xmm3,>h12=%xmm3
vpxor 384( % rcx), % xmm3, % xmm3

# qhasm: h3 = h3 ^ h12
# asm 1: vpxor <h12=reg128#4,<h3=reg128#11,>h3=reg128#11
# asm 2: vpxor <h12=%xmm3,<h3=%xmm10,>h3=%xmm10
vpxor % xmm3, % xmm10, % xmm10

# qhasm: h0 = h12
# asm 1: movdqa <h12=reg128#4,>h0=reg128#4
# asm 2: movdqa <h12=%xmm3,>h0=%xmm3
movdqa % xmm3, % xmm3

# qhasm: h11 = h11 ^ mem128[ ptr + 352 ]
# asm 1: vpxor 352(<ptr=int64#4),<h11=reg128#6,>h11=reg128#6
# asm 2: vpxor 352(<ptr=%rcx),<h11=%xmm5,>h11=%xmm5
vpxor 352( % rcx), % xmm5, % xmm5

# qhasm: h11 = h11 ^ mem128[ ptr + 176 ]
# asm 1: vpxor 176(<ptr=int64#4),<h11=reg128#6,>h11=reg128#6
# asm 2: vpxor 176(<ptr=%rcx),<h11=%xmm5,>h11=%xmm5
vpxor 176( % rcx), % xmm5, % xmm5

# qhasm: mem128[ input_0 + 176 ] = h11
# asm 1: movdqu   <h11=reg128#6,176(<input_0=int64#1)
# asm 2: movdqu   <h11=%xmm5,176(<input_0=%rdi)
movdqu   % xmm5, 176( % rdi)

# qhasm: h10 = h10 ^ mem128[ ptr + 320 ]
# asm 1: vpxor 320(<ptr=int64#4),<h10=reg128#1,>h10=reg128#1
# asm 2: vpxor 320(<ptr=%rcx),<h10=%xmm0,>h10=%xmm0
vpxor 320( % rcx), % xmm0, % xmm0

# qhasm: h10 = h10 ^ mem128[ ptr + 144 ]
# asm 1: vpxor 144(<ptr=int64#4),<h10=reg128#1,>h10=reg128#1
# asm 2: vpxor 144(<ptr=%rcx),<h10=%xmm0,>h10=%xmm0
vpxor 144( % rcx), % xmm0, % xmm0

# qhasm: mem128[ input_0 + 160 ] = h10
# asm 1: movdqu   <h10=reg128#1,160(<input_0=int64#1)
# asm 2: movdqu   <h10=%xmm0,160(<input_0=%rdi)
movdqu   % xmm0, 160( % rdi)

# qhasm: h9 = h9 ^ mem128[ ptr + 288 ]
# asm 1: vpxor 288(<ptr=int64#4),<h9=reg128#3,>h9=reg128#1
# asm 2: vpxor 288(<ptr=%rcx),<h9=%xmm2,>h9=%xmm0
vpxor 288( % rcx), % xmm2, % xmm0

# qhasm: h9 = h9 ^ mem128[ ptr + 112 ]
# asm 1: vpxor 112(<ptr=int64#4),<h9=reg128#1,>h9=reg128#1
# asm 2: vpxor 112(<ptr=%rcx),<h9=%xmm0,>h9=%xmm0
vpxor 112( % rcx), % xmm0, % xmm0

# qhasm: mem128[ input_0 + 144 ] = h9
# asm 1: movdqu   <h9=reg128#1,144(<input_0=int64#1)
# asm 2: movdqu   <h9=%xmm0,144(<input_0=%rdi)
movdqu   % xmm0, 144( % rdi)

# qhasm: h8 = h8 ^ mem128[ ptr + 256 ]
# asm 1: vpxor 256(<ptr=int64#4),<h8=reg128#5,>h8=reg128#1
# asm 2: vpxor 256(<ptr=%rcx),<h8=%xmm4,>h8=%xmm0
vpxor 256( % rcx), % xmm4, % xmm0

# qhasm: h8 = h8 ^ mem128[ ptr + 80 ]
# asm 1: vpxor 80(<ptr=int64#4),<h8=reg128#1,>h8=reg128#1
# asm 2: vpxor 80(<ptr=%rcx),<h8=%xmm0,>h8=%xmm0
vpxor 80( % rcx), % xmm0, % xmm0

# qhasm: mem128[ input_0 + 128 ] = h8
# asm 1: movdqu   <h8=reg128#1,128(<input_0=int64#1)
# asm 2: movdqu   <h8=%xmm0,128(<input_0=%rdi)
movdqu   % xmm0, 128( % rdi)

# qhasm: h7 = h7 ^ mem128[ ptr + 224 ]
# asm 1: vpxor 224(<ptr=int64#4),<h7=reg128#7,>h7=reg128#1
# asm 2: vpxor 224(<ptr=%rcx),<h7=%xmm6,>h7=%xmm0
vpxor 224( % rcx), % xmm6, % xmm0

# qhasm: h7 = h7 ^ mem128[ ptr + 48 ]
# asm 1: vpxor 48(<ptr=int64#4),<h7=reg128#1,>h7=reg128#1
# asm 2: vpxor 48(<ptr=%rcx),<h7=%xmm0,>h7=%xmm0
vpxor 48( % rcx), % xmm0, % xmm0

# qhasm: mem128[ input_0 + 112 ] = h7
# asm 1: movdqu   <h7=reg128#1,112(<input_0=int64#1)
# asm 2: movdqu   <h7=%xmm0,112(<input_0=%rdi)
movdqu   % xmm0, 112( % rdi)

# qhasm: h6 = h6 ^ mem128[ ptr + 192 ]
# asm 1: vpxor 192(<ptr=int64#4),<h6=reg128#8,>h6=reg128#1
# asm 2: vpxor 192(<ptr=%rcx),<h6=%xmm7,>h6=%xmm0
vpxor 192( % rcx), % xmm7, % xmm0

# qhasm: h6 = h6 ^ mem128[ ptr + 16 ]
# asm 1: vpxor 16(<ptr=int64#4),<h6=reg128#1,>h6=reg128#1
# asm 2: vpxor 16(<ptr=%rcx),<h6=%xmm0,>h6=%xmm0
vpxor 16( % rcx), % xmm0, % xmm0

# qhasm: mem128[ input_0 + 96 ] = h6
# asm 1: movdqu   <h6=reg128#1,96(<input_0=int64#1)
# asm 2: movdqu   <h6=%xmm0,96(<input_0=%rdi)
movdqu   % xmm0, 96( % rdi)

# qhasm: h5 = h5 ^ mem128[ ptr + 160 ]
# asm 1: vpxor 160(<ptr=int64#4),<h5=reg128#9,>h5=reg128#1
# asm 2: vpxor 160(<ptr=%rcx),<h5=%xmm8,>h5=%xmm0
vpxor 160( % rcx), % xmm8, % xmm0

# qhasm: mem128[ input_0 + 80 ] = h5
# asm 1: movdqu   <h5=reg128#1,80(<input_0=int64#1)
# asm 2: movdqu   <h5=%xmm0,80(<input_0=%rdi)
movdqu   % xmm0, 80( % rdi)

# qhasm: h4 = h4 ^ mem128[ ptr + 128 ]
# asm 1: vpxor 128(<ptr=int64#4),<h4=reg128#10,>h4=reg128#1
# asm 2: vpxor 128(<ptr=%rcx),<h4=%xmm9,>h4=%xmm0
vpxor 128( % rcx), % xmm9, % xmm0

# qhasm: mem128[ input_0 + 64 ] = h4
# asm 1: movdqu   <h4=reg128#1,64(<input_0=int64#1)
# asm 2: movdqu   <h4=%xmm0,64(<input_0=%rdi)
movdqu   % xmm0, 64( % rdi)

# qhasm: h3 = h3 ^ mem128[ ptr + 96 ]
# asm 1: vpxor 96(<ptr=int64#4),<h3=reg128#11,>h3=reg128#1
# asm 2: vpxor 96(<ptr=%rcx),<h3=%xmm10,>h3=%xmm0
vpxor 96( % rcx), % xmm10, % xmm0

# qhasm: mem128[ input_0 + 48 ] = h3
# asm 1: movdqu   <h3=reg128#1,48(<input_0=int64#1)
# asm 2: movdqu   <h3=%xmm0,48(<input_0=%rdi)
movdqu   % xmm0, 48( % rdi)

# qhasm: h2 = h2 ^ mem128[ ptr + 64 ]
# asm 1: vpxor 64(<ptr=int64#4),<h2=reg128#12,>h2=reg128#1
# asm 2: vpxor 64(<ptr=%rcx),<h2=%xmm11,>h2=%xmm0
vpxor 64( % rcx), % xmm11, % xmm0

# qhasm: mem128[ input_0 + 32 ] = h2
# asm 1: movdqu   <h2=reg128#1,32(<input_0=int64#1)
# asm 2: movdqu   <h2=%xmm0,32(<input_0=%rdi)
movdqu   % xmm0, 32( % rdi)

# qhasm: h1 = h1 ^ mem128[ ptr + 32 ]
# asm 1: vpxor 32(<ptr=int64#4),<h1=reg128#2,>h1=reg128#1
# asm 2: vpxor 32(<ptr=%rcx),<h1=%xmm1,>h1=%xmm0
vpxor 32( % rcx), % xmm1, % xmm0

# qhasm: mem128[ input_0 + 16 ] = h1
# asm 1: movdqu   <h1=reg128#1,16(<input_0=int64#1)
# asm 2: movdqu   <h1=%xmm0,16(<input_0=%rdi)
movdqu   % xmm0, 16( % rdi)

# qhasm: h0 = h0 ^ mem128[ ptr + 0 ]
# asm 1: vpxor 0(<ptr=int64#4),<h0=reg128#4,>h0=reg128#1
# asm 2: vpxor 0(<ptr=%rcx),<h0=%xmm3,>h0=%xmm0
vpxor 0( % rcx), % xmm3, % xmm0

# qhasm: mem128[ input_0 + 0 ] = h0
# asm 1: movdqu   <h0=reg128#1,0(<input_0=int64#1)
# asm 2: movdqu   <h0=%xmm0,0(<input_0=%rdi)
movdqu   % xmm0, 0( % rdi)

# qhasm: return
add % r11, % rsp
ret
