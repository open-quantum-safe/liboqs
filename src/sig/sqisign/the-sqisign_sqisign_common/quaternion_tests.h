/** @file
 *
 * @authors Sina Schaeffler
 *
 * @brief Declarations of tests of quaternion algebra operations
 */

#ifndef QUATERNION_TESTS_H
#define QUATERNION_TESTS_H

#include <quaternion.h>
#include <stdio.h>
#include "internal.h"

/**
 * @ingroup quat_helpers
 * @defgroup quat_tests Quaternion module test functions
 * @{
 */

/**
 * @defgroup quat_test_helpers Quaternion functions for testing ideal functions
 * @{
 */

void quat_lattice_norm(ibz_t *norm, const quat_lattice_t *ideal);
void quat_lattice_from_ideal_split(quat_lattice_t *lat, const quat_ideal_t *ideal, const quat_alg_elem_t *split);
// returns 1 if solution found
int quat_represent_integer_even(quat_alg_elem_t *gamma, const ibz_t *n_gamma);
int quat_ideal_shape(const quat_ideal_t *ideal);
// tests returning 1 if statement true
int quat_ideal_equal(const quat_ideal_t *a, const quat_ideal_t *b);
int quat_lattice_is_ideal(const quat_lattice_t *lat);
int quat_lattice_is_O0_ideal(const quat_lattice_t *lat);
int quat_lattice_is_maximal_order(const quat_lattice_t *lat);
int quat_ideals_equivalence(const quat_ideal_t *a, const quat_ideal_t *b);
/**
 * @}
 */

/**
 * @defgroup quat_test_inputs Quaternion module random test input generation
 * @{
 */

/**
 * @brief Generates list of random ideals of O0
 *
 * @param res Output: random value in [-2^bitsize,2^bitsize]
 * @param bitsize Maximal output bitsize
 * @return 1 if success, 0 if failure
 */
int ibz_rand_interval_bits(ibz_t *res, int bitsize);

/**
 * @brief Generates list of random ideals of O0
 *
 * @param ideals Array of iterations left ideals of O0 and of prime norms of size norm_bitsize
 * @param norm_bitsize Bitsize of the norms of the outut ideals
 * @param iterations Number of ideals to sample. Most be smaller than lattices is long
 * @return 0 if success, 1 if failure
 */
int quat_test_input_random_ideal_generation(quat_ideal_t *ideals, int norm_bitsize, int iterations);

/**
 * @brief Generates list of random ideals of O0
 *
 * @param lattices Output: Array of iterations left ideals of O0 and of prime norms of size norm_bitsize, given only by
 * their lattices (in inert HNF)
 * @param norms Output: Array which will contain the norms of the sampled ideals, in the same order as their lattices
 * are. Can be NULL, in which case no norm is output. Otherwise, it must be an array of length at least iterations
 * @param norm_bitsize Bitsize of the norms of the outut ideals
 * @param iterations Number of ideals to sample. Most be smaller than lattices is long
 * @return 0 if success, 1 if failure
 */
int quat_test_input_random_ideal_lattice_generation(quat_lattice_t *lattices,
                                                    ibz_t *norms,
                                                    int norm_bitsize,
                                                    int iterations);

/**
 * @brief Generates list of random ideals passing by O0
 *
 * @param lattices Output: Array of iterations resplike ideals and of norms of size norm_bitsize, given only by
 * their lattices in a triangular basis
 * @param norms Output: Array of norms of the ideals given by the output lattices. Can be NULL
 * @param bitsize Bitsize of the norms of the outut ideals
 * @param iterations Number of ideals to sample. Most be smaller than lattices is long
 * @return 0 if success, 1 if failure
 */
int quat_test_input_resplike_lattice_generation(quat_lattice_t *lattices, ibz_t *norms, int bitsize, int iterations);

/**
 * @brief Generates list of random lattices
 *
 * @param lattices Output: Array of iterations lattices
 * @param bitsize Bitsize of the coefficients of a random basis of the lattices
 * @param iterations Number of lattices to sample.Most be smaller than lattices is long
 * @return 0 if success, 1 if failure
 */
int quat_test_input_random_lattice_generation(quat_lattice_t *lattices, int bitsize, int iterations);
/**
 * @}
 */

/** @brief Test initializers for quaternion algebra types
 *
 * Test initializers  for the following types:
 *
 * quat_alg_t
 *
 * quat_alg_elem_t
 *
 * quat_alg_coord_t
 *
 * ibz_vec_2_t
 *
 * ibz_vec_4_t
 *
 * ibz_mat_2x2_t
 *
 * ibz_mat_4x4_t
 *
 * quat_lattice_t
 *
 * quat_ideal_t
 */
int quat_test_init(void);

/** @brief Test integer, quadratic form and matrix functions for dimension 4 from the quaternion module
 *
 */
int quat_test_dim4(void);

