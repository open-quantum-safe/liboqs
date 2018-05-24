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
#                              const uint64_t inv_h0_compact[DV],
#                              const uint64_t inv_h1_compact[DV])

.set unsat_counter, %rdi
.set s, %rsi
.set inv_h0_compact, %rdx
.set inv_h1_compact, %rcx

.set tmp32, %eax
.set tmp, %rax

.set itr1, %r10
.set itr2, %r11

#define ALL_YMMS i,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
#define YMM_NUM  16
#define TOTAL_YMMS_SIZE  (YMM_NUM*YMM_SIZE)

.macro SUM tag inv_h_compressed res_offset
    xor itr1, itr1
    xor tmp, tmp
    
.Lloop\tag:

    .irp ALL_YMMS
        vxorps %ymm\i, %ymm\i, %ymm\i
    .endr

    xor itr2, itr2

.Linner_loop\tag:

        #load position
        mov (\inv_h_compressed, itr2, 4), tmp32

        #adjust loop offset
        add itr1, tmp 

        .irp ALL_YMMS
            vpaddb (YMM_SIZE*\i)(s, tmp, 1), %ymm\i, %ymm\i
        .endr
        
        inc itr2
        cmp $DV, itr2
        jl .Linner_loop\tag

    .irp ALL_YMMS
        vmovdqu %ymm\i, \res_offset + (YMM_SIZE*\i)(unsat_counter, itr1, 1)
    .endr

    add $TOTAL_YMMS_SIZE, itr1
    cmp $R_DDQWORDS_BITS, itr1
    jnz .Lloop\tag
.endm

.globl    compute_counter_of_unsat
.hidden   compute_counter_of_unsat
.type     compute_counter_of_unsat,@function
.align    16
compute_counter_of_unsat:
    SUM h0 inv_h0_compact 0
    SUM h1 inv_h1_compact R_BITS

    ret
.size    compute_counter_of_unsat,.-compute_counter_of_unsat

#################################################
#void recompute(OUT syndrom_t* s,
#               IN const uint32_t numPositions,
#               IN const uint32_t positions[R_BITS],
#               IN const uint32_t h_compressed[DV])

#This function is optimized to w<128+16=144!

#if LEVEL==5
  #define MAX_ITER    128
  #define ITER_INC    64
  #define YMM_INDICES 0,2,4,6
#else
  #if LEVEL==3
    #define YMM_INDICES 0,2,4
    #define ITER_INC    48
    #define MAX_ITER    96
  #else
    #if LEVEL==1
      #define YMM_INDICES 0,2,4,6
      #define ITER_INC    64
      #define MAX_ITER    64
    #endif
  #endif
#endif

#define DV_REM (DV - MAX_ITER)

.set s,         %rdi
.set numPos,    %rsi
.set positions, %rdx
.set h_compressed, %rcx

.set pos_itr,  %r8
.set itr2,  %r9
.set h_itr,  %r10

.set H00,   %ymm0
.set H02,   %ymm1
.set H04,   %ymm2
.set H06,   %ymm3
.set H10,   %ymm4
.set H12,   %ymm5
.set H14,   %ymm6
.set H16,   %ymm7

.set POS,   %ymm8
.set RBITS, %ymm9
.set MASK,  %ymm10
.set RES,   %ymm11
.set MASK2, %ymm12
.set RES2,  %ymm13

.set _CMP_LT_OS, 0x1

.globl    recompute
.hidden   recompute
.type     recompute,@function
.align    16
recompute:

    #When there are no positions to flip do nothing.
    test numPos, numPos
    je .Lexit
    
    #Allocate room on the stack.
    sub $2*YMM_SIZE, %rsp
    
    #Initialize the h_compressed iterator to 0.
    xor h_itr, h_itr

    #Load rbits (32bit) to RBITS wide-reg.
    mov $R_BITS, %eax
    mov %eax, (%rsp)
    vbroadcastss (%rsp), RBITS

.Lstart:
    #Load 8(regs)*8(32bit indices)=64 (32bit indices)
    .irp i, YMM_INDICES
        vmovdqu YMM_SIZE*\i(h_compressed, h_itr, 4), H0\i
        vmovdqu YMM_SIZE*(\i+1)(h_compressed, h_itr, 4), H1\i
    .endr
    
    #initialize pos_itr
    xor pos_itr, pos_itr
    
.Lpos_loop:
    vbroadcastss (positions, pos_itr, 4), POS
    
    .irp i, YMM_INDICES
        vcmpps $_CMP_LT_OS, H0\i, POS, MASK
        vcmpps $_CMP_LT_OS, H1\i, POS, MASK2
        vpsubd H0\i, POS, RES
        vpsubd H1\i, POS, RES2

        vpand  MASK, RBITS, MASK
        vpand  MASK2, RBITS, MASK2
        
        vpaddd RES, MASK, RES
        vpaddd RES2, MASK2, RES2
        vmovdqu RES, (%rsp)
        vmovdqu RES2, YMM_SIZE(%rsp)
        
        xor itr2, itr2
.Linside_loop\i:
        mov (%rsp, itr2, 4), %eax
        xor $1, (s, %rax, 1)
        inc itr2
        cmp $16, itr2
        jne .Linside_loop\i
    .endr
    
    add $1, pos_itr
    cmp numPos, pos_itr
    jne .Lpos_loop

    add $ITER_INC, h_itr
    cmp $MAX_ITER, h_itr
    jne .Lstart

#Handle the additional w - 128 bits in h_compressed.
.Ltail:
    vmovdqu YMM_SIZE*0(h_compressed, h_itr, 4), H00
    vmovdqu YMM_SIZE*1(h_compressed, h_itr, 4), H02
    
    xor pos_itr, pos_itr
    
.Lpos_tail_loop:
    vbroadcastss (positions, pos_itr, 4), POS
    
    vcmpps $_CMP_LT_OS, H00, POS, MASK
    vcmpps $_CMP_LT_OS, H02, POS, MASK2
    vpsubd H00, POS, RES
    vpsubd H02, POS, RES2
    
    vpand  MASK, RBITS, MASK
    vpand  MASK2, RBITS, MASK2

    vpaddd RES, MASK, RES
    vpaddd RES2, MASK2, RES2

    vmovdqu RES, (%rsp)
    vmovdqu RES2, YMM_SIZE(%rsp)

    xor itr2, itr2
.Linside_tail_loop:
    mov (%rsp, itr2, 4), %eax
    xor $1, (s, %rax, 1)
    inc itr2
    cmp $DV_REM, itr2
    jne .Linside_tail_loop

    inc pos_itr
    cmp numPos, pos_itr
    jne .Lpos_tail_loop
  
    #Fix RSP offset.
    add $2*YMM_SIZE, %rsp
    
.Lexit:
    ret
.size    recompute,.-recompute
