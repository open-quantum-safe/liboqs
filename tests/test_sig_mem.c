// SPDX-License-Identifier: MIT

#if defined(_WIN32)
#pragma warning(disable : 4244 4293)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

#include "system_info.c"
#include "tmp_store.c"

typedef enum sig_ops {
	SIG_KEYGEN = 0,
	SIG_SIGN   = 1,
	SIG_VERIFY = 2
} SIG_OPS;

static OQS_STATUS sig_test_correctness(const char *method_name, SIG_OPS op) {

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

	switch (op) {
	case SIG_KEYGEN:
		printf("================================================================================\n");
		printf("Executing keygen for SIGALG %s\n", sig->method_name);
		printf("================================================================================\n");

		public_key = malloc(sig->length_public_key);
		secret_key = malloc(sig->length_secret_key);
		if ((public_key == NULL) || (secret_key == NULL)) {
			fprintf(stderr, "ERROR: malloc failed\n");
			goto err;
		}
		rc = OQS_SIG_keypair(sig, public_key, secret_key);
		if (rc != OQS_SUCCESS) {
			fprintf(stderr, "ERROR: OQS_SIG_keypair failed\n");
			goto err;
		}
		if (oqs_fstore("pk", method_name, public_key, sig->length_public_key) != OQS_SUCCESS) {
			goto err;
		}
		if (oqs_fstore("sk", method_name, secret_key, sig->length_secret_key) != OQS_SUCCESS) {
			goto err;
		}
		ret = OQS_SUCCESS;
		goto cleanup;

	case SIG_SIGN:
		printf("================================================================================\n");
		printf("Executing sign for SIGALG %s\n", sig->method_name);
		printf("================================================================================\n");

		public_key = malloc(sig->length_public_key);
		secret_key = malloc(sig->length_secret_key);
		message = malloc(message_len);
		signature = malloc(sig->length_signature);

		if ((public_key == NULL) || (secret_key == NULL) || (message == NULL) || (signature == NULL)) {
			fprintf(stderr, "ERROR: malloc failed\n");
			goto err;
		}
		if (oqs_fload("pk", method_name, public_key, sig->length_public_key, &signature_len) != OQS_SUCCESS) {
			goto err;
		}
		if (oqs_fload("sk", method_name, secret_key, sig->length_secret_key, &signature_len) != OQS_SUCCESS) {
			goto err;
		}

		OQS_randombytes(message, message_len);

		rc = OQS_SIG_sign(sig, signature, &signature_len, message, message_len, secret_key);
		if (rc != OQS_SUCCESS) {
			fprintf(stderr, "ERROR: OQS_SIG_sign failed\n");
			goto err;
		}
		if (oqs_fstore("ct", method_name, message, message_len) != OQS_SUCCESS) {
			goto err;
		}
		if (oqs_fstore("se", method_name, signature, signature_len) != OQS_SUCCESS) {
			goto err;
		}
		ret = OQS_SUCCESS;
		goto cleanup;

	case SIG_VERIFY:
		printf("================================================================================\n");
		printf("Executing verify for SIGALG %s\n", sig->method_name);
		printf("================================================================================\n");

		public_key = malloc(sig->length_public_key);
		secret_key = malloc(sig->length_secret_key);
		message = malloc(message_len);
		signature = malloc(sig->length_signature);

		if ((public_key == NULL) || (secret_key == NULL) || (message == NULL) || (signature == NULL)) {
			fprintf(stderr, "ERROR: malloc failed\n");
			goto err;
		}
		if (oqs_fload("pk", method_name, public_key, sig->length_public_key, &signature_len) != OQS_SUCCESS) {
			goto err;
		}
		if (oqs_fload("sk", method_name, secret_key, sig->length_secret_key, &signature_len) != OQS_SUCCESS) {
			goto err;
		}
		if (oqs_fload("ct", method_name, message, message_len, &signature_len) != OQS_SUCCESS) {
			goto err;
		}
		if (oqs_fload("se", method_name, signature, sig->length_signature, &signature_len) != OQS_SUCCESS) {
			goto err;
		}

		rc = OQS_SIG_verify(sig, message, message_len, signature, signature_len, public_key);
		if (rc != OQS_SUCCESS) {
			fprintf(stderr, "ERROR: OQS_SIG_verify failed\n");
			goto err;
		}

		printf("verification passes as expected\n");
		ret = OQS_SUCCESS;
		goto cleanup;

	default:
		fprintf(stderr, "Incorrect operation requested. Aborting.\n");
		goto err;
	}

err:
	ret = OQS_ERROR;

cleanup:
	if (public_key) {
		OQS_MEM_insecure_free(public_key);
	}
	if (message) {
		OQS_MEM_insecure_free(message);
	}
	if (signature) {
		OQS_MEM_insecure_free(signature);
	}
	if (sig != NULL) {
		OQS_MEM_secure_free(secret_key, sig->length_secret_key);
		OQS_SIG_free(sig);
	}

	return ret;
}

int main(int argc, char **argv) {
	OQS_init();

	if (argc != 3) {
		fprintf(stderr, "Usage: test_sig algname operation (0,1,2)\n");
		fprintf(stderr, "  algname: ");
		for (size_t i = 0; i < OQS_SIG_algs_length; i++) {
			if (i > 0) {
				fprintf(stderr, ", ");
			}
			fprintf(stderr, "%s", OQS_SIG_alg_identifier(i));
		}
		fprintf(stderr, "\n");
		OQS_destroy();
		return EXIT_FAILURE;
	}

	print_system_info();

	char *alg_name = argv[1];
	if (!OQS_SIG_alg_is_enabled(alg_name)) {
		printf("Signature algorithm %s not enabled!\n", alg_name);
		OQS_destroy();
		return EXIT_FAILURE;
	}

	// Use system RNG in this program
	OQS_randombytes_switch_algorithm(OQS_RAND_alg_system);

	oqs_fstore_init();

	OQS_STATUS rc = sig_test_correctness(alg_name, (unsigned int)atoi(argv[2]));

	if (rc != OQS_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	OQS_destroy();
	return EXIT_SUCCESS;
}
