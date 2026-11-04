#include <stdlib.h>
#include <string.h>
#include <limits.h>
// AES-NI cleanses round keys regardless of SHAKE backend, so OpenSSL may not be linked.
#if defined(QRUOV_PRIM_OQS)
#include <oqs/common.h>
#define QRUOV_CLEANSE(p, n) OQS_MEM_cleanse((p), (n))
#define QRUOV_MALLOC(n) OQS_MEM_malloc((n))
#define QRUOV_SECURE_FREE(p, n) OQS_MEM_secure_free((p), (n))
#else
#include <openssl/crypto.h>
#include <openssl/evp.h>
#define QRUOV_CLEANSE(p, n) OPENSSL_cleanse((p), (n))
#define QRUOV_MALLOC(n) malloc((n)) // IGNORE memory-check
#define QRUOV_SECURE_FREE(p, n) do { OPENSSL_cleanse((p), (n)); free((p)); } while (0) // IGNORE memory-check
#endif // QRUOV_PRIM_OQS
#include "prim.h"
#if defined(PRIM_AES_BACKEND_X86AESNI)
#include "x86aesni.h"
#endif // PRIM_AES_BACKEND_X86AESNI

#if defined(QRUOV_PRIM_OQS)
void shake256_init(shake256 *ctx)
{
    OQS_SHA3_shake256_inc_init(&ctx->st);
    ctx->finalized = 0;
}

void shake256_reset(shake256 *ctx)
{
    OQS_SHA3_shake256_inc_ctx_reset(&ctx->st);
    ctx->finalized = 0;
}

void shake256_update(shake256 *ctx, const uint8_t *data, size_t size)
{
    OQS_SHA3_shake256_inc_absorb(&ctx->st, data, size);
}

void shake256_digestfinal(shake256 *ctx, uint8_t *dst, size_t size)
{
    shake256_squeeze(ctx, dst, size);
}

// inc_squeeze() continues the stream across calls, so no cache is needed here.
void shake256_squeeze(shake256 *ctx, uint8_t *dst, size_t size)
{
    if (!ctx->finalized) {
        OQS_SHA3_shake256_inc_finalize(&ctx->st);
        ctx->finalized = 1;
    }
    OQS_SHA3_shake256_inc_squeeze(dst, size, &ctx->st);
}

void shake256_free(shake256 *ctx)
{
    // release() does not NULL st.ctx, and is not NULL-safe on every backend.
    if (ctx->st.ctx == NULL) return;
    OQS_SHA3_shake256_inc_ctx_release(&ctx->st);
    ctx->st.ctx = NULL;
    ctx->finalized = 0;
}

void shake128_init(shake128 *ctx)
{
    OQS_SHA3_shake128_inc_init(&ctx->st);
}

void shake128_update(shake128 *ctx, const uint8_t *data, size_t size)
{
    OQS_SHA3_shake128_inc_absorb(&ctx->st, data, size);
}

void shake128_digestfinal(shake128 *ctx, uint8_t *dst, size_t size)
{
    OQS_SHA3_shake128_inc_finalize(&ctx->st);
    OQS_SHA3_shake128_inc_squeeze(dst, size, &ctx->st);
}

void shake128_free(shake128 *ctx)
{
    if (ctx->st.ctx == NULL) return;
    OQS_SHA3_shake128_inc_ctx_release(&ctx->st);
    ctx->st.ctx = NULL;
}

#else // QRUOV_PRIM_OQS

#if PRIM_NO_EVP_DIGEST_SQUEEZE
static void shake256_clear_cache(shake256 *ctx)
{
    if (ctx->cache != NULL) {
        QRUOV_SECURE_FREE(ctx->cache, ctx->cache_size);
        ctx->cache = NULL;
    }
    ctx->offset = 0;
    ctx->cache_size = 0;
}
#endif // PRIM_NO_EVP_DIGEST_SQUEEZE

