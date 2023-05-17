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

# qhasm: int64 p

# qhasm: int64 e

# qhasm: int64 s

# qhasm: reg256 pp

# qhasm: reg256 ee

# qhasm: reg256 ss

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
add $32, % r11
sub % r11, % rsp

# qhasm: input_1 += 1357008
# asm 1: add  $1357008,<input_1=int64#2
# asm 2: add  $1357008,<input_1=%rsi
add  $1357008, % rsi

# qhasm: buf_ptr = &buf
# asm 1: leaq <buf=stack256#1,>buf_ptr=int64#4
# asm 2: leaq <buf=0(%rsp),>buf_ptr=%rcx
leaq 0( % rsp), % rcx

# qhasm: row = 1664
# asm 1: mov  $1664,>row=int64#5
# asm 2: mov  $1664,>row=%r8
mov  $1664, % r8

# qhasm: loop:
._loop:

# qhasm: row -= 1
# asm 1: sub  $1,<row=int64#5
# asm 2: sub  $1,<row=%r8
sub  $1, % r8

# qhasm: ss = mem256[ input_1 + 0 ]
# asm 1: vmovupd   0(<input_1=int64#2),>ss=reg256#1
# asm 2: vmovupd   0(<input_1=%rsi),>ss=%ymm0
vmovupd   0( % rsi), % ymm0

# qhasm: ee = mem256[ input_2 + 208 ]
# asm 1: vmovupd   208(<input_2=int64#3),>ee=reg256#2
# asm 2: vmovupd   208(<input_2=%rdx),>ee=%ymm1
vmovupd   208( % rdx), % ymm1

# qhasm: ss &= ee
# asm 1: vpand <ee=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpand <ee=%ymm1,<ss=%ymm0,<ss=%ymm0
vpand % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 32 ]
# asm 1: vmovupd   32(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   32(<input_1=%rsi),>pp=%ymm1
vmovupd   32( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 240 ]
# asm 1: vmovupd   240(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   240(<input_2=%rdx),>ee=%ymm2
vmovupd   240( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 272 ]
# asm 1: vmovupd   272(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   272(<input_2=%rdx),>ee=%ymm2
vmovupd   272( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 304 ]
# asm 1: vmovupd   304(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   304(<input_2=%rdx),>ee=%ymm2
vmovupd   304( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 336 ]
# asm 1: vmovupd   336(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   336(<input_2=%rdx),>ee=%ymm2
vmovupd   336( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 368 ]
# asm 1: vmovupd   368(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   368(<input_2=%rdx),>ee=%ymm2
vmovupd   368( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 400 ]
# asm 1: vmovupd   400(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   400(<input_2=%rdx),>ee=%ymm2
vmovupd   400( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 432 ]
# asm 1: vmovupd   432(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   432(<input_2=%rdx),>ee=%ymm2
vmovupd   432( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 464 ]
# asm 1: vmovupd   464(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   464(<input_2=%rdx),>ee=%ymm2
vmovupd   464( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 496 ]
# asm 1: vmovupd   496(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   496(<input_2=%rdx),>ee=%ymm2
vmovupd   496( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 528 ]
# asm 1: vmovupd   528(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   528(<input_2=%rdx),>ee=%ymm2
vmovupd   528( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 560 ]
# asm 1: vmovupd   560(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   560(<input_2=%rdx),>ee=%ymm2
vmovupd   560( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 592 ]
# asm 1: vmovupd   592(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   592(<input_2=%rdx),>ee=%ymm2
vmovupd   592( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 624 ]
# asm 1: vmovupd   624(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   624(<input_2=%rdx),>ee=%ymm2
vmovupd   624( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 656 ]
# asm 1: vmovupd   656(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   656(<input_2=%rdx),>ee=%ymm2
vmovupd   656( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 688 ]
# asm 1: vmovupd   688(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   688(<input_2=%rdx),>ee=%ymm2
vmovupd   688( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 720 ]
# asm 1: vmovupd   720(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   720(<input_2=%rdx),>ee=%ymm2
vmovupd   720( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 752 ]
# asm 1: vmovupd   752(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   752(<input_2=%rdx),>ee=%ymm2
vmovupd   752( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 784 ]
# asm 1: vmovupd   784(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   784(<input_2=%rdx),>ee=%ymm2
vmovupd   784( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 816 ]
# asm 1: vmovupd   816(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   816(<input_2=%rdx),>ee=%ymm2
vmovupd   816( % rdx), % ymm2

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

