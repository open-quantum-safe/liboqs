/**
 * \file sha2_ossl.c
 * \brief Implementation of the OQS SHA2 API via calls to OpenSSL's SHA-2 functions
 */

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

#endif
