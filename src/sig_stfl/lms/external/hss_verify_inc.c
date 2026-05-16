// SPDX-License-Identifier: MIT
/*
 * This is the code that implements the hierarchical part of the LMS hash
 * based signatures; in this case, incremental verification
 */
#include <string.h>
#include "common_defs.h"
#include "hss_verify_inc.h"
#include "lm_verify.h"
#include "lm_common.h"
#include "lm_ots_verify.h"
#include "hash.h"
#include "endian.h"
#include "hss_thread.h"
#include "hss_internal.h"
#include "lm_ots_common.h"
#include "hss.h"

/*
 * public key buffer as HSS format: level || lm_type || lm_ots || ...
 *                    LMS format: lm_type || lm_ots || ...
 * big-endian 32-bit word (bytes 8..11) is a supported LM-OTS parameter set
 * (see OQS_LMOTS_TYPE_LIST). In that layout, bytes 0..3 are the level count,
 * bytes 4..7 are the top-level LMS type, and bytes 8..11 are lm_ots. For a
 * bare LMS public key, bytes 8..11 are the start of the I field and are not
 * generally a valid LM-OTS id, so this function returns false.
 *

 * Returns true if the third word matches a known LM-OTS code; false otherwise.
 */
static bool is_hss_public_key(const unsigned char *public_key) {
	param_set_t w2 = (param_set_t)get_bigendian(public_key + 8, 4);
	return oqs_lmots_type_list_contains(w2);
}

/*
 * Parse a public key buffer as a serialized LMS or HSS.
 * Returns 0 on failure; 1 for a single-tree LMS public key; or the HSS level
 * count (from the first four bytes) when the buffer matches the HSS layout.
 *
 * In HSS form, bytes 8..11 are lm_ots (see OQS_LMOTS_TYPE_LIST);
 * in LMS form, bytes 8..11 begin the I field. is_hss_public_key() encodes
 * that distinction; lm_type fields are checked against OQS_LMS_TYPE_LIST.
 */
static uint_fast32_t public_key_levels(const unsigned char *public_key) {
	param_set_t w0 = (param_set_t)get_bigendian(public_key, 4);
	param_set_t w1 = (param_set_t)get_bigendian(public_key + 4, 4);
	param_set_t w2 = (param_set_t)get_bigendian(public_key + 8, 4);

	if (is_hss_public_key(public_key)) {
		uint_fast32_t levels = (uint_fast32_t)w0;
        /*
         * The level for public keys for single level trees sometimes have a value of zero.
         * in this case, bump  public key level to 1.
         */
        if (levels == 0)
            levels = 1;
		if (levels < MIN_HSS_LEVELS || levels > MAX_HSS_LEVELS) {
			return 0;
		}
		if (!oqs_lms_type_list_contains(w1) || !oqs_lmots_type_list_contains(w2)) {
			return 0;
		}

		return levels;
	}

	if (oqs_lms_type_list_contains(w0) && oqs_lmots_type_list_contains(w1)) {
		return 1;
	}

	return 0;
}

/*
 * Start the process of validating an HSS signature incrementally. Parameters:
 * ctx - The state we'll use to track the incremental validation
 * public_key - pointer to the public key
 * siganture - the signature we're checking
 * signature_len - the length of the signature
 */
