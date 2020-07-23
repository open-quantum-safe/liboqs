#include "consts.h"
.include "shuffle.inc"

.macro butterfly rl0,rl1,rl2,rl3,rh0,rh1,rh2,rh3,z0=3,z1=3,z2=3,z3=3
#mul
vpmuludq	%ymm\z0,%ymm\rh0,%ymm\rh0
vpmuludq	%ymm\z1,%ymm\rh1,%ymm\rh1
vpmuludq	%ymm\z2,%ymm\rh2,%ymm\rh2
vpmuludq	%ymm\z3,%ymm\rh3,%ymm\rh3

#reduce
vpmuludq	%ymm0,%ymm\rh0,%ymm12
vpmuludq	%ymm0,%ymm\rh1,%ymm13
vpmuludq	%ymm0,%ymm\rh2,%ymm14
vpmuludq	%ymm0,%ymm\rh3,%ymm15
vpmuludq	%ymm1,%ymm12,%ymm12
vpmuludq	%ymm1,%ymm13,%ymm13
vpmuludq	%ymm1,%ymm14,%ymm14
vpmuludq	%ymm1,%ymm15,%ymm15
vpaddq		%ymm\rh0,%ymm12,%ymm12
vpaddq		%ymm\rh1,%ymm13,%ymm13
vpaddq		%ymm\rh2,%ymm14,%ymm14
vpaddq		%ymm\rh3,%ymm15,%ymm15
vpsrlq		$32,%ymm12,%ymm12
vpsrlq		$32,%ymm13,%ymm13
vpsrlq		$32,%ymm14,%ymm14
vpsrlq		$32,%ymm15,%ymm15

#update
vpaddd		%ymm2,%ymm\rl0,%ymm\rh0
vpaddd		%ymm2,%ymm\rl1,%ymm\rh1
vpaddd		%ymm2,%ymm\rl2,%ymm\rh2
vpaddd		%ymm2,%ymm\rl3,%ymm\rh3
vpaddd		%ymm12,%ymm\rl0,%ymm\rl0
vpaddd		%ymm13,%ymm\rl1,%ymm\rl1
vpaddd		%ymm14,%ymm\rl2,%ymm\rl2
vpaddd		%ymm15,%ymm\rl3,%ymm\rl3
vpsubd		%ymm12,%ymm\rh0,%ymm\rh0
vpsubd		%ymm13,%ymm\rh1,%ymm\rh1
vpsubd		%ymm14,%ymm\rh2,%ymm\rh2
vpsubd		%ymm15,%ymm\rh3,%ymm\rh3
.endm

.text
ntt_levels0t2_avx:
level0:
#zetas
vpbroadcastd	(%rdx),%ymm3

#load
vpmovzxdq	(%rsi),%ymm4
vpmovzxdq	128(%rsi),%ymm5
vpmovzxdq	256(%rsi),%ymm6
vpmovzxdq	384(%rsi),%ymm7
vpmovzxdq	512(%rsi),%ymm8
vpmovzxdq	640(%rsi),%ymm9
vpmovzxdq	768(%rsi),%ymm10
vpmovzxdq	896(%rsi),%ymm11

butterfly	4,5,6,7,8,9,10,11

level1:
#zetas
vpbroadcastd	4(%rdx),%ymm12
vpbroadcastd	8(%rdx),%ymm13

butterfly	4,5,8,9,6,7,10,11,12,12,13,13

level2:
#zetas
vpbroadcastd	12(%rdx),%ymm12
vpbroadcastd	16(%rdx),%ymm13
vpbroadcastd	20(%rdx),%ymm14
vpbroadcastd	24(%rdx),%ymm15

butterfly	4,6,8,10,5,7,9,11,12,13,14,15

#store
vmovdqa		%ymm4,(%rdi)
vmovdqa		%ymm5,256(%rdi)
vmovdqa		%ymm6,512(%rdi)
vmovdqa		%ymm7,768(%rdi)
vmovdqa		%ymm8,1024(%rdi)
vmovdqa		%ymm9,1280(%rdi)
vmovdqa		%ymm10,1536(%rdi)
vmovdqa		%ymm11,1792(%rdi)

