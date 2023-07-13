#if !defined( HSS_RESERVE_H_ )
#define HSS_RESERVE_H_

/*
 * This is the internal include file for the reservation functions for this
 * subsystem. It should not be used by applications
 */

#include "common_defs.h"

struct hss_working_key;

void hss_set_reserve_count(struct hss_working_key *w, sequence_t count);

bool hss_advance_count(struct hss_working_key *w, sequence_t new_count,
        bool (*update_private_key)(unsigned char *private_key,
                size_t len_private_key, void *context),
        void *context,
        struct hss_extra_info *info, bool *trash_private_key);

#endif /* HSS_RESERVE_H_ */
