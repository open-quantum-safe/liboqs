// SPDX-License-Identifier: MIT

#include <oqs/oqs.h>

#include "sha3x4.h"

#if OQS_USE_PTHREADS
#include <pthread.h>
#endif

extern struct OQS_SHA3_x4_callbacks sha3_x4_default_callbacks;

static const struct OQS_SHA3_x4_callbacks *callbacks = &sha3_x4_default_callbacks;

/* See sha3.c for the rationale: select the backend once, with a single
 * atomic pointer swap, before any SHA3-x4 entry point can dispatch. */
#if OQS_USE_PTHREADS
static pthread_once_t callbacks_init_once = PTHREAD_ONCE_INIT;
#else
static int callbacks_initialized = 0;
#endif

static void init_callbacks(void) {
#if defined(OQS_USE_SHA3_AVX512VL) && defined(OQS_DIST_X86_64_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX512)) {
		extern const struct OQS_SHA3_x4_callbacks sha3_x4_avx512vl_callbacks;
		callbacks = &sha3_x4_avx512vl_callbacks;
	}
#endif
}

static inline void ensure_callbacks_initialized(void) {
#if OQS_USE_PTHREADS
	pthread_once(&callbacks_init_once, init_callbacks);
#else
	if (!callbacks_initialized) {
		init_callbacks();
		callbacks_initialized = 1;
	}
#endif
}

static inline const struct OQS_SHA3_x4_callbacks *get_cb(void) {
	ensure_callbacks_initialized();
	return callbacks;
}

OQS_API void OQS_SHA3_x4_set_callbacks(struct OQS_SHA3_x4_callbacks *new_callbacks) {
	ensure_callbacks_initialized();
	callbacks = new_callbacks;
}

void OQS_SHA3_shake128_x4(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3, size_t outlen, const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inlen) {
	get_cb()->SHA3_shake128_x4(out0, out1, out2, out3, outlen, in0, in1, in2, in3, inlen);
}

void OQS_SHA3_shake128_x4_inc_init(OQS_SHA3_shake128_x4_inc_ctx *state) {
	get_cb()->SHA3_shake128_x4_inc_init(state);
}

void OQS_SHA3_shake128_x4_inc_absorb(OQS_SHA3_shake128_x4_inc_ctx *state, const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inlen) {
	get_cb()->SHA3_shake128_x4_inc_absorb(state, in0, in1, in2, in3, inlen);
}

void OQS_SHA3_shake128_x4_inc_finalize(OQS_SHA3_shake128_x4_inc_ctx *state) {
	get_cb()->SHA3_shake128_x4_inc_finalize(state);
}

void OQS_SHA3_shake128_x4_inc_squeeze(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3, size_t outlen, OQS_SHA3_shake128_x4_inc_ctx *state) {
	get_cb()->SHA3_shake128_x4_inc_squeeze(out0, out1, out2, out3, outlen, state);
}

void OQS_SHA3_shake128_x4_inc_ctx_release(OQS_SHA3_shake128_x4_inc_ctx *state) {
	get_cb()->SHA3_shake128_x4_inc_ctx_release(state);
}

void OQS_SHA3_shake128_x4_inc_ctx_clone(OQS_SHA3_shake128_x4_inc_ctx *dest, const OQS_SHA3_shake128_x4_inc_ctx *src) {
	get_cb()->SHA3_shake128_x4_inc_ctx_clone(dest, src);
}

void OQS_SHA3_shake128_x4_inc_ctx_reset(OQS_SHA3_shake128_x4_inc_ctx *state) {
	get_cb()->SHA3_shake128_x4_inc_ctx_reset(state);
}

void OQS_SHA3_shake256_x4(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3, size_t outlen, const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inlen) {
	get_cb()->SHA3_shake256_x4(out0, out1, out2, out3, outlen, in0, in1, in2, in3, inlen);
}

void OQS_SHA3_shake256_x4_inc_init(OQS_SHA3_shake256_x4_inc_ctx *state) {
	get_cb()->SHA3_shake256_x4_inc_init(state);
}

void OQS_SHA3_shake256_x4_inc_absorb(OQS_SHA3_shake256_x4_inc_ctx *state, const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inlen) {
	get_cb()->SHA3_shake256_x4_inc_absorb(state, in0, in1, in2, in3, inlen);
}

void OQS_SHA3_shake256_x4_inc_finalize(OQS_SHA3_shake256_x4_inc_ctx *state) {
	get_cb()->SHA3_shake256_x4_inc_finalize(state);
}

void OQS_SHA3_shake256_x4_inc_squeeze(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3, size_t outlen, OQS_SHA3_shake256_x4_inc_ctx *state) {
	get_cb()->SHA3_shake256_x4_inc_squeeze(out0, out1, out2, out3, outlen, state);
}

void OQS_SHA3_shake256_x4_inc_ctx_release(OQS_SHA3_shake256_x4_inc_ctx *state) {
	get_cb()->SHA3_shake256_x4_inc_ctx_release(state);
}

void OQS_SHA3_shake256_x4_inc_ctx_clone(OQS_SHA3_shake256_x4_inc_ctx *dest, const OQS_SHA3_shake256_x4_inc_ctx *src) {
	get_cb()->SHA3_shake256_x4_inc_ctx_clone(dest, src);
}

void OQS_SHA3_shake256_x4_inc_ctx_reset(OQS_SHA3_shake256_x4_inc_ctx *state) {
	get_cb()->SHA3_shake256_x4_inc_ctx_reset(state);
}
