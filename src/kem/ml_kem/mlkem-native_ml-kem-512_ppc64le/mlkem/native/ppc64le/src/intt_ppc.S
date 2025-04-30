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

.machine "any"
.text

# Barrett reduce constatnts
#define V20159  0
#define V_25	1
#define V_26    2
#define V_MKQ   3

# Montgomery reduce constatnts
#define V_16    1
#define V_QI1   2
#define V_QI2   3
#define V_QI8   4
#define V_NMKQ  5
#define V_Z0    7
#define V_Z1    8
#define V_Z2    9
#define V_Z3    10
#define V_ZETA  10
#define V1441	10

.macro Load_4Coeffs start next step
	mr	9, \start	# j
	add	10, 4, 9	# J + len*2
	addi	16, 9, \next
	addi	17, 10, \step
	addi	18, 16, \next
	addi	19, 17, \step
	addi	20, 18, \next
	addi	21, 19, \step
	lxvd2x	32+8, 3, 10	# r[j+len]
	lxvd2x	32+12, 3, 17	# r[j+len]
	lxvd2x	32+16, 3, 19	# r[j+len]
	lxvd2x	32+20, 3, 21	# r[j+len]
	xxpermdi 32+8, 32+8, 32+8, 2
	xxpermdi 32+12, 32+12, 32+12, 2
	xxpermdi 32+16, 32+16, 32+16, 2
	xxpermdi 32+20, 32+20, 32+20, 2

	lxvd2x	32+21, 3, 9
	lxvd2x	32+22, 3, 16
	lxvd2x	32+23, 3, 18
	lxvd2x	32+24, 3, 20
	xxpermdi 32+21, 32+21, 32+21, 2
	xxpermdi 32+22, 32+22, 32+22, 2
	xxpermdi 32+23, 32+23, 32+23, 2
	xxpermdi 32+24, 32+24, 32+24, 2

	vsubuhm 25, 8, 21		# r[j+len] - t
	vsubuhm 26, 12, 22		# r[j+len] - t
	vsubuhm 30, 16, 23		# r[j+len] - t
	vsubuhm 31, 20, 24		# r[j+len] - t
	vadduhm 8, 8, 21		# r[j+len] + t
	vadduhm 12, 12, 22		# r[j+len] + t
	vadduhm 16, 16, 23		# r[j+len] + t
	vadduhm 20, 20, 24		# r[j+len] + t
.endm

.macro BREDUCE_4X _v0 _v1 _v2 _v3
	vxor	7, 7, 7
	xxlor	32+3, 6, 6	# V_MKQ
	xxlor	32+1, 7, 7	# V_25
	xxlor	32+2, 8, 8	# V_26
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

#-----------------------------------
# MREDUCE_4X(len, start, _vz0, _vz1, _vz2, _vz3)
#
.macro MREDUCE_4X _vz0 _vz1 _vz2 _vz3 _vo0 _vo1 _vo2 _vo3
	vmulosh	12, 25, \_vz0
	vmulesh	13, 25, \_vz0
	vmulosh	17, 26, \_vz1
	vmulesh	18, 26, \_vz1
	vmulosh	22, 30, \_vz2
	vmulesh	23, 30, \_vz2
	vmulosh	27, 31, \_vz3
	vmulesh	28, 31, \_vz3
	xxmrglw	32+14, 32+13, 32+12
	xxmrghw	32+13, 32+13, 32+12
	xxmrglw	32+19, 32+18, 32+17
	xxmrghw	32+18, 32+18, 32+17
	xxmrglw	32+24, 32+23, 32+22
	xxmrghw	32+23, 32+23, 32+22
	xxmrglw	32+29, 32+28, 32+27
	xxmrghw	32+28, 32+28, 32+27

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

	vpkuwum \_vo0, 13, 14
	vpkuwum \_vo1, 18, 19
	vpkuwum \_vo2, 23, 24
	vpkuwum \_vo3, 28, 29
.endm

.macro Set_mont_consts
	xxlor	32+5, 0, 0	# V_NMKQ
	xxlor	32+1, 1, 1	# V_16
	xxlor	32+2, 2, 2	# V_QI1
	xxlor	32+3, 3, 3	# V_QI2
	xxlor	32+4, 4, 4	# V_QI8
.endm

