#ifndef SPX_HARAKAX4_H
#define SPX_HARAKAX4_H

#include "context.h"
#include "params.h"

/* Haraka Sponge */
#define haraka_Sx4 SPX_NAMESPACE(haraka_Sx4)
void haraka_Sx4(unsigned char *out0,
                unsigned char *out1,
                unsigned char *out2,
                unsigned char *out3,
                unsigned long long outlen,
                const unsigned char *in0,
                const unsigned char *in1,
                const unsigned char *in2,
                const unsigned char *in3,
                unsigned long long inlen,
                const spx_ctx *ctx);

/* Applies the 512-bit Haraka permutation x4 to in. */
#define haraka512_perm_x4 SPX_NAMESPACE(haraka512_perm_x4)
void haraka512_perm_x4(unsigned char *out, const unsigned char *in,
                       const spx_ctx *ctx);

/* Implementation of Haraka-512 x4*/
#define haraka512x4 SPX_NAMESPACE(haraka512x4)
void haraka512x4(unsigned char *out, const unsigned char *in,
                 const spx_ctx *ctx);

/* Implementation of Haraka-256 x4 */
#define haraka256x4 SPX_NAMESPACE(haraka256x4)
void haraka256x4(unsigned char *out, const unsigned char *in,
                 const spx_ctx *ctx);

#endif
