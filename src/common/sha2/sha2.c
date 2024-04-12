// SPDX-License-Identifier: MIT
#include <stdio.h>

#include "sha2.h"
#include "sha2_local.h"

static struct OQS_SHA2_callbacks *callbacks = &sha2_default_callbacks;

OQS_API void OQS_SHA2_set_callbacks(struct OQS_SHA2_callbacks *new_callbacks) {
	callbacks = new_callbacks;
}

void OQS_SHA2_sha256_inc_init(OQS_SHA2_sha256_ctx *state) {
	callbacks->SHA2_sha256_inc_init(state);
}

void OQS_SHA2_sha256_inc_ctx_clone(OQS_SHA2_sha256_ctx *dest, const OQS_SHA2_sha256_ctx *src) {
	callbacks->SHA2_sha256_inc_ctx_clone(dest, src);
}

void OQS_SHA2_sha256_inc_blocks(OQS_SHA2_sha256_ctx *state, const uint8_t *in, size_t inblocks) {
	callbacks->SHA2_sha256_inc_blocks(state, in, inblocks);
}

void OQS_SHA2_sha256_inc(OQS_SHA2_sha256_ctx *state, const uint8_t *in, size_t len) {
	callbacks->SHA2_sha256_inc(state, in, len);
}

void OQS_SHA2_sha256_inc_finalize(uint8_t *out, OQS_SHA2_sha256_ctx *state, const uint8_t *in, size_t inlen) {
	callbacks->SHA2_sha256_inc_finalize(out, state, in, inlen);
}

void OQS_SHA2_sha256_inc_ctx_release(OQS_SHA2_sha256_ctx *state) {
	callbacks->SHA2_sha256_inc_ctx_release(state);
}

void OQS_SHA2_sha384_inc_init(OQS_SHA2_sha384_ctx *state) {
	callbacks->SHA2_sha384_inc_init(state);
}

void OQS_SHA2_sha384_inc_ctx_clone(OQS_SHA2_sha384_ctx *dest, const OQS_SHA2_sha384_ctx *src) {
	callbacks->SHA2_sha384_inc_ctx_clone(dest, src);
}

void OQS_SHA2_sha384_inc_blocks(OQS_SHA2_sha384_ctx *state, const uint8_t *in, size_t inblocks) {
	callbacks->SHA2_sha384_inc_blocks(state, in, inblocks);
}

void OQS_SHA2_sha384_inc_finalize(uint8_t *out, OQS_SHA2_sha384_ctx *state, const uint8_t *in, size_t inlen) {
	callbacks->SHA2_sha384_inc_finalize(out, state, in, inlen);
}

void OQS_SHA2_sha384_inc_ctx_release(OQS_SHA2_sha384_ctx *state) {
	callbacks->SHA2_sha384_inc_ctx_release(state);
}

void OQS_SHA2_sha512_inc_init(OQS_SHA2_sha512_ctx *state) {
	callbacks->SHA2_sha512_inc_init(state);
}

void OQS_SHA2_sha512_inc_ctx_clone(OQS_SHA2_sha512_ctx *dest, const OQS_SHA2_sha512_ctx *src) {
	callbacks->SHA2_sha512_inc_ctx_clone(dest, src);
}

void OQS_SHA2_sha512_inc_blocks(OQS_SHA2_sha512_ctx *state, const uint8_t *in, size_t inblocks) {
	callbacks->SHA2_sha512_inc_blocks(state, in, inblocks);
}

void OQS_SHA2_sha512_inc_finalize(uint8_t *out, OQS_SHA2_sha512_ctx *state, const uint8_t *in, size_t inlen) {
	callbacks->SHA2_sha512_inc_finalize(out, state, in, inlen);
}

void OQS_SHA2_sha512_inc_ctx_release(OQS_SHA2_sha512_ctx *state) {
	callbacks->SHA2_sha512_inc_ctx_release(state);
}

void OQS_SHA2_sha256(uint8_t *out, const uint8_t *in, size_t inlen) {
	callbacks->SHA2_sha256(out, in, inlen);
}

void OQS_SHA2_sha384(uint8_t *out, const uint8_t *in, size_t inlen) {
	callbacks->SHA2_sha384(out, in, inlen);
}

void OQS_SHA2_sha512(uint8_t *out, const uint8_t *in, size_t inlen) {
	callbacks->SHA2_sha512(out, in, inlen);
}