.macro Load_next_4zetas
	lxv	32+V_Z0, 0(14)
	lxv	32+V_Z1, 16(14)
	lxv	32+V_Z2, 32(14)
	lxv	32+V_Z3, 48(14)
	addi	14, 14, 64
.endm

.macro Write_B4C _vs0 _vs1 _vs2 _vs3
	stxvx	\_vs0, 3, 9
	stxvx	\_vs1, 3, 16
	stxvx	\_vs2, 3, 18
	stxvx	\_vs3, 3, 20
.endm

.macro Write_M4C _vs0 _vs1 _vs2 _vs3
	stxvx	\_vs0, 3, 10
	stxvx	\_vs1, 3, 17
	stxvx	\_vs2, 3, 19
	stxvx	\_vs3, 3, 21
.endm

.macro Reload_4coeffs
	lxv	32+25, 0(3)
	lxv	32+26, 16(3)
	lxv	32+30, 32(3)
	lxv	32+31, 48(3)
	addi	3, 3, 64
.endm

.macro MWrite_8X _vs0 _vs1 _vs2 _vs3 _vs4 _vs5 _vs6 _vs7
	stxv	\_vs0, -128(3)
	stxv	\_vs1, -112(3)
	stxv	\_vs2, -96(3)
	stxv	\_vs3, -80(3)
	stxv	\_vs4, -64(3)
	stxv	\_vs5, -48(3)
	stxv	\_vs6, -32(3)
	stxv	\_vs7, -16(3)
.endm

.macro Write_Len2_4C _vs0 _vs1 _vs2 _vs3
        xxmrglw 32+12, \_vs0, 10
        xxmrghw 32+11, \_vs0, 10
        xxpermdi 10, 32+12, 32+11, 3
        xxmrglw 32+16, \_vs1, 11
        xxmrghw 32+15, \_vs1, 11
        xxpermdi 11, 32+16, 32+15, 3
        xxmrglw 32+12, \_vs2, 12
        xxmrghw 32+11, \_vs2, 12
        xxpermdi 12, 32+12, 32+11, 3
        xxmrglw 32+16, \_vs3, 13
        xxmrghw 32+15, \_vs3, 13
        xxpermdi 13, 32+16, 32+15, 3
        stxvd2x   10, 3, 9
        stxvd2x   11, 3, 16
        stxvd2x   12, 3, 18
        stxvd2x   13, 3, 20
.endm

.macro Write_Len4_4C _vs0 _vs1 _vs2 _vs3
	xxpermdi 10, 10, \_vs0, 3
	xxpermdi 11, 11, \_vs1, 3
	xxpermdi 12, 12, \_vs2, 3
	xxpermdi 13, 13, \_vs3, 3
        stxvd2x   10, 3, 9
        stxvd2x   11, 3, 16
        stxvd2x   12, 3, 18
        stxvd2x   13, 3, 20
.endm

# intt
# t = r[j];
# r[j] = barrett_reduce(t + r[j + len]);
# r[j + len] = r[j + len] - t;
# r[j + len] = fqmul(zeta, r[j + len]);

#
# mlk_intt_ppc(r)
#
.global mlk_intt_ppc
.align 4
mlk_intt_ppc:
.localentry     mlk_intt_ppc,.-mlk_intt_ppc

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

	# init vectors and constants
	# Setup for Montgomery reduce
	addis	8,2,.nmkq@toc@ha
	addi	8,8,.nmkq@toc@l
	lxv	0, 0(8)

	# 16
	#xxspltiw 1, 16		# for power9 and above
	vspltisw 1, 8
	vadduwm  1, 1, 1

	vspltish V_QI1, 1		# *QINV
	vspltish V_QI2, 2		# *QINV
	vspltish V_QI8, 8		# *QINV
	xxlor	2, 32+2, 32+2
	xxlor	3, 32+3, 32+3
	xxlor	4, 32+4, 32+4

	# Setup for Barrett reduce
	addis	8,2,.mkq@toc@ha
	addi	8,8,.mkq@toc@l
	addis	9,2,.C20159@toc@ha
	addi	9,9,.C20159@toc@l
	addis	10,2,.C25@toc@ha
	addi	10,10,.C25@toc@l

	lxv	6, 0(8)		# V_MKQ
	lxv	32+0, 0(9)	# V20159
	lxv	7, 0(10)	# V_25

	#xxspltiw 8, 26		# for power9 and above
	vspltisw 8, 13
	vadduwm  8, 8, 8

	# zetas array
	#addis	14,2,.izeta63@toc@ha
	#addi	14,14,.izeta63@toc@l

