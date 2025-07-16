# Copyright (c) 2025 Intel Corporation
#
# SPDX-License-Identifier: MIT

# Define arg registers
.equ arg1, %rdi
.equ arg2, %rsi

.text

# Initialized Keccak state in registers
#
# input:
# output: xmm0-xmm24
.globl  keccak_1600_init_state
.type   keccak_1600_init_state,@function
.hidden keccak_1600_init_state
.balign 32
keccak_1600_init_state:
    vpxorq      %xmm0, %xmm0, %xmm0
    vpxorq      %xmm1, %xmm1, %xmm1
    vpxorq      %xmm2, %xmm2, %xmm2
    vmovdqa64   %ymm0, %ymm3
    vmovdqa64   %ymm0, %ymm4
    vmovdqa64   %ymm0, %ymm5
    vmovdqa64   %ymm0, %ymm6
    vmovdqa64   %ymm0, %ymm7
    vmovdqa64   %ymm0, %ymm8
    vmovdqa64   %ymm0, %ymm9
    vmovdqa64   %ymm0, %ymm10
    vmovdqa64   %ymm0, %ymm11
    vmovdqa64   %ymm0, %ymm12
    vmovdqa64   %ymm0, %ymm13
    vmovdqa64   %ymm0, %ymm14
    vmovdqa64   %ymm0, %ymm15
    vmovdqa64   %ymm0, %ymm16
    vmovdqa64   %ymm0, %ymm17
    vmovdqa64   %ymm0, %ymm18
    vmovdqa64   %ymm0, %ymm19
    vmovdqa64   %ymm0, %ymm20
    vmovdqa64   %ymm0, %ymm21
    vmovdqa64   %ymm0, %ymm22
    vmovdqa64   %ymm0, %ymm23
    vmovdqa64   %ymm0, %ymm24
    ret
.size   keccak_1600_init_state,.-keccak_1600_init_state


# Loads Keccak state from memory into registers
#
# input:  arg1 - state pointer
# output: xmm0-xmm24
.globl  keccak_1600_load_state
.type   keccak_1600_load_state,@function
.hidden keccak_1600_load_state
.balign 32
keccak_1600_load_state:
    vmovq   (8*0)(arg1),  %xmm0
    vmovq   (8*1)(arg1),  %xmm1
    vmovq   (8*2)(arg1),  %xmm2
    vmovq   (8*3)(arg1),  %xmm3
    vmovq   (8*4)(arg1),  %xmm4
    vmovq   (8*5)(arg1),  %xmm5
    vmovq   (8*6)(arg1),  %xmm6
    vmovq   (8*7)(arg1),  %xmm7
    vmovq   (8*8)(arg1),  %xmm8
    vmovq   (8*9)(arg1),  %xmm9
    vmovq   (8*10)(arg1), %xmm10
    vmovq   (8*11)(arg1), %xmm11
    vmovq   (8*12)(arg1), %xmm12
    vmovq   (8*13)(arg1), %xmm13
    vmovq   (8*14)(arg1), %xmm14
    vmovq   (8*15)(arg1), %xmm15
    vmovq   (8*16)(arg1), %xmm16
    vmovq   (8*17)(arg1), %xmm17
    vmovq   (8*18)(arg1), %xmm18
    vmovq   (8*19)(arg1), %xmm19
    vmovq   (8*20)(arg1), %xmm20
    vmovq   (8*21)(arg1), %xmm21
    vmovq   (8*22)(arg1), %xmm22
    vmovq   (8*23)(arg1), %xmm23
    vmovq   (8*24)(arg1), %xmm24
    ret
.size   keccak_1600_load_state,.-keccak_1600_load_state


