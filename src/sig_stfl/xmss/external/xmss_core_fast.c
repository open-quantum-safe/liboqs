#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <oqs/rand.h>

#include "hash.h"
#include "hash_address.h"
#include "params.h"
#include "wots.h"
#include "utils.h"
#include "xmss_commons.h"
#include "xmss_core.h"

typedef struct{
    unsigned char h;
    unsigned long next_idx;
    unsigned char stackusage;
    unsigned char completed;
    unsigned char *node;
} treehash_inst;

typedef struct {
    unsigned char *stack;
    unsigned int stackoffset;
    unsigned char *stacklevels;
    unsigned char *auth;
    unsigned char *keep;
    treehash_inst *treehash;
    unsigned char *retain;
    unsigned int next_leaf;
} bds_state;

/* These serialization functions provide a transition between the current
   way of storing the state in an exposed struct, and storing it as part of the
   byte array that is the secret key.
   They will probably be refactored in a non-backwards-compatible way, soon. */

static void xmssmt_serialize_state(const xmss_params *params,
                                   unsigned char *sk, bds_state *states)
{
    unsigned int i, j;

    /* Skip past the 'regular' sk */
    sk += params->index_bytes + 4*params->n;

    for (i = 0; i < 2*params->d - 1; i++) {
        sk += (params->tree_height + 1) * params->n; /* stack */

        ull_to_bytes(sk, 4, states[i].stackoffset);
        sk += 4;

        sk += params->tree_height + 1; /* stacklevels */
        sk += params->tree_height * params->n; /* auth */
        sk += (params->tree_height >> 1) * params->n; /* keep */

        for (j = 0; j < params->tree_height - params->bds_k; j++) {
            ull_to_bytes(sk, 1, states[i].treehash[j].h);
            sk += 1;

            ull_to_bytes(sk, 4, states[i].treehash[j].next_idx);
            sk += 4;

            ull_to_bytes(sk, 1, states[i].treehash[j].stackusage);
            sk += 1;

            ull_to_bytes(sk, 1, states[i].treehash[j].completed);
            sk += 1;

            sk += params->n; /* node */
        }

        /* retain */
        sk += ((1 << params->bds_k) - params->bds_k - 1) * params->n;

        ull_to_bytes(sk, 4, states[i].next_leaf);
        sk += 4;
    }
}

static void xmssmt_deserialize_state(const xmss_params *params,
                                     bds_state *states,
                                     unsigned char **wots_sigs,
                                     unsigned char *sk)
{
    unsigned int i, j;

    /* Skip past the 'regular' sk */
    sk += params->index_bytes + 4*params->n;

    // TODO These data sizes follow from the (former) test xmss_core_fast.c
    // TODO They should be reconsidered / motivated more explicitly

    for (i = 0; i < 2*params->d - 1; i++) {
        states[i].stack = sk;
        sk += (params->tree_height + 1) * params->n;

        states[i].stackoffset = bytes_to_ull(sk, 4);
        sk += 4;

        states[i].stacklevels = sk;
        sk += params->tree_height + 1;

        states[i].auth = sk;
        sk += params->tree_height * params->n;

        states[i].keep = sk;
        sk += (params->tree_height >> 1) * params->n;

        for (j = 0; j < params->tree_height - params->bds_k; j++) {
            states[i].treehash[j].h = bytes_to_ull(sk, 1);
            sk += 1;

            states[i].treehash[j].next_idx = bytes_to_ull(sk, 4);
            sk += 4;

            states[i].treehash[j].stackusage = bytes_to_ull(sk, 1);
            sk += 1;

            states[i].treehash[j].completed = bytes_to_ull(sk, 1);
            sk += 1;

            states[i].treehash[j].node = sk;
            sk += params->n;
        }

        states[i].retain = sk;
        sk += ((1 << params->bds_k) - params->bds_k - 1) * params->n;

        states[i].next_leaf = bytes_to_ull(sk, 4);
        sk += 4;
    }

    if (params->d > 1) {
        *wots_sigs = sk;
    }
}

static void xmss_serialize_state(const xmss_params *params,
                                 unsigned char *sk, bds_state *state)
{
    xmssmt_serialize_state(params, sk, state);
}

static void xmss_deserialize_state(const xmss_params *params,
                                   bds_state *state, unsigned char *sk)
{
    xmssmt_deserialize_state(params, state, NULL, sk);
}

static void memswap(void *a, void *b, void *t, unsigned long long len)
{
    memcpy(t, a, len);
    memcpy(a, b, len);
    memcpy(b, t, len);
}

/**
 * Swaps the content of two bds_state objects, swapping actual memory rather
 * than pointers.
 * As we're mapping memory chunks in the secret key to bds state objects,
 * it is now necessary to make swaps 'real swaps'. This could be done in the
 * serialization function as well, but that causes more overhead
 */
