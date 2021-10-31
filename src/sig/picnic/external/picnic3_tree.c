/*! @file tree.c
 *  @brief This file has the tree implementation used to generate random seeds
 *  and commit to multiple values with a Merkle tree.
 *
 *  This file is part of the reference implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "endian_compat.h"
#include "kdf_shake.h"
#include "picnic.h"
#include "picnic3_tree.h"
#include "picnic3_types.h"

/* The smallest tree has numNodes = 31, so we need at least 64 bit to represent nodes and the flags.
 * On 32 bit platforms, it might be more efficient to work with 32-bit words, though. At least on
 * 64 bit Linux, uint_fast32_t is 64 bits wide. */
typedef uint_fast32_t bitset_word_t;
#define BITSET_WORD_C(v) ((bitset_word_t)(v))

static inline bitset_word_t get_bit(const bitset_word_t* array, size_t index) {
  return array[index / (sizeof(bitset_word_t) * 8)] >> (index % (sizeof(bitset_word_t) * 8)) & 0x1;
}

static inline void set_bit(bitset_word_t* array, size_t index) {
  array[index / (sizeof(bitset_word_t) * 8)] |= BITSET_WORD_C(1)
                                                << (index % (sizeof(bitset_word_t) * 8));
}

static int contains(size_t* list, size_t len, size_t value) {
  for (size_t i = 0; i < len; i++) {
    if (list[i] == value) {
      return 1;
    }
  }
  return 0;
}

static bitset_word_t exists(tree_t* tree, size_t i) {
  if (i >= tree->numNodes) {
    return 0;
  }
  return get_bit(tree->haveNodeExists, 2 * i);
}

static bitset_word_t haveNode(tree_t* tree, size_t i) {
  return get_bit(tree->haveNodeExists, 2 * i + 1);
}

static void markNode(tree_t* tree, size_t i) {
  set_bit(tree->haveNodeExists, 2 * i + 1);
}

static bool existsNotHaveNode(tree_t* tree, size_t i) {
  return (tree->haveNodeExists[2 * i / (sizeof(bitset_word_t) * 8)] >>
              (2 * i % (sizeof(bitset_word_t) * 8)) &
          0x3) == 0x01;
}

tree_t* createTree(size_t numLeaves, size_t dataSize) {
  tree_t* tree = malloc(sizeof(tree_t));

  tree->depth = ceil_log2(numLeaves) + 1;
  tree->numNodes =
      ((1 << (tree->depth)) - 1) -
      ((1 << (tree->depth - 1)) - numLeaves); /* Num nodes in complete - number of missing leaves */
  tree->numLeaves = numLeaves;
  tree->dataSize  = dataSize;
  tree->nodes     = calloc(tree->numNodes, dataSize);

  /* Depending on the number of leaves, the tree may not be complete */
  tree->haveNodeExists =
      calloc((2 * tree->numNodes + (sizeof(bitset_word_t) * 8) - 1) / (sizeof(bitset_word_t) * 8),
             sizeof(bitset_word_t));
  /* Set leaves */
  for (size_t i = 0; i < tree->numLeaves; ++i) {
    set_bit(tree->haveNodeExists, 2 * (tree->numNodes - tree->numLeaves + i));
  }
  for (int i = tree->numNodes - tree->numLeaves; i > 0; i--) {
    if (exists(tree, 2 * i + 1) || exists(tree, 2 * i + 2)) {
      set_bit(tree->haveNodeExists, 2 * i);
    }
  }
  set_bit(tree->haveNodeExists, 0);

  return tree;
}

void freeTree(tree_t* tree) {
  if (tree != NULL) {
    free(tree->nodes);
    free(tree->haveNodeExists);
    free(tree);
  }
}

static int isLeftChild(size_t node) {
  assert(node != 0);
  return (node % 2 == 1);
}

static int hasRightChild(tree_t* tree, size_t node) {
  return (2 * node + 2 < tree->numNodes && exists(tree, node));
}

