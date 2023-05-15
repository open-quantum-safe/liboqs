#ifndef XMSS_HASH_ADDRESS_H
#define XMSS_HASH_ADDRESS_H

#include <stdint.h>
#include "namespace.h"

#define XMSS_ADDR_TYPE_OTS 0
#define XMSS_ADDR_TYPE_LTREE 1
#define XMSS_ADDR_TYPE_HASHTREE 2

#define set_layer_addr XMSS_INNER_NAMESPACE(set_layer_addr)
void set_layer_addr(uint32_t addr[8], uint32_t layer);

#define set_tree_addr XMSS_INNER_NAMESPACE(set_tree_addr)
void set_tree_addr(uint32_t addr[8], uint64_t tree);

#define set_type XMSS_INNER_NAMESPACE(set_type)
void set_type(uint32_t addr[8], uint32_t type);

#define set_key_and_mask XMSS_INNER_NAMESPACE(set_key_and_mask)
void set_key_and_mask(uint32_t addr[8], uint32_t key_and_mask);

/* Copies the layer and tree part of one address into the other */
#define copy_subtree_addr XMSS_INNER_NAMESPACE(copy_subtree_addr)
void copy_subtree_addr(uint32_t out[8], const uint32_t in[8]);

/* These functions are used for OTS addresses. */
#define set_ots_addr XMSS_INNER_NAMESPACE(set_ots_addr)
void set_ots_addr(uint32_t addr[8], uint32_t ots);

#define set_chain_addr XMSS_INNER_NAMESPACE(set_chain_addr)
void set_chain_addr(uint32_t addr[8], uint32_t chain);

#define set_hash_addr XMSS_INNER_NAMESPACE(set_hash_addr)
void set_hash_addr(uint32_t addr[8], uint32_t hash);

/* This function is used for L-tree addresses. */
#define set_ltree_addr XMSS_INNER_NAMESPACE(set_ltree_addr)
void set_ltree_addr(uint32_t addr[8], uint32_t ltree);

/* These functions are used for hash tree addresses. */
#define set_tree_height XMSS_INNER_NAMESPACE(set_tree_height)
void set_tree_height(uint32_t addr[8], uint32_t tree_height);

#define set_tree_index XMSS_INNER_NAMESPACE(set_tree_index)
void set_tree_index(uint32_t addr[8], uint32_t tree_index);

#endif
