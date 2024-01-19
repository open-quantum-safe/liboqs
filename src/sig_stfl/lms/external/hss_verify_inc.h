// SPDX-License-Identifier: MIT
#if !defined( HSS_VERIFY_INC_H_ )
#define HSS_VERIFY_INC_H_
#include <stdbool.h>
#include <stddef.h>
#include "hash.h"
#include "common_defs.h"
#include "hss.h"
#include "lms_namespace.h"

/*
 * These are the functions to validate a signature incrementally.
 * That is, we assume that we don't have the entire message at
 * once, instead, we have it in pieces (for example, the signature
 * is of a multigigabyte file)
 *
 * Usage:
 *    struct hss_validate_inc ctx;
 *    bool success = hss_validate_init( &ctx, public_key, signature );
 *    hss_validate_update( &ctx, message_part_1, len_1 );
 *    hss_validate_update( &ctx, message_part_2, len_2 );
 *    hss_validate_update( &ctx, message_part_3, len_3 );
 *    success = hss_validate_finalize( &ctx, signature );
 *    if (success) printf( "The signature validated\n" );
 *
 * This is in its own include file because we need to import some
 * 'not-generally-for-general-consumption' include files to make
 * it work (as they're in the hss_validate_inc structure)
 */

/*
 * This is the context structure that holds the intermedate results of an
 * in-process validation
 * It's a application-visible structure for ease of use: the application can
 * allocate it as an automatic, and if the application aborts in the middle of
 * the validation, it doesn't cause a memory leak
 */
struct hss_validate_inc {
    enum hss_error_code status; /* Either hss_error_none if we're in */
                       /* process, or the reason why we'd fail */
    size_t signature_offset; /* Offset of the final signature within the */
                      /* HSS signature */
    size_t signature_len; /* Length of the final signature */

    unsigned h;         /* Hash function used */

        /* The final public key.  We need this at finalization time, */
        /* however they might not be in the signature (L=1 case) */
    unsigned char final_public_key[8 + I_LEN + MAX_HASH];

    union hash_context hash_ctx; /* For the running hash we use */
};

struct hss_extra_info;

/* Starts off the process of incrementally validating a signature */
/* If it detects a failure, this returns false */
/* Handing the return code is optional; if this fails, the finalization */
/* step will fail too */
bool hss_validate_signature_init(
    struct hss_validate_inc *ctx,
    const unsigned char *public_key,
    const unsigned char *signature, size_t signature_len,
    struct hss_extra_info *info);

/* This adds another piece of the message to validate */
/* Again, the result code is optional */
bool hss_validate_signature_update(
    struct hss_validate_inc *ctx,
    const void *message_segment,
    size_t len_message_segment);

/* This finalizes the signature validation */
/* This returns true if the signature validates (and we didn't detect any */
/* intermediate failures) */
/* We ask the caller to pass in the signature again, because we'd prefer */
/* not having to place the final LMS signature in the ctx structure; that'd */
/* make it larger than we'd like */
bool hss_validate_signature_finalize(
    struct hss_validate_inc *ctx,
    const unsigned char *signature,
    struct hss_extra_info *info);

#endif /* HSS_VERIFY_INC_H_ */
