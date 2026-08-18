#include "namespace.h"

#define vec_reduce_asm CRYPTO_NAMESPACE(vec_reduce_asm)
#define _vec_reduce_asm _CRYPTO_NAMESPACE(vec_reduce_asm)

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

# qhasm: int64 t

# qhasm: int64 c

# qhasm: int64 r

# qhasm: enter vec_reduce_asm
.p2align 5
.global _vec_reduce_asm
.global vec_reduce_asm
_vec_reduce_asm:
vec_reduce_asm:
mov % rsp, % r11
and $31, % r11
add $0, % r11
sub % r11, % rsp

# qhasm: r = 0
# asm 1: mov  $0,>r=int64#7
# asm 2: mov  $0,>r=%rax
mov  $0, % rax

# qhasm: t = mem64[ input_0 + 88 ]
# asm 1: movq   88(<input_0=int64#1),>t=int64#2
# asm 2: movq   88(<input_0=%rdi),>t=%rsi
movq   88( % rdi), % rsi

# qhasm: c = count(t)
# asm 1: popcnt <t=int64#2, >c=int64#2
# asm 2: popcnt <t=%rsi, >c=%rsi
popcnt % rsi, % rsi

# qhasm: (uint32) c &= 1
# asm 1: and  $1,<c=int64#2d
# asm 2: and  $1,<c=%esi
and  $1, % esi

# qhasm: r <<= 1
# asm 1: shl  $1,<r=int64#7
# asm 2: shl  $1,<r=%rax
shl  $1, % rax

# qhasm: r |= c
# asm 1: or   <c=int64#2,<r=int64#7
# asm 2: or   <c=%rsi,<r=%rax
or   % rsi, % rax

# qhasm: t = mem64[ input_0 + 80 ]
# asm 1: movq   80(<input_0=int64#1),>t=int64#2
# asm 2: movq   80(<input_0=%rdi),>t=%rsi
movq   80( % rdi), % rsi

# qhasm: c = count(t)
# asm 1: popcnt <t=int64#2, >c=int64#2
# asm 2: popcnt <t=%rsi, >c=%rsi
popcnt % rsi, % rsi

# qhasm: (uint32) c &= 1
# asm 1: and  $1,<c=int64#2d
# asm 2: and  $1,<c=%esi
and  $1, % esi

# qhasm: r <<= 1
# asm 1: shl  $1,<r=int64#7
# asm 2: shl  $1,<r=%rax
shl  $1, % rax

# qhasm: r |= c
# asm 1: or   <c=int64#2,<r=int64#7
# asm 2: or   <c=%rsi,<r=%rax
or   % rsi, % rax

# qhasm: t = mem64[ input_0 + 72 ]
# asm 1: movq   72(<input_0=int64#1),>t=int64#2
# asm 2: movq   72(<input_0=%rdi),>t=%rsi
movq   72( % rdi), % rsi

# qhasm: c = count(t)
# asm 1: popcnt <t=int64#2, >c=int64#2
# asm 2: popcnt <t=%rsi, >c=%rsi
popcnt % rsi, % rsi

# qhasm: (uint32) c &= 1
# asm 1: and  $1,<c=int64#2d
# asm 2: and  $1,<c=%esi
and  $1, % esi

# qhasm: r <<= 1
# asm 1: shl  $1,<r=int64#7
# asm 2: shl  $1,<r=%rax
shl  $1, % rax

# qhasm: r |= c
# asm 1: or   <c=int64#2,<r=int64#7
# asm 2: or   <c=%rsi,<r=%rax
or   % rsi, % rax

# qhasm: t = mem64[ input_0 + 64 ]
# asm 1: movq   64(<input_0=int64#1),>t=int64#2
# asm 2: movq   64(<input_0=%rdi),>t=%rsi
movq   64( % rdi), % rsi

# qhasm: c = count(t)
# asm 1: popcnt <t=int64#2, >c=int64#2
# asm 2: popcnt <t=%rsi, >c=%rsi
popcnt % rsi, % rsi

