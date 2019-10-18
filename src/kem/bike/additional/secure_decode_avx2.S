##############################################################################
# Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License").
# You may not use this file except in compliance with the License.
# A copy of the License is located at
#
# http://aws.amazon.com/apache2.0
#
# or in the "license" file accompanying this file. This file is distributed
# on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
# express or implied. See the License for the specific language governing
# permissions and limitations under the License.
# The license is detailed in the file LICENSE.md, and applies to this file.
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

.text    
########################################################################
#void calc_upc(uint8_t unsat_counter[N_BITS],
#              const uint8_t s[N_BITS],
#              const uint64_t inv_h0_compressed[DV],
#              const uint64_t inv_h1_compressed[DV])

#define unsat_counter %rdi
#define s %rsi
#define inv_h0_compressed %rdx
#define inv_h1_compressed %rcx

#define tmp32 %eax
#define tmp %rax

#define itr1 %r10
#define itr2 %r11

#define YMM_NUM  8
#define TOTAL_YMMS_SIZE  (YMM_NUM*YMM_SIZE)

#define mask %ymm15

.macro SUM tag inv_h_compressed res_offset
    xor itr1, itr1
.Lloop\tag:

    vxorps %ymm0, %ymm0, %ymm0
    vxorps %ymm1, %ymm1, %ymm1
    vxorps %ymm2, %ymm2, %ymm2
    vxorps %ymm3, %ymm3, %ymm3
    vxorps %ymm4, %ymm4, %ymm4
    vxorps %ymm5, %ymm5, %ymm5
    vxorps %ymm6, %ymm6, %ymm6
    vxorps %ymm7, %ymm7, %ymm7

    xor itr2, itr2

.Linner_loop\tag:

        #load position
        vbroadcastss 0x4(\inv_h_compressed, itr2, 8), mask
        mov (\inv_h_compressed, itr2, 8), tmp32
        
        #adjust loop offset
        add itr1, tmp 

        vpand (YMM_SIZE*0)(s, tmp, 1), mask, %ymm8
        vpand (YMM_SIZE*1)(s, tmp, 1), mask, %ymm9
        vpand (YMM_SIZE*2)(s, tmp, 1), mask, %ymm10
        vpand (YMM_SIZE*3)(s, tmp, 1), mask, %ymm11
        vpand (YMM_SIZE*4)(s, tmp, 1), mask, %ymm12
        vpand (YMM_SIZE*5)(s, tmp, 1), mask, %ymm13

        vpaddb %ymm0, %ymm8, %ymm0
        vpaddb %ymm1, %ymm9, %ymm1
        vpaddb %ymm2, %ymm10, %ymm2
        vpaddb %ymm3, %ymm11, %ymm3
        vpaddb %ymm4, %ymm12, %ymm4
        
        vpand (YMM_SIZE*6)(s, tmp, 1), mask, %ymm14
        vpand (YMM_SIZE*7)(s, tmp, 1), mask, %ymm15
        
        vpaddb %ymm5, %ymm13, %ymm5
        vpaddb %ymm6, %ymm14, %ymm6
        vpaddb %ymm7, %ymm15, %ymm7
        
        inc itr2
        cmp $FAKE_DV, itr2
        jl .Linner_loop\tag

    vmovdqu %ymm0, \res_offset + (YMM_SIZE*0)(unsat_counter, itr1, 1)
    vmovdqu %ymm1, \res_offset + (YMM_SIZE*1)(unsat_counter, itr1, 1)
    vmovdqu %ymm2, \res_offset + (YMM_SIZE*2)(unsat_counter, itr1, 1)
    vmovdqu %ymm3, \res_offset + (YMM_SIZE*3)(unsat_counter, itr1, 1)
    vmovdqu %ymm4, \res_offset + (YMM_SIZE*4)(unsat_counter, itr1, 1)
    vmovdqu %ymm5, \res_offset + (YMM_SIZE*5)(unsat_counter, itr1, 1)
    vmovdqu %ymm6, \res_offset + (YMM_SIZE*6)(unsat_counter, itr1, 1)
    vmovdqu %ymm7, \res_offset + (YMM_SIZE*7)(unsat_counter, itr1, 1)

    add $TOTAL_YMMS_SIZE, itr1
    cmp $R_DDQWORDS_BITS, itr1
    jnz .Lloop\tag
.endm

.globl    calc_upc
.hidden   calc_upc
.type     calc_upc,@function
.align    16
calc_upc:
    SUM h0 inv_h0_compressed 0
    SUM h1 inv_h1_compressed R_BITS

    ret
.size    calc_upc,.-calc_upc
