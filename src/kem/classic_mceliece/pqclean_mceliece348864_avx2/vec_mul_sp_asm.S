#include "namespace.h"

#define vec_mul_sp_asm CRYPTO_NAMESPACE(vec_mul_sp_asm)
#define _vec_mul_sp_asm _CRYPTO_NAMESPACE(vec_mul_sp_asm)

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

# qhasm: reg256 s0

# qhasm: reg256 s1

# qhasm: reg256 s2

# qhasm: reg256 s3

# qhasm: reg256 s4

# qhasm: reg256 s5

# qhasm: reg256 b0

# qhasm: reg256 b1

# qhasm: reg256 b2

# qhasm: reg256 b3

# qhasm: reg256 b4

# qhasm: reg256 b5

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

# qhasm: int64 h0

# qhasm: int64 h1

# qhasm: int64 h2

# qhasm: int64 h3

# qhasm: int64 h4

# qhasm: int64 h5

# qhasm: int64 h6

# qhasm: int64 h7

# qhasm: int64 h8

# qhasm: int64 h9

# qhasm: int64 h10

# qhasm: int64 h11

# qhasm: int64 h12

# qhasm: int64 h13

# qhasm: int64 h14

# qhasm: int64 h15

# qhasm: int64 h16

# qhasm: int64 h17

# qhasm: int64 h18

# qhasm: int64 h19

# qhasm: int64 h20

# qhasm: int64 h21

# qhasm: int64 h22

# qhasm: stack4864 buf

# qhasm: int64 ptr

# qhasm: int64 tmp

# qhasm: stack64 r11_stack

# qhasm: stack64 r12_stack

# qhasm: stack64 r13_stack

# qhasm: stack64 r14_stack

# qhasm: stack64 r15_stack

# qhasm: stack64 rbx_stack

# qhasm: stack64 rbp_stack

# qhasm: enter vec_mul_sp_asm
.p2align 5
.global _vec_mul_sp_asm
.global vec_mul_sp_asm
_vec_mul_sp_asm:
vec_mul_sp_asm:
mov % rsp, % r11
and $31, % r11
add $672, % r11
sub % r11, % rsp

# qhasm: r11_stack = caller_r11
# asm 1: movq <caller_r11=int64#9,>r11_stack=stack64#1
# asm 2: movq <caller_r11=%r11,>r11_stack=608(%rsp)
movq % r11, 608( % rsp)

# qhasm: r12_stack = caller_r12
# asm 1: movq <caller_r12=int64#10,>r12_stack=stack64#2
# asm 2: movq <caller_r12=%r12,>r12_stack=616(%rsp)
movq % r12, 616( % rsp)

# qhasm: r13_stack = caller_r13
# asm 1: movq <caller_r13=int64#11,>r13_stack=stack64#3
# asm 2: movq <caller_r13=%r13,>r13_stack=624(%rsp)
movq % r13, 624( % rsp)

# qhasm: r14_stack = caller_r14
# asm 1: movq <caller_r14=int64#12,>r14_stack=stack64#4
# asm 2: movq <caller_r14=%r14,>r14_stack=632(%rsp)
movq % r14, 632( % rsp)

# qhasm: r15_stack = caller_r15
# asm 1: movq <caller_r15=int64#13,>r15_stack=stack64#5
# asm 2: movq <caller_r15=%r15,>r15_stack=640(%rsp)
movq % r15, 640( % rsp)

# qhasm: rbx_stack = caller_rbx
# asm 1: movq <caller_rbx=int64#14,>rbx_stack=stack64#6
# asm 2: movq <caller_rbx=%rbx,>rbx_stack=648(%rsp)
movq % rbx, 648( % rsp)

# qhasm: ptr = &buf
# asm 1: leaq <buf=stack4864#1,>ptr=int64#4
# asm 2: leaq <buf=0(%rsp),>ptr=%rcx
leaq 0( % rsp), % rcx

# qhasm: s0 = mem256[ input_1 +  0 ]
# asm 1: vmovupd   0(<input_1=int64#2),>s0=reg256#1
# asm 2: vmovupd   0(<input_1=%rsi),>s0=%ymm0
vmovupd   0( % rsi), % ymm0

# qhasm: s1 = mem256[ input_1 + 32 ]
# asm 1: vmovupd   32(<input_1=int64#2),>s1=reg256#2
# asm 2: vmovupd   32(<input_1=%rsi),>s1=%ymm1
vmovupd   32( % rsi), % ymm1

# qhasm: s2 = mem256[ input_1 + 64 ]
# asm 1: vmovupd   64(<input_1=int64#2),>s2=reg256#3
# asm 2: vmovupd   64(<input_1=%rsi),>s2=%ymm2
vmovupd   64( % rsi), % ymm2

# qhasm: a5[0,1,2,3] = s2[2,2,3,3]
# asm 1: vpermq $0xfa,<s2=reg256#3,>a5=reg256#4
# asm 2: vpermq $0xfa,<s2=%ymm2,>a5=%ymm3
vpermq $0xfa, % ymm2, % ymm3

# qhasm: r = mem256[ input_2 + 160 ]
# asm 1: vmovupd   160(<input_2=int64#3),>r=reg256#5
# asm 2: vmovupd   160(<input_2=%rdx),>r=%ymm4
vmovupd   160( % rdx), % ymm4

# qhasm: b5[0,1,2,3] = r[1,3,1,3]
# asm 1: vpermq $0xdd,<r=reg256#5,>b5=reg256#5
# asm 2: vpermq $0xdd,<r=%ymm4,>b5=%ymm4
vpermq $0xdd, % ymm4, % ymm4

# qhasm: r10 = a5 & b5
# asm 1: vpand <a5=reg256#4,<b5=reg256#5,>r10=reg256#6
# asm 2: vpand <a5=%ymm3,<b5=%ymm4,>r10=%ymm5
vpand % ymm3, % ymm4, % ymm5

# qhasm: mem256[ ptr + 320 ] = r10
# asm 1: vmovupd   <r10=reg256#6,320(<ptr=int64#4)
# asm 2: vmovupd   <r10=%ymm5,320(<ptr=%rcx)
vmovupd   % ymm5, 320( % rcx)

