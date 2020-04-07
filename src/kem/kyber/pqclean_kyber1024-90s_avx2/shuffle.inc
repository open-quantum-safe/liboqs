.macro shuffle8 r0,r1,r2,r3
vperm2i128	$0x20,%ymm\r1,%ymm\r0,%ymm\r2
vperm2i128	$0x31,%ymm\r1,%ymm\r0,%ymm\r3
.endm

.macro shuffle4 r0,r1,r2,r3
vpunpcklqdq	%ymm\r1,%ymm\r0,%ymm\r2
vpunpckhqdq	%ymm\r1,%ymm\r0,%ymm\r3
.endm

.macro shuffle2 r0,r1,r2,r3
vpsllq		$32,%ymm\r1,%ymm12
vpsrlq		$32,%ymm\r0,%ymm13
vpblendd	$0xAA,%ymm12,%ymm\r0,%ymm\r2
vpblendd	$0xAA,%ymm\r1,%ymm13,%ymm\r3
.endm

.macro shuffle1 r0,r1,r2,r3
vpslld		$16,%ymm\r1,%ymm12
vpsrld		$16,%ymm\r0,%ymm13
vpblendw	$0xAA,%ymm12,%ymm\r0,%ymm\r2
vpblendw	$0xAA,%ymm\r1,%ymm13,%ymm\r3
.endm
