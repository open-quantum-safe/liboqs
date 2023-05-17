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

# qhasm: int64 t0

# qhasm: int64 t1

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

# qhasm: t0 = mem64[ input_0 + 192 ]
# asm 1: movq   192(<input_0=int64#1),>t0=int64#2
# asm 2: movq   192(<input_0=%rdi),>t0=%rsi
movq   192( % rdi), % rsi

# qhasm: t1 = mem64[ input_0 + 200 ]
# asm 1: movq   200(<input_0=int64#1),>t1=int64#3
# asm 2: movq   200(<input_0=%rdi),>t1=%rdx
movq   200( % rdi), % rdx

# qhasm: t0 ^= t1
# asm 1: xor  <t1=int64#3,<t0=int64#2
# asm 2: xor  <t1=%rdx,<t0=%rsi
xor  % rdx, % rsi

# qhasm: c = count(t0)
# asm 1: popcnt <t0=int64#2, >c=int64#2
# asm 2: popcnt <t0=%rsi, >c=%rsi
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

# qhasm: t0 = mem64[ input_0 + 176 ]
# asm 1: movq   176(<input_0=int64#1),>t0=int64#2
# asm 2: movq   176(<input_0=%rdi),>t0=%rsi
movq   176( % rdi), % rsi

# qhasm: t1 = mem64[ input_0 + 184 ]
# asm 1: movq   184(<input_0=int64#1),>t1=int64#3
# asm 2: movq   184(<input_0=%rdi),>t1=%rdx
movq   184( % rdi), % rdx

# qhasm: t0 ^= t1
# asm 1: xor  <t1=int64#3,<t0=int64#2
# asm 2: xor  <t1=%rdx,<t0=%rsi
xor  % rdx, % rsi

# qhasm: c = count(t0)
# asm 1: popcnt <t0=int64#2, >c=int64#2
# asm 2: popcnt <t0=%rsi, >c=%rsi
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

# qhasm: t0 = mem64[ input_0 + 160 ]
# asm 1: movq   160(<input_0=int64#1),>t0=int64#2
# asm 2: movq   160(<input_0=%rdi),>t0=%rsi
movq   160( % rdi), % rsi

# qhasm: t1 = mem64[ input_0 + 168 ]
# asm 1: movq   168(<input_0=int64#1),>t1=int64#3
# asm 2: movq   168(<input_0=%rdi),>t1=%rdx
movq   168( % rdi), % rdx

# qhasm: t0 ^= t1
# asm 1: xor  <t1=int64#3,<t0=int64#2
# asm 2: xor  <t1=%rdx,<t0=%rsi
xor  % rdx, % rsi

# qhasm: c = count(t0)
# asm 1: popcnt <t0=int64#2, >c=int64#2
# asm 2: popcnt <t0=%rsi, >c=%rsi
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

# qhasm: t0 = mem64[ input_0 + 144 ]
# asm 1: movq   144(<input_0=int64#1),>t0=int64#2
# asm 2: movq   144(<input_0=%rdi),>t0=%rsi
movq   144( % rdi), % rsi

# qhasm: t1 = mem64[ input_0 + 152 ]
# asm 1: movq   152(<input_0=int64#1),>t1=int64#3
# asm 2: movq   152(<input_0=%rdi),>t1=%rdx
movq   152( % rdi), % rdx

# qhasm: t0 ^= t1
# asm 1: xor  <t1=int64#3,<t0=int64#2
# asm 2: xor  <t1=%rdx,<t0=%rsi
xor  % rdx, % rsi

# qhasm: c = count(t0)
# asm 1: popcnt <t0=int64#2, >c=int64#2
# asm 2: popcnt <t0=%rsi, >c=%rsi
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

# qhasm: t0 = mem64[ input_0 + 128 ]
# asm 1: movq   128(<input_0=int64#1),>t0=int64#2
# asm 2: movq   128(<input_0=%rdi),>t0=%rsi
movq   128( % rdi), % rsi

