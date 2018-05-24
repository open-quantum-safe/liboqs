##############################################################################
# Additional implementation of "BIKE: Bit Flipping Key Encapsulation". 
# Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
#
# Written by Nir Drucker and Shay Gueron
# AWS Cryptographic Algorithms Group
# (ndrucker@amazon.com, gueron@amazon.com)
#
# The license is detailed in the file LICENSE.txt, and applies to this file.
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

#This function is optimized to weight % 3 = 0! (When LEVEL=5)
#and to len % 64 = 0!
#other sizes will cause buffer overflows.

#ABI
.set a,      %rdi
.set wlist,  %rsi
.set len,    %rdx
.set weight, %rcx

.set dword_pos, %r8d
.set bit_pos,   %r9d
.set bit_mask,  %r10d
.set itr,       %r11
.set w_itr,     %r12

.set DWORD_POS0,  %ymm0
.set DWORD_POS1,  %ymm1
.set DWORD_POS2,  %ymm2

.set BIT_MASK0,   %ymm3
.set BIT_MASK1,   %ymm4
.set BIT_MASK2,   %ymm5
.set INC,         %ymm6

.set DWORDS_ITR0, %ymm7
.set DWORDS_ITR1, %ymm8

.set MEM0,       %ymm9
.set MEM1,       %ymm10

.set CMP0,       %ymm11
.set CMP1,       %ymm12

#define YMM_USED 01

.macro LOAD_POS i
        mov $1, bit_mask

        mov 0x8*\i(wlist, w_itr, 8), dword_pos
        mov 0x8*\i(wlist, w_itr, 8), bit_pos

        #mask the bit mask if needed.
        #wlist elements are 4bytes value and 4 bytes mask.
        and (0x8 * \i) + 0x4(wlist, w_itr, 8), bit_mask

        shr $5, dword_pos
        and $31, bit_pos
        shlx bit_pos, bit_mask, bit_mask

        #copy to tmp mem in order to broadcast.
        mov dword_pos,   (%rsp)
        mov bit_mask, 0x8(%rsp)
        
        #copy to wide regs.
        vpbroadcastd (%rsp), DWORD_POS\i
        vpbroadcastd 0x8(%rsp), BIT_MASK\i
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
        .irpc i,YMM_USED
            vmovdqu  INIT_POS\i(%rip), DWORDS_ITR\i
        .endr

        LOAD_POS 0
        LOAD_POS 1
        LOAD_POS 2
        
        xor itr, itr

.align 16
.loop:
        .irpc i,YMM_USED
            vmovdqu YMM_SIZE*\i(a, itr, 1), MEM\i
        .endr

        .irpc j,012
            .irpc i,YMM_USED
                vpcmpeqd DWORDS_ITR\i, DWORD_POS\j, CMP\i
            .endr

            .irpc i,YMM_USED
                vpand CMP\i, BIT_MASK\j, CMP\i
            .endr

            .irpc i,YMM_USED
                vpor MEM\i, CMP\i, MEM\i
            .endr
        .endr

        .irpc i,YMM_USED
            vmovdqu MEM\i, YMM_SIZE*\i(a, itr, 1)
        .endr

        .irpc i,YMM_USED
            vpaddq INC, DWORDS_ITR\i, DWORDS_ITR\i
        .endr

        add $2*0x20, itr
        cmp len, itr
        jl .loop

    add $3, w_itr
    cmp weight, w_itr
    jle .wloop

#Do the rest if requried. (<3).
#if LEVEL < 5

    #restore
    add $3, weight
    cmp weight, w_itr
    je .exit

.rest_wloop:
        .irpc i,YMM_USED
            vmovdqu  INIT_POS\i(%rip), DWORDS_ITR\i
        .endr

        LOAD_POS 0
        xor itr, itr

.rest_loop:
        .irpc i,YMM_USED
            vmovdqu YMM_SIZE*\i(a, itr, 1), MEM\i
        .endr

        .irpc i,YMM_USED
            vpcmpeqd DWORDS_ITR\i, DWORD_POS0, CMP\i
        .endr

        .irpc i,YMM_USED
            vpand CMP\i, BIT_MASK0, CMP\i
        .endr

        .irpc i,YMM_USED
            vpor MEM\i, CMP\i, MEM\i
        .endr

        .irpc i,YMM_USED
            vmovdqu MEM\i, YMM_SIZE*\i(a, itr, 1)
        .endr

        .irpc i,YMM_USED
            vpaddq INC, DWORDS_ITR\i, DWORDS_ITR\i
        .endr

        add $2*0x20, itr
        cmp len, itr
        jl .rest_loop
    
    inc w_itr
    cmp weight, w_itr
    jl .rest_wloop
        
#endif

.exit:
    add $2*8, %rsp
    pop w_itr
    ret
.size    secure_set_bits,.-secure_set_bits
