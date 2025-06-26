// SPDX-License-Identifier: MIT

#if defined(_WIN32)
#pragma warning(disable : 4244 4293)
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#if defined(_WIN32)
#include <string.h>
#define strcasecmp _stricmp
#else
#include <strings.h>
#endif

#include <time.h>

#include <oqs/oqs.h>
#include "tmp_store.c"
#include "system_info.c"
#include "test_helpers.h"

#if OQS_USE_PTHREADS_IN_TESTS
#include <pthread.h>
static pthread_mutex_t *test_sk_lock = NULL;
static pthread_mutex_t *sk_lock = NULL;
#endif

#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif

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

	memset(line, 0, MAX_MARKER_LEN);
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
	int ch, started;
	unsigned long i;
	unsigned char ich;

	if (Length == 0) {
		a[0] = 0x00;
		return 1;
	}
	OQS_MEM_cleanse(a, Length);
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

/*
 * Write stateful secret keys to disk.
 */
static OQS_STATUS save_secret_key(uint8_t *key_buf, size_t buf_len, void *context) {
	if (key_buf == NULL || buf_len == 0 || context == NULL) {
		return OQS_ERROR;
	}
	const char *context_char = context;

	if (oqs_fstore("sk", context_char, key_buf, buf_len) == OQS_SUCCESS) {
		printf("\n================================================================================\n");
		printf("Updated STFL SK  <%s>.\n", context_char);
		printf("================================================================================\n");
		return OQS_SUCCESS;
	}

	return OQS_ERROR;
}

#if OQS_USE_PTHREADS_IN_TESTS

static OQS_SIG_STFL_SECRET_KEY *lock_test_sk = NULL;
static OQS_SIG_STFL *lock_test_sig_obj = NULL;
static uint8_t *lock_test_public_key = NULL;
static char *lock_test_context = NULL;
static uint8_t *signature_1 = NULL;
static uint8_t *signature_2 = NULL;
static size_t signature_len_1;
static size_t signature_len_2;
static uint8_t message_1[] = "The quick brown fox ...";
static uint8_t message_2[] = "The quick brown fox jumped from the tree.";

static OQS_STATUS lock_sk_key(void *mutex) {
	if (mutex == NULL) {
		return OQS_ERROR;
	}

	if (pthread_mutex_lock((pthread_mutex_t *)mutex)) {
		return OQS_ERROR;
	}
	return  OQS_SUCCESS;
}

static OQS_STATUS unlock_sk_key(void *mutex) {
	if (mutex == NULL) {
		return OQS_ERROR;
	}

	if (pthread_mutex_unlock((pthread_mutex_t *)mutex)) {
		return OQS_ERROR;
	}
	return  OQS_SUCCESS;
}
#else
static OQS_STATUS lock_sk_key(UNUSED void *mutex) {
	return OQS_SUCCESS;
}

static OQS_STATUS unlock_sk_key(UNUSED void *mutex) {
	return OQS_SUCCESS;
}
#endif

