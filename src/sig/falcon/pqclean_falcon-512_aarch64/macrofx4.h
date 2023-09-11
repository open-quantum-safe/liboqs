/*
 * 64-bit Floating point NEON macro x4
 *
 * =============================================================================
 * Copyright (c) 2023 by Cryptographic Engineering Research Group (CERG)
 * ECE Department, George Mason University
 * Fairfax, VA, U.S.A.
 * Author: Duc Tri Nguyen
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * =============================================================================
 * @author   Duc Tri Nguyen <dnguye69@gmu.edu>, <cothannguyen@gmail.com>
 */

#include <arm_neon.h>
#include "macrof.h"

#define vloadx4(c, addr) c = vld1q_f64_x4(addr);

#define vstorex4(addr, c) vst1q_f64_x4(addr, c);

#define vfdupx4(c, constant)          \
    c.val[0] = vdupq_n_f64(constant); \
    c.val[1] = vdupq_n_f64(constant); \
    c.val[2] = vdupq_n_f64(constant); \
    c.val[3] = vdupq_n_f64(constant);

#define vfnegx4(c, a)               \
    c.val[0] = vnegq_f64(a.val[0]); \
    c.val[1] = vnegq_f64(a.val[1]); \
    c.val[2] = vnegq_f64(a.val[2]); \
    c.val[3] = vnegq_f64(a.val[3]);

#define vfmulnx4(c, a, n)                \
    c.val[0] = vmulq_n_f64(a.val[0], n); \
    c.val[1] = vmulq_n_f64(a.val[1], n); \
    c.val[2] = vmulq_n_f64(a.val[2], n); \
    c.val[3] = vmulq_n_f64(a.val[3], n);

// c = a - b
#define vfsubx4(c, a, b)                      \
    c.val[0] = vsubq_f64(a.val[0], b.val[0]); \
    c.val[1] = vsubq_f64(a.val[1], b.val[1]); \
    c.val[2] = vsubq_f64(a.val[2], b.val[2]); \
    c.val[3] = vsubq_f64(a.val[3], b.val[3]);

// c = a + b
#define vfaddx4(c, a, b)                      \
    c.val[0] = vaddq_f64(a.val[0], b.val[0]); \
    c.val[1] = vaddq_f64(a.val[1], b.val[1]); \
    c.val[2] = vaddq_f64(a.val[2], b.val[2]); \
    c.val[3] = vaddq_f64(a.val[3], b.val[3]);

#define vfmulx4(c, a, b)                      \
    c.val[0] = vmulq_f64(a.val[0], b.val[0]); \
    c.val[1] = vmulq_f64(a.val[1], b.val[1]); \
    c.val[2] = vmulq_f64(a.val[2], b.val[2]); \
    c.val[3] = vmulq_f64(a.val[3], b.val[3]);

#define vfmulx4_i(c, a, b)             \
    c.val[0] = vmulq_f64(a.val[0], b); \
    c.val[1] = vmulq_f64(a.val[1], b); \
    c.val[2] = vmulq_f64(a.val[2], b); \
    c.val[3] = vmulq_f64(a.val[3], b);

#define vfinvx4(c, a)                                 \
    c.val[0] = vdivq_f64(vdupq_n_f64(1.0), a.val[0]); \
    c.val[1] = vdivq_f64(vdupq_n_f64(1.0), a.val[1]); \
    c.val[2] = vdivq_f64(vdupq_n_f64(1.0), a.val[2]); \
    c.val[3] = vdivq_f64(vdupq_n_f64(1.0), a.val[3]);

#define vfcvtx4(c, a)                   \
    c.val[0] = vcvtq_f64_s64(a.val[0]); \
    c.val[1] = vcvtq_f64_s64(a.val[1]); \
    c.val[2] = vcvtq_f64_s64(a.val[2]); \
    c.val[3] = vcvtq_f64_s64(a.val[3]);

#define vfmlax4(d, c, a, b)                        \
    vfmla(d.val[0], c.val[0], a.val[0], b.val[0]); \
    vfmla(d.val[1], c.val[1], a.val[1], b.val[1]); \
    vfmla(d.val[2], c.val[2], a.val[2], b.val[2]); \
    vfmla(d.val[3], c.val[3], a.val[3], b.val[3]);

