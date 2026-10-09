/** @file
 *
 * @authors Sina Schaeffler
 *
 * @brief Declarations for helper functions for quaternion algebra implementation
 */

#ifndef QUAT_HELPER_H
#define QUAT_HELPER_H

#include <quaternion.h>
#include <quaternion_data.h>
#include <quaternion_constants.h>
#include <assert.h>
/**
 * @ingroup quat_quat
 * @defgroup quat_helpers Quaternion module internal functions
 * @{
 */

/**
 * @defgroup quat_alg_helpers Helper functions for the alg library
 * @{
 */

/**
 * @brief helper function for initializing small quaternion algebras.
 *
 * p must have less than 32 bit
 */
void quat_alg_init_set_ui(quat_alg_t *alg, unsigned int p);

/** @brief Sets an algebra element to the given integer values, without normalizing it
 *
 * None of the inputs can be equal to -2^31.
 *
 * @param elem Output: algebra element of coordinates [coord0,coord1,coord2,coord3] and denominator denom
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

/** @brief a*b
 *
 * Multiply two coordinate vectors as elements of the algebra in basis (1,i,j,ij) with i^2 = -1, j^2 = -p
 *
 * @param res Output: Will contain product
 * @param a
 * @param b
 * @param alg The quaternion algebra
 */
void quat_alg_coord_mul(ibz_vec_4_t *res, const ibz_vec_4_t *a, const ibz_vec_4_t *b, const quat_alg_t *alg);

/** @brief Test if x is 0
 *
 * x is 0 iff all coordinates in x->coord are 0
 *
 * @returns 1 if x=0, 0 otherwise
 * @param x
 */
int quat_alg_elem_is_zero(const quat_alg_elem_t *x);

/** @brief a=b
 *
 * Test if a and b represent the same quaternion algebra element
 *
 * @param a
 * @param b
 * @returns 1 if a=b, 0 otherwise
 */
int quat_alg_elem_equal(const quat_alg_elem_t *a, const quat_alg_elem_t *b);

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

/** @brief Normalize representation of alg_elem x
 *
 * @param x Algebra element whose representation will be normalized
 *
 * Modification of x.
 * Sets coord and denom of x so that gcd(denom, content(coord))=1
 * without changing the value of x = (coord0/denom, coord1/denom, coord2/denom, coord3/denom).
 */
void quat_alg_normalize(quat_alg_elem_t *x);

/** @brief Copies the given values into an algebra element, without normalizing it
 *
 * @param elem Output: algebra element of coordinates [coord0,coord1,coord2,coord3] and denominator denom
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

/**
 * @brief Creates algebra element from scalar
 *
 * Resulting element has 1-coordinate equal to numerator/denominator
 *
 * @param elem Output: algebra element with numerator/denominator as first coordiante (1-coordinate), 0 elsewhere
 * (i,j,ij coordinates)
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

/** @}
 */

/**
 * @defgroup quat_dim4_helpers Helper functions for functions for matrices or vectors in dimension 4
 * @{
 */

/**
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
void ibz_mat_4x4_inv_make_coeff_pmp(ibz_t *coeff,
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
void ibz_mat_4x4_inv_make_coeff_mpm(ibz_t *coeff,
                                    const ibz_t *a1,
                                    const ibz_t *a2,
                                    const ibz_t *b1,
                                    const ibz_t *b2,
                                    const ibz_t *c1,
                                    const ibz_t *c2);

/** @brief Matrix determinant and a matrix inv such that inv/det is the inverse matrix of the input
 *
 * Implemented following the method of 2x2 minors explained at Method from
 * https://www.geometrictools.com/Documentation/LaplaceExpansionTheorem.pdf (visited on 3rd of May 2023, 16h15 CEST)
 *
 * @returns 1 if the determinant of mat is not 0 and an inverse was computed, 0 otherwise
 * @param inv Output: Will contain an integer matrix which, divided by det, yields the rational inverse of the
 * matrix if it exists. Can be NULL
 * @param det Output: Will contain the determinant of the input matrix, can be NULL
 * @param mat Matrix of which the inverse will be computed
 */
int ibz_mat_4x4_inv_with_det_as_denom(ibz_mat_4x4_t *inv, ibz_t *det, const ibz_mat_4x4_t *mat);

/** @brief Copies all values from a 4x4 integer matrix to another one
 *
 * @param copy Output: matrix which will have its entries set to copied's entries
 * @param copied Input matrix
 */
