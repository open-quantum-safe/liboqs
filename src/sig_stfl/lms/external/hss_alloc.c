// SPDX-License-Identifier: MIT
/*
 * This is the code which allocates a working key (and initializes the fields
 * that are independent of the key)
 */
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "hss.h"
#include "hss_internal.h"
#include "lm_common.h"

#define MALLOC_OVERHEAD  8   /* Our simplistic model about the overhead */
                             /* that malloc takes up is that it adds 8 */
                             /* bytes to any request we make.  This isn't */
                             /* precise (especially if we consider external */
                             /* fragmentation), it's just a guideline */

/*
 * Function to estimate the amount of memory we'd use at a particular level,
 * if we went with a particular subtree size
 * - i is which tree in the scheme we're talking about; 0 is the root tree
 *   We have this because we allocate less for the root tree
 * - subtree_size is the size of the subtrees we're considering
 * - total_length is the size of the trees
 * - size_hash is the length of the hash output (always 32 currently)
 * - if psubtree_levels is non-NULL, we'll return the number of subtree levels
 *   here
 * - if pstack_total is non-NULL, we'll return the bytes of stack space needed
 *   by the subtrees of this level here
 * The value returned is the amount of space used by the merkle
 * level structures, the subtree structures, plus the additional stack
 * space required
 */
static size_t compute_level_memory_usage(int i, unsigned subtree_size,
                       unsigned total_height, unsigned size_hash,
                       unsigned *psubtree_levels,
                       size_t *pstack_total) {
    /* Compute the number of subtree levels we'd have */
    unsigned subtree_levels = (total_height + subtree_size - 1) / subtree_size;
    unsigned top_subtree_size = total_height - (subtree_levels-1)*subtree_size;
        /* The top level tree has no next subtrees */
    int have_next_subtree = (i == 0) ? 0 : 1;
    size_t stack_total = 0;

    /* Compute the memory this would use */
    size_t memory_used = sizeof(struct merkle_level) + MALLOC_OVERHEAD;
    unsigned j;
    for (j=0; j<subtree_levels-1; j++) {
            /* # of subtrees at this level */
        int num_subtrees = 2 + have_next_subtree;
            /* the size of each subtree */
        size_t size_subtree = sizeof(struct subtree) + MALLOC_OVERHEAD +
                               size_hash * (((size_t)2<<subtree_size)-1);
        size_t size_stack = (num_subtrees-1) * size_hash * j * subtree_size;
        memory_used += num_subtrees * size_subtree + size_stack;
        stack_total += size_stack;
    }

    /* The top level subtree is a bit different; it has no building subtree */
    int num_subtrees = 1 + have_next_subtree;  /* No BUILDING subtrees */
    size_t size_subtree = sizeof(struct subtree) + MALLOC_OVERHEAD +
                               size_hash * (((size_t)2<<top_subtree_size)-1);
    size_t size_stack = (num_subtrees-1) * size_hash * j * subtree_size;
    memory_used += num_subtrees * size_subtree + size_stack;
    stack_total += size_stack;

    if (psubtree_levels) *psubtree_levels = subtree_levels;
    if (pstack_total) *pstack_total = stack_total;
    return memory_used;
}

/*
 * As we generate signatures, we walk through the Merkle tree.  As we walk
 * through the tree at a particular level, it will occassionally send an
 * update to its parent tree (that is, give an opportunity for the parent tree
 * to do a walk itself).  This function returns, given the number of levels
 * and subtree height within a Merkle tree, the number of updates this level
 * would give its parent
 *
 * It does come up in some corner cases (such as a bottom level tree with H=5
 * and subtree height 3 (8 updates sent to the paremt); and a top level tree
 * with H=20 or 25).
 */
static merkle_index_t compute_updates_generated( unsigned height,
                                                 unsigned subtree ) {
        /* Special case: if the tree consists of 1 subtree, we'll send an */
        /* update at each signature */
    if (height <= subtree) return (merkle_index_t)1<<height;

    int num_sublevels = (height + subtree - 1) / subtree;

        /* We'll send an update for every node covered by the next-to-top */
        /* subtree */
    return (merkle_index_t)1 << ((num_sublevels-1) * subtree);
}

