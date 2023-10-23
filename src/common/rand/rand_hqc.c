#include "oqs/sha3.h"

#define PRNG_DOMAIN 1

// TODO document licence etc

OQS_SHA3_shake256_inc_ctx shake_prng_state = { NULL };

void OQS_randombytes_hqc_kat(uint8_t *buf, size_t n);

void OQS_randombytes_hqc_kat_init(const uint8_t *entropy_input, const uint8_t *personalization_string) {
	uint8_t domain = PRNG_DOMAIN;
	if (shake_prng_state.ctx != NULL) {
		OQS_SHA3_shake256_inc_ctx_reset(&shake_prng_state);
	} else {
		OQS_SHA3_shake256_inc_init(&shake_prng_state);
	}
	OQS_SHA3_shake256_inc_absorb(&shake_prng_state, entropy_input, 48);
	if (personalization_string) {
		OQS_SHA3_shake256_inc_absorb(&shake_prng_state, personalization_string, 48);
	}
	OQS_SHA3_shake256_inc_absorb(&shake_prng_state, &domain, 1);
	OQS_SHA3_shake256_inc_finalize(&shake_prng_state);
}

void OQS_randombytes_hqc_kat(uint8_t *buf, size_t n) {
	OQS_SHA3_shake256_inc_squeeze(buf, n, &shake_prng_state);
}

void OQS_randombytes_hqc_kat_free() {
	if (shake_prng_state.ctx != NULL) {
		OQS_SHA3_shake256_inc_ctx_release(&shake_prng_state);
		shake_prng_state.ctx = NULL;
	}
}
