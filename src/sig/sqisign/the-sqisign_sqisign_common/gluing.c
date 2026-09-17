#include "gluing.h"
#include <assert.h>

#include <ct_testing.h>

// Returns 1 if the basis is as expected and 0 otherwise
// We only expect this to fail for malformed signatures, so
// do not require this to run in constant time.
static int
verify_two_torsion(const theta_couple_point_t *K1_2, const theta_couple_point_t *K2_2, const theta_couple_curve_t *E12)
{
    // First check if all points in K1_2 or K2_2 are exactly two torsion
    // if they are not then the points did not have order 8 when we started gluing
    if (!ec_is_two_torsion(&K1_2->P1, &E12->E1) || !ec_is_two_torsion(&K1_2->P2, &E12->E2) ||
        !ec_is_two_torsion(&K2_2->P1, &E12->E1) || !ec_is_two_torsion(&K2_2->P2, &E12->E2)) {
        debug_print("Not all kernel points are of the right torsion");
        return 0;
    }

    // Now ensure that P1, Q1 and P2, Q2 are independent. For points of order two this means that they're not the same
    if (ec_is_equal(&K1_2->P1, &K2_2->P1) || ec_is_equal(&K1_2->P2, &K2_2->P2)) {
        debug_print("Not all kernel points are independant");
        return 0;
    }

    return 1;
}

// xDBLADD function specialised to the gluing. Uses the fact that both A24 and P are normalised
// Input:  projective Montgomery points P=(XP:ZP) and Q=(XQ:ZQ) such that xP=XP/ZP and xQ=XQ/ZQ, the difference
//         PQ=P-Q=(XPQ:1), and the Montgomery curve constants A24 = (A+2C/4C:1).
// Output: projective Montgomery points R <- 2*P = (XR:ZR) such that x(2P)=XR/ZR, and S <- P+Q = (XS:ZS) such that =
//         x(Q+P)=XS/ZS.
static void
gluing_special_xDBLADD(ec_point_t *R,
                       ec_point_t *S,
                       const ec_point_t *P,
                       const ec_point_t *Q,
                       const ec_point_t *PQ,
                       const ec_point_t *A24)
{
    fp2_t t0, t1, t2;

    fp2_add(&t0, &P->x, &P->z);
    fp2_sub(&t1, &P->x, &P->z);
    fp2_sqr(&R->x, &t0);
    fp2_sub(&t2, &Q->x, &Q->z);
    fp2_add(&S->x, &Q->x, &Q->z);
    fp2_mul(&t0, &t0, &t2);
    fp2_sqr(&R->z, &t1);
    fp2_mul(&t1, &t1, &S->x);
    fp2_sub(&t2, &R->x, &R->z);
    fp2_mul(&R->x, &R->x, &R->z);
    fp2_mul(&S->x, &A24->x, &t2);
    fp2_sub(&S->z, &t0, &t1);
    fp2_add(&R->z, &R->z, &S->x);
    fp2_add(&S->x, &t0, &t1);
    fp2_mul(&R->z, &R->z, &t2);
    fp2_sqr(&S->z, &S->z);
    fp2_sqr(&S->x, &S->x);
    fp2_mul(&S->z, &S->z, &PQ->x);
}

// batch normalize ec points
// assume no one is 0 (i.e.z = 0).
static void
batch_ec_normalise_points(ec_point_t *Pts, uint8_t len)
{
    fp2_t mem[len];
    uint8_t i;
    for (i = 0; i < len; i++) {
        fp2_copy(&mem[i], &Pts[i].z);
    }
    fp2_batched_inv(mem, len);
    for (i = 0; i < len; i++) {
        fp2_mul(&Pts[i].x, &Pts[i].x, &mem[i]);
        fp2_set_one(&Pts[i].z);
    }
}

void
gluing_ladder(ec_point_t *Reven,
              ec_point_t *Rodd,
              const ec_point_t *P,
              const ec_point_t *Q,
              const ec_point_t *PmQ,
              const int pow2,
              const ec_curve_t *curve)
{ // Montgomery ladder adapted to compute a ladder used during gluing.
  // Input: projective Montgomery point P, Q, PmQ, pow2 a power of 2, and
  //        the Montgomery curve constants (A:C) (or A24 = (A+2C/4C:1) if normalized).
  // Output: projective Montgomery points Rodd <- [2^pow2-1]*P  and Reven <- [2^pow2]*P.
    ec_point_t R0, R1;

    // assume the curve is normalised
    assert(curve->is_A24_computed_and_normalized);

    // R0 <- (1:0), R1 <- P
    fp2_copy(&R1.x, &P->x);
    fp2_copy(&R1.z, &P->z);
    fp2_copy(&R0.x, &Q->x);
    fp2_copy(&R0.z, &Q->z);

    // Main loop
    for (int i = pow2; i > 0; i--) {
        gluing_special_xDBLADD(&R1, &R0, &R1, &R0, PmQ, &curve->A24);
    }

    fp2_copy(&Reven->x, &R1.x);
    fp2_copy(&Reven->z, &R1.z);
    fp2_copy(&Rodd->x, &R0.x);
    fp2_copy(&Rodd->z, &R0.z);
}

