
#include "macros.inc"

.align 2
.global PQCLEAN_LIGHTSABER_AARCH64_asm_ntt_SIMD_top
.global _PQCLEAN_LIGHTSABER_AARCH64_asm_ntt_SIMD_top
#ifndef __clang__
.type PQCLEAN_LIGHTSABER_AARCH64_asm_ntt_SIMD_top, %function
#endif
PQCLEAN_LIGHTSABER_AARCH64_asm_ntt_SIMD_top:
_PQCLEAN_LIGHTSABER_AARCH64_asm_ntt_SIMD_top:

    push_all
    Q         .req w20
    src0      .req  x0
    src1      .req  x1
    src2      .req  x2
    src3      .req  x3
    src4      .req  x4
    src5      .req  x5
    src6      .req  x6
    src7      .req  x7
    src8      .req  x8
    src9      .req  x9
    src10     .req x10
    src11     .req x11
    src12     .req x12
    src13     .req x13
    src14     .req x14
    src15     .req x15
    table     .req x28
    counter   .req x19

    ldr Q, [x2]

    mov table, x1

    add  src1, src0, #64
    add  src2, src0, #128
   
    add  src3, src0, #192
    add  src4, src0, #256

    add  src5, src0, #320
    add  src6, src0, #384

    add  src7, src0, #448
    add  src8, src0, #512

    add  src9, src0, #576
    add src10, src0, #640

    add src11, src0, #704
    add src12, src0, #768

    add src13, src0, #832
    add src14, src0, #896

    add src15, src0, #960

    ld1 {v20.4S, v21.4S, v22.4S, v23.4S}, [table], #64

    mov v20.S[0], Q

    ld1 { v0.4S}, [ src0]
    ld1 { v2.4S}, [ src2]
    ld1 { v4.4S}, [ src4]
    ld1 { v6.4S}, [ src6]
    ld1 { v8.4S}, [ src8]
    ld1 {v10.4S}, [src10]
    ld1 {v12.4S}, [src12]
    ld1 {v14.4S}, [src14]

    ld1 { v1.4S}, [ src1]
    ld1 { v3.4S}, [ src3]
    ld1 { v5.4S}, [ src5]
    ld1 { v7.4S}, [ src7]
    ld1 { v9.4S}, [ src9]
    ld1 {v11.4S}, [src11]
    ld1 {v13.4S}, [src13]
    ld1 {v15.4S}, [src15]

    qq_butterfly_top  v0,  v2,  v4,  v6,  v8, v10, v12, v14, v16, v17, v18, v19, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3
    qq_butterfly_mixed  v0,  v2,  v4,  v6,  v8, v10, v12, v14, v16, v17, v18, v19,  v1,  v3,  v5,  v7,  v9, v11, v13, v15, v28, v29, v30, v31, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3
    qq_butterfly_mixed  v1,  v3,  v5,  v7,  v9, v11, v13, v15, v28, v29, v30, v31,  v0,  v2,  v8, v10,  v4,  v6, v12, v14, v16, v17, v18, v19, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3
    qq_butterfly_mixed  v0,  v2,  v8, v10,  v4,  v6, v12, v14, v16, v17, v18, v19,  v1,  v3,  v9, v11,  v5,  v7, v13, v15, v28, v29, v30, v31, v20, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3
    qq_butterfly_mixed  v1,  v3,  v9, v11,  v5,  v7, v13, v15, v28, v29, v30, v31,  v0,  v4,  v8, v12,  v2,  v6, v10, v14, v16, v17, v18, v19, v20, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3
    qq_butterfly_mixed  v0,  v4,  v8, v12,  v2,  v6, v10, v14, v16, v17, v18, v19,  v1,  v5,  v9, v13,  v3,  v7, v11, v15, v28, v29, v30, v31, v20, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3
    qq_butterfly_bot  v1,  v5,  v9, v13,  v3,  v7, v11, v15, v28, v29, v30, v31, v20, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3

    mov counter, #3
    _ntt_top_loop:

    st1 { v0.4S}, [ src0], #16
    ld1 { v0.4S}, [ src0]
    st1 { v2.4S}, [ src2], #16
    ld1 { v2.4S}, [ src2]
    st1 { v4.4S}, [ src4], #16
    ld1 { v4.4S}, [ src4]
    st1 { v6.4S}, [ src6], #16
    ld1 { v6.4S}, [ src6]
    st1 { v8.4S}, [ src8], #16
    ld1 { v8.4S}, [ src8]
    st1 {v10.4S}, [src10], #16
    ld1 {v10.4S}, [src10]
    st1 {v12.4S}, [src12], #16
    ld1 {v12.4S}, [src12]
    st1 {v14.4S}, [src14], #16
    ld1 {v14.4S}, [src14]

    st1 { v1.4S}, [ src1], #16
    ld1 { v1.4S}, [ src1]
    st1 { v3.4S}, [ src3], #16
    ld1 { v3.4S}, [ src3]
    st1 { v5.4S}, [ src5], #16
    ld1 { v5.4S}, [ src5]
    st1 { v7.4S}, [ src7], #16
    ld1 { v7.4S}, [ src7]
    st1 { v9.4S}, [ src9], #16
    ld1 { v9.4S}, [ src9]
    st1 {v11.4S}, [src11], #16
    ld1 {v11.4S}, [src11]
    st1 {v13.4S}, [src13], #16
    ld1 {v13.4S}, [src13]
    st1 {v15.4S}, [src15], #16
    ld1 {v15.4S}, [src15]

    qq_butterfly_top  v0,  v2,  v4,  v6,  v8, v10, v12, v14, v16, v17, v18, v19, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3
    qq_butterfly_mixed  v0,  v2,  v4,  v6,  v8, v10, v12, v14, v16, v17, v18, v19,  v1,  v3,  v5,  v7,  v9, v11, v13, v15, v28, v29, v30, v31, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3
    qq_butterfly_mixed  v1,  v3,  v5,  v7,  v9, v11, v13, v15, v28, v29, v30, v31,  v0,  v2,  v8, v10,  v4,  v6, v12, v14, v16, v17, v18, v19, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3
    qq_butterfly_mixed  v0,  v2,  v8, v10,  v4,  v6, v12, v14, v16, v17, v18, v19,  v1,  v3,  v9, v11,  v5,  v7, v13, v15, v28, v29, v30, v31, v20, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3
    qq_butterfly_mixed  v1,  v3,  v9, v11,  v5,  v7, v13, v15, v28, v29, v30, v31,  v0,  v4,  v8, v12,  v2,  v6, v10, v14, v16, v17, v18, v19, v20, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3
    qq_butterfly_mixed  v0,  v4,  v8, v12,  v2,  v6, v10, v14, v16, v17, v18, v19,  v1,  v5,  v9, v13,  v3,  v7, v11, v15, v28, v29, v30, v31, v20, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3
    qq_butterfly_bot  v1,  v5,  v9, v13,  v3,  v7, v11, v15, v28, v29, v30, v31, v20, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3

    sub counter, counter, #1
    cbnz counter, _ntt_top_loop

    st1 { v0.4S}, [ src0], #16
    st1 { v2.4S}, [ src2], #16
    st1 { v4.4S}, [ src4], #16
    st1 { v6.4S}, [ src6], #16
    st1 { v8.4S}, [ src8], #16
    st1 {v10.4S}, [src10], #16
    st1 {v12.4S}, [src12], #16
    st1 {v14.4S}, [src14], #16

    st1 { v1.4S}, [ src1], #16
    st1 { v3.4S}, [ src3], #16
    st1 { v5.4S}, [ src5], #16
    st1 { v7.4S}, [ src7], #16
    st1 { v9.4S}, [ src9], #16
    st1 {v11.4S}, [src11], #16
    st1 {v13.4S}, [src13], #16
    st1 {v15.4S}, [src15], #16

    .unreq    Q
    .unreq    src0
    .unreq    src1
    .unreq    src2
    .unreq    src3
    .unreq    src4
    .unreq    src5
    .unreq    src6
    .unreq    src7
    .unreq    src8
    .unreq    src9
    .unreq    src10
    .unreq    src11
    .unreq    src12
    .unreq    src13
    .unreq    src14
    .unreq    src15
    .unreq    table
    .unreq    counter
    pop_all

    br lr


