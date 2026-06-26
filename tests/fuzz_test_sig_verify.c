/*
 * FUZZ TARGET: OQS_SIG_verify() / OQS_SIG_verify_with_ctx_str()
 * SOURCE FILE: src/sig/sig.c
 *
 * ATTACK SURFACE:
 *   Remote callers control serialized public keys, signatures, messages, and
 *   context strings supplied to generic signature verification APIs.
 *
 * ATTACK SCENARIO:
 *   An attacker sends malformed verification inputs that reach scheme-specific
 *   signature/public-key decoders and context handling before authentication.
 *
 * POTENTIAL IMPACT:
 *   - Memory corruption or out-of-bounds reads in signature decoders
 *   - Alternate encodings or context handling bugs that bypass verification
 *   - Denial of service from malformed verification inputs
 *
 * REPRODUCTION IN REAL DEPLOYMENT:
 *   1. Save the crashing corpus input.
 *   2. Re-run this target with the same algorithm enabled and ASan active.
 *   3. Feed the extracted verification tuple to the application verify path.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <oqs/oqs.h>

#include "fuzz/fuzz_helpers.h"
#include "fuzz/fuzz_sig_cache.h"

static void patch_fields(uint8_t **fields, const size_t *field_lens, const size_t *offsets, size_t field_count, size_t first_field, oqs_fuzz_cursor *cursor) {
	for (size_t i = 0; i < field_count; i++) {
		size_t field_index = (first_field + i) % field_count;
		size_t remaining_fields = field_count - i;
		size_t remaining = oqs_fuzz_remaining(cursor);
		size_t patch_len = remaining_fields > 0 ? (remaining + remaining_fields - 1) / remaining_fields : 0;
		oqs_fuzz_patch(fields[field_index], field_lens[field_index], offsets[field_index], cursor, patch_len);
	}
}

int LLVMFuzzerInitialize(int *argc, char ***argv) {
	(void)argc;
	(void)argv;
	oqs_fuzz_init();
	return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
	oqs_fuzz_header header;
	oqs_fuzz_cursor cursor;
	if (!oqs_fuzz_read_header(data, size, &header, &cursor)) {
		return 0;
	}

	size_t algorithm_index = (size_t)header.algorithm_index % (size_t)(OQS_SIG_algs_length);
	oqs_fuzz_sig_cache_entry *entry = oqs_fuzz_sig_cache_get(algorithm_index);
	if (entry == NULL) {
		return 0;
	}

	bool use_ctx_api = (header.mode % 3u) != 0;
	bool null_ctx = (header.mode % 3u) == 2;
	const uint8_t *baseline_signature = entry->signature;
	size_t baseline_signature_len = entry->signature_len;
	const uint8_t *baseline_ctx = NULL;
	size_t baseline_ctx_len = 0;
	if (use_ctx_api && !null_ctx && entry->ctx_signature_len > 0) {
		baseline_signature = entry->ctx_signature;
		baseline_signature_len = entry->ctx_signature_len;
		baseline_ctx = entry->ctx;
		baseline_ctx_len = OQS_FUZZ_SIG_BASE_CTX_LEN;
	}

	size_t max_signature_len = entry->sig->length_signature + OQS_FUZZ_SIGNATURE_SLACK;
	size_t signature_len = oqs_fuzz_take_size(&cursor, max_signature_len, baseline_signature_len);
	size_t message_len = oqs_fuzz_take_size(&cursor, OQS_FUZZ_MAX_MESSAGE_LEN, OQS_FUZZ_SIG_BASE_MESSAGE_LEN);
	size_t ctx_len = null_ctx ? 0 : oqs_fuzz_take_size(&cursor, OQS_FUZZ_MAX_CTX_LEN, baseline_ctx_len);

	size_t offsets[4];
	for (size_t i = 0; i < 4; i++) {
		offsets[i] = (size_t)oqs_fuzz_take_u32(&cursor, 0);
	}

	oqs_fuzz_copy_baseline(entry->scratch_public_key, entry->sig->length_public_key, entry->public_key, entry->sig->length_public_key);
	oqs_fuzz_copy_baseline(entry->scratch_signature, max_signature_len, baseline_signature, baseline_signature_len);
	oqs_fuzz_copy_baseline(entry->scratch_message, OQS_FUZZ_MAX_MESSAGE_LEN, entry->message, OQS_FUZZ_SIG_BASE_MESSAGE_LEN);
	oqs_fuzz_copy_baseline(entry->scratch_ctx, OQS_FUZZ_MAX_CTX_LEN, baseline_ctx, baseline_ctx_len);

	uint8_t *fields[4] = {
		entry->scratch_public_key,
		entry->scratch_signature,
		entry->scratch_message,
		entry->scratch_ctx,
	};
	size_t field_lens[4] = {
		entry->sig->length_public_key,
		signature_len,
		message_len,
		ctx_len,
	};
	size_t field_count = null_ctx ? 3 : 4;
	patch_fields(fields, field_lens, offsets, field_count, (size_t)header.flags % field_count, &cursor);

	if (use_ctx_api) {
		const uint8_t *ctx = null_ctx ? NULL : entry->scratch_ctx;
		(void)OQS_SIG_verify_with_ctx_str(entry->sig, entry->scratch_message, message_len, entry->scratch_signature, signature_len, ctx, ctx_len, entry->scratch_public_key);
	} else {
		(void)OQS_SIG_verify(entry->sig, entry->scratch_message, message_len, entry->scratch_signature, signature_len, entry->scratch_public_key);
	}
	return 0;
}
