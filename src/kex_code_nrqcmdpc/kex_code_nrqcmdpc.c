#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <oqs/kex.h>
#include <oqs/rand.h>

#include "kex_code_nrqcmdpc.h"
#include "nrqcmdpc.h"

#if defined(WINDOWS)
#define UNUSED
/* TODO: test if this works in Visual Studio */
#else
#define UNUSED __attribute__ ((unused))
#endif

OQS_KEX *OQS_KEX_code_nrqcmdpc_new(
    OQS_RAND *rand) {
	OQS_KEX *k;

	if ((k = malloc(sizeof(OQS_KEX))) == NULL) {
		return NULL;
	}

	k->ctx = NULL;
	k->method_name = strdup("CODE NRQCMDPC");

	/* TODO: security needs to be re-evaluated, see: https://eprint.iacr.org/2016/858 */
	k->estimated_classical_security = 80;
	k->estimated_quantum_security = 80;

	k->seed = NULL;
	k->seed_len = 0;
	k->named_parameters = NULL;
	k->rand = rand;
	k->params = NULL;

	k->alice_0 = &OQS_KEX_code_nrqcmdpc_alice_0;
	k->bob = &OQS_KEX_code_nrqcmdpc_bob;
	k->alice_1 = &OQS_KEX_code_nrqcmdpc_alice_1;
	k->alice_priv_free = &OQS_KEX_code_nrqcmdpc_alice_priv_free;
	k->free = &OQS_KEX_code_nrqcmdpc_free;

	return k;
}

int OQS_KEX_code_nrqcmdpc_alice_0(
    OQS_KEX *k,
    void **alice_priv,
    uint8_t **alice_msg,
    size_t *alice_msg_len) {
	*alice_priv = NULL;
	*alice_msg = NULL;

	if ((*alice_priv = malloc(sizeof(par_ch_t))) == NULL) {
		return 0;
	}

	if ((*alice_msg = malloc(PUBLIC_KEY_BYTES)) == NULL) {
		free(*alice_priv);
		return 0;
	}

	oqs_kex_code_nrqcmdpc_keypair(*alice_msg, *alice_priv, k->rand);
	*alice_msg_len = PUBLIC_KEY_BYTES;

	return 1;
}

int OQS_KEX_code_nrqcmdpc_bob(
    OQS_KEX *k,
    const uint8_t *alice_msg,
    const size_t alice_msg_len,
    uint8_t **bob_msg,
    size_t *bob_msg_len,
    uint8_t **key,
    size_t *key_len) {
	*bob_msg = NULL;
	*key = NULL;

	if (alice_msg_len != PUBLIC_KEY_BYTES) {
		return 0;
	}
	if ((*bob_msg = malloc(SYNDROME_BYTES)) == NULL) {
		return 0;
	}
	*bob_msg_len = SYNDROME_BYTES;

	if ((*key = malloc(KEY_BYTES)) == NULL) {
		free(bob_msg);
		return 0;
	}

	oqs_kex_code_nrqcmdpc_encrypt(*key, *bob_msg, alice_msg, k->rand);
	*key_len = KEY_BYTES;

	return 1;
}

int OQS_KEX_code_nrqcmdpc_alice_1(
    UNUSED OQS_KEX *k,
    const void *alice_priv,
    const uint8_t *bob_msg,
    const size_t bob_msg_len,
    uint8_t **key,
    size_t *key_len) {
	*key = NULL;


	if (bob_msg_len != SYNDROME_BYTES) {
		return 0;
	}
	if ((*key = malloc(ERROR_BYTES)) == NULL) {
		return 0;
	}

	if (!oqs_kex_code_nrqcmdpc_decrypt(*key, bob_msg, alice_priv)) {
		/* TODO: clear key memory? */
		free(*key);
		*key = NULL;
		return 0;
	}
	*key_len = KEY_BYTES;

	return 1;
}

void OQS_KEX_code_nrqcmdpc_alice_priv_free(
    UNUSED OQS_KEX *k,
    void *alice_priv) {
	if (alice_priv != NULL) {
		free(alice_priv);
	}
}

void OQS_KEX_code_nrqcmdpc_free(
    OQS_KEX *k) {
	if (k != NULL) {
		if (k->method_name != NULL) {
			free(k->method_name);
		}
		k->method_name = NULL;
		free(k);
	}
}
