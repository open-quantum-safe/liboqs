#include "blc_default.h"
#include "ggm_tree.h"
#include "benchmark.h"
#include "seed_commit.h"

#if defined(SUPERCOP)
#include "crypto_declassify.h"
#endif

#include "blc_common.h"

int BLC_Commit_default(const uint8_t mseed[MQOM2_PARAM_SEED_SIZE], const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)], uint8_t com1[MQOM2_PARAM_DIGEST_SIZE], blc_key_default_t* key, field_ext_elt x0[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], field_ext_elt u0[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)], field_ext_elt u1[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)]) {
	int ret = -1;
	enc_ctx DECL_VAR(ctx_seed_commit1), DECL_VAR(ctx_seed_commit2);
	xof_context DECL_VAR(xof_ctx);
	uint8_t tweaked_salt[MQOM2_PARAM_SALT_SIZE];
	uint8_t delta[MQOM2_PARAM_SEED_SIZE];
	uint8_t rseed[MQOM2_PARAM_TAU][MQOM2_PARAM_SEED_SIZE];
	uint32_t e, i;
	/* The serialization of x */
	uint8_t _x[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)];
	/* Tree rseed PRG salt, set to 0 */
	uint8_t tree_prg_salt[MQOM2_PARAM_SALT_SIZE] = { 0 };
	/* PRG cache */
	prg_key_sched_cache_x8 *prg_cache_x8 = NULL;

	/* Compute the rseed table */
	ret = PRG(tree_prg_salt, 0, mseed, MQOM2_PARAM_TAU * MQOM2_PARAM_SEED_SIZE, (uint8_t*)rseed, NULL);
	ERR(ret, err);
	/* Compute delta */
	field_base_serialize(x, MQOM2_PARAM_MQ_N, _x);
	memcpy(delta, _x, MQOM2_PARAM_SEED_SIZE);

#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	memcpy(key->salt, salt, MQOM2_PARAM_SALT_SIZE);
	memcpy(key->delta, delta, MQOM2_PARAM_SEED_SIZE);
	memcpy((uint8_t*) key->rseed, (uint8_t*) rseed, sizeof(rseed));
#endif

	uint8_t hash_ls_com[MQOM2_PARAM_TAU][MQOM2_PARAM_DIGEST_SIZE];

	/* Define "node" and "ls_com" that point either to the BLC key or to local arrays */
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	uint8_t node_e[MQOM2_PARAM_FULL_TREE_SIZE + 1][MQOM2_PARAM_SEED_SIZE];
	uint8_t ls_com_e[4][MQOM2_PARAM_NB_EVALS][MQOM2_PARAM_DIGEST_SIZE];
#endif
	uint8_t (*node[MQOM2_PARAM_TAU])[MQOM2_PARAM_SEED_SIZE];
	uint8_t (*ls_com[MQOM2_PARAM_TAU])[MQOM2_PARAM_DIGEST_SIZE];
	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
		node[e] = node_e;
		ls_com[e] = ls_com_e[e % 4];
#else
		node[e] = key->node[e];
		ls_com[e] = key->ls_com[e];
#endif
	}

	uint8_t lseed[MQOM2_PARAM_NB_EVALS][MQOM2_PARAM_SEED_SIZE];
	uint8_t exp[MQOM2_PARAM_NB_EVALS][BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)];
	field_base_elt bar_x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
	field_ext_elt bar_u[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)];
	field_ext_elt tmp_n[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)];
	field_ext_elt tmp_eta[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)];
	field_base_elt acc_x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
	uint8_t data_folding[MQOM2_PARAM_NB_EVALS_LOG][BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)];
	uint8_t acc[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)];
	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
		/* Initialize the PRG cache when used */
#ifndef NO_BLC_PRG_CACHE
		prg_cache_x8 = init_prg_cache_x8(PRG_BLC_SIZE);
