#include "namespace.h"

#define syndrome_asm CRYPTO_NAMESPACE(syndrome_asm)
#define _syndrome_asm _CRYPTO_NAMESPACE(syndrome_asm)

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

# qhasm: int64 b64

# qhasm: int64 synd

# qhasm: int64 addr

# qhasm: int64 c

# qhasm: int64 c_all

# qhasm: int64 row

# qhasm: reg256 pp

# qhasm: reg256 ee

# qhasm: reg256 ss

# qhasm: int64 b0

# qhasm: int64 b1

# qhasm: int64 i

# qhasm: int64 p

# qhasm: int64 e

# qhasm: int64 s

# qhasm: int64 tmp

# qhasm: stack64 back

# qhasm: int64 buf_ptr

# qhasm: stack256 buf

# qhasm: enter syndrome_asm
.p2align 5
.global _syndrome_asm
.global syndrome_asm
_syndrome_asm:
syndrome_asm:
mov % rsp, % r11
and $31, % r11
add $64, % r11
sub % r11, % rsp

# qhasm: input_2 += 193
# asm 1: add  $193,<input_2=int64#3
# asm 2: add  $193,<input_2=%rdx
add  $193, % rdx

# qhasm: *(uint8  *) (input_0 + 193) = 0
# asm 1: movb   $0,193(<input_0=int64#1)
# asm 2: movb   $0,193(<input_0=%rdi)
movb   $0, 193( % rdi)

# qhasm: tmp = *(uint8  *) (input_2 + 0)
# asm 1: movzbq 0(<input_2=int64#3),>tmp=int64#4
# asm 2: movzbq 0(<input_2=%rdx),>tmp=%rcx
movzbq 0( % rdx), % rcx

# qhasm: back = tmp
# asm 1: movq <tmp=int64#4,>back=stack64#1
# asm 2: movq <tmp=%rcx,>back=32(%rsp)
movq % rcx, 32( % rsp)

# qhasm: i = 0
# asm 1: mov  $0,>i=int64#4
# asm 2: mov  $0,>i=%rcx
mov  $0, % rcx

# qhasm: inner1:
._inner1:

# qhasm: addr = input_2 + i
# asm 1: lea  (<input_2=int64#3,<i=int64#4),>addr=int64#5
# asm 2: lea  (<input_2=%rdx,<i=%rcx),>addr=%r8
lea  ( % rdx, % rcx), % r8

# qhasm: b0 = *(uint8  *) (addr + 0)
# asm 1: movzbq 0(<addr=int64#5),>b0=int64#6
# asm 2: movzbq 0(<addr=%r8),>b0=%r9
movzbq 0( % r8), % r9

# qhasm: b1 = *(uint8  *) (addr + 1)
# asm 1: movzbq 1(<addr=int64#5),>b1=int64#7
# asm 2: movzbq 1(<addr=%r8),>b1=%rax
movzbq 1( % r8), % rax

# qhasm: (uint64) b0 >>= 3
# asm 1: shr  $3,<b0=int64#6
# asm 2: shr  $3,<b0=%r9
shr  $3, % r9

# qhasm: b1 <<= 5
# asm 1: shl  $5,<b1=int64#7
# asm 2: shl  $5,<b1=%rax
shl  $5, % rax

# qhasm: b0 |= b1
# asm 1: or   <b1=int64#7,<b0=int64#6
# asm 2: or   <b1=%rax,<b0=%r9
or   % rax, % r9

# qhasm: *(uint8  *) (addr + 0) = b0
# asm 1: movb   <b0=int64#6b,0(<addr=int64#5)
# asm 2: movb   <b0=%r9b,0(<addr=%r8)
movb   % r9b, 0( % r8)

# qhasm: i += 1
# asm 1: add  $1,<i=int64#4
# asm 2: add  $1,<i=%rcx
add  $1, % rcx

# qhasm: =? i-676
# asm 1: cmp  $676,<i=int64#4
# asm 2: cmp  $676,<i=%rcx
cmp  $676, % rcx
# comment:fp stack unchanged by jump

