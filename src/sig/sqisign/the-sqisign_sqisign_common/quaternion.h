/** @file
 *
 * @authors Luca De Feo, Sina Schaeffler
 *
 * @brief Declarations for quaternion algebra operations
 */

#ifndef QUATERNION_H
#define QUATERNION_H

// #include <rng.h>
#include <sqisign_namespace.h>
#include <mp.h>
#include <assert.h>
#include <tools.h>

/** @defgroup quat_quat Quaternion algebra
 * @{
 */

/** @defgroup quat_vec_t Types for integer vectors and matrices
 * @{
 */

/** @brief Type for vector of 2 big integers
 *
 * @typedef ibz_vec_2_t
 */
typedef struct ibz_vec_2_t
{
    ibz_t v[2];
} ibz_vec_2_t; // struct so that const pointers to it are valid ISO C11

/** @brief Type for vectors of 4 integers
 *
 * @typedef ibz_vec_4_t
 *
 * Represented as a vector of 4 ibz_t (big integer) elements
 */
typedef struct ibz_vec_4_t
{
    ibz_t v[4];
} ibz_vec_4_t;

/** @brief Type for 2 by 2 matrices of integers
 *
 * @typedef ibz_mat_2x2_t
 *
 * Represented as a matrix of 2 vectors of 2 ibz_t (big integer) elements
 */
typedef struct ibz_mat_2x2_t
{
    ibz_t m[2][2];
} ibz_mat_2x2_t;

/** @brief Type for 4 by 4 matrices of integers
 *
 * @typedef ibz_mat_4x4_t
 *
 * Represented as a matrix of 4 vectors of 4 ibz_t (big integer) elements
 */
typedef struct ibz_mat_4x4_t
{
    ibz_t m[4][4];
} ibz_mat_4x4_t;
/**
 * @}
 */

/** @defgroup quat_quat_t Types for quaternion algebras
 * @{
 */

/** @brief Type for quaternion algebras
 *
 * @typedef quat_alg_t
 *
 * @struct quat_alg
 *
 * The quaternion algebra ramified at p = 3 mod 4 and ∞.
 */
typedef struct quat_alg
{
    ibz_t p; ///< Prime number, must be = 3 mod 4.
} quat_alg_t;

/** @brief Type for quaternion algebra elements
 *
 * @typedef quat_alg_elem_t
 *
 * @struct quat_alg_elem
 *
 * Represented as a array *coord* of 4 ibz_t integers and a common ibz_t denominator *denom*.
 *
 * The representation is not necessarily normalized, that is, gcd(denom, content(coord)) might not be 1.
 * For getting a normalized representation, use the quat_alg_normalize function
 *
 * The elements are always represented in basis (1,i,j,ij) of the quaternion algebra, with i^2=-1 and j^2 = -p
 */
typedef struct quat_alg_elem
{
    ibz_t denom;       ///< Denominator by which all coordinates are divided (big integer, must not be 0)
    ibz_vec_4_t coord; ///< Numerators of the 4 coordinates of the quaternion algebra element in basis (1,i,j,ij)
} quat_alg_elem_t;

/** @brief Type for lattices in dimension 4
 *
 * @typedef quat_lattice_t
 *
 * @struct quat_lattice
 *
 * Represented as a rational (`frac`) times an integreal lattice (`basis`)
 *
 * The basis is such that its columns divided by its denominator are elements of the quaternion algebra,
 * represented in basis (1,i,j,ij) where i^2 = -1, j^2 = -p.
 *
 * All lattices must have full rank (4)
 */
typedef struct quat_lattice
{
    ibz_t denom;         ///< Denominator by which the basis is divided (big integer, must not be 0)
    ibz_mat_4x4_t basis; ///< Integer basis of the lattice  (its columns divided by denom are algebra elements
                         ///< in the usual basis)
} quat_lattice_t;

/** @brief Type for left ideals of maximal orders in quaternion algebras
 *
 * @typedef quat_ideal_t
 *
 * @struct quat_ideal
 *
 * Left O0-ideal in inert HNF
 *
 * Basis in inert HNF on denominator 2
 *
 * 2norm  0     2x      2(n-y)-1
 *  0    2norm 2y+1       2x
 *  0     0      1        0
 *  0     0      0        1
 *
 */
typedef struct quat_ideal
{
    ibz_t x;    ///< x of an ideal in inert HNF
    ibz_t y;    ///< y of an ideal in inert HNF
    ibz_t norm; ///< norm of the lattice
} quat_ideal_t;

/** @}
 */

/** @defgroup quat_c Initializers
 * @{
 */
