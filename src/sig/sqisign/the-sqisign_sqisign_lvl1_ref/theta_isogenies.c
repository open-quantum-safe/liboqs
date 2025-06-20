#include "theta_isogenies.h"
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <tools.h>
#include <rng.h>

// Select a base change matrix in constant time, with M1 a regular
// base change matrix and M2 a precomputed base change matrix
// If option = 0 then M <- M1, else if option = 0xFF...FF then M <- M2
static inline void
select_base_change_matrix(basis_change_matrix_t *M,
                          const basis_change_matrix_t *M1,
                          const precomp_basis_change_matrix_t *M2,
                          const uint32_t option)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            fp2_select(&M->m[i][j], &M1->m[i][j], &FP2_CONSTANTS[M2->m[i][j]], option);
}

// Set a regular base change matrix from a precomputed one
static inline void
set_base_change_matrix_from_precomp(basis_change_matrix_t *res, const precomp_basis_change_matrix_t *M)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            res->m[i][j] = FP2_CONSTANTS[M->m[i][j]];
}

static inline void
choose_index_theta_point(fp2_t *res, int ind, const theta_point_t *T)
{
    const fp2_t *src = NULL;
    switch (ind % 4) {
        case 0:
            src = &T->x;
            break;
        case 1:
            src = &T->y;
            break;
        case 2:
            src = &T->z;
            break;
        case 3:
            src = &T->t;
            break;
        default:
            assert(0);
    }
    fp2_copy(res, src);
}

// same as apply_isomorphism method but more efficient when the t component of P is zero.
static void
apply_isomorphism_general(theta_point_t *res,
                          const basis_change_matrix_t *M,
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

static void
apply_isomorphism(theta_point_t *res, const basis_change_matrix_t *M, const theta_point_t *P)
{
    apply_isomorphism_general(res, M, P, true);
}

// set res = M1 * M2 with matrix multiplication
static void
base_change_matrix_multiplication(basis_change_matrix_t *res,
                                  const basis_change_matrix_t *M1,
                                  const basis_change_matrix_t *M2)
{
    basis_change_matrix_t tmp;
    fp2_t sum, m_ik, m_kj;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fp2_set_zero(&sum);
            for (int k = 0; k < 4; k++) {
                m_ik = M1->m[i][k];
                m_kj = M2->m[k][j];
                fp2_mul(&m_ik, &m_ik, &m_kj);
                fp2_add(&sum, &sum, &m_ik);
            }
            tmp.m[i][j] = sum;
        }
    }
    *res = tmp;
}

// compute the theta_point corresponding to the couple of point T on an elliptic product
static void
base_change(theta_point_t *out, const theta_gluing_t *phi, const theta_couple_point_t *T)
{
    theta_point_t null_point;

    // null_point = (a : b : c : d)
    // a = P1.x P2.x, b = P1.x P2.z, c = P1.z P2.x, d = P1.z P2.z
    fp2_mul(&null_point.x, &T->P1.x, &T->P2.x);
    fp2_mul(&null_point.y, &T->P1.x, &T->P2.z);
    fp2_mul(&null_point.z, &T->P2.x, &T->P1.z);
    fp2_mul(&null_point.t, &T->P1.z, &T->P2.z);

    // Apply the basis change
    apply_isomorphism(out, &phi->M, &null_point);
}

static void
action_by_translation_z_and_det(fp2_t *z_inv, fp2_t *det_inv, const ec_point_t *P4, const ec_point_t *P2)
{
    // Store the Z-coordinate to invert
    fp2_copy(z_inv, &P4->z);

    // Then collect detij = xij wij - uij zij
    fp2_t tmp;
    fp2_mul(det_inv, &P4->x, &P2->z);
    fp2_mul(&tmp, &P4->z, &P2->x);
    fp2_sub(det_inv, det_inv, &tmp);
}

static void
action_by_translation_compute_matrix(translation_matrix_t *G,
                                     const ec_point_t *P4,
                                     const ec_point_t *P2,
                                     const fp2_t *z_inv,
                                     const fp2_t *det_inv)
{
    fp2_t tmp;

    // Gi.g10 = uij xij /detij - xij/zij
    fp2_mul(&tmp, &P4->x, z_inv);
    fp2_mul(&G->g10, &P4->x, &P2->x);
    fp2_mul(&G->g10, &G->g10, det_inv);
    fp2_sub(&G->g10, &G->g10, &tmp);

    // Gi.g11 = uij zij * detij
    fp2_mul(&G->g11, &P2->x, det_inv);
    fp2_mul(&G->g11, &G->g11, &P4->z);

    // Gi.g00 = -Gi.g11
    fp2_neg(&G->g00, &G->g11);

    // Gi.g01 = - wij zij detij
    fp2_mul(&G->g01, &P2->z, det_inv);
    fp2_mul(&G->g01, &G->g01, &P4->z);
    fp2_neg(&G->g01, &G->g01);
}

// Returns 1 if the basis is as expected and 0 otherwise
// We only expect this to fail for malformed signatures, so
// do not require this to run in constant time.
static int
verify_two_torsion(const theta_couple_point_t *K1_2, const theta_couple_point_t *K2_2, const theta_couple_curve_t *E12)
{
    // First check if any point in K1_2 or K2_2 is zero, if they are then the points did not have
    // order 8 when we started gluing
    if (ec_is_zero(&K1_2->P1) | ec_is_zero(&K1_2->P2) | ec_is_zero(&K2_2->P1) | ec_is_zero(&K2_2->P2)) {
        return 0;
    }

    // Now ensure that P1, Q1 and P2, Q2 are independent. For points of order two this means
    // that they're not the same
    if (ec_is_equal(&K1_2->P1, &K2_2->P1) | ec_is_equal(&K1_2->P2, &K2_2->P2)) {
        return 0;
    }

    // Finally, double points to ensure all points have order exactly 0
    theta_couple_point_t O1, O2;
    double_couple_point(&O1, K1_2, E12);
    double_couple_point(&O2, K2_2, E12);
    // If this check fails then the points had order 2*f for some f, and the kernel is malformed.
    if (!(ec_is_zero(&O1.P1) & ec_is_zero(&O1.P2) & ec_is_zero(&O2.P1) & ec_is_zero(&O2.P2))) {
        return 0;
    }

    return 1;
}