/**
 * @brief Given in=(P1,P2) such that P1-P2 = diff and pow2, compute out= ([2^pow2]P1,[2^pow2]P1 - P2). This is a
 * technical function used to compute gluing without using the xz-coordinates.
 *
 * @param out Output: the lifted points
 * @param in: the input points
 * @param diff : the difference between points in the input-
 * @param pow2 the desired power of 2
 * @param E an elliptic curve. We assume that A24 is computed and normalised.
 */
static void
gluing_double_kernel_couple_points(theta_couple_point_t *out,
                                   const theta_couple_point_t *in,
                                   const ec_point_t *diff,
                                   const int pow2,
                                   const ec_curve_t *E)
{
    gluing_ladder(&out->P1, &out->P2, &in->P1, &in->P2, diff, pow2, E);
}

// Apply the change of theta coordinates to a point P.
static void
gluing_apply_change_coord(theta_point_t *res,
                          const gluing_change_coord_matrix_t *M,
                          const theta_point_t *P,
                          const bool Pt_not_zero)
{
    fp2_t x1;
    theta_point_t temp;

    fp2_mul(&temp.x, &P->x, &M->m[0][0]);
    fp2_mul(&x1, &P->y, &M->m[0][1]);
    fp2_add(&temp.x, &temp.x, &x1);
    fp2_mul(&x1, &P->z, &M->m[0][2]);
    fp2_add(&temp.x, &temp.x, &x1);

    fp2_mul(&temp.y, &P->x, &M->m[1][0]);
    fp2_mul(&x1, &P->y, &M->m[1][1]);
    fp2_add(&temp.y, &temp.y, &x1);
    fp2_mul(&x1, &P->z, &M->m[1][2]);
    fp2_add(&temp.y, &temp.y, &x1);

    fp2_mul(&temp.z, &P->x, &M->m[2][0]);
    fp2_mul(&x1, &P->y, &M->m[2][1]);
    fp2_add(&temp.z, &temp.z, &x1);
    fp2_mul(&x1, &P->z, &M->m[2][2]);
    fp2_add(&temp.z, &temp.z, &x1);

    fp2_mul(&temp.t, &P->x, &M->m[3][0]);
    fp2_mul(&x1, &P->y, &M->m[3][1]);
    fp2_add(&temp.t, &temp.t, &x1);
    fp2_mul(&x1, &P->z, &M->m[3][2]);
    fp2_add(&temp.t, &temp.t, &x1);

    if (Pt_not_zero) {
        fp2_mul(&x1, &P->t, &M->m[0][3]);
        fp2_add(&temp.x, &temp.x, &x1);

        fp2_mul(&x1, &P->t, &M->m[1][3]);
        fp2_add(&temp.y, &temp.y, &x1);

        fp2_mul(&x1, &P->t, &M->m[2][3]);
        fp2_add(&temp.z, &temp.z, &x1);

        fp2_mul(&x1, &P->t, &M->m[3][3]);
        fp2_add(&temp.t, &temp.t, &x1);
    }

    fp2_copy(&res->x, &temp.x);
    fp2_copy(&res->y, &temp.y);
    fp2_copy(&res->z, &temp.z);
    fp2_copy(&res->t, &temp.t);
}

// compute the theta_point corresponding to the couple of point T on an elliptic product
void
gluing_couple_point_to_theta(theta_point_t *out, const gluing_change_coord_matrix_t *M, const theta_couple_point_t *T)
{
    theta_point_t null_point;

    // null_point = (a : b : c : d)
    // a = P1.x P2.x, b = P1.x P2.z, c = P1.z P2.x, d = P1.z P2.z
    fp2_mul(&null_point.x, &T->P1.x, &T->P2.x);
    fp2_mul(&null_point.y, &T->P1.x, &T->P2.z);
    fp2_mul(&null_point.z, &T->P2.x, &T->P1.z);
    fp2_mul(&null_point.t, &T->P1.z, &T->P2.z);

    // Apply the basis change
    gluing_apply_change_coord(out, M, &null_point, true);
}

// Branchless equality mask: all-ones iff a == b. The callers below select on the secret index
// pos; a short-circuit `pos == a || pos == b` may compile to a conditional jump on pos, so the
// selection masks are computed with pure bit arithmetic instead.
static inline uint32_t
ct_eq_mask(uint32_t a, uint32_t b)
{
    uint32_t d = a ^ b;
    return ((d | (0u - d)) >> 31) - 1u;
}

