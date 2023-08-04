
/*
 * CC0 1.0 Universal or the following MIT License
 *
 * MIT License
 *
 * Copyright (c) 2023: Hanno Becker, Vincent Hwang, Matthias J. Kannwischer, Bo-Yin Yang, and Shang-Yi Yang
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "macros.inc"

#include "params.h"

.align 2
.global PQCLEAN_KYBER768_AARCH64__asm_point_mul_extended
.global _PQCLEAN_KYBER768_AARCH64__asm_point_mul_extended
PQCLEAN_KYBER768_AARCH64__asm_point_mul_extended:
_PQCLEAN_KYBER768_AARCH64__asm_point_mul_extended:

    push_all
    Q         .req w20
    des       .req x0
    src1      .req x1
    src2ex    .req x2
    counter   .req x19

    ldrsh Q, [x3]

    dup v20.8H, Q

    // TODO: unroll this, currently we are using only 16 SIMD registers
    mov counter, #4
    _point_mul_extended_loop:

    ld2 { v0.8H,  v1.8H}, [src1], #32
    ld2 { v2.8H,  v3.8H}, [src1], #32
    ld2 { v4.8H,  v5.8H}, [src1], #32
    ld2 { v6.8H,  v7.8H}, [src1], #32

    ld2 { v8.8H,  v9.8H}, [src2ex], #32
    ld2 {v10.8H, v11.8H}, [src2ex], #32
    ld2 {v12.8H, v13.8H}, [src2ex], #32
    ld2 {v14.8H, v15.8H}, [src2ex], #32

    sqrdmulh  v0.8H,  v1.8H,  v8.8H
    sqrdmulh  v2.8H,  v3.8H, v10.8H
    sqrdmulh  v4.8H,  v5.8H, v12.8H
    sqrdmulh  v6.8H,  v7.8H, v14.8H

    mul       v1.8H,  v1.8H,  v9.8H
    mul       v3.8H,  v3.8H, v11.8H
    mul       v5.8H,  v5.8H, v13.8H
    mul       v7.8H,  v7.8H, v15.8H

    mls       v1.8H,  v0.8H, v20.8H
    mls       v3.8H,  v2.8H, v20.8H
    mls       v5.8H,  v4.8H, v20.8H
    mls       v7.8H,  v6.8H, v20.8H

    st1 { v1.8H}, [des], #16
    st1 { v3.8H}, [des], #16
    st1 { v5.8H}, [des], #16
    st1 { v7.8H}, [des], #16

    sub counter, counter, #1
    cbnz counter, _point_mul_extended_loop

    .unreq    Q
    .unreq    des
    .unreq    src1
    .unreq    src2ex
    .unreq    counter
    pop_all

    br lr


.align 2
.global PQCLEAN_KYBER768_AARCH64__asm_asymmetric_mul
.global _PQCLEAN_KYBER768_AARCH64__asm_asymmetric_mul
PQCLEAN_KYBER768_AARCH64__asm_asymmetric_mul:
_PQCLEAN_KYBER768_AARCH64__asm_asymmetric_mul:

    push_all
    Q         .req w28
    Qprime2   .req w27
    des       .req x11
    src1_0    .req x0
    src2_0    .req x1
    src2asy_0 .req x2
    src1_1    .req x4
    src2_1    .req x5
    src2asy_1 .req x6
    src1_2    .req x8
    src2_2    .req x9
    src2asy_2 .req x10
    src1_3    .req x12
    src2_3    .req x13
    src2asy_3 .req x14
    counter   .req x19

    ldrsh Q, [x3, #0]
    ldrsh Qprime2, [x3, #2]

    add des, x4, #0

    add     src1_1,    src1_0, #512*1
    add     src2_1,    src2_0, #512*1
    add  src2asy_1, src2asy_0, #256*1

#if KYBER_K > 2
    add     src1_2,    src1_0, #512*2
    add     src2_2,    src2_0, #512*2
    add  src2asy_2, src2asy_0, #256*2
#endif

#if KYBER_K > 3
    add     src1_3,    src1_0, #512*3
    add     src2_3,    src2_0, #512*3
    add  src2asy_3, src2asy_0, #256*3
#endif

    dup v28.8H, Q
    dup v29.8H, Qprime2

    // TODO:interleaving
    mov counter, #16
    _asymmetric_mul_loop:

    ld2 { v0.8H,  v1.8H}, [   src1_0], #32
    ld2 { v2.8H,  v3.8H}, [   src2_0], #32
    ld1 { v5.8H}, [src2asy_0], #16

    smull  v16.4S,  v0.4H,  v2.4H
    smull2 v20.4S,  v0.8H,  v2.8H
    smull  v17.4S,  v0.4H,  v3.4H
    smull2 v21.4S,  v0.8H,  v3.8H

    smlal  v16.4S,  v1.4H,  v5.4H
    smlal2 v20.4S,  v1.8H,  v5.8H
    smlal  v17.4S,  v1.4H,  v2.4H
    smlal2 v21.4S,  v1.8H,  v2.8H

    ld2 { v0.8H,  v1.8H}, [   src1_1], #32
    ld2 { v2.8H,  v3.8H}, [   src2_1], #32
    ld1 { v5.8H}, [src2asy_1], #16

    smlal  v16.4S,  v0.4H,  v2.4H
    smlal2 v20.4S,  v0.8H,  v2.8H
    smlal  v17.4S,  v0.4H,  v3.4H
    smlal2 v21.4S,  v0.8H,  v3.8H

    smlal  v16.4S,  v1.4H,  v5.4H
    smlal2 v20.4S,  v1.8H,  v5.8H
    smlal  v17.4S,  v1.4H,  v2.4H
    smlal2 v21.4S,  v1.8H,  v2.8H

#if KYBER_K > 2
    ld2 { v0.8H,  v1.8H}, [   src1_2], #32
    ld2 { v2.8H,  v3.8H}, [   src2_2], #32
    ld1 { v5.8H}, [src2asy_2], #16

    smlal  v16.4S,  v0.4H,  v2.4H
    smlal2 v20.4S,  v0.8H,  v2.8H
    smlal  v17.4S,  v0.4H,  v3.4H
    smlal2 v21.4S,  v0.8H,  v3.8H

    smlal  v16.4S,  v1.4H,  v5.4H
    smlal2 v20.4S,  v1.8H,  v5.8H
    smlal  v17.4S,  v1.4H,  v2.4H
    smlal2 v21.4S,  v1.8H,  v2.8H
#endif

#if KYBER_K > 3
    ld2 { v0.8H,  v1.8H}, [   src1_3], #32
    ld2 { v2.8H,  v3.8H}, [   src2_3], #32
    ld1 { v5.8H}, [src2asy_3], #16

    smlal  v16.4S,  v0.4H,  v2.4H
    smlal2 v20.4S,  v0.8H,  v2.8H
    smlal  v17.4S,  v0.4H,  v3.4H
    smlal2 v21.4S,  v0.8H,  v3.8H

    smlal  v16.4S,  v1.4H,  v5.4H
    smlal2 v20.4S,  v1.8H,  v5.8H
    smlal  v17.4S,  v1.4H,  v2.4H
    smlal2 v21.4S,  v1.8H,  v2.8H
#endif

    uzp1    v0.8H, v16.8H, v20.8H
    uzp1    v1.8H, v17.8H, v21.8H

    mul     v0.8H,  v0.8H, v29.8H
    mul     v1.8H,  v1.8H, v29.8H

    smlal  v16.4S,  v0.4H, v28.4H
    smlal2 v20.4S,  v0.8H, v28.8H
    smlal  v17.4S,  v1.4H, v28.4H
    smlal2 v21.4S,  v1.8H, v28.8H

    uzp2   v24.8H, v16.8H, v20.8H
    uzp2   v25.8H, v17.8H, v21.8H

    st2 {v24.8H, v25.8H}, [des], #32

    sub counter, counter, #1
    cbnz counter, _asymmetric_mul_loop

    .unreq    Q
    .unreq    Qprime2
    .unreq    des
    .unreq    src1_0
    .unreq    src2_0
    .unreq    src2asy_0
    .unreq    src1_1
    .unreq    src2_1
    .unreq    src2asy_1
    .unreq    src1_2
    .unreq    src2_2
    .unreq    src2asy_2
    .unreq    src1_3
    .unreq    src2_3
    .unreq    src2asy_3
    .unreq    counter
    pop_all

    br lr


.align 2
.global PQCLEAN_KYBER768_AARCH64__asm_asymmetric_mul_montgomery
.global _PQCLEAN_KYBER768_AARCH64__asm_asymmetric_mul_montgomery
PQCLEAN_KYBER768_AARCH64__asm_asymmetric_mul_montgomery:
_PQCLEAN_KYBER768_AARCH64__asm_asymmetric_mul_montgomery:

    push_all
    Q         .req w28
    Qprime2   .req w27
    R3        .req w26
    R3p       .req w25
    des       .req x11
    src1_0    .req x0
    src2_0    .req x1
    src2asy_0 .req x2
    src1_1    .req x4
    src2_1    .req x5
    src2asy_1 .req x6
    src1_2    .req x8
    src2_2    .req x9
    src2asy_2 .req x10
    src1_3    .req x12
    src2_3    .req x13
    src2asy_3 .req x14
    counter   .req x19

    ldrsh Q, [x3, #0]
    ldrsh Qprime2, [x3, #2]

    ldrsh R3, [x3, #8]
    ldrsh R3p, [x3, #10]

    add des, x4, #0

    add     src1_1,    src1_0, #512*1
    add     src2_1,    src2_0, #512*1
    add  src2asy_1, src2asy_0, #256*1

#if KYBER_K > 2
    add     src1_2,    src1_0, #512*2
    add     src2_2,    src2_0, #512*2
    add  src2asy_2, src2asy_0, #256*2
#endif

#if KYBER_K > 3
    add     src1_3,    src1_0, #512*3
    add     src2_3,    src2_0, #512*3
    add  src2asy_3, src2asy_0, #256*3
#endif

    dup v26.8H, R3
    dup v27.8H, R3p

    dup v28.8H, Q
    dup v29.8H, Qprime2

    // TODO: interleaving
    mov counter, #16
    _asymmetric_mul_montgomery_loop:

    ld2 { v0.8H,  v1.8H}, [   src1_0], #32
    ld2 { v2.8H,  v3.8H}, [   src2_0], #32
    ld1 { v5.8H}, [src2asy_0], #16

    smull  v16.4S,  v0.4H,  v2.4H
    smull2 v20.4S,  v0.8H,  v2.8H
    smull  v17.4S,  v0.4H,  v3.4H
    smull2 v21.4S,  v0.8H,  v3.8H

    smlal  v16.4S,  v1.4H,  v5.4H
    smlal2 v20.4S,  v1.8H,  v5.8H
    smlal  v17.4S,  v1.4H,  v2.4H
    smlal2 v21.4S,  v1.8H,  v2.8H

    ld2 { v0.8H,  v1.8H}, [   src1_1], #32
    ld2 { v2.8H,  v3.8H}, [   src2_1], #32
    ld1 { v5.8H}, [src2asy_1], #16

    smlal  v16.4S,  v0.4H,  v2.4H
    smlal2 v20.4S,  v0.8H,  v2.8H
    smlal  v17.4S,  v0.4H,  v3.4H
    smlal2 v21.4S,  v0.8H,  v3.8H

    smlal  v16.4S,  v1.4H,  v5.4H
    smlal2 v20.4S,  v1.8H,  v5.8H
    smlal  v17.4S,  v1.4H,  v2.4H
    smlal2 v21.4S,  v1.8H,  v2.8H

#if KYBER_K > 2
    ld2 { v0.8H,  v1.8H}, [   src1_2], #32
    ld2 { v2.8H,  v3.8H}, [   src2_2], #32
    ld1 { v5.8H}, [src2asy_2], #16

    smlal  v16.4S,  v0.4H,  v2.4H
    smlal2 v20.4S,  v0.8H,  v2.8H
    smlal  v17.4S,  v0.4H,  v3.4H
    smlal2 v21.4S,  v0.8H,  v3.8H

    smlal  v16.4S,  v1.4H,  v5.4H
    smlal2 v20.4S,  v1.8H,  v5.8H
    smlal  v17.4S,  v1.4H,  v2.4H
    smlal2 v21.4S,  v1.8H,  v2.8H
#endif

#if KYBER_K > 3
    ld2 { v0.8H,  v1.8H}, [   src1_3], #32
    ld2 { v2.8H,  v3.8H}, [   src2_3], #32
    ld1 { v5.8H}, [src2asy_3], #16

    smlal  v16.4S,  v0.4H,  v2.4H
    smlal2 v20.4S,  v0.8H,  v2.8H
    smlal  v17.4S,  v0.4H,  v3.4H
    smlal2 v21.4S,  v0.8H,  v3.8H

    smlal  v16.4S,  v1.4H,  v5.4H
    smlal2 v20.4S,  v1.8H,  v5.8H
    smlal  v17.4S,  v1.4H,  v2.4H
    smlal2 v21.4S,  v1.8H,  v2.8H
#endif

    uzp1    v0.8H, v16.8H, v20.8H
    uzp1    v1.8H, v17.8H, v21.8H

    mul     v0.8H,  v0.8H, v29.8H
    mul     v1.8H,  v1.8H, v29.8H

    smlal  v16.4S,  v0.4H, v28.4H
    smlal2 v20.4S,  v0.8H, v28.8H
    smlal  v17.4S,  v1.4H, v28.4H
    smlal2 v21.4S,  v1.8H, v28.8H

    uzp2   v24.8H, v16.8H, v20.8H
    uzp2   v25.8H, v17.8H, v21.8H

    sqrdmulh v16.8H, v24.8H, v26.8H
    sqrdmulh v17.8H, v25.8H, v26.8H

    mul      v24.8H, v24.8H, v27.8H
    mul      v25.8H, v25.8H, v27.8H

    mls      v24.8H, v16.8H, v28.8H
    mls      v25.8H, v17.8H, v28.8H

    st2 {v24.8H, v25.8H}, [des], #32

    sub counter, counter, #1
    cbnz counter, _asymmetric_mul_montgomery_loop

    .unreq    Q
    .unreq    Qprime2
    .unreq    R3
    .unreq    R3p
    .unreq    des
    .unreq    src1_0
    .unreq    src2_0
    .unreq    src2asy_0
    .unreq    src1_1
    .unreq    src2_1
    .unreq    src2asy_1
    .unreq    src1_2
    .unreq    src2_2
    .unreq    src2asy_2
    .unreq    src1_3
    .unreq    src2_3
    .unreq    src2asy_3
    .unreq    counter
    pop_all

    br lr



