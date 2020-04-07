.include "shuffle.inc"
.include "fq.inc"

.macro butterfly rl0,rl1,rl2,rl3,rh0,rh1,rh2,rh3 zl0=1,zl1=1,zh0=2,zh1=2
#update & mul
vpsubw		%ymm\rh0,%ymm\rl0,%ymm12
vpsubw		%ymm\rh1,%ymm\rl1,%ymm13
vpsubw		%ymm\rh2,%ymm\rl2,%ymm14

vpaddw		%ymm\rh0,%ymm\rl0,%ymm\rl0
vpaddw		%ymm\rh1,%ymm\rl1,%ymm\rl1
vpmullw		%ymm\zl0,%ymm12,%ymm\rh0

vpaddw		%ymm\rh2,%ymm\rl2,%ymm\rl2
vpmullw		%ymm\zl0,%ymm13,%ymm\rh1
vpsubw		%ymm\rh3,%ymm\rl3,%ymm15

vpaddw		%ymm\rh3,%ymm\rl3,%ymm\rl3
vpmullw		%ymm\zl1,%ymm14,%ymm\rh2
vpmullw		%ymm\zl1,%ymm15,%ymm\rh3

vpmulhw		%ymm\zh0,%ymm12,%ymm12
vpmulhw		%ymm\zh0,%ymm13,%ymm13

vpmulhw		%ymm\zh1,%ymm14,%ymm14
vpmulhw		%ymm\zh1,%ymm15,%ymm15

#reduce
vpmulhw		%ymm0,%ymm\rh0,%ymm\rh0
vpmulhw		%ymm0,%ymm\rh1,%ymm\rh1
vpmulhw		%ymm0,%ymm\rh2,%ymm\rh2
vpmulhw		%ymm0,%ymm\rh3,%ymm\rh3
vpsubw		%ymm\rh0,%ymm12,%ymm\rh0
vpsubw		%ymm\rh1,%ymm13,%ymm\rh1
vpsubw		%ymm\rh2,%ymm14,%ymm\rh2
vpsubw		%ymm\rh3,%ymm15,%ymm\rh3
.endm

.global PQCLEAN_KYBER51290S_AVX2_invntt_levels0t5_avx
.p2align 5
PQCLEAN_KYBER51290S_AVX2_invntt_levels0t5_avx:
#consts
vmovdqa         PQCLEAN_KYBER51290S_AVX2_16xq(%rip),%ymm0

level0:
#zetas
vmovdqu		(%rsi),%ymm15
vmovdqu		64(%rsi),%ymm3
vmovdqu		32(%rsi),%ymm1
vmovdqu		96(%rsi),%ymm2

#load
vmovdqa         (%rdi),%ymm4
vmovdqa         32(%rdi),%ymm5
vmovdqa         64(%rdi),%ymm6
vmovdqa         96(%rdi),%ymm7
vmovdqa         128(%rdi),%ymm8
vmovdqa         160(%rdi),%ymm9
vmovdqa         192(%rdi),%ymm10
vmovdqa         224(%rdi),%ymm11

butterfly	4,5,8,9,6,7,10,11 15,3,1,2

level1:
#zetas
vmovdqu		128(%rsi),%ymm3
vmovdqu		160(%rsi),%ymm2

butterfly	4,5,6,7,8,9,10,11 3,3,2,2

shuffle1	4,5,3,5
shuffle1	6,7,4,7
shuffle1	8,9,6,9
shuffle1	10,11,8,11

level2:
#zetas
vmovdqu		192(%rsi),%ymm10
vmovdqu		224(%rsi),%ymm2

#consts
vmovdqa		PQCLEAN_KYBER51290S_AVX2_16xv(%rip),%ymm1

butterfly	3,4,6,8,5,7,9,11 10,10,2,2

red16		3

shuffle2	3,4,10,4
shuffle2	6,8,3,8
shuffle2	5,7,6,7
shuffle2	9,11,5,11

level3:
#zetas
vmovdqu		256(%rsi),%ymm9
vmovdqu		288(%rsi),%ymm2

butterfly	10,3,6,5,4,8,7,11 9,9,2,2

red16		10

