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

#include <stdbool.h>
#include <stdint.h>

#if !defined(KeccakP200_excluded)
#define KeccakP200_excluded
#endif

#if !defined(KeccakP400_excluded)
#define KeccakP400_excluded
#endif

#if !defined(KeccakP800_excluded)
#define KeccakP800_excluded
#endif

#if !defined(SUPERCOP)
#include "sha3/KeccakHash.h"
#else
#include <libkeccak.a.headers/KeccakHash.h>
#endif
// this is not in SUPERCOP, so we ship it ourselves
#include "sha3/KeccakHashtimes4.h"

#include "picnic_impl.h"

typedef Keccak_HashInstance hash_context;

static inline void hash_init(hash_context* ctx, const picnic_instance_t* pp) {
  if (pp->digest_size == 32) {
    Keccak_HashInitialize_SHAKE128(ctx);
  } else {
    Keccak_HashInitialize_SHAKE256(ctx);
  }
}

static inline void hash_update(hash_context* ctx, const uint8_t* data, size_t size) {
  Keccak_HashUpdate(ctx, data, size << 3);
}

static inline void hash_init_prefix(hash_context* ctx, const picnic_instance_t* pp,
                                    const uint8_t prefix) {
  hash_init(ctx, pp);
  hash_update(ctx, &prefix, sizeof(prefix));
}

static inline void hash_final(hash_context* ctx) {
  Keccak_HashFinal(ctx, NULL);
}

static inline void hash_squeeze(hash_context* ctx, uint8_t* buffer, size_t buflen) {
  Keccak_HashSqueeze(ctx, buffer, buflen << 3);
}

typedef Keccak_HashInstance kdf_shake_t;

#define kdf_shake_init(ctx, pp) hash_init((ctx), (pp))
#define kdf_shake_init_prefix(ctx, pp, prefix) hash_init_prefix((ctx), (pp), (prefix))
#define kdf_shake_update_key(ctx, key, keylen) hash_update((ctx), (key), (keylen))
#define kdf_shake_finalize_key(ctx) hash_final((ctx))
#define kdf_shake_get_randomness(ctx, dst, count) hash_squeeze((ctx), (dst), (count))
#define kdf_shake_clear(ctx)

// Instances that work with 4 states in parallel
typedef Keccak_HashInstancetimes4 hash_context_x4;

static inline void hash_init_x4(hash_context_x4* ctx, const picnic_instance_t* pp) {
  if (pp->digest_size == 32) {
    Keccak_HashInitializetimes4_SHAKE128(ctx);
  } else {
    Keccak_HashInitializetimes4_SHAKE256(ctx);
  }
}

static inline void hash_update_x4(hash_context_x4* ctx, const uint8_t** data, size_t size) {
  Keccak_HashUpdatetimes4(ctx, data, size << 3);
}

static inline void hash_init_prefix_x4(hash_context_x4* ctx, const picnic_instance_t* pp,
                                       const uint8_t prefix) {
  hash_init_x4(ctx, pp);
  const uint8_t* prefixes[] = {&prefix, &prefix, &prefix, &prefix};
  hash_update_x4(ctx, prefixes, sizeof(prefix));
}

static inline void hash_final_x4(hash_context_x4* ctx) {
  Keccak_HashFinaltimes4(ctx, NULL);
}

static inline void hash_squeeze_x4(hash_context_x4* ctx, uint8_t** buffer, size_t buflen) {
  Keccak_HashSqueezetimes4(ctx, buffer, buflen << 3);
}

typedef Keccak_HashInstancetimes4 kdf_shake_x4_t;

#define kdf_shake_x4_init(ctx, pp) hash_init_x4((ctx), (pp))
#define kdf_shake_x4_init_prefix(ctx, pp, prefix) hash_init_prefix_x4((ctx), (pp), (prefix))
#define kdf_shake_x4_update_key(ctx, key, keylen) hash_update_x4((ctx), (key), (keylen))
#define kdf_shake_x4_finalize_key(ctx) hash_final_x4((ctx))
#define kdf_shake_x4_get_randomness(ctx, dst, count) hash_squeeze_x4((ctx), (dst), (count))
#define kdf_shake_x4_clear(ctx)
#endif
