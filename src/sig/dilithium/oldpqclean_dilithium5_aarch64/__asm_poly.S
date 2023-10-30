
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
.global PQCLEAN_DILITHIUM5_AARCH64__asm_10_to_32
.global _PQCLEAN_DILITHIUM5_AARCH64__asm_10_to_32
PQCLEAN_DILITHIUM5_AARCH64__asm_10_to_32:
_PQCLEAN_DILITHIUM5_AARCH64__asm_10_to_32:

    mov x7, #16
    _10_to_32_loop:

    ldr w2, [x1], #4

    ubfx w3, w2, #0, #10
    str w3, [x0], #4
    ubfx w4, w2, #10, #10
    str w4, [x0], #4
    ubfx w5, w2, #20, #10
    str w5, [x0], #4
    lsr w6, w2, #30    

    ldr w2, [x1], #4

    ubfx w3, w2, #0, #8
    lsl w3, w3, #2
    orr w3, w3, w6
    str w3, [x0], #4
    ubfx w4, w2, #8, #10
    str w4, [x0], #4
    ubfx w5, w2, #18, #10
    str w5, [x0], #4
    lsr w6, w2, #28

    ldr w2, [x1], #4

    ubfx w3, w2, #0, #6
    lsl w3, w3, #4
    orr w3, w3, w6
    str w3, [x0], #4
    ubfx w4, w2, #6, #10
    str w4, [x0], #4
    ubfx w5, w2, #16, #10
    str w5, [x0], #4
    lsr w6, w2, #26

    ldr w2, [x1], #4

    ubfx w3, w2, #0, #4
    lsl w3, w3, #6
    orr w3, w3, w6
    str w3, [x0], #4
    ubfx w4, w2, #4, #10
    str w4, [x0], #4
    ubfx w5, w2, #14, #10
    str w5, [x0], #4
    lsr w6, w2, #24

    ldr w2, [x1], #4

    ubfx w3, w2, #0, #2
    lsl w3, w3, #8
    orr w3, w3, w6
    str w3, [x0], #4
    ubfx w4, w2, #2, #10
    str w4, [x0], #4
    ubfx w5, w2, #12, #10
    str w5, [x0], #4
    ubfx w6, w2, #22, #10
    str w6, [x0], #4

    sub x7, x7, #1
    cbnz x7, _10_to_32_loop

    br lr

.align 2
.global PQCLEAN_DILITHIUM5_AARCH64__asm_poly_reduce
.global _PQCLEAN_DILITHIUM5_AARCH64__asm_poly_reduce
PQCLEAN_DILITHIUM5_AARCH64__asm_poly_reduce:
_PQCLEAN_DILITHIUM5_AARCH64__asm_poly_reduce:

    ldr w4, [x1]

    dup v24.4S, w4

    add x1, x0, #0

    ld1 { v0.4S}, [x1], #16
    ld1 { v1.4S}, [x1], #16
    ld1 { v2.4S}, [x1], #16
    ld1 { v3.4S}, [x1], #16
    
    ld1 { v4.4S}, [x1], #16
    srshr  v16.4S, v0.4S, #23
    ld1 { v5.4S}, [x1], #16
    srshr  v17.4S, v1.4S, #23
    ld1 { v6.4S}, [x1], #16
    srshr  v18.4S, v2.4S, #23
    ld1 { v7.4S}, [x1], #16
    srshr  v19.4S, v3.4S, #23
    
    srshr  v20.4S, v4.4S, #23
    mls     v0.4S, v16.4S, v24.4S
    srshr  v21.4S, v5.4S, #23
    mls     v1.4S, v17.4S, v24.4S
    srshr  v22.4S, v6.4S, #23
    mls     v2.4S, v18.4S, v24.4S
    srshr  v23.4S, v7.4S, #23
    mls     v3.4S, v19.4S, v24.4S
    
    mls     v4.4S, v20.4S, v24.4S
    st1 { v0.4S}, [x0], #16
    mls     v5.4S, v21.4S, v24.4S
    st1 { v1.4S}, [x0], #16
    mls     v6.4S, v22.4S, v24.4S
    st1 { v2.4S}, [x0], #16
    mls     v7.4S, v23.4S, v24.4S
    st1 { v3.4S}, [x0], #16

    mov x16, #7
    _poly_reduce_loop:

    st1 { v4.4S}, [x0], #16
    ld1 { v0.4S}, [x1], #16
    st1 { v5.4S}, [x0], #16
    ld1 { v1.4S}, [x1], #16
    st1 { v6.4S}, [x0], #16
    ld1 { v2.4S}, [x1], #16
    st1 { v7.4S}, [x0], #16
    ld1 { v3.4S}, [x1], #16

    ld1 { v4.4S}, [x1], #16
    srshr  v16.4S, v0.4S, #23
    ld1 { v5.4S}, [x1], #16
    srshr  v17.4S, v1.4S, #23
    ld1 { v6.4S}, [x1], #16
    srshr  v18.4S, v2.4S, #23
    ld1 { v7.4S}, [x1], #16
    srshr  v19.4S, v3.4S, #23

    srshr  v20.4S, v4.4S, #23
    mls     v0.4S, v16.4S, v24.4S
    srshr  v21.4S, v5.4S, #23
    mls     v1.4S, v17.4S, v24.4S
    srshr  v22.4S, v6.4S, #23
    mls     v2.4S, v18.4S, v24.4S
    srshr  v23.4S, v7.4S, #23
    mls     v3.4S, v19.4S, v24.4S

    mls     v4.4S, v20.4S, v24.4S
    st1 { v0.4S}, [x0], #16
    mls     v5.4S, v21.4S, v24.4S
    st1 { v1.4S}, [x0], #16
    mls     v6.4S, v22.4S, v24.4S
    st1 { v2.4S}, [x0], #16
    mls     v7.4S, v23.4S, v24.4S
    st1 { v3.4S}, [x0], #16

    sub x16, x16, #1
    cbnz x16, _poly_reduce_loop

    st1 { v4.4S}, [x0], #16
    st1 { v5.4S}, [x0], #16
    st1 { v6.4S}, [x0], #16
    st1 { v7.4S}, [x0], #16

    br lr

