#ifdef SUPERCOP
#include "crypto_sign.h"
#include "crypto_declassify.h"
#else
#include "api.h"
#endif

#include <stdlib.h>
#include "common.h"
#include "fields.h"
#include "xof.h"
#include "expand_mq.h"
#include "blc.h"
#include "piop.h"
#include "benchmark.h"
#include "sign.h"

#if defined(USE_XOF_X4)
int SampleChallenge(const uint8_t hash[MQOM2_PARAM_DIGEST_SIZE], uint16_t i_star[MQOM2_PARAM_TAU], uint8_t nonce[4]) {
	int ret = -1;
	int e;
	xof_context_x4 DECL_VAR(xof_ctx);
	uint32_t nonce_int[4] = { 0, 1, 2, 3 };
	uint8_t _nonce[4][4];
	uint16_t _i_star[4][MQOM2_PARAM_TAU];
	unsigned int i;
	const uint8_t *constant_5[4] = { (const uint8_t*) "\x05", (const uint8_t*) "\x05", (const uint8_t*) "\x05", (const uint8_t*) "\x05" };
	const uint8_t *hash_ptr[4] = { hash, hash, hash, hash };
	uint8_t *nonce_ptr[4] = { _nonce[0], _nonce[1], _nonce[2], _nonce[3] };

	uint16_t val;
	uint8_t tmp[4][MQOM2_PARAM_TAU * 2 + 2];
	uint8_t *tmp_ptr[4] = { tmp[0], tmp[1], tmp[2], tmp[3] };

	do {
		for (i = 0; i < 4; i++) {
			_nonce[i][0] = nonce_int[i] & 0xff;
			_nonce[i][1] = (nonce_int[i] >> 8) & 0xff;
			_nonce[i][2] = (nonce_int[i] >> 16) & 0xff;
			_nonce[i][3] = (nonce_int[i] >> 24) & 0xff;
		}
		ret = xof_init_x4(&xof_ctx);
		ERR(ret, err);
		ret = xof_update_x4(&xof_ctx, constant_5, 1);
		ERR(ret, err);
		ret = xof_update_x4(&xof_ctx, hash_ptr, MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
		ret = xof_update_x4(&xof_ctx, (const uint8_t**)nonce_ptr, 4);
		ERR(ret, err);
		ret = xof_squeeze_x4(&xof_ctx, tmp_ptr, MQOM2_PARAM_TAU * 2 + 2);
		ERR(ret, err);
		for (i = 0; i < 4; i++) {
			for (e = 0; e < MQOM2_PARAM_TAU; e++) {
				_i_star[i][e] = (tmp[i][2 * e] + 256 * tmp[i][2 * e + 1]) & ((1 << MQOM2_PARAM_NB_EVALS_LOG) -1);
			}
			val = (tmp[i][2 * MQOM2_PARAM_TAU] + tmp[i][2 * MQOM2_PARAM_TAU + 1] * 256) & ((1 << MQOM2_PARAM_W) -1);
#ifdef SUPERCOP
			/* Rejection loop declassification for SUPERCOP */
			crypto_declassify(&val, sizeof(val));
#endif
			if (val == 0) {
				goto out_loop;
			}
			nonce_int[i] += 4;
		}
	} while (1);

out_loop:
	memcpy(i_star, _i_star[i], MQOM2_PARAM_TAU * sizeof(uint16_t));
	memcpy(nonce, _nonce[i], 4);

	ret = 0;
err:
	xof_clean_ctx_x4(&xof_ctx);
	return ret;
}
#else
int SampleChallenge(const uint8_t hash[MQOM2_PARAM_DIGEST_SIZE], uint16_t i_star[MQOM2_PARAM_TAU], uint8_t nonce[4]) {
	int ret = -1;
	int e;
	xof_context DECL_VAR(xof_ctx);
	uint32_t nonce_int = 0;

	uint16_t val;
	uint8_t tmp[MQOM2_PARAM_TAU * 2 + 2];
	do {
		nonce[0] = nonce_int & 0xff;
		nonce[1] = (nonce_int >> 8) & 0xff;
		nonce[2] = (nonce_int >> 16) & 0xff;
		nonce[3] = (nonce_int >> 24) & 0xff;
		ret = xof_init(&xof_ctx);
		ERR(ret, err);
		ret = xof_update(&xof_ctx, (const uint8_t*) "\x05", 1);
		ERR(ret, err);
		ret = xof_update(&xof_ctx, hash, MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
		ret = xof_update(&xof_ctx, nonce, 4);
		ERR(ret, err);
		ret = xof_squeeze(&xof_ctx, tmp, MQOM2_PARAM_TAU * 2 + 2);
		ERR(ret, err);
		for (e = 0; e < MQOM2_PARAM_TAU; e++) {
			i_star[e] = (tmp[2 * e] + 256 * tmp[2 * e + 1]) & ((1 << MQOM2_PARAM_NB_EVALS_LOG) -1);
		}
		val = (tmp[2 * MQOM2_PARAM_TAU] + tmp[2 * MQOM2_PARAM_TAU + 1] * 256) & ((1 << MQOM2_PARAM_W) -1);
#ifdef SUPERCOP
		/* Rejection loop declassification for SUPERCOP */
		crypto_declassify(&val, sizeof(val));
#endif
		nonce_int++;
	} while (val != 0);

	ret = 0;
err:
	xof_clean_ctx(&xof_ctx);
	return ret;
}
#endif

/* XXX: NOTE: USE_SIGNATURE_BUFFER_AS_TEMP is not compatible with libOQS because
 * of (non-)cleansing breaking the release of XOF contexts. */
#if defined(USE_SIGNATURE_BUFFER_AS_TEMP) && defined(libOQS)
#error "Error: USE_SIGNATURE_BUFFER_AS_TEMP and libOQS are NOT compatible ..."
#endif

int Sign(const uint8_t sk[MQOM2_SK_SIZE], const uint8_t *msg, unsigned long long mlen, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t mseed[MQOM2_PARAM_SEED_SIZE], uint8_t sig[MQOM2_SIG_SIZE]) {
	int ret = -1;
	int e;
	uint8_t msg_hash[MQOM2_PARAM_DIGEST_SIZE];
	uint8_t *hash = msg_hash;
	blc_key_t key;
	field_base_elt x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
	/* XXX: alignment of u0 because of aliasing of i_star */
	ALIGN(4) field_ext_elt u0[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)];
	field_ext_elt u1[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)];
	/* Aliasing to save stack space */
	field_ext_elt (*alpha0)[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)] = u0;
	field_ext_elt (*alpha1)[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)] = u1;

	/* Prepare the signature */
	unsigned int pos = 0;
	memcpy(&sig[pos], salt, MQOM2_PARAM_SALT_SIZE);
	pos += MQOM2_PARAM_SALT_SIZE;
	uint8_t *com1 = &sig[pos];
	pos += MQOM2_PARAM_DIGEST_SIZE;
	uint8_t *com2 = &sig[pos];
	pos += MQOM2_PARAM_DIGEST_SIZE;
	uint8_t *serialized_alpha1 = &sig[pos];
	pos += MQOM2_PARAM_TAU * BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_ETA * MQOM2_PARAM_MU);
	uint8_t *opening = &sig[pos];
	uint8_t *nonce = &sig[MQOM2_SIG_SIZE - 4];

