#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

static OQS_STATUS sig_test_correctness(const char *method_name) {

	OQS_SIG *sig = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *message = NULL;
	uint8_t *signed_message = NULL;
	size_t message_len = 50;
	size_t signed_message_len;
	OQS_STATUS rc, ret = OQS_ERROR;

	sig = OQS_SIG_new(method_name);
	if (sig == NULL) {
		return OQS_SUCCESS;
	}

	printf("================================================================================\n");
	printf("Sample computation for SIG %s\n", sig->method_name);
	printf("================================================================================\n");

	public_key = malloc(sig->length_public_key);
	secret_key = malloc(sig->length_secret_key);
	message = malloc(message_len + sig->length_sig_overhead);
	signed_message = malloc(message_len + sig->length_sig_overhead);

	if ((public_key == NULL) || (secret_key == NULL) || (message == NULL) || (signed_message == NULL)) {
		fprintf(stderr, "ERROR: malloc failed!\n");
		goto err;
	}

	OQS_randombytes(message, message_len);

	rc = OQS_SIG_keypair(sig, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_keypair failed\n");
		goto err;
	}

	rc = OQS_SIG_sign(sig, signed_message, &signed_message_len, message, message_len, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_sign failed!\n");
		goto err;
	}

	rc = OQS_SIG_sign_open(sig, message, &message_len, signed_message, signed_message_len, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_sign_open failed!\n");
		goto err;
	}

	/* modify the signed message to invalidate the signature */
	signed_message[0]++;
	rc = OQS_SIG_sign_open(sig, message, &message_len, signed_message, signed_message_len, public_key);
	if (rc != OQS_ERROR) {
		fprintf(stderr, "ERROR: OQS_SIG_sign_open should have failed!\n");
		goto err;
	}

	ret = OQS_SUCCESS;
	printf("verification passes as expected\n");
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (sig != NULL) {
		OQS_MEM_secure_free(secret_key, sig->length_secret_key);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(signed_message);
	OQS_MEM_insecure_free(message);
	OQS_SIG_free(sig);

	return ret;
}

int main() {

	int ret = EXIT_SUCCESS;
	OQS_STATUS rc;

	// Use system RNG in this program
	OQS_randombytes_switch_algorithm(OQS_RAND_alg_system);

	for (size_t i = 0; i < OQS_SIG_algs_length; i++) {
		rc = sig_test_correctness(OQS_SIG_alg_identifier(i));
		if (rc != OQS_SUCCESS) {
			ret = EXIT_FAILURE;
		}
	}

	return ret;
}
