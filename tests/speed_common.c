// SPDX-License-Identifier: MIT

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

#include <oqs/aes.h>
#include <oqs/sha2.h>
#include <oqs/sha3.h>

static const uint8_t test_aes128_key[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
static const uint8_t test_aes256_key[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

static OQS_STATUS speed_aes128(uint64_t duration, size_t message_len) {
	uint8_t *message = NULL;
	uint8_t *ciphertext = NULL;
	void *schedule = NULL;

	message = malloc(message_len);
	if (message == NULL) {
		fprintf(stderr, "ERROR: malloc failed\n");
		return OQS_ERROR;
	}
	ciphertext = malloc(message_len);
	if (ciphertext == NULL) {
		OQS_MEM_insecure_free(message);
		fprintf(stderr, "ERROR: malloc failed\n");
		return OQS_ERROR;
	}

	OQS_randombytes(message, message_len);

	TIME_OPERATION_SECONDS({ OQS_AES128_ECB_load_schedule(test_aes128_key, &schedule); OQS_AES128_free_schedule(schedule); }, "OQS_AES128_ECB_load+free_sch", duration);

	OQS_AES128_ECB_load_schedule(test_aes128_key, &schedule);
	TIME_OPERATION_SECONDS(OQS_AES128_ECB_enc_sch(message, message_len, schedule, ciphertext), "OQS_AES128_ECB_enc_sch", duration);
	TIME_OPERATION_SECONDS(OQS_AES128_ECB_enc(message, message_len, test_aes128_key, ciphertext), "OQS_AES128_ECB_enc", duration);
	OQS_AES128_free_schedule(schedule);

	OQS_MEM_insecure_free(message);
	OQS_MEM_insecure_free(ciphertext);

	return OQS_SUCCESS;
}

static OQS_STATUS speed_aes256(uint64_t duration, size_t message_len) {
	uint8_t *message = NULL;
	uint8_t *ciphertext = NULL;
	uint8_t nonce[12] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	void *schedule = NULL;

	message = malloc(message_len);
	if (message == NULL) {
		fprintf(stderr, "ERROR: malloc failed\n");
		return OQS_ERROR;
	}
	ciphertext = malloc(message_len);
	if (ciphertext == NULL) {
		OQS_MEM_insecure_free(message);
		fprintf(stderr, "ERROR: malloc failed\n");
		return OQS_ERROR;
	}

	OQS_randombytes(message, message_len);

	TIME_OPERATION_SECONDS({ OQS_AES256_ECB_load_schedule(test_aes256_key, &schedule); OQS_AES256_free_schedule(schedule); }, "OQS_AES256_ECB_load+free_sch", duration);

	OQS_AES256_ECB_load_schedule(test_aes256_key, &schedule);
	TIME_OPERATION_SECONDS(OQS_AES256_ECB_enc_sch(message, message_len, schedule, ciphertext), "OQS_AES256_ECB_enc_sch", duration);

	TIME_OPERATION_SECONDS(OQS_AES256_ECB_enc(message, message_len, test_aes256_key, ciphertext), "OQS_AES256_ECB_enc", duration);
	OQS_AES256_free_schedule(schedule);

	TIME_OPERATION_SECONDS({ OQS_AES256_CTR_inc_init(test_aes256_key, &schedule); OQS_AES256_CTR_inc_iv(nonce, 12, schedule); OQS_AES256_free_schedule(schedule); }, "OQS_AES256_CTR_init+iv+free", duration);

	OQS_AES256_CTR_inc_init(test_aes256_key, &schedule);
	OQS_AES256_CTR_inc_iv(nonce, 12, schedule);

	TIME_OPERATION_SECONDS(OQS_AES256_CTR_inc_stream_iv(nonce, 12, schedule, ciphertext, message_len), "OQS_AES256_CTR_inc_stream_iv", duration);

	TIME_OPERATION_SECONDS(OQS_AES256_CTR_inc_stream_blks(schedule, ciphertext, message_len / 16), "OQS_AES256_CTR_inc_stream_blks", duration);

	OQS_AES256_free_schedule(schedule);

	OQS_MEM_insecure_free(message);
	OQS_MEM_insecure_free(ciphertext);

	return OQS_SUCCESS;
}

static OQS_STATUS speed_sha256(uint64_t duration, size_t message_len) {
	uint8_t *message = NULL;
	uint8_t output[32];

	message = malloc(message_len);
	if (message == NULL) {
		fprintf(stderr, "ERROR: malloc failed\n");
		return OQS_ERROR;
	}

	OQS_randombytes(message, message_len);

	// main SHA2-256 API
	TIME_OPERATION_SECONDS(OQS_SHA2_sha256(output, message, message_len), "OQS_SHA2_sha256", duration);

	OQS_MEM_insecure_free(message);

	return OQS_SUCCESS;
}

static OQS_STATUS speed_sha384(uint64_t duration, size_t message_len) {
	uint8_t *message = NULL;
	uint8_t output[48];

	message = malloc(message_len);
	if (message == NULL) {
		fprintf(stderr, "ERROR: malloc failed\n");
		return OQS_ERROR;
	}

	OQS_randombytes(message, message_len);

	// main SHA2-384 API
	TIME_OPERATION_SECONDS(OQS_SHA2_sha384(output, message, message_len), "OQS_SHA2_sha384", duration);

	OQS_MEM_insecure_free(message);

	return OQS_SUCCESS;
}

static OQS_STATUS speed_sha512(uint64_t duration, size_t message_len) {
	uint8_t *message = NULL;
	uint8_t output[64];

	message = malloc(message_len);
	if (message == NULL) {
		fprintf(stderr, "ERROR: malloc failed\n");
		return OQS_ERROR;
	}

	OQS_randombytes(message, message_len);

	// main SHA2-512 API
	TIME_OPERATION_SECONDS(OQS_SHA2_sha512(output, message, message_len), "OQS_SHA2_sha512", duration);

	OQS_MEM_insecure_free(message);

	return OQS_SUCCESS;
}

static OQS_STATUS speed_sha3(uint64_t duration, size_t message_len) {
	uint8_t *message = NULL;

	message = malloc(message_len);
	if (message == NULL) {
		fprintf(stderr, "ERROR: malloc failed\n");
		return OQS_ERROR;
	}

	OQS_randombytes(message, message_len);

	// main SHA3-256 API
	uint8_t sha3_256_output[32];
	TIME_OPERATION_SECONDS(OQS_SHA3_sha3_256(sha3_256_output, message, message_len), "OQS_SHA3_sha3_256", duration);

	// main SHA3-384 API
	uint8_t sha3_384_output[48];
	TIME_OPERATION_SECONDS(OQS_SHA3_sha3_384(sha3_384_output, message, message_len), "OQS_SHA3_sha3_384", duration);

	// main SHA3-512 APT
	uint8_t sha3_512_output[64];
	TIME_OPERATION_SECONDS(OQS_SHA3_sha3_512(sha3_512_output, message, message_len), "OQS_SHA3_sha3_512", duration);


	OQS_MEM_insecure_free(message);

	return OQS_SUCCESS;
}

static OQS_STATUS speed_shake128(uint64_t duration, size_t message_len, size_t output_len) {
	uint8_t *message = NULL;
	uint8_t *output = NULL;

	message = malloc(message_len);
	if (message == NULL) {
		fprintf(stderr, "ERROR: malloc failed\n");
		return OQS_ERROR;
	}
	output = malloc(output_len);
	if (output == NULL) {
		OQS_MEM_insecure_free(message);
		fprintf(stderr, "ERROR: malloc failed\n");
		return OQS_ERROR;
	}

	OQS_randombytes(message, message_len);

	// main SHAKE-128 API
	TIME_OPERATION_SECONDS(OQS_SHA3_shake128(output, output_len, message, message_len), "OQS_SHA3_shake128", duration);

	OQS_MEM_insecure_free(message);
	OQS_MEM_insecure_free(output);

	return OQS_SUCCESS;
}

static OQS_STATUS speed_shake256(uint64_t duration, size_t message_len, size_t output_len) {
	uint8_t *message = NULL;
	uint8_t *output = NULL;

	message = malloc(message_len);
	if (message == NULL) {
		fprintf(stderr, "ERROR: malloc failed\n");
		return OQS_ERROR;
	}
	output = malloc(output_len);
	if (output == NULL) {
		OQS_MEM_insecure_free(message);
		fprintf(stderr, "ERROR: malloc failed\n");
		return OQS_ERROR;
	}

	OQS_randombytes(message, message_len);

	// main SHAKE-256 API
	TIME_OPERATION_SECONDS(OQS_SHA3_shake256(output, output_len, message, message_len), "OQS_SHA3_shake256", duration);

	OQS_MEM_insecure_free(message);
	OQS_MEM_insecure_free(output);

	return OQS_SUCCESS;
}

static OQS_STATUS printAlgs(void) {
	printf("aes128\n");
	printf("aes256\n");
	printf("sha256\n");
	printf("sha384\n");
	printf("sha512\n");
	printf("sha3\n");
	printf("shake128\n");
	printf("shake256\n");
	return OQS_SUCCESS;
}

int main(int argc, char **argv) {

	int ret = EXIT_SUCCESS;
	OQS_STATUS rc;

	bool printUsage = false;
	uint64_t duration = 1;
	size_t message_len = 64;
	size_t output_len = 64;
	char *single_alg = NULL;

	OQS_init();
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
		}
		if ((strcmp(argv[i], "--duration") == 0) || (strcmp(argv[i], "-d") == 0)) {
			if (i < argc - 1) {
				duration = (uint64_t)strtol(argv[i + 1], NULL, 10);
				if (duration > 0) {
					i += 1;
					continue;
				}
			}
		} else if ((strcmp(argv[i], "--msglen") == 0) || (strcmp(argv[i], "-m") == 0)) {
			if (i < argc - 1) {
				message_len = (size_t)strtol(argv[i + 1], NULL, 10);
				if (message_len > 0) {
					i += 1;
					continue;
				}
			}
		} else if ((strcmp(argv[i], "--outlen") == 0) || (strcmp(argv[i], "-o") == 0)) {
			if (i < argc - 1) {
				output_len = (size_t)strtol(argv[i + 1], NULL, 10);
				if (output_len > 0) {
					i += 1;
					continue;
				}
			}
		} else if ((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i], "-h") == 0)) {
			printUsage = true;
			break;
		} else {
			single_alg = argv[i];
		}
	}

	if (printUsage) {
		fprintf(stderr, "Usage: speed_common <options>\n");
		fprintf(stderr, "\n");
		fprintf(stderr, "<options>\n");
		fprintf(stderr, "--algs             Print common algorithms and terminate\n");
		fprintf(stderr, "--duration n\n");
		fprintf(stderr, "-d n               Run each speed test for approximately n seconds, default n=1\n");
		fprintf(stderr, "--msglen n\n");
		fprintf(stderr, "-m n               Specify the length of the message in bytes (must be multiple of 16 if AES is run) to run the common operations on, default n=64\n");
		fprintf(stderr, "--outlen n \n");
		fprintf(stderr, "-o n               Specify the length of the output in bytes for algorithms that support variable output length (like SHAKE), default n=64\n");
		fprintf(stderr, "--help\n");
		fprintf(stderr, "-h                 Print usage\n");
		fprintf(stderr, "\n");
		fprintf(stderr, "<alg>              Only run the specified algorithm. Must be one of the algorithms output by --algs\n");
		OQS_destroy();
		return EXIT_FAILURE;
	}

	print_system_info();

	printf("Speed test\n");
	printf("==========\n");

	PRINT_TIMER_HEADER
	if (single_alg != NULL) {
		if (strcmp(single_alg, "aes128") == 0) {
			if ( message_len % 16 != 0 ) {
				fprintf(stderr, "ERROR: message length must be multiple of 16 for AES\n");
				ret = EXIT_FAILURE;
			} else {
				rc = speed_aes128(duration, message_len);
				if (rc != OQS_SUCCESS) {
					ret = EXIT_FAILURE;
				}
			}
		} else if (strcmp(single_alg, "aes256") == 0) {
			if ( message_len % 16 != 0 ) {
				fprintf(stderr, "ERROR: message length must be multiple of 16 for AES\n");
				ret = EXIT_FAILURE;
			} else {
				rc = speed_aes256(duration, message_len);
				if (rc != OQS_SUCCESS) {
					ret = EXIT_FAILURE;
				}
			}
		} else if (strcmp(single_alg, "sha256") == 0) {
			rc = speed_sha256(duration, message_len);
			if (rc != OQS_SUCCESS) {
				ret = EXIT_FAILURE;
			}
		} else if (strcmp(single_alg, "sha384") == 0) {
			rc = speed_sha384(duration, message_len);
			if (rc != OQS_SUCCESS) {
				ret = EXIT_FAILURE;
			}
		} else if (strcmp(single_alg, "sha512") == 0) {
			rc = speed_sha512(duration, message_len);
			if (rc != OQS_SUCCESS) {
				ret = EXIT_FAILURE;
			}
		} else if (strcmp(single_alg, "sha3") == 0) {
			rc = speed_sha3(duration, message_len);
			if (rc != OQS_SUCCESS) {
				ret = EXIT_FAILURE;
			}
		} else if (strcmp(single_alg, "shake128") == 0) {
			rc = speed_shake128(duration, message_len, output_len);
			if (rc != OQS_SUCCESS) {
				ret = EXIT_FAILURE;
			}
		} else if (strcmp(single_alg, "shake256") == 0) {
			rc = speed_shake256(duration, message_len, output_len);
			if (rc != OQS_SUCCESS) {
				ret = EXIT_FAILURE;
			}
		} else {
			fprintf(stderr, "ERROR: Algorithm not recognized. Try --help for help or --algs for a list of algorithms\n");
			return EXIT_FAILURE;
		}
	} else {
		if ( message_len % 16 != 0 ) {
			fprintf(stderr, "ERROR: message length must be multiple of 16 for AES\n");
			ret = EXIT_FAILURE;
		} else {
			rc = speed_aes128(duration, message_len);
			if (rc != OQS_SUCCESS) {
				ret = EXIT_FAILURE;
			}
			rc = speed_aes256(duration, message_len);
			if (rc != OQS_SUCCESS) {
				ret = EXIT_FAILURE;
			}
		}

		rc = speed_sha256(duration, message_len);
		if (rc != OQS_SUCCESS) {
			ret = EXIT_FAILURE;
		}

		rc = speed_sha384(duration, message_len);
		if (rc != OQS_SUCCESS) {
			ret = EXIT_FAILURE;
		}

		rc = speed_sha512(duration, message_len);
		if (rc != OQS_SUCCESS) {
			ret = EXIT_FAILURE;
		}

		rc = speed_sha3(duration, message_len);
		if (rc != OQS_SUCCESS) {
			ret = EXIT_FAILURE;
		}

		rc = speed_shake128(duration, message_len, output_len);
		if (rc != OQS_SUCCESS) {
			ret = EXIT_FAILURE;
		}

		rc = speed_shake256(duration, message_len, output_len);
		if (rc != OQS_SUCCESS) {
			ret = EXIT_FAILURE;
		}

	}
	PRINT_TIMER_FOOTER

	OQS_destroy();
	return ret;
}
