#include "blc_memopt.h"
#include "ggm_tree.h"
#include "benchmark.h"
#include "seed_commit.h"
#include "blc_memopt_common.h"

int BLC_Commit_x1_memopt(uint32_t e, const uint8_t rseed[MQOM2_PARAM_SEED_SIZE], const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)], const uint8_t delta[MQOM2_PARAM_SEED_SIZE], uint8_t com[MQOM2_PARAM_DIGEST_SIZE], uint8_t partial_delta_x[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)-MQOM2_PARAM_SEED_SIZE], field_ext_elt x0[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], field_ext_elt u0[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)], field_ext_elt u1[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)]) {
	int ret = -1;
	uint32_t i;
	prg_key_sched_cache* prg_cache = NULL;

	seedcommit_ctx_t DECL_VAR(seedcommit_ctx);
	uint8_t lseed[MQOM2_PARAM_SEED_SIZE];
	uint8_t ls_com[BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE][MQOM2_PARAM_DIGEST_SIZE];
	uint8_t exp[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)];
	field_base_elt bar_x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
	field_ext_elt bar_u[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)];
	field_ext_elt tmp_n[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)];
	/* Alias tmp_eta to save stack space */
	field_ext_elt *tmp_eta = bar_u;
	/* Alias acc_x to save stack space */
	field_base_elt *acc_x = bar_x;
	uint8_t data_folding[MQOM2_PARAM_NB_EVALS_LOG][BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)];
	uint8_t acc[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)];
	xof_context DECL_VAR(xof_ctx);
	ggmtree_ctx_t DECL_VAR(ggm_tree);

	/* Initialize the PRG cache when used */
#ifndef NO_BLC_PRG_CACHE
	prg_cache = init_prg_cache(PRG_BLC_SIZE);
#endif

	__BENCHMARK_START__(BS_BLC_EXPAND_TREE);
	ret = GGMTree_InitIncrementalExpansion(&ggm_tree, salt, rseed, delta, e);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_EXPAND_TREE);

	__BENCHMARK_START__(BS_BLC_SEED_COMMIT);
	ret = init_seedcommit(&seedcommit_ctx, salt, e);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_SEED_COMMIT);

	__BENCHMARK_START__(BS_BLC_XOF);
	ret = xof_init(&xof_ctx);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, (const uint8_t*) "\x06", 1);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_XOF);

	memset((uint8_t*) data_folding, 0, sizeof(data_folding));
	memset((uint8_t*) acc, 0, sizeof(acc));
	for (i = 0; i < MQOM2_PARAM_NB_EVALS; i++) {
		uint32_t i_mod = i % BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE;
		__BENCHMARK_START__(BS_BLC_EXPAND_TREE);
		GGMTree_GetNextLeaf(&ggm_tree, lseed);
		__BENCHMARK_STOP__(BS_BLC_EXPAND_TREE);

		__BENCHMARK_START__(BS_BLC_PRG);
		memcpy(exp, lseed, MQOM2_PARAM_SEED_SIZE);
		ret = PRG(salt, e, lseed, PRG_BLC_SIZE, exp + MQOM2_PARAM_SEED_SIZE, prg_cache);
		ERR(ret, err);
		__BENCHMARK_STOP__(BS_BLC_PRG);

		__BENCHMARK_START__(BS_BLC_ARITH);
		field_base_vect_add(acc, exp, acc, MQOM2_PARAM_MQ_N + MQOM2_PARAM_ETA * MQOM2_PARAM_MU);
		uint8_t j = get_gray_code_bit_position(i);
		field_base_vect_add(data_folding[j], acc, data_folding[j], MQOM2_PARAM_MQ_N + MQOM2_PARAM_ETA * MQOM2_PARAM_MU);
		__BENCHMARK_STOP__(BS_BLC_ARITH);

		__BENCHMARK_START__(BS_BLC_SEED_COMMIT);
		SeedCommit(&seedcommit_ctx, lseed, ls_com[i_mod]);
		__BENCHMARK_STOP__(BS_BLC_SEED_COMMIT);

		if (i_mod == BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE - 1) {
			__BENCHMARK_START__(BS_BLC_XOF);
			ret = xof_update(&xof_ctx, (uint8_t*) ls_com, BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE * MQOM2_PARAM_DIGEST_SIZE);
			ERR(ret, err);
			__BENCHMARK_STOP__(BS_BLC_XOF);
		}
	}

	__BENCHMARK_START__(BS_BLC_ARITH);
	memset(x0, 0, BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_MQ_N));
	for (uint32_t j = 0; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
		field_base_parse(data_folding[j], MQOM2_PARAM_MQ_N, bar_x);
		field_ext_base_constant_vect_mult((1 << j), bar_x, tmp_n, MQOM2_PARAM_MQ_N);
		field_ext_vect_add(x0, tmp_n, x0, MQOM2_PARAM_MQ_N);
	}

	memset(u0, 0, BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA));
	for (uint32_t j = 0; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
		field_ext_parse(data_folding[j] + BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N), MQOM2_PARAM_ETA, bar_u);
		field_ext_constant_vect_mult((1 << j), bar_u, tmp_eta, MQOM2_PARAM_ETA);
		field_ext_vect_add(u0, tmp_eta, u0, MQOM2_PARAM_ETA);
	}

	field_base_parse(acc, MQOM2_PARAM_MQ_N, acc_x);
	field_ext_parse(acc + BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N), MQOM2_PARAM_ETA, u1);

	field_base_elt delta_x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
	uint8_t serialized_delta_x[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)];
	field_base_vect_add(x, acc_x, delta_x, MQOM2_PARAM_MQ_N);
	field_base_serialize(delta_x, MQOM2_PARAM_MQ_N, serialized_delta_x);
	memcpy(partial_delta_x, serialized_delta_x + MQOM2_PARAM_SEED_SIZE, BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE);
	__BENCHMARK_STOP__(BS_BLC_ARITH);

	__BENCHMARK_START__(BS_BLC_XOF);
	ret = xof_squeeze(&xof_ctx, com, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_XOF);

	ret = 0;