# qhasm: r = mem256[ input_2 + 128 ]
# asm 1: vmovupd   128(<input_2=int64#3),>r=reg256#6
# asm 2: vmovupd   128(<input_2=%rdx),>r=%ymm5
vmovupd   128( % rdx), % ymm5

# qhasm: b4[0,1,2,3] = r[1,3,1,3]
# asm 1: vpermq $0xdd,<r=reg256#6,>b4=reg256#6
# asm 2: vpermq $0xdd,<r=%ymm5,>b4=%ymm5
vpermq $0xdd, % ymm5, % ymm5

# qhasm: r9 = a5 & b4
# asm 1: vpand <a5=reg256#4,<b4=reg256#6,>r9=reg256#7
# asm 2: vpand <a5=%ymm3,<b4=%ymm5,>r9=%ymm6
vpand % ymm3, % ymm5, % ymm6

# qhasm: r = mem256[ input_2 + 96 ]
# asm 1: vmovupd   96(<input_2=int64#3),>r=reg256#8
# asm 2: vmovupd   96(<input_2=%rdx),>r=%ymm7
vmovupd   96( % rdx), % ymm7

# qhasm: b3[0,1,2,3] = r[1,3,1,3]
# asm 1: vpermq $0xdd,<r=reg256#8,>b3=reg256#8
# asm 2: vpermq $0xdd,<r=%ymm7,>b3=%ymm7
vpermq $0xdd, % ymm7, % ymm7

# qhasm: r8 = a5 & b3
# asm 1: vpand <a5=reg256#4,<b3=reg256#8,>r8=reg256#9
# asm 2: vpand <a5=%ymm3,<b3=%ymm7,>r8=%ymm8
vpand % ymm3, % ymm7, % ymm8

# qhasm: r = mem256[ input_2 + 64 ]
# asm 1: vmovupd   64(<input_2=int64#3),>r=reg256#10
# asm 2: vmovupd   64(<input_2=%rdx),>r=%ymm9
vmovupd   64( % rdx), % ymm9

# qhasm: b2[0,1,2,3] = r[1,3,1,3]
# asm 1: vpermq $0xdd,<r=reg256#10,>b2=reg256#10
# asm 2: vpermq $0xdd,<r=%ymm9,>b2=%ymm9
vpermq $0xdd, % ymm9, % ymm9

# qhasm: r7 = a5 & b2
# asm 1: vpand <a5=reg256#4,<b2=reg256#10,>r7=reg256#11
# asm 2: vpand <a5=%ymm3,<b2=%ymm9,>r7=%ymm10
vpand % ymm3, % ymm9, % ymm10

# qhasm: r = mem256[ input_2 + 32 ]
# asm 1: vmovupd   32(<input_2=int64#3),>r=reg256#12
# asm 2: vmovupd   32(<input_2=%rdx),>r=%ymm11
vmovupd   32( % rdx), % ymm11

# qhasm: b1[0,1,2,3] = r[1,3,1,3]
# asm 1: vpermq $0xdd,<r=reg256#12,>b1=reg256#12
# asm 2: vpermq $0xdd,<r=%ymm11,>b1=%ymm11
vpermq $0xdd, % ymm11, % ymm11

# qhasm: r6 = a5 & b1
# asm 1: vpand <a5=reg256#4,<b1=reg256#12,>r6=reg256#13
# asm 2: vpand <a5=%ymm3,<b1=%ymm11,>r6=%ymm12
vpand % ymm3, % ymm11, % ymm12

# qhasm: r = mem256[ input_2 + 0 ]
# asm 1: vmovupd   0(<input_2=int64#3),>r=reg256#14
# asm 2: vmovupd   0(<input_2=%rdx),>r=%ymm13
vmovupd   0( % rdx), % ymm13

# qhasm: b0[0,1,2,3] = r[1,3,1,3]
# asm 1: vpermq $0xdd,<r=reg256#14,>b0=reg256#14
# asm 2: vpermq $0xdd,<r=%ymm13,>b0=%ymm13
vpermq $0xdd, % ymm13, % ymm13

# qhasm: r5 = a5 & b0
# asm 1: vpand <a5=reg256#4,<b0=reg256#14,>r5=reg256#4
# asm 2: vpand <a5=%ymm3,<b0=%ymm13,>r5=%ymm3
vpand % ymm3, % ymm13, % ymm3

# qhasm: a4[0,1,2,3] = s2[0,0,1,1]
# asm 1: vpermq $0x50,<s2=reg256#3,>a4=reg256#3
# asm 2: vpermq $0x50,<s2=%ymm2,>a4=%ymm2
vpermq $0x50, % ymm2, % ymm2

# qhasm: r = a4 & b5
# asm 1: vpand <a4=reg256#3,<b5=reg256#5,>r=reg256#15
# asm 2: vpand <a4=%ymm2,<b5=%ymm4,>r=%ymm14
vpand % ymm2, % ymm4, % ymm14

# qhasm: r9 ^= r
# asm 1: vpxor <r=reg256#15,<r9=reg256#7,<r9=reg256#7
# asm 2: vpxor <r=%ymm14,<r9=%ymm6,<r9=%ymm6
vpxor % ymm14, % ymm6, % ymm6

# qhasm: mem256[ ptr + 288 ] = r9
# asm 1: vmovupd   <r9=reg256#7,288(<ptr=int64#4)
# asm 2: vmovupd   <r9=%ymm6,288(<ptr=%rcx)
vmovupd   % ymm6, 288( % rcx)

# qhasm: r = a4 & b4
# asm 1: vpand <a4=reg256#3,<b4=reg256#6,>r=reg256#7
# asm 2: vpand <a4=%ymm2,<b4=%ymm5,>r=%ymm6
vpand % ymm2, % ymm5, % ymm6

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#7,<r8=reg256#9,<r8=reg256#9
# asm 2: vpxor <r=%ymm6,<r8=%ymm8,<r8=%ymm8
vpxor % ymm6, % ymm8, % ymm8

# qhasm: r = a4 & b3
# asm 1: vpand <a4=reg256#3,<b3=reg256#8,>r=reg256#7
# asm 2: vpand <a4=%ymm2,<b3=%ymm7,>r=%ymm6
vpand % ymm2, % ymm7, % ymm6

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#7,<r7=reg256#11,<r7=reg256#11
# asm 2: vpxor <r=%ymm6,<r7=%ymm10,<r7=%ymm10
vpxor % ymm6, % ymm10, % ymm10

