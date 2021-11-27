
.align 2
.global PQCLEAN_SABER_AARCH64_asm_1_to_16
.global _PQCLEAN_SABER_AARCH64_asm_1_to_16
#ifndef __clang__
.type PQCLEAN_SABER_AARCH64_asm_1_to_16, %function
#endif
PQCLEAN_SABER_AARCH64_asm_1_to_16:
_PQCLEAN_SABER_AARCH64_asm_1_to_16:

    mov x15, #8
    _1_to_16_outer_loop:

    ldr w2, [x1], #4

    mov x11, #4
    _1_to_16_inner_loop:

    sbfx w3, w2, #0, #1
    strh w3, [x0], #2
    sbfx w4, w2, #1, #1
    strh w4, [x0], #2
    sbfx w5, w2, #2, #1
    strh w5, [x0], #2
    sbfx w6, w2, #3, #1
    strh w6, [x0], #2
    sbfx w7, w2, #4, #1
    strh w7, [x0], #2
    sbfx w8, w2, #5, #1
    strh w8, [x0], #2
    sbfx w9, w2, #6, #1
    strh w9, [x0], #2
    sbfx w10, w2, #7, #1
    strh w10, [x0], #2

    lsr w2, w2, #8

    sub x11, x11, #1
    cbnz x11, _1_to_16_inner_loop

    sub x15, x15, #1
    cbnz x15, _1_to_16_outer_loop

    br lr


.align 2
.global PQCLEAN_SABER_AARCH64_asm_4_to_16
.global _PQCLEAN_SABER_AARCH64_asm_4_to_16
#ifndef __clang__
.type PQCLEAN_SABER_AARCH64_asm_4_to_16, %function
#endif
PQCLEAN_SABER_AARCH64_asm_4_to_16:
_PQCLEAN_SABER_AARCH64_asm_4_to_16:

    mov x7, #32
    _4_to_16_loop:

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #4
    strh w3, [x0], #2
    sbfx w4, w2, #4, #4
    strh w4, [x0], #2
    sbfx w5, w2, #8, #4
    strh w5, [x0], #2
    sbfx w6, w2, #12, #4
    strh w6, [x0], #2

    sbfx w3, w2, #16, #4
    strh w3, [x0], #2
    sbfx w4, w2, #20, #4
    strh w4, [x0], #2
    sbfx w5, w2, #24, #4
    strh w5, [x0], #2
    sbfx w6, w2, #28, #4
    strh w6, [x0], #2

    sub x7, x7, #1
    cbnz x7, _4_to_16_loop

    br lr


.align 2
.global PQCLEAN_SABER_AARCH64_asm_10_to_32
.global _PQCLEAN_SABER_AARCH64_asm_10_to_32
#ifndef __clang__
.type PQCLEAN_SABER_AARCH64_asm_10_to_32, %function
#endif
PQCLEAN_SABER_AARCH64_asm_10_to_32:
_PQCLEAN_SABER_AARCH64_asm_10_to_32:

    mov x7, #16
    _10_to_32_loop:

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #10
    str w3, [x0], #4
    sbfx w4, w2, #10, #10
    str w4, [x0], #4
    sbfx w5, w2, #20, #10
    str w5, [x0], #4
    lsr w6, w2, #30    

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #8
    lsl w3, w3, #2
    orr w3, w3, w6
    str w3, [x0], #4
    sbfx w4, w2, #8, #10
    str w4, [x0], #4
    sbfx w5, w2, #18, #10
    str w5, [x0], #4
    lsr w6, w2, #28

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #6
    lsl w3, w3, #4
    orr w3, w3, w6
    str w3, [x0], #4
    sbfx w4, w2, #6, #10
    str w4, [x0], #4
    sbfx w5, w2, #16, #10
    str w5, [x0], #4
    lsr w6, w2, #26

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #4
    lsl w3, w3, #6
    orr w3, w3, w6
    str w3, [x0], #4
    sbfx w4, w2, #4, #10
    str w4, [x0], #4
    sbfx w5, w2, #14, #10
    str w5, [x0], #4
    lsr w6, w2, #24

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #2
    lsl w3, w3, #8
    orr w3, w3, w6
    str w3, [x0], #4
    sbfx w4, w2, #2, #10
    str w4, [x0], #4
    sbfx w5, w2, #12, #10
    str w5, [x0], #4
    sbfx w6, w2, #22, #10
    str w6, [x0], #4

    sub x7, x7, #1
    cbnz x7, _10_to_32_loop

    br lr

