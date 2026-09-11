/* MQOM3_VARIANT_GUARD: this unit implements the CT variant only. Build systems
 * that compile every .c of a directory - SUPERCOP does, and so does any wildcard
 * Makefile - hand it to the compiler for OT instances too, where its parameters
 * simply do not exist. Rather than requiring every such build to filter its source
 * list, the unit reduces to a single typedef there. Not an empty file: ISO C forbids
 * an empty translation unit and -Wpedantic says so.
 * The in-tree Makefile still selects the right family, so nothing changes for it. */
#include "mqom3_parameters.h"
#if MQOM3_PARAM_OT_VARIANT != 1

/* The SeedExpand cache default is resolved once in blc_common.h, so that this
 * identifier has a single value across every unit of the binary. */
#include "blc_ct_default.h"
#include "ggm_tree_common.h"
#include "ggm_tree_small.h"
#include "benchmark.h"
#include "seed_commit.h"

#if defined(SUPERCOP)
#include "crypto_declassify.h"
#endif

#include "blc_common.h"
#include "blc_convert.h"
#include "domain_separation.h"

int CT_BLC_Commit_default(const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], CT_blc_key_default_t* key, field_ext_elt x0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt u1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;
	uint8_t delta[MQOM3_PARAM_SEED_SIZE];
	uint32_t e;
	/* SeedExpand cache (ECB variant: single-key cache since all 8 seeds share the same e) */
	SeedExpand_cache_ecb *cache_ecb = NULL;
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	/* Heap-allocated to avoid large stack frames */
	uint8_t (*node_e)[MQOM3_PARAM_SEED_SIZE] = NULL;
	uint8_t (*ls_com_e)[MQOM3_PARAM_DIGEST_SIZE] = NULL;
#endif
	uint8_t (*lseed)[MQOM3_PARAM_SEED_SIZE] = NULL;

	/* Compute delta */
	DeriveDelta(x, delta);

#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	memcpy(key->salt, salt, MQOM3_PARAM_SALT_SIZE);
	memcpy(key->delta, delta, MQOM3_PARAM_SEED_SIZE);
	memcpy(key->mseed, mseed, MQOM3_PARAM_SEED_SIZE);
#endif

	/* Define "node" and "ls_com" that point either to the BLC key or to heap buffers */
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	node_e = mqom_malloc((size_t)(MQOM3_PARAM_FULL_TREE_SIZE + 1) * MQOM3_PARAM_SEED_SIZE);
	ERR_NULL(node_e, err);
	ls_com_e = mqom_malloc((size_t)MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_DIGEST_SIZE);
	ERR_NULL(ls_com_e, err);
#endif
	uint8_t (*node[MQOM3_PARAM_TAU])[MQOM3_PARAM_SEED_SIZE];
	uint8_t (*ls_com[MQOM3_PARAM_TAU])[MQOM3_PARAM_DIGEST_SIZE];
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
		node[e] = node_e;
		ls_com[e] = ls_com_e;
#else
		node[e] = key->node[e];
		ls_com[e] = key->ls_com[e];
#endif
	}

	lseed = mqom_malloc((size_t)MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_SEED_SIZE);
	ERR_NULL(lseed, err);
	/* Allocate SeedExpand cache once; reused across all TAU executions */
	cache_ecb = init_SeedExpand_cache_ecb(MQOM3_PARAM_SEED_SIZE + PRG_BLC_SIZE);
	/* cache_ecb may be NULL (BLC_SEEDEXPAND_CACHE not set or alloc failed): all cache ops handle NULL */
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		__BENCHMARK_START__(BS_BLC_EXPAND_TREE);
		ret = SmallGGMTree_Expand(salt, mseed, delta, e, node[e], lseed);
		ERR(ret, err);
		__BENCHMARK_STOP__(BS_BLC_EXPAND_TREE);

		/* BLC.ConvertToLine (see spec) - Hash_7 (com1) batched after the loop */
		ret = BLC_ConvertToLine(salt, e, (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE]) lseed, x,
		                        ls_com[e], key->partial_delta_x[e],
		                        x0[e], u0[e], u1[e], cache_ecb);
		ERR(ret, err);
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
		/* No-KEEP: ls_com is a shared buffer overwritten next iteration; hash now */
		__BENCHMARK_START__(BS_BLC_XOF);
		ret = BLC_Hash_com1(com1[e], salt, e, (const uint8_t*) ls_com[e], key->partial_delta_x[e]);
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
			dxp[e] = (const uint8_t*) key->partial_delta_x[e];
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
	mqom_cleanse(delta, sizeof(delta));
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	mqom_cleanse(node_e, (size_t)(MQOM3_PARAM_FULL_TREE_SIZE + 1) * MQOM3_PARAM_SEED_SIZE);
	mqom_free(node_e, (size_t)(MQOM3_PARAM_FULL_TREE_SIZE + 1) * MQOM3_PARAM_SEED_SIZE);
	mqom_free(ls_com_e, (size_t)MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_DIGEST_SIZE);