#endif

		__BENCHMARK_START__(BS_BLC_EXPAND_TREE);
		ret = GGMTree_Expand(salt, rseed[e], delta, e, node[e], lseed);
		ERR(ret, err);
		__BENCHMARK_STOP__(BS_BLC_EXPAND_TREE);

		__BENCHMARK_START__(BS_BLC_SEED_COMMIT);
		TweakSalt(salt, tweaked_salt, 0, e, 0);
		ret = enc_key_sched(&ctx_seed_commit1, tweaked_salt);
		ERR(ret, err);
		tweaked_salt[0] ^= 0x01;
		ret = enc_key_sched(&ctx_seed_commit2, tweaked_salt);
		ERR(ret, err);
		__BENCHMARK_STOP__(BS_BLC_SEED_COMMIT);

		memset((uint8_t*) data_folding, 0, sizeof(data_folding));
		memset((uint8_t*) acc, 0, sizeof(acc));
		for (i = 0; i < MQOM2_PARAM_NB_EVALS; i += 8) {
			__BENCHMARK_START__(BS_BLC_SEED_COMMIT);
			SeedCommit_x4(&ctx_seed_commit1, &ctx_seed_commit2,
			              lseed[i + 0], lseed[i + 1], lseed[i + 2], lseed[i + 3], ls_com[e][i + 0], ls_com[e][i + 1], ls_com[e][i + 2], ls_com[e][i + 3]);
			SeedCommit_x4(&ctx_seed_commit1, &ctx_seed_commit2,
			              lseed[i + 4], lseed[i + 5], lseed[i + 6], lseed[i + 7], ls_com[e][i + 4], ls_com[e][i + 5], ls_com[e][i + 6], ls_com[e][i + 7]);
			__BENCHMARK_STOP__(BS_BLC_SEED_COMMIT);

			__BENCHMARK_START__(BS_BLC_PRG);
			memcpy(exp[i + 0], lseed[i + 0], MQOM2_PARAM_SEED_SIZE);
			memcpy(exp[i + 1], lseed[i + 1], MQOM2_PARAM_SEED_SIZE);
			memcpy(exp[i + 2], lseed[i + 2], MQOM2_PARAM_SEED_SIZE);
			memcpy(exp[i + 3], lseed[i + 3], MQOM2_PARAM_SEED_SIZE);
			memcpy(exp[i + 4], lseed[i + 4], MQOM2_PARAM_SEED_SIZE);
			memcpy(exp[i + 5], lseed[i + 5], MQOM2_PARAM_SEED_SIZE);
			memcpy(exp[i + 6], lseed[i + 6], MQOM2_PARAM_SEED_SIZE);
			memcpy(exp[i + 7], lseed[i + 7], MQOM2_PARAM_SEED_SIZE);
			uint8_t *exp_ptr[8] = {
				(uint8_t*) (exp[i + 0] + MQOM2_PARAM_SEED_SIZE),
				(uint8_t*) (exp[i + 1] + MQOM2_PARAM_SEED_SIZE),
				(uint8_t*) (exp[i + 2] + MQOM2_PARAM_SEED_SIZE),
				(uint8_t*) (exp[i + 3] + MQOM2_PARAM_SEED_SIZE),
				(uint8_t*) (exp[i + 4] + MQOM2_PARAM_SEED_SIZE),
				(uint8_t*) (exp[i + 5] + MQOM2_PARAM_SEED_SIZE),
				(uint8_t*) (exp[i + 6] + MQOM2_PARAM_SEED_SIZE),
				(uint8_t*) (exp[i + 7] + MQOM2_PARAM_SEED_SIZE)
			};
			ret = PRG_x8(salt, &e, (const uint8_t (*)[MQOM2_PARAM_SEED_SIZE])&lseed[i], PRG_BLC_SIZE, exp_ptr, prg_cache_x8, 1);
			ERR(ret, err);
			__BENCHMARK_STOP__(BS_BLC_PRG);

			__BENCHMARK_START__(BS_BLC_ARITH);
			for (uint32_t i_ = 0; i_ < 8; i_++) {
				uint8_t j = get_gray_code_bit_position(i + i_);
				field_base_vect_add(acc, exp[i + i_], acc, MQOM2_PARAM_MQ_N + MQOM2_PARAM_ETA * MQOM2_PARAM_MU);
				field_base_vect_add(data_folding[j], acc, data_folding[j], MQOM2_PARAM_MQ_N + MQOM2_PARAM_ETA * MQOM2_PARAM_MU);
			}
			__BENCHMARK_STOP__(BS_BLC_ARITH);
		}
		/* Invalidate the PRG cache */
		destroy_prg_cache_x8(prg_cache_x8);
		prg_cache_x8 = NULL;

		/* Compute P_u and P_z */
		__BENCHMARK_START__(BS_BLC_ARITH);
		field_base_parse(acc, MQOM2_PARAM_MQ_N, acc_x);
		field_ext_parse(acc + BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N), MQOM2_PARAM_ETA, u1[e]);

		memset(x0[e], 0, sizeof(x0[e]));
		for (uint32_t j = 0; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
			field_base_parse(data_folding[j], MQOM2_PARAM_MQ_N, bar_x);
			field_ext_base_constant_vect_mult((1 << j), bar_x, tmp_n, MQOM2_PARAM_MQ_N);
			field_ext_vect_add(x0[e], tmp_n, x0[e], MQOM2_PARAM_MQ_N);
		}

		memset(u0[e], 0, sizeof(u0[e]));
		for (uint32_t j = 0; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
			field_ext_parse(data_folding[j] + BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N), MQOM2_PARAM_ETA, bar_u);
			field_ext_constant_vect_mult((1 << j), bar_u, tmp_eta, MQOM2_PARAM_ETA);
			field_ext_vect_add(u0[e], tmp_eta, u0[e], MQOM2_PARAM_ETA);
		}

		field_base_elt delta_x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
		uint8_t serialized_delta_x[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)];
		field_base_vect_add(x, acc_x, delta_x, MQOM2_PARAM_MQ_N);
		field_base_serialize(delta_x, MQOM2_PARAM_MQ_N, serialized_delta_x);
		memcpy(key->partial_delta_x[e], serialized_delta_x + MQOM2_PARAM_SEED_SIZE, BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE);
		__BENCHMARK_STOP__(BS_BLC_ARITH);

		__BENCHMARK_START__(BS_BLC_XOF);
