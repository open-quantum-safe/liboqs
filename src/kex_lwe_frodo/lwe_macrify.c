// [.]_2
void MACRIFY(oqs_kex_lwe_frodo_round2)(unsigned char *out, uint16_t *in) {
	oqs_kex_lwe_frodo_key_round(in, PARAMS_NBAR * PARAMS_NBAR, PARAMS_LOG2Q - PARAMS_EXTRACTED_BITS);
	int i;
	for (i = 0; i < PARAMS_NBAR * PARAMS_NBAR; i++) {
		in[i] >>= PARAMS_LOG2Q - PARAMS_EXTRACTED_BITS;  // drop bits that were zeroed out
	}

	// out should have enough space for the key
	oqs_kex_lwe_frodo_pack(out, PARAMS_KEY_BITS / 8, in, PARAMS_NBAR * PARAMS_NBAR, PARAMS_EXTRACTED_BITS);
}

void MACRIFY(oqs_kex_lwe_frodo_crossround2)(unsigned char *out, const uint16_t *in) {
	int i;
	// out should have enough space for N_BAR * N_BAR bits
	memset((unsigned char *)out, 0, PARAMS_REC_HINT_LENGTH);

	uint16_t whole = 1 << (PARAMS_LOG2Q - PARAMS_EXTRACTED_BITS);
	uint16_t half = whole >> 1;
	uint16_t mask = whole - 1;

	for (i = 0; i < PARAMS_NBAR * PARAMS_NBAR; i++) {
		uint16_t remainder = in[i] & mask;
		out[i / 8] += (remainder >= half) << (i % 8);
	}
}

void MACRIFY(oqs_kex_lwe_frodo_reconcile)(unsigned char *out, uint16_t *w, const unsigned char *hint) {
	oqs_kex_lwe_frodo_key_round_hints(w, PARAMS_NBAR * PARAMS_NBAR, PARAMS_LOG2Q - PARAMS_EXTRACTED_BITS, hint);
	int i;
	for (i = 0; i < PARAMS_NBAR * PARAMS_NBAR; i++) {
		w[i] >>= PARAMS_LOG2Q - PARAMS_EXTRACTED_BITS;  // drop bits that were zeroed out
	}
	oqs_kex_lwe_frodo_pack(out, PARAMS_KEY_BITS / 8, w, PARAMS_NBAR * PARAMS_NBAR, PARAMS_EXTRACTED_BITS);
}

// Generate-and-multiply: generate A row-wise, multiply by s on the right.
int MACRIFY(oqs_kex_lwe_frodo_mul_add_as_plus_e_on_the_fly)(uint16_t *out, const uint16_t *s, const uint16_t *e, struct oqs_kex_lwe_frodo_params *params) {
	// A (N x N)
	// s,e (N x N_BAR)
	// out = A * s + e (N x N_BAR)

	int i, j, k;
	int ret = 0;
	uint16_t *a_row = NULL;
	uint16_t *s_transpose = NULL;

	for (i = 0; i < PARAMS_N; i++) {
		for (j = 0; j < PARAMS_NBAR; j++) {
			out[i * PARAMS_NBAR + j] = e[i * PARAMS_NBAR + j];
		}
	}

	size_t a_rowlen = PARAMS_N * sizeof(int16_t);
	a_row = (uint16_t *) malloc(a_rowlen);
	if (a_row == NULL) {
		goto err;
	}

	// transpose s to store it in the column-major order
	s_transpose = (uint16_t *) malloc(PARAMS_NBAR * PARAMS_N * sizeof(int16_t));
	if (s_transpose == NULL) {
		goto err;
	}

	for (j = 0; j < PARAMS_N; j++) {
		for (k = 0; k < PARAMS_NBAR; k++) {
			s_transpose[k * PARAMS_N + j] = s[j * PARAMS_NBAR + k];
		}
	}

	assert(params->seed_len == 16);
	void *aes_key_schedule = NULL;
	OQS_AES128_load_schedule(params->seed, &aes_key_schedule, 1);

	for (i = 0; i < PARAMS_N; i++) {
		// go through A's rows
		memset(a_row, 0, a_rowlen);
		for (j = 0; j < PARAMS_N; j += PARAMS_STRIPE_STEP) {
			// Loading values in the little-endian order!
			a_row[j] = i;
			a_row[j + 1] = j;
		}

		OQS_AES128_ECB_enc_sch((uint8_t *) a_row, a_rowlen, aes_key_schedule, (uint8_t *) a_row);

		for (k = 0; k < PARAMS_NBAR; k++) {
			uint16_t sum = 0;
			for (j = 0; j < PARAMS_N; j++) {
				// matrix-vector multiplication happens here
				sum += a_row[j] * s_transpose[k * PARAMS_N + j];
			}
			out[i * PARAMS_NBAR + k] += sum;
			out[i * PARAMS_NBAR + k] %= PARAMS_Q;
		}
	}

	OQS_AES128_free_schedule(aes_key_schedule);

	ret = 1;
	goto cleanup;

err:
	memset(out, 0, PARAMS_NBAR * PARAMS_N * sizeof(uint16_t));

cleanup:
	if (a_row != NULL) {
		memset(a_row, 0, a_rowlen);
		free(a_row);
	}

	if (s_transpose != NULL) {
		memset(s_transpose, 0, PARAMS_NBAR * PARAMS_N * sizeof(int16_t));
		free(s_transpose);
	}

	return ret;
}

