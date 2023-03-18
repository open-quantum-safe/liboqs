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
    PQCLEAN_VLA(unsigned char, bitmask0, inblocks * SPX_N);
    PQCLEAN_VLA(unsigned char, bitmask1, inblocks * SPX_N);
    PQCLEAN_VLA(unsigned char, bitmask2, inblocks * SPX_N);
    PQCLEAN_VLA(unsigned char, bitmask3, inblocks * SPX_N);
    unsigned char outbuf[32 * 4];
    unsigned char buf_tmp[64 * 4];
    unsigned int i;

    if (inblocks == 1) {
        memset(buf_tmp, 0, 64 * 4);

        // Generate masks first in buffer
        memcpy(buf_tmp,      addrx4 + 0 * 8, 32);
        memcpy(buf_tmp + 32, addrx4 + 1 * 8, 32);
        memcpy(buf_tmp + 64, addrx4 + 2 * 8, 32);
        memcpy(buf_tmp + 96, addrx4 + 3 * 8, 32);

        haraka256x4(outbuf, buf_tmp, ctx);

        /* move addresses to make room for inputs; zero old values */
        memcpy(buf_tmp + 192, buf_tmp + 96, SPX_ADDR_BYTES);
        memcpy(buf_tmp + 128, buf_tmp + 64, SPX_ADDR_BYTES);
        memcpy(buf_tmp + 64,  buf_tmp + 32, SPX_ADDR_BYTES);
        /* skip memcpy(buf_tmp, buf_tmp, SPX_ADDR_BYTES); already in place */

        /* skip memset(buf_tmp, 0, SPX_ADDR_BYTES); remained untouched */
        memset(buf_tmp + 32, 0, SPX_ADDR_BYTES);
        /* skip memset(buf_tmp + 64, 0, SPX_ADDR_BYTES); contains addr1 */
        memset(buf_tmp + 96, 0, SPX_ADDR_BYTES);

        for (i = 0; i < SPX_N; i++) {
            buf_tmp[SPX_ADDR_BYTES + i]       = in0[i] ^ outbuf[i];
            buf_tmp[SPX_ADDR_BYTES + i + 64]  = in1[i] ^ outbuf[i + 32];
            buf_tmp[SPX_ADDR_BYTES + i + 128] = in2[i] ^ outbuf[i + 64];
            buf_tmp[SPX_ADDR_BYTES + i + 192] = in3[i] ^ outbuf[i + 96];
        }

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

        haraka_Sx4(bitmask0, bitmask1, bitmask2, bitmask3, inblocks * SPX_N,
                   buf0, buf1, buf2, buf3, SPX_ADDR_BYTES, ctx);

        for (i = 0; i < inblocks * SPX_N; i++) {
            buf0[SPX_ADDR_BYTES + i] = in0[i] ^ bitmask0[i];
            buf1[SPX_ADDR_BYTES + i] = in1[i] ^ bitmask1[i];
            buf2[SPX_ADDR_BYTES + i] = in2[i] ^ bitmask2[i];
            buf3[SPX_ADDR_BYTES + i] = in3[i] ^ bitmask3[i];
        }

        haraka_Sx4(out0, out1, out2, out3, SPX_N,
                   buf0, buf1, buf2, buf3, SPX_ADDR_BYTES + inblocks * SPX_N,
                   ctx);
    }
}
