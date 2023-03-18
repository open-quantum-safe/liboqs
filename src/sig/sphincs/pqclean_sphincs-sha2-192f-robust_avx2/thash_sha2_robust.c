#include <stdint.h>
#include <string.h>

#include "address.h"
#include "context.h"
#include "hash.h"
#include "params.h"
#include "sha2.h"
#include "thash.h"
#include "utils.h"

static void thash_512(unsigned char *out, const unsigned char *in, unsigned int inblocks,
                      const spx_ctx *ctx, uint32_t addr[8]);

/**
 * Takes an array of inblocks concatenated arrays of SPX_N bytes.
 */
void thash(unsigned char *out, const unsigned char *in, unsigned int inblocks,
           const spx_ctx *ctx, uint32_t addr[8]) {
    if (inblocks > 1) {
        thash_512(out, in, inblocks, ctx, addr);
        return;
    }
    unsigned char outbuf[SPX_SHA256_OUTPUT_BYTES];
    PQCLEAN_VLA(uint8_t, bitmask, inblocks * SPX_N);
    PQCLEAN_VLA(uint8_t, buf, SPX_N + SPX_SHA256_OUTPUT_BYTES + inblocks * SPX_N);
    sha256ctx sha2_state;
    unsigned int i;

    memcpy(buf, ctx->pub_seed, SPX_N);
    memcpy(buf + SPX_N, addr, SPX_SHA256_ADDR_BYTES);
    mgf1_256(bitmask, inblocks * SPX_N, buf, SPX_N + SPX_SHA256_ADDR_BYTES);

    /* Retrieve precomputed state containing pub_seed */
    sha256_inc_ctx_clone(&sha2_state, &ctx->state_seeded);

    for (i = 0; i < inblocks * SPX_N; i++) {
        buf[SPX_N + SPX_SHA256_ADDR_BYTES + i] = in[i] ^ bitmask[i];
    }

    sha256_inc_finalize(outbuf, &sha2_state, buf + SPX_N,
                        SPX_SHA256_ADDR_BYTES + inblocks * SPX_N);
    memcpy(out, outbuf, SPX_N);
}

static void thash_512(unsigned char *out, const unsigned char *in, unsigned int inblocks,
                      const spx_ctx *ctx, uint32_t addr[8]) {
    unsigned char outbuf[SPX_SHA512_OUTPUT_BYTES];
    PQCLEAN_VLA(uint8_t, bitmask, inblocks * SPX_N);
    PQCLEAN_VLA(uint8_t, buf, SPX_N + SPX_SHA256_ADDR_BYTES + inblocks * SPX_N);
    sha512ctx sha2_state;
    unsigned int i;

    memcpy(buf, ctx->pub_seed, SPX_N);
    memcpy(buf + SPX_N, addr, SPX_SHA256_ADDR_BYTES);
    mgf1_512(bitmask, inblocks * SPX_N, buf, SPX_N + SPX_SHA256_ADDR_BYTES);

    /* Retrieve precomputed state containing pub_seed */
    sha512_inc_ctx_clone(&sha2_state, &ctx->state_seeded_512);

    for (i = 0; i < inblocks * SPX_N; i++) {
        buf[SPX_N + SPX_SHA256_ADDR_BYTES + i] = in[i] ^ bitmask[i];
    }

    sha512_inc_finalize(outbuf, &sha2_state, buf + SPX_N,
                        SPX_SHA256_ADDR_BYTES + inblocks * SPX_N);
    memcpy(out, outbuf, SPX_N);
}
