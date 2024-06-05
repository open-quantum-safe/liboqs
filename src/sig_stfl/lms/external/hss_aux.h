// SPDX-License-Identifier: MIT
#if !defined( HSS_AUX_H_ )
#define HSS_AUX_H_

/*
 * This is the internal API to the subsystem that deals with aux data
 * This should not be included by files outside this subsystem
 */

#include "common_defs.h"
#include <stdlib.h>
#include <stdint.h>
#include "lms_namespace.h"

struct hss_working_key;

/* This is a bitmap that lists which aux levels we have */
typedef uint_fast32_t aux_level_t;

/* This is the expanded version of the aux data */
struct expanded_aux_data {
    unsigned char *data[ MAX_MERKLE_HEIGHT+1 ];
};

/*
 * These are some internal routines that handle aux data
 */
/* Internal function used to compute the optimal aux level */
aux_level_t hss_optimal_aux_level( size_t max_length,
                   const param_set_t *lm_type,
                   const param_set_t *lm_ots_type,
                   size_t *actual_len );

/* Generate pointers into a saved aux data */
/* If w is provided, we do sanity checking on the data within aux_data */
struct expanded_aux_data *hss_expand_aux_data( const unsigned char *aux_data,
                   size_t len_aux_data,
                   struct expanded_aux_data *temp, unsigned size_hash,
                   struct hss_working_key *w );

/*
 * Save the marker within the aux data
 */
void hss_store_aux_marker( unsigned char *aux_data, aux_level_t aux_level );

/* Save an intermediate node */
void hss_save_aux_data( struct expanded_aux_data *data, unsigned level,
                        unsigned size_hash, merkle_index_t q,
                        const unsigned char *cur_val );

/* Do the final touches on the aux data */
void hss_finalize_aux_data(struct expanded_aux_data *data,
                           unsigned size_hash, unsigned hash,
                           const unsigned char *seed);

/* Get a set of intermediate nodes from the aux data */
bool hss_extract_aux_data(const struct expanded_aux_data *aux, unsigned level,
            const struct hss_working_key *w, unsigned char *dest,
            merkle_index_t node_offset, merkle_index_t node_count);

#endif /* HSS_AUX_H_ */
