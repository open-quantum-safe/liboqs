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

#include <math.h>

/*
 * We wrap the native 'double' type into a structure so that the C compiler
 * complains if we inadvertently use raw arithmetic operators on the 'fpr'
 * type instead of using the inline functions below. This should have no
 * extra runtime cost, since all the functions below are 'inline'.
 */
typedef struct {
    double v;
} fpr;

static inline fpr
FPR(double v) {
    fpr x;

    x.v = v;
    return x;
}

static inline fpr
fpr_of(int64_t i) {
    return FPR((double)i);
}

static const fpr fpr_q = { 12289.0 };
static const fpr fpr_inverse_of_q = { 1.0 / 12289.0 };
static const fpr fpr_inv_2sqrsigma0 = { .150865048875372721532312163019 };
static const fpr fpr_inv_sigma[] = {
    { 0.0 }, /* unused */
    { 0.0069054793295940891952143765991630516 },
    { 0.0068102267767177975961393730687908629 },
    { 0.0067188101910722710707826117910434131 },
    { 0.0065883354370073665545865037227681924 },
    { 0.0064651781207602900738053897763485516 },
    { 0.0063486788828078995327741182928037856 },
    { 0.0062382586529084374473367528433697537 },
    { 0.0061334065020930261548984001431770281 },
    { 0.0060336696681577241031668062510953022 },
    { 0.0059386453095331159950250124336477482 }
};
static const fpr fpr_sigma_min[] = {
    { 0.0 }, /* unused */
    { 1.1165085072329102588881898380334015 },
    { 1.1321247692325272405718031785357108 },
    { 1.1475285353733668684571123112513188 },
    { 1.1702540788534828939713084716509250 },
    { 1.1925466358390344011122170489094133 },
    { 1.2144300507766139921088487776957699 },
    { 1.2359260567719808790104525941706723 },
    { 1.2570545284063214162779743112075080 },
    { 1.2778336969128335860256340575729042 },
    { 1.2982803343442918539708792538826807 }
};
static const fpr fpr_log2 = { 0.69314718055994530941723212146 };
static const fpr fpr_inv_log2 = { 1.4426950408889634073599246810 };
static const fpr fpr_bnorm_max = { 16822.4121 };
static const fpr fpr_zero = { 0.0 };
static const fpr fpr_one = { 1.0 };
static const fpr fpr_two = { 2.0 };
static const fpr fpr_onehalf = { 0.5 };
static const fpr fpr_invsqrt2 = { 0.707106781186547524400844362105 };
static const fpr fpr_invsqrt8 = { 0.353553390593273762200422181052 };
static const fpr fpr_ptwo31 = { 2147483648.0 };
static const fpr fpr_ptwo31m1 = { 2147483647.0 };
static const fpr fpr_mtwo31m1 = { -2147483647.0 };
static const fpr fpr_ptwo63m1 = { 9223372036854775807.0 };
static const fpr fpr_mtwo63m1 = { -9223372036854775807.0 };
static const fpr fpr_ptwo63 = { 9223372036854775808.0 };

