# Copyright (c) 2025 Intel Corporation
#
# SPDX-License-Identifier: MIT

# Define arg registers
.equ arg1, %rdi
.equ arg2, %rsi
.equ arg3, %rdx
.equ arg4, %rcx

# Define SHA3 rates
.equ SHA3_256_RATE, 136
.equ SHA3_384_RATE, 104
.equ SHA3_512_RATE, 72
.equ SHAKE128_RATE, 168
.equ SHAKE256_RATE, 136

# Define SHA3 digest sizes
.equ SHA3_256_DIGEST_SZ, 32
.equ SHA3_384_DIGEST_SZ, 48
.equ SHA3_512_DIGEST_SZ, 64

# Define SHA3 EOM bytes
.equ SHA3_256_EOM, 0x06
.equ SHA3_384_EOM, 0x06
.equ SHA3_512_EOM, 0x06
.equ SHAKE128_EOM, 0x1F
.equ SHAKE256_EOM, 0x1F


# External utility functions
.extern	keccak_1600_permute
.extern	keccak_1600_init_state
.extern	keccak_1600_load_state
.extern	keccak_1600_save_state
.extern	keccak_1600_partial_add
.extern	keccak_1600_copy_with_padding
.extern	keccak_1600_copy_digest
.extern	keccak_1600_extract_bytes


# Define macros

