/*
 * High-speed vectorize FFT code for arbitrary `logn`.
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

#include "inner.h"
#include "macrof.h"
#include "macrofx4.h"

/*
 * 1 layer of Forward FFT for 2 complex points (4 coefficients).
 * Note: The scalar version is faster than vectorized code.
 */
static void PQCLEAN_FALCONPADDED1024_AARCH64_FFT_log2(fpr *f) {
    fpr x_re, x_im, y_re, y_im, v_re, v_im, t_re, t_im, s;

    x_re = f[0];
    y_re = f[1];
    x_im = f[2];
    y_im = f[3];
    s = fpr_tab_log2[0];

    t_re = y_re * s;
    t_im = y_im * s;

    v_re = t_re - t_im;
    v_im = t_re + t_im;

    f[0] = x_re + v_re;
    f[1] = x_re - v_re;
    f[2] = x_im + v_im;
    f[3] = x_im - v_im;
}

/*
 * Vectorized 2 layers of Forward FFT for 4 complex points (8 coefficients).
 */
static void PQCLEAN_FALCONPADDED1024_AARCH64_FFT_log3(fpr *f) {
    // Total SIMD registers: 18 = 4 + 6 + 8
    float64x2x4_t tmp;                                        // 4
    float64x2x2_t s_re_im, x, y;                              // 6
    float64x2_t v_re, v_im, x_re, x_im, y_re, y_im, t_x, t_y; // 8

    vloadx4(tmp, &f[0]);
    s_re_im.val[0] = vld1q_dup_f64(&fpr_tab_log2[0]);

    vfmul(v_re, tmp.val[1], s_re_im.val[0]);
    vfmul(v_im, tmp.val[3], s_re_im.val[0]);

    vfsub(t_x, v_re, v_im);
    vfadd(t_y, v_re, v_im);

    vfsub(tmp.val[1], tmp.val[0], t_x);
    vfsub(tmp.val[3], tmp.val[2], t_y);

    vfadd(tmp.val[0], tmp.val[0], t_x);
    vfadd(tmp.val[2], tmp.val[2], t_y);

    x_re = vtrn1q_f64(tmp.val[0], tmp.val[1]);
    y_re = vtrn2q_f64(tmp.val[0], tmp.val[1]);
    x_im = vtrn1q_f64(tmp.val[2], tmp.val[3]);
    y_im = vtrn2q_f64(tmp.val[2], tmp.val[3]);

    vload2(s_re_im, &fpr_tab_log3[0]);

    FWD_TOP(v_re, v_im, y_re, y_im, s_re_im.val[0], s_re_im.val[1]);

    FPC_ADD(x.val[0], y.val[0], x_re, x_im, v_re, v_im);
    FPC_SUB(x.val[1], y.val[1], x_re, x_im, v_re, v_im);

    vstore2(&f[0], x);
    vstore2(&f[4], y);
}

/*
 * Vectorized 3 layers of Forward FFT for 8 complex points (16 coefficients).
 */
static void PQCLEAN_FALCONPADDED1024_AARCH64_FFT_log4(fpr *f) {
    // Total SIMD register: 26 = 8 + 18
    float64x2x4_t t0, t1;                                          // 8
    float64x2x2_t x_re, x_im, y_re, y_im, v1, v2, tx, ty, s_re_im; // 18

    vloadx4(t0, &f[0]);
    vloadx4(t1, &f[8]);
    vload(s_re_im.val[0], &fpr_tab_log2[0]);

    vfmul(v1.val[0], t0.val[2], s_re_im.val[0]);
    vfmul(v1.val[1], t0.val[3], s_re_im.val[0]);

    vfmul(v2.val[0], t1.val[2], s_re_im.val[0]);
    vfmul(v2.val[1], t1.val[3], s_re_im.val[0]);

    vfsub(tx.val[0], v1.val[0], v2.val[0]);
    vfsub(tx.val[1], v1.val[1], v2.val[1]);

    vfadd(ty.val[0], v1.val[0], v2.val[0]);
    vfadd(ty.val[1], v1.val[1], v2.val[1]);

    FWD_BOT(t0.val[0], t1.val[0], t0.val[2], t1.val[2], tx.val[0], ty.val[0]);
    FWD_BOT(t0.val[1], t1.val[1], t0.val[3], t1.val[3], tx.val[1], ty.val[1]);

    vload(s_re_im.val[0], &fpr_tab_log3[0]);

    FWD_TOP_LANE(v1.val[0], v1.val[1], t0.val[1], t1.val[1], s_re_im.val[0]);
    FWD_TOP_LANE(v2.val[0], v2.val[1], t0.val[3], t1.val[3], s_re_im.val[0]);

    FWD_BOT(t0.val[0], t1.val[0], t0.val[1], t1.val[1], v1.val[0], v1.val[1]);
    FWD_BOTJ(t0.val[2], t1.val[2], t0.val[3], t1.val[3], v2.val[0], v2.val[1]);

    x_re.val[0] = t0.val[0];
    x_re.val[1] = t0.val[2];
    y_re.val[0] = t0.val[1];
    y_re.val[1] = t0.val[3];

    x_im.val[0] = t1.val[0];
    x_im.val[1] = t1.val[2];
    y_im.val[0] = t1.val[1];
    y_im.val[1] = t1.val[3];

    t0.val[0] = vzip1q_f64(x_re.val[0], x_re.val[1]);
    t0.val[1] = vzip2q_f64(x_re.val[0], x_re.val[1]);
    t0.val[2] = vzip1q_f64(y_re.val[0], y_re.val[1]);
    t0.val[3] = vzip2q_f64(y_re.val[0], y_re.val[1]);

    t1.val[0] = vzip1q_f64(x_im.val[0], x_im.val[1]);
    t1.val[1] = vzip2q_f64(x_im.val[0], x_im.val[1]);
    t1.val[2] = vzip1q_f64(y_im.val[0], y_im.val[1]);
    t1.val[3] = vzip2q_f64(y_im.val[0], y_im.val[1]);

    vload2(s_re_im, &fpr_tab_log4[0]);

    FWD_TOP(v1.val[0], v1.val[1], t0.val[1], t1.val[1], s_re_im.val[0], s_re_im.val[1]);
    FWD_TOP(v2.val[0], v2.val[1], t0.val[3], t1.val[3], s_re_im.val[0], s_re_im.val[1]);

    FWD_BOT(t0.val[0], t1.val[0], t0.val[1], t1.val[1], v1.val[0], v1.val[1]);
    FWD_BOTJ(t0.val[2], t1.val[2], t0.val[3], t1.val[3], v2.val[0], v2.val[1]);

    vstore4(&f[0], t0);
    vstore4(&f[8], t1);
}

/*
 * Vectorized 4 layers of Forward FFT for 16 complex points (32 coefficients).
 */
