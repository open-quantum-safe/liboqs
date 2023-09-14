// SPDX-License-Identifier: MIT

#if defined(_WIN32)
#pragma warning(disable : 4244 4293)
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>
#include "tmp_store.c"

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

/*
 * Write stateful secret keys to disk.
 */
static OQS_STATUS test_save_secret_key(uint8_t *key_buf, size_t buf_len, void *context) {
	uint8_t *kb = key_buf;
	fprintf(stderr, "\n%s test saved STFL SK  <%s>.\n", __FUNCTION__, (const char *)context);

	if (key_buf && context && buf_len != 0) {
		if (oqs_fstore("sk", (const char *)context, kb, buf_len) == OQS_SUCCESS) {
			fprintf(stderr, "\nUpdated saved STFL SK  <%s>.\n", (const char *)context);
			return OQS_SUCCESS;
		} else {
			return OQS_ERROR;
		}
	}
	return OQS_ERROR;
}

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
	int ch, started;
	unsigned long i;
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

OQS_STATUS sig_stfl_keypair_from_keygen(OQS_SIG_STFL *sig, uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key) {
	OQS_STATUS rc;

	if ((sig == NULL) || (public_key == NULL) || (secret_key == NULL)) {
		return OQS_ERROR;
	}

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
	if (!ReadHex(fp_rsp, public_key, sig->length_public_key, "pk = ")) {
		fprintf(stderr, "ERROR: unable to read 'pk' from <%s>\n", katfile);
		goto err;
	}

	if (!ReadHex(fp_rsp, secret_key->secret_key_data, sig->length_secret_key, "sk = ")) {
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
OQS_STATUS sig_stfl_KATs_keygen(OQS_SIG_STFL *sig, uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key, const char *katfile) {

	printf("%s ", sig->method_name);
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
	OQS_SIG_STFL_SECRET_KEY *secret_key = NULL;
	const OQS_SIG_STFL_SECRET_KEY *sk = NULL;
	OQS_SIG_STFL_SECRET_KEY *secret_key_rd = NULL;
	uint8_t *message = NULL;
	size_t message_len = 100;
	uint8_t *signature = NULL;
	size_t signature_len;

	uint8_t *sk_buf = NULL;
	uint8_t *read_pk_buf = NULL;
	char *context = NULL;
	const char *file_store = NULL;
	size_t sk_buf_len = 0;
	size_t read_pk_len = 0;

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

	secret_key = OQS_SIG_STFL_SECRET_KEY_new(sig->method_name);
	secret_key_rd = OQS_SIG_STFL_SECRET_KEY_new(sig->method_name);
	public_key = malloc(sig->length_public_key + 2 * sizeof(magic_t));
	message = malloc(message_len + 2 * sizeof(magic_t));
	signature = malloc(sig->length_signature + 2 * sizeof(magic_t));

	if ((public_key == NULL) || (secret_key == NULL) || (message == NULL) || (signature == NULL)) {
		fprintf(stderr, "ERROR: malloc failed\n");
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
	sk = secret_key;
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_keypair failed\n");
		goto err;
	}
	/*
	 #define OQS_SIG_STFL_alg_xmssmt_sha256_h20_2 "XMSSMT-SHA2_20/2_256"
	 #define OQS_SIG_STFL_alg_xmssmt_sha256_h20_4 "XMSSMT-SHA2_20/4_256"
	#define OQS_SIG_STFL_alg_xmssmt_sha256_h40_2 "XMSSMT-SHA2_40/2_256"
	#define OQS_SIG_STFL_alg_xmssmt_sha256_h40_4 "XMSSMT-SHA2_40/4_256"
	#define OQS_SIG_STFL_alg_xmssmt_sha256_h40_8 "XMSSMT-SHA2_40/8_256"
	#define OQS_SIG_STFL_alg_xmssmt_sha256_h60_3 "XMSSMT-SHA2_60/3_256"
	#define OQS_SIG_STFL_alg_xmssmt_sha256_h60_6 "XMSSMT-SHA2_60/6_256"
	#define OQS_SIG_STFL_alg_xmssmt_sha256_h60_12 "XMSSMT-SHA2_60/12_256"
	#define OQS_SIG_STFL_alg_xmssmt_shake128_h20_2 "XMSSMT-SHAKE_20/2_256"
	#define OQS_SIG_STFL_alg_xmssmt_shake128_h20_4 "XMSSMT-SHAKE_20/4_256"
	#define OQS_SIG_STFL_alg_xmssmt_shake128_h40_2 "XMSSMT-SHAKE_40/2_256"
	#define OQS_SIG_STFL_alg_xmssmt_shake128_h40_4 "XMSSMT-SHAKE_40/4_256"
	#define OQS_SIG_STFL_alg_xmssmt_shake128_h40_8 "XMSSMT-SHAKE_40/8_256"
	#define OQS_SIG_STFL_alg_xmssmt_shake128_h60_3 "XMSSMT-SHAKE_60/3_256"

	#define OQS_SIG_STFL_alg_xmssmt_shake128_h60_6 "XMSSMT-SHAKE_60/6_256"
	#define OQS_SIG_STFL_alg_xmssmt_shake128_h60_12 "XMSSMT-SHAKE_60/12_256"
	 */

	rc = OQS_SECRET_KEY_STFL_serialize_key(sk, &sk_buf_len, &sk_buf);

	if (strcmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h20_2) == 0) {
		file_store = "XMSSMT-SHA2_20-2_256";
	} else if (strcmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h20_4) == 0) {
		file_store = "XMSSMT-SHA2_20-4_256";
	} else if (strcmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_2) == 0) {
		file_store = "XMSSMT-SHA2_40-2_256";
	} else if (strcmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_4) == 0) {
		file_store = "XMSSMT-SHA2_40-4_256";
	} else if (strcmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_8) == 0) {
		file_store = "XMSSMT-SHA2_40-8_256";
	} else if (strcmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_3) == 0) {
		file_store = "XMSSMT-SHA2_60-3_256";
	} else if (strcmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_6) == 0) {
		file_store = "XMSSMT-SHA2_60-6_256";
	} else if (strcmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_12) == 0) {
		file_store = "XMSSMT-SHA2_60-12_256";
	} else if (strcmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h20_2) == 0) {
		file_store = "XMSSMT-SHAKE_20-2_256";
	} else if (strcmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h20_4) == 0) {
		file_store = "XMSSMT-SHAKE_20-4_256";
	} else if (strcmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_2) == 0) {
		file_store = "XMSSMT-SHAKE_40-2_256";
	} else if (strcmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_4) == 0) {
		file_store = "XMSSMT-SHAKE_40-4_256";
	} else if (strcmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_8) == 0) {
		file_store = "XMSSMT-SHAKE_40-8_256";
	} else if (strcmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_3) == 0) {
		file_store = "XMSSMT-SHAKE_60-3_256";
	} else if (strcmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_6) == 0) {
		file_store = "XMSSMT-SHAKE_60-6_256";
	} else if (strcmp(sig->method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_12) == 0) {
		file_store = "XMSSMT-SHAKE_60-12_256";
	} else {
		file_store = sig->method_name;
	}

	/* write key pair to disk */
	if (oqs_fstore("sk", file_store, sk_buf, sk_buf_len) != OQS_SUCCESS) {
		goto err;
	}

	if (oqs_fstore("pk", file_store, public_key, sig->length_public_key) != OQS_SUCCESS) {
		goto err;
	}

	/* set context and secure store callback */
	context = strdup(((file_store)));
	OQS_SIG_STFL_SECRET_KEY_SET_store_cb(secret_key, test_save_secret_key, (void *)context);

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
	read_pk_buf = malloc(sig->length_public_key);
	if (oqs_fload("pk", file_store, read_pk_buf, sig->length_public_key, &read_pk_len) != OQS_SUCCESS) {
		goto err;
	}
	rc = OQS_SIG_STFL_verify(sig, message, message_len, signature, signature_len, read_pk_buf);
	OQS_TEST_CT_DECLASSIFY(&rc, sizeof rc);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: 2nd Verify with restored public key OQS_SIG_STFL_verify failed\n");
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
	OQS_SIG_STFL_free(sig);

	OQS_MEM_insecure_free(read_pk_buf);
	return ret;
}

