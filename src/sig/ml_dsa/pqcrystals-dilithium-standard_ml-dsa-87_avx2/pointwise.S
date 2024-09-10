#include "params.h"
#include "consts.h"

.text
.global cdecl(pointwise_avx)
cdecl(pointwise_avx):
#consts
vmovdqa		_8XQINV*4(%rcx),%ymm0
vmovdqa		_8XQ*4(%rcx),%ymm1

xor		%eax,%eax
_looptop1:
#load
vmovdqa		(%rsi),%ymm2
vmovdqa		32(%rsi),%ymm4
vmovdqa		64(%rsi),%ymm6
vmovdqa		(%rdx),%ymm10
vmovdqa		32(%rdx),%ymm12
vmovdqa		64(%rdx),%ymm14
vpsrlq		$32,%ymm2,%ymm3
vpsrlq		$32,%ymm4,%ymm5
vmovshdup	%ymm6,%ymm7
vpsrlq		$32,%ymm10,%ymm11
vpsrlq		$32,%ymm12,%ymm13
vmovshdup	%ymm14,%ymm15

#mul
vpmuldq		%ymm2,%ymm10,%ymm2
vpmuldq		%ymm3,%ymm11,%ymm3
vpmuldq		%ymm4,%ymm12,%ymm4
vpmuldq		%ymm5,%ymm13,%ymm5
vpmuldq		%ymm6,%ymm14,%ymm6
vpmuldq		%ymm7,%ymm15,%ymm7

#reduce
vpmuldq		%ymm0,%ymm2,%ymm10
vpmuldq		%ymm0,%ymm3,%ymm11
vpmuldq		%ymm0,%ymm4,%ymm12
vpmuldq		%ymm0,%ymm5,%ymm13
vpmuldq		%ymm0,%ymm6,%ymm14
vpmuldq		%ymm0,%ymm7,%ymm15
vpmuldq		%ymm1,%ymm10,%ymm10
vpmuldq		%ymm1,%ymm11,%ymm11
vpmuldq		%ymm1,%ymm12,%ymm12
vpmuldq		%ymm1,%ymm13,%ymm13
vpmuldq		%ymm1,%ymm14,%ymm14
vpmuldq		%ymm1,%ymm15,%ymm15
vpsubq		%ymm10,%ymm2,%ymm2
vpsubq		%ymm11,%ymm3,%ymm3
vpsubq		%ymm12,%ymm4,%ymm4
vpsubq		%ymm13,%ymm5,%ymm5
vpsubq		%ymm14,%ymm6,%ymm6
vpsubq		%ymm15,%ymm7,%ymm7
vpsrlq		$32,%ymm2,%ymm2
vpsrlq		$32,%ymm4,%ymm4
vmovshdup	%ymm6,%ymm6

#store
vpblendd	$0xAA,%ymm3,%ymm2,%ymm2
vpblendd	$0xAA,%ymm5,%ymm4,%ymm4
vpblendd	$0xAA,%ymm7,%ymm6,%ymm6
vmovdqa		%ymm2,(%rdi)
vmovdqa		%ymm4,32(%rdi)
vmovdqa		%ymm6,64(%rdi)

add		$96,%rdi
add		$96,%rsi
add		$96,%rdx
add		$1,%eax
cmp		$10,%eax
jb 		_looptop1

vmovdqa		(%rsi),%ymm2
vmovdqa		32(%rsi),%ymm4
vmovdqa		(%rdx),%ymm10
vmovdqa		32(%rdx),%ymm12
vpsrlq		$32,%ymm2,%ymm3
vpsrlq		$32,%ymm4,%ymm5
vmovshdup	%ymm10,%ymm11
vmovshdup	%ymm12,%ymm13

#mul
vpmuldq		%ymm2,%ymm10,%ymm2
vpmuldq		%ymm3,%ymm11,%ymm3
vpmuldq		%ymm4,%ymm12,%ymm4
vpmuldq		%ymm5,%ymm13,%ymm5

