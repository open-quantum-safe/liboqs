// SPDX-License-Identifier: MIT

#include <oqs/oqs.h>

#ifdef OQS_USE_SHA3_OPENSSL

#include "sha3.h"
#include "sha3x4.h"

#include <openssl/evp.h>
#include "../ossl_helpers.h"

#include <string.h>

/* SHAKE-128 */

static void SHA3_shake128_x4(uint8_t *output0, uint8_t *output1, uint8_t *output2, uint8_t *output3, size_t outlen,
                             const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inplen) {
	OQS_SHA3_shake128(output0, outlen, in0, inplen);
	OQS_SHA3_shake128(output1, outlen, in1, inplen);
	OQS_SHA3_shake128(output2, outlen, in2, inplen);
	OQS_SHA3_shake128(output3, outlen, in3, inplen);
}

/* SHAKE128 incremental */

typedef struct {
	EVP_MD_CTX *mdctx0;
	EVP_MD_CTX *mdctx1;
	EVP_MD_CTX *mdctx2;
	EVP_MD_CTX *mdctx3;
	size_t n_out;
} intrn_shake128_x4_inc_ctx;

static void SHA3_shake128_x4_inc_init(OQS_SHA3_shake128_x4_inc_ctx *state) {
	state->ctx = OQS_MEM_malloc(sizeof(intrn_shake128_x4_inc_ctx));

	intrn_shake128_x4_inc_ctx *s = (intrn_shake128_x4_inc_ctx *)state->ctx;
	s->mdctx0 = OSSL_FUNC(EVP_MD_CTX_new)();
	s->mdctx1 = OSSL_FUNC(EVP_MD_CTX_new)();
	s->mdctx2 = OSSL_FUNC(EVP_MD_CTX_new)();
	s->mdctx3 = OSSL_FUNC(EVP_MD_CTX_new)();
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx0, oqs_shake128(), NULL);
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx1, oqs_shake128(), NULL);
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx2, oqs_shake128(), NULL);
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx3, oqs_shake128(), NULL);
	s->n_out = 0;
}

static void SHA3_shake128_x4_inc_absorb(OQS_SHA3_shake128_x4_inc_ctx *state, const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inplen) {
	intrn_shake128_x4_inc_ctx *s = (intrn_shake128_x4_inc_ctx *)state->ctx;
	OSSL_FUNC(EVP_DigestUpdate)(s->mdctx0, in0, inplen);
	OSSL_FUNC(EVP_DigestUpdate)(s->mdctx1, in1, inplen);
	OSSL_FUNC(EVP_DigestUpdate)(s->mdctx2, in2, inplen);
	OSSL_FUNC(EVP_DigestUpdate)(s->mdctx3, in3, inplen);
}

static void SHA3_shake128_x4_inc_finalize(OQS_SHA3_shake128_x4_inc_ctx *state) {
	(void)state;
}

