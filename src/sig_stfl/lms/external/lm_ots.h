#if !defined( LM_OTS_H_ )
#define LM_OTS_H_

#include "common_defs.h"
#include <stddef.h>

/*
 * These are routines that implement the OTS signature scheme.  These routines
 * never actually form a "private key"; instead, the signer passes the 'seed'
 * (and public data) to form the public key and to do the actual signature.
 * We do this because the LM routines are actually better suited for doing
 * seed management.
 */
struct seed_derive;

/*
 * Compute the public key.  Note that it doesn't compute a 'private key';
 * the signature algorithm gets that data when we pass the parameters again
 * Parameters:
 * lm_ots_type - The parameter set
 * I - The I public identifier to use
 * q - The diversification string, passed as a 4 byte integer
 * seed - The structure used to generate seeds
 * public_key - Where to place the public key
 * public_key_len - The length of the above buffer
 * This returns true on success
 */
bool lm_ots_generate_public_key(
    param_set_t lm_ots_type,
    const unsigned char *I, /* Public key identifier */
    merkle_index_t q,       /* Diversification string, 4 bytes value */
    struct seed_derive *seed,
    unsigned char *public_key, size_t public_key_len);

/*
 * Sign a message.  Warning: the caller is expected to make sure that it signs
 * only one message with a given seed/I/q set
 * Parameters:
 * lm_ots_type - The parameter set
 * I - The I public identifier to use
 * q - The diversification string, passed as a 4 byte integer
 * seed - The structure used to generate seeds
 * message - Message to sign
 * message_len - Length of the message
 * prehashed - Set if the message hashing has already taken place
 * signature - Where to place the signature
 * signature_len - The length of the above buffer
 * This returns true on success
 */
bool lm_ots_generate_signature(
    param_set_t lm_ots_type,
    const unsigned char *I,
    merkle_index_t q,
    struct seed_derive *seed,
    const void *message, size_t message_len, bool prehashed,
    unsigned char *signature, size_t signature_len);

/* The include file for the verification routine */
#include "lm_ots_verify.h"

/* The include file for the common access routines */
#include "lm_ots_common.h"

#endif /* LM_OTS_H_ */
