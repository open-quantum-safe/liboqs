/*
 * fuzz_test_kem.c
 *
 * Minimal fuzz test for liboqs.
 *
 * SPDX-License-Identifier: MIT
 */

#include "oqs/kem.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <oqs/oqs.h>

size_t min(size_t a, size_t b) {
	return a < b ? a : b;
}

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

		ctx.init.algorithm_index %= OQS_KEM_algs_length;
	} else {
		ctx.data = data;
		ctx.data_len = data_len;
	}

	srand(ctx.init.random_seed);
	OQS_randombytes_custom_algorithm(&fuzz_rand);

	return ctx;
}

void cleanup_heap(uint8_t *secret_key, uint8_t *shared_secret_e,
                  uint8_t *shared_secret_d, uint8_t *public_key,
                  uint8_t *ciphertext, OQS_KEM *kem);


/** Fuzzing of the KEM */
static OQS_STATUS fuzz_kem(const uint8_t *data, size_t data_len) {
	OQS_KEM *kem = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *ciphertext = NULL;
	uint8_t *shared_secret_e = NULL;
	uint8_t *shared_secret_d = NULL;

	fuzz_ctx_t ctx = init_fuzz_context(data, data_len);
	const char *algorithm = OQS_KEM_alg_identifier(ctx.init.algorithm_index);
	kem = OQS_KEM_new(algorithm);
	if (kem == NULL) {
		printf("%s was not enabled at compile-time.\n", algorithm);
		return OQS_ERROR;
	}

	public_key = OQS_MEM_malloc(kem->length_public_key);
	secret_key = OQS_MEM_malloc(kem->length_secret_key);
	ciphertext = OQS_MEM_malloc(kem->length_ciphertext);
	shared_secret_e = OQS_MEM_malloc(kem->length_shared_secret);
	shared_secret_d = OQS_MEM_malloc(kem->length_shared_secret);

	if ((public_key == NULL) || (secret_key == NULL) || (ciphertext == NULL) ||
	        (shared_secret_e == NULL) || (shared_secret_d == NULL)) {
		fprintf(stderr, "ERROR: OQS_MEM_malloc failed!\n");
		cleanup_heap(secret_key, shared_secret_e, shared_secret_d, public_key,
		             ciphertext, kem);

		return OQS_ERROR;
	}

	memcpy(shared_secret_e, ctx.data, min(ctx.data_len, kem->length_shared_secret));
	OQS_STATUS rc = OQS_KEM_keypair(kem, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_keypair failed!\n");
		cleanup_heap(secret_key, shared_secret_e, shared_secret_d, public_key,
		             ciphertext, kem);

		return rc;
	}
	rc = OQS_KEM_encaps(kem, ciphertext, shared_secret_e, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_encaps failed!\n");
		cleanup_heap(secret_key, shared_secret_e, shared_secret_d, public_key,
		             ciphertext, kem);

		return rc;
	}
	rc = OQS_KEM_decaps(kem, shared_secret_d, ciphertext, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_decaps failed! with algorithm index %u\n", ctx.init.algorithm_index);
		cleanup_heap(secret_key, shared_secret_e, shared_secret_d, public_key,
		             ciphertext, kem);

		return rc;
	}
	assert(memcmp(shared_secret_d, shared_secret_e, kem->length_shared_secret) == 0);

	cleanup_heap(secret_key, shared_secret_e, shared_secret_d, public_key,
	             ciphertext, kem);

	return OQS_SUCCESS; // success
}

void cleanup_heap(uint8_t *secret_key, uint8_t *shared_secret_e,
                  uint8_t *shared_secret_d, uint8_t *public_key,
                  uint8_t *ciphertext, OQS_KEM *kem) {
	if (kem != NULL) {
		OQS_MEM_secure_free(secret_key, kem->length_secret_key);
		OQS_MEM_secure_free(shared_secret_e, kem->length_shared_secret);
		OQS_MEM_secure_free(shared_secret_d, kem->length_shared_secret);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(ciphertext);
	OQS_KEM_free(kem);
}

int LLVMFuzzerTestOneInput(const char *data, size_t size) {
	OQS_init();
	if (OQS_ERROR == fuzz_kem((const uint8_t *)data, size)) {
		// If we get an error prune testcase from corpus.
		return -1;
	}
	OQS_destroy();
	return 0;
}
