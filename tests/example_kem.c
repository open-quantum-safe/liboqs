/*
 * example_kem.c
 *
 * Minimal example of a Diffie-Hellman-style post-quantum key encapsulation
 * implemented in liboqs.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

/* Cleaning up memory etc */
void cleanup_stack(uint8_t *secret_key, size_t secret_key_len,
                   uint8_t *shared_secret_e, uint8_t *shared_secret_d,
                   size_t shared_secret_len);

void cleanup_heap(uint8_t *secret_key, uint8_t *shared_secret_e,
                  uint8_t *shared_secret_d, uint8_t *public_key,
                  uint8_t *ciphertext, OQS_KEM *kem);

/* This function gives an example of the operations performed by both
 * the decapsulator and the encapsulator in a single KEM session,
 * using only compile-time macros and allocating variables
 * statically on the stack, calling a specific algorithm's functions
 * directly.
 *
 * The macros OQS_KEM_ml_kem_768_length_* and the functions
 * OQS_KEM_ml_kem_768_* are only defined if the algorithm
 * ML-KEM-768 was enabled at compile-time which must be
 * checked using the OQS_ENABLE_KEM_ml_kem_768 macro.
 *
 * <oqs/oqsconfig.h>, which is included in <oqs/oqs.h>, contains macros
 * indicating which algorithms were enabled when this instance of liboqs
 * was compiled.
 */
static OQS_STATUS example_stack(void) {
#ifndef OQS_ENABLE_KEM_ml_kem_768 // if ML-KEM-768 was not enabled at compile-time
	printf("[example_stack] OQS_KEM_ml_kem_768 was not enabled at "
	       "compile-time.\n");
	return OQS_SUCCESS; // nothing done successfully ;-)
#else
	uint8_t public_key[OQS_KEM_ml_kem_768_length_public_key];
	uint8_t secret_key[OQS_KEM_ml_kem_768_length_secret_key];
	uint8_t ciphertext[OQS_KEM_ml_kem_768_length_ciphertext];
	uint8_t shared_secret_e[OQS_KEM_ml_kem_768_length_shared_secret];
	uint8_t shared_secret_d[OQS_KEM_ml_kem_768_length_shared_secret];

	OQS_STATUS rc = OQS_KEM_ml_kem_768_keypair(public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_ml_kem_768_keypair failed!\n");
		cleanup_stack(secret_key, OQS_KEM_ml_kem_768_length_secret_key,
		              shared_secret_e, shared_secret_d,
		              OQS_KEM_ml_kem_768_length_shared_secret);

		return OQS_ERROR;
	}
	rc = OQS_KEM_ml_kem_768_encaps(ciphertext, shared_secret_e, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_ml_kem_768_encaps failed!\n");
		cleanup_stack(secret_key, OQS_KEM_ml_kem_768_length_secret_key,
		              shared_secret_e, shared_secret_d,
		              OQS_KEM_ml_kem_768_length_shared_secret);

		return OQS_ERROR;
	}
	rc = OQS_KEM_ml_kem_768_decaps(shared_secret_d, ciphertext, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_ml_kem_768_decaps failed!\n");
		cleanup_stack(secret_key, OQS_KEM_ml_kem_768_length_secret_key,
		              shared_secret_e, shared_secret_d,
		              OQS_KEM_ml_kem_768_length_shared_secret);

		return OQS_ERROR;
	}
	printf("[example_stack] OQS_KEM_ml_kem_768 operations completed.\n");

	return OQS_SUCCESS; // success!
#endif
}

/* This function gives an example of the operations performed by both
 * the decapsulator and the encapsulator in a single KEM session,
 * allocating variables dynamically on the heap and calling the generic
 * OQS_KEM object.
 *
 * This does not require the use of compile-time macros to check if the
 * algorithm in question was enabled at compile-time; instead, the caller
 * must check that the OQS_KEM object returned is not NULL.
 */
static OQS_STATUS example_heap(void) {
	OQS_KEM *kem = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *ciphertext = NULL;
	uint8_t *shared_secret_e = NULL;
	uint8_t *shared_secret_d = NULL;

	kem = OQS_KEM_new(OQS_KEM_alg_ml_kem_768);
	if (kem == NULL) {
		printf("[example_heap]  OQS_KEM_ml_kem_768 was not enabled at "
		       "compile-time.\n");
		return OQS_SUCCESS;
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

	OQS_STATUS rc = OQS_KEM_keypair(kem, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_keypair failed!\n");
		cleanup_heap(secret_key, shared_secret_e, shared_secret_d, public_key,
		             ciphertext, kem);

		return OQS_ERROR;
	}
	rc = OQS_KEM_encaps(kem, ciphertext, shared_secret_e, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_encaps failed!\n");
		cleanup_heap(secret_key, shared_secret_e, shared_secret_d, public_key,
		             ciphertext, kem);

		return OQS_ERROR;
	}
	rc = OQS_KEM_decaps(kem, shared_secret_d, ciphertext, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_decaps failed!\n");
		cleanup_heap(secret_key, shared_secret_e, shared_secret_d, public_key,
		             ciphertext, kem);

		return OQS_ERROR;
	}

	printf("[example_heap]  OQS_KEM_ml_kem_768 operations completed.\n");
	cleanup_heap(secret_key, shared_secret_e, shared_secret_d, public_key,
	             ciphertext, kem);

	return OQS_SUCCESS; // success
}

int main(void) {
	OQS_init();
	if (example_stack() == OQS_SUCCESS && example_heap() == OQS_SUCCESS) {
		OQS_destroy();
		return EXIT_SUCCESS;
	} else {
		OQS_destroy();
		return EXIT_FAILURE;
	}
}

void cleanup_stack(uint8_t *secret_key, size_t secret_key_len,
                   uint8_t *shared_secret_e, uint8_t *shared_secret_d,
                   size_t shared_secret_len) {
	OQS_MEM_cleanse(secret_key, secret_key_len);
	OQS_MEM_cleanse(shared_secret_e, shared_secret_len);
	OQS_MEM_cleanse(shared_secret_d, shared_secret_len);
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
