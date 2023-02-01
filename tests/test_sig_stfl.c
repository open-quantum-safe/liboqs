// SPDX-License-Identifier: MIT

#if defined(_WIN32)
#pragma warning(disable : 4244 4293)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

typedef struct magic_s {
	uint8_t val[31];
} magic_t;

OQS_STATUS lock_sk_key(OQS_SECRET_KEY *sk) {
	return sk != NULL ? OQS_SUCCESS : OQS_ERROR;
}

OQS_STATUS release_sk_key(OQS_SECRET_KEY *sk) {
	return sk != NULL ? OQS_SUCCESS : OQS_ERROR;
}

OQS_STATUS do_nothing_save(const OQS_SECRET_KEY *sk) {
	return sk != NULL ? OQS_SUCCESS : OQS_ERROR;
}

static OQS_STATUS sig_stfl_test_correctness(const char *method_name) {

	OQS_SIG_STFL *sig = NULL;
	uint8_t *public_key = NULL;
	OQS_SECRET_KEY *secret_key = NULL;
	uint8_t *message = NULL;
	size_t message_len = 100;
	uint8_t *signature = NULL;
	size_t signature_len;
	OQS_STATUS rc = OQS_SUCCESS, ret = OQS_ERROR;

	//The magic numbers are random values.
	//The length of the magic number was chosen to be 31 to break alignment
	magic_t magic;
	OQS_randombytes(magic.val, sizeof(magic_t));

	sig = OQS_SIG_STFL_new(method_name);
	if (sig == NULL) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_new failed\n");
		goto err;
	}

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
    secret_key->save_secret_key = do_nothing_save;

	if (secret_key == NULL) {
		fprintf(stderr, "ERROR: OQS_SECRET_KEY_new failed\n");
		goto err;
	}

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

    rc = OQS_SIG_STFL_keypair(sig, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		goto err;
	}

	rc = OQS_SIG_STFL_sign(sig, signature, &signature_len, message, message_len, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_sign failed\n");
		goto err;
	}

	if (public_key == NULL) {
		fprintf(stderr, "ERROR: public_key is NULL\n");
		goto err;
	}

	rc = OQS_SIG_STFL_verify(sig, message, message_len, signature, signature_len, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_verify failed\n");
		goto err;
	}

	OQS_randombytes(signature, signature_len);
	rc = OQS_SIG_STFL_verify(sig, message, message_len, signature, signature_len, public_key);
	if (rc != OQS_ERROR) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_verify should have failed!\n");
		goto err;
	}

	// ===========================================================================================

	printf("verification passes as expected\n");
	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (secret_key != NULL) {
		OQS_SECRET_KEY_free(secret_key);
	}


	return ret;
}

int main(void) {
    const char *method_name = "XMSS-SHA2_10_256";
	OQS_STATUS rc = sig_stfl_test_correctness(method_name);
	if (rc != OQS_SUCCESS) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