void shake256_init(shake256 *ctx)
{
    ctx->md_ctx = EVP_MD_CTX_new();
    if (ctx->md_ctx == NULL) abort();
#if PRIM_NO_EVP_DIGEST_SQUEEZE
    ctx->offset = 0;
    ctx->cache_size = 0;
    ctx->cache = NULL;
#endif // PRIM_NO_EVP_DIGEST_SQUEEZE
    shake256_reset(ctx);
}

void shake256_reset(shake256 *ctx)
{
#if PRIM_NO_EVP_DIGEST_SQUEEZE
    shake256_clear_cache(ctx);
#endif // PRIM_NO_EVP_DIGEST_SQUEEZE
    if (EVP_DigestInit_ex2(ctx->md_ctx, EVP_shake256(), NULL) != 1) abort();
}

void shake256_update(shake256 *ctx, const uint8_t *data, size_t size)
{
    if (EVP_DigestUpdate(ctx->md_ctx, data, size) != 1) abort();
#if PRIM_NO_EVP_DIGEST_SQUEEZE
    shake256_clear_cache(ctx);
#endif // PRIM_NO_EVP_DIGEST_SQUEEZE
}

void shake256_digestfinal(shake256 *ctx, uint8_t *dst, size_t size)
{
    if (EVP_DigestFinalXOF(ctx->md_ctx, dst, size) != 1) abort();
}

void shake256_squeeze(shake256 *ctx, uint8_t *dst, size_t size)
{
#if !PRIM_NO_EVP_DIGEST_SQUEEZE
    if (EVP_DigestSqueeze(ctx->md_ctx, dst, size) != 1) abort();
#else
    if (size > SIZE_MAX - ctx->offset) abort();
    size_t total = ctx->offset + size;
    if (total > ctx->cache_size) {
        size_t new_size = ctx->cache_size ? ctx->cache_size : 1088;
        while (new_size < total) {
            if (new_size > SIZE_MAX / 2) {
                new_size = total;
                break;
            }
            new_size <<= 1;
        }
        EVP_MD_CTX *tmp = EVP_MD_CTX_new();
        if (tmp == NULL) abort();
        if (EVP_MD_CTX_copy_ex(tmp, ctx->md_ctx) != 1) {
            EVP_MD_CTX_free(tmp);
            abort();
        }
        uint8_t *new_cache = QRUOV_MALLOC(new_size);
        if (new_cache == NULL) {
            EVP_MD_CTX_free(tmp);
            abort();
        }
        if (EVP_DigestFinalXOF(tmp, new_cache, new_size) != 1) {
            QRUOV_SECURE_FREE(new_cache, new_size);
            EVP_MD_CTX_free(tmp);
            abort();
        }
        EVP_MD_CTX_free(tmp);
        if (ctx->cache != NULL) {
            QRUOV_SECURE_FREE(ctx->cache, ctx->cache_size);
        }
        ctx->cache = new_cache;
        ctx->cache_size = new_size;
    }
    memcpy(dst, &ctx->cache[ctx->offset], size);
    ctx->offset = total;
#endif // PRIM_NO_EVP_DIGEST_SQUEEZE
}

void shake256_free(shake256 *ctx)
{
#if PRIM_NO_EVP_DIGEST_SQUEEZE
    shake256_clear_cache(ctx);
#endif // PRIM_NO_EVP_DIGEST_SQUEEZE
    EVP_MD_CTX_free(ctx->md_ctx);
    ctx->md_ctx = NULL;
}

void shake128_init(shake128 *ctx)
{
    ctx->md_ctx = EVP_MD_CTX_new();
    if (ctx->md_ctx == NULL) abort();
    if (EVP_DigestInit_ex2(ctx->md_ctx, EVP_shake128(), NULL) != 1) abort();
}

void shake128_update(shake128 *ctx, const uint8_t *data, size_t size)
{
    if (EVP_DigestUpdate(ctx->md_ctx, data, size) != 1) abort();
}

void shake128_digestfinal(shake128 *ctx, uint8_t *dst, size_t size)
{
    if (EVP_DigestFinalXOF(ctx->md_ctx, dst, size) != 1) abort();
}

