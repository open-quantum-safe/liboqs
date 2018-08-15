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

#include "../functions_renaming.h"

.data

.align    16
mask_256:
.long 0x0c0f0e0d,0x0c0f0e0d,0x0c0f0e0d,0x0c0f0e0d
con:
.long 1,1,1,1

########################################################################
# void AES_256_Key_Expansion (OUT uint8_t *ks, IN  const uint8_t *key);

#Linux ABI
.set ks,  %rdi
.set key, %rsi

.text
.hidden AES_256_Key_Expansion
.globl  AES_256_Key_Expansion
.type   AES_256_Key_Expansion,@function
AES_256_Key_Expansion:
   vmovdqu (key), %xmm1
   vmovdqu 16(key), %xmm3
   vmovdqa %xmm1, (ks)
   vmovdqa %xmm3, 16(ks)
   vmovdqa con(%rip), %xmm0
   vmovdqa mask_256(%rip), %xmm15
   vpxor %xmm14, %xmm14, %xmm14

   mov $6, %ax
LOOP_256_VAES:

   add $32, ks
   dec %ax

   vpshufb     %xmm15, %xmm3, %xmm2
   vaesenclast %xmm0,  %xmm2, %xmm2
   vpslld      $1,     %xmm0, %xmm0
   vpslldq     $4,     %xmm1, %xmm4
   vpxor       %xmm4,  %xmm1, %xmm1
   vpslldq     $4,     %xmm4, %xmm4
   vpxor       %xmm4,  %xmm1, %xmm1
   vpslldq     $4,     %xmm4, %xmm4
   vpxor       %xmm4,  %xmm1, %xmm1
   vpxor       %xmm2,  %xmm1, %xmm1
   vmovdqa     %xmm1,  (ks)

   vpshufd     $0xff,  %xmm1, %xmm2
   vaesenclast %xmm14, %xmm2, %xmm2
   vpslldq     $4,     %xmm3, %xmm4
   vpxor       %xmm4,  %xmm3, %xmm3
   vpslldq     $4,     %xmm4, %xmm4
   vpxor       %xmm4,  %xmm3, %xmm3
   vpslldq     $4,     %xmm4, %xmm4
   vpxor       %xmm4,  %xmm3, %xmm3
   vpxor       %xmm2,  %xmm3, %xmm3
   vmovdqa     %xmm3,  16(ks)

   jne LOOP_256_VAES

   vpshufb     %xmm15, %xmm3, %xmm2
   vaesenclast %xmm0,  %xmm2, %xmm2
   vpslldq     $4,     %xmm1, %xmm4
   vpxor       %xmm4,  %xmm1, %xmm1
   vpslldq     $4,     %xmm4, %xmm4
   vpxor       %xmm4,  %xmm1, %xmm1
   vpslldq     $4,     %xmm4, %xmm4
   vpxor       %xmm4,  %xmm1, %xmm1
   vpxor       %xmm2,  %xmm1, %xmm1
   vmovdqa     %xmm1,  32(ks)

   ret
.size   AES_256_Key_Expansion,.-AES_256_Key_Expansion
