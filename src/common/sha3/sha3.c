// SPDX-License-Identifier: MIT

#include <oqs/oqs.h>

#include "sha3.h"

extern struct OQS_SHA3_callbacks sha3_default_callbacks;

static struct OQS_SHA3_callbacks *callbacks = &sha3_default_callbacks;

OQS_API void OQS_SHA3_set_callbacks(struct OQS_SHA3_callbacks *new_callbacks) {
	callbacks = new_callbacks;
}

void OQS_SHA3_sha3_256(uint8_t *output, const uint8_t *input, size_t inplen) {
	callbacks->SHA3_sha3_256(output, input, inplen);
}

void OQS_SHA3_sha3_256_inc_init(OQS_SHA3_sha3_256_inc_ctx *state) {
	callbacks->SHA3_sha3_256_inc_init(state);
}

void OQS_SHA3_sha3_256_inc_absorb(OQS_SHA3_sha3_256_inc_ctx *state, const uint8_t *input, size_t inlen) {
	callbacks->SHA3_sha3_256_inc_absorb(state, input, inlen);
}

void OQS_SHA3_sha3_256_inc_finalize(uint8_t *output, OQS_SHA3_sha3_256_inc_ctx *state) {
	callbacks->SHA3_sha3_256_inc_finalize(output, state);
}

void OQS_SHA3_sha3_256_inc_ctx_release(OQS_SHA3_sha3_256_inc_ctx *state) {
	callbacks->SHA3_sha3_256_inc_ctx_release(state);
}

void OQS_SHA3_sha3_256_inc_ctx_reset(OQS_SHA3_sha3_256_inc_ctx *state) {
	callbacks->SHA3_sha3_256_inc_ctx_reset(state);
}

void OQS_SHA3_sha3_256_inc_ctx_clone(OQS_SHA3_sha3_256_inc_ctx *dest, const OQS_SHA3_sha3_256_inc_ctx *src) {
	callbacks->SHA3_sha3_256_inc_ctx_clone(dest, src);
}

void OQS_SHA3_sha3_384(uint8_t *output, const uint8_t *input, size_t inplen) {
	callbacks->SHA3_sha3_384(output, input, inplen);
}

void OQS_SHA3_sha3_384_inc_init(OQS_SHA3_sha3_384_inc_ctx *state) {
	callbacks->SHA3_sha3_384_inc_init(state);
}

void OQS_SHA3_sha3_384_inc_absorb(OQS_SHA3_sha3_384_inc_ctx *state, const uint8_t *input, size_t inlen) {
	callbacks->SHA3_sha3_384_inc_absorb(state, input, inlen);
}

void OQS_SHA3_sha3_384_inc_finalize(uint8_t *output, OQS_SHA3_sha3_384_inc_ctx *state) {
	callbacks->SHA3_sha3_384_inc_finalize(output, state);
}

void OQS_SHA3_sha3_384_inc_ctx_release(OQS_SHA3_sha3_384_inc_ctx *state) {
	callbacks->SHA3_sha3_384_inc_ctx_release(state);
}

void OQS_SHA3_sha3_384_inc_ctx_reset(OQS_SHA3_sha3_384_inc_ctx *state) {
	callbacks->SHA3_sha3_384_inc_ctx_reset(state);
}

void OQS_SHA3_sha3_384_inc_ctx_clone(OQS_SHA3_sha3_384_inc_ctx *dest, const OQS_SHA3_sha3_384_inc_ctx *src) {
	callbacks->SHA3_sha3_384_inc_ctx_clone(dest, src);
}

void OQS_SHA3_sha3_512(uint8_t *output, const uint8_t *input, size_t inplen) {
	callbacks->SHA3_sha3_512(output, input, inplen);
}

void OQS_SHA3_sha3_512_inc_init(OQS_SHA3_sha3_512_inc_ctx *state) {
	callbacks->SHA3_sha3_512_inc_init(state);
}

void OQS_SHA3_sha3_512_inc_absorb(OQS_SHA3_sha3_512_inc_ctx *state, const uint8_t *input, size_t inlen) {
	callbacks->SHA3_sha3_512_inc_absorb(state, input, inlen);
}

