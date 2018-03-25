#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

/* This function gives an example of the operations performed by both
 * the decapsulator and the encapsulator in a single KEM session,
 * using only compile-time macros and allocating variables
 * statically on the stack, calling a specific algorithm's functions
 * directly.
 *
 * The macros OQS_KEM_frodokem_640_aes_length_* and the functions OQS_KEM_frodokem_640_aes_*
 * are only defined if the algorithm FrodoKEM-640-AES was enabled at compile-time
 * which must be checked using the OQS_ENABLE_KEM_frodokem_640_aes macro.
 *
 * <oqs/config.h>, which is included in <oqs/oqs.h>, contains macros
 * indicating which algorithms were enabled when this instance of liboqs
 * was compiled.
 */
static OQS_STATUS example_stack() {

#ifdef OQS_ENABLE_KEM_frodokem_640_aes
	// if FrodoKEM-640-AES was enabled at compile-time

	OQS_STATUS rc;
	uint8_t public_key[OQS_KEM_frodokem_640_aes_length_public_key];
	uint8_t secret_key[OQS_KEM_frodokem_640_aes_length_secret_key];
	uint8_t ciphertext[OQS_KEM_frodokem_640_aes_length_ciphertext];
	uint8_t shared_secret_e[OQS_KEM_frodokem_640_aes_length_shared_secret];
	uint8_t shared_secret_d[OQS_KEM_frodokem_640_aes_length_shared_secret];
	rc = OQS_KEM_frodokem_640_aes_keypair(public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_frodokem_640_aes_keypair failed!\n");
		return OQS_ERROR;
	}
	rc = OQS_KEM_frodokem_640_aes_encaps(ciphertext, shared_secret_e, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_frodokem_640_aes_encaps failed!\n");
		return OQS_ERROR;
	}
	rc = OQS_KEM_frodokem_640_aes_decaps(shared_secret_d, ciphertext, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_frodokem_640_aes_decaps failed!\n");
		return OQS_ERROR;
	}
	printf("[example_stack] OQS_KEM_frodokem_640_aes operations completed.\n");
	return OQS_SUCCESS; // success!

#else
	// if FrodoKEM-640-AES was not enabled at compile-time

	return OQS_ERROR;

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
static OQS_STATUS example_heap() {

	OQS_KEM *kem = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *ciphertext = NULL;
	uint8_t *shared_secret_e = NULL;
	uint8_t *shared_secret_d = NULL;
	OQS_STATUS rc;
	int ret = OQS_ERROR;

	kem = OQS_KEM_new(OQS_KEM_DEFAULT);
	if (kem == NULL) {
		// Default KEM was not enabled at compile-time
		return OQS_ERROR;
	}

	public_key = malloc(kem->length_public_key);
	secret_key = malloc(kem->length_secret_key);
	ciphertext = malloc(kem->length_ciphertext);
	shared_secret_e = malloc(kem->length_shared_secret);
	shared_secret_d = malloc(kem->length_shared_secret);
	if ((public_key == NULL) || (secret_key == NULL) || (ciphertext == NULL) || (shared_secret_e == NULL) || (shared_secret_d == NULL)) {
		fprintf(stderr, "ERROR: malloc failed!\n");
		goto err;
	}

	rc = OQS_KEM_keypair(kem, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_keypair failed!\n");
		goto err;
	}
	rc = OQS_KEM_encaps(kem, ciphertext, shared_secret_e, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_encaps failed!\n");
		goto err;
	}
	rc = OQS_KEM_decaps(kem, shared_secret_d, ciphertext, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_decaps failed!\n");
		goto err;
	}

	printf("[example_heap]  OQS_KEM_DEFAULT operations completed.\n");
	ret = OQS_SUCCESS; // success
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	free(public_key);
	free(secret_key);
	free(ciphertext);
	free(shared_secret_e);
	free(shared_secret_d);

	return ret;
}

int main() {
	example_stack();
	example_heap();
}