#define vfmlsx4(d, c, a, b)                        \
    vfmls(d.val[0], c.val[0], a.val[0], b.val[0]); \
    vfmls(d.val[1], c.val[1], a.val[1], b.val[1]); \
    vfmls(d.val[2], c.val[2], a.val[2], b.val[2]); \
    vfmls(d.val[3], c.val[3], a.val[3], b.val[3]);

#define vfrintx4(c, a)                   \
    c.val[0] = vcvtnq_s64_f64(a.val[0]); \
    c.val[1] = vcvtnq_s64_f64(a.val[1]); \
    c.val[2] = vcvtnq_s64_f64(a.val[2]); \
    c.val[3] = vcvtnq_s64_f64(a.val[3]);

/*
 * Wrapper for FFT, split/merge and poly_float.c
 */

#define FPC_MUL(d_re, d_im, a_re, a_im, b_re, b_im) \
    vfmul(d_re, a_re, b_re);                        \
    vfmls(d_re, d_re, a_im, b_im);                  \
    vfmul(d_im, a_re, b_im);                        \
    vfmla(d_im, d_im, a_im, b_re);

#define FPC_MULx2(d_re, d_im, a_re, a_im, b_re, b_im)          \
    vfmul(d_re.val[0], a_re.val[0], b_re.val[0]);              \
    vfmls(d_re.val[0], d_re.val[0], a_im.val[0], b_im.val[0]); \
    vfmul(d_re.val[1], a_re.val[1], b_re.val[1]);              \
    vfmls(d_re.val[1], d_re.val[1], a_im.val[1], b_im.val[1]); \
    vfmul(d_im.val[0], a_re.val[0], b_im.val[0]);              \
    vfmla(d_im.val[0], d_im.val[0], a_im.val[0], b_re.val[0]); \
    vfmul(d_im.val[1], a_re.val[1], b_im.val[1]);              \
    vfmla(d_im.val[1], d_im.val[1], a_im.val[1], b_re.val[1]);

#define FPC_MULx4(d_re, d_im, a_re, a_im, b_re, b_im)          \
    vfmul(d_re.val[0], a_re.val[0], b_re.val[0]);              \
    vfmls(d_re.val[0], d_re.val[0], a_im.val[0], b_im.val[0]); \
    vfmul(d_re.val[1], a_re.val[1], b_re.val[1]);              \
    vfmls(d_re.val[1], d_re.val[1], a_im.val[1], b_im.val[1]); \
    vfmul(d_re.val[2], a_re.val[2], b_re.val[2]);              \
    vfmls(d_re.val[2], d_re.val[2], a_im.val[2], b_im.val[2]); \
    vfmul(d_re.val[3], a_re.val[3], b_re.val[3]);              \
    vfmls(d_re.val[3], d_re.val[3], a_im.val[3], b_im.val[3]); \
    vfmul(d_im.val[0], a_re.val[0], b_im.val[0]);              \
    vfmla(d_im.val[0], d_im.val[0], a_im.val[0], b_re.val[0]); \
    vfmul(d_im.val[1], a_re.val[1], b_im.val[1]);              \
    vfmla(d_im.val[1], d_im.val[1], a_im.val[1], b_re.val[1]); \
    vfmul(d_im.val[2], a_re.val[2], b_im.val[2]);              \
    vfmla(d_im.val[2], d_im.val[2], a_im.val[2], b_re.val[2]); \
    vfmul(d_im.val[3], a_re.val[3], b_im.val[3]);              \
    vfmla(d_im.val[3], d_im.val[3], a_im.val[3], b_re.val[3]);

#define FPC_MLA(d_re, d_im, a_re, a_im, b_re, b_im) \
    vfmla(d_re, d_re, a_re, b_re);                  \
    vfmls(d_re, d_re, a_im, b_im);                  \
    vfmla(d_im, d_im, a_re, b_im);                  \
    vfmla(d_im, d_im, a_im, b_re);

