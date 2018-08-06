.global ntt
ntt:
mov		%rsp,%r11
and		$31,%r11
add		$2048,%r11
sub		%r11,%rsp

mov		%rsp,%rcx
vmovdqa		_8xqinv(%rip),%ymm0
vmovdqa		_8xq(%rip),%ymm1
vmovdqa		_8x2q(%rip),%ymm2
vpbroadcastq	4(%rdx),%ymm3

xor		%eax,%eax

.p2align 5
_looptop:
#load
vpmovzxdq	(%rsi),%ymm4
vpmovzxdq	128(%rsi),%ymm5
vpmovzxdq	256(%rsi),%ymm6
vpmovzxdq	384(%rsi),%ymm7
vpmovzxdq	512(%rsi),%ymm8
vpmovzxdq	640(%rsi),%ymm9
vpmovzxdq	768(%rsi),%ymm10
vpmovzxdq	896(%rsi),%ymm11

#level 0
#mul
vpmuludq	%ymm3,%ymm8,%ymm8
vpmuludq	%ymm3,%ymm9,%ymm9
vpmuludq	%ymm3,%ymm10,%ymm10
vpmuludq	%ymm3,%ymm11,%ymm11

#reduce
vpmuludq	%ymm0,%ymm8,%ymm12
vpmuludq	%ymm0,%ymm9,%ymm13
vpmuludq	%ymm0,%ymm10,%ymm14
vpmuludq	%ymm0,%ymm11,%ymm15
vpmuludq	%ymm1,%ymm12,%ymm12
vpmuludq	%ymm1,%ymm13,%ymm13
vpmuludq	%ymm1,%ymm14,%ymm14
vpmuludq	%ymm1,%ymm15,%ymm15
vpaddq		%ymm8,%ymm12,%ymm12
vpaddq		%ymm9,%ymm13,%ymm13
vpaddq		%ymm10,%ymm14,%ymm14
vpaddq		%ymm11,%ymm15,%ymm15
vpsrlq		$32,%ymm12,%ymm12
vpsrlq		$32,%ymm13,%ymm13
vpsrlq		$32,%ymm14,%ymm14
vpsrlq		$32,%ymm15,%ymm15

#update
vpaddd		%ymm2,%ymm4,%ymm8
vpaddd		%ymm2,%ymm5,%ymm9
vpaddd		%ymm2,%ymm6,%ymm10
vpaddd		%ymm2,%ymm7,%ymm11
vpaddd		%ymm12,%ymm4,%ymm4
vpaddd		%ymm13,%ymm5,%ymm5
vpaddd		%ymm14,%ymm6,%ymm6
vpaddd		%ymm15,%ymm7,%ymm7
vpsubd		%ymm12,%ymm8,%ymm8
vpsubd		%ymm13,%ymm9,%ymm9
vpsubd		%ymm14,%ymm10,%ymm10
vpsubd		%ymm15,%ymm11,%ymm11

#level 1
#mul
vpbroadcastq	8(%rdx),%ymm12
vpbroadcastq	12(%rdx),%ymm13
vpmuludq	%ymm12,%ymm6,%ymm6
vpmuludq	%ymm12,%ymm7,%ymm7
vpmuludq	%ymm13,%ymm10,%ymm10
vpmuludq	%ymm13,%ymm11,%ymm11

#reduce
vpmuludq	%ymm0,%ymm6,%ymm12
vpmuludq	%ymm0,%ymm7,%ymm13
vpmuludq	%ymm0,%ymm10,%ymm14
vpmuludq	%ymm0,%ymm11,%ymm15
vpmuludq	%ymm1,%ymm12,%ymm12
vpmuludq	%ymm1,%ymm13,%ymm13
vpmuludq	%ymm1,%ymm14,%ymm14
vpmuludq	%ymm1,%ymm15,%ymm15
vpaddq		%ymm6,%ymm12,%ymm12
vpaddq		%ymm7,%ymm13,%ymm13
vpaddq		%ymm10,%ymm14,%ymm14
vpaddq		%ymm11,%ymm15,%ymm15
vpsrlq		$32,%ymm12,%ymm12
vpsrlq		$32,%ymm13,%ymm13
vpsrlq		$32,%ymm14,%ymm14
vpsrlq		$32,%ymm15,%ymm15

