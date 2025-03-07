// SPDX-License-Identifier: MIT

#if defined(_WIN32)
#pragma warning(disable : 4244 4293)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

#if OQS_USE_PTHREADS
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

typedef struct magic_s {
	uint8_t val[31];
} magic_t;

static OQS_STATUS flip_bit(uint8_t *array, uint64_t array_length, uint64_t bit_position) {
	uint64_t byte_index = bit_position / 8;
	uint8_t bit_index = bit_position % 8;
	if (byte_index >= array_length) {
		fprintf(stderr, "ERROR: flip_bit index is out of bounds!\n");
		return OQS_ERROR;
	}
	array[byte_index] ^= (1 << bit_index);
	return OQS_SUCCESS;
}

/* flip bits of the message (or signature) one at a time, and check that the verification fails */
static OQS_STATUS test_bitflip(OQS_SIG *sig, uint8_t *message, size_t message_len, uint8_t *signature, size_t signature_len, uint8_t *public_key, bool bitflips_all[2], size_t bitflips[2], bool use_ctx, uint8_t *ctx, size_t ctx_i) {
	OQS_STATUS rc;
	/* the first test (EUF-CMA) flips bits of the message
	   the second test (SUF-CMA) flips bits of the signature */
	int num_tests = sig->suf_cma ? 2 : 1;
	for (int test = 0; test < num_tests; test++) {
		/* select the array to tamper with (message or signature) */
		uint8_t *tampered_array = (test == 1) ? signature : message;
		size_t tampered_array_len = (test == 1) ? signature_len : message_len;
		/* select the number of bitflips */
		uint64_t bitflips_selected = bitflips_all[test] ? tampered_array_len * 8 : bitflips[test];
		for (uint64_t i = 0; i < bitflips_selected; i ++) {
			uint64_t random_bit_index;
			OQS_randombytes((uint8_t *)&random_bit_index, sizeof(i));
			random_bit_index = random_bit_index % (tampered_array_len * 8);
			uint64_t bit_index = bitflips_all[test] ? i : random_bit_index;
			/* flip the bit */
			rc = flip_bit(tampered_array, tampered_array_len, bit_index);
			if (rc != OQS_SUCCESS) {
				return OQS_ERROR;
			}
			/* check that the verification fails */
			if (use_ctx) {
				rc = OQS_SIG_verify_with_ctx_str(sig, message, message_len, signature, signature_len, ctx, ctx_i, public_key);
			} else {
				rc = OQS_SIG_verify(sig, message, message_len, signature, signature_len, public_key);
			}
			if (rc != OQS_ERROR) {
				fprintf(stderr, "ERROR: OQS_SIG_verify should have failed after flipping bit %llu of the %s!\n", (unsigned long long)bit_index, (test == 0) ? "message" : "signature");
				return OQS_ERROR;
			}
			/* flip back the bit */
			rc = flip_bit(tampered_array, tampered_array_len, bit_index);
			if (rc != OQS_SUCCESS) {
				return OQS_ERROR;
			}
		}
	}
	return OQS_SUCCESS;
}


