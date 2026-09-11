/* MQOM3_VARIANT_GUARD: this unit implements the CT variant only - the small
 * per-execution trees are used by blc_ct_default.c and blc_ct_memopt.c alone,
 * the OT path using the single large tree instead. Unlike the large-tree units
 * this one does compile under OT parameters (it needs only NB_EVALS, SEED_SIZE
 * and friends, which both variants define), so the guard is not a compilation
 * necessity: it keeps the CT/OT treatment symmetric and stops ~5 kB of code
 * nothing calls from being linked into every OT binary.
 * See the same marker on the large-tree and blc_*_ counterparts. */
#include "mqom3_parameters.h"
#if MQOM3_PARAM_OT_VARIANT != 1

#include "ggm_tree_small.h"
#include "ggm_tree_common.h"
#include "ggm_tree_common_ecb.h"

/* Number of parent nodes handed to a single bulk ECB call when expanding one
 * level of the small GGM tree.  All nodes of a level share the same key (the
 * tweaked salt), and node[] stores a level contiguously, so a whole level could
 * be done in one call; chunking only exists to keep the working set (chunk
 * parents + 2*chunk children) inside L1 while still amortizing the per-call
 * overhead of the ECB primitive over many blocks.
 * Note this no longer influences memory: SeedDerive_nb_ecb writes its children
 * straight into node[] and uses no batch-sized scratch buffer, unlike the
 * previous SeedDerive_x8_ecb path whose gather/enc_out/linortho buffers scaled
 * with the batch width. */
#ifndef SMALL_GGM_TREE_LEVEL_CHUNK
#define SMALL_GGM_TREE_LEVEL_CHUNK 128
#endif
/* Must be a power of two */
#if (SMALL_GGM_TREE_LEVEL_CHUNK < 1) || \
    ((SMALL_GGM_TREE_LEVEL_CHUNK & (SMALL_GGM_TREE_LEVEL_CHUNK - 1)) != 0)
#error "SMALL_GGM_TREE_LEVEL_CHUNK must be a power of two."
#endif

/* NOTE: in the "node" tree representation, we accept to remove the two first cells to simplify the indices computations.
 * The first cell is used to avoid the "0" index, and the second cell is the root of the correlated tree that is not used.
 * */
int SmallGGMTree_Expand(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t delta[MQOM3_PARAM_SEED_SIZE], uint32_t e, uint8_t node[MQOM3_PARAM_FULL_TREE_SIZE + 1][MQOM3_PARAM_SEED_SIZE], uint8_t lseed[MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
	/* j is the level in the tree, k is the index in the nodes array */
	uint32_t j, k;
	enc_ctx DECL_VAR(ctx);
	enc_ctx_ecb DECL_VAR(ctx_ecb);
	uint8_t tweaked_salt[MQOM3_PARAM_SALT_SIZE];

	/* Some sanity check */
	if ((1 << MQOM3_PARAM_NB_EVALS_LOG) != MQOM3_PARAM_NB_EVALS) {
		ret = -1;
		goto err;
	}

	/* The root node is not defined; level 0 (its two children) derives via
	 * SeedDerive(TweakSalt(salt,2,IndexIdentifier(e,0)), mseed), uniformly
	 * with every other level below (replaces the removed TweakSeed primitive). */
	TweakSalt(salt, tweaked_salt, SALT_SEL_GGM, IndexIdentifier(e, 0));
	ret = enc_key_sched(&ctx, tweaked_salt);
	ERR(ret, err);
	ret = SeedDerive(&ctx, mseed, node[2]);
	ERR(ret, err);
	xor_blocks(node[2], delta, node[3]);

	/* Now deal with the other levels */
	for (j = 1; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		/* Level j holds 2^j nodes, stored contiguously in node[2^j .. 2^(j+1)-1],
		 * and their children occupy node[2^(j+1) .. 2^(j+2)-1], also contiguous.
		 * A chunk of num_derivations parents can therefore be fed to a single
		 * bulk ECB encryption straight out of node[] and expanded in place into
		 * node[2*k ...], with no gather buffer and no scatter list.
		 * Level 1 only has 2 nodes, so the chunk is clamped to the level size. */
		uint32_t level_size = (uint32_t)1 << j;
		uint32_t num_derivations = (level_size < (uint32_t)SMALL_GGM_TREE_LEVEL_CHUNK) ?
		                           level_size : (uint32_t)SMALL_GGM_TREE_LEVEL_CHUNK;
		/* For a whole level in the tree, we use the same key, which is the
		 * tweaked salt, hence the common (single, non-duplicated) key
		 * schedule -- shared by every batch at this level, of whatever size. */
		TweakSalt(salt, tweaked_salt, SALT_SEL_GGM, IndexIdentifier(e, j));
		ret = enc_key_sched_ecb(&ctx_ecb, tweaked_salt);
		ERR(ret, err);
		/* NOTE: num_derivations is a power of two that never exceeds this
		 * level's node count, so it always evenly divides it -- no
		 * leftover/remainder handling needed here. */
		for (k = level_size; k < (2 * level_size); k += num_derivations) {
			ret = SeedDerive_nb_ecb(&ctx_ecb,
			              (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE]) node[k],
			              (uint8_t (*)[MQOM3_PARAM_SEED_SIZE]) node[2 * k],
			              num_derivations);
			ERR(ret, err);
		}
	}

	/* The lseed are the leaves of the tree, copied from it */
	memcpy(lseed, node[MQOM3_PARAM_FULL_TREE_SIZE + 1 - MQOM3_PARAM_NB_EVALS], MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_SEED_SIZE);

	ret = 0;
err:
	enc_clean_ctx(&ctx);
	enc_clean_ctx_ecb(&ctx_ecb);
	return ret;
}