.align 2
.global PQCLEAN_DILITHIUM5_AARCH64__asm_poly_caddq
.global _PQCLEAN_DILITHIUM5_AARCH64__asm_poly_caddq
PQCLEAN_DILITHIUM5_AARCH64__asm_poly_caddq:
_PQCLEAN_DILITHIUM5_AARCH64__asm_poly_caddq:

    ldr w4, [x1]

    dup v24.4S, w4

    add x1, x0, #0

    ld1 { v0.4S}, [x1], #16
    ld1 { v1.4S}, [x1], #16
    ld1 { v2.4S}, [x1], #16
    ld1 { v3.4S}, [x1], #16

    ld1 { v4.4S}, [x1], #16
    sshr  v16.4S, v0.4S, #31
    ld1 { v5.4S}, [x1], #16
    sshr  v17.4S, v1.4S, #31
    ld1 { v6.4S}, [x1], #16
    sshr  v18.4S, v2.4S, #31
    ld1 { v7.4S}, [x1], #16
    sshr  v19.4S, v3.4S, #31

    sshr  v20.4S, v4.4S, #31
    mls     v0.4S, v16.4S, v24.4S
    sshr  v21.4S, v5.4S, #31
    mls     v1.4S, v17.4S, v24.4S
    sshr  v22.4S, v6.4S, #31
    mls     v2.4S, v18.4S, v24.4S
    sshr  v23.4S, v7.4S, #31
    mls     v3.4S, v19.4S, v24.4S

    mls     v4.4S, v20.4S, v24.4S
    st1 { v0.4S}, [x0], #16
    mls     v5.4S, v21.4S, v24.4S
    st1 { v1.4S}, [x0], #16
    mls     v6.4S, v22.4S, v24.4S
    st1 { v2.4S}, [x0], #16
    mls     v7.4S, v23.4S, v24.4S
    st1 { v3.4S}, [x0], #16

    mov x16, #7
    _poly_caddq_loop:

    st1 { v4.4S}, [x0], #16
    ld1 { v0.4S}, [x1], #16
    st1 { v5.4S}, [x0], #16
    ld1 { v1.4S}, [x1], #16
    st1 { v6.4S}, [x0], #16
    ld1 { v2.4S}, [x1], #16
    st1 { v7.4S}, [x0], #16
    ld1 { v3.4S}, [x1], #16

    ld1 { v4.4S}, [x1], #16
    sshr  v16.4S, v0.4S, #31
    ld1 { v5.4S}, [x1], #16
    sshr  v17.4S, v1.4S, #31
    ld1 { v6.4S}, [x1], #16
    sshr  v18.4S, v2.4S, #31
    ld1 { v7.4S}, [x1], #16
    sshr  v19.4S, v3.4S, #31

    sshr  v20.4S, v4.4S, #31
    mls     v0.4S, v16.4S, v24.4S
    sshr  v21.4S, v5.4S, #31
    mls     v1.4S, v17.4S, v24.4S
    sshr  v22.4S, v6.4S, #31
    mls     v2.4S, v18.4S, v24.4S
    sshr  v23.4S, v7.4S, #31
    mls     v3.4S, v19.4S, v24.4S

    mls     v4.4S, v20.4S, v24.4S
    st1 { v0.4S}, [x0], #16
    mls     v5.4S, v21.4S, v24.4S
    st1 { v1.4S}, [x0], #16
    mls     v6.4S, v22.4S, v24.4S
    st1 { v2.4S}, [x0], #16
    mls     v7.4S, v23.4S, v24.4S
    st1 { v3.4S}, [x0], #16

    sub x16, x16, #1
    cbnz x16, _poly_caddq_loop

    st1 { v4.4S}, [x0], #16
    st1 { v5.4S}, [x0], #16
    st1 { v6.4S}, [x0], #16
    st1 { v7.4S}, [x0], #16

    br lr

.align 2
.global PQCLEAN_DILITHIUM5_AARCH64__asm_poly_freeze
.global _PQCLEAN_DILITHIUM5_AARCH64__asm_poly_freeze
PQCLEAN_DILITHIUM5_AARCH64__asm_poly_freeze:
_PQCLEAN_DILITHIUM5_AARCH64__asm_poly_freeze:

    ldr w4, [x1]

    dup v24.4S, w4

    add x1, x0, #0

    ld1 { v0.4S}, [x1], #16
    ld1 { v1.4S}, [x1], #16
    ld1 { v2.4S}, [x1], #16
    ld1 { v3.4S}, [x1], #16

    ld1 { v4.4S}, [x1], #16
    srshr  v16.4S, v0.4S, #23
    ld1 { v5.4S}, [x1], #16
    srshr  v17.4S, v1.4S, #23
    ld1 { v6.4S}, [x1], #16
    srshr  v18.4S, v2.4S, #23
    ld1 { v7.4S}, [x1], #16
    srshr  v19.4S, v3.4S, #23
    
    srshr  v20.4S, v4.4S, #23
    mls     v0.4S, v16.4S, v24.4S
    srshr  v21.4S, v5.4S, #23
    mls     v1.4S, v17.4S, v24.4S
    srshr  v22.4S, v6.4S, #23
    mls     v2.4S, v18.4S, v24.4S
    srshr  v23.4S, v7.4S, #23
    mls     v3.4S, v19.4S, v24.4S
    
    mls     v4.4S, v20.4S, v24.4S
    sshr  v16.4S, v0.4S, #31
    mls     v5.4S, v21.4S, v24.4S
    sshr  v17.4S, v1.4S, #31
    mls     v6.4S, v22.4S, v24.4S
    sshr  v18.4S, v2.4S, #31
    mls     v7.4S, v23.4S, v24.4S
    sshr  v19.4S, v3.4S, #31
    
    sshr  v20.4S, v4.4S, #31
    mls     v0.4S, v16.4S, v24.4S
    sshr  v21.4S, v5.4S, #31
    mls     v1.4S, v17.4S, v24.4S
    sshr  v22.4S, v6.4S, #31
    mls     v2.4S, v18.4S, v24.4S
    sshr  v23.4S, v7.4S, #31
    mls     v3.4S, v19.4S, v24.4S
    
    mls     v4.4S, v20.4S, v24.4S
    st1 { v0.4S}, [x0], #16
    mls     v5.4S, v21.4S, v24.4S
    st1 { v1.4S}, [x0], #16
    mls     v6.4S, v22.4S, v24.4S
    st1 { v2.4S}, [x0], #16
    mls     v7.4S, v23.4S, v24.4S
    st1 { v3.4S}, [x0], #16

    mov x16, #8
    _poly_freeze_loop:

    st1 { v4.4S}, [x0], #16
    ld1 { v0.4S}, [x1], #16
    st1 { v5.4S}, [x0], #16
    ld1 { v1.4S}, [x1], #16
    st1 { v6.4S}, [x0], #16
    ld1 { v2.4S}, [x1], #16
    st1 { v7.4S}, [x0], #16
    ld1 { v3.4S}, [x1], #16

    ld1 { v4.4S}, [x1], #16
    srshr  v16.4S, v0.4S, #23
    ld1 { v5.4S}, [x1], #16
    srshr  v17.4S, v1.4S, #23
    ld1 { v6.4S}, [x1], #16
    srshr  v18.4S, v2.4S, #23
    ld1 { v7.4S}, [x1], #16
    srshr  v19.4S, v3.4S, #23

    srshr  v20.4S, v4.4S, #23
    mls     v0.4S, v16.4S, v24.4S
    srshr  v21.4S, v5.4S, #23
    mls     v1.4S, v17.4S, v24.4S
    srshr  v22.4S, v6.4S, #23
    mls     v2.4S, v18.4S, v24.4S
    srshr  v23.4S, v7.4S, #23
    mls     v3.4S, v19.4S, v24.4S

    mls     v4.4S, v20.4S, v24.4S
    sshr  v16.4S, v0.4S, #31
    mls     v5.4S, v21.4S, v24.4S
    sshr  v17.4S, v1.4S, #31
    mls     v6.4S, v22.4S, v24.4S
    sshr  v18.4S, v2.4S, #31
    mls     v7.4S, v23.4S, v24.4S
    sshr  v19.4S, v3.4S, #31

    sshr  v20.4S, v4.4S, #31
    mls     v0.4S, v16.4S, v24.4S
    sshr  v21.4S, v5.4S, #31
    mls     v1.4S, v17.4S, v24.4S
    sshr  v22.4S, v6.4S, #31
    mls     v2.4S, v18.4S, v24.4S
    sshr  v23.4S, v7.4S, #31
    mls     v3.4S, v19.4S, v24.4S

    mls     v4.4S, v20.4S, v24.4S
    st1 { v0.4S}, [x0], #16
    mls     v5.4S, v21.4S, v24.4S
    st1 { v1.4S}, [x0], #16
    mls     v6.4S, v22.4S, v24.4S
    st1 { v2.4S}, [x0], #16
    mls     v7.4S, v23.4S, v24.4S
    st1 { v3.4S}, [x0], #16

    sub x16, x16, #1
    cbnz x16, _poly_freeze_loop

    st1 { v4.4S}, [x0], #16
    st1 { v5.4S}, [x0], #16
    st1 { v6.4S}, [x0], #16
    st1 { v7.4S}, [x0], #16

    br lr

