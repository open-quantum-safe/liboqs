#ifndef PRIM_H
#define PRIM_H

#include <stddef.h>
#include <stdint.h>

// QRUOV_PRIM_OQS: build against liboqs instead of OpenSSL, as in pqov's utils_hash.c.
#if defined(QRUOV_PRIM_OQS)
#include <oqs/sha3.h>
#include <oqs/aes.h>

// Defined even without EVP: test.c prints it unconditionally.
#define PRIM_NO_EVP_DIGEST_SQUEEZE 0
#else
#include <openssl/evp.h>

// OpenSSL < 3.3 does not provide EVP_DigestSqueeze().
// For testing, defining PRIM_FORCE_NO_EVP_DIGEST_SQUEEZE forces
// the fallback path even when the API exists.
#if defined(PRIM_FORCE_NO_EVP_DIGEST_SQUEEZE)
#define PRIM_NO_EVP_DIGEST_SQUEEZE 1
#else
#define PRIM_NO_EVP_DIGEST_SQUEEZE (!OPENSSL_VERSION_PREREQ(3, 3))
#endif // PRIM_FORCE_NO_EVP_DIGEST_SQUEEZE
#endif // QRUOV_PRIM_OQS

typedef struct shake256 {
#if defined(QRUOV_PRIM_OQS)
    OQS_SHA3_shake256_inc_ctx st;
    // liboqs needs an explicit finalize before the first squeeze; this API has
    // none, so the first squeeze does it.
    int finalized;
#else
    EVP_MD_CTX *md_ctx;
#if PRIM_NO_EVP_DIGEST_SQUEEZE
    size_t offset;
    size_t cache_size;
    uint8_t *cache;
#endif // PRIM_NO_EVP_DIGEST_SQUEEZE
#endif // QRUOV_PRIM_OQS
} shake256;

typedef struct shake128 {
#if defined(QRUOV_PRIM_OQS)
    OQS_SHA3_shake128_inc_ctx st;
#else
    EVP_MD_CTX *md_ctx;
#endif // QRUOV_PRIM_OQS
} shake128;

typedef struct aes128 {
#if defined(PRIM_AES_BACKEND_X86AESNI)
    // AES-128 round keys (11 x 16 bytes), 16-byte aligned for AES-NI loads.
    uint8_t round_keys[11 * 16] __attribute__((aligned(16)));
#elif defined(QRUOV_PRIM_OQS)
    void *schedule;
#else
    EVP_CIPHER_CTX *cipher_ctx;
#endif // PRIM_AES_BACKEND_X86AESNI
} aes128;

// Requires:
//   - message := update* -> digestfinal | update* -> squeeze*
//   - call order: init -> message -> (reset -> message)* -> free
void shake256_init(shake256 *ctx);
void shake256_reset(shake256 *ctx);
void shake256_update(shake256 *ctx, const uint8_t *data, size_t size);
void shake256_digestfinal(shake256 *ctx, uint8_t *dst, size_t size);
void shake256_squeeze(shake256 *ctx, uint8_t *dst, size_t size);
void shake256_free(shake256 *ctx);

// Requires: call order is init -> update* -> digestfinal -> free
void shake128_init(shake128 *ctx);
void shake128_update(shake128 *ctx, const uint8_t *data, size_t size);
void shake128_digestfinal(shake128 *ctx, uint8_t *dst, size_t size);
void shake128_free(shake128 *ctx);

// Requires: call order is init -> stream* -> free
void aes128ctr_init(aes128 *ctx, const uint8_t key[16]);
void aes128ctr_stream(aes128 *ctx, const uint8_t iv[8], uint8_t *dst, size_t size);
void aes128ctr_free(aes128 *ctx);

#endif // PRIM_H