#if defined(USE_XOF_X4)
		if ((e % 4) == 3) {
			xof_context_x4 DECL_VAR(xof_ctx_x4);
			/* Use the X4 XOF on the previously computed 4 */
			const uint8_t *constant_6[4] = { (const uint8_t*) "\x06", (const uint8_t*) "\x06", (const uint8_t*) "\x06", (const uint8_t*) "\x06" };
			const uint8_t *to_hash_ptr[4] = { (const uint8_t*) ls_com[e - 3], (const uint8_t*) ls_com[e - 2], (const uint8_t*) ls_com[e - 1], (const uint8_t*) ls_com[e] };
			uint8_t *hash_ptr[4] = { hash_ls_com[e - 3], hash_ls_com[e - 2], hash_ls_com[e - 1], hash_ls_com[e] };
			ret = xof_init_x4(&xof_ctx_x4);
			ERR(ret, err);
			ret = xof_update_x4(&xof_ctx_x4, constant_6, 1);
			ERR(ret, err);
			ret = xof_update_x4(&xof_ctx_x4, to_hash_ptr, MQOM2_PARAM_NB_EVALS * MQOM2_PARAM_DIGEST_SIZE);
			ERR(ret, err);
			ret = xof_squeeze_x4(&xof_ctx_x4, hash_ptr, MQOM2_PARAM_DIGEST_SIZE);
			ERR(ret, err);
			xof_clean_ctx_x4(&xof_ctx_x4);
		} else if (e >= (4 * (MQOM2_PARAM_TAU / 4)))
			/* No room for X4 XOF, perform regular */
		{
			ret = xof_init(&xof_ctx);
			ERR(ret, err);
			ret = xof_update(&xof_ctx, (const uint8_t*) "\x06", 1);
			ERR(ret, err);
			ret = xof_update(&xof_ctx, (const uint8_t*) ls_com[e], MQOM2_PARAM_NB_EVALS * MQOM2_PARAM_DIGEST_SIZE);
			ERR(ret, err);
			ret = xof_squeeze(&xof_ctx, hash_ls_com[e], MQOM2_PARAM_DIGEST_SIZE);
			ERR(ret, err);
		}
