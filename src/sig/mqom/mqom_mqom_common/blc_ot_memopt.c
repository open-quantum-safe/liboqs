/* MQOM3_VARIANT_GUARD: this unit implements the OT variant only. Build systems
 * that compile every .c of a directory - SUPERCOP does, and so does any wildcard
 * Makefile - hand it to the compiler for CT instances too, where its parameters
 * simply do not exist. Rather than requiring every such build to filter its source
 * list, the unit reduces to a single typedef there. Not an empty file: ISO C forbids
 * an empty translation unit and -Wpedantic says so.
 * The in-tree Makefile still selects the right family, so nothing changes for it. */
#include "mqom3_parameters.h"
#if MQOM3_PARAM_OT_VARIANT == 1

#include "blc_ot_memopt.h"
#include "benchmark.h"
#include "blc_common.h"
#include "blc_memopt.h"
#include "blc_memopt_folding.h"
#include "blc_memopt_seedcommit.h"
#include "ggm_tree_large_incr.h"
#include "ggm_tree_large_incr_batch.h"
#include "seed_commit.h"
#include "domain_separation.h"

#if defined(SUPERCOP)
#include "crypto_declassify.h"
#endif

/* Optional x2 batching of the large-tree incremental traversal (see
 * ggm_tree_large_incr_batch.h) - off by default, byte-identical output
 * either way. The batched context types carry an extra small leaf buffer,
 * so the dispatch covers the context type and Init/Clean too, not just
 * GetNextLeaf(Partial).
 *
 * Sign and Verify dispatch independently: batching pays off on Sign's
 * constant-time cipher but not on Verify's public one, where it would only
 * cost stack (see the header for the measured trade-off). */
#if LARGE_GGM_TREE_INCR_BATCH == 1
typedef large_ggmtree_incr_batch_ctx_t         large_ggmtree_incr_dispatch_ctx_t;
#define LargeGGMTree_InitIncrementalExpansion_dispatch         LargeGGMTree_InitIncrementalExpansion_batch
#define LargeGGMTree_GetNextLeaf_dispatch                      LargeGGMTree_GetNextLeaf_batch
#define LargeGGMTree_CleanIncrementalExpansion_dispatch        LargeGGMTree_CleanIncrementalExpansion_batch
#else
typedef large_ggmtree_incr_ctx_t         large_ggmtree_incr_dispatch_ctx_t;
#define LargeGGMTree_InitIncrementalExpansion_dispatch         LargeGGMTree_InitIncrementalExpansion
#define LargeGGMTree_GetNextLeaf_dispatch                      LargeGGMTree_GetNextLeaf
#define LargeGGMTree_CleanIncrementalExpansion_dispatch        LargeGGMTree_CleanIncrementalExpansion
#endif

#if LARGE_GGM_TREE_INCR_BATCH_VERIFY == 1
typedef large_ggmtree_incr_batch_partial_ctx_t large_ggmtree_incr_partial_dispatch_ctx_t;
#define LargeGGMTree_InitIncrementalPartialExpansion_dispatch  LargeGGMTree_InitIncrementalPartialExpansion_batch
#define LargeGGMTree_GetNextLeafPartial_dispatch               LargeGGMTree_GetNextLeafPartial_batch
#define LargeGGMTree_CleanIncrementalPartialExpansion_dispatch LargeGGMTree_CleanIncrementalPartialExpansion_batch
#else
typedef large_ggmtree_incr_partial_ctx_t large_ggmtree_incr_partial_dispatch_ctx_t;
#define LargeGGMTree_InitIncrementalPartialExpansion_dispatch  LargeGGMTree_InitIncrementalPartialExpansion
#define LargeGGMTree_GetNextLeafPartial_dispatch               LargeGGMTree_GetNextLeafPartial
#define LargeGGMTree_CleanIncrementalPartialExpansion_dispatch LargeGGMTree_CleanIncrementalPartialExpansion
#endif

static inline uint32_t ot_batch_min(uint32_t a, uint32_t b) {
	return (a < b) ? a : b;
}

