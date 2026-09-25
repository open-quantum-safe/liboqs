/*
 *  SPDX-License-Identifier: MIT
 */

#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif

#if defined(_WIN32) && !defined(HAVE_OPENSSL)
#if !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x0601
#elif _WIN32_WINNT < 0x0601
#error "At least Windows 7 is required."
#endif
#endif

#include "aes.h"

#include "cpu.h"
#include "fields.h"
#include "compat.h"
#include "utils.h"

#if defined(HAVE_AESNI)
#include "aesni.h"
#endif
#if defined(HAVE_OPENSSL)
#include <openssl/evp.h>
#elif defined(_WIN32)
#include <windows.h>
#endif
#include <string.h>

#define KEY_WORDS_128 4
#define KEY_WORDS_192 6
#define KEY_WORDS_256 8

#define AES_BLOCK_WORDS 4
#define RIJNDAEL_BLOCK_WORDS_192 6
#define RIJNDAEL_BLOCK_WORDS_256 8

static const bf8_t round_constants[30] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
    0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91,
};

static bf8_t compute_sbox(bf8_t in) {
  const bf8_t t = bf8_inv(in);
  return t ^ rotl8(t, 1) ^ rotl8(t, 2) ^ rotl8(t, 3) ^ rotl8(t, 4) ^ UINT8_C(0x63);
}

#if defined(FAEST_TESTS)
void aes_increment_iv(uint8_t* iv)
#else
static inline void aes_increment_iv(uint8_t* iv)
#endif
{
  uint32_t iv0;
  memcpy(&iv0, iv, sizeof(uint32_t));
  iv0 = htole32(le32toh(iv0) + 1);
  memcpy(iv, &iv0, sizeof(uint32_t));
}

// ## AES ##
// Round Functions
static void add_round_key(unsigned int round, aes_block_t state, const aes_round_keys_t* round_key,
                          unsigned int block_words) {
  for (unsigned int c = 0; c < block_words; c++) {
    xor_small_u8_array_inplace(&state[c][0], &round_key->round_keys[round][c][0], AES_NR);
  }
}

static void sub_bytes(aes_block_t state, unsigned int block_words) {
  for (unsigned int c = 0; c < block_words; c++) {
    for (unsigned int r = 0; r < AES_NR; r++) {
      state[c][r] = compute_sbox(state[c][r]);
    }
  }
}

static void shift_row(aes_block_t state, unsigned int block_words) {
  aes_block_t new_state;
  switch (block_words) {
  case 4:
  case 6:
    for (unsigned int i = 0; i < block_words; ++i) {
      new_state[i][0] = state[i][0];
      new_state[i][1] = state[(i + 1) % block_words][1];
      new_state[i][2] = state[(i + 2) % block_words][2];
      new_state[i][3] = state[(i + 3) % block_words][3];
    }
    break;
  case 8:
    for (unsigned int i = 0; i < block_words; i++) {
      new_state[i][0] = state[i][0];
      new_state[i][1] = state[(i + 1) % 8][1];
      new_state[i][2] = state[(i + 3) % 8][2];
      new_state[i][3] = state[(i + 4) % 8][3];
    }
    break;
  }

  for (unsigned int i = 0; i < block_words; ++i) {
    memcpy(&state[i][0], &new_state[i][0], AES_NR);
  }
}

static void mix_column(aes_block_t state, unsigned int block_words) {
  for (unsigned int c = 0; c < block_words; c++) {
    bf8_t tmp[4];
    tmp[0] = bf8_mul(state[c][0], 0x02) ^ bf8_mul(state[c][1], 0x03) ^ state[c][2] ^ state[c][3];
    tmp[1] = state[c][0] ^ bf8_mul(state[c][1], 0x02) ^ bf8_mul(state[c][2], 0x03) ^ state[c][3];
    tmp[2] = state[c][0] ^ state[c][1] ^ bf8_mul(state[c][2], 0x02) ^ bf8_mul(state[c][3], 0x03);
    tmp[3] = bf8_mul(state[c][0], 0x03) ^ state[c][1] ^ state[c][2] ^ bf8_mul(state[c][3], 0x02);

    memcpy(state[c], tmp, sizeof(tmp));
  }
}

// Key Expansion functions
static void sub_words(bf8_t* words) {
  words[0] = compute_sbox(words[0]);
  words[1] = compute_sbox(words[1]);
  words[2] = compute_sbox(words[2]);
  words[3] = compute_sbox(words[3]);
}

static inline void rot_word(bf8_t* words) {
  uint32_t w;
  memcpy(&w, words, sizeof(w));
#if defined(FAEST_IS_LITTLE_ENDIAN)
  w = rotr32(w, 8);
#else
  w = htole32(rotr32(le32toh(w), 8));
#endif
  memcpy(words, &w, sizeof(w));
}

void expand_key(aes_round_keys_t* round_keys, const uint8_t* key, unsigned int key_words,
                unsigned int block_words, unsigned int num_rounds) {
  for (unsigned int k = 0; k < key_words; k++) {
    memcpy(round_keys->round_keys[k / block_words][k % block_words], &key[4 * k], 4);
  }

  for (unsigned int k = key_words; k < block_words * (num_rounds + 1); ++k) {
    bf8_t tmp[AES_NR];
    memcpy(tmp, round_keys->round_keys[(k - 1) / block_words][(k - 1) % block_words], sizeof(tmp));

    if (k % key_words == 0) {
      rot_word(tmp);
      sub_words(tmp);
      tmp[0] ^= round_constants[(k / key_words) - 1];
    }

    if (key_words > 6 && (k % key_words) == 4) {
      sub_words(tmp);
    }

    const unsigned int m = k - key_words;
    xor_small_u8_array(round_keys->round_keys[k / block_words][k % block_words],
                       round_keys->round_keys[m / block_words][m % block_words], tmp, 4);
  }
}

// Calling Functions

void aes128_init_round_keys(aes_round_keys_t* round_key, const uint8_t* key) {
  expand_key(round_key, key, KEY_WORDS_128, AES_BLOCK_WORDS, AES_ROUNDS_128);
}

void aes192_init_round_keys(aes_round_keys_t* round_key, const uint8_t* key) {
  expand_key(round_key, key, KEY_WORDS_192, AES_BLOCK_WORDS, AES_ROUNDS_192);
}