.align 2
.global PQCLEAN_DILITHIUM5_AARCH64__asm_poly_power2round
.global _PQCLEAN_DILITHIUM5_AARCH64__asm_poly_power2round
PQCLEAN_DILITHIUM5_AARCH64__asm_poly_power2round:
_PQCLEAN_DILITHIUM5_AARCH64__asm_poly_power2round:

    mov w4, #1

    dup v28.4S, w4

    ld1 { v0.4S}, [x2], #16
    ld1 { v1.4S}, [x2], #16
    ld1 { v2.4S}, [x2], #16
    ld1 { v3.4S}, [x2], #16

    ld1 {v20.4S}, [x2], #16
    sub    v4.4S,  v0.4S, v28.4S
    ld1 {v21.4S}, [x2], #16
    sub    v5.4S,  v1.4S, v28.4S
    ld1 {v22.4S}, [x2], #16
    sub    v6.4S,  v2.4S, v28.4S
    ld1 {v23.4S}, [x2], #16
    sub    v7.4S,  v3.4S, v28.4S

    sub   v24.4S, v20.4S, v28.4S
    srshr v16.4S,  v4.4S, #13
    sub   v25.4S, v21.4S, v28.4S
    srshr v17.4S,  v5.4S, #13
    sub   v26.4S, v22.4S, v28.4S
    srshr v18.4S,  v6.4S, #13
    sub   v27.4S, v23.4S, v28.4S
    srshr v19.4S,  v7.4S, #13

    srshr v28.4S, v24.4S, #13
    st1 {v16.4S}, [x0], #16
    srshr v29.4S, v25.4S, #13
    st1 {v17.4S}, [x0], #16
    srshr v30.4S, v26.4S, #13
    st1 {v18.4S}, [x0], #16
    srshr v31.4S, v27.4S, #13
    st1 {v19.4S}, [x0], #16

    st1 {v28.4S}, [x0], #16
    shl    v4.4S, v16.4S, #13
    st1 {v29.4S}, [x0], #16
    shl    v5.4S, v17.4S, #13
    st1 {v30.4S}, [x0], #16
    shl    v6.4S, v18.4S, #13
    st1 {v31.4S}, [x0], #16
    shl    v7.4S, v19.4S, #13

    shl   v24.4S, v28.4S, #13
    sub   v16.4S,  v0.4S,  v4.4S
    shl   v25.4S, v29.4S, #13
    sub   v17.4S,  v1.4S,  v5.4S
    shl   v26.4S, v30.4S, #13
    sub   v18.4S,  v2.4S,  v6.4S
    shl   v27.4S, v31.4S, #13
    sub   v19.4S,  v3.4S,  v7.4S

    sub   v28.4S, v20.4S, v24.4S
    st1 {v16.4S}, [x1], #16
    sub   v29.4S, v21.4S, v25.4S
    st1 {v17.4S}, [x1], #16
    sub   v30.4S, v22.4S, v26.4S
    st1 {v18.4S}, [x1], #16
    sub   v31.4S, v23.4S, v27.4S
    st1 {v19.4S}, [x1], #16

    mov x16, #7
    _poly_power2round_loop:

    st1 {v28.4S}, [x1], #16
    dup v28.4S, w4
    ld1 { v0.4S}, [x2], #16
    st1 {v29.4S}, [x1], #16
    ld1 { v1.4S}, [x2], #16
    st1 {v30.4S}, [x1], #16
    ld1 { v2.4S}, [x2], #16
    st1 {v31.4S}, [x1], #16
    ld1 { v3.4S}, [x2], #16

    ld1 {v20.4S}, [x2], #16
    sub    v4.4S,  v0.4S, v28.4S
    ld1 {v21.4S}, [x2], #16
    sub    v5.4S,  v1.4S, v28.4S
    ld1 {v22.4S}, [x2], #16
    sub    v6.4S,  v2.4S, v28.4S
    ld1 {v23.4S}, [x2], #16
    sub    v7.4S,  v3.4S, v28.4S

    sub   v24.4S, v20.4S, v28.4S
    srshr v16.4S,  v4.4S, #13
    sub   v25.4S, v21.4S, v28.4S
    srshr v17.4S,  v5.4S, #13
    sub   v26.4S, v22.4S, v28.4S
    srshr v18.4S,  v6.4S, #13
    sub   v27.4S, v23.4S, v28.4S
    srshr v19.4S,  v7.4S, #13

    srshr v28.4S, v24.4S, #13
    st1 {v16.4S}, [x0], #16
    srshr v29.4S, v25.4S, #13
    st1 {v17.4S}, [x0], #16
    srshr v30.4S, v26.4S, #13
    st1 {v18.4S}, [x0], #16
    srshr v31.4S, v27.4S, #13
    st1 {v19.4S}, [x0], #16

    st1 {v28.4S}, [x0], #16
    shl    v4.4S, v16.4S, #13
    st1 {v29.4S}, [x0], #16
    shl    v5.4S, v17.4S, #13
    st1 {v30.4S}, [x0], #16
    shl    v6.4S, v18.4S, #13
    st1 {v31.4S}, [x0], #16
    shl    v7.4S, v19.4S, #13

    shl   v24.4S, v28.4S, #13
    sub   v16.4S,  v0.4S,  v4.4S
    shl   v25.4S, v29.4S, #13
    sub   v17.4S,  v1.4S,  v5.4S
    shl   v26.4S, v30.4S, #13
    sub   v18.4S,  v2.4S,  v6.4S
    shl   v27.4S, v31.4S, #13
    sub   v19.4S,  v3.4S,  v7.4S

    sub   v28.4S, v20.4S, v24.4S
    st1 {v16.4S}, [x1], #16
    sub   v29.4S, v21.4S, v25.4S
    st1 {v17.4S}, [x1], #16
    sub   v30.4S, v22.4S, v26.4S
    st1 {v18.4S}, [x1], #16
    sub   v31.4S, v23.4S, v27.4S
    st1 {v19.4S}, [x1], #16

    sub x16, x16, #1
    cbnz x16, _poly_power2round_loop

    st1 {v28.4S}, [x1], #16
    st1 {v29.4S}, [x1], #16
    st1 {v30.4S}, [x1], #16
    st1 {v31.4S}, [x1], #16

    br lr

