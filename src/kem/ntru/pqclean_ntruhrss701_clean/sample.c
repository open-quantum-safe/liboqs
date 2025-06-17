#include "sample.h"

void PQCLEAN_NTRUHRSS701_CLEAN_sample_fg(poly *f, poly *g, const unsigned char uniformbytes[NTRU_SAMPLE_FG_BYTES]) {
    PQCLEAN_NTRUHRSS701_CLEAN_sample_iid_plus(f, uniformbytes);
    PQCLEAN_NTRUHRSS701_CLEAN_sample_iid_plus(g, uniformbytes + NTRU_SAMPLE_IID_BYTES);

}

void PQCLEAN_NTRUHRSS701_CLEAN_sample_rm(poly *r, poly *m, const unsigned char uniformbytes[NTRU_SAMPLE_RM_BYTES]) {
    PQCLEAN_NTRUHRSS701_CLEAN_sample_iid(r, uniformbytes);
    PQCLEAN_NTRUHRSS701_CLEAN_sample_iid(m, uniformbytes + NTRU_SAMPLE_IID_BYTES);

}

void PQCLEAN_NTRUHRSS701_CLEAN_sample_iid_plus(poly *r, const unsigned char uniformbytes[NTRU_SAMPLE_IID_BYTES]) {
    /* Sample r using PQCLEAN_NTRUHRSS701_CLEAN_sample_iid then conditionally flip    */
    /* signs of even index coefficients so that <x*r, r> >= 0.      */

    int i;
    uint16_t s = 0;

    PQCLEAN_NTRUHRSS701_CLEAN_sample_iid(r, uniformbytes);

    /* Map {0,1,2} -> {0, 1, 2^16 - 1} */
    for (i = 0; i < NTRU_N - 1; i++) {
        r->coeffs[i] = r->coeffs[i] | (-(r->coeffs[i] >> 1));
    }

    /* s = <x*r, r>.  (r[n-1] = 0) */
    for (i = 0; i < NTRU_N - 1; i++) {
        s += (uint16_t)((uint32_t)r->coeffs[i + 1] * (uint32_t)r->coeffs[i]);
    }

    /* Extract sign of s (sign(0) = 1) */
    s = 1 | (-(s >> 15));

    for (i = 0; i < NTRU_N; i += 2) {
        r->coeffs[i] = (uint16_t)((uint32_t)s * (uint32_t)r->coeffs[i]);
    }

    /* Map {0,1,2^16-1} -> {0, 1, 2} */
    for (i = 0; i < NTRU_N; i++) {
        r->coeffs[i] = 3 & (r->coeffs[i] ^ (r->coeffs[i] >> 15));
    }
}

