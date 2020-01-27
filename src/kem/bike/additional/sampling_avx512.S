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
INIT_POS1:
.long   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15
INIT_POS2:
.long  16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
INIT_POS3:
.long  32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47
INIT_POS4:
.long  48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63

DWORDS_INC:
.long 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64

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
#define w_itr     %rax

#define DWORD_POS0  %zmm0
#define DWORD_POS1  %zmm1
#define DWORD_POS2  %zmm2

#define BIT_MASK0   %zmm3
#define BIT_MASK1   %zmm4
#define BIT_MASK2   %zmm5
#define INC         %zmm6

#define DWORDS_ITR1 %zmm7
#define DWORDS_ITR2 %zmm8
#define DWORDS_ITR3 %zmm9
#define DWORDS_ITR4 %zmm10
#define DWORDS_ITR5 %zmm11
#define DWORDS_ITR6 %zmm12
#define DWORDS_ITR7 %zmm13
#define DWORDS_ITR8 %zmm14

#define MEM1       %zmm15
#define MEM2       %zmm16
#define MEM3       %zmm17
#define MEM4       %zmm18
#define MEM5       %zmm19
#define MEM6       %zmm20
#define MEM7       %zmm21
#define MEM8       %zmm22

#define _MM_CMPINT_EQ 0

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
    sub $2*8, %rsp
    mov $-1, %eax
    kmovd %eax, %k1
    sub $3, weight

    xor w_itr, w_itr
.wloop:
        vmovdqa64  INIT_POS1(%rip), DWORDS_ITR1
        vmovdqa64  INIT_POS2(%rip), DWORDS_ITR2
        vmovdqa64  INIT_POS3(%rip), DWORDS_ITR3
        vmovdqa64  INIT_POS4(%rip), DWORDS_ITR4

        vmovdqa64   DWORDS_INC(%rip), INC
        vpaddd INC, DWORDS_ITR1, DWORDS_ITR5
        vpaddd INC, DWORDS_ITR2, DWORDS_ITR6
        vpaddd INC, DWORDS_ITR3, DWORDS_ITR7
        vpaddd INC, DWORDS_ITR4, DWORDS_ITR8
        vpaddd INC, INC, INC

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
        vmovdqu64 ZMM_SIZE*(1-1)(a, itr, 1), MEM1
        vmovdqu64 ZMM_SIZE*(2-1)(a, itr, 1), MEM2
        vmovdqu64 ZMM_SIZE*(3-1)(a, itr, 1), MEM3
        vmovdqu64 ZMM_SIZE*(4-1)(a, itr, 1), MEM4
        vmovdqu64 ZMM_SIZE*(5-1)(a, itr, 1), MEM5
        vmovdqu64 ZMM_SIZE*(6-1)(a, itr, 1), MEM6
        vmovdqu64 ZMM_SIZE*(7-1)(a, itr, 1), MEM7
        vmovdqu64 ZMM_SIZE*(8-1)(a, itr, 1), MEM8

        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR1, DWORD_POS0, %k1
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR2, DWORD_POS0, %k2
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR3, DWORD_POS0, %k3
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR4, DWORD_POS0, %k4
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR5, DWORD_POS0, %k5
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR6, DWORD_POS0, %k6
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR7, DWORD_POS0, %k7

        vpord MEM1, BIT_MASK0, MEM1{%k1}
        vpord MEM2, BIT_MASK0, MEM2{%k2}
        vpord MEM3, BIT_MASK0, MEM3{%k3}
        vpord MEM4, BIT_MASK0, MEM4{%k4}
        vpord MEM5, BIT_MASK0, MEM5{%k5}
        vpord MEM6, BIT_MASK0, MEM6{%k6}
        vpord MEM7, BIT_MASK0, MEM7{%k7}

        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR8, DWORD_POS0, %k1
        vpord MEM8, BIT_MASK0, MEM8{%k1}

        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR1, DWORD_POS1, %k1
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR2, DWORD_POS1, %k2
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR3, DWORD_POS1, %k3
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR4, DWORD_POS1, %k4
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR5, DWORD_POS1, %k5
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR6, DWORD_POS1, %k6
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR7, DWORD_POS1, %k7

        vpord MEM1, BIT_MASK1, MEM1{%k1}
        vpord MEM2, BIT_MASK1, MEM2{%k2}
        vpord MEM3, BIT_MASK1, MEM3{%k3}
        vpord MEM4, BIT_MASK1, MEM4{%k4}
        vpord MEM5, BIT_MASK1, MEM5{%k5}
        vpord MEM6, BIT_MASK1, MEM6{%k6}
        vpord MEM7, BIT_MASK1, MEM7{%k7}

        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR8, DWORD_POS1, %k1
        vpord MEM8, BIT_MASK1, MEM8{%k1}

        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR1, DWORD_POS2, %k1
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR2, DWORD_POS2, %k2
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR3, DWORD_POS2, %k3
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR4, DWORD_POS2, %k4
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR5, DWORD_POS2, %k5
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR6, DWORD_POS2, %k6
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR7, DWORD_POS2, %k7

        vpord MEM1, BIT_MASK2, MEM1{%k1}
        vpord MEM2, BIT_MASK2, MEM2{%k2}
        vpord MEM3, BIT_MASK2, MEM3{%k3}
        vpord MEM4, BIT_MASK2, MEM4{%k4}
        vpord MEM5, BIT_MASK2, MEM5{%k5}
        vpord MEM6, BIT_MASK2, MEM6{%k6}
        vpord MEM7, BIT_MASK2, MEM7{%k7}

        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR8, DWORD_POS2, %k1
        vpord MEM8, BIT_MASK2, MEM8{%k1}

        vmovdqu64 MEM1, ZMM_SIZE*(1-1)(a, itr, 1)
        vmovdqu64 MEM2, ZMM_SIZE*(2-1)(a, itr, 1)
        vmovdqu64 MEM3, ZMM_SIZE*(3-1)(a, itr, 1)
        vmovdqu64 MEM4, ZMM_SIZE*(4-1)(a, itr, 1)
        vmovdqu64 MEM5, ZMM_SIZE*(5-1)(a, itr, 1)
        vmovdqu64 MEM6, ZMM_SIZE*(6-1)(a, itr, 1)
        vmovdqu64 MEM7, ZMM_SIZE*(7-1)(a, itr, 1)
        vmovdqu64 MEM8, ZMM_SIZE*(8-1)(a, itr, 1)

        vpaddd INC, DWORDS_ITR1, DWORDS_ITR1
        vpaddd INC, DWORDS_ITR2, DWORDS_ITR2
        vpaddd INC, DWORDS_ITR3, DWORDS_ITR3
        vpaddd INC, DWORDS_ITR4, DWORDS_ITR4
        vpaddd INC, DWORDS_ITR5, DWORDS_ITR5
        vpaddd INC, DWORDS_ITR6, DWORDS_ITR6
        vpaddd INC, DWORDS_ITR7, DWORDS_ITR7
        vpaddd INC, DWORDS_ITR8, DWORDS_ITR8

        add $8*ZMM_SIZE, itr
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
        vmovdqa64  INIT_POS1(%rip), DWORDS_ITR1
        vmovdqa64  INIT_POS2(%rip), DWORDS_ITR2
        vmovdqa64  INIT_POS3(%rip), DWORDS_ITR3
        vmovdqa64  INIT_POS4(%rip), DWORDS_ITR4
        vmovdqa64  DWORDS_INC(%rip), INC

        vpaddd INC, DWORDS_ITR1, DWORDS_ITR5
        vpaddd INC, DWORDS_ITR2, DWORDS_ITR6
        vpaddd INC, DWORDS_ITR3, DWORDS_ITR7
        vpaddd INC, DWORDS_ITR4, DWORDS_ITR8
        vpaddd INC, INC, INC

        LOAD_POS 0
        vpbroadcastd (%rsp), DWORD_POS0
        vpbroadcastd 0x8(%rsp), BIT_MASK0
        
        xor itr, itr