#if defined(USE_SIGNATURE_BUFFER_AS_TEMP)
	/* When possible, we alias x0 with the opening */
	field_ext_elt x0_[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)];
	field_ext_elt (*x0)[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)];
	if((MQOM2_PARAM_TAU * FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N) * sizeof(field_ext_elt)) >= sizeof(blc_key_t)){
		x0 = (field_ext_elt (*)[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)])opening;
	}
	else{
		x0 = x0_;
	}
#else
        field_ext_elt x0[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)];
#endif

#if defined(MQOM2_FOR_LIBOQS)
	/* XXX: NOTE: aliasing is not compatible with libOQS because of the way internal magic values
	 * are checked in non constant-time.
	 * */
        xof_context xof_ctx_;
        xof_context *xof_ctx = NULL;
	xof_ctx = &xof_ctx_;
#else
	/* When possible, we alias the xof context to save stack space */
	xof_context xof_ctx_;
	xof_context *xof_ctx = NULL;
	if((MQOM2_PARAM_TAU * FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N) * sizeof(field_ext_elt)) >= sizeof(xof_context)){
		xof_ctx = (xof_context*)x0;
	}
	else{
		xof_ctx = &xof_ctx_;
	}
#endif
	memset(xof_ctx, 0, sizeof(xof_context));

	/* Parse the secret key */
	const uint8_t *pk = &sk[0];
	field_base_parse(&sk[(2 * MQOM2_PARAM_SEED_SIZE) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU)], MQOM2_PARAM_MQ_N, x);

	/* Hash message */
	ret = xof_init(xof_ctx);
	ERR(ret, err);
	ret = xof_update(xof_ctx, (const uint8_t*) "\x02", 1);
	ERR(ret, err);
	ret = xof_update(xof_ctx, msg, mlen);
	ERR(ret, err);
	ret = xof_squeeze(xof_ctx, msg_hash, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* Commit Lines */
	__BENCHMARK_START__(BS_BLC_COMMIT);
	ret = BLC_Commit(mseed, salt, x, com1, &key, x0, u0, u1);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_COMMIT);

	/* Compute P_alpha */
	__BENCHMARK_START__(BS_PIOP_COMPUTE);
	ret = ComputePAlpha(com1, (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)])x0,
				  (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)])u0,
				  (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)])u1,
				  x, &sk[0], alpha0, alpha1);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_PIOP_COMPUTE);

	/* Hash P_alpha and compute Fiat-Shamir hash */
	ret = xof_init(xof_ctx);
	ERR(ret, err);
	ret = xof_update(xof_ctx, (const uint8_t*) "\x03", 1);
	ERR(ret, err);

