// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

#if CMAKE_USE_PTHREADS_INIT
#include <pthread.h>
#endif

#ifdef OQS_ENABLE_TEST_CONSTANT_TIME
#include <valgrind/memcheck.h>
#define OQS_TEST_CT_CLASSIFY(addr, len)  VALGRIND_MAKE_MEM_UNDEFINED(addr, len)
#define OQS_TEST_CT_DECLASSIFY(addr, len)  VALGRIND_MAKE_MEM_DEFINED(addr, len)
#else
#define OQS_TEST_CT_CLASSIFY(addr, len)
#define OQS_TEST_CT_DECLASSIFY(addr, len)
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
	uint8_t val[31];
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

	//The magic numbers are random values.
	//The length of the magic number was chosen to be 31 to break alignment
	magic_t magic;
	OQS_randombytes(magic.val, sizeof(magic_t));

	kem = OQS_KEM_new(method_name);
	if (kem == NULL) {
		fprintf(stderr, "ERROR: OQS_KEM_new failed\n");
		goto err;
	}

	printf("================================================================================\n");
	printf("Sample computation for KEM %s\n", kem->method_name);
	printf("================================================================================\n");

	public_key = malloc(kem->length_public_key + 2 * sizeof(magic_t));
	secret_key = malloc(kem->length_secret_key + 2 * sizeof(magic_t));
	ciphertext = malloc(kem->length_ciphertext + 2 * sizeof(magic_t));
	shared_secret_e = malloc(kem->length_shared_secret + 2 * sizeof(magic_t));
	shared_secret_d = malloc(kem->length_shared_secret + 2 * sizeof(magic_t));

	if ((public_key == NULL) || (secret_key == NULL) || (ciphertext == NULL) || (shared_secret_e == NULL) || (shared_secret_d == NULL)) {
		fprintf(stderr, "ERROR: malloc failed\n");
		goto err;
	}

	//Set the magic numbers before
	memcpy(public_key, magic.val, sizeof(magic_t));
	memcpy(secret_key, magic.val, sizeof(magic_t));
	memcpy(ciphertext, magic.val, sizeof(magic_t));
	memcpy(shared_secret_e, magic.val, sizeof(magic_t));
	memcpy(shared_secret_d, magic.val, sizeof(magic_t));

	public_key += sizeof(magic_t);
	secret_key += sizeof(magic_t);
	ciphertext += sizeof(magic_t);
	shared_secret_e += sizeof(magic_t);
	shared_secret_d += sizeof(magic_t);

	// and after
	memcpy(public_key + kem->length_public_key, magic.val, sizeof(magic_t));
	memcpy(secret_key + kem->length_secret_key, magic.val, sizeof(magic_t));
	memcpy(ciphertext + kem->length_ciphertext, magic.val, sizeof(magic_t));
	memcpy(shared_secret_e + kem->length_shared_secret, magic.val, sizeof(magic_t));
	memcpy(shared_secret_d + kem->length_shared_secret, magic.val, sizeof(magic_t));

	rc = OQS_KEM_keypair(kem, public_key, secret_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_keypair failed\n");
		goto err;
	}

	OQS_TEST_CT_DECLASSIFY(public_key, kem->length_public_key);
	rc = OQS_KEM_encaps(kem, ciphertext, shared_secret_e, public_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_encaps failed\n");
		goto err;
	}

	OQS_TEST_CT_DECLASSIFY(ciphertext, kem->length_ciphertext);
	rc = OQS_KEM_decaps(kem, shared_secret_d, ciphertext, secret_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_decaps failed\n");
		goto err;
	}

	OQS_TEST_CT_DECLASSIFY(shared_secret_d, kem->length_shared_secret);
	OQS_TEST_CT_DECLASSIFY(shared_secret_e, kem->length_shared_secret);
	rv = memcmp(shared_secret_e, shared_secret_d, kem->length_shared_secret);
	if (rv != 0) {
		fprintf(stderr, "ERROR: shared secrets are not equal\n");
		OQS_print_hex_string("shared_secret_e", shared_secret_e, kem->length_shared_secret);
		OQS_print_hex_string("shared_secret_d", shared_secret_d, kem->length_shared_secret);
		goto err;
	} else {
		printf("shared secrets are equal\n");
	}

	// test invalid encapsulation (call should either fail or result in invalid shared secret)
	OQS_randombytes(ciphertext, kem->length_ciphertext);
	OQS_TEST_CT_DECLASSIFY(ciphertext, kem->length_ciphertext);
	rc = OQS_KEM_decaps(kem, shared_secret_d, ciphertext, secret_key);
	OQS_TEST_CT_DECLASSIFY(shared_secret_d, kem->length_shared_secret);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc == OQS_SUCCESS && memcmp(shared_secret_e, shared_secret_d, kem->length_shared_secret) == 0) {
		fprintf(stderr, "ERROR: OQS_KEM_decaps succeeded on wrong input\n");
		goto err;
	}