static size_t getParent(size_t node) {
  assert(node != 0);
  return ((node + 1) >> 1) - 1;

  //if (isLeftChild(node)) {
  //  /* (node - 1) / 2, but since node % 2 == 1, that's the same as node / 2 */
  //  return node >> 1;
  //}
  //return (node - 2) / 2;
}

uint8_t* getLeaves(tree_t* tree) {
  return &tree->nodes[(tree->numNodes - tree->numLeaves) * tree->dataSize];
}

uint8_t* getLeaf(tree_t* tree, size_t leafIndex) {
  assert(leafIndex < tree->numLeaves);
  size_t firstLeaf = tree->numNodes - tree->numLeaves;
  return &tree->nodes[(firstLeaf + leafIndex) * tree->dataSize];
}

static void hashSeed(uint8_t* digest, const uint8_t* inputSeed, uint8_t* salt, uint8_t hashPrefix,
                     size_t repIndex, size_t nodeIndex, const picnic_instance_t* params) {
  hash_context ctx;

  hash_init_prefix(&ctx, params->digest_size, hashPrefix);
  hash_update(&ctx, inputSeed, params->seed_size);
  hash_update(&ctx, salt, SALT_SIZE);
  hash_update_uint16_le(&ctx, repIndex);
  hash_update_uint16_le(&ctx, nodeIndex);
  hash_final(&ctx);
  hash_squeeze(&ctx, digest, 2 * params->seed_size);
  hash_clear(&ctx);
}

static void hashSeed_x4(uint8_t** digest, const tree_t* tree, uint8_t* salt,
                        uint8_t hashPrefix, size_t repIndex, size_t nodeIndex,
                        const picnic_instance_t* params) {
  const size_t seed_size = params->seed_size;

  hash_context_x4 ctx;
  hash_init_prefix_x4(&ctx, params->digest_size, hashPrefix);
  hash_update_x4_4(&ctx, &tree->nodes[nodeIndex * seed_size],
                   &tree->nodes[(nodeIndex + 1) * seed_size],
                   &tree->nodes[(nodeIndex + 2) * seed_size],
                   &tree->nodes[(nodeIndex + 3) * seed_size], seed_size);

  hash_update_x4_1(&ctx, salt, SALT_SIZE);
  hash_update_x4_uint16_le(&ctx, repIndex);

  const uint16_t nodes[4] = {nodeIndex, nodeIndex + 1, nodeIndex + 2, nodeIndex + 3};
  hash_update_x4_uint16s_le(&ctx, nodes);

  hash_final_x4(&ctx);
  hash_squeeze_x4(&ctx, digest, 2 * seed_size);
  hash_clear_x4(&ctx);
}

