#ifndef SPX_SHA256X8_H
#define SPX_SHA256X8_H

#include "params.h"
#include "sha256avx.h"

#define sha256x8_seeded SPX_NAMESPACE(sha256x8_seeded)
void sha256x8_seeded(
    unsigned char *out0,
    unsigned char *out1,
    unsigned char *out2,
    unsigned char *out3,
    unsigned char *out4,
    unsigned char *out5,
    unsigned char *out6,
    unsigned char *out7,
    const sha256x8ctx *seed,
    const unsigned char *in0,
    const unsigned char *in1,
    const unsigned char *in2,
    const unsigned char *in3,
    const unsigned char *in4,
    const unsigned char *in5,
    const unsigned char *in6,
    const unsigned char *in7, unsigned long long inlen);

/* This provides a wrapper around the internals of 8x parallel SHA256 */
#define sha256x8 SPX_NAMESPACE(sha256x8)
void sha256x8(unsigned char *out0,
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
#define mgf1x8 SPX_NAMESPACE(mgf1x8)
void mgf1x8(unsigned char *outx8, unsigned long outlen,
            const unsigned char *in0,
            const unsigned char *in1,
            const unsigned char *in2,
            const unsigned char *in3,
            const unsigned char *in4,
            const unsigned char *in5,
            const unsigned char *in6,
            const unsigned char *in7,
            unsigned long inlen);
#endif
