/**
 * \file sha3_ossl.c
 * \brief Implementation of the OQS SHA3 API via calls to OpenSSL's SHA-3 functions
 */

#include <oqs/oqs.h>

#ifdef USE_SHA3_OPENSSL

#include "sha3.h"

#include <openssl/evp.h>

static void inline do_hash(uint8_t *output, const uint8_t *input, size_t inplen, const EVP_MD *md) {
	EVP_MD_CTX *mdctx;
	unsigned int outlen;
	mdctx = EVP_MD_CTX_create();
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, input, inplen);
	EVP_DigestFinal_ex(mdctx, output, &outlen);
	EVP_MD_CTX_destroy(mdctx);
}

void OQS_SHA3_sha3_256(uint8_t *output, const uint8_t *input, size_t inplen) {
	const EVP_MD *md;
	md = EVP_sha3_256();
	do_hash(output, input, inplen, md);
}

#endif
