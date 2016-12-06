int MACRIFY(OQS_KEX_lwe_frodo_alice_0)(OQS_KEX *k, void **alice_priv, uint8_t **alice_msg, size_t *alice_msg_len) {

	int ret;

	struct oqs_kex_lwe_frodo_params *params = (struct oqs_kex_lwe_frodo_params *) k->params;

	*alice_priv = NULL;
	*alice_msg = NULL;

	/* allocate private key, error, and outgoing message */
	*alice_priv = malloc(PARAMS_N * PARAMS_NBAR * sizeof(uint16_t));
	if (*alice_priv == NULL) {
		goto err;
	}
	uint16_t b[PARAMS_N * PARAMS_NBAR];
	uint16_t e[PARAMS_N * PARAMS_NBAR];
	*alice_msg = malloc(params->pub_len);
	if (*alice_msg == NULL) {
		goto err;
	}

	/* generate S and E */
	oqs_kex_lwe_frodo_sample_n(*alice_priv, PARAMS_N * PARAMS_NBAR, params, k->rand);
	oqs_kex_lwe_frodo_sample_n(e, PARAMS_N * PARAMS_NBAR, params, k->rand);

	/* compute B = AS + E */
	MACRIFY(oqs_kex_lwe_frodo_mul_add_as_plus_e_on_the_fly)(b, *alice_priv, e, params);

	oqs_kex_lwe_frodo_pack(*alice_msg, params->pub_len, b, PARAMS_N * PARAMS_NBAR, params->log2_q);

	*alice_msg_len = params->pub_len;

	ret = 1;
	goto cleanup;

err:
	OQS_MEM_cleanse(e, sizeof(e));
	free(*alice_msg);
	*alice_msg = NULL;
	free(*alice_priv);
	*alice_priv = NULL;
	ret = 0;

cleanup:
	return ret;
}

int MACRIFY(OQS_KEX_lwe_frodo_bob)(OQS_KEX *k, const uint8_t *alice_msg, const size_t alice_msg_len, uint8_t **bob_msg, size_t *bob_msg_len, uint8_t **key, size_t *key_len) {

	int ret;

	struct oqs_kex_lwe_frodo_params *params = (struct oqs_kex_lwe_frodo_params *) k->params;

	uint8_t *bob_rec = NULL;
	*bob_msg = NULL;
	*key = NULL;

	/* check length of other party's public key */
	if (alice_msg_len != params->pub_len) {
		goto err;
	}

	/* allocate private key, errors, outgoing message, and key */
	uint16_t bob_priv[PARAMS_N * PARAMS_NBAR];
	uint16_t bprime[PARAMS_N * PARAMS_NBAR];
	uint16_t eprime[PARAMS_N * PARAMS_NBAR];
	uint16_t eprimeprime[PARAMS_N * PARAMS_NBAR];
	uint16_t b[PARAMS_N * PARAMS_NBAR];
	uint16_t v[PARAMS_N * PARAMS_NBAR];
	*bob_msg = malloc(params->pub_len + params->rec_hint_len);
	if (*bob_msg == NULL) {
		goto err;
	}
	bob_rec = *bob_msg + params->pub_len;
	*key = malloc(PARAMS_KEY_BYTES);
	if (*key == NULL) {
		goto err;
	}

	/* generate S' and E' */
	oqs_kex_lwe_frodo_sample_n(bob_priv, params->n * params->nbar, params, k->rand);
	oqs_kex_lwe_frodo_sample_n(eprime, params->n * params->nbar, params, k->rand);

	/* compute B' = S'A + E' */
	MACRIFY(oqs_kex_lwe_frodo_mul_add_sa_plus_e_on_the_fly)(bprime, bob_priv, eprime, params);

	oqs_kex_lwe_frodo_pack(*bob_msg, params->pub_len, bprime, params->n * params->nbar, params->log2_q);

	/* generate E'' */
	oqs_kex_lwe_frodo_sample_n(eprimeprime, params->nbar * params->nbar, params, k->rand);

	/* unpack B */
	oqs_kex_lwe_frodo_unpack(b, params->n * params->nbar, alice_msg, alice_msg_len, params->log2_q);

	/* compute V = S'B + E'' */
	MACRIFY(oqs_kex_lwe_frodo_mul_add_sb_plus_e)(v, b, bob_priv, eprimeprime);

	/* compute C = <V>_{2^B} */
	MACRIFY(oqs_kex_lwe_frodo_crossround2)(bob_rec, v);

	/* compute K = round(V)_{2^B} */
	MACRIFY(oqs_kex_lwe_frodo_round2)(*key, v);

	*bob_msg_len = params->pub_len + params->rec_hint_len;
	*key_len = PARAMS_KEY_BYTES;

	ret = 1;
	goto cleanup;

err:
	ret = 0;
	free(*bob_msg);
	*bob_msg = NULL;
	OQS_MEM_secure_free(*key, PARAMS_KEY_BYTES);
	*key = NULL;

cleanup:
	OQS_MEM_cleanse(eprime, sizeof(eprime));
	OQS_MEM_cleanse(eprimeprime, sizeof(eprimeprime));
	OQS_MEM_cleanse(v, sizeof(v));

	return ret;

}

int MACRIFY(OQS_KEX_lwe_frodo_alice_1)(OQS_KEX *k, const void *alice_priv, const uint8_t *bob_msg, const size_t bob_msg_len, uint8_t **key, size_t *key_len) {

	int ret;

	struct oqs_kex_lwe_frodo_params *params = (struct oqs_kex_lwe_frodo_params *) k->params;

	*key = NULL;

	/* check length of other party's public key */
	if (bob_msg_len != params->pub_len + params->rec_hint_len) {
		goto err;
	}

	/* allocate working values and session key */
	uint16_t bprime[PARAMS_N * PARAMS_NBAR];
	uint16_t w[PARAMS_N * PARAMS_NBAR];

	*key = malloc(PARAMS_KEY_BYTES);
	if (*key == NULL) {
		goto err;
	}

	/* unpack B' */
	oqs_kex_lwe_frodo_unpack(bprime, PARAMS_N * PARAMS_NBAR, bob_msg, params->pub_len, params->log2_q);

	/* compute W = B'S */
	MACRIFY(oqs_kex_lwe_frodo_mul_bs)(w, bprime, (uint16_t *) alice_priv);

	/* compute K = rec(B'S, C) */
	const uint8_t *bob_rec = bob_msg + params->pub_len;
	MACRIFY(oqs_kex_lwe_frodo_reconcile)(*key, w, bob_rec);

	*key_len = PARAMS_KEY_BYTES;

	ret = 1;
	goto cleanup;

err:
	ret = 0;
	OQS_MEM_secure_free(*key, PARAMS_KEY_BYTES);
	*key = NULL;

cleanup:
	return ret;

}
