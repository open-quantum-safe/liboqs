#ifndef PQCLEAN_SPHINCSSHA256256FSIMPLE_AVX2_UTILSX8_H
#define PQCLEAN_SPHINCSSHA256256FSIMPLE_AVX2_UTILSX8_H

#include <stdint.h>

#include "hash_state.h"
#include "params.h"

/**
 * For a given leaf index, computes the authentication path and the resulting
 * root node using Merkle's TreeHash algorithm.
 * Expects the layer and tree parts of the tree_addr to be set, as well as the
 * tree type (i.e. PQCLEAN_SPHINCSSHA256256FSIMPLE_AVX2_ADDR_TYPE_HASHTREE or PQCLEAN_SPHINCSSHA256256FSIMPLE_AVX2_ADDR_TYPE_FORSTREE).
 * Applies the offset idx_offset to indices before building addresses, so that
 * it is possible to continue counting indices across trees.
 */

void PQCLEAN_SPHINCSSHA256256FSIMPLE_AVX2_treehashx8_FORS_HEIGHT(
    unsigned char *rootx8, unsigned char *auth_pathx8,
    const unsigned char *sk_seed, const unsigned char *pub_seed,
    const uint32_t leaf_idx[8], uint32_t idx_offset[8],
    void (*gen_leafx8)(
        unsigned char * /* leaf0 */,
        unsigned char * /* leaf1 */,
        unsigned char * /* leaf2 */,
        unsigned char * /* leaf3 */,
        unsigned char * /* leaf4 */,
        unsigned char * /* leaf5 */,
        unsigned char * /* leaf6 */,
        unsigned char * /* leaf7 */,
        const unsigned char * /* sk_seed */,
        const unsigned char * /* pub_seed */,
        uint32_t /* addr_idx0 */,
        uint32_t /* addr_idx1 */,
        uint32_t /* addr_idx2 */,
        uint32_t /* addr_idx3 */,
        uint32_t /* addr_idx4 */,
        uint32_t /* addr_idx5 */,
        uint32_t /* addr_idx6 */,
        uint32_t /* addr_idx7 */,
        const uint32_t[8] /* tree_addr */,
        const hash_state * /* state_seeded */),
    uint32_t tree_addrx8[8 * 8],
    const hash_state *state_seeded);

#endif
