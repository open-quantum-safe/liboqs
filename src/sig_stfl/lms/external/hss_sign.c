// SPDX-License-Identifier: MIT
/*
 * This is an implementation of the HSS signature scheme from LMS
 * This is the part that actually generates the signature
 */
#include <stdlib.h>
#include <string.h>
#include "common_defs.h"
#include "hss.h"
#include "hash.h"
#include "endian.h"
#include "hss_internal.h"
#include "hss_aux.h"
#include "hss_thread.h"
#include "hss_reserve.h"
#include "lm_ots.h"
#include "lm_ots_common.h"
#include "hss_derive.h"

/*
 * This adds one leaf to the building and next subtree.
 */
enum subtree_build_status {
    subtree_got_error,      /* Oops, something broke */
    subtree_more_to_do,     /* Processed node, still more to do */
    subtree_did_last_node,  /* Processed last node */
    subtree_all_done        /* We're good */
};
static enum subtree_build_status subtree_add_next_node(
                     struct subtree *subtree,
                     struct merkle_level *tree,
                     int next_tree,
                     struct thread_collection *col) {
    unsigned subtree_size = (subtree->level>0 ? tree->subtree_size :
                                                tree->top_subtree_size);
    unsigned log_leafs = subtree_size + subtree->levels_below;
    merkle_index_t max_index = (merkle_index_t)1 << log_leafs;
         /* Check if there is anything more to do */
    if (subtree->current_index == max_index) return subtree_all_done;
    unsigned hash_size = tree->hash_size;
    unsigned char cur_val[MAX_HASH];

    /* Compute the leaf node */
    merkle_index_t i;
    unsigned ots_len = (unsigned int)lm_ots_get_public_key_len(tree->lm_ots_type);
    unsigned char pub_key[ LEAF_MAX_LEN ];
    const unsigned char *I = (next_tree ? tree->I_next : tree->I);
    memcpy( pub_key + LEAF_I, I, I_LEN );
    SET_D( pub_key + LEAF_D, D_LEAF );
    merkle_index_t r = subtree->left_leaf + subtree->current_index;
    merkle_index_t q = r | ((merkle_index_t)1 << tree->level);
    put_bigendian( pub_key + LEAF_R, q, 4);

    const unsigned char *seed = (next_tree ? tree->seed_next : tree->seed);
    struct seed_derive derive;
    if (!hss_seed_derive_init( &derive, tree->lm_type, tree->lm_ots_type,
                       I, seed )) return subtree_got_error;
    hss_seed_derive_set_q(&derive, r);
    if (!lm_ots_generate_public_key(tree->lm_ots_type, I,
                   r, &derive, pub_key + LEAF_PK, ots_len)) {
        hss_seed_derive_done(&derive);
        return subtree_got_error;
    }
    hss_seed_derive_done(&derive);

    /* Hash it to form the leaf node */
    union hash_context ctx;
    hss_hash_ctx( cur_val, tree->h, &ctx, pub_key, LEAF_LEN(hash_size));

        /* Where in the subtree we store the values */
    merkle_index_t subtree_index = subtree->current_index +
                                           ((merkle_index_t)1 << log_leafs);
    enum subtree_build_status status = subtree_more_to_do;

    /* Walk up the stack, and then up the tree */
    for (i=0;; i++) {
        if (i >= subtree->levels_below) {
            /* This node is within the subtree; save it */
            memcpy( &subtree->nodes[ (subtree_index-1) * hash_size ], cur_val, hash_size );
        }
        if (subtree_index == 1) { /* Hit the root */
            status = subtree_did_last_node;
            break;
        }
        if ((q & 1) == 0) break;   /* Hit a left node */
        q >>= 1;

        /* This is a right node; combine it with the left node */
        unsigned char *left_node;
        if (i >= subtree->levels_below) {
            /* The left node is in the tree */
            left_node = &subtree->nodes[ (subtree_index-2) * hash_size ];
        } else {
            /* The left node is on the stack */
            left_node = subtree->stack + (i * hash_size);
        }
        hss_combine_internal_nodes( cur_val,
                                left_node, cur_val,
                                tree->h, I, hash_size,
                                q);
        subtree_index >>= 1;
    }

    /* If we haven't got out of the stack, put the value there */
    if (i < subtree->levels_below) {
        if (col) hss_thread_before_write(col);
        memcpy( subtree->stack + (i * hash_size), cur_val, hash_size );
        if (col) hss_thread_after_write(col);
    }

    /* Ok, we've done another node */
    subtree->current_index += 1;

    return status;
}