// Computes the action by translation for four points
// (P1, P2) and (Q1, Q2) on E1 x E2 simultaneously to
// save on inversions.
// Returns 0 if any of Pi or Qi does not have order 2
// and 1 otherwise
static int
action_by_translation(translation_matrix_t *Gi,
                      const theta_couple_point_t *K1_4,
                      const theta_couple_point_t *K2_4,
                      const theta_couple_curve_t *E12)
{
    // Compute points of order 2 from Ki_4
    theta_couple_point_t K1_2, K2_2;
    double_couple_point(&K1_2, K1_4, E12);
    double_couple_point(&K2_2, K2_4, E12);

    if (!verify_two_torsion(&K1_2, &K2_2, E12)) {
        return 0;
    }

    // We need to invert four Z coordinates and
    // four determinants which we do with batched
    // inversion
    fp2_t inverses[8];
    action_by_translation_z_and_det(&inverses[0], &inverses[4], &K1_4->P1, &K1_2.P1);
    action_by_translation_z_and_det(&inverses[1], &inverses[5], &K1_4->P2, &K1_2.P2);
    action_by_translation_z_and_det(&inverses[2], &inverses[6], &K2_4->P1, &K2_2.P1);
    action_by_translation_z_and_det(&inverses[3], &inverses[7], &K2_4->P2, &K2_2.P2);

    fp2_batched_inv(inverses, 8);
    if (fp2_is_zero(&inverses[0]))
        return 0; // something was wrong with our input (which somehow was not caught by
                  // verify_two_torsion)

    action_by_translation_compute_matrix(&Gi[0], &K1_4->P1, &K1_2.P1, &inverses[0], &inverses[4]);
    action_by_translation_compute_matrix(&Gi[1], &K1_4->P2, &K1_2.P2, &inverses[1], &inverses[5]);
    action_by_translation_compute_matrix(&Gi[2], &K2_4->P1, &K2_2.P1, &inverses[2], &inverses[6]);
    action_by_translation_compute_matrix(&Gi[3], &K2_4->P2, &K2_2.P2, &inverses[3], &inverses[7]);

    return 1;
}