.align 2
.global PQCLEAN_LIGHTSABER_AARCH64_asm_ntt_SIMD_bot
.global _PQCLEAN_LIGHTSABER_AARCH64_asm_ntt_SIMD_bot
#ifndef __clang__
.type PQCLEAN_LIGHTSABER_AARCH64_asm_ntt_SIMD_bot, %function
#endif
PQCLEAN_LIGHTSABER_AARCH64_asm_ntt_SIMD_bot:
_PQCLEAN_LIGHTSABER_AARCH64_asm_ntt_SIMD_bot:

    push_all
    Q         .req w20 
    src0      .req x0
    src1      .req x1
    src2      .req x2
    src3      .req x3
    table0    .req x27
    table1    .req x28
    counter   .req x19

    ldr Q, [x2]

    add table0, x1, #64
    add table1, x1, #320

    add src1, src0, #0
    add src2, src0, #512
    add src3, src0, #512

    ld1 { v0.4S,  v1.4S,  v2.4S,  v3.4S}, [src0], #64
    ld1 { v8.4S,  v9.4S, v10.4S, v11.4S}, [src2], #64
    ld1 { v4.4S,  v5.4S,  v6.4S,  v7.4S}, [src0], #64
    ld1 {v12.4S, v13.4S, v14.4S, v15.4S}, [src2], #64

    ld1 {v20.4S, v21.4S, v22.4S, v23.4S}, [table0], #64
    ld1 {v24.4S, v25.4S, v26.4S, v27.4S}, [table1], #64

    mov v20.S[0], Q

    qq_butterfly_top  v0,  v1,  v2,  v3,  v4,  v5,  v6,  v7, v16, v17, v18, v19, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3
    qq_butterfly_mixed  v0,  v1,  v2,  v3,  v4,  v5,  v6,  v7, v16, v17, v18, v19,  v8,  v9, v10, v11, v12, v13, v14, v15, v28, v29, v30, v31, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v24, 2, 3, v24, 2, 3, v24, 2, 3, v24, 2, 3
    qq_butterfly_mixed  v8,  v9, v10, v11, v12, v13, v14, v15, v28, v29, v30, v31,  v0,  v1,  v4,  v5,  v2,  v3,  v6,  v7, v16, v17, v18, v19, v20, v24, 2, 3, v24, 2, 3, v24, 2, 3, v24, 2, 3, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3
    qq_butterfly_mixed  v0,  v1,  v4,  v5,  v2,  v3,  v6,  v7, v16, v17, v18, v19,  v8,  v9, v12, v13, v10, v11, v14, v15, v28, v29, v30, v31, v20, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3, v25, 0, 1, v25, 0, 1, v25, 2, 3, v25, 2, 3
    qq_butterfly_mixed  v8,  v9, v12, v13, v10, v11, v14, v15, v28, v29, v30, v31,  v0,  v2,  v4,  v6,  v1,  v3,  v5,  v7, v16, v17, v18, v19, v20, v25, 0, 1, v25, 0, 1, v25, 2, 3, v25, 2, 3, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3
    qq_butterfly_mixed  v0,  v2,  v4,  v6,  v1,  v3,  v5,  v7, v16, v17, v18, v19,  v8, v10, v12, v14,  v9, v11, v13, v15, v28, v29, v30, v31, v20, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3, v26, 0, 1, v26, 2, 3, v27, 0, 1, v27, 2, 3
    qq_butterfly_bot  v8, v10, v12, v14,  v9, v11, v13, v15, v28, v29, v30, v31, v20, v26, 0, 1, v26, 2, 3, v27, 0, 1, v27, 2, 3

    mov counter, #3
    _ntt_bot_loop:

    st1 { v0.4S}, [src1], #16
    ld1 { v0.4S}, [src0], #16
    st1 { v1.4S}, [src1], #16
    ld1 { v1.4S}, [src0], #16
    st1 { v2.4S}, [src1], #16
    ld1 { v2.4S}, [src0], #16
    st1 { v3.4S}, [src1], #16
    ld1 { v3.4S}, [src0], #16
    st1 { v4.4S}, [src1], #16
    ld1 { v4.4S}, [src0], #16
    st1 { v5.4S}, [src1], #16
    ld1 { v5.4S}, [src0], #16
    st1 { v6.4S}, [src1], #16
    ld1 { v6.4S}, [src0], #16
    st1 { v7.4S}, [src1], #16
    ld1 { v7.4S}, [src0], #16
    st1 { v8.4S}, [src3], #16
    ld1 { v8.4S}, [src2], #16
    st1 { v9.4S}, [src3], #16
    ld1 { v9.4S}, [src2], #16
    st1 {v10.4S}, [src3], #16
    ld1 {v10.4S}, [src2], #16
    st1 {v11.4S}, [src3], #16
    ld1 {v11.4S}, [src2], #16
    st1 {v12.4S}, [src3], #16
    ld1 {v12.4S}, [src2], #16
    st1 {v13.4S}, [src3], #16
    ld1 {v13.4S}, [src2], #16
    st1 {v14.4S}, [src3], #16
    ld1 {v14.4S}, [src2], #16
    st1 {v15.4S}, [src3], #16
    ld1 {v15.4S}, [src2], #16

    ld1 {v20.4S, v21.4S, v22.4S, v23.4S}, [table0], #64
    ld1 {v24.4S, v25.4S, v26.4S, v27.4S}, [table1], #64

    mov v20.S[0], Q

    qq_butterfly_top  v0,  v1,  v2,  v3,  v4,  v5,  v6,  v7, v16, v17, v18, v19, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3
    qq_butterfly_mixed  v0,  v1,  v2,  v3,  v4,  v5,  v6,  v7, v16, v17, v18, v19,  v8,  v9, v10, v11, v12, v13, v14, v15, v28, v29, v30, v31, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v24, 2, 3, v24, 2, 3, v24, 2, 3, v24, 2, 3
    qq_butterfly_mixed  v8,  v9, v10, v11, v12, v13, v14, v15, v28, v29, v30, v31,  v0,  v1,  v4,  v5,  v2,  v3,  v6,  v7, v16, v17, v18, v19, v20, v24, 2, 3, v24, 2, 3, v24, 2, 3, v24, 2, 3, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3
    qq_butterfly_mixed  v0,  v1,  v4,  v5,  v2,  v3,  v6,  v7, v16, v17, v18, v19,  v8,  v9, v12, v13, v10, v11, v14, v15, v28, v29, v30, v31, v20, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3, v25, 0, 1, v25, 0, 1, v25, 2, 3, v25, 2, 3
    qq_butterfly_mixed  v8,  v9, v12, v13, v10, v11, v14, v15, v28, v29, v30, v31,  v0,  v2,  v4,  v6,  v1,  v3,  v5,  v7, v16, v17, v18, v19, v20, v25, 0, 1, v25, 0, 1, v25, 2, 3, v25, 2, 3, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3
    qq_butterfly_mixed  v0,  v2,  v4,  v6,  v1,  v3,  v5,  v7, v16, v17, v18, v19,  v8, v10, v12, v14,  v9, v11, v13, v15, v28, v29, v30, v31, v20, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3, v26, 0, 1, v26, 2, 3, v27, 0, 1, v27, 2, 3
    qq_butterfly_bot  v8, v10, v12, v14,  v9, v11, v13, v15, v28, v29, v30, v31, v20, v26, 0, 1, v26, 2, 3, v27, 0, 1, v27, 2, 3

    sub counter, counter, #1
    cbnz counter, _ntt_bot_loop

    st1 { v0.4S,  v1.4S,  v2.4S,  v3.4S}, [src1], #64
    st1 { v8.4S,  v9.4S, v10.4S, v11.4S}, [src3], #64
    st1 { v4.4S,  v5.4S,  v6.4S,  v7.4S}, [src1], #64
    st1 {v12.4S, v13.4S, v14.4S, v15.4S}, [src3], #64

    .unreq    Q
    .unreq    src0
    .unreq    src1
    .unreq    src2
    .unreq    src3
    .unreq    table0
    .unreq    table1
    pop_all

    br lr
















