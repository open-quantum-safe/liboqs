#include "poly.h"


void PQCLEAN_NTRUHRSS1373_CLEAN_poly_Sq_tobytes(unsigned char *r, const poly *a) {
    int i, j;
    uint16_t t[4];

    for (i = 0; i < NTRU_PACK_DEG / 4; i++) {
        for (j = 0; j < 4; j++) {
            t[j] = MODQ(a->coeffs[4 * i + j]);
        }

        //     t0   t1  t2  t3
        //  r0  8
        //  r1  6 | 2
        //  r2      8
        //  r3      4 | 4
        //  r4          8
        //  r5          2 | 6
        //  r6              8
        r[7 * i + 0] = (unsigned char) ( t[0] & 0xff );
        r[7 * i + 1] = (unsigned char) ((t[0] >> 8) | ((t[1] & 0x03) << 6));
        r[7 * i + 2] = (unsigned char) ((t[1] >> 2) & 0xff);
        r[7 * i + 3] = (unsigned char) ((t[1] >> 10) | ((t[2] & 0x0f) << 4));
        r[7 * i + 4] = (unsigned char) ((t[2] >> 4) & 0xff);
        r[7 * i + 5] = (unsigned char) ((t[2] >> 12) | ((t[3] & 0x3f) << 2));
        r[7 * i + 6] = (unsigned char) ( t[3] >> 6);
    }

    // i=NTRU_PACK_DEG/4;
    if (NTRU_PACK_DEG % 4 == 2) {
        t[0] = MODQ(a->coeffs[NTRU_PACK_DEG - 2]);
        t[1] = MODQ(a->coeffs[NTRU_PACK_DEG - 1]);
        r[7 * i + 0] = (unsigned char) ( t[0] & 0xff );
        r[7 * i + 1] = (unsigned char) ((t[0] >> 8) | ((t[1] & 0x03) << 6));
        r[7 * i + 2] = (unsigned char) ((t[1] >> 2) & 0xff);
        r[7 * i + 3] = (unsigned char) ( t[1] >> 10 );
    }
}

void PQCLEAN_NTRUHRSS1373_CLEAN_poly_Sq_frombytes(poly *r, const unsigned char *a) {
    int i;
    for (i = 0; i < NTRU_PACK_DEG / 4; i++) {
        r->coeffs[4 * i + 0] =  a[7 * i + 0]       | (((uint16_t)a[7 * i + 1] & 0x3f) << 8);
        r->coeffs[4 * i + 1] = (a[7 * i + 1] >> 6) | (((uint16_t)a[7 * i + 2]       ) << 2) | (((uint16_t)a[7 * i + 3] & 0x0f) << 10);
        r->coeffs[4 * i + 2] = (a[7 * i + 3] >> 4) | (((uint16_t)a[7 * i + 4] & 0xff) << 4) | (((uint16_t)a[7 * i + 5] & 0x03) << 12);
        r->coeffs[4 * i + 3] = (a[7 * i + 5] >> 2) | (((uint16_t)a[7 * i + 6]       ) << 6);
    }

    // i=NTRU_PACK_DEG/4;
    if (NTRU_PACK_DEG % 4 == 2) {
        r->coeffs[4 * i + 0] =  a[7 * i + 0]       | (((uint16_t)a[7 * i + 1] & 0x3f) << 8);
        r->coeffs[4 * i + 1] = (a[7 * i + 1] >> 6) | (((uint16_t)a[7 * i + 2]       ) << 2) | (((uint16_t)a[7 * i + 3] & 0x0f) << 10);
    }
    r->coeffs[NTRU_N - 1] = 0;
}

void PQCLEAN_NTRUHRSS1373_CLEAN_poly_Rq_sum_zero_tobytes(unsigned char *r, const poly *a) {
    PQCLEAN_NTRUHRSS1373_CLEAN_poly_Sq_tobytes(r, a);
}

void PQCLEAN_NTRUHRSS1373_CLEAN_poly_Rq_sum_zero_frombytes(poly *r, const unsigned char *a) {
    int i;
    PQCLEAN_NTRUHRSS1373_CLEAN_poly_Sq_frombytes(r, a);

    /* Set r[n-1] so that the sum of coefficients is zero mod q */
    r->coeffs[NTRU_N - 1] = 0;
    for (i = 0; i < NTRU_PACK_DEG; i++) {
        r->coeffs[NTRU_N - 1] -= r->coeffs[i];
    }
}
