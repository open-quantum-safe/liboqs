// SPDX-License-Identifier: Apache-2.0

// liboqs integration shim: route SQIsign's FIPS202 (SHA3/SHAKE) calls to liboqs OQS_SHA3_*.
// This header overrides the in-tree src/common/generic/include/fips202.h when SQIsign is built
// inside liboqs; the bundled fips202.c is excluded from the liboqs source list.
//
// The OQS_SHA3 API mirrors the PQClean FIPS202 API used by SQIsign, with matching argument
// orders, so the incremental + one-shot calls map directly. The only non-trivial part is the
// non-incremental "ctx" XOF API (shake*_absorb / shake*_squeezeblocks), which OQS does not expose
// directly: it is emulated on top of the OQS incremental context below. The resulting output is
// bit-for-bit identical (SHAKE is independent of incremental vs. one-shot absorption).

#ifndef FIPS202_H
#define FIPS202_H

#include <stddef.h>
#include <stdint.h>
#include <oqs/sha3.h>

// Keccak rates (bytes) -- needed by the squeezeblocks wrappers and by SQIsign callers.
#define SHAKE128_RATE 168
#define SHAKE256_RATE 136
#define SHA3_256_RATE 136
#define SHA3_384_RATE 104
#define SHA3_512_RATE 72

// ---- Incremental contexts -> OQS incremental contexts -------------------------------------
#define shake128incctx OQS_SHA3_shake128_inc_ctx
#define shake256incctx OQS_SHA3_shake256_inc_ctx
#define sha3_256incctx OQS_SHA3_sha3_256_inc_ctx
#define sha3_384incctx OQS_SHA3_sha3_384_inc_ctx
#define sha3_512incctx OQS_SHA3_sha3_512_inc_ctx

// Non-incremental ("ctx") contexts are emulated using the OQS incremental contexts.
#define shake128ctx OQS_SHA3_shake128_inc_ctx
#define shake256ctx OQS_SHA3_shake256_inc_ctx

// ---- SHAKE128 incremental -----------------------------------------------------------------
#define shake128_inc_init OQS_SHA3_shake128_inc_init
#define shake128_inc_absorb OQS_SHA3_shake128_inc_absorb
#define shake128_inc_finalize OQS_SHA3_shake128_inc_finalize
#define shake128_inc_squeeze OQS_SHA3_shake128_inc_squeeze
#define shake128_inc_ctx_clone OQS_SHA3_shake128_inc_ctx_clone
#define shake128_inc_ctx_release OQS_SHA3_shake128_inc_ctx_release
#define shake128_inc_ctx_reset OQS_SHA3_shake128_inc_ctx_reset

// ---- SHAKE256 incremental -----------------------------------------------------------------
#define shake256_inc_init OQS_SHA3_shake256_inc_init
#define shake256_inc_absorb OQS_SHA3_shake256_inc_absorb
#define shake256_inc_finalize OQS_SHA3_shake256_inc_finalize
#define shake256_inc_squeeze OQS_SHA3_shake256_inc_squeeze
#define shake256_inc_ctx_clone OQS_SHA3_shake256_inc_ctx_clone
#define shake256_inc_ctx_release OQS_SHA3_shake256_inc_ctx_release
#define shake256_inc_ctx_reset OQS_SHA3_shake256_inc_ctx_reset

// ---- One-shot XOFs (identical argument order) ---------------------------------------------
#define shake128 OQS_SHA3_shake128
#define shake256 OQS_SHA3_shake256

// ---- SHA3-256/384/512 (incremental + one-shot, identical argument order) ------------------
#define sha3_256_inc_init OQS_SHA3_sha3_256_inc_init
#define sha3_256_inc_absorb OQS_SHA3_sha3_256_inc_absorb
#define sha3_256_inc_finalize OQS_SHA3_sha3_256_inc_finalize
#define sha3_256_inc_ctx_clone OQS_SHA3_sha3_256_inc_ctx_clone
#define sha3_256_inc_ctx_release OQS_SHA3_sha3_256_inc_ctx_release
#define sha3_256 OQS_SHA3_sha3_256

#define sha3_384_inc_init OQS_SHA3_sha3_384_inc_init
#define sha3_384_inc_absorb OQS_SHA3_sha3_384_inc_absorb
#define sha3_384_inc_finalize OQS_SHA3_sha3_384_inc_finalize
#define sha3_384_inc_ctx_clone OQS_SHA3_sha3_384_inc_ctx_clone
#define sha3_384_inc_ctx_release OQS_SHA3_sha3_384_inc_ctx_release
#define sha3_384 OQS_SHA3_sha3_384

#define sha3_512_inc_init OQS_SHA3_sha3_512_inc_init
#define sha3_512_inc_absorb OQS_SHA3_sha3_512_inc_absorb
#define sha3_512_inc_finalize OQS_SHA3_sha3_512_inc_finalize
#define sha3_512_inc_ctx_clone OQS_SHA3_sha3_512_inc_ctx_clone
#define sha3_512_inc_ctx_release OQS_SHA3_sha3_512_inc_ctx_release
#define sha3_512 OQS_SHA3_sha3_512

// ---- Non-incremental ("ctx") XOF API emulated on the OQS incremental context --------------
static inline void
shake128_absorb(OQS_SHA3_shake128_inc_ctx *state, const uint8_t *input, size_t inlen)
{
    OQS_SHA3_shake128_inc_init(state);
    OQS_SHA3_shake128_inc_absorb(state, input, inlen);
    OQS_SHA3_shake128_inc_finalize(state);
}

static inline void
shake128_squeezeblocks(uint8_t *output, size_t nblocks, OQS_SHA3_shake128_inc_ctx *state)
{
    OQS_SHA3_shake128_inc_squeeze(output, nblocks * SHAKE128_RATE, state);
}

static inline void
shake128_ctx_release(OQS_SHA3_shake128_inc_ctx *state)
{
    OQS_SHA3_shake128_inc_ctx_release(state);
}

static inline void
shake128_ctx_clone(OQS_SHA3_shake128_inc_ctx *dest, const OQS_SHA3_shake128_inc_ctx *src)
{
    OQS_SHA3_shake128_inc_ctx_clone(dest, src);
}

static inline void
shake256_absorb(OQS_SHA3_shake256_inc_ctx *state, const uint8_t *input, size_t inlen)
{
    OQS_SHA3_shake256_inc_init(state);
    OQS_SHA3_shake256_inc_absorb(state, input, inlen);
    OQS_SHA3_shake256_inc_finalize(state);
}

static inline void
shake256_squeezeblocks(uint8_t *output, size_t nblocks, OQS_SHA3_shake256_inc_ctx *state)
{
    OQS_SHA3_shake256_inc_squeeze(output, nblocks * SHAKE256_RATE, state);
}

static inline void
shake256_ctx_release(OQS_SHA3_shake256_inc_ctx *state)
{
    OQS_SHA3_shake256_inc_ctx_release(state);
}

static inline void
shake256_ctx_clone(OQS_SHA3_shake256_inc_ctx *dest, const OQS_SHA3_shake256_inc_ctx *src)
{
    OQS_SHA3_shake256_inc_ctx_clone(dest, src);
}

#endif // FIPS202_H
