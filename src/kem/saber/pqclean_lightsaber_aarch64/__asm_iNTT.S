
#include "macros.inc"

.align 2
.global PQCLEAN_LIGHTSABER_AARCH64_asm_intt_SIMD_top
.global _PQCLEAN_LIGHTSABER_AARCH64_asm_intt_SIMD_top
#ifndef __clang__
.type PQCLEAN_LIGHTSABER_AARCH64_asm_intt_SIMD_top, %function
#endif
PQCLEAN_LIGHTSABER_AARCH64_asm_intt_SIMD_top:
_PQCLEAN_LIGHTSABER_AARCH64_asm_intt_SIMD_top:

    push_all
    Q         .req w20
    src0      .req  x0
    des0      .req  x1
    src1      .req  x2
    des1      .req  x3
    table     .req x28
    counter   .req x19

    ldr Q, [x2]

    mov table, x1

    add des0, src0, #0
    add src1, src0, #512
    add des1, src0, #512

    ld1 {v20.4S, v21.4S, v22.4S, v23.4S}, [table], #64

    mov v20.S[0], Q

    ld1 { v0.4S}, [src0], #16
    ld1 { v1.4S}, [src0], #16
    ld1 { v2.4S}, [src0], #16
    ld1 { v3.4S}, [src0], #16
    ld1 { v4.4S}, [src0], #16
    ld1 { v5.4S}, [src0], #16
    ld1 { v6.4S}, [src0], #16
    ld1 { v7.4S}, [src0], #16

    ld1 { v8.4S}, [src1], #16
    ld1 { v9.4S}, [src1], #16
    ld1 {v10.4S}, [src1], #16
    ld1 {v11.4S}, [src1], #16
    ld1 {v12.4S}, [src1], #16
    ld1 {v13.4S}, [src1], #16
    ld1 {v14.4S}, [src1], #16
    ld1 {v15.4S}, [src1], #16

    qq_add_sub v16, v17, v18, v19,  v1,  v3,  v5,  v7,  v0,  v2,  v4,  v6,  v1,  v3,  v5,  v7
    qq_add_sub v28, v29, v30, v31,  v9, v11, v13, v15,  v8, v10, v12, v14,  v9, v11, v13, v15

    qq_add_sub  v0,  v4,  v8, v12,  v2,  v6, v10, v14, v16, v18, v28, v30, v17, v19, v29, v31

    dq_butterfly_top  v1,  v5,  v3,  v7, v16, v17, v20, v21, 2, 3, v21, 2, 3

    dq_butterfly_mixed  v1,  v5,  v3,  v7, v16, v17,  v9, v13, v11, v15, v18, v19, v20, v21, 2, 3, v21, 2, 3, v21, 2, 3, v21, 2, 3
    dq_butterfly_mixed  v9, v13, v11, v15, v18, v19,  v0,  v1,  v4,  v5, v28, v29, v20, v21, 2, 3, v21, 2, 3, v22, 0, 1, v22, 2, 3
    dq_butterfly_mixed  v0,  v1,  v4,  v5, v28, v29,  v2,  v3,  v6,  v7, v30, v31, v20, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3
    dq_butterfly_mixed  v2,  v3,  v6,  v7, v30, v31,  v8,  v9, v12, v13, v16, v17, v20, v23, 0, 1, v23, 2, 3, v22, 0, 1, v22, 2, 3
    dq_butterfly_mixed  v8,  v9, v12, v13, v16, v17, v10, v11, v14, v15, v18, v19, v20, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3
    dq_butterfly_bot v10, v11, v14, v15, v18, v19, v20, v23, 0, 1, v23, 2, 3

    mov counter, #3
    _intt_top_loop:

    st1 { v0.4S}, [des0], #16
    ld1 { v0.4S}, [src0], #16
    st1 { v1.4S}, [des0], #16
    ld1 { v1.4S}, [src0], #16
    st1 { v2.4S}, [des0], #16
    ld1 { v2.4S}, [src0], #16
    st1 { v3.4S}, [des0], #16
    ld1 { v3.4S}, [src0], #16
    st1 { v4.4S}, [des0], #16
    ld1 { v4.4S}, [src0], #16
    st1 { v5.4S}, [des0], #16
    ld1 { v5.4S}, [src0], #16
    st1 { v6.4S}, [des0], #16
    ld1 { v6.4S}, [src0], #16
    st1 { v7.4S}, [des0], #16
    ld1 { v7.4S}, [src0], #16

    st1 { v8.4S}, [des1], #16
    ld1 { v8.4S}, [src1], #16
    st1 { v9.4S}, [des1], #16
    ld1 { v9.4S}, [src1], #16
    st1 {v10.4S}, [des1], #16
    ld1 {v10.4S}, [src1], #16
    st1 {v11.4S}, [des1], #16
    ld1 {v11.4S}, [src1], #16
    st1 {v12.4S}, [des1], #16
    ld1 {v12.4S}, [src1], #16
    st1 {v13.4S}, [des1], #16
    ld1 {v13.4S}, [src1], #16
    st1 {v14.4S}, [des1], #16
    ld1 {v14.4S}, [src1], #16
    st1 {v15.4S}, [des1], #16
    ld1 {v15.4S}, [src1], #16

    qq_add_sub v16, v17, v18, v19,  v1,  v3,  v5,  v7,  v0,  v2,  v4,  v6,  v1,  v3,  v5,  v7
    qq_add_sub v28, v29, v30, v31,  v9, v11, v13, v15,  v8, v10, v12, v14,  v9, v11, v13, v15

    qq_add_sub  v0,  v4,  v8, v12,  v2,  v6, v10, v14, v16, v18, v28, v30, v17, v19, v29, v31

    dq_butterfly_top  v1,  v5,  v3,  v7, v16, v17, v20, v21, 2, 3, v21, 2, 3

    dq_butterfly_mixed  v1,  v5,  v3,  v7, v16, v17,  v9, v13, v11, v15, v18, v19, v20, v21, 2, 3, v21, 2, 3, v21, 2, 3, v21, 2, 3
    dq_butterfly_mixed  v9, v13, v11, v15, v18, v19,  v0,  v1,  v4,  v5, v28, v29, v20, v21, 2, 3, v21, 2, 3, v22, 0, 1, v22, 2, 3
    dq_butterfly_mixed  v0,  v1,  v4,  v5, v28, v29,  v2,  v3,  v6,  v7, v30, v31, v20, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3
    dq_butterfly_mixed  v2,  v3,  v6,  v7, v30, v31,  v8,  v9, v12, v13, v16, v17, v20, v23, 0, 1, v23, 2, 3, v22, 0, 1, v22, 2, 3
    dq_butterfly_mixed  v8,  v9, v12, v13, v16, v17, v10, v11, v14, v15, v18, v19, v20, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3
    dq_butterfly_bot v10, v11, v14, v15, v18, v19, v20, v23, 0, 1, v23, 2, 3

    sub counter, counter, #1
    cbnz counter, _intt_top_loop

    st1 { v0.4S}, [des0], #16
    st1 { v1.4S}, [des0], #16
    st1 { v2.4S}, [des0], #16
    st1 { v3.4S}, [des0], #16
    st1 { v4.4S}, [des0], #16
    st1 { v5.4S}, [des0], #16
    st1 { v6.4S}, [des0], #16
    st1 { v7.4S}, [des0], #16

    st1 { v8.4S}, [des1], #16
    st1 { v9.4S}, [des1], #16
    st1 {v10.4S}, [des1], #16
    st1 {v11.4S}, [des1], #16
    st1 {v12.4S}, [des1], #16
    st1 {v13.4S}, [des1], #16
    st1 {v14.4S}, [des1], #16
    st1 {v15.4S}, [des1], #16

    .unreq    Q
    .unreq    src0
    .unreq    des0
    .unreq    src1
    .unreq    des1
    .unreq    table
    .unreq    counter
    pop_all

    br lr


