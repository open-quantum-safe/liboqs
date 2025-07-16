#ifndef _BIEXT_H_
#define _BIEXT_H_

#include <sqisign_namespace.h>
#include <ec.h>

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

// Computes e = e_{2^e}(P, Q) using biextension ladder
void weil(fp2_t *r, uint32_t e, const ec_point_t *P, const ec_point_t *Q, const ec_point_t *PQ, ec_curve_t *E);

// Computes (reduced) z = t_{2^e}(P, Q) using biextension ladder
void reduced_tate(fp2_t *r, uint32_t e, const ec_point_t *P, const ec_point_t *Q, const ec_point_t *PQ, ec_curve_t *E);

// Given two bases <P, Q> and <R, S> computes scalars
// such that R = [r1]P + [r2]Q, S = [s1]P + [s2]Q
void ec_dlog_2_weil(digit_t *r1,
                    digit_t *r2,
                    digit_t *s1,
                    digit_t *s2,
                    ec_basis_t *PQ,
                    const ec_basis_t *RS,
                    ec_curve_t *curve,
                    int e);

// Given two bases <P, Q> and <R, S>
// where <P, Q> is a basis for E[2^f]
// the full 2-torsion, and <R,S> a basis
// for smaller torsion E[2^e]
// computes scalars r1, r2, s1, s2
// such that R = [r1]P + [r2]Q, S = [s1]P + [s2]Q
void ec_dlog_2_tate(digit_t *r1,
                    digit_t *r2,
                    digit_t *s1,
                    digit_t *s2,
                    const ec_basis_t *PQ,
                    const ec_basis_t *RS,
                    ec_curve_t *curve,
                    int e);

void ec_dlog_2_tate_to_full(digit_t *r1,
                            digit_t *r2,
                            digit_t *s1,
                            digit_t *s2,
                            ec_basis_t *PQ,
                            ec_basis_t *RS,
                            ec_curve_t *curve,
                            int e);

#endif