void quat_alg_init_set(quat_alg_t *alg, const ibz_t *p);
void quat_alg_elem_init(quat_alg_elem_t *elem);
void ibz_vec_2_init(ibz_vec_2_t *vec);
void ibz_vec_4_init(ibz_vec_4_t *vec);
void ibz_mat_2x2_init(ibz_mat_2x2_t *mat);
void ibz_mat_4x4_init(ibz_mat_4x4_t *mat);
void quat_lattice_init(quat_lattice_t *lat);
void quat_ideal_init(quat_ideal_t *ideal);
/** @}
 */

/** @defgroup quat_printers Print functions for types from the quaternion module
 * @{
 */
void ibz_mat_2x2_print(const ibz_mat_2x2_t *mat);
void ibz_mat_4x4_print(const ibz_mat_4x4_t *mat);
void ibz_vec_2_print(const ibz_vec_2_t *vec);
void ibz_vec_4_print(const ibz_vec_4_t *vec);

void quat_lattice_print(const quat_lattice_t *lat);
void quat_alg_print(const quat_alg_t *alg);
void quat_alg_elem_print(const quat_alg_elem_t *elem);
void quat_ideal_print(const quat_ideal_t *ideal);

/** @}
 */

/** @defgroup quat_int Integer functions for quaternion algebra
 * @{
 */

/** @defgroup quat_int_mat Integer matrix and vector functions
 * @{
 */

/**
 * @brief Inverse of 2x2 integer matrices modulo m
 *
 * @param inv Output matrix
 * @param mat Input matrix
 * @param m Integer modulo
 * @return 1 if inverse exists 0 otherwise
 */
int ibz_mat_2x2_inv_mod(ibz_mat_2x2_t *inv, const ibz_mat_2x2_t *mat, const ibz_t *m);

/** @brief mat*vec in dimension 2 for integers
 *
 * @param res Output vector
 * @param mat Input matrix
 * @param vec Input vector
 */
void ibz_mat_2x2_eval(ibz_vec_2_t *res, const ibz_mat_2x2_t *mat, const ibz_vec_2_t *vec);

/** @brief NormalizeMatrix()
 *
 * @param mat Output/Input matrix
 * @param e Input int
 */
void ibz_mat_2x2_normalize(ibz_mat_2x2_t *mat, int e);

/** @}
 */

/** @defgroup quat_integer Higher-level integer functions for quaternion algebra
 * @{
 */

/**
 * @brief Generates a random prime
 *
 * A number is accepted as prime if it passes a QUAT_primality_num_iter-round Miller-Rabin test.
 * This function is fairly inefficient and mostly meant for tests.
 *
 * @returns 1 if a prime is found, 0 otherwise
 * @param p Output: The prime (if found)
 * @param is3mod4 If 1, the prime is required to be 3 mod 4, if 0 no congruence condition is imposed
 * @param bitsize Maximal size of output prime
 */
int ibz_generate_random_prime(ibz_t *p, int is3mod4, int bitsize);

/** @}
 */

/** @}
 */

/** @defgroup quat_quat_f Quaternion algebra functions
 * @{
 */

/**
 * @brief Copies an algebra element
 *
 * @param copy Output: The element into which another one is copied
 * @param copied Source element copied into copy
 */
void quat_alg_elem_copy(quat_alg_elem_t *copy, const quat_alg_elem_t *copied);

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

/**
 * @brief a*b for quaternion algebra elements
 *
 * @param res Output: will be set to the product ab (in this order)
 * @param a Input quaternion element
 * @param b Input quaternion element
 * @param alg The quaternion algebra
 */
void quat_alg_mul(quat_alg_elem_t *res, const quat_alg_elem_t *a, const quat_alg_elem_t *b, const quat_alg_t *alg);

/**
 * @brief Standard involution in a quaternion algebra
 *
 * @param conj Output: image of x by standard involution of the quaternion algebra alg
 * @param x element of alg whose image is searched
 */
void quat_alg_conj(quat_alg_elem_t *conj, const quat_alg_elem_t *x);

/** @brief reduced norm of alg_elem x
 *
 * @param res_num Output: rational which will contain the numerator of the reduced norm of a
 * @param res_denom Output: rational which will contain the denominator of the reduced norm of a
 * (it is 1 if the norm is integer)
 * @param x Algebra element whose norm is computed
 * @param alg The quaternion algebra
 */
void quat_alg_norm(ibz_t *res_num, ibz_t *res_denom, const quat_alg_elem_t *x, const quat_alg_t *alg);

/**
 * @brief Multiply quaternion element with integer
 *
 * No normalization of the output
 *
 * @param prod Output: scalar*elem
 * @param scalar Integer
 * @param elem Quaternion
 */
void quat_alg_elem_scalar_mul(quat_alg_elem_t *prod, const ibz_t *scalar, const quat_alg_elem_t *elem);