/*
 * This will compute the number of updates this tree will need from its child
 */
static merkle_index_t compute_updates_required( unsigned height,
                                                unsigned subtree ) {
    int num_sublevels = (height + subtree - 1) / subtree;

    /*
     * This tree will need this many updates:
     * num_sublevels-1 for the BUILDING subtrees (the topmost subtree doesn't
     *       get one)
     * 1 for the NEXT tree
     * 1 for our parent
     */
    return num_sublevels + 1;
}

/*
 * Given a tree height (and whether it's the top level, and the hash size),
 * this returns the subtree size that yields the least amount of memory.
 * This is used for the non-bottom trees (as there really isn't any point in
 * trying to using more memory to make them faster)
 * i = 0 is when tree level we're talking about; 0 for the root tree; nonzero
 * of the others.  n is the size of the hash (which, for the current parameter
 * sets, is always 32).  We included these because they do influence the
 * memory allocation; however, with the current parameter sets, these don't
 * actually change the answer
 */
int hss_smallest_subtree_size(int tree_height, int i, int n) {
    /* Experements have found that subtree height 2 consistently
     * uses the least memory
     * Actually, in the corner case of H=15,25, i!=0, it turned out
     * that height 3 used a bit less, however not enough to call it
     * out
     * These expirements were done with n=32; different n would likely
     * give other results
     */
    LMS_UNUSED(tree_height);
    LMS_UNUSED(i);
    LMS_UNUSED(n);
#if MIN_SUBTREE > 2
#error We assume that a subtree of size 2 is allowed
#endif
    return 2;
}

/*
 * This allocates a working key for a particular parameter set, and sets up
 * the data fields that are key independent; it doesn't set anything that
 * does depend on the key.  memory_target is used to guide time/memory
 * trade-offs; it's the target memory budget that we try to stay below if
 * possible
 */
