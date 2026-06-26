/*
 * FUZZ TARGET: OQS_KEM_encaps() / OQS_KEM_encaps_derand()
 * SOURCE FILE: src/kem/kem.c
 *
 * ATTACK SURFACE:
 *   Remote peers and protocol wrappers pass attacker-controlled serialized
 *   KEM public keys into the generic encapsulation API.
 *
 * ATTACK SCENARIO:
 *   An attacker supplies a malformed public key that reaches a scheme-specific
 *   unpack/decoder through OQS_KEM_encaps before any shared secret exists.
 *
 * POTENTIAL IMPACT:
 *   - Memory corruption or out-of-bounds reads in public-key decoders
 *   - Denial of service from malformed public-key handling
 *
 * REPRODUCTION IN REAL DEPLOYMENT:
 *   1. Save the crashing corpus input.
 *   2. Re-run this target with the same algorithm enabled and ASan active.
 *   3. Pass the extracted public-key bytes to the application KEM encaps path.
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
	oqs_fuzz_kem_cache_entry *entry = oqs_fuzz_kem_cache_get(algorithm_index, false);
	if (entry == NULL) {
		return 0;
	}

	oqs_fuzz_copy_baseline(entry->scratch_public_key, entry->kem->length_public_key, entry->public_key, entry->kem->length_public_key);
	size_t public_key_offset = (size_t)oqs_fuzz_take_u32(&cursor, header.flags);
	oqs_fuzz_patch(entry->scratch_public_key, entry->kem->length_public_key, public_key_offset, &cursor, oqs_fuzz_remaining(&cursor));

	oqs_fuzz_seed_prng(oqs_fuzz_hash(data, size, UINT64_C(0x454e434150530000)));
	if ((header.mode & 1u) != 0 && entry->kem->length_encaps_seed > 0) {
		OQS_randombytes(entry->scratch_seed, entry->kem->length_encaps_seed);
		(void)OQS_KEM_encaps_derand(entry->kem, entry->scratch_ciphertext, entry->scratch_shared_secret, entry->scratch_public_key, entry->scratch_seed);
	} else {
		(void)OQS_KEM_encaps(entry->kem, entry->scratch_ciphertext, entry->scratch_shared_secret, entry->scratch_public_key);
	}
	return 0;
}