#else
		ret = xof_init(&xof_ctx);
		ERR(ret, err);
		ret = xof_update(&xof_ctx, (const uint8_t*) "\x06", 1);
		ERR(ret, err);
		ret = xof_update(&xof_ctx, (const uint8_t*) ls_com[e], MQOM2_PARAM_NB_EVALS * MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
		ret = xof_squeeze(&xof_ctx, hash_ls_com[e], MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
#endif
		__BENCHMARK_STOP__(BS_BLC_XOF);
	}

	__BENCHMARK_START__(BS_BLC_XOF);
	ret = xof_init(&xof_ctx);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, (const uint8_t*) "\x07", 1);
	ERR(ret, err);
	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
		ret = xof_update(&xof_ctx, hash_ls_com[e], MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
	}
	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
		ret = xof_update(&xof_ctx, key->partial_delta_x[e], BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE);
		ERR(ret, err);
	}
	ret = xof_squeeze(&xof_ctx, com1, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_XOF);

	ret = 0;
err:
	enc_clean_ctx(&ctx_seed_commit1);
	enc_clean_ctx(&ctx_seed_commit2);
	xof_clean_ctx(&xof_ctx);
	destroy_prg_cache_x8(prg_cache_x8);
	return ret;
}

int BLC_Open_default(const blc_key_default_t* key, const uint16_t i_star[MQOM2_PARAM_TAU], uint8_t opening[MQOM2_PARAM_OPENING_SIZE]) {
	int ret = -1;
	int e;
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	enc_ctx DECL_VAR(ctx_seed_commit1), DECL_VAR(ctx_seed_commit2);
	uint8_t lseed[MQOM2_PARAM_SEED_SIZE];
	uint8_t tweaked_salt[MQOM2_PARAM_SALT_SIZE];
#endif

	uint8_t *path = &opening[0];
	uint8_t *out_ls_com = &opening[MQOM2_PARAM_TAU * MQOM2_PARAM_SEED_SIZE * MQOM2_PARAM_NB_EVALS_LOG];
	uint8_t *partial_delta_x = &opening[MQOM2_PARAM_TAU * (MQOM2_PARAM_SEED_SIZE * MQOM2_PARAM_NB_EVALS_LOG + MQOM2_PARAM_DIGEST_SIZE)];

	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
#if defined(SUPERCOP)
		/* XXX: NOTE: we explicitly declassify i_star[e] as it is public data but comes from a dataflow involving secret data
		 * through hashing */
		crypto_declassify(&i_star[e], sizeof(i_star[e]));
#endif
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
		ret = GGMTree_ExpandPath(key->salt, key->rseed[e], key->delta, e, i_star[e], (uint8_t(*)[MQOM2_PARAM_SEED_SIZE]) &path[e * (MQOM2_PARAM_NB_EVALS_LOG * MQOM2_PARAM_SEED_SIZE)], lseed);
		ERR(ret, err);
		TweakSalt(key->salt, tweaked_salt, 0, e, 0);
		ret = enc_key_sched(&ctx_seed_commit1, tweaked_salt);
		ERR(ret, err);
		tweaked_salt[0] ^= 0x01;
		ret = enc_key_sched(&ctx_seed_commit2, tweaked_salt);
		ERR(ret, err);
		SeedCommit(&ctx_seed_commit1, &ctx_seed_commit2, lseed, &out_ls_com[e * MQOM2_PARAM_DIGEST_SIZE]);
#else
		ret = GGMTree_Open(key->node[e], i_star[e], (uint8_t(*)[MQOM2_PARAM_SEED_SIZE]) &path[e * (MQOM2_PARAM_NB_EVALS_LOG * MQOM2_PARAM_SEED_SIZE)]);
		ERR(ret, err);
		memcpy(&out_ls_com[e * MQOM2_PARAM_DIGEST_SIZE], key->ls_com[e][i_star[e]], MQOM2_PARAM_DIGEST_SIZE);
#endif

		memcpy(&partial_delta_x[e * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE)], key->partial_delta_x[e], BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE);
	}

	ret = 0;
