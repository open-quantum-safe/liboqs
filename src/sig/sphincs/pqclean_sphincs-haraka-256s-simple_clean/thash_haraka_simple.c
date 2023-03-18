#include <stdint.h>
#include <string.h>

#include "address.h"
#include "params.h"
#include "thash.h"
#include "utils.h"

#include "haraka.h"

/**
 * Takes an array of inblocks concatenated arrays of SPX_N bytes.
 */
void thash(unsigned char *out, const unsigned char *in, unsigned int inblocks,
           const spx_ctx *ctx, uint32_t addr[8]) {
    PQCLEAN_VLA(uint8_t, buf, SPX_ADDR_BYTES + inblocks * SPX_N);
    unsigned char outbuf[32];
    unsigned char buf_tmp[64];

    if (inblocks == 1) {
        /* F function */
        /* Since SPX_N may be smaller than 32, we need a temporary buffer. */
        memset(buf_tmp, 0, 64);
        memcpy(buf_tmp, addr, 32);
        memcpy(buf_tmp + SPX_ADDR_BYTES, in, SPX_N);

        haraka512(outbuf, buf_tmp, ctx);
        memcpy(out, outbuf, SPX_N);
    } else {
        /* All other tweakable hashes*/
        memcpy(buf, addr, 32);
        memcpy(buf + SPX_ADDR_BYTES, in, inblocks * SPX_N);

        haraka_S(out, SPX_N, buf, SPX_ADDR_BYTES + inblocks * SPX_N, ctx);
    }
}
