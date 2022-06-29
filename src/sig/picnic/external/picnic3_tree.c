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

static inline bitset_word_t get_bit(const bitset_word_t* array, size_t index) {
  return array[index / (sizeof(bitset_word_t) * 8)] >> (index % (sizeof(bitset_word_t) * 8)) & 0x1;
}

static inline void set_bit(bitset_word_t* array, size_t index) {
  array[index / (sizeof(bitset_word_t) * 8)] |= BITSET_WORD_C(1)
                                                << (index % (sizeof(bitset_word_t) * 8));
}

#if defined(WITH_OPT) && defined(WITH_SSE2) && (defined(__x86_64__) || defined(_M_X64))
#include "simd.h"
#endif

static bool contains(unsigned int* list, size_t len, unsigned int value) {
  size_t i = 0;
#if defined(WITH_OPT) && defined(WITH_SSE2) && (defined(__x86_64__) || defined(_M_X64))
  const size_t len4    = (len / 4) * 4;
  const word128 value4 = _mm_set1_epi32(value);

  for (; i < len4; i += 4) {
    const word128 tmp = _mm_cmpeq_epi32(value4, mm128_loadu(&list[i]));
    if (_mm_movemask_epi8(tmp)) {
      return true;
    }
  }
#endif
  for (; i < len; i++) {
    if (list[i] == value) {
      return true;
    }
  }
  return false;
}

/* Check if a node exists */
static bitset_word_t exists(const tree_t* tree, unsigned int i) {
  if (i >= tree->numNodes) {
    return 0;
  }
  return get_bit(tree->haveNodeExists, 2 * i);
}

/* Check if we have data for a node */
static bool haveNode(const tree_t* tree, unsigned int i) {
  return get_bit(tree->haveNodeExists, 2 * i + 1);
}

/* Mark a node has having data */
static void markNode(tree_t* tree, unsigned int i) {
  set_bit(tree->haveNodeExists, 2 * i + 1);
}

/* Check if a node exists but does not have data */
static bool existsNotHaveNode(const tree_t* tree, unsigned int i) {
  return (tree->haveNodeExists[2 * i / (sizeof(bitset_word_t) * 8)] >>
              (2 * i % (sizeof(bitset_word_t) * 8)) &
          0x3) == 0x01;
}

static void initNodes(tree_t* tree) {
#if BITSET_WORD_MAX == UINT64_MAX
  if (tree->numLeaves == 16) {
    tree->haveNodeExists[0] = BITSET_WORD_C(0x1555555555555555);
    return;
  }
#elif BITSET_WORD_MAX == UINT32_MAX
  if (tree->numLeaves == 16) {
    tree->haveNodeExists[0] = BITSET_WORD_C(0x55555555);
    tree->haveNodeExists[1] = BITSET_WORD_C(0x15555555);
    return;
  }
#endif

  const unsigned int num_nodes_twice = 2 * tree->numNodes;
  unsigned int node                  = 2 * (tree->numNodes - tree->numLeaves);
#if BITSET_WORD_MAX == UINT64_MAX || BITSET_WORD_MAX == UINT32_MAX
  /* Set leaves up to bitset_word_t boundary */
  for (; node < num_nodes_twice && node % (sizeof(bitset_word_t) * 8); node += 2) {
    set_bit(tree->haveNodeExists, node);
  }
  /* Set leaves taking a full bitset_word_t */
  const unsigned int num_nodes_twice_word_boundary =
      (num_nodes_twice / (sizeof(bitset_word_t) * 8)) * sizeof(bitset_word_t) * 8;
  for (; node < num_nodes_twice_word_boundary; node += sizeof(bitset_word_t) * 8) {
#if BITSET_WORD_MAX == UINT64_MAX
    tree->haveNodeExists[node / (sizeof(bitset_word_t) * 8)] = BITSET_WORD_C(0x5555555555555555);
#elif BITSET_WORD_MAX == UINT32_MAX
    tree->haveNodeExists[node / (sizeof(bitset_word_t) * 8)] = BITSET_WORD_C(0x55555555);
#endif
  }
#endif
  /* Set remaining leaves */
  for (; node < num_nodes_twice; node += 2) {
    set_bit(tree->haveNodeExists, node);
  }

  /* Build tree */
  for (unsigned int i = 2 * (tree->numNodes - tree->numLeaves); i > 0; i -= 2) {
    if (exists(tree, i + 1) || exists(tree, i + 2)) {
      set_bit(tree->haveNodeExists, i);
    }
  }
  set_bit(tree->haveNodeExists, 0);
}

