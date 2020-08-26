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

void PQCLEAN_NTRUHPS4096821_CLEAN_poly_mod_3_Phi_n(poly *r) {
    int i;
    for (i = 0; i < NTRU_N; i++) {
        r->coeffs[i] = mod3(r->coeffs[i] + 2 * r->coeffs[NTRU_N - 1]);
    }
}

void PQCLEAN_NTRUHPS4096821_CLEAN_poly_mod_q_Phi_n(poly *r) {
    int i;
    for (i = 0; i < NTRU_N; i++) {
        r->coeffs[i] = r->coeffs[i] - r->coeffs[NTRU_N - 1];
    }
}

void PQCLEAN_NTRUHPS4096821_CLEAN_poly_Rq_to_S3(poly *r, const poly *a) {
    /* NOTE: Assumes input is in [0,Q-1]^N */
    /*       Produces output in {0,1,2}^N */
    int i;

    /* Center coeffs around 3Q: [0, Q-1] -> [3Q - Q/2, 3Q + Q/2) */
    for (i = 0; i < NTRU_N; i++) {
        r->coeffs[i] = ((MODQ(a->coeffs[i]) >> (NTRU_LOGQ - 1)) ^ 3) << NTRU_LOGQ;
        r->coeffs[i] += MODQ(a->coeffs[i]);
    }

    PQCLEAN_NTRUHPS4096821_CLEAN_poly_mod_3_Phi_n(r);
}