void shake128_free(shake128 *ctx)
{
    EVP_MD_CTX_free(ctx->md_ctx);
    ctx->md_ctx = NULL;
}

#endif // QRUOV_PRIM_OQS

#if defined(PRIM_AES_BACKEND_X86AESNI)
void aes128ctr_init(aes128 *ctx, const uint8_t key[16])
{
    AES128_Key_Expansion(ctx->round_keys, key);
}

void aes128ctr_stream(aes128 *ctx, const uint8_t iv[8], uint8_t *dst, size_t size)
{
    const size_t blocks = size / 16u;
    const size_t tail = size % 16u;
    uint8_t nonce[16] = {0};
    memcpy(nonce, iv, 8);

    if (blocks != 0) {
        if (blocks > ULONG_MAX) abort();
        AES128_CTR_Stream(dst, (unsigned long)blocks, ctx->round_keys, nonce, 0);
    }
    if (tail != 0) {
        uint8_t tail_block[16];
        if (blocks > UINT32_MAX) abort();
        AES128_CTR_Stream(tail_block, 1, ctx->round_keys, nonce, (uint32_t)blocks);
        memcpy(dst + (blocks * 16u), tail_block, tail);
    }
}

void aes128ctr_free(aes128 *ctx)
{
    QRUOV_CLEANSE(ctx->round_keys, sizeof(ctx->round_keys));
}
#elif defined(QRUOV_PRIM_OQS)
void aes128ctr_init(aes128 *ctx, const uint8_t key[16])
{
    OQS_AES128_CTR_inc_init(key, &ctx->schedule);
}

void aes128ctr_stream(aes128 *ctx, const uint8_t iv[8], uint8_t *dst, size_t size)
{
    if (size == 0) return;
    // liboqs exits unless the IV is 12 or 16 bytes; QR-UOV passes 8 and zero-pads.
    // Counter carry differs between backends past 2^32 blocks; QR-UOV stays far below.
    uint8_t ctr_iv[16] = {0};
    memcpy(ctr_iv, iv, 8);
    OQS_AES128_CTR_inc_stream_iv(ctr_iv, sizeof(ctr_iv), ctx->schedule, dst, size);
}

void aes128ctr_free(aes128 *ctx)
{
    OQS_AES128_free_schedule(ctx->schedule);
    ctx->schedule = NULL;
}
#else
void aes128ctr_init(aes128 *ctx, const uint8_t key[16])
{
    ctx->cipher_ctx = EVP_CIPHER_CTX_new();
    if (ctx->cipher_ctx == NULL) abort();
    if (EVP_EncryptInit_ex2(ctx->cipher_ctx, EVP_aes_128_ctr(), key, NULL, NULL) != 1) abort();
}

void aes128ctr_stream(aes128 *ctx, const uint8_t iv[8], uint8_t *dst, size_t size)
{
    uint8_t ctr_iv[16] = {0};
    uint8_t *out = dst;
    size_t remaining = size;

    memcpy(ctr_iv, iv, 8);
    if (EVP_EncryptInit_ex2(ctx->cipher_ctx, NULL, NULL, ctr_iv, NULL) != 1) abort();

    if (size == 0) return;
    memset(dst, 0, size);
    while (remaining != 0) {
        const size_t chunk = remaining > (size_t)INT_MAX ? (size_t)INT_MAX : remaining;
        const int in_size = (int)chunk;
        int out_size = 0;
        if (EVP_EncryptUpdate(ctx->cipher_ctx, out, &out_size, out, in_size) != 1) abort();
        if (out_size != in_size) abort();
        out += chunk;
        remaining -= chunk;
    }
}

void aes128ctr_free(aes128 *ctx)
{
    EVP_CIPHER_CTX_free(ctx->cipher_ctx);
    ctx->cipher_ctx = NULL;
}
#endif // PRIM_AES_BACKEND_X86AESNI
