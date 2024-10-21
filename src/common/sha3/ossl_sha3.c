/**
 * \file sha3_ossl.c
 * \brief Implementation of the OQS SHA3 API via calls to OpenSSL's SHA-3 functions
 *
 * SPDX-License-Identifier: MIT
 */

#include <oqs/oqs.h>

#ifdef OQS_USE_SHA3_OPENSSL

#include "sha3.h"

#include "../ossl_helpers.h"
#include <string.h>

static void do_hash(uint8_t *output, const uint8_t *input, size_t inplen, const EVP_MD *md) {
	EVP_MD_CTX *mdctx;
	mdctx = OSSL_FUNC(EVP_MD_CTX_new)();
	OSSL_FUNC(EVP_DigestInit_ex)(mdctx, md, NULL);
	OSSL_FUNC(EVP_DigestUpdate)(mdctx, input, inplen);
	OSSL_FUNC(EVP_DigestFinal_ex)(mdctx, output, NULL);
	OSSL_FUNC(EVP_MD_CTX_free)(mdctx);
}

static void do_xof(uint8_t *output, size_t outlen, const uint8_t *input, size_t inplen, const EVP_MD *md) {
	EVP_MD_CTX *mdctx;
	mdctx = OSSL_FUNC(EVP_MD_CTX_new)();
	OSSL_FUNC(EVP_DigestInit_ex)(mdctx, md, NULL);
	OSSL_FUNC(EVP_DigestUpdate)(mdctx, input, inplen);
	OSSL_FUNC(EVP_DigestFinalXOF)(mdctx, output, outlen);
	OSSL_FUNC(EVP_MD_CTX_free)(mdctx);
}

/* SHA3-256 */

static void SHA3_sha3_256(uint8_t *output, const uint8_t *input, size_t inplen) {
	do_hash(output, input, inplen, oqs_sha3_256());
}

/* SHA3-256 incremental */

static void SHA3_sha3_256_inc_init(OQS_SHA3_sha3_256_inc_ctx *state) {
	state->ctx = OSSL_FUNC(EVP_MD_CTX_new)();
	EVP_MD_CTX *s = (EVP_MD_CTX *)state->ctx;
	OSSL_FUNC(EVP_DigestInit_ex)(s, oqs_sha3_256(), NULL);
}

static void SHA3_sha3_256_inc_absorb(OQS_SHA3_sha3_256_inc_ctx *state, const uint8_t *input, size_t inplen) {
	OSSL_FUNC(EVP_DigestUpdate)((EVP_MD_CTX *)state->ctx, input, inplen);
}

static void SHA3_sha3_256_inc_finalize(uint8_t *output, OQS_SHA3_sha3_256_inc_ctx *state) {
	OSSL_FUNC(EVP_DigestFinal_ex)((EVP_MD_CTX *)state->ctx, output, NULL);
}

static void SHA3_sha3_256_inc_ctx_release(OQS_SHA3_sha3_256_inc_ctx *state) {
	OSSL_FUNC(EVP_MD_CTX_free)((EVP_MD_CTX *)state->ctx);
}

static void SHA3_sha3_256_inc_ctx_clone(OQS_SHA3_sha3_256_inc_ctx *dest, const OQS_SHA3_sha3_256_inc_ctx *src) {
	OSSL_FUNC(EVP_MD_CTX_copy_ex)((EVP_MD_CTX *)dest->ctx, (EVP_MD_CTX *)src->ctx);
}

static void SHA3_sha3_256_inc_ctx_reset(OQS_SHA3_sha3_256_inc_ctx *state) {
	EVP_MD_CTX *s = state->ctx;
	OSSL_FUNC(EVP_MD_CTX_reset)(s);
	OSSL_FUNC(EVP_DigestInit_ex)(s, oqs_sha3_256(), NULL);
}

/* SHA3-384 */

static void SHA3_sha3_384(uint8_t *output, const uint8_t *input, size_t inplen) {
	do_hash(output, input, inplen, oqs_sha3_384());
}

/* SHA3-384 incremental */
static void SHA3_sha3_384_inc_init(OQS_SHA3_sha3_384_inc_ctx *state) {
	state->ctx = OSSL_FUNC(EVP_MD_CTX_new)();
	OSSL_FUNC(EVP_DigestInit_ex)((EVP_MD_CTX *)state->ctx, oqs_sha3_384(), NULL);
}

static void SHA3_sha3_384_inc_absorb(OQS_SHA3_sha3_384_inc_ctx *state, const uint8_t *input, size_t inplen) {
	OSSL_FUNC(EVP_DigestUpdate)((EVP_MD_CTX *)state->ctx, input, inplen);
}

