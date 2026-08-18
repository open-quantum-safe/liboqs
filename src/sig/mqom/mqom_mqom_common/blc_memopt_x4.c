#include "blc_memopt.h"
#include "ggm_tree.h"
#include "benchmark.h"
#include "seed_commit.h"
#include "blc_memopt_common.h"

int BLC_Commit_x4_memopt(const uint32_t e[4], const uint8_t rseed[4][MQOM2_PARAM_SEED_SIZE], const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)], const uint8_t delta[MQOM2_PARAM_SEED_SIZE], uint8_t com[4][MQOM2_PARAM_DIGEST_SIZE], uint8_t partial_delta_x[4][BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)-MQOM2_PARAM_SEED_SIZE], field_ext_elt x0[4][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], field_ext_elt u0[4][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)], field_ext_elt u1[4][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)]) {
	int ret = -1;
	uint32_t i;
	prg_key_sched_cache_x4* prg_cache_x4 = NULL;

	seedcommit_ctx_x4_t DECL_VAR(seedcommit_ctx_x4);
	uint8_t lseed_x4[4][MQOM2_PARAM_SEED_SIZE];
	uint8_t ls_com_x4[4][BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE][MQOM2_PARAM_DIGEST_SIZE];
	uint8_t exp_x4[4 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA))];
	field_base_elt bar_x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
	field_ext_elt bar_u[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)];
	field_ext_elt tmp_n[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)];
	field_ext_elt tmp_eta[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)];
	field_base_elt acc_x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
	uint8_t data_folding_x4[MQOM2_PARAM_NB_EVALS_LOG][4 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA))];
	uint8_t acc_x4[4 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA))];
	xof_context_x4 DECL_VAR(xof_ctx_x4);
	ggmtree_ctx_x4_t DECL_VAR(ggm_tree_x4);
	const uint8_t *ls_com_x4_ptr[4] = {(uint8_t*) ls_com_x4[0], (uint8_t*) ls_com_x4[1], (uint8_t*) ls_com_x4[2], (uint8_t*) ls_com_x4[3]};

	/* Initialize the PRG cache when used */
#ifndef NO_BLC_PRG_CACHE
	prg_cache_x4 = init_prg_cache_x4(PRG_BLC_SIZE);
