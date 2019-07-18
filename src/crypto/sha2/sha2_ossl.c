/**
 * \file sha2_ossl.c
 * \brief Implementation of the OQS SHA2 API via calls to OpenSSL's SHA-2 functions
 */

#include <assert.h>

#include <oqs/oqs.h>

#ifdef USE_SHA2_OPENSSL

#include "sha2.h"

#include <openssl/evp.h>

static void do_hash(uint8_t *output, const uint8_t *input, size_t inplen, const EVP_MD *md) {
	EVP_MD_CTX *mdctx;
	unsigned int outlen;
	mdctx = EVP_MD_CTX_create();
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, input, inplen);
	EVP_DigestFinal_ex(mdctx, output, &outlen);
	EVP_MD_CTX_destroy(mdctx);
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

void OQS_SHA2_sha256_inc_init(void **state) {
	EVP_MD_CTX *mdctx;
	const EVP_MD *md = NULL;
	md = EVP_sha256();
	assert(md != NULL);
	mdctx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(mdctx, md, NULL);
	*state = mdctx;
}

#define SHA2_BLOCK_SIZE 64

void OQS_SHA2_sha256_inc_blocks(void *state, const uint8_t *in, size_t inblocks) {
	EVP_DigestUpdate((EVP_MD_CTX *) state, in, inblocks * SHA2_BLOCK_SIZE);
 
}
void OQS_SHA2_sha256_inc_finalize(uint8_t *out, void *state, const uint8_t *in, size_t inlen) {
	unsigned int md_len;
	if (inlen > 0) {
		EVP_DigestUpdate((EVP_MD_CTX *) state, in, inlen);
	}
	EVP_DigestFinal_ex((EVP_MD_CTX *) state, out, &md_len);
	EVP_MD_CTX_free((EVP_MD_CTX *) state);
}

void OQS_SHA2_sha256_inc_dupe_state(void **stateout, const void *statein) {
	OQS_SHA2_sha256_inc_init(stateout);
	EVP_MD_CTX_copy_ex(*stateout, statein);
}

#endif