# Saves Keccak state to memory
#
# input:  arg1 - state pointer
#         xmm0-xmm24 - Keccak state registers
# output: memory from [arg1] to [arg1 + 25*8]
.globl  keccak_1600_save_state
.type   keccak_1600_save_state,@function
.hidden keccak_1600_save_state
.balign 32
keccak_1600_save_state:
    vmovq   %xmm0,  (8*0)(arg1)
    vmovq   %xmm1,  (8*1)(arg1)
    vmovq   %xmm2,  (8*2)(arg1)
    vmovq   %xmm3,  (8*3)(arg1)
    vmovq   %xmm4,  (8*4)(arg1)
    vmovq   %xmm5,  (8*5)(arg1)
    vmovq   %xmm6,  (8*6)(arg1)
    vmovq   %xmm7,  (8*7)(arg1)
    vmovq   %xmm8,  (8*8)(arg1)
    vmovq   %xmm9,  (8*9)(arg1)
    vmovq   %xmm10, (8*10)(arg1)
    vmovq   %xmm11, (8*11)(arg1)
    vmovq   %xmm12, (8*12)(arg1)
    vmovq   %xmm13, (8*13)(arg1)
    vmovq   %xmm14, (8*14)(arg1)
    vmovq   %xmm15, (8*15)(arg1)
    vmovq   %xmm16, (8*16)(arg1)
    vmovq   %xmm17, (8*17)(arg1)
    vmovq   %xmm18, (8*18)(arg1)
    vmovq   %xmm19, (8*19)(arg1)
    vmovq   %xmm20, (8*20)(arg1)
    vmovq   %xmm21, (8*21)(arg1)
    vmovq   %xmm22, (8*22)(arg1)
    vmovq   %xmm23, (8*23)(arg1)
    vmovq   %xmm24, (8*24)(arg1)
    ret
.size   keccak_1600_save_state,.-keccak_1600_save_state


# Add input data to state when message length is less than rate
#
# input:
#    r13  - state
#    arg2 - message pointer (updated on output)
#    r12  - length (clobbered on output)
# output:
#    memory - state from [r13] to [r13 + r12 - 1]
# clobbered:
#    rax, k1, ymm31
.globl  keccak_1600_partial_add
.type   keccak_1600_partial_add,@function
.hidden keccak_1600_partial_add
.balign 32
keccak_1600_partial_add:
.partial_add_loop:
    cmpq        $32, %r12
    jb          .lt_32_bytes

    vmovdqu64   (arg2), %ymm31
    vpxorq      (%r13), %ymm31, %ymm31
    vmovdqu64   %ymm31, (%r13)
    addq        $32, arg2
    addq        $32, %r13
    subq        $32, %r12
    jz          .partial_add_done
    jmp         .partial_add_loop

.lt_32_bytes:
    xorq        %rax, %rax
    bts         %r12, %rax
    decq        %rax
    kmovq       %rax, %k1               # k1 is the mask of message bytes to read
    vmovdqu8    (arg2), %ymm31{%k1}{z}  # Read 0 to 31 bytes
    vpxorq      (%r13), %ymm31, %ymm31
    vmovdqu8    %ymm31, (%r13){%k1}
    addq        %r12, arg2              # Increment message pointer

.partial_add_done:
    ret
.size   keccak_1600_partial_add,.-keccak_1600_partial_add


# Extract bytes from state and write to output
#
# input:
#    r13  - state
#    r10  - output pointer (updated on output)
#    r12  - length (clobbered on output)
# output:
#    memory - output from [r10] to [r10 + r12 - 1]
# clobbered:
#    rax, k1, ymm31
.globl  keccak_1600_extract_bytes
.type   keccak_1600_extract_bytes,@function
.hidden keccak_1600_extract_bytes
.balign 32
keccak_1600_extract_bytes:
.extract_32_byte_loop:
    cmpq        $32, %r12
    jb          .extract_lt_32_bytes

    vmovdqu64   (%r13), %ymm31
    vmovdqu64   %ymm31, (%r10)
    addq        $32, %r13
    addq        $32, %r10
    subq        $32, %r12
    jz          .extract_done
    jmp         .extract_32_byte_loop

.extract_lt_32_bytes:
    xorq        %rax, %rax
    bts         %r12, %rax
    decq        %rax
    kmovq       %rax, %k1                   # k1 is the mask of the last message bytes
    vmovdqu8    (%r13), %ymm31{%k1}{z}      # Read 0 to 31 bytes
    vmovdqu8    %ymm31, (%r10){%k1}
    addq        %r12, %r10                  # Increment output pointer
.extract_done:
    ret
