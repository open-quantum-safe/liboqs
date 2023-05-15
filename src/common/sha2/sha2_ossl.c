/**
 * \file sha2_ossl.c
 * \brief Implementation of the OQS SHA2 API via calls to OpenSSL's SHA-2 functions
 *
 * SPDX-License-Identifier: MIT
 */

#include <oqs/oqs.h>

#ifdef OQS_USE_SHA2_OPENSSL

#include "sha2.h"

#include <openssl/evp.h>
#include "../ossl_helpers.h"

static void do_hash(uint8_t *output, const uint8_t *input, size_t inplen, const EVP_MD *md) {
	EVP_MD_CTX *mdctx;
	unsigned int outlen;
	mdctx = EVP_MD_CTX_new();
	OQS_EXIT_IF_NULLPTR(mdctx, "OpenSSL");
	OQS_OPENSSL_GUARD(EVP_DigestInit_ex(mdctx, md, NULL));
	OQS_OPENSSL_GUARD(EVP_DigestUpdate(mdctx, input, inplen));
	OQS_OPENSSL_GUARD(EVP_DigestFinal_ex(mdctx, output, &outlen));
	EVP_MD_CTX_free(mdctx);
}

void OQS_SHA2_sha256(uint8_t *output, const uint8_t *input, size_t inplen) {
	const EVP_MD *md;
	md = oqs_sha256();
	OQS_EXIT_IF_NULLPTR(md, "OpenSSL");
	do_hash(output, input, inplen, md);
}

void OQS_SHA2_sha384(uint8_t *output, const uint8_t *input, size_t inplen) {
	const EVP_MD *md;
	md = oqs_sha384();
	OQS_EXIT_IF_NULLPTR(md, "OpenSSL");
	do_hash(output, input, inplen, md);
}

void OQS_SHA2_sha512(uint8_t *output, const uint8_t *input, size_t inplen) {
	const EVP_MD *md;
	md = oqs_sha512();
	OQS_EXIT_IF_NULLPTR(md, "OpenSSL");
	do_hash(output, input, inplen, md);
}

#define SHA2_BLOCK_SIZE 64

void OQS_SHA2_sha256_inc_init(OQS_SHA2_sha256_ctx *state) {
	EVP_MD_CTX *mdctx;
	const EVP_MD *md = NULL;
	md = oqs_sha256();
	OQS_EXIT_IF_NULLPTR(md, "OpenSSL");
	mdctx = EVP_MD_CTX_new();
	OQS_EXIT_IF_NULLPTR(mdctx, "OpenSSL");
	OQS_OPENSSL_GUARD(EVP_DigestInit_ex(mdctx, md, NULL));
	state->ctx = mdctx;
}

void OQS_SHA2_sha256_inc_blocks(OQS_SHA2_sha256_ctx *state, const uint8_t *in, size_t inblocks) {
	OQS_OPENSSL_GUARD(EVP_DigestUpdate((EVP_MD_CTX *) state->ctx, in, inblocks * SHA2_BLOCK_SIZE));
}

void OQS_SHA2_sha256_inc_finalize(uint8_t *out, OQS_SHA2_sha256_ctx *state, const uint8_t *in, size_t inlen) {
	unsigned int md_len;
	if (inlen > 0) {
		OQS_OPENSSL_GUARD(EVP_DigestUpdate((EVP_MD_CTX *) state->ctx, in, inlen));
	}
	OQS_OPENSSL_GUARD(EVP_DigestFinal_ex((EVP_MD_CTX *) state->ctx, out, &md_len));
	EVP_MD_CTX_free((EVP_MD_CTX *) state->ctx);
	state->ctx = NULL;
}

void OQS_SHA2_sha256_inc_ctx_release(OQS_SHA2_sha256_ctx *state) {
	EVP_MD_CTX_destroy((EVP_MD_CTX *) state->ctx);
	state->ctx = NULL;
}

void OQS_SHA2_sha256_inc_ctx_clone(OQS_SHA2_sha256_ctx *dest, const OQS_SHA2_sha256_ctx *src) {
	OQS_SHA2_sha256_inc_init(dest);
	OQS_OPENSSL_GUARD(EVP_MD_CTX_copy_ex((EVP_MD_CTX *) dest->ctx, (EVP_MD_CTX *) src->ctx));
}

