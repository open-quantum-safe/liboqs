/**
 * \file avx512vl_sha3.c
 * \brief Implementation of the OQS SHA3 API using the AVX512VL low interface.
 *
 * Copyright (c) 2025 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 */

#include "sha3.h"

#include <oqs/common.h>
#include <stdbool.h>

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define KECCAK_CTX_ALIGNMENT 32
#define _KECCAK_CTX_BYTES (200 + sizeof(uint64_t))
#define KECCAK_CTX_BYTES                                                       \
  (KECCAK_CTX_ALIGNMENT *                                                      \
   ((_KECCAK_CTX_BYTES + KECCAK_CTX_ALIGNMENT - 1) / KECCAK_CTX_ALIGNMENT))

/*
 * External compact functions
 */
extern void SHA3_sha3_256_avx512vl(uint8_t *output, const uint8_t *input,
                                   size_t inlen);
extern void SHA3_sha3_384_avx512vl(uint8_t *output, const uint8_t *input,
                                   size_t inlen);
extern void SHA3_sha3_512_avx512vl(uint8_t *output, const uint8_t *input,
                                   size_t inlen);
extern void SHA3_shake128_avx512vl(uint8_t *output, size_t outlen,
                                   const uint8_t *input, size_t inlen);
extern void SHA3_shake256_avx512vl(uint8_t *output, size_t outlen,
                                   const uint8_t *input, size_t inlen);

/*
 * External reset functions
 */
extern void
SHA3_sha3_256_inc_ctx_reset_avx512vl(OQS_SHA3_sha3_256_inc_ctx *state);
extern void
SHA3_sha3_384_inc_ctx_reset_avx512vl(OQS_SHA3_sha3_384_inc_ctx *state);
extern void
SHA3_sha3_512_inc_ctx_reset_avx512vl(OQS_SHA3_sha3_512_inc_ctx *state);
extern void
SHA3_shake128_inc_ctx_reset_avx512vl(OQS_SHA3_shake128_inc_ctx *state);
extern void
SHA3_shake256_inc_ctx_reset_avx512vl(OQS_SHA3_shake256_inc_ctx *state);

/*
 * External absorb functions
 */
extern void SHA3_sha3_256_inc_absorb_avx512vl(OQS_SHA3_sha3_256_inc_ctx *state,
        const uint8_t *input,
        size_t inlen);
extern void SHA3_sha3_384_inc_absorb_avx512vl(OQS_SHA3_sha3_384_inc_ctx *state,
        const uint8_t *input,
        size_t inlen);
extern void SHA3_sha3_512_inc_absorb_avx512vl(OQS_SHA3_sha3_512_inc_ctx *state,
        const uint8_t *input,
        size_t inlen);
extern void SHA3_shake128_inc_absorb_avx512vl(OQS_SHA3_shake128_inc_ctx *state,
        const uint8_t *input,
        size_t inlen);
extern void SHA3_shake256_inc_absorb_avx512vl(OQS_SHA3_shake256_inc_ctx *state,
        const uint8_t *input,
        size_t inlen);
/*
 * External finalize functions
 */
extern void
SHA3_sha3_256_inc_finalize_avx512vl(uint8_t *output,
                                    OQS_SHA3_sha3_256_inc_ctx *state);
extern void
SHA3_sha3_384_inc_finalize_avx512vl(uint8_t *output,
                                    OQS_SHA3_sha3_384_inc_ctx *state);
extern void
SHA3_sha3_512_inc_finalize_avx512vl(uint8_t *output,
                                    OQS_SHA3_sha3_512_inc_ctx *state);
extern void
SHA3_shake128_inc_finalize_avx512vl(OQS_SHA3_shake128_inc_ctx *state);
extern void
SHA3_shake256_inc_finalize_avx512vl(OQS_SHA3_shake256_inc_ctx *state);

/*
 * External squeeze functions
 */
extern void
SHA3_shake128_inc_squeeze_avx512vl(uint8_t *output, size_t outlen,
                                   OQS_SHA3_shake128_inc_ctx *state);