OQS_STATUS sig_stfl_keypair_from_keygen(OQS_SIG_STFL *sig, uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key) {
	OQS_STATUS rc;

	rc = OQS_SIG_STFL_keypair(sig, public_key, secret_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_STATUS sig_stfl_keypair_from_KATs(OQS_SIG_STFL *sig, uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key, const char *katfile) {
	OQS_STATUS ret = OQS_ERROR;
	FILE *fp_rsp = NULL;

	if ((fp_rsp = fopen(katfile, "r")) == NULL) {
		fprintf(stderr, "Couldn't open <%s> for read\n", katfile);
		goto err;
	}

	// Grab the pk and sk from KAT file
	if (!ReadHex(fp_rsp, public_key, sig->length_public_key, (char *)"pk = ")) {
		fprintf(stderr, "ERROR: unable to read 'pk' from <%s>\n", katfile);
		goto err;
	}

	if (!ReadHex(fp_rsp, secret_key->secret_key_data, sig->length_secret_key, (char *)"sk = ")) {
		fprintf(stderr, "ERROR: unable to read 'sk' from <%s>\n", katfile);
		goto err;
	}

	// We are done reading, clean up and exit
	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (fp_rsp) {
		fclose(fp_rsp);
	}
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
OQS_STATUS sig_stfl_KATs_keygen(OQS_SIG_STFL *sig, uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key, const char *katfile) {
	if (sig == NULL || public_key == NULL || secret_key == NULL ) {
		return OQS_ERROR;
	}

#ifdef OQS_ENABLE_SIG_STFL_XMSS
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

#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h16_192
	} else if (0 == strcasecmp(sig->method_name, OQS_SIG_STFL_alg_xmss_sha256_h16_192)) {
		goto from_kats;
#endif
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h20_192
	} else if (0 == strcasecmp(sig->method_name, OQS_SIG_STFL_alg_xmss_sha256_h20_192)) {
		goto from_kats;
#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16_192
	} else if (0 == strcasecmp(sig->method_name, OQS_SIG_STFL_alg_xmss_shake256_h16_192)) {
		goto from_kats;
#endif
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20_192
	} else if (0 == strcasecmp(sig->method_name, OQS_SIG_STFL_alg_xmss_shake256_h20_192)) {
		goto from_kats;
#endif

#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16_256
	} else if (0 == strcasecmp(sig->method_name, OQS_SIG_STFL_alg_xmss_shake256_h16_256)) {
		goto from_kats;
#endif
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20_256
	} else if (0 == strcasecmp(sig->method_name, OQS_SIG_STFL_alg_xmss_shake256_h20_256)) {
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
#endif //OQS_ENABLE_SIG_STFL_XMSS
	(void)(katfile);
	return sig_stfl_keypair_from_keygen(sig, public_key, secret_key);
}

typedef struct magic_s {
	uint8_t val[31];
} magic_t;

static char *convert_method_name_to_file_name(const char *method_name) {
	if (method_name == NULL) {
		return NULL;
	}

	const char *file_store = NULL;
	if (strcmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h20_2) == 0) {
		file_store = "XMSSMT-SHA2_20-2_256";
	} else if (strcmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h20_4) == 0) {
		file_store = "XMSSMT-SHA2_20-4_256";
	} else if (strcmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_2) == 0) {
		file_store = "XMSSMT-SHA2_40-2_256";
	} else if (strcmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_4) == 0) {
		file_store = "XMSSMT-SHA2_40-4_256";
	} else if (strcmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_8) == 0) {
		file_store = "XMSSMT-SHA2_40-8_256";
	} else if (strcmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_3) == 0) {
		file_store = "XMSSMT-SHA2_60-3_256";
	} else if (strcmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_6) == 0) {
		file_store = "XMSSMT-SHA2_60-6_256";
	} else if (strcmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_12) == 0) {
		file_store = "XMSSMT-SHA2_60-12_256";
	} else if (strcmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h20_2) == 0) {
		file_store = "XMSSMT-SHAKE_20-2_256";
	} else if (strcmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h20_4) == 0) {
		file_store = "XMSSMT-SHAKE_20-4_256";
	} else if (strcmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_2) == 0) {
		file_store = "XMSSMT-SHAKE_40-2_256";
	} else if (strcmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_4) == 0) {
		file_store = "XMSSMT-SHAKE_40-4_256";
	} else if (strcmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_8) == 0) {
		file_store = "XMSSMT-SHAKE_40-8_256";
	} else if (strcmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_3) == 0) {
		file_store = "XMSSMT-SHAKE_60-3_256";
	} else if (strcmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_6) == 0) {
		file_store = "XMSSMT-SHAKE_60-6_256";
	} else if (strcmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_12) == 0) {
		file_store = "XMSSMT-SHAKE_60-12_256";
	} else {
		file_store = method_name;
	}

	return strdup(file_store);
}

static OQS_STATUS sig_stfl_test_correctness(const char *method_name, const char *katfile, bool bitflips_all[2], size_t bitflips[2]) {

	OQS_SIG_STFL *sig = NULL;
	uint8_t *public_key = NULL;
	OQS_SIG_STFL_SECRET_KEY *secret_key = NULL;
	OQS_SIG_STFL_SECRET_KEY *secret_key_rd = NULL;
	uint8_t *message = NULL;
	size_t message_len = 100;
	uint8_t *signature = NULL;
	size_t signature_len;

	uint8_t *sk_buf = NULL;
	uint8_t *read_pk_buf = NULL;
	char *context = NULL;
	char *file_store = NULL;
	size_t sk_buf_len = 0;
	size_t read_pk_len = 0;

	magic_t magic;

	OQS_STATUS rc, ret = OQS_ERROR;

	//The magic numbers are random values.
	//The length of the magic number was chosen to be 31 to break alignment
	OQS_randombytes(magic.val, sizeof(magic_t));

	sig = OQS_SIG_STFL_new(method_name);
	if (sig == NULL) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_new failed\n");
		goto err;
	}

	char bitflips_as_str[2][50];
	for (int i = 0; i < 2; i++) {
		if (bitflips_all[i]) {
			snprintf(bitflips_as_str[i], sizeof(bitflips_as_str[i]), "all");
		} else {
			snprintf(bitflips_as_str[i], sizeof(bitflips_as_str[i]), "%zu random", bitflips[i]);
		}
	}

	printf("================================================================================\n");
	printf("Sample computation for stateful signature %s\n", sig->method_name);
	printf("Sample computation for signature %s\n", sig->method_name);
	if (sig->euf_cma) {
		printf("Testing EUF-CMA by flipping %s bits of the message\n", bitflips_as_str[0]);
	}
	if (sig->suf_cma) {
		printf("Testing SUF-CMA by flipping %s bits of the signature\n", bitflips_as_str[1]);
	}
	printf("Version source: %s\n", sig->alg_version);
	printf("================================================================================\n");

	secret_key = OQS_SIG_STFL_SECRET_KEY_new(sig->method_name);
	secret_key_rd = OQS_SIG_STFL_SECRET_KEY_new(sig->method_name);

	OQS_SIG_STFL_SECRET_KEY_SET_lock(secret_key, lock_sk_key);
	OQS_SIG_STFL_SECRET_KEY_SET_unlock(secret_key, unlock_sk_key);

	file_store = convert_method_name_to_file_name(sig->method_name);
	if (file_store == NULL) {
		fprintf(stderr, "%s: file_store is null\n", __func__);
		goto err;
	}

	/* set context and secure store callback */
	context = strdup(((file_store)));
	OQS_SIG_STFL_SECRET_KEY_SET_store_cb(secret_key, save_secret_key, (void *)context);

