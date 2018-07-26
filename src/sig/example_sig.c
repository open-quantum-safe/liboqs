#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

#define MESSAGE_LEN 50

/* This function gives an example of the signing operations
 * using only compile-time macros and allocating variables
 * statically on the stack, calling a specific algorithm's functions
 * directly.
 *
 * The macros OQS_SIG_qTESLA_I_length_* and the functions OQS_SIG_qTESLA_I_*
 * are only defined if the algorithm Picnic-L1-FS was enabled at compile-time
 * which must be checked using the OQS_ENABLE_SIG_qTESLA_I macro.
 *
 * <oqs/config.h>, which is included in <oqs/oqs.h>, contains macros
 * indicating which algorithms were enabled when this instance of liboqs
 * was compiled.
 */
static OQS_STATUS example_stack() {

#ifdef OQS_ENABLE_SIG_qTESLA_I
	// if Picnic-L1-FS was enabled at compile-time

	OQS_STATUS rc;
	OQS_STATUS ret = OQS_ERROR;

	uint8_t public_key[OQS_SIG_qTESLA_I_length_public_key];
	uint8_t secret_key[OQS_SIG_qTESLA_I_length_secret_key];
	uint8_t message[MESSAGE_LEN];
	uint8_t signed_message[MESSAGE_LEN + OQS_SIG_qTESLA_I_length_sig_overhead];
	uint8_t opened_message[MESSAGE_LEN + OQS_SIG_qTESLA_I_length_sig_overhead];
	size_t message_len = MESSAGE_LEN;
	size_t signed_message_len;
	size_t opened_message_len;

	OQS_randombytes(message, MESSAGE_LEN);

	rc = OQS_SIG_qTESLA_I_keypair(public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_qTESLA_I_keypair failed!\n");
		goto err;
	}
	rc = OQS_SIG_qTESLA_I_sign(signed_message, &signed_message_len, message, message_len, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_qTESLA_I_sign failed!\n");
		goto err;
	}
	rc = OQS_SIG_qTESLA_I_sign_open(opened_message, &opened_message_len, signed_message, signed_message_len, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_qTESLA_I_sign_open failed!\n");
		goto err;
	}
	printf("[example_stack] OQS_SIG_qTESLA_I operations completed.\n");
	ret = OQS_SUCCESS; // success!
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	OQS_MEM_cleanse(secret_key, OQS_SIG_qTESLA_I_length_secret_key);
	return ret;

#else
	printf("[example_stack] OQS_SIG_qTESLA_I was not enabled at compile-time.\n");
	return OQS_ERROR;

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
static OQS_STATUS example_heap() {

	OQS_SIG *sig = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *message = NULL;
	uint8_t *signed_message = NULL;
	uint8_t *opened_message = NULL;
	size_t message_len = 50;
	size_t signed_message_len;
	size_t opened_message_len;
	OQS_STATUS rc;
	OQS_STATUS ret = OQS_ERROR;

	sig = OQS_SIG_new(OQS_SIG_alg_qTESLA_I);
	if (sig == NULL) {
		printf("[example_heap]  OQS_SIG_alg_qTESLA_I was not enabled at compile-time.\n");
		return OQS_ERROR;
	}

	public_key = malloc(sig->length_public_key);
	secret_key = malloc(sig->length_secret_key);
	message = malloc(message_len);
	signed_message = malloc(message_len + sig->length_sig_overhead);
	opened_message = malloc(message_len + sig->length_sig_overhead);
	if ((public_key == NULL) || (secret_key == NULL) || (message == NULL) || (signed_message == NULL) || (opened_message == NULL)) {
		fprintf(stderr, "ERROR: malloc failed!\n");
		goto err;
	}

	OQS_randombytes(message, message_len);

	rc = OQS_SIG_keypair(sig, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_keypair failed!\n");
		goto err;
	}
	rc = OQS_SIG_sign(sig, signed_message, &signed_message_len, message, message_len, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_sign failed!\n");
		goto err;
	}
	rc = OQS_SIG_sign_open(sig, opened_message, &opened_message_len, signed_message, signed_message_len, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_sign_open failed!\n");
		goto err;
	}

	printf("[example_heap]  OQS_SIG_qTESLA_I operations completed.\n");
	ret = OQS_SUCCESS; // success
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (sig != NULL) {
		OQS_MEM_secure_free(secret_key, sig->length_secret_key);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(opened_message);
	OQS_MEM_insecure_free(signed_message);
	OQS_MEM_insecure_free(message);
	OQS_SIG_free(sig);

	return ret;
}

int main() {
	example_stack();
	example_heap();
}
