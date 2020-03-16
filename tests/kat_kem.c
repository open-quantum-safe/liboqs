// This KAT test only generates a subset of the NIST KAT files.
// To extract the subset from a submission file, use the command:
//     cat PQCkemKAT_whatever.rsp | head -n 8 | tail -n 6

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

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

static OQS_STATUS kem_kat(const char *method_name) {

	uint8_t entropy_input[48];
	uint8_t seed[48];
	FILE *fh = NULL;
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
		printf("[kem_kat] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	for (uint8_t i = 0; i < 48; i++) {
		entropy_input[i] = i;
	}

	rc = OQS_randombytes_switch_algorithm(OQS_RAND_alg_nist_kat);
	if (rc != OQS_SUCCESS) {
		goto err;
	}
	OQS_randombytes_nist_kat_init(entropy_input, NULL, 256);

	fh = stdout;

	fprintf(fh, "count = 0\n");
	OQS_randombytes(seed, 48);
	fprintBstr(fh, "seed = ", seed, 48);

	OQS_randombytes_nist_kat_init(seed, NULL, 256);

	public_key = malloc(kem->length_public_key);
	secret_key = malloc(kem->length_secret_key);
	ciphertext = malloc(kem->length_ciphertext);
	shared_secret_e = malloc(kem->length_shared_secret);
	shared_secret_d = malloc(kem->length_shared_secret);
	if ((public_key == NULL) || (secret_key == NULL) || (ciphertext == NULL) || (shared_secret_e == NULL) || (shared_secret_d == NULL)) {
		fprintf(stderr, "[kat_kem] %s ERROR: malloc failed!\n", method_name);
		goto err;
	}

	rc = OQS_KEM_keypair(kem, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[kat_kem] %s ERROR: OQS_KEM_keypair failed!\n", method_name);
		goto err;
	}
	fprintBstr(fh, "pk = ", public_key, kem->length_public_key);
	fprintBstr(fh, "sk = ", secret_key, kem->length_secret_key);

	rc = OQS_KEM_encaps(kem, ciphertext, shared_secret_e, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[kat_kem] %s ERROR: OQS_KEM_encaps failed!\n", method_name);
		goto err;
	}
	fprintBstr(fh, "ct = ", ciphertext, kem->length_ciphertext);
	fprintBstr(fh, "ss = ", shared_secret_e, kem->length_shared_secret);

	rc = OQS_KEM_decaps(kem, shared_secret_d, ciphertext, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[kat_kem] %s ERROR: OQS_KEM_decaps failed!\n", method_name);
		goto err;
	}

	rv = memcmp(shared_secret_e, shared_secret_d, kem->length_shared_secret);
	if (rv != 0) {
		fprintf(stderr, "[kat_kem] %s ERROR: shared secrets are not equal\n", method_name);
		OQS_print_hex_string("shared_secret_e", shared_secret_e, kem->length_shared_secret);
		OQS_print_hex_string("shared_secret_d", shared_secret_d, kem->length_shared_secret);
		goto err;
	}

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

algo_not_enabled:
	ret = OQS_SUCCESS;

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

	if (argc != 2) {
		fprintf(stderr, "Usage: kat_kem algname\n");
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
		return EXIT_FAILURE;
	}

	char *alg_name = argv[1];
	OQS_STATUS rc = kem_kat(alg_name);
	if (rc != OQS_SUCCESS) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
