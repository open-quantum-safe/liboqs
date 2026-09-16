#ifndef __GGM_TREE_SMALL_INCR_BATCH_H__
#define __GGM_TREE_SMALL_INCR_BATCH_H__

/* MQOM3 parameters */
#include "mqom3_parameters.h"
/* Encryption primitive */
#include "enc.h"
/* Common helpers */
#include "common.h"

#include "ggm_tree_common.h"

#ifndef SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES_LOG
#define SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES_LOG 4
#endif

#if SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES_LOG >= MQOM3_PARAM_NB_EVALS_LOG
#error "SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES_LOG should be smaller than MQOM3_PARAM_NB_EVALS_LOG"
#endif
/* Lower bound: the subtree expansion needs at least two leaves. */
#if SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES_LOG < 1
#error "SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES_LOG should be at least 1"
#endif

#define SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES (1<<SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES_LOG)

/* Deal with namespacing */
#define SmallGGMTree_InitIncrementalExpansion_batch MQOM_NAMESPACE(SmallGGMTree_InitIncrementalExpansion_batch)
#define SmallGGMTree_GetNextLeafs_batch MQOM_NAMESPACE(SmallGGMTree_GetNextLeafs_batch)
#define SmallGGMTree_InitIncrementalPartialExpansion_batch MQOM_NAMESPACE(SmallGGMTree_InitIncrementalPartialExpansion_batch)
#define SmallGGMTree_GetNextLeafsPartial_batch MQOM_NAMESPACE(SmallGGMTree_GetNextLeafsPartial_batch)

typedef struct {
	uint32_t active;
	uint32_t num_leaf;
	uint32_t e;
	const uint8_t* salt;
#if GGMTREE_NB_ENC_CTX_IN_MEMORY == 0
	/* Dummy value, not used */
	enc_ctx_ecb *ctx_enc;
#else
	enc_ctx_ecb ctx_enc[GGMTREE_NB_ENC_CTX_IN_MEMORY];
#endif
	uint8_t path[MQOM3_PARAM_NB_EVALS_LOG-SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES_LOG+1][MQOM3_PARAM_SEED_SIZE];
} small_ggmtree_ctx_batch_t;

/* Cleaning function */
static inline void small_ggmtree_ctx_batch_t_clean(small_ggmtree_ctx_batch_t *ctx) {
#if GGMTREE_NB_ENC_CTX_IN_MEMORY > 0
	unsigned int i;
	for(i = 0; i < GGMTREE_NB_ENC_CTX_IN_MEMORY; i++) {
		enc_clean_ctx_ecb(&ctx->ctx_enc[i]);
	}
#endif
	/* path holds secret GGM tree seed material (this is the non-pub,
	 * signer-side context walking the full secret tree). */
	mqom_cleanse(ctx->path, sizeof(ctx->path));
}
int SmallGGMTree_InitIncrementalExpansion_batch(small_ggmtree_ctx_batch_t* ctx, const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t delta[MQOM3_PARAM_SEED_SIZE], uint32_t e);

int SmallGGMTree_GetNextLeafs_batch(small_ggmtree_ctx_batch_t* ctx, uint8_t lseeds[][MQOM3_PARAM_SEED_SIZE]);

typedef struct {
	uint32_t active;
	uint32_t num_leaf;
	uint32_t e;
	const uint8_t* salt;
	uint32_t i_star;
#if GGMTREE_NB_ENC_CTX_IN_MEMORY == 0
	/* Dummy value, not used */
	enc_ctx_pub_ecb *ctx_enc;
#else
	enc_ctx_pub_ecb ctx_enc[GGMTREE_NB_ENC_CTX_IN_MEMORY];
#endif
	uint8_t path[MQOM3_PARAM_NB_EVALS_LOG-SMALL_GGM_TREE_NB_SIMULTANEOUS_LEAVES_LOG+1][MQOM3_PARAM_SEED_SIZE];
	uint8_t opening[MQOM3_PARAM_NB_EVALS_LOG][MQOM3_PARAM_SEED_SIZE];
} small_ggmtree_ctx_partial_batch_t;

/* Cleaning function */
static inline void small_ggmtree_ctx_partial_batch_t_clean(small_ggmtree_ctx_partial_batch_t *ctx) {
	(void)ctx;
#if GGMTREE_NB_ENC_CTX_IN_MEMORY > 0
	unsigned int i;
	for(i = 0; i < GGMTREE_NB_ENC_CTX_IN_MEMORY; i++){
		enc_clean_ctx_pub_ecb(&ctx->ctx_enc[i]);
	}
#endif
}

int SmallGGMTree_InitIncrementalPartialExpansion_batch(small_ggmtree_ctx_partial_batch_t* ctx, const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t path[MQOM3_PARAM_NB_EVALS_LOG][MQOM3_PARAM_SEED_SIZE], uint32_t e, uint32_t i_star);

int SmallGGMTree_GetNextLeafsPartial_batch(small_ggmtree_ctx_partial_batch_t* ctx, uint8_t lseeds[][MQOM3_PARAM_SEED_SIZE]);

#endif /* __GGM_TREE_SMALL_INCR_BATCH_H__ */
