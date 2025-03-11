// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file utils_prng.c
/// @brief The implementation of PRNG related functions.
///

#include "utils_prng.h"

#include <string.h>  // for memcpy()


//
// Defining prng_set_publicinputs() and aes128ctr_publicinputs()
//
//

#if defined(_UTILS_PQM4_)

int prng_set_publicinputs(prng_publicinputs_t *ctx, const unsigned char prng_seed[16]) {
    #ifdef _4ROUND_AES_
    aes128_4rounds_ctr_keyexp_publicinputs(&ctx->ctx, prng_seed);
    #else
    aes128_ctr_keyexp_publicinputs(&ctx->ctx, prng_seed);
    #endif
    ctx->used = RNG_OUTPUTLEN;
    ctx->ctr = 0;
    return 0;
}



static
int aes128ctr_publicinputs( unsigned char *out, unsigned nblocks, const unsigned char *n, uint32_t ctr, const prng_publicinputs_t *ctx ) {
    #ifdef _4ROUND_AES_
    aes128_4rounds_ctr_publicinputs(out, nblocks * RNG_OUTPUTLEN, n, ctr, &ctx->ctx);
    #else
    aes128_ctr_publicinputs(out, nblocks * RNG_OUTPUTLEN, n, ctr, &ctx->ctx);
    #endif
    return 0;
}

void prng_release_publicinputs(prng_publicinputs_t *ctx){
    //no-op
    (void) ctx;
}

#elif defined(_UTILS_AESNI_)

#include "x86aesni.h"

int prng_set_publicinputs(prng_publicinputs_t *ctx, const unsigned char prng_seed[16]) {
    //memcpy(ctx->key, prng_seed, 32);
    ctx->used = RNG_OUTPUTLEN;
    ctx->ctr = 0;

    ctx->key_offset = (((uint64_t)(ctx->key)) & 0xf) ? 16 - (((uint64_t)(ctx->key)) & 0xf) : 0;
    #ifdef _4ROUND_AES_
    AES128_4R_Key_Expansion ( ctx->key + ctx->key_offset, prng_seed );
    #else
    AES128_Key_Expansion ( ctx->key + ctx->key_offset, prng_seed );
    #endif
    return 0;
}


static
int aes128ctr_publicinputs( unsigned char *out, unsigned nblocks, const unsigned char *n, uint32_t ctr, const prng_publicinputs_t *ctx) {
    #ifdef _4ROUND_AES_
    AES128_4R_CTR_Encrypt ( out, (RNG_OUTPUTLEN / 16)*nblocks, ctx->key + ctx->key_offset, n, ctr );
    #else
    AES128_CTR_Encrypt ( out, (RNG_OUTPUTLEN / 16)*nblocks, ctx->key + ctx->key_offset, n, ctr );
    #endif
    return 0;
}

void prng_release_publicinputs(prng_publicinputs_t *ctx){
    //no-op
    (void) ctx;
}

#elif defined(_UTILS_NEONAES_)

#include "aes_neonaes.h"

int prng_set_publicinputs(prng_publicinputs_t *ctx, const unsigned char prng_seed[16]) {
    ctx->used = RNG_OUTPUTLEN;
    ctx->ctr = 0;
    #ifdef _4ROUND_AES_
    aes128_4r_keyexp_neonaes( ctx->key, prng_seed );
    #else
    aes128_keyexp_neonaes( ctx->key, prng_seed );
    #endif
    return 0;
}


static
int aes128ctr_publicinputs( unsigned char *out, unsigned nblocks, const unsigned char *n, uint32_t ctr, const prng_publicinputs_t *ctx) {
    while (nblocks--) {
        #ifdef _4ROUND_AES_
        aes128ctrx4_4r_enc_neonaes( out, n, ctr, ctx->key );
        #else
        aes128ctrx4_enc_neonaes( out, n, ctr, ctx->key );
        #endif
        ctr += 4;
        out += 64;
    }
    return 0;
}

void prng_release_publicinputs(prng_publicinputs_t *ctx){
    //no-op
    (void) ctx;
}

