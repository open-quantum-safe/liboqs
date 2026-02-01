#ifndef __GGM_TREE_H__
#define __GGM_TREE_H__

/* MQOM2 parameters */
#include "mqom2_parameters.h"
/* Encryption primitive */
#include "enc.h"
/* Common helpers */
#include "common.h"

/* Deal with namespacing */
#define GGMTree_Expand MQOM_NAMESPACE(GGMTree_Expand)
#define GGMTree_Open MQOM_NAMESPACE(GGMTree_Open)
#define GGMTree_PartiallyExpand MQOM_NAMESPACE(GGMTree_PartiallyExpand)
#define GGMTree_ExpandPath MQOM_NAMESPACE(GGMTree_ExpandPath)
#define GGMTree_InitIncrementalExpansion MQOM_NAMESPACE(GGMTree_InitIncrementalExpansion)
#define GGMTree_InitIncrementalExpansion_x2 MQOM_NAMESPACE(GGMTree_InitIncrementalExpansion_x2)
#define GGMTree_InitIncrementalExpansion_x4 MQOM_NAMESPACE(GGMTree_InitIncrementalExpansion_x4)
#define GGMTree_GetNextLeaf MQOM_NAMESPACE(GGMTree_GetNextLeaf)
#define GGMTree_GetNextLeaf_x2 MQOM_NAMESPACE(GGMTree_GetNextLeaf_x2)
#define GGMTree_GetNextLeaf_x4 MQOM_NAMESPACE(GGMTree_GetNextLeaf_x4)
#define GGMTree_InitIncrementalPartialExpansion MQOM_NAMESPACE(GGMTree_InitIncrementalPartialExpansion)
#define GGMTree_InitIncrementalPartialExpansion_x2 MQOM_NAMESPACE(GGMTree_InitIncrementalPartialExpansion_x2)
#define GGMTree_InitIncrementalPartialExpansion_x4 MQOM_NAMESPACE(GGMTree_InitIncrementalPartialExpansion_x4)
#define GGMTree_GetNextLeafPartial MQOM_NAMESPACE(GGMTree_GetNextLeafPartial)
#define GGMTree_GetNextLeafPartial_x2 MQOM_NAMESPACE(GGMTree_GetNextLeafPartial_x2)
#define GGMTree_GetNextLeafPartial_x4 MQOM_NAMESPACE(GGMTree_GetNextLeafPartial_x4)

int GGMTree_Expand(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t rseed[MQOM2_PARAM_SEED_SIZE], const uint8_t delta[MQOM2_PARAM_SEED_SIZE], uint32_t e, uint8_t node[MQOM2_PARAM_FULL_TREE_SIZE + 1][MQOM2_PARAM_SEED_SIZE], uint8_t lseed[MQOM2_PARAM_NB_EVALS][MQOM2_PARAM_SEED_SIZE]);

int GGMTree_Open(const uint8_t node[MQOM2_PARAM_FULL_TREE_SIZE + 1][MQOM2_PARAM_SEED_SIZE], uint32_t i_star, uint8_t path[MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE]);

int GGMTree_PartiallyExpand(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t path[MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE], uint32_t e, uint32_t i_star, uint8_t lseed[MQOM2_PARAM_NB_EVALS][MQOM2_PARAM_SEED_SIZE]);

int GGMTree_ExpandPath(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t rseed[MQOM2_PARAM_SEED_SIZE], const uint8_t delta[MQOM2_PARAM_SEED_SIZE], uint32_t e, uint32_t i_star, uint8_t path[MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE], uint8_t lseed[MQOM2_PARAM_SEED_SIZE]);

#if !defined(GGMTREE_NB_ENC_CTX_IN_MEMORY)
/* Default to 1 */
#define GGMTREE_NB_ENC_CTX_IN_MEMORY 1
#else
#if GGMTREE_NB_ENC_CTX_IN_MEMORY > (MQOM2_PARAM_NB_EVALS_LOG-2)
#error "GGMTREE_NB_ENC_CTX_IN_MEMORY should be smaller than (or equal to) MQOM2_PARAM_NB_EVALS_LOG-2"
#endif
#endif
#define GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY ((MQOM2_PARAM_NB_EVALS_LOG-1)-GGMTREE_NB_ENC_CTX_IN_MEMORY)

typedef struct {
	uint32_t active;
	uint32_t num_leaf;
	uint8_t tweaked_salts[GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY][MQOM2_PARAM_SEED_SIZE];
#if GGMTREE_NB_ENC_CTX_IN_MEMORY == 0
	/* Dummy value, not used */
	enc_ctx *ctx_enc;
#else
	enc_ctx ctx_enc[GGMTREE_NB_ENC_CTX_IN_MEMORY];
#endif
	uint8_t path[MQOM2_PARAM_NB_EVALS_LOG + 1][MQOM2_PARAM_SEED_SIZE];
} ggmtree_ctx_t;

