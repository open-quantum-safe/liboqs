#include <stddef.h>
#include <string.h>

#include "address.h"
#include "hash.h"
#include "hash_state.h"
#include "params.h"
#include "thash.h"
#include "utils.h"

/**
 * Converts the value of 'in' to 'outlen' bytes in big-endian byte order.
 */
void PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_ull_to_bytes(
    unsigned char *out, size_t outlen, unsigned long long in) {

    /* Iterate over out in decreasing order, for big-endianness. */
    for (size_t i = outlen; i > 0; i--) {
        out[i - 1] = in & 0xff;
        in = in >> 8;
    }
}

/**
 * Converts the inlen bytes in 'in' from big-endian byte order to an integer.
 */
unsigned long long PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_bytes_to_ull(
    const unsigned char *in, size_t inlen) {
    unsigned long long retval = 0;

    for (size_t i = 0; i < inlen; i++) {
        retval |= ((unsigned long long)in[i]) << (8 * (inlen - 1 - i));
    }
    return retval;
}

/**
 * Computes a root node given a leaf and an auth path.
 * Expects address to be complete other than the tree_height and tree_index.
 */
void PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_compute_root(
    unsigned char *root, const unsigned char *leaf,
    uint32_t leaf_idx, uint32_t idx_offset,
    const unsigned char *auth_path, uint32_t tree_height,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {
    uint32_t i;
    unsigned char buffer[2 * SPX_N];

    /* If leaf_idx is odd (last bit = 1), current path element is a right child
       and auth_path has to go left. Otherwise it is the other way around. */
    if (leaf_idx & 1) {
        memcpy(buffer + SPX_N, leaf, SPX_N);
        memcpy(buffer, auth_path, SPX_N);
    } else {
        memcpy(buffer, leaf, SPX_N);
        memcpy(buffer + SPX_N, auth_path, SPX_N);
    }
    auth_path += SPX_N;

    for (i = 0; i < tree_height - 1; i++) {
        leaf_idx >>= 1;
        idx_offset >>= 1;
        /* Set the address of the node we're creating. */
        PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_set_tree_height(addr, i + 1);
        PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_set_tree_index(
            addr, leaf_idx + idx_offset);

        /* Pick the right or left neighbor, depending on parity of the node. */
        if (leaf_idx & 1) {
            PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_thash_2(
                buffer + SPX_N, buffer, pub_seed, addr, hash_state_seeded);
            memcpy(buffer, auth_path, SPX_N);
        } else {
            PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_thash_2(
                buffer, buffer, pub_seed, addr, hash_state_seeded);
            memcpy(buffer + SPX_N, auth_path, SPX_N);
        }
        auth_path += SPX_N;
    }

    /* The last iteration is exceptional; we do not copy an auth_path node. */
    leaf_idx >>= 1;
    idx_offset >>= 1;
    PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_set_tree_height(addr, tree_height);
    PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_set_tree_index(
        addr, leaf_idx + idx_offset);
    PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_thash_2(
        root, buffer, pub_seed, addr, hash_state_seeded);
}

/**
 * For a given leaf index, computes the authentication path and the resulting
 * root node using Merkle's TreeHash algorithm.
 * Expects the layer and tree parts of the tree_addr to be set, as well as the
 * tree type (i.e. SPX_ADDR_TYPE_HASHTREE or SPX_ADDR_TYPE_FORSTREE).
 * Applies the offset idx_offset to indices before building addresses, so that
 * it is possible to continue counting indices across trees.
 */
static void PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_treehash(
    unsigned char *root, unsigned char *auth_path,
    unsigned char *stack, unsigned int *heights,
    const unsigned char *sk_seed, const unsigned char *pub_seed,
    uint32_t leaf_idx, uint32_t idx_offset, uint32_t tree_height,
    void (*gen_leaf)(
        unsigned char * /* leaf */,
        const unsigned char * /* sk_seed */,
        const unsigned char * /* pub_seed */,
        uint32_t /* addr_idx */, const uint32_t[8] /* tree_addr */,
        const hash_state * /* hash_state_seeded */),
    uint32_t tree_addr[8],
    const hash_state *hash_state_seeded) {

    unsigned int offset = 0;
    uint32_t idx;
    uint32_t tree_idx;

    for (idx = 0; idx < (uint32_t)(1 << tree_height); idx++) {
        /* Add the next leaf node to the stack. */
        gen_leaf(stack + offset * SPX_N,
                 sk_seed, pub_seed, idx + idx_offset, tree_addr,
                 hash_state_seeded);
        offset++;
        heights[offset - 1] = 0;

        /* If this is a node we need for the auth path.. */
        if ((leaf_idx ^ 0x1) == idx) {
            memcpy(auth_path, stack + (offset - 1)*SPX_N, SPX_N);
        }

        /* While the top-most nodes are of equal height.. */
        while (offset >= 2 && heights[offset - 1] == heights[offset - 2]) {
            /* Compute index of the new node, in the next layer. */
            tree_idx = (idx >> (heights[offset - 1] + 1));

            /* Set the address of the node we're creating. */
            PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_set_tree_height(
                tree_addr, heights[offset - 1] + 1);
            PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_set_tree_index(
                tree_addr, tree_idx + (idx_offset >> (heights[offset - 1] + 1)));
            /* Hash the top-most nodes from the stack together. */
            PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_thash_2(
                stack + (offset - 2)*SPX_N, stack + (offset - 2)*SPX_N,
                pub_seed, tree_addr, hash_state_seeded);
            offset--;
            /* Note that the top-most node is now one layer higher. */
            heights[offset - 1]++;

            /* If this is a node we need for the auth path.. */
            if (((leaf_idx >> heights[offset - 1]) ^ 0x1) == tree_idx) {
                memcpy(auth_path + heights[offset - 1]*SPX_N,
                       stack + (offset - 1)*SPX_N, SPX_N);
            }
        }
    }
    memcpy(root, stack, SPX_N);
}

/* The wrappers below ensure that we use fixed-size buffers on the stack */

void PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_treehash_FORS_HEIGHT(
    unsigned char *root, unsigned char *auth_path,
    const unsigned char *sk_seed, const unsigned char *pub_seed,
    uint32_t leaf_idx, uint32_t idx_offset,
    void (*gen_leaf)(
        unsigned char * /* leaf */,
        const unsigned char * /* sk_seed */,
        const unsigned char * /* pub_seed */,
        uint32_t /* addr_idx */, const uint32_t[8] /* tree_addr */,
        const hash_state * /* hash_state_seeded */),
    uint32_t tree_addr[8], const hash_state *hash_state_seeded) {

    unsigned char stack[(SPX_FORS_HEIGHT + 1)*SPX_N];
    unsigned int heights[SPX_FORS_HEIGHT + 1];

    PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_treehash(
        root, auth_path, stack, heights, sk_seed, pub_seed,
        leaf_idx, idx_offset, SPX_FORS_HEIGHT, gen_leaf, tree_addr, hash_state_seeded);
}

void PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_treehash_TREE_HEIGHT(
    unsigned char *root, unsigned char *auth_path,
    const unsigned char *sk_seed, const unsigned char *pub_seed,
    uint32_t leaf_idx, uint32_t idx_offset,
    void (*gen_leaf)(
        unsigned char * /* leaf */,
        const unsigned char * /* sk_seed */,
        const unsigned char * /* pub_seed */,
        uint32_t /* addr_idx */, const uint32_t[8] /* tree_addr */,
        const hash_state * /* hash_state_seeded */),
    uint32_t tree_addr[8], const hash_state *hash_state_seeded) {

    unsigned char stack[(SPX_TREE_HEIGHT + 1)*SPX_N];
    unsigned int heights[SPX_TREE_HEIGHT + 1];

    PQCLEAN_SPHINCSHARAKA256FROBUST_CLEAN_treehash(
        root, auth_path, stack, heights, sk_seed, pub_seed,
        leaf_idx, idx_offset, SPX_TREE_HEIGHT, gen_leaf, tree_addr, hash_state_seeded);
}