# qhasm: (uint32) c &= 1
# asm 1: and  $1,<c=int64#2d
# asm 2: and  $1,<c=%esi
and  $1, % esi

# qhasm: r <<= 1
# asm 1: shl  $1,<r=int64#7
# asm 2: shl  $1,<r=%rax
shl  $1, % rax

# qhasm: r |= c
# asm 1: or   <c=int64#2,<r=int64#7
# asm 2: or   <c=%rsi,<r=%rax
or   % rsi, % rax

# qhasm: t = mem64[ input_0 + 56 ]
# asm 1: movq   56(<input_0=int64#1),>t=int64#2
# asm 2: movq   56(<input_0=%rdi),>t=%rsi
movq   56( % rdi), % rsi

# qhasm: c = count(t)
# asm 1: popcnt <t=int64#2, >c=int64#2
# asm 2: popcnt <t=%rsi, >c=%rsi
popcnt % rsi, % rsi

# qhasm: (uint32) c &= 1
# asm 1: and  $1,<c=int64#2d
# asm 2: and  $1,<c=%esi
and  $1, % esi

# qhasm: r <<= 1
# asm 1: shl  $1,<r=int64#7
# asm 2: shl  $1,<r=%rax
shl  $1, % rax

# qhasm: r |= c
# asm 1: or   <c=int64#2,<r=int64#7
# asm 2: or   <c=%rsi,<r=%rax
or   % rsi, % rax

# qhasm: t = mem64[ input_0 + 48 ]
# asm 1: movq   48(<input_0=int64#1),>t=int64#2
# asm 2: movq   48(<input_0=%rdi),>t=%rsi
movq   48( % rdi), % rsi

# qhasm: c = count(t)
# asm 1: popcnt <t=int64#2, >c=int64#2
# asm 2: popcnt <t=%rsi, >c=%rsi
popcnt % rsi, % rsi

# qhasm: (uint32) c &= 1
# asm 1: and  $1,<c=int64#2d
# asm 2: and  $1,<c=%esi
and  $1, % esi

# qhasm: r <<= 1
# asm 1: shl  $1,<r=int64#7
# asm 2: shl  $1,<r=%rax
shl  $1, % rax

# qhasm: r |= c
# asm 1: or   <c=int64#2,<r=int64#7
# asm 2: or   <c=%rsi,<r=%rax
or   % rsi, % rax

# qhasm: t = mem64[ input_0 + 40 ]
# asm 1: movq   40(<input_0=int64#1),>t=int64#2
# asm 2: movq   40(<input_0=%rdi),>t=%rsi
movq   40( % rdi), % rsi

# qhasm: c = count(t)
# asm 1: popcnt <t=int64#2, >c=int64#2
# asm 2: popcnt <t=%rsi, >c=%rsi
popcnt % rsi, % rsi

# qhasm: (uint32) c &= 1
# asm 1: and  $1,<c=int64#2d
# asm 2: and  $1,<c=%esi
and  $1, % esi

# qhasm: r <<= 1
# asm 1: shl  $1,<r=int64#7
# asm 2: shl  $1,<r=%rax
shl  $1, % rax

# qhasm: r |= c
# asm 1: or   <c=int64#2,<r=int64#7
# asm 2: or   <c=%rsi,<r=%rax
or   % rsi, % rax

# qhasm: t = mem64[ input_0 + 32 ]
# asm 1: movq   32(<input_0=int64#1),>t=int64#2
# asm 2: movq   32(<input_0=%rdi),>t=%rsi
movq   32( % rdi), % rsi

# qhasm: c = count(t)
# asm 1: popcnt <t=int64#2, >c=int64#2
# asm 2: popcnt <t=%rsi, >c=%rsi
popcnt % rsi, % rsi

# qhasm: (uint32) c &= 1
# asm 1: and  $1,<c=int64#2d
# asm 2: and  $1,<c=%esi
and  $1, % esi

# qhasm: r <<= 1
# asm 1: shl  $1,<r=int64#7
# asm 2: shl  $1,<r=%rax
shl  $1, % rax

