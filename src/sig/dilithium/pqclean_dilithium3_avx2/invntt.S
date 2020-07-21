#include "consts.h"
.include "shuffle.inc"

.macro butterfly l0,l1,l2,l3,h0,h1,h2,h3,z0=15,z1=3
vpaddd		%ymm2,%ymm\l0,%ymm12
vpaddd		%ymm2,%ymm\l1,%ymm13
vpaddd		%ymm2,%ymm\l2,%ymm14

vpsubd		%ymm\h0,%ymm12,%ymm12
vpsubd		%ymm\h1,%ymm13,%ymm13
vpsubd		%ymm\h2,%ymm14,%ymm14

vpmuludq	%ymm\z0,%ymm12,%ymm12
vpmuludq	%ymm\z0,%ymm13,%ymm13
vpaddd		%ymm2,%ymm\l3,%ymm15

vpmuludq	%ymm\z1,%ymm14,%ymm14
vpsubd		%ymm\h3,%ymm15,%ymm15
vpaddd		%ymm\l0,%ymm\h0,%ymm\l0

vpmuludq	%ymm\z1,%ymm15,%ymm15
vpaddd		%ymm\l1,%ymm\h1,%ymm\l1
vpaddd		%ymm\l2,%ymm\h2,%ymm\l2

vpaddd		%ymm\l3,%ymm\h3,%ymm\l3

vpmuludq	%ymm0,%ymm12,%ymm\h0
vpmuludq	%ymm0,%ymm13,%ymm\h1
vpmuludq	%ymm0,%ymm14,%ymm\h2
vpmuludq	%ymm0,%ymm15,%ymm\h3
vpmuludq	%ymm1,%ymm\h0,%ymm\h0
vpmuludq	%ymm1,%ymm\h1,%ymm\h1
vpmuludq	%ymm1,%ymm\h2,%ymm\h2
vpmuludq	%ymm1,%ymm\h3,%ymm\h3
vpaddq		%ymm12,%ymm\h0,%ymm\h0
vpaddq		%ymm13,%ymm\h1,%ymm\h1
vpaddq		%ymm14,%ymm\h2,%ymm\h2
vpaddq		%ymm15,%ymm\h3,%ymm\h3
vpsrlq		$32,%ymm\h0,%ymm\h0
vpsrlq		$32,%ymm\h1,%ymm\h1
vpsrlq		$32,%ymm\h2,%ymm\h2
vpsrlq		$32,%ymm\h3,%ymm\h3
.endm

.text
invntt_levels0t4_avx:
#load
vmovdqa		(%rsi),%ymm6
vmovdqa		32(%rsi),%ymm7
vmovdqa		64(%rsi),%ymm5
vmovdqa		96(%rsi),%ymm10

#reorder
shuffle8	6,5,8,5
shuffle8	7,10,6,10

shuffle4	8,6,4,6
shuffle4	5,10,8,10

vpsrlq		$32,%ymm4,%ymm5
vpsrlq		$32,%ymm6,%ymm7
vpsrlq		$32,%ymm8,%ymm9
vpsrlq		$32,%ymm10,%ymm11

level0:
vpmovzxdq	(%rdx),%ymm3
vpmovzxdq	16(%rdx),%ymm15
vpaddd		%ymm2,%ymm4,%ymm12
vpaddd		%ymm2,%ymm6,%ymm13
vpaddd		%ymm2,%ymm8,%ymm14

vpsubd		%ymm5,%ymm12,%ymm12
vpsubd		%ymm7,%ymm13,%ymm13
vpsubd		%ymm9,%ymm14,%ymm14

vpmuludq	%ymm3,%ymm12,%ymm12
vpmuludq	%ymm15,%ymm13,%ymm13
vpaddd		%ymm2,%ymm10,%ymm15

vpsubd		%ymm11,%ymm15,%ymm15
vpaddd		%ymm4,%ymm5,%ymm4
vpaddd		%ymm6,%ymm7,%ymm6
vpmovzxdq	32(%rdx),%ymm5
vpmovzxdq	48(%rdx),%ymm7

vpmuludq	%ymm5,%ymm14,%ymm14
vpmuludq	%ymm7,%ymm15,%ymm15
vpaddd		%ymm8,%ymm9,%ymm8

vpaddd		%ymm10,%ymm11,%ymm10

