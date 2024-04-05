// SPDX-License-Identifier: MIT

#include <oqs/oqs.h>

#include "sha3x4.h"

extern struct OQS_SHA3_x4_callbacks sha3_x4_default_callbacks;

static struct OQS_SHA3_x4_callbacks *callbacks = &sha3_x4_default_callbacks;

OQS_API void OQS_SHA3_x4_set_callbacks(struct OQS_SHA3_x4_callbacks *new_callbacks) {
	callbacks = new_callbacks;
}

void OQS_SHA3_shake128_x4(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3, size_t outlen, const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inlen) {
	callbacks->SHA3_shake128_x4(out0, out1, out2, out3, outlen, in0, in1, in2, in3, inlen);
}

void OQS_SHA3_shake128_x4_inc_init(OQS_SHA3_shake128_x4_inc_ctx *state) {
	callbacks->SHA3_shake128_x4_inc_init(state);
}

void OQS_SHA3_shake128_x4_inc_absorb(OQS_SHA3_shake128_x4_inc_ctx *state, const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inlen) {
	callbacks->SHA3_shake128_x4_inc_absorb(state, in0, in1, in2, in3, inlen);
}

void OQS_SHA3_shake128_x4_inc_finalize(OQS_SHA3_shake128_x4_inc_ctx *state) {
	callbacks->SHA3_shake128_x4_inc_finalize(state);
}

void OQS_SHA3_shake128_x4_inc_squeeze(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3, size_t outlen, OQS_SHA3_shake128_x4_inc_ctx *state) {
	callbacks->SHA3_shake128_x4_inc_squeeze(out0, out1, out2, out3, outlen, state);
}

void OQS_SHA3_shake128_x4_inc_ctx_release(OQS_SHA3_shake128_x4_inc_ctx *state) {
	callbacks->SHA3_shake128_x4_inc_ctx_release(state);
}

void OQS_SHA3_shake128_x4_inc_ctx_clone(OQS_SHA3_shake128_x4_inc_ctx *dest, const OQS_SHA3_shake128_x4_inc_ctx *src) {
	callbacks->SHA3_shake128_x4_inc_ctx_clone(dest, src);
}

void OQS_SHA3_shake128_x4_inc_ctx_reset(OQS_SHA3_shake128_x4_inc_ctx *state) {
	callbacks->SHA3_shake128_x4_inc_ctx_reset(state);
}

void OQS_SHA3_shake256_x4(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3, size_t outlen, const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inlen) {
	callbacks->SHA3_shake256_x4(out0, out1, out2, out3, outlen, in0, in1, in2, in3, inlen);
}

void OQS_SHA3_shake256_x4_inc_init(OQS_SHA3_shake256_x4_inc_ctx *state) {
	callbacks->SHA3_shake256_x4_inc_init(state);
}

void OQS_SHA3_shake256_x4_inc_absorb(OQS_SHA3_shake256_x4_inc_ctx *state, const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inlen) {
	callbacks->SHA3_shake256_x4_inc_absorb(state, in0, in1, in2, in3, inlen);
}

void OQS_SHA3_shake256_x4_inc_finalize(OQS_SHA3_shake256_x4_inc_ctx *state) {
	callbacks->SHA3_shake256_x4_inc_finalize(state);
}

void OQS_SHA3_shake256_x4_inc_squeeze(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3, size_t outlen, OQS_SHA3_shake256_x4_inc_ctx *state) {
	callbacks->SHA3_shake256_x4_inc_squeeze(out0, out1, out2, out3, outlen, state);
}

void OQS_SHA3_shake256_x4_inc_ctx_release(OQS_SHA3_shake256_x4_inc_ctx *state) {
	callbacks->SHA3_shake256_x4_inc_ctx_release(state);
}

void OQS_SHA3_shake256_x4_inc_ctx_clone(OQS_SHA3_shake256_x4_inc_ctx *dest, const OQS_SHA3_shake256_x4_inc_ctx *src) {
	callbacks->SHA3_shake256_x4_inc_ctx_clone(dest, src);
}

void OQS_SHA3_shake256_x4_inc_ctx_reset(OQS_SHA3_shake256_x4_inc_ctx *state) {
	callbacks->SHA3_shake256_x4_inc_ctx_reset(state);
}
