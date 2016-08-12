#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

#include <oqs/rand.h>

int main() {
	OQS_RAND *r = OQS_RAND_new();
	if (r == NULL) {
		fprintf(stderr, "r is NULL\n");
		return -1;
	}
	for (int i = 0; i < 128; i++) {
		uint8_t b;
		b = OQS_RAND_8(r);
		printf("%0" PRIx8, b);
	}
	printf("\n");
	for (int i = 0; i < 128 / 4; i++) {
		uint32_t b = OQS_RAND_32(r);
		printf("%0" PRIx32, b);
	}
	printf("\n");
	for (int i = 0; i < 128 / 8; i++) {
		uint64_t b = OQS_RAND_64(r);
		printf("%0" PRIx64, b);
	}
	printf("\n");
	OQS_RAND_free(r);
}
