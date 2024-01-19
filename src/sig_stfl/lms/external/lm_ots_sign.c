// SPDX-License-Identifier: MIT
/*
 * This is the code that implements the one-time-signature part of the LMS hash
 * based signatures
 */
#include <string.h>
#include "common_defs.h"
#include "lm_ots.h"
#include "lm_ots_common.h"
#include "hash.h"
#include "endian.h"
#include "hss_zeroize.h"
#include "hss_derive.h"
#include "hss_internal.h"

bool lm_ots_generate_public_key(
    param_set_t lm_ots_type,
    const unsigned char *I, /* Public key identifier */
    merkle_index_t q,       /* Diversification string, 4 bytes value */
    struct seed_derive *seed,
    unsigned char *public_key, size_t public_key_len) {

    /* Look up the parameter set */
    unsigned h, n, w, p, ls;
    LMS_UNUSED(public_key_len);
    if (!lm_ots_look_up_parameter_set( lm_ots_type, &h, &n, &w, &p, &ls ))
        return false;

    /* Start the hash that computes the final value */
    union hash_context public_ctx;
    hss_init_hash_context(h, &public_ctx);
    {
        unsigned char prehash_prefix[ PBLC_PREFIX_LEN ];
        memcpy( prehash_prefix + PBLC_I, I, I_LEN );
        put_bigendian( prehash_prefix + PBLC_Q, q, 4 );
        SET_D( prehash_prefix + PBLC_D, D_PBLC );
        hss_update_hash_context(h, &public_ctx, prehash_prefix,
                                PBLC_PREFIX_LEN );
    }

    /* Now generate the public key */
    /* This is where we spend the majority of the time during key gen and */
    /* signing operations; it would make sense to attempt to try to take */
    /* advantage of parallel (SIMD) hardware; even if we use it nowhere */
    /* else, we'd get a significant speed up */
    unsigned i, j;

    unsigned char buf[ ITER_MAX_LEN ];
    memcpy( buf + ITER_I, I, I_LEN );
    put_bigendian( buf + ITER_Q, q, 4 );
    union hash_context ctx;

    hss_seed_derive_set_j( seed, 0 );

    for (i=0; i<p; i++) {
        hss_seed_derive( buf + ITER_PREV, seed, i < p-1 );
        put_bigendian( buf + ITER_K, i, 2 );
        /* We'll place j in the buffer below */
        for (j=0; j < (unsigned)(1<<w) - 1; j++) {
            buf[ITER_J] = j;

            hss_hash_ctx( buf + ITER_PREV, h, &ctx, buf, ITER_LEN(n) );
        }
        /* Include that in the hash */
        hss_update_hash_context(h, &public_ctx, buf + ITER_PREV, n );
    }

    /* And the result of the running hash is the public key */
    hss_finalize_hash_context( h, &public_ctx, public_key );

    hss_zeroize( &ctx, sizeof ctx );

    return true;
}

/*
 * This generates the randomizer C.  We assume seed has been initialized to
 * the expected q value
 */
void lm_ots_generate_randomizer(unsigned char *c, unsigned n,
                                struct seed_derive *seed) {
    unsigned char randomizer[ SEED_LEN ];

    hss_seed_derive_set_j( seed, SEED_RANDOMIZER_INDEX );

    hss_seed_derive( randomizer, seed, false );

    memcpy( c, randomizer, n );
}


bool lm_ots_generate_signature(
    param_set_t lm_ots_type,
    const unsigned char *I, /* Public key identifier */
    merkle_index_t q,       /* Diversification string, 4 bytes value */
    struct seed_derive *seed,
    const void *message, size_t message_len, bool prehashed,
    unsigned char *signature, size_t signature_len) {

    /* Look up the parameter set */
    unsigned h, n, w, p, ls;
    if (!lm_ots_look_up_parameter_set( lm_ots_type, &h, &n, &w, &p, &ls ))
        return false;

    /* Check if we have enough room */
    if (signature_len < 4 + n + p*n) return false;

    /* Export the parameter set to the signature */
    put_bigendian( signature, lm_ots_type, 4 );

    union hash_context ctx;
    /* Select the randomizer.  Note: we do this determanistically, because
     * upper levels of the HSS tree sometimes sign the same message with the
     * same index (between multiple reboots), hence we want to make sure that
     * the randomizer for a particualr index is the same
     * Also, if we're prehashed, we assume the caller has already selected it,
     * and placed it into the siganture */

    if (!prehashed) {
        lm_ots_generate_randomizer( signature+4, n, seed);
    }

    /* Compute the initial hash */
    unsigned char Q[MAX_HASH + 2];
    if (!prehashed) {
        hss_init_hash_context(h, &ctx);

        /* First, we hash the message prefix */
        unsigned char prefix[MESG_PREFIX_MAXLEN];
        memcpy( prefix + MESG_I, I, I_LEN );
        put_bigendian( prefix + MESG_Q, q, 4 );
        SET_D( prefix + MESG_D, D_MESG );
        memcpy( prefix + MESG_C, signature+4, n );
        hss_update_hash_context(h, &ctx, prefix, MESG_PREFIX_LEN(n) );

            /* Then, the message */
        hss_update_hash_context(h, &ctx, message, message_len );
        hss_finalize_hash_context( h, &ctx, Q );
    } else {
        memcpy( Q, message, n );
    }

    /* Append the checksum to the randomized hash */
    put_bigendian( &Q[n], lm_ots_compute_checksum(Q, n, w, ls), 2 );

    unsigned i;
    unsigned char tmp[ITER_MAX_LEN];

    /* Preset the parts of tmp that don't change */
    memcpy( tmp + ITER_I, I, I_LEN );
    put_bigendian( tmp + ITER_Q, q, 4 );

    hss_seed_derive_set_j( seed, 0 );
    for (i=0; i<p; i++) {
        put_bigendian( tmp + ITER_K, i, 2 );
        hss_seed_derive( tmp + ITER_PREV, seed, i<p-1 );
        unsigned a = lm_ots_coef( Q, i, w );
        unsigned j;
        for (j=0; j<a; j++) {
            tmp[ITER_J] = j;
            hss_hash_ctx( tmp + ITER_PREV, h, &ctx, tmp, ITER_LEN(n) );
        }
        memcpy( &signature[ 4 + n + n*i ], tmp + ITER_PREV, n );
    }

    hss_zeroize( &ctx, sizeof ctx );

    return true;
}