static void expandSeeds(tree_t* tree, uint8_t* salt, size_t repIndex,
                        const picnic_instance_t* params) {
  const size_t seed_size = params->seed_size;

  uint8_t tmp[4 * 2 * MAX_SEED_SIZE_BYTES];
  uint8_t* tmp_ptr[4] = {&tmp[0], &tmp[2 * MAX_SEED_SIZE_BYTES], &tmp[2 * 2 * MAX_SEED_SIZE_BYTES],
                         &tmp[3 * 2 * MAX_SEED_SIZE_BYTES]};

  /* Walk the tree, expanding seeds where possible. Compute children of
   * non-leaf nodes. */
  size_t lastNonLeaf = getParent(tree->numNodes - 1);
  size_t i           = 0;
  /* expand the first 4 seeds*/
  for (; i <= MIN(2, lastNonLeaf); i++) {
    if (!haveNode(tree, i)) {
      continue;
    }

    hashSeed(tmp, &tree->nodes[i * seed_size], salt, HASH_PREFIX_1, repIndex, i, params);

    if (!haveNode(tree, 2 * i + 1)) {
      /* left child = H_left(seed_i || salt || t || i) */
      memcpy(&tree->nodes[(2 * i + 1) * seed_size], tmp, seed_size);
      markNode(tree, 2 * i + 1);
    }

    /* The last non-leaf node will only have a left child when there are an odd number of leaves */
    if (existsNotHaveNode(tree, 2 * i + 2)) {
      /* right child = H_right(seed_i || salt || t || i)  */
      memcpy(&tree->nodes[(2 * i + 2) * seed_size], tmp + seed_size, seed_size);
      markNode(tree, 2 * i + 2);
    }
  }
  /* now hash in groups of 4 for faster hashing */
  for (; i <= lastNonLeaf / 4 * 4; i += 4) {
    hashSeed_x4(tmp_ptr, tree, salt, HASH_PREFIX_1, repIndex, i, params);

    for (size_t j = i; j < i + 4; j++) {
      if (!haveNode(tree, j)) {
        continue;
      }
      if (!haveNode(tree, 2 * j + 1)) {
        /* left child = H_left(seed_i || salt || t || j) */
        memcpy(&tree->nodes[(2 * j + 1) * seed_size], tmp_ptr[j - i], seed_size);
        markNode(tree, 2 * j + 1);
      }

      /* The last non-leaf node will only have a left child when there are an odd number of leaves
       */
      if (existsNotHaveNode(tree, 2 * j + 2)) {
        /* right child = H_right(seed_i || salt || t || j)  */
        memcpy(&tree->nodes[(2 * j + 2) * seed_size], tmp_ptr[j - i] + seed_size, seed_size);
        markNode(tree, 2 * j + 2);
      }
    }
  }
  /* handle last few, which are not a multiple of 4 */
  for (; i <= lastNonLeaf; i++) {
    if (!haveNode(tree, i)) {
      continue;
    }

    hashSeed(tmp, &tree->nodes[i * seed_size], salt, HASH_PREFIX_1, repIndex, i, params);

    if (!haveNode(tree, 2 * i + 1)) {
      /* left child = H_left(seed_i || salt || t || i) */
      memcpy(&tree->nodes[(2 * i + 1) * seed_size], tmp, seed_size);
      markNode(tree, 2 * i + 1);
    }

    /* The last non-leaf node will only have a left child when there are an odd number of leaves */
    if (existsNotHaveNode(tree, 2 * i + 2)) {
      /* right child = H_right(seed_i || salt || t || i)  */
      memcpy(&tree->nodes[(2 * i + 2) * seed_size], tmp + seed_size, seed_size);
      markNode(tree, 2 * i + 2);
    }
  }
}

tree_t* generateSeeds(size_t nSeeds, uint8_t* rootSeed, uint8_t* salt, size_t repIndex,
                      const picnic_instance_t* params) {
  tree_t* tree = createTree(nSeeds, params->seed_size);

  memcpy(tree->nodes, rootSeed, params->seed_size);
  markNode(tree, 0);
  expandSeeds(tree, salt, repIndex, params);

  return tree;
}

static int isLeafNode(tree_t* tree, size_t node) {
  return (2 * node + 1 >= tree->numNodes);
}

static int hasSibling(tree_t* tree, size_t node) {
  if (!exists(tree, node)) {
    return 0;
  }

  if (isLeftChild(node) && !exists(tree, node + 1)) {
    return 0;
  }

  return 1;
}

static size_t getSibling(tree_t* tree, size_t node) {
  assert(node < tree->numNodes);
  assert(node != 0);
  assert(hasSibling(tree, node));

  if (isLeftChild(node)) {
    if (node + 1 < tree->numNodes) {
      return node + 1;
    } else {
      assert(!"getSibling: request for node with not sibling");
      return 0;
    }
  } else {
    return node - 1;
  }
}

