// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

#if OQS_USE_PTHREADS_IN_TESTS
#include <pthread.h>
#endif

#include "system_info.c"

/* Displays hexadecimal strings */
static void OQS_print_hex_string(const char *label, const uint8_t *str, size_t len) {
	printf("%-20s (%4zu bytes):  ", label, len);
	for (size_t i = 0; i < (len); i++) {
		printf("%02X", str[i]);
	}
	printf("\n");
}

typedef struct magic_s {
	uint8_t val[32];
} magic_t;

static OQS_STATUS kem_test_correctness(const char *method_name) {

	OQS_KEM *kem = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *ciphertext = NULL;
	uint8_t *shared_secret_e = NULL;
	uint8_t *shared_secret_d = NULL;
	OQS_STATUS rc, ret = OQS_ERROR;
	int rv;

	//The magic numbers are 32 random values.
	//The length of the magic number was chosen arbitrarilly to 32.
	magic_t magic = {{
			0xfa, 0xfa, 0xfa, 0xfa, 0xbc, 0xbc, 0xbc, 0xbc,
			0x15, 0x61, 0x15, 0x61, 0x15, 0x61, 0x15, 0x61,
			0xad, 0xad, 0x43, 0x43, 0xad, 0xad, 0x34, 0x34,
			0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78
		}
	};

	kem = OQS_KEM_new(method_name);
	if (kem == NULL) {
		fprintf(stderr, "ERROR: OQS_KEM_new failed\n");
		goto err;
	}

	printf("================================================================================\n");
	printf("Sample computation for KEM %s\n", kem->method_name);
	printf("================================================================================\n");

	public_key = malloc(kem->length_public_key + sizeof(magic_t));
	secret_key = malloc(kem->length_secret_key + sizeof(magic_t));
	ciphertext = malloc(kem->length_ciphertext + sizeof(magic_t));
	shared_secret_e = malloc(kem->length_shared_secret + sizeof(magic_t));
	shared_secret_d = malloc(kem->length_shared_secret + sizeof(magic_t));

	//Set the magic numbers
	memcpy(public_key + kem->length_public_key, magic.val, sizeof(magic_t));
	memcpy(secret_key + kem->length_secret_key, magic.val, sizeof(magic_t));
	memcpy(ciphertext + kem->length_ciphertext, magic.val, sizeof(magic_t));
	memcpy(shared_secret_e + kem->length_shared_secret, magic.val, sizeof(magic_t));
	memcpy(shared_secret_d + kem->length_shared_secret, magic.val, sizeof(magic_t));

	if ((public_key == NULL) || (secret_key == NULL) || (ciphertext == NULL) || (shared_secret_e == NULL) || (shared_secret_d == NULL)) {
		fprintf(stderr, "ERROR: malloc failed\n");
		goto err;
	}

	rc = OQS_KEM_keypair(kem, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_keypair failed\n");
		goto err;
	}

	rc = OQS_KEM_encaps(kem, ciphertext, shared_secret_e, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_encaps failed\n");
		goto err;
	}

	rc = OQS_KEM_decaps(kem, shared_secret_d, ciphertext, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_decaps failed\n");
		goto err;
	}

	rv = memcmp(shared_secret_e, shared_secret_d, kem->length_shared_secret);
	if (rv != 0) {
		fprintf(stderr, "ERROR: shared secrets are not equal\n");
		OQS_print_hex_string("shared_secret_e", shared_secret_e, kem->length_shared_secret);
		OQS_print_hex_string("shared_secret_d", shared_secret_d, kem->length_shared_secret);
		goto err;
	} else {
		printf("shared secrets are equal\n");
	}

	rv = memcmp(public_key + kem->length_public_key, magic.val, sizeof(magic_t));
	rv |= memcmp(secret_key + kem->length_secret_key, magic.val, sizeof(magic_t));
	rv |= memcmp(ciphertext + kem->length_ciphertext, magic.val, sizeof(magic_t));
	rv |= memcmp(shared_secret_e + kem->length_shared_secret, magic.val, sizeof(magic_t));
	rv |= memcmp(shared_secret_d + kem->length_shared_secret, magic.val, sizeof(magic_t));
	if (rv != 0) {
		fprintf(stderr, "ERROR: Magic numbers do not match\n");
		goto err;
	}

	// test invalid encapsulation (call should either fail or result in invalid shared secret)
	OQS_randombytes(ciphertext, kem->length_ciphertext);
	rc = OQS_KEM_decaps(kem, shared_secret_d, ciphertext, secret_key);
	if (rc == OQS_SUCCESS && memcmp(shared_secret_e, shared_secret_d, kem->length_shared_secret) == 0) {
		fprintf(stderr, "ERROR: OQS_KEM_decaps succeeded on wrong input\n");
		goto err;
	}

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (kem != NULL) {
		OQS_MEM_secure_free(secret_key, kem->length_secret_key);
		OQS_MEM_secure_free(shared_secret_e, kem->length_shared_secret);
		OQS_MEM_secure_free(shared_secret_d, kem->length_shared_secret);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(ciphertext);
	OQS_KEM_free(kem);

	return ret;
}

#if OQS_USE_PTHREADS_IN_TESTS
struct thread_data {
	char *alg_name;
	OQS_STATUS rc;
};

void *test_wrapper(void *arg) {
	struct thread_data *td = arg;
	td->rc = kem_test_correctness(td->alg_name);
	return NULL;
}
#endif

int main(int argc, char **argv) {

	if (argc != 2) {
		fprintf(stderr, "Usage: test_kem algname\n");
		fprintf(stderr, "  algname: ");
		for (size_t i = 0; i < OQS_KEM_algs_length; i++) {
			if (i > 0) {
				fprintf(stderr, ", ");
			}
			fprintf(stderr, "%s", OQS_KEM_alg_identifier(i));
		}
		fprintf(stderr, "\n");
		return EXIT_FAILURE;
	}

	print_system_info();

	char *alg_name = argv[1];
	if (!OQS_KEM_alg_is_enabled(alg_name)) {
		printf("KEM algorithm %s not enabled!\n", alg_name);
		return EXIT_FAILURE;
	}

	// Use system RNG in this program
	OQS_randombytes_switch_algorithm(OQS_RAND_alg_system);

	OQS_STATUS rc;
#if OQS_USE_PTHREADS_IN_TESTS
#define MAX_LEN_KEM_NAME_ 64
	// don't run Classic McEliece in threads because of large stack usage
	char no_thread_kem_patterns[][MAX_LEN_KEM_NAME_]  = {"Classic-McEliece", "HQC-256-"};
	int test_in_thread = 1;
	for (size_t i = 0 ; i < sizeof(no_thread_kem_patterns) / MAX_LEN_KEM_NAME_; ++i) {
		if (strstr(alg_name, no_thread_kem_patterns[i]) != NULL) {
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
		rc = kem_test_correctness(alg_name);
	}
#else
	rc = kem_test_correctness(alg_name);
#endif
	if (rc != OQS_SUCCESS) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
