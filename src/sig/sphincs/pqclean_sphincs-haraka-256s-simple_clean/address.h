#ifndef SPX_ADDRESS_H
#define SPX_ADDRESS_H

#include <stdint.h>

#include "params.h"

/* The hash types that are passed to set_type */
#define SPX_ADDR_TYPE_WOTS 0
#define SPX_ADDR_TYPE_WOTSPK 1
#define SPX_ADDR_TYPE_HASHTREE 2
#define SPX_ADDR_TYPE_FORSTREE 3
#define SPX_ADDR_TYPE_FORSPK 4
#define SPX_ADDR_TYPE_WOTSPRF 5
#define SPX_ADDR_TYPE_FORSPRF 6

#define set_layer_addr SPX_NAMESPACE(set_layer_addr)
void set_layer_addr(uint32_t addr[8], uint32_t layer);

#define set_tree_addr SPX_NAMESPACE(set_tree_addr)
void set_tree_addr(uint32_t addr[8], uint64_t tree);

#define set_type SPX_NAMESPACE(set_type)
void set_type(uint32_t addr[8], uint32_t type);

/* Copies the layer and tree part of one address into the other */
#define copy_subtree_addr SPX_NAMESPACE(copy_subtree_addr)
void copy_subtree_addr(uint32_t out[8], const uint32_t in[8]);

/* These functions are used for WOTS and FORS addresses. */

#define set_keypair_addr SPX_NAMESPACE(set_keypair_addr)
void set_keypair_addr(uint32_t addr[8], uint32_t keypair);

#define set_chain_addr SPX_NAMESPACE(set_chain_addr)
void set_chain_addr(uint32_t addr[8], uint32_t chain);

#define set_hash_addr SPX_NAMESPACE(set_hash_addr)
void set_hash_addr(uint32_t addr[8], uint32_t hash);

#define copy_keypair_addr SPX_NAMESPACE(copy_keypair_addr)
void copy_keypair_addr(uint32_t out[8], const uint32_t in[8]);

/* These functions are used for all hash tree addresses (including FORS). */

#define set_tree_height SPX_NAMESPACE(set_tree_height)
void set_tree_height(uint32_t addr[8], uint32_t tree_height);

#define set_tree_index SPX_NAMESPACE(set_tree_index)
void set_tree_index(uint32_t addr[8], uint32_t tree_index);

#endif