/* Returns the number of bytes written to output */
static size_t* getRevealedNodes(tree_t* tree, uint16_t* hideList, size_t hideListSize,
                                size_t* outputSize) {
  /* Compute paths up from hideList to root, store as sets of nodes */
  size_t pathLen = tree->depth - 1;

  /* pathSets[i][0...hideListSize] stores the nodes in the path at depth i
   * for each of the leaf nodes in hideListSize */
  size_t** pathSets = malloc(pathLen * sizeof(size_t*));
  size_t* slab      = malloc(hideListSize * pathLen * sizeof(size_t));

  for (size_t i = 0; i < pathLen; i++) {
    pathSets[i] = slab;
    slab += hideListSize;
  }

  /* Compute the paths back to the root */
  for (size_t i = 0; i < hideListSize; i++) {
    size_t pos = 0;
    size_t node =
        hideList[i] +
        (tree->numNodes - tree->numLeaves); /* input lists leaf indexes, translate to nodes */
    pathSets[pos][i] = node;
    pos++;
    while ((node = getParent(node)) != 0) {
      pathSets[pos][i] = node;
      pos++;
    }
  }

  /* Determine seeds to reveal */
  size_t* revealed   = malloc(tree->numLeaves * sizeof(size_t));
  size_t revealedPos = 0;
  for (size_t d = 0; d < pathLen; d++) {
    for (size_t i = 0; i < hideListSize; i++) {
      if (!hasSibling(tree, pathSets[d][i])) {
        continue;
      }
      size_t sibling = getSibling(tree, pathSets[d][i]);
      if (!contains(pathSets[d], hideListSize, sibling)) {
        // Determine the seed to reveal
        while (!hasRightChild(tree, sibling) && !isLeafNode(tree, sibling)) {
          sibling = 2 * sibling + 1; // sibling = leftChild(sibling)
        }
        // Only reveal if we haven't already
        if (!contains(revealed, revealedPos, sibling)) {
          revealed[revealedPos] = sibling;
          revealedPos++;
        }
      }
    }
  }

  free(pathSets[0]);
  free(pathSets);

  *outputSize = revealedPos;
  return revealed;
}

size_t revealSeedsSize(size_t numNodes, uint16_t* hideList, size_t hideListSize,
                       const picnic_instance_t* params) {
  tree_t* tree            = createTree(numNodes, params->seed_size);
  size_t numNodesRevealed = 0;
  size_t* revealed        = getRevealedNodes(tree, hideList, hideListSize, &numNodesRevealed);

  freeTree(tree);
  free(revealed);
  return numNodesRevealed * params->seed_size;
}

size_t revealSeeds(tree_t* tree, uint16_t* hideList, size_t hideListSize, uint8_t* output,
                   size_t outputSize, const picnic_instance_t* params) {
  uint8_t* outputBase = output;
  size_t revealedSize = 0;

  if (outputSize > INT_MAX) {
    return -1;
  }
  int outLen = (int)outputSize;

  size_t* revealed = getRevealedNodes(tree, hideList, hideListSize, &revealedSize);
  for (size_t i = 0; i < revealedSize; i++) {
    outLen -= params->seed_size;
    if (outLen < 0) {
      assert(!"Insufficient sized buffer provided to revealSeeds");
      free(revealed);
      return 0;
    }
    memcpy(output, &tree->nodes[revealed[i] * params->seed_size], params->seed_size);
    output += params->seed_size;
  }

  free(revealed);
  return output - outputBase;
}

int reconstructSeeds(tree_t* tree, uint16_t* hideList, size_t hideListSize, uint8_t* input,
                     size_t inputLen, uint8_t* salt, size_t repIndex,
                     const picnic_instance_t* params) {
  int ret = 0;

  if (inputLen > INT_MAX) {
    return -1;
  }
  int inLen = (int)inputLen;

  size_t revealedSize = 0;
  size_t* revealed    = getRevealedNodes(tree, hideList, hideListSize, &revealedSize);
  for (size_t i = 0; i < revealedSize; i++) {
    inLen -= params->seed_size;
    if (inLen < 0) {
      ret = -1;
      goto Exit;
    }
    memcpy(&tree->nodes[revealed[i] * params->seed_size], input, params->seed_size);
    markNode(tree, revealed[i]);
    input += params->seed_size;
  }

  expandSeeds(tree, salt, repIndex, params);

Exit:
  free(revealed);
  return ret;
}

