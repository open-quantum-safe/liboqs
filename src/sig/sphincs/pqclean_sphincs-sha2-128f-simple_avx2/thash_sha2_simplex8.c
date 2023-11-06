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
    PQCLEAN_VLA(unsigned char, bufx8, 8 * (SPX_SHA256_ADDR_BYTES + inblocks * SPX_N));
    unsigned char outbufx8[8 * SPX_SHA256_OUTPUT_BYTES];
    unsigned int i;

    for (i = 0; i < 8; i++) {
        memcpy(bufx8 + i * (SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
               addrx8 + i * 8, SPX_SHA256_ADDR_BYTES);
    }

    memcpy(bufx8 + SPX_SHA256_ADDR_BYTES +
           0 * (SPX_SHA256_ADDR_BYTES + inblocks * SPX_N), in0, inblocks * SPX_N);
    memcpy(bufx8 + SPX_SHA256_ADDR_BYTES +
           1 * (SPX_SHA256_ADDR_BYTES + inblocks * SPX_N), in1, inblocks * SPX_N);
    memcpy(bufx8 + SPX_SHA256_ADDR_BYTES +
           2 * (SPX_SHA256_ADDR_BYTES + inblocks * SPX_N), in2, inblocks * SPX_N);
    memcpy(bufx8 + SPX_SHA256_ADDR_BYTES +
           3 * (SPX_SHA256_ADDR_BYTES + inblocks * SPX_N), in3, inblocks * SPX_N);
    memcpy(bufx8 + SPX_SHA256_ADDR_BYTES +
           4 * (SPX_SHA256_ADDR_BYTES + inblocks * SPX_N), in4, inblocks * SPX_N);
    memcpy(bufx8 + SPX_SHA256_ADDR_BYTES +
           5 * (SPX_SHA256_ADDR_BYTES + inblocks * SPX_N), in5, inblocks * SPX_N);
    memcpy(bufx8 + SPX_SHA256_ADDR_BYTES +
           6 * (SPX_SHA256_ADDR_BYTES + inblocks * SPX_N), in6, inblocks * SPX_N);
    memcpy(bufx8 + SPX_SHA256_ADDR_BYTES +
           7 * (SPX_SHA256_ADDR_BYTES + inblocks * SPX_N), in7, inblocks * SPX_N);

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
        bufx8 + 0 * (SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + 1 * (SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + 2 * (SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + 3 * (SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + 4 * (SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + 5 * (SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + 6 * (SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
        bufx8 + 7 * (SPX_SHA256_ADDR_BYTES + inblocks * SPX_N),
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
