#include <stdint.h>
#include <string.h>

#include "thashx8.h"

#include "address.h"
#include "hash.h"
#include "params.h"
#include "sha2.h"
#include "sha256avx.h"
#include "sha256x8.h"
#include "utils.h"

#include "sha512x4.h"

static void thashx8_512(
    unsigned char *out0,
    unsigned char *out1,
    unsigned char *out2,
    unsigned char *out3,
    unsigned char *out4,
    unsigned char *out5,
    unsigned char *out6,
    unsigned char *out7,
    const unsigned char *in0,
    const unsigned char *in1,
    const unsigned char *in2,
    const unsigned char *in3,
    const unsigned char *in4,
    const unsigned char *in5,
    const unsigned char *in6,
    const unsigned char *in7,
    unsigned int inblocks,
    const spx_ctx *ctx,
    uint32_t addrx8[8 * 8]
);

/**
 * 8-way parallel version of thash; takes 8x as much input and output
 */
void thashx8(unsigned char *out0,
             unsigned char *out1,
             unsigned char *out2,
             unsigned char *out3,
             unsigned char *out4,
             unsigned char *out5,
             unsigned char *out6,
             unsigned char *out7,
             const unsigned char *in0,
             const unsigned char *in1,
             const unsigned char *in2,
             const unsigned char *in3,
             const unsigned char *in4,
             const unsigned char *in5,
             const unsigned char *in6,
             const unsigned char *in7, unsigned int inblocks,
             const spx_ctx *ctx, uint32_t addrx8[8 * 8]) {
    if (inblocks > 1) {
        thashx8_512(
            out0, out1, out2, out3, out4, out5, out6, out7,
            in0, in1, in2, in3, in4, in5, in6, in7,
            inblocks, ctx, addrx8);
        return;
    }
    PQCLEAN_VLA(unsigned char, bufx8, 8 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N));
    unsigned char outbufx8[8 * SPX_SHA256_OUTPUT_BYTES];
    PQCLEAN_VLA(unsigned char, bitmaskx8, 8 * (inblocks * SPX_N));
    unsigned int i;

    for (i = 0; i < 8; i++) {
        memcpy(bufx8 + i * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
               ctx->pub_seed, SPX_N);
        memcpy(bufx8 + SPX_N +
               i * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
               addrx8 + i * 8, SPX_SHA256_ADDR_BYTES);
    }

    mgf1x8(bitmaskx8, inblocks * SPX_N,
           bufx8 + 0 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
           bufx8 + 1 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
           bufx8 + 2 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
           bufx8 + 3 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
           bufx8 + 4 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
           bufx8 + 5 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
           bufx8 + 6 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
           bufx8 + 7 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
           SPX_N + SPX_SHA256_ADDR_BYTES);

    for (i = 0; i < inblocks * SPX_N; i++) {
        bufx8[SPX_N + SPX_SHA256_ADDR_BYTES + i +
                    0 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N)] =
                  in0[i] ^ bitmaskx8[i + 0 * (inblocks * SPX_N)];
        bufx8[SPX_N + SPX_SHA256_ADDR_BYTES + i +
                    1 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N)] =
                  in1[i] ^ bitmaskx8[i + 1 * (inblocks * SPX_N)];
        bufx8[SPX_N + SPX_SHA256_ADDR_BYTES + i +
                    2 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N)] =
                  in2[i] ^ bitmaskx8[i + 2 * (inblocks * SPX_N)];
        bufx8[SPX_N + SPX_SHA256_ADDR_BYTES + i +
                    3 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N)] =
                  in3[i] ^ bitmaskx8[i + 3 * (inblocks * SPX_N)];
        bufx8[SPX_N + SPX_SHA256_ADDR_BYTES + i +
                    4 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N)] =
                  in4[i] ^ bitmaskx8[i + 4 * (inblocks * SPX_N)];
        bufx8[SPX_N + SPX_SHA256_ADDR_BYTES + i +
                    5 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N)] =
                  in5[i] ^ bitmaskx8[i + 5 * (inblocks * SPX_N)];
        bufx8[SPX_N + SPX_SHA256_ADDR_BYTES + i +
                    6 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N)] =
                  in6[i] ^ bitmaskx8[i + 6 * (inblocks * SPX_N)];
        bufx8[SPX_N + SPX_SHA256_ADDR_BYTES + i +
                    7 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N)] =
                  in7[i] ^ bitmaskx8[i + 7 * (inblocks * SPX_N)];
    }

    sha256x8_seeded(
        /* out */
        outbufx8 + 0 * SPX_SHA256_OUTPUT_BYTES,
        outbufx8 + 1 * SPX_SHA256_OUTPUT_BYTES,
        outbufx8 + 2 * SPX_SHA256_OUTPUT_BYTES,
        outbufx8 + 3 * SPX_SHA256_OUTPUT_BYTES,
        outbufx8 + 4 * SPX_SHA256_OUTPUT_BYTES,
        outbufx8 + 5 * SPX_SHA256_OUTPUT_BYTES,
        outbufx8 + 6 * SPX_SHA256_OUTPUT_BYTES,
        outbufx8 + 7 * SPX_SHA256_OUTPUT_BYTES,

        /* seed */
        &ctx->statex8_seeded,

        /* in */
        bufx8 + SPX_N + 0 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + SPX_N + 1 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + SPX_N + 2 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + SPX_N + 3 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + SPX_N + 4 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + SPX_N + 5 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + SPX_N + 6 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + SPX_N + 7 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        SPX_SHA256_ADDR_BYTES + inblocks * SPX_N /* len */
    );

    memcpy(out0, outbufx8 + 0 * SPX_SHA256_OUTPUT_BYTES, SPX_N);
    memcpy(out1, outbufx8 + 1 * SPX_SHA256_OUTPUT_BYTES, SPX_N);
    memcpy(out2, outbufx8 + 2 * SPX_SHA256_OUTPUT_BYTES, SPX_N);
    memcpy(out3, outbufx8 + 3 * SPX_SHA256_OUTPUT_BYTES, SPX_N);
    memcpy(out4, outbufx8 + 4 * SPX_SHA256_OUTPUT_BYTES, SPX_N);
    memcpy(out5, outbufx8 + 5 * SPX_SHA256_OUTPUT_BYTES, SPX_N);
    memcpy(out6, outbufx8 + 6 * SPX_SHA256_OUTPUT_BYTES, SPX_N);
    memcpy(out7, outbufx8 + 7 * SPX_SHA256_OUTPUT_BYTES, SPX_N);
}