/**
 * @brief Given x an element of order, factor it into its primitive and imprimitive parts
 *
 * Given x in order, return a coordinate vector primitive_x
 * and an integer content such that x = content · order primitive_x,
 * where order is given by a basis and x / content is primitive in order.
 *
 * @param primitive_x Output: coordinates of a primitive element of order (in order's basis)
 * @param content Output: content of x's coordinate vector in order's basis
 * @param order order of alg
 * @param x element of order, must be in order
 */
void quat_alg_make_primitive(ibz_vec_4_t *primitive_x,
                             ibz_t *content,
                             const quat_alg_elem_t *x,
                             const quat_lattice_t *order);

/** @brief divides all values in vector by same scalar
 *
 * @returns 1 if scalar divided all values in mat, 0 otherwise (division is performed in both cases)
 * @param quot Output
 * @param scalar
 * @param vec
 */
int ibz_vec_4_scalar_div(ibz_vec_4_t *quot, const ibz_t *scalar, const ibz_vec_4_t *vec);
// end quat_quat_f
/** @}
 */

/** @defgroup quat_f Functions specific to O0 ideals
 * @{
 */
/**
 * @brief InertGenToIdOdd: Create O0 ideal in inert HNF form from norm and generator if it exists
 *
 * @param ideal Output: left ideal of O0 generated by norm, gen
 * @param gen Generator of the ideal. A O0-element of norm multiple of norm
 * @param norm Odd positive integer dividing nrd(gen)
 * @returns 1 on success, 0 if failure due to ideal not having an inert HNF
 */
int quat_ideal_create_O0_inert_odd(quat_ideal_t *ideal, const quat_alg_elem_t *gen, const ibz_t *norm);

/**
 * @brief GenToIdPowTwo: Create power-of-two norm O0 ideal from norm and generator
 *
 * This only works if the ideal+2O0 does not contain i+1
 *
 * @param ideal Output: ideal of norm norm
 * @param gen Generator of the ideal. A O0-element of norm multiple of norm
 * @param norm 2^e a power of 2
 *
 * @returns 1 on success, 0 if i+1 was in ideal+2O0 (no inert ideal basis exists)
 */
int quat_ideal_create_O0_pow_two(quat_ideal_t *ideal, const quat_alg_elem_t *gen, const ibz_t *norm);

/**
 * @brief PrimeNormIdealInertSampling: Create uniformly random O0 ideal of given prime norm
 *
 * @param ideal Output: ideal of norm norm in inert HNF
 * @param norm Odd prime
 * @param prng_domain Randomness domain used
 * @returns 1 on success, 0 on randomness failure. Infinite loop if no solution exists
 */
int quat_random_ideal_O0_given_prime_norm(quat_ideal_t *ideal, const ibz_t *norm, prng_domain_ctx_t *prng_domain);

/**
 * @brief RandomIdealGivenNorm: Create uniformly random O0 ideal of given odd norm
 *
 * Output ideal is of the form ideal*split_gen
 *
 * @param ideal Output: inert part of ideal of norm norm
 * @param split_gen Output: generator of the split part of the output ideal
 * @param norm Odd positive integer, must be large enough that norm*QUAT_prime_cofactor>plog(p) where p is the algebra
 * parameter
 * @param shift_gen Quaternion element that the generator of the new ideal is multiplied with. This multipication
 * preserves the uniform distribution, and only serves to choose a canonical auxiliary ideal class. The norm of this
 * element must be coprime to norm.
 * @param prng_domain Randomness domain used
 * @returns 1 on success, 0 on randomness or representInteger failure. The latter may occur for too small input norm
 */
int quat_random_ideal_O0_given_arbitrary_odd_norm(quat_ideal_t *ideal,
                                                  quat_alg_elem_t *split_gen,
                                                  const ibz_t *norm,
                                                  const quat_alg_elem_t *shift_gen,
                                                  prng_domain_ctx_t *prng_domain);

/**
 * @brief Intersect: Intersection of odd norm O0-ideals of coprime norms
 *
 * @param intersection Output: inert HNF of I such that I=intersection*split_a= a*split_a intersect b
 * @param a Odd norm O0-ideal in inert HNF
 * @param b Odd norm O0-ideal in inert HNF of norm coprime to norm(a)norm(split_a)
 * @param split_a Split factor multiplied to a to obtain input ideal a (u+iv). Can be NULL to indicate 1+0i (that means
 * a inert)
 */
void quat_ideal_intersect_O0(quat_ideal_t *intersection,
                             const quat_ideal_t *a,
                             const quat_ideal_t *b,
                             const quat_alg_elem_t *split_a);