// TODO this should not be necessary if we keep better track of the states
static void deep_state_swap(const xmss_params *params,
                            bds_state *a, bds_state *b)
{
    if (a->stack == NULL || b->stack == NULL) {
        return;
    }
    // TODO this is extremely ugly and should be refactored
    // TODO right now, this ensures that both 'stack' and 'retain' fit
    unsigned char t[
        ((params->tree_height + 1) > ((1 << params->bds_k) - params->bds_k - 1)
         ? (params->tree_height + 1)
         : ((1 << params->bds_k) - params->bds_k - 1))
        * params->n];
    unsigned int i;

    memswap(a->stack, b->stack, t, (params->tree_height + 1) * params->n);
    memswap(&a->stackoffset, &b->stackoffset, t, sizeof(a->stackoffset));
    memswap(a->stacklevels, b->stacklevels, t, params->tree_height + 1);
    memswap(a->auth, b->auth, t, params->tree_height * params->n);
    memswap(a->keep, b->keep, t, (params->tree_height >> 1) * params->n);

    for (i = 0; i < params->tree_height - params->bds_k; i++) {
        memswap(&a->treehash[i].h, &b->treehash[i].h, t, sizeof(a->treehash[i].h));
        memswap(&a->treehash[i].next_idx, &b->treehash[i].next_idx, t, sizeof(a->treehash[i].next_idx));
        memswap(&a->treehash[i].stackusage, &b->treehash[i].stackusage, t, sizeof(a->treehash[i].stackusage));
        memswap(&a->treehash[i].completed, &b->treehash[i].completed, t, sizeof(a->treehash[i].completed));
        memswap(a->treehash[i].node, b->treehash[i].node, t, params->n);
    }

    memswap(a->retain, b->retain, t, ((1 << params->bds_k) - params->bds_k - 1) * params->n);
    memswap(&a->next_leaf, &b->next_leaf, t, sizeof(a->next_leaf));
}

static int treehash_minheight_on_stack(const xmss_params *params,
                                       bds_state *state,
                                       const treehash_inst *treehash)
{
    unsigned int r = params->tree_height, i;

    for (i = 0; i < treehash->stackusage; i++) {
        if (state->stacklevels[state->stackoffset - i - 1] < r) {
            r = state->stacklevels[state->stackoffset - i - 1];
        }
    }
    return r;
}

/**
 * Merkle's TreeHash algorithm. The address only needs to initialize the first 78 bits of addr. Everything else will be set by treehash.
 * Currently only used for key generation.
 *
 */
static void treehash_init(const xmss_params *params,
                          unsigned char *node, int height, int index,
                          bds_state *state, const unsigned char *sk_seed,
                          const unsigned char *pub_seed, const uint32_t addr[8])
{
    unsigned int idx = index;
    // use three different addresses because at this point we use all three formats in parallel
    uint32_t ots_addr[8] = {0};
    uint32_t ltree_addr[8] = {0};
    uint32_t node_addr[8] = {0};
    // only copy layer and tree address parts
    copy_subtree_addr(ots_addr, addr);
    // type = ots
    set_type(ots_addr, 0);
    copy_subtree_addr(ltree_addr, addr);
    set_type(ltree_addr, 1);
    copy_subtree_addr(node_addr, addr);
    set_type(node_addr, 2);

    uint32_t lastnode, i;
    unsigned char *stack = calloc((height+1)*params->n, sizeof(unsigned char));
    unsigned int stacklevels[height+1];
    unsigned int stackoffset=0;
    unsigned int nodeh;

    lastnode = idx+(1<<height);

    for (i = 0; i < params->tree_height-params->bds_k; i++) {
        state->treehash[i].h = i;
        state->treehash[i].completed = 1;
        state->treehash[i].stackusage = 0;
    }

    i = 0;
    for (; idx < lastnode; idx++) {
        set_ltree_addr(ltree_addr, idx);
        set_ots_addr(ots_addr, idx);
        gen_leaf_wots(params, stack+stackoffset*params->n, sk_seed, pub_seed, ltree_addr, ots_addr);
        stacklevels[stackoffset] = 0;
        stackoffset++;
        if (params->tree_height - params->bds_k > 0 && i == 3) {
            memcpy(state->treehash[0].node, stack+stackoffset*params->n, params->n);
        }
        while (stackoffset>1 && stacklevels[stackoffset-1] == stacklevels[stackoffset-2]) {
            nodeh = stacklevels[stackoffset-1];
            if (i >> nodeh == 1) {
                memcpy(state->auth + nodeh*params->n, stack+(stackoffset-1)*params->n, params->n);
            }
            else {
                if (nodeh < params->tree_height - params->bds_k && i >> nodeh == 3) {
                    memcpy(state->treehash[nodeh].node, stack+(stackoffset-1)*params->n, params->n);
                }
                else if (nodeh >= params->tree_height - params->bds_k) {
                    memcpy(state->retain + ((1 << (params->tree_height - 1 - nodeh)) + nodeh - params->tree_height + (((i >> nodeh) - 3) >> 1)) * params->n, stack+(stackoffset-1)*params->n, params->n);
                }
            }
            set_tree_height(node_addr, stacklevels[stackoffset-1]);
            set_tree_index(node_addr, (idx >> (stacklevels[stackoffset-1]+1)));
            thash_h(params, stack+(stackoffset-2)*params->n, stack+(stackoffset-2)*params->n, pub_seed, node_addr);
            stacklevels[stackoffset-2]++;
            stackoffset--;
        }
        i++;
    }

    for (i = 0; i < params->n; i++) {
        node[i] = stack[i];
    }

    OQS_MEM_insecure_free(stack);
}