#define FPC_MLAx2(d_re, d_im, a_re, a_im, b_re, b_im)          \
    vfmla(d_re.val[0], d_re.val[0], a_re.val[0], b_re.val[0]); \
    vfmls(d_re.val[0], d_re.val[0], a_im.val[0], b_im.val[0]); \
    vfmla(d_re.val[1], d_re.val[1], a_re.val[1], b_re.val[1]); \
    vfmls(d_re.val[1], d_re.val[1], a_im.val[1], b_im.val[1]); \
    vfmla(d_im.val[0], d_im.val[0], a_re.val[0], b_im.val[0]); \
    vfmla(d_im.val[0], d_im.val[0], a_im.val[0], b_re.val[0]); \
    vfmla(d_im.val[1], d_im.val[1], a_re.val[1], b_im.val[1]); \
    vfmla(d_im.val[1], d_im.val[1], a_im.val[1], b_re.val[1]);

#define FPC_MLAx4(d_re, d_im, a_re, a_im, b_re, b_im)          \
    vfmla(d_re.val[0], d_re.val[0], a_re.val[0], b_re.val[0]); \
    vfmls(d_re.val[0], d_re.val[0], a_im.val[0], b_im.val[0]); \
    vfmla(d_re.val[1], d_re.val[1], a_re.val[1], b_re.val[1]); \
    vfmls(d_re.val[1], d_re.val[1], a_im.val[1], b_im.val[1]); \
    vfmla(d_re.val[2], d_re.val[2], a_re.val[2], b_re.val[2]); \
    vfmls(d_re.val[2], d_re.val[2], a_im.val[2], b_im.val[2]); \
    vfmla(d_re.val[3], d_re.val[3], a_re.val[3], b_re.val[3]); \
    vfmls(d_re.val[3], d_re.val[3], a_im.val[3], b_im.val[3]); \
    vfmla(d_im.val[0], d_im.val[0], a_re.val[0], b_im.val[0]); \
    vfmla(d_im.val[0], d_im.val[0], a_im.val[0], b_re.val[0]); \
    vfmla(d_im.val[1], d_im.val[1], a_re.val[1], b_im.val[1]); \
    vfmla(d_im.val[1], d_im.val[1], a_im.val[1], b_re.val[1]); \
    vfmla(d_im.val[2], d_im.val[2], a_re.val[2], b_im.val[2]); \
    vfmla(d_im.val[2], d_im.val[2], a_im.val[2], b_re.val[2]); \
    vfmla(d_im.val[3], d_im.val[3], a_re.val[3], b_im.val[3]); \
    vfmla(d_im.val[3], d_im.val[3], a_im.val[3], b_re.val[3]);

#define FPC_MUL_CONJx4(d_re, d_im, a_re, a_im, b_re, b_im)     \
    vfmul(d_re.val[0], b_im.val[0], a_im.val[0]);              \
    vfmla(d_re.val[0], d_re.val[0], a_re.val[0], b_re.val[0]); \
    vfmul(d_re.val[1], b_im.val[1], a_im.val[1]);              \
    vfmla(d_re.val[1], d_re.val[1], a_re.val[1], b_re.val[1]); \
    vfmul(d_re.val[2], b_im.val[2], a_im.val[2]);              \
    vfmla(d_re.val[2], d_re.val[2], a_re.val[2], b_re.val[2]); \
    vfmul(d_re.val[3], b_im.val[3], a_im.val[3]);              \
    vfmla(d_re.val[3], d_re.val[3], a_re.val[3], b_re.val[3]); \
    vfmul(d_im.val[0], b_re.val[0], a_im.val[0]);              \
    vfmls(d_im.val[0], d_im.val[0], a_re.val[0], b_im.val[0]); \
    vfmul(d_im.val[1], b_re.val[1], a_im.val[1]);              \
    vfmls(d_im.val[1], d_im.val[1], a_re.val[1], b_im.val[1]); \
    vfmul(d_im.val[2], b_re.val[2], a_im.val[2]);              \
    vfmls(d_im.val[2], d_im.val[2], a_re.val[2], b_im.val[2]); \
    vfmul(d_im.val[3], b_re.val[3], a_im.val[3]);              \
    vfmls(d_im.val[3], d_im.val[3], a_re.val[3], b_im.val[3]);

