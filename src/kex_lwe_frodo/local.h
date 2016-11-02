#ifndef _OQS_KEX_LWE_FRODO_LOCAL_H_
#define _OQS_KEX_LWE_FRODO_LOCAL_H_

#include <stdint.h>
#include <assert.h>
#include <oqs/aes.h>
#include <oqs/rand.h>

struct oqs_kex_lwe_frodo_params {
	uint8_t *seed;
	size_t seed_len;
	char *param_name;
	uint16_t log2_q;
	uint16_t q;
	uint16_t n;
	uint16_t extracted_bits;
	uint16_t nbar;
	uint16_t key_bits;
	uint16_t rec_hint_len;
	uint32_t pub_len;
	uint16_t stripe_step;
	int sampler_num;
	uint16_t *cdf_table;
	size_t cdf_table_len;
};

void oqs_kex_lwe_frodo_key_round(uint16_t *vec, const size_t length, const int b);
void oqs_kex_lwe_frodo_key_round_hints(uint16_t *vec, const size_t length, const int b, const unsigned char *hint);
void oqs_kex_lwe_frodo_pack(unsigned char *out, const size_t outlen, const uint16_t *in, const size_t inlen, const unsigned char lsb);
void oqs_kex_lwe_frodo_unpack(uint16_t *out, const size_t outlen, const unsigned char *in, const size_t inlen, const unsigned char lsb);

int oqs_kex_lwe_frodo_sample_n(uint16_t *s, const size_t n, struct oqs_kex_lwe_frodo_params *params, OQS_RAND *rand);

// [.]_2
#define OQS_KEX_LWE_FRODO_ROUND2(__NAME, __NBAR, __LOG2_Q, __EXTRACTED_BITS, __KEYBITS) \
static inline void oqs_kex_lwe_frodo_round2_ ## __NAME (unsigned char *out, uint16_t *in) { \
	oqs_kex_lwe_frodo_key_round(in, __NBAR * __NBAR, __LOG2_Q - __EXTRACTED_BITS); \
	int i; \
	for (i = 0; i < __NBAR * __NBAR; i++) { \
		in[i] >>= __LOG2_Q - __EXTRACTED_BITS; \
	} \
	oqs_kex_lwe_frodo_pack(out, __KEYBITS / 8, in, __NBAR * __NBAR, __EXTRACTED_BITS); \
}

#define OQS_KEX_LWE_FRODO_CROSSROUND2(__NAME, __NBAR, __LOG2_Q, __EXTRACTED_BITS, __REC_HINT_LEN) \
static inline void oqs_kex_lwe_frodo_crossround2_ ## __NAME (unsigned char *out, const uint16_t *in) { \
	int i; \
	memset((unsigned char *)out, 0, __REC_HINT_LEN); \
	\
	const uint16_t whole = 1 << (__LOG2_Q - __EXTRACTED_BITS); \
	const uint16_t half = whole >> 1; \
	const uint16_t mask = whole - 1; \
 	\
	for (i = 0; i < __NBAR * __NBAR; i++) { \
		uint16_t remainder = in[i] & mask; \
		out[i / 8] += (remainder >= half) << (i % 8); \
	} \
}

#define OQS_KEX_LWE_FRODO_RECONCILE(__NAME, __NBAR, __LOG2_Q, __EXTRACTED_BITS, __KEYBITS) \
static inline void oqs_kex_lwe_frodo_reconcile_ ## __NAME (unsigned char *out, uint16_t *w, const unsigned char *hint) { \
	oqs_kex_lwe_frodo_key_round_hints(w, __NBAR * __NBAR, __LOG2_Q - __EXTRACTED_BITS, hint); \
	int i; \
	for (i = 0; i < __NBAR * __NBAR; i++) { \
		w[i] >>= __LOG2_Q - __EXTRACTED_BITS;  /* drop bits that were zeroed out */ \
	} \
	oqs_kex_lwe_frodo_pack(out, __KEYBITS / 8, w, __NBAR * __NBAR, __EXTRACTED_BITS); \
}

