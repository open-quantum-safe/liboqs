#include "crypto_encode_653x4621.h"

/* auto-generated; do not edit */

#define int16 int16_t
#define uint16 uint16_t
#define uint32 uint32_t

void PQCLEAN_SNTRUP653_CLEAN_crypto_encode_653x4621(unsigned char *out, const void *v) {
    const int16 *R0 = v;
    /* XXX: caller could overlap R with input */
    uint16 R[327];
    long i;
    uint16 r0, r1;
    uint32 r2;

    for (i = 0; i < 326; ++i) {
        r0 = (uint16) ((R0[2 * i] + 2310) & 16383);
        r1 = (uint16) ((R0[2 * i + 1] + 2310) & 16383);
        r2 = r0 + r1 * (uint32)4621;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[326] = (uint16) ((R0[652] + 2310) & 16383);

    for (i = 0; i < 163; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)326;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[163] = R[326];

    for (i = 0; i < 82; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)416;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }

    for (i = 0; i < 40; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)676;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    r0 = R[80];
    r1 = R[81];
    r2 = r0 + r1 * (uint32)676;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[40] = (uint16) r2;

    for (i = 0; i < 20; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)1786;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[20] = R[40];

    for (i = 0; i < 10; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)12461;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[10] = R[20];

    for (i = 0; i < 5; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)2370;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[5] = R[10];

    for (i = 0; i < 3; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)86;
        R[i] = (uint16) r2;
    }

    r0 = R[0];
    r1 = R[1];
    r2 = r0 + r1 * (uint32)7396;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[0] = (uint16) r2;
    R[1] = R[2];

    r0 = R[0];
    r1 = R[1];
    r2 = r0 + r1 * (uint32)835;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[0] = (uint16) r2;

    r0 = R[0];
    *out++ = (unsigned char) r0;
}