#endif
	mqom_cleanse(lseed, (size_t)MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_SEED_SIZE);
	mqom_free(lseed, (size_t)MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_SEED_SIZE);
	return ret;
}

int CT_BLC_Open_default(const CT_blc_key_default_t* key, const uint16_t i_star[MQOM3_PARAM_TAU], const field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], uint8_t opening[MQOM3_PARAM_OPENING_SIZE]) {
	int ret = -1;
	int e;
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	seedcommit_ctx_t seedcommit_ctx = { 0 };
	uint8_t lseed[MQOM3_PARAM_SEED_SIZE];
	uint8_t tweaked_salt_0[MQOM3_PARAM_SALT_SIZE], tweaked_salt_1[MQOM3_PARAM_SALT_SIZE];
#endif

	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		uint8_t *chunk         = &opening[e * CT_CHUNK_SIZE];
		uint8_t *path_e        = chunk;
		uint8_t *ls_com_e      = chunk + CT_CHUNK_LSCOM_OFF;
		uint8_t *pdelta_e      = chunk + CT_CHUNK_PDELTA_OFF;
		uint8_t *alpha1_e      = chunk + CT_CHUNK_ALPHA1_OFF;
#if defined(SUPERCOP)
		/* XXX: NOTE: we explicitly declassify i_star[e] as it is public data but comes from a dataflow involving secret data
		 * through hashing */
		crypto_declassify(&i_star[e], sizeof(i_star[e]));
#endif
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
		ret = SmallGGMTree_ExpandPath(key->salt, key->mseed, key->delta, e, i_star[e], (uint8_t(*)[MQOM3_PARAM_SEED_SIZE]) path_e, lseed);
		ERR(ret, err);
		TweakSalt(key->salt, tweaked_salt_0, SALT_SEL_BLC_LEFT, (uint32_t)e);
		TweakSalt(key->salt, tweaked_salt_1, SALT_SEL_BLC_RIGHT, (uint32_t)e);
		ret = init_seedcommit(&seedcommit_ctx, tweaked_salt_0, tweaked_salt_1);
		ERR(ret, err);
		ret = SeedCommit(&seedcommit_ctx, lseed, ls_com_e);
		ERR(ret, err);
#else
		ret = SmallGGMTree_Open(key->node[e], i_star[e], (uint8_t(*)[MQOM3_PARAM_SEED_SIZE]) path_e);
		ERR(ret, err);
		memcpy(ls_com_e, key->ls_com[e][i_star[e]], MQOM3_PARAM_DIGEST_SIZE);
#endif

		memcpy(pdelta_e, key->partial_delta_x[e], CT_CHUNK_PDELTA_SIZE);
		field_ext_serialize(alpha1[e], MQOM3_PARAM_ETA, alpha1_e);
	}

	ret = 0;
err:
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	mqom_cleanse(lseed, sizeof(lseed));
	seedcommit_clean_ctx(&seedcommit_ctx);
#endif
	return ret;
}

int CT_BLC_Eval_default(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t opening[MQOM3_PARAM_OPENING_SIZE], const uint16_t i_star[MQOM3_PARAM_TAU], uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], field_ext_elt x_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;
	uint32_t e;
	/* Heap-allocated to avoid large stack frames (up to 256 KB + 4 MB for cat5-gf2 with XOF x8) */
	uint8_t (*lseed)[MQOM3_PARAM_SEED_SIZE] = NULL;
#if BLC_HASH7_BATCH_MAX > 1
	uint8_t (*ls_com_batch)[MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_DIGEST_SIZE] = NULL;
	const uint8_t *lsc_ptrs[BLC_HASH7_BATCH_MAX];
	const uint8_t *dxp_ptrs[BLC_HASH7_BATCH_MAX];
	uint32_t batch_start = 0, batch_count = 0;
