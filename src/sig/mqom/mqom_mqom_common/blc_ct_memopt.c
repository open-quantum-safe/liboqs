/* MQOM3_VARIANT_GUARD: this unit implements the CT variant only. Build systems
 * that compile every .c of a directory - SUPERCOP does, and so does any wildcard
 * Makefile - hand it to the compiler for OT instances too, where its parameters
 * simply do not exist. Rather than requiring every such build to filter its source
 * list, the unit reduces to a single typedef there. Not an empty file: ISO C forbids
 * an empty translation unit and -Wpedantic says so.
 * The in-tree Makefile still selects the right family, so nothing changes for it. */
#include "mqom3_parameters.h"
#if MQOM3_PARAM_OT_VARIANT != 1

#include "blc_ct_memopt.h"
#include "blc_ct_common.h"
#include "ggm_tree_common.h"
#include "ggm_tree_small.h"
#include "ggm_tree_small_incr.h"
#include "ggm_tree_small_incr_batch.h"
#include "benchmark.h"
#include "seed_commit.h"

#if defined(SUPERCOP)
#include "crypto_declassify.h"
#endif

#include "domain_separation.h"
#include "blc_memopt.h"
#include "blc_memopt_folding.h"
#include "blc_memopt_seedcommit.h"

#ifndef SMALL_GGM_TREE_NO_BATCHING
#if SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES % BLC_NB_LEAF_SEEDS_IN_PARALLEL != 0
#error BLC_NB_LEAF_SEEDS_IN_PARALLEL should divide SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES.
#endif
#endif

int CT_BLC_Commit_memopt_partial(uint32_t e, const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], const uint8_t delta[MQOM3_PARAM_SEED_SIZE], uint8_t com[MQOM3_PARAM_DIGEST_SIZE], uint8_t partial_delta_x[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)-MQOM3_PARAM_SEED_SIZE], field_ext_elt x0[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u0[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt u1[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;
	uint32_t i, i_;

#ifdef SMALL_GGM_TREE_NO_BATCHING
	uint8_t lseeds[BLC_NB_LEAF_SEEDS_IN_PARALLEL][MQOM3_PARAM_SEED_SIZE];
	small_ggmtree_ctx_t DECL_VAR(ggm_tree);
#else
	uint8_t lseeds[SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES][MQOM3_PARAM_SEED_SIZE];
	small_ggmtree_ctx_batch_t DECL_VAR(ggm_tree);
#endif
	/* Zeroed: the early ERR() jumps below reach the cleanup, which walks
	 * folding->enc_ctx[]. On backends whose contexts hold allocated state
	 * (libOQS, RIJNDAEL_EXTERNAL) cleaning an indeterminate context is a free()
	 * on a garbage pointer. The OT counterpart memsets for the same reason. */
	folding_sign_t folding = { 0 };
	seedcommit_sign_ctx_t seedcommit_ctx = { 0 };

	// Initialize the GGM tree
	__BENCHMARK_START__(BS_BLC_EXPAND_TREE);
#ifdef SMALL_GGM_TREE_NO_BATCHING
	ret = SmallGGMTree_InitIncrementalExpansion(&ggm_tree, salt, mseed, delta, e);
#else
	ret = SmallGGMTree_InitIncrementalExpansion_batch(&ggm_tree, salt, mseed, delta, e);
#endif
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_EXPAND_TREE);

	/* Incremental implementation of BLC.ConvertToLine (see spec), split into
	 * two parallel streams: seedcommit (XOF) and folding (SeedExpand + arithmetic) */
	// Initialize the hash context
	ret = init_seedcommit_sign(&seedcommit_ctx, salt, e);
	ERR(ret, err);

	ret = InitializeFolding_sign(&folding, salt, e, x0, u0);
	ERR(ret, err);
#ifdef SMALL_GGM_TREE_NO_BATCHING
	for (i = 0; i < MQOM3_PARAM_NB_EVALS; i+= BLC_NB_LEAF_SEEDS_IN_PARALLEL) {

		// Derive the next leaf seeds
		__BENCHMARK_START__(BS_BLC_EXPAND_TREE);
		for (i_ = 0; i_<BLC_NB_LEAF_SEEDS_IN_PARALLEL; i_++) {
			ret = SmallGGMTree_GetNextLeaf(&ggm_tree, lseeds[i_]);
			ERR(ret, err);
		}
		__BENCHMARK_STOP__(BS_BLC_EXPAND_TREE);

		// Compute the individual commitments for all the seed leafs,
		// and incrementally hash them.
		ret = SeedCommitThenAbsorb_sign(&seedcommit_ctx, lseeds);
		ERR(ret, err);

		// Expand each seed and accumulate the expanded tapes
		ret = SeedExpandThenAccumulate_sign(&folding, i, lseeds);
		ERR(ret, err);
	}
#else
	for (i = 0; i < MQOM3_PARAM_NB_EVALS; i+= SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES) {
		// Derive the next leaf seeds
		__BENCHMARK_START__(BS_BLC_EXPAND_TREE);
		ret = SmallGGMTree_GetNextLeafs_batch(&ggm_tree, lseeds);
		ERR(ret, err);
		__BENCHMARK_STOP__(BS_BLC_EXPAND_TREE);

		for (i_ = 0; i_ < (SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES/BLC_NB_LEAF_SEEDS_IN_PARALLEL); i_++) {
			// Compute the individual commitments for all the seed leafs,
			// and incrementally hash them.
			ret = SeedCommitThenAbsorb_sign(&seedcommit_ctx, (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE])&lseeds[i_*BLC_NB_LEAF_SEEDS_IN_PARALLEL]);
			ERR(ret, err);

			// Expand each seed and accumulate the expanded tapes
			ret = SeedExpandThenAccumulate_sign(&folding, i + i_*BLC_NB_LEAF_SEEDS_IN_PARALLEL, (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE])&lseeds[i_*BLC_NB_LEAF_SEEDS_IN_PARALLEL]);
			ERR(ret, err);
		}
	}