int OT_BLC_Commit_memopt_partial(uint32_t e_start, uint32_t batch, const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], uint8_t com1[][MQOM3_PARAM_DIGEST_SIZE], OT_blc_key_memopt_t* key, field_ext_elt x0[][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u0[][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt u1[][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;
	uint32_t b, abs_leaf, exec_e;

	/* Per-batch contexts (up to BLC_OT_BATCH_SIZE executions per tree pass) */
	seedcommit_sign_ctx_t sc_ctx[BLC_OT_BATCH_SIZE] = { 0 };
	folding_sign_t fold_ctx[BLC_OT_BATCH_SIZE];
	large_ggmtree_incr_dispatch_ctx_t DECL_VAR(tree_ctx);

	/* Leaf seed buffer: one slot per execution in the batch, BLC_NB_LEAF_SEEDS_IN_PARALLEL deep */
	uint8_t lseed_buf[BLC_OT_BATCH_SIZE][BLC_NB_LEAF_SEEDS_IN_PARALLEL][MQOM3_PARAM_SEED_SIZE];
	uint32_t buf_count[BLC_OT_BATCH_SIZE];
	uint32_t flush_count[BLC_OT_BATCH_SIZE];
	uint8_t lseed_one[MQOM3_PARAM_SEED_SIZE];
	int leaf_ret;

	if ((batch > BLC_OT_BATCH_SIZE) || ((e_start + batch) > MQOM3_PARAM_TAU)) {
		return -1;
	}

	/* Zero contexts so cleanup is safe even on partial init */
	memset(sc_ctx,   0, batch * sizeof(sc_ctx[0]));
	memset(fold_ctx, 0, batch * sizeof(fold_ctx[0]));

	for (b = 0; b < batch; b++) {
		ret = init_seedcommit_sign(&sc_ctx[b], salt, e_start + b);
		ERR(ret, pass_err);
		ret = InitializeFolding_sign(&fold_ctx[b], salt, e_start + b, x0[b], u0[b]);
		ERR(ret, pass_err);
		buf_count[b]   = 0;
		flush_count[b] = 0;
	}

	/* Traverse the full large GGM tree for this pass */
	__BENCHMARK_START__(BS_BLC_EXPAND_TREE);
	ret = LargeGGMTree_InitIncrementalExpansion_dispatch(&tree_ctx, salt, mseed);
	__BENCHMARK_STOP__(BS_BLC_EXPAND_TREE);
	ERR(ret, pass_err);

	for (abs_leaf = 0; abs_leaf < (uint32_t)(MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS); abs_leaf++) {
		__BENCHMARK_START__(BS_BLC_EXPAND_TREE);
		leaf_ret = LargeGGMTree_GetNextLeaf_dispatch(&tree_ctx, lseed_one);
		__BENCHMARK_STOP__(BS_BLC_EXPAND_TREE);
		if (leaf_ret != 0) {
			ret = (leaf_ret > 0) ? -1 : leaf_ret;
			goto pass_err;
		}

		exec_e = abs_leaf % MQOM3_PARAM_TAU;
		if ((exec_e < e_start) || (exec_e >= (e_start + batch))) {
			continue;
		}
		b = exec_e - e_start;

		memcpy(lseed_buf[b][buf_count[b]], lseed_one, MQOM3_PARAM_SEED_SIZE);
		buf_count[b]++;

		if (buf_count[b] == BLC_NB_LEAF_SEEDS_IN_PARALLEL) {
			ret = SeedCommitThenAbsorb_sign(&sc_ctx[b],
			        (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE]) lseed_buf[b]);
			ERR(ret, pass_err);
			ret = SeedExpandThenAccumulate_sign(&fold_ctx[b],
			        flush_count[b] * BLC_NB_LEAF_SEEDS_IN_PARALLEL,
			        (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE]) lseed_buf[b]);
			ERR(ret, pass_err);
			buf_count[b]   = 0;
			flush_count[b]++;
		}
	}

	/* Finalize: delta_x, com1[e] */
	__BENCHMARK_START__(BS_BLC_ARITH);
	for (b = 0; b < batch; b++) {
		OT_FinalizeFolding_sign(&fold_ctx[b], x, key->delta_x[e_start + b],
		                        x0[b], u0[b], u1[b]);
	}
	__BENCHMARK_STOP__(BS_BLC_ARITH);

	for (b = 0; b < batch; b++) {
		__BENCHMARK_START__(BS_BLC_XOF);
		ret = xof_update(&sc_ctx[b].xof_ctx, key->delta_x[e_start + b],
		                 BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N));
		ERR(ret, pass_err);
		ret = xof_squeeze(&sc_ctx[b].xof_ctx, com1[b], MQOM3_PARAM_DIGEST_SIZE);
		__BENCHMARK_STOP__(BS_BLC_XOF);
		ERR(ret, pass_err);
	}
	ret = 0;

