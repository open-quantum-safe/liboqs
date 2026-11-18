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
#include "cpu.h"

#if defined(HAVE_ARM_AES)
#include "aes_arm.h"
#endif

#if defined(HAVE_AESNI)
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

/* Two vectors per round; Rijndael-192 uses only the low half of the second. */
ATTR_TARGET_AESNI static void rijndael192_init_round_keys_aesni(__m128i* rk, const uint8_t* key) {
  __m128i first = _mm_loadu_si128((const __m128i_u*)key);
  __m128i tail  = _mm_loadu_si64(key + 16);
  rk[0]         = first;
  rk[1]         = tail;
  for (unsigned int r = 1; r <= AES_ROUNDS_192; ++r) {
    const __m128i core = _mm_xor_si128(_mm_shuffle_epi32(_mm_aeskeygenassist_si128(tail, 0), 0x55),
                                       _mm_set1_epi32(aes_round_constants[r - 1]));
    first              = _mm_xor_si128(aes128_keyexpand(first), core);
    tail =
        _mm_xor_si128(_mm_xor_si128(tail, _mm_slli_si128(tail, 4)), _mm_shuffle_epi32(first, 0xff));
    rk[2 * r]     = first;
    rk[2 * r + 1] = tail;
  }
}

ATTR_TARGET_AESNI static void rijndael256_init_round_keys_aesni(__m128i* rk, const uint8_t* key) {
  __m128i first  = _mm_loadu_si128((const __m128i_u*)key);
  __m128i second = _mm_loadu_si128((const __m128i_u*)(key + 16));
  rk[0]          = first;
  rk[1]          = second;
  for (unsigned int r = 1; r <= AES_ROUNDS_256; ++r) {
    const __m128i core =
        _mm_xor_si128(_mm_shuffle_epi32(_mm_aeskeygenassist_si128(second, 0), 0xff),
                      _mm_set1_epi32(aes_round_constants[r - 1]));
    first         = _mm_xor_si128(aes128_keyexpand(first), core);
    second        = _mm_xor_si128(aes128_keyexpand(second),
                                  _mm_shuffle_epi32(_mm_aeskeygenassist_si128(first, 0), 0xaa));
    rk[2 * r]     = first;
    rk[2 * r + 1] = second;
  }
}

/* Pre-permute the two registers so AESENC's four-column ShiftRows becomes
 * Rijndael's six-column ShiftRows. Only the low two columns of second are live.
 * The first register's rows select columns (0,1,2,3), (4,1,2,3),
 * (4,5,2,3), and (4,5,0,3), respectively, before AES ShiftRows. */
ATTR_TARGET_AESNI static inline void rijndael192_permute(__m128i* first, __m128i* second) {
  const __m128i a    = *first;
  const __m128i b    = *second;
  const __m128i high = _mm_slli_si128(a, 8);

  *first = _mm_or_si128(
      _mm_and_si128(a, _mm_set_epi32(-1, 0x00ffffff, 0x0000ffff, 0x000000ff)),
      _mm_or_si128(_mm_and_si128(b, _mm_set_epi32(0, 0, (int)0xffff0000u, (int)0xffffff00u)),
                   _mm_and_si128(high, _mm_set_epi32(0, (int)0xff000000u, 0, 0))));
  *second = _mm_or_si128(
      _mm_and_si128(b, _mm_set_epi32(0, 0, 0x0000ffff, 0x000000ff)),
      _mm_or_si128(_mm_and_si128(high, _mm_set_epi32((int)0xffff0000u, 0x00ffff00, 0, 0)),
                   _mm_and_si128(_mm_srli_si128(a, 8), _mm_set_epi32(0, 0, 0, (int)0xff000000u))));
}

/* Rijndael-256 shifts rows by (0, 1, 3, 4). Exchange selected bytes between
 * halves, then rotate the last two rows by one column before AESENC. */
