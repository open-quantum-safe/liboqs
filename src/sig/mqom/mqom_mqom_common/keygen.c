#ifdef SUPERCOP
#include "crypto_sign.h"
#else
#include "api.h"
#endif
#include "keygen.h"

/* The buffers below only need aligned(64) on FIELDS_AVX512 or FIELDS_AVX2:
 * this is due to false postives on ASan's fake stack */
#if defined(FIELDS_AVX512) || defined(FIELDS_AVX2)
#define KEYGEN_BUF_ALIGN ALIGN(64)
#else
#define KEYGEN_BUF_ALIGN
#endif

#ifndef MEMORY_EFFICIENT_KEYGEN
/* Cycles optimized KeyGen (i.e. uses memory to expand MQ matrices): this is the default */
/* Some useful types definition */
/* NOTE: we use multi-dimensional array types to ease usage of indices.
 * While we can use pure local variables, these become too large to fit the stack
 * and heap allocation is needed. */
typedef field_ext_elt (*MatrixSetMQ)[MQOM3_PARAM_MQ_N][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
typedef field_ext_elt (*VectorSetMQ)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];

int KeyGen(const uint8_t seed_x[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)], const uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE], uint8_t sk[MQOM3_SK_SIZE], uint8_t pk[MQOM3_PK_SIZE]) {
	int ret = -1;
	KEYGEN_BUF_ALIGN field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	KEYGEN_BUF_ALIGN field_ext_elt y[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];
	uint32_t i;
	field_ext_elt *_A_hat = NULL;
	field_ext_elt *_b_hat = NULL;
	_A_hat = (field_ext_elt*)mqom_malloc(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU * MQOM3_PARAM_MQ_N * FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N) * sizeof(field_ext_elt));
	if (_A_hat == NULL) {
		ret = -1;
		goto err;
	}
	_b_hat = (field_ext_elt*)mqom_malloc(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU * FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N) * sizeof(field_ext_elt));
	if (_b_hat == NULL) {
		ret = -1;
		goto err;
	}
	MatrixSetMQ A_hat = (MatrixSetMQ)_A_hat;
	VectorSetMQ b_hat = (VectorSetMQ)_b_hat;

	/* Sanity check for field operations */
	if ((FIELD_BASE_LOG2_CARD == 1) && ((8 * FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)) != MQOM3_PARAM_MQ_N)) {
		ret = -1;
		goto err;
	}

	/* Parse x directly from seed_x (XOF_0 removed in v3) */
	field_base_parse(seed_x, MQOM3_PARAM_MQ_N, x);

	/* Expand the equations */
	ret = ExpandEquations(mseed_eq, A_hat, b_hat);
	ERR(ret, err);

	/* Perform the MQ equations computation in y. */
	KEYGEN_BUF_ALIGN field_ext_elt vect_tmp[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	for (i = 0; i < MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU; i++) {
		field_ext_elt y_i;
		field_ext_base_mat_mult((field_ext_elt*)A_hat[i], x, vect_tmp, MQOM3_PARAM_MQ_N, TRI_INF);
		y_i = field_base_ext_vect_mult(x, vect_tmp, MQOM3_PARAM_MQ_N) ^ field_base_ext_vect_mult(x, b_hat[i], MQOM3_PARAM_MQ_N);
		field_ext_vect_pack(y_i, y, i);
	}
	/* Serialize the public key */
	memcpy(&pk[0], mseed_eq, 2 * MQOM3_PARAM_SEED_SIZE);
	field_ext_serialize(y, MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU, &pk[2 * MQOM3_PARAM_SEED_SIZE]);
	/* Serialize the secret key */
	memcpy(&sk[0], mseed_eq, 2 * MQOM3_PARAM_SEED_SIZE);
	memcpy(&sk[2 * MQOM3_PARAM_SEED_SIZE], &pk[2 * MQOM3_PARAM_SEED_SIZE], BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU));
	field_base_serialize(x, MQOM3_PARAM_MQ_N, &sk[(2 * MQOM3_PARAM_SEED_SIZE) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)]);

	ret = 0;
err:
	if (_A_hat) {
		mqom_free(_A_hat, MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU * MQOM3_PARAM_MQ_N * FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N) * sizeof(field_ext_elt));
	}
	if (_b_hat) {
		mqom_free(_b_hat, MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU * FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N) * sizeof(field_ext_elt));
	}
	mqom_cleanse((void*)x, sizeof(x));
	mqom_cleanse((void*)y, sizeof(y));
	mqom_cleanse((void*)vect_tmp, sizeof(vect_tmp));

	return ret;
}