#define FPC_MLA_CONJx4(d_re, d_im, a_re, a_im, b_re, b_im)     \
    vfmla(d_re.val[0], d_re.val[0], b_im.val[0], a_im.val[0]); \
    vfmla(d_re.val[0], d_re.val[0], a_re.val[0], b_re.val[0]); \
    vfmla(d_re.val[1], d_re.val[1], b_im.val[1], a_im.val[1]); \
    vfmla(d_re.val[1], d_re.val[1], a_re.val[1], b_re.val[1]); \
    vfmla(d_re.val[2], d_re.val[2], b_im.val[2], a_im.val[2]); \
    vfmla(d_re.val[2], d_re.val[2], a_re.val[2], b_re.val[2]); \
    vfmla(d_re.val[3], d_re.val[3], b_im.val[3], a_im.val[3]); \
    vfmla(d_re.val[3], d_re.val[3], a_re.val[3], b_re.val[3]); \
    vfmla(d_im.val[0], d_im.val[0], b_re.val[0], a_im.val[0]); \
    vfmls(d_im.val[0], d_im.val[0], a_re.val[0], b_im.val[0]); \
    vfmla(d_im.val[1], d_im.val[1], b_re.val[1], a_im.val[1]); \
    vfmls(d_im.val[1], d_im.val[1], a_re.val[1], b_im.val[1]); \
    vfmla(d_im.val[2], d_im.val[2], b_re.val[2], a_im.val[2]); \
    vfmls(d_im.val[2], d_im.val[2], a_re.val[2], b_im.val[2]); \
    vfmla(d_im.val[3], d_im.val[3], b_re.val[3], a_im.val[3]); \
    vfmls(d_im.val[3], d_im.val[3], a_re.val[3], b_im.val[3]);

#define FPC_MUL_LANE(d_re, d_im, a_re, a_im, b_re_im) \
    vfmul_lane(d_re, a_re, b_re_im, 0);               \
    vfmls_lane(d_re, d_re, a_im, b_re_im, 1);         \
    vfmul_lane(d_im, a_re, b_re_im, 1);               \
    vfmla_lane(d_im, d_im, a_im, b_re_im, 0);

#define FPC_MUL_LANEx4(d_re, d_im, a_re, a_im, b_re_im)            \
    vfmul_lane(d_re.val[0], a_re.val[0], b_re_im, 0);              \
    vfmls_lane(d_re.val[0], d_re.val[0], a_im.val[0], b_re_im, 1); \
    vfmul_lane(d_re.val[1], a_re.val[1], b_re_im, 0);              \
    vfmls_lane(d_re.val[1], d_re.val[1], a_im.val[1], b_re_im, 1); \
    vfmul_lane(d_re.val[2], a_re.val[2], b_re_im, 0);              \
    vfmls_lane(d_re.val[2], d_re.val[2], a_im.val[2], b_re_im, 1); \
    vfmul_lane(d_re.val[3], a_re.val[3], b_re_im, 0);              \
    vfmls_lane(d_re.val[3], d_re.val[3], a_im.val[3], b_re_im, 1); \
    vfmul_lane(d_im.val[0], a_re.val[0], b_re_im, 1);              \
    vfmla_lane(d_im.val[0], d_im.val[0], a_im.val[0], b_re_im, 0); \
    vfmul_lane(d_im.val[1], a_re.val[1], b_re_im, 1);              \
    vfmla_lane(d_im.val[1], d_im.val[1], a_im.val[1], b_re_im, 0); \
    vfmul_lane(d_im.val[2], a_re.val[2], b_re_im, 1);              \
    vfmla_lane(d_im.val[2], d_im.val[2], a_im.val[2], b_re_im, 0); \
    vfmul_lane(d_im.val[3], a_re.val[3], b_re_im, 1);              \
    vfmla_lane(d_im.val[3], d_im.val[3], a_im.val[3], b_re_im, 0);

#define FWD_TOP(t_re, t_im, b_re, b_im, zeta_re, zeta_im) \
    FPC_MUL(t_re, t_im, b_re, b_im, zeta_re, zeta_im);

#define FWD_TOP_LANE(t_re, t_im, b_re, b_im, zeta) \
    FPC_MUL_LANE(t_re, t_im, b_re, b_im, zeta);

#define FWD_TOP_LANEx4(t_re, t_im, b_re, b_im, zeta) \
    FPC_MUL_LANEx4(t_re, t_im, b_re, b_im, zeta);

/*
 * FPC
 */

#define FPC_SUB(d_re, d_im, a_re, a_im, b_re, b_im) \
    d_re = vsubq_f64(a_re, b_re);                   \
    d_im = vsubq_f64(a_im, b_im);

