
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

# qhasm: reg128 b0

# qhasm: reg128 b1

# qhasm: reg128 b2

# qhasm: reg128 b3

# qhasm: reg128 b4

# qhasm: reg128 b5

# qhasm: reg128 b6

# qhasm: reg128 b7

# qhasm: reg128 b8

# qhasm: reg128 b9

# qhasm: reg128 b10

# qhasm: reg128 b11

# qhasm: reg128 a0

# qhasm: reg128 a1

# qhasm: reg128 a2

# qhasm: reg128 a3

# qhasm: reg128 a4

# qhasm: reg128 a5

# qhasm: reg128 r0

# qhasm: reg128 r1

# qhasm: reg128 r2

# qhasm: reg128 r3

# qhasm: reg128 r4

# qhasm: reg128 r5

# qhasm: reg128 r6

# qhasm: reg128 r7

# qhasm: reg128 r8

# qhasm: reg128 r9

# qhasm: reg128 r10

# qhasm: reg128 r11

# qhasm: reg128 r12

# qhasm: reg128 r13

# qhasm: reg128 r14

# qhasm: reg128 r15

# qhasm: reg128 r16

# qhasm: reg128 r17

# qhasm: reg128 r18

# qhasm: reg128 r19

# qhasm: reg128 r20

# qhasm: reg128 r21

# qhasm: reg128 r22

# qhasm: reg128 r

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

# qhasm: stack2432 buf

# qhasm: int64 ptr

# qhasm: int64 tmp

# qhasm: stack64 r11_stack

# qhasm: stack64 r12_stack

# qhasm: stack64 r13_stack

# qhasm: stack64 r14_stack

# qhasm: stack64 r15_stack

# qhasm: stack64 rbx_stack

# qhasm: stack64 rbp_stack

# qhasm: enter vec_mul_asm
.p2align 5
.global _PQCLEAN_MCELIECE348864_SSE_vec_mul_asm
.global PQCLEAN_MCELIECE348864_SSE_vec_mul_asm
_PQCLEAN_MCELIECE348864_SSE_vec_mul_asm:
PQCLEAN_MCELIECE348864_SSE_vec_mul_asm:
mov %rsp,%r11
and $31,%r11
add $672,%r11
sub %r11,%rsp

# qhasm: r11_stack = caller_r11
# asm 1: movq <caller_r11=int64#9,>r11_stack=stack64#1
# asm 2: movq <caller_r11=%r11,>r11_stack=608(%rsp)
movq %r11,608(%rsp)

# qhasm: r12_stack = caller_r12
# asm 1: movq <caller_r12=int64#10,>r12_stack=stack64#2
# asm 2: movq <caller_r12=%r12,>r12_stack=616(%rsp)
movq %r12,616(%rsp)

# qhasm: r13_stack = caller_r13
# asm 1: movq <caller_r13=int64#11,>r13_stack=stack64#3
# asm 2: movq <caller_r13=%r13,>r13_stack=624(%rsp)
movq %r13,624(%rsp)

# qhasm: r14_stack = caller_r14
# asm 1: movq <caller_r14=int64#12,>r14_stack=stack64#4
# asm 2: movq <caller_r14=%r14,>r14_stack=632(%rsp)
movq %r14,632(%rsp)

# qhasm: r15_stack = caller_r15
# asm 1: movq <caller_r15=int64#13,>r15_stack=stack64#5
# asm 2: movq <caller_r15=%r15,>r15_stack=640(%rsp)
movq %r15,640(%rsp)

# qhasm: rbx_stack = caller_rbx
# asm 1: movq <caller_rbx=int64#14,>rbx_stack=stack64#6
# asm 2: movq <caller_rbx=%rbx,>rbx_stack=648(%rsp)
movq %rbx,648(%rsp)

# qhasm: ptr = &buf
# asm 1: leaq <buf=stack2432#1,>ptr=int64#5
# asm 2: leaq <buf=0(%rsp),>ptr=%r8
leaq 0(%rsp),%r8

# qhasm: tmp = input_3
# asm 1: mov  <input_3=int64#4,>tmp=int64#6
# asm 2: mov  <input_3=%rcx,>tmp=%r9
mov  %rcx,%r9

# qhasm: tmp *= 11
# asm 1: imulq  $11,<tmp=int64#6,>tmp=int64#6
# asm 2: imulq  $11,<tmp=%r9,>tmp=%r9
imulq  $11,%r9,%r9

# qhasm: input_2 += tmp
# asm 1: add  <tmp=int64#6,<input_2=int64#3
# asm 2: add  <tmp=%r9,<input_2=%rdx
add  %r9,%rdx

# qhasm: b11 = mem64[ input_2 + 0 ] x2
# asm 1: movddup 0(<input_2=int64#3),>b11=reg128#1
# asm 2: movddup 0(<input_2=%rdx),>b11=%xmm0
movddup 0(%rdx),%xmm0

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  %rcx,%rdx

# qhasm: a5[0] = mem64[ input_1 + 40 ]
# asm 1: pinsrq $0x0,40(<input_1=int64#2),<a5=reg128#2
# asm 2: pinsrq $0x0,40(<input_1=%rsi),<a5=%xmm1
pinsrq $0x0,40(%rsi),%xmm1

# qhasm: a5[1] = mem64[ input_1 + 88 ]
# asm 1: pinsrq $0x1,88(<input_1=int64#2),<a5=reg128#2
# asm 2: pinsrq $0x1,88(<input_1=%rsi),<a5=%xmm1
pinsrq $0x1,88(%rsi),%xmm1

# qhasm: r16 = b11 & a5
# asm 1: vpand <a5=reg128#2,<b11=reg128#1,>r16=reg128#3
# asm 2: vpand <a5=%xmm1,<b11=%xmm0,>r16=%xmm2
vpand %xmm1,%xmm0,%xmm2

# qhasm: mem128[ ptr + 256 ] = r16
# asm 1: movdqu   <r16=reg128#3,256(<ptr=int64#5)
# asm 2: movdqu   <r16=%xmm2,256(<ptr=%r8)
movdqu   %xmm2,256(%r8)

# qhasm: a4[0] = mem64[ input_1 + 32 ]
# asm 1: pinsrq $0x0,32(<input_1=int64#2),<a4=reg128#3
# asm 2: pinsrq $0x0,32(<input_1=%rsi),<a4=%xmm2
pinsrq $0x0,32(%rsi),%xmm2

# qhasm: a4[1] = mem64[ input_1 + 80 ]
# asm 1: pinsrq $0x1,80(<input_1=int64#2),<a4=reg128#3
# asm 2: pinsrq $0x1,80(<input_1=%rsi),<a4=%xmm2
pinsrq $0x1,80(%rsi),%xmm2

# qhasm: r15 = b11 & a4
# asm 1: vpand <a4=reg128#3,<b11=reg128#1,>r15=reg128#4
# asm 2: vpand <a4=%xmm2,<b11=%xmm0,>r15=%xmm3
vpand %xmm2,%xmm0,%xmm3

# qhasm: a3[0] = mem64[ input_1 + 24 ]
# asm 1: pinsrq $0x0,24(<input_1=int64#2),<a3=reg128#5
# asm 2: pinsrq $0x0,24(<input_1=%rsi),<a3=%xmm4
pinsrq $0x0,24(%rsi),%xmm4

# qhasm: a3[1] = mem64[ input_1 + 72 ]
# asm 1: pinsrq $0x1,72(<input_1=int64#2),<a3=reg128#5
# asm 2: pinsrq $0x1,72(<input_1=%rsi),<a3=%xmm4
pinsrq $0x1,72(%rsi),%xmm4

# qhasm: r14 = b11 & a3
# asm 1: vpand <a3=reg128#5,<b11=reg128#1,>r14=reg128#6
# asm 2: vpand <a3=%xmm4,<b11=%xmm0,>r14=%xmm5
vpand %xmm4,%xmm0,%xmm5

# qhasm: a2[0] = mem64[ input_1 + 16 ]
# asm 1: pinsrq $0x0,16(<input_1=int64#2),<a2=reg128#7
# asm 2: pinsrq $0x0,16(<input_1=%rsi),<a2=%xmm6
pinsrq $0x0,16(%rsi),%xmm6

