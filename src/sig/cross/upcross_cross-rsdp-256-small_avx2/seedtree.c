/**
 *
 * Reference ISO-C11 Implementation of CROSS.
 *
 * @version 2.2 (July 2025)
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
#include <string.h> // memcpy(...), memset(...)

#include "csprng_hash.h"
#include "seedtree.h"

#define LEFT_CHILD(i) (2*(i)+1)
#define RIGHT_CHILD(i) (2*(i)+2)
#define PARENT(i) ( ((i)%2) ? (((i)-1)/2) : (((i)-2)/2) )
#define SIBLING(i) ( ((i)%2) ? (i)+1 : (i)-1 )

/* Seed tree implementation. The binary seed tree is linearized into an array
 * from root to leaves, and from left to right.
 */

#define TO_PUBLISH 1
#define NOT_TO_PUBLISH 0

/*****************************************************************************/
/**
 * const unsigned char *indices: input parameter denoting an array
 * with a number of binary cells equal to "leaves" representing
 * the labels of the nodes identified as leaves of the tree[...]
 * passed as second parameter.
 * A label = 1 means that the byteseed of the node having the same index
 * has to be released; = 0, otherwise.
 *
 * unsigned char *tree: input/output parameter denoting an array
 * with a number of binary cells equal to "2*leaves-1";
 * the first "leaves" cells (i.e., the ones with positions from 0 to leaves-1)
 * are the ones that will be modified by the current subroutine,
 * the last "leaves" cells will be a copy of the input array passed as first
 * parameter.
 *
 * uint64_t leaves: input parameter;
 *
 */

static
void label_leaves(unsigned char flag_tree[NUM_NODES_SEED_TREE],
                  const unsigned char indices_to_publish[T]) {
	const uint16_t cons_leaves[TREE_SUBROOTS] = TREE_CONSECUTIVE_LEAVES;
	const uint16_t leaves_start_indices[TREE_SUBROOTS] = TREE_LEAVES_START_INDICES;

	unsigned int cnt = 0;
	for (size_t i = 0; i < TREE_SUBROOTS; i++) {
		for (size_t j = 0; j < cons_leaves[i]; j++) {
			flag_tree[leaves_start_indices[i] + j] = indices_to_publish[cnt];
			cnt++;
		}
	}
}

static void compute_seeds_to_publish(
    /* linearized binary tree of boolean nodes containing
     * flags for each node 1-filled nodes are to be released */
    unsigned char flags_tree_to_publish[NUM_NODES_SEED_TREE],
    /* Boolean Array indicating which of the T seeds must be
     * released convention as per the above defines */
    const unsigned char indices_to_publish[T]) {
	/* the indices to publish may be less than the full leaves, copy them
	 * into the linearized tree leaves */
	label_leaves(flags_tree_to_publish, indices_to_publish);

	const uint16_t off[LOG2(T) + 1] = TREE_OFFSETS;
	const uint16_t npl[LOG2(T) + 1] = TREE_NODES_PER_LEVEL;
	const uint16_t leaves_start_indices[TREE_SUBROOTS] = TREE_LEAVES_START_INDICES;

	/* compute the value for the internal nodes of the tree starting from the
	 * the leaves, right to left */
	unsigned int start_node = leaves_start_indices[0];
	for (int level = LOG2(T); level > 0; level--) {
		for (int i = npl[level] - 2; i >= 0; i -= 2) {
			uint16_t current_node = start_node + i;
			uint16_t parent_node = PARENT(current_node) + (off[level - 1] >> 1);

			flags_tree_to_publish[parent_node] = (flags_tree_to_publish[current_node] == TO_PUBLISH)
			                                     && (flags_tree_to_publish[SIBLING(current_node)] == TO_PUBLISH);
		}
		start_node -= npl[level - 1];
	}
} /* end compute_seeds_to_publish */

