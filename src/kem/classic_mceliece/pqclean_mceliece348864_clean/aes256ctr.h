#ifndef PQCLEAN_MCELIECE348864_CLEAN_AES256CTR_H
#define PQCLEAN_MCELIECE348864_CLEAN_AES256CTR_H

#include <stddef.h>
#include <stdint.h>

#include "aes.h"

#include "namespace.h"

#define aes256ctr CRYPTO_NAMESPACE(aes256ctr)

void aes256ctr(
    uint8_t *out,
    size_t outlen,
    const uint8_t nonce[AESCTR_NONCEBYTES],
    const uint8_t key[AES256_KEYBYTES]
);

#endif
