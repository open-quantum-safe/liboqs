/*
 * Falcon signature verification.
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
#include "poly.h"

/* see inner.h */
void PQCLEAN_FALCONPADDED1024_AARCH64_to_ntt(int16_t *h) {
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_ntt(h, NTT_NONE);
}

void PQCLEAN_FALCONPADDED1024_AARCH64_to_ntt_monty(int16_t *h) {
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_ntt(h, NTT_MONT);
}

/* see inner.h */
int PQCLEAN_FALCONPADDED1024_AARCH64_verify_raw(const int16_t *c0, const int16_t *s2,
        int16_t *h, int16_t *tmp) {
    int16_t *tt = tmp;

    /*
     * Compute s1 = c0 - s2*h mod phi mod q (in tt[]).
     */

    memcpy(tt, s2, sizeof(int16_t) * FALCON_N);
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_ntt(h, NTT_NONE);
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_ntt(tt, NTT_MONT_INV);
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_montmul_ntt(tt, h);
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_invntt(tt, INVNTT_NONE);
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_sub_barrett(tt, c0, tt);

    /*
     * Signature is valid if and only if the aggregate (s1,s2) vector
     * is short enough.
     */
    return PQCLEAN_FALCONPADDED1024_AARCH64_is_short(tt, s2);
}

/* see inner.h */
int PQCLEAN_FALCONPADDED1024_AARCH64_compute_public(int16_t *h, const int8_t *f, const int8_t *g, int16_t *tmp) {
    int16_t *tt = tmp;

    PQCLEAN_FALCONPADDED1024_AARCH64_poly_int8_to_int16(h, g);
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_ntt(h, NTT_NONE);

    PQCLEAN_FALCONPADDED1024_AARCH64_poly_int8_to_int16(tt, f);
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_ntt(tt, NTT_MONT);

    if (PQCLEAN_FALCONPADDED1024_AARCH64_poly_compare_with_zero(tt)) {
        return 0;
    }
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_div_12289(h, tt);

    PQCLEAN_FALCONPADDED1024_AARCH64_poly_invntt(h, INVNTT_NINV);

    PQCLEAN_FALCONPADDED1024_AARCH64_poly_convert_to_unsigned(h);

    return 1;
}

/* see inner.h */
int PQCLEAN_FALCONPADDED1024_AARCH64_complete_private(int8_t *G, const int8_t *f,
        const int8_t *g, const int8_t *F,
        uint8_t *tmp) {
    int16_t *t1, *t2;

    t1 = (int16_t *)tmp;
    t2 = t1 + FALCON_N;

    PQCLEAN_FALCONPADDED1024_AARCH64_poly_int8_to_int16(t1, g);
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_ntt(t1, NTT_NONE);

    PQCLEAN_FALCONPADDED1024_AARCH64_poly_int8_to_int16(t2, F);
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_ntt(t2, NTT_MONT);

    PQCLEAN_FALCONPADDED1024_AARCH64_poly_montmul_ntt(t1, t2);

    PQCLEAN_FALCONPADDED1024_AARCH64_poly_int8_to_int16(t2, f);
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_ntt(t2, NTT_MONT);

    if (PQCLEAN_FALCONPADDED1024_AARCH64_poly_compare_with_zero(t2)) {
        return 0;
    }
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_div_12289(t1, t2);

    PQCLEAN_FALCONPADDED1024_AARCH64_poly_invntt(t1, INVNTT_NINV);

    if (PQCLEAN_FALCONPADDED1024_AARCH64_poly_int16_to_int8(G, t1)) {
        return 0;
    }
    return 1;
}

/* see inner.h */
int PQCLEAN_FALCONPADDED1024_AARCH64_is_invertible(const int16_t *s2, uint8_t *tmp) {
    int16_t *tt = (int16_t *)tmp;
    uint16_t r;

    memcpy(tt, s2, sizeof(int16_t) * FALCON_N);
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_ntt(tt, NTT_MONT);

    r = PQCLEAN_FALCONPADDED1024_AARCH64_poly_compare_with_zero(tt);

    return (int)(1u - (r >> 15));
}

/* see inner.h */
int PQCLEAN_FALCONPADDED1024_AARCH64_verify_recover(int16_t *h, const int16_t *c0,
        const int16_t *s1, const int16_t *s2,
        uint8_t *tmp) {
    int16_t *tt = (int16_t *)tmp;
    uint16_t r;

    /*
     * Compute h = (c0 - s1) / s2. If one of the coefficients of s2
     * is zero (in NTT representation) then the operation fails. We
     * keep that information into a flag so that we do not deviate
     * from strict constant-time processing; if all coefficients of
     * s2 are non-zero, then the high bit of r will be zero.
     */

    PQCLEAN_FALCONPADDED1024_AARCH64_poly_sub_barrett(h, c0, s1);
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_ntt(h, NTT_NONE);

    /*
     * Reduce elements of s1 and s2 modulo q; then write s2 into tt[]
     * and c0 - s1 into h[].
     */
    memcpy(tt, s2, sizeof(int16_t) * FALCON_N);
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_ntt(tt, NTT_MONT);
    r = PQCLEAN_FALCONPADDED1024_AARCH64_poly_compare_with_zero(tt);
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_div_12289(h, tt);

    PQCLEAN_FALCONPADDED1024_AARCH64_poly_invntt(h, INVNTT_NINV);

    /*
     * Signature is acceptable if and only if it is short enough,
     * and s2 was invertible mod phi mod q. The caller must still
     * check that the rebuilt public key matches the expected
     * value (e.g. through a hash).
     */
    r = (uint16_t) (~r & (uint16_t) - PQCLEAN_FALCONPADDED1024_AARCH64_is_short(s1, s2));
    return (int)(r >> 15);
}

/* see inner.h */
int PQCLEAN_FALCONPADDED1024_AARCH64_count_nttzero(const int16_t *sig, uint8_t *tmp) {
    int16_t *s2 = (int16_t *)tmp;

    memcpy(s2, sig, sizeof(int16_t) * FALCON_N);
    PQCLEAN_FALCONPADDED1024_AARCH64_poly_ntt(s2, NTT_MONT);

    int r = PQCLEAN_FALCONPADDED1024_AARCH64_poly_compare_with_zero(s2);

    return r;
}
