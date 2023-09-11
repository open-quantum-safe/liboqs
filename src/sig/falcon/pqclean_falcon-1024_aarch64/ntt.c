/*
 * High-speed vectorize NTT for N = 512, 1024
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
#include "macrous.h"
#include "ntt_consts.h"
#include "poly.h"

/*
 * Assume Input in the range [-Q/2, Q/2]
 * Total Barrett point for N = 512, 1024: 2048, 4096
 */
void PQCLEAN_FALCON1024_AARCH64_poly_ntt(int16_t a[FALCON_N], ntt_domain_t mont) {
    // Total SIMD registers 29 = 16 + 12 + 1
    int16x8x4_t v0, v1, v2, v3; // 16
    int16x8x4_t zl, zh, t, t2;  // 12
    int16x8x2_t zlh, zhh;       // 4
    int16x8_t neon_qmvq;        // 1
    const int16_t *ptr_ntt_br = PQCLEAN_FALCON1024_AARCH64_ntt_br;
    const int16_t *ptr_ntt_qinv_br = PQCLEAN_FALCON1024_AARCH64_ntt_qinv_br;

    neon_qmvq = vld1q_s16(PQCLEAN_FALCON1024_AARCH64_qmvq);
    zl.val[0] = vld1q_s16(ptr_ntt_br);
    zh.val[0] = vld1q_s16(ptr_ntt_qinv_br);
    ptr_ntt_br += 8;
    ptr_ntt_qinv_br += 8;

    // Layer 9, 8, 7
    int16x8x2_t u0, u1, u2, u3, u4, u5, u6, u7;

    for (unsigned j = 0; j < 128; j += 16) {
        vload_s16_x2(u0, &a[j]);
        vload_s16_x2(u1, &a[j + 128]);
        vload_s16_x2(u2, &a[j + 256]);
        vload_s16_x2(u3, &a[j + 384]);

        vload_s16_x2(u4, &a[j + 512]);
        vload_s16_x2(u5, &a[j + 640]);
        vload_s16_x2(u6, &a[j + 768]);
        vload_s16_x2(u7, &a[j + 896]);

        // u0, 4: .5
        // u1, 5: .5
        // u2, 6: .5
        // u3, 7: .5

        // Layer 9
        // u0 - u4, u1 - u5
        // u2 - u6, u3 - u7
        ctbf_bri_top(u4.val[0], zl.val[0], zh.val[0], 1, neon_qmvq, t.val[0]);
        ctbf_bri_top(u4.val[1], zl.val[0], zh.val[0], 1, neon_qmvq, t.val[1]);
        ctbf_bri_top(u5.val[0], zl.val[0], zh.val[0], 1, neon_qmvq, t.val[2]);
        ctbf_bri_top(u5.val[1], zl.val[0], zh.val[0], 1, neon_qmvq, t.val[3]);

        ctbf_bri_top(u6.val[0], zl.val[0], zh.val[0], 1, neon_qmvq, t2.val[0]);
        ctbf_bri_top(u6.val[1], zl.val[0], zh.val[0], 1, neon_qmvq, t2.val[1]);
        ctbf_bri_top(u7.val[0], zl.val[0], zh.val[0], 1, neon_qmvq, t2.val[2]);
        ctbf_bri_top(u7.val[1], zl.val[0], zh.val[0], 1, neon_qmvq, t2.val[3]);

        ctbf_bot(u0.val[0], u4.val[0], t.val[0]);
        ctbf_bot(u0.val[1], u4.val[1], t.val[1]);
        ctbf_bot(u1.val[0], u5.val[0], t.val[2]);
        ctbf_bot(u1.val[1], u5.val[1], t.val[3]);

        ctbf_bot(u2.val[0], u6.val[0], t2.val[0]);
        ctbf_bot(u2.val[1], u6.val[1], t2.val[1]);
        ctbf_bot(u3.val[0], u7.val[0], t2.val[2]);
        ctbf_bot(u3.val[1], u7.val[1], t2.val[3]);

        // u0, 4: 1.2
        // u1, 5: 1.2
        // u2, 6: 1.2
        // u3, 7: 1.2

        // Layer 8
        // u0 - u2, u1 - u3
        // u4 - u6, u5 - u7
        ctbf_bri_top(u2.val[0], zl.val[0], zh.val[0], 2, neon_qmvq, t.val[0]);
        ctbf_bri_top(u2.val[1], zl.val[0], zh.val[0], 2, neon_qmvq, t.val[1]);
        ctbf_bri_top(u3.val[0], zl.val[0], zh.val[0], 2, neon_qmvq, t.val[2]);
        ctbf_bri_top(u3.val[1], zl.val[0], zh.val[0], 2, neon_qmvq, t.val[3]);

        ctbf_bri_top(u6.val[0], zl.val[0], zh.val[0], 3, neon_qmvq, t2.val[0]);
        ctbf_bri_top(u6.val[1], zl.val[0], zh.val[0], 3, neon_qmvq, t2.val[1]);
        ctbf_bri_top(u7.val[0], zl.val[0], zh.val[0], 3, neon_qmvq, t2.val[2]);
        ctbf_bri_top(u7.val[1], zl.val[0], zh.val[0], 3, neon_qmvq, t2.val[3]);

        ctbf_bot(u0.val[0], u2.val[0], t.val[0]);
        ctbf_bot(u0.val[1], u2.val[1], t.val[1]);
        ctbf_bot(u1.val[0], u3.val[0], t.val[2]);
        ctbf_bot(u1.val[1], u3.val[1], t.val[3]);

        ctbf_bot(u4.val[0], u6.val[0], t2.val[0]);
        ctbf_bot(u4.val[1], u6.val[1], t2.val[1]);
        ctbf_bot(u5.val[0], u7.val[0], t2.val[2]);
        ctbf_bot(u5.val[1], u7.val[1], t2.val[3]);

        // 2.14 -> 0.5
        barrett_x2(u0, 0, 1, 0, 1, neon_qmvq, t);
        barrett_x2(u1, 0, 1, 2, 3, neon_qmvq, t);
        barrett_x2(u2, 0, 1, 0, 1, neon_qmvq, t);
        barrett_x2(u3, 0, 1, 2, 3, neon_qmvq, t);

        barrett_x2(u4, 0, 1, 0, 1, neon_qmvq, t2);
        barrett_x2(u5, 0, 1, 2, 3, neon_qmvq, t2);
        barrett_x2(u6, 0, 1, 0, 1, neon_qmvq, t2);
        barrett_x2(u7, 0, 1, 2, 3, neon_qmvq, t2);
        // u0, 4: .5
        // u1, 5: .5
        // u2, 6: .5
        // u3, 7: .5

        // Layer 7
        // u0 - u1, u2 - u3
        // u4 - u5, u6 - u7
        ctbf_bri_top(u1.val[0], zl.val[0], zh.val[0], 4, neon_qmvq, t.val[0]);
        ctbf_bri_top(u1.val[1], zl.val[0], zh.val[0], 4, neon_qmvq, t.val[1]);
        ctbf_bri_top(u3.val[0], zl.val[0], zh.val[0], 5, neon_qmvq, t.val[2]);
        ctbf_bri_top(u3.val[1], zl.val[0], zh.val[0], 5, neon_qmvq, t.val[3]);

        ctbf_bri_top(u5.val[0], zl.val[0], zh.val[0], 6, neon_qmvq, t2.val[0]);
        ctbf_bri_top(u5.val[1], zl.val[0], zh.val[0], 6, neon_qmvq, t2.val[1]);
        ctbf_bri_top(u7.val[0], zl.val[0], zh.val[0], 7, neon_qmvq, t2.val[2]);
        ctbf_bri_top(u7.val[1], zl.val[0], zh.val[0], 7, neon_qmvq, t2.val[3]);

        ctbf_bot(u0.val[0], u1.val[0], t.val[0]);
        ctbf_bot(u0.val[1], u1.val[1], t.val[1]);
        ctbf_bot(u2.val[0], u3.val[0], t.val[2]);
        ctbf_bot(u2.val[1], u3.val[1], t.val[3]);

        ctbf_bot(u4.val[0], u5.val[0], t2.val[0]);
        ctbf_bot(u4.val[1], u5.val[1], t2.val[1]);
        ctbf_bot(u6.val[0], u7.val[0], t2.val[2]);
        ctbf_bot(u6.val[1], u7.val[1], t2.val[3]);

        // u0, 4: 1.2
        // u1, 5: 1.2
        // u2, 6: 1.2
        // u3, 7: 1.2

        // Store at 1.2Q
        vstore_s16_x2(&a[j], u0);
        vstore_s16_x2(&a[j + 128], u1);
        vstore_s16_x2(&a[j + 256], u2);
        vstore_s16_x2(&a[j + 384], u3);

        vstore_s16_x2(&a[j + 512], u4);
        vstore_s16_x2(&a[j + 640], u5);
        vstore_s16_x2(&a[j + 768], u6);
        vstore_s16_x2(&a[j + 896], u7);
    }

    // Layer 6, 5, 4, 3, 2, 1, 0
    for (unsigned j = 0; j < FALCON_N; j += 128) {
        vload_s16_x4(v0, &a[j]);
        vload_s16_x4(v1, &a[j + 32]);
        vload_s16_x4(v2, &a[j + 64]);
        vload_s16_x4(v3, &a[j + 96]);

        vload_s16_x2(zlh, ptr_ntt_br);
        vload_s16_x2(zhh, ptr_ntt_qinv_br);
        ptr_ntt_br += 16;
        ptr_ntt_qinv_br += 16;

        // Layer 6
        // v0 - v2, v1 - v3
        ctbf_bri_top_x4(v2, zlh.val[0], zhh.val[0], 0, 0, 0, 0, neon_qmvq, t);
        ctbf_bri_top_x4(v3, zlh.val[0], zhh.val[0], 0, 0, 0, 0, neon_qmvq, t2);

        ctbf_bot_x4(v0, v2, t);
        ctbf_bot_x4(v1, v3, t2);

        // 2.3 -> 0.5
        barrett_x4(v0, neon_qmvq, t);
        barrett_x4(v1, neon_qmvq, t);
        barrett_x4(v2, neon_qmvq, t);
        barrett_x4(v3, neon_qmvq, t);

        // Layer 5
        // v0 - v1, v2 - v3
        ctbf_bri_top_x4(v1, zlh.val[0], zhh.val[0], 1, 1, 1, 1, neon_qmvq, t);
        ctbf_bri_top_x4(v3, zlh.val[0], zhh.val[0], 2, 2, 2, 2, neon_qmvq, t2);

        ctbf_bot_x4(v0, v1, t);
        ctbf_bot_x4(v2, v3, t2);

        // 1.3

        // Layer 4
        // v0(0, 1 - 2, 3)
        // v1(0, 1 - 2, 3)
        // v2(0, 1 - 2, 3)
        // v3(0, 1 - 2, 3)
        ctbf_bri_top(v0.val[2], zlh.val[0], zhh.val[0], 3, neon_qmvq, t.val[0]);
        ctbf_bri_top(v0.val[3], zlh.val[0], zhh.val[0], 3, neon_qmvq, t.val[1]);
        ctbf_bri_top(v1.val[2], zlh.val[0], zhh.val[0], 4, neon_qmvq, t.val[2]);
        ctbf_bri_top(v1.val[3], zlh.val[0], zhh.val[0], 4, neon_qmvq, t.val[3]);

        ctbf_bri_top(v2.val[2], zlh.val[0], zhh.val[0], 5, neon_qmvq, t2.val[0]);
        ctbf_bri_top(v2.val[3], zlh.val[0], zhh.val[0], 5, neon_qmvq, t2.val[1]);
        ctbf_bri_top(v3.val[2], zlh.val[0], zhh.val[0], 6, neon_qmvq, t2.val[2]);
        ctbf_bri_top(v3.val[3], zlh.val[0], zhh.val[0], 6, neon_qmvq, t2.val[3]);

        ctbf_bot(v0.val[0], v0.val[2], t.val[0]);
        ctbf_bot(v0.val[1], v0.val[3], t.val[1]);
        ctbf_bot(v1.val[0], v1.val[2], t.val[2]);
        ctbf_bot(v1.val[1], v1.val[3], t.val[3]);

        ctbf_bot(v2.val[0], v2.val[2], t2.val[0]);
        ctbf_bot(v2.val[1], v2.val[3], t2.val[1]);
        ctbf_bot(v3.val[0], v3.val[2], t2.val[2]);
        ctbf_bot(v3.val[1], v3.val[3], t2.val[3]);

        // 2.3 -> 0.5
        barrett_x4(v0, neon_qmvq, t);
        barrett_x4(v1, neon_qmvq, t);
        barrett_x4(v2, neon_qmvq, t2);
        barrett_x4(v3, neon_qmvq, t2);

        // Layer 3
        // v0(0, 2 - 1, 3)
        // v1(0, 2 - 1, 3)
        // v2(0, 2 - 1, 3)
        // v3(0, 2 - 1, 3)
        ctbf_bri_top(v0.val[1], zlh.val[0], zhh.val[0], 7, neon_qmvq, t.val[0]);
        ctbf_bri_top(v0.val[3], zlh.val[1], zhh.val[1], 0, neon_qmvq, t.val[1]);
        ctbf_bri_top(v1.val[1], zlh.val[1], zhh.val[1], 1, neon_qmvq, t.val[2]);
        ctbf_bri_top(v1.val[3], zlh.val[1], zhh.val[1], 2, neon_qmvq, t.val[3]);

        ctbf_bri_top(v2.val[1], zlh.val[1], zhh.val[1], 3, neon_qmvq, t2.val[0]);
        ctbf_bri_top(v2.val[3], zlh.val[1], zhh.val[1], 4, neon_qmvq, t2.val[1]);
        ctbf_bri_top(v3.val[1], zlh.val[1], zhh.val[1], 5, neon_qmvq, t2.val[2]);
        ctbf_bri_top(v3.val[3], zlh.val[1], zhh.val[1], 6, neon_qmvq, t2.val[3]);

        ctbf_bot(v0.val[0], v0.val[1], t.val[0]);
        ctbf_bot(v0.val[2], v0.val[3], t.val[1]);
        ctbf_bot(v1.val[0], v1.val[1], t.val[2]);
        ctbf_bot(v1.val[2], v1.val[3], t.val[3]);

        ctbf_bot(v2.val[0], v2.val[1], t2.val[0]);
        ctbf_bot(v2.val[2], v2.val[3], t2.val[1]);
        ctbf_bot(v3.val[0], v3.val[1], t2.val[2]);
        ctbf_bot(v3.val[2], v3.val[3], t2.val[3]);

        // 1.3

        // Layer 2
        // Input:
        // 0,  1,  2,  3  | 4,  5,  6,  7
        // 8,  9,  10, 11 | 12, 13, 14, 15
        // 16, 17, 18, 19 | 20, 21, 22, 23
        // 24, 25, 26, 27 | 28, 29, 30, 31
        arrange(t, v0, 0, 2, 1, 3, 0, 1, 2, 3);
        v0 = t;
        arrange(t, v1, 0, 2, 1, 3, 0, 1, 2, 3);
        v1 = t;
        arrange(t2, v2, 0, 2, 1, 3, 0, 1, 2, 3);
        v2 = t2;
        arrange(t2, v3, 0, 2, 1, 3, 0, 1, 2, 3);
        v3 = t2;
        // Output:
        // 0,  1,  2,  3  | 16, 17, 18, 19
        // 4,  5,  6,  7  | 20, 21, 22, 23
        // 8,  9,  10, 11 | 24, 25, 26, 27
        // 12, 13, 14, 15 | 28, 29, 30, 31
        vload_s16_x4(zl, ptr_ntt_br);
        vload_s16_x4(zh, ptr_ntt_qinv_br);
        ptr_ntt_br += 32;
        ptr_ntt_qinv_br += 32;

        ctbf_br_top(v0.val[1], zl.val[0], zh.val[0], neon_qmvq, t.val[0]);
        ctbf_br_top(v1.val[1], zl.val[1], zh.val[1], neon_qmvq, t.val[1]);
        ctbf_br_top(v2.val[1], zl.val[2], zh.val[2], neon_qmvq, t.val[2]);
        ctbf_br_top(v3.val[1], zl.val[3], zh.val[3], neon_qmvq, t.val[3]);

        ctbf_bot(v0.val[0], v0.val[1], t.val[0]);
        ctbf_bot(v1.val[0], v1.val[1], t.val[1]);
        ctbf_bot(v2.val[0], v2.val[1], t.val[2]);
        ctbf_bot(v3.val[0], v3.val[1], t.val[3]);

        vload_s16_x4(zl, ptr_ntt_br);
        vload_s16_x4(zh, ptr_ntt_qinv_br);
        ptr_ntt_br += 32;
        ptr_ntt_qinv_br += 32;

        ctbf_br_top(v0.val[3], zl.val[0], zh.val[0], neon_qmvq, t.val[0]);
        ctbf_br_top(v1.val[3], zl.val[1], zh.val[1], neon_qmvq, t.val[1]);
        ctbf_br_top(v2.val[3], zl.val[2], zh.val[2], neon_qmvq, t.val[2]);
        ctbf_br_top(v3.val[3], zl.val[3], zh.val[3], neon_qmvq, t.val[3]);

        ctbf_bot(v0.val[2], v0.val[3], t.val[0]);
        ctbf_bot(v1.val[2], v1.val[3], t.val[1]);
        ctbf_bot(v2.val[2], v2.val[3], t.val[2]);
        ctbf_bot(v3.val[2], v3.val[3], t.val[3]);

        // 2.3 -> 0.5
        barrett_x4(v0, neon_qmvq, t);
        barrett_x4(v1, neon_qmvq, t);
        barrett_x4(v2, neon_qmvq, t2);
        barrett_x4(v3, neon_qmvq, t2);

        // Layer 1: v0.val[0] x v0.val[2] | v0.val[1] x v0.val[3]
        // v0.val[0]: 0,  1,  2,  3  | 16, 17, 18, 19
        // v0.val[1]: 4,  5,  6,  7  | 20, 21, 22, 23
        // v0.val[2]: 8,  9,  10, 11 | 24, 25, 26, 27
        // v0.val[3]: 12, 13, 14, 15 | 28, 29, 30, 31
        // transpose 4x4
        transpose(v0, t);
        transpose(v1, t);
        transpose(v2, t2);
        transpose(v3, t2);
        // v0.val[0]: 0, 4, 8,  12 | 16, 20, 24, 28
        // v0.val[1]: 1, 5, 9,  13 | 17, 21, 25, 29
        // v0.val[2]: 2, 6, 10, 14 | 18, 22, 26, 30
        // v0.val[3]: 3, 7, 11, 15 | 19, 23, 27, 31

        vload_s16_x4(zl, ptr_ntt_br);
        vload_s16_x4(zh, ptr_ntt_qinv_br);
        ptr_ntt_br += 32;
        ptr_ntt_qinv_br += 32;

        ctbf_br_top(v0.val[2], zl.val[0], zh.val[0], neon_qmvq, t.val[0]);
        ctbf_br_top(v0.val[3], zl.val[0], zh.val[0], neon_qmvq, t.val[1]);
        ctbf_br_top(v1.val[2], zl.val[1], zh.val[1], neon_qmvq, t.val[2]);
        ctbf_br_top(v1.val[3], zl.val[1], zh.val[1], neon_qmvq, t.val[3]);

        ctbf_bot(v0.val[0], v0.val[2], t.val[0]);
        ctbf_bot(v0.val[1], v0.val[3], t.val[1]);
        ctbf_bot(v1.val[0], v1.val[2], t.val[2]);
        ctbf_bot(v1.val[1], v1.val[3], t.val[3]);

        ctbf_br_top(v2.val[2], zl.val[2], zh.val[2], neon_qmvq, t.val[0]);
        ctbf_br_top(v2.val[3], zl.val[2], zh.val[2], neon_qmvq, t.val[1]);
        ctbf_br_top(v3.val[2], zl.val[3], zh.val[3], neon_qmvq, t.val[2]);
        ctbf_br_top(v3.val[3], zl.val[3], zh.val[3], neon_qmvq, t.val[3]);

        ctbf_bot(v2.val[0], v2.val[2], t.val[0]);
        ctbf_bot(v2.val[1], v2.val[3], t.val[1]);
        ctbf_bot(v3.val[0], v3.val[2], t.val[2]);
        ctbf_bot(v3.val[1], v3.val[3], t.val[3]);

        // 1.3

        // Layer 0
        // v(0, 2 - 1, 3)
        vload_s16_x4(zl, ptr_ntt_br);
        vload_s16_x4(zh, ptr_ntt_qinv_br);
        ptr_ntt_br += 32;
        ptr_ntt_qinv_br += 32;

        ctbf_br_top(v0.val[1], zl.val[0], zh.val[0], neon_qmvq, t.val[0]);
        ctbf_br_top(v1.val[1], zl.val[1], zh.val[1], neon_qmvq, t.val[1]);
        ctbf_br_top(v2.val[1], zl.val[2], zh.val[2], neon_qmvq, t.val[2]);
        ctbf_br_top(v3.val[1], zl.val[3], zh.val[3], neon_qmvq, t.val[3]);

        ctbf_bot(v0.val[0], v0.val[1], t.val[0]);
        ctbf_bot(v1.val[0], v1.val[1], t.val[1]);
        ctbf_bot(v2.val[0], v2.val[1], t.val[2]);
        ctbf_bot(v3.val[0], v3.val[1], t.val[3]);

        vload_s16_x4(zl, ptr_ntt_br);
        vload_s16_x4(zh, ptr_ntt_qinv_br);
        ptr_ntt_br += 32;
        ptr_ntt_qinv_br += 32;

        ctbf_br_top(v0.val[3], zl.val[0], zh.val[0], neon_qmvq, t.val[0]);
        ctbf_br_top(v1.val[3], zl.val[1], zh.val[1], neon_qmvq, t.val[1]);
        ctbf_br_top(v2.val[3], zl.val[2], zh.val[2], neon_qmvq, t.val[2]);
        ctbf_br_top(v3.val[3], zl.val[3], zh.val[3], neon_qmvq, t.val[3]);

        ctbf_bot(v0.val[2], v0.val[3], t.val[0]);
        ctbf_bot(v1.val[2], v1.val[3], t.val[1]);
        ctbf_bot(v2.val[2], v2.val[3], t.val[2]);
        ctbf_bot(v3.val[2], v3.val[3], t.val[3]);

        // 2.3

        if (mont == NTT_MONT) {
            // Convert to Montgomery domain by multiply with FALCON_MONT
            barmuli_mont_x8(v0, v1, neon_qmvq, t, t2);
            barmuli_mont_x8(v2, v3, neon_qmvq, t, t2);
        } else if (mont == NTT_MONT_INV) {
            barmuli_mont_ninv_x8(v0, v1, neon_qmvq, t, t2);
            barmuli_mont_ninv_x8(v2, v3, neon_qmvq, t, t2);
        }

        vstore_s16_4(&a[j], v0);
        vstore_s16_4(&a[j + 32], v1);
        vstore_s16_4(&a[j + 64], v2);
        vstore_s16_4(&a[j + 96], v3);
    }
}