pass_err:
	for (b = 0; b < batch; b++) {
		seedcommit_sign_clean_ctx(&sc_ctx[b]);
		folding_sign_clean_ctx(&fold_ctx[b]);
	}
	LargeGGMTree_CleanIncrementalExpansion_dispatch(&tree_ctx);
	/* lseed_buf/lseed_one hold leaf seeds in the clear, sitting between the two
	 * contexts cleaned above - the same gap that was closed on the CT side. One
	 * wipe per pass, not per leaf, so the cost is a single call. */
	mqom_cleanse(lseed_buf, sizeof(lseed_buf));
	mqom_cleanse(lseed_one, sizeof(lseed_one));
	return ret;
}

int OT_BLC_Commit_memopt(const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], OT_blc_key_memopt_t* key, field_ext_elt x0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt u1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;
	uint32_t e_start, batch;

	memcpy(key->mseed, mseed, MQOM3_PARAM_SEED_SIZE);
	memcpy(key->salt, salt, MQOM3_PARAM_SALT_SIZE);

	for (e_start = 0; e_start < MQOM3_PARAM_TAU; e_start += BLC_OT_BATCH_SIZE) {
		batch = ot_batch_min(BLC_OT_BATCH_SIZE, MQOM3_PARAM_TAU - e_start);
		ret = OT_BLC_Commit_memopt_partial(e_start, batch, mseed, salt, x, &com1[e_start], key, &x0[e_start], &u0[e_start], &u1[e_start]);
		ERR(ret, err);
	}

	ret = 0;
err:
	return ret;
}

int OT_BLC_Open_memopt(const OT_blc_key_memopt_t* key, const uint16_t i_star[MQOM3_PARAM_TAU], const field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], uint8_t opening[MQOM3_PARAM_OPENING_SIZE]) {
	int ret = -1;
	uint32_t e;
	uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU];
	uint8_t hidden_lseeds[MQOM3_PARAM_TAU][MQOM3_PARAM_SEED_SIZE];
	seedcommit_ctx_t seedcommit_ctx = { 0 };
	uint8_t tweaked_salt_0[MQOM3_PARAM_SALT_SIZE], tweaked_salt_1[MQOM3_PARAM_SALT_SIZE];

	/* Opening format: path | ls_com_hidden | delta_x | alpha1_section */
	uint8_t *path           = &opening[0];
	uint8_t *ls_com_hidden  = path + ((size_t)MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE);
	uint8_t *delta_x_out    = ls_com_hidden + ((size_t)MQOM3_PARAM_TAU * MQOM3_PARAM_DIGEST_SIZE);
	uint8_t *alpha1_section = delta_x_out + ((size_t)MQOM3_PARAM_TAU * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N));

	/* hidden_leaf_idxs[e] = TAU * i_star[e] + e (column-major BLC leaf indexing) */
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
#if defined(SUPERCOP)
		/* XXX: NOTE: we explicitly declassify i_star[e] as it is public data but comes from a dataflow involving secret data
		 * through hashing */
		crypto_declassify(&i_star[e], sizeof(i_star[e]));
#endif
		hidden_leaf_idxs[e] = (uint32_t)(MQOM3_PARAM_TAU * (uint32_t)i_star[e] + (uint32_t)e);
	}

	/* Re-expand tree to obtain path and hidden leaf seeds. Explicitly _dfs (not the
	 * generic LargeGGMTree_ExpandPath): under LARGE_TREE_BFS (the OT default), the
	 * generic name resolves to the BFS variant, which mqom_malloc's the *entire*
	 * tree - defeating the point of memopt for this one call. _dfs uses O(H) stack
	 * memory unconditionally, matching what BLC_Commit/Eval already get via the
	 * incremental iterator above/below. */
	ret = LargeGGMTree_ExpandPath_dfs(key->salt, key->mseed, hidden_leaf_idxs, path, hidden_lseeds);
	ERR(ret, err);

	/* Commit each hidden leaf seed (individual SeedCommit, not streaming) */
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		TweakSalt(key->salt, tweaked_salt_0, SALT_SEL_BLC_LEFT, e);
		TweakSalt(key->salt, tweaked_salt_1, SALT_SEL_BLC_RIGHT, e);
		ret = init_seedcommit(&seedcommit_ctx, tweaked_salt_0, tweaked_salt_1);
		ERR(ret, err);
		ret = SeedCommit(&seedcommit_ctx, hidden_lseeds[e],
		           &ls_com_hidden[(size_t)e * MQOM3_PARAM_DIGEST_SIZE]);
		ERR(ret, err);
	}

	/* Serialize delta_x and alpha1 */
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		memcpy(&delta_x_out[(size_t)e * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)],
		       key->delta_x[e], BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N));
		field_ext_serialize(alpha1[e], MQOM3_PARAM_ETA,
		                    &alpha1_section[(size_t)e * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU)]);
	}

	ret = 0;
