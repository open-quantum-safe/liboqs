##############################################################################
# Additional implementation of "BIKE: Bit Flipping Key Encapsulation". 
# Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
#
# Written by Nir Drucker and Shay Gueron
# AWS Cryptographic Algorithms Group
# (ndrucker@amazon.com, gueron@amazon.com)
#
# Multi block SHA384 AVX2/AVX512 is due to: 
# "Multi Block (MB) SHA384 for x86_64 architectures that support 
# AVX2/AVX512 instructions set" by Shay Gueron and Regev Shemy at
# https://mta.openssl.org/pipermail/openssl-dev/2016-August/008238.html
##############################################################################

#define __ASM_FILE__
#include "bike_defs.h"

.text

#ifdef USE_AVX512F_INSTRUCTIONS
.hidden sha384_512_multi_block_avx512
.globl  sha384_512_multi_block_avx512
.type   sha384_512_multi_block_avx512,@function
.align  64

.set W0,%zmm0
.set W1,%zmm1
.set W2,%zmm2
.set W3,%zmm3
.set W4,%zmm4
.set W5,%zmm5
.set W6,%zmm6
.set W7,%zmm7
.set W8,%zmm24
.set W9,%zmm25
.set W10,%zmm26
.set W11,%zmm27
.set W12,%zmm28
.set W13,%zmm29
.set W14,%zmm14
.set W15,%zmm15

.set A,%zmm16
.set B,%zmm17
.set C,%zmm18
.set D,%zmm19
.set E,%zmm20
.set F,%zmm21
.set G,%zmm22
.set H,%zmm23

.set TMP0,%zmm8
.set TMP1,%zmm9
.set TMP2,%zmm10
.set TMP3,%zmm11
.set TMP4,%zmm12
.set TMP5,%zmm13
.set TMP6,%zmm30

.set AND_BLK, %zmm31

.set TMP0y, %ymm8



.macro round A,B,C,D,E,F,G,H,i,w0,w1,w9,w14
    vprorq   $14, \E, %zmm8
    vprorq   $18, \E, %zmm9
    vprorq   $41, \E, %zmm10
    vpternlogq $0x96, %zmm10, %zmm9, %zmm8 # zmm8 = S1
    vpaddq   \w0, \H, %zmm12
    vmovdqa64   \G, %zmm13
    vmovdqa64   \A, \H
    vprorq   $28, \A, %zmm9
    vprorq   $34, \A, %zmm10
    vprorq   $39, \A, %zmm11
    vpternlogq $0x96, %zmm11, %zmm10, %zmm9 # zmm9 = S0
    vpbroadcastq   (%rbx), %zmm11
    vpternlogq  $0xd8, \E, \F, %zmm13
    vpternlogq  $0xe8, \C, \B, \H
    vpaddq   %zmm12, %zmm11, %zmm11
    vpaddq   \w9, \w0, \w0
    vpaddq   %zmm13, %zmm11, %zmm11
    vpaddq   %zmm8, %zmm11, %zmm11
    vmovdqa64  \w1, %zmm8
    vmovdqa64  \w14, %zmm13
    vprorq   $19, %zmm13, %zmm10
    vprorq   $61, %zmm13, %zmm12
    vpsrlq   $6, %zmm13, %zmm13
    vpternlogq $0x96, %zmm10, %zmm12, %zmm13
    vpaddq   %zmm11, \H, \H
    vpaddq   %zmm11, \D, \D
    vpaddq   %zmm9, \H, \H
    lea      8(%rbx), %rbx
    # Calculate sigma0
    vprorq   $1, %zmm8, %zmm11
    vprorq   $8, %zmm8, %zmm12
    vpsrlq   $7, %zmm8, %zmm8
    vpternlogq $0x96, %zmm11, %zmm12, %zmm8
    vpaddq   %zmm13, \w0, \w0
    vpaddq   %zmm8, \w0, \w0
.endm

.macro round_last A,B,C,D,E,F,G,H,i,w0
    vprorq   $14, \E, %zmm8
    vprorq   $18, \E, %zmm9
    vprorq   $41, \E, %zmm10
    vpternlogq $0x96, %zmm10, %zmm9, %zmm8
    vpaddq   \w0, \H, %zmm12
    vprorq   $28, \A, %zmm9
    vprorq   $34, \A, %zmm10
    vprorq   $39, \A, %zmm11
    vpternlogq $0x96, %zmm11, %zmm10, %zmm9
    vpbroadcastq   (%rbx), %zmm11
    vmovdqa64   \G, %zmm10
    vpternlogq  $0xd8, \E, \F, %zmm10
    vpaddq   %zmm12, %zmm11, %zmm11
    lea      8(%rbx), %rbx
    vpaddq   %zmm10, %zmm11, %zmm11
    vmovdqa64   \A, \H
    vpternlogq  $0xe8, \C, \B, \H
    vpaddq   %zmm8, %zmm11, %zmm11
    vpaddq   %zmm11, \D, \D
    vpaddq   %zmm11, \H, \H
    vpaddq   %zmm9, \H, \H
.endm

sha384_512_multi_block_avx512:
.align  32

