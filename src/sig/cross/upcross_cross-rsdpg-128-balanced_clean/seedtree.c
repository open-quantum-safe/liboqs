/**
 *
 * Reference ISO-C11 Implementation of CROSS.
 *
 * @version 1.1 (March 2023)
 *
 * @author Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author Gerardo Pelosi <gerardo.pelosi@polimi.it>
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

#include "seedtree.h"
#include <stdint.h>
#include <string.h> // memcpy(...), memset(...)

#define LEFT_CHILD(i) (2*(i)+1)
#define RIGHT_CHILD(i) (2*(i)+2)
#define PARENT(i) (((i)-1)/2)

/* Seed tree implementation. The binary seed tree is linearized into an array
 * from root to leaves, and from left to right. The nodes are numbered picking
 * the indexes from the corresponding full tree, having 2**LOG2(T) leaves */
#define DIV_BY_TWO_CEIL(i)  ((i)/2 + (i) % 2)

#define TO_PUBLISH 1
#define NOT_TO_PUBLISH 0

/* PQClean-edit: avoid VLA */
#define CSPRNG_INPUT_LEN (SALT_LENGTH_BYTES + SEED_LENGTH_BYTES + SIZEOF_UINT16)
//const uint32_t csprng_input_len = SALT_LENGTH_BYTES + SEED_LENGTH_BYTES + sizeof(uint16_t);

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

#define NUM_LEAVES_STENCIL_SEED_TREE ( 1UL << LOG2(T) )
#define NUM_INNER_NODES_STENCIL_SEED_TREE ( NUM_LEAVES_STENCIL_SEED_TREE-1 )
#define NUM_NODES_STENCIL_SEED_TREE ( 2*NUM_LEAVES_STENCIL_SEED_TREE-1 )

static void compute_seeds_to_publish(
    /* linearized binary tree of boolean nodes containing
     * flags for each node 1-filled nodes are not to be
     * released */
    unsigned char flags_tree_to_publish[NUM_NODES_STENCIL_SEED_TREE],
    /* Boolean Array indicating which of the T seeds must be
     * released convention as per the above defines */
    const unsigned char indices_to_publish[T]) {
	/* the indices to publish may be less than the full leaves, copy them
	 * into the linearized tree leaves */
	memcpy(flags_tree_to_publish + NUM_INNER_NODES_STENCIL_SEED_TREE,
	       indices_to_publish,
	       T);
	memset(flags_tree_to_publish,
	       NOT_TO_PUBLISH,
	       NUM_INNER_NODES_STENCIL_SEED_TREE * sizeof(unsigned char));
	/* compute the value for the internal nodes of the tree starting from the
	 * fathers of the leaves, right to left */
	for (int i = NUM_LEAVES_STENCIL_SEED_TREE - 2; i >= 0; i--) {
		if ( ( flags_tree_to_publish[LEFT_CHILD(i)]  == TO_PUBLISH) &&
		        ( flags_tree_to_publish[RIGHT_CHILD(i)] == TO_PUBLISH) ) {
			flags_tree_to_publish[i] = TO_PUBLISH;
		}
	}
} /* end compute_seeds_to_publish */

/**
 * unsigned char *seed_tree:
 * it is intended as an output parameter;
 * storing the linearized binary seed tree
 *
 * The root seed is taken as a parameter.
 * The seed of its TWO children are computed expanding (i.e., shake128...) the
 * entropy in "salt" + "seedBytes of the parent" +
 *            "int, encoded over 16 bits - uint16_t,  associated to each node
 *             from roots to leaves layer-by-layer from left to right,
 *             counting from 0 (the integer bound with the root node)"
 */