#if defined(USE_SIGNATURE_BUFFER_AS_TEMP)
	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
		uint8_t *buffer = &serialized_alpha1[e * BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_ETA * MQOM2_PARAM_MU)];
		field_ext_serialize(alpha0[e], MQOM2_PARAM_ETA, buffer);
		ret = xof_update(xof_ctx, buffer, BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_ETA * MQOM2_PARAM_MU));
		ERR(ret, err);
	}
#else
	uint8_t alpha[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_ETA * MQOM2_PARAM_MU)];
	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
		field_ext_serialize(alpha0[e], MQOM2_PARAM_ETA, alpha);
		ret = xof_update(xof_ctx, alpha, BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_ETA * MQOM2_PARAM_MU));
		ERR(ret, err);
	}

#endif
	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
		uint8_t *buffer = &serialized_alpha1[e * BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_ETA * MQOM2_PARAM_MU)];
		field_ext_serialize(alpha1[e], MQOM2_PARAM_ETA, buffer);
		ret = xof_update(xof_ctx, buffer, BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_ETA * MQOM2_PARAM_MU));
		ERR(ret, err);
	}
	ret = xof_squeeze(xof_ctx, com2, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	ret = xof_init(xof_ctx);
	ERR(ret, err);
	ret = xof_update(xof_ctx, (const uint8_t*) "\x04", 1);
	ERR(ret, err);
	ret = xof_update(xof_ctx, pk, MQOM2_PK_SIZE);
	ERR(ret, err);
	ret = xof_update(xof_ctx, com1, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_update(xof_ctx, com2, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_update(xof_ctx, msg_hash, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_squeeze(xof_ctx, hash, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* Sample Challenge */
	/* XXX: alias i_star when possible to save stack space */
	uint16_t i_star_[MQOM2_PARAM_TAU];
	uint16_t *i_star;
	if(sizeof(u0) >= (2 * MQOM2_PARAM_TAU)){
		i_star = (uint16_t*)u0;
	}
	else{
		i_star = i_star_;
	}
	__BENCHMARK_START__(BS_SAMPLE_CHALLENGE);
	ret = SampleChallenge(hash, i_star, nonce);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_SAMPLE_CHALLENGE);

	/* Open Line Evaluation */
	__BENCHMARK_START__(BS_BLC_OPEN);
	ret = BLC_Open(&key, i_star, opening);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_OPEN);

	ret = 0;
err:
	mqom_cleanse((void*)&key, sizeof(key));
	mqom_cleanse((void*)x, sizeof(x));
	mqom_cleanse((void*)msg_hash, sizeof(msg_hash));
#if defined(USE_SIGNATURE_BUFFER_AS_TEMP)
	/* XXX: do not cleanse if temporary variable is aliased */
	if((MQOM2_PARAM_TAU * FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N) * sizeof(field_ext_elt)) < sizeof(blc_key_t)){
		mqom_cleanse((void*)x0, sizeof(x0));
	}
	/* XXX: do not cleanse if temporary variable is aliased */
	if((MQOM2_PARAM_TAU * FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N) * sizeof(field_ext_elt)) < sizeof(xof_context)){
		xof_clean_ctx(xof_ctx);
	}
#else
	xof_clean_ctx(xof_ctx);
	mqom_cleanse((void*)x0, sizeof(x0));
#endif
	mqom_cleanse((void*)u0, sizeof(u0));
	mqom_cleanse((void*)u1, sizeof(u1));
	/* In case of error, clean the signature buffer */
	if(ret){
		memset(sig, 0, MQOM2_SIG_SIZE);
	}

	return ret;
}

