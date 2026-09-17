/** @file
 *
 * @authors Gora Adj, Isaac Canales, Jorge Chávez, Décio Gazzoni, Francisco Rodríguez
 *
 * @brief Declarations for big integers
 */

#ifndef MP_H
#define MP_H

#include <stdint.h>
#include <tutil.h>
#include <sqisign_namespace.h>
#include <mp_internal.h>

typedef struct prng_domain_ctx prng_domain_ctx_t;

#define NUM_BITS_LIMB ((int)(8 * sizeof(digit_t)))

/* Number of digit_t limbs needed to hold `bits` bits. */
#define NLIMBS(bits) ((int)(((bits) + (NUM_BITS_LIMB - 1)) / (NUM_BITS_LIMB)))

/* Round `bits` up to a whole limb, and a 2-arg max. */
#define LIMB_ALIGN(bits) (NLIMBS(bits) * NUM_BITS_LIMB)
#define MAXB(a, b) ((a) > (b) ? (a) : (b))

/* floor(log2(x)) for x > 0. */
#define LOG2(x) (63 - __builtin_clzll((unsigned long long)(x)))

/* The full width of an ibz_t, in bits: no value may exceed this. */
#define IBZ_MAX_BITS (IBZ_NLIMBS * NUM_BITS_LIMB)

/** @ingroup quat_quat
 * @defgroup ibz_all Signed big integers
 * @{
 */

/** @defgroup ibz_t Precise number types
 * @{
 */

/**
 * @brief Type for signed big integers of up to IBZ_NLIMBS limbs
 *
 * @typedef ibz_t
 *
 * Fixed-size signed integer backed by a digit_t limb array.
 * `limbs` is the digit array in two's complement representation (the signed bit is the most significant bit of the
 * highest limb).
 * `bitlen` is a bound for the number of bits of the signed value in two's complement, sign bit included:
 * -2^(bitlen-1) < value < 2^(bitlen-1).
 */
struct ibz_t;
typedef struct ibz_t ibz_t;

/** @}
 */

/** @defgroup ibz_c Constants
 * @{
 */

extern const ibz_t ibz_const_zero;
extern const ibz_t ibz_const_one;
extern const ibz_t ibz_const_two;
extern const ibz_t ibz_const_three;

/** @}
 */

/** @defgroup ibz_zi Initialization and Serialization
 * @{
 */

/**
 * @brief Initializes to 0
 *
 * @param t Integer to be initialized
 */
void ibz_init(ibz_t *t);

/**
 * @brief Interprets the lowest bitlen bits of x as a signed integer (in two's complement representation), sets this
 * value to a and sets the bit length of a to bitlen.
 *
 * Valid values for bitlen are in the interval 1 to NUM_BITS_LIMB, inclusive. If bitlen is zero, a is set to zero and
 * the bit length of a is set to one. If bitlen is greater than 32, bitlen is considered to be 32.
 *
 * If the lowest bitlen bits of x are equal to -2^(bitlen-1), a is set to zero.
 */
void ibz_set(ibz_t *a, sdigit_t x, int bitlen);

/**
 * @brief Copy value into target
 */
void ibz_copy(ibz_t *target, const ibz_t *value);

/**
 * @brief Exchange the values of a and b
 */
void ibz_swap(ibz_t *a, ibz_t *b);

/**
 * @brief Exchange the values of a and b according to the value of cond:
 *
 * - If cond has all its bits set to 1, a and b are exchanged;
 * - If cond has all its bits set to 0, a and b are not exchanged;
 * - Otherwise, the behaviour is undefined and most likely an erroneous result is computed.
 */
void ibz_cswap(ibz_t *a, ibz_t *b, digit_t cond);

/**
 * @brief Sets the bit length of a to bitlen (i.e., the value of a is in the interval -2^(bitlen-1) + 1 to
 * 2^(bitlen-1) - 1, inclusive) and updates the value of a as follows:
 *
 * - If bitlen is greater or equal than the current bit length of a, the value of a is not changed;
 * - Otherwise, the current value of a is truncated to bitlen bits and those bits are interpreted as an integer in the
 *   interval -2^(bitlen-1)+1 to 2^(bitlen-1)-1, inclusive. If the truncated value is equal to -2^(bitlen-1), a is set
 *   to zero. If bitlen is zero, a is set to zero and the bit length of a is set to one.
 */