#if OQS_USE_PTHREADS_IN_TESTS
	OQS_SIG_STFL_SECRET_KEY_SET_mutex(secret_key, sk_lock);
#endif
	public_key = OQS_MEM_malloc(sig->length_public_key + 2 * sizeof(magic_t));
	message = OQS_MEM_malloc(message_len + 2 * sizeof(magic_t));
	signature = OQS_MEM_malloc(sig->length_signature + 2 * sizeof(magic_t));

	if ((public_key == NULL) || (secret_key == NULL) || (message == NULL) || (signature == NULL)) {
		fprintf(stderr, "ERROR: OQS_MEM_malloc failed\n");
		goto err;
	}

	//Set the magic numbers before
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

	rc = OQS_SIG_STFL_SECRET_KEY_serialize(&sk_buf, &sk_buf_len, secret_key);
	if (rc != OQS_SUCCESS) {
		goto err;
	}

	/* write key pair to disk */
	if (oqs_fstore("sk", file_store, sk_buf, sk_buf_len) != OQS_SUCCESS) {
		goto err;
	}

	if (oqs_fstore("pk", file_store, public_key, sig->length_public_key) != OQS_SUCCESS) {
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

	/* Read public key and re-test verify.*/
	read_pk_buf = OQS_MEM_malloc(sig->length_public_key);
	if (oqs_fload("pk", file_store, read_pk_buf, sig->length_public_key, &read_pk_len) != OQS_SUCCESS) {
		goto err;
	}
	rc = OQS_SIG_STFL_verify(sig, message, message_len, signature, signature_len, read_pk_buf);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: 2nd Verify with restored public key OQS_SIG_STFL_verify failed\n");
	}

	rc = test_sig_stfl_bitflip(sig, message, message_len, signature, signature_len, public_key, bitflips_all, bitflips);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		goto err;
	}

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

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	OQS_SIG_STFL_SECRET_KEY_free(secret_key);
	OQS_SIG_STFL_SECRET_KEY_free(secret_key_rd);
	if (public_key) {
		OQS_MEM_insecure_free(public_key - sizeof(magic_t));
	}
	if (message) {
		OQS_MEM_insecure_free(message - sizeof(magic_t));
	}
	if (signature) {
		OQS_MEM_insecure_free(signature - sizeof(magic_t));
	}
	OQS_MEM_secure_free(sk_buf, sk_buf_len);
	OQS_SIG_STFL_free(sig);

	OQS_MEM_insecure_free(read_pk_buf);
	OQS_MEM_insecure_free(context);
	OQS_MEM_insecure_free(file_store);

	return ret;
}