#reduce
vpmuldq		%ymm0,%ymm2,%ymm10
vpmuldq		%ymm0,%ymm3,%ymm11
vpmuldq		%ymm0,%ymm4,%ymm12
vpmuldq		%ymm0,%ymm5,%ymm13
vpmuldq		%ymm1,%ymm10,%ymm10
vpmuldq		%ymm1,%ymm11,%ymm11
vpmuldq		%ymm1,%ymm12,%ymm12
vpmuldq		%ymm1,%ymm13,%ymm13
vpsubq		%ymm10,%ymm2,%ymm2
vpsubq		%ymm11,%ymm3,%ymm3
vpsubq		%ymm12,%ymm4,%ymm4
vpsubq		%ymm13,%ymm5,%ymm5
vpsrlq		$32,%ymm2,%ymm2
vmovshdup	%ymm4,%ymm4

#store
vpblendd	$0x55,%ymm2,%ymm3,%ymm2
vpblendd	$0x55,%ymm4,%ymm5,%ymm4
vmovdqa		%ymm2,(%rdi)
vmovdqa		%ymm4,32(%rdi)

ret

.macro pointwise off
#load
vmovdqa		\off(%rsi),%ymm6
vmovdqa		\off+32(%rsi),%ymm8
vmovdqa		\off(%rdx),%ymm10
vmovdqa		\off+32(%rdx),%ymm12
vpsrlq		$32,%ymm6,%ymm7
vpsrlq		$32,%ymm8,%ymm9
vmovshdup	%ymm10,%ymm11
vmovshdup	%ymm12,%ymm13

#mul
vpmuldq		%ymm6,%ymm10,%ymm6
vpmuldq		%ymm7,%ymm11,%ymm7
vpmuldq		%ymm8,%ymm12,%ymm8
vpmuldq		%ymm9,%ymm13,%ymm9
.endm

.macro acc
vpaddq		%ymm6,%ymm2,%ymm2
vpaddq		%ymm7,%ymm3,%ymm3
vpaddq		%ymm8,%ymm4,%ymm4
vpaddq		%ymm9,%ymm5,%ymm5
.endm

.global cdecl(pointwise_acc_avx)
cdecl(pointwise_acc_avx):
#consts
vmovdqa		_8XQINV*4(%rcx),%ymm0
vmovdqa		_8XQ*4(%rcx),%ymm1

xor		%eax,%eax
_looptop2:
pointwise	0

#mov
vmovdqa		%ymm6,%ymm2
vmovdqa		%ymm7,%ymm3
vmovdqa		%ymm8,%ymm4
vmovdqa		%ymm9,%ymm5

pointwise	1024
acc

#if L >= 3
pointwise	2048
acc
#endif

#if L >= 4
pointwise	3072
acc
#endif

#if L >= 5
pointwise	4096
acc
#endif

#if L >= 6
pointwise	5120
acc
#endif

#if L >= 7
pointwise	6144
acc
#endif

#reduce
vpmuldq		%ymm0,%ymm2,%ymm6
vpmuldq		%ymm0,%ymm3,%ymm7
vpmuldq		%ymm0,%ymm4,%ymm8
vpmuldq		%ymm0,%ymm5,%ymm9
vpmuldq		%ymm1,%ymm6,%ymm6
vpmuldq		%ymm1,%ymm7,%ymm7
vpmuldq		%ymm1,%ymm8,%ymm8
vpmuldq		%ymm1,%ymm9,%ymm9
vpsubq		%ymm6,%ymm2,%ymm2
vpsubq		%ymm7,%ymm3,%ymm3
vpsubq		%ymm8,%ymm4,%ymm4
vpsubq		%ymm9,%ymm5,%ymm5
vpsrlq		$32,%ymm2,%ymm2
vmovshdup	%ymm4,%ymm4

#store
vpblendd	$0xAA,%ymm3,%ymm2,%ymm2
vpblendd	$0xAA,%ymm5,%ymm4,%ymm4

vmovdqa		%ymm2,(%rdi)
vmovdqa		%ymm4,32(%rdi)

add		$64,%rsi
add		$64,%rdx
add		$64,%rdi
add		$1,%eax
cmp		$16,%eax
jb _looptop2

ret

.section .note.GNU-stack,"",@progbits