void ibz_mat_4x4_copy(ibz_mat_4x4_t *copy, const ibz_mat_4x4_t *copied);

/** @}
 */

/**
 * @defgroup quat_dim4_lat_helpers Helper functions on vectors and matrices used mainly for lattices
 * @{
 */

/** @brief Copy all values from one vector to another
 *
 * @param copy Output: copy of copied
 * @param copied Input, to be copied into copy
 */
void ibz_vec_4_copy(ibz_vec_4_t *copy, const ibz_vec_4_t *copied);

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
 * None of the inputs can be equal to -2^31.
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

/** @brief multiplies all values in vector by same scalar
 *
 * @param prod Output
 * @param scalar
 * @param vec
 */
void ibz_vec_4_scalar_mul(ibz_vec_4_t *prod, const ibz_t *scalar, const ibz_vec_4_t *vec);

/**
 * @brief content of a 4-vector of integers
 *
 * The content is the GCD of all entries.
 *
 * @param v A 4-vector of integers
 * @param content Output: the resulting gcd
 */
void ibz_vec_4_content(ibz_t *content, const ibz_vec_4_t *v);

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

/** @brief Equality test for 4x4 integer matrices
 *
 * @returns 1 if equal, 0 otherwise
 * @param mat1
 * @param mat2
 */
int ibz_mat_4x4_equal(const ibz_mat_4x4_t *mat1, const ibz_mat_4x4_t *mat2);

/** @brief transpose a 4x4 integer matrix
 *
 * @param transposed Output: is set to the transposition of mat
 * @param mat Input matrix
 */
void ibz_mat_4x4_transpose(ibz_mat_4x4_t *transposed, const ibz_mat_4x4_t *mat);

/** @brief a*b for a,b integer 4x4 matrices
 *
 * Naive implementation
 *
 * @param res Output: A 4x4 integer matrix
 * @param a
 * @param b
 */
void ibz_mat_4x4_mul(ibz_mat_4x4_t *res, const ibz_mat_4x4_t *a, const ibz_mat_4x4_t *b);

/** @brief divides all values in matrix by same scalar
 *
 * @returns 1 if scalar divided all values in mat, 0 otherwise (division is performed in both cases)
 * @param quot Output
 * @param scalar
 * @param mat
 */
int ibz_mat_4x4_scalar_div(ibz_mat_4x4_t *quot, const ibz_t *scalar, const ibz_mat_4x4_t *mat);

/**
 * @brief mat*vec
 *
 *
 * @param res Output: coordinate vector
 * @param mat Integer 4x4 matrix
 * @param vec Integer vector (coordinate vector)
 *
 * Multiplies 4x4 integer matrix mat by a 4-integers column vector vec
 */
void ibz_mat_4x4_eval(ibz_vec_4_t *res, const ibz_mat_4x4_t *mat, const ibz_vec_4_t *vec);

/**
 * @brief vec*mat
 *
 *
 * @param res Output: coordinate vector.
 * @param vec Integer vector (coordinate vector)
 * @param mat Integer 4x4 matrix
 *
 * Multiplies 4x4 integer matrix mat by a 4-integers row vector vec (on the left)
 */
void ibz_mat_4x4_eval_t(ibz_vec_4_t *res, const ibz_vec_4_t *vec, const ibz_mat_4x4_t *mat);

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

/** @}
 */
/** @}
 */

/** @defgroup quat_qf Quadratic form functions
 * @{
 */

/**
 * @brief Quadratic form evaluation
 *
 * qf and coord must be represented in the same basis.
 *
 * @param res Output: coordinate vector
 * @param qf Quadratic form (4x4 integer matrix)
 * @param coord Integer vector (coordinate vector)
 */
void quat_qf_eval(ibz_t *res, const ibz_mat_4x4_t *qf, const ibz_vec_4_t *coord);
/** @}
 */

/**
 * @defgroup quat_dim2_helpers Helper functions for dimension 2
 * @{
 */

/** @brief Set vector coefficients to the given integers
 *
 * @param vec Output: Vector
 * @param a0
 * @param a1
 */
void ibz_vec_2_set(ibz_vec_2_t *vec, int a0, int a1);

/** @brief Set matrix coefficients to the given integers
 *
 * @param mat Output: Matrix
 * @param a00
 * @param a01
 * @param a10
 * @param a11
 */
void ibz_mat_2x2_set(ibz_mat_2x2_t *mat, int a00, int a01, int a10, int a11);

