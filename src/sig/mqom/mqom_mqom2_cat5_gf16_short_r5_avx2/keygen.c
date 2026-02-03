#ifdef SUPERCOP
#include "crypto_sign.h"
#else
#include "api.h"
#endif
#include "keygen.h"

#ifndef MEMORY_EFFICIENT_KEYGEN
/* Cycles optimized KeyGen (i.e. uses memory to expand MQ matrices): this is the default */
/* Some useful types definition */
/* NOTE: we use multi-dimensional array types to ease usage of indices.
 * While we can use pure local variables, these become too large to fit the stack
 * and heap allocation is needed. */
typedef field_ext_elt (*MatrixSetMQ)[MQOM2_PARAM_MQ_N][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)];
typedef field_ext_elt (*VectorSetMQ)[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)];

int KeyGen(const uint8_t seed_key[2 * MQOM2_PARAM_SEED_SIZE], uint8_t sk[MQOM2_SK_SIZE], uint8_t pk[MQOM2_PK_SIZE]) {
	int ret = -1;
	field_base_elt x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
	field_ext_elt y[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU)];
	/* Byte array representing x */
	uint8_t _x[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)];
	uint8_t mseed_eq[2 * MQOM2_PARAM_SEED_SIZE];
	uint32_t i;
	xof_context DECL_VAR(xof_ctx);
	field_ext_elt *_A_hat = NULL;
	field_ext_elt *_b_hat = NULL;
	_A_hat = (field_ext_elt*)mqom_malloc(MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU * MQOM2_PARAM_MQ_N * FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N) * sizeof(field_ext_elt));
	if (_A_hat == NULL) {
		ret = -1;
		goto err;
	}
	_b_hat = (field_ext_elt*)mqom_malloc(MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU * FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N) * sizeof(field_ext_elt));
	if (_b_hat == NULL) {
		ret = -1;
		goto err;
	}
	MatrixSetMQ A_hat = (MatrixSetMQ)_A_hat;
	VectorSetMQ b_hat = (VectorSetMQ)_b_hat;

	/* Sanity check for field operations */
	if ((FIELD_BASE_LOG2_CARD == 1) && ((8 * FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)) != MQOM2_PARAM_MQ_N)) {
		ret = -1;
		goto err;
	}

	/* Generate x and mseed_eq */
	ret = xof_init(&xof_ctx);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, (const uint8_t*) "\x00", 1);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, seed_key, 2 * MQOM2_PARAM_SEED_SIZE);
	ERR(ret, err);
	/* XXX: NOTE: since we are sure that we are byte aligned and there are no endianness issues
	 * for base fields, we could directly XOF squeeze in x, but for readability we prefer to
	 * use field_base_parse (i.e. the cost of a memcpy) */
	ret = xof_squeeze(&xof_ctx, _x, sizeof(_x));
	ERR(ret, err);
	field_base_parse(_x, MQOM2_PARAM_MQ_N, x);
	ret = xof_squeeze(&xof_ctx, mseed_eq, sizeof(mseed_eq));
	ERR(ret, err);

	/* Expand the equations */
	ret = ExpandEquations(mseed_eq, A_hat, b_hat);
	ERR(ret, err);

	/* Perform the MQ equations computation in y */
	for (i = 0; i < MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU; i++) {
		field_ext_elt y_i;
		field_ext_elt vect_tmp[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)];
		field_ext_base_mat_mult((field_ext_elt*)A_hat[i], x, vect_tmp, MQOM2_PARAM_MQ_N, TRI_INF);
		y_i = field_base_ext_vect_mult(x, vect_tmp, MQOM2_PARAM_MQ_N) ^ field_base_ext_vect_mult(x, b_hat[i], MQOM2_PARAM_MQ_N);
		field_ext_vect_pack(y_i, y, i);
	}
	/* Serialize the public key */
	memcpy(&pk[0], mseed_eq, 2 * MQOM2_PARAM_SEED_SIZE);
	field_ext_serialize(y, MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU, &pk[2 * MQOM2_PARAM_SEED_SIZE]);
	/* Serialize the secret key */
	memcpy(&sk[0], mseed_eq, 2 * MQOM2_PARAM_SEED_SIZE);
	memcpy(&sk[2 * MQOM2_PARAM_SEED_SIZE], &pk[2 * MQOM2_PARAM_SEED_SIZE], BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU));
	field_base_serialize(x, MQOM2_PARAM_MQ_N, &sk[(2 * MQOM2_PARAM_SEED_SIZE) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU)]);

	ret = 0;
err:
	if (_A_hat) {
		mqom_free(_A_hat, MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU * MQOM2_PARAM_MQ_N * FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N) * sizeof(field_ext_elt));
	}
	if (_b_hat) {
		mqom_free(_b_hat, MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU * FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N) * sizeof(field_ext_elt));
	}
	mqom_cleanse((void*)x, sizeof(x));
	mqom_cleanse((void*)y, sizeof(y));
	mqom_cleanse((void*)_x, sizeof(_x));
	mqom_cleanse((void*)mseed_eq, sizeof(mseed_eq));
	xof_clean_ctx(&xof_ctx);

	return ret;
}

