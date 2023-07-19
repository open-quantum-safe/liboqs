
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

.align 2
.global PQCLEAN_KYBER1024_AARCH64__asm_add_reduce
.global _PQCLEAN_KYBER1024_AARCH64__asm_add_reduce
PQCLEAN_KYBER1024_AARCH64__asm_add_reduce:
_PQCLEAN_KYBER1024_AARCH64__asm_add_reduce:

    mov w4, #3329
    mov w5, #25519

    add x2, x0, #0

    dup v0.8H, w4
    dup v1.8H, w5

    ld1 {v24.8H, v25.8H, v26.8H, v27.8H}, [x1], #64
    ld1 {v28.8H, v29.8H, v30.8H, v31.8H}, [x1], #64
    ld1 {v16.8H, v17.8H, v18.8H, v19.8H}, [x2], #64
    ld1 {v20.8H, v21.8H, v22.8H, v23.8H}, [x2], #64

    add  v4.8H, v16.8H, v24.8H
    add  v5.8H, v17.8H, v25.8H
    add  v6.8H, v18.8H, v26.8H
    add  v7.8H, v19.8H, v27.8H

    add v16.8H, v20.8H, v28.8H
    add v17.8H, v21.8H, v29.8H
    add v18.8H, v22.8H, v30.8H
    add v19.8H, v23.8H, v31.8H

    oo_barrett  v4,  v5,  v6,  v7, v20, v21, v22, v23, v16, v17, v18, v19, v24, v25, v26, v27,  v1, #11,  v0

    mov x15, #3
    _add_reduce_loop:

    st1 { v4.8H,  v5.8H,  v6.8H,  v7.8H}, [x0], #64
    ld1 {v24.8H, v25.8H, v26.8H, v27.8H}, [x1], #64
    st1 {v16.8H, v17.8H, v18.8H, v19.8H}, [x0], #64
    ld1 {v28.8H, v29.8H, v30.8H, v31.8H}, [x1], #64
    ld1 {v16.8H, v17.8H, v18.8H, v19.8H}, [x2], #64
    ld1 {v20.8H, v21.8H, v22.8H, v23.8H}, [x2], #64

    add  v4.8H, v16.8H, v24.8H
    add  v5.8H, v17.8H, v25.8H
    add  v6.8H, v18.8H, v26.8H
    add  v7.8H, v19.8H, v27.8H

    add v16.8H, v20.8H, v28.8H
    add v17.8H, v21.8H, v29.8H
    add v18.8H, v22.8H, v30.8H
    add v19.8H, v23.8H, v31.8H

    oo_barrett  v4,  v5,  v6,  v7, v20, v21, v22, v23, v16, v17, v18, v19, v24, v25, v26, v27,  v1, #11,  v0

    sub x15, x15, #1
    cbnz x15, _add_reduce_loop

    st1 { v4.8H,  v5.8H,  v6.8H,  v7.8H}, [x0], #64
    st1 {v16.8H, v17.8H, v18.8H, v19.8H}, [x0], #64

    br lr

.align 2
.global PQCLEAN_KYBER1024_AARCH64__asm_sub_reduce
.global _PQCLEAN_KYBER1024_AARCH64__asm_sub_reduce
PQCLEAN_KYBER1024_AARCH64__asm_sub_reduce:
_PQCLEAN_KYBER1024_AARCH64__asm_sub_reduce:

    mov w4, #3329
    mov w5, #25519

    add x2, x0, #0

    dup v0.8H, w4
    dup v1.8H, w5

    ld1 {v24.8H, v25.8H, v26.8H, v27.8H}, [x1], #64
    ld1 {v28.8H, v29.8H, v30.8H, v31.8H}, [x1], #64
    ld1 {v16.8H, v17.8H, v18.8H, v19.8H}, [x2], #64
    ld1 {v20.8H, v21.8H, v22.8H, v23.8H}, [x2], #64

    sub  v4.8H, v16.8H, v24.8H
    sub  v5.8H, v17.8H, v25.8H
    sub  v6.8H, v18.8H, v26.8H
    sub  v7.8H, v19.8H, v27.8H

    sub v16.8H, v20.8H, v28.8H
    sub v17.8H, v21.8H, v29.8H
    sub v18.8H, v22.8H, v30.8H
    sub v19.8H, v23.8H, v31.8H

    oo_barrett  v4,  v5,  v6,  v7, v20, v21, v22, v23, v16, v17, v18, v19, v24, v25, v26, v27,  v1, #11,  v0

    mov x15, #3
    _sub_reduce_loop:

    st1 { v4.8H,  v5.8H,  v6.8H,  v7.8H}, [x0], #64
    ld1 {v24.8H, v25.8H, v26.8H, v27.8H}, [x1], #64
    st1 {v16.8H, v17.8H, v18.8H, v19.8H}, [x0], #64
    ld1 {v28.8H, v29.8H, v30.8H, v31.8H}, [x1], #64
    ld1 {v16.8H, v17.8H, v18.8H, v19.8H}, [x2], #64
    ld1 {v20.8H, v21.8H, v22.8H, v23.8H}, [x2], #64

    sub  v4.8H, v16.8H, v24.8H
    sub  v5.8H, v17.8H, v25.8H
    sub  v6.8H, v18.8H, v26.8H
    sub  v7.8H, v19.8H, v27.8H

    sub v16.8H, v20.8H, v28.8H
    sub v17.8H, v21.8H, v29.8H
    sub v18.8H, v22.8H, v30.8H
    sub v19.8H, v23.8H, v31.8H

    oo_barrett  v4,  v5,  v6,  v7, v20, v21, v22, v23, v16, v17, v18, v19, v24, v25, v26, v27,  v1, #11,  v0

    sub x15, x15, #1
    cbnz x15, _sub_reduce_loop

    st1 { v4.8H,  v5.8H,  v6.8H,  v7.8H}, [x0], #64
    st1 {v16.8H, v17.8H, v18.8H, v19.8H}, [x0], #64

    br lr

