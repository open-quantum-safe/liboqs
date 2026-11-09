#ifndef BIEXT_H
#define BIEXT_H

#include <sqisign_namespace.h>
#include <ec.h>

/**
 * @ingroup ec
 * @defgroup pairing Pairing on eliptic curves
 * @{
 */

typedef struct pairing_params
{
    uint32_t e;     // Points have order 2^e
    ec_point_t P;   // x(P)
    ec_point_t Q;   // x(Q)
    ec_point_t PQ;  // x(P-Q) = (PQX/PQZ : 1)
    fp2_t ixP;      // PZ/PX
    fp2_t ixQ;      // QZ/QX
    ec_point_t A24; // ((A+2)/4 : 1)
} pairing_params_t;

// For two bases <P, Q> and <R, S> store:
// x(P - R), x(P - S), x(R - Q), x(S - Q)
typedef struct pairing_dlog_diff_points
{
    ec_point_t PmR; // x(P - R)
    ec_point_t PmS; // x(P - S)
    ec_point_t RmQ; // x(R - Q)
    ec_point_t SmQ; // x(S - Q)
} pairing_dlog_diff_points_t;

typedef struct pairing_dlog_params
{
    uint32_t e;                      // Points have order 2^e
    ec_basis_t PQ;                   // x(P), x(Q), x(P-Q)
    ec_basis_t RS;                   // x(R), x(S), x(R-S)
    pairing_dlog_diff_points_t diff; // x(P - R), x(P - S), x(R - Q), x(S - Q)
    fp2_t ixP;                       // PZ/PX
    fp2_t ixQ;                       // QZ/QX
    fp2_t ixR;                       // RZ/RX
    fp2_t ixS;                       // SZ/SX
    ec_point_t A24;                  // ((A+2)/4 : 1)
} pairing_dlog_params_t;

/**
 * @brief compute the reduced Tate pairing of (P, Q), over 2^e.
 *
 * @param r output: the computed pairing
 * @param e output: the power of 2 of the
 * @param P the first point
 * @param Q the second point
 * @param PQ the difference between the 2 points
 * @param E the curve
 */
void pairing_reduced_tate(fp2_t *r,
                          uint32_t e,
                          const ec_point_t *P,
                          const ec_point_t *Q,
                          const ec_point_t *PQ,
                          ec_curve_t *E);

/**
 * @brief Given two bases <P, Q> and <R, S> where <P, Q> is a basis for E[2^f] e full 2-torsion, and <R,S> a basis for
 * smaller torsion E[2^e] computes scalars r1, r2, s1, s2 such that R = [r1]P + [r2]Q, S = [s1]P + [s2]Q
 *
 * @param r1 output: first coeff such that R = [r1]P + [r2]Q
 * @param r2 output: second coeff such that R = [r1]P + [r2]Q
 * @param s1 output: first coeff such that S = [s1]P + [s2]Q
 * @param s2 output: second coeff such that S = [s1]P + [s2]Q
 * @param PQ the first basis
 * @param RS the second input
 * @param curve the curve all the points are defined over
 * @param e the power of 2 such that <P,Q> = <R,S> = E[2^e]
 *
 */
void pairing_dlog_2_tate(digit_t *r1,
                         digit_t *r2,
                         digit_t *s1,
                         digit_t *s2,
                         const ec_basis_t *PQ,
                         const ec_basis_t *RS,
                         ec_curve_t *curve,
                         int e);

/**
 * @}
 */

#endif
