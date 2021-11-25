
#include "macros.inc"
#include "SABER_params.h"

.align 2
.global PQCLEAN_LIGHTSABER_AARCH64_asm_asymmetric_mul
.global _PQCLEAN_LIGHTSABER_AARCH64_asm_asymmetric_mul
#ifndef __clang__
.type PQCLEAN_LIGHTSABER_AARCH64_asm_asymmetric_mul, %function
#endif
PQCLEAN_LIGHTSABER_AARCH64_asm_asymmetric_mul:
_PQCLEAN_LIGHTSABER_AARCH64_asm_asymmetric_mul:

    push_all

    ldr w28, [x3, #0]
    ldr w27, [x3, #4]

    dup v28.4S, w28
    dup v29.4S, w27

    add x11,  x0, #0

    add  x4,  x0, #1024
    add  x5,  x1, #1024
    add  x6,  x2, #1024

.if SABER_L > 2
    add  x8,  x0, #2048
    add  x9,  x1, #2048
    add x10,  x2, #2048
.endif

.if SABER_L > 3
    add x12,  x0, #3072
    add x13,  x1, #3072
    add x14,  x2, #3072
.endif

    mov x16, #16
    _asymmetric_loop:

    ld4 { v0.4S,  v1.4S,  v2.4S,  v3.4S}, [ x0], #64
    ld4 { v4.4S,  v5.4S,  v6.4S,  v7.4S}, [ x1], #64
    ld4 { v8.4S,  v9.4S, v10.4S, v11.4S}, [ x2], #64

    _4x4_asymmetric smull, smull2,  v3,  v9, v10, v11,  v4, v16, v20, v17, v21, v18, v22, v19, v23
    _4x4_asymmetric smlal, smlal2,  v2, v10, v11,  v4,  v5, v16, v20, v17, v21, v18, v22, v19, v23
    _4x4_asymmetric smlal, smlal2,  v1, v11,  v4,  v5,  v6, v16, v20, v17, v21, v18, v22, v19, v23
    _4x4_asymmetric smlal, smlal2,  v0,  v4,  v5,  v6,  v7, v16, v20, v17, v21, v18, v22, v19, v23

    ld4 { v0.4S,  v1.4S,  v2.4S,  v3.4S}, [ x4], #64
    ld4 { v4.4S,  v5.4S,  v6.4S,  v7.4S}, [ x5], #64
    ld4 { v8.4S,  v9.4S, v10.4S, v11.4S}, [ x6], #64

    _4x4_asymmetric smlal, smlal2,  v3,  v9, v10, v11,  v4, v16, v20, v17, v21, v18, v22, v19, v23
    _4x4_asymmetric smlal, smlal2,  v2, v10, v11,  v4,  v5, v16, v20, v17, v21, v18, v22, v19, v23
    _4x4_asymmetric smlal, smlal2,  v1, v11,  v4,  v5,  v6, v16, v20, v17, v21, v18, v22, v19, v23
    _4x4_asymmetric smlal, smlal2,  v0,  v4,  v5,  v6,  v7, v16, v20, v17, v21, v18, v22, v19, v23

.if SABER_L > 2
    ld4 { v0.4S,  v1.4S,  v2.4S,  v3.4S}, [ x8], #64
    ld4 { v4.4S,  v5.4S,  v6.4S,  v7.4S}, [ x9], #64
    ld4 { v8.4S,  v9.4S, v10.4S, v11.4S}, [x10], #64

    _4x4_asymmetric smlal, smlal2,  v3,  v9, v10, v11,  v4, v16, v20, v17, v21, v18, v22, v19, v23
    _4x4_asymmetric smlal, smlal2,  v2, v10, v11,  v4,  v5, v16, v20, v17, v21, v18, v22, v19, v23
    _4x4_asymmetric smlal, smlal2,  v1, v11,  v4,  v5,  v6, v16, v20, v17, v21, v18, v22, v19, v23
    _4x4_asymmetric smlal, smlal2,  v0,  v4,  v5,  v6,  v7, v16, v20, v17, v21, v18, v22, v19, v23
.endif

.if SABER_L > 3
    ld4 { v0.4S,  v1.4S,  v2.4S,  v3.4S}, [x12], #64
    ld4 { v4.4S,  v5.4S,  v6.4S,  v7.4S}, [x13], #64
    ld4 { v8.4S,  v9.4S, v10.4S, v11.4S}, [x14], #64

    _4x4_asymmetric smlal, smlal2,  v3,  v9, v10, v11,  v4, v16, v20, v17, v21, v18, v22, v19, v23
    _4x4_asymmetric smlal, smlal2,  v2, v10, v11,  v4,  v5, v16, v20, v17, v21, v18, v22, v19, v23
    _4x4_asymmetric smlal, smlal2,  v1, v11,  v4,  v5,  v6, v16, v20, v17, v21, v18, v22, v19, v23
    _4x4_asymmetric smlal, smlal2,  v0,  v4,  v5,  v6,  v7, v16, v20, v17, v21, v18, v22, v19, v23
.endif

    qq_montgomery v24, v25, v26, v27, v16, v17, v18, v19, v20, v21, v22, v23, v0, v1, v2, v3, v29, v28

    st4 {v24.4S, v25.4S, v26.4S, v27.4S}, [x11], #64

    sub x16, x16, #1
    cbnz x16, _asymmetric_loop

    pop_all

    br lr

.align 2
.global PQCLEAN_LIGHTSABER_AARCH64_asm_point_mul_extended
.global _PQCLEAN_LIGHTSABER_AARCH64_asm_point_mul_extended
#ifndef __clang__
.type PQCLEAN_LIGHTSABER_AARCH64_asm_point_mul_extended, %function
#endif
PQCLEAN_LIGHTSABER_AARCH64_asm_point_mul_extended:
_PQCLEAN_LIGHTSABER_AARCH64_asm_point_mul_extended:

    push_all

    ldr w20, [x3]

    ld1 { v0.4S}, [x1], #16
    ld1 { v1.4S}, [x1], #16
    ld1 { v2.4S}, [x1], #16
    ld1 { v3.4S}, [x1], #16

    ld2 { v4.4S,  v5.4S}, [x2], #32
    ld2 { v6.4S,  v7.4S}, [x2], #32
    ld2 { v8.4S,  v9.4S}, [x2], #32
    ld2 {v10.4S, v11.4S}, [x2], #32

    sqrdmulh v12.4S,  v0.4S,  v4.4S
    sqrdmulh v13.4S,  v1.4S,  v6.4S
    sqrdmulh v14.4S,  v2.4S,  v8.4S
    sqrdmulh v15.4S,  v3.4S, v10.4S

    mov x16, #7
    _point_mul_loop:

    dup v4.4S, w20

    mul       v0.4S,  v0.4S,  v5.4S
    ld1 {v16.4S}, [x1], #16
    mul       v1.4S,  v1.4S,  v7.4S
    ld1 {v17.4S}, [x1], #16
    mul       v2.4S,  v2.4S,  v9.4S
    ld1 {v18.4S}, [x1], #16
    mul       v3.4S,  v3.4S, v11.4S
    ld1 {v19.4S}, [x1], #16

    mls       v0.4S, v12.4S,  v4.4S
    ld2 {v20.4S, v21.4S}, [x2], #32
    mls       v1.4S, v13.4S,  v4.4S
    ld2 {v22.4S, v23.4S}, [x2], #32
    mls       v2.4S, v14.4S,  v4.4S
    ld2 {v24.4S, v25.4S}, [x2], #32
    mls       v3.4S, v15.4S,  v4.4S
    ld2 {v26.4S, v27.4S}, [x2], #32

    st1 { v0.4S}, [x0], #16
    sqrdmulh v28.4S, v16.4S, v20.4S
    st1 { v1.4S}, [x0], #16
    sqrdmulh v29.4S, v17.4S, v22.4S
    st1 { v2.4S}, [x0], #16
    sqrdmulh v30.4S, v18.4S, v24.4S
    st1 { v3.4S}, [x0], #16
    sqrdmulh v31.4S, v19.4S, v26.4S

    dup v20.4S, w20

    mul      v16.4S, v16.4S, v21.4S
    ld1 { v0.4S}, [x1], #16
    mul      v17.4S, v17.4S, v23.4S
    ld1 { v1.4S}, [x1], #16
    mul      v18.4S, v18.4S, v25.4S
    ld1 { v2.4S}, [x1], #16
    mul      v19.4S, v19.4S, v27.4S
    ld1 { v3.4S}, [x1], #16

    mls      v16.4S, v28.4S, v20.4S
    ld2 { v4.4S,  v5.4S}, [x2], #32
    mls      v17.4S, v29.4S, v20.4S
    ld2 { v6.4S,  v7.4S}, [x2], #32
    mls      v18.4S, v30.4S, v20.4S
    ld2 { v8.4S,  v9.4S}, [x2], #32
    mls      v19.4S, v31.4S, v20.4S
    ld2 {v10.4S, v11.4S}, [x2], #32

    st1 {v16.4S}, [x0], #16
    sqrdmulh v12.4S,  v0.4S,  v4.4S
    st1 {v17.4S}, [x0], #16
    sqrdmulh v13.4S,  v1.4S,  v6.4S
    st1 {v18.4S}, [x0], #16
    sqrdmulh v14.4S,  v2.4S,  v8.4S
    st1 {v19.4S}, [x0], #16
    sqrdmulh v15.4S,  v3.4S, v10.4S

    sub x16, x16, #1
    cbnz x16, _point_mul_loop

    dup v4.4S, w20

    mul       v0.4S,  v0.4S,  v5.4S
    ld1 {v16.4S}, [x1], #16
    mul       v1.4S,  v1.4S,  v7.4S
    ld1 {v17.4S}, [x1], #16
    mul       v2.4S,  v2.4S,  v9.4S
    ld1 {v18.4S}, [x1], #16
    mul       v3.4S,  v3.4S, v11.4S
    ld1 {v19.4S}, [x1], #16

    mls       v0.4S, v12.4S,  v4.4S
    ld2 {v20.4S, v21.4S}, [x2], #32
    mls       v1.4S, v13.4S,  v4.4S
    ld2 {v22.4S, v23.4S}, [x2], #32
    mls       v2.4S, v14.4S,  v4.4S
    ld2 {v24.4S, v25.4S}, [x2], #32
    mls       v3.4S, v15.4S,  v4.4S
    ld2 {v26.4S, v27.4S}, [x2], #32

    st1 { v0.4S}, [x0], #16
    sqrdmulh v28.4S, v16.4S, v20.4S
    st1 { v1.4S}, [x0], #16
    sqrdmulh v29.4S, v17.4S, v22.4S
    st1 { v2.4S}, [x0], #16
    sqrdmulh v30.4S, v18.4S, v24.4S
    st1 { v3.4S}, [x0], #16
    sqrdmulh v31.4S, v19.4S, v26.4S

    dup v20.4S, w20

    mul      v16.4S, v16.4S, v21.4S
    mul      v17.4S, v17.4S, v23.4S
    mul      v18.4S, v18.4S, v25.4S
    mul      v19.4S, v19.4S, v27.4S

    mls      v16.4S, v28.4S, v20.4S
    mls      v17.4S, v29.4S, v20.4S
    mls      v18.4S, v30.4S, v20.4S
    mls      v19.4S, v31.4S, v20.4S

    st1 {v16.4S}, [x0], #16
    st1 {v17.4S}, [x0], #16
    st1 {v18.4S}, [x0], #16
    st1 {v19.4S}, [x0], #16

    pop_all

    br lr





