/*
 * This steps the next tree by one.  We need to do this 2**tree->level times,
 * and then the next tree will be ready
 */
static int hss_step_next_tree (struct merkle_level *tree,
                               const struct hss_working_key *w,
                               struct thread_collection *col) {
    struct subtree *prev_subtree = 0;
    struct subtree *subtree = 0;
    int j;

    LMS_UNUSED(w);
    /* Search for the subtree to update */
    for (j = tree->sublevels-1; j>=0; j--) {
        subtree = tree->subtree[j][NEXT_TREE];
        if (subtree->current_index < MAX_SUBINDEX) break;
        prev_subtree = subtree;
   }
   unsigned height_subtree = (j == 0) ? tree->top_subtree_size :
                                        tree->subtree_size;
   if (j >= 0) {
       /* For subtrees other than the bottom one, we get the first */
       /* node 'for free' (as it's the root of the previous subtree */
       if (subtree->current_index == 0 && prev_subtree) {
           /* For the initial node of the subtree, reuse the root */
           /* of the previous one */
           unsigned hash_size = tree->hash_size;
           memcpy( &subtree->nodes[ hash_size * (((merkle_index_t)1<<height_subtree)-1) ],
                   &prev_subtree->nodes[ 0 ],
                   hash_size );
           subtree->current_index = ((merkle_index_t)1 << subtree->levels_below);
       }

       /* Add the next node */
       switch (subtree_add_next_node( subtree, tree, 1, col )) {
       case subtree_got_error: default: return 0; /* Huh? */
       case subtree_more_to_do:
            break;
       case subtree_did_last_node:
       case subtree_all_done:
            /* Mark this subtree as 'all processed' */
            subtree->current_index = MAX_SUBINDEX;
            break;
       }
    }

    return 1;
}

/*
 * Generate the next Merkle signature for a given level
 */
static int generate_merkle_signature(
                     unsigned char *signature, unsigned signature_len,
                     struct merkle_level *tree,
                     const struct hss_working_key *w,
                     const void *message, size_t message_len) {
    /* First off, write the index value */
    LMS_UNUSED(w);
    if (signature_len < 4) return 0;
    merkle_index_t current_index = tree->current_index;
    put_bigendian( signature, current_index, 4 );
    signature += 4; signature_len -= 4;

    /* Write the OTS signature */
    size_t ots_sig_size = lm_ots_get_signature_len( tree->lm_ots_type );
    if (ots_sig_size == 0 || ots_sig_size > signature_len) return 0;
    if (message == NULL) {
        /* Internal interface: if message = NULL, we're supposed to */
        /* generate everything *except* the OTS signature */
        OQS_MEM_cleanse( signature, ots_sig_size );
    } else {
        struct seed_derive derive;
        if (!hss_seed_derive_init( &derive,
                            tree->lm_type, tree->lm_ots_type,
                            tree->I, tree->seed )) return 0;
        hss_seed_derive_set_q(&derive, current_index);
        bool success = lm_ots_generate_signature( tree->lm_ots_type, tree->I,
                                    current_index, &derive,
                                    message, message_len, false,
                                    signature, ots_sig_size);
        hss_seed_derive_done(&derive);
        if (!success) return 0;
    }
    signature += ots_sig_size; signature_len -= (unsigned)ots_sig_size;

    /* Write the LM parameter set */
    if (signature_len < 4) return 0;
    put_bigendian( signature, tree->lm_type, 4 );
    signature += 4; signature_len -= 4;

    /* Now, write the authentication path */
    int i, j;
    merkle_index_t index = current_index;
    unsigned n = tree->hash_size;
    for (i = tree->sublevels-1; i>=0; i--) {
        int height = (i == 0) ? tree->top_subtree_size : tree->subtree_size;
        struct subtree *subtree = tree->subtree[i][ACTIVE_TREE];
        merkle_index_t subtree_index = (index &
                                            (((merkle_index_t)1 << height) - 1)) +
                                       ((merkle_index_t)1 << height);
        for (j = height-1; j>=0; j--) {
            if (signature_len < n) return 0;
            memcpy( signature, subtree->nodes + n * ((subtree_index^1) - 1), n );
            signature += n; signature_len -= n;
            subtree_index >>= 1;
        }
        index >>= height;
    }

    /* Mark that we've generated a signature */
    tree->current_index = current_index + 1;

    return 1;
}

