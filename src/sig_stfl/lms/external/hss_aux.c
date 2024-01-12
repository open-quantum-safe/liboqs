// SPDX-License-Identifier: MIT
/*
 * This is the implementation of the aux data within the HSS tree
 */

#include <string.h>
#include "hss_aux.h"
#include "hss_internal.h"
#include "common_defs.h"
#include "lm_common.h"
#include "endian.h"
#include "hash.h"
#include "hss_zeroize.h"

/*
 * The structure of aux data
 *
 * The current format of the file is:
 * [4 bytes of marker]:
 *  - bit 31 is set (to indicate that the aux data is nonempty; a 0 first byte
 *    indicates that, yes, we have no bananas); because we store the marker
 *    in bigendian format, this bit 31 is in the first byte.
 *  - bit i is set if we have the hashes for intermediate level i
 * For each set bit i (in ascending sequence):
 *   - 1<<i hashes (which are the node values for level i of the top level
 *     Merkle tree)
 * Finally, an HMAC for the entire file (except for the HMAC); the key for the
 * HMAC is derived from the master seed
 */
#define AUX_DATA_MARKER 0   /* The marker for the aux data; either the aux */
                            /* level we're saving (4 bytes; first byte */
                            /* nonezero), or NO_AUX_DATA if we're not */
                            /* using it */
#define NO_AUX_DATA  0x00
#define AUX_DATA_HASHES 4   /* The actual hashes start here */

static void compute_seed_derive( unsigned char *result, unsigned hash,
     const unsigned char *seed, union hash_context *ctx);
static void compute_hmac( unsigned char *dest,
                          unsigned hash, unsigned size_hash,
                          union hash_context *ctx,
                          unsigned char *key,
                          const unsigned char *data, size_t len_data);

/*
 * This computes the optimal aux level (which is a bitmap of the levels we save
 * in the aux data, plus a set msbit if we save anything), given the bound on
 * the size of the aux data we can support.
 *
 * At the current time, the top level tree always has a subtree size which is
 * a simple function of the tree height (unless we're dealing with an HSS
 * height of 1, in which case it'll always be a multiple of that value); hence
 * the nodes we need to recompute the subtrees are at the levels 0 mod n from
 * the bottom.  So, we store as many of those levels as can fit; the highest
 * levels give us the most savings (and are the cheapest to store), so we
 * start there
 */
aux_level_t hss_optimal_aux_level( size_t max_length,
                   const param_set_t *lm_type,
                   const param_set_t *lm_ots_type,
                   size_t *actual_len ) {
    unsigned h0;  /* The height of the root tree */
    unsigned size_hash;  /* The size of each hash that would appear in the */
                  /* aux data */
    LMS_UNUSED(lm_ots_type);
    if (!lm_look_up_parameter_set(lm_type[0], NULL, &size_hash, &h0)) {
        /* Unrecognized parameter set */
        return 0;
    }

    if (max_length < AUX_DATA_HASHES + size_hash) {
        /* Not enough room for even the marker and the MAC (not count the */
        /* actual data we'd want to store) */
        if (actual_len) *actual_len = 1;
        return 0;
    }
    size_t orig_max_length = max_length;
    max_length -= AUX_DATA_HASHES + size_hash;

    aux_level_t aux_level = 0;
    unsigned subtree_size = hss_smallest_subtree_size(h0, 0, size_hash);
    unsigned level = h0 % subtree_size; /* This is the level of the base of the */
                                 /* topmost subtree */
    if (level == 0) level = subtree_size;   /* No point in saving the root */

    /* Step through the levels, see what will fit */
    for (; level < h0; level += subtree_size) {
        size_t len_this_level = (size_t)size_hash<<level;
        if (max_length >= len_this_level) {
            /* This level fits; add it */
            max_length -= len_this_level;
            /* We also set the MSBit to signify that we're saving something */
            aux_level |= 0x80000000UL | ((aux_level_t)1<<level);
        } else {
            /* This level doesn't fit; no larger one would fit either */
            break;
        }
    }

    if (actual_len) *actual_len = orig_max_length - max_length;

    return aux_level;
}

/*
 * This compares the n bytes at l'a' and at 'b' in time (hopefully) independent
 * for what's in 'a' and 'b'
 * If they are the same, it returns 0 - if not, it returns some other value
 * I wrote it this way to make it less likely that a clever compiler would
 * be able to 'optimize' this into something nonconstant time (in part, because
 * while we know 'sum' will never wrap, the compiler is likely not able to
 * verify it)
 */
