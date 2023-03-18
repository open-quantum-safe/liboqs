#include <stdint.h>
#include <string.h>

#include "address.h"
#include "params.h"
#include "thashx4.h"

#include "harakax4.h"

#include "utils.h"

/**
 * 4-way parallel version of thash; takes 4x as much input and output
 */
#define thashx4 SPX_NAMESPACE(thashx4)
void thashx4(unsigned char *out0,
             unsigned char *out1,
             unsigned char *out2,
             unsigned char *out3,
             const unsigned char *in0,
             const unsigned char *in1,
             const unsigned char *in2,
             const unsigned char *in3, unsigned int inblocks,
             const spx_ctx *ctx, uint32_t addrx4[4 * 8]) {
    PQCLEAN_VLA(unsigned char, buf0, SPX_ADDR_BYTES + inblocks * SPX_N);
    PQCLEAN_VLA(unsigned char, buf1, SPX_ADDR_BYTES + inblocks * SPX_N);
    PQCLEAN_VLA(unsigned char, buf2, SPX_ADDR_BYTES + inblocks * SPX_N);
    PQCLEAN_VLA(unsigned char, buf3, SPX_ADDR_BYTES + inblocks * SPX_N);
    unsigned char outbuf[32 * 4];
    unsigned char buf_tmp[64 * 4];

    if (inblocks == 1) {
        memset(buf_tmp, 0, 64 * 4);

        memcpy(buf_tmp,       addrx4 + 0 * 8, 32);
        memcpy(buf_tmp + 64,  addrx4 + 1 * 8, 32);
        memcpy(buf_tmp + 128, addrx4 + 2 * 8, 32);
        memcpy(buf_tmp + 192, addrx4 + 3 * 8, 32);

        memcpy(buf_tmp + SPX_ADDR_BYTES,       in0, SPX_N);
        memcpy(buf_tmp + SPX_ADDR_BYTES + 64,  in1, SPX_N);
        memcpy(buf_tmp + SPX_ADDR_BYTES + 128, in2, SPX_N);
        memcpy(buf_tmp + SPX_ADDR_BYTES + 192, in3, SPX_N);

        haraka512x4(outbuf, buf_tmp, ctx);

        memcpy(out0, outbuf,      SPX_N);
        memcpy(out1, outbuf + 32, SPX_N);
        memcpy(out2, outbuf + 64, SPX_N);
        memcpy(out3, outbuf + 96, SPX_N);
    } else {
        /* All other tweakable hashes*/
        memcpy(buf0, addrx4 + 0 * 8, 32);
        memcpy(buf1, addrx4 + 1 * 8, 32);
        memcpy(buf2, addrx4 + 2 * 8, 32);
        memcpy(buf3, addrx4 + 3 * 8, 32);

        memcpy(buf0 + SPX_ADDR_BYTES, in0, inblocks * SPX_N);
        memcpy(buf1 + SPX_ADDR_BYTES, in1, inblocks * SPX_N);
        memcpy(buf2 + SPX_ADDR_BYTES, in2, inblocks * SPX_N);
        memcpy(buf3 + SPX_ADDR_BYTES, in3, inblocks * SPX_N);

        haraka_Sx4(out0, out1, out2, out3, SPX_N,
                   buf0, buf1, buf2, buf3, SPX_ADDR_BYTES + inblocks * SPX_N,
                   ctx);
    }
}