int Verify_default(const uint8_t pk[MQOM2_PK_SIZE], const uint8_t *msg, unsigned long long mlen, const uint8_t sig[MQOM2_SIG_SIZE]) {
	int ret = -1;
	int e;
	uint8_t mseed_eq[2 * MQOM2_PARAM_SEED_SIZE];
	uint8_t msg_hash[MQOM2_PARAM_DIGEST_SIZE], hash[MQOM2_PARAM_DIGEST_SIZE], com2_[MQOM2_PARAM_DIGEST_SIZE];
	field_ext_elt y[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU)];
	field_ext_elt x_eval[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)];

#if defined(MQOM2_FOR_LIBOQS)
	/* XXX: NOTE: aliasing is not compatible with libOQS because of the way internal magic values
	 * are checked in non constant-time.
	 * */
        xof_context xof_ctx_;
        xof_context *xof_ctx = NULL;
	xof_ctx = &xof_ctx_;
#else
	/* When possible, we alias the xof context to save stack space */
	xof_context xof_ctx_;
	xof_context *xof_ctx = NULL;
	if((MQOM2_PARAM_TAU * FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N) * sizeof(field_ext_elt)) >= sizeof(xof_context)){
		xof_ctx = (xof_context*)x_eval;
	}
	else{
		xof_ctx = &xof_ctx_;
	}
