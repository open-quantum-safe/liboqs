/*
 * Fuzz KEM encapsulation with attacker-controlled public keys.
 *
 * Input layout: little-endian uint32 random seed, little-endian uint32
 * algorithm index, followed by public-key bytes. The key is zero-padded or
 * truncated to the selected algorithm's public-key length.
 *
 * SPDX-License-Identifier: MIT
 */

#include <oqs/kem.h>
#include <oqs/oqs.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_HEADER_SIZE 8

typedef struct {
	uint32_t random_seed;
	uint32_t algorithm_index;
	const uint8_t *public_key;
	size_t public_key_len;
} fuzz_context_t;

static uint32_t random_state;

static uint32_t read_u32_le(const uint8_t *input) {
	return (uint32_t) input[0] |
	       ((uint32_t) input[1] << 8) |
	       ((uint32_t) input[2] << 16) |
	       ((uint32_t) input[3] << 24);
}

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

static fuzz_context_t init_fuzz_context(const uint8_t *data, size_t size) {
	fuzz_context_t ctx = {0, 0, data, size};
	if (size >= INPUT_HEADER_SIZE) {
		ctx.random_seed = read_u32_le(data);
		ctx.algorithm_index = read_u32_le(data + sizeof(uint32_t));
		ctx.public_key = data + INPUT_HEADER_SIZE;
		ctx.public_key_len = size - INPUT_HEADER_SIZE;
	}
	ctx.algorithm_index %= OQS_KEM_algs_length;
	seed_random(ctx.random_seed);
	OQS_randombytes_custom_algorithm(&fuzz_randombytes);
	return ctx;
}

static void cleanup_buffers(uint8_t *public_key, uint8_t *ciphertext,
                            uint8_t *ciphertext_repeat, uint8_t *shared_secret,
                            uint8_t *shared_secret_repeat, uint8_t *seed,
                            OQS_KEM *kem) {
	if (kem != NULL) {
		OQS_MEM_secure_free(shared_secret, kem->length_shared_secret);
		OQS_MEM_secure_free(shared_secret_repeat, kem->length_shared_secret);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(ciphertext);
	OQS_MEM_insecure_free(ciphertext_repeat);
	OQS_MEM_insecure_free(seed);
	OQS_KEM_free(kem);
}

static OQS_STATUS fuzz_kem_encaps(const uint8_t *data, size_t size) {
	fuzz_context_t ctx = init_fuzz_context(data, size);
	const char *algorithm = OQS_KEM_alg_identifier(ctx.algorithm_index);
	OQS_KEM *kem = OQS_KEM_new(algorithm);
	if (kem == NULL) {
		return OQS_ERROR;
	}

	uint8_t *public_key = OQS_MEM_malloc(kem->length_public_key);
	uint8_t *ciphertext = OQS_MEM_malloc(kem->length_ciphertext);
	uint8_t *shared_secret = OQS_MEM_malloc(kem->length_shared_secret);
	uint8_t *ciphertext_repeat = NULL;
	uint8_t *shared_secret_repeat = NULL;
	uint8_t *seed = NULL;

	if (public_key == NULL || ciphertext == NULL || shared_secret == NULL) {
		cleanup_buffers(public_key, ciphertext, ciphertext_repeat,
		                shared_secret, shared_secret_repeat, seed, kem);
		return OQS_ERROR;
	}

	memset(public_key, 0, kem->length_public_key);
	size_t public_key_bytes = ctx.public_key_len < kem->length_public_key ?
	                          ctx.public_key_len : kem->length_public_key;
	if (public_key_bytes > 0) {
		memcpy(public_key, ctx.public_key, public_key_bytes);
	}

	(void) OQS_KEM_encaps(kem, ciphertext, shared_secret, public_key);

	if (kem->encaps_derand != NULL && kem->length_encaps_seed > 0) {
		ciphertext_repeat = OQS_MEM_malloc(kem->length_ciphertext);
		shared_secret_repeat = OQS_MEM_malloc(kem->length_shared_secret);
		seed = OQS_MEM_malloc(kem->length_encaps_seed);
		if (ciphertext_repeat == NULL || shared_secret_repeat == NULL || seed == NULL) {
			cleanup_buffers(public_key, ciphertext, ciphertext_repeat,
			                shared_secret, shared_secret_repeat, seed, kem);
			return OQS_ERROR;
		}

		memset(seed, 0, kem->length_encaps_seed);
		size_t seed_bytes = size < kem->length_encaps_seed ? size : kem->length_encaps_seed;
		if (seed_bytes > 0) {
			memcpy(seed, data, seed_bytes);
		}

		OQS_STATUS first = OQS_KEM_encaps_derand(kem, ciphertext, shared_secret,
		                                        public_key, seed);
		OQS_STATUS second = OQS_KEM_encaps_derand(kem, ciphertext_repeat,
		                                         shared_secret_repeat, public_key,
		                                         seed);
		if (first != second ||
		        (first == OQS_SUCCESS &&
		         (memcmp(ciphertext, ciphertext_repeat, kem->length_ciphertext) != 0 ||
		          memcmp(shared_secret, shared_secret_repeat, kem->length_shared_secret) != 0))) {
			fprintf(stderr, "Derandomized encapsulation is not deterministic for %s\n", algorithm);
			abort();
		}
	}

	cleanup_buffers(public_key, ciphertext, ciphertext_repeat,
	                shared_secret, shared_secret_repeat, seed, kem);
	return OQS_SUCCESS;
}

int LLVMFuzzerTestOneInput(const char *data, size_t size) {
	OQS_init();
	OQS_STATUS status = fuzz_kem_encaps((const uint8_t *) data, size);
	OQS_destroy();
	return status == OQS_SUCCESS ? 0 : -1;
}
