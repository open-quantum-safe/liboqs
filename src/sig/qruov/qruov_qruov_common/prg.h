#ifndef PRG_H
#define PRG_H

#include "qruov_param.h"
#include "prim.h"

// Public-seed expansion uses AES-128-CTR when PRG_IS_AES == 1 (default), and SHAKE128 when PRG_IS_AES == 0.

typedef struct prg {
#if PRG_IS_AES
    aes128 ctx;
#else
    uint8_t seed[16];
#endif // PRG_IS_AES
} prg;

// Requires: call order is init -> yield* -> free
void prg_init(prg *ctx, const uint8_t seed[16]);
void prg_yield(prg *ctx, uint8_t *out, uint16_t index, size_t len);
void prg_free(prg *ctx);

#endif // PRG_H