static OQS_STATUS sig_test_correctness(const char *method_name, bool bitflips_all[2], size_t bitflips[2]) {

	OQS_SIG *sig = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *message = NULL;
	size_t message_len = 100;
	uint8_t ctx[257] = { 0 };
	uint8_t *signature = NULL;
	size_t signature_len;
	OQS_STATUS rc, ret = OQS_ERROR;

	//The magic numbers are random values.
	//The length of the magic number was chosen to be 31 to break alignment
	magic_t magic;
	OQS_randombytes(magic.val, sizeof(magic_t));

	sig = OQS_SIG_new(method_name);
	if (sig == NULL) {
		fprintf(stderr, "ERROR: OQS_SIG_new failed\n");
		goto err;
	}

	char bitflips_as_str[2][50];
	for (int i = 0; i < 2; i++) {
		if (bitflips_all[i]) {
			snprintf(bitflips_as_str[i], sizeof(bitflips_as_str[i]), "all");
		} else {
			snprintf(bitflips_as_str[i], sizeof(bitflips_as_str[i]), "%ld random", bitflips[i]);
		}
	}

	printf("================================================================================\n");
	printf("Sample computation for signature %s\n", sig->method_name);
	if (sig->euf_cma) {
		printf("Testing EUF-CMA by flipping %s bits of the message\n", bitflips_as_str[0]);
	}
	if (sig->suf_cma) {
		printf("Testing SUF-CMA by flipping %s bits of the signature\n", bitflips_as_str[1]);
	}
	printf("Version source: %s\n", sig->alg_version);
	printf("================================================================================\n");

	public_key = OQS_MEM_malloc(sig->length_public_key + 2 * sizeof(magic_t));
	secret_key = OQS_MEM_malloc(sig->length_secret_key + 2 * sizeof(magic_t));
	message = OQS_MEM_malloc(message_len + 2 * sizeof(magic_t));
	signature = OQS_MEM_malloc(sig->length_signature + 2 * sizeof(magic_t));

	if ((public_key == NULL) || (secret_key == NULL) || (message == NULL) || (signature == NULL)) {
		fprintf(stderr, "ERROR: OQS_MEM_malloc failed\n");
		goto err;
	}

	//Set the magic numbers before
	memcpy(public_key, magic.val, sizeof(magic_t));
	memcpy(secret_key, magic.val, sizeof(magic_t));
	memcpy(message, magic.val, sizeof(magic_t));
	memcpy(signature, magic.val, sizeof(magic_t));

	public_key += sizeof(magic_t);
	secret_key += sizeof(magic_t);
	message += sizeof(magic_t);
	signature += sizeof(magic_t);

	// and after
	memcpy(public_key + sig->length_public_key, magic.val, sizeof(magic_t));
	memcpy(secret_key + sig->length_secret_key, magic.val, sizeof(magic_t));
	memcpy(message + message_len, magic.val, sizeof(magic_t));
	memcpy(signature + sig->length_signature, magic.val, sizeof(magic_t));

	OQS_randombytes(message, message_len);
	OQS_TEST_CT_DECLASSIFY(message, message_len);

	rc = OQS_SIG_keypair(sig, public_key, secret_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_keypair failed\n");
		goto err;
	}

	rc = OQS_SIG_sign(sig, signature, &signature_len, message, message_len, secret_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_sign failed\n");
		goto err;
	}

	OQS_TEST_CT_DECLASSIFY(public_key, sig->length_public_key);
	OQS_TEST_CT_DECLASSIFY(signature, signature_len);
	rc = OQS_SIG_verify(sig, message, message_len, signature, signature_len, public_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_verify failed\n");
		goto err;
	}

	rc = test_bitflip(sig, message, message_len, signature, signature_len, public_key, bitflips_all, bitflips, false, NULL, 0);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		goto err;
	}


	/* testing signing with context, if supported */
	OQS_randombytes(ctx, 257);
	if (sig->sig_with_ctx_support) {
		for (size_t i = 0; i < 256; ++i) {
			rc = OQS_SIG_sign_with_ctx_str(sig, signature, &signature_len, message, message_len, ctx, i, secret_key);
			OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
			if (rc != OQS_SUCCESS) {
				fprintf(stderr, "ERROR: OQS_SIG_sign_with_ctx_str failed\n");
				goto err;
			}

			OQS_TEST_CT_DECLASSIFY(public_key, sig->length_public_key);
			OQS_TEST_CT_DECLASSIFY(signature, signature_len);
			rc = OQS_SIG_verify_with_ctx_str(sig, message, message_len, signature, signature_len, ctx, i, public_key);
			OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
			if (rc != OQS_SUCCESS) {
				fprintf(stderr, "ERROR: OQS_SIG_verify_with_ctx_str failed\n");
				goto err;
			}

			rc = test_bitflip(sig, message, message_len, signature, signature_len, public_key, bitflips_all, bitflips, true, ctx, i);
			OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
			if (rc != OQS_SUCCESS) {
				goto err;
			}

		}

		rc = OQS_SIG_sign_with_ctx_str(sig, signature, &signature_len, message, message_len, ctx, 256, secret_key);
		OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
		if (rc != OQS_ERROR) {
			fprintf(stderr, "ERROR: OQS_SIG_sign_with_ctx_str should only support up to 255 byte contexts\n");
			goto err;
		}
	} else {
		rc = OQS_SIG_sign_with_ctx_str(sig, signature, &signature_len, message, message_len, ctx, 1, secret_key);
		if (rc != OQS_ERROR) {
			fprintf(stderr, "ERROR: OQS_SIG_sign_with_ctx_str should fail without support for context strings\n");
			goto err;
		}
	}

	rc = OQS_SIG_sign_with_ctx_str(sig, signature, &signature_len, message, message_len, NULL, 0, secret_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_sign_with_ctx_str should always succeed when providing a NULL context string\n");
		goto err;
	}
	OQS_TEST_CT_DECLASSIFY(public_key, sig->length_public_key);
	OQS_TEST_CT_DECLASSIFY(signature, signature_len);
	rc = OQS_SIG_verify_with_ctx_str(sig, message, message_len, signature, signature_len, NULL, 0, public_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_verify_with_ctx_str failed\n");
		goto err;
	}

	rc = test_bitflip(sig, message, message_len, signature, signature_len, public_key, bitflips_all, bitflips, true, NULL, 0);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		goto err;
	}

