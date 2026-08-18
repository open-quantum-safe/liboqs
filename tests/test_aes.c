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

/* Displays hexadecimal strings */
static void OQS_print_hex_string(const char *label, const uint8_t *str, size_t len) {
	printf("%-20s (%4zu bytes):  ", label, len);
	for (size_t i = 0; i < (len); i++) {
		printf("%02X", str[i]);
	}
	printf("\n");
}

#define BENCH_DURATION 1

static const uint8_t test_aes128_plaintext[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
static const uint8_t test_aes128_key[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
static const uint8_t test_aes128_ciphertext[] = {0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30, 0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a};

static const uint8_t test_aes256_plaintext[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
static const uint8_t test_aes256_key[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
static const uint8_t test_aes256_ciphertext[] = {0x8e, 0xa2, 0xb7, 0xca, 0x51, 0x67, 0x45, 0xbf, 0xea, 0xfc, 0x49, 0x90, 0x4b, 0x49, 0x60, 0x89};

static int test_aes128_correctness(void) {
	uint8_t derived_ciphertext[16];
	void *schedule = NULL;
	OQS_AES128_ECB_load_schedule(test_aes128_key, &schedule);
	OQS_AES128_ECB_enc_sch(test_aes128_plaintext, sizeof(test_aes128_plaintext), schedule, derived_ciphertext);
	if (memcmp(test_aes128_ciphertext, derived_ciphertext, 16) != 0) {
		printf("test_aes128_correctness ciphertext does not match\n");
		OQS_print_hex_string("expected ciphertext", test_aes128_ciphertext, 16);
		OQS_print_hex_string("derived  ciphertext", derived_ciphertext, 16);
		return EXIT_FAILURE;
	}
	OQS_AES128_free_schedule(schedule);
	return EXIT_SUCCESS;
}

// test vector #3 from https://tools.ietf.org/html/rfc3686#section-6
static const uint8_t test_aes128ctr_key[] = {0x76, 0x91, 0xBE, 0x03, 0x5E, 0x50, 0x20, 0xA8, 0xAC, 0x6E, 0x61, 0x85, 0x29, 0xF9, 0xA0, 0xDC};
static const uint8_t test_aes128ctr_iv[] = {0x00, 0xE0, 0x01, 0x7B, 0x27, 0x77, 0x7F, 0x3F, 0x4A, 0x17, 0x86, 0xF0, 0x00, 0x00, 0x00, 0x01};
static const uint8_t test_aes128ctr_plaintext[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23};
static const uint8_t test_aes128ctr_ciphertext[] = {0xC1, 0xCF, 0x48, 0xA8, 0x9F, 0x2F, 0xFD, 0xD9, 0xCF, 0x46, 0x52, 0xE9, 0xEF, 0xDB, 0x72, 0xD7, 0x45, 0x40, 0xA4, 0x2B, 0xDE, 0x6D, 0x78, 0x36, 0xD5, 0x9A, 0x5C, 0xEA, 0xAE, 0xF3, 0x10, 0x53, 0x25, 0xB2, 0x07, 0x2F};

static int test_aes128ctr_correctness(void) {
	uint8_t derived_ciphertext[36];
	void *schedule = NULL;
	OQS_AES128_CTR_inc_init(test_aes128ctr_key, &schedule);
	OQS_AES128_CTR_inc_stream_iv(test_aes128ctr_iv, sizeof(test_aes128ctr_iv), schedule, derived_ciphertext, sizeof(derived_ciphertext));
	for (size_t i = 0; i < sizeof(derived_ciphertext); i++) {
		derived_ciphertext[i] ^= test_aes128ctr_plaintext[i];
	}
	if (memcmp(test_aes128ctr_ciphertext, derived_ciphertext, 36) != 0) {
		printf("test_aes128ctr_correctness ciphertext does not match\n");
		OQS_print_hex_string("expected ciphertext", test_aes128ctr_ciphertext, 36);
		OQS_print_hex_string("derived  ciphertext", derived_ciphertext, 36);
		return EXIT_FAILURE;
	}
	OQS_AES128_free_schedule(schedule);
	return EXIT_SUCCESS;
}

static int test_aes256_correctness(void) {
	uint8_t derived_ciphertext[16];
	void *schedule = NULL;
	OQS_AES256_ECB_load_schedule(test_aes256_key, &schedule);
	OQS_AES256_ECB_enc_sch(test_aes256_plaintext, sizeof(test_aes256_plaintext), schedule, derived_ciphertext);
	if (memcmp(test_aes256_ciphertext, derived_ciphertext, 16) != 0) {
		printf("test_aes256_correctness ciphertext does not match\n");
		OQS_print_hex_string("expected ciphertext", test_aes256_ciphertext, 16);
		OQS_print_hex_string("derived  ciphertext", derived_ciphertext, 16);
		return EXIT_FAILURE;
	}
	OQS_AES256_free_schedule(schedule);
	return EXIT_SUCCESS;
}

// test vector #9 from https://tools.ietf.org/html/rfc3686#section-6
static const uint8_t test_aes256ctr_key[] = {0xFF, 0x7A, 0x61, 0x7C, 0xE6, 0x91, 0x48, 0xE4, 0xF1, 0x72, 0x6E, 0x2F, 0x43, 0x58, 0x1D, 0xE2, 0xAA, 0x62, 0xD9, 0xF8, 0x05, 0x53, 0x2E, 0xDF, 0xF1, 0xEE, 0xD6, 0x87, 0xFB, 0x54, 0x15, 0x3D};
static const uint8_t test_aes256ctr_iv[] = {0x00, 0x1C, 0xC5, 0xB7, 0x51, 0xA5, 0x1D, 0x70, 0xA1, 0xC1, 0x11, 0x48, 0x00, 0x00, 0x00, 0x01};
static const uint8_t test_aes256ctr_plaintext[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23};
static const uint8_t test_aes256ctr_ciphertext[] = {0xEB, 0x6C, 0x52, 0x82, 0x1D, 0x0B, 0xBB, 0xF7, 0xCE, 0x75, 0x94, 0x46, 0x2A, 0xCA, 0x4F, 0xAA, 0xB4, 0x07, 0xDF, 0x86, 0x65, 0x69, 0xFD, 0x07, 0xF4, 0x8C, 0xC0, 0xB5, 0x83, 0xD6, 0x07, 0x1F, 0x1E, 0xC0, 0xE6, 0xB8};

static int test_aes256ctr_correctness(void) {
	uint8_t derived_ciphertext[36];
	void *schedule = NULL;
	OQS_AES256_CTR_inc_init(test_aes256ctr_key, &schedule);
	OQS_AES256_CTR_inc_stream_iv(test_aes256ctr_iv, sizeof(test_aes256ctr_iv), schedule, derived_ciphertext, sizeof(derived_ciphertext));
	for (size_t i = 0; i < sizeof(derived_ciphertext); i++) {
		derived_ciphertext[i] ^= test_aes256ctr_plaintext[i];
	}
	if (memcmp(test_aes256ctr_ciphertext, derived_ciphertext, 36) != 0) {
		printf("test_aes256ctr_correctness ciphertext does not match\n");
		OQS_print_hex_string("expected ciphertext", test_aes256ctr_ciphertext, 36);
		OQS_print_hex_string("derived  ciphertext", derived_ciphertext, 36);
		return EXIT_FAILURE;
	}
	OQS_AES256_free_schedule(schedule);
	return EXIT_SUCCESS;
}

static void speed_aes128(void) {
	uint8_t ciphertext[16];
	void *schedule = NULL, *schedule_dec = NULL;
	TIME_OPERATION_SECONDS({ OQS_AES128_ECB_load_schedule(test_aes128_key, &schedule); OQS_AES128_free_schedule(schedule); }, "OQS_AES128_ECB_load+free_sch", BENCH_DURATION);

	OQS_AES128_ECB_load_schedule(test_aes128_key, &schedule);
	TIME_OPERATION_SECONDS(OQS_AES128_ECB_enc_sch(test_aes128_plaintext, sizeof(test_aes128_plaintext), schedule, ciphertext), "OQS_AES128_ECB_enc_sch", BENCH_DURATION);
	TIME_OPERATION_SECONDS(OQS_AES128_ECB_enc(test_aes128_plaintext, sizeof(test_aes128_plaintext), test_aes128_key, ciphertext), "OQS_AES128_ECB_enc", BENCH_DURATION);
	OQS_AES128_free_schedule(schedule);
	OQS_AES128_free_schedule(schedule_dec);
}

static void speed_aes256(void) {
	uint8_t ciphertext[16];
	void *schedule = NULL, *schedule_dec = NULL;
	TIME_OPERATION_SECONDS({ OQS_AES256_ECB_load_schedule(test_aes256_key, &schedule); OQS_AES256_free_schedule(schedule); }, "OQS_AES256_ECB_load+free_sch", BENCH_DURATION);

	OQS_AES256_ECB_load_schedule(test_aes256_key, &schedule);
	TIME_OPERATION_SECONDS(OQS_AES256_ECB_enc_sch(test_aes256_plaintext, sizeof(test_aes256_plaintext), schedule, ciphertext), "OQS_AES256_ECB_enc_sch", BENCH_DURATION);

	TIME_OPERATION_SECONDS(OQS_AES256_ECB_enc(test_aes256_plaintext, sizeof(test_aes256_plaintext), test_aes256_key, ciphertext), "OQS_AES256_ECB_enc", BENCH_DURATION);
	OQS_AES256_free_schedule(schedule);
	OQS_AES256_free_schedule(schedule_dec);
}

extern struct OQS_AES_callbacks aes_default_callbacks;

static bool aes_callback_called = false;
static void override_AES128_ECB_load_schedule(const uint8_t *key, void **_schedule) {
	aes_callback_called = true;
	aes_default_callbacks.AES128_ECB_load_schedule(key, _schedule);
}

int main(int argc, char **argv) {
	bool bench = false;

	struct OQS_AES_callbacks callbacks = aes_default_callbacks;
	callbacks.AES128_ECB_load_schedule = override_AES128_ECB_load_schedule;
	OQS_AES_set_callbacks(&callbacks);

	OQS_init();
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (strcmp(argv[i], "--bench") == 0 || strcmp(argv[i], "-b") == 0) {
				bench = true;
			} else {
				printf("Usage: ./test_aes [options]\n");
				printf("\nOptions:\n");
				printf("  --bench, -b\n");
				printf("    Run benchmarks\n");
				if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "-help") == 0) || (strcmp(argv[i], "--help") == 0)) {
					OQS_destroy();
					return EXIT_SUCCESS;
				} else {
					OQS_destroy();
					return EXIT_FAILURE;
				}
			}
		}
	}

	print_system_info();

	printf("=== test_aes correctness ===\n");
	if (test_aes128_correctness() != EXIT_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	if (test_aes128ctr_correctness() != EXIT_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}

	if (test_aes256_correctness() != EXIT_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	if (test_aes256ctr_correctness() != EXIT_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}

	if (!aes_callback_called) {
		printf("AES callback was not called\n");
		OQS_destroy();
		return EXIT_FAILURE;
	}

	printf("Tests passed.\n\n");

	if (bench) {
		printf("=== test_aes performance ===\n");
		PRINT_TIMER_HEADER
		speed_aes128();
		speed_aes256();
		PRINT_TIMER_FOOTER
	}
	OQS_destroy();

	return EXIT_SUCCESS;
}
