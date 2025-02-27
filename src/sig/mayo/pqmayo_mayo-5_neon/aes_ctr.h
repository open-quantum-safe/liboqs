// SPDX-License-Identifier: Apache-2.0

#ifndef AESCTR_H
#define AESCTR_H

#include <stddef.h>
#include <stdint.h>

void AES_256_ECB(const uint8_t *input, const uint8_t *key, uint8_t *output);
#define AES_ECB_encrypt AES_256_ECB

#ifdef ENABLE_AESNI
int AES_128_CTR_NI(unsigned char *output, size_t outputByteLen,
                   const unsigned char *input, size_t inputByteLen);
int AES_128_CTR_4R_NI(unsigned char *output, size_t outputByteLen,
                      const unsigned char *input, size_t inputByteLen);
#define AES_128_CTR AES_128_CTR_NI
#else
#ifdef ENABLE_AESNEON
int AES_128_CTR_NEON(unsigned char *output, size_t outputByteLen,
                   const unsigned char *input, size_t inputByteLen);
int AES_128_CTR_4R_NI(unsigned char *output, size_t outputByteLen,
                      const unsigned char *input, size_t inputByteLen);
#define AES_128_CTR AES_128_CTR_NEON
#else
#include <aes.h>
static inline int AES_128_CTR(unsigned char *output, size_t outputByteLen,
                const unsigned char *input, size_t inputByteLen) {
    (void) inputByteLen;
    uint8_t iv[12] = { 0 };
    aes128ctr_prf(output, outputByteLen, input, iv);
    return (int) outputByteLen;
}
#endif
#endif

#endif