static void treehash_update(const xmss_params *params,
                            treehash_inst *treehash, bds_state *state,
                            const unsigned char *sk_seed,
                            const unsigned char *pub_seed,
                            const uint32_t addr[8])
{
    uint32_t ots_addr[8] = {0};
    uint32_t ltree_addr[8] = {0};
    uint32_t node_addr[8] = {0};
    // only copy layer and tree address parts
    copy_subtree_addr(ots_addr, addr);
    // type = ots
    set_type(ots_addr, 0);
    copy_subtree_addr(ltree_addr, addr);
    set_type(ltree_addr, 1);
    copy_subtree_addr(node_addr, addr);
    set_type(node_addr, 2);

    set_ltree_addr(ltree_addr, treehash->next_idx);
    set_ots_addr(ots_addr, treehash->next_idx);

    unsigned char nodebuffer[2 * params->n];
    unsigned int nodeheight = 0;
    gen_leaf_wots(params, nodebuffer, sk_seed, pub_seed, ltree_addr, ots_addr);
    while (treehash->stackusage > 0 && state->stacklevels[state->stackoffset-1] == nodeheight) {
        memcpy(nodebuffer + params->n, nodebuffer, params->n);
        memcpy(nodebuffer, state->stack + (state->stackoffset-1)*params->n, params->n);
        set_tree_height(node_addr, nodeheight);
        set_tree_index(node_addr, (treehash->next_idx >> (nodeheight+1)));
        thash_h(params, nodebuffer, nodebuffer, pub_seed, node_addr);
        nodeheight++;
        treehash->stackusage--;
        state->stackoffset--;
    }
    if (nodeheight == treehash->h) { // this also implies stackusage == 0
        memcpy(treehash->node, nodebuffer, params->n);
        treehash->completed = 1;
    }
    else {
        memcpy(state->stack + state->stackoffset*params->n, nodebuffer, params->n);
        treehash->stackusage++;
        state->stacklevels[state->stackoffset] = nodeheight;
        state->stackoffset++;
        treehash->next_idx++;
    }
}

/**
 * Performs treehash updates on the instance that needs it the most.
 * Returns the updated number of available updates.
 **/
static char bds_treehash_update(const xmss_params *params,
                                bds_state *state, unsigned int updates,
                                const unsigned char *sk_seed,
                                unsigned char *pub_seed,
                                const uint32_t addr[8])
{
    uint32_t i, j;
    unsigned int level, l_min, low;
    unsigned int used = 0;

    for (j = 0; j < updates; j++) {
        l_min = params->tree_height;
        level = params->tree_height - params->bds_k;
        for (i = 0; i < params->tree_height - params->bds_k; i++) {
            if (state->treehash[i].completed) {
                low = params->tree_height;
            }
            else if (state->treehash[i].stackusage == 0) {
                low = i;
            }
            else {
                low = treehash_minheight_on_stack(params, state, &(state->treehash[i]));
            }
            if (low < l_min) {
                level = i;
                l_min = low;
            }
        }
        if (level == params->tree_height - params->bds_k) {
            break;
        }
        treehash_update(params, &(state->treehash[level]), state, sk_seed, pub_seed, addr);
        used++;
    }
    return updates - used;
}

/**
 * Updates the state (typically NEXT_i) by adding a leaf and updating the stack
 * Returns -1 if all leaf nodes have already been processed
 **/
