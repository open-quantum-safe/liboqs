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

.text    
#void compute_counter_of_unsat(uint8_t unsat_counter[N_BITS],
#                              const uint8_t s[R_BITS],
#                              const uint64_t inv_h0_compressed[DV],
#                              const uint64_t inv_h1_compressed[DV])

.set unsat_counter, %rdi
.set s, %rsi
.set inv_h0_compressed, %rdx
.set inv_h1_compressed, %rcx

.set tmp32, %eax
.set tmp, %rax

.set itr1, %r10
.set itr2, %r11

.set mask, %zmm31

#define LOW_HALF_ZMMS  i,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
#define ZMM_NUM  16

.macro SUM tag inv_h_compressed res_offset
    xor itr1, itr1
.Lloop\tag:

    .irp LOW_HALF_ZMMS
        vxorps %zmm\i, %zmm\i, %zmm\i
    .endr

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

    .irp LOW_HALF_ZMMS
        vmovdqu64 %zmm\i, \res_offset + (ZMM_SIZE*\i)(unsat_counter, itr1, 1)
    .endr

    add $16*ZMM_SIZE, itr1
    cmp $R_QDQWORDS_BITS, itr1
    jnz .Lloop\tag
.endm

.globl    compute_counter_of_unsat
.hidden   compute_counter_of_unsat
.type     compute_counter_of_unsat,@function
.align    16
compute_counter_of_unsat:
    SUM h0 inv_h0_compressed 0
    SUM h1 inv_h1_compressed R_BITS

    ret
.size    compute_counter_of_unsat,.-compute_counter_of_unsat


#################################################
#void find_error1(IN OUT e_t* e,
#                 OUT e_t* black_e,
#                 OUT e_t* gray_e,
#                 IN const uint8_t* upc,
#                 IN const uint32_t black_th,
#                 IN const uint32_t gray_th);

#ABI
.set e,        %rdi
.set black_e,  %rsi
.set gray_e,   %rdx
.set upc,      %rcx
.set black_th, %r8
.set gray_th,  %r9

.set val,       %bl
.set val64,     %rbx
.set itr,       %r10
.set tmp,       %r11
.set black_acc, %r12
.set gray_acc,  %r13
.set bit,       %r14
.set qw_itr,    %r15
.set n0,        %rbp

.set cmp_res,   %al
.set cmp_res64, %rax

.macro MASK_OR threshold acc
    #Compare bit 0.
    cmp   \threshold, val64
    setl  cmp_res
    dec   cmp_res64
    #or the masked bit
    mov   bit, tmp
    and   cmp_res64, tmp
    not   cmp_res64
    and   cmp_res64, val64
    xor   cmp_res64, cmp_res64
    or    tmp, \acc
.endm

.globl    find_error1
.hidden   find_error1
.type     find_error1,@function
.align    16
find_error1:
    push black_acc
    push gray_acc
    push bit
    push val64
    push qw_itr
    push n0

    xor val64, val64
    xor qw_itr, qw_itr
    xor black_acc, black_acc
    xor gray_acc, gray_acc

    mov $N0, n0
    mov $1, bit
    xor   cmp_res64, cmp_res64

.find_err1_start:
    movb (upc), val
    mov $R_BITS-1, itr
    
    MASK_OR black_th black_acc
    MASK_OR gray_th gray_acc

.find_err1_loop:

    movb (upc, itr, 1), val
    xor   cmp_res64, cmp_res64
    rol bit
    
    #Store qw after 64 iterations.
    cmp $1, bit
    jne .dont_store1

    #Update all error lists.
    movq black_acc, (black_e, qw_itr, 8)
    xorq black_acc, (e, qw_itr, 8)
    movq gray_acc, (gray_e, qw_itr, 8)
    
    #Restart the acc blocks
    xor black_acc, black_acc
    xor gray_acc, gray_acc
    
    inc qw_itr
.dont_store1:

    MASK_OR black_th black_acc
    MASK_OR gray_th gray_acc

    dec itr
    jnz .find_err1_loop

    dec n0
    jz .find_err1_end

    #Restart the process with next circulant block.
    rol bit
    lea R_BITS(upc), upc
    jmp .find_err1_start

.find_err1_end:
    shl  $3, qw_itr
    sub $N_EXTRA_BYTES, qw_itr
    shl $8*N_EXTRA_BYTES, black_acc
    shl $8*N_EXTRA_BYTES, gray_acc

    #update the final values
    xorq black_acc, (black_e, qw_itr, 1)
    xorq black_acc, (e, qw_itr, 1)
    xorq gray_acc, (gray_e, qw_itr, 1)

    pop n0
    pop qw_itr
    pop val64
    pop bit
    pop gray_acc
    pop black_acc
    ret
.size find_error1,.-find_error1

#################################################
#void find_error2(IN OUT e_t* e,
#                 OUT e_t* pos_e,
#                 IN const uint8_t* upc,
#                 IN const uint32_t threshold)
#ABI
.set e,         %rdi
.set pos_e,     %rsi
.set upc,       %rdx
.set threshold, %rcx

.set val,       %bl
.set val64,     %rbx
.set itr,       %r10
.set tmp,       %r11
.set pos_acc,   %r12
.set bit,       %r13
.set qw_itr,    %r14
.set n0,        %rbp

.globl    find_error2
.hidden   find_error2
.type     find_error2,@function
.align    16
find_error2:
    push pos_acc
    push bit
    push val64
    push qw_itr
    push n0

    xor val64, val64
    xor qw_itr, qw_itr
    xor pos_acc, pos_acc

    mov $N0, n0
    mov $1, bit
    xor cmp_res64, cmp_res64

.find_err2_start:
    movb (upc), val
    mov $R_BITS-1, itr
    
    MASK_OR threshold pos_acc

.find_err2_loop:

    movb (upc, itr, 1), val
    xor   cmp_res64, cmp_res64
    rol bit
    
    #Store qw after 64 iterations.
    cmp $1, bit
    jne .dont_store2

    #use only the positionssin the given position list
    andq (pos_e, qw_itr, 8), pos_acc
    #update the error.
    xorq pos_acc, (e, qw_itr, 8)
    xorq pos_acc, pos_acc
    
    inc qw_itr
.dont_store2:

    MASK_OR threshold pos_acc

    dec itr
    jnz .find_err2_loop

    dec n0
    jz .find_err2_end

    #Restart the process with next circulant block.
    rol bit
    lea R_BITS(upc), upc
    jmp .find_err2_start

.find_err2_end:
    shl  $3, qw_itr
    sub $N_EXTRA_BYTES, qw_itr
    shl $8*N_EXTRA_BYTES, pos_acc
   
    #update the final values
    andq (pos_e, qw_itr, 1), pos_acc
    xorq pos_acc, (e, qw_itr, 1)

    pop n0
    pop qw_itr
    pop val64
    pop bit
    pop pos_acc
    ret
.size find_error2,.-find_error2

