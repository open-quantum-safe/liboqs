#ifndef PQCLEAN_FALCON512_CLEAN_FPR_H
#define PQCLEAN_FALCON512_CLEAN_FPR_H

/*
 * Floating-point operations.
 *
 * ==========================(LICENSE BEGIN)============================
 *
 * Copyright (c) 2017-2019  Falcon Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * ===========================(LICENSE END)=============================
 *
 * @author   Thomas Pornin <thomas.pornin@nccgroup.com>
 */


/* ====================================================================== */
/*
 * Custom floating-point implementation with integer arithmetics. We
 * use IEEE-754 "binary64" format, with some simplifications:
 *
 *   - Top bit is s = 1 for negative, 0 for positive.
 *
 *   - Exponent e uses the next 11 bits (bits 52 to 62, inclusive).
 *
 *   - Mantissa m uses the 52 low bits.
 *
 * Encoded value is, in general: (-1)^s * 2^(e-1023) * (1 + m*2^(-52))
 * i.e. the mantissa really is a 53-bit number (less than 2.0, but not
 * less than 1.0), but the top bit (equal to 1 by definition) is omitted
 * in the encoding.
 *
 * In IEEE-754, there are some special values:
 *
 *   - If e = 2047, then the value is either an infinite (m = 0) or
 *     a NaN (m != 0).
 *
 *   - If e = 0, then the value is either a zero (m = 0) or a subnormal,
 *     aka "denormalized number" (m != 0).
 *
 * Of these, we only need the zeros. The caller is responsible for not
 * providing operands that would lead to infinites, NaNs or subnormals.
 * If inputs are such that values go out of range, then indeterminate
 * values are returned (it would still be deterministic, but no specific
 * value may be relied upon).
 *
 * At the C level, the three parts are stored in a 64-bit unsigned
 * word.
 *
 * One may note that a property of the IEEE-754 format is that order
 * is preserved for positive values: if two positive floating-point
 * values x and y are such that x < y, then their respective encodings
 * as _signed_ 64-bit integers i64(x) and i64(y) will be such that
 * i64(x) < i64(y). For negative values, order is reversed: if x < 0,
 * y < 0, and x < y, then ia64(x) > ia64(y).
 *
 * IMPORTANT ASSUMPTIONS:
 * ======================
 *
 * For proper computations, and constant-time behaviour, we assume the
 * following:
 *
 *   - 32x32->64 multiplication (unsigned) has an execution time that
 *     is independent of its operands. This is true of most modern
 *     x86 and ARM cores. Notable exceptions are the ARM Cortex M0, M0+
 *     and M3 (in the M0 and M0+, this is done in software, so it depends
 *     on that routine), and the PowerPC cores from the G3/G4 lines.
 *     For more info, see: https://www.bearssl.org/ctmul.html
 *
 *   - Left-shifts and right-shifts of 32-bit values have an execution
 *     time which does not depend on the shifted value nor on the
 *     shift count. An historical exception is the Pentium IV, but most
 *     modern CPU have barrel shifters. Some small microcontrollers
 *     might have varying-time shifts (not the ARM Cortex M*, though).
 *
 *   - Right-shift of a signed negative value performs a sign extension.
 *     As per the C standard, this operation returns an
 *     implementation-defined result (this is NOT an "undefined
 *     behaviour"). On most/all systems, an arithmetic shift is
 *     performed, because this is what makes most sense.
 */

/*
 * Normally we should declare the 'fpr' type to be a struct or union
 * around the internal 64-bit value; however, we want to use the
 * direct 64-bit integer type to enable a lighter call convention on
 * ARM platforms. This means that direct (invalid) use of operators
 * such as '*' or '+' will not be caught by the compiler. We rely on
 * the "normal" (non-emulated) code to detect such instances.
 */
typedef uint64_t fpr;

/*
 * For computations, we split values into an integral mantissa in the
 * 2^54..2^55 range, and an (adjusted) exponent. The lowest bit is
 * "sticky" (it is set to 1 if any of the bits below it is 1); when
 * re-encoding, the low two bits are dropped, but may induce an
 * increment in the value for proper rounding.
 */

/*
 * Right-shift a 64-bit unsigned value by a possibly secret shift count.
 * We assumed that the underlying architecture had a barrel shifter for
 * 32-bit shifts, but for 64-bit shifts on a 32-bit system, this will
 * typically invoke a software routine that is not necessarily
 * constant-time; hence the function below.
 *
 * Shift count n MUST be in the 0..63 range.
 */
#define fpr_ursh   PQCLEAN_FALCON512_CLEAN_fpr_ursh
uint64_t fpr_ursh(uint64_t x, int n);

/*
 * Right-shift a 64-bit signed value by a possibly secret shift count
 * (see fpr_ursh() for the rationale).
 *
 * Shift count n MUST be in the 0..63 range.
 */
#define fpr_irsh   PQCLEAN_FALCON512_CLEAN_fpr_irsh
int64_t fpr_irsh(int64_t x, int n);

/*
 * Left-shift a 64-bit unsigned value by a possibly secret shift count
 * (see fpr_ursh() for the rationale).
 *
 * Shift count n MUST be in the 0..63 range.
 */