# qhasm: t1 = mem64[ input_0 + 136 ]
# asm 1: movq   136(<input_0=int64#1),>t1=int64#3
# asm 2: movq   136(<input_0=%rdi),>t1=%rdx
movq   136( % rdi), % rdx

# qhasm: t0 ^= t1
# asm 1: xor  <t1=int64#3,<t0=int64#2
# asm 2: xor  <t1=%rdx,<t0=%rsi
xor  % rdx, % rsi

# qhasm: c = count(t0)
# asm 1: popcnt <t0=int64#2, >c=int64#2
# asm 2: popcnt <t0=%rsi, >c=%rsi
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

# qhasm: t0 = mem64[ input_0 + 112 ]
# asm 1: movq   112(<input_0=int64#1),>t0=int64#2
# asm 2: movq   112(<input_0=%rdi),>t0=%rsi
movq   112( % rdi), % rsi

# qhasm: t1 = mem64[ input_0 + 120 ]
# asm 1: movq   120(<input_0=int64#1),>t1=int64#3
# asm 2: movq   120(<input_0=%rdi),>t1=%rdx
movq   120( % rdi), % rdx

# qhasm: t0 ^= t1
# asm 1: xor  <t1=int64#3,<t0=int64#2
# asm 2: xor  <t1=%rdx,<t0=%rsi
xor  % rdx, % rsi

# qhasm: c = count(t0)
# asm 1: popcnt <t0=int64#2, >c=int64#2
# asm 2: popcnt <t0=%rsi, >c=%rsi
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

# qhasm: t0 = mem64[ input_0 + 96 ]
# asm 1: movq   96(<input_0=int64#1),>t0=int64#2
# asm 2: movq   96(<input_0=%rdi),>t0=%rsi
movq   96( % rdi), % rsi

# qhasm: t1 = mem64[ input_0 + 104 ]
# asm 1: movq   104(<input_0=int64#1),>t1=int64#3
# asm 2: movq   104(<input_0=%rdi),>t1=%rdx
movq   104( % rdi), % rdx

# qhasm: t0 ^= t1
# asm 1: xor  <t1=int64#3,<t0=int64#2
# asm 2: xor  <t1=%rdx,<t0=%rsi
xor  % rdx, % rsi

# qhasm: c = count(t0)
# asm 1: popcnt <t0=int64#2, >c=int64#2
# asm 2: popcnt <t0=%rsi, >c=%rsi
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

# qhasm: t0 = mem64[ input_0 + 80 ]
# asm 1: movq   80(<input_0=int64#1),>t0=int64#2
# asm 2: movq   80(<input_0=%rdi),>t0=%rsi
movq   80( % rdi), % rsi

# qhasm: t1 = mem64[ input_0 + 88 ]
# asm 1: movq   88(<input_0=int64#1),>t1=int64#3
# asm 2: movq   88(<input_0=%rdi),>t1=%rdx
movq   88( % rdi), % rdx

# qhasm: t0 ^= t1
# asm 1: xor  <t1=int64#3,<t0=int64#2
# asm 2: xor  <t1=%rdx,<t0=%rsi
xor  % rdx, % rsi

# qhasm: c = count(t0)
# asm 1: popcnt <t0=int64#2, >c=int64#2
# asm 2: popcnt <t0=%rsi, >c=%rsi
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

# qhasm: t0 = mem64[ input_0 + 64 ]
# asm 1: movq   64(<input_0=int64#1),>t0=int64#2
# asm 2: movq   64(<input_0=%rdi),>t0=%rsi
movq   64( % rdi), % rsi

# qhasm: t1 = mem64[ input_0 + 72 ]
# asm 1: movq   72(<input_0=int64#1),>t1=int64#3
# asm 2: movq   72(<input_0=%rdi),>t1=%rdx
movq   72( % rdi), % rdx