/** @brief Copy matrix
 *
 * @param copy Output: Matrix into which copied will be copied
 * @param copied
 */
void ibz_mat_2x2_copy(ibz_mat_2x2_t *copy, const ibz_mat_2x2_t *copied);

/** @brief mat*scalar in dimension 2 for integers
 *
 * @param prod Output matrix
 * @param mat Input matrix
 * @param scalar Input scalar
 */
void ibz_mat_2x2_scalar_mul(ibz_mat_2x2_t *prod, const ibz_t *scalar, const ibz_mat_2x2_t *mat);

/** @brief a*b multiplication in dimension 2 for square matrices
 *
 * @param prod Output matrix
 * @param mat_a 1rst input matrix
 * @param mat_b 2nd input matrix
 */
void ibz_mat_2x2_mul(ibz_mat_2x2_t *prod, const ibz_mat_2x2_t *mat_a, const ibz_mat_2x2_t *mat_b);

/** @brief Matrix inverse for 2x2 matrices
 *
 * @returns 1 if the determinant of mat is not 0 and an inverse was computed, 0 otherwise
 * @param inv Output: Will contain an integer matrix which, divided by det, yields the rational inverse of the
 * matrix if it exists, can be NULL
 * @param det Output: Will contain the determinant of the input matrix, can be NULL
 * @param mat Matrix of which the inverse will be computed
 */
int ibz_mat_2x2_inv_with_det_as_denom(ibz_mat_2x2_t *inv, ibz_t *det, const ibz_mat_2x2_t *mat);

/** @brief Determinant of a 2x2 integer matrix given as 4 integers
 *
 * @param det Output: Determinant of the matrix
 * @param a11 matrix coefficient (upper left corner)
 * @param a12 matrix coefficient (upper right corner)
 * @param a21 matrix coefficient (lower left corner)
 * @param a22 matrix coefficient (lower right corner)
 */
void ibz_mat_2x2_det_from_ibz(ibz_t *det, const ibz_t *a11, const ibz_t *a12, const ibz_t *a21, const ibz_t *a22);

/**
 * @brief Multiplication of 2x2 integer matrices modulo m
 *
 * @param prod Output matrix
 * @param mat_a First Input matrix
 * @param mat_b Second Input matrix
 * @param m Integer modulo
 * */
void ibz_mat_2x2_mul_mod(ibz_mat_2x2_t *prod, const ibz_mat_2x2_t *mat_a, const ibz_mat_2x2_t *mat_b, const ibz_t *m);

/** @}
 */

/**
 * @defgroup quat_lattice_helper Helper functions for the lattice library (dimension 4)
 * @{
 */
/** @brief Divides basis and denominator of a lattice by their gcd
 *
 * @param reduced Output
 * @param lat Lattice
 */
void quat_lattice_reduce_denom(quat_lattice_t *reduced, const quat_lattice_t *lat);

/**
 * @brief Test whether x in lat. If so, compute its coordinates in lat's basis.
 *
 * @param coord Output: Set to the coordinates of x in lat. May be NULL.
 * @param lat The lattice, full-rank and with upper triangular basis
 * @param x An element of the quaternion algebra
 * @return 1 if x in lat, 0 otherwise
 */
int quat_lattice_contains(ibz_vec_4_t *coord, const quat_lattice_t *lat, const quat_alg_elem_t *x);

/**
 * @brief Lattice inclusion test
 *
 * overlat must have an upper triangular basis matrix
 *
 * @returns 1 if sublat is included in overlat, 0 otherwise
 * @param sublat Lattice whose inclusion in overlat will be testes
 * @param overlat Lattice given with an upper triangular basis matrix
 */
int quat_lattice_inclusion(const quat_lattice_t *sublat, const quat_lattice_t *overlat);

/**
 * @brief Lattice equality
 *
 * Lattice bases are assumed to be both triangular and full rank
 *
 * @returns 1 if both lattices are equal, 0 otherwise
 * @param lat1 Lattice given by an upper triangular basis
 * @param lat2 Lattice given by an upper triangular basis
 */
int quat_lattice_equal(const quat_lattice_t *lat1, const quat_lattice_t *lat2);

/** @brief Compute the Gram matrix of the quaternion trace bilinear form
 *
 * Given a lattice of the quaternion algebra, computes the Gram matrix of the bilinear form
 *
 *      〈a,b〉 := [lattice->denom^2] Tr(a·conj(b))
 *
 * multiplied by the square of the denominator of the lattice.
 *
 * This matrix always has integer entries.
 *
 * @param G Output: Gram matrix of the trace bilinear form on the lattice,
 * multiplied by the square of the denominator of the lattice
 * @param lattice A lattice
 */
