// SPDX-License-Identifier: MIT

// This tests the test vectors published by NIST CAVP

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <oqs/oqs.h>

#include "system_info.c"

struct {
	const uint8_t *pos;
} prng_state = {
	.pos = 0
};

static void fprintBstr(FILE *fp, const char *S, const uint8_t *A, size_t L) {
	size_t i;
	fprintf(fp, "%s", S);
	for (i = 0; i < L; i++) {
		fprintf(fp, "%02X", A[i]);
	}
	if (L == 0) {
		fprintf(fp, "00");
	}
	fprintf(fp, "\n");
}

static uint8_t hexCharToDecimal(char c) {
	if (c >= '0' && c <= '9') {
		return (uint8_t) (c - '0');
	} else if (c >= 'a' && c <= 'f') {
		return (uint8_t) (c - 'a' + 10);
	} else if (c >= 'A' && c <= 'F') {
		return (uint8_t) (c - 'A' + 10);
	} else {
		fprintf(stderr, "Invalid hex character: %c\n", c);
		return 0;
	}
}

static void hexStringToByteArray(const char *hexString, uint8_t *byteArray) {
	size_t len = strlen(hexString);

	if (len % 2 != 0) {
		fprintf(stderr, "Hex string must have an even number of characters\n");
		exit(EXIT_FAILURE);
	}

	for (size_t i = 0, j = 0; i < len; i += 2, j++) {
		byteArray[j] = (uint8_t) ((hexCharToDecimal(hexString[i]) << 4) | hexCharToDecimal(hexString[i + 1]));
	}
}

/* HQC-specific functions */
static inline bool is_ml_dsa(const char *method_name) {
	return (0 == strcmp(method_name, OQS_SIG_alg_ml_dsa_44_ipd))
	       || (0 == strcmp(method_name, OQS_SIG_alg_ml_dsa_65_ipd))
	       || (0 == strcmp(method_name, OQS_SIG_alg_ml_dsa_87_ipd))
	       || (0 == strcmp(method_name, OQS_SIG_alg_ml_dsa_44))
	       || (0 == strcmp(method_name, OQS_SIG_alg_ml_dsa_65))
	       || (0 == strcmp(method_name, OQS_SIG_alg_ml_dsa_87));
}

static void MLDSA_randombytes_init(const uint8_t *entropy_input, const uint8_t *personalization_string) {
	(void) personalization_string;
	prng_state.pos = entropy_input;
}

static void MLDSA_randombytes(uint8_t *random_array, size_t bytes_to_read) {
	memcpy(random_array, prng_state.pos, bytes_to_read);
	prng_state.pos += bytes_to_read;
}

static void MLDSA_randombytes_free(void) {
	prng_state.pos = 0;
}

