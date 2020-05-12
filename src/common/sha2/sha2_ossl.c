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

static void do_hash(uint8_t *output, const uint8_t *input, size_t inplen, const EVP_MD *md) {
	EVP_MD_CTX *mdctx;
	unsigned int outlen;
	mdctx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, input, inplen);
	EVP_DigestFinal_ex(mdctx, output, &outlen);
	EVP_MD_CTX_free(mdctx);
}

void OQS_SHA2_sha256(uint8_t *output, const uint8_t *input, size_t inplen) {
	const EVP_MD *md;
	md = EVP_sha256();
	do_hash(output, input, inplen, md);
}

void OQS_SHA2_sha384(uint8_t *output, const uint8_t *input, size_t inplen) {
	const EVP_MD *md;
	md = EVP_sha384();
	do_hash(output, input, inplen, md);
}

void OQS_SHA2_sha512(uint8_t *output, const uint8_t *input, size_t inplen) {
	const EVP_MD *md;
	md = EVP_sha512();
	do_hash(output, input, inplen, md);
}

#define SHA2_BLOCK_SIZE 64

void OQS_SHA2_sha256_inc_init(OQS_SHA2_sha256_ctx *state) {
	EVP_MD_CTX *mdctx;
	const EVP_MD *md = NULL;
	md = EVP_sha256();
	OQS_EXIT_IF_NULLPTR(md);
	mdctx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(mdctx, md, NULL);
	state->ctx = mdctx;
}

void OQS_SHA2_sha256_inc_blocks(OQS_SHA2_sha256_ctx *state, const uint8_t *in, size_t inblocks) {
	EVP_DigestUpdate((EVP_MD_CTX *) state->ctx, in, inblocks * SHA2_BLOCK_SIZE);
}

void OQS_SHA2_sha256_inc_finalize(uint8_t *out, OQS_SHA2_sha256_ctx *state, const uint8_t *in, size_t inlen) {
	unsigned int md_len;
	if (inlen > 0) {
		EVP_DigestUpdate((EVP_MD_CTX *) state->ctx, in, inlen);
	}
	EVP_DigestFinal_ex((EVP_MD_CTX *) state->ctx, out, &md_len);
	EVP_MD_CTX_free((EVP_MD_CTX *) state->ctx);
}

void OQS_SHA2_sha256_inc_ctx_release(OQS_SHA2_sha256_ctx *state) {
	EVP_MD_CTX_destroy((EVP_MD_CTX *) state->ctx);
}

void OQS_SHA2_sha256_inc_ctx_clone(OQS_SHA2_sha256_ctx *dest, const OQS_SHA2_sha256_ctx *src) {
	OQS_SHA2_sha256_inc_init(dest);
	EVP_MD_CTX_copy_ex((EVP_MD_CTX *) dest->ctx, (EVP_MD_CTX *) src->ctx);
}

void OQS_SHA2_sha384_inc_init(OQS_SHA2_sha384_ctx *state) {
	EVP_MD_CTX *mdctx;
	const EVP_MD *md = NULL;
	md = EVP_sha384();
	OQS_EXIT_IF_NULLPTR(md);
	mdctx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(mdctx, md, NULL);
	state->ctx = mdctx;
}

void OQS_SHA2_sha384_inc_blocks(OQS_SHA2_sha384_ctx *state, const uint8_t *in, size_t inblocks) {
	EVP_DigestUpdate((EVP_MD_CTX *) state->ctx, in, inblocks * SHA2_BLOCK_SIZE);
}

void OQS_SHA2_sha384_inc_finalize(uint8_t *out, OQS_SHA2_sha384_ctx *state, const uint8_t *in, size_t inlen) {
	unsigned int md_len;
	if (inlen > 0) {
		EVP_DigestUpdate((EVP_MD_CTX *) state->ctx, in, inlen);
	}
	EVP_DigestFinal_ex((EVP_MD_CTX *) state->ctx, out, &md_len);
	EVP_MD_CTX_free((EVP_MD_CTX *) state->ctx);
}

void OQS_SHA2_sha384_inc_ctx_release(OQS_SHA2_sha384_ctx *state) {
	EVP_MD_CTX_destroy((EVP_MD_CTX *) state->ctx);
}

void OQS_SHA2_sha384_inc_ctx_clone(OQS_SHA2_sha384_ctx *dest, const OQS_SHA2_sha384_ctx *src) {
	OQS_SHA2_sha384_inc_init(dest);
	EVP_MD_CTX_copy_ex((EVP_MD_CTX *) dest->ctx, (EVP_MD_CTX *) src->ctx);
}

void OQS_SHA2_sha512_inc_init(OQS_SHA2_sha512_ctx *state) {
	EVP_MD_CTX *mdctx;
	const EVP_MD *md = NULL;
	md = EVP_sha512();
	OQS_EXIT_IF_NULLPTR(md);
	mdctx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(mdctx, md, NULL);
	state->ctx = mdctx;
}

void OQS_SHA2_sha512_inc_blocks(OQS_SHA2_sha512_ctx *state, const uint8_t *in, size_t inblocks) {
	EVP_DigestUpdate((EVP_MD_CTX *) state->ctx, in, inblocks * SHA2_BLOCK_SIZE);
}

void OQS_SHA2_sha512_inc_finalize(uint8_t *out, OQS_SHA2_sha512_ctx *state, const uint8_t *in, size_t inlen) {
	unsigned int md_len;
	if (inlen > 0) {
		EVP_DigestUpdate((EVP_MD_CTX *) state->ctx, in, inlen);
	}
	EVP_DigestFinal_ex((EVP_MD_CTX *) state->ctx, out, &md_len);
	EVP_MD_CTX_free((EVP_MD_CTX *) state->ctx);
}

void OQS_SHA2_sha512_inc_ctx_release(OQS_SHA2_sha512_ctx *state) {
	EVP_MD_CTX_destroy((EVP_MD_CTX *) state->ctx);
}

void OQS_SHA2_sha512_inc_ctx_clone(OQS_SHA2_sha512_ctx *dest, const OQS_SHA2_sha512_ctx *src) {
	OQS_SHA2_sha512_inc_init(dest);
	EVP_MD_CTX_copy_ex((EVP_MD_CTX *) dest->ctx, (EVP_MD_CTX *) src->ctx);
}

#endif
