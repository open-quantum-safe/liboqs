// SPDX-License-Identifier: MIT
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <oqs/oqs.h>
#include <oqs/rand_nist.h>  // Internal NIST DRBG API
#include <oqs/sha3.h>       // Internal SHA3 API

#include "test_helpers.h"

/* HQC PRNG implementation */

// State for HQC PRNG. Analogue of DRBG_ctx in rand/rand_nist.c
static OQS_SHA3_shake256_inc_ctx hqc_prng_state = { NULL };

// Allocate the state.
static void hqc_prng_new(void) {
	OQS_SHA3_shake256_inc_init(&hqc_prng_state);
}

// entropy_input must have length 48.
// If personalization_string is non-null, its length must also be 48.
static void hqc_prng_seed(const uint8_t *entropy_input, const uint8_t *personalization_string) {
	uint8_t domain = 1;
	// reset state
	OQS_SHA3_shake256_inc_ctx_reset(&hqc_prng_state);
	OQS_SHA3_shake256_inc_absorb(&hqc_prng_state, entropy_input, 48);
	if (personalization_string != NULL) {
		OQS_SHA3_shake256_inc_absorb(&hqc_prng_state, personalization_string, 48);
	}
	OQS_SHA3_shake256_inc_absorb(&hqc_prng_state, &domain, 1);
	OQS_SHA3_shake256_inc_finalize(&hqc_prng_state);
}

// random_array must have length bytes_to_read.
static void hqc_prng_randombytes(uint8_t *random_array, size_t bytes_to_read) {
	OQS_SHA3_shake256_inc_squeeze(random_array, bytes_to_read, &hqc_prng_state);
}

static void hqc_prng_get_state(void *out) {
	OQS_SHA3_shake256_inc_ctx_clone((OQS_SHA3_shake256_inc_ctx *)out, &hqc_prng_state);
}

static void hqc_prng_set_state(const void *in) {
	OQS_SHA3_shake256_inc_ctx_clone(&hqc_prng_state, (const OQS_SHA3_shake256_inc_ctx *)in);
}

static void hqc_prng_free(OQS_KAT_PRNG_state *saved_state) {
	if (saved_state != NULL) {
		OQS_SHA3_shake256_inc_ctx_release(&saved_state->hqc_state);
	}
	if (hqc_prng_state.ctx != NULL) {
		OQS_SHA3_shake256_inc_ctx_release(&hqc_prng_state);
		hqc_prng_state.ctx = NULL;
	}
}

/* Helpers for identifying algorithms */
static int is_mceliece(const char *method_name) {
	return ( !strcmp(method_name, OQS_KEM_alg_classic_mceliece_348864)
	         || !strcmp(method_name, OQS_KEM_alg_classic_mceliece_348864f)
	         || !strcmp(method_name, OQS_KEM_alg_classic_mceliece_460896)
	         || !strcmp(method_name, OQS_KEM_alg_classic_mceliece_460896f)
	         || !strcmp(method_name, OQS_KEM_alg_classic_mceliece_6688128)
	         || !strcmp(method_name, OQS_KEM_alg_classic_mceliece_6688128f)
	         || !strcmp(method_name, OQS_KEM_alg_classic_mceliece_6960119)
	         || !strcmp(method_name, OQS_KEM_alg_classic_mceliece_6960119f)
	         || !strcmp(method_name, OQS_KEM_alg_classic_mceliece_8192128)
	         || !strcmp(method_name, OQS_KEM_alg_classic_mceliece_8192128f) );
}

static int is_hqc(const char *method_name) {
	return (0 == strcmp(method_name, OQS_KEM_alg_hqc_128))
	       || (0 == strcmp(method_name, OQS_KEM_alg_hqc_192))
	       || (0 == strcmp(method_name, OQS_KEM_alg_hqc_256));
}

/* OQS_KAT_PRNG interface implementation */

OQS_KAT_PRNG *OQS_KAT_PRNG_new(const char *method_name) {
	OQS_KAT_PRNG *prng = malloc(sizeof(OQS_KAT_PRNG));
	if (prng != NULL) {
		prng->max_kats = is_mceliece(method_name) ? 10 : 100;
		if (is_hqc(method_name)) {
			// set randombytes function
			OQS_randombytes_custom_algorithm(&hqc_prng_randombytes);
			// reset the PRNG
			hqc_prng_new();
			// initialize saved state
			OQS_SHA3_shake256_inc_init(&prng->saved_state.hqc_state);
			prng->seed = &hqc_prng_seed;
			prng->get_state = &hqc_prng_get_state;
			prng->set_state = &hqc_prng_set_state;
			prng->free = &hqc_prng_free;
		} else {
			// set randombytes function
			OQS_randombytes_custom_algorithm(&OQS_randombytes_nist_kat);
			prng->seed = &OQS_randombytes_nist_kat_init_256bit;
			prng->get_state = &OQS_randombytes_nist_kat_get_state;
			prng->set_state = &OQS_randombytes_nist_kat_set_state;
			prng->free = NULL;
		}
	}
	return prng;
}

// entropy_input must have length 48.
// If personalization_string is non-null, its length must also be 48.
void OQS_KAT_PRNG_seed(OQS_KAT_PRNG *prng, const uint8_t *entropy_input, const uint8_t *personalization_string) {
	if (prng != NULL) {
		prng->seed(entropy_input, personalization_string);
	}
}

void OQS_KAT_PRNG_save_state(OQS_KAT_PRNG *prng) {
	if (prng != NULL) {
		prng->get_state(&prng->saved_state);
	}
}

void OQS_KAT_PRNG_restore_state(OQS_KAT_PRNG *prng) {
	if (prng != NULL) {
		prng->set_state(&prng->saved_state);
	}
}

void OQS_KAT_PRNG_free(OQS_KAT_PRNG *prng) {
	if (prng != NULL) {
		// saved_state needs to be handled dynamically
		if (prng->free != NULL) {
			prng->free(&prng->saved_state);
		}
	}
	OQS_MEM_insecure_free(prng);
}

/* Displays hexadecimal strings */
void OQS_print_hex_string(const char *label, const uint8_t *str, size_t len) {
	printf("%-20s (%4zu bytes):  ", label, len);
	for (size_t i = 0; i < (len); i++) {
		printf("%02X", str[i]);
	}
	printf("\n");
}

void OQS_fprintBstr(FILE *fp, const char *S, const uint8_t *A, size_t L) {
	size_t i;
	fprintf(fp, "%s", S);
	for (i = 0; i < L; i++) {
		fprintf(fp, "%02X", A[i]);
	}
	if (L == 0) {
		fprintf(fp, "00");
	}
	fprintf(fp, "\n");
}