static OQS_STATUS sig_stfl_test_secret_key(const char *method_name, const char *katfile) {
	OQS_STATUS rc = OQS_SUCCESS;
	OQS_SIG_STFL_SECRET_KEY *sk = NULL;
	OQS_SIG_STFL_SECRET_KEY *sk_from_file = NULL;
	unsigned long long num_sig_left = 0, max_num_sigs = 0;
	OQS_SIG_STFL *sig_obj = NULL;
	uint8_t *public_key = NULL;
	uint8_t *from_file_sk_buf = NULL;
	uint8_t *to_file_sk_buf = NULL;
	size_t from_file_sk_len = 0;
	size_t to_file_sk_len = 0;
	char *context = NULL;
	char *context_2 = NULL;
	char *file_store_name = NULL;

	/*
	 * Temporarily skip algs with long key generation times.
	 */

	printf("================================================================================\n");
	printf("Create stateful Signature  %s\n", method_name);
	printf("================================================================================\n");

	sig_obj = OQS_SIG_STFL_new(method_name);
	if (sig_obj == NULL) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_new failed\n");
		goto err;
	}

	public_key = OQS_MEM_malloc(sig_obj->length_public_key * sizeof(uint8_t));

	printf("================================================================================\n");
	printf("Create stateful Secret Key  %s\n", method_name);
	printf("================================================================================\n");

	sk = OQS_SIG_STFL_SECRET_KEY_new(method_name);
	if (sk == NULL) {
		fprintf(stderr, "ERROR: OQS_SECRET_KEY_new failed\n");
		goto err;
	}

	printf("================================================================================\n");
	printf("Generate keypair  %s\n", method_name);
	printf("================================================================================\n");

	rc = sig_stfl_KATs_keygen(sig_obj, public_key, sk, katfile);

	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "OQS STFL key gen failed.\n");
		goto err;
	}

	/*
	 * Get max num signature and the amount remaining
	 */
	rc = OQS_SIG_STFL_sigs_total((const OQS_SIG_STFL *)sig_obj, &max_num_sigs, (const OQS_SIG_STFL_SECRET_KEY *)sk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "OQS STFL key: Failed to get max number of sig from %s.\n", method_name);
		goto err;
	}

	rc = OQS_SIG_STFL_sigs_remaining((const OQS_SIG_STFL *)sig_obj, &num_sig_left, (const OQS_SIG_STFL_SECRET_KEY *)sk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "OQS STFL key: Failed to get the remaining number of sig from %s.\n", method_name);
		goto err;
	}

	/* write sk key to disk */
	rc = OQS_SIG_STFL_SECRET_KEY_serialize(&to_file_sk_buf, &to_file_sk_len, sk);
	if (rc != OQS_SUCCESS) {
		goto err;
	}

	file_store_name  = convert_method_name_to_file_name(sig_obj->method_name);
	if (oqs_fstore("sk", file_store_name, to_file_sk_buf, to_file_sk_len) != OQS_SUCCESS) {
		goto err;
	}

	if (sk->secret_key_data == NULL) {
		fprintf(stderr, "ERROR: OQS_SECRET_KEY_new incomplete.\n");
		goto err;
	}

	/* set context and secure store callback */
	if (sk->set_scrt_key_store_cb != NULL) {
		context = strdup(file_store_name);
		sk->set_scrt_key_store_cb(sk, save_secret_key, (void *)context);
	}

	/* read secret key from disk */
	from_file_sk_buf = OQS_MEM_malloc(to_file_sk_len);
	if (oqs_fload("sk", file_store_name, from_file_sk_buf, to_file_sk_len, &from_file_sk_len) != OQS_SUCCESS) {
		goto err;
	}
	if (to_file_sk_len != from_file_sk_len) {
		fprintf(stderr, "ERROR:  OQS_SECRET_KEY_new stored length not equal read length\n");
		goto err;
	}

	sk_from_file = OQS_SIG_STFL_SECRET_KEY_new(method_name);
	if (sk_from_file == NULL) {
		fprintf(stderr, "ERROR: 2nd OQS_SECRET_KEY_new failed\n");
		goto err;
	}

	context_2 = strdup(file_store_name);
	rc = OQS_SIG_STFL_SECRET_KEY_deserialize(sk_from_file, from_file_sk_buf, from_file_sk_len, (void *)context_2);

	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "OQS restore %s from file failed.\n", method_name);
		goto err;
	}

	rc = OQS_SUCCESS;
	goto cleanup;

err:
	rc = OQS_ERROR;
cleanup:

	OQS_SIG_STFL_SECRET_KEY_free(sk);
	OQS_SIG_STFL_SECRET_KEY_free(sk_from_file);

	OQS_MEM_insecure_free(public_key);
	OQS_MEM_secure_free(to_file_sk_buf, to_file_sk_len);
	OQS_MEM_secure_free(from_file_sk_buf, from_file_sk_len);
	OQS_SIG_STFL_free(sig_obj);
	OQS_MEM_insecure_free(context);
	OQS_MEM_insecure_free(context_2);
	OQS_MEM_insecure_free(file_store_name);
	return rc;
}

#ifdef OQS_ENABLE_TEST_CONSTANT_TIME
static void TEST_SIG_STFL_randombytes(uint8_t *random_array, size_t bytes_to_read) {
	// We can't make direct calls to the system randombytes on some platforms,
	// so we have to swap out the OQS_randombytes provider.

	(void)OQS_randombytes_switch_algorithm("system");
	OQS_randombytes(random_array, bytes_to_read);
	OQS_randombytes_custom_algorithm(&TEST_SIG_STFL_randombytes);

	// OQS_TEST_CT_CLASSIFY tells Valgrind's memcheck tool to issue a warning if
	// the program branches on any byte that depends on random_array. This helps us
	// identify timing side-channels, as these bytes often contain secret data.
	OQS_TEST_CT_CLASSIFY(random_array, bytes_to_read);
}
#endif