.size   keccak_1600_extract_bytes,.-keccak_1600_extract_bytes

# Copy partial block message into temporary buffer, add padding byte and EOM bit
#
#    r13  [in/out] destination pointer
#    r12  [in/out] source pointer
#    r11  [in/out] length in bytes
#    r9   [in] rate
#    r8   [in] pointer to the padding byte
# output:
#    memory - output from [r13] to [r13 + r11 - 1], [r13 + r11] padding, [r13 + r9 - 1] EOM
# clobbered:
#    rax, r15, k1, k2, ymm31
.globl  keccak_1600_copy_with_padding
.type   keccak_1600_copy_with_padding, @function
.hidden keccak_1600_copy_with_padding
.balign 32
keccak_1600_copy_with_padding:
    # Clear the temporary buffer
    vpxorq      %ymm31, %ymm31, %ymm31
    vmovdqu64   %ymm31, (32*0)(%r13)
    vmovdqu64   %ymm31, (32*1)(%r13)
    vmovdqu64   %ymm31, (32*2)(%r13)
    vmovdqu64   %ymm31, (32*3)(%r13)
    vmovdqu64   %ymm31, (32*4)(%r13)
    vmovdqu64   %ymm31, (32*5)(%r13)
    vmovdqu64   %ymm31, (32*6)(%r13)
    vmovdqu64   %ymm31, (32*7)(%r13)

    xorq    %r15, %r15
.balign 32
.copy32_with_padding_loop:
    cmpq        $32, %r11                   # Check at least 32 remaining
    jb          .partial32_with_padding     # If no, then do final copy with padding

    vmovdqu64   (%r12,%r15), %ymm31
    vmovdqu64   %ymm31, (%r13,%r15)
    subq        $32, %r11                   # Decrement the remaining length
    addq        $32, %r15                   # Increment offset
    jmp         .copy32_with_padding_loop

.partial32_with_padding:
    xorq        %rax, %rax
    bts         %r11, %rax
    kmovq       %rax, %k2                   # k2 is mask of the 1st byte after the message
    decq        %rax
    kmovq       %rax, %k1                   # k1 is the mask of the last message bytes
    vmovdqu8    (%r12,%r15), %ymm31{%k1}{z} # Read 0 to 31 bytes
    vpbroadcastb (%r8), %ymm31{%k2}         # Add padding
    vmovdqu64   %ymm31, (%r13,%r15)         # Store whole 32 bytes
    xorb        $0x80, (-1)(%r13,%r9)       # EOM bit - XOR the last byte of the block
    ret
.size   keccak_1600_copy_with_padding,.-keccak_1600_copy_with_padding


.globl  keccak_1600_copy_digest
.type   keccak_1600_copy_digest, @function
.hidden keccak_1600_copy_digest
.balign 32
keccak_1600_copy_digest:
 .copy32_digest_loop:
    cmp         $32, arg2                   # Check at least 32 remaining
    jb          .partial32                  # If no, then copy final bytes

    vmovdqu64   (%r12), %ymm31
    vmovdqu64   %ymm31, (%r13)
    addq        $32, %r13                   # Increment destination pointer
    addq        $32, %r12                   # Increment source pointer
    subq        $32, arg2                   # Decrement the remaining length
    jz          .done
    jmp         .copy32_digest_loop

.partial32:
    xorq        %rax, %rax
    bts         arg2, %rax
    dec         %rax
    kmovq       %rax, %k1                   # k1 is the mask of the last message bytes
    vmovdqu8    (%r12), %ymm31{%k1}{z}      # Read 0 to 31 bytes
    vmovdqu8    %ymm31, (%r13){%k1}         # Store 0 to 31 bytes
.done:
    ret
.size   keccak_1600_copy_digest,.-keccak_1600_copy_digest

