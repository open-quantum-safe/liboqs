/*
 *  SPDX-License-Identifier: MIT
 */

#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif

#if defined(_WIN32) && !defined(HAVE_OPENSSL) && !defined(OQS)
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
#if defined(HAVE_ARM_AES)
#include "aes_arm.h"
#endif
#if defined(HAVE_OPENSSL)
#include <openssl/evp.h>
#elif defined(_WIN32) && !defined(OQS)
#include <windows.h>
#endif
#include <string.h>

#define KEY_WORDS_128 4
#define KEY_WORDS_192 6
#define KEY_WORDS_256 8

#define AES_BLOCK_WORDS 4
#define RIJNDAEL_BLOCK_WORDS_192 6
#define RIJNDAEL_BLOCK_WORDS_256 8

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
#if defined(FAEST_IS_LITTLE_ENDIAN)
  iv0 += 1;
#else
  iv0 = htole32(le32toh(iv0) + 1);
#endif
  memcpy(iv, &iv0, sizeof(uint32_t));
}

// ## AES ##
//
// Note that his is an unoptimized implementation of AES for reference and the
// computation of the witness. All other (especially time critical) uses of AES
// either use AES-NI or an external AES implementation (e.g., from OpenSSL) and
// only fallback to this implementation if there is no other option.

static inline aes_word_t load_word(const uint8_t* src) {
  aes_word_t word;
  memcpy(&word, src, sizeof(word));
  return le32toh(word);
}

static inline void store_word(uint8_t* dst, aes_word_t word) {
  word = htole32(word);
  memcpy(dst, &word, sizeof(word));
}

// Round functions

static aes_word_t sub_word(aes_word_t word) {
  aes_word_t result = 0;
  for (unsigned int r = 0; r < AES_NR; ++r) {
    result |= (uint32_t)compute_sbox((uint8_t)(word >> (8 * r))) << (8 * r);
  }
  return result;
}

static void add_round_key(unsigned int round, aes_block_t state, const aes_round_keys_t* round_key,
                          unsigned int block_words) {
  for (unsigned int c = 0; c < block_words; c++) {
    state[c] ^= round_key->round_keys[round][c];
  }
}

static void sub_bytes(aes_block_t state, unsigned int block_words) {
  for (unsigned int c = 0; c < block_words; c++) {
    state[c] = sub_word(state[c]);
  }
}

static void shift_row(aes_block_t state, unsigned int block_words) {
  for (unsigned int row = 1; row < AES_NR; ++row) {
    // Row offsets are (0, 1, 2, 3), or (0, 1, 3, 4) for Rijndael-256.
    const unsigned int shift = block_words == RIJNDAEL_BLOCK_WORDS_256 && row > 1 ? row + 1 : row;

    const aes_word_t mask = UINT32_C(0xff) << (8 * row);
    aes_word_t saved[4];
    // Save the prefix, move the remaining bytes left, then wrap the prefix around.
    for (unsigned int col = 0; col < shift; ++col) {
      saved[col] = state[col] & mask;
    }
    for (unsigned int col = 0; col < block_words - shift; ++col) {
      state[col] = (state[col] & ~mask) | (state[col + shift] & mask);
    }
    for (unsigned int col = 0; col < shift; ++col) {
      state[block_words - shift + col] = (state[block_words - shift + col] & ~mask) | saved[col];
    }
  }
}

static void mix_column(aes_block_t state, unsigned int block_words) {
  for (unsigned int col = 0; col < block_words; ++col) {
    const bf8_t a   = (uint8_t)state[col];
    const bf8_t b   = (uint8_t)(state[col] >> 8);
    const bf8_t c   = (uint8_t)(state[col] >> 16);
    const bf8_t d   = (uint8_t)(state[col] >> 24);
    const bf8_t sum = a ^ b ^ c ^ d;

    // instead of computing a * (X + 1) + b * X, compute (a + b) * X + a
    state[col] = (uint32_t)(a ^ sum ^ bf8_mulX(a ^ b)) |
                 ((uint32_t)(b ^ sum ^ bf8_mulX(b ^ c)) << 8) |
                 ((uint32_t)(c ^ sum ^ bf8_mulX(c ^ d)) << 16) |
                 ((uint32_t)(d ^ sum ^ bf8_mulX(d ^ a)) << 24);
  }
}

// Key Expansion functions
static inline aes_word_t rot_word(aes_word_t word) {
  return rotr32(word, 8);
}

