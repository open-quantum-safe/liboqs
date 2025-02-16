/**
 *
 * Reference ISO-C11 Implementation of CROSS.
 *
 * @version 2.0 (February 2025)
 *
 * Authors listed in alphabetical order:
 *
 * @author: Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author: Marco Gianvecchio <marco.gianvecchio@mail.polimi.it>
 * @author: Patrick Karl <patrick.karl@tum.de>
 * @author: Gerardo Pelosi <gerardo.pelosi@polimi.it>
 * @author: Jonas Schupp <jonas.schupp@tum.de>
 *
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
 *
 **/

#include <stdint.h>
#include <string.h>

#include "csprng_hash.h"
#include "merkle_tree.h"
#include "parameters.h"

#define PARENT(i) ( ((i)%2) ? (((i)-1)/2) : (((i)-2)/2) )
#define SIBLING(i) ( ((i)%2) ? (i)+1 : (i)-1 )

#define CHALLENGE_PROOF_VALUE 0
#define NOT_COMPUTED 0
#define COMPUTED 1

/*****************************************************************************/
static
void place_cmt_on_leaves(unsigned char merkle_tree[NUM_NODES_MERKLE_TREE *
                         HASH_DIGEST_LENGTH],
                         unsigned char commitments[T][HASH_DIGEST_LENGTH]) {
	const uint16_t cons_leaves[TREE_SUBROOTS] = TREE_CONSECUTIVE_LEAVES;
	const uint16_t leaves_start_indices[TREE_SUBROOTS] = TREE_LEAVES_START_INDICES;

	unsigned int cnt = 0;
	for (size_t i = 0; i < TREE_SUBROOTS; i++) {
		for (size_t j = 0; j < cons_leaves[i]; j++) {
			memcpy(merkle_tree + (leaves_start_indices[i] + j)*HASH_DIGEST_LENGTH,
			       commitments + cnt,
			       HASH_DIGEST_LENGTH);
			cnt++;
		}
	}
}

/*****************************************************************************/
static
void label_leaves(unsigned char flag_tree[NUM_NODES_MERKLE_TREE],
                  const unsigned char challenge[T]) {
	const uint16_t cons_leaves[TREE_SUBROOTS] = TREE_CONSECUTIVE_LEAVES;
	const uint16_t leaves_start_indices[TREE_SUBROOTS] = TREE_LEAVES_START_INDICES;

	unsigned int cnt = 0;
	for (size_t i = 0; i < TREE_SUBROOTS; i++) {
		for (size_t j = 0; j < cons_leaves[i]; j++) {
			if (challenge[cnt] == CHALLENGE_PROOF_VALUE) {
				flag_tree[leaves_start_indices[i] + j] = COMPUTED;
			}
			cnt++;
		}
	}
}