ATTR_TARGET_AESNI static inline void rijndael256_permute(__m128i* first, __m128i* second) {
  const __m128i swap = _mm_and_si128(
      _mm_xor_si128(*first, *second),
      _mm_set_epi32((int)0xff000000u, (int)0xffff0000u, (int)0xffff0000u, (int)0xffffff00u));
  const __m128i a        = _mm_xor_si128(*first, swap);
  const __m128i b        = _mm_xor_si128(*second, swap);
  const __m128i low_rows = _mm_set1_epi32(0x0000ffff);

  *first  = _mm_or_si128(_mm_and_si128(a, low_rows),
                         _mm_andnot_si128(low_rows, _mm_shuffle_epi32(a, 0x39)));
  *second = _mm_or_si128(_mm_and_si128(b, low_rows),
                         _mm_andnot_si128(low_rows, _mm_shuffle_epi32(b, 0x39)));
}

ATTR_TARGET_AESNI static void
rijndael192_encrypt_block_aesni(const __m128i* rk, const uint8_t* plaintext, uint8_t* ciphertext) {
  __m128i first  = _mm_xor_si128(_mm_loadu_si128((const __m128i_u*)plaintext), rk[0]);
  __m128i second = _mm_xor_si128(_mm_loadu_si64(plaintext + 16), rk[1]);
  for (unsigned int r = 1; r < AES_ROUNDS_192; ++r) {
    rijndael192_permute(&first, &second);
    first  = _mm_aesenc_si128(first, rk[2 * r]);
    second = _mm_aesenc_si128(second, rk[2 * r + 1]);
  }

  rijndael192_permute(&first, &second);
  first  = _mm_aesenclast_si128(first, rk[2 * AES_ROUNDS_192]);
  second = _mm_aesenclast_si128(second, rk[2 * AES_ROUNDS_192 + 1]);
  _mm_storeu_si128((__m128i_u*)ciphertext, first);
  _mm_storel_epi64((__m128i*)(ciphertext + 16), second);
}

ATTR_TARGET_AESNI static void
rijndael256_encrypt_block_aesni(const __m128i* rk, const uint8_t* plaintext, uint8_t* ciphertext) {
  __m128i first  = _mm_xor_si128(_mm_loadu_si128((const __m128i_u*)plaintext), rk[0]);
  __m128i second = _mm_xor_si128(_mm_loadu_si128((const __m128i_u*)(plaintext + 16)), rk[1]);
  for (unsigned int r = 1; r < AES_ROUNDS_256; ++r) {

    rijndael256_permute(&first, &second);
    first  = _mm_aesenc_si128(first, rk[2 * r]);
    second = _mm_aesenc_si128(second, rk[2 * r + 1]);
  }
  rijndael256_permute(&first, &second);
  first  = _mm_aesenclast_si128(first, rk[2 * AES_ROUNDS_256]);
  second = _mm_aesenclast_si128(second, rk[2 * AES_ROUNDS_256 + 1]);
  _mm_storeu_si128((__m128i_u*)ciphertext, first);
  _mm_storeu_si128((__m128i_u*)(ciphertext + 16), second);
}

ATTR_TARGET_AESNI static void owf_em_192_aesni(const uint8_t* key, const uint8_t* input,
                                               uint8_t* output) {
  __m128i round_keys[2 * (AES_ROUNDS_192 + 1)];
  rijndael192_init_round_keys_aesni(round_keys, input);
  rijndael192_encrypt_block_aesni(round_keys, key, output);
  xor_u8_array_inplace(output, key, 24);
}

ATTR_TARGET_AESNI static void owf_em_256_aesni(const uint8_t* key, const uint8_t* input,
                                               uint8_t* output) {
  __m128i round_keys[2 * (AES_ROUNDS_256 + 1)];
  rijndael256_init_round_keys_aesni(round_keys, input);
  rijndael256_encrypt_block_aesni(round_keys, key, output);
  xor_u8_array_inplace(output, key, 32);
}

#if defined(HAVE_AVX2)
ATTR_TARGET_AESNI_AVX2 static void rijndael192_init_round_keys_aesni_avx2(__m128i* rk,
                                                                          const uint8_t* key) {
  __m128i first = _mm_loadu_si128((const __m128i_u*)key);
  __m128i tail  = _mm_loadu_si64(key + 16);
  rk[0]         = first;
  rk[1]         = tail;
  for (unsigned int r = 1; r <= AES_ROUNDS_192; ++r) {
    const __m128i core = _mm_xor_si128(_mm_shuffle_epi32(_mm_aeskeygenassist_si128(tail, 0), 0x55),
                                       _mm_set1_epi32(aes_round_constants[r - 1]));
    first              = _mm_xor_si128(aes128_keyexpand(first), core);
    tail =
        _mm_xor_si128(_mm_xor_si128(tail, _mm_slli_si128(tail, 4)), _mm_shuffle_epi32(first, 0xff));
    rk[2 * r]     = first;
    rk[2 * r + 1] = tail;
  }
}

