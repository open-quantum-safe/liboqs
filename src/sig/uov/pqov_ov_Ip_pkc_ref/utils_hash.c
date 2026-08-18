// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file utils_hash.c
/// @brief the adapter for hash functions.
///
///

#include "utils_hash.h"


#if defined(_UTILS_OPENSSL_)||defined(_UTILS_SUPERCOP_)

#include <openssl/evp.h>


int hash_init( hash_ctx *ctx ) {
    ctx->x = EVP_MD_CTX_new();
    if (!ctx->x) {
        return -1;
    }

    #if defined(_HASH_SHAKE128_)
    int ok = EVP_DigestInit_ex(ctx->x, EVP_shake128(), NULL);
    #else
    int ok = EVP_DigestInit_ex(ctx->x, EVP_shake256(), NULL);
    #endif
    return (ok) ? 0 : -1;
}

int hash_update( hash_ctx *ctx, const unsigned char *mesg, size_t mlen ) {
    int ok = EVP_DigestUpdate(ctx->x, mesg, mlen);
    return (ok) ? 0 : -1;
}

int hash_ctx_copy( hash_ctx *nctx, const hash_ctx *octx ) {
    nctx->x = EVP_MD_CTX_new();
    if (!nctx->x) {
        return -1;
    }

    int ok = EVP_MD_CTX_copy(nctx->x, octx->x);
    return (ok) ? 0 : -1;
}

int hash_final_digest( unsigned char *out, size_t outlen, hash_ctx *ctx ) {
    int ok = EVP_DigestFinalXOF(ctx->x, out, outlen);
    EVP_MD_CTX_free(ctx->x);
    return (ok) ? 0 : -1;
}

#elif defined(_UTILS_PQM4_)

int hash_init( hash_ctx *ctx ) {
    #if defined(_HASH_SHAKE128_)
    shake128_inc_init( ctx );
    #else
    shake256_inc_init( ctx );
    #endif
    return 0;
}

int hash_update( hash_ctx *ctx, const unsigned char *mesg, size_t mlen ) {
    #if defined(_HASH_SHAKE128_)
    shake128_inc_absorb( ctx, mesg, mlen );
    #else
    shake256_inc_absorb( ctx, mesg, mlen );
    #endif
    return 0;
}

int hash_ctx_copy( hash_ctx *nctx, const hash_ctx *octx ) {
    #if defined(_HASH_SHAKE128_)
    shake128_inc_ctx_clone(nctx, octx);
    #else
    shake256_inc_ctx_clone(nctx, octx);
    #endif
    return 0;
}

int hash_final_digest( unsigned char *out, size_t outlen, hash_ctx *ctx ) {
    #if defined(_HASH_SHAKE128_)
    shake128_inc_finalize(ctx);
    shake128_inc_squeeze(out, outlen, ctx);
    shake128_inc_ctx_release(ctx);
    #else
    shake256_inc_finalize(ctx);
    shake256_inc_squeeze(out, outlen, ctx);
    shake256_inc_ctx_release(ctx);
    #endif
    return 0;
}

#elif defined(_UTILS_OQS_)
#include <oqs/sha3.h>

int hash_init( hash_ctx *ctx ) {
    #if defined(_HASH_SHAKE128_)
    OQS_SHA3_shake128_inc_init( ctx );
    #else
    OQS_SHA3_shake256_inc_init( ctx );
    #endif
    return 0;
}

int hash_update( hash_ctx *ctx, const unsigned char *mesg, size_t mlen ) {
    #if defined(_HASH_SHAKE128_)
    OQS_SHA3_shake128_inc_absorb( ctx, mesg, mlen );

    #else
    OQS_SHA3_shake256_inc_absorb( ctx, mesg, mlen );
    #endif
    return 0;
}

int hash_ctx_copy( hash_ctx *nctx, const hash_ctx *octx ) {
    #if defined(_HASH_SHAKE128_)
    OQS_SHA3_shake128_inc_init( nctx );
    OQS_SHA3_shake128_inc_ctx_clone(nctx, octx);
    #else
    OQS_SHA3_shake256_inc_init( nctx );
    OQS_SHA3_shake256_inc_ctx_clone(nctx, octx);
    #endif
    return 0;
}

int hash_final_digest( unsigned char *out, size_t outlen, hash_ctx *ctx ) {
    #if defined(_HASH_SHAKE128_)
    OQS_SHA3_shake128_inc_finalize(ctx);
    OQS_SHA3_shake128_inc_squeeze(out, outlen, ctx);
    OQS_SHA3_shake128_inc_ctx_release(ctx);
    #else
    OQS_SHA3_shake256_inc_finalize(ctx);
    OQS_SHA3_shake256_inc_squeeze(out, outlen, ctx);
    OQS_SHA3_shake256_inc_ctx_release(ctx);
    #endif
    return 0;
}

#else

// default implementation

#include "fips202.h"


int hash_init( hash_ctx *ctx ) {
    #if defined(_HASH_SHAKE128_)
    shake128_init( ctx );
    #else
    shake256_init( ctx );
    #endif
    return 0;
}

int hash_update( hash_ctx *ctx, const unsigned char *mesg, size_t mlen ) {
    #if defined(_HASH_SHAKE128_)
    shake128_absorb( ctx, mesg, mlen );
    #else
    shake256_absorb( ctx, mesg, mlen );
    #endif
    return 0;
}

#include "string.h"

int hash_ctx_copy( hash_ctx *nctx, const hash_ctx *octx ) {
    memcpy( nctx, octx, sizeof(hash_ctx) );
    return 0;
}

int hash_final_digest( unsigned char *out, size_t outlen, hash_ctx *ctx ) {
    #if defined(_HASH_SHAKE128_)
    shake128_finalize( ctx );

    unsigned nblocks = outlen / SHAKE128_RATE;
    shake128_squeezeblocks(out, nblocks, ctx);
    outlen -= nblocks * SHAKE128_RATE;
    out += nblocks * SHAKE128_RATE;
    shake128_squeeze(out, outlen, ctx);
    #else
    shake256_finalize( ctx );

    unsigned nblocks = outlen / SHAKE256_RATE;
    shake256_squeezeblocks(out, nblocks, ctx);
    outlen -= nblocks * SHAKE256_RATE;
    out += nblocks * SHAKE256_RATE;
    shake256_squeeze(out, outlen, ctx);
    #endif

    return 0;
}

#endif