#update
vpaddd		%ymm2,%ymm4,%ymm6
vpaddd		%ymm2,%ymm5,%ymm7
vpaddd		%ymm2,%ymm8,%ymm10
vpaddd		%ymm2,%ymm9,%ymm11
vpaddd		%ymm12,%ymm4,%ymm4
vpaddd		%ymm13,%ymm5,%ymm5
vpaddd		%ymm14,%ymm8,%ymm8
vpaddd		%ymm15,%ymm9,%ymm9
vpsubd		%ymm12,%ymm6,%ymm6
vpsubd		%ymm13,%ymm7,%ymm7
vpsubd		%ymm14,%ymm10,%ymm10
vpsubd		%ymm15,%ymm11,%ymm11

#level 2
vpbroadcastq	16(%rdx),%ymm12
vpbroadcastq	20(%rdx),%ymm13
vpbroadcastq	24(%rdx),%ymm14
vpbroadcastq	28(%rdx),%ymm15
vpmuludq	%ymm12,%ymm5,%ymm5
vpmuludq	%ymm13,%ymm7,%ymm7
vpmuludq	%ymm14,%ymm9,%ymm9
vpmuludq	%ymm15,%ymm11,%ymm11

#reduce
vpmuludq	%ymm0,%ymm5,%ymm12
vpmuludq	%ymm0,%ymm7,%ymm13
vpmuludq	%ymm0,%ymm9,%ymm14
vpmuludq	%ymm0,%ymm11,%ymm15
vpmuludq	%ymm1,%ymm12,%ymm12
vpmuludq	%ymm1,%ymm13,%ymm13
vpmuludq	%ymm1,%ymm14,%ymm14
vpmuludq	%ymm1,%ymm15,%ymm15
vpaddq		%ymm5,%ymm12,%ymm12
vpaddq		%ymm7,%ymm13,%ymm13
vpaddq		%ymm9,%ymm14,%ymm14
vpaddq		%ymm11,%ymm15,%ymm15
vpsrlq		$32,%ymm12,%ymm12
vpsrlq		$32,%ymm13,%ymm13
vpsrlq		$32,%ymm14,%ymm14
vpsrlq		$32,%ymm15,%ymm15

#update
vpaddd		%ymm2,%ymm4,%ymm5
vpaddd		%ymm2,%ymm6,%ymm7
vpaddd		%ymm2,%ymm8,%ymm9
vpaddd		%ymm2,%ymm10,%ymm11
vpaddd		%ymm12,%ymm4,%ymm4
vpaddd		%ymm13,%ymm6,%ymm6
vpaddd		%ymm14,%ymm8,%ymm8
vpaddd		%ymm15,%ymm10,%ymm10
vpsubd		%ymm12,%ymm5,%ymm5
vpsubd		%ymm13,%ymm7,%ymm7
vpsubd		%ymm14,%ymm9,%ymm9
vpsubd		%ymm15,%ymm11,%ymm11

#store
vmovdqa		%ymm4,(%rcx)
vmovdqa		%ymm5,256(%rcx)
vmovdqa		%ymm6,512(%rcx)
vmovdqa		%ymm7,768(%rcx)
vmovdqa		%ymm8,1024(%rcx)
vmovdqa		%ymm9,1280(%rcx)
vmovdqa		%ymm10,1536(%rcx)
vmovdqa		%ymm11,1792(%rcx)

add		$1,%eax
add		$16,%rsi
add		$32,%rcx
cmp		$8,%eax
jb _looptop

mov		$32,%r8
sub		$256,%rcx
#sub		$128,%rsi
xor		%eax,%eax

.p2align 5
_looptop2:
vpbroadcastq	(%rdx,%r8),%ymm3

#load
vmovdqa		(%rcx),%ymm4
vmovdqa		32(%rcx),%ymm5
vmovdqa		64(%rcx),%ymm6
vmovdqa		96(%rcx),%ymm7
vmovdqa		128(%rcx),%ymm8
vmovdqa		160(%rcx),%ymm9
vmovdqa		192(%rcx),%ymm10
vmovdqa		224(%rcx),%ymm11

#level 3
#mul
vpmuludq	%ymm3,%ymm8,%ymm8
vpmuludq	%ymm3,%ymm9,%ymm9
vpmuludq	%ymm3,%ymm10,%ymm10
vpmuludq	%ymm3,%ymm11,%ymm11

#reduce
vpmuludq	%ymm0,%ymm8,%ymm12
vpmuludq	%ymm0,%ymm9,%ymm13
vpmuludq	%ymm0,%ymm10,%ymm14
vpmuludq	%ymm0,%ymm11,%ymm15
vpmuludq	%ymm1,%ymm12,%ymm12
vpmuludq	%ymm1,%ymm13,%ymm13
vpmuludq	%ymm1,%ymm14,%ymm14
vpmuludq	%ymm1,%ymm15,%ymm15
vpaddq		%ymm8,%ymm12,%ymm12
vpaddq		%ymm9,%ymm13,%ymm13
vpaddq		%ymm10,%ymm14,%ymm14
vpaddq		%ymm11,%ymm15,%ymm15
vpsrlq		$32,%ymm12,%ymm12
vpsrlq		$32,%ymm13,%ymm13
vpsrlq		$32,%ymm14,%ymm14
vpsrlq		$32,%ymm15,%ymm15

