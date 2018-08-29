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

.align 16, 0x90
MASK1:
.long 0x0c0f0e0d,0x0c0f0e0d,0x0c0f0e0d,0x0c0f0e0d
CON1:
.long 1,1,1,1

.set k256_size, 32

.text

################################################################################
# void aes256_key_expansion(OUT aes256_ks_t* ks, IN const uint8_t* key);
# The output parameter must be 16 bytes aligned!
#
#Linux ABI
.set out,      %rdi
.set in,       %rsi

.set CON,      %xmm0
.set MASK_REG, %xmm1

.set IN0,      %xmm2
.set IN1,      %xmm3

.set TMP1,     %xmm4
.set TMP2,     %xmm5

.set ZERO,     %xmm15

.macro ROUND1 in0 in1
    add $k256_size, out
    vpshufb     MASK_REG, \in1, TMP2
    vaesenclast CON,  TMP2, TMP2
    vpslld      $1,   CON,  CON
    vpslldq     $4,   \in0, TMP1
    vpxor       TMP1, \in0, \in0
    vpslldq     $4,   TMP1, TMP1
    vpxor       TMP1, \in0, \in0
    vpslldq     $4,   TMP1, TMP1	   
    vpxor       TMP1, \in0, \in0
    vpxor       TMP2, \in0, \in0
    vmovdqa     \in0, (out)
.endm

.macro ROUND2
   vpshufd     $0xff, IN0,  TMP2
   vaesenclast ZERO,  TMP2, TMP2
   vpslldq     $4,    IN1,  TMP1
   vpxor       TMP1,  IN1,  IN1
   vpslldq     $4,    TMP1, TMP1
   vpxor       TMP1,  IN1,  IN1
   vpslldq     $4,    TMP1, TMP1
   vpxor       TMP1,  IN1,  IN1
   vpxor       TMP2,  IN1,  IN1
   vmovdqa     IN1,   16(out)
.endm

.type   aes256_key_expansion,@function
.hidden aes256_key_expansion
.globl  aes256_key_expansion
aes256_key_expansion:
   vmovdqu (in),   IN0
   vmovdqu 16(in), IN1
   vmovdqa IN0,    (out)
   vmovdqa IN1,  16(out)

   vmovdqa CON1(%rip), CON
   vmovdqa MASK1(%rip), MASK_REG

   vpxor ZERO, ZERO, ZERO

   mov $6, %ax
.loop256:

   ROUND1 IN0 IN1
   dec %ax
   ROUND2
   jne .loop256

   ROUND1 IN0 IN1

   ret
.size aes256_key_expansion, .-aes256_key_expansion