// Generate-and-multiply: generate A row-wise, multiply by s on the right.
// A (N x N) 
// s,e (N x N_BAR) 
// out = A * s + e (N x N_BAR)
#define OQS_KEX_LWE_FRODO_MUL_ADD_AS_PLUS_E_ON_THE_FLY(__NAME, __N, __NBAR, __STRIPE_STEP, __Q) \
static inline int oqs_kex_lwe_frodo_mul_add_as_plus_e_on_the_fly_ ## __NAME (uint16_t *out, const uint16_t *s, const uint16_t *e, struct oqs_kex_lwe_frodo_params *params) { \
\
	int i, j, k; \
	int ret = 0; \
	uint16_t *a_row = NULL; \
	uint16_t *s_transpose = NULL; \
\
	for (i = 0; i < __N; i++) { \
		for (j = 0; j < __NBAR; j++) { \
			out[i * __NBAR + j] = e[i * __NBAR + j]; \
		} \
	} \
\
	size_t a_rowlen = __N * sizeof(int16_t); \
	a_row = (uint16_t *) malloc(a_rowlen); \
	if (a_row == NULL) { \
		goto err; \
	} \
\
	/* transpose s to store it in the column-major order*/ \
	s_transpose = (uint16_t *) malloc(__NBAR * __N * sizeof(int16_t)); \
	if (s_transpose == NULL) { \
		goto err; \
	} \
\
	for (j = 0; j < __N; j++) { \
		for (k = 0; k < __NBAR; k++) { \
			s_transpose[k * __N + j] = s[j * __NBAR + k]; \
		} \
	} \
\
	assert(params->seed_len == 16); \
	void *aes_key_schedule = NULL; \
	OQS_AES128_load_schedule(params->seed, &aes_key_schedule, 1); \
\
	for (i = 0; i < __N; i++) { \
		/* go through A's rows*/ \
		memset(a_row, 0, a_rowlen); \
		for (j = 0; j < __N; j += __STRIPE_STEP) { \
			/* Loading values in the little-endian order!*/ \
			a_row[j] = i; \
			a_row[j + 1] = j; \
		} \
\
		OQS_AES128_ECB_enc_sch((uint8_t *) a_row, a_rowlen, aes_key_schedule, (uint8_t *) a_row); \
\
		for (k = 0; k < __NBAR; k++) { \
			uint16_t sum = 0; \
			for (j = 0; j < __N; j++) { \
				/* matrix-vector multiplication happens here*/ \
				sum += a_row[j] * s_transpose[k * __N + j]; \
			} \
			out[i * __NBAR + k] += sum; \
			out[i * __NBAR + k] %= __Q; \
		} \
	} \
\
	OQS_AES128_free_schedule(aes_key_schedule); \
\
	ret = 1; \
	goto cleanup; \
\
err: \
	memset(out, 0, __NBAR * __N * sizeof(uint16_t)); \
\
cleanup: \
	if (a_row != NULL) { \
		memset(a_row, 0, a_rowlen); \
		free(a_row); \
	} \
\
	if (s_transpose != NULL) { \
		memset(s_transpose, 0, __NBAR * __N * sizeof(int16_t)); \
		free(s_transpose); \
	} \
\
	return ret; \
}


// Generate-and-multiply: generate A column-wise, multiply by s' on the left.
#define OQS_KEX_LWE_FRODO_MUL_ADD_SA_PLUS_E_ON_THE_FLY(__NAME, __N, __NBAR, __STRIPE_STEP, __Q) \
static inline int oqs_kex_lwe_frodo_mul_add_sa_plus_e_on_the_fly_ ## __NAME (uint16_t *out, const uint16_t *s, const uint16_t *e, struct oqs_kex_lwe_frodo_params *params) { \
	/* a (N x N) */ \
	/* s',e' (N_BAR x N) */ \
	/* out = s'a + e' (N_BAR x N) */ \