void PQCLEAN_CROSSRSDPG128BALANCED_CLEAN_generate_seed_tree_from_root(unsigned char
        seed_tree[NUM_NODES_SEED_TREE * SEED_LENGTH_BYTES],
        const unsigned char root_seed[SEED_LENGTH_BYTES],
        const unsigned char salt[SALT_LENGTH_BYTES]) {
	/* input buffer to the CSPRNG, contains a salt, the seed to be expanded
	 * and the integer index of the node being expanded for domain separation */
	const uint32_t csprng_input_len = SALT_LENGTH_BYTES +
	                                  SEED_LENGTH_BYTES +
	                                  sizeof(uint16_t);
	unsigned char csprng_input[CSPRNG_INPUT_LEN];
	CSPRNG_STATE_T tree_csprng_state;
	memcpy(csprng_input + SEED_LENGTH_BYTES, salt, SALT_LENGTH_BYTES);

	/* Set the root seed in the tree from the received parameter */
	memcpy(seed_tree, root_seed, SEED_LENGTH_BYTES);

	/* missing_nodes_before[i] contains the total number of missing nodes before
	 * level i (the root is level 0). This constant vector is precomputed */
	const int missing_nodes_before[LOG2(T) + 1] = MISSING_NODES_BEFORE_LEVEL_ARRAY;
	/* Generate the log_2(t) layers from the root, each iteration generates a tree
	 * level; iterate on nodes of the parent level */
	const int nodes_in_level[LOG2(T) + 1] = NODES_PER_LEVEL_ARRAY;
	int ancestors = 0;
	for (int level = 0; level < LOG2(T); level++) {
		for (int node_in_level = 0; node_in_level < nodes_in_level[level]; node_in_level++ ) {
			uint16_t father_node_idx = ancestors + node_in_level;
			uint16_t father_node_storage_idx = father_node_idx - missing_nodes_before[level];
			/* prepare the CSPRNG input to expand the children of node i */
			memcpy(csprng_input,
			       seed_tree + father_node_storage_idx * SEED_LENGTH_BYTES,
			       SEED_LENGTH_BYTES);
			*((uint16_t *)(csprng_input + SALT_LENGTH_BYTES + SEED_LENGTH_BYTES)) = to_little_endian16(father_node_idx);
			/* expand the children (stored contiguously) */
			initialize_csprng(&tree_csprng_state, csprng_input, csprng_input_len);
			csprng_randombytes(seed_tree + (LEFT_CHILD(father_node_idx) - missing_nodes_before[level + 1] ) *SEED_LENGTH_BYTES,
			                   SEED_LENGTH_BYTES,
			                   &tree_csprng_state);
			if ((RIGHT_CHILD(father_node_idx) - missing_nodes_before[level + 1]) < NUM_NODES_SEED_TREE ) {
				csprng_randombytes(seed_tree + (RIGHT_CHILD(father_node_idx) - missing_nodes_before[level + 1])*SEED_LENGTH_BYTES,
				                   SEED_LENGTH_BYTES,
				                   &tree_csprng_state);
			}

			/* PQClean-edit: CSPRNG release context */
			csprng_release(&tree_csprng_state);

		}
		ancestors += (1L << level);
	}
} /* end generate_seed_tree */

/*****************************************************************************/

/*****************************************************************************/
int PQCLEAN_CROSSRSDPG128BALANCED_CLEAN_publish_seeds(unsigned char *seed_storage,
        // OUTPUT: sequence of seeds to be released
        const unsigned char
        seed_tree[NUM_NODES_SEED_TREE * SEED_LENGTH_BYTES],
        // INPUT: binary array storing in each cell a binary value (i.e., 0 or 1),
        //        which in turn denotes if the seed of the node with the same index
        //        must be released (i.e., cell == 0) or not (i.e., cell == 1).
        //        Indeed the seed will be stored in the sequence computed as a result into the out[...] array.
        // INPUT: binary array denoting which node has to be released (cell == TO_PUBLISH) or not
        const unsigned char indices_to_publish[T]
                                                     ) {
	/* complete linearized binary tree containing boolean values determining
	 * if a node is to be released or not. Nodes set to 1 are not to be released
	 * oldest ancestor of sets of nodes equal to 0 are to be released */
	unsigned char flags_tree_to_publish[NUM_NODES_STENCIL_SEED_TREE] = {0};
	compute_seeds_to_publish(flags_tree_to_publish, indices_to_publish);
	const int missing_nodes_before[LOG2(T) + 1] = MISSING_NODES_BEFORE_LEVEL_ARRAY;
	const int nodes_in_level[LOG2(T) + 1] = NODES_PER_LEVEL_ARRAY;

	int num_seeds_published = 0;
	int node_idx = 1;
	/* no sense in trying to publish the root node, start examining from level 1
	 * */

	int ancestors = 1;
	for (int level = 1; level < LOG2(T) + 1; level++) {
		for (int node_in_level = 0; node_in_level < nodes_in_level[level]; node_in_level++ ) {
			node_idx = ancestors + node_in_level;
			int node_storage_idx = node_idx - missing_nodes_before[level];
			if ( (flags_tree_to_publish[node_idx] == TO_PUBLISH) &&
			        (flags_tree_to_publish[PARENT(node_idx)] == NOT_TO_PUBLISH) ) {
				memcpy(seed_storage + num_seeds_published * SEED_LENGTH_BYTES,
				       seed_tree + node_storage_idx * SEED_LENGTH_BYTES,
				       SEED_LENGTH_BYTES);
				num_seeds_published++;
			}
		}
		ancestors += (1L << level);
	}

	return num_seeds_published;
} /* end PQCLEAN_CROSSRSDPG128BALANCED_CLEAN_publish_seeds */

