// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#if defined(_WIN32)
#include <string.h>
#define strcasecmp _stricmp
#else
#include <strings.h>
#endif

#include <oqs/oqs.h>
#include <oqs/sha3.h>
#if OQS_USE_PTHREADS
#include <pthread.h>
#endif

#ifdef OQS_ENABLE_KEM_ML_KEM
#define MLKEM_SECRET_LEN      32
#endif

#include "system_info.c"
#include "test_helpers.h"

#ifdef OQS_ENABLE_KEM_ML_KEM
/* mlkem rejection key testcase */
static bool mlkem_rej_testcase(OQS_KEM *kem, uint8_t *ciphertext, uint8_t *secret_key) {
	// sanity checks
	if ((kem == NULL) || (ciphertext == NULL) || (secret_key == NULL)) {
		fprintf(stderr, "ERROR: inputs NULL!\n");
		return false;
	}
	// Only run tests for ML-KEM
	if (!(strcasecmp(kem->method_name, OQS_KEM_alg_ml_kem_512) == 0 ||
	        strcasecmp(kem->method_name, OQS_KEM_alg_ml_kem_768) == 0 ||
	        strcasecmp(kem->method_name, OQS_KEM_alg_ml_kem_1024) == 0)) {
		return true;
	}
	// Buffer to hold z and c. z is always 32 bytes
	uint8_t *buff_z_c = NULL;
	bool retval = false;
	OQS_STATUS rc;
	int rv;
	size_t length_z_c = 32 + kem->length_ciphertext;
	buff_z_c = OQS_MEM_malloc(length_z_c);
	if (buff_z_c == NULL) {
		fprintf(stderr, "ERROR: OQS_MEM_malloc failed\n");
		return false;
	}
	// Scenario 1: Test rejection key by corrupting the secret key
	secret_key[0] += 1;
	uint8_t shared_secret_r[MLKEM_SECRET_LEN]; // expected output
	uint8_t shared_secret_d[MLKEM_SECRET_LEN]; // calculated output
	memcpy(buff_z_c, &secret_key[kem->length_secret_key - 32], 32);
	memcpy(&buff_z_c[MLKEM_SECRET_LEN], ciphertext, kem->length_ciphertext);
	// Calculate expected secret in case of corrupted cipher: shake256(z || c)
	OQS_SHA3_shake256(shared_secret_r, MLKEM_SECRET_LEN, buff_z_c, length_z_c);
	rc = OQS_KEM_decaps(kem, shared_secret_d, ciphertext, secret_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_decaps failed for rejection testcase scenario 1\n");
		goto cleanup;
	}
	OQS_TEST_CT_DECLASSIFY(shared_secret_d, MLKEM_SECRET_LEN);
	OQS_TEST_CT_DECLASSIFY(shared_secret_r, MLKEM_SECRET_LEN);
	rv = memcmp(shared_secret_d, shared_secret_r, MLKEM_SECRET_LEN);
	if (rv != 0) {
		fprintf(stderr, "ERROR: shared secrets are not equal for rejection key in decapsulation scenario 1\n");
		OQS_print_hex_string("shared_secret_d", shared_secret_d, MLKEM_SECRET_LEN);
		OQS_print_hex_string("shared_secret_r", shared_secret_r, MLKEM_SECRET_LEN);
		goto cleanup;
	}
	secret_key[0] -= 1; // Restore private key
	OQS_MEM_cleanse(buff_z_c, length_z_c); // Reset buffer

	// Scenario 2: Test rejection key by corrupting the ciphertext
	ciphertext[0] += 1;
	memcpy(buff_z_c, &secret_key[kem->length_secret_key - 32], 32);
	memcpy(&buff_z_c[MLKEM_SECRET_LEN], ciphertext, kem->length_ciphertext);

	// Calculate expected secret in case of corrupted cipher: shake256(z || c)
	OQS_SHA3_shake256(shared_secret_r, MLKEM_SECRET_LEN, buff_z_c, length_z_c);
	rc = OQS_KEM_decaps(kem, shared_secret_d, ciphertext, secret_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_decaps failed for rejection testcase scenario 2\n");
		goto cleanup;
	}
	OQS_TEST_CT_DECLASSIFY(shared_secret_d, MLKEM_SECRET_LEN);
	OQS_TEST_CT_DECLASSIFY(shared_secret_r, MLKEM_SECRET_LEN);
	rv = memcmp(shared_secret_d, shared_secret_r, MLKEM_SECRET_LEN);
	if (rv != 0) {
		fprintf(stderr, "ERROR: shared secrets are not equal for rejection key in decapsulation scenario 2\n");
		OQS_print_hex_string("shared_secret_d", shared_secret_d, MLKEM_SECRET_LEN);
		OQS_print_hex_string("shared_secret_r", shared_secret_r, MLKEM_SECRET_LEN);
		goto cleanup;
	}
	ciphertext[0] -= 1; // Restore ciphertext
	retval = true;
cleanup:
	if (buff_z_c) {
		OQS_MEM_secure_free(buff_z_c, length_z_c);
	}
	return retval;
}
#endif //OQS_ENABLE_KEM_ML_KEM

