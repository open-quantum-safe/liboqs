#include <stdint.h>
#include <string.h>

#include "hashx2.h"

#include "address.h"
#include "f1600x2.h"
#include "fips202x2.h"
#include "params.h"

/*
 * 2-way parallel version of prf_addr; takes 2x as much input and output
 */
void prf_addrx2(unsigned char *out0,
                unsigned char *out1,
                const spx_ctx *ctx,
                const uint32_t addrx2[2 * 8]) {
    /* As we write and read only a few quadwords, it is more efficient to
     * build and extract from the fourway SHAKE256 state by hand. */
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
    for (int i = 0; i < SPX_N / 8; i++) {
        uint64_t x = load64(ctx->sk_seed + 8 * i);
        state[2 * (SPX_N / 8 + i + 4)] = x;
        state[2 * (SPX_N / 8 + i + 4) + 1] = x;
    }

    /* SHAKE domain separator and padding. */
    state[2 * (SPX_N / 4 + 4)] = 0x1f;
    state[2 * (SPX_N / 4 + 4) + 1] = 0x1f;

    state[2 * 16] = 0x80ULL << 56;
    state[2 * 16 + 1] = 0x80ULL << 56;

    f1600x2(state);

    for (int i = 0; i < SPX_N / 8; i++) {
        store64(out0 + 8 * i, state[2 * i]);
        store64(out1 + 8 * i, state[2 * i + 1]);
    }
}