# qhasm: r = a4 & b2
# asm 1: vpand <a4=reg256#3,<b2=reg256#10,>r=reg256#7
# asm 2: vpand <a4=%ymm2,<b2=%ymm9,>r=%ymm6
vpand % ymm2, % ymm9, % ymm6

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#7,<r6=reg256#13,<r6=reg256#13
# asm 2: vpxor <r=%ymm6,<r6=%ymm12,<r6=%ymm12
vpxor % ymm6, % ymm12, % ymm12

# qhasm: r = a4 & b1
# asm 1: vpand <a4=reg256#3,<b1=reg256#12,>r=reg256#7
# asm 2: vpand <a4=%ymm2,<b1=%ymm11,>r=%ymm6
vpand % ymm2, % ymm11, % ymm6

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#7,<r5=reg256#4,<r5=reg256#4
# asm 2: vpxor <r=%ymm6,<r5=%ymm3,<r5=%ymm3
vpxor % ymm6, % ymm3, % ymm3

# qhasm: r4 = a4 & b0
# asm 1: vpand <a4=reg256#3,<b0=reg256#14,>r4=reg256#3
# asm 2: vpand <a4=%ymm2,<b0=%ymm13,>r4=%ymm2
vpand % ymm2, % ymm13, % ymm2

# qhasm: a3[0,1,2,3] = s1[2,2,3,3]
# asm 1: vpermq $0xfa,<s1=reg256#2,>a3=reg256#7
# asm 2: vpermq $0xfa,<s1=%ymm1,>a3=%ymm6
vpermq $0xfa, % ymm1, % ymm6

# qhasm: r = a3 & b5
# asm 1: vpand <a3=reg256#7,<b5=reg256#5,>r=reg256#15
# asm 2: vpand <a3=%ymm6,<b5=%ymm4,>r=%ymm14
vpand % ymm6, % ymm4, % ymm14

# qhasm: r8 ^= r
# asm 1: vpxor <r=reg256#15,<r8=reg256#9,<r8=reg256#9
# asm 2: vpxor <r=%ymm14,<r8=%ymm8,<r8=%ymm8
vpxor % ymm14, % ymm8, % ymm8

# qhasm: mem256[ ptr + 256 ] = r8
# asm 1: vmovupd   <r8=reg256#9,256(<ptr=int64#4)
# asm 2: vmovupd   <r8=%ymm8,256(<ptr=%rcx)
vmovupd   % ymm8, 256( % rcx)

# qhasm: r = a3 & b4
# asm 1: vpand <a3=reg256#7,<b4=reg256#6,>r=reg256#9
# asm 2: vpand <a3=%ymm6,<b4=%ymm5,>r=%ymm8
vpand % ymm6, % ymm5, % ymm8

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#9,<r7=reg256#11,<r7=reg256#11
# asm 2: vpxor <r=%ymm8,<r7=%ymm10,<r7=%ymm10
vpxor % ymm8, % ymm10, % ymm10

# qhasm: r = a3 & b3
# asm 1: vpand <a3=reg256#7,<b3=reg256#8,>r=reg256#9
# asm 2: vpand <a3=%ymm6,<b3=%ymm7,>r=%ymm8
vpand % ymm6, % ymm7, % ymm8

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#9,<r6=reg256#13,<r6=reg256#13
# asm 2: vpxor <r=%ymm8,<r6=%ymm12,<r6=%ymm12
vpxor % ymm8, % ymm12, % ymm12

# qhasm: r = a3 & b2
# asm 1: vpand <a3=reg256#7,<b2=reg256#10,>r=reg256#9
# asm 2: vpand <a3=%ymm6,<b2=%ymm9,>r=%ymm8
vpand % ymm6, % ymm9, % ymm8

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#9,<r5=reg256#4,<r5=reg256#4
# asm 2: vpxor <r=%ymm8,<r5=%ymm3,<r5=%ymm3
vpxor % ymm8, % ymm3, % ymm3

# qhasm: r = a3 & b1
# asm 1: vpand <a3=reg256#7,<b1=reg256#12,>r=reg256#9
# asm 2: vpand <a3=%ymm6,<b1=%ymm11,>r=%ymm8
vpand % ymm6, % ymm11, % ymm8

# qhasm: r4 ^= r
# asm 1: vpxor <r=reg256#9,<r4=reg256#3,<r4=reg256#3
# asm 2: vpxor <r=%ymm8,<r4=%ymm2,<r4=%ymm2
vpxor % ymm8, % ymm2, % ymm2

# qhasm: r3 = a3 & b0
# asm 1: vpand <a3=reg256#7,<b0=reg256#14,>r3=reg256#7
# asm 2: vpand <a3=%ymm6,<b0=%ymm13,>r3=%ymm6
vpand % ymm6, % ymm13, % ymm6

# qhasm: a2[0,1,2,3] = s1[0,0,1,1]
# asm 1: vpermq $0x50,<s1=reg256#2,>a2=reg256#2
# asm 2: vpermq $0x50,<s1=%ymm1,>a2=%ymm1
vpermq $0x50, % ymm1, % ymm1

# qhasm: r = a2 & b5
# asm 1: vpand <a2=reg256#2,<b5=reg256#5,>r=reg256#9
# asm 2: vpand <a2=%ymm1,<b5=%ymm4,>r=%ymm8
vpand % ymm1, % ymm4, % ymm8

# qhasm: r7 ^= r
# asm 1: vpxor <r=reg256#9,<r7=reg256#11,<r7=reg256#11
# asm 2: vpxor <r=%ymm8,<r7=%ymm10,<r7=%ymm10
vpxor % ymm8, % ymm10, % ymm10

# qhasm: mem256[ ptr + 224 ] = r7
# asm 1: vmovupd   <r7=reg256#11,224(<ptr=int64#4)
# asm 2: vmovupd   <r7=%ymm10,224(<ptr=%rcx)
vmovupd   % ymm10, 224( % rcx)

# qhasm: r = a2 & b4
# asm 1: vpand <a2=reg256#2,<b4=reg256#6,>r=reg256#9
# asm 2: vpand <a2=%ymm1,<b4=%ymm5,>r=%ymm8
vpand % ymm1, % ymm5, % ymm8

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#9,<r6=reg256#13,<r6=reg256#13
# asm 2: vpxor <r=%ymm8,<r6=%ymm12,<r6=%ymm12
vpxor % ymm8, % ymm12, % ymm12