/* Cleaning function */
static inline void ggmtree_ctx_t_clean(ggmtree_ctx_t *ctx) {
	(void)ctx;
#if GGMTREE_NB_ENC_CTX_IN_MEMORY > 0
	unsigned int i;
	for(i = 0; i < GGMTREE_NB_ENC_CTX_IN_MEMORY; i++){
		enc_clean_ctx(&ctx->ctx_enc[i]);
	}
#endif
}

int GGMTree_InitIncrementalExpansion(ggmtree_ctx_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t rseed[MQOM2_PARAM_SEED_SIZE], const uint8_t delta[MQOM2_PARAM_SEED_SIZE], uint32_t e);

int GGMTree_GetNextLeaf(ggmtree_ctx_t* ctx, uint8_t lseed[MQOM2_PARAM_SEED_SIZE]);

typedef struct {
	uint32_t active;
	uint32_t num_leaf;
	uint8_t tweaked_salts[2][GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY][MQOM2_PARAM_SEED_SIZE];
#if GGMTREE_NB_ENC_CTX_IN_MEMORY == 0
	/* Dummy value, not used */
	enc_ctx_x2 *ctx_enc_x2;
#else
	enc_ctx_x2 ctx_enc_x2[GGMTREE_NB_ENC_CTX_IN_MEMORY];
#endif
	uint8_t path[2][MQOM2_PARAM_NB_EVALS_LOG + 1][MQOM2_PARAM_SEED_SIZE];
} ggmtree_ctx_x2_t;

/* Cleaning function */
static inline void ggmtree_ctx_x2_t_clean(ggmtree_ctx_x2_t *ctx) {
	(void)ctx;
#if GGMTREE_NB_ENC_CTX_IN_MEMORY > 0
	unsigned int i;
	for(i = 0; i < GGMTREE_NB_ENC_CTX_IN_MEMORY; i++){
		enc_clean_ctx_x2(&ctx->ctx_enc_x2[i]);
	}
#endif
}

int GGMTree_InitIncrementalExpansion_x2(ggmtree_ctx_x2_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t rseed[2][MQOM2_PARAM_SEED_SIZE], const uint8_t delta[MQOM2_PARAM_SEED_SIZE], const uint32_t e[2]);

int GGMTree_GetNextLeaf_x2(ggmtree_ctx_x2_t* ctx, uint8_t lseed[2][MQOM2_PARAM_SEED_SIZE]);

typedef struct {
	uint32_t active;
	uint32_t num_leaf;
	uint8_t tweaked_salts[4][GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY][MQOM2_PARAM_SEED_SIZE];
#if GGMTREE_NB_ENC_CTX_IN_MEMORY == 0
	/* Dummy value, not used */
	enc_ctx_x4 *ctx_enc_x4;
#else
	enc_ctx_x4 ctx_enc_x4[GGMTREE_NB_ENC_CTX_IN_MEMORY];
#endif
	uint8_t path[4][MQOM2_PARAM_NB_EVALS_LOG + 1][MQOM2_PARAM_SEED_SIZE];
} ggmtree_ctx_x4_t;

/* Cleaning function */
static inline void ggmtree_ctx_x4_t_clean(ggmtree_ctx_x4_t *ctx) {
	(void)ctx;
#if GGMTREE_NB_ENC_CTX_IN_MEMORY > 0
	unsigned int i;
	for(i = 0; i < GGMTREE_NB_ENC_CTX_IN_MEMORY; i++){
		enc_clean_ctx_x4(&ctx->ctx_enc_x4[i]);
	}
#endif
}

int GGMTree_InitIncrementalExpansion_x4(ggmtree_ctx_x4_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t rseed[4][MQOM2_PARAM_SEED_SIZE], const uint8_t delta[MQOM2_PARAM_SEED_SIZE], const uint32_t e[4]);

int GGMTree_GetNextLeaf_x4(ggmtree_ctx_x4_t* ctx, uint8_t lseed[4][MQOM2_PARAM_SEED_SIZE]);

typedef struct {
	uint32_t active;
	uint32_t num_leaf;
	uint8_t tweaked_salts[GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY][MQOM2_PARAM_SEED_SIZE];
#if GGMTREE_NB_ENC_CTX_IN_MEMORY == 0
	/* Dummy value, not used */
	enc_ctx_pub *ctx_enc;
#else
	enc_ctx_pub ctx_enc[GGMTREE_NB_ENC_CTX_IN_MEMORY];
#endif
	uint8_t path[MQOM2_PARAM_NB_EVALS_LOG + 1][MQOM2_PARAM_SEED_SIZE];
	uint8_t opening[MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE];
	uint32_t i_star;
} ggmtree_ctx_partial_t;

