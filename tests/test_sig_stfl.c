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

#ifdef OQS_ENABLE_TEST_CONSTANT_TIME
#include <valgrind/memcheck.h>
#define OQS_TEST_CT_CLASSIFY(addr, len)  VALGRIND_MAKE_MEM_UNDEFINED(addr, len)
#define OQS_TEST_CT_DECLASSIFY(addr, len)  VALGRIND_MAKE_MEM_DEFINED(addr, len)
#else
#define OQS_TEST_CT_CLASSIFY(addr, len)
#define OQS_TEST_CT_DECLASSIFY(addr, len)
#endif

#include "system_info.c"

// Useful for mode of operations
#define READING 'r'
#define SAVING 's'
#define NOTHING 'x'
#define DIRECTORY_PLUS_PREFIX "../tests/saved_keys/tmp_"

#define PRIVATE_KEY_EXT ".prv"
#define PUBLIC_KEY_EXT ".pub"

typedef struct magic_s {
	uint8_t val[31];
} magic_t;

void prepend(char *s, const char *t) {
	size_t len = strlen(t);
	memmove(s + len, s, strlen(s) + 1);
	memcpy(s, t, len);
}

OQS_STATUS lock_sk_key(OQS_SECRET_KEY *sk) {
	uint8_t ss = *(sk->secret_key);
	ss = 0;
	return ss == 0 ? OQS_SUCCESS : OQS_ERROR;
}

OQS_STATUS release_sk_key(OQS_SECRET_KEY *sk) {
	uint8_t ss = *(sk->secret_key);
	ss = 0;
	return ss == 0 ? OQS_SUCCESS : OQS_ERROR;
}

OQS_STATUS do_nothing_save(const OQS_SECRET_KEY *sk) {
	uint8_t ss = *(sk->secret_key);
	ss = 0;
	return ss == 0 ? OQS_SUCCESS : OQS_ERROR;
}

OQS_STATUS sk_file_write(const OQS_SECRET_KEY *sk) {
	char filename[64];
	strcpy(filename, (char *)sk->data);

	strcat(filename, PRIVATE_KEY_EXT);
	FILE *printer = fopen(filename, "w+");
	if (printer == NULL) {
		return OQS_ERROR;
	}

	// Write the entire secret key byte array to the specified file.
	for (unsigned long i = 0; i < sk->length_secret_key; i++) {
		if (fputc(sk->secret_key[i], printer) == EOF) {
			return OQS_ERROR;
		}
	}
	fclose(printer);
	return OQS_SUCCESS;
}

