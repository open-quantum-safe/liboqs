#ifndef __GGM_TREE_LARGE_INCR_BATCH_H__
#define __GGM_TREE_LARGE_INCR_BATCH_H__

#include <stdint.h>
#include "mqom3_parameters.h"
#include "ggm_tree_large.h"

/* Optional x2-batched variant of the memopt (incremental DFS) large-tree
 * traversal used by OT. The plain ggm_tree_large_incr.c derives exactly one
 * tree node at a time (one enc_key_sched + SeedDerive call per internal
 * node). This batches 2 independently-keyed nodes together per derivation
 * step, structurally equivalent to MQOM2's onetree design
 * (ggm_tree_multi_2.c), though re-derived independently rather than copying
 * its exact index arithmetic (which uses a confusing -1 offset convention
 * this implementation avoids by keying every node directly by its own
 * index, MQOM3's native convention).
 */
/* Sign and Verify are switched independently, because the trade-off is not
 * the same on the two sides. Sign drives the constant-time (bitsliced)
 * cipher, where pairing two derivations into one x2 call is a clear win
 * (board-measured -6% to -22% cycles). Verify drives the public cipher,
 * which has no multi-key parallelism to exploit: batching there buys
 * nothing measurable (+0.1% to +0.5% cycles, i.e. noise or slightly worse)
 * while the partial context has to carry BOTH the single-node outer stack -
 * the path/hidden-node resolution cannot be batched, path_idx being a shared
 * counter consumed in a fixed order - AND the inner pair stack, costing up
 * to ~1.3 KB of stack. Hence LARGE_GGM_TREE_INCR_BATCH_VERIFY defaults to 0
 * even when the Sign-side switch is on. The Verify path is kept (rather than
 * deleted) so a future public backend with genuine x2 batching - a pipelined
 * hardware accelerator, a bitsliced public backend - only needs the flag
 * flipped and re-measured. */
#ifndef LARGE_GGM_TREE_INCR_BATCH
#define LARGE_GGM_TREE_INCR_BATCH 0
#endif
#if (LARGE_GGM_TREE_INCR_BATCH != 0) && (LARGE_GGM_TREE_INCR_BATCH != 1)
#error "LARGE_GGM_TREE_INCR_BATCH must be 0 (off) or 1 (on)"
#endif

#ifndef LARGE_GGM_TREE_INCR_BATCH_VERIFY
#define LARGE_GGM_TREE_INCR_BATCH_VERIFY 0
#endif
#if (LARGE_GGM_TREE_INCR_BATCH_VERIFY != 0) && (LARGE_GGM_TREE_INCR_BATCH_VERIFY != 1)
#error "LARGE_GGM_TREE_INCR_BATCH_VERIFY must be 0 (off) or 1 (on)"
#endif

/* Shared by both sides */
#define LARGE_GGM_TREE_INCR_BATCH_ANY \
	((LARGE_GGM_TREE_INCR_BATCH == 1) || (LARGE_GGM_TREE_INCR_BATCH_VERIFY == 1))

#if LARGE_GGM_TREE_INCR_BATCH_ANY

/* One stack frame: two independent, already-known pending nodes. */
typedef struct {
	uint32_t kA;
	uint8_t seedA[MQOM3_PARAM_SEED_SIZE];
	uint32_t kB;
	uint8_t seedB[MQOM3_PARAM_SEED_SIZE];
} large_ggm_pair_frame_t;

#if LARGE_GGM_TREE_INCR_BATCH == 1
/* Sign/commit side: the whole tree is "known" from mseed, so a single
 * pair-stack traversal (bootstrap node 1's own two children via one
 * single-key step, then batch from there) covers everything. Leaves land in
 * buf, drained one per GetNextLeaf_batch call. */
typedef struct {
	large_ggm_pair_frame_t stack[MQOM3_PARAM_LARGE_GGM_H + 2];
	int sp;
	uint32_t num_leaf;
	uint8_t salt[MQOM3_PARAM_SALT_SIZE];
	uint8_t buf[2][MQOM3_PARAM_SEED_SIZE];
	uint32_t buf_count;
	uint32_t buf_pos;
} large_ggmtree_incr_batch_ctx_t;
#endif /* LARGE_GGM_TREE_INCR_BATCH == 1 */