static OQS_STATUS sig_stfl_test_secret_key(const char *method_name) {
	OQS_STATUS rc = OQS_SUCCESS;
	OQS_SIG_STFL_SECRET_KEY *sk = NULL;
	OQS_SIG_STFL_SECRET_KEY *sk_frm_file = NULL;

	OQS_SIG_STFL *sig_obj = NULL;
	uint8_t *public_key = NULL;
	uint8_t *frm_file_sk_buf = NULL;
	uint8_t *to_file_sk_buf = NULL;
	size_t frm_file_sk_len = 0;
	size_t to_file_sk_len = 0;
	char *context = NULL;
	char *context_2 = NULL;

	/*
	 * Temporarily skip algs with long key generation times.
	 */

	if (strcmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h5_w1) != 0) {
		goto skip_test;
	} else {
		goto keep_going;
	}

//	if (0) {
//
//#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h16
//	} else if (strcmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h16) == 0) {
//		goto skip_test;
//#endif
//#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h20
//	} else if (strcmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h20) == 0) {
//		goto skip_test;
//#endif
//
//#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h16
//	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h16)) {
//		goto skip_test;
//#endif
//#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h20
//	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h20)) {
//		goto skip_test;
//#endif
//
//#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h16
//	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h16)) {
//		goto skip_test;
//#endif
//#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h20
//	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h20)) {
//		goto skip_test;
//#endif
//
//#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16
//	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h16)) {
//		goto skip_test;
//#endif
//#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20
//	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h20)) {
//		goto skip_test;
//#endif
//
//#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_2
//	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_2)) {
//		goto skip_test;
//#endif
//#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_3
//	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_3)) {
//		goto skip_test;
//#endif
//
//#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_2
//	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_2)) {
//		goto skip_test;
//#endif
//#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_3
//	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_3)) {
//		goto skip_test;
//#endif
//	} else {
//		goto keep_going;
//	}

