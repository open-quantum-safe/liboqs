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

typedef struct magic_s {
	uint8_t val[31];
} magic_t;

#define MAX_FILE_NAME_LEN 60
char filename[MAX_FILE_NAME_LEN] = "XMSS-SHA2_10_256";

void prepend(char *s, const char *t) {
	size_t len = strlen(t);
	memmove(s + len, s, strlen(s) + 1);
	memcpy(s, t, len);
}

OQS_STATUS lock_sk_key(OQS_SECRET_KEY *sk) {
	printf("lk=%02x\n", sk->secret_key[0]);
	return 0;
}

OQS_STATUS release_sk_key(OQS_SECRET_KEY *sk) {
	printf("rk=%02x\n", sk->secret_key[0]);
	return 0;
}

OQS_STATUS do_nothing_save(const OQS_SECRET_KEY *sk) {
	printf("save_k=%02x\n", sk->secret_key[0]);
	return 0;
}

OQS_STATUS sk_file_write(const OQS_SECRET_KEY *sk) {

	prepend(filename, "./tmp/oqs_");
	strcat(filename, "__sk");

	printf("\nWriting to file %s\n", filename);
	FILE *printer = fopen(filename, "w+");
	if (printer == NULL) {
		perror("ERROR! There is no such file. Terminating...");
		return -1;
	}

	// Write the entire secret key byte array to the specified file.
	for (unsigned long i = 0; i < sk->length_secret_key; i++) {
		if (fputc(sk->secret_key[i], printer) == EOF) {
			perror("ERROR! There is no such file. Terminating...");
			return -1;
		}
	}
	fclose(printer);
	printf("Completed the write operation\n");
	return 0;
}

static OQS_STATUS sig_test_correctness(const char *method_name) {

	OQS_SIG_STFL *sig = NULL;
	uint8_t *public_key = NULL;
	OQS_SECRET_KEY *secret_key = NULL;
	uint8_t *message = NULL;
	size_t message_len = 100;
	uint8_t *signature = NULL;
	size_t signature_len;
	OQS_STATUS rc, ret = OQS_ERROR;

	//The magic numbers are random values.
	//The length of the magic number was chosen to be 31 to break alignment
	magic_t magic;
	OQS_randombytes(magic.val, sizeof(magic_t));

	sig = OQS_SIG_STFL_new(method_name);
	if (sig == NULL) {
		fprintf(stderr, "ERROR: OQS_SIG_new failed\n");
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
	secret_key->lock_key = lock_sk_key;
	secret_key->release_key = release_sk_key;
	secret_key->oqs_save_updated_sk_key = sk_file_write;
	if (secret_key == NULL) {
		fprintf(stderr, "ERROR: OQS_SECRET_KEY_new failed\n");
		goto err;
	}

	//Set the magic numbers before
	memcpy(public_key, magic.val, sizeof(magic_t));
	// memcpy(secret_key, magic.val, sizeof(magic_t));
	memcpy(message, magic.val, sizeof(magic_t));
	memcpy(signature, magic.val, sizeof(magic_t));

	public_key += sizeof(magic_t);
	//secret_key->secret_key += sizeof(magic_t);
	message += sizeof(magic_t);
	signature += sizeof(magic_t);

	// and after
	memcpy(public_key + sig->length_public_key, magic.val, sizeof(magic_t));
	// memcpy(secret_key + sizeof(secret_key), magic.val, sizeof(magic_t));
	memcpy(message + message_len, magic.val, sizeof(magic_t));
	memcpy(signature + sig->length_signature, magic.val, sizeof(magic_t));

	OQS_randombytes(message, message_len);
	OQS_TEST_CT_DECLASSIFY(message, message_len);

	rc = OQS_SIG_STFL_keypair(sig, public_key, secret_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_keypair failed\n");
		goto err;
	}

	rc = OQS_SIG_STFL_sign(sig, signature, &signature_len, message, message_len, secret_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_sign failed\n");
		goto err;
	}

	OQS_TEST_CT_DECLASSIFY(public_key, sig->length_public_key);
	OQS_TEST_CT_DECLASSIFY(signature, signature_len);
	rc = OQS_SIG_STFL_verify(sig, message, message_len, signature, signature_len, public_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_verify failed\n");
		goto err;
	}

	/* modify the signature to invalidate it */
	OQS_randombytes(signature, signature_len);
	OQS_TEST_CT_DECLASSIFY(signature, signature_len);
	rc = OQS_SIG_STFL_verify(sig, message, message_len, signature, signature_len, public_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_ERROR) {
		fprintf(stderr, "ERROR: OQS_SIG_verify should have failed!\n");
		goto err;
	}

#ifndef OQS_ENABLE_TEST_CONSTANT_TIME
	/* check magic values */
	int rv = memcmp(public_key + sig->length_public_key, magic.val, sizeof(magic_t));
	//rv |= memcmp(secret_key->secret_key + secret_key->length_secret_key, magic.val, sizeof(magic_t));
	rv |= memcmp(message + message_len, magic.val, sizeof(magic_t));
	rv |= memcmp(signature + sig->length_signature, magic.val, sizeof(magic_t));
	rv |= memcmp(public_key - sizeof(magic_t), magic.val, sizeof(magic_t));
	//rv |= memcmp(secret_key- sizeof(magic_t), magic.val, sizeof(magic_t));
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
	if (sig != NULL) {
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
		for (size_t i = 0; i < OQS_SIG_STFL_algs_length; i++) {
			if (i > 0) {
				fprintf(stderr, ", ");
			}
			fprintf(stderr, "%s", OQS_SIG_STFL_alg_identifier(i));
		}
		fprintf(stderr, "\n");
		return EXIT_FAILURE;
	}

	print_system_info();

	char *alg_name = argv[1];
	if (!OQS_SIG_STFL_alg_is_enabled(alg_name)) {
		printf("Signature algorithm %s not enabled!\n", alg_name);
		return EXIT_FAILURE;
	}
	strcpy(filename, alg_name);

#ifdef OQS_ENABLE_TEST_CONSTANT_TIME
	OQS_randombytes_custom_algorithm(&TEST_SIG_randombytes);
#else
	OQS_randombytes_switch_algorithm("system");
#endif

	OQS_STATUS rc;
#if OQS_USE_PTHREADS_IN_TESTS
#define MAX_LEN_SIG_NAME_ 64
	// don't run Rainbow III and V in threads because of large stack usage
	char no_thread_sig_patterns[][MAX_LEN_SIG_NAME_]  = {"Rainbow-III", "Rainbow-V"};
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
