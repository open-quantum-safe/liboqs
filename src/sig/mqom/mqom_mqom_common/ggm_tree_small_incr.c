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

#include "ggm_tree_small_incr.h"

int SmallGGMTree_InitIncrementalExpansion(small_ggmtree_ctx_t* ctx, const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t delta[MQOM3_PARAM_SEED_SIZE], uint32_t e) {
	uint32_t j;
	int ret = -1;
	uint8_t tweaked_salt[MQOM3_PARAM_SALT_SIZE];
	enc_ctx DECL_VAR(ctx_enc0);
	for (j = 1; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		if (j < GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY + 1) {
			TweakSalt(salt, ctx->tweaked_salts[j - 1], SALT_SEL_GGM, IndexIdentifier(e, j));
		} else {
			TweakSalt(salt, tweaked_salt, SALT_SEL_GGM, IndexIdentifier(e, j));
			ret = enc_key_sched(&ctx->ctx_enc[j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY], tweaked_salt);
			ERR(ret, err);
		}
	}
	memcpy(ctx->path[0], delta, MQOM3_PARAM_SEED_SIZE);
	/* Level 0: root derives via SeedDerive(TweakSalt(salt,2,IndexIdentifier(e,0)), mseed),
	 * uniformly with every other level above (replaces the removed TweakSeed
	 * primitive). One-shot, unlike the levels above which are cached/reused
	 * across many GetNextLeaf calls. */
	TweakSalt(salt, tweaked_salt, SALT_SEL_GGM, IndexIdentifier(e, 0));
	ret = enc_key_sched(&ctx_enc0, tweaked_salt);
	ERR(ret, err);
	ret = SeedDerive(&ctx_enc0, mseed, ctx->path[1]);
	ERR(ret, err);
	ctx->active = 0;
	ctx->e = e;

	ret = 0;
err:
	enc_clean_ctx(&ctx_enc0);
	return ret;
}

int SmallGGMTree_GetNextLeaf(small_ggmtree_ctx_t* ctx, uint8_t lseed[MQOM3_PARAM_SEED_SIZE]) {
	uint32_t j;
	int ret = -1;

	/* Exhaustion guard */
	if (ctx->active && ((ctx->num_leaf + 1) >= (MQOM3_PARAM_NB_EVALS))) {
		return -1;
	}

	if (ctx->active) {
		uint32_t new_num_leaf = ctx->num_leaf + 1;
		uint32_t diff = ctx->num_leaf ^ new_num_leaf;
		ctx->num_leaf = new_num_leaf;
		j = 1;
		while (((diff >> (MQOM3_PARAM_NB_EVALS_LOG - j)) & 0x1) == 0) {
			j++;
		}
		xor_blocks(ctx->path[j - 1], ctx->path[j], ctx->path[j]);
	} else {
		ctx->num_leaf = 0;
		ctx->active = 1;
		j = 1;
	}
	for (; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		if (j < GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY + 1) {
			enc_ctx DECL_VAR(ctx_enc);
			ret = enc_key_sched(&ctx_enc, ctx->tweaked_salts[j - 1]);
			ERR(ret, err1);
			ret = SeedDerive(&ctx_enc, ctx->path[j], ctx->path[j + 1]);
err1:
			enc_clean_ctx(&ctx_enc);
			ERR(ret, err);
		} else {
			enc_ctx* ctx_enc_precomputed = &ctx->ctx_enc[j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY];
			ret = SeedDerive(ctx_enc_precomputed, ctx->path[j], ctx->path[j + 1]);
			ERR(ret, err);
		}
	}
	memcpy(lseed, ctx->path[MQOM3_PARAM_NB_EVALS_LOG], MQOM3_PARAM_SEED_SIZE);

	ret = 0;
err:
	return ret;
}