/**
 * 2x4-way parallel version of thash; this is for the uses of thash that are
 * based on SHA-512
 */
static void thashx8_512(
    unsigned char *out0,
    unsigned char *out1,
    unsigned char *out2,
    unsigned char *out3,
    unsigned char *out4,
    unsigned char *out5,
    unsigned char *out6,
    unsigned char *out7,
    const unsigned char *in0,
    const unsigned char *in1,
    const unsigned char *in2,
    const unsigned char *in3,
    const unsigned char *in4,
    const unsigned char *in5,
    const unsigned char *in6,
    const unsigned char *in7,
    unsigned int inblocks,
    const spx_ctx *ctx,
    uint32_t addrx8[8 * 8]) {
    PQCLEAN_VLA(unsigned char, bufx8, 8 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N));
    unsigned char outbuf[4 * SPX_SHA512_OUTPUT_BYTES];
    PQCLEAN_VLA(unsigned char, bitmaskx4, 4 * (inblocks * SPX_N));
    unsigned int i;

    for (i = 0; i < 8; i++) {
        memcpy(bufx8 + i * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
               ctx->pub_seed, SPX_N);
        memcpy(bufx8 + SPX_N +
               i * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
               addrx8 + i * 8, SPX_SHA256_ADDR_BYTES);
    }

    mgf1x4_512(bitmaskx4, inblocks * SPX_N,
               bufx8 + 0 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
               bufx8 + 1 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
               bufx8 + 2 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
               bufx8 + 3 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
               SPX_N + SPX_SHA256_ADDR_BYTES);

    for (i = 0; i < inblocks * SPX_N; i++) {
        bufx8[SPX_N + SPX_SHA256_ADDR_BYTES + i +
                    0 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N)] =
                  in0[i] ^ bitmaskx4[i + 0 * (inblocks * SPX_N)];
        bufx8[SPX_N + SPX_SHA256_ADDR_BYTES + i +
                    1 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N)] =
                  in1[i] ^ bitmaskx4[i + 1 * (inblocks * SPX_N)];
        bufx8[SPX_N + SPX_SHA256_ADDR_BYTES + i +
                    2 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N)] =
                  in2[i] ^ bitmaskx4[i + 2 * (inblocks * SPX_N)];
        bufx8[SPX_N + SPX_SHA256_ADDR_BYTES + i +
                    3 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N)] =
                  in3[i] ^ bitmaskx4[i + 3 * (inblocks * SPX_N)];
    }

    mgf1x4_512(bitmaskx4, inblocks * SPX_N,
               bufx8 + 4 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
               bufx8 + 5 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
               bufx8 + 6 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
               bufx8 + 7 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
               SPX_N + SPX_SHA256_ADDR_BYTES);

    for (i = 0; i < inblocks * SPX_N; i++) {
        bufx8[SPX_N + SPX_SHA256_ADDR_BYTES + i +
                    4 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N)] =
                  in4[i] ^ bitmaskx4[i + 0 * (inblocks * SPX_N)];
        bufx8[SPX_N + SPX_SHA256_ADDR_BYTES + i +
                    5 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N)] =
                  in5[i] ^ bitmaskx4[i + 1 * (inblocks * SPX_N)];
        bufx8[SPX_N + SPX_SHA256_ADDR_BYTES + i +
                    6 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N)] =
                  in6[i] ^ bitmaskx4[i + 2 * (inblocks * SPX_N)];
        bufx8[SPX_N + SPX_SHA256_ADDR_BYTES + i +
                    7 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N)] =
                  in7[i] ^ bitmaskx4[i + 3 * (inblocks * SPX_N)];
    }

    sha512x4_seeded(
        outbuf + 0 * SPX_SHA512_OUTPUT_BYTES,
        outbuf + 1 * SPX_SHA512_OUTPUT_BYTES,
        outbuf + 2 * SPX_SHA512_OUTPUT_BYTES,
        outbuf + 3 * SPX_SHA512_OUTPUT_BYTES,
        &ctx->statex4_seeded_512, /* seed */
        bufx8 + SPX_N + 0 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + SPX_N + 1 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + SPX_N + 2 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + SPX_N + 3 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        SPX_SHA256_ADDR_BYTES + inblocks * SPX_N /* len */
    );

    memcpy(out0, outbuf + 0 * SPX_SHA512_OUTPUT_BYTES, SPX_N);
    memcpy(out1, outbuf + 1 * SPX_SHA512_OUTPUT_BYTES, SPX_N);
    memcpy(out2, outbuf + 2 * SPX_SHA512_OUTPUT_BYTES, SPX_N);
    memcpy(out3, outbuf + 3 * SPX_SHA512_OUTPUT_BYTES, SPX_N);

    sha512x4_seeded(
        outbuf + 0 * SPX_SHA512_OUTPUT_BYTES,
        outbuf + 1 * SPX_SHA512_OUTPUT_BYTES,
        outbuf + 2 * SPX_SHA512_OUTPUT_BYTES,
        outbuf + 3 * SPX_SHA512_OUTPUT_BYTES,
        &ctx->statex4_seeded_512, /* seed */
        bufx8 + SPX_N + 4 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + SPX_N + 5 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + SPX_N + 6 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + SPX_N + 7 * (SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        SPX_SHA256_ADDR_BYTES + inblocks * SPX_N /* len */
    );

    memcpy(out4, outbuf + 0 * SPX_SHA512_OUTPUT_BYTES, SPX_N);
    memcpy(out5, outbuf + 1 * SPX_SHA512_OUTPUT_BYTES, SPX_N);
    memcpy(out6, outbuf + 2 * SPX_SHA512_OUTPUT_BYTES, SPX_N);
    memcpy(out7, outbuf + 3 * SPX_SHA512_OUTPUT_BYTES, SPX_N);
}