.align 4
__Len2:
	#
	# 1. len = 2, start = 0, 4, 8, 12,...244, 248, 252 
	# Update zetas vectors, each vector has 2 zetas
	addis	14,2,.izeta127@toc@ha
	addi	14,14,.izeta127@toc@l
	li	4, 4
	li	15, 4
	mtctr	15
	li	5, 0
__Loop2:
	Load_4Coeffs 5, 16, 16
	BREDUCE_4X 4, 9, 13, 17
	xxlor	10, 32+4, 32+4
	xxlor	11, 32+9, 32+9
	xxlor	12, 32+13, 32+13
	xxlor	13, 32+17, 32+17
	Set_mont_consts
	Load_next_4zetas
	MREDUCE_4X V_Z0, V_Z1, V_Z2, V_Z3, 13, 18, 23, 28
	Write_Len2_4C 32+13, 32+18, 32+23, 32+28

	addi	5, 5, 64

	Load_4Coeffs 5, 16, 16
	BREDUCE_4X 4, 9, 13, 17
	xxlor	10, 32+4, 32+4
	xxlor	11, 32+9, 32+9
	xxlor	12, 32+13, 32+13
	xxlor	13, 32+17, 32+17
	Set_mont_consts
	Load_next_4zetas
	MREDUCE_4X V_Z0, V_Z1, V_Z2, V_Z3, 13, 18, 23, 28
	Write_Len2_4C 32+13, 32+18, 32+23, 32+28
	addi	5, 5, 64
	bdnz	__Loop2

.align 4
__Len4:
	#
	# 2. len = 4, start = 0, 8, 16, 24,...232, 240, 248 
	addis	14,2,.izeta63@toc@ha
	addi	14,14,.izeta63@toc@l
	li	5, 0
	li	4, 8
	li	15, 4                   # loops
	mtctr	15
__Loop4:
	Load_4Coeffs 5, 16, 16
	BREDUCE_4X 4, 9, 13, 17
	xxlor	10, 32+4, 32+4
	xxlor	11, 32+9, 32+9
	xxlor	12, 32+13, 32+13
	xxlor	13, 32+17, 32+17
	Set_mont_consts
	Load_next_4zetas
	MREDUCE_4X V_Z0, V_Z1, V_Z2, V_Z3, 13, 18, 23, 28
	Write_Len4_4C 32+13, 32+18, 32+23, 32+28
	addi	5, 5, 64

	Load_4Coeffs 5, 16, 16
	BREDUCE_4X 4, 9, 13, 17
	xxlor	10, 32+4, 32+4
	xxlor	11, 32+9, 32+9
	xxlor	12, 32+13, 32+13
	xxlor	13, 32+17, 32+17
	Set_mont_consts
	Load_next_4zetas
	MREDUCE_4X V_Z0, V_Z1, V_Z2, V_Z3, 13, 18, 23, 28
	Write_Len4_4C 32+13, 32+18, 32+23, 32+28
	addi	5, 5, 64
	bdnz	__Loop4

.align 4
__Len8:
	# 3. len = 8, start = 0, 16, 32, 48,...208, 224, 240 
	#addi	14, 14, 512
	li	4, 16
	li	5, 0

	Load_4Coeffs 5, 32, 32
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	Load_next_4zetas
	MREDUCE_4X V_Z0, V_Z1, V_Z2, V_Z3, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28
	li	5, 128

	Load_4Coeffs 5, 32, 32
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	Load_next_4zetas
	MREDUCE_4X V_Z0, V_Z1, V_Z2, V_Z3, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28
	li	5, 256

	Load_4Coeffs 5, 32, 32
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	Load_next_4zetas
	MREDUCE_4X V_Z0, V_Z1, V_Z2, V_Z3, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28
	li	5, 384

	Load_4Coeffs 5, 32, 32
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	Load_next_4zetas
	MREDUCE_4X V_Z0, V_Z1, V_Z2, V_Z3, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28

