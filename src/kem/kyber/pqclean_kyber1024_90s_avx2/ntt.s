.include "shuffle.inc"
.include "fq.inc"

.macro butterfly rl0,rl1,rl2,rl3,rh0,rh1,rh2,rh3 zl0=15,zl1=15,zh0=1,zh1=1
#mul
vpmullw		%ymm\zl0,%ymm\rh0,%ymm12
vpmullw		%ymm\zl0,%ymm\rh1,%ymm13
vpmullw		%ymm\zl1,%ymm\rh2,%ymm14
vpmullw		%ymm\zl1,%ymm\rh3,%ymm15
vpmulhw		%ymm\zh0,%ymm\rh0,%ymm\rh0
vpmulhw		%ymm\zh0,%ymm\rh1,%ymm\rh1
vpmulhw		%ymm\zh1,%ymm\rh2,%ymm\rh2
vpmulhw		%ymm\zh1,%ymm\rh3,%ymm\rh3

#reduce
vpmulhw		%ymm0,%ymm12,%ymm12
vpmulhw		%ymm0,%ymm13,%ymm13
vpmulhw		%ymm0,%ymm14,%ymm14
vpmulhw		%ymm0,%ymm15,%ymm15
vpsubw		%ymm12,%ymm\rh0,%ymm12
vpsubw		%ymm13,%ymm\rh1,%ymm13
vpsubw		%ymm14,%ymm\rh2,%ymm14
vpsubw		%ymm15,%ymm\rh3,%ymm15

#update
vpsubw		%ymm12,%ymm\rl0,%ymm\rh0
vpaddw		%ymm12,%ymm\rl0,%ymm\rl0
vpsubw		%ymm13,%ymm\rl1,%ymm\rh1
vpaddw		%ymm13,%ymm\rl1,%ymm\rl1
vpsubw		%ymm14,%ymm\rl2,%ymm\rh2
vpaddw		%ymm14,%ymm\rl2,%ymm\rl2
vpsubw		%ymm15,%ymm\rl3,%ymm\rh3
vpaddw		%ymm15,%ymm\rl3,%ymm\rl3
.endm

# We break the dependency chains with the cost of slightly more additions.
# But they can be run in parallel to the multiplications on execution port 5
# (multiplications only go to ports 0 and 1)
.macro butterfly2 rl0,rl1,rl2,rl3,rh0,rh1,rh2,rh3 x=3,y=2,zl0=15,zl1=15,zh0=1,zh1=1
#mul
vpmullw		%ymm\zl0,%ymm\rh0,%ymm12
vpmulhw		%ymm\zh0,%ymm\rh0,%ymm\x
vpmullw		%ymm\zl0,%ymm\rh1,%ymm13
vpmulhw		%ymm\zh0,%ymm\rh1,%ymm\rh0
vpmullw		%ymm\zl1,%ymm\rh2,%ymm14
vpmulhw		%ymm\zh1,%ymm\rh2,%ymm\y
vpmullw		%ymm\zl1,%ymm\rh3,%ymm15
vpmulhw		%ymm\zh1,%ymm\rh3,%ymm\rh2

#reduce
vpmulhw		%ymm0,%ymm12,%ymm12
vpmulhw		%ymm0,%ymm13,%ymm13
vpmulhw		%ymm0,%ymm14,%ymm14
vpmulhw		%ymm0,%ymm15,%ymm15

vpsubw		%ymm\rh0,%ymm\rl1,%ymm\rh1
vpaddw		%ymm\rh0,%ymm\rl1,%ymm\rl1
vpsubw		%ymm\x,%ymm\rl0,%ymm\rh0
vpaddw		%ymm\x,%ymm\rl0,%ymm\rl0
vpsubw		%ymm\rh2,%ymm\rl3,%ymm\rh3
vpaddw		%ymm\rh2,%ymm\rl3,%ymm\rl3
vpsubw		%ymm\y,%ymm\rl2,%ymm\rh2
vpaddw		%ymm\y,%ymm\rl2,%ymm\rl2

#update
vpaddw		%ymm12,%ymm\rh0,%ymm\rh0
vpsubw		%ymm12,%ymm\rl0,%ymm\rl0
vpaddw		%ymm13,%ymm\rh1,%ymm\rh1
vpsubw		%ymm13,%ymm\rl1,%ymm\rl1
vpaddw		%ymm14,%ymm\rh2,%ymm\rh2
vpsubw		%ymm14,%ymm\rl2,%ymm\rl2
vpaddw		%ymm15,%ymm\rh3,%ymm\rh3
vpsubw		%ymm15,%ymm\rl3,%ymm\rl3
.endm

