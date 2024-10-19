// SPDX-License-Identifier: MIT
/*
 * This is the routine that generates the ephemeral ("working") key from the
 * short private value.  It builds all the various current, building and
 * next subtrees for the various levels (to at least the extent required
 * for the current count within the key).
 *
 * The code is made considerably more complex because we try to take
 * advantage of parallelism.  To do this, we explicitly list the parts
 * of the subtrees we need to build (which is most of the computation), and
 * have different worker threads build the various parts,
 *
 * However, it turns out that this is sometimes insufficient; sometimes,
 * the work consists of one or two expensive nodes (perhaps the top level
 * subtree), and a lot of comparatively cheap ones; in this case, we'd have
 * most of our threads go through the cheap ones quickly, and have one or
 * two threads working on the expensive one, and everyone will end up waiting
 * for that.  To mitigate that, we attempt to subdivide the most expensive
 * requests; instead of having a single thread computing the expensive node,
 * we may issue four or eight threads to compute the nodes two or three
 * levels below (and have the main thread do the final computation when
 * all the threads are completed).
 *
 * This works out pretty good; however man does add complexity :-(
 */
#include <string.h>
#include <limits.h>
#include "hss.h"
#include "hss_internal.h"
#include "hss_aux.h"
#include "hash.h"
#include "hss_thread.h"
#include "hss_reserve.h"
#include "lm_ots_common.h"
#include "endian.h"

#define DO_FLOATING_POINT 1  /* If clear, we avoid floating point operations */
    /* You can turn this off for two reasons: */
    /* - Your platform doesn't implement floating point */
    /* - Your platform is single threaded (we use floating point to figure */
    /*   out how to split up tasks between threads; if the same thread */
    /*   will do all the work, dividing it cleverly doesn't buy anything */
    /*   (and that's a quite a bit of code that gets eliminated) */
    /* On the other hand, if you are threaded, you'd really want this if */
    /* at all possible; without this, one thread ends up doing the bulk of */
    /* the work, and so we end up going not that much faster than single */
    /* threaded mode */

/*
 * This routine assumes that we have filled in the bottom node_count nodes of
 * the subtree; it tries to compute as many internal nodes as possible
 */
static void fill_subtree(const struct merkle_level *tree,
                         struct subtree *subtree,
                         merkle_index_t node_count,
                         const unsigned char *I) {
    if (node_count <= 1) return; /* If we can't compute any more nodes, */
                                 /* don't bother trying */
    unsigned h_subtree = (subtree->level == 0) ? tree->top_subtree_size :
                                                 tree->subtree_size;

        /* Index into the node array where we're starting */
    merkle_index_t lower_index = ((merkle_index_t)1 << h_subtree) - 1;

    unsigned hash_size = tree->hash_size;

        /* The node identier (initially of the bottom left node of the */
        /* subtree */
    merkle_index_t node_id = (((merkle_index_t)1 << tree->level) +
                                                       subtree->left_leaf)
                                    >> subtree->levels_below;

    /* Fill in as many levels of internal nodes as possible */
    int sublevel;
    for (sublevel = h_subtree-1; sublevel >= 0; sublevel--) {
        node_count >>= 1;
        if (node_count == 0) break;   /* Can't do any more */
        merkle_index_t prev_lower_index = lower_index;
        lower_index >>= 1;
        node_id >>= 1;

        merkle_index_t i;
        for (i=0; i<node_count; i++) {
            hss_combine_internal_nodes(
                        &subtree->nodes[ hash_size *(lower_index + i)],
                        &subtree->nodes[ hash_size *(prev_lower_index + 2*i)],
                        &subtree->nodes[ hash_size *(prev_lower_index + 2*i+1)],
                        tree->h, I, hash_size,
                        node_id + i);
        }
    }
}

/*
 * This routine takes the 2**num_level hashes, and computes up num_level's,
 * returning the value of the top node.  This is sort of like fill_tree,
 * except that it returns only the top node, not the intermediate ones
 * One warning: this does modify the passed value of hashes; our current
 * caller doesn't care about that.
 */
