.global invntt
invntt:
mov		%rsp,%r11
and		$31,%r11
add		$2048,%r11
sub		%r11,%rsp

mov		%rsp,%rcx
vmovdqa		_8xqinv(%rip),%ymm0
vmovdqa		_8xq(%rip),%ymm1
vmovdqa		_8x256q(%rip),%ymm2

xor		%eax,%eax

.p2align 5
_looptop:
#load
vmovdqa		(%rsi),%ymm4
vmovdqa		32(%rsi),%ymm6
vmovdqa		64(%rsi),%ymm8
vmovdqa		96(%rsi),%ymm10
vpsrlq		$32,%ymm4,%ymm5
vpsrlq		$32,%ymm6,%ymm7
vpsrlq		$32,%ymm8,%ymm9
vpsrlq		$32,%ymm10,%ymm11

#level 0
#update
vpaddd		%ymm2,%ymm4,%ymm12
vpaddd		%ymm2,%ymm6,%ymm13
vpaddd		%ymm2,%ymm8,%ymm14
vpaddd		%ymm2,%ymm10,%ymm15
vpaddd		%ymm5,%ymm4,%ymm4
vpaddd		%ymm7,%ymm6,%ymm6
vpaddd		%ymm9,%ymm8,%ymm8
vpaddd		%ymm11,%ymm10,%ymm10
vpsubd		%ymm5,%ymm12,%ymm5
vpsubd		%ymm7,%ymm13,%ymm7
vpsubd		%ymm9,%ymm14,%ymm9
vpsubd		%ymm11,%ymm15,%ymm11

lea		(,%rax,8),%r8
vpmovzxdq	(%rdx,%r8,8),%ymm12
vpmovzxdq	16(%rdx,%r8,8),%ymm13
vpmovzxdq	32(%rdx,%r8,8),%ymm14
vpmovzxdq	48(%rdx,%r8,8),%ymm15

#mul
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
vpsrlq		$32,%ymm12,%ymm5
vpsrlq		$32,%ymm13,%ymm7
vpsrlq		$32,%ymm14,%ymm9
vpsrlq		$32,%ymm15,%ymm11

#level 1
vpshufd		$0xC6,%ymm5,%ymm13
vpshufd		$0xC6,%ymm4,%ymm12
vpshufd		$0xC6,%ymm7,%ymm15
vpshufd		$0xC6,%ymm6,%ymm14
vpblendd	$0x33,%ymm4,%ymm13,%ymm4
vpblendd	$0x33,%ymm12,%ymm5,%ymm5
vpblendd	$0x33,%ymm6,%ymm15,%ymm6
vpblendd	$0x33,%ymm14,%ymm7,%ymm7
vpshufd		$0xC6,%ymm9,%ymm13
vpshufd		$0xC6,%ymm8,%ymm12
vpshufd		$0xC6,%ymm11,%ymm15
vpshufd		$0xC6,%ymm10,%ymm14
vpblendd	$0x33,%ymm8,%ymm13,%ymm8
vpblendd	$0x33,%ymm12,%ymm9,%ymm9
vpblendd	$0x33,%ymm10,%ymm15,%ymm10
vpblendd	$0x33,%ymm14,%ymm11,%ymm11

#update
vpaddd		%ymm2,%ymm4,%ymm12
vpaddd		%ymm2,%ymm6,%ymm13
vpaddd		%ymm2,%ymm8,%ymm14
vpaddd		%ymm2,%ymm10,%ymm15
vpaddd		%ymm5,%ymm4,%ymm4
vpaddd		%ymm7,%ymm6,%ymm6
vpaddd		%ymm9,%ymm8,%ymm8
vpaddd		%ymm11,%ymm10,%ymm10
vpsubd		%ymm5,%ymm12,%ymm5
vpsubd		%ymm7,%ymm13,%ymm7
vpsubd		%ymm9,%ymm14,%ymm9
vpsubd		%ymm11,%ymm15,%ymm11

vpbroadcastq	512(%rdx,%r8,4),%ymm12
vpbroadcastq	516(%rdx,%r8,4),%ymm13
vpbroadcastq	520(%rdx,%r8,4),%ymm14
vpbroadcastq	524(%rdx,%r8,4),%ymm15
vpblendd	$0x0F,%ymm12,%ymm13,%ymm12
vpblendd	$0x0F,%ymm14,%ymm15,%ymm13
vpbroadcastq	528(%rdx,%r8,4),%ymm14
vpbroadcastq	532(%rdx,%r8,4),%ymm15
vpblendd	$0x0F,%ymm14,%ymm15,%ymm14
vpbroadcastq	536(%rdx,%r8,4),%ymm15
vpbroadcastq	540(%rdx,%r8,4),%ymm3
vpblendd	$0x0F,%ymm15,%ymm3,%ymm15

