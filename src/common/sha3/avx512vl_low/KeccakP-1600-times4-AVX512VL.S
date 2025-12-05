# Copyright (c) 2025 Intel Corporation
#
# SPDX-License-Identifier: MIT

# Define arg registers
.equ arg1,  %rdi
.equ arg2,  %rsi
.equ arg3,  %rdx
.equ arg4,  %rcx
.equ arg5,  %r8

.text

# Loads Keccak state from memory into registers
#
# input:  arg1 - state pointer
# output: ymm0-ymm24
.globl  keccak_1600_load_state_x4
.type   keccak_1600_load_state_x4,@function
.hidden keccak_1600_load_state_x4
.balign 32
keccak_1600_load_state_x4:
    vmovdqu64   (32*0)(arg1),  %ymm0
    vmovdqu64   (32*1)(arg1),  %ymm1
    vmovdqu64   (32*2)(arg1),  %ymm2
    vmovdqu64   (32*3)(arg1),  %ymm3
    vmovdqu64   (32*4)(arg1),  %ymm4
    vmovdqu64   (32*5)(arg1),  %ymm5
    vmovdqu64   (32*6)(arg1),  %ymm6
    vmovdqu64   (32*7)(arg1),  %ymm7
    vmovdqu64   (32*8)(arg1),  %ymm8
    vmovdqu64   (32*9)(arg1),  %ymm9
    vmovdqu64   (32*10)(arg1), %ymm10
    vmovdqu64   (32*11)(arg1), %ymm11
    vmovdqu64   (32*12)(arg1), %ymm12
    vmovdqu64   (32*13)(arg1), %ymm13
    vmovdqu64   (32*14)(arg1), %ymm14
    vmovdqu64   (32*15)(arg1), %ymm15
    vmovdqu64   (32*16)(arg1), %ymm16
    vmovdqu64   (32*17)(arg1), %ymm17
    vmovdqu64   (32*18)(arg1), %ymm18
    vmovdqu64   (32*19)(arg1), %ymm19
    vmovdqu64   (32*20)(arg1), %ymm20
    vmovdqu64   (32*21)(arg1), %ymm21
    vmovdqu64   (32*22)(arg1), %ymm22
    vmovdqu64   (32*23)(arg1), %ymm23
    vmovdqu64   (32*24)(arg1), %ymm24
    ret
.size   keccak_1600_load_state_x4,.-keccak_1600_load_state_x4


# Saves Keccak state to memory
#
# input:  arg1 - state pointer
#         ymm0-ymm24 - Keccak state registers
# output: memory from [arg1] to [arg1 + 100*8]
.globl  keccak_1600_save_state_x4
.type   keccak_1600_save_state_x4,@function
.hidden keccak_1600_save_state_x4
.balign 32
keccak_1600_save_state_x4:
    vmovdqu64   %ymm0,  (32*0)(arg1)
    vmovdqu64   %ymm1,  (32*1)(arg1)
    vmovdqu64   %ymm2,  (32*2)(arg1)
    vmovdqu64   %ymm3,  (32*3)(arg1)
    vmovdqu64   %ymm4,  (32*4)(arg1)
    vmovdqu64   %ymm5,  (32*5)(arg1)
    vmovdqu64   %ymm6,  (32*6)(arg1)
    vmovdqu64   %ymm7,  (32*7)(arg1)
    vmovdqu64   %ymm8,  (32*8)(arg1)
    vmovdqu64   %ymm9,  (32*9)(arg1)
    vmovdqu64   %ymm10, (32*10)(arg1)
    vmovdqu64   %ymm11, (32*11)(arg1)
    vmovdqu64   %ymm12, (32*12)(arg1)
    vmovdqu64   %ymm13, (32*13)(arg1)
    vmovdqu64   %ymm14, (32*14)(arg1)
    vmovdqu64   %ymm15, (32*15)(arg1)
    vmovdqu64   %ymm16, (32*16)(arg1)
    vmovdqu64   %ymm17, (32*17)(arg1)
    vmovdqu64   %ymm18, (32*18)(arg1)
    vmovdqu64   %ymm19, (32*19)(arg1)
    vmovdqu64   %ymm20, (32*20)(arg1)
    vmovdqu64   %ymm21, (32*21)(arg1)
    vmovdqu64   %ymm22, (32*22)(arg1)
    vmovdqu64   %ymm23, (32*23)(arg1)
    vmovdqu64   %ymm24, (32*24)(arg1)
    ret