\
	int i, j, k, kk; \
	int ret = 0; \
	uint16_t *a_cols = NULL; \
	uint16_t *a_cols_t = NULL; \
\
	for (i = 0; i < __NBAR; i++) { \
		for (j = 0; j < __N; j++) { \
			out[i * __N + j] = e[i * __N + j]; \
		} \
	} \
\
	size_t a_colslen = __N * __STRIPE_STEP * sizeof(int16_t); \
	/* a_cols stores 8 columns of A at a time.*/ \
	a_cols = (uint16_t *) malloc(a_colslen); \
	a_cols_t = (uint16_t *) malloc(a_colslen);  /* a_cols transposed (stored in the column-major order).*/ \
	if ((a_cols == NULL) || (a_cols_t == NULL)) { \
		goto err; \
	} \
\
	assert(params->seed_len == 16); \
	void *aes_key_schedule = NULL; \
	OQS_AES128_load_schedule(params->seed, &aes_key_schedule, 1); \
\
	for (kk = 0; kk < __N; kk += __STRIPE_STEP) { \
		/* Go through A's columns, 8 (== __STRIPE_STEP) columns at a time. */ \
		memset(a_cols, 0, a_colslen); \
		for (i = 0; i < __N; i++) { \
			/* Loading values in the little-endian order! */ \
			a_cols[i * __STRIPE_STEP] = i; \
			a_cols[i * __STRIPE_STEP + 1] = kk; \
		} \
\
		OQS_AES128_ECB_enc_sch((uint8_t *) a_cols, a_colslen, aes_key_schedule, (uint8_t *) a_cols); \
\
		/* transpose a_cols to have access to it in the column-major order. */ \
		for (i = 0; i < __N; i++) \
			for (k = 0; k < __STRIPE_STEP; k++) { \
				a_cols_t[k * __N + i] = a_cols[i * __STRIPE_STEP + k]; \
			} \
\
		for (i = 0; i < __NBAR; i++) \
			for (k = 0; k < __STRIPE_STEP; k++) { \
				uint16_t sum = 0; \
				for (j = 0; j < __N; j++) { \
					sum += s[i * __N + j] * a_cols_t[k * __N + j]; \
				} \
				out[i * __N + kk + k] += sum; \
				out[i * __N + kk + k] %= __Q; \
			} \
	} \
\
	OQS_AES128_free_schedule(aes_key_schedule); \
\
	ret = 1; \
	goto cleanup; \
\
err: \
	memset(out, 0, __NBAR * __N * sizeof(uint16_t)); \
\
cleanup: \
	if (a_cols != NULL) { \
		memset(a_cols, 0, a_colslen); \
		free(a_cols); \
	} \
\
	if (a_cols_t != NULL) { \
		memset(a_cols_t, 0, a_colslen); \
		free(a_cols_t); \
	} \
\
	return ret; \
}


// multiply by s on the right
#define OQS_KEX_LWE_FRODO_MUL_BS(__NAME, __N, __NBAR, __Q) \
static inline void oqs_kex_lwe_frodo_mul_bs_ ## __NAME (uint16_t *out, const uint16_t *b, const uint16_t *s) { \
	/* b (N_BAR x N) */ \
	/* s (N x N_BAR) */ \
	/* out = bs */ \
	int i, j, k; \
	for (i = 0; i < __NBAR; i++) { \
		for (j = 0; j < __NBAR; j++) { \
			out[i * __NBAR + j] = 0; \
			for (k = 0; k < __N; k++) { \
				out[i * __NBAR + j] += b[i * __N + k] * s[k * __NBAR + j]; \
			} \
			out[i * __NBAR + j] %= __Q;  /* not really necessary since LWE_Q is a power of 2.*/ \
		} \
	} \
}