#mul
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
vpsrlq		$32,%ymm12,%ymm5
vpsrlq		$32,%ymm13,%ymm7
vpsrlq		$32,%ymm14,%ymm9
vpsrlq		$32,%ymm15,%ymm11

#level 2
vperm2i128	$0x02,%ymm4,%ymm5,%ymm3
vperm2i128	$0x13,%ymm4,%ymm5,%ymm4
vperm2i128	$0x02,%ymm6,%ymm7,%ymm5
vperm2i128	$0x13,%ymm6,%ymm7,%ymm6
vperm2i128	$0x02,%ymm8,%ymm9,%ymm7
vperm2i128	$0x13,%ymm8,%ymm9,%ymm8
vperm2i128	$0x02,%ymm10,%ymm11,%ymm9
vperm2i128	$0x13,%ymm10,%ymm11,%ymm10

#update
vpaddd		%ymm2,%ymm3,%ymm12
vpaddd		%ymm2,%ymm5,%ymm13
vpaddd		%ymm2,%ymm7,%ymm14
vpaddd		%ymm2,%ymm9,%ymm15
vpaddd		%ymm4,%ymm3,%ymm3
vpaddd		%ymm6,%ymm5,%ymm5
vpaddd		%ymm8,%ymm7,%ymm7
vpaddd		%ymm10,%ymm9,%ymm9
vpsubd		%ymm4,%ymm12,%ymm4
vpsubd		%ymm6,%ymm13,%ymm6
vpsubd		%ymm8,%ymm14,%ymm8
vpsubd		%ymm10,%ymm15,%ymm10

vpbroadcastq	768(%rdx,%r8,2),%ymm12
vpbroadcastq	772(%rdx,%r8,2),%ymm13
vpbroadcastq	776(%rdx,%r8,2),%ymm14
vpbroadcastq	780(%rdx,%r8,2),%ymm15

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
vpsrlq		$32,%ymm12,%ymm4
vpsrlq		$32,%ymm13,%ymm6
vpsrlq		$32,%ymm14,%ymm8
vpsrlq		$32,%ymm15,%ymm10

#level 3
#update
vpaddd		%ymm2,%ymm3,%ymm12
vpaddd		%ymm2,%ymm4,%ymm13
vpaddd		%ymm2,%ymm7,%ymm14
vpaddd		%ymm2,%ymm8,%ymm15
vpaddd		%ymm5,%ymm3,%ymm3
vpaddd		%ymm6,%ymm4,%ymm4
vpaddd		%ymm9,%ymm7,%ymm7
vpaddd		%ymm10,%ymm8,%ymm8
vpsubd		%ymm5,%ymm12,%ymm5
vpsubd		%ymm6,%ymm13,%ymm6
vpsubd		%ymm9,%ymm14,%ymm9
vpsubd		%ymm10,%ymm15,%ymm10

vpbroadcastq	896(%rdx,%r8),%ymm12
vpbroadcastq	900(%rdx,%r8),%ymm13

#mul
vpmuludq	%ymm12,%ymm5,%ymm5
vpmuludq	%ymm12,%ymm6,%ymm6
vpmuludq	%ymm13,%ymm9,%ymm9
vpmuludq	%ymm13,%ymm10,%ymm10

#reduce
vpmuludq	%ymm0,%ymm5,%ymm12
vpmuludq	%ymm0,%ymm6,%ymm13
vpmuludq	%ymm0,%ymm9,%ymm14
vpmuludq	%ymm0,%ymm10,%ymm15
vpmuludq	%ymm1,%ymm12,%ymm12
vpmuludq	%ymm1,%ymm13,%ymm13
vpmuludq	%ymm1,%ymm14,%ymm14
vpmuludq	%ymm1,%ymm15,%ymm15
vpaddq		%ymm5,%ymm12,%ymm12
vpaddq		%ymm6,%ymm13,%ymm13
vpaddq		%ymm9,%ymm14,%ymm14
vpaddq		%ymm10,%ymm15,%ymm15
vpsrlq		$32,%ymm12,%ymm5
vpsrlq		$32,%ymm13,%ymm6
vpsrlq		$32,%ymm14,%ymm9
vpsrlq		$32,%ymm15,%ymm10