struct hss_working_key *allocate_working_key(
    unsigned levels,
    const param_set_t *lm_type, const param_set_t *lm_ots_type,
    size_t memory_target,
    struct hss_extra_info *info) {
    struct hss_extra_info temp_info = { 0 };
    if (!info) info = &temp_info;

    if (levels < MIN_HSS_LEVELS || levels > MAX_HSS_LEVELS) {
        info->error_code = hss_error_bad_param_set;
        return 0;
    }

    /* Assign the memory target to a *signed* variable; signed so that it */
    /* can take on negative values meaningfully (to account for cases where */
    /* we are "overbudget") */
    signed long mem_target;
    if (memory_target > LONG_MAX) {
        mem_target = LONG_MAX;
    } else {
        mem_target = (signed long)memory_target;
    }
#if 0
signed long initial_mem_target = mem_target; /* DEBUG HACK */
#endif

    struct hss_working_key *w = OQS_MEM_malloc( sizeof *w );
    if (!w) {
        info->error_code = hss_error_out_of_memory;
        return NULL;
    }
    mem_target -= (signed long)sizeof(*w) + MALLOC_OVERHEAD;
    unsigned i;
    w->levels = levels;
    w->status = hss_error_key_uninitialized; /* Not usable until we see a */
                                             /* private key */
    w->autoreserve = 0;

    /* Initialize all the allocated data structures to NULL */
    /* We do this up front so that if we hit an error in the middle, we can */
    /* just free everything */
    for (i=0; i<MAX_HSS_LEVELS-1; i++) {
        w->signed_pk[i] = NULL;
    }
    for (i=0; i<MAX_HSS_LEVELS; i++) {
        w->tree[i] = NULL;
    }
    w->stack = NULL;

    /* Allocate all the memory for the level signatures */
    size_t signature_len = 4;   /* At the same time, ocmpute the sig length */
    for (i=0; i < levels; i++) {
        w->siglen[i] = lm_get_signature_len( lm_type[i], lm_ots_type[i] );
        signature_len += w->siglen[i];
            /* Size of this level's Merkle public key */
        size_t pklen = lm_get_public_key_len(lm_type[i]);
        if (i != 0) signature_len += pklen;
        if (w->siglen[i] == 0) {
            hss_free_working_key(w);
            info->error_code = hss_error_bad_param_set;
            return 0;
        }
            /* We don't need a allocate a signature for the topmost */
        if (i == 0) continue;

        w->signed_pk_len[i] = w->siglen[i-1] + pklen;

        w->signed_pk[i] = OQS_MEM_malloc( w->signed_pk_len[i] );
        if (!w->signed_pk[i]) {
            hss_free_working_key(w);
            info->error_code = hss_error_out_of_memory;
            return 0;
        }
        mem_target -= (signed long)w->signed_pk_len[i] + MALLOC_OVERHEAD;
    }
    w->signature_len = signature_len;

    /* Also account for the overhead for the stack allocation (the memory */
    /* used by the stack will be accounted as a part of the tree level size */
     mem_target -= (signed long)MALLOC_OVERHEAD;

    /*
     * Plot out how many subtree sizes we have at each level.  We start by
     * computing how much memory we'd use if we minimize each level
     */
    unsigned subtree_size[MAX_HSS_LEVELS];
    unsigned subtree_levels[MAX_HSS_LEVELS];
    unsigned level_hash[MAX_HSS_LEVELS];
    unsigned level_height[MAX_HSS_LEVELS];
    unsigned hash_size[MAX_HSS_LEVELS];
    unsigned total_height = 0;

    /* Parse the parameter sets */
    for (i=0; i<levels; i++) {

        if (!lm_look_up_parameter_set(lm_type[i], &level_hash[i],
                              &hash_size[i], &level_height[i])) {
            hss_free_working_key(w);
            info->error_code = hss_error_bad_param_set;
            return 0;
        }

        total_height += level_height[i];  /* Also track the number of */
                      /* signatures we can generate with this parm set */
    }

    /*
     * Select which subtree sizes that is faster, and fit within the memory
     * we've been given. For the nonbottom levels, we always use what's the
     * smallest for that particular tree height; there's no point in wasting
     * extra memory to make them faster (in that each one can be done during
     * the time the bottom level BUILDING subtrees don't need updating).
     */
    size_t stack_usage = 0;
    for (i=0; i<levels-1; i++) {
        int subtree =  hss_smallest_subtree_size(level_height[i], i,
                                                 hash_size[i]);

        /*
         * Double check to make sure that this subtree size will allow
         * enough updates to perculate to the tree above it.  I'm pretty
         * sure that, with the current logic and parameter sets, we'll
         * never actually need to adjust the subtree size; this is here
         * as a double check (at the very least, in case someone adds a
         * new parameter set)
         */

            /* Updates we will to send to the parent */
        merkle_index_t updates_generated = compute_updates_generated(
                                                         level_height[i],
                                                         subtree );

            /* Updates that the parent will need */
        merkle_index_t updates_required;
        if (i == 0)
            updates_required = 0;   /* no parent; no updates needed */
        else {
            updates_required = compute_updates_required( level_height[i-1],
                                                         subtree_size[i-1] );
        }

        if (updates_generated < updates_required) {
            /*
             * The current settings don't have the parent receiving enough
             * updates; if we generate enough signatures, we'll end up using
             * a partially updated parent (and that's not good eats).  Adjust
             * things to avoid the possibility
             */
            /* A subtree of 5 is always sufficient (as it always generates */
            /* at least 32 updates) */
            subtree = 5;
        }
        subtree_size[i] = subtree;
        size_t stack_used;
        size_t mem = compute_level_memory_usage(i, subtree,
                       level_height[i], hash_size[i], &subtree_levels[i],
                        &stack_used );

        mem_target -= (signed long)mem;
        stack_usage += stack_used;
    }

    /*
     * For the bottom-most level, look for the size that is the fastest (fewest
     * number of sublevels), and fits within the memory we've been given
     * Rules of preference:
     * - We prefer something that fits within our budget over something that
     *   doesn't
     * - For things that don't fit our budget, we'll take the samllest
     * - For things that do fit out budget, we'll take the fastest (and the
     *   smallest if they're equally fast; no need to waste memory)
     */
    i = levels - 1;
    enum {
        nothing_yet,              /* We haven't found anything yet */
        found_overbudget,         /* We found something, but it used more */
                                  /* memory than the budget we were given */
        found_plenty_memory,      /* We found something that fits within */
                                  /* out budget */
    } search_status = nothing_yet;
    unsigned long best_mem = 0;
    unsigned long best_levels = 0;
    unsigned best_j = 0;
    size_t best_stack_used = 0;
    unsigned j;
    for (j = MIN_SUBTREE; j <= level_height[i]; j++) {
        if (levels == 1) {
            /* If the tree consists of a single level, then we don't need to */
            /* make sure that we have enough time to update the higher trees */
            /* (as there ain't none).  However, to make sure that we work */
            /* with aux data, make sure that the number of levels is a */
            /* multiple of the default subtree size.  We could make the aux */
            /* data logic smarter to handle this corner case, but, well, */
            /* it's a corner csae */
            int only_subtree_size = hss_smallest_subtree_size(level_height[i],
                                                         0, hash_size[i]);
            if (j % only_subtree_size != 0) continue;
        } else {
            /* Make sure this size will send enough updates to our parent */
            merkle_index_t updates_generated = compute_updates_generated(
                                             level_height[i], j );

            merkle_index_t updates_required = compute_updates_required(
                                   level_height[i-1], subtree_size[i-1] );
            if (updates_generated < updates_required) {
                 /* This subtree size won't work */
                 continue;
            }
        }

            /* Amount of memory this would use */
            /* This is a signed type so that the comparison works as */
            /* expected if mem_target is negative */
        size_t stack_used;
        signed long mem = (unsigned long)compute_level_memory_usage(i, j,
                       level_height[i], hash_size[i], &subtree_levels[i],
                       &stack_used );
            /* # of sublevels this would have */
        unsigned sub_levels = (level_height[i] + j - 1) / j;

            /*
             * Now, from a worse-case standpoint, one sublevel isn't any
             * better than two (for a couple of the updates, we'll need to
             * update the parent).  However, from an average time, one
             * sublevel is much better.  So, we allow one sublevel to be
             * considered better than two (even though everywhere else, we
             * consider worse-case time).
             */

        if (mem > mem_target) {
            /* This would use more memory than we'd like; accept it if */
            /* either we have no solution, or it uses less memory than what */
            /* we've seen */
            if (search_status != nothing_yet && mem > (signed long)best_mem) continue;

            /* This solution is the best so far (however, it doesn't fit) */
            search_status = found_overbudget;
        } else {
            /* This is within our budget; accept it if we haven't seen a */
            /* previous solution within our budget, or this uses fewer */
            /* levels than the previous solution */
            if (search_status == found_plenty_memory) {
                if (sub_levels > best_levels) {
                    /* We've already seen a faster solution */
                    continue;
                }
                if (sub_levels == best_levels && mem > (signed long)best_mem) {
                    /* We've already seen an equally fast solution that */
                    /* uses less memory */
                    continue;
                }
            }

            /* This solution is the best so far (and it fits) */
            search_status = found_plenty_memory;
        }
        /* This is the best option so far; record it */
        best_j = j;
        best_mem = mem;
        best_levels = sub_levels;
        best_stack_used = stack_used;
    }

    if (search_status == nothing_yet) {
        /* This can't really happen */
        hss_free_working_key(w);
        info->error_code = hss_error_internal;
        return 0;
    }
#if 0
printf( "Allocation = %ld\n", initial_mem_target - mem_target + best_mem ); /* DEBUG HACK */
#endif

    subtree_size[i] = best_j;
    subtree_levels[i] = (level_height[i] + best_j - 1) / best_j;
    stack_usage += best_stack_used;

    unsigned char *stack;
    if (stack_usage == 0) {
        stack = NULL;   /* Hey!  No stack required */
                        /* Avoid the malloc, as malloc(0) is allowed to fail */
    } else {
        stack = OQS_MEM_malloc(stack_usage);
        if (!stack) {
            hss_free_working_key(w);
            info->error_code = hss_error_out_of_memory;
            return 0;
        }
    }
    w->stack = stack;
    size_t stack_index = 0;

    /*
     * Ok, we've figured out the sizes for everything; now do the actual
     * allocations
     */
    for (i = 0; i<levels; i++) {
        struct merkle_level *tree = OQS_MEM_malloc( sizeof *tree );
        if (!tree) {
            hss_free_working_key(w);
            info->error_code = hss_error_out_of_memory;
            return 0;
        }
        unsigned h0 = level_height[i];
        tree->level = h0;
        tree->h = level_hash[i];
        tree->hash_size = hash_size[i];
        tree->lm_type = lm_type[i];
        tree->lm_ots_type = lm_ots_type[i];
        /* We'll initialize current_index from the private key */
        tree->max_index = (1L << tree->level) - 1;
        tree->sublevels = subtree_levels[i];
        tree->subtree_size = subtree_size[i];
        unsigned top_subtree_size = h0 - (subtree_levels[i]-1)*subtree_size[i];
        tree->top_subtree_size = top_subtree_size;

        unsigned k;
        for (j=0; j<MAX_SUBLEVELS; j++)
            for (k=0; k<NUM_SUBTREE; k++)
                tree->subtree[j][k] = NULL;
        w->tree[i] = tree;

        unsigned subtree_level = 0;
        unsigned levels_below = h0;
        for (j=0; j<subtree_levels[i]; j++) {
            /* The height of the subtrees at this level  */
            unsigned height = (j == 0) ? top_subtree_size : subtree_size[i];
            levels_below -= height;

            for (k=0; k<NUM_SUBTREE; k++) {
                /* The root subtree doesn't get a 'building subtree' */
                if (j == 0 && k == BUILDING_TREE) continue;
                /* The subtrees in the topmost tree don't get a */
                /* 'next subtree' */
                if (k == NEXT_TREE && i == 0) continue;

                struct subtree *s = OQS_MEM_malloc( sizeof *s + hash_size[i] *
                                               (((size_t)2<<height)-1));
                if (!s) {
                    hss_free_working_key(w);
                    info->error_code = hss_error_out_of_memory;
                    return 0;
                }

                s->level = subtree_level;
                s->levels_below = levels_below;
                tree->subtree[j][k] = s;
                if (k == ACTIVE_TREE) {
                    /* Active trees don't need no stack */
                    s->stack = NULL;
                } else if (levels_below == 0) {
                    /* Bottom level subtrees don't need no stack */
                    s->stack = NULL;
                } else {
                    s->stack = &stack[stack_index];
                    stack_index += hash_size[i] * levels_below;
                }
            }

            subtree_level += height;
        }
    }

/* SANITY CHECK */
    if (stack_index != stack_usage) {
        hss_free_working_key(w);
        info->error_code = hss_error_internal;
        return 0;
    }
/* SANITY CHECK */

    /* Compute the max number of signatures we can generate */
    if (total_height > 64) total_height = 64; /* (bounded by 2**64) */
    w->max_count = ((sequence_t)2 << (total_height-1)) - 1; /* height-1 so */
            /* we don't try to shift by 64, and hit undefined behavior */

        /* We use the count 0xffff..ffff to signify 'we've used up all our */
        /* signatures'.  Make sure that is above max_count, even for */
        /* parameter sets that can literally generate 2**64 signatures (by */
        /* letting them generate only 2**64-1) */
    if (total_height == 64) w->max_count--;

    return w;
}

void hss_free_working_key(struct hss_working_key *w) {
    int i;
    if (!w) return;
    for (i=0; i<MAX_HSS_LEVELS; i++) {
        struct merkle_level *tree = w->tree[i];
        if (tree) {
            unsigned j, k;
            for (j=0; j<MAX_SUBLEVELS; j++)
                for (k=0; k<3; k++)
                    OQS_MEM_insecure_free(tree->subtree[j][k]);
            hss_zeroize( tree, sizeof *tree ); /* We have seeds here */
        }
        OQS_MEM_insecure_free(tree);
    }
    for (i=0; i<MAX_HSS_LEVELS-1; i++) {
        OQS_MEM_insecure_free(w->signed_pk[i]);
    }
    OQS_MEM_insecure_free(w->stack);
    hss_zeroize( w, sizeof *w ); /* We have secret information here */
    OQS_MEM_insecure_free(w);
}