/*
 * This signed the root of tree with the parent; it places both the signature
 * and the public key into signed_key
 */
bool hss_create_signed_public_key(unsigned char *signed_key,
                                    size_t len_signature,
                                    struct merkle_level *tree,
                                    struct merkle_level *parent,
                                    struct hss_working_key *w) {
    /* Where we place the public key */
    unsigned char *public_key = signed_key + len_signature;

    /* Place the public key there */
    put_bigendian( public_key + 0, tree->lm_type, 4 );
    put_bigendian( public_key + 4, tree->lm_ots_type, 4 );
    memcpy( public_key + 8, tree->I, I_LEN );
    unsigned hash_size = tree->hash_size;
        /* This is where the root hash is */
    memcpy( public_key + 8 + I_LEN,
                   tree->subtree[0][ACTIVE_TREE]->nodes,
                   hash_size );
    unsigned len_public_key = 8 + I_LEN + hash_size;

        /* Now, generate the signature */
    if (!generate_merkle_signature( signed_key, (unsigned)len_signature,
                         parent, w, public_key, len_public_key)) {
        return false;
    }

    parent->update_count = UPDATE_NEXT;  /* The parent has generated a */
                              /* signature; it's now eligible for another */
                              /* round of updates */

    return true;
}

struct gen_sig_detail {
    unsigned char *signature;
    size_t signature_len;
    const unsigned char *message;
    size_t message_len;
    struct hss_working_key *w;
    enum hss_error_code *got_error;
};
/* This does the actual signature generation */
/* It is (potentially) run within a thread */
static void do_gen_sig( const void *detail, struct thread_collection *col) {
    const struct gen_sig_detail *d = detail;
    size_t signature_len = d->signature_len;
    unsigned char *signature = d->signature;
    struct hss_working_key *w = d->w;
    unsigned levels = w->levels;

        /* The number of signed public keys */
    if (signature_len < 4) goto failed;
    put_bigendian( signature, levels - 1, 4 );
    signature += 4; signature_len -= 4;
        /* The signed public keys */
    unsigned i;
    for (i=1; i<levels; i++) {
            /* Note: we've already generated the signatures for the */
            /* nonbottom trees, and so their current count will already be */
            /* advanced */
        size_t len_signed_pk = w->signed_pk_len[i];
        if (signature_len < len_signed_pk) goto failed;
        memcpy( signature, w->signed_pk[i], len_signed_pk );
        signature += len_signed_pk; signature_len -= len_signed_pk;
    }
        /* And finally the signature of the actual message */
    if (signature_len < w->siglen[levels-1]) goto failed; /* Oops, not enough room */

    const unsigned char *message = d->message;
    size_t message_len = d->message_len;

    if (!generate_merkle_signature(signature, (unsigned)signature_len,
              w->tree[ levels-1 ], w, message, message_len)) {
        goto failed;
    }

    /* Success! */
    return;

failed:
    /* Report failure */
    hss_thread_before_write(col);
    *d->got_error = hss_error_internal;
    hss_thread_after_write(col);
}

struct step_next_detail {
    struct hss_working_key *w;
    struct merkle_level *tree;
    enum hss_error_code *got_error;
};
/* This steps the next tree */
/* It is (potentially) run within a thread */
static void do_step_next( const void *detail, struct thread_collection *col) {
    const struct step_next_detail *d = detail;
    struct hss_working_key *w = d->w;
    struct merkle_level *tree = d->tree;

    if (!hss_step_next_tree( tree, w, col )) {
        /* Report failure */
        hss_thread_before_write(col);
        *d->got_error = hss_error_internal;
        hss_thread_after_write(col);
    }
}

struct step_building_detail {
    struct merkle_level *tree;
    struct subtree *subtree;
    enum hss_error_code *got_error;
};
/* This steps the building tree */
/* It is (potentially) run within a thread */
static void do_step_building( const void *detail,
                                            struct thread_collection *col) {
    const struct step_building_detail *d = detail;
    struct merkle_level *tree = d->tree;
    struct subtree *subtree = d->subtree;

    switch (subtree_add_next_node( subtree, tree, 0, col )) {
    case subtree_got_error: default:
        /* Huh? Report failure */
        hss_thread_before_write(col);
        *d->got_error = hss_error_internal;
        hss_thread_after_write(col);
        break;
    case subtree_more_to_do:
    case subtree_did_last_node:
    case subtree_all_done:
         break;
    }
}

