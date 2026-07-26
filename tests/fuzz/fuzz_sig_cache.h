// SPDX-License-Identifier: MIT

#ifndef OQS_TESTS_FUZZ_SIG_CACHE_H
#define OQS_TESTS_FUZZ_SIG_CACHE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <oqs/oqs.h>

#define OQS_FUZZ_SIG_BASE_MESSAGE_LEN 32
#define OQS_FUZZ_SIG_BASE_CTX_LEN 4

typedef struct {
	bool initialized;
	bool available;
	OQS_SIG *sig;
	uint8_t *public_key;
	uint8_t *message;
	uint8_t *signature;
	size_t signature_len;
	uint8_t *ctx;
	uint8_t *ctx_signature;
	size_t ctx_signature_len;
	uint8_t *scratch_public_key;
	uint8_t *scratch_message;
	uint8_t *scratch_signature;
	uint8_t *scratch_ctx;
} oqs_fuzz_sig_cache_entry;

oqs_fuzz_sig_cache_entry *oqs_fuzz_sig_cache_get(size_t algorithm_index);

#endif
