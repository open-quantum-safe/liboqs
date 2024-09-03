/*
 * Reference ISO-C11 Implementation of CROSS.
 *
 * @version 1.1 (May 2023)
 *
 * @author: Patrick Karl <patrick.karl@tum.de>
 *
 * This code is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <string.h>

#include "merkle.h"

#define LEAVES_FULL_TREE(L) ( (1UL << LOG2(L) ) )
#define LEAVES_HALF_TREE(L) ( (LEAVES_FULL_TREE(L) >> 1) )

#define PARENT(i) ( ((i)%2) ? (((i)-1)/2) : (((i)-2)/2) )
#define RIGHT_CHILD(i) ( (2*(i)+2) )
#define LEFT_CHILD(i) ( (2*(i)+1) )
#define SIBLING(i) ( ((i)%2) ? (i)+1 : (i)-1 )

#define RL(i) ((i)==1 ? r_node : l_node)
#define OFFSET(i) ( (i)*HASH_DIGEST_LENGTH )

#define CHALLENGE_PROOF_VALUE 0
#define INVALID_MERKLE_NODE 0
#define VALID_MERKLE_NODE 1

#define NOT_COMPUTED 0
#define COMPUTED 1

/*
 * setup_tree()
 *
 * uint16_t layer_offset[LOG2(T)+1]    :   Stores one offset per layer for layer change.
 *                                      Required for the computation of PARENT and CHILD nodes.
 * uint16_t nodes_per_layer[LOG2(T)+1] :   Stores the numbers of nodes used in the truncated Merkle tree.
 */
static
void setup_tree(uint16_t layer_offsets[LOG2(T) + 1],
                uint16_t nodes_per_layer[LOG2(T) + 1]) {
	uint32_t depth, layer;
	uint32_t r_leaves;
	int subtree_found;

	/* Initialize array with full node counts */
	for (size_t i = 0; i < LOG2(T) + 1; i++) {
		layer_offsets[i] = (1UL << i);
	}

	/* Count root node */
	layer = 0;
	layer_offsets[layer] -= 1;

	/* Count left tree nodes (always full) */
	for (size_t i = 1; i < LOG2(T) + 1; i++) {
		layer_offsets[i] -= (1UL << (i - 1));
	}

	/* Check every full subtree on right side and subtract missing nodes */
	r_leaves    = T - (1UL << (LOG2(T) - 1));
	layer       = 1;
	while (r_leaves > 0) {
		depth = 0;
		subtree_found = 0;
		while ( !subtree_found ) {
			if (r_leaves <= (1UL << depth)) {
				for (int i = depth; i > 0; i--) {
					layer_offsets[layer + i] -= (1UL << (i - 1));
				}
				r_leaves -= LEAVES_HALF_TREE(r_leaves);
				layer_offsets[layer] -= 1;
				layer++;
				subtree_found = 1;
			} else {
				depth++;
			}
		}
	}

	/* For the offset, subtract all missing nodes from previous layers from current layer */
	for (int i = LOG2(T); i >= 0; i--) {
		nodes_per_layer[i] = (1UL << i) - layer_offsets[i];
		for (int j = i - 1; j >= 0; j--) {
			layer_offsets[i] -= layer_offsets[j];
		}
		layer_offsets[i] >>= 1;
	}
}

/*
 * get_leaf_indices() is quite similar to setup_tree(), however requires the
 * offset values to compute the correct indices.
 *
 * uint16_t merkle_leaf_indices[T]  : Stores the indices in the truncated tree
 * where the leaves are placed.
 * uint16_t layer_offsets[LOG2(T)+1]   : Same as above.
 */
static
void get_leaf_indices(uint16_t merkle_leaf_indices[T],
                      const uint16_t layer_offsets[LOG2(T) + 1]) {
	uint32_t r_leaves;
	uint32_t idx_ctr = 0;

	/* r_node: current root node of next subtree, will always be right-child of previous root */
	/* l_node: traverses from current root node to left-childs until depth of subtree is found */
	uint32_t r_node, l_node;
	uint32_t layer, depth, subtree_found;

	/* If tree is already balanced, simply copy leaves to corresponding position */
	if (T == (1UL << LOG2(T))) {
		for (size_t i = 0; i < T; i++) {
			merkle_leaf_indices[i] = T - 1 + i;
		}
		return;
	}

	/* Create (un-) balanced Merkle tree */
	r_leaves = T;
	depth = 0;
	layer = 0;
	r_node  = 0;
	l_node  = LEFT_CHILD(r_node) - 2 * layer_offsets[layer + depth];
	while (r_leaves > 0) {
		depth = 1;
		subtree_found = 0;
		/* Start from the current root node r_node until the size of a full left-subtree is found. */
		/* If only one leaf is remaining, put it to current root-node, macro RL() is used to decide that. */
		while ( !subtree_found ) {
			if (r_leaves <= (1UL << depth)) {
				for (size_t j = 0; j < LEAVES_HALF_TREE(r_leaves); j++) {
					merkle_leaf_indices[idx_ctr++] = RL(r_leaves) + j;
				}
				r_node = RIGHT_CHILD(r_node) - 2 * layer_offsets[layer];
				l_node = LEFT_CHILD(r_node) - 2 * layer_offsets[layer];
				layer++;
				r_leaves -= LEAVES_HALF_TREE(r_leaves);
				subtree_found = 1;
			} else {
				l_node = LEFT_CHILD(l_node) - 2 * layer_offsets[layer + depth];
				depth++;
			}
		}
	}
}

/* PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generate_merkle_tree()
 *
 * unsigned char merkle_tree[NUM_NODES_MERKLE_TREE * HASH_DIGEST_LENGTH] :
 * stores the hashes of the associated tree nodes.
 * const unsigned char commitments[T][HASH_DIGEST_LENGTH]    : Contains the
 * hashed commitments that build the tree.
 */
void PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generate_merkle_tree(unsigned char merkle_tree[NUM_NODES_MERKLE_TREE *
        HASH_DIGEST_LENGTH],
        unsigned char commitments[T][HASH_DIGEST_LENGTH]) {
	size_t i;
	uint32_t node_ctr, parent_layer;

	uint16_t merkle_leaf_indices[T];
	uint16_t layer_offsets[LOG2(T) + 1];
	uint16_t nodes_per_layer[LOG2(T) + 1];

	/* Setup the tree to get offsets for the computation of PARENT/CHILD nodes, as well as the number of nodes per layer */
	/* Move leafs in correct positions of the unbalanced Merkle tree */
	setup_tree(layer_offsets, nodes_per_layer);
	get_leaf_indices(merkle_leaf_indices, layer_offsets);

	/* Place commitments on the leaves indicated by merkle_leaf_indices */
	for (i = 0; i < T; i++) {
		memcpy(merkle_tree + merkle_leaf_indices[i]*HASH_DIGEST_LENGTH,
		       commitments + i,
		       HASH_DIGEST_LENGTH);
	}

	/* Hash the child nodes */
	node_ctr = 0;
	parent_layer = LOG2(T) - 1;
	for (i = NUM_NODES_MERKLE_TREE - 1; i > 0; i -= 2) {
		hash(merkle_tree + OFFSET(PARENT(i) + layer_offsets[parent_layer]), merkle_tree + OFFSET(SIBLING(i)), 2 * HASH_DIGEST_LENGTH);
		if (node_ctr >= (uint32_t) nodes_per_layer[parent_layer + 1] - 2) {
			parent_layer--;
			node_ctr = 0;
		} else {
			node_ctr += 2;
		}
	}
}

/* PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generate_merkle_proof()
 *
 * uint16_t merkle_proof_indices[TREE_NODES_TO_STORE]   : stores the sorted indices required for the proof.
 * uint16_t merkle_proof_len                            : Actual length of the proof. Can vary depending on the challenge.
 * const unsigned char challenge                        : Challenge that indicated which nodes will be recomputed by the verifier.
 */
void PQCLEAN_CROSSRSDPG256SMALL_CLEAN_generate_merkle_proof(uint16_t merkle_proof_indices[TREE_NODES_TO_STORE],
        uint16_t *merkle_proof_len,
        const unsigned char challenge[T]) {
	unsigned char flag_tree[NUM_NODES_MERKLE_TREE] = {NOT_COMPUTED};
	uint32_t node_ctr, parent_layer;
	size_t i;

	uint16_t layer_offsets[LOG2(T) + 1];
	uint16_t nodes_per_layer[LOG2(T) + 1];
	uint16_t merkle_leaf_indices[T];

	/* Setup the tree to get offsets for the computation of PARENT/CHILD nodes, as well as the number of nodes per layer */
	setup_tree(layer_offsets, nodes_per_layer);
	get_leaf_indices(merkle_leaf_indices, layer_offsets);

	/* Use challenges to mark nodes of path tree */
	for (size_t j = 0; j < T; j++) {
		if (challenge[j] == CHALLENGE_PROOF_VALUE) {
			flag_tree[merkle_leaf_indices[j]] = COMPUTED;
		}
	}

	/* Loop over all nodes, starting at the leaves */
	/* If at least one sibling is marked as COMPUTED, also mark the PARENT as such */
	/* Only add sibling of COMPUTED sibling as proof node if not both of them are marked as COMPUTED. */
	node_ctr = 0;
	parent_layer = LOG2(T) - 1;
	*merkle_proof_len = 0;
	for (i = NUM_NODES_MERKLE_TREE - 1; i > 0; i -= 2) {

		flag_tree[PARENT(i) + layer_offsets[parent_layer]] = (flag_tree[i] == COMPUTED) || (flag_tree[SIBLING(i)] == COMPUTED);

		/* Add left sibling only if left one was computed */
		if ( (flag_tree[i] == COMPUTED) && (flag_tree[SIBLING(i)] == NOT_COMPUTED) ) {
			merkle_proof_indices[(*merkle_proof_len)++] = SIBLING(i);
		}

		/* Add right sibling only right was computed */
		if ( (flag_tree[i] == NOT_COMPUTED) && (flag_tree[SIBLING(i)] == COMPUTED) ) {
			merkle_proof_indices[(*merkle_proof_len)++] = i;
		}

		/* Due to the unbalenced structure we got to keep track of the nodes per layer processed */
		if (node_ctr >= (uint32_t) nodes_per_layer[parent_layer + 1] - 2) {
			parent_layer--;
			node_ctr = 0;
		} else {
			node_ctr += 2;
		}
	}
}