bool createTree(tree_t* tree, unsigned int numLeaves, unsigned int dataSize) {
  if (!tree || !numLeaves) {
    return false;
  }

  unsigned int depth = ceil_log2(numLeaves) + 1;
  /* Num nodes in complete - number of missing leaves */
  unsigned int num_nodes = ((1 << depth) - 1) - ((1 << (depth - 1)) - numLeaves);

  tree->nodes = dataSize ? calloc(num_nodes, dataSize) : NULL;
  /* Depending on the number of leaves, the tree may not be complete */
  tree->haveNodeExists =
      calloc((2 * num_nodes + sizeof(bitset_word_t) * 8 - 1) / (sizeof(bitset_word_t) * 8),
             sizeof(bitset_word_t));
  if ((dataSize && !tree->nodes) || !tree->haveNodeExists) {
    clearTree(tree);
    return false;
  }

  tree->depth     = depth;
  tree->dataSize  = dataSize;
  tree->numNodes  = num_nodes;
  tree->numLeaves = numLeaves;

  initNodes(tree);
  return true;
}

void clearTree(tree_t* tree) {
  if (tree) {
    free(tree->haveNodeExists);
    free(tree->nodes);
  }
}

static bool isLeftChild(unsigned int node) {
  assert(node != 0);
  return (node % 2 == 1);
}

static bool hasRightChild(const tree_t* tree, unsigned int node) {
  return (2 * node + 2 < tree->numNodes && exists(tree, node));
}

static unsigned int getParent(unsigned int node) {
  assert(node != 0);
  return ((node + 1) >> 1) - 1;

  // if (isLeftChild(node)) {
  //  /* (node - 1) / 2, but since node % 2 == 1, that's the same as node / 2 */
  //  return node >> 1;
  //}
  // return (node - 2) / 2;
}

uint8_t* getLeaves(tree_t* tree) {
  return &tree->nodes[(tree->numNodes - tree->numLeaves) * tree->dataSize];
}

uint8_t* getLeaf(tree_t* tree, unsigned int leafIndex) {
  assert(leafIndex < tree->numLeaves);
  unsigned int firstLeaf = tree->numNodes - tree->numLeaves;
  return &tree->nodes[(firstLeaf + leafIndex) * tree->dataSize];
}

