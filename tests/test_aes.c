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

/* Appendix C.2 of FIPS 197 */
static const uint8_t test_aes192_plaintext[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
static const uint8_t test_aes192_key[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
static const uint8_t test_aes192_ciphertext[] = {0xdd, 0xa9, 0x7c, 0xa4, 0x86, 0x4c, 0xdf, 0xe0, 0x6e, 0xaf, 0x70, 0xa0, 0xec, 0x0d, 0x71, 0x91};

static int test_aes192_correctness(void) {
	uint8_t derived_ciphertext[16];
	void *schedule = NULL;
	OQS_AES192_ECB_load_schedule(test_aes192_key, &schedule);
	OQS_AES192_ECB_enc_sch(test_aes192_plaintext, sizeof(test_aes192_plaintext), schedule, derived_ciphertext);
	if (memcmp(test_aes192_ciphertext, derived_ciphertext, 16) != 0) {
		printf("test_aes192_correctness ciphertext does not match\n");
		OQS_print_hex_string("expected ciphertext", test_aes192_ciphertext, 16);
		OQS_print_hex_string("derived  ciphertext", derived_ciphertext, 16);
		OQS_AES192_free_schedule(schedule);
		return EXIT_FAILURE;
	}
	OQS_AES192_free_schedule(schedule);

	OQS_AES192_ECB_enc(test_aes192_plaintext, sizeof(test_aes192_plaintext), test_aes192_key, derived_ciphertext);
	if (memcmp(test_aes192_ciphertext, derived_ciphertext, 16) != 0) {
		printf("test_aes192_correctness one-shot ciphertext does not match\n");
		OQS_print_hex_string("expected ciphertext", test_aes192_ciphertext, 16);
		OQS_print_hex_string("derived  ciphertext", derived_ciphertext, 16);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

/* Multi-block ECB test vectors from NIST SP 800-38A, Appendix F.1. All three
 * key sizes encrypt the same four plaintext blocks, so that the multi-block
 * code paths of every backend are exercised. */
static const uint8_t test_aes_ecb_plaintext[64] = {
	0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
	0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
	0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
	0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
};

/* Appendix F.1.1, ECB-AES128.Encrypt */
static const uint8_t test_aes128_ecb_key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
static const uint8_t test_aes128_ecb_ciphertext[64] = {
	0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60, 0xa8, 0x9e, 0xca, 0xf3, 0x24, 0x66, 0xef, 0x97,
	0xf5, 0xd3, 0xd5, 0x85, 0x03, 0xb9, 0x69, 0x9d, 0xe7, 0x85, 0x89, 0x5a, 0x96, 0xfd, 0xba, 0xaf,
	0x43, 0xb1, 0xcd, 0x7f, 0x59, 0x8e, 0xce, 0x23, 0x88, 0x1b, 0x00, 0xe3, 0xed, 0x03, 0x06, 0x88,
	0x7b, 0x0c, 0x78, 0x5e, 0x27, 0xe8, 0xad, 0x3f, 0x82, 0x23, 0x20, 0x71, 0x04, 0x72, 0x5d, 0xd4
};

/* Appendix F.1.3, ECB-AES192.Encrypt */
static const uint8_t test_aes192_ecb_key[] = {0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5, 0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b};
static const uint8_t test_aes192_ecb_ciphertext[64] = {
	0xbd, 0x33, 0x4f, 0x1d, 0x6e, 0x45, 0xf2, 0x5f, 0xf7, 0x12, 0xa2, 0x14, 0x57, 0x1f, 0xa5, 0xcc,
	0x97, 0x41, 0x04, 0x84, 0x6d, 0x0a, 0xd3, 0xad, 0x77, 0x34, 0xec, 0xb3, 0xec, 0xee, 0x4e, 0xef,
	0xef, 0x7a, 0xfd, 0x22, 0x70, 0xe2, 0xe6, 0x0a, 0xdc, 0xe0, 0xba, 0x2f, 0xac, 0xe6, 0x44, 0x4e,
	0x9a, 0x4b, 0x41, 0xba, 0x73, 0x8d, 0x6c, 0x72, 0xfb, 0x16, 0x69, 0x16, 0x03, 0xc1, 0x8e, 0x0e
};

/* Appendix F.1.5, ECB-AES256.Encrypt */
static const uint8_t test_aes256_ecb_key[] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};
static const uint8_t test_aes256_ecb_ciphertext[64] = {
	0xf3, 0xee, 0xd1, 0xbd, 0xb5, 0xd2, 0xa0, 0x3c, 0x06, 0x4b, 0x5a, 0x7e, 0x3d, 0xb1, 0x81, 0xf8,
	0x59, 0x1c, 0xcb, 0x10, 0xd4, 0x10, 0xed, 0x26, 0xdc, 0x5b, 0xa7, 0x4a, 0x31, 0x36, 0x28, 0x70,
	0xb6, 0xed, 0x21, 0xb9, 0x9c, 0xa6, 0xf4, 0xf9, 0xf1, 0x53, 0xe7, 0xb1, 0xbe, 0xaf, 0xed, 0x1d,
	0x23, 0x30, 0x4b, 0x7a, 0x39, 0xf9, 0xf3, 0xff, 0x06, 0x7d, 0x8d, 0x8f, 0x9e, 0x24, 0xec, 0xc7
};

/* Checks that the bytes of buf beyond len were left untouched (still zero), so
 * that a multi-block implementation writing past the requested length is
 * caught even when the prefix is correct. */
static int check_untouched_tail(const char *name, const char *what, const uint8_t *buf, size_t len, size_t buf_len) {
	for (size_t i = len; i < buf_len; i++) {
		if (buf[i] != 0) {
			printf("%s %s wrote past the requested length (%zu bytes)\n", name, what, len);
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

typedef void (*aes_ecb_load_schedule_fn)(const uint8_t *key, void **schedule);
typedef void (*aes_ecb_free_schedule_fn)(void *schedule);
typedef void (*aes_ecb_enc_sch_fn)(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext);
typedef void (*aes_ecb_enc_fn)(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext);

static int test_aes_ecb_multiblock(const char *name, const uint8_t *key, const uint8_t *ciphertext64,
                                   aes_ecb_load_schedule_fn load_schedule, aes_ecb_free_schedule_fn free_schedule,
                                   aes_ecb_enc_sch_fn enc_sch, aes_ecb_enc_fn enc) {
	/* Nine blocks: the four SP 800-38A blocks twice, then the first block
	 * again. Every whole-block length from one to nine blocks is tested, so
	 * that a backend's four-block loop runs zero, one and two times and every
	 * tail length of zero to three blocks occurs. In ECB mode repeated blocks
	 * must encrypt identically. */
	uint8_t plaintext[144];
	uint8_t expected[144];
	uint8_t derived[144];
	memcpy(plaintext, test_aes_ecb_plaintext, 64);
	memcpy(plaintext + 64, test_aes_ecb_plaintext, 64);
	memcpy(plaintext + 128, test_aes_ecb_plaintext, 16);
	memcpy(expected, ciphertext64, 64);
	memcpy(expected + 64, ciphertext64, 64);
	memcpy(expected + 128, ciphertext64, 16);

	for (size_t len = 16; len <= sizeof(plaintext); len += 16) {
		void *schedule = NULL;
		memset(derived, 0, sizeof(derived));
		load_schedule(key, &schedule);
		enc_sch(plaintext, len, schedule, derived);
		free_schedule(schedule);
		if (memcmp(expected, derived, len) != 0) {
			printf("%s ciphertext does not match (%zu bytes)\n", name, len);
			OQS_print_hex_string("expected ciphertext", expected, len);
			OQS_print_hex_string("derived  ciphertext", derived, len);
			return EXIT_FAILURE;
		}
		if (check_untouched_tail(name, "enc_sch", derived, len, sizeof(derived)) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
		memset(derived, 0, sizeof(derived));
		enc(plaintext, len, key, derived);
		if (memcmp(expected, derived, len) != 0) {
			printf("%s one-shot ciphertext does not match (%zu bytes)\n", name, len);
			OQS_print_hex_string("expected ciphertext", expected, len);
			OQS_print_hex_string("derived  ciphertext", derived, len);
			return EXIT_FAILURE;
		}
		if (check_untouched_tail(name, "one-shot", derived, len, sizeof(derived)) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
		/* FrodoKEM encrypts its public matrix in place, so a multi-block path
		 * must read every input block of a group before writing any output. */
		memcpy(derived, plaintext, len);
		memset(derived + len, 0, sizeof(derived) - len);
		load_schedule(key, &schedule);
		enc_sch(derived, len, schedule, derived);
		free_schedule(schedule);
		if (memcmp(expected, derived, len) != 0) {
			printf("%s in-place ciphertext does not match (%zu bytes)\n", name, len);
			OQS_print_hex_string("expected ciphertext", expected, len);
			OQS_print_hex_string("derived  ciphertext", derived, len);
			return EXIT_FAILURE;
		}
		if (check_untouched_tail(name, "in-place", derived, len, sizeof(derived)) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

static int test_aes128_multiblock_correctness(void) {
	return test_aes_ecb_multiblock("test_aes128_multiblock_correctness", test_aes128_ecb_key, test_aes128_ecb_ciphertext,
	                               OQS_AES128_ECB_load_schedule, OQS_AES128_free_schedule, OQS_AES128_ECB_enc_sch, OQS_AES128_ECB_enc);
}

static int test_aes192_multiblock_correctness(void) {
	return test_aes_ecb_multiblock("test_aes192_multiblock_correctness", test_aes192_ecb_key, test_aes192_ecb_ciphertext,
	                               OQS_AES192_ECB_load_schedule, OQS_AES192_free_schedule, OQS_AES192_ECB_enc_sch, OQS_AES192_ECB_enc);
}

static int test_aes256_multiblock_correctness(void) {
	return test_aes_ecb_multiblock("test_aes256_multiblock_correctness", test_aes256_ecb_key, test_aes256_ecb_ciphertext,
	                               OQS_AES256_ECB_load_schedule, OQS_AES256_free_schedule, OQS_AES256_ECB_enc_sch, OQS_AES256_ECB_enc);
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

// OQS_AES128_CTR_inc_iv() stores the IV inside the key schedule; this entry
// point had no coverage, so an undersized schedule went unnoticed under ASan.
static int test_aes128ctr_inc_iv(void) {
	void *schedule = NULL;
	OQS_AES128_CTR_inc_init(test_aes128ctr_key, &schedule);
	OQS_AES128_CTR_inc_iv(test_aes128ctr_iv, 12, schedule);
	OQS_AES128_CTR_inc_iv(test_aes128ctr_iv, sizeof(test_aes128ctr_iv), schedule);
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

/* Multi-block CTR test vectors. The keys and the initial counter block are
 * those of NIST SP 800-38A, Appendix F.5, and the first four keystream blocks
 * are the "Output Block" values listed there (F.5.1 for AES-128, F.5.5 for
 * AES-256). Blocks five to nine continue the same counter, f0f1...fdff03 to
 * f0f1...fdff07, and were generated with OpenSSL 3. No carry leaves the low
 * 32 bits over this range, so the 32-bit counter increment used by liboqs
 * agrees with the 128-bit increment of SP 800-38A. Nine blocks let a single
 * call cover two four-block groups and a tail. */
static const uint8_t test_aes_ctr_iv[16] = {0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};

/* Appendix F.5.1, CTR-AES128.Encrypt, with the key of F.1.1 */
static const uint8_t test_aes128_ctr_keystream[144] = {
	0xec, 0x8c, 0xdf, 0x73, 0x98, 0x60, 0x7c, 0xb0, 0xf2, 0xd2, 0x16, 0x75, 0xea, 0x9e, 0xa1, 0xe4,
	0x36, 0x2b, 0x7c, 0x3c, 0x67, 0x73, 0x51, 0x63, 0x18, 0xa0, 0x77, 0xd7, 0xfc, 0x50, 0x73, 0xae,
	0x6a, 0x2c, 0xc3, 0x78, 0x78, 0x89, 0x37, 0x4f, 0xbe, 0xb4, 0xc8, 0x1b, 0x17, 0xba, 0x6c, 0x44,
	0xe8, 0x9c, 0x39, 0x9f, 0xf0, 0xf1, 0x98, 0xc6, 0xd4, 0x0a, 0x31, 0xdb, 0x15, 0x6c, 0xab, 0xfe,
	0xb0, 0x0d, 0x47, 0xf8, 0x14, 0x8a, 0x91, 0x0e, 0xf0, 0x68, 0x30, 0x97, 0x90, 0x4b, 0xa5, 0x02,
	0x58, 0x99, 0x44, 0x5a, 0x4d, 0xe1, 0x01, 0xf5, 0x13, 0xca, 0xd1, 0x98, 0x7d, 0x89, 0xe9, 0x1b,
	0x3b, 0xd9, 0xac, 0x79, 0x49, 0xde, 0x2b, 0xf9, 0x65, 0x69, 0xac, 0x38, 0x43, 0xf8, 0x72, 0x42,
	0x7d, 0x9a, 0xce, 0x80, 0x47, 0xc3, 0x53, 0x09, 0x15, 0x5a, 0xb8, 0xa8, 0xf0, 0x85, 0x97, 0xb1,
	0xb7, 0x9c, 0xb9, 0x26, 0x40, 0xee, 0x48, 0x97, 0x95, 0xaf, 0x36, 0x15, 0x2a, 0xb3, 0xf6, 0x3b
};

/* Appendix F.5.5, CTR-AES256.Encrypt, with the key of F.1.5 */
static const uint8_t test_aes256_ctr_keystream[144] = {
	0x0b, 0xdf, 0x7d, 0xf1, 0x59, 0x17, 0x16, 0x33, 0x5e, 0x9a, 0x8b, 0x15, 0xc8, 0x60, 0xc5, 0x02,
	0x5a, 0x6e, 0x69, 0x9d, 0x53, 0x61, 0x19, 0x06, 0x54, 0x33, 0x86, 0x3c, 0x8f, 0x65, 0x7b, 0x94,
	0x1b, 0xc1, 0x2c, 0x9c, 0x01, 0x61, 0x0d, 0x5d, 0x0d, 0x8b, 0xd6, 0xa3, 0x37, 0x8e, 0xca, 0x62,
	0x29, 0x56, 0xe1, 0xc8, 0x69, 0x35, 0x36, 0xb1, 0xbe, 0xe9, 0x9c, 0x73, 0xa3, 0x15, 0x76, 0xb6,
	0x8b, 0x77, 0xff, 0xe0, 0xd9, 0x7c, 0x09, 0x92, 0xd7, 0xf7, 0x0e, 0x1c, 0xe9, 0xcf, 0xc3, 0xb7,
	0x4c, 0xeb, 0x67, 0x82, 0x60, 0x06, 0x08, 0x83, 0x6c, 0xa2, 0xcb, 0x45, 0x25, 0x9a, 0x85, 0xad,
	0x19, 0xce, 0x53, 0xfc, 0x30, 0x15, 0x81, 0x72, 0xd5, 0x57, 0x1c, 0x40, 0xee, 0x41, 0x59, 0x68,
	0x34, 0x6f, 0x29, 0x09, 0xbe, 0xcd, 0x61, 0x03, 0x64, 0x27, 0xdc, 0x9b, 0x2b, 0xd7, 0x6d, 0xe6,
	0xde, 0xbd, 0x93, 0x7d, 0x4a, 0x6e, 0xed, 0x09, 0xd6, 0x4f, 0xf3, 0x29, 0x2b, 0xc1, 0x8e, 0x70
};

typedef void (*aes_ctr_inc_init_fn)(const uint8_t *key, void **schedule);
typedef void (*aes_ctr_inc_stream_iv_fn)(const uint8_t *iv, size_t iv_len, const void *schedule, uint8_t *out, size_t out_len);

static int test_aes_ctr_multiblock(const char *name, const uint8_t *key, const uint8_t *keystream144,
                                   aes_ctr_inc_init_fn inc_init, aes_ecb_free_schedule_fn free_schedule,
                                   aes_ctr_inc_stream_iv_fn stream_iv) {
	/* Every whole-block length from one to nine blocks, plus lengths that end
	 * in a partial block before and after a four-block group. */
	const size_t lengths[] = {16, 32, 48, 64, 80, 96, 112, 128, 144, 36, 100};
	uint8_t derived[144];
	for (size_t i = 0; i < sizeof(lengths) / sizeof(lengths[0]); i++) {
		size_t len = lengths[i];
		void *schedule = NULL;
		memset(derived, 0, sizeof(derived));
		inc_init(key, &schedule);
		stream_iv(test_aes_ctr_iv, sizeof(test_aes_ctr_iv), schedule, derived, len);
		free_schedule(schedule);
		if (memcmp(keystream144, derived, len) != 0) {
			printf("%s keystream does not match (%zu bytes)\n", name, len);
			OQS_print_hex_string("expected keystream", keystream144, len);
			OQS_print_hex_string("derived  keystream", derived, len);
			return EXIT_FAILURE;
		}
		if (check_untouched_tail(name, "stream_iv", derived, len, sizeof(derived)) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

static int test_aes128ctr_multiblock_correctness(void) {
	return test_aes_ctr_multiblock("test_aes128ctr_multiblock_correctness", test_aes128_ecb_key, test_aes128_ctr_keystream,
	                               OQS_AES128_CTR_inc_init, OQS_AES128_free_schedule, OQS_AES128_CTR_inc_stream_iv);
}

static int test_aes256ctr_multiblock_correctness(void) {
	return test_aes_ctr_multiblock("test_aes256ctr_multiblock_correctness", test_aes256_ecb_key, test_aes256_ctr_keystream,
	                               OQS_AES256_CTR_inc_init, OQS_AES256_free_schedule, OQS_AES256_CTR_inc_stream_iv);
}

/* OQS_AES256_CTR_inc_stream_blks() carries the counter in the schedule from
 * one call to the next. Produce the same nine keystream blocks through several
 * call patterns, so that the counter is checked across four-block groups and
 * their tails, both within a call and between calls. */
static int test_aes256ctr_stream_blks(void) {
	static const size_t patterns[][4] = {{9, 0, 0, 0}, {4, 5, 0, 0}, {5, 4, 0, 0}, {1, 4, 4, 0}, {3, 3, 3, 0}, {2, 7, 0, 0}};
	uint8_t derived[144];
	for (size_t p = 0; p < sizeof(patterns) / sizeof(patterns[0]); p++) {
		void *schedule = NULL;
		size_t offset = 0;
		memset(derived, 0, sizeof(derived));
		OQS_AES256_CTR_inc_init(test_aes256_ecb_key, &schedule);
		OQS_AES256_CTR_inc_iv(test_aes_ctr_iv, sizeof(test_aes_ctr_iv), schedule);
		for (size_t c = 0; c < 4 && patterns[p][c] > 0; c++) {
			OQS_AES256_CTR_inc_stream_blks(schedule, derived + offset, patterns[p][c]);
			offset += 16 * patterns[p][c];
		}
		OQS_AES256_free_schedule(schedule);
		if (memcmp(test_aes256_ctr_keystream, derived, sizeof(derived)) != 0) {
			printf("test_aes256ctr_stream_blks keystream does not match (call pattern %zu)\n", p);
			OQS_print_hex_string("expected keystream", test_aes256_ctr_keystream, sizeof(derived));
			OQS_print_hex_string("derived  keystream", derived, sizeof(derived));
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

/* Bulk buffers for the multi-block benchmarks: 256 blocks, i.e. 64 four-block
 * groups, so that the interleaved paths of the backends dominate. */
#define BENCH_BULK_BYTES 4096
static uint8_t bench_bulk_in[BENCH_BULK_BYTES];
static uint8_t bench_bulk_out[BENCH_BULK_BYTES];

static void speed_aes128(void) {
	uint8_t ciphertext[16];
	void *schedule = NULL;
	TIME_OPERATION_SECONDS({ OQS_AES128_ECB_load_schedule(test_aes128_key, &schedule); OQS_AES128_free_schedule(schedule); }, "OQS_AES128_ECB_load+free_sch", BENCH_DURATION);

	OQS_AES128_ECB_load_schedule(test_aes128_key, &schedule);
	TIME_OPERATION_SECONDS(OQS_AES128_ECB_enc_sch(test_aes128_plaintext, sizeof(test_aes128_plaintext), schedule, ciphertext), "OQS_AES128_ECB_enc_sch 1 blk", BENCH_DURATION);
	TIME_OPERATION_SECONDS(OQS_AES128_ECB_enc_sch(bench_bulk_in, BENCH_BULK_BYTES, schedule, bench_bulk_out), "OQS_AES128_ECB_enc_sch 256 blk", BENCH_DURATION);
	TIME_OPERATION_SECONDS(OQS_AES128_ECB_enc(test_aes128_plaintext, sizeof(test_aes128_plaintext), test_aes128_key, ciphertext), "OQS_AES128_ECB_enc", BENCH_DURATION);
	OQS_AES128_free_schedule(schedule);

	/* CTR gets its own schedule: on the OpenSSL backend the ECB and CTR contexts differ. */
	OQS_AES128_CTR_inc_init(test_aes128_key, &schedule);
	TIME_OPERATION_SECONDS(OQS_AES128_CTR_inc_stream_iv(test_aes_ctr_iv, sizeof(test_aes_ctr_iv), schedule, bench_bulk_out, BENCH_BULK_BYTES), "OQS_AES128_CTR_stream_iv 256 blk", BENCH_DURATION);
	OQS_AES128_free_schedule(schedule);
}

static void speed_aes192(void) {
	uint8_t ciphertext[16];
	void *schedule = NULL;
	TIME_OPERATION_SECONDS({ OQS_AES192_ECB_load_schedule(test_aes192_key, &schedule); OQS_AES192_free_schedule(schedule); }, "OQS_AES192_ECB_load+free_sch", BENCH_DURATION);

	OQS_AES192_ECB_load_schedule(test_aes192_key, &schedule);
	TIME_OPERATION_SECONDS(OQS_AES192_ECB_enc_sch(test_aes192_plaintext, sizeof(test_aes192_plaintext), schedule, ciphertext), "OQS_AES192_ECB_enc_sch 1 blk", BENCH_DURATION);
	TIME_OPERATION_SECONDS(OQS_AES192_ECB_enc_sch(bench_bulk_in, BENCH_BULK_BYTES, schedule, bench_bulk_out), "OQS_AES192_ECB_enc_sch 256 blk", BENCH_DURATION);
	TIME_OPERATION_SECONDS(OQS_AES192_ECB_enc(test_aes192_plaintext, sizeof(test_aes192_plaintext), test_aes192_key, ciphertext), "OQS_AES192_ECB_enc", BENCH_DURATION);
	OQS_AES192_free_schedule(schedule);
}

static void speed_aes256(void) {
	uint8_t ciphertext[16];
	void *schedule = NULL;
	TIME_OPERATION_SECONDS({ OQS_AES256_ECB_load_schedule(test_aes256_key, &schedule); OQS_AES256_free_schedule(schedule); }, "OQS_AES256_ECB_load+free_sch", BENCH_DURATION);

	OQS_AES256_ECB_load_schedule(test_aes256_key, &schedule);
	TIME_OPERATION_SECONDS(OQS_AES256_ECB_enc_sch(test_aes256_plaintext, sizeof(test_aes256_plaintext), schedule, ciphertext), "OQS_AES256_ECB_enc_sch 1 blk", BENCH_DURATION);
	TIME_OPERATION_SECONDS(OQS_AES256_ECB_enc_sch(bench_bulk_in, BENCH_BULK_BYTES, schedule, bench_bulk_out), "OQS_AES256_ECB_enc_sch 256 blk", BENCH_DURATION);
	TIME_OPERATION_SECONDS(OQS_AES256_ECB_enc(test_aes256_plaintext, sizeof(test_aes256_plaintext), test_aes256_key, ciphertext), "OQS_AES256_ECB_enc", BENCH_DURATION);
	OQS_AES256_free_schedule(schedule);

	OQS_AES256_CTR_inc_init(test_aes256_key, &schedule);
	TIME_OPERATION_SECONDS(OQS_AES256_CTR_inc_stream_iv(test_aes_ctr_iv, sizeof(test_aes_ctr_iv), schedule, bench_bulk_out, BENCH_BULK_BYTES), "OQS_AES256_CTR_stream_iv 256 blk", BENCH_DURATION);
	OQS_AES256_CTR_inc_iv(test_aes_ctr_iv, sizeof(test_aes_ctr_iv), schedule);
	TIME_OPERATION_SECONDS(OQS_AES256_CTR_inc_stream_blks(schedule, bench_bulk_out, BENCH_BULK_BYTES / 16), "OQS_AES256_CTR_stream_blks 256 blk", BENCH_DURATION);
	OQS_AES256_free_schedule(schedule);
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
	if (test_aes128_multiblock_correctness() != EXIT_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	if (test_aes128ctr_correctness() != EXIT_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	if (test_aes128ctr_inc_iv() != EXIT_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	if (test_aes128ctr_multiblock_correctness() != EXIT_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	if (test_aes192_correctness() != EXIT_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	if (test_aes192_multiblock_correctness() != EXIT_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}

	if (test_aes256_correctness() != EXIT_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	if (test_aes256_multiblock_correctness() != EXIT_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	if (test_aes256ctr_correctness() != EXIT_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	if (test_aes256ctr_multiblock_correctness() != EXIT_SUCCESS) {
		OQS_destroy();
		return EXIT_FAILURE;
	}
	if (test_aes256ctr_stream_blks() != EXIT_SUCCESS) {
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
		speed_aes192();
		speed_aes256();
		PRINT_TIMER_FOOTER
	}
	OQS_destroy();

	return EXIT_SUCCESS;
}
