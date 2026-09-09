/* MQOM3_VARIANT_GUARD: this unit implements the OT variant only. Build systems
 * that compile every .c of a directory - SUPERCOP does, and so does any wildcard
 * Makefile - hand it to the compiler for CT instances too, where its parameters
 * simply do not exist. Rather than requiring every such build to filter its source
 * list, the unit reduces to a single typedef there. Not an empty file: ISO C forbids
 * an empty translation unit and -Wpedantic says so.
 * The in-tree Makefile still selects the right family, so nothing changes for it. */
#include "mqom3_parameters.h"
#if MQOM3_PARAM_OT_VARIANT == 1

/* DFS-specific GGM tree operations for the OT (One-Tree) large variant.
 * All four functions here are always compiled, under their _dfs-suffixed names
 * (see ggm_tree_large_dfs.h): callers wanting the O(H)-memory guarantee regardless
 * of LARGE_TREE_BFS (BLC memopt) call these names directly; blc_default_ot.c uses
 * the generic names, aliased to these only when LARGE_TREE_BFS is not set. */

#include "ggm_tree_large.h"
#include "ggm_tree_large_incr.h"
#include "ggm_tree_large_common.h"

int LargeGGMTree_Expand_dfs(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], uint8_t node[LARGE_GGM_TREE_SIZE][MQOM3_PARAM_SEED_SIZE], uint8_t lseed[MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
	uint32_t k, i;
	uint8_t tweaked_salt[MQOM3_PARAM_SALT_SIZE];
	enc_ctx DECL_VAR(ctx);

	/* Root seed */
	memcpy(node[1], mseed, MQOM3_PARAM_SEED_SIZE);

	/* Expand: k = 1 to tauN-1 */
	for(k = 1; k < (uint32_t)LARGE_GGM_TREE_NB_LEAVES; k++) {
		TweakSalt(salt, tweaked_salt, SALT_SEL_GGM, k);
		ret = enc_key_sched(&ctx, tweaked_salt);
		ERR(ret, err);
		/* node[2k] = SeedDerive(tweaked_salt, node[k]) */
		ret = SeedDerive(&ctx, node[k], node[2 * k]);
		ERR(ret, err);
		/* node[2k+1] = node[2k] ^ node[k] */
		xor_blocks(node[2 * k], node[k], node[(2 * k) + 1]);
	}

	/* Extract leaf seeds: lseed[i] = node[LeafPosition(i)] */
	for(i = 0; i < (uint32_t)LARGE_GGM_TREE_NB_LEAVES; i++) {
		memcpy(lseed[i], node[LeafPosition(i)], MQOM3_PARAM_SEED_SIZE);
	}

	ret = 0;
err:
	enc_clean_ctx(&ctx);
	return ret;
}

int LargeGGMTree_ExpandLeavesOnly_dfs(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], uint8_t lseed[MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_SEED_SIZE]) {
	large_ggmtree_incr_ctx_t ctx;
	int ret;
	uint32_t i;

	ret = LargeGGMTree_InitIncrementalExpansion(&ctx, salt, mseed);
	ERR(ret, err);
	for(i = 0; i < (uint32_t)LARGE_GGM_TREE_NB_LEAVES; i++) {
		ret = LargeGGMTree_GetNextLeaf(&ctx, lseed[i]);
		ERR(ret, err);
	}
	ret = 0;
err:
	LargeGGMTree_CleanIncrementalExpansion(&ctx);
	return ret;
}