/**
 * @brief EquivalentCoprimeIdeal: Find odd norm small equivalent inert ideal with required (co)primality given an ideal
 *
 *
 * @param gen Output: Generator of the output ideal, can be NULL
 * @param equiv Output: equivalent ideal, generated by gen and its norm. Norm is always odd
 * @param ideal Input ideal, basis must be reduced
 * @param coprime_to Integer input. If 0, equiv.norm is prime, else coprime to coprime_to. If NULL or 2, output is an
 * odd equivalent ideal
 * @param prng_domain Randomness domain used for sampling
 * @returns 1 on success, 0 on randomness failure. Can loop indefinitely if no solution exists.
 */
int quat_ideal_small_equivalent_coprime(quat_alg_elem_t *gen,
                                        quat_ideal_t *equiv,
                                        const quat_ideal_t *ideal,
                                        const ibz_t *coprime_to,
                                        prng_domain_ctx_t *prng_domain);

// end quat_f
/** @}
 */
/** @defgroup quat_protocol_f Functions specific to O0 ideals and the SQIsign protocol
 * @{
 */

/**
<<<<<<< HEAD
 * @brief QuaternionResponseComputation: Function to compute a response ideal
 *
 * Computes sk_chall_ideal and and resp_quat, the response ideal's norm and sk_chall_quat to obtain sk_chall_ideal
=======
 * @brief QuaternionResponseComputation: Function to compute an response ideal resp
>>>>>>> main
 *
 * @param sk_chall_ideal Output:  ideal of small prime norm equivalent to the intersection of skideal with
 * ideal_chall_two
 * @param resp_quat Output: resp_quat quaternion element defining the response isogeny of SQIsign, contained in
 * conjugate(sk_chall_ideal) * ideal_commit, of norm equal to norm * n(ideal_commit ) * n(sk_chall_ideal)
 * it corresponds to the endomorphism of E0 made of the composition of dual(phi_sk_chall) o phi_response o phi_commit
 * where phi_response has degree equal to norm. Can be non-primitive in O0.
 * @param sk_chall_quat Output: Quaternion used to obtain sk_chall_ideal from the intersection of the sk and chall
 * ideals It snorm is the product of the norms of sk_chall_ideal, sk_ideal anf ideal_chall_two
 * @param norm Output: norm of the response ideal, defined above
 * @param sk_ideal input; The ideal correspoding the the secret key
 * @param ideal_chall_two Challenge ideal (pulled back to O0 through skideal)
 * @param chall_split Split part of the challenge ideal (pulled back to O0 through skideal)
 * @param ideal_commit Commitment ideal (prime norm representative in inert HNF)
 * @param prng_domain Randomness domain
 * @param e 2^e is the upper bound on the response norm
 */
void quat_response_element(quat_ideal_t *sk_chall_ideal,
                           quat_alg_elem_t *resp_quat,
                           ibz_t *norm,
                           quat_alg_elem_t *sk_chall_quat,
                           const quat_ideal_t *sk_ideal,
                           const quat_ideal_t *ideal_chall_two,
                           const quat_alg_elem_t *chall_split,
                           const quat_ideal_t *ideal_commit,
                           prng_domain_ctx_t *prng_domain,
                           int e);
// end quat_protocol_f
/** @}
 */

/** @defgroup quat_stdorder Functions specific to O0
 * @{
 */
/** @brief Basis change to (1,i,(i+j)/2,(1+ij)/2) for elements of O0
 *
 * Change the basis in which an element is give from 1,i,j,ij to (1,i,(i+j)/2,(1+ij)/2)
 * the usual basis of the special maximal order O0.
 * Only for elements of O0.
 *
 * @param vec Output: Coordinates of el in basis (1,i,(i+j)/2,(1+ij)/2)
 * @param el Imput: An algebra element in O0
 */
void quat_change_to_O0_basis(ibz_vec_4_t *vec, const quat_alg_elem_t *el);

/**
 * @brief Finds two ideals equivalent to ideal such that norm(I1)+norm(I2)= 2^two_power
 *
 * Uses quat_qlapoty_normeq and computes output suitable for id2iso
 *
 * @param beta1 Output: quaternion in ideal
 * @param d1 Output: norm(beta1)/norm(ideal)
 * @param theta Output: conj(beta2*conj(beta1)/norm(ideal)) such that norm(beta1)+norm(beta2)=norm(ideal)2^two_power
 * @param ideal Input ideal. Must be left ideal of O0
 * @returns 1 if success, 0 if failure. Failure can occur on inputs ideals with an equivalent of very short norm
 */
int quat_qlapoty(quat_alg_elem_t *beta1, ibz_t *d1, quat_alg_elem_t *theta, const quat_ideal_t *ideal);

// end quat_stdorder
/** @}
 */
// end quat_quat
/** @}
 */
#endif
