// SPDX-License-Identifier: MIT
/*
 * This is the code that implements the hierarchical part of the LMS hash
 * based signatures
 */
#include <string.h>
#include "common_defs.h"
#include "hss_verify.h"
#include "lm_verify.h"
#include "lm_common.h"
#include "lm_ots_verify.h"
#include "hash.h"
#include "endian.h"
#include "hss_thread.h"
#include "hss_internal.h"
#include "hss.h"

/* The HSS public key consists of: */
/* Number of levels (1-8) (4 bytes) */
/* The top level LM public key */

/* The HSS signature consists of: */
/* A word giving the number of levels - 1 == L-1 */
/* L-1 iterations of (i = 1..L-1): */
/*    - LMS Signature of public key i (signed by the pub key of level i-1) */
/*    - LMS Public key (of level i) */
/* - LMS Signature of the message, signed by the bottomost pub key */

/* This is the routine that runs on a thread to validate an LMS signature */
void validate_internal_sig(const void *data,
                               struct thread_collection *col) {
    const struct verify_detail *d = data;

    bool success = lm_validate_signature(d->public_key,
                                         d->message, d->message_len, false,
                                         d->signature, d->signature_len);

    if (!success) {
        /* Drat, it failed; call the failure in */
        hss_thread_before_write(col);
        *d->got_error = hss_error_bad_signature;
        hss_thread_after_write(col);
    }
}

/*
 * Validate an HSS signature, using a public key.  Parameters:
 * public_key - pointer to the public key
 * message - the mmessage that was supposedly signed
 * message_len - the size of the message
 * siganture - the signature we're checking
 * signature_len - the length of the signature
 *
 * This returns true if everything checks out and the signature verifies
 * false on error (whether the error is because the signature didn't verify,
 * or we hit some sort of error on the way)
 */
bool hss_validate_signature(
    const unsigned char *public_key,
    const void *message, size_t message_len,
    const unsigned char *signature, size_t signature_len,
    struct hss_extra_info *info) {
    struct hss_extra_info temp_info = { 0 };
    if (!info) info = &temp_info;
    unsigned i;

    /* Get the number of levels the signature claims */
    if (signature_len < 4) {
         info->error_code = hss_error_bad_signature;
         return false;
    }
    uint_fast32_t levels = (uint_fast32_t)get_bigendian( signature, 4 ) + 1;
        /* +1 because what's in the signature is levels-1 */
    signature += 4; signature_len -= 4;
    if (levels < MIN_HSS_LEVELS || levels > MAX_HSS_LEVELS ||
                               levels != (uint_fast32_t)get_bigendian( public_key, 4 )) {
        info->error_code = hss_error_bad_signature;
        return false;
    }

    /* Compare that to what the public key says */
    uint_fast32_t pub_levels = (uint_fast32_t)get_bigendian( public_key, 4 );
    if (levels != pub_levels) {
        /* Signature and public key don't agree */
        info->error_code = hss_error_bad_signature;
        return false;
    }
    /* We'll use the LMS public key embedded in the HSS public key as the */
    /* key to use to validate the top level signature */
    public_key += 4;

    struct thread_collection *col = hss_thread_init(info->num_threads);
    enum hss_error_code got_error = hss_error_none;
    struct verify_detail detail;
    detail.got_error = &got_error;

    /* Parse through the signature, kicking off the tasks to validate */
    /* individual LMS signatures within it as we go */
    for (i=0; i<levels-1; i++) {
        /*
         * At this point of time, the current position in the signature
         * looks like (or, rather, is *supposed to look like*) this:
         *     <Signature A><Public Key B><Other stuff>
         * where:
         * - Signature A is the LMS signature of Public Key B
         * - Public Key B is the message we're verifying (and will be
         *   interpreted as a public key in the next iteration)
         * public_key points to Public Key A, which is the public key that
         * we use to verify Signature A
         */

        /* Get the length of Signature A */
        param_set_t lm_type = (param_set_t)get_bigendian( public_key, 4 );
        param_set_t lm_ots_type = (param_set_t)get_bigendian( public_key+4, 4 );
        unsigned l_siglen = (unsigned)lm_get_signature_len(lm_type, lm_ots_type);
        if (l_siglen == 0 || l_siglen > signature_len) {
            info->error_code = hss_error_bad_signature;
             goto failed;
        }

        /* Retain a pointer to Signature A, and advance the current */
        /* pointer to Public Key B */
        const unsigned char *l_sig = signature;
        signature += l_siglen; signature_len -= l_siglen;

        /* The next thing is the next level public key (Public Key B) */
        /* which we need to validate) */
        if (signature_len < 4) {
            info->error_code = hss_error_bad_signature;
            goto failed;
        }
        /*
         * Get how long Public Key B would be, assuming it is a valid
         * public key.  If it's not a valid public key (that is, if
         * someone other than the valid signer modified it), then
         * Signature A will not validate, and so we'll catch that
         */
        lm_type = (param_set_t)get_bigendian( signature, 4 );
        unsigned l_pubkeylen = (unsigned)lm_get_public_key_len(lm_type);
        if (l_pubkeylen == 0 || l_pubkeylen > signature_len) {
            info->error_code = hss_error_bad_signature;
            goto failed;
        }

        /* Retain a pointer to Public Key B, and advance the current */
        /* pointer past it (to the data the next iteration cares about) */
        const unsigned char *l_pubkey = signature;
        signature += l_pubkeylen; signature_len -= l_pubkeylen;

        /* Now, schedule the validation of Signature A */
        detail.public_key = public_key;    /* Public key A */
        detail.message = l_pubkey;         /* Public key B, that is, */
                                           /* the message to validate */
        detail.message_len = l_pubkeylen;
        detail.signature = l_sig;          /* Signature A */
        detail.signature_len = l_siglen;
        hss_thread_issue_work( col, validate_internal_sig,
                               &detail, sizeof detail );

        /* We validated this level's public key (or, at least, scheduled */
        /* it, if it turns out not to validate, we'll catch it below) */
        /* Use the current Public Key B as the next level's Public Key A */
        public_key = l_pubkey;
    }

    /*
     * We're at the bottom level; now, the current position in the signature
     * looks like (or, rather, is *supposed to look like*) this:
     *     <Signature A>
     * where:
     * - Signature A is the bottom signature, which signs the actual
     *   message
     * public_key points to the bottom level public key, which is used to
     * validate the signature
     *
     * Just go ahead and schedule the validation
     */
    detail.public_key = public_key;    /* Public key to use */
    detail.message = message;          /* The user's message that needs */
    detail.message_len = message_len;  /* validation */
    detail.signature = signature;      /* Bottom level LMS signature */
    detail.signature_len = signature_len;
    hss_thread_issue_work( col, validate_internal_sig,
                           &detail, sizeof detail );

    /* Wait for all the threads to complete */
    hss_thread_done(col);

    /* It succeeded if none of the threads reported an error */
    if (got_error == hss_error_none) return true;
    info->error_code = got_error;
    return false;

failed:           /* If we get an intermediate failure */
    hss_thread_done(col);
    return false;
}
