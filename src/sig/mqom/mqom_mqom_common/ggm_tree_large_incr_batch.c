/* MQOM3_VARIANT_GUARD: this unit implements the OT variant only. Build systems
 * that compile every .c of a directory - SUPERCOP does, and so does any wildcard
 * Makefile - hand it to the compiler for CT instances too, where its parameters
 * simply do not exist. Rather than requiring every such build to filter its source
 * list, the unit reduces to a single typedef there. Not an empty file: ISO C forbids
 * an empty translation unit and -Wpedantic says so.
 * The in-tree Makefile still selects the right family, so nothing changes for it. */
#include "mqom3_parameters.h"
#if MQOM3_PARAM_OT_VARIANT == 1

#include "ggm_tree_large_incr_batch.h"

#if LARGE_GGM_TREE_INCR_BATCH_ANY

#include "ggm_tree_large_common.h"
#include "ggm_tree_common.h"

/* Sanity check */
#if (((MQOM3_PARAM_TAU) * (MQOM3_PARAM_NB_EVALS)) % 2) != 0
#error "LARGE_GGM_TREE_INCR_BATCH requires an even tau*NB_EVALS"
#endif

/* Derive both kA's and kB's children in one x2 call: 2 distinct keys
 * (key(kA) on seedA, key(kB) on seedB), each node's own key on its own
 * seed. Right child = left child XOR parent seed, same trick as the
 * unbatched path.
 *
 * Generated in two flavours from a single body: Sign drives the
 * constant-time block cipher (secret seeds), Verify the public one, since
 * the partial expansion only ever handles public data - same split as
 * SeedDerive/SeedDerive_pub, as the CT small tree, and as MQOM2's own
 * is_public flag on its one-tree iterator. */
#define GEN_DERIVE_PAIR(NAME, CTX, KEY_SCHED, DERIVE, CLEAN)                    \
static int NAME(const uint8_t salt[MQOM3_PARAM_SALT_SIZE],                      \
        uint32_t kA, const uint8_t seedA[MQOM3_PARAM_SEED_SIZE],                \
        uint32_t kB, const uint8_t seedB[MQOM3_PARAM_SEED_SIZE],                \
        uint8_t leftA[MQOM3_PARAM_SEED_SIZE], uint8_t rightA[MQOM3_PARAM_SEED_SIZE], \
        uint8_t leftB[MQOM3_PARAM_SEED_SIZE], uint8_t rightB[MQOM3_PARAM_SEED_SIZE]) { \
	uint8_t tsA[MQOM3_PARAM_SALT_SIZE];                                         \
	uint8_t tsB[MQOM3_PARAM_SALT_SIZE];                                         \
	CTX DECL_VAR(ctx_enc);                                                      \
	int ret;                                                                    \
                                                                                \
	TweakSalt(salt, tsA, SALT_SEL_GGM, kA);                                                \
	TweakSalt(salt, tsB, SALT_SEL_GGM, kB);                                                \
	ret = KEY_SCHED(&ctx_enc, tsA, tsB);                                        \
	ERR(ret, err);                                                              \
	ret = DERIVE(&ctx_enc, seedA, seedB, leftA, leftB);                         \
	ERR(ret, err);                                                              \
	xor_blocks(leftA, seedA, rightA);                                           \
	xor_blocks(leftB, seedB, rightB);                                           \
	ret = 0;                                                                    \
err:                                                                            \
	CLEAN(&ctx_enc);                                                            \
	return ret;                                                                 \
}

GEN_DERIVE_PAIR(derive_pair,     enc_ctx_x2,     enc_key_sched_x2,     SeedDerive_x2_x2,     enc_clean_ctx_x2)
GEN_DERIVE_PAIR(derive_pair_pub, enc_ctx_pub_x2, enc_key_sched_pub_x2, SeedDerive_x2_x2_pub, enc_clean_ctx_pub_x2)

/* Bootstrap a pair frame from a single already-known node k (its own two
 * children), via one single-key derivation - unavoidable since only one
 * seed is available yet. Used both by Sign's Init (k=1, seed=mseed) and by
 * Verify's outer stack the moment a node resolves to known+non-leaf.
 * Precondition: k is not a leaf (both callers only invoke this on nodes
 * already checked non-leaf). Same two-flavour generation as above. */
