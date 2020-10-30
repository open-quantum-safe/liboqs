#include "crypto_encode_653x1541.h"

/* auto-generated; do not edit */

#define int16 int16_t
#define uint16 uint16_t
#define uint32 uint32_t

void PQCLEAN_SNTRUP653_CLEAN_crypto_encode_653x1541(unsigned char *out, const void *v) {
    const int16 *R0 = v;
    /* XXX: caller could overlap R with input */
    uint16 R[327];
    long i;
    uint16 r0, r1;
    uint32 r2;

    for (i = 0; i < 326; ++i) {
        r0 = (uint16) ((((R0[2 * i] + 2310) & 16383) * 10923) >> 15);
        r1 = (uint16) ((((R0[2 * i + 1] + 2310) & 16383) * 10923) >> 15);
        r2 = r0 + r1 * (uint32)1541;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[326] = (uint16) ((((R0[652] + 2310) & 16383) * 10923) >> 15);

    for (i = 0; i < 163; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)9277;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[163] = R[326];

    for (i = 0; i < 82; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)1314;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }

    for (i = 0; i < 41; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)6745;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }

    for (i = 0; i < 20; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)695;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[20] = R[40];

    for (i = 0; i < 10; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)1887;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[10] = R[20];

    for (i = 0; i < 5; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)13910;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[5] = R[10];

    for (i = 0; i < 2; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)2953;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    r0 = R[4];
    r1 = R[5];
    r2 = r0 + r1 * (uint32)2953;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[2] = (uint16) r2;

    r0 = R[0];
    r1 = R[1];
    r2 = r0 + r1 * (uint32)134;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[0] = (uint16) r2;
    R[1] = R[2];

    r0 = R[0];
    r1 = R[1];
    r2 = r0 + r1 * (uint32)71;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[0] = (uint16) r2;

    r0 = R[0];
    *out++ = (unsigned char) r0;
    r0 >>= 8;
    *out++ = (unsigned char) r0;
}
