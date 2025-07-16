# Copyright (c) 2025 Intel Corporation
#
# SPDX-License-Identifier: MIT

# Define arg registers
.equ arg1,  %rdi
.equ arg2,  %rsi
.equ arg3,  %rdx
.equ arg4,  %rcx
.equ arg5,  %r8
.equ arg5d, %r8d
.equ arg6,  %r9

# arg7-10 on stack
#define arg7  (2*8)(%rbp)
#define arg8  (3*8)(%rbp)
#define arg9  (4*8)(%rbp)
#define arg10 (5*8)(%rbp)


# Define SHA3 rates
.equ SHA3_256_RATE, 136
.equ SHA3_384_RATE, 104
.equ SHA3_512_RATE, 72
.equ SHAKE128_RATE, 168
.equ SHAKE256_RATE, 136


# SHA3 multi-rate padding byte (added after the message)
.equ SHAKE_MRATE_PADDING, 0x1F


# Stack frame layout for shake128_x4 and shake256_x4 operations
.equ STATE_SIZE,        ((25 * 8 * 4) + 8)
.equ sf_arg1,           0
.equ sf_arg2,           sf_arg1 + 8                   # save arg2, output pointer
.equ sf_arg3,           sf_arg2 + 8                   # save arg3, output pointer
.equ sf_arg4,           sf_arg3 + 8                   # save arg4, output pointer
.equ sf_arg5,           sf_arg4 + 8                   # save arg5, output length
.equ sf_state_ptr,      sf_arg5 + 8                   # state context structure (pointer to a pointer)
.equ sf_state_x4,       sf_state_ptr + 8              # start of x4 state structure
.equ sf_size,           sf_state_x4 + STATE_SIZE


# External utility functions
.extern	keccak_1600_init_state
.extern	keccak_1600_permute
.extern	keccak_1600_load_state_x4
.extern	keccak_1600_save_state_x4
.extern	keccak_1600_partial_add_x4
.extern	keccak_1600_extract_bytes_x4


# Define macros for x4 operations