static void hashSeed(uint8_t* digest, const uint8_t* inputSeed, uint8_t* salt, uint8_t hashPrefix,
                     unsigned int repIndex, unsigned int nodeIndex,
                     const picnic_instance_t* params) {
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

static void hashSeed_x4(uint8_t** digest, const tree_t* tree, uint8_t* salt, uint8_t hashPrefix,
                        unsigned int repIndex, unsigned int nodeIndex,
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

static void expandSeeds(tree_t* tree, uint8_t* salt, unsigned int repIndex,
                        const picnic_instance_t* params) {
  const size_t seed_size = params->seed_size;

  uint8_t tmp[4 * 2 * MAX_SEED_SIZE_BYTES];
  uint8_t* tmp_ptr[4] = {&tmp[0], &tmp[2 * MAX_SEED_SIZE_BYTES], &tmp[2 * 2 * MAX_SEED_SIZE_BYTES],
                         &tmp[3 * 2 * MAX_SEED_SIZE_BYTES]};

  /* Walk the tree, expanding seeds where possible. Compute children of
   * non-leaf nodes. */
  const unsigned int lastNonLeaf = getParent(tree->numNodes - 1);
  unsigned int i                 = 0;
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

    for (unsigned int j = i; j < i + 4; j++) {
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

bool generateSeeds(tree_t* tree, unsigned int nSeeds, uint8_t* rootSeed, uint8_t* salt,
                   size_t repIndex, const picnic_instance_t* params) {
  if (!createTree(tree, nSeeds, params->seed_size)) {
    return false;
  }

  memcpy(tree->nodes, rootSeed, params->seed_size);
  markNode(tree, 0);
  expandSeeds(tree, salt, repIndex, params);

  return true;
}

static bool isLeafNode(const tree_t* tree, unsigned int node) {
  return 2 * node + 1 >= tree->numNodes;
}

static bool hasSibling(const tree_t* tree, unsigned int node) {
  return exists(tree, node) && (!isLeftChild(node) || exists(tree, node + 1));
}

static unsigned int getSibling(const tree_t* tree, unsigned int node) {
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
static unsigned int* getRevealedNodes(tree_t* tree, uint16_t* hideList, size_t hideListSize,
                                      size_t* outputSize) {
  /* Compute paths up from hideList to root, store as sets of nodes */
  const unsigned int pathLen = tree->depth - 1;

  /* pathSets[i][0...hideListSize] ~= pathSets[i * hideListSize + ...] stores the nodes in the path
   * at depth i for each of the leaf nodes in hideListSize */
  unsigned int* pathSets = calloc(hideListSize * pathLen, sizeof(unsigned int));

  /* Compute the paths back to the root */
  for (size_t i = 0; i < hideListSize; i++) {
    /* input lists leaf indexes, translate to nodes */
    unsigned int node                    = hideList[i] + (tree->numNodes - tree->numLeaves);
    pathSets[/* 0 * hideListSize + */ i] = node;
    unsigned int pos                     = 1;
    while ((node = getParent(node)) != 0) {
      pathSets[pos * hideListSize + i] = node;
      pos++;
    }
  }

  /* Determine seeds to reveal */
  unsigned int* revealed   = malloc(tree->numLeaves * sizeof(unsigned int));
  unsigned int revealedPos = 0;
  for (unsigned int d = 0; d < pathLen; d++) {
    for (size_t i = 0; i < hideListSize; i++) {
      unsigned int node = pathSets[d * hideListSize + i];
      if (!hasSibling(tree, node)) {
        continue;
      }
      unsigned int sibling = getSibling(tree, node);
      if (!contains(&pathSets[d * hideListSize], hideListSize, sibling)) {
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

  free(pathSets);

  *outputSize = revealedPos;
  return revealed;
}

size_t revealSeedsSize(unsigned int numNodes, uint16_t* hideList, size_t hideListSize,
                       const picnic_instance_t* params) {
  tree_t tree;
  if (!createTree(&tree, numNodes, 0)) {
    return SIZE_MAX;
  }

  size_t numNodesRevealed = 0;
  unsigned int* revealed  = getRevealedNodes(&tree, hideList, hideListSize, &numNodesRevealed);
  clearTree(&tree);
  free(revealed);
  return numNodesRevealed * params->seed_size;
}

size_t revealSeeds(tree_t* tree, uint16_t* hideList, size_t hideListSize, uint8_t* output,
                   size_t outputSize, const picnic_instance_t* params) {
  uint8_t* outputBase = output;
  size_t revealedSize = 0;

  unsigned int* revealed = getRevealedNodes(tree, hideList, hideListSize, &revealedSize);
  if (outputSize < params->seed_size * revealedSize) {
    assert(!"Insufficient sized buffer provided to revealSeeds");
    free(revealed);
    return 0;
  }

  for (size_t i = 0; i < revealedSize; i++) {
    memcpy(output, &tree->nodes[revealed[i] * params->seed_size], params->seed_size);
    output += params->seed_size;
  }

  free(revealed);
  return output - outputBase;
}

int reconstructSeeds(tree_t* tree, uint16_t* hideList, size_t hideListSize, uint8_t* input,
                     size_t inputLen, uint8_t* salt, unsigned int repIndex,
                     const picnic_instance_t* params) {
  int ret                = -1;
  size_t revealedSize    = 0;
  unsigned int* revealed = getRevealedNodes(tree, hideList, hideListSize, &revealedSize);
  if (!revealed || inputLen < revealedSize * params->seed_size) {
    goto Exit;
  }

  for (size_t i = 0; i < revealedSize; i++) {
    memcpy(&tree->nodes[revealed[i] * params->seed_size], input, params->seed_size);
    markNode(tree, revealed[i]);
    input += params->seed_size;
  }

  expandSeeds(tree, salt, repIndex, params);
  ret = 0;

Exit:
  free(revealed);
  return ret;
}

static void computeParentHash(tree_t* tree, unsigned int child, uint8_t* salt,
                              const picnic_instance_t* params) {
  if (!exists(tree, child)) {
    return;
  }

  unsigned int parent = getParent(child);
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
  unsigned int firstLeaf = tree->numNodes - tree->numLeaves;

  /* Copy data to the leaves. The actual data being committed to has already been
   * hashed, according to the spec. */
  for (unsigned int i = 0; i < tree->numLeaves; i++) {
    if (leafData[i] != NULL) {
      memcpy(&tree->nodes[(firstLeaf + i) * tree->dataSize], leafData[i], tree->dataSize);
      markNode(tree, firstLeaf + i);
    }
  }
  /* Starting at the leaves, work up the tree, computing the hashes for intermediate nodes */
  for (unsigned int i = tree->numNodes; i > 0; i--) {
    computeParentHash(tree, i, salt, params);
  }
}

/* Note that we never output the root node */
static unsigned int* getRevealedMerkleNodes(const tree_t* tree, uint16_t* missingLeaves,
                                            size_t missingLeavesSize, size_t* outputSize) {
  if (!missingLeaves && missingLeavesSize) {
    return NULL;
  }

  const unsigned int firstLeaf = tree->numNodes - tree->numLeaves;
  bitset_word_t* missingNodes =
      calloc((tree->numNodes + sizeof(bitset_word_t) * 8 - 1) / (sizeof(bitset_word_t) * 8),
             sizeof(bitset_word_t));
  if (!missingNodes) {
    return NULL;
  }

  /* Mark leaves that are missing */
  for (size_t i = 0; i < missingLeavesSize; i++) {
    set_bit(missingNodes, firstLeaf + missingLeaves[i]);
  }

  /* For the nonleaf nodes, if both leaves are missing, mark it as missing too */
  for (unsigned int i = getParent(tree->numNodes - 1); i > 0; i--) {
    if (!exists(tree, i)) {
      continue;
    }
    if (exists(tree, 2 * i + 2)) {
      if (get_bit(missingNodes, 2 * i + 1) && get_bit(missingNodes, 2 * i + 2)) {
        set_bit(missingNodes, i);
      }
    } else {
      if (get_bit(missingNodes, 2 * i + 1)) {
        set_bit(missingNodes, i);
      }
    }
  }

  /* For each missing leaf node, add the highest missing node on the path
   * back to the root to the set to be revealed */
  unsigned int* revealed = malloc(tree->numLeaves * sizeof(unsigned int));
  if (!revealed) {
    free(missingNodes);
    return NULL;
  }

  unsigned int pos = 0;
  for (unsigned int i = 0; i < missingLeavesSize; i++) {
    /* input is leaf indexes, translate to nodes */
    unsigned int node = missingLeaves[i] + firstLeaf;
    do {
      if (!get_bit(missingNodes, getParent(node))) {
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
  tree_t tree;
  if (!createTree(&tree, numNodes, params->digest_size)) {
    return SIZE_MAX;
  }

  size_t revealedSize = 0;
  unsigned int* revealed =
      getRevealedMerkleNodes(&tree, missingLeaves, missingLeavesSize, &revealedSize);

  clearTree(&tree);
  free(revealed);

  return revealedSize * params->digest_size;
}

/* Serialze the missing nodes that the verifier will require to check commitments for non-missing
 * leaves */
uint8_t* openMerkleTree(tree_t* tree, uint16_t* missingLeaves, size_t missingLeavesSize,
                        size_t* outputSizeBytes) {
  size_t revealedSize = 0;
  unsigned int* revealed =
      getRevealedMerkleNodes(tree, missingLeaves, missingLeavesSize, &revealedSize);
  if (!revealed) {
    return NULL;
  }

  /* Serialize output */
  *outputSizeBytes = revealedSize * tree->dataSize;
  uint8_t* output  = malloc(*outputSizeBytes);
  if (!output) {
    free(revealed);
    return NULL;
  }

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
  assert(missingLeavesSize < tree->numLeaves);

  int ret             = -1;
  size_t revealedSize = 0;
  unsigned int* revealed =
      getRevealedMerkleNodes(tree, missingLeaves, missingLeavesSize, &revealedSize);
  if (!revealed) {
    goto Exit;
  }
  assert(!contains(revealed, revealedSize, 0));

  if (inputSize != revealedSize * tree->dataSize) {
    goto Exit;
  }

  /* Deserialize input */
  for (size_t i = 0; i < revealedSize; i++) {
    memcpy(&tree->nodes[revealed[i] * tree->dataSize], input, tree->dataSize);
    input += tree->dataSize;
    markNode(tree, revealed[i]);
  }
  ret = 0;

Exit:
  free(revealed);

  return ret;
}

/* verifyMerkleTree: verify for each leaf that is set */
int verifyMerkleTree(tree_t* tree, /* uint16_t* missingLeaves, size_t missingLeavesSize, */
                     uint8_t** leafData, uint8_t* salt, const picnic_instance_t* params) {
  unsigned int firstLeaf = tree->numNodes - tree->numLeaves;

  /* Copy the leaf data, where we have it. The actual data being committed to has already been
   * hashed, according to the spec. */
  for (unsigned int i = 0; i < tree->numLeaves; i++) {
    if (leafData[i] != NULL) {
      if (haveNode(tree, firstLeaf + i)) {
        return -1; /* A leaf was assigned from the prover for a node we've recomputed */
      }

      memcpy(&tree->nodes[(firstLeaf + i) * tree->dataSize], leafData[i], tree->dataSize);
      markNode(tree, firstLeaf + i);
    }
  }

  /* At this point the tree has some of the leaves, and some intermediate nodes
   * Work up the tree, computing all nodes we don't have that are missing. */
  for (unsigned int i = tree->numNodes; i > 0; i--) {
    computeParentHash(tree, i, salt, params);
  }

  /* Fail if the root was not computed. */
  if (!haveNode(tree, 0)) {
    return -1;
  }

  return 0;
}