#define GEN_BOOTSTRAP_PAIR(NAME, CTX, KEY_SCHED, DERIVE, CLEAN, CLEANSE)        \
static int NAME(const uint8_t salt[MQOM3_PARAM_SALT_SIZE],                      \
        uint32_t k, const uint8_t seed[MQOM3_PARAM_SEED_SIZE],                  \
        large_ggm_pair_frame_t *out) {                                          \
	uint8_t tweaked_salt[MQOM3_PARAM_SALT_SIZE];                                \
	uint8_t left_seed[MQOM3_PARAM_SEED_SIZE];                                   \
	CTX DECL_VAR(ctx_enc);                                                      \
	int ret;                                                                    \
                                                                                    \
	TweakSalt(salt, tweaked_salt, SALT_SEL_GGM, k);                                        \
	ret = KEY_SCHED(&ctx_enc, tweaked_salt);                                    \
	ERR(ret, err);                                                              \
	ret = DERIVE(&ctx_enc, seed, left_seed);                                    \
	ERR(ret, err);                                                              \
	out->kA = 2 * k;                                                            \
	memcpy(out->seedA, left_seed, MQOM3_PARAM_SEED_SIZE);                       \
	out->kB = (2 * k) + 1;                                                      \
	xor_blocks(left_seed, seed, out->seedB);                                    \
	ret = 0;                                                                    \
err:                                                                                \
	CLEAN(&ctx_enc);                                                            \
	CLEANSE(left_seed, sizeof(left_seed));                                      \
	return ret;                                                                 \
}

#if LARGE_GGM_TREE_INCR_BATCH == 1
GEN_BOOTSTRAP_PAIR(bootstrap_pair,     enc_ctx,     enc_key_sched,     SeedDerive,     enc_clean_ctx,     mqom_cleanse)
#endif
#if LARGE_GGM_TREE_INCR_BATCH_VERIFY == 1
GEN_BOOTSTRAP_PAIR(bootstrap_pair_pub, enc_ctx_pub, enc_key_sched_pub, SeedDerive_pub, enc_clean_ctx_pub, GGM_NO_CLEANSE)
#endif

/* Runs the pair engine on stack / *sp until it either produces a leaf pair
 * (written into buf, *buf_count set to 2) or the stack is exhausted
 * (*sp == 0, *buf_count == 0). Shared by Sign (whole tree, always "known")
 * and Verify (the known-subtree inner stack, entered once a node resolves
 * to known+non-leaf). Left-first DFS: kA's whole subtree (however deep) is
 * fully drained before kB's is ever touched, exactly like the plain
 * single-node algorithm, just with 2-node frames as the atomic unit.
 *
 * is_public selects the block cipher flavour (0 for Sign, 1 for Verify) so
 * the traversal itself exists in a single copy - one predictable branch per
 * derivation, against a full block-cipher call. */
static int run_pair_engine(const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
        large_ggm_pair_frame_t *stack, int *sp,
        uint8_t buf[2][MQOM3_PARAM_SEED_SIZE], uint32_t *buf_count, int is_public) {
	int ret;
	uint8_t work[6][MQOM3_PARAM_SEED_SIZE];

	*buf_count = 0;
	while (*sp > 0) {
		large_ggm_pair_frame_t *top = &stack[*sp - 1];

		if (IsLeaf(top->kA)) {
			/* kA,kB are always true siblings {2P,2P+1} and tau*NB_EVALS is
			 * even (enforced above), so IsLeaf(kA) implies IsLeaf(kB). */
			memcpy(buf[0], top->seedA, MQOM3_PARAM_SEED_SIZE);
			memcpy(buf[1], top->seedB, MQOM3_PARAM_SEED_SIZE);
			*buf_count = 2;
			(*sp)--;
			if (!is_public) { mqom_cleanse(work, sizeof(work)); }
			return 0;
		}

		{
			large_ggm_pair_frame_t *deferred;
			uint32_t kA = top->kA, kB = top->kB;
			uint8_t *seedA = work[0], *seedB = work[1];
			uint8_t *leftA = work[2], *rightA = work[3];
			uint8_t *leftB = work[4], *rightB = work[5];

			memcpy(seedA, top->seedA, MQOM3_PARAM_SEED_SIZE);
			memcpy(seedB, top->seedB, MQOM3_PARAM_SEED_SIZE);
			(*sp)--;

			if (is_public) {
				ret = derive_pair_pub(salt, kA, seedA, kB, seedB, leftA, rightA, leftB, rightB);
			} else {
				ret = derive_pair(salt, kA, seedA, kB, seedB, leftA, rightA, leftB, rightB);
			}
			ERR(ret, err);

			/* Push kB's own children (deferred), then kA's (explored now, on top). */
			deferred = &stack[(*sp)++];
			deferred->kA = 2 * kB;
			memcpy(deferred->seedA, leftB, MQOM3_PARAM_SEED_SIZE);
			deferred->kB = (2 * kB) + 1;
			memcpy(deferred->seedB, rightB, MQOM3_PARAM_SEED_SIZE);

			{
				large_ggm_pair_frame_t *current = &stack[(*sp)++];
				current->kA = 2 * kA;
				memcpy(current->seedA, leftA, MQOM3_PARAM_SEED_SIZE);
				current->kB = (2 * kA) + 1;
				memcpy(current->seedB, rightA, MQOM3_PARAM_SEED_SIZE);
			}
		}
	}
	if (!is_public) { mqom_cleanse(work, sizeof(work)); }
	return 1;

err:
	if (!is_public) { mqom_cleanse(work, sizeof(work)); }
	return -1;
}

