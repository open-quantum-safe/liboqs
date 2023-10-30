
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

#ifndef MACROS_COMMON
#define MACROS_COMMON

// for ABI

.macro push_all

    sub sp, sp, #(16*9)
    stp x19, x20, [sp, #16*0]
    stp x21, x22, [sp, #16*1]
    stp x23, x24, [sp, #16*2]
    stp x25, x26, [sp, #16*3]
    stp x27, x28, [sp, #16*4]
    stp  d8,  d9, [sp, #16*5]
    stp d10, d11, [sp, #16*6]
    stp d12, d13, [sp, #16*7]
    stp d14, d15, [sp, #16*8]

.endm

.macro pop_all

    ldp x19, x20, [sp, #16*0]
    ldp x21, x22, [sp, #16*1]
    ldp x23, x24, [sp, #16*2]
    ldp x25, x26, [sp, #16*3]
    ldp x27, x28, [sp, #16*4]
    ldp  d8,  d9, [sp, #16*5]
    ldp d10, d11, [sp, #16*6]
    ldp d12, d13, [sp, #16*7]
    ldp d14, d15, [sp, #16*8]
    add sp, sp, #(16*9)

.endm

// vector-scalar butterflies

.macro wrap_dX_butterfly_top a0, a1, b0, b1, t0, t1, mod, z0, l0, h0, z1, l1, h1, wX, nX

    mul      \t0\wX, \b0\wX, \z0\nX[\h0]
    mul      \t1\wX, \b1\wX, \z1\nX[\h1]

    sqrdmulh \b0\wX, \b0\wX, \z0\nX[\l0]
    sqrdmulh \b1\wX, \b1\wX, \z1\nX[\l1]

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]

.endm

.macro wrap_dX_butterfly_bot a0, a1, b0, b1, t0, t1, mod, z0, l0, h0, z1, l1, h1, wX, nX

    sub     \b0\wX, \a0\wX, \t0\wX
    sub     \b1\wX, \a1\wX, \t1\wX

    add     \a0\wX, \a0\wX, \t0\wX
    add     \a1\wX, \a1\wX, \t1\wX

.endm

.macro wrap_dX_butterfly_mixed a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, wX, nX

    sub      \b0\wX, \a0\wX, \t0\wX
    mul      \t2\wX, \b2\wX, \z2\nX[\h2]
    sub      \b1\wX, \a1\wX, \t1\wX
    mul      \t3\wX, \b3\wX, \z3\nX[\h3]

    add      \a0\wX, \a0\wX, \t0\wX
    sqrdmulh \b2\wX, \b2\wX, \z2\nX[\l2]
    add      \a1\wX, \a1\wX, \t1\wX
    sqrdmulh \b3\wX, \b3\wX, \z3\nX[\l3]

    mls      \t2\wX, \b2\wX, \mod\nX[0]
    mls      \t3\wX, \b3\wX, \mod\nX[0]

.endm

.macro wrap_dX_butterfly_mixed_rev a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, wX, nX

    mul      \t0\wX, \b0\wX, \z0\nX[\h0]
    sub      \b2\wX, \a2\wX, \t2\wX
    mul      \t1\wX, \b1\wX, \z1\nX[\h1]
    sub      \b3\wX, \a3\wX, \t3\wX

    sqrdmulh \b0\wX, \b0\wX, \z0\nX[\l0]
    add      \a2\wX, \a2\wX, \t2\wX
    sqrdmulh \b1\wX, \b1\wX, \z1\nX[\l1]
    add      \a3\wX, \a3\wX, \t3\wX

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]

.endm

.macro wrap_qX_butterfly_top a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, wX, nX

    mul      \t0\wX, \b0\wX, \z0\nX[\h0]
    mul      \t1\wX, \b1\wX, \z1\nX[\h1]
    mul      \t2\wX, \b2\wX, \z2\nX[\h2]
    mul      \t3\wX, \b3\wX, \z3\nX[\h3]

    sqrdmulh \b0\wX, \b0\wX, \z0\nX[\l0]
    sqrdmulh \b1\wX, \b1\wX, \z1\nX[\l1]
    sqrdmulh \b2\wX, \b2\wX, \z2\nX[\l2]
    sqrdmulh \b3\wX, \b3\wX, \z3\nX[\l3]

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]
    mls      \t2\wX, \b2\wX, \mod\nX[0]
    mls      \t3\wX, \b3\wX, \mod\nX[0]

.endm

.macro wrap_qX_butterfly_bot a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, wX, nX

    sub     \b0\wX, \a0\wX, \t0\wX
    sub     \b1\wX, \a1\wX, \t1\wX
    sub     \b2\wX, \a2\wX, \t2\wX
    sub     \b3\wX, \a3\wX, \t3\wX

    add     \a0\wX, \a0\wX, \t0\wX
    add     \a1\wX, \a1\wX, \t1\wX
    add     \a2\wX, \a2\wX, \t2\wX
    add     \a3\wX, \a3\wX, \t3\wX

.endm

.macro wrap_qX_butterfly_mixed a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, a4, a5, a6, a7, b4, b5, b6, b7, t4, t5, t6, t7, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7, wX, nX

    sub      \b0\wX, \a0\wX, \t0\wX
    mul      \t4\wX, \b4\wX, \z4\nX[\h4]
    sub      \b1\wX, \a1\wX, \t1\wX
    mul      \t5\wX, \b5\wX, \z5\nX[\h5]
    sub      \b2\wX, \a2\wX, \t2\wX
    mul      \t6\wX, \b6\wX, \z6\nX[\h6]
    sub      \b3\wX, \a3\wX, \t3\wX
    mul      \t7\wX, \b7\wX, \z7\nX[\h7]

    add      \a0\wX, \a0\wX, \t0\wX
    sqrdmulh \b4\wX, \b4\wX, \z4\nX[\l4]
    add      \a1\wX, \a1\wX, \t1\wX
    sqrdmulh \b5\wX, \b5\wX, \z5\nX[\l5]
    add      \a2\wX, \a2\wX, \t2\wX
    sqrdmulh \b6\wX, \b6\wX, \z6\nX[\l6]
    add      \a3\wX, \a3\wX, \t3\wX
    sqrdmulh \b7\wX, \b7\wX, \z7\nX[\l7]

    mls      \t4\wX, \b4\wX, \mod\nX[0]
    mls      \t5\wX, \b5\wX, \mod\nX[0]
    mls      \t6\wX, \b6\wX, \mod\nX[0]
    mls      \t7\wX, \b7\wX, \mod\nX[0]

.endm

.macro wrap_qX_butterfly_mixed_rev a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, a4, a5, a6, a7, b4, b5, b6, b7, t4, t5, t6, t7, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7, wX, nX

    mul      \t0\wX, \b0\wX, \z0\nX[\h0]
    sub      \b4\wX, \a4\wX, \t4\wX
    mul      \t1\wX, \b1\wX, \z1\nX[\h1]
    sub      \b5\wX, \a5\wX, \t5\wX
    mul      \t2\wX, \b2\wX, \z2\nX[\h2]
    sub      \b6\wX, \a6\wX, \t6\wX
    mul      \t3\wX, \b3\wX, \z3\nX[\h3]
    sub      \b7\wX, \a7\wX, \t7\wX

    sqrdmulh \b0\wX, \b0\wX, \z0\nX[\l0]
    add      \a4\wX, \a4\wX, \t4\wX
    sqrdmulh \b1\wX, \b1\wX, \z1\nX[\l1]
    add      \a5\wX, \a5\wX, \t5\wX
    sqrdmulh \b2\wX, \b2\wX, \z2\nX[\l2]
    add      \a6\wX, \a6\wX, \t6\wX
    sqrdmulh \b3\wX, \b3\wX, \z3\nX[\l3]
    add      \a7\wX, \a7\wX, \t7\wX

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]
    mls      \t2\wX, \b2\wX, \mod\nX[0]
    mls      \t3\wX, \b3\wX, \mod\nX[0]

.endm

// vector-vector butterflies

.macro wrap_dX_butterfly_vec_top a0, a1, b0, b1, t0, t1, mod, l0, h0, l1, h1, wX, nX

    mul      \t0\wX, \b0\wX, \h0\wX
    mul      \t1\wX, \b1\wX, \h1\wX

    sqrdmulh \b0\wX, \b0\wX, \l0\wX
    sqrdmulh \b1\wX, \b1\wX, \l1\wX

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]

.endm

.macro wrap_dX_butterfly_vec_bot a0, a1, b0, b1, t0, t1, mod, l0, h0, l1, h1, wX, nX

    sub     \b0\wX, \a0\wX, \t0\wX
    sub     \b1\wX, \a1\wX, \t1\wX

    add     \a0\wX, \a0\wX, \t0\wX
    add     \a1\wX, \a1\wX, \t1\wX

.endm

.macro wrap_dX_butterfly_vec_mixed a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, l0, h0, l1, h1, l2, h2, l3, h3, wX, nX

    sub      \b0\wX, \a0\wX, \t0\wX
    mul      \t2\wX, \b2\wX, \h2\wX
    sub      \b1\wX, \a1\wX, \t1\wX
    mul      \t3\wX, \b3\wX, \h3\wX

    add      \a0\wX, \a0\wX, \t0\wX
    sqrdmulh \b2\wX, \b2\wX, \l2\wX
    add      \a1\wX, \a1\wX, \t1\wX
    sqrdmulh \b3\wX, \b3\wX, \l3\wX

    mls      \t2\wX, \b2\wX, \mod\nX[0]
    mls      \t3\wX, \b3\wX, \mod\nX[0]

.endm

.macro wrap_dX_butterfly_vec_mixed_rev a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, l0, h0, l1, h1, l2, h2, l3, h3, wX, nX

    mul      \t0\wX, \b0\wX, \h0\wX
    sub      \b2\wX, \a2\wX, \t2\wX
    mul      \t1\wX, \b1\wX, \h1\wX
    sub      \b3\wX, \a3\wX, \t3\wX

    sqrdmulh \b0\wX, \b0\wX, \l0\wX
    add      \a2\wX, \a2\wX, \t2\wX
    sqrdmulh \b1\wX, \b1\wX, \l1\wX
    add      \a3\wX, \a3\wX, \t3\wX

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]

.endm

// vector-scalar Barrett reduction

.macro wrap_qX_barrett a0, a1, a2, a3, t0, t1, t2, t3, barrett_const, shrv, Q, wX, nX

    sqdmulh \t0\wX, \a0\wX, \barrett_const\nX[0]
    sqdmulh \t1\wX, \a1\wX, \barrett_const\nX[0]

    sqdmulh \t2\wX, \a2\wX, \barrett_const\nX[0]
    srshr   \t0\wX, \t0\wX, \shrv
    sqdmulh \t3\wX, \a3\wX, \barrett_const\nX[0]
    srshr   \t1\wX, \t1\wX, \shrv

    srshr   \t2\wX, \t2\wX, \shrv
    mls     \a0\wX, \t0\wX, \Q\wX
    srshr   \t3\wX, \t3\wX, \shrv
    mls     \a1\wX, \t1\wX, \Q\wX

    mls     \a2\wX, \t2\wX, \Q\wX
    mls     \a3\wX, \t3\wX, \Q\wX

.endm

.macro wrap_oX_barrett a0, a1, a2, a3, t0, t1, t2, t3, a4, a5, a6, a7, t4, t5, t6, t7, barrett_const, shrv, Q, wX, nX

    sqdmulh \t0\wX, \a0\wX, \barrett_const\nX[0]
    sqdmulh \t1\wX, \a1\wX, \barrett_const\nX[0]
    sqdmulh \t2\wX, \a2\wX, \barrett_const\nX[0]
    sqdmulh \t3\wX, \a3\wX, \barrett_const\nX[0]

    srshr   \t0\wX, \t0\wX, \shrv
    sqdmulh \t4\wX, \a4\wX, \barrett_const\nX[0]
    srshr   \t1\wX, \t1\wX, \shrv
    sqdmulh \t5\wX, \a5\wX, \barrett_const\nX[0]
    srshr   \t2\wX, \t2\wX, \shrv
    sqdmulh \t6\wX, \a6\wX, \barrett_const\nX[0]
    srshr   \t3\wX, \t3\wX, \shrv
    sqdmulh \t7\wX, \a7\wX, \barrett_const\nX[0]

    mls     \a0\wX, \t0\wX, \Q\wX
    srshr   \t4\wX, \t4\wX, \shrv
    mls     \a1\wX, \t1\wX, \Q\wX
    srshr   \t5\wX, \t5\wX, \shrv
    mls     \a2\wX, \t2\wX, \Q\wX
    srshr   \t6\wX, \t6\wX, \shrv
    mls     \a3\wX, \t3\wX, \Q\wX
    srshr   \t7\wX, \t7\wX, \shrv

    mls     \a4\wX, \t4\wX, \Q\wX
    mls     \a5\wX, \t5\wX, \Q\wX
    mls     \a6\wX, \t6\wX, \Q\wX
    mls     \a7\wX, \t7\wX, \Q\wX

.endm

// vector-vector Barrett reduction

.macro wrap_qo_barrett_vec a0, a1, a2, a3, t0, t1, t2, t3, barrett_const, shrv, Q, wX, nX

    sqdmulh \t0\wX, \a0\wX, \barrett_const\wX
    sqdmulh \t1\wX, \a1\wX, \barrett_const\wX

    sqdmulh \t2\wX, \a2\wX, \barrett_const\wX
    srshr   \t0\wX, \t0\wX, \shrv
    sqdmulh \t3\wX, \a3\wX, \barrett_const\wX
    srshr   \t1\wX, \t1\wX, \shrv

    srshr   \t2\wX, \t2\wX, \shrv
    mls     \a0\wX, \t0\wX, \Q\wX
    srshr   \t3\wX, \t3\wX, \shrv
    mls     \a1\wX, \t1\wX, \Q\wX

    mls     \a2\wX, \t2\wX, \Q\wX
    mls     \a3\wX, \t3\wX, \Q\wX

.endm

.macro wrap_oo_barrett_vec a0, a1, a2, a3, t0, t1, t2, t3, a4, a5, a6, a7, t4, t5, t6, t7, barrett_const, shrv, Q, wX, nX

    sqdmulh \t0\wX, \a0\wX, \barrett_const\wX
    sqdmulh \t1\wX, \a1\wX, \barrett_const\wX
    sqdmulh \t2\wX, \a2\wX, \barrett_const\wX
    sqdmulh \t3\wX, \a3\wX, \barrett_const\wX

    srshr   \t0\wX, \t0\wX, \shrv
    sqdmulh \t4\wX, \a4\wX, \barrett_const\wX
    srshr   \t1\wX, \t1\wX, \shrv
    sqdmulh \t5\wX, \a5\wX, \barrett_const\wX
    srshr   \t2\wX, \t2\wX, \shrv
    sqdmulh \t6\wX, \a6\wX, \barrett_const\wX
    srshr   \t3\wX, \t3\wX, \shrv
    sqdmulh \t7\wX, \a7\wX, \barrett_const\wX

    mls     \a0\wX, \t0\wX, \Q\wX
    srshr   \t4\wX, \t4\wX, \shrv
    mls     \a1\wX, \t1\wX, \Q\wX
    srshr   \t5\wX, \t5\wX, \shrv
    mls     \a2\wX, \t2\wX, \Q\wX
    srshr   \t6\wX, \t6\wX, \shrv
    mls     \a3\wX, \t3\wX, \Q\wX
    srshr   \t7\wX, \t7\wX, \shrv

    mls     \a4\wX, \t4\wX, \Q\wX
    mls     \a5\wX, \t5\wX, \Q\wX
    mls     \a6\wX, \t6\wX, \Q\wX
    mls     \a7\wX, \t7\wX, \Q\wX

.endm

// Montgomery multiplication

.macro wrap_qX_montgomery_mul b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, wX, nX

    mul      \b0\wX, \t0\wX, \z0\nX[\h0]
    mul      \b1\wX, \t1\wX, \z1\nX[\h1]
    mul      \b2\wX, \t2\wX, \z2\nX[\h2]
    mul      \b3\wX, \t3\wX, \z3\nX[\h3]

    sqrdmulh \t0\wX, \t0\wX, \z0\nX[\l0]
    sqrdmulh \t1\wX, \t1\wX, \z1\nX[\l1]
    sqrdmulh \t2\wX, \t2\wX, \z2\nX[\l2]
    sqrdmulh \t3\wX, \t3\wX, \z3\nX[\l3]

    mls      \b0\wX, \t0\wX, \mod\nX[0]
    mls      \b1\wX, \t1\wX, \mod\nX[0]
    mls      \b2\wX, \t2\wX, \mod\nX[0]
    mls      \b3\wX, \t3\wX, \mod\nX[0]

.endm

// Montgomery reduction with long

.macro wrap_qX_montgomery c0, c1, c2, c3, l0, l1, l2, l3, h0, h1, h2, h3, t0, t1, t2, t3, Qprime, Q, lX, wX, dwX

    uzp1 \t0\wX, \l0\wX, \h0\wX
    uzp1 \t1\wX, \l1\wX, \h1\wX
    uzp1 \t2\wX, \l2\wX, \h2\wX
    uzp1 \t3\wX, \l3\wX, \h3\wX

    mul \t0\wX, \t0\wX, \Qprime\wX
    mul \t1\wX, \t1\wX, \Qprime\wX
    mul \t2\wX, \t2\wX, \Qprime\wX
    mul \t3\wX, \t3\wX, \Qprime\wX

    smlal  \l0\dwX, \t0\lX, \Q\lX
    smlal2 \h0\dwX, \t0\wX, \Q\wX
    smlal  \l1\dwX, \t1\lX, \Q\lX
    smlal2 \h1\dwX, \t1\wX, \Q\wX
    smlal  \l2\dwX, \t2\lX, \Q\lX
    smlal2 \h2\dwX, \t2\wX, \Q\wX
    smlal  \l3\dwX, \t3\lX, \Q\lX
    smlal2 \h3\dwX, \t3\wX, \Q\wX

    uzp2 \c0\wX, \l0\wX, \h0\wX
    uzp2 \c1\wX, \l1\wX, \h1\wX
    uzp2 \c2\wX, \l2\wX, \h2\wX
    uzp2 \c3\wX, \l3\wX, \h3\wX

.endm

// add_sub, sub_add

.macro wrap_qX_add_sub s0, s1, s2, s3, t0, t1, t2, t3, a0, a1, a2, a3, b0, b1, b2, b3, wX

    add \s0\wX, \a0\wX, \b0\wX
    sub \t0\wX, \a0\wX, \b0\wX
    add \s1\wX, \a1\wX, \b1\wX
    sub \t1\wX, \a1\wX, \b1\wX
    add \s2\wX, \a2\wX, \b2\wX
    sub \t2\wX, \a2\wX, \b2\wX
    add \s3\wX, \a3\wX, \b3\wX
    sub \t3\wX, \a3\wX, \b3\wX

.endm

.macro wrap_qX_sub_add s0, s1, s2, s3, t0, t1, t2, t3, a0, a1, a2, a3, b0, b1, b2, b3, wX

    sub \t0\wX, \a0\wX, \b0\wX
    add \s0\wX, \a0\wX, \b0\wX
    sub \t1\wX, \a1\wX, \b1\wX
    add \s1\wX, \a1\wX, \b1\wX
    sub \t2\wX, \a2\wX, \b2\wX
    add \s2\wX, \a2\wX, \b2\wX
    sub \t3\wX, \a3\wX, \b3\wX
    add \s3\wX, \a3\wX, \b3\wX

.endm


#endif