vpmuludq	%ymm0,%ymm12,%ymm5
vpmuludq	%ymm0,%ymm13,%ymm7
vpmuludq	%ymm0,%ymm14,%ymm9
vpmuludq	%ymm0,%ymm15,%ymm11
vpmuludq	%ymm1,%ymm5,%ymm5
vpmuludq	%ymm1,%ymm7,%ymm7
vpmuludq	%ymm1,%ymm9,%ymm9
vpmuludq	%ymm1,%ymm11,%ymm11
vpaddq		%ymm12,%ymm5,%ymm5
vpaddq		%ymm13,%ymm7,%ymm7
vpaddq		%ymm14,%ymm9,%ymm9
vpaddq		%ymm15,%ymm11,%ymm11
vpsrlq		$32,%ymm5,%ymm5
vpsrlq		$32,%ymm7,%ymm7
vpsrlq		$32,%ymm9,%ymm9
vpsrlq		$32,%ymm11,%ymm11

level1:
#zetas
vpmovzxdq	64(%rdx),%ymm15
vpmovzxdq	80(%rdx),%ymm3

butterfly	4,5,8,9,6,7,10,11

level2:
#zetas
vpmovzxdq	96(%rdx),%ymm3

butterfly	4,5,6,7,8,9,10,11,3,3

#shuffle
shuffle4	4,5,3,5
shuffle4	6,7,4,7
shuffle4	8,9,6,9
shuffle4	10,11,8,11

level3:
#zetas
vpbroadcastd	112(%rdx),%ymm14
vpbroadcastd	116(%rdx),%ymm15
vpblendd	$0xF0,%ymm15,%ymm14,%ymm10

butterfly	3,4,6,8,5,7,9,11,10,10

#shuffle
shuffle8	3,4,10,4
shuffle8	6,8,3,8
shuffle8	5,7,6,7
shuffle8	9,11,5,11

level4:
#zetas
vpbroadcastd	120(%rdx),%ymm9

butterfly	10,3,6,5,4,8,7,11,9,9

#store
vmovdqa		%ymm10,(%rdi)
vmovdqa		%ymm3,32(%rdi)
vmovdqa		%ymm6,64(%rdi)
vmovdqa		%ymm5,96(%rdi)
vmovdqa		%ymm4,128(%rdi)
vmovdqa		%ymm8,160(%rdi)
vmovdqa		%ymm7,192(%rdi)
vmovdqa		%ymm11,224(%rdi)

ret

invntt_levels5t7_avx:
#load
vmovdqa		(%rsi),%ymm4
vmovdqa		256(%rsi),%ymm5
vmovdqa		512(%rsi),%ymm6
vmovdqa		768(%rsi),%ymm7
vmovdqa		1024(%rsi),%ymm8
vmovdqa		1280(%rsi),%ymm9
vmovdqa		1536(%rsi),%ymm10
vmovdqa		1792(%rsi),%ymm11

level5:
vpbroadcastd	(%rdx),%ymm3
vpbroadcastd	4(%rdx),%ymm15
vpaddd		%ymm2,%ymm4,%ymm12
vpaddd		%ymm2,%ymm6,%ymm13
vpaddd		%ymm2,%ymm8,%ymm14

vpsubd		%ymm5,%ymm12,%ymm12
vpsubd		%ymm7,%ymm13,%ymm13
vpsubd		%ymm9,%ymm14,%ymm14

vpmuludq	%ymm3,%ymm12,%ymm12
vpmuludq	%ymm15,%ymm13,%ymm13
vpaddd		%ymm2,%ymm10,%ymm15

vpsubd		%ymm11,%ymm15,%ymm15
vpaddd		%ymm4,%ymm5,%ymm4
vpaddd		%ymm6,%ymm7,%ymm6
vpbroadcastd	8(%rdx),%ymm5
vpbroadcastd	12(%rdx),%ymm7

vpmuludq	%ymm5,%ymm14,%ymm14
vpmuludq	%ymm7,%ymm15,%ymm15
vpaddd		%ymm8,%ymm9,%ymm8

vpaddd		%ymm10,%ymm11,%ymm10

