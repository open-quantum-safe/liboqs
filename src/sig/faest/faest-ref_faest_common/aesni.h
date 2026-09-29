/*
 *  SPDX-License-Identifier: MIT
 */

#ifndef FAEST_AESNI_H
#define FAEST_AESNI_H

#include <stdint.h>

#include "macros.h"
#include "compat.h"

#include <wmmintrin.h>
#if defined(HAVE_AVX2)
#include <immintrin.h>
#endif

ATTR_TARGET_AESNI static inline __m128i aes128_keyexpand(__m128i key) {
  key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
  key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
  return _mm_xor_si128(key, _mm_slli_si128(key, 4));
}

ATTR_TARGET_AESNI static inline __m128i aes192_keyexpand_2(__m128i key, __m128i key2) {
  key  = _mm_shuffle_epi32(key, 0xff);
  key2 = _mm_xor_si128(key2, _mm_slli_si128(key2, 4));
  return _mm_xor_si128(key, key2);
}

#define KEYEXP128_H(K1, K2, I, S)                                                                  \
  _mm_xor_si128(aes128_keyexpand(K1), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(K2, I), S))

#define KEYEXP128(K, I) KEYEXP128_H(K, K, I, 0xff)
#define KEYEXP192(K1, K2, I) KEYEXP128_H(K1, K2, I, 0x55)
#define KEYEXP192_2(K1, K2) aes192_keyexpand_2(K1, K2)
#define KEYEXP256(K1, K2, I) KEYEXP128_H(K1, K2, I, 0xff)
#define KEYEXP256_2(K1, K2) KEYEXP128_H(K1, K2, 0x00, 0xaa)

ATTR_TARGET_AESNI static inline void aes128_expand_key_aesni(__m128i* rk, const uint8_t* key) {
  rk[0]  = _mm_loadu_si128((const __m128i_u*)key);
  rk[1]  = KEYEXP128(rk[0], 0x01);
  rk[2]  = KEYEXP128(rk[1], 0x02);
  rk[3]  = KEYEXP128(rk[2], 0x04);
  rk[4]  = KEYEXP128(rk[3], 0x08);
  rk[5]  = KEYEXP128(rk[4], 0x10);
  rk[6]  = KEYEXP128(rk[5], 0x20);
  rk[7]  = KEYEXP128(rk[6], 0x40);
  rk[8]  = KEYEXP128(rk[7], 0x80);
  rk[9]  = KEYEXP128(rk[8], 0x1B);
  rk[10] = KEYEXP128(rk[9], 0x36);
}

ATTR_TARGET_AESNI static inline void aes192_expand_key_aesni(__m128i* rk, const uint8_t* key) {
  __m128i temp[2];
  rk[0]   = _mm_loadu_si128((const __m128i_u*)key);
  rk[1]   = _mm_loadu_si64((const __m128i_u*)(key + 16));
  temp[0] = KEYEXP192(rk[0], rk[1], 0x01);
  temp[1] = KEYEXP192_2(temp[0], rk[1]);
  rk[1]   = _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(rk[1]), _mm_castsi128_pd(temp[0]), 0));
  rk[2] = _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(temp[0]), _mm_castsi128_pd(temp[1]), 1));
  rk[3] = KEYEXP192(temp[0], temp[1], 0x02);
  rk[4] = KEYEXP192_2(rk[3], temp[1]);
  temp[0] = KEYEXP192(rk[3], rk[4], 0x04);
  temp[1] = KEYEXP192_2(temp[0], rk[4]);
  rk[4]   = _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(rk[4]), _mm_castsi128_pd(temp[0]), 0));
  rk[5] = _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(temp[0]), _mm_castsi128_pd(temp[1]), 1));
  rk[6] = KEYEXP192(temp[0], temp[1], 0x08);
  rk[7] = KEYEXP192_2(rk[6], temp[1]);
  temp[0] = KEYEXP192(rk[6], rk[7], 0x10);
  temp[1] = KEYEXP192_2(temp[0], rk[7]);
  rk[7]   = _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(rk[7]), _mm_castsi128_pd(temp[0]), 0));
  rk[8] = _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(temp[0]), _mm_castsi128_pd(temp[1]), 1));
  rk[9] = KEYEXP192(temp[0], temp[1], 0x20);
  rk[10]  = KEYEXP192_2(rk[9], temp[1]);
  temp[0] = KEYEXP192(rk[9], rk[10], 0x40);
  temp[1] = KEYEXP192_2(temp[0], rk[10]);
  rk[10] = _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(rk[10]), _mm_castsi128_pd(temp[0]), 0));
  rk[11] =
      _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(temp[0]), _mm_castsi128_pd(temp[1]), 1));
  rk[12] = KEYEXP192(temp[0], temp[1], 0x80);
}

