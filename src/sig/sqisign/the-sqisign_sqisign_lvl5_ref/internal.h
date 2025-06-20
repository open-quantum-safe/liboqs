/** @file
 *
 * @authors Sina Schaeffler
 *
 * @brief Declarations for helper functions for quaternion algebra implementation
 */

#ifndef QUAT_HELPER_H
#define QUAT_HELPER_H

#include <quaternion.h>
#include <assert.h>
#include "intbig_internal.h"

/** @internal
 * @ingroup quat_quat
 * @defgroup quat_helpers Quaternion module internal functions
 * @{
 */

/**  @internal
 * @defgroup quat_alg_helpers Helper functions for the alg library
 * @{
 */

/**  @internal
 * @brief helper function for initializing small quaternion algebras.
 */
void quat_alg_init_set_ui(quat_alg_t *alg,
                          unsigned int p); // test/lattice, test/ideal, test/algebra

/** @brief a*b
 *
 * Multiply two coordinate vectors as elements of the algebra in basis (1,i,j,ij) with i^2 = -1, j^2
 * = -p
 *
 * @param res Output: Will contain product
 * @param a
 * @param b
 * @param alg The quaternion algebra
 */
void quat_alg_coord_mul(ibz_vec_4_t *res, const ibz_vec_4_t *a, const ibz_vec_4_t *b, const quat_alg_t *alg);

/** @brief a=b
 *
 * Test if a and b represent the same quaternion algebra element
 *
 * @param a
 * @param b
 * @returns 1 if a=b, 0 otherwise
 */
int quat_alg_elem_equal(const quat_alg_elem_t *a, const quat_alg_elem_t *b);

/** @brief Test if x is 0
 *
 * @returns 1 if x=0, 0 otherwise
 *
 * x is 0 iff all coordinates in x->coord are 0
 */
int quat_alg_elem_is_zero(const quat_alg_elem_t *x);

/** @brief Compute same denominator form of two quaternion algebra elements
 *
 * res_a=a and res_b=b (representing the same element) and res_a.denom = res_b.denom
 *
 * @param res_a
 * @param res_b
 * @param a
 * @param b
 */
void quat_alg_equal_denom(quat_alg_elem_t *res_a,
                          quat_alg_elem_t *res_b,
                          const quat_alg_elem_t *a,
                          const quat_alg_elem_t *b);

/** @brief Copies the given values into an algebra element, without normalizing it
 *
 * @param elem Output: algebra element of coordinates [coord0,coord1,coord2,coord3] and denominator
 * denom
 * @param denom Denominator, must be non zero
 * @param coord0 Coordinate on 1 (0th vector of standard algebra basis)
 * @param coord1 Coordinate on i (1st vector of standard algebra basis)
 * @param coord2 Coordinate on j (2nd vector of standard algebra basis)
 * @param coord3 Coordinate on ij (3rd vector of standard algebra basis)
 */
void quat_alg_elem_copy_ibz(quat_alg_elem_t *elem,
                            const ibz_t *denom,
                            const ibz_t *coord0,
                            const ibz_t *coord1,
                            const ibz_t *coord2,
                            const ibz_t *coord3);

/** @brief Sets an algebra element to the given integer values, without normalizing it
 *
 * @param elem Output: algebra element of coordinates [coord0,coord1,coord2,coord3] and denominator
 * denom
 * @param denom Denominator, must be non zero
 * @param coord0 Coordinate on 1 (0th vector of standard algebra basis)
 * @param coord1 Coordinate on i (1st vector of standard algebra basis)
 * @param coord2 Coordinate on j (2nd vector of standard algebra basis)
 * @param coord3 Coordinate on ij (3rd vector of standard algebra basis)
 */
void quat_alg_elem_set(quat_alg_elem_t *elem,
                       int32_t denom,
                       int32_t coord0,
                       int32_t coord1,
                       int32_t coord2,
                       int32_t coord3);

/**
 * @brief Creates algebra element from scalar
 *
 * Resulting element has 1-coordinate equal to numerator/denominator
 *
 * @param elem Output: algebra element with numerator/denominator as first coordiante
 * (1-coordinate), 0 elsewhere (i,j,ij coordinates)
 * @param numerator
 * @param denominator Assumed  non zero
 */