/**
 * @brief Given T1 and T2 a basis of E[4], compute the symetric ellements above T1, T2 and T1*T2 that can be used to
 * compute the theta structure induced by T1 and T2.
 *
 * @param basis Output: the symetric ellement basis of T1, T2
 * @param T1_4 a point in E[4]
 * @param T2_4 a point in E[4]
 *
 */
static void
compute_symmetric_element(gluing_symmetric_element_basis_t *basis, const ec_point_t *T1_4, const ec_point_t *T2_4)
{
    fp2_t r1, r2, s1, s2, tmp1, tmp2;
    uint32_t pos = 255u;

    // Test if T1_4 or T2_4 is canonical one
    fp2_add(&r1, &T1_4->x, &T1_4->z);
    fp2_sub(&s1, &T1_4->x, &T1_4->z);
    fp2_add(&r2, &T2_4->x, &T2_4->z);
    fp2_sub(&s2, &T2_4->x, &T2_4->z);

    pos ^= (pos ^ 0u) & fp2_is_zero(&s1);
    pos ^= (pos ^ 1u) & fp2_is_zero(&r1);
    pos ^= (pos ^ 2u) & fp2_is_zero(&s2);
    pos ^= (pos ^ 3u) & fp2_is_zero(&r2);

    // Test if T1_4 + T2_4 is canonical point
    fp2_mul(&s2, &r1, &s2);    // s2 =  (x1 + z1)(x2 - z2)
    fp2_mul_by_i(&r1, &s2, 1); // r1 = i(x1 + z1)(x2 - z2)
    fp2_mul(&r2, &r2, &s1);    // r2 =  (x1 - z1)(x2 + z2)

    fp2_add(&s1, &r1, &r2);
    fp2_sub(&s2, &r1, &r2);
    pos ^= (pos ^ 4u) & fp2_is_zero(&s1);
    pos ^= (pos ^ 5u) & fp2_is_zero(&s2);

    fp2_copy(&tmp1, &T1_4->x);
    fp2_copy(&tmp2, &T1_4->z);
    fp2_select(&tmp1, &tmp1, &T2_4->x, ct_eq_mask(pos >> 1, 0u)); // pos == 0 or 1
    fp2_select(&tmp2, &tmp2, &T2_4->z, ct_eq_mask(pos >> 1, 0u));
    fp2_sqr(&s1, &tmp1);
    fp2_sqr(&s2, &tmp2);
    fp2_sub(&basis->delta, &s1, &s2); // delta = x2^2 - z2^2
    fp2_add(&r1, &s1, &s2);           // r1 = x2^2 +z2^2
    fp2_mul(&r2, &tmp1, &tmp2);
    fp2_add(&r2, &r2, &r2); // r2 = 2x2z2

    // g1 = (-r1,-r2),(r2,r1)
    fp2_neg(&basis->g1.s00, &r1);
    fp2_copy(&basis->g1.s01, &r2);
    fp2_neg(&basis->g1.s10, &r2);

    // g2 = (0,-d),(-d,0)
    fp2_set_zero(&basis->g2.s00);
    fp2_neg(&basis->g2.s01, &basis->delta);
    // g2 = (0,d),(d,0)
    fp2_select(&basis->g2.s01, &basis->g2.s01, &basis->delta, -(pos & 1u));
    fp2_copy(&basis->g2.s10, &basis->g2.s01);

    // g1 = (-r1,-r2), (r2,r1); g2 = (0,-d),(-d,0)
    fp2_cswap(&basis->g1.s00, &basis->g2.s00, ct_eq_mask(pos >> 1, 0u)); // pos == 0 or 1
    fp2_cswap(&basis->g1.s01, &basis->g2.s01, ct_eq_mask(pos >> 1, 0u));
    fp2_cswap(&basis->g1.s10, &basis->g2.s10, ct_eq_mask(pos >> 1, 0u));

    // g3 = (ir2,ir1),(-ir1,-ir2)
    uint32_t flg1 = (ct_eq_mask(pos, 0u) | ct_eq_mask(pos, 3u) | ct_eq_mask(pos, 4u)) & 1u;
    // g3 = (-ir2,-ir1),(ir1,ir2)
    uint32_t flg2 = (ct_eq_mask(pos, 1u) | ct_eq_mask(pos, 2u) | ct_eq_mask(pos, 5u)) & 1u;
    fp2_mul_by_i(&basis->g3.s00, &r2, flg1);
    fp2_mul_by_i(&basis->g3.s01, &r1, flg2);
    fp2_mul_by_i(&basis->g3.s10, &r1, flg1);

    // g2 = (-ir2,-ir1),(ir1,ir2); g3 = (0,-d),(-d,0)
    fp2_cswap(&basis->g2.s00, &basis->g3.s00, ct_eq_mask(pos >> 1, 2u)); // pos == 4 or 5
    fp2_cswap(&basis->g2.s01, &basis->g3.s01, ct_eq_mask(pos >> 1, 2u));
    fp2_cswap(&basis->g2.s10, &basis->g3.s10, ct_eq_mask(pos >> 1, 2u));
}