ATTR_TARGET_AESNI static inline void aes256_expand_key_aesni(__m128i* rk, const uint8_t* key) {
  rk[0]  = _mm_loadu_si128((const __m128i_u*)key);
  rk[1]  = _mm_loadu_si128((const __m128i_u*)(key + 16));
  rk[2]  = KEYEXP256(rk[0], rk[1], 0x01);
  rk[3]  = KEYEXP256_2(rk[1], rk[2]);
  rk[4]  = KEYEXP256(rk[2], rk[3], 0x02);
  rk[5]  = KEYEXP256_2(rk[3], rk[4]);
  rk[6]  = KEYEXP256(rk[4], rk[5], 0x04);
  rk[7]  = KEYEXP256_2(rk[5], rk[6]);
  rk[8]  = KEYEXP256(rk[6], rk[7], 0x08);
  rk[9]  = KEYEXP256_2(rk[7], rk[8]);
  rk[10] = KEYEXP256(rk[8], rk[9], 0x10);
  rk[11] = KEYEXP256_2(rk[9], rk[10]);
  rk[12] = KEYEXP256(rk[10], rk[11], 0x20);
  rk[13] = KEYEXP256_2(rk[11], rk[12]);
  rk[14] = KEYEXP256(rk[12], rk[13], 0x40);
}

#if defined(HAVE_AVX2)
ATTR_TARGET_AESNI_AVX2 static inline void aes128_expand_key_aesni_avx2(__m128i* rk,
                                                                       const uint8_t* key) {
  rk[0]  = _mm_loadu_si128((const __m128i_u*)key);
  rk[1]  = KEYEXP128(rk[0], 0x01);
  rk[2]  = KEYEXP128(rk[1], 0x02);
  rk[3]  = KEYEXP128(rk[2], 0x04);
  rk[4]  = KEYEXP128(rk[3], 0x08);
  rk[5]  = KEYEXP128(rk[4], 0x10);
  rk[6]  = KEYEXP128(rk[5], 0x20);
  rk[7]  = KEYEXP128(rk[6], 0x40);
  rk[8]  = KEYEXP128(rk[7], 0x80);
  rk[9]  = KEYEXP128(rk[8], 0x1B);
  rk[10] = KEYEXP128(rk[9], 0x36);
}

ATTR_TARGET_AESNI_AVX2 static inline void aes192_expand_key_aesni_avx2(__m128i* rk,
                                                                       const uint8_t* key) {
  __m128i temp[2];
  rk[0]   = _mm_loadu_si128((const __m128i_u*)key);
  rk[1]   = _mm_loadu_si64((const __m128i_u*)(key + 16));
  temp[0] = KEYEXP192(rk[0], rk[1], 0x01);
  temp[1] = KEYEXP192_2(temp[0], rk[1]);
  rk[1]   = _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(rk[1]), _mm_castsi128_pd(temp[0]), 0));
  rk[2] = _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(temp[0]), _mm_castsi128_pd(temp[1]), 1));
  rk[3] = KEYEXP192(temp[0], temp[1], 0x02);
  rk[4] = KEYEXP192_2(rk[3], temp[1]);
  temp[0] = KEYEXP192(rk[3], rk[4], 0x04);
  temp[1] = KEYEXP192_2(temp[0], rk[4]);
  rk[4]   = _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(rk[4]), _mm_castsi128_pd(temp[0]), 0));
  rk[5] = _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(temp[0]), _mm_castsi128_pd(temp[1]), 1));
  rk[6] = KEYEXP192(temp[0], temp[1], 0x08);
  rk[7] = KEYEXP192_2(rk[6], temp[1]);
  temp[0] = KEYEXP192(rk[6], rk[7], 0x10);
  temp[1] = KEYEXP192_2(temp[0], rk[7]);
  rk[7]   = _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(rk[7]), _mm_castsi128_pd(temp[0]), 0));
  rk[8] = _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(temp[0]), _mm_castsi128_pd(temp[1]), 1));
  rk[9] = KEYEXP192(temp[0], temp[1], 0x20);
  rk[10]  = KEYEXP192_2(rk[9], temp[1]);
  temp[0] = KEYEXP192(rk[9], rk[10], 0x40);
  temp[1] = KEYEXP192_2(temp[0], rk[10]);
  rk[10] = _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(rk[10]), _mm_castsi128_pd(temp[0]), 0));
  rk[11] =
      _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(temp[0]), _mm_castsi128_pd(temp[1]), 1));
  rk[12] = KEYEXP192(temp[0], temp[1], 0x80);
}

ATTR_TARGET_AESNI_AVX2 static inline void aes256_expand_key_aesni_avx2(__m128i* rk,
                                                                       const uint8_t* key) {
  rk[0]  = _mm_loadu_si128((const __m128i_u*)key);
  rk[1]  = _mm_loadu_si128((const __m128i_u*)(key + 16));
  rk[2]  = KEYEXP256(rk[0], rk[1], 0x01);
  rk[3]  = KEYEXP256_2(rk[1], rk[2]);
  rk[4]  = KEYEXP256(rk[2], rk[3], 0x02);
  rk[5]  = KEYEXP256_2(rk[3], rk[4]);
  rk[6]  = KEYEXP256(rk[4], rk[5], 0x04);
  rk[7]  = KEYEXP256_2(rk[5], rk[6]);
  rk[8]  = KEYEXP256(rk[6], rk[7], 0x08);
  rk[9]  = KEYEXP256_2(rk[7], rk[8]);
  rk[10] = KEYEXP256(rk[8], rk[9], 0x10);
  rk[11] = KEYEXP256_2(rk[9], rk[10]);
  rk[12] = KEYEXP256(rk[10], rk[11], 0x20);
  rk[13] = KEYEXP256_2(rk[11], rk[12]);
  rk[14] = KEYEXP256(rk[12], rk[13], 0x40);
}
#endif

#endif
