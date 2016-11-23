/********************************************************************************************
* SIDH: an efficient supersingular isogeny-based cryptography library for Diffie-Hellman key
*       exchange providing 128 bits of quantum security and 192 bits of classical security.
*
*    Copyright (c) Microsoft Corporation. All rights reserved.
*
*
* Abstract: internal header file
*
*********************************************************************************************/

#ifndef __SIDH_INTERNAL_H__
#define __SIDH_INTERNAL_H__


// For C++
#ifdef __cplusplus
extern "C" {
#endif


#include "SIDH.h"


// Basic constants

#define SIDH_ALICE                 0
#define SIDH_BOB                   1
#define SIDH_MAX_INT_POINTS_ALICE  8
// Fixed parameters for isogeny tree computation
#define SIDH_MAX_INT_POINTS_BOB    10
#define SIDH_MAX_Alice             185
#define SIDH_MAX_Bob               239


// SIDH's basic element definitions and point representations

typedef digit_t felm_t[NWORDS_FIELD];                             // Datatype for representing 751-bit field elements (768-bit max.)
typedef digit_t dfelm_t[2 * NWORDS_FIELD];                        // Datatype for representing double-precision 2x751-bit field elements (2x768-bit max.)
typedef felm_t  f2elm_t[2];                                       // Datatype for representing quadratic extension field elements GF(p751^2)
typedef f2elm_t publickey_t[3];                                   // Datatype for representing public keys equivalent to three GF(p751^2) elements

typedef struct {
	f2elm_t x;
	f2elm_t y;
} point_affine;            // Point representation in affine coordinates on Montgomery curve.
typedef point_affine point_t[1];

typedef struct {
	f2elm_t X;
	f2elm_t Z;
} point_proj;              // Point representation in projective XZ Montgomery coordinates.
typedef point_proj point_proj_t[1];

typedef struct {
	felm_t x;
	felm_t y;
} point_basefield_affine;    // Point representation in affine coordinates on Montgomery curve over the base field.
typedef point_basefield_affine point_basefield_t[1];

typedef struct {
	felm_t X;
	felm_t Z;
} point_basefield_proj;      // Point representation in projective XZ Montgomery coordinates over the base field.
typedef point_basefield_proj point_basefield_proj_t[1];


// Macro definitions

#define NBITS_TO_NBYTES(nbits)      (((nbits)+7)/8)                                          // Conversion macro from number of bits to number of bytes
#define NBITS_TO_NWORDS(nbits)      (((nbits)+(sizeof(digit_t)*8)-1)/(sizeof(digit_t)*8))    // Conversion macro from number of bits to number of computer words
#define NBYTES_TO_NWORDS(nbytes)    (((nbytes)+sizeof(digit_t)-1)/sizeof(digit_t))           // Conversion macro from number of bytes to number of computer words

// Macro to avoid compiler warnings when detecting unreferenced parameters
#define UNREFERENCED_PARAMETER(PAR) (PAR)


/********************** Constant-time unsigned comparisons ***********************/

// The following functions return 1 (TRUE) if condition is true, 0 (FALSE) otherwise

static __inline unsigned int is_digit_nonzero_ct(digit_t x) {
	// Is x != 0?
	return (unsigned int)((x | (0 - x)) >> (RADIX - 1));
}

static __inline unsigned int is_digit_zero_ct(digit_t x) {
	// Is x = 0?
	return (unsigned int)(1 ^ is_digit_nonzero_ct(x));
}

static __inline unsigned int is_digit_lessthan_ct(digit_t x, digit_t y) {
	// Is x < y?
	return (unsigned int)((x ^ ((x ^ y) | ((x - y) ^ y))) >> (RADIX - 1));
}


/********************** Macros for platform-dependent operations **********************/

#if !defined(SIDH_ASM)

// Digit multiplication
#define MUL(multiplier, multiplicand, hi, lo)                                                     \
    digit_x_digit((multiplier), (multiplicand), &(lo));

// Digit addition with carry
#define ADDC(carryIn, addend1, addend2, carryOut, sumOut)                                         \
    { digit_t tempReg = (addend1) + (digit_t)(carryIn);                                           \
    (sumOut) = (addend2) + tempReg;                                                               \
    (carryOut) = (is_digit_lessthan_ct(tempReg, (digit_t)(carryIn)) | is_digit_lessthan_ct((sumOut), tempReg)); }

// Digit subtraction with borrow
#define SUBC(borrowIn, minuend, subtrahend, borrowOut, differenceOut)                             \
    { digit_t tempReg = (minuend) - (subtrahend);                                                 \
    unsigned int borrowReg = (is_digit_lessthan_ct((minuend), (subtrahend)) | ((borrowIn) & is_digit_zero_ct(tempReg)));  \
    (differenceOut) = tempReg - (digit_t)(borrowIn);                                              \
    (borrowOut) = borrowReg; }

// Shift right with flexible datatype
#define SHIFTR(highIn, lowIn, shift, shiftOut, DigitSize)                                         \
    (shiftOut) = ((lowIn) >> (shift)) ^ ((highIn) << (DigitSize - (shift)));

// Shift left with flexible datatype
#define SHIFTL(highIn, lowIn, shift, shiftOut, DigitSize)                                         \
    (shiftOut) = ((highIn) << (shift)) ^ ((lowIn) >> (DigitSize - (shift)));

// 64x64-bit multiplication
#define MUL128(multiplier, multiplicand, product)                                                 \
    mp_mul((digit_t*)&(multiplier), (digit_t*)&(multiplicand), (digit_t*)&(product), NWORDS_FIELD/2);

// 128-bit addition, inputs < 2^127
#define ADD128(addend1, addend2, addition)                                                        \
    mp_add((digit_t*)(addend1), (digit_t*)(addend2), (digit_t*)(addition), NWORDS_FIELD);

// 128-bit addition with output carry
#define ADC128(addend1, addend2, carry, addition)                                                 \
    (carry) = mp_add((digit_t*)(addend1), (digit_t*)(addend2), (digit_t*)(addition), NWORDS_FIELD);

#elif (TARGET == TARGET_AMD64 && OS_TARGET == OS_WIN)

// Digit multiplication
#define MUL(multiplier, multiplicand, hi, lo)                                                     \
    (lo) = _umul128((multiplier), (multiplicand), (hi));

// Digit addition with carry
#define ADDC(carryIn, addend1, addend2, carryOut, sumOut)                                         \
    (carryOut) = _addcarry_u64((carryIn), (addend1), (addend2), &(sumOut));

// Digit subtraction with borrow
#define SUBC(borrowIn, minuend, subtrahend, borrowOut, differenceOut)                             \
    (borrowOut) = _subborrow_u64((borrowIn), (minuend), (subtrahend), &(differenceOut));

// Digit shift right
#define SHIFTR(highIn, lowIn, shift, shiftOut, DigitSize)                                         \
    (shiftOut) = __shiftright128((lowIn), (highIn), (shift));

// Digit shift left
#define SHIFTL(highIn, lowIn, shift, shiftOut, DigitSize)                                         \
    (shiftOut) = __shiftleft128((lowIn), (highIn), (shift));

// 64x64-bit multiplication
#define MUL128(multiplier, multiplicand, product)                                                 \
    (product)[0] = _umul128((multiplier), (multiplicand), &(product)[1]);

// 128-bit addition, inputs < 2^127
#define ADD128(addend1, addend2, addition)                                                        \
    { unsigned char carry = _addcarry_u64(0, (addend1)[0], (addend2)[0], &(addition)[0]);         \
    _addcarry_u64(carry, (addend1)[1], (addend2)[1], &(addition)[1]); }

// 128-bit addition with output carry
#define ADC128(addend1, addend2, carry, addition)                                                 \
    (carry) = _addcarry_u64(0, (addend1)[0], (addend2)[0], &(addition)[0]);                       \
    (carry) = _addcarry_u64((carry), (addend1)[1], (addend2)[1], &(addition)[1]);

// 128-bit subtraction, subtrahend < 2^127
#define SUB128(minuend, subtrahend, difference)                                                   \
    { unsigned char borrow = _subborrow_u64(0, (minuend)[0], (subtrahend)[0], &(difference)[0]);  \
    _subborrow_u64(borrow, (minuend)[1], (subtrahend)[1], &(difference)[1]); }

// 128-bit right shift, max. shift value is 64
#define SHIFTR128(Input, shift, shiftOut)                                                         \
    (shiftOut)[0]  = __shiftright128((Input)[0], (Input)[1], (shift));                            \
    (shiftOut)[1] = (Input)[1] >> (shift);

// 128-bit left shift, max. shift value is 64
#define SHIFTL128(Input, shift, shiftOut)                                                         \
    (shiftOut)[1]  = __shiftleft128((Input)[0], (Input)[1], (shift));                             \
    (shiftOut)[0] = (Input)[0] << (shift);

#define MULADD128(multiplier, multiplicand, addend, carry, result);    \
    { uint128_t product;                                               \
      MUL128(multiplier, multiplicand, product);                       \
      ADC128(addend, product, carry, result); }

#elif (TARGET == TARGET_AMD64 && OS_TARGET == OS_LINUX)

// Digit multiplication
#define MUL(multiplier, multiplicand, hi, lo)                                                     \
    { uint128_t tempReg = (uint128_t)(multiplier) * (uint128_t)(multiplicand);                    \
    *(hi) = (digit_t)(tempReg >> RADIX);                                                          \
    (lo) = (digit_t)tempReg; }

// Digit addition with carry
#define ADDC(carryIn, addend1, addend2, carryOut, sumOut)                                         \
    { uint128_t tempReg = (uint128_t)(addend1) + (uint128_t)(addend2) + (uint128_t)(carryIn);     \
    (carryOut) = (digit_t)(tempReg >> RADIX);                                                     \
    (sumOut) = (digit_t)tempReg; }

// Digit subtraction with borrow
#define SUBC(borrowIn, minuend, subtrahend, borrowOut, differenceOut)                             \
    { uint128_t tempReg = (uint128_t)(minuend) - (uint128_t)(subtrahend) - (uint128_t)(borrowIn); \
    (borrowOut) = (digit_t)(tempReg >> (sizeof(uint128_t)*8 - 1));                                \
    (differenceOut) = (digit_t)tempReg; }

// Digit shift right
#define SHIFTR(highIn, lowIn, shift, shiftOut, DigitSize)                                         \
    (shiftOut) = ((lowIn) >> (shift)) ^ ((highIn) << (RADIX - (shift)));

// Digit shift left
#define SHIFTL(highIn, lowIn, shift, shiftOut, DigitSize)                                         \
    (shiftOut) = ((highIn) << (shift)) ^ ((lowIn) >> (RADIX - (shift)));

#endif


// Multiprecision multiplication selection
#if !defined(SIDH_ASM) && (TARGET == TARGET_AMD64)
#define mp_mul_comba         mp_mul
#else
#define mp_mul_schoolbook    mp_mul
#endif



/**************** Function prototypes ****************/
/************* Multiprecision functions **************/

// Copy wordsize digits, c = a, where lng(a) = nwords
void copy_words(digit_t *a, digit_t *c, unsigned int nwords);

// Multiprecision addition, c = a+b, where lng(a) = lng(b) = nwords. Returns the carry bit
extern unsigned int mp_add(digit_t *a, digit_t *b, digit_t *c, unsigned int nwords);

// Multiprecision subtraction, c = a-b, where lng(a) = lng(b) = nwords. Returns the borrow bit
extern unsigned int mp_sub(digit_t *a, digit_t *b, digit_t *c, unsigned int nwords);

// Multiprecision right shift by one
void mp_shiftr1(digit_t *x, unsigned int nwords);

// Multiprecision left right shift by one
void mp_shiftl1(digit_t *x, unsigned int nwords);

// Digit multiplication, digit * digit -> 2-digit result
void digit_x_digit(digit_t a, digit_t b, digit_t *c);

// Multiprecision schoolbook multiply, c = a*b, where lng(a) = lng(b) = nwords.
void mp_mul_schoolbook(digit_t *a, digit_t *b, digit_t *c, unsigned int nwords);

// Multiprecision comba multiply, c = a*b, where lng(a) = lng(b) = nwords.
void mp_mul_comba(digit_t *a, digit_t *b, digit_t *c, unsigned int nwords);

/************ Field arithmetic functions *************/

// Copy of a field element, c = a
void fpcopy751(felm_t a, felm_t c);

// Zeroing a field element, a = 0
void fpzero751(felm_t a);

// Modular addition, c = a+b mod p751
extern void fpadd751(digit_t *a, digit_t *b, digit_t *c);
extern void fpadd751_asm(digit_t *a, digit_t *b, digit_t *c);

// Modular subtraction, c = a-b mod p751
extern void fpsub751(digit_t *a, digit_t *b, digit_t *c);
extern void fpsub751_asm(digit_t *a, digit_t *b, digit_t *c);

// Modular negation, a = -a mod p751
extern void fpneg751(digit_t *a);

// Modular division by two, c = a/2 mod p751.
void fpdiv2_751(digit_t *a, digit_t *c);

// Modular correction to reduce field element a in [0, 2*p751-1] to [0, p751-1].
void fpcorrection751(digit_t *a);

// 751-bit Montgomery reduction, c = a mod p
void rdc_mont(digit_t *a, digit_t *c);

// Field multiplication using Montgomery arithmetic, c = a*b*R^-1 mod p751, where R=2^768
void fpmul751_mont(felm_t a, felm_t b, felm_t c);
void mul751_asm(felm_t a, felm_t b, dfelm_t c);
void rdc751_asm(dfelm_t ma, dfelm_t mc);

// Field squaring using Montgomery arithmetic, c = a*b*R^-1 mod p751, where R=2^768
void fpsqr751_mont(felm_t ma, felm_t mc);

// Conversion to Montgomery representation
void to_mont(felm_t a, felm_t mc);

// Conversion from Montgomery representation to standard representation
void from_mont(felm_t ma, felm_t c);

// Field inversion, a = a^-1 in GF(p751)
void fpinv751_mont(felm_t a);

/************ GF(p^2) arithmetic functions *************/

// Copy of a GF(p751^2) element, c = a
void fp2copy751(f2elm_t a, f2elm_t c);

// Zeroing a GF(p751^2) element, a = 0
void fp2zero751(f2elm_t a);

// GF(p751^2) negation, a = -a in GF(p751^2)
void fp2neg751(f2elm_t a);

// GF(p751^2) addition, c = a+b in GF(p751^2)
void fp2add751(f2elm_t a, f2elm_t b, f2elm_t c);

// GF(p751^2) subtraction, c = a-b in GF(p751^2)
void fp2sub751(f2elm_t a, f2elm_t b, f2elm_t c);

// GF(p751^2) division by two, c = a/2  in GF(p751^2)
void fp2div2_751(f2elm_t a, f2elm_t c);

// Modular correction, a = a in GF(p751^2)
void fp2correction751(f2elm_t a);

// GF(p751^2) squaring using Montgomery arithmetic, c = a^2 in GF(p751^2)
void fp2sqr751_mont(f2elm_t a, f2elm_t c);

// GF(p751^2) multiplication using Montgomery arithmetic, c = a*b in GF(p751^2)
void fp2mul751_mont(f2elm_t a, f2elm_t b, f2elm_t c);

// Conversion of a GF(p751^2) element to Montgomery representation
void to_fp2mont(f2elm_t a, f2elm_t mc);

// Conversion of a GF(p751^2) element from Montgomery representation to standard representation
void from_fp2mont(f2elm_t ma, f2elm_t c);

// GF(p751^2) inversion using Montgomery arithmetic, a = (a0-i*a1)/(a0^2+a1^2)
void fp2inv751_mont(f2elm_t a);

// Select either x or y depending on value of option
void select_f2elm(f2elm_t x, f2elm_t y, f2elm_t z, digit_t option);

/************ Elliptic curve and isogeny functions *************/

// Check if curve isogeny structure is NULL
bool is_CurveIsogenyStruct_null(PCurveIsogenyStruct pCurveIsogeny);

// Swap points over the base field
void swap_points_basefield(point_basefield_proj_t P, point_basefield_proj_t Q, digit_t option);

// Swap points
void swap_points(point_proj_t P, point_proj_t Q, digit_t option);

// Computes the j-invariant of a Montgomery curve with projective constant.
void j_inv(f2elm_t A, f2elm_t C, f2elm_t jinv);

// Simultaneous doubling and differential addition.
void xDBLADD(point_proj_t P, point_proj_t Q, f2elm_t xPQ, f2elm_t A24);

// Doubling of a Montgomery point in projective coordinates (X:Z).
void xDBL(point_proj_t P, point_proj_t Q, f2elm_t A24, f2elm_t C24);

// Computes [2^e](X:Z) on Montgomery curve with projective constant via e repeated doublings.
void xDBLe(point_proj_t P, point_proj_t Q, f2elm_t A, f2elm_t C, int e);

// Computes [2^e](X:Z) on Montgomery curve with projective constant via e repeated doublings and collects a few intermediate multiples.
void xDBLe_collect(point_proj_t P, point_proj_t Q, f2elm_t A, f2elm_t C, unsigned int left_bound, const unsigned int right_bound, const unsigned int *col, point_proj_t *pts, unsigned int *pts_index, unsigned int *npts);

// Differential addition.
void xADD(point_proj_t P, point_proj_t Q, f2elm_t xPQ);

// Doubling of a Montgomery point in projective coordinates (X:Z) over the base field.
void xDBL_basefield(point_basefield_proj_t P, point_basefield_proj_t Q);

// Simultaneous doubling and differential addition over the base field.
void xDBLADD_basefield(point_basefield_proj_t P, point_basefield_proj_t Q, felm_t xPQ, felm_t A24);

// The Montgomery ladder
void ladder(felm_t x, digit_t *m, point_basefield_proj_t P, point_basefield_proj_t Q, felm_t A24, unsigned int order_bits, unsigned int order_fullbits, PCurveIsogenyStruct CurveIsogeny);

// Computes key generation entirely in the base field
SIDH_CRYPTO_STATUS secret_pt(point_basefield_t P, digit_t *m, unsigned int AliceOrBob, point_proj_t R, PCurveIsogenyStruct CurveIsogeny);

// Computes P+[m]Q via x-only arithmetic.
SIDH_CRYPTO_STATUS ladder_3_pt(f2elm_t xP, f2elm_t xQ, f2elm_t xPQ, digit_t *m, unsigned int AliceOrBob, point_proj_t W, f2elm_t A, PCurveIsogenyStruct CurveIsogeny);

// Computes the corresponding 4-isogeny of a projective Montgomery point (X4:Z4) of order 4.
void get_4_isog(point_proj_t P, f2elm_t A, f2elm_t C, f2elm_t *coeff);

// Evaluates the isogeny at the point (X:Z) in the domain of the isogeny
void eval_4_isog(point_proj_t P, f2elm_t *coeff);

// Computes first 4-isogeny computed by Alice.
void first_4_isog(point_proj_t P, f2elm_t A, f2elm_t Aout, f2elm_t Cout, PCurveIsogenyStruct CurveIsogeny);

// Tripling of a Montgomery point in projective coordinates (X:Z).
void xTPL(point_proj_t P, point_proj_t Q, f2elm_t A24, f2elm_t C24);

// Computes [3^e](X:Z) on Montgomery curve with projective constant via e repeated triplings.
void xTPLe(point_proj_t P, point_proj_t Q, f2elm_t A, f2elm_t C, int e);

// Computes [3^e](X:Z) on Montgomery curve with projective constant via e repeated triplings and collects a few intermediate multiples.
void xTPLe_collect(point_proj_t P, point_proj_t Q, f2elm_t A, f2elm_t C, unsigned int left_bound, const unsigned int right_bound, const unsigned int *col, point_proj_t *pts, unsigned int *pts_index, unsigned int *npts);

// Computes the corresponding 3-isogeny of a projective Montgomery point (X3:Z3) of order 3.
void get_3_isog(point_proj_t P, f2elm_t A, f2elm_t C);

// Computes the 3-isogeny R=phi(X:Z), given projective point (X3:Z3) of order 3 on a Montgomery curve and a point P = (X:Z).
void eval_3_isog(point_proj_t P, point_proj_t Q);

// 3-way simultaneous inversion
void inv_3_way(f2elm_t z1, f2elm_t z2, f2elm_t z3);

// Computing the point D = (x(Q-P),z(Q-P))
void distort_and_diff(felm_t xP, point_proj_t d, PCurveIsogenyStruct CurveIsogeny);

// Given the x-coordinates of P, Q, and R, returns the value A corresponding to the Montgomery curve E_A: y^2=x^3+A*x^2+x such that R=Q-P on E_A.
void get_A(f2elm_t xP, f2elm_t xQ, f2elm_t xR, f2elm_t A, PCurveIsogenyStruct CurveIsogeny);

/************ Public key validation functions *************/

// Validation of Alice's public key (ran by Bob)
// CurveIsogeny must be set up in advance using SIDH_curve_initialize().
SIDH_CRYPTO_STATUS Validate_PKA(f2elm_t A, publickey_t PKA, bool *valid, PCurveIsogenyStruct CurveIsogeny, OQS_RAND *rand);

// Validation of Bob's public key (ran by Alice)
// CurveIsogeny must be set up in advance using SIDH_curve_initialize().
SIDH_CRYPTO_STATUS Validate_PKB(f2elm_t A, publickey_t PKB, bool *valid, PCurveIsogenyStruct CurveIsogeny, OQS_RAND *rand);


#ifdef __cplusplus
}
#endif


#endif
