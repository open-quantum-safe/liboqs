#include "consts.h"
.include "shuffle.inc"

.macro butterfly l,h,zl0=1,zl1=1,zh0=2,zh1=2
vpmuldq		%ymm\zl0,%ymm\h,%ymm13
vmovshdup	%ymm\h,%ymm12
vpmuldq		%ymm\zl1,%ymm12,%ymm14

vpmuldq		%ymm\zh0,%ymm\h,%ymm\h
vpmuldq		%ymm\zh1,%ymm12,%ymm12

vpmuldq		%ymm0,%ymm13,%ymm13
vpmuldq		%ymm0,%ymm14,%ymm14

vmovshdup	%ymm\h,%ymm\h
vpblendd	$0xAA,%ymm12,%ymm\h,%ymm\h

vpsubd		%ymm\h,%ymm\l,%ymm12
vpaddd		%ymm\h,%ymm\l,%ymm\l

vmovshdup	%ymm13,%ymm13
vpblendd	$0xAA,%ymm14,%ymm13,%ymm13

vpaddd		%ymm13,%ymm12,%ymm\h
vpsubd		%ymm13,%ymm\l,%ymm\l
.endm

.macro levels0t1 off
/* level 0 */
vpbroadcastd	(_ZETAS_QINV+1)*4(%rsi),%ymm1
vpbroadcastd	(_ZETAS+1)*4(%rsi),%ymm2

vmovdqa		  0+32*\off(%rdi),%ymm4
vmovdqa		128+32*\off(%rdi),%ymm5
vmovdqa		256+32*\off(%rdi),%ymm6
vmovdqa	 	384+32*\off(%rdi),%ymm7
vmovdqa		512+32*\off(%rdi),%ymm8
vmovdqa		640+32*\off(%rdi),%ymm9
vmovdqa		768+32*\off(%rdi),%ymm10
vmovdqa	 	896+32*\off(%rdi),%ymm11

butterfly	4,8
butterfly	5,9
butterfly	6,10
butterfly	7,11

/* level 1 */
vpbroadcastd	(_ZETAS_QINV+2)*4(%rsi),%ymm1
vpbroadcastd	(_ZETAS+2)*4(%rsi),%ymm2
butterfly	4,6
butterfly	5,7

vpbroadcastd	(_ZETAS_QINV+3)*4(%rsi),%ymm1
vpbroadcastd	(_ZETAS+3)*4(%rsi),%ymm2
butterfly	8,10
butterfly	9,11

vmovdqa		%ymm4,  0+32*\off(%rdi)
vmovdqa		%ymm5,128+32*\off(%rdi)
vmovdqa		%ymm6,256+32*\off(%rdi)
vmovdqa		%ymm7,384+32*\off(%rdi)
vmovdqa		%ymm8,512+32*\off(%rdi)
vmovdqa		%ymm9,640+32*\off(%rdi)
vmovdqa		%ymm10,768+32*\off(%rdi)
vmovdqa		%ymm11,896+32*\off(%rdi)
.endm

.macro levels2t7 off
/* level 2 */
vmovdqa		256*\off+  0(%rdi),%ymm4
vmovdqa		256*\off+ 32(%rdi),%ymm5
vmovdqa		256*\off+ 64(%rdi),%ymm6
vmovdqa	 	256*\off+ 96(%rdi),%ymm7
vmovdqa		256*\off+128(%rdi),%ymm8
vmovdqa		256*\off+160(%rdi),%ymm9
vmovdqa		256*\off+192(%rdi),%ymm10
vmovdqa	 	256*\off+224(%rdi),%ymm11

vpbroadcastd	(_ZETAS_QINV+4+\off)*4(%rsi),%ymm1
vpbroadcastd	(_ZETAS+4+\off)*4(%rsi),%ymm2

butterfly	4,8
butterfly	5,9
butterfly	6,10
butterfly	7,11

shuffle8	4,8,3,8
shuffle8	5,9,4,9
shuffle8	6,10,5,10
shuffle8	7,11,6,11

/* level 3 */
vmovdqa		(_ZETAS_QINV+8+8*\off)*4(%rsi),%ymm1
vmovdqa		(_ZETAS+8+8*\off)*4(%rsi),%ymm2