#define fpr_ulsh   PQCLEAN_FALCON512_CLEAN_fpr_ulsh
uint64_t fpr_ulsh(uint64_t x, int n);

/*
 * Expectations:
 *   s = 0 or 1
 *   exponent e is "arbitrary" and unbiased
 *   2^54 <= m < 2^55
 * Numerical value is (-1)^2 * m * 2^e
 *
 * Exponents which are too low lead to value zero. If the exponent is
 * too large, the returned value is indeterminate.
 *
 * If m = 0, then a zero is returned (using the provided sign).
 * If e < -1076, then a zero is returned (regardless of the value of m).
 * If e >= -1076 and e != 0, m must be within the expected range
 * (2^54 to 2^55-1).
 */
#define FPR   PQCLEAN_FALCON512_CLEAN_FPR
fpr FPR(int s, int e, uint64_t m);


#define fpr_scaled   PQCLEAN_FALCON512_CLEAN_fpr_scaled
fpr fpr_scaled(int64_t i, int sc);

#define fpr_of   PQCLEAN_FALCON512_CLEAN_fpr_of
fpr fpr_of(int64_t i);

static const fpr fpr_q = 4667981563525332992;
static const fpr fpr_inverse_of_q = 4545632735260551042;
static const fpr fpr_inv_2sqrsigma0 = 4594603506513722306;
static const fpr fpr_inv_sigma = 4573359825155195350;
static const fpr fpr_sigma_min_9 = 4608495221497168882;
static const fpr fpr_sigma_min_10 = 4608586345619182117;
static const fpr fpr_log2 = 4604418534313441775;
static const fpr fpr_inv_log2 = 4609176140021203710;
static const fpr fpr_bnorm_max = 4670353323383631276;
static const fpr fpr_zero = 0;
static const fpr fpr_one = 4607182418800017408;
static const fpr fpr_two = 4611686018427387904;
static const fpr fpr_onehalf = 4602678819172646912;
static const fpr fpr_invsqrt2 = 4604544271217802189;
static const fpr fpr_invsqrt8 = 4600040671590431693;
static const fpr fpr_ptwo31 = 4746794007248502784;
static const fpr fpr_ptwo31m1 = 4746794007244308480;
static const fpr fpr_mtwo31m1 = 13970166044099084288U;
static const fpr fpr_ptwo63m1 = 4890909195324358656;
static const fpr fpr_mtwo63m1 = 14114281232179134464U;
static const fpr fpr_ptwo63 = 4890909195324358656;

#define fpr_rint   PQCLEAN_FALCON512_CLEAN_fpr_rint
int64_t fpr_rint(fpr x);

#define fpr_floor   PQCLEAN_FALCON512_CLEAN_fpr_floor
int64_t fpr_floor(fpr x);

#define fpr_trunc   PQCLEAN_FALCON512_CLEAN_fpr_trunc
int64_t fpr_trunc(fpr x);

#define fpr_add   PQCLEAN_FALCON512_CLEAN_fpr_add
fpr fpr_add(fpr x, fpr y);

#define fpr_sub   PQCLEAN_FALCON512_CLEAN_fpr_sub
fpr fpr_sub(fpr x, fpr y);

#define fpr_neg   PQCLEAN_FALCON512_CLEAN_fpr_neg
fpr fpr_neg(fpr x);

#define fpr_half   PQCLEAN_FALCON512_CLEAN_fpr_half
fpr fpr_half(fpr x);

#define fpr_double   PQCLEAN_FALCON512_CLEAN_fpr_double
fpr fpr_double(fpr x);

#define fpr_mul   PQCLEAN_FALCON512_CLEAN_fpr_mul
fpr fpr_mul(fpr x, fpr y);

#define fpr_sqr   PQCLEAN_FALCON512_CLEAN_fpr_sqr
fpr fpr_sqr(fpr x);

#define fpr_div   PQCLEAN_FALCON512_CLEAN_fpr_div
fpr fpr_div(fpr x, fpr y);

#define fpr_inv   PQCLEAN_FALCON512_CLEAN_fpr_inv
fpr fpr_inv(fpr x);

#define fpr_sqrt   PQCLEAN_FALCON512_CLEAN_fpr_sqrt
fpr fpr_sqrt(fpr x);

#define fpr_lt   PQCLEAN_FALCON512_CLEAN_fpr_lt
int fpr_lt(fpr x, fpr y);

/*
 * Compute exp(x) for x such that |x| <= ln 2. We want a precision of 50
 * bits or so.
 */
#define fpr_expm_p63   PQCLEAN_FALCON512_CLEAN_fpr_expm_p63
uint64_t fpr_expm_p63(fpr x, fpr ccs);

#define fpr_gm_tab   PQCLEAN_FALCON512_CLEAN_fpr_gm_tab
extern const fpr fpr_gm_tab[];

#define fpr_p2_tab   PQCLEAN_FALCON512_CLEAN_fpr_p2_tab
extern const fpr fpr_p2_tab[];

/* ====================================================================== */
#endif
