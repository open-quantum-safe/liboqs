/*
 *  SPDX-License-Identifier: MIT
 */

#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif

#include <assert.h>
#include <string.h>

#include "owf.h"
#include "aes.h"
#include "utils.h"

#if defined(HAVE_AESNI)
#include "cpu.h"
#include "aesni.h"

ATTR_TARGET_AESNI static void owf_128_aesni(const uint8_t* key, const uint8_t* input,
                                            uint8_t* output) {
  __m128i rk[AES_ROUNDS_128 + 1];
  aes128_expand_key_aesni(rk, key);

  __m128i m = _mm_xor_si128(_mm_loadu_si128((const __m128i_u*)input), rk[0]);
  for (unsigned int round = 1; round != AES_ROUNDS_128; ++round) {
    m = _mm_aesenc_si128(m, rk[round]);
  }
  m = _mm_aesenclast_si128(m, rk[AES_ROUNDS_128]);
  _mm_storeu_si128((__m128i_u*)output, m);
}

ATTR_TARGET_AESNI static void owf_192_aesni(const uint8_t* key, const uint8_t* input,
                                            uint8_t* output) {
  __m128i rk[AES_ROUNDS_192 + 1];
  aes192_expand_key_aesni(rk, key);

  __m128i temp[2];
  temp[1] = _mm_loadu_si128((const __m128i_u*)input);
  temp[0] = _mm_xor_si128(temp[1], rk[0]);
  temp[1] = _mm_xor_si128(temp[0], _mm_setr_epi32(1, 0, 0, 0));
  for (unsigned int round = 1; round != AES_ROUNDS_192; ++round) {
    temp[0] = _mm_aesenc_si128(temp[0], rk[round]);
    temp[1] = _mm_aesenc_si128(temp[1], rk[round]);
  }
  temp[0] = _mm_aesenclast_si128(temp[0], rk[AES_ROUNDS_192]);
  temp[1] = _mm_aesenclast_si128(temp[1], rk[AES_ROUNDS_192]);
  _mm_storeu_si128((__m128i_u*)output, temp[0]);
  _mm_storeu_si128((__m128i_u*)(output + IV_SIZE), temp[1]);
}

ATTR_TARGET_AESNI static void owf_256_aesni(const uint8_t* key, const uint8_t* input,
                                            uint8_t* output) {
  __m128i rk[AES_ROUNDS_256 + 1];
  aes256_expand_key_aesni(rk, key);

  __m128i temp[2];
  temp[1] = _mm_loadu_si128((const __m128i_u*)input);
  temp[0] = _mm_xor_si128(temp[1], rk[0]);
  temp[1] = _mm_xor_si128(temp[0], _mm_setr_epi32(1, 0, 0, 0));
  for (unsigned int round = 1; round != AES_ROUNDS_256; ++round) {
    temp[0] = _mm_aesenc_si128(temp[0], rk[round]);
    temp[1] = _mm_aesenc_si128(temp[1], rk[round]);
  }
  temp[0] = _mm_aesenclast_si128(temp[0], rk[AES_ROUNDS_256]);
  temp[1] = _mm_aesenclast_si128(temp[1], rk[AES_ROUNDS_256]);
  _mm_storeu_si128((__m128i_u*)output, temp[0]);
  _mm_storeu_si128((__m128i_u*)(output + IV_SIZE), temp[1]);
}

ATTR_TARGET_AESNI static void owf_em_128_aesni(const uint8_t* key, const uint8_t* input,
                                               uint8_t* output) {
  __m128i rk[AES_ROUNDS_128 + 1];
  aes128_expand_key_aesni(rk, input);

  __m128i mkey = _mm_loadu_si128((const __m128i_u*)key);
  __m128i m    = _mm_xor_si128(mkey, rk[0]);
  for (unsigned int round = 1; round != AES_ROUNDS_128; ++round) {
    m = _mm_aesenc_si128(m, rk[round]);
  }
  m = _mm_aesenclast_si128(m, rk[AES_ROUNDS_128]);
  m = _mm_xor_si128(m, mkey);
  _mm_storeu_si128((__m128i_u*)output, m);
}