#if OQS_USE_PTHREADS_IN_TESTS
static OQS_STATUS sig_stfl_test_query_key(const char *method_name) {
	OQS_STATUS rc = OQS_SUCCESS;
	size_t message_len_1 = sizeof(message_1);
	size_t message_len_2 = sizeof(message_2);

	/*
	 * Temporarily skip algs with long key generation times.
	 */

	printf("================================================================================\n");
	printf("Testing stateful Signature Verification %s\n", method_name);
	printf("================================================================================\n");

	if ( lock_test_sk == NULL || lock_test_sig_obj == NULL ||
	        signature_1 == NULL || signature_2 == NULL ||
	        lock_test_public_key == NULL) {
		return OQS_ERROR;
	}

	printf("================================================================================\n");
	printf("Sig Verify 1  %s\n", method_name);
	printf("================================================================================\n");

	rc = OQS_SIG_STFL_verify(lock_test_sig_obj, message_1, message_len_1, signature_1, signature_len_1, lock_test_public_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: lock thread test OQS_SIG_STFL_verify failed\n");
		goto err;
	}

	printf("================================================================================\n");
	printf("Sig Verify 2 %s\n", method_name);
	printf("================================================================================\n");

	rc = OQS_SIG_STFL_verify(lock_test_sig_obj, message_2, message_len_2, signature_2, signature_len_2, lock_test_public_key);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: lock thread test OQS_SIG_STFL_verify failed\n");
		goto err;
	}
	printf("================================================================================\n");
	printf("Stateful Signature Verification %s Passed.\n", method_name);
	printf("================================================================================\n");

	return OQS_SUCCESS;

err:
	return OQS_ERROR;
}

static OQS_STATUS sig_stfl_test_sig_gen(const char *method_name) {
	OQS_STATUS rc = OQS_SUCCESS;
	size_t message_len_1 = sizeof(message_1);
	size_t message_len_2 = sizeof(message_2);

	char *context = NULL;
	char *key_store_name = NULL;

	/*
	 * Temporarily skip algs with long key generation times.
	 */

	printf("================================================================================\n");
	printf("Testing stateful Signature Generation %s\n", method_name);
	printf("================================================================================\n");

	if ( lock_test_sk == NULL || lock_test_sig_obj == NULL) {
		return OQS_ERROR;
	}

	key_store_name = convert_method_name_to_file_name(method_name);
	/* set context and secure store callback */
	context = strdup(((key_store_name)));
	OQS_SIG_STFL_SECRET_KEY_SET_store_cb(lock_test_sk, save_secret_key, (void *)context);

	/*
	 * Get max num signature and the amount remaining
	 */
	unsigned long long num_sig_left = 0, max_num_sigs = 0;
	rc = OQS_SIG_STFL_sigs_total((const OQS_SIG_STFL *)lock_test_sig_obj, &max_num_sigs, (const OQS_SIG_STFL_SECRET_KEY *)lock_test_sk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "OQS STFL key: Failed to get max number of sig from %s.\n", method_name);
		goto err;
	}

	rc = OQS_SIG_STFL_sigs_remaining((const OQS_SIG_STFL *)lock_test_sig_obj, &num_sig_left, (const OQS_SIG_STFL_SECRET_KEY *)lock_test_sk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "OQS STFL key: Failed to get the remaining number of sig from %s.\n", method_name);
		goto err;
	}

	printf("================================================================================\n");
	printf("Sig Gen 1  %s\n", method_name);
	printf("================================================================================\n");

	signature_1 = OQS_MEM_malloc(lock_test_sig_obj->length_signature);

	rc = OQS_SIG_STFL_sign(lock_test_sig_obj, signature_1, &signature_len_1, message_1, message_len_1, lock_test_sk);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: lock thread test OQS_SIG_STFL_sign failed\n");
		goto err;
	}

	/*
	 * Get max num signature and the amount remaining
	 */
	num_sig_left = 0, max_num_sigs = 0;
	rc = OQS_SIG_STFL_sigs_total((const OQS_SIG_STFL *)lock_test_sig_obj, &max_num_sigs, (const OQS_SIG_STFL_SECRET_KEY *)lock_test_sk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "OQS STFL key: Failed to get max number of sig from %s.\n", method_name);
		goto err;
	}

	rc = OQS_SIG_STFL_sigs_remaining((const OQS_SIG_STFL *)lock_test_sig_obj, &num_sig_left, (const OQS_SIG_STFL_SECRET_KEY *)lock_test_sk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "OQS STFL key: Failed to get the remaining number of sig from %s.\n", method_name);
		goto err;
	}

	printf("================================================================================\n");
	printf("Sig Gen 2 %s\n", method_name);
	printf("================================================================================\n");

	signature_2 = OQS_MEM_malloc(lock_test_sig_obj->length_signature);

	rc = OQS_SIG_STFL_sign(lock_test_sig_obj, signature_2, &signature_len_2, message_2, message_len_2, lock_test_sk);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: lock thread test OQS_SIG_STFL_sign failed\n");
		goto err;
	}

	printf("================================================================================\n");
	printf("Stateful Key Gen %s Passed.\n", method_name);
	printf("================================================================================\n");

	/*
	 * Get max num signature and the amount remaining
	 */
	num_sig_left = 0, max_num_sigs = 0;
	rc = OQS_SIG_STFL_sigs_total((const OQS_SIG_STFL *)lock_test_sig_obj, &max_num_sigs, (const OQS_SIG_STFL_SECRET_KEY *)lock_test_sk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "OQS STFL key: Failed to get max number of sig from %s.\n", method_name);
		goto err;
	}

	rc = OQS_SIG_STFL_sigs_remaining((const OQS_SIG_STFL *)lock_test_sig_obj, &num_sig_left, (const OQS_SIG_STFL_SECRET_KEY *)lock_test_sk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "OQS STFL key: Failed to get the remaining number of sig from %s.\n", method_name);
		goto err;
	}

	rc = OQS_SUCCESS;
	goto cleanup;