static void computeParentHash(tree_t* tree, size_t child, uint8_t* salt,
                              const picnic_instance_t* params) {
  if (!exists(tree, child)) {
    return;
  }

  size_t parent = getParent(child);

  if (haveNode(tree, parent)) {
    return;
  }

  /* Compute the hash for parent, if we have everything */
  if (!haveNode(tree, 2 * parent + 1)) {
    return;
  }

  if (existsNotHaveNode(tree, 2 * parent + 2)) {
    return;
  }

  /* Compute parent data = H(left child data || [right child data] || salt || parent idx) */
  hash_context ctx;

  hash_init_prefix(&ctx, params->digest_size, HASH_PREFIX_3);
  hash_update(&ctx, &tree->nodes[(2 * parent + 1) * params->digest_size], params->digest_size);
  if (hasRightChild(tree, parent)) {
    /* One node may not have a right child when there's an odd number of leaves */
    hash_update(&ctx, &tree->nodes[(2 * parent + 2) * params->digest_size], params->digest_size);
  }

  hash_update(&ctx, salt, SALT_SIZE);
  hash_update_uint16_le(&ctx, parent);
  hash_final(&ctx);
  hash_squeeze(&ctx, &tree->nodes[parent * params->digest_size], params->digest_size);
  hash_clear(&ctx);
  markNode(tree, parent);
}

/* Create a Merkle tree by hashing up all nodes.
 * leafData must have length tree->numNodes, but some may be NULL. */
void buildMerkleTree(tree_t* tree, uint8_t** leafData, uint8_t* salt,
                     const picnic_instance_t* params) {
  size_t firstLeaf = tree->numNodes - tree->numLeaves;

  /* Copy data to the leaves. The actual data being committed to has already been
   * hashed, according to the spec. */
  for (size_t i = 0; i < tree->numLeaves; i++) {
    if (leafData[i] != NULL) {
      memcpy(&tree->nodes[(firstLeaf + i) * tree->dataSize], leafData[i], tree->dataSize);
      markNode(tree, firstLeaf + i);
    }
  }
  /* Starting at the leaves, work up the tree, computing the hashes for intermediate nodes */
  for (int i = (int)tree->numNodes; i > 0; i--) {
    computeParentHash(tree, i, salt, params);
  }
}

/* Note that we never output the root node */
static size_t* getRevealedMerkleNodes(tree_t* tree, uint16_t* missingLeaves,
                                      size_t missingLeavesSize, size_t* outputSize) {
  size_t firstLeaf      = tree->numNodes - tree->numLeaves;
  uint8_t* missingNodes = calloc(tree->numNodes, 1);

  /* Mark leaves that are missing */
  for (size_t i = 0; i < missingLeavesSize; i++) {
    missingNodes[firstLeaf + missingLeaves[i]] = 1;
  }

  /* For the nonleaf nodes, if both leaves are missing, mark it as missing too */
  int lastNonLeaf = getParent(tree->numNodes - 1);
  for (int i = lastNonLeaf; i > 0; i--) {
    if (!exists(tree, i)) {
      continue;
    }
    if (exists(tree, 2 * i + 2)) {
      if (missingNodes[2 * i + 1] && missingNodes[2 * i + 2]) {
        missingNodes[i] = 1;
      }
    } else {
      if (missingNodes[2 * i + 1]) {
        missingNodes[i] = 1;
      }
    }
  }

  /* For each missing leaf node, add the highest missing node on the path
   * back to the root to the set to be revealed */
  size_t* revealed = malloc(tree->numLeaves * sizeof(size_t));
  size_t pos       = 0;
  for (size_t i = 0; i < missingLeavesSize; i++) {
    size_t node = missingLeaves[i] + firstLeaf; /* input is leaf indexes, translate to nodes */
    do {
      if (!missingNodes[getParent(node)]) {
        if (!contains(revealed, pos, node)) {
          revealed[pos] = node;
          pos++;
        }
        break;
      }
    } while ((node = getParent(node)) != 0);
  }

  free(missingNodes);
  *outputSize = pos;
  return revealed;
}