.align 2
.global PQCLEAN_DILITHIUM5_AARCH64__asm_poly_add
.global _PQCLEAN_DILITHIUM5_AARCH64__asm_poly_add
PQCLEAN_DILITHIUM5_AARCH64__asm_poly_add:
_PQCLEAN_DILITHIUM5_AARCH64__asm_poly_add:

    ld1 {v0.4S}, [x1], #16
    ld1 {v4.4S}, [x2], #16
    add v16.4S,  v0.4S,  v4.4S
    ld1 {v1.4S}, [x1], #16
    ld1 {v5.4S}, [x2], #16
    add v17.4S,  v1.4S,  v5.4S
    ld1 {v2.4S}, [x1], #16
    ld1 {v6.4S}, [x2], #16
    add v18.4S,  v2.4S,  v6.4S
    ld1 {v3.4S}, [x1], #16
    ld1 {v7.4S}, [x2], #16
    add v19.4S,  v3.4S,  v7.4S

    mov x16, #15
    _poly_add_loop:

    st1 {v16.4S}, [x0], #16
    ld1 {v0.4S}, [x1], #16
    ld1 {v4.4S}, [x2], #16
    add v16.4S,  v0.4S,  v4.4S
    st1 {v17.4S}, [x0], #16
    ld1 {v1.4S}, [x1], #16
    ld1 {v5.4S}, [x2], #16
    add v17.4S,  v1.4S,  v5.4S
    st1 {v18.4S}, [x0], #16
    ld1 {v2.4S}, [x1], #16
    ld1 {v6.4S}, [x2], #16
    add v18.4S,  v2.4S,  v6.4S
    st1 {v19.4S}, [x0], #16
    ld1 {v3.4S}, [x1], #16
    ld1 {v7.4S}, [x2], #16
    add v19.4S,  v3.4S,  v7.4S

    sub x16, x16, #1
    cbnz x16, _poly_add_loop

    st1 {v16.4S}, [x0], #16
    st1 {v17.4S}, [x0], #16
    st1 {v18.4S}, [x0], #16
    st1 {v19.4S}, [x0], #16

    br lr

.align 2
.global PQCLEAN_DILITHIUM5_AARCH64__asm_poly_sub
.global _PQCLEAN_DILITHIUM5_AARCH64__asm_poly_sub
PQCLEAN_DILITHIUM5_AARCH64__asm_poly_sub:
_PQCLEAN_DILITHIUM5_AARCH64__asm_poly_sub:

    ld1 {v0.4S}, [x1], #16
    ld1 {v4.4S}, [x2], #16
    sub v16.4S,  v0.4S,  v4.4S
    ld1 {v1.4S}, [x1], #16
    ld1 {v5.4S}, [x2], #16
    sub v17.4S,  v1.4S,  v5.4S
    ld1 {v2.4S}, [x1], #16
    ld1 {v6.4S}, [x2], #16
    sub v18.4S,  v2.4S,  v6.4S
    ld1 {v3.4S}, [x1], #16
    ld1 {v7.4S}, [x2], #16
    sub v19.4S,  v3.4S,  v7.4S

    mov x16, #15
    _poly_sub_loop:
    
    st1 {v16.4S}, [x0], #16
    ld1 {v0.4S}, [x1], #16
    ld1 {v4.4S}, [x2], #16
    sub v16.4S,  v0.4S,  v4.4S
    st1 {v17.4S}, [x0], #16
    ld1 {v1.4S}, [x1], #16
    ld1 {v5.4S}, [x2], #16
    sub v17.4S,  v1.4S,  v5.4S
    st1 {v18.4S}, [x0], #16
    ld1 {v2.4S}, [x1], #16
    ld1 {v6.4S}, [x2], #16
    sub v18.4S,  v2.4S,  v6.4S
    st1 {v19.4S}, [x0], #16
    ld1 {v3.4S}, [x1], #16
    ld1 {v7.4S}, [x2], #16
    sub v19.4S,  v3.4S,  v7.4S

    sub x16, x16, #1
    cbnz x16, _poly_sub_loop

    st1 {v16.4S}, [x0], #16
    st1 {v17.4S}, [x0], #16
    st1 {v18.4S}, [x0], #16
    st1 {v19.4S}, [x0], #16

    br lr

