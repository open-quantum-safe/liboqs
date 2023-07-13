#if !defined(LM_VERIFY_H_)
#define LM_VERIFY_H_

#include <stddef.h>
#include <stdbool.h>

bool lm_validate_signature(
    const unsigned char *public_key,
    const void *message, size_t message_len, bool prehashed,
    const unsigned char *signature, size_t signature_len);

#endif /* LM_VERIFY_H_ */
