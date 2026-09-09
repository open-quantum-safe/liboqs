#ifndef __GGM_TREE_SMALL_INCR_H__
#define __GGM_TREE_SMALL_INCR_H__

/* MQOM3 parameters */
#include "mqom3_parameters.h"
/* Encryption primitive */
#include "enc.h"
/* Common helpers */
#include "common.h"

#include "ggm_tree_common.h"

/* Deal with namespacing */
#define SmallGGMTree_InitIncrementalExpansion MQOM_NAMESPACE(SmallGGMTree_InitIncrementalExpansion)
#define SmallGGMTree_GetNextLeaf MQOM_NAMESPACE(SmallGGMTree_GetNextLeaf)
#define SmallGGMTree_InitIncrementalPartialExpansion MQOM_NAMESPACE(SmallGGMTree_InitIncrementalPartialExpansion)
#define SmallGGMTree_GetNextLeafPartial MQOM_NAMESPACE(SmallGGMTree_GetNextLeafPartial)

typedef struct {
	uint32_t active;
	uint32_t num_leaf;
	uint32_t e;
	uint8_t tweaked_salts[GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY][MQOM3_PARAM_SALT_SIZE];
#if GGMTREE_NB_ENC_CTX_IN_MEMORY == 0
	/* Dummy value, not used */
	enc_ctx *ctx_enc;
#else
	enc_ctx ctx_enc[GGMTREE_NB_ENC_CTX_IN_MEMORY];
#endif
	uint8_t path[MQOM3_PARAM_NB_EVALS_LOG + 1][MQOM3_PARAM_SEED_SIZE];
} small_ggmtree_ctx_t;

/* Cleaning function */
static inline void small_ggmtree_ctx_t_clean(small_ggmtree_ctx_t *ctx) {
#if GGMTREE_NB_ENC_CTX_IN_MEMORY > 0
	unsigned int i;
	for(i = 0; i < GGMTREE_NB_ENC_CTX_IN_MEMORY; i++){
		enc_clean_ctx(&ctx->ctx_enc[i]);
	}
#endif
	/* path/tweaked_salts hold secret GGM tree seed material (this is the
	 * non-pub, signer-side context walking the full secret tree). */
	mqom_cleanse(ctx->path, sizeof(ctx->path));
	mqom_cleanse(ctx->tweaked_salts, sizeof(ctx->tweaked_salts));
}
int SmallGGMTree_InitIncrementalExpansion(small_ggmtree_ctx_t* ctx, const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t delta[MQOM3_PARAM_SEED_SIZE], uint32_t e);

int SmallGGMTree_GetNextLeaf(small_ggmtree_ctx_t* ctx, uint8_t lseed[MQOM3_PARAM_SEED_SIZE]);

typedef struct {
	uint32_t active;
	uint32_t num_leaf;
	uint8_t tweaked_salts[GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY][MQOM3_PARAM_SALT_SIZE];
#if GGMTREE_NB_ENC_CTX_IN_MEMORY == 0
	/* Dummy value, not used */
	enc_ctx_pub *ctx_enc;
#else
	enc_ctx_pub ctx_enc[GGMTREE_NB_ENC_CTX_IN_MEMORY];
#endif
	uint8_t path[MQOM3_PARAM_NB_EVALS_LOG + 1][MQOM3_PARAM_SEED_SIZE];
	uint8_t opening[MQOM3_PARAM_NB_EVALS_LOG][MQOM3_PARAM_SEED_SIZE];
	uint32_t i_star;
} small_ggmtree_ctx_partial_t;

/* Cleaning function */
static inline void small_ggmtree_ctx_partial_t_clean(small_ggmtree_ctx_partial_t *ctx) {
	(void)ctx;
#if GGMTREE_NB_ENC_CTX_IN_MEMORY > 0
	unsigned int i;
	for(i = 0; i < GGMTREE_NB_ENC_CTX_IN_MEMORY; i++){
		enc_clean_ctx_pub(&ctx->ctx_enc[i]);
	}
#endif
}

int SmallGGMTree_InitIncrementalPartialExpansion(small_ggmtree_ctx_partial_t* ctx, const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t path[MQOM3_PARAM_NB_EVALS_LOG][MQOM3_PARAM_SEED_SIZE], uint32_t e, uint32_t i_star);

int SmallGGMTree_GetNextLeafPartial(small_ggmtree_ctx_partial_t* ctx, uint8_t lseed[MQOM3_PARAM_SEED_SIZE]);


#endif /* __GGM_TREE_SMALL_INCR_H__ */
