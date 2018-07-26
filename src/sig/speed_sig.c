#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

#include "../common/ds_benchmark.h"

static OQS_STATUS sig_speed_wrapper(const char *method_name, int duration, bool printInfo) {

	OQS_SIG *sig = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *message = NULL;
	uint8_t *signed_message = NULL;
	size_t message_len = 50;
	size_t signed_message_len;
	OQS_STATUS ret = OQS_ERROR;

	sig = OQS_SIG_new(method_name);
	if (sig == NULL) {
		return OQS_SUCCESS;
	}

	public_key = malloc(sig->length_public_key);
	secret_key = malloc(sig->length_secret_key);
	message = malloc(message_len + sig->length_sig_overhead);
	signed_message = malloc(message_len + sig->length_sig_overhead);

	if ((public_key == NULL) || (secret_key == NULL) || (message == NULL) || (signed_message == NULL)) {
		fprintf(stderr, "ERROR: malloc failed\n");
		goto err;
	}

	OQS_randombytes(message, message_len);

	printf("%-30s | %10s | %14s | %15s | %10s | %16s | %10s\n", sig->method_name, "", "", "", "", "", "");
	TIME_OPERATION_SECONDS(OQS_SIG_keypair(sig, public_key, secret_key), "keypair", duration)
	TIME_OPERATION_SECONDS(OQS_SIG_sign(sig, signed_message, &signed_message_len, message, message_len, secret_key), "sign", duration)
	TIME_OPERATION_SECONDS(OQS_SIG_sign_open(sig, message, &message_len, signed_message, signed_message_len, public_key), "sign_open", duration)

	if (printInfo) {
		printf("public key bytes: %zu, secret key bytes: %zu, signed message bytes: %zu\n", sig->length_public_key, sig->length_secret_key, signed_message_len);
	}

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (sig != NULL) {
		OQS_MEM_secure_free(secret_key, sig->length_secret_key);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(signed_message);
	OQS_MEM_insecure_free(message);
	OQS_SIG_free(sig);

	return ret;
}

OQS_STATUS printAlgs() {
	for (size_t i = 0; i < OQS_SIG_algs_length; i++) {
		OQS_SIG *sig = OQS_SIG_new(OQS_SIG_alg_identifier(i));
		if (sig == NULL) {
			printf("%s (disabled)\n", OQS_SIG_alg_identifier(i));
		} else {
			printf("%s\n", OQS_SIG_alg_identifier(i));
		}
		OQS_SIG_free(sig);
	}
	return OQS_SUCCESS;
}

int main(int argc, char **argv) {

	int ret = EXIT_SUCCESS;
	OQS_STATUS rc;

	bool printUsage = false;
	int duration = 3;
	bool printSigInfo = false;

	OQS_SIG *single_sig = NULL;

	OQS_randombytes_switch_algorithm(OQS_RAND_alg_openssl);

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
			printSigInfo = true;
			continue;
		} else {
			single_sig = OQS_SIG_new(argv[i]);
			if (single_sig == NULL) {
				printUsage = true;
				break;
			}
		}
	}

	if (printUsage) {
		fprintf(stderr, "Usage: speed_sig <options> <alg>\n");
		fprintf(stderr, "\n");
		fprintf(stderr, "<options>\n");
		fprintf(stderr, "--algs             Print supported algorithms and terminate\n");
		fprintf(stderr, "--duration n\n");
		fprintf(stderr, " -d n              Run each speed test for approximately n seconds, default n=3\n");
		fprintf(stderr, "--help\n");
		fprintf(stderr, " -h                Print usage\n");
		fprintf(stderr, "--info\n");
		fprintf(stderr, " -i                Print info (sizes, security level) about each SIG\n");
		fprintf(stderr, "\n");
		fprintf(stderr, "<alg>              Only run the specified SIG method; must be one of the algorithms output by --algs\n");
		return EXIT_FAILURE;
	}

	printf("Compiler setup\n");
	printf("==============\n");
	printf("Date:     %s\n", OQS_COMPILE_DATE);
	printf("Compiler: %s (%s)\n", OQS_COMPILE_CC, OQS_COMPILE_CC_VERSION);
	printf("OS:       %s\n", OQS_COMPILE_UNAME);
	printf("CFLAGS:   %s\n", OQS_COMPILE_CFLAGS);
	printf("LDFLAGS:  %s\n", OQS_COMPILE_LDFLAGS);
	printf("RNG:      OpenSSL\n");
	printf("\n");

	printf("Speed test\n");
	printf("==========\n");

	PRINT_TIMER_HEADER
	if (single_sig != NULL) {
		rc = sig_speed_wrapper(single_sig->method_name, duration, printSigInfo);
		if (rc != OQS_SUCCESS) {
			ret = EXIT_FAILURE;
		}
	} else {
		for (size_t i = 0; i < OQS_SIG_algs_length; i++) {
			rc = sig_speed_wrapper(OQS_SIG_alg_identifier(i), duration, printSigInfo);
			if (rc != OQS_SUCCESS) {
				ret = EXIT_FAILURE;
			}
		}
	}
	PRINT_TIMER_FOOTER

	return ret;
}