static char bds_state_update(const xmss_params *params,
                             bds_state *state, const unsigned char *sk_seed,
                             const unsigned char *pub_seed,
                             const uint32_t addr[8])
{
    if (state == NULL || state->stacklevels == NULL) {
        return -1;
    }

    uint32_t ltree_addr[8] = {0};
    uint32_t node_addr[8] = {0};
    uint32_t ots_addr[8] = {0};

    unsigned int nodeh;
    int idx = state->next_leaf;
    if (idx == 1 << params->tree_height) {
        return -1;
    }

    // only copy layer and tree address parts
    copy_subtree_addr(ots_addr, addr);
    // type = ots
    set_type(ots_addr, 0);
    copy_subtree_addr(ltree_addr, addr);
    set_type(ltree_addr, 1);
    copy_subtree_addr(node_addr, addr);
    set_type(node_addr, 2);

    set_ots_addr(ots_addr, idx);
    set_ltree_addr(ltree_addr, idx);

    gen_leaf_wots(params, state->stack+state->stackoffset*params->n, sk_seed, pub_seed, ltree_addr, ots_addr);

    state->stacklevels[state->stackoffset] = 0;
    state->stackoffset++;
    if (params->tree_height - params->bds_k > 0 && idx == 3) {
        memcpy(state->treehash[0].node, state->stack+state->stackoffset*params->n, params->n);
    }
    while (state->stackoffset>1 && state->stacklevels[state->stackoffset-1] == state->stacklevels[state->stackoffset-2]) {
        nodeh = state->stacklevels[state->stackoffset-1];
        if (idx >> nodeh == 1) {
            memcpy(state->auth + nodeh*params->n, state->stack+(state->stackoffset-1)*params->n, params->n);
        }
        else {
            if (nodeh < params->tree_height - params->bds_k && idx >> nodeh == 3) {
                memcpy(state->treehash[nodeh].node, state->stack+(state->stackoffset-1)*params->n, params->n);
            }
            else if (nodeh >= params->tree_height - params->bds_k) {
                memcpy(state->retain + ((1 << (params->tree_height - 1 - nodeh)) + nodeh - params->tree_height + (((idx >> nodeh) - 3) >> 1)) * params->n, state->stack+(state->stackoffset-1)*params->n, params->n);
            }
        }
        set_tree_height(node_addr, state->stacklevels[state->stackoffset-1]);
        set_tree_index(node_addr, (idx >> (state->stacklevels[state->stackoffset-1]+1)));
        thash_h(params, state->stack+(state->stackoffset-2)*params->n, state->stack+(state->stackoffset-2)*params->n, pub_seed, node_addr);

        state->stacklevels[state->stackoffset-2]++;
        state->stackoffset--;
    }
    state->next_leaf++;
    return 0;
}

/**
 * Returns the auth path for node leaf_idx and computes the auth path for the
 * next leaf node, using the algorithm described by Buchmann, Dahmen and Szydlo
 * in "Post Quantum Cryptography", Springer 2009.
 */
static void bds_round(const xmss_params *params,
                      bds_state *state, const unsigned long leaf_idx,
                      const unsigned char *sk_seed,
                      const unsigned char *pub_seed, uint32_t addr[8])
{
    unsigned int i;
    unsigned int tau = params->tree_height;
    unsigned int startidx;
    unsigned int offset, rowidx;
    unsigned char buf[2 * params->n];

    uint32_t ots_addr[8] = {0};
    uint32_t ltree_addr[8] = {0};
    uint32_t node_addr[8] = {0};

    // only copy layer and tree address parts
    copy_subtree_addr(ots_addr, addr);
    // type = ots
    set_type(ots_addr, 0);
    copy_subtree_addr(ltree_addr, addr);
    set_type(ltree_addr, 1);
    copy_subtree_addr(node_addr, addr);
    set_type(node_addr, 2);

    for (i = 0; i < params->tree_height; i++) {
        if (! ((leaf_idx >> i) & 1)) {
            tau = i;
            break;
        }
    }

    if (tau > 0) {
        memcpy(buf, state->auth + (tau-1) * params->n, params->n);
        // we need to do this before refreshing state->keep to prevent overwriting
        memcpy(buf + params->n, state->keep + ((tau-1) >> 1) * params->n, params->n);
    }
    if (!((leaf_idx >> (tau + 1)) & 1) && (tau < params->tree_height - 1)) {
        memcpy(state->keep + (tau >> 1)*params->n, state->auth + tau*params->n, params->n);
    }
    if (tau == 0) {
        set_ltree_addr(ltree_addr, leaf_idx);
        set_ots_addr(ots_addr, leaf_idx);
        gen_leaf_wots(params, state->auth, sk_seed, pub_seed, ltree_addr, ots_addr);
    }
    else {
        set_tree_height(node_addr, (tau-1));
        set_tree_index(node_addr, leaf_idx >> tau);
        thash_h(params, state->auth + tau * params->n, buf, pub_seed, node_addr);
        for (i = 0; i < tau; i++) {
            if (i < params->tree_height - params->bds_k) {
                memcpy(state->auth + i * params->n, state->treehash[i].node, params->n);
            }
            else {
                offset = (1 << (params->tree_height - 1 - i)) + i - params->tree_height;
                rowidx = ((leaf_idx >> i) - 1) >> 1;
                memcpy(state->auth + i * params->n, state->retain + (offset + rowidx) * params->n, params->n);
            }
        }

        for (i = 0; i < ((tau < params->tree_height - params->bds_k) ? tau : (params->tree_height - params->bds_k)); i++) {
            startidx = leaf_idx + 1 + 3 * (1 << i);
            if (startidx < 1U << params->tree_height) {
                state->treehash[i].h = i;
                state->treehash[i].next_idx = startidx;
                state->treehash[i].completed = 0;
                state->treehash[i].stackusage = 0;
            }
        }
    }
}

