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

# Poly_tomont: Inplace conversion of all coefficients of a polynomial
#              from normal domain to Montgomery domain
#
# Arguments:*r: pointer to input/output polynomial
#

#define V1353	0
#define V_16    1
#define V_QI1   2
#define V_QI2   3
#define V_QI8   4
#define V_NMKQ  5

.machine "any"
.text

#
# montgomery_reduce
# t = a * QINV
# t = (a - (int32_t)t*_MLKEM_Q) >> 16
#
#-----------------------------------
# MREDUCE_4X(_v0, _v1, _v2, _v3)
#
.macro MREDUCE_4X _v0 _v1 _v2 _v3
	lxvd2x	32+13, 0, 3
	addi	3, 3, 16
	lxvd2x	32+18, 0, 3
	addi	3, 3, 16
	lxvd2x	32+23, 0, 3
	addi	3, 3, 16
	lxvd2x	32+7, 0, 3
	addi	3, 3, 16

	vmulosh	12, 13, V1353
	vmulesh	13, 13, V1353
	vmulosh	17, 18, V1353
	vmulesh	18, 18, V1353
	vmulosh	22, 23, V1353
	vmulesh	23, 23, V1353
	vmulosh	6, 7, V1353
	vmulesh	7, 7, V1353
	xxmrglw	32+14, 32+13, 32+12
	xxmrghw	32+13, 32+13, 32+12
	xxmrglw	32+19, 32+18, 32+17
	xxmrghw	32+18, 32+18, 32+17
	xxmrglw	32+24, 32+23, 32+22
	xxmrghw	32+23, 32+23, 32+22
	xxmrglw	32+8, 32+7, 32+6
	xxmrghw	32+7, 32+7, 32+6

	# a * QINV
	vpkuwum 15, 13, 14
	vpkuwum 20, 18, 19
	vpkuwum 25, 23, 24
	vpkuwum 9, 7, 8

	vslh	16, 15, V_QI1
	vslh	21, 20, V_QI1
	vslh	26, 25, V_QI1
	vslh	10, 9, V_QI1
	vadduhm 16, 16, 15
	vadduhm 21, 21, 20
	vadduhm 26, 26, 25
	vadduhm 10, 10, 9
	vslh	16, 16, V_QI2
	vslh	21, 21, V_QI2
	vslh	26, 26, V_QI2
	vslh	10, 10, V_QI2
	vadduhm 16, 16, 15
	vadduhm 21, 21, 20
	vadduhm 26, 26, 25
	vadduhm 10, 10, 9
	vslh	16, 16, V_QI8
	vslh	21, 21, V_QI8
	vslh	26, 26, V_QI8
	vslh	10, 10, V_QI8
	vsubuhm 15, 15, 16
	vsubuhm 20, 20, 21
	vsubuhm 25, 25, 26
	vsubuhm 9, 9, 10

	vmulosh 12, 15, V_NMKQ
	vmulesh 15, 15, V_NMKQ
	vmulosh 17, 20, V_NMKQ
	vmulesh 20, 20, V_NMKQ
	vmulosh 22, 25, V_NMKQ
	vmulesh 25, 25, V_NMKQ
	vmulosh 6, 9, V_NMKQ
	vmulesh 9, 9, V_NMKQ
	xxmrglw 32+16, 32+15, 32+12
	xxmrghw 32+15, 32+15, 32+12
	xxmrglw 32+21, 32+20, 32+17
	xxmrghw 32+20, 32+20, 32+17
	xxmrglw 32+26, 32+25, 32+22
	xxmrghw 32+25, 32+25, 32+22
	xxmrglw 32+10, 32+9, 32+6
	xxmrghw 32+9, 32+9, 32+6

	vadduwm 16, 16, 14
	vadduwm 15, 15, 13
	vadduwm 21, 21, 19
	vadduwm 20, 20, 18
	vadduwm 26, 26, 24
	vadduwm 25, 25, 23
	vadduwm 10, 10, 8
	vadduwm 9, 9, 7
	vsraw 14, 16, V_16		# >> 16
	vsraw 13, 15, V_16		# >> 16
	vsraw 19, 21, V_16		# >> 16
	vsraw 18, 20, V_16		# >> 16
	vsraw 24, 26, V_16		# >> 16
	vsraw 23, 25, V_16		# >> 16
	vsraw 8, 10, V_16		# >> 16
	vsraw 7, 9, V_16		# >> 16

	vpkuwum \_v0, 13, 14
	vpkuwum \_v1, 18, 19
	vpkuwum \_v2, 23, 24
	vpkuwum \_v3, 7, 8
.endm

.macro Write_8X
	stxvd2x	32+27, 4, 3
	stxvd2x	32+28, 5, 3
	stxvd2x	32+29, 6, 3
	stxvd2x	32+30, 7, 3
	stxvd2x	32+13, 8, 3
	stxvd2x	32+18, 9, 3
	stxvd2x	32+23, 10, 3
	stxvd2x	32+7, 11, 3
.endm

.align 4
.globl mlk_poly_tomont_ppc
mlk_poly_tomont_ppc:
.localentry	mlk_poly_tomont_ppc,.-mlk_poly_tomont_ppc
	stdu	1, -320(1)
	mflr	0

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

	addis	9,2,.nmkq@toc@ha
	addi	9,9,.nmkq@toc@l
	addis	10,2,.C1353@toc@ha
	addi	10,10,.C1353@toc@l

	vspltish V_QI1,1
	vspltish V_QI2,2
	vspltish V_QI8,8

	lxv	32+V_NMKQ,0(9)
	lxv	32+V1353,0(10)

	vspltisw V_16, 8
	vadduwm	V_16, V_16, V_16

	li	4, -128
	li	5, -112
	li	6, -96
	li	7, -80
	li	8, -64
	li	9, -48
	li	10, -32
	li	11, -16

	MREDUCE_4X 27, 28, 29, 30
	MREDUCE_4X 13, 18, 23, 7
	Write_8X

	MREDUCE_4X 27, 28, 29, 30
	MREDUCE_4X 13, 18, 23, 7
	Write_8X

	MREDUCE_4X 27, 28, 29, 30
	MREDUCE_4X 13, 18, 23, 7
	Write_8X

	MREDUCE_4X 27, 28, 29, 30
	MREDUCE_4X 13, 18, 23, 7
	Write_8X

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
	mtlr	0
	addi	1, 1, 320
	blr
.size	mlk_poly_tomont_ppc,.-mlk_poly_tomont_ppc

.data
.align 4
# -MLKEM_Q
.nmkq:
.short -3329, -3329, -3329, -3329, -3329, -3329, -3329, -3329

.C1353:
.short  1353, 1353, 1353, 1353, 1353, 1353, 1353, 1353