# qhasm: a2[1] = mem64[ input_1 + 64 ]
# asm 1: pinsrq $0x1,64(<input_1=int64#2),<a2=reg128#7
# asm 2: pinsrq $0x1,64(<input_1=%rsi),<a2=%xmm6
pinsrq $0x1,64(%rsi),%xmm6

# qhasm: r13 = b11 & a2
# asm 1: vpand <a2=reg128#7,<b11=reg128#1,>r13=reg128#8
# asm 2: vpand <a2=%xmm6,<b11=%xmm0,>r13=%xmm7
vpand %xmm6,%xmm0,%xmm7

# qhasm: a1[0] = mem64[ input_1 + 8 ]
# asm 1: pinsrq $0x0,8(<input_1=int64#2),<a1=reg128#9
# asm 2: pinsrq $0x0,8(<input_1=%rsi),<a1=%xmm8
pinsrq $0x0,8(%rsi),%xmm8

# qhasm: a1[1] = mem64[ input_1 + 56 ]
# asm 1: pinsrq $0x1,56(<input_1=int64#2),<a1=reg128#9
# asm 2: pinsrq $0x1,56(<input_1=%rsi),<a1=%xmm8
pinsrq $0x1,56(%rsi),%xmm8

# qhasm: r12 = b11 & a1
# asm 1: vpand <a1=reg128#9,<b11=reg128#1,>r12=reg128#10
# asm 2: vpand <a1=%xmm8,<b11=%xmm0,>r12=%xmm9
vpand %xmm8,%xmm0,%xmm9

# qhasm: a0[0] = mem64[ input_1 + 0 ]
# asm 1: pinsrq $0x0,0(<input_1=int64#2),<a0=reg128#11
# asm 2: pinsrq $0x0,0(<input_1=%rsi),<a0=%xmm10
pinsrq $0x0,0(%rsi),%xmm10

# qhasm: a0[1] = mem64[ input_1 + 48 ]
# asm 1: pinsrq $0x1,48(<input_1=int64#2),<a0=reg128#11
# asm 2: pinsrq $0x1,48(<input_1=%rsi),<a0=%xmm10
pinsrq $0x1,48(%rsi),%xmm10

# qhasm: r11 = b11 & a0
# asm 1: vpand <a0=reg128#11,<b11=reg128#1,>r11=reg128#1
# asm 2: vpand <a0=%xmm10,<b11=%xmm0,>r11=%xmm0
vpand %xmm10,%xmm0,%xmm0

# qhasm: b10 = mem64[ input_2 + 0 ] x2
# asm 1: movddup 0(<input_2=int64#3),>b10=reg128#12
# asm 2: movddup 0(<input_2=%rdx),>b10=%xmm11
movddup 0(%rdx),%xmm11

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  %rcx,%rdx

# qhasm: r = b10 & a5
# asm 1: vpand <a5=reg128#2,<b10=reg128#12,>r=reg128#13
# asm 2: vpand <a5=%xmm1,<b10=%xmm11,>r=%xmm12
vpand %xmm1,%xmm11,%xmm12

# qhasm: r15 ^= r
# asm 1: pxor <r=reg128#13,<r15=reg128#4
# asm 2: pxor <r=%xmm12,<r15=%xmm3
pxor %xmm12,%xmm3

# qhasm: mem128[ ptr + 240 ] = r15
# asm 1: movdqu   <r15=reg128#4,240(<ptr=int64#5)
# asm 2: movdqu   <r15=%xmm3,240(<ptr=%r8)
movdqu   %xmm3,240(%r8)

# qhasm: r = b10 & a4
# asm 1: vpand <a4=reg128#3,<b10=reg128#12,>r=reg128#4
# asm 2: vpand <a4=%xmm2,<b10=%xmm11,>r=%xmm3
vpand %xmm2,%xmm11,%xmm3

# qhasm: r14 ^= r
# asm 1: pxor <r=reg128#4,<r14=reg128#6
# asm 2: pxor <r=%xmm3,<r14=%xmm5
pxor %xmm3,%xmm5

# qhasm: r = b10 & a3
# asm 1: vpand <a3=reg128#5,<b10=reg128#12,>r=reg128#4
# asm 2: vpand <a3=%xmm4,<b10=%xmm11,>r=%xmm3
vpand %xmm4,%xmm11,%xmm3

# qhasm: r13 ^= r
# asm 1: pxor <r=reg128#4,<r13=reg128#8
# asm 2: pxor <r=%xmm3,<r13=%xmm7
pxor %xmm3,%xmm7

# qhasm: r = b10 & a2
# asm 1: vpand <a2=reg128#7,<b10=reg128#12,>r=reg128#4
# asm 2: vpand <a2=%xmm6,<b10=%xmm11,>r=%xmm3
vpand %xmm6,%xmm11,%xmm3

# qhasm: r12 ^= r
# asm 1: pxor <r=reg128#4,<r12=reg128#10
# asm 2: pxor <r=%xmm3,<r12=%xmm9
pxor %xmm3,%xmm9

# qhasm: r = b10 & a1
# asm 1: vpand <a1=reg128#9,<b10=reg128#12,>r=reg128#4
# asm 2: vpand <a1=%xmm8,<b10=%xmm11,>r=%xmm3
vpand %xmm8,%xmm11,%xmm3

# qhasm: r11 ^= r
# asm 1: pxor <r=reg128#4,<r11=reg128#1
# asm 2: pxor <r=%xmm3,<r11=%xmm0
pxor %xmm3,%xmm0

# qhasm: r10 = b10 & a0
# asm 1: vpand <a0=reg128#11,<b10=reg128#12,>r10=reg128#4
# asm 2: vpand <a0=%xmm10,<b10=%xmm11,>r10=%xmm3
vpand %xmm10,%xmm11,%xmm3

# qhasm: b9 = mem64[ input_2 + 0 ] x2
# asm 1: movddup 0(<input_2=int64#3),>b9=reg128#12
# asm 2: movddup 0(<input_2=%rdx),>b9=%xmm11
movddup 0(%rdx),%xmm11

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  %rcx,%rdx

# qhasm: r = b9 & a5
# asm 1: vpand <a5=reg128#2,<b9=reg128#12,>r=reg128#13
# asm 2: vpand <a5=%xmm1,<b9=%xmm11,>r=%xmm12
vpand %xmm1,%xmm11,%xmm12

# qhasm: r14 ^= r
# asm 1: pxor <r=reg128#13,<r14=reg128#6
# asm 2: pxor <r=%xmm12,<r14=%xmm5
pxor %xmm12,%xmm5

# qhasm: mem128[ ptr + 224 ] = r14
# asm 1: movdqu   <r14=reg128#6,224(<ptr=int64#5)
# asm 2: movdqu   <r14=%xmm5,224(<ptr=%r8)
movdqu   %xmm5,224(%r8)

# qhasm: r = b9 & a4
# asm 1: vpand <a4=reg128#3,<b9=reg128#12,>r=reg128#6
# asm 2: vpand <a4=%xmm2,<b9=%xmm11,>r=%xmm5
vpand %xmm2,%xmm11,%xmm5

# qhasm: r13 ^= r
# asm 1: pxor <r=reg128#6,<r13=reg128#8
# asm 2: pxor <r=%xmm5,<r13=%xmm7
pxor %xmm5,%xmm7

# qhasm: r = b9 & a3
# asm 1: vpand <a3=reg128#5,<b9=reg128#12,>r=reg128#6
# asm 2: vpand <a3=%xmm4,<b9=%xmm11,>r=%xmm5
vpand %xmm4,%xmm11,%xmm5

# qhasm: r12 ^= r
# asm 1: pxor <r=reg128#6,<r12=reg128#10
# asm 2: pxor <r=%xmm5,<r12=%xmm9
pxor %xmm5,%xmm9

# qhasm: r = b9 & a2
# asm 1: vpand <a2=reg128#7,<b9=reg128#12,>r=reg128#6
# asm 2: vpand <a2=%xmm6,<b9=%xmm11,>r=%xmm5
vpand %xmm6,%xmm11,%xmm5

# qhasm: r11 ^= r
# asm 1: pxor <r=reg128#6,<r11=reg128#1
# asm 2: pxor <r=%xmm5,<r11=%xmm0
pxor %xmm5,%xmm0