void quat_alg_scalar(quat_alg_elem_t *elem, const ibz_t *numerator, const ibz_t *denominator);

/** @brief a+b for algebra elements
 *
 * @param res Output
 * @param a Algebra element
 * @param b Algebra element
 */
void quat_alg_add(quat_alg_elem_t *res, const quat_alg_elem_t *a, const quat_alg_elem_t *b);

/** @brief a-b for algebra elements
 *
 * @param res Output
 * @param a Algebra element
 * @param b Algebra element
 */
void quat_alg_sub(quat_alg_elem_t *res, const quat_alg_elem_t *a, const quat_alg_elem_t *b);

/** @brief Multiplies algebra element by integer scalar, without normalizing it
 *
 * @param res Output
 * @param scalar Integer
 * @param elem Algebra element
 */
void quat_alg_elem_mul_by_scalar(quat_alg_elem_t *res, const ibz_t *scalar, const quat_alg_elem_t *elem);

/** @}
 */

/**  @internal
 * @defgroup quat_dim4_helpers Helper functions for functions for matrices or vectors in dimension 4
 * @{
 */

/**  @internal
 * @defgroup quat_inv_helpers Helper functions for the integer matrix inversion function
 * @{
 */

/** @brief a1a2+b1b2+c1c2
 *
 * @param coeff Output: The coefficien which was computed as a1a2+b1b2-c1c2
 * @param a1
 * @param a2
 * @param b1
 * @param b2
 * @param c1
 * @param c2
 */
void ibz_inv_dim4_make_coeff_pmp(ibz_t *coeff,
                                 const ibz_t *a1,
                                 const ibz_t *a2,
                                 const ibz_t *b1,
                                 const ibz_t *b2,
                                 const ibz_t *c1,
                                 const ibz_t *c2);

/** @brief -a1a2+b1b2-c1c2
 *
 * @param coeff Output: The coefficien which was computed as -a1a2+b1b2-c1c2
 * @param a1
 * @param a2
 * @param b1
 * @param b2
 * @param c1
 * @param c2
 */
void ibz_inv_dim4_make_coeff_mpm(ibz_t *coeff,
                                 const ibz_t *a1,
                                 const ibz_t *a2,
                                 const ibz_t *b1,
                                 const ibz_t *b2,
                                 const ibz_t *c1,
                                 const ibz_t *c2);

/** @brief Matrix determinant and a matrix inv such that inv/det is the inverse matrix of the input
 *
 * Implemented following the methof of 2x2 minors explained at Method from
 * https://www.geometrictools.com/Documentation/LaplaceExpansionTheorem.pdf (visited on 3rd of May
 * 2023, 16h15 CEST)
 *
 * @returns 1 if the determinant of mat is not 0 and an inverse was computed, 0 otherwise
 * @param inv Output: Will contain an integer matrix which, dividet by det, will yield the rational
 * inverse of the matrix if it exists, can be NULL
 * @param det Output: Will contain the determinant of the input matrix, can be NULL
 * @param mat Matrix of which the inverse will be computed
 */
int ibz_mat_4x4_inv_with_det_as_denom(ibz_mat_4x4_t *inv, ibz_t *det, const ibz_mat_4x4_t *mat);

/** @}
 */

/**  @internal
 * @defgroup quat_dim4_lat_helpers Helper functions on vectors and matrices used mainly for lattices
 * @{
 */

/** @brief Copy all values from one vector to another
 *
 * @param new Output: is set to same values as vec
 * @param vec
 */
void ibz_vec_4_copy(ibz_vec_4_t *new, const ibz_vec_4_t *vec);

/** @brief set res to values coord0,coord1,coord2,coord3
 *
 * @param res Output: Will contain vector (coord0,coord1,coord2,coord3)
 * @param coord0
 * @param coord1
 * @param coord2
 * @param coord3
 */
void ibz_vec_4_copy_ibz(ibz_vec_4_t *res,
                        const ibz_t *coord0,
                        const ibz_t *coord1,
                        const ibz_t *coord2,
                        const ibz_t *coord3);