#ifndef OQS_ENABLE_TEST_CONSTANT_TIME
	rv = memcmp(public_key + kem->length_public_key, magic.val, sizeof(magic_t));
	rv |= memcmp(secret_key + kem->length_secret_key, magic.val, sizeof(magic_t));
	rv |= memcmp(ciphertext + kem->length_ciphertext, magic.val, sizeof(magic_t));
	rv |= memcmp(shared_secret_e + kem->length_shared_secret, magic.val, sizeof(magic_t));
	rv |= memcmp(shared_secret_d + kem->length_shared_secret, magic.val, sizeof(magic_t));
	rv |= memcmp(public_key - sizeof(magic_t), magic.val, sizeof(magic_t));
	rv |= memcmp(secret_key - sizeof(magic_t), magic.val, sizeof(magic_t));
	rv |= memcmp(ciphertext - sizeof(magic_t), magic.val, sizeof(magic_t));
	rv |= memcmp(shared_secret_e - sizeof(magic_t), magic.val, sizeof(magic_t));
	rv |= memcmp(shared_secret_d - sizeof(magic_t), magic.val, sizeof(magic_t));
	if (rv != 0) {
		fprintf(stderr, "ERROR: Magic numbers do not match\n");
		goto err;
	}
#endif

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (secret_key) {
		OQS_MEM_secure_free(secret_key - sizeof(magic_t), kem->length_secret_key + 2 * sizeof(magic_t));
	}
	if (shared_secret_e) {
		OQS_MEM_secure_free(shared_secret_e - sizeof(magic_t), kem->length_shared_secret + 2 * sizeof(magic_t));
	}
	if (shared_secret_d) {
		OQS_MEM_secure_free(shared_secret_d - sizeof(magic_t), kem->length_shared_secret + 2 * sizeof(magic_t));
	}
	if (public_key) {
		OQS_MEM_insecure_free(public_key - sizeof(magic_t));
	}
	if (ciphertext) {
		OQS_MEM_insecure_free(ciphertext - sizeof(magic_t));
	}
	OQS_KEM_free(kem);

	return ret;
}

#ifdef OQS_ENABLE_TEST_CONSTANT_TIME
static void TEST_KEM_randombytes(uint8_t *random_array, size_t bytes_to_read) {
	// We can't make direct calls to the system randombytes on some platforms,
	// so we have to swap out the OQS_randombytes provider.
	OQS_randombytes_switch_algorithm("system");
	OQS_randombytes(random_array, bytes_to_read);
	OQS_randombytes_custom_algorithm(&TEST_KEM_randombytes);

	// OQS_TEST_CT_CLASSIFY tells Valgrind's memcheck tool to issue a warning if
	// the program branches on any byte that depends on random_array. This helps us
	// identify timing side-channels, as these bytes often contain secret data.
	OQS_TEST_CT_CLASSIFY(random_array, bytes_to_read);
}
#endif

#if CMAKE_USE_PTHREADS_INIT
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
	OQS_init();

	printf("Testing KEM algorithms using liboqs version %s\n", OQS_version());

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
		OQS_destroy();
		return EXIT_FAILURE;
	}

	print_system_info();

	char *alg_name = argv[1];
	if (!OQS_KEM_alg_is_enabled(alg_name)) {
		printf("KEM algorithm %s not enabled!\n", alg_name);
		OQS_destroy();
		return EXIT_FAILURE;
	}

#ifdef OQS_ENABLE_TEST_CONSTANT_TIME
	OQS_randombytes_custom_algorithm(&TEST_KEM_randombytes);
#else
	OQS_randombytes_switch_algorithm("system");
#endif

	OQS_STATUS rc;
#if CMAKE_USE_PTHREADS_INIT
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
			OQS_destroy();
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
		OQS_destroy();
		return EXIT_FAILURE;
	}
	OQS_destroy();
	return EXIT_SUCCESS;
}