static unsigned memcmp_consttime( const void *a, const void *b, size_t n ) {
    unsigned sum = 0;
    const unsigned char *p = a;
    const unsigned char *q = b;
    while (n--) {
	sum += *p++ ^ *q++;
    }
    return sum;
}

/*
 * This takes a saved aux data, and initializes an array of pointers into it
 * If a working key is provided, it'll also authenticate the data within the
 * structure
 */
struct expanded_aux_data *hss_expand_aux_data( const unsigned char *aux_data,
                   size_t len_aux_data,
                   struct expanded_aux_data *temp, unsigned size_hash,
                   struct hss_working_key *w ) {
    /* Check if we really have any aux data */
    if (!aux_data || aux_data[AUX_DATA_MARKER] == NO_AUX_DATA) return 0;

    const unsigned char *orig_aux_data = aux_data;
    unsigned long aux_level = (unsigned long)get_bigendian( aux_data, 4 );
    aux_data += 4;
    aux_level &= 0x7ffffffffL;  /* Turn off the 'used' marker */

    unsigned h;
    for (h = 0; h <= MAX_MERKLE_HEIGHT; h++, aux_level >>= 1) {
        if (aux_level & 1) {
            temp->data[h] = (void *)aux_data;
            aux_data += (size_t)size_hash << h;
        } else {
            temp->data[h] = 0;   /* No data at this level */
        }
    }

    /* Now, check if the data is valid */
    if (w) {
        /* Check to see if the data is valid */
        size_t expected_len = (aux_data - orig_aux_data) + size_hash;
        if (expected_len > len_aux_data) {
            /* Either the first 4 bytes were messed up, or the file was */
            /* truncated */
            return 0;
        }
        if (len_aux_data < 4 + size_hash) return 0;

        /* Now, MAC the entire aux file */
        union hash_context ctx;
        unsigned char key[ MAX_HASH ];
        compute_seed_derive( key, w->tree[0]->h, w->working_key_seed, &ctx );
        unsigned char expected_mac[ MAX_HASH ];
        compute_hmac( expected_mac, w->tree[0]->h, size_hash, &ctx, key,
                          orig_aux_data, aux_data - orig_aux_data );
        hss_zeroize( key, size_hash );
        hss_zeroize( &ctx, sizeof ctx );
        if (0 != memcmp_consttime( expected_mac, aux_data, size_hash)) {
            /* The MAC did not agree; ignore the aux data */
            return 0;
        }
    }
    return temp;
}

/*
 * This returns the amount of aux data we would use, given the maximum bound
 * on how much aux data we are allowed, and the parameter sets
 */
size_t hss_get_aux_data_len(size_t max_length,
                   unsigned levels,
                   const param_set_t *lm_type,
                   const param_set_t *lm_ots_type) {
    size_t len = 0;
    LMS_UNUSED(levels);
    if (!hss_optimal_aux_level( max_length, lm_type, lm_ots_type, &len )) {
        return 1;  /* 1 byte marker to say 'we're not using it */
    }

    return len;
}

/*
 * Save the marker within the aux data
 */
void hss_store_aux_marker( unsigned char *aux_data, aux_level_t aux_level ) {
    if (aux_level == 0) {
         /* Aux data doesn't help; mark it as unused */
         aux_data[AUX_DATA_MARKER] = NO_AUX_DATA;
    } else {
         put_bigendian( &aux_data[AUX_DATA_MARKER], aux_level, 4 );
    }
}

/*
 * This is called while we are building the initial top level Merkle tree (to
 * compute the root).  This is called for each internal node, and allows the
 * aux data a chance to save the intermediate value
 */
void hss_save_aux_data( struct expanded_aux_data *data, unsigned level,
                        unsigned size_hash, merkle_index_t q,
                        const unsigned char *cur_val ) {
    if (!data) return;     /* We're not recording anything */
    if (!data->data[level]) return;  /* We're not recording anything for */
                           /* this level */

    /* We are recording it; save a copy in the aux data */
    memcpy( data->data[level] + size_hash * q, cur_val, size_hash );
}

/*
 * This generates the derived value that we'll use as a key the authenticate
 * the aux data.  We pass the ctx (rather than using a local one) so we have
 * one less thing to zeroize
 *
 * We use a derived key (rather than using the seed directly) because the
 * outer hash within the HMAC don't use the diversification factors that every
 * other hash within this packet does; hence for HMAC, we use a key that
 * is independent of every other hash used
 */