static void PQCLEAN_FALCONPADDED1024_AARCH64_FFT_log5(fpr *f, const unsigned logn) {
    // Total SIMD register: 34 = 2 + 32
    float64x2x2_t s_re_im;                                        // 2
    float64x2x4_t x_re, x_im, y_re, y_im, t_re, t_im, v_re, v_im; // 32

    const unsigned int falcon_n = 1 << logn;
    const unsigned int hn = falcon_n >> 1;

    unsigned int level = logn - 3;
    const fpr *fpr_tab2 = fpr_table[level++],
               *fpr_tab3 = fpr_table[level++],
                *fpr_tab4 = fpr_table[level++],
                 *fpr_tab5 = fpr_table[level];
    int k2 = 0, k3 = 0, k4 = 0, k5 = 0;

    for (unsigned j = 0; j < hn; j += 16) {
        vload(s_re_im.val[0], &fpr_tab2[k2]);

        /*
         * We only increase k2 when j value has the form j = 32*x + 16
         * Modulo 32 both sides, then check if (j % 32) == 16.
         */
        k2 += 2 * ((j & 31) == 16);

        vloadx4(y_re, &f[j + 8]);
        vloadx4(y_im, &f[j + 8 + hn]);

        if (logn == 5) {
            // Handle special case when use fpr_tab_log2, where re == im
            // This reduce number of multiplications,
            // although equal number of instructions as the "else" branch
            vfmulx4_i(t_im, y_im, s_re_im.val[0]);
            vfmulx4_i(t_re, y_re, s_re_im.val[0]);
            vfsubx4(v_re, t_re, t_im);
            vfaddx4(v_im, t_re, t_im);
        } else {
            FWD_TOP_LANEx4(v_re, v_im, y_re, y_im, s_re_im.val[0]);
        }

        vloadx4(x_re, &f[j]);
        vloadx4(x_im, &f[j + hn]);

        if ((j >> 4) & 1) {
            FWD_BOTJx4(x_re, x_im, y_re, y_im, v_re, v_im);
        } else {
            FWD_BOTx4(x_re, x_im, y_re, y_im, v_re, v_im);
        }

        vload(s_re_im.val[0], &fpr_tab3[k3]);
        k3 += 2;

        FWD_TOP_LANE(t_re.val[0], t_im.val[0], x_re.val[2], x_im.val[2], s_re_im.val[0]);
        FWD_TOP_LANE(t_re.val[1], t_im.val[1], x_re.val[3], x_im.val[3], s_re_im.val[0]);
        FWD_TOP_LANE(t_re.val[2], t_im.val[2], y_re.val[2], y_im.val[2], s_re_im.val[0]);
        FWD_TOP_LANE(t_re.val[3], t_im.val[3], y_re.val[3], y_im.val[3], s_re_im.val[0]);

        FWD_BOT(x_re.val[0], x_im.val[0], x_re.val[2], x_im.val[2], t_re.val[0], t_im.val[0]);
        FWD_BOT(x_re.val[1], x_im.val[1], x_re.val[3], x_im.val[3], t_re.val[1], t_im.val[1]);
        FWD_BOTJ(y_re.val[0], y_im.val[0], y_re.val[2], y_im.val[2], t_re.val[2], t_im.val[2]);
        FWD_BOTJ(y_re.val[1], y_im.val[1], y_re.val[3], y_im.val[3], t_re.val[3], t_im.val[3]);

        vloadx2(s_re_im, &fpr_tab4[k4]);
        k4 += 4;

        FWD_TOP_LANE(t_re.val[0], t_im.val[0], x_re.val[1], x_im.val[1], s_re_im.val[0]);
        FWD_TOP_LANE(t_re.val[1], t_im.val[1], x_re.val[3], x_im.val[3], s_re_im.val[0]);
        FWD_TOP_LANE(t_re.val[2], t_im.val[2], y_re.val[1], y_im.val[1], s_re_im.val[1]);
        FWD_TOP_LANE(t_re.val[3], t_im.val[3], y_re.val[3], y_im.val[3], s_re_im.val[1]);

        FWD_BOT(x_re.val[0], x_im.val[0], x_re.val[1], x_im.val[1], t_re.val[0], t_im.val[0]);
        FWD_BOTJ(x_re.val[2], x_im.val[2], x_re.val[3], x_im.val[3], t_re.val[1], t_im.val[1]);
        FWD_BOT(y_re.val[0], y_im.val[0], y_re.val[1], y_im.val[1], t_re.val[2], t_im.val[2]);
        FWD_BOTJ(y_re.val[2], y_im.val[2], y_re.val[3], y_im.val[3], t_re.val[3], t_im.val[3]);

        transpose_f64(x_re, x_re, v_re, 0, 2, 0);
        transpose_f64(x_re, x_re, v_re, 1, 3, 1);
        transpose_f64(x_im, x_im, v_im, 0, 2, 0);
        transpose_f64(x_im, x_im, v_im, 1, 3, 1);

        v_re.val[0] = x_re.val[2];
        x_re.val[2] = x_re.val[1];
        x_re.val[1] = v_re.val[0];

        v_im.val[0] = x_im.val[2];
        x_im.val[2] = x_im.val[1];
        x_im.val[1] = v_im.val[0];

        transpose_f64(y_re, y_re, v_re, 0, 2, 2);
        transpose_f64(y_re, y_re, v_re, 1, 3, 3);
        transpose_f64(y_im, y_im, v_im, 0, 2, 2);
        transpose_f64(y_im, y_im, v_im, 1, 3, 3);

        v_re.val[0] = y_re.val[2];
        y_re.val[2] = y_re.val[1];
        y_re.val[1] = v_re.val[0];

        v_im.val[0] = y_im.val[2];
        y_im.val[2] = y_im.val[1];
        y_im.val[1] = v_im.val[0];

        vload2(s_re_im, &fpr_tab5[k5]);
        k5 += 4;

        FWD_TOP(t_re.val[0], t_im.val[0], x_re.val[1], x_im.val[1], s_re_im.val[0], s_re_im.val[1]);
        FWD_TOP(t_re.val[1], t_im.val[1], x_re.val[3], x_im.val[3], s_re_im.val[0], s_re_im.val[1]);

        vload2(s_re_im, &fpr_tab5[k5]);
        k5 += 4;

        FWD_TOP(t_re.val[2], t_im.val[2], y_re.val[1], y_im.val[1], s_re_im.val[0], s_re_im.val[1]);
        FWD_TOP(t_re.val[3], t_im.val[3], y_re.val[3], y_im.val[3], s_re_im.val[0], s_re_im.val[1]);

        FWD_BOT(x_re.val[0], x_im.val[0], x_re.val[1], x_im.val[1], t_re.val[0], t_im.val[0]);
        FWD_BOTJ(x_re.val[2], x_im.val[2], x_re.val[3], x_im.val[3], t_re.val[1], t_im.val[1]);

        vstore4(&f[j], x_re);
        vstore4(&f[j + hn], x_im);

        FWD_BOT(y_re.val[0], y_im.val[0], y_re.val[1], y_im.val[1], t_re.val[2], t_im.val[2]);
        FWD_BOTJ(y_re.val[2], y_im.val[2], y_re.val[3], y_im.val[3], t_re.val[3], t_im.val[3]);

        vstore4(&f[j + 8], y_re);
        vstore4(&f[j + 8 + hn], y_im);
    }
}