#else
	uint8_t (*ls_com_scalar)[MQOM3_PARAM_DIGEST_SIZE] = NULL;
#endif

	/* SeedExpand cache (ECB variant: single-key cache since all 8 seeds share the same e).
	 * Allocated once and reused across all TAU executions; entries invalidated between iterations. */
	SeedExpand_cache_pub_ecb *cache_ecb = init_SeedExpand_cache_pub_ecb(MQOM3_PARAM_SEED_SIZE + PRG_BLC_SIZE);
	/* cache_ecb may be NULL (BLC_SEEDEXPAND_CACHE not set or alloc failed): all cache ops handle NULL */

	lseed = mqom_malloc((size_t)MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_SEED_SIZE);
	ERR_NULL(lseed, err);
#if BLC_HASH7_BATCH_MAX > 1
	ls_com_batch = mqom_malloc((size_t)BLC_HASH7_BATCH_MAX * MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_DIGEST_SIZE);
	ERR_NULL(ls_com_batch, err);
#else
	ls_com_scalar = mqom_malloc((size_t)MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_DIGEST_SIZE);
	ERR_NULL(ls_com_scalar, err);
#endif

	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		const uint8_t *chunk    = &opening[e * CT_CHUNK_SIZE];
		const uint8_t *path_e   = chunk;
		const uint8_t *lscom_e  = chunk + CT_CHUNK_LSCOM_OFF;
		const uint8_t *pdelta_e = chunk + CT_CHUNK_PDELTA_OFF;
#if BLC_HASH7_BATCH_MAX > 1
		uint8_t (*ls_com_slot)[MQOM3_PARAM_DIGEST_SIZE] = ls_com_batch[batch_count];
#else
		uint8_t (*ls_com_slot)[MQOM3_PARAM_DIGEST_SIZE] = ls_com_scalar;
#endif

		ret = SmallGGMTree_PartiallyExpand(salt, (const uint8_t(*)[MQOM3_PARAM_SEED_SIZE]) path_e, e, i_star[e], lseed);
		ERR(ret, err);

		/* BLC.ConvertToLineEval (see spec) - outputs into ls_com_slot; Hash_7 batched below */
		ret = BLC_ConvertToLineEval(salt, e, (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE]) lseed,
		                            i_star[e], lscom_e, pdelta_e,
		                            x_eval[e], u_eval[e], ls_com_slot, cache_ecb);
		ERR(ret, err);

		/* Invalidate cache entries for next execution (tweaked salts change with e) */
		reset_SeedExpand_cache_pub_ecb_all(cache_ecb);

#if BLC_HASH7_BATCH_MAX > 1
		lsc_ptrs[batch_count] = (const uint8_t*) ls_com_slot;
		dxp_ptrs[batch_count] = pdelta_e;
		batch_count++;
		if ((batch_count == BLC_HASH7_BATCH_MAX) || (e == MQOM3_PARAM_TAU - 1)) {
			ret = BLC_Hash_com1_batch(batch_start, batch_count, com1 + batch_start, salt, lsc_ptrs, dxp_ptrs);
			ERR(ret, err);
			batch_start = e + 1;
			batch_count = 0;
		}
#else
		ret = BLC_Hash_com1(com1[e], salt, e, (const uint8_t*) ls_com_slot, pdelta_e);
		ERR(ret, err);
#endif
	}

	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		field_ext_parse(&opening[e * CT_CHUNK_SIZE + CT_CHUNK_ALPHA1_OFF], MQOM3_PARAM_ETA, alpha1[e]);
	}

	ret = 0;
err:
	destroy_SeedExpand_cache_pub_ecb(cache_ecb);
	mqom_free(lseed, (size_t)MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_SEED_SIZE);
#if BLC_HASH7_BATCH_MAX > 1
	mqom_free(ls_com_batch, (size_t)BLC_HASH7_BATCH_MAX * MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_DIGEST_SIZE);
#else
	mqom_free(ls_com_scalar, (size_t)MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_DIGEST_SIZE);
#endif
	return ret;
}

void CT_BLC_PrintConfig_default(void) {
	mqom_print("  BLC: default CT\r\n");

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

	// GGM Tree
	mqom_print("    GGMTREE_NB_ENC_CTX_IN_MEMORY %d\r\n", GGMTREE_NB_ENC_CTX_IN_MEMORY);
}

#else /* MQOM3_VARIANT_GUARD: OT build, nothing to compile here */
typedef int blc_ct_default_unused_in_ot_build;
#endif