void quat_lattice_gram(ibz_mat_4x4_t *G, const quat_lattice_t *lattice);

/**
 * @brief Sample from the intersection of a lattice with a ball
 *
 * Sample a uniform non-zero vector of norm <= radius from the lattice.
 *
 * @param res Output: sampled quaternion from the lattice
 * @param res_ideal_norm Output: Norm of res divided by the norm of the ideal given by lattice, can be NULL
 * @param lattice Input lattice, must be a lattice of a product of a right and left inert O0-ideal as output by
 * quat_ideal_mul_O0
 * @param radius The ball radius (output ideal norm = quaternion norm/input ideal norm)
 * @param prng_domain Randomness domain
 * @return 0 if an error occurred (ball too small or RNG error), 1 otherwise
 */
int quat_lattice_sample_from_ball(quat_alg_elem_t *res,
                                  ibz_t *res_ideal_norm,
                                  const quat_lattice_t *lattice,
                                  const ibz_t *radius,
                                  prng_domain_ctx_t *prng_domain);

/** @}
 */

/**
 * @defgroup quat_stdorder Functions specific to O0
 * @{
 */

/** @brief Set lattice to O0
 *
 * @param O0 Lattice to be set to (1,i,(i+j)/2,(1+ij)/2)
 */
void quat_lattice_O0_set(quat_lattice_t *O0);

/**
 * @brief Representing an integer by the quadratic norm form of the standard order O0.
 *
 * @returns 1 if the computation succeeded, 0 otherwise
 * @param gamma Output: a quaternion element
 * @param n_gamma Target norm of gamma. n_gamma must be odd.
 * If n_gamma/(p*params.order->q) < 2^QUAT_repres_bound_input failure is likely
 * @param prng_domain Randomness domain
 *
 * This algorithm finds a primitive quaternion element gamma of n_gamma inside O0.
 * Failure is possible, especially on too small input n_gamma
 */
int quat_represent_integer(quat_alg_elem_t *gamma, const ibz_t *n_gamma, prng_domain_ctx_t *prng_domain);

/** @}
 */
/**
 * @defgroup quat_qlapoty_helper Helper functions for qlapoty
 * @{
 */

/**
 * @brief Sum the suqres of 2 integer to a^2+b^2
 *
 * @param sum Output: a^2+b^2
 * @param a Integer
 * @param b Integer
 */
void ibz_sum_two_squares(ibz_t *sum, const ibz_t *a, const ibz_t *b);

/**
 * @brief Find integers x and y such that x^2 + y^2 = p
 *
 * Uses Cornacchia's algorithm, should be used  only for prime p
 *
 * @param x Output, positive and smaller or equal y
 * @param y Output, larger or equal x
 * @param p parameter defining the equation, must be prime
 * @return 1 if success, 0 otherwise
 */
int ibz_cornacchia_prime(ibz_t *x, ibz_t *y, const ibz_t *p);

/** @brief round a/b to closest integer q
 *
 * @param q Output: rounded quotient
 * @param a
 * @param b
 */
void ibz_rounded_div(ibz_t *q, const ibz_t *a, const ibz_t *b);

/**
 * @brief Finds two deal Ii~ideal ideals such that norm(I1)+norm(I2)=QUAT_qlapoty_used_power_of_two
 * where Ii=small_equiv*conj(mui)/norm(small_equiv)
 *
 * In this small_equiv denotes (ideal*conj(smallest/norm(ideal)).
 *
 * @param mu1 Output: mu1 such that I1 = (ideal*conj(smallest/norm(ideal)) conj(mu1)/norm(small_equiv)
 * @param mu2 Output: mu2 such that I2 = (ideal*conj(smallest/norm(ideal))* conj(mu2)/norm(small_equiv)
 * @param theta Output: mu2*conj(mu1)/norm
 * @param smallest Output: Smallest element in ideal
 * @param ideal Input ideal. Must be left ideal of the special extremal maximal order O0.
 * @returns 1 if success, 0 if failure. Failure certain if a very small equivalent ideal of the input exists
 */
int quat_qlapoty_normeq(quat_alg_elem_t *mu1,
                        quat_alg_elem_t *mu2,
                        quat_alg_elem_t *theta,
                        quat_alg_elem_t *smallest,
                        const quat_ideal_t *ideal);