# Perform Keccak permutation
#
# YMM registers 0 to 24 are used as Keccak state registers.
# This function, as is, can work on 1 to 4 independent states at the same time.
#
# There is no clear boundary between Theta, Rho, Pi, Chi and Iota steps.
# Instructions corresponding to these steps overlap for better efficiency.
#
# ymm0-ymm24    [in/out]    Keccak state registers (one SIMD per one state register)
# ymm25-ymm31   [clobbered] temporary SIMD registers
# r13           [clobbered] used for round tracking
# r14           [clobbered] used for access to SHA3 constant table
.globl  keccak_1600_permute
.type   keccak_1600_permute,@function
.hidden keccak_1600_permute
.balign 32
keccak_1600_permute:
    movl    $24, %r13d                          # 24 rounds
    leaq    sha3_rc(%rip), %r14                 # Load the address of the SHA3 round constants

.balign 32
keccak_rnd_loop:
    # Theta step

    # Compute column parities
    # C[5] = [0, 0, 0, 0, 0]
    # for x in 0 to 4:
    #     C[x] = state[x][0] XOR state[x][1] XOR state[x][2] XOR state[x][3] XOR state[x][4]

    vmovdqa64   %ymm0, %ymm25
    vpternlogq  $0x96, %ymm5, %ymm10, %ymm25
    vmovdqa64   %ymm1, %ymm26
    vpternlogq  $0x96, %ymm11, %ymm6, %ymm26
    vmovdqa64   %ymm2, %ymm27
    vpternlogq  $0x96, %ymm12, %ymm7, %ymm27

    vmovdqa64   %ymm3, %ymm28
    vpternlogq  $0x96, %ymm13, %ymm8, %ymm28
    vmovdqa64   %ymm4, %ymm29
    vpternlogq  $0x96, %ymm14, %ymm9, %ymm29
    vpternlogq  $0x96, %ymm20, %ymm15, %ymm25

    vpternlogq  $0x96, %ymm21, %ymm16, %ymm26
    vpternlogq  $0x96, %ymm22, %ymm17, %ymm27
    vpternlogq  $0x96, %ymm23, %ymm18, %ymm28

    # Start computing D values and keep computing column parity
    # D[5] = [0, 0, 0, 0, 0]
    # for x in 0 to 4:
    #     D[x] = C[(x+4) mod 5] XOR ROTATE_LEFT(C[(x+1) mod 5], 1)

    vprolq      $1, %ymm26, %ymm30
    vprolq      $1, %ymm27, %ymm31
    vpternlogq  $0x96, %ymm24, %ymm19, %ymm29

    # Continue computing D values and apply Theta
    # for x in 0 to 4:
    #     for y in 0 to 4:
    #         state[x][y] = state[x][y] XOR D[x]

    vpternlogq  $0x96, %ymm30, %ymm29, %ymm0
    vpternlogq  $0x96, %ymm30, %ymm29, %ymm10
    vpternlogq  $0x96, %ymm30, %ymm29, %ymm20

    vpternlogq  $0x96, %ymm30, %ymm29, %ymm5
    vpternlogq  $0x96, %ymm30, %ymm29, %ymm15
    vprolq      $1, %ymm28, %ymm30

    vpternlogq  $0x96, %ymm31, %ymm25, %ymm6
    vpternlogq  $0x96, %ymm31, %ymm25, %ymm16
    vpternlogq  $0x96, %ymm31, %ymm25, %ymm1

    vpternlogq  $0x96, %ymm31, %ymm25, %ymm11
    vpternlogq  $0x96, %ymm31, %ymm25, %ymm21
    vprolq      $1, %ymm29, %ymm31

    vpbroadcastq    (%r14), %ymm29              # Load the round constant into ymm29 (Iota)
    addq    $8, %r14                            # Increment the pointer to the next round constant

    vpternlogq  $0x96, %ymm30, %ymm26, %ymm12
    vpternlogq  $0x96, %ymm30, %ymm26, %ymm7
    vpternlogq  $0x96, %ymm30, %ymm26, %ymm22

    vpternlogq  $0x96, %ymm30, %ymm26, %ymm17
    vpternlogq  $0x96, %ymm30, %ymm26, %ymm2
    vprolq      $1, %ymm25, %ymm30

    # Rho step
    # Keep applying Theta and start Rho step
    #
    # ROTATION_OFFSETS[5][5] = [
    #     [0, 1, 62, 28, 27],
    #     [36, 44, 6, 55, 20],
    #     [3, 10, 43, 25, 39],
    #     [41, 45, 15, 21, 8],
    #     [18, 2, 61, 56, 14] ]
    #
    # for x in 0 to 4:
    #     for y in 0 to 4:
    #         state[x][y] = ROTATE_LEFT(state[x][y], ROTATION_OFFSETS[x][y])

    vpternlogq  $0x96, %ymm31, %ymm27, %ymm3
    vpternlogq  $0x96, %ymm31, %ymm27, %ymm13
    vpternlogq  $0x96, %ymm31, %ymm27, %ymm23

    vprolq      $44, %ymm6, %ymm6
    vpternlogq  $0x96, %ymm31, %ymm27, %ymm18
    vpternlogq  $0x96, %ymm31, %ymm27, %ymm8

    vprolq      $43, %ymm12, %ymm12
    vprolq      $21, %ymm18, %ymm18
    vpternlogq  $0x96, %ymm30, %ymm28, %ymm24

    vprolq      $14, %ymm24, %ymm24
    vprolq      $28, %ymm3, %ymm3
    vpternlogq  $0x96, %ymm30, %ymm28, %ymm9

    vprolq      $20, %ymm9, %ymm9
    vprolq      $3, %ymm10, %ymm10
    vpternlogq  $0x96, %ymm30, %ymm28, %ymm19

    vprolq      $45, %ymm16, %ymm16
    vprolq      $61, %ymm22, %ymm22
    vpternlogq  $0x96, %ymm30, %ymm28, %ymm4

    vprolq      $1, %ymm1, %ymm1
    vprolq      $6, %ymm7, %ymm7
    vpternlogq  $0x96, %ymm30, %ymm28, %ymm14

    # Continue with Rho and start Pi and Chi steps at the same time
    # Ternary logic 0xD2 is used for Chi step
    #
    # for x in 0 to 4:
    #     for y in 0 to 4:
    #         state[x][y] = state[x][y] XOR ((NOT state[(x+1) mod 5][y]) AND state[(x+2) mod 5][y])

    vprolq      $25, %ymm13, %ymm13
    vprolq      $8, %ymm19, %ymm19
    vmovdqa64   %ymm0, %ymm30
    vpternlogq  $0xD2, %ymm12, %ymm6, %ymm30

    vprolq      $18, %ymm20, %ymm20
    vprolq      $27, %ymm4, %ymm4
    vpxorq      %ymm29, %ymm30, %ymm30          # Iota step

    vprolq      $36, %ymm5, %ymm5
    vprolq      $10, %ymm11, %ymm11
    vmovdqa64   %ymm6, %ymm31
    vpternlogq  $0xD2, %ymm18, %ymm12, %ymm31

    vprolq      $15, %ymm17, %ymm17
    vprolq      $56, %ymm23, %ymm23
    vpternlogq  $0xD2, %ymm24, %ymm18, %ymm12

    vprolq      $62, %ymm2, %ymm2
    vprolq      $55, %ymm8, %ymm8
    vpternlogq  $0xD2, %ymm0, %ymm24, %ymm18

    vprolq      $39, %ymm14, %ymm14
    vprolq      $41, %ymm15, %ymm15
    vpternlogq  $0xD2, %ymm6, %ymm0, %ymm24
    vmovdqa64   %ymm30, %ymm0
    vmovdqa64   %ymm31, %ymm6

    vprolq      $2, %ymm21, %ymm21
    vmovdqa64   %ymm3, %ymm30
    vpternlogq  $0xD2, %ymm10, %ymm9, %ymm30
    vmovdqa64   %ymm9, %ymm31
    vpternlogq  $0xD2, %ymm16, %ymm10, %ymm31

    vpternlogq  $0xD2, %ymm22, %ymm16, %ymm10
    vpternlogq  $0xD2, %ymm3, %ymm22, %ymm16
    vpternlogq  $0xD2, %ymm9, %ymm3, %ymm22
    vmovdqa64   %ymm30, %ymm3
    vmovdqa64   %ymm31, %ymm9

    vmovdqa64   %ymm1, %ymm30
    vpternlogq  $0xD2, %ymm13, %ymm7, %ymm30
    vmovdqa64   %ymm7, %ymm31
    vpternlogq  $0xD2, %ymm19, %ymm13, %ymm31
    vpternlogq  $0xD2, %ymm20, %ymm19, %ymm13

    vpternlogq  $0xD2, %ymm1, %ymm20, %ymm19
    vpternlogq  $0xD2, %ymm7, %ymm1, %ymm20
    vmovdqa64   %ymm30, %ymm1
    vmovdqa64   %ymm31, %ymm7
    vmovdqa64   %ymm4, %ymm30
    vpternlogq  $0xD2, %ymm11, %ymm5, %ymm30

    vmovdqa64   %ymm5, %ymm31
    vpternlogq  $0xD2, %ymm17, %ymm11, %ymm31
    vpternlogq  $0xD2, %ymm23, %ymm17, %ymm11
    vpternlogq  $0xD2, %ymm4, %ymm23, %ymm17

    vpternlogq  $0xD2, %ymm5, %ymm4, %ymm23
    vmovdqa64   %ymm30, %ymm4
    vmovdqa64   %ymm31, %ymm5
    vmovdqa64   %ymm2, %ymm30
    vpternlogq  $0xD2, %ymm14, %ymm8, %ymm30
    vmovdqa64   %ymm8, %ymm31
    vpternlogq  $0xD2, %ymm15, %ymm14, %ymm31

    vpternlogq  $0xD2, %ymm21, %ymm15, %ymm14
    vpternlogq  $0xD2, %ymm2, %ymm21, %ymm15
    vpternlogq  $0xD2, %ymm8, %ymm2, %ymm21
    vmovdqa64   %ymm30, %ymm2
    vmovdqa64   %ymm31, %ymm8

    # Complete the steps and get updated state registers in ymm0 to ymm24
    vmovdqa64   %ymm3,  %ymm30
    vmovdqa64   %ymm18, %ymm3
    vmovdqa64   %ymm17, %ymm18
    vmovdqa64   %ymm11, %ymm17
    vmovdqa64   %ymm7,  %ymm11
    vmovdqa64   %ymm10, %ymm7
    vmovdqa64   %ymm1,  %ymm10
    vmovdqa64   %ymm6,  %ymm1
    vmovdqa64   %ymm9,  %ymm6
    vmovdqa64   %ymm22, %ymm9
    vmovdqa64   %ymm14, %ymm22
    vmovdqa64   %ymm20, %ymm14
    vmovdqa64   %ymm2,  %ymm20
    vmovdqa64   %ymm12, %ymm2
    vmovdqa64   %ymm13, %ymm12
    vmovdqa64   %ymm19, %ymm13
    vmovdqa64   %ymm23, %ymm19
    vmovdqa64   %ymm15, %ymm23
    vmovdqa64   %ymm4,  %ymm15
    vmovdqa64   %ymm24, %ymm4
    vmovdqa64   %ymm21, %ymm24
    vmovdqa64   %ymm8,  %ymm21
    vmovdqa64   %ymm16, %ymm8
    vmovdqa64   %ymm5,  %ymm16
    vmovdqa64   %ymm30, %ymm5

    decl        %r13d                # Decrement the round counter
    jnz         keccak_rnd_loop      # Jump to the start of the loop if r13d is not zero
    ret
.size   keccak_1600_permute,.-keccak_1600_permute

.section .rodata

.balign 64
sha3_rc:
# SHA3 round constants
# These constants are used in each round of the Keccak permutation
.quad 0x0000000000000001, 0x0000000000008082
.quad 0x800000000000808a, 0x8000000080008000
.quad 0x000000000000808b, 0x0000000080000001
.quad 0x8000000080008081, 0x8000000000008009
.quad 0x000000000000008a, 0x0000000000000088
.quad 0x0000000080008009, 0x000000008000000a
.quad 0x000000008000808b, 0x800000000000008b
.quad 0x8000000000008089, 0x8000000000008003
.quad 0x8000000000008002, 0x8000000000000080
.quad 0x000000000000800a, 0x800000008000000a
.quad 0x8000000080008081, 0x8000000000008080
.quad 0x0000000080000001, 0x8000000080008008

.section .note.GNU-stack,"",%progbits