#rdi - hashs
#rsi - buff pointers
#rdx - min length
_avx512_shortcut:

    push  %rbx
    push  %rbp
    push  %r12
    push  %r13
    push  %r14
    push  %r15
    xor %r14, %r14
    xor %r13, %r13
    mov   %rsp, %rbp     # copy %rsp
    movl  8(%rsi), %edx #right after the pointer of the buffer there is the len
    movl  8+16*7(%rsi), %r14d #since only last buffer could be different on size than others and by 1
    cmpl %r14d, %edx #if equal it means no need for last block handling
    je .Lbody_avx512 #we handle last iteration differently
    mov $1, %r13
    jb .Llast_zero
    movl %edx, %r13d
    sub %r14d, %r13d
    vmovdqu64 and_last_block(%rip), AND_BLK #if above it means last block is less
    jmp .Lbody_avx512
.Llast_zero:
    vmovdqu64 and_last_block_not(%rip), AND_BLK #if lower it means last block is larger than the rest and we take him as our loop var
    movq %r14, %rdx
# first step - load the message, taking a word of each message, into appropriate
# location inside an zmm register
.Lbody_avx512:
    xor   %r8, %r8
    mov $204, %r9
    kmovb %r9d, %k2
    #load the values of H
    vmovdqu64   64*0(%rdi), A
    vmovdqu64   64*1(%rdi), B
    vmovdqu64   64*2(%rdi), C
    vmovdqu64   64*3(%rdi), D
    vmovdqu64   64*4(%rdi), E
    vmovdqu64   64*5(%rdi), F
    vmovdqu64   64*6(%rdi), G
    vmovdqu64   64*7(%rdi), H
    xor  %r9, %r9
    movq   $64, %r10
    jmp   _L_outer_loop
.align 64
_L_outer_loop:
    movq    16*0(%rsi), %r8
    vmovdqu64   (%r8, %r9), W0
    vmovdqu64   (%r8, %r10), W8
    movq    16*1(%rsi), %r8
    vmovdqu64   (%r8, %r9), W1
    vmovdqu64   (%r8, %r10), W9
    movq    16*2(%rsi), %r8
    vmovdqu64   (%r8, %r9), W2
    vmovdqu64   (%r8, %r10), W10
    movq    16*3(%rsi), %r8
    vmovdqu64   (%r8, %r9), W3
    vmovdqu64   (%r8, %r10), W11
    movq    16*4(%rsi), %r8
    vmovdqu64   (%r8, %r9), W4
    vmovdqu64   (%r8, %r10), W12
    movq    16*5(%rsi), %r8
    vmovdqu64   (%r8, %r9), W5
    vmovdqu64   (%r8, %r10), W13
    movq    16*6(%rsi), %r8
    vmovdqu64   (%r8, %r9), W6
    vmovdqu64   (%r8, %r10), W14
    movq    16*7(%rsi), %r8
    vmovdqu64   (%r8, %r9), W7
    vmovdqu64   (%r8, %r10), W15

    add $128, %r9
    add $128, %r10

    vpunpcklqdq  W1, W0, TMP0
    vpermq $0x4e, TMP0, TMP4
    vpunpcklqdq  W3, W2, TMP1
    vpermq $0x4e, TMP1, TMP5
    vpblendmq TMP1 ,TMP4, TMP1{%k2}
    vpblendmq TMP5 ,TMP0, TMP0{%k2}
    vpunpcklqdq  W5, W4, TMP2
    vpunpcklqdq  W7, W6, TMP3
    vpermq $0x4e, TMP3, TMP6
    vpermq  $0x4e, TMP2, TMP4
    vpblendmq TMP3, TMP4 ,TMP4{%k2}
    vpblendmq TMP6, TMP2, TMP2{%k2}
    vpunpckhqdq W1, W0, TMP3
    vshufi64x2 $0x44, TMP2, TMP0, W0
    vshufi64x2 $0xee, TMP2, TMP0, TMP0
    vpunpckhqdq  W3, W2, TMP2
    vpermq $0x4e, TMP3, W3
    vpunpckhqdq  W5, W4, TMP6
    vmovdqu64 TMP0, W4
    vpunpckhqdq  W7, W6, TMP0
    vpermq $0x4e, TMP6, W1
    vshufi64x2 $0x44, TMP4, TMP1, W2
    vshufi64x2 $0xee, TMP4, TMP1, W6
    vpblendmq TMP2, W3 ,TMP1{%k2}
    vpblendmq TMP0, W1, W7 {%k2}
    vpermq $0x4e, TMP2, TMP2
    vpermq $0x4e, TMP0, TMP0
    vpblendmq TMP2, TMP3 ,TMP4{%k2}
    vpblendmq TMP0, TMP6 ,W5{%k2}
    vshufi64x2 $0x44, W7, TMP1, W3
    vshufi64x2 $0xee, W7, TMP1, W7
    vshufi64x2 $0x44, W5, TMP4, W1
    vshufi64x2 $0xee, W5, TMP4, W5
    vpunpcklqdq  W9, W8, TMP0
    vpermq $0x4e, TMP0, TMP4
    vpunpcklqdq  W11, W10, TMP1
    vpermq $0x4e, TMP1, TMP5
    vpblendmq TMP1 ,TMP4, TMP1{%k2}
    vpblendmq TMP5 ,TMP0, TMP0{%k2}
    vpunpcklqdq  W13, W12, TMP2
    vpunpcklqdq  W15, W14, TMP3
    vpermq $0x4e, TMP3, TMP6
    vpermq  $0x4e, TMP2, TMP4
    vpblendmq TMP3, TMP4 ,TMP4{%k2}
    vpblendmq TMP6, TMP2, TMP2{%k2}
    vpunpckhqdq W9, W8, TMP3
    vshufi64x2 $0x44, TMP2, TMP0, W8
    vshufi64x2 $0xee, TMP2, TMP0, TMP0
    vpunpckhqdq  W11, W10, TMP2
    vpermq $0x4e, TMP3, W11
    vpunpckhqdq  W13, W12, TMP6
    vmovdqu64 TMP0, W12
    vpunpckhqdq  W15, W14, TMP0
    vpermq $0x4e, TMP6, W9
    vshufi64x2 $0x44, TMP4, TMP1, W10
    vshufi64x2 $0xee, TMP4, TMP1, W14
    vpblendmq TMP2, W11 ,TMP1{%k2}
    vpblendmq TMP0, W9, W15 {%k2}
    vpermq $0x4e, TMP2, TMP2
    vpermq $0x4e, TMP0, TMP0
    vpblendmq TMP2, TMP3 ,TMP4{%k2}
    vpblendmq TMP0, TMP6 ,W13{%k2}
    vshufi64x2 $0x44, W15, TMP1, W11
    vshufi64x2 $0xee, W15, TMP1, W15
    vshufi64x2 $0x44, W13, TMP4, W9
    vshufi64x2 $0xee, W13, TMP4, W13
    vpshufb bswap_mask64_512(%rip), W0, W0
    vpshufb bswap_mask64_512(%rip), W1, W1
    vpshufb bswap_mask64_512(%rip), W2, W2
    vpshufb bswap_mask64_512(%rip), W3, W3
    vpshufb bswap_mask64_512(%rip), W4, W4
    vpshufb bswap_mask64_512(%rip), W5, W5
    vpshufb bswap_mask64_512(%rip), W6, W6
    vpshufb bswap_mask64_512(%rip), W7, W7
    vpshufb bswap_mask64_512(%rip), W8, W8
    vpshufb bswap_mask64_512(%rip), W9, W9
    vpshufb bswap_mask64_512(%rip), W10, W10
    vpshufb bswap_mask64_512(%rip), W11, W11
    vpshufb bswap_mask64_512(%rip), W12, W12
    vpshufb bswap_mask64_512(%rip), W13, W13
    vpshufb bswap_mask64_512(%rip), W14, W14
    vpshufb bswap_mask64_512(%rip), W15, W15
    #load address of the K table
    lea K_table_512(%rip), %rbx
    mov   $4, %rcx
    jmp   _L_inner_loop