#level 4
#update
vpaddd		%ymm2,%ymm3,%ymm12
vpaddd		%ymm2,%ymm4,%ymm13
vpaddd		%ymm2,%ymm5,%ymm14
vpaddd		%ymm2,%ymm6,%ymm15
vpaddd		%ymm7,%ymm3,%ymm3
vpaddd		%ymm8,%ymm4,%ymm4
vpaddd		%ymm9,%ymm5,%ymm5
vpaddd		%ymm10,%ymm6,%ymm6
vpsubd		%ymm7,%ymm12,%ymm7
vpsubd		%ymm8,%ymm13,%ymm8
vpsubd		%ymm9,%ymm14,%ymm9
vpsubd		%ymm10,%ymm15,%ymm10

vpbroadcastq	960(%rdx,%rax,4),%ymm12

#mul
vpmuludq	%ymm12,%ymm7,%ymm7
vpmuludq	%ymm12,%ymm8,%ymm8
vpmuludq	%ymm12,%ymm9,%ymm9
vpmuludq	%ymm12,%ymm10,%ymm10

#reduce
vpmuludq	%ymm0,%ymm7,%ymm12
vpmuludq	%ymm0,%ymm8,%ymm13
vpmuludq	%ymm0,%ymm9,%ymm14
vpmuludq	%ymm0,%ymm10,%ymm15
vpmuludq	%ymm1,%ymm12,%ymm12
vpmuludq	%ymm1,%ymm13,%ymm13
vpmuludq	%ymm1,%ymm14,%ymm14
vpmuludq	%ymm1,%ymm15,%ymm15
vpaddq		%ymm7,%ymm12,%ymm12
vpaddq		%ymm8,%ymm13,%ymm13
vpaddq		%ymm9,%ymm14,%ymm14
vpaddq		%ymm10,%ymm15,%ymm15
vpsrlq		$32,%ymm12,%ymm7
vpsrlq		$32,%ymm13,%ymm8
vpsrlq		$32,%ymm14,%ymm9
vpsrlq		$32,%ymm15,%ymm10

#store
vmovdqa		%ymm3,(%rcx)
vmovdqa		%ymm4,32(%rcx)
vmovdqa		%ymm5,64(%rcx)
vmovdqa		%ymm6,96(%rcx)
vmovdqa		%ymm7,128(%rcx)
vmovdqa		%ymm8,160(%rcx)
vmovdqa		%ymm9,192(%rcx)
vmovdqa		%ymm10,224(%rcx)

add		$1,%eax
add		$128,%rsi
add		$256,%rcx
cmp		$8,%eax
jb _looptop

sub		$2048,%rcx
xor		%eax,%eax

.p2align 5
_looptop2:
#load
vmovdqa		(%rcx),%ymm4
vmovdqa		256(%rcx),%ymm5
vmovdqa		512(%rcx),%ymm6
vmovdqa		768(%rcx),%ymm7
vmovdqa		1024(%rcx),%ymm8
vmovdqa		1280(%rcx),%ymm9
vmovdqa		1536(%rcx),%ymm10
vmovdqa		1792(%rcx),%ymm11

#level 5
#update
vpaddd		%ymm2,%ymm4,%ymm12
vpaddd		%ymm2,%ymm6,%ymm13
vpaddd		%ymm2,%ymm8,%ymm14
vpaddd		%ymm2,%ymm10,%ymm15
vpaddd		%ymm5,%ymm4,%ymm4
vpaddd		%ymm7,%ymm6,%ymm6
vpaddd		%ymm9,%ymm8,%ymm8
vpaddd		%ymm11,%ymm10,%ymm10
vpsubd		%ymm5,%ymm12,%ymm5
vpsubd		%ymm7,%ymm13,%ymm7
vpsubd		%ymm9,%ymm14,%ymm9
vpsubd		%ymm11,%ymm15,%ymm11

vpbroadcastq	992(%rdx),%ymm12
vpbroadcastq	996(%rdx),%ymm13
vpbroadcastq	1000(%rdx),%ymm14
vpbroadcastq	1004(%rdx),%ymm15

#mul
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
vpsrlq		$32,%ymm12,%ymm5
vpsrlq		$32,%ymm13,%ymm7
vpsrlq		$32,%ymm14,%ymm9
vpsrlq		$32,%ymm15,%ymm11

#level 6
vpaddd		%ymm2,%ymm4,%ymm12
vpaddd		%ymm2,%ymm5,%ymm13
vpaddd		%ymm2,%ymm8,%ymm14
vpaddd		%ymm2,%ymm9,%ymm15
vpaddd		%ymm6,%ymm4,%ymm4
vpaddd		%ymm7,%ymm5,%ymm5
vpaddd		%ymm10,%ymm8,%ymm8
vpaddd		%ymm11,%ymm9,%ymm9
vpsubd		%ymm6,%ymm12,%ymm6
vpsubd		%ymm7,%ymm13,%ymm7
vpsubd		%ymm10,%ymm14,%ymm10
vpsubd		%ymm11,%ymm15,%ymm11

