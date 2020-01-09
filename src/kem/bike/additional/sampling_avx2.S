##############################################################################
# Copyright 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

.data 

.align 64
INIT_POS0:
.long  0,  1,  2,  3,  4,  5,  6,  7
INIT_POS1:
.long  8,  9, 10, 11, 12, 13, 14, 15

DWORDS_INC:
.long 16, 16, 16, 16, 16, 16, 16, 16

.text    
#void secure_set_bits(IN OUT uint8_t* a, 
#                     IN const compressed_n_t* wlist,
#                     IN const uint32_t a_len,
#                     IN const uint32_t weight)
#{
#    const uint32_t dword_pos = pos >> 5;
#    const uint32_t bit_pos = pos & 0x1f;
#    r[dword_pos] |= (BIT(bit_pos) & mask);
#}
#

#ABI
#define a      %rdi
#define wlist  %rsi
#define len    %rdx
#define weight %rcx

#define dword_pos %r8d
#define bit_pos   %r9d
#define bit_mask  %r10d
#define itr       %r11
#define w_itr     %r12

#define DWORD_POS0  %ymm0
#define DWORD_POS1  %ymm1
#define DWORD_POS2  %ymm2

#define BIT_MASK0   %ymm3
#define BIT_MASK1   %ymm4
#define BIT_MASK2   %ymm5
#define INC         %ymm6

#define DWORDS_ITR0 %ymm7
#define DWORDS_ITR1 %ymm8

#define MEM0       %ymm9
#define MEM1       %ymm10

#define CMP0       %ymm11
#define CMP1       %ymm12

# Set %rsp[16:0] = (bit_mask, dword_pos)
.macro LOAD_POS i
        mov $1, bit_mask

        mov 0x4*\i(wlist, w_itr, 4), dword_pos
        mov 0x4*\i(wlist, w_itr, 4), bit_pos

        shr $5, dword_pos
        and $31, bit_pos
        shlx bit_pos, bit_mask, bit_mask

        #copy to tmp mem in order to broadcast.
        mov dword_pos,   (%rsp)
        mov bit_mask, 0x8(%rsp)
.endm

.globl    secure_set_bits
.hidden   secure_set_bits
.type     secure_set_bits,@function
.align    16
secure_set_bits:
    push w_itr
    sub $2*8, %rsp

    sub $3, weight
    xor w_itr, w_itr
    vmovdqu  DWORDS_INC(%rip), INC

.wloop:
        vmovdqu  INIT_POS0(%rip), DWORDS_ITR0
        vmovdqu  INIT_POS1(%rip), DWORDS_ITR1

        LOAD_POS 0
        vpbroadcastd (%rsp), DWORD_POS0
        vpbroadcastd 0x8(%rsp), BIT_MASK0

        LOAD_POS 1
        vpbroadcastd (%rsp), DWORD_POS1
        vpbroadcastd 0x8(%rsp), BIT_MASK1
        
        LOAD_POS 2
        vpbroadcastd (%rsp), DWORD_POS2
        vpbroadcastd 0x8(%rsp), BIT_MASK2
        
        xor itr, itr

.align 16
.loop:
        vmovdqu YMM_SIZE*0(a, itr, 1), MEM0
        vmovdqu YMM_SIZE*1(a, itr, 1), MEM1

        vpcmpeqd DWORDS_ITR0, DWORD_POS0, CMP0
        vpcmpeqd DWORDS_ITR1, DWORD_POS0, CMP1
        vpand CMP0, BIT_MASK0, CMP0
        vpand CMP1, BIT_MASK0, CMP1
        vpor MEM0, CMP0, MEM0
        vpor MEM1, CMP1, MEM1
        
        vpcmpeqd DWORDS_ITR0, DWORD_POS1, CMP0
        vpcmpeqd DWORDS_ITR1, DWORD_POS1, CMP1
        vpand CMP0, BIT_MASK1, CMP0
        vpand CMP1, BIT_MASK1, CMP1
        vpor MEM0, CMP0, MEM0
        vpor MEM1, CMP1, MEM1
            
        vpcmpeqd DWORDS_ITR0, DWORD_POS2, CMP0
        vpcmpeqd DWORDS_ITR1, DWORD_POS2, CMP1
        vpand CMP0, BIT_MASK2, CMP0
        vpand CMP1, BIT_MASK2, CMP1
        vpor MEM0, CMP0, MEM0
        vpor MEM1, CMP1, MEM1

        vmovdqu MEM0, YMM_SIZE*0(a, itr, 1)
        vmovdqu MEM1, YMM_SIZE*1(a, itr, 1)
        vpaddq INC, DWORDS_ITR0, DWORDS_ITR0
        vpaddq INC, DWORDS_ITR1, DWORDS_ITR1

        add $2*0x20, itr
        cmp len, itr
        jl .loop

    add $3, w_itr
    cmp weight, w_itr
    jle .wloop

    #restore
    add $3, weight
    cmp weight, w_itr
    je .exit

.rest_wloop:
        vmovdqu  INIT_POS0(%rip), DWORDS_ITR0
        vmovdqu  INIT_POS1(%rip), DWORDS_ITR1

        LOAD_POS 0
        vpbroadcastd (%rsp), DWORD_POS0
        vpbroadcastd 0x8(%rsp), BIT_MASK0

        xor itr, itr

.rest_loop:
        vmovdqu YMM_SIZE*0(a, itr, 1), MEM0
        vmovdqu YMM_SIZE*1(a, itr, 1), MEM1
        vpcmpeqd DWORDS_ITR0, DWORD_POS0, CMP0
        vpcmpeqd DWORDS_ITR1, DWORD_POS0, CMP1
        vpand CMP0, BIT_MASK0, CMP0
        vpand CMP1, BIT_MASK0, CMP1
        vpor MEM0, CMP0, MEM0
        vpor MEM1, CMP1, MEM1
        vmovdqu MEM0, YMM_SIZE*0(a, itr, 1)
        vmovdqu MEM1, YMM_SIZE*1(a, itr, 1)
        vpaddq INC, DWORDS_ITR0, DWORDS_ITR0
        vpaddq INC, DWORDS_ITR1, DWORDS_ITR1

        add $2*0x20, itr
        cmp len, itr
        jl .rest_loop
    
    inc w_itr
    cmp weight, w_itr
    jl .rest_wloop
        
.exit:
    add $2*8, %rsp
    pop w_itr
    ret
.size    secure_set_bits,.-secure_set_bits
