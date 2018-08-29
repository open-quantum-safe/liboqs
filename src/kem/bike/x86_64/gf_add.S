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

###################################################################################
#void gf2_add(const uint64_t *res, const uint64_t *a, const uint64_t *b, const uint64_t size)

# According to Linux ABI.
# Note that b is replaced because we need rdx.
.set res,   %rdi
.set a,     %rsi
.set b,     %r9
.set len,   %rcx

.set idx,   %r8

# For copying one byte at a time.
.set tmp_b, %dl

# For copying 32 bytes at a time.
.set tmp,   %ymm0

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

# Adding in groups of 32 bytes (read/xor/store).
.align    16
.Lgf2x_avx2_add_loop64:
    vmovdqu -YMM_SIZE(a ,idx, 8), tmp
    vpxor   -YMM_SIZE(b, idx, 8), tmp,tmp
    vmovdqu tmp, -YMM_SIZE(res, idx, 8)
    sub $4, idx
    jnz .Lgf2x_avx2_add_loop64

# Check for a reminder (less than 32 bytes).
.Lgf2x_avx2_add_bytes:
    mov len, idx
    shr $3, len
    and $-4, len
    and $0x1f, idx
    jz .Lgf2x_avx2_add_end

# Update a,b,res to the last added value.
    lea (a, len, 8), a
    lea (b, len, 8), b
    lea (res, len, 8), res

# Adding one byte at a time (read/xor/store)
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
