/*
 * FUZZ TARGET: OQS_KEM_decaps()
 * SOURCE FILE: src/kem/kem.c
 *
 * ATTACK SURFACE:
 *   Remote peers provide attacker-controlled KEM ciphertexts to decapsulation
 *   APIs during key establishment.
 *
 * ATTACK SCENARIO:
 *   An attacker sends a malformed ciphertext that reaches scheme-specific
 *   ciphertext unpacking and rejection logic with a valid local secret key.
 *
 * POTENTIAL IMPACT:
 *   - Memory corruption or out-of-bounds reads in ciphertext decoders
 *   - Denial of service during unauthenticated key establishment
 *
 * REPRODUCTION IN REAL DEPLOYMENT:
 *   1. Save the crashing corpus input.
 *   2. Re-run this target with the same algorithm enabled and ASan active.
 *   3. Deliver the extracted ciphertext to the application decapsulation path.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stddef.h>
#include <stdint.h>

#include <oqs/oqs.h>

#include "fuzz/fuzz_helpers.h"
#include "fuzz/fuzz_kem_cache.h"

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

	size_t algorithm_index = (size_t)header.algorithm_index % OQS_KEM_algs_length;
	oqs_fuzz_kem_cache_entry *entry = oqs_fuzz_kem_cache_get(algorithm_index, true);
	if (entry == NULL) {
		return 0;
	}

	oqs_fuzz_copy_baseline(entry->scratch_ciphertext, entry->kem->length_ciphertext, entry->ciphertext, entry->kem->length_ciphertext);
	size_t ciphertext_offset = (size_t)oqs_fuzz_take_u32(&cursor, header.flags);
	oqs_fuzz_patch(entry->scratch_ciphertext, entry->kem->length_ciphertext, ciphertext_offset, &cursor, oqs_fuzz_remaining(&cursor));

	(void)OQS_KEM_decaps(entry->kem, entry->scratch_shared_secret, entry->scratch_ciphertext, entry->secret_key);
	return 0;
}