#if defined(HAVE_AVX2)
ATTR_TARGET_AESNI_AVX2 static void owf_128_aesni_avx2(const uint8_t* key, const uint8_t* input,
                                                      uint8_t* output) {
  __m128i rk[AES_ROUNDS_128 + 1];
  aes128_expand_key_aesni_avx2(rk, key);

  __m128i m = _mm_xor_si128(_mm_loadu_si128((const __m128i_u*)input), rk[0]);
  for (unsigned int round = 1; round != AES_ROUNDS_128; ++round) {
    m = _mm_aesenc_si128(m, rk[round]);
  }
  m = _mm_aesenclast_si128(m, rk[AES_ROUNDS_128]);
  _mm_storeu_si128((__m128i_u*)output, m);
}

ATTR_TARGET_AESNI_AVX2 static void owf_192_aesni_avx2(const uint8_t* key, const uint8_t* input,
                                                      uint8_t* output) {
  __m128i rk[AES_ROUNDS_192 + 1];
  aes192_expand_key_aesni_avx2(rk, key);

  __m128i temp[2];
  temp[1] = _mm_loadu_si128((const __m128i_u*)input);
  temp[0] = _mm_xor_si128(temp[1], rk[0]);
  temp[1] = _mm_xor_si128(temp[0], _mm_setr_epi32(1, 0, 0, 0));
  for (unsigned int round = 1; round != AES_ROUNDS_192; ++round) {
    temp[0] = _mm_aesenc_si128(temp[0], rk[round]);
    temp[1] = _mm_aesenc_si128(temp[1], rk[round]);
  }
  temp[0] = _mm_aesenclast_si128(temp[0], rk[AES_ROUNDS_192]);
  temp[1] = _mm_aesenclast_si128(temp[1], rk[AES_ROUNDS_192]);
  _mm_storeu_si128((__m128i_u*)output, temp[0]);
  _mm_storeu_si128((__m128i_u*)(output + IV_SIZE), temp[1]);
}

ATTR_TARGET_AESNI_AVX2 static void owf_256_aesni_avx2(const uint8_t* key, const uint8_t* input,
                                                      uint8_t* output) {
  __m128i rk[AES_ROUNDS_256 + 1];
  aes256_expand_key_aesni_avx2(rk, key);

  __m128i temp[2];
  temp[1] = _mm_loadu_si128((const __m128i_u*)input);
  temp[0] = _mm_xor_si128(temp[1], rk[0]);
  temp[1] = _mm_xor_si128(temp[0], _mm_setr_epi32(1, 0, 0, 0));
  for (unsigned int round = 1; round != AES_ROUNDS_256; ++round) {
    temp[0] = _mm_aesenc_si128(temp[0], rk[round]);
    temp[1] = _mm_aesenc_si128(temp[1], rk[round]);
  }
  temp[0] = _mm_aesenclast_si128(temp[0], rk[AES_ROUNDS_256]);
  temp[1] = _mm_aesenclast_si128(temp[1], rk[AES_ROUNDS_256]);
  _mm_storeu_si128((__m128i_u*)output, temp[0]);
  _mm_storeu_si128((__m128i_u*)(output + IV_SIZE), temp[1]);
}

ATTR_TARGET_AESNI_AVX2 static void owf_em_128_aesni_avx2(const uint8_t* key, const uint8_t* input,
                                                         uint8_t* output) {
  __m128i rk[AES_ROUNDS_128 + 1];
  aes128_expand_key_aesni_avx2(rk, input);

  __m128i mkey = _mm_loadu_si128((const __m128i_u*)key);
  __m128i m    = _mm_xor_si128(mkey, rk[0]);
  for (unsigned int round = 1; round != AES_ROUNDS_128; ++round) {
    m = _mm_aesenc_si128(m, rk[round]);
  }
  m = _mm_aesenclast_si128(m, rk[AES_ROUNDS_128]);
  m = _mm_xor_si128(m, mkey);
  _mm_storeu_si128((__m128i_u*)output, m);
}
#endif
#endif

