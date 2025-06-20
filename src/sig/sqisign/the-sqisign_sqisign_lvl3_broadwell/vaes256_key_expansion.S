#***************************************************************************
# This implementation is a modified version of the code,
# written by Nir Drucker and Shay Gueron
# AWS Cryptographic Algorithms Group
# (ndrucker@amazon.com, gueron@amazon.com)
#
# Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
#  
# Licensed under the Apache License, Version 2.0 (the "License").
# You may not use this file except in compliance with the License.
# A copy of the License is located at
#  
#     http://www.apache.org/licenses/LICENSE-2.0
#  
# or in the "license" file accompanying this file. This file is distributed 
# on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
# express or implied. See the License for the specific language governing 
# permissions and limitations under the License.
# The license is detailed in the file LICENSE.txt, and applies to this file.
#***************************************************************************

.intel_syntax noprefix
.data

.p2align 4, 0x90
MASK1:
.long 0x0c0f0e0d,0x0c0f0e0d,0x0c0f0e0d,0x0c0f0e0d
CON1:
.long 1,1,1,1

.set k256_size, 32

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",@progbits
#endif
.text

################################################################################
# void aes256_key_expansion(OUT aes256_ks_t* ks, IN const uint8_t* key);
# The output parameter must be 16 bytes aligned!
#
#Linux ABI
#define out rdi
#define in  rsi

#define CON      xmm0
#define MASK_REG xmm1

#define IN0      xmm2
#define IN1      xmm3

#define TMP1     xmm4
#define TMP2     xmm5

#define ZERO     xmm15

.macro ROUND1 in0 in1
    add         out,   k256_size
    vpshufb     TMP2,  \in1, MASK_REG
    aesenclast  TMP2,  CON
    vpslld      CON,   CON,  1
    vpslldq     TMP1,  \in0, 4
    vpxor       \in0,  \in0, TMP1
    vpslldq     TMP1,  TMP1, 4
    vpxor       \in0,  \in0, TMP1
    vpslldq     TMP1,  TMP1, 4
    vpxor       \in0,  \in0, TMP1
    vpxor       \in0,  \in0, TMP2
    vmovdqa     [out], \in0

.endm

.macro ROUND2
   vpshufd     TMP2,     IN0,  0xff
   aesenclast  TMP2,     ZERO
   vpslldq     TMP1,     IN1,  4
   vpxor       IN1,      IN1,  TMP1
   vpslldq     TMP1,     TMP1, 4
   vpxor       IN1,      IN1,  TMP1
   vpslldq     TMP1,     TMP1, 4
   vpxor       IN1,      IN1, TMP1
   vpxor       IN1,      IN1, TMP2
   vmovdqa     [out+16], IN1
.endm

#ifdef __APPLE__
#define AES256_KEY_EXPANSION _aes256_key_expansion
#else
#define AES256_KEY_EXPANSION aes256_key_expansion
#endif

#ifndef __APPLE__
.type   AES256_KEY_EXPANSION,@function
.hidden AES256_KEY_EXPANSION
#endif
.globl  AES256_KEY_EXPANSION
AES256_KEY_EXPANSION:
   vmovdqu IN0,      [in]
   vmovdqu IN1,      [in+16]
   vmovdqa [out],    IN0
   vmovdqa [out+16], IN1

   vmovdqa CON,      [rip+CON1]
   vmovdqa MASK_REG, [rip+MASK1]

   vpxor   ZERO, ZERO, ZERO

   mov     ax, 6
.loop256:

   ROUND1  IN0, IN1
   dec     ax
   ROUND2
   jne     .loop256

   ROUND1  IN0, IN1

   ret
#ifndef __APPLE__
.size AES256_KEY_EXPANSION, .-AES256_KEY_EXPANSION
#endif