/** @}
 */

/**
 * @defgroup quat_ideal_helpers Internal quaternion ideal functions
 * @{
 */

/**
 * @brief Create lattice from inert O0 ideal
 *
 * @param lattice Output: ideal given by lattice basis
 * @param ideal Input ideal
 */
void quat_to_lattice(quat_lattice_t *lattice, const quat_ideal_t *ideal);

/**
 * @brief Create  inert O0 ideal from its components
 *
 * @param ideal Output: ideal
 * @param n Odd positive integer, ideal norm
 * @param x Positive integer, x in the note
 * @param y Positive integer, y in the note
 */
void quat_xyn_to_inert_ideal(quat_ideal_t *ideal, const ibz_t *x, const ibz_t *y, const ibz_t *n);

/**
 * @brief Generator of an odd norm O0 ideal in inert HNF
 *
 * @param ideal Input O0 ideal in inert HNF given by x,y, N, where N must be odd
 * @param gen Output: generator of ideal of the form (2x+i(2y+1)+j)/2
 */
void quat_ideal_odd_inert_gen(quat_alg_elem_t *gen, const quat_ideal_t *ideal);

/**
 * @brief x mod modO0
 *
 *
 * @param red Output: x modulo mod O0
 * @param x Input quaternion, must be in O0
 * @param mod Integer modulus, positive larger than 1
 */
void quat_mod_O0(quat_alg_elem_t *red, const quat_alg_elem_t *x, const ibz_t *mod);

/**
 * @brief Copy ideal
 *
 * @param copy Output: will contain copy of copied
 * @param copied input
 */
void quat_ideal_copy(quat_ideal_t *copy, const quat_ideal_t *copied);

/**
 * @brief GenToIdOdd: Create odd norm O0 ideal from norm and generator
 *
 * This only works if the ideal+2O0 does not contain i+1
 *
 * @param ideal Output: ideal of norm norm
 * @param split_gen Output: genertor of the largest factor of the ideal whose HNF is in split form
 * @param gen Generator of the ideal A O0-element of norm multiple of norm
 * @param norm An odd integer
 */
void quat_ideal_create_O0_odd(quat_ideal_t *ideal,
                              quat_alg_elem_t *split_gen,
                              const quat_alg_elem_t *gen,
                              const ibz_t *norm);

/**
 * @brief IdealMultiplication: Product conjugate(a)b
 *
 * @param prod Output: ideal (not of O0) conjugate(a)b given by its lattice
 * @param a Odd norm O0-ideal, in HNF on denominator 2
 * @param b Odd norm O0-ideal of norm coprime to norm(a), in HNF on denominator 2
 */
void quat_ideal_mul_O0(quat_lattice_t *prod, const quat_ideal_t *a, const quat_ideal_t *b);

/**
 * @brief Gram matrix of a product lat = conjugate(a)b of inert O0 ideals a,b,  divided by the product's norm
 *
 * @param gram Output: gram matrix of ideal (not of O0) lat in basis output by quat_ideal_mul_O0, divided
 * by its norm
 * @param lat Lattice as output by quat_ideal_mul_O0
 */
void quat_ideal_product_gram_matrix(ibz_mat_4x4_t *gram, const quat_lattice_t *lat);

/**
 * @brief Multiplication of gaussian integers
 *
 * @param prod Output: a*b in Z[i]
 * @param a Gaussian integer
 * @param b Gaussian integer
 */
void ibz_vec_2_gaussian_mul(ibz_vec_2_t *prod, const ibz_vec_2_t *a, const ibz_vec_2_t *b);

/**
 * @brief Addition of 2-vectors
 *
 * @param copy Output: copy of copied
 * @param copied Input
 */
void ibz_vec_2_copy(ibz_vec_2_t *copy, const ibz_vec_2_t *copied);

/**
 * @brief GCD of gaussian integers. Not defined for 0,0
 *
 * At least one of a,b must be non-zero
 *
 * @param gcd Output: Will contain a (non-unique) gcd of a and b as gaussian integers
 * @param a Gaussian integer
 * @param b Gaussian integer
 */
void ibz_vec_2_gaussian_gcd(ibz_vec_2_t *gcd, const ibz_vec_2_t *a, const ibz_vec_2_t *b);

/**
 * @brief Test whether a gaussian integer (or 2-vector) is zero
 *
 * @param x Vector to test
 * @return 1 if both coordinates are zero, 0 otherwise
 */