/** @brief Set a vector of 4 integers to given values
 *
 * @param vec Output: is set to given coordinates
 * @param coord0
 * @param coord1
 * @param coord2
 * @param coord3
 */
void ibz_vec_4_set(ibz_vec_4_t *vec, int32_t coord0, int32_t coord1, int32_t coord2, int32_t coord3);

/** @brief a+b
 *
 * Add two integer 4-vectors
 *
 * @param res Output: Will contain sum
 * @param a
 * @param b
 */
void ibz_vec_4_add(ibz_vec_4_t *res, const ibz_vec_4_t *a, const ibz_vec_4_t *b);

/** @brief a-b
 *
 * Substract two integer 4-vectors
 *
 * @param res Output: Will contain difference
 * @param a
 * @param b
 */
void ibz_vec_4_sub(ibz_vec_4_t *res, const ibz_vec_4_t *a, const ibz_vec_4_t *b);

/** @brief x=0
 *
 * Test if a vector x has only zero coordinates
 *
 * @returns 0 if x has at least one non-zero coordinates, 1 otherwise
 * @param x
 */
int ibz_vec_4_is_zero(const ibz_vec_4_t *x);

/** @brief Compute the linear combination lc = coeff_a vec_a + coeff_b vec_b
 *
 * @param lc Output: linear combination lc = coeff_a vec_a + coeff_b vec_b
 * @param coeff_a Scalar multiplied to vec_a
 * @param vec_a
 * @param coeff_b Scalar multiplied to vec_b
 * @param vec_b
 */
void ibz_vec_4_linear_combination(ibz_vec_4_t *lc,
                                  const ibz_t *coeff_a,
                                  const ibz_vec_4_t *vec_a,
                                  const ibz_t *coeff_b,
                                  const ibz_vec_4_t *vec_b);

/** @brief multiplies all values in vector by same scalar
 *
 * @param prod Output
 * @param scalar
 * @param vec
 */
void ibz_vec_4_scalar_mul(ibz_vec_4_t *prod, const ibz_t *scalar, const ibz_vec_4_t *vec);

/** @brief divides all values in vector by same scalar
 *
 * @returns 1 if scalar divided all values in mat, 0 otherwise (division is performed in both cases)
 * @param quot Output
 * @param scalar
 * @param vec
 */
int ibz_vec_4_scalar_div(ibz_vec_4_t *quot, const ibz_t *scalar, const ibz_vec_4_t *vec);

/** @brief Negation for vectors of 4 integers
 *
 * @param neg Output: is set to -vec
 * @param vec
 */
void ibz_vec_4_negate(ibz_vec_4_t *neg, const ibz_vec_4_t *vec);

/**
 * @brief content of a 4-vector of integers
 *
 * The content is the GCD of all entries.
 *
 * @param v A 4-vector of integers
 * @param content Output: the resulting gcd
 */
void ibz_vec_4_content(ibz_t *content, const ibz_vec_4_t *v);

/** @brief -mat for mat a 4x4 integer matrix
 *
 * @param neg Output: is set to -mat
 * @param mat Input matrix
 */
void ibz_mat_4x4_negate(ibz_mat_4x4_t *neg, const ibz_mat_4x4_t *mat);

/** @brief Set all coefficients of a matrix to zero for 4x4 integer matrices
 *
 * @param zero
 */
void ibz_mat_4x4_zero(ibz_mat_4x4_t *zero);

/** @brief Set a matrix to the identity for 4x4 integer matrices
 *
 * @param id
 */
void ibz_mat_4x4_identity(ibz_mat_4x4_t *id);

/** @brief Test equality to identity for 4x4 integer matrices
 *
 * @returns 1 if mat is the identity matrix, 0 otherwise
 * @param mat
 */
int ibz_mat_4x4_is_identity(const ibz_mat_4x4_t *mat);

/** @brief Equality test for 4x4 integer matrices
 *
 * @returns 1 if equal, 0 otherwise
 * @param mat1
 * @param mat2
 */
int ibz_mat_4x4_equal(const ibz_mat_4x4_t *mat1, const ibz_mat_4x4_t *mat2);