// Given the appropriate four torsion, computes the change of basis to compute the correct theta null point.
void
gluing_basis_compute(gluing_change_coord_matrix_t *M,
                     const theta_couple_point_t *K1_4,
                     const theta_couple_point_t *K2_4)
{
    // Compute the four 2x2 matrices for the action by translation on the four points:
    gluing_symmetric_element_basis_t Basis_1, Basis_2;
    fp2_t delta12, tmp1;

    compute_symmetric_element(&Basis_1, &K1_4->P1, &K2_4->P1);
    compute_symmetric_element(&Basis_2, &K1_4->P2, &K2_4->P2);

    fp2_mul(&delta12, &Basis_1.delta, &Basis_2.delta);

    // Compute the first row
    fp2_copy(&M->m[0][0], &delta12);
    fp2_mul(&tmp1, &Basis_1.g1.s00, &Basis_2.g1.s00);
    fp2_add(&M->m[0][0], &M->m[0][0], &tmp1);
    fp2_mul(&tmp1, &Basis_1.g2.s00, &Basis_2.g2.s00);
    fp2_add(&M->m[0][0], &M->m[0][0], &tmp1);
    fp2_mul(&tmp1, &Basis_1.g3.s00, &Basis_2.g3.s00);
    fp2_sub(&M->m[0][0], &M->m[0][0], &tmp1);

    fp2_mul(&M->m[0][1], &Basis_1.g1.s00, &Basis_2.g1.s01);
    fp2_mul(&tmp1, &Basis_1.g2.s00, &Basis_2.g2.s01);
    fp2_add(&M->m[0][1], &M->m[0][1], &tmp1);
    fp2_mul(&tmp1, &Basis_1.g3.s00, &Basis_2.g3.s01);
    fp2_sub(&M->m[0][1], &M->m[0][1], &tmp1);

    fp2_mul(&M->m[0][2], &Basis_1.g1.s01, &Basis_2.g1.s00);
    fp2_mul(&tmp1, &Basis_1.g2.s01, &Basis_2.g2.s00);
    fp2_add(&M->m[0][2], &M->m[0][2], &tmp1);
    fp2_mul(&tmp1, &Basis_1.g3.s01, &Basis_2.g3.s00);
    fp2_sub(&M->m[0][2], &M->m[0][2], &tmp1);

    fp2_mul(&M->m[0][3], &Basis_1.g1.s01, &Basis_2.g1.s01);
    fp2_mul(&tmp1, &Basis_1.g2.s01, &Basis_2.g2.s01);
    fp2_add(&M->m[0][3], &M->m[0][3], &tmp1);
    fp2_mul(&tmp1, &Basis_1.g3.s01, &Basis_2.g3.s01);
    fp2_sub(&M->m[0][3], &M->m[0][3], &tmp1);

    // Compute the second row
    fp2_mul(&M->m[1][0], &Basis_2.g2.s00, &M->m[0][0]);
    fp2_mul(&tmp1, &Basis_2.g2.s10, &M->m[0][1]);
    fp2_add(&M->m[1][0], &M->m[1][0], &tmp1);

    fp2_mul(&M->m[1][1], &Basis_2.g2.s01, &M->m[0][0]);
    fp2_mul(&tmp1, &Basis_2.g2.s00, &M->m[0][1]);
    fp2_sub(&M->m[1][1], &M->m[1][1], &tmp1);

    fp2_mul(&M->m[1][2], &Basis_2.g2.s00, &M->m[0][2]);
    fp2_mul(&tmp1, &Basis_2.g2.s10, &M->m[0][3]);
    fp2_add(&M->m[1][2], &M->m[1][2], &tmp1);

    fp2_mul(&M->m[1][3], &Basis_2.g2.s01, &M->m[0][2]);
    fp2_mul(&tmp1, &Basis_2.g2.s00, &M->m[0][3]);
    fp2_sub(&M->m[1][3], &M->m[1][3], &tmp1);

    // Compute the third row
    fp2_mul(&M->m[2][0], &Basis_1.g1.s00, &M->m[0][0]);
    fp2_mul(&tmp1, &Basis_1.g1.s10, &M->m[0][2]);
    fp2_add(&M->m[2][0], &M->m[2][0], &tmp1);

    fp2_mul(&M->m[2][1], &Basis_1.g1.s00, &M->m[0][1]);
    fp2_mul(&tmp1, &Basis_1.g1.s10, &M->m[0][3]);
    fp2_add(&M->m[2][1], &M->m[2][1], &tmp1);

    fp2_mul(&M->m[2][2], &Basis_1.g1.s01, &M->m[0][0]);
    fp2_mul(&tmp1, &Basis_1.g1.s00, &M->m[0][2]);
    fp2_sub(&M->m[2][2], &M->m[2][2], &tmp1);

    fp2_mul(&M->m[2][3], &Basis_1.g1.s01, &M->m[0][1]);
    fp2_mul(&tmp1, &Basis_1.g1.s00, &M->m[0][3]);
    fp2_sub(&M->m[2][3], &M->m[2][3], &tmp1);

    // Compute the last row
    fp2_mul(&M->m[3][0], &Basis_1.g1.s00, &M->m[1][0]);
    fp2_mul(&tmp1, &Basis_1.g1.s10, &M->m[1][2]);
    fp2_add(&M->m[3][0], &M->m[3][0], &tmp1);

    fp2_mul(&M->m[3][1], &Basis_1.g1.s00, &M->m[1][1]);
    fp2_mul(&tmp1, &Basis_1.g1.s10, &M->m[1][3]);
    fp2_add(&M->m[3][1], &M->m[3][1], &tmp1);

    fp2_mul(&M->m[3][2], &Basis_1.g1.s01, &M->m[1][0]);
    fp2_mul(&tmp1, &Basis_1.g1.s00, &M->m[1][2]);
    fp2_sub(&M->m[3][2], &M->m[3][2], &tmp1);

    fp2_mul(&M->m[3][3], &Basis_1.g1.s01, &M->m[1][1]);
    fp2_mul(&tmp1, &Basis_1.g1.s00, &M->m[1][3]);
    fp2_sub(&M->m[3][3], &M->m[3][3], &tmp1);

    // scalar realignement:
    for (uint8_t i = 0; i < 4; i++) {
        fp2_mul(&M->m[0][i], &M->m[0][i], &delta12);
        fp2_mul(&M->m[1][i], &M->m[1][i], &Basis_1.delta);
        fp2_mul(&M->m[2][i], &M->m[2][i], &Basis_2.delta);
    }
}