OQS_STATUS sig_vector(const char *method_name,
                      uint8_t *prng_output_stream,
                      const uint8_t *sig_msg, size_t sig_msg_len, const uint8_t *sig_sk,
                      const uint8_t *verif_sig, const uint8_t *verif_pk, const uint8_t *verif_msg, size_t verif_msg_len) {

	uint8_t *entropy_input;
	FILE *fh = NULL;
	OQS_SIG *sig = NULL;
	uint8_t *msg = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *signature = NULL;
	uint8_t *signed_msg = NULL;
	size_t signature_len = 0;
	size_t signed_msg_len = 0;
	OQS_STATUS rc, ret = OQS_ERROR;

	void (*randombytes_init)(const uint8_t *, const uint8_t *) = NULL;
	void (*randombytes_free)(void) = NULL;

	sig = OQS_SIG_new(method_name);
	if (sig == NULL) {
		printf("[sig_kat] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	if (is_ml_dsa(method_name)) {
		OQS_randombytes_custom_algorithm(&MLDSA_randombytes);
		randombytes_init = &MLDSA_randombytes_init;
		randombytes_free = &MLDSA_randombytes_free;
		entropy_input = (uint8_t *) prng_output_stream;
	} else {
		// Only ML-DSA-ipd supported
		goto err;
	}

	randombytes_init(entropy_input, NULL);

	fh = stdout;

	public_key = malloc(sig->length_public_key);
	secret_key = malloc(sig->length_secret_key);
	signature = malloc(sig->length_signature);
	if ((public_key == NULL) || (secret_key == NULL) || (signature == NULL)) {
		fprintf(stderr, "[vectors_sig] %s ERROR: malloc failed!\n", method_name);
		goto err;
	}

	rc = OQS_SIG_keypair(sig, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[vectors_sig] %s ERROR: OQS_SIG_keypair failed!\n", method_name);
		goto err;
	}
	fprintBstr(fh, "pk: ", public_key, sig->length_public_key);
	fprintBstr(fh, "sk: ", secret_key, sig->length_secret_key);

	rc = OQS_SIG_sign(sig, signature, &signature_len, sig_msg, sig_msg_len, sig_sk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[vectors_sig] %s ERROR: OQS_SIG_sign failed!\n", method_name);
		goto err;
	}

	fprintBstr(fh, "signature: ", signature, signature_len);

	rc = OQS_SIG_verify(sig, verif_msg, verif_msg_len, verif_sig, signature_len, verif_pk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[vectors_sig] %s ERROR: OQS_SIG_verify failed!\n", method_name);
		goto err;
	}

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;
	goto cleanup;

algo_not_enabled:
	ret = OQS_SUCCESS;

cleanup:
	if (sig != NULL) {
		OQS_MEM_secure_free(secret_key, sig->length_secret_key);
		OQS_MEM_secure_free(signed_msg, signed_msg_len);
	}
	if (randombytes_free != NULL) {
		randombytes_free();
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(signature);
	OQS_MEM_insecure_free(msg);
	OQS_SIG_free(sig);
	return ret;
}

int main(int argc, char **argv) {
	OQS_STATUS rc;

	OQS_init();

	if (argc != 8) {
		fprintf(stderr, "Usage: vectors_sig algname prng_output_stream sig_msg sig_sk verif_sig verif_pk verif_msg\n");
		fprintf(stderr, "  algname: ");
		for (size_t i = 0; i < OQS_SIG_algs_length; i++) {
			if (i > 0) {
				fprintf(stderr, ", ");
			}
			fprintf(stderr, "%s", OQS_SIG_alg_identifier(i));
		}
		fprintf(stderr, "\n");
		printf("\n");
		print_system_info();
		OQS_destroy();
		return EXIT_FAILURE;
	}

	char *alg_name = argv[1];
	char *prng_output_stream = argv[2];
	char *sig_msg = argv[3];
	size_t sig_msg_len = strlen(sig_msg) / 2;
	char *sig_sk = argv[4];
	char *verif_sig = argv[5];
	char *verif_pk = argv[6];
	char *verif_msg = argv[7];
	size_t verif_msg_len = strlen(verif_msg) / 2;

	uint8_t *prng_output_stream_bytes = NULL;
	uint8_t *sig_msg_bytes = NULL;
	uint8_t *sig_sk_bytes = NULL;
	uint8_t *verif_sig_bytes = NULL;
	uint8_t *verif_pk_bytes = NULL;
	uint8_t *verif_msg_bytes = NULL;

	OQS_SIG *sig = OQS_SIG_new(alg_name);
	if (sig == NULL) {
		printf("[vectors_sig] %s was not enabled at compile-time.\n", alg_name);
		rc = OQS_ERROR;
		goto err;
	}

	if (strlen(prng_output_stream) % 2 != 0 ||
	        strlen(sig_msg) % 2 != 0 || // variable length
	        strlen(sig_sk) != 2 * sig->length_secret_key ||
	        strlen(verif_sig) != 2 * sig->length_signature ||
	        strlen(verif_pk) != 2 * sig->length_public_key ||
	        strlen(verif_msg) % 2 != 0) { // variable length
		rc = OQS_ERROR;
		goto err;
	}

	prng_output_stream_bytes = malloc(strlen(prng_output_stream) / 2);
	sig_msg_bytes = malloc(strlen(sig_msg) / 2);
	sig_sk_bytes = malloc(sig->length_secret_key);
	verif_sig_bytes = malloc(sig->length_signature);
	verif_pk_bytes = malloc(sig->length_public_key);
	verif_msg_bytes = malloc(strlen(verif_msg) / 2);

	if ((prng_output_stream_bytes == NULL) || (sig_msg_bytes == NULL) || (sig_sk_bytes == NULL) || (verif_sig_bytes == NULL) || (verif_pk_bytes == NULL) || (verif_msg_bytes == NULL)) {
		fprintf(stderr, "[vectors_sig] ERROR: malloc failed!\n");
		rc = OQS_ERROR;
		goto err;
	}


	hexStringToByteArray(prng_output_stream, prng_output_stream_bytes);
	hexStringToByteArray(sig_msg, sig_msg_bytes);
	hexStringToByteArray(sig_sk, sig_sk_bytes);
	hexStringToByteArray(verif_sig, verif_sig_bytes);
	hexStringToByteArray(verif_pk, verif_pk_bytes);
	hexStringToByteArray(verif_msg, verif_msg_bytes);

	rc = sig_vector(alg_name, prng_output_stream_bytes, sig_msg_bytes, sig_msg_len, sig_sk_bytes, verif_sig_bytes, verif_pk_bytes, verif_msg_bytes, verif_msg_len);

err:
	OQS_MEM_insecure_free(prng_output_stream_bytes);
	OQS_MEM_insecure_free(sig_msg_bytes);
	OQS_MEM_insecure_free(sig_sk_bytes);
	OQS_MEM_insecure_free(verif_sig_bytes);
	OQS_MEM_insecure_free(verif_pk_bytes);
	OQS_MEM_insecure_free(verif_msg_bytes);

	OQS_SIG_free(sig);

	OQS_destroy();

	if (rc != OQS_SUCCESS) {
		return EXIT_FAILURE;
	} else {
		return EXIT_SUCCESS;
	}
}