int SmallGGMTree_Open(const uint8_t node[MQOM3_PARAM_FULL_TREE_SIZE + 1][MQOM3_PARAM_SEED_SIZE], uint32_t i_star, uint8_t path[MQOM3_PARAM_NB_EVALS_LOG][MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
	uint32_t i, j;

	/* Sanity check */
	if (i_star >= MQOM3_PARAM_NB_EVALS) {
		ret = -1;
		goto err;
	}

	i = MQOM3_PARAM_NB_EVALS + i_star;
	for (j = 0; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		memcpy(path[j], node[i ^ 1], MQOM3_PARAM_SEED_SIZE);
		i = (i / 2);
	}

	ret = 0;
err:
	return ret;
}

/* XXX: NOTE: we can use public encryption API here as this function is used for verification */
int SmallGGMTree_PartiallyExpand(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t path[MQOM3_PARAM_NB_EVALS_LOG][MQOM3_PARAM_SEED_SIZE], uint32_t e, uint32_t i_star, uint8_t lseed[MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
	/* j is the level in the tree, k is the index in the nodes array */
	uint32_t i, j, k;
	enc_ctx_pub_ecb DECL_VAR(ctx_ecb);
	uint8_t tweaked_salt[MQOM3_PARAM_SALT_SIZE];
	/* Heap-allocate the full tree to avoid large stack frames (up to 512 KB for cat5-gf2) */
	uint8_t (*node)[MQOM3_PARAM_SEED_SIZE] = NULL;
	node = mqom_malloc((size_t)(MQOM3_PARAM_FULL_TREE_SIZE + 1) * MQOM3_PARAM_SEED_SIZE);
	ERR_NULL(node, err);

	/* Some sanity checks */
	if ((1 << MQOM3_PARAM_NB_EVALS_LOG) != MQOM3_PARAM_NB_EVALS) {
		ret = -1;
		goto err;
	}
	if (i_star >= MQOM3_PARAM_NB_EVALS) {
		ret = -1;
		goto err;
	}

	/* Copy the path.
	 * This also fixes, once and for all, which nodes are defined: the loop
	 * writes the sibling of the hidden leaf's ancestor at every level, and the
	 * forward sweep below then derives everything reachable from those. */
	i = MQOM3_PARAM_NB_EVALS + i_star;
	for (j = 0; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		memcpy(node[i ^ 1], path[j], MQOM3_PARAM_SEED_SIZE);
		i = (i / 2);
	}

	/* Compute the other nodes when possible.
	 * */
	for (j = 1; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		/* Level j holds 2^j nodes at contiguous indices node[2^j .. 2^(j+1)-1].
		 * On the verification side some of them are still undefined */
		uint32_t level_start = (uint32_t)1 << j;
		uint32_t level_end = (uint32_t)1 << (j + 1);
		uint32_t hole = (MQOM3_PARAM_NB_EVALS + i_star) >> (MQOM3_PARAM_NB_EVALS_LOG - j);
		uint32_t max_run = (level_start < (uint32_t)SMALL_GGM_TREE_LEVEL_CHUNK) ?
		                   level_start : (uint32_t)SMALL_GGM_TREE_LEVEL_CHUNK;
		uint32_t part;
		/* For a whole level in the tree, we use the same key, which is the
		 * tweaked salt, hence the common (single, non-duplicated) key
		 * schedule -- shared by every batch at this level, of whatever size. */
		TweakSalt(salt, tweaked_salt, SALT_SEL_GGM, IndexIdentifier(e, j));
		ret = enc_key_sched_pub_ecb(&ctx_ecb, tweaked_salt);
		ERR(ret, err);
		for (part = 0; part < 2; part++) {
			uint32_t run_start = (part == 0) ? level_start : (hole + 1);
			uint32_t run_end = (part == 0) ? hole : level_end;
			for (k = run_start; k < run_end; k += max_run) {
				uint32_t run = ((run_end - k) < max_run) ? (run_end - k) : max_run;
				ret = SeedDerive_nb_pub_ecb(&ctx_ecb,
				                  (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE]) node[k],
				                  (uint8_t (*)[MQOM3_PARAM_SEED_SIZE]) node[2 * k],
				                  run);
				ERR(ret, err);
			}
		}
	}

	/* The lseed are the leaves of the tree, copied from it.
	 * Copy around the hidden leaf rather than over it: node[] slot i_star was
	 * never derived, so copying it and overwriting it afterwards would read
	 * uninitialized storage for no purpose. */
	memcpy(lseed[0], node[MQOM3_PARAM_FULL_TREE_SIZE + 1 - MQOM3_PARAM_NB_EVALS], (size_t)i_star * MQOM3_PARAM_SEED_SIZE);
	/* Set the hidden leave to zero */
	memset(lseed[i_star], 0, MQOM3_PARAM_SEED_SIZE);
	/* Guarded because for the last leaf (i_star == NB_EVALS - 1) the copy is
	 * empty and both pointers would be one past the end of their array. */
	if ((i_star + 1) < MQOM3_PARAM_NB_EVALS) {
		memcpy(lseed[i_star + 1], node[MQOM3_PARAM_FULL_TREE_SIZE + 2 - MQOM3_PARAM_NB_EVALS + i_star],
		       (size_t)(MQOM3_PARAM_NB_EVALS - i_star - 1) * MQOM3_PARAM_SEED_SIZE);
	}

	ret = 0;
err:
	enc_clean_ctx_pub_ecb(&ctx_ecb);
	mqom_free(node, (size_t)(MQOM3_PARAM_FULL_TREE_SIZE + 1) * MQOM3_PARAM_SEED_SIZE);
	return ret;
}

int SmallGGMTree_ExpandPath(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t delta[MQOM3_PARAM_SEED_SIZE], uint32_t e, uint32_t i_star, uint8_t path[MQOM3_PARAM_NB_EVALS_LOG][MQOM3_PARAM_SEED_SIZE], uint8_t lseed[MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
	uint32_t j;
	enc_ctx DECL_VAR(ctx);
	uint8_t tweaked_salt[MQOM3_PARAM_SALT_SIZE];

	/* Sanity check */
	if (i_star >= MQOM3_PARAM_NB_EVALS) {
		ret = -1;
		goto err;
	}

	uint32_t num_leaf = MQOM3_PARAM_NB_EVALS + i_star;

	uint8_t node[2][MQOM3_PARAM_SEED_SIZE];
	uint8_t parent[MQOM3_PARAM_SEED_SIZE];
	memcpy(parent, delta, MQOM3_PARAM_SEED_SIZE);

	for (j = 0; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		/* Level 0 derives node[0] from mseed (replacing the removed TweakSeed
		 * primitive); every other level derives from the previous level's
		 * selected node (parent), uniformly via SeedDerive. The xor operand
		 * "parent" stays delta at j==0: the correlated-tree offset, which is
		 * DeriveDelta(x) - the first SEED_SIZE bytes of the serialized secret
		 * witness, so secret material, not public. */
		TweakSalt(salt, tweaked_salt, SALT_SEL_GGM, IndexIdentifier(e, j));
		ret = enc_key_sched(&ctx, tweaked_salt);
		ERR(ret, err);
		if (j == 0) {
			ret = SeedDerive(&ctx, mseed, node[0]);
		} else {
			ret = SeedDerive(&ctx, parent, node[0]);
		}
		ERR(ret, err);
		xor_blocks(node[0], parent, node[1]);

		uint32_t bit = (num_leaf >> (MQOM3_PARAM_NB_EVALS_LOG - 1 - j)) & 0x01;
		memcpy(path[MQOM3_PARAM_NB_EVALS_LOG - 1 - j], node[bit ^ 1], MQOM3_PARAM_SEED_SIZE);
		memcpy(parent, node[bit], MQOM3_PARAM_SEED_SIZE);
	}

	memcpy(lseed, parent, MQOM3_PARAM_SEED_SIZE);

	ret = 0;
err:
	enc_clean_ctx(&ctx);
	/* node/parent hold unrevealed GGM tree seed material derived from the
	 * secret mseed/delta; wipe them before returning. */
	mqom_cleanse(node, sizeof(node));
	mqom_cleanse(parent, sizeof(parent));
	return ret;
}

#else /* MQOM3_VARIANT_GUARD: OT build, nothing to compile here */
typedef int ggm_tree_small_unused_in_ot_build;
#endif
