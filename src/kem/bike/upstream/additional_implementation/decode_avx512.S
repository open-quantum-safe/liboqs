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

.set tmp32, %r8d
.set tmp, %r8

.set itr1, %r10
.set itr2, %r11

#define ALL_ZMMS i,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
#define ZMM_NUM  32

.macro SUM tag inv_h_compact res_offset
    xor itr1, itr1
.Lloop\tag:

    .irp ALL_ZMMS
        vxorps %zmm\i, %zmm\i, %zmm\i
    .endr

    xor tmp, tmp
    xor itr2, itr2

.Linner_loop\tag:

        #load position
        mov (\inv_h_compact, itr2, 4), tmp32

        #adjust loop offset
        add itr1, tmp 

        .irp ALL_ZMMS
            vpaddb (ZMM_SIZE*\i)(s, tmp, 1), %zmm\i, %zmm\i
        .endr
        
        add $ALL_ZMM_SIZE, tmp
        inc itr2
        cmp $DV, itr2
        jl .Linner_loop\tag

    .irp ALL_ZMMS
        vmovdqu64 %zmm\i, \res_offset + (ZMM_SIZE*\i)(unsat_counter, itr1, 1)
    .endr

    add $ALL_ZMM_SIZE, itr1
    cmp $R_QDQWORDS_BITS, itr1
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
  #define ITER_INC    128
  #define ZMM_INDICES 0,2,4,6
#else
  #if LEVEL==3
    #define ZMM_INDICES 0,2,4
    #define ITER_INC    96
  #else
    #if LEVEL==1
      #define ZMM_INDICES 0,2
      #define ITER_INC    64
    #endif
  #endif
#endif

#define DV_REM (DV - ITER_INC)

.set s,         %rdi
.set numPos,    %rsi
.set positions, %rdx
.set h_compressed, %rcx

.set pos_itr,  %r8
.set itr2,  %r9

.set H00,   %zmm0
.set H02,   %zmm1
.set H04,   %zmm2
.set H06,   %zmm3
.set H10,   %zmm4
.set H12,   %zmm5
.set H14,   %zmm6
.set H16,   %zmm7

.set POS,   %zmm28
.set RBITS, %zmm29
.set RES,   %zmm30
.set RES2,  %zmm31

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
    sub $2*ZMM_SIZE, %rsp

    #Load rbits (32bit) to RBITS wide-reg.
    mov $R_BITS, %eax
    mov %eax, (%rsp)
    vbroadcastss (%rsp), RBITS

    #Load 8(regs)*16(32bit indices)=128 (32bit indices)
    .irp i, ZMM_INDICES
    vmovdqu64 ZMM_SIZE*\i(h_compressed), H0\i
    vmovdqu64 ZMM_SIZE*(\i+1)(h_compressed), H1\i
    .endr
    
    #initialize pos_itr
    xor pos_itr, pos_itr
    
.Lpos_loop:
    vbroadcastss (positions, pos_itr, 4), POS
    
    .irp i,ZMM_INDICES
    vcmpps $_CMP_LT_OS, H0\i, POS, %k1
    vcmpps $_CMP_LT_OS, H1\i, POS, %k2
    vpsubd H0\i, POS, RES
    vpsubd H1\i, POS, RES2
    
    vpaddd RES, RBITS, RES{%k1}
    vpaddd RES2, RBITS, RES2{%k2}
    vmovdqu64 RES, (%rsp)
    vmovdqu64 RES2, ZMM_SIZE(%rsp)
    
    xor itr2, itr2
.Linside_loop\i:
    mov (%rsp, itr2, 4), %eax
    xor $1, (s, %rax, 1)
    inc itr2
    cmp $32, itr2
    jne .Linside_loop\i
    .endr
    
    inc pos_itr
    cmp numPos, pos_itr
    jne .Lpos_loop

#Handle the additional w - 128 bits in h_compressed.
.Ltail:
    vmovdqu64 4*ITER_INC(h_compressed), H00
    xor pos_itr, pos_itr
    
.Lpos_tail_loop:
    vbroadcastss (positions, pos_itr, 4), POS
    
    vcmpps $_CMP_LT_OS, H00, POS, %k1
    vpsubd H00, POS, RES
    vpaddd RES, RBITS, RES{%k1}

    vmovdqu64 RES, (%rsp)

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
    
    add $2*ZMM_SIZE, %rsp
    
.Lexit:
    ret
.size    recompute,.-recompute