void OQS_SHA2_sha384_inc_init(OQS_SHA2_sha384_ctx *state) {
	EVP_MD_CTX *mdctx;
	const EVP_MD *md = NULL;
	md = oqs_sha384();
	OQS_EXIT_IF_NULLPTR(md, "OpenSSL");
	mdctx = EVP_MD_CTX_new();
	OQS_EXIT_IF_NULLPTR(mdctx, "OpenSSL");
	OQS_OPENSSL_GUARD(EVP_DigestInit_ex(mdctx, md, NULL));
	state->ctx = mdctx;
}

void OQS_SHA2_sha384_inc_blocks(OQS_SHA2_sha384_ctx *state, const uint8_t *in, size_t inblocks) {
	OQS_OPENSSL_GUARD(EVP_DigestUpdate((EVP_MD_CTX *) state->ctx, in, inblocks * 2 * SHA2_BLOCK_SIZE));
}

void OQS_SHA2_sha384_inc_finalize(uint8_t *out, OQS_SHA2_sha384_ctx *state, const uint8_t *in, size_t inlen) {
	unsigned int md_len;
	if (inlen > 0) {
		OQS_OPENSSL_GUARD(EVP_DigestUpdate((EVP_MD_CTX *) state->ctx, in, inlen));
	}
	OQS_OPENSSL_GUARD(EVP_DigestFinal_ex((EVP_MD_CTX *) state->ctx, out, &md_len));
	EVP_MD_CTX_free((EVP_MD_CTX *) state->ctx);
	state->ctx = NULL;
}

void OQS_SHA2_sha384_inc_ctx_release(OQS_SHA2_sha384_ctx *state) {
	EVP_MD_CTX_destroy((EVP_MD_CTX *) state->ctx);
	state->ctx = NULL;
}

void OQS_SHA2_sha384_inc_ctx_clone(OQS_SHA2_sha384_ctx *dest, const OQS_SHA2_sha384_ctx *src) {
	OQS_SHA2_sha384_inc_init(dest);
	OQS_OPENSSL_GUARD(EVP_MD_CTX_copy_ex((EVP_MD_CTX *) dest->ctx, (EVP_MD_CTX *) src->ctx));
}

void OQS_SHA2_sha512_inc_init(OQS_SHA2_sha512_ctx *state) {
	EVP_MD_CTX *mdctx;
	const EVP_MD *md = NULL;
	md = oqs_sha512();
	OQS_EXIT_IF_NULLPTR(md, "OpenSSL");
	mdctx = EVP_MD_CTX_new();
	OQS_EXIT_IF_NULLPTR(mdctx, "OpenSSL");
	OQS_OPENSSL_GUARD(EVP_DigestInit_ex(mdctx, md, NULL));
	state->ctx = mdctx;
}

void OQS_SHA2_sha512_inc_blocks(OQS_SHA2_sha512_ctx *state, const uint8_t *in, size_t inblocks) {
	OQS_OPENSSL_GUARD(EVP_DigestUpdate((EVP_MD_CTX *) state->ctx, in, inblocks * 2 * SHA2_BLOCK_SIZE));
}

void OQS_SHA2_sha512_inc_finalize(uint8_t *out, OQS_SHA2_sha512_ctx *state, const uint8_t *in, size_t inlen) {
	unsigned int md_len;
	if (inlen > 0) {
		OQS_OPENSSL_GUARD(EVP_DigestUpdate((EVP_MD_CTX *) state->ctx, in, inlen));
	}
	OQS_OPENSSL_GUARD(EVP_DigestFinal_ex((EVP_MD_CTX *) state->ctx, out, &md_len));
	EVP_MD_CTX_free((EVP_MD_CTX *) state->ctx);
	state->ctx = NULL;
}

void OQS_SHA2_sha512_inc_ctx_release(OQS_SHA2_sha512_ctx *state) {
	EVP_MD_CTX_destroy((EVP_MD_CTX *) state->ctx);
	state->ctx = NULL;
}

void OQS_SHA2_sha512_inc_ctx_clone(OQS_SHA2_sha512_ctx *dest, const OQS_SHA2_sha512_ctx *src) {
	OQS_SHA2_sha512_inc_init(dest);
	OQS_OPENSSL_GUARD(EVP_MD_CTX_copy_ex((EVP_MD_CTX *) dest->ctx, (EVP_MD_CTX *) src->ctx));
}

#endif
