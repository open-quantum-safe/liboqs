/** @file
 *
 * @authors Max Duparc, Antonin Leroux
 *
 * @brief The HD-isogenies algorithm required by the signature
 *
 */

#ifndef HD_H
#define HD_H

#include <sqisign_namespace.h>
#include <ec.h>
#include <stdio.h>

/** @defgroup hd_module Abelian surfaces and their isogenies
 * @{
 */

/** @defgroup hd_struct Data structures for dimension 2
 * @{
 */

/** @brief Type for couple point with XZ coordinates
 * @typedef theta_couple_point_t
 *
 * @struct theta_couple_point
 *
 * Structure for the couple point on an elliptic product using XZ coordinates
 */
typedef struct theta_couple_point
{
    ec_point_t P1;
    ec_point_t P2;
} theta_couple_point_t;

/** @brief Type for two couple points T1, T2 with XZ coordinates
 * @typedef theta_kernel_couple_points_t
 *
 * @struct theta_kernel_couple_points
 *
 * Structure for a couple of theta couple points T1 and T2
 */
typedef struct theta_kernel_couple_points
{
    theta_couple_point_t T1;
    theta_couple_point_t T2;
} theta_kernel_couple_points_t;

/** @brief Type for couple curve *
 * @typedef theta_couple_curve_t
 *
 * @struct theta_couple_curve
 *
 * the  theta_couple_curve structure
 */
typedef struct theta_couple_curve
{
    ec_curve_t E1;
    ec_curve_t E2;
} theta_couple_curve_t;

/** @brief Type for a product E1 x E2 with corresponding bases
 * @typedef theta_couple_curve_with_basis_t
 *
 * @struct theta_couple_curve_with_basis
 *
 * tType for a product E1 x E2 with corresponding bases Ei[2^n]
 */
typedef struct theta_couple_curve_with_basis
{
    ec_curve_t E1;
    ec_curve_t E2;
    ec_basis_t B1;
    ec_basis_t B2;
} theta_couple_curve_with_basis_t;

/** @brief Type for theta point *
 * @typedef theta_point_t
 *
 * @struct theta_point
 *
 * the  theta_point structure used
 */
typedef struct theta_point
{
    fp2_t x;
    fp2_t y;
    fp2_t z;
    fp2_t t;
} theta_point_t;

/** @brief Type for theta structure *
 * @typedef theta_structure_t
 *
 * @struct theta_structure
 *
 * the  theta_structure structure used
 */
typedef struct theta_structure
{
    theta_point_t inv_dual_null_point;
    theta_point_t dbl_data;
    theta_point_t inv_sqr_null_point;

    bool precomputation;
} theta_structure_t;

/** @brief Type for standard (2,2) theta isogeny *
 * @typedef theta_isogeny_t
 *
 * @struct theta_isogeny
 *
 * the theta_isogeny structure
 */
typedef struct theta_isogeny
{
    theta_structure_t codomain;

} theta_isogeny_t;

// end of hd_struct
/**
 * @}
 */

/** @defgroup hd_couple Couple of points
 * @{
 */

/**
 * @brief Compute the double of the theta couple point in on the elliptic product E12
 *
 * @param out Output: the theta_couple_point
 * @param in the theta couple point in the elliptic product
 * @param E1E2 an elliptic product
 * in = (P1,P2)
 * out = [2] (P1,P2)
 *
 */
void theta_DBL_couple_point(theta_couple_point_t *out,
                            const theta_couple_point_t *in,
                            const theta_couple_curve_t *E1E2);

/**
 * @brief Compute the iterated double of the theta couple point in on the elliptic product E12
 *
 * @param out Output: the theta_couple_point
 * @param n : the number of iteration
 * @param E1E2 an elliptic product
 * @param in the theta couple point in the elliptic product
 * in = (P1,P2)
 * out = [2^n] (P1,P2)
 *
 */
void theta_DBL_couple_point_iter(theta_couple_point_t *out,
                                 uint16_t n,
                                 const theta_couple_point_t *in,
                                 const theta_couple_curve_t *E1E2);

// end of hd_couple
/**
 * @}
 */

/** @defgroup hd_chain HD chains
 * @{
 */

/**
 * @brief Compute a (2,2) isogeny chain in dimension 2 between elliptic products in the theta_model and evaluate at a
 * list of points of the form (P1,0) or (0,P2). Returns 0 if the codomain fails to split (or there is an error during
 * the computation) and 1 otherwise.
 *
 * @param n : the length of the isogeny chain
 * @param E12 an elliptic curve product
 * @param ker T1, T2 and T1-T2. couple points on E12[2^(n+2)]
 * @param E34 Output: the codomain curve
 * @param P12 Input/Output: pointer to points to be pushed through the isogeny (in-place)
 * @param numP: length of the list of points given in P12 (can be zero)
 * @returns 1 on success 0 on failure
 *
 */
