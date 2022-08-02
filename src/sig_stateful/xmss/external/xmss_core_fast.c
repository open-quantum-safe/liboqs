#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include <oqs/rand.h>
#include "hash.h"
#include "hash_address.h"
#include "params.h"
#include "wots.h"
#include "utils.h"
#include "xmss_commons.h"
#include "xmss_core.h"
#include "xmss_namespace.h"

typedef struct{
    uint8_t h;
    unsigned long next_idx;
    uint8_t stackusage;
    uint8_t completed;
    uint8_t *node;
    #ifdef FORWARD_SECURE
        uint8_t *seed_active;
        uint8_t *seed_next;
    #endif
} treehash_inst;

typedef struct {
    uint8_t *stack;
    unsigned int stackoffset;
    uint8_t *stacklevels;
    uint8_t *auth;
    uint8_t *keep;
    treehash_inst *treehash;
    uint8_t *retain;
    unsigned int next_leaf;
    #ifdef FORWARD_SECURE
    uint8_t *left_leaf;
    #endif
} bds_state;

/* These serialization functions provide a transition between the current
   way of storing the state in an exposed struct, and storing it as part of the
   byte array that is the secret key.
   They will probably be refactored in a non-backwards-compatible way, soon. */

static void xmssmt_serialize_state(const xmss_params *params,
                                   uint8_t *sk, bds_state *states)
{
    unsigned int i, j;

    /* Skip past the 'regular' sk */
    #ifdef FORWARD_SECURE
    sk += params->index_bytes + (2 + 2*params->d)*params->n;
    #else
    sk += params->index_bytes + 4*params->n;
    #endif

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
            #ifdef FORWARD_SECURE
            sk += params->n; /* seed_active */
            sk += params->n; /* seed_next */
            #endif
        }

        /* retain */
        sk += ((1 << params->bds_k) - params->bds_k - 1) * params->n;

        ull_to_bytes(sk, 4, states[i].next_leaf);
        sk += 4;

        #ifdef FORWARD_SECURE
        /* left_leaf */
        sk += params->n;
        #endif
    }
}

static void xmssmt_deserialize_state(const xmss_params *params,
                                     bds_state *states,
                                     uint8_t **wots_sigs,
                                     uint8_t *sk)
{
    unsigned int i, j;

    /* Skip past the 'regular' sk */
    #ifdef FORWARD_SECURE
    sk += params->index_bytes + (2 + 2*params->d)*params->n;
    #else
    sk += params->index_bytes + 4*params->n;
    #endif

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

            #ifdef FORWARD_SECURE
            states[i].treehash[j].seed_active = sk;
            sk += params->n;

            states[i].treehash[j].seed_next = sk;
            sk += params->n;
            #endif
        }

        states[i].retain = sk;
        sk += ((1 << params->bds_k) - params->bds_k - 1) * params->n;

        states[i].next_leaf = bytes_to_ull(sk, 4);
        sk += 4;
        #ifdef FORWARD_SECURE
        states[i].left_leaf = sk;
        sk += params->n;
        #endif
    }

    if (params->d > 1) {
        *wots_sigs = sk;
    }
}

static void xmss_serialize_state(const xmss_params *params,
                                 uint8_t *sk, bds_state *state)
{
    xmssmt_serialize_state(params, sk, state);
}

static void xmss_deserialize_state(const xmss_params *params,
                                   bds_state *state, uint8_t *sk)
{
    xmssmt_deserialize_state(params, state, NULL, sk);
}


/**
 * Move the content of one bds_state object into another
 */