.align 2
.global PQCLEAN_DILITHIUM5_AARCH64__asm_poly_shiftl
.global _PQCLEAN_DILITHIUM5_AARCH64__asm_poly_shiftl
PQCLEAN_DILITHIUM5_AARCH64__asm_poly_shiftl:
_PQCLEAN_DILITHIUM5_AARCH64__asm_poly_shiftl:

    add x1, x0, #0

    ld1 { v0.4S}, [x1], #16
    shl v16.4S,  v0.4S, #13
    ld1 { v1.4S}, [x1], #16
    shl v17.4S,  v1.4S, #13
    ld1 { v2.4S}, [x1], #16
    shl v18.4S,  v2.4S, #13
    ld1 { v3.4S}, [x1], #16
    shl v19.4S,  v3.4S, #13
    ld1 { v4.4S}, [x1], #16
    shl v20.4S,  v4.4S, #13
    ld1 { v5.4S}, [x1], #16
    shl v21.4S,  v5.4S, #13
    ld1 { v6.4S}, [x1], #16
    shl v22.4S,  v6.4S, #13
    ld1 { v7.4S}, [x1], #16
    shl v23.4S,  v7.4S, #13

    mov x16, #7
    _poly_shiftl_loop:

    st1 {v16.4S}, [x0], #16
    ld1 { v0.4S}, [x1], #16
    shl v16.4S,  v0.4S, #13
    st1 {v17.4S}, [x0], #16
    ld1 { v1.4S}, [x1], #16
    shl v17.4S,  v1.4S, #13
    st1 {v18.4S}, [x0], #16
    ld1 { v2.4S}, [x1], #16
    shl v18.4S,  v2.4S, #13
    st1 {v19.4S}, [x0], #16
    ld1 { v3.4S}, [x1], #16
    shl v19.4S,  v3.4S, #13
    st1 {v20.4S}, [x0], #16
    ld1 { v4.4S}, [x1], #16
    shl v20.4S,  v4.4S, #13
    st1 {v21.4S}, [x0], #16
    ld1 { v5.4S}, [x1], #16
    shl v21.4S,  v5.4S, #13
    st1 {v22.4S}, [x0], #16
    ld1 { v6.4S}, [x1], #16
    shl v22.4S,  v6.4S, #13
    st1 {v23.4S}, [x0], #16
    ld1 { v7.4S}, [x1], #16
    shl v23.4S,  v7.4S, #13

    sub x16, x16, #1
    cbnz x16, _poly_shiftl_loop

    st1 {v16.4S}, [x0], #16
    st1 {v17.4S}, [x0], #16
    st1 {v18.4S}, [x0], #16
    st1 {v19.4S}, [x0], #16
    st1 {v20.4S}, [x0], #16
    st1 {v21.4S}, [x0], #16
    st1 {v22.4S}, [x0], #16
    st1 {v23.4S}, [x0], #16

    br lr

