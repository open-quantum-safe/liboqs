// SPDX-License-Identifier: MIT
#ifndef OQS_TEST_HELPERS_H
#define OQS_TEST_HELPERS_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <oqs/sha3.h>
#include <oqs/rand_nist.h>

#ifdef OQS_ENABLE_TEST_CONSTANT_TIME
#include <valgrind/memcheck.h>
#define OQS_TEST_CT_CLASSIFY(addr, len)  VALGRIND_MAKE_MEM_UNDEFINED(addr, len)
#define OQS_TEST_CT_DECLASSIFY(addr, len)  VALGRIND_MAKE_MEM_DEFINED(addr, len)
#else
#define OQS_TEST_CT_CLASSIFY(addr, len)
#define OQS_TEST_CT_DECLASSIFY(addr, len)
#endif

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

OQS_STATUS flip_bit(uint8_t *array, uint64_t array_length, uint64_t bit_position);

OQS_STATUS test_sig_bitflip(OQS_SIG *sig, uint8_t *message, size_t message_len, uint8_t *signature, size_t signature_len, uint8_t *public_key, bool bitflips_all[2], size_t bitflips[2], bool use_ctx, uint8_t *ctx, size_t ctx_i);

OQS_STATUS test_sig_stfl_bitflip(OQS_SIG_STFL *sig, uint8_t *message, size_t message_len, uint8_t *signature, size_t signature_len, uint8_t *public_key, bool bitflips_all[2], size_t bitflips[2]);

#endif