ATTR_TARGET_AESNI_AVX2 static void rijndael256_init_round_keys_aesni_avx2(__m128i* rk,
                                                                          const uint8_t* key) {
  __m128i first  = _mm_loadu_si128((const __m128i_u*)key);
  __m128i second = _mm_loadu_si128((const __m128i_u*)(key + 16));
  rk[0]          = first;
  rk[1]          = second;
  for (unsigned int r = 1; r <= AES_ROUNDS_256; ++r) {
    const __m128i core =
        _mm_xor_si128(_mm_shuffle_epi32(_mm_aeskeygenassist_si128(second, 0), 0xff),
                      _mm_set1_epi32(aes_round_constants[r - 1]));
    first         = _mm_xor_si128(aes128_keyexpand(first), core);
    second        = _mm_xor_si128(aes128_keyexpand(second),
                                  _mm_shuffle_epi32(_mm_aeskeygenassist_si128(first, 0), 0xaa));
    rk[2 * r]     = first;
    rk[2 * r + 1] = second;
  }
}

/* Byte shuffles perform the within-register correction; blends select bytes
 * crossing the 128-bit halves. The unused Rijndael-192 bytes are zero-filled. */
ATTR_TARGET_AESNI_AVX2 static inline void rijndael192_permute_avx2(__m128i* first,
                                                                   __m128i* second) {
  const __m128i a           = *first;
  const __m128i b           = *second;
  const __m128i first_order = _mm_setr_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 3, 12, 13, 14, 15);
  const __m128i second_order =
      _mm_setr_epi8(-1, -1, -1, 11, -1, -1, -1, -1, -1, 1, 2, -1, -1, -1, 6, 7);

  *first  = _mm_blendv_epi8(_mm_shuffle_epi8(a, first_order), b,
                            _mm_set_epi32(0, 0, (int)0xffff0000u, (int)0xffffff00u));
  *second = _mm_blendv_epi8(_mm_shuffle_epi8(a, second_order), b,
                            _mm_set_epi32(0, 0, 0x0000ffff, 0x000000ff));
}

ATTR_TARGET_AESNI_AVX2 static inline void rijndael256_permute_avx2(__m128i* first,
                                                                   __m128i* second) {
  const __m128i order = _mm_setr_epi8(0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13, 2, 3);
  const __m128i cross =
      _mm_set_epi32((int)0xffff0000u, (int)0xff000000u, (int)0xffff0000u, (int)0xffffff00u);
  const __m128i a = _mm_shuffle_epi8(*first, order);
  const __m128i b = _mm_shuffle_epi8(*second, order);

  *first  = _mm_blendv_epi8(a, b, cross);
  *second = _mm_blendv_epi8(b, a, cross);
}

ATTR_TARGET_AESNI_AVX2 static void rijndael192_encrypt_block_aesni_avx2(const __m128i* rk,
                                                                        const uint8_t* plaintext,
                                                                        uint8_t* ciphertext) {
  __m128i first  = _mm_xor_si128(_mm_loadu_si128((const __m128i_u*)plaintext), rk[0]);
  __m128i second = _mm_xor_si128(_mm_loadu_si64(plaintext + 16), rk[1]);
  for (unsigned int r = 1; r < AES_ROUNDS_192; ++r) {
    rijndael192_permute_avx2(&first, &second);
    first  = _mm_aesenc_si128(first, rk[2 * r]);
    second = _mm_aesenc_si128(second, rk[2 * r + 1]);
  }

  rijndael192_permute_avx2(&first, &second);
  first  = _mm_aesenclast_si128(first, rk[2 * AES_ROUNDS_192]);
  second = _mm_aesenclast_si128(second, rk[2 * AES_ROUNDS_192 + 1]);
  _mm_storeu_si128((__m128i_u*)ciphertext, first);
  _mm_storel_epi64((__m128i*)(ciphertext + 16), second);
}