err:
	seedcommit_clean_ctx(&seedcommit_ctx);
	destroy_prg_cache(prg_cache);
	xof_clean_ctx(&xof_ctx);
	ggmtree_ctx_t_clean(&ggm_tree);
	return ret;
}

int BLC_Eval_x1_memopt(uint32_t e, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t path[MQOM2_PARAM_SEED_SIZE*MQOM2_PARAM_NB_EVALS_LOG], const uint8_t out_ls_com[MQOM2_PARAM_DIGEST_SIZE], const uint8_t partial_delta_x[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)-MQOM2_PARAM_SEED_SIZE], uint16_t i_star, uint8_t com[MQOM2_PARAM_DIGEST_SIZE], field_ext_elt x_eval[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], field_ext_elt u_eval[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)]) {
	int ret = -1;
	uint32_t i;
	prg_key_sched_cache_pub *prg_cache = NULL;

	seedcommit_ctx_pub_t DECL_VAR(seedcommit_ctx);
	uint8_t lseed[MQOM2_PARAM_SEED_SIZE];
	uint8_t ls_com[BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE][MQOM2_PARAM_DIGEST_SIZE];
	uint8_t exp[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)];
	uint8_t data_folding[MQOM2_PARAM_NB_EVALS_LOG][BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)];
	uint8_t acc[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)];
	field_base_elt bar_x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
	field_ext_elt bar_u[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)];
	field_ext_elt tmp_n[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)];
	/* Alias tmp_eta to save stack space */
	field_ext_elt *tmp_eta = bar_u;
	/* Alias acc_x to save stack space */
	field_base_elt *acc_x = bar_x;
	xof_context DECL_VAR(xof_ctx);
	ggmtree_ctx_partial_t DECL_VAR(ggm_tree);

	/* Initialize the PRG cache when used */
#ifndef NO_BLC_PRG_CACHE
	prg_cache = init_prg_cache_pub(PRG_BLC_SIZE);
