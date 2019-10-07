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

#define mask %zmm31

#define ZMM_NUM  16

.macro SUM tag inv_h_compressed res_offset
    xor itr1, itr1
.Lloop\tag:

    vxorps %zmm0,  %zmm0,  %zmm0
    vxorps %zmm1,  %zmm1,  %zmm1
    vxorps %zmm2,  %zmm2,  %zmm2
    vxorps %zmm3,  %zmm3,  %zmm3
    vxorps %zmm4,  %zmm4,  %zmm4
    vxorps %zmm5,  %zmm5,  %zmm5
    vxorps %zmm6,  %zmm6,  %zmm6
    vxorps %zmm7,  %zmm7,  %zmm7
    vxorps %zmm8,  %zmm8,  %zmm8
    vxorps %zmm9,  %zmm9,  %zmm9
    vxorps %zmm10, %zmm10, %zmm10
    vxorps %zmm11, %zmm11, %zmm11
    vxorps %zmm12, %zmm12, %zmm12
    vxorps %zmm13, %zmm13, %zmm13
    vxorps %zmm14, %zmm14, %zmm14
    vxorps %zmm15, %zmm15, %zmm15

    xor itr2, itr2

.Linner_loop\tag:

        #load position
        vbroadcastss 0x4(\inv_h_compressed, itr2, 8), mask
        mov (\inv_h_compressed, itr2, 8), tmp32
        
        #adjust loop offset
        add itr1, tmp 

        vpandq (ZMM_SIZE*0)(s, tmp, 1), mask, %zmm16
        vpandq (ZMM_SIZE*1)(s, tmp, 1), mask, %zmm17
        vpandq (ZMM_SIZE*2)(s, tmp, 1), mask, %zmm18
        vpandq (ZMM_SIZE*3)(s, tmp, 1), mask, %zmm19
        
        vpaddb %zmm0, %zmm16, %zmm0
        vpaddb %zmm1, %zmm17, %zmm1
        vpaddb %zmm2, %zmm18, %zmm2
        vpaddb %zmm3, %zmm19, %zmm3

        vpandq (ZMM_SIZE*4)(s, tmp, 1), mask, %zmm20
        vpandq (ZMM_SIZE*5)(s, tmp, 1), mask, %zmm21
        vpandq (ZMM_SIZE*6)(s, tmp, 1), mask, %zmm22
        vpandq (ZMM_SIZE*7)(s, tmp, 1), mask, %zmm23

        vpaddb %zmm4, %zmm20, %zmm4
        vpaddb %zmm5, %zmm21, %zmm5
        vpaddb %zmm6, %zmm22, %zmm6
        vpaddb %zmm7, %zmm23, %zmm7

        vpandq (ZMM_SIZE*8)(s, tmp, 1), mask, %zmm24
        vpandq (ZMM_SIZE*9)(s, tmp, 1), mask, %zmm25
        vpandq (ZMM_SIZE*10)(s, tmp, 1), mask, %zmm26
        vpandq (ZMM_SIZE*11)(s, tmp, 1), mask, %zmm27

        vpaddb %zmm8, %zmm24, %zmm8
        vpaddb %zmm9, %zmm25, %zmm9
        vpaddb %zmm10, %zmm26, %zmm10
        vpaddb %zmm11, %zmm27, %zmm11

        vpandq (ZMM_SIZE*12)(s, tmp, 1), mask, %zmm28
        vpandq (ZMM_SIZE*13)(s, tmp, 1), mask, %zmm29
        vpandq (ZMM_SIZE*14)(s, tmp, 1), mask, %zmm30
        vpandq (ZMM_SIZE*15)(s, tmp, 1), mask, %zmm31

        vpaddb %zmm12, %zmm28, %zmm12
        vpaddb %zmm13, %zmm29, %zmm13
        vpaddb %zmm14, %zmm30, %zmm14
        vpaddb %zmm15, %zmm31, %zmm15
                
        inc itr2
        cmp $FAKE_DV, itr2
        jl .Linner_loop\tag

    vmovdqu64 %zmm0, \res_offset + (ZMM_SIZE*0)(unsat_counter, itr1, 1)
    vmovdqu64 %zmm1, \res_offset + (ZMM_SIZE*1)(unsat_counter, itr1, 1)
    vmovdqu64 %zmm2, \res_offset + (ZMM_SIZE*2)(unsat_counter, itr1, 1)
    vmovdqu64 %zmm3, \res_offset + (ZMM_SIZE*3)(unsat_counter, itr1, 1)
    vmovdqu64 %zmm4, \res_offset + (ZMM_SIZE*4)(unsat_counter, itr1, 1)
    vmovdqu64 %zmm5, \res_offset + (ZMM_SIZE*5)(unsat_counter, itr1, 1)
    vmovdqu64 %zmm6, \res_offset + (ZMM_SIZE*6)(unsat_counter, itr1, 1)
    vmovdqu64 %zmm7, \res_offset + (ZMM_SIZE*7)(unsat_counter, itr1, 1)
    vmovdqu64 %zmm8, \res_offset + (ZMM_SIZE*8)(unsat_counter, itr1, 1)
    vmovdqu64 %zmm9, \res_offset + (ZMM_SIZE*9)(unsat_counter, itr1, 1)
    vmovdqu64 %zmm10, \res_offset + (ZMM_SIZE*10)(unsat_counter, itr1, 1)
    vmovdqu64 %zmm11, \res_offset + (ZMM_SIZE*11)(unsat_counter, itr1, 1)
    vmovdqu64 %zmm12, \res_offset + (ZMM_SIZE*12)(unsat_counter, itr1, 1)
    vmovdqu64 %zmm13, \res_offset + (ZMM_SIZE*13)(unsat_counter, itr1, 1)
    vmovdqu64 %zmm14, \res_offset + (ZMM_SIZE*14)(unsat_counter, itr1, 1)
    vmovdqu64 %zmm15, \res_offset + (ZMM_SIZE*15)(unsat_counter, itr1, 1)

    add $16*ZMM_SIZE, itr1
    cmp $R_QDQWORDS_BITS, itr1
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
