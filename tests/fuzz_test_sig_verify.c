// SPDX-License-Identifier: MIT

// Fuzz the signature *verification* path with attacker-controlled bytes.
//
// tests/fuzz_test_sig.c generates a fresh keypair, signs fuzzer-provided data
// with it and verifies the result, so the public key and signature it hands to
// OQS_SIG_verify are always well formed and only the message is mutated. In a
// real protocol the message is the one part the attacker does not choose: the
// signature, and in many deployments the public key, arrive from the wire.
//
// This harness drives OQS_SIG_verify with those mutated instead, across the
// algorithms the build actually enabled.
//
// Two strategies, chosen per input by a flag bit, because they reach different
// code:
//   MUTATE - start from a genuine keypair and signature, then flip bytes. Keeps
//            enough structure to clear the early length and format checks and
//            reach the arithmetic underneath.
//   RANDOM - fill both buffers from the fuzz data outright. Exercises the early
//            rejection paths and whatever parsing precedes them.
//
// Buffer sizes are always the sizes the algorithm declares; passing a short
// buffer would be API misuse rather than a finding. signature_len is a real API
// parameter, so it is varied across [0, length_signature].

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oqs/oqs.h>

typedef struct {
	uint32_t algorithm_index;
	uint32_t siglen_sel;
	uint8_t strategy;
	uint8_t mutations;
	uint8_t pad[2];
} fuzz_hdr_t;

// Selecting modulo OQS_SIG_algs_length would pick from every identifier liboqs
// knows rather than the ones this build contains, so most inputs would be spent
// on a disabled algorithm. Collect the enabled set once.
static int sig_enabled[OQS_SIG_algs_length];
static size_t sig_enabled_count;

static const char *enabled_alg(uint32_t raw) {
	if (sig_enabled_count == 0) {
		for (size_t i = 0; i < OQS_SIG_algs_length; i++) {
			const char *name = OQS_SIG_alg_identifier(i);
			if (name != NULL && OQS_SIG_alg_is_enabled(name)) {
				sig_enabled[sig_enabled_count++] = (int)i;
			}
		}
	}
	if (sig_enabled_count == 0) {
		return NULL;
	}
	return OQS_SIG_alg_identifier((size_t)sig_enabled[raw % sig_enabled_count]);
}

static void fill_from(uint8_t *dst, size_t dst_len, const uint8_t *src,
                      size_t src_len, uint32_t salt) {
	if (src_len == 0) {
		memset(dst, (int)(salt & 0xFF), dst_len);
		return;
	}
	for (size_t i = 0; i < dst_len; i++) {
		dst[i] = (uint8_t)(src[(i + salt) % src_len] ^ (uint8_t)(i * 31u + salt));
	}
}

#define STRATEGY_MUTATE 0
#define STRATEGY_RANDOM 1

static void fuzz_verify(const uint8_t *data, size_t data_len) {
	if (data_len <= sizeof(fuzz_hdr_t)) {
		return;
	}
	fuzz_hdr_t hdr;
	memcpy(&hdr, data, sizeof(hdr));
	const uint8_t *body = data + sizeof(hdr);
	const size_t body_len = data_len - sizeof(hdr);

	const char *alg = enabled_alg(hdr.algorithm_index);
	if (alg == NULL) {
		return;
	}
	OQS_SIG *sig = OQS_SIG_new(alg);
	if (sig == NULL) {
		return;
	}

	uint8_t *pk = malloc(sig->length_public_key);
	uint8_t *sk = malloc(sig->length_secret_key);
	uint8_t *sm = malloc(sig->length_signature);
	uint8_t *msg = malloc(body_len);
	uint8_t *pk0 = malloc(sig->length_public_key);
	uint8_t *sm0 = malloc(sig->length_signature);
	if (!pk || !sk || !sm || !msg || !pk0 || !sm0) {
		goto done;
	}

	memcpy(msg, body, body_len);
	size_t sig_len = sig->length_signature;
	const uint8_t strategy = (uint8_t)(hdr.strategy & 1u);

	if (strategy == STRATEGY_MUTATE) {
		if (OQS_SIG_keypair(sig, pk, sk) != OQS_SUCCESS) {
			goto done;
		}
		if (OQS_SIG_sign(sig, sm, &sig_len, msg, body_len, sk) != OQS_SUCCESS) {
			goto done;
		}
		memcpy(pk0, pk, sig->length_public_key);
		memcpy(sm0, sm, sig->length_signature);

		// Advance an LCG per flip and mix the body in, so two flips cannot land
		// on the same index with the same delta and XOR back to the original.
		// Indexing straight off f cancelled out entirely for short inputs,
		// which made every "mutated" signature verify as genuine.
		const unsigned flips = (hdr.mutations % 16u) + 1u;
		uint32_t h = hdr.mutations * 2654435761u + (uint32_t)body_len;
		for (unsigned f = 0; f < flips; f++) {
			h = h * 1664525u + 1013904223u + body[f % body_len];
			const uint8_t delta = (uint8_t)((h >> 24) | 1u);
			if ((f & 1u) == 0u && sig_len > 0) {
				sm[h % sig_len] ^= delta;
			} else {
				pk[h % sig->length_public_key] ^= delta;
			}
		}
		// The forgery check below only means something if a byte really moved.
		// Guarantee it rather than trusting the loop.
		if (memcmp(sm0, sm, sig->length_signature) == 0 &&
		    memcmp(pk0, pk, sig->length_public_key) == 0) {
			sm[0] ^= 0xFFu;
		}
	} else {
		fill_from(pk, sig->length_public_key, body, body_len, hdr.algorithm_index);
		fill_from(sm, sig->length_signature, body, body_len, hdr.siglen_sel);
	}

	// signature_len is caller-supplied, so a wrong one must be rejected rather
	// than trusted. Exercise the whole permitted range, including 0.
	if ((hdr.siglen_sel & 0x80000000u) != 0u) {
		sig_len = (size_t)(hdr.siglen_sel & 0x7FFFFFFFu) % (sig->length_signature + 1u);
	}

	// The contract under test: for any bytes, verify returns a status and does
	// not read out of bounds, double free or crash. ASan is the oracle.
	const OQS_STATUS rc = OQS_SIG_verify(sig, msg, body_len, sm, sig_len, pk);

	// A mutated signature or public key verifying as genuine is a forgery, not
	// a crash, so check it explicitly. Re-confirm the buffers really differ
	// before claiming one, so a no-op mutation cannot be mistaken for a break.
	if (rc == OQS_SUCCESS && strategy == STRATEGY_MUTATE) {
		const bool changed =
		    (memcmp(sm0, sm, sig->length_signature) != 0) ||
		    (memcmp(pk0, pk, sig->length_public_key) != 0);
		if (changed && sig_len == sig->length_signature) {
			fprintf(stderr, "FORGERY: %s accepted a mutated signature/public key\n", alg);
			abort();
		}
	}

done:
	if (sig != NULL) {
		OQS_MEM_secure_free(sk, sig->length_secret_key);
	} else {
		OQS_MEM_insecure_free(sk);
	}
	OQS_MEM_insecure_free(pk);
	OQS_MEM_insecure_free(sm);
	OQS_MEM_insecure_free(msg);
	OQS_MEM_insecure_free(pk0);
	OQS_MEM_insecure_free(sm0);
	OQS_SIG_free(sig);
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
	OQS_init();
	fuzz_verify(data, size);
	OQS_destroy();
	return 0;
}
