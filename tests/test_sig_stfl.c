// SPDX-License-Identifier: MIT

#if defined(_WIN32)
#pragma warning(disable : 4244 4293)
#endif

#include <ctype.h>
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

/*
 * For stateful signature, we skip key generation because it can takes hours to complete.
 * So the ReadHex and and FindMarker serve the purpose of reading pre-generate keypair from KATs.
 */
#define MAX_MARKER_LEN 50

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int FindMarker(FILE *infile, const char *marker) {
	char line[MAX_MARKER_LEN];
	unsigned long i, len;
	int curr_line;

	len = strlen(marker);
	if (len > MAX_MARKER_LEN - 1) {
		len = MAX_MARKER_LEN - 1;
	}

	for (i = 0; i < len; i++) {
		curr_line = fgetc(infile);
		line[i] = (char)curr_line;
		if (curr_line == EOF) {
			return 0;
		}
	}
	line[len] = '\0';

	while (1) {
		if (!strncmp(line, marker, len)) {
			return 1;
		}

		for (i = 0; i < len - 1; i++) {
			line[i] = line[i + 1];
		}
		curr_line = fgetc(infile);
		line[len - 1] = (char)curr_line;
		if (curr_line == EOF) {
			return 0;
		}
		line[len] = '\0';
	}

	// shouldn't get here
	return 0;
}

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int ReadHex(FILE *infile, unsigned char *a, unsigned long Length, char *str) {
	int i, ch, started;
	unsigned char ich;

	if (Length == 0) {
		a[0] = 0x00;
		return 1;
	}
	memset(a, 0x00, Length);
	started = 0;
	if (FindMarker(infile, str))
		while ((ch = fgetc(infile)) != EOF) {
			if (!isxdigit(ch)) {
				if (!started) {
					if (ch == '\n') {
						break;
					} else {
						continue;
					}
				} else {
					break;
				}
			}
			started = 1;
			if ((ch >= '0') && (ch <= '9')) {
				ich = (unsigned char)ch - '0';
			} else if ((ch >= 'A') && (ch <= 'F')) {
				ich = (unsigned char)ch - 'A' + 10;
			} else if ((ch >= 'a') && (ch <= 'f')) {
				ich = (unsigned char)ch - 'a' + 10;
			} else {
				// shouldn't ever get here
				ich = 0;
			}

			for (i = 0; i < Length - 1; i++) {
				a[i] = (unsigned char) (a[i] << 4) | (unsigned char) (a[i + 1] >> 4);
			}
			a[Length - 1] = (unsigned char) (a[Length - 1] << 4) | (unsigned char) ich;
		} else {
		return 0;
	}

	return 1;
}