err:
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	enc_clean_ctx(&ctx_seed_commit1);
	enc_clean_ctx(&ctx_seed_commit2);
#endif
	return ret;
}

/* Deal with X4 XOF buffering optimization in BLC_Eval */
#if defined(USE_XOF_X4)
#define LS_COMM_E_ALLOC 4
#define LS_COMM_E_COEFF(e) ((e) % 4)
#else
#define LS_COMM_E_ALLOC 1
#define LS_COMM_E_COEFF(e) 0
#endif

int BLC_Eval_default(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t com1[MQOM2_PARAM_DIGEST_SIZE], const uint8_t opening[MQOM2_PARAM_OPENING_SIZE], const uint16_t i_star[MQOM2_PARAM_TAU], field_ext_elt x_eval[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], field_ext_elt u_eval[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)]) {
	int ret = -1;
	enc_ctx_pub DECL_VAR(ctx_seed_commit1), DECL_VAR(ctx_seed_commit2);
	xof_context DECL_VAR(xof_ctx);
	uint32_t e, i;
	uint8_t tweaked_salt[MQOM2_PARAM_SALT_SIZE];
	uint8_t lseed[MQOM2_PARAM_NB_EVALS][MQOM2_PARAM_SEED_SIZE];
	uint8_t com1_[MQOM2_PARAM_DIGEST_SIZE];
	uint8_t ls_com_e[LS_COMM_E_ALLOC][MQOM2_PARAM_NB_EVALS][MQOM2_PARAM_DIGEST_SIZE];
	uint8_t exp[MQOM2_PARAM_NB_EVALS][BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)];

	const uint8_t *path = &opening[0];
	const uint8_t *out_ls_com = &opening[MQOM2_PARAM_TAU * MQOM2_PARAM_SEED_SIZE * MQOM2_PARAM_NB_EVALS_LOG];
	const uint8_t *partial_delta_x = &opening[MQOM2_PARAM_TAU * (MQOM2_PARAM_SEED_SIZE * MQOM2_PARAM_NB_EVALS_LOG + MQOM2_PARAM_DIGEST_SIZE)];

	uint8_t hash_ls_com[MQOM2_PARAM_TAU][MQOM2_PARAM_DIGEST_SIZE];

	/* PRG cache */
	prg_key_sched_cache_pub_x8 *prg_cache_x8 = NULL;

	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
		/* Initialize the PRG cache when used */
#ifndef NO_BLC_PRG_CACHE
		prg_cache_x8 = init_prg_cache_pub_x8(PRG_BLC_SIZE);
