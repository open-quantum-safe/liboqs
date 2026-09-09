#ifndef __GGM_TREE_LARGE_INCR_H__
#define __GGM_TREE_LARGE_INCR_H__

#include <stdint.h>
#include "mqom3_parameters.h"
#include "ggm_tree_large.h"

/* Pull-based left-first DFS iterator for the large GGM tree.
 * Yields all TAU*NB_EVALS leaf seeds in logical order 0..TAU*NB_EVALS-1.
 * Peak stack depth: LARGE_GGM_H+1 entries -> O(H*(4+SEED_SIZE)) bytes, no heap. */
typedef struct {
	struct {
		uint32_t k;
		uint8_t seed[MQOM3_PARAM_SEED_SIZE];
	} stack[MQOM3_PARAM_LARGE_GGM_H + 2];
	int sp;
	uint32_t num_leaf;
	uint8_t salt[MQOM3_PARAM_SALT_SIZE];
} large_ggmtree_incr_ctx_t;

/* Pull-based left-first DFS iterator for partial expansion (verifier side).
 * Mirrors LargeGGMTree_PartiallyExpand but yields one leaf at a time.
 * Hidden leaves yield a zero seed; non-hidden leaves yield their derived seed.
 * Path seeds are consumed in the same left-first order as written by LargeGGMTree_ExpandPath. */
typedef struct {
	struct {
		uint32_t k;
		uint8_t seed[MQOM3_PARAM_SEED_SIZE];
		uint8_t known;
	} stack[MQOM3_PARAM_LARGE_GGM_H + 2];
	int sp;
	uint32_t num_leaf;
	uint8_t salt[MQOM3_PARAM_SALT_SIZE];
	/* Hidden-node tracking (mirrors LargeGGMTree_PartiallyExpand). Sorted
	 * in place for O(log |hn|) binary search - the unsorted insertion
	 * order is never read again after GetSensitiveNodeIndexes populates
	 * this, so no separate sorted_hn copy is needed. */
	uint32_t hidden_nodes[MQOM3_PARAM_TAU * (MQOM3_PARAM_LARGE_GGM_H + 1)];
	uint32_t nb_hidden_nodes;
	const uint8_t *path;
	uint32_t path_idx;
	int32_t size;
} large_ggmtree_incr_partial_ctx_t;

/* Deal with namespacing */
#define LargeGGMTree_InitIncrementalExpansion        MQOM_NAMESPACE(LargeGGMTree_InitIncrementalExpansion)
#define LargeGGMTree_GetNextLeaf                     MQOM_NAMESPACE(LargeGGMTree_GetNextLeaf)
#define LargeGGMTree_CleanIncrementalExpansion       MQOM_NAMESPACE(LargeGGMTree_CleanIncrementalExpansion)
#define LargeGGMTree_InitIncrementalPartialExpansion MQOM_NAMESPACE(LargeGGMTree_InitIncrementalPartialExpansion)
#define LargeGGMTree_GetNextLeafPartial              MQOM_NAMESPACE(LargeGGMTree_GetNextLeafPartial)
#define LargeGGMTree_CleanIncrementalPartialExpansion MQOM_NAMESPACE(LargeGGMTree_CleanIncrementalPartialExpansion)

/* Initialize a full-expansion iterator. Pushes root with mseed; ready for GetNextLeaf calls. */
int LargeGGMTree_InitIncrementalExpansion(large_ggmtree_incr_ctx_t *ctx,
        const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
        const uint8_t mseed[MQOM3_PARAM_SEED_SIZE]);

/* Derive and return the next leaf seed in logical order.
 * Returns 0: leaf written to lseed_out. Returns 1: all leaves exhausted. Returns -1: error. */
int LargeGGMTree_GetNextLeaf(large_ggmtree_incr_ctx_t *ctx,
        uint8_t lseed_out[MQOM3_PARAM_SEED_SIZE]);

/* Zero-out the iterator context (secure erasure of key material). */
void LargeGGMTree_CleanIncrementalExpansion(large_ggmtree_incr_ctx_t *ctx);

/* Initialize a partial-expansion iterator (verifier side).
 * hidden_leaf_idxs[e] = tau*i_star[e] + e (column-major BLC indexing).
 * path must remain valid for the lifetime of ctx (pointer is stored, not copied). */
int LargeGGMTree_InitIncrementalPartialExpansion(large_ggmtree_incr_partial_ctx_t *ctx,
        const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
        const uint8_t path[MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE],
        const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU]);

/* Return next leaf seed; hidden leaves return a zero-filled seed.
 * Returns 0: leaf written. Returns 1: all leaves done. Returns -1: error. */
int LargeGGMTree_GetNextLeafPartial(large_ggmtree_incr_partial_ctx_t *ctx,
        uint8_t lseed_out[MQOM3_PARAM_SEED_SIZE]);

/* Zero-out the partial-expansion iterator context. */
void LargeGGMTree_CleanIncrementalPartialExpansion(large_ggmtree_incr_partial_ctx_t *ctx);

#endif /* __GGM_TREE_LARGE_INCR_H__ */