vpbroadcastq	1008(%rdx),%ymm12
vpbroadcastq	1012(%rdx),%ymm13

#mul
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
vpsrlq		$32,%ymm12,%ymm6
vpsrlq		$32,%ymm13,%ymm7
vpsrlq		$32,%ymm14,%ymm10
vpsrlq		$32,%ymm15,%ymm11

#level 7
vpaddd		%ymm2,%ymm4,%ymm12
vpaddd		%ymm2,%ymm5,%ymm13
vpaddd		%ymm2,%ymm6,%ymm14
vpaddd		%ymm2,%ymm7,%ymm15
vpaddd		%ymm8,%ymm4,%ymm4
vpaddd		%ymm9,%ymm5,%ymm5
vpaddd		%ymm10,%ymm6,%ymm6
vpaddd		%ymm11,%ymm7,%ymm7
vpsubd		%ymm8,%ymm12,%ymm8
vpsubd		%ymm9,%ymm13,%ymm9
vpsubd		%ymm10,%ymm14,%ymm10
vpsubd		%ymm11,%ymm15,%ymm11

vpbroadcastq	_f(%rip),%ymm12

#mul
vpmuludq	%ymm12,%ymm4,%ymm4
vpmuludq	%ymm12,%ymm5,%ymm5
vpmuludq	%ymm12,%ymm6,%ymm6
vpmuludq	%ymm12,%ymm7,%ymm7

#reduce
vpmuludq	%ymm0,%ymm4,%ymm12
vpmuludq	%ymm0,%ymm5,%ymm13
vpmuludq	%ymm0,%ymm6,%ymm14
vpmuludq	%ymm0,%ymm7,%ymm15
vpmuludq	%ymm1,%ymm12,%ymm12
vpmuludq	%ymm1,%ymm13,%ymm13
vpmuludq	%ymm1,%ymm14,%ymm14
vpmuludq	%ymm1,%ymm15,%ymm15
vpaddq		%ymm4,%ymm12,%ymm4
vpaddq		%ymm5,%ymm13,%ymm5
vpaddq		%ymm6,%ymm14,%ymm6
vpaddq		%ymm7,%ymm15,%ymm7

vpbroadcastq	1016(%rdx),%ymm12

#mul
vpmuludq	%ymm12,%ymm8,%ymm8
vpmuludq	%ymm12,%ymm9,%ymm9
vpmuludq	%ymm12,%ymm10,%ymm10
vpmuludq	%ymm12,%ymm11,%ymm11

#reduce
vpmuludq	%ymm0,%ymm8,%ymm12
vpmuludq	%ymm0,%ymm9,%ymm13
vpmuludq	%ymm0,%ymm10,%ymm14
vpmuludq	%ymm0,%ymm11,%ymm15
vpmuludq	%ymm1,%ymm12,%ymm12
vpmuludq	%ymm1,%ymm13,%ymm13
vpmuludq	%ymm1,%ymm14,%ymm14
vpmuludq	%ymm1,%ymm15,%ymm15
vpaddq		%ymm8,%ymm12,%ymm8
vpaddq		%ymm9,%ymm13,%ymm9
vpaddq		%ymm10,%ymm14,%ymm10
vpaddq		%ymm11,%ymm15,%ymm11

#store FIXME
vmovdqa		_mask(%rip),%ymm3
vpermd		%ymm4,%ymm3,%ymm4
vpermd		%ymm5,%ymm3,%ymm5
vpermd		%ymm6,%ymm3,%ymm6
vpermd		%ymm7,%ymm3,%ymm7
vpermd		%ymm8,%ymm3,%ymm8
vpermd		%ymm9,%ymm3,%ymm9
vpermd		%ymm10,%ymm3,%ymm10
vpermd		%ymm11,%ymm3,%ymm11
vmovdqa		%xmm4,(%rdi)
vmovdqa		%xmm5,128(%rdi)
vmovdqa		%xmm6,256(%rdi)
vmovdqa		%xmm7,384(%rdi)
vmovdqa		%xmm8,512(%rdi)
vmovdqa		%xmm9,640(%rdi)
vmovdqa		%xmm10,768(%rdi)
vmovdqa		%xmm11,896(%rdi)

add		$1,%eax
add		$16,%rdi
add		$32,%rcx
cmp		$8,%eax
jb _looptop2

add 		%r11,%rsp
ret