# qhasm: r = b9 & a1
# asm 1: vpand <a1=reg128#9,<b9=reg128#12,>r=reg128#6
# asm 2: vpand <a1=%xmm8,<b9=%xmm11,>r=%xmm5
vpand %xmm8,%xmm11,%xmm5

# qhasm: r10 ^= r
# asm 1: pxor <r=reg128#6,<r10=reg128#4
# asm 2: pxor <r=%xmm5,<r10=%xmm3
pxor %xmm5,%xmm3

# qhasm: r9 = b9 & a0
# asm 1: vpand <a0=reg128#11,<b9=reg128#12,>r9=reg128#6
# asm 2: vpand <a0=%xmm10,<b9=%xmm11,>r9=%xmm5
vpand %xmm10,%xmm11,%xmm5

# qhasm: b8 = mem64[ input_2 + 0 ] x2
# asm 1: movddup 0(<input_2=int64#3),>b8=reg128#12
# asm 2: movddup 0(<input_2=%rdx),>b8=%xmm11
movddup 0(%rdx),%xmm11

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  %rcx,%rdx

# qhasm: r = b8 & a5
# asm 1: vpand <a5=reg128#2,<b8=reg128#12,>r=reg128#13
# asm 2: vpand <a5=%xmm1,<b8=%xmm11,>r=%xmm12
vpand %xmm1,%xmm11,%xmm12

# qhasm: r13 ^= r
# asm 1: pxor <r=reg128#13,<r13=reg128#8
# asm 2: pxor <r=%xmm12,<r13=%xmm7
pxor %xmm12,%xmm7

# qhasm: mem128[ ptr + 208 ] = r13
# asm 1: movdqu   <r13=reg128#8,208(<ptr=int64#5)
# asm 2: movdqu   <r13=%xmm7,208(<ptr=%r8)
movdqu   %xmm7,208(%r8)

# qhasm: r = b8 & a4
# asm 1: vpand <a4=reg128#3,<b8=reg128#12,>r=reg128#8
# asm 2: vpand <a4=%xmm2,<b8=%xmm11,>r=%xmm7
vpand %xmm2,%xmm11,%xmm7

# qhasm: r12 ^= r
# asm 1: pxor <r=reg128#8,<r12=reg128#10
# asm 2: pxor <r=%xmm7,<r12=%xmm9
pxor %xmm7,%xmm9

# qhasm: r = b8 & a3
# asm 1: vpand <a3=reg128#5,<b8=reg128#12,>r=reg128#8
# asm 2: vpand <a3=%xmm4,<b8=%xmm11,>r=%xmm7
vpand %xmm4,%xmm11,%xmm7

# qhasm: r11 ^= r
# asm 1: pxor <r=reg128#8,<r11=reg128#1
# asm 2: pxor <r=%xmm7,<r11=%xmm0
pxor %xmm7,%xmm0

# qhasm: r = b8 & a2
# asm 1: vpand <a2=reg128#7,<b8=reg128#12,>r=reg128#8
# asm 2: vpand <a2=%xmm6,<b8=%xmm11,>r=%xmm7
vpand %xmm6,%xmm11,%xmm7

# qhasm: r10 ^= r
# asm 1: pxor <r=reg128#8,<r10=reg128#4
# asm 2: pxor <r=%xmm7,<r10=%xmm3
pxor %xmm7,%xmm3

# qhasm: r = b8 & a1
# asm 1: vpand <a1=reg128#9,<b8=reg128#12,>r=reg128#8
# asm 2: vpand <a1=%xmm8,<b8=%xmm11,>r=%xmm7
vpand %xmm8,%xmm11,%xmm7

# qhasm: r9 ^= r
# asm 1: pxor <r=reg128#8,<r9=reg128#6
# asm 2: pxor <r=%xmm7,<r9=%xmm5
pxor %xmm7,%xmm5

# qhasm: r8 = b8 & a0
# asm 1: vpand <a0=reg128#11,<b8=reg128#12,>r8=reg128#8
# asm 2: vpand <a0=%xmm10,<b8=%xmm11,>r8=%xmm7
vpand %xmm10,%xmm11,%xmm7

# qhasm: b7 = mem64[ input_2 + 0 ] x2
# asm 1: movddup 0(<input_2=int64#3),>b7=reg128#12
# asm 2: movddup 0(<input_2=%rdx),>b7=%xmm11
movddup 0(%rdx),%xmm11

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  %rcx,%rdx

# qhasm: r = b7 & a5
# asm 1: vpand <a5=reg128#2,<b7=reg128#12,>r=reg128#13
# asm 2: vpand <a5=%xmm1,<b7=%xmm11,>r=%xmm12
vpand %xmm1,%xmm11,%xmm12

# qhasm: r12 ^= r
# asm 1: pxor <r=reg128#13,<r12=reg128#10
# asm 2: pxor <r=%xmm12,<r12=%xmm9
pxor %xmm12,%xmm9

# qhasm: mem128[ ptr + 192 ] = r12
# asm 1: movdqu   <r12=reg128#10,192(<ptr=int64#5)
# asm 2: movdqu   <r12=%xmm9,192(<ptr=%r8)
movdqu   %xmm9,192(%r8)

# qhasm: r = b7 & a4
# asm 1: vpand <a4=reg128#3,<b7=reg128#12,>r=reg128#10
# asm 2: vpand <a4=%xmm2,<b7=%xmm11,>r=%xmm9
vpand %xmm2,%xmm11,%xmm9

# qhasm: r11 ^= r
# asm 1: pxor <r=reg128#10,<r11=reg128#1
# asm 2: pxor <r=%xmm9,<r11=%xmm0
pxor %xmm9,%xmm0

# qhasm: r = b7 & a3
# asm 1: vpand <a3=reg128#5,<b7=reg128#12,>r=reg128#10
# asm 2: vpand <a3=%xmm4,<b7=%xmm11,>r=%xmm9
vpand %xmm4,%xmm11,%xmm9

# qhasm: r10 ^= r
# asm 1: pxor <r=reg128#10,<r10=reg128#4
# asm 2: pxor <r=%xmm9,<r10=%xmm3
pxor %xmm9,%xmm3

# qhasm: r = b7 & a2
# asm 1: vpand <a2=reg128#7,<b7=reg128#12,>r=reg128#10
# asm 2: vpand <a2=%xmm6,<b7=%xmm11,>r=%xmm9
vpand %xmm6,%xmm11,%xmm9

# qhasm: r9 ^= r
# asm 1: pxor <r=reg128#10,<r9=reg128#6
# asm 2: pxor <r=%xmm9,<r9=%xmm5
pxor %xmm9,%xmm5

# qhasm: r = b7 & a1
# asm 1: vpand <a1=reg128#9,<b7=reg128#12,>r=reg128#10
# asm 2: vpand <a1=%xmm8,<b7=%xmm11,>r=%xmm9
vpand %xmm8,%xmm11,%xmm9

# qhasm: r8 ^= r
# asm 1: pxor <r=reg128#10,<r8=reg128#8
# asm 2: pxor <r=%xmm9,<r8=%xmm7
pxor %xmm9,%xmm7

# qhasm: r7 = b7 & a0
# asm 1: vpand <a0=reg128#11,<b7=reg128#12,>r7=reg128#10
# asm 2: vpand <a0=%xmm10,<b7=%xmm11,>r7=%xmm9
vpand %xmm10,%xmm11,%xmm9

# qhasm: b6 = mem64[ input_2 + 0 ] x2
# asm 1: movddup 0(<input_2=int64#3),>b6=reg128#12
# asm 2: movddup 0(<input_2=%rdx),>b6=%xmm11
movddup 0(%rdx),%xmm11

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  %rcx,%rdx

# qhasm: r = b6 & a5
# asm 1: vpand <a5=reg128#2,<b6=reg128#12,>r=reg128#13
# asm 2: vpand <a5=%xmm1,<b6=%xmm11,>r=%xmm12
vpand %xmm1,%xmm11,%xmm12

# qhasm: r11 ^= r
# asm 1: pxor <r=reg128#13,<r11=reg128#1
# asm 2: pxor <r=%xmm12,<r11=%xmm0
pxor %xmm12,%xmm0