struct update_parent_detail {
    struct hss_working_key *w;
    enum hss_error_code *got_error;
};
/*
 * This gives an update to the parent (non-bottom Merkle trees)
 */
static void do_update_parent( const void *detail,
                                            struct thread_collection *col) {
    const struct update_parent_detail *d = detail;
    struct hss_working_key *w = d->w;
    unsigned levels = w->levels;
    unsigned current_level = levels - 2;  /* We start with the first */
                                          /* non-bottom level */
    for (;;) {
        struct merkle_level *tree = w->tree[current_level];
        switch (tree->update_count) {
        case UPDATE_DONE: return;   /* No more updates needed */
        case UPDATE_NEXT:           /* Our job is to update the next tree */
            tree->update_count = UPDATE_PARENT;
            if (current_level == 0) return; /* No next tree to update */
            if (!hss_step_next_tree( tree, w, col )) goto failed;
            return;
        case UPDATE_PARENT:         /* Our job is to update our parent */
            tree->update_count = UPDATE_BUILDING + 0;
            if (current_level == 0) return; /* No parent to update */
            current_level -= 1;
            continue;
        default: {
            /* Which building tree we need to update */
            unsigned level_to_update =
                              (tree->update_count - UPDATE_BUILDING) + 1;
            if (level_to_update >= tree->sublevels) {
                /* We've completed all the updates we need to do (until */
                /* the next time we need to sign something) */
                tree->update_count = UPDATE_DONE;
                return;
            }

            /* Next time, update the next BUILDING subtree */
            tree->update_count += 1;

            struct subtree *subtree =
                              tree->subtree[level_to_update][BUILDING_TREE];

            /* The number of leaves in this tree */
            merkle_index_t tree_leaves = (merkle_index_t)1 << tree->level;

            /* Check if we'd actually use the building tree */
            if (subtree->left_leaf >= tree_leaves) {
                    /* We'll never use it; don't bother updating it */
                return;
            }

            /* We'll use the BUILDING_TREE, actually add a node */
            switch (subtree_add_next_node( subtree, tree, 0, col )) {
            case subtree_got_error: default: goto failed; /* Huh? */
            case subtree_did_last_node:
            case subtree_all_done:
            case subtree_more_to_do:
                /* We're done everything we need to do for this step */
                return;
            }
        }
        }
    }

failed:
    /* Huh? Report failure */
    hss_thread_before_write(col);
    *d->got_error = hss_error_internal;
    hss_thread_after_write(col);
}

/*
 * Code to actually generate the signature
 */