/*
 * Assume input in range [-Q, Q]
 * Total Barrett point N = 512, 1024: 1792, 3840
 */
void PQCLEAN_FALCON1024_AARCH64_poly_invntt(int16_t a[FALCON_N], invntt_domain_t ninv) {
    // Total SIMD registers: 29 = 16 + 12 + 1
    int16x8x4_t v0, v1, v2, v3; // 16
    int16x8x4_t zl, zh, t, t2;  // 12
    int16x8x2_t zlh, zhh;       // 4
    int16x8_t neon_qmvq;        // 1
    const int16_t *ptr_invntt_br = PQCLEAN_FALCON1024_AARCH64_invntt_br;
    const int16_t *ptr_invntt_qinv_br = PQCLEAN_FALCON1024_AARCH64_invntt_qinv_br;

    neon_qmvq = vld1q_s16(PQCLEAN_FALCON1024_AARCH64_qmvq);
    unsigned j;

    // Layer 0, 1, 2, 3, 4, 5, 6
    for (j = 0; j < FALCON_N; j += 128) {
        vload_s16_4(v0, &a[j]);
        vload_s16_4(v1, &a[j + 32]);
        vload_s16_4(v2, &a[j + 64]);
        vload_s16_4(v3, &a[j + 96]);

        // Layer 0
        // v0.val[0]: 0, 4, 8,  12 | 16, 20, 24, 28
        // v0.val[1]: 1, 5, 9,  13 | 17, 21, 25, 29
        // v0.val[2]: 2, 6, 10, 14 | 18, 22, 26, 30
        // v0.val[3]: 3, 7, 11, 15 | 19, 23, 27, 31

        gsbf_top(v0.val[0], v0.val[1], t.val[0]);
        gsbf_top(v1.val[0], v1.val[1], t.val[1]);
        gsbf_top(v2.val[0], v2.val[1], t.val[2]);
        gsbf_top(v3.val[0], v3.val[1], t.val[3]);

        gsbf_top(v0.val[2], v0.val[3], t2.val[0]);
        gsbf_top(v1.val[2], v1.val[3], t2.val[1]);
        gsbf_top(v2.val[2], v2.val[3], t2.val[2]);
        gsbf_top(v3.val[2], v3.val[3], t2.val[3]);

        vload_s16_x2(zlh, ptr_invntt_br);
        vload_s16_x2(zhh, ptr_invntt_qinv_br);
        ptr_invntt_br += 16;
        ptr_invntt_qinv_br += 16;

        // 0 - 1*, 2 - 3*
        gsbf_br_bot(v0.val[1], zlh.val[0], zhh.val[0], neon_qmvq, t.val[0]);
        gsbf_br_bot(v1.val[1], zlh.val[1], zhh.val[1], neon_qmvq, t.val[1]);

        vload_s16_x2(zlh, ptr_invntt_br);
        vload_s16_x2(zhh, ptr_invntt_qinv_br);
        ptr_invntt_br += 16;
        ptr_invntt_qinv_br += 16;

        gsbf_br_bot(v2.val[1], zlh.val[0], zhh.val[0], neon_qmvq, t.val[2]);
        gsbf_br_bot(v3.val[1], zlh.val[1], zhh.val[1], neon_qmvq, t.val[3]);

        vload_s16_x4(zl, ptr_invntt_br);
        vload_s16_x4(zh, ptr_invntt_qinv_br);
        ptr_invntt_br += 32;
        ptr_invntt_qinv_br += 32;

        gsbf_br_bot(v0.val[3], zl.val[0], zh.val[0], neon_qmvq, t2.val[0]);
        gsbf_br_bot(v1.val[3], zl.val[1], zh.val[1], neon_qmvq, t2.val[1]);
        gsbf_br_bot(v2.val[3], zl.val[2], zh.val[2], neon_qmvq, t2.val[2]);
        gsbf_br_bot(v3.val[3], zl.val[3], zh.val[3], neon_qmvq, t2.val[3]);

        // 0: 2
        // 1: 1.3
        // 2: 2
        // 3: 1.3

        barrett(v0.val[0], neon_qmvq, t.val[0]);
        barrett(v1.val[0], neon_qmvq, t.val[1]);
        barrett(v2.val[0], neon_qmvq, t.val[2]);
        barrett(v3.val[0], neon_qmvq, t.val[3]);

        // 0: 0.5
        // 1: 1.3
        // 2: 2
        // 3: 1.3

        // Layer 1
        // v0.val[0]: 0, 4, 8,  12 | 16, 20, 24, 28
        // v0.val[1]: 1, 5, 9,  13 | 17, 21, 25, 29
        // v0.val[2]: 2, 6, 10, 14 | 18, 22, 26, 30
        // v0.val[3]: 3, 7, 11, 15 | 19, 23, 27, 31
        // 0 - 2*, 1 - 3*

        vload_s16_x2(zlh, ptr_invntt_br);
        vload_s16_x2(zhh, ptr_invntt_qinv_br);
        ptr_invntt_br += 16;
        ptr_invntt_qinv_br += 16;

        gsbf_top(v0.val[0], v0.val[2], t.val[0]);
        gsbf_top(v0.val[1], v0.val[3], t.val[1]);
        gsbf_top(v1.val[0], v1.val[2], t.val[2]);
        gsbf_top(v1.val[1], v1.val[3], t.val[3]);

        gsbf_top(v2.val[0], v2.val[2], t2.val[0]);
        gsbf_top(v2.val[1], v2.val[3], t2.val[1]);
        gsbf_top(v3.val[0], v3.val[2], t2.val[2]);
        gsbf_top(v3.val[1], v3.val[3], t2.val[3]);

        gsbf_br_bot(v0.val[2], zlh.val[0], zhh.val[0], neon_qmvq, t.val[0]);
        gsbf_br_bot(v0.val[3], zlh.val[0], zhh.val[0], neon_qmvq, t.val[1]);
        gsbf_br_bot(v1.val[2], zlh.val[1], zhh.val[1], neon_qmvq, t.val[2]);
        gsbf_br_bot(v1.val[3], zlh.val[1], zhh.val[1], neon_qmvq, t.val[3]);

        vload_s16_x2(zlh, ptr_invntt_br);
        vload_s16_x2(zhh, ptr_invntt_qinv_br);
        ptr_invntt_br += 16;
        ptr_invntt_qinv_br += 16;

        gsbf_br_bot(v2.val[2], zlh.val[0], zhh.val[0], neon_qmvq, t2.val[0]);
        gsbf_br_bot(v2.val[3], zlh.val[0], zhh.val[0], neon_qmvq, t2.val[1]);
        gsbf_br_bot(v3.val[2], zlh.val[1], zhh.val[1], neon_qmvq, t2.val[2]);
        gsbf_br_bot(v3.val[3], zlh.val[1], zhh.val[1], neon_qmvq, t2.val[3]);

        // 0: 2.5
        // 1: 2.6
        // 2: 1.5
        // 3: 1.5

        barrett_x4(v0, neon_qmvq, t);
        barrett_x4(v1, neon_qmvq, t);
        barrett_x4(v2, neon_qmvq, t2);
        barrett_x4(v3, neon_qmvq, t2);

        // 0: 0.5
        // 1: 0.5
        // 2: 0.5
        // 3: 0.5

        // Layer 2
        // Before Transpose
        // v0.val[0]: 0, 4, 8,  12 | 16, 20, 24, 28
        // v0.val[1]: 1, 5, 9,  13 | 17, 21, 25, 29
        // v0.val[2]: 2, 6, 10, 14 | 18, 22, 26, 30
        // v0.val[3]: 3, 7, 11, 15 | 19, 23, 27, 31
        transpose(v0, t);
        transpose(v1, t);
        transpose(v2, t2);
        transpose(v3, t2);

        // After Transpose
        // v0.val[0]: 0,  1,  2,  3  | 16,  17,  18,  19
        // v0.val[1]: 4,  5,  6,  7  | 20,  21,  22,  23
        // v0.val[2]: 8,  9,  10, 11 | 24,  25,  26,  27
        // v0.val[3]: 12, 13, 14, 15 | 28,  29,  30,  31
        // 0 - 1*, 2 - 3*
        vload_s16_x2(zlh, ptr_invntt_br);
        vload_s16_x2(zhh, ptr_invntt_qinv_br);
        ptr_invntt_br += 16;
        ptr_invntt_qinv_br += 16;

        gsbf_top(v0.val[0], v0.val[1], t.val[0]);
        gsbf_top(v1.val[0], v1.val[1], t.val[1]);
        gsbf_top(v2.val[0], v2.val[1], t.val[2]);
        gsbf_top(v3.val[0], v3.val[1], t.val[3]);

        gsbf_top(v0.val[2], v0.val[3], t2.val[0]);
        gsbf_top(v1.val[2], v1.val[3], t2.val[1]);
        gsbf_top(v2.val[2], v2.val[3], t2.val[2]);
        gsbf_top(v3.val[2], v3.val[3], t2.val[3]);

        gsbf_br_bot(v0.val[1], zlh.val[0], zhh.val[0], neon_qmvq, t.val[0]);
        gsbf_br_bot(v1.val[1], zlh.val[1], zhh.val[1], neon_qmvq, t.val[1]);

        vload_s16_x2(zlh, ptr_invntt_br);
        vload_s16_x2(zhh, ptr_invntt_qinv_br);
        ptr_invntt_br += 16;
        ptr_invntt_qinv_br += 16;

        gsbf_br_bot(v2.val[1], zlh.val[0], zhh.val[0], neon_qmvq, t.val[2]);
        gsbf_br_bot(v3.val[1], zlh.val[1], zhh.val[1], neon_qmvq, t.val[3]);

        vload_s16_x4(zl, ptr_invntt_br);
        vload_s16_x4(zh, ptr_invntt_qinv_br);
        ptr_invntt_br += 32;
        ptr_invntt_qinv_br += 32;

        gsbf_br_bot(v0.val[3], zl.val[0], zh.val[0], neon_qmvq, t2.val[0]);
        gsbf_br_bot(v1.val[3], zl.val[1], zh.val[1], neon_qmvq, t2.val[1]);
        gsbf_br_bot(v2.val[3], zl.val[2], zh.val[2], neon_qmvq, t2.val[2]);
        gsbf_br_bot(v3.val[3], zl.val[3], zh.val[3], neon_qmvq, t2.val[3]);

        // 0: 1
        // 1: 0.9
        // 2: 1
        // 3: 0.9

        // Layer 3
        // Re-arrange vector from
        // v0.val[0]: 0,  1,  2,  3  | 16,  17,  18,  19
        // v0.val[1]: 4,  5,  6,  7  | 20,  21,  22,  23
        // v0.val[2]: 8,  9,  10, 11 | 24,  25,  26,  27
        // v0.val[3]: 12, 13, 14, 15 | 28,  29,  30,  31
        // Compiler will handle register re-naming
        arrange(t, v0, 0, 1, 2, 3, 0, 2, 1, 3);
        v0 = t;

        // Compiler will handle register re-naming
        arrange(t, v1, 0, 1, 2, 3, 0, 2, 1, 3);
        v1 = t;

        // Compiler will handle register re-naming
        arrange(t2, v2, 0, 1, 2, 3, 0, 2, 1, 3);
        v2 = t2;

        // Compiler will handle register re-naming
        arrange(t2, v3, 0, 1, 2, 3, 0, 2, 1, 3);
        v3 = t2;
        // To
        // v0.val[0]: 0,  1,  2,  3  | 4,  5,  6,  7
        // v0.val[1]: 8,  9,  10, 11 | 12, 13, 14, 15
        // v0.val[2]: 16, 17, 18, 19 | 20, 21, 22, 23
        // v0.val[3]: 24, 25, 26, 27 | 28, 29, 30, 31
        // 0 - 1, 2 - 3
        vload_s16_x2(zlh, ptr_invntt_br);
        vload_s16_x2(zhh, ptr_invntt_qinv_br);
        ptr_invntt_br += 16;
        ptr_invntt_qinv_br += 16;

        gsbf_top(v0.val[0], v0.val[1], t.val[0]);
        gsbf_top(v0.val[2], v0.val[3], t.val[1]);
        gsbf_top(v1.val[0], v1.val[1], t.val[2]);
        gsbf_top(v1.val[2], v1.val[3], t.val[3]);

        gsbf_top(v2.val[0], v2.val[1], t2.val[0]);
        gsbf_top(v2.val[2], v2.val[3], t2.val[1]);
        gsbf_top(v3.val[0], v3.val[1], t2.val[2]);
        gsbf_top(v3.val[2], v3.val[3], t2.val[3]);

        gsbf_bri_bot(v0.val[1], zlh.val[0], zhh.val[0], 0, neon_qmvq, t.val[0]);
        gsbf_bri_bot(v0.val[3], zlh.val[0], zhh.val[0], 1, neon_qmvq, t.val[1]);
        gsbf_bri_bot(v1.val[1], zlh.val[0], zhh.val[0], 2, neon_qmvq, t.val[2]);
        gsbf_bri_bot(v1.val[3], zlh.val[0], zhh.val[0], 3, neon_qmvq, t.val[3]);

        gsbf_bri_bot(v2.val[1], zlh.val[0], zhh.val[0], 4, neon_qmvq, t2.val[0]);
        gsbf_bri_bot(v2.val[3], zlh.val[0], zhh.val[0], 5, neon_qmvq, t2.val[1]);
        gsbf_bri_bot(v3.val[1], zlh.val[0], zhh.val[0], 6, neon_qmvq, t2.val[2]);
        gsbf_bri_bot(v3.val[3], zlh.val[0], zhh.val[0], 7, neon_qmvq, t2.val[3]);

        // 0: 2
        // 1: 1.3
        // 2: 2
        // 3: 1.3

        barrett(v0.val[0], neon_qmvq, t.val[0]);
        barrett(v1.val[0], neon_qmvq, t.val[1]);
        barrett(v2.val[0], neon_qmvq, t.val[2]);
        barrett(v3.val[0], neon_qmvq, t.val[3]);

        // 0: 0.5
        // 1: 1.3
        // 2: 2
        // 3: 1.3

        // Layer 4
        // v0.val[0]: 0,  1,  2,  3  | 4,  5,  6,  7
        // v0.val[1]: 8,  9,  10, 11 | 12, 13, 14, 15
        // v0.val[2]: 16, 17, 18, 19 | 20, 21, 22, 23
        // v0.val[3]: 24, 25, 26, 27 | 28, 29, 30, 31
        // 0 - 2, 1 - 3

        gsbf_top(v0.val[0], v0.val[2], t.val[0]);
        gsbf_top(v0.val[1], v0.val[3], t.val[1]);
        gsbf_top(v1.val[0], v1.val[2], t.val[2]);
        gsbf_top(v1.val[1], v1.val[3], t.val[3]);

        gsbf_top(v2.val[0], v2.val[2], t2.val[0]);
        gsbf_top(v2.val[1], v2.val[3], t2.val[1]);
        gsbf_top(v3.val[0], v3.val[2], t2.val[2]);
        gsbf_top(v3.val[1], v3.val[3], t2.val[3]);

        gsbf_bri_bot(v0.val[2], zlh.val[1], zhh.val[1], 0, neon_qmvq, t.val[0]);
        gsbf_bri_bot(v0.val[3], zlh.val[1], zhh.val[1], 0, neon_qmvq, t.val[1]);
        gsbf_bri_bot(v1.val[2], zlh.val[1], zhh.val[1], 1, neon_qmvq, t.val[2]);
        gsbf_bri_bot(v1.val[3], zlh.val[1], zhh.val[1], 1, neon_qmvq, t.val[3]);

        gsbf_bri_bot(v2.val[2], zlh.val[1], zhh.val[1], 2, neon_qmvq, t2.val[0]);
        gsbf_bri_bot(v2.val[3], zlh.val[1], zhh.val[1], 2, neon_qmvq, t2.val[1]);
        gsbf_bri_bot(v3.val[2], zlh.val[1], zhh.val[1], 3, neon_qmvq, t2.val[2]);
        gsbf_bri_bot(v3.val[3], zlh.val[1], zhh.val[1], 3, neon_qmvq, t2.val[3]);

        // 0: 2.5
        // 1: 2.5
        // 2: 1.5
        // 3: 1.5

        barrett_x4(v0, neon_qmvq, t);
        barrett_x4(v1, neon_qmvq, t);
        barrett_x4(v2, neon_qmvq, t2);
        barrett_x4(v3, neon_qmvq, t2);

        // 0: 0.5
        // 1: 0.5
        // 2: 0.5
        // 3: 0.5

        // Layer 5
        // Cross block
        // v0.0->3 - v1.0->3
        gsbf_top_x4(v0, v1, t);
        gsbf_top_x4(v2, v3, t2);

        gsbf_bri_bot_x4(v1, zlh.val[1], zhh.val[1], 4, 4, 4, 4, neon_qmvq, t);
        gsbf_bri_bot_x4(v3, zlh.val[1], zhh.val[1], 5, 5, 5, 5, neon_qmvq, t2);

        // v0: 1
        // v1: 0.9
        // v2: 1
        // v3: 0.9

        // Layer 6
        // Cross block
        // v0.0->3 - v2.0->3
        gsbf_top_x4(v0, v2, t);
        gsbf_top_x4(v1, v3, t2);

        gsbf_bri_bot_x4(v2, zlh.val[1], zhh.val[1], 6, 6, 6, 6, neon_qmvq, t);
        gsbf_bri_bot_x4(v3, zlh.val[1], zhh.val[1], 6, 6, 6, 6, neon_qmvq, t2);

        // v0: 2
        // v1: 1.8
        // v2: 1.3
        // v3: 1.2

        vstore_s16_x4(&a[j], v0);
        vstore_s16_x4(&a[j + 32], v1);
        vstore_s16_x4(&a[j + 64], v2);
        vstore_s16_x4(&a[j + 96], v3);
    }

    ptr_invntt_br += 8 * ninv;
    ptr_invntt_qinv_br += 8 * ninv;
    zl.val[0] = vld1q_s16(ptr_invntt_br);
    zh.val[0] = vld1q_s16(ptr_invntt_qinv_br);

    // Layer 7, 8, 9
    int16x8x2_t u0, u1, u2, u3, u4, u5, u6, u7;

    for (j = 0; j < 128; j += 16) {
        vload_s16_x2(u0, &a[j]);
        vload_s16_x2(u1, &a[j + 128]);
        vload_s16_x2(u2, &a[j + 256]);
        vload_s16_x2(u3, &a[j + 384]);

        vload_s16_x2(u4, &a[j + 512]);
        vload_s16_x2(u5, &a[j + 640]);
        vload_s16_x2(u6, &a[j + 768]);
        vload_s16_x2(u7, &a[j + 896]);

        // 2
        barrett_x2(u0, 0, 1, 0, 1, neon_qmvq, t);
        barrett_x2(u1, 0, 1, 2, 3, neon_qmvq, t);
        barrett_x2(u2, 0, 1, 0, 1, neon_qmvq, t);
        barrett_x2(u3, 0, 1, 2, 3, neon_qmvq, t);

        barrett_x2(u4, 0, 1, 0, 1, neon_qmvq, t2);
        barrett_x2(u5, 0, 1, 2, 3, neon_qmvq, t2);
        barrett_x2(u6, 0, 1, 0, 1, neon_qmvq, t2);
        barrett_x2(u7, 0, 1, 2, 3, neon_qmvq, t2);

        // u0, 4: 0.5
        // u1, 5: 0.5
        // u2, 6: 0.5
        // u3, 7: 0.5

        // Layer 7
        // u0 - u1, u2 - u3
        // u4 - u5, u6 - u7
        gsbf_top(u0.val[0], u1.val[0], t.val[0]);
        gsbf_top(u0.val[1], u1.val[1], t.val[1]);
        gsbf_top(u2.val[0], u3.val[0], t.val[2]);
        gsbf_top(u2.val[1], u3.val[1], t.val[3]);

        gsbf_top(u4.val[0], u5.val[0], t2.val[0]);
        gsbf_top(u4.val[1], u5.val[1], t2.val[1]);
        gsbf_top(u6.val[0], u7.val[0], t2.val[2]);
        gsbf_top(u6.val[1], u7.val[1], t2.val[3]);

        gsbf_bri_bot(u1.val[0], zl.val[0], zh.val[0], 0, neon_qmvq, t.val[0]);
        gsbf_bri_bot(u1.val[1], zl.val[0], zh.val[0], 0, neon_qmvq, t.val[1]);
        gsbf_bri_bot(u3.val[0], zl.val[0], zh.val[0], 1, neon_qmvq, t.val[2]);
        gsbf_bri_bot(u3.val[1], zl.val[0], zh.val[0], 1, neon_qmvq, t.val[3]);

        gsbf_bri_bot(u5.val[0], zl.val[0], zh.val[0], 2, neon_qmvq, t2.val[0]);
        gsbf_bri_bot(u5.val[1], zl.val[0], zh.val[0], 2, neon_qmvq, t2.val[1]);
        gsbf_bri_bot(u7.val[0], zl.val[0], zh.val[0], 3, neon_qmvq, t2.val[2]);
        gsbf_bri_bot(u7.val[1], zl.val[0], zh.val[0], 3, neon_qmvq, t2.val[3]);

        // u0, 4: 1
        // u1, 5: .87
        // u2, 6: 1
        // u3, 7: .87

        // Layer 8
        // u0 - u2, u1 - u3
        // u4 - u6, u5 - u7
        gsbf_top(u0.val[0], u2.val[0], t.val[0]);
        gsbf_top(u0.val[1], u2.val[1], t.val[1]);
        gsbf_top(u1.val[0], u3.val[0], t.val[2]);
        gsbf_top(u1.val[1], u3.val[1], t.val[3]);

        gsbf_top(u4.val[0], u6.val[0], t2.val[0]);
        gsbf_top(u4.val[1], u6.val[1], t2.val[1]);
        gsbf_top(u5.val[0], u7.val[0], t2.val[2]);
        gsbf_top(u5.val[1], u7.val[1], t2.val[3]);

        gsbf_bri_bot(u2.val[0], zl.val[0], zh.val[0], 4, neon_qmvq, t.val[0]);
        gsbf_bri_bot(u2.val[1], zl.val[0], zh.val[0], 4, neon_qmvq, t.val[1]);
        gsbf_bri_bot(u3.val[0], zl.val[0], zh.val[0], 4, neon_qmvq, t.val[2]);
        gsbf_bri_bot(u3.val[1], zl.val[0], zh.val[0], 4, neon_qmvq, t.val[3]);

        gsbf_bri_bot(u6.val[0], zl.val[0], zh.val[0], 5, neon_qmvq, t2.val[0]);
        gsbf_bri_bot(u6.val[1], zl.val[0], zh.val[0], 5, neon_qmvq, t2.val[1]);
        gsbf_bri_bot(u7.val[0], zl.val[0], zh.val[0], 5, neon_qmvq, t2.val[2]);
        gsbf_bri_bot(u7.val[1], zl.val[0], zh.val[0], 5, neon_qmvq, t2.val[3]);

        // u0, 4: 2
        // u2, 6: 1.25
        // u1, 5: 1.75
        // u3, 7: 1.15

        barrett_x2(u0, 0, 1, 0, 1, neon_qmvq, t);
        barrett_x2(u4, 0, 1, 2, 3, neon_qmvq, t);
        barrett_x2(u1, 0, 1, 0, 1, neon_qmvq, t2);
        barrett_x2(u5, 0, 1, 2, 3, neon_qmvq, t2);

        // u0, 4: 0.5
        // u2, 6: 1.25
        // u1, 5: 0.5
        // u3, 7: 1.15

        // Layer 9
        // u0 - u4, u1 - u5
        // u2 - u6, u3 - u7
        gsbf_top(u0.val[0], u4.val[0], t.val[0]);
        gsbf_top(u0.val[1], u4.val[1], t.val[1]);
        gsbf_top(u1.val[0], u5.val[0], t.val[2]);
        gsbf_top(u1.val[1], u5.val[1], t.val[3]);

        gsbf_top(u2.val[0], u6.val[0], t2.val[0]);
        gsbf_top(u2.val[1], u6.val[1], t2.val[1]);
        gsbf_top(u3.val[0], u7.val[0], t2.val[2]);
        gsbf_top(u3.val[1], u7.val[1], t2.val[3]);

        gsbf_bri_bot(u4.val[0], zl.val[0], zh.val[0], 6, neon_qmvq, t.val[0]);
        gsbf_bri_bot(u4.val[1], zl.val[0], zh.val[0], 6, neon_qmvq, t.val[1]);
        gsbf_bri_bot(u5.val[0], zl.val[0], zh.val[0], 6, neon_qmvq, t.val[2]);
        gsbf_bri_bot(u5.val[1], zl.val[0], zh.val[0], 6, neon_qmvq, t.val[3]);

        gsbf_bri_bot(u6.val[0], zl.val[0], zh.val[0], 6, neon_qmvq, t2.val[0]);
        gsbf_bri_bot(u6.val[1], zl.val[0], zh.val[0], 6, neon_qmvq, t2.val[1]);
        gsbf_bri_bot(u7.val[0], zl.val[0], zh.val[0], 6, neon_qmvq, t2.val[2]);
        gsbf_bri_bot(u7.val[1], zl.val[0], zh.val[0], 6, neon_qmvq, t2.val[3]);

        // u0, 4: 1, .87
        // u2, 6: 2.5, 1.5
        // u1, 5: 1, .87
        // u3, 7: 2.3, 1.4

        if (ninv == INVNTT_NINV) {
            barmul_invntt_x2(u0, zl.val[0], zh.val[0], 7, neon_qmvq, t);
            barmul_invntt_x2(u1, zl.val[0], zh.val[0], 7, neon_qmvq, t);
            barmul_invntt_x2(u2, zl.val[0], zh.val[0], 7, neon_qmvq, t2);
            barmul_invntt_x2(u3, zl.val[0], zh.val[0], 7, neon_qmvq, t2);
        }

        // u0, 4: .87, .87
        // u2, 6: 1.5, 1.5
        // u1, 5: .87, .87
        // u3, 7: 1.4, 1.4

        barrett_x2(u2, 0, 1, 0, 1, neon_qmvq, t);
        barrett_x2(u6, 0, 1, 2, 3, neon_qmvq, t);
        barrett_x2(u3, 0, 1, 0, 1, neon_qmvq, t2);
        barrett_x2(u7, 0, 1, 2, 3, neon_qmvq, t2);

        // u0, 4: .87, .87
        // u2, 6: .5, .5
        // u1, 5: .87, .87
        // u3, 7: .5, .5

        vstore_s16_x2(&a[j], u0);
        vstore_s16_x2(&a[j + 128], u1);
        vstore_s16_x2(&a[j + 256], u2);
        vstore_s16_x2(&a[j + 384], u3);

        vstore_s16_x2(&a[j + 512], u4);
        vstore_s16_x2(&a[j + 640], u5);
        vstore_s16_x2(&a[j + 768], u6);
        vstore_s16_x2(&a[j + 896], u7);
    }
}

void PQCLEAN_FALCON1024_AARCH64_poly_montmul_ntt(int16_t f[FALCON_N], const int16_t g[FALCON_N]) {
    // Total SIMD registers: 29 = 28 + 1
    int16x8x4_t a, b, c, d, e1, e2, t, k; // 28
    int16x8_t neon_qmvm;                  // 1
    neon_qmvm = vld1q_s16(PQCLEAN_FALCON1024_AARCH64_qmvq);

    for (unsigned i = 0; i < FALCON_N; i += 64) {
        vload_s16_x4(a, &f[i]);
        vload_s16_x4(b, &g[i]);
        vload_s16_x4(c, &f[i + 32]);
        vload_s16_x4(d, &g[i + 32]);

        montmul_x8(e1, e2, a, b, c, d, neon_qmvm, t, k);

        vstore_s16_x4(&f[i], e1);
        vstore_s16_x4(&f[i + 32], e2);
    }
}

/* ===================================================================== */
