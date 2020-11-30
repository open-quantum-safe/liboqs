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
	uint8_t val[32];
} magic_t;

typedef enum kem_ops {
	KEM_KEYGEN    = 0,
        KEM_ENCAPS    = 1,
        KEM_DECAPS    = 2
} KEM_OPS;

#define STORE_PREFIX "/tmp/oqs-temp-file-"
#define MAXPATHLEN 64

static OQS_STATUS oqs_fstore(const char* fname, uint8_t *data, int len) {
	char fpath[MAXPATHLEN];
	strcpy(fpath,STORE_PREFIX);
	FILE* fp = fopen(strcat(fpath,fname), "wb");
	if (!fp) {
		fprintf(stderr, "Couldn't open %s for writing.\n", fpath);
		return OQS_ERROR;
	}
	fwrite(data,len,1,fp);
	fclose(fp);
	return OQS_SUCCESS;
}

static OQS_STATUS oqs_fload(const char* fname, uint8_t *data, int len) {
	char fpath[MAXPATHLEN];
	strcpy(fpath,STORE_PREFIX);
	FILE* fp = fopen(strcat(fpath,fname), "r");
	if (!fp) {
		fprintf(stderr, "Couldn't open %s for reading.\n", fpath);
		return OQS_ERROR;
	}
	if (fread(data,len,1,fp) != 1) {
		fprintf(stderr, "Couldn't read all %d bytes correctly. Exiting.\n", len);
		return OQS_ERROR;
	}
	fclose(fp);
	return OQS_SUCCESS;
}

static OQS_STATUS kem_test_correctness(const char *method_name, KEM_OPS op) {

	OQS_KEM *kem = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *ciphertext = NULL;
	uint8_t *shared_secret_e = NULL;
	uint8_t *shared_secret_d = NULL;
	OQS_STATUS rc, ret = OQS_ERROR;
	int rv;

	kem = OQS_KEM_new(method_name);
	if (kem == NULL) {
		fprintf(stderr, "ERROR: OQS_KEM_new failed\n");
		goto err;
	}

	printf("================================================================================\n");
	printf("Sample computation for KEM %s\n", kem->method_name);
	printf("================================================================================\n");

	public_key = malloc(kem->length_public_key);
	secret_key = malloc(kem->length_secret_key);
	ciphertext = malloc(kem->length_ciphertext);
	shared_secret_e = malloc(kem->length_shared_secret);
	shared_secret_d = malloc(kem->length_shared_secret);

	if ((public_key == NULL) || (secret_key == NULL) || (ciphertext == NULL) || (shared_secret_e == NULL) || (shared_secret_d == NULL)) {
		fprintf(stderr, "ERROR: malloc failed\n");
		goto err;
	}

	if (op == KEM_KEYGEN) {
		rc = OQS_KEM_keypair(kem, public_key, secret_key);
		if (rc != OQS_SUCCESS) {
			fprintf(stderr, "ERROR: OQS_KEM_keypair failed\n");
			goto err;
		}
		if (oqs_fstore("pk", public_key, kem->length_public_key) != OQS_SUCCESS) goto err;
		if (oqs_fstore("sk", secret_key, kem->length_secret_key) != OQS_SUCCESS) goto err;
		ret = OQS_SUCCESS;
		goto cleanup;
	}
	else { // get it from file
		if (oqs_fload("pk", public_key, kem->length_public_key) != OQS_SUCCESS) goto err;
                if (oqs_fload("sk", secret_key, kem->length_secret_key) != OQS_SUCCESS) goto err;	
	}

	if (op == KEM_ENCAPS) {
		rc = OQS_KEM_encaps(kem, ciphertext, shared_secret_e, public_key);
		if (rc != OQS_SUCCESS) {
			fprintf(stderr, "ERROR: OQS_KEM_encaps failed\n");
			goto err;
		}
		if (oqs_fstore("ct", ciphertext, kem->length_ciphertext) != OQS_SUCCESS) goto err;
		if (oqs_fstore("se", shared_secret_e, kem->length_shared_secret) != OQS_SUCCESS) goto err;
		ret = OQS_SUCCESS;
		goto cleanup;
	}
	else { // get it from file
		if (oqs_fload("ct", ciphertext, kem->length_ciphertext) != OQS_SUCCESS) goto err;
                if (oqs_fload("se", shared_secret_e, kem->length_shared_secret) != OQS_SUCCESS) goto err;	
	}

	if (op == KEM_DECAPS) {
		rc = OQS_KEM_decaps(kem, shared_secret_d, ciphertext, secret_key);
		if (rc != OQS_SUCCESS) {
			fprintf(stderr, "ERROR: OQS_KEM_decaps failed\n");
			goto err;
		}

	 	rv = memcmp(shared_secret_e, shared_secret_d, kem->length_shared_secret);
		if (rv != 0) {
			fprintf(stderr, "ERROR: shared secrets are not equal\n");
			OQS_print_hex_string("shared_secret_e", shared_secret_e, kem->length_shared_secret);
			OQS_print_hex_string("shared_secret_d", shared_secret_d, kem->length_shared_secret);
			goto err;
		} else {
			printf("shared secrets are equal\n");
		}
	}

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (kem != NULL) {
		OQS_MEM_secure_free(secret_key, kem->length_secret_key);
		OQS_MEM_secure_free(shared_secret_e, kem->length_shared_secret);
		OQS_MEM_secure_free(shared_secret_d, kem->length_shared_secret);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(ciphertext);
	OQS_KEM_free(kem);

	return ret;
}

int main(int argc, char **argv) {

	if (argc != 3) {
		fprintf(stderr, "Usage: test_kem_mem algname operation (0,1,2)\n");
		fprintf(stderr, "  algname: ");
		for (size_t i = 0; i < OQS_KEM_algs_length; i++) {
			if (i > 0) {
				fprintf(stderr, ", ");
			}
			fprintf(stderr, "%s", OQS_KEM_alg_identifier(i));
		}
		fprintf(stderr, "\n");
		return EXIT_FAILURE;
	}

	print_system_info();

	char *alg_name = argv[1];
	if (!OQS_KEM_alg_is_enabled(alg_name)) {
		printf("KEM algorithm %s not enabled!\n", alg_name);
		return EXIT_FAILURE;
	}

	// Use system RNG in this program
	OQS_randombytes_switch_algorithm(OQS_RAND_alg_system);

	OQS_STATUS rc;

	rc = kem_test_correctness(alg_name, atoi(argv[2]));

	if (rc != OQS_SUCCESS) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
