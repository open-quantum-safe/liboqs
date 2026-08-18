
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

#ifndef MACROS_S
#define MACROS_S

#include "macros_common.inc"

.macro qo_barrett a0, a1, a2, a3, t0, t1, t2, t3, barrett_const, shrv, Q
    wrap_qX_barrett \a0, \a1, \a2, \a3, \t0, \t1, \t2, \t3, \barrett_const, \shrv, \Q, .8H, .H
.endm

.macro oo_barrett a0, a1, a2, a3, t0, t1, t2, t3, a4, a5, a6, a7, t4, t5, t6, t7, barrett_const, shrv, Q
    wrap_oX_barrett \a0, \a1, \a2, \a3, \t0, \t1, \t2, \t3, \a4, \a5, \a6, \a7, \t4, \t5, \t6, \t7, \barrett_const, \shrv, \Q, .8H, .H
.endm


.macro qo_barrett_vec a0, a1, a2, a3, t0, t1, t2, t3, barrett_const, shrv, Q
    wrap_qo_barrett_vec \a0, \a1, \a2, \a3, \t0, \t1, \t2, \t3, \barrett_const, \shrv, \Q, .8H, .H
.endm

.macro oo_barrett_vec a0, a1, a2, a3, t0, t1, t2, t3, a4, a5, a6, a7, t4, t5, t6, t7, barrett_const, shrv, Q
    wrap_oo_barrett_vec \a0, \a1, \a2, \a3, \t0, \t1, \t2, \t3, \a4, \a5, \a6, \a7, \t4, \t5, \t6, \t7, \barrett_const, \shrv, \Q, .8H, .H
.endm


.macro qo_butterfly_top a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3
    wrap_qX_butterfly_top \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .8H, .H
.endm

.macro qo_butterfly_bot a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3
    wrap_qX_butterfly_bot \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .8H, .H
.endm

.macro qo_butterfly_mixed a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, a4, a5, a6, a7, b4, b5, b6, b7, t4, t5, t6, t7, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7
    wrap_qX_butterfly_mixed \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \a4, \a5, \a6, \a7, \b4, \b5, \b6, \b7, \t4, \t5, \t6, \t7, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, \z4, \l4, \h4, \z5, \l5, \h5, \z6, \l6, \h6, \z7, \l7, \h7, .8H, .H
.endm

.macro qo_butterfly_mixed_rev a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, a4, a5, a6, a7, b4, b5, b6, b7, t4, t5, t6, t7, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7
    wrap_qX_butterfly_mixed_rev \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \a4, \a5, \a6, \a7, \b4, \b5, \b6, \b7, \t4, \t5, \t6, \t7, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, \z4, \l4, \h4, \z5, \l5, \h5, \z6, \l6, \h6, \z7, \l7, \h7, .8H, .H
.endm


.macro do_butterfly_vec_top a0, a1, b0, b1, t0, t1, mod, l0, h0, l1, h1
    wrap_dX_butterfly_vec_top \a0, \a1, \b0, \b1, \t0, \t1, \mod, \l0, \h0, \l1, \h1, .8H, .H
.endm

.macro do_butterfly_vec_bot a0, a1, b0, b1, t0, t1, mod, l0, h0, l1, h1
    wrap_dX_butterfly_vec_bot \a0, \a1, \b0, \b1, \t0, \t1, \mod, \l0, \h0, \l1, \h1, .8H, .H
.endm

.macro do_butterfly_vec_mixed a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, l0, h0, l1, h1, l2, h2, l3, h3
    wrap_dX_butterfly_vec_mixed \a0, \a1, \b0, \b1, \t0, \t1, \a2, \a3, \b2, \b3, \t2, \t3, \mod, \l0, \h0, \l1, \h1, \l2, \h2, \l3, \h3, .8H, .H
.endm

.macro do_butterfly_vec_mixed_rev a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, l0, h0, l1, h1, l2, h2, l3, h3
    wrap_dX_butterfly_vec_mixed_rev \a0, \a1, \b0, \b1, \t0, \t1, \a2, \a3, \b2, \b3, \t2, \t3, \mod, \l0, \h0, \l1, \h1, \l2, \h2, \l3, \h3, .8H, .H
.endm


#endif