static void deep_state_move(const xmss_params *params,
                            bds_state *dst, bds_state *src)
{
    unsigned int i;
    memmove(dst->stack, src->stack,(params->tree_height + 1) * params->n);
    dst->stackoffset = src->stackoffset;
    memmove(dst->stacklevels, src->stacklevels, params->tree_height + 1);
    memmove(dst->auth, src->auth, params->tree_height * params->n);
    memmove(dst->keep, src->keep, (params->tree_height >> 1) * params->n);

    for (i = 0; i < params->tree_height - params->bds_k; i++) {
        dst->treehash[i].h = src->treehash[i].h;
        dst->treehash[i].next_idx = src->treehash[i].next_idx;
        dst->treehash[i].stackusage = src->treehash[i].stackusage;
        dst->treehash[i].completed = src->treehash[i].completed;
        memmove(dst->treehash[i].node, src->treehash[i].node, params->n);
        #ifdef FORWARD_SECURE
        memmove(dst->treehash[i].seed_active, src->treehash[i].seed_active, params->n);
        memmove(dst->treehash[i].seed_next, src->treehash[i].seed_next, params->n);
        // erase the seeds
        memset(src->treehash[i].seed_active, 0, params->n);
        memset(src->treehash[i].seed_next, 0, params->n);
        #endif
    }

    memmove(dst->retain, src->retain, ((1 << params->bds_k) - params->bds_k - 1) * params->n);
    dst->next_leaf = src->next_leaf;


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
                          uint8_t *node, int height, int index,
                          bds_state *state, uint8_t *sk_seed,
                          const uint8_t *pub_seed, const uint32_t addr[8])
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
    uint8_t stack[(height+1)*params->n];
    unsigned int stacklevels[height+1];
    unsigned int stackoffset=0;
    unsigned int nodeh;

    #ifdef FORWARD_SECURE
    uint8_t ots_seed[params->n];
    #endif

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
        #ifdef FORWARD_SECURE
        hash_prg(params, ots_seed, sk_seed, sk_seed, pub_seed, ots_addr);
        gen_leaf_wots(params, stack+stackoffset*params->n, ots_seed, pub_seed, ltree_addr, ots_addr);
        #else
        gen_leaf_wots(params, stack+stackoffset*params->n, sk_seed, pub_seed, ltree_addr, ots_addr);
        #endif
        stacklevels[stackoffset] = 0;
        stackoffset++;
        if (params->tree_height - params->bds_k > 0 && i == 3) {
            memcpy(state->treehash[0].node, stack+stackoffset*params->n, params->n);
        }
        #ifdef FORWARD_SECURE
        unsigned int j;
        for(j=0; j <  params->tree_height - params->bds_k - 1; j++) {
            if(idx >> j == 3 && (idx & ((1<<j)-1)) == 0) {
                memcpy(state->treehash[j].seed_next, sk_seed, params->n);
            }
        }
        #endif

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
}

