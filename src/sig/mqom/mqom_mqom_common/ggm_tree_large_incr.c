/* MQOM3_VARIANT_GUARD: this unit implements the OT variant only. Build systems
 * that compile every .c of a directory - SUPERCOP does, and so does any wildcard
 * Makefile - hand it to the compiler for CT instances too, where its parameters
 * simply do not exist. Rather than requiring every such build to filter its source
 * list, the unit reduces to a single typedef there. Not an empty file: ISO C forbids
 * an empty translation unit and -Wpedantic says so.
 * The in-tree Makefile still selects the right family, so nothing changes for it. */
#include "mqom3_parameters.h"
#if MQOM3_PARAM_OT_VARIANT == 1

#include "ggm_tree_large_incr.h"
#include "ggm_tree_large_common.h"

int LargeGGMTree_InitIncrementalExpansion(large_ggmtree_incr_ctx_t *ctx,
        const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
        const uint8_t mseed[MQOM3_PARAM_SEED_SIZE]) {
	memcpy(ctx->salt, salt, MQOM3_PARAM_SALT_SIZE);
	ctx->stack[0].k = 1;
	memcpy(ctx->stack[0].seed, mseed, MQOM3_PARAM_SEED_SIZE);
	ctx->sp = 1;
	ctx->num_leaf = 0;
	return 0;
}

int LargeGGMTree_GetNextLeaf(large_ggmtree_incr_ctx_t *ctx,
        uint8_t lseed_out[MQOM3_PARAM_SEED_SIZE]) {
	uint8_t seed[MQOM3_PARAM_SEED_SIZE];
	uint8_t left_seed[MQOM3_PARAM_SEED_SIZE];
	uint8_t tweaked_salt[MQOM3_PARAM_SALT_SIZE];
	enc_ctx DECL_VAR(ctx_enc);
	uint32_t k;
	int ret = -1;

	while(ctx->sp > 0) {
		ctx->sp--;
		k = ctx->stack[ctx->sp].k;
		memcpy(seed, ctx->stack[ctx->sp].seed, MQOM3_PARAM_SEED_SIZE);

		if(IsLeaf(k)) {
			/* Leaf: yield seed to caller */
			memcpy(lseed_out, seed, MQOM3_PARAM_SEED_SIZE);
			ctx->num_leaf++;
			enc_clean_ctx(&ctx_enc);
			mqom_cleanse(seed, sizeof(seed));
			mqom_cleanse(left_seed, sizeof(left_seed));
			return 0;
		}

		/* Internal node: derive children, push right then left (left-first DFS) */
		TweakSalt(ctx->salt, tweaked_salt, SALT_SEL_GGM, k);
		ret = enc_key_sched(&ctx_enc, tweaked_salt);
		ERR(ret, err);
		ret = SeedDerive(&ctx_enc, seed, left_seed);
		ERR(ret, err);
		/* right = left XOR parent */
		ctx->stack[ctx->sp].k = (2 * k) + 1;
		xor_blocks(left_seed, seed, ctx->stack[ctx->sp].seed);
		ctx->sp++;
		ctx->stack[ctx->sp].k = (2 * k);
		memcpy(ctx->stack[ctx->sp].seed, left_seed, MQOM3_PARAM_SEED_SIZE);
		ctx->sp++;
	}

	/* All leaves exhausted */
	enc_clean_ctx(&ctx_enc);
	mqom_cleanse(seed, sizeof(seed));
	mqom_cleanse(left_seed, sizeof(left_seed));
	return 1;

err:
	enc_clean_ctx(&ctx_enc);
	mqom_cleanse(seed, sizeof(seed));
	mqom_cleanse(left_seed, sizeof(left_seed));
	return -1;
}

void LargeGGMTree_CleanIncrementalExpansion(large_ggmtree_incr_ctx_t *ctx) {
	mqom_cleanse(ctx, sizeof(*ctx));
}

int LargeGGMTree_InitIncrementalPartialExpansion(large_ggmtree_incr_partial_ctx_t *ctx,
        const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
        const uint8_t path[MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE],
        const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU]) {
	memcpy(ctx->salt, salt, MQOM3_PARAM_SALT_SIZE);
	ctx->path = path;
	ctx->path_idx = 0;
	ctx->num_leaf = 0;

	GetSensitiveNodeIndexes(hidden_leaf_idxs, ctx->hidden_nodes, &ctx->nb_hidden_nodes);
	ctx->size = ((int32_t)ctx->nb_hidden_nodes) - (2 * (int32_t)MQOM3_PARAM_TAU) + 1;
	if(ctx->size > (int32_t)MQOM3_PARAM_LARGE_GGM_T_OPEN) {
		return -1;
	}

	/* Sort hidden nodes in place for O(log |hn|) binary search during
	 * traversal: hidden_nodes is never read in its original insertion
	 * order afterward, so no separate sorted copy is needed. */
	sort_u32(ctx->hidden_nodes, ctx->nb_hidden_nodes);

	/* Push unknown root */
	ctx->stack[0].k = 1;
	ctx->stack[0].known = 0;
	ctx->sp = 1;

	return 0;
}

