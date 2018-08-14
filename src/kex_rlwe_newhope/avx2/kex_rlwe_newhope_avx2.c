#if defined(WINDOWS)
#define UNUSED
// __attribute__ not supported in VS, is there something else I should define?
#else
#define UNUSED __attribute__((unused))
#endif

#include <stdlib.h>
#include <string.h>
#if !defined(WINDOWS)
#include <strings.h>
#include <unistd.h>
#endif

#include <oqs/common.h>
#include <oqs/kex.h>
#include <oqs/rand.h>

#include "kex_rlwe_newhope_avx2.h"
#include "newhope.c"
#include "params.h"

OQS_KEX *OQS_KEX_rlwe_newhope_avx2_new(OQS_RAND *rand) {
	OQS_KEX *k = malloc(sizeof(OQS_KEX));
	if (k == NULL) {
		return NULL;
	}
	k->method_name = strdup("RLWE NewHope AVX2");
	k->estimated_classical_security = 229; // http://eprint.iacr.org/2015/1092.pdf Table 1 NewHope dual known classical
	k->estimated_quantum_security = 206;   // http://eprint.iacr.org/2015/1092.pdf Table 1 NewHope dual known quantum
	k->seed = NULL;
	k->seed_len = 0;
	k->named_parameters = 0;
	k->rand = rand;
	k->params = NULL;
	k->alice_0 = &OQS_KEX_rlwe_newhope_avx2_alice_0;
	k->bob = &OQS_KEX_rlwe_newhope_avx2_bob;
	k->alice_1 = &OQS_KEX_rlwe_newhope_avx2_alice_1;
	k->alice_priv_free = &OQS_KEX_rlwe_newhope_avx2_alice_priv_free;
	k->free = &OQS_KEX_rlwe_newhope_avx2_free; // IGNORE free-check
	return k;
}

OQS_STATUS OQS_KEX_rlwe_newhope_avx2_alice_0(UNUSED OQS_KEX *k, void **alice_priv, uint8_t **alice_msg, size_t *alice_msg_len) {

	OQS_STATUS ret;

	/* allocate public/private key pair */
	*alice_msg = malloc(NEWHOPE_SENDABYTES);
	if (*alice_msg == NULL) {
		goto err;
	}
	*alice_priv = malloc(sizeof(poly));
	if (*alice_priv == NULL) {
		goto err;
	}

	/* generate public/private key pair */
	newhope_keygen(*alice_msg, (poly *) (*alice_priv));
	*alice_msg_len = NEWHOPE_SENDABYTES;

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;
	OQS_MEM_insecure_free(*alice_msg);
	*alice_msg = NULL;
	OQS_MEM_secure_free(*alice_priv, sizeof(poly));
	*alice_priv = NULL;

cleanup:

	return ret;
}

OQS_STATUS OQS_KEX_rlwe_newhope_avx2_bob(UNUSED OQS_KEX *k, const uint8_t *alice_msg, const size_t alice_msg_len, uint8_t **bob_msg, size_t *bob_msg_len, uint8_t **key, size_t *key_len) {

	OQS_STATUS ret;

	if (alice_msg_len != NEWHOPE_SENDABYTES) {
		goto err;
	}

	/* allocate message and session key */
	*bob_msg = malloc(NEWHOPE_SENDBBYTES);
	if (*bob_msg == NULL) {
		goto err;
	}
	*key = malloc(32);
	if (*key == NULL) {
		goto err;
	}

	/* generate Bob's response */
	newhope_sharedb(*key, *bob_msg, alice_msg);
	*bob_msg_len = NEWHOPE_SENDBBYTES;
	*key_len = 32;

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;
	OQS_MEM_insecure_free(*bob_msg);
	*bob_msg = NULL;
	OQS_MEM_secure_free(*key, *key_len);
	*key = NULL;

cleanup:

	return ret;
}

OQS_STATUS OQS_KEX_rlwe_newhope_avx2_alice_1(UNUSED OQS_KEX *k, const void *alice_priv, const uint8_t *bob_msg, const size_t bob_msg_len, uint8_t **key, size_t *key_len) {

	OQS_STATUS ret;

	if (bob_msg_len != NEWHOPE_SENDBBYTES) {
		goto err;
	}

	/* allocate session key */
	*key = malloc(32);
	if (*key == NULL) {
		goto err;
	}

	/* generate Alice's session key */
	newhope_shareda(*key, (poly *) alice_priv, bob_msg);
	*key_len = 32;

	ret = OQS_SUCCESS;
	goto cleanup;

err:
	ret = OQS_ERROR;
	OQS_MEM_secure_free(*key, *key_len);
	*key = NULL;

cleanup:

	return ret;
}

void OQS_KEX_rlwe_newhope_avx2_alice_priv_free(UNUSED OQS_KEX *k, void *alice_priv) {
	if (alice_priv) {
		OQS_MEM_secure_free(alice_priv, sizeof(poly));
	}
}

void OQS_KEX_rlwe_newhope_avx2_free(OQS_KEX *k) {
	if (k) {
		OQS_MEM_insecure_free(k->named_parameters);
		k->named_parameters = NULL;
		OQS_MEM_insecure_free(k->method_name);
		k->method_name = NULL;
	}
	OQS_MEM_insecure_free(k);
}
