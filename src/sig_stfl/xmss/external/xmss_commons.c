// SPDX-License-Identifier: (Apache-2.0 OR MIT) AND CC0-1.0
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "hash.h"
#include "hash_address.h"
#include "params.h"
#include "wots.h"
#include "utils.h"
#include "xmss_commons.h"

/**
 * Computes a leaf node from a WOTS public key using an L-tree.
 * Note that this destroys the used WOTS public key.
 */
static void l_tree(const xmss_params *params,
                   unsigned char *leaf, unsigned char *wots_pk,
                   const unsigned char *pub_seed, uint32_t addr[8], 
                   unsigned char *thash_buf)
{
    unsigned int l = params->wots_len;
    unsigned int parent_nodes;
    uint32_t i;
    uint32_t height = 0;

    set_tree_height(addr, height);

    while (l > 1) {
        parent_nodes = l >> 1;
        for (i = 0; i < parent_nodes; i++) {
            set_tree_index(addr, i);
            /* Hashes the nodes at (i*2)*params->n and (i*2)*params->n + 1 */
            thash_h(params, wots_pk + i*params->n,
                           wots_pk + (i*2)*params->n, pub_seed, addr, thash_buf);
        }
        /* If the row contained an odd number of nodes, the last node was not
           hashed. Instead, we pull it up to the next layer. */
        if (l & 1) {
            memcpy(wots_pk + (l >> 1)*params->n,
                   wots_pk + (l - 1)*params->n, params->n);
            l = (l >> 1) + 1;
        }
        else {
            l = l >> 1;
        }
        height++;
        set_tree_height(addr, height);
    }
    memcpy(leaf, wots_pk, params->n);
}

/**
 * Computes a root node given a leaf and an auth path
 */
static void compute_root(const xmss_params *params, unsigned char *root,
                         const unsigned char *leaf, unsigned long leafidx,
                         const unsigned char *auth_path,
                         const unsigned char *pub_seed, uint32_t addr[8],
                         unsigned char *buffer,
                         unsigned char *thash_buf)
{
    uint32_t i;    

    /* If leafidx is odd (last bit = 1), current path element is a right child
       and auth_path has to go left. Otherwise it is the other way around. */
    if (leafidx & 1) {
        memcpy(buffer + params->n, leaf, params->n);
        memcpy(buffer, auth_path, params->n);
    }
    else {
        memcpy(buffer, leaf, params->n);
        memcpy(buffer + params->n, auth_path, params->n);
    }
    auth_path += params->n;

    for (i = 0; i < params->tree_height - 1; i++) {
        set_tree_height(addr, i);
        leafidx >>= 1;
        set_tree_index(addr, leafidx);

        /* Pick the right or left neighbor, depending on parity of the node. */
        if (leafidx & 1) {
            thash_h(params, buffer + params->n, buffer, pub_seed, addr, thash_buf);
            memcpy(buffer, auth_path, params->n);
        }
        else {
            thash_h(params, buffer, buffer, pub_seed, addr, thash_buf);
            memcpy(buffer + params->n, auth_path, params->n);
        }
        auth_path += params->n;
    }

    /* The last iteration is exceptional; we do not copy an auth_path node. */
    set_tree_height(addr, params->tree_height - 1);
    leafidx >>= 1;
    set_tree_index(addr, leafidx);
    thash_h(params, root, buffer, pub_seed, addr, thash_buf);

}


/**
 * Computes the leaf at a given address. First generates the WOTS key pair,
 * then computes leaf using l_tree. As this happens position independent, we
 * only require that addr encodes the right ltree-address.
 */
void gen_leaf_wots(const xmss_params *params, unsigned char *leaf,
                   const unsigned char *sk_seed, const unsigned char *pub_seed,
                   uint32_t ltree_addr[8], uint32_t ots_addr[8])
{
    unsigned char *pk = OQS_MEM_malloc(params->wots_sig_bytes + 2 * params->padding_len + 6 * params->n + 32);
    if (pk == NULL) {
        return;
    }
    unsigned char *thash_buf = pk + params->wots_sig_bytes;

    wots_pkgen(params, pk, sk_seed, pub_seed, ots_addr);

    l_tree(params, leaf, pk, pub_seed, ltree_addr, thash_buf);

    OQS_MEM_insecure_free(pk);
}


/**
 * Verifies a given message signature pair under a given public key.
 * Note that this assumes a pk without an OID, i.e. [root || PUB_SEED]
 */
