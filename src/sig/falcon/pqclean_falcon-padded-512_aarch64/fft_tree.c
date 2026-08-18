/*
 * High-speed vectorize FFT tree for arbitrary `logn`.
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
 * 1 layer of Merge FFT for 2 complex points (4 coefficients).
 */
static inline void PQCLEAN_FALCONPADDED512_AARCH64_poly_mergeFFT_log2(fpr *f, const fpr *f0, const fpr *f1) {
    fpr a_re, a_im, b_re, b_im, d_re, d_im, s;
    a_re = f0[0];
    a_im = f0[1];
    s = fpr_tab_log2[0];
    b_re = f1[0] * s;
    b_im = f1[1] * s;

    d_re = b_re - b_im;
    d_im = b_re + b_im;

    f[0] = a_re + d_re;
    f[2] = a_im + d_im;
    f[1] = a_re - d_re;
    f[3] = a_im - d_im;
}

/*
 * Vectorized 1 layer of Merge FFT for 4 complex points (8 coefficients).
 */
static inline void PQCLEAN_FALCONPADDED512_AARCH64_poly_mergeFFT_log3(fpr *f, const fpr *f0, const fpr *f1) {
    // Total SIMD registers: 12 = 10 + 2
    float64x2x2_t g1, g0, g_re, g_im, s_re_im; // 10
    float64x2_t t_re, t_im;                    // 2

    vloadx2(g1, &f1[0]);

    vload2(s_re_im, &fpr_tab_log3[0]);

    FWD_TOP(t_re, t_im, g1.val[0], g1.val[1], s_re_im.val[0], s_re_im.val[1]);

    vloadx2(g0, &f0[0]);

    FPC_ADD(g_re.val[0], g_im.val[0], g0.val[0], g0.val[1], t_re, t_im);
    FPC_SUB(g_re.val[1], g_im.val[1], g0.val[0], g0.val[1], t_re, t_im);

    vstore2(&f[0], g_re);
    vstore2(&f[4], g_im);
}

/*
 * Vectorized 1 layer of Merge FFT for 8 complex points (16 coefficients).
 */
static inline void PQCLEAN_FALCONPADDED512_AARCH64_poly_mergeFFT_log4(fpr *f, const fpr *f0, const fpr *f1, const unsigned logn) {
    const unsigned n = 1 << logn;
    const unsigned ht = n >> 2;
    const fpr *fpr_merge = fpr_table[logn];

    // Total SIMD register 22 = 14 + 8
    float64x2x2_t g1_re, g1_im, g0_re, g0_im, s_re_im, t_re, t_im; // 14
    float64x2x4_t g_re, g_im;                                      // 8

    for (unsigned j = 0; j < ht; j += 4) {
        vload2(g1_re, &f1[j]);
        vload2(g1_im, &f1[j + ht]);

        vload2(s_re_im, &fpr_merge[j]);

        FWD_TOP(t_re.val[0], t_im.val[0], g1_re.val[0], g1_im.val[0], s_re_im.val[0], s_re_im.val[1]);
        vload2(g0_re, &f0[j]);

        FWD_TOP(t_re.val[1], t_im.val[1], g1_re.val[1], g1_im.val[1], s_re_im.val[0], s_re_im.val[1]);
        vload2(g0_im, &f0[j + ht]);

        FPC_ADD(g_re.val[0], g_im.val[0], g0_re.val[0], g0_im.val[0], t_re.val[0], t_im.val[0]);
        FPC_SUB(g_re.val[1], g_im.val[1], g0_re.val[0], g0_im.val[0], t_re.val[0], t_im.val[0]);
        FPC_ADDJ(g_re.val[2], g_im.val[2], g0_re.val[1], g0_im.val[1], t_re.val[1], t_im.val[1]);
        FPC_SUBJ(g_re.val[3], g_im.val[3], g0_re.val[1], g0_im.val[1], t_re.val[1], t_im.val[1]);

        vstore4(&f[j << 1], g_re);
        vstore4(&f[(j + ht) << 1], g_im);
    }
}

/*
 * 1 layer of Split FFT for 2 complex points (4 coefficients).
 */
static void
PQCLEAN_FALCONPADDED512_AARCH64_poly_splitFFT_log2(fpr *restrict f0, fpr *restrict f1, const fpr *restrict f) {
    fpr a_re, a_im, b_re, b_im, d_re, d_im, s;
    a_re = f[0];
    b_re = f[1];
    a_im = f[2];
    b_im = f[3];
    s = fpr_tab_log2[0] * 0.5;

    f0[0] = (a_re + b_re) * 0.5;
    f0[1] = (a_im + b_im) * 0.5;

    d_re = (a_re - b_re) * s;
    d_im = (a_im - b_im) * s;

    f1[0] = d_im + d_re;
    f1[1] = d_im - d_re;
}

/*
 * Vectorized 1 layer of Split FFT for 4 complex points (8 coefficients).
 */
