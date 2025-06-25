#ifndef LLL_INTERNALS_H
#define LLL_INTERNALS_H

/** @file
 *
 * @authors Sina Schaeffler
 *
 * @brief Declarations of functions only used for the LLL tets
 */

#include <quaternion.h>

/** @internal
 * @ingroup quat_helpers
 * @defgroup lll_internal Functions only used for LLL or its tests
 * @{
 */

/** @internal
 * @ingroup lll_internal
 * @defgroup lll_params Parameters used by the L2 implementation (floats) and its tests (ints)
 * @{
 */

#define DELTABAR 0.995
#define DELTA_NUM 99
#define DELTA_DENOM 100

#define ETABAR 0.505
#define EPSILON_NUM 1
#define EPSILON_DENOM 100

#define PREC 64
/**
 * @}
 */

/** @internal
 * @ingroup lll_internal
 * @defgroup ibq_t Types for rationals
 * @{
 */

/** @brief Type for fractions of integers
 *
 * @typedef ibq_t
 *
 * For fractions of integers of arbitrary size, used by intbig module, using gmp
 */
typedef ibz_t ibq_t[2];
typedef ibq_t ibq_vec_4_t[4];
typedef ibq_t ibq_mat_4x4_t[4][4];

/**@}
 */

/** @internal
 * @ingroup lll_internal
 * @defgroup lll_ibq_c Constructors and Destructors and Printers
 * @{
 */

void ibq_init(ibq_t *x);
void ibq_finalize(ibq_t *x);

void ibq_mat_4x4_init(ibq_mat_4x4_t *mat);
void ibq_mat_4x4_finalize(ibq_mat_4x4_t *mat);

void ibq_vec_4_init(ibq_vec_4_t *vec);
void ibq_vec_4_finalize(ibq_vec_4_t *vec);

void ibq_mat_4x4_print(const ibq_mat_4x4_t *mat);
void ibq_vec_4_print(const ibq_vec_4_t *vec);

/** @}
 */

/** @internal
 * @ingroup lll_internal
 * @defgroup lll_qa Basic fraction arithmetic
 * @{
 */

/** @brief sum=a+b
 */
void ibq_add(ibq_t *sum, const ibq_t *a, const ibq_t *b);

/** @brief diff=a-b
 */
void ibq_sub(ibq_t *diff, const ibq_t *a, const ibq_t *b);

/** @brief neg=-x
 */
void ibq_neg(ibq_t *neg, const ibq_t *x);

/** @brief abs=|x|
 */
void ibq_abs(ibq_t *abs, const ibq_t *x);

/** @brief prod=a*b
 */
void ibq_mul(ibq_t *prod, const ibq_t *a, const ibq_t *b);

/** @brief inv=1/x
 *
 * @returns 0 if x is 0, 1 if inverse exists and was computed
 */
int ibq_inv(ibq_t *inv, const ibq_t *x);

/** @brief Compare a and b
 *
 * @returns a positive value if a > b, zero if a = b, and a negative value if a < b
 */
int ibq_cmp(const ibq_t *a, const ibq_t *b);

/** @brief Test if x is 0
 *
 * @returns 1 if x=0, 0 otherwise
 */
int ibq_is_zero(const ibq_t *x);

/** @brief Test if x is 1
 *
 * @returns 1 if x=1, 0 otherwise
 */
int ibq_is_one(const ibq_t *x);

/** @brief Set q to a/b if b not 0
 *
 * @returns 1 if b not 0 and q is set, 0 otherwise
 */
int ibq_set(ibq_t *q, const ibz_t *a, const ibz_t *b);

/** @brief Copy value into target
 */
void ibq_copy(ibq_t *target, const ibq_t *value);

/** @brief Checks if q is an integer
 *
 * @returns 1 if yes, 0 if not
 */
int ibq_is_ibz(const ibq_t *q);

/**
 * @brief Converts a fraction q to an integer y, if q is an integer.
 *
 * @returns 1 if z is an integer, 0 if not
 */
int ibq_to_ibz(ibz_t *z, const ibq_t *q);
/** @}
 */

/**  @internal
 * @ingroup lll_internal
 * @defgroup quat_lll_verify_helpers Helper functions for lll verification in dimension 4
 * @{
 */

/** @brief Set ibq to parameters delta and eta = 1/2 + epsilon using L2 constants
 */
void quat_lll_set_ibq_parameters(ibq_t *delta, ibq_t *eta);

/** @brief Set an ibq vector to 4 given integer coefficients
 */
void ibq_vec_4_copy_ibz(ibq_vec_4_t *vec,
                        const ibz_t *coeff0,
                        const ibz_t *coeff1,
                        const ibz_t *coeff2,
                        const ibz_t *coeff3); // dim4, test/dim4

/** @brief Bilinear form vec00*vec10+vec01*vec11+q*vec02*vec12+q*vec03*vec13 for ibz_q
 */
void quat_lll_bilinear(ibq_t *b, const ibq_vec_4_t *vec0, const ibq_vec_4_t *vec1,
                       const ibz_t *q); // dim4, test/dim4

/** @brief Outputs the transposition of the orthogonalised matrix of mat (as fractions)
 *
 * For the bilinear form vec00*vec10+vec01*vec11+q*vec02*vec12+q*vec03*vec13
 */
void quat_lll_gram_schmidt_transposed_with_ibq(ibq_mat_4x4_t *orthogonalised_transposed,
                                               const ibz_mat_4x4_t *mat,
                                               const ibz_t *q); // dim4

/** @brief Verifies if mat is lll-reduced for parameter coeff and norm defined by q
 *
 * For the bilinear form vec00*vec10+vec01*vec11+q*vec02*vec12+q*vec03*vec13
 */
int quat_lll_verify(const ibz_mat_4x4_t *mat,
                    const ibq_t *delta,
                    const ibq_t *eta,
                    const quat_alg_t *alg); // test/lattice, test/dim4
                                            /** @}
                                             */

/** @internal
 * @ingroup lll_internal
 * @defgroup lll_internal_gram Internal LLL function
 * @{
 */

/** @brief In-place L2 reduction core function
 *
 * Given a lattice basis represented by the columns of a 4x4 matrix
 * and the Gram matrix of its bilinear form, L2-reduces the basis
 * in-place and updates the Gram matrix accordingly.
 *
 * Implements the L2 Algorithm of Nguyen-Stehlé, also known as fplll:
 * https://iacr.org/archive/eurocrypt2005/34940217/34940217.pdf
 *
 * Parameters are in lll/lll_internals.h
 *
 * @param G In/Output: Gram matrix of the lattice basis
 * @param basis In/Output: lattice basis
 */
void quat_lll_core(ibz_mat_4x4_t *G, ibz_mat_4x4_t *basis);

/**
 * @brief LLL reduction on 4-dimensional lattice
 *
 * Implements the L2 Algorithm of Nguyen-Stehlé, also known as fplll:
 * https://iacr.org/archive/eurocrypt2005/34940217/34940217.pdf
 *
 * Parameters are in lll/lll_internals.h
 *
 * @param red Output: LLL reduced basis
 * @param lattice In/Output:  lattice with 4-dimensional basis
 * @param alg The quaternion algebra
 */
int quat_lattice_lll(ibz_mat_4x4_t *red, const quat_lattice_t *lattice, const quat_alg_t *alg);

/**
 * @}
 */

// end of lll_internal
/** @}
 */
#endif