err:
	mqom_cleanse(hidden_lseeds, sizeof(hidden_lseeds));
	seedcommit_clean_ctx(&seedcommit_ctx);
	return ret;
}

int OT_BLC_Eval_memopt_partial(uint32_t e_start, uint32_t batch, const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t path[MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE], const uint16_t i_star[MQOM3_PARAM_TAU], const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU], const uint8_t *ls_com_hidden, const uint8_t *delta_x_in, uint8_t com1[][MQOM3_PARAM_DIGEST_SIZE], field_ext_elt x_eval[][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u_eval[][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;
	uint32_t b, abs_leaf, exec_e;

	/* Per-batch contexts */
	seedcommit_verify_ctx_t sc_ctx[BLC_OT_BATCH_SIZE] = { 0 };
	folding_verify_t fold_ctx[BLC_OT_BATCH_SIZE];
	large_ggmtree_incr_partial_dispatch_ctx_t DECL_VAR(tree_ctx);

	uint8_t lseed_buf[BLC_OT_BATCH_SIZE][BLC_NB_LEAF_SEEDS_IN_PARALLEL][MQOM3_PARAM_SEED_SIZE];
	uint32_t buf_count[BLC_OT_BATCH_SIZE];
	uint32_t flush_count[BLC_OT_BATCH_SIZE];
	uint8_t lseed_one[MQOM3_PARAM_SEED_SIZE];
	int leaf_ret;

	if ((batch > BLC_OT_BATCH_SIZE) || ((e_start + batch) > MQOM3_PARAM_TAU)) {
		return -1;
	}

	/* Zero contexts so cleanup is safe even on partial init */
	memset(sc_ctx,   0, batch * sizeof(sc_ctx[0]));
	memset(fold_ctx, 0, batch * sizeof(fold_ctx[0]));

	for (b = 0; b < batch; b++) {
		uint32_t exec_e_b = e_start + b;
		ret = init_seedcommit_verify(&sc_ctx[b], salt, exec_e_b, i_star[exec_e_b],
		                             &ls_com_hidden[(size_t)exec_e_b * MQOM3_PARAM_DIGEST_SIZE]);
		ERR(ret, pass_err);
		ret = InitializeFolding_verify(&fold_ctx[b], salt, exec_e_b, x_eval[b], u_eval[b]);
		ERR(ret, pass_err);
		buf_count[b]   = 0;
		flush_count[b] = 0;
	}

	/* Initialize partial expansion iterator for this pass */
	ret = LargeGGMTree_InitIncrementalPartialExpansion_dispatch(&tree_ctx, salt, path, hidden_leaf_idxs);
	ERR(ret, pass_err);

	for (abs_leaf = 0; abs_leaf < (uint32_t)(MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS); abs_leaf++) {
		leaf_ret = LargeGGMTree_GetNextLeafPartial_dispatch(&tree_ctx, lseed_one);
		if (leaf_ret != 0) {
			ret = (leaf_ret > 0) ? -1 : leaf_ret;
			goto pass_err;
		}

		exec_e = abs_leaf % MQOM3_PARAM_TAU;
		if ((exec_e < e_start) || (exec_e >= (e_start + batch))) {
			continue;
		}
		b = exec_e - e_start;

		memcpy(lseed_buf[b][buf_count[b]], lseed_one, MQOM3_PARAM_SEED_SIZE);
		buf_count[b]++;

		if (buf_count[b] == BLC_NB_LEAF_SEEDS_IN_PARALLEL) {
			uint32_t i_base = flush_count[b] * BLC_NB_LEAF_SEEDS_IN_PARALLEL;
			ret = SeedCommitThenAbsorb_verify(&sc_ctx[b], i_base,
			        (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE]) lseed_buf[b]);
			ERR(ret, pass_err);
			ret = SeedExpandThenAccumulate_verify(&fold_ctx[b], i_base,
			        (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE]) lseed_buf[b],
			        i_star[e_start + b]);
			ERR(ret, pass_err);
			buf_count[b]   = 0;
			flush_count[b]++;
		}
	}

	/* Finalize: x_eval, u_eval, com1[e] */
	for (b = 0; b < batch; b++) {
		uint32_t exec_e_b = e_start + b;
		OT_FinalizeFolding_verify(&fold_ctx[b], i_star[exec_e_b],
		                          &delta_x_in[(size_t)exec_e_b * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)],
		                          x_eval[b], u_eval[b]);

		ret = xof_update(&sc_ctx[b].xof_ctx,
		                 &delta_x_in[(size_t)exec_e_b * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)],
		                 BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N));
		ERR(ret, pass_err);
		ret = xof_squeeze(&sc_ctx[b].xof_ctx, com1[b], MQOM3_PARAM_DIGEST_SIZE);
		ERR(ret, pass_err);
	}
	ret = 0;