#ifndef OQS_ENABLE_TEST_CONSTANT_TIME
	/* check magic values */
	int rv = memcmp(public_key + sig->length_public_key, magic.val, sizeof(magic_t));
	rv |= memcmp(secret_key + sig->length_secret_key, magic.val, sizeof(magic_t));
	rv |= memcmp(message + message_len, magic.val, sizeof(magic_t));
	rv |= memcmp(signature + sig->length_signature, magic.val, sizeof(magic_t));
	rv |= memcmp(public_key - sizeof(magic_t), magic.val, sizeof(magic_t));
	rv |= memcmp(secret_key - sizeof(magic_t), magic.val, sizeof(magic_t));
	rv |= memcmp(message - sizeof(magic_t), magic.val, sizeof(magic_t));
	rv |= memcmp(signature - sizeof(magic_t), magic.val, sizeof(magic_t));
	if (rv) {
		fprintf(stderr, "ERROR: Magic numbers do not match\n");
		goto err;
	}
#endif

	printf("verification passes as expected\n");
	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (secret_key) {
		OQS_MEM_secure_free(secret_key - sizeof(magic_t), sig->length_secret_key + 2 * sizeof(magic_t));
	}
	if (public_key) {
		OQS_MEM_insecure_free(public_key - sizeof(magic_t));
	}
	if (message) {
		OQS_MEM_insecure_free(message - sizeof(magic_t));
	}
	if (signature) {
		OQS_MEM_insecure_free(signature - sizeof(magic_t));
	}
	OQS_SIG_free(sig);

	return ret;
}

#ifdef OQS_ENABLE_TEST_CONSTANT_TIME
static void TEST_SIG_randombytes(uint8_t *random_array, size_t bytes_to_read) {
	// We can't make direct calls to the system randombytes on some platforms,
	// so we have to swap out the OQS_randombytes provider.
	OQS_randombytes_switch_algorithm("system");
	OQS_randombytes(random_array, bytes_to_read);
	OQS_randombytes_custom_algorithm(&TEST_SIG_randombytes);

	// OQS_TEST_CT_CLASSIFY tells Valgrind's memcheck tool to issue a warning if
	// the program branches on any byte that depends on random_array. This helps us
	// identify timing side-channels, as these bytes often contain secret data.
	OQS_TEST_CT_CLASSIFY(random_array, bytes_to_read);
}
#endif