int xmss_core_sign_open(const xmss_params *params,
                        const unsigned char *m, unsigned long long mlen,
                        const unsigned char *sm, unsigned long long smlen,
                        const unsigned char *pk)
{
    /* XMSS signatures are fundamentally an instance of XMSSMT signatures.
       For d=1, as is the case with XMSS, some of the calls in the XMSSMT
       routine become vacuous (i.e. the loop only iterates once, and address
       management can be simplified a bit).*/
    return xmssmt_core_sign_open(params, m, mlen, sm, smlen, pk);
}

/**
 * Verifies a given message signature pair under a given public key.
 * Note that this assumes a pk without an OID, i.e. [root || PUB_SEED]
 */
int xmssmt_core_sign_open(const xmss_params *params,
                          const unsigned char *m, unsigned long long mlen,
                          const unsigned char *sm, unsigned long long smlen,
                          const unsigned char *pk)
{
    const unsigned char *pub_root = pk;
    const unsigned char *pub_seed = pk + params->n;

    unsigned char *tmp = OQS_MEM_malloc(params->wots_sig_bytes + params->n + params->n +
                                + 2 *params->n + 2 * params->padding_len + 6 * params->n + 32);
    if (tmp == NULL) {
        return -1;
    }
    unsigned char *wots_pk = tmp;
    unsigned char *leaf = tmp + params->wots_sig_bytes;
    unsigned char *root = leaf + params->n;
    unsigned char *compute_root_buf = root + params->n;
    unsigned char *thash_buf = compute_root_buf + 2*params->n;

    unsigned long long prefix_length = params->padding_len + 3*params->n;
    unsigned long long m_with_prefix_len = mlen + prefix_length;
    unsigned char *m_with_prefix = NULL;
    unsigned char *mhash = root;
    unsigned long long idx = 0;
    unsigned int i, ret;
    uint32_t idx_leaf;

    uint32_t ots_addr[8] = {0};
    uint32_t ltree_addr[8] = {0};
    uint32_t node_addr[8] = {0};

    set_type(ots_addr, XMSS_ADDR_TYPE_OTS);
    set_type(ltree_addr, XMSS_ADDR_TYPE_LTREE);
    set_type(node_addr, XMSS_ADDR_TYPE_HASHTREE);

    // Unused since smlen is a constant
    (void) smlen;

    if ((m_with_prefix_len == 0) || (m_with_prefix = OQS_MEM_malloc(m_with_prefix_len)) == NULL){
        ret = -1;
        goto fail;
    }

    /* Convert the index bytes from the signature to an integer. */
    idx = bytes_to_ull(sm, params->index_bytes);

    /* Put the message at the m_with_prefix buffer, so that we can
     * prepend the required other inputs for the hash function. */
    memcpy(m_with_prefix, sm + params->sig_bytes - prefix_length, (size_t)prefix_length);
    memcpy(m_with_prefix + prefix_length, m, (size_t)mlen);

    /* Compute the message hash. */
    hash_message(params, mhash, sm + params->index_bytes, pk, idx,
                m_with_prefix,
                 mlen);
    sm += params->index_bytes + params->n;

    /* For each subtree.. */
    for (i = 0; i < params->d; i++) {
        idx_leaf = (idx & ((1 << params->tree_height)-1));
        idx = idx >> params->tree_height;

        set_layer_addr(ots_addr, i);
        set_layer_addr(ltree_addr, i);
        set_layer_addr(node_addr, i);

        set_tree_addr(ltree_addr, idx);
        set_tree_addr(ots_addr, idx);
        set_tree_addr(node_addr, idx);

        /* The WOTS public key is only correct if the signature was correct. */
        set_ots_addr(ots_addr, idx_leaf);
        /* Initially, root = mhash, but on subsequent iterations it is the root
           of the subtree below the currently processed subtree. */
        wots_pk_from_sig(params, wots_pk, sm, root, pub_seed, ots_addr);
        sm += params->wots_sig_bytes;

        /* Compute the leaf node using the WOTS public key. */
        set_ltree_addr(ltree_addr, idx_leaf);
        l_tree(params, leaf, wots_pk, pub_seed, ltree_addr, thash_buf);

        /* Compute the root node of this subtree. */
        compute_root(params, root, leaf, idx_leaf, sm, pub_seed, node_addr, compute_root_buf, thash_buf);
        sm += params->tree_height*params->n;
    }

    /* Check if the root node equals the root node in the public key. */
    if (memcmp(root, pub_root, params->n)) {
        /* If not, return fail */
        ret = -1;
        goto fail;
    }
    ret = 0;
fail:
    OQS_MEM_insecure_free(tmp);
    OQS_MEM_insecure_free(m_with_prefix);
    return ret;

}