# qhasm: r = a2 & b3
# asm 1: vpand <a2=reg256#2,<b3=reg256#8,>r=reg256#9
# asm 2: vpand <a2=%ymm1,<b3=%ymm7,>r=%ymm8
vpand % ymm1, % ymm7, % ymm8

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#9,<r5=reg256#4,<r5=reg256#4
# asm 2: vpxor <r=%ymm8,<r5=%ymm3,<r5=%ymm3
vpxor % ymm8, % ymm3, % ymm3

# qhasm: r = a2 & b2
# asm 1: vpand <a2=reg256#2,<b2=reg256#10,>r=reg256#9
# asm 2: vpand <a2=%ymm1,<b2=%ymm9,>r=%ymm8
vpand % ymm1, % ymm9, % ymm8

# qhasm: r4 ^= r
# asm 1: vpxor <r=reg256#9,<r4=reg256#3,<r4=reg256#3
# asm 2: vpxor <r=%ymm8,<r4=%ymm2,<r4=%ymm2
vpxor % ymm8, % ymm2, % ymm2

# qhasm: r = a2 & b1
# asm 1: vpand <a2=reg256#2,<b1=reg256#12,>r=reg256#9
# asm 2: vpand <a2=%ymm1,<b1=%ymm11,>r=%ymm8
vpand % ymm1, % ymm11, % ymm8

# qhasm: r3 ^= r
# asm 1: vpxor <r=reg256#9,<r3=reg256#7,<r3=reg256#7
# asm 2: vpxor <r=%ymm8,<r3=%ymm6,<r3=%ymm6
vpxor % ymm8, % ymm6, % ymm6

# qhasm: r2 = a2 & b0
# asm 1: vpand <a2=reg256#2,<b0=reg256#14,>r2=reg256#2
# asm 2: vpand <a2=%ymm1,<b0=%ymm13,>r2=%ymm1
vpand % ymm1, % ymm13, % ymm1

# qhasm: a1[0,1,2,3] = s0[2,2,3,3]
# asm 1: vpermq $0xfa,<s0=reg256#1,>a1=reg256#9
# asm 2: vpermq $0xfa,<s0=%ymm0,>a1=%ymm8
vpermq $0xfa, % ymm0, % ymm8

# qhasm: r = a1 & b5
# asm 1: vpand <a1=reg256#9,<b5=reg256#5,>r=reg256#11
# asm 2: vpand <a1=%ymm8,<b5=%ymm4,>r=%ymm10
vpand % ymm8, % ymm4, % ymm10

# qhasm: r6 ^= r
# asm 1: vpxor <r=reg256#11,<r6=reg256#13,<r6=reg256#13
# asm 2: vpxor <r=%ymm10,<r6=%ymm12,<r6=%ymm12
vpxor % ymm10, % ymm12, % ymm12

# qhasm: mem256[ ptr + 192 ] = r6
# asm 1: vmovupd   <r6=reg256#13,192(<ptr=int64#4)
# asm 2: vmovupd   <r6=%ymm12,192(<ptr=%rcx)
vmovupd   % ymm12, 192( % rcx)

# qhasm: r = a1 & b4
# asm 1: vpand <a1=reg256#9,<b4=reg256#6,>r=reg256#11
# asm 2: vpand <a1=%ymm8,<b4=%ymm5,>r=%ymm10
vpand % ymm8, % ymm5, % ymm10

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#11,<r5=reg256#4,<r5=reg256#4
# asm 2: vpxor <r=%ymm10,<r5=%ymm3,<r5=%ymm3
vpxor % ymm10, % ymm3, % ymm3

# qhasm: r = a1 & b3
# asm 1: vpand <a1=reg256#9,<b3=reg256#8,>r=reg256#11
# asm 2: vpand <a1=%ymm8,<b3=%ymm7,>r=%ymm10
vpand % ymm8, % ymm7, % ymm10

# qhasm: r4 ^= r
# asm 1: vpxor <r=reg256#11,<r4=reg256#3,<r4=reg256#3
# asm 2: vpxor <r=%ymm10,<r4=%ymm2,<r4=%ymm2
vpxor % ymm10, % ymm2, % ymm2

# qhasm: r = a1 & b2
# asm 1: vpand <a1=reg256#9,<b2=reg256#10,>r=reg256#11
# asm 2: vpand <a1=%ymm8,<b2=%ymm9,>r=%ymm10
vpand % ymm8, % ymm9, % ymm10

# qhasm: r3 ^= r
# asm 1: vpxor <r=reg256#11,<r3=reg256#7,<r3=reg256#7
# asm 2: vpxor <r=%ymm10,<r3=%ymm6,<r3=%ymm6
vpxor % ymm10, % ymm6, % ymm6

# qhasm: r = a1 & b1
# asm 1: vpand <a1=reg256#9,<b1=reg256#12,>r=reg256#11
# asm 2: vpand <a1=%ymm8,<b1=%ymm11,>r=%ymm10
vpand % ymm8, % ymm11, % ymm10

# qhasm: r2 ^= r
# asm 1: vpxor <r=reg256#11,<r2=reg256#2,<r2=reg256#2
# asm 2: vpxor <r=%ymm10,<r2=%ymm1,<r2=%ymm1
vpxor % ymm10, % ymm1, % ymm1

# qhasm: r1 = a1 & b0
# asm 1: vpand <a1=reg256#9,<b0=reg256#14,>r1=reg256#9
# asm 2: vpand <a1=%ymm8,<b0=%ymm13,>r1=%ymm8
vpand % ymm8, % ymm13, % ymm8

# qhasm: a0[0,1,2,3] = s0[0,0,1,1]
# asm 1: vpermq $0x50,<s0=reg256#1,>a0=reg256#1
# asm 2: vpermq $0x50,<s0=%ymm0,>a0=%ymm0
vpermq $0x50, % ymm0, % ymm0

# qhasm: r = a0 & b5
# asm 1: vpand <a0=reg256#1,<b5=reg256#5,>r=reg256#5
# asm 2: vpand <a0=%ymm0,<b5=%ymm4,>r=%ymm4
vpand % ymm0, % ymm4, % ymm4

