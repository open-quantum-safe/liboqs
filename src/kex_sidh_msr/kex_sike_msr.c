#if defined(WINDOWS)
#pragma warning(disable : 4047 4090) /* FIXMEOQS: needed still? */
#endif

#if defined(WINDOWS)
#define UNUSED
#else
#define UNUSED __attribute__((unused))
#endif

#include <stdlib.h>
#include <string.h>
#if !defined(WINDOWS)
#include <strings.h>
#include <unistd.h>
#endif

#include <oqs/kex.h>
#include <oqs/rand.h>

#include "kex_sike_msr.h"

#if defined(WINDOWS)
#define strdup _strdup // for strdup deprecation warning
#endif

OQS_KEX *OQS_KEX_sike_msr_new(OQS_RAND *rand, const char *named_parameters) {
	return NULL;
}

int OQS_KEX_sike_msr_alice_0(OQS_KEX *k, void **alice_priv, uint8_t **alice_msg, size_t *alice_msg_len) {
	return 0;
}

int OQS_KEX_sike_msr_bob(OQS_KEX *k, const uint8_t *alice_msg, const size_t alice_msg_len, uint8_t **bob_msg, size_t *bob_msg_len, uint8_t **key, size_t *key_len) {
	return 0;
}

int OQS_KEX_sike_msr_alice_1(OQS_KEX *k, const void *alice_priv, const uint8_t *bob_msg, const size_t bob_msg_len, uint8_t **key, size_t *key_len) {
	return 0;
}

void OQS_KEX_sike_msr_alice_priv_free(UNUSED OQS_KEX *k, void *alice_priv) {
}

void OQS_KEX_sike_msr_free(OQS_KEX *k) {
}
