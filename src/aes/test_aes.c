#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <oqs/rand.h>

#include "aes.h"
#include "aes_local.h"
#include "../ds_benchmark.h"

#define BENCH_DURATION 1

#define TEST_ITERATIONS 100

#define TEST_REPEATEDLY(x) \
	for (int i = 0; i < TEST_ITERATIONS; i++) { \
		int ok = (x); \
		if (ok != EXIT_SUCCESS) { \
			fprintf(stderr, "Failure in %s (iteration %d)\n", #x, i); \
			return EXIT_FAILURE; \
		} \
	}

static void print_bytes(uint8_t *bytes, size_t num_bytes) {
	for (size_t i = 0; i < num_bytes; i++) {
		printf("%02x", (unsigned)bytes[i]);
	}
}

static int test_aes128_correctness_c(OQS_RAND *rand) {
	uint8_t key[16], plaintext[16], ciphertext[16], decrypted[16];
	void *schedule = NULL;
	OQS_RAND_n(rand, key, 16);
	OQS_RAND_n(rand, plaintext, 16);
	oqs_aes128_load_schedule_c(key, &schedule);
	oqs_aes128_enc_c(plaintext, schedule, ciphertext);
	oqs_aes128_dec_c(ciphertext, schedule, decrypted);
	oqs_aes128_free_schedule_c(schedule);
	if (memcmp(plaintext, decrypted, 16) == 0) {
		return EXIT_SUCCESS;
	} else {
		print_bytes(plaintext, 16);
		printf("\n");
		print_bytes(decrypted, 16);
		printf("\n");
		return EXIT_FAILURE;
		return EXIT_FAILURE;
	}
}

#ifndef AES_DISABLE_NI
static int test_aes128_correctness_ni(OQS_RAND *rand) {
	uint8_t key[16], plaintext[16], ciphertext[16], decrypted[16];
	void *schedule = NULL;
	OQS_RAND_n(rand, key, 16);
	OQS_RAND_n(rand, plaintext, 16);
	oqs_aes128_load_schedule_ni(key, &schedule);
	oqs_aes128_enc_ni(plaintext, schedule, ciphertext);
	oqs_aes128_dec_ni(ciphertext, schedule, decrypted);
	oqs_aes128_free_schedule_ni(schedule);
	if (memcmp(plaintext, decrypted, 16) == 0) {
		return EXIT_SUCCESS;
	} else {
		print_bytes(plaintext, 16);
		printf("\n");
		print_bytes(decrypted, 16);
		printf("\n");
		return EXIT_FAILURE;
	}
}

static int test_aes128_c_equals_ni(OQS_RAND *rand) {
	uint8_t key[16], plaintext[16], ciphertext_c[16], ciphertext_ni[16];
	void *schedule_c = NULL, *schedule_ni = NULL;
	OQS_RAND_n(rand, key, 16);
	OQS_RAND_n(rand, plaintext, 16);
	oqs_aes128_load_schedule_c(key, &schedule_c);
	oqs_aes128_load_schedule_ni(key, &schedule_ni);
	oqs_aes128_enc_c(plaintext, schedule_c, ciphertext_c);
	oqs_aes128_enc_ni(plaintext, schedule_ni, ciphertext_ni);
	oqs_aes128_free_schedule_c(schedule_c);
	oqs_aes128_free_schedule_ni(schedule_ni);
	if (memcmp(ciphertext_c, ciphertext_ni, 16) == 0) {
		return EXIT_SUCCESS;
	} else {
		print_bytes(ciphertext_c, 16);
		printf("\n");
		print_bytes(ciphertext_ni, 16);
		printf("\n");
		return EXIT_FAILURE;
	}
}

static int test_aes128_ecb_correctness_ni(OQS_RAND *rand) {
	uint8_t key[16], plaintext[320], ciphertext[320], decrypted[320];
	void *schedule = NULL;
	OQS_RAND_n(rand, key, 16);
	OQS_RAND_n(rand, plaintext, 320);
	oqs_aes128_load_schedule_ni(key, &schedule);
	oqs_aes128_ecb_enc_ni(plaintext, 320, schedule, ciphertext);
	oqs_aes128_ecb_dec_ni(ciphertext, 320, schedule, decrypted);
	oqs_aes128_free_schedule_ni(schedule);
	if (memcmp(plaintext, decrypted, 320) == 0) {
		return EXIT_SUCCESS;
	} else {
		print_bytes(plaintext, 320);
		printf("\n");
		print_bytes(decrypted, 320);
		printf("\n");
		return EXIT_FAILURE;
	}
}
#endif

static int test_aes128_ecb_correctness_c(OQS_RAND *rand) {
	uint8_t key[16], plaintext[320], ciphertext[320], decrypted[320];
	void *schedule = NULL;
	OQS_RAND_n(rand, key, 16);
	OQS_RAND_n(rand, plaintext, 320);
	oqs_aes128_load_schedule_c(key, &schedule);
	oqs_aes128_ecb_enc_c(plaintext, 320, schedule, ciphertext);
	oqs_aes128_ecb_dec_c(ciphertext, 320, schedule, decrypted);
	oqs_aes128_free_schedule_c(schedule);
	if (memcmp(plaintext, decrypted, 320) == 0) {
		return EXIT_SUCCESS;
	} else {
		print_bytes(plaintext, 320);
		printf("\n");
		print_bytes(decrypted, 320);
		printf("\n");
		return EXIT_FAILURE;
	}
}

#ifdef USE_OPENSSL
static int test_aes128_ecb_correctness_ossl(OQS_RAND *rand) {
	uint8_t key[16], plaintext[320], ciphertext[320], decrypted[320];
	void *schedule = NULL;
	OQS_RAND_n(rand, key, 16);
	OQS_RAND_n(rand, plaintext, 320);
	oqs_aes128_load_schedule_ossl(key, &schedule, 1);
	oqs_aes128_ecb_enc_ossl(plaintext, 320, schedule, ciphertext);
	oqs_aes128_free_schedule_ossl(schedule);
	oqs_aes128_load_schedule_ossl(key, &schedule, 0);
	oqs_aes128_ecb_dec_ossl(ciphertext, 320, schedule, decrypted);
	oqs_aes128_free_schedule_ossl(schedule);
	if (memcmp(plaintext, decrypted, 320) == 0) {
		return EXIT_SUCCESS;
	} else {
		print_bytes(plaintext, 320);
		printf("\n");
		print_bytes(decrypted, 320);
		printf("\n");
		return EXIT_FAILURE;
	}
}
#endif

static void speed_aes128_c(OQS_RAND *rand) {
	uint8_t key[16], plaintext[320], ciphertext[320], decrypted[320];
	void *schedule = NULL;
	OQS_RAND_n(rand, key, 16);
	OQS_RAND_n(rand, plaintext, 320);
	TIME_OPERATION_SECONDS(oqs_aes128_load_schedule_c(key, &schedule), "oqs_aes128_load_schedule_c", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_enc_c(plaintext, schedule, ciphertext), "oqs_aes128_enc_c", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_dec_c(ciphertext, schedule, decrypted), "oqs_aes128_dec_c", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_enc_c(plaintext, 320, key, ciphertext), "oqs_aes128_ecb_enc_c", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_dec_c(ciphertext, 320, key, decrypted), "oqs_aes128_ecb_dec_c", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_enc_sch_c(plaintext, 320, schedule, ciphertext), "oqs_aes128_ecb_enc_sch_c", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_dec_sch_c(ciphertext, 320, schedule, decrypted), "oqs_aes128_ecb_dec_sch_c", BENCH_DURATION);
}

#ifndef AES_DISABLE_NI
static void speed_aes128_ni(OQS_RAND *rand) {
	uint8_t key[16], plaintext[320], ciphertext[320], decrypted[320];
	void *schedule = NULL;
	OQS_RAND_n(rand, key, 16);
	OQS_RAND_n(rand, plaintext, 320);
	TIME_OPERATION_SECONDS(oqs_aes128_load_schedule_ni(key, &schedule), "oqs_aes128_load_schedule_ni", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_enc_ni(plaintext, schedule, ciphertext), "oqs_aes128_enc_ni", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_dec_ni(ciphertext, schedule, decrypted), "oqs_aes128_dec_ni", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_enc_ni(plaintext, 320, key, ciphertext), "oqs_aes128_ecb_enc_ni", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_dec_ni(ciphertext, 320, key, decrypted), "oqs_aes128_ecb_dec_ni", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_enc_sch_ni(plaintext, 320, schedule, ciphertext), "oqs_aes128_ecb_enc_sch_ni", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_dec_sch_ni(ciphertext, 320, schedule, decrypted), "oqs_aes128_ecb_dec_sch_ni", BENCH_DURATION);
}
#endif

#ifdef USE_OPENSSL
static void speed_aes128_ossl(OQS_RAND *rand) {
	uint8_t key[16], plaintext[320], ciphertext[320];
	void *schedule = NULL;
	OQS_RAND_n(rand, key, 16);
	OQS_RAND_n(rand, plaintext, 320);
	TIME_OPERATION_SECONDS(oqs_aes128_load_schedule_ossl(key, &schedule, 1), "oqs_aes128_load_schedule_ossl 1", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_load_schedule_ossl(key, &schedule, 0), "oqs_aes128_load_schedule_ossl 0", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_enc_ossl(plaintext, 320, key, ciphertext), "oqs_aes128_ecb_enc_ossl", BENCH_DURATION);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_dec_ossl(ciphertext, 320, key, plaintext), "oqs_aes128_ecb_dec_ossl", BENCH_DURATION);
	oqs_aes128_load_schedule_ossl(key, &schedule, 1);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_enc_sch_ossl(plaintext, 320, schedule, ciphertext), "oqs_aes128_ecb_enc_sch_ossl", BENCH_DURATION);
	oqs_aes128_load_schedule_ossl(key, &schedule, 0);
	TIME_OPERATION_SECONDS(oqs_aes128_ecb_dec_sch_ossl(ciphertext, 320, schedule, plaintext), "oqs_aes128_ecb_dec_sch_ossl", BENCH_DURATION);
}
#endif