#endif
	memset(xof_ctx, 0, sizeof(xof_context));

	/* Parse the public key */
	memcpy(mseed_eq, &pk[0], 2 * MQOM2_PARAM_SEED_SIZE);
	field_ext_parse(&pk[2 * MQOM2_PARAM_SEED_SIZE], MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU, y);

	/* Parse the signature */
	unsigned int pos = 0;
	const uint8_t *salt = &sig[pos];
	pos += MQOM2_PARAM_SALT_SIZE;
	const uint8_t *com1 = &sig[pos];
	pos += MQOM2_PARAM_DIGEST_SIZE;
	const uint8_t *com2 = &sig[pos];
	pos += MQOM2_PARAM_DIGEST_SIZE;
	const uint8_t *serialized_alpha1 = &sig[pos];
	pos += MQOM2_PARAM_TAU * BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_ETA * MQOM2_PARAM_MU);
	const uint8_t *opening = &sig[pos];
	const uint8_t *nonce = &sig[MQOM2_SIG_SIZE - 4];

	/* Hash message */
	ret = xof_init(xof_ctx);
	ERR(ret, err);
	ret = xof_update(xof_ctx, (const uint8_t*) "\x02", 1);
	ERR(ret, err);
	ret = xof_update(xof_ctx, msg, mlen);
	ERR(ret, err);
	ret = xof_squeeze(xof_ctx, msg_hash, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* Compute Fiat-Shamir hash */
	ret = xof_init(xof_ctx);
	ERR(ret, err);
	ret = xof_update(xof_ctx, (const uint8_t*) "\x04", 1);
	ERR(ret, err);
	ret = xof_update(xof_ctx, pk, MQOM2_PK_SIZE);
	ERR(ret, err);
	ret = xof_update(xof_ctx, com1, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_update(xof_ctx, com2, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_update(xof_ctx, msg_hash, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_squeeze(xof_ctx, hash, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* Sample Challenge */
	uint16_t i_star[MQOM2_PARAM_TAU];
	uint8_t tmp[MQOM2_PARAM_TAU * 2 + 2];
	ret = xof_init(xof_ctx);
	ERR(ret, err);
	ret = xof_update(xof_ctx, (const uint8_t*) "\x05", 1);
	ERR(ret, err);
	ret = xof_update(xof_ctx, hash, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_update(xof_ctx, nonce, 4);
	ERR(ret, err);
	ret = xof_squeeze(xof_ctx, tmp, MQOM2_PARAM_TAU * 2 + 2);
	ERR(ret, err);
	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
		i_star[e] = (tmp[2 * e] + 256 * tmp[2 * e + 1]) & ((1 << MQOM2_PARAM_NB_EVALS_LOG) -1);
	}
	uint16_t val = (tmp[2 * MQOM2_PARAM_TAU] + tmp[2 * MQOM2_PARAM_TAU + 1] * 256) & ((1 << MQOM2_PARAM_W) -1);
	if (val != 0) {
		ret = -1;
		goto err;
	}

	/* Get Opened Evaluations */
	field_ext_elt u_eval[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)];
	ret = BLC_Eval(salt, com1, opening, i_star, x_eval, u_eval);
	ERR(ret, err);

	/* Recompute P_alpha */
	field_ext_elt alpha0[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)];
	field_ext_elt alpha1[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)];
	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
		field_ext_parse(&serialized_alpha1[e * BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_ETA * MQOM2_PARAM_MU)], MQOM2_PARAM_ETA, alpha1[e]);
	}
	ret = RecomputePAlpha(com1, (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)])alpha1, i_star,
				    (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)])x_eval,
				    (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)])u_eval, mseed_eq, y, alpha0);
	ERR(ret, err);

	/* Hash P_alpha */
	ret = xof_init(xof_ctx);
	ERR(ret, err);
	ret = xof_update(xof_ctx, (const uint8_t*) "\x03", 1);
	ERR(ret, err);
	uint8_t alpha[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_ETA * MQOM2_PARAM_MU)];
	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
		field_ext_serialize(alpha0[e], MQOM2_PARAM_ETA, alpha);
		ret = xof_update(xof_ctx, alpha, BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_ETA * MQOM2_PARAM_MU));
		ERR(ret, err);
	}
	ret = xof_update(xof_ctx, serialized_alpha1, MQOM2_PARAM_TAU * BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_ETA * MQOM2_PARAM_MU));
	ERR(ret, err);
	ret = xof_squeeze(xof_ctx, com2_, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	if (memcmp(com2, com2_, MQOM2_PARAM_DIGEST_SIZE)) {
		ret = -1;
		goto err;
	}

	ret = 0;
err:
	xof_clean_ctx(xof_ctx);
	return ret;
}