/* Cleaning function */
static inline void ggmtree_ctx_partial_t_clean(ggmtree_ctx_partial_t *ctx) {
	(void)ctx;
#if GGMTREE_NB_ENC_CTX_IN_MEMORY > 0
	unsigned int i;
	for(i = 0; i < GGMTREE_NB_ENC_CTX_IN_MEMORY; i++){
		enc_clean_ctx_pub(&ctx->ctx_enc[i]);
	}
#endif
}

int GGMTree_InitIncrementalPartialExpansion(ggmtree_ctx_partial_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t path[MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE], uint32_t e, uint32_t i_star);

int GGMTree_GetNextLeafPartial(ggmtree_ctx_partial_t* ctx, uint8_t lseed[MQOM2_PARAM_SEED_SIZE]);

typedef struct {
	uint32_t active;
	uint32_t num_leaf;
	uint8_t tweaked_salts[2][GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY][MQOM2_PARAM_SEED_SIZE];
#if GGMTREE_NB_ENC_CTX_IN_MEMORY == 0
	/* Dummy value, not used */
	enc_ctx_pub **ctx_enc;
#else
	enc_ctx_pub ctx_enc[2][GGMTREE_NB_ENC_CTX_IN_MEMORY];
#endif
	uint8_t path[2][MQOM2_PARAM_NB_EVALS_LOG + 1][MQOM2_PARAM_SEED_SIZE];
	const uint8_t opening[2][MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE];
	uint32_t i_star[2];
} ggmtree_ctx_partial_x2_t;

/* Cleaning function */
static inline void ggmtree_ctx_partial_x2_t_clean(ggmtree_ctx_partial_x2_t *ctx) {
	(void)ctx;
#if GGMTREE_NB_ENC_CTX_IN_MEMORY > 0
	unsigned int i;
	for(i = 0; i < GGMTREE_NB_ENC_CTX_IN_MEMORY; i++){
		enc_clean_ctx_pub(&ctx->ctx_enc[0][i]);
		enc_clean_ctx_pub(&ctx->ctx_enc[1][i]);
	}
#endif
}

int GGMTree_InitIncrementalPartialExpansion_x2(ggmtree_ctx_partial_x2_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t (*path[2])[MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE], const uint32_t e[2], const uint32_t i_star[2]);

int GGMTree_GetNextLeafPartial_x2(ggmtree_ctx_partial_x2_t* ctx, uint8_t lseed[2][MQOM2_PARAM_SEED_SIZE]);

typedef struct {
	uint32_t active;
	uint32_t num_leaf;
	uint8_t tweaked_salts[4][GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY][MQOM2_PARAM_SEED_SIZE];
#if GGMTREE_NB_ENC_CTX_IN_MEMORY == 0
	/* Dummy value, not used */
	enc_ctx_pub **ctx_enc;
#else
	enc_ctx_pub ctx_enc[4][GGMTREE_NB_ENC_CTX_IN_MEMORY];
#endif
	uint8_t path[4][MQOM2_PARAM_NB_EVALS_LOG + 1][MQOM2_PARAM_SEED_SIZE];
	const uint8_t opening[4][MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE];
	uint32_t i_star[4];
} ggmtree_ctx_partial_x4_t;

/* Cleaning function */
static inline void ggmtree_ctx_partial_x4_t_clean(ggmtree_ctx_partial_x4_t *ctx) {
	(void)ctx;
#if GGMTREE_NB_ENC_CTX_IN_MEMORY > 0
	unsigned int i;
	for(i = 0; i < GGMTREE_NB_ENC_CTX_IN_MEMORY; i++){
		enc_clean_ctx_pub(&ctx->ctx_enc[0][i]);
		enc_clean_ctx_pub(&ctx->ctx_enc[1][i]);
		enc_clean_ctx_pub(&ctx->ctx_enc[2][i]);
		enc_clean_ctx_pub(&ctx->ctx_enc[3][i]);
	}
#endif
}

int GGMTree_InitIncrementalPartialExpansion_x4(ggmtree_ctx_partial_x4_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t (*path[4])[MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE], const uint32_t e[4], const uint32_t i_star[4]);

int GGMTree_GetNextLeafPartial_x4(ggmtree_ctx_partial_x4_t* ctx, uint8_t lseed[4][MQOM2_PARAM_SEED_SIZE]);

#endif /* __GGM_TREE_H__ */