static void SHA3_shake128_x4_inc_squeeze(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3, size_t outlen, OQS_SHA3_shake128_x4_inc_ctx *state) {
	intrn_shake128_x4_inc_ctx *s = (intrn_shake128_x4_inc_ctx *)state->ctx;
#if OPENSSL_VERSION_NUMBER >= 0x30300000L
	EVP_DigestSqueeze(s->mdctx0, out0, outlen);
	EVP_DigestSqueeze(s->mdctx1, out1, outlen);
	EVP_DigestSqueeze(s->mdctx2, out2, outlen);
	EVP_DigestSqueeze(s->mdctx3, out3, outlen);
#else
	EVP_MD_CTX *clone;

	clone = OSSL_FUNC(EVP_MD_CTX_new)();
	OSSL_FUNC(EVP_DigestInit_ex)(clone, oqs_shake128(), NULL);
	if (s->n_out == 0) {
		OSSL_FUNC(EVP_MD_CTX_copy_ex)(clone, s->mdctx0);
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, out0, outlen);
		OSSL_FUNC(EVP_MD_CTX_copy_ex)(clone, s->mdctx1);
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, out1, outlen);
		OSSL_FUNC(EVP_MD_CTX_copy_ex)(clone, s->mdctx2);
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, out2, outlen);
		OSSL_FUNC(EVP_MD_CTX_copy_ex)(clone, s->mdctx3);
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, out3, outlen);
	} else {
		uint8_t *tmp = OQS_MEM_malloc(s->n_out + outlen);
		OQS_EXIT_IF_NULLPTR(tmp, "SHA3x4");
		OSSL_FUNC(EVP_MD_CTX_copy_ex)(clone, s->mdctx0);
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, tmp, s->n_out + outlen);
		memcpy(out0, tmp + s->n_out, outlen);
		OSSL_FUNC(EVP_MD_CTX_copy_ex)(clone, s->mdctx1);
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, tmp, s->n_out + outlen);
		memcpy(out1, tmp + s->n_out, outlen);
		OSSL_FUNC(EVP_MD_CTX_copy_ex)(clone, s->mdctx2);
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, tmp, s->n_out + outlen);
		memcpy(out2, tmp + s->n_out, outlen);
		OSSL_FUNC(EVP_MD_CTX_copy_ex)(clone, s->mdctx3);
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, tmp, s->n_out + outlen);
		memcpy(out3, tmp + s->n_out, outlen);
		OQS_MEM_insecure_free(tmp);
	}
	OSSL_FUNC(EVP_MD_CTX_free)(clone);
	s->n_out += outlen;
#endif
}

static void SHA3_shake128_x4_inc_ctx_clone(OQS_SHA3_shake128_x4_inc_ctx *dest, const OQS_SHA3_shake128_x4_inc_ctx *src) {
	intrn_shake128_x4_inc_ctx *s = (intrn_shake128_x4_inc_ctx *)src->ctx;
	intrn_shake128_x4_inc_ctx *d = (intrn_shake128_x4_inc_ctx *)dest->ctx;
	OSSL_FUNC(EVP_MD_CTX_copy_ex)(d->mdctx0, s->mdctx0);
	OSSL_FUNC(EVP_MD_CTX_copy_ex)(d->mdctx1, s->mdctx1);
	OSSL_FUNC(EVP_MD_CTX_copy_ex)(d->mdctx2, s->mdctx2);
	OSSL_FUNC(EVP_MD_CTX_copy_ex)(d->mdctx3, s->mdctx3);
	d->n_out = s->n_out;
}

static void SHA3_shake128_x4_inc_ctx_release(OQS_SHA3_shake128_x4_inc_ctx *state) {
	intrn_shake128_x4_inc_ctx *s = (intrn_shake128_x4_inc_ctx *)state->ctx;
	OSSL_FUNC(EVP_MD_CTX_free)(s->mdctx0);
	OSSL_FUNC(EVP_MD_CTX_free)(s->mdctx1);
	OSSL_FUNC(EVP_MD_CTX_free)(s->mdctx2);
	OSSL_FUNC(EVP_MD_CTX_free)(s->mdctx3);
	OQS_MEM_insecure_free(s);
}

static void SHA3_shake128_x4_inc_ctx_reset(OQS_SHA3_shake128_x4_inc_ctx *state) {
	intrn_shake128_x4_inc_ctx *s = (intrn_shake128_x4_inc_ctx *)state->ctx;
	OSSL_FUNC(EVP_MD_CTX_reset)(s->mdctx0);
	OSSL_FUNC(EVP_MD_CTX_reset)(s->mdctx1);
	OSSL_FUNC(EVP_MD_CTX_reset)(s->mdctx2);
	OSSL_FUNC(EVP_MD_CTX_reset)(s->mdctx3);
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx0, oqs_shake128(), NULL);
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx1, oqs_shake128(), NULL);
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx2, oqs_shake128(), NULL);
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx3, oqs_shake128(), NULL);
	s->n_out = 0;
}

/* SHAKE-256 */

static void SHA3_shake256_x4(uint8_t *output0, uint8_t *output1, uint8_t *output2, uint8_t *output3, size_t outlen,
                             const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inplen) {
	OQS_SHA3_shake256(output0, outlen, in0, inplen);
	OQS_SHA3_shake256(output1, outlen, in1, inplen);
	OQS_SHA3_shake256(output2, outlen, in2, inplen);
	OQS_SHA3_shake256(output3, outlen, in3, inplen);
}

