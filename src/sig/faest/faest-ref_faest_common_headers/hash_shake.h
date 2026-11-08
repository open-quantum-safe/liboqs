/*
 *  SPDX-License-Identifier: MIT
 */

#ifndef HASH_SHAKE_H
#define HASH_SHAKE_H

#include <stdint.h>
#include <stdio.h>

#include "macros.h"
#include "endian_compat.h"

#if defined(WITH_SHAKE_OPENSSL)
#include <openssl/evp.h>

FAEST_BEGIN_C_DECL

typedef struct hash_context_openssl_s {
  EVP_MD_CTX* ctx;
} hash_context;

/**
 * Initialize hash context based on the security parameter. If the security parameter is 128,
 * SHAKE128 is used, otherwise SHAKE256 is used.
 */
static inline void hash_init(hash_context* ctx, unsigned int security_param) {
  const EVP_MD* md = NULL;
  if (security_param == 128) {
    md = EVP_shake128();
  } else {
    md = EVP_shake256();
  }

  ctx->ctx = EVP_MD_CTX_new();
  if (!EVP_DigestInit_ex2(ctx->ctx, md, NULL)) {
    EVP_MD_CTX_free(ctx->ctx);
    ctx->ctx = NULL;
  }
}

static inline void hash_copy(hash_context* dst, const hash_context* src) {
  dst->ctx = EVP_MD_CTX_dup(src->ctx);
}

static inline void hash_update(hash_context* ctx, const uint8_t* data, size_t size) {
  EVP_DigestUpdate(ctx->ctx, data, size);
}

#define hash_final(ctx)                                                                            \
  {                                                                                                \
    (void)ctx;                                                                                     \
  }

static inline void hash_squeeze(hash_context* ctx, uint8_t* buffer, size_t buflen) {
  EVP_DigestSqueeze(ctx->ctx, buffer, buflen);
}

static inline void hash_clear(hash_context* ctx) {
  EVP_MD_CTX_free(ctx->ctx);
  ctx->ctx = NULL;
}

FAEST_END_C_DECL

#elif defined(OQS)
#include <oqs/sha3.h>

FAEST_BEGIN_C_DECL

typedef struct hash_context_oqs_s {
  union {
    OQS_SHA3_shake128_inc_ctx shake128_ctx;
    OQS_SHA3_shake256_inc_ctx shake256_ctx;
  };
  unsigned char shake256;
} hash_context;

/**
 * Initialize hash context based on the security parameter. If the security parameter is 128,
 * SHAKE128 is used, otherwise SHAKE256 is used.
 */
static inline void hash_init(hash_context* ctx, unsigned int security_param) {
  if (security_param == 128) {
    OQS_SHA3_shake128_inc_init(&ctx->shake128_ctx);
    ctx->shake256 = 0;
  } else {
    OQS_SHA3_shake256_inc_init(&ctx->shake256_ctx);
    ctx->shake256 = 1;
  }
}

static inline void hash_copy(hash_context* dst, const hash_context* src) {
  if (!src->shake256) {
    OQS_SHA3_shake128_inc_init(&dst->shake128_ctx);
    OQS_SHA3_shake128_inc_ctx_clone(&dst->shake128_ctx, &src->shake128_ctx);
    dst->shake256 = 0;
  } else {
    OQS_SHA3_shake256_inc_init(&dst->shake256_ctx);
    OQS_SHA3_shake256_inc_ctx_clone(&dst->shake256_ctx, &src->shake256_ctx);
    dst->shake256 = 1;
  }
}

static inline void hash_update(hash_context* ctx, const uint8_t* data, size_t size) {
  if (ctx->shake256) {
    OQS_SHA3_shake256_inc_absorb(&ctx->shake256_ctx, data, size);
  } else {
    OQS_SHA3_shake128_inc_absorb(&ctx->shake128_ctx, data, size);
  }
}

static inline void hash_final(hash_context* ctx) {
  if (ctx->shake256) {
    OQS_SHA3_shake256_inc_finalize(&ctx->shake256_ctx);
  } else {
    OQS_SHA3_shake128_inc_finalize(&ctx->shake128_ctx);
  }
}

static inline void hash_squeeze(hash_context* ctx, uint8_t* buffer, size_t buflen) {
  if (ctx->shake256) {
    OQS_SHA3_shake256_inc_squeeze(buffer, buflen, &ctx->shake256_ctx);
  } else {
    OQS_SHA3_shake128_inc_squeeze(buffer, buflen, &ctx->shake128_ctx);
  }
}

static inline void hash_clear(hash_context* ctx) {
  if (ctx->shake256) {
    OQS_SHA3_shake256_inc_ctx_release(&ctx->shake256_ctx);
  } else {
    OQS_SHA3_shake128_inc_ctx_release(&ctx->shake128_ctx);
  }
}

FAEST_END_C_DECL

#else
#if !defined(SUPERCOP)
#if defined(__cplusplus)
extern "C" {
#endif
#if defined(WITH_SHAKE_XKCP)
#include <KeccakHash.h>
#else
/* use SHAKE implementation in sha3/ */
#include "sha3/KeccakHash.h"
#endif
#if defined(__cplusplus)
}
#endif
#else
/* use SUPERCOP implementation */
#include <libkeccak.a.headers/KeccakHash.h>
#endif

FAEST_BEGIN_C_DECL

typedef Keccak_HashInstance hash_context;

/**
 * Initialize hash context based on the security parameter. If the security parameter is 128,
 * SHAKE128 is used, otherwise SHAKE256 is used.
 */
static inline void hash_init(hash_context* ctx, unsigned int security_param) {
  if (security_param == 128) {
    Keccak_HashInitialize_SHAKE128(ctx);
  } else {
    Keccak_HashInitialize_SHAKE256(ctx);
  }
}

static inline void hash_copy(hash_context* dst, const hash_context* src) {
  memcpy(dst, src, sizeof(*dst));
}

static inline void hash_update(hash_context* ctx, const uint8_t* data, size_t size) {
  Keccak_HashUpdate(ctx, data, size << 3);
}

static inline void hash_final(hash_context* ctx) {
  Keccak_HashFinal(ctx, NULL);
}

static inline void hash_squeeze(hash_context* ctx, uint8_t* buffer, size_t buflen) {
  Keccak_HashSqueeze(ctx, buffer, buflen << 3);
}

#define hash_clear(ctx)                                                                            \
  {                                                                                                \
    (void)ctx;                                                                                     \
  }
#endif

static inline void hash_update_uint32_le(hash_context* ctx, uint32_t data) {
#if defined(FAEST_IS_BIG_ENDIAN)
  data = htole32(data);
#endif
  hash_update(ctx, (const uint8_t*)&data, sizeof(data));
}

FAEST_END_C_DECL

#endif