.size   keccak_1600_save_state_x4,.-keccak_1600_save_state_x4


# Add input data to state when message length is less than rate
#
# input:
#    r10  - state pointer to absorb into (clobbered)
#    arg2 - message pointer lane 0 (updated on output)
#    arg3 - message pointer lane 1 (updated on output)
#    arg4 - message pointer lane 2 (updated on output)
#    arg5 - message pointer lane 3 (updated on output)
#    r12  - length in bytes (clobbered on output)
# output:
#    memory - state from [r10] to [r10 + 4*r12 - 1]
# clobbered:
#    rax, rbx, r15, k1, ymm31-ymm29
.globl  keccak_1600_partial_add_x4
.type   keccak_1600_partial_add_x4,@function
.hidden keccak_1600_partial_add_x4
.balign 32
keccak_1600_partial_add_x4:
    movq        (8*100)(%r10), %rax
    testl       $7, %eax
    jz          .start_aligned_to_4x8

    # start offset is not aligned to register size
    # - calculate remaining capacity of the register
    # - get the min between length and the capacity of the register
    # - perform partial add on the register
    # - once aligned to the register go into ymm loop

    movq        %rax, %r15                              # %r15 = s[100]

    andl        $7, %eax
    negl        %eax
    addl        $8, %eax                                # register capacity = 8 - (offset % 8)
    cmpl        %eax, %r12d
    cmovb       %r12d, %eax                             # %eax = min(register capacity, $length)

    leaq        byte_kmask_0_to_7(%rip), %rbx
    kmovb       (%rbx,%rax), %k1                        # message load mask

    movq        %r15, %rbx
    andl        $~7, %ebx
    leaq        (%r10, %rbx,4), %r10                    # get to state starting register

    movq        %r15, %rbx
    andl        $7, %ebx

    vmovdqu8    (%r10), %ymm31                          # load & store / allocate SB for the register
    vmovdqu8    %ymm31, (%r10)

    vmovdqu8    (arg2), %xmm31{%k1}{z}                  # Read 1 to 7 bytes from lane 0
    vmovdqu8    (8*0)(%r10,%rbx), %xmm30{%k1}{z}        # Read 1 to 7 bytes from state reg lane 0
    vpxorq      %xmm30, %xmm31, %xmm31
    vmovdqu8    %xmm31, (8*0)(%r10,%rbx){%k1}           # Write 1 to 7 bytes to state reg lane 0

    vmovdqu8    (arg3), %xmm31{%k1}{z}                  # Read 1 to 7 bytes from lane 1
    vmovdqu8    (8*1)(%r10,%rbx), %xmm30{%k1}{z}        # Read 1 to 7 bytes from state reg lane 1
    vpxorq      %xmm30, %xmm31, %xmm31
    vmovdqu8    %xmm31, (8*1)(%r10,%rbx){%k1}           # Write 1 to 7 bytes to state reg lane 1

    vmovdqu8    (arg4), %xmm31{%k1}{z}                  # Read 1 to 7 bytes from lane 2
    vmovdqu8    (8*2)(%r10,%rbx), %xmm30{%k1}{z}        # Read 1 to 7 bytes from state reg lane 2
    vpxorq      %xmm30, %xmm31, %xmm31
    vmovdqu8    %xmm31, (8*2)(%r10,%rbx){%k1}           # Write 1 to 7 bytes to state reg lane 2

    vmovdqu8    (arg5), %xmm31{%k1}{z}                  # Read 1 to 7 bytes from lane 3
    vmovdqu8    (8*3)(%r10,%rbx), %xmm30{%k1}{z}        # Read 1 to 7 bytes from state reg lane 3
    vpxorq      %xmm30, %xmm31, %xmm31
    vmovdqu8    %xmm31, (8*3)(%r10,%rbx){%k1}           # Write 1 to 7 bytes to state reg lane 3

    subq        %rax, %r12
    jz          .zero_bytes

    addq        %rax, arg2
    addq        %rax, arg3
    addq        %rax, arg4
    addq        %rax, arg5
    addq        $32, %r10
    xorq        %rax, %rax
    jmp         .ymm_loop

.start_aligned_to_4x8:
    leaq        (%r10,%rax,4), %r10
    xorq        %rax, %rax