/* SHAKE256 incremental */

typedef struct {
	EVP_MD_CTX *mdctx0;
	EVP_MD_CTX *mdctx1;
	EVP_MD_CTX *mdctx2;
	EVP_MD_CTX *mdctx3;
	size_t n_out;
} intrn_shake256_x4_inc_ctx;

static void SHA3_shake256_x4_inc_init(OQS_SHA3_shake256_x4_inc_ctx *state) {
	state->ctx = OQS_MEM_malloc(sizeof(intrn_shake256_x4_inc_ctx));

	intrn_shake256_x4_inc_ctx *s = (intrn_shake256_x4_inc_ctx *)state->ctx;
	s->mdctx0 = OSSL_FUNC(EVP_MD_CTX_new)();
	s->mdctx1 = OSSL_FUNC(EVP_MD_CTX_new)();
	s->mdctx2 = OSSL_FUNC(EVP_MD_CTX_new)();
	s->mdctx3 = OSSL_FUNC(EVP_MD_CTX_new)();
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx0, oqs_shake256(), NULL);
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx1, oqs_shake256(), NULL);
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx2, oqs_shake256(), NULL);
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx3, oqs_shake256(), NULL);
	s->n_out = 0;
}

static void SHA3_shake256_x4_inc_absorb(OQS_SHA3_shake256_x4_inc_ctx *state, const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inplen) {
	intrn_shake256_x4_inc_ctx *s = (intrn_shake256_x4_inc_ctx *)state->ctx;
	OSSL_FUNC(EVP_DigestUpdate)(s->mdctx0, in0, inplen);
	OSSL_FUNC(EVP_DigestUpdate)(s->mdctx1, in1, inplen);
	OSSL_FUNC(EVP_DigestUpdate)(s->mdctx2, in2, inplen);
	OSSL_FUNC(EVP_DigestUpdate)(s->mdctx3, in3, inplen);
}

static void SHA3_shake256_x4_inc_finalize(OQS_SHA3_shake256_x4_inc_ctx *state) {
	(void)state;
}

static void SHA3_shake256_x4_inc_squeeze(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3, size_t outlen, OQS_SHA3_shake256_x4_inc_ctx *state) {
	intrn_shake256_x4_inc_ctx *s = (intrn_shake256_x4_inc_ctx *)state->ctx;
#if OPENSSL_VERSION_NUMBER >= 0x30300000L
	EVP_DigestSqueeze(s->mdctx0, out0, outlen);
	EVP_DigestSqueeze(s->mdctx1, out1, outlen);
	EVP_DigestSqueeze(s->mdctx2, out2, outlen);
	EVP_DigestSqueeze(s->mdctx3, out3, outlen);
#else
	EVP_MD_CTX *clone;

	clone = OSSL_FUNC(EVP_MD_CTX_new)();
	OSSL_FUNC(EVP_DigestInit_ex)(clone, oqs_shake256(), NULL);
	if (s->n_out == 0) {
		OSSL_FUNC(EVP_MD_CTX_copy_ex)(clone, s->mdctx0);
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, out0, outlen);
		OSSL_FUNC(EVP_MD_CTX_copy_ex)(clone, s->mdctx1);
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, out1, outlen);
		OSSL_FUNC(EVP_MD_CTX_copy_ex)(clone, s->mdctx2);
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, out2, outlen);
		OSSL_FUNC(EVP_MD_CTX_copy_ex)(clone, s->mdctx3);
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, out3, outlen);
	} else {
		uint8_t *tmp = OQS_MEM_malloc(s->n_out + outlen);
		OQS_EXIT_IF_NULLPTR(tmp, "SHA3x4");
		OSSL_FUNC(EVP_MD_CTX_copy_ex)(clone, s->mdctx0);
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, tmp, s->n_out + outlen);
		memcpy(out0, tmp + s->n_out, outlen);
		OSSL_FUNC(EVP_MD_CTX_copy_ex)(clone, s->mdctx1);
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, tmp, s->n_out + outlen);
		memcpy(out1, tmp + s->n_out, outlen);
		OSSL_FUNC(EVP_MD_CTX_copy_ex)(clone, s->mdctx2);
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, tmp, s->n_out + outlen);
		memcpy(out2, tmp + s->n_out, outlen);
		OSSL_FUNC(EVP_MD_CTX_copy_ex)(clone, s->mdctx3);
		OSSL_FUNC(EVP_DigestFinalXOF)(clone, tmp, s->n_out + outlen);
		memcpy(out3, tmp + s->n_out, outlen);
		OQS_MEM_insecure_free(tmp);
	}
	OSSL_FUNC(EVP_MD_CTX_free)(clone);
	s->n_out += outlen;
