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

OQS_STATUS dummy_secure_storage(uint8_t *sk_buf, size_t sk_buf_len, void *context) {
	// suppress unused parameter warning
	(void)(sk_buf);
	(void)(sk_buf_len);
	(void)(context);
	return OQS_SUCCESS;
}

// reset secret key: some schemes fail to create a new secret key over a previous secret key
OQS_SIG_STFL_SECRET_KEY *reset_secret_key(OQS_SIG_STFL *sig, OQS_SIG_STFL_SECRET_KEY *secret_key) {
	OQS_SIG_STFL_SECRET_KEY_free(secret_key);
	secret_key = OQS_SIG_STFL_SECRET_KEY_new(sig->method_name);
	OQS_SIG_STFL_SECRET_KEY_SET_store_cb(secret_key, &dummy_secure_storage, secret_key);
	return secret_key;
}

static void fullcycle(OQS_SIG_STFL *sig, uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key, uint8_t *signature, size_t signature_len, uint8_t *message, size_t message_len) {
	if (OQS_SIG_STFL_keypair(sig, public_key, secret_key) != OQS_SUCCESS) {
		printf("keygen error. Exiting.\n");
		exit(-1);
	}
	if (OQS_SIG_STFL_sign(sig, signature, &signature_len, message, message_len, secret_key) != OQS_SUCCESS) {
		printf("sign error. Exiting.\n");
		exit(-1);
	}
	if (OQS_SIG_STFL_verify(sig, message, message_len, signature, signature_len, public_key) != OQS_SUCCESS) {
		printf("verify error. Exiting.\n");
		exit(-1);
	}
}