err:
	rc = OQS_ERROR;
cleanup:
	OQS_MEM_insecure_free(context);
	OQS_MEM_insecure_free(key_store_name);

	return rc;
}

static OQS_STATUS sig_stfl_test_secret_key_lock(const char *method_name, const char *katfile) {
	OQS_STATUS rc = OQS_SUCCESS;

	printf("================================================================================\n");
	printf("Testing stateful Signature locks %s\n", method_name);
	printf("================================================================================\n");

	/*
	 * Temporarily skip algs with long key generation times.
	 */

	printf("================================================================================\n");
	printf("Create stateful Signature  %s\n", method_name);
	printf("================================================================================\n");

	lock_test_sig_obj = OQS_SIG_STFL_new(method_name);
	if (lock_test_sig_obj == NULL) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_new failed\n");
		goto err;
	}

	lock_test_public_key = OQS_MEM_malloc(lock_test_sig_obj->length_public_key * sizeof(uint8_t));

	printf("================================================================================\n");
	printf("Create stateful Secret Key  %s\n", method_name);
	printf("================================================================================\n");

	lock_test_sk = OQS_SIG_STFL_SECRET_KEY_new(method_name);
	if (lock_test_sk == NULL) {
		fprintf(stderr, "ERROR: OQS_SECRET_KEY_new failed\n");
		goto err;
	}

	OQS_SIG_STFL_SECRET_KEY_SET_lock(lock_test_sk, lock_sk_key);
	OQS_SIG_STFL_SECRET_KEY_SET_unlock(lock_test_sk, unlock_sk_key);

	OQS_SIG_STFL_SECRET_KEY_SET_mutex(lock_test_sk, test_sk_lock);

	printf("================================================================================\n");
	printf("Generate keypair  %s\n", method_name);
	printf("================================================================================\n");

	rc = sig_stfl_KATs_keygen(lock_test_sig_obj, lock_test_public_key, lock_test_sk, katfile);

	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "OQS STFL key gen failed.\n");
		goto err;
	}

	if (lock_test_sk->secret_key_data == NULL) {
		fprintf(stderr, "ERROR: OQS_SECRET_KEY_new incomplete.\n");
		goto err;
	}

	/* set context and secure store callback */
	if (lock_test_sk->set_scrt_key_store_cb) {
		lock_test_context = convert_method_name_to_file_name(method_name);
		lock_test_sk->set_scrt_key_store_cb(lock_test_sk, save_secret_key, (void *)lock_test_context);
	}

	return OQS_SUCCESS;

err:
	return OQS_ERROR;
}


typedef struct thread_data {
	const char *alg_name;
	const char *katfile;
	bool *bitflips_all;
	size_t *bitflips;
	OQS_STATUS rc;
	// OQS_STATUS rc1;
} thread_data_t;

typedef struct lock_test_data {
	const char *alg_name;
	const char *katfile;
	OQS_STATUS rc;
} lock_test_data_t;

void *test_query_key(void *arg) {
	struct lock_test_data *td = arg;
	printf("\n%s: Start Query Stateful Key info\n", __func__);
	td->rc = sig_stfl_test_query_key(td->alg_name);
	OQS_thread_stop();
	printf("%s: End Query Stateful Key info\n\n", __func__);
	return NULL;
}

void *test_sig_gen(void *arg) {
	struct lock_test_data *td = arg;
	printf("\n%s: Start Generate Stateful Signature\n", __func__);
	td->rc = sig_stfl_test_sig_gen(td->alg_name);
	OQS_thread_stop();
	printf("%s: End Generate Stateful Signature\n\n", __func__);
	return NULL;
}

void *test_create_keys(void *arg) {
	struct lock_test_data *td = arg;
	printf("\n%s: Start Generate Keys\n", __func__);
	td->rc = sig_stfl_test_secret_key_lock(td->alg_name, td->katfile);
	OQS_thread_stop();
	printf("%s: End Generate Stateful Keys\n\n", __func__);
	return NULL;
}