pass_err:
	for (b = 0; b < batch; b++) {
		seedcommit_verify_clean_ctx(&sc_ctx[b]);
		folding_verify_clean_ctx(&fold_ctx[b]);
	}
	LargeGGMTree_CleanIncrementalPartialExpansion_dispatch(&tree_ctx);
	return ret;
}

int OT_BLC_Eval_memopt(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t opening[MQOM3_PARAM_OPENING_SIZE], const uint16_t i_star[MQOM3_PARAM_TAU], uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], field_ext_elt x_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;
	uint32_t e, e_start, batch;
	uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU];

	/* Parse opening format: path | ls_com_hidden | delta_x | alpha1_section */
	const uint8_t *path           = &opening[0];
	const uint8_t *ls_com_hidden  = path + ((size_t)MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE);
	const uint8_t *delta_x_in     = ls_com_hidden + ((size_t)MQOM3_PARAM_TAU * MQOM3_PARAM_DIGEST_SIZE);
	const uint8_t *alpha1_section = delta_x_in + ((size_t)MQOM3_PARAM_TAU * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N));

	/* hidden_leaf_idxs[e] = TAU * i_star[e] + e */
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		hidden_leaf_idxs[e] = (uint32_t)(MQOM3_PARAM_TAU * (uint32_t)i_star[e] + (uint32_t)e);
	}

	for (e_start = 0; e_start < MQOM3_PARAM_TAU; e_start += BLC_OT_BATCH_SIZE) {
		batch = ot_batch_min(BLC_OT_BATCH_SIZE, MQOM3_PARAM_TAU - e_start);
		ret = OT_BLC_Eval_memopt_partial(e_start, batch, salt, path, i_star, hidden_leaf_idxs,
		                                 ls_com_hidden, delta_x_in, &com1[e_start], &x_eval[e_start], &u_eval[e_start]);
		ERR(ret, err);
	}

	/* Parse alpha1 from opening */
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		field_ext_parse(&alpha1_section[(size_t)e * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU)],
		                MQOM3_PARAM_ETA, alpha1[e]);
	}

	ret = 0;
err:
	return ret;
}

void OT_BLC_PrintConfig_memopt(void) {
	mqom_print("  BLC: memopt OT (one-tree)\r\n");
	mqom_print("    BLC_OT_BATCH_SIZE: %d\r\n", BLC_OT_BATCH_SIZE);

#ifdef SEED_COMMIT_MEMOPT
	mqom_print("    SEED_COMMIT_MEMOPT ON\r\n");
#else
	mqom_print("    SEED_COMMIT_MEMOPT OFF\r\n");
#endif

	mqom_print("    BLC_NB_LEAF_SEEDS_IN_PARALLEL %d\r\n", BLC_NB_LEAF_SEEDS_IN_PARALLEL);
#ifdef BLC_SEEDEXPAND_CACHE
	mqom_print("    SeedExpand cache ON\r\n");
#endif
#ifdef BLC_SEEDCOMMIT_CACHE
	mqom_print("    SeedCommit cache ON\r\n");
#endif
#ifdef BLC_NO_FAST_FOLDING
	mqom_print("    BLC_NO_FAST_FOLDING ON\r\n");
#else
	mqom_print("    BLC_NO_FAST_FOLDING OFF\r\n");
#endif
}

#else /* MQOM3_VARIANT_GUARD: CT build, nothing to compile here */
typedef int blc_ot_memopt_unused_in_ct_build;
#endif