/**
 * unsigned char *seed_tree:
 * it is intended as an output parameter;
 * storing the linearized seed tree
 *
 * The root seed is taken as a parameter.
 * The seed of its TWO children are computed expanding (i.e., shake128...) the
 * entropy in "salt" + "seedBytes of the parent" +
 *            "int, encoded over 16 bits - uint16_t,  associated to each node
 *             from roots to leaves layer-by-layer from left to right,
 *             counting from 0 (the integer bound with the root node)"
 */
void gen_seed_tree(unsigned char seed_tree[NUM_NODES_SEED_TREE * SEED_LENGTH_BYTES],
                   const unsigned char root_seed[SEED_LENGTH_BYTES],
                   const unsigned char salt[SALT_LENGTH_BYTES]) {
	PAR_CSPRNG_STATE_T tree_csprng_state;

	/* CSPRNG input: father seed | salt | father node index
	 * CSPRNG output: left child seed | right child seed */

	/* enqueue the calls to the CSPRNG */
	int to_expand = 0;
	unsigned char in_queue[4][CSPRNG_INPUT_LENGTH];
	uint16_t in_queue_dsc[4];
	int out_pos_queue[4] = {0};

	/* copy the salt into all 4 CSPRNG inputs */
	for (int i = 0; i < 4; i++) {
		memcpy(in_queue[i] + SEED_LENGTH_BYTES, salt, SALT_LENGTH_BYTES);
	}

	/* Set the root seed in the tree from the received parameter */
	memcpy(seed_tree, root_seed, SEED_LENGTH_BYTES);

	/* off contains the offsets required to move between two layers in order
	 * to compensate for the truncation.
	 * npl contains the number of nodes per level.
	 * lpl contains the number of leaves per level
	 * */
	const uint16_t off[LOG2(T) + 1] = TREE_OFFSETS;
	const uint16_t npl[LOG2(T) + 1] = TREE_NODES_PER_LEVEL;
	const uint16_t lpl[LOG2(T) + 1] = TREE_LEAVES_PER_LEVEL;

	/* Generate the log_2(t) layers from the root, each iteration generates a tree
	 * level; iterate on nodes of the parent level; the leaf nodes on each level
	 * don't need to be expanded, thus only iterate to npl[level]-lpl[level] */
	int start_node = 0;
	for (int level = 0; level < LOG2(T); level++) {
		for (int node_in_level = 0; node_in_level < npl[level] - lpl[level]; node_in_level++ ) {

			to_expand++;
			uint16_t father_node = start_node + node_in_level;
			uint16_t left_child_node = LEFT_CHILD(father_node) - off[level];

			/* save the father seed in the CSPRNG input */
			memcpy(in_queue[to_expand - 1],
			       seed_tree + father_node * SEED_LENGTH_BYTES,
			       SEED_LENGTH_BYTES);

			/* save the position of the CSPRNG output (the left child) */
			out_pos_queue[to_expand - 1] = left_child_node * SEED_LENGTH_BYTES;

			/* add a domain separator to the CSPRNG input (the index of the father node) */
			in_queue_dsc[to_expand - 1] = CSPRNG_DOMAIN_SEP_CONST + father_node;

			/* call CSPRNG in batches of 4 (or less when changing tree level) */
			if (to_expand == 4 || node_in_level == (npl[level] - lpl[level] - 1)) {
				csprng_initialize_par(
				    to_expand,
				    &tree_csprng_state,
				    in_queue[0],
				    in_queue[1],
				    in_queue[2],
				    in_queue[3],
				    CSPRNG_INPUT_LENGTH,
				    in_queue_dsc[0],
				    in_queue_dsc[1],
				    in_queue_dsc[2],
				    in_queue_dsc[3]);
				csprng_randombytes_par(
				    to_expand,
				    &tree_csprng_state,
				    seed_tree + out_pos_queue[0],
				    seed_tree + out_pos_queue[1],
				    seed_tree + out_pos_queue[2],
				    seed_tree + out_pos_queue[3],
				    2 * SEED_LENGTH_BYTES);
				/* PQClean-edit: CSPRNG release context */
				csprng_release_par(to_expand, &tree_csprng_state);
				to_expand = 0;
			}
		}
		start_node += npl[level];
	}
} /* end generate_seed_tree */