ATTR_TARGET_AESNI_AVX2 static void rijndael256_encrypt_block_aesni_avx2(const __m128i* rk,
                                                                        const uint8_t* plaintext,
                                                                        uint8_t* ciphertext) {
  __m128i first  = _mm_xor_si128(_mm_loadu_si128((const __m128i_u*)plaintext), rk[0]);
  __m128i second = _mm_xor_si128(_mm_loadu_si128((const __m128i_u*)(plaintext + 16)), rk[1]);
  for (unsigned int r = 1; r < AES_ROUNDS_256; ++r) {

    rijndael256_permute_avx2(&first, &second);
    first  = _mm_aesenc_si128(first, rk[2 * r]);
    second = _mm_aesenc_si128(second, rk[2 * r + 1]);
  }

  rijndael256_permute_avx2(&first, &second);
  first  = _mm_aesenclast_si128(first, rk[2 * AES_ROUNDS_256]);
  second = _mm_aesenclast_si128(second, rk[2 * AES_ROUNDS_256 + 1]);
  _mm_storeu_si128((__m128i_u*)ciphertext, first);
  _mm_storeu_si128((__m128i_u*)(ciphertext + 16), second);
}

ATTR_TARGET_AESNI_AVX2 static void owf_em_192_aesni_avx2(const uint8_t* key, const uint8_t* input,
                                                         uint8_t* output) {
  __m128i round_keys[2 * (AES_ROUNDS_192 + 1)];
  rijndael192_init_round_keys_aesni_avx2(round_keys, input);
  rijndael192_encrypt_block_aesni_avx2(round_keys, key, output);
  xor_u8_array_inplace(output, key, 24);
}

ATTR_TARGET_AESNI_AVX2 static void owf_em_256_aesni_avx2(const uint8_t* key, const uint8_t* input,
                                                         uint8_t* output) {
  __m128i round_keys[2 * (AES_ROUNDS_256 + 1)];
  rijndael256_init_round_keys_aesni_avx2(round_keys, input);
  rijndael256_encrypt_block_aesni_avx2(round_keys, key, output);
  xor_u8_array_inplace(output, key, 32);
}

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

#if defined(HAVE_ARM_AES)
ATTR_TARGET_ARM_AES static void owf_arm_aes(const uint8_t* key, const uint8_t* input,
                                            uint8_t* output, unsigned int key_words) {
  const unsigned int rounds = key_words + 6;
  const unsigned int blocks = key_words == 4 ? 1 : 2;
  uint8x16_t rk[AES_MAX_ROUNDS + 1];
  arm_aes_expand_key(rk, key, key_words, rounds);

  uint8x16_t states[2];
  states[0] = vld1q_u8(input);
  if (blocks == 2) {
    const uint8_t flip[IV_SIZE] = {1};
    states[1]                   = veorq_u8(states[0], vld1q_u8(flip));
  }
  arm_aes_encrypt_blocks(states, blocks, rk, rounds);
  for (unsigned int i = 0; i < blocks; ++i) {
    vst1q_u8(output + i * IV_SIZE, states[i]);
  }
}

ATTR_TARGET_ARM_AES static void owf_em_128_arm_aes(const uint8_t* key, const uint8_t* input,
                                                   uint8_t* output) {
  uint8x16_t rk[AES_ROUNDS_128 + 1];
  arm_aes_expand_key(rk, input, 4, AES_ROUNDS_128);

  const uint8x16_t mkey = vld1q_u8(key);
  uint8x16_t state      = mkey;
  arm_aes_encrypt_blocks(&state, 1, rk, AES_ROUNDS_128);
  vst1q_u8(output, veorq_u8(state, mkey));
}

/* Each Rijndael round key occupies two vectors. For the 192-bit schedule,
 * repeat the last two words so arm_aes_key_core sees the last valid word. */
ATTR_TARGET_ARM_AES static void rijndael192_init_round_keys_arm_aes(uint8x16_t* rk,
                                                                    const uint8_t* key) {
  uint8x16_t first = rk[0] = vld1q_u8(key);
  const uint8x8_t last     = vld1_u8(key + IV_SIZE);
  uint8x16_t tail = rk[1] = vcombine_u8(last, last);
  for (unsigned int r = 1; r <= AES_ROUNDS_192; ++r) {
    first = veorq_u8(arm_aes_key_prefix(first), arm_aes_key_core(tail, aes_round_constants[r - 1]));
    tail  = veorq_u8(arm_aes_key_prefix(tail), arm_aes_last_word(first));
    tail  = vcombine_u8(vget_low_u8(tail), vget_low_u8(tail));
    rk[2 * r]     = first;
    rk[2 * r + 1] = tail;
  }
}