.align 4
__Len16:
	#
	# 4. len = 16, start = 0, 32, 64,,...160, 192, 224
	#addi	14, 14, 768
	li	5, 0
	li	4, 32

	Load_4Coeffs 5, 64, 64
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	Load_next_4zetas
	MREDUCE_4X V_Z0, V_Z1, V_Z2, V_Z3, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28
        li      5, 16
	Load_4Coeffs 5, 64, 64
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	addi	14, 14, -64
	Load_next_4zetas
	MREDUCE_4X V_Z0, V_Z1, V_Z2, V_Z3, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28

        li      5, 256
	Load_4Coeffs 5, 64, 64
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	Load_next_4zetas
	MREDUCE_4X V_Z0, V_Z1, V_Z2, V_Z3, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28

        li      5, 272
	Load_4Coeffs 5, 64, 64
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	addi	14, 14, -64
	Load_next_4zetas
        MREDUCE_4X V_Z0, V_Z1, V_Z2, V_Z3, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28

.align 4
__Len32:
        #
        # 5. len = 32, start = 0, 64, 128, 192
	#addi	14, 14, 896
	li	5, 0
	li	4, 64

	Load_4Coeffs 5, 16, 16
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	lvx	V_ZETA, 0, 14
        addi    14, 14, 16
	MREDUCE_4X V_ZETA, V_ZETA, V_ZETA, V_ZETA, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28
	li	5, 128

	Load_4Coeffs 5, 16, 16
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	lvx	V_ZETA, 0, 14
        addi    14, 14, 16
	MREDUCE_4X V_ZETA, V_ZETA, V_ZETA, V_ZETA, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28
	li	5, 256

	Load_4Coeffs 5, 16, 16
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	lvx	V_ZETA, 0, 14
        addi    14, 14, 16
	MREDUCE_4X V_ZETA, V_ZETA, V_ZETA, V_ZETA, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28
	li	5, 384

	Load_4Coeffs 5, 16, 16
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	lvx	V_ZETA, 0, 14
        addi    14, 14, 16
	MREDUCE_4X V_ZETA, V_ZETA, V_ZETA, V_ZETA, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28

.align 4
__Len64:
	#
	# 6. len = 64, start = 0, 128
	#addi	14, 14, 960
	li	5, 0
	li	4, 128
	Load_4Coeffs 5, 16, 16
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	lvx	V_ZETA, 0, 14
	addi	14, 14, 16
	MREDUCE_4X V_ZETA, V_ZETA, V_ZETA, V_ZETA, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28
	li	5, 64

	Load_4Coeffs 5, 16, 16
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	lxv	32+10, -16(14)
	MREDUCE_4X V_ZETA, V_ZETA, V_ZETA, V_ZETA, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28
	li	5, 256

	Load_4Coeffs 5, 16, 16
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	lvx	V_ZETA, 0, 14
	addi	14, 14, 16
	MREDUCE_4X V_ZETA, V_ZETA, V_ZETA, V_ZETA, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28
	li	5, 320

	Load_4Coeffs 5, 16, 16
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	lxv	32+10, -16(14)
	MREDUCE_4X V_ZETA, V_ZETA, V_ZETA, V_ZETA, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28

.align 4
__Len128:
	# 7. len = 128, start = 0
	#
	#addi	14, 14, 992
	li	5, 0            # start
	li	4, 256          # len * 2

	Load_4Coeffs 5, 16, 16
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	lvx	V_ZETA, 0, 14
	xxlor	9, 32+10, 32+10
	MREDUCE_4X V_ZETA, V_ZETA, V_ZETA, V_ZETA, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28
	li	5, 64

	Load_4Coeffs 5, 16, 16
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	xxlor	32+10, 9, 9
	MREDUCE_4X V_ZETA, V_ZETA, V_ZETA, V_ZETA, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28
	li	5, 128

	Load_4Coeffs 5, 16, 16
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	xxlor	32+10, 9, 9
	MREDUCE_4X V_ZETA, V_ZETA, V_ZETA, V_ZETA, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28
	li	5, 192

	Load_4Coeffs 5, 16, 16
	BREDUCE_4X 4, 9, 13, 17
	Write_B4C 32+4, 32+9, 32+13, 32+17
	Set_mont_consts
	xxlor	32+10, 9, 9
	MREDUCE_4X V_ZETA, V_ZETA, V_ZETA, V_ZETA, 13, 18, 23, 28
	Write_M4C 32+13, 32+18, 32+23, 32+28