/**
 * @brief Compute the gluing isogeny from an elliptic product
 *
 * @param out Output: the theta_gluing
 * @param K1_8 a couple point
 * @param E12 an elliptic curve product
 * @param K2_8 a point in E2[8]
 *
 * out : E1xE2 -> A of kernel [4](K1_8,K2_8)
 * if the kernel supplied has the incorrect order, or gluing seems malformed, returns 0, otherwise returns 1.
 */
int
gluing_compute(theta_gluing_t *out,
               const theta_couple_curve_t *E12,
               const theta_couple_point_t *K1_8,
               const theta_couple_point_t *K2_8,
               const bool dual_codomain_bool,
               const bool verify)
{
    // Ensure that we have been given the eight torsion
#ifndef NDEBUG
    {
        int check = test_point_order_twof(&K1_8->P1, &E12->E1, 3);
        if (!check)
            debug_print("K1_8->P1 does not have order 8");
        check = test_point_order_twof(&K2_8->P1, &E12->E1, 3);
        if (!check)
            debug_print("K2_8->P1 does not have order 8");
        check = test_point_order_twof(&K1_8->P2, &E12->E2, 3);
        if (!check)
            debug_print("K1_8->P2 does not have order 8");
        check = test_point_order_twof(&K2_8->P2, &E12->E2, 3);
        if (!check)
            debug_print("K2_8->P2 does not have order 8");
    }
#endif

    // Given points in E[8] x E[8] we need the four torsion below
    theta_couple_point_t K1_4, K2_4;
    theta_couple_point_t K1_2, K2_2;

    theta_DBL_couple_point(&K1_4, K1_8, E12);
    theta_DBL_couple_point(&K2_4, K2_8, E12);
    theta_DBL_couple_point(&K1_2, &K1_4, E12);
    theta_DBL_couple_point(&K2_2, &K2_4, E12);

    if (verify) {
        if (!verify_two_torsion(&K1_2, &K2_2, E12)) {
            debug_print("gluing failed as kernel does not have correct order");
            return 0;
        }
    }
    gluing_basis_compute(&out->M, &K1_4, &K2_4);

    // apply the base change to the kernel
    theta_point_t TT1, TT2;

    gluing_couple_point_to_theta(&TT1, &out->M, K1_8);
    gluing_couple_point_to_theta(&TT2, &out->M, K2_8);

    // compute the codomain
    to_squared_theta(&TT1, &TT1);
    to_squared_theta(&TT2, &TT2);

    // If the kernel is well formed then TT1.t and TT2.t are zero
    // if they are not, we exit early as the signature we are validating is probably malformed
    uint32_t t_coords_zero = fp2_is_zero(&TT1.t) & fp2_is_zero(&TT2.t);
    // Validity check on secret-derived theta coordinates that never fails for honest inputs; only the pass/fail bit is
    // declassified, the coordinates stay tainted
    CT_TESTING_MAKE_PUBLIC(&t_coords_zero, sizeof(t_coords_zero));
    if (!t_coords_zero) {
        debug_print("gluing failed TT1.t or TT2.t is not zero");
        return 0;
    }
    // Test our projective factors are non zero
    uint32_t proj_factor_zero =
        fp2_is_zero(&TT1.x) | fp2_is_zero(&TT2.x) | fp2_is_zero(&TT1.y) | fp2_is_zero(&TT2.z) | fp2_is_zero(&TT1.z);
    // Same rationale as above
    CT_TESTING_MAKE_PUBLIC(&proj_factor_zero, sizeof(proj_factor_zero));
    if (proj_factor_zero) {
        debug_print("unexpected zeroes in gluing theta point.");
        return 0; // invalid input
    }

    // Projective factor: Ax
    fp2_mul(&out->codomain.x, &TT1.x, &TT2.x);
    fp2_mul(&out->codomain.y, &TT1.y, &TT2.x);
    fp2_mul(&out->codomain.z, &TT1.x, &TT2.z);
    fp2_set_zero(&out->codomain.t);
    // Projective factor: ABCxz
    fp2_mul(&out->inv_dual_theta_null.x, &TT1.y, &TT2.z);
    fp2_copy(&out->inv_dual_theta_null.y, &out->codomain.z);
    fp2_copy(&out->inv_dual_theta_null.z, &out->codomain.y);
    fp2_set_zero(&out->inv_dual_theta_null.t);

    // Compute the two components of phi(K1_8) = (x:x:y:y), so inv is (y:y:x:x).
    fp2_mul(&out->inv_imageK1_8.x, &TT1.z, &out->inv_dual_theta_null.z);
    fp2_mul(&out->inv_imageK1_8.z, &TT1.x, &out->inv_dual_theta_null.x);
    fp2_copy(&out->inv_imageK1_8.y, &out->inv_imageK1_8.x);
    fp2_copy(&out->inv_imageK1_8.t, &out->inv_imageK1_8.z);

    // Compute the two components of phi(K2_8) = (z:w:z:w), so inv is (w:z:w:z)
    fp2_mul(&out->inv_imageK2_8.x, &TT2.y, &out->inv_dual_theta_null.y);
    fp2_mul(&out->inv_imageK2_8.y, &TT2.x, &out->inv_dual_theta_null.x);
    fp2_copy(&out->inv_imageK2_8.z, &out->inv_imageK2_8.x);
    fp2_copy(&out->inv_imageK2_8.t, &out->inv_imageK2_8.y);

    // compute the final codomain
    if (!dual_codomain_bool)
        theta_hadamard(&out->codomain, &out->codomain);
    return 1;
}