.align 16
_L_inner_loop:

    round A, B, C, D, E, F, G, H, 0, W0, W1, W9, W14
    round H, A, B, C, D, E, F, G, 1, W1, W2, W10, W15
    round G, H, A, B, C, D, E, F, 2, W2, W3, W11, W0
    round F, G, H, A, B, C, D, E, 3, W3, W4, W12, W1
    round E, F, G, H, A, B, C, D, 4, W4, W5, W13, W2
    round D, E, F, G, H, A, B, C, 5, W5, W6, W14, W3
    round C, D, E, F, G, H, A, B, 6, W6, W7, W15, W4
    round B, C, D, E, F, G, H, A, 7, W7, W8, W0, W5

    round A, B, C, D, E, F, G, H, 8, W8, W9, W1, W6
    round H, A, B, C, D, E, F, G, 9, W9, W10, W2, W7
    round G, H, A, B, C, D, E, F, 10, W10, W11, W3, W8
    round F, G, H, A, B, C, D, E, 11, W11, W12, W4, W9
    round E, F, G, H, A, B, C, D, 12, W12, W13, W5, W10
    round D, E, F, G, H, A, B, C, 13, W13, W14, W6, W11
    round C, D, E, F, G, H, A, B, 14, W14, W15, W7, W12
    round B, C, D, E, F, G, H, A, 15, W15, W0, W8, W13

    dec   %rcx
    jne   _L_inner_loop

    round_last A, B, C, D, E, F, G, H, 0, W0
    round_last H, A, B, C, D, E, F, G, 1, W1
    round_last G, H, A, B, C, D, E, F, 2, W2
    round_last F, G, H, A, B, C, D, E, 3, W3
    round_last E, F, G, H, A, B, C, D, 4, W4
    round_last D, E, F, G, H, A, B, C, 5, W5
    round_last C, D, E, F, G, H, A, B, 6, W6
    round_last B, C, D, E, F, G, H, A, 7, W7
    round_last A, B, C, D, E, F, G, H, 8, W8
    round_last H, A, B, C, D, E, F, G, 9, W9
    round_last G, H, A, B, C, D, E, F, 10, W10
    round_last F, G, H, A, B, C, D, E, 11, W11
    round_last E, F, G, H, A, B, C, D, 12, W12
    round_last D, E, F, G, H, A, B, C, 13, W13
    round_last C, D, E, F, G, H, A, B, 14, W14
    round_last B, C, D, E, F, G, H, A, 15, W15

    #update hash values
    cmpl %r13d, %edx
    jne CONTINUE
    dec %r13
    vpandq  AND_BLK, A, A
    vpandq  AND_BLK, B, B
    vpandq  AND_BLK, C, C
    vpandq  AND_BLK, D, D
    vpandq  AND_BLK, E, E
    vpandq  AND_BLK, F, F
    vpandq  AND_BLK, G, G
    vpandq  AND_BLK, H, H
    #update hash values