.align 2
.global PQCLEAN_SABER_AARCH64_asm_13_to_32
.global _PQCLEAN_SABER_AARCH64_asm_13_to_32
#ifndef __clang__
.type PQCLEAN_SABER_AARCH64_asm_13_to_32, %function
#endif
PQCLEAN_SABER_AARCH64_asm_13_to_32:
_PQCLEAN_SABER_AARCH64_asm_13_to_32:

    mov x7, #8
    _13_to_32_loop:

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #13
    str w3, [x0], #4
    sbfx w4, w2, #13, #13
    str w4, [x0], #4
    lsr w5, w2, #26

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #7
    lsl w3, w3, #6
    orr w3, w3, w5
    str w3, [x0], #4
    sbfx w4, w2, #7, #13
    str w4, [x0], #4
    lsr w5, w2, #20

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #1
    lsl w3, w3, #12
    orr w3, w3, w5
    str w3, [x0], #4
    sbfx w4, w2, #1, #13
    str w4, [x0], #4
    sbfx w5, w2, #14, #13
    str w5, [x0], #4
    lsr w5, w2, #27

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #8
    lsl w3, w3, #5
    orr w3, w3, w5
    str w3, [x0], #4
    sbfx w4, w2, #8, #13
    str w4, [x0], #4
    lsr w5, w2, #21

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #2
    lsl w3, w3, #11
    orr w3, w3, w5
    str w3, [x0], #4
    sbfx w4, w2, #2, #13
    str w4, [x0], #4
    sbfx w5, w2, #15, #13
    str w5, [x0], #4
    lsr w5, w2, #28

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #9
    lsl w3, w3, #4
    orr w3, w3, w5
    str w3, [x0], #4
    sbfx w4, w2, #9, #13
    str w4, [x0], #4
    lsr w5, w2, #22

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #3
    lsl w3, w3, #10
    orr w3, w3, w5
    str w3, [x0], #4
    sbfx w4, w2, #3, #13
    str w4, [x0], #4
    sbfx w5, w2, #16, #13
    str w5, [x0], #4
    lsr w5, w2, #29

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #10
    lsl w3, w3, #3
    orr w3, w3, w5
    str w3, [x0], #4
    sbfx w4, w2, #10, #13
    str w4, [x0], #4
    lsr w5, w2, #23

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #4
    lsl w3, w3, #9
    orr w3, w3, w5
    str w3, [x0], #4
    sbfx w4, w2, #4, #13
    str w4, [x0], #4
    sbfx w5, w2, #17, #13
    str w5, [x0], #4
    lsr w5, w2, #30

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #11
    lsl w3, w3, #2
    orr w3, w3, w5
    str w3, [x0], #4
    sbfx w4, w2, #11, #13
    str w4, [x0], #4
    lsr w5, w2, #24

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #5
    lsl w3, w3, #8
    orr w3, w3, w5
    str w3, [x0], #4
    sbfx w4, w2, #5, #13
    str w4, [x0], #4
    sbfx w5, w2, #18, #13
    str w5, [x0], #4
    lsr w5, w2, #31

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #12
    lsl w3, w3, #1
    orr w3, w3, w5
    str w3, [x0], #4
    sbfx w4, w2, #12, #13
    str w4, [x0], #4
    lsr w5, w2, #25

    ldr w2, [x1], #4

    sbfx w3, w2, #0, #6
    lsl w3, w3, #7
    orr w3, w3, w5
    str w3, [x0], #4
    sbfx w4, w2, #6, #13
    str w4, [x0], #4
    sbfx w5, w2, #19, #13
    str w5, [x0], #4

    sub x7, x7, #1
    cbnz x7, _13_to_32_loop

    br lr


.align 2
.global PQCLEAN_SABER_AARCH64_asm_16_to_32
.global _PQCLEAN_SABER_AARCH64_asm_16_to_32
#ifndef __clang__
.type PQCLEAN_SABER_AARCH64_asm_16_to_32, %function
#endif
PQCLEAN_SABER_AARCH64_asm_16_to_32:
_PQCLEAN_SABER_AARCH64_asm_16_to_32:

    mov x7, #128
    _sbfx_loop:

    ldr w4, [x1], #4
    sbfx w5, w4, #0, #13
    sbfx w6, w4, #16, #13
    str w5, [x0], #4
    str w6, [x0], #4

    sub x7, x7, #1
    cbnz x7, _sbfx_loop

    br lr