/**
 * @brief Evaluate a gluing isogeny from an elliptic product on a basis
 *
 * @param image1 Output: the theta_point of the image of the first couple of points
 * @param image2 Output : the theta point of the image of the second couple of points
 * @param xyT1: A pair of points (X : Y : Z) on E1E2 to glue using phi
 * @param xyT2: A pair of points (X : Y : Z) on E1E2 to glue using phi
 * @param phi : a gluing isogeny E1 x E2 -> A
 *
 **/
void
gluing_eval_point_bary(theta_point_t *image,
                       const ec_bary_coordinates_t *add_comp1,
                       const ec_bary_coordinates_t *add_comp2,
                       const theta_gluing_t *phi,
                       const theta_point_t *aux_point_inv,
                       const bool dual_codomain_bool)
{
    theta_point_t T1, T2;

    // Compute T1 and T2 derived from the cross addition components.
    fp2_mul(&T1.x, &add_comp1->u, &add_comp2->u); // T1x = u1u2
    fp2_mul(&T2.t, &add_comp1->v, &add_comp2->v); // T2t = v1v2
    fp2_add(&T1.x, &T1.x, &T2.t);                 // T1x = u1u2 + v1v2
    fp2_mul(&T1.y, &add_comp1->u, &add_comp2->w); // T1y = u1w2
    fp2_mul(&T1.z, &add_comp1->w, &add_comp2->u); // T1z = w1u2
    fp2_mul(&T1.t, &add_comp1->w, &add_comp2->w); // T1t = w1w2
    fp2_add(&T2.x, &add_comp1->u, &add_comp1->v); // T2x = (u1+v1)
    fp2_add(&T2.y, &add_comp2->u, &add_comp2->v); // T2y = (u2+v2)
    fp2_mul(&T2.x, &T2.x, &T2.y);                 // T2x = (u1+v1)(u2+v2)
    fp2_sub(&T2.x, &T2.x, &T1.x);                 // T1x = v1u2 + u1v2
    fp2_mul(&T2.y, &add_comp1->v, &add_comp2->w); // T2y = v1w2
    fp2_mul(&T2.z, &add_comp1->w, &add_comp2->v); // T2z = w1v2
    fp2_set_zero(&T2.t);                          // T2t = 0

    // Apply the basis change and compute their respective square
    // theta(P+Q) = M.T1 - M.T2 and theta(P-Q) = M.T1 + M.T2
    gluing_apply_change_coord(&T1, &phi->M, &T1, true);
    gluing_apply_change_coord(&T2, &phi->M, &T2, false);
    pointwise_square(&T1, &T1);
    pointwise_square(&T2, &T2);

    // the difference between the two is therefore theta(P+Q)theta(P-Q) whose theta_hadamard transform is then the
    // product of the dual theta_points of phi(P) and phi(Q).
    fp2_sub(&T1.x, &T1.x, &T2.x);
    fp2_sub(&T1.y, &T1.y, &T2.y);
    fp2_sub(&T1.z, &T1.z, &T2.z);
    fp2_sub(&T1.t, &T1.t, &T2.t);
    theta_hadamard(&T1, &T1);

    // Compute (x, y, z, t)
    // As imageK1_8 = (x:x:y:y), its inverse is (y:y:x:x).
    fp2_mul(&image->x, &T1.x, &aux_point_inv->x);
    fp2_mul(&image->y, &T1.y, &aux_point_inv->y);
    fp2_mul(&image->z, &T1.z, &aux_point_inv->z);
    fp2_mul(&image->t, &T1.t, &aux_point_inv->t);

    if (!dual_codomain_bool)
        theta_hadamard(image, image);
}