static void SHA3_sha3_384_inc_finalize(uint8_t *output, OQS_SHA3_sha3_384_inc_ctx *state) {
	OSSL_FUNC(EVP_DigestFinal_ex)((EVP_MD_CTX *)state->ctx, output, NULL);
}

static void SHA3_sha3_384_inc_ctx_release(OQS_SHA3_sha3_384_inc_ctx *state) {
	OSSL_FUNC(EVP_MD_CTX_free)((EVP_MD_CTX *)state->ctx);
}

static void SHA3_sha3_384_inc_ctx_clone(OQS_SHA3_sha3_384_inc_ctx *dest, const OQS_SHA3_sha3_384_inc_ctx *src) {
	OSSL_FUNC(EVP_MD_CTX_copy_ex)((EVP_MD_CTX *)dest->ctx, (EVP_MD_CTX *)src->ctx);
}

static void SHA3_sha3_384_inc_ctx_reset(OQS_SHA3_sha3_384_inc_ctx *state) {
	EVP_MD_CTX *s = state->ctx;
	OSSL_FUNC(EVP_MD_CTX_reset)(s);
	OSSL_FUNC(EVP_DigestInit_ex)(s, oqs_sha3_384(), NULL);
}

/* SHA3-512 */

static void SHA3_sha3_512(uint8_t *output, const uint8_t *input, size_t inplen) {
	do_hash(output, input, inplen, oqs_sha3_512());
}

/* SHA3-512 incremental */

static void SHA3_sha3_512_inc_init(OQS_SHA3_sha3_512_inc_ctx *state) {
	state->ctx = OSSL_FUNC(EVP_MD_CTX_new)();
	OSSL_FUNC(EVP_DigestInit_ex)((EVP_MD_CTX *)state->ctx, oqs_sha3_512(), NULL);
}

static void SHA3_sha3_512_inc_absorb(OQS_SHA3_sha3_512_inc_ctx *state, const uint8_t *input, size_t inplen) {
	OSSL_FUNC(EVP_DigestUpdate)((EVP_MD_CTX *)state->ctx, input, inplen);
}

static void SHA3_sha3_512_inc_finalize(uint8_t *output, OQS_SHA3_sha3_512_inc_ctx *state) {
	OSSL_FUNC(EVP_DigestFinal_ex)((EVP_MD_CTX *)state->ctx, output, NULL);
}

static void SHA3_sha3_512_inc_ctx_release(OQS_SHA3_sha3_512_inc_ctx *state) {
	OSSL_FUNC(EVP_MD_CTX_free)((EVP_MD_CTX *)state->ctx);
}

static void SHA3_sha3_512_inc_ctx_clone(OQS_SHA3_sha3_512_inc_ctx *dest, const OQS_SHA3_sha3_512_inc_ctx *src) {
	OSSL_FUNC(EVP_MD_CTX_copy_ex)((EVP_MD_CTX *)dest->ctx, (EVP_MD_CTX *)src->ctx);
}

static void SHA3_sha3_512_inc_ctx_reset(OQS_SHA3_sha3_512_inc_ctx *state) {
	EVP_MD_CTX *s = state->ctx;
	OSSL_FUNC(EVP_MD_CTX_reset)(s);
	OSSL_FUNC(EVP_DigestInit_ex)(s, oqs_sha3_512(), NULL);
}

/* SHAKE-128 */

static void SHA3_shake128(uint8_t *output, size_t outlen, const uint8_t *input, size_t inplen) {
	do_xof(output, outlen, input, inplen, oqs_shake128());
}

/* SHAKE-128 incremental
 *
 * Note: the comment below has been addressed in OpenSSL version 3.3.0
 *
 * XXX: The OpenSSL XOF API only allows for a single
 * call to squeeze (EVP_DigestFinalXOF).
 *
 * There is work in progress
 *    https://github.com/openssl/openssl/pull/7921
 * to fix this. For now we have to fake it.
 *
 * When we need to squeeze `k` new bytes from the state, we
 *      - clone the state,
 *      - dynamically allocate a buffer of size n+k,
 *      - call EVP_DigestFinalXOF(clone, internal buffer, n+k)
 *      - copy the last k bytes of the output back to the caller.
 * When n=0 we use the output buffer directly.
 */

typedef struct {
	EVP_MD_CTX *mdctx;
	size_t n_out;
} intrn_shake128_inc_ctx;