# Absorb input bytes into state registers
#
# input [in]   message pointer
# offset [in]  message offset pointer
# rate [in]    SHA3 variant absorb rate
.macro absorb_bytes input, offset, rate
    vmovq   (\input, \offset, 1),   %xmm31
    vpxorq  %ymm31, %ymm0, %ymm0
    vmovq   8(\input, \offset, 1),  %xmm31
    vpxorq  %ymm31, %ymm1, %ymm1
    vmovq   16(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm2, %ymm2
    vmovq   24(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm3, %ymm3
    vmovq   32(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm4, %ymm4
    vmovq   40(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm5, %ymm5
    vmovq   48(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm6, %ymm6
    vmovq   56(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm7, %ymm7
    vmovq   64(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm8, %ymm8
    # SHA3_512 rate, 72 bytes
.if \rate > SHA3_512_RATE
    # SHA3_384 rate
    vmovq   72(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm9, %ymm9
    vmovq   80(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm10, %ymm10
    vmovq   88(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm11, %ymm11
    vmovq   96(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm12, %ymm12
.endif
.if \rate > SHA3_384_RATE
    # SHA3_256 and shake256 rate
    vmovq   104(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm13, %ymm13
    vmovq   112(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm14, %ymm14
    vmovq   120(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm15, %ymm15
    vmovq   128(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm16, %ymm16
.endif
.if \rate > SHA3_256_RATE
    # SHAKE128 rate
    vmovq   136(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm17, %ymm17
    vmovq   144(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm18, %ymm18
    vmovq   152(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm19, %ymm19
    vmovq   160(\input, \offset, 1), %xmm31
    vpxorq  %ymm31, %ymm20, %ymm20
.endif
.endm


# Store state from SIMD registers to memory
# State registers are kept in xmm0-xmm24
#
# output [in] destination pointer
# offset [in] destination offset
# n      [in] numerical values, number of 8-byte state registers to extract
.macro extract_state output, offset, n
    # SHA3 256
    vmovq   %xmm0,    0(\output, \offset)
    vmovq   %xmm1,    8(\output, \offset)
    vmovq   %xmm2,   16(\output, \offset)
    vmovq   %xmm3,   24(\output, \offset)
.if \n > 4
    # SHA3 384
    vmovq   %xmm4,   32(\output, \offset)
    vmovq   %xmm5,   40(\output, \offset)
.endif
.if \n > 6
    # SHA3 512
    vmovq   %xmm6,   48(\output, \offset)
    vmovq   %xmm7,   56(\output, \offset)
.endif
.if \n > 8
    # SHAKE 256
    vmovq   %xmm8,   64(\output, \offset)
    vmovq   %xmm9,   72(\output, \offset)
    vmovq   %xmm10,  80(\output, \offset)
    vmovq   %xmm11,  88(\output, \offset)
    vmovq   %xmm12,  96(\output, \offset)
    vmovq   %xmm13, 104(\output, \offset)
    vmovq   %xmm14, 112(\output, \offset)
    vmovq   %xmm15, 120(\output, \offset)
    vmovq   %xmm16, 128(\output, \offset)
.endif
.if \n > 17
    # SHAKE 128
    vmovq   %xmm17, 136(\output, \offset)
    vmovq   %xmm18, 144(\output, \offset)
    vmovq   %xmm19, 152(\output, \offset)
    vmovq   %xmm20, 160(\output, \offset)
.endif
.endm


# Process a message with SHA3-256/384/512 and store digest
#
# input:
#   rate [in] SHA3 rate
.macro sha3_complete rate
    pushq   %rbp
    pushq   %rbx
    pushq   %r12
    pushq   %r13
    pushq   %r14
    pushq   %r15
    subq    $8*32, %rsp

    mov     $\rate, %r9d                # Initialize the rate
    movq    arg3, %r11                  # copy message length to r11
    xorq    %r12, %r12                  # zero message offset

    # Initialize the state array to zero
    call    keccak_1600_init_state

    # Process the input message in blocks
.balign 32
1:  # main loop

    cmp     %r9, %r11
    jb      2f

    absorb_bytes arg2, %r12, \rate 

    subq    %r9, %r11                   # Subtract the rate from the remaining length
    addq    %r9, %r12                   # Adjust the pointer to the next block of the input message
    call    keccak_1600_permute         # Perform the Keccak permutation
    jmp     1b

.balign 32
2:  # main loop done

    movq    %rsp, %r13                  # dst pointer
    addq    arg2, %r12                  # src pointer
    # r11 is length in bytes already
    # r9 is rate in bytes already
    lea     sha3_eom(%rip), %r8
    call    keccak_1600_copy_with_padding

    # Add padded block to the state
    xorq    %r8, %r8
    absorb_bytes %rsp, %r8, \rate

    # Finalize the state and extract the output
    call    keccak_1600_permute

    # Clear the temporary buffer
    vpxorq      %ymm31, %ymm31, %ymm31
    vmovdqu64   %ymm31, (32*0)(%rsp)
    vmovdqu64   %ymm31, (32*1)(%rsp)
    vmovdqu64   %ymm31, (32*2)(%rsp)
    vmovdqu64   %ymm31, (32*3)(%rsp)
    vmovdqu64   %ymm31, (32*4)(%rsp)
    vmovdqu64   %ymm31, (32*5)(%rsp)
    vmovdqu64   %ymm31, (32*6)(%rsp)
    vmovdqu64   %ymm31, (32*7)(%rsp)

    # Store the state into the digest buffer
    xorq    %r8, %r8
.if \rate == SHA3_256_RATE
    extract_state arg1, %r8, 4
.endif
.if \rate == SHA3_384_RATE
    extract_state arg1, %r8, 6
.endif
.if \rate == SHA3_512_RATE
    extract_state arg1, %r8, 8
.endif

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

    addq    $8*32, %rsp
    popq    %r15
    popq    %r14
    popq    %r13
    popq    %r12
    popq    %rbx
    popq    %rbp
.endm


# Process a message with SHAKE-128/256 and store digest
#
# input:
#   rate [in] SHAKE rate
.macro shake_complete rate
    pushq   %rbp
    pushq   %rbx
    pushq   %r12
    pushq   %r13
    pushq   %r14
    pushq   %r15
    subq    $8*32, %rsp

    mov     $\rate, %r9d                    # Initialize the rate for SHAKE
    movq    arg4, %r11                      # copy message length to %r11
    xorq    %r12, %r12                      # zero message offset
    xorq    %r10, %r10

    # Initialize the state array to zero
    call    keccak_1600_init_state

    # Process the input message in blocks
.balign 32
1:  # main loop

    cmp     %r9, %r11
    jb      2f

    absorb_bytes arg3, %r12, \rate

    subq    %r9, %r11                       # Subtract the rate from the remaining length
    addq    %r9, %r12                       # Adjust the pointer to the next block of the input message
    call    keccak_1600_permute             # Perform the Keccak permutation

    jmp     1b

.balign 32
2:  # main loop done

    movq    %rsp, %r13                      # dst pointer
    addq    arg3, %r12                      # src pointer

    # r11 is length in bytes already
    # r9 is rate in bytes already
    lea     shake_eom(%rip), %r8
    call    keccak_1600_copy_with_padding

    # Add padded block to the state
    xorq    %r8, %r8
    absorb_bytes %rsp, %r8, \rate
    call    keccak_1600_permute             # Perform the Keccak permutation

    # Clear the temporary buffer
    vpxorq      %ymm31, %ymm31, %ymm31
    vmovdqu64   %ymm31, (32*0)(%rsp)
    vmovdqu64   %ymm31, (32*1)(%rsp)
    vmovdqu64   %ymm31, (32*2)(%rsp)
    vmovdqu64   %ymm31, (32*3)(%rsp)
    vmovdqu64   %ymm31, (32*4)(%rsp)
    vmovdqu64   %ymm31, (32*5)(%rsp)
    vmovdqu64   %ymm31, (32*6)(%rsp)
    vmovdqu64   %ymm31, (32*7)(%rsp)

.balign 32
3:  # xof loop

    cmp     %r9, arg2
    jb      4f

    extract_state arg1, %r10, (\rate / 8)   # Store the state into the digest buffer
    call    keccak_1600_permute             # Perform the Keccak permutation

    subq    %r9, arg2                       # Subtract the rate from the remaining length
    jz      5f                              # If equal, jump to the done label
    addq    %r9, %r10                       # Adjust the output digest pointer for the next block
    jmp     3b

.balign 32
4:  # store final block

    # Store the state for the last block of SHAKE in the temporary buffer
    xorq    %r8, %r8
    extract_state %rsp, %r8, (\rate / 8)

    # Copy digest from the buffer to the output buffer byte by byte
    lea     (arg1, %r10), %r13
    movq    %rsp, %r12

    # arg2 is length in bytes
    call    keccak_1600_copy_digest

    # Clear the temporary buffer
    vpxorq      %ymm31, %ymm31, %ymm31
    vmovdqu64   %ymm31, (32*0)(%rsp)
    vmovdqu64   %ymm31, (32*1)(%rsp)
    vmovdqu64   %ymm31, (32*2)(%rsp)
    vmovdqu64   %ymm31, (32*3)(%rsp)
    vmovdqu64   %ymm31, (32*4)(%rsp)
    vmovdqu64   %ymm31, (32*5)(%rsp)
    vmovdqu64   %ymm31, (32*6)(%rsp)
    vmovdqu64   %ymm31, (32*7)(%rsp)

5:  # done

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

    addq    $8*32, %rsp
    popq    %r15
    popq    %r14
    popq    %r13
    popq    %r12
    popq    %rbx
    popq    %rbp
.endm


# Absorb input into state
#
# input:
#   rate [in] SHA3 rate
.macro sha3_absorb rate
    pushq   %rbp
    pushq   %rbx
    pushq   %r12
    pushq   %r13
    pushq   %r14
    pushq   %r15

    movq    (arg1), arg1            # state.ctx into arg1

    # check for partially processed block
    movq    (8*25)(arg1), %r14
    orq     %r14, %r14              # s[25] == 0? 
    je      2f


    # process remaining bytes if message long enough
    movq    $\rate, %r12            # c = rate - s[25]
    subq    %r14, %r12              # %r12 = capacity

    cmp     %r12, arg3              # if mlen < capacity then cannot permute yet
    jb      1f                      # skip permute

    movq    arg3, %r10
    leaq    (arg1, %r14), %r13      # %r13 = state + s[25]
    movq    arg2, arg3
    call    keccak_1600_partial_add

    movq    %r10, arg3

    call    keccak_1600_load_state
    call    keccak_1600_permute

    movq    $0, (8*25)(arg1)        # clear s[25]
    jmp     3f                      # partial block done

1:  # skip permute

    movq    arg3, %r11              # copy message length to %r11
    xorq    %r12, %r12              # zero message offset
    addq    %r11, (8*25)(arg1)      # store partially processed length in s[25]
    addq    %r14, arg1              # state += s[25]
    jmp     6f

2:  # absorb start

    call    keccak_1600_load_state

3:  # partial block done

    movq    arg3, %r11              # copy message length to %r11
    xorq    %r12, %r12              # zero message offset

    # Process the input message in blocks
.balign 32
4:  # main absorb loop

    cmpq    $\rate, %r11            # compare mlen to rate
    jb      5f

    absorb_bytes arg2, %r12, \rate  # input

    subq    $\rate, %r11            # Subtract the rate from the remaining length
    addq    $\rate, %r12            # Adjust the pointer to the next block of the input message
    call    keccak_1600_permute     # Perform the Keccak permutation

    jmp 4b

.balign 32
5:  # absorb loop done

    call    keccak_1600_save_state

    addq    %r11, (8*25)(arg1)      # store partially processed length in s[25]

6:  # final partial add

    addq    %r12, arg2
    movq    arg1, %r13
    movq    %r11, %r12
    call    keccak_1600_partial_add

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
.endm


# Finalize processing state and store SHA3 digest in output buffer
#
# Input:
#   - output/arg1: pointer to the output buffer
#   - state/arg2: pointer to the state
.macro sha3_finalize rate, eom_byte
    pushq   %rbp
    pushq   %rbx
    pushq   %r12
    pushq   %r13
    pushq   %r14
    pushq   %r15

    movq    (arg2), arg2                # state.ctx into arg2
    movq    (8*25)(arg2), %r11          # load state offset from s[25]
    xorb    $\eom_byte, (arg2, %r11)    # add EOM byte
    xorb    $0x80, (\rate-1)(arg2)      # write EOM

    movq    arg1, %r15                  # save arg1
    movq    arg2, arg1
    call    keccak_1600_load_state

    movq    %r15, arg1

    # clobbers r13, r14 
    call    keccak_1600_permute

    # extract digest
    xorq    %r8, %r8
.if \rate == SHA3_256_RATE
    extract_state arg1, %r8, 4
.endif
.if \rate == SHA3_384_RATE
    extract_state arg1, %r8, 6
.endif
.if \rate == SHA3_512_RATE
    extract_state arg1, %r8, 8
.endif

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
.endm


# Absorb input into state
#
# input:
#   rate [in] SHA3 rate
.macro shake_absorb rate
    pushq   %rbp
    pushq   %rbx
    pushq   %r12
    pushq   %r13
    pushq   %r14
    pushq   %r15

    movq    (arg1), arg1                    # state.ctx into arg1

    # check for partially processed block
    movq    (8*25)(arg1), %r14
    orq     %r14, %r14                      # check if s[25] is 0
    je      2f


    # process remaining bytes if message long enough 
    movq    $\rate, %r12
    subq    %r14, %r12                      # %r12 = capacity = rate - s[25]

    cmp     %r12, arg3                      # if mlen <= capacity then no permute
    jbe     1f                              # skip permute

    subq    %r12, arg3

    # r13/state, arg2/input, r12/length
    leaq    (arg1, %r14), %r13              # %r13 = state + s[25]
    call    keccak_1600_partial_add         # arg2 is updated

    call    keccak_1600_load_state

    call    keccak_1600_permute

    movq    $0, (8*25)(arg1)                # clear s[25]
    jmp     3f

1:  # skip permute

    leaq    (arg3, %r14), %r10
    movq    %r10, (8*25)(arg1)              # s[25] += inlen
    # r13/state, arg2/input, r12/length
    leaq    (arg1, %r14), %r13              # state + s[24]
    movq    arg3, %r12
    call    keccak_1600_partial_add

    cmpq    $\rate, %r10                    # s[25] >= rate
    jb      6f

    call    keccak_1600_load_state
    call    keccak_1600_permute
    call    keccak_1600_save_state

    movq    $0, (8*25)(arg1)                # clear s[25]
    jmp     6f

2:  # main loop start

    call    keccak_1600_load_state

3:  # partial block done

    movq    arg3, %r11                      # copy message length to %r11
    xorq    %r12, %r12                      # zero message offset

    # Process the input message in blocks
.balign 32
4:  # main loop

    cmpq    $\rate, %r11                    # compare mlen to rate
    jb      5f

    absorb_bytes    arg2, %r12, \rate

    subq    $\rate, %r11                    # Subtract the rate from the remaining length
    addq    $\rate, %r12                    # Adjust the pointer to the next block of the input message
    call    keccak_1600_permute             # Perform the Keccak permutation

    jmp     4b                              # next loop

.balign 32
5:  # main loop done

    call    keccak_1600_save_state

    movq    %r11, (8*25)(arg1)              # update s[25]
    orq     %r11, %r11
    jz      6f

    # r13/state, arg2/input, r12/length
    addq    %r12, arg2
    movq    arg1, %r13
    movq    %r11, %r12
    call    keccak_1600_partial_add

6:  # done

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
.endm


# Squeeze bytes from state to output buffer
#
# Input:
#   - output/arg1: pointer to the output buffer for the message digest
#   - outlen/arg2: length of the output in bytes
#   - state/arg3: pointer to the state
.macro shake_squeeze rate
    pushq   %rbp
    pushq   %rbx
    pushq   %r12
    pushq   %r13
    pushq   %r14
    pushq   %r15

    or      arg2, arg2
    jz      5f

    movq    arg1, %rax                      # rotate arg1 with arg3
    movq    (arg3), arg1                    # arg1 - state.ctx
    movq    %rax, arg3                      # arg3 - output buffer

    # check for partially processed block
    movq    (8*25)(arg1), %r14              # s[25] - capacity
    orq     %r14, %r14
    jnz     1f

    call    keccak_1600_load_state

    jmp     2f

1:  # no init permute

    # extract bytes: r13 - state/src, r10 - output/dst, r12 - length = min(capacity, outlen)
    movl    $\rate, %r11d
    subq    %r14, %r11                      # state offset
    leaq    (arg1, %r11), %r13

    movq    arg3, %r10

    movq    %r14, %r12
    cmpq    %r14, arg2
    cmovb   arg2, %r12                      # %r12 = min(capacity, outlen)

    subq    %r12, arg2                      # outlen -= length
    subq    %r12, %r14                      # capacity -= length
    movq    %r14, (8*25)(arg1)              # update s[25]

    call    keccak_1600_extract_bytes

    orq     %r14, %r14
    jnz     5f                              # check if s[25] not 0

    movq    %r10, arg3                      # updated output buffer

    call    keccak_1600_load_state

.balign 32
2:  # main loop

    cmp     $\rate, arg2                    # outlen > r
    jb      3f

    call    keccak_1600_permute

    # Extract SHAKE rate bytes into the destination buffer
    xorq    %r8, %r8
    extract_state   arg3, %r8, (\rate / 8)

    addq    $\rate, arg3                    # dst += r
    subq    $\rate, arg2                    # outlen -= r
    jmp     2b

.balign 32
3:  # final extract

    or      arg2, arg2
    jz      4f                              # no end permute

    movl    $\rate, %r14d
    subq    arg2, %r14
    movq    %r14, (8*25)(arg1)              # s[25] = c

    call    keccak_1600_permute

    call    keccak_1600_save_state

    # extract bytes: r13 - state/src, r10 - output/dst, r12 - length
    movq    arg1, %r13
    movq    arg3, %r10
    movq    arg2, %r12
    call    keccak_1600_extract_bytes

    jmp     5f                              # jump to done

4:  # no end permute

    movq    $0, (8*25)(arg1)                # s[25] = c
    call    keccak_1600_save_state

5:  # done

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
.endm


.text

#
# Compact API
#

# -----------------------------------------------------------------------------
#
# void SHA3_sha3_256_avx512vl(uint8_t *output, const uint8_t *input, size_t inplen);
#
.globl  SHA3_sha3_256_avx512vl
.type   SHA3_sha3_256_avx512vl,@function
.hidden SHA3_sha3_256_avx512vl
.balign 32
SHA3_sha3_256_avx512vl:
    sha3_complete SHA3_256_RATE
    ret
.size   SHA3_sha3_256_avx512vl,.-SHA3_sha3_256_avx512vl

# -----------------------------------------------------------------------------
#
# void SHA3_sha3_384_avx512vl(uint8_t *output, const uint8_t *input, size_t inplen);
#
.globl  SHA3_sha3_384_avx512vl
.type   SHA3_sha3_384_avx512vl,@function
.hidden SHA3_sha3_384_avx512vl
.balign 32
SHA3_sha3_384_avx512vl:
    sha3_complete SHA3_384_RATE
    ret
.size   SHA3_sha3_384_avx512vl,.-SHA3_sha3_384_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_sha3_512_avx512vl(uint8_t *output, const uint8_t *input, size_t inplen);
#
.globl  SHA3_sha3_512_avx512vl
.type   SHA3_sha3_512_avx512vl,@function
.hidden SHA3_sha3_512_avx512vl
.balign 32
SHA3_sha3_512_avx512vl:
    sha3_complete SHA3_512_RATE
    ret
.size   SHA3_sha3_512_avx512vl,.-SHA3_sha3_512_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_shake128_avx512vl(uint8_t *output, size_t outlen, const uint8_t *input, size_t inplen);
#
.globl  SHA3_shake128_avx512vl
.type   SHA3_shake128_avx512vl,@function
.hidden SHA3_shake128_avx512vl
.balign 32
SHA3_shake128_avx512vl:
    shake_complete SHAKE128_RATE
    ret
.size   SHA3_shake128_avx512vl,.-SHA3_shake128_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_shake256_avx512vl(uint8_t *output, size_t outlen, const uint8_t *input, size_t inplen);
#
.globl  SHA3_shake256_avx512vl
.type   SHA3_shake256_avx512vl,@function
.hidden SHA3_shake256_avx512vl
.balign 32
SHA3_shake256_avx512vl:
    shake_complete SHAKE256_RATE
    ret
.size   SHA3_shake256_avx512vl,.-SHA3_shake256_avx512vl


#
# Init/Reset API
#

# -----------------------------------------------------------------------------
#
# void SHA3_sha3_256_inc_ctx_reset_avx512vl(OQS_SHA3_sha3_256_inc_ctx *state);
#
.globl  SHA3_sha3_256_inc_ctx_reset_avx512vl
.type   SHA3_sha3_256_inc_ctx_reset_avx512vl,@function
.hidden SHA3_sha3_256_inc_ctx_reset_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_sha3_384_inc_ctx_reset_avx512vl(OQS_SHA3_sha3_384_inc_ctx *state);
#
.globl  SHA3_sha3_384_inc_ctx_reset_avx512vl
.type   SHA3_sha3_384_inc_ctx_reset_avx512vl,@function
.hidden SHA3_sha3_384_inc_ctx_reset_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_sha3_512_inc_ctx_reset_avx512vl(OQS_SHA3_sha3_512_inc_ctx *state);
#
.globl  SHA3_sha3_512_inc_ctx_reset_avx512vl
.type   SHA3_sha3_512_inc_ctx_reset_avx512vl,@function
.hidden SHA3_sha3_512_inc_ctx_reset_avx512vl

# -----------------------------------------------------------------------------
#
# void SHA3_shake128_inc_ctx_reset_avx512vl(OQS_SHA3_shake128_inc_ctx *state);
#
.globl  SHA3_shake128_inc_ctx_reset_avx512vl
.type   SHA3_shake128_inc_ctx_reset_avx512vl,@function
.hidden SHA3_shake128_inc_ctx_reset_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_shake256_inc_ctx_reset_avx512vl(OQS_SHA3_shake256_inc_ctx *state);
#
.globl  SHA3_shake256_inc_ctx_reset_avx512vl
.type   SHA3_shake256_inc_ctx_reset_avx512vl,@function
.hidden SHA3_shake256_inc_ctx_reset_avx512vl


.balign 32
SHA3_sha3_256_inc_ctx_reset_avx512vl:
SHA3_sha3_384_inc_ctx_reset_avx512vl:
SHA3_sha3_512_inc_ctx_reset_avx512vl:
SHA3_shake128_inc_ctx_reset_avx512vl:
SHA3_shake256_inc_ctx_reset_avx512vl:
    movq        (arg1), arg1            # load arg1.ctx
    vpxorq      %xmm0, %xmm0, %xmm0
    vmovdqu64   %ymm0,   0(arg1)        # clear 200 bytes of arg1s
    vmovdqu64   %ymm0,  32(arg1)
    vmovdqu64   %ymm0,  64(arg1)
    vmovdqu64   %ymm0,  96(arg1)
    vmovdqu64   %ymm0, 128(arg1)
    vmovdqu64   %ymm0, 160(arg1)
    vmovdqu64   %xmm0, 192(arg1)        # also clear additional 8 bytes s[25]
    ret
.size   SHA3_sha3_256_inc_ctx_reset_avx512vl,.-SHA3_sha3_256_inc_ctx_reset_avx512vl
.size   SHA3_sha3_384_inc_ctx_reset_avx512vl,.-SHA3_sha3_384_inc_ctx_reset_avx512vl
.size   SHA3_sha3_512_inc_ctx_reset_avx512vl,.-SHA3_sha3_512_inc_ctx_reset_avx512vl
.size   SHA3_shake128_inc_ctx_reset_avx512vl,.-SHA3_shake128_inc_ctx_reset_avx512vl
.size   SHA3_shake256_inc_ctx_reset_avx512vl,.-SHA3_shake256_inc_ctx_reset_avx512vl


#
# Absorb API
#

# -----------------------------------------------------------------------------
#
# void SHA3_sha3_256_inc_absorb_avx512vl(OQS_SHA3_sha3_256_inc_ctx *state, const uint8_t *input, size_t inlen);
#
.globl  SHA3_sha3_256_inc_absorb_avx512vl
.type   SHA3_sha3_256_inc_absorb_avx512vl,@function
.hidden SHA3_sha3_256_inc_absorb_avx512vl
.balign 32
SHA3_sha3_256_inc_absorb_avx512vl:
    sha3_absorb SHA3_256_RATE
    ret
.size   SHA3_sha3_256_inc_absorb_avx512vl,.-SHA3_sha3_256_inc_absorb_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_sha3_384_inc_absorb_avx512vl(OQS_SHA3_sha3_384_inc_ctx *state, const uint8_t *input, size_t inlen);
#
.globl  SHA3_sha3_384_inc_absorb_avx512vl
.type   SHA3_sha3_384_inc_absorb_avx512vl,@function
.hidden SHA3_sha3_384_inc_absorb_avx512vl
.balign 32
SHA3_sha3_384_inc_absorb_avx512vl:
    sha3_absorb SHA3_384_RATE
    ret
.size   SHA3_sha3_384_inc_absorb_avx512vl,.-SHA3_sha3_384_inc_absorb_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_sha3_512_inc_absorb_avx512vl(OQS_SHA3_sha3_512_inc_ctx *state, const uint8_t *input, size_t inlen);
#
.globl  SHA3_sha3_512_inc_absorb_avx512vl
.type   SHA3_sha3_512_inc_absorb_avx512vl,@function
.hidden SHA3_sha3_512_inc_absorb_avx512vl
.balign 32
SHA3_sha3_512_inc_absorb_avx512vl:
    sha3_absorb SHA3_512_RATE
    ret
.size   SHA3_sha3_512_inc_absorb_avx512vl,.-SHA3_sha3_512_inc_absorb_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_shake128_inc_absorb_avx512vl(OQS_SHA3_shake128_inc_ctx *state, const uint8_t *input, size_t inlen);
#
.globl  SHA3_shake128_inc_absorb_avx512vl
.type   SHA3_shake128_inc_absorb_avx512vl,@function
.hidden SHA3_shake128_inc_absorb_avx512vl
.balign 32
SHA3_shake128_inc_absorb_avx512vl:
    shake_absorb SHAKE128_RATE
    ret
.size   SHA3_shake128_inc_absorb_avx512vl,.-SHA3_shake128_inc_absorb_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_shake256_inc_absorb_avx512vl(OQS_SHA3_shake256_inc_ctx *state, const uint8_t *input, size_t inlen);
#
.globl  SHA3_shake256_inc_absorb_avx512vl
.type   SHA3_shake256_inc_absorb_avx512vl,@function
.hidden SHA3_shake256_inc_absorb_avx512vl
.balign 32
SHA3_shake256_inc_absorb_avx512vl:
    shake_absorb SHAKE256_RATE
    ret
.size   SHA3_shake256_inc_absorb_avx512vl,.-SHA3_shake256_inc_absorb_avx512vl


#
# Finalize API
#

# -----------------------------------------------------------------------------
#
# void SHA3_sha3_256_inc_finalize_avx512vl(uint8_t *output, OQS_SHA3_sha3_256_inc_ctx *state);
#
.globl  SHA3_sha3_256_inc_finalize_avx512vl
.type   SHA3_sha3_256_inc_finalize_avx512vl,@function
.hidden SHA3_sha3_256_inc_finalize_avx512vl
.balign 32
SHA3_sha3_256_inc_finalize_avx512vl:
    sha3_finalize SHA3_256_RATE, SHA3_256_EOM
    ret
.size   SHA3_sha3_256_inc_finalize_avx512vl,.-SHA3_sha3_256_inc_finalize_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_sha3_384_inc_finalize_avx512vl(uint8_t *output, OQS_SHA3_sha3_384_inc_ctx *state);
#
.globl  SHA3_sha3_384_inc_finalize_avx512vl
.type   SHA3_sha3_384_inc_finalize_avx512vl,@function
.hidden SHA3_sha3_384_inc_finalize_avx512vl
.balign 32
SHA3_sha3_384_inc_finalize_avx512vl:
    sha3_finalize SHA3_384_RATE, SHA3_384_EOM
    ret
.size   SHA3_sha3_384_inc_finalize_avx512vl,.-SHA3_sha3_384_inc_finalize_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_sha3_512_inc_finalize_avx512vl(uint8_t *output, OQS_SHA3_sha3_512_inc_ctx *state);
#
.globl  SHA3_sha3_512_inc_finalize_avx512vl
.type   SHA3_sha3_512_inc_finalize_avx512vl,@function
.hidden SHA3_sha3_512_inc_finalize_avx512vl
.balign 32
SHA3_sha3_512_inc_finalize_avx512vl:
    sha3_finalize SHA3_512_RATE, SHA3_512_EOM
    ret
.size   SHA3_sha3_512_inc_finalize_avx512vl,.-SHA3_sha3_512_inc_finalize_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_shake128_inc_finalize_avx512vl(OQS_SHA3_shake128_inc_ctx *state);
#
.globl  SHA3_shake128_inc_finalize_avx512vl
.type   SHA3_shake128_inc_finalize_avx512vl,@function
.hidden SHA3_shake128_inc_finalize_avx512vl
.balign 32
SHA3_shake128_inc_finalize_avx512vl:
    movq    (arg1), arg1                    # state.ctx into %rdi
    movq    (8*25)(arg1), %r11              # load state offset from s[25]
    xorb    $SHAKE128_EOM, (arg1, %r11)     # add EOM 
    xorb    $0x80, (SHAKE128_RATE-1)(arg1)  # write EOM 
    movq    $0, (8*25)(arg1)                # clear s[25]
    ret
.size   SHA3_shake128_inc_finalize_avx512vl,.-SHA3_shake128_inc_finalize_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_shake256_inc_finalize_avx512vl(OQS_SHA3_shake256_inc_ctx *state);
#
.globl  SHA3_shake256_inc_finalize_avx512vl
.type   SHA3_shake256_inc_finalize_avx512vl,@function
.hidden SHA3_shake256_inc_finalize_avx512vl
.balign 32
SHA3_shake256_inc_finalize_avx512vl:
    movq    (arg1), arg1                    # state.ctx into %rdi
    movq    (8*25)(arg1), %r11              # load state offset from s[25]
    xorb    $SHAKE256_EOM, (arg1, %r11)     # add EOM 
    xorb    $0x80, (SHAKE256_RATE-1)(arg1)  # write EOM 
    movq    $0, (8*25)(arg1)                # clear s[25]
    ret
.size   SHA3_shake256_inc_finalize_avx512vl,.-SHA3_shake256_inc_finalize_avx512vl


#
# Squeeze API
#

# -----------------------------------------------------------------------------
#
# void SHA3_shake128_inc_squeeze_avx512vl(uint8_t *output, size_t outlen, OQS_SHA3_shake128_inc_ctx *state);
#
.globl  SHA3_shake128_inc_squeeze_avx512vl
.type   SHA3_shake128_inc_squeeze_avx512vl,@function
.hidden SHA3_shake128_inc_squeeze_avx512vl
.balign 32
SHA3_shake128_inc_squeeze_avx512vl:
    shake_squeeze SHAKE128_RATE
    ret
.size   SHA3_shake128_inc_squeeze_avx512vl,.-SHA3_shake128_inc_squeeze_avx512vl


# -----------------------------------------------------------------------------
#
# void SHA3_shake256_inc_squeeze_avx512vl(uint8_t *output, size_t outlen, OQS_SHA3_shake256_inc_ctx *state);
#
.globl  SHA3_shake256_inc_squeeze_avx512vl
.type   SHA3_shake256_inc_squeeze_avx512vl,@function
.hidden SHA3_shake256_inc_squeeze_avx512vl
.balign 32
SHA3_shake256_inc_squeeze_avx512vl:
    shake_squeeze SHAKE256_RATE
    ret
.size   SHA3_shake256_inc_squeeze_avx512vl,.-SHA3_shake256_inc_squeeze_avx512vl


.section .rodata

sha3_eom:
.byte  0x06

shake_eom:
.byte  0x1F

.section .note.GNU-stack,"",%progbits