OQS_STATUS sig_stfl_keypair_from_keygen(OQS_SIG_STFL *sig, uint8_t *public_key, uint8_t *secret_key) {
	OQS_STATUS rc;
	rc = OQS_SIG_STFL_keypair(sig, public_key, secret_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_STATUS sig_stfl_keypair_from_KATs(OQS_SIG_STFL *sig, uint8_t *public_key, uint8_t *secret_key, const char *katfile) {
	OQS_STATUS ret = OQS_ERROR;
	FILE *fp_rsp = NULL;

	if ((fp_rsp = fopen(katfile, "r")) == NULL) {
		fprintf(stderr, "Couldn't open <%s> for read\n", katfile);
		goto err;
	}

	// Grab the pk and sk from KAT file
	if (!ReadHex(fp_rsp, public_key, sig->length_public_key, "pk = ")) {
		fprintf(stderr, "ERROR: unable to read 'pk' from <%s>\n", katfile);
		goto err;
	}

	if (!ReadHex(fp_rsp, secret_key, sig->length_secret_key, "sk = ")) {
		fprintf(stderr, "ERROR: unable to read 'sk' from <%s>\n", katfile);
		goto err;
	}

	// We are done reading, clean up and exit
	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	fclose(fp_rsp);
	return ret;
}

/*
 * We read from KATs these parameters:
 * XMSS-SHA2_16_256
 * XMSS-SHA2_20_256
 * XMSS-SHAKE_16_256
 * XMSS-SHAKE_20_256
 * XMSSMT-SHA2_40/2_256
 * XMSSMT-SHA2_60/3_256
 * XMSSMT-SHAKE_40/2_256
 * XMSSMT-SHAKE_60/3_256
 */
OQS_STATUS sig_stfl_KATs_keygen(OQS_SIG_STFL *sig, uint8_t *public_key, uint8_t *secret_key, const char *katfile) {

	printf("%s", sig->method_name);
	if (0) {

#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h16
	} else if (strcmp(sig->method_name, OQS_SIG_STFL_alg_xmss_sha256_h16) == 0) {
		goto from_kats;
#endif
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h20
	} else if (strcmp(sig->method_name, OQS_SIG_STFL_alg_xmss_sha256_h20) == 0) {
		goto from_kats;
#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h16
	} else if (0 == strcasecmp(sig->method_name, OQS_SIG_STFL_alg_xmss_shake128_h16)) {
		goto from_kats;
#endif
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h20
	} else if (0 == strcasecmp(sig->method_name, OQS_SIG_STFL_alg_xmss_shake128_h20)) {
		goto from_kats;
#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h16
	} else if (0 == strcasecmp(sig->method_name, OQS_SIG_STFL_alg_xmss_sha512_h16)) {
		goto from_kats;
#endif
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h20
	} else if (0 == strcasecmp(sig->method_name, OQS_SIG_STFL_alg_xmss_sha512_h20)) {
		goto from_kats;
#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16
	} else if (0 == strcasecmp(sig->method_name, OQS_SIG_STFL_alg_xmss_shake256_h16)) {
		goto from_kats;
#endif
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20
	} else if (0 == strcasecmp(sig->method_name, OQS_SIG_STFL_alg_xmss_shake256_h20)) {
		goto from_kats;
#endif

#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_2
	} else if (0 == strcasecmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_2)) {
		goto from_kats;
#endif
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_3
	} else if (0 == strcasecmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_3)) {
		goto from_kats;
#endif

#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_2
	} else if (0 == strcasecmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_2)) {
		goto from_kats;
#endif
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_3
	} else if (0 == strcasecmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_3)) {
		goto from_kats;
#endif
	} else {
		goto from_keygen;
	}

from_kats:
	return sig_stfl_keypair_from_KATs(sig, public_key, secret_key, katfile);

from_keygen:
	return sig_stfl_keypair_from_keygen(sig, public_key, secret_key);
}

typedef struct magic_s {
	uint8_t val[31];
} magic_t;

static OQS_STATUS sig_stfl_test_correctness(const char *method_name, const char *katfile) {

	OQS_SIG_STFL *sig = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
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
		fprintf(stderr, "ERROR: OQS_SIG_STFL_new failed\n");
		goto err;
	}

	printf("================================================================================\n");
	printf("Sample computation for stateful signature %s\n", sig->method_name);
	printf("================================================================================\n");

	public_key = malloc(sig->length_public_key + 2 * sizeof(magic_t));
	secret_key = malloc(sig->length_secret_key + 2 * sizeof(magic_t));
	message = malloc(message_len + 2 * sizeof(magic_t));
	signature = malloc(sig->length_signature + 2 * sizeof(magic_t));

	if ((public_key == NULL) || (secret_key == NULL) || (message == NULL) || (signature == NULL)) {
		fprintf(stderr, "ERROR: malloc failed\n");
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

	/*
	 * Some keypair generation is fast, so we only read keypair from KATs for slow XMSS parameters
	 */
	rc = sig_stfl_KATs_keygen(sig, public_key, secret_key, katfile);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_keypair failed\n");
		goto err;
	}

	rc = OQS_SIG_STFL_sign(sig, signature, &signature_len, message, message_len, secret_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_sign failed\n");
		goto err;
	}

	OQS_TEST_CT_DECLASSIFY(public_key, sig->length_public_key);
	OQS_TEST_CT_DECLASSIFY(signature, signature_len);
	rc = OQS_SIG_STFL_verify(sig, message, message_len, signature, signature_len, public_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_verify failed\n");
		goto err;
	}

	/* modify the signature to invalidate it */
	OQS_randombytes(signature, signature_len);
	OQS_TEST_CT_DECLASSIFY(signature, signature_len);
	rc = OQS_SIG_STFL_verify(sig, message, message_len, signature, signature_len, public_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_ERROR) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_verify should have failed!\n");
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
	OQS_SIG_STFL_free(sig);

	return ret;
}