#update
vpaddd		%ymm2,%ymm4,%ymm8
vpaddd		%ymm2,%ymm5,%ymm9
vpaddd		%ymm2,%ymm6,%ymm10
vpaddd		%ymm2,%ymm7,%ymm11
vpaddd		%ymm12,%ymm4,%ymm4
vpaddd		%ymm13,%ymm5,%ymm5
vpaddd		%ymm14,%ymm6,%ymm6
vpaddd		%ymm15,%ymm7,%ymm7
vpsubd		%ymm12,%ymm8,%ymm8
vpsubd		%ymm13,%ymm9,%ymm9
vpsubd		%ymm14,%ymm10,%ymm10
vpsubd		%ymm15,%ymm11,%ymm11

#level 4
#mul
vpbroadcastq	(%rdx,%r8,2),%ymm12
vpbroadcastq	4(%rdx,%r8,2),%ymm13
vpmuludq	%ymm12,%ymm6,%ymm6
vpmuludq	%ymm12,%ymm7,%ymm7
vpmuludq	%ymm13,%ymm10,%ymm10
vpmuludq	%ymm13,%ymm11,%ymm11

#reduce
vpmuludq	%ymm0,%ymm6,%ymm12
vpmuludq	%ymm0,%ymm7,%ymm13
vpmuludq	%ymm0,%ymm10,%ymm14
vpmuludq	%ymm0,%ymm11,%ymm15
vpmuludq	%ymm1,%ymm12,%ymm12
vpmuludq	%ymm1,%ymm13,%ymm13
vpmuludq	%ymm1,%ymm14,%ymm14
vpmuludq	%ymm1,%ymm15,%ymm15
vpaddq		%ymm6,%ymm12,%ymm12
vpaddq		%ymm7,%ymm13,%ymm13
vpaddq		%ymm10,%ymm14,%ymm14
vpaddq		%ymm11,%ymm15,%ymm15
vpsrlq		$32,%ymm12,%ymm12
vpsrlq		$32,%ymm13,%ymm13
vpsrlq		$32,%ymm14,%ymm14
vpsrlq		$32,%ymm15,%ymm15

#update
vpaddd		%ymm2,%ymm4,%ymm6
vpaddd		%ymm2,%ymm5,%ymm7
vpaddd		%ymm2,%ymm8,%ymm10
vpaddd		%ymm2,%ymm9,%ymm11
vpaddd		%ymm12,%ymm4,%ymm4
vpaddd		%ymm13,%ymm5,%ymm5
vpaddd		%ymm14,%ymm8,%ymm8
vpaddd		%ymm15,%ymm9,%ymm9
vpsubd		%ymm12,%ymm6,%ymm6
vpsubd		%ymm13,%ymm7,%ymm7
vpsubd		%ymm14,%ymm10,%ymm10
vpsubd		%ymm15,%ymm11,%ymm11

#level 5
vpbroadcastq	(%rdx,%r8,4),%ymm12
vpbroadcastq	4(%rdx,%r8,4),%ymm13
vpbroadcastq	8(%rdx,%r8,4),%ymm14
vpbroadcastq	12(%rdx,%r8,4),%ymm15
vpmuludq	%ymm12,%ymm5,%ymm5
vpmuludq	%ymm13,%ymm7,%ymm7
vpmuludq	%ymm14,%ymm9,%ymm9
vpmuludq	%ymm15,%ymm11,%ymm11

#reduce
vpmuludq	%ymm0,%ymm5,%ymm12
vpmuludq	%ymm0,%ymm7,%ymm13
vpmuludq	%ymm0,%ymm9,%ymm14
vpmuludq	%ymm0,%ymm11,%ymm15
vpmuludq	%ymm1,%ymm12,%ymm12
vpmuludq	%ymm1,%ymm13,%ymm13
vpmuludq	%ymm1,%ymm14,%ymm14
vpmuludq	%ymm1,%ymm15,%ymm15
vpaddq		%ymm5,%ymm12,%ymm12
vpaddq		%ymm7,%ymm13,%ymm13
vpaddq		%ymm9,%ymm14,%ymm14
vpaddq		%ymm11,%ymm15,%ymm15
vpsrlq		$32,%ymm12,%ymm12
vpsrlq		$32,%ymm13,%ymm13
vpsrlq		$32,%ymm14,%ymm14
vpsrlq		$32,%ymm15,%ymm15

