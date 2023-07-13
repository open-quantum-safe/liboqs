#if !defined( LM_OTS_VERIFY_H_ )
#define LM_OTS_VERIFY_H_

#include <stddef.h>
#include "common_defs.h"

/*
 * This validates an OTS signature, but instead of producing a SUCCESS/FAILURE
 * return, it generates the root value (which the caller is expected to check).
 * It can return false (failure), for things such as unrecognized parameter
 * set It also makes sure that the parameter set of the signature is that
 * value (as we need to make sure that the attacker didn't substitute a
 * weaker one)
 */
bool lm_ots_validate_signature_compute(
    unsigned char *computed_public_key,
    const unsigned char *I,
    merkle_index_t q,       /* Diversification string, 4 bytes value */
    const void *message, size_t message_len, bool prehashed,
    const unsigned char *signature, size_t signature_len,
    param_set_t expected_parameter_set);

#endif /* LM_OTS_VERIFY_H_ */
