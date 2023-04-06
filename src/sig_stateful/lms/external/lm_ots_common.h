#if !defined( LM_OTS_COMMON_H_ )
#define LM_OTS_COMMON_H_

#include <stddef.h>
#include "common_defs.h"

bool lm_ots_look_up_parameter_set(param_set_t parameter_set,
     unsigned *h, unsigned *n, unsigned *w, unsigned *p, unsigned *ls);
size_t lm_ots_get_public_key_len(param_set_t lm_ots_type);
size_t lm_ots_get_signature_len(param_set_t lm_ots_type);
unsigned lm_ots_hashes_per_public_key(param_set_t lm_ots_type);
unsigned lm_ots_compute_checksum(const unsigned char *Q, unsigned Q_len,
                                 unsigned w, unsigned ls);
unsigned lm_ots_coef(const unsigned char *Q, unsigned i, unsigned w);

#endif /* LM_OTS_COMMON_H_ */