#update
vpaddd		%ymm2,%ymm4,%ymm5
vpaddd		%ymm2,%ymm6,%ymm7
vpaddd		%ymm2,%ymm8,%ymm9
vpaddd		%ymm2,%ymm10,%ymm11
vpaddd		%ymm12,%ymm4,%ymm4
vpaddd		%ymm13,%ymm6,%ymm6
vpaddd		%ymm14,%ymm8,%ymm8
vpaddd		%ymm15,%ymm10,%ymm10
vpsubd		%ymm12,%ymm5,%ymm5
vpsubd		%ymm13,%ymm7,%ymm7
vpsubd		%ymm14,%ymm9,%ymm9
vpsubd		%ymm15,%ymm11,%ymm11

#level 6
vpbroadcastq	(%rdx,%r8,8),%ymm12
vpbroadcastq	4(%rdx,%r8,8),%ymm13
vpbroadcastq	8(%rdx,%r8,8),%ymm14
vpbroadcastq	12(%rdx,%r8,8),%ymm15
vpblendd	$0x0F,%ymm12,%ymm13,%ymm12
vpblendd	$0x0F,%ymm14,%ymm15,%ymm13
vpbroadcastq	16(%rdx,%r8,8),%ymm14
vpbroadcastq	20(%rdx,%r8,8),%ymm15
vpblendd	$0x0F,%ymm14,%ymm15,%ymm14
vpbroadcastq	24(%rdx,%r8,8),%ymm15
vpbroadcastq	28(%rdx,%r8,8),%ymm3
vpblendd	$0x0F,%ymm15,%ymm3,%ymm15

#vpbroadcastq	(%rsi,%rcx,8),%xmm12
#vperm2i128	$0x04,%ymm12,%ymm12,%ymm12
#vpbroadcastq	8(%rsi,%rcx,8),%xmm13
#vperm2i128	$0x04,%ymm13,%ymm13,%ymm13
#vpbroadcastq	16(%rsi,%rcx,8),%xmm14
#vperm2i128	$0x04,%ymm14,%ymm14,%ymm14
#vpbroadcastq	24(%rsi,%rcx,8),%xmm15
#vperm2i128	$0x04,%ymm15,%ymm15,%ymm15
#vpbroadcastq	4(%rsi,%rcx,8),%xmm12
#vpbroadcastq	12(%rsi,%rcx,8),%xmm13
#vpbroadcastq	20(%rsi,%rcx,8),%xmm14
#vpbroadcastq	28(%rsi,%rcx,8),%xmm15

vperm2i128	$0x02,%ymm4,%ymm5,%ymm3
vperm2i128	$0x13,%ymm4,%ymm5,%ymm4
vperm2i128	$0x02,%ymm6,%ymm7,%ymm5
vperm2i128	$0x13,%ymm6,%ymm7,%ymm6
vperm2i128	$0x02,%ymm8,%ymm9,%ymm7
vperm2i128	$0x13,%ymm8,%ymm9,%ymm8
vperm2i128	$0x02,%ymm10,%ymm11,%ymm9
vperm2i128	$0x13,%ymm10,%ymm11,%ymm10

#mul
vpmuludq	%ymm12,%ymm4,%ymm4
vpmuludq	%ymm13,%ymm6,%ymm6
vpmuludq	%ymm14,%ymm8,%ymm8
vpmuludq	%ymm15,%ymm10,%ymm10

#reduce
vpmuludq	%ymm0,%ymm4,%ymm12
vpmuludq	%ymm0,%ymm6,%ymm13
vpmuludq	%ymm0,%ymm8,%ymm14
vpmuludq	%ymm0,%ymm10,%ymm15
vpmuludq	%ymm1,%ymm12,%ymm12
vpmuludq	%ymm1,%ymm13,%ymm13
vpmuludq	%ymm1,%ymm14,%ymm14
vpmuludq	%ymm1,%ymm15,%ymm15
vpaddq		%ymm4,%ymm12,%ymm12
vpaddq		%ymm6,%ymm13,%ymm13
vpaddq		%ymm8,%ymm14,%ymm14
vpaddq		%ymm10,%ymm15,%ymm15
vpsrlq		$32,%ymm12,%ymm12
vpsrlq		$32,%ymm13,%ymm13
vpsrlq		$32,%ymm14,%ymm14
vpsrlq		$32,%ymm15,%ymm15