static inline int64_t
fpr_rint(fpr x) {
    /*
     * We do not want to use llrint() since it might be not
     * constant-time.
     *
     * Suppose that x >= 0. If x >= 2^52, then it is already an
     * integer. Otherwise, if x < 2^52, then computing x+2^52 will
     * yield a value that will be rounded to the nearest integer
     * with exactly the right rules (round-to-nearest-even).
     *
     * In order to have constant-time processing, we must do the
     * computation for both x >= 0 and x < 0 cases, and use a
     * cast to an integer to access the sign and select the proper
     * value. Such casts also allow us to find out if |x| < 2^52.
     */
    int64_t sx, tx, rp, rn, m;
    uint32_t ub;

    sx = (int64_t)(x.v - 1.0);
    tx = (int64_t)x.v;
    rp = (int64_t)(x.v + 4503599627370496.0) - 4503599627370496;
    rn = (int64_t)(x.v - 4503599627370496.0) + 4503599627370496;

    /*
     * If tx >= 2^52 or tx < -2^52, then result is tx.
     * Otherwise, if sx >= 0, then result is rp.
     * Otherwise, result is rn. We use the fact that when x is
     * close to 0 (|x| <= 0.25) then both rp and rn are correct;
     * and if x is not close to 0, then trunc(x-1.0) yields the
     * appropriate sign.
     */

    /*
     * Clamp rp to zero if tx < 0.
     * Clamp rn to zero if tx >= 0.
     */
    m = sx >> 63;
    rn &= m;
    rp &= ~m;

    /*
     * Get the 12 upper bits of tx; if they are not all zeros or
     * all ones, then tx >= 2^52 or tx < -2^52, and we clamp both
     * rp and rn to zero. Otherwise, we clamp tx to zero.
     */
    ub = (uint32_t)((uint64_t)tx >> 52);
    m = -(int64_t)((((ub + 1) & 0xFFF) - 2) >> 31);
    rp &= m;
    rn &= m;
    tx &= ~m;

    /*
     * Only one of tx, rn or rp (at most) can be non-zero at this
     * point.
     */
    return tx | rn | rp;
}

static inline int64_t
fpr_floor(fpr x) {
    int64_t r;

    /*
     * The cast performs a trunc() (rounding toward 0) and thus is
     * wrong by 1 for most negative values. The correction below is
     * constant-time as long as the compiler turns the
     * floating-point conversion result into a 0/1 integer without a
     * conditional branch or another non-constant-time construction.
     * This should hold on all modern architectures with an FPU (and
     * if it is false on a given arch, then chances are that the FPU
     * itself is not constant-time, making the point moot).
     */
    r = (int64_t)x.v;
    return r - (x.v < (double)r);
}

static inline int64_t
fpr_trunc(fpr x) {
    return (int64_t)x.v;
}

static inline fpr
fpr_add(fpr x, fpr y) {
    return FPR(x.v + y.v);
}

static inline fpr
fpr_sub(fpr x, fpr y) {
    return FPR(x.v - y.v);
}

static inline fpr
fpr_neg(fpr x) {
    return FPR(-x.v);
}

static inline fpr
fpr_half(fpr x) {
    return FPR(x.v * 0.5);
}

static inline fpr
fpr_double(fpr x) {
    return FPR(x.v + x.v);
}

static inline fpr
fpr_mul(fpr x, fpr y) {
    return FPR(x.v * y.v);
}

static inline fpr
fpr_sqr(fpr x) {
    return FPR(x.v * x.v);
}

static inline fpr
fpr_inv(fpr x) {
    return FPR(1.0 / x.v);
}

static inline fpr
fpr_div(fpr x, fpr y) {
    return FPR(x.v / y.v);
}

static inline void
fpr_sqrt_avx2(double *t) {
    __m128d x;

    x = _mm_load1_pd(t);
    x = _mm_sqrt_pd(x);
    _mm_storel_pd(t, x);
}

static inline fpr
fpr_sqrt(fpr x) {
    /*
     * We prefer not to have a dependency on libm when it can be
     * avoided. On x86, calling the sqrt() libm function inlines
     * the relevant opcode (fsqrt or sqrtsd, depending on whether
     * the 387 FPU or SSE2 is used for floating-point operations)
     * but then makes an optional call to the library function
     * for proper error handling, in case the operand is negative.
     *
     * To avoid this dependency, we use intrinsics or inline assembly
     * on recognized platforms:
     *
     *  - If AVX2 is explicitly enabled, then we use SSE2 intrinsics.
     *
     *  - On GCC/Clang with SSE maths, we use SSE2 intrinsics.
     *
     *  - On GCC/Clang on i386, or MSVC on i386, we use inline assembly
     *    to call the 387 FPU fsqrt opcode.
     *
     *  - On GCC/Clang/XLC on PowerPC, we use inline assembly to call
     *    the fsqrt opcode (Clang needs a special hack).
     *
     *  - On GCC/Clang on ARM with hardware floating-point, we use
     *    inline assembly to call the vqsrt.f64 opcode. Due to a
     *    complex ecosystem of compilers and assembly syntaxes, we
     *    have to call it "fsqrt" or "fsqrtd", depending on case.
     *
     * If the platform is not recognized, a call to the system
     * library function sqrt() is performed. On some compilers, this
     * may actually inline the relevant opcode, and call the library
     * function only when the input is invalid (e.g. negative);
     * Falcon never actually calls sqrt() on a negative value, but
     * the dependency to libm will still be there.
     */

    fpr_sqrt_avx2(&x.v);
    return x;
}