int theta_chain_compute_and_eval(uint16_t n,
                                 /*const*/ theta_couple_curve_t *E12,
                                 const theta_kernel_couple_points_t *ker,
                                 theta_couple_curve_t *E34,
                                 theta_couple_point_t *P12,
                                 uint8_t numP);

/**
 * @brief Compute a (2,2) isogeny chain in dimension 2 between elliptic products in the theta_model and evaluate at a
 * list of points of the form (P1,0) or (0,P2). Returns 0 if the codomain fails to split (or there is an error during
 * the computation) and 1 otherwise.
 * This function only computes half the curves andf points (those related to E3) For the generic function, use
 * theta_chain_compute_and_eval instead.
 *
 * @param n : the length of the isogeny chain
 * @param E12 an elliptic curve product
 * @param ker T1, T2 and T1-T2. couple points on E12[2^(n+2)]
 * @param E34 Output: the codomain curve. Only E3 was computed.
 * @param P12 Input/Output: pointer to points to be pushed through the isogeny. Only points on E3 where computed
 * (in-place)
 * @param numP: length of the list of points given in P12 (can be zero)
 * @returns 1 on success 0 on failure
 *
 */
int theta_chain_compute_and_eval_E1(uint16_t n,
                                    /*const*/ theta_couple_curve_t *E12,
                                    const theta_kernel_couple_points_t *ker,
                                    theta_couple_curve_t *E34,
                                    theta_couple_point_t *P12,
                                    uint8_t numP);

/**
 * @brief Compute a (2,2) isogeny chain in dimension 2 between elliptic products in the theta_model and evaluate at a
 * list of points of the form (P1,0) or (0,P2). Returns 0 if the codomain fails to split (or there is an error during
 * the computation) and 1 otherwise.
 * This function only computes half the curves andf points (those related to E4) For the generic function, use
 * theta_chain_compute_and_eval instead.
 *
 * @param n : the length of the isogeny chain
 * @param E12 an elliptic curve product
 * @param ker T1, T2 and T1-T2. couple points on E12[2^(n+2)]
 * @param E34 Output: the codomain curve. Only E4 was computed.
 * @param P12 Input/Output: pointer to points to be pushed through the isogeny. Only points on E4 where computed
 *            (in-place)
 * @param numP: length of the list of points given in P12 (can be zero)
 * @returns 1 on success 0 on failure
 *
 */
int theta_chain_compute_and_eval_E2(uint16_t n,
                                    /*const*/ theta_couple_curve_t *E12,
                                    const theta_kernel_couple_points_t *ker,
                                    theta_couple_curve_t *E34,
                                    theta_couple_point_t *P12,
                                    uint8_t numP);

/**
 * @brief Compute a (2,2) isogeny chain in dimension 2 between elliptic products in the theta_model and evaluate at a
 * list of points of the form (P1,0) or (0,P2). Returns 0 if the codomain fails to split (or there is an error during
 * the computation) and 1 otherwise.
 * Compared to theta_chain_compute_and_eval, it does extra isotropy checks on the kernel.
 *
 * @param n : the length of the isogeny chain
 * @param E12 an elliptic curve product
 * @param ker T1, T2 and T1-T2. couple points on E12[2^(n+2)]
 * @param E34 Output: the codomain curve
 * @param P12 Input/Output: pointer to points to be pushed through the isogeny (in-place)
 * @param numP: length of the list of points given in P12 (can be zero)
 * @returns 1 on success 0 on failure
 *
 */
int theta_chain_compute_and_eval_verify(uint16_t n,
                                        /*const*/ theta_couple_curve_t *E12,
                                        const theta_kernel_couple_points_t *ker,
                                        theta_couple_curve_t *E34,
                                        theta_couple_point_t *P12,
                                        uint8_t numP);

/**
 * @brief Given a bases B1 on E1 and B2 on E2 copies this to create a kernel
 *         on E1 x E2 as couple points T1, T2.
 *
 * @param ker Output: a kernel for dim_two_isogenies (T1, T2)
 * @param B1 Input basis on E1
 * @param B2 Input basis on E2
 **/
void theta_copy_bases_to_kernel(theta_kernel_couple_points_t *ker, const ec_basis_t *B1, const ec_basis_t *B2);

#ifndef NDEBUG
/**
 * @brief Given a couple of points (P1, P2) on a couple of curves (E1, E2) this function tests if both points are of
 * order exactly 2^t
 *
 * @param T: couple point (P1, P2)
 * @param E: a couple of curves (E1, E2)
 * @param t: an integer
 * @returns 0xFFFFFFFF on success, 0 on failure
 */
static inline int
test_couple_point_order_twof(const theta_couple_point_t *T, const theta_couple_curve_t *E, uint16_t t)
{
    int check_P1 = test_point_order_twof(&T->P1, &E->E1, t);
    int check_P2 = test_point_order_twof(&T->P2, &E->E2, t);

    return check_P1 & check_P2;
}
#endif

// end of hd_functions
/**
 * @}
 */
// end of hd_module
/**
 * @}
 */
#endif