extern void
SHA3_shake256_inc_squeeze_avx512vl(uint8_t *output, size_t outlen,
                                   OQS_SHA3_shake256_inc_ctx *state);

/*
 * SHA3-256
 */
static void SHA3_sha3_256_inc_init_avx512vl(OQS_SHA3_sha3_256_inc_ctx *state) {
	state->ctx = OQS_MEM_aligned_alloc(KECCAK_CTX_ALIGNMENT, KECCAK_CTX_BYTES);
	OQS_EXIT_IF_NULLPTR(state->ctx, "SHA3");
	SHA3_sha3_256_inc_ctx_reset_avx512vl(state);
}

static void
SHA3_sha3_256_inc_ctx_release_avx512vl(OQS_SHA3_sha3_256_inc_ctx *state) {
	SHA3_sha3_256_inc_ctx_reset_avx512vl(state);
	OQS_MEM_aligned_free(state->ctx);
}

static void
SHA3_sha3_256_inc_ctx_clone_avx512vl(OQS_SHA3_sha3_256_inc_ctx *dest,
                                     const OQS_SHA3_sha3_256_inc_ctx *src) {
	memcpy(dest->ctx, src->ctx, KECCAK_CTX_BYTES);
}

/*
 * SHA3-384
 */
static void SHA3_sha3_384_inc_init_avx512vl(OQS_SHA3_sha3_384_inc_ctx *state) {
	state->ctx = OQS_MEM_aligned_alloc(KECCAK_CTX_ALIGNMENT, KECCAK_CTX_BYTES);
	OQS_EXIT_IF_NULLPTR(state->ctx, "SHA3");
	SHA3_sha3_384_inc_ctx_reset_avx512vl(state);
}

static void
SHA3_sha3_384_inc_ctx_release_avx512vl(OQS_SHA3_sha3_384_inc_ctx *state) {
	SHA3_sha3_384_inc_ctx_reset_avx512vl(state);
	OQS_MEM_aligned_free(state->ctx);
}

static void
SHA3_sha3_384_inc_ctx_clone_avx512vl(OQS_SHA3_sha3_384_inc_ctx *dest,
                                     const OQS_SHA3_sha3_384_inc_ctx *src) {
	memcpy(dest->ctx, src->ctx, KECCAK_CTX_BYTES);
}

/*
 * SHA3-512
 */
static void SHA3_sha3_512_inc_init_avx512vl(OQS_SHA3_sha3_512_inc_ctx *state) {
	state->ctx = OQS_MEM_aligned_alloc(KECCAK_CTX_ALIGNMENT, KECCAK_CTX_BYTES);
	OQS_EXIT_IF_NULLPTR(state->ctx, "SHA3");
	SHA3_sha3_512_inc_ctx_reset_avx512vl(state);
}

static void
SHA3_sha3_512_inc_ctx_release_avx512vl(OQS_SHA3_sha3_512_inc_ctx *state) {
	SHA3_sha3_512_inc_ctx_reset_avx512vl(state);
	OQS_MEM_aligned_free(state->ctx);
}

static void
SHA3_sha3_512_inc_ctx_clone_avx512vl(OQS_SHA3_sha3_512_inc_ctx *dest,
                                     const OQS_SHA3_sha3_512_inc_ctx *src) {
	memcpy(dest->ctx, src->ctx, KECCAK_CTX_BYTES);
}

/*
 * SHAKE128
 */
static void SHA3_shake128_inc_init_avx512vl(OQS_SHA3_shake128_inc_ctx *state) {
	state->ctx = OQS_MEM_aligned_alloc(KECCAK_CTX_ALIGNMENT, KECCAK_CTX_BYTES);
	OQS_EXIT_IF_NULLPTR(state->ctx, "SHA3");
	SHA3_shake128_inc_ctx_reset_avx512vl(state);
}