/*****************************************************************************/
void seed_leaves(unsigned char rounds_seeds[T * SEED_LENGTH_BYTES],
                 unsigned char seed_tree[NUM_NODES_SEED_TREE * SEED_LENGTH_BYTES]) {
	const uint16_t cons_leaves[TREE_SUBROOTS] = TREE_CONSECUTIVE_LEAVES;
	const uint16_t leaves_start_indices[TREE_SUBROOTS] = TREE_LEAVES_START_INDICES;

	unsigned int cnt = 0;
	for (size_t i = 0; i < TREE_SUBROOTS; i++) {
		for (size_t j = 0; j < cons_leaves[i]; j++) {
			memcpy(rounds_seeds + cnt * SEED_LENGTH_BYTES,
			       seed_tree + (leaves_start_indices[i] + j)*SEED_LENGTH_BYTES,
			       SEED_LENGTH_BYTES);
			cnt++;
		}
	}
}


/*****************************************************************************/
int seed_path(unsigned char *seed_storage,
              // OUTPUT: sequence of seeds to be released
              const unsigned char
              seed_tree[NUM_NODES_SEED_TREE * SEED_LENGTH_BYTES],
              // INPUT: linearized seedtree array with 2T-1 nodes each of size SEED_LENGTH_BYTES
              const unsigned char indices_to_publish[T]
              // INPUT: binary array denoting which node has to be released (cell == TO_PUBLISH) or not
             ) {
	/* complete linearized binary tree containing boolean values determining
	 * if a node is to be released or not according to convention above.
	 * */
	unsigned char flags_tree_to_publish[NUM_NODES_SEED_TREE] = {NOT_TO_PUBLISH};
	compute_seeds_to_publish(flags_tree_to_publish, indices_to_publish);

	const uint16_t off[LOG2(T) + 1] = TREE_OFFSETS;
	const uint16_t npl[LOG2(T) + 1] = TREE_NODES_PER_LEVEL;

	/* no sense in trying to publish the root node, start examining from level 1 */
	int start_node = 1;
	int num_seeds_published = 0;
	for (int level = 1; level <= LOG2(T); level++) {
		for (int node_in_level = 0; node_in_level < npl[level]; node_in_level++ ) {
			uint16_t current_node = start_node + node_in_level;
			uint16_t father_node = PARENT(current_node) + (off[level - 1] >> 1);

			/* if seed is to published and its ancestor/parent node is not,
			 * add it to the seed storage */
			if ( (flags_tree_to_publish[current_node] == TO_PUBLISH) &&
			        (flags_tree_to_publish[father_node] == NOT_TO_PUBLISH) ) {
				memcpy(seed_storage + num_seeds_published * SEED_LENGTH_BYTES,
				       seed_tree + current_node * SEED_LENGTH_BYTES,
				       SEED_LENGTH_BYTES);
				num_seeds_published++;
			}
		}
		start_node += npl[level];
	}
	return num_seeds_published;
} /* end publish_seeds */

/*****************************************************************************/