void ibz_set_bound(ibz_t *a, int bitlen);

/**
 * @brief Returns the current bitlen bound for x
 */
int ibz_get_bound(const ibz_t *x);

/**
 * @brief Copy little-endian unsigned digit array to target.
 *
 * Since the source represents a non-negative integer of bitlen bits, target is set to have numwords*sizeof(digit_t)+1
 * bits of precision.
 *
 *  @param target Target ibz_t element
 *  @param source array of digits
 *  @param bitlen number of bits to import
 */
void ibz_copy_bits(ibz_t *target, const digit_t *source, int bitlen);

/**
 * @brief Copy little-endian unsigned digit array to target, up to numwords words.
 *
 * Since the source represents a non-negative integer of bitlen bits, target is set to have NUMBITS_LIMB*numwords+1 bits
 * of precision
 *
 *  @param target Target ibz_t element
 *  @param source array of digits
 *  @param numwords number of words to import
 */
void ibz_copy_digits(ibz_t *target, const digit_t *source, int numwords);

#define ibz_copy_digit_array(I, T)                                                                                     \
    do {                                                                                                               \
        ibz_copy_digits((I), (T), sizeof(T) / sizeof(digit_t));                                                        \
    } while (0)

/**
 * @brief Copy a non-negative ibz_t to the target digit_t array.
 *
 *  @param target Target digit_t array
 *  @param source ibz_t element
 */
void ibz_to_digits(digit_t *target, const ibz_t *source);

#define ibz_to_digit_array(T, I)                                                                                       \
    do {                                                                                                               \
        memset((T), 0, sizeof(T));                                                                                     \
        assert((ssize_t)sizeof((T)) >= ibz_size_in_base((I), 256));                                                    \
        ibz_to_digits((T), (I));                                                                                       \
    } while (0)

/** @brief Get int32_t equal to i mod 2^32
 *
 * @param i Input integer
 */
int32_t ibz_get(const ibz_t *i);

/**/

/** @brief Writes the representation of a big integer either in base 10 or base 16 to a string buffer
 */
int ibz_convert_to_str(const ibz_t *i, char *str, int base);

/** @brief Imports a big integer from a string buffer in base 10 or base 16
 */
int ibz_set_from_str(ibz_t *i, const char *str, int base);

/** @brief Prints a big integer in base 10 or base 16 to stdout
 */
int ibz_print(const ibz_t *num, int base);

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

/** @brief out=-a if mask==-1, out=a if mask==0
 */
void ibz_cneg(ibz_t *out, const ibz_t *a, digit_t mask);

/** @brief abs=|a|
 */
void ibz_abs(ibz_t *abs, const ibz_t *a);

/** @brief Euclidean division of a by b
 *
 * Computes quotient, remainder so that remainder+quotient*b = a where 0<=|remainder|<|b|
 * The quotient is rounded towards zero.
 */
void ibz_div(ibz_t *quotient, ibz_t *remainder, const ibz_t *a, const ibz_t *b);

/** @brief Arithmetic right shift of a by exp bits
 *
 * Computes quotient = floor(a / 2^exp), rounding towards NEGATIVE INFINITY, not towards zero.
 */
void ibz_div_2exp(ibz_t *quotient, const ibz_t *a, uint32_t exp);

/** @brief Multiplication of a by 2^exp
 *
 * Computes product = a * 2^exp as a left shift of a's two's-complement representation, truncated
 * to IBZ_NLIMBS * NUM_BITS_LIMB bits.
 */
void ibz_mul_2exp(ibz_t *product, const ibz_t *a, uint32_t exp);

/** @brief Two adic valuation computation
 *
 * Computes the position of the first 1 in the binary representation of the integer given in input
 *
 * When this number is a power of two this gives the two adic valuation of the integer
 */
int ibz_two_adic(const ibz_t *pow);

/**
 * @brief Floor of Integer square root
 *
 * @param sqrt Output: Set to the floor of an integer square root
 * @param a number of which a floor of an integer square root is searched
 */
void ibz_sqrt_floor(ibz_t *sqrt, const ibz_t *a);

/** @brief pow=x^e
 *
 * Truncates e to ebits bits, then computes pow=x^e. Assumes valid inputs. The case 0^0 yields 0.
 */
