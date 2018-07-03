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

#ifndef SUPERCOP
#include "sha3/KeccakHash.h"
#else
#include <libkeccak.a.headers/KeccakHash.h>
#endif

#include "picnic_impl.h"

typedef Keccak_HashInstance hash_context;

void hash_init(hash_context* ctx, const picnic_instance_t* pp);

#define hash_update(ctx, data, size) Keccak_HashUpdate((ctx), (data), (size) << 3)
#define hash_final(ctx) Keccak_HashFinal((ctx), NULL)
#define hash_squeeze(buffer, buflen, ctx) Keccak_HashSqueeze((ctx), (buffer), (buflen) << 3)

typedef Keccak_HashInstance kdf_shake_t;

#define kdf_shake_init(ctx, pp) hash_init((ctx), (pp))
#define kdf_shake_update_key(ctx, key, keylen) hash_update((ctx), (key), (keylen))
#define kdf_shake_finalize_key(ctx) hash_final((ctx))
#define kdf_shake_get_randomness(ctx, dst, count) hash_squeeze((dst), (count), (ctx))
#define kdf_shake_clear(ctx)

#endif