.align 2
.global PQCLEAN_LIGHTSABER_AARCH64_asm_intt_SIMD_bot
.global _PQCLEAN_LIGHTSABER_AARCH64_asm_intt_SIMD_bot
#ifndef __clang__
.type PQCLEAN_LIGHTSABER_AARCH64_asm_intt_SIMD_bot, %function
#endif
PQCLEAN_LIGHTSABER_AARCH64_asm_intt_SIMD_bot:
_PQCLEAN_LIGHTSABER_AARCH64_asm_intt_SIMD_bot:

    push_all
    Q         .req w20
    Qhalf     .req w21
    nQhalf    .req w22
    src0      .req  x0
    src1      .req  x1
    src2      .req  x2
    src3      .req  x3
    src4      .req  x4
    src5      .req  x5
    src6      .req  x6
    src7      .req  x7
    table     .req x28
    twistT0   .req  x8
    twistT1   .req  x9
    twistT2   .req x10
    twistT3   .req x11
    twistT4   .req x12
    twistT5   .req x13
    twistT6   .req x14
    twistT7   .req x15
    counter   .req x19

    add twistT0, x3, #256*0
    add twistT1, x3, #256*1
    add twistT2, x3, #256*2
    add twistT3, x3, #256*3
    add twistT4, x3, #256*4
    add twistT5, x3, #256*5
    add twistT6, x3, #256*6
    add twistT7, x3, #256*7

    ldr Q, [x2]
    lsr Qhalf, Q, #1
    neg nQhalf, Qhalf

    add table, x1, #64 

    add src1, src0, #128
    add src2, src0, #256
    add src3, src0, #384
    add src4, src0, #512
    add src5, src0, #640
    add src6, src0, #768
    add src7, src0, #896

    ld1 { v0.4S}, [ src0]
    ld1 { v1.4S}, [ src1]
    ld1 { v2.4S}, [ src2]
    ld1 { v3.4S}, [ src3]
    ld1 { v4.4S}, [ src4]
    ld1 { v5.4S}, [ src5]
    ld1 { v6.4S}, [ src6]
    ld1 { v7.4S}, [ src7]

    ld1 {v20.4S}, [table], #16
    ld1 {v21.4S}, [table], #16
    ld1 {v22.4S}, [table], #16
    ld1 {v23.4S}, [table], #16

    dup v24.4S, Q
    dup v25.4S, Qhalf
    dup v26.4S, nQhalf

    dq_butterfly_top  v4,  v6,  v5,  v7, v18, v19, v24, v20, 2, 3, v20, 2, 3
    dq_butterfly_mixed  v4,  v6,  v5,  v7, v18, v19,  v0,  v2,  v1,  v3, v16, v17, v24, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3
    dq_butterfly_mixed  v0,  v2,  v1,  v3, v16, v17,  v4,  v5,  v6,  v7, v18, v19, v24, v20, 2, 3, v20, 2, 3, v21, 0, 1, v21, 2, 3
    dq_butterfly_mixed  v4,  v5,  v6,  v7, v18, v19,  v0,  v1,  v2,  v3, v16, v17, v24, v21, 0, 1, v21, 2, 3, v21, 0, 1, v21, 2, 3
    dq_butterfly_mixed  v0,  v1,  v2,  v3, v16, v17,  v2,  v3,  v6,  v7, v18, v19, v24, v21, 0, 1, v21, 2, 3, v23, 0, 1, v23, 2, 3
    dq_butterfly_mixed  v2,  v3,  v6,  v7, v18, v19,  v0,  v1,  v4,  v5, v16, v17, v24, v23, 0, 1, v23, 2, 3, v22, 0, 1, v22, 2, 3
    dq_butterfly_bot  v0,  v1,  v4,  v5, v16, v17, v24, v22, 0, 1, v22, 2, 3

    ld2 { v8.4S,  v9.4S}, [twistT0], #32
    ld2 {v10.4S, v11.4S}, [twistT1], #32
    ld2 {v12.4S, v13.4S}, [twistT2], #32
    ld2 {v14.4S, v15.4S}, [twistT3], #32

    sqrdmulh v16.4S,  v0.4S,  v8.4S
    sqrdmulh v17.4S,  v1.4S, v10.4S
    sqrdmulh v18.4S,  v2.4S, v12.4S
    sqrdmulh v19.4S,  v3.4S, v14.4S

    mul       v0.4S,  v0.4S,  v9.4S
    mul       v1.4S,  v1.4S, v11.4S
    mul       v2.4S,  v2.4S, v13.4S
    mul       v3.4S,  v3.4S, v15.4S

    mls       v0.4S, v16.4S, v24.4S
    ld2 { v8.4S,  v9.4S}, [twistT4], #32
    mls       v1.4S, v17.4S, v24.4S
    ld2 {v10.4S, v11.4S}, [twistT5], #32
    mls       v2.4S, v18.4S, v24.4S
    ld2 {v12.4S, v13.4S}, [twistT6], #32
    mls       v3.4S, v19.4S, v24.4S
    ld2 {v14.4S, v15.4S}, [twistT7], #32

    cmge     v18.4S, v26.4S,  v0.4S
    sqrdmulh v20.4S,  v4.4S,  v8.4S
    cmge     v19.4S, v26.4S,  v1.4S
    sqrdmulh v21.4S,  v5.4S, v10.4S
    cmgt     v16.4S,  v0.4S, v25.4S
    sqrdmulh v22.4S,  v6.4S, v12.4S
    cmgt     v17.4S,  v1.4S, v25.4S
    sqrdmulh v23.4S,  v7.4S, v14.4S

    sub      v16.4S, v16.4S, v18.4S
    mul       v4.4S,  v4.4S,  v9.4S
    sub      v17.4S, v17.4S, v19.4S
    mul       v5.4S,  v5.4S, v11.4S

    mla       v0.4S, v16.4S, v24.4S
    mul       v6.4S,  v6.4S, v13.4S
    mla       v1.4S, v17.4S, v24.4S
    mul       v7.4S,  v7.4S, v15.4S

    cmge     v18.4S, v26.4S,  v2.4S
    mls       v4.4S, v20.4S, v24.4S
    cmge     v19.4S, v26.4S,  v3.4S
    mls       v5.4S, v21.4S, v24.4S
    cmgt     v16.4S,  v2.4S, v25.4S
    mls       v6.4S, v22.4S, v24.4S
    cmgt     v17.4S,  v3.4S, v25.4S
    mls       v7.4S, v23.4S, v24.4S

    sub      v16.4S, v16.4S, v18.4S
    cmge     v22.4S, v26.4S,  v4.4S
    sub      v17.4S, v17.4S, v19.4S
    cmge     v23.4S, v26.4S,  v5.4S

    mla       v2.4S, v16.4S, v24.4S
    cmgt     v20.4S,  v4.4S, v25.4S
    mla       v3.4S, v17.4S, v24.4S
    cmgt     v21.4S,  v5.4S, v25.4S

    st1 { v0.4S}, [ src0], #16
    sub      v20.4S, v20.4S, v22.4S
    st1 { v1.4S}, [ src1], #16
    sub      v21.4S, v21.4S, v23.4S
    st1 { v2.4S}, [ src2], #16
    mla       v4.4S, v20.4S, v24.4S
    st1 { v3.4S}, [ src3], #16
    mla       v5.4S, v21.4S, v24.4S

    mov counter, #7
    _intt_bot_loop:

    cmge     v22.4S, v26.4S,  v6.4S
    ld1 { v0.4S}, [ src0]
    cmge     v23.4S, v26.4S,  v7.4S
    ld1 { v1.4S}, [ src1]
    cmgt     v20.4S,  v6.4S, v25.4S
    ld1 { v2.4S}, [ src2]
    cmgt     v21.4S,  v7.4S, v25.4S
    ld1 { v3.4S}, [ src3]

    sub      v20.4S, v20.4S, v22.4S
    sub      v21.4S, v21.4S, v23.4S

    mla       v6.4S, v20.4S, v24.4S
    mla       v7.4S, v21.4S, v24.4S

    st1 { v4.4S}, [ src4], #16
    ld1 { v4.4S}, [ src4]
    st1 { v5.4S}, [ src5], #16
    ld1 { v5.4S}, [ src5]
    st1 { v6.4S}, [ src6], #16
    ld1 { v6.4S}, [ src6]
    st1 { v7.4S}, [ src7], #16
    ld1 { v7.4S}, [ src7]

    ld1 {v20.4S}, [table], #16
    ld1 {v21.4S}, [table], #16
    ld1 {v22.4S}, [table], #16
    ld1 {v23.4S}, [table], #16

    dup v24.4S, Q
    dup v25.4S, Qhalf
    dup v26.4S, nQhalf

    dq_butterfly_top  v4,  v6,  v5,  v7, v18, v19, v24, v20, 2, 3, v20, 2, 3
    dq_butterfly_mixed  v4,  v6,  v5,  v7, v18, v19,  v0,  v2,  v1,  v3, v16, v17, v24, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3
    dq_butterfly_mixed  v0,  v2,  v1,  v3, v16, v17,  v4,  v5,  v6,  v7, v18, v19, v24, v20, 2, 3, v20, 2, 3, v21, 0, 1, v21, 2, 3
    dq_butterfly_mixed  v4,  v5,  v6,  v7, v18, v19,  v0,  v1,  v2,  v3, v16, v17, v24, v21, 0, 1, v21, 2, 3, v21, 0, 1, v21, 2, 3
    dq_butterfly_mixed  v0,  v1,  v2,  v3, v16, v17,  v2,  v3,  v6,  v7, v18, v19, v24, v21, 0, 1, v21, 2, 3, v23, 0, 1, v23, 2, 3
    dq_butterfly_mixed  v2,  v3,  v6,  v7, v18, v19,  v0,  v1,  v4,  v5, v16, v17, v24, v23, 0, 1, v23, 2, 3, v22, 0, 1, v22, 2, 3
    dq_butterfly_bot  v0,  v1,  v4,  v5, v16, v17, v24, v22, 0, 1, v22, 2, 3

    ld2 { v8.4S,  v9.4S}, [twistT0], #32
    ld2 {v10.4S, v11.4S}, [twistT1], #32
    ld2 {v12.4S, v13.4S}, [twistT2], #32
    ld2 {v14.4S, v15.4S}, [twistT3], #32

    sqrdmulh v16.4S,  v0.4S,  v8.4S
    sqrdmulh v17.4S,  v1.4S, v10.4S
    sqrdmulh v18.4S,  v2.4S, v12.4S
    sqrdmulh v19.4S,  v3.4S, v14.4S

    mul       v0.4S,  v0.4S,  v9.4S
    mul       v1.4S,  v1.4S, v11.4S
    mul       v2.4S,  v2.4S, v13.4S
    mul       v3.4S,  v3.4S, v15.4S

    mls       v0.4S, v16.4S, v24.4S
    ld2 { v8.4S,  v9.4S}, [twistT4], #32
    mls       v1.4S, v17.4S, v24.4S
    ld2 {v10.4S, v11.4S}, [twistT5], #32
    mls       v2.4S, v18.4S, v24.4S
    ld2 {v12.4S, v13.4S}, [twistT6], #32
    mls       v3.4S, v19.4S, v24.4S
    ld2 {v14.4S, v15.4S}, [twistT7], #32

    cmge     v18.4S, v26.4S,  v0.4S
    sqrdmulh v20.4S,  v4.4S,  v8.4S
    cmge     v19.4S, v26.4S,  v1.4S
    sqrdmulh v21.4S,  v5.4S, v10.4S
    cmgt     v16.4S,  v0.4S, v25.4S
    sqrdmulh v22.4S,  v6.4S, v12.4S
    cmgt     v17.4S,  v1.4S, v25.4S
    sqrdmulh v23.4S,  v7.4S, v14.4S

    sub      v16.4S, v16.4S, v18.4S
    mul       v4.4S,  v4.4S,  v9.4S
    sub      v17.4S, v17.4S, v19.4S
    mul       v5.4S,  v5.4S, v11.4S

    mla       v0.4S, v16.4S, v24.4S
    mul       v6.4S,  v6.4S, v13.4S
    mla       v1.4S, v17.4S, v24.4S
    mul       v7.4S,  v7.4S, v15.4S

    cmge     v18.4S, v26.4S,  v2.4S
    mls       v4.4S, v20.4S, v24.4S
    cmge     v19.4S, v26.4S,  v3.4S
    mls       v5.4S, v21.4S, v24.4S
    cmgt     v16.4S,  v2.4S, v25.4S
    mls       v6.4S, v22.4S, v24.4S
    cmgt     v17.4S,  v3.4S, v25.4S
    mls       v7.4S, v23.4S, v24.4S

    sub      v16.4S, v16.4S, v18.4S
    cmge     v22.4S, v26.4S,  v4.4S
    sub      v17.4S, v17.4S, v19.4S
    cmge     v23.4S, v26.4S,  v5.4S

    mla       v2.4S, v16.4S, v24.4S
    cmgt     v20.4S,  v4.4S, v25.4S
    mla       v3.4S, v17.4S, v24.4S
    cmgt     v21.4S,  v5.4S, v25.4S

    st1 { v0.4S}, [ src0], #16
    sub      v20.4S, v20.4S, v22.4S
    st1 { v1.4S}, [ src1], #16
    sub      v21.4S, v21.4S, v23.4S
    st1 { v2.4S}, [ src2], #16
    mla       v4.4S, v20.4S, v24.4S
    st1 { v3.4S}, [ src3], #16
    mla       v5.4S, v21.4S, v24.4S

    sub counter, counter, #1
    cbnz counter, _intt_bot_loop

    cmge     v22.4S, v26.4S,  v6.4S
    cmge     v23.4S, v26.4S,  v7.4S
    cmgt     v20.4S,  v6.4S, v25.4S
    cmgt     v21.4S,  v7.4S, v25.4S
    
    sub      v20.4S, v20.4S, v22.4S
    sub      v21.4S, v21.4S, v23.4S

    mla       v6.4S, v20.4S, v24.4S
    mla       v7.4S, v21.4S, v24.4S

    st1 { v4.4S}, [ src4], #16
    st1 { v5.4S}, [ src5], #16
    st1 { v6.4S}, [ src6], #16
    st1 { v7.4S}, [ src7], #16

    .unreq    Q
    .unreq    Qhalf
    .unreq    nQhalf
    .unreq    src0
    .unreq    src1
    .unreq    src2
    .unreq    src3
    .unreq    src4
    .unreq    src5
    .unreq    src6
    .unreq    src7
    .unreq    table
    .unreq    twistT0
    .unreq    twistT1
    .unreq    twistT2
    .unreq    twistT3
    .unreq    twistT4
    .unreq    twistT5
    .unreq    twistT6
    .unreq    twistT7
    .unreq    counter
    pop_all

    br lr