# qhasm: mem128[ ptr + 176 ] = r11
# asm 1: movdqu   <r11=reg128#1,176(<ptr=int64#5)
# asm 2: movdqu   <r11=%xmm0,176(<ptr=%r8)
movdqu   %xmm0,176(%r8)

# qhasm: r = b6 & a4
# asm 1: vpand <a4=reg128#3,<b6=reg128#12,>r=reg128#1
# asm 2: vpand <a4=%xmm2,<b6=%xmm11,>r=%xmm0
vpand %xmm2,%xmm11,%xmm0

# qhasm: r10 ^= r
# asm 1: pxor <r=reg128#1,<r10=reg128#4
# asm 2: pxor <r=%xmm0,<r10=%xmm3
pxor %xmm0,%xmm3

# qhasm: r = b6 & a3
# asm 1: vpand <a3=reg128#5,<b6=reg128#12,>r=reg128#1
# asm 2: vpand <a3=%xmm4,<b6=%xmm11,>r=%xmm0
vpand %xmm4,%xmm11,%xmm0

# qhasm: r9 ^= r
# asm 1: pxor <r=reg128#1,<r9=reg128#6
# asm 2: pxor <r=%xmm0,<r9=%xmm5
pxor %xmm0,%xmm5

# qhasm: r = b6 & a2
# asm 1: vpand <a2=reg128#7,<b6=reg128#12,>r=reg128#1
# asm 2: vpand <a2=%xmm6,<b6=%xmm11,>r=%xmm0
vpand %xmm6,%xmm11,%xmm0

# qhasm: r8 ^= r
# asm 1: pxor <r=reg128#1,<r8=reg128#8
# asm 2: pxor <r=%xmm0,<r8=%xmm7
pxor %xmm0,%xmm7

# qhasm: r = b6 & a1
# asm 1: vpand <a1=reg128#9,<b6=reg128#12,>r=reg128#1
# asm 2: vpand <a1=%xmm8,<b6=%xmm11,>r=%xmm0
vpand %xmm8,%xmm11,%xmm0

# qhasm: r7 ^= r
# asm 1: pxor <r=reg128#1,<r7=reg128#10
# asm 2: pxor <r=%xmm0,<r7=%xmm9
pxor %xmm0,%xmm9

# qhasm: r6 = b6 & a0
# asm 1: vpand <a0=reg128#11,<b6=reg128#12,>r6=reg128#1
# asm 2: vpand <a0=%xmm10,<b6=%xmm11,>r6=%xmm0
vpand %xmm10,%xmm11,%xmm0

# qhasm: b5 = mem64[ input_2 + 0 ] x2
# asm 1: movddup 0(<input_2=int64#3),>b5=reg128#12
# asm 2: movddup 0(<input_2=%rdx),>b5=%xmm11
movddup 0(%rdx),%xmm11

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  %rcx,%rdx

# qhasm: r = b5 & a5
# asm 1: vpand <a5=reg128#2,<b5=reg128#12,>r=reg128#13
# asm 2: vpand <a5=%xmm1,<b5=%xmm11,>r=%xmm12
vpand %xmm1,%xmm11,%xmm12

# qhasm: r10 ^= r
# asm 1: pxor <r=reg128#13,<r10=reg128#4
# asm 2: pxor <r=%xmm12,<r10=%xmm3
pxor %xmm12,%xmm3

# qhasm: mem128[ ptr + 160 ] = r10
# asm 1: movdqu   <r10=reg128#4,160(<ptr=int64#5)
# asm 2: movdqu   <r10=%xmm3,160(<ptr=%r8)
movdqu   %xmm3,160(%r8)

# qhasm: r = b5 & a4
# asm 1: vpand <a4=reg128#3,<b5=reg128#12,>r=reg128#4
# asm 2: vpand <a4=%xmm2,<b5=%xmm11,>r=%xmm3
vpand %xmm2,%xmm11,%xmm3

# qhasm: r9 ^= r
# asm 1: pxor <r=reg128#4,<r9=reg128#6
# asm 2: pxor <r=%xmm3,<r9=%xmm5
pxor %xmm3,%xmm5

# qhasm: r = b5 & a3
# asm 1: vpand <a3=reg128#5,<b5=reg128#12,>r=reg128#4
# asm 2: vpand <a3=%xmm4,<b5=%xmm11,>r=%xmm3
vpand %xmm4,%xmm11,%xmm3

# qhasm: r8 ^= r
# asm 1: pxor <r=reg128#4,<r8=reg128#8
# asm 2: pxor <r=%xmm3,<r8=%xmm7
pxor %xmm3,%xmm7

# qhasm: r = b5 & a2
# asm 1: vpand <a2=reg128#7,<b5=reg128#12,>r=reg128#4
# asm 2: vpand <a2=%xmm6,<b5=%xmm11,>r=%xmm3
vpand %xmm6,%xmm11,%xmm3

# qhasm: r7 ^= r
# asm 1: pxor <r=reg128#4,<r7=reg128#10
# asm 2: pxor <r=%xmm3,<r7=%xmm9
pxor %xmm3,%xmm9

# qhasm: r = b5 & a1
# asm 1: vpand <a1=reg128#9,<b5=reg128#12,>r=reg128#4
# asm 2: vpand <a1=%xmm8,<b5=%xmm11,>r=%xmm3
vpand %xmm8,%xmm11,%xmm3

# qhasm: r6 ^= r
# asm 1: pxor <r=reg128#4,<r6=reg128#1
# asm 2: pxor <r=%xmm3,<r6=%xmm0
pxor %xmm3,%xmm0

# qhasm: r5 = b5 & a0
# asm 1: vpand <a0=reg128#11,<b5=reg128#12,>r5=reg128#4
# asm 2: vpand <a0=%xmm10,<b5=%xmm11,>r5=%xmm3
vpand %xmm10,%xmm11,%xmm3

# qhasm: b4 = mem64[ input_2 + 0 ] x2
# asm 1: movddup 0(<input_2=int64#3),>b4=reg128#12
# asm 2: movddup 0(<input_2=%rdx),>b4=%xmm11
movddup 0(%rdx),%xmm11

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  %rcx,%rdx

# qhasm: r = b4 & a5
# asm 1: vpand <a5=reg128#2,<b4=reg128#12,>r=reg128#13
# asm 2: vpand <a5=%xmm1,<b4=%xmm11,>r=%xmm12
vpand %xmm1,%xmm11,%xmm12

# qhasm: r9 ^= r
# asm 1: pxor <r=reg128#13,<r9=reg128#6
# asm 2: pxor <r=%xmm12,<r9=%xmm5
pxor %xmm12,%xmm5

# qhasm: mem128[ ptr + 144 ] = r9
# asm 1: movdqu   <r9=reg128#6,144(<ptr=int64#5)
# asm 2: movdqu   <r9=%xmm5,144(<ptr=%r8)
movdqu   %xmm5,144(%r8)

# qhasm: r = b4 & a4
# asm 1: vpand <a4=reg128#3,<b4=reg128#12,>r=reg128#6
# asm 2: vpand <a4=%xmm2,<b4=%xmm11,>r=%xmm5
vpand %xmm2,%xmm11,%xmm5

# qhasm: r8 ^= r
# asm 1: pxor <r=reg128#6,<r8=reg128#8
# asm 2: pxor <r=%xmm5,<r8=%xmm7
pxor %xmm5,%xmm7

# qhasm: r = b4 & a3
# asm 1: vpand <a3=reg128#5,<b4=reg128#12,>r=reg128#6
# asm 2: vpand <a3=%xmm4,<b4=%xmm11,>r=%xmm5
vpand %xmm4,%xmm11,%xmm5

# qhasm: r7 ^= r
# asm 1: pxor <r=reg128#6,<r7=reg128#10
# asm 2: pxor <r=%xmm5,<r7=%xmm9
pxor %xmm5,%xmm9

# qhasm: r = b4 & a2
# asm 1: vpand <a2=reg128#7,<b4=reg128#12,>r=reg128#6
# asm 2: vpand <a2=%xmm6,<b4=%xmm11,>r=%xmm5
vpand %xmm6,%xmm11,%xmm5

# qhasm: r6 ^= r
# asm 1: pxor <r=reg128#6,<r6=reg128#1
# asm 2: pxor <r=%xmm5,<r6=%xmm0
pxor %xmm5,%xmm0

