#include "poly.h"

/* Map {0, 1, 2} -> {0,1,q-1} in place */
void PQCLEAN_NTRUHPS4096821_AVX2_poly_Z3_to_Zq(poly *r) {
    int i;
    for (i = 0; i < NTRU_N; i++) {
        r->coeffs[i] = r->coeffs[i] | ((-(r->coeffs[i] >> 1)) & (NTRU_Q - 1));
    }
}

/* Map {0, 1, q-1} -> {0,1,2} in place */
void PQCLEAN_NTRUHPS4096821_AVX2_poly_trinary_Zq_to_Z3(poly *r) {
    int i;
    for (i = 0; i < NTRU_N; i++) {
        r->coeffs[i] = MODQ(r->coeffs[i]);
        r->coeffs[i] = 3 & (r->coeffs[i] ^ (r->coeffs[i] >> (NTRU_LOGQ - 1)));
    }
}

void PQCLEAN_NTRUHPS4096821_AVX2_poly_Sq_mul(poly *r, const poly *a, const poly *b) {
    PQCLEAN_NTRUHPS4096821_AVX2_poly_Rq_mul(r, a, b);
    PQCLEAN_NTRUHPS4096821_AVX2_poly_mod_q_Phi_n(r);
}

void PQCLEAN_NTRUHPS4096821_AVX2_poly_S3_mul(poly *r, const poly *a, const poly *b) {
    int i;

    /* Our S3 multiplications do not overflow mod q,    */
    /* so we can re-purpose PQCLEAN_NTRUHPS4096821_AVX2_poly_Rq_mul, as long as we  */
    /* follow with an explicit reduction mod q.         */
    PQCLEAN_NTRUHPS4096821_AVX2_poly_Rq_mul(r, a, b);
    for (i = 0; i < NTRU_N; i++) {
        r->coeffs[i] = MODQ(r->coeffs[i]);
    }
    PQCLEAN_NTRUHPS4096821_AVX2_poly_mod_3_Phi_n(r);
}

static void PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_inv_to_Rq_inv(poly *r, const poly *ai, const poly *a) {

    int i;
    poly b, c;
    poly s;

    // for 0..4
    //    ai = ai * (2 - a*ai)  mod q
    for (i = 0; i < NTRU_N; i++) {
        b.coeffs[i] = -(a->coeffs[i]);
    }

    for (i = 0; i < NTRU_N; i++) {
        r->coeffs[i] = ai->coeffs[i];
    }

    PQCLEAN_NTRUHPS4096821_AVX2_poly_Rq_mul(&c, r, &b);
    c.coeffs[0] += 2; // c = 2 - a*ai
    PQCLEAN_NTRUHPS4096821_AVX2_poly_Rq_mul(&s, &c, r); // s = ai*c

    PQCLEAN_NTRUHPS4096821_AVX2_poly_Rq_mul(&c, &s, &b);
    c.coeffs[0] += 2; // c = 2 - a*s
    PQCLEAN_NTRUHPS4096821_AVX2_poly_Rq_mul(r, &c, &s); // r = s*c

    PQCLEAN_NTRUHPS4096821_AVX2_poly_Rq_mul(&c, r, &b);
    c.coeffs[0] += 2; // c = 2 - a*r
    PQCLEAN_NTRUHPS4096821_AVX2_poly_Rq_mul(&s, &c, r); // s = r*c

    PQCLEAN_NTRUHPS4096821_AVX2_poly_Rq_mul(&c, &s, &b);
    c.coeffs[0] += 2; // c = 2 - a*s
    PQCLEAN_NTRUHPS4096821_AVX2_poly_Rq_mul(r, &c, &s); // r = s*c
}

void PQCLEAN_NTRUHPS4096821_AVX2_poly_Rq_inv(poly *r, const poly *a) {
    poly ai2;
    PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_inv(&ai2, a);
    PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_inv_to_Rq_inv(r, &ai2, a);
}