#else /* MEMORY_EFFICIENT_KEYGEN */
/* Memory optimized KeyGen: MQ matrices expansion is streamed */
int KeyGen(const uint8_t seed_x[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)], const uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE], uint8_t sk[MQOM3_SK_SIZE], uint8_t pk[MQOM3_PK_SIZE]) {
	int ret = -1;
	KEYGEN_BUF_ALIGN field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	KEYGEN_BUF_ALIGN field_ext_elt y[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];
	uint32_t i, j;
	/* Only use rows for A_hat and b_hat to save memory */
	KEYGEN_BUF_ALIGN field_ext_elt A_hat_row[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	/* NOTE: we reuse the A_hat_row memory slot to save memory */
	field_ext_elt *b_hat_row = A_hat_row;
	/* Streaming expand equation context */
	ExpandEquations_ctx EEctx = { 0 };

	/* Sanity check for field operations */
	if ((FIELD_BASE_LOG2_CARD == 1) && ((8 * FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)) != MQOM3_PARAM_MQ_N)) {
		ret = -1;
		goto err;
	}

	/* Parse x directly from seed_x (XOF_0 removed in v3) */
	field_base_parse(seed_x, MQOM3_PARAM_MQ_N, x);

	/* Compute the equations expansion in a streaming way to save memory */
	ret = ExpandEquations_memopt_init(mseed_eq, &EEctx);
	ERR(ret, err);
	KEYGEN_BUF_ALIGN field_ext_elt tmp[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	for (i = 0; i < MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU; i++) {
		field_ext_elt y_i;
		/* Perform operations row by row for A_hat */
		for (j = 0; j < MQOM3_PARAM_MQ_N; j++) {
			ret = ExpandEquations_memopt_update(&EEctx, A_hat_row);
			ERR(ret, err);
			tmp[j] = field_base_ext_vect_mult(x, A_hat_row, j + 1);
		}
		y_i = field_base_ext_vect_mult(x, tmp, MQOM3_PARAM_MQ_N);
		/* Generate and add b_hat row */
		ret = ExpandEquations_memopt_update(&EEctx, b_hat_row);
		ERR(ret, err);
		y_i ^= field_base_ext_vect_mult(x, b_hat_row, MQOM3_PARAM_MQ_N);
		field_ext_vect_pack(y_i, y, i);
	}

	/* Serialize the public key */
	memcpy(&pk[0], mseed_eq, 2 * MQOM3_PARAM_SEED_SIZE);
	field_ext_serialize(y, MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU, &pk[2 * MQOM3_PARAM_SEED_SIZE]);
	/* Serialize the secret key */
	memcpy(&sk[0], mseed_eq, 2 * MQOM3_PARAM_SEED_SIZE);
	memcpy(&sk[2 * MQOM3_PARAM_SEED_SIZE], &pk[2 * MQOM3_PARAM_SEED_SIZE], BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU));
	field_base_serialize(x, MQOM3_PARAM_MQ_N, &sk[(2 * MQOM3_PARAM_SEED_SIZE) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)]);

	ret = 0;
err:
	ExpandEquations_memopt_final(&EEctx);
	mqom_cleanse((void*)x, sizeof(x));
	mqom_cleanse((void*)y, sizeof(y));
	mqom_cleanse((void*)tmp, sizeof(tmp));

	return ret;
}
#endif

#if !defined(MQOM3_FOR_MUPQ) && !defined(MQOM3_FOR_LIBOQS)
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

	/* Sample seed_x and mseed_eq independently */
	uint8_t seed_x[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)];
	uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE];

#if defined(SUPERCOP) || defined(MQOM3_FOR_LIBOQS)
	randombytes(seed_x, BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N));
	randombytes(mseed_eq, 2 * MQOM3_PARAM_SEED_SIZE);
#else
	ret = randombytes(seed_x, BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N));
	ERR(ret, err);
	ret = randombytes(mseed_eq, 2 * MQOM3_PARAM_SEED_SIZE);
	ERR(ret, err);
#endif

	/* Run deterministic key generation */
	ret = KeyGen(seed_x, mseed_eq, sk, pk);
	ERR(ret, err);

	ret = 0;
err:
	if (ret) {
		memset(pk, 0, MQOM3_PK_SIZE);
		memset(sk, 0, MQOM3_SK_SIZE);
	}
	mqom_cleanse(seed_x, sizeof(seed_x));
	mqom_cleanse(mseed_eq, sizeof(mseed_eq));
	return ret;
}