void aes256_init_round_keys(aes_round_keys_t* round_key, const uint8_t* key) {
  expand_key(round_key, key, KEY_WORDS_256, AES_BLOCK_WORDS, AES_ROUNDS_256);
}

void rijndael192_init_round_keys(aes_round_keys_t* round_key, const uint8_t* key) {
  expand_key(round_key, key, KEY_WORDS_192, RIJNDAEL_BLOCK_WORDS_192, AES_ROUNDS_192);
}

void rijndael256_init_round_keys(aes_round_keys_t* round_key, const uint8_t* key) {
  expand_key(round_key, key, KEY_WORDS_256, RIJNDAEL_BLOCK_WORDS_256, AES_ROUNDS_256);
}

static void load_state(aes_block_t state, const uint8_t* src, unsigned int block_words) {
  for (unsigned int i = 0; i != block_words * 4; ++i) {
    state[i / 4][i % 4] = bf8_load(&src[i]);
  }
}

static uint8_t* store_state(uint8_t* dst, aes_block_t state, unsigned int block_words) {
  for (unsigned int i = 0; i != block_words * 4; ++i, ++dst) {
    bf8_store(dst, state[i / 4][i % 4]);
  }
  return dst;
}

static void aes_encrypt(const aes_round_keys_t* keys, aes_block_t state, unsigned int block_words,
                        unsigned int num_rounds) {
  // first round
  add_round_key(0, state, keys, block_words);

  for (unsigned int round = 1; round < num_rounds; ++round) {
    sub_bytes(state, block_words);
    shift_row(state, block_words);
    mix_column(state, block_words);
    add_round_key(round, state, keys, block_words);
  }

  // last round
  sub_bytes(state, block_words);
  shift_row(state, block_words);
  add_round_key(num_rounds, state, keys, block_words);
}

#if defined(FAEST_TESTS)
void aes128_encrypt_block(const aes_round_keys_t* key, const uint8_t* plaintext,
                          uint8_t* ciphertext) {
  aes_block_t state;
  load_state(state, plaintext, AES_BLOCK_WORDS);
  aes_encrypt(key, state, AES_BLOCK_WORDS, AES_ROUNDS_128);
  store_state(ciphertext, state, AES_BLOCK_WORDS);
}

void aes192_encrypt_block(const aes_round_keys_t* key, const uint8_t* plaintext,
                          uint8_t* ciphertext) {
  aes_block_t state;
  load_state(state, plaintext, AES_BLOCK_WORDS);
  aes_encrypt(key, state, AES_BLOCK_WORDS, AES_ROUNDS_192);
  store_state(ciphertext, state, AES_BLOCK_WORDS);
}

void aes256_encrypt_block(const aes_round_keys_t* key, const uint8_t* plaintext,
                          uint8_t* ciphertext) {
  aes_block_t state;
  load_state(state, plaintext, AES_BLOCK_WORDS);
  aes_encrypt(key, state, AES_BLOCK_WORDS, AES_ROUNDS_256);
  store_state(ciphertext, state, AES_BLOCK_WORDS);
}
#endif

void rijndael192_encrypt_block(const aes_round_keys_t* key, const uint8_t* plaintext,
                               uint8_t* ciphertext) {
  aes_block_t state;
  load_state(state, plaintext, RIJNDAEL_BLOCK_WORDS_192);
  aes_encrypt(key, state, RIJNDAEL_BLOCK_WORDS_192, AES_ROUNDS_192);
  store_state(ciphertext, state, RIJNDAEL_BLOCK_WORDS_192);
}

void rijndael256_encrypt_block(const aes_round_keys_t* key, const uint8_t* plaintext,
                               uint8_t* ciphertext) {
  aes_block_t state;
  load_state(state, plaintext, RIJNDAEL_BLOCK_WORDS_256);
  aes_encrypt(key, state, RIJNDAEL_BLOCK_WORDS_256, AES_ROUNDS_256);
  store_state(ciphertext, state, RIJNDAEL_BLOCK_WORDS_256);
}

static void add_to_upper_word(uint8_t* iv, uint32_t tweak) {
  uint32_t iv3;
  memcpy(&iv3, iv + IV_SIZE - sizeof(uint32_t), sizeof(uint32_t));
  iv3 = htole32(le32toh(iv3) + tweak);
  memcpy(iv + IV_SIZE - sizeof(uint32_t), &iv3, sizeof(uint32_t));
}

#if defined(HAVE_AESNI)
ATTR_TARGET_AESNI ATTR_ALWAYS_INLINE static inline __m128i sse2_increment_iv(__m128i iv) {
  return _mm_add_epi32(iv, _mm_set_epi32(0, 0, 0, 1));
}

ATTR_TARGET_AESNI static void prg_aesni_128(const uint8_t* key, const uint8_t* iv, uint8_t* out,
                                            size_t outlen) {
  __m128i rk[AES_ROUNDS_128 + 1];
  aes128_expand_key_aesni(rk, key);

  __m128i miv = _mm_load_si128((const __m128i*)iv);
  for (size_t idx = 0; idx < outlen / IV_SIZE; idx += 1, out += IV_SIZE) {
    __m128i m = _mm_xor_si128(miv, rk[0]);
    for (unsigned int round = 1; round != AES_ROUNDS_128; ++round) {
      m = _mm_aesenc_si128(m, rk[round]);
    }
    m = _mm_aesenclast_si128(m, rk[AES_ROUNDS_128]);
    _mm_storeu_si128((__m128i_u*)out, m);
    miv = sse2_increment_iv(miv);
  }

  if (outlen % IV_SIZE) {
    __m128i m = _mm_xor_si128(miv, rk[0]);
    for (unsigned int round = 1; round != AES_ROUNDS_128; ++round) {
      m = _mm_aesenc_si128(m, rk[round]);
    }
    m = _mm_aesenclast_si128(m, rk[AES_ROUNDS_128]);

    uint8_t last_block[IV_SIZE];
    _mm_storeu_si128((__m128i_u*)last_block, m);

    memcpy(out, last_block, outlen % IV_SIZE);
  }
}

