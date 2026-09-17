/** @file
 *
 * @authors Antonin Leroux, Sina Schaeffler, Max Duparc
 *
 * @brief The id2iso algorithms
 */

#ifndef ID2ISO_H
#define ID2ISO_H

#include <biextension.h>
#include <ec.h>
#include <hd.h>
#include <quaternion.h>
#include <quaternion_constants.h>
#include <quaternion_data.h>
#include <sqisign_namespace.h>

/** @defgroup id2iso_id2iso Ideal to isogeny conversion
 * @{
 */

/** @defgroup id2iso_others Helpers functions
 * @{
 */

/**
 * @brief Applies some 2x2 matrix on a basis of E[2^TORSION_EVEN_POWER]
 *
 * @param P the basis
 * @param E the curve
 * @param mat the matrix
 * @param f TORSION_EVEN_POWER
 * @param last_point If 0, 3rd point of basis is not computed
 * @returns 1 if success, 0 if error
 *
 * helper function, works in place
 *
 */
int id2iso_matrix_application_even_basis(ec_basis_t *P,
                                         const ec_curve_t *E,
                                         ibz_mat_2x2_t *mat,
                                         int f,
                                         bool last_point);

/**
 * @brief Applies some endomorphism of an alternate curve to E[f]
 *
 * @param P the basis
 * @param E the curve (E is not required to be the alternate curve in question since in the end we only apply a matrix)
 * @param theta the endomorphism
 * @param f TORSION_EVEN_POWER
 * @param last_point If 0, 3rd point of basis is not computed
 *
 * helper function, works in place
 *
 */
void id2iso_endomorphism_application_even_basis(ec_basis_t *P,
                                                const ec_curve_t *E,
                                                const quat_alg_elem_t *theta,
                                                int f,
                                                bool last_point);

/**
 * @brief Translating a kernel on the curve E0,
 * represented as a vector with respect to the precomputed 2^f-torsion basis, into the corresponding O0-ideal
 *
 * @param ideal Output : the inert part of the output O0-ideal
 * @param split Output : the split part of the output O0-ideal
 * @param f : exponent definining the norm of the ideal to compute
 * @param vec2 : length-2 vector giving the 2-power part of the kernel with respect to the precomputed 2^f basis
 *
 */
void id2iso_kernel_dlogs_to_ideal_even(quat_ideal_t *ideal, quat_alg_elem_t *split, const ibz_vec_2_t *vec2, int f);

/**
 * @brief Change of basis matrix for full basis B2
 *
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
void id2iso_change_of_basis_matrix_tate(ibz_mat_2x2_t *mat,
                                        const ec_basis_t *B1,
                                        const ec_basis_t *B2,
                                        ec_curve_t *E,
                                        int f);

/**
 * @brief Change of basis matrix for full basis B2
 *
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
 * mat encodes the coordinates of the points of B1 in the basis B2, by applying id2iso_change_of_basis_matrix_tate and
 * inverting the outcome
 */
void id2iso_change_of_basis_matrix_tate_invert(ibz_mat_2x2_t *mat,
                                               const ec_basis_t *B1,
                                               const ec_basis_t *B2,
                                               ec_curve_t *E,
                                               int f);

/** @}
 */

/** @defgroup id2iso_Qalpoty Id2Iso functions
 * @{
 */
/**
 * @brief Translating an ideal into a representation of the corresponding isogeny
 *
 * @param beta1 Output: quaternion element
 * @param d1 Output: integer (norm of beta1)
 * @param codomain the codomain of the isogeny corresponding to ideal
 * @param basis Output : evaluation of the canonical basis of E0 through the ideal corresponding to ideal
 * @param ideal : O0 - ideal in input
 * @returns 1 if the computation succeeded, 0 otherwise
 *
 * Compute the codomain and image on the basis of E0 of the isogeny E0 -> codomain corresponding to ideal.
 * beta1 and beta2 are element of ideal output of Qlapoty norm equation with relative norm d1 and d2 such that
 * d_1 + d_2 = target, with target = 2^(TORSION_PLUS_EVEN_POWER-2).
 * codomain and basis are computed with the help of a dimension 2 isogeny of degree target using a Kani diagram
 *
 */
int dim2id2iso_ideal_to_isogeny_qlapoty(quat_alg_elem_t *beta1,
                                        ibz_t *d1,
                                        ec_curve_t *codomain,
                                        ec_basis_t *basis,
                                        const quat_ideal_t *ideal);

/**
 * @brief Translating an ideal into a representation of the corresponding isogeny
 *
 * @param basis Output : evaluation of the canonical basis of E0 through the ideal corresponding to ideal
 * @param ideal : ideal in input
 * @param codomain
 * @returns 1 if the computation succeeds, 0 otherwise
 *
 * This is a wrapper around the ideal to isogeny qlapoty function
 */
int dim2id2iso_arbitrary_isogeny_evaluation(ec_basis_t *basis, ec_curve_t *codomain, const quat_ideal_t *ideal);

/** @}
 */
/** @}
 */

#endif