.align 2
.global PQCLEAN_KYBER1024_AARCH64__asm_add_add_reduce
.global _PQCLEAN_KYBER1024_AARCH64__asm_add_add_reduce
PQCLEAN_KYBER1024_AARCH64__asm_add_add_reduce:
_PQCLEAN_KYBER1024_AARCH64__asm_add_add_reduce:

    mov w4, #3329
    mov w5, #25519

    add x3, x0, #0

    dup v0.8H, w4
    dup v1.8H, w5

    ld1 { v4.8H,  v5.8H,  v6.8H,  v7.8H}, [x3], #64
    ld1 {v20.8H, v21.8H, v22.8H, v23.8H}, [x3], #64
    ld1 {v16.8H, v17.8H, v18.8H, v19.8H}, [x1], #64
    ld1 {v24.8H, v25.8H, v26.8H, v27.8H}, [x1], #64

    add   v4.8H,  v4.8H, v16.8H
    add   v5.8H,  v5.8H, v17.8H
    ld1 {v16.8H, v17.8H}, [x2], #32
    add   v6.8H,  v6.8H, v18.8H
    add   v7.8H,  v7.8H, v19.8H
    ld1 {v18.8H, v19.8H}, [x2], #32
    add  v20.8H, v20.8H, v24.8H
    add  v21.8H, v21.8H, v25.8H
    ld1 {v24.8H, v25.8H}, [x2], #32
    add  v22.8H, v22.8H, v26.8H
    add  v23.8H, v23.8H, v27.8H
    ld1 {v26.8H, v27.8H}, [x2], #32

    add   v4.8H,  v4.8H, v16.8H
    add   v5.8H,  v5.8H, v17.8H
    add   v6.8H,  v6.8H, v18.8H
    add   v7.8H,  v7.8H, v19.8H
    add  v20.8H, v20.8H, v24.8H
    add  v21.8H, v21.8H, v25.8H
    add  v22.8H, v22.8H, v26.8H
    add  v23.8H, v23.8H, v27.8H

    oo_barrett  v4,  v5,  v6,  v7, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27,  v1, #11,  v0

    mov x15, #3
    _add_add_reduce_loop:

    st1 { v4.8H,  v5.8H,  v6.8H,  v7.8H}, [x0], #64
    ld1 { v4.8H,  v5.8H,  v6.8H,  v7.8H}, [x3], #64
    st1 {v20.8H, v21.8H, v22.8H, v23.8H}, [x0], #64
    ld1 {v20.8H, v21.8H, v22.8H, v23.8H}, [x3], #64
    ld1 {v16.8H, v17.8H, v18.8H, v19.8H}, [x1], #64
    ld1 {v24.8H, v25.8H, v26.8H, v27.8H}, [x1], #64

    add   v4.8H,  v4.8H, v16.8H
    add   v5.8H,  v5.8H, v17.8H
    ld1 {v16.8H, v17.8H}, [x2], #32
    add   v6.8H,  v6.8H, v18.8H
    add   v7.8H,  v7.8H, v19.8H
    ld1 {v18.8H, v19.8H}, [x2], #32
    add  v20.8H, v20.8H, v24.8H
    add  v21.8H, v21.8H, v25.8H
    ld1 {v24.8H, v25.8H}, [x2], #32
    add  v22.8H, v22.8H, v26.8H
    add  v23.8H, v23.8H, v27.8H
    ld1 {v26.8H, v27.8H}, [x2], #32

    add   v4.8H,  v4.8H, v16.8H
    add   v5.8H,  v5.8H, v17.8H
    add   v6.8H,  v6.8H, v18.8H
    add   v7.8H,  v7.8H, v19.8H
    add  v20.8H, v20.8H, v24.8H
    add  v21.8H, v21.8H, v25.8H
    add  v22.8H, v22.8H, v26.8H
    add  v23.8H, v23.8H, v27.8H

    oo_barrett  v4,  v5,  v6,  v7, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27,  v1, #11,  v0

    sub x15, x15, #1
    cbnz x15, _add_add_reduce_loop

    st1 { v4.8H,  v5.8H,  v6.8H,  v7.8H}, [x0], #64
    st1 {v20.8H, v21.8H, v22.8H, v23.8H}, [x0], #64

    br lr












