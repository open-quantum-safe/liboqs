##############################################################################
# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0"
#
# Written by Nir Drucker and Shay Gueron
# AWS Cryptographic Algorithms Group.
# (ndrucker@amazon.com, gueron@amazon.com)
#
# Based on:
# github.com/Shay-Gueron/A-toolbox-for-software-optimization-of-QC-MDPC-code-based-cryptosystems
##############################################################################

#define __ASM_FILE__
#include "bike_defs.h"

#define p %rdi
#define itr %r8

.hidden red
.globl  red
.type   red,@function
.align  16
red:
    xor itr, itr

.align  16
.Lred_loop:
    vmovdqu R_QW    *0x8(p,itr,8), %ymm2
    vmovdqu (R_QW-1)*0x8(p,itr,8), %ymm3
    vmovdqu (p,itr,8), %ymm4
    
    vpsllq $LAST_R_QW_TRAIL, %ymm2, %ymm2
    vpsrlq $LAST_R_QW_LEAD,  %ymm3, %ymm3
    
    vpor  %ymm2, %ymm3, %ymm5
    vpxor %ymm4, %ymm5, %ymm5
    vmovdqu %ymm5, (p,itr,8)

    add $4, itr
    cmp $R_QW, itr
    jb .Lred_loop

    movq $LAST_R_QW_MASK, %r10
    andq %r10, (R_QW-1)*0x8(p)

    vpxor %ymm0, %ymm0, %ymm0
    lea ((2*R_QW)*0x8) - YMM_SIZE(p), %r8
    lea (R_QW)*0x8(p), p

.align 32
.Lzero_loop:
    vmovdqu %ymm0, (%r8)
    lea -YMM_SIZE(%r8), %r8
    cmp %r8, p
    jg .Lzero_loop

    ret
.size   red,.-red