ATTR_TARGET_AESNI static void prg_2_aesni_128(const uint8_t* key, const uint8_t* iv, uint8_t* out) {
  __m128i rk[AES_ROUNDS_128 + 1];
  aes128_expand_key_aesni(rk, key);

  __m128i temp[2];
  temp[0] = _mm_load_si128((const __m128i*)iv);
  temp[1] = sse2_increment_iv(temp[0]);
  temp[0] = _mm_xor_si128(temp[0], rk[0]);
  temp[1] = _mm_xor_si128(temp[1], rk[0]);
  for (unsigned int round = 1; round != AES_ROUNDS_128; ++round) {
    temp[0] = _mm_aesenc_si128(temp[0], rk[round]);
    temp[1] = _mm_aesenc_si128(temp[1], rk[round]);
  }
  _mm_storeu_si128((__m128i_u*)out, _mm_aesenclast_si128(temp[0], rk[AES_ROUNDS_128]));
  _mm_storeu_si128((__m128i_u*)out + 1, _mm_aesenclast_si128(temp[1], rk[AES_ROUNDS_128]));
}

ATTR_TARGET_AESNI static void prg_4_aesni_128(const uint8_t* key, const uint8_t* iv, uint8_t* out) {
  __m128i rk[AES_ROUNDS_128 + 1];
  aes128_expand_key_aesni(rk, key);

  __m128i temp[4];
  temp[0] = _mm_load_si128((const __m128i*)iv);
  temp[1] = sse2_increment_iv(temp[0]);
  temp[2] = sse2_increment_iv(temp[1]);
  temp[3] = sse2_increment_iv(temp[2]);
  temp[0] = _mm_xor_si128(temp[0], rk[0]);
  temp[1] = _mm_xor_si128(temp[1], rk[0]);
  temp[2] = _mm_xor_si128(temp[2], rk[0]);
  temp[3] = _mm_xor_si128(temp[3], rk[0]);
  for (unsigned int round = 1; round != AES_ROUNDS_128; ++round) {
    temp[0] = _mm_aesenc_si128(temp[0], rk[round]);
    temp[1] = _mm_aesenc_si128(temp[1], rk[round]);
    temp[2] = _mm_aesenc_si128(temp[2], rk[round]);
    temp[3] = _mm_aesenc_si128(temp[3], rk[round]);
  }
  _mm_store_si128((__m128i*)out, _mm_aesenclast_si128(temp[0], rk[AES_ROUNDS_128]));
  _mm_store_si128((__m128i*)out + 1, _mm_aesenclast_si128(temp[1], rk[AES_ROUNDS_128]));
  _mm_store_si128((__m128i*)out + 2, _mm_aesenclast_si128(temp[2], rk[AES_ROUNDS_128]));
  _mm_store_si128((__m128i*)out + 3, _mm_aesenclast_si128(temp[3], rk[AES_ROUNDS_128]));
}

ATTR_TARGET_AESNI static void prg_aesni_192(const uint8_t* key, const uint8_t* iv, uint8_t* out,
                                            size_t outlen) {
  __m128i rk[AES_ROUNDS_192 + 1];
  aes192_expand_key_aesni(rk, key);

  __m128i miv = _mm_load_si128((const __m128i*)iv);
  for (size_t idx = 0; idx < outlen / IV_SIZE; idx += 1, out += IV_SIZE) {
    __m128i m = _mm_xor_si128(miv, rk[0]);
    for (unsigned int round = 1; round != AES_ROUNDS_192; ++round) {
      m = _mm_aesenc_si128(m, rk[round]);
    }
    m = _mm_aesenclast_si128(m, rk[AES_ROUNDS_192]);
    _mm_storeu_si128((__m128i_u*)out, m);
    miv = sse2_increment_iv(miv);
  }

  if (outlen % IV_SIZE) {
    __m128i m = _mm_xor_si128(miv, rk[0]);
    for (unsigned int round = 1; round != AES_ROUNDS_192; ++round) {
      m = _mm_aesenc_si128(m, rk[round]);
    }
    m = _mm_aesenclast_si128(m, rk[AES_ROUNDS_192]);

    uint8_t last_block[IV_SIZE];
    _mm_storeu_si128((__m128i_u*)last_block, m);

    memcpy(out, last_block, outlen % IV_SIZE);
  }
}

ATTR_TARGET_AESNI static void prg_2_aesni_192(const uint8_t* key, const uint8_t* iv, uint8_t* out) {
  __m128i rk[AES_ROUNDS_192 + 1];
  aes192_expand_key_aesni(rk, key);

  __m128i temp[3];
  temp[0] = _mm_load_si128((const __m128i*)iv);
  temp[1] = sse2_increment_iv(temp[0]);
  temp[2] = sse2_increment_iv(temp[1]);
  temp[0] = _mm_xor_si128(temp[0], rk[0]);
  temp[1] = _mm_xor_si128(temp[1], rk[0]);
  temp[2] = _mm_xor_si128(temp[2], rk[0]);
  for (unsigned int round = 1; round != AES_ROUNDS_192; ++round) {
    temp[0] = _mm_aesenc_si128(temp[0], rk[round]);
    temp[1] = _mm_aesenc_si128(temp[1], rk[round]);
    temp[2] = _mm_aesenc_si128(temp[2], rk[round]);
  }
  _mm_storeu_si128((__m128i_u*)out, _mm_aesenclast_si128(temp[0], rk[AES_ROUNDS_192]));
  _mm_storeu_si128((__m128i_u*)out + 1, _mm_aesenclast_si128(temp[1], rk[AES_ROUNDS_192]));
  _mm_storeu_si128((__m128i_u*)out + 2, _mm_aesenclast_si128(temp[2], rk[AES_ROUNDS_192]));
}

ATTR_TARGET_AESNI static void prg_4_aesni_192(const uint8_t* key, const uint8_t* iv, uint8_t* out) {
  __m128i rk[AES_ROUNDS_192 + 1];
  aes192_expand_key_aesni(rk, key);

  __m128i temp[6];
  temp[0] = _mm_load_si128((const __m128i*)iv);
  for (unsigned int i = 1; i != 6; ++i) {
    temp[i] = sse2_increment_iv(temp[i - 1]);
  }
  for (unsigned int i = 0; i != 6; ++i) {
    temp[i] = _mm_xor_si128(temp[i], rk[0]);
  }
  for (unsigned int round = 1; round != AES_ROUNDS_192; ++round) {
    for (unsigned int i = 0; i != 6; ++i) {
      temp[i] = _mm_aesenc_si128(temp[i], rk[round]);
    }
  }
  for (unsigned int i = 0; i != 6; ++i) {
    _mm_store_si128((__m128i*)out + i, _mm_aesenclast_si128(temp[i], rk[AES_ROUNDS_192]));
  }
}

