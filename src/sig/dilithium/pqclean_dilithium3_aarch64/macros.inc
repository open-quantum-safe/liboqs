#include "macros_common.inc"

.macro wrap_trn_4x4 a0, a1, a2, a3, t0, t1, t2, t3, qS, dD

    trn1 \t0\qS, \a0\qS, \a1\qS
    trn2 \t1\qS, \a0\qS, \a1\qS
    trn1 \t2\qS, \a2\qS, \a3\qS
    trn2 \t3\qS, \a2\qS, \a3\qS

    trn1 \a0\dD, \t0\dD, \t2\dD
    trn2 \a2\dD, \t0\dD, \t2\dD
    trn1 \a1\dD, \t1\dD, \t3\dD
    trn2 \a3\dD, \t1\dD, \t3\dD

.endm

.macro trn_4x4 a0, a1, a2, a3, t0, t1, t2, t3
    wrap_trn_4x4 \a0, \a1, \a2, \a3, \t0, \t1, \t2, \t3, .4S, .2D
.endm


.macro dq_butterfly_vec_bot a0, a1, b0, b1, t0, t1, mod, l0, h0, l1, h1
    wrap_dX_butterfly_vec_bot \a0, \a1, \b0, \b1, \t0, \t1, \mod, \l0, \h0, \l1, \h1, .4S, .S
.endm

.macro dq_butterfly_vec_top a0, a1, b0, b1, t0, t1, mod, l0, h0, l1, h1
    wrap_dX_butterfly_vec_top \a0, \a1, \b0, \b1, \t0, \t1, \mod, \l0, \h0, \l1, \h1, .4S, .S
.endm

.macro dq_butterfly_vec_mixed a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, l0, h0, l1, h1, l2, h2, l3, h3
    wrap_dX_butterfly_vec_mixed \a0, \a1, \b0, \b1, \t0, \t1, \a2, \a3, \b2, \b3, \t2, \t3, \mod, \l0, \h0, \l1, \h1, \l2, \h2, \l3, \h3, .4S, .S
.endm

.macro dq_butterfly_vec_mixed_rev a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, l0, h0, l1, h1, l2, h2, l3, h3
    wrap_dX_butterfly_vec_mixed_rev \a0, \a1, \b0, \b1, \t0, \t1, \a2, \a3, \b2, \b3, \t2, \t3, \mod, \l0, \h0, \l1, \h1, \l2, \h2, \l3, \h3, .4S, .S
.endm


.macro dq_butterfly_top a0, a1, b0, b1, t0, t1, mod, z0, l0, h0, z1, l1, h1
    wrap_dX_butterfly_top \a0, \a1, \b0, \b1, \t0, \t1, \mod, \z0, \l0, \h0, \z1, \l1, \h1, .4S, .S
.endm

.macro dq_butterfly_bot a0, a1, b0, b1, t0, t1, mod, z0, l0, h0, z1, l1, h1
    wrap_dX_butterfly_bot \a0, \a1, \b0, \b1, \t0, \t1, \mod, \z0, \l0, \h0, \z1, \l1, \h1, .4S, .S
.endm

.macro dq_butterfly_mixed a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3
    wrap_dX_butterfly_mixed \a0, \a1, \b0, \b1, \t0, \t1, \a2, \a3, \b2, \b3, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .4S, .S
.endm

.macro dq_butterfly_mixed_rev a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3
    wrap_dX_butterfly_mixed_rev \a0, \a1, \b0, \b1, \t0, \t1, \a2, \a3, \b2, \b3, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .4S, .S
.endm


.macro qq_montgomery_mul b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3
    wrap_qX_montgomery_mul \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .4S, .S
.endm


.macro qq_butterfly_top a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3
    wrap_qX_butterfly_top \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .4S, .S
.endm

.macro qq_butterfly_bot a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3
    wrap_qX_butterfly_bot \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .4S, .S
.endm

.macro qq_butterfly_mixed a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, a4, a5, a6, a7, b4, b5, b6, b7, t4, t5, t6, t7, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7
    wrap_qX_butterfly_mixed \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \a4, \a5, \a6, \a7, \b4, \b5, \b6, \b7, \t4, \t5, \t6, \t7, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, \z4, \l4, \h4, \z5, \l5, \h5, \z6, \l6, \h6, \z7, \l7, \h7, .4S, .S
.endm

.macro qq_butterfly_mixed_rev a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, a4, a5, a6, a7, b4, b5, b6, b7, t4, t5, t6, t7, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7
    wrap_qX_butterfly_mixed_rev \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \a4, \a5, \a6, \a7, \b4, \b5, \b6, \b7, \t4, \t5, \t6, \t7, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, \z4, \l4, \h4, \z5, \l5, \h5, \z6, \l6, \h6, \z7, \l7, \h7, .4S, .S
.endm


.macro qq_montgomery c0, c1, c2, c3, l0, l1, l2, l3, h0, h1, h2, h3, t0, t1, t2, t3, Qprime, Q
    wrap_qX_montgomery \c0, \c1, \c2, \c3, \l0, \l1, \l2, \l3, \h0, \h1, \h2, \h3, \t0, \t1, \t2, \t3, \Qprime, \Q, .2S, .4S, .2D
.endm

.macro qq_sub_add s0, s1, s2, s3, t0, t1, t2, t3, a0, a1, a2, a3, b0, b1, b2, b3
    wrap_qX_sub_add \s0, \s1, \s2, \s3, \t0, \t1, \t2, \t3, \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, .4S
.endm
