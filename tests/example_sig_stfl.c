/*
 * example_sig_stfl.c
 *
 * Minimal example of using a post-quantum stateful signature implemented in liboqs.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

#define MESSAGE_LEN 50

static OQS_STATUS do_nothing_save(uint8_t *key_buf, size_t buf_len, void *context) {
	(void)(context);
	(void)(buf_len);
	return key_buf != NULL ? OQS_SUCCESS : OQS_ERROR;
}

/* This function gives an example of the signing operations,
 * allocating variables dynamically on the heap and calling the
 * OQS_SIG_STFL and OQS_SIG_STFL_SECRET_KEY objects.
 *
 * This does not require the use of compile-time macros to check if the
 * algorithm in question was enabled at compile-time; instead, the caller
 * must check that the OQS_SIG object returned is not NULL.
 */
static OQS_STATUS stfl_example(char *method_name) {

	OQS_SIG_STFL *sig = NULL;
	uint8_t *public_key = NULL;
	OQS_SIG_STFL_SECRET_KEY *secret_key = NULL;
	uint8_t *message = NULL;
	uint8_t *signature = NULL;
	size_t message_len = MESSAGE_LEN;
	size_t signature_len;
	char *sk_fname = NULL;
	OQS_STATUS rc = OQS_ERROR;

	/*
	 * Steps
	 * 1. create stateful signature object
	 * 2. create secret key object
	 * 3. set key storage callback function
	 *    set mutex if necessary
	 * 4. Generate key-pair
	 * 5. Signature generation
	 * 6. verify signature
	 */
	sig = OQS_SIG_STFL_new(method_name);
	if (sig == NULL) {
		printf("[Stateful sig]  %s new failed.\n", method_name);
		return OQS_ERROR;
	}

	secret_key = OQS_SIG_STFL_SECRET_KEY_new(method_name);
	if (secret_key == NULL) {
		printf("[Stateful secret key] %s new failed.\n", method_name);
		goto err;
	}

	/*
	 * Allocate storage for public key, secret key filename, message and signature
	 */
	public_key = OQS_MEM_malloc(sig->length_public_key);
	sk_fname = OQS_MEM_malloc(strlen(method_name) + strlen(".sk") + 1);
	message = OQS_MEM_malloc(message_len);
	signature = OQS_MEM_malloc(sig->length_signature);
	if ((public_key == NULL) || (message == NULL) || (signature == NULL) || (sk_fname == NULL)) {
		fprintf(stderr, "ERROR: OQS_MEM_malloc failed!\n");
		goto err;
	}

	strcpy(sk_fname, method_name);
	strcat(sk_fname, ".sk");
	/*
	 * set callback to securely store the secret key
	 * secret keys are one time use only. So after a signature gen
	 * the secret key most be advanced to the next
	 */
	OQS_SIG_STFL_SECRET_KEY_SET_store_cb(secret_key, do_nothing_save, (void *)sk_fname);

	/*
	 * Generate key pair
	 */
	rc = OQS_SIG_STFL_keypair(sig, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		printf("[Stateful key pair generation] %s new failed.\n", method_name);
		goto err;
	}

	// let's create a random test message to sign
	OQS_randombytes(message, message_len);

	rc = OQS_SIG_STFL_sign(sig, signature, &signature_len, message, message_len, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_sign failed %s!\n", method_name);
		goto err;
	}
	rc = OQS_SIG_STFL_verify(sig, message, message_len, signature, signature_len, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_verify failed %s!\n", method_name);
		goto err;
	}

	printf("[Stateful signature]  %s operations completed.\n", method_name);
err:
//cleanup
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(sk_fname);
	OQS_MEM_secure_free(message, message_len);
	OQS_MEM_insecure_free(signature);
	OQS_SIG_STFL_free(sig);
	OQS_SIG_STFL_SECRET_KEY_free(secret_key);

	return rc;
}

int main(void) {
#ifndef OQS_ALLOW_STFL_KEY_AND_SIG_GEN
	OQS_init();
	printf("Stateful signature algorithms key and signature generation is not enabled.\n");
	if (stfl_example((char *)"XMSS-SHA2_10_256") == OQS_ERROR && stfl_example((char *)"LMS_SHA256_H10_W4") == OQS_ERROR) {
		OQS_destroy();
		return EXIT_SUCCESS;
	} else {
		OQS_destroy();
		return EXIT_FAILURE;
	}
#else
	OQS_STATUS lms_status;
	OQS_STATUS xmss_status;
	OQS_init();
	xmss_status = stfl_example((char *)"XMSS-SHA2_10_256");
	lms_status = stfl_example((char *)"LMS_SHA256_H10_W4");
	OQS_destroy();

#ifndef OQS_ALLOW_XMSS_KEY_AND_SIG_GEN
	if (xmss_status == OQS_ERROR) {
		xmss_status = OQS_SUCCESS;
	} else {
		xmss_status = OQS_ERROR;
	}
#endif
#ifndef OQS_ALLOW_LMS_KEY_AND_SIG_GEN
	if (lms_status == OQS_ERROR) {
		lms_status = OQS_SUCCESS;
	} else {
		lms_status = OQS_ERROR;
	}
#endif
	if ((xmss_status == OQS_SUCCESS) && (lms_status == OQS_SUCCESS)) {
		return EXIT_SUCCESS;
	} else {
		return EXIT_FAILURE;
	}
#endif
}