#elif defined(_UTILS_NEONBSAES_)


#include "aes128_4r_ffs.h"
#include "neon_aesffs.h"

int prng_set_publicinputs(prng_publicinputs_t *ctx, const unsigned char prng_seed[16]) {
    ctx->used = RNG_OUTPUTLEN;
    ctx->ctr = 0;

    #ifdef _4ROUND_AES_
    aes128_4r_keyschedule_ffs_lut( ctx->key, prng_seed);
    #else
    aes128_keyschedule_ffs_lut( ctx->key, prng_seed );
    #endif
    return 0;
}


static
int aes128ctr_publicinputs( unsigned char *out, unsigned nblocks, const unsigned char *n, uint32_t ctr, const prng_publicinputs_t *ctx) {
    while (nblocks--) {
        #ifdef _4ROUND_AES_
        neon_aes128ctrx8_4r_encrypt_ffs(out, n, ctr, ctx->key );
        #else
        neon_aes128ctrx8_encrypt_ffs(out, n, ctr, ctx->key );
        #endif
        out += 16 * 8;
        ctr += 8;
    }
    return 0;
}

void prng_release_publicinputs(prng_publicinputs_t *ctx){
    //no-op
    (void) ctx;
}

//#elif defined(_UTILS_SUPERCOP_)
// ERROR -- no implementation yet.
//#include "crypto_stream_aes256ctr.h"
//
// crypto_stream_aes256ctr(unsigned char *x,unsigned xlen, const unsigned char *nonce, const unsigned char *key)
//#define aes256ctr  crypto_stream_aes256ctr
//#error "needs to be implemented"

#elif defined(_UTILS_OQS_)

#include <oqs/aes.h>
int prng_set_publicinputs(prng_publicinputs_t *ctx, const unsigned char prng_seed[16]) {
    ctx->ctr =0;
    ctx->used = RNG_OUTPUTLEN;
    OQS_AES128_CTR_inc_init(prng_seed, &ctx->ctx);
    return 0;
}


static inline uint32_t br_swap32(uint32_t x) {
    x = ((x & (uint32_t)0x00FF00FF) << 8)
        | ((x >> 8) & (uint32_t)0x00FF00FF);
    return (x << 16) | (x >> 16);
}

static
int aes128ctr_publicinputs( unsigned char *out, unsigned nblocks, const unsigned char *n, uint32_t ctr, const prng_publicinputs_t *pctx ) {
    uint32_t iv[4];
    memcpy(iv, n, AES128CTR_NONCELEN);

    #if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    iv[3] = ctr;
    #else
    iv[3] = br_swap32(ctr);
    #endif

    OQS_AES128_CTR_inc_stream_iv((uint8_t*)iv, 16, pctx->ctx, out, nblocks*16);
    return 0;
}

void prng_release_publicinputs(prng_publicinputs_t *ctx){
    OQS_AES128_free_schedule(ctx->ctx);
}

#else

//
// default
//



#include "aes128_4r_ffs.h"


int prng_set_publicinputs(prng_publicinputs_t *ctx, const unsigned char prng_seed[16]) {
    ctx->used = RNG_OUTPUTLEN;
    ctx->ctr = 0;
    #ifdef _4ROUND_AES_
    aes128_4r_keyschedule_ffs_lut(ctx->key, prng_seed);
    #else
    aes128_keyschedule_ffs_lut(ctx->key, prng_seed);
    #endif
    return 0;
}


static inline uint32_t br_swap32(uint32_t x) {
    x = ((x & (uint32_t)0x00FF00FF) << 8)
        | ((x >> 8) & (uint32_t)0x00FF00FF);
    return (x << 16) | (x >> 16);
}