#endif

	__BENCHMARK_START__(BS_BLC_EXPAND_TREE);
	ret = GGMTree_InitIncrementalExpansion_x4(&ggm_tree_x4, salt, rseed, delta, e);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_EXPAND_TREE);

	__BENCHMARK_START__(BS_BLC_SEED_COMMIT);
	ret = init_seedcommit_x4(&seedcommit_ctx_x4, salt, e);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_SEED_COMMIT);

	__BENCHMARK_START__(BS_BLC_XOF);
	ret = xof_init_x4(&xof_ctx_x4);
	ERR(ret, err);
	const uint8_t *constant_6[4] = { (const uint8_t*) "\x06", (const uint8_t*) "\x06", (const uint8_t*) "\x06", (const uint8_t*) "\x06" };
	ret = xof_update_x4(&xof_ctx_x4, constant_6, 1);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_XOF);

	memset((uint8_t*) data_folding_x4, 0, sizeof(data_folding_x4));
	memset((uint8_t*) acc_x4, 0, sizeof(acc_x4));
	for (i = 0; i < MQOM2_PARAM_NB_EVALS; i++) {
		uint32_t i_mod = i % BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE;
		__BENCHMARK_START__(BS_BLC_EXPAND_TREE);
		GGMTree_GetNextLeaf_x4(&ggm_tree_x4, lseed_x4);
		__BENCHMARK_STOP__(BS_BLC_EXPAND_TREE);

		__BENCHMARK_START__(BS_BLC_SEED_COMMIT);
		SeedCommit_x4_x4(&seedcommit_ctx_x4, lseed_x4[0], lseed_x4[1], lseed_x4[2], lseed_x4[3], ls_com_x4[0][i_mod], ls_com_x4[1][i_mod], ls_com_x4[2][i_mod], ls_com_x4[3][i_mod]);
		__BENCHMARK_STOP__(BS_BLC_SEED_COMMIT);

		if (i_mod == BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE - 1) {
			__BENCHMARK_START__(BS_BLC_XOF);
			ret = xof_update_x4(&xof_ctx_x4, ls_com_x4_ptr, BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE * MQOM2_PARAM_DIGEST_SIZE);
			ERR(ret, err);
			__BENCHMARK_STOP__(BS_BLC_XOF);
		}

		__BENCHMARK_START__(BS_BLC_PRG);
		memcpy(exp_x4 + 0 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)), lseed_x4[0], MQOM2_PARAM_SEED_SIZE);
		memcpy(exp_x4 + 1 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)), lseed_x4[1], MQOM2_PARAM_SEED_SIZE);
		memcpy(exp_x4 + 2 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)), lseed_x4[2], MQOM2_PARAM_SEED_SIZE);
		memcpy(exp_x4 + 3 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)), lseed_x4[3], MQOM2_PARAM_SEED_SIZE);
		uint8_t *exp_ptr[4] = {
			(uint8_t*) (exp_x4 + 0 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)) + MQOM2_PARAM_SEED_SIZE),
			(uint8_t*) (exp_x4 + 1 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)) + MQOM2_PARAM_SEED_SIZE),
			(uint8_t*) (exp_x4 + 2 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)) + MQOM2_PARAM_SEED_SIZE),
			(uint8_t*) (exp_x4 + 3 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)) + MQOM2_PARAM_SEED_SIZE)
		};
		ret = PRG_x4(salt, e, (const uint8_t (*)[MQOM2_PARAM_SEED_SIZE])lseed_x4, PRG_BLC_SIZE, exp_ptr, prg_cache_x4, 4);
		ERR(ret, err);
		__BENCHMARK_STOP__(BS_BLC_PRG);

		__BENCHMARK_START__(BS_BLC_ARITH);
		field_base_vect_add(acc_x4, exp_x4, acc_x4, 4 * (MQOM2_PARAM_MQ_N + MQOM2_PARAM_ETA * MQOM2_PARAM_MU));
		uint8_t j = get_gray_code_bit_position(i);
		field_base_vect_add(data_folding_x4[j], acc_x4, data_folding_x4[j], 4 * (MQOM2_PARAM_MQ_N + MQOM2_PARAM_ETA * MQOM2_PARAM_MU));
		__BENCHMARK_STOP__(BS_BLC_ARITH);
	}

	__BENCHMARK_START__(BS_BLC_ARITH);
	for (uint32_t e_ = 0; e_ < 4; e_++) {
		memset(x0[e_], 0, BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_MQ_N));
		for (uint32_t j = 0; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
			field_base_parse(data_folding_x4[j] + e_ * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)), MQOM2_PARAM_MQ_N, bar_x);
			field_ext_base_constant_vect_mult((1 << j), bar_x, tmp_n, MQOM2_PARAM_MQ_N);
			field_ext_vect_add(x0[e_], tmp_n, x0[e_], MQOM2_PARAM_MQ_N);
		}

		memset(u0[e_], 0, BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA));
		for (uint32_t j = 0; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
			field_ext_parse(data_folding_x4[j] + BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + e_ * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)), MQOM2_PARAM_ETA, bar_u);
			field_ext_constant_vect_mult((1 << j), bar_u, tmp_eta, MQOM2_PARAM_ETA);
			field_ext_vect_add(u0[e_], tmp_eta, u0[e_], MQOM2_PARAM_ETA);
		}
	}

	field_ext_parse(acc_x4 + BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N), MQOM2_PARAM_ETA, u1[0]);
	field_ext_parse(acc_x4 + BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + 1 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)), MQOM2_PARAM_ETA, u1[1]);
	field_ext_parse(acc_x4 + BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + 2 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)), MQOM2_PARAM_ETA, u1[2]);
	field_ext_parse(acc_x4 + BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + 3 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)), MQOM2_PARAM_ETA, u1[3]);

	field_base_elt delta_x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
	uint8_t serialized_delta_x[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)];
	for (uint32_t e_ = 0; e_ < 4; e_++) {
		field_base_parse(&acc_x4[e_ * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA))], MQOM2_PARAM_MQ_N, acc_x);
		field_base_vect_add(x, acc_x, delta_x, MQOM2_PARAM_MQ_N);
		field_base_serialize(delta_x, MQOM2_PARAM_MQ_N, serialized_delta_x);
		memcpy(partial_delta_x[e_], serialized_delta_x + MQOM2_PARAM_SEED_SIZE, BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE);
	}
	__BENCHMARK_STOP__(BS_BLC_ARITH);

	__BENCHMARK_START__(BS_BLC_XOF);
	uint8_t *hash_ptr[4] = { com[0], com[1], com[2], com[3] };
	ret = xof_squeeze_x4(&xof_ctx_x4, hash_ptr, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_XOF);

	ret = 0;
err:
	seedcommit_clean_ctx_x4(&seedcommit_ctx_x4);
	destroy_prg_cache_x4(prg_cache_x4);
	xof_clean_ctx_x4(&xof_ctx_x4);
	ggmtree_ctx_x4_t_clean(&ggm_tree_x4);
	return ret;
}