.balign 32
.ymm_loop:
    cmpl        $8, %r12d
    jb          .lt_8_bytes

    vmovq       (arg2, %rax), %xmm31                    # Read 8 bytes from lane 0
    vpinsrq     $1, (arg3, %rax), %xmm31, %xmm31        # Read 8 bytes from lane 1
    vmovq       (arg4, %rax), %xmm30                    # Read 8 bytes from lane 2
    vpinsrq     $1, (arg5, %rax),%xmm30, %xmm30         # Read 8 bytes from lane 3
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31
    vpxorq      (%r10,%rax,4), %ymm31, %ymm31           # Add data with the state
    vmovdqu64   %ymm31, (%r10,%rax,4)
    addq        $8, %rax
    subq        $8, %r12
    jz          .zero_bytes

    jmp         .ymm_loop

.balign 32
.zero_bytes:
    addq        %rax, arg2
    addq        %rax, arg3
    addq        %rax, arg4
    addq        %rax, arg5
    ret

.balign 32
.lt_8_bytes:
    addq        %rax, arg2
    addq        %rax, arg3
    addq        %rax, arg4
    addq        %rax, arg5
    leaq        (%r10,%rax,4), %r10

    leaq        byte_kmask_0_to_7(%rip), %rbx
    kmovb       (%rbx,%r12), %k1                        # message load mask

    vmovdqu8    (arg2), %xmm31{%k1}{z}                  # Read 1 to 7 bytes from lane 0
    vmovdqu8    (arg3), %xmm30{%k1}{z}                  # Read 1 to 7 bytes from lane 1
    vpunpcklqdq %xmm30, %xmm31, %xmm31                  # Interleave data from lane 0 and lane 1
    vmovdqu8    (arg4), %xmm30{%k1}{z}                  # Read 1 to 7 bytes from lane 2
    vmovdqu8    (arg5), %xmm29{%k1}{z}                  # Read 1 to 7 bytes from lane 3
    vpunpcklqdq %xmm29, %xmm30, %xmm30                  # Interleave data from lane 2 and lane 3
    vinserti32x4 $1, %xmm30, %ymm31, %ymm31

    vpxorq      (%r10), %ymm31, %ymm31                  # Add data to the state
    vmovdqu64   %ymm31, (%r10)                          # Update state in memory

    addq        %r12, arg2                              # increment message pointer lane 0
    addq        %r12, arg3                              # increment message pointer lane 1
    addq        %r12, arg4                              # increment message pointer lane 2
    addq        %r12, arg5                              # increment message pointer lane 3
    ret
.size   keccak_1600_partial_add_x4,.-keccak_1600_partial_add_x4