vpmuludq	%ymm0,%ymm12,%ymm5
vpmuludq	%ymm0,%ymm13,%ymm7
vpmuludq	%ymm0,%ymm14,%ymm9
vpmuludq	%ymm0,%ymm15,%ymm11
vpmuludq	%ymm1,%ymm5,%ymm5
vpmuludq	%ymm1,%ymm7,%ymm7
vpmuludq	%ymm1,%ymm9,%ymm9
vpmuludq	%ymm1,%ymm11,%ymm11
vpaddq		%ymm12,%ymm5,%ymm5
vpaddq		%ymm13,%ymm7,%ymm7
vpaddq		%ymm14,%ymm9,%ymm9
vpaddq		%ymm15,%ymm11,%ymm11
vpsrlq		$32,%ymm5,%ymm5
vpsrlq		$32,%ymm7,%ymm7
vpsrlq		$32,%ymm9,%ymm9
vpsrlq		$32,%ymm11,%ymm11

level6:
#zetas
vpbroadcastd	16(%rdx),%ymm15
vpbroadcastd	20(%rdx),%ymm3

butterfly	4,5,8,9,6,7,10,11

level7:
#zetas
vpbroadcastd	24(%rdx),%ymm3

butterfly	4,5,6,7,8,9,10,11,3,3

#consts
vmovdqa		_8XDIV*4(%rcx),%ymm3

vpmuludq        %ymm3,%ymm4,%ymm4
vpmuludq        %ymm3,%ymm5,%ymm5
vpmuludq        %ymm3,%ymm6,%ymm6
vpmuludq        %ymm3,%ymm7,%ymm7
vpmuludq        %ymm0,%ymm4,%ymm12
vpmuludq        %ymm0,%ymm5,%ymm13
vpmuludq        %ymm0,%ymm6,%ymm14
vpmuludq        %ymm0,%ymm7,%ymm15
vpmuludq        %ymm1,%ymm12,%ymm12
vpmuludq        %ymm1,%ymm13,%ymm13
vpmuludq        %ymm1,%ymm14,%ymm14
vpmuludq        %ymm1,%ymm15,%ymm15
vpaddq          %ymm12,%ymm4,%ymm4
vpaddq          %ymm13,%ymm5,%ymm5
vpaddq          %ymm14,%ymm6,%ymm6
vpaddq          %ymm15,%ymm7,%ymm7
vpsrlq          $32,%ymm4,%ymm4
vpsrlq          $32,%ymm5,%ymm5
vpsrlq          $32,%ymm6,%ymm6
vpsrlq          $32,%ymm7,%ymm7

#store
vmovdqa         _MASK*4(%rcx),%ymm3
vpermd          %ymm4,%ymm3,%ymm4
vpermd          %ymm5,%ymm3,%ymm5
vpermd          %ymm6,%ymm3,%ymm6
vpermd          %ymm7,%ymm3,%ymm7
vpermd          %ymm8,%ymm3,%ymm8
vpermd          %ymm9,%ymm3,%ymm9
vpermd          %ymm10,%ymm3,%ymm10
vpermd          %ymm11,%ymm3,%ymm11
vmovdqa         %xmm4,(%rdi)
vmovdqa         %xmm5,128(%rdi)
vmovdqa         %xmm6,256(%rdi)
vmovdqa         %xmm7,384(%rdi)
vmovdqa         %xmm8,512(%rdi)
vmovdqa         %xmm9,640(%rdi)
vmovdqa         %xmm10,768(%rdi)
vmovdqa         %xmm11,896(%rdi)

ret

.global cdecl(invntt_avx)
cdecl(invntt_avx):
#entry
mov		%rsp,%r10
mov		%rsp,%rax
and		$31,%rax
sub		%rax,%rsp
sub		$2048,%rsp

#consts
vmovdqa		_8XQINV*4(%rsi),%ymm0
vmovdqa		_8XQ*4(%rsi),%ymm1
vmovdqa		_8X256Q*4(%rsi),%ymm2

#levels0t4
mov		%rsi,%rcx
lea		_ZETAS_INV*4(%rsi),%rdx
mov		%rdi,%rsi
mov		%rsp,%rdi
xor		%eax,%eax
_looptop1:
call		invntt_levels0t4_avx
add		$256,%rdi
add		$128,%rsi
add		$124,%rdx
add		$32,%eax
cmp		$256,%eax
jne		_looptop1

#levels5t7
sub		$1024,%rsi
mov		%rsi,%rdi
mov		%rsp,%rsi
lea		(_ZETAS_INV+248)*4(%rcx),%rdx
xor		%eax,%eax
_looptop2:
call		invntt_levels5t7_avx
add		$16,%rdi
add		$32,%rsi
add		$32,%eax
cmp		$256,%eax
jne		_looptop2

#return
mov		%r10,%rsp
ret