# qhasm: ee = mem256[ input_2 + 848 ]
# asm 1: vmovupd   848(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   848(<input_2=%rdx),>ee=%ymm2
vmovupd   848( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 672 ]
# asm 1: vmovupd   672(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   672(<input_1=%rsi),>pp=%ymm1
vmovupd   672( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 880 ]
# asm 1: vmovupd   880(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   880(<input_2=%rdx),>ee=%ymm2
vmovupd   880( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 704 ]
# asm 1: vmovupd   704(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   704(<input_1=%rsi),>pp=%ymm1
vmovupd   704( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 912 ]
# asm 1: vmovupd   912(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   912(<input_2=%rdx),>ee=%ymm2
vmovupd   912( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 736 ]
# asm 1: vmovupd   736(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   736(<input_1=%rsi),>pp=%ymm1
vmovupd   736( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 944 ]
# asm 1: vmovupd   944(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   944(<input_2=%rdx),>ee=%ymm2
vmovupd   944( % rdx), % ymm2

# qhasm: pp &= ee
# asm 1: vpand <ee=reg256#3,<pp=reg256#2,<pp=reg256#2
# asm 2: vpand <ee=%ymm2,<pp=%ymm1,<pp=%ymm1
vpand % ymm2, % ymm1, % ymm1

# qhasm: ss ^= pp
# asm 1: vpxor <pp=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <pp=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: pp = mem256[ input_1 + 768 ]
# asm 1: vmovupd   768(<input_1=int64#2),>pp=reg256#2
# asm 2: vmovupd   768(<input_1=%rsi),>pp=%ymm1
vmovupd   768( % rsi), % ymm1

# qhasm: ee = mem256[ input_2 + 976 ]
# asm 1: vmovupd   976(<input_2=int64#3),>ee=reg256#3
# asm 2: vmovupd   976(<input_2=%rdx),>ee=%ymm2
vmovupd   976( % rdx), % ymm2

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

# qhasm: s = mem64[input_1 + 800]
# asm 1: movq   800(<input_1=int64#2),>s=int64#6
# asm 2: movq   800(<input_1=%rsi),>s=%r9
movq   800( % rsi), % r9

# qhasm: e = mem64[input_2 + 1008]
# asm 1: movq   1008(<input_2=int64#3),>e=int64#7
# asm 2: movq   1008(<input_2=%rdx),>e=%rax
movq   1008( % rdx), % rax

# qhasm: s &= e
# asm 1: and  <e=int64#7,<s=int64#6
# asm 2: and  <e=%rax,<s=%r9
and  % rax, % r9

# qhasm: p = mem64[input_1 + 808]
# asm 1: movq   808(<input_1=int64#2),>p=int64#7
# asm 2: movq   808(<input_1=%rsi),>p=%rax
movq   808( % rsi), % rax

# qhasm: e = mem64[input_2 + 1016]
# asm 1: movq   1016(<input_2=int64#3),>e=int64#8
# asm 2: movq   1016(<input_2=%rdx),>e=%r10
movq   1016( % rdx), % r10

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

# qhasm: input_1 -= 816
# asm 1: sub  $816,<input_1=int64#2
# asm 2: sub  $816,<input_1=%rsi
sub  $816, % rsi

# qhasm: =? row-0
# asm 1: cmp  $0,<row=int64#5
# asm 2: cmp  $0,<row=%r8
cmp  $0, % r8
# comment:fp stack unchanged by jump

