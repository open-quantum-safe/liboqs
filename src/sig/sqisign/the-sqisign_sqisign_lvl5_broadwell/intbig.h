/** @file
 *
 * @authors Luca De Feo, Sina Schaeffler
 *
 * @brief Declarations for big integers in the reference implementation
 */

#ifndef INTBIG_H
#define INTBIG_H

#include <sqisign_namespace.h>
#if defined(MINI_GMP)
#include <mini-gmp.h>
#include <mini-gmp-extra.h>
#else
#include <gmp.h>
#endif
#include <stdint.h>
#include <tutil.h>

/** @ingroup quat_quat
 * @defgroup ibz_all Signed big integers (gmp-based)
 * @{
 */

/** @defgroup ibz_t Precise number types
 * @{
 */

/** @brief Type for signed long integers
 *
 * @typedef ibz_t
 *
 * For integers of arbitrary size, used by intbig module, using gmp
 */
typedef mpz_t ibz_t;

/** @}
 */

/** @defgroup ibz_c Constants
 * @{
 */

/**
 * Constant zero
 */
extern const ibz_t ibz_const_zero;

/**
 * Constant one
 */
extern const ibz_t ibz_const_one;

/**
 * Constant two
 */
extern const ibz_t ibz_const_two;

/**
 * Constant three
 */
extern const ibz_t ibz_const_three;

/** @}
 */

/** @defgroup ibz_finit Constructors and Destructors
 * @{
 */

void ibz_init(ibz_t *x);
void ibz_finalize(ibz_t *x);

/** @}
 */

/** @defgroup ibz_za Basic integer arithmetic
 * @{
 */

/** @brief sum=a+b
 */
void ibz_add(ibz_t *sum, const ibz_t *a, const ibz_t *b);

/** @brief diff=a-b
 */
void ibz_sub(ibz_t *diff, const ibz_t *a, const ibz_t *b);

/** @brief prod=a*b
 */
void ibz_mul(ibz_t *prod, const ibz_t *a, const ibz_t *b);

/** @brief neg=-a
 */
void ibz_neg(ibz_t *neg, const ibz_t *a);

/** @brief abs=|a|
 */
void ibz_abs(ibz_t *abs, const ibz_t *a);

/** @brief Euclidean division of a by b
 *
 * Computes quotient, remainder so that remainder+quotient*b = a where 0<=|remainder|<|b|
 * The quotient is rounded towards zero.
 */
void ibz_div(ibz_t *quotient, ibz_t *remainder, const ibz_t *a, const ibz_t *b);

/** @brief Euclidean division of a by 2^exp
 *
 * Computes a right shift of abs(a) by exp bits, then sets sign(quotient) to sign(a).
 *
 * Division and rounding is as in ibz_div.
 */
void ibz_div_2exp(ibz_t *quotient, const ibz_t *a, uint32_t exp);

/** @brief Two adic valuation computation
 *
 * Computes the position of the first 1 in the binary representation of the integer given in input
 *
 * When this number is a power of two this gives the two adic valuation of the integer
 */
int ibz_two_adic(ibz_t *pow);

/** @brief r = a mod b
 *
 * Assumes valid inputs
 * The sign of the divisor is ignored, the result is always non-negative
 */
void ibz_mod(ibz_t *r, const ibz_t *a, const ibz_t *b);

unsigned long int ibz_mod_ui(const mpz_t *n, unsigned long int d);

/** @brief Test if a = 0 mod b
 */
int ibz_divides(const ibz_t *a, const ibz_t *b);

/** @brief pow=x^e
 *
 * Assumes valid inputs, The case 0^0 yields 1.
 */
void ibz_pow(ibz_t *pow, const ibz_t *x, uint32_t e);

/** @brief pow=(x^e) mod m
 *
 * Assumes valid inputs
 */
void ibz_pow_mod(ibz_t *pow, const ibz_t *x, const ibz_t *e, const ibz_t *m);

/** @brief Compare a and b
 *
 * @returns a positive value if a > b, zero if a = b, and a negative value if a < b
 */
int ibz_cmp(const ibz_t *a, const ibz_t *b);

/** @brief Test if x is 0
 *
 * @returns 1 if x=0, 0 otherwise
 */
int ibz_is_zero(const ibz_t *x);