# qhasm: r = b4 & a1
# asm 1: vpand <a1=reg128#9,<b4=reg128#12,>r=reg128#6
# asm 2: vpand <a1=%xmm8,<b4=%xmm11,>r=%xmm5
vpand %xmm8,%xmm11,%xmm5

# qhasm: r5 ^= r
# asm 1: pxor <r=reg128#6,<r5=reg128#4
# asm 2: pxor <r=%xmm5,<r5=%xmm3
pxor %xmm5,%xmm3

# qhasm: r4 = b4 & a0
# asm 1: vpand <a0=reg128#11,<b4=reg128#12,>r4=reg128#6
# asm 2: vpand <a0=%xmm10,<b4=%xmm11,>r4=%xmm5
vpand %xmm10,%xmm11,%xmm5

# qhasm: b3 = mem64[ input_2 + 0 ] x2
# asm 1: movddup 0(<input_2=int64#3),>b3=reg128#12
# asm 2: movddup 0(<input_2=%rdx),>b3=%xmm11
movddup 0(%rdx),%xmm11

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  %rcx,%rdx

# qhasm: r = b3 & a5
# asm 1: vpand <a5=reg128#2,<b3=reg128#12,>r=reg128#13
# asm 2: vpand <a5=%xmm1,<b3=%xmm11,>r=%xmm12
vpand %xmm1,%xmm11,%xmm12

# qhasm: r8 ^= r
# asm 1: pxor <r=reg128#13,<r8=reg128#8
# asm 2: pxor <r=%xmm12,<r8=%xmm7
pxor %xmm12,%xmm7

# qhasm: mem128[ ptr + 128 ] = r8
# asm 1: movdqu   <r8=reg128#8,128(<ptr=int64#5)
# asm 2: movdqu   <r8=%xmm7,128(<ptr=%r8)
movdqu   %xmm7,128(%r8)

# qhasm: r = b3 & a4
# asm 1: vpand <a4=reg128#3,<b3=reg128#12,>r=reg128#8
# asm 2: vpand <a4=%xmm2,<b3=%xmm11,>r=%xmm7
vpand %xmm2,%xmm11,%xmm7

# qhasm: r7 ^= r
# asm 1: pxor <r=reg128#8,<r7=reg128#10
# asm 2: pxor <r=%xmm7,<r7=%xmm9
pxor %xmm7,%xmm9

# qhasm: r = b3 & a3
# asm 1: vpand <a3=reg128#5,<b3=reg128#12,>r=reg128#8
# asm 2: vpand <a3=%xmm4,<b3=%xmm11,>r=%xmm7
vpand %xmm4,%xmm11,%xmm7

# qhasm: r6 ^= r
# asm 1: pxor <r=reg128#8,<r6=reg128#1
# asm 2: pxor <r=%xmm7,<r6=%xmm0
pxor %xmm7,%xmm0

# qhasm: r = b3 & a2
# asm 1: vpand <a2=reg128#7,<b3=reg128#12,>r=reg128#8
# asm 2: vpand <a2=%xmm6,<b3=%xmm11,>r=%xmm7
vpand %xmm6,%xmm11,%xmm7

# qhasm: r5 ^= r
# asm 1: pxor <r=reg128#8,<r5=reg128#4
# asm 2: pxor <r=%xmm7,<r5=%xmm3
pxor %xmm7,%xmm3

# qhasm: r = b3 & a1
# asm 1: vpand <a1=reg128#9,<b3=reg128#12,>r=reg128#8
# asm 2: vpand <a1=%xmm8,<b3=%xmm11,>r=%xmm7
vpand %xmm8,%xmm11,%xmm7

# qhasm: r4 ^= r
# asm 1: pxor <r=reg128#8,<r4=reg128#6
# asm 2: pxor <r=%xmm7,<r4=%xmm5
pxor %xmm7,%xmm5

# qhasm: r3 = b3 & a0
# asm 1: vpand <a0=reg128#11,<b3=reg128#12,>r3=reg128#8
# asm 2: vpand <a0=%xmm10,<b3=%xmm11,>r3=%xmm7
vpand %xmm10,%xmm11,%xmm7

# qhasm: b2 = mem64[ input_2 + 0 ] x2
# asm 1: movddup 0(<input_2=int64#3),>b2=reg128#12
# asm 2: movddup 0(<input_2=%rdx),>b2=%xmm11
movddup 0(%rdx),%xmm11

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  %rcx,%rdx

# qhasm: r = b2 & a5
# asm 1: vpand <a5=reg128#2,<b2=reg128#12,>r=reg128#13
# asm 2: vpand <a5=%xmm1,<b2=%xmm11,>r=%xmm12
vpand %xmm1,%xmm11,%xmm12

# qhasm: r7 ^= r
# asm 1: pxor <r=reg128#13,<r7=reg128#10
# asm 2: pxor <r=%xmm12,<r7=%xmm9
pxor %xmm12,%xmm9

# qhasm: mem128[ ptr + 112 ] = r7
# asm 1: movdqu   <r7=reg128#10,112(<ptr=int64#5)
# asm 2: movdqu   <r7=%xmm9,112(<ptr=%r8)
movdqu   %xmm9,112(%r8)

# qhasm: r = b2 & a4
# asm 1: vpand <a4=reg128#3,<b2=reg128#12,>r=reg128#10
# asm 2: vpand <a4=%xmm2,<b2=%xmm11,>r=%xmm9
vpand %xmm2,%xmm11,%xmm9

# qhasm: r6 ^= r
# asm 1: pxor <r=reg128#10,<r6=reg128#1
# asm 2: pxor <r=%xmm9,<r6=%xmm0
pxor %xmm9,%xmm0

# qhasm: r = b2 & a3
# asm 1: vpand <a3=reg128#5,<b2=reg128#12,>r=reg128#10
# asm 2: vpand <a3=%xmm4,<b2=%xmm11,>r=%xmm9
vpand %xmm4,%xmm11,%xmm9

# qhasm: r5 ^= r
# asm 1: pxor <r=reg128#10,<r5=reg128#4
# asm 2: pxor <r=%xmm9,<r5=%xmm3
pxor %xmm9,%xmm3

# qhasm: r = b2 & a2
# asm 1: vpand <a2=reg128#7,<b2=reg128#12,>r=reg128#10
# asm 2: vpand <a2=%xmm6,<b2=%xmm11,>r=%xmm9
vpand %xmm6,%xmm11,%xmm9

# qhasm: r4 ^= r
# asm 1: pxor <r=reg128#10,<r4=reg128#6
# asm 2: pxor <r=%xmm9,<r4=%xmm5
pxor %xmm9,%xmm5

# qhasm: r = b2 & a1
# asm 1: vpand <a1=reg128#9,<b2=reg128#12,>r=reg128#10
# asm 2: vpand <a1=%xmm8,<b2=%xmm11,>r=%xmm9
vpand %xmm8,%xmm11,%xmm9

# qhasm: r3 ^= r
# asm 1: pxor <r=reg128#10,<r3=reg128#8
# asm 2: pxor <r=%xmm9,<r3=%xmm7
pxor %xmm9,%xmm7

# qhasm: r2 = b2 & a0
# asm 1: vpand <a0=reg128#11,<b2=reg128#12,>r2=reg128#10
# asm 2: vpand <a0=%xmm10,<b2=%xmm11,>r2=%xmm9
vpand %xmm10,%xmm11,%xmm9

# qhasm: b1 = mem64[ input_2 + 0 ] x2
# asm 1: movddup 0(<input_2=int64#3),>b1=reg128#12
# asm 2: movddup 0(<input_2=%rdx),>b1=%xmm11
movddup 0(%rdx),%xmm11

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  %rcx,%rdx

# qhasm: r = b1 & a5
# asm 1: vpand <a5=reg128#2,<b1=reg128#12,>r=reg128#13
# asm 2: vpand <a5=%xmm1,<b1=%xmm11,>r=%xmm12
vpand %xmm1,%xmm11,%xmm12

# qhasm: r6 ^= r
# asm 1: pxor <r=reg128#13,<r6=reg128#1
# asm 2: pxor <r=%xmm12,<r6=%xmm0
pxor %xmm12,%xmm0

