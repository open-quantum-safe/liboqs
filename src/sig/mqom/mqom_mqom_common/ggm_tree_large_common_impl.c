/* MQOM3_VARIANT_GUARD: this unit implements the OT variant only. Build systems
 * that compile every .c of a directory - SUPERCOP does, and so does any wildcard
 * Makefile - hand it to the compiler for CT instances too, where its parameters
 * simply do not exist. Rather than requiring every such build to filter its source
 * list, the unit reduces to a single typedef there. Not an empty file: ISO C forbids
 * an empty translation unit and -Wpedantic says so.
 * The in-tree Makefile still selects the right family, so nothing changes for it. */
#include "mqom3_parameters.h"
#if MQOM3_PARAM_OT_VARIANT == 1

/* Utility functions that are independent of the expansion strategy (DFS or BFS).
 *
 * LargeGGMIsValidOpeningSet: pure index arithmetic over hidden_nodes.
 * LargeGGMTree_Open:         memcpy from stored node[] using GetNodeIndexesInPath;
 *                            no AES derivation, valid under both DFS and BFS. */

#include "ggm_tree_large.h"
#include "ggm_tree_large_common.h"

void GetSensitiveNodeIndexes(const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU],
                             uint32_t hidden_nodes[MQOM3_PARAM_TAU * (MQOM3_PARAM_LARGE_GGM_H + 1)],
                             uint32_t *nb_hidden_nodes) {
	uint32_t e, j;
	int32_t merge_pos;
	uint32_t sorted_idxs[MQOM3_PARAM_TAU];
	uint32_t leaf_path[MQOM3_PARAM_LARGE_GGM_H + 1];

	/* Local copy then insertion sort */
	for(e = 0; e < MQOM3_PARAM_TAU; e++) {
		sorted_idxs[e] = hidden_leaf_idxs[e];
	}
	for(e = 1; e < MQOM3_PARAM_TAU; e++) {
		uint32_t key = sorted_idxs[e];
		int32_t k = (int32_t)e - 1;
		while((k >= 0) && (sorted_idxs[k] > key)) {
			sorted_idxs[k + 1] = sorted_idxs[k];
			k--;
		}
		sorted_idxs[k + 1] = key;
	}

	/* Initialize leaf_path to zero (no path stored yet) */
	for(j = 0; j <= MQOM3_PARAM_LARGE_GGM_H; j++) {
		leaf_path[j] = 0;
	}
	(*nb_hidden_nodes) = 0;

	for(e = 0; e < MQOM3_PARAM_TAU; e++) {
		uint32_t node_idx = LeafPosition(sorted_idxs[e]);
		uint32_t depth    = LeafDepth(sorted_idxs[e]);
		merge_pos = (int32_t)depth;

		/* Walk up from the leaf toward the root until we meet the stored path */
		while((merge_pos >= 0) && (leaf_path[merge_pos] != node_idx)) {
			leaf_path[merge_pos] = node_idx;
			node_idx >>= 1; /* go to parent */
			merge_pos--;
		}

		/* Append the new portion of the path (below the merge point) */
		for(j = (uint32_t)(merge_pos + 1); j <= depth; j++) {
			hidden_nodes[(*nb_hidden_nodes)++] = leaf_path[j];
		}
	}
}

int GetNodeIndexesInPath(const uint32_t *hidden_nodes, uint32_t nb_hidden_nodes,
                         uint32_t path_node_idxs[MQOM3_PARAM_LARGE_GGM_T_OPEN],
                         uint32_t *nb_path_nodes) {
	uint32_t sorted_hn[MQOM3_PARAM_TAU * (MQOM3_PARAM_LARGE_GGM_H + 1)];
	uint32_t stack[MQOM3_PARAM_LARGE_GGM_H + 1];
	uint32_t i, k, pos;
	int32_t size;
	int sp;

	size = ((int32_t)nb_hidden_nodes) - (2 * (int32_t)MQOM3_PARAM_TAU) + 1;
	if(size > (int32_t)MQOM3_PARAM_LARGE_GGM_T_OPEN) {
		return -1;
	}

	/* Sort for O(log |hn|) binary search during DFS */
	for(i = 0; i < nb_hidden_nodes; i++) {
		sorted_hn[i] = hidden_nodes[i];
	}
	sort_u32(sorted_hn, nb_hidden_nodes);

	/* Left-first DFS from root: push right then left (LIFO -> left popped first) */
	sp = 0;
	pos = 0;
	stack[sp++] = 1;

	while(sp > 0) {
		k = stack[--sp];

		if(!is_in_sorted(sorted_hn, nb_hidden_nodes, k)) {
			if((size < (int32_t)MQOM3_PARAM_LARGE_GGM_T_OPEN) && (!IsLeaf(k))) {
				size++;
				/* Fall through: expand this non-hidden internal node */
			} else {
				/* Reveal: record index, same order as ExpandPath/Open */
				if(pos >= MQOM3_PARAM_LARGE_GGM_T_OPEN) {
					return -1;
				}
				path_node_idxs[pos++] = k;
				continue;
			}
		}

		if(!IsLeaf(k)) {
			stack[sp++] = (2 * k) + 1;
			stack[sp++] = (2 * k);
		}
	}

	*nb_path_nodes = pos;
	return 0;
}

int LargeGGMIsValidOpeningSet(const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU]) {
	uint32_t hidden_nodes[MQOM3_PARAM_TAU * (MQOM3_PARAM_LARGE_GGM_H + 1)];
	uint32_t nb_hidden_nodes;
	int32_t size;

	GetSensitiveNodeIndexes(hidden_leaf_idxs, hidden_nodes, &nb_hidden_nodes);
	size = ((int32_t)nb_hidden_nodes) - (2 * (int32_t)MQOM3_PARAM_TAU) + 1;
	return (size <= (int32_t)MQOM3_PARAM_LARGE_GGM_T_OPEN);
}

int LargeGGMTree_Open(const uint8_t node[LARGE_GGM_TREE_SIZE][MQOM3_PARAM_SEED_SIZE], const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU], uint8_t path[MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE]) {
	/* Seeds are already in node[] (filled by Expand/Expand_bfs during KeyGen).
	 * GetNodeIndexesInPath reproduces the same traversal order as the verifier,
	 * so path entries are consumed in the same order by PartiallyExpand. */
	uint32_t hidden_nodes[MQOM3_PARAM_TAU * (MQOM3_PARAM_LARGE_GGM_H + 1)];
	uint32_t path_node_idxs[MQOM3_PARAM_LARGE_GGM_T_OPEN];
	uint32_t nb_hidden_nodes, nb_path_nodes, pos;
	int ret;

	GetSensitiveNodeIndexes(hidden_leaf_idxs, hidden_nodes, &nb_hidden_nodes);
	ret = GetNodeIndexesInPath(hidden_nodes, nb_hidden_nodes, path_node_idxs, &nb_path_nodes);
	if(ret) {
		return ret;
	}

	for(pos = 0; pos < nb_path_nodes; pos++) {
		memcpy(path + (pos * MQOM3_PARAM_SEED_SIZE), node[path_node_idxs[pos]], MQOM3_PARAM_SEED_SIZE);
	}
	/* Sanity check */
	if(nb_path_nodes != MQOM3_PARAM_LARGE_GGM_T_OPEN) {
		return -1;
	}
	return 0;
}

#else /* MQOM3_VARIANT_GUARD: CT build, nothing to compile here */
typedef int ggm_tree_large_common_impl_unused_in_ct_build;
#endif