void ibz_pow(ibz_t *pow, const ibz_t *x, uint32_t e, int ebits);

/** @brief Compare a and b
 *
 * @returns +1 if a > b, zero if a = b, and -1 if a < b
 */
int ibz_cmp(const ibz_t *a, const ibz_t *b);

/** @brief Test if x is 0
 *
 * @returns -1 if x=0, 0 otherwise
 */
int ibz_is_zero(const ibz_t *x);

/** @brief Test if x is 1
 *
 * @returns -1 if x=1, 0 otherwise
 */
int ibz_is_one(const ibz_t *x);

/** @brief Test if x >= 0
 *
 * @returns -1 if x >= 0, 0 otherwise
 */
int ibz_is_positive(const ibz_t *x);

/** @brief Compare x to y
 *
 * @returns 0 if x=y, +1 if x>y, -1 if x<y
 */
int ibz_cmp_int32(const ibz_t *x, int32_t y);

/** @brief Test if x is even
 *
 * @returns -1 if x is even, 0 otherwise
 */
int ibz_is_even(const ibz_t *x);

/** @brief Test if x is odd
 *
 * @returns -1 if x is odd, 0 otherwise
 */
int ibz_is_odd(const ibz_t *x);

/** @brief generate random value in [a, b]
 *
 * assumes that b>a
 *
 * @param rand Output: Integer in range [-m,m]
 * @param domain seeded PRNG domain context to sample from
 * @param a lower sampling bound
 * @param b higher sampling bound
 * @returns -1 on success, 0 on failure
 */
int ibz_rand_interval_with_domain(ibz_t *rand, const ibz_t *a, const ibz_t *b, prng_domain_ctx_t *domain);

/** @brief generate random value in [a, b], sampling from prng_default_domain
 *  assumes that a < b, and b is at least 64 bits less than the global bound.
 *
 *  Use ibz_rand_interval_with_domain() to sample from a specific domain instead.
 * @returns -1 on success, 0 on failure
 */
int ibz_rand_interval(ibz_t *rand, const ibz_t *a, const ibz_t *b);

/** @brief generate random value in [-m, m]
 *
 *  assumes that m > 0.
 *  assumed that m > 0 and bitlength of m < 32 bit
 *
 * @param rand Output: Integer in range [-m,m]
 * @param domain seeded PRNG domain context to sample from
 * @param m sampling bound
 * @returns -1 on success, 0 on failure
 */
int ibz_rand_interval_minm_m_with_domain(ibz_t *rand, int32_t m, prng_domain_ctx_t *domain);

/** @brief generate random value in [-m, m], sampling from prng_default_domain
 *  assumes that m > 0.
 *
 *  Use ibz_rand_interval_minm_m_with_domain() to sample from a specific domain instead.
 * @returns -1 on success, 0 on failure
 */
int ibz_rand_interval_minm_m(ibz_t *rand, int32_t m);

/** @brief Bitsize of a, equal to ceil(log2(|a|+1))
 *
 *  @returns Bitsize of a.
 *
 */
int ibz_bitsize(const ibz_t *a);

/** @brief Size of a in given base, equal to ceil(log_base(|a|+1))
 *
 *  @returns Size of a in given base.
 *
 */
int ibz_size_in_base(const ibz_t *a, int base);

/** @brief Constant-time variant of ibz_set_bound
 *
 * Identical result, but with no data-dependent control flow: the decrease branch masks and
 * sign-extends the top limb instead of calling normalise(), whose low-limb scan has a
 * value-dependent trip count. Use this on secret data; ibz_set_bound remains the right choice
 * everywhere else.
 */
void ibz_set_bound_ct(ibz_t *a, int bitlen);

/** @brief Constant-time variant of ibz_bitsize
 *
 * Identical result, but scans every limb and selects with a mask rather than breaking out of the
 * scan early. Slower than ibz_bitsize by a factor that grows with IBZ_NLIMBS, so use it only
 * where the value is secret.
 */
int ibz_bitsize_ct(const ibz_t *a);

/** @brief Multiplies by a small integer and sets the bound of the result to a known value. The caller must ensure that
 * the bound is correct, i.e. |prod| < 2^(result_bound-1).
 *
 */