# qhasm: mem128[ ptr + 96 ] = r6
# asm 1: movdqu   <r6=reg128#1,96(<ptr=int64#5)
# asm 2: movdqu   <r6=%xmm0,96(<ptr=%r8)
movdqu   %xmm0,96(%r8)

# qhasm: r = b1 & a4
# asm 1: vpand <a4=reg128#3,<b1=reg128#12,>r=reg128#1
# asm 2: vpand <a4=%xmm2,<b1=%xmm11,>r=%xmm0
vpand %xmm2,%xmm11,%xmm0

# qhasm: r5 ^= r
# asm 1: pxor <r=reg128#1,<r5=reg128#4
# asm 2: pxor <r=%xmm0,<r5=%xmm3
pxor %xmm0,%xmm3

# qhasm: r = b1 & a3
# asm 1: vpand <a3=reg128#5,<b1=reg128#12,>r=reg128#1
# asm 2: vpand <a3=%xmm4,<b1=%xmm11,>r=%xmm0
vpand %xmm4,%xmm11,%xmm0

# qhasm: r4 ^= r
# asm 1: pxor <r=reg128#1,<r4=reg128#6
# asm 2: pxor <r=%xmm0,<r4=%xmm5
pxor %xmm0,%xmm5

# qhasm: r = b1 & a2
# asm 1: vpand <a2=reg128#7,<b1=reg128#12,>r=reg128#1
# asm 2: vpand <a2=%xmm6,<b1=%xmm11,>r=%xmm0
vpand %xmm6,%xmm11,%xmm0

# qhasm: r3 ^= r
# asm 1: pxor <r=reg128#1,<r3=reg128#8
# asm 2: pxor <r=%xmm0,<r3=%xmm7
pxor %xmm0,%xmm7

# qhasm: r = b1 & a1
# asm 1: vpand <a1=reg128#9,<b1=reg128#12,>r=reg128#1
# asm 2: vpand <a1=%xmm8,<b1=%xmm11,>r=%xmm0
vpand %xmm8,%xmm11,%xmm0

# qhasm: r2 ^= r
# asm 1: pxor <r=reg128#1,<r2=reg128#10
# asm 2: pxor <r=%xmm0,<r2=%xmm9
pxor %xmm0,%xmm9

# qhasm: r1 = b1 & a0
# asm 1: vpand <a0=reg128#11,<b1=reg128#12,>r1=reg128#1
# asm 2: vpand <a0=%xmm10,<b1=%xmm11,>r1=%xmm0
vpand %xmm10,%xmm11,%xmm0

# qhasm: b0 = mem64[ input_2 + 0 ] x2
# asm 1: movddup 0(<input_2=int64#3),>b0=reg128#12
# asm 2: movddup 0(<input_2=%rdx),>b0=%xmm11
movddup 0(%rdx),%xmm11

# qhasm: input_2 -= input_3
# asm 1: sub  <input_3=int64#4,<input_2=int64#3
# asm 2: sub  <input_3=%rcx,<input_2=%rdx
sub  %rcx,%rdx

# qhasm: r = b0 & a5
# asm 1: vpand <a5=reg128#2,<b0=reg128#12,>r=reg128#2
# asm 2: vpand <a5=%xmm1,<b0=%xmm11,>r=%xmm1
vpand %xmm1,%xmm11,%xmm1

# qhasm: r5 ^= r
# asm 1: pxor <r=reg128#2,<r5=reg128#4
# asm 2: pxor <r=%xmm1,<r5=%xmm3
pxor %xmm1,%xmm3

# qhasm: mem128[ ptr + 80 ] = r5
# asm 1: movdqu   <r5=reg128#4,80(<ptr=int64#5)
# asm 2: movdqu   <r5=%xmm3,80(<ptr=%r8)
movdqu   %xmm3,80(%r8)

# qhasm: r = b0 & a4
# asm 1: vpand <a4=reg128#3,<b0=reg128#12,>r=reg128#2
# asm 2: vpand <a4=%xmm2,<b0=%xmm11,>r=%xmm1
vpand %xmm2,%xmm11,%xmm1

# qhasm: r4 ^= r
# asm 1: pxor <r=reg128#2,<r4=reg128#6
# asm 2: pxor <r=%xmm1,<r4=%xmm5
pxor %xmm1,%xmm5

# qhasm: r = b0 & a3
# asm 1: vpand <a3=reg128#5,<b0=reg128#12,>r=reg128#2
# asm 2: vpand <a3=%xmm4,<b0=%xmm11,>r=%xmm1
vpand %xmm4,%xmm11,%xmm1

# qhasm: r3 ^= r
# asm 1: pxor <r=reg128#2,<r3=reg128#8
# asm 2: pxor <r=%xmm1,<r3=%xmm7
pxor %xmm1,%xmm7

# qhasm: r = b0 & a2
# asm 1: vpand <a2=reg128#7,<b0=reg128#12,>r=reg128#2
# asm 2: vpand <a2=%xmm6,<b0=%xmm11,>r=%xmm1
vpand %xmm6,%xmm11,%xmm1

# qhasm: r2 ^= r
# asm 1: pxor <r=reg128#2,<r2=reg128#10
# asm 2: pxor <r=%xmm1,<r2=%xmm9
pxor %xmm1,%xmm9

# qhasm: r = b0 & a1
# asm 1: vpand <a1=reg128#9,<b0=reg128#12,>r=reg128#2
# asm 2: vpand <a1=%xmm8,<b0=%xmm11,>r=%xmm1
vpand %xmm8,%xmm11,%xmm1

# qhasm: r1 ^= r
# asm 1: pxor <r=reg128#2,<r1=reg128#1
# asm 2: pxor <r=%xmm1,<r1=%xmm0
pxor %xmm1,%xmm0

# qhasm: r0 = b0 & a0
# asm 1: vpand <a0=reg128#11,<b0=reg128#12,>r0=reg128#2
# asm 2: vpand <a0=%xmm10,<b0=%xmm11,>r0=%xmm1
vpand %xmm10,%xmm11,%xmm1

# qhasm: mem128[ ptr + 64 ] = r4
# asm 1: movdqu   <r4=reg128#6,64(<ptr=int64#5)
# asm 2: movdqu   <r4=%xmm5,64(<ptr=%r8)
movdqu   %xmm5,64(%r8)

# qhasm: mem128[ ptr + 48 ] = r3
# asm 1: movdqu   <r3=reg128#8,48(<ptr=int64#5)
# asm 2: movdqu   <r3=%xmm7,48(<ptr=%r8)
movdqu   %xmm7,48(%r8)

# qhasm: mem128[ ptr + 32 ] = r2
# asm 1: movdqu   <r2=reg128#10,32(<ptr=int64#5)
# asm 2: movdqu   <r2=%xmm9,32(<ptr=%r8)
movdqu   %xmm9,32(%r8)

# qhasm: mem128[ ptr + 16 ] = r1
# asm 1: movdqu   <r1=reg128#1,16(<ptr=int64#5)
# asm 2: movdqu   <r1=%xmm0,16(<ptr=%r8)
movdqu   %xmm0,16(%r8)

# qhasm: mem128[ ptr + 0 ] = r0
# asm 1: movdqu   <r0=reg128#2,0(<ptr=int64#5)
# asm 2: movdqu   <r0=%xmm1,0(<ptr=%r8)
movdqu   %xmm1,0(%r8)

# qhasm: h22 = mem64[ ptr + 264 ]
# asm 1: movq   264(<ptr=int64#5),>h22=int64#2
# asm 2: movq   264(<ptr=%r8),>h22=%rsi
movq   264(%r8),%rsi

# qhasm: h13 = h22
# asm 1: mov  <h22=int64#2,>h13=int64#3
# asm 2: mov  <h22=%rsi,>h13=%rdx
mov  %rsi,%rdx

# qhasm: h10 = h22
# asm 1: mov  <h22=int64#2,>h10=int64#2
# asm 2: mov  <h22=%rsi,>h10=%rsi
mov  %rsi,%rsi

# qhasm: h21 = mem64[ ptr + 248 ]
# asm 1: movq   248(<ptr=int64#5),>h21=int64#4
# asm 2: movq   248(<ptr=%r8),>h21=%rcx
movq   248(%r8),%rcx

# qhasm: h12 = h21
# asm 1: mov  <h21=int64#4,>h12=int64#6
# asm 2: mov  <h21=%rcx,>h12=%r9
mov  %rcx,%r9