# qhasm: r |= c
# asm 1: or   <c=int64#2,<r=int64#7
# asm 2: or   <c=%rsi,<r=%rax
or   % rsi, % rax

# qhasm: t = mem64[ input_0 + 24 ]
# asm 1: movq   24(<input_0=int64#1),>t=int64#2
# asm 2: movq   24(<input_0=%rdi),>t=%rsi
movq   24( % rdi), % rsi

# qhasm: c = count(t)
# asm 1: popcnt <t=int64#2, >c=int64#2
# asm 2: popcnt <t=%rsi, >c=%rsi
popcnt % rsi, % rsi

# qhasm: (uint32) c &= 1
# asm 1: and  $1,<c=int64#2d
# asm 2: and  $1,<c=%esi
and  $1, % esi

# qhasm: r <<= 1
# asm 1: shl  $1,<r=int64#7
# asm 2: shl  $1,<r=%rax
shl  $1, % rax

# qhasm: r |= c
# asm 1: or   <c=int64#2,<r=int64#7
# asm 2: or   <c=%rsi,<r=%rax
or   % rsi, % rax

# qhasm: t = mem64[ input_0 + 16 ]
# asm 1: movq   16(<input_0=int64#1),>t=int64#2
# asm 2: movq   16(<input_0=%rdi),>t=%rsi
movq   16( % rdi), % rsi

# qhasm: c = count(t)
# asm 1: popcnt <t=int64#2, >c=int64#2
# asm 2: popcnt <t=%rsi, >c=%rsi
popcnt % rsi, % rsi

# qhasm: (uint32) c &= 1
# asm 1: and  $1,<c=int64#2d
# asm 2: and  $1,<c=%esi
and  $1, % esi

# qhasm: r <<= 1
# asm 1: shl  $1,<r=int64#7
# asm 2: shl  $1,<r=%rax
shl  $1, % rax

# qhasm: r |= c
# asm 1: or   <c=int64#2,<r=int64#7
# asm 2: or   <c=%rsi,<r=%rax
or   % rsi, % rax

# qhasm: t = mem64[ input_0 + 8 ]
# asm 1: movq   8(<input_0=int64#1),>t=int64#2
# asm 2: movq   8(<input_0=%rdi),>t=%rsi
movq   8( % rdi), % rsi

# qhasm: c = count(t)
# asm 1: popcnt <t=int64#2, >c=int64#2
# asm 2: popcnt <t=%rsi, >c=%rsi
popcnt % rsi, % rsi

# qhasm: (uint32) c &= 1
# asm 1: and  $1,<c=int64#2d
# asm 2: and  $1,<c=%esi
and  $1, % esi

# qhasm: r <<= 1
# asm 1: shl  $1,<r=int64#7
# asm 2: shl  $1,<r=%rax
shl  $1, % rax

# qhasm: r |= c
# asm 1: or   <c=int64#2,<r=int64#7
# asm 2: or   <c=%rsi,<r=%rax
or   % rsi, % rax

# qhasm: t = mem64[ input_0 + 0 ]
# asm 1: movq   0(<input_0=int64#1),>t=int64#1
# asm 2: movq   0(<input_0=%rdi),>t=%rdi
movq   0( % rdi), % rdi

# qhasm: c = count(t)
# asm 1: popcnt <t=int64#1, >c=int64#1
# asm 2: popcnt <t=%rdi, >c=%rdi
popcnt % rdi, % rdi

# qhasm: (uint32) c &= 1
# asm 1: and  $1,<c=int64#1d
# asm 2: and  $1,<c=%edi
and  $1, % edi

# qhasm: r <<= 1
# asm 1: shl  $1,<r=int64#7
# asm 2: shl  $1,<r=%rax
shl  $1, % rax

# qhasm: r |= c
# asm 1: or   <c=int64#1,<r=int64#7
# asm 2: or   <c=%rdi,<r=%rax
or   % rdi, % rax

# qhasm: return r
add % r11, % rsp
ret