static OQS_STATUS sig_stfl_test_correctness(const char *method_name, char mode, const char *filestem) {

	OQS_SIG_STFL *sig = NULL;
	uint8_t *public_key = NULL;
	OQS_SECRET_KEY *secret_key = NULL;
	uint8_t *message = NULL;
	size_t message_len = 100;
	uint8_t *signature = NULL;
	size_t signature_len;
	OQS_STATUS rc, ret = OQS_ERROR;
	char filename[64];

	//The magic numbers are random values.
	//The length of the magic number was chosen to be 31 to break alignment
	magic_t magic;
	OQS_randombytes(magic.val, sizeof(magic_t));

	sig = OQS_SIG_STFL_new(method_name);
	if (sig == NULL) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_new failed\n");
		goto err;
	}

	printf("================================================================================\n");
	printf("Sample computation for signature %s\n", sig->method_name);
	printf("================================================================================\n");

	public_key = malloc(sig->length_public_key + 2 * sizeof(magic_t));
	message = malloc(message_len + 2 * sizeof(magic_t));
	signature = malloc(sig->length_signature + 2 * sizeof(magic_t));
	if ((public_key == NULL) || (message == NULL) || (signature == NULL)) {
		fprintf(stderr, "ERROR: malloc failed\n");
		goto err;
	}

	// Define the secret key object
	secret_key = OQS_SECRET_KEY_new(method_name);
	secret_key->data = (void *)filestem;
	secret_key->lock_key = lock_sk_key;
	secret_key->release_key = release_sk_key;
	if (mode == SAVING) {
		secret_key->save_secret_key = sk_file_write;
	} else {
		secret_key->save_secret_key = do_nothing_save;
	}

	if (secret_key == NULL) {
		fprintf(stderr, "ERROR: OQS_SECRET_KEY_new failed\n");
		goto err;
	}

	/**
	 * Magic numbers are not inserted into the secret key because for we need an
	 * OQS_SECRET_KEY object for the secret key. The secret key byte array is an attribute
	 * of the object whose size gets assigend when new() is called.
 	 */

	// Set the magic numbers before
	memcpy(public_key, magic.val, sizeof(magic_t));
	memcpy(message, magic.val, sizeof(magic_t));
	memcpy(signature, magic.val, sizeof(magic_t));

	public_key += sizeof(magic_t);
	message += sizeof(magic_t);
	signature += sizeof(magic_t);

	// and after
	memcpy(public_key + sig->length_public_key, magic.val, sizeof(magic_t));
	memcpy(message + message_len, magic.val, sizeof(magic_t));
	memcpy(signature + sig->length_signature, magic.val, sizeof(magic_t));

	// =================================== KEYPAIR / FILE READ =========================================

	OQS_randombytes(message, message_len);
	OQS_TEST_CT_DECLASSIFY(message, message_len);

	if (mode == READING) {
		strcpy(filename, filestem);
		prepend(filename, DIRECTORY_PLUS_PREFIX);

		// Public Key
		strcat(filename, PUBLIC_KEY_EXT);
		FILE *pub_key = fopen(filename, "rb");
		if (pub_key == NULL) {
			rc = OQS_ERROR;
		}
		for (unsigned int i = 0; i < sig->length_public_key; i++) {
			public_key[i] = fgetc(pub_key);
		}
		fclose(pub_key);

		// Private Key
		filename[strlen(filename) - strlen(PUBLIC_KEY_EXT)] = '\0';
		strcat(filename, PRIVATE_KEY_EXT);
		FILE *prv_key = fopen(filename, "rb");
		if (prv_key == NULL) {
			rc = OQS_ERROR;
		}
		for (unsigned int i = 0; i < secret_key->length_secret_key; i++) {
			secret_key->secret_key[i] = fgetc(prv_key);
		}
		fclose(prv_key);
	} else {
		rc = OQS_SIG_STFL_keypair(sig, public_key, secret_key);
	}
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		if (mode == READING) {
			fprintf(stderr, "ERROR: OQS_SIG_STFL read from file failed\n");
		} else {
			fprintf(stderr, "ERROR: OQS_SIG_STFL_keypair failed\n");
		}
		goto err;
	}

	// Save the keypair to the specified filestem
	if (mode == SAVING) {
		// Private Key
		secret_key->save_secret_key(secret_key);

		// Public Key
		strcpy(filename, filestem);
		strcat(filename, PUBLIC_KEY_EXT);
		FILE *pub_key = fopen(filename, "w+");
		if (pub_key == NULL) {
			fprintf(stderr, "ERROR: fopen failed.\n");
			rc = OQS_ERROR;
			goto err;
		}

		for (unsigned int i = 0; i < sig->length_public_key; i++) {
			if (fputc(public_key[i], pub_key) == EOF) {
				return OQS_ERROR;
			}
		}
		fclose(pub_key);
	}

	// ===========================================================================================


	// ====================================== SIGNING =============================================

	rc = OQS_SIG_STFL_sign(sig, signature, &signature_len, message, message_len, secret_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_sign failed\n");
		goto err;
	}

	// ===========================================================================================


	// =================================== VERIFICATION I =========================================

	OQS_TEST_CT_DECLASSIFY(public_key, sig->length_public_key);
	OQS_TEST_CT_DECLASSIFY(signature, signature_len);
	rc = OQS_SIG_STFL_verify(sig, message, message_len, signature, signature_len, public_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_verify failed\n");
		goto err;
	}

	// ===========================================================================================


	// ============================ VERIFICATION II (failure) =====================================

	/* modify the signature to invalidate it */
	OQS_randombytes(signature, signature_len);
	OQS_TEST_CT_DECLASSIFY(signature, signature_len);
	rc = OQS_SIG_STFL_verify(sig, message, message_len, signature, signature_len, public_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_ERROR) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_verify should have failed!\n");
		goto err;
	}

	// ===========================================================================================

