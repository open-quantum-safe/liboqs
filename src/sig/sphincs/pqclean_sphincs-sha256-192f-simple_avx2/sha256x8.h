#ifndef PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256X8_H
#define PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256X8_H

#include "sha256avx.h"

#define PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_BLOCK_BYTES 64
#define PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_SHA256_OUTPUT_BYTES 32  /* This does not necessarily equal PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_N */

/* This provides a wrapper around the internals of 8x parallel SHA256 */
void PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_sha256x8(unsigned char *out0,
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
        const unsigned char *in7, unsigned long long inlen);

/**
 * Note that inlen should be sufficiently small that it still allows for
 * an array to be allocated on the stack. Typically 'in' is merely a seed.
 * Outputs outlen number of bytes
 */
void PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_mgf1x8(unsigned char *outx8, unsigned long outlen,
        const unsigned char *in0,
        const unsigned char *in1,
        const unsigned char *in2,
        const unsigned char *in3,
        const unsigned char *in4,
        const unsigned char *in5,
        const unsigned char *in6,
        const unsigned char *in7,
        unsigned long inlen);

void PQCLEAN_SPHINCSSHA256192FSIMPLE_AVX2_seed_statex8(sha256ctxx8 *ctx, const unsigned char *pub_seed);
#endif