# qhasm: t0 ^= t1
# asm 1: xor  <t1=int64#3,<t0=int64#2
# asm 2: xor  <t1=%rdx,<t0=%rsi
xor  % rdx, % rsi

# qhasm: c = count(t0)
# asm 1: popcnt <t0=int64#2, >c=int64#2
# asm 2: popcnt <t0=%rsi, >c=%rsi
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

# qhasm: t0 = mem64[ input_0 + 48 ]
# asm 1: movq   48(<input_0=int64#1),>t0=int64#2
# asm 2: movq   48(<input_0=%rdi),>t0=%rsi
movq   48( % rdi), % rsi

# qhasm: t1 = mem64[ input_0 + 56 ]
# asm 1: movq   56(<input_0=int64#1),>t1=int64#3
# asm 2: movq   56(<input_0=%rdi),>t1=%rdx
movq   56( % rdi), % rdx

# qhasm: t0 ^= t1
# asm 1: xor  <t1=int64#3,<t0=int64#2
# asm 2: xor  <t1=%rdx,<t0=%rsi
xor  % rdx, % rsi

# qhasm: c = count(t0)
# asm 1: popcnt <t0=int64#2, >c=int64#2
# asm 2: popcnt <t0=%rsi, >c=%rsi
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

# qhasm: t0 = mem64[ input_0 + 32 ]
# asm 1: movq   32(<input_0=int64#1),>t0=int64#2
# asm 2: movq   32(<input_0=%rdi),>t0=%rsi
movq   32( % rdi), % rsi

# qhasm: t1 = mem64[ input_0 + 40 ]
# asm 1: movq   40(<input_0=int64#1),>t1=int64#3
# asm 2: movq   40(<input_0=%rdi),>t1=%rdx
movq   40( % rdi), % rdx

# qhasm: t0 ^= t1
# asm 1: xor  <t1=int64#3,<t0=int64#2
# asm 2: xor  <t1=%rdx,<t0=%rsi
xor  % rdx, % rsi

# qhasm: c = count(t0)
# asm 1: popcnt <t0=int64#2, >c=int64#2
# asm 2: popcnt <t0=%rsi, >c=%rsi
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

# qhasm: t0 = mem64[ input_0 + 16 ]
# asm 1: movq   16(<input_0=int64#1),>t0=int64#2
# asm 2: movq   16(<input_0=%rdi),>t0=%rsi
movq   16( % rdi), % rsi

# qhasm: t1 = mem64[ input_0 + 24 ]
# asm 1: movq   24(<input_0=int64#1),>t1=int64#3
# asm 2: movq   24(<input_0=%rdi),>t1=%rdx
movq   24( % rdi), % rdx

# qhasm: t0 ^= t1
# asm 1: xor  <t1=int64#3,<t0=int64#2
# asm 2: xor  <t1=%rdx,<t0=%rsi
xor  % rdx, % rsi

# qhasm: c = count(t0)
# asm 1: popcnt <t0=int64#2, >c=int64#2
# asm 2: popcnt <t0=%rsi, >c=%rsi
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

# qhasm: t0 = mem64[ input_0 + 0 ]
# asm 1: movq   0(<input_0=int64#1),>t0=int64#2
# asm 2: movq   0(<input_0=%rdi),>t0=%rsi
movq   0( % rdi), % rsi

# qhasm: t1 = mem64[ input_0 + 8 ]
# asm 1: movq   8(<input_0=int64#1),>t1=int64#1
# asm 2: movq   8(<input_0=%rdi),>t1=%rdi
movq   8( % rdi), % rdi

# qhasm: t0 ^= t1
# asm 1: xor  <t1=int64#1,<t0=int64#2
# asm 2: xor  <t1=%rdi,<t0=%rsi
xor  % rdi, % rsi

# qhasm: c = count(t0)
# asm 1: popcnt <t0=int64#2, >c=int64#1
# asm 2: popcnt <t0=%rsi, >c=%rdi
popcnt % rsi, % rdi

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
