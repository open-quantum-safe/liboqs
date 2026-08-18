/*
 * example_sig.c
 *
 * Minimal example of using a post-quantum signature implemented in liboqs.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

#define MESSAGE_LEN 50

/* Cleaning up memory etc */
void cleanup_stack(uint8_t *secret_key, size_t secret_key_len);

void cleanup_heap(uint8_t *public_key, uint8_t *secret_key,
                  uint8_t *message, uint8_t *signature,
                  OQS_SIG *sig);

/* This function gives an example of the signing operations
 * using only compile-time macros and allocating variables
 * statically on the stack, calling a specific algorithm's functions
 * directly.
 *
 * The macros OQS_SIG_ml_dsa_65_length_* and the functions OQS_SIG_ml_dsa_65_*
 * are only defined if the algorithm ml_dsa_65 was enabled at compile-time
 * which must be checked using the OQS_ENABLE_SIG_ml_dsa_65 macro.
 *
 * <oqs/oqsconfig.h>, which is included in <oqs/oqs.h>, contains macros
 * indicating which algorithms were enabled when this instance of liboqs
 * was compiled.
 */
static OQS_STATUS example_stack(void) {

#ifdef OQS_ENABLE_SIG_ml_dsa_65

	OQS_STATUS rc;

	uint8_t public_key[OQS_SIG_ml_dsa_65_length_public_key];
	uint8_t secret_key[OQS_SIG_ml_dsa_65_length_secret_key];
	uint8_t message[MESSAGE_LEN];
	uint8_t signature[OQS_SIG_ml_dsa_65_length_signature];
	size_t message_len = MESSAGE_LEN;
	size_t signature_len;

	// let's create a random test message to sign
	OQS_randombytes(message, message_len);

	rc = OQS_SIG_ml_dsa_65_keypair(public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_ml_dsa_65_keypair failed!\n");
		cleanup_stack(secret_key, OQS_SIG_ml_dsa_65_length_secret_key);
		return OQS_ERROR;
	}
	rc = OQS_SIG_ml_dsa_65_sign(signature, &signature_len, message, message_len, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_ml_dsa_65_sign failed!\n");
		cleanup_stack(secret_key, OQS_SIG_ml_dsa_65_length_secret_key);
		return OQS_ERROR;
	}
	rc = OQS_SIG_ml_dsa_65_verify(message, message_len, signature, signature_len, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_ml_dsa_65_verify failed!\n");
		cleanup_stack(secret_key, OQS_SIG_ml_dsa_65_length_secret_key);
		return OQS_ERROR;
	}

	printf("[example_stack] OQS_SIG_ml_dsa_65 operations completed.\n");
	cleanup_stack(secret_key, OQS_SIG_ml_dsa_65_length_secret_key);
	return OQS_SUCCESS; // success!

#else

	printf("[example_stack] OQS_SIG_ml_dsa_65 was not enabled at compile-time.\n");
	return OQS_SUCCESS;

#endif
}

/* This function gives an example of the signing operations,
 * allocating variables dynamically on the heap and calling the generic
 * OQS_SIG object.
 *
 * This does not require the use of compile-time macros to check if the
 * algorithm in question was enabled at compile-time; instead, the caller
 * must check that the OQS_SIG object returned is not NULL.
 */
static OQS_STATUS example_heap(void) {

#ifdef OQS_ENABLE_SIG_ml_dsa_65

	OQS_SIG *sig = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *message = NULL;
	uint8_t *signature = NULL;
	size_t message_len = MESSAGE_LEN;
	size_t signature_len;
	OQS_STATUS rc;

	sig = OQS_SIG_new(OQS_SIG_alg_ml_dsa_65);
	if (sig == NULL) {
		printf("[example_heap]  OQS_SIG_alg_ml_dsa_65 was not enabled at compile-time.\n");
		return OQS_ERROR;
	}

	public_key = OQS_MEM_malloc(sig->length_public_key);
	secret_key = OQS_MEM_malloc(sig->length_secret_key);
	message = OQS_MEM_malloc(message_len);
	signature = OQS_MEM_malloc(sig->length_signature);
	if ((public_key == NULL) || (secret_key == NULL) || (message == NULL) || (signature == NULL)) {
		fprintf(stderr, "ERROR: OQS_MEM_malloc failed!\n");
		cleanup_heap(public_key, secret_key, message, signature, sig);
		return OQS_ERROR;
	}

	// let's create a random test message to sign
	OQS_randombytes(message, message_len);

	rc = OQS_SIG_keypair(sig, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_keypair failed!\n");
		cleanup_heap(public_key, secret_key, message, signature, sig);
		return OQS_ERROR;
	}
	rc = OQS_SIG_sign(sig, signature, &signature_len, message, message_len, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_sign failed!\n");
		cleanup_heap(public_key, secret_key, message, signature, sig);
		return OQS_ERROR;
	}
	rc = OQS_SIG_verify(sig, message, message_len, signature, signature_len, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_verify failed!\n");
		cleanup_heap(public_key, secret_key, message, signature, sig);
		return OQS_ERROR;
	}

	printf("[example_heap]  OQS_SIG_ml_dsa_65 operations completed.\n");
	cleanup_heap(public_key, secret_key, message, signature, sig);
	return OQS_SUCCESS; // success
#else

	printf("[example_heap] OQS_SIG_ml_dsa_65 was not enabled at compile-time.\n");
	return OQS_SUCCESS;

#endif
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

void cleanup_stack(uint8_t *secret_key, size_t secret_key_len) {
	OQS_MEM_cleanse(secret_key, secret_key_len);
}

void cleanup_heap(uint8_t *public_key, uint8_t *secret_key,
                  uint8_t *message, uint8_t *signature,
                  OQS_SIG *sig) {
	if (sig != NULL) {
		OQS_MEM_secure_free(secret_key, sig->length_secret_key);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(message);
	OQS_MEM_insecure_free(signature);
	OQS_SIG_free(sig);
}
