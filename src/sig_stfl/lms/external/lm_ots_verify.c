// SPDX-License-Identifier: MIT
/*
 * This is the code that implements the one-time-signature part of the LMS hash
 * based signatures
 */
#include <string.h>
#include "lm_ots_verify.h"
#include "lm_ots_common.h"
#include "hash.h"
#include "endian.h"
#include "common_defs.h"

/*
 * This validate a OTS signature for a message.  It doesn't actually use the
 * public key explicitly; instead, it just produces the root key, based on the
 * message; the caller is assumed to compare it to the expected value
 * Parameters:
 * - computed_public_key - where to place the reconstructed root.  It is
 *      assumed that the caller has allocated enough space
 * - I: the nonce value ("I") to use
 * - q: diversification string
 * - message - the message to verify
 * - message_len - the length of the message
 * - message_prehashed - true if the message has already undergone the initial
 *              (D_MESG) hash
 * - signature - the signature
 * - signature_len - the length of the signature
 * - parameter_set - what we expect the parameter set to be
 *
 * This returns true on successfully recomputing a root value; whether it is
 * the right one is something the caller would need to verify
 */
bool lm_ots_validate_signature_compute(
    unsigned char *computed_public_key,
    const unsigned char *I, merkle_index_t q,
    const void *message, size_t message_len, bool message_prehashed,
    const unsigned char *signature, size_t signature_len,
    param_set_t expected_parameter_set) {
    if (signature_len < 4) return false;  /* Ha, ha, very funny... */

    /* We don't trust the parameter set that's in the signature; verify it */
    param_set_t parameter_set = (param_set_t)get_bigendian( signature, 4 );
    if (parameter_set != expected_parameter_set) {
        return false;
    }

    unsigned h, n, w, p, ls;
    if (!lm_ots_look_up_parameter_set( parameter_set, &h, &n, &w, &p, &ls ))
        return false;

    if (signature_len != 4 + n * (p+1)) return false;

    const unsigned char *C = signature + 4;
    const unsigned char *y = C + n;

    unsigned char Q[MAX_HASH + 2];
    if (message_prehashed) {
        memcpy( Q, message, n );
     } else {
        union hash_context ctx;
        /* Compute the initial hash */
        hss_init_hash_context(h, &ctx);
            /* Hash the message prefix */
        {
            unsigned char prefix[ MESG_PREFIX_MAXLEN ];
            memcpy( prefix + MESG_I, I, I_LEN );
            put_bigendian( prefix + MESG_Q, q, 4 );
            SET_D( prefix + MESG_D, D_MESG );
            memcpy( prefix + MESG_C, C, n );
            hss_update_hash_context(h, &ctx, prefix, MESG_PREFIX_LEN(n) );
        }
            /* Then, the message */
        hss_update_hash_context(h, &ctx, message, message_len );

        hss_finalize_hash_context( h, &ctx, Q );
    }

    /* Append the checksum to the randomized hash */
    put_bigendian( &Q[n], lm_ots_compute_checksum(Q, n, w, ls), 2 );

    /* And, start building the parts for the final hash */
    union hash_context final_ctx;
    hss_init_hash_context(h, &final_ctx);
    {
        unsigned char prehash_prefix[ PBLC_PREFIX_LEN ];
        memcpy( prehash_prefix + PBLC_I, I, I_LEN );
        put_bigendian( prehash_prefix + PBLC_Q, q, 4 );
        SET_D( prehash_prefix + PBLC_D, D_PBLC );
        hss_update_hash_context(h, &final_ctx, prehash_prefix,
                                PBLC_PREFIX_LEN );
    }

    unsigned i;
    unsigned char tmp[ITER_MAX_LEN];

    /* Preset the parts of tmp that don't change */
    memcpy( tmp + ITER_I, I, I_LEN );
    put_bigendian( tmp + ITER_Q, q, 4 );

    unsigned max_digit = (1<<w) - 1;
    for (i=0; i<p; i++) {
        put_bigendian( tmp + ITER_K, i, 2 );
        memcpy( tmp + ITER_PREV, y + i*n, n );
        unsigned a = lm_ots_coef( Q, i, w );
        unsigned j;
        for (j=a; j<max_digit; j++) {
            union hash_context ctx;
            tmp[ITER_J] = j;
            hss_hash_ctx( tmp + ITER_PREV, h, &ctx, tmp, ITER_LEN(n) );
        }

        hss_update_hash_context(h, &final_ctx, tmp + ITER_PREV, n );
    }

    /* Ok, finalize the public key hash */
    hss_finalize_hash_context( h, &final_ctx, computed_public_key );

    /*
     * We succeeded in computing a root value; the caller will need to decide
     * if the root we computed is actually the correct one
     */
    return true;
}