ret

ntt_levels3t8_avx:
#load
vmovdqa		(%rsi),%ymm4
vmovdqa		32(%rsi),%ymm5
vmovdqa		64(%rsi),%ymm6
vmovdqa		96(%rsi),%ymm7
vmovdqa		128(%rsi),%ymm8
vmovdqa		160(%rsi),%ymm9
vmovdqa		192(%rsi),%ymm10
vmovdqa		224(%rsi),%ymm11

level3:
#zetas
vpbroadcastd	(%rdx),%ymm3

butterfly	4,5,6,7,8,9,10,11

level4:
#zetas
vpbroadcastd	4(%rdx),%ymm12
vpbroadcastd	8(%rdx),%ymm13
vpblendd	$0xF0,%ymm13,%ymm12,%ymm12

shuffle8	4,8,3,8
shuffle8	5,9,4,9
shuffle8	6,10,5,10
shuffle8	7,11,6,11

butterfly	3,8,4,9,5,10,6,11,12,12,12,12

level5:
#zetas
vpmovzxdq	12(%rdx),%ymm12

shuffle4	3,5,7,5
shuffle4	8,10,3,10
shuffle4	4,6,8,6
shuffle4	9,11,4,11

butterfly	7,5,3,10,8,6,4,11,12,12,12,12

level6:
#zetas
vpmovzxdq	28(%rdx),%ymm12
vpmovzxdq	44(%rdx),%ymm13

butterfly	7,5,8,6,3,10,4,11,12,12,13,13

level7:
#zetas
vpmovzxdq	60(%rdx),%ymm12
vpmovzxdq	76(%rdx),%ymm13
vpmovzxdq	92(%rdx),%ymm14
vpmovzxdq	108(%rdx),%ymm15

butterfly	7,3,8,4,5,10,6,11,12,13,14,15

#store
vpsllq		$32,%ymm5,%ymm5
vpsllq		$32,%ymm10,%ymm10
vpsllq		$32,%ymm6,%ymm6
vpsllq		$32,%ymm11,%ymm11
vpblendd	$0xAA,%ymm5,%ymm7,%ymm7
vpblendd	$0xAA,%ymm10,%ymm3,%ymm3
vpblendd	$0xAA,%ymm6,%ymm8,%ymm8
vpblendd	$0xAA,%ymm11,%ymm4,%ymm4

shuffle4	7,3,5,3
shuffle4	8,4,7,4

shuffle8	5,7,6,7
shuffle8	3,4,5,4

vmovdqa		%ymm6,(%rdi)
vmovdqa		%ymm5,32(%rdi)
vmovdqa		%ymm7,64(%rdi)
vmovdqa		%ymm4,96(%rdi)

ret

.global cdecl(ntt_avx)
cdecl(ntt_avx):
#entry
mov		%rsp,%r10
mov		%rsp,%rax
and		$31,%rax
sub		%rax,%rsp
sub		$2048,%rsp

#consts
vmovdqa		_8XQINV*4(%rsi),%ymm0
vmovdqa		_8XQ*4(%rsi),%ymm1
vmovdqa		_8X2Q*4(%rsi),%ymm2

#levels0t2
mov		%rsi,%rcx
lea		(_ZETAS+1)*4(%rsi),%rdx
mov		%rdi,%rsi
mov		%rsp,%rdi
xor		%eax,%eax
_looptop1:
call		ntt_levels0t2_avx
add		$32,%rdi
add		$16,%rsi
add		$32,%eax
cmp		$256,%eax
jne		_looptop1

#levels3t8
sub		$128,%rsi
mov		%rsi,%rdi
mov		%rsp,%rsi
add		$28,%rdx
xor		%eax,%eax
_looptop2:
call		ntt_levels3t8_avx
add		$128,%rdi
add		$256,%rsi
add		$124,%rdx
add		$32,%eax
cmp		$256,%eax
jne		_looptop2

#return
mov		%r10,%rsp
ret

