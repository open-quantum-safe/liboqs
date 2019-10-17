#ifndef SPX_UTILS_H
#define SPX_UTILS_H

#include "hash_state.h"
#include "params.h"
#include <stddef.h>
#include <stdint.h>

/**
 * Converts the value of 'in' to 'outlen' bytes in big-endian byte order.
 */
void PQCLEAN_SPHINCSSHA256256FSIMPLE_CLEAN_ull_to_bytes(
    unsigned char *out, size_t outlen, unsigned long long in);

/**
 * Converts the inlen bytes in 'in' from big-endian byte order to an integer.
 */
unsigned long long PQCLEAN_SPHINCSSHA256256FSIMPLE_CLEAN_bytes_to_ull(
    const unsigned char *in, size_t inlen);

/**
 * Computes a root node given a leaf and an auth path.
 * Expects address to be complete other than the tree_height and tree_index.
 */
void PQCLEAN_SPHINCSSHA256256FSIMPLE_CLEAN_compute_root(
    unsigned char *root, const unsigned char *leaf,
    uint32_t leaf_idx, uint32_t idx_offset,
    const unsigned char *auth_path, uint32_t tree_height,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded);

/**
 * For a given leaf index, computes the authentication path and the resulting
 * root node using Merkle's TreeHash algorithm.
 * Expects the layer and tree parts of the tree_addr to be set, as well as the
 * tree type (i.e. SPX_ADDR_TYPE_HASHTREE or SPX_ADDR_TYPE_FORSTREE).
 * Applies the offset idx_offset to indices before building addresses, so that
 * it is possible to continue counting indices across trees.
 */
void PQCLEAN_SPHINCSSHA256256FSIMPLE_CLEAN_treehash_FORS_HEIGHT(
    unsigned char *root, unsigned char *auth_path,
    const unsigned char *sk_seed, const unsigned char *pub_seed,
    uint32_t leaf_idx, uint32_t idx_offset,
    void (*gen_leaf)(
        unsigned char * /* leaf */,
        const unsigned char * /* sk_seed */,
        const unsigned char * /* pub_seed */,
        uint32_t /* addr_idx */, const uint32_t[8] /* tree_addr */,
        const hash_state * /* hash_state_seeded */),
    uint32_t tree_addr[8], const hash_state *hash_state_seeded);

void PQCLEAN_SPHINCSSHA256256FSIMPLE_CLEAN_treehash_TREE_HEIGHT(
    unsigned char *root, unsigned char *auth_path,
    const unsigned char *sk_seed, const unsigned char *pub_seed,
    uint32_t leaf_idx, uint32_t idx_offset,
    void (*gen_leaf)(
        unsigned char * /* leaf */,
        const unsigned char * /* sk_seed */,
        const unsigned char * /* pub_seed */,
        uint32_t /* addr_idx */, const uint32_t[8] /* tree_addr */,
        const hash_state * /* hash_state_seeded */),
    uint32_t tree_addr[8], const hash_state *hash_state_seeded);

#endif