/*
 * Vectorized 1 layer of Forward FFT for 16 complex points (32 coefficients).
 */
static void PQCLEAN_FALCONPADDED1024_AARCH64_FFT_logn1(fpr *f, const unsigned logn) {
    const unsigned n = 1 << logn;
    const unsigned hn = n >> 1;
    const unsigned ht = n >> 2;

    // Total SIMD register: 25 = 1 + 24
    float64x2_t s_re_im;                                          // 1
    float64x2x4_t a_re, a_im, b_re, b_im, t_re, t_im, v_re, v_im; // 24

    s_re_im = vld1q_dup_f64(&fpr_tab_log2[0]);
    for (unsigned j = 0; j < ht; j += 8) {
        vloadx4(b_re, &f[j + ht]);
        vfmulx4_i(t_re, b_re, s_re_im);

        vloadx4(b_im, &f[j + ht + hn]);
        vfmulx4_i(t_im, b_im, s_re_im);

        vfsubx4(v_re, t_re, t_im);
        vfaddx4(v_im, t_re, t_im);

        vloadx4(a_re, &f[j]);
        vloadx4(a_im, &f[j + hn]);

        FWD_BOTx4(a_re, a_im, b_re, b_im, v_re, v_im);
        vstorex4(&f[j + ht], b_re);
        vstorex4(&f[j], a_re);

        vstorex4(&f[j + ht + hn], b_im);
        vstorex4(&f[j + hn], a_im);
    }
}

/*
 * Vectorized 2 layers of Forward FFT for 16 complex points (32 coefficients).
 */
static void PQCLEAN_FALCONPADDED1024_AARCH64_FFT_logn2(fpr *f, const unsigned logn, const unsigned level) {
    const unsigned int falcon_n = 1 << logn;
    const unsigned int hn = falcon_n >> 1;

    // Total SIMD register: 26 = 8 + 16 + 2
    float64x2x4_t t_re, t_im;                   // 8
    float64x2x2_t x1_re, x2_re, x1_im, x2_im,
                  y1_re, y2_re, y1_im, y2_im;   // 16
    float64x2_t s1_re_im, s2_re_im;             // 2

    const fpr *fpr_tab1 = NULL, *fpr_tab2 = NULL;
    unsigned l, len, start, j, k1, k2;
    unsigned bar = logn - level + 2;

    for (l = level - 1; l > 4; l -= 2) {
        len = 1 << (l - 2);
        fpr_tab1 = fpr_table[bar++];
        fpr_tab2 = fpr_table[bar++];
        k1 = 0;
        k2 = 0;

        for (start = 0; start < hn; start += 1U << l) {
            vload(s1_re_im, &fpr_tab1[k1]);
            vload(s2_re_im, &fpr_tab2[k2]);
            k1 += 2U * ((start & 127) == 64);
            k2 += 2;

            for (j = start; j < start + len; j += 4) {

                vloadx2(y1_re, &f[j + 2 * len]);
                vloadx2(y1_im, &f[j + 2 * len + hn]);

                vloadx2(y2_re, &f[j + 3 * len]);
                vloadx2(y2_im, &f[j + 3 * len + hn]);

                FWD_TOP_LANE(t_re.val[0], t_im.val[0], y1_re.val[0], y1_im.val[0], s1_re_im);
                FWD_TOP_LANE(t_re.val[1], t_im.val[1], y1_re.val[1], y1_im.val[1], s1_re_im);
                FWD_TOP_LANE(t_re.val[2], t_im.val[2], y2_re.val[0], y2_im.val[0], s1_re_im);
                FWD_TOP_LANE(t_re.val[3], t_im.val[3], y2_re.val[1], y2_im.val[1], s1_re_im);

                vloadx2(x1_re, &f[j]);
                vloadx2(x1_im, &f[j + hn]);
                vloadx2(x2_re, &f[j + len]);
                vloadx2(x2_im, &f[j + len + hn]);

                FWD_BOT(x1_re.val[0], x1_im.val[0], y1_re.val[0], y1_im.val[0], t_re.val[0], t_im.val[0]);
                FWD_BOT(x1_re.val[1], x1_im.val[1], y1_re.val[1], y1_im.val[1], t_re.val[1], t_im.val[1]);
                FWD_BOT(x2_re.val[0], x2_im.val[0], y2_re.val[0], y2_im.val[0], t_re.val[2], t_im.val[2]);
                FWD_BOT(x2_re.val[1], x2_im.val[1], y2_re.val[1], y2_im.val[1], t_re.val[3], t_im.val[3]);

                FWD_TOP_LANE(t_re.val[0], t_im.val[0], x2_re.val[0], x2_im.val[0], s2_re_im);
                FWD_TOP_LANE(t_re.val[1], t_im.val[1], x2_re.val[1], x2_im.val[1], s2_re_im);
                FWD_TOP_LANE(t_re.val[2], t_im.val[2], y2_re.val[0], y2_im.val[0], s2_re_im);
                FWD_TOP_LANE(t_re.val[3], t_im.val[3], y2_re.val[1], y2_im.val[1], s2_re_im);

                FWD_BOT(x1_re.val[0], x1_im.val[0], x2_re.val[0], x2_im.val[0], t_re.val[0], t_im.val[0]);
                FWD_BOT(x1_re.val[1], x1_im.val[1], x2_re.val[1], x2_im.val[1], t_re.val[1], t_im.val[1]);

                vstorex2(&f[j], x1_re);
                vstorex2(&f[j + hn], x1_im);
                vstorex2(&f[j + len], x2_re);
                vstorex2(&f[j + len + hn], x2_im);

                FWD_BOTJ(y1_re.val[0], y1_im.val[0], y2_re.val[0], y2_im.val[0], t_re.val[2], t_im.val[2]);
                FWD_BOTJ(y1_re.val[1], y1_im.val[1], y2_re.val[1], y2_im.val[1], t_re.val[3], t_im.val[3]);

                vstorex2(&f[j + 2 * len], y1_re);
                vstorex2(&f[j + 2 * len + hn], y1_im);
                vstorex2(&f[j + 3 * len], y2_re);
                vstorex2(&f[j + 3 * len + hn], y2_im);
            }

            start += 1U << l;
            if (start >= hn) {
                break;
            }

            vload(s1_re_im, &fpr_tab1[k1]);
            vload(s2_re_im, &fpr_tab2[k2]);
            k1 += 2U * ((start & 127) == 64);
            k2 += 2;

            for (j = start; j < start + len; j += 4) {

                vloadx2(y1_re, &f[j + 2 * len]);
                vloadx2(y1_im, &f[j + 2 * len + hn]);

                vloadx2(y2_re, &f[j + 3 * len]);
                vloadx2(y2_im, &f[j + 3 * len + hn]);

                FWD_TOP_LANE(t_re.val[0], t_im.val[0], y1_re.val[0], y1_im.val[0], s1_re_im);
                FWD_TOP_LANE(t_re.val[1], t_im.val[1], y1_re.val[1], y1_im.val[1], s1_re_im);
                FWD_TOP_LANE(t_re.val[2], t_im.val[2], y2_re.val[0], y2_im.val[0], s1_re_im);
                FWD_TOP_LANE(t_re.val[3], t_im.val[3], y2_re.val[1], y2_im.val[1], s1_re_im);

                vloadx2(x1_re, &f[j]);
                vloadx2(x1_im, &f[j + hn]);
                vloadx2(x2_re, &f[j + len]);
                vloadx2(x2_im, &f[j + len + hn]);

                FWD_BOTJ(x1_re.val[0], x1_im.val[0], y1_re.val[0], y1_im.val[0], t_re.val[0], t_im.val[0]);
                FWD_BOTJ(x1_re.val[1], x1_im.val[1], y1_re.val[1], y1_im.val[1], t_re.val[1], t_im.val[1]);
                FWD_BOTJ(x2_re.val[0], x2_im.val[0], y2_re.val[0], y2_im.val[0], t_re.val[2], t_im.val[2]);
                FWD_BOTJ(x2_re.val[1], x2_im.val[1], y2_re.val[1], y2_im.val[1], t_re.val[3], t_im.val[3]);

                FWD_TOP_LANE(t_re.val[0], t_im.val[0], x2_re.val[0], x2_im.val[0], s2_re_im);
                FWD_TOP_LANE(t_re.val[1], t_im.val[1], x2_re.val[1], x2_im.val[1], s2_re_im);
                FWD_TOP_LANE(t_re.val[2], t_im.val[2], y2_re.val[0], y2_im.val[0], s2_re_im);
                FWD_TOP_LANE(t_re.val[3], t_im.val[3], y2_re.val[1], y2_im.val[1], s2_re_im);

                FWD_BOT(x1_re.val[0], x1_im.val[0], x2_re.val[0], x2_im.val[0], t_re.val[0], t_im.val[0]);
                FWD_BOT(x1_re.val[1], x1_im.val[1], x2_re.val[1], x2_im.val[1], t_re.val[1], t_im.val[1]);

                vstorex2(&f[j], x1_re);
                vstorex2(&f[j + hn], x1_im);
                vstorex2(&f[j + len], x2_re);
                vstorex2(&f[j + len + hn], x2_im);

                FWD_BOTJ(y1_re.val[0], y1_im.val[0], y2_re.val[0], y2_im.val[0], t_re.val[2], t_im.val[2]);
                FWD_BOTJ(y1_re.val[1], y1_im.val[1], y2_re.val[1], y2_im.val[1], t_re.val[3], t_im.val[3]);

                vstorex2(&f[j + 2 * len], y1_re);
                vstorex2(&f[j + 2 * len + hn], y1_im);
                vstorex2(&f[j + 3 * len], y2_re);
                vstorex2(&f[j + 3 * len + hn], y2_im);
            }
        }
    }
}

