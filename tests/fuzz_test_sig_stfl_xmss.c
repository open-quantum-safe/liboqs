/*
 * fuzz_test_sig_stfl_xmss.c
 *
 * Fuzz test for XMSS stateful hash-based signature verification in liboqs.
 * Targets OQS_SIG_STFL_verify for the following parameter sets as specified
 * in https://github.com/open-quantum-safe/liboqs/issues/2399:
 *   - XMSS-SHA2_10_256
 *   - XMSSMT-SHA2_20/2_256
 *
 * The fuzzer mutates the signature, message, and public_key inputs fed to
 * OQS_SIG_STFL_verify to detect crashes, memory errors, and undefined
 * behaviour. Key generation is skipped — keypairs are generated once at
 * startup and reused across fuzzing iterations.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

/* The two target algorithms specified in issue #2399 */
static const char *TARGET_ALGS[] = {
	"XMSS-SHA2_10_256",
	"XMSSMT-SHA2_20/2_256",
};
static const size_t NUM_TARGET_ALGS = 2;

/*
 * Fuzz input layout:
 *   [0]              algorithm selector byte (index into TARGET_ALGS)
 *   [1..pk_len]      mutated public key
 *   [pk_len+1..end]  mutated signature + message (split at sig_len boundary)
 */

typedef struct {
	OQS_SIG_STFL *sig;
	uint8_t *public_key;
	uint8_t *signature;
	size_t signature_len;
	uint8_t *message;
	size_t message_len;
} stfl_keypair_t;

/* One cached keypair per target algorithm, initialised once */
static stfl_keypair_t cached[2];
static bool initialised = false;

static OQS_STATUS do_nothing_save(uint8_t *key_buf, size_t buf_len,
                                  void *context) {
	(void)context;
	(void)buf_len;
	return key_buf != NULL ? OQS_SUCCESS : OQS_ERROR;
}

static void cleanup_cached(void) {
	for (size_t i = 0; i < NUM_TARGET_ALGS; i++) {
		if (cached[i].sig != NULL) {
			OQS_MEM_insecure_free(cached[i].public_key);
			OQS_MEM_insecure_free(cached[i].signature);
			OQS_MEM_secure_free(cached[i].message, cached[i].message_len);
			OQS_SIG_STFL_free(cached[i].sig);
			cached[i].sig = NULL;
		}
	}
}

/*
 * Generate keypairs once at startup for both target algorithms.
 * Key generation for XMSS can be slow — this avoids doing it on
 * every fuzzing iteration.
 */
static OQS_STATUS init_keypairs(void) {
	for (size_t i = 0; i < NUM_TARGET_ALGS; i++) {
		cached[i].sig = NULL;
		cached[i].public_key = NULL;
		cached[i].signature = NULL;
		cached[i].message = NULL;
	}

	for (size_t i = 0; i < NUM_TARGET_ALGS; i++) {
		OQS_SIG_STFL *sig = OQS_SIG_STFL_new(TARGET_ALGS[i]);
		if (sig == NULL) {
			fprintf(stderr, "ERROR: %s not enabled at compile-time.\n",
			        TARGET_ALGS[i]);
			cleanup_cached();
			return OQS_ERROR;
		}
		cached[i].sig = sig;

		cached[i].public_key = OQS_MEM_malloc(sig->length_public_key);
		cached[i].message_len = 50;
		cached[i].message = OQS_MEM_malloc(cached[i].message_len);
		cached[i].signature = OQS_MEM_malloc(sig->length_signature);

		if (!cached[i].public_key || !cached[i].message ||
		        !cached[i].signature) {
			fprintf(stderr, "ERROR: OQS_MEM_malloc failed for %s.\n",
			        TARGET_ALGS[i]);
			cleanup_cached();
			return OQS_ERROR;
		}

		OQS_SIG_STFL_SECRET_KEY *sk = OQS_SIG_STFL_SECRET_KEY_new(TARGET_ALGS[i]);
		if (sk == NULL) {
			fprintf(stderr, "ERROR: SECRET_KEY_new failed for %s.\n",
			        TARGET_ALGS[i]);
			cleanup_cached();
			return OQS_ERROR;
		}

		OQS_SIG_STFL_SECRET_KEY_SET_store_cb(sk, do_nothing_save, NULL);

		OQS_STATUS rc = OQS_SIG_STFL_keypair(sig, cached[i].public_key, sk);
		if (rc != OQS_SUCCESS) {
			fprintf(stderr, "ERROR: keypair failed for %s.\n", TARGET_ALGS[i]);
			OQS_SIG_STFL_SECRET_KEY_free(sk);
			cleanup_cached();
			return OQS_ERROR;
		}

		OQS_randombytes(cached[i].message, cached[i].message_len);

		rc = OQS_SIG_STFL_sign(sig, cached[i].signature,
		                       &cached[i].signature_len,
		                       cached[i].message, cached[i].message_len, sk);
		OQS_SIG_STFL_SECRET_KEY_free(sk);

		if (rc != OQS_SUCCESS) {
			fprintf(stderr, "ERROR: sign failed for %s.\n", TARGET_ALGS[i]);
			cleanup_cached();
			return OQS_ERROR;
		}
	}
	return OQS_SUCCESS;
}