/** @brief Copies all values from a 4x4 integer matrix to another one
 *
 * @param new Output: matrix which will have its entries set to mat's entries
 * @param mat Input matrix
 */
void ibz_mat_4x4_copy(ibz_mat_4x4_t *new, const ibz_mat_4x4_t *mat);

/** @brief Matrix by integer multiplication
 *
 * @param prod Output
 * @param scalar
 * @param mat
 */
void ibz_mat_4x4_scalar_mul(ibz_mat_4x4_t *prod, const ibz_t *scalar, const ibz_mat_4x4_t *mat);

/** @brief gcd of all values in matrix
 *
 * @param gcd Output
 * @param mat
 */
void ibz_mat_4x4_gcd(ibz_t *gcd, const ibz_mat_4x4_t *mat);

/** @brief Verifies whether the 4x4 input matrix is in Hermite Normal Form
 *
 * @returns 1 if mat is in HNF, 0 otherwise
 * @param mat Matrix to be tested
 */
int ibz_mat_4x4_is_hnf(const ibz_mat_4x4_t *mat);

/** @brief Hermite Normal Form of a matrix of 8 integer vectors, computed using a multiple of its
 * determinant as modulo
 *
 * Algorithm used is the one at number 2.4.8 in Henri Cohen's "A Course in Computational Algebraic
 * Number Theory" (Springer Verlag, in series "Graduate texts in Mathematics") from 1993
 *
 * @param hnf Output: Matrix in Hermite Normal Form generating the same lattice as generators
 * @param generators matrix whose colums generate the same lattice than the output
 * @param generator_number number of generators given
 * @param mod integer, must be a multiple of the volume of the lattice generated by the columns of
 * generators
 */
void ibz_mat_4xn_hnf_mod_core(ibz_mat_4x4_t *hnf,
                              int generator_number,
                              const ibz_vec_4_t *generators,
                              const ibz_t *mod);

/** @}
 */
/** @}
 */

/**  @internal
 * @defgroup quat_dim2_helpers Helper functions for dimension 2
 * @{
 */

/** @brief Set vector coefficients to the given integers
 *
 * @param vec Output: Vector
 * @param a0
 * @param a1
 */
void ibz_vec_2_set(ibz_vec_2_t *vec, int a0, int a1); // test/dim2

/** @brief Set matrix coefficients to the given integers
 *
 * @param mat Output: Matrix
 * @param a00
 * @param a01
 * @param a10
 * @param a11
 */
void ibz_mat_2x2_set(ibz_mat_2x2_t *mat, int a00, int a01, int a10, int a11); // test/dim2

void ibz_mat_2x2_add(ibz_mat_2x2_t *sum, const ibz_mat_2x2_t *a,
                     const ibz_mat_2x2_t *b); // unused

/** @brief Determinant of a 2x2 integer matrix given as 4 integers
 *
 * @param det Output: Determinant of the matrix
 * @param a11 matrix coefficient (upper left corner)
 * @param a12 matrix coefficient (upper right corner)
 * @param a21 matrix coefficient (lower left corner)
 * @param a22 matrix coefficient (lower right corner)
 */
void ibz_mat_2x2_det_from_ibz(ibz_t *det,
                              const ibz_t *a11,
                              const ibz_t *a12,
                              const ibz_t *a21,
                              const ibz_t *a22); // dim4

/**
 * @brief a*b for 2x2 integer matrices modulo m
 *
 * @param prod Output matrix
 * @param mat_a Input matrix
 * @param mat_b Input matrix
 * @param m Integer modulo
 */
void ibz_2x2_mul_mod(ibz_mat_2x2_t *prod,
                     const ibz_mat_2x2_t *mat_a,
                     const ibz_mat_2x2_t *mat_b,
                     const ibz_t *m); // test/dim2
/** @}
 */

/**  @internal
 * @defgroup quat_lattice_helper Helper functions for the lattice library (dimension 4)
 * @{
 */

/**
 * @brief Modifies a lattice to put it in hermite normal form
 *
 * In-place modification of the lattice.
 *
 * @param lat input lattice
 *
 * On a correct lattice this function changes nothing (since it is already in HNF), but it can be
 * used to put a handmade one in correct form in order to use the other lattice functions.
 */
