/* MQOM3_VARIANT_GUARD: this unit implements the OT variant only. Build systems
 * that compile every .c of a directory - SUPERCOP does, and so does any wildcard
 * Makefile - hand it to the compiler for CT instances too, where its parameters
 * simply do not exist. Rather than requiring every such build to filter its source
 * list, the unit reduces to a single typedef there. Not an empty file: ISO C forbids
 * an empty translation unit and -Wpedantic says so.
 * The in-tree Makefile still selects the right family, so nothing changes for it. */
#include "mqom3_parameters.h"
#if MQOM3_PARAM_OT_VARIANT == 1

/* The SeedExpand cache default is resolved once in blc_common.h, so that this
 * identifier has a single value across every unit of the binary. */
#include "blc_ot_default.h"
#include "blc_common.h"
#include "blc_convert.h"
#include "benchmark.h"
#include "seed_commit.h"
#include "domain_separation.h"
#if defined(SUPERCOP)
#include "crypto_declassify.h"
#endif

int OT_BLC_Commit_default(const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], OT_blc_key_default_t* key, field_ext_elt x0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt u1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;
	uint32_t e, i;
	/* SeedExpand cache per execution */
	SeedExpand_cache_ecb *cache_ecb = NULL;

	/* Heap-allocated to avoid large stack frames, as in the sibling functions
	 * below: at NB_EVALS = 8192 these two arrays alone are 128 KB and 256 KB.
	 * Declared here (before the first ERR jump) so the err path can free them. */
	uint8_t (*lseed_e)[MQOM3_PARAM_SEED_SIZE] = NULL;
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	uint8_t (*ls_com_e)[MQOM3_PARAM_DIGEST_SIZE] = NULL;
#endif

	/* lseed_all[tau*i + e] = leaf seed for execution e, eval index i */
	uint8_t (*lseed_all)[MQOM3_PARAM_SEED_SIZE] = mqom_malloc((size_t)(MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS) * MQOM3_PARAM_SEED_SIZE);
	ERR_NULL(lseed_all, err);

#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	/* Save mseed/salt in the key for later re-expansion in BLC_Open */
	memcpy(key->mseed, mseed, MQOM3_PARAM_SEED_SIZE);
	memcpy(key->salt, salt, MQOM3_PARAM_SALT_SIZE);
#endif

	/* Define "ls_com" pointers: KEEP uses per-execution key storage; no-KEEP reuses one slot. */
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	ls_com_e = mqom_malloc((size_t)MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_DIGEST_SIZE);
	ERR_NULL(ls_com_e, err);
	uint8_t (*ls_com[MQOM3_PARAM_TAU])[MQOM3_PARAM_DIGEST_SIZE];
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		ls_com[e] = ls_com_e;
	}
#else
	uint8_t (*ls_com[MQOM3_PARAM_TAU])[MQOM3_PARAM_DIGEST_SIZE];
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		ls_com[e] = key->ls_com[e];
	}
#endif

	__BENCHMARK_START__(BS_BLC_EXPAND_TREE);
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	ret = LargeGGMTree_ExpandLeavesOnly(salt, mseed, lseed_all);
#else
	ret = LargeGGMTree_Expand(salt, mseed, key->node, lseed_all);
#endif
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_EXPAND_TREE);

	/* Leaf seed buffer for one execution: lseed_e[i] = lseed_all[tau*i + e] */
	lseed_e = mqom_malloc((size_t)MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_SEED_SIZE);
	ERR_NULL(lseed_e, err);

	/* Allocate SeedExpand cache once; reused across all TAU executions */
	cache_ecb = init_SeedExpand_cache_ecb(MQOM3_PARAM_SEED_SIZE + PRG_BLC_SIZE);
	/* cache_ecb may be NULL (BLC_SEEDEXPAND_CACHE not set or alloc failed): all cache ops handle NULL */
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		/* Extract leaf seeds for execution e (column-major: logical leaf tau*i + e) */
		for (i = 0; i < MQOM3_PARAM_NB_EVALS; i++) {
			memcpy(lseed_e[i], lseed_all[(MQOM3_PARAM_TAU * i) + e], MQOM3_PARAM_SEED_SIZE);
		}

		/* BLC.ConvertToLine (see spec): commit seeds, fold - Hash_7 (com1) batched after loop */
		ret = BLC_ConvertToLine(salt, e, (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE]) lseed_e, x,
		                        ls_com[e], key->delta_x[e],
		                        x0[e], u0[e], u1[e], cache_ecb);
		ERR(ret, err);
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
		/* No-KEEP: ls_com is a shared buffer overwritten next iteration; hash now */
		__BENCHMARK_START__(BS_BLC_XOF);
		ret = BLC_Hash_com1(com1[e], salt, e, (const uint8_t*) ls_com[e], key->delta_x[e]);
		__BENCHMARK_STOP__(BS_BLC_XOF);
		ERR(ret, err);
