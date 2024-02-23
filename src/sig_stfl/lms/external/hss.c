// SPDX-License-Identifier: MIT
/*
 * This is an implementation of the HSS signature scheme from LMS
 * This is designed to be full-featured
 *
 * Currently, this file consists of functions that don't have a better home
 */
#include <stdlib.h>
#include <string.h>
#include "common_defs.h"
#include "hss.h"
#include "hash.h"
#include "endian.h"
#include "hss_internal.h"
#include "hss_aux.h"
#include "hss_derive.h"
#include "config.h"
#include "lm_ots_common.h"

/*
 * Allocate and load an ephemeral key
 */
struct hss_working_key *hss_load_private_key(
    bool (*read_private_key)(unsigned char *private_key,
            size_t len_private_key, void *context),
        void *context,
    size_t memory_target,
    const unsigned char *aux_data, size_t len_aux_data,
    struct hss_extra_info *info ) {

    /* Step 1: determine the parameter set */
    unsigned levels;
    param_set_t lm[ MAX_HSS_LEVELS ];
    param_set_t ots[ MAX_HSS_LEVELS ];
    if (!hss_get_parameter_set( &levels, lm, ots, read_private_key, context)) {
        /* Can't read private key, or private key invalid */
        return 0;
    }

    /* Step 2: allocate the ephemeral key */
    struct hss_working_key *w = allocate_working_key(levels, lm, ots,
                                                 memory_target, info);
    if (!w) {
        /* Memory allocation failure, most likely (we've already vetted */
        /* the parameter sets) */
        return 0;
    }

    /* Step 3: load the ephemeral key */
    if (! hss_generate_working_key( read_private_key, context,
                                    aux_data, len_aux_data, w, info )) {
        /* About the only thing I can see failing here is perhaps */
        /* attempting to reread the private key failed the second time; */
        /* seems unlikely, but not impossible */
        hss_free_working_key( w );
        return 0;
    }

    /* Success! */
    return w;
}

/*
 * Internal function to generate the root seed and I value (based on the
 * private seed).  We do this (rather than select seed, I at random) so that
 * we don't need to store it in our private key; we can recompute them
 */
bool hss_generate_root_seed_I_value(unsigned char *seed, unsigned char *I,
                                    const unsigned char *master_seed) {
#if SECRET_METHOD == 2
    /* In ACVP mode, we use the master seed as the source for both the */
    /* root seed, and the root I value */
    memcpy( seed, master_seed, SEED_LEN );
    memcpy( I, master_seed + SEED_LEN, I_LEN );
#else
    /*
     * We use a two-level hashing scheme so that we end up using the master
     * seed only twice throughout the system (once here, once to generate the
     * aux hmac key)
     */
    unsigned char hash_preimage[ TOPSEED_LEN ];
    unsigned char hash_postimage[ MAX_HASH ];

    memset( hash_preimage + TOPSEED_I, 0, I_LEN );
    memset( hash_preimage + TOPSEED_Q, 0, 4 );
    SET_D( hash_preimage + TOPSEED_D, D_TOPSEED );
    hash_preimage[TOPSEED_WHICH] = 0x00;
    memcpy( hash_preimage + TOPSEED_SEED, master_seed, SEED_LEN );

        /* We use a fixed SHA256 hash; we don't care about interoperability */
        /* so we don't need to worry about what parameter set the */
        /* user specified */
#if I_LEN > 32 || SEED_LEN != 32
#error This logic needs to be reworked
#endif
    union hash_context ctx;

    hss_hash_ctx(hash_postimage, HASH_SHA256, &ctx, hash_preimage,
                                                            TOPSEED_LEN );
    memcpy( hash_preimage + TOPSEED_SEED, hash_postimage, SEED_LEN );

    /* Now compute the top level seed */
    hash_preimage[TOPSEED_WHICH] = 0x01;
    hss_hash_ctx(seed, HASH_SHA256, &ctx, hash_preimage, TOPSEED_LEN );

    /* Now compute the top level I value */
    hash_preimage[TOPSEED_WHICH] = 0x02;
    hss_hash_ctx(hash_postimage, HASH_SHA256, &ctx, hash_preimage,
                                                            TOPSEED_LEN );
    memcpy( I, hash_postimage, I_LEN );

    hss_zeroize( hash_preimage, sizeof hash_preimage );  /* There's keying */
                                                       /* data here */
    hss_zeroize( &ctx, sizeof ctx );
#endif
    return true;
}

/*
 * Internal function to generate the child I value (based on the parent's
 * I value).  While this needs to be determanistic (so that we can create the
 * same I values between reboots), there's no requirement for interoperability.
 * So we use a fixed SHA256; when we support a hash function other than SHA256,
 * we needn't update this.
 */
bool hss_generate_child_seed_I_value( unsigned char *seed, unsigned char *I,
                   const unsigned char *parent_seed,
                   const unsigned char *parent_I,
                   merkle_index_t index,
                   param_set_t lm, param_set_t ots) {
    struct seed_derive derive;
    if (!hss_seed_derive_init( &derive, lm, ots, parent_I, parent_seed )) {
        return false;
    }

    hss_seed_derive_set_q( &derive, index );

    /* Compute the child seed value */
    hss_seed_derive_set_j( &derive, SEED_CHILD_SEED );
    hss_seed_derive( seed, &derive, true );
        /* True sets the j value to SEED_CHILD_I */

    /* Compute the child I value; with increment_j set to true in the */
    /* above call, derive has been set to the SEED_CHILD_I position */
    unsigned char postimage[ SEED_LEN ];
    hss_seed_derive( postimage, &derive, false );
    memcpy( I, postimage, I_LEN );

    hss_seed_derive_done( &derive );

    return true;
}

void hss_init_extra_info( struct hss_extra_info *p ) {
    if (p) memset( p, 0, sizeof *p );
}

void hss_extra_info_set_threads( struct hss_extra_info *p, int num_threads ) {
    if (p) p->num_threads = num_threads;
}

bool hss_extra_info_test_last_signature( struct hss_extra_info *p ) {
    if (!p) return false;
    return p->last_signature;
}

enum hss_error_code hss_extra_info_test_error_code( struct hss_extra_info *p ) {
    if (!p) return hss_error_got_null;
    return p->error_code;
}
