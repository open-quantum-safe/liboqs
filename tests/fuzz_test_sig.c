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
#include <stdlib.h>

#include <oqs/oqs.h>

void cleanup_heap(uint8_t *public_key, uint8_t *secret_key,
                  uint8_t *signature,
                  OQS_SIG *sig);

static OQS_STATUS fuzz_sig(const uint8_t *data, size_t data_len) {
	OQS_SIG *sig = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *signature = NULL;
	size_t signature_len;
	OQS_STATUS rc;

	// Select algorithm based on fuzzed data.
	size_t algorithm_index = 0;
	const uint8_t *message = NULL;
	size_t message_len = 0;
	if (data_len > sizeof(size_t)) {
		memcpy(&algorithm_index, data, sizeof(size_t));
		message = data + sizeof(size_t);
		message_len = data_len - sizeof(size_t);

		algorithm_index %= OQS_SIG_algs_length;
	} else {
		message = data;
		message_len = data_len;
	}
	const char *algorithm = OQS_SIG_alg_identifier(algorithm_index);

	sig = OQS_SIG_new(algorithm);
	if (sig == NULL) {
		printf("%s was not enabled at compile-time.\n", algorithm);
		return OQS_ERROR;
	}

	public_key = malloc(sig->length_public_key);
	secret_key = malloc(sig->length_secret_key);
	signature = malloc(sig->length_signature);
	if ((public_key == NULL) || (secret_key == NULL) || (message == NULL) || (signature == NULL)) {
		fprintf(stderr, "ERROR: malloc failed!\n");
		cleanup_heap(public_key, secret_key,  signature, sig);
		return OQS_ERROR;
	}

	rc = OQS_SIG_keypair(sig, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_keypair failed!\n");
		cleanup_heap(public_key, secret_key,  signature, sig);
		return OQS_ERROR;
	}
	rc = OQS_SIG_sign(sig, signature, &signature_len, message, message_len, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_sign failed!\n");
		cleanup_heap(public_key, secret_key,  signature, sig);
		return OQS_ERROR;
	}
	rc = OQS_SIG_verify(sig, message, message_len, signature, signature_len, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_verify failed!\n");
		cleanup_heap(public_key, secret_key,  signature, sig);
		exit(1);
	}

	cleanup_heap(public_key, secret_key,  signature, sig);
	return OQS_SUCCESS; // success
}

void cleanup_heap(uint8_t *public_key, uint8_t *secret_key,
                  uint8_t *signature,
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