#endif

		/* Invalidate cache entries for next execution (tweaked salts change with e) */
		reset_SeedExpand_cache_ecb_all(cache_ecb);
	}

#ifdef BLC_KEEP_ALL_TREES_IN_MEMORY
	/* Batch Hash_7 for all TAU executions using xof_x8 / xof_x4 / scalar */
	{
		const uint8_t *lsc[MQOM3_PARAM_TAU];
		const uint8_t *dxp[MQOM3_PARAM_TAU];
		for (e = 0; e < MQOM3_PARAM_TAU; e++) {
			lsc[e] = (const uint8_t*) key->ls_com[e];
			dxp[e] = (const uint8_t*) key->delta_x[e];
		}
		__BENCHMARK_START__(BS_BLC_XOF);
		ret = BLC_Hash_com1_batch(0, MQOM3_PARAM_TAU, com1, salt, lsc, dxp);
		__BENCHMARK_STOP__(BS_BLC_XOF);
		ERR(ret, err);
	}
#endif

	ret = 0;
err:
	destroy_SeedExpand_cache_ecb(cache_ecb);
	mqom_cleanse(lseed_all, (size_t)(MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS) * MQOM3_PARAM_SEED_SIZE);
	mqom_free(lseed_all, (size_t)(MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS) * MQOM3_PARAM_SEED_SIZE);
	mqom_cleanse(lseed_e, (size_t)MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_SEED_SIZE);
	mqom_free(lseed_e, (size_t)MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_SEED_SIZE);
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	mqom_free(ls_com_e, (size_t)MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_DIGEST_SIZE);
#endif
	return ret;
}

int OT_BLC_Open_default(const OT_blc_key_default_t* key, const uint16_t i_star[MQOM3_PARAM_TAU], const field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], uint8_t opening[MQOM3_PARAM_OPENING_SIZE]) {
	int ret = -1;
	int e;
	uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU];
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	/* hidden_lseeds needed only in no-keep path: recomputed from ExpandPath to commit */
	uint8_t hidden_lseeds[MQOM3_PARAM_TAU][MQOM3_PARAM_SEED_SIZE];
	seedcommit_ctx_t seedcommit_ctx = { 0 };
	uint8_t tweaked_salt_0[MQOM3_PARAM_SALT_SIZE], tweaked_salt_1[MQOM3_PARAM_SALT_SIZE];
#endif

	/* Opening format: path | ls_com_hidden | delta_x | alpha1_section */
	uint8_t *path           = &opening[0];
	uint8_t *ls_com_hidden  = path + ((size_t)MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE);
	uint8_t *delta_x_out    = ls_com_hidden + ((size_t)MQOM3_PARAM_TAU * MQOM3_PARAM_DIGEST_SIZE);
	uint8_t *alpha1_section = delta_x_out + ((size_t)MQOM3_PARAM_TAU * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N));

	/* hidden_leaf_idxs[e] = tau*i_star[e] + e (column-major BLC leaf indexing) */
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
#if defined(SUPERCOP)
		/* XXX: NOTE: we explicitly declassify i_star[e] as it is public data but comes from a dataflow involving secret data
		 * through hashing */
		crypto_declassify(&i_star[e], sizeof(i_star[e]));
#endif
		hidden_leaf_idxs[e] = (uint32_t)(MQOM3_PARAM_TAU * (uint32_t)i_star[e] + (uint32_t)e);
	}

#ifdef BLC_KEEP_ALL_TREES_IN_MEMORY
	/* Fast path: open from stored node array, copy precomputed hidden leaf commitments.
	 * hidden_lseeds not needed here: commitments come from key->ls_com (precomputed during Commit). */
	ret = LargeGGMTree_Open(key->node, hidden_leaf_idxs, path);
	ERR(ret, err);
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		memcpy(&ls_com_hidden[(size_t)e * MQOM3_PARAM_DIGEST_SIZE], key->ls_com[e][i_star[e]], MQOM3_PARAM_DIGEST_SIZE);
	}