# qhasm: h9 = h21
# asm 1: mov  <h21=int64#4,>h9=int64#4
# asm 2: mov  <h21=%rcx,>h9=%rcx
mov  %rcx,%rcx

# qhasm: h20 = mem64[ ptr + 232 ]
# asm 1: movq   232(<ptr=int64#5),>h20=int64#7
# asm 2: movq   232(<ptr=%r8),>h20=%rax
movq   232(%r8),%rax

# qhasm: h11 = h20
# asm 1: mov  <h20=int64#7,>h11=int64#8
# asm 2: mov  <h20=%rax,>h11=%r10
mov  %rax,%r10

# qhasm: h8 = h20
# asm 1: mov  <h20=int64#7,>h8=int64#7
# asm 2: mov  <h20=%rax,>h8=%rax
mov  %rax,%rax

# qhasm: h19 = mem64[ ptr + 216 ]
# asm 1: movq   216(<ptr=int64#5),>h19=int64#9
# asm 2: movq   216(<ptr=%r8),>h19=%r11
movq   216(%r8),%r11

# qhasm: h10 ^= h19
# asm 1: xor  <h19=int64#9,<h10=int64#2
# asm 2: xor  <h19=%r11,<h10=%rsi
xor  %r11,%rsi

# qhasm: h7 = h19
# asm 1: mov  <h19=int64#9,>h7=int64#9
# asm 2: mov  <h19=%r11,>h7=%r11
mov  %r11,%r11

# qhasm: h18 = mem64[ ptr + 200 ]
# asm 1: movq   200(<ptr=int64#5),>h18=int64#10
# asm 2: movq   200(<ptr=%r8),>h18=%r12
movq   200(%r8),%r12

# qhasm: h9 ^= h18
# asm 1: xor  <h18=int64#10,<h9=int64#4
# asm 2: xor  <h18=%r12,<h9=%rcx
xor  %r12,%rcx

# qhasm: h6 = h18
# asm 1: mov  <h18=int64#10,>h6=int64#10
# asm 2: mov  <h18=%r12,>h6=%r12
mov  %r12,%r12

# qhasm: h17 = mem64[ ptr + 184 ]
# asm 1: movq   184(<ptr=int64#5),>h17=int64#11
# asm 2: movq   184(<ptr=%r8),>h17=%r13
movq   184(%r8),%r13

# qhasm: h8 ^= h17
# asm 1: xor  <h17=int64#11,<h8=int64#7
# asm 2: xor  <h17=%r13,<h8=%rax
xor  %r13,%rax

# qhasm: h5 = h17
# asm 1: mov  <h17=int64#11,>h5=int64#11
# asm 2: mov  <h17=%r13,>h5=%r13
mov  %r13,%r13

# qhasm: h16 = mem64[ ptr + 168 ]
# asm 1: movq   168(<ptr=int64#5),>h16=int64#12
# asm 2: movq   168(<ptr=%r8),>h16=%r14
movq   168(%r8),%r14

# qhasm: h16 ^= *(uint64 *) ( ptr + 256 )
# asm 1: xorq 256(<ptr=int64#5),<h16=int64#12
# asm 2: xorq 256(<ptr=%r8),<h16=%r14
xorq 256(%r8),%r14

# qhasm: h7 ^= h16
# asm 1: xor  <h16=int64#12,<h7=int64#9
# asm 2: xor  <h16=%r14,<h7=%r11
xor  %r14,%r11

# qhasm: h4 = h16
# asm 1: mov  <h16=int64#12,>h4=int64#12
# asm 2: mov  <h16=%r14,>h4=%r14
mov  %r14,%r14

# qhasm: h15 = mem64[ ptr + 152 ]
# asm 1: movq   152(<ptr=int64#5),>h15=int64#13
# asm 2: movq   152(<ptr=%r8),>h15=%r15
movq   152(%r8),%r15

# qhasm: h15 ^= *(uint64 *) ( ptr + 240 )
# asm 1: xorq 240(<ptr=int64#5),<h15=int64#13
# asm 2: xorq 240(<ptr=%r8),<h15=%r15
xorq 240(%r8),%r15

# qhasm: h6 ^= h15
# asm 1: xor  <h15=int64#13,<h6=int64#10
# asm 2: xor  <h15=%r15,<h6=%r12
xor  %r15,%r12

# qhasm: h3 = h15
# asm 1: mov  <h15=int64#13,>h3=int64#13
# asm 2: mov  <h15=%r15,>h3=%r15
mov  %r15,%r15

# qhasm: h14 = mem64[ ptr + 136 ]
# asm 1: movq   136(<ptr=int64#5),>h14=int64#14
# asm 2: movq   136(<ptr=%r8),>h14=%rbx
movq   136(%r8),%rbx

# qhasm: h14 ^= *(uint64 *) ( ptr + 224 )
# asm 1: xorq 224(<ptr=int64#5),<h14=int64#14
# asm 2: xorq 224(<ptr=%r8),<h14=%rbx
xorq 224(%r8),%rbx

# qhasm: h5 ^= h14
# asm 1: xor  <h14=int64#14,<h5=int64#11
# asm 2: xor  <h14=%rbx,<h5=%r13
xor  %rbx,%r13

# qhasm: h2 = h14
# asm 1: mov  <h14=int64#14,>h2=int64#14
# asm 2: mov  <h14=%rbx,>h2=%rbx
mov  %rbx,%rbx

# qhasm: h13 ^= *(uint64 *) ( ptr + 120 )
# asm 1: xorq 120(<ptr=int64#5),<h13=int64#3
# asm 2: xorq 120(<ptr=%r8),<h13=%rdx
xorq 120(%r8),%rdx

# qhasm: h13 ^= *(uint64 *) ( ptr + 208 )
# asm 1: xorq 208(<ptr=int64#5),<h13=int64#3
# asm 2: xorq 208(<ptr=%r8),<h13=%rdx
xorq 208(%r8),%rdx

# qhasm: h4 ^= h13
# asm 1: xor  <h13=int64#3,<h4=int64#12
# asm 2: xor  <h13=%rdx,<h4=%r14
xor  %rdx,%r14

# qhasm: h1 = h13
# asm 1: mov  <h13=int64#3,>h1=int64#3
# asm 2: mov  <h13=%rdx,>h1=%rdx
mov  %rdx,%rdx

# qhasm: h12 ^= *(uint64 *) ( ptr + 104 )
# asm 1: xorq 104(<ptr=int64#5),<h12=int64#6
# asm 2: xorq 104(<ptr=%r8),<h12=%r9
xorq 104(%r8),%r9

# qhasm: h12 ^= *(uint64 *) ( ptr + 192 )
# asm 1: xorq 192(<ptr=int64#5),<h12=int64#6
# asm 2: xorq 192(<ptr=%r8),<h12=%r9
xorq 192(%r8),%r9

# qhasm: h3 ^= h12
# asm 1: xor  <h12=int64#6,<h3=int64#13
# asm 2: xor  <h12=%r9,<h3=%r15
xor  %r9,%r15

# qhasm: h0 = h12
# asm 1: mov  <h12=int64#6,>h0=int64#6
# asm 2: mov  <h12=%r9,>h0=%r9
mov  %r9,%r9

# qhasm: h11 ^= *(uint64 *) ( ptr + 176 )
# asm 1: xorq 176(<ptr=int64#5),<h11=int64#8
# asm 2: xorq 176(<ptr=%r8),<h11=%r10
xorq 176(%r8),%r10

# qhasm: h11 ^= *(uint64 *) ( ptr + 88 )
# asm 1: xorq 88(<ptr=int64#5),<h11=int64#8
# asm 2: xorq 88(<ptr=%r8),<h11=%r10
xorq 88(%r8),%r10

# qhasm: mem64[ input_0 + 88 ] = h11
# asm 1: movq   <h11=int64#8,88(<input_0=int64#1)
# asm 2: movq   <h11=%r10,88(<input_0=%rdi)
movq   %r10,88(%rdi)

# qhasm: h10 ^= *(uint64 *) ( ptr + 160 )
# asm 1: xorq 160(<ptr=int64#5),<h10=int64#2
# asm 2: xorq 160(<ptr=%r8),<h10=%rsi
xorq 160(%r8),%rsi