# qhasm: r5 ^= r
# asm 1: vpxor <r=reg256#5,<r5=reg256#4,<r5=reg256#4
# asm 2: vpxor <r=%ymm4,<r5=%ymm3,<r5=%ymm3
vpxor % ymm4, % ymm3, % ymm3

# qhasm: mem256[ ptr + 160 ] = r5
# asm 1: vmovupd   <r5=reg256#4,160(<ptr=int64#4)
# asm 2: vmovupd   <r5=%ymm3,160(<ptr=%rcx)
vmovupd   % ymm3, 160( % rcx)

# qhasm: r = a0 & b4
# asm 1: vpand <a0=reg256#1,<b4=reg256#6,>r=reg256#4
# asm 2: vpand <a0=%ymm0,<b4=%ymm5,>r=%ymm3
vpand % ymm0, % ymm5, % ymm3

# qhasm: r4 ^= r
# asm 1: vpxor <r=reg256#4,<r4=reg256#3,<r4=reg256#3
# asm 2: vpxor <r=%ymm3,<r4=%ymm2,<r4=%ymm2
vpxor % ymm3, % ymm2, % ymm2

# qhasm: r = a0 & b3
# asm 1: vpand <a0=reg256#1,<b3=reg256#8,>r=reg256#4
# asm 2: vpand <a0=%ymm0,<b3=%ymm7,>r=%ymm3
vpand % ymm0, % ymm7, % ymm3

# qhasm: r3 ^= r
# asm 1: vpxor <r=reg256#4,<r3=reg256#7,<r3=reg256#7
# asm 2: vpxor <r=%ymm3,<r3=%ymm6,<r3=%ymm6
vpxor % ymm3, % ymm6, % ymm6

# qhasm: r = a0 & b2
# asm 1: vpand <a0=reg256#1,<b2=reg256#10,>r=reg256#4
# asm 2: vpand <a0=%ymm0,<b2=%ymm9,>r=%ymm3
vpand % ymm0, % ymm9, % ymm3

# qhasm: r2 ^= r
# asm 1: vpxor <r=reg256#4,<r2=reg256#2,<r2=reg256#2
# asm 2: vpxor <r=%ymm3,<r2=%ymm1,<r2=%ymm1
vpxor % ymm3, % ymm1, % ymm1

# qhasm: r = a0 & b1
# asm 1: vpand <a0=reg256#1,<b1=reg256#12,>r=reg256#4
# asm 2: vpand <a0=%ymm0,<b1=%ymm11,>r=%ymm3
vpand % ymm0, % ymm11, % ymm3

# qhasm: r1 ^= r
# asm 1: vpxor <r=reg256#4,<r1=reg256#9,<r1=reg256#9
# asm 2: vpxor <r=%ymm3,<r1=%ymm8,<r1=%ymm8
vpxor % ymm3, % ymm8, % ymm8

# qhasm: r0 = a0 & b0
# asm 1: vpand <a0=reg256#1,<b0=reg256#14,>r0=reg256#1
# asm 2: vpand <a0=%ymm0,<b0=%ymm13,>r0=%ymm0
vpand % ymm0, % ymm13, % ymm0

# qhasm: mem256[ ptr + 128 ] = r4
# asm 1: vmovupd   <r4=reg256#3,128(<ptr=int64#4)
# asm 2: vmovupd   <r4=%ymm2,128(<ptr=%rcx)
vmovupd   % ymm2, 128( % rcx)

# qhasm: mem256[ ptr + 96 ] = r3
# asm 1: vmovupd   <r3=reg256#7,96(<ptr=int64#4)
# asm 2: vmovupd   <r3=%ymm6,96(<ptr=%rcx)
vmovupd   % ymm6, 96( % rcx)

# qhasm: mem256[ ptr + 64 ] = r2
# asm 1: vmovupd   <r2=reg256#2,64(<ptr=int64#4)
# asm 2: vmovupd   <r2=%ymm1,64(<ptr=%rcx)
vmovupd   % ymm1, 64( % rcx)

# qhasm: mem256[ ptr + 32 ] = r1
# asm 1: vmovupd   <r1=reg256#9,32(<ptr=int64#4)
# asm 2: vmovupd   <r1=%ymm8,32(<ptr=%rcx)
vmovupd   % ymm8, 32( % rcx)

# qhasm: mem256[ ptr + 0 ] = r0
# asm 1: vmovupd   <r0=reg256#1,0(<ptr=int64#4)
# asm 2: vmovupd   <r0=%ymm0,0(<ptr=%rcx)
vmovupd   % ymm0, 0( % rcx)

# qhasm: h22 = mem64[ ptr + 344 ]
# asm 1: movq   344(<ptr=int64#4),>h22=int64#2
# asm 2: movq   344(<ptr=%rcx),>h22=%rsi
movq   344( % rcx), % rsi

# qhasm: h13 = h22
# asm 1: mov  <h22=int64#2,>h13=int64#3
# asm 2: mov  <h22=%rsi,>h13=%rdx
mov  % rsi, % rdx

# qhasm: h10 = h22
# asm 1: mov  <h22=int64#2,>h10=int64#2
# asm 2: mov  <h22=%rsi,>h10=%rsi
mov  % rsi, % rsi

# qhasm: h21 = mem64[ ptr + 336 ]
# asm 1: movq   336(<ptr=int64#4),>h21=int64#5
# asm 2: movq   336(<ptr=%rcx),>h21=%r8
movq   336( % rcx), % r8

# qhasm: h21 ^= *(uint64 *) ( ptr + 328 )
# asm 1: xorq 328(<ptr=int64#4),<h21=int64#5
# asm 2: xorq 328(<ptr=%rcx),<h21=%r8
xorq 328( % rcx), % r8

# qhasm: h12 = h21
# asm 1: mov  <h21=int64#5,>h12=int64#6
# asm 2: mov  <h21=%r8,>h12=%r9
mov  % r8, % r9

# qhasm: h9 = h21
# asm 1: mov  <h21=int64#5,>h9=int64#5
# asm 2: mov  <h21=%r8,>h9=%r8
mov  % r8, % r8

# qhasm: h20 = mem64[ ptr + 312 ]
# asm 1: movq   312(<ptr=int64#4),>h20=int64#7
# asm 2: movq   312(<ptr=%rcx),>h20=%rax
movq   312( % rcx), % rax

