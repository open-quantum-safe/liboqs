// SPDX-License-Identifier: MIT
/*
 * This includes some computation methods that are shared between different
 * subsystems of the HSS signature package
 */

#include <string.h>
#include "hss_internal.h"
#include "hss.h"
#include "hash.h"
#include "hss_thread.h"
#include "lm_ots_common.h"
#include "lm_ots.h"
#include "endian.h"
#include "hss_derive.h"

/* Count the number of 1 bits at the end (lsbits) of the integer */
/* Do it in the obvious way; straightline code may be faster (no */
/* unpredictable jumps, which are costly), but that would be less scrutable */
/* (and this code is "fast enough") */
static int trailing_1_bits(merkle_index_t n) {
    int i;
    for (i=0; n&1; n>>=1, i++)
        ;
    return i;
}

/*
 * Compute the value of an internal node within a Merkle tree
 */
static enum hss_error_code hss_compute_internal_node( unsigned char *dest,
                            merkle_index_t node_num,
                            const unsigned char *seed,
                            param_set_t lm_type,
                            param_set_t lm_ots_type,
                            unsigned h,
                            unsigned leaf_level,
                            const unsigned char *I) {
    unsigned hash_size = hss_hash_length(h);

    /* We're store intermediate nodes here */
    unsigned char stack[ MAX_HASH * MAX_MERKLE_HEIGHT];

    merkle_index_t tree_size = (merkle_index_t)1 << leaf_level;
    merkle_index_t r = node_num;
    int levels_to_bottom = 0;
    if (r == 0) return hss_error_internal;  /* So no to infinite loops */
    while (r < tree_size) {
        r <<= 1;
        levels_to_bottom++;
    }
    merkle_index_t q = r - tree_size;

    merkle_index_t i;
    unsigned ots_len = (unsigned)lm_ots_get_public_key_len(lm_ots_type);
    unsigned char pub_key[ LEAF_MAX_LEN ];
    memcpy( pub_key + LEAF_I, I, I_LEN );
    SET_D( pub_key + LEAF_D, D_LEAF );

    struct seed_derive derive;
    if (!hss_seed_derive_init( &derive, lm_type, lm_ots_type,
                               I, seed)) {
        return hss_error_bad_param_set;
    }

    for (i=0;; i++, r++, q++) {
        /* Generate the next OTS public key */
        hss_seed_derive_set_q( &derive, q );
        if (!lm_ots_generate_public_key(lm_ots_type, I,
                   q, &derive, pub_key + LEAF_PK, ots_len)) {
            return hss_error_bad_param_set; /* The only reason the above */
                                            /* could fail */
        }

        /*
         * For the subtree which this leaf node forms the final piece, put the
         * destination to where we'll want it, either on the stack, or if this
         * is the final piece, to where the caller specified
         */
        unsigned char *current_buf;
        int stack_offset = trailing_1_bits( i );
        if (stack_offset == levels_to_bottom) {
            current_buf = dest;
        } else {
            current_buf = &stack[stack_offset * hash_size ];
        }

        /* Hash it to form the leaf node */
        put_bigendian( pub_key + LEAF_R, r, 4);
        union hash_context ctx;
        hss_hash_ctx( current_buf, h, &ctx, pub_key, LEAF_LEN(hash_size) );

        /* Work up the stack, combining right nodes with the left nodes */
        /* that we've already computed */
        int sp;
        for (sp = 1; sp <= stack_offset; sp++) {
            hss_combine_internal_nodes( current_buf,
                            &stack[(sp-1) * hash_size], current_buf,
                            h, I, hash_size,
                            r >> sp );
        }

        /* We're not at a left branch, or at the target node */

        /* Because we've set current_buf to point to where we want to place */
        /* the result of this loop, we don't need to memcpy it */

        /* Check if this was the last leaf (and so we've just computed the */
        /* target node) */
        if (stack_offset == levels_to_bottom) {
            /* We're at the target node; the node we were asked to compute */
            /* We've already placed the value into dest, so we're all done */
            break;
        }
    }

    hss_seed_derive_done( &derive );

    return hss_error_none;
}

/*
 * Combine adjacent left and right nodes within the Merkle tree
 * together
 */
void hss_combine_internal_nodes( unsigned char *dest,
        const unsigned char *left_node, const unsigned char *right_node,
        int h, const unsigned char *I, unsigned hash_size,
        merkle_index_t node_num) {
    unsigned char hash_val[ INTR_MAX_LEN ];
    memcpy( hash_val + INTR_I, I, I_LEN );
    put_bigendian( hash_val + INTR_R, node_num, 4 );
    SET_D( hash_val + INTR_D, D_INTR );

    memcpy( hash_val + INTR_PK,             left_node,  hash_size );
    memcpy( hash_val + INTR_PK + hash_size, right_node, hash_size );
    union hash_context ctx;
    hss_hash_ctx( dest, h, &ctx, hash_val, INTR_LEN(hash_size) );
}

/*
 * This computes an array of intermediate Merkle nodes given by data
 * This may be run in a worker (non-main) thread
 */
void hss_gen_intermediate_tree(const void *data,
                               struct thread_collection *col) {
    const struct intermed_tree_detail *d = data;
    unsigned hash_len = hss_hash_length(d->h);
    unsigned i;

    for (i=0; i<d->node_count; i++) {
        unsigned char result[ MAX_HASH ];
        enum hss_error_code status = hss_compute_internal_node( result,
                            d->node_num + i,
                            d->seed,
                            d->lm_type,
                            d->lm_ots_type,
                            d->h,
                            d->tree_height,
                            d->I);

        /* Report the results */
        hss_thread_before_write(col);
        if (status == hss_error_none) {
            /* Copy out the resulting hash */
            memcpy( d->dest + i*hash_len, result, hash_len );
        } else {
            /* Something went wrong; report the bad news */
            *d->got_error = status;
            hss_thread_after_write(col);  /* No point in working more */
            return;
        }
        hss_thread_after_write(col);
    }
}
