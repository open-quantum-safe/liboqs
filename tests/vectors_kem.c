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

typedef struct {
	const uint8_t *pos;
} fixed_prng_state;

fixed_prng_state prng_state = { .pos = 0 };

/* Displays hexadecimal strings */
void OQS_print_hex_string(const char *label, const uint8_t *str, size_t len) {
	printf("%-20s (%4zu bytes):  ", label, len);
	for (size_t i = 0; i < (len); i++) {
		printf("%02X", str[i]);
	}
	printf("\n");
}

void fprintBstr(FILE *fp, const char *S, const uint8_t *A, size_t L) {
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

static inline uint16_t UINT16_TO_BE(const uint16_t x) {
	union {
		uint16_t val;
		uint8_t bytes[2];
	} y;
	y.bytes[0] = (x >> 8) & 0xFF;
	y.bytes[1] = x & 0xFF;
	return y.val;
}

/* HQC-specific functions */
static inline bool is_ml_kem(const char *method_name) {
	return (0 == strcmp(method_name, OQS_KEM_alg_ml_kem_512))
	       || (0 == strcmp(method_name, OQS_KEM_alg_ml_kem_768))
	       || (0 == strcmp(method_name, OQS_KEM_alg_ml_kem_1024));
}

static void MLKEM_randombytes_init(const uint8_t *entropy_input, const uint8_t *personalization_string) {
	(void) personalization_string;
	prng_state.pos = entropy_input;
}

static void MLKEM_randombytes(uint8_t *random_array, size_t bytes_to_read) {
	memcpy(random_array, prng_state.pos, bytes_to_read);
	prng_state.pos += bytes_to_read;
}

static void MLKEM_randombytes_free(void) {
	prng_state.pos = 0;
}

OQS_STATUS kem_vector(const char *method_name,
                      uint8_t *prng_output_stream,
                      const uint8_t *encaps_pk, const uint8_t *encaps_K,
                      const uint8_t *decaps_sk, const uint8_t *decaps_ciphertext, const uint8_t *decaps_kprime) {

	uint8_t *entropy_input;
	uint8_t seed[48];
	FILE *fh = NULL;
	OQS_KEM *kem = NULL;
	uint8_t *msg = NULL;
	size_t msg_len = 0;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *ss_encaps = NULL;
	uint8_t *ct_encaps = NULL;
	uint8_t *ss_decaps = NULL;
	OQS_STATUS rc, ret = OQS_ERROR;
	int rv;

	void (*randombytes_init)(const uint8_t *, const uint8_t *) = NULL;
	void (*randombytes_free)(void) = NULL;

	kem = OQS_KEM_new(method_name);
	if (kem == NULL) {
		printf("[vectors_kem] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	if (is_ml_kem(method_name)) {
		OQS_randombytes_custom_algorithm(&MLKEM_randombytes);
		randombytes_init = &MLKEM_randombytes_init;
		randombytes_free = &MLKEM_randombytes_free;
		entropy_input = (uint8_t *) prng_output_stream;
	} else {
		// Only ML-KEM-ipd supported
		goto err;
	}

	randombytes_init(entropy_input, NULL);

	fh = stdout;

	public_key = malloc(kem->length_public_key);
	secret_key = malloc(kem->length_secret_key);
	ss_encaps = malloc(kem->length_shared_secret);
	ct_encaps = malloc(kem->length_ciphertext);
	ss_decaps = malloc(kem->length_shared_secret);
	if ((public_key == NULL) || (secret_key == NULL) || (ss_encaps == NULL) || (ct_encaps == NULL) || (ss_decaps == NULL)) {
		fprintf(stderr, "[vectors_kem] %s ERROR: malloc failed!\n", method_name);
		goto err;
	}

	rc = OQS_KEM_keypair(kem, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_KEM_keypair failed!\n", method_name);
		goto err;
	}
	fprintBstr(fh, "ek: ", public_key, kem->length_public_key);
	fprintBstr(fh, "dk: ", secret_key, kem->length_secret_key);

	rc = OQS_KEM_encaps(kem, ct_encaps, ss_encaps, encaps_pk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_KEM_encaps failed!\n", method_name);
		goto err;
	}

	fprintBstr(fh, "c: ", ct_encaps, kem->length_ciphertext);
	fprintBstr(fh, "K: ", ss_encaps, kem->length_shared_secret);

	rc = OQS_KEM_decaps(kem, ss_decaps, decaps_ciphertext, decaps_sk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_KEM_decaps failed!\n", method_name);
		goto err;
	}

	rv = memcmp(ss_decaps, decaps_kprime, kem->length_shared_secret);
	if (rv != 0) {
		fprintf(stderr, "[vectors_kem] %s ERROR: shared secrets are not equal\n", method_name);
		OQS_print_hex_string("ss_decaps", ss_decaps, kem->length_shared_secret);
		OQS_print_hex_string("decaps_kprime", decaps_kprime, kem->length_shared_secret);
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
	if (kem != NULL) {
		OQS_MEM_secure_free(secret_key, kem->length_secret_key);
		OQS_MEM_secure_free(ss_encaps, kem->length_shared_secret);
		OQS_MEM_secure_free(ss_decaps, kem->length_shared_secret);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(ct_encaps);
	return ret;
}

int main(int argc, char **argv) {
	OQS_init();

	if (argc != 8) {
		fprintf(stderr, "Usage: vectors_kem algname prng_output_stream encaps_pk encaps_K decaps_sk decaps_ciphertext decaps_kprime\n");
		fprintf(stderr, "  algname: ");
		for (size_t i = 0; i < OQS_KEM_algs_length; i++) {
			if (i > 0) {
				fprintf(stderr, ", ");
			}
			fprintf(stderr, "%s", OQS_KEM_alg_identifier(i));
		}
		fprintf(stderr, "\n");
		printf("\n");
		print_system_info();
		OQS_destroy();
		return EXIT_FAILURE;
	}

	char *alg_name = argv[1];
	char *prng_output_stream = argv[2]; // z || d || m

	char *encaps_pk = argv[3];
	char *encaps_K = argv[4];

	char *decaps_sk = argv[5];
	char *decaps_ciphertext = argv[6];
	char *decaps_kprime = argv[7];


	if (strlen(prng_output_stream) % 2 != 0 ||
	        strlen(encaps_pk) % 2 != 0 ||
	        strlen(encaps_K) % 2 != 0 ||
	        strlen(decaps_sk) % 2 != 0 ||
	        strlen(decaps_ciphertext) % 2 != 0 ||
	        strlen(decaps_kprime) % 2 != 0) {
		return EXIT_FAILURE;
	}

	uint8_t *prng_output_stream_bytes = malloc(strlen(prng_output_stream) / 2);
	uint8_t *encaps_pk_bytes = malloc(strlen(encaps_pk) / 2);
	uint8_t *encaps_K_bytes = malloc(strlen(encaps_K) / 2);
	uint8_t *decaps_sk_bytes = malloc(strlen(decaps_sk) / 2);
	uint8_t *decaps_ciphertext_bytes = malloc(strlen(decaps_ciphertext) / 2);
	uint8_t *decaps_kprime_bytes = malloc(strlen(decaps_kprime) / 2);

	hexStringToByteArray(prng_output_stream, prng_output_stream_bytes);
	hexStringToByteArray(encaps_pk, encaps_pk_bytes);
	hexStringToByteArray(encaps_K, encaps_K_bytes);
	hexStringToByteArray(decaps_sk, decaps_sk_bytes);
	hexStringToByteArray(decaps_ciphertext, decaps_ciphertext_bytes);
	hexStringToByteArray(decaps_kprime, decaps_kprime_bytes);

	OQS_STATUS rc = kem_vector(alg_name, prng_output_stream_bytes, encaps_pk_bytes, encaps_K_bytes, decaps_sk_bytes, decaps_ciphertext_bytes, decaps_kprime_bytes);
	if (rc != OQS_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}

	OQS_MEM_insecure_free(prng_output_stream_bytes);
	OQS_MEM_insecure_free(encaps_pk_bytes);
	OQS_MEM_insecure_free(encaps_K_bytes);
	OQS_MEM_insecure_free(decaps_sk_bytes);
	OQS_MEM_insecure_free(decaps_ciphertext_bytes);
	OQS_MEM_insecure_free(decaps_kprime_bytes);

	OQS_destroy();
	return EXIT_SUCCESS;
}