uint8_t rebuild_tree(unsigned char
                     seed_tree[NUM_NODES_SEED_TREE * SEED_LENGTH_BYTES],
                     const unsigned char indices_to_publish[T],
                     const unsigned char *stored_seeds,
                     const unsigned char salt[SALT_LENGTH_BYTES]) {
	/* complete linearized binary tree containing boolean values determining
	 * if a node is to be released or not according to aboves convention
	 */
	unsigned char flags_tree_to_publish[NUM_NODES_SEED_TREE] = {0};
	compute_seeds_to_publish(flags_tree_to_publish, indices_to_publish);

	PAR_CSPRNG_STATE_T tree_csprng_state;

	/* enqueue the calls to the CSPRNG */
	int to_expand = 0;
	unsigned char in_queue[4][CSPRNG_INPUT_LENGTH];
	uint16_t in_queue_dsc[4];
	int out_pos_queue[4] = {0};

	/* copy the salt into all 4 CSPRNG inputs */
	for (int i = 0; i < 4; i++) {
		memcpy(in_queue[i] + SEED_LENGTH_BYTES, salt, SALT_LENGTH_BYTES);
	}

	const uint16_t off[LOG2(T) + 1] = TREE_OFFSETS;
	const uint16_t npl[LOG2(T) + 1] = TREE_NODES_PER_LEVEL;
	const uint16_t lpl[LOG2(T) + 1] = TREE_LEAVES_PER_LEVEL;

	/* regenerating the seed tree never starts from the root, as it is never
	 * disclosed */
	int nodes_used = 0;
	int start_node = 1;
	for (int level = 1; level <= LOG2(T); level++) {
		for (int node_in_level = 0; node_in_level < npl[level]; node_in_level++ ) {

			uint16_t current_node = start_node + node_in_level;
			uint16_t father_node = PARENT(current_node) + (off[level - 1] >> 1);
			uint16_t left_child = LEFT_CHILD(current_node) - off[level];

			/* if the current node is a seed which was published (thus its father
			 * was not), memcpy it in place */
			if ( flags_tree_to_publish[current_node] == TO_PUBLISH ) {
				if ( flags_tree_to_publish[father_node] == NOT_TO_PUBLISH ) {
					memcpy(seed_tree + current_node * SEED_LENGTH_BYTES,
					       stored_seeds + nodes_used * SEED_LENGTH_BYTES,
					       SEED_LENGTH_BYTES );
					nodes_used++;
				}
			}
			/* If the current node is published and not a leaf, CSPRNG-expand its children.
			 * Since there is no reason of expanding leaves, only iterate to nodes per level (npl)
			 * minus leaves per level (lpl) in each level */
			if ( ( flags_tree_to_publish[current_node] == TO_PUBLISH ) && (node_in_level < npl[level] - lpl[level] ) ) {

				to_expand++;

				/* save the father seed in the CSPRNG input */
				memcpy(in_queue[to_expand - 1],
				       seed_tree + current_node * SEED_LENGTH_BYTES,
				       SEED_LENGTH_BYTES);

				/* save the position of the CSPRNG output (the left child) */
				out_pos_queue[to_expand - 1] = left_child * SEED_LENGTH_BYTES;

				/* add a domain separator to the CSPRNG input (the index of the father node) */
				in_queue_dsc[to_expand - 1] = CSPRNG_DOMAIN_SEP_CONST + current_node;
			}

			/* call CSPRNG in batches of 4 (or less when changing tree level) */
			if (to_expand == 4 || node_in_level == (npl[level] - lpl[level] - 1)) {
				csprng_initialize_par(
				    to_expand,
				    &tree_csprng_state,
				    in_queue[0],
				    in_queue[1],
				    in_queue[2],
				    in_queue[3],
				    CSPRNG_INPUT_LENGTH,
				    in_queue_dsc[0],
				    in_queue_dsc[1],
				    in_queue_dsc[2],
				    in_queue_dsc[3]);
				csprng_randombytes_par(
				    to_expand,
				    &tree_csprng_state,
				    seed_tree + out_pos_queue[0],
				    seed_tree + out_pos_queue[1],
				    seed_tree + out_pos_queue[2],
				    seed_tree + out_pos_queue[3],
				    2 * SEED_LENGTH_BYTES);
				/* PQClean-edit: CSPRNG release context */
				csprng_release_par(to_expand, &tree_csprng_state);
				to_expand = 0;
			}
		}
		start_node += npl[level];
	}

	// Check for correct zero padding in the remaining parth of the seed path to
	// prevent trivial forgery
	uint8_t error = 0;
	for (int i = nodes_used * SEED_LENGTH_BYTES; i < TREE_NODES_TO_STORE * SEED_LENGTH_BYTES; i++) {
		error |= stored_seeds[i];
	}
	return (error == 0);
} /* end regenerate_leaves */