CONTINUE:
    vpaddq  64*0(%rdi), A, A
    vpaddq  64*1(%rdi), B, B
    vpaddq  64*2(%rdi), C, C
    vpaddq  64*3(%rdi), D, D
    vpaddq  64*4(%rdi), E, E
    vpaddq  64*5(%rdi), F, F
    vpaddq  64*6(%rdi), G, G
    vpaddq  64*7(%rdi), H, H

END:
    vmovdqu64  A, 0*64(%rdi)
    vmovdqu64  B, 1*64(%rdi)
    vmovdqu64  C, 2*64(%rdi)
    vmovdqu64  D, 3*64(%rdi)
    vmovdqu64  E, 4*64(%rdi)
    vmovdqu64  F, 5*64(%rdi)
    vmovdqu64  G, 6*64(%rdi)
    vmovdqu64  H, 7*64(%rdi)

    dec   %rdx
    jne   _L_outer_loop
    mov   %rbp, %rsp
    pop   %r15
    pop   %r14
    pop   %r13
    pop   %r12
    pop   %rbp
    pop   %rbx
.Lepilogue_avx512:
    .byte   0xf3,0xc3
    ret
.size   sha384_512_multi_block_avx512,.-sha384_512_multi_block_avx512

# // USE_AVX512F_INSTRUCTIONS
#else

.hidden sha384_512_multi_block_avx2
.globl  sha384_512_multi_block_avx2
.type   sha384_512_multi_block_avx2,@function
.align  64

.set W0_y,%ymm0
.set W1_y,%ymm1
.set W2_y,%ymm2
.set W3_y,%ymm3
.set TMP0_y,%ymm4
.set TMP1_y,%ymm5
.set TMP2_y,%ymm6
.set TMP3_y,%ymm7

.set A_y,%ymm8
.set B_y,%ymm9
.set C_y,%ymm10
.set D_y,%ymm11
.set E_y,%ymm12
.set F_y,%ymm13
.set G_y,%ymm14
.set H_y,%ymm15
.set AND_BLK_y, %ymm7

.macro vprorq_ymm i, j, in, out
    vpsrlq $\i, \in, TMP3_y
    vpsllq $\j, \in, \out
    vpor TMP3_y, \out, \out
.endm

.macro round_y A,B,C,D,E,F,G,H,i,w0,w1,w9,w14

    vmovdqu \w0*32(%rsp), W0_y
    vprorq_ymm   14, 50, \E, TMP0_y
    vprorq_ymm   18, 46, \E, TMP1_y
    vprorq_ymm   41, 23, \E, TMP2_y
    vpxor TMP0_y, TMP1_y, TMP0_y
    vpxor TMP0_y, TMP2_y, TMP0_y

    vpaddq   W0_y, \H, W2_y
    vmovdqa   \G, TMP2_y
    vmovdqa   \A, \H

    vprorq_ymm   28, 36, \A, TMP1_y
    vprorq_ymm   34, 30, \A, W1_y
    vpxor TMP1_y, W1_y, TMP1_y
    vprorq_ymm   39, 25, \A, W1_y
    vpxor TMP1_y, W1_y, TMP1_y

    vmovdqu ones(%rip), W3_y
    vpxor \E, W3_y, W3_y
    vpand W3_y, TMP2_y, TMP2_y
    vpand \F, \E, W1_y
    vpxor W1_y, TMP2_y, TMP2_y


    vpand \H, \B, W1_y
    vpand \H, \C, W3_y
    vpxor W1_y, W3_y, \H
    vpand \B, \C, W1_y
    vpxor W1_y, \H, \H

    vpbroadcastq   (%rbx), W3_y
    vpaddq   W2_y, W3_y, W3_y
    vmovdqu \w9*32(%rsp), W2_y
    vpaddq   W2_y , W0_y, W0_y

    vpaddq   TMP2_y, W3_y, TMP2_y
    vpaddq   TMP0_y, TMP2_y, TMP2_y

    vmovdqu \w1*32(%rsp), W1_y # zmm8
    vmovdqu \w14*32(%rsp), W3_y #zmm13

    vprorq_ymm   19, 45, W3_y, TMP0_y
    vprorq_ymm   61, 3, W3_y, W2_y
    vpxor TMP0_y, W2_y, TMP0_y
    vpsrlq   $6, W3_y, W3_y
    vpxor TMP0_y, W3_y, TMP0_y #TMP0 = zmm13

    vpaddq   TMP2_y, \H, \H
    vpaddq   TMP2_y, \D, \D
    vpaddq   TMP1_y, \H, \H

    lea      8(%rbx), %rbx

    # Calculate sigma0
    vprorq_ymm   1, 63, W1_y, W2_y
    vprorq_ymm   8, 56, W1_y, W3_y
    vpxor W2_y, W3_y, W3_y
    vpsrlq   $7, W1_y, W1_y
    vpxor W1_y, W3_y, W1_y

    vpaddq   TMP0_y, W0_y, W0_y
    vpaddq   W1_y, W0_y, W0_y

    vmovdqu   W0_y, \w0*32(%rsp)