# Extract bytes from state and write to outputs
#
# input:
#    r10  - state pointer to start extracting from (clobbered)
#    arg1 - output pointer lane 0 (updated on output)
#    arg2 - output pointer lane 1 (updated on output)
#    arg3 - output pointer lane 2 (updated on output)
#    arg4 - output pointer lane 3 (updated on output)
#    r12  - length in bytes (clobbered on output)
#    r11  - state offset to start extract from
# output:
#    memory - output lane 0 from [arg1] to [arg1 + r12 - 1]
#    memory - output lane 1 from [arg2] to [arg2 + r12 - 1]
#    memory - output lane 2 from [arg3] to [arg3 + r12 - 1]
#    memory - output lane 3 from [arg4] to [arg4 + r12 - 1]
# clobbered:
#    rax, rbx, k1, ymm31-ymm30
.globl  keccak_1600_extract_bytes_x4
.type   keccak_1600_extract_bytes_x4,@function
.hidden keccak_1600_extract_bytes_x4
.balign 32
keccak_1600_extract_bytes_x4:
    orq         %r12, %r12
    jz          .extract_zero_bytes

    testl       $7, %r11d
    jz          .extract_start_aligned_to_4x8

    # extract offset is not aligned to the register size (8 bytes)
    # - calculate remaining capacity of the register
    # - get the min between length to extract and register capacity
    # - perform partial add on the register

    movq        %r11, %rax                             # %rax = %r11 = offset in the state

    andl        $7, %eax
    negl        %eax
    addl        $8, %eax                                # register capacity = 8 - (offset % 8)
    cmpl        %eax, %r12d
    cmovb       %r12d, %eax                             # %eax = min(register capacity, length)

    leaq        byte_kmask_0_to_7(%rip), %rbx
    kmovb       (%rbx,%rax), %k1                        # message store mask

    movq        %r11, %rbx
    andl        $~7, %ebx
    leaq        (%r10,%rbx,4), %r10                     # get to state starting register

    movq        %r11, %rbx
    andl        $7, %ebx

    vmovdqu8    (8*0)(%r10,%rbx), %xmm31{%k1}{z}        # Read 1 to 7 bytes from state reg lane 0
    vmovdqu8    %xmm31, (arg1){%k1}                     # Write 1 to 7 bytes to lane 0 output

    vmovdqu8    (8*1)(%r10,%rbx), %xmm31{%k1}{z}        # Read 1 to 7 bytes from state reg lane 1
    vmovdqu8    %xmm31, (arg2){%k1}                     # Write 1 to 7 bytes to lane 1 output

    vmovdqu8    (8*2)(%r10,%rbx), %xmm31{%k1}{z}        # Read 1 to 7 bytes from state reg lane 2
    vmovdqu8    %xmm31, (arg3){%k1}                     # Write 1 to 7 bytes to lane 2 output

    vmovdqu8    (8*3)(%r10,%rbx), %xmm31{%k1}{z}        # Read 1 to 7 bytes from state reg 
    vmovdqu8    %xmm31, (arg4){%k1}                     # Write 1 to 7 bytes to lane 3 output

    # increment output registers
    addq        %rax, arg1
    addq        %rax, arg2
    addq        %rax, arg3
    addq        %rax, arg4

    # decrement length to extract
    subq        %rax, %r12
    jz          .extract_zero_bytes

    # there is more data to extract, update state register pointer and go to the main loop
    addq        $32, %r10
    xorq        %rax, %rax
    jmp         .ymm_loop

.extract_start_aligned_to_4x8:
    leaq        (%r10,%r11,4), %r10
    xorq        %rax, %rax

.balign 32
.extract_ymm_loop:
    cmpq        $8, %r12
    jb          .extract_lt_8_bytes

    vmovdqu64   (%r10), %xmm31
    vmovdqu64   (16)(%r10), %xmm30
    vmovq       %xmm31, (arg1, %rax)
    vpextrq     $1, %xmm31, (arg2, %rax)
    vmovq       %xmm30, (arg3, %rax)
    vpextrq     $1, %xmm30, (arg4, %rax)
    addq        $8, %rax
    subq        $8, %r12
    jz          .zero_bytes_left

    addq        $4*8, %r10
    jmp         .extract_ymm_loop


.balign 32
.zero_bytes_left:
    # increment output pointers
    addq        %rax, arg1
    addq        %rax, arg2
    addq        %rax, arg3
    addq        %rax, arg4
.extract_zero_bytes:
    ret

.balign 32
.extract_lt_8_bytes:
    addq        %rax, arg1
    addq        %rax, arg2
    addq        %rax, arg3
    addq        %rax, arg4

    leaq        byte_kmask_0_to_7(%rip), %rax
    kmovb       (%rax,%r12), %k1                # k1 is the mask of message bytes to read

    vmovq       (0*8)(%r10), %xmm31             # Read 8 bytes from state lane 0
    vmovdqu8    %xmm31, (arg1){%k1}             # Extract 1 to 7 bytes of state into output 0
    vmovq       (1*8)(%r10), %xmm31             # Read 8 bytes from state lane 1
    vmovdqu8    %xmm31, (arg2){%k1}             # Extract 1 to 7 bytes of state into output 1
    vmovq       (2*8)(%r10), %xmm31             # Read 8 bytes from state lane 2
    vmovdqu8    %xmm31, (arg3){%k1}             # Extract 1 to 7 bytes of state into output 2
    vmovq       (3*8)(%r10), %xmm31             # Read 8 bytes from state lane 3
    vmovdqu8    %xmm31, (arg4){%k1}             # Extract 1 to 7 bytes of state into output 3

    # increment output pointers
    addq        %r12, arg1
    addq        %r12, arg2
    addq        %r12, arg3
    addq        %r12, arg4
    ret
.size   keccak_1600_extract_bytes_x4,.-keccak_1600_extract_bytes_x4

.section .rodata

.balign 8
byte_kmask_0_to_7:
    .byte       0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f  # 0xff should never happen

.section .note.GNU-stack,"",%progbits
