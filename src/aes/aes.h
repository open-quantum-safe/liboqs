#ifndef __OQS_AES_H
#define __OQS_AES_H

#include <stdint.h>
#include <wmmintrin.h>

void OQS_AES128_load_schedule(uint8_t *key, __m128i *schedule);
void OQS_AES128_enc(uint8_t *plainText, __m128i *schedule, uint8_t *cipher_text);
void OQS_AES128_dec(uint8_t *cipher_text, __m128i *schedule, uint8_t *plain_text);

#endif