#endif

	// Finalize the folding to get the committed polynomials
	__BENCHMARK_START__(BS_BLC_ARITH);
	CT_FinalizeFolding_sign(&folding, x, partial_delta_x, x0, u0, u1);
	__BENCHMARK_STOP__(BS_BLC_ARITH);

	// Get the global commitment digest (v3: include partial_delta_x in Hash_7)
	__BENCHMARK_START__(BS_BLC_XOF);
	ret = xof_update(&seedcommit_ctx.xof_ctx, partial_delta_x, BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) - MQOM3_PARAM_SEED_SIZE);
	ERR(ret, err);
	ret = xof_squeeze(&seedcommit_ctx.xof_ctx, com, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_XOF);

	ret = 0;
err:
	seedcommit_sign_clean_ctx(&seedcommit_ctx);
#ifdef SMALL_GGM_TREE_NO_BATCHING
	small_ggmtree_ctx_t_clean(&ggm_tree);
#else
	small_ggmtree_ctx_batch_t_clean(&ggm_tree);
#endif
	folding_sign_clean_ctx(&folding);
	/* lseeds holds the leaf seeds in the clear, batch after batch: the tree
	 * context above wipes its own path, and SeedCommitThenAbsorb_sign wipes its
	 * scratch, but this buffer sits between the two and was left behind. */
	mqom_cleanse(lseeds, sizeof(lseeds));
	return ret;
}

int CT_BLC_Commit_memopt(const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], CT_blc_key_memopt_t* key, field_ext_elt x0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt u1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;
	uint32_t e;

	/* Compute delta */
	DeriveDelta(x, key->delta);

	memcpy(key->mseed, mseed, MQOM3_PARAM_SEED_SIZE);
	memcpy(key->salt, salt, MQOM3_PARAM_SALT_SIZE);

	/* v3: com1[e] = Hash_7(ls_com[e], partial_delta_x[e]) */
	uint8_t hash_ls_com[MQOM3_PARAM_DIGEST_SIZE];
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		ret = CT_BLC_Commit_memopt_partial(e, key->mseed, salt, x, key->delta, hash_ls_com, key->partial_delta_x[e], x0[e], u0[e], u1[e]);
		ERR(ret, err);
		memcpy(com1[e], hash_ls_com, MQOM3_PARAM_DIGEST_SIZE);
	}

	ret = 0;
err:
	return ret;
}

