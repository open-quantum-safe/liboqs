#include "poly.h"


void PQCLEAN_NTRUHRSS701_CLEAN_poly_lift(poly *r, const poly *a) {
    /* NOTE: Assumes input is in {0,1,2}^N */
    /*       Produces output in [0,Q-1]^N */
    int i;
    poly b;
    uint16_t t, zj;

    /* Define z by <z*x^i, x-1> = delta_{i,0} mod 3:      */
    /*   t      = -1/N mod p = -N mod 3                   */
    /*   z[0]   = 2 - t mod 3                             */
    /*   z[1]   = 0 mod 3                                 */
    /*   z[j]   = z[j-1] + t mod 3                        */
    /* We'll compute b = a/(x-1) mod (3, Phi) using       */
    /*   b[0] = <z, a>, b[1] = <z*x,a>, b[2] = <z*x^2,a>  */
    /*   b[i] = b[i-3] - (a[i] + a[i-1] + a[i-2])         */
    t = 3 - (NTRU_N % 3);
    b.coeffs[0] = a->coeffs[0] * (2 - t) + a->coeffs[1] * 0 + a->coeffs[2] * t;
    b.coeffs[1] = a->coeffs[1] * (2 - t) + a->coeffs[2] * 0;
    b.coeffs[2] = a->coeffs[2] * (2 - t);

    zj = 0; /* z[1] */
    for (i = 3; i < NTRU_N; i++) {
        b.coeffs[0] += a->coeffs[i] * (zj + 2 * t);
        b.coeffs[1] += a->coeffs[i] * (zj + t);
        b.coeffs[2] += a->coeffs[i] * zj;
        zj = (zj + t) % 3;
    }
    b.coeffs[1] += a->coeffs[0] * (zj + t);
    b.coeffs[2] += a->coeffs[0] * zj;
    b.coeffs[2] += a->coeffs[1] * (zj + t);

    b.coeffs[0] = b.coeffs[0];
    b.coeffs[1] = b.coeffs[1];
    b.coeffs[2] = b.coeffs[2];
    for (i = 3; i < NTRU_N; i++) {
        b.coeffs[i] = b.coeffs[i - 3] + 2 * (a->coeffs[i] + a->coeffs[i - 1] + a->coeffs[i - 2]);
    }

    /* Finish reduction mod Phi by subtracting Phi * b[N-1] */
    PQCLEAN_NTRUHRSS701_CLEAN_poly_mod_3_Phi_n(&b);

    /* Switch from {0,1,2} to {0,1,q-1} coefficient representation */
    PQCLEAN_NTRUHRSS701_CLEAN_poly_Z3_to_Zq(&b);

    /* Multiply by (x-1) */
    r->coeffs[0] = -(b.coeffs[0]);
    for (i = 0; i < NTRU_N - 1; i++) {
        r->coeffs[i + 1] = b.coeffs[i] - b.coeffs[i + 1];
    }
}