bool hss_generate_signature(
    struct hss_working_key *w,
    bool (*update_private_key)(unsigned char *private_key,
            size_t len_private_key, void *context),
    void *context,
    const void *message, size_t message_len,
    unsigned char *signature, size_t signature_buf_len,
    struct hss_extra_info *info) {
    struct hss_extra_info temp_info = { 0 };
    if (!info) info = &temp_info;
    unsigned i;
    bool trash_private_key = false;

    info->last_signature = false;

    if (!w) {
         info->error_code = hss_error_got_null;
         goto failed;
    }
    if (w->status != hss_error_none) {
        info->error_code = w->status;
        goto failed;
    }

    /* If we're given a raw private key, make sure it's the one we're */
    /* thinking of */
    if (!update_private_key) {
        if (0 != memcmp( context, w->private_key, PRIVATE_KEY_LEN)) {
            info->error_code = hss_error_key_mismatch;
            return false;   /* Private key mismatch */
        }
    }

    /* Check if the buffer we were given is too short */
    if (w->signature_len > signature_buf_len) {
        /* The signature would overflow the buffer */
        info->error_code = hss_error_buffer_overflow;
        goto failed;
    }

    unsigned levels = w->levels;
    /*
     * Compile the current count
     */
    sequence_t current_count = 0;
    for (i=0; i < levels; i++) {
        struct merkle_level *tree = w->tree[i];
        current_count <<= tree->level;
            /* We subtract 1 because the nonbottom trees are already advanced */
        current_count += (sequence_t)tree->current_index - 1;
    }
    current_count += 1;   /* Bottom most tree isn't already advanced */

    /* Ok, try to advance the private key */
    if (!hss_advance_count(w, current_count,
                               update_private_key, context, info,
                               &trash_private_key)) {
        /* hss_advance_count fills in the error reason */
        goto failed;
    }

       /* Ok, now actually generate the signature */

    /* We'll be doing several things in parallel */
    struct thread_collection *col = hss_thread_init(info->num_threads);
    enum hss_error_code got_error = hss_error_none;

    /* Generate the signature */
    {
        struct gen_sig_detail gen_detail;
        gen_detail.signature = signature;
        gen_detail.signature_len = w->signature_len;
        gen_detail.message = message;
        gen_detail.message_len = message_len;
        gen_detail.w = w;
        gen_detail.got_error = &got_error;

        hss_thread_issue_work(col, do_gen_sig, &gen_detail, sizeof gen_detail);
    }

    /* Update the bottom level next tree */
    if (levels > 1) {
        struct step_next_detail step_detail;
        step_detail.w = w;
        step_detail.tree = w->tree[levels-1];
        step_detail.got_error = &got_error;

        hss_thread_issue_work(col, do_step_next, &step_detail, sizeof step_detail);
    }

    /* Issue orders to step each of the building subtrees in the bottom tree */
    int skipped_a_level = 0;   /* Set if the below issued didn't issue an */
                               /* order for at least one level */
    {
        struct merkle_level *tree = w->tree[levels-1];
        merkle_index_t updates_before_end = tree->max_index - tree->current_index + 1;
        int h_subtree = tree->subtree_size;
        for (i=1; i<tree->sublevels; i++) {
            struct subtree *subtree = tree->subtree[i][BUILDING_TREE];
                /* Check if there is a building tree */
            if (updates_before_end < (merkle_index_t)1 <<
                                         (subtree->levels_below + h_subtree)) {
                /* No; we're at the last subtree within this tree */
                skipped_a_level = 1;
                continue;
            }
            struct step_building_detail step_detail;
            step_detail.tree = tree;
            step_detail.subtree = subtree;
            step_detail.got_error = &got_error;

            hss_thread_issue_work(col, do_step_building, &step_detail, sizeof step_detail);

        }
            /* If there's only one sublevel, act as if we always skipped a sublevel */
        if (tree->sublevels == 1) skipped_a_level = 1;
    }

    /*
     * And, if we're allowed to give the parent a chance to update, and
     * there's a parent with some updating that needs to be done, schedule
     * that to be done
     */
    if (skipped_a_level &&
        levels > 1 && w->tree[levels-2]->update_count != UPDATE_DONE) {
        struct update_parent_detail detail;
        detail.w = w;
        detail.got_error = &got_error;
        hss_thread_issue_work(col, do_update_parent, &detail, sizeof detail);
    }

    /* Wait for all of them to finish */
    hss_thread_done(col);

    /* Check if any of them reported a failure */
    if (got_error != hss_error_none) {
        info->error_code = got_error;
        goto failed;
    }

    current_count += 1;  /* The new count is one more than what is */
                         /* implied by the initial state of the Merkle trees */

    /*
     * Now, we scan to see if we exhausted a Merkle tree, and need to update it
     * At the same time, we check to see if we need to advance the subtrees
     */
    sequence_t cur_count = current_count;
    unsigned merkle_levels_below = 0;
    int switch_merkle = w->levels;
    struct merkle_level *tree;
    for (i = w->levels-1; i>=1; i--, merkle_levels_below += tree->level) {
        tree = w->tree[i];

        if (0 == (cur_count & (((sequence_t)1 << (merkle_levels_below + tree->level))-1))) {
            /* We exhausted this tree */
            if ((i-1) == 0) {
                /* We've run out of signatures; we've already caught this */
                /* above; just make *sure* we've marked the key as */
                /* unusable, and give up */
                w->status = hss_error_private_key_expired;
                break;
            }

            /* Remember we'll need to switch to the NEXT_TREE */
            switch_merkle = i;
            continue;
        }

        /* Check if we need to advance any of the subtrees */
        unsigned subtree_levels_below = 0;
        unsigned j;
        for (j = tree->sublevels-1; j>0; j--) {
            subtree_levels_below += tree->subtree_size;
            if (0 != (cur_count & (((sequence_t)1 << (merkle_levels_below + subtree_levels_below))-1))) {
                /* We're in the middle of this subtree */
                goto done_advancing;
            }

            /* Switch to the building subtree */
            struct subtree *next = tree->subtree[j][BUILDING_TREE];
            struct subtree *prev = tree->subtree[j][ACTIVE_TREE];
            unsigned char *stack = next->stack;  /* Stack stays with */
                                                 /* building tree */
            tree->subtree[j][ACTIVE_TREE] = next;
                /* We need to reset the parameters on the new building subtree */
            prev->current_index = 0;
            prev->left_leaf += (merkle_index_t)2 << subtree_levels_below;
            tree->subtree[j][BUILDING_TREE] = prev;
            next->stack = NULL;
            prev->stack = stack;
        }
    }
done_advancing:
    /* Check if we used up any Merkle trees; if we have, switch to the */
    /* NEXT_TREE (which we've built in our spare time) */
    for (i = switch_merkle; i < w->levels; i++) {
        struct merkle_level *tree_l = w->tree[i];
        struct merkle_level *parent = w->tree[i-1];
        unsigned j;

        /* Rearrange the subtrees */
        for (j=0; j<tree->sublevels; j++) {
            /* Make the NEXT_TREE active; replace it with the current active */
            struct subtree *active = tree_l->subtree[j][NEXT_TREE];
            struct subtree *next = tree_l->subtree[j][ACTIVE_TREE];
            unsigned char *stack = active->stack;  /* Stack stays with */
                                                 /* next tree */

            active->left_leaf = 0;
            next->current_index = 0;
            next->left_leaf = 0;
            tree_l->subtree[j][ACTIVE_TREE] = active;
            tree_l->subtree[j][NEXT_TREE] = next;
            active->stack = NULL;
            next->stack = stack;
            if (j > 0) {
                /* Also reset the building tree */
                struct subtree *building = tree->subtree[j][BUILDING_TREE];
                building->current_index = 0;
                merkle_index_t size_subtree = (merkle_index_t)1 <<
                                (tree->subtree_size + building->levels_below);
                building->left_leaf = size_subtree;
            }
        }

        /* Copy in the value of seed, I we'll use for the new tree */
        memcpy( tree_l->seed, tree->seed_next, SEED_LEN );
        memcpy( tree_l->I, tree->I_next, I_LEN );

        /* Compute the new next I, which is derived from either the parent's */
        /* I or the parent's I_next value */
        merkle_index_t index = parent->current_index;
        if (index == parent->max_index) {
            hss_generate_child_seed_I_value(tree->seed_next, tree->I_next,
                                       parent->seed_next, parent->I_next, 0,
                                       parent->lm_type,
                                       parent->lm_ots_type);
        } else {
            hss_generate_child_seed_I_value( tree->seed_next, tree->I_next,
                                       parent->seed, parent->I, index+1,
                                       parent->lm_type,
                                       parent->lm_ots_type);
         }

        tree_l->current_index = 0;  /* We're starting this from scratch */

         /* Generate the signature of the new level */
         if (!hss_create_signed_public_key( w->signed_pk[i], w->siglen[i-1],
                                            tree_l, parent, w )) {
            info->error_code = hss_error_internal;
            goto failed;
        }
    }

    /* And we've set things up for the next signature... */

    if (trash_private_key) {
        OQS_MEM_cleanse( w->private_key, PRIVATE_KEY_LEN );
    }

    return true;

failed:

    if (trash_private_key) {
        OQS_MEM_cleanse( w->private_key, PRIVATE_KEY_LEN );
    }

    /* On failure, make sure that we don't return anything that might be */
    /* misconstrued as a real signature */
    OQS_MEM_cleanse( signature, signature_buf_len );
    return false;
}

/*
 * Get the signature length
 */
size_t hss_get_signature_len_from_working_key(struct hss_working_key *w) {
    if (!w || w->status != hss_error_none) return 0;

    int levels = w->levels;
    if (levels > MAX_HSS_LEVELS) return 0;
    param_set_t lm[MAX_HSS_LEVELS], ots[MAX_HSS_LEVELS];
    int i;
    for (i=0; i<levels; i++) {
        lm[i] = w->tree[i]->lm_type;
        ots[i] = w->tree[i]->lm_ots_type;
    }

    return hss_get_signature_len(levels, lm, ots);
}