// Given the appropriate four torsion, computes the
// change of basis to compute the correct theta null
// point.
// Returns 0 if the order of K1_4 or K2_4 is not 4
static int
gluing_change_of_basis(basis_change_matrix_t *M,
                       const theta_couple_point_t *K1_4,
                       const theta_couple_point_t *K2_4,
                       const theta_couple_curve_t *E12)
{
    // Compute the four 2x2 matrices for the action by translation
    // on the four points:
    translation_matrix_t Gi[4];
    if (!action_by_translation(Gi, K1_4, K2_4, E12))
        return 0;

    // Computation of the 4x4 matrix from Mij
    // t001, t101 (resp t002, t102) first column of M11 * M21 (resp M12 * M22)
    fp2_t t001, t101, t002, t102, tmp;

    fp2_mul(&t001, &Gi[0].g00, &Gi[2].g00);
    fp2_mul(&tmp, &Gi[0].g01, &Gi[2].g10);
    fp2_add(&t001, &t001, &tmp);

    fp2_mul(&t101, &Gi[0].g10, &Gi[2].g00);
    fp2_mul(&tmp, &Gi[0].g11, &Gi[2].g10);
    fp2_add(&t101, &t101, &tmp);

    fp2_mul(&t002, &Gi[1].g00, &Gi[3].g00);
    fp2_mul(&tmp, &Gi[1].g01, &Gi[3].g10);
    fp2_add(&t002, &t002, &tmp);

    fp2_mul(&t102, &Gi[1].g10, &Gi[3].g00);
    fp2_mul(&tmp, &Gi[1].g11, &Gi[3].g10);
    fp2_add(&t102, &t102, &tmp);

    // trace for the first row
    fp2_set_one(&M->m[0][0]);
    fp2_mul(&tmp, &t001, &t002);
    fp2_add(&M->m[0][0], &M->m[0][0], &tmp);
    fp2_mul(&tmp, &Gi[2].g00, &Gi[3].g00);
    fp2_add(&M->m[0][0], &M->m[0][0], &tmp);
    fp2_mul(&tmp, &Gi[0].g00, &Gi[1].g00);
    fp2_add(&M->m[0][0], &M->m[0][0], &tmp);

    fp2_mul(&M->m[0][1], &t001, &t102);
    fp2_mul(&tmp, &Gi[2].g00, &Gi[3].g10);
    fp2_add(&M->m[0][1], &M->m[0][1], &tmp);
    fp2_mul(&tmp, &Gi[0].g00, &Gi[1].g10);
    fp2_add(&M->m[0][1], &M->m[0][1], &tmp);

    fp2_mul(&M->m[0][2], &t101, &t002);
    fp2_mul(&tmp, &Gi[2].g10, &Gi[3].g00);
    fp2_add(&M->m[0][2], &M->m[0][2], &tmp);
    fp2_mul(&tmp, &Gi[0].g10, &Gi[1].g00);
    fp2_add(&M->m[0][2], &M->m[0][2], &tmp);

    fp2_mul(&M->m[0][3], &t101, &t102);
    fp2_mul(&tmp, &Gi[2].g10, &Gi[3].g10);
    fp2_add(&M->m[0][3], &M->m[0][3], &tmp);
    fp2_mul(&tmp, &Gi[0].g10, &Gi[1].g10);
    fp2_add(&M->m[0][3], &M->m[0][3], &tmp);

    // Compute the action of (0,out.K2_4.P2) for the second row
    fp2_mul(&tmp, &Gi[3].g01, &M->m[0][1]);
    fp2_mul(&M->m[1][0], &Gi[3].g00, &M->m[0][0]);
    fp2_add(&M->m[1][0], &M->m[1][0], &tmp);

    fp2_mul(&tmp, &Gi[3].g11, &M->m[0][1]);
    fp2_mul(&M->m[1][1], &Gi[3].g10, &M->m[0][0]);
    fp2_add(&M->m[1][1], &M->m[1][1], &tmp);

    fp2_mul(&tmp, &Gi[3].g01, &M->m[0][3]);
    fp2_mul(&M->m[1][2], &Gi[3].g00, &M->m[0][2]);
    fp2_add(&M->m[1][2], &M->m[1][2], &tmp);

    fp2_mul(&tmp, &Gi[3].g11, &M->m[0][3]);
    fp2_mul(&M->m[1][3], &Gi[3].g10, &M->m[0][2]);
    fp2_add(&M->m[1][3], &M->m[1][3], &tmp);

    // compute the action of (K1_4.P1,0) for the third row
    fp2_mul(&tmp, &Gi[0].g01, &M->m[0][2]);
    fp2_mul(&M->m[2][0], &Gi[0].g00, &M->m[0][0]);
    fp2_add(&M->m[2][0], &M->m[2][0], &tmp);

    fp2_mul(&tmp, &Gi[0].g01, &M->m[0][3]);
    fp2_mul(&M->m[2][1], &Gi[0].g00, &M->m[0][1]);
    fp2_add(&M->m[2][1], &M->m[2][1], &tmp);

    fp2_mul(&tmp, &Gi[0].g11, &M->m[0][2]);
    fp2_mul(&M->m[2][2], &Gi[0].g10, &M->m[0][0]);
    fp2_add(&M->m[2][2], &M->m[2][2], &tmp);

    fp2_mul(&tmp, &Gi[0].g11, &M->m[0][3]);
    fp2_mul(&M->m[2][3], &Gi[0].g10, &M->m[0][1]);
    fp2_add(&M->m[2][3], &M->m[2][3], &tmp);

    // compute the action of (K1_4.P1,K2_4.P2) for the final row
    fp2_mul(&tmp, &Gi[0].g01, &M->m[1][2]);
    fp2_mul(&M->m[3][0], &Gi[0].g00, &M->m[1][0]);
    fp2_add(&M->m[3][0], &M->m[3][0], &tmp);

    fp2_mul(&tmp, &Gi[0].g01, &M->m[1][3]);
    fp2_mul(&M->m[3][1], &Gi[0].g00, &M->m[1][1]);
    fp2_add(&M->m[3][1], &M->m[3][1], &tmp);

    fp2_mul(&tmp, &Gi[0].g11, &M->m[1][2]);
    fp2_mul(&M->m[3][2], &Gi[0].g10, &M->m[1][0]);
    fp2_add(&M->m[3][2], &M->m[3][2], &tmp);

    fp2_mul(&tmp, &Gi[0].g11, &M->m[1][3]);
    fp2_mul(&M->m[3][3], &Gi[0].g10, &M->m[1][1]);
    fp2_add(&M->m[3][3], &M->m[3][3], &tmp);

    return 1;
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
 * if the kernel supplied has the incorrect order, or gluing seems malformed,
 * returns 0, otherwise returns 1.
 */
static int
gluing_compute(theta_gluing_t *out,
               const theta_couple_curve_t *E12,
               const theta_couple_jac_point_t *xyK1_8,
               const theta_couple_jac_point_t *xyK2_8,
               bool verify)
{
    // Ensure that we have been given the eight torsion
#ifndef NDEBUG
    {
        int check = test_jac_order_twof(&xyK1_8->P1, &E12->E1, 3);
        if (!check)
            debug_print("xyK1_8->P1 does not have order 8");
        check = test_jac_order_twof(&xyK2_8->P1, &E12->E1, 3);
        if (!check)
            debug_print("xyK2_8->P1 does not have order 8");
        check = test_jac_order_twof(&xyK1_8->P2, &E12->E2, 3);
        if (!check)
            debug_print("xyK2_8->P1 does not have order 8");
        check = test_jac_order_twof(&xyK2_8->P2, &E12->E2, 3);
        if (!check)
            debug_print("xyK2_8->P2 does not have order 8");
    }
#endif

    out->xyK1_8 = *xyK1_8;
    out->domain = *E12;

    // Given points in E[8] x E[8] we need the four torsion below
    theta_couple_jac_point_t xyK1_4, xyK2_4;

    double_couple_jac_point(&xyK1_4, xyK1_8, E12);
    double_couple_jac_point(&xyK2_4, xyK2_8, E12);

    // Convert from (X:Y:Z) coordinates to (X:Z)
    theta_couple_point_t K1_8, K2_8;
    theta_couple_point_t K1_4, K2_4;

    couple_jac_to_xz(&K1_8, xyK1_8);
    couple_jac_to_xz(&K2_8, xyK2_8);
    couple_jac_to_xz(&K1_4, &xyK1_4);
    couple_jac_to_xz(&K2_4, &xyK2_4);

    // Set the basis change matrix, if we have not been given a valid K[8] for this computation
    // gluing_change_of_basis will detect this and return 0
    if (!gluing_change_of_basis(&out->M, &K1_4, &K2_4, E12)) {
        debug_print("gluing failed as kernel does not have correct order");
        return 0;
    }

    // apply the base change to the kernel
    theta_point_t TT1, TT2;

    base_change(&TT1, out, &K1_8);
    base_change(&TT2, out, &K2_8);

    // compute the codomain
    to_squared_theta(&TT1, &TT1);
    to_squared_theta(&TT2, &TT2);

    // If the kernel is well formed then TT1.t and TT2.t are zero
    // if they are not, we exit early as the signature we are validating
    // is probably malformed
    if (!(fp2_is_zero(&TT1.t) & fp2_is_zero(&TT2.t))) {
        debug_print("gluing failed TT1.t or TT2.t is not zero");
        return 0;
    }
    // Test our projective factors are non zero
    if (fp2_is_zero(&TT1.x) | fp2_is_zero(&TT2.x) | fp2_is_zero(&TT1.y) | fp2_is_zero(&TT2.z) | fp2_is_zero(&TT1.z))
        return 0; // invalid input

    // Projective factor: Ax
    fp2_mul(&out->codomain.x, &TT1.x, &TT2.x);
    fp2_mul(&out->codomain.y, &TT1.y, &TT2.x);
    fp2_mul(&out->codomain.z, &TT1.x, &TT2.z);
    fp2_set_zero(&out->codomain.t);
    // Projective factor: ABCxz
    fp2_mul(&out->precomputation.x, &TT1.y, &TT2.z);
    fp2_copy(&out->precomputation.y, &out->codomain.z);
    fp2_copy(&out->precomputation.z, &out->codomain.y);
    fp2_set_zero(&out->precomputation.t);

    // Compute the two components of phi(K1_8) = (x:x:y:y).
    fp2_mul(&out->imageK1_8.x, &TT1.x, &out->precomputation.x);
    fp2_mul(&out->imageK1_8.y, &TT1.z, &out->precomputation.z);

    // If K1_8 and K2_8 are our 8-torsion points, this ensures that the
    // 4-torsion points [2]K1_8 and [2]K2_8 are isotropic.
    if (verify) {
        fp2_t t1, t2;
        fp2_mul(&t1, &TT1.y, &out->precomputation.y);
        if (!fp2_is_equal(&out->imageK1_8.x, &t1))
            return 0;
        fp2_mul(&t1, &TT2.x, &out->precomputation.x);
        fp2_mul(&t2, &TT2.z, &out->precomputation.z);
        if (!fp2_is_equal(&t2, &t1))
            return 0;
    }

    // compute the final codomain
    hadamard(&out->codomain, &out->codomain);
    return 1;
}

// sub routine of the gluing eval
static void
gluing_eval_point(theta_point_t *image, const theta_couple_jac_point_t *P, const theta_gluing_t *phi)
{
    theta_point_t T1, T2;
    add_components_t add_comp1, add_comp2;

    // Compute the cross addition components of P1+Q1 and P2+Q2
    jac_to_xz_add_components(&add_comp1, &P->P1, &phi->xyK1_8.P1, &phi->domain.E1);
    jac_to_xz_add_components(&add_comp2, &P->P2, &phi->xyK1_8.P2, &phi->domain.E2);

    // Compute T1 and T2 derived from the cross addition components.
    fp2_mul(&T1.x, &add_comp1.u, &add_comp2.u); // T1x = u1u2
    fp2_mul(&T2.t, &add_comp1.v, &add_comp2.v); // T2t = v1v2
    fp2_add(&T1.x, &T1.x, &T2.t);               // T1x = u1u2 + v1v2
    fp2_mul(&T1.y, &add_comp1.u, &add_comp2.w); // T1y = u1w2
    fp2_mul(&T1.z, &add_comp1.w, &add_comp2.u); // T1z = w1u2
    fp2_mul(&T1.t, &add_comp1.w, &add_comp2.w); // T1t = w1w2
    fp2_add(&T2.x, &add_comp1.u, &add_comp1.v); // T2x = (u1+v1)
    fp2_add(&T2.y, &add_comp2.u, &add_comp2.v); // T2y = (u2+v2)
    fp2_mul(&T2.x, &T2.x, &T2.y);               // T2x = (u1+v1)(u2+v2)
    fp2_sub(&T2.x, &T2.x, &T1.x);               // T1x = v1u2 + u1v2
    fp2_mul(&T2.y, &add_comp1.v, &add_comp2.w); // T2y = v1w2
    fp2_mul(&T2.z, &add_comp1.w, &add_comp2.v); // T2z = w1v2
    fp2_set_zero(&T2.t);                        // T2t = 0

    // Apply the basis change and compute their respective square
    // theta(P+Q) = M.T1 - M.T2 and theta(P-Q) = M.T1 + M.T2
    apply_isomorphism_general(&T1, &phi->M, &T1, true);
    apply_isomorphism_general(&T2, &phi->M, &T2, false);
    pointwise_square(&T1, &T1);
    pointwise_square(&T2, &T2);

    // the difference between the two is therefore theta(P+Q)theta(P-Q)
    // whose hadamard transform is then the product of the dual
    // theta_points of phi(P) and phi(Q).
    fp2_sub(&T1.x, &T1.x, &T2.x);
    fp2_sub(&T1.y, &T1.y, &T2.y);
    fp2_sub(&T1.z, &T1.z, &T2.z);
    fp2_sub(&T1.t, &T1.t, &T2.t);
    hadamard(&T1, &T1);

    // Compute (x, y, z, t)
    // As imageK1_8 = (x:x:y:y), its inverse is (y:y:x:x).
    fp2_mul(&image->x, &T1.x, &phi->imageK1_8.y);
    fp2_mul(&image->y, &T1.y, &phi->imageK1_8.y);
    fp2_mul(&image->z, &T1.z, &phi->imageK1_8.x);
    fp2_mul(&image->t, &T1.t, &phi->imageK1_8.x);

    hadamard(image, image);
}

// Same as gluing_eval_point but in the very special case where we already know that the point will
// have a zero coordinate at the place where the zero coordinate of the dual_theta_nullpoint would
// have made the computation difficult
static int
gluing_eval_point_special_case(theta_point_t *image, const theta_couple_point_t *P, const theta_gluing_t *phi)
{
    theta_point_t T;

    // Apply the basis change
    base_change(&T, phi, P);

    // Apply the to_squared_theta transform
    to_squared_theta(&T, &T);

    // This coordinate should always be 0 in a gluing because D=0.
    // If this is not the case, something went very wrong, so reject
    if (!fp2_is_zero(&T.t))
        return 0;

    // Compute (x, y, z, t)
    fp2_mul(&image->x, &T.x, &phi->precomputation.x);
    fp2_mul(&image->y, &T.y, &phi->precomputation.y);
    fp2_mul(&image->z, &T.z, &phi->precomputation.z);
    fp2_set_zero(&image->t);

    hadamard(image, image);
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
static void
gluing_eval_basis(theta_point_t *image1,
                  theta_point_t *image2,
                  const theta_couple_jac_point_t *xyT1,
                  const theta_couple_jac_point_t *xyT2,
                  const theta_gluing_t *phi)
{
    gluing_eval_point(image1, xyT1, phi);
    gluing_eval_point(image2, xyT2, phi);
}

/**
 * @brief Compute a (2,2) isogeny in dimension 2 in the theta_model
 *
 * @param out Output: the theta_isogeny
 * @param A a theta null point for the domain
 * @param T1_8 a point in A[8]
 * @param T2_8 a point in A[8]
 * @param hadamard_bool_1 a boolean used for the last two steps of the chain
 * @param hadamard_bool_2 a boolean used for the last two steps of the chain
 *
 * out : A -> B of kernel [4](T1_8,T2_8)
 * hadamard_bool_1 controls if the domain is in standard or dual coordinates
 * hadamard_bool_2 controls if the codomain is in standard or dual coordinates
 * verify: add extra sanity check to ensure our 8-torsion points are coherent with the isogeny
 *
 */
static int
theta_isogeny_compute(theta_isogeny_t *out,
                      const theta_structure_t *A,
                      const theta_point_t *T1_8,
                      const theta_point_t *T2_8,
                      bool hadamard_bool_1,
                      bool hadamard_bool_2,
                      bool verify)
{
    out->hadamard_bool_1 = hadamard_bool_1;
    out->hadamard_bool_2 = hadamard_bool_2;
    out->domain = *A;
    out->T1_8 = *T1_8;
    out->T2_8 = *T2_8;
    out->codomain.precomputation = false;

    theta_point_t TT1, TT2;

    if (hadamard_bool_1) {
        hadamard(&TT1, T1_8);
        to_squared_theta(&TT1, &TT1);
        hadamard(&TT2, T2_8);
        to_squared_theta(&TT2, &TT2);
    } else {
        to_squared_theta(&TT1, T1_8);
        to_squared_theta(&TT2, T2_8);
    }

    fp2_t t1, t2;

    // Test that our projective factor ABCDxzw is non zero, where
    // TT1=(Ax, Bx, Cy, Dy), TT2=(Az, Bw, Cz, Dw)
    // But ABCDxzw=0 can only happen if we had an unexpected splitting in
    // the isogeny chain.
    // In either case reject
    // (this is not strictly necessary, we could just return (0:0:0:0))
    if (fp2_is_zero(&TT2.x) | fp2_is_zero(&TT2.y) | fp2_is_zero(&TT2.z) | fp2_is_zero(&TT2.t) | fp2_is_zero(&TT1.x) |
        fp2_is_zero(&TT1.y))
        return 0;

    fp2_mul(&t1, &TT1.x, &TT2.y);
    fp2_mul(&t2, &TT1.y, &TT2.x);
    fp2_mul(&out->codomain.null_point.x, &TT2.x, &t1);
    fp2_mul(&out->codomain.null_point.y, &TT2.y, &t2);
    fp2_mul(&out->codomain.null_point.z, &TT2.z, &t1);
    fp2_mul(&out->codomain.null_point.t, &TT2.t, &t2);
    fp2_t t3;
    fp2_mul(&t3, &TT2.z, &TT2.t);
    fp2_mul(&out->precomputation.x, &t3, &TT1.y);
    fp2_mul(&out->precomputation.y, &t3, &TT1.x);
    fp2_copy(&out->precomputation.z, &out->codomain.null_point.t);
    fp2_copy(&out->precomputation.t, &out->codomain.null_point.z);

    // If T1_8 and T2_8 are our 8-torsion points, this ensures that the
    // 4-torsion points 2T1_8 and 2T2_8 are isotropic.
    if (verify) {
        fp2_mul(&t1, &TT1.x, &out->precomputation.x);
        fp2_mul(&t2, &TT1.y, &out->precomputation.y);
        if (!fp2_is_equal(&t1, &t2))
            return 0;
        fp2_mul(&t1, &TT1.z, &out->precomputation.z);
        fp2_mul(&t2, &TT1.t, &out->precomputation.t);
        if (!fp2_is_equal(&t1, &t2))
            return 0;
        fp2_mul(&t1, &TT2.x, &out->precomputation.x);
        fp2_mul(&t2, &TT2.z, &out->precomputation.z);
        if (!fp2_is_equal(&t1, &t2))
            return 0;
        fp2_mul(&t1, &TT2.y, &out->precomputation.y);
        fp2_mul(&t2, &TT2.t, &out->precomputation.t);
        if (!fp2_is_equal(&t1, &t2))
            return 0;
    }

    if (hadamard_bool_2) {
        hadamard(&out->codomain.null_point, &out->codomain.null_point);
    }
    return 1;
}

/**
 * @brief Compute a (2,2) isogeny when only the 4 torsion above the kernel is known and not the 8
 * torsion
 *
 * @param out Output: the theta_isogeny
 * @param A a theta null point for the domain
 * @param T1_4 a point in A[4]
 * @param T2_4 a point in A[4]
 * @param hadamard_bool_1 a boolean
 * @param hadamard_bool_2 a boolean
 *
 * out : A -> B of kernel [2](T1_4,T2_4)
 * hadamard_bool_1 controls if the domain is in standard or dual coordinates
 * hadamard_bool_2 controls if the codomain is in standard or dual coordinates
 *
 */
static void
theta_isogeny_compute_4(theta_isogeny_t *out,
                        const theta_structure_t *A,
                        const theta_point_t *T1_4,
                        const theta_point_t *T2_4,
                        bool hadamard_bool_1,
                        bool hadamard_bool_2)
{
    out->hadamard_bool_1 = hadamard_bool_1;
    out->hadamard_bool_2 = hadamard_bool_2;
    out->domain = *A;
    out->T1_8 = *T1_4;
    out->T2_8 = *T2_4;
    out->codomain.precomputation = false;

    theta_point_t TT1, TT2;
    // we will compute:
    // TT1 = (xAB, _ , xCD, _)
    // TT2 = (AA,BB,CC,DD)

    // fp2_t xA_inv,zA_inv,tB_inv;

    if (hadamard_bool_1) {
        hadamard(&TT1, T1_4);
        to_squared_theta(&TT1, &TT1);

        hadamard(&TT2, &A->null_point);
        to_squared_theta(&TT2, &TT2);
    } else {
        to_squared_theta(&TT1, T1_4);
        to_squared_theta(&TT2, &A->null_point);
    }

    fp2_t sqaabb, sqaacc;
    fp2_mul(&sqaabb, &TT2.x, &TT2.y);
    fp2_mul(&sqaacc, &TT2.x, &TT2.z);
    // No need to check the square roots, only used for signing.
    // sqaabb = sqrt(AA*BB)
    fp2_sqrt(&sqaabb);
    // sqaacc = sqrt(AA*CC)
    fp2_sqrt(&sqaacc);

    // we compute out->codomain.null_point = (xAB * sqaacc * AA, xAB *sqaabb *sqaacc, xCD*sqaabb *
    // AA) out->precomputation = (xAB * BB * CC *DD , sqaabb * CC * DD * xAB , sqaacc * BB* DD * xAB
    // , xCD * sqaabb *sqaacc * BB)

    fp2_mul(&out->codomain.null_point.y, &sqaabb, &sqaacc);
    fp2_mul(&out->precomputation.t, &out->codomain.null_point.y, &TT1.z);
    fp2_mul(&out->codomain.null_point.y, &out->codomain.null_point.y,
            &TT1.x); // done for out->codomain.null_point.y

    fp2_mul(&out->codomain.null_point.t, &TT1.z, &sqaabb);
    fp2_mul(&out->codomain.null_point.t, &out->codomain.null_point.t,
            &TT2.x); // done for out->codomain.null_point.t

    fp2_mul(&out->codomain.null_point.x, &TT1.x, &TT2.x);
    fp2_mul(&out->codomain.null_point.z, &out->codomain.null_point.x,
            &TT2.z); // done for out->codomain.null_point.z
    fp2_mul(&out->codomain.null_point.x, &out->codomain.null_point.x,
            &sqaacc); // done for out->codomain.null_point.x

    fp2_mul(&out->precomputation.x, &TT1.x, &TT2.t);
    fp2_mul(&out->precomputation.z, &out->precomputation.x, &TT2.y);
    fp2_mul(&out->precomputation.x, &out->precomputation.x, &TT2.z);
    fp2_mul(&out->precomputation.y, &out->precomputation.x, &sqaabb); // done for out->precomputation.y
    fp2_mul(&out->precomputation.x, &out->precomputation.x, &TT2.y);  // done for out->precomputation.x
    fp2_mul(&out->precomputation.z, &out->precomputation.z, &sqaacc); // done for out->precomputation.z
    fp2_mul(&out->precomputation.t, &out->precomputation.t, &TT2.y);  // done for out->precomputation.t

    if (hadamard_bool_2) {
        hadamard(&out->codomain.null_point, &out->codomain.null_point);
    }
}

/**
 * @brief Compute a (2,2) isogeny when only the kernel is known and not the 8 or 4 torsion above
 *
 * @param out Output: the theta_isogeny
 * @param A a theta null point for the domain
 * @param T1_2 a point in A[2]
 * @param T2_2 a point in A[2]
 * @param hadamard_bool_1 a boolean
 * @param boo2 a boolean
 *
 * out : A -> B of kernel (T1_2,T2_2)
 * hadamard_bool_1 controls if the domain is in standard or dual coordinates
 * hadamard_bool_2 controls if the codomain is in standard or dual coordinates
 *
 */
static void
theta_isogeny_compute_2(theta_isogeny_t *out,
                        const theta_structure_t *A,
                        const theta_point_t *T1_2,
                        const theta_point_t *T2_2,
                        bool hadamard_bool_1,
                        bool hadamard_bool_2)
{
    out->hadamard_bool_1 = hadamard_bool_1;
    out->hadamard_bool_2 = hadamard_bool_2;
    out->domain = *A;
    out->T1_8 = *T1_2;
    out->T2_8 = *T2_2;
    out->codomain.precomputation = false;

    theta_point_t TT2;
    // we will compute:
    // TT2 = (AA,BB,CC,DD)

    if (hadamard_bool_1) {
        hadamard(&TT2, &A->null_point);
        to_squared_theta(&TT2, &TT2);
    } else {
        to_squared_theta(&TT2, &A->null_point);
    }

    // we compute out->codomain.null_point = (AA,sqaabb, sqaacc, sqaadd)
    // out->precomputation = (  BB * CC *DD , sqaabb * CC * DD , sqaacc * BB* DD , sqaadd * BB * CC)
    fp2_copy(&out->codomain.null_point.x, &TT2.x);
    fp2_mul(&out->codomain.null_point.y, &TT2.x, &TT2.y);
    fp2_mul(&out->codomain.null_point.z, &TT2.x, &TT2.z);
    fp2_mul(&out->codomain.null_point.t, &TT2.x, &TT2.t);
    // No need to check the square roots, only used for signing.
    fp2_sqrt(&out->codomain.null_point.y);
    fp2_sqrt(&out->codomain.null_point.z);
    fp2_sqrt(&out->codomain.null_point.t);

    fp2_mul(&out->precomputation.x, &TT2.z, &TT2.t);
    fp2_mul(&out->precomputation.y,
            &out->precomputation.x,
            &out->codomain.null_point.y);                            // done for out->precomputation.y
    fp2_mul(&out->precomputation.x, &out->precomputation.x, &TT2.y); // done for out->precomputation.x
    fp2_mul(&out->precomputation.z, &TT2.t, &out->codomain.null_point.z);
    fp2_mul(&out->precomputation.z, &out->precomputation.z, &TT2.y); // done for out->precomputation.z
    fp2_mul(&out->precomputation.t, &TT2.z, &out->codomain.null_point.t);
    fp2_mul(&out->precomputation.t, &out->precomputation.t, &TT2.y); // done for out->precomputation.t

    if (hadamard_bool_2) {
        hadamard(&out->codomain.null_point, &out->codomain.null_point);
    }
}

static void
theta_isogeny_eval(theta_point_t *out, const theta_isogeny_t *phi, const theta_point_t *P)
{
    if (phi->hadamard_bool_1) {
        hadamard(out, P);
        to_squared_theta(out, out);
    } else {
        to_squared_theta(out, P);
    }
    fp2_mul(&out->x, &out->x, &phi->precomputation.x);
    fp2_mul(&out->y, &out->y, &phi->precomputation.y);
    fp2_mul(&out->z, &out->z, &phi->precomputation.z);
    fp2_mul(&out->t, &out->t, &phi->precomputation.t);

    if (phi->hadamard_bool_2) {
        hadamard(out, out);
    }
}

#if defined(ENABLE_SIGN)
// Sample a random secret index in [0, 5] to select one of the 6 normalisation
// matrices for the normalisation of the output of the (2,2)-chain during
// splitting
static unsigned char
sample_random_index(void)
{
    // To avoid bias in reduction we should only consider integers smaller
    // than 2^32 which are a multiple of 6, so we only reduce bytes with a
    // value in [0, 4294967292-1].
    // We have 4294967292/2^32 = ~99.9999999% chance that the first try is "good".
    unsigned char seed_arr[4];
    uint32_t seed;

    do {
        randombytes(seed_arr, 4);
        seed = (seed_arr[0] | (seed_arr[1] << 8) | (seed_arr[2] << 16) | (seed_arr[3] << 24));
    } while (seed >= 4294967292U);

    uint32_t secret_index = seed - (((uint64_t)seed * 2863311531U) >> 34) * 6;
    assert(secret_index == seed % 6); // ensure the constant time trick above works
    return (unsigned char)secret_index;
}
#endif

static bool
splitting_compute(theta_splitting_t *out, const theta_structure_t *A, int zero_index, bool randomize)

{
    // init
    uint32_t ctl;
    uint32_t count = 0;
    fp2_t U_cst, t1, t2;

    memset(&out->M, 0, sizeof(basis_change_matrix_t));

    // enumerate through all indices
    for (int i = 0; i < 10; i++) {
        fp2_set_zero(&U_cst);
        for (int t = 0; t < 4; t++) {
            // Iterate through the null point
            choose_index_theta_point(&t2, t, &A->null_point);
            choose_index_theta_point(&t1, t ^ EVEN_INDEX[i][1], &A->null_point);

            // Compute t1 * t2
            fp2_mul(&t1, &t1, &t2);
            // If CHI_EVAL(i,t) is +1 we want ctl to be 0 and
            // If CHI_EVAL(i,t) is -1 we want ctl to be 0xFF..FF
            ctl = (uint32_t)(CHI_EVAL[EVEN_INDEX[i][0]][t] >> 1);
            assert(ctl == 0 || ctl == 0xffffffff);

            fp2_neg(&t2, &t1);
            fp2_select(&t1, &t1, &t2, ctl);

            // Then we compute U_cst ± (t1 * t2)
            fp2_add(&U_cst, &U_cst, &t1);
        }

        // If U_cst is 0 then update the splitting matrix
        ctl = fp2_is_zero(&U_cst);
        count -= ctl;
        select_base_change_matrix(&out->M, &out->M, &SPLITTING_TRANSFORMS[i], ctl);
        if (zero_index != -1 && i == zero_index &&
            !ctl) { // extra checks if we know exactly where the 0 index should be
            return 0;
        }
    }

#if defined(ENABLE_SIGN)
    // Pick a random normalization matrix
    if (randomize) {
        unsigned char secret_index = sample_random_index();
        basis_change_matrix_t Mrandom;

        set_base_change_matrix_from_precomp(&Mrandom, &NORMALIZATION_TRANSFORMS[0]);

        // Use a constant time selection to pick the index we want
        for (unsigned char i = 1; i < 6; i++) {
            // When i == secret_index, mask == 0 and 0xFF..FF otherwise
            int32_t mask = i - secret_index;
            mask = (mask | -mask) >> 31;
            select_base_change_matrix(&Mrandom, &Mrandom, &NORMALIZATION_TRANSFORMS[i], ~mask);
        }
        base_change_matrix_multiplication(&out->M, &Mrandom, &out->M);
    }
#else
    assert(!randomize);
#endif

    // apply the isomorphism to ensure the null point is compatible with splitting
    apply_isomorphism(&out->B.null_point, &out->M, &A->null_point);

    // splitting was successful only if exactly one zero was identified
    return count == 1;
}

static int
theta_product_structure_to_elliptic_product(theta_couple_curve_t *E12, theta_structure_t *A)
{
    fp2_t xx, yy;

    // This should be true from our computations in splitting_compute
    // but still check this for sanity
    if (!is_product_theta_point(&A->null_point))
        return 0;

    ec_curve_init(&(E12->E1));
    ec_curve_init(&(E12->E2));

    // A valid elliptic theta null point has no zero coordinate
    if (fp2_is_zero(&A->null_point.x) | fp2_is_zero(&A->null_point.y) | fp2_is_zero(&A->null_point.z))
        return 0;

    // xx = x², yy = y²
    fp2_sqr(&xx, &A->null_point.x);
    fp2_sqr(&yy, &A->null_point.y);
    // xx = x^4, yy = y^4
    fp2_sqr(&xx, &xx);
    fp2_sqr(&yy, &yy);

    // A2 = -2(x^4+y^4)/(x^4-y^4)
    fp2_add(&E12->E2.A, &xx, &yy);
    fp2_sub(&E12->E2.C, &xx, &yy);
    fp2_add(&E12->E2.A, &E12->E2.A, &E12->E2.A);
    fp2_neg(&E12->E2.A, &E12->E2.A);

    // same with x,z
    fp2_sqr(&xx, &A->null_point.x);
    fp2_sqr(&yy, &A->null_point.z);
    fp2_sqr(&xx, &xx);
    fp2_sqr(&yy, &yy);

    // A1 = -2(x^4+z^4)/(x^4-z^4)
    fp2_add(&E12->E1.A, &xx, &yy);
    fp2_sub(&E12->E1.C, &xx, &yy);
    fp2_add(&E12->E1.A, &E12->E1.A, &E12->E1.A);
    fp2_neg(&E12->E1.A, &E12->E1.A);

    if (fp2_is_zero(&E12->E1.C) | fp2_is_zero(&E12->E2.C))
        return 0;

    return 1;
}

static int
theta_point_to_montgomery_point(theta_couple_point_t *P12, const theta_point_t *P, const theta_structure_t *A)
{
    fp2_t temp;
    const fp2_t *x, *z;

    if (!is_product_theta_point(P))
        return 0;

    x = &P->x;
    z = &P->y;
    if (fp2_is_zero(x) & fp2_is_zero(z)) {
        x = &P->z;
        z = &P->t;
    }
    if (fp2_is_zero(x) & fp2_is_zero(z)) {
        return 0; // at this point P=(0:0:0:0) so is invalid
    }
    // P2.X = A.null_point.y * P.x + A.null_point.x * P.y
    // P2.Z = - A.null_point.y * P.x + A.null_point.x * P.y
    fp2_mul(&P12->P2.x, &A->null_point.y, x);
    fp2_mul(&temp, &A->null_point.x, z);
    fp2_sub(&P12->P2.z, &temp, &P12->P2.x);
    fp2_add(&P12->P2.x, &P12->P2.x, &temp);

    x = &P->x;
    z = &P->z;
    if (fp2_is_zero(x) & fp2_is_zero(z)) {
        x = &P->y;
        z = &P->t;
    }
    // P1.X = A.null_point.z * P.x + A.null_point.x * P.z
    // P1.Z = -A.null_point.z * P.x + A.null_point.x * P.z
    fp2_mul(&P12->P1.x, &A->null_point.z, x);
    fp2_mul(&temp, &A->null_point.x, z);
    fp2_sub(&P12->P1.z, &temp, &P12->P1.x);
    fp2_add(&P12->P1.x, &P12->P1.x, &temp);
    return 1;
}

static int
_theta_chain_compute_impl(unsigned n,
                          theta_couple_curve_t *E12,
                          const theta_kernel_couple_points_t *ker,
                          bool extra_torsion,
                          theta_couple_curve_t *E34,
                          theta_couple_point_t *P12,
                          size_t numP,
                          bool verify,
                          bool randomize)
{
    theta_structure_t theta;

    // lift the basis
    theta_couple_jac_point_t xyT1, xyT2;

    ec_basis_t bas1 = { .P = ker->T1.P1, .Q = ker->T2.P1, .PmQ = ker->T1m2.P1 };
    ec_basis_t bas2 = { .P = ker->T1.P2, .Q = ker->T2.P2, .PmQ = ker->T1m2.P2 };
    if (!lift_basis(&xyT1.P1, &xyT2.P1, &bas1, &E12->E1))
        return 0;
    if (!lift_basis(&xyT1.P2, &xyT2.P2, &bas2, &E12->E2))
        return 0;

    const unsigned extra = HD_extra_torsion * extra_torsion;

#ifndef NDEBUG
    assert(extra == 0 || extra == 2); // only cases implemented
    if (!test_point_order_twof(&bas2.P, &E12->E2, n + extra))
        debug_print("bas2.P does not have correct order");

    if (!test_jac_order_twof(&xyT2.P2, &E12->E2, n + extra))
        debug_print("xyT2.P2 does not have correct order");
#endif

    theta_point_t pts[numP ? numP : 1];

    int space = 1;
    for (unsigned i = 1; i < n; i *= 2)
        ++space;

    uint16_t todo[space];
    todo[0] = n - 2 + extra;

    int current = 0;

    // kernel points for the gluing isogeny
    theta_couple_jac_point_t jacQ1[space], jacQ2[space];
    jacQ1[0] = xyT1;
    jacQ2[0] = xyT2;
    while (todo[current] != 1) {
        assert(todo[current] >= 2);
        ++current;
        assert(current < space);
        // the gluing isogeny is quite a bit more expensive than the others,
        // so we adjust the usual splitting rule here a little bit: towards
        // the end of the doubling chain it will be cheaper to recompute the
        // doublings after evaluation than to push the intermediate points.
        const unsigned num_dbls = todo[current - 1] >= 16 ? todo[current - 1] / 2 : todo[current - 1] - 1;
        assert(num_dbls && num_dbls < todo[current - 1]);
        double_couple_jac_point_iter(&jacQ1[current], num_dbls, &jacQ1[current - 1], E12);
        double_couple_jac_point_iter(&jacQ2[current], num_dbls, &jacQ2[current - 1], E12);
        todo[current] = todo[current - 1] - num_dbls;
    }

    // kernel points for the remaining isogeny steps
    theta_point_t thetaQ1[space], thetaQ2[space];

    // the gluing step
    theta_gluing_t first_step;
    {
        assert(todo[current] == 1);

        // compute the gluing isogeny
        if (!gluing_compute(&first_step, E12, &jacQ1[current], &jacQ2[current], verify))
            return 0;

        // evaluate
        for (unsigned j = 0; j < numP; ++j) {
            assert(ec_is_zero(&P12[j].P1) || ec_is_zero(&P12[j].P2));
            if (!gluing_eval_point_special_case(&pts[j], &P12[j], &first_step))
                return 0;
        }

        // push kernel points through gluing isogeny
        for (int j = 0; j < current; ++j) {
            gluing_eval_basis(&thetaQ1[j], &thetaQ2[j], &jacQ1[j], &jacQ2[j], &first_step);
            --todo[j];
        }

        --current;
    }

    // set-up the theta_structure for the first codomain
    theta.null_point = first_step.codomain;
    theta.precomputation = 0;
    theta_precomputation(&theta);

    theta_isogeny_t step;

    // and now we do the remaining steps
    for (unsigned i = 1; current >= 0 && todo[current]; ++i) {
        assert(current < space);
        while (todo[current] != 1) {
            assert(todo[current] >= 2);
            ++current;
            assert(current < space);
            const unsigned num_dbls = todo[current - 1] / 2;
            assert(num_dbls && num_dbls < todo[current - 1]);
            double_iter(&thetaQ1[current], &theta, &thetaQ1[current - 1], num_dbls);
            double_iter(&thetaQ2[current], &theta, &thetaQ2[current - 1], num_dbls);
            todo[current] = todo[current - 1] - num_dbls;
        }

        // computing the next step
        int ret;
        if (i == n - 2) // penultimate step
            ret = theta_isogeny_compute(&step, &theta, &thetaQ1[current], &thetaQ2[current], 0, 0, verify);
        else if (i == n - 1) // ultimate step
            ret = theta_isogeny_compute(&step, &theta, &thetaQ1[current], &thetaQ2[current], 1, 0, false);
        else
            ret = theta_isogeny_compute(&step, &theta, &thetaQ1[current], &thetaQ2[current], 0, 1, verify);
        if (!ret)
            return 0;

        for (unsigned j = 0; j < numP; ++j)
            theta_isogeny_eval(&pts[j], &step, &pts[j]);

        // updating the codomain
        theta = step.codomain;

        // pushing the kernel
        assert(todo[current] == 1);
        for (int j = 0; j < current; ++j) {
            theta_isogeny_eval(&thetaQ1[j], &step, &thetaQ1[j]);
            theta_isogeny_eval(&thetaQ2[j], &step, &thetaQ2[j]);
            assert(todo[j]);
            --todo[j];
        }

        --current;
    }

    assert(current == -1);

    if (!extra_torsion) {
        if (n >= 3) {
            // in the last step we've skipped pushing the kernel since current was == 0, let's do it now
            theta_isogeny_eval(&thetaQ1[0], &step, &thetaQ1[0]);
            theta_isogeny_eval(&thetaQ2[0], &step, &thetaQ2[0]);
        }

        // penultimate step
        theta_isogeny_compute_4(&step, &theta, &thetaQ1[0], &thetaQ2[0], 0, 0);
        for (unsigned j = 0; j < numP; ++j)
            theta_isogeny_eval(&pts[j], &step, &pts[j]);
        theta = step.codomain;
        theta_isogeny_eval(&thetaQ1[0], &step, &thetaQ1[0]);
        theta_isogeny_eval(&thetaQ2[0], &step, &thetaQ2[0]);

        // ultimate step
        theta_isogeny_compute_2(&step, &theta, &thetaQ1[0], &thetaQ2[0], 1, 0);
        for (unsigned j = 0; j < numP; ++j)
            theta_isogeny_eval(&pts[j], &step, &pts[j]);
        theta = step.codomain;
    }

    // final splitting step
    theta_splitting_t last_step;

    bool is_split = splitting_compute(&last_step, &theta, extra_torsion ? 8 : -1, randomize);

    if (!is_split) {
        debug_print("kernel did not generate an isogeny between elliptic products");
        return 0;
    }

    if (!theta_product_structure_to_elliptic_product(E34, &last_step.B))
        return 0;

    // evaluate
    for (size_t j = 0; j < numP; ++j) {
        apply_isomorphism(&pts[j], &last_step.M, &pts[j]);
        if (!theta_point_to_montgomery_point(&P12[j], &pts[j], &last_step.B))
            return 0;
    }

    return 1;
}

int
theta_chain_compute_and_eval(unsigned n,
                             /*const*/ theta_couple_curve_t *E12,
                             const theta_kernel_couple_points_t *ker,
                             bool extra_torsion,
                             theta_couple_curve_t *E34,
                             theta_couple_point_t *P12,
                             size_t numP)
{
    return _theta_chain_compute_impl(n, E12, ker, extra_torsion, E34, P12, numP, false, false);
}

// Like theta_chain_compute_and_eval, adding extra verification checks;
// used in the signature verification
int
theta_chain_compute_and_eval_verify(unsigned n,
                                    /*const*/ theta_couple_curve_t *E12,
                                    const theta_kernel_couple_points_t *ker,
                                    bool extra_torsion,
                                    theta_couple_curve_t *E34,
                                    theta_couple_point_t *P12,
                                    size_t numP)
{
    return _theta_chain_compute_impl(n, E12, ker, extra_torsion, E34, P12, numP, true, false);
}

int
theta_chain_compute_and_eval_randomized(unsigned n,
                                        /*const*/ theta_couple_curve_t *E12,
                                        const theta_kernel_couple_points_t *ker,
                                        bool extra_torsion,
                                        theta_couple_curve_t *E34,
                                        theta_couple_point_t *P12,
                                        size_t numP)
{
    return _theta_chain_compute_impl(n, E12, ker, extra_torsion, E34, P12, numP, false, true);
}
