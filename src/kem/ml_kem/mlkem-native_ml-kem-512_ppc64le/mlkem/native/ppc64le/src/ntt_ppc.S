/*
 * Copyright (c) 2024-2025 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

#
# Copyright 2025- IBM Corp.
#
#===================================================================================
# Written by Danny Tsen <dtsen@us.ibm.com>
#

#define V_QI1	2
#define V_QI2	3
#define V_QI8	4
#define V_NMKQ	5
#define	V_Z0	7
#define	V_Z1	8
#define	V_Z2	9
#define	V_Z3	10
#define V_ZETA	10
#define V_16	11

.machine "any"
.text

#
# montgomery_reduce
# t = a * QINV
# t = (a - (int32_t)t*_MLKEM_Q) >> 16
#
#-----------------------------------
# MREDUCE_4X(start, _vz0, _vz1, _vz2, _vz3)
#
.macro MREDUCE_4X start next step _vz0 _vz1 _vz2 _vz3
	mr	9, \start
	add	10, 4, 9	# J + len*2
	addi	16, 9, \next
	addi	17, 10, \step
	addi	18, 16, \next
	addi	19, 17, \step
	addi	20, 18, \next
	addi	21, 19, \step
	lxvd2x	32+13, 3, 10	# r[j+len]
	lxvd2x	32+18, 3, 17	# r[j+len]
	lxvd2x	32+23, 3, 19	# r[j+len]
	lxvd2x	32+28, 3, 21	# r[j+len]
	xxpermdi 32+13, 32+13, 32+13, 2
	xxpermdi 32+18, 32+18, 32+18, 2
	xxpermdi 32+23, 32+23, 32+23, 2
	xxpermdi 32+28, 32+28, 32+28, 2

	# fqmul = zeta * coefficient
	vmulosh	12, 13, \_vz0
	vmulesh	13, 13, \_vz0
	vmulosh	17, 18, \_vz1
	vmulesh	18, 18, \_vz1
	vmulosh	22, 23, \_vz2
	vmulesh	23, 23, \_vz2
	vmulosh	27, 28, \_vz3
	vmulesh	28, 28, \_vz3
	xxmrglw	32+14, 32+13, 32+12
	xxmrghw	32+13, 32+13, 32+12
	xxmrglw	32+19, 32+18, 32+17
	xxmrghw	32+18, 32+18, 32+17
	xxmrglw	32+24, 32+23, 32+22
	xxmrghw	32+23, 32+23, 32+22
	xxmrglw	32+29, 32+28, 32+27
	xxmrghw	32+28, 32+28, 32+27

	# a * QINV
	vpkuwum 15, 13, 14
	vpkuwum 20, 18, 19
	vpkuwum 25, 23, 24
	vpkuwum 30, 28, 29

	vslh	16, 15, V_QI1
	vslh	21, 20, V_QI1
	vslh	26, 25, V_QI1
	vslh	31, 30, V_QI1
	vadduhm 16, 16, 15
	vadduhm 21, 21, 20
	vadduhm 26, 26, 25
	vadduhm 31, 31, 30
	vslh	16, 16, V_QI2
	vslh	21, 21, V_QI2
	vslh	26, 26, V_QI2
	vslh	31, 31, V_QI2
	vadduhm 16, 16, 15
	vadduhm 21, 21, 20
	vadduhm 26, 26, 25
	vadduhm 31, 31, 30
	vslh	16, 16, V_QI8
	vslh	21, 21, V_QI8
	vslh	26, 26, V_QI8
	vslh	31, 31, V_QI8
	vsubuhm  15, 15, 16
	vsubuhm 20, 20, 21
	vsubuhm 25, 25, 26
	vsubuhm 30, 30, 31

	vmulosh 12, 15, V_NMKQ
	vmulesh 15, 15, V_NMKQ
	vmulosh 17, 20, V_NMKQ
	vmulesh 20, 20, V_NMKQ
	vmulosh 22, 25, V_NMKQ
	vmulesh 25, 25, V_NMKQ
	vmulosh 27, 30, V_NMKQ
	vmulesh 30, 30, V_NMKQ
	xxmrglw 32+16, 32+15, 32+12
	xxmrghw 32+15, 32+15, 32+12
	xxmrglw 32+21, 32+20, 32+17
	xxmrghw 32+20, 32+20, 32+17
	xxmrglw 32+26, 32+25, 32+22
	xxmrghw 32+25, 32+25, 32+22
	xxmrglw 32+31, 32+30, 32+27
	xxmrghw 32+30, 32+30, 32+27

	vadduwm 16, 16, 14
	vadduwm 15, 15, 13
	vadduwm 21, 21, 19
	vadduwm 20, 20, 18
	vadduwm 26, 26, 24
	vadduwm 25, 25, 23
	vadduwm 31, 31, 29
	vadduwm 30, 30, 28
	vsraw 14, 16, V_16		# >> 16
	vsraw 13, 15, V_16		# >> 16
	vsraw 19, 21, V_16		# >> 16
	vsraw 18, 20, V_16		# >> 16
	vsraw 24, 26, V_16		# >> 16
	vsraw 23, 25, V_16		# >> 16
	vsraw 29, 31, V_16		# >> 16
	vsraw 28, 30, V_16		# >> 16

	lxvd2x	32+12, 3, 9	# r[j]
	lxvd2x	32+17, 3, 16	# r[j]
	lxvd2x	32+22, 3, 18	# r[j]
	lxvd2x	32+27, 3, 20	# r[j]
	xxpermdi 32+12, 32+12, 32+12, 2
	xxpermdi 32+17, 32+17, 32+17, 2
	xxpermdi 32+22, 32+22, 32+22, 2
	xxpermdi 32+27, 32+27, 32+27, 2

	vpkuwum 13, 13, 14
	vpkuwum 18, 18, 19
	vpkuwum 23, 23, 24
	vpkuwum 28, 28, 29

	vsubuhm 16, 12, 13		# r - t
	vadduhm 15, 13, 12		# r + t
	vsubuhm 21, 17, 18		# r - t
	vadduhm 20, 18, 17		# r + t
	vsubuhm 26, 22, 23		# r - t
	vadduhm 25, 23, 22		# r + t
	vsubuhm 31, 27, 28		# r - t
	vadduhm 30, 28, 27		# r + t
.endm

.macro Write_One
	stxvx 32+15, 3, 9
	stxvx 32+16, 3, 10
	stxvx 32+20, 3, 16
	stxvx 32+21, 3, 17
	stxvx 32+25, 3, 18
	stxvx 32+26, 3, 19
	stxvx 32+30, 3, 20
	stxvx 32+31, 3, 21
.endm

.macro Write_Two
	xxpermdi 32+17, 32+16, 32+15, 3
	xxpermdi 32+22, 32+21, 32+20, 3
	xxpermdi 32+27, 32+26, 32+25, 3
	xxpermdi 32+29, 32+31, 32+30, 3

	stxvx	32+17, 3, 9
	stxvx	32+22, 3, 16
	stxvx	32+27, 3, 18
	stxvx	32+29, 3, 20
.endm

.macro Write_Three
	xxmrglw	32+14, 32+16, 32+15
	xxmrghw	32+13, 32+16, 32+15
	xxpermdi 32+17, 32+13, 32+14, 3
	xxmrglw	32+19, 32+21, 32+20
	xxmrghw	32+18, 32+21, 32+20
	xxpermdi 32+22, 32+18, 32+19, 3
	xxmrglw	32+14, 32+26, 32+25
	xxmrghw	32+13, 32+26, 32+25
	xxpermdi 32+27, 32+13, 32+14, 3
	xxmrglw	32+24, 32+31, 32+30
	xxmrghw	32+23, 32+31, 32+30
	xxpermdi 32+29, 32+23, 32+24, 3
	stxvx	32+17, 3, 9
	stxvx	32+22, 3, 16
	stxvx	32+27, 3, 18
	stxvx	32+29, 3, 20
.endm

.macro Load_next_4zetas
	lxv	32+V_Z0, 0(14)
	lxv	32+V_Z1, 16(14)
	lxv	32+V_Z2, 32(14)
	lxv	32+V_Z3, 48(14)
	addi	14, 14, 64
.endm

#
# mlk_ntt_ppc(int16_t *r)
#
.global mlk_ntt_ppc
.align 4
mlk_ntt_ppc:
.localentry     mlk_ntt_ppc,.-mlk_ntt_ppc

	stdu	1, -352(1)
	mflr	0
	std	14, 56(1)
	std	15, 64(1)
	std	16, 72(1)
	std	17, 80(1)
	std	18, 88(1)
	std	19, 96(1)
	std	20, 104(1)
	std	21, 112(1)
	stxv	32+20, 128(1)
	stxv	32+21, 144(1)
	stxv	32+22, 160(1)
	stxv	32+23, 176(1)
	stxv	32+24, 192(1)
	stxv	32+25, 208(1)
	stxv	32+26, 224(1)
	stxv	32+27, 240(1)
	stxv	32+28, 256(1)
	stxv	32+29, 272(1)
	stxv	32+30, 288(1)
	stxv	32+31, 304(1)

	# get MLKEM_Q
	addis	8,2,.nmkq@toc@ha
	addi	8,8,.nmkq@toc@l
	lvx	V_NMKQ,0,8

	# zetas array
	addis	14,2,.K1@toc@ha
	addi	14,14,.K1@toc@l

	# 16
	vspltisw V_16, 8
	vadduwm	V_16, V_16, V_16

	vspltish V_QI1, 1
	vspltish V_QI2, 2
	vspltish V_QI8, 8

.align 4
__Len128:
	#
	# Compute coefficients of the NTT based on the following loop.
	#   for (len = 128; len ≥ 2; len =  len/2)
	#
	# 1. len = 128, start = 0
	#
	li	5, 0		# start
	li	4, 256		# len * 2
	lvx	V_ZETA, 0, 14
	addi	14, 14, 16

	MREDUCE_4X 5, 16, 16, V_ZETA, V_ZETA, V_ZETA, V_ZETA
	Write_One
	li	5, 64
	MREDUCE_4X 5, 16, 16, V_ZETA, V_ZETA, V_ZETA, V_ZETA
	Write_One
	li	5, 128
	MREDUCE_4X 5, 16, 16, V_ZETA, V_ZETA, V_ZETA, V_ZETA
	Write_One
	li	5, 192
	MREDUCE_4X 5, 16, 16, V_ZETA, V_ZETA, V_ZETA, V_ZETA
	Write_One

.align 4
__Len64:
	#
	# 2. len = 64, start = 0, 128
	# k += 2
	li	5, 0
	li	4, 128
	lvx	V_ZETA, 0, 14
	addi	14, 14, 16
	MREDUCE_4X 5, 16, 16, V_ZETA, V_ZETA, V_ZETA, V_ZETA
	Write_One
	li	5, 64
	MREDUCE_4X 5, 16, 16, V_ZETA, V_ZETA, V_ZETA, V_ZETA
	Write_One
	li	5, 256

	lvx	V_ZETA, 0, 14
	addi	14, 14, 16
	MREDUCE_4X 5, 16, 16, V_ZETA, V_ZETA, V_ZETA, V_ZETA
	Write_One
	li	5, 320
	MREDUCE_4X 5, 16, 16, V_ZETA, V_ZETA, V_ZETA, V_ZETA
	Write_One

.align 4
__Len32:
	#
	# 3. len = 32, start = 0, 64, 128, 192
	# k += 4
	li	5, 0
	li	4, 64
	lvx	V_ZETA, 0, 14
	addi	14, 14, 16
	MREDUCE_4X 5, 16, 16, V_ZETA, V_ZETA, V_ZETA, V_ZETA
	Write_One
	#li	5, 64
	li	5, 128

	lvx	V_ZETA, 0, 14
	addi	14, 14, 16
	MREDUCE_4X 5, 16, 16, V_ZETA, V_ZETA, V_ZETA, V_ZETA
	Write_One
	#li	5, 128
	li	5, 256

	lvx	V_ZETA, 0, 14
	addi	14, 14, 16
	MREDUCE_4X 5, 16, 16, V_ZETA, V_ZETA, V_ZETA, V_ZETA
	Write_One
	#li	5, 192
	li	5, 384

	lvx	V_ZETA, 0, 14
	addi	14, 14, 16
	MREDUCE_4X 5, 16, 16, V_ZETA, V_ZETA, V_ZETA, V_ZETA
	Write_One

.align 4
__Len16:
	#
	# 4. len = 16, start = 0, 32, 64,,...160, 192, 224
	# k += 8
	li	5, 0
	li	4, 32
	Load_next_4zetas
	MREDUCE_4X 5, 64, 64, V_Z0, V_Z1, V_Z2, V_Z3
	Write_One
	li	5, 16
	MREDUCE_4X 5, 64, 64, V_Z0, V_Z1, V_Z2, V_Z3
	Write_One

	Load_next_4zetas
	li	5, 256
	MREDUCE_4X 5, 64, 64, V_Z0, V_Z1, V_Z2, V_Z3
	Write_One
	li	5, 272
	MREDUCE_4X  5, 64, 64, V_Z0, V_Z1, V_Z2, V_Z3
	Write_One

.align 4
__Len8:
	#
	# 5. len = 8, start = 0, 16, 32, 48,...208, 224, 240 
	# k += 16
	li	5, 0
	li	4, 16
	Load_next_4zetas
	MREDUCE_4X 5, 32, 32, V_Z0, V_Z1, V_Z2, V_Z3
	Write_One
	li	5, 128

	Load_next_4zetas
	MREDUCE_4X 5, 32, 32, V_Z0, V_Z1, V_Z2, V_Z3
	Write_One
	li	5, 256

	Load_next_4zetas
	MREDUCE_4X 5, 32, 32, V_Z0, V_Z1, V_Z2, V_Z3
	Write_One
	li	5, 384

	Load_next_4zetas
	MREDUCE_4X 5, 32, 32, V_Z0, V_Z1, V_Z2, V_Z3
	Write_One

	#
	# 6. len = 4, start = 0, 8, 16, 24,...232, 240, 248 
	# k += 32
	li	15, 4			# loops
	mtctr	15
	li	5, 0
	li	4, 8
.align 4
__Len4:
	Load_next_4zetas
	MREDUCE_4X 5, 16, 16, V_Z0, V_Z1, V_Z2, V_Z3
	Write_Two
	addi	5, 5, 64

	Load_next_4zetas
	MREDUCE_4X 5, 16, 16, V_Z0, V_Z1, V_Z2, V_Z3
	Write_Two
	addi	5, 5, 64

	bdnz	__Len4

	#
	# 7. len = 2, start = 0, 4, 8, 12,...244, 248, 252 
	# k += 64
	# Update zetas vectors, each vector has 2 zetas
	addis	14,2,.K64@toc@ha
	addi	14,14,.K64@toc@l

	li	15, 4
	mtctr	15
	li	5, 0
	li	4, 4
.align 4
__Len2:
	Load_next_4zetas
	MREDUCE_4X 5, 16, 16, V_Z0, V_Z1, V_Z2, V_Z3
	Write_Three
	addi	5, 5, 64

	Load_next_4zetas
	MREDUCE_4X 5, 16, 16, V_Z0, V_Z1, V_Z2, V_Z3
	Write_Three
	addi	5, 5, 64

	bdnz	__Len2

__ntt_out:
	lxv	32+20, 128(1)
	lxv	32+21, 144(1)
	lxv	32+22, 160(1)
	lxv	32+23, 176(1)
	lxv	32+24, 192(1)
	lxv	32+25, 208(1)
	lxv	32+26, 224(1)
	lxv	32+27, 240(1)
	lxv	32+28, 256(1)
	lxv	32+29, 272(1)
	lxv	32+30, 288(1)
	lxv	32+31, 304(1)
	ld	14, 56(1)
	ld	15, 64(1)
	ld	16, 72(1)
	ld	16, 72(1)
	ld	17, 80(1)
	ld	18, 88(1)
	ld	19, 96(1)
	ld	20, 104(1)
	ld	21, 112(1)

	mtlr	0
	addi    1, 1, 352
	blr
.size     mlk_ntt_ppc,.-mlk_ntt_ppc

.data
.align 4
# -MLKEM_Q
.nmkq:
.short  -3329, -3329, -3329, -3329, -3329, -3329, -3329, -3329

# zetas
.K1:
.short -758, -758, -758, -758, -758, -758, -758, -758
.short -359, -359, -359, -359, -359, -359, -359, -359
.short -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517
.short 1493, 1493, 1493, 1493, 1493, 1493, 1493, 1493
.short 1422, 1422, 1422, 1422, 1422, 1422, 1422, 1422
.short 287, 287, 287, 287, 287, 287, 287, 287
.short 202, 202, 202, 202, 202, 202, 202, 202
.short -171, -171, -171, -171, -171, -171, -171, -171
.short 622, 622, 622, 622, 622, 622, 622, 622
.short 1577, 1577, 1577, 1577, 1577, 1577, 1577, 1577
.short 182, 182, 182, 182, 182, 182, 182, 182
.short 962, 962, 962, 962, 962, 962, 962, 962
.short -1202, -1202, -1202, -1202, -1202, -1202, -1202, -1202
.short -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474
.short 1468, 1468, 1468, 1468, 1468, 1468, 1468, 1468
.short 573, 573, 573, 573, 573, 573, 573, 573
.short -1325, -1325, -1325, -1325, -1325, -1325, -1325, -1325
.short 264, 264, 264, 264, 264, 264, 264, 264
.short 383, 383, 383, 383, 383, 383, 383, 383
.short -829, -829, -829, -829, -829, -829, -829, -829
.short 1458, 1458, 1458, 1458, 1458, 1458, 1458, 1458
.short -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602
.short -130, -130, -130, -130, -130, -130, -130, -130
.short -681, -681, -681, -681, -681, -681, -681, -681
.short 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017
.short 732, 732, 732, 732, 732, 732, 732, 732
.short 608, 608, 608, 608, 608, 608, 608, 608
.short -1542, -1542, -1542, -1542, -1542, -1542, -1542, -1542
.short 411, 411, 411, 411, 411, 411, 411, 411
.short -205, -205, -205, -205, -205, -205, -205, -205
.short -1571, -1571, -1571, -1571, -1571, -1571, -1571, -1571
.short 1223, 1223, 1223, 1223, 1223, 1223, 1223, 1223
.short 652, 652, 652, 652, 652, 652, 652, 652
.short -552, -552, -552, -552, -552, -552, -552, -552
.short 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015
.short -1293, -1293, -1293, -1293, -1293, -1293, -1293, -1293
.short 1491, 1491, 1491, 1491, 1491, 1491, 1491, 1491
.short -282, -282, -282, -282, -282, -282, -282, -282
.short -1544, -1544, -1544, -1544, -1544, -1544, -1544, -1544
.short 516, 516, 516, 516, 516, 516, 516, 516
.short -8, -8, -8, -8, -8, -8, -8, -8
.short -320, -320, -320, -320, -320, -320, -320, -320
.short -666, -666, -666, -666, -666, -666, -666, -666
.short -1618, -1618, -1618, -1618, -1618, -1618, -1618, -1618
.short -1162, -1162, -1162, -1162, -1162, -1162, -1162, -1162
.short 126, 126, 126, 126, 126, 126, 126, 126
.short 1469, 1469, 1469, 1469, 1469, 1469, 1469, 1469
.short -853, -853, -853, -853, -853, -853, -853, -853
.short -90, -90, -90, -90, -90, -90, -90, -90
.short -271, -271, -271, -271, -271, -271, -271, -271
.short 830, 830, 830, 830, 830, 830, 830, 830
.short 107, 107, 107, 107, 107, 107, 107, 107
.short -1421, -1421, -1421, -1421, -1421, -1421, -1421, -1421
.short -247, -247, -247, -247, -247, -247, -247, -247
.short -951, -951, -951, -951, -951, -951, -951, -951
.short -398, -398, -398, -398, -398, -398, -398, -398
.short 961, 961, 961, 961, 961, 961, 961, 961
.short -1508, -1508, -1508, -1508, -1508, -1508, -1508, -1508
.short -725, -725, -725, -725, -725, -725, -725, -725
.short 448, 448, 448, 448, 448, 448, 448, 448
.short -1065, -1065, -1065, -1065, -1065, -1065, -1065, -1065
.short 677, 677, 677, 677, 677, 677, 677, 677
.short -1275, -1275, -1275, -1275, -1275, -1275, -1275, -1275
.K64:
.short -1103, -1103, -1103, -1103, 430, 430, 430, 430
.short 555, 555, 555, 555, 843, 843, 843, 843
.short -1251, -1251, -1251, -1251, 871, 871, 871, 871
.short 1550, 1550, 1550, 1550, 105, 105, 105, 105
.short 422, 422, 422, 422, 587, 587, 587, 587
.short 177, 177, 177, 177, -235, -235, -235, -235
.short -291, -291, -291, -291, -460, -460, -460, -460
.short 1574, 1574, 1574, 1574, 1653, 1653, 1653, 1653
.short -246, -246, -246, -246, 778, 778, 778, 778
.short 1159, 1159, 1159, 1159, -147, -147, -147, -147
.short -777, -777, -777, -777, 1483, 1483, 1483, 1483
.short -602, -602, -602, -602, 1119, 1119, 1119, 1119
.short -1590, -1590, -1590, -1590, 644, 644, 644, 644
.short -872, -872, -872, -872, 349, 349, 349, 349
.short 418, 418, 418, 418, 329, 329, 329, 329
.short -156, -156, -156, -156, -75, -75, -75, -75
.short 817, 817, 817, 817, 1097, 1097, 1097, 1097
.short 603, 603, 603, 603, 610, 610, 610, 610
.short 1322, 1322, 1322, 1322, -1285, -1285, -1285, -1285
.short -1465, -1465, -1465, -1465, 384, 384, 384, 384
.short -1215, -1215, -1215, -1215, -136, -136, -136, -136
.short 1218, 1218, 1218, 1218, -1335, -1335, -1335, -1335
.short -874, -874, -874, -874, 220, 220, 220, 220
.short -1187, -1187, -1187, -1187, -1659, -1659, -1659, -1659
.short -1185, -1185, -1185, -1185, -1530, -1530, -1530, -1530
.short -1278, -1278, -1278, -1278, 794, 794, 794, 794
.short -1510, -1510, -1510, -1510, -854, -854, -854, -854
.short -870, -870, -870, -870, 478, 478, 478, 478
.short -108, -108, -108, -108, -308, -308, -308, -308
.short 996, 996, 996, 996, 991, 991, 991, 991
.short 958, 958, 958, 958, -1460, -1460, -1460, -1460
.short 1522, 1522, 1522, 1522, 1628, 1628, 1628, 1628