// multiply by s on the left
#define OQS_KEX_LWE_FRODO_MUL_ADD_SB_PLUS_E(__NAME, __N, __NBAR, __Q) \
void oqs_kex_lwe_frodo_mul_add_sb_plus_e_ ## __NAME (uint16_t *out, const uint16_t *b, const uint16_t *s, const uint16_t *e) { \
	/* b (N x N_BAR) */ \
	/* s (N_BAR x N) */ \
	/* e (N_BAR x N_BAR) */ \
	/* out = sb + e */ \
	int i, j, k; \
	for (k = 0; k < __NBAR; k++) { \
		for (i = 0; i < __NBAR; i++) { \
			out[k * __NBAR + i] = e[k * __NBAR + i]; \
			for (j = 0; j < __N; j++) { \
				out[k * __NBAR + i] += s[k * __N + j] * b[j * __NBAR + i]; \
			} \
			out[k * __NBAR + i] %= __Q;  /* not really necessary since LWE_Q is a power of 2. */ \
		} \
	} \
}

#define OQS_KEX_LWE_FRODO_ALICE_0(__NAME) \
static inline int OQS_KEX_lwe_frodo_alice_0_ ## __NAME (OQS_KEX *k, void **alice_priv, uint8_t **alice_msg, size_t *alice_msg_len) { \
\
	int ret; \
\
	struct oqs_kex_lwe_frodo_params *params = (struct oqs_kex_lwe_frodo_params *) k->params; \
	uint16_t *b = NULL, *e = NULL; \
\
	*alice_priv = NULL; \
	*alice_msg = NULL; \
\
	/* allocate private key, error, and outgoing message */ \
	*alice_priv = malloc(params->n * params->nbar * sizeof(uint16_t)); \
	if (*alice_priv == NULL) { \
		goto err; \
	} \
	b = (uint16_t *) malloc(params->n * params->nbar * sizeof(uint16_t)); \
	if (b == NULL) { \
		goto err; \
	} \
	e = (uint16_t *) malloc(params->n * params->nbar * sizeof(uint16_t)); \
	if (e == NULL) { \
		goto err; \
	} \
	*alice_msg = malloc(params->pub_len); \
	if (*alice_msg == NULL) { \
		goto err; \
	} \
\
	/* generate S and E */ \
	ret = oqs_kex_lwe_frodo_sample_n(*alice_priv, params->n * params->nbar, params, k->rand); \
	if (ret != 1) { \
		goto err; \
	} \
	ret = oqs_kex_lwe_frodo_sample_n(e, params->n * params->nbar, params, k->rand); \
	if (ret != 1) { \
		goto err; \
	} \
\
	/* compute B = AS + E */ \
	ret = oqs_kex_lwe_frodo_mul_add_as_plus_e_on_the_fly_ ## __NAME (b, *alice_priv, e, params); \
	if (ret != 1) { \
		goto err; \
	} \
	oqs_kex_lwe_frodo_pack(*alice_msg, params->pub_len, b, params->n * params->nbar, params->log2_q); \
\
	*alice_msg_len = params->pub_len; \
\
	ret = 1; \
	goto cleanup; \
\
err: \
	ret = 0; \
	free(*alice_msg); \
	*alice_msg = NULL; \
	free(*alice_priv); \
	*alice_priv = NULL; \
\
cleanup: \
	free(e); \
	free(b); \
	return ret; \
\
}

#define OQS_KEX_LWE_FRODO_BOB(__NAME) \
static inline int OQS_KEX_lwe_frodo_bob_ ## __NAME (OQS_KEX *k, const uint8_t *alice_msg, const size_t alice_msg_len, uint8_t **bob_msg, size_t *bob_msg_len, uint8_t **key, size_t *key_len) { \
\
	int ret; \
\
	struct oqs_kex_lwe_frodo_params *params = (struct oqs_kex_lwe_frodo_params *) k->params; \
\
	uint16_t *bob_priv = NULL; \
	uint8_t *bob_rec = NULL; \
	uint16_t *b = NULL, *bprime = NULL, *eprime = NULL, *eprimeprime = NULL; \
	uint16_t *v = NULL; \
	*bob_msg = NULL; \
	*key = NULL; \
\
	/* check length of other party's public key */ \
	if (alice_msg_len != params->pub_len) { \
		goto err; \
	} \