static inline void PQCLEAN_FALCONPADDED512_AARCH64_poly_splitFFT_log3(fpr *f0, fpr *f1, const fpr *f) {
    // Total SIMD registers: 12
    float64x2x2_t re, im, g0, g1, s_re_im, tm; // 12

    vload2(re, &f[0]);
    vload2(im, &f[4]);

    FPC_ADD(g0.val[0], g0.val[1], re.val[0], im.val[0], re.val[1], im.val[1]);
    FPC_SUB(tm.val[0], tm.val[1], re.val[0], im.val[0], re.val[1], im.val[1]);
    vload2(s_re_im, &fpr_tab_log3[0]);

    vfmuln(g0.val[0], g0.val[0], 0.5);
    vfmuln(g0.val[1], g0.val[1], 0.5);
    vstorex2(&f0[0], g0);

    vfmuln(s_re_im.val[0], s_re_im.val[0], 0.5);
    vfmuln(s_re_im.val[1], s_re_im.val[1], 0.5);

    INV_BOTJ(g1.val[0], g1.val[1], tm.val[0], tm.val[1], s_re_im.val[0], s_re_im.val[1]);

    vstorex2(&f1[0], g1);
}

/*
 * Vectorized 1 layer of Split FFT for 8 complex points (16 coefficients).
 */
static inline void PQCLEAN_FALCONPADDED512_AARCH64_poly_splitFFT_log4(fpr *f0, fpr *f1, const fpr *f, const unsigned logn) {
    const unsigned n = 1 << logn;
    const unsigned hn = n >> 1;
    const unsigned ht = n >> 2;
    const fpr *fpr_split = fpr_table[logn];

    // Total SIMD register 23 = 1 + 8 + 14
    float64x2_t half;                                              // 1
    float64x2x4_t g_re, g_im;                                      // 8
    float64x2x2_t s_re_im, t_re, t_im, g1_re, g1_im, g0_re, g0_im; // 14

    half = vdupq_n_f64(0.5);
    for (unsigned j = 0; j < ht; j += 4) {
        unsigned j2 = j << 1;
        vload4(g_re, &f[j2]);
        vload4(g_im, &f[j2 + hn]);

        FPC_ADD(g0_re.val[0], g0_im.val[0], g_re.val[0], g_im.val[0], g_re.val[1], g_im.val[1]);
        FPC_ADD(g0_re.val[1], g0_im.val[1], g_re.val[2], g_im.val[2], g_re.val[3], g_im.val[3]);

        FPC_SUB(t_re.val[0], t_im.val[0], g_re.val[0], g_im.val[0], g_re.val[1], g_im.val[1]);
        FPC_SUB(t_re.val[1], t_im.val[1], g_re.val[3], g_im.val[3], g_re.val[2], g_im.val[2]);

        vload2(s_re_im, &fpr_split[j]);

        vfmul(g0_re.val[0], g0_re.val[0], half);
        vfmul(g0_re.val[1], g0_re.val[1], half);
        vstore2(&f0[j], g0_re);

        vfmul(g0_im.val[0], g0_im.val[0], half);
        vfmul(g0_im.val[1], g0_im.val[1], half);
        vstore2(&f0[j + ht], g0_im);

        vfmul(s_re_im.val[0], s_re_im.val[0], half);
        vfmul(s_re_im.val[1], s_re_im.val[1], half);

        INV_BOTJ(g1_re.val[0], g1_im.val[0], t_re.val[0], t_im.val[0], s_re_im.val[0], s_re_im.val[1]);
        INV_BOTJm(g1_re.val[1], g1_im.val[1], t_re.val[1], t_im.val[1], s_re_im.val[0], s_re_im.val[1]);

        vstore2(&f1[j], g1_re);
        vstore2(&f1[j + ht], g1_im);
    }
}

/*
 * Vectorized Split FFT implementation
 */
void PQCLEAN_FALCONPADDED512_AARCH64_poly_split_fft(fpr *restrict f0, fpr *restrict f1, const fpr *f, const unsigned logn) {
    switch (logn) {
    case 1:
        //  n = 2; hn = 1; qn = 0;
        f0[0] = f[0];
        f1[0] = f[1];
        break;

    case 2:
        PQCLEAN_FALCONPADDED512_AARCH64_poly_splitFFT_log2(f0, f1, f);
        break;

    case 3:
        PQCLEAN_FALCONPADDED512_AARCH64_poly_splitFFT_log3(f0, f1, f);
        break;

    default:
        PQCLEAN_FALCONPADDED512_AARCH64_poly_splitFFT_log4(f0, f1, f, logn);
        break;
    }
}

/*
 * Vectorized Merge FFT implementation
 */
void PQCLEAN_FALCONPADDED512_AARCH64_poly_merge_fft(fpr *restrict f, const fpr *restrict f0,
        const fpr *restrict f1, const unsigned logn) {
    switch (logn) {
    case 1:
        // n = 2; hn = 1;
        f[0] = f0[0];
        f[1] = f1[0];
        break;

    case 2:
        PQCLEAN_FALCONPADDED512_AARCH64_poly_mergeFFT_log2(f, f0, f1);
        break;

    case 3:
        PQCLEAN_FALCONPADDED512_AARCH64_poly_mergeFFT_log3(f, f0, f1);
        break;

    default:
        PQCLEAN_FALCONPADDED512_AARCH64_poly_mergeFFT_log4(f, f0, f1, logn);
        break;
    }
}