int BLC_Eval_x4_memopt(const uint32_t e[4], const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t path[4][MQOM2_PARAM_SEED_SIZE*MQOM2_PARAM_NB_EVALS_LOG], const uint8_t out_ls_com[4][MQOM2_PARAM_DIGEST_SIZE], const uint8_t partial_delta_x[4][BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)-MQOM2_PARAM_SEED_SIZE], const uint16_t i_star[4], uint8_t com[4][MQOM2_PARAM_DIGEST_SIZE], field_ext_elt x_eval[4][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], field_ext_elt u_eval[4][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)]) {
	int ret = -1;
	uint32_t i;
	prg_key_sched_cache_pub_x4* prg_cache_x4 = NULL;

	seedcommit_ctx_pub_x4_t DECL_VAR(seedcommit_ctx_x4);
	uint8_t lseed_x4[4][MQOM2_PARAM_SEED_SIZE];
	uint8_t ls_com_x4[4][BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE][MQOM2_PARAM_DIGEST_SIZE];
	uint8_t exp_x4[4 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA))];
	uint8_t data_folding_x4[MQOM2_PARAM_NB_EVALS_LOG][4 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA))];
	uint8_t acc_x4[4 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA))];
	field_base_elt bar_x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
	field_ext_elt bar_u[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)];
	field_ext_elt tmp_n[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)];
	field_ext_elt tmp_eta[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)];
	field_base_elt acc_x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
	xof_context_x4 DECL_VAR(xof_ctx_x4);
	ggmtree_ctx_partial_x4_t DECL_VAR(ggm_tree_x4);
	const uint8_t *ls_com_x4_ptr[4] = {(uint8_t*) ls_com_x4[0], (uint8_t*) ls_com_x4[1], (uint8_t*) ls_com_x4[2], (uint8_t*) ls_com_x4[3]};

	/* Initialize the PRG cache when used */
#ifndef NO_BLC_PRG_CACHE
	prg_cache_x4 = init_prg_cache_pub_x4(PRG_BLC_SIZE);