static OQS_STATUS fuzz_sig_stfl_xmss(const uint8_t *data, size_t data_len) {
	if (data_len < 1) {
		return OQS_ERROR;
	}

	/* Select target algorithm from first byte of fuzz input */
	size_t alg_idx = data[0] % NUM_TARGET_ALGS;
	stfl_keypair_t *kp = &cached[alg_idx];
	OQS_SIG_STFL *sig = kp->sig;

	const uint8_t *fuzz_data = data + 1;
	size_t fuzz_len = data_len - 1;

	/*
	 * Build mutated public_key, signature, and message from fuzz input.
	 * Use the real sizes from the sig object but fill with fuzzed bytes.
	 * If fuzz input is too short, zero-pad.
	 */
	size_t pk_len = sig->length_public_key;
	size_t sig_len = kp->signature_len;
	size_t msg_len = kp->message_len;

	uint8_t *mutated_pk = OQS_MEM_malloc(pk_len);
	uint8_t *mutated_sig = OQS_MEM_malloc(sig_len);
	uint8_t *mutated_msg = OQS_MEM_malloc(msg_len);

	if (!mutated_pk || !mutated_sig || !mutated_msg) {
		OQS_MEM_insecure_free(mutated_pk);
		OQS_MEM_insecure_free(mutated_sig);
		OQS_MEM_insecure_free(mutated_msg);
		return OQS_ERROR;
	}

	/* Start from real values, then overwrite with fuzz bytes */
	memcpy(mutated_pk, kp->public_key, pk_len);
	memcpy(mutated_sig, kp->signature, sig_len);
	memcpy(mutated_msg, kp->message, msg_len);

	size_t offset = 0;

	if (offset < fuzz_len) {
		size_t copy = (fuzz_len - offset) < pk_len ? (fuzz_len - offset) : pk_len;
		memcpy(mutated_pk, fuzz_data + offset, copy);
		offset += copy;
	}
	if (offset < fuzz_len) {
		size_t copy = (fuzz_len - offset) < sig_len ? (fuzz_len - offset) : sig_len;
		memcpy(mutated_sig, fuzz_data + offset, copy);
		offset += copy;
	}
	if (offset < fuzz_len) {
		size_t copy = (fuzz_len - offset) < msg_len ? (fuzz_len - offset) : msg_len;
		memcpy(mutated_msg, fuzz_data + offset, copy);
	}

	/*
	 * Call OQS_SIG_STFL_verify with mutated inputs.
	 * We expect OQS_ERROR for invalid inputs — a crash here is a bug.
	 */
	OQS_SIG_STFL_verify(sig, mutated_msg, msg_len, mutated_sig, sig_len,
	                    mutated_pk);

	OQS_MEM_insecure_free(mutated_pk);
	OQS_MEM_insecure_free(mutated_sig);
	OQS_MEM_secure_free(mutated_msg, msg_len);

	return OQS_SUCCESS;
}

int LLVMFuzzerInitialize(int *argc, char ***argv) {
	(void)argc;
	(void)argv;
	OQS_init();
	if (OQS_SUCCESS != init_keypairs()) {
		return -1;
	}
	initialised = true;
	return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
	if (!initialised) {
		return -1;
	}
	fuzz_sig_stfl_xmss(data, size);
	return 0;
}
