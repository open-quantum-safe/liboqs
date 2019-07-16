#include <stdint.h>

#include "decode.h"
#include "params.h"
#include "uint32.h"

extern void PQCLEAN_NTRULPR761_CLEAN_Decode(uint16_t *out, const unsigned char *S, const uint16_t *M, long long len) {
    if (len == 1) {
        if (M[0] == 1) {
            *out = 0;
        } else if (M[0] <= 256) {
            *out = PQCLEAN_NTRULPR761_CLEAN_uint32_mod_uint14(S[0], M[0]);
        } else {
            *out = PQCLEAN_NTRULPR761_CLEAN_uint32_mod_uint14(S[0] + (((uint16_t)S[1]) << 8), M[0]);
        }
    }
    if (len > 1) {
        uint16_t R2[(p + 1) / 2];
        uint16_t M2[(p + 1) / 2];
        uint16_t bottomr[p / 2];
        uint32_t bottomt[p / 2];
        long long i;
        for (i = 0; i < len - 1; i += 2) {
            uint32_t m = M[i] * (uint32_t) M[i + 1];
            if (m > 256 * 16383) {
                bottomt[i / 2] = 256 * 256;
                bottomr[i / 2] = S[0] + 256 * S[1];
                S += 2;
                M2[i / 2] = (uint16_t) ((((m + 255) >> 8) + 255) >> 8);
            } else if (m >= 16384) {
                bottomt[i / 2] = 256;
                bottomr[i / 2] = S[0];
                S += 1;
                M2[i / 2] = (uint16_t) ((m + 255) >> 8);
            } else {
                bottomt[i / 2] = 1;
                bottomr[i / 2] = 0;
                M2[i / 2] = (uint16_t) m;
            }
        }
        if (i < len) {
            M2[i / 2] = M[i];
        }
        PQCLEAN_NTRULPR761_CLEAN_Decode(R2, S, M2, (len + 1) / 2);
        for (i = 0; i < len - 1; i += 2) {
            uint32_t r = bottomr[i / 2];
            uint32_t r1;
            uint16_t r0;
            r += bottomt[i / 2] * R2[i / 2];
            PQCLEAN_NTRULPR761_CLEAN_uint32_divmod_uint14(&r1, &r0, r, M[i]);
            r1 = PQCLEAN_NTRULPR761_CLEAN_uint32_mod_uint14(r1, M[i + 1]); /* only needed for invalid inputs */
            *out++ = r0;
            *out++ = (uint16_t) r1;
        }
        if (i < len) {
            *out++ = R2[i / 2];
        }
    }
}