/*
 * 1 layer of Inverse FFT for 2 complex points (4 coefficients).
 * Note: The scalar version is faster than vectorized code.
 */
static void PQCLEAN_FALCONPADDED1024_AARCH64_iFFT_log2(fpr *f) {
    fpr x_re, x_im, y_re, y_im, s;
    x_re = f[0];
    y_re = f[1];
    x_im = f[2];
    y_im = f[3];
    s = fpr_tab_log2[0] * 0.5;

    f[0] = (x_re + y_re) * 0.5;
    f[2] = (x_im + y_im) * 0.5;

    x_re = (x_re - y_re) * s;
    x_im = (x_im - y_im) * s;

    f[1] = x_im + x_re;
    f[3] = x_im - x_re;
}

/*
 * Vectorized 2 layers of Inverse FFT for 4 complex point (8 coefficients).
 */
static void PQCLEAN_FALCONPADDED1024_AARCH64_iFFT_log3(fpr *f) {
    // Total SIMD registers: 12 = 4 + 8
    float64x2x4_t tmp;                          // 4
    float64x2x2_t x_re_im, y_re_im, v, s_re_im; // 8

    vload2(x_re_im, &f[0]);
    vload2(y_re_im, &f[4]);

    vfsub(v.val[0], x_re_im.val[0], x_re_im.val[1]);
    vfsub(v.val[1], y_re_im.val[0], y_re_im.val[1]);
    vfadd(x_re_im.val[0], x_re_im.val[0], x_re_im.val[1]);
    vfadd(x_re_im.val[1], y_re_im.val[0], y_re_im.val[1]);

    vload2(s_re_im, &fpr_tab_log3[0]);

    vfmul(y_re_im.val[0], v.val[1], s_re_im.val[1]);
    vfmla(y_re_im.val[0], y_re_im.val[0], v.val[0], s_re_im.val[0]);
    vfmul(y_re_im.val[1], v.val[1], s_re_im.val[0]);
    vfmls(y_re_im.val[1], y_re_im.val[1], v.val[0], s_re_im.val[1]);

    tmp.val[0] = vtrn1q_f64(x_re_im.val[0], y_re_im.val[0]);
    tmp.val[1] = vtrn2q_f64(x_re_im.val[0], y_re_im.val[0]);
    tmp.val[2] = vtrn1q_f64(x_re_im.val[1], y_re_im.val[1]);
    tmp.val[3] = vtrn2q_f64(x_re_im.val[1], y_re_im.val[1]);

    s_re_im.val[0] = vld1q_dup_f64(&fpr_tab_log2[0]);

    vfadd(x_re_im.val[0], tmp.val[0], tmp.val[1]);
    vfadd(x_re_im.val[1], tmp.val[2], tmp.val[3]);
    vfsub(v.val[0], tmp.val[0], tmp.val[1]);
    vfsub(v.val[1], tmp.val[2], tmp.val[3]);

    vfmuln(tmp.val[0], x_re_im.val[0], 0.25);
    vfmuln(tmp.val[2], x_re_im.val[1], 0.25);

    vfmuln(s_re_im.val[0], s_re_im.val[0], 0.25);

    vfmul(y_re_im.val[0], v.val[0], s_re_im.val[0]);
    vfmul(y_re_im.val[1], v.val[1], s_re_im.val[0]);

    vfadd(tmp.val[1], y_re_im.val[1], y_re_im.val[0]);
    vfsub(tmp.val[3], y_re_im.val[1], y_re_im.val[0]);

    vstorex4(&f[0], tmp);
}

