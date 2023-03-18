#include <string.h>

#include "utils.h"
#include "utilsx8.h"

#include "address.h"
#include "params.h"
#include "thashx8.h"

/*
 * Generate the entire Merkle tree, computing the authentication path for leaf_idx,
 * and the resulting root node using Merkle's TreeHash algorithm.
 * Expects the layer and tree parts of the tree_addr to be set, as well as the
 * tree type (i.e. SPX_ADDR_TYPE_HASHTREE or SPX_ADDR_TYPE_FORSTREE)
 *
 * This expects tree_addrx8 to be initialized to 8 parallel addr structures for
 * the Merkle tree nodes
 *
 * Applies the offset idx_offset to indices before building addresses, so that
 * it is possible to continue counting indices across trees.
 *
 * This works by using the standard Merkle tree building algorithm, except
 * that each 'node' tracked is actually 8 consecutive nodes in the real tree.
 * When we combine two logical nodes ABCDEFGH and STUVWXYZ, we perform the H
 * operation on adjacent real nodes, forming the parent logical node
 * (AB)(CD)(EF)(GH)(ST)(UV)(WX)(YZ)
 *
 * When we get to the top three levels of the real tree (where there is only
 * one logical node), we continue this operation three more times; the right
 * most real node will by the actual root (and the other 7 nodes will be
 * garbage).  We follow the same thashx8 logic so that the 'extract
 * authentication path components' part of the loop is still executed (and
 * to simplify the code somewhat)
 *
 * This currently assumes tree_height >= 3; I suspect that doing an adjusting
 * idx, addr_idx on the gen_leafx8 call if tree_height < 3 would fix it; since
 * we don't actually use such short trees, I haven't bothered
 */
void treehashx8(unsigned char *root, unsigned char *auth_path,
                const spx_ctx *ctx,
                uint32_t leaf_idx, uint32_t idx_offset,
                uint32_t tree_height,
                void (*gen_leafx8)(
                    unsigned char * /* Where to write the leaves */,
                    const spx_ctx *,
                    uint32_t idx, void *info),
                uint32_t tree_addrx8[8 * 8],
                void *info) {
    /* This is where we keep the intermediate nodes */
    PQCLEAN_VLA(unsigned char, stackx8, tree_height * 8 * SPX_N);
    uint32_t left_adj = 0, prev_left_adj = 0; /* When we're doing the top 3 */
    /* levels, the left-most part of the tree isn't at the beginning */
    /* of current[].  These give the offset of the actual start */

    uint32_t idx;
    uint32_t max_idx = ((uint32_t)1 << (tree_height - 3)) - 1;
    for (idx = 0;; idx++) {
        unsigned char current[8 * SPX_N]; /* Current logical node */
        gen_leafx8( current, ctx, 8 * idx + idx_offset,
                    info );

        /* Now combine the freshly generated right node with previously */
        /* generated left ones */
        uint32_t internal_idx_offset = idx_offset;
        uint32_t internal_idx = idx;
        uint32_t internal_leaf = leaf_idx;
        uint32_t h;     /* The height we are in the Merkle tree */
        for (h = 0;; h++, internal_idx >>= 1, internal_leaf >>= 1) {

            /* Special processing if we're at the top of the tree */
            if (h >= tree_height - 3) {
                if (h == tree_height) {
                    /* We hit the root; return it */
                    memcpy( root, &current[7 * SPX_N], SPX_N );
                    return;
                }
                /* The tree indexing logic is a bit off in this case */
                /* Adjust it so that the left-most node of the part of */
                /* the tree that we're processing has index 0 */
                prev_left_adj = left_adj;
                left_adj = (uint32_t)(8 - (1 << (tree_height - h - 1)));
            }

            /* Check if we hit the top of the tree */
            if (h == tree_height) {
                /* We hit the root; return it */
                memcpy( root, &current[7 * SPX_N], SPX_N );
                return;
            }

            /*
             * Check if one of the nodes we have is a part of the
             * authentication path; if it is, write it out
             */
            if ((((internal_idx << 3) ^ internal_leaf) & ~0x7U) == 0) {
                memcpy( &auth_path[ h * SPX_N ],
                        &current[(((internal_leaf & 7) ^ 1) + prev_left_adj) * SPX_N],
                        SPX_N );
            }

            /*
             * Check if we're at a left child; if so, stop going up the stack
             * Exception: if we've reached the end of the tree, keep on going
             * (so we combine the last 8 nodes into the one root node in three
             * more iterations)
             */
            if ((internal_idx & 1) == 0 && idx < max_idx) {
                break;
            }

            /* Ok, we're at a right node (or doing the top 3 levels) */
            /* Now combine the left and right logical nodes together */

            /* Set the address of the node we're creating. */
            uint32_t j;
            internal_idx_offset >>= 1;
            for (j = 0; j < 8; j++) {
                set_tree_height(tree_addrx8 + j * 8, h + 1);
                set_tree_index(tree_addrx8 + j * 8,
                               (8 / 2) * (internal_idx & ~1U) + j - left_adj + internal_idx_offset );
            }
            unsigned char *left = &stackx8[h * 8 * SPX_N];
            thashx8( &current[0 * SPX_N],
                     &current[1 * SPX_N],
                     &current[2 * SPX_N],
                     &current[3 * SPX_N],
                     &current[4 * SPX_N],
                     &current[5 * SPX_N],
                     &current[6 * SPX_N],
                     &current[7 * SPX_N],
                     &left   [0 * SPX_N],
                     &left   [2 * SPX_N],
                     &left   [4 * SPX_N],
                     &left   [6 * SPX_N],
                     &current[0 * SPX_N],
                     &current[2 * SPX_N],
                     &current[4 * SPX_N],
                     &current[6 * SPX_N],
                     2, ctx, tree_addrx8);
        }

        /* We've hit a left child; save the current for when we get the */
        /* corresponding right right */
        memcpy( &stackx8[h * 8 * SPX_N], current, 8 * SPX_N);
    }
}
