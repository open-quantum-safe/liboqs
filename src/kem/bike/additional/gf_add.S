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

.text    

###################################################################################
#void gf2_add(const uint64_t *res, const uint64_t *a, const uint64_t *b, const uint64_t size)

#According to Linux ABI.
#b is replaced because we need rdx.
#define res   %rdi
#define a     %rsi
#define b     %r9
#define len   %rcx

#define idx   %r8

#For copying one byte at a time.
#define tmp_b %dl

#For copying 32 bytes at a time.
#define tmp   %ymm0

.globl    gf2x_add_avx2
.hidden   gf2x_add_avx2
.type     gf2x_add_avx2,@function
.align    16
gf2x_add_avx2:
    mov len, idx
    mov %rdx, b
    shr $3, idx
    and $-4, idx
    jz .Lgf2x_avx2_add_bytes

#Adding in groups of 32 bytes (read/xor/store).
.align    16
.Lgf2x_avx2_add_loop64:
    vmovdqu -YMM_SIZE(a ,idx, 8), tmp
    vpxor -YMM_SIZE(b, idx, 8), tmp,tmp
    vmovdqu tmp, -YMM_SIZE(res, idx, 8)
    sub $4, idx
    jnz .Lgf2x_avx2_add_loop64

#Check for a reminder (less than 32 bytes).
.Lgf2x_avx2_add_bytes:
    mov len, idx
    shr $3, len
    and $-4, len
    and $0x1f, idx
    jz .Lgf2x_avx2_add_end

#update a,b,res to the last added value.
    lea (a, len, 8), a
    lea (b, len, 8), b
    lea (res, len, 8), res

#Adding one byte at a time (read/xor/store)
.align    16
.Lgf2x_avx2_add_loop8:
    movb -0x1(a ,idx, 1), tmp_b
    xorb -0x1(b, idx, 1), tmp_b
    movb tmp_b, -0x1(res, idx, 1)
    dec idx
    jnz .Lgf2x_avx2_add_loop8

.Lgf2x_avx2_add_end:
    ret
.size    gf2x_add_avx2,.-gf2x_add_avx2
