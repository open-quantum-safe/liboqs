#include "params.h"

.macro schoolbook off,sign
#load
vmovdqa		\off+32(%rsi),%ymm7	# b
vmovdqa		\off+32(%rdx),%ymm8	# d
vmovdqa		\off(%rsi),%ymm9	# a
vmovdqa		\off(%rdx),%ymm10	# c

#mul
vpmullw		%ymm7,%ymm8,%ymm11	# bd.lo
vpmulhw		%ymm7,%ymm8,%ymm12	# bd.hi
vpmullw		%ymm7,%ymm10,%ymm13	# bc.lo
vpmulhw		%ymm7,%ymm10,%ymm7	# bc.hi
vpmullw		%ymm9,%ymm8,%ymm14	# ad.lo
vpmulhw		%ymm9,%ymm8,%ymm8	# ad.hi
vpmullw		%ymm9,%ymm10,%ymm15	# ac.lo
vpmulhw		%ymm9,%ymm10,%ymm9	# ac.hi

#reduce
vpmullw		%ymm1,%ymm11,%ymm11
vpmulhw		%ymm0,%ymm11,%ymm11
vpsubw		%ymm11,%ymm12,%ymm11	# bd

#mul
vpmullw		%ymm2,%ymm11,%ymm10	# rbd.lo
vpmulhw		%ymm2,%ymm11,%ymm11	# rbd.hi

#unpack
vpunpcklwd	%ymm7,%ymm13,%ymm12	# bc0
vpunpckhwd	%ymm7,%ymm13,%ymm7	# bc1
vpunpcklwd	%ymm8,%ymm14,%ymm13	# ad0
vpunpckhwd	%ymm8,%ymm14,%ymm8	# ad1
vpunpcklwd	%ymm9,%ymm15,%ymm14	# ac0
vpunpckhwd	%ymm9,%ymm15,%ymm9	# ac1
vpunpcklwd	%ymm11,%ymm10,%ymm15	# rbd0
vpunpckhwd	%ymm11,%ymm10,%ymm10	# rbd1

#add
.ifeq \sign
vpaddd		%ymm14,%ymm15,%ymm14	# x0
vpaddd		%ymm9,%ymm10,%ymm9	# x1
.else
vpsubd		%ymm15,%ymm14,%ymm14	# x0
vpsubd		%ymm10,%ymm9,%ymm9	# x1
.endif
vpaddd		%ymm12,%ymm13,%ymm12	# y0
vpaddd		%ymm7,%ymm8,%ymm7	# y1
.endm

.macro red a0,a1,b0,b1 x,y,z
#pack
vpxor		%ymm\x,%ymm\x,%ymm\x
vpblendw	$0xAA,%ymm\x,%ymm\a0,%ymm\y
vpblendw	$0xAA,%ymm\x,%ymm\a1,%ymm\z
vpsrld		$16,%ymm\a0,%ymm\a0
vpsrld		$16,%ymm\a1,%ymm\a1
vpackusdw	%ymm\z,%ymm\y,%ymm\z
vpackusdw	%ymm\a1,%ymm\a0,%ymm\a0
vpblendw	$0xAA,%ymm\x,%ymm\b0,%ymm\y
vpblendw	$0xAA,%ymm\x,%ymm\b1,%ymm\x
vpsrld		$16,%ymm\b0,%ymm\b0
vpsrld		$16,%ymm\b1,%ymm\b1
vpackusdw	%ymm\x,%ymm\y,%ymm\y
vpackusdw	%ymm\b1,%ymm\b0,%ymm\b0

#reduce
vpmullw		%ymm1,%ymm\z,%ymm\z
vpmullw		%ymm1,%ymm\y,%ymm\y
vpmulhw		%ymm0,%ymm\z,%ymm\z
vpmulhw		%ymm0,%ymm\y,%ymm\y
vpsubw		%ymm\z,%ymm\a0,%ymm\a0
vpsubw		%ymm\y,%ymm\b0,%ymm\b0
.endm

.global PQCLEAN_KYBER512_AVX2_basemul_acc_avx
PQCLEAN_KYBER512_AVX2_basemul_acc_avx:
#consts
vmovdqa		PQCLEAN_KYBER512_AVX2_16xq(%rip),%ymm0
vmovdqa		PQCLEAN_KYBER512_AVX2_16xqinv(%rip),%ymm1
vmovdqu		(%rcx),%ymm2

poly0.0:
schoolbook	0,0

#mov
vmovdqa		%ymm14,%ymm3
vmovdqa		%ymm9,%ymm4
vmovdqa		%ymm12,%ymm5
vmovdqa		%ymm7,%ymm6

poly1.0:
schoolbook	512,0

#add
vpaddd		%ymm14,%ymm3,%ymm3
vpaddd		%ymm9,%ymm4,%ymm4
vpaddd		%ymm12,%ymm5,%ymm5
vpaddd		%ymm7,%ymm6,%ymm6



#reduce
red		3,4,5,6 7,8,9

#store
vmovdqa		%ymm3,(%rdi)
vmovdqa		%ymm5,32(%rdi)

poly0.1:
schoolbook	64,1

#mov
vmovdqa		%ymm14,%ymm3
vmovdqa		%ymm9,%ymm4
vmovdqa		%ymm12,%ymm5
vmovdqa		%ymm7,%ymm6

poly1.1:
schoolbook	576,1

#add
vpaddd		%ymm14,%ymm3,%ymm3
vpaddd		%ymm9,%ymm4,%ymm4
vpaddd		%ymm12,%ymm5,%ymm5
vpaddd		%ymm7,%ymm6,%ymm6



#reduce
red		3,4,5,6 7,8,9

#store
vmovdqa		%ymm3,64(%rdi)
vmovdqa		%ymm5,96(%rdi)

ret

.global PQCLEAN_KYBER512_AVX2_basemul_avx
PQCLEAN_KYBER512_AVX2_basemul_avx:
#consts
vmovdqa		PQCLEAN_KYBER512_AVX2_16xq(%rip),%ymm0
vmovdqa		PQCLEAN_KYBER512_AVX2_16xqinv(%rip),%ymm1
vmovdqu		(%rcx),%ymm2

schoolbook	0,0

#reduce
red		14,9,12,7 8,10,11

#store
vmovdqa		%ymm14,(%rdi)
vmovdqa		%ymm12,32(%rdi)

schoolbook	64,1

#reduce
red		14,9,12,7 8,10,11

#store
vmovdqa		%ymm14,64(%rdi)
vmovdqa		%ymm12,96(%rdi)

ret
