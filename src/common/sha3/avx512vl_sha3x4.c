/**
 * \file avx512vl_sha3x4.c
 * \brief Implementation of the OQS SHA3 times 4 API using the AVX512VL low interface.
 *
 * Copyright (c) 2025 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 */

#include "sha3x4.h"

#include <oqs/common.h>
#include <oqs/oqsconfig.h>

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define KECCAK_X4_CTX_ALIGNMENT 32
#define _KECCAK_X4_CTX_BYTES    (800 + sizeof(uint64_t))
#define KECCAK_X4_CTX_BYTES                                                                        \
        (KECCAK_X4_CTX_ALIGNMENT *                                                                 \
         ((_KECCAK_X4_CTX_BYTES + KECCAK_X4_CTX_ALIGNMENT - 1) / KECCAK_X4_CTX_ALIGNMENT))

/********** SHAKE128 ***********/

/* SHAKE128 external */

extern void
SHA3_shake128_x4_avx512vl(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3, size_t outlen,
                          const uint8_t *in0, const uint8_t *in1, const uint8_t *in2,
                          const uint8_t *in3, size_t inlen);

extern void
SHA3_shake128_x4_inc_ctx_reset_avx512vl(OQS_SHA3_shake128_x4_inc_ctx *state);

extern void
SHA3_shake128_x4_inc_absorb_avx512vl(OQS_SHA3_shake128_x4_inc_ctx *state, const uint8_t *in0,
                                     const uint8_t *in1, const uint8_t *in2, const uint8_t *in3,
                                     size_t inlen);

extern void
SHA3_shake128_x4_inc_finalize_avx512vl(OQS_SHA3_shake128_x4_inc_ctx *state);

extern void
SHA3_shake128_x4_inc_squeeze_avx512vl(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3,
                                      size_t outlen, OQS_SHA3_shake128_x4_inc_ctx *state);

/* SHAKE128 incremental */

static void
SHA3_shake128_x4_inc_init_avx512vl(OQS_SHA3_shake128_x4_inc_ctx *state) {
	state->ctx = OQS_MEM_aligned_alloc(KECCAK_X4_CTX_ALIGNMENT, KECCAK_X4_CTX_BYTES);
	OQS_EXIT_IF_NULLPTR(state->ctx, "SHA3x4");
	SHA3_shake128_x4_inc_ctx_reset_avx512vl(state);
}

static void
SHA3_shake128_x4_inc_ctx_clone_avx512vl(OQS_SHA3_shake128_x4_inc_ctx *dest,
                                        const OQS_SHA3_shake128_x4_inc_ctx *src) {
	memcpy(dest->ctx, src->ctx, KECCAK_X4_CTX_BYTES);
}

static void
SHA3_shake128_x4_inc_ctx_release_avx512vl(OQS_SHA3_shake128_x4_inc_ctx *state) {
	SHA3_shake128_x4_inc_ctx_reset_avx512vl(state);
	OQS_MEM_aligned_free(state->ctx);
}

/********** SHAKE256 ***********/

/* SHAKE256 external */

extern void
SHA3_shake256_x4_avx512vl(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3, size_t outlen,
                          const uint8_t *in0, const uint8_t *in1, const uint8_t *in2,
                          const uint8_t *in3, size_t inlen);

extern void
SHA3_shake256_x4_inc_ctx_reset_avx512vl(OQS_SHA3_shake256_x4_inc_ctx *state);

extern void
SHA3_shake256_x4_inc_absorb_avx512vl(OQS_SHA3_shake256_x4_inc_ctx *state, const uint8_t *in0,
                                     const uint8_t *in1, const uint8_t *in2, const uint8_t *in3,
                                     size_t inlen);

extern void
SHA3_shake256_x4_inc_finalize_avx512vl(OQS_SHA3_shake256_x4_inc_ctx *state);

extern void
SHA3_shake256_x4_inc_squeeze_avx512vl(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3,
                                      size_t outlen, OQS_SHA3_shake256_x4_inc_ctx *state);

/* SHAKE256 incremental */

static void
SHA3_shake256_x4_inc_init_avx512vl(OQS_SHA3_shake256_x4_inc_ctx *state) {
	state->ctx = OQS_MEM_aligned_alloc(KECCAK_X4_CTX_ALIGNMENT, KECCAK_X4_CTX_BYTES);
	OQS_EXIT_IF_NULLPTR(state->ctx, "SHA3x4");
	SHA3_shake256_x4_inc_ctx_reset_avx512vl(state);
}

static void
SHA3_shake256_x4_inc_ctx_clone_avx512vl(OQS_SHA3_shake256_x4_inc_ctx *dest,
                                        const OQS_SHA3_shake256_x4_inc_ctx *src) {
	memcpy(dest->ctx, src->ctx, KECCAK_X4_CTX_BYTES);
}

static void
SHA3_shake256_x4_inc_ctx_release_avx512vl(OQS_SHA3_shake256_x4_inc_ctx *state) {
	SHA3_shake256_x4_inc_ctx_reset_avx512vl(state);
	OQS_MEM_aligned_free(state->ctx);
}

const struct OQS_SHA3_x4_callbacks sha3_x4_avx512vl_callbacks = {
	SHA3_shake128_x4_avx512vl,
	SHA3_shake128_x4_inc_init_avx512vl,
	SHA3_shake128_x4_inc_absorb_avx512vl,
	SHA3_shake128_x4_inc_finalize_avx512vl,
	SHA3_shake128_x4_inc_squeeze_avx512vl,
	SHA3_shake128_x4_inc_ctx_release_avx512vl,
	SHA3_shake128_x4_inc_ctx_clone_avx512vl,
	SHA3_shake128_x4_inc_ctx_reset_avx512vl,
	SHA3_shake256_x4_avx512vl,
	SHA3_shake256_x4_inc_init_avx512vl,
	SHA3_shake256_x4_inc_absorb_avx512vl,
	SHA3_shake256_x4_inc_finalize_avx512vl,
	SHA3_shake256_x4_inc_squeeze_avx512vl,
	SHA3_shake256_x4_inc_ctx_release_avx512vl,
	SHA3_shake256_x4_inc_ctx_clone_avx512vl,
	SHA3_shake256_x4_inc_ctx_reset_avx512vl,
};
