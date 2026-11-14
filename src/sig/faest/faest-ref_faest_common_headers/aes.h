/*
 *  SPDX-License-Identifier: MIT
 */

#ifndef FAEST_AES_H
#define FAEST_AES_H

#include "macros.h"
#include "instances.h"

#include <stdint.h>
#include <stdlib.h>

#if defined(HAVE_OPENSSL)
#include <openssl/evp.h>
#elif defined(OQS)
#include <oqs/aes.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

FAEST_BEGIN_C_DECL

#define AES_ROUNDS_128 10
#define AES_ROUNDS_192 12
#define AES_ROUNDS_256 14
#define AES_MAX_ROUNDS 14

typedef uint32_t aes_word_t;
// round key with 4 (AES) up to 8 (Rijndael-256) units
// TODO: aes_round_key_t[8] should this be 8 ?
typedef aes_word_t aes_round_key_t[8];

// # of rows
#define AES_NR 4

// AES round constants
static const uint8_t aes_round_constants[30] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
    0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91,
};

// block with 4 (AES) up to 8 (Rijndael-256) units
#define MAX_AES_BLOCK_WORDS 8
typedef aes_word_t aes_block_t[MAX_AES_BLOCK_WORDS];

typedef struct {
  aes_round_key_t round_keys[AES_MAX_ROUNDS + 1];
} aes_round_keys_t;

void aes128_init_round_keys(aes_round_keys_t* round_key, const uint8_t* key);
void aes192_init_round_keys(aes_round_keys_t* round_key, const uint8_t* key);
void aes256_init_round_keys(aes_round_keys_t* round_key, const uint8_t* key);
void rijndael192_init_round_keys(aes_round_keys_t* round_key, const uint8_t* key);
void rijndael256_init_round_keys(aes_round_keys_t* round_key, const uint8_t* key);

#if defined(FAEST_TESTS)
void aes128_encrypt_block(const aes_round_keys_t* key, const uint8_t* plaintext,
                          uint8_t* ciphertext);
void aes192_encrypt_block(const aes_round_keys_t* key, const uint8_t* plaintext,
                          uint8_t* ciphertext);
void aes256_encrypt_block(const aes_round_keys_t* key, const uint8_t* plaintext,
                          uint8_t* ciphertext);
#endif
void rijndael192_encrypt_block(const aes_round_keys_t* key, const uint8_t* plaintext,
                               uint8_t* ciphertext);
void rijndael256_encrypt_block(const aes_round_keys_t* key, const uint8_t* plaintext,
                               uint8_t* ciphertext);

#if defined(FAEST_TESTS)
void aes_increment_iv(uint8_t* iv);
#endif

void aes_extend_witness_7(uint8_t* w, const uint8_t* key, const uint8_t* in,
                          const faest_paramset_t* params);

void expand_key(aes_round_keys_t* round_keys, const uint8_t* key, unsigned int key_words,
                unsigned int block_words, unsigned int num_rounds);

void prg(const uint8_t* key, const uint8_t* iv, uint32_t tweak, uint8_t* out, unsigned int bits,
         size_t outlen);
void prg_2_lambda(const uint8_t* key, const uint8_t* iv, uint32_t tweak, uint8_t* out,
                  unsigned int bits);
void prg_4_lambda(const uint8_t* key, const uint8_t* iv, uint32_t tweak, uint8_t* out,
                  unsigned int bits);

typedef struct {
#if defined(HAVE_OPENSSL)
  EVP_CIPHER_CTX* ctx;
#elif defined(OQS)
  void* sched;
  unsigned int seclvl;
#elif defined(_WIN32)
  BCRYPT_ALG_HANDLE aes_handle;
  BCRYPT_KEY_HANDLE key_handle;
#else
  aes_round_keys_t round_keys;
  unsigned int seclvl;
#endif
} generic_aes_ecb_t;

int generic_aes_ecb_new(generic_aes_ecb_t* ctx, const uint8_t* key, unsigned int seclvl);
int generic_aes_ecb_encrypt(generic_aes_ecb_t* ctx, uint8_t* ciphertext, const uint8_t* plaintext,
                            size_t blocks);
void generic_aes_ecb_free(generic_aes_ecb_t* ctx);

FAEST_END_C_DECL

#endif