#define FPC_SUBx4(d_re, d_im, a_re, a_im, b_re, b_im)  \
    d_re.val[0] = vsubq_f64(a_re.val[0], b_re.val[0]); \
    d_im.val[0] = vsubq_f64(a_im.val[0], b_im.val[0]); \
    d_re.val[1] = vsubq_f64(a_re.val[1], b_re.val[1]); \
    d_im.val[1] = vsubq_f64(a_im.val[1], b_im.val[1]); \
    d_re.val[2] = vsubq_f64(a_re.val[2], b_re.val[2]); \
    d_im.val[2] = vsubq_f64(a_im.val[2], b_im.val[2]); \
    d_re.val[3] = vsubq_f64(a_re.val[3], b_re.val[3]); \
    d_im.val[3] = vsubq_f64(a_im.val[3], b_im.val[3]);

#define FPC_ADD(d_re, d_im, a_re, a_im, b_re, b_im) \
    d_re = vaddq_f64(a_re, b_re);                   \
    d_im = vaddq_f64(a_im, b_im);

#define FPC_ADDx4(d_re, d_im, a_re, a_im, b_re, b_im)  \
    d_re.val[0] = vaddq_f64(a_re.val[0], b_re.val[0]); \
    d_im.val[0] = vaddq_f64(a_im.val[0], b_im.val[0]); \
    d_re.val[1] = vaddq_f64(a_re.val[1], b_re.val[1]); \
    d_im.val[1] = vaddq_f64(a_im.val[1], b_im.val[1]); \
    d_re.val[2] = vaddq_f64(a_re.val[2], b_re.val[2]); \
    d_im.val[2] = vaddq_f64(a_im.val[2], b_im.val[2]); \
    d_re.val[3] = vaddq_f64(a_re.val[3], b_re.val[3]); \
    d_im.val[3] = vaddq_f64(a_im.val[3], b_im.val[3]);

#define FWD_BOT(a_re, a_im, b_re, b_im, t_re, t_im) \
    FPC_SUB(b_re, b_im, a_re, a_im, t_re, t_im);    \
    FPC_ADD(a_re, a_im, a_re, a_im, t_re, t_im);

#define FWD_BOTx4(a_re, a_im, b_re, b_im, t_re, t_im) \
    FPC_SUBx4(b_re, b_im, a_re, a_im, t_re, t_im);    \
    FPC_ADDx4(a_re, a_im, a_re, a_im, t_re, t_im);

/*
 * FPC_J
 */

#define FPC_ADDJ(d_re, d_im, a_re, a_im, b_re, b_im) \
    d_re = vsubq_f64(a_re, b_im);                    \
    d_im = vaddq_f64(a_im, b_re);

#define FPC_ADDJx4(d_re, d_im, a_re, a_im, b_re, b_im) \
    d_re.val[0] = vsubq_f64(a_re.val[0], b_im.val[0]); \
    d_im.val[0] = vaddq_f64(a_im.val[0], b_re.val[0]); \
    d_re.val[1] = vsubq_f64(a_re.val[1], b_im.val[1]); \
    d_im.val[1] = vaddq_f64(a_im.val[1], b_re.val[1]); \
    d_re.val[2] = vsubq_f64(a_re.val[2], b_im.val[2]); \
    d_im.val[2] = vaddq_f64(a_im.val[2], b_re.val[2]); \
    d_re.val[3] = vsubq_f64(a_re.val[3], b_im.val[3]); \
    d_im.val[3] = vaddq_f64(a_im.val[3], b_re.val[3]);

#define FPC_SUBJ(d_re, d_im, a_re, a_im, b_re, b_im) \
    d_re = vaddq_f64(a_re, b_im);                    \
    d_im = vsubq_f64(a_im, b_re);

#define FPC_SUBJx4(d_re, d_im, a_re, a_im, b_re, b_im) \
    d_re.val[0] = vaddq_f64(a_re.val[0], b_im.val[0]); \
    d_im.val[0] = vsubq_f64(a_im.val[0], b_re.val[0]); \
    d_re.val[1] = vaddq_f64(a_re.val[1], b_im.val[1]); \
    d_im.val[1] = vsubq_f64(a_im.val[1], b_re.val[1]); \
    d_re.val[2] = vaddq_f64(a_re.val[2], b_im.val[2]); \
    d_im.val[2] = vsubq_f64(a_im.val[2], b_re.val[2]); \
    d_re.val[3] = vaddq_f64(a_re.val[3], b_im.val[3]); \
    d_im.val[3] = vsubq_f64(a_im.val[3], b_re.val[3]);

