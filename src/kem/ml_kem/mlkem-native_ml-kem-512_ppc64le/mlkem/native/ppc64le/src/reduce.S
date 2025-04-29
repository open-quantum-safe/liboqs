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

#
# poly_reduce: Applies Barrett reduction to all coefficients of a polynomial
#              for details of the Barrett reduction
#
# Arguments: *r: pointer to input/output polynomial
#

# Barrett reduce constatnts
#define V20159  0
#define V_25	1
#define V_26    2
#define V_MKQ   3

.machine "any"
.text

.macro BREDUCE_4X _v0 _v1 _v2 _v3
	lxvd2x	32+8, 0, 3
	lxvd2x	32+12, 14, 3
	lxvd2x	32+16, 15, 3
	lxvd2x	32+20, 16, 3
	addi	3, 3, 64
	vmulosh	6, 8, V20159
	vmulesh	5, 8, V20159
	vmulosh	11, 12, V20159
	vmulesh	10, 12, V20159
	vmulosh	15, 16, V20159
	vmulesh	14, 16, V20159
	vmulosh	19, 20, V20159
	vmulesh	18, 20, V20159
	xxmrglw	32+4, 32+5, 32+6
	xxmrghw	32+5, 32+5, 32+6
	xxmrglw	32+9, 32+10, 32+11
	xxmrghw	32+10, 32+10, 32+11
	xxmrglw	32+13, 32+14, 32+15
	xxmrghw	32+14, 32+14, 32+15
	xxmrglw	32+17, 32+18, 32+19
	xxmrghw	32+18, 32+18, 32+19
	vadduwm	4, 4, V_25
	vadduwm	5, 5, V_25
	vadduwm	9, 9, V_25
	vadduwm	10, 10, V_25
	vadduwm	13, 13, V_25
	vadduwm	14, 14, V_25
	vadduwm	17, 17, V_25
	vadduwm	18, 18, V_25
	vsraw	4, 4, V_26
	vsraw	5, 5, V_26
	vsraw	9, 9, V_26
	vsraw	10, 10, V_26
	vsraw	13, 13, V_26
	vsraw	14, 14, V_26
	vsraw	17, 17, V_26
	vsraw	18, 18, V_26
	vpkuwum	4, 5, 4
	vsubuhm	4, 7, 4
	vpkuwum	9, 10, 9
	vsubuhm	9, 7, 9
	vpkuwum	13, 14, 13
	vsubuhm	13, 7, 13
	vpkuwum	17, 18, 17
	vsubuhm	17, 7, 17
	vmladduhm \_v0, 4, V_MKQ, 8
	vmladduhm \_v1, 9, V_MKQ, 12
	vmladduhm \_v2, 13, V_MKQ, 16
	vmladduhm \_v3, 17, V_MKQ, 20
.endm

.macro Write_8X
	stxvd2x 32+21, 4, 3
	stxvd2x 32+22, 5, 3
	stxvd2x 32+23, 6, 3
	stxvd2x 32+24, 7, 3
	stxvd2x 32+4, 8, 3
	stxvd2x 32+9, 9, 3
	stxvd2x 32+13, 10, 3
	stxvd2x 32+17, 11, 3
.endm

#
# Conditional addition to get unsigned canonical representative
#
.macro To_unsigned_16
	lxv	32+12, 0(3)
	lxv	32+13, 16(3)
	lxv	32+14, 32(3)
	lxv	32+15, 48(3)
	addi	3, 3, 64
	vsrh	1, 12, 10
	vsrh	0, 13, 10
	vsrh	3, 14, 10
	vsrh	2, 15, 10
	vadduhm 7, 12, 11
	vadduhm 8, 13, 11
	vadduhm 5, 14, 11
	vadduhm 6, 15, 11
	vcmpequh 1, 1, 9
	vcmpequh 0, 0, 9
	vcmpequh 3, 3, 9
	vcmpequh 2, 2, 9
	xxsel	32+1, 32+7,32+12, 32+1
	xxsel	32+0, 32+8,32+13, 32+0
	xxsel	32+3, 32+5,32+14, 32+3
	xxsel	32+2, 32+6,32+15, 32+2
	stxv	32+3, -32(3)
	stxv	32+2, -16(3)
	stxv	32+1, -64(3)
	stxv	32+0, -48(3)
.endm

.align 4
.globl mlk_reduce_ppc
mlk_reduce_ppc:
.localentry     mlk_reduce_ppc,.-mlk_reduce_ppc
	stdu	1, -224(1)
	mflr	0
	std	14, 96(1)
	std	15, 104(1)
	std	16, 112(1)
	stxv	32+20, 128(1)
	stxv	32+21, 144(1)
	stxv	32+22, 160(1)
	stxv	32+23, 176(1)
	stxv	32+24, 192(1)

	addis	8,2,.mkq@toc@ha
	addi	8,8,.mkq@toc@l
	addis	9,2,.C20159@toc@ha
	addi	9,9,.C20159@toc@l
	addis	10,2,.C25@toc@ha
	addi	10,10,.C25@toc@l

	vxor	7, 7, 7

	lxv	32+V_MKQ, 0(8)
	lxv	32+V20159, 0(9)
	lxv	32+V_25, 0(10)

	li	4, -128
	li	5, -112
	li	6, -96
	li	7, -80
	li	8, -64
	li	9, -48
	li	10, -32
	li	11, -16

	li	14, 16
	li	15, 32
	li	16, 48

	vspltisw V_26, 13
	vadduwm	V_26, V_26, V_26

	BREDUCE_4X 21, 22, 23, 24
	BREDUCE_4X 4, 9, 13, 17
	Write_8X

	BREDUCE_4X 21, 22, 23, 24
	BREDUCE_4X 4, 9, 13, 17
	Write_8X

	BREDUCE_4X 21, 22, 23, 24
	BREDUCE_4X 4, 9, 13, 17
	Write_8X

	BREDUCE_4X 21, 22, 23, 24
	BREDUCE_4X 4, 9, 13, 17
	Write_8X

	#
	# To unsigned canonical
	#
.align 4
	addi	3, 3, -512
	xxspltib 32+9 ,0
	vspltish 10, 15
	vmr	11, V_MKQ

	To_unsigned_16
	To_unsigned_16
	To_unsigned_16
	To_unsigned_16
	To_unsigned_16
	To_unsigned_16
	To_unsigned_16
	To_unsigned_16

	ld	14, 96(1)
	ld	15, 104(1)
	ld	16, 112(1)
	lxv	32+20, 128(1)
	lxv	32+21, 144(1)
	lxv	32+22, 160(1)
	lxv	32+23, 176(1)
	lxv	32+24, 192(1)
	mtlr	0
	addi	1, 1, 224
	blr
.size   mlk_reduce_ppc,.-mlk_reduce_ppc

.align 4
.data
# MLKEM_Q
.mkq:
.short 3329, 3329, 3329, 3329, 3329, 3329, 3329, 3329

.C20159:
.short  20159, 20159, 20159, 20159, 20159, 20159, 20159, 20159

# 0x2000000
.C25:
.long   33554432, 33554432, 33554432, 33554432
