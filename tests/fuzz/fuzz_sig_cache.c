// SPDX-License-Identifier: MIT

#include "fuzz_sig_cache.h"

#include "fuzz_helpers.h"

static oqs_fuzz_sig_cache_entry oqs_fuzz_sig_cache[OQS_SIG_algs_length];

oqs_fuzz_sig_cache_entry *oqs_fuzz_sig_cache_get(size_t algorithm_index) {
	if (algorithm_index >= OQS_SIG_algs_length) {
		return NULL;
	}

	oqs_fuzz_sig_cache_entry *entry = &oqs_fuzz_sig_cache[algorithm_index];
	if (entry->initialized) {
		return entry->available ? entry : NULL;
	}
	entry->initialized = true;

	const char *method_name = OQS_SIG_alg_identifier(algorithm_index);
	entry->sig = OQS_SIG_new(method_name);
	if (entry->sig == NULL) {
		return NULL;
	}

	uint8_t *secret_key = OQS_MEM_malloc(entry->sig->length_secret_key);
	entry->public_key = OQS_MEM_malloc(entry->sig->length_public_key);
	entry->message = OQS_MEM_malloc(OQS_FUZZ_SIG_BASE_MESSAGE_LEN);
	entry->signature = OQS_MEM_malloc(entry->sig->length_signature);
	entry->ctx = OQS_MEM_malloc(OQS_FUZZ_SIG_BASE_CTX_LEN);
	entry->ctx_signature = OQS_MEM_malloc(entry->sig->length_signature);
	entry->scratch_public_key = OQS_MEM_malloc(entry->sig->length_public_key);
	entry->scratch_message = OQS_MEM_malloc(OQS_FUZZ_MAX_MESSAGE_LEN);
	entry->scratch_signature = OQS_MEM_malloc(entry->sig->length_signature + OQS_FUZZ_SIGNATURE_SLACK);
	entry->scratch_ctx = OQS_MEM_malloc(OQS_FUZZ_MAX_CTX_LEN);
	if (secret_key == NULL || entry->public_key == NULL || entry->message == NULL || entry->signature == NULL || entry->ctx == NULL || entry->ctx_signature == NULL ||
	        entry->scratch_public_key == NULL || entry->scratch_message == NULL || entry->scratch_signature == NULL || entry->scratch_ctx == NULL) {
		OQS_MEM_secure_free(secret_key, entry->sig->length_secret_key);
		return NULL;
	}

	for (size_t i = 0; i < OQS_FUZZ_SIG_BASE_MESSAGE_LEN; i++) {
		entry->message[i] = (uint8_t)(0x40u + (uint8_t)i);
	}
	for (size_t i = 0; i < OQS_FUZZ_SIG_BASE_CTX_LEN; i++) {
		entry->ctx[i] = (uint8_t)(0xa0u + (uint8_t)i);
	}

	oqs_fuzz_seed_prng(UINT64_C(0x5349470000000000) ^ (uint64_t)algorithm_index);
	if (OQS_SIG_keypair(entry->sig, entry->public_key, secret_key) != OQS_SUCCESS) {
		OQS_MEM_secure_free(secret_key, entry->sig->length_secret_key);
		return NULL;
	}

	if (OQS_SIG_sign(entry->sig, entry->signature, &entry->signature_len, entry->message, OQS_FUZZ_SIG_BASE_MESSAGE_LEN, secret_key) != OQS_SUCCESS) {
		OQS_MEM_secure_free(secret_key, entry->sig->length_secret_key);
		return NULL;
	}

	if (entry->sig->sig_with_ctx_support) {
		oqs_fuzz_seed_prng(UINT64_C(0x4354580000000000) ^ (uint64_t)algorithm_index);
		if (OQS_SIG_sign_with_ctx_str(entry->sig, entry->ctx_signature, &entry->ctx_signature_len, entry->message, OQS_FUZZ_SIG_BASE_MESSAGE_LEN, entry->ctx, OQS_FUZZ_SIG_BASE_CTX_LEN, secret_key) != OQS_SUCCESS) {
			entry->ctx_signature_len = 0;
		}
	}

	OQS_MEM_secure_free(secret_key, entry->sig->length_secret_key);
	entry->available = true;
	return entry;
}