ATTR_TARGET_AESNI static void prg_aesni_256(const uint8_t* key, const uint8_t* iv, uint8_t* out,
                                            size_t outlen) {
  __m128i rk[AES_ROUNDS_256 + 1];
  aes256_expand_key_aesni(rk, key);

  __m128i miv = _mm_load_si128((const __m128i*)iv);
  for (size_t idx = 0; idx < outlen / IV_SIZE; idx += 1, out += IV_SIZE) {
    __m128i m = _mm_xor_si128(miv, rk[0]);
    for (unsigned int round = 1; round != AES_ROUNDS_256; ++round) {
      m = _mm_aesenc_si128(m, rk[round]);
    }
    m = _mm_aesenclast_si128(m, rk[AES_ROUNDS_256]);
    _mm_storeu_si128((__m128i_u*)out, m);
    miv = sse2_increment_iv(miv);
  }

  if (outlen % IV_SIZE) {
    __m128i m = _mm_xor_si128(miv, rk[0]);
    for (unsigned int round = 1; round != AES_ROUNDS_256; ++round) {
      m = _mm_aesenc_si128(m, rk[round]);
    }
    m = _mm_aesenclast_si128(m, rk[AES_ROUNDS_256]);

    uint8_t last_block[IV_SIZE];
    _mm_storeu_si128((__m128i_u*)last_block, m);

    memcpy(out, last_block, outlen % IV_SIZE);
  }
}

ATTR_TARGET_AESNI static void prg_2_aesni_256(const uint8_t* key, const uint8_t* iv, uint8_t* out) {
  __m128i rk[AES_ROUNDS_256 + 1];
  aes256_expand_key_aesni(rk, key);

  __m128i temp[4];
  temp[0] = _mm_load_si128((const __m128i*)iv);
  for (unsigned int i = 1; i != 4; ++i) {
    temp[i] = sse2_increment_iv(temp[i - 1]);
  }
  for (unsigned int i = 0; i != 4; ++i) {
    temp[i] = _mm_xor_si128(temp[i], rk[0]);
  }
  for (unsigned int round = 1; round != AES_ROUNDS_256; ++round) {
    for (unsigned int i = 0; i != 4; ++i) {
      temp[i] = _mm_aesenc_si128(temp[i], rk[round]);
    }
  }
  for (unsigned int i = 0; i != 4; ++i) {
    _mm_storeu_si128((__m128i_u*)out + i, _mm_aesenclast_si128(temp[i], rk[AES_ROUNDS_256]));
  }
}

ATTR_TARGET_AESNI static void prg_4_aesni_256(const uint8_t* key, const uint8_t* iv, uint8_t* out) {
  __m128i rk[AES_ROUNDS_256 + 1];
  aes256_expand_key_aesni(rk, key);

  __m128i temp[8];
  temp[0] = _mm_load_si128((const __m128i*)iv);
  for (unsigned int i = 1; i != 8; ++i) {
    temp[i] = sse2_increment_iv(temp[i - 1]);
  }
  for (unsigned int i = 0; i != 8; ++i) {
    temp[i] = _mm_xor_si128(temp[i], rk[0]);
  }
  for (unsigned int round = 1; round != AES_ROUNDS_256; ++round) {
    for (unsigned int i = 0; i != 8; ++i) {
      temp[i] = _mm_aesenc_si128(temp[i], rk[round]);
    }
  }
  for (unsigned int i = 0; i != 8; ++i) {
    _mm_store_si128((__m128i*)out + i, _mm_aesenclast_si128(temp[i], rk[AES_ROUNDS_256]));
  }
}

#if defined(HAVE_AVX2)
ATTR_TARGET_AESNI_AVX2 static void prg_aesni_avx_128(const uint8_t* key, const uint8_t* iv,
                                                     uint8_t* out, size_t outlen) {
  __m128i rk[AES_ROUNDS_128 + 1];
  aes128_expand_key_aesni_avx2(rk, key);

  __m128i miv = _mm_load_si128((const __m128i*)iv);
  for (size_t idx = 0; idx < outlen / IV_SIZE; idx += 1, out += IV_SIZE) {
    __m128i m = _mm_xor_si128(miv, rk[0]);
    for (unsigned int round = 1; round != AES_ROUNDS_128; ++round) {
      m = _mm_aesenc_si128(m, rk[round]);
    }
    m = _mm_aesenclast_si128(m, rk[AES_ROUNDS_128]);
    _mm_storeu_si128((__m128i_u*)out, m);
    miv = sse2_increment_iv(miv);
  }

  if (outlen % IV_SIZE) {
    __m128i m = _mm_xor_si128(miv, rk[0]);
    for (unsigned int round = 1; round != AES_ROUNDS_128; ++round) {
      m = _mm_aesenc_si128(m, rk[round]);
    }
    m = _mm_aesenclast_si128(m, rk[AES_ROUNDS_128]);

    uint8_t last_block[IV_SIZE];
    _mm_storeu_si128((__m128i_u*)last_block, m);

    memcpy(out, last_block, outlen % IV_SIZE);
  }
}

ATTR_TARGET_AESNI_AVX2 static void prg_2_aesni_avx_128(const uint8_t* key, const uint8_t* iv,
                                                       uint8_t* out) {
  __m128i rk[AES_ROUNDS_128 + 1];
  aes128_expand_key_aesni_avx2(rk, key);

  __m128i temp[2];
  temp[0] = _mm_load_si128((const __m128i*)iv);
  temp[1] = sse2_increment_iv(temp[0]);
  temp[0] = _mm_xor_si128(temp[0], rk[0]);
  temp[1] = _mm_xor_si128(temp[1], rk[0]);
  for (unsigned int round = 1; round != AES_ROUNDS_128; ++round) {
    temp[0] = _mm_aesenc_si128(temp[0], rk[round]);
    temp[1] = _mm_aesenc_si128(temp[1], rk[round]);
  }
  temp[0] = _mm_aesenclast_si128(temp[0], rk[AES_ROUNDS_128]);
  temp[1] = _mm_aesenclast_si128(temp[1], rk[AES_ROUNDS_128]);
  _mm_storeu_si128((__m128i_u*)out, temp[0]);
  _mm_storeu_si128((__m128i_u*)(out + IV_SIZE), temp[1]);
}