# qhasm: h20 ^= *(uint64 *) ( ptr + 320 )
# asm 1: xorq 320(<ptr=int64#4),<h20=int64#7
# asm 2: xorq 320(<ptr=%rcx),<h20=%rax
xorq 320( % rcx), % rax

# qhasm: h11 = h20
# asm 1: mov  <h20=int64#7,>h11=int64#8
# asm 2: mov  <h20=%rax,>h11=%r10
mov  % rax, % r10

# qhasm: h8 = h20
# asm 1: mov  <h20=int64#7,>h8=int64#7
# asm 2: mov  <h20=%rax,>h8=%rax
mov  % rax, % rax

# qhasm: h19 = mem64[ ptr + 304 ]
# asm 1: movq   304(<ptr=int64#4),>h19=int64#9
# asm 2: movq   304(<ptr=%rcx),>h19=%r11
movq   304( % rcx), % r11

# qhasm: h19 ^= *(uint64 *) ( ptr + 296 )
# asm 1: xorq 296(<ptr=int64#4),<h19=int64#9
# asm 2: xorq 296(<ptr=%rcx),<h19=%r11
xorq 296( % rcx), % r11

# qhasm: h10 ^= h19
# asm 1: xor  <h19=int64#9,<h10=int64#2
# asm 2: xor  <h19=%r11,<h10=%rsi
xor  % r11, % rsi

# qhasm: h7 = h19
# asm 1: mov  <h19=int64#9,>h7=int64#9
# asm 2: mov  <h19=%r11,>h7=%r11
mov  % r11, % r11

# qhasm: h18 = mem64[ ptr + 280 ]
# asm 1: movq   280(<ptr=int64#4),>h18=int64#10
# asm 2: movq   280(<ptr=%rcx),>h18=%r12
movq   280( % rcx), % r12

# qhasm: h18 ^= *(uint64 *) ( ptr + 288 )
# asm 1: xorq 288(<ptr=int64#4),<h18=int64#10
# asm 2: xorq 288(<ptr=%rcx),<h18=%r12
xorq 288( % rcx), % r12

# qhasm: h9 ^= h18
# asm 1: xor  <h18=int64#10,<h9=int64#5
# asm 2: xor  <h18=%r12,<h9=%r8
xor  % r12, % r8

# qhasm: h6 = h18
# asm 1: mov  <h18=int64#10,>h6=int64#10
# asm 2: mov  <h18=%r12,>h6=%r12
mov  % r12, % r12

# qhasm: h17 = mem64[ ptr + 272 ]
# asm 1: movq   272(<ptr=int64#4),>h17=int64#11
# asm 2: movq   272(<ptr=%rcx),>h17=%r13
movq   272( % rcx), % r13

# qhasm: h17 ^= *(uint64 *) ( ptr + 264 )
# asm 1: xorq 264(<ptr=int64#4),<h17=int64#11
# asm 2: xorq 264(<ptr=%rcx),<h17=%r13
xorq 264( % rcx), % r13

# qhasm: h8 ^= h17
# asm 1: xor  <h17=int64#11,<h8=int64#7
# asm 2: xor  <h17=%r13,<h8=%rax
xor  % r13, % rax

# qhasm: h5 = h17
# asm 1: mov  <h17=int64#11,>h5=int64#11
# asm 2: mov  <h17=%r13,>h5=%r13
mov  % r13, % r13

# qhasm: h16 = mem64[ ptr + 248 ]
# asm 1: movq   248(<ptr=int64#4),>h16=int64#12
# asm 2: movq   248(<ptr=%rcx),>h16=%r14
movq   248( % rcx), % r14

# qhasm: h16 ^= *(uint64 *) ( ptr + 256 )
# asm 1: xorq 256(<ptr=int64#4),<h16=int64#12
# asm 2: xorq 256(<ptr=%rcx),<h16=%r14
xorq 256( % rcx), % r14

# qhasm: h7 ^= h16
# asm 1: xor  <h16=int64#12,<h7=int64#9
# asm 2: xor  <h16=%r14,<h7=%r11
xor  % r14, % r11

# qhasm: h4 = h16
# asm 1: mov  <h16=int64#12,>h4=int64#12
# asm 2: mov  <h16=%r14,>h4=%r14
mov  % r14, % r14

# qhasm: h15 = mem64[ ptr + 240 ]
# asm 1: movq   240(<ptr=int64#4),>h15=int64#13
# asm 2: movq   240(<ptr=%rcx),>h15=%r15
movq   240( % rcx), % r15

# qhasm: h15 ^= *(uint64 *) ( ptr + 232 )
# asm 1: xorq 232(<ptr=int64#4),<h15=int64#13
# asm 2: xorq 232(<ptr=%rcx),<h15=%r15
xorq 232( % rcx), % r15

# qhasm: h6 ^= h15
# asm 1: xor  <h15=int64#13,<h6=int64#10
# asm 2: xor  <h15=%r15,<h6=%r12
xor  % r15, % r12

# qhasm: h3 = h15
# asm 1: mov  <h15=int64#13,>h3=int64#13
# asm 2: mov  <h15=%r15,>h3=%r15
mov  % r15, % r15

# qhasm: h14 = mem64[ ptr + 216 ]
# asm 1: movq   216(<ptr=int64#4),>h14=int64#14
# asm 2: movq   216(<ptr=%rcx),>h14=%rbx
movq   216( % rcx), % rbx

# qhasm: h14 ^= *(uint64 *) ( ptr + 224 )
# asm 1: xorq 224(<ptr=int64#4),<h14=int64#14
# asm 2: xorq 224(<ptr=%rcx),<h14=%rbx
xorq 224( % rcx), % rbx

# qhasm: h5 ^= h14
# asm 1: xor  <h14=int64#14,<h5=int64#11
# asm 2: xor  <h14=%rbx,<h5=%r13
xor  % rbx, % r13

# qhasm: h2 = h14
# asm 1: mov  <h14=int64#14,>h2=int64#14
# asm 2: mov  <h14=%rbx,>h2=%rbx
mov  % rbx, % rbx

