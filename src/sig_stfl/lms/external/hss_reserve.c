// SPDX-License-Identifier: MIT
#include <string.h>
#include "common_defs.h"
#include "hss_internal.h"
#include "hss_reserve.h"
#include "endian.h"

/*
 * Initialize the reservation count to the given value
 */
void hss_set_reserve_count(struct hss_working_key *w, sequence_t count) {
    w->reserve_count = count;
}

/*
 * Set the autoreserve count
 */
bool hss_set_autoreserve(struct hss_working_key *w,
            unsigned sigs_to_autoreserve, struct hss_extra_info *info) {
    if (!w) {
        if (info) info->error_code = hss_error_got_null;
        return false;
    }

    /* Note: we do not check if the working key is in a usable state */
    /* There are a couple of odd-ball scenarios (e.g. when they've */
    /* manually allocated the key, but haven't loaded it yet) that we */
    /* don't have a good reason to disallow */

    w->autoreserve = sigs_to_autoreserve;
    return true;
}

/*
 * This is called when we generate a signature; it checks if we need
 * to write out a new private key (and advance the reservation); if it
 * decides it needs to write out a new private key, it also decides how
 * far it needs to advance it
 */
bool hss_advance_count(struct hss_working_key *w, sequence_t cur_count,
        bool (*update_private_key)(unsigned char *private_key,
                size_t len_private_key, void *context),
        void *context,
        struct hss_extra_info *info, bool *trash_private_key) {

    if (cur_count == w->max_count) {
        /* We hit the end of the root; this will be the last signature */
        /* this private key can do */
        w->status = hss_error_private_key_expired; /* Fail if they try to */
                                                   /* sign any more */
        info->last_signature = true;
            /* Make sure we zeroize the private key */
        *trash_private_key = true;  /* We can't trash our copy of the */
                /* private key until after we've generated the signature */
                /* We can trash the copy in secure storage, though */
        if (update_private_key) {
            unsigned char private_key[PRIVATE_KEY_LEN];
            OQS_MEM_cleanse(private_key, PRIVATE_KEY_LEN);
            if (!update_private_key(private_key, PRIVATE_KEY_LEN, context)) {
                info->error_code = hss_error_private_key_write_failed;
                return false;
            }
        } else {
            memset( context, PARM_SET_END, PRIVATE_KEY_LEN );
        }
        return true;
    }
    sequence_t new_count = cur_count + 1;

    if (new_count > w->reserve_count) {
        /* We need to advance the reservation */

        /* Check if we have enough space to do the entire autoreservation */
        if (w->max_count - new_count > w->autoreserve) {
            new_count += w->autoreserve;
        } else {
            /* If we don't have enough space, reserve what we can */
            new_count = w->max_count;
        }

        put_bigendian( w->private_key + PRIVATE_KEY_INDEX, new_count,
                       PRIVATE_KEY_INDEX_LEN );
        if (update_private_key) {
            if (!update_private_key(w->private_key, PRIVATE_KEY_INDEX_LEN,
                                   context)) {
                 /* Oops, we couldn't write the private key; undo the */
                 /* reservation advance (and return an error) */
                 info->error_code = hss_error_private_key_write_failed;
                 put_bigendian( w->private_key + PRIVATE_KEY_INDEX,
                       w->reserve_count, PRIVATE_KEY_INDEX_LEN );
                return false;
            }
        } else {
            put_bigendian( context, new_count, PRIVATE_KEY_INDEX_LEN );
        }
        w->reserve_count = new_count;
    }

    return true;
}

/*
 * This will make sure that (at least) N signatures are reserved; that is, we
 * won't need to actually call the update function for the next N signatures
 * generated
 *
 * This can be useful if the update_private_key function is expensive.
 *
 * Note that if, N (or more) signatures are already reserved, this won't do
 * anything.
 */
bool hss_reserve_signature(
    struct hss_working_key *w,
    bool (*update_private_key)(unsigned char *private_key,
            size_t len_private_key, void *context),
    void *context,
    unsigned sigs_to_reserve,
    struct hss_extra_info *info) {
    struct hss_extra_info temp_info = { 0 };
    if (!info) info = &temp_info;
    if (!w) {
        info->error_code = hss_error_got_null;
        return false;
    }
    if (w->status != hss_error_none) {
        info->error_code = w->status;;
        return false;
    }

    if (sigs_to_reserve > w->max_count) {
        info->error_code = hss_error_not_that_many_sigs_left;
        return false; /* Very funny */
    }

    /*
     * If we're given a raw private key, make sure it's the one we're
     * thinking of.
     * I have no idea why someone would reserve signatures if they have
     * a raw private key (which is cheap to update), however there's no
     * reason we shouldn't support it
     */
    if (!update_private_key) {
        if (0 != memcmp( context, w->private_key, PRIVATE_KEY_LEN)) {
            info->error_code = hss_error_key_mismatch;
            return false;   /* Private key mismatch */
        }
    }

    /* Figure out what the current count is */
    sequence_t current_count = 0;
    unsigned i;
    for (i = 0; i<w->levels; i++) {
        struct merkle_level *tree = w->tree[i];
            /* -1 because the current_index counts the signatures to the */
            /* current next level */
        current_count = (current_count << tree->level) +
                                                  tree->current_index - 1;
    }
    current_count += 1;   /* The bottom-most tree isn't advanced */

    sequence_t new_reserve_count;  /* This is what the new reservation */
                     /* setting would be (if we accept the reservation) */
    if (current_count > w->max_count - sigs_to_reserve) {
        /* Not that many sigantures left */
        /* Reserve as many as we can */
        new_reserve_count = w->max_count;
    } else {
        new_reserve_count = current_count + sigs_to_reserve;
    }

    if (new_reserve_count <= w->reserve_count) {
        /* We already have (at least) that many reserved; do nothing */
        return true;
    }

    /* Attempt to update the count in the private key */
    put_bigendian( w->private_key + PRIVATE_KEY_INDEX, new_reserve_count,
                   PRIVATE_KEY_INDEX_LEN );
    /* Update the copy in NV storage */
    if (update_private_key) {
        if (!update_private_key(w->private_key, PRIVATE_KEY_INDEX_LEN,
                                                                  context)) {
             /* Oops, couldn't update it */
             put_bigendian( w->private_key + PRIVATE_KEY_INDEX,
                        w->reserve_count, PRIVATE_KEY_INDEX_LEN );
             info->error_code = hss_error_private_key_write_failed;
             return false;
        }
    } else {
        memcpy( context, w->private_key, PRIVATE_KEY_INDEX_LEN );
    }
    w->reserve_count = new_reserve_count;

    return true;
}