int LargeGGMTree_ExpandPath_dfs(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU], uint8_t path[MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE], uint8_t hidden_lseeds[MQOM3_PARAM_TAU][MQOM3_PARAM_SEED_SIZE]) {
	/* Stack holds (node_index, seed) pairs; seeds are derived on-the-fly from the root. */
	struct { uint32_t k; uint8_t seed[MQOM3_PARAM_SEED_SIZE]; } stack[MQOM3_PARAM_LARGE_GGM_H + 1];
	/* Sorted in place for O(log |hn|) binary search: the unsorted insertion
	 * order is never read again after GetSensitiveNodeIndexes populates
	 * this, so no separate sorted copy is needed. */
	uint32_t hidden_nodes[MQOM3_PARAM_TAU * (MQOM3_PARAM_LARGE_GGM_H + 1)];
	uint32_t nb_hidden_nodes, path_len, k;
	uint8_t seed[MQOM3_PARAM_SEED_SIZE];
	uint8_t left_seed[MQOM3_PARAM_SEED_SIZE];
	uint8_t tweaked_salt[MQOM3_PARAM_SALT_SIZE];
	enc_ctx DECL_VAR(ctx);
	int32_t size;
	int ret = -1, sp;

	GetSensitiveNodeIndexes(hidden_leaf_idxs, hidden_nodes, &nb_hidden_nodes);
	size = ((int32_t)nb_hidden_nodes) - (2 * (int32_t)MQOM3_PARAM_TAU) + 1;
	if(size > (int32_t)MQOM3_PARAM_LARGE_GGM_T_OPEN) {
		goto err;
	}

	sort_u32(hidden_nodes, nb_hidden_nodes);

	/* DFS from root with on-the-fly seed derivation */
	sp = 0;
	path_len = 0;
	stack[sp].k = 1;
	memcpy(stack[sp].seed, mseed, MQOM3_PARAM_SEED_SIZE);
	sp++;

	while(sp > 0) {
		sp--;
		k = stack[sp].k;
		memcpy(seed, stack[sp].seed, MQOM3_PARAM_SEED_SIZE);

		if(!is_in_sorted(hidden_nodes, nb_hidden_nodes, k)) {
			if((size < (int32_t)MQOM3_PARAM_LARGE_GGM_T_OPEN) && (!IsLeaf(k))) {
				size++;
				/* Fall through: expand this non-hidden internal node */
			} else {
				/* Reveal. */
				if (path_len >= MQOM3_PARAM_LARGE_GGM_T_OPEN) {
					goto err;
				}
				memcpy(path + (path_len * MQOM3_PARAM_SEED_SIZE), seed, MQOM3_PARAM_SEED_SIZE);
				path_len++;
				continue;
			}
		}

		if(!IsLeaf(k)) {
			/* Derive left and right child seeds, push both (right then left, LIFO -> left first) */
			TweakSalt(salt, tweaked_salt, SALT_SEL_GGM, k);
			ret = enc_key_sched(&ctx, tweaked_salt);
			ERR(ret, err);
			ret = SeedDerive(&ctx, seed, left_seed);
			ERR(ret, err);
			/* right = left XOR parent (spec: right_child = left_child ^ node) */
			stack[sp].k = (2 * k) + 1;
			xor_blocks(left_seed, seed, stack[sp].seed);
			sp++;
			stack[sp].k = (2 * k);
			memcpy(stack[sp].seed, left_seed, MQOM3_PARAM_SEED_SIZE);
			sp++;
		} else {
			/* Hidden leaf: e = abs_leaf_idx % tau */
			memcpy(hidden_lseeds[(FromLeafPosition(k) % MQOM3_PARAM_TAU)], seed, MQOM3_PARAM_SEED_SIZE);
		}
	}
	/* Sanity check. */
	if(path_len != MQOM3_PARAM_LARGE_GGM_T_OPEN) {
		ret = -1;
		goto err;
	}
	ret = 0;
err:
	enc_clean_ctx(&ctx);
	/* stack/seed/left_seed hold unrevealed GGM tree seed material derived from
	 * the secret mseed; wipe them before returning. */
	mqom_cleanse(stack, sizeof(stack));
	mqom_cleanse(seed, sizeof(seed));
	mqom_cleanse(left_seed, sizeof(left_seed));
	return ret;
}