/*****************************************************************************/
void tree_root(uint8_t root[HASH_DIGEST_LENGTH],
               unsigned char tree[NUM_NODES_MERKLE_TREE * HASH_DIGEST_LENGTH],
               unsigned char leaves[T][HASH_DIGEST_LENGTH]) {
	/* off contains the offsets required to move between two layers in order
	 * to compensate for the truncation.
	 * npl contains the number of nodes per level.
	 * leaves_start_indices contains the index of the leftmost leaf of each full binary subtree
	 */
	const uint16_t off[LOG2(T) + 1] = TREE_OFFSETS;
	const uint16_t npl[LOG2(T) + 1] = TREE_NODES_PER_LEVEL;
	const uint16_t leaves_start_indices[TREE_SUBROOTS] = TREE_LEAVES_START_INDICES;

	/* Place the commitments on the (unbalanced-) Merkle tree using helper arrays for indexing */
	place_cmt_on_leaves(tree, leaves);

	/* Enqueue the calls to hash */
	int to_hash = 0;
	unsigned char *in_pos_queue[4] = {0};
	int out_pos_queue[4] = {0};

	/* Start hashing the nodes from right to left, starting always with
	 * the left-child node */
	unsigned int start_node = leaves_start_indices[0];
	for (int level = LOG2(T); level > 0; level--) {
		for (int i = npl[level] - 2; i >= 0; i -= 2) {
			to_hash++;
			uint16_t current_node = start_node + i;
			uint16_t parent_node = PARENT(current_node) + (off[level - 1] >> 1);
			/* Save the position of the hash outputs and copy input nodes into array */
			in_pos_queue[to_hash - 1] = tree + current_node * HASH_DIGEST_LENGTH;
			out_pos_queue[to_hash - 1] = parent_node * HASH_DIGEST_LENGTH;
			/* Hash in batches of 4 (or less when changing tree level) */
			if (to_hash == 4 || i == 0) {
				hash_par(
				    to_hash,
				    tree + out_pos_queue[0],
				    tree + out_pos_queue[1],
				    tree + out_pos_queue[2],
				    tree + out_pos_queue[3],
				    in_pos_queue[0],
				    in_pos_queue[1],
				    in_pos_queue[2],
				    in_pos_queue[3],
				    2 * HASH_DIGEST_LENGTH,
				    HASH_DOMAIN_SEP_CONST,
				    HASH_DOMAIN_SEP_CONST,
				    HASH_DOMAIN_SEP_CONST,
				    HASH_DOMAIN_SEP_CONST);
				to_hash = 0;
			}
		}
		start_node -= npl[level - 1];
	}

	/* Root is at first position of the tree */
	memcpy(root, tree, HASH_DIGEST_LENGTH);
}

/*****************************************************************************/
uint16_t tree_proof(uint8_t mtp[HASH_DIGEST_LENGTH * TREE_NODES_TO_STORE],
                    const uint8_t tree[NUM_NODES_MERKLE_TREE * HASH_DIGEST_LENGTH],
                    const uint8_t leaves_to_reveal[T]) {
	/* Label the flag tree to identify computed/valid nodes */
	unsigned char flag_tree[NUM_NODES_MERKLE_TREE] = {NOT_COMPUTED};
	label_leaves(flag_tree, leaves_to_reveal);

	const uint16_t off[LOG2(T) + 1] = TREE_OFFSETS;
	const uint16_t npl[LOG2(T) + 1] = TREE_NODES_PER_LEVEL;
	const uint16_t leaves_start_indices[TREE_SUBROOTS] = TREE_LEAVES_START_INDICES;

	int published = 0;
	unsigned int start_node = leaves_start_indices[0];
	for (int level = LOG2(T); level > 0; level--) {
		for (int i = npl[level] - 2; i >= 0; i -= 2) {
			uint16_t current_node = start_node + i;
			uint16_t parent_node = PARENT(current_node) + (off[level - 1] >> 1);

			flag_tree[parent_node] = (flag_tree[current_node] == COMPUTED) || (flag_tree[SIBLING(current_node)] == COMPUTED);

			/* Add left sibling only if right one was computed but left wasn't */
			if ( (flag_tree[current_node] == NOT_COMPUTED) && (flag_tree[SIBLING(current_node)] == COMPUTED) ) {
				memcpy(mtp + published * HASH_DIGEST_LENGTH, tree + current_node * HASH_DIGEST_LENGTH, HASH_DIGEST_LENGTH);
				published++;
			}

			/* Add right sibling only if left was computed but right wasn't */
			if ( (flag_tree[current_node] == COMPUTED) && (flag_tree[SIBLING(current_node)] == NOT_COMPUTED) ) {
				memcpy(mtp + published * HASH_DIGEST_LENGTH, tree + SIBLING(current_node)*HASH_DIGEST_LENGTH, HASH_DIGEST_LENGTH);
				published++;
			}
		}
		start_node -= npl[level - 1];
	}
	return published;
}