# qhasm: goto inner1 if !=
jne ._inner1

# qhasm: b0 = *(uint8  *) (addr + 1)
# asm 1: movzbq 1(<addr=int64#5),>b0=int64#4
# asm 2: movzbq 1(<addr=%r8),>b0=%rcx
movzbq 1( % r8), % rcx

# qhasm: (uint64) b0 >>= 3
# asm 1: shr  $3,<b0=int64#4
# asm 2: shr  $3,<b0=%rcx
shr  $3, % rcx

# qhasm: *(uint8  *) (addr + 1) = b0
# asm 1: movb   <b0=int64#4b,1(<addr=int64#5)
# asm 2: movb   <b0=%cl,1(<addr=%r8)
movb   % cl, 1( % r8)

# qhasm: input_1 += 1047319
# asm 1: add  $1047319,<input_1=int64#2
# asm 2: add  $1047319,<input_1=%rsi
add  $1047319, % rsi

# qhasm: buf_ptr = &buf
# asm 1: leaq <buf=stack256#1,>buf_ptr=int64#4
# asm 2: leaq <buf=0(%rsp),>buf_ptr=%rcx
leaq 0( % rsp), % rcx

# qhasm: row = 1547
# asm 1: mov  $1547,>row=int64#5
# asm 2: mov  $1547,>row=%r8
mov  $1547, % r8

# qhasm: loop:
._loop:

# qhasm: row -= 1
# asm 1: sub  $1,<row=int64#5
# asm 2: sub  $1,<row=%r8
sub  $1, % r8

# qhasm: input_1 -= 677
# asm 1: sub  $677,<input_1=int64#2
# asm 2: sub  $677,<input_1=%rsi
sub  $677, % rsi

# qhasm: ss = mem256[ input_1 + 0 ]
# asm 1: vmovupd   0(<input_1=int64#2),>ss=reg256#1
# asm 2: vmovupd   0(<input_1=%rsi),>ss=%ymm0
vmovupd   0( % rsi), % ymm0

# qhasm: ee = mem256[ input_2 + 0 ]
# asm 1: vmovupd   0(<input_2=int64#3),>ee=reg256#2
# asm 2: vmovupd   0(<input_2=%rdx),>ee=%ymm1
vmovupd   0( % rdx), % ymm1

