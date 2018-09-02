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

#ifdef USE_AVX512F_INSTRUCTIONS

#ifdef CONSTANT_TIME

.local _CMP_LT_OS

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

#// CONSTANT_TIME
#else

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

# //CONSTANT_TIME
#endif

# //USE_AVX512F_INSTRUCTIONS
#elif defined(USE_AVX2_INSTRUCTIONS)

#ifdef CONSTANT_TIME


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

#define LOW_HALF_YMMS  i,0,1,2,3,4,5,6,7
#define YMM_NUM  8
#define TOTAL_YMMS_SIZE  (YMM_NUM*YMM_SIZE)

.set mask, %ymm15

.macro SUM tag inv_h_compressed res_offset
    xor itr1, itr1
.Lloop\tag:

    .irp LOW_HALF_YMMS
        vxorps %ymm\i, %ymm\i, %ymm\i
    .endr

    xor itr2, itr2

.Linner_loop\tag:

        #load position
        vbroadcastss 0x4(\inv_h_compressed, itr2, 8), mask
        mov (\inv_h_compressed, itr2, 8), tmp32
        
        #adjust loop offset
        add itr1, tmp 

        vpand (YMM_SIZE*0)(s, tmp, 1), mask, %ymm8
        vpand (YMM_SIZE*1)(s, tmp, 1), mask, %ymm9
        vpand (YMM_SIZE*2)(s, tmp, 1), mask, %ymm10
        vpand (YMM_SIZE*3)(s, tmp, 1), mask, %ymm11
        vpand (YMM_SIZE*4)(s, tmp, 1), mask, %ymm12
        vpand (YMM_SIZE*5)(s, tmp, 1), mask, %ymm13

        vpaddb %ymm0, %ymm8, %ymm0
        vpaddb %ymm1, %ymm9, %ymm1
        vpaddb %ymm2, %ymm10, %ymm2
        vpaddb %ymm3, %ymm11, %ymm3
        vpaddb %ymm4, %ymm12, %ymm4
        
        vpand (YMM_SIZE*6)(s, tmp, 1), mask, %ymm14
        vpand (YMM_SIZE*7)(s, tmp, 1), mask, %ymm15
        
        vpaddb %ymm5, %ymm13, %ymm5
        vpaddb %ymm6, %ymm14, %ymm6
        vpaddb %ymm7, %ymm15, %ymm7
        
        inc itr2
        cmp $FAKE_DV, itr2
        jl .Linner_loop\tag

    .irp LOW_HALF_YMMS
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
    xor cmp_res64, cmp_res64

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

#// CONSTANT_TIME
#else

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

# //CONSTANT_TIME
#endif

# //USE_AVX2_INSTRUCTIONS
#endif