# qhasm: h13 ^= *(uint64 *) ( ptr + 208 )
# asm 1: xorq 208(<ptr=int64#4),<h13=int64#3
# asm 2: xorq 208(<ptr=%rcx),<h13=%rdx
xorq 208( % rcx), % rdx

# qhasm: h13 ^= *(uint64 *) ( ptr + 200 )
# asm 1: xorq 200(<ptr=int64#4),<h13=int64#3
# asm 2: xorq 200(<ptr=%rcx),<h13=%rdx
xorq 200( % rcx), % rdx

# qhasm: h4 ^= h13
# asm 1: xor  <h13=int64#3,<h4=int64#12
# asm 2: xor  <h13=%rdx,<h4=%r14
xor  % rdx, % r14

# qhasm: h1 = h13
# asm 1: mov  <h13=int64#3,>h1=int64#3
# asm 2: mov  <h13=%rdx,>h1=%rdx
mov  % rdx, % rdx

# qhasm: h12 ^= *(uint64 *) ( ptr + 184 )
# asm 1: xorq 184(<ptr=int64#4),<h12=int64#6
# asm 2: xorq 184(<ptr=%rcx),<h12=%r9
xorq 184( % rcx), % r9

# qhasm: h12 ^= *(uint64 *) ( ptr + 192 )
# asm 1: xorq 192(<ptr=int64#4),<h12=int64#6
# asm 2: xorq 192(<ptr=%rcx),<h12=%r9
xorq 192( % rcx), % r9

# qhasm: h3 ^= h12
# asm 1: xor  <h12=int64#6,<h3=int64#13
# asm 2: xor  <h12=%r9,<h3=%r15
xor  % r9, % r15

# qhasm: h0 = h12
# asm 1: mov  <h12=int64#6,>h0=int64#6
# asm 2: mov  <h12=%r9,>h0=%r9
mov  % r9, % r9

# qhasm: h11 ^= *(uint64 *) ( ptr + 176 )
# asm 1: xorq 176(<ptr=int64#4),<h11=int64#8
# asm 2: xorq 176(<ptr=%rcx),<h11=%r10
xorq 176( % rcx), % r10

# qhasm: h11 ^= *(uint64 *) ( ptr + 168 )
# asm 1: xorq 168(<ptr=int64#4),<h11=int64#8
# asm 2: xorq 168(<ptr=%rcx),<h11=%r10
xorq 168( % rcx), % r10

# qhasm: mem64[ input_0 + 88 ] = h11
# asm 1: movq   <h11=int64#8,88(<input_0=int64#1)
# asm 2: movq   <h11=%r10,88(<input_0=%rdi)
movq   % r10, 88( % rdi)

# qhasm: h10 ^= *(uint64 *) ( ptr + 152 )
# asm 1: xorq 152(<ptr=int64#4),<h10=int64#2
# asm 2: xorq 152(<ptr=%rcx),<h10=%rsi
xorq 152( % rcx), % rsi

# qhasm: h10 ^= *(uint64 *) ( ptr + 160 )
# asm 1: xorq 160(<ptr=int64#4),<h10=int64#2
# asm 2: xorq 160(<ptr=%rcx),<h10=%rsi
xorq 160( % rcx), % rsi

# qhasm: mem64[ input_0 + 80 ] = h10
# asm 1: movq   <h10=int64#2,80(<input_0=int64#1)
# asm 2: movq   <h10=%rsi,80(<input_0=%rdi)
movq   % rsi, 80( % rdi)

# qhasm: h9 ^= *(uint64 *) ( ptr + 144 )
# asm 1: xorq 144(<ptr=int64#4),<h9=int64#5
# asm 2: xorq 144(<ptr=%rcx),<h9=%r8
xorq 144( % rcx), % r8

# qhasm: h9 ^= *(uint64 *) ( ptr + 136 )
# asm 1: xorq 136(<ptr=int64#4),<h9=int64#5
# asm 2: xorq 136(<ptr=%rcx),<h9=%r8
xorq 136( % rcx), % r8

# qhasm: mem64[ input_0 + 72 ] = h9
# asm 1: movq   <h9=int64#5,72(<input_0=int64#1)
# asm 2: movq   <h9=%r8,72(<input_0=%rdi)
movq   % r8, 72( % rdi)

# qhasm: h8 ^= *(uint64 *) ( ptr + 120 )
# asm 1: xorq 120(<ptr=int64#4),<h8=int64#7
# asm 2: xorq 120(<ptr=%rcx),<h8=%rax
xorq 120( % rcx), % rax

# qhasm: h8 ^= *(uint64 *) ( ptr + 128 )
# asm 1: xorq 128(<ptr=int64#4),<h8=int64#7
# asm 2: xorq 128(<ptr=%rcx),<h8=%rax
xorq 128( % rcx), % rax

# qhasm: mem64[ input_0 + 64 ] = h8
# asm 1: movq   <h8=int64#7,64(<input_0=int64#1)
# asm 2: movq   <h8=%rax,64(<input_0=%rdi)
movq   % rax, 64( % rdi)

# qhasm: h7 ^= *(uint64 *) ( ptr + 112 )
# asm 1: xorq 112(<ptr=int64#4),<h7=int64#9
# asm 2: xorq 112(<ptr=%rcx),<h7=%r11
xorq 112( % rcx), % r11

# qhasm: h7 ^= *(uint64 *) ( ptr + 104 )
# asm 1: xorq 104(<ptr=int64#4),<h7=int64#9
# asm 2: xorq 104(<ptr=%rcx),<h7=%r11
xorq 104( % rcx), % r11

# qhasm: mem64[ input_0 + 56 ] = h7
# asm 1: movq   <h7=int64#9,56(<input_0=int64#1)
# asm 2: movq   <h7=%r11,56(<input_0=%rdi)
movq   % r11, 56( % rdi)

# qhasm: h6 ^= *(uint64 *) ( ptr + 88 )
# asm 1: xorq 88(<ptr=int64#4),<h6=int64#10
# asm 2: xorq 88(<ptr=%rcx),<h6=%r12
xorq 88( % rcx), % r12

# qhasm: h6 ^= *(uint64 *) ( ptr + 96 )
# asm 1: xorq 96(<ptr=int64#4),<h6=int64#10
# asm 2: xorq 96(<ptr=%rcx),<h6=%r12
xorq 96( % rcx), % r12

