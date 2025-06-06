// SPDX-License-Identifier: MIT

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

#if defined(OQS_USE_RASPBERRY_PI)
#define _OQS_RASPBERRY_PI
#endif
#if defined(OQS_SPEED_USE_ARM_PMU)
#define SPEED_USE_ARM_PMU
#endif
#include "ds_benchmark.h"
#include "system_info.c"

static void fullcycletest(OQS_KEM *kem, uint8_t *public_key, uint8_t *secret_key, uint8_t *ciphertext, uint8_t *shared_secret_e, uint8_t *shared_secret_d) {
	if (OQS_KEM_keypair(kem, public_key, secret_key) != OQS_SUCCESS) {
		printf("Error creating KEM key. Exiting.\n");
		exit(-1);
	}
	if (OQS_KEM_encaps(kem, ciphertext, shared_secret_e, public_key) != OQS_SUCCESS) {
		printf("Error during KEM encaps. Exiting.\n");
		exit(-1);
	}
	if (OQS_KEM_decaps(kem, shared_secret_d, ciphertext, secret_key) != OQS_SUCCESS) {
		printf("Error during KEM decaps. Exiting.\n");
		exit(-1);
	}

}

static OQS_STATUS kem_speed_wrapper(const char *method_name, uint64_t duration, bool printInfo, bool doFullCycle) {

	OQS_KEM *kem = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *ciphertext = NULL;
	uint8_t *shared_secret_e = NULL;
	uint8_t *shared_secret_d = NULL;
	OQS_STATUS ret = OQS_ERROR;

	kem = OQS_KEM_new(method_name);
	if (kem == NULL) {
		return OQS_SUCCESS;
	}

	public_key = OQS_MEM_malloc(kem->length_public_key);
	secret_key = OQS_MEM_malloc(kem->length_secret_key);
	ciphertext = OQS_MEM_malloc(kem->length_ciphertext);
	shared_secret_e = OQS_MEM_malloc(kem->length_shared_secret);
	shared_secret_d = OQS_MEM_malloc(kem->length_shared_secret);

	if ((public_key == NULL) || (secret_key == NULL) || (ciphertext == NULL) || (shared_secret_e == NULL) || (shared_secret_d == NULL)) {
		fprintf(stderr, "ERROR: OQS_MEM_malloc failed\n");
		goto err;
	}

	printf("%-36s | %10s | %14s | %15s | %10s | %25s | %10s\n", kem->method_name, "", "", "", "", "", "");
	if (!doFullCycle) {
		TIME_OPERATION_SECONDS(OQS_KEM_keypair(kem, public_key, secret_key), "keygen", duration)
		TIME_OPERATION_SECONDS(OQS_KEM_encaps(kem, ciphertext, shared_secret_e, public_key), "encaps", duration)
		TIME_OPERATION_SECONDS(OQS_KEM_decaps(kem, shared_secret_d, ciphertext, secret_key), "decaps", duration)
	} else {
		TIME_OPERATION_SECONDS(fullcycletest(kem, public_key, secret_key, ciphertext, shared_secret_e, shared_secret_d), "fullcycletest", duration)
	}

	if (printInfo) {
		printf("public key bytes: %zu, ciphertext bytes: %zu, secret key bytes: %zu, shared secret key bytes: %zu, NIST level: %d, IND-CCA: %s\n", kem->length_public_key, kem->length_ciphertext, kem->length_secret_key, kem->length_shared_secret, kem->claimed_nist_level, kem->ind_cca ? "Y" : "N");
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

static OQS_STATUS printAlgs(void) {
	for (size_t i = 0; i < OQS_KEM_algs_length; i++) {
		OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_identifier(i));
		if (kem == NULL) {
			printf("%s (disabled)\n", OQS_KEM_alg_identifier(i));
		} else {
			printf("%s\n", OQS_KEM_alg_identifier(i));
		}
		OQS_KEM_free(kem);
	}
	return OQS_SUCCESS;
}

int main(int argc, char **argv) {

	int ret = EXIT_SUCCESS;
	OQS_STATUS rc;

	bool printUsage = false;
	uint64_t duration = 3;
	bool printKemInfo = false;
	bool doFullCycle = false;

	OQS_KEM *single_kem = NULL;

	OQS_init();
#ifdef OQS_USE_OPENSSL
	rc = OQS_randombytes_switch_algorithm(OQS_RAND_alg_openssl);
	if (rc != OQS_SUCCESS) {
		printf("Could not generate random data with OpenSSL RNG\n");
		OQS_destroy();
		return EXIT_FAILURE;
	}
#endif

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--algs") == 0) {
			rc = printAlgs();
			if (rc == OQS_SUCCESS) {
				OQS_destroy();
				return EXIT_SUCCESS;
			} else {
				OQS_destroy();
				return EXIT_FAILURE;
			}
		} else if ((strcmp(argv[i], "--duration") == 0) || (strcmp(argv[i], "-d") == 0)) {
			if (i < argc - 1) {
				duration = (uint64_t)strtol(argv[i + 1], NULL, 10);
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
		} else if ((strcmp(argv[i], "--fullcycle") == 0) || (strcmp(argv[i], "-f") == 0)) {
			doFullCycle = true;
			continue;
		} else {
			single_kem = OQS_KEM_new(argv[i]);
			if (single_kem == NULL) {
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
		fprintf(stderr, "--fullcycle\n");
		fprintf(stderr, " -f                Do full keygen-encaps-decaps cycle for each KEM\n");
		fprintf(stderr, "\n");
		fprintf(stderr, "<alg>              Only run the specified KEM method; must be one of the algorithms output by --algs\n");
		return EXIT_FAILURE;
	}

	print_system_info();

	printf("Speed test\n");
	printf("==========\n");

	PRINT_TIMER_HEADER
	if (single_kem != NULL) {
		rc = kem_speed_wrapper(single_kem->method_name, duration, printKemInfo, doFullCycle);
		if (rc != OQS_SUCCESS) {
			ret = EXIT_FAILURE;
		}
		OQS_KEM_free(single_kem);
	} else {
		for (size_t i = 0; i < OQS_KEM_algs_length; i++) {
			rc = kem_speed_wrapper(OQS_KEM_alg_identifier(i), duration, printKemInfo, doFullCycle);
			if (rc != OQS_SUCCESS) {
				ret = EXIT_FAILURE;
			}
		}
	}
	PRINT_TIMER_FOOTER
	OQS_destroy();

	return ret;
}