void owf_128(const uint8_t* key, const uint8_t* input, uint8_t* output) {
#if defined(HAVE_AESNI)
#if defined(HAVE_AVX2)
  if (CPU_SUPPORTS_AESNI_AVX2) {
    owf_128_aesni_avx2(key, input, output);
    return;
  }
#endif
  if (CPU_SUPPORTS_AESNI) {
    owf_128_aesni(key, input, output);
    return;
  }
#endif

  generic_aes_ecb_t ctx;
  int ret = generic_aes_ecb_new(&ctx, key, 128);
  assert(ret == 0);
  (void)ret;

  ret = generic_aes_ecb_encrypt(&ctx, output, input, 1);
  assert(ret == 0);
  (void)ret;

  generic_aes_ecb_free(&ctx);
}

void owf_192(const uint8_t* key, const uint8_t* input, uint8_t* output) {
#if defined(HAVE_AESNI)
#if defined(HAVE_AVX2)
  if (CPU_SUPPORTS_AESNI_AVX2) {
    owf_192_aesni_avx2(key, input, output);
    return;
  }
#endif
  if (CPU_SUPPORTS_AESNI) {
    owf_192_aesni(key, input, output);
    return;
  }
#endif

  generic_aes_ecb_t ctx;
  int ret = generic_aes_ecb_new(&ctx, key, 192);
  assert(ret == 0);
  (void)ret;

  uint8_t buf[2 * IV_SIZE];
  memcpy(buf, input, IV_SIZE);
  memcpy(buf + IV_SIZE, input, IV_SIZE);
  buf[IV_SIZE] ^= 0x1;

  ret = generic_aes_ecb_encrypt(&ctx, output, buf, 2);
  assert(ret == 0);
  (void)ret;

  generic_aes_ecb_free(&ctx);
}

void owf_256(const uint8_t* key, const uint8_t* input, uint8_t* output) {
#if defined(HAVE_AESNI)
#if defined(HAVE_AVX2)
  if (CPU_SUPPORTS_AESNI_AVX2) {
    owf_256_aesni_avx2(key, input, output);
    return;
  }
#endif
  if (CPU_SUPPORTS_AESNI) {
    owf_256_aesni(key, input, output);
    return;
  }
#endif

  generic_aes_ecb_t ctx;
  int ret = generic_aes_ecb_new(&ctx, key, 256);
  assert(ret == 0);
  (void)ret;

  uint8_t buf[2 * IV_SIZE];
  memcpy(buf, input, IV_SIZE);
  memcpy(buf + IV_SIZE, input, IV_SIZE);
  buf[IV_SIZE] ^= 0x1;

  ret = generic_aes_ecb_encrypt(&ctx, output, buf, 2);
  assert(ret == 0);
  (void)ret;

  generic_aes_ecb_free(&ctx);
}

void owf_em_128(const uint8_t* key, const uint8_t* input, uint8_t* output) {
#if defined(HAVE_AESNI)
#if defined(HAVE_AVX2)
  if (CPU_SUPPORTS_AESNI_AVX2) {
    owf_em_128_aesni_avx2(key, input, output);
    return;
  }
#endif
  if (CPU_SUPPORTS_AESNI) {
    owf_em_128_aesni(key, input, output);
    return;
  }
#endif

  // same as owf_128 with swapped keys and the additional xor
  owf_128(input, key, output);
  xor_u8_array_inplace(output, key, 16);
}

void owf_em_192(const uint8_t* key, const uint8_t* input, uint8_t* output) {
  aes_round_keys_t round_keys;
  rijndael192_init_round_keys(&round_keys, input);
  rijndael192_encrypt_block(&round_keys, key, output);
  xor_u8_array_inplace(output, key, 24);
}

void owf_em_256(const uint8_t* key, const uint8_t* input, uint8_t* output) {
  aes_round_keys_t round_keys;
  rijndael256_init_round_keys(&round_keys, input);
  rijndael256_encrypt_block(&round_keys, key, output);
  xor_u8_array_inplace(output, key, 32);
}