shuffle4	10,3,9,3
shuffle4	6,5,10,5
shuffle4	4,8,6,8
shuffle4	7,11,4,11

level4:
#zetas
vmovdqu		320(%rsi),%ymm7
vmovdqu		352(%rsi),%ymm2

butterfly	9,10,6,4,3,5,8,11 7,7,2,2

red16		9

shuffle8	9,10,7,10
shuffle8	6,4,9,4
shuffle8	3,5,6,5
shuffle8	8,11,3,11

level5:
#zetas
vpbroadcastd	384(%rsi),%ymm8
vpbroadcastd	388(%rsi),%ymm2

butterfly	7,9,6,3,10,4,5,11 8,8,2,2

red16		7

#store
vmovdqa         %ymm7,(%rdi)
vmovdqa         %ymm9,32(%rdi)
vmovdqa         %ymm6,64(%rdi)
vmovdqa         %ymm3,96(%rdi)
vmovdqa         %ymm10,128(%rdi)
vmovdqa         %ymm4,160(%rdi)
vmovdqa         %ymm5,192(%rdi)
vmovdqa         %ymm11,224(%rdi)

ret

.global PQCLEAN_KYBER51290S_AVX2_invntt_level6_avx
PQCLEAN_KYBER51290S_AVX2_invntt_level6_avx:
#consts
vmovdqa		PQCLEAN_KYBER51290S_AVX2_16xq(%rip),%ymm0

#zetas
vpbroadcastd	(%rsi),%ymm1
vpbroadcastd	4(%rsi),%ymm2

#load
vmovdqa         (%rdi),%ymm4
vmovdqa         32(%rdi),%ymm5
vmovdqa         64(%rdi),%ymm6
vmovdqa         96(%rdi),%ymm7
vmovdqa         256(%rdi),%ymm8
vmovdqa         288(%rdi),%ymm9
vmovdqa         320(%rdi),%ymm10
vmovdqa         352(%rdi),%ymm11

butterfly	4,5,6,7,8,9,10,11

#consts
vmovdqa		PQCLEAN_KYBER51290S_AVX2_16xflo(%rip),%ymm12
vmovdqa		PQCLEAN_KYBER51290S_AVX2_16xfhi(%rip),%ymm13

#store
vmovdqa		%ymm8,256(%rdi)
vmovdqa		%ymm9,288(%rdi)
vmovdqa		%ymm10,320(%rdi)
vmovdqa		%ymm11,352(%rdi)

fqmulprecomp	12,13,4 8
fqmulprecomp	12,13,5 9
fqmulprecomp	12,13,6 10
fqmulprecomp	12,13,7 11

#store
vmovdqa		%ymm4,(%rdi)
vmovdqa		%ymm5,32(%rdi)
vmovdqa		%ymm6,64(%rdi)
vmovdqa		%ymm7,96(%rdi)

#load
vmovdqa         128(%rdi),%ymm4
vmovdqa         160(%rdi),%ymm5
vmovdqa         192(%rdi),%ymm6
vmovdqa         224(%rdi),%ymm7
vmovdqa         384(%rdi),%ymm8
vmovdqa         416(%rdi),%ymm9
vmovdqa         448(%rdi),%ymm10
vmovdqa         480(%rdi),%ymm11

butterfly	4,5,6,7,8,9,10,11

#consts
vmovdqa		PQCLEAN_KYBER51290S_AVX2_16xflo(%rip),%ymm12
vmovdqa		PQCLEAN_KYBER51290S_AVX2_16xfhi(%rip),%ymm13

#store
vmovdqa		%ymm8,384(%rdi)
vmovdqa		%ymm9,416(%rdi)
vmovdqa		%ymm10,448(%rdi)
vmovdqa		%ymm11,480(%rdi)

fqmulprecomp	12,13,4 8
fqmulprecomp	12,13,5 9
fqmulprecomp	12,13,6 10
fqmulprecomp	12,13,7 11

#store
vmovdqa		%ymm4,128(%rdi)
vmovdqa		%ymm5,160(%rdi)
vmovdqa		%ymm6,192(%rdi)
vmovdqa		%ymm7,224(%rdi)

ret
