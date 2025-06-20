// SPDX-License-Identifier: Apache-2.0

#ifndef AES_H
#define AES_H

#include <stddef.h>
#include <stdint.h>

void AES_256_ECB(const uint8_t *input, const uint8_t *key, uint8_t *output);
#define AES_ECB_encrypt AES_256_ECB

#ifdef ENABLE_AESNI
int AES_128_CTR_NI(unsigned char *output,
                   size_t outputByteLen,
                   const unsigned char *input,
                   size_t inputByteLen);
int AES_128_CTR_4R_NI(unsigned char *output,
                      size_t outputByteLen,
                      const unsigned char *input,
                      size_t inputByteLen);
#define AES_128_CTR AES_128_CTR_NI
#else
int AES_128_CTR(unsigned char *output,
                size_t outputByteLen,
                const unsigned char *input,
                size_t inputByteLen);
#endif

#endif
