// SPDX-License-Identifier: MIT

// This tests the test vectors published by NIST ACVP

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

/* ML_KEM-specific functions */
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

static OQS_STATUS kem_kg_vector(const char *method_name,
                                uint8_t *prng_output_stream,
                                const uint8_t *kg_pk, const uint8_t *kg_sk) {

	uint8_t *entropy_input;
	FILE *fh = NULL;
	OQS_KEM *kem = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	OQS_STATUS rc, ret = OQS_ERROR;

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
		// Only ML-KEM supported
		goto err;
	}

	randombytes_init(entropy_input, NULL);

	fh = stdout;

	public_key = OQS_MEM_malloc(kem->length_public_key);
	secret_key = OQS_MEM_malloc(kem->length_secret_key);

	if ((public_key == NULL) || (secret_key == NULL)) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_MEM_malloc failed!\n", method_name);
		goto err;
	}

	if ((prng_output_stream == NULL) || (kg_pk == NULL) || (kg_sk == NULL)) {
		fprintf(stderr, "[vectors_kem] %s ERROR: inputs NULL!\n", method_name);
		goto err;
	}

	rc = OQS_KEM_keypair(kem, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_KEM_keypair failed!\n", method_name);
		goto err;
	}
	fprintBstr(fh, "ek: ", public_key, kem->length_public_key);
	fprintBstr(fh, "dk: ", secret_key, kem->length_secret_key);

	if (!memcmp(public_key, kg_pk, kem->length_public_key) && !memcmp(secret_key, kg_sk, kem->length_secret_key)) {
		ret = OQS_SUCCESS;
	} else {
		ret = OQS_ERROR;
		fprintf(stderr, "[vectors_kem] %s ERROR: public key or private key doesn't match!\n", method_name);
	}
	goto cleanup;

err:
	ret = OQS_ERROR;
	goto cleanup;

algo_not_enabled:
	ret = OQS_SUCCESS;

cleanup:
	if (kem != NULL) {
		OQS_MEM_secure_free(secret_key, kem->length_secret_key);
	}
	if (randombytes_free != NULL) {
		randombytes_free();
	}
	OQS_MEM_insecure_free(public_key);
	OQS_KEM_free(kem);
	return ret;
}

static OQS_STATUS kem_vector_encdec_aft(const char *method_name,
                                        uint8_t *prng_output_stream,
                                        const uint8_t *encdec_pk,
                                        const uint8_t *encdec_k, const uint8_t *encdec_c) {

	uint8_t *entropy_input;
	FILE *fh = NULL;
	OQS_KEM *kem = NULL;
	uint8_t *ss_encaps = NULL;
	uint8_t *ct_encaps = NULL;
	OQS_STATUS rc, ret = OQS_ERROR;

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
		// Only ML-KEM supported
		goto err;
	}

	randombytes_init(entropy_input, NULL);

	fh = stdout;

	ss_encaps = OQS_MEM_malloc(kem->length_shared_secret);
	ct_encaps = OQS_MEM_malloc(kem->length_ciphertext);
	if ((ss_encaps == NULL) || (ct_encaps == NULL)) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_MEM_malloc failed!\n", method_name);
		goto err;
	}

	if ((prng_output_stream == NULL) || (encdec_pk == NULL) || (encdec_k == NULL) || (encdec_c == NULL)) {
		fprintf(stderr, "[vectors_kem] %s ERROR: inputs NULL!\n", method_name);
		goto err;
	}

	rc = OQS_KEM_encaps(kem, ct_encaps, ss_encaps, encdec_pk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_KEM_encaps failed!\n", method_name);
		goto err;
	}

	fprintBstr(fh, "c: ", ct_encaps, kem->length_ciphertext);
	fprintBstr(fh, "k: ", ss_encaps, kem->length_shared_secret);

	if (!memcmp(ct_encaps, encdec_c, kem->length_ciphertext) && !memcmp(ss_encaps, encdec_k, kem->length_shared_secret)) {
		ret = OQS_SUCCESS;
	} else {
		ret = OQS_ERROR;
		fprintf(stderr, "[vectors_kem] %s ERROR (AFT): ciphertext or shared secret doesn't match!\n", method_name);
	}

	goto cleanup;

err:
	ret = OQS_ERROR;
	goto cleanup;

algo_not_enabled:
	ret = OQS_SUCCESS;

cleanup:
	if (kem != NULL) {
		OQS_MEM_secure_free(ss_encaps, kem->length_shared_secret);
	}
	if (randombytes_free != NULL) {
		randombytes_free();
	}
	OQS_MEM_insecure_free(ct_encaps);
	OQS_KEM_free(kem);
	return ret;
}