static void SHA3_shake128_inc_init(OQS_SHA3_shake128_inc_ctx *state) {
	state->ctx = OQS_MEM_malloc(sizeof(intrn_shake128_inc_ctx));

	intrn_shake128_inc_ctx *s = (intrn_shake128_inc_ctx *)state->ctx;
	s->mdctx = OSSL_FUNC(EVP_MD_CTX_new)();
	s->n_out = 0;
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx, oqs_shake128(), NULL);
}

static void SHA3_shake128_inc_absorb(OQS_SHA3_shake128_inc_ctx *state, const uint8_t *input, size_t inplen) {
	intrn_shake128_inc_ctx *s = (intrn_shake128_inc_ctx *)state->ctx;
	OSSL_FUNC(EVP_DigestUpdate)(s->mdctx, input, inplen);
}

static void SHA3_shake128_inc_finalize(OQS_SHA3_shake128_inc_ctx *state) {
	(void)state;
}

static void SHA3_shake128_inc_squeeze(uint8_t *output, size_t outlen, OQS_SHA3_shake128_inc_ctx *state) {
	intrn_shake128_inc_ctx *s = (intrn_shake128_inc_ctx *)state->ctx;
#if OPENSSL_VERSION_NUMBER >= 0x30300000L
	EVP_DigestSqueeze(s->mdctx, output, outlen);
#else
	EVP_MD_CTX *clone;

	clone = OSSL_FUNC(EVP_MD_CTX_new)();
	OSSL_FUNC(EVP_DigestInit_ex)(clone, oqs_shake128(), NULL);
	OSSL_FUNC(EVP_MD_CTX_copy_ex)(clone, s->mdctx);
	if (s->n_out == 0) {
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, output, outlen);
	} else {
		uint8_t *tmp = OQS_MEM_malloc(s->n_out + outlen);
		OQS_EXIT_IF_NULLPTR(tmp, "SHA3");
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, tmp, s->n_out + outlen);
		memcpy(output, tmp + s->n_out, outlen);
		OQS_MEM_insecure_free(tmp); // IGNORE free-check
	}
	OSSL_FUNC(EVP_MD_CTX_free)(clone);
	s->n_out += outlen;
#endif
}

static void SHA3_shake128_inc_ctx_release(OQS_SHA3_shake128_inc_ctx *state) {
	intrn_shake128_inc_ctx *s = (intrn_shake128_inc_ctx *)state->ctx;
	OSSL_FUNC(EVP_MD_CTX_free)(s->mdctx);
	OQS_MEM_insecure_free(s); // IGNORE free-check
}

static void SHA3_shake128_inc_ctx_clone(OQS_SHA3_shake128_inc_ctx *dest, const OQS_SHA3_shake128_inc_ctx *src) {
	intrn_shake128_inc_ctx *s = (intrn_shake128_inc_ctx *)src->ctx;
	intrn_shake128_inc_ctx *d = (intrn_shake128_inc_ctx *)dest->ctx;
	OSSL_FUNC(EVP_MD_CTX_copy_ex)(d->mdctx, s->mdctx);
	d->n_out = s->n_out;
}

static void SHA3_shake128_inc_ctx_reset(OQS_SHA3_shake128_inc_ctx *state) {
	intrn_shake128_inc_ctx *s = (intrn_shake128_inc_ctx *)state->ctx;
	OSSL_FUNC(EVP_MD_CTX_reset)(s->mdctx);
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx, oqs_shake128(), NULL);
	s->n_out = 0;
}

/* SHAKE-256 */

static void SHA3_shake256(uint8_t *output, size_t outlen, const uint8_t *input, size_t inplen) {
	do_xof(output, outlen, input, inplen, oqs_shake256());
}

/* SHAKE-256 incremental */

typedef struct {
	EVP_MD_CTX *mdctx;
	size_t n_out;
} intrn_shake256_inc_ctx;

static void SHA3_shake256_inc_init(OQS_SHA3_shake256_inc_ctx *state) {
	state->ctx = OQS_MEM_malloc(sizeof(intrn_shake256_inc_ctx));

	intrn_shake256_inc_ctx *s = (intrn_shake256_inc_ctx *)state->ctx;
	s->mdctx = OSSL_FUNC(EVP_MD_CTX_new)();
	s->n_out = 0;
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx, oqs_shake256(), NULL);
}

static void SHA3_shake256_inc_absorb(OQS_SHA3_shake256_inc_ctx *state, const uint8_t *input, size_t inplen) {
	intrn_shake256_inc_ctx *s = (intrn_shake256_inc_ctx *)state->ctx;
	OSSL_FUNC(EVP_DigestUpdate)(s->mdctx, input, inplen);
}

