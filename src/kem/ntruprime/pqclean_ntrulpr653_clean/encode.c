#include <stdint.h>

#include "encode.h"
#include "params.h"

/* 0 <= R[i] < M[i] < 16384 */
void PQCLEAN_NTRULPR653_CLEAN_Encode(unsigned char *out, const uint16_t *R, const uint16_t *M, long long len) {
    if (len == 1) {
        uint16_t r = R[0];
        uint16_t m = M[0];
        while (m > 1) {
            *out++ = (unsigned char) r;
            r >>= 8;
            m = (m + 255) >> 8;
        }
    }
    if (len > 1) {
        uint16_t R2[(p + 1) / 2];
        uint16_t M2[(p + 1) / 2];
        long long i;
        for (i = 0; i < len - 1; i += 2) {
            uint32_t m0 = M[i];
            uint32_t r = R[i] + R[i + 1] * m0;
            uint32_t m = M[i + 1] * m0;
            while (m >= 16384) {
                *out++ = (unsigned char) r;
                r >>= 8;
                m = (m + 255) >> 8;
            }
            R2[i / 2] = (uint16_t) r;
            M2[i / 2] = (uint16_t) m;
        }
        if (i < len) {
            R2[i / 2] = R[i];
            M2[i / 2] = M[i];
        }
        PQCLEAN_NTRULPR653_CLEAN_Encode(out, R2, M2, (len + 1) / 2);
    }
}