#endif
}

static void SHA3_shake256_x4_inc_ctx_clone(OQS_SHA3_shake256_x4_inc_ctx *dest, const OQS_SHA3_shake256_x4_inc_ctx *src) {
	intrn_shake256_x4_inc_ctx *s = (intrn_shake256_x4_inc_ctx *)src->ctx;
	intrn_shake256_x4_inc_ctx *d = (intrn_shake256_x4_inc_ctx *)dest->ctx;
	OSSL_FUNC(EVP_MD_CTX_copy_ex)(d->mdctx0, s->mdctx0);
	OSSL_FUNC(EVP_MD_CTX_copy_ex)(d->mdctx1, s->mdctx1);
	OSSL_FUNC(EVP_MD_CTX_copy_ex)(d->mdctx2, s->mdctx2);
	OSSL_FUNC(EVP_MD_CTX_copy_ex)(d->mdctx3, s->mdctx3);
	d->n_out = s->n_out;
}

static void SHA3_shake256_x4_inc_ctx_release(OQS_SHA3_shake256_x4_inc_ctx *state) {
	intrn_shake256_x4_inc_ctx *s = (intrn_shake256_x4_inc_ctx *)state->ctx;
	OSSL_FUNC(EVP_MD_CTX_free)(s->mdctx0);
	OSSL_FUNC(EVP_MD_CTX_free)(s->mdctx1);
	OSSL_FUNC(EVP_MD_CTX_free)(s->mdctx2);
	OSSL_FUNC(EVP_MD_CTX_free)(s->mdctx3);
	OQS_MEM_insecure_free(s);
}

static void SHA3_shake256_x4_inc_ctx_reset(OQS_SHA3_shake256_x4_inc_ctx *state) {
	intrn_shake256_x4_inc_ctx *s = (intrn_shake256_x4_inc_ctx *)state->ctx;
	OSSL_FUNC(EVP_MD_CTX_reset)(s->mdctx0);
	OSSL_FUNC(EVP_MD_CTX_reset)(s->mdctx1);
	OSSL_FUNC(EVP_MD_CTX_reset)(s->mdctx2);
	OSSL_FUNC(EVP_MD_CTX_reset)(s->mdctx3);
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx0, oqs_shake256(), NULL);
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx1, oqs_shake256(), NULL);
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx2, oqs_shake256(), NULL);
	OSSL_FUNC(EVP_DigestInit_ex)(s->mdctx3, oqs_shake256(), NULL);
	s->n_out = 0;
}

extern struct OQS_SHA3_x4_callbacks sha3_x4_default_callbacks;

struct OQS_SHA3_x4_callbacks sha3_x4_default_callbacks = {
	SHA3_shake128_x4,
	SHA3_shake128_x4_inc_init,
	SHA3_shake128_x4_inc_absorb,
	SHA3_shake128_x4_inc_finalize,
	SHA3_shake128_x4_inc_squeeze,
	SHA3_shake128_x4_inc_ctx_release,
	SHA3_shake128_x4_inc_ctx_clone,
	SHA3_shake128_x4_inc_ctx_reset,
	SHA3_shake256_x4,
	SHA3_shake256_x4_inc_init,
	SHA3_shake256_x4_inc_absorb,
	SHA3_shake256_x4_inc_finalize,
	SHA3_shake256_x4_inc_squeeze,
	SHA3_shake256_x4_inc_ctx_release,
	SHA3_shake256_x4_inc_ctx_clone,
	SHA3_shake256_x4_inc_ctx_reset,
};

#endif
