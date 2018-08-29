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

.text    
########################################################################################
# void gf2_muladd_4x4(uint64_t *res, const uint64_t *a, const uint64_t *b)

.set res, %rdi
.set a, %rsi
.set b, %rdx

.set A0, %xmm0
.set A1, %xmm1

.set B0, %xmm2
.set B1, %xmm3

.set X0, %xmm4
.set X1, %xmm5
.set X2, %xmm6
.set X3, %xmm7
.set X4, %xmm8
.set X5, %xmm9
.set X6, %xmm10
.set X7, %xmm11

.set XT0, %xmm12
.set XT1, %xmm13
.set XT2, %xmm14

#                              a1     a2     a3     a4
#                              b1     b2     b3     b4
#------------------------------------------------------
#                            b4a1   b4a2   b4a3   b4a4
#                     b3a1   b3a2   b3a3   b3a4
#              b2a1   b2a2   b2a3   b2a4
#       b1a1   b1a2   b1a3   b1a4
#------------------------------------------------------
#                                             a-X0=b4a4
#                                          b-X0=b4a3
#                                          b-X1=b3a4
#                                        a-X1=b4a2
#                                        a-X2=b3a3
#                                        a-X3=b2a4
#                                      b-X2=b4a1
#                                      b-X3=b3a2
#                                      b-X4=b2a3
#                                      b-X5=b1a4
#                                    a-X4=b3a1
#                                    a-X5=b2a2
#                                    a-X6=b1a3
#                                  b-X6=b2a1
#                                  b-X7=b1a2
#                                a-X7=b1a1


.globl   gf2_muladd_4x4
.hidden  gf2_muladd_4x4
.type   gf2_muladd_4x4,@function
.align  16
gf2_muladd_4x4:
    .irp i, 0, 1
      vmovdqu XMM_SIZE*\i(a), A\i
      vmovdqu XMM_SIZE*\i(b), B\i
    .endr
          
    vpclmulqdq $0x00, A0, B0, X0
    vpclmulqdq $0x00, A1, B0, X1
    vpclmulqdq $0x11, A0, B0, X2
    vpclmulqdq $0x00, A0, B1, X3

    vxorpd X1, X2, X2
    vxorpd X2, X3, X3

    vmovdqu X0, 16*0(res)
    vmovdqu X3, 16*1(res)

    vpclmulqdq $0x11, A1, B0, X4
    vpclmulqdq $0x00, A1, B1, X5
    vpclmulqdq $0x11, A0, B1, X6
    vpclmulqdq $0x11, A1, B1, X7

    vxorpd X4, X5, X5
    vxorpd X5, X6, X6

    vmovdqu X7, 16*3(res)
    vmovdqu X6, 16*2(res)

    vmovdqu 8*1(res), XT0
    vmovdqu 8*3(res), XT1
    vmovdqu 8*5(res), XT2
    
    vpclmulqdq $0x10, A0, B0, X0
    vpclmulqdq $0x01, A0, B0, X1
    vpclmulqdq $0x01, A1, B0, X2
    vpclmulqdq $0x10, A1, B0, X3
    vpclmulqdq $0x01, A0, B1, X4
    vpclmulqdq $0x10, A0, B1, X5
    vpclmulqdq $0x10, A1, B1, X6
    vpclmulqdq $0x01, A1, B1, X7

    vxorpd X2, X3, X3
    vxorpd X3, XT1, XT1
    vxorpd X4, X5, X4
    vxorpd X4, XT1, XT1

    vxorpd X0, XT0, XT0
    vxorpd X1, XT0, XT0
    
    vxorpd X6, XT2, XT2
    vxorpd X7, XT2, XT2

    vmovdqu XT0, 8*1(res)
    vmovdqu XT1, 8*3(res)
    vmovdqu XT2, 8*5(res)
    
  ret
.size   gf2_muladd_4x4,.-gf2_muladd_4x4