static void treehash_update(const xmss_params *params,
                            treehash_inst *treehash, bds_state *state,
                            const uint8_t *sk_seed,
                            const uint8_t *pub_seed,
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

    uint8_t nodebuffer[2 * params->n];
    unsigned int nodeheight = 0;
    #ifdef FORWARD_SECURE
    uint8_t ots_seed[params->n];

    //sk_seed is not needed here suppress warning
    (void) sk_seed;

    hash_prg(params, ots_seed, treehash->seed_active, treehash->seed_active, pub_seed, ots_addr);
    gen_leaf_wots(params, nodebuffer, ots_seed, pub_seed, ltree_addr, ots_addr);
    #else
    gen_leaf_wots(params, nodebuffer, sk_seed, pub_seed, ltree_addr, ots_addr);
    #endif
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
                                const uint8_t *sk_seed,
                                uint8_t *pub_seed,
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
                             bds_state *state, uint8_t *sk_seed,
                             const uint8_t *pub_seed,
                             const uint32_t addr[8])
{
    uint32_t ltree_addr[8] = {0};
    uint32_t node_addr[8] = {0};
    uint32_t ots_addr[8] = {0};

    unsigned int nodeh;
    int idx = state->next_leaf;

    #ifdef FORWARD_SECURE
    uint8_t ots_seed[params->n];
    #endif


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

    #ifdef FORWARD_SECURE
    hash_prg(params, ots_seed, sk_seed, sk_seed, pub_seed, ots_addr);
    gen_leaf_wots(params, state->stack+state->stackoffset*params->n, ots_seed, pub_seed, ltree_addr, ots_addr);
    #else
    gen_leaf_wots(params, state->stack+state->stackoffset*params->n, sk_seed, pub_seed, ltree_addr, ots_addr);
    #endif

    state->stacklevels[state->stackoffset] = 0;
    state->stackoffset++;
    if (params->tree_height - params->bds_k > 0 && idx == 3) {
        memcpy(state->treehash[0].node, state->stack+state->stackoffset*params->n, params->n);
    }

    #ifdef FORWARD_SECURE
    unsigned int j;
    for(j=0; j <  params->tree_height - params->bds_k-1; j++){
        int idxtmp = idx;
        if(addr[0] == 0) {
            idxtmp++;
        }
        if(idxtmp >> j == 3 && (idxtmp & ((1<<j)-1))==0){
            memcpy(state->treehash[j].seed_next, sk_seed, params->n);
        }
    }
    #endif

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
                      const uint8_t *sk_seed,
                      const uint8_t *pub_seed, uint32_t addr[8])
{
    unsigned int i;
    unsigned int tau = params->tree_height;
    unsigned int startidx;
    unsigned int offset, rowidx;
    uint8_t buf[2 * params->n];

    uint32_t ots_addr[8] = {0};
    uint32_t ltree_addr[8] = {0};
    uint32_t node_addr[8] = {0};

    #ifdef FORWARD_SECURE
    uint8_t ots_seed[params->n];
    #endif

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

        #ifdef FORWARD_SECURE
        // if lowest layer, generate a new leaf
        if(addr[0] == 0){
            hash_prg(params, ots_seed, NULL, sk_seed, pub_seed, ots_addr);
            gen_leaf_wots(params, state->auth, ots_seed, pub_seed, ltree_addr, ots_addr);
        } else {
            // otherwise use the cached left leaf (as seed was already deleted)
            memcpy(state->auth, state->left_leaf, params->n);
        }
        #else
        gen_leaf_wots(params, state->auth, sk_seed, pub_seed, ltree_addr, ots_addr);
        #endif
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

                #ifdef FORWARD_SECURE
                memcpy(state->treehash[i].seed_active, state->treehash[i].seed_next, params->n);
                #endif
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
            #ifdef FORWARD_SECURE
            + (params->tree_height - params->bds_k) * (7 + 3*params->n)
            + params->n
            #else
            + (params->tree_height - params->bds_k) * (7 + params->n)
            #endif
            + ((1 << params->bds_k) - params->bds_k - 1) * params->n
            + 4
         )
        #ifdef FORWARD_SECURE
        + (2 * params->d - 2) * params->n  //seeds
        #endif
        + (params->d - 1) * params->wots_sig_bytes;
}

int xmss_core_increment_authpath(const xmss_params *params, uint8_t *sk, unsigned long long amount) {
    return xmssmt_core_increment_authpath(params, sk, amount);
}

/*
 * Generates a XMSS key pair for a given parameter set.
 * Format sk: [(32bit) idx || SK_SEED || SK_PRF || root || PUB_SEED]
 * Format pk: [root || PUB_SEED] omitting algo oid.
 */
int xmss_core_keypair(const xmss_params *params,
                      uint8_t *pk, uint8_t *sk)
{
    uint32_t addr[8] = {0};

    #ifdef FORWARD_SECURE
    uint8_t ots_seed[params->n];
    #endif

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
    #ifdef FORWARD_SECURE
    memcpy(ots_seed, sk + params->index_bytes, params->n);
    treehash_init(params, pk, params->tree_height, 0, &state, ots_seed, sk + params->index_bytes + 3*params->n, addr);
    #else
    treehash_init(params, pk, params->tree_height, 0, &state, sk + params->index_bytes, sk + params->index_bytes + 3*params->n, addr);
    #endif
    // copy root to sk
    memcpy(sk + params->index_bytes + 2*params->n, pk, params->n);

    /* Write the BDS state into sk. */
    xmss_serialize_state(params, sk, &state);

    return 0;
}

/**
 * Signs a message.
 * Returns
 * 1. an array containing the signature  AND
 * 2. an updated secret key!
 *
 */