#update
vpaddd		%ymm2,%ymm3,%ymm4
vpaddd		%ymm2,%ymm5,%ymm6
vpaddd		%ymm2,%ymm7,%ymm8
vpaddd		%ymm2,%ymm9,%ymm10
vpaddd		%ymm12,%ymm3,%ymm3
vpaddd		%ymm13,%ymm5,%ymm5
vpaddd		%ymm14,%ymm7,%ymm7
vpaddd		%ymm15,%ymm9,%ymm9
vpsubd		%ymm12,%ymm4,%ymm4
vpsubd		%ymm13,%ymm6,%ymm6
vpsubd		%ymm14,%ymm8,%ymm8
vpsubd		%ymm15,%ymm10,%ymm10

#level 7
lea		(,%r8,8),%r9

vpshufd		$0xD2,%ymm3,%ymm12
vpshufd		$0x2D,%ymm4,%ymm13
vpshufd		$0xD2,%ymm5,%ymm14
vpshufd		$0x2D,%ymm6,%ymm15
vpblendd	$0x66,%ymm12,%ymm13,%ymm3
vpblendd	$0x66,%ymm13,%ymm12,%ymm4
vpblendd	$0x66,%ymm14,%ymm15,%ymm5
vpblendd	$0x66,%ymm15,%ymm14,%ymm6
vpshufd		$0xD2,%ymm7,%ymm12
vpshufd		$0x2D,%ymm8,%ymm13
vpshufd		$0xD2,%ymm9,%ymm14
vpshufd		$0x2D,%ymm10,%ymm15
vpblendd	$0x66,%ymm12,%ymm13,%ymm7
vpblendd	$0x66,%ymm13,%ymm12,%ymm8
vpblendd	$0x66,%ymm14,%ymm15,%ymm9
vpblendd	$0x66,%ymm15,%ymm14,%ymm10

vpmovzxdq	(%rdx,%r9,2),%ymm12
vpmovzxdq	16(%rdx,%r9,2),%ymm13
vpmovzxdq	32(%rdx,%r9,2),%ymm14
vpmovzxdq	48(%rdx,%r9,2),%ymm15

#mul
vpmuludq	%ymm12,%ymm4,%ymm4
vpmuludq	%ymm13,%ymm6,%ymm6
vpmuludq	%ymm14,%ymm8,%ymm8
vpmuludq	%ymm15,%ymm10,%ymm10

#reduce
vpmuludq	%ymm0,%ymm4,%ymm12
vpmuludq	%ymm0,%ymm6,%ymm13
vpmuludq	%ymm0,%ymm8,%ymm14
vpmuludq	%ymm0,%ymm10,%ymm15
vpmuludq	%ymm1,%ymm12,%ymm12
vpmuludq	%ymm1,%ymm13,%ymm13
vpmuludq	%ymm1,%ymm14,%ymm14
vpmuludq	%ymm1,%ymm15,%ymm15
vpaddq		%ymm4,%ymm12,%ymm12
vpaddq		%ymm6,%ymm13,%ymm13
vpaddq		%ymm8,%ymm14,%ymm14
vpaddq		%ymm10,%ymm15,%ymm15

#update
vpaddd		%ymm2,%ymm3,%ymm4
vpaddd		%ymm2,%ymm5,%ymm6
vpaddd		%ymm2,%ymm7,%ymm8
vpaddd		%ymm2,%ymm9,%ymm10
vpaddd		%ymm12,%ymm3,%ymm3
vpaddd		%ymm13,%ymm5,%ymm5
vpaddd		%ymm14,%ymm7,%ymm7
vpaddd		%ymm15,%ymm9,%ymm9
vpsubd		%ymm12,%ymm4,%ymm4
vpsubd		%ymm13,%ymm6,%ymm6
vpsubd		%ymm14,%ymm8,%ymm8
vpsubd		%ymm15,%ymm10,%ymm10

#store
vpsrlq		$32,%ymm3,%ymm3
vpsrlq		$32,%ymm5,%ymm5
vpsrlq		$32,%ymm7,%ymm7
vpsrlq		$32,%ymm9,%ymm9
vpblendd	$0x55,%ymm3,%ymm4,%ymm3
vpblendd	$0x55,%ymm5,%ymm6,%ymm5
vpblendd	$0x55,%ymm7,%ymm8,%ymm7
vpblendd	$0x55,%ymm9,%ymm10,%ymm9
vmovdqa		%ymm3,(%rdi)
vmovdqa		%ymm5,32(%rdi)
vmovdqa		%ymm7,64(%rdi)
vmovdqa		%ymm9,96(%rdi)

add		$1,%eax
add		$4,%r8
add		$256,%rcx
add		$128,%rdi
cmp		$8,%eax
jb _looptop2

add 		%r11,%rsp
ret
