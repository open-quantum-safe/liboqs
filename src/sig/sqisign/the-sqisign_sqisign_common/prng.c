// SPDX-License-Identifier: Apache-2.0

#include <assert.h>
#include <string.h>
#include <prng.h>
#include <rng.h>
#include <mem.h>
#include <fips202.h>

// Marks a seeded root or domain context. Reduces the risk that reused memory from a stack-allocated domain context
// would contain a "simple" value such as 1 and be falsely interpreted as seeded.
#define PRNG_SEEDED_MAGIC UINT32_C(0x5EEDED)

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

_Static_assert(sizeof(DS_DEFAULT_DOMAIN) == PRNG_DOMAIN_SEP_BYTES + 1,
               "domain separators must be exactly " STRINGIFY(PRNG_DOMAIN_SEP_BYTES) " characters");

// The seed every domain stream is derived from. Retained for the duration of an operation so that domains can be
// derived at any point without consuming more entropy. Local to this file: prng_domain_seed() is its only consumer.
typedef struct
{
    unsigned char seed[PRNG_SEED_BYTES];
    uint32_t seeded;
} prng_root_t;

static SQISIGN_THREAD_LOCAL prng_root_t prng_root;

SQISIGN_THREAD_LOCAL prng_domain_ctx_t PRNG_default_domain;

int
prng_seed(void)
{
    prng_root.seeded = 0;
    if (randombytes(prng_root.seed, PRNG_SEED_BYTES) != 0) {
        prng_clear();
        return -1;
    }
    prng_root.seeded = PRNG_SEEDED_MAGIC;

    if (prng_domain_seed(&PRNG_default_domain, DS_DEFAULT_DOMAIN) != 0) {
        prng_clear();
        return -1;
    }

    return 0;
}

void
prng_clear(void)
{
    // Also resets prng_root.seeded to 0
    sqisign_secure_clear(&prng_root, sizeof(prng_root));
    prng_domain_clear(&PRNG_default_domain);
}

int
prng_domain_seed(prng_domain_ctx_t *ctx, const char domain[PRNG_DOMAIN_SEP_BYTES + 1])
{
    assert(strlen(domain) == PRNG_DOMAIN_SEP_BYTES);
    if (strlen(domain) != PRNG_DOMAIN_SEP_BYTES)
        return -1;
    // Deriving a domain before a successful prng_seed() is a usage error
    assert(prng_root.seeded == PRNG_SEEDED_MAGIC);
    if (prng_root.seeded != PRNG_SEEDED_MAGIC)
        return -1;

    ctx->seeded = 0;
    shake256_inc_init(&ctx->ctx);
    shake256_inc_absorb(&ctx->ctx, prng_root.seed, PRNG_SEED_BYTES);
    shake256_inc_absorb(&ctx->ctx, (const unsigned char *)domain, PRNG_DOMAIN_SEP_BYTES);
    shake256_inc_finalize(&ctx->ctx);
    ctx->seeded = PRNG_SEEDED_MAGIC;

    return 0;
}

void
prng_domain_clear(prng_domain_ctx_t *ctx)
{
    // The incremental SHAKE API requires pairing every inc_init with a release;
    // implementations are allowed to heap-allocate their state in init.
    if (ctx->seeded == PRNG_SEEDED_MAGIC)
        shake256_inc_ctx_release(&ctx->ctx);
    // Also resets ctx->seeded to 0
    sqisign_secure_clear(ctx, sizeof(*ctx));
}

int
prng_random_bytes(prng_domain_ctx_t *ctx, unsigned char *out, size_t len)
{
    // Squeezing from an unseeded context is a usage error
    assert(ctx->seeded == PRNG_SEEDED_MAGIC);
    if (ctx->seeded != PRNG_SEEDED_MAGIC)
        return -1;
    shake256_inc_squeeze(out, len, &ctx->ctx);
    return 0;
}
