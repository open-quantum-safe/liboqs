// SPDX-License-Identifier: MIT

#if defined(_WIN32)
#pragma warning(disable : 4244 4293)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

#if OQS_USE_PTHREADS_IN_TESTS
#include <pthread.h>
#endif

#include "system_info.c"

static OQS_STATUS sig_test_correctness(const char *method_name) {

	OQS_SIG *sig = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *message = NULL;
	size_t message_len = 100;
	uint8_t *signature = NULL;
	size_t signature_len;
	OQS_STATUS rc, ret = OQS_ERROR;

	sig = OQS_SIG_new(method_name);
	if (sig == NULL) {
		fprintf(stderr, "ERROR: OQS_SIG_new failed\n");
		goto err;
	}

	printf("================================================================================\n");
	printf("Sample computation for signature %s\n", sig->method_name);
	printf("================================================================================\n");

	public_key = malloc(sig->length_public_key);
	secret_key = malloc(sig->length_secret_key);
	message = malloc(message_len);
	signature = malloc(sig->length_signature);

	if ((public_key == NULL) || (secret_key == NULL) || (message == NULL) || (signature == NULL)) {
		fprintf(stderr, "ERROR: malloc failed\n");
		goto err;
	}

	OQS_randombytes(message, message_len);

	rc = OQS_SIG_keypair(sig, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_keypair failed\n");
		goto err;
	}

	rc = OQS_SIG_sign(sig, signature, &signature_len, message, message_len, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_sign failed\n");
		goto err;
	}

	rc = OQS_SIG_verify(sig, message, message_len, signature, signature_len, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_verify failed\n");
		goto err;
	}

	/* modify the signature to invalidate it */
	OQS_randombytes(signature, signature_len);
	rc = OQS_SIG_verify(sig, message, message_len, signature, signature_len, public_key);
	if (rc != OQS_ERROR) {
		fprintf(stderr, "ERROR: OQS_SIG_verify should have failed!\n");
		goto err;
	}
	printf("verification passes as expected\n");
	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (sig != NULL) {
		OQS_MEM_secure_free(secret_key, sig->length_secret_key);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(message);
	OQS_MEM_insecure_free(signature);
	OQS_SIG_free(sig);

	return ret;
}

#if OQS_USE_PTHREADS_IN_TESTS
struct thread_data {
	char *alg_name;
	OQS_STATUS rc;
};

void *test_wrapper(void *arg) {
	struct thread_data *td = arg;
	td->rc = sig_test_correctness(td->alg_name);
	return NULL;
}
#endif

int main(int argc, char **argv) {

	if (argc != 2) {
		fprintf(stderr, "Usage: test_sig algname\n");
		fprintf(stderr, "  algname: ");
		for (size_t i = 0; i < OQS_SIG_algs_length; i++) {
			if (i > 0) {
				fprintf(stderr, ", ");
			}
			fprintf(stderr, "%s", OQS_SIG_alg_identifier(i));
		}
		fprintf(stderr, "\n");
		return EXIT_FAILURE;
	}

	print_system_info();

	char *alg_name = argv[1];
	if (!OQS_SIG_alg_is_enabled(alg_name)) {
		printf("Signature algorithm %s not enabled!\n", alg_name);
		return EXIT_FAILURE;
	}

	// Use system RNG in this program
	OQS_randombytes_switch_algorithm(OQS_RAND_alg_system);

	OQS_STATUS rc;
#if OQS_USE_PTHREADS_IN_TESTS
#define MAX_LEN_SIG_NAME_ 64
	// don't run Rainbow IIIc and Vc in threads because of large stack usage
	char no_thread_sig_patterns[][MAX_LEN_SIG_NAME_]  = {"Rainbow-IIIc", "Rainbow-Vc"};
	int test_in_thread = 1;
	for (size_t i = 0 ; i < sizeof(no_thread_sig_patterns) / MAX_LEN_SIG_NAME_; ++i) {
		if (strstr(alg_name, no_thread_sig_patterns[i]) != NULL) {
			test_in_thread = 0;
			break;
		}
	}
	if (test_in_thread) {
		pthread_t thread;
		struct thread_data td;
		td.alg_name = alg_name;
		int trc = pthread_create(&thread, NULL, test_wrapper, &td);
		if (trc) {
			fprintf(stderr, "ERROR: Creating pthread\n");
			return EXIT_FAILURE;
		}
		pthread_join(thread, NULL);
		rc = td.rc;
	} else {
		rc = sig_test_correctness(alg_name);
	}
#else
	rc = sig_test_correctness(alg_name);
#endif
	if (rc != OQS_SUCCESS) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
