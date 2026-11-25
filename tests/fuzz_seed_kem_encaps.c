/*
 * Generate a reproducible valid-public-key seed for every enabled KEM.
 * Run with a destination corpus directory that already exists.
 *
 * SPDX-License-Identifier: MIT
 */

#include <oqs/kem.h>
#include <oqs/oqs.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t random_state;

static void seed_random(uint32_t seed) {
	random_state = seed == 0 ? 0x6d2b79f5U : seed;
}

static void fuzz_randombytes(uint8_t *output, size_t length) {
	for (size_t i = 0; i < length; i++) {
		random_state ^= random_state << 13;
		random_state ^= random_state >> 17;
		random_state ^= random_state << 5;
		output[i] = (uint8_t) random_state;
	}
}

static void write_u32_le(uint8_t *output, uint32_t value) {
	output[0] = (uint8_t) value;
	output[1] = (uint8_t) (value >> 8);
	output[2] = (uint8_t) (value >> 16);
	output[3] = (uint8_t) (value >> 24);
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <existing-corpus-directory>\n", argv[0]);
		return EXIT_FAILURE;
	}

	OQS_init();
	int result = EXIT_SUCCESS;
	for (size_t index = 0; index < OQS_KEM_algs_length; index++) {
		const char *algorithm = OQS_KEM_alg_identifier(index);
		OQS_KEM *kem = OQS_KEM_new(algorithm);
		if (kem == NULL) {
			continue;
		}

		uint8_t *public_key = OQS_MEM_malloc(kem->length_public_key);
		uint8_t *secret_key = OQS_MEM_malloc(kem->length_secret_key);
		if (public_key == NULL || secret_key == NULL) {
			fprintf(stderr, "Could not allocate key buffers for %s\n", algorithm);
			OQS_MEM_insecure_free(public_key);
			OQS_MEM_secure_free(secret_key, kem->length_secret_key);
			OQS_KEM_free(kem);
			result = EXIT_FAILURE;
			break;
		}

		uint32_t random_seed = 0x4f515300U ^ (uint32_t) index;
		seed_random(random_seed);
		OQS_randombytes_custom_algorithm(&fuzz_randombytes);
		if (OQS_KEM_keypair(kem, public_key, secret_key) != OQS_SUCCESS) {
			fprintf(stderr, "Could not generate seed public key for %s\n", algorithm);
			OQS_MEM_insecure_free(public_key);
			OQS_MEM_secure_free(secret_key, kem->length_secret_key);
			OQS_KEM_free(kem);
			result = EXIT_FAILURE;
			break;
		}

		char path[4096];
		int path_len = snprintf(path, sizeof(path), "%s/kem_%zu", argv[1], index);
		if (path_len < 0 || (size_t) path_len >= sizeof(path)) {
			fprintf(stderr, "Corpus path is too long\n");
			OQS_MEM_insecure_free(public_key);
			OQS_MEM_secure_free(secret_key, kem->length_secret_key);
			OQS_KEM_free(kem);
			result = EXIT_FAILURE;
			break;
		}

		FILE *file = fopen(path, "wb");
		if (file == NULL) {
			perror(path);
			OQS_MEM_insecure_free(public_key);
			OQS_MEM_secure_free(secret_key, kem->length_secret_key);
			OQS_KEM_free(kem);
			result = EXIT_FAILURE;
			break;
		}

		uint8_t header[8];
		write_u32_le(header, random_seed);
		write_u32_le(header + sizeof(uint32_t), (uint32_t) index);
		bool write_failed = fwrite(header, 1, sizeof(header), file) != sizeof(header) ||
		                    fwrite(public_key, 1, kem->length_public_key, file) != kem->length_public_key;
		if (fclose(file) != 0 || write_failed) {
			fprintf(stderr, "Could not write seed for %s\n", algorithm);
			remove(path);
			OQS_MEM_insecure_free(public_key);
			OQS_MEM_secure_free(secret_key, kem->length_secret_key);
			OQS_KEM_free(kem);
			result = EXIT_FAILURE;
			break;
		}

		OQS_MEM_insecure_free(public_key);
		OQS_MEM_secure_free(secret_key, kem->length_secret_key);
		OQS_KEM_free(kem);
	}
	OQS_destroy();
	return result;
}
