// SPDX-License-Identifier: MIT

#include <oqs/oqs.h>

#include "sha3.h"

#if OQS_USE_PTHREADS
#include <pthread.h>
#endif

extern struct OQS_SHA3_callbacks sha3_default_callbacks;

static struct OQS_SHA3_callbacks *callbacks = &sha3_default_callbacks;

/* The top-level SHA3 backend is selected once, before any SHA3 entry point
 * is allowed to dispatch. We swap the `callbacks` pointer atomically (a
 * single aligned-pointer store) rather than overwriting the contents of the
 * default callbacks struct, so concurrent readers never observe a torn
 * vtable, and a single in-flight call cannot be split across two backends.
 */
#if OQS_USE_PTHREADS
static pthread_once_t callbacks_init_once = PTHREAD_ONCE_INIT;
#else
static int callbacks_initialized = 0;
#endif

static void init_callbacks(void) {
#if defined(OQS_USE_SHA3_AVX512VL) && defined(OQS_DIST_X86_64_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX512)) {
		extern const struct OQS_SHA3_callbacks sha3_avx512vl_callbacks;
		callbacks = (struct OQS_SHA3_callbacks *) &sha3_avx512vl_callbacks;
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

static inline struct OQS_SHA3_callbacks *get_cb(void) {
	ensure_callbacks_initialized();
	return callbacks;
}

OQS_API void OQS_SHA3_set_callbacks(struct OQS_SHA3_callbacks *new_callbacks) {
	/* Ensure the one-time auto-detection has run first, so a later SHA3
	 * call cannot clobber the user's choice. */
	ensure_callbacks_initialized();
	callbacks = new_callbacks;
}

void OQS_SHA3_sha3_256(uint8_t *output, const uint8_t *input, size_t inplen) {
	get_cb()->SHA3_sha3_256(output, input, inplen);
}

void OQS_SHA3_sha3_256_inc_init(OQS_SHA3_sha3_256_inc_ctx *state) {
	get_cb()->SHA3_sha3_256_inc_init(state);
}

void OQS_SHA3_sha3_256_inc_absorb(OQS_SHA3_sha3_256_inc_ctx *state, const uint8_t *input, size_t inlen) {
	get_cb()->SHA3_sha3_256_inc_absorb(state, input, inlen);
}

void OQS_SHA3_sha3_256_inc_finalize(uint8_t *output, OQS_SHA3_sha3_256_inc_ctx *state) {
	get_cb()->SHA3_sha3_256_inc_finalize(output, state);
}

void OQS_SHA3_sha3_256_inc_ctx_release(OQS_SHA3_sha3_256_inc_ctx *state) {
	get_cb()->SHA3_sha3_256_inc_ctx_release(state);
}

void OQS_SHA3_sha3_256_inc_ctx_reset(OQS_SHA3_sha3_256_inc_ctx *state) {
	get_cb()->SHA3_sha3_256_inc_ctx_reset(state);
}

void OQS_SHA3_sha3_256_inc_ctx_clone(OQS_SHA3_sha3_256_inc_ctx *dest, const OQS_SHA3_sha3_256_inc_ctx *src) {
	get_cb()->SHA3_sha3_256_inc_ctx_clone(dest, src);
}

void OQS_SHA3_sha3_384(uint8_t *output, const uint8_t *input, size_t inplen) {
	get_cb()->SHA3_sha3_384(output, input, inplen);
}

void OQS_SHA3_sha3_384_inc_init(OQS_SHA3_sha3_384_inc_ctx *state) {
	get_cb()->SHA3_sha3_384_inc_init(state);
}

void OQS_SHA3_sha3_384_inc_absorb(OQS_SHA3_sha3_384_inc_ctx *state, const uint8_t *input, size_t inlen) {
	get_cb()->SHA3_sha3_384_inc_absorb(state, input, inlen);
}

void OQS_SHA3_sha3_384_inc_finalize(uint8_t *output, OQS_SHA3_sha3_384_inc_ctx *state) {
	get_cb()->SHA3_sha3_384_inc_finalize(output, state);
}

void OQS_SHA3_sha3_384_inc_ctx_release(OQS_SHA3_sha3_384_inc_ctx *state) {
	get_cb()->SHA3_sha3_384_inc_ctx_release(state);
}

void OQS_SHA3_sha3_384_inc_ctx_reset(OQS_SHA3_sha3_384_inc_ctx *state) {
	get_cb()->SHA3_sha3_384_inc_ctx_reset(state);
}

void OQS_SHA3_sha3_384_inc_ctx_clone(OQS_SHA3_sha3_384_inc_ctx *dest, const OQS_SHA3_sha3_384_inc_ctx *src) {
	get_cb()->SHA3_sha3_384_inc_ctx_clone(dest, src);
}

void OQS_SHA3_sha3_512(uint8_t *output, const uint8_t *input, size_t inplen) {
	get_cb()->SHA3_sha3_512(output, input, inplen);
}

void OQS_SHA3_sha3_512_inc_init(OQS_SHA3_sha3_512_inc_ctx *state) {
	get_cb()->SHA3_sha3_512_inc_init(state);
}

void OQS_SHA3_sha3_512_inc_absorb(OQS_SHA3_sha3_512_inc_ctx *state, const uint8_t *input, size_t inlen) {
	get_cb()->SHA3_sha3_512_inc_absorb(state, input, inlen);
}

void OQS_SHA3_sha3_512_inc_finalize(uint8_t *output, OQS_SHA3_sha3_512_inc_ctx *state) {
	get_cb()->SHA3_sha3_512_inc_finalize(output, state);
}

void OQS_SHA3_sha3_512_inc_ctx_release(OQS_SHA3_sha3_512_inc_ctx *state) {
	get_cb()->SHA3_sha3_512_inc_ctx_release(state);
}

void OQS_SHA3_sha3_512_inc_ctx_reset(OQS_SHA3_sha3_512_inc_ctx *state) {
	get_cb()->SHA3_sha3_512_inc_ctx_reset(state);
}

void OQS_SHA3_sha3_512_inc_ctx_clone(OQS_SHA3_sha3_512_inc_ctx *dest, const OQS_SHA3_sha3_512_inc_ctx *src) {
	get_cb()->SHA3_sha3_512_inc_ctx_clone(dest, src);
}

void OQS_SHA3_shake128(uint8_t *output, size_t outlen, const uint8_t *input, size_t inplen) {
	get_cb()->SHA3_shake128(output, outlen, input, inplen);
}

void OQS_SHA3_shake128_inc_init(OQS_SHA3_shake128_inc_ctx *state) {
	get_cb()->SHA3_shake128_inc_init(state);
}

void OQS_SHA3_shake128_inc_absorb(OQS_SHA3_shake128_inc_ctx *state, const uint8_t *input, size_t inlen) {
	get_cb()->SHA3_shake128_inc_absorb(state, input, inlen);
}

void OQS_SHA3_shake128_inc_finalize(OQS_SHA3_shake128_inc_ctx *state) {
	get_cb()->SHA3_shake128_inc_finalize(state);
}

void OQS_SHA3_shake128_inc_squeeze(uint8_t *output, size_t outlen, OQS_SHA3_shake128_inc_ctx *state) {
	get_cb()->SHA3_shake128_inc_squeeze(output, outlen, state);
}

void OQS_SHA3_shake128_inc_ctx_release(OQS_SHA3_shake128_inc_ctx *state) {
	get_cb()->SHA3_shake128_inc_ctx_release(state);
}

void OQS_SHA3_shake128_inc_ctx_clone(OQS_SHA3_shake128_inc_ctx *dest, const OQS_SHA3_shake128_inc_ctx *src) {
	get_cb()->SHA3_shake128_inc_ctx_clone(dest, src);
}

void OQS_SHA3_shake128_inc_ctx_reset(OQS_SHA3_shake128_inc_ctx *state) {
	get_cb()->SHA3_shake128_inc_ctx_reset(state);
}

void OQS_SHA3_shake256(uint8_t *output, size_t outlen, const uint8_t *input, size_t inplen) {
	get_cb()->SHA3_shake256(output, outlen, input, inplen);
}

void OQS_SHA3_shake256_inc_init(OQS_SHA3_shake256_inc_ctx *state) {
	get_cb()->SHA3_shake256_inc_init(state);
}

void OQS_SHA3_shake256_inc_absorb(OQS_SHA3_shake256_inc_ctx *state, const uint8_t *input, size_t inlen) {
	get_cb()->SHA3_shake256_inc_absorb(state, input, inlen);
}

void OQS_SHA3_shake256_inc_finalize(OQS_SHA3_shake256_inc_ctx *state) {
	get_cb()->SHA3_shake256_inc_finalize(state);
}

void OQS_SHA3_shake256_inc_squeeze(uint8_t *output, size_t outlen, OQS_SHA3_shake256_inc_ctx *state) {
	get_cb()->SHA3_shake256_inc_squeeze(output, outlen, state);
}

void OQS_SHA3_shake256_inc_ctx_release(OQS_SHA3_shake256_inc_ctx *state) {
	get_cb()->SHA3_shake256_inc_ctx_release(state);
}

void OQS_SHA3_shake256_inc_ctx_clone(OQS_SHA3_shake256_inc_ctx *dest, const OQS_SHA3_shake256_inc_ctx *src) {
	get_cb()->SHA3_shake256_inc_ctx_clone(dest, src);
}

void OQS_SHA3_shake256_inc_ctx_reset(OQS_SHA3_shake256_inc_ctx *state) {
	get_cb()->SHA3_shake256_inc_ctx_reset(state);
}