static OQS_STATUS sig_speed_wrapper(const char *method_name, uint64_t duration, bool printInfo, bool doFullCycle) {
	OQS_SIG_STFL *sig = NULL;
	uint8_t *public_key = NULL;
	OQS_SIG_STFL_SECRET_KEY *secret_key = NULL;
	uint8_t *message = NULL;
	uint8_t *signature = NULL;
	size_t message_len = 50;
	size_t signature_len = 0;
	OQS_STATUS ret = OQS_ERROR;

	// if keygen and signing is disabled then we can't benchmark and we simply return OQS_SUCCESS
#ifndef OQS_ALLOW_XMSS_KEY_AND_SIG_GEN
	if (strstr(method_name, "XMSS") != NULL) {
		printf("XMSS keygen and signing is not enabled.\n");
		return OQS_SUCCESS;
	}
#endif
#ifndef OQS_ALLOW_LMS_KEY_AND_SIG_GEN
	if (strstr(method_name, "LMS") != NULL) {
		printf("LMS keygen and signing is not enabled.\n");
		return OQS_SUCCESS;
	}
#endif

	sig = OQS_SIG_STFL_new(method_name);
	if (sig == NULL) {
		return OQS_SUCCESS;
	}

	secret_key = OQS_SIG_STFL_SECRET_KEY_new(sig->method_name);
	if (secret_key == NULL) {
		fprintf(stderr, "ERROR: OQS_SIG_STFL_SECRET_KEY_new failed\n");
		goto err;
	}
	// for LMS context must not be NULL
	OQS_SIG_STFL_SECRET_KEY_SET_store_cb(secret_key, &dummy_secure_storage, secret_key);

	public_key = malloc(sig->length_public_key);
	message = malloc(message_len);
	signature = malloc(sig->length_signature);

	if ((public_key == NULL) || (secret_key == NULL) || (message == NULL) || (signature == NULL)) {
		fprintf(stderr, "ERROR: malloc failed\n");
		goto err;
	}

	OQS_randombytes(message, message_len);

	printf("%-36s | %10s | %14s | %15s | %10s | %25s | %10s\n", sig->method_name, "", "", "", "", "", "");
	if (!doFullCycle) {
		// benchmark keygen: need to reset secret key between calls
		OQS_STATUS status = 0;
		TIME_OPERATION_SECONDS_MAXIT({ status = OQS_SIG_STFL_keypair(sig, public_key, secret_key); }, "keypair", duration, 1, {
			if (status != OQS_SUCCESS) {
				printf("keygen error. Exiting.\n");
				exit(-1);
			}
			secret_key = reset_secret_key(sig, secret_key);
		})
		// benchmark sign: need to generate new secret key after available signatures have been exhausted
		unsigned long long max_sigs;
		OQS_SIG_STFL_sigs_total(sig, &max_sigs, secret_key);
		TIME_OPERATION_SECONDS_MAXIT({ status = OQS_SIG_STFL_sign(sig, signature, &signature_len, message, message_len, secret_key); }, "sign", duration, max_sigs, {
			if (status != OQS_SUCCESS) {
				printf("sign error. Exiting.\n");
				exit(-1);
			}
			OQS_SIG_STFL_keypair(sig, public_key, secret_key);
		})
		// benchmark verification
		TIME_OPERATION_SECONDS({ OQS_SIG_STFL_verify(sig, message, message_len, signature, signature_len, public_key); }, "verify", duration)
	} else {
		// benchmark fullcycle: need to reset secret key between calls
		TIME_OPERATION_SECONDS_MAXIT({ fullcycle(sig, public_key, secret_key, signature, signature_len, message, message_len); }, "fullcycle", duration, 1, { secret_key = reset_secret_key(sig, secret_key); })
	}

	if (printInfo) {
		printf("public key bytes: %zu, secret key bytes: %zu, signature bytes: %zu\n", sig->length_public_key, sig->length_secret_key, sig->length_signature);
		if (signature_len != sig->length_signature) {
			printf("   Actual signature length returned (%zu) less than declared maximum signature length (%zu)\n", signature_len, sig->length_signature);
		}
	}

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;

cleanup:
	if (sig != NULL) {
		OQS_SIG_STFL_SECRET_KEY_free(secret_key);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(signature);
	OQS_MEM_insecure_free(message);
	OQS_SIG_STFL_free(sig);

	return ret;
}

static OQS_STATUS printAlgs(void) {
	for (size_t i = 0; i < OQS_SIG_STFL_algs_length; i++) {
		OQS_SIG_STFL *sig = OQS_SIG_STFL_new(OQS_SIG_STFL_alg_identifier(i));
		if (sig == NULL) {
			printf("%s (disabled)\n", OQS_SIG_STFL_alg_identifier(i));
		} else {
			printf("%s\n", OQS_SIG_STFL_alg_identifier(i));
		}
		OQS_SIG_STFL_free(sig);
	}
	return OQS_SUCCESS;
}

int main(int argc, char **argv) {

	int ret = EXIT_SUCCESS;
	OQS_STATUS rc;

	bool printUsage = false;
	uint64_t duration = 3;
	bool printSigInfo = false;
	bool doFullCycle = false;
	bool onlyMaxSigs10 = false;

	OQS_SIG_STFL *single_sig = NULL;

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
			printSigInfo = true;
			continue;
		} else if ((strcmp(argv[i], "--fullcycle") == 0) || (strcmp(argv[i], "-f") == 0)) {
			doFullCycle = true;
			continue;
		} else if ((strcmp(argv[i], "--limit10") == 0) || (strcmp(argv[i], "-l") == 0)) {
			onlyMaxSigs10 = true;
			continue;
		} else {
			single_sig = OQS_SIG_STFL_new(argv[i]);
			if (single_sig == NULL) {
				printUsage = true;
				break;
			}
		}
	}

	if (printUsage) {
		fprintf(stderr, "Usage: speed_sig_stfl <options> <alg>\n");
		fprintf(stderr, "\n");
		fprintf(stderr, "<options>\n");
		fprintf(stderr, "--algs             Print supported algorithms and terminate\n");
		fprintf(stderr, "--duration n\n");
		fprintf(stderr, " -d n              Run each speed test for approximately n seconds, default n=3\n");
		fprintf(stderr, "--help\n");
		fprintf(stderr, " -h                Print usage\n");
		fprintf(stderr, "--info\n");
		fprintf(stderr, " -i                Print info (sizes, security level) about each SIG\n");
		fprintf(stderr, "--fullcycle\n");
		fprintf(stderr, " -f                Test full keygen-sign-verify cycle of each SIG\n");
		fprintf(stderr, "--limit10          Test only algorithms with 2^10 max signatures\n");
		fprintf(stderr, " -l\n");
		fprintf(stderr, "\n");
		fprintf(stderr, "<alg>              Only run the specified SIG method; must be one of the algorithms output by --algs\n");
		OQS_destroy();
		return EXIT_FAILURE;
	}

	print_system_info();

	printf("Speed test\n");
	printf("==========\n");

	PRINT_TIMER_HEADER
	if (single_sig != NULL) {
		rc = sig_speed_wrapper(single_sig->method_name, duration, printSigInfo, doFullCycle);
		if (rc != OQS_SUCCESS) {
			ret = EXIT_FAILURE;
		}
		OQS_SIG_STFL_free(single_sig);

	} else {
		for (size_t i = 0; i < OQS_SIG_STFL_algs_length; i++) {
			if (onlyMaxSigs10 > 0 && strstr(OQS_SIG_STFL_alg_identifier(i), "_10") == NULL && strstr(OQS_SIG_STFL_alg_identifier(i), "H10") == NULL) {
				continue;
			}
			rc = sig_speed_wrapper(OQS_SIG_STFL_alg_identifier(i), duration, printSigInfo, doFullCycle);
			if (rc != OQS_SUCCESS) {
				ret = EXIT_FAILURE;
			}
		}
	}
	PRINT_TIMER_FOOTER
	OQS_destroy();

	return ret;
}