void quat_lattice_hnf(quat_lattice_t *lat); // lattice, test/lattice, test/algebra,

/**
 * @brief Lattice equality
 *
 * Lattice bases are assumed to be under HNF, but denominators are free.
 *
 * @returns 1 if both lattices are equal, 0 otherwise
 * @param lat1
 * @param lat2
 */
int quat_lattice_equal(const quat_lattice_t *lat1,
                       const quat_lattice_t *lat2); // ideal, lattice, test/lattice, test/ideal

/**
 * @brief Lattice inclusion test
 *
 * Lattice bases are assumed to be under HNF, but denominators are free.
 *
 * @returns 1 if sublat is included in overlat, 0 otherwise
 * @param sublat Lattice whose inclusion in overlat will be testes
 * @param overlat
 */
int quat_lattice_inclusion(const quat_lattice_t *sublat,
                           const quat_lattice_t *overlat); // test/lattice, test/ideal

/** @brief Divides basis and denominator of a lattice by their gcd
 *
 * @param reduced Output
 * @param lat Lattice
 */
void quat_lattice_reduce_denom(quat_lattice_t *reduced,
                               const quat_lattice_t *lat); // lattice, ideal,

/** @brief a+b for lattices
 *
 * @param res Output
 * @param lat1 Lattice
 * @param lat2 Lattice
 */
void quat_lattice_add(quat_lattice_t *res,
                      const quat_lattice_t *lat1,
                      const quat_lattice_t *lat2); // ideal, lattice, test/lattice

/** @brief a*b for lattices
 *
 * @param res Output
 * @param lat1 Lattice
 * @param lat2 Lattice
 * @param alg The quaternion algebra
 */
void quat_lattice_mul(quat_lattice_t *res,
                      const quat_lattice_t *lat1,
                      const quat_lattice_t *lat2,
                      const quat_alg_t *alg); // ideal, lattie, test/ideal, test/lattice

/**
 * @brief Computes the dual lattice of lat, without putting its basis in HNF
 *
 * This function returns a lattice not under HNF. For careful internal use only.
 *
 * Computation method described in https://cseweb.ucsd.edu/classes/sp14/cse206A-a/lec4.pdf consulted
 * on 19 of May 2023, 12h40 CEST
 *
 * @param dual Output: The dual lattice of lat. ATTENTION: is not under HNF. hnf computation must be
 * applied before using lattice functions on it
 * @param lat lattice, the dual of it will be computed
 */
void quat_lattice_dual_without_hnf(quat_lattice_t *dual,
                                   const quat_lattice_t *lat); // lattice, ideal

/**
 * @brief Multiply all columns of lat with coord (as algebra elements)
 *
 * The columns and coord are seen as algebra elements in basis 1,i,j,ij, i^2 = -1, j^2 = -p). Coord
 * is multiplied to the right of lat.
 *
 * The output matrix is not under HNF.
 *
 * @param prod Output: Matrix not under HND whose columns represent the algebra elements obtained as
 * L*coord for L column of lat.
 * @param lat Matrix whose columns are algebra elements in basis (1,i,j,ij)
 * @param coord Integer coordinate algebra element in basis (1,i,j,ij)
 * @param alg The quaternion algebra
 */
void quat_lattice_mat_alg_coord_mul_without_hnf(ibz_mat_4x4_t *prod,
                                                const ibz_mat_4x4_t *lat,
                                                const ibz_vec_4_t *coord,
                                                const quat_alg_t *alg); // lattice

/** @brief The index of sublat into overlat
 *
 * Assumes inputs are in HNF.
 *
 * @param index Output
 * @param sublat A lattice in HNF, must be sublattice of overlat
 * @param overlat A lattice in HNF, must be overlattice of sublat
 */
void quat_lattice_index(ibz_t *index, const quat_lattice_t *sublat,
                        const quat_lattice_t *overlat); // ideal

/** @brief Compute the Gram matrix of the quaternion trace bilinear form
 *
 * Given a lattice of the quaternion algebra, computes the Gram matrix
 * of the bilinear form
 *
 *      〈a,b〉 := [lattice->denom^2] Tr(a·conj(b))
 *
 * multiplied by the square of the denominator of the lattice.
 *
 * This matrix always has integer entries.
 *
 * @param G Output: Gram matrix of the trace bilinear form on the lattice, multiplied by the square
 * of the denominator of the lattice
 * @param lattice A lattice
 * @param alg The quaternion algebra
 */