/** @brief Test integer, lattice and matrix functions for dimension 2 from the quaternion module
 *
 * Runs unit tests for the following functions
 *
 * void ibz_mat_2x2_eval(ibz_vec_2_t *res, const ibz_mat_2x2_t *mat, const ibz_vec_2_t *vec);
 *
 * int ibz_mat_2x2_inv_mod(ibz_mat_2x2_t *inv, const ibz_mat_2x2_t *mat, const ibz_t *m);
 *
 * void ibz_mat_2x2_normalize(ibz_mat_2x2_t *mat, int e);
 */
int quat_test_dim2(void);

/** @brief Test 2d lattice reduction and subfunctions
 *
 * Test in lll/test/dim2_tests.c for functions defined in the lll folder
 *
 * void quat_lll_dim2_short_basis(ibz_mat_2x2_t *reduced, const ibz_mat_2x2_t *basis, int det_bits);
 *
 * void quat_lll_dim2_sumofsquares(ibz_t *x, ibz_t *y, const ibz_t *p, const ibz_t *r);
 */
int quat_test_lll_dim2(void);

/** @brief Test integer functions
 *
 * Runs unit tests for the following functions
 *
 * int ibz_generate_random_prime(ibz_t *p, int is3mod4, int bitsize);
 */
int quat_test_integers(void);

/** @brief Test operations on quaternion algebra elements
 *
 * Runs unit tests for the following functions
 *
 * void quat_alg_mul(quat_alg_elem_t *res, const quat_alg_elem_t *a, const quat_alg_elem_t *b, const quat_alg_t *alg);
 *
 * void quat_alg_conj(quat_alg_elem_t *conj, const quat_alg_elem_t *x);
 *
 * void quat_alg_make_primitive(quat_alg_coord_t *primitive_x, ibz_t *content, const quat_alg_elem_t *x, const
 * quat_lattice_t *order, const quat_alg_t *alg){
 *
 * void quat_alg_elem_scalar_mul(quat_alg_elem_t *prod, const ibz_t *scalar, const quat_alg_elem_t *elem);
 *
 * void quat_alg_norm(ibz_t *res_num, ibz_t *res_denom, const quat_alg_elem_t *x, const quat_alg_t *alg);
 */
int quat_test_algebra(void);

/** @brief Test operations on lattices
 */
int quat_test_lattice(void);

/** @brief Test for lattice reduction and functions based on it
 *
 * Test in lll/test/lll_tests.c Also covers the functions defined in lvlx/lll_applications.c
 *
 * void quat_ideal_reduce_basis(quat_lattice_t *reduced, const quat_ideal_t *ideal);
 *
 * int quat_lattice_bound_parallelogram(ibz_vec_4_t *box, ibz_mat_4x4_t *U, const quat_lattice_t *lat, const ibz_t
 * *radius);
 *
 * void quat_lll_dual_reduce_ideal(quat_lattice_t *reduced, ibz_t gram_diag[4], ibz_mat_4x4_t *Ainv, const
 * quat_lattice_t *hnf, const quat_alg_t *alg);
 *
 */
int quat_test_lll(void);

/** @brief Test operations on represent integer
 *
 * void quat_change_to_O0_basis(ibz_vec_4_t *vec, const quat_alg_elem_t *el);
 *
 */
int quat_test_stdorder(void);

/** @brief Test for qlapoty and its subfunctions
 *
 *  int quat_qlapoty(quat_alg_elem_t *beta1, ibz_t *d1, quat_alg_elem_t *theta, const quat_ideal_t *ideal););
 */
int quat_test_qlapoty(void);

/** @brief Test for quaternion ideal functions
 *
 * int quat_ideal_create_O0_inert_odd(quat_ideal_t *ideal, const quat_alg_elem_t *gen, const ibz_t *norm);
 *
 * int quat_ideal_create_O0_pow_two(quat_ideal_t *ideal, const quat_alg_elem_t *gen, const ibz_t *norm);
 *
 * int quat_random_ideal_O0_given_prime_norm(quat_ideal_t *ideal, const ibz_t *norm, prng_domain_ctx_t *prng_domain,);
 *
 * int quat_random_ideal_O0_given_arbitrary_odd_norm(quat_ideal_t *ideal, quat_alg_elem_t *split_gen, const
 * ibz_t *norm prng_domain_ctx_t *prng_domain);
 *
 * void quat_ideal_intersect_O0(quat_ideal_t *intersection, const quat_ideal_t *a, const quat_ideal_t
 * *b, const quat_alg_elem_t *split_a);
 *
 * int quat_ideal_small_equivalent_coprime(quat_alg_elem_t *gen, quat_ideal_t *equiv, const quat_ideal_t
 * *ideal, const ibz_t *coprime_to);
 */
int quat_test_ideal(void);

/** @brief Test for quaternion protocol functions
 *
 * void quat_response_element(quat_ideal_t *resp_ideal, quat_alg_elem_t *resp_split, const quat_ideal_t
 * *skideal, const quat_ideal_t *ideal_chall_two, const quat_ideal_t *ideal_commit,prng_domain_ctx_t *prng_domain);
 */
int quat_test_protocol(void);

/** @}
 */

#endif
