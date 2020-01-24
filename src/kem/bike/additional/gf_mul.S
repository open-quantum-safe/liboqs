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
#include "defs.h"

.text    
########################################################################################
# void gf2_muladd_4x4(uint64_t *res, const uint64_t *a, const uint64_t *b)

#define res %rdi
#define a   %rsi
#define b   %rdx

#define A0 %xmm0
#define A1 %xmm1

#define B0 %xmm2
#define B1 %xmm3

#define X0 %xmm4
#define X1 %xmm5
#define X2 %xmm6
#define X3 %xmm7
#define X4 %xmm8
#define X5 %xmm9
#define X6 %xmm10
#define X7 %xmm11

#define XT0 %xmm12
#define XT1 %xmm13
#define XT2 %xmm14

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
    vmovdqu XMM_SIZE*0(a), A0
    vmovdqu XMM_SIZE*0(b), B0
    vmovdqu XMM_SIZE*1(a), A1
    vmovdqu XMM_SIZE*1(b), B1
          
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

#undef res
#undef a 
#undef b 
#undef A0
#undef A1
#undef B0
#undef B1
#undef X0
#undef X1
#undef X2
#undef X3
#undef X4
#undef X5
#undef X6
#undef X7
#undef XT0
#undef XT1
#undef XT2

########################################################################################
# void karatzuba_add1(OUT const uint64_t *res, 
#                     IN const uint64_t  *a, 
#                     IN const uint64_t  *b, 
#                     IN const uint64_t  n_half, 
#                              uint64_t  *alah);
#
# The variables alah|blbh|tmp are located on the secure buffer in that order exactly!
# but we recalculate their offsets here in order not to pass them on the stack.

#define res    %rdi
#define a      %rsi
#define b      %rdx
#define n_half %rcx
#define alah   %r8
#define blbh   %r9
#define tmp    %r10

#define itr    %r11

#define a_high %r12
#define b_high %r13
#define res1   %r14

# We use rax and not r15 to save push/pop
#define res2   %rax

#define a_wide    %ymm0
#define b_wide    %ymm1
#define res1_wide %ymm2

#define alah_wide %ymm3
#define blbh_wide %ymm4
#define tmp_wide  %ymm5

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

#undef res   
#undef a     
#undef b     
#undef n_half
#undef alah  
#undef blbh  
#undef tmp   
#undef itr   
#undef a_high
#undef b_high
#undef res1  
#undef res2  
#undef a_wide   
#undef b_wide   
#undef res1_wide
#undef alah_wide
#undef blbh_wide
#undef tmp_wide 
   
#######################################################
#EXTERNC void karatzuba_add2(OUT uint64_t *res1, 
#                            OUT uint64_t *res2, 
#                            IN  const uint64_t *res,
#                            IN  const uint64_t *tmp, 
#                            IN  const uint64_t n_half);

# Cancel previous definitions
#undef res1
#undef res2
#undef tmp
#undef n_half
#undef tmp_wide

#define res1   %rdi
#define res2   %rsi
#define res    %rdx
#define tmp    %rcx
#define n_half %r8

#define res3 %r10
#define itr %r11

#define tmp_wide %ymm0
#define res_wide %ymm1
#define res1_wide %ymm2
#define res2_wide %ymm3
#define res3_wide %ymm4

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
