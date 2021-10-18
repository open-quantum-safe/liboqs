#include "crypto_encode_1277x2627.h"

/* auto-generated; do not edit */

#define int16 int16_t
#define uint16 uint16_t
#define uint32 uint32_t

void PQCLEAN_NTRULPR1277_CLEAN_crypto_encode_1277x2627(unsigned char *out, const void *v) {
    const int16 *R0 = v;
    /* XXX: caller could overlap R with input */
    uint16 R[639];
    long i;
    uint16 r0, r1;
    uint32 r2;

    for (i = 0; i < 638; ++i) {
        r0 = (uint16) ((((R0[2 * i] + 3939) & 16383) * 10923) >> 15);
        r1 = (uint16) ((((R0[2 * i + 1] + 3939) & 16383) * 10923) >> 15);
        r2 = r0 + r1 * (uint32)2627;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[638] = (uint16) ((((R0[1276] + 3939) & 16383) * 10923) >> 15);

    for (i = 0; i < 319; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)106;
        R[i] = (uint16) r2;
    }
    R[319] = R[638];

    for (i = 0; i < 160; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)11236;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }

    for (i = 0; i < 80; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)1927;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }

    for (i = 0; i < 40; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)14506;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }

    for (i = 0; i < 19; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)3211;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    r0 = R[38];
    r1 = R[39];
    r2 = r0 + r1 * (uint32)3211;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[19] = (uint16) r2;

    for (i = 0; i < 10; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)158;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }

    for (i = 0; i < 4; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)98;
        R[i] = (uint16) r2;
    }
    r0 = R[8];
    r1 = R[9];
    r2 = r0 + r1 * (uint32)98;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[4] = (uint16) r2;

    for (i = 0; i < 2; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)9604;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[2] = R[4];

    r0 = R[0];
    r1 = R[1];
    r2 = r0 + r1 * (uint32)1408;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[0] = (uint16) r2;
    R[1] = R[2];

    r0 = R[0];
    r1 = R[1];
    r2 = r0 + r1 * (uint32)7744;
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