/** @brief Test if x is 1
 *
 * @returns 1 if x=1, 0 otherwise
 */
int ibz_is_one(const ibz_t *x);

/** @brief Compare x to y
 *
 * @returns 0 if x=y, positive if x>y, negative if x<y
 */
int ibz_cmp_int32(const ibz_t *x, int32_t y);

/** @brief Test if x is even
 *
 * @returns 1 if x is even, 0 otherwise
 */
int ibz_is_even(const ibz_t *x);

/** @brief Test if x is odd
 *
 * @returns 1 if x is odd, 0 otherwise
 */
int ibz_is_odd(const ibz_t *x);

/** @brief set i to value x
 */
void ibz_set(ibz_t *i, int32_t x);

/** @brief Copy value into target
 */
void ibz_copy(ibz_t *target, const ibz_t *value);

/** @brief Exchange values of a and b
 */
void ibz_swap(ibz_t *a, ibz_t *b);

/** @brief Copy dig array to target, given digits and the length of the dig array
 *  Restriction: dig represents a non-negative integer
 *
 *  @param target Target ibz_t element
 *  @param dig array of digits
 *  @param dig_len length of the digits array
 */
void ibz_copy_digits(ibz_t *target, const digit_t *dig, int dig_len);
#define ibz_copy_digit_array(I, T)                                                                                     \
    do {                                                                                                               \
        ibz_copy_digits((I), (T), sizeof(T) / sizeof(*(T)));                                                           \
    } while (0)

// void ibz_printf(const char* format, ...);
#define ibz_printf gmp_printf

/** @brief Copy an ibz_t to target digit_t array.
 *  Restrictions: ibz >= 0 and target must hold sufficient elements to hold ibz
 *
 *  @param target Target digit_t array
 *  @param ibz ibz source ibz_t element
 */
void ibz_to_digits(digit_t *target, const ibz_t *ibz);
#define ibz_to_digit_array(T, I)                                                                                       \
    do {                                                                                                               \
        memset((T), 0, sizeof(T));                                                                                     \
        ibz_to_digits((T), (I));                                                                                       \
    } while (0)

/** @brief get int32_t equal to the lowest bits of i
 *
 * Should not be used to get the value of i if its bitsize is close to 32 bit
 * It can however be used on any i to get an int32_t of the same parity as i (and same value modulo
 * 4)
 *
 * @param i Input integer
 */
int32_t ibz_get(const ibz_t *i);

/** @brief generate random value in [a, b]
 *  assumed that a >= 0 and b >= 0 and a < b
 * @returns 1 on success, 0 on failiure
 */
int ibz_rand_interval(ibz_t *rand, const ibz_t *a, const ibz_t *b);

/** @brief generate random value in [-m, m]
 *  assumed that m > 0 and bitlength of m < 32 bit
 * @returns 1 on success, 0 on failiure
 */
int ibz_rand_interval_minm_m(ibz_t *rand, int32_t m);

/** @brief Bitsize of a.
 *
 *  @returns Bitsize of a.
 *
 */
int ibz_bitsize(const ibz_t *a);

/** @brief Size of a in given base.
 *
 *  @returns Size of a in given base.
 *
 */
int ibz_size_in_base(const ibz_t *a, int base);

/** @}
 */

/** @defgroup ibz_n Number theory functions
 * @{
 */

/**
 * @brief Greatest common divisor
 *
 * @param gcd Output: Set to the gcd of a and b
 * @param a
 * @param b
 */
void ibz_gcd(ibz_t *gcd, const ibz_t *a, const ibz_t *b);

/**
 * @brief Modular inverse
 *
 * @param inv Output: Set to the integer in [0,mod[ such that a*inv = 1 mod (mod) if it exists
 * @param a
 * @param mod
 * @returns 1 if inverse exists and was computed, 0 otherwise
 */
int ibz_invmod(ibz_t *inv, const ibz_t *a, const ibz_t *mod);

/**
 * @brief Floor of Integer square root
 *
 * @param sqrt Output: Set to the floor of an integer square root
 * @param a number of which a floor of an integer square root is searched
 */
void ibz_sqrt_floor(ibz_t *sqrt, const ibz_t *a);

/** @}
 */

// end of ibz_all
/** @}
 */
#endif