void *test_correctness_wrapper(void *arg) {
	struct thread_data *td = arg;
	td->rc = sig_stfl_test_correctness(td->alg_name, td->katfile, td->bitflips_all, td->bitflips);
	OQS_thread_stop();
	return NULL;
}

void *test_secret_key_wrapper(void *arg) {
	struct thread_data *td = arg;
	td->rc = sig_stfl_test_secret_key(td->alg_name, td->katfile);
	OQS_thread_stop();
	return NULL;
}
#endif

/*
 * When key and signature generation is off
 * these operations should fail. So flip the results.
 */
static OQS_STATUS update_test_result( OQS_STATUS rc, int xmss_or_lms) {
	OQS_STATUS rc_update = rc;
	if (xmss_or_lms) {
		;
#ifndef OQS_ALLOW_XMSS_KEY_AND_SIG_GEN
		if (rc_update == OQS_ERROR) {
			rc_update = OQS_SUCCESS;
		} else {
			rc_update = OQS_ERROR;
		}
#endif
	} else {
		;
#ifndef OQS_ALLOW_LMS_KEY_AND_SIG_GEN
		if (rc_update == OQS_ERROR) {
			rc_update = OQS_SUCCESS;
		} else {
			rc_update = OQS_ERROR;
		}
#endif
	}
	return rc_update;
}