static void compute_seed_derive( unsigned char *result, unsigned hash,
     const unsigned char *seed, union hash_context *ctx) {
    hss_init_hash_context( hash, ctx );
    unsigned char prefix[ DAUX_PREFIX_LEN ];
    memset( prefix, 0, DAUX_D );
    SET_D( prefix + DAUX_D, D_DAUX );
    hss_update_hash_context( hash, ctx, prefix, sizeof prefix );
    hss_update_hash_context( hash, ctx, seed, SEED_LEN );
    hss_finalize_hash_context( hash, ctx, result );

    hss_zeroize( &ctx, sizeof ctx );
}

static void xor_key( unsigned char *key, unsigned xor_val, unsigned len_key) {
    unsigned i;
    for (i = 0; i<len_key; i++) {
        key[i] ^= xor_val;
    }
}

#define IPAD 0x36
#define OPAD 0x5c

/*
 * This computes the hmac; it assumes that the key is size_hash bytes
 * long (and while it does modify it during processing, it restores
 * it at the end)
 * This can obviously be optimized; however, this is not performance critical,
 * so we keep it simple
 */
static void compute_hmac( unsigned char *dest,
                          unsigned hash, unsigned size_hash,
                          union hash_context *ctx,
                          unsigned char *key,
                          const unsigned char *data, size_t len_data) {
    unsigned block_size = hss_hash_blocksize(hash);

    /* Step 1: first phase of the HMAC */
    hss_init_hash_context( hash, ctx );
    xor_key( key, IPAD, size_hash );
    hss_update_hash_context( hash, ctx, key, size_hash );
    unsigned j;
    for (j = size_hash; j<block_size; j++) {
         const unsigned char ipad = IPAD;
         hss_update_hash_context( hash, ctx, &ipad, 1 );
    }
    hss_update_hash_context( hash, ctx, data, len_data );

    hss_finalize_hash_context( hash, ctx, dest );  /* We place the */
               /* intermediate MAC result where the final result will go */

    /* Step 2: second phase of the HMAC */
    hss_init_hash_context( hash, ctx );
    xor_key( key, IPAD^OPAD, size_hash );
    hss_update_hash_context( hash, ctx, key, size_hash );
    for (j = size_hash; j<block_size; j++) {
         const unsigned char opad = OPAD;
         hss_update_hash_context( hash, ctx, &opad, 1 );
    }
    hss_update_hash_context( hash, ctx, dest, size_hash );
    hss_finalize_hash_context( hash, ctx, dest );  /* This time it's the */
                   /* actual MAC result */

    /* Step 3: clean up for the next round */
    xor_key( key, OPAD, size_hash );
}

/*
 * This is called when we're done computing the aux data; this generates the
 * authentication code that goes with each level
 */
void hss_finalize_aux_data( struct expanded_aux_data *data,
                            unsigned size_hash, unsigned hash,
                            const unsigned char *seed) {
    if (!data) return;     /* We're not recording anything */

    union hash_context ctx;

    /* Generate the key we'll use to authenticate the data */
    unsigned char aux_seed[ MAX_HASH ];
    compute_seed_derive( aux_seed, hash, seed, &ctx );

    unsigned char *aux = 0;
    size_t total_length = 4;
    unsigned i;
    for (i = 0; i<MAX_MERKLE_HEIGHT+1; i++) {
        if (data->data[i]) {
            total_length += (size_t)size_hash << i;
            if (!aux) {
                aux = data->data[i] - 4;
            }
        }
    }
    if (aux) {
        compute_hmac( aux+total_length, hash, size_hash, &ctx, aux_seed,
                      aux, total_length );
    }

    hss_zeroize( &ctx, sizeof ctx );
    hss_zeroize( aux_seed, size_hash );
}

/*
 * This is called when we need to use aux data; it checks to see if we've
 * stored the nodes within the aux data; if we have, it extracts them,
 * and returns true
 */
bool hss_extract_aux_data(const struct expanded_aux_data *aux, unsigned level,
            const struct hss_working_key *w, unsigned char *dest,
            merkle_index_t node_offset,    /* Offset of node on this level */
            merkle_index_t node_count) {   /* # of nodes to restore */
    if (!aux) return false;              /* No aux data */
    if (!aux->data[level]) return false; /* We don't have that specific */
                                     /* level saved */
    unsigned hash_size = w->tree[0]->hash_size;

    /* We do have the data; copy it to the destination */
    memcpy( dest,
            aux->data[level] + node_offset*hash_size,
            node_count * hash_size );

    return true;
}