/*
 * Vectorized 3 layers of Inverse FFT for 8 complex point (16 coefficients).
 */
static void PQCLEAN_FALCONPADDED1024_AARCH64_iFFT_log4(fpr *f) {
    // Total SIMD registers: 18 = 12 + 6
    float64x2x4_t re, im, t;           // 12
    float64x2x2_t t_re, t_im, s_re_im; // 6

    vload4(re, &f[0]);
    vload4(im, &f[8]);

    INV_TOPJ(t_re.val[0], t_im.val[0], re.val[0], im.val[0], re.val[1], im.val[1]);
    INV_TOPJm(t_re.val[1], t_im.val[1], re.val[2], im.val[2], re.val[3], im.val[3]);

    vload2(s_re_im, &fpr_tab_log4[0]);

    INV_BOTJ(re.val[1], im.val[1], t_re.val[0], t_im.val[0], s_re_im.val[0], s_re_im.val[1]);
    INV_BOTJm(re.val[3], im.val[3], t_re.val[1], t_im.val[1], s_re_im.val[0], s_re_im.val[1]);

    // re: 0, 4 | 1, 5 | 2, 6 | 3, 7
    // im: 8, 12| 9, 13|10, 14|11, 15
    transpose_f64(re, re, t, 0, 1, 0);
    transpose_f64(re, re, t, 2, 3, 1);
    transpose_f64(im, im, t, 0, 1, 2);
    transpose_f64(im, im, t, 2, 3, 3);

    // re: 0, 1 | 4,  5 | 2, 3 | 6, 7
    // im: 8, 9 | 12, 13|10, 11| 14, 15
    t.val[0] = re.val[1];
    re.val[1] = re.val[2];
    re.val[2] = t.val[0];

    t.val[1] = im.val[1];
    im.val[1] = im.val[2];
    im.val[2] = t.val[1];

    // re: 0, 1 |  2,  3| 4,  5 | 6, 7
    // im: 8, 9 | 10, 11| 12, 13| 14, 15
    INV_TOPJ(t_re.val[0], t_im.val[0], re.val[0], im.val[0], re.val[1], im.val[1]);
    INV_TOPJm(t_re.val[1], t_im.val[1], re.val[2], im.val[2], re.val[3], im.val[3]);

    vload(s_re_im.val[0], &fpr_tab_log3[0]);

    INV_BOTJ_LANE(re.val[1], im.val[1], t_re.val[0], t_im.val[0], s_re_im.val[0]);
    INV_BOTJm_LANE(re.val[3], im.val[3], t_re.val[1], t_im.val[1], s_re_im.val[0]);

    INV_TOPJ(t_re.val[0], t_im.val[0], re.val[0], im.val[0], re.val[2], im.val[2]);
    INV_TOPJ(t_re.val[1], t_im.val[1], re.val[1], im.val[1], re.val[3], im.val[3]);

    vfmuln(re.val[0], re.val[0], 0.12500000000);
    vfmuln(re.val[1], re.val[1], 0.12500000000);
    vfmuln(im.val[0], im.val[0], 0.12500000000);
    vfmuln(im.val[1], im.val[1], 0.12500000000);

    s_re_im.val[0] = vld1q_dup_f64(&fpr_tab_log2[0]);

    vfmuln(s_re_im.val[0], s_re_im.val[0], 0.12500000000);

    vfmul(t_re.val[0], t_re.val[0], s_re_im.val[0]);
    vfmul(t_re.val[1], t_re.val[1], s_re_im.val[0]);
    vfmul(t_im.val[0], t_im.val[0], s_re_im.val[0]);
    vfmul(t_im.val[1], t_im.val[1], s_re_im.val[0]);

    vfsub(im.val[2], t_im.val[0], t_re.val[0]);
    vfsub(im.val[3], t_im.val[1], t_re.val[1]);
    vfadd(re.val[2], t_im.val[0], t_re.val[0]);
    vfadd(re.val[3], t_im.val[1], t_re.val[1]);

    vstorex4(&f[0], re);
    vstorex4(&f[8], im);
}

/*
 * Vectorized 4 layers of Inverse FFT for 16 complex point (32 coefficients).
 */