#define FWD_BOTJ(a_re, a_im, b_re, b_im, t_re, t_im) \
    FPC_SUBJ(b_re, b_im, a_re, a_im, t_re, t_im);    \
    FPC_ADDJ(a_re, a_im, a_re, a_im, t_re, t_im);

#define FWD_BOTJx4(a_re, a_im, b_re, b_im, t_re, t_im) \
    FPC_SUBJx4(b_re, b_im, a_re, a_im, t_re, t_im);    \
    FPC_ADDJx4(a_re, a_im, a_re, a_im, t_re, t_im);

//============== Inverse FFT
/*
 * FPC_J
 * a * conj(b)
 * Original (without swap):
 * d_re = b_im * a_im + a_re * b_re;
 * d_im = b_re * a_im - a_re * b_im;
 */
#define FPC_MUL_BOTJ_LANE(d_re, d_im, a_re, a_im, b_re_im) \
    vfmul_lane(d_re, a_re, b_re_im, 0);                    \
    vfmla_lane(d_re, d_re, a_im, b_re_im, 1);              \
    vfmul_lane(d_im, a_im, b_re_im, 0);                    \
    vfmls_lane(d_im, d_im, a_re, b_re_im, 1);

#define FPC_MUL_BOTJ_LANEx4(d_re, d_im, a_re, a_im, b_re_im)       \
    vfmul_lane(d_re.val[0], a_re.val[0], b_re_im, 0);              \
    vfmla_lane(d_re.val[0], d_re.val[0], a_im.val[0], b_re_im, 1); \
    vfmul_lane(d_im.val[0], a_im.val[0], b_re_im, 0);              \
    vfmls_lane(d_im.val[0], d_im.val[0], a_re.val[0], b_re_im, 1); \
    vfmul_lane(d_re.val[1], a_re.val[1], b_re_im, 0);              \
    vfmla_lane(d_re.val[1], d_re.val[1], a_im.val[1], b_re_im, 1); \
    vfmul_lane(d_im.val[1], a_im.val[1], b_re_im, 0);              \
    vfmls_lane(d_im.val[1], d_im.val[1], a_re.val[1], b_re_im, 1); \
    vfmul_lane(d_re.val[2], a_re.val[2], b_re_im, 0);              \
    vfmla_lane(d_re.val[2], d_re.val[2], a_im.val[2], b_re_im, 1); \
    vfmul_lane(d_im.val[2], a_im.val[2], b_re_im, 0);              \
    vfmls_lane(d_im.val[2], d_im.val[2], a_re.val[2], b_re_im, 1); \
    vfmul_lane(d_re.val[3], a_re.val[3], b_re_im, 0);              \
    vfmla_lane(d_re.val[3], d_re.val[3], a_im.val[3], b_re_im, 1); \
    vfmul_lane(d_im.val[3], a_im.val[3], b_re_im, 0);              \
    vfmls_lane(d_im.val[3], d_im.val[3], a_re.val[3], b_re_im, 1);

#define FPC_MUL_BOTJ(d_re, d_im, a_re, a_im, b_re, b_im) \
    vfmul(d_re, b_im, a_im);                             \
    vfmla(d_re, d_re, a_re, b_re);                       \
    vfmul(d_im, b_re, a_im);                             \
    vfmls(d_im, d_im, a_re, b_im);

#define INV_TOPJ(t_re, t_im, a_re, a_im, b_re, b_im) \
    FPC_SUB(t_re, t_im, a_re, a_im, b_re, b_im);     \
    FPC_ADD(a_re, a_im, a_re, a_im, b_re, b_im);

#define INV_TOPJx4(t_re, t_im, a_re, a_im, b_re, b_im) \
    FPC_SUBx4(t_re, t_im, a_re, a_im, b_re, b_im);     \
    FPC_ADDx4(a_re, a_im, a_re, a_im, b_re, b_im);

#define INV_BOTJ(b_re, b_im, t_re, t_im, zeta_re, zeta_im) \
    FPC_MUL_BOTJ(b_re, b_im, t_re, t_im, zeta_re, zeta_im);

