/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifndef KDF_SHAKE_H
#define KDF_SHAKE_H

#include <stdint.h>

#include "macros.h"
#include "endian_compat.h"

#if defined(WITH_SHAKE_S390_CPACF)
/* use the KIMD/KLMD instructions from CPACF for SHAKE support on S390 */
#include "sha3/s390_cpacf.h"
#elif defined(OQS) || (0 && defined(PQCLEAN))
#if defined(OQS)
/* use OQS's SHAKE implementation */
#include <oqs/sha3.h>
#include <oqs/sha3x4.h>
#elif defined(PQCLEAN)
/* PQClean's SHAKE implementation
 *
 * PQClean current does not expose the AVX2-optimized version of Keccakx4.
 */
#include <fips202.h>
#define OQS_SHA3_shake128_inc_ctx shake128incctx
#define OQS_SHA3_shake128_inc_init shake128_inc_init
#define OQS_SHA3_shake128_inc_absorb shake128_inc_absorb
#define OQS_SHA3_shake128_inc_finalize shake128_inc_finalize
#define OQS_SHA3_shake128_inc_squeeze shake128_inc_squeeze
#define OQS_SHA3_shake128_inc_ctx_release shake128_inc_ctx_release
#define OQS_SHA3_shake256_inc_ctx shake256incctx
#define OQS_SHA3_shake256_inc_init shake256_inc_init
#define OQS_SHA3_shake256_inc_absorb shake256_inc_absorb
#define OQS_SHA3_shake256_inc_finalize shake256_inc_finalize
#define OQS_SHA3_shake256_inc_squeeze shake256_inc_squeeze
#define OQS_SHA3_shake256_inc_ctx_release shake256_inc_ctx_release
#endif

typedef struct hash_context_oqs_s {
  union {
    OQS_SHA3_shake128_inc_ctx shake128_ctx;
    OQS_SHA3_shake256_inc_ctx shake256_ctx;
  };
  unsigned char shake256;
} hash_context;

/**
 * Initialize hash context based on the digest size used by Picnic. If the size is 32 bytes,
 * SHAKE128 is used, otherwise SHAKE256 is used.
 */