int xmss_core_sign(const xmss_params *params,
                   OQS_SECRET_KEY *secret_key,
                   uint8_t *sm, unsigned long long *smlen,
                   const uint8_t *m, unsigned long long mlen)
{
    uint8_t *sk = secret_key->secret_key + XMSS_OID_LEN;

    const uint8_t *pub_root = sk + params->index_bytes + 2*params->n;

    uint16_t i = 0;

    bds_state state;
    treehash_inst treehash[params->tree_height - params->bds_k];
    state.treehash = treehash;
    
    // Lock the secret key object until all our read / write operations on it are complete.
    secret_key->lock_key(secret_key);

    // Extract index
    unsigned long long idx = bytes_to_ull(sk, params->index_bytes);

    /* Check if we can still sign with this sk, return -2 if not: */
    // Extract the max_sigs
    unsigned long long max = bytes_to_ull(sk + params->sk_bytes - params->bytes_for_max, params->bytes_for_max);

    if (idx >= max) {
        printf("ERROR! Exceeded maximum number of sigs");
        return -2;
    }
    /* Load the BDS state from sk. */
    xmss_deserialize_state(params, &state, sk);

    // Extract remaining SK
    uint8_t sk_seed[params->n];
    memcpy(sk_seed, sk + params->index_bytes, params->n);
    uint8_t sk_prf[params->n];
    memcpy(sk_prf, sk + params->index_bytes + params->n, params->n);
    uint8_t pub_seed[params->n];
    memcpy(pub_seed, sk + params->index_bytes + 3*params->n, params->n);

    // index as 32 bytes string
    uint8_t idx_bytes_32[32];
    ull_to_bytes(idx_bytes_32, 32, idx);

     /** ===============================================================================
     * This is where the key update procedure takes place, this is the only change that
     * is made in XMSS. The counter is incremented. */

    // Update SK

    sk[0] = ((idx + 1) >> 24) & 255;
    sk[1] = ((idx + 1) >> 16) & 255;
    sk[2] = ((idx + 1) >> 8) & 255;
    sk[3] = (idx + 1) & 255;

    /* Secret key for this non-forward-secure version is now updated.
       A production implementation should consider using a file handle instead,
       and write the updated secret key at this point! */

    /** =============================================================================== */


    // Init working params
    uint8_t R[params->n];
    uint8_t msg_h[params->n];
    uint8_t ots_seed[params->n];
    uint32_t ots_addr[8] = {0};

    // ---------------------------------
    // Message Hashing
    // ---------------------------------

    // Message Hash:
    // First compute pseudorandom value
    prf(params, R, idx_bytes_32, sk_prf);

    uint8_t *buffer = (uint8_t*)malloc((mlen + 4 * params->n) * sizeof(uint8_t));
    
    memcpy(buffer + 4* params->n, m, mlen);
    
    /* Compute the message hash. */
    hash_message(params, msg_h, R, pub_root, idx,
                buffer, mlen);

    free(buffer);

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

    // Compute seed for OTS key pair
    #ifdef FORWARD_SECURE
    hash_prg(params, ots_seed, sk + params->index_bytes, sk + params->index_bytes, pub_seed, ots_addr);
    #else
    get_seed(params, ots_seed, sk_seed, ots_addr);
    #endif

    // Compute WOTS signature
    wots_sign(params, sm, msg_h, ots_seed, pub_seed, ots_addr);

    sm += params->wots_sig_bytes;
    *smlen += params->wots_sig_bytes;

    // the auth path was already computed during the previous round
    memcpy(sm, state.auth, params->tree_height*params->n);

    bds_round(params, &state, idx, sk_seed, pub_seed, ots_addr);
    bds_treehash_update(params, &state, (params->tree_height - params->bds_k) >> 1, sk_seed, pub_seed, ots_addr);

    sm += params->tree_height*params->n;
    *smlen += params->tree_height*params->n;

    #ifdef FORWARD_SECURE
    // move forward next seeds for all tree hash instances
    for (i = 0; i < params->tree_height-params->bds_k-1; i++) {
        set_ots_addr(ots_addr, 1+3*(1<<i)+idx);
        hash_prg(params, NULL, state.treehash[i].seed_next, state.treehash[i].seed_next, pub_seed, ots_addr);
    }

    #endif
    /* Write the updated BDS state back into sk. */
    xmss_serialize_state(params, sk, &state);

    secret_key->oqs_save_updated_sk_key(secret_key);

    secret_key->release_key(secret_key);

    return 0;
}