########################################################################################
# void karatzuba_add1(OUT const uint64_t *res, 
#                     IN const uint64_t  *a, 
#                     IN const uint64_t  *b, 
#                     IN const uint64_t  n_half, 
#                              uint64_t  *alah);
#
# The variables alah|blbh|tmp are located on the secure buffer in that order exactly!
# but we recalculate their offsets here in order not to pass them on the stack.

.set res, %rdi
.set a, %rsi
.set b, %rdx
.set n_half, %rcx
.set alah, %r8
.set blbh, %r9
.set tmp, %r10

.set itr, %r11

.set a_high, %r12
.set b_high, %r13
.set res1, %r14

# We use rax and not r15 to save push/pop
.set res2, %rax

.set a_wide,    %ymm0
.set b_wide,    %ymm1
.set res1_wide, %ymm2

.set alah_wide, %ymm3
.set blbh_wide, %ymm4
.set tmp_wide,  %ymm5

.globl  karatzuba_add1
.hidden karatzuba_add1
.type   karatzuba_add1,@function
.align  32
karatzuba_add1:
    push %r12
    push %r13
    push %r14

    lea (alah, n_half, 8), blbh
    lea (res,  n_half, 8), res1
    lea (a,    n_half, 8), a_high
    lea (b,    n_half, 8), b_high
    lea (res1, n_half, 8), res2
    lea (blbh, n_half, 8), tmp

    xor itr, itr
    jmp .lT1
.align 32
.lT1:
    vmovdqu (a, itr, 8),    a_wide
    vmovdqu (b, itr, 8),    b_wide
    vmovdqu (res1, itr, 8), res1_wide
   
    vpxor (a_high, itr, 8), a_wide, alah_wide
    vpxor (b_high, itr, 8), b_wide, blbh_wide
    vpxor (res2, itr, 8),   res1_wide, tmp_wide

    # a_low + a_high
    vmovdqu alah_wide, (alah, itr, 8)
    
    # b_low + b_high
    vmovdqu blbh_wide, (blbh, itr, 8)
    
    # Storing res1 and res2 together in one location
    # It will be used to xor "res2|res1" in the future.
    vmovdqu tmp_wide, (tmp, itr, 8)

    add $4, itr
    cmp n_half, itr
    jl .lT1

    pop %r14
    pop %r13
    pop %r12
    ret
.size karatzuba_add1,.-karatzuba_add1

   
#######################################################
# void karatzuba_add2(OUT const uint64_t *res,
#                     IN  const uint64_t *res1, 
#                     IN  const uint64_t *res2, 
#                     IN  const uint64_t *tmp, 
#                     IN  const uint64_t n_half);

.set res,    %rdi
.set res1,   %rsi
.set res2,   %rdx
.set tmp,    %rcx
.set n_half, %r8

.set res3, %r10
.set itr, %r11

.set tmp_wide, %ymm0
.set res_wide, %ymm1
.set res1_wide, %ymm2
.set res2_wide, %ymm3
.set res3_wide, %ymm4

.globl  karatzuba_add2
.hidden karatzuba_add2
.type   karatzuba_add2,@function
.align 32
karatzuba_add2:

    lea (res2, n_half, 8), res3
    
    dec n_half
    xor itr, itr
    jmp .lT2
.align    32
.lT2:
    vmovdqu (tmp, itr, 8), tmp_wide
    vmovdqu (res1, itr, 8), res1_wide
    vmovdqu (res2, itr, 8), res2_wide
    
    vpxor (res, itr, 8), tmp_wide, res_wide
    vpxor (res3, itr, 8), tmp_wide, res3_wide
    
    vpxor res_wide , res1_wide, res1_wide
    vpxor res3_wide, res2_wide, res2_wide
    
    vmovdqu res1_wide, (res1, itr, 8)
    vmovdqu res2_wide, (res2, itr, 8)

    add $4, itr
    cmp n_half, itr
    jl .lT2
    
.Lt2_end:
    
    ret
.size karatzuba_add2,.-karatzuba_add2