void ibz_mul_by_int_and_set_bound(ibz_t *prod, const ibz_t *a, int32_t b, int result_bound);

/** @brief Adds two big integers and sets the bound of the result to a known value. The caller must ensure that the
 * bound is correct, i.e. |sum| < 2^(result_bound-1).
 *
 */
void ibz_add_and_set_bound(ibz_t *sum, const ibz_t *a, const ibz_t *b, int result_bound);

/** @brief Adds a big integers and a small one, and sets the bound of the result to a known value. The caller must
 * ensure that the bound is correct, i.e. |sum| < 2^(result_bound-1).
 *
 */
void ibz_add_int_and_set_bound(ibz_t *sum, const ibz_t *a, int32_t b, int result_bound);

/** @brief Extracts an unsigned integer corresponding to bits [offset:offset+bitlen] of a, which is assumed to be
 * non-negative.
 *
 */
uint64_t ibz_extract_u64(const ibz_t *a, int offset);

/** @brief Extracts a signed integer with magnitude corresponding to bits [offset:offset+bitlen] of |a| and sign equal
 * to the sign(a).
 *
 * NOTE: bit 63 of the extracted magnitude is masked off, so this returns at most 63 magnitude
 * bits. The 64th bit of the window is discarded.
 */
int64_t ibz_extract_i64(const ibz_t *a, int offset);

/** @brief Masked logarithmic shift. shift may be SECRET; max_shift and out_bitlen are PUBLIC.
 *
 * shift >= 0 : left (exact, truncated to out_bitlen).
 * shift < 0  : arithmetic (floor) right -- not ibz_div_2exp, which truncates toward zero.
 * Requires: |shift| <= max_shift.
 */
void ibz_ct_shift(ibz_t *res, const ibz_t *a, int32_t shift, int32_t max_shift, int out_bitlen);

/** @brief res = floor((a*b) / 2^P). P is a public constant.
 */
void ibz_ct_highmul_p(ibz_t *res, const ibz_t *a, const ibz_t *b, int P, int out_bitlen);

/** @brief res = trunc_{out_bitlen}(a*b * 2^shift), shift SECRET (bounded by max_shift).
 *
 * Same trunc(...) convention as ibz_ct_shift.
 */
void ibz_ct_highmul_s(ibz_t *res, const ibz_t *a, const ibz_t *b, int32_t shift, int32_t max_shift, int out_bitlen);

/** @brief Mask-returning comparisons: all-ones (digit_t)-1 iff the predicate holds, else 0.
 */
digit_t ibz_ct_lt_mask(const ibz_t *a, const ibz_t *b); // a < b
digit_t ibz_ct_nonzero_mask(const ibz_t *a);            // a != 0

/** @brief W ~= 2^(P+s)/D, s = bitsize(D) (SECRET, returned via *s_out), D > 0.
 *
 * Satisfies (num << P)/D == ibz_ct_highmul_s(num, W, -s, ..., ...). Accuracy is set by
 * nrounds: the 64-bit seed is good to ~61 bits and each Newton-Raphson round roughly doubles
 * that. The lattice reduction passes FP_NR_ROUNDS, which lll_config.h derives (and
 * static-asserts) against FP_P_BITS; at that setting |W - 2^(P+s)/D| <~ 8.
 */
void ibz_ct_fp_recip(ibz_t *W, int32_t *s_out, const ibz_t *D, int P, int nrounds);

// --- Lehmer-LG window/shift primitives -----------------------------------------------------

/** @brief q = round(x / 2^P), ties toward +infinity, for P a nonnegative multiple of NUM_BITS_LIMB.
 */
void ibz_ct_round_shift_limb(ibz_t *q, const ibz_t *x, int P, int out_bitlen);

/** @brief res = trunc_{out_bitlen}(a << shift), the left-only sibling of ibz_ct_shift.
 *
 * shift >= 0 and SECRET; max_shift and out_bitlen are PUBLIC.
 * Requires: shift <= max_shift.
 */
void ibz_ct_shl(ibz_t *res, const ibz_t *a, int32_t shift, int32_t max_shift, int out_bitlen);

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
 * @brief Extended gcd algorithm. a and b are required to have at least 64 bits than the global bound.
 *
 * @param gcd Output: Set to the gcd of a and b
 * @param u Output: The coefficient of a
 * @param v Output: The coefficient of b
 * @param a
 * @param b
 */