/* Verification only ever manipulates public data, so this uses the public
 * block cipher (typically table-based) rather than the constant-time one
 * Sign needs - same convention as the CT small tree
 * (SmallGGMTree_GetNextLeafPartial). Note the derivation stays one node at a
 * time: unlike the CT small tree, which shares one key per level and can
 * therefore batch same-key blocks through the _ecb path, the large tree keys
 * every node individually, and the public backend provides no multi-key
 * batching (enc_encrypt_pub is x1, enc_encrypt_pub_ecb is same-key only). */
int LargeGGMTree_GetNextLeafPartial(large_ggmtree_incr_partial_ctx_t *ctx,
        uint8_t lseed_out[MQOM3_PARAM_SEED_SIZE]) {
	uint8_t seed[MQOM3_PARAM_SEED_SIZE];
	uint8_t left_seed[MQOM3_PARAM_SEED_SIZE];
	uint8_t tweaked_salt[MQOM3_PARAM_SALT_SIZE];
	enc_ctx_pub DECL_VAR(ctx_enc);
	uint32_t k;
	uint8_t known;
	int ret = -1;

	while(ctx->sp > 0) {
		ctx->sp--;
		k     = ctx->stack[ctx->sp].k;
		known = ctx->stack[ctx->sp].known;
		if(known) {
			memcpy(seed, ctx->stack[ctx->sp].seed, MQOM3_PARAM_SEED_SIZE);
		}

		/* Non-hidden node with unknown seed: read from path (or expand if budget allows) */
		if((!known) && (!is_in_sorted(ctx->hidden_nodes, ctx->nb_hidden_nodes, k))) {
			if((ctx->size < (int32_t)MQOM3_PARAM_LARGE_GGM_T_OPEN) && (!IsLeaf(k))) {
				ctx->size++;
				/* Fall through with known=0: children inherit unknown, filled from path */
			} else {
				/* Read seed from path (mirrors the reveal in LargeGGMTree_ExpandPath).
				 * Same bound check as the DFS variant and GetNodeIndexesInPath:
				 * an overrun would stay inside the opening allocation, so no
				 * sanitizer would report it. */
				if(ctx->path_idx >= MQOM3_PARAM_LARGE_GGM_T_OPEN) {
					goto err;
				}
				memcpy(seed, ctx->path + ((size_t)ctx->path_idx * MQOM3_PARAM_SEED_SIZE), MQOM3_PARAM_SEED_SIZE);
				ctx->path_idx++;
				known = 1;
			}
		}

		if(!IsLeaf(k)) {
			if(known) {
				/* Derive children (push right then left, LIFO -> left first) */
				TweakSalt(ctx->salt, tweaked_salt, SALT_SEL_GGM, k);
				ret = enc_key_sched_pub(&ctx_enc, tweaked_salt);
				ERR(ret, err);
				ret = SeedDerive_pub(&ctx_enc, seed, left_seed);
				ERR(ret, err);
				ctx->stack[ctx->sp].k = (2 * k) + 1;
				xor_blocks(left_seed, seed, ctx->stack[ctx->sp].seed);
				ctx->stack[ctx->sp].known = 1;
				ctx->sp++;
				ctx->stack[ctx->sp].k = (2 * k);
				memcpy(ctx->stack[ctx->sp].seed, left_seed, MQOM3_PARAM_SEED_SIZE);
				ctx->stack[ctx->sp].known = 1;
				ctx->sp++;
			} else {
				/* Unknown seed: propagate to both children (right then left) */
				ctx->stack[ctx->sp].k = (2 * k) + 1;
				ctx->stack[ctx->sp].known = 0;
				ctx->sp++;
				ctx->stack[ctx->sp].k = (2 * k);
				ctx->stack[ctx->sp].known = 0;
				ctx->sp++;
			}
		} else {
			/* Leaf: known leaves yield their seed; hidden leaves (known=0) yield zeroes.
			 * The BLC layer handles hidden leaves via the hidden_index substitution. */
			ctx->num_leaf++;
			if(known) {
				memcpy(lseed_out, seed, MQOM3_PARAM_SEED_SIZE);
			} else {
				memset(lseed_out, 0, MQOM3_PARAM_SEED_SIZE);
			}
			enc_clean_ctx_pub(&ctx_enc);
			return 0;
		}
	}

	/* All leaves exhausted */
	enc_clean_ctx_pub(&ctx_enc);
	return 1;

err:
	enc_clean_ctx_pub(&ctx_enc);
	return -1;
}

void LargeGGMTree_CleanIncrementalPartialExpansion(large_ggmtree_incr_partial_ctx_t *ctx) {
	mqom_cleanse(ctx, sizeof(*ctx));
}

#else /* MQOM3_VARIANT_GUARD: CT build, nothing to compile here */
typedef int ggm_tree_large_incr_unused_in_ct_build;
#endif