int main(int argc, char **argv) {
	OQS_STATUS  rc = OQS_ERROR, rc1 = OQS_ERROR;
	OQS_init();
	rc = oqs_fstore_init();
	if (rc != OQS_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}

	printf("Testing stateful signature algorithms using liboqs version %s\n", OQS_version());

	if (argc < 2 || argc > 5) {
		fprintf(stderr, "Usage: test_sig_stfl algname [katfile] [bitflips_msg] [bitflips_sig]\n");
		fprintf(stderr, "  algname: ");
		for (size_t i = 0; i < OQS_SIG_STFL_algs_length; i++) {
			if (i > 0) {
				fprintf(stderr, ", ");
			}
			fprintf(stderr, "%s", OQS_SIG_STFL_alg_identifier(i));
		}
		fprintf(stderr, "\n");
		fprintf(stderr, " bitflips_msg: the number of random bitflips to perform for each EUF-CMA signature (\"all\" to flip every bit)\n");
		fprintf(stderr, " bitflips_sig: the number of random bitflips to perform for each SUF-CMA signature (\"all\" to flip every bit)\n");
		OQS_destroy();
		return EXIT_FAILURE;
	}

	print_system_info();

	const char *alg_name = argv[1];
	const char *katfile = argv[2];
	int is_xmss = 0;
	if (strstr(alg_name, "XMSS") != NULL) {
		is_xmss = 1;
		if (argc < 3) {
			fprintf(stderr, "KAT file must be provided for XMSS.\n");
			OQS_destroy();
			return EXIT_FAILURE;
		}
	}

	/* by default, flip 50 random bits of the message and signature (to test EUF-CMA and SUF-CMA, respectively) */
	bool bitflips_all[2] = {false, false};
	size_t bitflips[2] = {50, 50};
	if (argc >= 4) {
		if (strcmp(argv[2], "all") == 0) {
			bitflips_all[0] = true;
		} else {
			bitflips[0] = (size_t)strtol(argv[3], NULL, 10);
		}
	}
	if (argc == 5) {
		if (strcmp(argv[3], "all") == 0) {
			bitflips_all[1] = true;
		} else {
			bitflips[1] = (size_t)strtol(argv[4], NULL, 10);
		}
	}

	/*
	 * Tests executed by CI/DI only run algoritms that have been enabled.
	 *
	 */
	if (!OQS_SIG_STFL_alg_is_enabled(alg_name)) {
		printf("Stateful signature algorithm %s not enabled!\n", alg_name);
		OQS_destroy();
		if (is_xmss) {
#ifndef OQS_ENABLE_SIG_STFL_XMSS
			return EXIT_SUCCESS;
#else
			return EXIT_FAILURE;
#endif
		} else {
#ifndef OQS_ENABLE_SIG_STFL_LMS
			return EXIT_SUCCESS;
#else
			return EXIT_FAILURE;
#endif
		}
	}

#ifdef OQS_ENABLE_TEST_CONSTANT_TIME
	OQS_randombytes_custom_algorithm(&TEST_SIG_STFL_randombytes);
#else
	rc = OQS_randombytes_switch_algorithm("system");
	if (rc != OQS_SUCCESS) {
		printf("Could not generate random data with system RNG\n");
		OQS_destroy();
		return EXIT_FAILURE;
	}
#endif

	int exit_status = EXIT_SUCCESS;

#if OQS_USE_PTHREADS_IN_TESTS
#define MAX_LEN_SIG_NAME_ 64
	OQS_STATUS rc_create = OQS_ERROR, rc_sign = OQS_ERROR, rc_query = OQS_ERROR;

	pthread_t thread;
	pthread_t create_key_thread;
	pthread_t sign_key_thread;
	pthread_t query_key_thread;

	thread_data_t td = {.alg_name = alg_name, .katfile = katfile, .bitflips_all = bitflips_all, .bitflips = bitflips, .rc = OQS_ERROR};
	thread_data_t td_2 = {.alg_name = alg_name, .katfile = katfile, .bitflips_all = bitflips_all, .bitflips = bitflips, .rc = OQS_ERROR};

	lock_test_data_t td_create = {.alg_name = alg_name, .katfile = katfile, .rc = OQS_ERROR};
	lock_test_data_t td_sign = {.alg_name = alg_name, .katfile = katfile, .rc = OQS_ERROR};
	lock_test_data_t td_query = {.alg_name = alg_name, .katfile = katfile, .rc = OQS_ERROR};

	test_sk_lock = (pthread_mutex_t *)OQS_MEM_malloc(sizeof(pthread_mutex_t));
	if (test_sk_lock == NULL) {
		goto err;
	}
	sk_lock = (pthread_mutex_t *)OQS_MEM_malloc(sizeof(pthread_mutex_t));
	if (sk_lock == NULL) {
		goto err;
	}

	if (pthread_mutex_init(test_sk_lock, NULL) || pthread_mutex_init(sk_lock, NULL)) {
		fprintf(stderr, "ERROR: Initializing mutex\n");
		exit_status = EXIT_FAILURE;
		goto err;
	}

	if (pthread_create(&thread, NULL, test_correctness_wrapper, &td)) {
		fprintf(stderr, "ERROR: Creating pthread for test_wrapper\n");
		exit_status = EXIT_FAILURE;
		goto err;
	}
	pthread_join(thread, NULL);
	rc = td.rc;
	rc = update_test_result(rc, is_xmss);

	if (pthread_create(&thread, NULL, test_secret_key_wrapper, &td_2)) {
		fprintf(stderr, "ERROR: Creating pthread for test_wrapper_2\n");
		exit_status = EXIT_FAILURE;
		goto err;
	}
	pthread_join(thread, NULL);
	rc1 = td_2.rc;
	rc1 = update_test_result(rc1, is_xmss);

	if (pthread_create(&create_key_thread, NULL, test_create_keys, &td_create)) {
		fprintf(stderr, "ERROR: Creating pthread for test_create_keys\n");
		exit_status = EXIT_FAILURE;
		goto err;
	}
	pthread_join(create_key_thread, NULL);
	rc_create = td_create.rc;
	rc_create = update_test_result(rc_create, is_xmss);

	if (pthread_create(&sign_key_thread, NULL, test_sig_gen, &td_sign)) {
		fprintf(stderr, "ERROR: Creating pthread for test_sig_gen\n");
		exit_status = EXIT_FAILURE;
		goto err;
	}
	pthread_join(sign_key_thread, NULL);
	rc_sign = td_sign.rc;
	rc_sign = update_test_result(rc_sign, is_xmss);

	if (pthread_create(&query_key_thread, NULL, test_query_key, &td_query)) {
		fprintf(stderr, "ERROR: Creating pthread for test_query_key\n");
		exit_status = EXIT_FAILURE;
		goto err;
	}
	pthread_join(query_key_thread, NULL);
	rc_query = td_query.rc;
	rc_query = update_test_result(rc_query, is_xmss);

err:
	if (test_sk_lock) {
		pthread_mutex_destroy(test_sk_lock);
	}
	if (sk_lock) {
		pthread_mutex_destroy(sk_lock);
	}

	OQS_SIG_STFL_SECRET_KEY_free(lock_test_sk);
	OQS_MEM_insecure_free(lock_test_public_key);
	OQS_SIG_STFL_free(lock_test_sig_obj);
	OQS_MEM_insecure_free(lock_test_context);
	OQS_MEM_insecure_free(signature_1);
	OQS_MEM_insecure_free(signature_2);

	OQS_destroy();
	if (rc != OQS_SUCCESS || rc1 != OQS_SUCCESS) {
		return EXIT_FAILURE;
	}

#if OQS_USE_PTHREADS_IN_TESTS
	if (rc_create != OQS_SUCCESS || rc_sign != OQS_SUCCESS || rc_query != OQS_SUCCESS) {
		return EXIT_FAILURE;
	}
#endif
	return exit_status;
#else
	rc = sig_stfl_test_correctness(alg_name, katfile, bitflips_all, bitflips);
	rc1 = sig_stfl_test_secret_key(alg_name, katfile);

	OQS_destroy();
	rc = update_test_result(rc, is_xmss);
	rc1 = update_test_result(rc1, is_xmss);


	if (rc != OQS_SUCCESS || rc1 != OQS_SUCCESS) {
		return EXIT_FAILURE;
	}
	return exit_status;
#endif
}
