
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