butterfly	3,5
butterfly	8,10
butterfly	4,6
butterfly	9,11

shuffle4	3,5,7,5
shuffle4	8,10,3,10
shuffle4	4,6,8,6
shuffle4	9,11,4,11

/* level 4 */
vmovdqa		(_ZETAS_QINV+40+8*\off)*4(%rsi),%ymm1
vmovdqa		(_ZETAS+40+8*\off)*4(%rsi),%ymm2

butterfly	7,8
butterfly	5,6
butterfly	3,4
butterfly	10,11

shuffle2	7,8,9,8
shuffle2	5,6,7,6
shuffle2	3,4,5,4
shuffle2	10,11,3,11

/* level 5 */
vmovdqa		(_ZETAS_QINV+72+8*\off)*4(%rsi),%ymm1
vmovdqa		(_ZETAS+72+8*\off)*4(%rsi),%ymm2
vpsrlq		$32,%ymm1,%ymm10
vmovshdup	%ymm2,%ymm15

butterfly	9,5,1,10,2,15
butterfly	8,4,1,10,2,15
butterfly	7,3,1,10,2,15
butterfly	6,11,1,10,2,15

/* level 6 */
vmovdqa		(_ZETAS_QINV+104+8*\off)*4(%rsi),%ymm1
vmovdqa		(_ZETAS+104+8*\off)*4(%rsi),%ymm2
vpsrlq		$32,%ymm1,%ymm10
vmovshdup	%ymm2,%ymm15
butterfly	9,7,1,10,2,15
butterfly	8,6,1,10,2,15

vmovdqa		(_ZETAS_QINV+104+8*\off+32)*4(%rsi),%ymm1
vmovdqa		(_ZETAS+104+8*\off+32)*4(%rsi),%ymm2
vpsrlq		$32,%ymm1,%ymm10
vmovshdup	%ymm2,%ymm15
butterfly	5,3,1,10,2,15
butterfly	4,11,1,10,2,15

/* level 7 */
vmovdqa		(_ZETAS_QINV+168+8*\off)*4(%rsi),%ymm1
vmovdqa		(_ZETAS+168+8*\off)*4(%rsi),%ymm2
vpsrlq		$32,%ymm1,%ymm10
vmovshdup	%ymm2,%ymm15
butterfly	9,8,1,10,2,15

vmovdqa		(_ZETAS_QINV+168+8*\off+32)*4(%rsi),%ymm1
vmovdqa		(_ZETAS+168+8*\off+32)*4(%rsi),%ymm2
vpsrlq		$32,%ymm1,%ymm10
vmovshdup	%ymm2,%ymm15
butterfly	7,6,1,10,2,15

vmovdqa		(_ZETAS_QINV+168+8*\off+64)*4(%rsi),%ymm1
vmovdqa		(_ZETAS+168+8*\off+64)*4(%rsi),%ymm2
vpsrlq		$32,%ymm1,%ymm10
vmovshdup	%ymm2,%ymm15
butterfly	5,4,1,10,2,15

vmovdqa		(_ZETAS_QINV+168+8*\off+96)*4(%rsi),%ymm1
vmovdqa		(_ZETAS+168+8*\off+96)*4(%rsi),%ymm2
vpsrlq		$32,%ymm1,%ymm10
vmovshdup	%ymm2,%ymm15
butterfly	3,11,1,10,2,15

vmovdqa		%ymm9,256*\off+  0(%rdi)
vmovdqa		%ymm8,256*\off+ 32(%rdi)
vmovdqa		%ymm7,256*\off+ 64(%rdi)
vmovdqa		%ymm6,256*\off+ 96(%rdi)
vmovdqa		%ymm5,256*\off+128(%rdi)
vmovdqa		%ymm4,256*\off+160(%rdi)
vmovdqa		%ymm3,256*\off+192(%rdi)
vmovdqa		%ymm11,256*\off+224(%rdi)
.endm

.text
.global cdecl(ntt_avx)
cdecl(ntt_avx):
vmovdqa		_8XQ*4(%rsi),%ymm0

levels0t1	0
levels0t1	1
levels0t1	2
levels0t1	3

levels2t7	0
levels2t7	1
levels2t7	2
levels2t7	3

ret

.section .note.GNU-stack,"",@progbits
