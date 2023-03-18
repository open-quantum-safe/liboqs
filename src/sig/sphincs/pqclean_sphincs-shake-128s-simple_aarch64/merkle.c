#include <stdint.h>
#include <string.h>

#include "merkle.h"

#include "address.h"
#include "params.h"
#include "utils.h"
#include "utilsx2.h"
#include "wots.h"
#include "wotsx2.h"

/*
 * This generates a Merkle signature (WOTS signature followed by the Merkle
 * authentication path).
 */
void merkle_sign(uint8_t *sig, unsigned char *root,
                 const spx_ctx *ctx,
                 uint32_t wots_addr[8], uint32_t tree_addr[8],
                 uint32_t idx_leaf) {
    unsigned char *auth_path = sig + SPX_WOTS_BYTES;
    uint32_t tree_addrx2[2 * 8] = { 0 };
    int j;
    struct leaf_info_x2 info = { 0 };
    unsigned steps[ SPX_WOTS_LEN ];

    info.wots_sig = sig;
    chain_lengths(steps, root);
    info.wots_steps = steps;

    for (j = 0; j < 2; j++) {
        set_type(&tree_addrx2[8 * j], SPX_ADDR_TYPE_HASHTREE);
        set_type(&info.leaf_addr[8 * j], SPX_ADDR_TYPE_WOTS);
        set_type(&info.pk_addr[8 * j], SPX_ADDR_TYPE_WOTSPK);
        copy_subtree_addr(&tree_addrx2[8 * j], tree_addr);
        copy_subtree_addr(&info.leaf_addr[8 * j], wots_addr);
        copy_subtree_addr(&info.pk_addr[8 * j], wots_addr);
    }

    info.wots_sign_leaf = idx_leaf;

    treehashx2(root, auth_path, ctx,
               idx_leaf, 0,
               SPX_TREE_HEIGHT,
               wots_gen_leafx2,
               tree_addrx2, &info);
}

/* Compute root node of the top-most subtree. */
void merkle_gen_root(unsigned char *root, const spx_ctx *ctx) {
    /* We do not need the auth path in key generation, but it simplifies the
       code to have just one treehash routine that computes both root and path
       in one function. */
    unsigned char auth_path[SPX_TREE_HEIGHT * SPX_N + SPX_WOTS_BYTES];
    uint32_t top_tree_addr[8] = {0};
    uint32_t wots_addr[8] = {0};

    set_layer_addr(top_tree_addr, SPX_D - 1);
    set_layer_addr(wots_addr, SPX_D - 1);

    merkle_sign(auth_path, root, ctx,
                wots_addr, top_tree_addr,
                ~0 /* ~0 means "don't bother generating an auth path */ );
}
