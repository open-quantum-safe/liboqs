#if !defined( HSS_VERIFY_H_ )
#define HSS_VERIFY_H_

#include <stdbool.h>

struct hss_extra_info;
/*
 * This is the function to validate a signature; return true if it validates,
 * false if it doesn't
 *
 * public_key is the pointer to the public key
 *
 * message, message_len is the message to validate
 *
 * signature, signature_len is the signature to validate
 */
bool hss_validate_signature(
    const unsigned char *public_key,
    const void *message, size_t message_len,
    const unsigned char *signature, size_t signature_len,
    struct hss_extra_info *info);

#endif /* HSS_VERIFY_H_ */