// Generate-and-multiply: generate A column-wise, multiply by s' on the left.
int MACRIFY(oqs_kex_lwe_frodo_mul_add_sa_plus_e_on_the_fly)(uint16_t *out, const uint16_t *s, const uint16_t *e, struct oqs_kex_lwe_frodo_params *params) {
	// a (N x N)
	// s',e' (N_BAR x N)
	// out = s'a + e' (N_BAR x N)

	int i, j, k, kk;
	int ret = 0;
	uint16_t *a_cols = NULL;
	uint16_t *a_cols_t = NULL;

	for (i = 0; i < PARAMS_NBAR; i++) {
		for (j = 0; j < PARAMS_N; j++) {
			out[i * PARAMS_N + j] = e[i * PARAMS_N + j];
		}
	}

	size_t a_colslen = PARAMS_N * PARAMS_STRIPE_STEP * sizeof(int16_t);
	// a_cols stores 8 columns of A at a time.
	a_cols = (uint16_t *) malloc(a_colslen);
	a_cols_t = (uint16_t *) malloc(a_colslen);  // a_cols transposed (stored in the column-major order).
	if ((a_cols == NULL) || (a_cols_t == NULL)) {
		goto err;
	}

	assert(params->seed_len == 16);
	void *aes_key_schedule = NULL;
	OQS_AES128_load_schedule(params->seed, &aes_key_schedule, 1);

	for (kk = 0; kk < PARAMS_N; kk += PARAMS_STRIPE_STEP) {
		// Go through A's columns, 8 (== PARAMS_STRIPE_STEP) columns at a time.
		memset(a_cols, 0, a_colslen);
		for (i = 0; i < PARAMS_N; i++) {
			// Loading values in the little-endian order!
			a_cols[i * PARAMS_STRIPE_STEP] = i;
			a_cols[i * PARAMS_STRIPE_STEP + 1] = kk;
		}

		OQS_AES128_ECB_enc_sch((uint8_t *) a_cols, a_colslen, aes_key_schedule, (uint8_t *) a_cols);

		// transpose a_cols to have access to it in the column-major order.
		for (i = 0; i < PARAMS_N; i++)
			for (k = 0; k < PARAMS_STRIPE_STEP; k++) {
				a_cols_t[k * PARAMS_N + i] = a_cols[i * PARAMS_STRIPE_STEP + k];
			}

		for (i = 0; i < PARAMS_NBAR; i++)
			for (k = 0; k < PARAMS_STRIPE_STEP; k++) {
				uint16_t sum = 0;
				for (j = 0; j < PARAMS_N; j++) {
					sum += s[i * PARAMS_N + j] * a_cols_t[k * PARAMS_N + j];
				}
				out[i * PARAMS_N + kk + k] += sum;
				out[i * PARAMS_N + kk + k] %= PARAMS_Q;
			}
	}

	OQS_AES128_free_schedule(aes_key_schedule);

	ret = 1;
	goto cleanup;

err:
	memset(out, 0, PARAMS_NBAR * PARAMS_N * sizeof(uint16_t));

cleanup:
	if (a_cols != NULL) {
		memset(a_cols, 0, a_colslen);
		free(a_cols);
	}

	if (a_cols_t != NULL) {
		memset(a_cols_t, 0, a_colslen);
		free(a_cols_t);
	}

	return ret;
}

// multiply by s on the right
void MACRIFY(oqs_kex_lwe_frodo_mul_bs)(uint16_t *out, const uint16_t *b, const uint16_t *s) {
	// b (N_BAR x N)
	// s (N x N_BAR)
	// out = bs
	int i, j, k;
	for (i = 0; i < PARAMS_NBAR; i++) {
		for (j = 0; j < PARAMS_NBAR; j++) {
			out[i * PARAMS_NBAR + j] = 0;
			for (k = 0; k < PARAMS_N; k++) {
				out[i * PARAMS_NBAR + j] += b[i * PARAMS_N + k] * s[k * PARAMS_NBAR + j];
			}
			out[i * PARAMS_NBAR + j] %= PARAMS_Q;  // not really necessary since LWE_Q is a power of 2.
		}
	}
}

// multiply by s on the left
void MACRIFY(oqs_kex_lwe_frodo_mul_add_sb_plus_e)(uint16_t *out, const uint16_t *b, const uint16_t *s, const uint16_t *e) {
	// b (N x N_BAR)
	// s (N_BAR x N)
	// e (N_BAR x N_BAR)
	// out = sb + e
	int i, j, k;
	for (k = 0; k < PARAMS_NBAR; k++) {
		for (i = 0; i < PARAMS_NBAR; i++) {
			out[k * PARAMS_NBAR + i] = e[k * PARAMS_NBAR + i];
			for (j = 0; j < PARAMS_N; j++) {
				out[k * PARAMS_NBAR + i] += s[k * PARAMS_N + j] * b[j * PARAMS_NBAR + i];
			}
			out[k * PARAMS_NBAR + i] %= PARAMS_Q;  // not really necessary since LWE_Q is a power of 2.
		}
	}
}

