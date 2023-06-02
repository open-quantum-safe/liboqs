// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

#include "system_info.c"

/* Displays hexadecimal strings */
static void OQS_print_hex_string(const char *label, const uint8_t *str, size_t len) {
	printf("%-20s (%4zu bytes):  ", label, len);
	for (size_t i = 0; i < (len); i++) {
		printf("%02X", str[i]);
	}
	printf("\n");
}

typedef struct magic_s {
	uint8_t val[31];
} magic_t;


static OQS_STATUS kem_test_key_derive(const char *method_name) {

	OQS_KEM *kem = NULL;
	uint8_t *randomness = NULL;
	uint8_t *public_key_1 = NULL;
	uint8_t *secret_key_1 = NULL;
	uint8_t *public_key_2 = NULL;
	uint8_t *secret_key_2 = NULL;

	OQS_STATUS rc, ret = OQS_ERROR;
	int rv;

	//The magic numbers are random values
	//The length of the magic number was chosen to be 31 to break alignment
	magic_t magic;
	OQS_randombytes(magic.val, sizeof(magic_t));

	kem = OQS_KEM_new(method_name);
	if (kem == NULL) {
		fprintf(stderr, "ERROR: OQS_KEM_new failed\n");
		goto err;
	}

	printf("================================================================================\n");
	printf("Sample computation for KEM key derive %s\n", kem->method_name);
	printf("================================================================================\n");

	public_key_1 = malloc(kem->length_public_key + 2 * sizeof(magic_t));
	secret_key_1 = malloc(kem->length_secret_key + 2 * sizeof(magic_t));
	public_key_2 = malloc(kem->length_public_key + 2 * sizeof(magic_t));
	secret_key_2 = malloc(kem->length_secret_key + 2 * sizeof(magic_t));
	randomness = malloc(kem->length_randomness);

	if ((public_key_1 == NULL) || (secret_key_1 == NULL) || (public_key_2 == NULL) || (secret_key_2 == NULL) || (randomness == NULL)) {
		fprintf(stderr, "ERROR: malloc failed\n");
		goto err;
	}


	//Set the magic numbers before
	memcpy(public_key_1, magic.val, sizeof(magic_t));
	memcpy(secret_key_1, magic.val, sizeof(magic_t));
	memcpy(public_key_2, magic.val, sizeof(magic_t));
	memcpy(secret_key_2, magic.val, sizeof(magic_t));

	public_key_1 += sizeof(magic_t);
	secret_key_1 += sizeof(magic_t);
	public_key_2 += sizeof(magic_t);
	secret_key_2 += sizeof(magic_t);

	// and after
	memcpy(public_key_1 + kem->length_public_key, magic.val, sizeof(magic_t));
	memcpy(secret_key_1 + kem->length_secret_key, magic.val, sizeof(magic_t));
	memcpy(public_key_2 + kem->length_public_key, magic.val, sizeof(magic_t));
	memcpy(secret_key_2 + kem->length_secret_key, magic.val, sizeof(magic_t));

	// generate a random seed
	OQS_randombytes(randomness, kem->length_randomness);

	rc = OQS_KEM_derive_keypair(kem, randomness, public_key_1, secret_key_1);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_derive_keypair failed\n");
		goto err;
	}

	rc = OQS_KEM_derive_keypair(kem, randomness, public_key_2, secret_key_2);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_derive_keypair failed\n");
		goto err;
	}

	rv = memcmp(secret_key_1, secret_key_2, kem->length_secret_key);
	if (rv != 0) {
		fprintf(stderr, "ERROR: secret keys are not equal\n");
		OQS_print_hex_string("secret_key_1", secret_key_1, kem->length_secret_key);
		OQS_print_hex_string("secret_key_2", secret_key_2, kem->length_secret_key);
		goto err;
	} else {
		printf("secret keys are equal\n");
	}

	rv = memcmp(public_key_1, public_key_2, kem->length_public_key);
	if (rv != 0) {
		fprintf(stderr, "ERROR: public keys are not equal\n");
		OQS_print_hex_string("public_key_1", public_key_1, kem->length_public_key);
		OQS_print_hex_string("public_key_2", public_key_2, kem->length_public_key);
		goto err;
	} else {
		printf("public keys are equal\n");
	}

	rv = memcmp(public_key_1 + kem->length_public_key, magic.val, sizeof(magic_t));
	rv |= memcmp(public_key_2 + kem->length_public_key, magic.val, sizeof(magic_t));
	rv |= memcmp(secret_key_1 + kem->length_secret_key, magic.val, sizeof(magic_t));
	rv |= memcmp(secret_key_2 + kem->length_secret_key, magic.val, sizeof(magic_t));
	rv |= memcmp(public_key_1 - sizeof(magic_t), magic.val, sizeof(magic_t));
	rv |= memcmp(public_key_2 - sizeof(magic_t), magic.val, sizeof(magic_t));
	rv |= memcmp(secret_key_1 - sizeof(magic_t), magic.val, sizeof(magic_t));
	rv |= memcmp(secret_key_2 - sizeof(magic_t), magic.val, sizeof(magic_t));
	if (rv != 0) {
		fprintf(stderr, "ERROR: Magic numbers do not match\n");
		goto err;
	}

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (kem != NULL) {
		OQS_MEM_secure_free(secret_key_1 - sizeof(magic_t), kem->length_secret_key + 2 * sizeof(magic_t));
		OQS_MEM_secure_free(secret_key_2 - sizeof(magic_t), kem->length_secret_key + 2 * sizeof(magic_t));
	}
	OQS_MEM_insecure_free(public_key_1 - sizeof(magic_t));
	OQS_MEM_insecure_free(public_key_2 - sizeof(magic_t));
	OQS_KEM_free(kem);

	return ret;
}

int main(int argc, char **argv) {
	OQS_init();

	printf("Testing KEM keypair derivation from a seed using liboqs version %s\n", OQS_version());

	if (argc != 2) {
		fprintf(stderr, "Usage: test_kem algname\n");
		fprintf(stderr, "  algname: ");
		for (size_t i = 0; i < OQS_KEM_algs_length; i++) {
			if (i > 0) {
				fprintf(stderr, ", ");
			}
			fprintf(stderr, "%s", OQS_KEM_alg_identifier(i));
		}
		fprintf(stderr, "\n");
		OQS_destroy();
		return EXIT_FAILURE;
	}

	print_system_info();

	char *alg_name = argv[1];
	if (!OQS_KEM_alg_is_enabled(alg_name)) {
		printf("KEM algorithm %s not enabled!\n", alg_name);
		OQS_destroy();
		return EXIT_FAILURE;
	}

	OQS_STATUS rc;
	rc = kem_test_key_derive(alg_name);
	if (rc != OQS_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	OQS_destroy();
	return EXIT_SUCCESS;
}