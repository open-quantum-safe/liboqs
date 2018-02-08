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

OQS_STATUS printAlgs() {
	for (int i = OQS_KEM_alg_default; i < OQS_KEM_alg_last; i++) {
		OQS_KEM *kem = OQS_KEM_new(i);
		if (kem == NULL) {
			return OQS_ERROR;
		}
		printf("%s\n", kem->method_name);
		OQS_KEM_free(kem);
	}
	return OQS_SUCCESS;

}

int main(int argc, char **argv) {

	int ret = EXIT_SUCCESS;
	OQS_STATUS rc;

	bool printUsage = false;
	int duration = 3;
	bool printKemInfo = false;
	enum OQS_KEM_alg_name single_alg = OQS_KEM_alg_last;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--algs") == 0) {
			rc = printAlgs();
			if (rc == OQS_SUCCESS) {
				return EXIT_SUCCESS;
			} else {
				return EXIT_FAILURE;
			}
		} else if ((strcmp(argv[i], "--duration") == 0) || (strcmp(argv[i], "-d") == 0)) {
			if (i < argc - 1) {
				duration = (int) strtol(argv[i + 1], NULL, 10);
				if (duration > 0) {
					i += 1;
					continue;
				}
			}
		} else if ((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i], "-h") == 0)) {
			printUsage = true;
			break;
		} else if ((strcmp(argv[i], "--info") == 0) || (strcmp(argv[i], "-i") == 0)) {
			printKemInfo = true;
			continue;
		} else {
			for (int alg_name = OQS_KEM_alg_default; alg_name < OQS_KEM_alg_last; alg_name++) {
				OQS_KEM *kem = OQS_KEM_new(alg_name);
				if (kem == NULL) {
					return EXIT_FAILURE;
				}
				if (strcmp(argv[i], kem->method_name) == 0) {
					single_alg = alg_name;
				}
				OQS_KEM_free(kem);
			}
			// didn't find one?
			if (single_alg == OQS_KEM_alg_last) {
				printUsage = true;
				break;
			}
		}
	}

	if (printUsage) {
		fprintf(stderr, "Usage: speed_kem <options> <alg>\n");
		fprintf(stderr, "\n");
		fprintf(stderr, "<options>\n");
		fprintf(stderr, "--algs             Print supported algorithms and terminate\n");
		fprintf(stderr, "--duration n\n");
		fprintf(stderr, " -d n              Run each speed test for approximately n seconds, default n=3\n");
		fprintf(stderr, "--help\n");
		fprintf(stderr, " -h                Print usage\n");
		fprintf(stderr, "--info\n");
		fprintf(stderr, " -i                Print info (sizes, security level) about each KEM\n");
		fprintf(stderr, "\n");
		fprintf(stderr, "<alg>              Only run the specified KEM method; must be one of the algorithms output by --algs\n");
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
	if (single_alg != OQS_KEM_alg_last) {
		rc = kem_speed_wrapper(single_alg, duration, printKemInfo);
		if (rc != OQS_SUCCESS) {
			ret = EXIT_FAILURE;
		}
	} else {
		for (int i = OQS_KEM_alg_default; i < OQS_KEM_alg_last; i++) {
			rc = kem_speed_wrapper(i, duration, printKemInfo);
			if (rc != OQS_SUCCESS) {
				ret = EXIT_FAILURE;
			}
		}
	}
	PRINT_TIMER_FOOTER

	return ret;
}
