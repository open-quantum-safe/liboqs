#include <stdint.h>

#include "address.h"
#include "params.h"
#include "utils.h"

void PQCLEAN_SPHINCSSHA256128FSIMPLE_CLEAN_addr_to_bytes(
    unsigned char *bytes, const uint32_t addr[8]) {
    int i;

    for (i = 0; i < 8; i++) {
        PQCLEAN_SPHINCSSHA256128FSIMPLE_CLEAN_ull_to_bytes(
            bytes + i * 4, 4, addr[i]);
    }
}

void PQCLEAN_SPHINCSSHA256128FSIMPLE_CLEAN_set_layer_addr(
    uint32_t addr[8], uint32_t layer) {
    addr[0] = layer;
}

void PQCLEAN_SPHINCSSHA256128FSIMPLE_CLEAN_set_tree_addr(
    uint32_t addr[8], uint64_t tree) {
    addr[1] = 0;
    addr[2] = (uint32_t) (tree >> 32);
    addr[3] = (uint32_t) tree;
}

void PQCLEAN_SPHINCSSHA256128FSIMPLE_CLEAN_set_type(
    uint32_t addr[8], uint32_t type) {
    addr[4] = type;
}

void PQCLEAN_SPHINCSSHA256128FSIMPLE_CLEAN_copy_subtree_addr(
    uint32_t out[8], const uint32_t in[8]) {
    out[0] = in[0];
    out[1] = in[1];
    out[2] = in[2];
    out[3] = in[3];
}

/* These functions are used for OTS addresses. */

void PQCLEAN_SPHINCSSHA256128FSIMPLE_CLEAN_set_keypair_addr(
    uint32_t addr[8], uint32_t keypair) {
    addr[5] = keypair;
}

void PQCLEAN_SPHINCSSHA256128FSIMPLE_CLEAN_copy_keypair_addr(
    uint32_t out[8], const uint32_t in[8]) {
    out[0] = in[0];
    out[1] = in[1];
    out[2] = in[2];
    out[3] = in[3];
    out[5] = in[5];
}

void PQCLEAN_SPHINCSSHA256128FSIMPLE_CLEAN_set_chain_addr(
    uint32_t addr[8], uint32_t chain) {
    addr[6] = chain;
}

void PQCLEAN_SPHINCSSHA256128FSIMPLE_CLEAN_set_hash_addr(
    uint32_t addr[8], uint32_t hash) {
    addr[7] = hash;
}

/* These functions are used for all hash tree addresses (including FORS). */

void PQCLEAN_SPHINCSSHA256128FSIMPLE_CLEAN_set_tree_height(
    uint32_t addr[8], uint32_t tree_height) {
    addr[6] = tree_height;
}

void PQCLEAN_SPHINCSSHA256128FSIMPLE_CLEAN_set_tree_index(
    uint32_t addr[8], uint32_t tree_index) {
    addr[7] = tree_index;
}
