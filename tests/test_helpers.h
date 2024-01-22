// SPDX-License-Identifier: MIT
#ifndef OQS_TEST_HELPERS_H
#define OQS_TEST_HELPERS_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <oqs/sha3.h>

typedef union {
	OQS_SHA3_shake256_inc_ctx hqc_state;
	OQS_NIST_DRBG_struct nist_state;
} OQS_KAT_PRNG_state;

typedef struct {
	size_t max_kats;
	OQS_KAT_PRNG_state saved_state;
	// The caller should use the OQS_KAT_PRNG_* functions instead of these callbacks.
	void (*seed)(const uint8_t *, const uint8_t *);
	void (*get_state)(void *);
	void (*set_state)(const void *);
	void (*free)(OQS_KAT_PRNG_state *);
} OQS_KAT_PRNG;

OQS_KAT_PRNG *OQS_KAT_PRNG_new(const char *method_name);

void OQS_KAT_PRNG_seed(OQS_KAT_PRNG *prng, const uint8_t *seed, const uint8_t *personalization_string);

void OQS_KAT_PRNG_save_state(OQS_KAT_PRNG *prng);

void OQS_KAT_PRNG_restore_state(OQS_KAT_PRNG *prng);

void OQS_KAT_PRNG_free(OQS_KAT_PRNG *prng);

void OQS_print_hex_string(const char *label, const uint8_t *str, size_t len);

void OQS_fprintBstr(FILE *fp, const char *S, const uint8_t *A, size_t L);

#endif