ATTR_TARGET_AESNI_AVX2 static void prg_4_aesni_avx_128(const uint8_t* key, const uint8_t* iv,
                                                       uint8_t* out) {
  __m128i rk[AES_ROUNDS_128 + 1];
  aes128_expand_key_aesni_avx2(rk, key);

  __m128i temp[4];
  temp[0] = _mm_load_si128((const __m128i*)iv);
  temp[1] = sse2_increment_iv(temp[0]);
  temp[2] = sse2_increment_iv(temp[1]);
  temp[3] = sse2_increment_iv(temp[2]);
  temp[0] = _mm_xor_si128(temp[0], rk[0]);
  temp[1] = _mm_xor_si128(temp[1], rk[0]);
  temp[2] = _mm_xor_si128(temp[2], rk[0]);
  temp[3] = _mm_xor_si128(temp[3], rk[0]);
  for (unsigned int round = 1; round != AES_ROUNDS_128; ++round) {
    temp[0] = _mm_aesenc_si128(temp[0], rk[round]);
    temp[1] = _mm_aesenc_si128(temp[1], rk[round]);
    temp[2] = _mm_aesenc_si128(temp[2], rk[round]);
    temp[3] = _mm_aesenc_si128(temp[3], rk[round]);
  }
  _mm_store_si128((__m128i*)out, _mm_aesenclast_si128(temp[0], rk[AES_ROUNDS_128]));
  _mm_store_si128((__m128i*)out + 1, _mm_aesenclast_si128(temp[1], rk[AES_ROUNDS_128]));
  _mm_store_si128((__m128i*)out + 2, _mm_aesenclast_si128(temp[2], rk[AES_ROUNDS_128]));
  _mm_store_si128((__m128i*)out + 3, _mm_aesenclast_si128(temp[3], rk[AES_ROUNDS_128]));
}

ATTR_TARGET_AESNI_AVX2 static void prg_aesni_avx_192(const uint8_t* key, const uint8_t* iv,
                                                     uint8_t* out, size_t outlen) {
  __m128i rk[AES_ROUNDS_192 + 1];
  aes192_expand_key_aesni_avx2(rk, key);

  __m128i miv = _mm_load_si128((const __m128i*)iv);
  for (size_t idx = 0; idx < outlen / IV_SIZE; idx += 1, out += IV_SIZE) {
    __m128i m = _mm_xor_si128(miv, rk[0]);
    for (unsigned int round = 1; round != AES_ROUNDS_192; ++round) {
      m = _mm_aesenc_si128(m, rk[round]);
    }
    m = _mm_aesenclast_si128(m, rk[AES_ROUNDS_192]);
    _mm_storeu_si128((__m128i_u*)out, m);
    miv = sse2_increment_iv(miv);
  }

  if (outlen % IV_SIZE) {
    __m128i m = _mm_xor_si128(miv, rk[0]);
    for (unsigned int round = 1; round != AES_ROUNDS_192; ++round) {
      m = _mm_aesenc_si128(m, rk[round]);
    }
    m = _mm_aesenclast_si128(m, rk[AES_ROUNDS_192]);

    uint8_t last_block[IV_SIZE];
    _mm_storeu_si128((__m128i_u*)last_block, m);

    memcpy(out, last_block, outlen % IV_SIZE);
  }
}

ATTR_TARGET_AESNI_AVX2 static void prg_2_aesni_avx_192(const uint8_t* key, const uint8_t* iv,
                                                       uint8_t* out) {
  __m128i rk[AES_ROUNDS_192 + 1];
  aes192_expand_key_aesni_avx2(rk, key);

  __m128i temp[3];
  temp[0] = _mm_load_si128((const __m128i*)iv);
  temp[1] = sse2_increment_iv(temp[0]);
  temp[2] = sse2_increment_iv(temp[1]);
  temp[0] = _mm_xor_si128(temp[0], rk[0]);
  temp[1] = _mm_xor_si128(temp[1], rk[0]);
  temp[2] = _mm_xor_si128(temp[2], rk[0]);
  for (unsigned int round = 1; round != AES_ROUNDS_192; ++round) {
    temp[0] = _mm_aesenc_si128(temp[0], rk[round]);
    temp[1] = _mm_aesenc_si128(temp[1], rk[round]);
    temp[2] = _mm_aesenc_si128(temp[2], rk[round]);
  }
  _mm_storeu_si128((__m128i_u*)out, _mm_aesenclast_si128(temp[0], rk[AES_ROUNDS_192]));
  _mm_storeu_si128((__m128i_u*)out + 1, _mm_aesenclast_si128(temp[1], rk[AES_ROUNDS_192]));
  _mm_storeu_si128((__m128i_u*)out + 2, _mm_aesenclast_si128(temp[2], rk[AES_ROUNDS_192]));
}

ATTR_TARGET_AESNI_AVX2 static void prg_4_aesni_avx_192(const uint8_t* key, const uint8_t* iv,
                                                       uint8_t* out) {
  __m128i rk[AES_ROUNDS_192 + 1];
  aes192_expand_key_aesni_avx2(rk, key);

  __m128i temp[6];
  temp[0] = _mm_load_si128((const __m128i*)iv);
  for (unsigned int i = 1; i != 6; ++i) {
    temp[i] = sse2_increment_iv(temp[i - 1]);
  }
  for (unsigned int i = 0; i != 6; ++i) {
    temp[i] = _mm_xor_si128(temp[i], rk[0]);
  }
  for (unsigned int round = 1; round != AES_ROUNDS_192; ++round) {
    for (unsigned int i = 0; i != 6; ++i) {
      temp[i] = _mm_aesenc_si128(temp[i], rk[round]);
    }
  }
  for (unsigned int i = 0; i != 6; ++i) {
    _mm_store_si128((__m128i*)out + i, _mm_aesenclast_si128(temp[i], rk[AES_ROUNDS_192]));
  }
}