static void PQCLEAN_FALCONPADDED1024_AARCH64_iFFT_log5(fpr *f, const unsigned logn, const unsigned last) {
    // Total SIMD register: 26 = 24 + 2
    float64x2x4_t x_re, x_im, y_re, y_im, t_re, t_im; // 24
    float64x2x2_t s_re_im;                            // 2
    const unsigned n = 1 << logn;
    const unsigned hn = n >> 1;

    unsigned int level = logn;
    const fpr *fpr_tab5 = fpr_table[level--],
               *fpr_tab4 = fpr_table[level--],
                *fpr_tab3 = fpr_table[level--],
                 *fpr_tab2 = fpr_table[level];
    int k2 = 0, k3 = 0, k4 = 0, k5 = 0;

    for (unsigned j = 0; j < hn; j += 16) {

        vload4(x_re, &f[j]);
        vload4(x_im, &f[j + hn]);

        INV_TOPJ(t_re.val[0], t_im.val[0], x_re.val[0], x_im.val[0], x_re.val[1], x_im.val[1]);
        INV_TOPJm(t_re.val[2], t_im.val[2], x_re.val[2], x_im.val[2], x_re.val[3], x_im.val[3]);

        vload4(y_re, &f[j + 8]);
        vload4(y_im, &f[j + 8 + hn]);

        INV_TOPJ(t_re.val[1], t_im.val[1], y_re.val[0], y_im.val[0], y_re.val[1], y_im.val[1]);
        INV_TOPJm(t_re.val[3], t_im.val[3], y_re.val[2], y_im.val[2], y_re.val[3], y_im.val[3]);

        vload2(s_re_im, &fpr_tab5[k5]);
        k5 += 4;

        INV_BOTJ(x_re.val[1], x_im.val[1], t_re.val[0], t_im.val[0], s_re_im.val[0], s_re_im.val[1]);
        INV_BOTJm(x_re.val[3], x_im.val[3], t_re.val[2], t_im.val[2], s_re_im.val[0], s_re_im.val[1]);

        vload2(s_re_im, &fpr_tab5[k5]);
        k5 += 4;

        INV_BOTJ(y_re.val[1], y_im.val[1], t_re.val[1], t_im.val[1], s_re_im.val[0], s_re_im.val[1]);
        INV_BOTJm(y_re.val[3], y_im.val[3], t_re.val[3], t_im.val[3], s_re_im.val[0], s_re_im.val[1]);

        transpose_f64(x_re, x_re, t_re, 0, 1, 0);
        transpose_f64(x_re, x_re, t_re, 2, 3, 1);
        transpose_f64(y_re, y_re, t_re, 0, 1, 2);
        transpose_f64(y_re, y_re, t_re, 2, 3, 3);

        transpose_f64(x_im, x_im, t_im, 0, 1, 0);
        transpose_f64(x_im, x_im, t_im, 2, 3, 1);
        transpose_f64(y_im, y_im, t_im, 0, 1, 2);
        transpose_f64(y_im, y_im, t_im, 2, 3, 3);

        t_re.val[0] = x_re.val[1];
        x_re.val[1] = x_re.val[2];
        x_re.val[2] = t_re.val[0];

        t_re.val[1] = y_re.val[1];
        y_re.val[1] = y_re.val[2];
        y_re.val[2] = t_re.val[1];

        t_im.val[0] = x_im.val[1];
        x_im.val[1] = x_im.val[2];
        x_im.val[2] = t_im.val[0];

        t_im.val[1] = y_im.val[1];
        y_im.val[1] = y_im.val[2];
        y_im.val[2] = t_im.val[1];

        INV_TOPJ(t_re.val[0], t_im.val[0], x_re.val[0], x_im.val[0], x_re.val[1], x_im.val[1]);
        INV_TOPJm(t_re.val[1], t_im.val[1], x_re.val[2], x_im.val[2], x_re.val[3], x_im.val[3]);

        INV_TOPJ(t_re.val[2], t_im.val[2], y_re.val[0], y_im.val[0], y_re.val[1], y_im.val[1]);
        INV_TOPJm(t_re.val[3], t_im.val[3], y_re.val[2], y_im.val[2], y_re.val[3], y_im.val[3]);

        vloadx2(s_re_im, &fpr_tab4[k4]);
        k4 += 4;

        INV_BOTJ_LANE(x_re.val[1], x_im.val[1], t_re.val[0], t_im.val[0], s_re_im.val[0]);
        INV_BOTJm_LANE(x_re.val[3], x_im.val[3], t_re.val[1], t_im.val[1], s_re_im.val[0]);

        INV_BOTJ_LANE(y_re.val[1], y_im.val[1], t_re.val[2], t_im.val[2], s_re_im.val[1]);
        INV_BOTJm_LANE(y_re.val[3], y_im.val[3], t_re.val[3], t_im.val[3], s_re_im.val[1]);

        INV_TOPJ(t_re.val[0], t_im.val[0], x_re.val[0], x_im.val[0], x_re.val[2], x_im.val[2]);
        INV_TOPJ(t_re.val[1], t_im.val[1], x_re.val[1], x_im.val[1], x_re.val[3], x_im.val[3]);

        INV_TOPJm(t_re.val[2], t_im.val[2], y_re.val[0], y_im.val[0], y_re.val[2], y_im.val[2]);
        INV_TOPJm(t_re.val[3], t_im.val[3], y_re.val[1], y_im.val[1], y_re.val[3], y_im.val[3]);

        vload(s_re_im.val[0], &fpr_tab3[k3]);
        k3 += 2;

        INV_BOTJ_LANE(x_re.val[2], x_im.val[2], t_re.val[0], t_im.val[0], s_re_im.val[0]);
        INV_BOTJ_LANE(x_re.val[3], x_im.val[3], t_re.val[1], t_im.val[1], s_re_im.val[0]);

        INV_BOTJm_LANE(y_re.val[2], y_im.val[2], t_re.val[2], t_im.val[2], s_re_im.val[0]);
        INV_BOTJm_LANE(y_re.val[3], y_im.val[3], t_re.val[3], t_im.val[3], s_re_im.val[0]);

        if ((j >> 4) & 1) {
            INV_TOPJmx4(t_re, t_im, x_re, x_im, y_re, y_im);
        } else {
            INV_TOPJx4(t_re, t_im, x_re, x_im, y_re, y_im);
        }

        vload(s_re_im.val[0], &fpr_tab2[k2]);
        k2 += 2 * ((j & 31) == 16);

        if (last) {
            vfmuln(s_re_im.val[0], s_re_im.val[0], fpr_p2_tab[logn]);
            vfmulnx4(x_re, x_re, fpr_p2_tab[logn]);
            vfmulnx4(x_im, x_im, fpr_p2_tab[logn]);
        }
        vstorex4(&f[j], x_re);
        vstorex4(&f[j + hn], x_im);

        if (logn == 5) {
            // Special case in fpr_tab_log2 where re == im
            vfmulx4_i(t_re, t_re, s_re_im.val[0]);
            vfmulx4_i(t_im, t_im, s_re_im.val[0]);

            vfaddx4(y_re, t_im, t_re);
            vfsubx4(y_im, t_im, t_re);
        } else {
            if ((j >> 4) & 1) {
                INV_BOTJm_LANEx4(y_re, y_im, t_re, t_im, s_re_im.val[0]);
            } else {
                INV_BOTJ_LANEx4(y_re, y_im, t_re, t_im, s_re_im.val[0]);
            }
        }

        vstorex4(&f[j + 8], y_re);
        vstorex4(&f[j + 8 + hn], y_im);
    }
}

/*
 * Vectorized 1 layer of Inverse FFT for 16 complex points (32 coefficients).
 */
static void PQCLEAN_FALCONPADDED1024_AARCH64_iFFT_logn1(fpr *f, const unsigned logn, const unsigned last) {
    // Total SIMD register 26 = 24 + 2
    float64x2x4_t a_re, a_im, b_re, b_im, t_re, t_im; // 24
    float64x2_t s_re_im;                              // 2

    const unsigned n = 1 << logn;
    const unsigned hn = n >> 1;
    const unsigned ht = n >> 2;

    for (unsigned j = 0; j < ht; j += 8) {
        vloadx4(a_re, &f[j]);
        vloadx4(a_im, &f[j + hn]);
        vloadx4(b_re, &f[j + ht]);
        vloadx4(b_im, &f[j + ht + hn]);

        INV_TOPJx4(t_re, t_im, a_re, a_im, b_re, b_im);

        s_re_im = vld1q_dup_f64(&fpr_tab_log2[0]);

        if (last) {
            vfmuln(s_re_im, s_re_im, fpr_p2_tab[logn]);
            vfmulnx4(a_re, a_re, fpr_p2_tab[logn]);
            vfmulnx4(a_im, a_im, fpr_p2_tab[logn]);
        }

        vstorex4(&f[j], a_re);
        vstorex4(&f[j + hn], a_im);

        vfmulx4_i(t_re, t_re, s_re_im);
        vfmulx4_i(t_im, t_im, s_re_im);

        vfaddx4(b_re, t_im, t_re);
        vfsubx4(b_im, t_im, t_re);

        vstorex4(&f[j + ht], b_re);
        vstorex4(&f[j + ht + hn], b_im);
    }
}

/*
 * Vectorized 2 layers of Inverse FFT for 16 complex points (32 coefficients).
 */