static void SHA3_shake256_inc_finalize(OQS_SHA3_shake256_inc_ctx *state) {
	(void)state;
}

static void SHA3_shake256_inc_squeeze(uint8_t *output, size_t outlen, OQS_SHA3_shake256_inc_ctx *state) {
	intrn_shake256_inc_ctx *s = (intrn_shake256_inc_ctx *)state->ctx;
#if OPENSSL_VERSION_NUMBER >= 0x30300000L
	EVP_DigestSqueeze(s->mdctx, output, outlen);
#else
	EVP_MD_CTX *clone;

	clone = OSSL_FUNC(EVP_MD_CTX_new)();
	OSSL_FUNC(EVP_DigestInit_ex)(clone, oqs_shake256(), NULL);
	OSSL_FUNC(EVP_MD_CTX_copy_ex)(clone, s->mdctx);
	if (s->n_out == 0) {
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, output, outlen);
	} else {
		uint8_t *tmp = OQS_MEM_malloc(s->n_out + outlen);
		OQS_EXIT_IF_NULLPTR(tmp, "SHA3");
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, tmp, s->n_out + outlen);
		memcpy(output, tmp + s->n_out, outlen);
		OQS_MEM_insecure_free(tmp); // IGNORE free-check
	}
	OSSL_FUNC(EVP_MD_CTX_free)(clone);
	s->n_out += outlen;
#endif
}

static void SHA3_shake256_inc_ctx_release(OQS_SHA3_shake256_inc_ctx *state) {
	intrn_shake256_inc_ctx *s = (intrn_shake256_inc_ctx *)state->ctx;
	OSSL_FUNC(EVP_MD_CTX_free)(s->mdctx);
	OQS_MEM_insecure_free(s); // IGNORE free-check
}

static void SHA3_shake256_inc_ctx_clone(OQS_SHA3_shake256_inc_ctx *dest, const OQS_SHA3_shake256_inc_ctx *src) {
	intrn_shake256_inc_ctx *s = (intrn_shake256_inc_ctx *)src->ctx;
	intrn_shake256_inc_ctx *d = (intrn_shake256_inc_ctx *)dest->ctx;
	OSSL_FUNC(EVP_MD_CTX_copy_ex)(d->mdctx, s->mdctx);
	d->n_out = s->n_out;
}

static void SHA3_shake256_inc_ctx_reset(OQS_SHA3_shake256_inc_ctx *state) {
	intrn_shake256_inc_ctx *s = (intrn_shake256_inc_ctx *)state->ctx;
	OSSL_FUNC(EVP_MD_CTX_reset)(s->mdctx);
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx, oqs_shake256(), NULL);
	s->n_out = 0;
}

extern struct OQS_SHA3_callbacks sha3_default_callbacks;

struct OQS_SHA3_callbacks sha3_default_callbacks = {
	SHA3_sha3_256,
	SHA3_sha3_256_inc_init,
	SHA3_sha3_256_inc_absorb,
	SHA3_sha3_256_inc_finalize,
	SHA3_sha3_256_inc_ctx_release,
	SHA3_sha3_256_inc_ctx_reset,
	SHA3_sha3_256_inc_ctx_clone,
	SHA3_sha3_384,
	SHA3_sha3_384_inc_init,
	SHA3_sha3_384_inc_absorb,
	SHA3_sha3_384_inc_finalize,
	SHA3_sha3_384_inc_ctx_release,
	SHA3_sha3_384_inc_ctx_reset,
	SHA3_sha3_384_inc_ctx_clone,
	SHA3_sha3_512,
	SHA3_sha3_512_inc_init,
	SHA3_sha3_512_inc_absorb,
	SHA3_sha3_512_inc_finalize,
	SHA3_sha3_512_inc_ctx_release,
	SHA3_sha3_512_inc_ctx_reset,
	SHA3_sha3_512_inc_ctx_clone,
	SHA3_shake128,
	SHA3_shake128_inc_init,
	SHA3_shake128_inc_absorb,
	SHA3_shake128_inc_finalize,
	SHA3_shake128_inc_squeeze,
	SHA3_shake128_inc_ctx_release,
	SHA3_shake128_inc_ctx_clone,
	SHA3_shake128_inc_ctx_reset,
	SHA3_shake256,
	SHA3_shake256_inc_init,
	SHA3_shake256_inc_absorb,
	SHA3_shake256_inc_finalize,
	SHA3_shake256_inc_squeeze,
	SHA3_shake256_inc_ctx_release,
	SHA3_shake256_inc_ctx_clone,
	SHA3_shake256_inc_ctx_reset,
};

#endif
