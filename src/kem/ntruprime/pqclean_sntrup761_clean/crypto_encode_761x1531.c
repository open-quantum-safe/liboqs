#include "crypto_encode_761x1531.h"

/* auto-generated; do not edit */

#define int16 int16_t
#define uint16 uint16_t
#define uint32 uint32_t

void PQCLEAN_SNTRUP761_CLEAN_crypto_encode_761x1531(unsigned char *out, const void *v) {
    const int16 *R0 = v;
    /* XXX: caller could overlap R with input */
    uint16 R[381];
    long i;
    uint16 r0, r1;
    uint32 r2;

    for (i = 0; i < 380; ++i) {
        r0 = (uint16) ((((R0[2 * i] + 2295) & 16383) * 10923) >> 15);
        r1 = (uint16) ((((R0[2 * i + 1] + 2295) & 16383) * 10923) >> 15);
        r2 = r0 + r1 * (uint32)1531;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[380] = (uint16) ((((R0[760] + 2295) & 16383) * 10923) >> 15);

    for (i = 0; i < 190; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)9157;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[190] = R[380];

    for (i = 0; i < 95; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)1280;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[95] = R[190];

    for (i = 0; i < 48; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)6400;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }

    for (i = 0; i < 24; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)625;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }

    for (i = 0; i < 12; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)1526;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }

    for (i = 0; i < 6; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)9097;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }

    for (i = 0; i < 3; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)1263;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }

    r0 = R[0];
    r1 = R[1];
    r2 = r0 + r1 * (uint32)6232;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[0] = (uint16) r2;
    R[1] = R[2];

    r0 = R[0];
    r1 = R[1];
    r2 = r0 + r1 * (uint32)593;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[0] = (uint16) r2;

    r0 = R[0];
    *out++ = (unsigned char) r0;
    r0 >>= 8;
    *out++ = (unsigned char) r0;
}