static void
SHA3_shake128_inc_ctx_clone_avx512vl(OQS_SHA3_shake128_inc_ctx *dest,
                                     const OQS_SHA3_shake128_inc_ctx *src) {
	memcpy(dest->ctx, src->ctx, KECCAK_CTX_BYTES);
}

static void
SHA3_shake128_inc_ctx_release_avx512vl(OQS_SHA3_shake128_inc_ctx *state) {
	SHA3_shake128_inc_ctx_reset_avx512vl(state);
	OQS_MEM_aligned_free(state->ctx);
}

/*
 * SHAKE256
 */
static void SHA3_shake256_inc_init_avx512vl(OQS_SHA3_shake256_inc_ctx *state) {
	state->ctx = OQS_MEM_aligned_alloc(KECCAK_CTX_ALIGNMENT, KECCAK_CTX_BYTES);
	OQS_EXIT_IF_NULLPTR(state->ctx, "SHA3");
	SHA3_shake256_inc_ctx_reset_avx512vl(state);
}

static void
SHA3_shake256_inc_ctx_release_avx512vl(OQS_SHA3_shake256_inc_ctx *state) {
	SHA3_shake256_inc_ctx_reset_avx512vl(state);
	OQS_MEM_aligned_free(state->ctx);
}

static void
SHA3_shake256_inc_ctx_clone_avx512vl(OQS_SHA3_shake256_inc_ctx *dest,
                                     const OQS_SHA3_shake256_inc_ctx *src) {
	memcpy(dest->ctx, src->ctx, KECCAK_CTX_BYTES);
}

const struct OQS_SHA3_callbacks sha3_avx512vl_callbacks = {
	SHA3_sha3_256_avx512vl,
	SHA3_sha3_256_inc_init_avx512vl,
	SHA3_sha3_256_inc_absorb_avx512vl,
	SHA3_sha3_256_inc_finalize_avx512vl,
	SHA3_sha3_256_inc_ctx_release_avx512vl,
	SHA3_sha3_256_inc_ctx_reset_avx512vl,
	SHA3_sha3_256_inc_ctx_clone_avx512vl,
	SHA3_sha3_384_avx512vl,
	SHA3_sha3_384_inc_init_avx512vl,
	SHA3_sha3_384_inc_absorb_avx512vl,
	SHA3_sha3_384_inc_finalize_avx512vl,
	SHA3_sha3_384_inc_ctx_release_avx512vl,
	SHA3_sha3_384_inc_ctx_reset_avx512vl,
	SHA3_sha3_384_inc_ctx_clone_avx512vl,
	SHA3_sha3_512_avx512vl,
	SHA3_sha3_512_inc_init_avx512vl,
	SHA3_sha3_512_inc_absorb_avx512vl,
	SHA3_sha3_512_inc_finalize_avx512vl,
	SHA3_sha3_512_inc_ctx_release_avx512vl,
	SHA3_sha3_512_inc_ctx_reset_avx512vl,
	SHA3_sha3_512_inc_ctx_clone_avx512vl,
	SHA3_shake128_avx512vl,
	SHA3_shake128_inc_init_avx512vl,
	SHA3_shake128_inc_absorb_avx512vl,
	SHA3_shake128_inc_finalize_avx512vl,
	SHA3_shake128_inc_squeeze_avx512vl,
	SHA3_shake128_inc_ctx_release_avx512vl,
	SHA3_shake128_inc_ctx_clone_avx512vl,
	SHA3_shake128_inc_ctx_reset_avx512vl,
	SHA3_shake256_avx512vl,
	SHA3_shake256_inc_init_avx512vl,
	SHA3_shake256_inc_absorb_avx512vl,
	SHA3_shake256_inc_finalize_avx512vl,
	SHA3_shake256_inc_squeeze_avx512vl,
	SHA3_shake256_inc_ctx_release_avx512vl,
	SHA3_shake256_inc_ctx_clone_avx512vl,
	SHA3_shake256_inc_ctx_reset_avx512vl,
};