ATTR_TARGET_ARM_AES static void rijndael256_init_round_keys_arm_aes(uint8x16_t* rk,
                                                                    const uint8_t* key) {
  uint8x16_t first = rk[0] = vld1q_u8(key);
  uint8x16_t second = rk[1] = vld1q_u8(key + IV_SIZE);
  for (unsigned int r = 1; r <= AES_ROUNDS_256; ++r) {
    first =
        veorq_u8(arm_aes_key_prefix(first), arm_aes_key_core(second, aes_round_constants[r - 1]));
    const uint8x16_t subword = vaeseq_u8(arm_aes_last_word(first), vdupq_n_u8(0));
    second                   = veorq_u8(arm_aes_key_prefix(second), subword);
    rk[2 * r]                = first;
    rk[2 * r + 1]            = second;
  }
}

/* AESE adds the key before SubBytes/ShiftRows. Correct its ShiftRows afterwards,
 * selecting across both registers, so the key schedule stays in its native order. */
ATTR_TARGET_ARM_AES static inline uint8x16x2_t
rijndael_permute_arm_aes(uint8x16x2_t state, uint8x16_t first_order, uint8x16_t second_order) {
  uint8x16x2_t result;
#if defined(__aarch64__) || defined(_M_ARM64) || defined(_M_ARM64EC)
  result.val[0] = vqtbl2q_u8(state, first_order);
  result.val[1] = vqtbl2q_u8(state, second_order);
#else
  const uint8x8x4_t table = {{vget_low_u8(state.val[0]), vget_high_u8(state.val[0]),
                              vget_low_u8(state.val[1]), vget_high_u8(state.val[1])}};
  result.val[0]           = vcombine_u8(vtbl4_u8(table, vget_low_u8(first_order)),
                                        vtbl4_u8(table, vget_high_u8(first_order)));
  result.val[1]           = vcombine_u8(vtbl4_u8(table, vget_low_u8(second_order)),
                                        vtbl4_u8(table, vget_high_u8(second_order)));
#endif
  return result;
}

ATTR_TARGET_ARM_AES static inline uint8x16x2_t
rijndael_encrypt_block_arm_aes(const uint8x16_t* rk, uint8x16x2_t state, unsigned int rounds,
                               uint8x16_t first_order, uint8x16_t second_order) {
  for (unsigned int r = 0; r < rounds - 1; ++r) {
    state.val[0] = vaeseq_u8(state.val[0], rk[2 * r]);
    state.val[1] = vaeseq_u8(state.val[1], rk[2 * r + 1]);
    state        = rijndael_permute_arm_aes(state, first_order, second_order);
    state.val[0] = vaesmcq_u8(state.val[0]);
    state.val[1] = vaesmcq_u8(state.val[1]);
  }

  state.val[0] = vaeseq_u8(state.val[0], rk[2 * (rounds - 1)]);
  state.val[1] = vaeseq_u8(state.val[1], rk[2 * (rounds - 1) + 1]);
  state        = rijndael_permute_arm_aes(state, first_order, second_order);
  state.val[0] = veorq_u8(state.val[0], rk[2 * rounds]);
  state.val[1] = veorq_u8(state.val[1], rk[2 * rounds + 1]);
  return state;
}

ATTR_TARGET_ARM_AES static void owf_em_192_arm_aes(const uint8_t* key, const uint8_t* input,
                                                   uint8_t* output) {
  const uint8_t first_indices[IV_SIZE]  = {0, 1, 2, 3, 4, 5, 6, 23, 8, 9, 26, 27, 12, 29, 30, 7};
  const uint8_t second_indices[IV_SIZE] = {16,  17,  10,  11,  20,  13,  14,  15,
                                           255, 255, 255, 255, 255, 255, 255, 255};

  uint8x16_t rk[2 * (AES_ROUNDS_192 + 1)];
  rijndael192_init_round_keys_arm_aes(rk, input);

  const uint8x16x2_t mkey  = {{vld1q_u8(key), vcombine_u8(vld1_u8(key + IV_SIZE), vdup_n_u8(0))}};
  const uint8x16x2_t state = rijndael_encrypt_block_arm_aes(
      rk, mkey, AES_ROUNDS_192, vld1q_u8(first_indices), vld1q_u8(second_indices));

  vst1q_u8(output, veorq_u8(state.val[0], mkey.val[0]));
  vst1_u8(output + IV_SIZE, vget_low_u8(veorq_u8(state.val[1], mkey.val[1])));
}

