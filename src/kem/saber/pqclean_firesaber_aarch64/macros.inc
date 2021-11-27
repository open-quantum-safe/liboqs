
#ifndef MACROS_S
#define MACROS_S

#include "macros_common.inc"

.macro dq_butterfly_top a0, a1, b0, b1, t0, t1, mod, z0, l0, h0, z1, l1, h1
    wrap_dX_butterfly_top \a0, \a1, \b0, \b1, \t0, \t1, \mod, \z0, \l0, \h0, \z1, \l1, \h1, .4S, .S
.endm

.macro dq_butterfly_bot a0, a1, b0, b1, t0, t1, mod, z0, l0, h0, z1, l1, h1
    wrap_dX_butterfly_bot \a0, \a1, \b0, \b1, \t0, \t1, \mod, \z0, \l0, \h0, \z1, \l1, \h1, .4S, .S
.endm

.macro dq_butterfly_mixed a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3
    wrap_dX_butterfly_mixed \a0, \a1, \b0, \b1, \t0, \t1, \a2, \a3, \b2, \b3, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .4S, .S
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

.macro wrap_4x4_asymmetric mulacc, mulacc2, a0, b0, b1, b2, b3, l0, h0, l1, h1, l2, h2, l3, h3, dS, qS, dD

    \mulacc  \l0\dD, \a0\dS, \b0\dS
    \mulacc2 \h0\dD, \a0\qS, \b0\qS
    \mulacc  \l1\dD, \a0\dS, \b1\dS
    \mulacc2 \h1\dD, \a0\qS, \b1\qS
    \mulacc  \l2\dD, \a0\dS, \b2\dS
    \mulacc2 \h2\dD, \a0\qS, \b2\qS
    \mulacc  \l3\dD, \a0\dS, \b3\dS
    \mulacc2 \h3\dD, \a0\qS, \b3\qS

.endm

.macro _4x4_asymmetric mulacc, mulacc2, a0, b0, b1, b2, b3, l0, h0, l1, h1, l2, h2, l3, h3
    wrap_4x4_asymmetric \mulacc, \mulacc2, \a0, \b0, \b1, \b2, \b3, \l0, \h0, \l1, \h1, \l2, \h2, \l3, \h3, .2S, .4S, .2D
.endm

.macro qq_montgomery c0, c1, c2, c3, l0, l1, l2, l3, h0, h1, h2, h3, t0, t1, t2, t3, Qprime, Q
    wrap_qX_montgomery \c0, \c1, \c2, \c3, \l0, \l1, \l2, \l3, \h0, \h1, \h2, \h3, \t0, \t1, \t2, \t3, \Qprime, \Q, .2S, .4S, .2D
.endm

.macro qq_add_sub s0, s1, s2, s3, t0, t1, t2, t3, a0, a1, a2, a3, b0, b1, b2, b3
    wrap_qX_add_sub \s0, \s1, \s2, \s3, \t0, \t1, \t2, \t3, \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, .4S
.endm

#endif