int SmallGGMTree_InitIncrementalPartialExpansion(small_ggmtree_ctx_partial_t* ctx, const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t path[MQOM3_PARAM_NB_EVALS_LOG][MQOM3_PARAM_SEED_SIZE], uint32_t e, uint32_t i_star) {
	uint32_t j;
	int ret = -1;
	uint8_t tweaked_salt[MQOM3_PARAM_SALT_SIZE];

	/* Sanity check: same guard as SmallGGMTree_PartiallyExpand (ggm_tree_small.c).
	 * Without it, GetNextLeafPartial's unsigned subtraction/shift on ctx->i_star
	 * could underflow and index ctx->path/ctx->opening out of bounds. */
	if (i_star >= MQOM3_PARAM_NB_EVALS) {
		ret = -1;
		goto err;
	}

	for (j = 1; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		if (j < GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY + 1) {
			TweakSalt(salt, ctx->tweaked_salts[j - 1], SALT_SEL_GGM, IndexIdentifier(e, j));
		} else {
			TweakSalt(salt, tweaked_salt, SALT_SEL_GGM, IndexIdentifier(e, j));
			ret = enc_key_sched_pub(&ctx->ctx_enc[j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY], tweaked_salt);
			ERR(ret, err);
		}
	}
	memcpy((uint8_t*) ctx->opening, (uint8_t*) path, sizeof(ctx->opening));
	ctx->i_star = i_star;
	ctx->active = 0;

	ret = 0;
err:
	return ret;
}

int SmallGGMTree_GetNextLeafPartial(small_ggmtree_ctx_partial_t* ctx, uint8_t lseed[MQOM3_PARAM_SEED_SIZE]) {
	/* Exhaustion guard */
	if (ctx->active && ((ctx->num_leaf + 1) >= (MQOM3_PARAM_NB_EVALS))) {
		return -1;
	}

	uint32_t j;
	int ret = -1;

	if (ctx->active) {
		uint32_t new_num_leaf = ctx->num_leaf + 1;
		uint32_t diff = ctx->num_leaf ^ new_num_leaf;
		ctx->num_leaf = new_num_leaf;
		j = 1;
		while (((diff >> (MQOM3_PARAM_NB_EVALS_LOG - j)) & 0x1) == 0) {
			j++;
		}
	} else {
		ctx->num_leaf = 0;
		ctx->active = 1;
		j = 1;
	}
	uint32_t diff2 = ctx->num_leaf ^ ctx->i_star;
	if (diff2 == 0) {
		memset(lseed, 0, MQOM3_PARAM_SEED_SIZE);
	} else {
		uint32_t higher = 1;
		while (((diff2 >> (MQOM3_PARAM_NB_EVALS_LOG - higher)) & 0x1) == 0) {
			higher++;
		}
		if (j <= higher) {
			memcpy(ctx->path[higher], ctx->opening[MQOM3_PARAM_NB_EVALS_LOG - higher], MQOM3_PARAM_SEED_SIZE);
			j = higher;
		} else { // j > higher
			xor_blocks(ctx->path[j - 1], ctx->path[j], ctx->path[j]);
		}
		for (; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
			if (j < GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY + 1) {
				enc_ctx_pub DECL_VAR(ctx_enc);
				ret = enc_key_sched_pub(&ctx_enc, ctx->tweaked_salts[j - 1]);
				ERR(ret, err1);
				ret = SeedDerive_pub(&ctx_enc, ctx->path[j], ctx->path[j + 1]);
err1:
				enc_clean_ctx_pub(&ctx_enc);
				ERR(ret, err);
			} else {
				enc_ctx_pub* ctx_enc_precomputed = &ctx->ctx_enc[j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY];
				ret = SeedDerive_pub(ctx_enc_precomputed, ctx->path[j], ctx->path[j + 1]);
				ERR(ret, err);
			}
		}
		memcpy(lseed, ctx->path[MQOM3_PARAM_NB_EVALS_LOG], MQOM3_PARAM_SEED_SIZE);
	}

	ret = 0;
err:
	return ret;
}

#else /* MQOM3_VARIANT_GUARD: OT build, nothing to compile here */
typedef int ggm_tree_small_incr_unused_in_ot_build;
#endif