#define INV_BOTJ_LANE(b_re, b_im, t_re, t_im, zeta) \
    FPC_MUL_BOTJ_LANE(b_re, b_im, t_re, t_im, zeta);

#define INV_BOTJ_LANEx4(b_re, b_im, t_re, t_im, zeta) \
    FPC_MUL_BOTJ_LANEx4(b_re, b_im, t_re, t_im, zeta);

/*
 * FPC_Jm
 * a * -conj(b)
 * d_re = a_re * b_im - a_im * b_re;
 * d_im = a_im * b_im + a_re * b_re;
 */
#define FPC_MUL_BOTJm_LANE(d_re, d_im, a_re, a_im, b_re_im) \
    vfmul_lane(d_re, a_re, b_re_im, 1);                     \
    vfmls_lane(d_re, d_re, a_im, b_re_im, 0);               \
    vfmul_lane(d_im, a_re, b_re_im, 0);                     \
    vfmla_lane(d_im, d_im, a_im, b_re_im, 1);

#define FPC_MUL_BOTJm_LANEx4(d_re, d_im, a_re, a_im, b_re_im)      \
    vfmul_lane(d_re.val[0], a_re.val[0], b_re_im, 1);              \
    vfmls_lane(d_re.val[0], d_re.val[0], a_im.val[0], b_re_im, 0); \
    vfmul_lane(d_im.val[0], a_re.val[0], b_re_im, 0);              \
    vfmla_lane(d_im.val[0], d_im.val[0], a_im.val[0], b_re_im, 1); \
    vfmul_lane(d_re.val[1], a_re.val[1], b_re_im, 1);              \
    vfmls_lane(d_re.val[1], d_re.val[1], a_im.val[1], b_re_im, 0); \
    vfmul_lane(d_im.val[1], a_re.val[1], b_re_im, 0);              \
    vfmla_lane(d_im.val[1], d_im.val[1], a_im.val[1], b_re_im, 1); \
    vfmul_lane(d_re.val[2], a_re.val[2], b_re_im, 1);              \
    vfmls_lane(d_re.val[2], d_re.val[2], a_im.val[2], b_re_im, 0); \
    vfmul_lane(d_im.val[2], a_re.val[2], b_re_im, 0);              \
    vfmla_lane(d_im.val[2], d_im.val[2], a_im.val[2], b_re_im, 1); \
    vfmul_lane(d_re.val[3], a_re.val[3], b_re_im, 1);              \
    vfmls_lane(d_re.val[3], d_re.val[3], a_im.val[3], b_re_im, 0); \
    vfmul_lane(d_im.val[3], a_re.val[3], b_re_im, 0);              \
    vfmla_lane(d_im.val[3], d_im.val[3], a_im.val[3], b_re_im, 1);

#define FPC_MUL_BOTJm(d_re, d_im, a_re, a_im, b_re, b_im) \
    vfmul(d_re, a_re, b_im);                              \
    vfmls(d_re, d_re, a_im, b_re);                        \
    vfmul(d_im, a_im, b_im);                              \
    vfmla(d_im, d_im, a_re, b_re);

#define INV_TOPJm(t_re, t_im, a_re, a_im, b_re, b_im) \
    FPC_SUB(t_re, t_im, b_re, b_im, a_re, a_im);      \
    FPC_ADD(a_re, a_im, a_re, a_im, b_re, b_im);

#define INV_TOPJmx4(t_re, t_im, a_re, a_im, b_re, b_im) \
    FPC_SUBx4(t_re, t_im, b_re, b_im, a_re, a_im);      \
    FPC_ADDx4(a_re, a_im, a_re, a_im, b_re, b_im);

#define INV_BOTJm(b_re, b_im, t_re, t_im, zeta_re, zeta_im) \
    FPC_MUL_BOTJm(b_re, b_im, t_re, t_im, zeta_re, zeta_im);

#define INV_BOTJm_LANE(b_re, b_im, t_re, t_im, zeta) \
    FPC_MUL_BOTJm_LANE(b_re, b_im, t_re, t_im, zeta);

#define INV_BOTJm_LANEx4(b_re, b_im, t_re, t_im, zeta) \
    FPC_MUL_BOTJm_LANEx4(b_re, b_im, t_re, t_im, zeta);