.endm

.macro round_last_y A,B,C,D,E,F,G,H,i,w0

    vmovdqu \w0*32(%rsp), W0_y

    vprorq_ymm   14, 50, \E, TMP0_y
    vprorq_ymm   18, 46, \E, TMP1_y
    vprorq_ymm   41, 23, \E, TMP2_y
    vpxor TMP0_y, TMP1_y, TMP0_y #
    vpxor TMP0_y, TMP2_y, TMP0_y # TMP0_y = S1

    vpaddq   W0_y, \H, W2_y


    vprorq_ymm   28, 36, \A, TMP1_y
    vprorq_ymm   34, 30, \A, W1_y
    vpxor TMP1_y, W1_y, TMP1_y
    vprorq_ymm   39, 25, \A, W1_y
    vpxor TMP1_y, W1_y, TMP1_y


    vmovdqa   \G, TMP2_y

    vmovdqu ones(%rip), W3_y
    vpxor \E, W3_y, W3_y
    vpand W3_y, TMP2_y, TMP2_y
    vpand \F, \E, W3_y
    vpxor W3_y, TMP2_y, TMP2_y

    vpbroadcastq   (%rbx), W3_y
    vpaddq   W2_y, W3_y, W3_y
    lea      8(%rbx), %rbx

    vpaddq   TMP2_y, W3_y, W3_y

    vmovdqa   \A, \H
    vpand \H, \B, TMP2_y
    vpand \H, \C, TMP3_y
    vpxor TMP2_y, TMP3_y, \H
    vpand \B, \C, TMP3_y
    vpxor TMP3_y, \H, \H



    vpaddq  TMP0_y, W3_y, W3_y
    vpaddq  W3_y, \D, \D
    vpaddq  W3_y, \H, \H
    vpaddq  TMP1_y, \H, \H

.endm

sha384_512_multi_block_avx2:

    push  %rbx
    push  %rbp
    push  %r8
    push  %r9
    push  %r10
    push  %r11
    push  %r12
    push  %r13
    push  %r14
    push  %r15
    xor %r14, %r14
    xor %r13, %r13
    movq   %rsp, %rbp     # copy %rsp
    movq  %rcx, %rsp
    movl  8(%rsi), %edx #right after the pointer of the buffer there is the len
    movl  8+16*3(%rsi), %r14d #since only last buffer could be different on size than others and by 1
    cmpl %r14d, %edx #if equal it means no need for last block handling
    je .Lbody_avx2 #we handle last iteration differently
    mov $1, %r13
    jb .Llast_zero_y
    movl %edx, %r13d
    sub %r14d, %r13d
    vmovdqu and_last_block_256(%rip), AND_BLK_y #if above it means last block is less
    vmovdqu AND_BLK_y, 16*32(%rsp)
    jmp .Lbody_avx2
.Llast_zero_y:
    vmovdqu and_last_block_not_256(%rip), AND_BLK_y #if lower it means last block is larger than the rest and we take him as our loop var
    vmovdqu AND_BLK_y, 16*32(%rsp)
    movq %r14, %rdx
# first step - load the message, taking a word of each message, into appropriate
# location inside an zmm register
.Lbody_avx2:
    xor   %r8, %r8
    #load the values of H
    vmovdqu   64*0(%rdi), A_y
    vmovdqu   64*1(%rdi), B_y
    vmovdqu   64*2(%rdi), C_y
    vmovdqu   64*3(%rdi), D_y
    vmovdqu   64*4(%rdi), E_y
    vmovdqu   64*5(%rdi), F_y
    vmovdqu   64*6(%rdi), G_y
    vmovdqu   64*7(%rdi), H_y
    xor  %r9, %r9
    movq    16*0(%rsi), %r8
    movq    16*1(%rsi), %r9
    movq    16*2(%rsi), %r10
    movq    16*3(%rsi), %r11
    xor %r12, %r12
    jmp   _L_outer_loop_y