#else
	/* Memory-efficient path: re-expand to get path, recompute hidden leaf commitments */
	ret = LargeGGMTree_ExpandPath(key->salt, key->mseed, hidden_leaf_idxs, path, hidden_lseeds);
	ERR(ret, err);
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		TweakSalt(key->salt, tweaked_salt_0, SALT_SEL_BLC_LEFT, (uint32_t)e);
		TweakSalt(key->salt, tweaked_salt_1, SALT_SEL_BLC_RIGHT, (uint32_t)e);
		ret = init_seedcommit(&seedcommit_ctx, tweaked_salt_0, tweaked_salt_1);
		ERR(ret, err);
		ret = SeedCommit(&seedcommit_ctx, hidden_lseeds[e], &ls_com_hidden[(size_t)e * MQOM3_PARAM_DIGEST_SIZE]);
		ERR(ret, err);
	}
#endif

	/* Serialize delta_x and alpha1 */
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		memcpy(&delta_x_out[(size_t)e * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)], key->delta_x[e], BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N));
		field_ext_serialize(alpha1[e], MQOM3_PARAM_ETA, &alpha1_section[(size_t)e * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU)]);
	}

	ret = 0;
err:
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	mqom_cleanse(hidden_lseeds, sizeof(hidden_lseeds));
	seedcommit_clean_ctx(&seedcommit_ctx);
#endif
	return ret;
}

int OT_BLC_Eval_default(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t opening[MQOM3_PARAM_OPENING_SIZE], const uint16_t i_star[MQOM3_PARAM_TAU], uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], field_ext_elt x_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;
	uint32_t e, i;
	uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU];
	/* Heap-allocated to avoid large stack frames */
	uint8_t (*lseed_e)[MQOM3_PARAM_SEED_SIZE] = NULL;
#if BLC_HASH7_BATCH_MAX > 1
	uint8_t (*ls_com_batch)[MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_DIGEST_SIZE] = NULL;
	const uint8_t *lsc_ptrs[BLC_HASH7_BATCH_MAX];
	const uint8_t *dxp_ptrs[BLC_HASH7_BATCH_MAX];
	uint32_t batch_start = 0, batch_count = 0;
#else
	uint8_t (*ls_com_scalar)[MQOM3_PARAM_DIGEST_SIZE] = NULL;
#endif
	/* SeedExpand cache for verifier; allocated once, reused across all TAU executions */
	SeedExpand_cache_pub_ecb *cache_ecb = init_SeedExpand_cache_pub_ecb(MQOM3_PARAM_SEED_SIZE + PRG_BLC_SIZE);
	/* cache_ecb may be NULL (BLC_SEEDEXPAND_CACHE not set or alloc failed): all cache ops handle NULL */

	/* Parse opening format: path | ls_com_hidden | delta_x | alpha1_section */
	const uint8_t *path           = &opening[0];
	const uint8_t *ls_com_hidden  = path + ((size_t)MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE);
	const uint8_t *delta_x_in     = ls_com_hidden + ((size_t)MQOM3_PARAM_TAU * MQOM3_PARAM_DIGEST_SIZE);
	const uint8_t *alpha1_section = delta_x_in + ((size_t)MQOM3_PARAM_TAU * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N));

	/* All leaf seeds: lseed_all[tau*i + e] (hidden slots remain zero after PartiallyExpand) */
	uint8_t (*lseed_all)[MQOM3_PARAM_SEED_SIZE] = mqom_malloc((size_t)(MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS) * MQOM3_PARAM_SEED_SIZE);
	ERR_NULL(lseed_all, err);

	lseed_e = mqom_malloc((size_t)MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_SEED_SIZE);
	ERR_NULL(lseed_e, err);
#if BLC_HASH7_BATCH_MAX > 1
	ls_com_batch = mqom_malloc((size_t)BLC_HASH7_BATCH_MAX * MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_DIGEST_SIZE);
	ERR_NULL(ls_com_batch, err);
#else
	ls_com_scalar = mqom_malloc((size_t)MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_DIGEST_SIZE);
	ERR_NULL(ls_com_scalar, err);