.align 4
	#
	# Montgomery reduce loops with constant 1441
	#
	addis	10,2,.C1441@toc@ha
	addi	10,10,.C1441@toc@l
	lvx	V1441, 0, 10

	Reload_4coeffs
	MREDUCE_4X V1441, V1441, V1441, V1441, 6, 7, 8, 9
	Reload_4coeffs
	MREDUCE_4X V1441, V1441, V1441, V1441, 13, 18, 23, 28
	MWrite_8X 32+6, 32+7, 32+8, 32+9, 32+13, 32+18, 32+23, 32+28

	Reload_4coeffs
	MREDUCE_4X V1441, V1441, V1441, V1441, 6, 7, 8, 9
	Reload_4coeffs
	MREDUCE_4X V1441, V1441, V1441, V1441, 13, 18, 23, 28
	MWrite_8X 32+6, 32+7, 32+8, 32+9, 32+13, 32+18, 32+23, 32+28

	Reload_4coeffs
	MREDUCE_4X V1441, V1441, V1441, V1441, 6, 7, 8, 9
	Reload_4coeffs
	MREDUCE_4X V1441, V1441, V1441, V1441, 13, 18, 23, 28
	MWrite_8X 32+6, 32+7, 32+8, 32+9, 32+13, 32+18, 32+23, 32+28

	Reload_4coeffs
	MREDUCE_4X V1441, V1441, V1441, V1441, 6, 7, 8, 9
	Reload_4coeffs
	MREDUCE_4X V1441, V1441, V1441, V1441, 13, 18, 23, 28
	MWrite_8X 32+6, 32+7, 32+8, 32+9, 32+13, 32+18, 32+23, 32+28

__intt_out:
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
.size     mlk_intt_ppc,.-mlk_intt_ppc

.data
.align 4
# -MLKEM_Q
.nmkq:
.short -3329, -3329, -3329, -3329, -3329, -3329, -3329, -3329

# MLKEM_Q
.mkq:
.short 3329, 3329, 3329, 3329, 3329, 3329, 3329, 3329

.C20159:
.short  20159, 20159, 20159, 20159, 20159, 20159, 20159, 20159

# 0x2000000
.C25:
.long   33554432, 33554432, 33554432, 33554432

.C1441:
.short 1441, 1441, 1441, 1441, 1441, 1441, 1441, 1441