#if OQS_USE_PTHREADS
struct thread_data {
	char *alg_name;
	bool *bitflips_all;
	size_t *bitflips;
	OQS_STATUS rc;
};

void *test_wrapper(void *arg) {
	struct thread_data *td = arg;
	td->rc = sig_test_correctness(td->alg_name, td->bitflips_all, td->bitflips);
	OQS_thread_stop();
	return NULL;
}
#endif

int main(int argc, char **argv) {
	OQS_init();

	printf("Testing signature algorithms using liboqs version %s\n", OQS_version());

	if (argc < 2 || argc > 4) {
		fprintf(stderr, "Usage: test_sig algname [bitflips_msg] [bitflips_sig]\n");
		fprintf(stderr, "  algname: ");
		for (size_t i = 0; i < OQS_SIG_algs_length; i++) {
			if (i > 0) {
				fprintf(stderr, ", ");
			}
			fprintf(stderr, "%s", OQS_SIG_alg_identifier(i));
		}
		fprintf(stderr, "\n");
		fprintf(stderr, "  bitflips_msg: the number of random bitflips to perform for each EUF-CMA signature (\"all\" to flip every bit)\n");
		fprintf(stderr, "  bitflips_sig: the number of random bitflips to perform for each SUF-CMA signature (\"all\" to flip every bit)\n");
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

	/* by default, flip 50 random bits of the message and signature (to test EUF-CMA and SUF-CMA, respectively) */
	bool bitflips_all[2] = {false, false};
	size_t bitflips[2] = {50, 50};
	if (argc >= 3) {
		if (strcmp(argv[2], "all") == 0) {
			bitflips_all[0] = true;
		} else {
			bitflips[0] = (size_t)strtol(argv[2], NULL, 10);
		}
	}
	if (argc == 4) {
		if (strcmp(argv[3], "all") == 0) {
			bitflips_all[1] = true;
		} else {
			bitflips[1] = (size_t)strtol(argv[3], NULL, 10);
		}
	}

#ifdef OQS_ENABLE_TEST_CONSTANT_TIME
	OQS_randombytes_custom_algorithm(&TEST_SIG_randombytes);
#else
	OQS_randombytes_switch_algorithm("system");
#endif

	OQS_STATUS rc;
#if OQS_USE_PTHREADS
#define MAX_LEN_SIG_NAME_ 64
	// don't run algorithms with large stack usage in threads
	char no_thread_sig_patterns[][MAX_LEN_SIG_NAME_]  = {"MAYO-5", "cross-rsdp-128-small", "cross-rsdp-192-small", "cross-rsdp-256-balanced", "cross-rsdp-256-small", "cross-rsdpg-192-small", "cross-rsdpg-256-small"};
	int test_in_thread = 1;
	for (size_t i = 0 ; i < sizeof(no_thread_sig_patterns) / MAX_LEN_SIG_NAME_; ++i) {
		if (strstr(alg_name, no_thread_sig_patterns[i]) != NULL) {
			test_in_thread = 0;
			break;
		}
	}
	if (test_in_thread) {
		pthread_t thread;
		struct thread_data td = {.alg_name = alg_name, .bitflips_all = bitflips_all, .bitflips = bitflips, .rc = OQS_ERROR};
		int trc = pthread_create(&thread, NULL, test_wrapper, &td);
		if (trc) {
			fprintf(stderr, "ERROR: Creating pthread\n");
			OQS_destroy();
			return EXIT_FAILURE;
		}
		pthread_join(thread, NULL);
		rc = td.rc;
	} else {
		rc = sig_test_correctness(alg_name, bitflips_all, bitflips);
	}
#else
	rc = sig_test_correctness(alg_name, bitflips_all, bitflips);
#endif
	if (rc != OQS_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	OQS_destroy();
	return EXIT_SUCCESS;
}
