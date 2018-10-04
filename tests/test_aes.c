#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

#include "ds_benchmark.h"
#include <oqs/aes.h>
#include "../src/crypto/aes/aes_local.h"

/* Displays hexadecimal strings */
static void OQS_print_hex_string(const char *label, const uint8_t *str, size_t len) {
	printf("%-20s (%4zu bytes):  ", label, len);
	for (size_t i = 0; i < (len); i++) {
		printf("%02X", ((unsigned char *) (str))[i]);
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

static int test_aes128_correctness_c() {
	uint8_t derived_plaintext[16], derived_ciphertext[16];
	void *schedule = NULL;
	oqs_aes128_load_schedule_c(test_aes128_key, &schedule);
	oqs_aes128_enc_c(test_aes128_plaintext, schedule, derived_ciphertext);
	if (memcmp(test_aes128_ciphertext, derived_ciphertext, 16) != 0) {
		printf("test_aes128_correctness_c ciphertext does not match\n");
		OQS_print_hex_string("expected ciphertext", test_aes128_ciphertext, 16);
		OQS_print_hex_string("derived  ciphertext", derived_ciphertext, 16);
		return EXIT_FAILURE;
	}
	oqs_aes128_dec_c(test_aes128_ciphertext, schedule, derived_plaintext);
	if (memcmp(test_aes128_plaintext, derived_plaintext, 16) != 0) {
		printf("test_aes128_correctness_c plaintext does not match\n");
		OQS_print_hex_string("expected plaintext", test_aes128_plaintext, 16);
		OQS_print_hex_string("derived  plaintext", derived_plaintext, 16);
		return EXIT_FAILURE;
	}
	oqs_aes128_free_schedule_c(schedule);
	return EXIT_SUCCESS;
}

static int test_aes256_correctness_c() {
	uint8_t derived_plaintext[16], derived_ciphertext[16];
	void *schedule = NULL;
	oqs_aes256_load_schedule_c(test_aes256_key, &schedule);
	oqs_aes256_enc_c(test_aes256_plaintext, schedule, derived_ciphertext);
	if (memcmp(test_aes256_ciphertext, derived_ciphertext, 16) != 0) {
		printf("test_aes256_correctness_c ciphertext does not match\n");
		OQS_print_hex_string("expected ciphertext", test_aes256_ciphertext, 16);
		OQS_print_hex_string("derived  ciphertext", derived_ciphertext, 16);
		return EXIT_FAILURE;
	}
	oqs_aes256_dec_c(test_aes256_ciphertext, schedule, derived_plaintext);
	if (memcmp(test_aes256_plaintext, derived_plaintext, 16) != 0) {
		printf("test_aes256_correctness_c plaintext does not match\n");
		OQS_print_hex_string("expected plaintext", test_aes256_plaintext, 16);
		OQS_print_hex_string("derived  plaintext", derived_plaintext, 16);
		return EXIT_FAILURE;
	}
	oqs_aes256_free_schedule_c(schedule);
	return EXIT_SUCCESS;
}

#ifdef AES_ENABLE_NI
static int test_aes128_correctness_ni() {
	uint8_t derived_plaintext[16], derived_ciphertext[16];
	void *schedule = NULL;
	oqs_aes128_load_schedule_ni(test_aes128_key, &schedule);
	oqs_aes128_enc_ni(test_aes128_plaintext, schedule, derived_ciphertext);
	if (memcmp(test_aes128_ciphertext, derived_ciphertext, 16) != 0) {
		printf("test_aes128_correctness_ni ciphertext does not match\n");
		OQS_print_hex_string("expected ciphertext", test_aes128_ciphertext, 16);
		OQS_print_hex_string("derived  ciphertext", derived_ciphertext, 16);
		return EXIT_FAILURE;
	}
	oqs_aes128_dec_ni(test_aes128_ciphertext, schedule, derived_plaintext);
	if (memcmp(test_aes128_plaintext, derived_plaintext, 16) != 0) {
		printf("test_aes128_correctness_ni plaintext does not match\n");
		OQS_print_hex_string("expected plaintext", test_aes128_plaintext, 16);
		OQS_print_hex_string("derived  plaintext", derived_plaintext, 16);
		return EXIT_FAILURE;
	}
	oqs_aes128_free_schedule_ni(schedule);
	return EXIT_SUCCESS;
}
#endif

#ifdef USE_OPENSSL
static int test_aes128_correctness_ossl() {
	uint8_t derived_plaintext[16], derived_ciphertext[16];
	oqs_aes128_ecb_enc_ossl(test_aes128_plaintext, 16, test_aes128_key, derived_ciphertext);
	if (memcmp(test_aes128_ciphertext, derived_ciphertext, 16) != 0) {
		printf("test_aes128_correctness_ossl ciphertext does not match\n");
		OQS_print_hex_string("expected ciphertext", test_aes128_ciphertext, 16);
		OQS_print_hex_string("derived  ciphertext", derived_ciphertext, 16);
		return EXIT_FAILURE;
	}
	oqs_aes128_ecb_dec_ossl(test_aes128_ciphertext, 16, test_aes128_key, derived_plaintext);
	if (memcmp(test_aes128_plaintext, derived_plaintext, 16) != 0) {
		printf("test_aes128_correctness_ossl plaintext does not match\n");
		OQS_print_hex_string("expected plaintext", test_aes128_plaintext, 16);
		OQS_print_hex_string("derived  plaintext", derived_plaintext, 16);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
#endif

#ifdef USE_OPENSSL
static int test_aes256_correctness_ossl() {
	uint8_t derived_plaintext[16], derived_ciphertext[16];
	oqs_aes256_ecb_enc_ossl(test_aes256_plaintext, 16, test_aes256_key, derived_ciphertext);
	if (memcmp(test_aes256_ciphertext, derived_ciphertext, 16) != 0) {
		printf("test_aes256_correctness_ossl ciphertext does not match\n");
		OQS_print_hex_string("expected ciphertext", test_aes256_ciphertext, 16);
		OQS_print_hex_string("derived  ciphertext", derived_ciphertext, 16);
		return EXIT_FAILURE;
	}
	oqs_aes256_ecb_dec_ossl(test_aes256_ciphertext, 16, test_aes256_key, derived_plaintext);
	if (memcmp(test_aes256_plaintext, derived_plaintext, 16) != 0) {
		printf("test_aes256_correctness_ossl plaintext does not match\n");
		OQS_print_hex_string("expected plaintext", test_aes256_plaintext, 16);
		OQS_print_hex_string("derived  plaintext", derived_plaintext, 16);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
#endif

static void speed_aes128_c() {
	uint8_t plaintext[16], ciphertext[16];
	void *schedule = NULL;
	TIME_OPERATION_SECONDS({ oqs_aes128_load_schedule_c(test_aes128_key, &schedule); oqs_aes128_free_schedule_c(schedule); }, "oqs_aes128_load_schedule_c", BENCH_DURATION);

	oqs_aes128_load_schedule_c(test_aes128_key, &schedule);
	TIME_OPERATION_SECONDS(oqs_aes128_enc_c(test_aes128_plaintext, schedule, ciphertext), "oqs_aes128_enc_c", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_dec_c(test_aes128_ciphertext, schedule, ciphertext), "oqs_aes128_dec_c", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_enc_c(test_aes128_plaintext, 16, test_aes128_key, ciphertext), "oqs_aes128_ecb_enc_c", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_dec_c(test_aes128_ciphertext, 16, test_aes128_key, plaintext), "oqs_aes128_ecb_dec_c", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_enc_sch_c(test_aes128_plaintext, 16, schedule, ciphertext), "oqs_aes128_ecb_enc_sch_c", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_dec_sch_c(test_aes128_ciphertext, 16, schedule, plaintext), "oqs_aes128_ecb_dec_sch_c", BENCH_DURATION);
	oqs_aes128_free_schedule_c(schedule);
}

static void speed_aes256_c() {
	uint8_t plaintext[16], ciphertext[16];
	void *schedule = NULL;
	TIME_OPERATION_SECONDS({ oqs_aes256_load_schedule_c(test_aes256_key, &schedule); oqs_aes256_free_schedule_c(schedule); }, "oqs_aes256_load_schedule_c", BENCH_DURATION);

	oqs_aes256_load_schedule_c(test_aes256_key, &schedule);
	TIME_OPERATION_SECONDS(oqs_aes256_enc_c(test_aes256_plaintext, schedule, ciphertext), "oqs_aes256_enc_c", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes256_dec_c(test_aes256_ciphertext, schedule, ciphertext), "oqs_aes256_dec_c", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes256_ecb_enc_c(test_aes256_plaintext, 16, test_aes256_key, ciphertext), "oqs_aes256_ecb_enc_c", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes256_ecb_dec_c(test_aes256_ciphertext, 16, test_aes256_key, plaintext), "oqs_aes256_ecb_dec_c", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes256_ecb_enc_sch_c(test_aes256_plaintext, 16, schedule, ciphertext), "oqs_aes256_ecb_enc_sch_c", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes256_ecb_dec_sch_c(test_aes256_ciphertext, 16, schedule, plaintext), "oqs_aes256_ecb_dec_sch_c", BENCH_DURATION);
	oqs_aes256_free_schedule_c(schedule);
}

#ifdef AES_ENABLE_NI
static void speed_aes128_ni() {
	uint8_t plaintext[16], ciphertext[16];
	void *schedule = NULL;
	TIME_OPERATION_SECONDS({ oqs_aes128_load_schedule_ni(test_aes128_key, &schedule); oqs_aes128_free_schedule_ni(schedule); }, "oqs_aes128_load_schedule_ni", BENCH_DURATION);

	oqs_aes128_load_schedule_ni(test_aes128_key, &schedule);
	TIME_OPERATION_SECONDS(oqs_aes128_enc_ni(test_aes128_plaintext, schedule, ciphertext), "oqs_aes128_enc_ni", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_dec_ni(test_aes128_ciphertext, schedule, ciphertext), "oqs_aes128_dec_ni", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_enc_ni(test_aes128_plaintext, 16, test_aes128_key, ciphertext), "oqs_aes128_ecb_enc_ni", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_dec_ni(test_aes128_ciphertext, 16, test_aes128_key, plaintext), "oqs_aes128_ecb_dec_ni", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_enc_sch_ni(test_aes128_plaintext, 16, schedule, ciphertext), "oqs_aes128_ecb_enc_sch_ni", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_dec_sch_ni(test_aes128_ciphertext, 16, schedule, plaintext), "oqs_aes128_ecb_dec_sch_ni", BENCH_DURATION);
	oqs_aes128_free_schedule_ni(schedule);
}
#endif

#ifdef USE_OPENSSL
static void speed_aes128_ossl() {
	uint8_t plaintext[16], ciphertext[16];
	void *schedule = NULL;
	TIME_OPERATION_SECONDS({ oqs_aes128_load_schedule_ossl(test_aes128_key, &schedule, 1); oqs_aes128_free_schedule_ossl(schedule); }, "oqs_aes128_load_schedule_ossl", BENCH_DURATION);

	oqs_aes128_load_schedule_ossl(test_aes128_key, &schedule, 1);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_enc_ossl(test_aes128_plaintext, 16, test_aes128_key, ciphertext), "oqs_aes128_ecb_enc_ossl", BENCH_DURATION);
	oqs_aes128_free_schedule_ossl(schedule);
	oqs_aes128_load_schedule_ossl(test_aes128_key, &schedule, 0);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_dec_ossl(test_aes128_ciphertext, 16, test_aes128_key, plaintext), "oqs_aes128_ecb_dec_ossl", BENCH_DURATION);
	oqs_aes128_free_schedule_ossl(schedule);
	oqs_aes128_load_schedule_ossl(test_aes128_key, &schedule, 1);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_enc_sch_ossl(test_aes128_plaintext, 16, schedule, ciphertext), "oqs_aes128_ecb_enc_sch_ossl", BENCH_DURATION);
	oqs_aes128_free_schedule_ossl(schedule);
	oqs_aes128_load_schedule_ossl(test_aes128_key, &schedule, 0);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_dec_sch_ossl(test_aes128_ciphertext, 16, schedule, plaintext), "oqs_aes128_ecb_dec_sch_ossl", BENCH_DURATION);
	oqs_aes128_free_schedule_ossl(schedule);
}
#endif

#ifdef USE_OPENSSL
static void speed_aes256_ossl() {
	uint8_t plaintext[16], ciphertext[16];
	void *schedule = NULL;
	TIME_OPERATION_SECONDS({ oqs_aes256_load_schedule_ossl(test_aes256_key, &schedule, 1); oqs_aes256_free_schedule_ossl(schedule); }, "oqs_aes256_load_schedule_ossl", BENCH_DURATION);

	oqs_aes256_load_schedule_ossl(test_aes256_key, &schedule, 1);
	TIME_OPERATION_SECONDS(oqs_aes256_ecb_enc_ossl(test_aes256_plaintext, 16, test_aes256_key, ciphertext), "oqs_aes256_ecb_enc_ossl", BENCH_DURATION);
	oqs_aes256_free_schedule_ossl(schedule);
	oqs_aes256_load_schedule_ossl(test_aes256_key, &schedule, 0);
	TIME_OPERATION_SECONDS(oqs_aes256_ecb_dec_ossl(test_aes256_ciphertext, 16, test_aes256_key, plaintext), "oqs_aes256_ecb_dec_ossl", BENCH_DURATION);
	oqs_aes256_free_schedule_ossl(schedule);
	oqs_aes256_load_schedule_ossl(test_aes256_key, &schedule, 1);
	TIME_OPERATION_SECONDS(oqs_aes256_ecb_enc_sch_ossl(test_aes256_plaintext, 16, schedule, ciphertext), "oqs_aes256_ecb_enc_sch_ossl", BENCH_DURATION);
	oqs_aes256_free_schedule_ossl(schedule);
	oqs_aes256_load_schedule_ossl(test_aes256_key, &schedule, 0);
	TIME_OPERATION_SECONDS(oqs_aes256_ecb_dec_sch_ossl(test_aes256_ciphertext, 16, schedule, plaintext), "oqs_aes256_ecb_dec_sch_ossl", BENCH_DURATION);
	oqs_aes256_free_schedule_ossl(schedule);
}
#endif

int main(int argc, char **argv) {
	bool bench = false;

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
					return EXIT_SUCCESS;
				} else {
					return EXIT_FAILURE;
				}
			}
		}
	}

	printf("=== test_aes correctness ===\n");
	if (test_aes128_correctness_c() != EXIT_SUCCESS)
		return EXIT_FAILURE;
	if (test_aes256_correctness_c() != EXIT_SUCCESS)
		return EXIT_FAILURE;
#ifdef AES_ENABLE_NI
	if (test_aes128_correctness_ni() != EXIT_SUCCESS)
		return EXIT_FAILURE;
#endif
#ifdef USE_OPENSSL
	if (test_aes128_correctness_ossl() != EXIT_SUCCESS)
		return EXIT_FAILURE;
	if (test_aes256_correctness_ossl() != EXIT_SUCCESS)
		return EXIT_FAILURE;
#endif
	printf("Tests passed.\n\n");

	if (bench) {
		printf("=== test_aes performance ===\n");
		PRINT_TIMER_HEADER
		speed_aes128_c();
		speed_aes256_c();
#ifdef AES_ENABLE_NI
		speed_aes128_ni();
#endif
#ifdef USE_OPENSSL
		speed_aes128_ossl();
		speed_aes256_ossl();
#endif
		PRINT_TIMER_FOOTER
	}

	return EXIT_SUCCESS;
}
