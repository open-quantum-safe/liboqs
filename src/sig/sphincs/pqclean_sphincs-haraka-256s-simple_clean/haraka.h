#ifndef SPX_HARAKA_H
#define SPX_HARAKA_H

#include "context.h"
#include "params.h"

/* Tweak constants with seed */
#define tweak_constants SPX_NAMESPACE(tweak_constants)
void tweak_constants(spx_ctx *ctx);

/* Haraka Sponge */
#define haraka_S_inc_init SPX_NAMESPACE(haraka_S_inc_init)
void haraka_S_inc_init(uint8_t *s_inc);
#define haraka_S_inc_absorb SPX_NAMESPACE(haraka_S_inc_absorb)
void haraka_S_inc_absorb(uint8_t *s_inc, const uint8_t *m, size_t mlen,
                         const spx_ctx *ctx);
#define haraka_S_inc_finalize SPX_NAMESPACE(haraka_S_inc_finalize)
void haraka_S_inc_finalize(uint8_t *s_inc);
#define haraka_S_inc_squeeze SPX_NAMESPACE(haraka_S_inc_squeeze)
void haraka_S_inc_squeeze(uint8_t *out, size_t outlen, uint8_t *s_inc,
                          const spx_ctx *ctx);
#define haraka_S SPX_NAMESPACE(haraka_S)
void haraka_S(unsigned char *out, unsigned long long outlen,
              const unsigned char *in, unsigned long long inlen,
              const spx_ctx *ctx);

/* Applies the 512-bit Haraka permutation to in. */
#define haraka512_perm SPX_NAMESPACE(haraka512_perm)
void haraka512_perm(unsigned char *out, const unsigned char *in,
                    const spx_ctx *ctx);

/* Implementation of Haraka-512 */
#define haraka512 SPX_NAMESPACE(haraka512)
void haraka512(unsigned char *out, const unsigned char *in,
               const spx_ctx *ctx);

/* Implementation of Haraka-256 */
#define haraka256 SPX_NAMESPACE(haraka256)
void haraka256(unsigned char *out, const unsigned char *in,
               const spx_ctx *ctx);

#endif