void ibz_xgcd(ibz_t *gcd, ibz_t *u, ibz_t *v, const ibz_t *a, const ibz_t *b);

/**
 * @brief Returns x such that 0 <= x < m1*m2 with x=a1 mod m1 and x=a2 mod m2.
 * Operands are required to have at least 64 bits than the global bound, and
 * ai must already be reduced mod mi.
 */
void ibz_crt(ibz_t *x, const ibz_t *a1, const ibz_t *a2, const ibz_t *m1, const ibz_t *m2);

/**
 * @brief Probabilistic primality test
 *
 * n is required to be at least 66 bits below the global bound.
 *
 * @param n number to test for primality
 * @param reps number of repetitions for the test
 * @returns -1 if n is probably prime, 0 if n is composite
 */
int ibz_probab_prime(const ibz_t *n, int reps);

/** @}
 */

/** @defgroup ibz_m Modular operations
 * @{
 */

/** @brief r = a mod b
 *
 * Assumes valid inputs
 * The sign of the divisor is ignored, the result is always non-negative
 */
void ibz_mod(ibz_t *r, const ibz_t *a, const ibz_t *b);

/** @brief r = a mod 2^exp
 *
 * Assumes valid inputs
 */
void ibz_mod2exp(ibz_t *r, const ibz_t *a, const uint32_t exp);

/** @brief r = n mod d, returns remainder as unsigned long
 */
unsigned long int ibz_mod_ui(const ibz_t *n, unsigned long int d);

/** @brief Test if a = 0 mod b
 * @returns -1 if a is divisible by b, 0 otherwise
 */
int ibz_divides(const ibz_t *a, const ibz_t *b);

/** @brief pow=(x^e) mod p for p an odd prime
 *
 * Assumes that 0 <= x,e < p
 */
void ibz_pow_mod(ibz_t *pow, const ibz_t *x, const ibz_t *e, const ibz_t *p);

/** @brief Computes the square root of -1 modulo an odd prime p which is 1 mod 4
 *
 * p is required to be at least 65 bits below the global bound.
 */
void ibz_sqrt_m1_mod(ibz_t *res, const ibz_t *p);

/** @brief Attempts to find a square-root of -1 modulo any nunmber.
 *
 * @param res Output: the square-root of -1 mod n.
 * @param n: modulus
 * @returns -1 if a confirmed square-root of -1 was found, 0 otherwise.
 */
int ibz_sqrt_m1_mod_verified(ibz_t *res, const ibz_t *n);

/**
 * @brief Computes the Legendre symbol (a/p) for an odd prime p assuming 0 <= a < p
 * @returns -1 if a is a non-square, 0 if a = 0 mod p, 1 if a is a non-zero square
 */
int ibz_legendre(const ibz_t *a, const ibz_t *p);

/**
 * @brief Modular inverse. Operands are required to be 64 bits below the global bound.
 *
 * @param inv Output: Set to the integer in [0,mod[ such that a*inv = 1 mod (mod) if it exists
 * @param a
 * @param mod
 * @returns -1 if inverse exists and was computed, 0 otherwise
 */
int ibz_invmod(ibz_t *inv, const ibz_t *a, const ibz_t *mod);

/**
 * @brief In-place inverse of the 2x2 matrix [[r1, r2], [s1, s2]] modulo 2^e.
 * e must be 64 bits below the global bound.
 *
 * @param r1
 * @param r2
 * @param s1
 * @param s2
 * @param e
 * @returns -1 if inverse exists and was computed, 0 otherwise
 */
int ibz_invmat(ibz_t *r1, ibz_t *r2, ibz_t *s1, ibz_t *s2, int e);

/**
 * @brief Modular square-root modulo a prime
 *
 * p is required to be at least 65 bits below the global bound.
 *
 * @param sqrt Output: Set to the integer in [0,p-1] such that sqrt^2 = a (mod p) if it exists
 * @param p a prime equal to 1 mod 4
 * @param a number in [0,p-1]
 * @returns -1 on success, 0 if a is a non-square
 */
int ibz_sqrt_mod_p(ibz_t *sqrt, const ibz_t *a, const ibz_t *p);

/** @}
 */

// end of ibz_all
/** @}
 */
#endif