int ibz_vec_2_is_zero(const ibz_vec_2_t *x);

/**
 * @brief Euclidean division of gaussian integers: a = q*b + r with N(r) < N(b)
 *
 * @param q Output: quotient, may be NULL
 * @param r Output: remainder, may be NULL
 * @param a Gaussian integer
 * @param b Gaussian integer, must be non-zero
 */
void ibz_vec_2_gaussian_euclidean_division(ibz_vec_2_t *q, ibz_vec_2_t *r, const ibz_vec_2_t *a, const ibz_vec_2_t *b);

/**
 * @brief InertGenToId: Create O0 ideal in inert HNF form from norm and generator if it exists
 *
 * Otherwise this function an creates the largest inert HNF ideal dividing the requied one
 *
 * @param ideal Output: left ideal of O0 generated by norm, gen
 * @param gen Generator of the ideal. A O0-element of norm multiple of norm
 * @param norm Positive integer dividing nrd(gen)
 * @returns 0 if ideal has no inert HNF, 1 on success
 */
int quat_ideal_create_O0_inert(quat_ideal_t *ideal, const quat_alg_elem_t *gen, const ibz_t *norm);

/**
 * @brief SmallestEquivalentIdeal: Equivalent ideal of minimal norm to a left O0 ideal
 *
 * @param gen Output: generator of red
 * @param equiv Output: element of ideal such that red=ideal*conj(equiv)/n(ideal)
 * @param red Output: ideal equivalent to ideal of minimal norm
 * @param ideal O0-ideal. Largest factor with inert HNF of the target ideal
 */
void quat_ideal_shortest_equivalent(quat_alg_elem_t *gen,
                                    quat_alg_elem_t *equiv,
                                    quat_ideal_t *red,
                                    const quat_ideal_t *ideal);

/**
 * @brief Find small equivalent inert ideal with required (co)primality given a reduced ideal basis
 *
 *
 * @param gen Output: Generator of the output ideal
 * @param equiv Output: equivalent ideal, generated by gen and its norm. Norm is always odd
 * @param ideal Input ideal given by a lattice with reduced basis
 * @param ideal_norm Input ideal norm
 * @param coprime_to Integer input. If 0 equiv.norm is prime, else coprime to coprime_toIf NULL or 2, output is an
 * odd equivalent ideal
 * @param prng_domain Randomness domain used
 * @returns 1 on success, 0 on randomness failure. Infinite loop if no solution exists
 */
int quat_ideal_small_equivalent_coprime_enumeration(quat_alg_elem_t *gen,
                                                    quat_ideal_t *equiv,
                                                    const quat_lattice_t *ideal,
                                                    const ibz_t *ideal_norm,
                                                    const ibz_t *coprime_to,
                                                    prng_domain_ctx_t *prng_domain);
/** @}
 */
/**
 * @defgroup quat_lattice_reduction_exports Lattice reduction applications used by quaternion functions
 * @{
 */

/**
 * @brief Reduce the basis of the left ideal, without considering its denominator
 *
 * This function reduces the basis of the lattice of the ideal, but it does completely ignore its
 * denominator. So the outputs of this function must still be divided by the appropriate power of
 * ideal.lattice.denom.
 *
 * Constant-time.
 *
 * @param reduced Output: Lattice defining the ideal, which has its basis in a lll-reduced form.
 * Must be divided by ideal.lattice.denom before usage
 * @param ideal ideal whose basis will be reduced
 */
void quat_ideal_reduce_basis(quat_lattice_t *reduced, const quat_ideal_t *ideal);

/**
 * @brief Compute an integer parallelogram containing the ball of given radius for the positive definite quadratic form
 * defined by the Gram matrix G.
 *
 * The computed parallelogram is defined by the vectors
 *
 *      (x₁ x₂ x₃ x₄) · U
 *
 * with x_i ∈ [ -box[i], box[i] ].
 *
 * @param box Output: bounds of the parallelogram
 * @param U Output: Unimodular transformation defining the parallelogram
 * @param lat Input lattice, must have the shape of an output of quat_ideal_mul_O0
 * @param radius Radius of the ball, must be non-negative
 * @returns 0 if the box only contains the origin, 1 otherwise
 */
int quat_lattice_bound_parallelogram(ibz_vec_4_t *box,
                                     ibz_mat_4x4_t *U,
                                     const quat_lattice_t *lat,
                                     const ibz_t *radius);

/** @}
 */
/** @}
 */

#endif