.align 64
_L_outer_loop_y:
    vmovdqu   (%r8, %r12), W0_y
    vmovdqu   (%r9, %r12), W1_y
    vmovdqu   (%r10, %r12), W2_y
    vmovdqu   (%r11, %r12), W3_y

    vpunpcklqdq W1_y, W0_y, TMP0_y
    vpunpckhqdq W1_y, W0_y, TMP1_y
    vpunpcklqdq W3_y, W2_y, TMP2_y
    vpunpckhqdq W3_y, W2_y, TMP3_y

    vperm2i128 $0x20, TMP2_y, TMP0_y, W0_y
    vperm2i128 $0x20, TMP3_y, TMP1_y, W1_y
    vperm2i128 $0x31, TMP2_y, TMP0_y, W2_y
    vperm2i128 $0x31, TMP3_y, TMP1_y, W3_y


    add $32, %r12

    vpshufb bswap_mask64_256(%rip), W0_y, W0_y
    vpshufb bswap_mask64_256(%rip), W1_y, W1_y
    vpshufb bswap_mask64_256(%rip), W2_y, W2_y
    vpshufb bswap_mask64_256(%rip), W3_y, W3_y

    vmovdqu W0_y, 0*32(%rsp)
    vmovdqu W1_y, 1*32(%rsp)
    vmovdqu W2_y, 2*32(%rsp)
    vmovdqu W3_y, 3*32(%rsp)

    vmovdqu   (%r8, %r12), W0_y
    vmovdqu   (%r9, %r12), W1_y
    vmovdqu   (%r10, %r12), W2_y
    vmovdqu   (%r11, %r12), W3_y

    vpunpcklqdq W1_y, W0_y, TMP0_y
    vpunpckhqdq W1_y, W0_y, TMP1_y
    vpunpcklqdq W3_y, W2_y, TMP2_y
    vpunpckhqdq W3_y, W2_y, TMP3_y

    vperm2i128 $0x20, TMP2_y, TMP0_y, W0_y
    vperm2i128 $0x20, TMP3_y, TMP1_y, W1_y
    vperm2i128 $0x31, TMP2_y, TMP0_y, W2_y
    vperm2i128 $0x31, TMP3_y, TMP1_y, W3_y

    add $32, %r12

    vpshufb bswap_mask64_256(%rip), W0_y, W0_y
    vpshufb bswap_mask64_256(%rip), W1_y, W1_y
    vpshufb bswap_mask64_256(%rip), W2_y, W2_y
    vpshufb bswap_mask64_256(%rip), W3_y, W3_y

    vmovdqu W0_y, 4*32(%rsp)
    vmovdqu W1_y, 5*32(%rsp)
    vmovdqu W2_y, 6*32(%rsp)
    vmovdqu W3_y, 7*32(%rsp)

    vmovdqu   (%r8, %r12), W0_y
    vmovdqu   (%r9, %r12), W1_y
    vmovdqu   (%r10, %r12), W2_y
    vmovdqu   (%r11, %r12), W3_y

    vpunpcklqdq W1_y, W0_y, TMP0_y
    vpunpckhqdq W1_y, W0_y, TMP1_y
    vpunpcklqdq W3_y, W2_y, TMP2_y
    vpunpckhqdq W3_y, W2_y, TMP3_y

    vperm2i128 $0x20, TMP2_y, TMP0_y, W0_y
    vperm2i128 $0x20, TMP3_y, TMP1_y, W1_y
    vperm2i128 $0x31, TMP2_y, TMP0_y, W2_y
    vperm2i128 $0x31, TMP3_y, TMP1_y, W3_y

    add $32, %r12

    vpshufb bswap_mask64_256(%rip), W0_y, W0_y
    vpshufb bswap_mask64_256(%rip), W1_y, W1_y
    vpshufb bswap_mask64_256(%rip), W2_y, W2_y
    vpshufb bswap_mask64_256(%rip), W3_y, W3_y

    vmovdqu W0_y, 8*32(%rsp)
    vmovdqu W1_y, 9*32(%rsp)
    vmovdqu W2_y, 10*32(%rsp)
    vmovdqu W3_y, 11*32(%rsp)

    vmovdqu   (%r8, %r12), W0_y
    vmovdqu   (%r9, %r12), W1_y
    vmovdqu   (%r10, %r12), W2_y
    vmovdqu   (%r11, %r12), W3_y

    vpunpcklqdq W1_y, W0_y, TMP0_y
    vpunpckhqdq W1_y, W0_y, TMP1_y
    vpunpcklqdq W3_y, W2_y, TMP2_y
    vpunpckhqdq W3_y, W2_y, TMP3_y

    vperm2i128 $0x20, TMP2_y, TMP0_y, W0_y
    vperm2i128 $0x20, TMP3_y, TMP1_y, W1_y
    vperm2i128 $0x31, TMP2_y, TMP0_y, W2_y
    vperm2i128 $0x31, TMP3_y, TMP1_y, W3_y

    add $32, %r12

    vpshufb bswap_mask64_256(%rip), W0_y, W0_y
    vpshufb bswap_mask64_256(%rip), W1_y, W1_y
    vpshufb bswap_mask64_256(%rip), W2_y, W2_y
    vpshufb bswap_mask64_256(%rip), W3_y, W3_y

    vmovdqu W0_y, 12*32(%rsp)
    vmovdqu W1_y, 13*32(%rsp)
    vmovdqu W2_y, 14*32(%rsp)
    vmovdqu W3_y, 15*32(%rsp)

    #load address of the K table
    lea K_table_512(%rip), %rbx
    mov   $4, %rcx
    jmp   _L_inner_loop_y