static void hash_subtree( unsigned char *dest,
                          unsigned char *hashes,
                          unsigned num_level, merkle_index_t node_index,
                          unsigned hash_size,
                          int h, const unsigned char *I) {

    /* Combine the nodes to form the tree, until we get to the two top nodes */
    /* This will overwrite the hashes array; that's OK, because we don't */
    /* need those anymore */
    for (; num_level > 1; num_level--) {
        unsigned i;
        merkle_index_t this_level_node_index = node_index << (num_level-1);
        for (i = 0; i < ((unsigned)1<<(num_level-1)); i++) {
            hss_combine_internal_nodes(
                        &hashes[ hash_size * i ],
                        &hashes[ hash_size * (2*i) ],
                        &hashes[ hash_size * (2*i + 1) ],
                        h, I, hash_size,
                        this_level_node_index + i);
        }
    }

    /* Combine the top two nodes to form our actual target */
    hss_combine_internal_nodes(
                        dest,
                        &hashes[ 0 ],
                        &hashes[ hash_size ],
                        h, I, hash_size,
                        node_index);
}

#if DO_FLOATING_POINT
/*
 * This structure is a note reminding us that we've decided to split this
 * init_order into several requests, which can be run on independent threads
 */
struct sub_order {
    unsigned num_hashes;         /* The number of hashes this suborder is */
                                 /* split up into */
    unsigned level;              /* Levels deep into the tree we go */
    merkle_index_t node_num_first_target; /* The node number of the left */
                                 /* most hash that we're standing in for */
    unsigned char h[1];          /* The hashes go here; we'll malloc */
                                 /* enough space to let them fit */
};
#endif

/*
 * This is an internal request to compute the bottom N nodes (starting from the
 * left) of a subtree (and to contruct the internal nodes that based solely on
 * those N leaf nodes)
 */
struct init_order {
    const struct merkle_level *tree;
    struct subtree *subtree;
    merkle_index_t count_nodes;   /* # of bottom level nodes we need to */
                                  /* generate */
    const unsigned char *prev_node; /* For nonbottom subtrees, sometimes one */
                                  /* of the nodes is the root of the */
                                  /* next level subtree that we compute in */
                                  /* its entirety. If so, this is a pointer */
                                  /* to where we will find the precomputed */
                                  /* value.  This allows us to avoid */
                                  /* computing that specific node */
    merkle_index_t prev_index;    /* This is the index of the */
                                  /* precomputed node, where 0 is the */
                                  /* leftmost bottom node of this subtree */
    char next_tree;               /* If clear, we do this on the current */
                                  /* tree level (seed, I values); if set, */
                                  /* we do this on the next */
    char already_computed_lower;  /* If set, we've already computed the */
                                  /* lower nodes (and all we need to do is */
                                  /* fill the upper); no need to ask the */
                                  /* threads do do anything */
                                  /* We may still need to build the */
                                  /* interiors of the subtrees, of course */
#if DO_FLOATING_POINT
    float cost;                   /* Approximate number of hash compression */
                                  /* operations per node */
    struct sub_order *sub;        /* If non-NULL, this gives details on how */
                                  /* we want to subdivide the order between */
                                  /* different threads */
#endif
};

#if DO_FLOATING_POINT
    /* This comparison function sorts the most expensive orders first */
static int compare_order_by_cost(const void *a, const void *b) {
    const struct init_order *p = a;
    const struct init_order *q = b;

    if (p->cost > q->cost) return -1;
    if (p->cost < q->cost) return  1;

    return 0;
}
#else
    /* This comparison function sorts the higher level subtrees first */
static int compare_order_by_subtree_level(const void *a, const void *b) {
    const struct init_order *p = a;
    unsigned p_subtree = p->subtree->level;
    const struct init_order *q = b;
    unsigned q_subtree = q->subtree->level;

    if (p_subtree < q_subtree) return -1;
    if (p_subtree > q_subtree) return  1;

    return 0;
}
#endif

#if DO_FLOATING_POINT
static float estimate_total_cost(struct init_order *order,
                                 unsigned count_order);

/*
 * This is a simple minded log function, returning an int.  Yes, using the
 * built-in log() function would be easier, however I don't want to pull in
 * the -lm library just for this
 */
static unsigned my_log2(float f) {
#define MAX_LOG 10
    unsigned n;
    for (n=1; f > 2 && n < MAX_LOG; n++)
        f /= 2;
    return n;
}
#endif

/*
 * This is the point of this entire file.
 *
 * It fills in an already allocated working key, based on the private key
 */