int xmssmt_core_increment_authpath(const xmss_params *params, uint8_t *sk, unsigned long long amount) {
    unsigned long long i, j = 0;
    
    bds_state state;
    treehash_inst treehash[params->tree_height - params->bds_k];
    state.treehash = treehash;

    xmss_deserialize_state(params, &state, sk);
    
    // Extract remaining SK
    uint8_t sk_seed[params->n];
    memcpy(sk_seed, sk + params->index_bytes, params->n);
    uint8_t sk_prf[params->n];
    memcpy(sk_prf, sk + params->index_bytes + params->n, params->n);
    uint8_t pub_seed[params->n];
    memcpy(pub_seed, sk + params->index_bytes + 3*params->n, params->n);

    // Init working params;
    uint32_t ots_addr[8] = {0};

    /* Check if we can still sign with this sk, return -2 if not: */
    // Extract index
    unsigned long long idx = bytes_to_ull(sk, params->index_bytes);
    // Extract the max_sigs
    unsigned long long max = bytes_to_ull(sk + params->sk_bytes - params->bytes_for_max, params->bytes_for_max);
    if (idx >= max) {
        return -2;
    }

    // Update SK
    sk[0] = ((idx + amount) >> 24) & 255;
    sk[1] = ((idx + amount) >> 16) & 255;
    sk[2] = ((idx + amount) >> 8) & 255;
    sk[3] = (idx + amount) & 255;

    for (i = 0; i < amount; i++) {
        bds_round(params, &state, idx, sk_seed, pub_seed, ots_addr);
        bds_treehash_update(params, &state, (params->tree_height - params->bds_k) >> 1, sk_seed, pub_seed, ots_addr);
        
        #ifdef FORWARD_SECURE
            // move forward next seeds for all tree hash instances
            for (j = 0; j < params->tree_height-params->bds_k-1; j++) {
                set_ots_addr(ots_addr, 1+3*(1<<i)+idx);
                hash_prg(params, NULL, state.treehash[i].seed_next, state.treehash[i].seed_next, pub_seed, ots_addr);
            }
        #endif
    }
    
    xmss_serialize_state(params, sk, &state);
    
    return 0;
}

/*
 * Generates a XMSSMT key pair for a given parameter set.
 * Format sk: [(ceil(h/8) bit) idx || SK_SEED || SK_PRF || root || PUB_SEED]
 * Format pk: [root || PUB_SEED] omitting algo oid.
 */