int CT_BLC_Open_memopt(const CT_blc_key_memopt_t* key, const uint16_t i_star[MQOM3_PARAM_TAU], const field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], uint8_t opening[MQOM3_PARAM_OPENING_SIZE]) {
	int ret = -1;
	int e;
	seedcommit_ctx_t seedcommit_ctx = { 0 };
	uint8_t lseed[MQOM3_PARAM_SEED_SIZE];
	uint8_t tweaked_salt_0[MQOM3_PARAM_SALT_SIZE], tweaked_salt_1[MQOM3_PARAM_SALT_SIZE];

	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		uint8_t *chunk    = &opening[e * CT_CHUNK_SIZE];
		uint8_t *path_e   = chunk;
		uint8_t *lscom_e  = chunk + CT_CHUNK_LSCOM_OFF;
		uint8_t *pdelta_e = chunk + CT_CHUNK_PDELTA_OFF;
		uint8_t *alpha1_e = chunk + CT_CHUNK_ALPHA1_OFF;
#if defined(SUPERCOP)
		/* XXX: NOTE: we explicitly declassify i_star[e] as it is public data but comes from a dataflow involving secret data
		 * through hashing */
		crypto_declassify(&i_star[e], sizeof(i_star[e]));
#endif
		ret = SmallGGMTree_ExpandPath(key->salt, key->mseed, key->delta, e, i_star[e], (uint8_t(*)[MQOM3_PARAM_SEED_SIZE]) path_e, lseed);
		ERR(ret, err);
		TweakSalt(key->salt, tweaked_salt_0, SALT_SEL_BLC_LEFT, (uint32_t)e);
		TweakSalt(key->salt, tweaked_salt_1, SALT_SEL_BLC_RIGHT, (uint32_t)e);
		ret = init_seedcommit(&seedcommit_ctx, tweaked_salt_0, tweaked_salt_1);
		ERR(ret, err);
		ret = SeedCommit(&seedcommit_ctx, lseed, lscom_e);
		ERR(ret, err);

		memcpy(pdelta_e, key->partial_delta_x[e], CT_CHUNK_PDELTA_SIZE);
		field_ext_serialize(alpha1[e], MQOM3_PARAM_ETA, alpha1_e);
	}

	ret = 0;
err:
	mqom_cleanse(lseed, sizeof(lseed));
	seedcommit_clean_ctx(&seedcommit_ctx);
	return ret;
}

int CT_BLC_Eval_memopt_partial(uint32_t e, const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t path[MQOM3_PARAM_SEED_SIZE*MQOM3_PARAM_NB_EVALS_LOG], const uint8_t out_ls_com[MQOM3_PARAM_DIGEST_SIZE], const uint8_t partial_delta_x[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)-MQOM3_PARAM_SEED_SIZE], uint16_t i_star, uint8_t com[MQOM3_PARAM_DIGEST_SIZE], field_ext_elt x_eval[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u_eval[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;
	uint32_t i, i_;

#ifdef SMALL_GGM_TREE_NO_BATCHING
	uint8_t lseeds[BLC_NB_LEAF_SEEDS_IN_PARALLEL][MQOM3_PARAM_SEED_SIZE];
	small_ggmtree_ctx_partial_t DECL_VAR(ggm_tree);
#else
	uint8_t lseeds[SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES][MQOM3_PARAM_SEED_SIZE];
	small_ggmtree_ctx_partial_batch_t DECL_VAR(ggm_tree);
#endif
	folding_verify_t folding = { 0 };
	seedcommit_verify_ctx_t seedcommit_ctx = { 0 };


	// Initialize the GGM tree
#ifdef SMALL_GGM_TREE_NO_BATCHING
	ret = SmallGGMTree_InitIncrementalPartialExpansion(&ggm_tree, salt, (const uint8_t(*)[MQOM3_PARAM_SEED_SIZE]) path, e, i_star);
#else
	ret = SmallGGMTree_InitIncrementalPartialExpansion_batch(&ggm_tree, salt, (const uint8_t(*)[MQOM3_PARAM_SEED_SIZE]) path, e, i_star);
#endif
	ERR(ret, err);

	/* Incremental implementation of BLC.ConvertToLineEval (see spec), split into
	 * two parallel streams: seedcommit (XOF) and folding (SeedExpand + arithmetic) */
	// Initialize the hash context
	ret = init_seedcommit_verify(&seedcommit_ctx, salt, e, i_star, out_ls_com);
	ERR(ret, err);

	ret = InitializeFolding_verify(&folding, salt, e, x_eval, u_eval);
	ERR(ret, err);
#ifdef SMALL_GGM_TREE_NO_BATCHING
	for (i = 0; i < MQOM3_PARAM_NB_EVALS; i+= BLC_NB_LEAF_SEEDS_IN_PARALLEL) {
		for (i_ = 0; i_<BLC_NB_LEAF_SEEDS_IN_PARALLEL; i_++) {
			ret = SmallGGMTree_GetNextLeafPartial(&ggm_tree, lseeds[i_]);
			ERR(ret, err);
		}

		ret = SeedCommitThenAbsorb_verify(&seedcommit_ctx, i, lseeds);
		ERR(ret, err);
		ret = SeedExpandThenAccumulate_verify(&folding, i, lseeds, i_star);
		ERR(ret, err);
	}
#else
	for (i = 0; i < MQOM3_PARAM_NB_EVALS; i+= SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES) {
		ret = SmallGGMTree_GetNextLeafsPartial_batch(&ggm_tree, lseeds);
		ERR(ret, err);

		for (i_ = 0; i_ < (SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES/BLC_NB_LEAF_SEEDS_IN_PARALLEL); i_++) {
			ret = SeedCommitThenAbsorb_verify(&seedcommit_ctx, i + i_*BLC_NB_LEAF_SEEDS_IN_PARALLEL, (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE])&lseeds[i_*BLC_NB_LEAF_SEEDS_IN_PARALLEL]);
			ERR(ret, err);
			ret = SeedExpandThenAccumulate_verify(&folding, i + i_*BLC_NB_LEAF_SEEDS_IN_PARALLEL, (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE])&lseeds[i_*BLC_NB_LEAF_SEEDS_IN_PARALLEL], i_star);
			ERR(ret, err);
		}
	}
