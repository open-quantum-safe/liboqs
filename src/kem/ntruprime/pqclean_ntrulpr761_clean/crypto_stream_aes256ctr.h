#ifndef PQCLEAN_NTRULPR761_CLEAN_CRYPTO_STREAM_AES256CTR_H
#define PQCLEAN_NTRULPR761_CLEAN_CRYPTO_STREAM_AES256CTR_H
#include "aes.h"
#include <stddef.h>
#include <stdint.h>



int PQCLEAN_NTRULPR761_CLEAN_crypto_stream_aes256ctr(
    uint8_t *out,
    size_t outlen,
    const uint8_t nonce[AESCTR_NONCEBYTES],
    const uint8_t key[AES256_KEYBYTES]);

#endif