# qhasm: goto loop if !=
jne ._loop

# qhasm: ss = mem256[ input_0 + 0 ]
# asm 1: vmovupd   0(<input_0=int64#1),>ss=reg256#1
# asm 2: vmovupd   0(<input_0=%rdi),>ss=%ymm0
vmovupd   0( % rdi), % ymm0

# qhasm: ee = mem256[ input_2 + 0 ]
# asm 1: vmovupd   0(<input_2=int64#3),>ee=reg256#2
# asm 2: vmovupd   0(<input_2=%rdx),>ee=%ymm1
vmovupd   0( % rdx), % ymm1

# qhasm: ss ^= ee
# asm 1: vpxor <ee=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <ee=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: mem256[ input_0 + 0 ] = ss
# asm 1: vmovupd   <ss=reg256#1,0(<input_0=int64#1)
# asm 2: vmovupd   <ss=%ymm0,0(<input_0=%rdi)
vmovupd   % ymm0, 0( % rdi)

# qhasm: ss = mem256[ input_0 + 32 ]
# asm 1: vmovupd   32(<input_0=int64#1),>ss=reg256#1
# asm 2: vmovupd   32(<input_0=%rdi),>ss=%ymm0
vmovupd   32( % rdi), % ymm0

# qhasm: ee = mem256[ input_2 + 32 ]
# asm 1: vmovupd   32(<input_2=int64#3),>ee=reg256#2
# asm 2: vmovupd   32(<input_2=%rdx),>ee=%ymm1
vmovupd   32( % rdx), % ymm1

# qhasm: ss ^= ee
# asm 1: vpxor <ee=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <ee=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: mem256[ input_0 + 32 ] = ss
# asm 1: vmovupd   <ss=reg256#1,32(<input_0=int64#1)
# asm 2: vmovupd   <ss=%ymm0,32(<input_0=%rdi)
vmovupd   % ymm0, 32( % rdi)

# qhasm: ss = mem256[ input_0 + 64 ]
# asm 1: vmovupd   64(<input_0=int64#1),>ss=reg256#1
# asm 2: vmovupd   64(<input_0=%rdi),>ss=%ymm0
vmovupd   64( % rdi), % ymm0

# qhasm: ee = mem256[ input_2 + 64 ]
# asm 1: vmovupd   64(<input_2=int64#3),>ee=reg256#2
# asm 2: vmovupd   64(<input_2=%rdx),>ee=%ymm1
vmovupd   64( % rdx), % ymm1

# qhasm: ss ^= ee
# asm 1: vpxor <ee=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <ee=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: mem256[ input_0 + 64 ] = ss
# asm 1: vmovupd   <ss=reg256#1,64(<input_0=int64#1)
# asm 2: vmovupd   <ss=%ymm0,64(<input_0=%rdi)
vmovupd   % ymm0, 64( % rdi)

# qhasm: ss = mem256[ input_0 + 96 ]
# asm 1: vmovupd   96(<input_0=int64#1),>ss=reg256#1
# asm 2: vmovupd   96(<input_0=%rdi),>ss=%ymm0
vmovupd   96( % rdi), % ymm0

# qhasm: ee = mem256[ input_2 + 96 ]
# asm 1: vmovupd   96(<input_2=int64#3),>ee=reg256#2
# asm 2: vmovupd   96(<input_2=%rdx),>ee=%ymm1
vmovupd   96( % rdx), % ymm1

# qhasm: ss ^= ee
# asm 1: vpxor <ee=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <ee=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: mem256[ input_0 + 96 ] = ss
# asm 1: vmovupd   <ss=reg256#1,96(<input_0=int64#1)
# asm 2: vmovupd   <ss=%ymm0,96(<input_0=%rdi)
vmovupd   % ymm0, 96( % rdi)

