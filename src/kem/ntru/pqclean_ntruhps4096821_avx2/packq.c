#include "poly.h"


void PQCLEAN_NTRUHPS4096821_AVX2_poly_Sq_tobytes(unsigned char *r, const poly *a) {
    int i;

    for (i = 0; i < NTRU_PACK_DEG / 2; i++) {
        r[3 * i + 0] = (unsigned char) ( MODQ(a->coeffs[2 * i + 0]) & 0xff);
        r[3 * i + 1] = (unsigned char) ((MODQ(a->coeffs[2 * i + 0]) >>  8) | ((MODQ(a->coeffs[2 * i + 1]) & 0x0f) << 4));
        r[3 * i + 2] = (unsigned char) ((MODQ(a->coeffs[2 * i + 1]) >>  4));
    }
}

void PQCLEAN_NTRUHPS4096821_AVX2_poly_Sq_frombytes(poly *r, const unsigned char *a) {
    int i;
    for (i = 0; i < NTRU_PACK_DEG / 2; i++) {
        r->coeffs[2 * i + 0] = (a[3 * i + 0] >> 0) | (((uint16_t)a[3 * i + 1] & 0x0f) << 8);
        r->coeffs[2 * i + 1] = (a[3 * i + 1] >> 4) | (((uint16_t)a[3 * i + 2] & 0xff) << 4);
    }
    r->coeffs[NTRU_N - 1] = 0;
}

void PQCLEAN_NTRUHPS4096821_AVX2_poly_Rq_sum_zero_tobytes(unsigned char *r, const poly *a) {
    PQCLEAN_NTRUHPS4096821_AVX2_poly_Sq_tobytes(r, a);
}

void PQCLEAN_NTRUHPS4096821_AVX2_poly_Rq_sum_zero_frombytes(poly *r, const unsigned char *a) {
    int i;
    PQCLEAN_NTRUHPS4096821_AVX2_poly_Sq_frombytes(r, a);

    /* Set r[n-1] so that the sum of coefficients is zero mod q */
    r->coeffs[NTRU_N - 1] = 0;
    for (i = 0; i < NTRU_PACK_DEG; i++) {
        r->coeffs[NTRU_N - 1] -= r->coeffs[i];
    }
}
