/* Based on supercop-20200702/crypto_core/invhrss701/simpler/core.c */

#include "poly.h"

/* return -1 if x<0 and y<0; otherwise return 0 */
static inline int16_t both_negative_mask(int16_t x, int16_t y) {
    return (x & y) >> 15;
}

void PQCLEAN_NTRUHRSS701_CLEAN_poly_R2_inv(poly *r, const poly *a) {
    poly f, g, v, w;
    size_t i, loop;
    int16_t delta, sign, swap, t;

    for (i = 0; i < NTRU_N; ++i) {
        v.coeffs[i] = 0;
    }
    for (i = 0; i < NTRU_N; ++i) {
        w.coeffs[i] = 0;
    }
    w.coeffs[0] = 1;

    for (i = 0; i < NTRU_N; ++i) {
        f.coeffs[i] = 1;
    }
    for (i = 0; i < NTRU_N - 1; ++i) {
        g.coeffs[NTRU_N - 2 - i] = (a->coeffs[i] ^ a->coeffs[NTRU_N - 1]) & 1;
    }
    g.coeffs[NTRU_N - 1] = 0;

    delta = 1;

    for (loop = 0; loop < 2 * (NTRU_N - 1) - 1; ++loop) {
        for (i = NTRU_N - 1; i > 0; --i) {
            v.coeffs[i] = v.coeffs[i - 1];
        }
        v.coeffs[0] = 0;

        sign = g.coeffs[0] & f.coeffs[0];
        swap = both_negative_mask(-delta, -(int16_t) g.coeffs[0]);
        delta ^= swap & (delta ^ -delta);
        delta += 1;

        for (i = 0; i < NTRU_N; ++i) {
            t = swap & (f.coeffs[i] ^ g.coeffs[i]);
            f.coeffs[i] ^= t;
            g.coeffs[i] ^= t;
            t = swap & (v.coeffs[i] ^ w.coeffs[i]);
            v.coeffs[i] ^= t;
            w.coeffs[i] ^= t;
        }

        for (i = 0; i < NTRU_N; ++i) {
            g.coeffs[i] = g.coeffs[i] ^ (sign & f.coeffs[i]);
        }
        for (i = 0; i < NTRU_N; ++i) {
            w.coeffs[i] = w.coeffs[i] ^ (sign & v.coeffs[i]);
        }
        for (i = 0; i < NTRU_N - 1; ++i) {
            g.coeffs[i] = g.coeffs[i + 1];
        }
        g.coeffs[NTRU_N - 1] = 0;
    }

    for (i = 0; i < NTRU_N - 1; ++i) {
        r->coeffs[i] = v.coeffs[NTRU_N - 2 - i];
    }
    r->coeffs[NTRU_N - 1] = 0;
}
