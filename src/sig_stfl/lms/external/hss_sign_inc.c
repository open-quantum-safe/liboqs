// SPDX-License-Identifier: MIT
/*
 * This is the code that implements the hierarchical part of the LMS hash
 * based signatures; in this case, incremental signing
 */
#include <string.h>
#include "hss.h"
#include "common_defs.h"
#include "hss_verify_inc.h"
#include "lm_verify.h"
#include "lm_common.h"
#include "lm_ots.h"
#include "lm_ots_verify.h"
#include "hash.h"
#include "endian.h"
#include "hss_internal.h"
#include "hss_sign_inc.h"
#include "hss_derive.h"
#include <oqs/oqsconfig.h>

/*
 * Start the process of creating an HSS signature incrementally. Parameters:
 * ctx - The state we'll use to track the incremental signature
 * working_key - the in-memory version of the in-memory private key
 * update_private_key - function to call to update the master private key
 * context - context pointer for above
 * siganture - the buffer to hold the signature
 * signature_len - the length of the buffer
 * this_is_the_last_signature - if non-NULL, this will be set if this
 *    signature is the last for this private key
 */
#ifdef OQS_ALLOW_LMS_KEY_AND_SIG_GEN
bool hss_sign_init(
    struct hss_sign_inc *ctx,
    struct hss_working_key *w,
    bool (*update_private_key)(unsigned char *private_key,
            size_t len_private_key, void *context),
    void *context,
    unsigned char *signature, size_t signature_len,
    struct hss_extra_info *info) {
    struct hss_extra_info temp_info = { 0 };;
    if (!info) info = &temp_info;

    if (!ctx) {
        info->error_code = hss_error_got_null;
        return false;
    }
    ctx->status = hss_error_ctx_uninitialized; /* Until we hear otherwise, */
                                       /* we got a failure */

    if (!w) {
        info->error_code = hss_error_got_null;
        return false;
    }
    if (w->status != hss_error_none) {
        info->error_code = w->status;
        return false;
    }

    struct merkle_level *bottom = w->tree[ w->levels - 1 ];

    unsigned char I[I_LEN];
    memcpy( I, bottom->I, I_LEN );

    /* Compute the value of C we'll use */
    merkle_index_t q = bottom->current_index;
    ctx->q = q;
    int h = bottom->h;
    ctx->h = h;

    struct seed_derive derive;
    if (!hss_seed_derive_init( &derive, bottom->lm_type, bottom->lm_ots_type,
                       bottom->I, bottom->seed )) return false;
    hss_seed_derive_set_q(&derive, q);
    lm_ots_generate_randomizer( ctx->c, bottom->hash_size, &derive );
    hss_seed_derive_done(&derive);

    /*
     * Ask the signature generation process to do everything *except*
     * the bottom level OTS signature
     */
    bool success = hss_generate_signature( w,
                            update_private_key, context,
                            NULL, 0,  /* <--- we don't have the message yet */
                            signature, signature_len, info );
    if (!success) {
        /* On failure, hss_generate_signature fills in the failure reason */
        ctx->status = info->error_code;
        hss_zeroize( &ctx->c, sizeof ctx->c );  /* People don't get to */
                              /* learn what randomizer we would have used */
        return false;
    }

    /* Now, initialize the context */
    hss_init_hash_context( h, &ctx->hash_ctx );
    {
        unsigned char prefix[ MESG_PREFIX_MAXLEN ];
        memcpy( prefix + MESG_I, I, I_LEN );
        unsigned q_bin[4]; put_bigendian( q_bin, q, 4 );
        memcpy( prefix + MESG_Q, q_bin, 4 ); /* q */
        SET_D( prefix + MESG_D, D_MESG );
        int n = bottom->hash_size;
        memcpy( prefix + MESG_C, ctx->c, n );  /* C */
        hss_update_hash_context(h, &ctx->hash_ctx, prefix, MESG_PREFIX_LEN(n) );
    }

    /* It succeeded so far... */
    ctx->status = hss_error_none;
    return true;
}