#endif

		ret = GGMTree_PartiallyExpand(salt, (const uint8_t(*)[MQOM2_PARAM_SEED_SIZE]) &path[e * (MQOM2_PARAM_NB_EVALS_LOG * MQOM2_PARAM_SEED_SIZE)], e, i_star[e], lseed);
		ERR(ret, err);

		TweakSalt(salt, tweaked_salt, 0, e, 0);
		ret = enc_key_sched_pub(&ctx_seed_commit1, tweaked_salt);
		ERR(ret, err);
		tweaked_salt[0] ^= 0x01;
		ret = enc_key_sched_pub(&ctx_seed_commit2, tweaked_salt);
		ERR(ret, err);

		field_base_elt bar_x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
		field_ext_elt bar_u[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)];
		//field_base_elt bar_x_i[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)] = {0};
		//field_ext_elt bar_u_i[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)] = {0};
		field_ext_elt tmp_n[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)];
		field_ext_elt tmp_eta[FIELD_EXT_PACKING(MQOM2_PARAM_ETA)];
		field_base_elt acc_x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
		uint8_t data_folding[MQOM2_PARAM_NB_EVALS_LOG][BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)];
		uint8_t acc[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM2_PARAM_ETA)];

		memset((uint8_t*) data_folding, 0, sizeof(data_folding));
		memset((uint8_t*) acc, 0, sizeof(acc));
		for (i = 0; i < MQOM2_PARAM_NB_EVALS; i += 8) {
			// lseed[i_star[e]] is a bytestring with only zeroes
			// it does not prevent to compute seed commitment of it,
			// but we will not use the result
			SeedCommit_x4_pub(&ctx_seed_commit1, &ctx_seed_commit2, lseed[i + 0], lseed[i + 1], lseed[i + 2], lseed[i + 3],
			                  ls_com_e[LS_COMM_E_COEFF(e)][i + 0], ls_com_e[LS_COMM_E_COEFF(e)][i + 1], ls_com_e[LS_COMM_E_COEFF(e)][i + 2], ls_com_e[LS_COMM_E_COEFF(e)][i + 3]);
			SeedCommit_x4_pub(&ctx_seed_commit1, &ctx_seed_commit2, lseed[i + 4], lseed[i + 5], lseed[i + 6], lseed[i + 7],
			                  ls_com_e[LS_COMM_E_COEFF(e)][i + 4], ls_com_e[LS_COMM_E_COEFF(e)][i + 5], ls_com_e[LS_COMM_E_COEFF(e)][i + 6], ls_com_e[LS_COMM_E_COEFF(e)][i + 7]);

			memcpy(exp[i + 0], lseed[i + 0], MQOM2_PARAM_SEED_SIZE);
			memcpy(exp[i + 1], lseed[i + 1], MQOM2_PARAM_SEED_SIZE);
			memcpy(exp[i + 2], lseed[i + 2], MQOM2_PARAM_SEED_SIZE);
			memcpy(exp[i + 3], lseed[i + 3], MQOM2_PARAM_SEED_SIZE);
			memcpy(exp[i + 4], lseed[i + 4], MQOM2_PARAM_SEED_SIZE);
			memcpy(exp[i + 5], lseed[i + 5], MQOM2_PARAM_SEED_SIZE);
			memcpy(exp[i + 6], lseed[i + 6], MQOM2_PARAM_SEED_SIZE);
			memcpy(exp[i + 7], lseed[i + 7], MQOM2_PARAM_SEED_SIZE);
			uint8_t *exp_ptr[8] = {
				(uint8_t*) (exp[i + 0] + MQOM2_PARAM_SEED_SIZE),
				(uint8_t*) (exp[i + 1] + MQOM2_PARAM_SEED_SIZE),
				(uint8_t*) (exp[i + 2] + MQOM2_PARAM_SEED_SIZE),
				(uint8_t*) (exp[i + 3] + MQOM2_PARAM_SEED_SIZE),
				(uint8_t*) (exp[i + 4] + MQOM2_PARAM_SEED_SIZE),
				(uint8_t*) (exp[i + 5] + MQOM2_PARAM_SEED_SIZE),
				(uint8_t*) (exp[i + 6] + MQOM2_PARAM_SEED_SIZE),
				(uint8_t*) (exp[i + 7] + MQOM2_PARAM_SEED_SIZE)
			};
			ret = PRG_x8_pub(salt, &e, (const uint8_t (*)[MQOM2_PARAM_SEED_SIZE])&lseed[i], PRG_BLC_SIZE, exp_ptr, prg_cache_x8, 1);
			ERR(ret, err);

			for (uint32_t i_ = 0; i_ < 8; i_++) {
				if (i + i_ == i_star[e]) {
					memcpy(ls_com_e[LS_COMM_E_COEFF(e)][i_star[e]], &out_ls_com[e * MQOM2_PARAM_DIGEST_SIZE], MQOM2_PARAM_DIGEST_SIZE);
					memset(exp[i_star[e]], 0, MQOM2_PARAM_SEED_SIZE + PRG_BLC_SIZE);
				}
				field_base_vect_add(acc, exp[i + i_], acc, MQOM2_PARAM_MQ_N + MQOM2_PARAM_ETA * MQOM2_PARAM_MU);
				uint8_t j = get_gray_code_bit_position(i + i_);
				field_base_vect_add(data_folding[j], acc, data_folding[j], MQOM2_PARAM_MQ_N + MQOM2_PARAM_ETA * MQOM2_PARAM_MU);
			}
		}
		/* Invalidate the PRG cache */
		destroy_prg_cache_pub_x8(prg_cache_x8);
		prg_cache_x8 = NULL;

		field_ext_elt r = get_evaluation_point(i_star[e]);
		field_base_elt delta_x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)];
		uint8_t serialized_delta_x[BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N)];
		memset(serialized_delta_x, 0, MQOM2_PARAM_SEED_SIZE);
		memcpy(serialized_delta_x + MQOM2_PARAM_SEED_SIZE, &partial_delta_x[e * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE)], BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE);
		field_base_parse(serialized_delta_x, MQOM2_PARAM_MQ_N, delta_x);

		// Compute v_u and v_x
		memset(x_eval[e], 0, sizeof(x_eval[e]));
		for (uint32_t j = 0; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
			field_base_parse(data_folding[j], MQOM2_PARAM_MQ_N, bar_x);
			field_ext_base_constant_vect_mult((1 << j), bar_x, tmp_n, MQOM2_PARAM_MQ_N);
			field_ext_vect_add(x_eval[e], tmp_n, x_eval[e], MQOM2_PARAM_MQ_N);
		}
		field_base_parse(acc, MQOM2_PARAM_MQ_N, acc_x);
		field_base_vect_add(acc_x, delta_x, acc_x, MQOM2_PARAM_MQ_N);
		field_ext_base_constant_vect_mult(r, acc_x, tmp_n, MQOM2_PARAM_MQ_N);
		field_ext_vect_add(x_eval[e], tmp_n, x_eval[e], MQOM2_PARAM_MQ_N);

		memset((uint8_t*) u_eval[e], 0, sizeof(u_eval[e]));
		for (uint32_t j = 0; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
			field_ext_parse(data_folding[j] + BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N), MQOM2_PARAM_ETA, bar_u);
			field_ext_constant_vect_mult((1 << j), bar_u, tmp_eta, MQOM2_PARAM_ETA);
			field_ext_vect_add(u_eval[e], tmp_eta, u_eval[e], MQOM2_PARAM_ETA);
		}
		field_ext_parse(acc + BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N), MQOM2_PARAM_ETA, tmp_eta);
		field_ext_constant_vect_mult(r, tmp_eta, tmp_eta, MQOM2_PARAM_ETA);
		field_ext_vect_add(u_eval[e], tmp_eta, u_eval[e], MQOM2_PARAM_ETA);