# qhasm: mem64[ input_0 + 48 ] = h6
# asm 1: movq   <h6=int64#10,48(<input_0=int64#1)
# asm 2: movq   <h6=%r12,48(<input_0=%rdi)
movq   % r12, 48( % rdi)

# qhasm: h5 ^= *(uint64 *) ( ptr + 80 )
# asm 1: xorq 80(<ptr=int64#4),<h5=int64#11
# asm 2: xorq 80(<ptr=%rcx),<h5=%r13
xorq 80( % rcx), % r13

# qhasm: h5 ^= *(uint64 *) ( ptr + 72 )
# asm 1: xorq 72(<ptr=int64#4),<h5=int64#11
# asm 2: xorq 72(<ptr=%rcx),<h5=%r13
xorq 72( % rcx), % r13

# qhasm: mem64[ input_0 + 40 ] = h5
# asm 1: movq   <h5=int64#11,40(<input_0=int64#1)
# asm 2: movq   <h5=%r13,40(<input_0=%rdi)
movq   % r13, 40( % rdi)

# qhasm: h4 ^= *(uint64 *) ( ptr + 56 )
# asm 1: xorq 56(<ptr=int64#4),<h4=int64#12
# asm 2: xorq 56(<ptr=%rcx),<h4=%r14
xorq 56( % rcx), % r14

# qhasm: h4 ^= *(uint64 *) ( ptr + 64 )
# asm 1: xorq 64(<ptr=int64#4),<h4=int64#12
# asm 2: xorq 64(<ptr=%rcx),<h4=%r14
xorq 64( % rcx), % r14

# qhasm: mem64[ input_0 + 32 ] = h4
# asm 1: movq   <h4=int64#12,32(<input_0=int64#1)
# asm 2: movq   <h4=%r14,32(<input_0=%rdi)
movq   % r14, 32( % rdi)

# qhasm: h3 ^= *(uint64 *) ( ptr + 48 )
# asm 1: xorq 48(<ptr=int64#4),<h3=int64#13
# asm 2: xorq 48(<ptr=%rcx),<h3=%r15
xorq 48( % rcx), % r15

# qhasm: h3 ^= *(uint64 *) ( ptr + 40 )
# asm 1: xorq 40(<ptr=int64#4),<h3=int64#13
# asm 2: xorq 40(<ptr=%rcx),<h3=%r15
xorq 40( % rcx), % r15

# qhasm: mem64[ input_0 + 24 ] = h3
# asm 1: movq   <h3=int64#13,24(<input_0=int64#1)
# asm 2: movq   <h3=%r15,24(<input_0=%rdi)
movq   % r15, 24( % rdi)

# qhasm: h2 ^= *(uint64 *) ( ptr + 24 )
# asm 1: xorq 24(<ptr=int64#4),<h2=int64#14
# asm 2: xorq 24(<ptr=%rcx),<h2=%rbx
xorq 24( % rcx), % rbx

# qhasm: h2 ^= *(uint64 *) ( ptr + 32 )
# asm 1: xorq 32(<ptr=int64#4),<h2=int64#14
# asm 2: xorq 32(<ptr=%rcx),<h2=%rbx
xorq 32( % rcx), % rbx

# qhasm: mem64[ input_0 + 16 ] = h2
# asm 1: movq   <h2=int64#14,16(<input_0=int64#1)
# asm 2: movq   <h2=%rbx,16(<input_0=%rdi)
movq   % rbx, 16( % rdi)

# qhasm: h1 ^= *(uint64 *) ( ptr + 16 )
# asm 1: xorq 16(<ptr=int64#4),<h1=int64#3
# asm 2: xorq 16(<ptr=%rcx),<h1=%rdx
xorq 16( % rcx), % rdx

# qhasm: h1 ^= *(uint64 *) ( ptr + 8 )
# asm 1: xorq 8(<ptr=int64#4),<h1=int64#3
# asm 2: xorq 8(<ptr=%rcx),<h1=%rdx
xorq 8( % rcx), % rdx

# qhasm: mem64[ input_0 + 8 ] = h1
# asm 1: movq   <h1=int64#3,8(<input_0=int64#1)
# asm 2: movq   <h1=%rdx,8(<input_0=%rdi)
movq   % rdx, 8( % rdi)

# qhasm: h0 ^= *(uint64 *) ( ptr + 0 )
# asm 1: xorq 0(<ptr=int64#4),<h0=int64#6
# asm 2: xorq 0(<ptr=%rcx),<h0=%r9
xorq 0( % rcx), % r9

# qhasm: mem64[ input_0 + 0 ] = h0
# asm 1: movq   <h0=int64#6,0(<input_0=int64#1)
# asm 2: movq   <h0=%r9,0(<input_0=%rdi)
movq   % r9, 0( % rdi)

# qhasm: caller_r11 = r11_stack
# asm 1: movq <r11_stack=stack64#1,>caller_r11=int64#9
# asm 2: movq <r11_stack=608(%rsp),>caller_r11=%r11
movq 608( % rsp), % r11

# qhasm: caller_r12 = r12_stack
# asm 1: movq <r12_stack=stack64#2,>caller_r12=int64#10
# asm 2: movq <r12_stack=616(%rsp),>caller_r12=%r12
movq 616( % rsp), % r12

# qhasm: caller_r13 = r13_stack
# asm 1: movq <r13_stack=stack64#3,>caller_r13=int64#11
# asm 2: movq <r13_stack=624(%rsp),>caller_r13=%r13
movq 624( % rsp), % r13

# qhasm: caller_r14 = r14_stack
# asm 1: movq <r14_stack=stack64#4,>caller_r14=int64#12
# asm 2: movq <r14_stack=632(%rsp),>caller_r14=%r14
movq 632( % rsp), % r14

# qhasm: caller_r15 = r15_stack
# asm 1: movq <r15_stack=stack64#5,>caller_r15=int64#13
# asm 2: movq <r15_stack=640(%rsp),>caller_r15=%r15
movq 640( % rsp), % r15

# qhasm: caller_rbx = rbx_stack
# asm 1: movq <rbx_stack=stack64#6,>caller_rbx=int64#14
# asm 2: movq <rbx_stack=648(%rsp),>caller_rbx=%rbx
movq 648( % rsp), % rbx

# qhasm: return
add % r11, % rsp
ret