.align 2
.global PQCLEAN_DILITHIUM5_AARCH64__asm_poly_pointwise_montgomery
.global _PQCLEAN_DILITHIUM5_AARCH64__asm_poly_pointwise_montgomery
PQCLEAN_DILITHIUM5_AARCH64__asm_poly_pointwise_montgomery:
_PQCLEAN_DILITHIUM5_AARCH64__asm_poly_pointwise_montgomery:

    push_all

    ldr w20, [x3, #0]
    ldr w21, [x3, #4]

    dup v30.4S, w20
    dup v31.4S, w21

    ld1 { v0.4S}, [x1], #16
    ld1 { v1.4S}, [x1], #16
    ld1 { v2.4S}, [x1], #16
    ld1 { v3.4S}, [x1], #16
    ld1 { v4.4S}, [x2], #16
    ld1 { v5.4S}, [x2], #16
    ld1 { v6.4S}, [x2], #16
    ld1 { v7.4S}, [x2], #16

    smull  v12.2D,  v0.2S,  v4.2S
    smull2 v16.2D,  v0.4S,  v4.4S
    smull  v13.2D,  v1.2S,  v5.2S
    smull2 v17.2D,  v1.4S,  v5.4S
    smull  v14.2D,  v2.2S,  v6.2S
    smull2 v18.2D,  v2.4S,  v6.4S
    smull  v15.2D,  v3.2S,  v7.2S
    smull2 v19.2D,  v3.4S,  v7.4S

    uzp1   v20.4S, v12.4S, v16.4S
    uzp1   v21.4S, v13.4S, v17.4S
    uzp1   v22.4S, v14.4S, v18.4S
    uzp1   v23.4S, v15.4S, v19.4S

    mul    v24.4S, v20.4S, v31.4S
    mul    v25.4S, v21.4S, v31.4S
    mul    v26.4S, v22.4S, v31.4S
    mul    v27.4S, v23.4S, v31.4S

    smlsl  v12.2D, v24.2S, v30.2S
    smlsl2 v16.2D, v24.4S, v30.4S
    smlsl  v13.2D, v25.2S, v30.2S
    smlsl2 v17.2D, v25.4S, v30.4S
    smlsl  v14.2D, v26.2S, v30.2S
    smlsl2 v18.2D, v26.4S, v30.4S
    smlsl  v15.2D, v27.2S, v30.2S
    smlsl2 v19.2D, v27.4S, v30.4S

    uzp2   v24.4S, v12.4S, v16.4S
    uzp2   v25.4S, v13.4S, v17.4S
    uzp2   v26.4S, v14.4S, v18.4S
    uzp2   v27.4S, v15.4S, v19.4S

    mov x16, #15
    _poly_pointwise_montgomery_loop:

    st1 {v24.4S}, [x0], #16
    ld1 { v0.4S}, [x1], #16
    st1 {v25.4S}, [x0], #16
    ld1 { v1.4S}, [x1], #16
    st1 {v26.4S}, [x0], #16
    ld1 { v2.4S}, [x1], #16
    st1 {v27.4S}, [x0], #16
    ld1 { v3.4S}, [x1], #16

    ld1 { v4.4S}, [x2], #16
    ld1 { v5.4S}, [x2], #16
    ld1 { v6.4S}, [x2], #16
    ld1 { v7.4S}, [x2], #16

    smull  v12.2D,  v0.2S,  v4.2S
    smull2 v16.2D,  v0.4S,  v4.4S
    smull  v13.2D,  v1.2S,  v5.2S
    smull2 v17.2D,  v1.4S,  v5.4S
    smull  v14.2D,  v2.2S,  v6.2S
    smull2 v18.2D,  v2.4S,  v6.4S
    smull  v15.2D,  v3.2S,  v7.2S
    smull2 v19.2D,  v3.4S,  v7.4S

    uzp1   v20.4S, v12.4S, v16.4S
    uzp1   v21.4S, v13.4S, v17.4S
    uzp1   v22.4S, v14.4S, v18.4S
    uzp1   v23.4S, v15.4S, v19.4S

    mul    v24.4S, v20.4S, v31.4S
    mul    v25.4S, v21.4S, v31.4S
    mul    v26.4S, v22.4S, v31.4S
    mul    v27.4S, v23.4S, v31.4S

    smlsl  v12.2D, v24.2S, v30.2S
    smlsl2 v16.2D, v24.4S, v30.4S
    smlsl  v13.2D, v25.2S, v30.2S
    smlsl2 v17.2D, v25.4S, v30.4S
    smlsl  v14.2D, v26.2S, v30.2S
    smlsl2 v18.2D, v26.4S, v30.4S
    smlsl  v15.2D, v27.2S, v30.2S
    smlsl2 v19.2D, v27.4S, v30.4S

    uzp2   v24.4S, v12.4S, v16.4S
    uzp2   v25.4S, v13.4S, v17.4S
    uzp2   v26.4S, v14.4S, v18.4S
    uzp2   v27.4S, v15.4S, v19.4S

    sub x16, x16, #1
    cbnz x16, _poly_pointwise_montgomery_loop

    st1 {v24.4S}, [x0], #16
    st1 {v25.4S}, [x0], #16
    st1 {v26.4S}, [x0], #16
    st1 {v27.4S}, [x0], #16

    pop_all

    br lr


.align 2
.global PQCLEAN_DILITHIUM5_AARCH64__asm_polyvecl_pointwise_acc_montgomery
.global _PQCLEAN_DILITHIUM5_AARCH64__asm_polyvecl_pointwise_acc_montgomery
PQCLEAN_DILITHIUM5_AARCH64__asm_polyvecl_pointwise_acc_montgomery:
_PQCLEAN_DILITHIUM5_AARCH64__asm_polyvecl_pointwise_acc_montgomery:

    push_all

    ldr w20, [x3, #0]
    ldr w21, [x3, #4]

    add  x5,  x1, #1024*1
    add  x6,  x2, #1024*1

    add  x7,  x1, #1024*2
    add  x8,  x2, #1024*2

    add  x9,  x1, #1024*3
    add x10,  x2, #1024*3

#if L > 4
    add x11,  x1, #1024*4
    add x12,  x2, #1024*4
#endif

#if L > 5
    add x13,  x11, #1024*1
    add x14,  x12, #1024*1

    add x15,  x11, #1024*2
    add x19,  x12, #1024*2
#endif

    dup v30.4S, w20
    dup v31.4S, w21

    ld1 { v0.4S}, [x1], #16
    ld1 { v1.4S}, [x1], #16
    ld1 { v2.4S}, [x1], #16
    ld1 { v3.4S}, [x1], #16
    ld1 { v4.4S}, [x2], #16
    ld1 { v5.4S}, [x2], #16
    ld1 { v6.4S}, [x2], #16
    ld1 { v7.4S}, [x2], #16

    smull  v12.2D,  v0.2S,  v4.2S
    smull2 v16.2D,  v0.4S,  v4.4S
    ld1 { v0.4S}, [ x5], #16
    ld1 { v4.4S}, [ x6], #16
    smull  v13.2D,  v1.2S,  v5.2S
    smull2 v17.2D,  v1.4S,  v5.4S
    ld1 { v1.4S}, [ x5], #16
    ld1 { v5.4S}, [ x6], #16
    smull  v14.2D,  v2.2S,  v6.2S
    smull2 v18.2D,  v2.4S,  v6.4S
    ld1 { v2.4S}, [ x5], #16
    ld1 { v6.4S}, [ x6], #16
    smull  v15.2D,  v3.2S,  v7.2S
    smull2 v19.2D,  v3.4S,  v7.4S
    ld1 { v3.4S}, [ x5], #16
    ld1 { v7.4S}, [ x6], #16

    smlal  v12.2D,  v0.2S,  v4.2S
    smlal2 v16.2D,  v0.4S,  v4.4S
    ld1 { v0.4S}, [ x7], #16 
    ld1 { v4.4S}, [ x8], #16 
    smlal  v13.2D,  v1.2S,  v5.2S
    smlal2 v17.2D,  v1.4S,  v5.4S
    ld1 { v1.4S}, [ x7], #16 
    ld1 { v5.4S}, [ x8], #16 
    smlal  v14.2D,  v2.2S,  v6.2S
    smlal2 v18.2D,  v2.4S,  v6.4S
    ld1 { v2.4S}, [ x7], #16 
    ld1 { v6.4S}, [ x8], #16 
    smlal  v15.2D,  v3.2S,  v7.2S
    smlal2 v19.2D,  v3.4S,  v7.4S
    ld1 { v3.4S}, [ x7], #16 
    ld1 { v7.4S}, [ x8], #16

    smlal  v12.2D,  v0.2S,  v4.2S
    smlal2 v16.2D,  v0.4S,  v4.4S
    ld1 { v0.4S}, [ x9], #16 
    ld1 { v4.4S}, [x10], #16 
    smlal  v13.2D,  v1.2S,  v5.2S
    smlal2 v17.2D,  v1.4S,  v5.4S
    ld1 { v1.4S}, [ x9], #16 
    ld1 { v5.4S}, [x10], #16 
    smlal  v14.2D,  v2.2S,  v6.2S
    smlal2 v18.2D,  v2.4S,  v6.4S
    ld1 { v2.4S}, [ x9], #16 
    ld1 { v6.4S}, [x10], #16 
    smlal  v15.2D,  v3.2S,  v7.2S
    smlal2 v19.2D,  v3.4S,  v7.4S
    ld1 { v3.4S}, [ x9], #16 
    ld1 { v7.4S}, [x10], #16

#if L > 4
    smlal  v12.2D,  v0.2S,  v4.2S
    smlal2 v16.2D,  v0.4S,  v4.4S
    ld1 { v0.4S}, [x11], #16 
    ld1 { v4.4S}, [x12], #16 
    smlal  v13.2D,  v1.2S,  v5.2S
    smlal2 v17.2D,  v1.4S,  v5.4S
    ld1 { v1.4S}, [x11], #16 
    ld1 { v5.4S}, [x12], #16 
    smlal  v14.2D,  v2.2S,  v6.2S
    smlal2 v18.2D,  v2.4S,  v6.4S
    ld1 { v2.4S}, [x11], #16 
    ld1 { v6.4S}, [x12], #16 
    smlal  v15.2D,  v3.2S,  v7.2S
    smlal2 v19.2D,  v3.4S,  v7.4S
    ld1 { v3.4S}, [x11], #16 
    ld1 { v7.4S}, [x12], #16
#endif

#if L > 5
    smlal  v12.2D,  v0.2S,  v4.2S
    smlal2 v16.2D,  v0.4S,  v4.4S
    ld1 { v0.4S}, [x13], #16 
    ld1 { v4.4S}, [x14], #16 
    smlal  v13.2D,  v1.2S,  v5.2S
    smlal2 v17.2D,  v1.4S,  v5.4S
    ld1 { v1.4S}, [x13], #16 
    ld1 { v5.4S}, [x14], #16 
    smlal  v14.2D,  v2.2S,  v6.2S
    smlal2 v18.2D,  v2.4S,  v6.4S
    ld1 { v2.4S}, [x13], #16 
    ld1 { v6.4S}, [x14], #16 
    smlal  v15.2D,  v3.2S,  v7.2S
    smlal2 v19.2D,  v3.4S,  v7.4S
    ld1 { v3.4S}, [x13], #16 
    ld1 { v7.4S}, [x14], #16

    smlal  v12.2D,  v0.2S,  v4.2S
    smlal2 v16.2D,  v0.4S,  v4.4S
    ld1 { v0.4S}, [x15], #16 
    ld1 { v4.4S}, [x19], #16 
    smlal  v13.2D,  v1.2S,  v5.2S
    smlal2 v17.2D,  v1.4S,  v5.4S
    ld1 { v1.4S}, [x15], #16 
    ld1 { v5.4S}, [x19], #16 
    smlal  v14.2D,  v2.2S,  v6.2S
    smlal2 v18.2D,  v2.4S,  v6.4S
    ld1 { v2.4S}, [x15], #16 
    ld1 { v6.4S}, [x19], #16 
    smlal  v15.2D,  v3.2S,  v7.2S
    smlal2 v19.2D,  v3.4S,  v7.4S
    ld1 { v3.4S}, [x15], #16 
    ld1 { v7.4S}, [x19], #16
#endif

    mov x16, #15
    _polyvecl_pointwise_acc_montgomery_loop:

    smlal  v12.2D,  v0.2S,  v4.2S
    smlal2 v16.2D,  v0.4S,  v4.4S
    smlal  v13.2D,  v1.2S,  v5.2S
    smlal2 v17.2D,  v1.4S,  v5.4S
    smlal  v14.2D,  v2.2S,  v6.2S
    smlal2 v18.2D,  v2.4S,  v6.4S
    smlal  v15.2D,  v3.2S,  v7.2S
    smlal2 v19.2D,  v3.4S,  v7.4S

    uzp1   v20.4S, v12.4S, v16.4S
    ld1 { v0.4S}, [x1], #16
    uzp1   v21.4S, v13.4S, v17.4S
    ld1 { v1.4S}, [x1], #16
    uzp1   v22.4S, v14.4S, v18.4S
    ld1 { v2.4S}, [x1], #16
    uzp1   v23.4S, v15.4S, v19.4S
    ld1 { v3.4S}, [x1], #16

    mul    v24.4S, v20.4S, v31.4S
    ld1 { v4.4S}, [x2], #16
    mul    v25.4S, v21.4S, v31.4S
    ld1 { v5.4S}, [x2], #16
    mul    v26.4S, v22.4S, v31.4S
    ld1 { v6.4S}, [x2], #16
    mul    v27.4S, v23.4S, v31.4S
    ld1 { v7.4S}, [x2], #16

    smlsl  v12.2D, v24.2S, v30.2S
    smlsl2 v16.2D, v24.4S, v30.4S
    smlsl  v13.2D, v25.2S, v30.2S
    smlsl2 v17.2D, v25.4S, v30.4S
    smlsl  v14.2D, v26.2S, v30.2S
    smlsl2 v18.2D, v26.4S, v30.4S
    smlsl  v15.2D, v27.2S, v30.2S
    smlsl2 v19.2D, v27.4S, v30.4S

    uzp2   v24.4S, v12.4S, v16.4S
    uzp2   v25.4S, v13.4S, v17.4S
    uzp2   v26.4S, v14.4S, v18.4S
    uzp2   v27.4S, v15.4S, v19.4S

    smull  v12.2D,  v0.2S,  v4.2S
    smull2 v16.2D,  v0.4S,  v4.4S
    ld1 { v0.4S}, [ x5], #16
    st1 {v24.4S}, [x0], #16
    ld1 { v4.4S}, [ x6], #16
    smull  v13.2D,  v1.2S,  v5.2S
    smull2 v17.2D,  v1.4S,  v5.4S
    ld1 { v1.4S}, [ x5], #16
    st1 {v25.4S}, [x0], #16
    ld1 { v5.4S}, [ x6], #16
    smull  v14.2D,  v2.2S,  v6.2S
    smull2 v18.2D,  v2.4S,  v6.4S
    ld1 { v2.4S}, [ x5], #16
    st1 {v26.4S}, [x0], #16
    ld1 { v6.4S}, [ x6], #16
    smull  v15.2D,  v3.2S,  v7.2S
    smull2 v19.2D,  v3.4S,  v7.4S
    ld1 { v3.4S}, [ x5], #16
    st1 {v27.4S}, [x0], #16
    ld1 { v7.4S}, [ x6], #16

    smlal  v12.2D,  v0.2S,  v4.2S
    smlal2 v16.2D,  v0.4S,  v4.4S
    ld1 { v0.4S}, [ x7], #16 
    ld1 { v4.4S}, [ x8], #16 
    smlal  v13.2D,  v1.2S,  v5.2S
    smlal2 v17.2D,  v1.4S,  v5.4S
    ld1 { v1.4S}, [ x7], #16 
    ld1 { v5.4S}, [ x8], #16 
    smlal  v14.2D,  v2.2S,  v6.2S
    smlal2 v18.2D,  v2.4S,  v6.4S
    ld1 { v2.4S}, [ x7], #16 
    ld1 { v6.4S}, [ x8], #16 
    smlal  v15.2D,  v3.2S,  v7.2S
    smlal2 v19.2D,  v3.4S,  v7.4S
    ld1 { v3.4S}, [ x7], #16 
    ld1 { v7.4S}, [ x8], #16

    smlal  v12.2D,  v0.2S,  v4.2S
    smlal2 v16.2D,  v0.4S,  v4.4S
    ld1 { v0.4S}, [ x9], #16 
    ld1 { v4.4S}, [x10], #16 
    smlal  v13.2D,  v1.2S,  v5.2S
    smlal2 v17.2D,  v1.4S,  v5.4S
    ld1 { v1.4S}, [ x9], #16 
    ld1 { v5.4S}, [x10], #16 
    smlal  v14.2D,  v2.2S,  v6.2S
    smlal2 v18.2D,  v2.4S,  v6.4S
    ld1 { v2.4S}, [ x9], #16 
    ld1 { v6.4S}, [x10], #16 
    smlal  v15.2D,  v3.2S,  v7.2S
    smlal2 v19.2D,  v3.4S,  v7.4S
    ld1 { v3.4S}, [ x9], #16 
    ld1 { v7.4S}, [x10], #16

#if L > 4
    smlal  v12.2D,  v0.2S,  v4.2S
    smlal2 v16.2D,  v0.4S,  v4.4S
    ld1 { v0.4S}, [x11], #16 
    ld1 { v4.4S}, [x12], #16 
    smlal  v13.2D,  v1.2S,  v5.2S
    smlal2 v17.2D,  v1.4S,  v5.4S
    ld1 { v1.4S}, [x11], #16 
    ld1 { v5.4S}, [x12], #16 
    smlal  v14.2D,  v2.2S,  v6.2S
    smlal2 v18.2D,  v2.4S,  v6.4S
    ld1 { v2.4S}, [x11], #16 
    ld1 { v6.4S}, [x12], #16 
    smlal  v15.2D,  v3.2S,  v7.2S
    smlal2 v19.2D,  v3.4S,  v7.4S
    ld1 { v3.4S}, [x11], #16 
    ld1 { v7.4S}, [x12], #16
#endif

#if L > 5
    smlal  v12.2D,  v0.2S,  v4.2S
    smlal2 v16.2D,  v0.4S,  v4.4S
    ld1 { v0.4S}, [x13], #16 
    ld1 { v4.4S}, [x14], #16 
    smlal  v13.2D,  v1.2S,  v5.2S
    smlal2 v17.2D,  v1.4S,  v5.4S
    ld1 { v1.4S}, [x13], #16 
    ld1 { v5.4S}, [x14], #16 
    smlal  v14.2D,  v2.2S,  v6.2S
    smlal2 v18.2D,  v2.4S,  v6.4S
    ld1 { v2.4S}, [x13], #16 
    ld1 { v6.4S}, [x14], #16 
    smlal  v15.2D,  v3.2S,  v7.2S
    smlal2 v19.2D,  v3.4S,  v7.4S
    ld1 { v3.4S}, [x13], #16 
    ld1 { v7.4S}, [x14], #16

    smlal  v12.2D,  v0.2S,  v4.2S
    smlal2 v16.2D,  v0.4S,  v4.4S
    ld1 { v0.4S}, [x15], #16 
    ld1 { v4.4S}, [x19], #16 
    smlal  v13.2D,  v1.2S,  v5.2S
    smlal2 v17.2D,  v1.4S,  v5.4S
    ld1 { v1.4S}, [x15], #16 
    ld1 { v5.4S}, [x19], #16 
    smlal  v14.2D,  v2.2S,  v6.2S
    smlal2 v18.2D,  v2.4S,  v6.4S
    ld1 { v2.4S}, [x15], #16 
    ld1 { v6.4S}, [x19], #16 
    smlal  v15.2D,  v3.2S,  v7.2S
    smlal2 v19.2D,  v3.4S,  v7.4S
    ld1 { v3.4S}, [x15], #16 
    ld1 { v7.4S}, [x19], #16
#endif

    sub x16, x16, #1
    cbnz x16, _polyvecl_pointwise_acc_montgomery_loop

    smlal  v12.2D,  v0.2S,  v4.2S
    smlal2 v16.2D,  v0.4S,  v4.4S
    smlal  v13.2D,  v1.2S,  v5.2S
    smlal2 v17.2D,  v1.4S,  v5.4S
    smlal  v14.2D,  v2.2S,  v6.2S
    smlal2 v18.2D,  v2.4S,  v6.4S
    smlal  v15.2D,  v3.2S,  v7.2S
    smlal2 v19.2D,  v3.4S,  v7.4S

    uzp1   v20.4S, v12.4S, v16.4S
    uzp1   v21.4S, v13.4S, v17.4S
    uzp1   v22.4S, v14.4S, v18.4S
    uzp1   v23.4S, v15.4S, v19.4S

    mul    v24.4S, v20.4S, v31.4S
    mul    v25.4S, v21.4S, v31.4S
    mul    v26.4S, v22.4S, v31.4S
    mul    v27.4S, v23.4S, v31.4S

    smlsl  v12.2D, v24.2S, v30.2S
    smlsl2 v16.2D, v24.4S, v30.4S
    smlsl  v13.2D, v25.2S, v30.2S
    smlsl2 v17.2D, v25.4S, v30.4S
    smlsl  v14.2D, v26.2S, v30.2S
    smlsl2 v18.2D, v26.4S, v30.4S
    smlsl  v15.2D, v27.2S, v30.2S
    smlsl2 v19.2D, v27.4S, v30.4S

    uzp2   v24.4S, v12.4S, v16.4S
    uzp2   v25.4S, v13.4S, v17.4S
    uzp2   v26.4S, v14.4S, v18.4S
    uzp2   v27.4S, v15.4S, v19.4S

    st1 {v24.4S}, [x0], #16
    st1 {v25.4S}, [x0], #16
    st1 {v26.4S}, [x0], #16
    st1 {v27.4S}, [x0], #16

    pop_all

    br lr