ATTR_TARGET_AESNI_AVX2 static void prg_aesni_avx_256(const uint8_t* key, const uint8_t* iv,
                                                     uint8_t* out, size_t outlen) {
  __m128i rk[AES_ROUNDS_256 + 1];
  aes256_expand_key_aesni_avx2(rk, key);

  __m128i miv = _mm_load_si128((const __m128i*)iv);
  for (size_t idx = 0; idx < outlen / IV_SIZE; idx += 1, out += IV_SIZE) {
    __m128i m = _mm_xor_si128(miv, rk[0]);
    for (unsigned int round = 1; round != AES_ROUNDS_256; ++round) {
      m = _mm_aesenc_si128(m, rk[round]);
    }
    m = _mm_aesenclast_si128(m, rk[AES_ROUNDS_256]);
    _mm_storeu_si128((__m128i_u*)out, m);
    miv = sse2_increment_iv(miv);
  }

  if (outlen % IV_SIZE) {
    __m128i m = _mm_xor_si128(miv, rk[0]);
    for (unsigned int round = 1; round != AES_ROUNDS_256; ++round) {
      m = _mm_aesenc_si128(m, rk[round]);
    }
    m = _mm_aesenclast_si128(m, rk[AES_ROUNDS_256]);

    uint8_t last_block[IV_SIZE];
    _mm_storeu_si128((__m128i_u*)last_block, m);

    memcpy(out, last_block, outlen % IV_SIZE);
  }
}

ATTR_TARGET_AESNI_AVX2 static void prg_2_aesni_avx_256(const uint8_t* key, const uint8_t* iv,
                                                       uint8_t* out) {
  __m128i rk[AES_ROUNDS_256 + 1];
  aes256_expand_key_aesni_avx2(rk, key);

  __m128i temp[4];
  temp[0] = _mm_load_si128((const __m128i*)iv);
  for (unsigned int i = 1; i != 4; ++i) {
    temp[i] = sse2_increment_iv(temp[i - 1]);
  }
  for (unsigned int i = 0; i != 4; ++i) {
    temp[i] = _mm_xor_si128(temp[i], rk[0]);
  }
  for (unsigned int round = 1; round != AES_ROUNDS_256; ++round) {
    for (unsigned int i = 0; i != 4; ++i) {
      temp[i] = _mm_aesenc_si128(temp[i], rk[round]);
    }
  }
  for (unsigned int i = 0; i != 4; ++i) {
    _mm_storeu_si128((__m128i_u*)out + i, _mm_aesenclast_si128(temp[i], rk[AES_ROUNDS_256]));
  }
}

ATTR_TARGET_AESNI_AVX2 static void prg_4_aesni_avx_256(const uint8_t* key, const uint8_t* iv,
                                                       uint8_t* out) {
  __m128i rk[AES_ROUNDS_256 + 1];
  aes256_expand_key_aesni_avx2(rk, key);

  __m128i temp[8];
  temp[0] = _mm_load_si128((const __m128i*)iv);
  for (unsigned int i = 1; i != 8; ++i) {
    temp[i] = sse2_increment_iv(temp[i - 1]);
  }
  for (unsigned int i = 0; i != 8; ++i) {
    temp[i] = _mm_xor_si128(temp[i], rk[0]);
  }
  for (unsigned int round = 1; round != AES_ROUNDS_256; ++round) {
    for (unsigned int i = 0; i != 8; ++i) {
      temp[i] = _mm_aesenc_si128(temp[i], rk[round]);
    }
  }
  for (unsigned int i = 0; i != 8; ++i) {
    _mm_store_si128((__m128i*)out + i, _mm_aesenclast_si128(temp[i], rk[AES_ROUNDS_256]));
  }
}
#endif
#endif

static void generic_prg(const uint8_t* key, uint8_t* internal_iv, uint8_t* out, unsigned int seclvl,
                        size_t outlen) {
  generic_aes_ecb_t ctx;
  int ret = generic_aes_ecb_new(&ctx, key, seclvl);
  assert(ret == 0);
  (void)ret;

  for (size_t idx = 0; idx < outlen / IV_SIZE; idx += 1, out += IV_SIZE) {
    ret = generic_aes_ecb_encrypt(&ctx, out, internal_iv, 1);
    assert(ret == 0);
    (void)ret;
    aes_increment_iv(internal_iv);
  }

  if (outlen % IV_SIZE) {
    uint8_t last_block[IV_SIZE];
    ret = generic_aes_ecb_encrypt(&ctx, last_block, internal_iv, 1);
    assert(ret == 0);
    (void)ret;
    memcpy(out, last_block, outlen % IV_SIZE);
  }

  generic_aes_ecb_free(&ctx);
}

void prg(const uint8_t* key, const uint8_t* iv, uint32_t tweak, uint8_t* out, unsigned int seclvl,
         size_t outlen) {
  ATTR_ALIGNED(16) uint8_t internal_iv[IV_SIZE];
  memcpy(internal_iv, iv, IV_SIZE);
  add_to_upper_word(internal_iv, tweak);

#if defined(HAVE_AESNI)
// use AES-NI if possible
#if defined(HAVE_AVX2)
  if (CPU_SUPPORTS_AESNI_AVX2) {
    switch (seclvl) {
    case 256:
      prg_aesni_avx_256(key, internal_iv, out, outlen);
      return;
    case 192:
      prg_aesni_avx_192(key, internal_iv, out, outlen);
      return;
    default:
      prg_aesni_avx_128(key, internal_iv, out, outlen);
      return;
    }
  }
#endif

  if (CPU_SUPPORTS_AESNI) {
    switch (seclvl) {
    case 256:
      prg_aesni_256(key, internal_iv, out, outlen);
      return;
    case 192:
      prg_aesni_192(key, internal_iv, out, outlen);
      return;
    default:
      prg_aesni_128(key, internal_iv, out, outlen);
      return;
    }
  }
#endif

  generic_prg(key, internal_iv, out, seclvl, outlen);
}

void prg_2_lambda(const uint8_t* key, const uint8_t* iv, uint32_t tweak, uint8_t* out,
                  unsigned int seclvl) {
  ATTR_ALIGNED(16) uint8_t internal_iv[IV_SIZE];
  memcpy(internal_iv, iv, IV_SIZE);
  add_to_upper_word(internal_iv, tweak);

#if defined(HAVE_AESNI)
// use AES-NI if possible
#if defined(HAVE_AVX2)
  if (CPU_SUPPORTS_AESNI_AVX2) {
    switch (seclvl) {
    case 256:
      prg_2_aesni_avx_256(key, internal_iv, out);
      return;
    case 192:
      prg_2_aesni_avx_192(key, internal_iv, out);
      return;
    default:
      prg_2_aesni_avx_128(key, internal_iv, out);
      return;
    }
  }
#endif

  if (CPU_SUPPORTS_AESNI) {
    switch (seclvl) {
    case 256:
      prg_2_aesni_256(key, internal_iv, out);
      return;
    case 192:
      prg_2_aesni_192(key, internal_iv, out);
      return;
    default:
      prg_2_aesni_128(key, internal_iv, out);
      return;
    }
  }
#endif

  generic_prg(key, internal_iv, out, seclvl, seclvl * 2 / 8);
}