int main() {
	int ret;
	printf("=== test_aes correctness ===\n");
	OQS_RAND *rand = OQS_RAND_new(OQS_RAND_alg_default);
	if (rand == NULL) {
		fprintf(stderr, "OQS_RAND_new() failed\n");
		goto err;
	}
	TEST_REPEATEDLY(test_aes128_correctness_c(rand));
#ifndef AES_DISABLE_NI
	TEST_REPEATEDLY(test_aes128_correctness_ni(rand));
	TEST_REPEATEDLY(test_aes128_c_equals_ni(rand));
#endif
	TEST_REPEATEDLY(test_aes128_ecb_correctness_c(rand));
#ifndef AES_DISABLE_NI
	TEST_REPEATEDLY(test_aes128_ecb_correctness_ni(rand));
#endif
#ifdef USE_OPENSSL
	TEST_REPEATEDLY(test_aes128_ecb_correctness_ossl(rand));
#endif
	printf("Tests passed.\n\n");
	printf("=== test_aes performance ===\n");
	PRINT_TIMER_HEADER
	speed_aes128_c(rand);
#ifndef AES_DISABLE_NI
	speed_aes128_ni(rand);
#endif
#ifdef USE_OPENSSL
	speed_aes128_ossl(rand);
#endif
	PRINT_TIMER_FOOTER
	ret = EXIT_SUCCESS;
	goto cleanup;
err:
	ret = EXIT_FAILURE;
cleanup:
	OQS_RAND_free(rand);
	return ret;
}
