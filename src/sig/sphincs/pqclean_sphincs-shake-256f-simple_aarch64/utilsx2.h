#ifndef SPX_UTILSX2_H
#define SPX_UTILSX2_H

#include <stdint.h>

#include "context.h"
#include "params.h"

/**
 * For a given leaf index, computes the authentication path and the resulting
 * root node using Merkle's TreeHash algorithm.
 * Expects the layer and tree parts of the tree_addr to be set, as well as the
 * tree type (i.e. SPX_ADDR_TYPE_HASHTREE or SPX_ADDR_TYPE_FORSTREE).
 * Applies the offset idx_offset to indices before building addresses, so that
 * it is possible to continue counting indices across trees.
 *
 * This implementation uses SIMD to compute internal nodes 2 at a time (in
 * parallel)
 */
#define treehashx2 SPX_NAMESPACE(treehashx2)
void treehashx2(unsigned char *root, unsigned char *auth_path,
                const spx_ctx *ctx,
                uint32_t leaf_idx, uint32_t idx_offset, uint32_t tree_height,
                void (*gen_leafx2)(
                    unsigned char * /* Where to write the leaves */,
                    const spx_ctx * /* ctx */,
                    uint32_t addr_idx, void *info),
                uint32_t tree_addrx2[2 * 8], void *info);

#endif
