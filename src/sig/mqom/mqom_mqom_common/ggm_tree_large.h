#ifndef __GGM_TREE_LARGE_H__
#define __GGM_TREE_LARGE_H__

#include <stdint.h>
#include "mqom3_parameters.h"
#include "ggm_tree_common.h"

/* Total number of leaves in the large tree: tau * N */
#define LARGE_GGM_TREE_NB_LEAVES (MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS)
/* Total number of nodes: 2*tau*N - 1, array indexed 1..2*tau*N-1 */
#define LARGE_GGM_TREE_SIZE (2 * MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS)

/* h = ceil(log2(tau*N)), defined in OT parameter files via CEIL_LOG2 */
#define LARGE_GGM_H MQOM3_PARAM_LARGE_GGM_H

/* Number of "deep" leaves at depth h (others are at depth h-1) */
#define LARGE_GGM_NB_DEEP_LEAVES (LARGE_GGM_TREE_SIZE - (1 << LARGE_GGM_H))

/* LeafPosition(i): tree index of the i-th logical leaf (0-indexed).
 * Deep leaves (i < NB_DEEP_LEAVES) are at depth h, at positions 2^h + i.
 * Shallow leaves are at depth h-1, at positions tau*N + (i - NB_DEEP_LEAVES).
 * Constant-time: mask via arithmetic right-shift (SAR 31), no branch. */
static inline uint32_t LeafPosition(uint32_t i) {
	const uint32_t two_h = 1U << LARGE_GGM_H;
	const uint32_t M     = LARGE_GGM_TREE_NB_LEAVES;
	uint32_t mask = (uint32_t)((int32_t)(i - LARGE_GGM_NB_DEEP_LEAVES) >> 31);
	return two_h + i - (M & ~mask);
}

/* FromLeafPosition(k): logical leaf index of tree node k (k must be a leaf).
 * Constant-time: mask via arithmetic right-shift (SAR 31), no branch. */
static inline uint32_t FromLeafPosition(uint32_t k) {
	const uint32_t two_h = 1U << LARGE_GGM_H;
	const uint32_t M     = LARGE_GGM_TREE_NB_LEAVES;
	uint32_t mask = (uint32_t)((int32_t)(k - two_h) >> 31);
	return k - two_h + (M & mask);
}

/* LeafDepth(i): depth of the i-th logical leaf.
 * Constant-time: mask via arithmetic right-shift (SAR 31), no branch. */
static inline uint32_t LeafDepth(uint32_t i) {
	uint32_t mask = (uint32_t)((int32_t)(i - LARGE_GGM_NB_DEEP_LEAVES) >> 31);
	return LARGE_GGM_H - (~mask & 1U);
}

/* IsLeaf(k): true iff tree node k is a leaf (k >= tau*N). */
static inline int IsLeaf(uint32_t k) {
	return (k >= (uint32_t)(MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS));
}

/* Namespace macros for the common functions (always compiled in ggm_tree_large_common_impl.c). */
#define LargeGGMIsValidOpeningSet MQOM_NAMESPACE(LargeGGMIsValidOpeningSet)
#define LargeGGMTree_Open         MQOM_NAMESPACE(LargeGGMTree_Open)

/* Check that the opening set is within the T_open budget.
 * hidden_leaf_idxs[e]: absolute tree leaf index for execution e (= tau*i*[e] + e, computed by BLC). */
int LargeGGMIsValidOpeningSet(const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU]);

/* Open the large tree from a pre-stored node array (BLC_KEEP_ALL_TREES_IN_MEMORY path).
 * node[k]: seed of tree node k, as filled by LargeGGMTree_Expand.
 * hidden_leaf_idxs[e]: absolute tree leaf index for execution e.
 * path: serialized revealed node seeds (at most T_open * SEED_SIZE bytes).
 * Uses GetNodeIndexesInPath; returns -1 if the opening set violates the T_open budget. */
int LargeGGMTree_Open(const uint8_t node[LARGE_GGM_TREE_SIZE][MQOM3_PARAM_SEED_SIZE], const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU], uint8_t path[MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE]);

/* DFS and BFS variant headers: each is self-guarded by its own LARGE_TREE_BFS check.
 * They define the public expansion names (LargeGGMTree_Expand, ExpandLeavesOnly,
 * ExpandPath, PartiallyExpand) aliased to the appropriate implementation.
 * BLC callers include only this header and never reference LARGE_TREE_BFS directly. */
#include "ggm_tree_large_dfs.h"
#include "ggm_tree_large_bfs.h"

#endif /* __GGM_TREE_LARGE_H__ */