/*****************************************************************************/

int PQCLEAN_CROSSRSDPG128BALANCED_CLEAN_regenerate_round_seeds(unsigned char
        seed_tree[NUM_NODES_SEED_TREE * SEED_LENGTH_BYTES],
        const unsigned char indices_to_publish[T],
        const unsigned char *stored_seeds,
        const unsigned char salt[SALT_LENGTH_BYTES]) {
	/* complete linearized binary tree containing boolean values determining
	 * if a node is to be released or not. Nodes set to 1 are not to be released
	 * oldest ancestor of sets of nodes equal to 0 are to be released */
	unsigned char flags_tree_to_publish[NUM_NODES_STENCIL_SEED_TREE] = {0};
	compute_seeds_to_publish(flags_tree_to_publish, indices_to_publish);

	const uint32_t csprng_input_len = SALT_LENGTH_BYTES +
	                                  SEED_LENGTH_BYTES +
	                                  sizeof(uint16_t);
	unsigned char csprng_input[CSPRNG_INPUT_LEN];
	CSPRNG_STATE_T tree_csprng_state;

	memcpy(csprng_input + SEED_LENGTH_BYTES, salt, SALT_LENGTH_BYTES);

	int nodes_used = 0;
	/* missing_nodes_before[i] contains the total number of missing nodes before
	 * level i. Level 0 is taken to be the tree root This constant vector is precomputed */
	const int missing_nodes_before[LOG2(T) + 1] = MISSING_NODES_BEFORE_LEVEL_ARRAY;

	/* regenerating the seed tree never starts from the root, as it is never
	 * disclosed */
	int ancestors = 0;
	const int nodes_in_level[LOG2(T) + 1] = NODES_PER_LEVEL_ARRAY;
	for (int level = 0; level <= LOG2(T); level++) {
		for (int node_in_level = 0; node_in_level < nodes_in_level[level]; node_in_level++ ) {
			uint16_t father_node_idx = ancestors + node_in_level;
			uint16_t father_node_storage_idx = father_node_idx - missing_nodes_before[level];
			/* if the current node is a seed which was published, memcpy it in place */
			if ( flags_tree_to_publish[father_node_idx] == TO_PUBLISH ) {
				if ( flags_tree_to_publish[PARENT(father_node_idx)] == NOT_TO_PUBLISH ) {
					memcpy(seed_tree + SEED_LENGTH_BYTES * (father_node_storage_idx),
					       stored_seeds + SEED_LENGTH_BYTES * nodes_used,
					       SEED_LENGTH_BYTES );
					nodes_used++;
				}
			}
			/* if the current node is published and not a leaf,
			 * CSPRNG-expand its children */
			if ( ( flags_tree_to_publish[father_node_idx] == TO_PUBLISH ) &&
			        ( level < LOG2(T)) ) {
				/* prepare the CSPRNG input to expand the children of node father_node_idx */
				memcpy(csprng_input,
				       seed_tree + (father_node_storage_idx)*SEED_LENGTH_BYTES,
				       SEED_LENGTH_BYTES);
				*((uint16_t *)(csprng_input + SALT_LENGTH_BYTES + SEED_LENGTH_BYTES)) = to_little_endian16(father_node_idx);
				/* expand the children (stored contiguously) */
				initialize_csprng(&tree_csprng_state, csprng_input, csprng_input_len);
				csprng_randombytes(seed_tree + (LEFT_CHILD(father_node_idx) - missing_nodes_before[level + 1])*SEED_LENGTH_BYTES,
				                   SEED_LENGTH_BYTES,
				                   &tree_csprng_state);
				csprng_randombytes(seed_tree + (RIGHT_CHILD(father_node_idx) - missing_nodes_before[level + 1])*SEED_LENGTH_BYTES,
				                   SEED_LENGTH_BYTES,
				                   &tree_csprng_state);

				/* PQClean-edit: CSPRNG release context */
				csprng_release(&tree_csprng_state);
			}
		}
		ancestors += (1L << level);
	}
	return nodes_used;
} /* end regenerate_leaves */