#endif

	const uint8_t (*paths[4])[MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE] = {
		(const uint8_t(*)[MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE]) &path[0],
		(const uint8_t(*)[MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE]) &path[1],
		(const uint8_t(*)[MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE]) &path[2],
		(const uint8_t(*)[MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE]) &path[3],
	};
	uint32_t i_star_uint32[4] = {i_star[0], i_star[1], i_star[2], i_star[3]};
	ret = GGMTree_InitIncrementalPartialExpansion_x4(&ggm_tree_x4, salt, paths, e, i_star_uint32);
	ERR(ret, err);

	ret = init_seedcommit_pub_x4(&seedcommit_ctx_x4, salt, e);
	ERR(ret, err);

	ret = xof_init_x4(&xof_ctx_x4);
	ERR(ret, err);
	const uint8_t *constant_6[4] = { (const uint8_t*) "\x06", (const uint8_t*) "\x06", (const uint8_t*) "\x06", (const uint8_t*) "\x06" };
	ret = xof_update_x4(&xof_ctx_x4, constant_6, 1);
	ERR(ret, err);

	memset((uint8_t*) data_folding_x4, 0, sizeof(data_folding_x4));
	memset((uint8_t*) acc_x4, 0, sizeof(acc_x4));
	for (i = 0; i < MQOM2_PARAM_NB_EVALS; i++) {
		uint32_t i_mod = i % BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE;
		GGMTree_GetNextLeafPartial_x4(&ggm_tree_x4, lseed_x4);

		SeedCommit_x4_x4_pub(&seedcommit_ctx_x4, lseed_x4[0], lseed_x4[1], lseed_x4[2], lseed_x4[3], ls_com_x4[0][i_mod], ls_com_x4[1][i_mod], ls_com_x4[2][i_mod], ls_com_x4[3][i_mod]);

		for (uint32_t e_ = 0; e_ < 4; e_++) {
			if (i == i_star[e_]) {
				memcpy(ls_com_x4[e_][i_mod], out_ls_com[e_], MQOM2_PARAM_DIGEST_SIZE);
			}
		}

		if (i_mod == BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE - 1) {
			ret = xof_update_x4(&xof_ctx_x4, ls_com_x4_ptr, BLC_NB_SEED_COMMITMENTS_PER_HASH_UPDATE * MQOM2_PARAM_DIGEST_SIZE);
			ERR(ret, err);
		}

		memcpy(exp_x4 + 0 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)), lseed_x4[0], MQOM2_PARAM_SEED_SIZE);
		memcpy(exp_x4 + 1 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)), lseed_x4[1], MQOM2_PARAM_SEED_SIZE);
		memcpy(exp_x4 + 2 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)), lseed_x4[2], MQOM2_PARAM_SEED_SIZE);
		memcpy(exp_x4 + 3 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)), lseed_x4[3], MQOM2_PARAM_SEED_SIZE);
		uint8_t *exp_ptr[4] = {
			(uint8_t*) (exp_x4 + 0 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)) + MQOM2_PARAM_SEED_SIZE),
			(uint8_t*) (exp_x4 + 1 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)) + MQOM2_PARAM_SEED_SIZE),
			(uint8_t*) (exp_x4 + 2 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)) + MQOM2_PARAM_SEED_SIZE),
			(uint8_t*) (exp_x4 + 3 * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)) + MQOM2_PARAM_SEED_SIZE)
		};
		ret = PRG_x4_pub(salt, e, (const uint8_t (*)[MQOM2_PARAM_SEED_SIZE])lseed_x4, PRG_BLC_SIZE, exp_ptr, prg_cache_x4, 4);
		ERR(ret, err);
		for (uint32_t e_ = 0; e_ < 4; e_++) {
			if (i == i_star[e_]) {
				memset(exp_x4 + e_ * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)), 0, MQOM2_PARAM_SEED_SIZE + PRG_BLC_SIZE);
			}
		}

		uint8_t j = get_gray_code_bit_position(i);
		field_base_vect_add(acc_x4, exp_x4, acc_x4, 4 * (MQOM2_PARAM_MQ_N + MQOM2_PARAM_ETA * MQOM2_PARAM_MU));
		field_base_vect_add(data_folding_x4[j], acc_x4, data_folding_x4[j], 4 * (MQOM2_PARAM_MQ_N + MQOM2_PARAM_ETA * MQOM2_PARAM_MU));
	}

	field_ext_elt r[4] = {
		get_evaluation_point(i_star[0]), get_evaluation_point(i_star[1]),
		get_evaluation_point(i_star[2]), get_evaluation_point(i_star[3]),
	};

	for (uint32_t e_ = 0; e_ < 4; e_++) {
		field_base_elt delta_x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
		uint8_t serialized_delta_x[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)];
		memset(serialized_delta_x, 0, MQOM2_PARAM_SEED_SIZE);
		memcpy(serialized_delta_x + MQOM2_PARAM_SEED_SIZE, partial_delta_x[e_], BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE);
		field_base_parse(serialized_delta_x, MQOM2_PARAM_MQ_N, delta_x);

		memset(x_eval[e_], 0, BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_MQ_N));
		for (uint32_t j = 0; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
			field_base_parse(data_folding_x4[j] + e_ * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)), MQOM2_PARAM_MQ_N, bar_x);
			field_ext_base_constant_vect_mult((1 << j), bar_x, tmp_n, MQOM2_PARAM_MQ_N);
			field_ext_vect_add(x_eval[e_], tmp_n, x_eval[e_], MQOM2_PARAM_MQ_N);
		}
		field_base_parse(acc_x4 + e_ * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)), MQOM2_PARAM_MQ_N, acc_x);
		field_base_vect_add(acc_x, delta_x, acc_x, MQOM2_PARAM_MQ_N);
		field_ext_base_constant_vect_mult(r[e_], acc_x, tmp_n, MQOM2_PARAM_MQ_N);
		field_ext_vect_add(x_eval[e_], tmp_n, x_eval[e_], MQOM2_PARAM_MQ_N);

		memset(u_eval[e_], 0, BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA));
		for (uint32_t j = 0; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
			field_ext_parse(data_folding_x4[j] + BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + e_ * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)), MQOM2_PARAM_ETA, bar_u);
			field_ext_constant_vect_mult((1 << j), bar_u, tmp_eta, MQOM2_PARAM_ETA);
			field_ext_vect_add(u_eval[e_], tmp_eta, u_eval[e_], MQOM2_PARAM_ETA);
		}
		field_ext_parse(acc_x4 + BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + e_ * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)), MQOM2_PARAM_ETA, tmp_eta);
		field_ext_constant_vect_mult(r[e_], tmp_eta, tmp_eta, MQOM2_PARAM_ETA);
		field_ext_vect_add(u_eval[e_], tmp_eta, u_eval[e_], MQOM2_PARAM_ETA);
	}

	uint8_t *hash_ptr[4] = { com[0], com[1], com[2], com[3] };
	ret = xof_squeeze_x4(&xof_ctx_x4, hash_ptr, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	ret = 0;
err:
	seedcommit_clean_ctx_pub_x4(&seedcommit_ctx_x4);
	destroy_prg_cache_pub_x4(prg_cache_x4);
	xof_clean_ctx_x4(&xof_ctx_x4);
	ggmtree_ctx_partial_x4_t_clean(&ggm_tree_x4);
	return ret;
}
