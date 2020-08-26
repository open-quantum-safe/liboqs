#include "poly.h"

void PQCLEAN_NTRUHRSS701_CLEAN_poly_Rq_mul(poly *r, const poly *a, const poly *b) {
    int k, i;

    for (k = 0; k < NTRU_N; k++) {
        r->coeffs[k] = 0;
        for (i = 1; i < NTRU_N - k; i++) {
            r->coeffs[k] += a->coeffs[k + i] * b->coeffs[NTRU_N - i];
        }
        for (i = 0; i < k + 1; i++) {
            r->coeffs[k] += a->coeffs[k - i] * b->coeffs[i];
        }
    }
}
