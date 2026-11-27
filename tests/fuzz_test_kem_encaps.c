/*
 * fuzz_test_kem_encaps.c
 *
 * Fuzz KEM encapsulation with mutated public keys.
 *
 * Input layout:
 *   [ random_seed (4 bytes) | algorithm_index (4 bytes) | pk_mask | encaps_seed ]
 *
 * A valid key pair is generated from random_seed, then the first
 * length_public_key bytes after the header are XOR-ed onto the public key.
 * Missing bytes leave the key unchanged, so an empty input encapsulates against
 * a valid key and every mutation starts from one. The next length_encaps_seed
 * bytes seed OQS_KEM_encaps_derand where the algorithm supports it.
 *
 * Rejecting a malformed public key is correct behaviour. The harness fails only
 * on a sanitizer finding or when two derandomized encapsulations with the same
 * public key and seed disagree.
 *
 * Build and run (Clang):
 *   export CFLAGS=-fsanitize=fuzzer-no-link,address
 *   export LDFLAGS=-fsanitize=address
 *   cmake -S . -B build/fuzz -GNinja -DCMAKE_BUILD_TYPE=Debug -DOQS_BUILD_FUZZ_TESTS=ON
 *   ninja -C build/fuzz fuzz_test_kem_encaps
 *   build/fuzz/tests/fuzz_test_kem_encaps -max_total_time=300
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

typedef struct {
	uint32_t random_seed;
	uint32_t algorithm_index;
} fuzz_init_ctx_t;

static void fuzz_rand(uint8_t *random_array, size_t bytes_to_read) {
	for (size_t i = 0; i < bytes_to_read; i++) {
		random_array[i] = (uint8_t)rand();
	}
}

/* Copies up to len bytes from the input cursor into out, zero-filling the rest. */
static void take(uint8_t *out, size_t len, const uint8_t **data, size_t *data_len) {
	size_t n = *data_len < len ? *data_len : len;
	memset(out, 0, len);
	if (n > 0) {
		memcpy(out, *data, n);
	}
	*data += n;
	*data_len -= n;
}

static int fuzz_kem_encaps(const uint8_t *data, size_t data_len) {
	fuzz_init_ctx_t init = {0, 0};
	if (data_len >= sizeof(init)) {
		memcpy(&init, data, sizeof(init));
		data += sizeof(init);
		data_len -= sizeof(init);
	}
	srand(init.random_seed);
	OQS_randombytes_custom_algorithm(&fuzz_rand);

	const char *algorithm = OQS_KEM_alg_identifier(init.algorithm_index % OQS_KEM_algs_length);
	OQS_KEM *kem = OQS_KEM_new(algorithm);
	if (kem == NULL) {
		return -1; /* not enabled at compile time: keep out of the corpus */
	}

	uint8_t *public_key = OQS_MEM_malloc(kem->length_public_key);
	uint8_t *secret_key = OQS_MEM_malloc(kem->length_secret_key);
	uint8_t *pk_mask = OQS_MEM_malloc(kem->length_public_key);
	uint8_t *ciphertext = OQS_MEM_malloc(kem->length_ciphertext);
	uint8_t *ciphertext2 = OQS_MEM_malloc(kem->length_ciphertext);
	uint8_t *shared_secret = OQS_MEM_malloc(kem->length_shared_secret);
	uint8_t *shared_secret2 = OQS_MEM_malloc(kem->length_shared_secret);
	/* length_encaps_seed is 0 for schemes without derandomized encapsulation. */
	uint8_t *encaps_seed = OQS_MEM_malloc(kem->length_encaps_seed + 1);
	int ret = 0;

	if (public_key == NULL || secret_key == NULL || pk_mask == NULL || ciphertext == NULL ||
	        ciphertext2 == NULL || shared_secret == NULL || shared_secret2 == NULL || encaps_seed == NULL) {
		fprintf(stderr, "ERROR: OQS_MEM_malloc failed!\n");
		ret = -1;
		goto cleanup;
	}

	if (OQS_KEM_keypair(kem, public_key, secret_key) != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_keypair failed for %s!\n", algorithm);
		abort();
	}

	take(pk_mask, kem->length_public_key, &data, &data_len);
	for (size_t i = 0; i < kem->length_public_key; i++) {
		public_key[i] ^= pk_mask[i];
	}

	/* The status is not checked: rejecting a malformed key is allowed. */
	(void)OQS_KEM_encaps(kem, ciphertext, shared_secret, public_key);

	if (kem->encaps_derand != NULL) {
		take(encaps_seed, kem->length_encaps_seed, &data, &data_len);
		OQS_STATUS rc1 = OQS_KEM_encaps_derand(kem, ciphertext, shared_secret, public_key, encaps_seed);
		OQS_STATUS rc2 = OQS_KEM_encaps_derand(kem, ciphertext2, shared_secret2, public_key, encaps_seed);
		if (rc1 != rc2 || (rc1 == OQS_SUCCESS &&
		                   (memcmp(ciphertext, ciphertext2, kem->length_ciphertext) != 0 ||
		                    memcmp(shared_secret, shared_secret2, kem->length_shared_secret) != 0))) {
			fprintf(stderr, "ERROR: OQS_KEM_encaps_derand is not deterministic for %s!\n", algorithm);
			abort();
		}
	}

cleanup:
	OQS_MEM_secure_free(secret_key, kem->length_secret_key);
	OQS_MEM_secure_free(shared_secret, kem->length_shared_secret);
	OQS_MEM_secure_free(shared_secret2, kem->length_shared_secret);
	OQS_MEM_secure_free(encaps_seed, kem->length_encaps_seed + 1);
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(pk_mask);
	OQS_MEM_insecure_free(ciphertext);
	OQS_MEM_insecure_free(ciphertext2);
	OQS_KEM_free(kem);
	return ret;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
	OQS_init();
	int ret = fuzz_kem_encaps(data, size);
	OQS_destroy();
	return ret;
}