/*
 * PQCLEAN_CROSSRSDPG256SMALL_CLEAN_rebuild_merkle_tree()
 *
 * unsigned char merkle_tree[NUM_NODES_MERKLE_TREE*HASH_DIGEST_LENGTH] : Stores the Hashes of the recomputed Merkle tree.
 * const unsigned char merkle_proof[TREE_NODES_TO_STORE]               : Merkle proof containing the nodes required for recomputation.
 * const unsigned char commitments[T][HASH_DIGEST_LENGTH]              : Stores the commitments.
 * const unsigned char challenge[T]                                    : Challenge vector to indicate the computed commitments.
 */
void PQCLEAN_CROSSRSDPG256SMALL_CLEAN_rebuild_merkle_tree(unsigned char merkle_tree[NUM_NODES_MERKLE_TREE * HASH_DIGEST_LENGTH],
        const unsigned char merkle_proof[TREE_NODES_TO_STORE * HASH_DIGEST_LENGTH],
        unsigned char commitments[T][HASH_DIGEST_LENGTH],
        const unsigned char challenge[T]) {
	uint16_t flag_tree_valid[NUM_NODES_MERKLE_TREE] = {INVALID_MERKLE_NODE};

	uint16_t merkle_leaf_indices[T];
	uint16_t layer_offsets[LOG2(T) + 1];
	uint16_t nodes_per_layer[LOG2(T) + 1];

	uint16_t ctr;
	uint32_t node_ctr, parent_layer;
	size_t i;

	/* Input consists of hash digests stored at child nodes and the index of the parent node for domain separation */
	unsigned char hash_input[2 * HASH_DIGEST_LENGTH];

	/* Move leafs in correct positions of binary merkle tree */
	/* Setup the tree again, computing the offsets and from that, the leaf indices */
	setup_tree(layer_offsets, nodes_per_layer);
	get_leaf_indices(merkle_leaf_indices, layer_offsets);

	/* Copy the commitments to the positions indicated by the challenge */
	for (i = 0; i < T; i++) {
		if (challenge[i] == CHALLENGE_PROOF_VALUE) {
			flag_tree_valid[merkle_leaf_indices[i]] = VALID_MERKLE_NODE;
			memcpy(merkle_tree + merkle_leaf_indices[i]*HASH_DIGEST_LENGTH, commitments + i, HASH_DIGEST_LENGTH);
		}
	}
	/* Create hash tree by hashing valid leaf nodes */
	ctr = 0;
	node_ctr = 0;
	parent_layer = LOG2(T) - 1;
	for (i = NUM_NODES_MERKLE_TREE - 1; i > 0; i -= 2) {

		/* Both siblings are unused, but it must be kept track of the node and layer counter to chose the right offsets */
		if (flag_tree_valid[i] == INVALID_MERKLE_NODE && flag_tree_valid[SIBLING(i)] == INVALID_MERKLE_NODE) {
			if (node_ctr >= (uint32_t) nodes_per_layer[parent_layer + 1] - 2) {
				parent_layer--;
				node_ctr = 0;
			} else {
				node_ctr += 2;
			}
			continue;
		}

		/* Prepare input to hash */
		/* Process right sibling from the tree if valid, otherwise take it from the merkle proof */
		if (flag_tree_valid[i] == VALID_MERKLE_NODE) {
			memcpy(hash_input + HASH_DIGEST_LENGTH, merkle_tree + OFFSET(i), HASH_DIGEST_LENGTH);
		} else {
			memcpy(hash_input + HASH_DIGEST_LENGTH, merkle_proof + OFFSET(ctr), HASH_DIGEST_LENGTH);
			ctr++;
		}

		/* Process left sibling from the tree if valid, otherwise take it from the merkle proof */
		if (flag_tree_valid[SIBLING(i)] == VALID_MERKLE_NODE) {
			memcpy(hash_input, merkle_tree + OFFSET(SIBLING(i)), HASH_DIGEST_LENGTH);
		} else {
			memcpy(hash_input, merkle_proof + OFFSET(ctr), HASH_DIGEST_LENGTH);
			ctr++;
		}

		/* Hash it and store the digest at the parent node */
		hash(merkle_tree + OFFSET(PARENT(i) + layer_offsets[parent_layer]), hash_input, 2 * HASH_DIGEST_LENGTH);
		flag_tree_valid[PARENT(i) + layer_offsets[parent_layer]] = VALID_MERKLE_NODE;

		if (node_ctr >= (uint32_t) nodes_per_layer[parent_layer + 1] - 2) {
			parent_layer--;
			node_ctr = 0;
		} else {
			node_ctr += 2;
		}
	}
}
