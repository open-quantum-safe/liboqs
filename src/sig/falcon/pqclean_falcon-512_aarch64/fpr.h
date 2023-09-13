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

#include <arm_neon.h>
#include <math.h>

#include "macrof.h"
/*
 * We wrap the native 'double' type into a structure so that the C compiler
 * complains if we inadvertently use raw arithmetic operators on the 'fpr'
 * type instead of using the inline functions below. This should have no
 * extra runtime cost, since all the functions below are 'inline'.
 */
typedef double fpr;

static inline fpr
FPR(double v) {
    fpr x;

    x = v;
    return x;
}

static inline fpr
fpr_of(int64_t i) {
    return (double)i;
}

static const fpr fpr_q = 12289.0 ;
static const fpr fpr_inverse_of_q = 1.0 / 12289.0 ;
static const fpr fpr_inv_2sqrsigma0 = .150865048875372721532312163019 ;
static const fpr fpr_inv_sigma_9 = 0.0060336696681577241031668062510953022 ;
static const fpr fpr_sigma_min_9 = 1.2778336969128335860256340575729042 ;
static const fpr fpr_log2 = 0.69314718055994530941723212146 ;
static const fpr fpr_inv_log2 = 1.4426950408889634073599246810 ;
static const fpr fpr_bnorm_max = 16822.4121 ;
static const fpr fpr_zero = 0.0 ;
static const fpr fpr_one = 1.0 ;
static const fpr fpr_two = 2.0 ;
static const fpr fpr_onehalf = 0.5 ;
static const fpr fpr_invsqrt2 = 0.707106781186547524400844362105 ;
static const fpr fpr_invsqrt8 = 0.353553390593273762200422181052 ;
static const fpr fpr_ptwo31 = 2147483648.0 ;
static const fpr fpr_ptwo31m1 = 2147483647.0 ;
static const fpr fpr_mtwo31m1 = -2147483647.0 ;
static const fpr fpr_ptwo63m1 = 9223372036854775807.0 ;
static const fpr fpr_mtwo63m1 = -9223372036854775807.0 ;
static const fpr fpr_ptwo63 = 9223372036854775808.0 ;

static inline int64_t
fpr_rint(fpr x) {
    int64_t t;
    __asm__ ( "fcvtns   %x0, %d1": "=r" (t) : "w" (x));
    return t;
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
    r = (int64_t)x;
    return r - (x < (double)r);
}

static inline int64_t
fpr_trunc(fpr x) {
    return (int64_t)x;
}

static inline fpr
fpr_add(fpr x, fpr y) {
    return (x + y);
}

static inline fpr
fpr_sub(fpr x, fpr y) {
    return (x - y);
}

static inline fpr
fpr_neg(fpr x) {
    return (-x);
}

static inline fpr
fpr_half(fpr x) {
    return (x * 0.5);
}

static inline fpr
fpr_double(fpr x) {
    return (x + x);
}

static inline fpr
fpr_mul(fpr x, fpr y) {
    return (x * y);
}

static inline fpr
fpr_sqr(fpr x) {
    return (x * x);
}

static inline fpr
fpr_inv(fpr x) {
    return (1.0 / x);
}

static inline fpr
fpr_div(fpr x, fpr y) {
    return (x / y);
}

static inline fpr
fpr_sqrt(fpr x) {
    __asm__ ( "fsqrt   %d0, %d0" : "+w" (x) : : );
    return x;
}

static inline int
fpr_lt(fpr x, fpr y) {
    return x < y;
}

static inline uint64_t
fpr_expm_p63(fpr x, fpr ccs) {
    static const double C_expm[] = {
        1.000000000000000000000000000000,  // c0
        -0.999999999999994892974086724280, // c1
        0.500000000000019206858326015208,  // c2
        -0.166666666666984014666397229121, // c3
        0.041666666666110491190622155955,  // c4
        -0.008333333327800835146903501993, // c5
        0.001388888894063186997887560103,  // c6
        -0.000198412739277311890541063977, // c7
        0.000024801566833585381209939524,  // c8
        -0.000002755586350219122514855659, // c9
        0.000000275607356160477811864927,  // c10
        -0.000000025299506379442070029551, // c11
        0.000000002073772366009083061987,  // c12
        0.000000000000000000000000000000,
    };
    float64x2_t neon_x, neon_1x, neon_x2,
                neon_x4, neon_x8, neon_x12, neon_ccs;
    float64x2x4_t neon_exp0;
    float64x2x3_t neon_exp1;
    float64x2_t y1, y2, y3, y;
    double ret;

    neon_exp0 = vld1q_f64_x4(&C_expm[0]);
    neon_exp1 = vld1q_f64_x3(&C_expm[8]);
    neon_ccs = vdupq_n_f64(ccs);
    neon_ccs = vmulq_n_f64(neon_ccs, fpr_ptwo63);

    // x | x
    neon_x = vdupq_n_f64(x);
    // 1 | x
    neon_1x = vsetq_lane_f64(1.0, neon_x, 0);
    neon_x2 = vmulq_f64(neon_x, neon_x);
    neon_x4 = vmulq_f64(neon_x2, neon_x2);
    neon_x8 = vmulq_f64(neon_x4, neon_x4);
    neon_x12 = vmulq_f64(neon_x8, neon_x4);

    vfmla(y1, neon_exp0.val[0], neon_exp0.val[1], neon_x2);
    vfmla(y2, neon_exp0.val[2], neon_exp0.val[3], neon_x2);
    vfmla(y3, neon_exp1.val[0], neon_exp1.val[1], neon_x2);

    y1 = vmulq_f64(y1, neon_1x);
    y2 = vmulq_f64(y2, neon_1x);
    y3 = vmulq_f64(y3, neon_1x);

    vfmla(y, y1, y2, neon_x4);
    vfmla(y,  y, y3, neon_x8);
    vfmla(y,  y, neon_exp1.val[2], neon_x12);
    y = vmulq_f64( y, neon_ccs);
    ret = vaddvq_f64(y);

    return (uint64_t) ret;
}

#define fpr_p2_tab   PQCLEAN_FALCON512_AARCH64_fpr_p2_tab
extern const fpr fpr_p2_tab[];

#define fpr_tab_log2   PQCLEAN_FALCON512_AARCH64_fpr_tab_log2
#define fpr_tab_log3   PQCLEAN_FALCON512_AARCH64_fpr_tab_log3
#define fpr_tab_log4   PQCLEAN_FALCON512_AARCH64_fpr_tab_log4
#define fpr_tab_log5   PQCLEAN_FALCON512_AARCH64_fpr_tab_log5
#define fpr_tab_log6   PQCLEAN_FALCON512_AARCH64_fpr_tab_log6
#define fpr_tab_log7   PQCLEAN_FALCON512_AARCH64_fpr_tab_log7
#define fpr_tab_log8   PQCLEAN_FALCON512_AARCH64_fpr_tab_log8
#define fpr_tab_log9   PQCLEAN_FALCON512_AARCH64_fpr_tab_log9
#define fpr_table      PQCLEAN_FALCON512_AARCH64_fpr_table

extern const fpr fpr_tab_log2[];
extern const fpr fpr_tab_log3[];
extern const fpr fpr_tab_log4[];
extern const fpr fpr_tab_log5[];
extern const fpr fpr_tab_log6[];
extern const fpr fpr_tab_log7[];
extern const fpr fpr_tab_log8[];
extern const fpr fpr_tab_log9[];
extern const fpr *fpr_table[];

/* ====================================================================== */
