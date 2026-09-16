#ifndef __GGM_TREE_LARGE_COMMON_H__
#define __GGM_TREE_LARGE_COMMON_H__

/* Internal helpers shared across ggm_tree_large_*.c translation units.
 * Inline utilities (is_in_sorted, sort_u32) are defined here.
 * Non-trivial helpers (GetSensitiveNodeIndexes, GetNodeIndexesInPath) are
 * declared here and defined once in ggm_tree_large_common_impl.c. */

#include "ggm_tree_large.h"

/* Binary search of v in sorted array arr[0..n-1]. */
static inline int is_in_sorted(const uint32_t *arr, uint32_t n, uint32_t v) {
	int32_t lo = 0, hi = (int32_t)n - 1;
	while(lo <= hi) {
		int32_t mid = ((lo + hi) >> 1);
		if(arr[mid] == v) {
			return 1;
		} else if(arr[mid] < v) {
			lo = mid + 1;
		} else {
			hi = mid - 1;
		}
	}
	return 0;
}

/* Insertion sort of arr[0..n-1] in place. */
static inline void sort_u32(uint32_t *arr, uint32_t n) {
	uint32_t i;
	for(i = 1; i < n; i++) {
		uint32_t key = arr[i];
		int32_t j = (int32_t)i - 1;
		while((j >= 0) && (arr[j] > key)) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = key;
	}
}

/* Deal with namespacing */
#define GetSensitiveNodeIndexes MQOM_NAMESPACE(GetSensitiveNodeIndexes)
#define GetNodeIndexesInPath    MQOM_NAMESPACE(GetNodeIndexesInPath)

/* GetSensitiveNodeIndexes: collect tree node indices on the hidden-leaf paths.
 * Takes absolute tree leaf indices (hidden_leaf_idxs[e] = tau*i*[e] + e, computed by the BLC layer).
 * Spec bugs fixed: merge_pos decrements (not increments), node_idx is halved at each step.
 * The column-major BLC indexing (tau*i+e) means inputs are NOT necessarily sorted,
 * so an insertion sort on a local copy is applied before processing.
 * Defined in ggm_tree_large_common_impl.c. */
void GetSensitiveNodeIndexes(const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU],
                             uint32_t hidden_nodes[MQOM3_PARAM_TAU * (MQOM3_PARAM_LARGE_GGM_H + 1)],
                             uint32_t *nb_hidden_nodes);

/* GetNodeIndexesInPath: compute the ordered list of tree node indices to reveal.
 * Mirrors the spec algorithm: DFS from root, same order as ExpandPath/Open.
 * Assumption (spec): size = |hidden_nodes| - 2*tau + 1 <= T_open; returns -1 if violated.
 * path_node_idxs must have space for MQOM3_PARAM_LARGE_GGM_T_OPEN entries.
 * *nb_path_nodes receives the actual count (<= T_open).
 * Defined in ggm_tree_large_common_impl.c. */
int GetNodeIndexesInPath(const uint32_t *hidden_nodes, uint32_t nb_hidden_nodes,
                         uint32_t path_node_idxs[MQOM3_PARAM_LARGE_GGM_T_OPEN],
                         uint32_t *nb_path_nodes);

#endif /* __GGM_TREE_LARGE_COMMON_H__ */