.global PQCLEAN_KYBER102490S_AVX2_ntt_level0_avx
PQCLEAN_KYBER102490S_AVX2_ntt_level0_avx:
#consts
vmovdqa		PQCLEAN_KYBER102490S_AVX2_16xq(%rip),%ymm0

level0:
#zetas
vpbroadcastd	(%rsi),%ymm15
vpbroadcastd	4(%rsi),%ymm1

#load
vmovdqa		(%rdi),%ymm4
vmovdqa		32(%rdi),%ymm5
vmovdqa		64(%rdi),%ymm6
vmovdqa		96(%rdi),%ymm7
vmovdqa		256(%rdi),%ymm8
vmovdqa		288(%rdi),%ymm9
vmovdqa		320(%rdi),%ymm10
vmovdqa		352(%rdi),%ymm11

butterfly2	4,5,6,7,8,9,10,11

#store
vmovdqa		%ymm4,(%rdi)
vmovdqa		%ymm5,32(%rdi)
vmovdqa		%ymm6,64(%rdi)
vmovdqa		%ymm7,96(%rdi)
vmovdqa		%ymm8,256(%rdi)
vmovdqa		%ymm9,288(%rdi)
vmovdqa		%ymm10,320(%rdi)
vmovdqa		%ymm11,352(%rdi)

ret

.global PQCLEAN_KYBER102490S_AVX2_ntt_levels1t6_avx
PQCLEAN_KYBER102490S_AVX2_ntt_levels1t6_avx:
#consts
vmovdqa		PQCLEAN_KYBER102490S_AVX2_16xq(%rip),%ymm0

level1:
#zetas
vpbroadcastd	(%rsi),%ymm15
vpbroadcastd	4(%rsi),%ymm1

#load
vmovdqa		(%rdi),%ymm4
vmovdqa		32(%rdi),%ymm5
vmovdqa		64(%rdi),%ymm6
vmovdqa		96(%rdi),%ymm7
vmovdqa		128(%rdi),%ymm8
vmovdqa		160(%rdi),%ymm9
vmovdqa		192(%rdi),%ymm10
vmovdqa		224(%rdi),%ymm11

butterfly2	4,5,6,7,8,9,10,11 3

level2:
#zetas
vmovdqu		8(%rsi),%ymm15
vmovdqu		40(%rsi),%ymm1

shuffle8	4,8,3,8
shuffle8	5,9,4,9
shuffle8	6,10,5,10
shuffle8	7,11,6,11

butterfly2	3,8,4,9,5,10,6,11 7

level3:
#zetas
vmovdqu		72(%rsi),%ymm15
vmovdqu		104(%rsi),%ymm1

shuffle4	3,5,7,5
shuffle4	8,10,3,10
shuffle4	4,6,8,6
shuffle4	9,11,4,11

butterfly2	7,5,3,10,8,6,4,11 9

level4:
#zetas
vmovdqu		136(%rsi),%ymm15
vmovdqu		168(%rsi),%ymm1

shuffle2	7,8,9,8
shuffle2	5,6,7,6
shuffle2	3,4,5,4
shuffle2	10,11,3,11

butterfly2	9,8,7,6,5,4,3,11 10

level5:
#zetas
vmovdqu		200(%rsi),%ymm15
vmovdqu		232(%rsi),%ymm1

shuffle1	9,5,10,5
shuffle1	8,4,9,4
shuffle1	7,3,8,3
shuffle1	6,11,7,11

butterfly2	10,5,9,4,8,3,7,11 6

level6:
#zetas
vmovdqu		264(%rsi),%ymm14
vmovdqu		328(%rsi),%ymm15
vmovdqu		296(%rsi),%ymm1
vmovdqu		360(%rsi),%ymm2

butterfly2	10,5,8,3,9,4,7,11 6,1,14,15,1,2

vmovdqa		PQCLEAN_KYBER102490S_AVX2_16xv(%rip),%ymm1
red16		10 12
red16		5 13
red16		9 14
red16		4 15
red16		8 2
red16		3 6
red16		7 12
red16		11 13

#store
vmovdqa		%ymm10,(%rdi)
vmovdqa		%ymm5,32(%rdi)
vmovdqa		%ymm9,64(%rdi)
vmovdqa		%ymm4,96(%rdi)
vmovdqa		%ymm8,128(%rdi)
vmovdqa		%ymm3,160(%rdi)
vmovdqa		%ymm7,192(%rdi)
vmovdqa		%ymm11,224(%rdi)

ret