.rest_loop:

        vmovdqu64 ZMM_SIZE*(1-1)(a, itr, 1), MEM1
        vmovdqu64 ZMM_SIZE*(2-1)(a, itr, 1), MEM2
        vmovdqu64 ZMM_SIZE*(3-1)(a, itr, 1), MEM3
        vmovdqu64 ZMM_SIZE*(4-1)(a, itr, 1), MEM4
        vmovdqu64 ZMM_SIZE*(5-1)(a, itr, 1), MEM5
        vmovdqu64 ZMM_SIZE*(6-1)(a, itr, 1), MEM6
        vmovdqu64 ZMM_SIZE*(7-1)(a, itr, 1), MEM7
        vmovdqu64 ZMM_SIZE*(8-1)(a, itr, 1), MEM8

        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR1, DWORD_POS0, %k1
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR2, DWORD_POS0, %k2
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR3, DWORD_POS0, %k3
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR4, DWORD_POS0, %k4
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR5, DWORD_POS0, %k5
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR6, DWORD_POS0, %k6
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR7, DWORD_POS0, %k7

        vpord MEM1, BIT_MASK0, MEM1{%k1}
        vpord MEM2, BIT_MASK0, MEM2{%k2}
        vpord MEM3, BIT_MASK0, MEM3{%k3}
        vpord MEM4, BIT_MASK0, MEM4{%k4}
        vpord MEM5, BIT_MASK0, MEM5{%k5}
        vpord MEM6, BIT_MASK0, MEM6{%k6}
        vpord MEM7, BIT_MASK0, MEM7{%k7}
        
        vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR8, DWORD_POS0, %k1
        vpord MEM8, BIT_MASK0, MEM8{%k1}

        vmovdqu64 MEM1, ZMM_SIZE*(1-1)(a, itr, 1)
        vmovdqu64 MEM2, ZMM_SIZE*(2-1)(a, itr, 1)
        vmovdqu64 MEM3, ZMM_SIZE*(3-1)(a, itr, 1)
        vmovdqu64 MEM4, ZMM_SIZE*(4-1)(a, itr, 1)
        vmovdqu64 MEM5, ZMM_SIZE*(5-1)(a, itr, 1)
        vmovdqu64 MEM6, ZMM_SIZE*(6-1)(a, itr, 1)
        vmovdqu64 MEM7, ZMM_SIZE*(7-1)(a, itr, 1)
        vmovdqu64 MEM8, ZMM_SIZE*(8-1)(a, itr, 1)

        vpaddd INC, DWORDS_ITR1, DWORDS_ITR1
        vpaddd INC, DWORDS_ITR2, DWORDS_ITR2
        vpaddd INC, DWORDS_ITR3, DWORDS_ITR3
        vpaddd INC, DWORDS_ITR4, DWORDS_ITR4
        vpaddd INC, DWORDS_ITR5, DWORDS_ITR5
        vpaddd INC, DWORDS_ITR6, DWORDS_ITR6
        vpaddd INC, DWORDS_ITR7, DWORDS_ITR7
        vpaddd INC, DWORDS_ITR8, DWORDS_ITR8

        add $8*ZMM_SIZE, itr
        cmp len, itr
        jl .rest_loop
    
    inc w_itr
    cmp weight, w_itr
    jl .rest_wloop

.exit:

    add $2*8, %rsp
    ret
.size    secure_set_bits,.-secure_set_bits

