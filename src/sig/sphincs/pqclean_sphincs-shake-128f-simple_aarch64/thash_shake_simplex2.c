#include <stdint.h>
#include <string.h>

#include "thash.h"
#include "thashx2.h"

#include "address.h"
#include "params.h"
#include "utils.h"

#include "f1600x2.h"
#include "fips202x2.h"


void thash(unsigned char *out,
           const unsigned char *in,
           unsigned int inblocks,
           const spx_ctx *ctx, uint32_t addr[8]) {
    uint32_t addrx2 [2 * 8] = {
        addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], addr[6], addr[7],
        addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], addr[6], addr[7]
    };
    thashx2(out, out, in, in, inblocks, ctx, addrx2);
}

/**
 * 2-way parallel version of thash; takes 2x as much input and output
 */
void thashx2(unsigned char *out0,
             unsigned char *out1,
             const unsigned char *in0,
             const unsigned char *in1,
             unsigned int inblocks,
             const spx_ctx *ctx, uint32_t addrx2[2 * 8]) {
    if (inblocks == 1 || inblocks == 2) {
        /* As we write and read only a few quadwords, it is more efficient to
         * build and extract from the twoway SHAKE256 state by hand. */
        uint64_t state[50] = {0};
        for (int i = 0; i < SPX_N / 8; i++) {
            uint64_t x = load64(ctx->pub_seed + 8 * i);
            state[2 * i] = x;
            state[2 * i + 1] = x;
        }
        for (int i = 0; i < 4; i++) {
            state[2 * (SPX_N / 8 + i)] = (((uint64_t)addrx2[1 + 2 * i]) << 32)
                                         | (uint64_t)addrx2[2 * i];
            state[2 * (SPX_N / 8 + i) + 1] = (((uint64_t)addrx2[8 + 1 + 2 * i]) << 32)
                                             | (uint64_t)addrx2[8 + 2 * i];
        }

        for (unsigned int i = 0; i < (SPX_N / 8) * inblocks; i++) {
            state[2 * (SPX_N / 8 + 4 + i)] = load64(in0 + 8 * i);
            state[2 * (SPX_N / 8 + 4 + i) + 1] = load64(in1 + 8 * i);
        }

        /* Domain separator and padding. */
        state[2 * 16] = 0x80ULL << 56;
        state[2 * 16 + 1] = 0x80ULL << 56;

        state[2 * ((SPX_N / 8) * (1 + inblocks) + 4)] ^= 0x1f;
        state[2 * ((SPX_N / 8) * (1 + inblocks) + 4) + 1] ^= 0x1f;

        f1600x2(state);

        for (int i = 0; i < SPX_N / 8; i++) {
            store64(out0 + 8 * i, state[2 * i]);
            store64(out1 + 8 * i, state[2 * i + 1]);
        }
    } else {
        PQCLEAN_VLA(unsigned char, buf0, SPX_N + SPX_ADDR_BYTES + inblocks * SPX_N);
        PQCLEAN_VLA(unsigned char, buf1, SPX_N + SPX_ADDR_BYTES + inblocks * SPX_N);

        memcpy(buf0, ctx->pub_seed, SPX_N);
        memcpy(buf1, ctx->pub_seed, SPX_N);
        memcpy(buf0 + SPX_N, addrx2 + 0 * 8, SPX_ADDR_BYTES);
        memcpy(buf1 + SPX_N, addrx2 + 1 * 8, SPX_ADDR_BYTES);
        memcpy(buf0 + SPX_N + SPX_ADDR_BYTES, in0, inblocks * SPX_N);
        memcpy(buf1 + SPX_N + SPX_ADDR_BYTES, in1, inblocks * SPX_N);

        shake256x2(out0, out1, SPX_N,
                   buf0, buf1, SPX_N + SPX_ADDR_BYTES + inblocks * SPX_N);
    }
}