\
	/* allocate private key, errors, outgoing message, and key */ \
	bob_priv = malloc(params->n * params->nbar * sizeof(uint16_t)); \
	if (bob_priv == NULL) { \
		goto err; \
	} \
	bprime = (uint16_t *) malloc(params->n * params->nbar * sizeof(uint16_t)); \
	if (bprime == NULL) { \
		goto err; \
	} \
	eprime = (uint16_t *) malloc(params->n * params->nbar * sizeof(uint16_t)); \
	if (eprime == NULL) { \
		goto err; \
	} \
	eprimeprime = (uint16_t *) malloc(params->nbar * params->nbar * sizeof(uint16_t)); \
	if (eprimeprime == NULL) { \
		goto err; \
	} \
	b = (uint16_t *) malloc(params->n * params->nbar * sizeof(uint16_t)); \
	if (b == NULL) { \
		goto err; \
	} \
	v = (uint16_t *) malloc(params->nbar * params->nbar * sizeof(uint16_t)); \
	if (v == NULL) { \
		goto err; \
	} \
	*bob_msg = malloc(params->pub_len + params->rec_hint_len); \
	if (*bob_msg == NULL) { \
		goto err; \
	} \
	bob_rec = *bob_msg + params->pub_len; \
	*key = malloc(params->key_bits >> 3); \
	if (*key == NULL) { \
		goto err; \
	} \
\
	/* generate S' and E' */ \
	ret = oqs_kex_lwe_frodo_sample_n(bob_priv, params->n * params->nbar, params, k->rand); \
	if (ret != 1) { \
		goto err; \
	} \
	ret = oqs_kex_lwe_frodo_sample_n(eprime, params->n * params->nbar, params, k->rand); \
	if (ret != 1) { \
		goto err; \
	} \
\
	/* compute B' = S'A + E' */ \
	ret = oqs_kex_lwe_frodo_mul_add_sa_plus_e_on_the_fly_ ## __NAME(bprime, bob_priv, eprime, params); \
	if (ret != 1) { \
		goto err; \
	} \
	oqs_kex_lwe_frodo_pack(*bob_msg, params->pub_len, bprime, params->n * params->nbar, params->log2_q); \
 \
	/* generate E'' */ \
	ret = oqs_kex_lwe_frodo_sample_n(eprimeprime, params->nbar * params->nbar, params, k->rand); \
	if (ret != 1) { \
		goto err; \
	} \
\
	/* unpack B */ \
	oqs_kex_lwe_frodo_unpack(b, params->n * params->nbar, alice_msg, alice_msg_len, params->log2_q); \
\
	/* compute V = S'B + E'' */ \
	oqs_kex_lwe_frodo_mul_add_sb_plus_e_ ## __NAME(v, b, bob_priv, eprimeprime); \
\
	/* compute C = <V>_{2^B} */ \
	oqs_kex_lwe_frodo_crossround2_ ## __NAME(bob_rec, v); \
\
	/* compute K = round(V)_{2^B} */ \
	oqs_kex_lwe_frodo_round2_ ## __NAME(*key, v); \
\
	*bob_msg_len = params->pub_len + params->rec_hint_len; \
	*key_len = params->key_bits >> 3; \
\
	ret = 1; \
	goto cleanup; \
\
err: \
	ret = 0; \
	free(*bob_msg); \
	*bob_msg = NULL; \
	if (*key != NULL) { \
		memset(*key, 0, params->key_bits >> 3); \
	} \
	free(*key); \
	*key = NULL; \
\
cleanup: \
	free(bob_priv); \
	if (eprime != NULL) { \
		memset(eprime, 0, params->n * params->nbar * sizeof(uint16_t)); \
	} \
	free(bprime); \
	free(eprime); \
	if (eprimeprime != NULL) { \
		memset(eprimeprime, 0, params->nbar * params->nbar * sizeof(uint16_t)); \
	} \
	free(eprimeprime); \
	free(b); \
	if (v != NULL) { \
		memset(v, 0, params->nbar * params->nbar * sizeof(uint16_t)); \
	} \
	free(v); \