#endif

	ret = GGMTree_InitIncrementalPartialExpansion(&ggm_tree, salt, (const uint8_t(*)[MQOM2_PARAM_SEED_SIZE]) path, e, i_star);
	ERR(ret, err);

	ret = init_seedcommit_pub(&seedcommit_ctx, salt, e);
	ERR(ret, err);

	ret = xof_init(&xof_ctx);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, (const uint8_t*) "\x06", 1);
	ERR(ret, err);

	memset((uint8_t*) data_folding, 0, sizeof(data_folding));
	memset((uint8_t*) acc, 0, sizeof(acc));
	for (i = 0; i < MQOM2_PARAM_NB_EVALS; i++) {
		uint32_t i_mod = i % BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE;
		GGMTree_GetNextLeafPartial(&ggm_tree, lseed);

		if (i != i_star) {
			memcpy(exp, lseed, MQOM2_PARAM_SEED_SIZE);
			ret = PRG_pub(salt, e, lseed, PRG_BLC_SIZE, exp + MQOM2_PARAM_SEED_SIZE, prg_cache);
			ERR(ret, err);

			SeedCommit_pub(&seedcommit_ctx, lseed, ls_com[i_mod]);
		} else {
			memset(exp, 0, MQOM2_PARAM_SEED_SIZE + PRG_BLC_SIZE);
			memcpy(ls_com[i_mod], out_ls_com, MQOM2_PARAM_DIGEST_SIZE);
		}

		field_base_vect_add(acc, exp, acc, MQOM2_PARAM_MQ_N + MQOM2_PARAM_ETA * MQOM2_PARAM_MU);
		uint8_t j = get_gray_code_bit_position(i);
		field_base_vect_add(data_folding[j], acc, data_folding[j], MQOM2_PARAM_MQ_N + MQOM2_PARAM_ETA * MQOM2_PARAM_MU);

		if (i_mod == BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE - 1) {
			ret = xof_update(&xof_ctx, (uint8_t*) ls_com, BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE * MQOM2_PARAM_DIGEST_SIZE);
			ERR(ret, err);
		}
	}
	/* Invalidate the PRG cache */
	destroy_prg_cache_pub(prg_cache);
	prg_cache = NULL;

	field_ext_elt r = get_evaluation_point(i_star);

	field_base_elt delta_x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
	uint8_t serialized_delta_x[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)];
	memset(serialized_delta_x, 0, MQOM2_PARAM_SEED_SIZE);
	memcpy(serialized_delta_x + MQOM2_PARAM_SEED_SIZE, partial_delta_x, BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE);
	field_base_parse(serialized_delta_x, MQOM2_PARAM_MQ_N, delta_x);

	memset(x_eval, 0, BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_MQ_N));
	for (uint32_t j = 0; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
		field_base_parse(data_folding[j], MQOM2_PARAM_MQ_N, bar_x);
		field_ext_base_constant_vect_mult((1 << j), bar_x, tmp_n, MQOM2_PARAM_MQ_N);
		field_ext_vect_add(x_eval, tmp_n, x_eval, MQOM2_PARAM_MQ_N);
	}
	field_base_parse(acc, MQOM2_PARAM_MQ_N, acc_x);
	field_base_vect_add(acc_x, delta_x, acc_x, MQOM2_PARAM_MQ_N);
	field_ext_base_constant_vect_mult(r, acc_x, tmp_n, MQOM2_PARAM_MQ_N);
	field_ext_vect_add(x_eval, tmp_n, x_eval, MQOM2_PARAM_MQ_N);

	memset(u_eval, 0, BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA));
	for (uint32_t j = 0; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
		field_ext_parse(data_folding[j] + BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N), MQOM2_PARAM_ETA, bar_u);
		field_ext_constant_vect_mult((1 << j), bar_u, tmp_eta, MQOM2_PARAM_ETA);
		field_ext_vect_add(u_eval, tmp_eta, u_eval, MQOM2_PARAM_ETA);
	}
	field_ext_parse(acc + BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N), MQOM2_PARAM_ETA, tmp_eta);
	field_ext_constant_vect_mult(r, tmp_eta, tmp_eta, MQOM2_PARAM_ETA);
	field_ext_vect_add(u_eval, tmp_eta, u_eval, MQOM2_PARAM_ETA);

	ret = xof_squeeze(&xof_ctx, com, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	ret = 0;
err:
	seedcommit_clean_ctx_pub(&seedcommit_ctx);
	xof_clean_ctx(&xof_ctx);
	destroy_prg_cache_pub(prg_cache);
	ggmtree_ctx_partial_t_clean(&ggm_tree);
	return ret;
}
