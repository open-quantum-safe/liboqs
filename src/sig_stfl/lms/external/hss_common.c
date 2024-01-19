// SPDX-License-Identifier: MIT
/*
 * This is the code that is common between an HSS verifier, and a full HSS
 * implementation that both signs and verifies
 */
#include <stdint.h>
#include "common_defs.h"
#include "hss_common.h"
#include "lm_common.h"
#include "config.h"
/*
 * Get the length of the public key, given this particular parameter set
 */
size_t hss_get_public_key_len(unsigned levels,
                   const param_set_t *lm_type,
                   const param_set_t *lm_ots_type) {
    LMS_UNUSED(lm_ots_type);
    if (levels < MIN_HSS_LEVELS || levels > MAX_HSS_LEVELS) return 0;

    size_t first_pubkey = lm_get_public_key_len(lm_type[0]);
    if (first_pubkey == 0) return 0;

    return 4 + first_pubkey;
}

/*
 * Get the length of a signature, given this particular parameter set
 */
size_t hss_get_signature_len(unsigned levels,
                   const param_set_t *lm_type,
                   const param_set_t *lm_ots_type) {
    if (levels < MIN_HSS_LEVELS || levels > MAX_HSS_LEVELS) return 0;

    unsigned i;
    size_t tot_len = 4;
    for (i=0; i<levels; i++) {
        size_t next_sig_len = lm_get_signature_len(lm_type[i], lm_ots_type[i]);
        if (next_sig_len == 0) return 0;
        tot_len += next_sig_len;

        /* We also include the public keys for levels > 0 */
        if (i > 0) {
            size_t next_pub_len = lm_get_public_key_len(lm_type[i]);
            if (next_pub_len == 0) return 0;
            tot_len += next_pub_len;
        }
    }
    return tot_len;
}