\
	return ret; \
\
}

#define OQS_KEX_LWE_FRODO_ALICE_1(__NAME) \
static inline int OQS_KEX_lwe_frodo_alice_1_ ## __NAME (OQS_KEX *k, const void *alice_priv, const uint8_t *bob_msg, const size_t bob_msg_len, uint8_t **key, size_t *key_len) { \
\
	int ret; \
\
	struct oqs_kex_lwe_frodo_params *params = (struct oqs_kex_lwe_frodo_params *) k->params; \
\
	uint16_t *bprime = NULL, *w = NULL; \
	*key = NULL; \
\
	/* check length of other party's public key */ \
	if (bob_msg_len != params->pub_len + params->rec_hint_len) { \
		goto err; \
	} \
\
	/* allocate working values and session key */ \
	bprime = malloc(params->n * params->nbar * sizeof(uint16_t)); \
	if (bprime == NULL) { \
		goto err; \
	} \
	w = malloc(params->nbar * params->nbar * sizeof(uint16_t)); \
	if (w == NULL) { \
		goto err; \
	} \
	*key = malloc(params->key_bits >> 3); \
	if (*key == NULL) { \
		goto err; \
	} \
\
	/* unpack B' */ \
	oqs_kex_lwe_frodo_unpack(bprime, params->n * params->nbar, bob_msg, params->pub_len, params->log2_q); \
\
	/* compute W = B'S */ \
	oqs_kex_lwe_frodo_mul_bs_ ## __NAME (w, bprime, (uint16_t *) alice_priv); \
\
	/* compute K = rec(B'S, C) */ \
	const uint8_t *bob_rec = bob_msg + params->pub_len; \
	oqs_kex_lwe_frodo_reconcile_ ## __NAME (*key, w, bob_rec); \
\
	*key_len = params->key_bits >> 3; \
\
	ret = 1; \
	goto cleanup; \
\
err: \
	ret = 0; \
	memset(key, 0, params->key_bits >> 3); \
	free(*key); \
	*key = NULL; \
\
cleanup: \
	free(w); \
	free(bprime); \
	return ret; \
\
}

/* Define a set of the LWE_FRODO algorithms with the parameters provided */
#define OQS_KEX_LWE_FRODO_SET(__NAME, __N, __NBAR, __LOG2_Q, __Q, __EXTRACTED_BITS, __KEY_BITS, __STRIPE_STEP, __REC_HINT_LEN) \
OQS_KEX_LWE_FRODO_ROUND2(__NAME, __NBAR, __LOG2_Q, __EXTRACTED_BITS, __KEY_BITS) \
OQS_KEX_LWE_FRODO_CROSSROUND2(__NAME, __NBAR, __LOG2_Q, __EXTRACTED_BITS, __REC_HINT_LEN) \
OQS_KEX_LWE_FRODO_RECONCILE(__NAME, __NBAR, __LOG2_Q, __EXTRACTED_BITS, __KEY_BITS) \
OQS_KEX_LWE_FRODO_MUL_ADD_AS_PLUS_E_ON_THE_FLY(__NAME, __N, __NBAR, __STRIPE_STEP, __Q) \
OQS_KEX_LWE_FRODO_MUL_ADD_SA_PLUS_E_ON_THE_FLY(__NAME, __N, __NBAR, __STRIPE_STEP, __Q) \
OQS_KEX_LWE_FRODO_MUL_BS(__NAME, __N, __NBAR, __Q) \
OQS_KEX_LWE_FRODO_MUL_ADD_SB_PLUS_E(__NAME, __N, __NBAR, __Q) \
OQS_KEX_LWE_FRODO_ALICE_0(__NAME) \
OQS_KEX_LWE_FRODO_BOB(__NAME) \
OQS_KEX_LWE_FRODO_ALICE_1(__NAME)

#endif /* _OQS_KEX_RLWE_BCNS15_LOCAL_H_ */