skip_test:
	printf("Skip slow test %s.\n", method_name);
	return rc;

keep_going:

	printf("================================================================================\n");
	printf("Create stateful Signature  %s\n", method_name);
	printf("================================================================================\n");

	sig_obj = OQS_SIG_STFL_new(method_name);
	if (sig_obj == NULL) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_new failed\n");
		goto err;
	}

	public_key = malloc(sig_obj->length_public_key * sizeof(uint8_t));

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

	rc = OQS_SIG_STFL_keypair(sig_obj, public_key, sk);

	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "OQS STFL key gen failed.\n");
		goto err;
	}

	/* write sk key to disk */
	rc = OQS_SECRET_KEY_STFL_serialize_key(sk, &to_file_sk_len, &to_file_sk_buf);
	if (oqs_fstore("sk", sig_obj->method_name, to_file_sk_buf, to_file_sk_len) != OQS_SUCCESS) {
		goto err;
	}

	if (!sk->secret_key_data) {
		fprintf(stderr, "ERROR: OQS_SECRET_KEY_new incomplete.\n");
		OQS_MEM_insecure_free(public_key);
		goto err;
	}

	/* set context and secure store callback */
	if (sk->set_scrt_key_store_cb) {
		context = strdup(((method_name)));
		sk->set_scrt_key_store_cb(sk, test_save_secret_key, (void *)context);
	}


	/* read secret key from disk */
	frm_file_sk_buf = malloc(to_file_sk_len);
	if (oqs_fload("sk", method_name, frm_file_sk_buf, to_file_sk_len, &frm_file_sk_len) != OQS_SUCCESS) {
		goto err;
	}
	if (to_file_sk_len != frm_file_sk_len) {
		fprintf(stderr, "ERROR:  OQS_SECRET_KEY_new stored length not equal read length\n");
		goto err;
	}

	sk_frm_file = OQS_SIG_STFL_SECRET_KEY_new(method_name);
	if (sk_frm_file == NULL) {
		fprintf(stderr, "ERROR: 2nd OQS_SECRET_KEY_new failed\n");
		goto err;
	}

	context_2 = strdup(((method_name)));
	rc = OQS_SECRET_KEY_STFL_deserialize_key(sk_frm_file, frm_file_sk_len, frm_file_sk_buf, (void *)context_2);

	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "OQS restore %s from file failed.\n", method_name);
		goto err;
	}

	printf("Secret Key created as expected.\n");
	goto end_it;

err:
	rc = OQS_ERROR;
end_it:

	OQS_SIG_STFL_SECRET_KEY_free(sk);
	OQS_SIG_STFL_SECRET_KEY_free(sk_frm_file);

	OQS_MEM_insecure_free(public_key);
	OQS_MEM_secure_free(to_file_sk_buf, to_file_sk_len);
	OQS_MEM_secure_free(frm_file_sk_buf, frm_file_sk_len);
	OQS_SIG_STFL_free(sig_obj);
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
	oqs_fstore_init();

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