static OQS_STATUS sig_stfl_test_secret_key(const char *method_name) {
	OQS_STATUS rc = OQS_SUCCESS;
	OQS_SIG_STFL_SECRET_KEY *sk = NULL;

	sk = OQS_SIG_STFL_SECRET_KEY_new(method_name);
	if (sk == NULL) {
		fprintf(stderr, "ERROR: OQS_SECRET_KEY_new failed\n");
		goto err;
	}

	printf("================================================================================\n");
	printf("Create stateful Secret Key  %s\n", method_name);
	printf("================================================================================\n");

	if (!sk->secret_key_data) {
		fprintf(stderr, "ERROR: OQS_SECRET_KEY_new incomplete.\n");
		goto err;
	}

	OQS_SIG_STFL_SECRET_KEY_free(sk);
	printf("Secret Key created as expected.\n");
	goto end_it;

err:
	rc = OQS_ERROR;
end_it:
	return rc;
}

#ifdef OQS_ENABLE_TEST_CONSTANT_TIME
static void TEST_SIG_STFL_randombytes(uint8_t *random_array, size_t bytes_to_read) {
	// We can't make direct calls to the system randombytes on some platforms,
	// so we have to swap out the OQS_randombytes provider.
	OQS_randombytes_switch_algorithm("system");
	OQS_randombytes(random_array, bytes_to_read);
	OQS_randombytes_custom_algorithm(&TEST_SIG_STFL_randombytes);

	// OQS_TEST_CT_CLASSIFY tells Valgrind's memcheck tool to issue a warning if
	// the program branches on any byte that depends on random_array. This helps us
	// identify timing side-channels, as these bytes often contain secret data.
	OQS_TEST_CT_CLASSIFY(random_array, bytes_to_read);
}
#endif

#if OQS_USE_PTHREADS_IN_TESTS
struct thread_data {
	const char *alg_name;
	const char *katfile;
	OQS_STATUS rc;
	OQS_STATUS rc1;
};

void *test_wrapper(void *arg) {
	struct thread_data *td = arg;
	td->rc = sig_stfl_test_correctness(td->alg_name, td->katfile);
	td->rc1 = sig_stfl_test_secret_key(td->alg_name);
	return NULL;
}
#endif

int main(int argc, char **argv) {
	OQS_init();

	printf("Testing stateful signature algorithms using liboqs version %s\n", OQS_version());

	if (argc < 2) {
		fprintf(stderr, "Usage: test_sig_stfl algname katfile\n");
		fprintf(stderr, "  algname: ");
		for (size_t i = 0; i < OQS_SIG_STFL_algs_length; i++) {
			if (i > 0) {
				fprintf(stderr, ", ");
			}
			fprintf(stderr, "%s", OQS_SIG_STFL_alg_identifier(i));
		}
		fprintf(stderr, "\n");
		OQS_destroy();
		return EXIT_FAILURE;
	}

	print_system_info();

	const char *alg_name = argv[1];
	const char *katfile = argv[2];
	if (!OQS_SIG_STFL_alg_is_enabled(alg_name)) {
		printf("Stateful signature algorithm %s not enabled!\n", alg_name);
		OQS_destroy();
		return EXIT_FAILURE;
	}

#ifdef OQS_ENABLE_TEST_CONSTANT_TIME
	OQS_randombytes_custom_algorithm(&TEST_SIG_STFL_randombytes);
#else
	OQS_randombytes_switch_algorithm("system");
#endif

	OQS_STATUS rc, rc1;
#if OQS_USE_PTHREADS_IN_TESTS
#define MAX_LEN_SIG_NAME_ 64
	pthread_t thread;
	struct thread_data td;
	td.alg_name = alg_name;
	td.katfile = katfile;
	int trc = pthread_create(&thread, NULL, test_wrapper, &td);
	if (trc) {
		fprintf(stderr, "ERROR: Creating pthread\n");
		OQS_destroy();
		return EXIT_FAILURE;
	}
	pthread_join(thread, NULL);
	rc = td.rc;
	rc1 = td.rc1;
#else
	rc = sig_stfl_test_correctness(alg_name, katfile);
	rc1 = sig_stfl_test_secret_key(alg_name);
#endif
	if ((rc != OQS_SUCCESS) || (rc1 != OQS_SUCCESS)) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	OQS_destroy();
	return EXIT_SUCCESS;
}