#if LARGE_GGM_TREE_INCR_BATCH == 1
int LargeGGMTree_InitIncrementalExpansion_batch(large_ggmtree_incr_batch_ctx_t *ctx,
        const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE]) {
	int ret;

	memcpy(ctx->salt, salt, MQOM3_PARAM_SALT_SIZE);
	ret = bootstrap_pair(salt, 1, mseed, &ctx->stack[0]);
	ERR(ret, err);
	ctx->sp = 1;
	ctx->num_leaf = 0;
	ctx->buf_count = 0;
	ctx->buf_pos = 0;
	return 0;

err:
	return -1;
}

int LargeGGMTree_GetNextLeaf_batch(large_ggmtree_incr_batch_ctx_t *ctx, uint8_t lseed_out[MQOM3_PARAM_SEED_SIZE]) {
	int ret;

	if (ctx->buf_pos < ctx->buf_count) {
		memcpy(lseed_out, ctx->buf[ctx->buf_pos], MQOM3_PARAM_SEED_SIZE);
		ctx->buf_pos++;
		ctx->num_leaf++;
		return 0;
	}

	ret = run_pair_engine(ctx->salt, ctx->stack, &ctx->sp, ctx->buf, &ctx->buf_count, 0);
	if (ret < 0) {
		return -1;
	}
	if (ctx->buf_count == 0) {
		return 1;
	}
	memcpy(lseed_out, ctx->buf[0], MQOM3_PARAM_SEED_SIZE);
	ctx->buf_pos = 1;
	ctx->num_leaf++;
	return 0;
}

void LargeGGMTree_CleanIncrementalExpansion_batch(large_ggmtree_incr_batch_ctx_t *ctx) {
	mqom_cleanse(ctx, sizeof(*ctx));
}
#endif /* LARGE_GGM_TREE_INCR_BATCH == 1 */

#if LARGE_GGM_TREE_INCR_BATCH_VERIFY == 1
int LargeGGMTree_InitIncrementalPartialExpansion_batch(large_ggmtree_incr_batch_partial_ctx_t *ctx,
        const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
        const uint8_t path[MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE],
        const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU]) {
	memcpy(ctx->salt, salt, MQOM3_PARAM_SALT_SIZE);
	ctx->path = path;
	ctx->path_idx = 0;
	ctx->num_leaf = 0;

	GetSensitiveNodeIndexes(hidden_leaf_idxs, ctx->hidden_nodes, &ctx->nb_hidden_nodes);
	ctx->size = ((int32_t) ctx->nb_hidden_nodes) - (2 * (int32_t) MQOM3_PARAM_TAU) + 1;
	if (ctx->size > (int32_t) MQOM3_PARAM_LARGE_GGM_T_OPEN) {
		return -1;
	}
	sort_u32(ctx->hidden_nodes, ctx->nb_hidden_nodes);

	ctx->outer_stack[0].k = 1;
	ctx->outer_stack[0].known = 0;
	ctx->outer_sp = 1;
	ctx->inner_sp = 0;
	ctx->buf_count = 0;
	ctx->buf_pos = 0;

	return 0;
}

/* Verify/eval side. See ggm_tree_large_incr_batch.h for why the moment a
 * node resolves to known+non-leaf, its whole subtree can be handed to the
 * same pair engine Sign uses (a known node's subtree is guaranteed free of
 * both hidden leaves and any further path_idx consumption). */
