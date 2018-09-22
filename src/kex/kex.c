#include <assert.h>

#include <oqs/kex.h>

#define UNUSED_KEX(expr) \
	do {                 \
		(void) (expr);   \
	} while (0)

OQS_API OQS_KEX *OQS_KEX_new(OQS_RAND *rand, enum OQS_KEX_alg_name alg_name, const uint8_t *seed, const size_t seed_len, const char *named_parameters) {

	//To disable warnings when the function arguments are not being used depending
	//on which algorithm has been disabled
	UNUSED_KEX(rand);
	UNUSED_KEX(seed);
	UNUSED_KEX(seed_len);
	UNUSED_KEX(named_parameters);

	switch (alg_name) {
	case OQS_KEX_alg_default:
		return NULL;

	default:
		assert(0);
		return NULL;
	}
}

OQS_API OQS_STATUS OQS_KEX_alice_0(OQS_KEX *k, void **alice_priv, uint8_t **alice_msg, size_t *alice_msg_len) {
	if (k == NULL) {
		return OQS_ERROR;
	} else {
		return k->alice_0(k, alice_priv, alice_msg, alice_msg_len);
	}
}

OQS_API OQS_STATUS OQS_KEX_bob(OQS_KEX *k, const uint8_t *alice_msg, const size_t alice_msg_len, uint8_t **bob_msg, size_t *bob_msg_len, uint8_t **key, size_t *key_len) {
	if (k == NULL) {
		return OQS_ERROR;
	} else {
		return k->bob(k, alice_msg, alice_msg_len, bob_msg, bob_msg_len, key, key_len);
	}
}

OQS_API OQS_STATUS OQS_KEX_alice_1(OQS_KEX *k, const void *alice_priv, const uint8_t *bob_msg, const size_t bob_msg_len, uint8_t **key, size_t *key_len) {
	if (k == NULL) {
		return OQS_ERROR;
	} else {
		return k->alice_1(k, alice_priv, bob_msg, bob_msg_len, key, key_len);
	}
}

OQS_API void OQS_KEX_alice_priv_free(OQS_KEX *k, void *alice_priv) {
	if (k) {
		k->alice_priv_free(k, alice_priv);
	}
}

OQS_API void OQS_KEX_free(OQS_KEX *k) {
	if (k) {
		k->free(k); // IGNORE free-check
	}
}