void expand_key(aes_round_keys_t* round_keys, const uint8_t* key, unsigned int key_words,
                unsigned int block_words, unsigned int num_rounds) {
  for (unsigned int k = 0; k < key_words; k++) {
    round_keys->round_keys[k / block_words][k % block_words] = load_word(&key[4 * k]);
  }

  for (unsigned int k = key_words; k < block_words * (num_rounds + 1); ++k) {
    aes_word_t tmp = round_keys->round_keys[(k - 1) / block_words][(k - 1) % block_words];

    if (k % key_words == 0) {
      tmp = sub_word(rot_word(tmp));
      tmp ^= aes_round_constants[(k / key_words) - 1];
    }

    if (key_words > 6 && (k % key_words) == 4) {
      tmp = sub_word(tmp);
    }

    const unsigned int m = k - key_words;
    round_keys->round_keys[k / block_words][k % block_words] =
        round_keys->round_keys[m / block_words][m % block_words] ^ tmp;
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
  unsigned int i = 0;
  for (; i != block_words; ++i) {
    state[i] = load_word(&src[4 * i]);
  }
  for (; i != MAX_AES_BLOCK_WORDS; ++i) {
    state[i] = 0;
  }
}

static uint8_t* store_state(uint8_t* dst, aes_block_t state, unsigned int block_words) {
  for (unsigned int i = 0; i != block_words; ++i, dst += 4) {
    store_word(dst, state[i]);
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

#if defined(HAVE_ARM_AES)
ATTR_TARGET_ARM_AES ATTR_ALWAYS_INLINE static inline uint8x16_t neon_increment_iv(uint8x16_t iv) {
  const uint32x4_t increment = vsetq_lane_u32(1, vdupq_n_u32(0), 0);
  return vreinterpretq_u8_u32(vaddq_u32(vreinterpretq_u32_u8(iv), increment));
}

ATTR_TARGET_ARM_AES static void prg_arm_aes_128(const uint8_t* key, const uint8_t* iv, uint8_t* out,
                                                size_t outlen) {
  uint8x16_t rk[AES_ROUNDS_128 + 1];
  arm_aes128_expand_key(rk, key);

  uint8x16_t counter = vld1q_u8(iv);
  for (size_t idx = 0; idx < outlen / IV_SIZE; idx += 1, out += IV_SIZE) {
    uint8x16_t state = counter;
    arm_aes_encrypt_blocks(&state, 1, rk, AES_ROUNDS_128);
    vst1q_u8(out, state);
    counter = neon_increment_iv(counter);
  }

  if (outlen % IV_SIZE) {
    uint8x16_t state = counter;
    arm_aes_encrypt_blocks(&state, 1, rk, AES_ROUNDS_128);
    uint8_t last_block[IV_SIZE];
    vst1q_u8(last_block, state);
    memcpy(out, last_block, outlen % IV_SIZE);
  }
}

ATTR_TARGET_ARM_AES static void prg_arm_aes_192(const uint8_t* key, const uint8_t* iv, uint8_t* out,
                                                size_t outlen) {
  uint8x16_t rk[AES_ROUNDS_192 + 1];
  arm_aes192_expand_key(rk, key);

  uint8x16_t counter = vld1q_u8(iv);
  for (size_t idx = 0; idx < outlen / IV_SIZE; idx += 1, out += IV_SIZE) {
    uint8x16_t state = counter;
    arm_aes_encrypt_blocks(&state, 1, rk, AES_ROUNDS_192);
    vst1q_u8(out, state);
    counter = neon_increment_iv(counter);
  }

  if (outlen % IV_SIZE) {
    uint8x16_t state = counter;
    arm_aes_encrypt_blocks(&state, 1, rk, AES_ROUNDS_192);
    uint8_t last_block[IV_SIZE];
    vst1q_u8(last_block, state);
    memcpy(out, last_block, outlen % IV_SIZE);
  }
}

ATTR_TARGET_ARM_AES static void prg_arm_aes_256(const uint8_t* key, const uint8_t* iv, uint8_t* out,
                                                size_t outlen) {
  uint8x16_t rk[AES_ROUNDS_256 + 1];
  arm_aes256_expand_key(rk, key);

  uint8x16_t counter = vld1q_u8(iv);
  for (size_t idx = 0; idx < outlen / IV_SIZE; idx += 1, out += IV_SIZE) {
    uint8x16_t state = counter;
    arm_aes_encrypt_blocks(&state, 1, rk, AES_ROUNDS_256);
    vst1q_u8(out, state);
    counter = neon_increment_iv(counter);
  }

  if (outlen % IV_SIZE) {
    uint8x16_t state = counter;
    arm_aes_encrypt_blocks(&state, 1, rk, AES_ROUNDS_256);
    uint8_t last_block[IV_SIZE];
    vst1q_u8(last_block, state);
    memcpy(out, last_block, outlen % IV_SIZE);
  }
}
#endif

static void generic_prg(const uint8_t* key, uint8_t* internal_iv, uint8_t* out, unsigned int seclvl,
                        size_t outlen) {
  // liboqs scan-build mitigation (NOT a known runtime bug): upstream expresses
  // "these calls cannot fail" with assert(ret == 0). Under the clang static
  // analyzer (and any NDEBUG build) those asserts vanish, so the analyzer walks
  // the failure paths and reports ctx->sched as a garbage value and `out` as a
  // possible NULL dereference. Zero-initialising ctx and converting the asserts
  // into explicit early returns makes those paths well-defined without altering
  // behaviour on the real, never-failing call sequence.
  generic_aes_ecb_t ctx = {0};
  if (!key || !internal_iv || !out) {
    return;
  }
  if (generic_aes_ecb_new(&ctx, key, seclvl) != 0) {
    return;
  }

  for (size_t idx = 0; idx < outlen / IV_SIZE; idx += 1, out += IV_SIZE) {
    if (generic_aes_ecb_encrypt(&ctx, out, internal_iv, 1) != 0) {
      generic_aes_ecb_free(&ctx);
      return;
    }
    aes_increment_iv(internal_iv);
  }

  if (outlen % IV_SIZE) {
    uint8_t last_block[IV_SIZE];
    if (generic_aes_ecb_encrypt(&ctx, last_block, internal_iv, 1) != 0) {
      generic_aes_ecb_free(&ctx);
      return;
    }
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

#if defined(HAVE_ARM_AES)
  if (CPU_SUPPORTS_ARM_AES) {
    switch (seclvl) {
    case 256:
      prg_arm_aes_256(key, internal_iv, out, outlen);
      return;
    case 192:
      prg_arm_aes_192(key, internal_iv, out, outlen);
      return;
    default:
      prg_arm_aes_128(key, internal_iv, out, outlen);
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

#if defined(HAVE_ARM_AES)
  if (CPU_SUPPORTS_ARM_AES) {
    switch (seclvl) {
    case 256:
      prg_arm_aes_256(key, internal_iv, out, 256 * 2 / 8);
      return;
    case 192:
      prg_arm_aes_192(key, internal_iv, out, 192 * 2 / 8);
      return;
    default:
      prg_arm_aes_128(key, internal_iv, out, 128 * 2 / 8);
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

#if defined(HAVE_ARM_AES)
  if (CPU_SUPPORTS_ARM_AES) {
    switch (seclvl) {
    case 256:
      prg_arm_aes_256(key, internal_iv, out, 256 * 4 / 8);
      return;
    case 192:
      prg_arm_aes_192(key, internal_iv, out, 192 * 4 / 8);
      return;
    default:
      prg_arm_aes_128(key, internal_iv, out, 128 * 4 / 8);
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
      store_word(w, round_keys.round_keys[i / 4][i % 4]);
      w += sizeof(aes_word_t);
    }

    const unsigned int S_ke = params->Ske;
    for (unsigned int j = 0, ik = nk; j < S_ke / 4; ++j) {
      store_word(w, round_keys.round_keys[ik / 4][ik % 4]);
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
    // liboqs scan-build: zero-init (NOT a known runtime bug). load_state fills
    // only `block_words` columns of the max-sized block, which the analyzer
    // cannot correlate with the equally bounded reads in the round functions.
    aes_block_t state = {0};
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
    aes_block_t state = {0}; // liboqs scan-build: zero-init; see witness note above
    load_state(state, buf, AES_BLOCK_WORDS);

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

#if defined(OQS)
int generic_aes_ecb_new(generic_aes_ecb_t* ctx, const uint8_t* key, unsigned int seclvl) {
  assert(ctx && key);

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
  assert(ctx && ciphertext && plaintext);

  if (!ctx->sched) {
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
#elif defined(HAVE_OPENSSL)
int generic_aes_ecb_new(generic_aes_ecb_t* context, const uint8_t* key, unsigned int seclvl) {
  assert(context && key);

  context->ctx = NULL;

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
  assert(ctx && ciphertext && plaintext);

  int len = 0;
  int ret = EVP_EncryptUpdate(ctx->ctx, ciphertext, &len, plaintext, blocks * IV_SIZE);
  if (ret != 1 || len != (int)blocks * IV_SIZE) {
    return -1;
  }
  return 0;
}

void generic_aes_ecb_free(generic_aes_ecb_t* ctx) {
  if (ctx) {
    EVP_CIPHER_CTX_free(ctx->ctx);
  }
}
#elif defined(_WIN32)
int generic_aes_ecb_new(generic_aes_ecb_t* ctx, const uint8_t* key, unsigned int seclvl) {
  assert(ctx && key);

  ctx->aes_handle = NULL;
  ctx->key_handle = NULL;

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
  assert(ctx && ciphertext && plaintext);

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
  if (ctx) {
    BCryptDestroyKey(ctx->key_handle);
    BCryptCloseAlgorithmProvider(ctx->aes_handle, 0);
  }
}
#else
int generic_aes_ecb_new(generic_aes_ecb_t* ctx, const uint8_t* key, unsigned int seclvl) {
  assert(ctx && key);

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
  assert(ctx && ciphertext && plaintext);

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
  if (ctx) {
    faest_explicit_bzero(&ctx->round_keys, sizeof(ctx->round_keys));
  }
}
#endif
