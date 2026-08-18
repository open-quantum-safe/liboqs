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
#include "blc_memopt_common.h"
#include "piop.h"
#include "piop_memopt.h"
#include "benchmark.h"
#include "sign.h"

#include <stdio.h>
#if !defined(MQOM2_FOR_MUPQ) && !defined(MQOM2_FOR_LIBOQS)
#ifdef SUPERCOP
extern void randombytes(unsigned char* x, unsigned long long xlen);
#else
extern int randombytes(unsigned char* x, unsigned long long xlen);
#endif
#else
#include "randombytes.h"
#endif

#if defined(VERIFY_MEMOPT) && !defined(MEMORY_EFFICIENT_BLC)
#error "Verify memopt relies on BLC memopt, which is not defined."
#endif

#if defined(VERIFY_MEMOPT) && !defined(MEMORY_EFFICIENT_PIOP)
#error "Verify memopt relies on PIOP memopt, which is not defined."
#endif

int Verify_memopt(const uint8_t pk[MQOM2_PK_SIZE], const uint8_t *msg, unsigned long long mlen, const uint8_t sig[MQOM2_SIG_SIZE]) {
	int ret = -1;
	int e;
	uint8_t mseed_eq[2 * MQOM2_PARAM_SEED_SIZE];
	uint8_t msg_hash[MQOM2_PARAM_DIGEST_SIZE], hash[MQOM2_PARAM_DIGEST_SIZE], com2_[MQOM2_PARAM_DIGEST_SIZE];
	field_ext_elt y[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU)];
	xof_context DECL_VAR(xof_ctx);

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
	ret = xof_init(&xof_ctx);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, (const uint8_t*) "\x02", 1);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, msg, mlen);
	ERR(ret, err);
	ret = xof_squeeze(&xof_ctx, msg_hash, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* Compute Fiat-Shamir hash */
	ret = xof_init(&xof_ctx);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, (const uint8_t*) "\x04", 1);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, pk, MQOM2_PK_SIZE);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, com1, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, com2, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, msg_hash, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_squeeze(&xof_ctx, hash, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* Sample Challenge */
	uint16_t i_star[MQOM2_PARAM_TAU];
	uint8_t tmp[MQOM2_PARAM_TAU * 2 + 2];
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
	uint16_t val = (tmp[2 * MQOM2_PARAM_TAU] + tmp[2 * MQOM2_PARAM_TAU + 1] * 256) & ((1 << MQOM2_PARAM_W) -1);
	if (val != 0) {
		ret = -1;
		goto err;
	}

	ret = xof_init(&xof_ctx);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, (const uint8_t*) "\x07", 1);
	ERR(ret, err);
	const uint8_t *path = &opening[0];
	const uint8_t *out_ls_com = &opening[MQOM2_PARAM_TAU * MQOM2_PARAM_SEED_SIZE * MQOM2_PARAM_NB_EVALS_LOG];
	const uint8_t *partial_delta_x = &opening[MQOM2_PARAM_TAU * (MQOM2_PARAM_SEED_SIZE * MQOM2_PARAM_NB_EVALS_LOG + MQOM2_PARAM_DIGEST_SIZE)];

	field_ext_elt alpha0[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)];
	field_ext_elt alpha1[PIOP_NB_PARALLEL_REPETITIONS_VERIFY][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)];
	field_ext_elt x_eval[PIOP_NB_PARALLEL_REPETITIONS_VERIFY][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)];
	field_ext_elt u_eval[PIOP_NB_PARALLEL_REPETITIONS_VERIFY][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)];
	uint8_t hash_ls_com[MQOM2_PARAM_DIGEST_SIZE];
	uint8_t *com1_ = hash_ls_com;
	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
		const uint8_t* path_e  = &path[e * (MQOM2_PARAM_NB_EVALS_LOG * MQOM2_PARAM_SEED_SIZE)];
		const uint8_t* out_ls_com_e = &out_ls_com[e * MQOM2_PARAM_DIGEST_SIZE];
		const uint8_t* partial_delta_x_e = &partial_delta_x[e * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE)];
		uint32_t e_mod = e % PIOP_NB_PARALLEL_REPETITIONS_VERIFY;

		/* Get Opened Evaluations */
		ret = BLC_Eval_x1_memopt(e, salt, path_e, out_ls_com_e, partial_delta_x_e, i_star[e], hash_ls_com, x_eval[e_mod], u_eval[e_mod]);
		ERR(ret, err);
		ret = xof_update(&xof_ctx, hash_ls_com, MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);

		/* Recompute P_alpha */
		if((e_mod == PIOP_NB_PARALLEL_REPETITIONS_VERIFY - 1) || (e == MQOM2_PARAM_TAU - 1)) {
			uint32_t e_;
			/* Parse alpha1 */
			for (e_ = 0; e_ <= e_mod; e_++) {
				field_ext_parse(&serialized_alpha1[(e - e_mod + e_) * BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_ETA * MQOM2_PARAM_MU)], MQOM2_PARAM_ETA, alpha1[e_]);
			}
			ret = RecomputePAlpha_partial_memopt(e_mod + 1, com1, (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)])&alpha1[0],
							     &i_star[e - e_mod], (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)])x_eval,
							     (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)])u_eval, mseed_eq, y, &alpha0[e - e_mod]);
			ERR(ret, err);
		}
	}
	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
		ret = xof_update(&xof_ctx, &partial_delta_x[e * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE)], BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE);
		ERR(ret, err);
	}
	ret = xof_squeeze(&xof_ctx, com1_, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* Hash P_alpha */
	ret = xof_init(&xof_ctx);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, (const uint8_t*) "\x03", 1);
	ERR(ret, err);
	uint8_t alpha[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_ETA * MQOM2_PARAM_MU)];
	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
		field_ext_serialize(alpha0[e], MQOM2_PARAM_ETA, alpha);
		ret = xof_update(&xof_ctx, alpha, BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_ETA * MQOM2_PARAM_MU));
		ERR(ret, err);
	}
	ret = xof_update(&xof_ctx, serialized_alpha1, MQOM2_PARAM_TAU * BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_ETA * MQOM2_PARAM_MU));
	ERR(ret, err);
	ret = xof_squeeze(&xof_ctx, com2_, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	if (memcmp(com1, com1_, MQOM2_PARAM_DIGEST_SIZE) != 0) {
		ret = -1;
		goto err;
	}
	if (memcmp(com2, com2_, MQOM2_PARAM_DIGEST_SIZE)) {
		ret = -1;
		goto err;
	}

	ret = 0;
err:
	xof_clean_ctx(&xof_ctx);
	return ret;
}
