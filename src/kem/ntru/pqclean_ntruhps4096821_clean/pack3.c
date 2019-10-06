#include "poly.h"

void PQCLEAN_NTRUHPS4096821_CLEAN_poly_S3_tobytes(unsigned char msg[NTRU_OWCPA_MSGBYTES], const poly *a) {
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

void PQCLEAN_NTRUHPS4096821_CLEAN_poly_S3_frombytes(poly *r, const unsigned char msg[NTRU_OWCPA_MSGBYTES]) {
    int i;
    unsigned char c;

    for (i = 0; i < NTRU_PACK_DEG / 5; i++) {
        c = msg[i];
        r->coeffs[5 * i + 0] = PQCLEAN_NTRUHPS4096821_CLEAN_mod3(c);
        r->coeffs[5 * i + 1] = PQCLEAN_NTRUHPS4096821_CLEAN_mod3(c * 171 >> 9); // this is division by 3
        r->coeffs[5 * i + 2] = PQCLEAN_NTRUHPS4096821_CLEAN_mod3(c * 57 >> 9); // division by 3^2
        r->coeffs[5 * i + 3] = PQCLEAN_NTRUHPS4096821_CLEAN_mod3(c * 19 >> 9); // division by 3^3
        r->coeffs[5 * i + 4] = PQCLEAN_NTRUHPS4096821_CLEAN_mod3(c * 203 >> 14); // etc.
    }
    r->coeffs[NTRU_N - 1] = 0;
}