int LargeGGMTree_PartiallyExpand_dfs(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t path[MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE], const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU], uint8_t lseed[MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_SEED_SIZE]) {
	/* Mirror of ExpandPath: same DFS order and budget logic, reading from path where ExpandPath wrote.
	 * Nodes on the hidden-path subtree carry an unknown seed (known=0, propagated as null).
	 * Non-hidden boundary nodes read their seed from path; their descendants are then derived normally. */
	struct { uint32_t k; uint8_t seed[MQOM3_PARAM_SEED_SIZE]; uint8_t known; } stack[MQOM3_PARAM_LARGE_GGM_H + 1];
	/* Sorted in place for O(log |hn|) binary search: the unsorted insertion
	 * order is never read again after GetSensitiveNodeIndexes populates
	 * this, so no separate sorted copy is needed. */
	uint32_t hidden_nodes[MQOM3_PARAM_TAU * (MQOM3_PARAM_LARGE_GGM_H + 1)];
	uint32_t nb_hidden_nodes, path_idx, k;
	uint8_t seed[MQOM3_PARAM_SEED_SIZE];
	uint8_t left_seed[MQOM3_PARAM_SEED_SIZE];
	uint8_t tweaked_salt[MQOM3_PARAM_SALT_SIZE];
	/* Verification handles public data only, hence the public block cipher
	 * rather than the constant-time one (see LargeGGMTree_GetNextLeafPartial). */
	enc_ctx_pub DECL_VAR(ctx);
	int32_t size;
	int ret = -1, sp;
	uint8_t known;

	GetSensitiveNodeIndexes(hidden_leaf_idxs, hidden_nodes, &nb_hidden_nodes);
	size = ((int32_t)nb_hidden_nodes) - (2 * (int32_t)MQOM3_PARAM_TAU) + 1;
	if(size > (int32_t)MQOM3_PARAM_LARGE_GGM_T_OPEN) {
		goto err;
	}

	sort_u32(hidden_nodes, nb_hidden_nodes);

	/* DFS from root with unknown seed (null) */
	sp = 0;
	path_idx = 0;
	stack[sp].k = 1;
	stack[sp].known = 0;
	sp++;

	while(sp > 0) {
		sp--;
		k     = stack[sp].k;
		known = stack[sp].known;
		if(known) {
			memcpy(seed, stack[sp].seed, MQOM3_PARAM_SEED_SIZE);
		}

		/* Non-hidden node with unknown seed: read from path (or expand if budget allows) */
		if((!known) && (!is_in_sorted(hidden_nodes, nb_hidden_nodes, k))) {
			if((size < (int32_t)MQOM3_PARAM_LARGE_GGM_T_OPEN) && (!IsLeaf(k))) {
				size++;
				/* Fall through with known=0: children inherit null, filled from path later */
			} else {
				/* Read seed from path (mirrors the reveal in ExpandPath/Open).
				 * The budget logic guarantees path_idx stays below T_OPEN, but
				 * check it anyway: an overrun would read past the path inside
				 * the opening, i.e. within the same allocation, so no sanitizer
				 * would catch it. Same guard as GetNodeIndexesInPath. */
				if(path_idx >= MQOM3_PARAM_LARGE_GGM_T_OPEN) {
					goto err;
				}
				memcpy(seed, path + (path_idx * MQOM3_PARAM_SEED_SIZE), MQOM3_PARAM_SEED_SIZE);
				path_idx++;
				known = 1;
			}
		}

		if(!IsLeaf(k)) {
			if(known) {
				/* Derive left and right child seeds (push right then left, LIFO -> left first) */
				TweakSalt(salt, tweaked_salt, SALT_SEL_GGM, k);
				ret = enc_key_sched_pub(&ctx, tweaked_salt);
				ERR(ret, err);
				ret = SeedDerive_pub(&ctx, seed, left_seed);
				ERR(ret, err);
				stack[sp].k = (2 * k) + 1;
				xor_blocks(left_seed, seed, stack[sp].seed);
				stack[sp].known = 1;
				sp++;
				stack[sp].k = (2 * k);
				memcpy(stack[sp].seed, left_seed, MQOM3_PARAM_SEED_SIZE);
				stack[sp].known = 1;
				sp++;
			} else {
				/* Unknown seed (null): propagate null to both children (right then left) */
				stack[sp].k = (2 * k) + 1;
				stack[sp].known = 0;
				sp++;
				stack[sp].k = (2 * k);
				stack[sp].known = 0;
				sp++;
			}
		} else {
			/* Non-hidden leaf with known seed: write to lseed output.
			 * Hidden leaves (known=0) are zeroed: the header contract promises it. */
			if(known) {
				memcpy(lseed[FromLeafPosition(k)], seed, MQOM3_PARAM_SEED_SIZE);
			} else {
				memset(lseed[FromLeafPosition(k)], 0, MQOM3_PARAM_SEED_SIZE);
			}
		}
	}
	ret = 0;
err:
	enc_clean_ctx_pub(&ctx);
	return ret;
}

#else /* MQOM3_VARIANT_GUARD: CT build, nothing to compile here */
typedef int ggm_tree_large_dfs_unused_in_ct_build;
#endif
