/*
 * FUZZ TARGET: OQS_SIG_STFL_SECRET_KEY_deserialize()
 * SOURCE FILE: src/sig_stfl/sig_stfl.c
 *
 * ATTACK SURFACE:
 *   Applications deserialize persisted or remotely supplied XMSS/XMSSMT/LMS
 *   secret-key material through the generic stateful signature API.
 *
 * ATTACK SCENARIO:
 *   An attacker controls serialized stateful secret-key bytes or lengths and
 *   triggers parsing before later round-trip serialization.
 *
 * POTENTIAL IMPACT:
 *   - Memory corruption or out-of-bounds reads during deserialization
 *   - Corrupted state handling that crashes later serialization
 *   - Denial of service while loading persisted key material
 *
 * REPRODUCTION IN REAL DEPLOYMENT:
 *   1. Save the crashing corpus input.
 *   2. Re-run this target with hazardous STFL key/sign support enabled.
 *   3. Load the extracted serialized key through the application persistence path.
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
	uint8_t *serialized_key;
	size_t serialized_key_len;
	uint8_t *scratch_serialized_key;
} oqs_fuzz_stfl_deserialize_entry;

static oqs_fuzz_stfl_deserialize_entry oqs_fuzz_stfl_deserialize_cache[OQS_SIG_STFL_algs_length];
static int oqs_fuzz_store_context;

static OQS_STATUS fuzz_lock(void *mutex) {
	return mutex != NULL ? OQS_SUCCESS : OQS_ERROR;
}

static OQS_STATUS fuzz_unlock(void *mutex) {
	return mutex != NULL ? OQS_SUCCESS : OQS_ERROR;
}

static OQS_STATUS fuzz_store(uint8_t *sk_buf, size_t sk_buf_len, void *context) {
	return sk_buf != NULL && sk_buf_len > 0 && context != NULL ? OQS_SUCCESS : OQS_ERROR;
}

static oqs_fuzz_stfl_deserialize_entry *get_entry(size_t algorithm_index) {
	if (algorithm_index >= OQS_SIG_STFL_algs_length) {
		return NULL;
	}

	oqs_fuzz_stfl_deserialize_entry *entry = &oqs_fuzz_stfl_deserialize_cache[algorithm_index];
	if (entry->initialized) {
		return entry->available ? entry : NULL;
	}
	entry->initialized = true;

	const char *method_name = OQS_SIG_STFL_alg_identifier(algorithm_index);
	entry->sig = OQS_SIG_STFL_new(method_name);
	if (entry->sig == NULL || entry->sig->length_secret_key > OQS_FUZZ_MAX_SERIALIZED_KEY_LEN) {
		return NULL;
	}

	entry->serialized_key = OQS_MEM_malloc(OQS_FUZZ_MAX_SERIALIZED_KEY_LEN);
	entry->scratch_serialized_key = OQS_MEM_malloc(OQS_FUZZ_MAX_SERIALIZED_KEY_LEN);
	if (entry->serialized_key == NULL || entry->scratch_serialized_key == NULL) {
		return NULL;
	}

	entry->serialized_key_len = entry->sig->length_secret_key;
	oqs_fuzz_copy_baseline(entry->serialized_key, OQS_FUZZ_MAX_SERIALIZED_KEY_LEN, NULL, 0);

	oqs_fuzz_stfl_kat kat;
	if (oqs_fuzz_load_stfl_kat(method_name, &kat)) {
		if (kat.secret_key != NULL && kat.secret_key_len <= OQS_FUZZ_MAX_SERIALIZED_KEY_LEN) {
			memcpy(entry->serialized_key, kat.secret_key, kat.secret_key_len);
			entry->serialized_key_len = kat.secret_key_len;
		}
		oqs_fuzz_free_stfl_kat(&kat);
	}

	if (strncmp(method_name, "LMS_", 4) == 0) {
		entry->serialized_key_len = 64;
		(void)oqs_fuzz_make_lms_secret_key(method_name, entry->serialized_key, OQS_FUZZ_MAX_SERIALIZED_KEY_LEN);
	}

	entry->available = true;
	return entry;
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
	oqs_fuzz_stfl_deserialize_entry *entry = get_entry(algorithm_index);
	if (entry == NULL) {
		return 0;
	}

	size_t serialized_key_len = oqs_fuzz_take_size(&cursor, OQS_FUZZ_MAX_SERIALIZED_KEY_LEN, entry->serialized_key_len);
	size_t serialized_key_offset = (size_t)oqs_fuzz_take_u32(&cursor, header.flags);
	oqs_fuzz_copy_baseline(entry->scratch_serialized_key, OQS_FUZZ_MAX_SERIALIZED_KEY_LEN, entry->serialized_key, entry->serialized_key_len);
	oqs_fuzz_patch(entry->scratch_serialized_key, OQS_FUZZ_MAX_SERIALIZED_KEY_LEN, serialized_key_offset, &cursor, oqs_fuzz_remaining(&cursor));

	OQS_SIG_STFL_SECRET_KEY *secret_key = OQS_SIG_STFL_SECRET_KEY_new(entry->sig->method_name);
	if (secret_key == NULL) {
		return 0;
	}

	if (OQS_SIG_STFL_SECRET_KEY_deserialize(secret_key, entry->scratch_serialized_key, serialized_key_len, &oqs_fuzz_store_context) == OQS_SUCCESS) {
		OQS_SIG_STFL_SECRET_KEY_SET_lock(secret_key, &fuzz_lock);
		OQS_SIG_STFL_SECRET_KEY_SET_unlock(secret_key, &fuzz_unlock);
		OQS_SIG_STFL_SECRET_KEY_SET_mutex(secret_key, &oqs_fuzz_store_context);
		OQS_SIG_STFL_SECRET_KEY_SET_store_cb(secret_key, &fuzz_store, &oqs_fuzz_store_context);
		(void)OQS_SIG_STFL_SECRET_KEY_lock(secret_key);
		(void)OQS_SIG_STFL_SECRET_KEY_unlock(secret_key);

		uint8_t *serialized_roundtrip = NULL;
		size_t serialized_roundtrip_len = 0;
		if (OQS_SIG_STFL_SECRET_KEY_serialize(&serialized_roundtrip, &serialized_roundtrip_len, secret_key) == OQS_SUCCESS) {
			OQS_MEM_secure_free(serialized_roundtrip, serialized_roundtrip_len);
		}

	}

	OQS_SIG_STFL_SECRET_KEY_free(secret_key);
	return 0;
}