#endif

	/* hidden_leaf_idxs[e] = tau*i_star[e] + e */
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		hidden_leaf_idxs[e] = (uint32_t)(MQOM3_PARAM_TAU * (uint32_t)i_star[e] + (uint32_t)e);
	}

	/* Partially expand tree: all leaves filled except hidden ones (set to zero) */
	ret = LargeGGMTree_PartiallyExpand(salt, path, hidden_leaf_idxs, lseed_all);
	ERR(ret, err);

	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		/* Extract leaf seeds for execution e (hidden leaf lseed_e[i_star[e]] stays zero) */
		for (i = 0; i < MQOM3_PARAM_NB_EVALS; i++) {
			memcpy(lseed_e[i], lseed_all[(MQOM3_PARAM_TAU * i) + e], MQOM3_PARAM_SEED_SIZE);
		}
#if BLC_HASH7_BATCH_MAX > 1
		uint8_t (*ls_com_slot)[MQOM3_PARAM_DIGEST_SIZE] = ls_com_batch[batch_count];
#else
		uint8_t (*ls_com_slot)[MQOM3_PARAM_DIGEST_SIZE] = ls_com_scalar;
#endif

		/* BLC.ConvertToLineEval (see spec) - outputs into ls_com_slot; Hash_7 batched below */
		ret = BLC_ConvertToLineEval(salt, e, (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE]) lseed_e,
		                            i_star[e],
		                            &ls_com_hidden[(size_t)e * MQOM3_PARAM_DIGEST_SIZE],
		                            &delta_x_in[(size_t)e * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)],
		                            x_eval[e], u_eval[e], ls_com_slot, cache_ecb);
		ERR(ret, err);

		/* Invalidate cache entries for next execution (tweaked salts change with e) */
		reset_SeedExpand_cache_pub_ecb_all(cache_ecb);

#if BLC_HASH7_BATCH_MAX > 1
		lsc_ptrs[batch_count] = (const uint8_t*) ls_com_slot;
		dxp_ptrs[batch_count] = &delta_x_in[(size_t)e * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)];
		batch_count++;
		if ((batch_count == BLC_HASH7_BATCH_MAX) || (e == MQOM3_PARAM_TAU - 1)) {
			ret = BLC_Hash_com1_batch(batch_start, batch_count, com1 + batch_start, salt, lsc_ptrs, dxp_ptrs);
			ERR(ret, err);
			batch_start = e + 1;
			batch_count = 0;
		}
#else
		ret = BLC_Hash_com1(com1[e], salt, e, (const uint8_t*) ls_com_slot,
		                   &delta_x_in[(size_t)e * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)]);
		ERR(ret, err);
#endif
	}

	/* Parse alpha1 sections */
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		field_ext_parse(&alpha1_section[(size_t)e * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU)], MQOM3_PARAM_ETA, alpha1[e]);
	}

	ret = 0;
err:
	destroy_SeedExpand_cache_pub_ecb(cache_ecb);
	mqom_free(lseed_all, (size_t)(MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS) * MQOM3_PARAM_SEED_SIZE);
	mqom_free(lseed_e, (size_t)MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_SEED_SIZE);
#if BLC_HASH7_BATCH_MAX > 1
	mqom_free(ls_com_batch, (size_t)BLC_HASH7_BATCH_MAX * MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_DIGEST_SIZE);
#else
	mqom_free(ls_com_scalar, (size_t)MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_DIGEST_SIZE);
#endif
	return ret;
}

void OT_BLC_PrintConfig_default(void) {
	mqom_print("  BLC: default OT\r\n");

#ifdef BLC_SEEDEXPAND_CACHE
	mqom_print("    SeedExpand cache ON\r\n");
#else
	mqom_print("    SeedExpand cache OFF\r\n");
#endif

#ifdef BLC_KEEP_ALL_TREES_IN_MEMORY
	mqom_print("    BLC_KEEP_ALL_TREES_IN_MEMORY ON\r\n");
#else
	mqom_print("    BLC_KEEP_ALL_TREES_IN_MEMORY OFF (memory-efficient)\r\n");
#endif

#ifdef LARGE_TREE_BFS
#ifdef LARGE_TREE_BFS_X8
	mqom_print("    Large GGM expand: BFS x8\r\n");
#else
	mqom_print("    Large GGM expand: BFS x4\r\n");
#endif
#else
	mqom_print("    Large GGM expand: DFS (reference)\r\n");
#endif
}

#else /* MQOM3_VARIANT_GUARD: CT build, nothing to compile here */
typedef int blc_ot_default_unused_in_ct_build;
#endif