static void PQCLEAN_FALCONPADDED1024_AARCH64_iFFT_logn2(fpr *f, const unsigned logn, const unsigned level, unsigned last) {
    const unsigned int falcon_n = 1 << logn;
    const unsigned int hn = falcon_n >> 1;

    // Total SIMD register: 26 = 16 + 8 + 2
    float64x2x4_t t_re, t_im;                   // 8
    float64x2x2_t x1_re, x2_re, x1_im, x2_im,
                  y1_re, y2_re, y1_im, y2_im;   // 16
    float64x2_t s1_re_im, s2_re_im;             // 2

    const fpr *fpr_inv_tab1 = NULL, *fpr_inv_tab2 = NULL;
    unsigned l, len, start, j, k1, k2;
    unsigned bar = logn - 4;

    for (l = 4; l < logn - level - 1; l += 2) {
        len = 1 << l;
        last -= 1;
        fpr_inv_tab1 = fpr_table[bar--];
        fpr_inv_tab2 = fpr_table[bar--];
        k1 = 0;
        k2 = 0;

        for (start = 0; start < hn; start += 1U << (l + 2)) {
            vload(s1_re_im, &fpr_inv_tab1[k1]);
            vload(s2_re_im, &fpr_inv_tab2[k2]);
            k1 += 2;
            k2 += 2U * ((start & 127) == 64);
            if (!last) {
                vfmuln(s2_re_im, s2_re_im, fpr_p2_tab[logn]);
            }
            for (j = start; j < start + len; j += 4) {

                vloadx2(x1_re, &f[j]);
                vloadx2(x1_im, &f[j + hn]);
                vloadx2(y1_re, &f[j + len]);
                vloadx2(y1_im, &f[j + len + hn]);

                INV_TOPJ(t_re.val[0], t_im.val[0], x1_re.val[0], x1_im.val[0], y1_re.val[0], y1_im.val[0]);
                INV_TOPJ(t_re.val[1], t_im.val[1], x1_re.val[1], x1_im.val[1], y1_re.val[1], y1_im.val[1]);

                vloadx2(x2_re, &f[j + 2 * len]);
                vloadx2(x2_im, &f[j + 2 * len + hn]);
                vloadx2(y2_re, &f[j + 3 * len]);
                vloadx2(y2_im, &f[j + 3 * len + hn]);

                INV_TOPJm(t_re.val[2], t_im.val[2], x2_re.val[0], x2_im.val[0], y2_re.val[0], y2_im.val[0]);
                INV_TOPJm(t_re.val[3], t_im.val[3], x2_re.val[1], x2_im.val[1], y2_re.val[1], y2_im.val[1]);

                INV_BOTJ_LANE(y1_re.val[0], y1_im.val[0], t_re.val[0], t_im.val[0], s1_re_im);
                INV_BOTJ_LANE(y1_re.val[1], y1_im.val[1], t_re.val[1], t_im.val[1], s1_re_im);

                INV_BOTJm_LANE(y2_re.val[0], y2_im.val[0], t_re.val[2], t_im.val[2], s1_re_im);
                INV_BOTJm_LANE(y2_re.val[1], y2_im.val[1], t_re.val[3], t_im.val[3], s1_re_im);

                INV_TOPJ(t_re.val[0], t_im.val[0], x1_re.val[0], x1_im.val[0], x2_re.val[0], x2_im.val[0]);
                INV_TOPJ(t_re.val[1], t_im.val[1], x1_re.val[1], x1_im.val[1], x2_re.val[1], x2_im.val[1]);

                INV_TOPJ(t_re.val[2], t_im.val[2], y1_re.val[0], y1_im.val[0], y2_re.val[0], y2_im.val[0]);
                INV_TOPJ(t_re.val[3], t_im.val[3], y1_re.val[1], y1_im.val[1], y2_re.val[1], y2_im.val[1]);

                INV_BOTJ_LANE(x2_re.val[0], x2_im.val[0], t_re.val[0], t_im.val[0], s2_re_im);
                INV_BOTJ_LANE(x2_re.val[1], x2_im.val[1], t_re.val[1], t_im.val[1], s2_re_im);
                INV_BOTJ_LANE(y2_re.val[0], y2_im.val[0], t_re.val[2], t_im.val[2], s2_re_im);
                INV_BOTJ_LANE(y2_re.val[1], y2_im.val[1], t_re.val[3], t_im.val[3], s2_re_im);

                vstorex2(&f[j + 2 * len], x2_re);
                vstorex2(&f[j + 2 * len + hn], x2_im);

                vstorex2(&f[j + 3 * len], y2_re);
                vstorex2(&f[j + 3 * len + hn], y2_im);

                if (!last) {
                    vfmuln(x1_re.val[0], x1_re.val[0], fpr_p2_tab[logn]);
                    vfmuln(x1_re.val[1], x1_re.val[1], fpr_p2_tab[logn]);
                    vfmuln(x1_im.val[0], x1_im.val[0], fpr_p2_tab[logn]);
                    vfmuln(x1_im.val[1], x1_im.val[1], fpr_p2_tab[logn]);

                    vfmuln(y1_re.val[0], y1_re.val[0], fpr_p2_tab[logn]);
                    vfmuln(y1_re.val[1], y1_re.val[1], fpr_p2_tab[logn]);
                    vfmuln(y1_im.val[0], y1_im.val[0], fpr_p2_tab[logn]);
                    vfmuln(y1_im.val[1], y1_im.val[1], fpr_p2_tab[logn]);
                }

                vstorex2(&f[j], x1_re);
                vstorex2(&f[j + hn], x1_im);

                vstorex2(&f[j + len], y1_re);
                vstorex2(&f[j + len + hn], y1_im);
            }

            start += 1U << (l + 2);
            if (start >= hn) {
                break;
            }

            vload(s1_re_im, &fpr_inv_tab1[k1]);
            vload(s2_re_im, &fpr_inv_tab2[k2]);
            k1 += 2;
            k2 += 2U * ((start & 127) == 64);
            if (!last) {
                vfmuln(s2_re_im, s2_re_im, fpr_p2_tab[logn]);
            }

            for (j = start; j < start + len; j += 4) {

                vloadx2(x1_re, &f[j]);
                vloadx2(x1_im, &f[j + hn]);
                vloadx2(y1_re, &f[j + len]);
                vloadx2(y1_im, &f[j + len + hn]);

                INV_TOPJ(t_re.val[0], t_im.val[0], x1_re.val[0], x1_im.val[0], y1_re.val[0], y1_im.val[0]);
                INV_TOPJ(t_re.val[1], t_im.val[1], x1_re.val[1], x1_im.val[1], y1_re.val[1], y1_im.val[1]);

                vloadx2(x2_re, &f[j + 2 * len]);
                vloadx2(x2_im, &f[j + 2 * len + hn]);
                vloadx2(y2_re, &f[j + 3 * len]);
                vloadx2(y2_im, &f[j + 3 * len + hn]);

                INV_TOPJm(t_re.val[2], t_im.val[2], x2_re.val[0], x2_im.val[0], y2_re.val[0], y2_im.val[0]);
                INV_TOPJm(t_re.val[3], t_im.val[3], x2_re.val[1], x2_im.val[1], y2_re.val[1], y2_im.val[1]);

                INV_BOTJ_LANE(y1_re.val[0], y1_im.val[0], t_re.val[0], t_im.val[0], s1_re_im);
                INV_BOTJ_LANE(y1_re.val[1], y1_im.val[1], t_re.val[1], t_im.val[1], s1_re_im);

                INV_BOTJm_LANE(y2_re.val[0], y2_im.val[0], t_re.val[2], t_im.val[2], s1_re_im);
                INV_BOTJm_LANE(y2_re.val[1], y2_im.val[1], t_re.val[3], t_im.val[3], s1_re_im);

                INV_TOPJm(t_re.val[0], t_im.val[0], x1_re.val[0], x1_im.val[0], x2_re.val[0], x2_im.val[0]);
                INV_TOPJm(t_re.val[1], t_im.val[1], x1_re.val[1], x1_im.val[1], x2_re.val[1], x2_im.val[1]);

                INV_TOPJm(t_re.val[2], t_im.val[2], y1_re.val[0], y1_im.val[0], y2_re.val[0], y2_im.val[0]);
                INV_TOPJm(t_re.val[3], t_im.val[3], y1_re.val[1], y1_im.val[1], y2_re.val[1], y2_im.val[1]);

                INV_BOTJm_LANE(x2_re.val[0], x2_im.val[0], t_re.val[0], t_im.val[0], s2_re_im);
                INV_BOTJm_LANE(x2_re.val[1], x2_im.val[1], t_re.val[1], t_im.val[1], s2_re_im);
                INV_BOTJm_LANE(y2_re.val[0], y2_im.val[0], t_re.val[2], t_im.val[2], s2_re_im);
                INV_BOTJm_LANE(y2_re.val[1], y2_im.val[1], t_re.val[3], t_im.val[3], s2_re_im);

                vstorex2(&f[j + 2 * len], x2_re);
                vstorex2(&f[j + 2 * len + hn], x2_im);

                vstorex2(&f[j + 3 * len], y2_re);
                vstorex2(&f[j + 3 * len + hn], y2_im);

                if (!last) {
                    vfmuln(x1_re.val[0], x1_re.val[0], fpr_p2_tab[logn]);
                    vfmuln(x1_re.val[1], x1_re.val[1], fpr_p2_tab[logn]);
                    vfmuln(x1_im.val[0], x1_im.val[0], fpr_p2_tab[logn]);
                    vfmuln(x1_im.val[1], x1_im.val[1], fpr_p2_tab[logn]);

                    vfmuln(y1_re.val[0], y1_re.val[0], fpr_p2_tab[logn]);
                    vfmuln(y1_re.val[1], y1_re.val[1], fpr_p2_tab[logn]);
                    vfmuln(y1_im.val[0], y1_im.val[0], fpr_p2_tab[logn]);
                    vfmuln(y1_im.val[1], y1_im.val[1], fpr_p2_tab[logn]);
                }

                vstorex2(&f[j], x1_re);
                vstorex2(&f[j + hn], x1_im);

                vstorex2(&f[j + len], y1_re);
                vstorex2(&f[j + len + hn], y1_im);
            }
        }
    }
}

