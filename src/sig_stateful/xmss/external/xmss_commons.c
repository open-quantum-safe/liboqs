#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <oqs/oqs.h>
#ifdef _MSC_VER
#include <malloc.h>
#endif

#include "hash.h"
#include "hash_address.h"
#include "params.h"
#include "wots.h"
#include "utils.h"
#include "xmss_commons.h"
#include "xmss_namespace.h"

/**
 * Computes a leaf node from a WOTS public key using an L-tree.
 * Note that this destroys the used WOTS public key.
 */
static void l_tree(const xmss_params *params,
                   uint8_t *leaf, uint8_t *wots_pk,
                   const uint8_t *pub_seed, uint32_t addr[8])
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
                           wots_pk + (i*2)*params->n, pub_seed, addr);
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
static void compute_root(const xmss_params *params, uint8_t *root,
                         const uint8_t *leaf, unsigned long leafidx,
                         const uint8_t *auth_path,
                         const uint8_t *pub_seed, uint32_t addr[8])
{
    uint32_t i;
    #ifdef _MSC_VER
    uint8_t *buffer = (uint8_t *)_malloca(2 * params->n);
    #else
    uint8_t buffer[2*params->n];
    #endif

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
            thash_h(params, buffer + params->n, buffer, pub_seed, addr);
            memcpy(buffer, auth_path, params->n);
        }
        else {
            thash_h(params, buffer, buffer, pub_seed, addr);
            memcpy(buffer + params->n, auth_path, params->n);
        }
        auth_path += params->n;
    }

    /* The last iteration is exceptional; we do not copy an auth_path node. */
    set_tree_height(addr, params->tree_height - 1);
    leafidx >>= 1;
    set_tree_index(addr, leafidx);
    thash_h(params, root, buffer, pub_seed, addr);

    #ifdef _MSC_VER
    _freea(buffer);
    #endif
}


/**
 * Computes the leaf at a given address. First generates the WOTS key pair,
 * then computes leaf using l_tree. As this happens position independent, we
 * only require that addr encodes the right ltree-address.
 */
void gen_leaf_wots(const xmss_params *params, uint8_t *leaf,
                   const uint8_t *sk_seed, const uint8_t *pub_seed,
                   uint32_t ltree_addr[8], uint32_t ots_addr[8])
{
    #ifdef _MSC_VER
    uint8_t *seed = (uint8_t *)_malloca(params->n);
    uint8_t *pk = (uint8_t *)_malloca(params->wots_sig_bytes);
    #else
    uint8_t seed[params->n];
    uint8_t pk[params->wots_sig_bytes];
    #endif
    #ifdef FORWARD_SECURE
    unsigned int i;

    // in a forward secure setting, the sk_seed already contains the current seed
    for(i =0; i < params->n; i++) {
        seed[i] = sk_seed[i];
    }
    #else
    get_seed(params, seed, sk_seed, ots_addr);
    #endif
    wots_pkgen(params, pk, seed, pub_seed, ots_addr);

    l_tree(params, leaf, pk, pub_seed, ltree_addr);
    
    #ifdef _MSC_VER
    _freea(seed);
    _freea(pk);
    #endif
}

/**
 * Used for pseudo-random key generation.
 * Generates the seed for the WOTS key pair at address 'addr'.
 *
 * Takes n-byte sk_seed and returns n-byte seed using 32 byte address 'addr'.
 */
void get_seed(const xmss_params *params, uint8_t *seed,
              const uint8_t *sk_seed, uint32_t addr[8])
{
    uint8_t bytes[32];

    /* Make sure that chain addr, hash addr, and key bit are zeroed. */
    set_chain_addr(addr, 0);
    set_hash_addr(addr, 0);
    set_key_and_mask(addr, 0);

    /* Generate seed. */
    addr_to_bytes(bytes, addr);
    prf(params, seed, bytes, sk_seed);
}

/**
 * Verifies a given message signature pair under a given public key.
 * Note that this assumes a pk without an OID, i.e. [root || PUB_SEED]
 */
int xmss_core_verify(const xmss_params *params,
                        uint8_t *m, unsigned long long *mlen,
                        const uint8_t *sm, unsigned long long smlen,
                        const uint8_t *pk)
{
    /* XMSS signatures are fundamentally an instance of XMSSMT signatures.
       For d=1, as is the case with XMSS, some of the calls in the XMSSMT
       routine become vacuous (i.e. the loop only iterates once, and address
       management can be simplified a bit).*/
    return xmssmt_core_verify(params, m, mlen, sm, smlen, pk);
}

/**
 * Verifies a given message signature pair under a given public key.
 * Note that this assumes a pk without an OID, i.e. [root || PUB_SEED]
 */
int xmssmt_core_verify(const xmss_params *params,
                          uint8_t *m, unsigned long long *mlen,
                          const uint8_t *sm, unsigned long long smlen,
                          const uint8_t *pk)
{
    const uint8_t *pub_root = pk;
    const uint8_t *pub_seed = pk + params->n;

    #ifdef _MSC_VER
    uint8_t wots_pk = (uint8_t *)_malloca(params->wots_sig_bytes);
    uint8_t root = (uint8_t *)_malloca(params->n);
    uint8_t leaf = (uint8_t *)_malloca(params->n);
    #else
    uint8_t wots_pk[params->wots_sig_bytes];
    uint8_t leaf[params->n];
    uint8_t root[params->n];
    #endif
    uint8_t *mhash = root;
    unsigned long long idx = 0;
    unsigned int i;
    uint32_t idx_leaf;

    uint32_t ots_addr[8] = {0};
    uint32_t ltree_addr[8] = {0};
    uint32_t node_addr[8] = {0};

    set_type(ots_addr, XMSS_ADDR_TYPE_OTS);
    set_type(ltree_addr, XMSS_ADDR_TYPE_LTREE);
    set_type(node_addr, XMSS_ADDR_TYPE_HASHTREE);

    /* Convert the index bytes from the signature to an integer. */
    idx = bytes_to_ull(sm, params->index_bytes);

    uint8_t *buffer = (uint8_t*)malloc((*mlen + 4 * params->n) * sizeof(uint8_t));
    
    memcpy(buffer + 4* params->n, m, *mlen);

    /* Compute the message hash. */
    hash_message(params, mhash, sm + params->index_bytes, pk, idx,
                buffer, *mlen);
        
    OQS_MEM_insecure_free(buffer);
    buffer = NULL;

    sm += params->index_bytes + params->n;
    smlen -= (params->index_bytes + params->n);
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
        smlen -= params->wots_sig_bytes;

        /* Compute the leaf node using the WOTS public key. */
        set_ltree_addr(ltree_addr, idx_leaf);
        l_tree(params, leaf, wots_pk, pub_seed, ltree_addr);

        /* Compute the root node of this subtree. */
        compute_root(params, root, leaf, idx_leaf, sm, pub_seed, node_addr);
        sm += params->tree_height*params->n;
        smlen -= params->tree_height*params->n;
    }

    if (smlen != 0) return -1; /* Signature malformed. */

    /* Check if the root node equals the root node in the public key. */
    if (memcmp(root, pub_root, params->n)) {
        /* If not, zero the message */
        memset(m, 0, *mlen);
        *mlen = 0;
        return -1;
    }
    return 0;
}