/**
 * Given a set of parameters, this function returns the size of the secret key.
 * This is implementation specific, as varying choices in tree traversal will
 * result in varying requirements for state storage.
 *
 * This function handles both XMSS and XMSSMT parameter sets.
 */
unsigned long long xmss_xmssmt_core_sk_bytes(const xmss_params *params)
{
    return params->index_bytes + 4 * params->n
        + (2 * params->d - 1) * (
            (params->tree_height + 1) * params->n
            + 4
            + params->tree_height + 1
            + params->tree_height * params->n
            + (params->tree_height >> 1) * params->n
            + (params->tree_height - params->bds_k) * (7 + params->n)
            + ((1 << params->bds_k) - params->bds_k - 1) * params->n
            + 4
         )
        + (params->d - 1) * params->wots_sig_bytes;
}

/*
 * Generates a XMSS key pair for a given parameter set.
 * Format sk: [(32bit) idx || SK_SEED || SK_PRF || root || PUB_SEED]
 * Format pk: [root || PUB_SEED] omitting algo oid.
 */
int xmss_core_keypair(const xmss_params *params,
                      unsigned char *pk, unsigned char *sk)
{
    uint32_t addr[8] = {0};

    // TODO refactor BDS state not to need separate treehash instances
    bds_state state;
    treehash_inst treehash[params->tree_height - params->bds_k];
    state.treehash = treehash;

    xmss_deserialize_state(params, &state, sk);

    state.stackoffset = 0;
    state.next_leaf = 0;

    // Set idx = 0
    sk[0] = 0;
    sk[1] = 0;
    sk[2] = 0;
    sk[3] = 0;
    // Init SK_SEED (n byte) and SK_PRF (n byte)
    OQS_randombytes(sk + params->index_bytes, 2*params->n);

    // Init PUB_SEED (n byte)
    OQS_randombytes(sk + params->index_bytes + 3*params->n, params->n);
    // Copy PUB_SEED to public key
    memcpy(pk + params->n, sk + params->index_bytes + 3*params->n, params->n);

    // Compute root
    treehash_init(params, pk, params->tree_height, 0, &state, sk + params->index_bytes, sk + params->index_bytes + 3*params->n, addr);
    // copy root to sk
    memcpy(sk + params->index_bytes + 2*params->n, pk, params->n);

    /* Write the BDS state into sk. */
    xmss_serialize_state(params, sk, &state);

    return 0;
}

/**
 * Signs a message.
 * Returns
 * 1. an array containing the signature followed by the message AND
 * 2. an updated secret key!
 *
 */
