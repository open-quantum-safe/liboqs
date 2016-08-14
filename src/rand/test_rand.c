#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include <oqs/rand.h>

#define RAND_TEST_ITERATIONS 10000000L

static void rand_test_distribution_8(OQS_RAND *rand, unsigned long occurrences[256], int iterations) {
	uint8_t b;
	for (int i = 0; i < iterations; i++) {
		b = OQS_RAND_8(rand);
		OQS_RAND_test_record_occurrence(b, occurrences);
	}
}

static void rand_test_distribution_32(OQS_RAND *rand, unsigned long occurrences[256], int iterations) {
	uint32_t x;
	for (int i = 0; i < iterations; i++) {
		x = OQS_RAND_32(rand);
		uint8_t b;
		for (int j = 0; j < sizeof(uint32_t); j++) {
			b = (x >> j) & 0xFF;
			OQS_RAND_test_record_occurrence(b, occurrences);
		}
	}
}

static void rand_test_distribution_64(OQS_RAND *rand, unsigned long occurrences[256], int iterations) {
	uint64_t x;
	for (int i = 0; i < iterations; i++) {
		x = OQS_RAND_64(rand);
		uint8_t b;
		for (int j = 0; j < sizeof(uint64_t); j++) {
			b = (x >> j) & 0xFF;
			OQS_RAND_test_record_occurrence(b, occurrences);
		}
	}
}

static int rand_test_distribution_n(OQS_RAND *rand, unsigned long occurrences[256], int len) {
	uint8_t *x = malloc(len);
	if (x == NULL) return 0;
	OQS_RAND_n(rand, x, len);
	for (int i = 0; i < len; i++) {
		OQS_RAND_test_record_occurrence(x[i], occurrences);
	}
	free(x);
	return 1;
}

static int rand_test_distribution_wrapper(OQS_RAND *(*new_method)(), int iterations) {

	OQS_RAND *rand = new_method();
	if (rand == NULL) {
		fprintf(stderr, "rand is NULL\n");
		return 0;
	}

	printf("================================================================================\n");
	printf("Testing distribution of PRNG %s\n", rand->method_name);
	printf("================================================================================\n");

	unsigned long occurrences[256];
	for (int i = 0; i < 256; i++) {
		occurrences[i] = 0;
	}

	printf("1-byte mode for %d iterations\n", 8*iterations);
	rand_test_distribution_8(rand, occurrences, 8*iterations);
	printf("    Statistical distance from uniform: %12.10f\n", OQS_RAND_test_statistical_distance_from_uniform(occurrences));

	for (int i = 0; i < 256; i++) {
		occurrences[i] = 0;
	}

	printf("4-byte mode for %d iterations\n", 2*iterations);
	rand_test_distribution_32(rand, occurrences, 2*iterations);
	printf("    Statistical distance from uniform: %12.10f\n", OQS_RAND_test_statistical_distance_from_uniform(occurrences));

	for (int i = 0; i < 256; i++) {
		occurrences[i] = 0;
	}

	printf("8-byte mode for %d iterations\n", iterations);
	rand_test_distribution_64(rand, occurrences, iterations);
	printf("    Statistical distance from uniform: %12.10f\n", OQS_RAND_test_statistical_distance_from_uniform(occurrences));

	for (int i = 0; i < 256; i++) {
		occurrences[i] = 0;
	}

	printf("n-byte mode for %d bytes\n", 8*iterations);
	rand_test_distribution_n(rand, occurrences, 8*iterations);
	printf("    Statistical distance from uniform: %12.10f\n", OQS_RAND_test_statistical_distance_from_uniform(occurrences));

	OQS_RAND_free(rand);

	return 1;

}

int main() {

	int ret;

	ret = rand_test_distribution_wrapper(&OQS_RAND_new, RAND_TEST_ITERATIONS);
	if (ret != 1) goto err;

	ret = 1;
	goto cleanup;

err:
	ret = 0;
	fprintf(stderr, "ERROR!\n");

cleanup:

	return ret;

}