#endif
	CT_FinalizeFolding_verify(&folding, i_star, partial_delta_x, x_eval, u_eval);

	ret = xof_update(&seedcommit_ctx.xof_ctx, partial_delta_x, BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) - MQOM3_PARAM_SEED_SIZE);
	ERR(ret, err);
	ret = xof_squeeze(&seedcommit_ctx.xof_ctx, com, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	ret = 0;
err:
	seedcommit_verify_clean_ctx(&seedcommit_ctx);
#ifdef SMALL_GGM_TREE_NO_BATCHING
	small_ggmtree_ctx_partial_t_clean(&ggm_tree);
#else
	small_ggmtree_ctx_partial_batch_t_clean(&ggm_tree);
#endif
	folding_verify_clean_ctx(&folding);
	return ret;
}

int CT_BLC_Eval_memopt(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t opening[MQOM3_PARAM_OPENING_SIZE], const uint16_t i_star[MQOM3_PARAM_TAU], uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], field_ext_elt x_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;
	uint32_t e;

	/* v3: com1[e] = Hash_7(ls_com_e[e], partial_delta_x[e])  - output, not input */
	uint8_t hash_ls_com[MQOM3_PARAM_DIGEST_SIZE];
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		const uint8_t *chunk_e  = &opening[e * CT_CHUNK_SIZE];
		ret = CT_BLC_Eval_memopt_partial(e, salt, chunk_e, chunk_e + CT_CHUNK_LSCOM_OFF, chunk_e + CT_CHUNK_PDELTA_OFF, i_star[e], hash_ls_com, x_eval[e], u_eval[e]);
		ERR(ret, err);
		memcpy(com1[e], hash_ls_com, MQOM3_PARAM_DIGEST_SIZE);
	}

	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		field_ext_parse(&opening[e * CT_CHUNK_SIZE + CT_CHUNK_ALPHA1_OFF], MQOM3_PARAM_ETA, alpha1[e]);
	}

	ret = 0;
err:
	return ret;
}

void CT_BLC_PrintConfig_memopt(void) {
	mqom_print("  BLC: memopt\r\n");

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

	// GGM Tree
#ifndef SMALL_GGM_TREE_NO_BATCHING
	mqom_print("    SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES_LOG %d\r\n", SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES_LOG);
#endif

	mqom_print("    GGMTREE_NB_ENC_CTX_IN_MEMORY %d\r\n", GGMTREE_NB_ENC_CTX_IN_MEMORY);
}

#else /* MQOM3_VARIANT_GUARD: OT build, nothing to compile here */
typedef int blc_ct_memopt_unused_in_ot_build;
#endif
