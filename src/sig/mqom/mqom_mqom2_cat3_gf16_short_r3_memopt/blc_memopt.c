#include "blc_memopt.h"
#include "ggm_tree.h"
#include "benchmark.h"
#include "seed_commit.h"

#if defined(SUPERCOP)
#include "crypto_declassify.h"
#endif

#include "blc_memopt_common.h"

int BLC_Commit_memopt(const uint8_t mseed[MQOM2_PARAM_SEED_SIZE], const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)], uint8_t com1[MQOM2_PARAM_DIGEST_SIZE], blc_key_memopt_t* key, field_ext_elt x0[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], field_ext_elt u0[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)], field_ext_elt u1[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)]) {
	int ret = -1;
	uint32_t e;

	/* Compute delta */
	DeriveDelta(x, key->delta);

	/* Compute the rseed table */
	ret = DeriveRootSeeds(mseed, key->rseed);
	ERR(ret, err);

	memcpy(key->salt, salt, MQOM2_PARAM_SALT_SIZE);

	xof_context DECL_VAR(xof_ctx_hash_ls_com);
	ret = xof_init(&xof_ctx_hash_ls_com);
	ERR(ret, err);
	ret = xof_update(&xof_ctx_hash_ls_com, (const uint8_t*) "\x07", 1);
	ERR(ret, err);

	e = 0;
#if defined(BLC_INTERNAL_X4)
	uint8_t hash_ls_com_x4[4][MQOM2_PARAM_DIGEST_SIZE];
	for (; e + 3 < MQOM2_PARAM_TAU; e+=4) {
		uint32_t es[4] = {e + 0, e + 1, e + 2, e + 3};
		ret = BLC_Commit_x4_memopt(es, &key->rseed[e], salt, x, key->delta, hash_ls_com_x4, &key->partial_delta_x[e], &x0[e], &u0[e], &u1[e]);
		ERR(ret, err);
		__BENCHMARK_START__(BS_BLC_XOF);
		ret = xof_update(&xof_ctx_hash_ls_com, hash_ls_com_x4[0], MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
		ret = xof_update(&xof_ctx_hash_ls_com, hash_ls_com_x4[1], MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
		ret = xof_update(&xof_ctx_hash_ls_com, hash_ls_com_x4[2], MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
		ret = xof_update(&xof_ctx_hash_ls_com, hash_ls_com_x4[3], MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
		__BENCHMARK_STOP__(BS_BLC_XOF);
	}
#endif
#if defined(BLC_INTERNAL_X2) || defined(BLC_INTERNAL_X4)
	uint8_t hash_ls_com_x2[2][MQOM2_PARAM_DIGEST_SIZE];
	for (; e + 1 < MQOM2_PARAM_TAU; e+=2) {
		uint32_t es[2] = {e + 0, e + 1};
		ret = BLC_Commit_x2_memopt(es, &key->rseed[e], salt, x, key->delta, hash_ls_com_x2, &key->partial_delta_x[e], &x0[e], &u0[e], &u1[e]);
		ERR(ret, err);
		__BENCHMARK_START__(BS_BLC_XOF);
		ret = xof_update(&xof_ctx_hash_ls_com, hash_ls_com_x2[0], MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
		ret = xof_update(&xof_ctx_hash_ls_com, hash_ls_com_x2[1], MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
		__BENCHMARK_STOP__(BS_BLC_XOF);
	}
#endif
	uint8_t hash_ls_com[MQOM2_PARAM_DIGEST_SIZE];
	for (; e < MQOM2_PARAM_TAU; e++) {
		ret = BLC_Commit_x1_memopt(e, key->rseed[e], salt, x, key->delta, hash_ls_com, key->partial_delta_x[e], x0[e], u0[e], u1[e]);
		ERR(ret, err);
		__BENCHMARK_START__(BS_BLC_XOF);
		ret = xof_update(&xof_ctx_hash_ls_com, hash_ls_com, MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
		__BENCHMARK_STOP__(BS_BLC_XOF);
	}

	__BENCHMARK_START__(BS_BLC_XOF);
	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
		ret = xof_update(&xof_ctx_hash_ls_com, key->partial_delta_x[e], BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE);
		ERR(ret, err);
	}
	ret = xof_squeeze(&xof_ctx_hash_ls_com, com1, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_XOF);

	ret = 0;
err:
	return ret;
}

int BLC_Open_memopt(const blc_key_memopt_t* key, const uint16_t i_star[MQOM2_PARAM_TAU], uint8_t opening[MQOM2_PARAM_OPENING_SIZE]) {
	int ret = -1;
	int e;
	seedcommit_ctx_t DECL_VAR(seedcommit_ctx);
	uint8_t lseed[MQOM2_PARAM_SEED_SIZE];

	uint8_t *path = &opening[0];
	uint8_t *out_ls_com = &opening[MQOM2_PARAM_TAU * MQOM2_PARAM_SEED_SIZE * MQOM2_PARAM_NB_EVALS_LOG];
	uint8_t *partial_delta_x = &opening[MQOM2_PARAM_TAU * (MQOM2_PARAM_SEED_SIZE * MQOM2_PARAM_NB_EVALS_LOG + MQOM2_PARAM_DIGEST_SIZE)];

	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
#if defined(SUPERCOP)
		/* XXX: NOTE: we explicitly declassify i_star[e] as it is public data but comes from a dataflow involving secret data
		 * through hashing */
		crypto_declassify(&i_star[e], sizeof(i_star[e]));
#endif
		ret = GGMTree_ExpandPath(key->salt, key->rseed[e], key->delta, e, i_star[e], (uint8_t(*)[MQOM2_PARAM_SEED_SIZE]) &path[e * (MQOM2_PARAM_NB_EVALS_LOG * MQOM2_PARAM_SEED_SIZE)], lseed);
		ERR(ret, err);
		ret = init_seedcommit(&seedcommit_ctx, key->salt, e);
		ERR(ret, err);
		SeedCommit(&seedcommit_ctx, lseed, &out_ls_com[e * MQOM2_PARAM_DIGEST_SIZE]);

		memcpy(&partial_delta_x[e * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE)], key->partial_delta_x[e], BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE);
	}

	ret = 0;
err:
	seedcommit_clean_ctx(&seedcommit_ctx);
	return ret;
}

int BLC_Eval_memopt(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t com1[MQOM2_PARAM_DIGEST_SIZE], const uint8_t opening[MQOM2_PARAM_OPENING_SIZE], const uint16_t i_star[MQOM2_PARAM_TAU], field_ext_elt x_eval[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], field_ext_elt u_eval[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)]) {
	int ret = -1;
	uint32_t e;

	uint8_t com1_[MQOM2_PARAM_DIGEST_SIZE];

	xof_context DECL_VAR(xof_ctx_hash_ls_com);
	ret = xof_init(&xof_ctx_hash_ls_com);
	ERR(ret, err);
	ret = xof_update(&xof_ctx_hash_ls_com, (const uint8_t*) "\x07", 1);
	ERR(ret, err);

	const uint8_t *path = &opening[0];
	const uint8_t *out_ls_com = &opening[MQOM2_PARAM_TAU * MQOM2_PARAM_SEED_SIZE * MQOM2_PARAM_NB_EVALS_LOG];
	const uint8_t *partial_delta_x = &opening[MQOM2_PARAM_TAU * (MQOM2_PARAM_SEED_SIZE * MQOM2_PARAM_NB_EVALS_LOG + MQOM2_PARAM_DIGEST_SIZE)];

	e = 0;
#if defined(BLC_INTERNAL_X4)
	uint8_t hash_ls_com_x4[4][MQOM2_PARAM_DIGEST_SIZE];
	uint8_t path_e_x4[4][MQOM2_PARAM_SEED_SIZE*MQOM2_PARAM_NB_EVALS_LOG];
	uint8_t out_ls_com_e_x4[4][MQOM2_PARAM_DIGEST_SIZE];
	uint8_t partial_delta_x_e_x4[4][BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)-MQOM2_PARAM_SEED_SIZE];
	for (; e + 3 < MQOM2_PARAM_TAU; e+=4) {
		uint32_t es[4] = {e + 0, e + 1, e + 2, e + 3};

		for (uint32_t e_ = 0; e_ < 4; e_++) {
			memcpy(path_e_x4[e_], &path[es[e_] * (MQOM2_PARAM_NB_EVALS_LOG * MQOM2_PARAM_SEED_SIZE)], MQOM2_PARAM_NB_EVALS_LOG * MQOM2_PARAM_SEED_SIZE);
			memcpy(out_ls_com_e_x4[e_], &out_ls_com[es[e_] * MQOM2_PARAM_DIGEST_SIZE], MQOM2_PARAM_DIGEST_SIZE);
			memcpy(partial_delta_x_e_x4[e_], &partial_delta_x[es[e_] * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE)], BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)-MQOM2_PARAM_SEED_SIZE);
		}

		ret = BLC_Eval_x4_memopt(es, salt, path_e_x4, out_ls_com_e_x4, partial_delta_x_e_x4, &i_star[e], hash_ls_com_x4, &x_eval[e], &u_eval[e]);
		ERR(ret, err);

		ret = xof_update(&xof_ctx_hash_ls_com, hash_ls_com_x4[0], MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
		ret = xof_update(&xof_ctx_hash_ls_com, hash_ls_com_x4[1], MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
		ret = xof_update(&xof_ctx_hash_ls_com, hash_ls_com_x4[2], MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
		ret = xof_update(&xof_ctx_hash_ls_com, hash_ls_com_x4[3], MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
	}
#endif
#if defined(BLC_INTERNAL_X2) || defined(BLC_INTERNAL_X4)
	uint8_t hash_ls_com_x2[2][MQOM2_PARAM_DIGEST_SIZE];
	uint8_t path_e_x2[2][MQOM2_PARAM_SEED_SIZE*MQOM2_PARAM_NB_EVALS_LOG];
	uint8_t out_ls_com_e_x2[2][MQOM2_PARAM_DIGEST_SIZE];
	uint8_t partial_delta_x_e_x2[2][BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)-MQOM2_PARAM_SEED_SIZE];
	for (; e + 1 < MQOM2_PARAM_TAU; e+=2) {
		uint32_t es[2] = {e + 0, e + 1};

		for (uint32_t e_ = 0; e_ < 2; e_++) {
			memcpy(path_e_x2[e_], &path[es[e_] * (MQOM2_PARAM_NB_EVALS_LOG * MQOM2_PARAM_SEED_SIZE)], MQOM2_PARAM_NB_EVALS_LOG * MQOM2_PARAM_SEED_SIZE);
			memcpy(out_ls_com_e_x2[e_], &out_ls_com[es[e_] * MQOM2_PARAM_DIGEST_SIZE], MQOM2_PARAM_DIGEST_SIZE);
			memcpy(partial_delta_x_e_x2[e_], &partial_delta_x[es[e_] * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE)], BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)-MQOM2_PARAM_SEED_SIZE);
		}

		ret = BLC_Eval_x2_memopt(es, salt, path_e_x2, out_ls_com_e_x2, partial_delta_x_e_x2, &i_star[e], hash_ls_com_x2, &x_eval[e], &u_eval[e]);
		ERR(ret, err);

		ret = xof_update(&xof_ctx_hash_ls_com, hash_ls_com_x2[0], MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
		ret = xof_update(&xof_ctx_hash_ls_com, hash_ls_com_x2[1], MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
	}
#endif
	uint8_t hash_ls_com[MQOM2_PARAM_DIGEST_SIZE];
	for (; e < MQOM2_PARAM_TAU; e++) {
		const uint8_t* path_e  = &path[e * (MQOM2_PARAM_NB_EVALS_LOG * MQOM2_PARAM_SEED_SIZE)];
		const uint8_t* out_ls_com_e = &out_ls_com[e * MQOM2_PARAM_DIGEST_SIZE];
		const uint8_t* partial_delta_x_e = &partial_delta_x[e * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE)];

		ret = BLC_Eval_x1_memopt(e, salt, path_e, out_ls_com_e, partial_delta_x_e, i_star[e], hash_ls_com, x_eval[e], u_eval[e]);
		ERR(ret, err);
		ret = xof_update(&xof_ctx_hash_ls_com, hash_ls_com, MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
	}

	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
		ret = xof_update(&xof_ctx_hash_ls_com, &partial_delta_x[e * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE)], BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE);
		ERR(ret, err);
	}
	ret = xof_squeeze(&xof_ctx_hash_ls_com, com1_, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	if (memcmp(com1, com1_, MQOM2_PARAM_DIGEST_SIZE) != 0) {
		ret = -1;
		goto err;
	}

	ret = 0;
err:
	return ret;
}