.align 4
.izeta127:
.short 1628, 1628, 1628, 1628, 1522, 1522, 1522, 1522
.short -1460, -1460, -1460, -1460, 958, 958, 958, 958
.short 991, 991, 991, 991, 996, 996, 996, 996
.short -308, -308, -308, -308, -108, -108, -108, -108
.short 478, 478, 478, 478, -870, -870, -870, -870
.short -854, -854, -854, -854, -1510, -1510, -1510, -1510
.short 794, 794, 794, 794, -1278, -1278, -1278, -1278
.short -1530, -1530, -1530, -1530, -1185, -1185, -1185, -1185
.short -1659, -1659, -1659, -1659, -1187, -1187, -1187, -1187
.short 220, 220, 220, 220, -874, -874, -874, -874
.short -1335, -1335, -1335, -1335, 1218, 1218, 1218, 1218
.short -136, -136, -136, -136, -1215, -1215, -1215, -1215
.short 384, 384, 384, 384, -1465, -1465, -1465, -1465
.short -1285, -1285, -1285, -1285, 1322, 1322, 1322, 1322
.short 610, 610, 610, 610, 603, 603, 603, 603
.short 1097, 1097, 1097, 1097, 817, 817, 817, 817
.short -75, -75, -75, -75, -156, -156, -156, -156
.short 329, 329, 329, 329, 418, 418, 418, 418
.short 349, 349, 349, 349, -872, -872, -872, -872
.short 644, 644, 644, 644, -1590, -1590, -1590, -1590
.short 1119, 1119, 1119, 1119, -602, -602, -602, -602
.short 1483, 1483, 1483, 1483, -777, -777, -777, -777
.short -147, -147, -147, -147, 1159, 1159, 1159, 1159
.short 778, 778, 778, 778, -246, -246, -246, -246
.short 1653, 1653, 1653, 1653, 1574, 1574, 1574, 1574
.short -460, -460, -460, -460, -291, -291, -291, -291
.short -235, -235, -235, -235, 177, 177, 177, 177
.short 587, 587, 587, 587, 422, 422, 422, 422
.short 105, 105, 105, 105, 1550, 1550, 1550, 1550
.short 871, 871, 871, 871, -1251, -1251, -1251, -1251
.short 843, 843, 843, 843, 555, 555, 555, 555
.short 430, 430, 430, 430, -1103, -1103, -1103, -1103
.izeta63:
.short -1275, -1275, -1275, -1275, -1275, -1275, -1275, -1275
.short 677, 677, 677, 677, 677, 677, 677, 677
.short -1065, -1065, -1065, -1065, -1065, -1065, -1065, -1065
.short 448, 448, 448, 448, 448, 448, 448, 448
.short -725, -725, -725, -725, -725, -725, -725, -725
.short -1508, -1508, -1508, -1508, -1508, -1508, -1508, -1508
.short 961, 961, 961, 961, 961, 961, 961, 961
.short -398, -398, -398, -398, -398, -398, -398, -398
.short -951, -951, -951, -951, -951, -951, -951, -951
.short -247, -247, -247, -247, -247, -247, -247, -247
.short -1421, -1421, -1421, -1421, -1421, -1421, -1421, -1421
.short 107, 107, 107, 107, 107, 107, 107, 107
.short 830, 830, 830, 830, 830, 830, 830, 830
.short -271, -271, -271, -271, -271, -271, -271, -271
.short -90, -90, -90, -90, -90, -90, -90, -90
.short -853, -853, -853, -853, -853, -853, -853, -853
.short 1469, 1469, 1469, 1469, 1469, 1469, 1469, 1469
.short 126, 126, 126, 126, 126, 126, 126, 126
.short -1162, -1162, -1162, -1162, -1162, -1162, -1162, -1162
.short -1618, -1618, -1618, -1618, -1618, -1618, -1618, -1618
.short -666, -666, -666, -666, -666, -666, -666, -666
.short -320, -320, -320, -320, -320, -320, -320, -320
.short -8, -8, -8, -8, -8, -8, -8, -8
.short 516, 516, 516, 516, 516, 516, 516, 516
.short -1544, -1544, -1544, -1544, -1544, -1544, -1544, -1544
.short -282, -282, -282, -282, -282, -282, -282, -282
.short 1491, 1491, 1491, 1491, 1491, 1491, 1491, 1491
.short -1293, -1293, -1293, -1293, -1293, -1293, -1293, -1293
.short 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015
.short -552, -552, -552, -552, -552, -552, -552, -552
.short 652, 652, 652, 652, 652, 652, 652, 652
.short 1223, 1223, 1223, 1223, 1223, 1223, 1223, 1223
.short -1571, -1571, -1571, -1571, -1571, -1571, -1571, -1571
.short -205, -205, -205, -205, -205, -205, -205, -205
.short 411, 411, 411, 411, 411, 411, 411, 411
.short -1542, -1542, -1542, -1542, -1542, -1542, -1542, -1542
.short 608, 608, 608, 608, 608, 608, 608, 608
.short 732, 732, 732, 732, 732, 732, 732, 732
.short 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017
.short -681, -681, -681, -681, -681, -681, -681, -681
.short -130, -130, -130, -130, -130, -130, -130, -130
.short -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602
.short 1458, 1458, 1458, 1458, 1458, 1458, 1458, 1458
.short -829, -829, -829, -829, -829, -829, -829, -829
.short 383, 383, 383, 383, 383, 383, 383, 383
.short 264, 264, 264, 264, 264, 264, 264, 264
.short -1325, -1325, -1325, -1325, -1325, -1325, -1325, -1325
.short 573, 573, 573, 573, 573, 573, 573, 573
.short 1468, 1468, 1468, 1468, 1468, 1468, 1468, 1468
.short -1474, -1474, -1474, -1474, -1474, -1474, -1474, -1474
.short -1202, -1202, -1202, -1202, -1202, -1202, -1202, -1202
.short 962, 962, 962, 962, 962, 962, 962, 962
.short 182, 182, 182, 182, 182, 182, 182, 182
.short 1577, 1577, 1577, 1577, 1577, 1577, 1577, 1577
.short 622, 622, 622, 622, 622, 622, 622, 622
.short -171, -171, -171, -171, -171, -171, -171, -171
.short 202, 202, 202, 202, 202, 202, 202, 202
.short 287, 287, 287, 287, 287, 287, 287, 287
.short 1422, 1422, 1422, 1422, 1422, 1422, 1422, 1422
.short 1493, 1493, 1493, 1493, 1493, 1493, 1493, 1493
.short -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517
.short -359, -359, -359, -359, -359, -359, -359, -359
.short -758, -758, -758, -758, -758, -758, -758, -758