static inline void hash_init(hash_context* ctx, size_t digest_size) {
  if (digest_size == 32) {
    OQS_SHA3_shake128_inc_init(&ctx->shake128_ctx);
    ctx->shake256 = 0;
  } else {
    OQS_SHA3_shake256_inc_init(&ctx->shake256_ctx);
    ctx->shake256 = 1;
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

/* Instances that work with 4 states in parallel using the base implementation. */
typedef struct hash_context_x4_oqs_s {
  union {
    OQS_SHA3_shake128_x4_inc_ctx shake128_ctx;
    OQS_SHA3_shake256_x4_inc_ctx shake256_ctx;
  };
  unsigned int shake256;
} hash_context_x4;

static inline void hash_init_x4(hash_context_x4* ctx, size_t digest_size) {
  if (digest_size == 32) {
    OQS_SHA3_shake128_x4_inc_init(&ctx->shake128_ctx);
    ctx->shake256 = 0;
  } else {
    OQS_SHA3_shake256_x4_inc_init(&ctx->shake256_ctx);
    ctx->shake256 = 1;
  }
}

static inline void hash_update_x4(hash_context_x4* ctx, const uint8_t** data, size_t size) {
  if (ctx->shake256) {
    OQS_SHA3_shake256_x4_inc_absorb(&ctx->shake256_ctx, data[0], data[1], data[2], data[3], size);
  } else {
    OQS_SHA3_shake128_x4_inc_absorb(&ctx->shake128_ctx, data[0], data[1], data[2], data[3], size);
  }
}

static inline void hash_update_x4_4(hash_context_x4* ctx, const uint8_t* data0,
                                    const uint8_t* data1, const uint8_t* data2,
                                    const uint8_t* data3, size_t size) {
  if (ctx->shake256) {
    OQS_SHA3_shake256_x4_inc_absorb(&ctx->shake256_ctx, data0, data1, data2, data3, size);
  } else {
    OQS_SHA3_shake128_x4_inc_absorb(&ctx->shake128_ctx, data0, data1, data2, data3, size);
  }
}

static inline void hash_update_x4_1(hash_context_x4* ctx, const uint8_t* data, size_t size) {
  if (ctx->shake256) {
    OQS_SHA3_shake256_x4_inc_absorb(&ctx->shake256_ctx, data, data, data, data, size);
  } else {
    OQS_SHA3_shake128_x4_inc_absorb(&ctx->shake128_ctx, data, data, data, data, size);
  }
}

static inline void hash_init_prefix_x4(hash_context_x4* ctx, size_t digest_size,
                                       const uint8_t prefix) {
  if (digest_size == 32) {
    OQS_SHA3_shake128_x4_inc_init(&ctx->shake128_ctx);
    OQS_SHA3_shake128_x4_inc_absorb(&ctx->shake128_ctx, &prefix, &prefix, &prefix, &prefix,
                                    sizeof(prefix));
    ctx->shake256 = 0;
  } else {
    OQS_SHA3_shake256_x4_inc_init(&ctx->shake256_ctx);
    OQS_SHA3_shake256_x4_inc_absorb(&ctx->shake256_ctx, &prefix, &prefix, &prefix, &prefix,
                                    sizeof(prefix));
    ctx->shake256 = 1;
  }
}

static inline void hash_final_x4(hash_context_x4* ctx) {
  if (ctx->shake256) {
    OQS_SHA3_shake256_x4_inc_finalize(&ctx->shake256_ctx);
  } else {
    OQS_SHA3_shake128_x4_inc_finalize(&ctx->shake128_ctx);
  }
}

static inline void hash_squeeze_x4(hash_context_x4* ctx, uint8_t** buffer, size_t buflen) {
  if (ctx->shake256) {
    OQS_SHA3_shake256_x4_inc_squeeze(buffer[0], buffer[1], buffer[2], buffer[3], buflen,
                                     &ctx->shake256_ctx);
  } else {
    OQS_SHA3_shake128_x4_inc_squeeze(buffer[0], buffer[1], buffer[2], buffer[3], buflen,
                                     &ctx->shake128_ctx);
  }
}

static inline void hash_squeeze_x4_4(hash_context_x4* ctx, uint8_t* buffer0, uint8_t* buffer1,
                                     uint8_t* buffer2, uint8_t* buffer3, size_t buflen) {
  if (ctx->shake256) {
    OQS_SHA3_shake256_x4_inc_squeeze(buffer0, buffer1, buffer2, buffer3, buflen,
                                     &ctx->shake256_ctx);
  } else {
    OQS_SHA3_shake128_x4_inc_squeeze(buffer0, buffer1, buffer2, buffer3, buflen,
                                     &ctx->shake128_ctx);
  }
}

static inline void hash_clear_x4(hash_context_x4* ctx) {
  if (ctx->shake256) {
    OQS_SHA3_shake256_x4_inc_ctx_release(&ctx->shake256_ctx);
  } else {
    OQS_SHA3_shake128_x4_inc_ctx_release(&ctx->shake128_ctx);
  }
}
#else
#if !defined(SUPERCOP)
/* use SHAKE implementation in sha3/ */
#include "sha3/KeccakHash.h"
#else
/* use SUPERCOP implementation */
#include <libkeccak.a.headers/KeccakHash.h>
#endif

#if defined(WITH_KECCAK_X4)
/* use the Keccakx4 implementation */
#include "KeccakHashtimes4.h"
#endif

typedef Keccak_HashInstance hash_context;

/**
 * Initialize hash context based on the digest size used by Picnic. If the size is 32 bytes,
 * SHAKE128 is used, otherwise SHAKE256 is used.
 */
static inline void hash_init(hash_context* ctx, size_t digest_size) {
  if (digest_size == 32) {
    Keccak_HashInitialize_SHAKE128(ctx);
  } else {
    Keccak_HashInitialize_SHAKE256(ctx);
  }
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

#define hash_clear(ctx)
#endif

static inline void hash_update_uint16_le(hash_context* ctx, uint16_t data) {
  const uint16_t data_le = htole16(data);
  hash_update(ctx, (const uint8_t*)&data_le, sizeof(data_le));
}

static inline void hash_init_prefix(hash_context* ctx, size_t digest_size, const uint8_t prefix) {
  hash_init(ctx, digest_size);
  hash_update(ctx, &prefix, sizeof(prefix));
}

typedef hash_context kdf_shake_t;

#define kdf_shake_init(ctx, digest_size) hash_init((ctx), (digest_size))
#define kdf_shake_init_prefix(ctx, digest_size, prefix)                                            \
  hash_init_prefix((ctx), (digest_size), (prefix))
#define kdf_shake_update_key(ctx, key, keylen) hash_update((ctx), (key), (keylen))
#define kdf_shake_update_key_uint16_le(ctx, key) hash_update_uint16_le((ctx), (key))
#define kdf_shake_finalize_key(ctx) hash_final((ctx))
#define kdf_shake_get_randomness(ctx, dst, count) hash_squeeze((ctx), (dst), (count))
#define kdf_shake_clear(ctx) hash_clear((ctx))

#if defined(WITH_KECCAK_X4)
/* Instances that work with 4 states in parallel. */
typedef Keccak_HashInstancetimes4 hash_context_x4;

static inline void hash_init_x4(hash_context_x4* ctx, size_t digest_size) {
  if (digest_size == 32) {
    Keccak_HashInitializetimes4_SHAKE128(ctx);
  } else {
    Keccak_HashInitializetimes4_SHAKE256(ctx);
  }
}

static inline void hash_update_x4(hash_context_x4* ctx, const uint8_t** data, size_t size) {
  Keccak_HashUpdatetimes4(ctx, data, size << 3);
}

static inline void hash_update_x4_4(hash_context_x4* ctx, const uint8_t* data0,
                                    const uint8_t* data1, const uint8_t* data2,
                                    const uint8_t* data3, size_t size) {
  const uint8_t* data[4] = {data0, data1, data2, data3};
  hash_update_x4(ctx, data, size);
}

static inline void hash_update_x4_1(hash_context_x4* ctx, const uint8_t* data, size_t size) {
  const uint8_t* tmp[4] = {data, data, data, data};
  hash_update_x4(ctx, tmp, size);
}

static inline void hash_init_prefix_x4(hash_context_x4* ctx, size_t digest_size,
                                       const uint8_t prefix) {
  hash_init_x4(ctx, digest_size);
  hash_update_x4_1(ctx, &prefix, sizeof(prefix));
}

static inline void hash_final_x4(hash_context_x4* ctx) {
  Keccak_HashFinaltimes4(ctx, NULL);
}

static inline void hash_squeeze_x4(hash_context_x4* ctx, uint8_t** buffer, size_t buflen) {
  Keccak_HashSqueezetimes4(ctx, buffer, buflen << 3);
}

static inline void hash_squeeze_x4_4(hash_context_x4* ctx, uint8_t* buffer0, uint8_t* buffer1,
                                     uint8_t* buffer2, uint8_t* buffer3, size_t buflen) {
  uint8_t* buffer[4] = {buffer0, buffer1, buffer2, buffer3};
  hash_squeeze_x4(ctx, buffer, buflen);
}

#define hash_clear_x4(ctx)
#elif !defined(OQS)
/* Instances that work with 4 states in parallel using the base Keccak implementation. */
typedef struct hash_context_x4_s {
  hash_context instances[4];
} hash_context_x4;

static inline void hash_init_x4(hash_context_x4* ctx, size_t digest_size) {
  for (unsigned int i = 0; i < 4; ++i) {
    hash_init(&ctx->instances[i], digest_size);
  }
}

static inline void hash_update_x4(hash_context_x4* ctx, const uint8_t** data, size_t size) {
  for (unsigned int i = 0; i < 4; ++i) {
    hash_update(&ctx->instances[i], data[i], size);
  }
}

static inline void hash_update_x4_4(hash_context_x4* ctx, const uint8_t* data0,
                                    const uint8_t* data1, const uint8_t* data2,
                                    const uint8_t* data3, size_t size) {
  hash_update(&ctx->instances[0], data0, size);
  hash_update(&ctx->instances[1], data1, size);
  hash_update(&ctx->instances[2], data2, size);
  hash_update(&ctx->instances[3], data3, size);
}

static inline void hash_update_x4_1(hash_context_x4* ctx, const uint8_t* data, size_t size) {
  for (unsigned int i = 0; i < 4; ++i) {
    hash_update(&ctx->instances[i], data, size);
  }
}

static inline void hash_init_prefix_x4(hash_context_x4* ctx, size_t digest_size,
                                       const uint8_t prefix) {
  for (unsigned int i = 0; i < 4; ++i) {
    hash_init_prefix(&ctx->instances[i], digest_size, prefix);
  }
}

static inline void hash_final_x4(hash_context_x4* ctx) {
  for (unsigned int i = 0; i < 4; ++i) {
    hash_final(&ctx->instances[i]);
  }
}

static inline void hash_squeeze_x4(hash_context_x4* ctx, uint8_t** buffer, size_t buflen) {
  for (unsigned int i = 0; i < 4; ++i) {
    hash_squeeze(&ctx->instances[i], buffer[i], buflen);
  }
}

static inline void hash_squeeze_x4_4(hash_context_x4* ctx, uint8_t* buffer0, uint8_t* buffer1,
                                     uint8_t* buffer2, uint8_t* buffer3, size_t buflen) {
  hash_squeeze(&ctx->instances[0], buffer0, buflen);
  hash_squeeze(&ctx->instances[1], buffer1, buflen);
  hash_squeeze(&ctx->instances[2], buffer2, buflen);
  hash_squeeze(&ctx->instances[3], buffer3, buflen);
}

#define hash_clear_x4(ctx)
#endif

static inline void hash_update_x4_uint16_le(hash_context_x4* ctx, uint16_t data) {
  const uint16_t data_le = htole16(data);
  hash_update_x4_1(ctx, (const uint8_t*)&data_le, sizeof(data_le));
}

static inline void hash_update_x4_uint16s_le(hash_context_x4* ctx, const uint16_t data[4]) {
  const uint16_t data0_le = htole16(data[0]);
  const uint16_t data1_le = htole16(data[1]);
  const uint16_t data2_le = htole16(data[2]);
  const uint16_t data3_le = htole16(data[3]);
  hash_update_x4_4(ctx, (const uint8_t*)&data0_le, (const uint8_t*)&data1_le,
                   (const uint8_t*)&data2_le, (const uint8_t*)&data3_le, sizeof(data[0]));
}

typedef hash_context_x4 kdf_shake_x4_t;

#define kdf_shake_x4_init(ctx, digest_size) hash_init_x4((ctx), (digest_size))
#define kdf_shake_x4_init_prefix(ctx, digest_size, prefix)                                         \
  hash_init_prefix_x4((ctx), (digest_size), (prefix))
#define kdf_shake_x4_update_key(ctx, key, keylen) hash_update_x4((ctx), (key), (keylen))
#define kdf_shake_x4_update_key_4(ctx, key0, key1, key2, key3, keylen)                             \
  hash_update_x4_4((ctx), (key0), (key1), (key2), (key3), (keylen))
#define kdf_shake_x4_update_key_1(ctx, key, keylen) hash_update_x4_1((ctx), (key), (keylen))
#define kdf_shake_x4_update_key_uint16_le(ctx, key) hash_update_x4_uint16_le((ctx), (key))
#define kdf_shake_x4_update_key_uint16s_le(ctx, keys) hash_update_x4_uint16s_le((ctx), (keys))
#define kdf_shake_x4_finalize_key(ctx) hash_final_x4((ctx))
#define kdf_shake_x4_get_randomness(ctx, dst, count) hash_squeeze_x4((ctx), (dst), (count))
#define kdf_shake_x4_get_randomness_4(ctx, dst0, dst1, dst2, dst3, count)                          \
  hash_squeeze_x4_4((ctx), (dst0), (dst1), (dst2), (dst3), (count))
#define kdf_shake_x4_clear(ctx) hash_clear_x4((ctx))

#endif