# qhasm: ss &= ee
# asm 1: vpand <ee=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpand <ee=%ymm1,<ss=%ymm0,<ss=%ymm0
vpand % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 32 ]
# asm 1: vmovupd   32(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   32(<input_1=%rsi),>pp=%ymm1
vmovupd   32( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 32 ]
# asm 1: vmovupd   32(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   32(<input_2=%rdx),>ee=%ymm2
vmovupd   32( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 64 ]
# asm 1: vmovupd   64(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   64(<input_1=%rsi),>pp=%ymm1
vmovupd   64( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 64 ]
# asm 1: vmovupd   64(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   64(<input_2=%rdx),>ee=%ymm2
vmovupd   64( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 96 ]
# asm 1: vmovupd   96(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   96(<input_1=%rsi),>pp=%ymm1
vmovupd   96( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 96 ]
# asm 1: vmovupd   96(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   96(<input_2=%rdx),>ee=%ymm2
vmovupd   96( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 128 ]
# asm 1: vmovupd   128(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   128(<input_1=%rsi),>pp=%ymm1
vmovupd   128( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 128 ]
# asm 1: vmovupd   128(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   128(<input_2=%rdx),>ee=%ymm2
vmovupd   128( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 160 ]
# asm 1: vmovupd   160(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   160(<input_1=%rsi),>pp=%ymm1
vmovupd   160( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 160 ]
# asm 1: vmovupd   160(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   160(<input_2=%rdx),>ee=%ymm2
vmovupd   160( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 192 ]
# asm 1: vmovupd   192(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   192(<input_1=%rsi),>pp=%ymm1
vmovupd   192( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 192 ]
# asm 1: vmovupd   192(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   192(<input_2=%rdx),>ee=%ymm2
vmovupd   192( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 224 ]
# asm 1: vmovupd   224(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   224(<input_1=%rsi),>pp=%ymm1
vmovupd   224( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 224 ]
# asm 1: vmovupd   224(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   224(<input_2=%rdx),>ee=%ymm2
vmovupd   224( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 256 ]
# asm 1: vmovupd   256(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   256(<input_1=%rsi),>pp=%ymm1
vmovupd   256( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 256 ]
# asm 1: vmovupd   256(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   256(<input_2=%rdx),>ee=%ymm2
vmovupd   256( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 288 ]
# asm 1: vmovupd   288(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   288(<input_1=%rsi),>pp=%ymm1
vmovupd   288( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 288 ]
# asm 1: vmovupd   288(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   288(<input_2=%rdx),>ee=%ymm2
vmovupd   288( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 320 ]
# asm 1: vmovupd   320(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   320(<input_1=%rsi),>pp=%ymm1
vmovupd   320( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 320 ]
# asm 1: vmovupd   320(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   320(<input_2=%rdx),>ee=%ymm2
vmovupd   320( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 352 ]
# asm 1: vmovupd   352(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   352(<input_1=%rsi),>pp=%ymm1
vmovupd   352( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 352 ]
# asm 1: vmovupd   352(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   352(<input_2=%rdx),>ee=%ymm2
vmovupd   352( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 384 ]
# asm 1: vmovupd   384(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   384(<input_1=%rsi),>pp=%ymm1
vmovupd   384( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 384 ]
# asm 1: vmovupd   384(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   384(<input_2=%rdx),>ee=%ymm2
vmovupd   384( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 416 ]
# asm 1: vmovupd   416(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   416(<input_1=%rsi),>pp=%ymm1
vmovupd   416( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 416 ]
# asm 1: vmovupd   416(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   416(<input_2=%rdx),>ee=%ymm2
vmovupd   416( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 448 ]
# asm 1: vmovupd   448(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   448(<input_1=%rsi),>pp=%ymm1
vmovupd   448( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 448 ]
# asm 1: vmovupd   448(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   448(<input_2=%rdx),>ee=%ymm2
vmovupd   448( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 480 ]
# asm 1: vmovupd   480(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   480(<input_1=%rsi),>pp=%ymm1
vmovupd   480( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 480 ]
# asm 1: vmovupd   480(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   480(<input_2=%rdx),>ee=%ymm2
vmovupd   480( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 512 ]
# asm 1: vmovupd   512(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   512(<input_1=%rsi),>pp=%ymm1
vmovupd   512( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 512 ]
# asm 1: vmovupd   512(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   512(<input_2=%rdx),>ee=%ymm2
vmovupd   512( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 544 ]
# asm 1: vmovupd   544(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   544(<input_1=%rsi),>pp=%ymm1
vmovupd   544( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 544 ]
# asm 1: vmovupd   544(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   544(<input_2=%rdx),>ee=%ymm2
vmovupd   544( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 576 ]
# asm 1: vmovupd   576(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   576(<input_1=%rsi),>pp=%ymm1
vmovupd   576( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 576 ]
# asm 1: vmovupd   576(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   576(<input_2=%rdx),>ee=%ymm2
vmovupd   576( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 608 ]
# asm 1: vmovupd   608(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   608(<input_1=%rsi),>pp=%ymm1
vmovupd   608( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 608 ]
# asm 1: vmovupd   608(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   608(<input_2=%rdx),>ee=%ymm2
vmovupd   608( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 640 ]
# asm 1: vmovupd   640(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   640(<input_1=%rsi),>pp=%ymm1
vmovupd   640( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 640 ]
# asm 1: vmovupd   640(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   640(<input_2=%rdx),>ee=%ymm2
vmovupd   640( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: buf = ss
# asm 1: vmovapd <ss=reg256#1,>buf=stack256#1
# asm 2: vmovapd <ss=%ymm0,>buf=0(%rsp)
vmovapd % ymm0, 0( % rsp)

# qhasm: s = *(uint32  *) (input_1 + 672)
# asm 1: movl   672(<input_1=int64#2),>s=int64#6d
# asm 2: movl   672(<input_1=%rsi),>s=%r9d
movl   672( % rsi), % r9d

# qhasm: e = *(uint32  *) (input_2 + 672)
# asm 1: movl   672(<input_2=int64#3),>e=int64#7d
# asm 2: movl   672(<input_2=%rdx),>e=%eax
movl   672( % rdx), % eax

# qhasm: s &= e
# asm 1: and  <e=int64#7,<s=int64#6
# asm 2: and  <e=%rax,<s=%r9
and  % rax, % r9

# qhasm: p = *(uint8  *) (input_1 + 676)
# asm 1: movzbq 676(<input_1=int64#2),>p=int64#7
# asm 2: movzbq 676(<input_1=%rsi),>p=%rax
movzbq 676( % rsi), % rax

# qhasm: e = *(uint8  *) (input_2 + 676)
# asm 1: movzbq 676(<input_2=int64#3),>e=int64#8
# asm 2: movzbq 676(<input_2=%rdx),>e=%r10
movzbq 676( % rdx), % r10

# qhasm: p &= e
# asm 1: and  <e=int64#8,<p=int64#7
# asm 2: and  <e=%r10,<p=%rax
and  % r10, % rax

# qhasm: s ^= p
# asm 1: xor  <p=int64#7,<s=int64#6
# asm 2: xor  <p=%rax,<s=%r9
xor  % rax, % r9

# qhasm: c_all = count(s)
# asm 1: popcnt <s=int64#6, >c_all=int64#6
# asm 2: popcnt <s=%r9, >c_all=%r9
popcnt % r9, % r9

# qhasm: b64 = mem64[ buf_ptr + 0 ]
# asm 1: movq   0(<buf_ptr=int64#4),>b64=int64#7
# asm 2: movq   0(<buf_ptr=%rcx),>b64=%rax
movq   0( % rcx), % rax

# qhasm: c = count(b64)
# asm 1: popcnt <b64=int64#7, >c=int64#7
# asm 2: popcnt <b64=%rax, >c=%rax
popcnt % rax, % rax

# qhasm: c_all ^= c
# asm 1: xor  <c=int64#7,<c_all=int64#6
# asm 2: xor  <c=%rax,<c_all=%r9
xor  % rax, % r9

# qhasm: b64 = mem64[ buf_ptr + 8 ]
# asm 1: movq   8(<buf_ptr=int64#4),>b64=int64#7
# asm 2: movq   8(<buf_ptr=%rcx),>b64=%rax
movq   8( % rcx), % rax

# qhasm: c = count(b64)
# asm 1: popcnt <b64=int64#7, >c=int64#7
# asm 2: popcnt <b64=%rax, >c=%rax
popcnt % rax, % rax

# qhasm: c_all ^= c
# asm 1: xor  <c=int64#7,<c_all=int64#6
# asm 2: xor  <c=%rax,<c_all=%r9
xor  % rax, % r9

# qhasm: b64 = mem64[ buf_ptr + 16 ]
# asm 1: movq   16(<buf_ptr=int64#4),>b64=int64#7
# asm 2: movq   16(<buf_ptr=%rcx),>b64=%rax
movq   16( % rcx), % rax

# qhasm: c = count(b64)
# asm 1: popcnt <b64=int64#7, >c=int64#7
# asm 2: popcnt <b64=%rax, >c=%rax
popcnt % rax, % rax

# qhasm: c_all ^= c
# asm 1: xor  <c=int64#7,<c_all=int64#6
# asm 2: xor  <c=%rax,<c_all=%r9
xor  % rax, % r9

# qhasm: b64 = mem64[ buf_ptr + 24 ]
# asm 1: movq   24(<buf_ptr=int64#4),>b64=int64#7
# asm 2: movq   24(<buf_ptr=%rcx),>b64=%rax
movq   24( % rcx), % rax

# qhasm: c = count(b64)
# asm 1: popcnt <b64=int64#7, >c=int64#7
# asm 2: popcnt <b64=%rax, >c=%rax
popcnt % rax, % rax

# qhasm: c_all ^= c
# asm 1: xor  <c=int64#7,<c_all=int64#6
# asm 2: xor  <c=%rax,<c_all=%r9
xor  % rax, % r9

# qhasm: addr = row
# asm 1: mov  <row=int64#5,>addr=int64#7
# asm 2: mov  <row=%r8,>addr=%rax
mov  % r8, % rax

# qhasm: (uint64) addr >>= 3
# asm 1: shr  $3,<addr=int64#7
# asm 2: shr  $3,<addr=%rax
shr  $3, % rax

# qhasm: addr += input_0
# asm 1: add  <input_0=int64#1,<addr=int64#7
# asm 2: add  <input_0=%rdi,<addr=%rax
add  % rdi, % rax

# qhasm: synd = *(uint8  *) (addr + 0)
# asm 1: movzbq 0(<addr=int64#7),>synd=int64#8
# asm 2: movzbq 0(<addr=%rax),>synd=%r10
movzbq 0( % rax), % r10

# qhasm: synd <<= 1
# asm 1: shl  $1,<synd=int64#8
# asm 2: shl  $1,<synd=%r10
shl  $1, % r10

# qhasm: (uint32) c_all &= 1
# asm 1: and  $1,<c_all=int64#6d
# asm 2: and  $1,<c_all=%r9d
and  $1, % r9d

# qhasm: synd |= c_all
# asm 1: or   <c_all=int64#6,<synd=int64#8
# asm 2: or   <c_all=%r9,<synd=%r10
or   % r9, % r10

# qhasm: *(uint8  *) (addr + 0) = synd
# asm 1: movb   <synd=int64#8b,0(<addr=int64#7)
# asm 2: movb   <synd=%r10b,0(<addr=%rax)
movb   % r10b, 0( % rax)

# qhasm: =? row-0
# asm 1: cmp  $0,<row=int64#5
# asm 2: cmp  $0,<row=%r8
cmp  $0, % r8
# comment:fp stack unchanged by jump

# qhasm: goto loop if !=
jne ._loop

# qhasm: i = 676
# asm 1: mov  $676,>i=int64#2
# asm 2: mov  $676,>i=%rsi
mov  $676, % rsi

# qhasm: inner2:
._inner2:

# qhasm: i -= 1
# asm 1: sub  $1,<i=int64#2
# asm 2: sub  $1,<i=%rsi
sub  $1, % rsi

# qhasm: addr = input_2 + i
# asm 1: lea  (<input_2=int64#3,<i=int64#2),>addr=int64#4
# asm 2: lea  (<input_2=%rdx,<i=%rsi),>addr=%rcx
lea  ( % rdx, % rsi), % rcx

# qhasm: b0 = *(uint8  *) (addr + 0)
# asm 1: movzbq 0(<addr=int64#4),>b0=int64#5
# asm 2: movzbq 0(<addr=%rcx),>b0=%r8
movzbq 0( % rcx), % r8

# qhasm: b1 = *(uint8  *) (addr + 1)
# asm 1: movzbq 1(<addr=int64#4),>b1=int64#6
# asm 2: movzbq 1(<addr=%rcx),>b1=%r9
movzbq 1( % rcx), % r9

# qhasm: (uint64) b0 >>= 5
# asm 1: shr  $5,<b0=int64#5
# asm 2: shr  $5,<b0=%r8
shr  $5, % r8

# qhasm: b1 <<= 3
# asm 1: shl  $3,<b1=int64#6
# asm 2: shl  $3,<b1=%r9
shl  $3, % r9

# qhasm: b1 |= b0
# asm 1: or   <b0=int64#5,<b1=int64#6
# asm 2: or   <b0=%r8,<b1=%r9
or   % r8, % r9

# qhasm: *(uint8  *) (addr + 1) = b1
# asm 1: movb   <b1=int64#6b,1(<addr=int64#4)
# asm 2: movb   <b1=%r9b,1(<addr=%rcx)
movb   % r9b, 1( % rcx)

# qhasm: =? i-0
# asm 1: cmp  $0,<i=int64#2
# asm 2: cmp  $0,<i=%rsi
cmp  $0, % rsi
# comment:fp stack unchanged by jump

# qhasm: goto inner2 if !=
jne ._inner2

# qhasm: tmp = back
# asm 1: movq <back=stack64#1,>tmp=int64#2
# asm 2: movq <back=32(%rsp),>tmp=%rsi
movq 32( % rsp), % rsi

# qhasm: *(uint8  *) (input_2 + 0) = tmp
# asm 1: movb   <tmp=int64#2b,0(<input_2=int64#3)
# asm 2: movb   <tmp=%sil,0(<input_2=%rdx)
movb   % sil, 0( % rdx)

# qhasm: input_2 -= 193
# asm 1: sub  $193,<input_2=int64#3
# asm 2: sub  $193,<input_2=%rdx
sub  $193, % rdx

# qhasm: i = 0
# asm 1: mov  $0,>i=int64#2
# asm 2: mov  $0,>i=%rsi
mov  $0, % rsi

# qhasm: inner3:
._inner3:

# qhasm: s = *(uint8  *) (input_0 + 0)
# asm 1: movzbq 0(<input_0=int64#1),>s=int64#4
# asm 2: movzbq 0(<input_0=%rdi),>s=%rcx
movzbq 0( % rdi), % rcx

# qhasm: e = *(uint8  *) (input_2 + 0)
# asm 1: movzbq 0(<input_2=int64#3),>e=int64#5
# asm 2: movzbq 0(<input_2=%rdx),>e=%r8
movzbq 0( % rdx), % r8

# qhasm: s ^= e
# asm 1: xor  <e=int64#5,<s=int64#4
# asm 2: xor  <e=%r8,<s=%rcx
xor  % r8, % rcx

# qhasm: *(uint8  *) (input_0 + 0) = s
# asm 1: movb   <s=int64#4b,0(<input_0=int64#1)
# asm 2: movb   <s=%cl,0(<input_0=%rdi)
movb   % cl, 0( % rdi)

# qhasm: i += 1
# asm 1: add  $1,<i=int64#2
# asm 2: add  $1,<i=%rsi
add  $1, % rsi

# qhasm: input_0 += 1
# asm 1: add  $1,<input_0=int64#1
# asm 2: add  $1,<input_0=%rdi
add  $1, % rdi

# qhasm: input_2 += 1
# asm 1: add  $1,<input_2=int64#3
# asm 2: add  $1,<input_2=%rdx
add  $1, % rdx

# qhasm: =? i-193
# asm 1: cmp  $193,<i=int64#2
# asm 2: cmp  $193,<i=%rsi
cmp  $193, % rsi
# comment:fp stack unchanged by jump

# qhasm: goto inner3 if !=
jne ._inner3

# qhasm: s = *(uint8  *) (input_0 + 0)
# asm 1: movzbq 0(<input_0=int64#1),>s=int64#2
# asm 2: movzbq 0(<input_0=%rdi),>s=%rsi
movzbq 0( % rdi), % rsi

# qhasm: e = *(uint8  *) (input_2 + 0)
# asm 1: movzbq 0(<input_2=int64#3),>e=int64#3
# asm 2: movzbq 0(<input_2=%rdx),>e=%rdx
movzbq 0( % rdx), % rdx

# qhasm: (uint32) e &= 7
# asm 1: and  $7,<e=int64#3d
# asm 2: and  $7,<e=%edx
and  $7, % edx

# qhasm: s ^= e
# asm 1: xor  <e=int64#3,<s=int64#2
# asm 2: xor  <e=%rdx,<s=%rsi
xor  % rdx, % rsi

# qhasm: *(uint8  *) (input_0 + 0) = s
# asm 1: movb   <s=int64#2b,0(<input_0=int64#1)
# asm 2: movb   <s=%sil,0(<input_0=%rdi)
movb   % sil, 0( % rdi)

# qhasm: return
add % r11, % rsp
ret