void quat_lattice_gram(ibz_mat_4x4_t *G, const quat_lattice_t *lattice, const quat_alg_t *alg);

/**
 * @brief Compute an integer parallelogram containing the ball of
 * given radius for the positive definite quadratic form defined by
 * the Gram matrix G.
 *
 * The computed parallelogram is defined by the vectors
 *
 *      (x₁ x₂ x₃ x₄) · U
 *
 * with x_i ∈ [ -box[i], box[i] ].
 *
 * @param box Output: bounds of the parallelogram
 * @param U Output: Unimodular transformation defining the parallelogram
 * @param G Gram matrix of the quadratic form, must be full rank
 * @param radius Radius of the ball, must be non-negative
 * @returns 0 if the box only contains the origin, 1 otherwise
 */
int quat_lattice_bound_parallelogram(ibz_vec_4_t *box, ibz_mat_4x4_t *U, const ibz_mat_4x4_t *G, const ibz_t *radius);

/** @}
 */

/**  @internal
 * @defgroup quat_lideal_helper Helper functions for ideals and orders
 * @{
 */
/** @brief Set norm of an ideal given its lattice and parent order
 *
 * @param lideal In/Output: Ideal which has lattice and parent_order correctly set, but not
 * necessarily the norm. Will have norm correctly set too.
 */
void quat_lideal_norm(quat_left_ideal_t *lideal); // ideal

/**
 * @brief Left principal ideal of order, generated by x
 *
 * @param lideal Output: left ideal
 * @param alg quaternion algebra
 * @param order maximal order of alg whose left ideal is searched
 * @param x generating element
 *
 * Creates the left ideal in 'order' generated by the element 'x'
 */
void quat_lideal_create_principal(quat_left_ideal_t *lideal,
                                  const quat_alg_elem_t *x,
                                  const quat_lattice_t *order,
                                  const quat_alg_t *alg); // ideal, test/ideal

/**
 * @brief  Equality test for left ideals
 *
 * @returns 1 if both left ideals are equal, 0 otherwise
 * @param lideal1 left ideal
 * @param lideal2 left ideal
 * @param alg the quaternion algebra
 */
int quat_lideal_equals(const quat_left_ideal_t *lideal1,
                       const quat_left_ideal_t *lideal2,
                       const quat_alg_t *alg); // test/ideal

/**
 * @brief  Sum of two left ideals
 *
 * @param sum Output: Left ideal which is the sum of the 2 inputs
 * @param lideal1 left ideal
 * @param lideal2 left ideal
 * @param alg the quaternion algebra
 */
void quat_lideal_add(quat_left_ideal_t *sum,
                     const quat_left_ideal_t *lideal1,
                     const quat_left_ideal_t *lideal2,
                     const quat_alg_t *alg); // Not used outside

/**
 * @brief  Left ideal product of left ideal I and element alpha
 *
 * @param product Output: lideal I*alpha, must have integer norm
 * @param lideal left ideal
 * @param alpha element multiplied to lideal to get the product ideal
 * @param alg the quaternion algebra
 *
 * I*alpha where I is a left-ideal and alpha an element of the algebra
 *
 * The resulting ideal must have an integer norm
 *
 */
void quat_lideal_mul(quat_left_ideal_t *product,
                     const quat_left_ideal_t *lideal,
                     const quat_alg_elem_t *alpha,
                     const quat_alg_t *alg); // test/ideal

/** @brief Computes the inverse ideal (for a left ideal of a maximal order) without putting it under
 * HNF
 *
 * This function returns a lattice not under HNF. For careful internal use only
 *
 * Computes the inverse ideal for lideal as conjugate(lideal)/norm(lideal)
 *
 * @param inv Output: lattice which is lattice representation of the inverse ideal of lideal
 * ATTENTION: is not under HNF. hnf computation must be applied before using lattice functions on it
 * @param lideal Left ideal of a maximal order in alg
 * @param alg The quaternion algebra
 */
