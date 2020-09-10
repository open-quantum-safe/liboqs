#include "poly.h"

void PQCLEAN_NTRUHRSS701_AVX2_poly_S3_tobytes(unsigned char msg[NTRU_OWCPA_MSGBYTES], const poly *a) {
    int i;
    unsigned char c;

    for (i = 0; i < NTRU_PACK_DEG / 5; i++) {
        c =        a->coeffs[5 * i + 4] & 255;
        c = (3 * c + a->coeffs[5 * i + 3]) & 255;
        c = (3 * c + a->coeffs[5 * i + 2]) & 255;
        c = (3 * c + a->coeffs[5 * i + 1]) & 255;
        c = (3 * c + a->coeffs[5 * i + 0]) & 255;
        msg[i] = c;
    }
}

void PQCLEAN_NTRUHRSS701_AVX2_poly_S3_frombytes(poly *r, const unsigned char msg[NTRU_OWCPA_MSGBYTES]) {
    int i;
    unsigned char c;

    for (i = 0; i < NTRU_PACK_DEG / 5; i++) {
        c = msg[i];
        r->coeffs[5 * i + 0] = c;
        r->coeffs[5 * i + 1] = c * 171 >> 9; // this is division by 3
        r->coeffs[5 * i + 2] = c * 57 >> 9; // division by 3^2
        r->coeffs[5 * i + 3] = c * 19 >> 9; // division by 3^3
        r->coeffs[5 * i + 4] = c * 203 >> 14; // etc.
    }
    r->coeffs[NTRU_N - 1] = 0;
    PQCLEAN_NTRUHRSS701_AVX2_poly_mod_3_Phi_n(r);
}