int xmssmt_core_keypair(const xmss_params *params,
                        uint8_t *pk, uint8_t *sk)
{
    uint8_t ots_seed[params->n];

    uint32_t addr[8] = {0};
    unsigned int i;
    uint8_t *wots_sigs;

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


    #ifdef FORWARD_SECURE
    // Init SK_SEED (params->n byte * params->d) and SK_PRF (params->n byte)
    OQS_randombytes(sk+params->index_bytes, (1 + params->d)*params->n);

    // Init PUB_SEED (params->n byte)
    OQS_randombytes(sk+params->index_bytes + (2 + params->d)*params->n, params->n);

    // Copy PUB_SEED to public key
    memcpy(pk+params->n, sk+params->index_bytes+ (2 + params->d)*params->n, params->n);

    // next seeds contains the seeds for the NEXT_i trees
    uint8_t *next_seeds = sk+params->index_bytes+ (3 + params->d)*params->n;

    #else
    // Init SK_SEED (params->n byte) and SK_PRF (params->n byte)
    OQS_randombytes(sk+params->index_bytes, 2*params->n);

    // Init PUB_SEED (params->n byte)
    OQS_randombytes(sk+params->index_bytes + 3*params->n, params->n);
    // Copy PUB_SEED to public key
    memcpy(pk+params->n, sk+params->index_bytes+3*params->n, params->n);
    #endif

    // Start with the bottom-most layer
    set_layer_addr(addr, 0);
    // Set up state and compute wots signatures for all but topmost tree root
    for (i = 0; i < params->d - 1; i++) {
        // Compute seed for OTS key pair
        #ifdef FORWARD_SECURE
        memcpy(ots_seed, sk+params->index_bytes+i*params->n, params->n);
        treehash_init(params, pk, params->tree_height, 0, states + i, ots_seed, pk+params->n, addr);

        // treehash_init outputs the seed of the NEXT tree on that layer
        memcpy(next_seeds + i * params->n, ots_seed, params->n);

        set_layer_addr(addr, (i+1));
        hash_prg(params, ots_seed, NULL, sk+params->index_bytes+(i+1)*params->n, pk+params->n, addr);
        wots_sign(params, wots_sigs + i*params->wots_sig_bytes, pk, ots_seed, pk+params->n, addr);


        uint8_t tmp_wots_leaf[params->n];
        uint32_t ltree_addr[8] = {0};
        memcpy(ltree_addr, addr, sizeof ltree_addr);
        set_type(ltree_addr, 1);
        gen_leaf_wots(params, tmp_wots_leaf, ots_seed, pk+params->n, ltree_addr, addr);
        memcpy(states[i+1].left_leaf, tmp_wots_leaf, sizeof  tmp_wots_leaf);
        #else
        treehash_init(params, pk, params->tree_height, 0, states + i, sk+params->index_bytes, pk+params->n, addr);
        set_layer_addr(addr, (i+1));
        get_seed(params, ots_seed, sk + params->index_bytes, addr);
        wots_sign(params, wots_sigs + i*params->wots_sig_bytes, pk, ots_seed, pk+params->n, addr);
        #endif
    }
    // Address now points to the single tree on layer d-1
    #ifdef FORWARD_SECURE
    memcpy(ots_seed, sk+params->index_bytes+(params->d - 1)*params->n, params->n);
    treehash_init(params, pk, params->tree_height, 0, states + i, ots_seed, pk+params->n, addr);
    memcpy(sk + params->index_bytes + (1+params->d)*params->n, pk, params->n);

    set_tree_addr(addr, 0);
    set_ots_addr(addr, 0);
    for (i = 1; i < params->d; i++) {
        set_layer_addr(addr, i);
        hash_prg(params, NULL, sk+params->index_bytes+i*params->n, sk+params->index_bytes+i*params->n, pk+params->n, addr);
    }

    #else
    treehash_init(params, pk, params->tree_height, 0, states + i, sk+params->index_bytes, pk+params->n, addr);
    memcpy(sk + params->index_bytes + 2*params->n, pk, params->n);
    #endif

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
                     OQS_SECRET_KEY *secret_key,
                     uint8_t *sm, unsigned long long *smlen,
                     const uint8_t *m, unsigned long long mlen)
{

    uint8_t *sk = secret_key->secret_key + XMSS_OID_LEN;

    #ifdef FORWARD_SECURE
    const uint8_t *pub_root = sk + params->index_bytes + (1 + params->d)*params->n;
    uint8_t *next_seeds = sk + params->index_bytes + (3 + params->d)*params->n;
    #else
    const uint8_t *pub_root = sk + params->index_bytes + 2*params->n;
    #endif
    uint64_t idx_tree;
    uint32_t idx_leaf;
    uint64_t i, j;
    int needswap_upto = -1;
    unsigned int updates;

    uint8_t sk_seed[params->n];
    uint8_t sk_prf[params->n];
    uint8_t pub_seed[params->n];
    // Init working params
    uint8_t R[params->n];
    uint8_t msg_h[params->n];
    uint8_t ots_seed[params->n];
    uint32_t addr[8] = {0};
    uint32_t ots_addr[8] = {0};
    uint8_t idx_bytes_32[32];

    uint8_t *wots_sigs;

    bds_state states[2*params->d - 1];
    treehash_inst treehash[(2*params->d - 1) * (params->tree_height - params->bds_k)];
    for (i = 0; i < 2*params->d - 1; i++) {
        states[i].treehash = treehash + i * (params->tree_height - params->bds_k);
    }

    xmssmt_deserialize_state(params, states, &wots_sigs, sk);

    // Extract SK
    secret_key->lock_key(secret_key);

        // Extract index
    unsigned long long idx = bytes_to_ull(sk, params->index_bytes);

    /* Check if we can still sign with this sk, return -2 if not: */
    // Extract the max_sigs
    unsigned long long max = bytes_to_ull(sk + params->sk_bytes - params->bytes_for_max, params->bytes_for_max);

    /* ========= CHECKING AGAINST MAX =========== */
    // Check if we can still sign with this sk, return -2 if not: */
    if (idx >= max) {
        printf("ERROR! Exceeded maximum number of sigs\n");
        return -2;
    }

    memcpy(sk_seed, sk+params->index_bytes, params->n);
    #ifdef FORWARD_SECURE
    memcpy(sk_prf, sk+params->index_bytes+params->d*params->n, params->n);
    memcpy(pub_seed, sk+params->index_bytes+(2+params->d)*params->n, params->n);
    #else
    memcpy(sk_prf, sk+params->index_bytes+params->n, params->n);
    memcpy(pub_seed, sk+params->index_bytes+3*params->n, params->n);
    #endif

    /** ===============================================================================
     * This is where the key update procedure takes place, this is the only change that
     * is made in XMSS. The counter is incremented. */

    // Update SK
    for (i = 0; i < params->index_bytes; i++) {
        sk[i] = ((idx + 1) >> 8*(params->index_bytes - 1 - i)) & 255;
    }
    /* Secret key for this non-forward-secure version is now updated.
       A production implementation should consider using a file handle instead,
       and write the updated secret key at this point! */

    /** =============================================================================== */


    // ---------------------------------
    // Message Hashing
    // ---------------------------------

    // Message Hash:
    // First compute pseudorandom value
    ull_to_bytes(idx_bytes_32, 32, idx);
    prf(params, R, idx_bytes_32, sk_prf);

    uint8_t *buffer = (uint8_t*)malloc((mlen + 4 * params->n) * sizeof(uint8_t));
    
    memcpy(buffer + 4* params->n, m, mlen);
    
    /* Compute the message hash. */
    hash_message(params, msg_h, R, pub_root, idx,
                buffer, mlen);

    free(buffer);


    // /* Already put the message in the right place, to make it easier to prepend
    //  * things when computing the hash over the message. */
    // memcpy(sm + params->sig_bytes, m, mlen);

    // /* Compute the message hash. */
    // hash_message(params, msg_h, R, pub_root, idx,
    //              sm + params->sig_bytes - 4*params->n, mlen);

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

    #ifdef FORWARD_SECURE
    hash_prg(params, ots_seed, sk+params->index_bytes, sk+params->index_bytes, pub_seed, ots_addr);
    #else
    // Compute seed for OTS key pair
    get_seed(params, ots_seed, sk_seed, ots_addr);
    #endif

    // Compute WOTS signature
    wots_sign(params, sm, msg_h, ots_seed, pub_seed, ots_addr);

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
        memcpy(sm, states[i].auth, params->tree_height*params->n);
        sm += params->tree_height*params->n;
        *smlen += params->tree_height*params->n;
    }

    updates = (params->tree_height - params->bds_k) >> 1;

    set_tree_addr(addr, (idx_tree + 1));
    // mandatory update for NEXT_0 (does not count towards h-k/2) if NEXT_0 exists
    if ((1 + idx_tree) * (1 << params->tree_height) + idx_leaf < (1ULL << params->full_height)) {
        #ifdef FORWARD_SECURE
        bds_state_update(params, &states[params->d], next_seeds, pub_seed, addr);
        #else
        bds_state_update(params, &states[params->d], sk_seed, pub_seed, addr);
        #endif
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
            #ifdef FORWARD_SECURE
            updates = bds_treehash_update(params, &states[i], updates, sk_seed + i*params->n, pub_seed, addr);
            #else
            updates = bds_treehash_update(params, &states[i], updates, sk_seed, pub_seed, addr);
            #endif

            set_tree_addr(addr, (idx_tree + 1));
            // if a NEXT-tree exists for this level;
            if ((1 + idx_tree) * (1 << params->tree_height) + idx_leaf < (1ULL << (params->full_height - params->tree_height * i))) {
                if (i > 0 && updates > 0 && states[params->d + i].next_leaf < (1ULL << params->full_height)) {
                    #ifdef FORWARD_SECURE
                    bds_state_update(params, &states[params->d + i], next_seeds + i * params->n, pub_seed, addr);
                    #else
                    bds_state_update(params, &states[params->d + i], sk_seed, pub_seed, addr);
                    #endif
                    updates--;
                }
            }
        }
        else if (idx < (1ULL << params->full_height) - 1) {
            deep_state_move(params, states + i, states+params->d + i);

            set_layer_addr(ots_addr, (i+1));
            set_tree_addr(ots_addr, ((idx + 1) >> ((i+2) * params->tree_height)));
            set_ots_addr(ots_addr, (((idx >> ((i+1) * params->tree_height)) + 1) & ((1 << params->tree_height)-1)));
            #ifdef FORWARD_SECURE
            hash_prg(params, ots_seed, sk + params->index_bytes + (i+1) * params->n, sk + params->index_bytes + (i+1) * params->n, pub_seed, ots_addr);
            #else
            get_seed(params, ots_seed, sk+params->index_bytes, ots_addr);
            #endif
            wots_sign(params, wots_sigs + i*params->wots_sig_bytes, states[i].stack, ots_seed, pub_seed, ots_addr);

            #ifdef FORWARD_SECURE
            // if this is a left node, we need to store it for BDS
            if ((((idx >> ((i+1) * params->tree_height)) + 1) & ((1 << params->tree_height)-1)) % 2 == 0){
                uint8_t tmp_wots_leaf[params->n];
                uint32_t ltree_addr[8] = {0};
                memcpy(ltree_addr, ots_addr, sizeof ltree_addr);
                set_type(ltree_addr, 1);
                gen_leaf_wots(params, tmp_wots_leaf, ots_seed, pub_seed, ltree_addr, ots_addr);
                memcpy(states[i+1].left_leaf, tmp_wots_leaf, sizeof  tmp_wots_leaf);
            }

            // move forward next seeds for all tree hash instances
            for (j = 0; j < params->tree_height-params->bds_k-1; j++) {
                if((((idx >> ((i+1) * params->tree_height)) + 1) & ((1 << params->tree_height)-1)) == 1) continue;
                set_layer_addr(ots_addr, i+1);
                int treehash_seed_idx = 1 + (1<<j)*3 + (((idx >> ((i+1) * params->tree_height)) + 1));
                idx_tree = (treehash_seed_idx-2) >> (params->tree_height*(i+1));
                idx_leaf = (treehash_seed_idx-2) & ((1 << ((i+1)*params->tree_height))-1);
                set_ots_addr(ots_addr, idx_leaf);
                set_tree_addr(ots_addr, idx_tree);

                hash_prg(params, NULL, states[i+1].treehash[j].seed_next, states[i+1].treehash[j].seed_next, pub_seed, ots_addr);
            }
            #endif
            states[params->d + i].stackoffset = 0;
            states[params->d + i].next_leaf = 0;

            updates--; // WOTS-signing counts as one update
            needswap_upto = i;
            for (j = 0; j < params->tree_height-params->bds_k; j++) {
                states[i].treehash[j].completed = 1;
            }
        }
    }

    #ifdef FORWARD_SECURE
    set_layer_addr(ots_addr, 0);
    // move forward next seeds for all tree hash instances
    for (i = 0; i < params->tree_height-params->bds_k-1; i++) {
        int treehash_seed_idx = 1 + (1<<i)*3 + idx;
        idx_tree = treehash_seed_idx >> params->tree_height;
        idx_leaf = (treehash_seed_idx & ((1 << params->tree_height)-1));

        set_ots_addr(ots_addr, idx_leaf);
        set_tree_addr(ots_addr, idx_tree);
        hash_prg(params, NULL, states[0].treehash[i].seed_next, states[0].treehash[i].seed_next, pub_seed, ots_addr);
    }

    #endif
    xmssmt_serialize_state(params, sk, states);

    secret_key->oqs_save_updated_sk_key(secret_key);

    secret_key->release_key(secret_key);

    return 0;
}