#if defined(USE_XOF_X4)
		if ((e % 4) == 3) {
			/* Use the X4 XOF on the previously computed 4 */
			xof_context_x4 DECL_VAR(xof_ctx_x4);
			const uint8_t *constant_6[4] = { (const uint8_t*) "\x06", (const uint8_t*) "\x06", (const uint8_t*) "\x06", (const uint8_t*) "\x06" };
			const uint8_t *to_hash_ptr[4] = { (const uint8_t*) ls_com_e[LS_COMM_E_COEFF(e - 3)], (const uint8_t*) ls_com_e[LS_COMM_E_COEFF(e - 2)], (const uint8_t*) ls_com_e[LS_COMM_E_COEFF(e - 1)], (const uint8_t*) ls_com_e[LS_COMM_E_COEFF(e)] };
			uint8_t *hash_ptr[4] = { hash_ls_com[e - 3], hash_ls_com[e - 2], hash_ls_com[e - 1], hash_ls_com[e] };
			ret = xof_init_x4(&xof_ctx_x4);
			ERR(ret, err);
			ret = xof_update_x4(&xof_ctx_x4, constant_6, 1);
			ERR(ret, err);
			ret = xof_update_x4(&xof_ctx_x4, to_hash_ptr, MQOM2_PARAM_NB_EVALS * MQOM2_PARAM_DIGEST_SIZE);
			ERR(ret, err);
			ret = xof_squeeze_x4(&xof_ctx_x4, hash_ptr, MQOM2_PARAM_DIGEST_SIZE);
			ERR(ret, err);
			xof_clean_ctx_x4(&xof_ctx_x4);
		} else if (e >= (4 * (MQOM2_PARAM_TAU / 4)))
			/* No room for X4 XOF, perform regular */
		{
			ret = xof_init(&xof_ctx);
			ERR(ret, err);
			ret = xof_update(&xof_ctx, (const uint8_t*) "\x06", 1);
			ERR(ret, err);
			ret = xof_update(&xof_ctx, (const uint8_t*) ls_com_e[LS_COMM_E_COEFF(e)], MQOM2_PARAM_NB_EVALS * MQOM2_PARAM_DIGEST_SIZE);
			ERR(ret, err);
			ret = xof_squeeze(&xof_ctx, hash_ls_com[e], MQOM2_PARAM_DIGEST_SIZE);
			ERR(ret, err);
		}