.align 16
_L_inner_loop_y:

    round_y A_y, B_y, C_y, D_y, E_y, F_y, G_y, H_y, 0, 0, 1, 9, 14
    round_y H_y, A_y, B_y, C_y, D_y, E_y, F_y, G_y, 1, 1, 2, 10, 15
    round_y G_y, H_y, A_y, B_y, C_y, D_y, E_y, F_y, 2, 2, 3, 11, 0
    round_y F_y, G_y, H_y, A_y, B_y, C_y, D_y, E_y, 3, 3, 4, 12, 1
    round_y E_y, F_y, G_y, H_y, A_y, B_y, C_y, D_y, 4, 4, 5, 13, 2
    round_y D_y, E_y, F_y, G_y, H_y, A_y, B_y, C_y, 5, 5, 6, 14, 3
    round_y C_y, D_y, E_y, F_y, G_y, H_y, A_y, B_y, 6, 6, 7, 15, 4
    round_y B_y, C_y, D_y, E_y, F_y, G_y, H_y, A_y, 7, 7, 8, 0, 5

    round_y A_y, B_y, C_y, D_y, E_y, F_y, G_y, H_y, 8, 8, 9, 1, 6
    round_y H_y, A_y, B_y, C_y, D_y, E_y, F_y, G_y, 9, 9, 10,2, 7
    round_y G_y, H_y, A_y, B_y, C_y, D_y, E_y, F_y, 10, 10, 11, 3, 8
    round_y F_y, G_y, H_y, A_y, B_y, C_y, D_y, E_y, 11, 11, 12, 4, 9
    round_y E_y, F_y, G_y, H_y, A_y, B_y, C_y, D_y, 12, 12, 13, 5, 10
    round_y D_y, E_y, F_y, G_y, H_y, A_y, B_y, C_y, 13, 13, 14, 6, 11
    round_y C_y, D_y, E_y, F_y, G_y, H_y, A_y, B_y, 14, 14, 15, 7, 12
    round_y B_y, C_y, D_y, E_y, F_y, G_y, H_y, A_y, 15, 15, 0, 8, 13

    dec   %rcx
    jne   _L_inner_loop_y

    round_last_y A_y, B_y, C_y, D_y, E_y, F_y, G_y, H_y, 0, 0
    round_last_y H_y, A_y, B_y, C_y, D_y, E_y, F_y, G_y, 1, 1
    round_last_y G_y, H_y, A_y, B_y, C_y, D_y, E_y, F_y, 2, 2
    round_last_y F_y, G_y, H_y, A_y, B_y, C_y, D_y, E_y, 3, 3
    round_last_y E_y, F_y, G_y, H_y, A_y, B_y, C_y, D_y, 4, 4
    round_last_y D_y, E_y, F_y, G_y, H_y, A_y, B_y, C_y, 5, 5
    round_last_y C_y, D_y, E_y, F_y, G_y, H_y, A_y, B_y, 6, 6
    round_last_y B_y, C_y, D_y, E_y, F_y, G_y, H_y, A_y, 7, 7
    round_last_y A_y, B_y, C_y, D_y, E_y, F_y, G_y, H_y, 8, 8
    round_last_y H_y, A_y, B_y, C_y, D_y, E_y, F_y, G_y, 9, 9
    round_last_y G_y, H_y, A_y, B_y, C_y, D_y, E_y, F_y, 10, 10
    round_last_y F_y, G_y, H_y, A_y, B_y, C_y, D_y, E_y, 11, 11
    round_last_y E_y, F_y, G_y, H_y, A_y, B_y, C_y, D_y, 12, 12
    round_last_y D_y, E_y, F_y, G_y, H_y, A_y, B_y, C_y, 13, 13
    round_last_y C_y, D_y, E_y, F_y, G_y, H_y, A_y, B_y, 14, 14
    round_last_y B_y, C_y, D_y, E_y, F_y, G_y, H_y, A_y, 15, 15


    #update hash values
    cmpl %r13d, %edx
    jne CONTINUE_y
    vmovdqu 16*32(%rsp), AND_BLK_y
    dec %r13
    vpand  AND_BLK_y, A_y, A_y
    vpand  AND_BLK_y, B_y, B_y
    vpand  AND_BLK_y, C_y, C_y
    vpand  AND_BLK_y, D_y, D_y
    vpand  AND_BLK_y, E_y, E_y
    vpand  AND_BLK_y, F_y, F_y
    vpand  AND_BLK_y, G_y, G_y
    vpand  AND_BLK_y, H_y, H_y
    #update hash values

CONTINUE_y:
    vpaddq  64*0(%rdi), A_y, A_y
    vpaddq  64*1(%rdi), B_y, B_y
    vpaddq  64*2(%rdi), C_y, C_y
    vpaddq  64*3(%rdi), D_y, D_y
    vpaddq  64*4(%rdi), E_y, E_y
    vpaddq  64*5(%rdi), F_y, F_y
    vpaddq  64*6(%rdi), G_y, G_y
    vpaddq  64*7(%rdi), H_y, H_y

