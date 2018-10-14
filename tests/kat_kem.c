#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <oqs/oqs.h>

/* Displays hexadecimal strings */
void OQS_print_hex_string(const char *label, const uint8_t *str, size_t len) {
	printf("%-20s (%4zu bytes):  ", label, len);
	for (size_t i = 0; i < (len); i++) {
		printf("%02X", ((unsigned char *) (str))[i]);
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

OQS_STATUS kem_kat(const char *method_name) {

	uint8_t entropy_input[48];
	uint8_t seed[48];
	FILE *fh = NULL;
	OQS_KEM *kem = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *ciphertext = NULL;
	uint8_t *shared_secret_e = NULL;
	uint8_t *shared_secret_d = NULL;
	char filename[200];
	OQS_STATUS rc, ret = OQS_ERROR;
	int rv;

	kem = OQS_KEM_new(method_name);
	if (kem == NULL) {
		printf("[kem_kat] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	for (size_t i = 0; i < 48; i++) {
		entropy_input[i] = i;
	}

	rc = OQS_randombytes_switch_algorithm(OQS_RAND_alg_nist_kat);
	if (rc != OQS_SUCCESS) {
		goto err;
	}
	OQS_randombytes_nist_kat_init(entropy_input, NULL, 256);

	sprintf(filename, "kat_kem_rsp/%s.kat", method_name);

	fh = fopen(filename, "w");
	if (fh == NULL) {
		goto err;
	}

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
	if (fh != NULL) {
		fclose(fh);
	}
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

int main() {

	int ret = EXIT_SUCCESS;
	OQS_STATUS rc;

	int status;
#if defined(_WIN32)
	status = _mkdir("kat_kem_rsp");
#else
	status = mkdir("kat_kem_rsp", S_IRWXU);
#endif
	if (!((status == 0) || (errno == EEXIST))) {
		return EXIT_FAILURE;
	}

	for (size_t i = 0; i < OQS_KEM_algs_length; i++) {
		rc = kem_kat(OQS_KEM_alg_identifier(i));
		if (rc != OQS_SUCCESS) {
			ret = EXIT_FAILURE;
		}
	}

	return ret;
}
