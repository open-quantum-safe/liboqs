/** @file
 *
 * @authors Antonin Leroux
 *
 * @brief The id2iso algorithms
 */

#ifndef ID2ISO_H
#define ID2ISO_H

#include <biextension.h>
#include <ec.h>
#include <hd.h>
#include <hd_splitting_transforms.h>
#include <quaternion.h>
#include <quaternion_constants.h>
#include <quaternion_data.h>
#include <sqisign_namespace.h>

/** @defgroup id2iso_id2iso Ideal to isogeny conversion
 * @{
 */
static const quat_represent_integer_params_t QUAT_represent_integer_params = {
    .algebra = &QUATALG_PINFTY,                          /// The level-specific quaternion algebra
    .order = &(EXTREMAL_ORDERS[0]),                      // The special extremal order O0
    .primality_test_iterations = QUAT_primality_num_iter // precompted bound on the iteration number in primality tests
};

/*************************** Functions *****************************/

/** @defgroup id2iso_others Other functions needed for id2iso
 * @{
 */

/**
 * @brief Scalar multiplication [x]P + [y]Q where x and y are stored inside an
 * ibz_vec_2_t [x, y] and P, Q in E[2^f]
 *
 * @param res Output: the point R = [x]P + [y]Q
 * @param scalar_vec: a vector of ibz type elements (x, y)
 * @param f: an integer such that P, Q are in E[2^f]
 * @param PQ: an x-only basis x(P), x(Q) and x(P-Q)
 * @param curve: the curve E the points P, Q, R are defined on
 *
 */
void ec_biscalar_mul_ibz_vec(ec_point_t *res,
                             const ibz_vec_2_t *scalar_vec,
                             const int f,
                             const ec_basis_t *PQ,
                             const ec_curve_t *curve);

/**
 * @brief Translating an ideal of norm 2^f dividing p²-1 into the corresponding
 * kernel coefficients
 *
 * @param ker_dlog Output : two coefficients indicating the decomposition of the
 * kernel over the canonical basis of E0[2^f]
 * @param lideal_input : O0-ideal corresponding to the ideal to be translated of
 * norm 2^f
 *
 */
void id2iso_ideal_to_kernel_dlogs_even(ibz_vec_2_t *ker_dlog, const quat_left_ideal_t *lideal_input);

/**
 * @brief Applies some 2x2 matrix on a basis of E[2^TORSION_EVEN_POWER]
 *
 * @param P the basis
 * @param E the curve
 * @param mat the matrix
 * @param f TORSION_EVEN_POWER
 * @returns 1 if success, 0 if error
 *
 * helper function, works in place
 *
 */
int matrix_application_even_basis(ec_basis_t *P, const ec_curve_t *E, ibz_mat_2x2_t *mat, int f);

/**
 * @brief Applies some endomorphism of an alternate curve to E[f]
 *
 * @param P the basis
 * @param index_alternate_curve index of the alternate order in the list of precomputed extremal
 * orders
 * @param E the curve (E is not required to be the alternate curve in question since in the end we
 * only apply a matrix)
 * @param theta the endomorphism
 * @param f TORSION_EVEN_POWER
 *
 * helper function, works in place
 *
 */
void endomorphism_application_even_basis(ec_basis_t *P,
                                         const int index_alternate_curve,
                                         const ec_curve_t *E,
                                         const quat_alg_elem_t *theta,
                                         int f);

/**
 * @brief Translating a kernel on the curve E0, represented as a vector with
 * respect to the precomputed 2^f-torsion basis, into the corresponding O0-ideal
 *
 * @param lideal Output : the output O0-ideal
 * @param f : exponent definining the norm of the ideal to compute
 * @param vec2 : length-2 vector giving the 2-power part of the kernel with
 * respect to the precomputed 2^f basis
 *
 */
void id2iso_kernel_dlogs_to_ideal_even(quat_left_ideal_t *lideal, const ibz_vec_2_t *vec2, int f);

/**
 * @brief Change of basis matrix for full basis B2
 * Finds mat such that:
 * (mat*v).B2 = v.B1
 * where "." is the dot product, defined as (v1,v2).(P,Q) = v1*P + v2*Q
 *
 * @param mat the computed change of basis matrix
 * @param B1 the source basis for E[2^f]
 * @param B2 the target basis for E[2^e]
 * @param E the elliptic curve
 * @param f 2^f is the order of the points of the input basis
 *
 * mat encodes the coordinates of the points of B1 in the basis B2
 */
void change_of_basis_matrix_tate(ibz_mat_2x2_t *mat, const ec_basis_t *B1, const ec_basis_t *B2, ec_curve_t *E, int f);

/**
 * @brief Change of basis matrix for full basis B2
 * Finds mat such that:
 * (mat*v).B1 = [2^e-f]*v.B2
 * where "." is the dot product, defined as (v1,v2).(P,Q) = v1*P + v2*Q
 *
 * @param mat the computed change of basis matrix
 * @param B1 the source basis for E[2^e]
 * @param B2 the target basis for E[2^f]
 * @param E the elliptic curve
 * @param f 2^f is the order of the points of the input basis
 *
 * mat encodes the coordinates of the points of B1 in the basis B2, by
 * applying change_of_basis_matrix_tate and inverting the outcome
 */