// Same as gluing_eval_point but in the very special case where we already know that the point will have a zero
// coordinate at the place where the zero coordinate of the dual_theta_nullpoint would have made the computation
// difficult
int
gluing_eval_point_special_case(theta_point_t *image,
                               const theta_couple_point_t *P,
                               const theta_gluing_t *phi,
                               const bool dual_codomain_bool)
{
    theta_point_t T;

    // Apply the basis change
    gluing_couple_point_to_theta(&T, &phi->M, P);

    // Apply the to_squared_theta transform
    to_squared_theta(&T, &T);

    // This coordinate should always be 0 in a gluing because D=0. If this is not the case, something went very wrong,
    // so reject
    uint32_t t_coord_zero = fp2_is_zero(&T.t);
    // Validity check on secret-derived theta coordinates that never fails for honest inputs; only the pass/fail bit is
    // declassified, the coordinates stay tainted
    CT_TESTING_MAKE_PUBLIC(&t_coord_zero, sizeof(t_coord_zero));
    if (!t_coord_zero) {
        debug_print("evaluating at a couple_point with no zero coordinate");
        return 0;
    }
    // Compute (x, y, z, t)
    fp2_mul(&image->x, &T.x, &phi->inv_dual_theta_null.x);
    fp2_mul(&image->y, &T.y, &phi->inv_dual_theta_null.y);
    fp2_mul(&image->z, &T.z, &phi->inv_dual_theta_null.z);
    fp2_set_zero(&image->t);

    if (!dual_codomain_bool)
        theta_hadamard(image, image);
    return 1;
}

