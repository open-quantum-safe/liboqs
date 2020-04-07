.include "fq.inc"

.global PQCLEAN_KYBER1024_AVX2_reduce_avx
PQCLEAN_KYBER1024_AVX2_reduce_avx:
#consts
vmovdqa		PQCLEAN_KYBER1024_AVX2_16xq(%rip),%ymm0
vmovdqa		PQCLEAN_KYBER1024_AVX2_16xv(%rip),%ymm1

#load
vmovdqa		(%rdi),%ymm2
vmovdqa		32(%rdi),%ymm3
vmovdqa		64(%rdi),%ymm4
vmovdqa		96(%rdi),%ymm5
vmovdqa		128(%rdi),%ymm6
vmovdqa		160(%rdi),%ymm7
vmovdqa		192(%rdi),%ymm8
vmovdqa		224(%rdi),%ymm9

red16		2 10
red16		3 11
red16		4 12
red16		5 13
red16		6 14
red16		7 15
red16		8 10
red16		9 11

#store
vmovdqa		%ymm2,(%rdi)
vmovdqa		%ymm3,32(%rdi)
vmovdqa		%ymm4,64(%rdi)
vmovdqa		%ymm5,96(%rdi)
vmovdqa		%ymm6,128(%rdi)
vmovdqa		%ymm7,160(%rdi)
vmovdqa		%ymm8,192(%rdi)
vmovdqa		%ymm9,224(%rdi)

ret

.global PQCLEAN_KYBER1024_AVX2_csubq_avx
PQCLEAN_KYBER1024_AVX2_csubq_avx:
#consts
vmovdqa		PQCLEAN_KYBER1024_AVX2_16xq(%rip),%ymm0

#load
vmovdqa		(%rdi),%ymm1
vmovdqa		32(%rdi),%ymm2
vmovdqa		64(%rdi),%ymm3
vmovdqa		96(%rdi),%ymm4
vmovdqa		128(%rdi),%ymm5
vmovdqa		160(%rdi),%ymm6
vmovdqa		192(%rdi),%ymm7
vmovdqa		224(%rdi),%ymm8

csubq		1 9
csubq		2 10
csubq		3 11
csubq		4 12
csubq		5 13
csubq		6 14
csubq		7 15
csubq		8 9

#store
vmovdqa		%ymm1,(%rdi)
vmovdqa		%ymm2,32(%rdi)
vmovdqa		%ymm3,64(%rdi)
vmovdqa		%ymm4,96(%rdi)
vmovdqa		%ymm5,128(%rdi)
vmovdqa		%ymm6,160(%rdi)
vmovdqa		%ymm7,192(%rdi)
vmovdqa		%ymm8,224(%rdi)

ret

.global PQCLEAN_KYBER1024_AVX2_frommont_avx
PQCLEAN_KYBER1024_AVX2_frommont_avx:
#consts
vmovdqa		PQCLEAN_KYBER1024_AVX2_16xq(%rip),%ymm0
vmovdqa		PQCLEAN_KYBER1024_AVX2_16xmontsqlo(%rip),%ymm1
vmovdqa		PQCLEAN_KYBER1024_AVX2_16xmontsqhi(%rip),%ymm2

#load
vmovdqa		(%rdi),%ymm3
vmovdqa		32(%rdi),%ymm4
vmovdqa		64(%rdi),%ymm5
vmovdqa		96(%rdi),%ymm6
vmovdqa		128(%rdi),%ymm7
vmovdqa		160(%rdi),%ymm8
vmovdqa		192(%rdi),%ymm9
vmovdqa		224(%rdi),%ymm10

fqmulprecomp	1,2,3 11
fqmulprecomp	1,2,4 12
fqmulprecomp	1,2,5 13
fqmulprecomp	1,2,6 14
fqmulprecomp	1,2,7 15
fqmulprecomp	1,2,8 11
fqmulprecomp	1,2,9 12
fqmulprecomp	1,2,10 13

#store
vmovdqa		%ymm3,(%rdi)
vmovdqa		%ymm4,32(%rdi)
vmovdqa		%ymm5,64(%rdi)
vmovdqa		%ymm6,96(%rdi)
vmovdqa		%ymm7,128(%rdi)
vmovdqa		%ymm8,160(%rdi)
vmovdqa		%ymm9,192(%rdi)
vmovdqa		%ymm10,224(%rdi)

ret
