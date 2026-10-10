/*
 * fuzz_test_sig.c
 *
 * Minimal fuzz test for liboqs.
 *
 * SPDX-License-Identifier: MIT
 */

#include "oqs/sig.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>


typedef struct {
	uint32_t random_seed;
	uint32_t algorithm_index;
} fuzz_init_ctx_t;

typedef struct {
	fuzz_init_ctx_t init;
	const uint8_t *data;
	size_t data_len;
} fuzz_ctx_t;

fuzz_ctx_t init_fuzz_context(const uint8_t *data, size_t data_len);
void fuzz_rand(uint8_t *random_array, size_t bytes_to_read);

void fuzz_rand(uint8_t *random_array, size_t bytes_to_read) {
	for (size_t i = 0; i < bytes_to_read; i++) {
		random_array[i] = (uint8_t)rand();
	}
}

fuzz_ctx_t init_fuzz_context(const uint8_t *data, size_t data_len) {

	fuzz_ctx_t ctx = {{0, 0}, NULL, 0};
	if (data_len > sizeof(fuzz_init_ctx_t)) {
		memcpy(&ctx.init, data, sizeof(fuzz_init_ctx_t));
		ctx.data = data + sizeof(fuzz_init_ctx_t);
		ctx.data_len = data_len - sizeof(fuzz_init_ctx_t);

		ctx.init.algorithm_index %= OQS_SIG_algs_length;
	} else {
		ctx.data = data;
		ctx.data_len = data_len;
	}

	srand(ctx.init.random_seed);
	OQS_randombytes_custom_algorithm(&fuzz_rand);

	return ctx;
}

void cleanup_heap(uint8_t *public_key, uint8_t *secret_key, uint8_t *signature,
                  OQS_SIG *sig);

static OQS_STATUS fuzz_sig(const uint8_t *data, size_t data_len) {
	OQS_SIG *sig = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *signature = NULL;
	size_t signature_len;
	OQS_STATUS rc;

	fuzz_ctx_t ctx = init_fuzz_context(data, data_len);

	const char *algorithm = OQS_SIG_alg_identifier(ctx.init.algorithm_index);

	sig = OQS_SIG_new(algorithm);
	if (sig == NULL) {
		printf("%s was not enabled at compile-time.\n", algorithm);
		return OQS_ERROR;
	}

	public_key = malloc(sig->length_public_key);
	secret_key = malloc(sig->length_secret_key);
	signature = malloc(sig->length_signature);
	if ((public_key == NULL) || (secret_key == NULL) || (ctx.data == NULL) ||
	        (signature == NULL)) {
		fprintf(stderr, "ERROR: malloc failed!\n");
		cleanup_heap(public_key, secret_key, signature, sig);
		return OQS_ERROR;
	}

	rc = OQS_SIG_keypair(sig, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_keypair failed!\n");
		cleanup_heap(public_key, secret_key, signature, sig);
		return OQS_ERROR;
	}
	rc = OQS_SIG_sign(sig, signature, &signature_len, ctx.data, ctx.data_len,
	                  secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_sign failed!\n");
		cleanup_heap(public_key, secret_key, signature, sig);
		return OQS_ERROR;
	}
	rc = OQS_SIG_verify(sig, ctx.data, ctx.data_len, signature, signature_len,
	                    public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_verify failed!\n");
		cleanup_heap(public_key, secret_key, signature, sig);
		exit(1);
	}

	cleanup_heap(public_key, secret_key, signature, sig);
	return OQS_SUCCESS; // success
}

void cleanup_heap(uint8_t *public_key, uint8_t *secret_key, uint8_t *signature,
                  OQS_SIG *sig) {
	if (sig != NULL) {
		OQS_MEM_secure_free(secret_key, sig->length_secret_key);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(signature);
	OQS_SIG_free(sig);
}

int LLVMFuzzerTestOneInput(const char *data, size_t size) {
	OQS_init();
	if (OQS_ERROR == fuzz_sig((const uint8_t *)data, size)) {
		// If we get an error prune testcase from corpus.
		return -1;
	}
	OQS_destroy();
	return 0;
}
