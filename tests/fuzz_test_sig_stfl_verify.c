/*
 * FUZZ TARGET: OQS_SIG_STFL_verify()
 * SOURCE FILE: src/sig_stfl/sig_stfl.c
 *
 * ATTACK SURFACE:
 *   Remote callers control serialized XMSS/XMSSMT/LMS public keys and
 *   signatures passed into the generic stateful verification API.
 *
 * ATTACK SCENARIO:
 *   An attacker sends malformed stateful-signature inputs that reach hash-tree
 *   signature decoders and verifier state reconstruction before authentication.
 *
 * POTENTIAL IMPACT:
 *   - Memory corruption or out-of-bounds reads in stateful signature decoders
 *   - Denial of service from malformed signature/public-key handling
 *
 * REPRODUCTION IN REAL DEPLOYMENT:
 *   1. Save the crashing corpus input.
 *   2. Re-run this target with the same stateful algorithm enabled.
 *   3. Feed the extracted verification tuple to the application verify path.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <oqs/oqs.h>

#include "fuzz/fuzz_helpers.h"

typedef struct {
	bool initialized;
	bool available;
	OQS_SIG_STFL *sig;
	uint8_t *public_key;
	uint8_t *message;
	size_t message_len;
	uint8_t *signature;
	size_t signature_len;
	uint8_t *scratch_public_key;
	uint8_t *scratch_message;
	uint8_t *scratch_signature;
} oqs_fuzz_stfl_verify_entry;

static oqs_fuzz_stfl_verify_entry oqs_fuzz_stfl_verify_cache[OQS_SIG_STFL_algs_length];

static oqs_fuzz_stfl_verify_entry *get_entry(size_t algorithm_index) {
	if (algorithm_index >= OQS_SIG_STFL_algs_length) {
		return NULL;
	}

	oqs_fuzz_stfl_verify_entry *entry = &oqs_fuzz_stfl_verify_cache[algorithm_index];
	if (entry->initialized) {
		return entry->available ? entry : NULL;
	}
	entry->initialized = true;

	const char *method_name = OQS_SIG_STFL_alg_identifier(algorithm_index);
	entry->sig = OQS_SIG_STFL_new(method_name);
	if (entry->sig == NULL) {
		return NULL;
	}

	size_t max_signature_len = entry->sig->length_signature + OQS_FUZZ_SIGNATURE_SLACK;
	entry->public_key = OQS_MEM_malloc(entry->sig->length_public_key);
	entry->message = OQS_MEM_malloc(OQS_FUZZ_MAX_MESSAGE_LEN);
	entry->signature = OQS_MEM_malloc(entry->sig->length_signature);
	entry->scratch_public_key = OQS_MEM_malloc(entry->sig->length_public_key);
	entry->scratch_message = OQS_MEM_malloc(OQS_FUZZ_MAX_MESSAGE_LEN);
	entry->scratch_signature = OQS_MEM_malloc(max_signature_len);
	if (entry->public_key == NULL || entry->message == NULL || entry->signature == NULL ||
	        entry->scratch_public_key == NULL || entry->scratch_message == NULL || entry->scratch_signature == NULL) {
		return NULL;
	}

	entry->message_len = 32;
	entry->signature_len = entry->sig->length_signature;
	oqs_fuzz_copy_baseline(entry->public_key, entry->sig->length_public_key, NULL, 0);
	oqs_fuzz_copy_baseline(entry->message, OQS_FUZZ_MAX_MESSAGE_LEN, NULL, 0);
	oqs_fuzz_copy_baseline(entry->signature, entry->sig->length_signature, NULL, 0);

	oqs_fuzz_stfl_kat kat;
	if (oqs_fuzz_load_stfl_kat(method_name, &kat)) {
		if (kat.public_key != NULL && kat.public_key_len == entry->sig->length_public_key) {
			memcpy(entry->public_key, kat.public_key, kat.public_key_len);
		}
		if (kat.message != NULL && kat.message_len <= OQS_FUZZ_MAX_MESSAGE_LEN) {
			memcpy(entry->message, kat.message, kat.message_len);
			entry->message_len = kat.message_len;
		}
		if (kat.signature != NULL && kat.signature_len <= entry->sig->length_signature) {
			memcpy(entry->signature, kat.signature, kat.signature_len);
			entry->signature_len = kat.signature_len;
		}
		oqs_fuzz_free_stfl_kat(&kat);
	}

	entry->available = true;
	return entry;
}

static void patch_fields(uint8_t **fields, const size_t *field_lens, const size_t *offsets, size_t first_field, oqs_fuzz_cursor *cursor) {
	for (size_t i = 0; i < 3; i++) {
		size_t field_index = (first_field + i) % 3;
		size_t remaining_fields = 3 - i;
		size_t remaining = oqs_fuzz_remaining(cursor);
		size_t patch_len = (remaining + remaining_fields - 1) / remaining_fields;
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

	size_t algorithm_index = (size_t)header.algorithm_index % OQS_SIG_STFL_algs_length;
	oqs_fuzz_stfl_verify_entry *entry = get_entry(algorithm_index);
	if (entry == NULL) {
		return 0;
	}

	size_t max_signature_len = entry->sig->length_signature + OQS_FUZZ_SIGNATURE_SLACK;
	size_t signature_len = oqs_fuzz_take_size(&cursor, max_signature_len, entry->signature_len);
	size_t message_len = oqs_fuzz_take_size(&cursor, OQS_FUZZ_MAX_MESSAGE_LEN, entry->message_len);
	size_t offsets[3];
	for (size_t i = 0; i < 3; i++) {
		offsets[i] = (size_t)oqs_fuzz_take_u32(&cursor, 0);
	}

	oqs_fuzz_copy_baseline(entry->scratch_public_key, entry->sig->length_public_key, entry->public_key, entry->sig->length_public_key);
	oqs_fuzz_copy_baseline(entry->scratch_message, OQS_FUZZ_MAX_MESSAGE_LEN, entry->message, entry->message_len);
	oqs_fuzz_copy_baseline(entry->scratch_signature, max_signature_len, entry->signature, entry->signature_len);

	uint8_t *fields[3] = {
		entry->scratch_public_key,
		entry->scratch_signature,
		entry->scratch_message,
	};
	size_t field_lens[3] = {
		entry->sig->length_public_key,
		signature_len,
		message_len,
	};
	patch_fields(fields, field_lens, offsets, (size_t)header.flags % 3, &cursor);

	(void)OQS_SIG_STFL_verify(entry->sig, entry->scratch_message, message_len, entry->scratch_signature, signature_len, entry->scratch_public_key);
	return 0;
}