#ifndef OQS_ENABLE_TEST_CONSTANT_TIME
	/* check magic values */
	int rv = memcmp(public_key + sig->length_public_key, magic.val, sizeof(magic_t));
	rv |= memcmp(message + message_len, magic.val, sizeof(magic_t));
	rv |= memcmp(signature + sig->length_signature, magic.val, sizeof(magic_t));
	rv |= memcmp(public_key - sizeof(magic_t), magic.val, sizeof(magic_t));
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
	if (secret_key != NULL) {
		OQS_SECRET_KEY_free(secret_key);
	}
	OQS_MEM_insecure_free(public_key - sizeof(magic_t));
	OQS_MEM_insecure_free(message - sizeof(magic_t));
	OQS_MEM_insecure_free(signature - sizeof(magic_t));

	OQS_SIG_STFL_free(sig);

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

#if OQS_USE_PTHREADS_IN_TESTS
struct thread_data {
	char *alg_name;
	char mode_of_operation;
	const char *filestem;
	OQS_STATUS rc;
};

void *test_wrapper(void *arg) {
	struct thread_data *td = arg;
	td->rc = sig_stfl_test_correctness(td->alg_name, td->mode_of_operation, td->filestem);
	return NULL;
}
#endif

int main(int argc, char **argv) {

	if (argc < 2) {
		fprintf(stderr, "Usage: test_sig_stfl algname [optional arguments]\n");
		fprintf(stderr, "  algname: ");
		for (size_t i = 0; i < OQS_SIG_STFL_algs_length; i++) {
			if (i > 0) {
				fprintf(stderr, ", ");
			}
			fprintf(stderr, "%s", OQS_SIG_STFL_alg_identifier(i));
		}
		fprintf(stderr, "\n");
		fprintf(stderr, "  optional arguments:\n");
		fprintf(stderr, "    --save_key_to=<filestem_name> saves the keypair to a .pub/.prv pair in tests/saved_keys\n");
		fprintf(stderr, "    --read_key_from=<filestem_name> reads the keypair from a *.pub/*.prv pair in tests/saved_keys\n");
		return EXIT_FAILURE;
	}

	print_system_info();

	// Check if the algorithm is supported
	char *alg_name = argv[1];
	if (!OQS_SIG_STFL_alg_is_enabled(alg_name)) {
		printf("Signature algorithm %s not enabled!\n", alg_name);
		return EXIT_FAILURE;
	}

	// Extract the mode of operation from the command line arguments
	char mode_of_operation = 'x';
	char *filestem = NULL;
	if (argc > 2) {
		char argument[8];
		strncpy(argument, argv[2], 6);
		if (strcmp(argument, "--save") == 0) {
			mode_of_operation = SAVING;
			filestem = argv[2] + strlen("--save_key_to=");
		} else if (strcmp(argument, "--read") == 0) {
			mode_of_operation = READING;
			filestem = argv[2] + strlen("--read_key_from=");
		} else {
			fprintf(stderr, "Invalid mode of operation: %s\n", argv[2]);
			return EXIT_FAILURE;
		}
	} else {
		mode_of_operation = NOTHING;
	}

#ifdef OQS_ENABLE_TEST_CONSTANT_TIME
	OQS_randombytes_custom_algorithm(&TEST_SIG_randombytes);
#else
	OQS_randombytes_switch_algorithm("system");
#endif

	OQS_STATUS rc;
#if OQS_USE_PTHREADS_IN_TESTS
#define MAX_LEN_SIG_NAME_ 64

	pthread_t thread;
	struct thread_data td;
	td.alg_name = alg_name;
	td.mode_of_operation = mode_of_operation;
	td.filestem = filestem;
	int trc = pthread_create(&thread, NULL, test_wrapper, &td);
	if (trc) {
		fprintf(stderr, "ERROR: Creating pthread\n");
		return EXIT_FAILURE;
	}
	pthread_join(thread, NULL);
	rc = td.rc;
#else
	rc = sig_stfl_test_correctness(alg_name);
#endif
	if (rc != OQS_SUCCESS) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
