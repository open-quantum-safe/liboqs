#include "poly.h"

static uint16_t mod3(uint16_t a) {
    uint16_t r;
    int16_t t, c;

    r = (a >> 8) + (a & 0xff); // r mod 255 == a mod 255
    r = (r >> 4) + (r & 0xf); // r' mod 15 == r mod 15
    r = (r >> 2) + (r & 0x3); // r' mod 3 == r mod 3
    r = (r >> 2) + (r & 0x3); // r' mod 3 == r mod 3

    t = r - 3;
    c = t >> 15;

    return (c & r) ^ (~c & t);
}

void PQCLEAN_NTRUHPS40961229_CLEAN_poly_mod_3_Phi_n(poly *r) {
    int i;
    for (i = 0; i < NTRU_N; i++) {
        r->coeffs[i] = mod3(r->coeffs[i] + 2 * r->coeffs[NTRU_N - 1]);
    }
}

void PQCLEAN_NTRUHPS40961229_CLEAN_poly_mod_q_Phi_n(poly *r) {
    int i;
    for (i = 0; i < NTRU_N; i++) {
        r->coeffs[i] = r->coeffs[i] - r->coeffs[NTRU_N - 1];
    }
}

void PQCLEAN_NTRUHPS40961229_CLEAN_poly_Rq_to_S3(poly *r, const poly *a) {
    int i;
    uint16_t flag;

    /* The coefficients of a are stored as non-negative integers. */
    /* We must translate to representatives in [-q/2, q/2) before */
    /* reduction mod 3.                                           */
    for (i = 0; i < NTRU_N; i++) {
        /* Need an explicit reduction mod q here                    */
        r->coeffs[i] = MODQ(a->coeffs[i]);

        /* flag = 1 if r[i] >= q/2 else 0                            */
        flag = r->coeffs[i] >> (NTRU_LOGQ - 1);

        /* Now we will add (-q) mod 3 if r[i] >= q/2                 */
        /* Note (-q) mod 3=(-2^k) mod 3=1<<(1-(k&1))                */
        r->coeffs[i] += flag << (1 - (NTRU_LOGQ & 1));
    }

    PQCLEAN_NTRUHPS40961229_CLEAN_poly_mod_3_Phi_n(r);
}