#else /* MEMORY_EFFICIENT_KEYGEN */
/* Memory optimized KeyGen: MQ matrices expansion is streamed */
int KeyGen(const uint8_t seed_key[2 * MQOM2_PARAM_SEED_SIZE], uint8_t sk[MQOM2_SK_SIZE], uint8_t pk[MQOM2_PK_SIZE]) {
	int ret = -1;
	field_base_elt x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
	field_ext_elt y[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU)];
	/* Byte array representing x */
	uint8_t _x[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)];
	uint8_t mseed_eq[2 * MQOM2_PARAM_SEED_SIZE];
	uint32_t i, j;
	xof_context DECL_VAR(xof_ctx);
	/* Only use rows for A_hat and b_hat to save memory */
	field_ext_elt A_hat_row[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)];
	/* NOTE: we reuse the A_hat_row memory slot to save memory */
	field_ext_elt *b_hat_row = A_hat_row;
	/* Streaming expand equation context */
	ExpandEquations_ctx EEctx = { 0 };

	/* Sanity check for field operations */
	if ((FIELD_BASE_LOG2_CARD == 1) && ((8 * FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)) != MQOM2_PARAM_MQ_N)) {
		ret = -1;
		goto err;
	}

	/* Generate x and mseed_eq */
	ret = xof_init(&xof_ctx);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, (const uint8_t*) "\x00", 1);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, seed_key, 2 * MQOM2_PARAM_SEED_SIZE);
	ERR(ret, err);
	/* XXX: NOTE: since we are sure that we are byte aligned and there are no endianness issues
	 * for base fields, we could directly XOF squeeze in x, but for readability we prefer to
	 * use field_base_parse (i.e. the cost of a memcpy) */
	ret = xof_squeeze(&xof_ctx, _x, sizeof(_x));
	ERR(ret, err);
	field_base_parse(_x, MQOM2_PARAM_MQ_N, x);
	ret = xof_squeeze(&xof_ctx, mseed_eq, sizeof(mseed_eq));
	ERR(ret, err);

	/* Compute the equations expansion in a streaming way to save memory */
	ret = ExpandEquations_memopt_init(mseed_eq, &EEctx);
	ERR(ret, err);
	for (i = 0; i < MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU; i++) {
		field_ext_elt y_i;
		field_ext_elt tmp[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)];
		/* Perform operations row by row for A_hat */
		for (j = 0; j < MQOM2_PARAM_MQ_N; j++) {
			ret = ExpandEquations_memopt_update(&EEctx, A_hat_row);
			ERR(ret, err);
			tmp[j] = field_base_ext_vect_mult(x, A_hat_row, j + 1);
		}
		y_i = field_base_ext_vect_mult(x, tmp, MQOM2_PARAM_MQ_N);
		/* Generate and add b_hat row */
		ret = ExpandEquations_memopt_update(&EEctx, b_hat_row);
		ERR(ret, err);
		y_i ^= field_base_ext_vect_mult(x, b_hat_row, MQOM2_PARAM_MQ_N);
		field_ext_vect_pack(y_i, y, i);
	}

	/* Serialize the public key */
	memcpy(&pk[0], mseed_eq, 2 * MQOM2_PARAM_SEED_SIZE);
	field_ext_serialize(y, MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU, &pk[2 * MQOM2_PARAM_SEED_SIZE]);
	/* Serialize the secret key */
	memcpy(&sk[0], mseed_eq, 2 * MQOM2_PARAM_SEED_SIZE);
	memcpy(&sk[2 * MQOM2_PARAM_SEED_SIZE], &pk[2 * MQOM2_PARAM_SEED_SIZE], BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU));
	field_base_serialize(x, MQOM2_PARAM_MQ_N, &sk[(2 * MQOM2_PARAM_SEED_SIZE) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU)]);

	ret = 0;
err:
	ExpandEquations_memopt_final(&EEctx);
	mqom_cleanse((void*)x, sizeof(x));
	mqom_cleanse((void*)y, sizeof(y));
	mqom_cleanse((void*)_x, sizeof(_x));
	mqom_cleanse((void*)mseed_eq, sizeof(mseed_eq));
	xof_clean_ctx(&xof_ctx);

	return ret;
}
#endif

#if !defined(MQOM2_FOR_MUPQ) && !defined(MQOM2_FOR_LIBOQS)
#ifdef SUPERCOP
extern void randombytes(unsigned char* x, unsigned long long xlen);
#else
extern int randombytes(unsigned char* x, unsigned long long xlen);
#endif
#else
#include "randombytes.h"
#endif
int crypto_sign_keypair(unsigned char *pk, unsigned char *sk) {
	int ret = -1;

	/* Sample the seed key */
	uint8_t seed_key[2 * MQOM2_PARAM_SEED_SIZE];

#if defined(SUPERCOP) || defined(MQOM2_FOR_LIBOQS)
	randombytes(seed_key, 2 * MQOM2_PARAM_SEED_SIZE);
#else
	ret = randombytes(seed_key, 2 * MQOM2_PARAM_SEED_SIZE);
	ERR(ret, err);
#endif

	/* Run deterministic key generation */
	ret = KeyGen(seed_key, sk, pk);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}