void change_of_basis_matrix_tate_invert(ibz_mat_2x2_t *mat,
                                        const ec_basis_t *B1,
                                        const ec_basis_t *B2,
                                        ec_curve_t *E,
                                        int f);

/** @}
 */

/** @defgroup id2iso_arbitrary Arbitrary isogeny evaluation
 * @{
 */
/**
 * @brief Function to find elements u, v, d1, d2, beta1, beta2 for the ideal to isogeny
 *
 * @param u Output: integer
 * @param v Output: integer
 * @param beta1 Output: quaternion element
 * @param beta2 Output: quaternion element
 * @param d1 Output: integer
 * @param d2 Output: integer
 * @param index_alternate_order_1 Output: small integer (index of an alternate order)
 * @param index_alternate_order_2 Output: small integer (index of an alternate order)
 * @param target : integer, target norm
 * @param lideal : O0-ideal defining the search space
 * @param Bpoo : quaternion algebra
 * @param num_alternate_order number of alternate order we consider
 * @returns 1 if the computation succeeds, 0 otherwise
 *
 * Let us write ti = index_alternate_order_i,
 * we look for u,v,beta1,beta2,d1,d2,t1,t2
 * such that u d1 + v d2 = target
 * and where di = norm(betai)/norm(Ii), where the ideal Ii is equal to overbar{Ji} * lideal and
 * betai is in Ii where Ji is a connecting ideal between the maximal order O0 and O_ti t1,t2 must be
 * contained between 0 and num_alternate_order This corresponds to the function SuitableIdeals in
 * the spec
 */
int find_uv(ibz_t *u,
            ibz_t *v,
            quat_alg_elem_t *beta1,
            quat_alg_elem_t *beta2,
            ibz_t *d1,
            ibz_t *d2,
            int *index_alternate_order_1,
            int *index_alternate_order_2,
            const ibz_t *target,
            const quat_left_ideal_t *lideal,
            const quat_alg_t *Bpoo,
            int num_alternate_order);

/**
 * @brief Computes an arbitrary isogeny of fixed degree starting from E0
 * and evaluates it a list of points of the form (P1,0) or (0,P2).
 *
 * @param lideal Output : an ideal of norm u
 * @param u : integer
 * @param small : bit indicating if we the value of u is "small" meaning that we
 expect it to be
 * around sqrt{p}, in that case we use a length slightly above
 * @param E34 Output: the codomain curve
 * @param P12 Input/Output: pointer to points to be pushed through the isogeny
 (in-place)
 * @param numP: length of the list of points given in P12 (can be zero)
 * @param index_alternate_order : index of the special extremal order to be used (in the list of
 these orders)
 * @returns the length of the chain if the computation succeeded, zero upon
 failure
 *
 * F is an isogeny encoding an isogeny [adjust]*phi : E0 -> Eu of degree u
 * note that the codomain of F can be either Eu x Eu' or Eu' x Eu for some curve
 Eu'
 */
int fixed_degree_isogeny_and_eval(quat_left_ideal_t *lideal,
                                  const ibz_t *u,
                                  bool small,
                                  theta_couple_curve_t *E34,
                                  theta_couple_point_t *P12,
                                  size_t numP,
                                  const int index_alternate_order);

/**
 * @brief Translating an ideal into a representation of the corresponding
 * isogeny
 *
 * @param beta1 Output: quaternion element
 * @param beta2 Output: quaternion element
 * @param u Output: integer
 * @param v Output: integer
 * @param d1 Output: integer
 * @param d2 Output: integer
 * @param codomain the codomain of the isogeny corresponding to lideal
 * @param basis Output : evaluation of the canonical basis of E0 through the
 * ideal corresponding to lideal
 * @param lideal : O0 - ideal in input
 * @param Bpoo : the quaternion algebra
 * @returns 1 if the computation succeeded, 0 otherwise
 *
 * Compute the codomain and image on the basis of E0 of the isogeny
 * E0 -> codomain corresponding to lideal
 *
 * There is some integer e >= 0 such that
 * 2^e * u, 2^e * v,beta1, beta2, d1, d2 are the output of find_uv
 * on input target = 2^TORSION_PLUS_EVEN_POWER and lideal
 *
 * codomain and basis are computed with the help of a dimension 2 isogeny
 * of degree 2^TORSION_PLUS_EVEN_POWER - e using a Kani diagram
 *
 */
int dim2id2iso_ideal_to_isogeny_clapotis(quat_alg_elem_t *beta1,
                                         quat_alg_elem_t *beta2,
                                         ibz_t *u,
                                         ibz_t *v,
                                         ibz_t *d1,
                                         ibz_t *d2,
                                         ec_curve_t *codomain,
                                         ec_basis_t *basis,
                                         const quat_left_ideal_t *lideal,
                                         const quat_alg_t *Bpoo);

/**
 * @brief Translating an ideal into a representation of the corresponding
 * isogeny
 *
 * @param basis Output : evaluation of the canonical basis of E0 through the
 * ideal corresponding to lideal
 * @param lideal : ideal in input
 * @param codomain
 * @returns 1 if the computation succeeds, 0 otherwise
 *
 * This is a wrapper around the ideal to isogeny clapotis function
 */
int dim2id2iso_arbitrary_isogeny_evaluation(ec_basis_t *basis, ec_curve_t *codomain, const quat_left_ideal_t *lideal);

/** @}
 */

/** @}
 */

#endif
