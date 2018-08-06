.global pointwise_mul
pointwise_mul:
mov		%rsp,%r11
and		$31,%r11
sub		%r11,%rsp

vmovdqa		_8xqinv(%rip),%ymm0
vmovdqa		_8xq(%rip),%ymm1

xor		%eax,%eax

.p2align 5
_looptop:
#load
vmovdqa		(%rsi),%ymm2
vmovdqa		32(%rsi),%ymm4
vmovdqa		64(%rsi),%ymm6
vmovdqa		(%rdx),%ymm10
vmovdqa		32(%rdx),%ymm12
vmovdqa		64(%rdx),%ymm14
vpsrlq		$32,%ymm2,%ymm3
vpsrlq		$32,%ymm4,%ymm5
vpsrlq		$32,%ymm6,%ymm7
vpsrlq		$32,%ymm10,%ymm11
vpsrlq		$32,%ymm12,%ymm13
vpsrlq		$32,%ymm14,%ymm15

#mul
vpmuludq	%ymm2,%ymm10,%ymm2
vpmuludq	%ymm3,%ymm11,%ymm3
vpmuludq	%ymm4,%ymm12,%ymm4
vpmuludq	%ymm5,%ymm13,%ymm5
vpmuludq	%ymm6,%ymm14,%ymm6
vpmuludq	%ymm7,%ymm15,%ymm7

#reduce
vpmuludq	%ymm0,%ymm2,%ymm10
vpmuludq	%ymm0,%ymm3,%ymm11
vpmuludq	%ymm0,%ymm4,%ymm12
vpmuludq	%ymm0,%ymm5,%ymm13
vpmuludq	%ymm0,%ymm6,%ymm14
vpmuludq	%ymm0,%ymm7,%ymm15
vpmuludq	%ymm1,%ymm10,%ymm10
vpmuludq	%ymm1,%ymm11,%ymm11
vpmuludq	%ymm1,%ymm12,%ymm12
vpmuludq	%ymm1,%ymm13,%ymm13
vpmuludq	%ymm1,%ymm14,%ymm14
vpmuludq	%ymm1,%ymm15,%ymm15
vpaddq		%ymm2,%ymm10,%ymm2
vpaddq		%ymm3,%ymm11,%ymm3
vpaddq		%ymm4,%ymm12,%ymm4
vpaddq		%ymm5,%ymm13,%ymm5
vpaddq		%ymm6,%ymm14,%ymm6
vpaddq		%ymm7,%ymm15,%ymm7
vpsrlq		$32,%ymm2,%ymm2
vpsrlq		$32,%ymm4,%ymm4
vpsrlq		$32,%ymm6,%ymm6

#store
vpblendd	$0x55,%ymm2,%ymm3,%ymm2
vpblendd	$0x55,%ymm4,%ymm5,%ymm4
vpblendd	$0x55,%ymm6,%ymm7,%ymm6
vmovdqa		%ymm2,(%rdi)
vmovdqa		%ymm4,32(%rdi)
vmovdqa		%ymm6,64(%rdi)

add		$1,%eax
add		$96,%rdi
add		$96,%rsi
add		$96,%rdx
cmp		$10,%eax
jb _looptop

vmovdqa		(%rsi),%ymm2
vmovdqa		32(%rsi),%ymm4
vmovdqa		(%rdx),%ymm10
vmovdqa		32(%rdx),%ymm12
vpsrlq		$32,%ymm2,%ymm3
vpsrlq		$32,%ymm4,%ymm5
vpsrlq		$32,%ymm10,%ymm11
vpsrlq		$32,%ymm12,%ymm13

#mul
vpmuludq	%ymm2,%ymm10,%ymm2
vpmuludq	%ymm3,%ymm11,%ymm3
vpmuludq	%ymm4,%ymm12,%ymm4
vpmuludq	%ymm5,%ymm13,%ymm5

#reduce
vpmuludq	%ymm0,%ymm2,%ymm10
vpmuludq	%ymm0,%ymm3,%ymm11
vpmuludq	%ymm0,%ymm4,%ymm12
vpmuludq	%ymm0,%ymm5,%ymm13
vpmuludq	%ymm1,%ymm10,%ymm10
vpmuludq	%ymm1,%ymm11,%ymm11
vpmuludq	%ymm1,%ymm12,%ymm12
vpmuludq	%ymm1,%ymm13,%ymm13
vpaddq		%ymm2,%ymm10,%ymm2
vpaddq		%ymm3,%ymm11,%ymm3
vpaddq		%ymm4,%ymm12,%ymm4
vpaddq		%ymm5,%ymm13,%ymm5
vpsrlq		$32,%ymm2,%ymm2
vpsrlq		$32,%ymm4,%ymm4

#store
vpblendd	$0x55,%ymm2,%ymm3,%ymm2
vpblendd	$0x55,%ymm4,%ymm5,%ymm4
vmovdqa		%ymm2,(%rdi)
vmovdqa		%ymm4,32(%rdi)

add		%r11,%rsp
ret