# qhasm: h10 ^= *(uint64 *) ( ptr + 72 )
# asm 1: xorq 72(<ptr=int64#5),<h10=int64#2
# asm 2: xorq 72(<ptr=%r8),<h10=%rsi
xorq 72(%r8),%rsi

# qhasm: mem64[ input_0 + 80 ] = h10
# asm 1: movq   <h10=int64#2,80(<input_0=int64#1)
# asm 2: movq   <h10=%rsi,80(<input_0=%rdi)
movq   %rsi,80(%rdi)

# qhasm: h9 ^= *(uint64 *) ( ptr + 144 )
# asm 1: xorq 144(<ptr=int64#5),<h9=int64#4
# asm 2: xorq 144(<ptr=%r8),<h9=%rcx
xorq 144(%r8),%rcx

# qhasm: h9 ^= *(uint64 *) ( ptr + 56 )
# asm 1: xorq 56(<ptr=int64#5),<h9=int64#4
# asm 2: xorq 56(<ptr=%r8),<h9=%rcx
xorq 56(%r8),%rcx

# qhasm: mem64[ input_0 + 72 ] = h9
# asm 1: movq   <h9=int64#4,72(<input_0=int64#1)
# asm 2: movq   <h9=%rcx,72(<input_0=%rdi)
movq   %rcx,72(%rdi)

# qhasm: h8 ^= *(uint64 *) ( ptr + 128 )
# asm 1: xorq 128(<ptr=int64#5),<h8=int64#7
# asm 2: xorq 128(<ptr=%r8),<h8=%rax
xorq 128(%r8),%rax

# qhasm: h8 ^= *(uint64 *) ( ptr + 40 )
# asm 1: xorq 40(<ptr=int64#5),<h8=int64#7
# asm 2: xorq 40(<ptr=%r8),<h8=%rax
xorq 40(%r8),%rax

# qhasm: mem64[ input_0 + 64 ] = h8
# asm 1: movq   <h8=int64#7,64(<input_0=int64#1)
# asm 2: movq   <h8=%rax,64(<input_0=%rdi)
movq   %rax,64(%rdi)

# qhasm: h7 ^= *(uint64 *) ( ptr + 112 )
# asm 1: xorq 112(<ptr=int64#5),<h7=int64#9
# asm 2: xorq 112(<ptr=%r8),<h7=%r11
xorq 112(%r8),%r11

# qhasm: h7 ^= *(uint64 *) ( ptr + 24 )
# asm 1: xorq 24(<ptr=int64#5),<h7=int64#9
# asm 2: xorq 24(<ptr=%r8),<h7=%r11
xorq 24(%r8),%r11

# qhasm: mem64[ input_0 + 56 ] = h7
# asm 1: movq   <h7=int64#9,56(<input_0=int64#1)
# asm 2: movq   <h7=%r11,56(<input_0=%rdi)
movq   %r11,56(%rdi)

# qhasm: h6 ^= *(uint64 *) ( ptr + 96 )
# asm 1: xorq 96(<ptr=int64#5),<h6=int64#10
# asm 2: xorq 96(<ptr=%r8),<h6=%r12
xorq 96(%r8),%r12

# qhasm: h6 ^= *(uint64 *) ( ptr + 8 )
# asm 1: xorq 8(<ptr=int64#5),<h6=int64#10
# asm 2: xorq 8(<ptr=%r8),<h6=%r12
xorq 8(%r8),%r12

# qhasm: mem64[ input_0 + 48 ] = h6
# asm 1: movq   <h6=int64#10,48(<input_0=int64#1)
# asm 2: movq   <h6=%r12,48(<input_0=%rdi)
movq   %r12,48(%rdi)

# qhasm: h5 ^= *(uint64 *) ( ptr + 80 )
# asm 1: xorq 80(<ptr=int64#5),<h5=int64#11
# asm 2: xorq 80(<ptr=%r8),<h5=%r13
xorq 80(%r8),%r13

# qhasm: mem64[ input_0 + 40 ] = h5
# asm 1: movq   <h5=int64#11,40(<input_0=int64#1)
# asm 2: movq   <h5=%r13,40(<input_0=%rdi)
movq   %r13,40(%rdi)

# qhasm: h4 ^= *(uint64 *) ( ptr + 64 )
# asm 1: xorq 64(<ptr=int64#5),<h4=int64#12
# asm 2: xorq 64(<ptr=%r8),<h4=%r14
xorq 64(%r8),%r14

# qhasm: mem64[ input_0 + 32 ] = h4
# asm 1: movq   <h4=int64#12,32(<input_0=int64#1)
# asm 2: movq   <h4=%r14,32(<input_0=%rdi)
movq   %r14,32(%rdi)

# qhasm: h3 ^= *(uint64 *) ( ptr + 48 )
# asm 1: xorq 48(<ptr=int64#5),<h3=int64#13
# asm 2: xorq 48(<ptr=%r8),<h3=%r15
xorq 48(%r8),%r15

# qhasm: mem64[ input_0 + 24 ] = h3
# asm 1: movq   <h3=int64#13,24(<input_0=int64#1)
# asm 2: movq   <h3=%r15,24(<input_0=%rdi)
movq   %r15,24(%rdi)

# qhasm: h2 ^= *(uint64 *) ( ptr + 32 )
# asm 1: xorq 32(<ptr=int64#5),<h2=int64#14
# asm 2: xorq 32(<ptr=%r8),<h2=%rbx
xorq 32(%r8),%rbx

# qhasm: mem64[ input_0 + 16 ] = h2
# asm 1: movq   <h2=int64#14,16(<input_0=int64#1)
# asm 2: movq   <h2=%rbx,16(<input_0=%rdi)
movq   %rbx,16(%rdi)

# qhasm: h1 ^= *(uint64 *) ( ptr + 16 )
# asm 1: xorq 16(<ptr=int64#5),<h1=int64#3
# asm 2: xorq 16(<ptr=%r8),<h1=%rdx
xorq 16(%r8),%rdx

# qhasm: mem64[ input_0 + 8 ] = h1
# asm 1: movq   <h1=int64#3,8(<input_0=int64#1)
# asm 2: movq   <h1=%rdx,8(<input_0=%rdi)
movq   %rdx,8(%rdi)

# qhasm: h0 ^= *(uint64 *) ( ptr + 0 )
# asm 1: xorq 0(<ptr=int64#5),<h0=int64#6
# asm 2: xorq 0(<ptr=%r8),<h0=%r9
xorq 0(%r8),%r9

# qhasm: mem64[ input_0 + 0 ] = h0
# asm 1: movq   <h0=int64#6,0(<input_0=int64#1)
# asm 2: movq   <h0=%r9,0(<input_0=%rdi)
movq   %r9,0(%rdi)

# qhasm: caller_r11 = r11_stack
# asm 1: movq <r11_stack=stack64#1,>caller_r11=int64#9
# asm 2: movq <r11_stack=608(%rsp),>caller_r11=%r11
movq 608(%rsp),%r11

# qhasm: caller_r12 = r12_stack
# asm 1: movq <r12_stack=stack64#2,>caller_r12=int64#10
# asm 2: movq <r12_stack=616(%rsp),>caller_r12=%r12
movq 616(%rsp),%r12

# qhasm: caller_r13 = r13_stack
# asm 1: movq <r13_stack=stack64#3,>caller_r13=int64#11
# asm 2: movq <r13_stack=624(%rsp),>caller_r13=%r13
movq 624(%rsp),%r13

# qhasm: caller_r14 = r14_stack
# asm 1: movq <r14_stack=stack64#4,>caller_r14=int64#12
# asm 2: movq <r14_stack=632(%rsp),>caller_r14=%r14
movq 632(%rsp),%r14

# qhasm: caller_r15 = r15_stack
# asm 1: movq <r15_stack=stack64#5,>caller_r15=int64#13
# asm 2: movq <r15_stack=640(%rsp),>caller_r15=%r15
movq 640(%rsp),%r15

# qhasm: caller_rbx = rbx_stack
# asm 1: movq <rbx_stack=stack64#6,>caller_rbx=int64#14
# asm 2: movq <rbx_stack=648(%rsp),>caller_rbx=%rbx
movq 648(%rsp),%rbx

# qhasm: return
add %r11,%rsp
ret
