// SPDX-License-Identifier: Apache-2.0

#ifndef AESCTR_H
#define AESCTR_H

#include <stddef.h>
#include <stdint.h>

void AES_256_ECB(const uint8_t *input, const uint8_t *key, uint8_t *output);

/* AES-128 ECB over whole blocks. The portable and AES-NI implementations agree
   block for block, so the lambda keystream is backend independent. */
int AES_128_ECB_C(unsigned char *output, const unsigned char *input, size_t nblocks,
                  const unsigned char *key);
#ifdef ENABLE_AESNI
int AES_128_ECB_NI(unsigned char *output, const unsigned char *input, size_t nblocks,
                   const unsigned char *key);
#define AES_128_ECB AES_128_ECB_NI
#else
#define AES_128_ECB AES_128_ECB_C
#endif
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
int AES_128_CTR(unsigned char *output, size_t outputByteLen,
                const unsigned char *input, size_t inputByteLen);
#endif
#endif

#endif