static OQS_STATUS kem_vector_encdec_val(const char *method_name,
                                        const uint8_t *encdec_sk, const uint8_t *encdec_c,
                                        const uint8_t *encdec_k) {
	FILE *fh = NULL;
	OQS_KEM *kem = NULL;
	uint8_t *ss_decaps = NULL;
	OQS_STATUS rc, ret = OQS_ERROR;

	kem = OQS_KEM_new(method_name);
	if (kem == NULL) {
		printf("[vectors_kem] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	fh = stdout;

	ss_decaps = OQS_MEM_malloc(kem->length_shared_secret);

	if (ss_decaps == NULL) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_MEM_malloc failed!\n", method_name);
		goto err;
	}

	if ((encdec_sk == NULL) || (encdec_k == NULL) || (encdec_c == NULL)) {
		fprintf(stderr, "[vectors_kem] %s ERROR: inputs NULL!\n", method_name);
		goto err;
	}

	rc = OQS_KEM_decaps(kem, ss_decaps, encdec_c, encdec_sk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_KEM_encaps failed!\n", method_name);
		goto err;
	}

	fprintBstr(fh, "k: ", ss_decaps, kem->length_shared_secret);

	if (!memcmp(ss_decaps, encdec_k, kem->length_shared_secret)) {
		ret = OQS_SUCCESS;
	} else {
		ret = OQS_ERROR;
		fprintf(stderr, "[vectors_kem] %s ERROR (AFT): shared secret doesn't match!\n", method_name);
	}

	goto cleanup;

err:
	ret = OQS_ERROR;
	goto cleanup;

algo_not_enabled:
	ret = OQS_SUCCESS;

cleanup:
	if (kem != NULL) {
		OQS_MEM_secure_free(ss_decaps, kem->length_shared_secret);
	}
	OQS_KEM_free(kem);
	return ret;
}

int main(int argc, char **argv) {
	OQS_STATUS rc = OQS_SUCCESS;

	OQS_init();

	if (argc != 6 && argc != 7) {
		fprintf(stderr, "Usage: vectors_kem algname testname [testargs]\n");
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
	char *test_name = argv[2];
	char *prng_output_stream;
	char *kg_pk;
	char *kg_sk;
	char *encdec_aft_pk;
	char *encdec_aft_k;
	char *encdec_aft_c;

	char *encdec_val_sk;
	char *encdec_val_k;
	char *encdec_val_c;

	uint8_t *prng_output_stream_bytes = NULL;
	uint8_t *kg_pk_bytes = NULL;
	uint8_t *kg_sk_bytes = NULL;

	uint8_t *encdec_aft_pk_bytes = NULL;
	uint8_t *encdec_aft_k_bytes = NULL;
	uint8_t *encdec_aft_c_bytes = NULL;

	uint8_t *encdec_val_sk_bytes = NULL;
	uint8_t *encdec_val_k_bytes = NULL;
	uint8_t *encdec_val_c_bytes = NULL;

	OQS_KEM *kem = OQS_KEM_new(alg_name);
	if (kem == NULL) {
		printf("[vectors_kem] %s was not enabled at compile-time.\n", alg_name);
		rc = OQS_ERROR;
		goto err;
	}

	if (!strcmp(test_name, "keyGen")) {
		prng_output_stream = argv[3]; // d || z : both should be 32 bytes each as per FIPS-203
		kg_pk = argv[4];
		kg_sk = argv[5];

		if (strlen(prng_output_stream) != 128 ||
		        strlen(kg_pk) != 2 * kem->length_public_key ||
		        strlen(kg_sk) != 2 * kem->length_secret_key) {
			rc = OQS_ERROR;
			goto err;
		}

		prng_output_stream_bytes = OQS_MEM_malloc(strlen(prng_output_stream) / 2);
		kg_pk_bytes = OQS_MEM_malloc(kem->length_public_key);
		kg_sk_bytes = OQS_MEM_malloc(kem->length_secret_key);

		if ((prng_output_stream_bytes == NULL) || (kg_pk_bytes == NULL) || (kg_sk_bytes == NULL)) {
			fprintf(stderr, "[vectors_kem] ERROR: OQS_MEM_malloc failed!\n");
			rc = OQS_ERROR;
			goto err;
		}

		hexStringToByteArray(prng_output_stream, prng_output_stream_bytes);
		hexStringToByteArray(kg_pk, kg_pk_bytes);
		hexStringToByteArray(kg_sk, kg_sk_bytes);


		rc = kem_kg_vector(alg_name, prng_output_stream_bytes, kg_pk_bytes, kg_sk_bytes);
	} else if (!strcmp(test_name, "encDecAFT")) {
		prng_output_stream = argv[3]; // m : should be 32 bytes as per FIPS-203
		encdec_aft_pk = argv[4];
		encdec_aft_k = argv[5];
		encdec_aft_c = argv[6];

		if (strlen(prng_output_stream) != 64 ||
		        strlen(encdec_aft_c) != 2 * kem->length_ciphertext ||
		        strlen(encdec_aft_k) != 2 * kem->length_shared_secret ||
		        strlen(encdec_aft_pk) != 2 * kem->length_public_key) {
			rc = OQS_ERROR;
			goto err;
		}

		prng_output_stream_bytes = OQS_MEM_malloc(strlen(prng_output_stream) / 2);
		encdec_aft_pk_bytes = OQS_MEM_malloc(kem->length_public_key);
		encdec_aft_k_bytes = OQS_MEM_malloc(kem->length_shared_secret);
		encdec_aft_c_bytes = OQS_MEM_malloc(kem->length_ciphertext);

		if ((prng_output_stream_bytes == NULL) || (encdec_aft_pk_bytes == NULL) || (encdec_aft_k_bytes == NULL) || (encdec_aft_c_bytes == NULL)) {
			fprintf(stderr, "[vectors_kem] ERROR: OQS_MEM_malloc failed!\n");
			rc = OQS_ERROR;
			goto err;
		}

		hexStringToByteArray(prng_output_stream, prng_output_stream_bytes);
		hexStringToByteArray(encdec_aft_pk, encdec_aft_pk_bytes);
		hexStringToByteArray(encdec_aft_k, encdec_aft_k_bytes);
		hexStringToByteArray(encdec_aft_c, encdec_aft_c_bytes);

		rc = kem_vector_encdec_aft(alg_name, prng_output_stream_bytes, encdec_aft_pk_bytes, encdec_aft_k_bytes, encdec_aft_c_bytes);
	} else if (!strcmp(test_name, "encDecVAL")) {
		encdec_val_sk = argv[3];
		encdec_val_k = argv[4];
		encdec_val_c = argv[5];

		if (strlen(encdec_val_c) != 2 * kem->length_ciphertext ||
		        strlen(encdec_val_k) != 2 * kem->length_shared_secret ||
		        strlen(encdec_val_sk) != 2 * kem->length_secret_key) {
			rc = OQS_ERROR;
			goto err;
		}

		encdec_val_sk_bytes = OQS_MEM_malloc(kem->length_secret_key);
		encdec_val_k_bytes = OQS_MEM_malloc(kem->length_shared_secret);
		encdec_val_c_bytes = OQS_MEM_malloc(kem->length_ciphertext);

		if ((encdec_val_sk_bytes == NULL) || (encdec_val_k_bytes == NULL) || (encdec_val_c_bytes == NULL)) {
			fprintf(stderr, "[vectors_kem] ERROR: OQS_MEM_malloc failed!\n");
			rc = OQS_ERROR;
			goto err;
		}

		hexStringToByteArray(encdec_val_sk, encdec_val_sk_bytes);
		hexStringToByteArray(encdec_val_k, encdec_val_k_bytes);
		hexStringToByteArray(encdec_val_c, encdec_val_c_bytes);

		rc = kem_vector_encdec_val(alg_name, encdec_val_sk_bytes, encdec_val_c_bytes, encdec_val_k_bytes);
	} else {
		printf("[vectors_kem] %s only keyGen supported!\n", alg_name);
	}

err:
	OQS_MEM_insecure_free(prng_output_stream_bytes);
	OQS_MEM_insecure_free(kg_pk_bytes);
	OQS_MEM_insecure_free(kg_sk_bytes);

	OQS_MEM_insecure_free(encdec_aft_c_bytes);
	OQS_MEM_insecure_free(encdec_aft_k_bytes);
	OQS_MEM_insecure_free(encdec_aft_pk_bytes);

	OQS_MEM_insecure_free(encdec_val_c_bytes);
	OQS_MEM_insecure_free(encdec_val_k_bytes);
	OQS_MEM_insecure_free(encdec_val_sk_bytes);

	OQS_KEM_free(kem);

	OQS_destroy();

	if (rc != OQS_SUCCESS) {
		return EXIT_FAILURE;
	} else {
		return EXIT_SUCCESS;
	}
}
