#ifndef PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_ADDRESS_H
#define PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_ADDRESS_H

#include <stdint.h>

#define PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_ADDR_TYPE_WOTS 0
#define PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_ADDR_TYPE_WOTSPK 1
#define PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_ADDR_TYPE_HASHTREE 2
#define PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_ADDR_TYPE_FORSTREE 3
#define PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_ADDR_TYPE_FORSPK 4

void PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_addr_to_bytes(
    unsigned char *bytes, const uint32_t addr[8]);

void PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_set_layer_addr(
    uint32_t addr[8], uint32_t layer);

void PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_set_tree_addr(
    uint32_t addr[8], uint64_t tree);

void PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_set_type(
    uint32_t addr[8], uint32_t type);

/* Copies the layer and tree part of one address into the other */
void PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_copy_subtree_addr(
    uint32_t out[8], const uint32_t in[8]);

/* These functions are used for WOTS and FORS addresses. */

void PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_set_keypair_addr(
    uint32_t addr[8], uint32_t keypair);

void PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_set_chain_addr(
    uint32_t addr[8], uint32_t chain);

void PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_set_hash_addr(
    uint32_t addr[8], uint32_t hash);

void PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_copy_keypair_addr(
    uint32_t out[8], const uint32_t in[8]);

/* These functions are used for all hash tree addresses (including FORS). */

void PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_set_tree_height(
    uint32_t addr[8], uint32_t tree_height);

void PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_set_tree_index(
    uint32_t addr[8], uint32_t tree_index);

#endif
