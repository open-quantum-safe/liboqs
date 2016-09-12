#define UNUSED __attribute__ ((unused))

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#include <oqs/kex.h>
#include <oqs/rand.h>

#include "kex_lwe_frodo.h"
#include "local.h"

#define LWE_DIV_ROUNDUP(x, y) (((x) + (y)-1) / y)

OQS_KEX *OQS_KEX_lwe_frodo_new_recommended(OQS_RAND *rand, const uint8_t *seed, const size_t seed_len) {

	OQS_KEX *k;
	struct oqs_kex_lwe_frodo_params *params;

	if ((seed_len == 0) || (seed == NULL)) return NULL;

	k = malloc(sizeof(OQS_KEX));
	if (k == NULL) {
		goto err;
	}

	k->params = malloc(sizeof(struct oqs_kex_lwe_frodo_params));
	if (NULL == k->params) {
		goto err;
	}
	params = (struct oqs_kex_lwe_frodo_params *) k->params;

	k->method_name = strdup("LWE Frodo recommended");
	if (NULL == k->method_name) {
		goto err;
	}
	k->estimated_classical_security = 144;
	k->estimated_quantum_security = 130;
	k->rand = rand;
	k->ctx = NULL;
	k->alice_0 = &OQS_KEX_lwe_frodo_alice_0;
	k->bob = &OQS_KEX_lwe_frodo_bob;
	k->alice_1 = &OQS_KEX_lwe_frodo_alice_1;
	k->alice_priv_free = &OQS_KEX_lwe_frodo_alice_priv_free;
	k->free = &OQS_KEX_lwe_frodo_free;

	params->seed = malloc(seed_len);
	if (NULL == params->seed) {
		goto err;
	}
	memcpy(params->seed, seed, seed_len);
	params->seed_len = seed_len;
	params->param_name = strdup("recommended");
	if (NULL == params->param_name) {
		goto err;
	}
	params->log2_q = 15;
	params->q = 1 << params->log2_q;
	params->n = 752;
	params->extracted_bits = 4;
	params->nbar = 8;
	params->key_bits = 256;
	params->rec_hint_len = LWE_DIV_ROUNDUP(params->nbar * params->nbar, 8);
	params->pub_len = LWE_DIV_ROUNDUP(params->nbar * params->nbar * params->log2_q, 8);
	params->sampler_num = 12;
	params->cdf_table_len = 6;
	params->cdf_table = malloc(params->cdf_table_len * sizeof(uint16_t));
	if (NULL == params->cdf_table) {
		goto err;
	}
	uint16_t cdf_table_tmp[6] = {602, 1521, 1927, 2031, 2046, 2047};
	memcpy(params->cdf_table, cdf_table_tmp, 6);

	return k;

err:
	if (k) {
		if (k->params) {
			free(params->cdf_table);
			free(params->seed);
			free(params->param_name);
			free(k->params);
		}
		free(k->method_name);
	}
	return NULL;

}

int OQS_KEX_lwe_frodo_alice_0(OQS_KEX *k, void **alice_priv, uint8_t **alice_msg, size_t *alice_msg_len) {

	int ret;

	struct oqs_kex_lwe_frodo_params *params = (struct oqs_kex_lwe_frodo_params *) k->params;
	uint16_t *e = NULL;

	*alice_priv = NULL;
	*alice_msg = NULL;

	/* allocate public/private key pair */
	*alice_msg = malloc(params->pub_len);
	if (*alice_msg == NULL) {
		goto err;
	}
	*alice_priv = malloc(params->n * params->nbar * sizeof(uint16_t));
	if (*alice_priv == NULL) {
		goto err;
	}

	e = (uint16_t *) malloc(params->n * params->nbar * sizeof(uint16_t));
	if (e == NULL) {
		goto err;
	}

	oqs_kex_lwe_frodo_sample_n(*alice_priv, params->n * params->nbar, params, k->rand);
	oqs_kex_lwe_frodo_sample_n(e, params->n * params->nbar, params, k->rand);
	if (!oqs_kex_lwe_frodo_key_gen_client_gen_a(*alice_msg, params->seed, *alice_priv, e, params)) {
		goto err;
	}

	*alice_msg_len = params->pub_len;

	ret = 1;
	goto cleanup;

err:
	ret = 0;
	free(*alice_msg);
	*alice_msg = NULL;
	free(*alice_priv);
	*alice_priv = NULL;
	free(e);

cleanup:
	return ret;

}