bool hss_generate_working_key(
    bool (*read_private_key)(unsigned char *private_key,
            size_t len_private_key, void *context),
        void *context,
    const unsigned char *aux_data, size_t len_aux_data,  /* Optional */
    struct hss_working_key *w,
    struct hss_extra_info *info) {
    struct hss_extra_info temp_info = { 0 };
    if (!info) info = &temp_info;

    if (!w) {
        info->error_code = hss_error_got_null;
        return false;
    }
    w->status = hss_error_key_uninitialized; /* In case we detect an */
                                             /* error midway */

    if (!read_private_key && !context) {
        info->error_code = hss_error_no_private_buffer;
        return false;
    }

    /* Read the private key */
    unsigned char private_key[ PRIVATE_KEY_LEN ];
    if (read_private_key) {
        if (!read_private_key( private_key, PRIVATE_KEY_LEN, context)) {
            info->error_code = hss_error_private_key_read_failed;
            goto failed;
        }
    } else {
        memcpy( private_key, context, PRIVATE_KEY_LEN );
    }

    /*
     * Make sure that the private key and the allocated working key are
     * compatible; that the working_key was initialized with the same
     * parameter set
     */
    {
        if (w->levels > MAX_HSS_LEVELS) {
            info->error_code = hss_error_internal;
            goto failed;
        }
        unsigned char compressed[PRIVATE_KEY_PARAM_SET_LEN];
        param_set_t lm_type[MAX_HSS_LEVELS], lm_ots_type[MAX_HSS_LEVELS];
        unsigned i;
        for (i=0; i<w->levels; i++) {
            lm_type[i] = w->tree[i]->lm_type;
            lm_ots_type[i] = w->tree[i]->lm_ots_type;
        }

        if (!hss_compress_param_set( compressed, w->levels,
                      lm_type, lm_ots_type,
                      sizeof compressed )) {
               /* We're passed an unsupported param set */
            info->error_code = hss_error_internal;
            goto failed;
        }
        if (0 != memcmp( private_key + PRIVATE_KEY_PARAM_SET, compressed,
                      PRIVATE_KEY_PARAM_SET_LEN )) {
               /* The working set was initiallized with a different parmset */
            info->error_code = hss_error_incompatible_param_set;
            goto failed;
        }
    }

    sequence_t current_count = get_bigendian(
                 private_key + PRIVATE_KEY_INDEX, PRIVATE_KEY_INDEX_LEN );
    if (current_count > w->max_count) {
        info->error_code = hss_error_private_key_expired;  /* Hey!  We */
        goto failed;             /* can't generate any more signatures */
    }
    hss_set_reserve_count(w, current_count);

    memcpy( w->private_key, private_key, PRIVATE_KEY_LEN );

    /* Initialize all the levels of the tree */

    /* Initialize the current count for each level (from the bottom-up) */
    sequence_t i;
    sequence_t count = current_count;
    for (i = w->levels; i >= 1 ; i--) {
        struct merkle_level *tree = w->tree[i-1];
        unsigned index = count & tree->max_index;
        count >>= tree->level;
        tree->current_index = index;
    }

    /* Initialize the I values */
    for (i = 0; i < w->levels; i++) {
        struct merkle_level *tree = w->tree[i];

        /* Initialize the I, I_next elements */
        if (i == 0) {
            /* The root seed, I value is derived from the secret key */
            hss_generate_root_seed_I_value( tree->seed, tree->I,
                                            private_key+PRIVATE_KEY_SEED );
            /* We don't use the I_next value */
        } else {
            /* The seed, I is derived from the parent's values */

            /* Where we are in the Merkle tree */
            struct merkle_level *parent = w->tree[i-1];
            merkle_index_t index = parent->current_index;

            hss_generate_child_seed_I_value( tree->seed, tree->I,
                                             parent->seed,  parent->I,
                                             index, parent->lm_type,
                                             parent->lm_ots_type );
            /* The next seed, I is derived from either the parent's I */
            /* or the parent's next value */
            if (index == tree->max_index) {
                hss_generate_child_seed_I_value( tree->seed_next, tree->I_next,
                                            parent->seed_next,  parent->I_next,
                                            0, parent->lm_type,
                                            parent->lm_ots_type);
            } else {
                hss_generate_child_seed_I_value( tree->seed_next, tree->I_next,
                                            parent->seed,  parent->I,
                                            index+1, parent->lm_type,
                                            parent->lm_ots_type);
            }
        }
    }

    /* Generate the expanded aux data structure (or NULL if we don't have a */
    /* viable aux structure */
    struct expanded_aux_data *expanded_aux, temp_aux;
    expanded_aux = hss_expand_aux_data( aux_data, len_aux_data, &temp_aux,
                                        w->tree[0]->hash_size, w );

    /*
     * Now, build all the subtrees within the tree
     *
     * We initialize the various data structures, and create a list of
     * the nodes on the bottom levels of the subtrees that need to be
     * initialized
     */
        /* There are enough structures in this array to handle the maximum */
        /* number of orders we'll ever see */
    struct init_order order[MAX_HSS_LEVELS * MAX_SUBLEVELS * NUM_SUBTREE];
    struct init_order *p_order = order;
    int count_order = 0;

    /* Step through the levels, and for each Merkle tree, compile a list of */
    /* the orders to initialize the bottoms of the subtrees that we'll need */
    for (i = w->levels; i >= 1 ; i--) {
        struct merkle_level *tree = w->tree[i-1];
        unsigned hash_size = tree->hash_size;
            /* The current count within this tree */
        merkle_index_t tree_count = tree->current_index;
            /* The index of the leaf we're on */
        merkle_index_t leaf_index = tree_count;

        /* Generate the active subtrees */
        int j;
        /*int bot_level_subtree = (int)tree->level;*/  /* The level of the bottom of */
                                              /* the subtree */
        unsigned char *active_prev_node = 0;
        unsigned char *next_prev_node = 0;
        for (j=tree->sublevels-1; j>=0; j--) {
                /* The height of this subtree */
            int h_subtree = (j == 0) ? tree->top_subtree_size :
                                       tree->subtree_size;

            /* Initialize the active tree */
            struct subtree *active = tree->subtree[j][ACTIVE_TREE];

                /* Total number of leaf nodes below this subtree */
            merkle_index_t size_subtree = (merkle_index_t)1 <<
                                             (h_subtree + active->levels_below);
            /* Fill in the leaf index that's on the left side of this subtree */
                /* This is the index of the leaf that we did when we first */
                /* entered the active subtree */
            merkle_index_t left_leaf = leaf_index & ~(size_subtree - 1);
                /* This is the number of leaves we've done in this subtree */
            merkle_index_t subtree_count = leaf_index - left_leaf;
                /* If we're not in the bottom tree, it's possible that the */
                /* update process will miss the very first update before we */
                /* need to sign.  To account for that, generate one more */
                /* node than what our current count would suggest */
            if (i != w->levels - 1) {
                subtree_count++;
            }
            active->current_index = 0;
            active->left_leaf = left_leaf;
            merkle_index_t num_bottom_nodes = (merkle_index_t)1 << h_subtree;

            /* Check if we have aux data at this level */
            int already_computed_lower = 0;
            if (i == 0) {
                merkle_index_t lower_index = num_bottom_nodes-1;
                merkle_index_t node_offset = active->left_leaf>>active->levels_below;
                if (hss_extract_aux_data(expanded_aux, active->level+h_subtree,
                             w, &active->nodes[ hash_size * lower_index ],
                             node_offset, num_bottom_nodes)) {
                    /* We do have it precomputed in our aux data */
                    already_computed_lower = 1;
                }
            }
            /* No aux data at this level; schedule the bottom row to be computed */
            /* Schedule the creation of the entire active tree */
            p_order->tree = tree;
            p_order->subtree = active;
            p_order->count_nodes = (merkle_index_t)1 << h_subtree; /* All */
                                                /* the nodes in this subtree */
            p_order->next_tree = 0;
                /* Mark the root we inherented from the subtree just below us */
            p_order->prev_node = already_computed_lower ? NULL : active_prev_node;
            p_order->prev_index = (tree->current_index >> active->levels_below) & (num_bottom_nodes-1);

            p_order->already_computed_lower = already_computed_lower;
            p_order++; count_order++;

            /* For the next subtree, here's where our root will be */
            active_prev_node = &active->nodes[0];

            /* And initialize the building tree, assuming there is one, and */
            /* assuming that the active subtree isn't at the right edge of */
            /* the Merkle tree */
            if (j > 0 && (leaf_index + size_subtree <= tree->max_index )) {
                struct subtree *building = tree->subtree[j][BUILDING_TREE];

                    /* The number of leaves that make up one bottom node */
                    /* of this subtree */
                merkle_index_t size_below_tree = (merkle_index_t)1 << building->levels_below;
                    /* We need to initialize the building tree current index */
                    /* to a value at least as large as subtree_count */
                    /* We'd prefer not to have to specificallly initialize */
                    /* the stack, and so we round up to the next place the */
                    /* stack is empty */
                merkle_index_t building_count =
                              (subtree_count + size_below_tree - 1) &
                                                    ~(size_below_tree - 1);
                    /* # of bottom level nodes we've building right now */
                merkle_index_t num_nodes = building_count >> building->levels_below;
                building->left_leaf = left_leaf + size_subtree;
                building->current_index = building_count;

                /* Check if this is already in the aux data */
                already_computed_lower = 0;
                if (i == 0) {
                    merkle_index_t lower_index = num_bottom_nodes-1;
                    merkle_index_t node_offset = building->left_leaf>>building->levels_below;
                    if (hss_extract_aux_data(expanded_aux, building->level+h_subtree,
                             w, &building->nodes[ hash_size * lower_index ],
                             node_offset, num_nodes)) {
                        /* We do have it precomputed in our aux data */
                        already_computed_lower = 1;
                    }
                }

                /* Schedule the creation of the subset of the building tree */
                p_order->tree = tree;
                p_order->subtree = building;
                    /* # of nodes to construct */
                p_order->count_nodes = num_nodes;
                p_order->next_tree = 0;
                    /* We generally can't use the prev_node optimization */
                p_order->prev_node = NULL;
                p_order->prev_index = 0;

                p_order->already_computed_lower = already_computed_lower;
                p_order++; count_order++;
            } else if (j > 0) {
                tree->subtree[j][BUILDING_TREE]->current_index = 0;
            }

            /* And the NEXT_TREE (which is always left-aligned) */
            if ((i-1) > 0) {
                struct subtree *next = tree->subtree[j][NEXT_TREE];
                next->left_leaf = 0;
                merkle_index_t leaf_size =
                                     (merkle_index_t)1 << next->levels_below;

                merkle_index_t next_index = tree_count;
                /* If we're not in the bottom tree, it's possible that the */
                /* update process will miss the very first update before we */
                /* need to sign.  To account for that, potetially generate */
                /* one more node than what our current count would suggest */
                if ((i-1) != w->levels - 1) {
                    next_index++;
                }

                /* Make next_index the # of leaves we'll need to process to */
                /* forward this NEXT subtree to this state */
                next_index = (next_index + leaf_size - 1)/leaf_size;

                    /* This is set if we have a previous subtree */
                merkle_index_t prev_subtree = (next->levels_below ? 1 : 0);
                merkle_index_t num_nodes;
                unsigned char *next_next_node = 0;

                /* If next_index == 1, then if we're on a nonbottom subtree */
                /* the previous subtree is still building (and so we */
                /* needn't do anything).  The exception is if we're on the */
                /* bottom level, then there is no subtree, and so we still */
                /* need to build the initial left leaf */
                if (next_index <= prev_subtree) {
                    /* We're not started on this subtree yet */
                    next->current_index = 0;
                    num_nodes = 0;
                } else if (next_index < num_bottom_nodes) {
                    /* We're in the middle of building this tree */
                    next->current_index = next_index << next->levels_below;
                    num_nodes = next_index;
                } else {
                    /* We've completed building this tree */
                        /* How we note "we've generated this entire subtree" */
                    next->current_index = MAX_SUBINDEX;
                    num_nodes = num_bottom_nodes;
                        /* We've generated this entire tree; allow it to */
                        /* be inhereited for the next one */
                    next_next_node = &next->nodes[0];
                }
                if (num_nodes > 0) {
                    /* Schedule the creation of these nodes */
                    p_order->tree = tree;
                    p_order->subtree = next;
                        /* # of nodes to construct */
                    p_order->count_nodes = num_nodes;
                    p_order->next_tree = 1;
                    p_order->prev_node = next_prev_node;
                    p_order->prev_index = 0;

                    p_order->already_computed_lower = 0;
                    p_order++; count_order++;
                }
                next_prev_node = next_next_node;
            }

//            bot_level_subtree -= h_subtree;
            if (j == 0) break; //This is a single level tree
         }
        if (i == 0) break; //This is a single level tree
    }

#if DO_FLOATING_POINT
    /* Fill in the cost estimates */
    for (i=0; i<(sequence_t)count_order; i++) {
        p_order = &order[i];

        /*
         * While we're here, NULL out all the suborders; we'll fill them in
         * later if necessary
         */
        p_order->sub = 0;
        if (p_order->already_computed_lower) {
            /* If we pulled the data from the aux, no work required */
            p_order->cost = 0;
            continue;
        }
        unsigned winternitz = 8;
        unsigned p = 128;
        (void)lm_ots_look_up_parameter_set(p_order->tree->lm_ots_type, 0, 0,
                                           &winternitz, &p, 0);

        struct subtree *subtree = p_order->subtree;
        unsigned levels_below = subtree->levels_below;

        /*
         * Estimate the number of hashes that we'll need to compute to compute
         * one node; this is the number of leaf nodes times the number of
         * hashes used during a winternitz computation.  This ignores a few
         * other hashes, but gets the vast bulk of them
         */
        p_order->cost = (float)((merkle_index_t)1<<levels_below) *
                        (float)p *
                        (float)(1<<winternitz);
    }

    /*
     * We have a list of work items to be done.  It doesn't matter (for
     * correctness) what order we do them in; however we'd like to keep the
     * threads as busy as possible (an idle thread is wasted time).  So, what
     * we try is sort the list in decreasing work order; that makes it more
     * likely that all the threads will complete moderately close to the same
     * time.  Doing this optimally is (in the general case) an NP-hard
     * problem; this is a fairly decent heuristic.
     */
    qsort( order, count_order, sizeof *order, compare_order_by_cost );
#else
    /*
     * We have a list of work items to be done.  We don't need to sort the
     * objects into 'most costly first' order; however the prev_node logic
     * will assume that if a higher order subtree depends on a lower one,
     * the higher order subtree will appear first.  Make it so.
     */
    qsort( order, count_order, sizeof *order, compare_order_by_subtree_level );
#endif

#if DO_FLOATING_POINT
    /* Generate an estimate of the total cost */
    float est_total = estimate_total_cost( order, count_order );

    /* Estimate how much we should target each work item should take */
    unsigned num_tracks = 4 * hss_thread_num_tracks(info->num_threads);
    if (num_tracks == 0) num_tracks = 4;   /* Divide by 0; just say no */
    float est_max_per_work_item = est_total / num_tracks;

    /* Scan through the items, and see which ones should be subdivided */
    for (i=0; i<(sequence_t)count_order; i++) {
        p_order = &order[i];
        if (p_order->cost <= est_max_per_work_item) {
            break; /* Break because once we hit this point, the rest of the */
                   /* items will be cheaper */
        }

            /* Try to subdivide each item into subdiv pieces */
        unsigned subdiv = my_log2(p_order->cost / est_max_per_work_item);
        struct subtree *subtree = p_order->subtree;
            /* Make sure we don't try to subdivide lower than what the */
            /* Merkle tree structure allows */
        if (subdiv > subtree->levels_below) subdiv = subtree->levels_below;
        if (subdiv == 0) continue;
        merkle_index_t max_subdiv = (merkle_index_t)1 << subtree->levels_below;
        if (subdiv > max_subdiv) subdiv = max_subdiv;
        if (subdiv <= 1) continue;

        const struct merkle_level *tree = p_order->tree;
        size_t hash_len = tree->hash_size;
        merkle_index_t count_nodes = p_order->count_nodes;
        size_t total_hash = (hash_len * count_nodes) << subdiv;
        unsigned h_subtree = (subtree->level == 0) ? tree->top_subtree_size :
                                                     tree->subtree_size;
        struct sub_order *sub = OQS_MEM_malloc( sizeof *sub + total_hash );
        if (!sub) continue;  /* On malloc failure, don't bother trying */
                             /* to subdivide */

            /* Fill in the details of this suborder */
        sub->level = subdiv;
        sub->num_hashes = 1 << subdiv;
        sub->node_num_first_target =
                (subtree->left_leaf >> subtree->levels_below) +
                     ((merkle_index_t)1 << (h_subtree + subtree->level));
        p_order->sub = sub;
    }
#endif

    /* Now, generate all the nodes we've listed in parallel */
    struct thread_collection *col = hss_thread_init(info->num_threads);
    enum hss_error_code got_error = hss_error_none;

       /* We use this to decide the granularity of the requests we make */
#if DO_FLOATING_POINT
    unsigned core_target = 5 * hss_thread_num_tracks(info->num_threads);
    float prev_cost = 0;
#endif

    for (i=0; i<(sequence_t)count_order; i++) {
        p_order = &order[i];
        if (p_order->already_computed_lower) continue;  /* If it's already */
                                                  /* done, we needn't bother */
            /* If this work order is cheaper than what we've issued, allow */
            /* for a greater amount of consolidation */
#if DO_FLOATING_POINT
        if (prev_cost > 0) {
            if (p_order->cost <= 2 * prev_cost) {
                /* The cost per node has decreased by a factor of 2 (at */
                /* least); allow a single core to do more of the work */
                float ratio = prev_cost / p_order->cost;
                if (ratio > 1000) {
                    core_target = 1;
                } else {
                    core_target = (unsigned)(core_target / ratio);
                    if (core_target == 0) core_target = 1;
                }
                prev_cost = p_order->cost;
            }
        } else {
            prev_cost = p_order->cost;
        }
#endif

        const struct merkle_level *tree = p_order->tree;
        struct subtree *subtree = p_order->subtree;
        unsigned h_subtree = (subtree->level == 0) ? tree->top_subtree_size :
                                                     tree->subtree_size;
        merkle_index_t lower_index = ((merkle_index_t)1 << h_subtree) - 1;
        unsigned hash_size = tree->hash_size;
#if DO_FLOATING_POINT
        unsigned max_per_request = p_order->count_nodes / core_target;
        if (max_per_request == 0) max_per_request = 1;
#else
        unsigned max_per_request = UINT_MAX;
#endif

        /* If we're skipping a value, make sure we compute up to there */
        merkle_index_t right_side = p_order->count_nodes;
        if (p_order->prev_node && right_side > p_order->prev_index) {
            right_side = p_order->prev_index;
        }

        merkle_index_t n;
        struct intermed_tree_detail detail;

        detail.seed = (p_order->next_tree ? tree->seed_next : tree->seed);
        detail.lm_type = tree->lm_type;
        detail.lm_ots_type = tree->lm_ots_type;
        detail.h = tree->h;
        detail.tree_height = tree->level;
        detail.I = (p_order->next_tree ? tree->I_next : tree->I);
        detail.got_error = &got_error;

#if DO_FLOATING_POINT
        /* Check if we're actually doing a suborder */
        struct sub_order *sub = p_order->sub;
        if (sub) {
            /* Issue all the orders separately */
            unsigned hash_len = tree->hash_size;
            for (n = 0; n < p_order->count_nodes; n++ ) {
                if (n == right_side) continue;  /* Skip the omitted value */
                unsigned char *dest = &sub->h[ n * sub->num_hashes * hash_len ];
                merkle_index_t node_num = (sub->node_num_first_target+n) << sub->level;
                unsigned k;
                for (k=0; k < sub->num_hashes; k++) {
                    detail.dest = dest;
                    dest += hash_len;
                    detail.node_num = node_num;
                    node_num++;
                    detail.node_count = 1;

                    hss_thread_issue_work(col, hss_gen_intermediate_tree,
                                          &detail, sizeof detail );
                }
            }
            continue;
        }
#endif
        {
            /* We're not doing a suborder; issue the request in as large of */
            /* a chunk as we're allowed */
            for (n = 0; n < p_order->count_nodes; ) {
                merkle_index_t this_req = right_side - n;
                if (this_req > max_per_request) this_req = max_per_request;
                if (this_req == 0) {
                    /* We hit the value we're skipping; skip it, and go on to */
                    /* the real right side */
                    n++;
                    right_side = p_order->count_nodes;
                    continue;
                }

                /* Issue a work order for the next this_req elements */
                detail.dest = &subtree->nodes[ hash_size * (lower_index + n)];
                detail.node_num = (subtree->left_leaf >> subtree->levels_below) +
                     n + ((merkle_index_t)1 << (h_subtree + subtree->level));
                detail.node_count = this_req;

                hss_thread_issue_work(col, hss_gen_intermediate_tree,
                                      &detail, sizeof detail );

                n += this_req;
             }
         }
    }

    /* We've issued all the order; now wait until all the work is done */
    hss_thread_done(col);
    if (got_error != hss_error_none) {
            /* One of the worker threads detected an error */
#if DO_FLOATING_POINT
            /* Don't leak suborders on an intermediate error */
        for (i=0; i<(sequence_t)count_order; i++) {
            OQS_MEM_insecure_free( order[i].sub );
        }
#endif
        info->error_code = got_error;
        goto failed;
    }

#if DO_FLOATING_POINT
    /*
     * Now, if we did have suborders, recombine them into what was actually
     * wanted
     */
    for (i=0; i<(sequence_t)count_order; i++) {
        p_order = &order[i];
        struct sub_order *sub = p_order->sub;
        if (!sub) continue;   /* This order wasn't subdivided */

        const struct merkle_level *tree = p_order->tree;
        const unsigned char *I = (p_order->next_tree ? tree->I_next : tree->I);
        struct subtree *subtree = p_order->subtree;
        unsigned hash_size = tree->hash_size;
        unsigned h_subtree = (subtree->level == 0) ? tree->top_subtree_size :
                                                     tree->subtree_size;
        merkle_index_t lower_index = ((merkle_index_t)1 << h_subtree) - 1;

        merkle_index_t n;
        for (n = 0; n < p_order->count_nodes; n++ ) {
            if (p_order->prev_node && n == p_order->prev_index) continue;

            hash_subtree( &subtree->nodes[ hash_size * (lower_index + n)],
                          &sub->h[ hash_size * sub->num_hashes * n ],
                          sub->level, sub->node_num_first_target + n,
                          hash_size, tree->h, I);
        }

        OQS_MEM_insecure_free( sub );
        p_order->sub = 0;
    }
#endif

    /*
     * Now we have generated the lower level nodes of the subtrees; go back and
     * fill in the higher level nodes.
     * We do this in backwards order, so that we do the lower levels of the trees
     * first (as lower levels are cheaper, they'll be listed later in the
     * array; that's how we sorted, them, remember?).
     * That means if any subtrees inherit the root values of lower trees,
     * we compute those root values first
     */
    for (i=count_order; i>0; i--) {
        p_order = &order[i-1];
        const struct merkle_level *tree = p_order->tree;
        const unsigned char *I = (p_order->next_tree ? tree->I_next : tree->I);
        struct subtree *subtree = p_order->subtree;

        if (p_order->prev_node) {
            /* This subtree did have a bottom node that was the root node */
            /* of a lower subtree; fill it in */
            unsigned hash_size = tree->hash_size;
            unsigned h_subtree = (subtree->level == 0) ? tree->top_subtree_size :
                                                         tree->subtree_size;
            merkle_index_t lower_index = ((merkle_index_t)1 << h_subtree) - 1;

                /* Where in the subtree we place the previous root */
            unsigned set_index = (lower_index + p_order->prev_index) * hash_size;
            memcpy( &subtree->nodes[ set_index ], p_order->prev_node, hash_size );
        }

        /* Now, fill in all the internal nodes of the subtree */
        fill_subtree(tree, subtree, p_order->count_nodes, I);
    }

    /*
     * Hey; we've initialized all the subtrees (at least, as far as what
     * they'd be expected to be given the current count); hurray!
     */

    /*
     * Now, create all the signed public keys
     * Again, we could parallelize this; it's also fast enough not to be worth
     * the complexity
     */
    for (i = 1; i < w->levels; i++) {
        if (!hss_create_signed_public_key( w->signed_pk[i], w->siglen[i-1],
                                       w->tree[i], w->tree[i-1], w )) {
            info->error_code = hss_error_internal; /* Really shouldn't */
                                                   /* happen */
            goto failed;
        }
    }
    hss_zeroize( private_key, sizeof private_key );

    /*
     * And, we make each level as not needing an update from below (as we've
     * initialized them as already having the first update)
     */
    for (i = 0; i < w->levels - 1; i++) {
        w->tree[i]->update_count = UPDATE_DONE;
    }

    w->status = hss_error_none; /* This working key has been officially */
                                /* initialized, and now can be used */
    return true;

failed:
    hss_zeroize( private_key, sizeof private_key );
    return false;
}

#if DO_FLOATING_POINT
/*
 * This goes through the order, and estimates the total amount
 * This assumes that the highest cost element is listed first
 *
 * It returns the estimated number of hash compression operations total
 *
 * We use floating point because the number of hash compression functions can
 * vary a *lot*; floating point has great dynamic range.
 */
static float estimate_total_cost( struct init_order *order,
                                  unsigned count_order ) {
    if (count_order == 0) return 0;
    float total_cost = 0;

    unsigned i;

    for (i=0; i<count_order; i++) {
        unsigned long count = order[i].count_nodes;
        if (order[i].prev_node) count--;
        total_cost += (float)order[i].cost * count;
    }

    return total_cost;
}
#endif