#if LARGE_GGM_TREE_INCR_BATCH_VERIFY == 1

/* Verify/eval side: keeps the plain single-node, order-sensitive outer stack
 * (hidden-leaf / path-read resolution - path_idx is a single shared counter
 * consumed in a fixed order, so this part can't be batched or reordered).
 * The moment a node resolves to known+non-leaf, its whole subtree is
 * guaranteed hidden-free (see ggm_tree_large_incr_batch.c) and gets handed
 * to the same pair-stack engine Sign uses, via the inner stack below. */
typedef struct {
	struct {
		uint32_t k;
		uint8_t seed[MQOM3_PARAM_SEED_SIZE];
		uint8_t known;
	} outer_stack[MQOM3_PARAM_LARGE_GGM_H + 2];
	int outer_sp;
	large_ggm_pair_frame_t inner_stack[MQOM3_PARAM_LARGE_GGM_H + 2];
	int inner_sp;
	uint32_t num_leaf;
	uint8_t salt[MQOM3_PARAM_SALT_SIZE];
	uint32_t hidden_nodes[MQOM3_PARAM_TAU * (MQOM3_PARAM_LARGE_GGM_H + 1)];
	uint32_t nb_hidden_nodes;
	const uint8_t *path;
	uint32_t path_idx;
	int32_t size;
	uint8_t buf[2][MQOM3_PARAM_SEED_SIZE];
	uint32_t buf_count;
	uint32_t buf_pos;
} large_ggmtree_incr_batch_partial_ctx_t;
#endif /* LARGE_GGM_TREE_INCR_BATCH_VERIFY == 1 */

/* Deal with namespacing */
#if LARGE_GGM_TREE_INCR_BATCH == 1
#define LargeGGMTree_InitIncrementalExpansion_batch         MQOM_NAMESPACE(LargeGGMTree_InitIncrementalExpansion_batch)
#define LargeGGMTree_GetNextLeaf_batch                      MQOM_NAMESPACE(LargeGGMTree_GetNextLeaf_batch)
#define LargeGGMTree_CleanIncrementalExpansion_batch        MQOM_NAMESPACE(LargeGGMTree_CleanIncrementalExpansion_batch)

int LargeGGMTree_InitIncrementalExpansion_batch(large_ggmtree_incr_batch_ctx_t *ctx,
        const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
        const uint8_t mseed[MQOM3_PARAM_SEED_SIZE]);

int LargeGGMTree_GetNextLeaf_batch(large_ggmtree_incr_batch_ctx_t *ctx,
        uint8_t lseed_out[MQOM3_PARAM_SEED_SIZE]);

void LargeGGMTree_CleanIncrementalExpansion_batch(large_ggmtree_incr_batch_ctx_t *ctx);
#endif /* LARGE_GGM_TREE_INCR_BATCH == 1 */

#if LARGE_GGM_TREE_INCR_BATCH_VERIFY == 1
#define LargeGGMTree_InitIncrementalPartialExpansion_batch  MQOM_NAMESPACE(LargeGGMTree_InitIncrementalPartialExpansion_batch)
#define LargeGGMTree_GetNextLeafPartial_batch               MQOM_NAMESPACE(LargeGGMTree_GetNextLeafPartial_batch)
#define LargeGGMTree_CleanIncrementalPartialExpansion_batch MQOM_NAMESPACE(LargeGGMTree_CleanIncrementalPartialExpansion_batch)

int LargeGGMTree_InitIncrementalPartialExpansion_batch(large_ggmtree_incr_batch_partial_ctx_t *ctx,
        const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
        const uint8_t path[MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE],
        const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU]);

int LargeGGMTree_GetNextLeafPartial_batch(large_ggmtree_incr_batch_partial_ctx_t *ctx,
        uint8_t lseed_out[MQOM3_PARAM_SEED_SIZE]);

void LargeGGMTree_CleanIncrementalPartialExpansion_batch(large_ggmtree_incr_batch_partial_ctx_t *ctx);
#endif /* LARGE_GGM_TREE_INCR_BATCH_VERIFY == 1 */

#endif /* LARGE_GGM_TREE_INCR_BATCH_ANY */

#endif /* __GGM_TREE_LARGE_INCR_BATCH_H__ */
