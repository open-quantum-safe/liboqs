#ifndef AVX2_X86AESNI_H
#define AVX2_X86AESNI_H

#include <stdint.h>

void AES128_Key_Expansion(unsigned char *key, const unsigned char *userkey);
void AES128_CTR_Stream(unsigned char *out, unsigned long n_16B,
                       const unsigned char *key, const unsigned char nonce[16],
                       uint32_t ctr);

#endif // AVX2_X86AESNI_H