END_y:
    vmovdqu  A_y, 0*64(%rdi)
    vmovdqu  B_y, 1*64(%rdi)
    vmovdqu  C_y, 2*64(%rdi)
    vmovdqu  D_y, 3*64(%rdi)
    vmovdqu  E_y, 4*64(%rdi)
    vmovdqu  F_y, 5*64(%rdi)
    vmovdqu  G_y, 6*64(%rdi)
    vmovdqu  H_y, 7*64(%rdi)


    dec   %rdx
    jne   _L_outer_loop_y
    movq   %rbp, %rsp
    pop   %r15
    pop   %r14
    pop   %r13
    pop   %r12
    pop   %r11
    pop   %r10
    pop   %r9
    pop   %r8
    pop   %rbp
    pop   %rbx
.Lepilogue_avx2:
    .byte   0xf3,0xc3
    ret
.size   sha384_512_multi_block_avx2,.-sha384_512_multi_block_avx2

#endif

.data 

.align 64
and_last_block_not_256:
.quad 0,0,0,0xffffffffffffffff
and_last_block_256:
.quad 0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0x0000000000000000
ones:
.quad 0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff
bswap_mask64_256:
.byte 7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8

.align 64
and_last_block_not:
.quad 0,0,0,0,0,0,0,0xffffffffffffffff
and_last_block:
.quad 0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0x0000000000000000
bswap_mask64_512:
.byte 7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8
K_table_512:
.quad   0x428a2f98d728ae22
.quad   0x7137449123ef65cd
.quad   0xb5c0fbcfec4d3b2f
.quad   0xe9b5dba58189dbbc
.quad   0x3956c25bf348b538
.quad   0x59f111f1b605d019
.quad   0x923f82a4af194f9b
.quad   0xab1c5ed5da6d8118
.quad   0xd807aa98a3030242
.quad   0x12835b0145706fbe
.quad   0x243185be4ee4b28c
.quad   0x550c7dc3d5ffb4e2
.quad   0x72be5d74f27b896f
.quad   0x80deb1fe3b1696b1
.quad   0x9bdc06a725c71235
.quad   0xc19bf174cf692694
.quad   0xe49b69c19ef14ad2
.quad   0xefbe4786384f25e3
.quad   0x0fc19dc68b8cd5b5
.quad   0x240ca1cc77ac9c65
.quad   0x2de92c6f592b0275
.quad   0x4a7484aa6ea6e483
.quad   0x5cb0a9dcbd41fbd4
.quad   0x76f988da831153b5
.quad   0x983e5152ee66dfab
.quad   0xa831c66d2db43210
.quad   0xb00327c898fb213f
.quad   0xbf597fc7beef0ee4
.quad   0xc6e00bf33da88fc2
.quad   0xd5a79147930aa725
.quad   0x06ca6351e003826f
.quad   0x142929670a0e6e70
.quad   0x27b70a8546d22ffc
.quad   0x2e1b21385c26c926
.quad   0x4d2c6dfc5ac42aed
.quad   0x53380d139d95b3df
.quad   0x650a73548baf63de
.quad   0x766a0abb3c77b2a8
.quad   0x81c2c92e47edaee6
.quad   0x92722c851482353b
.quad   0xa2bfe8a14cf10364
.quad   0xa81a664bbc423001
.quad   0xc24b8b70d0f89791
.quad   0xc76c51a30654be30
.quad   0xd192e819d6ef5218
.quad   0xd69906245565a910
.quad   0xf40e35855771202a
.quad   0x106aa07032bbd1b8
.quad   0x19a4c116b8d2d0c8
.quad   0x1e376c085141ab53
.quad   0x2748774cdf8eeb99
.quad   0x34b0bcb5e19b48a8
.quad   0x391c0cb3c5c95a63
.quad   0x4ed8aa4ae3418acb
.quad   0x5b9cca4f7763e373
.quad   0x682e6ff3d6b2b8a3
.quad   0x748f82ee5defb2fc
.quad   0x78a5636f43172f60
.quad   0x84c87814a1f0ab72
.quad   0x8cc702081a6439ec
.quad   0x90befffa23631e28
.quad   0xa4506cebde82bde9
.quad   0xbef9a3f7b2c67915
.quad   0xc67178f2e372532b
.quad   0xca273eceea26619c
.quad   0xd186b8c721c0c207
.quad   0xeada7dd6cde0eb1e
.quad   0xf57d4f7fee6ed178
.quad   0x06f067aa72176fba
.quad   0x0a637dc5a2c898a6
.quad   0x113f9804bef90dae
.quad   0x1b710b35131c471b
.quad   0x28db77f523047d84
.quad   0x32caab7b40c72493
.quad   0x3c9ebe0a15c9bebc
.quad   0x431d67c49c100d4c
.quad   0x4cc5d4becb3e42b6
.quad   0x597f299cfc657e2a
.quad   0x5fcb6fab3ad6faec
.quad   0x6c44198c4a475817
