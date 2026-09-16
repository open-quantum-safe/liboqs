#ifndef __GGM_TREE_LARGE_BFS_H__
#define __GGM_TREE_LARGE_BFS_H__

#ifdef LARGE_TREE_BFS

#include "ggm_tree_large.h"

/* BFS level-by-level GGM tree expansion with x4 AES key-schedule batching.
 *
 * The DFS variants (LargeGGMTree_Expand / LargeGGMTree_ExpandLeavesOnly) pay
 * one enc_key_sched per node because every node has a distinct tweak (index k).
 * BFS processes all 2^j nodes at depth j simultaneously: they are independent,
 * so four are batched with enc_key_sched_x4 + SeedDerive_x4_x4, eliminating
 * the parent->child latency chain that stalls the DFS pipeline.
 *
 * LargeGGMTree_Expand_bfs    (KEEP_ALL path):
 *   Works in-place inside the caller-provided node[] array -- no extra alloc.
 *   Level j src/dst slots are disjoint in node[], so no aliasing issues.
 *
 * LargeGGMTree_ExpandLeavesOnly_bfs (no-keep path):
 *   Needs two rolling level buffers (cur/nxt, each BFS_MAX_LEVEL_NODES seeds).
 *   Too large for the stack (up to 256 KB for cat5); allocated via mqom_malloc.
 */

/* 2^(H-1): maximum number of nodes at a single BFS level (level H-1). */
#define BFS_MAX_LEVEL_NODES (1U << (LARGE_GGM_H - 1))

/* Number of internal (non-leaf) nodes at level H-1: those that have children
 * at depth H (the "deep" leaves).  Equals tauN - 2^(H-1). */
#define BFS_NB_INT_H1 (LARGE_GGM_NB_DEEP_LEAVES / 2)

#define LargeGGMTree_Expand_bfs           MQOM_NAMESPACE(LargeGGMTree_Expand_bfs)
#define LargeGGMTree_ExpandLeavesOnly_bfs MQOM_NAMESPACE(LargeGGMTree_ExpandLeavesOnly_bfs)
#define LargeGGMTree_ExpandPath_bfs       MQOM_NAMESPACE(LargeGGMTree_ExpandPath_bfs)
#define LargeGGMTree_PartiallyExpand_bfs  MQOM_NAMESPACE(LargeGGMTree_PartiallyExpand_bfs)

/* Alias all four public expansion names to BFS variants.
 * ggm_tree_large.h defines these names only under !LARGE_TREE_BFS, so these
 * defines introduce them for the first time here (BLC callers see only the public names). */
#define LargeGGMTree_Expand           LargeGGMTree_Expand_bfs
#define LargeGGMTree_ExpandLeavesOnly LargeGGMTree_ExpandLeavesOnly_bfs
#define LargeGGMTree_ExpandPath       LargeGGMTree_ExpandPath_bfs
#define LargeGGMTree_PartiallyExpand  LargeGGMTree_PartiallyExpand_bfs

/* Full-tree BFS (KEEP_ALL path): fills node[1..LARGE_GGM_TREE_SIZE-1] and lseed[].
 * Drop-in replacement for LargeGGMTree_Expand; no extra allocation. */
int LargeGGMTree_Expand_bfs(
        const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
        const uint8_t mseed[MQOM3_PARAM_SEED_SIZE],
        uint8_t node[LARGE_GGM_TREE_SIZE][MQOM3_PARAM_SEED_SIZE],
        uint8_t lseed[MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_SEED_SIZE]);

/* Leaves-only BFS (no-keep path): fills lseed[] without storing internal nodes.
 * Drop-in replacement for LargeGGMTree_ExpandLeavesOnly. */
int LargeGGMTree_ExpandLeavesOnly_bfs(
        const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
        const uint8_t mseed[MQOM3_PARAM_SEED_SIZE],
        uint8_t lseed[MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_SEED_SIZE]);

/* Open (no-keep path, BFS): re-expands from mseed using BFS x4 vectorization,
 * then extracts path seeds and hidden leaf seeds without a separate DFS pass.
 * Drop-in replacement for LargeGGMTree_ExpandPath; allocates node[] on the heap. */
int LargeGGMTree_ExpandPath_bfs(
        const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
        const uint8_t mseed[MQOM3_PARAM_SEED_SIZE],
        const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU],
        uint8_t path[MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE],
        uint8_t hidden_lseeds[MQOM3_PARAM_TAU][MQOM3_PARAM_SEED_SIZE]);

/* Partial expand (verifier, BFS): recovers all non-hidden leaf seeds from path using
 * BFS x4 AES vectorization.  Allocates node[] + assigned[] on the heap.
 * Drop-in replacement for LargeGGMTree_PartiallyExpand. */
int LargeGGMTree_PartiallyExpand_bfs(
        const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
        const uint8_t path[MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE],
        const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU],
        uint8_t lseed[MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_SEED_SIZE]);

#endif /* LARGE_TREE_BFS */

#endif /* __GGM_TREE_LARGE_BFS_H__ */