bool hss_validate_signature_init(
    struct hss_validate_inc *ctx,
    const unsigned char *public_key,
    const unsigned char *signature, size_t signature_len,
    struct hss_extra_info *info) {
    struct hss_extra_info temp_info = { 0 };
    if (!info) info = &temp_info;
    unsigned i;
    if (!ctx) {
        info->error_code = hss_error_got_null;
        return false;
    }
    ctx->status = hss_error_ctx_uninitialized; /* Until we hear otherwise, */
                                       /* we got a failure */

    const unsigned char *orig_signature = signature;

    /* Get the number of levels the signature claims */
    if (signature_len < 4) {
        ctx->status = info->error_code = hss_error_bad_signature;
        return false;
    }
    uint_fast32_t levels = (uint_fast32_t)get_bigendian( signature, 4 ) + 1;
        /* +1 because what's in the signature is levels-1 */
    signature += 4; signature_len -= 4;
    uint_fast32_t pub_levels = public_key_levels(public_key);
    if (is_hss_public_key(public_key)) {
        public_key += 4;
    }

    if (levels < MIN_HSS_LEVELS || levels > MAX_HSS_LEVELS ) {
        ctx->status = info->error_code = hss_error_bad_signature;
        return false;
    }

    if (levels != pub_levels) {
        /* Signature and public key don't agree */
        ctx->status = info->error_code = hss_error_bad_signature;
        return false;
    }


    /* Validate the upper levels of the signature */
    struct thread_collection *col = NULL;
    if (levels > 1) {
        col = hss_thread_init(info->num_threads);
        enum hss_error_code got_error = hss_error_none;
        struct verify_detail detail;
        detail.got_error = &got_error;

        /* Scan through the signature, kicking off the tasks to validate it */
        /* as we go.  Note that we don't validate the bottom level yet */
        for (i=0; i<levels-1; i++) {
            /* The next thing is the signature of this public key */
            param_set_t lm_type = (param_set_t)get_bigendian( public_key, 4 );
            param_set_t lm_ots_type = (param_set_t)get_bigendian( public_key+4, 4 );
            unsigned l_siglen = (unsigned)lm_get_signature_len(lm_type, lm_ots_type);
            if (l_siglen == 0 || l_siglen > signature_len) goto failed;
            const unsigned char *l_sig = signature;
            signature += l_siglen; signature_len -= l_siglen;

            /* The next thing is the next level public key (which we need */
            /* to validate) */
            if (signature_len < 4) goto failed;
            lm_type = (param_set_t)get_bigendian( signature, 4 );
            unsigned l_pubkeylen = (unsigned)lm_get_public_key_len(lm_type);
            if (l_pubkeylen == 0 || l_pubkeylen > signature_len) goto failed;
            const unsigned char *l_pubkey = signature;
            signature += l_pubkeylen; signature_len -= l_pubkeylen;

            /* Validate the signature of this level's public key */
            detail.public_key = public_key;
            detail.message = l_pubkey;
            detail.message_len = l_pubkeylen;
            detail.signature = l_sig;
            detail.signature_len = l_siglen;
            hss_thread_issue_work( col, validate_internal_sig,
                                   &detail, sizeof detail );

            /* We validated this level's public key (or, at least, */
            /* scheduled it, if it turns out not to validate, we'll catch */
            /* it below), use it to validate the next level */
            public_key = l_pubkey;
        }

        /* Wait for all the threads to complete */
        hss_thread_done(col);
        col = NULL;

        if (got_error != hss_error_none) {
            ctx->status = info->error_code = got_error;
            return false;
        }
    }

    ctx->signature_offset = signature - orig_signature;
    ctx->signature_len = signature_len;

    /* We have the public key in front of us; stash a copy */
    /* Right now, we have a fixed length public key */
    /* If that changes, we'll need to investigate the parmaeter set */
    memcpy( ctx->final_public_key, public_key, 8 + I_LEN + MAX_HASH );

    /* Now, initialize the context */
    param_set_t ots_type = (param_set_t)get_bigendian( public_key+4, 4 );

    unsigned h, n;
    if (!lm_ots_look_up_parameter_set(ots_type, &h, &n, NULL, NULL, NULL)) {
        /* Because we're checking in parallel, this may be caused by */
        /* a bad signature */
        ctx->status = info->error_code = hss_error_bad_signature;
        return false;
    }
    ctx->h = h;
    hss_init_hash_context( h, &ctx->hash_ctx );
    {
        unsigned char prefix[ MESG_PREFIX_MAXLEN ];
        memcpy( prefix + MESG_I, ctx->final_public_key+8, I_LEN );
        memcpy( prefix + MESG_Q, signature, 4 ); /* q */
        SET_D( prefix + MESG_D, D_MESG );
        memcpy( prefix + MESG_C, signature+8, n );  /* C */
        hss_update_hash_context(h, &ctx->hash_ctx, prefix, MESG_PREFIX_LEN(n) );
    }

    /* It succeeded so far... */
    ctx->status = hss_error_none;
    return true;

failed:           /* If we get an intermediate failure */
    if (col) hss_thread_done(col);
    ctx->status = info->error_code = hss_error_bad_signature;
    return false;
}

/* This adds another piece of the message to validate */
bool hss_validate_signature_update(
    struct hss_validate_inc *ctx,
    const void *message_segment,
    size_t len_message_segment) {
    if (!ctx || ctx->status != hss_error_none) return false;

    hss_update_hash_context(ctx->h, &ctx->hash_ctx,
                            message_segment, len_message_segment );

    return true;
}

/* We've added all the pieces of the messages, now do the validation */
bool hss_validate_signature_finalize(
    struct hss_validate_inc *ctx,
    const unsigned char *signature,
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

    /* Generate the final hash */
    unsigned char hash[ MAX_HASH ];
    unsigned h = ctx->h;
    hss_finalize_hash_context( h, &ctx->hash_ctx, hash );

    /* It passes iff the final signature validates */
    if (lm_validate_signature(
            ctx->final_public_key,
            hash, sizeof hash, true,
            signature + ctx->signature_offset, ctx->signature_len)) {
        return true;
    }

    info->error_code = hss_error_bad_signature;
    return false;
}