void quat_lideal_inverse_lattice_without_hnf(quat_lattice_t *inv,
                                             const quat_left_ideal_t *lideal,
                                             const quat_alg_t *alg); // ideal

/** @brief Computes the right transporter of two left ideals of the same maximal order
 *
 * Following the implementation of ideal isomorphisms in the code of LearningToSQI's sage
 * implementation of SQIsign. Computes the right transporter of (J:I) as inverse(I)J.
 *
 * @param trans Output: lattice which is right transporter from lideal1 to lideal2 (lideal2:lideal1)
 * @param lideal1 Left ideal of the same maximal order than lideal1 in alg
 * @param lideal2 Left ideal of the same maximal order than lideal1 in alg
 * @param alg The quaternion algebra
 */
void quat_lideal_right_transporter(quat_lattice_t *trans,
                                   const quat_left_ideal_t *lideal1,
                                   const quat_left_ideal_t *lideal2,
                                   const quat_alg_t *alg);

/**
 * @brief  Right order of a left ideal
 *
 * @param order Output: right order of the given ideal
 * @param lideal left ideal
 * @param alg the quaternion algebra
 */
void quat_lideal_right_order(quat_lattice_t *order, const quat_left_ideal_t *lideal,
                             const quat_alg_t *alg); // ideal

/**
 * @brief  Gram matrix of the trace map of the ideal class
 *
 * Compute the Gram matrix of the bilinear form
 *
 *     〈a, b〉 := Tr(a·conj(b)) / norm(lideal)
 *
 * on the basis of the ideal. This matrix has integer entries and its
 * integer congruence class only depends on the ideal class.
 *
 * @param G Output: Gram matrix of the trace map
 * @param lideal left ideal
 * @param alg the quaternion algebra
 */
void quat_lideal_class_gram(ibz_mat_4x4_t *G, const quat_left_ideal_t *lideal, const quat_alg_t *alg);

/** @brief Test if order is maximal
 *
 * Checks if the discriminant of the order equals the prime p defining the quaternion algebra.
 *
 * It is not verified whether the order is really an order. The output 1 only means that if it is an
 * order, then it is maximal.
 *
 * @returns 1 if order is maximal (assuming it is an order), 0 otherwise
 * @param order An order of the quaternion algebra (assumes to be an order, this is not tested)
 * @param alg The quaternion algebra
 */
int quat_order_is_maximal(const quat_lattice_t *order,
                          const quat_alg_t *alg); // ideal (only in asserts)

/** @brief Compute the discriminant of an order as sqrt(det(gram(reduced_norm)))
 *
 * @param disc: Output: The discriminant sqrt(det(gram(reduced_norm)))
 * @param order An order of the quaternion algebra
 * @param alg The quaternion algebra
 */
int quat_order_discriminant(ibz_t *disc, const quat_lattice_t *order,
                            const quat_alg_t *alg); // ideal

/** @}
 */

/** @internal
 * @ingroup quat_normeq
 * @{
 */

/** @brief Set lattice to O0
 *
 * @param O0 Lattice to be set to (1,i,(i+j)/2,(1+ij)/2)
 */
void quat_lattice_O0_set(quat_lattice_t *O0);

/** @brief Set p-extremal maximal order to O0
 *
 * @param O0 p-extremal order to be set to (1,i,(i+j)/2,(1+ij)/2)
 */
void quat_lattice_O0_set_extremal(quat_p_extremal_maximal_order_t *O0);

/**
 * @brief Create an element of a extremal maximal order from its coefficients
 *
 * @param elem Output: the quaternion element
 * @param order the order
 * @param coeffs the vector of 4 ibz coefficients
 * @param Bpoo quaternion algebra
 *
 * elem = x + z*y + z*u + t*z*v
 * where coeffs = [x,y,u,v] and t = order.t z = order.z
 *
 */
void quat_order_elem_create(quat_alg_elem_t *elem,
                            const quat_p_extremal_maximal_order_t *order,
                            const ibz_vec_4_t *coeffs,
                            const quat_alg_t *Bpoo); // normeq, untested

/** @}
 */
/** @}
 */

#endif