size_t openMerkleTreeSize(size_t numNodes, uint16_t* missingLeaves, size_t missingLeavesSize,
                          const picnic_instance_t* params) {

  tree_t* tree        = createTree(numNodes, params->digest_size);
  size_t revealedSize = 0;
  size_t* revealed = getRevealedMerkleNodes(tree, missingLeaves, missingLeavesSize, &revealedSize);

  freeTree(tree);
  free(revealed);

  return revealedSize * params->digest_size;
}

/* Serialze the missing nodes that the verifier will require to check commitments for non-missing
 * leaves */
uint8_t* openMerkleTree(tree_t* tree, uint16_t* missingLeaves, size_t missingLeavesSize,
                        size_t* outputSizeBytes) {
  size_t revealedSize = 0;
  size_t* revealed = getRevealedMerkleNodes(tree, missingLeaves, missingLeavesSize, &revealedSize);

  /* Serialize output */
  *outputSizeBytes    = revealedSize * tree->dataSize;
  uint8_t* output     = malloc(*outputSizeBytes);
  uint8_t* outputBase = output;

  for (size_t i = 0; i < revealedSize; i++) {
    memcpy(output, &tree->nodes[revealed[i] * tree->dataSize], tree->dataSize);
    output += tree->dataSize;
  }

  free(revealed);

  return outputBase;
}

/* addMerkleNodes: deserialize and add the data for nodes provided by the committer */
int addMerkleNodes(tree_t* tree, uint16_t* missingLeaves, size_t missingLeavesSize, uint8_t* input,
                   size_t inputSize) {
  int ret = 0;

  assert(missingLeavesSize < tree->numLeaves);

  if (inputSize > INT_MAX) {
    return -1;
  }
  int intLen = (int)inputSize;

  size_t revealedSize = 0;
  size_t* revealed = getRevealedMerkleNodes(tree, missingLeaves, missingLeavesSize, &revealedSize);
  assert(!contains(revealed, revealedSize, 0));

  /* Deserialize input */
  for (size_t i = 0; i < revealedSize; i++) {
    intLen -= tree->dataSize;
    if (intLen < 0) {
      ret = -1;
      goto Exit;
    }
    memcpy(&tree->nodes[revealed[i] * tree->dataSize], input, tree->dataSize);
    input += tree->dataSize;
    markNode(tree, revealed[i]);
  }

  if (intLen != 0) {
    ret = -1;
    goto Exit;
  }

Exit:

  free(revealed);

  return ret;
}

/* verifyMerkleTree: verify for each leaf that is set */
int verifyMerkleTree(tree_t* tree, /* uint16_t* missingLeaves, size_t missingLeavesSize, */
                     uint8_t** leafData, uint8_t* salt, const picnic_instance_t* params) {
  size_t firstLeaf = tree->numNodes - tree->numLeaves;

  /* Copy the leaf data, where we have it. The actual data being committed to has already been
   * hashed, according to the spec. */
  for (size_t i = 0; i < tree->numLeaves; i++) {
    if (leafData[i] != NULL) {
      if (haveNode(tree, firstLeaf + i)) {
        return -1; /* A leaf was assigned from the prover for a node we've recomputed */
      }

      if (leafData[i] != NULL) {
        memcpy(&tree->nodes[(firstLeaf + i) * tree->dataSize], leafData[i], tree->dataSize);
        markNode(tree, firstLeaf + i);
      }
    }
  }

  /* At this point the tree has some of the leaves, and some intermediate nodes
   * Work up the tree, computing all nodes we don't have that are missing. */
  for (int i = (int)tree->numNodes; i > 0; i--) {
    computeParentHash(tree, i, salt, params);
  }

  /* Fail if the root was not computed. */
  if (!haveNode(tree, 0)) {
    return -1;
  }

  return 0;
}