static inline int
fpr_lt(fpr x, fpr y) {
    return x.v < y.v;
}

static inline uint64_t
fpr_expm_p63(fpr x, fpr ccs) {
    /*
     * Polynomial approximation of exp(-x) is taken from FACCT:
     *   https://eprint.iacr.org/2018/1234
     * Specifically, values are extracted from the implementation
     * referenced from the FACCT article, and available at:
     *   https://github.com/raykzhao/gaussian
     * Tests over more than 24 billions of random inputs in the
     * 0..log(2) range have never shown a deviation larger than
     * 2^(-50) from the true mathematical value.
     */

    /*
     * AVX2 implementation uses more operations than Horner's method,
     * but with a lower expression tree depth. This helps because
     * additions and multiplications have a latency of 4 cycles on
     * a Skylake, but the CPU can issue two of them per cycle.
     */

    static const union {
        double d[12];
        __m256d v[3];
    } c = {
        {
            0.999999999999994892974086724280,
            0.500000000000019206858326015208,
            0.166666666666984014666397229121,
            0.041666666666110491190622155955,
            0.008333333327800835146903501993,
            0.001388888894063186997887560103,
            0.000198412739277311890541063977,
            0.000024801566833585381209939524,
            0.000002755586350219122514855659,
            0.000000275607356160477811864927,
            0.000000025299506379442070029551,
            0.000000002073772366009083061987
        }
    };

    double d1, d2, d4, d8, y;
    __m256d d14, d58, d9c;

    d1 = -x.v;
    d2 = d1 * d1;
    d4 = d2 * d2;
    d8 = d4 * d4;
    d14 = _mm256_set_pd(d4, d2 * d1, d2, d1);
    d58 = _mm256_mul_pd(d14, _mm256_set1_pd(d4));
    d9c = _mm256_mul_pd(d14, _mm256_set1_pd(d8));
    d14 = _mm256_mul_pd(d14, _mm256_loadu_pd(&c.d[0]));
    d58 = FMADD(d58, _mm256_loadu_pd(&c.d[4]), d14);
    d9c = FMADD(d9c, _mm256_loadu_pd(&c.d[8]), d58);
    d9c = _mm256_hadd_pd(d9c, d9c);
    y = 1.0 + _mm_cvtsd_f64(_mm256_castpd256_pd128(d9c)) // _mm256_cvtsd_f64(d9c)
        + _mm_cvtsd_f64(_mm256_extractf128_pd(d9c, 1));
    y *= ccs.v;

    /*
     * Final conversion goes through int64_t first, because that's what
     * the underlying opcode (vcvttsd2si) will do, and we know that the
     * result will fit, since x >= 0 and ccs < 1. If we did the
     * conversion directly to uint64_t, then the compiler would add some
     * extra code to cover the case of a source value of 2^63 or more,
     * and though the alternate path would never be exercised, the
     * extra comparison would cost us some cycles.
     */
    return (uint64_t)(int64_t)(y * fpr_ptwo63.v);

}

#define fpr_gm_tab   PQCLEAN_FALCON512_AVX2_fpr_gm_tab
extern const fpr fpr_gm_tab[];

#define fpr_p2_tab   PQCLEAN_FALCON512_AVX2_fpr_p2_tab
extern const fpr fpr_p2_tab[];

/* ====================================================================== */