/*
 * Scalable vectorized Forward FFT implementation.
 * Support logn from [1, 10]
 * Can be easily extended to logn > 10
 */
void PQCLEAN_FALCONPADDED1024_AARCH64_FFT(fpr *f, const unsigned logn) {
    unsigned level = logn;
    switch (logn) {
    case 2:
        PQCLEAN_FALCONPADDED1024_AARCH64_FFT_log2(f);
        break;

    case 3:
        PQCLEAN_FALCONPADDED1024_AARCH64_FFT_log3(f);
        break;

    case 4:
        PQCLEAN_FALCONPADDED1024_AARCH64_FFT_log4(f);
        break;

    case 5:
        PQCLEAN_FALCONPADDED1024_AARCH64_FFT_log5(f, 5);
        break;

    case 6:
        PQCLEAN_FALCONPADDED1024_AARCH64_FFT_logn1(f, logn);
        PQCLEAN_FALCONPADDED1024_AARCH64_FFT_log5(f, logn);
        break;

    case 7:
    case 9:
        PQCLEAN_FALCONPADDED1024_AARCH64_FFT_logn2(f, logn, level);
        PQCLEAN_FALCONPADDED1024_AARCH64_FFT_log5(f, logn);
        break;

    case 8:
    case 10:
        PQCLEAN_FALCONPADDED1024_AARCH64_FFT_logn1(f, logn);
        PQCLEAN_FALCONPADDED1024_AARCH64_FFT_logn2(f, logn, level - 1);
        PQCLEAN_FALCONPADDED1024_AARCH64_FFT_log5(f, logn);
        break;

    default:
        break;
    }
}

/*
 * Scalable vectorized Inverse FFT implementation.
 * Support logn from [1, 10]
 * Can be easily extended to logn > 10
 */
void PQCLEAN_FALCONPADDED1024_AARCH64_iFFT(fpr *f, const unsigned logn) {
    const unsigned level = (logn - 5) & 1;

    switch (logn) {
    case 2:
        PQCLEAN_FALCONPADDED1024_AARCH64_iFFT_log2(f);
        break;

    case 3:
        PQCLEAN_FALCONPADDED1024_AARCH64_iFFT_log3(f);
        break;

    case 4:
        PQCLEAN_FALCONPADDED1024_AARCH64_iFFT_log4(f);
        break;

    case 5:
        PQCLEAN_FALCONPADDED1024_AARCH64_iFFT_log5(f, 5, 1);
        break;

    case 6:
        PQCLEAN_FALCONPADDED1024_AARCH64_iFFT_log5(f, logn, 0);
        PQCLEAN_FALCONPADDED1024_AARCH64_iFFT_logn1(f, logn, 1);
        break;

    case 7:
    case 9:
        PQCLEAN_FALCONPADDED1024_AARCH64_iFFT_log5(f, logn, 0);
        PQCLEAN_FALCONPADDED1024_AARCH64_iFFT_logn2(f, logn, level, 1);
        break;

    case 8:
    case 10:
        PQCLEAN_FALCONPADDED1024_AARCH64_iFFT_log5(f, logn, 0);
        PQCLEAN_FALCONPADDED1024_AARCH64_iFFT_logn2(f, logn, level, 0);
        PQCLEAN_FALCONPADDED1024_AARCH64_iFFT_logn1(f, logn, 1);
        break;

    default:
        break;
    }
}
