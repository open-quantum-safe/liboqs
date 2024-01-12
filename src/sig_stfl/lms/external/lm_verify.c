// SPDX-License-Identifier: MIT
/*
 * This is the code that implements the tree part of the LMS hash
 * based signatures
 */
#include <string.h>
#include "lm_verify.h"
#include "lm_common.h"
#include "lm_ots_common.h"
#include "lm_ots_verify.h"
#include "hash.h"
#include "endian.h"
#include "common_defs.h"

/*
 * XDR requires us to pad the I value out to a multiple of 4
 * This computes how long the field will be after padding
 * That is, it rounds len_I up to the next multiple of 4
 */
#define padded_length(len_I) (((len_I) + 3) & ~3)

/*
 * This validate an LM signature for a message.  It does take an XDR-encoded
 * signature, and verify against it.
 * Parameters:
 * - public_key - the XDR-encoded public ley
 * - message - the message to verify
 * - message_len - the length of the message
 * - signature - the signature
 * - signature_len - the length of the signature
 *
 * This returns true if the signature verifies
 */
bool lm_validate_signature(
    const unsigned char *public_key,
    const void *message, size_t message_len, bool prehashed,
    const unsigned char *signature, size_t signature_len) {
    union hash_context ctx;

    param_set_t lm_type = (param_set_t)get_bigendian( public_key + LM_PUB_PARM_SET, 4 );
    param_set_t ots_type = (param_set_t)get_bigendian( public_key + LM_PUB_OTS_PARM_SET, 4 );

    unsigned h, n, height;
    if (!lm_look_up_parameter_set(lm_type, &h, &n, &height)) return false;

    unsigned char computed_public_key[MAX_HASH];

    const unsigned char *I = public_key + LM_PUB_I;

    if (signature_len < 8) return false;
    merkle_index_t count = (param_set_t)get_bigendian( signature, 4 );
    signature += 4; signature_len -= 4;  /* 4 bytes, rather then 8 */
        /*  the OTS type is expected to be a part of the OTS signature, */
        /* which lm_ots_validate_signature_compute will expect */

    /* Compute the OTS root */
    size_t ots_publen = lm_ots_get_public_key_len(ots_type);
    size_t ots_siglen = lm_ots_get_signature_len(ots_type);
    if (ots_publen == 0 || ots_siglen == 0) return false;
    if (signature_len < ots_siglen) return false;

    unsigned char ots_sig[LEAF_MAX_LEN];
    if (!lm_ots_validate_signature_compute(ots_sig + LEAF_PK, I, count,
                  message, message_len, prehashed,
                  signature, ots_siglen, ots_type)) return false;
    signature += ots_siglen; signature_len -= ots_siglen;

    /* Get the parameter set declared in the sigature; make sure it matches */
    /* what we expect */
    if (signature_len < 4) return false;
    param_set_t parameter_set = (param_set_t)get_bigendian( signature, 4 );
    if (parameter_set != lm_type) return false;
    signature += 4; signature_len -= 4;

    merkle_index_t count_nodes = (merkle_index_t)1 << height;

    if (signature_len != n * height) return false;  /* We expect the auth */
                                    /* path to be there as the last element */
    if (count >= count_nodes) return false;  /* Index out of range */
    merkle_index_t node_num = count + count_nodes;

    memcpy( ots_sig + LEAF_I, I, I_LEN );
    put_bigendian( ots_sig + LEAF_R, node_num, 4 );
    SET_D( ots_sig + LEAF_D, D_LEAF );
    hss_hash_ctx( computed_public_key, h, &ctx, ots_sig, LEAF_LEN(n) );

    unsigned char prehash[ INTR_MAX_LEN ];
    memcpy( prehash + INTR_I, I, I_LEN );
    SET_D( prehash + INTR_D, D_INTR );
    while (node_num > 1) {
        if (node_num % 2) {
            memcpy( prehash + INTR_PK + 0, signature, n );
            memcpy( prehash + INTR_PK + n, computed_public_key, n );
        } else {
            memcpy( prehash + INTR_PK + 0, computed_public_key, n );
            memcpy( prehash + INTR_PK + n, signature, n );
        }
        signature += n;
        node_num /= 2;
        put_bigendian( prehash + INTR_R, node_num, 4 );
        hss_hash_ctx( computed_public_key, h, &ctx, prehash, INTR_LEN(n) );
    }

    /* Now, check to see if the root we computed matches the root we should have */
    unsigned offset = LM_PUB_I + padded_length(I_LEN);

    return 0 == memcmp( computed_public_key, public_key + offset, n );
}