# Absorb input bytes into x4 state registers
#   ymm0-ymm24          [in] x4 state registers
#   ymm30-ymm31  [clobbered] used as a temporary registers
.macro absorb_bytes_x4 input0, input1, input2, input3, offset, rate
    vmovq   (\input0, \offset), %xmm31
    vpinsrq $1, (\input1, \offset), %xmm31, %xmm31
    vmovq   (\input2, \offset), %xmm30
    vpinsrq $1, (\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm0, %ymm0

    vmovq   8(\input0, \offset), %xmm31
    vpinsrq $1, 8(\input1, \offset), %xmm31, %xmm31
    vmovq   8(\input2, \offset), %xmm30
    vpinsrq $1, 8(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm1, %ymm1

    vmovq   16(\input0, \offset), %xmm31
    vpinsrq $1, 16(\input1, \offset), %xmm31, %xmm31
    vmovq   16(\input2, \offset), %xmm30
    vpinsrq $1, 16(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm2, %ymm2

    vmovq   24(\input0, \offset), %xmm31
    vpinsrq $1, 24(\input1, \offset), %xmm31, %xmm31
    vmovq   24(\input2, \offset), %xmm30
    vpinsrq $1, 24(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm3, %ymm3

    vmovq   32(\input0, \offset), %xmm31
    vpinsrq $1, 32(\input1, \offset), %xmm31, %xmm31
    vmovq   32(\input2, \offset), %xmm30
    vpinsrq $1, 32(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm4, %ymm4

    vmovq   40(\input0, \offset), %xmm31
    vpinsrq $1, 40(\input1, \offset), %xmm31, %xmm31
    vmovq   40(\input2, \offset), %xmm30
    vpinsrq $1, 40(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm5, %ymm5

    vmovq   48(\input0, \offset), %xmm31
    vpinsrq $1, 48(\input1, \offset), %xmm31, %xmm31
    vmovq   48(\input2, \offset), %xmm30
    vpinsrq $1, 48(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm6, %ymm6

    vmovq   56(\input0, \offset), %xmm31
    vpinsrq $1, 56(\input1, \offset), %xmm31, %xmm31
    vmovq   56(\input2, \offset), %xmm30
    vpinsrq $1, 56(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm7, %ymm7

    vmovq   64(\input0, \offset), %xmm31
    vpinsrq $1, 64(\input1, \offset), %xmm31, %xmm31
    vmovq   64(\input2, \offset), %xmm30
    vpinsrq $1, 64(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm8, %ymm8
    vmovq   72(\input0, \offset), %xmm31
    vpinsrq $1, 72(\input1, \offset), %xmm31, %xmm31
    vmovq   72(\input2, \offset), %xmm30
    vpinsrq $1, 72(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm9, %ymm9

    vmovq   80(\input0, \offset), %xmm31
    vpinsrq $1, 80(\input1, \offset), %xmm31, %xmm31
    vmovq   80(\input2, \offset), %xmm30
    vpinsrq $1, 80(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm10, %ymm10

    vmovq   88(\input0, \offset), %xmm31
    vpinsrq $1, 88(\input1, \offset), %xmm31, %xmm31
    vmovq   88(\input2, \offset), %xmm30
    vpinsrq $1, 88(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm11, %ymm11

    vmovq   96(\input0, \offset), %xmm31
    vpinsrq $1, 96(\input1, \offset), %xmm31, %xmm31
    vmovq   96(\input2, \offset), %xmm30
    vpinsrq $1, 96(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm12, %ymm12
    vmovq   104(\input0, \offset), %xmm31
    vpinsrq $1, 104(\input1, \offset), %xmm31, %xmm31
    vmovq   104(\input2, \offset), %xmm30
    vpinsrq $1, 104(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm13, %ymm13

    vmovq   112(\input0, \offset), %xmm31
    vpinsrq $1, 112(\input1, \offset), %xmm31, %xmm31
    vmovq   112(\input2, \offset), %xmm30
    vpinsrq $1, 112(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm14, %ymm14

    vmovq   120(\input0, \offset), %xmm31
    vpinsrq $1, 120(\input1, \offset), %xmm31, %xmm31
    vmovq   120(\input2, \offset), %xmm30
    vpinsrq $1, 120(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm15, %ymm15

    vmovq   128(\input0, \offset), %xmm31
    vpinsrq $1, 128(\input1, \offset), %xmm31, %xmm31
    vmovq   128(\input2, \offset), %xmm30
    vpinsrq $1, 128(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm16, %ymm16
.if \rate > SHA3_256_RATE
    # SHAKE128 rate
    vmovq   136(\input0, \offset), %xmm31
    vpinsrq $1, 136(\input1, \offset), %xmm31, %xmm31
    vmovq   136(\input2, \offset), %xmm30
    vpinsrq $1, 136(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm17, %ymm17

    vmovq   144(\input0, \offset), %xmm31
    vpinsrq $1, 144(\input1, \offset), %xmm31, %xmm31
    vmovq   144(\input2, \offset), %xmm30
    vpinsrq $1, 144(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm18, %ymm18

    vmovq   152(\input0, \offset), %xmm31
    vpinsrq $1, 152(\input1, \offset), %xmm31, %xmm31
    vmovq   152(\input2, \offset), %xmm30
    vpinsrq $1, 152(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm19, %ymm19

    vmovq   160(\input0, \offset), %xmm31
    vpinsrq $1, 160(\input1, \offset), %xmm31, %xmm31
    vmovq   160(\input2, \offset), %xmm30
    vpinsrq $1, 160(\input3, \offset), %xmm30, %xmm30
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq  %ymm31, %ymm20, %ymm20
.endif
.endm

# Store x4 state from SIMD registers to memory
#
#   ymm0-ymm24        [in] x4 state registers
#   ymm31      [clobbered] used as a temporary register
.macro extract_state_x4 output0, output1, output2, output3, offset, n
    vextracti64x2 $1, %ymm0, %xmm31
    vmovq       %xmm0,  (8*0)(\output0, \offset)
    vpextrq $1, %xmm0,  (8*0)(\output1, \offset)
    vmovq       %xmm31, (8*0)(\output2, \offset)
    vpextrq $1, %xmm31, (8*0)(\output3, \offset)

    vextracti64x2 $1, %ymm1, %xmm31
    vmovq       %xmm1,  (8*1)(\output0, \offset)
    vpextrq $1, %xmm1,  (8*1)(\output1, \offset)
    vmovq       %xmm31, (8*1)(\output2, \offset)
    vpextrq $1, %xmm31, (8*1)(\output3, \offset)

    vextracti64x2 $1, %ymm2, %xmm31
    vmovq       %xmm2,  (8*2)(\output0, \offset)
    vpextrq $1, %xmm2,  (8*2)(\output1, \offset)
    vmovq       %xmm31, (8*2)(\output2, \offset)
    vpextrq $1, %xmm31, (8*2)(\output3, \offset)

    vextracti64x2 $1, %ymm3, %xmm31
    vmovq       %xmm3,  (8*3)(\output0, \offset)
    vpextrq $1, %xmm3,  (8*3)(\output1, \offset)
    vmovq       %xmm31, (8*3)(\output2, \offset)
    vpextrq $1, %xmm31, (8*3)(\output3, \offset)

    vextracti64x2 $1, %ymm4, %xmm31
    vmovq       %xmm4,  (8*4)(\output0, \offset)
    vpextrq $1, %xmm4,  (8*4)(\output1, \offset)
    vmovq       %xmm31, (8*4)(\output2, \offset)
    vpextrq $1, %xmm31, (8*4)(\output3, \offset)

    vextracti64x2 $1, %ymm5, %xmm31
    vmovq       %xmm5,  (8*5)(\output0, \offset)
    vpextrq $1, %xmm5,  (8*5)(\output1, \offset)
    vmovq       %xmm31, (8*5)(\output2, \offset)
    vpextrq $1, %xmm31, (8*5)(\output3, \offset)

    vextracti64x2 $1, %ymm6, %xmm31
    vmovq       %xmm6,  (8*6)(\output0, \offset)
    vpextrq $1, %xmm6,  (8*6)(\output1, \offset)
    vmovq       %xmm31, (8*6)(\output2, \offset)
    vpextrq $1, %xmm31, (8*6)(\output3, \offset)

    vextracti64x2 $1, %ymm7, %xmm31
    vmovq       %xmm7,  (8*7)(\output0, \offset)
    vpextrq $1, %xmm7,  (8*7)(\output1, \offset)
    vmovq       %xmm31, (8*7)(\output2, \offset)
    vpextrq $1, %xmm31, (8*7)(\output3, \offset)

    vextracti64x2 $1, %ymm8, %xmm31
    vmovq       %xmm8,  (8*8)(\output0, \offset)
    vpextrq $1, %xmm8,  (8*8)(\output1, \offset)
    vmovq       %xmm31, (8*8)(\output2, \offset)
    vpextrq $1, %xmm31, (8*8)(\output3, \offset)

    vextracti64x2 $1, %ymm9, %xmm31
    vmovq       %xmm9,  (8*9)(\output0, \offset)
    vpextrq $1, %xmm9,  (8*9)(\output1, \offset)
    vmovq       %xmm31, (8*9)(\output2, \offset)
    vpextrq $1, %xmm31, (8*9)(\output3, \offset)

    vextracti64x2 $1, %ymm10, %xmm31
    vmovq       %xmm10, (8*10)(\output0, \offset)
    vpextrq $1, %xmm10, (8*10)(\output1, \offset)
    vmovq       %xmm31, (8*10)(\output2, \offset)
    vpextrq $1, %xmm31, (8*10)(\output3, \offset)

    vextracti64x2 $1, %ymm11, %xmm31
    vmovq       %xmm11, (8*11)(\output0, \offset)
    vpextrq $1, %xmm11, (8*11)(\output1, \offset)
    vmovq       %xmm31, (8*11)(\output2, \offset)
    vpextrq $1, %xmm31, (8*11)(\output3, \offset)

    vextracti64x2 $1, %ymm12, %xmm31
    vmovq       %xmm12, (8*12)(\output0, \offset)
    vpextrq $1, %xmm12, (8*12)(\output1, \offset)
    vmovq       %xmm31, (8*12)(\output2, \offset)
    vpextrq $1, %xmm31, (8*12)(\output3, \offset)

    vextracti64x2 $1, %ymm13, %xmm31
    vmovq       %xmm13, (8*13)(\output0, \offset)
    vpextrq $1, %xmm13, (8*13)(\output1, \offset)
    vmovq       %xmm31, (8*13)(\output2, \offset)
    vpextrq $1, %xmm31, (8*13)(\output3, \offset)

    vextracti64x2 $1, %ymm14, %xmm31
    vmovq       %xmm14, (8*14)(\output0, \offset)
    vpextrq $1, %xmm14, (8*14)(\output1, \offset)
    vmovq       %xmm31, (8*14)(\output2, \offset)
    vpextrq $1, %xmm31, (8*14)(\output3, \offset)

    vextracti64x2 $1, %ymm15, %xmm31
    vmovq       %xmm15, (8*15)(\output0, \offset)
    vpextrq $1, %xmm15, (8*15)(\output1, \offset)
    vmovq       %xmm31, (8*15)(\output2, \offset)
    vpextrq $1, %xmm31, (8*15)(\output3, \offset)

    vextracti64x2 $1, %ymm16, %xmm31
    vmovq       %xmm16, (8*16)(\output0, \offset)
    vpextrq $1, %xmm16, (8*16)(\output1, \offset)
    vmovq       %xmm31, (8*16)(\output2, \offset)
    vpextrq $1, %xmm31, (8*16)(\output3, \offset)

.if \n > 17
    vextracti64x2 $1, %ymm17, %xmm31
    vmovq       %xmm17, (8*17)(\output0, \offset)
    vpextrq $1, %xmm17, (8*17)(\output1, \offset)
    vmovq       %xmm31, (8*17)(\output2, \offset)
    vpextrq $1, %xmm31, (8*17)(\output3, \offset)

    vextracti64x2 $1, %ymm18, %xmm31
    vmovq       %xmm18, (8*18)(\output0, \offset)
    vpextrq $1, %xmm18, (8*18)(\output1, \offset)
    vmovq       %xmm31, (8*18)(\output2, \offset)
    vpextrq $1, %xmm31, (8*18)(\output3, \offset)

    vextracti64x2 $1, %ymm19, %xmm31
    vmovq       %xmm19, (8*19)(\output0, \offset)
    vpextrq $1, %xmm19, (8*19)(\output1, \offset)
    vmovq       %xmm31, (8*19)(\output2, \offset)
    vpextrq $1, %xmm31, (8*19)(\output3, \offset)

    vextracti64x2 $1, %ymm20, %xmm31
    vmovq       %xmm20, (8*20)(\output0, \offset)
    vpextrq $1, %xmm20, (8*20)(\output1, \offset)
    vmovq       %xmm31, (8*20)(\output2, \offset)
    vpextrq $1, %xmm31, (8*20)(\output3, \offset)
.endif
.endm

.text

#
# Init/Reset API
#

# -----------------------------------------------------------------------------
#
# void SHA3_shake128_x4_inc_ctx_reset_avx512vl(OQS_SHA3_shake128_inc_ctx *state);
#
.globl  SHA3_shake128_x4_inc_ctx_reset_avx512vl
.type   SHA3_shake128_x4_inc_ctx_reset_avx512vl,@function
.hidden SHA3_shake128_x4_inc_ctx_reset_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_shake256_inc_ctx_reset_x4_avx512vl(OQS_SHA3_shake256_inc_ctx *state);
#
.globl  SHA3_shake256_x4_inc_ctx_reset_avx512vl
.type   SHA3_shake256_x4_inc_ctx_reset_avx512vl,@function
.hidden SHA3_shake256_x4_inc_ctx_reset_avx512vl


.balign 32
SHA3_shake128_x4_inc_ctx_reset_avx512vl:
SHA3_shake256_x4_inc_ctx_reset_avx512vl:
    movq        (arg1), arg1            # load arg1.ctx
    vpxorq      %xmm31, %xmm31, %xmm31
    vmovdqu64   %ymm31, (32*0)(arg1)    # clear 800 bytes of state
    vmovdqu64   %ymm31, (32*1)(arg1)
    vmovdqu64   %ymm31, (32*2)(arg1)
    vmovdqu64   %ymm31, (32*3)(arg1)
    vmovdqu64   %ymm31, (32*4)(arg1)
    vmovdqu64   %ymm31, (32*5)(arg1)
    vmovdqu64   %ymm31, (32*6)(arg1)
    vmovdqu64   %ymm31, (32*7)(arg1)
    vmovdqu64   %ymm31, (32*8)(arg1)
    vmovdqu64   %ymm31, (32*9)(arg1)
    vmovdqu64   %ymm31, (32*10)(arg1)
    vmovdqu64   %ymm31, (32*11)(arg1)
    vmovdqu64   %ymm31, (32*12)(arg1)
    vmovdqu64   %ymm31, (32*13)(arg1)
    vmovdqu64   %ymm31, (32*14)(arg1)
    vmovdqu64   %ymm31, (32*15)(arg1)
    vmovdqu64   %ymm31, (32*16)(arg1)
    vmovdqu64   %ymm31, (32*17)(arg1)
    vmovdqu64   %ymm31, (32*18)(arg1)
    vmovdqu64   %ymm31, (32*19)(arg1)
    vmovdqu64   %ymm31, (32*20)(arg1)
    vmovdqu64   %ymm31, (32*21)(arg1)
    vmovdqu64   %ymm31, (32*22)(arg1)
    vmovdqu64   %ymm31, (32*23)(arg1)
    vmovdqu64   %ymm31, (32*24)(arg1)
    vmovq       %xmm31, (32*25)(arg1)   # also clear additional 8 bytes s[100]
    ret
.size   SHA3_shake128_x4_inc_ctx_reset_avx512vl,.-SHA3_shake128_x4_inc_ctx_reset_avx512vl
.size   SHA3_shake256_x4_inc_ctx_reset_avx512vl,.-SHA3_shake256_x4_inc_ctx_reset_avx512vl

#
# SHAKE128 API
#

# -----------------------------------------------------------------------------
#
# void SHA3_shake128_x4_avx512vl(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3,
#                                size_t outlen, const uint8_t *in0, const uint8_t *in1,
#                                const uint8_t *in2, const uint8_t *in3, size_t inlen);
#
.globl  SHA3_shake128_x4_avx512vl
.type   SHA3_shake128_x4_avx512vl,@function
.hidden SHA3_shake128_x4_avx512vl
.balign 32
SHA3_shake128_x4_avx512vl:
    pushq   %rbp
    movq    %rsp, %rbp
    pushq   %rbx

    subq    $sf_size, %rsp
    movq    %rsp, %rbx

    mov     arg1, (sf_arg1)(%rbx)
    mov     arg2, (sf_arg2)(%rbx)
    mov     arg3, (sf_arg3)(%rbx)
    mov     arg4, (sf_arg4)(%rbx)
    mov     arg5, (sf_arg5)(%rbx)

    lea     (sf_state_x4)(%rbx), arg1               # start of x4 state on the stack frame
    mov     arg1, (sf_state_ptr)(%rbx)

    # Initialize the state array to zero
    call    keccak_1600_init_state

    call    keccak_1600_save_state_x4

    movq    $0, (8*100)(arg1)                       # clear s[100]

    lea     (sf_state_ptr)(%rbx), arg1
    mov     arg6, arg2
    mov     arg7, arg3
    mov     arg8, arg4
    mov     arg9, arg5
    mov     arg10, arg6
    call    SHA3_shake128_x4_inc_absorb_avx512vl

    lea     (sf_state_ptr)(%rbx), arg1
    call    SHA3_shake128_x4_inc_finalize_avx512vl

    # squeeze
    mov     (sf_arg1)(%rbx), arg1
    mov     (sf_arg2)(%rbx), arg2
    mov     (sf_arg3)(%rbx), arg3
    mov     (sf_arg4)(%rbx), arg4
    mov     (sf_arg5)(%rbx), arg5
    lea     (sf_state_ptr)(%rbx), arg6
    call    SHA3_shake128_x4_inc_squeeze_avx512vl

    # Clear the temporary buffer
    lea         (sf_state_x4)(%rbx), %rax
    vpxorq      %ymm31, %ymm31, %ymm31
    vmovdqu64   %ymm31, (32*0)(%rax)
    vmovdqu64   %ymm31, (32*1)(%rax)
    vmovdqu64   %ymm31, (32*2)(%rax)
    vmovdqu64   %ymm31, (32*3)(%rax)
    vmovdqu64   %ymm31, (32*4)(%rax)
    vmovdqu64   %ymm31, (32*5)(%rax)
    vmovdqu64   %ymm31, (32*6)(%rax)
    vmovdqu64   %ymm31, (32*7)(%rax)
    vmovdqu64   %ymm31, (32*8)(%rax)
    vmovdqu64   %ymm31, (32*9)(%rax)
    vmovdqu64   %ymm31, (32*10)(%rax)
    vmovdqu64   %ymm31, (32*11)(%rax)
    vmovdqu64   %ymm31, (32*12)(%rax)
    vmovdqu64   %ymm31, (32*13)(%rax)
    vmovdqu64   %ymm31, (32*14)(%rax)
    vmovdqu64   %ymm31, (32*15)(%rax)
    vmovdqu64   %ymm31, (32*16)(%rax)
    vmovdqu64   %ymm31, (32*17)(%rax)
    vmovdqu64   %ymm31, (32*18)(%rax)
    vmovdqu64   %ymm31, (32*19)(%rax)
    vmovdqu64   %ymm31, (32*20)(%rax)
    vmovdqu64   %ymm31, (32*21)(%rax)
    vmovdqu64   %ymm31, (32*22)(%rax)
    vmovdqu64   %ymm31, (32*23)(%rax)
    vmovdqu64   %ymm31, (32*24)(%rax)
    vmovq       %xmm31, (32*25)(%rax)

    addq    $sf_size, %rsp
    popq    %rbx
    popq    %rbp
    ret
.size   SHA3_shake128_x4_avx512vl,.-SHA3_shake128_x4_avx512vl



# -----------------------------------------------------------------------------
#
# void SHA3_shake128_x4_inc_absorb_avx512vl(
#    OQS_SHA3_shake128_x4_inc_ctx *state,
#    const uint8_t *in0,
#    const uint8_t *in1,
#    const uint8_t *in2,
#    const uint8_t *in3,
#    size_t inlen);
#
.globl  SHA3_shake128_x4_inc_absorb_avx512vl
.type   SHA3_shake128_x4_inc_absorb_avx512vl,@function
.hidden SHA3_shake128_x4_inc_absorb_avx512vl
.balign 32
SHA3_shake128_x4_inc_absorb_avx512vl:
    pushq   %rbp
    movq    %rsp, %rbp
    pushq   %rbx
    pushq   %r12
    pushq   %r13
    pushq   %r14
    pushq   %r15

    mov     (arg1), arg1                            # state.ctx into arg1

    # check for partially processed block
    movq    (8*100)(arg1), %r14
    orq     %r14, %r14                              # s[100] == 0?
    je      .shake128_absorb_main_loop_start

    # process remaining bytes if message long enough
    movq    $SHAKE128_RATE, %r12                    # c = rate - s[100]
    subq    %r14, %r12                              # %r12 = capacity

    cmp     %r12, arg6                              # if mlen <= capacity then no permute
    jbe     .shake128_absorb_skip_permute

    subq    %r12, arg6

    # r10/state, arg2-arg5/inputs, r12/length
    movq    arg1, %r10                              # %r10 = state
    call    keccak_1600_partial_add_x4              # arg2-arg5 are updated

    call    keccak_1600_load_state_x4

    call    keccak_1600_permute

    movq    $0, (8*100)(arg1)                       # clear s[100]
    jmp     .shake128_absorb_partial_block_done

.shake128_absorb_skip_permute:
    # r10/state, arg2-arg5/inputs, r12/length
    movq    arg1, %r10
    movq    arg6, %r12
    call    keccak_1600_partial_add_x4

    leaq    (arg6, %r14), %r15
    mov     %r15, (8*100)(arg1)                     # s[100] += inlen

    cmpq    $SHAKE128_RATE, %r15                    # check s[100] below rate
    jb      .shake128_absorb_exit

    call    keccak_1600_load_state_x4

    call    keccak_1600_permute

    call    keccak_1600_save_state_x4

    movq    $0, (8*100)(arg1)                       # clear s[100]
    jmp     .shake128_absorb_exit

.shake128_absorb_main_loop_start:
    call    keccak_1600_load_state_x4

.shake128_absorb_partial_block_done:
    movq    arg6, %r11                              # copy message length to %r11
    xorq    %r12, %r12                              # zero message offset

    # Process the input message in blocks
.balign 32
.shake128_absorb_while_loop:
    cmpq    $SHAKE128_RATE, %r11                    # compare mlen to rate
    jb      .shake128_absorb_while_loop_done

    absorb_bytes_x4 arg2, arg3, arg4, arg5, %r12, SHAKE128_RATE

    subq    $SHAKE128_RATE, %r11                    # Subtract the rate from the remaining length
    addq    $SHAKE128_RATE, %r12                    # Adjust the pointer to the next block of the input message
    call    keccak_1600_permute                     # Perform the Keccak permutation

    jmp     .shake128_absorb_while_loop

.balign 32
.shake128_absorb_while_loop_done:
    call    keccak_1600_save_state_x4

    mov     %r11, (8*100)(arg1)                     # update s[100]
    orq     %r11, %r11
    jz      .shake128_absorb_exit

    movq    $0, (8*100)(arg1)                       # clear s[100]

    # r10/state, arg2-arg5/input, r12/length
    movq    arg1, %r10
    addq    %r12, arg2
    addq    %r12, arg3
    addq    %r12, arg4
    addq    %r12, arg5
    movq    %r11, %r12
    call    keccak_1600_partial_add_x4

    mov     %r11, (8*100)(arg1)                     # update s[100]

.shake128_absorb_exit:
    vpxorq      %xmm16, %xmm16, %xmm16
    vmovdqa64   %ymm16, %ymm17
    vmovdqa64   %ymm16, %ymm18
    vmovdqa64   %ymm16, %ymm19
    vmovdqa64   %ymm16, %ymm20
    vmovdqa64   %ymm16, %ymm21
    vmovdqa64   %ymm16, %ymm22
    vmovdqa64   %ymm16, %ymm23
    vmovdqa64   %ymm16, %ymm24
    vmovdqa64   %ymm16, %ymm25
    vmovdqa64   %ymm16, %ymm26
    vmovdqa64   %ymm16, %ymm27
    vmovdqa64   %ymm16, %ymm28
    vmovdqa64   %ymm16, %ymm29
    vmovdqa64   %ymm16, %ymm30
    vmovdqa64   %ymm16, %ymm31
    vzeroall

    popq    %r15
    popq    %r14
    popq    %r13
    popq    %r12
    popq    %rbx
    popq    %rbp
    ret
.size   SHA3_shake128_x4_inc_absorb_avx512vl,.-SHA3_shake128_x4_inc_absorb_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_shake128_x4_inc_finalize_avx512vl(OQS_SHA3_shake128_x4_inc_ctx *state);
#
.globl  SHA3_shake128_x4_inc_finalize_avx512vl
.type   SHA3_shake128_x4_inc_finalize_avx512vl,@function
.hidden SHA3_shake128_x4_inc_finalize_avx512vl
.balign 32
SHA3_shake128_x4_inc_finalize_avx512vl:
    mov         (arg1), arg1                            # state.ctx into arg1
    movq        (8*100)(arg1), %r11                     # load state offset from s[100]
    movq        %r11, %r10
    andl        $~7, %r10d                              # offset to the state register
    andl        $7, %r11d                               # offset within the register

    # add EOM byte right after the message
    vmovdqu32   (arg1, %r10, 4), %ymm31
    leaq        shake_msg_pad_x4(%rip), %rax
    subq        %r11, %rax
    vmovdqu32   (%rax), %ymm30
    vpxorq      %ymm30, %ymm31, %ymm31
    vmovdqu32   %ymm31, (arg1, %r10, 4)

    # add terminating byte at offset equal to rate - 1
    vmovdqu32   (SHAKE128_RATE*4 - 4*8)(arg1), %ymm31
    vmovdqa32   shake_terminator_byte_x4(%rip), %ymm30
    vpxorq      %ymm30, %ymm31, %ymm31
    vmovdqu32   %ymm31, (SHAKE128_RATE*4 - 4*8)(arg1)

    movq        $0, (8*100)(arg1)                       # clear s[100]
    vpxorq      %ymm31, %ymm31, %ymm31
    ret
.size   SHA3_shake128_x4_inc_finalize_avx512vl,.-SHA3_shake128_x4_inc_finalize_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_shake128_x4_inc_squeeze_avx512vl(
#     uint8_t *out0,
#     uint8_t *out1,
#     uint8_t *out2,
#     uint8_t *out3,
#     size_t outlen,
#     OQS_SHA3_shake128_x4_inc_ctx *state);
#
.globl  SHA3_shake128_x4_inc_squeeze_avx512vl
.type   SHA3_shake128_x4_inc_squeeze_avx512vl,@function
.hidden SHA3_shake128_x4_inc_squeeze_avx512vl
.balign 32
SHA3_shake128_x4_inc_squeeze_avx512vl:
    pushq       %rbp
    movq        %rsp, %rbp
    pushq       %rbx
    pushq       %r12
    pushq       %r13
    pushq       %r14
    pushq       %r15

    or          arg5, arg5
    jz          .shake128_squeeze_done

    mov         (arg6), arg6                    # arg6 - state.ctx

    # check for partially processed block
    movq        (8*100)(arg6), %r15             # s[100] - capacity
    orq         %r15, %r15
    jnz         .shake128_squeeze_no_init_permute


    movq        arg1, %r14
    mov         arg6, arg1
    call        keccak_1600_load_state_x4

    movq        %r14, arg1

    xorq        %rax, %rax
    jmp         .shake128_squeeze_loop


.balign 32
.shake128_squeeze_no_init_permute:
    # extract bytes: r10 - state/src, arg1-arg4 - output/dst, r12 - length = min(capacity, outlen), r11 - offset
    movq        arg6, %r10

    movq        %r15, %r12
    cmpq        %r15, arg5
    cmovb       arg5, %r12                      # %r12 = min(capacity, $outlen)

    sub         %r12, arg5                      # outlen -= length

    movl        $SHAKE128_RATE, %r11d
    subq        %r15, %r11                      # state offset

    subq        %r12, %r15                      # capacity -= length
    mov         %r15, (8*100)(arg6)             # update s[100]

    call        keccak_1600_extract_bytes_x4

    orq         %r15, %r15
    jnz         .shake128_squeeze_done          # check s[100] not zero

    movq        arg1, %r14# preserve arg1
    mov         arg6, arg1
    call        keccak_1600_load_state_x4

    movq        %r14, arg1
    xorq        %rax, %rax

.balign 32
.shake128_squeeze_loop:
    cmp         $SHAKE128_RATE, arg5            # outlen > r
    jb          .shake128_squeeze_final_extract

    call        keccak_1600_permute

    # Extract SHAKE128 rate bytes into the destination buffer
    extract_state_x4 arg1, arg2, arg3, arg4, %rax, (SHAKE128_RATE / 8)

    addq        $SHAKE128_RATE, %rax            # dst offset += r
    sub         $SHAKE128_RATE, arg5            # outlen -= r
    jmp         .shake128_squeeze_loop

.balign 32
.shake128_squeeze_final_extract:
    or          arg5, arg5
    jz          .shake128_squeeze_no_end_permute

    # update output pointers
    addq        %rax, arg1
    addq        %rax, arg2
    addq        %rax, arg3
    addq        %rax, arg4

    movl        $SHAKE128_RATE, %r15d
    subq        arg5, %r15
    mov         %r15, (8*100)(arg6)             # s[100] = c

    call        keccak_1600_permute


    movq        arg1, %r14
    mov         arg6, arg1
    call        keccak_1600_save_state_x4

    movq        %r14, arg1

    # extract bytes: r10 - state/src, arg1-arg4 - output/dst, r12 - length, r11 - offset = 0
    movq        arg6, %r10
    movq        arg5, %r12
    xorq        %r11, %r11
    call        keccak_1600_extract_bytes_x4

    jmp         .shake128_squeeze_done

.shake128_squeeze_no_end_permute:
    movq        $0, (8*100)(arg6)               # s[100] = 0
    mov         arg6, arg1
    call        keccak_1600_save_state_x4

.shake128_squeeze_done:
    vpxorq      %xmm16, %xmm16, %xmm16
    vmovdqa64   %ymm16, %ymm17
    vmovdqa64   %ymm16, %ymm18
    vmovdqa64   %ymm16, %ymm19
    vmovdqa64   %ymm16, %ymm20
    vmovdqa64   %ymm16, %ymm21
    vmovdqa64   %ymm16, %ymm22
    vmovdqa64   %ymm16, %ymm23
    vmovdqa64   %ymm16, %ymm24
    vmovdqa64   %ymm16, %ymm25
    vmovdqa64   %ymm16, %ymm26
    vmovdqa64   %ymm16, %ymm27
    vmovdqa64   %ymm16, %ymm28
    vmovdqa64   %ymm16, %ymm29
    vmovdqa64   %ymm16, %ymm30
    vmovdqa64   %ymm16, %ymm31
    vzeroall

    popq        %r15
    popq        %r14
    popq        %r13
    popq        %r12
    popq        %rbx
    popq        %rbp

    ret
.size   SHA3_shake128_x4_inc_squeeze_avx512vl,.-SHA3_shake128_x4_inc_squeeze_avx512vl


#
# SHAKE256 API
#

# -----------------------------------------------------------------------------
#
# void SHA3_shake256_x4_avx512vl(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3,
#                                size_t outlen, const uint8_t *in0, const uint8_t *in1,
#                                const uint8_t *in2, const uint8_t *in3, size_t inlen);
#
.globl  SHA3_shake256_x4_avx512vl
.type   SHA3_shake256_x4_avx512vl,@function
.hidden SHA3_shake256_x4_avx512vl
.balign 32
SHA3_shake256_x4_avx512vl:
    pushq   %rbp
    movq    %rsp, %rbp
    pushq   %rbx

    subq    $sf_size, %rsp
    movq    %rsp, %rbx

    mov     arg1, (sf_arg1)(%rbx)
    mov     arg2, (sf_arg2)(%rbx)
    mov     arg3, (sf_arg3)(%rbx)
    mov     arg4, (sf_arg4)(%rbx)
    mov     arg5, (sf_arg5)(%rbx)

    lea     (sf_state_x4)(%rbx), arg1               # start of x4 state on the stack frame
    mov     arg1, (sf_state_ptr)(%rbx)

    # Initialize the state array to zero
    call    keccak_1600_init_state

    call    keccak_1600_save_state_x4

    movq    $0, (8*100)(arg1)                       # clear s[100]

    lea     (sf_state_ptr)(%rbx), arg1
    mov     arg6, arg2
    mov     arg7, arg3
    mov     arg8, arg4
    mov     arg9, arg5
    mov     arg10, arg6
    call    SHA3_shake256_x4_inc_absorb_avx512vl


    lea     (sf_state_ptr)(%rbx), arg1
    call    SHA3_shake256_x4_inc_finalize_avx512vl


    # squeeze
    mov     (sf_arg1)(%rbx), arg1
    mov     (sf_arg2)(%rbx), arg2
    mov     (sf_arg3)(%rbx), arg3
    mov     (sf_arg4)(%rbx), arg4
    mov     (sf_arg5)(%rbx), arg5
    lea     (sf_state_ptr)(%rbx), arg6
    call    SHA3_shake256_x4_inc_squeeze_avx512vl

    # Clear the temporary buffer
    lea         (sf_state_x4)(%rbx), %rax
    vpxorq      %ymm31, %ymm31, %ymm31
    vmovdqu64   %ymm31, (32*0)(%rax)
    vmovdqu64   %ymm31, (32*1)(%rax)
    vmovdqu64   %ymm31, (32*2)(%rax)
    vmovdqu64   %ymm31, (32*3)(%rax)
    vmovdqu64   %ymm31, (32*4)(%rax)
    vmovdqu64   %ymm31, (32*5)(%rax)
    vmovdqu64   %ymm31, (32*6)(%rax)
    vmovdqu64   %ymm31, (32*7)(%rax)
    vmovdqu64   %ymm31, (32*8)(%rax)
    vmovdqu64   %ymm31, (32*9)(%rax)
    vmovdqu64   %ymm31, (32*10)(%rax)
    vmovdqu64   %ymm31, (32*11)(%rax)
    vmovdqu64   %ymm31, (32*12)(%rax)
    vmovdqu64   %ymm31, (32*13)(%rax)
    vmovdqu64   %ymm31, (32*14)(%rax)
    vmovdqu64   %ymm31, (32*15)(%rax)
    vmovdqu64   %ymm31, (32*16)(%rax)
    vmovdqu64   %ymm31, (32*17)(%rax)
    vmovdqu64   %ymm31, (32*18)(%rax)
    vmovdqu64   %ymm31, (32*19)(%rax)
    vmovdqu64   %ymm31, (32*20)(%rax)
    vmovdqu64   %ymm31, (32*21)(%rax)
    vmovdqu64   %ymm31, (32*22)(%rax)
    vmovdqu64   %ymm31, (32*23)(%rax)
    vmovdqu64   %ymm31, (32*24)(%rax)
    vmovq       %xmm31, (32*25)(%rax)

    addq    $sf_size, %rsp
    popq    %rbx
    popq    %rbp
    ret
.size   SHA3_shake256_x4_avx512vl,.-SHA3_shake256_x4_avx512vl



# -----------------------------------------------------------------------------
#
# void SHA3_shake256_x4_inc_absorb_avx512vl(
#    OQS_SHA3_shake256_x4_inc_ctx *state,
#    const uint8_t *in0,
#    const uint8_t *in1,
#    const uint8_t *in2,
#    const uint8_t *in3,
#    size_t inlen);
#
.globl  SHA3_shake256_x4_inc_absorb_avx512vl
.type   SHA3_shake256_x4_inc_absorb_avx512vl,@function
.hidden SHA3_shake256_x4_inc_absorb_avx512vl
.balign 32
SHA3_shake256_x4_inc_absorb_avx512vl:
    pushq   %rbp
    movq    %rsp, %rbp
    pushq   %rbx
    pushq   %r12
    pushq   %r13
    pushq   %r14
    pushq   %r15

    mov     (arg1), arg1                            # state.ctx into arg1

    # check for partially processed block
    movq    (8*100)(arg1), %r14
    orq     %r14, %r14                              # check s[100] is zero
    je      .shake256_absorb_main_loop_start


    # process remaining bytes if message long enough
    movq    $SHAKE256_RATE, %r12                    # c = rate - s[100]
    subq    %r14, %r12                              # %r12 = capacity

    cmp     %r12, arg6                              # if mlen <= capacity then no permute
    jbe     .shake256_absorb_skip_permute

    subq    %r12, arg6

    # r10/state, arg2-arg5/inputs, r12/length
    movq    arg1, %r10                              # %r10 = state
    call    keccak_1600_partial_add_x4              # arg2-arg5 are updated

    call    keccak_1600_load_state_x4

    call    keccak_1600_permute

    movq    $0, (8*100)(arg1)                       # clear s[100]
    jmp     .shake256_absorb_partial_block_done

.shake256_absorb_skip_permute:
    # r10/state, arg2-arg5/inputs, r12/length
    movq    arg1, %r10
    movq    arg6, %r12
    call    keccak_1600_partial_add_x4

    leaq    (arg6, %r14), %r15
    mov     %r15, (8*100)(arg1)                     # s[100] += inlen

    cmpq    $SHAKE256_RATE, %r15                    # s[100] >= rate ?
    jb      .shake256_absorb_exit

    call    keccak_1600_load_state_x4

    call    keccak_1600_permute

    call    keccak_1600_save_state_x4

    movq    $0, (8*100)(arg1)                       # clear s[100]
    jmp     .shake256_absorb_exit

.shake256_absorb_main_loop_start:
    call    keccak_1600_load_state_x4

.shake256_absorb_partial_block_done:
    movq    arg6, %r11                              # copy message length to %r11
    xorq    %r12, %r12                              # zero message offset

    # Process the input message in blocks
.balign 32
.shake256_absorb_while_loop:
    cmpq    $SHAKE256_RATE, %r11                    # compare mlen to rate
    jb      .shake256_absorb_while_loop_done

    absorb_bytes_x4 arg2, arg3, arg4, arg5, %r12, SHAKE256_RATE

    subq    $SHAKE256_RATE, %r11                    # Subtract the rate from the remaining length
    addq    $SHAKE256_RATE, %r12                    # Adjust the pointer to the next block of the input message
    call    keccak_1600_permute                     # Perform the Keccak permutation

    jmp     .shake256_absorb_while_loop

.balign 32
.shake256_absorb_while_loop_done:
    call    keccak_1600_save_state_x4

    mov     %r11, (8*100)(arg1)                     # update s[100]
    orq     %r11, %r11
    jz      .shake256_absorb_exit

    movq    $0, (8*100)(arg1)                       # clear s[100]

    # r10/state, arg2-arg5/input, r12/length
    movq    arg1, %r10
    addq    %r12, arg2
    addq    %r12, arg3
    addq    %r12, arg4
    addq    %r12, arg5
    movq    %r11, %r12
    call    keccak_1600_partial_add_x4

    mov     %r11, (8*100)(arg1)                     # update s[100]

.shake256_absorb_exit:
    vpxorq      %xmm16, %xmm16, %xmm16
    vmovdqa64   %ymm16, %ymm17
    vmovdqa64   %ymm16, %ymm18
    vmovdqa64   %ymm16, %ymm19
    vmovdqa64   %ymm16, %ymm20
    vmovdqa64   %ymm16, %ymm21
    vmovdqa64   %ymm16, %ymm22
    vmovdqa64   %ymm16, %ymm23
    vmovdqa64   %ymm16, %ymm24
    vmovdqa64   %ymm16, %ymm25
    vmovdqa64   %ymm16, %ymm26
    vmovdqa64   %ymm16, %ymm27
    vmovdqa64   %ymm16, %ymm28
    vmovdqa64   %ymm16, %ymm29
    vmovdqa64   %ymm16, %ymm30
    vmovdqa64   %ymm16, %ymm31
    vzeroall

    popq    %r15
    popq    %r14
    popq    %r13
    popq    %r12
    popq    %rbx
    popq    %rbp
    ret
.size   SHA3_shake256_x4_inc_absorb_avx512vl,.-SHA3_shake256_x4_inc_absorb_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_shake256_x4_inc_finalize(OQS_SHA3_shake256_x4_inc_ctx *state);
#
.globl  SHA3_shake256_x4_inc_finalize_avx512vl
.type   SHA3_shake256_x4_inc_finalize_avx512vl,@function
.hidden SHA3_shake256_x4_inc_finalize_avx512vl
.balign 32
SHA3_shake256_x4_inc_finalize_avx512vl:
    mov         (arg1), arg1                            # state.ctx into arg1
    movq        (8*100)(arg1), %r11                     # load state offset from s[100]
    movq        %r11, %r10
    andl        $~7, %r10d                              # offset to the state register
    andl        $7, %r11d                               # offset within the register

    # add EOM byte right after the message
    vmovdqu32   (arg1, %r10, 4), %ymm31
    leaq        shake_msg_pad_x4(%rip), %rax
    subq        %r11, %rax
    vmovdqu32   (%rax), %ymm30
    vpxorq      %ymm30, %ymm31, %ymm31
    vmovdqu32   %ymm31, (arg1, %r10, 4)

    # add terminating byte at offset equal to rate - 1
    vmovdqu32   (SHAKE256_RATE*4 - 4*8)(arg1), %ymm31
    vmovdqa32   shake_terminator_byte_x4(%rip), %ymm30
    vpxorq      %ymm30, %ymm31, %ymm31
    vmovdqu32   %ymm31, (SHAKE256_RATE*4 - 4*8)(arg1)

    movq        $0, (8*100)(arg1)                       # clear s[100]
    vpxorq      %ymm31, %ymm31, %ymm31
    ret
.size   SHA3_shake256_x4_inc_finalize_avx512vl,.-SHA3_shake256_x4_inc_finalize_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_shake256_x4_inc_squeeze_avx512vl(
#     uint8_t *out0,
#     uint8_t *out1,
#     uint8_t *out2,
#     uint8_t *out3,
#     size_t outlen,
#     OQS_SHA3_shake256_x4_inc_ctx *state);
#
.globl  SHA3_shake256_x4_inc_squeeze_avx512vl
.type   SHA3_shake256_x4_inc_squeeze_avx512vl,@function
.hidden SHA3_shake256_x4_inc_squeeze_avx512vl
.balign 32
SHA3_shake256_x4_inc_squeeze_avx512vl:
    pushq       %rbp
    movq        %rsp, %rbp
    pushq       %rbx
    pushq       %r12
    pushq       %r13
    pushq       %r14
    pushq       %r15

    or          arg5, arg5
    jz          .shake256_squeeze_done

    mov         (arg6), arg6                    # arg6 - state.ctx

    # check for partially processed block
    movq        (8*100)(arg6), %r15             # s[100] - capacity
    orq         %r15, %r15
    jnz         .shake256_squeeze_no_init_permute


    movq        arg1, %r14
    mov         arg6, arg1
    call        keccak_1600_load_state_x4

    movq        %r14, arg1

    xorq        %rax, %rax
    jmp         .shake256_squeeze_loop

.balign 32
.shake256_squeeze_no_init_permute:
    # extract bytes: r10 - state/src, arg1-arg4 - output/dst, r12 - length = min(capacity, outlen), r11 - offset
    movq        arg6, %r10

    movq        %r15, %r12
    cmpq        %r15, arg5
    cmovb       arg5, %r12                      # %r12 = min(capacity, $outlen)

    sub         %r12, arg5                      # outlen -= length

    movl        $SHAKE256_RATE, %r11d
    subq        %r15, %r11                      # state offset

    subq        %r12, %r15                      # capacity -= length
    mov         %r15, (8*100)(arg6)             # update s[100]

    call        keccak_1600_extract_bytes_x4

    orq         %r15, %r15
    jnz         .shake256_squeeze_done          # check s[100] not zero

    movq        arg1, %r14                      # preserve arg1
    mov         arg6, arg1
    call        keccak_1600_load_state_x4

    movq        %r14, arg1
    xorq        %rax, %rax

.balign 32
.shake256_squeeze_loop:
    cmp         $SHAKE256_RATE, arg5            # outlen > r
    jb          .shake256_squeeze_final_extract

    call        keccak_1600_permute

    # Extract SHAKE256 rate bytes into the destination buffer
    extract_state_x4 arg1, arg2, arg3, arg4, %rax, (SHAKE256_RATE / 8)

    addq        $SHAKE256_RATE, %rax            # dst offset += r
    sub         $SHAKE256_RATE, arg5            # outlen -= r
    jmp         .shake256_squeeze_loop

.balign 32
.shake256_squeeze_final_extract:
    or          arg5, arg5
    jz          .shake256_squeeze_no_end_permute

    # update output pointers
    addq        %rax, arg1
    addq        %rax, arg2
    addq        %rax, arg3
    addq        %rax, arg4

    movl        $SHAKE256_RATE, %r15d
    subq        arg5, %r15
    mov         %r15, (8*100)(arg6)             # s[100] = c

    call        keccak_1600_permute

    movq        arg1, %r14
    mov         arg6, arg1
    call        keccak_1600_save_state_x4

    movq        %r14, arg1

    # extract bytes: r10 - state/src, arg1-arg4 - output/dst, r12 - length, r11 - offset = 0
    movq        arg6, %r10
    movq        arg5, %r12
    xorq        %r11, %r11
    call        keccak_1600_extract_bytes_x4

    jmp         .shake256_squeeze_done

.shake256_squeeze_no_end_permute:
    movq        $0, (8*100)(arg6)               # s[100] = 0
    mov         arg6, arg1
    call        keccak_1600_save_state_x4

.shake256_squeeze_done:
    vpxorq      %xmm16, %xmm16, %xmm16
    vmovdqa64   %ymm16, %ymm17
    vmovdqa64   %ymm16, %ymm18
    vmovdqa64   %ymm16, %ymm19
    vmovdqa64   %ymm16, %ymm20
    vmovdqa64   %ymm16, %ymm21
    vmovdqa64   %ymm16, %ymm22
    vmovdqa64   %ymm16, %ymm23
    vmovdqa64   %ymm16, %ymm24
    vmovdqa64   %ymm16, %ymm25
    vmovdqa64   %ymm16, %ymm26
    vmovdqa64   %ymm16, %ymm27
    vmovdqa64   %ymm16, %ymm28
    vmovdqa64   %ymm16, %ymm29
    vmovdqa64   %ymm16, %ymm30
    vmovdqa64   %ymm16, %ymm31
    vzeroall

    popq        %r15
    popq        %r14
    popq        %r13
    popq        %r12
    popq        %rbx
    popq        %rbp

    ret
.size   SHA3_shake256_x4_inc_squeeze_avx512vl,.-SHA3_shake256_x4_inc_squeeze_avx512vl


.section .rodata

# SHAKE128 and SHAKE256 use the same terminator byte
.balign 32
shake_terminator_byte_x4:
.byte 0, 0, 0, 0, 0, 0, 0, 0x80
.byte 0, 0, 0, 0, 0, 0, 0, 0x80
.byte 0, 0, 0, 0, 0, 0, 0, 0x80
.byte 0, 0, 0, 0, 0, 0, 0, 0x80

# SHAKE128 and SHAKE256 use the same multi-rate padding byte
.balign 8
# This is not a mistake and these 8 zero bytes are required here.
# Address is decremented depending on the offset within the state register.
.byte 0, 0, 0, 0, 0, 0, 0, 0
shake_msg_pad_x4:
.byte SHAKE_MRATE_PADDING, 0, 0, 0, 0, 0, 0, 0
.byte SHAKE_MRATE_PADDING, 0, 0, 0, 0, 0, 0, 0
.byte SHAKE_MRATE_PADDING, 0, 0, 0, 0, 0, 0, 0
.byte SHAKE_MRATE_PADDING, 0, 0, 0, 0, 0, 0, 0

.section .note.GNU-stack,"",%progbits
