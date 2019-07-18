#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "address.h"
#include "fors.h"
#include "hash.h"
#include "hash_state.h"
#include "thash.h"
#include "utils.h"

static void fors_gen_sk(unsigned char *sk, const unsigned char *sk_seed,
                        uint32_t fors_leaf_addr[8], const hash_state *hash_state_seeded) {
    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_prf_addr(
        sk, sk_seed, fors_leaf_addr, hash_state_seeded);
}

static void fors_sk_to_leaf(unsigned char *leaf, const unsigned char *sk,
                            const unsigned char *pub_seed,
                            uint32_t fors_leaf_addr[8],
                            const hash_state *hash_state_seeded) {
    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_thash_1(
        leaf, sk, pub_seed, fors_leaf_addr, hash_state_seeded);
}

static void fors_gen_leaf(unsigned char *leaf, const unsigned char *sk_seed,
                          const unsigned char *pub_seed,
                          uint32_t addr_idx, const uint32_t fors_tree_addr[8],
                          const hash_state *hash_state_seeded) {
    uint32_t fors_leaf_addr[8] = {0};

    /* Only copy the parts that must be kept in fors_leaf_addr. */
    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_copy_keypair_addr(
        fors_leaf_addr, fors_tree_addr);
    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_set_type(
        fors_leaf_addr, SPX_ADDR_TYPE_FORSTREE);
    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_set_tree_index(
        fors_leaf_addr, addr_idx);

    fors_gen_sk(leaf, sk_seed, fors_leaf_addr, hash_state_seeded);
    fors_sk_to_leaf(leaf, leaf, pub_seed, fors_leaf_addr, hash_state_seeded);
}

/**
 * Interprets m as SPX_FORS_HEIGHT-bit unsigned integers.
 * Assumes m contains at least SPX_FORS_HEIGHT * SPX_FORS_TREES bits.
 * Assumes indices has space for SPX_FORS_TREES integers.
 */
static void message_to_indices(uint32_t *indices, const unsigned char *m) {
    unsigned int i, j;
    unsigned int offset = 0;

    for (i = 0; i < SPX_FORS_TREES; i++) {
        indices[i] = 0;
        for (j = 0; j < SPX_FORS_HEIGHT; j++) {
            indices[i] ^= (((uint32_t)m[offset >> 3] >> (offset & 0x7)) & 0x1) << j;
            offset++;
        }
    }
}

/**
 * Signs a message m, deriving the secret key from sk_seed and the FTS address.
 * Assumes m contains at least SPX_FORS_HEIGHT * SPX_FORS_TREES bits.
 */
void PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_fors_sign(
    unsigned char *sig, unsigned char *pk,
    const unsigned char *m,
    const unsigned char *sk_seed, const unsigned char *pub_seed,
    const uint32_t fors_addr[8], const hash_state *hash_state_seeded) {
    uint32_t indices[SPX_FORS_TREES];
    unsigned char roots[SPX_FORS_TREES * SPX_N];
    uint32_t fors_tree_addr[8] = {0};
    uint32_t fors_pk_addr[8] = {0};
    uint32_t idx_offset;
    unsigned int i;

    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_copy_keypair_addr(
        fors_tree_addr, fors_addr);
    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_copy_keypair_addr(
        fors_pk_addr, fors_addr);

    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_set_type(
        fors_tree_addr, SPX_ADDR_TYPE_FORSTREE);
    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_set_type(
        fors_pk_addr, SPX_ADDR_TYPE_FORSPK);

    message_to_indices(indices, m);

    for (i = 0; i < SPX_FORS_TREES; i++) {
        idx_offset = i * (1 << SPX_FORS_HEIGHT);

        PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_set_tree_height(
            fors_tree_addr, 0);
        PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_set_tree_index(
            fors_tree_addr, indices[i] + idx_offset);

        /* Include the secret key part that produces the selected leaf node. */
        fors_gen_sk(sig, sk_seed, fors_tree_addr, hash_state_seeded);
        sig += SPX_N;

        /* Compute the authentication path for this leaf node. */
        PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_treehash_FORS_HEIGHT(
            roots + i * SPX_N, sig, sk_seed, pub_seed,
            indices[i], idx_offset, fors_gen_leaf, fors_tree_addr,
            hash_state_seeded);
        sig += SPX_N * SPX_FORS_HEIGHT;
    }

    /* Hash horizontally across all tree roots to derive the public key. */
    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_thash_FORS_TREES(
        pk, roots, pub_seed, fors_pk_addr, hash_state_seeded);
}

/**
 * Derives the FORS public key from a signature.
 * This can be used for verification by comparing to a known public key, or to
 * subsequently verify a signature on the derived public key. The latter is the
 * typical use-case when used as an FTS below an OTS in a hypertree.
 * Assumes m contains at least SPX_FORS_HEIGHT * SPX_FORS_TREES bits.
 */
void PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_fors_pk_from_sig(
    unsigned char *pk,
    const unsigned char *sig, const unsigned char *m,
    const unsigned char *pub_seed, const uint32_t fors_addr[8],
    const hash_state *hash_state_seeded) {
    uint32_t indices[SPX_FORS_TREES];
    unsigned char roots[SPX_FORS_TREES * SPX_N];
    unsigned char leaf[SPX_N];
    uint32_t fors_tree_addr[8] = {0};
    uint32_t fors_pk_addr[8] = {0};
    uint32_t idx_offset;
    unsigned int i;

    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_copy_keypair_addr(
        fors_tree_addr, fors_addr);
    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_copy_keypair_addr(
        fors_pk_addr, fors_addr);

    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_set_type(
        fors_tree_addr, SPX_ADDR_TYPE_FORSTREE);
    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_set_type(
        fors_pk_addr, SPX_ADDR_TYPE_FORSPK);

    message_to_indices(indices, m);

    for (i = 0; i < SPX_FORS_TREES; i++) {
        idx_offset = i * (1 << SPX_FORS_HEIGHT);

        PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_set_tree_height(
            fors_tree_addr, 0);
        PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_set_tree_index(
            fors_tree_addr, indices[i] + idx_offset);

        /* Derive the leaf from the included secret key part. */
        fors_sk_to_leaf(leaf, sig, pub_seed, fors_tree_addr, hash_state_seeded);
        sig += SPX_N;

        /* Derive the corresponding root node of this tree. */
        PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_compute_root(
            roots + i * SPX_N, leaf, indices[i], idx_offset, sig,
            SPX_FORS_HEIGHT, pub_seed, fors_tree_addr, hash_state_seeded);
        sig += SPX_N * SPX_FORS_HEIGHT;
    }

    /* Hash horizontally across all tree roots to derive the public key. */
    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_thash_FORS_TREES(
        pk, roots, pub_seed, fors_pk_addr, hash_state_seeded);
}