// int OQS_KEX_lwe_frodo_bob(OQS_KEX *k, const uint8_t *alice_msg, const size_t alice_msg_len, uint8_t **bob_msg, size_t *bob_msg_len, uint8_t **key, size_t *key_len) {

// 	int ret;

// 	uint8_t *bob_priv = NULL;
// 	*bob_msg = NULL;
// 	*key = NULL;

// 	if (alice_msg_len != 1024 * sizeof(uint32_t)) {
// 		goto err;
// 	}

// 	bob_priv = malloc(1024 * sizeof(uint32_t));
// 	if (bob_priv == NULL) {
// 		goto err;
// 	}
// 	/* allocate message and session key */
// 	*bob_msg = malloc(1024 * sizeof(uint32_t) + 16 * sizeof(uint64_t));
// 	if (*bob_msg == NULL) {
// 		goto err;
// 	}
// 	*key = malloc(16 * sizeof(uint64_t));
// 	if (*key == NULL) {
// 		goto err;
// 	}

// 	/* generate public/private key pair */
// 	oqs_kex_lwe_frodo_generate_keypair(oqs_kex_lwe_frodo_a, (uint32_t *) bob_priv, (uint32_t *) *bob_msg, k->ctx, k->rand);

// 	/* generate Bob's response */
// 	uint8_t *bob_rec = *bob_msg + 1024 * sizeof(uint32_t);
// 	oqs_kex_lwe_frodo_compute_key_bob((uint32_t *) alice_msg, (uint32_t *) bob_priv, (uint64_t *) bob_rec, (uint64_t *) *key, k->ctx, k->rand);
// 	*bob_msg_len = 1024 * sizeof(uint32_t) + 16 * sizeof(uint64_t);
// 	*key_len = 16 * sizeof(uint64_t);

// 	ret = 1;
// 	goto cleanup;

// err:
// 	ret = 0;
// 	free(*bob_msg);
// 	free(*key);

// cleanup:
// 	free(bob_priv);

// 	return ret;

// }

// int OQS_KEX_lwe_frodo_alice_1(OQS_KEX *k, const void *alice_priv, const uint8_t *bob_msg, const size_t bob_msg_len, uint8_t **key, size_t *key_len) {

// 	int ret;

// 	*key = NULL;

// 	if (bob_msg_len != 1024 * sizeof(uint32_t) + 16 * sizeof(uint64_t)) {
// 		goto err;
// 	}

// 	/* allocate session key */
// 	*key = malloc(16 * sizeof(uint64_t));
// 	if (*key == NULL) {
// 		goto err;
// 	}

// 	/* generate Alice's session key */
// 	const uint8_t *bob_rec = bob_msg + 1024 * sizeof(uint32_t);
// 	oqs_kex_lwe_frodo_compute_key_alice((uint32_t *)bob_msg, (uint32_t *)alice_priv, (uint64_t *) bob_rec, (uint64_t *) *key, k->ctx);
// 	*key_len = 16 * sizeof(uint64_t);

// 	ret = 1;
// 	goto cleanup;

// err:
// 	ret = 0;
// 	free(*key);

// cleanup:

// 	return ret;

// }

void OQS_KEX_lwe_frodo_alice_priv_free(UNUSED OQS_KEX *k, void *alice_priv) {
	if (alice_priv) {
		free(alice_priv);
	}
}

void OQS_KEX_lwe_frodo_free(OQS_KEX *k) {
	if (!k) {
		return;
	}
	if (k->params) {
		struct oqs_kex_lwe_frodo_params *params = (struct oqs_kex_lwe_frodo_params *) k->params;
		free(params->cdf_table);
		params->cdf_table = NULL;
		free(params->seed);
		params->seed = NULL;
		free(params->param_name);
		params->param_name = NULL;
		free(k->params);
		k->params = NULL;
	}
	free(k->method_name);
	k->method_name = NULL;
	free(k);
}