int xmss_core_sign(const xmss_params *params,
                   unsigned char *sk,
                   unsigned char *sm, unsigned long long *smlen,
                   const unsigned char *m, unsigned long long mlen)
{
    if (params->full_height > 60) {
        // Unsupport Tree height
        return -2;
    }

    const unsigned char *pub_root = sk + params->index_bytes + 2*params->n;

    uint16_t i = 0;

    // TODO refactor BDS state not to need separate treehash instances
    bds_state state;
    treehash_inst treehash[params->tree_height - params->bds_k];
    state.treehash = treehash;

    /* Load the BDS state from sk. */
    xmss_deserialize_state(params, &state, sk);

    // Extract SK
    unsigned long long idx = ((unsigned long long)sk[0] << 24) | ((unsigned long long)sk[1] << 16) | ((unsigned long long)sk[2] << 8) | sk[3];

    /* Check if we can still sign with this sk.
     * If not, return -2
     * 
     * If this is the last possible signature (because the max index value 
     * is reached), production implementations should delete the secret key 
     * to prevent accidental further use.
     * 
     * For the case of total tree height of 64 we do not use the last signature 
     * to be on the safe side (there is no index value left to indicate that the 
     * key is finished, hence external handling would be necessary)
     */ 
    if (idx >= ((1ULL << params->full_height) - 1)) {
        // Delete secret key here. We only do this in memory, production code
        // has to make sure that this happens on disk.
        memset(sk, 0xFF, params->index_bytes);
        memset(sk + params->index_bytes, 0, (params->sk_bytes - params->index_bytes));
        if (idx > ((1ULL << params->full_height) - 1))
            return -2; // We already used all one-time keys
    }
    
    unsigned char sk_seed[params->n];
    memcpy(sk_seed, sk + params->index_bytes, params->n);
    unsigned char sk_prf[params->n];
    memcpy(sk_prf, sk + params->index_bytes + params->n, params->n);
    unsigned char pub_seed[params->n];
    memcpy(pub_seed, sk + params->index_bytes + 3*params->n, params->n);

    // index as 32 bytes string
    unsigned char idx_bytes_32[32];
    ull_to_bytes(idx_bytes_32, 32, idx);

    // Update SK
    sk[0] = ((idx + 1) >> 24) & 255;
    sk[1] = ((idx + 1) >> 16) & 255;
    sk[2] = ((idx + 1) >> 8) & 255;
    sk[3] = (idx + 1) & 255;
    // Secret key for this non-forward-secure version is now updated.
    // A production implementation should consider using a file handle instead,
    //  and write the updated secret key at this point!

    // Init working params
    unsigned char R[params->n];
    unsigned char msg_h[params->n];
    uint32_t ots_addr[8] = {0};

    // ---------------------------------
    // Message Hashing
    // ---------------------------------

    // Message Hash:
    // First compute pseudorandom value
    prf(params, R, idx_bytes_32, sk_prf);

    /* Already put the message in the right place, to make it easier to prepend
     * things when computing the hash over the message. */
    unsigned long long prefix_length = params->padding_len + 3*params->n;
    unsigned char m_with_prefix[mlen + prefix_length];
    memcpy(m_with_prefix, sm + params->sig_bytes - prefix_length, prefix_length);
    memcpy(m_with_prefix + prefix_length, m, mlen);

    /* Compute the message hash. */
    hash_message(params, msg_h, R, pub_root, idx,
                 m_with_prefix,
                 mlen);

    // Start collecting signature
    *smlen = 0;

    // Copy index to signature
    sm[0] = (idx >> 24) & 255;
    sm[1] = (idx >> 16) & 255;
    sm[2] = (idx >> 8) & 255;
    sm[3] = idx & 255;

    sm += 4;
    *smlen += 4;

    // Copy R to signature
    for (i = 0; i < params->n; i++) {
        sm[i] = R[i];
    }

    sm += params->n;
    *smlen += params->n;

    // ----------------------------------
    // Now we start to "really sign"
    // ----------------------------------

    // Prepare Address
    set_type(ots_addr, 0);
    set_ots_addr(ots_addr, idx);

    // Compute WOTS signature
    wots_sign(params, sm, msg_h, sk_seed, pub_seed, ots_addr);

    sm += params->wots_sig_bytes;
    *smlen += params->wots_sig_bytes;

    // the auth path was already computed during the previous round
    memcpy(sm, state.auth, params->tree_height*params->n);

    if (idx < (1U << params->tree_height) - 1) {
        bds_round(params, &state, idx, sk_seed, pub_seed, ots_addr);
        bds_treehash_update(params, &state, (params->tree_height - params->bds_k) >> 1, sk_seed, pub_seed, ots_addr);
    }

    *smlen += params->tree_height*params->n;

    /* Write the updated BDS state back into sk. */
    xmss_serialize_state(params, sk, &state);

    return 0;
}

/*
 * Generates a XMSSMT key pair for a given parameter set.
 * Format sk: [(ceil(h/8) bit) idx || SK_SEED || SK_PRF || root || PUB_SEED]
 * Format pk: [root || PUB_SEED] omitting algo oid.
 */
int xmssmt_core_keypair(const xmss_params *params,
                        unsigned char *pk, unsigned char *sk)
{
    uint32_t addr[8] = {0};
    unsigned int i;
    unsigned char *wots_sigs;

    // TODO refactor BDS state not to need separate treehash instances
    bds_state states[2*params->d - 1];
    treehash_inst treehash[(2*params->d - 1) * (params->tree_height - params->bds_k)];
    for (i = 0; i < 2*params->d - 1; i++) {
        states[i].treehash = treehash + i * (params->tree_height - params->bds_k);
    }

    xmssmt_deserialize_state(params, states, &wots_sigs, sk);

    for (i = 0; i < 2 * params->d - 1; i++) {
        states[i].stackoffset = 0;
        states[i].next_leaf = 0;
    }

    // Set idx = 0
    for (i = 0; i < params->index_bytes; i++) {
        sk[i] = 0;
    }
    // Init SK_SEED (params->n byte) and SK_PRF (params->n byte)
    OQS_randombytes(sk+params->index_bytes, 2*params->n);

    // Init PUB_SEED (params->n byte)
    OQS_randombytes(sk+params->index_bytes + 3*params->n, params->n);
    // Copy PUB_SEED to public key
    memcpy(pk+params->n, sk+params->index_bytes+3*params->n, params->n);

    // Start with the bottom-most layer
    set_layer_addr(addr, 0);
    // Set up state and compute wots signatures for all but topmost tree root
    for (i = 0; i < params->d - 1; i++) {
        // Compute seed for OTS key pair
        treehash_init(params, pk, params->tree_height, 0, states + i, sk+params->index_bytes, pk+params->n, addr);
        set_layer_addr(addr, (i+1));
        wots_sign(params, wots_sigs + i*params->wots_sig_bytes, pk, sk + params->index_bytes, pk+params->n, addr);
    }
    // Address now points to the single tree on layer d-1
    treehash_init(params, pk, params->tree_height, 0, states + i, sk+params->index_bytes, pk+params->n, addr);
    memcpy(sk + params->index_bytes + 2*params->n, pk, params->n);

    xmssmt_serialize_state(params, sk, states);

    return 0;
}

