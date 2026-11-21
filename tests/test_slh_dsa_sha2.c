// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <oqs/oqs.h>
#include "sha2_api.h"
#include "../src/common/sha2/sha2.h"

/* Exercise the private SLH-DSA hash implementation, including the cloned
 * chaining states used for public-seed prefixes and both padding boundaries. */
int main(void) {
	uint8_t message[4096], expected[64], actual[64];
	for (size_t i = 0; i < sizeof(message); i++) {
		message[i] = (uint8_t)(i * 197 + i / 11);
	}
	OQS_init();
	for (size_t len = 0; len <= sizeof(message); len += len < 260 ? 1 : 37) {
		for (size_t split = 0; split <= len; split += split < 130 ? 1 : 113) {
			sha2_256_t s256, copy256;
			sha2_512_t s512, copy512;
			OQS_SHA2_sha256(expected, message, len);
			sha2_256_init(&s256);
			sha2_256_update(&s256, message, split);
			sha2_256_copy(&copy256, &s256);
			sha2_256_update(&copy256, message + split, len - split);
			sha2_256_final(&copy256, actual);
			if (memcmp(actual, expected, 32) != 0) {
				fprintf(stderr, "SLH SHA-256 mismatch: length %zu split %zu\n", len, split);
				return 1;
			}
			OQS_SHA2_sha512(expected, message, len);
			sha2_512_init(&s512);
			sha2_512_update(&s512, message, split);
			sha2_512_copy(&copy512, &s512);
			sha2_512_update(&copy512, message + split, len - split);
			sha2_512_final(&copy512, actual);
			if (memcmp(actual, expected, 64) != 0) {
				fprintf(stderr, "SLH SHA-512 mismatch: length %zu split %zu\n", len, split);
				return 1;
			}
		}
		OQS_SHA2_sha384(expected, message, len);
		sha2_384(actual, message, len);
		if (memcmp(actual, expected, 48) != 0) {
			fprintf(stderr, "SLH SHA-384 mismatch: length %zu\n", len);
			return 1;
		}
	}
	OQS_destroy();
	puts("SLH-DSA private SHA-2 differential tests passed");
	return 0;
}
