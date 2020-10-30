#include "crypto_encode_761x4591.h"

/* auto-generated; do not edit */

#define int16 int16_t
#define uint16 uint16_t
#define uint32 uint32_t

void PQCLEAN_SNTRUP761_CLEAN_crypto_encode_761x4591(unsigned char *out, const void *v) {
    const int16 *R0 = v;
    /* XXX: caller could overlap R with input */
    uint16 R[381];
    long i;
    uint16 r0, r1;
    uint32 r2;

    for (i = 0; i < 380; ++i) {
        r0 = (uint16) ((R0[2 * i] + 2295) & 16383);
        r1 = (uint16) ((R0[2 * i + 1] + 2295) & 16383);
        r2 = r0 + r1 * (uint32)4591;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[380] = (uint16) ((R0[760] + 2295) & 16383);

    for (i = 0; i < 190; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)322;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[190] = R[380];

    for (i = 0; i < 95; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)406;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[95] = R[190];

    for (i = 0; i < 48; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)644;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }

    for (i = 0; i < 23; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)1621;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    r0 = R[46];
    r1 = R[47];
    r2 = r0 + r1 * (uint32)1621;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[23] = (uint16) r2;

    for (i = 0; i < 11; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)10265;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    r0 = R[22];
    r1 = R[23];
    r2 = r0 + r1 * (uint32)10265;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[11] = (uint16) r2;

    for (i = 0; i < 5; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)1608;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    r0 = R[10];
    r1 = R[11];
    r2 = r0 + r1 * (uint32)1608;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[5] = (uint16) r2;

    for (i = 0; i < 2; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)10101;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    r0 = R[4];
    r1 = R[5];
    r2 = r0 + r1 * (uint32)10101;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[2] = (uint16) r2;

    r0 = R[0];
    r1 = R[1];
    r2 = r0 + r1 * (uint32)1557;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[0] = (uint16) r2;
    R[1] = R[2];

    r0 = R[0];
    r1 = R[1];
    r2 = r0 + r1 * (uint32)9470;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[0] = (uint16) r2;

    r0 = R[0];
    *out++ = (unsigned char) r0;
    r0 >>= 8;
    *out++ = (unsigned char) r0;
}