void prg_4_lambda(const uint8_t* key, const uint8_t* iv, uint32_t tweak, uint8_t* out,
                  unsigned int seclvl) {
  ATTR_ALIGNED(16) uint8_t internal_iv[IV_SIZE];
  memcpy(internal_iv, iv, IV_SIZE);
  add_to_upper_word(internal_iv, tweak);

#if defined(HAVE_AESNI)
// use AES-NI if possible
#if defined(HAVE_AVX2)
  if (CPU_SUPPORTS_AESNI_AVX2) {
    assert((((uintptr_t)out) & (16 - 1)) == 0);
    switch (seclvl) {
    case 256:
      prg_4_aesni_avx_256(key, internal_iv, out);
      return;
    case 192:
      prg_4_aesni_avx_192(key, internal_iv, out);
      return;
    default:
      prg_4_aesni_avx_128(key, internal_iv, out);
      return;
    }
  }
#endif

  if (CPU_SUPPORTS_AESNI) {
    assert((((uintptr_t)out) & (16 - 1)) == 0);
    switch (seclvl) {
    case 256:
      prg_4_aesni_256(key, internal_iv, out);
      return;
    case 192:
      prg_4_aesni_192(key, internal_iv, out);
      return;
    default:
      prg_4_aesni_128(key, internal_iv, out);
      return;
    }
  }
#endif

  generic_prg(key, internal_iv, out, seclvl, seclvl * 4 / 8);
}

void aes_extend_witness_7(uint8_t* w, const uint8_t* key, const uint8_t* in,
                          const faest_paramset_t* params) {
  const unsigned int lambda      = params->lambda;
  const unsigned int num_rounds  = params->R;
  const unsigned int blocksize   = 32 * params->Nst;
  const unsigned int beta        = (lambda + blocksize - 1) / blocksize;
  const unsigned int block_words = blocksize / 32;
  const bool is_em               = faest_is_em(params);

#if !defined(NDEBUG)
  uint8_t* const w_out = w;
#endif

  if (is_em) {
    // switch input and key for EM
    const uint8_t* tmp = key;
    key                = in;
    in                 = tmp;
  }

  // line 6
  aes_round_keys_t round_keys;
  switch (lambda) {
  case 256:
    if (is_em) {
      // for scan-build
      assert(block_words == RIJNDAEL_BLOCK_WORDS_256);
      rijndael256_init_round_keys(&round_keys, key);
    } else {
      aes256_init_round_keys(&round_keys, key);
    }
    break;
  case 192:
    if (is_em) {
      // for scan-build
      assert(block_words == RIJNDAEL_BLOCK_WORDS_192);
      rijndael192_init_round_keys(&round_keys, key);
    } else {
      aes192_init_round_keys(&round_keys, key);
    }
    break;
  default:
    aes128_init_round_keys(&round_keys, key);
    break;
  }

  // line 7,8
  if (!is_em) {
    const unsigned int nk = lambda / 32;

    // Key schedule constraints only needed for normal AES, not EM variant.
    for (unsigned int i = 0; i != nk; ++i) {
      memcpy(w, round_keys.round_keys[i / 4][i % 4], sizeof(aes_word_t));
      w += sizeof(aes_word_t);
    }

    const unsigned int S_ke = params->Ske;
    for (unsigned int j = 0, ik = nk; j < S_ke / 4; ++j) {
      memcpy(w, round_keys.round_keys[ik / 4][ik % 4], sizeof(aes_word_t));
      w += sizeof(aes_word_t);
      ik += lambda == 192 ? 6 : 4;
    }
  } else {
    const unsigned int lambda_bytes = lambda / 8;

    // saving the OWF key to the extended witness
    w = faest_mempcpy(w, in, lambda_bytes);
  }

  assert(w - w_out == params->Lke / 8);

  // line 14
  // common part for AES-128, EM-128, EM-192, EM-256, first part for AES-192 and AES-256
  {
    // line 16
    aes_block_t state;
    load_state(state, in, block_words);

    // line 17
    add_round_key(0, state, &round_keys, block_words);

    for (unsigned int round = 1; round < num_rounds; ++round) {
      // line 20
      sub_bytes(state, block_words);
      // line 21
      shift_row(state, block_words);
      if (round % 2 == 0) {
        // line 23
        w = store_state(w, state, block_words);
      }
      // line 24
      mix_column(state, block_words);
      // line 25
      add_round_key(round, state, &round_keys, block_words);
    }
    // last round is not commited to, so not computed
  }

  // line 14
  if (beta == 2) {
    // AES-192 and AES-256
    uint8_t buf[16];
    memcpy(buf, in, sizeof(buf));
    buf[0] ^= 0x1;

    // line 16
    aes_block_t state;
    load_state(state, buf, block_words);

    // line 17
    add_round_key(0, state, &round_keys, block_words);

    for (unsigned int round = 1; round < num_rounds; ++round) {
      // line 20
      sub_bytes(state, block_words);
      // line 21
      shift_row(state, block_words);
      if (round % 2 == 0) {
        // line 23
        w = store_state(w, state, block_words);
      }
      // line 24
      mix_column(state, block_words);
      // line 25
      add_round_key(round, state, &round_keys, block_words);
    }
    // last round is not commited to, so not computed
  }
}

#if defined(HAVE_OPENSSL)
int generic_aes_ecb_new(generic_aes_ecb_t* context, const uint8_t* key, unsigned int seclvl) {
  const EVP_CIPHER* cipher = NULL;
  switch (seclvl) {
  case 256:
    cipher = EVP_aes_256_ecb();
    break;
  case 192:
    cipher = EVP_aes_192_ecb();
    break;
  default:
    cipher = EVP_aes_128_ecb();
    break;
  }

  EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    return -1;
  }

  EVP_EncryptInit_ex(ctx, cipher, NULL, key, NULL);
  context->ctx = ctx;
  return 0;
}

int generic_aes_ecb_encrypt(generic_aes_ecb_t* ctx, uint8_t* ciphertext, const uint8_t* plaintext,
                            size_t blocks) {
  int len = 0;
  int ret = EVP_EncryptUpdate(ctx->ctx, ciphertext, &len, plaintext, blocks * IV_SIZE);
  if (ret != 1 || len != (int)blocks * IV_SIZE) {
    return -1;
  }
  return 0;
}

