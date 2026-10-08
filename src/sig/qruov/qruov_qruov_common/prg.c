#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "prg.h"

void prg_init(prg *ctx, const uint8_t seed[16])
{
#if PRG_IS_AES
    aes128ctr_init(&ctx->ctx, seed);
#else
    memcpy(ctx->seed, seed, 16);
#endif // PRG_IS_AES
}

void prg_yield(prg *ctx, uint8_t *out, uint16_t index, size_t len)
{
#if PRG_IS_AES
    uint8_t nonce[8] = {0};
    store_u16_be(&nonce[6], index);
    aes128ctr_stream(&ctx->ctx, nonce, out, len);
#else
    shake128 state;
    uint8_t idx_be[2];
    shake128_init(&state);
    shake128_update(&state, ctx->seed, 16);
    store_u16_be(idx_be, index);
    shake128_update(&state, idx_be, sizeof(idx_be));
    shake128_digestfinal(&state, out, len);
    shake128_free(&state);
#endif // PRG_IS_AES
}

void prg_free(prg *ctx)
{
#if PRG_IS_AES
    aes128ctr_free(&ctx->ctx);
#else
    secure_zero(ctx->seed, sizeof(ctx->seed));
#endif // PRG_IS_AES
}
