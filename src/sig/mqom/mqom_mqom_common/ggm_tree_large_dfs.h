#ifndef __GGM_TREE_LARGE_DFS_H__
#define __GGM_TREE_LARGE_DFS_H__

/* DFS variant of the large GGM tree operations for the OT (One-Tree) variant.
 *
 * All four _dfs-suffixed symbols are always compiled by ggm_tree_large_dfs.c and
 * declared here unconditionally, regardless of LARGE_TREE_BFS: every one of them
 * uses O(H) stack memory (H = tree height), no full node[LARGE_GGM_TREE_SIZE]
 * allocation. This lets memory-constrained callers (BLC memopt) call them by their
 * _dfs-suffixed name explicitly and always get the low-memory implementation, even
 * in builds where LARGE_TREE_BFS is set - see blc_memopt_ot.c's use of
 * LargeGGMTree_ExpandPath_dfs, which would otherwise (via the generic name below)
 * silently get the BFS variant's full-tree heap allocation, defeating memopt's
 * point. [Round-3 fix: the BFS default introduced for OT made this a real gap -
 * ExpandPath was the one large-tree operation memopt didn't route through its own
 * incremental iterator, so it was the only one still exposed to the BFS tradeoff.]
 *
 * Under !LARGE_TREE_BFS the public names (LargeGGMTree_Expand, etc.) are aliased to
 * these DFS implementations, for blc_default_ot.c. Under LARGE_TREE_BFS they are
 * aliased to the BFS variants in ggm_tree_large_bfs.h instead (faster, but O(tree
 * size) memory) - appropriate there since blc_default_ot.c already keeps the full
 * tree in memory or doesn't need the memory-saving property memopt cares about. */

#include "ggm_tree_large.h"

/* Namespace macros for the DFS-specific symbol names (always needed, always compiled). */
#define LargeGGMTree_Expand_dfs           MQOM_NAMESPACE(LargeGGMTree_Expand_dfs)
#define LargeGGMTree_ExpandLeavesOnly_dfs MQOM_NAMESPACE(LargeGGMTree_ExpandLeavesOnly_dfs)
#define LargeGGMTree_ExpandPath_dfs       MQOM_NAMESPACE(LargeGGMTree_ExpandPath_dfs)
#define LargeGGMTree_PartiallyExpand_dfs  MQOM_NAMESPACE(LargeGGMTree_PartiallyExpand_dfs)

/* Expand the large GGM tree from a master seed (DFS, O(1) extra memory beyond node[]).
 * node[k]: caller-provided buffer for all tree nodes (1-indexed, size LARGE_GGM_TREE_SIZE).
 * lseed[i]: logical leaf seed for leaf i; BLC re-indexes as lseed_all[tau*i + e]. */
int LargeGGMTree_Expand_dfs(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], uint8_t node[LARGE_GGM_TREE_SIZE][MQOM3_PARAM_SEED_SIZE], uint8_t lseed[MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_SEED_SIZE]);

/* Expand only the leaf seeds using an incremental DFS (O(H) stack, no full node buffer).
 * Equivalent to LargeGGMTree_Expand but avoids the LARGE_GGM_TREE_SIZE*SEED_SIZE allocation.
 * lseed[i]: logical leaf seed for leaf i, written in order 0..tau*NB_EVALS-1. */
int LargeGGMTree_ExpandLeavesOnly_dfs(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], uint8_t lseed[MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_SEED_SIZE]);

/* Re-expand from mseed to produce path + hidden leaf seeds (no-KEEP signer path, O(H) stack). */
int LargeGGMTree_ExpandPath_dfs(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU], uint8_t path[MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE], uint8_t hidden_lseeds[MQOM3_PARAM_TAU][MQOM3_PARAM_SEED_SIZE]);

/* Recover all non-hidden leaf seeds from path (verifier side, O(H) stack).
 * lseed[i]: filled for all logical leaves except those in hidden_leaf_idxs (zeroed). */
int LargeGGMTree_PartiallyExpand_dfs(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t path[MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE], const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU], uint8_t lseed[MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_SEED_SIZE]);

#ifndef LARGE_TREE_BFS
/* Public-name aliases: map generic names to DFS implementations (blc_default_ot.c). */
#define LargeGGMTree_Expand           LargeGGMTree_Expand_dfs
#define LargeGGMTree_ExpandLeavesOnly LargeGGMTree_ExpandLeavesOnly_dfs
#define LargeGGMTree_ExpandPath       LargeGGMTree_ExpandPath_dfs
#define LargeGGMTree_PartiallyExpand  LargeGGMTree_PartiallyExpand_dfs
#endif /* !LARGE_TREE_BFS */

#endif /* __GGM_TREE_LARGE_DFS_H__ */