/* This adds another piece of the message to validate */
bool hss_sign_update(
    struct hss_sign_inc *ctx,
    const void *message_segment,
    size_t len_message_segment) {
    if (!ctx || ctx->status != hss_error_none) return false;

    hss_update_hash_context(ctx->h, &ctx->hash_ctx,
                            message_segment, len_message_segment );

    return true;
}

/* We've added all the pieces of the messages, now do the validation */
bool hss_sign_finalize(
    struct hss_sign_inc *ctx,
    const struct hss_working_key *working_key,
    unsigned char *signature,
    struct hss_extra_info *info) {
    struct hss_extra_info temp_info = { 0 };
    if (!info) info = &temp_info;

    if (!ctx) {
        info->error_code = hss_error_got_null;
        return false;
    }
    if (ctx->status != hss_error_none) {
        info->error_code = ctx->status;
        return false;
    }

    /* Success or fail, we can't use the context any more */
    ctx->status = hss_error_ctx_already_used;

    int L = working_key->levels;

    /* Step through the signature, looking for the place to put the OTS */
    /* signature, and (while we're at it) recovering the I and seed values */
    const unsigned char *I = working_key->tree[0]->I;
    const unsigned char *seed = working_key->tree[0]->seed;
        /* Note: we alternate buffers during generation in case */
        /* hss_generate_child_seed_I_value doesn't allow new values to */
        /* overwrite old ones */
    unsigned char I_buff[2][I_LEN];
    unsigned char seed_buff[2][SEED_LEN];

    /* Q: should we double check the various fixed fields of the signatures */
    /* (e.g. the number of signed keys, the parameter sets? */

    signature += 4;

    int i;
    for (i=0; i<L-1; i++) {
        merkle_index_t q = (merkle_index_t)get_bigendian( signature, 4 );
        if (q > working_key->tree[i]->max_index) {
            hss_zeroize( seed_buff, sizeof seed_buff );
            return 0;
        }
        if (!hss_generate_child_seed_I_value( seed_buff[i&1], I_buff[i&1],
                                         seed, I, q,
                                         working_key->tree[i]->lm_type,
                                         working_key->tree[i]->lm_ots_type )) {
            hss_zeroize( seed_buff, sizeof seed_buff );
            info->error_code = hss_error_internal;
            return false;
        }

        seed = seed_buff[i&1];
        I = I_buff[i&1];

        /* Step to the end of this signed key */
        signature += lm_get_signature_len( working_key->tree[i]->lm_type,
                                            working_key->tree[i]->lm_ots_type);
        signature += lm_get_public_key_len(working_key->tree[i+1]->lm_type);
    }

    /* Now, signature points to where the bottom LMS signature should go */
        /* It starts with the q value */
    put_bigendian( signature, ctx->q, 4 );
    signature += 4;
        /* And then the LM-OTS signature */

    /* Copy in the C value into the signature */
    memcpy( signature+4, ctx->c, 32 );

    /* Generate the final hash */
    unsigned char hash[ MAX_HASH ];
    hss_finalize_hash_context( ctx->h, &ctx->hash_ctx, hash );

    /* And the final OTS signature based on that hash */
    param_set_t lm_type = working_key->tree[i]->lm_type;
    param_set_t ots_type = working_key->tree[i]->lm_ots_type;
    struct seed_derive derive;
    bool success = hss_seed_derive_init( &derive, lm_type, ots_type,
                          I, seed );
    if (success) {
        hss_seed_derive_set_q( &derive, ctx->q );
        success = lm_ots_generate_signature(
               ots_type, I, ctx->q, &derive, hash, 0, true,
               signature, lm_ots_get_signature_len( ots_type ));

        hss_seed_derive_done( &derive );
    }
    if (!success) {
        info->error_code = hss_error_internal;
    }

    hss_zeroize( seed_buff, sizeof seed_buff );
    return success;
}
#endif
