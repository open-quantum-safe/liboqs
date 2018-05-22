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
#include "defs.h"

.data

.align  16
ONES_MASK:
.byte 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1

BIT_MASK:
.byte 0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80,0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80
.byte 0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80,0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80

SHUF_MASK:
.byte 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0
.byte 0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1
.byte 0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2
.byte 0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3

.text    

#############################################################################################
# convert a sequence of uint8_t elements which fully uses all 8-bits of an uint8_t element to
# a sequence of uint8_t which uses just a single bit per byte (either 0 or 1).
#void convert_to_redundant_rep(OUT uint8_t* out, 
#                             IN const uint8_t * in, 
#                             IN const uint64_t length)

.set out, %rdi
.set in,  %rsi
.set len, %rdx

.set in_itr, %r8
.set in_next_itr, %r9
.set out_itr, %r10
.set tmp, %al
.set tmp2, %cl

.set DUP4_0, %ymm0
.set DUP4_1, %ymm1
.set DUP4_2, %ymm2
.set DUP4_3, %ymm3
.set DUP4_4, %ymm4
.set DUP4_5, %ymm5

.set DUP4_MASKED0, %ymm6
.set DUP4_MASKED1, %ymm7
.set DUP4_MASKED2, %ymm8
.set DUP4_MASKED3, %ymm9
.set DUP4_MASKED4, %ymm10
.set DUP4_MASKED5, %ymm11

.set ZERO, %ymm12
.set SHUF_MASK_REG, %ymm13
.set BIT_MASK_REG, %ymm14
.set ONES, %ymm15

.globl    convert_to_redundant_rep
.hidden   convert_to_redundant_rep
.type     convert_to_redundant_rep,@function
.align    16
convert_to_redundant_rep:
    shr $3, len
    inc len

    xor in_itr, in_itr
    xor out_itr, out_itr
    mov $6*4, in_next_itr
    
    vmovdqu  BIT_MASK(%rip), BIT_MASK_REG
    vmovdqu  ONES_MASK(%rip), ONES
    vmovdqu  SHUF_MASK(%rip), SHUF_MASK_REG
    vpxor ZERO, ZERO, ZERO

.loop6:
    .irpc i,012345
        vpbroadcastd 0x4*\i(in, in_itr, 1), DUP4_\i
    .endr
    
    .irpc i,012345
        vpshufb SHUF_MASK_REG, DUP4_\i, DUP4_\i
    .endr
    
    .irpc i,012345
        vpand BIT_MASK_REG, DUP4_\i, DUP4_MASKED\i
    .endr
    
    .irpc i,012345
        vpcmpeqb ZERO, DUP4_MASKED\i, DUP4_MASKED\i
    .endr
    
    .irpc i,012345
        vpaddb DUP4_MASKED\i, ONES, DUP4_MASKED\i
    .endr
    
    .irpc i,012345
        vmovdqu DUP4_MASKED\i, YMM_SIZE*\i(out, out_itr, 1)
    .endr

    add $6*4, in_itr
    add $6*4, in_next_itr
    add $6*YMM_SIZE, out_itr
    cmp len, in_next_itr
    jb .loop6

.loop1:
    movb (in, in_itr, 1), tmp

    .irpc i,12345678
      mov tmp, tmp2
      shr $1, tmp
      and $1, tmp2
      movb tmp2, (out, out_itr, 1)
      inc out_itr
    .endr

    inc in_itr

    cmp len, in_itr
    jb .loop1

    ret
.size    convert_to_redundant_rep,.-convert_to_redundant_rep

#############################################################################################
# convert a sequence of uint8_t elements which fully uses all 8-bits of an uint8_t element to
# a sequence of uint8_t which uses just a single bit per byte (either 0 or 1).
# uint64_t count_ones(IN const uint8_t* in, IN const uint32_t len)

.set in, %rdi
# len of bytes.
.set len,  %rsi

.set qw_itr,  %r8
.set val64,   %rcx
.set val8,    %cl
.set pop_res, %r9
.set len_itr, %r10

.globl    count_ones
.hidden   count_ones
.type     count_ones,@function
.align    16
count_ones:
    push len
    xor %rax, %rax
    
    mov len, qw_itr
    mov len, len_itr
    shr $3, qw_itr
    and $0x7, len_itr
    
    test qw_itr,qw_itr
    jz .co_singles_loop

.co_qw_loop:
    movq -0x8(in, qw_itr, 8), val64
    popcnt val64, pop_res
    addq pop_res, %rax
    dec qw_itr
    jnz .co_qw_loop

.co_singles:
    test len_itr, len_itr
    jz .end_co
    
    #zero upper bits val64
    xor val64, val64
    
.co_singles_loop:
    movb -0x1(in, len, 1), val8
    popcnt val64, pop_res
    addq pop_res, %rax
    dec len
    dec len_itr
    jnz .co_singles_loop

.end_co:
    pop len
    ret

.size    count_ones,.-count_ones
