#ifndef XMSS_HASH_ADDRESS_H
#define XMSS_HASH_ADDRESS_H

#include <stdint.h>

#define XMSS_ADDR_TYPE_OTS 0
#define XMSS_ADDR_TYPE_LTREE 1
#define XMSS_ADDR_TYPE_HASHTREE 2

void set_layer_addr(uint32_t addr[8], uint32_t layer);

void set_tree_addr(uint32_t addr[8], uint64_t tree);

void set_type(uint32_t addr[8], uint32_t type);

void set_key_and_mask(uint32_t addr[8], uint32_t key_and_mask);

/* Copies the layer and tree part of one address into the other */
void copy_subtree_addr(uint32_t out[8], const uint32_t in[8]);

/* These functions are used for OTS addresses. */

void set_ots_addr(uint32_t addr[8], uint32_t ots);

void set_chain_addr(uint32_t addr[8], uint32_t chain);

void set_hash_addr(uint32_t addr[8], uint32_t hash);

/* This function is used for L-tree addresses. */

void set_ltree_addr(uint32_t addr[8], uint32_t ltree);

/* These functions are used for hash tree addresses. */

void set_tree_height(uint32_t addr[8], uint32_t tree_height);

void set_tree_index(uint32_t addr[8], uint32_t tree_index);

#endif