void generic_aes_ecb_free(generic_aes_ecb_t* ctx) {
  EVP_CIPHER_CTX_free(ctx->ctx);
}
#elif defined(OQS)
int generic_aes_ecb_new(generic_aes_ecb_t* ctx, const uint8_t* key, unsigned int seclvl) {
  if (!ctx || !key) {
    return -1;
  }

  ctx->seclvl = seclvl;
  ctx->sched  = NULL;
  switch (seclvl) {
  case 256:
    OQS_AES256_ECB_load_schedule(key, &ctx->sched);
    break;
  case 192:
    OQS_AES192_ECB_load_schedule(key, &ctx->sched);
    break;
  default:
    OQS_AES128_ECB_load_schedule(key, &ctx->sched);
    break;
  }

  return ctx->sched ? 0 : -1;
}

int generic_aes_ecb_encrypt(generic_aes_ecb_t* ctx, uint8_t* ciphertext, const uint8_t* plaintext,
                            size_t blocks) {
  if (!ctx || !ciphertext || !plaintext || !ctx->sched) {
    return -1;
  }

  const size_t nbytes = blocks * IV_SIZE;
  switch (ctx->seclvl) {
  case 256:
    OQS_AES256_ECB_enc_sch(plaintext, nbytes, ctx->sched, ciphertext);
    break;
  case 192:
    OQS_AES192_ECB_enc_sch(plaintext, nbytes, ctx->sched, ciphertext);
    break;
  default:
    OQS_AES128_ECB_enc_sch(plaintext, nbytes, ctx->sched, ciphertext);
    break;
  }
  return 0;
}

void generic_aes_ecb_free(generic_aes_ecb_t* ctx) {
  if (!ctx || !ctx->sched) {
    return;
  }

  switch (ctx->seclvl) {
  case 256:
    OQS_AES256_free_schedule(ctx->sched);
    break;
  case 192:
    OQS_AES192_free_schedule(ctx->sched);
    break;
  default:
    OQS_AES128_free_schedule(ctx->sched);
    break;
  }
  ctx->sched = NULL;
}
#elif defined(_WIN32)
int generic_aes_ecb_new(generic_aes_ecb_t* ctx, const uint8_t* key, unsigned int seclvl) {
  BCRYPT_ALG_HANDLE aes_handle = NULL;
  BCRYPT_KEY_HANDLE key_handle = NULL;

  NTSTATUS ret = BCryptOpenAlgorithmProvider(&aes_handle, BCRYPT_AES_ALGORITHM, NULL, 0);
  if (!BCRYPT_SUCCESS(ret)) {
    return -1;
  }
  ret = BCryptSetProperty(aes_handle, BCRYPT_CHAINING_MODE, (PUCHAR)BCRYPT_CHAIN_MODE_ECB,
                          (ULONG)((wcslen(BCRYPT_CHAIN_MODE_ECB) + 1) * sizeof(wchar_t)), 0);
  if (!BCRYPT_SUCCESS(ret)) {
    BCryptCloseAlgorithmProvider(aes_handle, 0);
    return -1;
  }

  ret = BCryptGenerateSymmetricKey(aes_handle, &key_handle, NULL, 0, (PUCHAR)key, seclvl / 8, 0);
  if (!BCRYPT_SUCCESS(ret)) {
    BCryptCloseAlgorithmProvider(aes_handle, 0);
    return -1;
  }

  ctx->aes_handle = aes_handle;
  ctx->key_handle = key_handle;
  return 0;
}

int generic_aes_ecb_encrypt(generic_aes_ecb_t* ctx, uint8_t* ciphertext, const uint8_t* plaintext,
                            size_t blocks) {
  ULONG len          = 0;
  const ULONG length = blocks * IV_SIZE;

  NTSTATUS ret =
      BCryptEncrypt(ctx->key_handle, plaintext, length, NULL, NULL, 0, ciphertext, length, &len, 0);
  if (!BCRYPT_SUCCESS(ret) || len != length) {
    return -1;
  }

  return 0;
}

void generic_aes_ecb_free(generic_aes_ecb_t* ctx) {
  BCryptDestroyKey(ctx->key_handle);
  BCryptCloseAlgorithmProvider(ctx->aes_handle, 0);
}
#else
int generic_aes_ecb_new(generic_aes_ecb_t* ctx, const uint8_t* key, unsigned int seclvl) {
  switch (seclvl) {
  case 256:
    aes256_init_round_keys(&ctx->round_keys, key);
    break;
  case 192:
    aes192_init_round_keys(&ctx->round_keys, key);
    break;
  default:
    aes128_init_round_keys(&ctx->round_keys, key);
    break;
  }

  ctx->seclvl = seclvl;
  return 0;
}

int generic_aes_ecb_encrypt(generic_aes_ecb_t* ctx, uint8_t* ciphertext, const uint8_t* plaintext,
                            size_t blocks) {
  switch (ctx->seclvl) {
  case 256: {
    for (; blocks; --blocks, plaintext += IV_SIZE, ciphertext += IV_SIZE) {
      aes_block_t state;
      load_state(state, plaintext, AES_BLOCK_WORDS);
      aes_encrypt(&ctx->round_keys, state, AES_BLOCK_WORDS, AES_ROUNDS_256);
      store_state(ciphertext, state, AES_BLOCK_WORDS);
    }
  }
  case 192: {
    for (; blocks; --blocks, plaintext += IV_SIZE, ciphertext += IV_SIZE) {
      aes_block_t state;
      load_state(state, plaintext, AES_BLOCK_WORDS);
      aes_encrypt(&ctx->round_keys, state, AES_BLOCK_WORDS, AES_ROUNDS_192);
      store_state(ciphertext, state, AES_BLOCK_WORDS);
    }
  }
  default: {
    for (; blocks; --blocks, plaintext += IV_SIZE, ciphertext += IV_SIZE) {
      aes_block_t state;
      load_state(state, plaintext, AES_BLOCK_WORDS);
      aes_encrypt(&ctx->round_keys, state, AES_BLOCK_WORDS, AES_ROUNDS_128);
      store_state(ciphertext, state, AES_BLOCK_WORDS);
    }
  }
  }

  return 0;
}

void generic_aes_ecb_free(generic_aes_ecb_t* ctx) {
  faest_explicit_bzero(&ctx->round_keys, sizeof(ctx->round_keys));
}
#endif
