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
# Convert a sequence of uint8_t elements which fully uses all 8-bits of an uint8_t element to
# a sequence of uint8_t which uses just a single bit per byte (either 0 or 1).
#void convert_to_redundant_rep(OUT uint8_t* out, 
#                             IN const uint8_t * in, 
#                             IN const uint64_t len)

#define out %rdi
#define in  %rsi
#define len %rdx

#define in_itr %r8
#define in_next_itr %r9
#define out_itr %r10
#define len_rem %r11
#define tmp %al
#define tmp2 %cl

#define DUP4_0 %ymm0
#define DUP4_1 %ymm1
#define DUP4_2 %ymm2
#define DUP4_3 %ymm3
#define DUP4_4 %ymm4
#define DUP4_5 %ymm5

#define DUP4_MASKED0 %ymm6
#define DUP4_MASKED1 %ymm7
#define DUP4_MASKED2 %ymm8
#define DUP4_MASKED3 %ymm9
#define DUP4_MASKED4 %ymm10
#define DUP4_MASKED5 %ymm11

#define ZERO          %ymm12
#define SHUF_MASK_REG %ymm13
#define BIT_MASK_REG  %ymm14
#define ONES          %ymm15

.globl    convert_to_redundant_rep
.hidden   convert_to_redundant_rep
.type     convert_to_redundant_rep,@function
.align    16
convert_to_redundant_rep:
    mov len, len_rem
    shr $3, len
    and $7, len_rem

    xor in_itr, in_itr
    xor out_itr, out_itr
    mov $6*4, in_next_itr
    
    vmovdqu  BIT_MASK(%rip), BIT_MASK_REG
    vmovdqu  ONES_MASK(%rip), ONES
    vmovdqu  SHUF_MASK(%rip), SHUF_MASK_REG
    vpxor ZERO, ZERO, ZERO

.loop6:
    vpbroadcastd 0x4*0(in, in_itr, 1), DUP4_0
    vpbroadcastd 0x4*1(in, in_itr, 1), DUP4_1
    vpbroadcastd 0x4*2(in, in_itr, 1), DUP4_2
    vpbroadcastd 0x4*3(in, in_itr, 1), DUP4_3
    vpbroadcastd 0x4*4(in, in_itr, 1), DUP4_4
    vpbroadcastd 0x4*5(in, in_itr, 1), DUP4_5
    
    vpshufb SHUF_MASK_REG, DUP4_0, DUP4_0
    vpshufb SHUF_MASK_REG, DUP4_1, DUP4_1
    vpshufb SHUF_MASK_REG, DUP4_2, DUP4_2
    vpshufb SHUF_MASK_REG, DUP4_3, DUP4_3
    vpshufb SHUF_MASK_REG, DUP4_4, DUP4_4
    vpshufb SHUF_MASK_REG, DUP4_5, DUP4_5
    
    vpand BIT_MASK_REG, DUP4_0, DUP4_MASKED0
    vpand BIT_MASK_REG, DUP4_1, DUP4_MASKED1
    vpand BIT_MASK_REG, DUP4_2, DUP4_MASKED2
    vpand BIT_MASK_REG, DUP4_3, DUP4_MASKED3
    vpand BIT_MASK_REG, DUP4_4, DUP4_MASKED4
    vpand BIT_MASK_REG, DUP4_5, DUP4_MASKED5
  
    vpcmpeqb ZERO, DUP4_MASKED0, DUP4_MASKED0
    vpcmpeqb ZERO, DUP4_MASKED1, DUP4_MASKED1
    vpcmpeqb ZERO, DUP4_MASKED2, DUP4_MASKED2
    vpcmpeqb ZERO, DUP4_MASKED3, DUP4_MASKED3
    vpcmpeqb ZERO, DUP4_MASKED4, DUP4_MASKED4
    vpcmpeqb ZERO, DUP4_MASKED5, DUP4_MASKED5
        
    vpaddb DUP4_MASKED0, ONES, DUP4_MASKED0
    vpaddb DUP4_MASKED1, ONES, DUP4_MASKED1
    vpaddb DUP4_MASKED2, ONES, DUP4_MASKED2
    vpaddb DUP4_MASKED3, ONES, DUP4_MASKED3
    vpaddb DUP4_MASKED4, ONES, DUP4_MASKED4
    vpaddb DUP4_MASKED5, ONES, DUP4_MASKED5
    
    vmovdqu DUP4_MASKED0, YMM_SIZE*0(out, out_itr, 1)
    vmovdqu DUP4_MASKED1, YMM_SIZE*1(out, out_itr, 1)
    vmovdqu DUP4_MASKED2, YMM_SIZE*2(out, out_itr, 1)
    vmovdqu DUP4_MASKED3, YMM_SIZE*3(out, out_itr, 1)
    vmovdqu DUP4_MASKED4, YMM_SIZE*4(out, out_itr, 1)
    vmovdqu DUP4_MASKED5, YMM_SIZE*5(out, out_itr, 1)

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

    movb (in, in_itr, 1), tmp
.tail:
    mov tmp, tmp2
    shr $1, tmp
    and $1, tmp2
    movb tmp2, (out, out_itr, 1)
    inc out_itr

    dec len_rem
    jnz .tail

    ret
.size    convert_to_redundant_rep,.-convert_to_redundant_rep

#############################################################################################
# convert a sequence of uint8_t elements which fully uses all 8-bits of an uint8_t element to
# a sequence of uint8_t which uses just a single bit per byte (either 0 or 1).
# uint64_t count_ones(IN const uint8_t* in, IN const uint32_t len)

#undef in
#undef len

#define in %rdi
# len of bytes.
#define len  %rsi

#define qw_itr  %r8
#define val64   %rcx
#define val8    %cl
#define pop_res %r9
#define len_itr %r10

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
