/**
 * \file sha3_ossl.c
 * \brief Implementation of the OQS SHA3 API via calls to OpenSSL's SHA-3 functions
 *
 * SPDX-License-Identifier: MIT
 */

#include <oqs/oqs.h>

#ifdef OQS_USE_SHA3_OPENSSL

#include "sha3.h"

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

static void do_xof(uint8_t *output, size_t outlen, const uint8_t *input, size_t inplen, const EVP_MD *md) {
	EVP_MD_CTX *mdctx;
	mdctx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, input, inplen);
	EVP_DigestFinalXOF(mdctx, output, outlen);
	EVP_MD_CTX_free(mdctx);
}

void OQS_SHA3_sha3_256(uint8_t *output, const uint8_t *input, size_t inplen) {
	const EVP_MD *md;
	md = EVP_sha3_256();
	do_hash(output, input, inplen, md);
}

void OQS_SHA3_sha3_384(uint8_t *output, const uint8_t *input, size_t inplen) {
	const EVP_MD *md;
	md = EVP_sha3_384();
	do_hash(output, input, inplen, md);
}

void OQS_SHA3_sha3_512(uint8_t *output, const uint8_t *input, size_t inplen) {
	const EVP_MD *md;
	md = EVP_sha3_512();
	do_hash(output, input, inplen, md);
}

void OQS_SHA3_shake128(uint8_t *output, size_t outlen, const uint8_t *input, size_t inplen) {
	const EVP_MD *md;
	md = EVP_shake128();
	do_xof(output, outlen, input, inplen, md);
}

void OQS_SHA3_shake256(uint8_t *output, size_t outlen, const uint8_t *input, size_t inplen) {
	const EVP_MD *md;
	md = EVP_shake256();
	do_xof(output, outlen, input, inplen, md);
}

#endif
