// SPDX-License-Identifier: MIT

#ifndef OQS_TESTS_FUZZ_KEM_CACHE_H
#define OQS_TESTS_FUZZ_KEM_CACHE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <oqs/oqs.h>

typedef struct {
	bool initialized;
	bool available;
	bool has_ciphertext;
	OQS_KEM *kem;
	uint8_t *public_key;
	uint8_t *secret_key;
	uint8_t *ciphertext;
	uint8_t *scratch_public_key;
	uint8_t *scratch_ciphertext;
	uint8_t *scratch_shared_secret;
	uint8_t *scratch_seed;
} oqs_fuzz_kem_cache_entry;

oqs_fuzz_kem_cache_entry *oqs_fuzz_kem_cache_get(size_t algorithm_index, bool need_ciphertext);

#endif
