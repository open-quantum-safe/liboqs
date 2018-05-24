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

#This function is optimized to weight % 3 = 0! (when LEVEL=5)
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
.set w_itr,     %rax

.set DWORD_POS0,  %zmm0
.set DWORD_POS1,  %zmm1
.set DWORD_POS2,  %zmm2

.set BIT_MASK0,   %zmm3
.set BIT_MASK1,   %zmm4
.set BIT_MASK2,   %zmm5
.set INC,         %zmm6

.set DWORDS_ITR1, %zmm7
.set DWORDS_ITR2, %zmm8
.set DWORDS_ITR3, %zmm9
.set DWORDS_ITR4, %zmm10
.set DWORDS_ITR5, %zmm11
.set DWORDS_ITR6, %zmm12
.set DWORDS_ITR7, %zmm13
.set DWORDS_ITR8, %zmm14

.set MEM1,       %zmm15
.set MEM2,       %zmm16
.set MEM3,       %zmm17
.set MEM4,       %zmm18
.set MEM5,       %zmm19
.set MEM6,       %zmm20
.set MEM7,       %zmm21
.set MEM8,       %zmm22


.set _MM_CMPINT_EQ, 0
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
    sub $2*8, %rsp
    mov $-1, %eax
    kmovd %eax, %k1
    sub $3, weight

    xor w_itr, w_itr
.wloop:
        .irpc i,1234
            vmovdqa64  INIT_POS\i(%rip), DWORDS_ITR\i
        .endr
        vmovdqa64   DWORDS_INC(%rip), INC
        vpaddd INC, DWORDS_ITR1, DWORDS_ITR5
        vpaddd INC, DWORDS_ITR2, DWORDS_ITR6
        vpaddd INC, DWORDS_ITR3, DWORDS_ITR7
        vpaddd INC, DWORDS_ITR4, DWORDS_ITR8
		vpaddd INC, INC, INC

        LOAD_POS 0
        LOAD_POS 1
        LOAD_POS 2
        
        xor itr, itr

.align 16
.loop:

        .irpc i,12345678
            vmovdqu64 ZMM_SIZE*(\i-1)(a, itr, 1), MEM\i
        .endr

        .irpc j,012
            .irpc i,1234567
                vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR\i, DWORD_POS\j, %k\i
            .endr

            .irpc i,1234567
                vpord MEM\i, BIT_MASK\j, MEM\i{%k\i}
            .endr
			
            vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR8, DWORD_POS\j, %k1
            vpord MEM8, BIT_MASK\j, MEM8{%k1}
        .endr

        .irpc i,12345678
            vmovdqu64 MEM\i, ZMM_SIZE*(\i-1)(a, itr, 1)
        .endr

        .irpc i,12345678
            vpaddd INC, DWORDS_ITR\i, DWORDS_ITR\i
        .endr

        add $8*ZMM_SIZE, itr
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
        .irpc i,1234
            vmovdqa64  INIT_POS\i(%rip), DWORDS_ITR\i
        .endr
        vmovdqa64   DWORDS_INC(%rip), INC
        vpaddd INC, DWORDS_ITR1, DWORDS_ITR5
        vpaddd INC, DWORDS_ITR2, DWORDS_ITR6
        vpaddd INC, DWORDS_ITR3, DWORDS_ITR7
        vpaddd INC, DWORDS_ITR4, DWORDS_ITR8
		vpaddd INC, INC, INC

        LOAD_POS 0
        
        xor itr, itr

.rest_loop:

        .irpc i,12345678
            vmovdqu64 ZMM_SIZE*(\i-1)(a, itr, 1), MEM\i
        .endr

		.irpc i,1234567
			vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR\i, DWORD_POS0, %k\i
		.endr

		.irpc i,1234567
			vpord MEM\i, BIT_MASK0, MEM\i{%k\i}
		.endr
		
		vpcmpd $_MM_CMPINT_EQ, DWORDS_ITR8, DWORD_POS0, %k1
		vpord MEM8, BIT_MASK0, MEM8{%k1}

        .irpc i,12345678
            vmovdqu64 MEM\i, ZMM_SIZE*(\i-1)(a, itr, 1)
        .endr

        .irpc i,12345678
            vpaddd INC, DWORDS_ITR\i, DWORDS_ITR\i
        .endr

        add $8*ZMM_SIZE, itr
        cmp len, itr
        jl .rest_loop
    
    inc w_itr
    cmp weight, w_itr
    jl .rest_wloop
        
#endif

.exit:

    add $2*8, %rsp
    ret
.size    secure_set_bits,.-secure_set_bits