ATTR_TARGET_ARM_AES static void owf_em_256_arm_aes(const uint8_t* key, const uint8_t* input,
                                                   uint8_t* output) {
  const uint8_t first_indices[IV_SIZE]  = {0, 1, 6, 23, 4, 5, 26, 27, 8, 9, 30, 31, 12, 29, 18, 19};
  const uint8_t second_indices[IV_SIZE] = {16, 17, 22, 7,  20, 21, 10, 11,
                                           24, 25, 14, 15, 28, 13, 2,  3};

  uint8x16_t rk[2 * (AES_ROUNDS_256 + 1)];
  rijndael256_init_round_keys_arm_aes(rk, input);

  const uint8x16x2_t mkey  = {{vld1q_u8(key), vld1q_u8(key + IV_SIZE)}};
  const uint8x16x2_t state = rijndael_encrypt_block_arm_aes(
      rk, mkey, AES_ROUNDS_256, vld1q_u8(first_indices), vld1q_u8(second_indices));

  vst1q_u8(output, veorq_u8(state.val[0], mkey.val[0]));
  vst1q_u8(output + IV_SIZE, veorq_u8(state.val[1], mkey.val[1]));
}
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

#if defined(HAVE_ARM_AES)
  if (CPU_SUPPORTS_ARM_AES) {
    owf_arm_aes(key, input, output, 4);
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

#if defined(HAVE_ARM_AES)
  if (CPU_SUPPORTS_ARM_AES) {
    owf_arm_aes(key, input, output, 6);
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

#if defined(HAVE_ARM_AES)
  if (CPU_SUPPORTS_ARM_AES) {
    owf_arm_aes(key, input, output, 8);
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

#if defined(HAVE_ARM_AES)
  if (CPU_SUPPORTS_ARM_AES) {
    owf_em_128_arm_aes(key, input, output);
    return;
  }
#endif

  // same as owf_128 with swapped keys and the additional xor
  owf_128(input, key, output);
  xor_u8_array_inplace(output, key, 16);
}

void owf_em_192(const uint8_t* key, const uint8_t* input, uint8_t* output) {
#if defined(HAVE_AESNI)
#if defined(HAVE_AVX2)
  if (CPU_SUPPORTS_AESNI_AVX2) {
    owf_em_192_aesni_avx2(key, input, output);
    return;
  }
#endif
  if (CPU_SUPPORTS_AESNI) {
    owf_em_192_aesni(key, input, output);
    return;
  }
#endif

#if defined(HAVE_ARM_AES)
  if (CPU_SUPPORTS_ARM_AES) {
    owf_em_192_arm_aes(key, input, output);
    return;
  }
#endif

  aes_round_keys_t round_keys;
  rijndael192_init_round_keys(&round_keys, input);
  rijndael192_encrypt_block(&round_keys, key, output);
  xor_u8_array_inplace(output, key, 24);
}

void owf_em_256(const uint8_t* key, const uint8_t* input, uint8_t* output) {
#if defined(HAVE_AESNI)
#if defined(HAVE_AVX2)
  if (CPU_SUPPORTS_AESNI_AVX2) {
    owf_em_256_aesni_avx2(key, input, output);
    return;
  }
#endif
  if (CPU_SUPPORTS_AESNI) {
    owf_em_256_aesni(key, input, output);
    return;
  }
#endif

#if defined(HAVE_ARM_AES)
  if (CPU_SUPPORTS_ARM_AES) {
    owf_em_256_arm_aes(key, input, output);
    return;
  }
#endif

  aes_round_keys_t round_keys;
  rijndael256_init_round_keys(&round_keys, input);
  rijndael256_encrypt_block(&round_keys, key, output);
  xor_u8_array_inplace(output, key, 32);
}