# qhasm: ss = mem256[ input_0 + 128 ]
# asm 1: vmovupd   128(<input_0=int64#1),>ss=reg256#1
# asm 2: vmovupd   128(<input_0=%rdi),>ss=%ymm0
vmovupd   128( % rdi), % ymm0

# qhasm: ee = mem256[ input_2 + 128 ]
# asm 1: vmovupd   128(<input_2=int64#3),>ee=reg256#2
# asm 2: vmovupd   128(<input_2=%rdx),>ee=%ymm1
vmovupd   128( % rdx), % ymm1

# qhasm: ss ^= ee
# asm 1: vpxor <ee=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <ee=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: mem256[ input_0 + 128 ] = ss
# asm 1: vmovupd   <ss=reg256#1,128(<input_0=int64#1)
# asm 2: vmovupd   <ss=%ymm0,128(<input_0=%rdi)
vmovupd   % ymm0, 128( % rdi)

# qhasm: ss = mem256[ input_0 + 160 ]
# asm 1: vmovupd   160(<input_0=int64#1),>ss=reg256#1
# asm 2: vmovupd   160(<input_0=%rdi),>ss=%ymm0
vmovupd   160( % rdi), % ymm0

# qhasm: ee = mem256[ input_2 + 160 ]
# asm 1: vmovupd   160(<input_2=int64#3),>ee=reg256#2
# asm 2: vmovupd   160(<input_2=%rdx),>ee=%ymm1
vmovupd   160( % rdx), % ymm1

# qhasm: ss ^= ee
# asm 1: vpxor <ee=reg256#2,<ss=reg256#1,<ss=reg256#1
# asm 2: vpxor <ee=%ymm1,<ss=%ymm0,<ss=%ymm0
vpxor % ymm1, % ymm0, % ymm0

# qhasm: mem256[ input_0 + 160 ] = ss
# asm 1: vmovupd   <ss=reg256#1,160(<input_0=int64#1)
# asm 2: vmovupd   <ss=%ymm0,160(<input_0=%rdi)
vmovupd   % ymm0, 160( % rdi)

# qhasm: s = mem64[ input_0 + 192 ]
# asm 1: movq   192(<input_0=int64#1),>s=int64#2
# asm 2: movq   192(<input_0=%rdi),>s=%rsi
movq   192( % rdi), % rsi

# qhasm: e = mem64[ input_2 + 192 ]
# asm 1: movq   192(<input_2=int64#3),>e=int64#4
# asm 2: movq   192(<input_2=%rdx),>e=%rcx
movq   192( % rdx), % rcx

# qhasm: s ^= e
# asm 1: xor  <e=int64#4,<s=int64#2
# asm 2: xor  <e=%rcx,<s=%rsi
xor  % rcx, % rsi

# qhasm: mem64[ input_0 + 192 ] = s
# asm 1: movq   <s=int64#2,192(<input_0=int64#1)
# asm 2: movq   <s=%rsi,192(<input_0=%rdi)
movq   % rsi, 192( % rdi)

# qhasm: s = mem64[ input_0 + 200 ]
# asm 1: movq   200(<input_0=int64#1),>s=int64#2
# asm 2: movq   200(<input_0=%rdi),>s=%rsi
movq   200( % rdi), % rsi

# qhasm: e = mem64[ input_2 + 200 ]
# asm 1: movq   200(<input_2=int64#3),>e=int64#3
# asm 2: movq   200(<input_2=%rdx),>e=%rdx
movq   200( % rdx), % rdx

# qhasm: s ^= e
# asm 1: xor  <e=int64#3,<s=int64#2
# asm 2: xor  <e=%rdx,<s=%rsi
xor  % rdx, % rsi

# qhasm: mem64[ input_0 + 200 ] = s
# asm 1: movq   <s=int64#2,200(<input_0=int64#1)
# asm 2: movq   <s=%rsi,200(<input_0=%rdi)
movq   % rsi, 200( % rdi)

# qhasm: return
add % r11, % rsp
ret
