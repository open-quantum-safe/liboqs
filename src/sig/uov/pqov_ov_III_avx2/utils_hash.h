// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file utils_hash.h
/// @brief the interface for adapting hash functions.
///
///
#ifndef _UTILS_HASH2_H_
#define _UTILS_HASH2_H_

#include "config.h"
#include "params.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_UTILS_OPENSSL_) || defined(_UTILS_SUPERCOP_)

#include <openssl/evp.h>

typedef struct hash_ctx {
  EVP_MD_CTX *x;
} hash_ctx;

#elif defined(_UTILS_PQM4_)

#include "fips202.h"

#if defined(_HASH_SHAKE128_)
#define hash_ctx shake128incctx
#else
// default
#define hash_ctx shake256incctx
#endif

#elif defined(_UTILS_OQS_)
#include <oqs/sha3.h>
#if defined(_HASH_SHAKE128_)
#define hash_ctx OQS_SHA3_shake128_inc_ctx
#else
// default
#define hash_ctx OQS_SHA3_shake256_inc_ctx
#endif

#else

#include "fips202.h"

typedef keccak_state hash_ctx;

#endif

#define hash_init PQOV_NAMESPACE(hash_init)
int hash_init(hash_ctx *ctx);

#define hash_update PQOV_NAMESPACE(hash_update)
int hash_update(hash_ctx *ctx, const unsigned char *mesg, size_t mlen);

#define hash_ctx_copy PQOV_NAMESPACE(hash_ctx_copy)
int hash_ctx_copy(hash_ctx *nctx,
                  const hash_ctx *octx); // nctx needs no hash_init()

#define hash_final_digest PQOV_NAMESPACE(hash_final_digest)
int hash_final_digest(unsigned char *digest, size_t dlen,
                      hash_ctx *ctx); // free ctx

#ifdef  __cplusplus
}
#endif

#endif // _UTILS_HASH_H_