void OQS_SHA3_sha3_512_inc_finalize(uint8_t *output, OQS_SHA3_sha3_512_inc_ctx *state) {
	callbacks->SHA3_sha3_512_inc_finalize(output, state);
}

void OQS_SHA3_sha3_512_inc_ctx_release(OQS_SHA3_sha3_512_inc_ctx *state) {
	callbacks->SHA3_sha3_512_inc_ctx_release(state);
}

void OQS_SHA3_sha3_512_inc_ctx_reset(OQS_SHA3_sha3_512_inc_ctx *state) {
	callbacks->SHA3_sha3_512_inc_ctx_reset(state);
}

void OQS_SHA3_sha3_512_inc_ctx_clone(OQS_SHA3_sha3_512_inc_ctx *dest, const OQS_SHA3_sha3_512_inc_ctx *src) {
	callbacks->SHA3_sha3_512_inc_ctx_clone(dest, src);
}

void OQS_SHA3_shake128(uint8_t *output, size_t outlen, const uint8_t *input, size_t inplen) {
	callbacks->SHA3_shake128(output, outlen, input, inplen);
}

void OQS_SHA3_shake128_inc_init(OQS_SHA3_shake128_inc_ctx *state) {
	callbacks->SHA3_shake128_inc_init(state);
}

void OQS_SHA3_shake128_inc_absorb(OQS_SHA3_shake128_inc_ctx *state, const uint8_t *input, size_t inlen) {
	callbacks->SHA3_shake128_inc_absorb(state, input, inlen);
}

void OQS_SHA3_shake128_inc_finalize(OQS_SHA3_shake128_inc_ctx *state) {
	callbacks->SHA3_shake128_inc_finalize(state);
}

void OQS_SHA3_shake128_inc_squeeze(uint8_t *output, size_t outlen, OQS_SHA3_shake128_inc_ctx *state) {
	callbacks->SHA3_shake128_inc_squeeze(output, outlen, state);
}

void OQS_SHA3_shake128_inc_ctx_release(OQS_SHA3_shake128_inc_ctx *state) {
	callbacks->SHA3_shake128_inc_ctx_release(state);
}

void OQS_SHA3_shake128_inc_ctx_clone(OQS_SHA3_shake128_inc_ctx *dest, const OQS_SHA3_shake128_inc_ctx *src) {
	callbacks->SHA3_shake128_inc_ctx_clone(dest, src);
}

void OQS_SHA3_shake128_inc_ctx_reset(OQS_SHA3_shake128_inc_ctx *state) {
	callbacks->SHA3_shake128_inc_ctx_reset(state);
}

void OQS_SHA3_shake256(uint8_t *output, size_t outlen, const uint8_t *input, size_t inplen) {
	callbacks->SHA3_shake256(output, outlen, input, inplen);
}

void OQS_SHA3_shake256_inc_init(OQS_SHA3_shake256_inc_ctx *state) {
	callbacks->SHA3_shake256_inc_init(state);
}

void OQS_SHA3_shake256_inc_absorb(OQS_SHA3_shake256_inc_ctx *state, const uint8_t *input, size_t inlen) {
	callbacks->SHA3_shake256_inc_absorb(state, input, inlen);
}

void OQS_SHA3_shake256_inc_finalize(OQS_SHA3_shake256_inc_ctx *state) {
	callbacks->SHA3_shake256_inc_finalize(state);
}

void OQS_SHA3_shake256_inc_squeeze(uint8_t *output, size_t outlen, OQS_SHA3_shake256_inc_ctx *state) {
	callbacks->SHA3_shake256_inc_squeeze(output, outlen, state);
}

void OQS_SHA3_shake256_inc_ctx_release(OQS_SHA3_shake256_inc_ctx *state) {
	callbacks->SHA3_shake256_inc_ctx_release(state);
}

void OQS_SHA3_shake256_inc_ctx_clone(OQS_SHA3_shake256_inc_ctx *dest, const OQS_SHA3_shake256_inc_ctx *src) {
	callbacks->SHA3_shake256_inc_ctx_clone(dest, src);
}

void OQS_SHA3_shake256_inc_ctx_reset(OQS_SHA3_shake256_inc_ctx *state) {
	callbacks->SHA3_shake256_inc_ctx_reset(state);
}
