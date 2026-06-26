// SPDX-License-Identifier: MIT

#include "fuzz_kem_cache.h"

#include "fuzz_helpers.h"

static oqs_fuzz_kem_cache_entry oqs_fuzz_kem_cache[OQS_KEM_algs_length];

oqs_fuzz_kem_cache_entry *oqs_fuzz_kem_cache_get(size_t algorithm_index, bool need_ciphertext) {
	if (algorithm_index >= OQS_KEM_algs_length) {
		return NULL;
	}

	oqs_fuzz_kem_cache_entry *entry = &oqs_fuzz_kem_cache[algorithm_index];
	if (!entry->initialized) {
		entry->initialized = true;
		const char *method_name = OQS_KEM_alg_identifier(algorithm_index);
		entry->kem = OQS_KEM_new(method_name);
		if (entry->kem == NULL) {
			return NULL;
		}

		entry->public_key = OQS_MEM_malloc(entry->kem->length_public_key);
		entry->secret_key = OQS_MEM_malloc(entry->kem->length_secret_key);
		entry->ciphertext = OQS_MEM_malloc(entry->kem->length_ciphertext);
		entry->scratch_public_key = OQS_MEM_malloc(entry->kem->length_public_key);
		entry->scratch_ciphertext = OQS_MEM_malloc(entry->kem->length_ciphertext);
		entry->scratch_shared_secret = OQS_MEM_malloc(entry->kem->length_shared_secret);
		size_t seed_len = entry->kem->length_encaps_seed > entry->kem->length_keypair_seed ? entry->kem->length_encaps_seed : entry->kem->length_keypair_seed;
		entry->scratch_seed = seed_len > 0 ? OQS_MEM_malloc(seed_len) : NULL;
		if (entry->public_key == NULL || entry->secret_key == NULL || entry->ciphertext == NULL ||
		        entry->scratch_public_key == NULL || entry->scratch_ciphertext == NULL || entry->scratch_shared_secret == NULL ||
		        (seed_len > 0 && entry->scratch_seed == NULL)) {
			return NULL;
		}

		oqs_fuzz_seed_prng(UINT64_C(0x4b454d0000000000) ^ (uint64_t)algorithm_index);
		if (OQS_KEM_keypair(entry->kem, entry->public_key, entry->secret_key) != OQS_SUCCESS) {
			return NULL;
		}
		entry->available = true;
	}

	if (!entry->available) {
		return NULL;
	}

	if (need_ciphertext && !entry->has_ciphertext) {
		uint8_t *shared_secret = OQS_MEM_malloc(entry->kem->length_shared_secret);
		if (shared_secret == NULL) {
			return NULL;
		}
		oqs_fuzz_seed_prng(UINT64_C(0x4349504845520000) ^ (uint64_t)algorithm_index);
		OQS_STATUS rc = OQS_KEM_encaps(entry->kem, entry->ciphertext, shared_secret, entry->public_key);
		OQS_MEM_secure_free(shared_secret, entry->kem->length_shared_secret);
		if (rc != OQS_SUCCESS) {
			return NULL;
		}
		entry->has_ciphertext = true;
	}

	return entry;
}