/**
 * Signs a message.
 * Returns
 * 1. an array containing the signature followed by the message AND
 * 2. an updated secret key!
 *
 */
int xmssmt_core_sign(const xmss_params *params,
                     unsigned char *sk,
                     unsigned char *sm, unsigned long long *smlen,
                     const unsigned char *m, unsigned long long mlen)
{
    if (params == NULL || params->full_height > 60) {
        // Unsupport parameter
        return -1;
    }

    const unsigned char *pub_root = sk + params->index_bytes + 2*params->n;

    uint64_t idx_tree;
    uint32_t idx_leaf;
    uint64_t i, j;
    int needswap_upto = -1;
    unsigned int updates;

    unsigned char sk_seed[params->n];
    unsigned char sk_prf[params->n];
    unsigned char pub_seed[params->n];
    // Init working params
    unsigned char R[params->n];
    unsigned char msg_h[params->n];
    uint32_t addr[8] = {0};
    uint32_t ots_addr[8] = {0};
    unsigned char idx_bytes_32[32];

    unsigned char *wots_sigs = NULL;
    unsigned long long prefix_length = params->padding_len + 3*params->n;
    unsigned char m_with_prefix[mlen + prefix_length];
    int ret = 0;

    // TODO refactor BDS state not to need separate treehash instances
    bds_state *states = calloc(2*params->d - 1, sizeof(bds_state));
    treehash_inst treehash[(2*params->d - 1) * (params->tree_height - params->bds_k)];
    for (i = 0; i < 2*params->d - 1; i++) {
        states[i].stack = NULL;
        states[i].stackoffset = 0;
        states[i].stacklevels = NULL;
        states[i].auth = NULL;
        states[i].keep = NULL;
        states[i].treehash = treehash + i * (params->tree_height - params->bds_k);
        states[i].retain = NULL;
        states[i].next_leaf = 0;
    }

    xmssmt_deserialize_state(params, states, &wots_sigs, sk);

    // Extract SK
    unsigned long long idx = 0;
    for (i = 0; i < params->index_bytes; i++) {
        idx |= ((unsigned long long)sk[i]) << 8*(params->index_bytes - 1 - i);
    }

    /* Check if we can still sign with this sk.
     * If not, return -2
     * 
     * If this is the last possible signature (because the max index value 
     * is reached), production implementations should delete the secret key 
     * to prevent accidental further use.
     * 
     * For the case of total tree height of 64 we do not use the last signature 
     * to be on the safe side (there is no index value left to indicate that the 
     * key is finished, hence external handling would be necessary)
     */ 
    if (idx >= ((1ULL << params->full_height) - 1)) {
        // Delete secret key here. We only do this in memory, production code
        // has to make sure that this happens on disk.
        memset(sk, 0xFF, params->index_bytes);
        memset(sk + params->index_bytes, 0, (params->sk_bytes - params->index_bytes));
        if (idx > ((1ULL << params->full_height) - 1)) {
            // We already used all one-time keys
            ret = -2;
            goto cleanup;
        }
    }
    
    memcpy(sk_seed, sk+params->index_bytes, params->n);
    memcpy(sk_prf, sk+params->index_bytes+params->n, params->n);
    memcpy(pub_seed, sk+params->index_bytes+3*params->n, params->n);

    // Update SK
    for (i = 0; i < params->index_bytes; i++) {
        sk[i] = ((idx + 1) >> 8*(params->index_bytes - 1 - i)) & 255;
    }
    // Secret key for this non-forward-secure version is now updated.
    // A production implementation should consider using a file handle instead,
    //  and write the updated secret key at this point!

    // ---------------------------------
    // Message Hashing
    // ---------------------------------

    // Message Hash:
    // First compute pseudorandom value
    ull_to_bytes(idx_bytes_32, 32, idx);
    prf(params, R, idx_bytes_32, sk_prf);

    /* Already put the message in the right place, to make it easier to prepend
     * things when computing the hash over the message. */
    memcpy(m_with_prefix, sm + params->sig_bytes - prefix_length, prefix_length);
    memcpy(m_with_prefix + prefix_length, m, mlen);

    /* Compute the message hash. */
    hash_message(params, msg_h, R, pub_root, idx,
                m_with_prefix,
                 mlen);

    // Start collecting signature
    *smlen = 0;

    // Copy index to signature
    for (i = 0; i < params->index_bytes; i++) {
        sm[i] = (idx >> 8*(params->index_bytes - 1 - i)) & 255;
    }

    sm += params->index_bytes;
    *smlen += params->index_bytes;

    // Copy R to signature
    for (i = 0; i < params->n; i++) {
        sm[i] = R[i];
    }

    sm += params->n;
    *smlen += params->n;

    // ----------------------------------
    // Now we start to "really sign"
    // ----------------------------------

    // Handle lowest layer separately as it is slightly different...

    // Prepare Address
    set_type(ots_addr, 0);
    idx_tree = idx >> params->tree_height;
    idx_leaf = (idx & ((1 << params->tree_height)-1));
    set_layer_addr(ots_addr, 0);
    set_tree_addr(ots_addr, idx_tree);
    set_ots_addr(ots_addr, idx_leaf);

    // Compute WOTS signature
    wots_sign(params, sm, msg_h, sk_seed, pub_seed, ots_addr);

    sm += params->wots_sig_bytes;
    *smlen += params->wots_sig_bytes;

    memcpy(sm, states[0].auth, params->tree_height*params->n);
    sm += params->tree_height*params->n;
    *smlen += params->tree_height*params->n;

    // prepare signature of remaining layers
    for (i = 1; i < params->d; i++) {
        // put WOTS signature in place
        memcpy(sm, wots_sigs + (i-1)*params->wots_sig_bytes, params->wots_sig_bytes);

        sm += params->wots_sig_bytes;
        *smlen += params->wots_sig_bytes;

        // put AUTH nodes in place
        if (states[i].auth == NULL) {
            ret = -1;
            goto cleanup;
        }
        memcpy(sm, states[i].auth, params->tree_height*params->n);
        sm += params->tree_height*params->n;
        *smlen += params->tree_height*params->n;
    }

    updates = (params->tree_height - params->bds_k) >> 1;

    set_tree_addr(addr, (idx_tree + 1));
    // mandatory update for NEXT_0 (does not count towards h-k/2) if NEXT_0 exists
    if ((1 + idx_tree) * (1 << params->tree_height) + idx_leaf < (1ULL << params->full_height)) {
        bds_state_update(params, &states[params->d], sk_seed, pub_seed, addr);
    }

    for (i = 0; i < params->d; i++) {
        // check if we're not at the end of a tree
        if (! (((idx + 1) & ((1ULL << ((i+1)*params->tree_height)) - 1)) == 0)) {
            idx_leaf = (idx >> (params->tree_height * i)) & ((1 << params->tree_height)-1);
            idx_tree = (idx >> (params->tree_height * (i+1)));
            set_layer_addr(addr, i);
            set_tree_addr(addr, idx_tree);
            if (i == (unsigned int) (needswap_upto + 1)) {
                bds_round(params, &states[i], idx_leaf, sk_seed, pub_seed, addr);
            }
            updates = bds_treehash_update(params, &states[i], updates, sk_seed, pub_seed, addr);
            set_tree_addr(addr, (idx_tree + 1));
            // if a NEXT-tree exists for this level;
            if ((1 + idx_tree) * (1 << params->tree_height) + idx_leaf < (1ULL << (params->full_height - params->tree_height * i))) {
                if (i > 0 && updates > 0 && states[params->d + i].next_leaf < (1ULL << params->full_height)) {
                    bds_state_update(params, &states[params->d + i], sk_seed, pub_seed, addr);
                    updates--;
                }
            }
        }
        else if (idx < (1ULL << params->full_height) - 1) {
            deep_state_swap(params, &states[params->d + i], &states[i]);

            set_layer_addr(ots_addr, (i+1));
            set_tree_addr(ots_addr, ((idx + 1) >> ((i+2) * params->tree_height)));
            set_ots_addr(ots_addr, (((idx >> ((i+1) * params->tree_height)) + 1) & ((1 << params->tree_height)-1)));

            wots_sign(params, wots_sigs + i*params->wots_sig_bytes, states[i].stack, sk_seed, pub_seed, ots_addr);

            states[params->d + i].stackoffset = 0;
            states[params->d + i].next_leaf = 0;

            updates--; // WOTS-signing counts as one update
            needswap_upto = i;
            for (j = 0; j < params->tree_height-params->bds_k; j++) {
                states[i].treehash[j].completed = 1;
            }
        }
    }

    xmssmt_serialize_state(params, sk, states);
    goto cleanup;

cleanup:
    OQS_MEM_insecure_free(states);

    return ret;
}