typedef struct magic_s {
	uint8_t val[31];
} magic_t;

static OQS_STATUS kem_test_correctness(const char *method_name, bool derand) {

	OQS_KEM *kem = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *ciphertext = NULL;
	uint8_t *shared_secret_e = NULL;
	uint8_t *shared_secret_d = NULL;
	uint8_t *keypair_seed = NULL;
	uint8_t *encaps_seed = NULL;
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

	if (!derand) {
		printf("================================================================================\n");
		printf("sample computation for KEM %s\n", kem->method_name);
		printf("Version source: %s\n", kem->alg_version);
		printf("================================================================================\n");
	}

	/* pk, sk, ciphertext, shared secret, and shared secret cmp are needed
	 * regardless of derand or not
	 */
	public_key = OQS_MEM_malloc(kem->length_public_key + 2 * sizeof(magic_t));
	secret_key = OQS_MEM_malloc(kem->length_secret_key + 2 * sizeof(magic_t));
	ciphertext = OQS_MEM_malloc(kem->length_ciphertext + 2 * sizeof(magic_t));
	shared_secret_e = OQS_MEM_malloc(kem->length_shared_secret + 2 * sizeof(magic_t));
	shared_secret_d = OQS_MEM_malloc(kem->length_shared_secret + 2 * sizeof(magic_t));
	if ((public_key == NULL) || (secret_key == NULL) || (ciphertext == NULL) || (shared_secret_e == NULL) || (shared_secret_d == NULL)) {
		fprintf(stderr, "ERROR: OQS_MEM_malloc failed\n");
		goto err;
	}
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
	memcpy(public_key + kem->length_public_key, magic.val, sizeof(magic_t));
	memcpy(secret_key + kem->length_secret_key, magic.val, sizeof(magic_t));
	memcpy(ciphertext + kem->length_ciphertext, magic.val, sizeof(magic_t));
	memcpy(shared_secret_e + kem->length_shared_secret, magic.val, sizeof(magic_t));
	memcpy(shared_secret_d + kem->length_shared_secret, magic.val, sizeof(magic_t));

	if (derand) {
		/* keypair_seed and encaps_seed are only needed if derand */
		keypair_seed = malloc(kem->length_keypair_seed + 2 * sizeof(magic_t));
		if (!keypair_seed) {
			fprintf(stderr, "Failed to allocate %zu bytes for keypair seed\n",
			        kem->length_keypair_seed);
			goto err;
		}
		encaps_seed = malloc(kem->length_encaps_seed + 2 * sizeof(magic_t));
		if (!encaps_seed) {
			fprintf(stderr, "Failed to allocate %zu bytes for encaps seed\n",
			        kem->length_encaps_seed);
			goto err;
		}
		memcpy(keypair_seed, magic.val, sizeof(magic_t));
		memcpy(encaps_seed, magic.val, sizeof(magic_t));
		keypair_seed += sizeof(magic_t);
		encaps_seed += sizeof(magic_t);
		memcpy(keypair_seed + kem->length_keypair_seed, magic.val,
		       sizeof(magic_t));
		memcpy(encaps_seed + kem->length_encaps_seed, magic.val,
		       sizeof(magic_t));
	}



	if (derand) {
		// On some systems, getentropy fails if given a zero-length array
		if (kem->length_keypair_seed > 0) {
			OQS_randombytes(keypair_seed, kem->length_keypair_seed);
		}
		rc = OQS_KEM_keypair_derand(kem, public_key, secret_key, keypair_seed);
		OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
		if (kem->length_keypair_seed == 0) {
			// If length_keypair_seed is set to 0 for this KEM scheme, a failure is expected
			if (rc != OQS_ERROR) {
				fprintf(stderr, "ERROR: OQS_KEM_keypair_derand succeeded but expected a failure\n");
				goto err;
			}
			printf("OQS_KEM_keypair_derand failed, as expected\n");
			ret = OQS_SUCCESS;
			goto cleanup;
		} else {
			if (rc != OQS_SUCCESS) {
				fprintf(stderr, "ERROR: OQS_KEM_keypair_derand failed\n");
				goto err;
			}
		}
	} else {
		rc = OQS_KEM_keypair(kem, public_key, secret_key);
		OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
		if (rc != OQS_SUCCESS) {
			fprintf(stderr, "ERROR: OQS_KEM_keypair failed\n");
			goto err;
		}
	}

	OQS_TEST_CT_DECLASSIFY(public_key, kem->length_public_key);
	if (derand) {
		// On some systems, getentropy fails if given a zero-length array
		if (kem->length_encaps_seed > 0) {
			OQS_randombytes(encaps_seed, kem->length_encaps_seed);
		}
		rc = OQS_KEM_encaps_derand(kem, ciphertext, shared_secret_e, public_key, encaps_seed);
		OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
		if (kem->length_encaps_seed == 0) {
			// If length_encaps_seed is set to 0 for this KEM scheme, a failure is expected
			if (rc != OQS_ERROR) {
				fprintf(stderr, "ERROR: OQS_KEM_encaps_derand succeeded but expected a failure\n");
				goto err;
			}
			printf("OQS_KEM_encaps_derand failed, as expected\n");
			ret = OQS_SUCCESS;
			goto cleanup;
		} else {
			if (rc != OQS_SUCCESS) {
				fprintf(stderr, "ERROR: OQS_KEM_encaps_derand failed\n");
				goto err;
			}
		}
	} else {
		rc = OQS_KEM_encaps(kem, ciphertext, shared_secret_e, public_key);
		OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
		if (rc != OQS_SUCCESS) {
			fprintf(stderr, "ERROR: OQS_KEM_encaps failed\n");
			goto err;
		}
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

#ifdef OQS_ENABLE_KEM_ML_KEM
	/* check mlkem rejection testcases. returns true for all other kem algos */
	if (false == mlkem_rej_testcase(kem, ciphertext, secret_key)) {
		goto err;
	}
#endif

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
	if (derand) {
		rv |= memcmp(keypair_seed + kem->length_keypair_seed, magic.val, sizeof(magic_t));
		rv |= memcmp(encaps_seed + kem->length_encaps_seed, magic.val, sizeof(magic_t));
		rv |= memcmp(keypair_seed - sizeof(magic_t), magic.val, sizeof(magic_t));
		rv |= memcmp(encaps_seed - sizeof(magic_t), magic.val, sizeof(magic_t));
	}
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
	if ((secret_key) && (kem != NULL)) {
		OQS_MEM_secure_free(secret_key - sizeof(magic_t), kem->length_secret_key + 2 * sizeof(magic_t));
	}
	if ((shared_secret_e) && (kem != NULL)) {
		OQS_MEM_secure_free(shared_secret_e - sizeof(magic_t), kem->length_shared_secret + 2 * sizeof(magic_t));
	}
	if ((shared_secret_d) && (kem != NULL)) {
		OQS_MEM_secure_free(shared_secret_d - sizeof(magic_t), kem->length_shared_secret + 2 * sizeof(magic_t));
	}
	if (public_key) {
		OQS_MEM_insecure_free(public_key - sizeof(magic_t));
	}
	if (ciphertext) {
		OQS_MEM_insecure_free(ciphertext - sizeof(magic_t));
	}
	if ((keypair_seed) && (kem != NULL)) {
		OQS_MEM_secure_free(keypair_seed - sizeof(magic_t), kem->length_keypair_seed + 2 * sizeof(magic_t));
	}
	if ((encaps_seed) && (kem != NULL)) {
		OQS_MEM_secure_free(encaps_seed - sizeof(magic_t), kem->length_encaps_seed + 2 * sizeof(magic_t));
	}
	OQS_KEM_free(kem);

	return ret;
}

#ifdef OQS_ENABLE_TEST_CONSTANT_TIME
static void TEST_KEM_randombytes(uint8_t *random_array, size_t bytes_to_read) {
	// We can't make direct calls to the system randombytes on some platforms,
	// so we have to swap out the OQS_randombytes provider.
	(void)OQS_randombytes_switch_algorithm("system");
	OQS_randombytes(random_array, bytes_to_read);
	OQS_randombytes_custom_algorithm(&TEST_KEM_randombytes);

	// OQS_TEST_CT_CLASSIFY tells Valgrind's memcheck tool to issue a warning if
	// the program branches on any byte that depends on random_array. This helps us
	// identify timing side-channels, as these bytes often contain secret data.
	OQS_TEST_CT_CLASSIFY(random_array, bytes_to_read);
}
#endif

#if OQS_USE_PTHREADS
struct thread_data {
	char *alg_name;
	OQS_STATUS rc;
};

void *test_wrapper(void *arg) {
	struct thread_data *td = arg;
	td->rc = kem_test_correctness(td->alg_name, false);
	if (td->rc == OQS_SUCCESS) {
		// test derandomized operations
		td->rc = kem_test_correctness(td->alg_name, true);
	}
	OQS_thread_stop();
	return NULL;
}
#endif

int main(int argc, char **argv) {
	OQS_STATUS rc;
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
	rc = OQS_randombytes_switch_algorithm("system");
	if (rc != OQS_SUCCESS) {
		printf("Could not generate random data with system RNG\n");
		OQS_destroy();
		return EXIT_FAILURE;
	}
#endif

#if OQS_USE_PTHREADS
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
		rc = kem_test_correctness(alg_name, false);
		if (rc == OQS_SUCCESS) {
			// test with derandomized keygen
			rc = kem_test_correctness(alg_name, true);
		}
	}
#else
	rc = kem_test_correctness(alg_name, false);
	if (rc == OQS_SUCCESS) {
		// test with derandomized keygen
		rc = kem_test_correctness(alg_name, true);
	}
#endif
	if (rc != OQS_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	OQS_destroy();
	return EXIT_SUCCESS;
}
