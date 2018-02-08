#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

#include "../common/ds_benchmark.h"

static OQS_STATUS kem_speed_wrapper(enum OQS_KEM_alg_name alg_name, int duration, bool printInfo) {

	OQS_KEM *kem = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *ciphertext = NULL;
	uint8_t *shared_secret_e = NULL;
	uint8_t *shared_secret_d = NULL;
	OQS_STATUS ret = OQS_ERROR;

	kem = OQS_KEM_new(alg_name);
	if (kem == NULL) {
		return OQS_SUCCESS;
	}

	public_key = malloc(kem->length_public_key);
	secret_key = malloc(kem->length_secret_key);
	ciphertext = malloc(kem->length_ciphertext);
	shared_secret_e = malloc(kem->length_shared_secret);
	shared_secret_d = malloc(kem->length_shared_secret);

	if ((public_key == NULL) || (secret_key == NULL) || (ciphertext == NULL) || (shared_secret_e == NULL) || (shared_secret_d == NULL)) {
		fprintf(stderr, "ERROR: malloc failed\n");
		goto err;
	}

	printf("%-30s | %10s | %14s | %15s | %10s | %16s | %10s\n", kem->method_name, "", "", "", "", "", "");
	TIME_OPERATION_SECONDS(OQS_KEM_keypair(kem, public_key, secret_key), "keygen", duration)
	TIME_OPERATION_SECONDS(OQS_KEM_encaps(kem, ciphertext, shared_secret_e, public_key), "encaps", duration)
	TIME_OPERATION_SECONDS(OQS_KEM_decaps(kem, shared_secret_d, ciphertext, secret_key), "decaps", duration)

	if (printInfo) {
		printf("public key bytes: %zu, ciphertext bytes: %zu, secret key bytes: %zu, shared secret key bytes: %zu, NIST level: %zd, IND-CCA: %s\n", kem->length_public_key, kem->length_ciphertext, kem->length_secret_key, kem->length_shared_secret, kem->claimed_nist_level, kem->ind_cca ? "Y" : "N");
	}

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	free(public_key);
	free(secret_key);
	free(ciphertext);
	free(shared_secret_e);
	free(shared_secret_d);
	OQS_KEM_free(kem);

	return ret;
}

int main(int argc, char **argv) {

	int ret = EXIT_SUCCESS;
	OQS_STATUS rc;

	bool printUsage = false;
	int duration = 3;
	bool printKemInfo = false;

	for (int i = 1; i < argc; i++) {
		if ((strcmp(argv[i], "--duration") == 0) || (strcmp(argv[i], "-d") == 0)) {
			if (i < argc - 1) {
				duration = (int) strtol(argv[i + 1], NULL, 10);
				if (duration > 0) {
					i += 1;
					continue;
				}
			}
		} else if ((strcmp(argv[i], "--info") == 0) || (strcmp(argv[i], "-i") == 0)) {
			printKemInfo = true;
			continue;
		}
		printUsage = true;
		break;
	}

	if (printUsage) {
		fprintf(stderr, "Usage: speed_kem <options>\n");
		fprintf(stderr, "Options: \n");
		fprintf(stderr, "--duration n\n");
		fprintf(stderr, "-d n               Run each speed test for approximately n seconds, default n=3\n");
		fprintf(stderr, "--info\n");
		fprintf(stderr, "-i                 Print info (sizes, security level) about each KEM\n");
		return EXIT_FAILURE;
	}

	printf("Compiler setup\n");
	printf("==============\n");
	printf("Date:     %s\n", OQS_COMPILE_DATE);
	printf("Compiler: %s (%s)\n", OQS_COMPILE_CC, OQS_COMPILE_CC_VERSION);
	printf("OS:       %s\n", OQS_COMPILE_UNAME);
	printf("CFLAGS:   %s\n", OQS_COMPILE_CFLAGS);
	printf("LDFLAGS:  %s\n", OQS_COMPILE_LDFLAGS);
	printf("\n");

	printf("Speed test\n");
	printf("==========\n");

	PRINT_TIMER_HEADER
	for (int i = OQS_KEM_alg_default; i < OQS_KEM_alg_last; i++) {
		rc = kem_speed_wrapper(i, duration, printKemInfo);
		if (rc != OQS_SUCCESS) {
			ret = EXIT_FAILURE;
		}
	}
	PRINT_TIMER_FOOTER

	return ret;
}