int
gluing_start_chain(theta_gluing_t *first_step,
                   theta_point_t *thetaQ1,
                   theta_point_t *thetaQ2,
                   theta_point_t *pts,
                   uint16_t *todo,
                   const uint8_t space,
                   const uint8_t numP,
                   const theta_couple_curve_t *E12,
                   const theta_couple_point_t *P12,
                   const theta_kernel_couple_points_t *ker,
                   const int8_t mode)
{
    int current = 0;

    // T_11, T_21 are both over E1, T_12, T_22 are over E2
    theta_couple_point_t T_11[space], T_12[space], T_21[space], T_22[space];

    // normalise points
    {
        ec_point_t ker_pts[4];
        ec_copy_point(&ker_pts[0], &ker->T1.P1);
        ec_copy_point(&ker_pts[1], &ker->T1.P2);
        ec_copy_point(&ker_pts[2], &ker->T2.P1);
        ec_copy_point(&ker_pts[3], &ker->T2.P2);
        batch_ec_normalise_points(ker_pts, 4);

        // init couple point to (P,0)
        ec_copy_point(&T_11[0].P1, &ker_pts[0]);
        ec_point_init(&T_11[0].P2);
        ec_copy_point(&T_12[0].P1, &ker_pts[1]);
        ec_point_init(&T_12[0].P2);
        ec_copy_point(&T_21[0].P1, &ker_pts[2]);
        ec_point_init(&T_21[0].P2);
        ec_copy_point(&T_22[0].P1, &ker_pts[3]);
        ec_point_init(&T_22[0].P2);
    }

    while (todo[current] != 1) {
        assert(todo[current] >= 2);
        ++current;
        assert(current < space);
        // the gluing isogeny is quite a bit more expensive than the others, so we adjust the usual splitting rule here
        // a little bit: towards the end of the doubling chain it will be cheaper to recompute the doublings after
        // evaluation than to push the intermediate points.
        const uint16_t num_dbls = todo[current - 1] >= 16 ? todo[current - 1] / 2 : todo[current - 1] - 1;
        assert(num_dbls && num_dbls < todo[current - 1]);
        gluing_double_kernel_couple_points(&T_11[current], &T_11[current - 1], &T_11[0].P1, num_dbls, &E12->E1);
        gluing_double_kernel_couple_points(&T_12[current], &T_12[current - 1], &T_12[0].P1, num_dbls, &E12->E2);
        gluing_double_kernel_couple_points(&T_21[current], &T_21[current - 1], &T_21[0].P1, num_dbls, &E12->E1);
        gluing_double_kernel_couple_points(&T_22[current], &T_22[current - 1], &T_22[0].P1, num_dbls, &E12->E2);

        todo[current] = todo[current - 1] - num_dbls;
    }

    // kernel points for the remaining isogeny steps
    {
        assert(todo[current] == 1);

        // compute the gluing isogeny
        theta_couple_point_t K1_8, K2_8;
        ec_copy_point(&K1_8.P1, &T_11[current].P1);
        ec_copy_point(&K1_8.P2, &T_12[current].P1);
        ec_copy_point(&K2_8.P1, &T_21[current].P1);
        ec_copy_point(&K2_8.P2, &T_22[current].P1);

        if (!gluing_compute(first_step, E12, &K1_8, &K2_8, 0, (mode == -1)))
            return 0;

        // evaluate special points
        for (int j = 0; j < numP; ++j) {
            assert(ec_is_zero(&P12[j].P1) || ec_is_zero(&P12[j].P2));
            if (!gluing_eval_point_special_case(&pts[j], &P12[j], first_step, 0))
                return 0;
        }

        // push kernel points through gluing isogeny
        ec_bary_coordinates_t add_comp1, add_comp2;

        // compute the first point
        ec_points_to_bary_coordinates(
            &add_comp1, &K1_8.P1, &T_11[0].P1, &T_11[current].P2); // [2^{f-3}]P, P, [2^{f-3}-1]P
        ec_points_to_bary_coordinates(
            &add_comp2, &K1_8.P2, &T_12[0].P1, &T_12[current].P2); // [2^{f-3}]Q, Q, [2^{f-3}-1]Q
        gluing_eval_point_bary(&thetaQ1[0], &add_comp1, &add_comp2, first_step, &first_step->inv_imageK1_8, 1);
        ec_points_to_bary_coordinates(
            &add_comp1, &K2_8.P1, &T_21[0].P1, &T_21[current].P2); // [2^{f-3}]P, P, [2^{f-3}-1]P
        ec_points_to_bary_coordinates(
            &add_comp2, &K2_8.P2, &T_22[0].P1, &T_22[current].P2); // [2^{f-3}]Q, Q, [2^{f-3}-1]Q
        gluing_eval_point_bary(&thetaQ2[0], &add_comp1, &add_comp2, first_step, &first_step->inv_imageK2_8, 1);
        --todo[0];
        // compute the inverse of both points

        theta_point_t inv_phiK1, inv_phiK2;
        theta_invert_point(&inv_phiK1, &thetaQ1[0]);
        theta_invert_point(&inv_phiK2, &thetaQ2[0]);
        theta_hadamard(&thetaQ1[0], &thetaQ1[0]);
        theta_hadamard(&thetaQ2[0], &thetaQ2[0]);

        for (int j = 1; j < current; ++j) {
            ec_points_to_bary_coordinates(&add_comp1, &T_11[j].P1, &T_11[0].P1, &T_11[j].P2); //[2^x]P, P, [2^x-1]P
            ec_points_to_bary_coordinates(&add_comp2, &T_12[j].P1, &T_12[0].P1, &T_12[j].P2); //[2^x]Q, Q, [2^x-1]Q
            gluing_eval_point_bary(&thetaQ1[j], &add_comp1, &add_comp2, first_step, &inv_phiK1, 0);
            ec_points_to_bary_coordinates(&add_comp1, &T_21[j].P1, &T_21[0].P1, &T_21[j].P2); //[2^x]P, P, [2^x-1]P
            ec_points_to_bary_coordinates(&add_comp2, &T_22[j].P1, &T_22[0].P1, &T_22[j].P2); //[2^x]Q, Q, [2^x-1]Q
            gluing_eval_point_bary(&thetaQ2[j], &add_comp1, &add_comp2, first_step, &inv_phiK2, 0);
            --todo[j];
        }

        --current;
        return current;
    }
}