static
int aes128ctr_publicinputs( unsigned char *out, unsigned nblocks, const unsigned char *n, uint32_t ctr, const prng_publicinputs_t *pctx ) {
    uint32_t ptext0[4];
    uint32_t ptext1[4];
    uint8_t *p0 = (uint8_t *)ptext0;
    uint8_t *p1 = (uint8_t *)ptext1;
    memcpy( p0, n, AES128CTR_NONCELEN );
    memcpy( p1, n, AES128CTR_NONCELEN );

    while (nblocks--) {
        uint32_t c0 = ctr;
        uint32_t c1 = ctr + 1;

        #if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
        ptext0[3] = c0;
        ptext1[3] = c1;
        #else
        ptext0[3] = br_swap32(c0);
        ptext1[3] = br_swap32(c1);
        #endif
        #ifdef _4ROUND_AES_
        aes128_4r_encrypt_ffs(out, out + 16, p0, p1, pctx->key);
        #else
        aes128_encrypt_ffs(out, out + 16, p0, p1, pctx->key);
        #endif
        out += 32;

        uint32_t c2 = ctr + 2;    // XXX: ctr might overflow 32-bit boundary
        uint32_t c3 = ctr + 3;    // For the usage in OV, this is ok for expanding pk only.

        #if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
        ptext0[3] = c2;
        ptext1[3] = c3;
        #else
        ptext0[3] = br_swap32(c2);
        ptext1[3] = br_swap32(c3);
        #endif
        #ifdef _4ROUND_AES_
        aes128_4r_encrypt_ffs(out, out + 16, p0, p1, pctx->key);
        #else
        aes128_encrypt_ffs(out, out + 16, p0, p1, pctx->key);
        #endif
        out += 32;
        ctr += 4;
    }
    return 0;
}

void prng_release_publicinputs(prng_publicinputs_t *ctx){
    //no-op
    (void) ctx;
}

#endif






int prng_gen_publicinputs(prng_publicinputs_t *ctx, unsigned char *out, unsigned long outlen) {
    unsigned long long xlen = outlen;
    unsigned long long ready;
    uint8_t nonce[AES128CTR_NONCELEN] = {0};

    if (ctx->used < RNG_OUTPUTLEN) {
        ready = RNG_OUTPUTLEN - ctx->used;
        if (xlen <= ready) {
            ready = xlen;
        }
        memcpy(out, &ctx->buf[ctx->used], ready);
        ctx->used += ready;
        xlen -= ready;
        out += ready;
    }


    if (xlen >= RNG_OUTPUTLEN) {
        uint32_t nblocks = xlen / RNG_OUTPUTLEN;
        aes128ctr_publicinputs(out, nblocks, nonce, ctx->ctr, ctx);
        ctx->ctr += (RNG_OUTPUTLEN / AES128_BLOCKSIZE) * nblocks;
        xlen -= nblocks * RNG_OUTPUTLEN;
        out += nblocks * RNG_OUTPUTLEN;
    }

    if (xlen > 0) {
        aes128ctr_publicinputs(ctx->buf, 1, nonce, ctx->ctr, ctx);
        ctx->ctr += (RNG_OUTPUTLEN / AES128_BLOCKSIZE);
        memcpy(out, ctx->buf, xlen);
        ctx->used = xlen;
    }
    return outlen;
}


void prng_skip_publicinputs(prng_publicinputs_t *ctx, unsigned long outlen) {
    if (ctx->used + outlen <= RNG_OUTPUTLEN ) {
        ctx->used += outlen;
        return;
    }
    outlen -= (RNG_OUTPUTLEN - ctx->used);

    unsigned long n_blocks_skip = outlen / RNG_OUTPUTLEN;
    unsigned long rem = outlen - n_blocks_skip * RNG_OUTPUTLEN;
    uint8_t nonce[AES128CTR_NONCELEN] = {0};
    if (rem) {
        ctx->ctr += n_blocks_skip * (RNG_OUTPUTLEN / AES128_BLOCKSIZE);
        ctx->used = rem;
        aes128ctr_publicinputs(ctx->buf, 1, nonce, ctx->ctr, ctx);
        ctx->ctr += (RNG_OUTPUTLEN / AES128_BLOCKSIZE);
    } else {  // 0 == rem
        ctx->ctr += n_blocks_skip * (RNG_OUTPUTLEN / AES128_BLOCKSIZE);
        ctx->used = RNG_OUTPUTLEN;
    }
}