int LargeGGMTree_GetNextLeafPartial_batch(large_ggmtree_incr_batch_partial_ctx_t *ctx, uint8_t lseed_out[MQOM3_PARAM_SEED_SIZE]) {
	int ret;

	if (ctx->buf_pos < ctx->buf_count) {
		memcpy(lseed_out, ctx->buf[ctx->buf_pos], MQOM3_PARAM_SEED_SIZE);
		ctx->buf_pos++;
		ctx->num_leaf++;
		return 0;
	}

	if (ctx->inner_sp > 0) {
		ret = run_pair_engine(ctx->salt, ctx->inner_stack, &ctx->inner_sp, ctx->buf, &ctx->buf_count, 1);
		if (ret < 0) {
			return -1;
		}
		if (ctx->buf_count > 0) {
			memcpy(lseed_out, ctx->buf[0], MQOM3_PARAM_SEED_SIZE);
			ctx->buf_pos = 1;
			ctx->num_leaf++;
			return 0;
		}
		/* Inner subtree fully drained (ctx->inner_sp == 0): fall through to
		 * the outer stack. */
	}

	while (ctx->outer_sp > 0) {
		uint32_t k;
		uint8_t known;
		uint8_t seed[MQOM3_PARAM_SEED_SIZE];

		ctx->outer_sp--;
		k     = ctx->outer_stack[ctx->outer_sp].k;
		known = ctx->outer_stack[ctx->outer_sp].known;
		if (known) {
			memcpy(seed, ctx->outer_stack[ctx->outer_sp].seed, MQOM3_PARAM_SEED_SIZE);
		}

		if ((!known) && (!is_in_sorted(ctx->hidden_nodes, ctx->nb_hidden_nodes, k))) {
			if ((ctx->size < (int32_t) MQOM3_PARAM_LARGE_GGM_T_OPEN) && (!IsLeaf(k))) {
				ctx->size++;
			} else {
				if (ctx->path_idx >= MQOM3_PARAM_LARGE_GGM_T_OPEN) {
					return -1;
				}
				memcpy(seed, ctx->path + ((size_t) ctx->path_idx * MQOM3_PARAM_SEED_SIZE), MQOM3_PARAM_SEED_SIZE);
				ctx->path_idx++;
				known = 1;
			}
		}

		if (IsLeaf(k)) {
			ctx->num_leaf++;
			if (known) {
				memcpy(lseed_out, seed, MQOM3_PARAM_SEED_SIZE);
			} else {
				memset(lseed_out, 0, MQOM3_PARAM_SEED_SIZE);
			}
			return 0;
		}

		if (known) {
			/* Hand this node's entire subtree to the pair engine. */
			ret = bootstrap_pair_pub(ctx->salt, k, seed, &ctx->inner_stack[0]);
			if (ret) {
				return -1;
			}
			ctx->inner_sp = 1;

			ret = run_pair_engine(ctx->salt, ctx->inner_stack, &ctx->inner_sp, ctx->buf, &ctx->buf_count, 1);
			if (ret < 0) {
				return -1;
			}
			/* A single non-leaf node's subtree always yields at least one
			 * leaf pair, so buf_count > 0 here. */
			memcpy(lseed_out, ctx->buf[0], MQOM3_PARAM_SEED_SIZE);
			ctx->buf_pos = 1;
			ctx->num_leaf++;
			return 0;
		}

		/* Propagate unknown to both children (right then left, LIFO -> left first). */
		ctx->outer_stack[ctx->outer_sp].k = (2 * k) + 1;
		ctx->outer_stack[ctx->outer_sp].known = 0;
		ctx->outer_sp++;
		ctx->outer_stack[ctx->outer_sp].k = 2 * k;
		ctx->outer_stack[ctx->outer_sp].known = 0;
		ctx->outer_sp++;
	}

	return 1;
}

void LargeGGMTree_CleanIncrementalPartialExpansion_batch(large_ggmtree_incr_batch_partial_ctx_t *ctx) {
	mqom_cleanse(ctx, sizeof(*ctx));
}
#endif /* LARGE_GGM_TREE_INCR_BATCH_VERIFY == 1 */

#endif /* LARGE_GGM_TREE_INCR_BATCH_ANY */

/* Avoid an empty translation unit (-Wpedantic) when batching is compiled
 * out (the default) - unconditional, harmless, never referenced. */
typedef int mqom_ggm_tree_large_incr_batch_c_not_empty;

#else /* MQOM3_VARIANT_GUARD: CT build, nothing to compile here */
typedef int ggm_tree_large_incr_batch_unused_in_ct_build;
#endif
