#include <immintrin.h>

#include "context.h"

/**
 * Absorb the constant pub_seed using one round of the compression function
 * This initializes state_seeded and state_seeded_512, which can then be
 * reused in thash
 **/
static void seed_state(spx_ctx *ctx) {
    uint8_t block[SPX_SHA512_BLOCK_BYTES];
    size_t i;

    for (i = 0; i < SPX_N; ++i) {
        block[i] = ctx->pub_seed[i];
    }
    for (i = SPX_N; i < SPX_SHA512_BLOCK_BYTES; ++i) {
        block[i] = 0;
    }
    /* block has been properly initialized for both SHA-256 and SHA-512 */

    sha256_inc_init(&ctx->state_seeded);
    sha256_inc_blocks(&ctx->state_seeded, block, 1);

    // initialize x8
    sha256_init8x(&ctx->statex8_seeded);
    sha256_transform8x(&ctx->statex8_seeded,
                       block, block, block, block, block, block, block, block);

    ctx->statex8_seeded.datalen = 0;
    ctx->statex8_seeded.msglen = 512;

    sha512_inc_init(&ctx->state_seeded_512);
    sha512_inc_blocks(&ctx->state_seeded_512, block, 1);

    // initialize x4
    sha512_init4x(&ctx->statex4_seeded_512);
    sha512_transform4x(&ctx->statex4_seeded_512, block, block, block, block);

    ctx->statex4_seeded_512.datalen = 0;
    ctx->statex4_seeded_512.msglen = 1024;

}

/* For SHA, we initialize the hash function at the start */
void initialize_hash_function(spx_ctx *ctx) {
    seed_state(ctx);
}

/* Free the incremental hashing context for heap-based SHA2 APIs */
void free_hash_function(spx_ctx *ctx) {
    sha256_inc_ctx_release(&ctx->state_seeded);
    sha512_inc_ctx_release(&ctx->state_seeded_512);
}
