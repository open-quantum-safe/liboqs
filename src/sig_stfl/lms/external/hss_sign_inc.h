// SPDX-License-Identifier: MIT
#if !defined( HSS_SIGN_INC_H_ )
#define HSS_SIGN_INC_H_
#include <stdbool.h>
#include <stddef.h>
#include "hash.h"
#include "common_defs.h"
#include "lms_namespace.h"

/*
 * These are the functions to sign a message incrementally.
 * That is, we assume that we don't have the entire message at
 * once, instead, we have it in pieces (for example, the signature
 * is of a multigigabyte file)
 *
 * Usage:
 *    struct hss_sign_inc ctx;
 *    bool success = hss_sign_init( &ctx, working_key,
 *            update_private_key, private_key_context,
 *            signature, signature_buffer_len,
 *            &lsat_signature );
 *    hss_sign_update( &ctx, message_part_1, len_1 );
 *    hss_sign_update( &ctx, message_part_2, len_2 );
 *    hss_sign_update( &ctx, message_part_3, len_3 );
 *    success = hss_sign_finalize( &ctx, working_key, signature );
 *    if (success) printf( "We generated the signature\n" );
 *
 * This is in its own include file because we need to import some
 * 'not-generally-for-general-consumption' include files to make
 * it work (as they're in the hss_sign_inc structure)
 */

/*
 * This is the context structure that holds the intermedate results of an
 * in-process signature
 * It's a application-visible structure for ease of use: the application can
 * allocate it as an automatic, and if the application aborts in the middle of
 * signing, it doesn't cause a memory leak
 */
struct hss_sign_inc {
    enum hss_error_code status; /* Either hss_error_none if we're in */
                       /* process, or the reason why we'd fail */

    int h;             /* The hash function */
    merkle_index_t q;  /* The index of the bottom level signature */
    union hash_context hash_ctx; /* For the running hash we use */

    unsigned char c[MAX_HASH];  /* The C value we used */
};

struct hss_extra_info;

/* Starts off the process of incrementally signing a message */
/* If it detects a failure, this returns false */
/* Handing the return code is optional; if this fails, the finalization */
/* step will fail too */
bool hss_sign_init(
    struct hss_sign_inc *ctx,
    struct hss_working_key *working_key,
    bool (*update_private_key)(unsigned char *private_key,
            size_t len_private_key, void *context),
    void *context,
    unsigned char *signature, size_t signature_len,
    struct hss_extra_info *info);

/* This adds another piece of the message to sign */
/* Again, the result code is optional */
bool hss_sign_update(
    struct hss_sign_inc *ctx,
    const void *message_segment,
    size_t len_message_segment);

/* This finalizes the signature generation */
/* This returns true if the signature was generated properly */
/* We ask the caller to pass in the working key again, we need to review */
/* the private key (we don't want to place it in the context) */
bool hss_sign_finalize(
    struct hss_sign_inc *ctx,
    const struct hss_working_key *working_key,
    unsigned char *signature,
    struct hss_extra_info *info);

#endif /* HSS_SIGN_INC_H_ */