#else
		ret = xof_init(&xof_ctx);
		ERR(ret, err);
		ret = xof_update(&xof_ctx, (const uint8_t*) "\x06", 1);
		ERR(ret, err);
		ret = xof_update(&xof_ctx, (const uint8_t*) ls_com_e[LS_COMM_E_COEFF(e)], MQOM2_PARAM_NB_EVALS * MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
		ret = xof_squeeze(&xof_ctx, hash_ls_com[e], MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
#endif
	}

	ret = xof_init(&xof_ctx);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, (const uint8_t*) "\x07", 1);
	ERR(ret, err);
	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
		ret = xof_update(&xof_ctx, hash_ls_com[e], MQOM2_PARAM_DIGEST_SIZE);
		ERR(ret, err);
	}
	for (e = 0; e < MQOM2_PARAM_TAU; e++) {
		ret = xof_update(&xof_ctx, &partial_delta_x[e * (BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE)], BYTE_SIZE_FIELD_BASE(MQOM2_PARAM_MQ_N) - MQOM2_PARAM_SEED_SIZE);
		ERR(ret, err);
	}
	ret = xof_squeeze(&xof_ctx, com1_, MQOM2_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	if (memcmp(com1, com1_, MQOM2_PARAM_DIGEST_SIZE) != 0) {
		ret = -1;
		goto err;
	}

	ret = 0;
err:
	enc_clean_ctx_pub(&ctx_seed_commit1);
	enc_clean_ctx_pub(&ctx_seed_commit2);
	xof_clean_ctx(&xof_ctx);
	destroy_prg_cache_pub_x8(prg_cache_x8);
	return ret;
}