/*****************************************************************************/
uint8_t recompute_root(uint8_t root[HASH_DIGEST_LENGTH],
                       uint8_t recomputed_leaves[T][HASH_DIGEST_LENGTH],
                       const uint8_t mtp[HASH_DIGEST_LENGTH * TREE_NODES_TO_STORE],
                       const uint8_t leaves_to_reveal[T]) {
	unsigned char tree[NUM_NODES_MERKLE_TREE * HASH_DIGEST_LENGTH];
	unsigned char flag_tree[NUM_NODES_MERKLE_TREE] = {NOT_COMPUTED};

	place_cmt_on_leaves(tree, recomputed_leaves);
	label_leaves(flag_tree, leaves_to_reveal);

	const uint16_t off[LOG2(T) + 1] = TREE_OFFSETS;
	const uint16_t npl[LOG2(T) + 1] = TREE_NODES_PER_LEVEL;
	const uint16_t leaves_start_indices[TREE_SUBROOTS] = TREE_LEAVES_START_INDICES;

	/* Enqueue the calls to hash */
	int to_hash = 0;
	int in_pos_queue[4] = {0};
	int out_pos_queue[4] = {0};

	unsigned int published = 0;
	unsigned int start_node = leaves_start_indices[0];
	for (int level = LOG2(T); level > 0; level--) {
		for (int i = npl[level] - 2; i >= 0; i -= 2) {
			uint16_t current_node = start_node + i;
			uint16_t parent_node = PARENT(current_node) + (off[level - 1] >> 1);

			uint8_t are_both_siblings_unused = flag_tree[current_node] == NOT_COMPUTED && flag_tree[SIBLING(current_node)] == NOT_COMPUTED;
			if (!are_both_siblings_unused) {

				/* At least one of the siblings is valid: there is a hash to compute */
				to_hash++;
				in_pos_queue[to_hash - 1] = current_node * HASH_DIGEST_LENGTH;
				out_pos_queue[to_hash - 1] = parent_node * HASH_DIGEST_LENGTH;

				/* If the left sibling was not computed take it from the merkle proof */
				if (!(flag_tree[current_node] == COMPUTED)) {
					memcpy(tree + current_node * HASH_DIGEST_LENGTH, mtp + published * HASH_DIGEST_LENGTH, HASH_DIGEST_LENGTH);
					published++;
				}

				/* If the right sibling was not computed take it from the merkle proof */
				if (!(flag_tree[SIBLING(current_node)] == COMPUTED)) {
					memcpy(tree + current_node * HASH_DIGEST_LENGTH + HASH_DIGEST_LENGTH, mtp + published * HASH_DIGEST_LENGTH, HASH_DIGEST_LENGTH);
					published++;
				}

				flag_tree[parent_node] = COMPUTED;
			}

			/* Hash in batches of 4 (or less when changing tree level) */
			if (to_hash == 4 || i == 0) {
				hash_par(
				    to_hash,
				    tree + out_pos_queue[0],
				    tree + out_pos_queue[1],
				    tree + out_pos_queue[2],
				    tree + out_pos_queue[3],
				    tree + in_pos_queue[0],
				    tree + in_pos_queue[1],
				    tree + in_pos_queue[2],
				    tree + in_pos_queue[3],
				    2 * HASH_DIGEST_LENGTH,
				    HASH_DOMAIN_SEP_CONST,
				    HASH_DOMAIN_SEP_CONST,
				    HASH_DOMAIN_SEP_CONST,
				    HASH_DOMAIN_SEP_CONST);
				to_hash = 0;
			}
		}
		start_node -= npl[level - 1];
	}

	/* Root is at first position of the tree */
	memcpy(root, tree, HASH_DIGEST_LENGTH);

	// Check for correct zero padding in the remaining parth of the Merkle proof to
	// prevent trivial forgery
	uint8_t error = 0;
	for (int i = published * HASH_DIGEST_LENGTH; i < TREE_NODES_TO_STORE * HASH_DIGEST_LENGTH; i++) {
		error |= mtp[i];
	}
	return (error == 0);
}
