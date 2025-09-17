#include "poly.h"

void PQCLEAN_NTRUHPS4096821_CLEAN_poly_lift(poly *r, const poly *a) {
    int i;
    for (i = 0; i < NTRU_N; i++) {
        r->coeffs[i] = a->coeffs[i];
    }
    PQCLEAN_NTRUHPS4096821_CLEAN_poly_Z3_to_Zq(r);
}


